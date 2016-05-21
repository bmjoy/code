/*
auth:  Ishaq
email: msl_cn@126.com
*/
#ifndef __TCP_recv_thread_H__
#define __TCP_recv_thread_H__

#include "en_epoll.h"
#include "../base_thread.h"

class i_rs_service
{
public:
	virtual int on_recv_event(int fd) = 0;
	virtual int on_send_event(int fd) = 0;
};

class rs_thread : public base_thread
                , public en_epoll
{
public:
	rs_thread();
	~rs_thread();
	void init(i_rs_service* service);
	bool start();
	bool stop();

	virtual int svc();
	virtual unsigned int do_events(int fd, unsigned int events);

private:
	bool b_run;
	i_rs_service* m_service;
};

#endif//__TCP_recv_thread_H__
