#include "StdAfx.h"
#include "http_get_proxy_engin_win.h"
#include "i_multiplex_proxy.h"


http_get_proxy_engin_win::http_get_proxy_engin_win(void)
{
	m_is_init = false;
	m_is_run = false;
	memset(&m_init_item, 0, sizeof(m_init_item));
	m_service = 0;
	m_last_index = 0;
	m_h_complish_port = INVALID_HANDLE_VALUE;
}

http_get_proxy_engin_win::~http_get_proxy_engin_win(void)
{
}

bool http_get_proxy_engin_win::init(proxy_init_item item)
{
	if(m_is_init)
		return false;
	memcpy(&m_init_item, &item, sizeof(proxy_init_item));


	m_con.resize(m_init_item.max_connect, 0);
	m_last_index = 0;

	m_con_thread.set_service(this);
	m_con_thread.init(m_init_item.con_threads, 1024*1024, 1024*200, 1024*1024*10, 1024);

	m_is_init = true;

	return true;
}

bool http_get_proxy_engin_win::set_service(i_http_get_proxy_service* service)
{
	m_service = service;
	if(!m_service)
		return false;
	return true;
}

bool http_get_proxy_engin_win::fini()
{
	if(!m_is_init)
		return false;
	if(m_is_run)
		return false;
	for(size_t i=0; i<m_con.size(); i++)
		delete m_con[i];
	m_con.resize(0);
	m_con_thread.fini();
	m_is_init = false;
	return true;
}

bool http_get_proxy_engin_win::start()
{
	if(!m_is_init)
		return false;
	if(m_is_run)
		return false;

	m_h_complish_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 4);
	if(!m_h_complish_port)
		return false;

	m_is_run = true;
	activate(m_init_item.rs_threads);
	m_con_thread.start();
	return true;
}

bool http_get_proxy_engin_win::stop()
{
	if(!m_is_run)
		return false;
	m_is_run = false;
	if(m_h_complish_port != INVALID_HANDLE_VALUE && 
	   m_h_complish_port != 0)
	{
		CloseHandle(m_h_complish_port);
		m_h_complish_port = INVALID_HANDLE_VALUE;
	}
	m_con_thread.stop();
	Sleep(1000);
	return true;
}

int http_get_proxy_engin_win::get_free_fd()
{
	guard g(m_mutex);
	int fd = -1;
	for(int i=m_last_index; i<m_init_item.max_connect; i++)
	{
		if(!m_con[i])
		{
			m_con[i] = new proxy_con_item(i);
			m_con[i]->init(m_init_item.buff_size);
			m_con[i]->set_is_free(false);
			fd = i;
			m_last_index = fd + 1;
			return fd;
		}
		if(m_con[i]->is_free())
		{
			m_con[i]->set_is_free(false);
			fd = i;
			m_last_index = fd + 1;
			return fd;
		}
	}
	for(int i=0; i<m_last_index; i++)
	{
		if(m_con[i]->is_free())
		{
			m_con[i]->set_is_free(false);
			fd = i;
			m_last_index = fd + 1;
			return fd;
		}
	}
	return fd;
}

int http_get_proxy_engin_win::get_free_size()
{
	guard g(m_mutex);
	int count = 0;
	for(size_t i=0; i<m_con.size(); i++)
	{
		if(!m_con[i] || m_con[i]->is_free())
			count ++;
	}
	return count;
}

proxy_fd_t http_get_proxy_engin_win::new_fd()
{
	return get_free_fd();
}

bool http_get_proxy_engin_win::get(const char* get_url, proxy_fd_t fd)
{
	if(fd < 0)
		return false;

	if(!m_con_thread.connect(m_h_complish_port, fd, m_con[fd], get_url))
	{
		cancel_get(fd);
		fd = 0;
		return false;
	}
	
	return true;
}

void http_get_proxy_engin_win::on_connect_resutl(proxy_fd_t fd, bool b)
{
	if(!b)
	{
		m_service->on_get_result(fd, 0, 0, CONNECT_CLOSE);
		cancel_get(fd);
	}
}

void http_get_proxy_engin_win::cancel_get(proxy_fd_t fd)
{
	if(fd<0 || (size_t)fd>=m_con.size())
		return;
	m_con[fd]->cancel_get();
}

void http_get_proxy_engin_win::set_data(proxy_fd_t fd, void* p)
{
	if(fd <0 || (size_t)fd>=m_con.size() || !m_con[fd])
		return;
	m_con[fd]->set_data(p);
}

void* http_get_proxy_engin_win::get_data(proxy_fd_t fd)
{
	if(fd <0 || (size_t)fd>=m_con.size() || !m_con[fd])
		return 0;
	return m_con[fd]->get_data();
}

int http_get_proxy_engin_win::svc()
{
	DWORD dw_size = 0;
	OVERLAPPEDSTRUCT_C* p_overlapped = 0;
	proxy_con_item* p_con = 0;
	while(m_is_run)
	{
		BOOL b = GetQueuedCompletionStatus(m_h_complish_port, &dw_size, 
			(PULONG_PTR)&p_con, 
			(LPOVERLAPPED*)&p_overlapped, INFINITE);
		if(!m_is_run)
			break;
		if(!p_con)
        {
            Sleep(1);
			continue;
        }
		if(!b)
		{
			m_service->on_get_result(p_con->get_fd(), 0, 0, CONNECT_CLOSE);
			cancel_get(p_con->get_fd());
			continue;
		}
		int data_len = 0;
		char* p_data = 0;
		int state = 0;
		if(!p_con->on_event(m_service))
		{
			cancel_get(p_con->get_fd());
		}
		/*
		if(p_con->get_data(&p_data, data_len, state, m_service))
		{
            if(data_len == 0 && state == HAS_MORE_DATA)
				continue;
            if(!m_service->on_get_result(p_con->get_fd(), p_data, data_len, state))
            {
                cancel_get(p_con->get_fd());
            }
		}
		else
		{
			m_service->on_get_result(p_con->get_fd(), 0, 0, END_OF_DATA);
			cancel_get(p_con->get_fd());
		}
		*/
	}
	return 0;
}

