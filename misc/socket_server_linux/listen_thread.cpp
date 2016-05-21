#include "listen_thread.h"
#include <string.h>
#include <iostream>
#include "en_epoll.h"
#include "cflog.h"
#include <errno.h>
using namespace std;

listen_thread::listen_thread()
{
	b_init = false;
	b_run = false;
	listen_fd = -1;
	pservice = 0;
}

listen_thread::~listen_thread()
{

}

bool listen_thread::init(int _port, i_listen_service* _pservice)
{
	if(b_init)
	{
		return false;
	}
	listen_fd = socket(AF_INET,SOCK_STREAM,0);
	//cout << "listen_fd: " << listen_fd << endl;
	if(listen_fd < 0)
	{
	    LOG_ERROR("listen_fd=%d, errno=%d", listen_fd, errno);
		return false;
	}
	if(!_pservice)
	{
		return false;
	}
	port = _port;
	pservice = _pservice;

	en_epoll::setreuseport(listen_fd);

	sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serveraddr.sin_port=htons(port);
	
	int err;
	int trytime = 0;
	err = bind(listen_fd,(sockaddr *)&serveraddr, sizeof(serveraddr));
	while(err!=0){
	    sleep(1);
	    err = bind(listen_fd,(sockaddr *)&serveraddr, sizeof(serveraddr));
	    trytime ++;
	    if(trytime > 3)
	    {
	        LOG_ERROR("socket bind fail, the port is in use.");
	        return false;
	    }
	}
	//printf("bind errno=%d, e=%d\n", errno, e);
	if(0 == listen(listen_fd,256))
	{
		b_init = true;
	}
	else
	{
	    printf("listen errno=%d\n", errno);
	    return false;
	}
	usleep(10);
	return b_init;
}

bool listen_thread::fini()
{
	if(b_run)
	{
		return false;
	}
	if(!b_init)
	{
		return false;
	}
	if(listen_fd >=0)
	    close(listen_fd);
	listen_fd = -1;
	b_init = false;
	return true;
}

bool listen_thread::start()
{
	if(!b_init)
	{
		return false;
	}
	if(b_run)
	{
		return false;
	}
	b_run = true;
	activate();
	return true;
}

bool listen_thread::stop()
{
	if(!b_init)
	{
		return false;
	}
	if(!b_run)
	{
		return false;
	}
	b_run = false;
	fini();
	return true;
}

int listen_thread::svc()
{
	//int timeout = 1000;
	while(b_run)
	{
		try
		{
			_listen();
		} catch (...)
		{
			cout << "listen svc exception." << endl;
		}
	}
	cout << "socket listen thread end." << endl;
	return 0;
}

void listen_thread::_listen()
{
	sockaddr_in sa;
	memset(&sa,0,sizeof(sa));
	socklen_t salen=sizeof(sa);
	int newsock=accept(listen_fd, (sockaddr *)&sa,&salen);
	if (newsock>=0)
	{
		if(pservice)
		{
			en_epoll::setnonblocking(newsock);
			en_epoll::setreuseport(newsock);
			en_epoll::setsendtimeout(newsock, 5);
			en_epoll::setrecvtimeout(newsock, 5);
			if(!pservice->on_new_connect(newsock, sa))
				close(newsock);
		}
	}
	else
	{
		cout << "newsock=" << newsock << endl;
	}
}
