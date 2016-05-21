/*
-----------------------------------------------------------------------------

File name       :   http_request.h          
Author          :   msl     
Version         :   1.1
Date            :   2013.06.21     
Description     :   http request 解析器
-----------------------------------------------------------------------------
*/
#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__
#include <stdlib.h>
#include <string.h>

//using namespace std;
enum HTTP_REQUEST_METHOD
{
    HTTP_NIL     ,//未知类型
    HTTP_GET     ,//请求获取Request-URI所标识的资源
    HTTP_POST    ,//在Request-URI所标识的资源后附加新的数据
    //以下方法不实现
    HTTP_HEAD    ,//请求获取由Request-URI所标识的资源的响应消息报头
    HTTP_PUT     ,//请求服务器存储一个资源，并用Request-URI作为其标识
    HTTP_DELETE  ,//请求服务器删除Request-URI所标识的资源
    HTTP_TRACE   ,//请求服务器回送收到的请求信息，主要用于测试或诊断
    HTTP_CONNECT ,//保留将来使用
    HTTP_OPTIONS ,//请求查询服务器的性能，或者查询与资源相关的选项和需求
};

enum HTTP_PARSE_RESULT
{
    HTTP_PARSE_OK,              //解析成功
    HTTP_PARSE_FAIL,            //解析失败
    HTTP_PARSE_NOT_COMPLISH     //解析未完成，POST方式才有效
};

template <int MAX_SIZE>
struct http_buffer
{
    char data[MAX_SIZE];
    int  len;
};

typedef struct tag_http_parse_string
{
    const char* s;
    int   len;
}http_parse_string;

typedef struct tag_http_kv_item
{
    http_parse_string name;
    http_parse_string value;
}http_kv_item;

#define HTTP_REQUEST_TEMPLETE_DEFINE int MAX_ARG_SIZE, int MAX_HEAD_SIZE, bool SET_EOF, int MAX_POST_SIZE
#define HTTP_REQUEST_TEMPLETE_IMPL MAX_ARG_SIZE, MAX_HEAD_SIZE, SET_EOF, MAX_POST_SIZE

template<HTTP_REQUEST_TEMPLETE_DEFINE>
class http_request
{
public:
    http_request(void);
    void clear();
    // HTTP_PARSE_RESULT
    int  parse(const char* p, int len);
    const char* get_arg(const char* name, int* len);
    const char* get_head(const char* name, int* len);
    int get_arg_string(const char* name, char* buf, int buflen);
    int get_arg_int(const char* name, int defaultvalue);
    __int64 get_arg_int64(const char* name, __int64 defaultvalue);
    int get_head_string(const char* name, char* buf, int buflen);

    http_kv_item m_head[MAX_HEAD_SIZE];
    int          m_head_count;
    http_kv_item m_arg[MAX_ARG_SIZE];
    int          m_arg_count;

    http_parse_string m_full_path_name;
    http_parse_string m_path;
    http_parse_string m_filename;
    http_parse_string m_filetype;

    http_buffer<MAX_POST_SIZE> m_post;

    int m_method;
    int m_content_length;
    static int parse_arg(const char* p, int len, int* ppos, http_kv_item* kv, int max_kv);
private:
    bool  compare_kv_string(const char* s1, const http_kv_item* s2);
    const char* get_kv_value(const char* s1, int* len, http_kv_item* kv, int kv_count);
    int   parse_method(const char* p, int len, int* ppos);
    int   parse_get(const char* p, int len);
    int   parse_post(const char* p, int len);
    bool  parse_uri(const char* p, int len, int* ppos);
    void  parse_path(const char* p, int len);
    bool  parse_head(const char* p, int len, int* ppos);
};

