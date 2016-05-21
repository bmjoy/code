#include "udp_thread.h"
#include <iostream>
using namespace std;
udp_thread::udp_thread(void):base_thread("udp_thread")
{
    m_rs = 0;
    m_run = false;
}

udp_thread::~udp_thread(void)
{
}

int udp_thread::init()
{
    m_udp.init();
    ring_buffer2_thread::init(1, 1024*1024, 1024*1024, 1024*1024*20, 1024*1024);
    return 0;
}

bool udp_thread::open(int rs, i_udp_service* service, int port)
{
    if(m_run)
        return false;
    if(0 != m_udp.open(port))
        return false;
    m_service = service;
    m_rs = rs;
    m_run = true;
    activate(m_rs);
    ring_buffer2_thread::start();
    return true;
}

void udp_thread::close()
{
    if(!m_run)
        return;
    m_run = false;
    m_udp.close();
    ring_buffer2_thread::stop();
    Sleep(300);
}

int udp_thread::send(char* ip, unsigned short port, void* data, int len)
{
    udp_id id;
    id.port = port;
    id.ip = inet_addr(ip);
    return send(id, data, len);
}

int udp_thread::send(udp_id id, void* data, int len)
{
    sockaddr_in addr;
    int addrlen = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(id.port);
    addr.sin_addr.s_addr = id.ip;
    put_data(&addr, sizeof(addr), data, len);
    return len;
}

void udp_thread::on_data(void* phead, int headlen, void* pdata, int datalen)
{
    sockaddr_in& addr = *(sockaddr_in*)phead;

    m_udp.send(addr, (char*)pdata, datalen);
}

int udp_thread::svc()
{
    char buf[102400];
    while (m_run)
    {
        sockaddr_in addr;
        int len = m_udp.read(addr, buf, sizeof(buf));
        if(len < 0) break;
        if(len == 0)
        {
            Sleep(1);
            continue;
        }
        udp_id id;
        id.ip = addr.sin_addr.s_addr;
        id.port = htons(addr.sin_port);
        if(m_service)
            m_service->on_recive(id, buf, len);
    }
    printf("%s end\n", thread_name);
    return 0;
}