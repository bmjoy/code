#ifndef __TCP_SOCKET_ITEM_WIN_H__
#define __TCP_SOCKET_ITEM_WIN_H__
#include <WinSock2.h>
#include "../block_buffer.h"
#include "../i_tcp_socket_engine.h"
#include "../mutex.h"
struct OverLappedStruct
{
	OVERLAPPED		OverLapped;
	unsigned short 	uOperationType;
	unsigned short 	HandleID;
};
#define SOCKET_SND		1	//SOCKET Send
#define SOCKET_REV		2	//SOCKET Recv


class tcp_socket_item
{
public:
	tcp_socket_item();
	~tcp_socket_item();

	void set_index(unsigned short index);
	bool attach(SOCKET s, sockaddr_in addr);
	

	SOCKET get_socket();
	socket_id get_socket_id();
	bool send(void* pdata, int datalen);
	bool is_online(unsigned short h);
	bool is_online();
	bool get_addr(struct sockaddr_in& addr);
	bool set_data(socket_id sid, void* pdata);
	bool get_data(socket_id sid, void** pdata);
	void close();
	bool check_time_out(unsigned int t, unsigned int dt, int c);

	bool on_recv(int size, unsigned short handle);
	bool on_send(int size, unsigned short handle);
	bool recv();
    void set_service(i_tcp_socket_service* service);
    void set_buff_pool(block_buffer_pool* pool);
    void set_max_package_count(int n);
    bool is_send_batch();
    void set_send_batch(bool b);
public:
    i_tcp_socket_service* p_service;
    block_buffer_pool* buff_pool;
    int max_package_count_per_second;

	static unsigned short handleid;
	static unsigned short make_handle_id();
private:
	bool do_send();
private:
	void* m_p_data;
	unsigned int m_last_time_stamp;
	int m_time_out_count;
	bool m_batch_send;
	struct sockaddr_in m_addr;
	socket_id m_sid;
	SOCKET    m_socket;
	//thread_mutex m_recv_mutex;
	//thread_mutex m_send_mutex;
    thread_mutex   m_mutex;

	OverLappedStruct m_SendOverData;
	OverLappedStruct m_RecvOverData;
	WSABUF			 m_ReadBuffer;
	WSABUF			 m_SendBuffer;
	block_buffer*    m_p_recv_buff;
	block_buffer*    m_p_send_tail;
	block_buffer*    m_p_send_head;
	bool             m_b_sending;
    int              m_package_count;
    unsigned int     m_package_timespame;
};

#endif//__TCP_SOCKET_ITEM_WIN_H__