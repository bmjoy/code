#include "tcp_socket_item.h"

unsigned short tcp_socket_item::handleid = 1;

thread_mutex handle_id_mutex;
unsigned short tcp_socket_item::make_handle_id()
{
    guard g(handle_id_mutex);
	handleid ++;
	if(handleid > 60000)
		handleid = 1;
	return handleid;
}

tcp_socket_item::tcp_socket_item()
{
	m_socket = INVALID_SOCKET;

	m_p_recv_buff = 0;
	m_p_send_tail = 0;
	m_p_send_head = 0;

	memset(&m_SendOverData, 0, sizeof(m_SendOverData));
	memset(&m_RecvOverData, 0, sizeof(m_RecvOverData));

	m_RecvOverData.uOperationType = SOCKET_REV;
	m_SendOverData.uOperationType = SOCKET_SND;

    p_service = 0;
    buff_pool = 0;
    max_package_count_per_second = 0;

    m_package_count = 0;
    m_package_timespame = 0;
}

tcp_socket_item::~tcp_socket_item()
{

}

void tcp_socket_item::set_index(unsigned short index)
{
	m_sid.index = index;
	m_sid.handle= 0;
}

bool tcp_socket_item::attach(SOCKET s, sockaddr_in addr)
{
    //guard gr(m_recv_mutex);
    //guard gs(m_send_mutex);
    guard g(m_mutex);

	m_socket = s;
	m_sid.handle = make_handle_id();

	m_addr = addr;

	m_last_time_stamp = GetTickCount();
	m_time_out_count = 0;
	m_batch_send = false;
	m_b_sending = false;
    m_p_data = 0;
    m_package_count = 0;
    m_package_timespame = 0;
	return recv();
}

SOCKET tcp_socket_item::get_socket()
{
	return m_socket;
}

bool tcp_socket_item::is_online(unsigned short h)
{
	if(h != m_sid.handle)
		return false;
	return is_online();
}

bool tcp_socket_item::check_time_out(unsigned int t, unsigned int dt, int c)
{
	if((t - m_last_time_stamp) > dt*1000)
	{
		m_time_out_count ++;
		if(m_time_out_count >= c)
			return false;
		m_last_time_stamp = t;
	}
	return true;
}

