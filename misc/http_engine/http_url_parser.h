#pragma once

class http_url_parser
{
public:
	http_url_parser(void);
	~http_url_parser(void);

	bool parser(const char* url, int len);
	char* get_domain();
	char* get_ip();
	char* get_path();
	unsigned short get_port();

private:
	void clear();
	bool check_ip_or_domain(char* buf, int len);
private:
	char m_domain[256];
	char m_ip[32];
	unsigned short m_port;
	char m_path[1024];
};
