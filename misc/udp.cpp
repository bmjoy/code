#include "Udp.h"

#pragma comment(lib,"Ws2_32.lib")


CUdp::CUdp(void)
:socket_(INVALID_SOCKET)
{
	init();
}

CUdp::~CUdp(void)
{
	close();
}

bool CUdp::init()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if(err != 0)
		return false;

	return true;
}

int CUdp::open(unsigned short port)
{
	socket_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (!socket_) {
		return -1;
	}
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);
	if(bind(socket_,(struct sockaddr *)&sin,sizeof(sin)) < 0)
	{
		return -1;
	}
	return 0;
}

int CUdp::close()
{
	if(socket_ == INVALID_SOCKET)
		return 0;
	closesocket(socket_);
	socket_ = INVALID_SOCKET;
	return 0;
}

int CUdp::read(sockaddr_in &addr, char* pBuff, int buflen)
{
	int sock_len = sizeof(addr);

	if(socket_ == INVALID_SOCKET)
		-1;
	int len = recvfrom(socket_, pBuff,buflen,0,(sockaddr *)&addr,&sock_len);
	if (len > 0)
		return len;

	return 0;
}

int CUdp::send(sockaddr_in  addr, char* pData, int len)
{
	if(socket_ == INVALID_SOCKET)
		return SOCKET_ERROR;

	int ret=0;

	ret = sendto(socket_,pData,len,0,(const sockaddr *)&addr,sizeof(addr));
	while(ret ==0)
	{
		ret = sendto(socket_,pData,len,0,(const sockaddr *)&addr,sizeof(addr));
	}

	if(ret != SOCKET_ERROR)
		return ret;
	return SOCKET_ERROR;
}

int CUdp::send(char* ip, unsigned short port, char* pData, int len)
{
	sockaddr_in sa;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = inet_addr(ip);
	sa.sin_family = AF_INET;
	return send(sa, pData, len);
}
