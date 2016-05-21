#include "proxy_con_item.h"
#include "i_http_get_proxy_engin.h"
#include <winsock2.h>
#include "i_multiplex_proxy.h"
#include "util.h"
#include "cflog.h"

shared_thread_mutex proxy_con_item::m_connect_mutex_pool(50);

#ifdef _DEBUG
#pragma comment(lib, "libcurld.lib")
#else
#pragma comment(lib, "libcurl.lib")
#endif


static int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms)
{
	struct timeval tv;
	fd_set infd, outfd, errfd;
	int res;

	tv.tv_sec = timeout_ms / 1000;
	tv.tv_usec= (timeout_ms % 1000) * 1000;

	FD_ZERO(&infd);
	FD_ZERO(&outfd);
	FD_ZERO(&errfd);

	FD_SET(sockfd, &errfd); /* always check for error */ 

	if(for_recv)
	{
		FD_SET(sockfd, &infd);
	}
	else
	{
		FD_SET(sockfd, &outfd);
	}

	/* select() returns the number of signalled sockets or -1 */ 
	res = select(sockfd + 1, &infd, &outfd, &errfd, &tv);
	return res;
}

proxy_con_item::proxy_con_item(int fd)
{
	m_fd = fd;
	m_is_free = true;
	m_buf = 0;
	curl = 0;
	m_buf_len = 0;
	memset(&m_overlapped, 0, sizeof(m_overlapped));
	m_sockfd = 0;

	m_mutex = &m_connect_mutex_pool;
	m_is_connecting = false;
    m_head_buf_len = 0;
    m_recv_len = 0;
    m_content_length = 0;
}

proxy_con_item::~proxy_con_item(void)
{
	if(m_buf)
	{
		delete[] m_buf;
		m_buf = 0;
	}
	m_buf_len = 0;
}

bool proxy_con_item::is_free()
{
	guard g(m_mutex);
	return m_is_free;
}

bool proxy_con_item::set_is_free(bool b)
{
	guard g(m_mutex);
	m_is_free = b;
	return m_is_free;
}

bool proxy_con_item::init(int buflen)
{
	m_buf = new char[buflen];
	m_buf_len = buflen;
	return true;
}

int proxy_con_item::get_fd()
{
	return m_fd;
}

void proxy_con_item::cancel_get()
{
	guard g(m_mutex);
	if(m_is_connecting)
	{
		m_is_connecting = false;
		return;
	}
	clear_up();
}

void proxy_con_item::clear_up()
{
	if(curl)
		curl_easy_cleanup(curl);
	curl = 0;
	m_sockfd = 0;
	m_pdata = 0;
    m_head_buf_len = 0;
	m_is_free = true;
    m_recv_len = 0;
    m_content_length = 0;
}

void proxy_con_item::set_data(void* pdata)
{
	m_pdata = pdata;
}

void* proxy_con_item::get_data()
{
	return m_pdata;
}

bool proxy_con_item::is_connecting()
{
	guard g(m_mutex);
	return m_is_connecting;
}

void proxy_con_item::set_connecting(bool b)
{
	guard g(m_mutex);
	m_is_connecting = b;
}

bool proxy_con_item::get_http(const char* uri, int urilen, HANDLE hCompPort)
{
	guard g(m_mutex);

    m_head_buf_len = 0;
    m_recv_len = 0;
    m_content_length = 0;

	if(!m_is_connecting)
		return false;

	if(!m_url_parser.parser(uri, strlen(uri)))
		return false;
	m_b_head = true;
	CURLcode res;
	size_t iolen = 0;
	char request[2048] = {0};
#pragma warning(disable:4996)
	sprintf(request, "GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n",
		m_url_parser.get_path(), m_url_parser.get_domain());
#pragma warning(default:4996)

	bool err = true;
	try
	{
		do
		{
			curl = curl_easy_init();
			if(!curl)
				break;
#ifdef _DEBUG
//		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif
			curl_easy_setopt(curl, CURLOPT_URL, uri);
			curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 3000L);

			res = curl_easy_perform(curl);
			if(CURLE_OK != res)
				break;
			long sockextr;
			res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &sockextr);
			if(CURLE_OK != res)
				break;
			m_sockfd = sockextr;
			if(m_sockfd==INVALID_SOCKET || m_sockfd == 0)
				break;
			if(!wait_on_socket(m_sockfd, 0, 1000L))
				break;
			iolen = 0;
			res = curl_easy_send(curl, request, strlen(request), &iolen);
			if(CURLE_OK != res)
				break;
			HANDLE h = CreateIoCompletionPort((HANDLE)m_sockfd, hCompPort,(ULONG_PTR)this, 1);

			int time_out=1000;//设置接收超时
			if(::setsockopt(m_sockfd ,SOL_SOCKET,SO_RCVTIMEO,(char *)&time_out, sizeof(time_out))==SOCKET_ERROR)
				break;

			if(!h)
				break;
			if(!reg_read())
				break;
			err = false;
		} while (false);
	}
	catch(...)
	{
		err = true;
	}
	m_is_connecting = false;
	if(err)
	{
		if(curl)
		{
			curl_easy_cleanup(curl);
			curl = 0;
			m_sockfd = 0;
		}
		return false;
	}

	return true;
}

