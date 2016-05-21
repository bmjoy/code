#include "en_epoll.h"
#include <string.h>
#include <iostream>
using namespace std;

en_epoll::en_epoll()
{
	_events = 0;
	_epfd = 0;
	_max_events = 0;
}

en_epoll::~en_epoll()
{
	fini();
}

int en_epoll::init(int max_events)
{
	if(max_events<1)
	{
		return -1;
	}
	if(_events)
	{
		return -2;
	}
	_max_events = max_events;

	_epfd = epoll_create(_max_events);

	if(_epfd == -1)
	{
		return -3;
	}
	_events = new epoll_event[_max_events];
	return 0;
}

int en_epoll::fini()
{
	if(_events)
	{
		delete[] _events;
		_events = 0;
	}
	if(_epfd)
	{
		close(_epfd);
		_epfd = 0;
	}
	return 0;
}

int en_epoll::poll(int timeout)
{
	int ntfd=epoll_wait(_epfd,_events,_max_events, timeout);
	if(ntfd<=0)
	{
		usleep(1000);
		return 0;
	}
	for(int i=0; i<ntfd; i++)
	{
		do_events(_events[i].data.fd, _events[i].events);
	}
	return 0;     
}

bool en_epoll::add(int fd,unsigned int events)
{
	epoll_event polevt;
	polevt.events=events;
	polevt.data.fd=fd;
	return ctl(EPOLL_CTL_ADD,fd,polevt)==0;
}

bool en_epoll::del(int fd,unsigned int events)
{
	epoll_event polevt;
	polevt.events=events;
	polevt.data.fd=fd;
	return ctl(EPOLL_CTL_DEL,fd,polevt)==0;
}

bool en_epoll::modify(int fd,unsigned int events)
{
	epoll_event polevt;
	polevt.events=events;
	polevt.data.fd=fd;
	return ctl(EPOLL_CTL_MOD,fd,polevt)==0;
}

int en_epoll::ctl(int op, int fd, struct epoll_event &event)
{
	int ret=epoll_ctl(_epfd,op,fd,&event);
	if (ret!=0)
	{
	//	fprintf(stderr, "epoll_ctl fail,op is %d,fd is %d, event=%d,errno is %s\n",op,fd,event.events,strerror(errno));
	}
	return ret;     
}

bool en_epoll::setnonblocking(int sock)
{
	int opts;
	opts=fcntl(sock,F_GETFL);
	if (opts<0)
		opts=O_NONBLOCK;
	else
		opts = opts|O_NONBLOCK;
	if (fcntl(sock,F_SETFL,opts)<0)
	{
		fprintf(stderr, "setnoblock error");
		return false;
	}
	else
		return true;
}

bool en_epoll::setreuseport(int sock)
{
	int on=1;
	int ret=setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(int));
	return ret!=-1;
}

bool en_epoll::setsendtimeout(int sock, int t)
{
	int ret = 0;
	struct timeval val;
	val.tv_usec = 0;
	val.tv_sec = t;
	ret = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &val, sizeof(val));
	return ret != -1;
}

bool en_epoll::setrecvtimeout(int sock, int t)
{
	int ret = 0;
	struct timeval val;
	val.tv_usec = 0;
	val.tv_sec = t;
	ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &val, sizeof(val));
	return ret != -1;
}
