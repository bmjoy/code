#ifndef __TCP_H__
#define __TCP_H__
#include "base_thread.h"
#ifndef _WIN32
typedef int SOCKET;
#else
#include <WinSock2.h>
#include <Windows.h>
#endif
#include "mutex.h"

enum TCP_CONNECT_STATE
{
    TCP_STATE_CLOSE,
    TCP_STATE_CONNECTING,
    TCP_STATE_CONNECTED
};

class i_tcp_service
{
public:
    virtual void on_connect(int result) = 0;
    virtual void on_close() = 0;
    virtual int  on_check_pack(char* pdata, int len) = 0;
    virtual bool on_recv(char* pdata, int len) = 0;
	virtual bool get_heart_test_data(char** data, int* len) = 0;
};

class CTcp : public base_thread
{
public:
	CTcp(void);
	~CTcp(void);

	void init(i_tcp_service* service);
	void close();
	bool connect(const char* ip, unsigned short port);
	bool send(const char* pdata, int len);
	bool is_connected();
    int  get_state(){return m_state;}

    virtual int svc();
private:
	void send_heart_test();
    bool on_recv();

#ifdef _WIN32
    HANDLE   m_handler;
#endif
    int      m_state;
    char     m_recv_buf[8000];
    int      m_recv_len;
	thread_mutex m_sendmutex;
    bool     m_is_connected;
    i_tcp_service* m_service;
	SOCKET	 m_fd;
    char     m_ip[32];
	unsigned short m_port;
};

#endif//__TCP_H__
