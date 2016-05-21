#include "ctcp.h"
#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)
#else
#endif
#include <stdio.h>

ctcp::ctcp(void)
{
    m_fd = 0;
    memset(m_err, 0, sizeof(m_err));
    m_is_keep_alive = true;
    m_is_no_block = true;
    m_last_err = 0;
    m_default_timeout = 3000;
    m_last_timeout = 0;
}

ctcp::~ctcp(void)
{
    close();
}

int _get_last_error()
{
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

int ctcp::set_read_time_out(int t)
{
    if(m_fd <= 0)
        return -1;
    if(m_last_timeout == t)
        return 0;
    m_last_timeout = t;
    return setsockopt(m_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&t, sizeof(int));
}

bool ctcp::init(bool is_keep_alive, bool is_no_block, int default_timeout)
{
    m_is_keep_alive = is_keep_alive;
    m_is_no_block = is_no_block;
    if(default_timeout > 0)
        m_default_timeout = default_timeout;
#ifdef _WIN32
    WSADATA data;
    if (WSAStartup(MAKEWORD(2,2), &data) != 0)
    {
        m_last_err = ::GetLastError();
        return false;
    }
#endif
    
    return true;
}

bool ctcp::connect(const char* ip, int port)
{
    if(is_connected())
        close();

    m_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(m_fd <= 0)
    {
        sprintf(m_err, "init socket fail");
        return false;
    }
    if(m_is_keep_alive)
    {
        int y = 1;
        if(-1 == setsockopt(m_fd,SOL_SOCKET,SO_KEEPALIVE, (const char *)&y, sizeof(y)))
            return false;
    }
    if(m_default_timeout > 0)
    {
        set_read_time_out(m_default_timeout);
    }
    sockaddr_in addr;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_family = AF_INET;
    if(0 != ::connect(m_fd, (SOCKADDR *)&addr, sizeof(addr)))
    {
        int err = WSAGetLastError();
        close();
        return false;
    }

    return true;
}

bool ctcp::is_connected()
{
    return (m_fd>0);
}

void ctcp::close()
{
#ifdef _WIN32
    ::closesocket(m_fd);
#else
    ::close(m_fd);
#endif
    m_last_err = 0;
    m_default_timeout = 3000;
    m_last_timeout = 0;
}

int ctcp::read(char* buf, int len, int timeout)
{
    if(timeout>0)
    {
        set_read_time_out(timeout);
    }
    else if(m_default_timeout > 0)
    {
        set_read_time_out(m_default_timeout);
    }
    int n = 0;
    n = ::recv(m_fd, buf, len, 0);
    if(n <= 0)
    {
        m_last_err = _get_last_error();
#ifdef _WIN32
        if(m_last_err == WSAEWOULDBLOCK)
            return 0;
#else
        if(m_last_err == EINTR || m_last_err == EWOULDBLOCK || m_last_err == EAGAIN)
            return 0;
#endif
        close();
        return -1;
    }
    return n;
}

int ctcp::write(char* buf, int len)
{
    int wl = 0;
    if(!is_connected())
        return -1;
    while(wl < len)
    {
        int sn = ::send(m_fd, &buf[wl], len - wl, 0);
        if(sn <= 0)
        {
            m_last_err = _get_last_error();
            close();
            return -1;
        }
        wl += sn;
    }
    return len;
}

int ctcp::get_last_error()
{
    return m_last_err;
}

const char* ctcp::get_err_msg()
{
    return m_err;
}