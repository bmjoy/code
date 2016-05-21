#ifndef __UDP_SOCKET_H__
#define __UDP_SOCKET_H__
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include "base_thread.h"
#include "ring_buffer2_thread.h"
#include "udp.h"

struct udp_id
{
    unsigned int   ip;
    unsigned short port;
    bool operator == (udp_id& id)
    {
        return (ip==id.ip && port == id.port);
    }
};

class i_udp_service
{
public:
    virtual void on_recive(udp_id id, char* data, int len) = 0;
};

class udp_thread : public base_thread
    , public ring_buffer2_thread<1>
{
public:
    udp_thread(void);
    ~udp_thread(void);
    int init();
    bool open(int rs, i_udp_service* service, int port=0);
    void close();
    int  send(char* ip, unsigned short port, void* data, int len);
    int  send(udp_id id, void* data, int len);
    virtual int svc();
    virtual void on_data(void* phead, int headlen, void* pdata, int datalen);
private:
    bool   m_run;
    i_udp_service* m_service;
    int    m_rs;
    CUdp   m_udp;
};

#endif//__UDP_SOCKET_H__