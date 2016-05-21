#ifndef __HTTP_GET_PROXY_CONNECT_THREAD_H__
#define __HTTP_GET_PROXY_CONNECT_THREAD_H__
#include "i_http_get_proxy_engin.h"
#include "proxy_con_item.h"
#include "ring_buffer_thread.h"

class http_get_proxy_engin_win;


class http_get_proxy_connect_thread : public ring_buffer_thread
{
public:
	http_get_proxy_connect_thread();
	void set_service(http_get_proxy_engin_win* p);
	bool connect(HANDLE h, proxy_fd_t fd, proxy_con_item* p_con, const char* uri);
	virtual void on_data(void* pdata, int len);

protected:
private:
	http_get_proxy_engin_win* m_p;
};

#endif//__HTTP_GET_PROXY_CONNECT_THREAD_H__