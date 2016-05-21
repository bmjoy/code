#include "http_get_proxy_connect_thread.h"
#include "http_get_proxy_engin_win.h"
#include "util.h"
#include <iostream>
using namespace std;

http_get_proxy_connect_thread::http_get_proxy_connect_thread()
{
	m_p = 0;
}

struct con_item 
{
	HANDLE h;
	proxy_fd_t fd;
	proxy_con_item* p_con;
};

void http_get_proxy_connect_thread::set_service(http_get_proxy_engin_win* p)
{
	m_p = p;
}

bool http_get_proxy_connect_thread::connect(HANDLE h, proxy_fd_t fd, proxy_con_item* p_con, const char* uri)
{
	char buf[1024] = {0};
	con_item& item = *((con_item*)buf);
	int hl = sizeof(con_item);
	int ul = strlen(uri);
	int len = sizeof(con_item) + strlen(uri) + 1;
	if(len > 1000)
		return false;

	if(!p_con)
		return false;
	item.fd = fd;
	item.p_con = p_con;
	item.h = h;
#pragma warning(disable:4996)
	strcpy(&buf[sizeof(con_item)], uri);
#pragma warning(default:4996)
	p_con->set_connecting(true);
	return this->put_data(buf, len);
}

void http_get_proxy_connect_thread::on_data(void* pdata, int len)
{
	con_item& item = *((con_item*)pdata);
	char* url = &(((char*)pdata)[sizeof(con_item)]);
	int ul = len - sizeof(con_item) - 1;
	bool b = item.p_con->get_http(url, ul, item.h);
	if(!b)
	{
		ODBG("http connect time out.%s\n", url);
	}
	m_p->on_connect_resutl(item.fd, b);
}