bool proxy_con_item::get_content_length()
{
    const char* len_tag = "content-length";
    const int length_tag_len = strlen(len_tag);
    char  tmp[40960];
    int   bl = sizeof(tmp)<m_recv_len?sizeof(tmp):m_recv_len;
    int i;
    int j;
    
    memcpy(tmp, m_buf, bl);
    for(i=0; i<bl; i++)
        tmp[i] = tolower(tmp[i]);

    if(m_content_length == 0)
    {
        //寻找length标签
        bool find_length = false;
        for(i=0; i<m_recv_len - length_tag_len; i++)
        {
            find_length = true;
            for(j=0; j<length_tag_len; j++)
            {
                if(tmp[i+j] != len_tag[j])
                {
                    find_length = false;
                    break;
                }
            }
            if(!find_length)
                continue;

            //寻找length长度
            char len_tmp[32] = {0};
            int  len_index = 0;
            int  n = (i+30)<m_recv_len?(i+30):m_recv_len;
            for(int j=i+length_tag_len; j<n; j++)
            {
                if(tmp[j]>='0' && tmp[j] <= '9')
                {
                    len_tmp[len_index] = tmp[j];
                    len_index ++;
                }
                else if(tmp[j] == '\n')
                {
                    if(len_index == 0)
                        return false;
                    m_content_length = atoi(len_tmp);
                }
            }
            break;
        }
        if(m_content_length <= 0)
            return false;
    }
    return true;
}

bool proxy_con_item::filter_head(int& datalen)
{
    if(!get_content_length())
        return false;
//    char temp[40960] = {0};
//    memcpy(temp, m_buf, datalen);
	const char* span = "\r\n\r\n";
	int index = -1;

	for(int i=0; i<datalen-4; i++)
	{
		if(m_buf[i]  == span[0] &&
		   m_buf[i+1]== span[1] &&
		   m_buf[i+2]== span[2] &&
		   m_buf[i+3]== span[3])
		{
			index = i;
			break;
		}
	}
    
    if(index >= 0)
	{
		memcpy(m_buf, &m_buf[index + 4], (datalen-index-4));
		datalen = datalen - index -4;
		m_b_head = false;
        m_recv_len -= (index+4);
		return true;
	}
	return false;
}

bool proxy_con_item::on_event(i_http_get_proxy_service* service)
{
	guard g(m_mutex);
	if(!curl)
		return false;
	if(!curl)
		return false;
	char* p = 0;
	int datalen = 0;
	int state;
	size_t iolen;
	bool ret = true;
	CURLcode res = curl_easy_recv(curl, &m_buf[m_head_buf_len], m_buf_len-m_head_buf_len, &iolen);
    m_recv_len += iolen;
	if(res == CURLE_OK && iolen>0)
	{
		p = m_buf;
		if(m_b_head)
		{
			m_head_buf_len += iolen;
			datalen = m_head_buf_len;
			if(!filter_head(datalen))
			{
				p = 0;
				datalen = 0;
			}
			else
			{
				m_head_buf_len = 0;
			}
		}
		else
			datalen = iolen;
		if(!reg_read())
			state = END_OF_DATA;
		else
			state = HAS_MORE_DATA;
		if(datalen > 0)
		{
			if(!service->on_get_result(m_fd, p, datalen, state))
				return false;
		}
        if(m_recv_len >= m_content_length)
        {
            LOG_DEBUG("转发完HTTP数据, 断开HTTP连接:m_recv_len=%d, m_content_length=%d\n", m_recv_len, m_content_length);

            closesocket(m_sockfd);
        }
		return true;
	}
	curl_easy_cleanup(curl);
	curl = 0;
	m_sockfd = 0;
	service->on_get_result(m_fd, 0, 0, END_OF_DATA);
	return false;
}
/*
bool proxy_con_item::get_data(char** p, int& datalen, int& state)
{
	guard g(m_mutex);
	if(!curl)
		return false;
	size_t iolen;
	CURLcode res = curl_easy_recv(curl, &m_buf[m_head_buf_len], m_buf_len-m_head_buf_len, &iolen);
    m_recv_len += iolen;
	if(res == CURLE_OK && iolen>0)
	{
		*p = m_buf;
		if(m_b_head)
		{
            m_head_buf_len += iolen;
            datalen = m_head_buf_len;
			if(!filter_head(datalen))
			{
				*p = 0;
				datalen = 0;
			}
            else
            {
                m_head_buf_len = 0;
            }
		}
        else
            datalen = iolen;
		if(!reg_read())
			state = END_OF_DATA;
		else
			state = HAS_MORE_DATA;
		return true;
	}
	curl_easy_cleanup(curl);
	curl = 0;
	m_sockfd = 0;
	
	return false;
}
*/
bool proxy_con_item::reg_read()
{
	DWORD dwThancferred = 0;
	DWORD dwFlags = 0;
	WSABUF rdbuf;
	memset(&rdbuf, 0, sizeof(rdbuf));
	::WSARecv(m_sockfd,&rdbuf,1,&dwThancferred,&dwFlags,(LPWSAOVERLAPPED)&m_overlapped,NULL);
	return true;
}