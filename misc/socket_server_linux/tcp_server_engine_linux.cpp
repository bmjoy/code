#include "tcp_server_engine_linux.h"
#include "../vector_helper.h"
#include "cflog.h"
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

tcp_server_engine_linux::tcp_server_engine_linux()
{
	m_b_run = false;
	m_send_thread_index = 0;
}

tcp_server_engine_linux::~tcp_server_engine_linux()
{
}

bool tcp_server_engine_linux::init(engine_init_item init_item, i_tcp_server_service *p_service)
{
	m_init_item = init_item;
	m_p_service = p_service;
	m_init_item.max_connects += 100;
	if(!m_buffer_pool.init(m_init_item.max_package_size*2, m_init_item.init_mem_pool_size,
					   m_init_item.init_mem_pool_size))
		return false;
	m_send_thread.resize(m_init_item.rs_threads);
	m_socket_item.resize(m_init_item.max_connects);
	if(!m_listen_thread.init(m_init_item.port, this))
    {
        LOG_ERROR("m_listen_thread.init errno=%d", errno);
        return false;
    }
	m_recv_thread.init(this);
	for(int i=0; i<(int)m_send_thread.size(); i++)
	{
		m_send_thread[i].init(this);
	}
	for(int i=0; i<m_init_item.max_connects; i++)
	{
		m_socket_item[i].init(&m_recv_thread, &m_buffer_pool);
	}
	return true;
}

bool tcp_server_engine_linux::set_service(i_tcp_socket_service *p_service)
{
	for(int i=0; i<(int)m_socket_item.size(); i++)
	{
		m_socket_item[i].set_service(p_service);
	}
	return true;
}

bool tcp_server_engine_linux::fini()
{
	release_vector(m_send_thread);
	release_vector(m_socket_item);
	m_buffer_pool.fini();
	m_listen_thread.fini();
	return true;
}

bool tcp_server_engine_linux::start()
{
	if(m_b_run)
	{
		return false;
	}
	if(!m_listen_thread.start())
	{
	    LOG_ERROR("m_listen_thread.start errno=%d\n", errno);
		return false;
    }
	//sleep(1);
	if(!m_recv_thread.start())
	{
	    LOG_ERROR("m_recv_thread.start errno=%d\n", errno);
		return false;
	}
	for(int i=0; i<(int)m_send_thread.size(); i++)
	{
		if(!m_send_thread[i].start())
			return false;
	}
	m_b_run = true;
	activate();
	return true;
}

bool tcp_server_engine_linux::stop()
{
	if(!m_b_run)
	{
		return false;
	}
	m_b_run = false;
	join();
	m_listen_thread.stop();
	m_recv_thread.stop();
	for(int i=0; i<(int)m_send_thread.size(); i++)
	{
		m_send_thread[i].stop();
	}
	return true;
}

bool tcp_server_engine_linux::get_address(socket_id sid, struct sockaddr_in &addr, bool b_remote)
{
	if(sid.index >= (int)m_socket_item.size())
	{
		return false;
	}
	return m_socket_item[sid.index].get_address(sid, addr);
}

bool tcp_server_engine_linux::get_ip(socket_id sid, char* ip)
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

bool tcp_server_engine_linux::get_data(socket_id sid, void * *p)
{
	if(sid.index >= (int)m_socket_item.size())
	{
		return false;
	}

	return m_socket_item[sid.index].get_data(sid, p);
}

int tcp_server_engine_linux::get_online_connections()
{
	int size = 0;
	for(int i=0; i<(int)m_socket_item.size(); i++)
	{
		if(m_socket_item[i].is_online())
		{
			size ++;
		}
	}
	return size;
}

bool tcp_server_engine_linux::get_service_port(unsigned short &port)
{
	port = m_init_item.port;
	return true;
}

bool tcp_server_engine_linux::is_online(socket_id sid)
{
	if(sid.index >= (int)m_socket_item.size())
	{
		return false;
	}
	return m_socket_item[sid.index].is_online(sid);
}

bool tcp_server_engine_linux::pause()
{
	return true;
}

bool tcp_server_engine_linux::resume()
{
	return true;
}

bool tcp_server_engine_linux::send(socket_id sid, void *p_head, int head_len, void *p_data, int data_len)
{
	if(sid.index >= (int)m_socket_item.size() ||
	   !m_socket_item[sid.index].is_online(sid))
	{
		return false;
	}
	m_socket_item[sid.index].send(p_data, data_len);
	return true;
}

void tcp_server_engine_linux::close_socket(socket_id sid, bool b_notify)
{
	if(sid.index>=(int)m_socket_item.size())
	{
		return;
	}
	m_socket_item[sid.index].close_socket(sid, b_notify);
}

bool tcp_server_engine_linux::send_batch(void *p_head, int head_len, void *p_data, int data_len)
{
	return true;
}

void tcp_server_engine_linux::set_batch_enable(socket_id sid, bool enable)
{
	
}

bool tcp_server_engine_linux::set_data(socket_id sid, void *p)
{
	if(sid.index >= (int)m_socket_item.size() ||
	   !m_socket_item[sid.index].is_online(sid))
	{
		return false;
	}
	return m_socket_item[sid.index].set_data(p);
}

bool tcp_server_engine_linux::on_new_connect(int fd, sockaddr_in sa)
{
	if(fd >= (int)m_socket_item.size())
		return false;
	m_send_thread_index ++;
	if(m_send_thread_index<0 || m_send_thread_index>=(int)m_send_thread.size())
	{
		m_send_thread_index = 0;
	}
	m_socket_item[fd].on_connect(fd, &m_send_thread[m_send_thread_index], sa);
	return true;
}

int tcp_server_engine_linux::on_send_event(int fd)
{
	if(fd >= (int)m_socket_item.size())
	{
		return -1;
	}
	return m_socket_item[fd].do_send_event();
}

int tcp_server_engine_linux::on_recv_event(int fd)
{
	if(fd >= (int)m_socket_item.size())
	{
		return -1;
	}
	return m_socket_item[fd].do_recv_event();
}

int tcp_server_engine_linux::svc()
{
	while(true)
	{
		sleep(1);
		this->check_time_out();
	}
	return 0;
}

void tcp_server_engine_linux::check_time_out()
{
	time(0);
	time_t ct = time(0);
	for(int i=0; i<(int)m_socket_item.size(); i++)
	{
		if(m_socket_item[i].is_online())
		{
			if(!m_socket_item[i].check_timeout(ct, m_init_item.time_out, m_init_item.time_out_count))
			{
				m_socket_item[i].close_socket(true);
			}
		}
	}
}
