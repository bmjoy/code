#include "tcp_server_engine_win.h"
#pragma warning(disable:4996)
///////////////////////////////////////////////////////////////////////////////
// tcp_server_engine_win class
tcp_server_engine_win::tcp_server_engine_win(void):base_thread("tcp_server_engine_win")
{
	memset(&m_init_item, 0, sizeof(m_init_item));
    m_socket_item = 0;
	m_p_service = 0;
	m_last_index = 0;
	m_b_run = false;
}

tcp_server_engine_win::~tcp_server_engine_win(void)
{
}
///////////////////////////////////////////////////////////////////////////////
// tcp_server_engine_win operations
bool tcp_server_engine_win::init(engine_init_item init_item, i_tcp_server_service* p_service)
{
	m_p_server = p_service;

	m_init_item = init_item;

	if(!m_listen.create(this))
		return false;

	buff_pool.init(m_init_item.max_package_size+128,
			m_init_item.init_mem_pool_size, m_init_item.step_mem_pool_size);

	m_socket_item = new tcp_socket_item[m_init_item.max_connects];

	for(int i=0; i<m_init_item.max_connects; i++)
	{
		m_socket_item[i].set_index(i);
        m_socket_item[i].set_buff_pool(&buff_pool);
        m_socket_item[i].set_max_package_count(m_init_item.max_package_count_per_second);
	}

	return true;
}

bool tcp_server_engine_win::get_service_port(unsigned short& port)
{
	return true;
}

bool tcp_server_engine_win::fini()
{
	delete[] m_socket_item;
	buff_pool.fini();

	return true;
}

bool tcp_server_engine_win::set_service(i_tcp_socket_service* p_service)
{
	if(!p_service)
		return false;
	m_p_service = p_service;
    if(m_socket_item)
    {
        for(int i=0; i<m_init_item.max_connects; i++)
        {
            m_socket_item[i].set_service(p_service);
        }
    }
	return true;
}

bool tcp_server_engine_win::start()
{
	if(m_b_run)
		return false;

	m_b_run = true;
	if(!activate(m_init_item.rs_threads))
	{
		printf("tcp_server_engine_win::start : activate(m_init_item.rs_threads)");
		return false;
	}
	if(!m_check_thread.start(this))
	{
		printf("tcp_server_engine_win::start : m_check_thread.start(this)");
		return false;
	}
	if(m_p_server)
		m_p_server->on_start();
	m_last_index = 0;
	if(!m_listen.bind(m_init_item.port))
	{
		printf("tcp_server_engine_win::start : m_listen.bind(m_init_item.port)");
		return false;
	}
	if(!m_listen.listen())
	{
		printf("tcp_server_engine_win::start : m_listen.listen");
		return false;
	}
	return true;
}

bool tcp_server_engine_win::stop()
{
	if(!m_b_run)
		return false;
	m_b_run = false;
	m_check_thread.stop();
    Sleep(100);
	m_listen.close_socket();
	if(m_p_server)
		m_p_server->on_stop();
	return true;
}

bool tcp_server_engine_win::pause()
{
	return true;
}

bool tcp_server_engine_win::resume()
{
	return true;
}

bool tcp_server_engine_win::get_address(socket_id sid, struct sockaddr_in& addr, bool b_remote)
{
	if(!is_online(sid))
		return false;
	m_socket_item[sid.index].get_addr(addr);
	return true;
}

bool tcp_server_engine_win::get_ip(socket_id sid, char* ip)
{
    struct sockaddr_in addr;
    if(!ip)return false;
    if(!get_address(sid, addr, true))
    {
        return false;
    }
    strcpy(ip, inet_ntoa(addr.sin_addr));
    return true;
}


bool tcp_server_engine_win::send(socket_id sid, void* p_head, int head_len, void* p_data, int data_len)
{
	if(!is_online(sid))
		return false;
	return m_socket_item[sid.index].send(p_data, data_len);
}

bool tcp_server_engine_win::send_batch(void* p_head, int head_len, void* p_data, int data_len)
{
    for(int i=0; i<m_init_item.max_connects; i++)
    {
        if(m_socket_item[i].is_online() && m_socket_item[i].is_send_batch())
        {
            m_socket_item[i].send(p_data, data_len);
        }
    }
	return true;
}

void tcp_server_engine_win::set_batch_enable(socket_id sid, bool enable)
{
    if(!is_online(sid))
        return;
    m_socket_item[sid.index].set_send_batch(enable);
}

void tcp_server_engine_win::close_socket(socket_id sid, bool b_notify)
{
	guard g(m_item_mutex);
	if (!is_online(sid))
	{
		return;
	}
	struct sockaddr_in addr;
	m_socket_item[sid.index].get_addr(addr);
	m_socket_item[sid.index].close();
	if(b_notify)
	{
		m_p_service->on_close(sid);
	}
}

bool tcp_server_engine_win::set_data(socket_id sid, void* p)
{
	if((int)sid.index<0 || (int)sid.index >= m_init_item.max_connects)
		return false;
	return m_socket_item[sid.index].set_data(sid, p);
}

