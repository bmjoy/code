#ifndef __I_HTTP_GET_PROXY_ENGIN_H__
#define __I_HTTP_GET_PROXY_ENGIN_H__
typedef int proxy_fd_t;
#define INVALIDE_PROXY_FD -1

struct proxy_init_item
{
	int rs_threads;
	int con_threads;
	int max_connect;
	int buff_size;
};


class i_http_get_proxy_service
{
public:
	virtual bool on_get_result(proxy_fd_t fd, void* p_data, int datalen, int state_code) = 0;
};

class i_http_get_proxy_engin
{
public:
	virtual bool init(proxy_init_item item) = 0;
	virtual bool set_service(i_http_get_proxy_service* service) = 0;
	virtual bool fini() = 0;
	virtual bool start() = 0;
	virtual bool stop() = 0;

	virtual proxy_fd_t new_fd() = 0;
	virtual bool get(const char* get_url, proxy_fd_t fd) = 0;
	virtual void cancel_get(proxy_fd_t fd) = 0;
	virtual void set_data(proxy_fd_t fd, void* p) = 0;
	virtual void* get_data(proxy_fd_t fd) = 0;
	virtual int get_free_size() = 0;
};

#endif//__I_HTTP_GET_PROXY_ENGIN_H__