void tcp_socket_item::close()
{
	//guard gr(m_recv_mutex);
	//guard gs(m_send_mutex);
    guard g(m_mutex);
	if(m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	if (m_p_recv_buff)
	{
		buff_pool->put(m_p_recv_buff);
		m_p_recv_buff = 0;
	}
	while(m_p_send_head)
	{
		block_buffer* p = m_p_send_head;
		m_p_send_head = m_p_send_head->next();
		buff_pool->put(p);
	}
	m_p_send_head = 0;
	m_p_send_tail = 0;
	m_sid.handle = 0;
}

bool tcp_socket_item::get_addr(struct sockaddr_in& addr)
{
	addr = m_addr;
	return true;
}

bool tcp_socket_item::set_data(socket_id sid, void* pdata)
{
	if(!is_online(sid.handle))
		return false;
	m_p_data = pdata;
	return true;
}

bool tcp_socket_item::get_data(socket_id sid, void** pdata)
{
	if(!is_online(sid.handle))
		return false;
	*pdata = m_p_data;
	return true;
}

bool tcp_socket_item::is_online()
{
	return (m_socket != INVALID_SOCKET);
}

socket_id tcp_socket_item::get_socket_id()
{
	return m_sid;
}

void tcp_socket_item::set_service(i_tcp_socket_service* service)
{
    p_service = service;
}

void tcp_socket_item::set_buff_pool(block_buffer_pool* pool)
{
    buff_pool = pool;
}

void tcp_socket_item::set_max_package_count(int n)
{
    max_package_count_per_second = n;
}

bool tcp_socket_item::is_send_batch()
{
    return m_batch_send;
}

void tcp_socket_item::set_send_batch(bool b)
{
    m_batch_send = b;
}

bool tcp_socket_item::recv()
{
	DWORD dwThancferred = 0;
	DWORD dwFlags = 0;
	m_RecvOverData.HandleID = m_sid.handle;
	m_ReadBuffer.buf = 0;
	m_ReadBuffer.len = 0;

	int ret = ::WSARecv(m_socket,&m_ReadBuffer,1,&dwThancferred,&dwFlags,(LPWSAOVERLAPPED)&m_RecvOverData,NULL);

	if (ret && (::WSAGetLastError()!=WSA_IO_PENDING))
	{
		return false;
	}
	return true;
}

bool tcp_socket_item::on_recv(int size, unsigned short handle)
{
	//guard gr(m_recv_mutex);
    guard g(m_mutex);
	if(handle != m_sid.handle)
		return false;
	if(!m_p_recv_buff)
	{
		m_p_recv_buff = buff_pool->get();
		if(!m_p_recv_buff)
			return false;
	}
	m_last_time_stamp = GetTickCount();
	m_time_out_count = 0;
    /// ¼ì²â·¢°üÆµÂÊ
    if(max_package_count_per_second > 0)
    {
        if((m_last_time_stamp-m_package_timespame) < 1000)
        {
            m_package_count ++;
            if(m_package_count > max_package_count_per_second)
                return false;
        }
        else
        {
            m_package_count = 0;
            m_package_timespame = GetTickCount();
        }
    }

	int iRevcCode = ::recv(m_socket, m_p_recv_buff->wr_ptr(), m_p_recv_buff->rest_free_len(), 0);
	if(iRevcCode <= 0)
	{
		int err =  WSAGetLastError();
		if (WSAEWOULDBLOCK == err)
			return true;
		return false;
	}
    
	m_p_recv_buff->wr_ptr(iRevcCode);
	while (m_p_recv_buff->rest_data_len()>0) 
	{
        int l = p_service->check_pack(m_sid, m_p_recv_buff->rd_ptr(), m_p_recv_buff->rest_data_len());
        
		if(l < 0 || l > m_p_recv_buff->rest_data_len())
			return false;
		if(l == 0)
			break;
        if(!p_service->on_recive(m_sid, m_p_recv_buff->rd_ptr(), l))
            return false;
        if(!m_p_recv_buff)
            return false;
		m_p_recv_buff->rd_ptr(l);
	}
	m_p_recv_buff->arrange_data();
	return recv();
}

bool tcp_socket_item::send(void* pdata, int datalen)
{
	//guard gs(m_send_mutex);
    guard g(m_mutex);
	if(!pdata || datalen==0)
		return false;
	if(!m_p_send_tail)
	{
		m_p_send_tail = buff_pool->get();
		if(!m_p_send_tail)
			return false;
		m_p_send_head = m_p_send_tail;
	}
	int w_len = 0;
	while(w_len < datalen)
	{
		int rl = m_p_send_tail->rest_free_len();
		int wl = datalen - w_len;
		int l = min(rl, wl);
		m_p_send_tail->write(&((char*)pdata)[w_len], l);
		w_len += l;
		if(w_len < datalen)
		{
			block_buffer* p = buff_pool->get();
			if(!p)
				return false;
			m_p_send_tail->next(p);
			m_p_send_tail = p;	
		}
	}
	return do_send();
}

bool tcp_socket_item::do_send()
{
	if(!m_p_send_head || m_b_sending || m_p_send_head->rest_data_len() == 0)
		return true;
	
	DWORD dwThancferred = 0;
	m_b_sending = true;
	m_SendOverData.HandleID = m_sid.handle;

	m_SendBuffer.buf = m_p_send_head->rd_ptr();
	m_SendBuffer.len = m_p_send_head->rest_data_len();

	if ((::WSASend(m_socket, &m_SendBuffer, 1, &dwThancferred, 0,
		(LPWSAOVERLAPPED)&m_SendOverData, NULL)
		==SOCKET_ERROR)
		&& (::WSAGetLastError()!=WSA_IO_PENDING))
	{
		m_b_sending = false;
		return false;
	}

	return true;
}

bool tcp_socket_item::on_send(int size, unsigned short handle)
{
	//guard gs(m_send_mutex);
    guard g(m_mutex);
	if(!m_p_send_head)
		return false;
	if(m_sid.handle != handle)
		return true;
	m_b_sending = false;
	if(size<0)
		return false;
	m_p_send_head->rd_ptr(size);
	if(m_p_send_head->rest_data_len() > 0)
		return do_send();
	if(m_p_send_head != m_p_send_tail)
	{
		block_buffer* p = m_p_send_head;
		m_p_send_head = m_p_send_head->next();
		buff_pool->put(p);
		return do_send();
	}
	m_p_send_head->reset();
	return true;
}
