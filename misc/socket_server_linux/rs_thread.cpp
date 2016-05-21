#include "rs_thread.h"
#include <iostream>
using namespace std;


rs_thread::rs_thread()
{
	m_service = 0;
	b_run = false;
}

rs_thread::~rs_thread()
{
}

void rs_thread::init(i_rs_service* service)
{
	en_epoll::init(255);
	m_service = service;
}

bool rs_thread::start()
{
	if(b_run)
	{
		return false;
	}
	if(!_epfd)
	{
		return false;
	}
	if(!m_service)
	{
		return false;
	}
	b_run = true;
	activate();
	return true;
}

bool rs_thread::stop()
{
	if(!b_run)
	{
		return false;
	}
	b_run = false;
	join();
	return true;
}

int rs_thread::svc()
{
	int timeout = 5000;
	try
	{
		while(b_run)
		{
			poll(timeout);
		}
	} catch (...)
	{
	}
	return 0;
}

unsigned int rs_thread::do_events(int fd, unsigned int events)
{
	if(!b_run)
	{
		del(fd, 0);
		return 0;
	}

	if(events & EPOLLIN)
	{
		m_service->on_recv_event(fd);
	//	if(0 != m_service->on_recv_event(fd))
	//		del(fd, 0);
	}
	if(events & EPOLLOUT)
	{
		m_service->on_send_event(fd);
	//	if(0 != m_service->on_send_event(fd))
	//		del(fd, 0);
	}

	return 0;
}



