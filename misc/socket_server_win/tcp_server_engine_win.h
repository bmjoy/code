#pragma once

#include "../i_tcp_socket_engine.h"
#include "tcp_socket_item.h"
#include <Windows.h>
#include "../block_buffer.h"
#include "listen_thread.h"

class tcp_server_engine_win;

///////////////////////////////////////////////////////////////////////////////
// check_time_out_thread class
class check_time_out_thread : public base_thread
{
	friend tcp_server_engine_win;
// construction & destrcution
public:
	check_time_out_thread();
// attributes
private:
	tcp_server_engine_win*	m_p_engine;
	bool					m_brun;
// operations
private:
	bool					start(tcp_server_engine_win* p_engine);
	void					stop();
protected:
public:
	virtual int				svc();

};
///////////////////////////////////////////////////////////////////////////////
// tcp_server_engine_win class
class tcp_server_engine_win : public i_tcp_server_engine,
							  public i_listen_accept,
							  public base_thread
{
	friend check_time_out_thread;
// construction & destruction
public:
	tcp_server_engine_win(void);
	~tcp_server_engine_win(void);
// attributes
private:
	bool					m_b_run;
	//int			        m_current_connections;
	engine_init_item		m_init_item;
	i_tcp_server_service*	m_p_server;
	i_tcp_socket_service*	m_p_service;
	listen_thread_win		m_listen;
	tcp_socket_item*		m_socket_item;
	thread_mutex			m_item_mutex;
	check_time_out_thread	m_check_thread;
	int						m_last_index;
    block_buffer_pool       buff_pool;
protected:
public:
// operations
private:
	void					on_recv(tcp_socket_item* p_item, int size, unsigned short handle);
	void					on_send(tcp_socket_item* p_item, int size, unsigned short handle);
	void					check_timeout();
public:
	virtual bool			init(engine_init_item init_item, i_tcp_server_service* p_service);
	virtual bool			get_service_port(unsigned short& port);
	virtual bool			fini();
	/*if has not initialized or the socket server has not in stop state, the function will return false. 
	otherwise return true.*/
	virtual bool			set_service(i_tcp_socket_service* p_service);
	virtual bool			start();
	virtual bool			stop();
	/*if socket server not in start state, the function will return false; 
	in pause state, the server will still offer service for connected client, but refuse the
	new connections.*/
	virtual bool			pause();
	virtual bool			resume();
	virtual bool			get_address(socket_id sid, struct sockaddr_in& addr, bool b_remote=true);
    virtual bool            get_ip(socket_id sid, char* ip);

	virtual bool			send(socket_id sid, void* p_head, int head_len, void* p_data, int data_len);
	virtual bool			send_batch(void* p_head, int head_len, void* p_data, int data_len);
	virtual void			set_batch_enable(socket_id sid, bool enable);
	virtual void			close_socket(socket_id sid, bool b_notify = true);
	virtual bool			set_data(socket_id sid, void* p);
	virtual bool			get_data(socket_id sid, void** p);
	virtual bool			is_online(socket_id sid);
	virtual int				get_online_connections();

	virtual bool			on_accept(SOCKET socket, sockaddr_in addr);
	virtual int				svc();
};
