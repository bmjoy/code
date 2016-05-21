#ifndef __CONNECT_THREAD_H__
#define __CONNECT_THREAD_H__
#include "tcp_connector.h"
#include "ring_buffer_thread.h"

class i_connect_service
{
public:
	virtual void on_connect_result(int index, tcp_connector* p_con, bool result) = 0;
};

class connect_thread : public ring_buffer_thread
{
public:
	connect_thread(void);
	~connect_thread(void);

	void set_service(i_connect_service* p_service);

	bool connect(tcp_connector* p_con, int index, const char* ip, unsigned short port);
	virtual void on_data(void* pdata, int len);

private:
	i_connect_service* m_pservice;
};


#endif//__CONNECT_THREAD_H__