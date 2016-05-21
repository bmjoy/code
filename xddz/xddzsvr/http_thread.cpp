#include "http_thread.h"
#include "crul/http_get_string.h"


enum HTTP_GET_TYPE
{
    HTTP_GET_SEND,
};

http_thread::http_thread(void)
{
}


http_thread::~http_thread(void)
{
}

void http_thread::http_get(char* url)
{
    int type = HTTP_GET_SEND;
    put_data(&type, sizeof(int), url, strlen(url)+1);
}


void http_thread::on_data(void* phead, int headlen, void* pdata, int datalen)
{
    int type = *(int*)phead;
    switch (type)
    {
    case HTTP_GET_SEND:
        {
            char* url = (char*)pdata;
            on_http_get(url);
        }
    default:
        break;
    }
}

void http_thread::on_http_get(char* url)
{
    char buf[1024];
    get_http_string(url, buf, sizeof(buf));
}
