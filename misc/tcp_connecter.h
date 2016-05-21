#ifndef __TCP_CONNECTOR_H__
#define __TCP_CONNECTOR_H__
#include "base_thread.h"
#include "block_buffer.h"
#include "mutex.h"
#ifdef _WIN32
#include <WinSock2.h>
#else
#endif
enum TCP_CONNECT_STATE
{
    TCP_STATE_CLOSE,
    TCP_STATE_CONNECTINT,
    TCP_STATE_CONNECTED
};
class i_tcp_connector_service
{
public:
    virtual void on_connect(int result) = 0;
    virtual void on_close(int reason) = 0;
    virtual int  check_pack(void* p_data, int len) = 0;
    virtual bool on_recive(void* p_data, int len) = 0;
};

class tcp_connecter : public base_thread
{
public:
    tcp_connecter(void);
    ~tcp_connecter(void);
    bool init(i_tcp_connector_service* service, int buffsize);
    bool fini();
    bool connect(char* ip, unsigned short port);
    bool close(bool notify);
    bool send(void* p_data, int len);
    virtual int svc();
    int  get_state();
private:
    void do_send();
    bool on_io_event();
    bool on_recive();
    bool on_send();
    bool on_close();
private:
    i_tcp_connector_service* m_service;
    block_buffer m_send_buf;
    block_buffer m_sending_buf;
    block_buffer m_recv_buf;
    thread_mutex m_send_mutex;
    thread_mutex m_recv_mutex;
    SOCKET m_socket;
    bool  m_sending;
    bool  m_is_connected;
    int   m_state;
#ifdef _WIN32
    HANDLE m_handle;
#endif
};


#endif//__TCP_CONNECTOR_H__