template<HTTP_REQUEST_TEMPLETE_DEFINE>
http_request<HTTP_REQUEST_TEMPLETE_IMPL>::http_request()
{
    clear();
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
void http_request<HTTP_REQUEST_TEMPLETE_IMPL>::clear()
{
    m_head_count = 0;
    m_arg_count = 0;
    m_path.len = 0;
    m_filename.len = 0;
    m_full_path_name.len = 0;
    m_method = HTTP_NIL;
    m_content_length = 0;
    m_post.len = 0;
    for(int i=0; i<MAX_HEAD_SIZE; i++)
    {
        m_head[i].name.len = 0;
        m_head[i].value.len = 0;
        m_head[i].name.s = 0;
        m_head[i].value.s = 0;
    }
    for(int i=0; i<MAX_ARG_SIZE; i++)
    {
        m_arg[i].name.len = 0;
        m_arg[i].value.len = 0;
        m_arg[i].name.s = 0;
        m_arg[i].value.s = 0;
    }
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
const char* http_request<HTTP_REQUEST_TEMPLETE_IMPL>::get_arg(const char* name, int* len)
{
    return get_kv_value(name, len, m_arg, m_arg_count);
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
const char* http_request<HTTP_REQUEST_TEMPLETE_IMPL>::get_head(const char* name, int* len)
{
    return get_kv_value(name, len, m_head, m_head_count);
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
int http_request<HTTP_REQUEST_TEMPLETE_IMPL>::get_arg_string(const char* name, char* buf, int buflen)
{
    int len = 0;
    const char* p = get_arg(name, &len);
    if(!p || len<=0)
        return -1;
    if(len >= buflen)
        return 0;
    memcpy(buf, p, len);
    buf[len] = 0;
    return len;
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
int http_request<HTTP_REQUEST_TEMPLETE_IMPL>::get_arg_int(const char* name, int defaultvalue)
{
    if(SET_EOF)
    {
        int len = 0;
        const char* buf = get_arg(name, &len);
        if(buf && len>0)
            return atoi(buf);
    }
    else
    {
        char buf[64] = {0};
        if(get_arg_string(name, buf, 60) > 0)
        {
            return atoi(buf);
        }
    }
    return defaultvalue;
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
__int64 http_request<HTTP_REQUEST_TEMPLETE_IMPL>::get_arg_int64(const char* name, __int64 defaultvalue)
{
    if(SET_EOF)
    {
        int len = 0;
        const char* buf = get_arg(name, &len);
        if(buf && len>0)
            return _atoi64(buf);
    }
    else
    {
        char buf[64] = {0};
        if(get_arg_string(name, buf, sizeof(buf)) > 0)
        {
            return _atoi64(buf);
        }
    }
    return defaultvalue;
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
int http_request<HTTP_REQUEST_TEMPLETE_IMPL>::get_head_string(const char* name, char* buf, int buflen)
{
    int len = 0;
    const char* p = get_head(name, &len);
    if(!p || len<=0)
        return -1;
    if(len >= buflen)
        return 0;
    memcpy(buf, p, len);
    return len;
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
const char* http_request<HTTP_REQUEST_TEMPLETE_IMPL>::get_kv_value(const char* s1, int* len, http_kv_item* kv, int kv_count)
{
    for(int i=0; i<kv_count; i++)
    {
        if(compare_kv_string(s1, &kv[i]))
        {
            *len = kv[i].value.len;
            return kv[i].value.s;
        }
    }
    *len = 0;
    return 0;
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
bool http_request<HTTP_REQUEST_TEMPLETE_IMPL>::compare_kv_string(const char* s1, const http_kv_item* kv)
{
    if(!s1 || !kv || kv->name.len==0)
        return false;
    int i;
    char buf[1024];
    int slen = (int)strlen(s1);
    for(i=0; i<slen; i++)
        buf[i] = tolower(s1[i]);
    buf[i] = 0;
    if(slen != kv->name.len) return false;
    for(i=0; i<kv->name.len; i++)
    {
        if(!buf[i]) return false;
        if(buf[i] != tolower(kv->name.s[i])) return false;
    }
    return true;
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
int http_request<HTTP_REQUEST_TEMPLETE_IMPL>::parse(const char* p, int len)
{
    int ppos = 0;
    m_method = parse_method(p, len, &ppos);
    if(m_method == HTTP_NIL)
        return HTTP_PARSE_FAIL;
    switch(m_method)
    {
    case HTTP_GET:
        return parse_get(&p[ppos], len-ppos);
    case HTTP_POST:
        return parse_post(&p[ppos], len-ppos);
    default:
        break;
    }
    return HTTP_PARSE_FAIL;
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
int http_request<HTTP_REQUEST_TEMPLETE_IMPL>::parse_method(const char* p, int len, int* ppos)
{
    if(len < 5 || !p)
        return HTTP_NIL;
    if(p[0] == 'G' && p[1] == 'E' && p[2] == 'T' && p[3] == ' ')
    {
        *ppos = 4;
        return HTTP_GET;
    }
    if(p[0] == 'P' && p[1] == 'O' && p[2] == 'S' && p[3] == 'T' && p[4] == ' ')
    {
        *ppos = 5;
        return HTTP_POST;
    }
    //不支持其他类型的method
    return HTTP_NIL;
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
int http_request<HTTP_REQUEST_TEMPLETE_IMPL>::parse_get(const char* p, int len)
{
    int ppos = 0;
    if(!parse_uri(p, len, &ppos))
        return HTTP_PARSE_FAIL;
    int tppos = 0;
    if(!parse_head(&p[ppos], len - ppos, &tppos))
        return HTTP_PARSE_FAIL;

    ppos += tppos;

    return HTTP_PARSE_OK;
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
int http_request<HTTP_REQUEST_TEMPLETE_IMPL>::parse_post(const char* p, int len)
{
    if(m_content_length > 0)
    {
        int copylen = len;
        if(copylen + m_post.len > m_content_length)
            copylen = m_content_length - m_post.len;
        memcpy(&m_post.data[m_post.len], p, copylen);
        if(m_post.len == m_content_length)
            return HTTP_PARSE_OK;
        return HTTP_PARSE_NOT_COMPLISH;
    }
    int ppos = 0;
    if(!parse_uri(p, len, &ppos))
        return HTTP_PARSE_FAIL;
    int tppos = 0;
    if(!parse_head(&p[ppos], len - ppos, &tppos))
        return HTTP_PARSE_FAIL;
    ppos += tppos;

    char content_length[64];
    int cl = get_head_string("Content Length", content_length, 64);
    if(cl <= 0)
        return HTTP_PARSE_FAIL;

    m_content_length = atoi(content_length);
    if(m_content_length == 0)
        return HTTP_PARSE_OK;
    
    if(m_content_length>MAX_POST_SIZE)
        return HTTP_PARSE_FAIL;
    m_post.data[m_content_length] = 0;

    int copylen = len - ppos;
    if(copylen > m_content_length)
        copylen = m_content_length;
    memcpy(m_post.data, &p[ppos], copylen);
    m_post.len = copylen;

    if(m_post.len == m_content_length)
        return HTTP_PARSE_OK;
    return HTTP_PARSE_NOT_COMPLISH;
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
bool http_request<HTTP_REQUEST_TEMPLETE_IMPL>::parse_uri(const char* p, int len, int* ppos)
{
    //parse full_path_name
    bool b_path = false;
    bool has_arg = false;
    int tppos = 0;
    m_full_path_name.s = p;
    m_full_path_name.len = -1;
    for(int i=0; i<len; i++)
    {
        if(p[i] == ' ' || p[i] == '?')
        {
            b_path = true;
            m_full_path_name.len = i;
            *ppos = i+1;
            if(p[i] == '?')
                has_arg = true;
            break;
        }
        if(p[i] == '\n')
            return false;
    }
    if(!b_path)
        return false;
    parse_path(m_full_path_name.s, m_full_path_name.len);
    tppos = 0;
    if(has_arg)
    {
        for(int i=0; i<MAX_ARG_SIZE; i++)
        {
            m_arg[i].name.len = 0;
            m_arg[i].value.len = 0;
        }
        m_arg_count = parse_arg(&p[*ppos], len - *ppos, &tppos, m_arg, MAX_ARG_SIZE);
        if(m_arg_count < 0)
            return false;
        *ppos += tppos;
    }
    //跳过第一行
    for(; (*ppos)<len; (*ppos)++)
    {
        if(p[(*ppos)] == '\n')
        {
            (*ppos) ++;
            return true;
        }
    }
    return false;
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
void http_request<HTTP_REQUEST_TEMPLETE_IMPL>::parse_path(const char* p, int len)
{
    m_path.len = 0;
    m_filename.len = 0;
    m_filetype.len = 0;
    for(int i=len-1; i>=0; i--)
    {
        if(m_filetype.len == 0 && p[i] == '.')
        {
            m_filetype.s = &p[i+1];
            m_filetype.len = len - i - 1;
        }
        if(m_path.len == 0 && p[i] == '/')
        {
            m_path.s = p;
            m_path.len = i + 1;
            m_filename.s = &p[i+1];
            m_filename.len = len - i - 1;
            break;
        }
    }
    if(SET_EOF)
        ((char*)(m_full_path_name.s))[m_full_path_name.len] = 0;
#if 0
    char s1[1024] = {0};
    char s2[1024] = {0};
    char s3[1024] = {0};
    char s4[1024] = {0};
    memcpy(s1, m_full_path_name.s, m_full_path_name.len);
    memcpy(s2, m_path.s, m_path.len);
    memcpy(s3, m_filename.s, m_filename.len);
    memcpy(s4, m_filetype.s, m_filetype.len);
#endif
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
int http_request<HTTP_REQUEST_TEMPLETE_IMPL>::parse_arg(const char* p, int len, int* ppos, http_kv_item* kv, int max_kv)
{
    int arg_count = 0;
    *ppos = 0;
    bool pase_key = true;
    for(int i=0; i<len; i++)
    {
        if(p[i] == ' ' || p[i] == '\n' || p[i] == 0)
        {
            kv[arg_count].value.len = i - *ppos;
            if(kv[arg_count].value.len > 0)
                kv[arg_count].value.s = &p[*ppos];
            else
                kv[arg_count].value.s = 0;
            *ppos = i + 1;
            arg_count ++;
            break;
        }
        if(p[i] == '=')
        {
            if(pase_key)
            {
                pase_key = false;
                kv[arg_count].name.s = &p[*ppos];
                kv[arg_count].name.len = i - *ppos;
                *ppos = i + 1;
            }
            else
            {
                return -1;
            }
        }
        else if(p[i] == '&')
        {
            if(pase_key)
                return false;
            pase_key = true;
            
            kv[arg_count].value.len = i - *ppos;
            if(kv[arg_count].value.len > 0)
                kv[arg_count].value.s = &p[*ppos];
            else
                kv[arg_count].value.s = 0;
            *ppos = i + 1;
            arg_count ++;
        }
    }

    if(SET_EOF)
    {
        for(int i=0; i<arg_count; i++)
        {
            if(kv[i].name.s && kv[i].name.len>0)
                ((char*)(kv[i].name.s))[kv[i].name.len] = 0;
            if(kv[i].value.s && kv[i].value.len>0)
                ((char*)(kv[i].value.s))[kv[i].value.len] = 0;
        }
    }

    return arg_count;
}

template<HTTP_REQUEST_TEMPLETE_DEFINE>
bool http_request<HTTP_REQUEST_TEMPLETE_IMPL>::parse_head(const char* p, int len, int* ppos)
{
    m_head_count = 0;
    *ppos = 0;
    bool pase_key = true;
    for(int i=0; i<len; i++)
    {
        if(pase_key)
        {
            if(p[i] == '\n')
            {
                *ppos = i + 1;
                break;
            }
            if(p[i] == ':')
            {
                pase_key = false;
                m_head[m_head_count].name.s = &p[*ppos];
                m_head[m_head_count].name.len = i - *ppos;
                *ppos = i + 2;
            }
        }
        else
        {
            if(p[i] == '\n')
            {
                pase_key = true;
                m_head[m_head_count].value.len = i - *ppos;
                if(m_head[m_head_count].value.len > 0)
                    m_head[m_head_count].value.s = &p[*ppos];
                else
                    m_head[m_head_count].value.s = 0;
                *ppos = i + 1;
                m_head_count ++;
            }
        }
    }

    if(SET_EOF)
    {
        for(int i=0; i<m_head_count; i++)
        {
            ((char*)(m_head[i].name.s))[m_head[i].name.len] = 0;
            ((char*)(m_head[i].value.s))[m_head[i].value.len] = 0;
        }
    }

    return true;
}

#define DEFAULT_SEND_FORMAT "HTTP/1.1 200 OK\r\n"\
    "Cache-Control: private\r\n"\
    "Server: %s\r\n"\
    "Content-Length: %d\r\n"\
    "Content-Type: text/html;charset=gb2312\r\n"\
    "Connection: Close\r\n"\
    "\r\n%s"

#if defined(_WIN32) || defined(_WIN64)
#define FORMAT_SEND_BUF(_buf, _buflen, _servername, _src, _srclen, _outlen) \
    _outlen = sprintf_s(_buf, _buflen, DEFAULT_SEND_FORMAT, _servername, _srclen, _src);
#endif

#endif//__HTTP_REQUEST_H__