#ifndef __LISTEN_THREAD_WIN_H__
#define __LISTEN_THREAD_WIN_H__
#include "../base_thread.h"

class i_listen_accept
{
public:
	virtual bool on_accept(SOCKET socket, sockaddr_in addr) = 0;
};

class listen_thread_win : public base_thread
{
public:
	listen_thread_win();
	~listen_thread_win();
	virtual int svc();

	bool create(i_listen_accept* p);
	bool bind(unsigned short port);
	bool listen();
	bool close_socket();
	SOCKET get_socket();
	HANDLE get_handle();
private:

private:
	bool   m_b_run;
	SOCKET m_socket;
	HANDLE m_hCompletionPort;
	i_listen_accept* m_p;
};

#endif//__LISTEN_THREAD_WIN_H__