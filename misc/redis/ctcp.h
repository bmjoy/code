#ifndef __C_TCP_CLIENT_H__
#define __C_TCP_CLIENT_H__


class ctcp
{
public:
    ctcp(void);
    ~ctcp(void);
    bool init(bool is_keep_alive=true, bool is_no_block=false, int default_timeout=0);
    bool connect(const char* ip, int port);
    bool is_connected();
    void close();
    int read(char* buf, int len, int timeout=0);
    int write(char* buf, int len);
    int get_last_error();
    const char* get_err_msg();
private:
    int set_read_time_out(int t);
private:
    int  m_fd;
    int  m_last_err;
    char m_err[256];
    bool m_is_keep_alive;
    bool m_is_no_block;
    int  m_default_timeout;
    int  m_last_timeout;
};


#endif//__C_TCP_CLIENT_H__
