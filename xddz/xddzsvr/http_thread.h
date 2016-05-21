#ifndef __HTTP_THREAD_H__
#define __HTTP_THREAD_H__
#include "singleton.h"
#include "ring_buffer2_thread.h"


class http_thread : public singleton<http_thread>, public ring_buffer2_thread<1>
{
public:
    http_thread(void);
    ~http_thread(void);
    void http_get(char* url);
    virtual void on_data(void* phead, int headlen, void* pdata, int datalen);

private:
    void on_http_get(char* url);

};
#define http http_thread::instance()


#endif//__HTTP_THREAD_H__