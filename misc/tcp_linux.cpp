#include "tcp.h"
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _ANDROID
#include <android/log.h>
#define LOG_TAG   "lib_tcp"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WORNING,LOG_TAG,__VA_ARGS__)
#else
#define LOGI(...) printf(__VA_ARGS__)
#define LOGE(...) printf(__VA_ARGS__)
#define LOGW(...) printf(__VA_ARGS__)
#endif
#define close_socket(s) if(s!=0)::close(s);s=0;

bool setnonblocking(int fd) {
	int opts;
	opts = fcntl(fd, F_GETFL);
	if (opts < 0)
		opts = O_NONBLOCK;
	else
		opts = opts | O_NONBLOCK;
	if (fcntl(fd, F_SETFL, opts) < 0) {
		LOGE("F_SETFL fail");
		return false;
	}
#if 1
	///////////////////////////////////////////////////////////////////////
	int val = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val)) == -1) {
        LOGE("SO_KEEPALIVE fail");
        return false;
    }
    val = 5;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &val, sizeof(val)) < 0) {
        LOGE("TCP_KEEPIDLE Fail");
        return false;
    }

    val = 5;
    if (val == 0) val = 1;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &val, sizeof(val)) < 0) {
        LOGE("TCP_KEEPINTVL Fail");
        return false;
    }

    val = 3;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &val, sizeof(val)) < 0) {
        LOGE("TCP_KEEPCNT Fail");
        return false;
    }
	///////////////////////////////////////////////////////////////////////
#endif
	return true;
}

CTcp::CTcp(void) {
	memset(m_recv_buf, 0, sizeof(m_recv_buf));
	m_recv_len = 0;
	m_fd = -1;
}

CTcp::~CTcp(void) {

}

void CTcp::init(i_tcp_service* service) {
	m_service = service;
}

void CTcp::close() {
    if(m_fd >=0)
	    close_socket(m_fd);
	m_fd = -1;
	m_is_connected = false;
	m_state = TCP_STATE_CLOSE;
}

bool CTcp::connect(const char* ip, unsigned short port) {
	if (!m_service)
		return false;
	m_recv_len = 0;
	m_is_connected = false;
	m_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_fd < 0) {
		LOGE("fd=%d errno=%d\n", m_fd, errno);
		return false;
	}
	if (!setnonblocking(m_fd)) {
		close_socket(m_fd);
		LOGE("setnonblocking fail");
		return false;
	}
	
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (inet_aton(ip, (struct in_addr *) &addr.sin_addr.s_addr) == 0) {
		close_socket(m_fd);
		LOGE("inet_aton fail");
		return false;
	}

	int n = ::connect(m_fd, (struct sockaddr*) &addr, sizeof(struct sockaddr));
	if (n < 0) {
		if (errno != EINPROGRESS) {
			LOGE("connect fail. n=%d errno=%d", n, errno);
			return false;
		}
	}
	

	if (n == 0) {
		m_is_connected = true;
		LOGE("connect ok.");
	} else {
		LOGE("connect=%d\n", n);
	}
    m_state = TCP_STATE_CONNECTING;
	activate(1);
	return true;
}

bool CTcp::send(const char* pdata, int len) {
	::send(m_fd, pdata, len, 0);
	return true;
}

bool CTcp::is_connected() {
	return (m_fd != 0);
}

bool CTcp::on_recv() {
	return true;
}

void CTcp::send_heart_test()
{
	char* data = 0;
	int   len = 0;
	if(m_service->get_heart_test_data(&data, &len) && data && len > 0)
	{
		send(data, len);
	}
}

bool wait_connect(int fd) {
	fd_set rset;
	fd_set wset;
	FD_ZERO(&rset);
	FD_SET(fd, &rset);
	wset = rset;
	struct timeval tval;
	tval.tv_sec = 5;
	tval.tv_usec = 0;
	int error;
	socklen_t len;
	int n = select(fd+1, NULL, &wset, NULL, &tval);
	if (n <= 0) {
		LOGE("time out connect error\n");
		return false;
	} else {
		len = sizeof(error);
		getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len);
		if (error) {
			LOGE("getsockopt error%d\n", error);
			return false;
		}
	}
	return true;
}

int CTcp::svc() {
	if (!m_is_connected) {
		if (!wait_connect(m_fd)) {
			m_service->on_connect(-1);
			close();
			return 0;
		}
	}
    m_state = TCP_STATE_CONNECTED;

	m_service->on_connect(0);
	m_recv_len = 0;
	while (true) {
		struct timeval tv;
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		fd_set rset;
		FD_ZERO(&rset);
		FD_SET(m_fd, &rset);

		int n = ::select(m_fd+1, &rset, NULL, NULL, &tv);
		if (n <= 0) {
			if (errno == 0 || errno == EINTR || errno == EWOULDBLOCK
					|| errno == EAGAIN) {
				LOGE("read time out: on select\n");
				send_heart_test();
				continue;
			}
			LOGE("select n=%d errno=%d\n", n, errno);
			break;
		}

		n = ::recv(m_fd, &m_recv_buf[m_recv_len],
				sizeof(m_recv_buf) - m_recv_len, 0);

		if (n <= 0) {
			if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
				LOGE("read time out: on recv\n");
				continue;
			}
			LOGE("recv n=%d, errno=%d\n", n, errno);
			break;
		}
		m_recv_len += n;
		bool err = false;
		while (true) {
			int len = m_service->on_check_pack(m_recv_buf, m_recv_len);
			if (len == 0)
				break;
			if (len < 0 || len > m_recv_len) {
				err = true;
				break;
			}
			if (!m_service->on_recv(m_recv_buf, len)) {
				err = true;
				break;
			}
			m_recv_len -= len;
			if (m_recv_len == 0)
				break;
			for (int i = 0; i < m_recv_len; i++) {
				m_recv_buf[i] = m_recv_buf[len + i];
			}
		}
		if (err)
			break;
	}
	close();
	m_service->on_close();
	return 0;
}
