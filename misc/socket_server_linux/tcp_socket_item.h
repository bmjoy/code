#ifndef __TCP_SOCKET_ITEM_H__
#define __TCP_SOCKET_ITEM_H__
#include "../i_tcp_socket_engine.h"
#include "en_epoll.h"
#include "../mutex.h"
#include "../block_buffer.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


class tcp_socket_item
{
public:
	tcp_socket_item();
	~tcp_socket_item();
	void init(en_epoll* recv_thread, block_buffer_pool* buff_pool);
	void set_service(i_tcp_socket_service* p_service);
	int  close_socket(socket_id sid, bool notify);
	int  close_socket(bool notify);
	bool is_online(socket_id id);
	bool is_online();
	bool check_timeout(time_t ct, int timeout, int counter);
	void on_connect(int fd, en_epoll* send_thread, sockaddr_in addr);
	bool get_address(socket_id sid, sockaddr_in &addr);
	bool get_data(socket_id sid, void * *p);
	bool set_data(void * p);


	int send(const void* p_data, int len);
	int do_recv_event();
	int do_send_event();
private:
	unsigned int on_recv();
	unsigned int on_send();

private:
	static unsigned short m_handleid;
	int m_fd;
	socket_id m_sid;
	sockaddr_in m_address;
	time_t m_time_stamp;
	int    m_time_out_counter;
	void* m_p_data;
#ifdef STURN_SPORT
	bool is_sturn_ok;
#endif

	block_buffer_pool* m_p_buff_pool;
	i_tcp_socket_service* m_p_service;
	en_epoll*     m_p_recv_thread;
	en_epoll*     m_p_send_thread;
	block_buffer* m_p_send_head;
	block_buffer* m_p_send_tail;
	block_buffer* m_p_recv_buff;
	thread_mutex  m_mutex;
	//thread_mutex  m_send_mutex;
	//thread_mutex  m_recv_mutex;
};

#endif//__TCP_SOCKET_ITEM_H__
