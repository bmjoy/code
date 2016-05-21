#ifndef __PROXY_CON_ITEM_H__
#define __PROXY_CON_ITEM_H__
#include "curl.h"
#include "http_url_parser.h"
#include "mutex.h"
#include "i_http_get_proxy_engin.h"
struct OVERLAPPEDSTRUCT_C
{
	OVERLAPPED    overlapped;
	curl_socket_t fd;
};

class proxy_con_item
{
public:
	proxy_con_item(int fd);
	~proxy_con_item(void);
	bool init(int buflen);
	bool is_free();
	bool set_is_free(bool b);
	bool on_event(i_http_get_proxy_service* service);
	bool get_data(char** p, int& datalen, int& state);
	bool get_http(const char* uri, int urilen, HANDLE hCompPort);
	int  get_fd();
	void cancel_get();
	void set_data(void* pdata);
	void* get_data();

	bool is_connecting();
	void set_connecting(bool b);
	void clear_up();
private:
	bool reg_read();
	bool filter_head(int& datalen);
    bool get_content_length();
private:
	http_url_parser  m_url_parser;
	OVERLAPPEDSTRUCT_C m_overlapped;
	int  m_fd;
	CURL *curl;
	curl_socket_t m_sockfd;

	bool  m_is_free;
	char* m_buf;
	int   m_buf_len;
	bool  m_b_head;
    int   m_head_buf_len;
    int   m_recv_len;
    int   m_content_length;

	void* m_pdata;

	static shared_thread_mutex m_connect_mutex_pool;
	thread_mutex* m_mutex;

	bool m_is_connecting;
};


#endif//__PROXY_CON_ITEM_H__