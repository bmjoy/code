#include "tcp_connecter.h"
#include <iostream>
using namespace std;

tcp_connecter::tcp_connecter(void):base_thread("tcp_connecter")
{
#ifdef _WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD( 2, 2 );
    WSAStartup( wVersionRequested, &wsaData );
#endif
    m_service = 0;
    m_socket = INVALID_SOCKET;
    m_handle = 0;
    m_sending = false;
    m_is_connected = false;
    m_state = TCP_STATE_CLOSE;
}

tcp_connecter::~tcp_connecter(void)
{
    
}

bool tcp_connecter::init(i_tcp_connector_service* service, int buffsize)
{
    m_service = service;
    if(buffsize < 4096)
        buffsize = 4096;
    m_send_buf.create(buffsize);
    m_recv_buf.create(buffsize);
    m_sending_buf.create(buffsize);

    return true;
}

bool tcp_connecter::fini()
{
    if(m_socket != INVALID_SOCKET)
    {
        ::closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
    if(m_handle != 0)
    {
        ::WSACloseEvent(m_handle);
        m_handle = 0;
    }
    m_send_buf.reset();
    m_recv_buf.reset();
    return true;
}

bool tcp_connecter::connect(char* ip, unsigned short port)
{
    close(false);
    if(m_handle == 0)
        m_handle = ::WSACreateEvent();
    if(m_handle == 0)
        return false;
    m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_socket == INVALID_SOCKET)
        return false;
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.S_un.S_addr = inet_addr(ip);
    //int set_size = 1024 * 1024;
    //int len = 0;
    //int error;
    //error = setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char *) &set_size, sizeof(set_size));
    //if(error != 0)
    //    return false;
    //error = setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char *) &len, sizeof(len));
    //if(error != 0)
    //    return false;
    int err;
    err = WSAConnect(m_socket, (sockaddr *)&addr, sizeof(addr), NULL, NULL, NULL, NULL);
    
    if(0 != err)
    {
        if(m_service != NULL)
            m_service->on_connect(err);
        return false;
    }

    if(0 != WSAEventSelect(m_socket, m_handle, FD_WRITE | FD_READ | FD_CLOSE | FD_CONNECT))
    {
        return false;
    }

    if(!activate())
    {
        fini();
        return false;
    }
    m_state = TCP_STATE_CONNECTINT;
    return true;
}

bool tcp_connecter::close(bool notify)
{
    fini();
    if(notify && (m_service))
        m_service->on_close(0);
    m_is_connected = false;
    m_state = TCP_STATE_CLOSE;
    return true;
}

bool tcp_connecter::send(void* p_data, int len)
{
    guard g(m_send_mutex);
    if(!p_data || !len || !m_is_connected)
        return false;
    if(m_send_buf.rest_free_len() < len)
        return false;
    m_send_buf.write((const char*)p_data, len);
    do_send();
    return true;
}

void tcp_connecter::do_send()
{
    if(m_sending)
        return;
    DWORD		send_len = 0;
    WSABUF		wsabuf;
    m_sending_buf.reset();

    m_sending_buf.write(m_send_buf.rd_ptr(), m_send_buf.data_len());
    m_send_buf.reset();

    wsabuf.buf	= (char*)m_sending_buf.rd_ptr();
    wsabuf.len	= m_sending_buf.data_len();

    if(wsabuf.len == 0)
        return;

    int	ret = WSASend(m_socket, &wsabuf, 1, &send_len, 0, NULL, NULL);
    m_send_buf.rd_ptr(send_len);
    m_send_buf.arrange_data();

    if(ret != 0)
    {
        int err = WSAGetLastError();
        if(err == WSAEWOULDBLOCK)
        {
            m_sending = true;
            return;
        }
        else
        {
            close(true);
        }
    }
}

int tcp_connecter::get_state()
{
    return m_state;
}

int tcp_connecter::svc()
{
    bool bRun = true;
    cout << "tcp_connecter::svc  begin" << endl;
    while (bRun)
    {
        WaitForSingleObject(m_handle, INFINITE);
        bRun = on_io_event();
    }
    cout << "tcp_connecter::svc  end" << endl;
    return 0;
}

bool tcp_connecter::on_io_event()
{
    WSANETWORKEVENTS	NetworkEvents;
    int n = ::WSAEnumNetworkEvents(m_socket, m_handle, &NetworkEvents);
    if(0 == n)
    {
        if(NetworkEvents.lNetworkEvents & FD_CONNECT)
        {
            if(NetworkEvents.iErrorCode[FD_CONNECT_BIT] == 0)
            {
                m_state = TCP_STATE_CONNECTED;
                m_is_connected = true;
                if(m_service)
                    m_service->on_connect(0);
                return true;
            }
        }

        if(NetworkEvents.lNetworkEvents & FD_CLOSE)
        {
            if(NetworkEvents.iErrorCode[FD_CLOSE_BIT] == 0)
            {
                ;
            }
            on_close();
            return false;
        }

        if(NetworkEvents.lNetworkEvents & FD_READ)
        {
            if(NetworkEvents.iErrorCode[FD_READ_BIT] == 0)
            {
                if(!on_recive())
                {
                    close(true);
                    return false;
                }
            }
        }

        if(NetworkEvents.lNetworkEvents & FD_WRITE)
        {
            if(NetworkEvents.iErrorCode[FD_WRITE_BIT] == 0)
            {
                if(!on_send())
                {
                    close(true);
                    return false;
                }
            }
        }
        return true;
    }
    else
    {
#ifdef _DEBUG
        int err = h_errno;
#endif
        return false;
    }
}

bool tcp_connecter::on_recive()
{
    DWORD			dwFlag = 0;
    DWORD			dwRecvDataLen;
    WSABUF			WSABuf;

    WSABuf.buf = m_recv_buf.wr_ptr();
    WSABuf.len = m_recv_buf.rest_free_len();

    int	iReturn = WSARecv(m_socket, &WSABuf, 1, &dwRecvDataLen, &dwFlag, NULL, NULL);

    if(0 == iReturn)
    {
        m_recv_buf.wr_ptr(dwRecvDataLen);
    }
    else if(iReturn == WSA_IO_PENDING)
    {
        m_recv_buf.wr_ptr(dwRecvDataLen);
    }
    else
    {
        return false;
    }
    while(m_recv_buf.rest_data_len() > 0)
    {
        int l = m_service->check_pack(m_recv_buf.rd_ptr(), m_recv_buf.rest_data_len());

        if(l < 0 || l > m_recv_buf.rest_data_len())
            return false;
        if(l == 0)
            break;
        if(!m_service->on_recive(m_recv_buf.rd_ptr(), l))
        {
            return false;
        }
        if(m_is_connected == false)
        {
            return false;
        }
        m_recv_buf.rd_ptr(l);
    }
    m_recv_buf.arrange_data();

    return true;
}

bool tcp_connecter::on_send()
{
    guard g(m_send_mutex);
    m_sending = false;
    m_send_buf.arrange_data();
    do_send();
    return true;
}

bool tcp_connecter::on_close()
{
    close(true);

    return false;
}

