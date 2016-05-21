#ifndef __TCPS_TCP_CONNECTOR_H__
#define __TCPS_TCP_CONNECTOR_H__
#include <WinSock2.h>
#include "block_buffer.h"
#include "i_tcp_socket_engine.h"
#include "mutex.h"
struct OVERLAPPEDSTRUCT_A
{
	OVERLAPPED		overlapped;
	unsigned short 	operation_type;
};
#define SOCKET_SND		1	//SOCKET Send
#define SOCKET_REV		2	//SOCKET Recv

class tcp_connector
{
public:
	tcp_connector(void);
	~tcp_connector(void);

	void init(int index, HANDLE h, block_buffer_pool* pool);

	bool connect(const char* ip, unsigned short port);
	bool close();
	bool send(void* pdata, int len);

	bool on_send(int size);
	bool on_recv(int size);

	bool is_connected();

	void set_service(i_tcp_socket_con_service* p_service);
	bool set_isuse(bool b);
	bool is_use();
	socket_id get_sid();
	void set_data(void* p);
	void* get_data();
	i_tcp_socket_con_service* service();
private:
	bool do_recv();
	bool do_send();
	block_buffer* new_block();
	void release_block(block_buffer* p);
private:
	i_tcp_socket_con_service* m_service;
	HANDLE				m_handle;
	bool				m_isuse;
	void*				m_pdata;
	block_buffer_pool*  m_p_pool;
private:
	socket_id			m_sid;
	SOCKET				m_socket;

	OVERLAPPEDSTRUCT_A	m_sendoverdata;
	WSABUF				m_sendbuffer;
	block_buffer*		m_psendblockhead;
	block_buffer*		m_psendblocktail;
	bool				m_bsending;
	thread_mutex		m_sendmutex;

	OVERLAPPEDSTRUCT_A	m_recvoverdata;
	WSABUF				m_readbuffer;
	block_buffer*		m_precvblock;
	thread_mutex		m_recvmutex;

};



#endif//__TCPS_TCP_CONNECTOR_H__