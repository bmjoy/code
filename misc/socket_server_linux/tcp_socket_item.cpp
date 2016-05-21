#include "tcp_socket_item.h"
#include <string.h>
#include <arpa/inet.h>

unsigned short tcp_socket_item::m_handleid = 0;

tcp_socket_item::tcp_socket_item()
{
	m_time_stamp = 0;
	m_fd = -1;
	m_sid = INVALID_SOCKET_ID;
	m_p_buff_pool = 0;
	m_p_service = 0;
	m_p_recv_thread = 0;
	m_p_send_thread = 0;
	m_p_send_head = 0;
	m_p_send_tail = 0;
	m_p_recv_buff = 0;
	m_time_out_counter = 0;
#ifdef STURN_SPORT
	is_sturn_ok = false;
#endif

}

tcp_socket_item::~tcp_socket_item()
{
}

void tcp_socket_item::init(en_epoll* recv_thread, block_buffer_pool* buff_pool)
{
	m_p_buff_pool = buff_pool;
	m_p_recv_thread = recv_thread;
	m_sid = INVALID_SOCKET_ID;
}

void tcp_socket_item::set_service(i_tcp_socket_service* p_service)
{
	m_p_service = p_service;
}

void tcp_socket_item::on_connect(int fd, en_epoll* send_thread, sockaddr_in addr)
{
	do
	{
		//guard gs(m_send_mutex);
		//guard gr(m_recv_mutex);
		guard g(m_mutex);
		if(m_sid != INVALID_SOCKET_ID)
		{
			close_socket(m_sid, true);
		}
		m_p_send_thread = send_thread;
	
		m_address = addr;
		m_fd = fd;
		m_sid.index = fd;
		m_sid.handle = m_handleid++;
	
		m_p_recv_buff = m_p_buff_pool->get();
		m_p_send_head = m_p_buff_pool->get();
		m_p_send_tail = m_p_send_head;
	
		m_p_recv_thread->add(fd, EPOLLIN|EPOLLET);
		m_p_send_thread->add(fd, EPOLLOUT|EPOLLET);
	
		m_time_stamp = time(0);
		m_time_out_counter = 0;
	#ifdef STURN_SPORT
		is_sturn_ok = false;
	#endif
	}while(false);
	bool b;

	m_p_service->on_connect(m_sid, m_address.sin_addr.s_addr, m_address.sin_port, b);
}

int tcp_socket_item::close_socket(socket_id sid, bool notify)
{
	do
	{
		//guard gs(m_send_mutex);
		//guard gr(m_recv_mutex);
		guard g(m_mutex);
		if(m_sid != sid)
		{
			return 0;
		}
		if(m_fd>=0)
		{
			m_p_recv_thread->del(m_fd, 0);
			if(m_p_send_thread)
			{
				m_p_send_thread->del(m_fd, 0);
			}
			close(m_fd);
			m_fd = -1;
		}
		m_sid = INVALID_SOCKET_ID;
		if(m_p_recv_buff)
		{
			m_p_buff_pool->put(m_p_recv_buff);
			m_p_recv_buff = 0;
		}
		while(m_p_send_head)
		{
			block_buffer* p = m_p_send_head;
			m_p_send_head = m_p_send_head->next();
			m_p_buff_pool->put(p);
		}
		m_p_send_tail = 0;
		m_p_data = 0;
		m_time_stamp = 0;
	
	} while (false);
	if(notify)
	{
        char ip[32] = {0};
        strcpy(ip, inet_ntoa(m_address.sin_addr));
		m_p_service->on_close(sid, ip);
	}
	return 0;
}

int tcp_socket_item::close_socket(bool notify)
{
	return close_socket(m_sid, notify);
}

bool tcp_socket_item::get_data(socket_id sid, void * *p)
{
	if(m_sid != sid)
	{
		return false;
	}
	*p = m_p_data;
	return true;
}

bool tcp_socket_item::set_data(void * p)
{
	m_p_data = p;
	return true;
}

bool tcp_socket_item::is_online(socket_id id)
{
	return (m_fd>0 && m_sid==id);
}

bool tcp_socket_item::is_online()
{
	return (m_fd>0);
}

bool tcp_socket_item::check_timeout(time_t ct, int timeout, int counter)
{
	//printf("m_time_out_counter=%d, timeout=%d counter=%d\n", m_time_out_counter, timeout, counter);
	int n = ct - m_time_stamp;
	if(n > timeout)
	{
		m_time_out_counter ++;
		if(m_time_out_counter > counter)
		{
			return false;
		}
		m_time_stamp = ct;
	}
	return true;
}

