#include "tcp_connector.h"


tcp_connector::tcp_connector(void)
{
	m_isuse = false;
	m_service = 0;
	m_sid = INVALID_SOCKET_ID;
	m_handle = INVALID_HANDLE_VALUE;
	m_socket = INVALID_SOCKET;
	
	memset(&m_sendoverdata, 0, sizeof(m_sendoverdata));
	memset(&m_sendbuffer, 0, sizeof(m_sendbuffer));
	m_sendoverdata.operation_type = SOCKET_SND;
	m_psendblockhead = 0;
	m_psendblocktail = 0;
	m_bsending = false;

	memset(&m_recvoverdata, 0, sizeof(m_recvoverdata));
	memset(&m_readbuffer, 0, sizeof(m_readbuffer));
	m_recvoverdata.operation_type = SOCKET_REV;
	m_precvblock = 0;

	m_pdata = 0;
}

tcp_connector::~tcp_connector(void)
{
}

void tcp_connector::init(int index, HANDLE h, block_buffer_pool* pool)
{
	m_sid.index = index;
	m_handle = h;
	m_p_pool = pool;
}

bool tcp_connector::set_isuse(bool b)
{
	guard gs(m_sendmutex);
	guard gr(m_recvmutex);
	if(m_isuse == b)
		return false;
	m_isuse = b;
	m_pdata = 0;
	return true;
}

bool tcp_connector::is_use()
{
	return m_isuse;
}

socket_id tcp_connector::get_sid()
{
	return m_sid;
}

void tcp_connector::set_data(void* p)
{
	m_pdata = p;
}

void* tcp_connector::get_data()
{
	return m_pdata;
}

i_tcp_socket_con_service* tcp_connector::service()
{
	return m_service;
}

void tcp_connector::set_service(i_tcp_socket_con_service* p_service)
{
	m_service = p_service;
}

