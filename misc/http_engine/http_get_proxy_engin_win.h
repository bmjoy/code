#ifndef __HTTP_GET_PROXY_ENGIN_WIN_H__
#define __HTTP_GET_PROXY_ENGIN_WIN_H__

#include "i_http_get_proxy_engin.h"
#include "base_thread.h"
#include "proxy_con_item.h"
#include "http_get_proxy_connect_thread.h"
#include <vector>
using namespace std;

class http_get_proxy_engin_win :
	public i_http_get_proxy_engin ,
	public base_thread
{
	friend http_get_proxy_connect_thread;
public:
	http_get_proxy_engin_win(void);
	~http_get_proxy_engin_win(void);

	virtual bool init(proxy_init_item item);
	virtual bool set_service(i_http_get_proxy_service* service);
	virtual bool fini();
	virtual bool start();
	virtual bool stop();

	virtual proxy_fd_t new_fd();
	virtual bool get(const char* get_url, proxy_fd_t fd);
	virtual void cancel_get(proxy_fd_t fd);
	virtual void set_data(proxy_fd_t fd, void* p);
	virtual void* get_data(proxy_fd_t fd);
	virtual int get_free_size();
	virtual int svc();
	void on_connect_resutl(proxy_fd_t fd, bool b);
private:
	int get_free_fd();
private:
	http_get_proxy_connect_thread m_con_thread;
	HANDLE m_h_complish_port;
	bool m_is_init;
	bool m_is_run;
	proxy_init_item m_init_item;
	i_http_get_proxy_service* m_service;
	vector<proxy_con_item*> m_con;
	int m_last_index;
	thread_mutex m_mutex;
};

#endif//__HTTP_GET_PROXY_ENGIN_WIN_H__

