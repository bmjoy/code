#include "connect_thread.h"

connect_thread::connect_thread(void)
{
	m_pservice = 0;
}

connect_thread::~connect_thread(void)
{
}

struct connect_item
{
	tcp_connector* p_con;
	int index;
	char ip[64];
	unsigned short port;
};

void connect_thread::set_service(i_connect_service* p_service)
{
	m_pservice = p_service;
}

bool connect_thread::connect(tcp_connector* p_con, int index, const char* ip, unsigned short port)
{
	if(!m_pservice)
		return false;
	connect_item item;
	item.p_con = p_con;
	item.index = index;
#pragma warning(disable:4996)
	strcpy(item.ip, ip);
#pragma warning(default:4996)
	item.port = port;
	put_data(&item, sizeof(item));
	return true;
}

void connect_thread::on_data(void* pdata, int len)
{
	connect_item& item = *(connect_item*)pdata;
	bool b = item.p_con->connect(item.ip, item.port);
	m_pservice->on_connect_result(item.index, item.p_con, b);
}

