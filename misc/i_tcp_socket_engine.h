#ifndef __I_TCP_SOCKET_ENGINE_H__
#define __I_TCP_SOCKET_ENGINE_H__


struct socket_id
{
	union
	{
		struct  
		{
			unsigned int index;
			unsigned int handle;
		};
		unsigned long long key;
	};
	bool operator == (socket_id sid) const 
	{
		return (sid.key == key);
	}
	bool operator != (socket_id sid) const 
	{
		return sid.key != key;
	}
    bool operator > (socket_id sid) const
    {
        return key > sid.key;
    }
    bool operator < (socket_id sid) const
    {
        return key < sid.key;
    }
};

const socket_id INVALID_SOCKET_ID = {0xFFFF,0xFFFF};

#define MAX_PACKGE_LEN 2000

struct engine_init_item
{
	int port;
	int max_connects;
	int max_package_size;
	int max_mem_pool_size;
	int init_mem_pool_size;
	int step_mem_pool_size;
	int max_package_count_per_second;
	int time_out;
	int time_out_count;
	int rs_threads;
	int con_threads;
};

#ifndef _WIN32
#define STURN_SPORT
#endif

class i_tcp_socket_service
{
public:
#ifdef STURN_SPORT
	virtual int  on_sturn(socket_id sid, void* p_data, int len) = 0;
#endif
	virtual bool on_connect(socket_id sid, unsigned int ip, unsigned short port, bool& enable_batch_send) = 0;
	virtual void on_close(socket_id sid) = 0;
	/*
	return parse rest data length. 
	if return value < 0 or return value > len. engine will close the connect. 
	*/ 
    virtual int  check_pack(socket_id sid, void* p_data, int len) = 0;
	virtual bool on_recive(socket_id sid, void* p_data, int len) = 0;
};

class i_tcp_server_service : public i_tcp_socket_service
{
public:
	virtual void on_start() = 0;
	virtual void on_stop() = 0;
	virtual void on_pause() = 0;
	virtual void on_resume() = 0;

};

class i_tcp_server_engine
{
public:
	/*if failed parse the configure file or 
	  has initialized return false,
	  otherwise return true.
	  the configure file format */
	virtual bool init(engine_init_item init_item, i_tcp_server_service* p_service=0) = 0;
	virtual bool get_service_port(unsigned short& port) = 0;
	virtual bool fini() = 0;
	/*if has not initialized or the socket server has not in stop state, the function will return false. 
	  otherwise return true.*/
	virtual bool set_service(i_tcp_socket_service* p_service) = 0;
	virtual bool start() = 0;
	virtual bool stop() = 0;
	/*if socket server not in start state, the function will return false; 
	  in pause state, the server will still offer service for connected client, but refuse the
	  new connections.*/
	virtual bool pause() = 0;
	virtual bool resume() = 0;
	virtual bool get_address(socket_id sid, struct sockaddr_in& addr, bool b_remote=true) = 0;
    virtual bool get_ip(socket_id sid, char* ip) = 0;
	virtual bool send(socket_id sid, void* p_head, int head_len, void* p_data, int data_len) = 0;
	virtual bool send_batch(void* p_head, int head_len, void* p_data, int data_len) = 0;
	virtual void set_batch_enable(socket_id sid, bool enable) = 0;
	virtual void close_socket(socket_id sid, bool b_notify = true) = 0;
	virtual bool set_data(socket_id sid, void* p) = 0;
	virtual bool get_data(socket_id sid, void** p) = 0;
	virtual bool is_online(socket_id sid) = 0;
	virtual int get_online_connections() = 0;

};

class i_tcp_connect_engine_service
{
public:
	virtual void on_start() = 0;
	virtual void on_stop() = 0;
	virtual void on_pause() = 0;
	virtual void on_resume() = 0;
	virtual void on_error(int err_id, const char* err_msg) = 0;
};

class i_tcp_socket_con_service
{
public:
	virtual bool on_connect(socket_id sid, bool result) = 0;
	virtual void on_close(socket_id sid) = 0;
	virtual int  check_pack(socket_id, void* p_data, int len) = 0;
	/*
	return parse rest data length. 
	if return value < 0 or return value > len. engine will close the connect. 
	*/ 
	virtual bool on_recive(socket_id sid, void* p_data, int len) = 0;

};

class i_tcp_connect_engine
{
public:
	virtual bool init(engine_init_item init_item) = 0;
	virtual bool fini() = 0;
	virtual bool start() = 0;
	virtual bool stop() = 0;
	virtual bool pause() = 0;
	virtual bool resume() = 0;

	virtual bool get_connect(socket_id& id, i_tcp_socket_con_service* p_service,
							 const char* p_heart_test_data, int heart_test_len) = 0;
	virtual bool free_connect(socket_id sid) = 0;
	virtual bool connect(socket_id sid, const char* ip, unsigned short port) = 0;
	virtual bool connect(socket_id sid, unsigned int ip, unsigned short port) = 0;
	virtual bool close_socket(socket_id sid) = 0;
	virtual bool send(socket_id sid, void* p_data, int data_len) = 0;
	virtual bool set_data(socket_id sid, void* p) = 0;
	virtual bool get_data(socket_id sid, void** p) = 0;
	virtual bool is_online(socket_id sid) = 0;
	virtual int get_online_connections() = 0;
};

#endif//__I_TCP_SOCKET_ENGINE_H__