bool tcp_socket_item::get_address(socket_id sid, sockaddr_in &addr)
{
	if(!is_online(sid))
	{
		return false;
	}
	addr = m_address;
	return true;
}

////////////////////////////////////////////////////////////////

int tcp_socket_item::send(const void* p_data, int len)
{
	//guard g(m_send_mutex);
	guard g(m_mutex);
	if(!p_data || len<=0)
		return -1;
	if(!m_p_send_thread)
		return -1;
	int wlen = 0;
	while(wlen<len)
	{
		int rl = m_p_send_tail->rest_free_len();
		int wl = len-wlen;
		int l = min(rl, wl);
		m_p_send_tail->write(&((char*)p_data)[wlen], l);
		wlen += l;
		if(wlen < len)
		{
			block_buffer* p = m_p_buff_pool->get();
			if(!p)
				return -1;
			m_p_send_tail->next(p);
			m_p_send_tail = p;
		}
	}

	m_p_send_thread->modify(m_fd, EPOLLOUT | EPOLLET);
	return len;
}

int tcp_socket_item::do_recv_event()
{
	unsigned int result = on_recv();
	if(result == 0)
	{
		m_p_recv_thread->modify(m_fd, EPOLLIN|EPOLLET);
	}
	else
	{
		close_socket(m_sid, true);
	}
	return 0;
}

int tcp_socket_item::do_send_event()
{
	unsigned int result = on_send();
	if(result == 0)
	{
		;//m_p_send_thread->del(m_fd, 0);
	}
	else
	{
		m_p_send_thread->modify(m_fd, EPOLLOUT|EPOLLET);
	}
	return 0;
}

unsigned int tcp_socket_item::on_recv()
{
	//guard g(m_recv_mutex);
	guard g(m_mutex);
	m_time_stamp = time(0);
	int n_recv = 0;
	int n_rest_free = 0;

	do
	{
		n_rest_free = m_p_recv_buff->rest_free_len();
		n_recv = recv(m_fd, m_p_recv_buff->wr_ptr(), m_p_recv_buff->rest_free_len(), 0);

		if(n_recv == 0)
			return -1;
		if(n_recv < 0)
		{
			if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
				return 0;
		}
		m_p_recv_buff->wr_ptr(n_recv);
#ifdef STURN_SPORT
		if(!is_sturn_ok)
		{
			int l = m_p_service->on_sturn(m_sid, m_p_recv_buff->rd_ptr(), m_p_recv_buff->rest_data_len());
			if(l < 0 || l > m_p_recv_buff->rest_data_len())
				return -1;
			if(l == 0 && n_recv == n_rest_free)
				return -1;
			if (l == 0)
			{
				return 0;
			}
			m_p_recv_buff->rd_ptr(l);
			is_sturn_ok = true;
		}
#endif
		while (m_p_recv_buff->rest_data_len()>0)
		{
			int l = m_p_service->check_pack(m_sid, m_p_recv_buff->rd_ptr(), m_p_recv_buff->rest_data_len());

			if(l < 0 || l > m_p_recv_buff->rest_data_len())
				return -1;
			if(l == 0 && n_recv == n_rest_free)
				return -1;
			if(l == 0)
				break;
			if(!m_p_service->on_recive(m_sid, m_p_recv_buff->rd_ptr(), l))
				return -1;
			m_p_recv_buff->rd_ptr(l);
		}
		m_p_recv_buff->arrange_data();
	} while (n_recv == n_rest_free);
	return 0;
}

unsigned int tcp_socket_item::on_send()
{
	//guard g(m_send_mutex);
	guard g(m_mutex);
	if(!m_p_send_head)
		return 0;
	if(m_p_send_head->rest_data_len() == 0)
	{
		if(m_p_send_head == m_p_send_tail)
		{
			m_p_send_head->reset();
			return 0;
		}
		block_buffer* p = m_p_send_head;
		m_p_send_head = m_p_send_head->next();
		m_p_buff_pool->put(p);
	}
	int slen = 0;
	slen = write(m_fd, m_p_send_head->rd_ptr(), m_p_send_head->rest_data_len());
	while(slen > 0)
	{
		m_p_send_head->rd_ptr(slen);
		if(m_p_send_head->rest_data_len() == 0)
		{
			if(m_p_send_head == m_p_send_tail)
			{
				m_p_send_head->reset();
				break;
			}
			block_buffer* p = m_p_send_head;
			m_p_send_head = m_p_send_head->next();
			m_p_buff_pool->put(p);
		}
		slen = write(m_fd, m_p_send_head->rd_ptr(), m_p_send_head->rest_data_len());
	}
	if(slen < 0)
	{
		if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
		{
			return EPOLLOUT;
		}
	}
	return 0;
}

