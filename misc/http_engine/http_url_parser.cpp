#include "http_url_parser.h"
#include <string.h>
#include <stdio.h>
#include <WinSock2.h>

const char* url_head = "http://";
const int   url_head_len = 7;

#pragma warning(disable:4996)

http_url_parser::http_url_parser(void)
{
}

http_url_parser::~http_url_parser(void)
{
}

void http_url_parser::clear()
{
	memset(m_domain, 0, sizeof(m_domain));
	memset(m_ip, 0, sizeof(m_ip));
	memset(m_path, 0, sizeof(m_path));
	m_port = 80;
}

bool http_url_parser::parser(const char* url, int len)
{
	char temp_ip[512] = {0};
	int index = 0;
	clear();
	if(len < url_head_len)
		return false;
	for(int i=0; i<url_head_len; i++)
	{
		if(url[i] != url_head[i])
			return false;
	}
	index = url_head_len;
	
	while (url[index] != ':' && url[index] != '/')
	{
		temp_ip[index-url_head_len] = url[index];
		index ++;
		if(index>500)
			return false;
		if(index == len)
		{
			return check_ip_or_domain(temp_ip, index-url_head_len);
		}
	}
	if(!check_ip_or_domain(temp_ip, index-url_head_len))
		return false;

	if(url[index] == ':')
	{
		index ++;
		int t = index;
		char port[16] = {0};
		while(url[index] != '/')
		{
			port[index-t] = url[index];
			index ++;
			if(index == len)
				break;
		}
		try
		{
			m_port = atoi(port);
			if(m_port == 0)
				return false;
		}
		catch(...)
		{
			return false;
		}
	}
	index ++;
	if(index < len)
		memcpy(m_path, &url[index], len - index);

	return true;
}

bool http_url_parser::check_ip_or_domain(char* buf, int len)
{
	int ip[4];
	int n = sscanf(buf, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
	if(n == 4)
	{
		sprintf(m_ip, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
		sprintf(m_domain, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
		return true;
	}

	buf[len] = 0;
	struct hostent * result = gethostbyname(buf);

	if(result)
	{
		sprintf(m_ip, "%s", inet_ntoa(*((struct in_addr*)(result->h_addr))));
	}
	strcpy(m_domain, buf);
	return true;
}

char* http_url_parser::get_domain()
{
	return m_domain;
}

char* http_url_parser::get_ip()
{
	return m_ip;
}

char* http_url_parser::get_path()
{
	return m_path;
}

unsigned short http_url_parser::get_port()
{
	return m_port;
}
