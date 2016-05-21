#include <WinSock2.h>
#include "listen_thread.h"
#pragma comment(lib, "Ws2_32.lib")


listen_thread_win::listen_thread_win():base_thread("listen_thread_win")
{
	m_socket = INVALID_SOCKET;
	m_b_run = false;
	m_p = 0;
}

listen_thread_win::~listen_thread_win()
{

}

bool listen_thread_win::create(i_listen_accept* p)
{
	if(!p)
		return false;
	m_p = p;
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if(err != 0)
		return FALSE;

	m_hCompletionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
	if(m_hCompletionPort == NULL)
		return false;

	m_socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 
		WSA_FLAG_OVERLAPPED);
	int i = WSAGetLastError();
	BOOL optValue=TRUE;
	//setsockopt(m_socket,SOL_SOCKET,SO_REUSEADDR,(char*)&optValue,sizeof(optValue));//端口重定向

	return(m_socket != INVALID_SOCKET);
}

bool listen_thread_win::bind(unsigned short port)
{
	struct sockaddr_in SocketAddr;
	::ZeroMemory(&SocketAddr, sizeof(SocketAddr));
	SocketAddr.sin_family = AF_INET;
	SocketAddr.sin_addr.s_addr = INADDR_ANY;
	SocketAddr.sin_port = htons(port);

	return(SOCKET_ERROR 
		!= ::bind(m_socket, (SOCKADDR*)&SocketAddr, sizeof(SocketAddr)));
}

bool listen_thread_win::listen()
{
	if(m_b_run)
		return false;
	if(::listen(m_socket,SOMAXCONN) == SOCKET_ERROR)
		return false;
	m_b_run = true;
	return activate();
	//return true;
}

bool listen_thread_win::close_socket()
{
	if(m_socket == INVALID_SOCKET)
		return false;
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
	Sleep(1000);
	return true;
}

SOCKET listen_thread_win::get_socket()
{
	return m_socket;
}

HANDLE listen_thread_win::get_handle()
{
	return m_hCompletionPort;
}

int listen_thread_win::svc()
{
	SOCKET hSocket = INVALID_SOCKET;

	sockaddr_in SocketAddr;
	int iAddrBuferLen = sizeof(SocketAddr);

	while(true)
	{
		try
		{
			//接收连接
			hSocket	= ::WSAAccept(m_socket, (sockaddr*)&SocketAddr, &iAddrBuferLen, NULL, NULL);

			if (WSAENOTSOCK == hSocket)
				continue;
			if (INVALID_SOCKET == hSocket)
			{
				int iErrorCode = WSAGetLastError();
				break;
			}
			u_long nonblockingmode = TRUE;
			ioctlsocket(hSocket, FIONBIO , &nonblockingmode);

			int TimeOut; 
			//设置发送超时
			TimeOut = 5000; 
			::setsockopt(hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&TimeOut,sizeof(TimeOut));
			//设置接收超时
			TimeOut = 5000; 
			::setsockopt(hSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut));

			if(!m_p->on_accept(hSocket, SocketAddr))
			{
				closesocket(hSocket);
			}
			hSocket = INVALID_SOCKET;
		}
		catch(...)
		{

		}
	}
	PostQueuedCompletionStatus(m_hCompletionPort, 0, 0, 0);
	CloseHandle(m_hCompletionPort);
	m_hCompletionPort = 0;
	return 0;
}
