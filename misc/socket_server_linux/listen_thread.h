/*
auth:  Ishaq
email: msl_cn@126.com
*/
#ifndef __listen_thread_H__
#define __listen_thread_H__
#include "../base_thread.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class i_listen_service
{
public:
	virtual bool on_new_connect(int fd, sockaddr_in sa) = 0;
};

class listen_thread : public base_thread
{
public:
	listen_thread();
	~listen_thread();

	virtual int svc();

	bool init(int _port, i_listen_service* _pservice);
	bool fini();
	bool start();
	bool stop();
private:
	void _listen();
private:
	int  listen_fd;
	bool b_init;
	bool b_run;
	int  port;
	i_listen_service* pservice;
};


#endif//__listen_thread_H__
