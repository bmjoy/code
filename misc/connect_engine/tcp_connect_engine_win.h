#pragma once
#include "i_tcp_socket_engine.h"
#include "tcp_connector.h"
#include "base_thread.h"
#include "connect_thread.h"
#include <vector>
#include "mutex.h"
using namespace std;

class tcp_connect_engine_win :
	public i_tcp_connect_engine ,
	public base_thread,
	public i_connect_service
{
public:
	tcp_connect_engine_win(void);
	~tcp_connect_engine_win(void);

	virtual int svc();

	virtual bool init(engine_init_item init_item);
	virtual bool fini();
	virtual bool start();
	virtual bool stop();
	virtual bool pause();
	virtual bool resume();

	virtual bool get_connect(socket_id& id, i_tcp_socket_con_service* p_service,
		const char* p_heart_test_data, int heart_test_len);
	virtual bool free_connect(socket_id id);
	virtual bool connect(socket_id id, const char* ip, unsigned short port);
	virtual bool connect(socket_id id, unsigned int ip, unsigned short port);
	virtual bool close_socket(socket_id id);
	virtual bool send(socket_id id, void* p_data, int data_len);
	virtual bool set_data(socket_id id, void* p);
	virtual bool get_data(socket_id id, void** p);
	virtual bool is_online(socket_id id);
	virtual int get_online_connections();

	//////////////////////////////////////////////////////////////////////////
	virtual void on_connect_result(int index, tcp_connector* p_con, bool result);

private:
	block_buffer_pool             m_buff_pool;
	connect_thread                m_con_thread;
private:
	int                           m_last_index;
	HANDLE                        m_handle;
	bool                          m_is_init;
	bool                          m_is_run;
	engine_init_item              m_init_item;
	thread_mutex                  m_vec_mutex;
	vector<tcp_connector*>        m_con_vector;
};