bool tcp_connector::connect(const char* ip, unsigned short port)
{
	bool is_error = true;

	if(is_connected())
	{
		close();
		Sleep(100);
	}

	do 
	{
		SOCKADDR_IN		saServer;
		saServer.sin_port = htons(port);
		saServer.sin_family = AF_INET;
		saServer.sin_addr.s_addr =inet_addr(ip);

		m_socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
        //struct linger linger;
		////////////////////////////////////////////////////////////////////////////
		u_long nonblockingmode = TRUE;
		ioctlsocket(m_socket, FIONBIO , &nonblockingmode);
		int TimeOut=1000; //设置发送超时
		if(::setsockopt(m_socket,SOL_SOCKET,SO_SNDTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
		{
			break;
		}
		TimeOut=1000;//设置接收超时
		if(::setsockopt(m_socket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
		{
			break;
		}
		////////////////////////////////////////////////////////////////////////////
		// 与服务器连接
		int nRet;
		nRet = ::connect(m_socket, (const sockaddr*)&saServer, sizeof(saServer));

		if(nRet == SOCKET_ERROR)
		{
			DWORD dwError;
			dwError = GetLastError();
			if(dwError != WSAEWOULDBLOCK)
			{
				break;
			}
		}

		//select 模型，即设置超时
		struct timeval timeout ;
		fd_set w;
		FD_ZERO(&w);
		FD_SET(m_socket, &w);
		timeout.tv_sec = 1; //连接超时1秒
		timeout.tv_usec =0;
		nRet = select(0, NULL, &w, 0, &timeout);

		if ( nRet <= 0 )
		{
			break;
		}
		HANDLE h = CreateIoCompletionPort((HANDLE)m_socket, m_handle,(ULONG_PTR)this, 1);
		if(h == 0)
		{
			break;
		}
		if(!do_recv())
			break;
		is_error = false;

	} while (false);
	if(is_error)
	{
		close();
		return false;
	}
	return true;
}

bool tcp_connector::close()
{
	guard gs(m_sendmutex);
	guard gr(m_recvmutex);

	if(m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	if(m_precvblock)
	{
		release_block(m_precvblock);
		m_precvblock = 0;
	}
	while(m_psendblockhead)
	{
		block_buffer* p = m_psendblockhead;
		m_psendblockhead = m_psendblockhead->next();
		release_block(p);
	}

	m_psendblockhead = 0;
	m_psendblocktail = 0;

	m_service->on_close(m_sid);

	return false;
}

bool tcp_connector::send(void* pdata, int len)
{
	guard gs(m_sendmutex);
	if(!pdata || len <= 0)
		return false;
	if(!m_psendblocktail)
	{
		m_psendblocktail = new_block();
		if(!m_psendblocktail)
			return false;
		m_psendblockhead = m_psendblocktail;
	}
	int w_len = 0;
	while(w_len < len)
	{
		int rl = m_psendblocktail->rest_free_len();
		int wl = len - w_len;
		int l = min(rl, wl);
		m_psendblocktail->write(&((char*)pdata)[w_len], l);
		w_len += l;
		if(w_len < len)
		{
			block_buffer* p = new_block();
			if(!p)
				return false;
			m_psendblocktail->next(p);
			m_psendblocktail = p;	
		}
	}
	return do_send();
}

bool tcp_connector::on_recv(int size)
{
	guard gr(&m_recvmutex);
	if(!m_precvblock)
		m_precvblock = new_block();
	if(!m_precvblock)
		return false;

	int n = ::recv(m_socket, m_precvblock->wr_ptr(), m_precvblock->rest_free_len(), 0);
	if(n <= 0)
	{
		int err =  WSAGetLastError();
		if (WSAEWOULDBLOCK == err)
			return true;
		return false;
	}
	m_precvblock->wr_ptr(n);
	int tl = m_precvblock->rest_data_len();

	while (m_precvblock->rest_data_len()>0) 
	{
		int l = m_service->check_pack(m_sid, m_precvblock->rd_ptr(), m_precvblock->rest_data_len());
		if(l < 0 || l > m_precvblock->rest_data_len())
			return false;
		if(l == 0)
			break;
		if(!m_service->on_recive(m_sid, m_precvblock->rd_ptr(), l))
			return false;
		if(!m_precvblock)
			return false;
		m_precvblock->rd_ptr(l);
	}
	m_precvblock->arrange_data();
	return do_recv();
}

bool tcp_connector::on_send(int size)
{
	guard gs(&m_sendmutex);
	m_bsending = false;
	if(size <= 0)
		return false;
	
	if(!m_psendblockhead)
		return true;

	m_psendblockhead->rd_ptr(size);
	if(m_psendblockhead->rest_data_len() > 0)
		return do_send();
	
	if(m_psendblockhead != m_psendblocktail)
	{
		block_buffer* p = m_psendblockhead;
        m_psendblockhead = m_psendblockhead->next();
		release_block(p);
		return do_send();
	}
	else
	{
		m_psendblockhead->reset();
	}

	return true;
}

bool tcp_connector::is_connected()
{
	return (m_socket != INVALID_SOCKET);
}

bool tcp_connector::do_recv()
{
	DWORD dwThancferred = 0;
	DWORD dwFlags = 0;

	m_readbuffer.buf = 0;
	m_readbuffer.len = 0;

	int ret = ::WSARecv(m_socket,&m_readbuffer,1,&dwThancferred,&dwFlags,(LPWSAOVERLAPPED)&m_recvoverdata,NULL);

	if (ret && (::WSAGetLastError()!=WSA_IO_PENDING))
	{
		return false;
	}
	
	return true;
}

bool tcp_connector::do_send()
{
	if(!m_psendblockhead)
		return true;
	if(m_bsending || m_psendblockhead->rest_data_len() == 0)
		return true;
	m_bsending = true;

	m_sendbuffer.buf = m_psendblockhead->rd_ptr();
	m_sendbuffer.len = m_psendblockhead->rest_data_len();

	DWORD dwThancferred = 0;

	if ((::WSASend(m_socket, &m_sendbuffer, 1, &dwThancferred, 0,
		(LPWSAOVERLAPPED)&m_sendoverdata, NULL)
		==SOCKET_ERROR)
		&& (::WSAGetLastError()!=WSA_IO_PENDING))
	{
		m_bsending = false;
		return false;
	}

	return true;

}

block_buffer* tcp_connector::new_block()
{
	if(!m_p_pool)
		return 0;

	block_buffer* p = m_p_pool->get();
	return p;
}

void tcp_connector::release_block(block_buffer* p)
{
	if(m_p_pool)
		m_p_pool->put(p);
}