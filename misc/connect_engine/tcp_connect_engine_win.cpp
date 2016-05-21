#include <WinSock2.h>
#include <Windows.h>
#include "tcp_connect_engine_win.h"
#pragma comment(lib, "Ws2_32.lib")

class engine_helper_2011_10_27
{
public:
	engine_helper_2011_10_27()
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;
		wVersionRequested = MAKEWORD( 2, 2 );
		err = WSAStartup( wVersionRequested, &wsaData );
	}
};

engine_helper_2011_10_27 g_engine_helper_2011_10_27;

#define CHECK_VALID  if(id.index>=m_con_vector.size() || id.index < 0)return false;if(!m_con_vector[id.index])return false


tcp_connect_engine_win::tcp_connect_engine_win(void):base_thread("tcp_connect_engine_win")
{
	memset(&m_init_item, 0, sizeof(m_init_item));
	m_is_init = false;
	m_is_run = false;
	m_handle = INVALID_HANDLE_VALUE;
	m_last_index = 0;
}

tcp_connect_engine_win::~tcp_connect_engine_win(void)
{
}

bool tcp_connect_engine_win::init(engine_init_item init_item)
{
	if(m_is_init)
		return false;
	memcpy(&m_init_item, &init_item, sizeof(m_init_item));
	
	if(m_init_item.max_connects <= 0 || m_init_item.max_connects>20000)
		return false;
	

	m_con_vector.resize(m_init_item.max_connects);
	for(int i=0; i<m_init_item.max_connects; i++)
		m_con_vector[i] = 0;

	m_con_thread.init(m_init_item.con_threads, 1024*1024, 1024*200, 1024*1024*2, 1024);
	m_con_thread.set_service(this);

	m_buff_pool.init(m_init_item.max_package_size*4, m_init_item.init_mem_pool_size, m_init_item.step_mem_pool_size);

	m_is_init = true;
	return true;
}

bool tcp_connect_engine_win::fini()
{
	if(!m_is_init || m_is_run)
		return false;
	m_is_init = false;

	for(int i=0; i<m_init_item.max_connects; i++)
	{
		if(m_con_vector[i])
		{
			delete m_con_vector[i];
			m_con_vector[i] = 0;
		}
	}
	m_con_vector.clear();
	m_con_vector.resize(0);
	m_buff_pool.fini();

	return true;
}

bool tcp_connect_engine_win::start()
{
	if(!m_is_init || m_is_run)
		return false;
	m_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 4);
	if(!m_handle)
		return false;

	if(!m_con_thread.start())
		return false;

    m_is_run = true;
	if(!activate(m_init_item.rs_threads))
    {
        m_is_run = false;
		return false;
    }

	return true;
}

bool tcp_connect_engine_win::stop()
{
	if(!m_is_run)
		return false;
	m_is_run = false;
	if(m_handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_handle);
		m_handle = INVALID_HANDLE_VALUE;
	}
	m_con_thread.stop();
	return true;
}

bool tcp_connect_engine_win::pause()
{
	return false;
}

bool tcp_connect_engine_win::resume()
{
	return false;
}

bool tcp_connect_engine_win::get_connect(socket_id& id, i_tcp_socket_con_service* p_service,
						 const char* p_heart_test_data, int heart_test_len)
{
	guard g(m_vec_mutex);
	if(!m_is_run)
		return false;

	for(int i=m_last_index; i<m_init_item.max_connects; i++)
	{
		if(!m_con_vector[i])
		{
			m_con_vector[i] = new tcp_connector();
			m_con_vector[i]->init(i, m_handle, &m_buff_pool);
			m_con_vector[i]->set_isuse(true);
			m_con_vector[i]->set_service(p_service);
			id = m_con_vector[i]->get_sid();
			m_last_index = i + 1;
			return true;
		}
		else
		{
			if(!m_con_vector[i]->is_use())
			{
				m_con_vector[i]->set_isuse(true);
				m_con_vector[i]->set_service(p_service);
				id = m_con_vector[i]->get_sid();
				return true;
			}
		}
	}

	return false;
}

bool tcp_connect_engine_win::free_connect(socket_id id)
{
	CHECK_VALID;
	m_con_vector[id.index]->set_isuse(false);
	return true;
}

bool tcp_connect_engine_win::connect(socket_id id, const char* ip, unsigned short port)
{
	CHECK_VALID;
	m_con_thread.connect(m_con_vector[id.index], id.index, ip, port);
	return true;
}

void tcp_connect_engine_win::on_connect_result(int index, tcp_connector* p_con, bool result)
{
	p_con->service()->on_connect(p_con->get_sid(), result);
}

bool tcp_connect_engine_win::connect(socket_id id, unsigned int ip, unsigned short port)
{
	CHECK_VALID;
	return false;
}

bool tcp_connect_engine_win::close_socket(socket_id id)
{
	CHECK_VALID;
	m_con_vector[id.index]->close();
	return true;
}

bool tcp_connect_engine_win::send(socket_id id, void* p_data, int data_len)
{
	CHECK_VALID;
	m_con_vector[id.index]->send(p_data, data_len);
	return true;
}

bool tcp_connect_engine_win::set_data(socket_id id, void* p)
{
	CHECK_VALID;
	m_con_vector[id.index]->set_data(p);
	return true;
}

bool tcp_connect_engine_win::get_data(socket_id id, void** p)
{
	CHECK_VALID;
	*p = m_con_vector[id.index]->get_data();
	return ((*p) != 0);
}

bool tcp_connect_engine_win::is_online(socket_id id)
{
	CHECK_VALID;
	return m_con_vector[id.index]->is_connected();
}

int tcp_connect_engine_win::get_online_connections()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
int tcp_connect_engine_win::svc()
{
	DWORD dw_size = 0;
	OVERLAPPEDSTRUCT_A* p_overlapped = 0;
	tcp_connector* p_con = 0;

	while(m_is_run)
	{
		try
		{
			dw_size = 0;
			p_overlapped = 0;
			p_con = 0;
			BOOL b = GetQueuedCompletionStatus(m_handle, &dw_size, 
				(PULONG_PTR)&p_con, 
				(LPOVERLAPPED*)&p_overlapped, INFINITE);
			if(!m_is_run)
				break;
			if(!p_overlapped)
            {
                Sleep(1);
				continue;
            }
			if(!p_con)
				continue;
			switch(p_overlapped->operation_type)
			{
			case SOCKET_REV:
				{
					if (!p_con->on_recv(dw_size))
					{
						p_con->close();
					}
				}
				break;
			case SOCKET_SND:
				{
					if(!p_con->on_send(dw_size))
					{
						p_con->close();
					}
				}
				break;
			default:
				break;
			}
		}
		catch(...)
		{
			printf("\ntcp_connect_engine_win exception.\n");
		}
	}

	return 0;
}