bool tcp_server_engine_win::get_data(socket_id sid, void** p)
{
	if((int)sid.index<0 || (int)sid.index >= m_init_item.max_connects)
		return false;
	return m_socket_item[sid.index].get_data(sid, p);
}

bool tcp_server_engine_win::is_online(socket_id sid)
{
	if((int)sid.index<0 || (int)sid.index >= m_init_item.max_connects)
		return false;
	return m_socket_item[sid.index].is_online(sid.handle);
}

int tcp_server_engine_win::get_online_connections()
{
	guard g(m_item_mutex);
	int count = 0;
	for(int i=0; i<m_init_item.max_connects; i++)
	{
		if(m_socket_item[i].is_online())
			count ++;
	}
	return count;
}

bool tcp_server_engine_win::on_accept(SOCKET socket, sockaddr_in addr)
{
	if(!m_b_run)
		return false;
	guard g(m_item_mutex);

	int index = -1;
	for(int i=m_last_index; i<m_init_item.max_connects; i++)
	{
		if(!m_socket_item[i].is_online())
		{
			index = i;
			break;
		}
	}
	if(index == -1)
	{
		for(int i=0; i<m_last_index; i++)
		{
			if(!m_socket_item[i].is_online())
			{
				index = i;
				break;
			}
		}
	}
	if(index == -1)
		return false;
	m_last_index = index;

	tcp_socket_item* p_item = &m_socket_item[index];

	HANDLE hAcceptCompletePort 
		= ::CreateIoCompletionPort((HANDLE)socket, m_listen.get_handle(),
		(ULONG_PTR)p_item, 0);
	if(!hAcceptCompletePort)
	{
		return false;
	}

	if(!m_socket_item[index].attach(socket, addr))
	{
		m_socket_item[index].close();
		return false;
	}
	unsigned int ip = addr.sin_addr.s_addr;
	unsigned short port = htons(addr.sin_port);
	bool b = false;
	if(!m_p_service->on_connect(p_item->get_socket_id(), ip, port, b))
	{
		return false;
	}
    if(b)
        p_item->set_send_batch(b);
	return true;
}

int tcp_server_engine_win::svc()
{
	DWORD dwTrancferred = 0;
	OverLappedStruct* pOverData = 0;
	tcp_socket_item* p_item = 0;

	while(m_b_run)
	{
		dwTrancferred = 0;
		p_item = NULL;
		pOverData = NULL;
		BOOL b = 
			::GetQueuedCompletionStatus(m_listen.get_handle(), &dwTrancferred, 
			(PULONG_PTR)&p_item, 
			(LPOVERLAPPED*)&pOverData, INFINITE);
		if(!m_b_run)
			break;
		if(!pOverData)
        {
            Sleep(1);
			continue;
        }
		switch(pOverData->uOperationType)
		{
		case SOCKET_SND:
			on_send(p_item, dwTrancferred, pOverData->HandleID);
			break;
		case SOCKET_REV:
			on_recv(p_item, dwTrancferred, pOverData->HandleID);
			break;
		}
	}

	return 0;
}

void tcp_server_engine_win::on_recv(tcp_socket_item* p_item, int size, unsigned short handle)
{
	if(!p_item)
		return;
	if(!p_item->on_recv(size, handle))
	{
		socket_id sid = p_item->get_socket_id();
		close_socket(sid, true);
	}
}

void tcp_server_engine_win::on_send(tcp_socket_item* p_item, int size, unsigned short handle)
{
	if(!p_item->on_send(size, handle))
	{
		socket_id sid = p_item->get_socket_id();
		close_socket(sid, true);
	}
}

void tcp_server_engine_win::check_timeout()
{
	guard g(m_item_mutex);
	unsigned int t = GetTickCount();
	for(int i=0; i<m_init_item.max_connects; i++)
	{
		if(m_socket_item[i].is_online())
		{
			if(!m_socket_item[i].check_time_out(t, m_init_item.time_out, m_init_item.time_out_count))
			{
				socket_id sid = m_socket_item[i].get_socket_id();
				close_socket(sid, true);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// check_time_out_thread class
check_time_out_thread::check_time_out_thread():base_thread("check_time_out_thread")
{
	m_brun = false;
}
///////////////////////////////////////////////////////////////////////////////
// check_time_out_thread operations
int check_time_out_thread::svc()
{
	while(m_brun)
	{
		Sleep(1000);
		m_p_engine->check_timeout();
	}

	return 0;
}
//---------------------------------------------------------------------------
bool check_time_out_thread::start(tcp_server_engine_win* p_engine)
{
	if(m_brun)
	{
		return false;
	}
	m_p_engine = p_engine;
	m_brun = true;
	return activate();
}
//---------------------------------------------------------------------------
void check_time_out_thread::stop()
{
	if(!m_brun)
	{
		return;
	}
	m_brun = false;
	Sleep(1100);
}
