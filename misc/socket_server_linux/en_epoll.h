/*
auth:  Ishaq
email: msl_cn@126.com
*/
#ifndef __en_epoll_h__
#define __en_epoll_h__

#include <errno.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


class en_epoll
{
public:
	en_epoll();
	~en_epoll();
	int  init(int max_events);
	int  fini();
	int  poll(int timeout=1000);
	bool add(int fd,unsigned int events);
	bool del(int fd,unsigned int events);
	bool modify(int fd,unsigned int events);
	int  ctl(int op, int fd, struct epoll_event &event);

	static bool setnonblocking(int sock);
	static bool setreuseport(int sock);
	static bool setsendtimeout(int sock, int t);
	static bool setrecvtimeout(int sock, int t);
public:
	virtual unsigned int do_events(int fd, unsigned int events){return 0;}

protected:
	epoll_event* _events;
	int          _epfd;
	int          _max_events;
};


#endif//__en_epoll_h__
