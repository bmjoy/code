#ifndef __UDP_H__
#define __UDP_H__

#include <WinSock2.h>

class CUdp
{
public:
	CUdp(void);
	~CUdp(void);

	bool init();

	int open(unsigned short port=0);
	int close();
	int read(sockaddr_in &addr, char* pBuff, int buflen);
	int send(sockaddr_in  addr, char* pData, int len);
	int send(char* ip, unsigned short port, char* pData, int len);

public:
	SOCKET	 socket_;

};


#endif//__UDP_H__