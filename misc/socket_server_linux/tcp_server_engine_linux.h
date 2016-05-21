#ifndef __TCP_SERVER_ENGINE_LINUX_H__
#define __TCP_SERVER_ENGINE_LINUX_H__
#include "../i_tcp_socket_engine.h"
#include "rs_thread.h"
#include "listen_thread.h"
#include "tcp_socket_item.h"
#include "../block_buffer.h"
#include "../base_thread.h"
#include <vector>
using namespace std;

class tcp_server_engine_linux
	: public i_tcp_server_engine
	, public i_listen_service
	, public i_rs_service
	, public base_thread
{
public:
	tcp_server_engine_linux();
	~tcp_server_engine_linux();
	//implement i_tcp_server_engine
	virtual bool init(engine_init_item init_item, i_tcp_server_service *p_service);
	virtual bool set_service(i_tcp_socket_service *p_service);
	virtual bool fini();
	virtual bool get_address(socket_id sid, struct sockaddr_in &addr, bool b_remote);
    virtual bool get_ip(socket_id sid, char* ip);
	virtual bool get_data(socket_id sid, void * *p);
	virtual int get_online_connections();
	virtual bool get_service_port(unsigned short &port);
	virtual bool is_online(socket_id sid);
	virtual bool pause();
	virtual bool resume();
	virtual bool send(socket_id sid, void *p_head, int head_len, void *p_data, int data_len);
	virtual void close_socket(socket_id sid, bool b_notify);
	virtual bool send_batch(void *p_head, int head_len, void *p_data, int data_len);
	virtual void set_batch_enable(socket_id sid, bool enable);
	virtual bool set_data(socket_id sid, void *p);
	virtual bool start();
	virtual bool stop();
	//implement i_listen_service
	virtual bool on_new_connect(int fd, sockaddr_in sa);
	//implement i_rs_service
	virtual int on_send_event(int fd);
	virtual int on_recv_event(int fd);
	//implement base_thread
	virtual int svc();
private:
	void check_time_out();
private:
	bool              m_b_run;
	vector<rs_thread> m_send_thread;
	int               m_send_thread_index;
	rs_thread         m_recv_thread;
	listen_thread     m_listen_thread;
	vector<tcp_socket_item> m_socket_item;
	engine_init_item  m_init_item;
	i_tcp_server_service* m_p_service;
	block_buffer_pool m_buffer_pool;
};

#endif//__TCP_SERVER_ENGINE_LINUX_H__
