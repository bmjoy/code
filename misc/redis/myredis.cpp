#include "myredis.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <Windows.h>
#pragma warning(disable:4996)
#endif

#define RDSACK_SINGLELINE  '+' //With a single line reply the first byte of the reply will be "+"
#define RDSACK_ERROR       '-' //With an error message the first byte of the reply will be "-"
#define RDSACK_INTEGER     ':' //With an integer number the first byte of the reply will be ":"
#define RDSACK_BULK        '$' //With bulk reply the first byte of the reply will be "$"
#define RDSACK_MULTIBULK   '*' //With multi-bulk reply the first byte of the reply will be "*"

#define REST_ERR m_err[0]=0

#define fmt_lcmd(x, ...)     m_buffer.datasize= snprintf(m_buffer.data, m_buffer.bufsize-1, x, __VA_ARGS__)

#ifdef _WIN32
#define snprintf _snprintf
#endif

const char* i2a(char* buf, int n)
{
    sprintf(buf, "%d", n);
    return buf;
}

myredis::myredis(void)
{
    REST_ERR;
    m_db = 0;
    m_buffer.bufsize = 0;
    m_buffer.data = 0;
    m_buffer.offset = 0;
    m_buffer.datasize = 0;

    m_bulklist.bulk_list = 0;
    m_bulklist.index = -1;
    m_bulklist.maxcount = 0;
    m_bulklist.response_count = 0;
}

myredis::~myredis(void)
{
}


bool myredis::init()
{
    m_buffer.bufsize = DEF_REDIS_BUFFER_SIZE;
    if(!(m_buffer.data = (char*)malloc(m_buffer.bufsize)))
        return false;

    if(!(m_bulklist.bulk_list = (redis_bulk*)calloc(DEF_REDIS_BULK_COUNT, sizeof(redis_bulk))))
        return false;
    m_bulklist.maxcount = DEF_REDIS_BULK_COUNT;

    if(!m_tcp.init())
        return false;

    clear_buf();
    return true;
}

void myredis::fini()
{
    free(m_buffer.data);
    free(m_bulklist.bulk_list);
    m_tcp.close();
}

bool myredis::connect(const char* ip, int port, const char* pwd, int ndb)
{
    if(!m_tcp.connect(ip, port))
        return false;
    if(pwd && strlen(pwd))
    {
        if(!auth(pwd))
            return false;
    }
    if(ndb != 0)
    {
        if(!select(ndb))
            return false;
    }
    return true;
}

bool myredis::close()
{
    m_tcp.close();

    return true;
}

bool myredis::is_connected()
{
    return m_tcp.is_connected();
}

const char* myredis::get_err()
{
    return m_err;
}

bool myredis::is_error()
{
    return (m_err[0] != 0);
}

int myredis::get_db()
{
    return m_db;
}

void myredis::clear_buf()
{
    REST_ERR;
    m_buffer.datasize = 0;
    m_buffer.offset = 0;
    m_bulklist.response_count = 0;
    m_bulklist.index = 0;
    m_bulklist.bulk_list[0].data = 0;
    m_bulklist.bulk_list[0].len = 0;
}

bool myredis::fmt_param(const char* val)
{
    int offset = 0;
    offset = snprintf(&m_buffer.data[m_buffer.datasize], (m_buffer.bufsize - m_buffer.datasize - 1), "$%d\r\n%s\r\n", ::strlen(val), val);
    if(offset < 0)
        return false;
    m_buffer.datasize += offset;
    return true;
}

int myredis::get_val_count(const char** val)
{
    int i=0, count=0;
    while(val[i++])count++;
    return count;
}

bool myredis::fmt_param(const char* val, int len)
{
    int offset = 0;
    offset = snprintf(&m_buffer.data[m_buffer.datasize], (m_buffer.bufsize - m_buffer.datasize - 1), "$%d\r\n", len);

    if(offset < 0)
        return false;
    m_buffer.datasize += offset;

    if((m_buffer.bufsize-m_buffer.datasize) <= len+3)//回车换行符号+0结尾符
        return false;

    memcpy(&m_buffer.data[m_buffer.datasize], val, len);
    m_buffer.datasize += len;
    memcpy(&m_buffer.data[m_buffer.datasize], "\r\n\0", 3);
    m_buffer.datasize += 2;
    return true;
}

bool myredis::send_cmd(const char* cmd, const char* val)
{
    REST_ERR;
    if(!cmd || !val)
    {
        sprintf(m_err, "param err");
        return false;
    }
    m_buffer.datasize = snprintf(m_buffer.data, m_buffer.bufsize, "*2\r\n");
    if(!fmt_param(cmd)) return false;
    if(!fmt_param(val)) return false;
    return send_request();
}

bool myredis::send_cmd(const char* cmd, const char** val)
{
    REST_ERR;
    if(!cmd || !val)
    {
        sprintf(m_err, "param err");
        return false;
    }
    int n = get_val_count(val);
    m_buffer.datasize = snprintf(m_buffer.data, m_buffer.bufsize, "*%d\r\n", n+1);
    if(!fmt_param(cmd)) return false;
    for(int i=0; i<n; i++)
    {
        if(!fmt_param(val[i]))
        {
            return false;
        }
    }
    return send_request();
}

bool myredis::send_cmd(const char* cmd, const char* val1, const char* val2)
{
    REST_ERR;
    if(!cmd || !val1 || !val2)
    {
        sprintf(m_err, "param err");
        return false;
    }
    m_buffer.datasize = snprintf(m_buffer.data, m_buffer.bufsize, "*3\r\n");
    if(!fmt_param(cmd)) return false;
    if(!fmt_param(val1)) return false;
    if(!fmt_param(val2)) return false;
    return send_request();
}

bool myredis::send_cmd(const char* cmd, const char* val1, const char* val2, const char* val3, int len3)
{
    if(!cmd || !val1 || !val2 || !val3)
    {
        sprintf(m_err, "param err");
        return false;
    }
    m_buffer.datasize = snprintf(m_buffer.data, m_buffer.bufsize, "*4\r\n");
    if(!fmt_param(cmd)) return false;
    if(!fmt_param(val1)) return false;
    if(!fmt_param(val2)) return false;
    if(len3>0)
    {
        if(!fmt_param(val3, len3))
            return false;
    }
    else
    {
        if(!fmt_param(val3))
            return false;
    }
    return send_request();
}

bool myredis::send_cmd(const char* cmd, const char* val1, const char* val2, int l2)
{
    REST_ERR;
    if(!cmd || !val1 || !val2 || !l2)
    {
        sprintf(m_err, "param err");
        return false;
    }
    m_buffer.datasize = snprintf(m_buffer.data, m_buffer.bufsize, "*3\r\n");
    if(!fmt_param(cmd)) return false;
    if(!fmt_param(val1)) return false;
    if(!fmt_param(val2, l2)) return false;
    return send_request();
}

bool myredis::send_cmd(const char* cmd, const char* val1, const char** val2)
{
    REST_ERR;
    if(!cmd || !val1 || !val2)
    {
        sprintf(m_err, "param err");
        return false;
    }
    int n = get_val_count(val2);
    if(n == 0)
    {
        sprintf(m_err, "param err");
        return false;
    }
    m_buffer.datasize = snprintf(m_buffer.data, m_buffer.bufsize, "*%d\r\n", n+2);
    if(!fmt_param(cmd)) return false;
    if(!fmt_param(val1)) return false;
    for(int i=0; i<n; i++)
    {
        if(!fmt_param(val2[i]))
        {
            return false;
        }
    }
    return send_request();
}

bool myredis::send_cmd(const char* cmd, const char** val1, const char** val2)
{
    REST_ERR;
    if(!cmd || !val1 || !val2)
    {
        sprintf(m_err, "param err");
        return false;
    }
    int n1, n2;
    n1 = get_val_count(val1);
    n2 = get_val_count(val2);
    if(n1 != n2)
    {
        sprintf(m_err, "param err");
        return false;
    }
    m_buffer.datasize = snprintf(m_buffer.data, m_buffer.bufsize, "*%d\r\n", n1*2+1);
    if(!fmt_param(cmd)) return false;
    for(int i=0; i<n1; i++)
    {
        if(!fmt_param(val1[i]))
            return false;
        if(!fmt_param(val2[i]))
            return false;
    }
    return send_request();
}

bool myredis::send_cmd(const char* cmd, const char* val, const char** val1, const char** val2)
{
    if(!cmd || !val || ! val1 || !val2)
    {
        sprintf(m_err, "param err");
        return false;
    }
    int n1, n2;
    n1 = get_val_count(val1);
    n2 = get_val_count(val2);
    if(n1 != n2)
    {
        sprintf(m_err, "param err");
        return false;
    }
    m_buffer.datasize = snprintf(m_buffer.data, m_buffer.bufsize, "*%d\r\n", n1*2+2);
    if(!fmt_param(cmd)) return false;
    if(!fmt_param(val)) return false;
    for(int i=0; i<n1; i++)
    {
        if(!fmt_param(val1[i]))
            return false;
        if(!fmt_param(val2[i]))
            return false;
    }
    return send_request();
}

bool myredis::read_response()
{
    clear_buf();
    do 
    {
        int n = m_tcp.read(&m_buffer.data[m_buffer.datasize], m_buffer.bufsize-m_buffer.datasize);
        if(n<0)
        {
            sprintf(m_err, "socket read err");
            m_tcp.close();
            return false;
        }
        if(n == 0)
        {
            sprintf(m_err, "socket read timeout");
            m_tcp.close();
            return false;
        }
        m_buffer.datasize += n;

        //检查响应缓存是否足够
        if((m_buffer.bufsize - m_buffer.datasize) < 10)//最小阈值10字节
        {
            //缓存超过最大限度
            if(m_buffer.bufsize>DEF_REDIS_BUFFER_SIZE)
            {
                sprintf(m_err, "response buff not enough.");
                return false;
            }
            char* p = (char*)realloc(m_buffer.data, m_buffer.bufsize + DEF_REDIS_BUFFER_SIZE);
            if(!p)
            {
                sprintf(m_err, "response buff not enough.");
                return false;
            }
        }
        m_buffer.data[m_buffer.datasize] = 0;
    } while (!is_response_complish());
    if(m_buffer.data[0] == RDSACK_ERROR)
    {
        strcat_s(m_err, sizeof(m_err), &m_buffer.data[1]);
        m_tcp.close();
        return false;
    }
    return true;
}

bool myredis::is_ok()
{
    return (m_buffer.data[1]=='O' && m_buffer.data[2] == 'K');
}

int myredis::get_integer()
{
    if(m_buffer.data[0] != RDSACK_INTEGER)
        return -1;
    return atoi(&m_buffer.data[1]);
}

int myredis::get_bulk(redis_bulk** out)
{
    *out = m_bulklist.bulk_list;
    return m_bulklist.response_count;
}

int myredis::get_bulk_str(char** out)
{
    if(out)
        *out = m_bulklist.bulk_list[0].data;
    return m_bulklist.bulk_list[0].len;
}

long long myredis::get_int64()
{
    if(m_buffer.data[0] != RDSACK_INTEGER)
        return -1;
#ifdef _WIN32
    return _atoi64(&m_buffer.data[1]);
#else
    return atol(&m_buffer.data[1]);
#endif
}

const char* myredis::get_string()
{
    if(m_buffer.data[0] != RDSACK_SINGLELINE)
        return 0;
    return &m_buffer.data[1];
}


char* myredis::get_line_end(const char* data,int size)
{
    if(size < 1)
        return 0;
    char* p = (char*)data;
    const char* pend = p + size - 1;
    while(p < pend)
    {
        if(*p == '\r' && *(p+1)=='\n')
            return (char*)p;
        p++;
    }
    return 0;
}

int myredis::get_bulk(const char* data,int size,int* len, char**value, int* ressize)
{
    if(data[0] != RDSACK_BULK)
        return -1;
    *value   = 0;
    *ressize = 0;
    const char*  pend  = get_line_end(data,size);

    if(!pend || pend - data + 2 > size)
        return 1; //not enough data
    *len = atoi(data+1);
    if(*len <= 0)
    {
        if(*len == 0)
            *ressize = size - 4;
        else if(*len == -1)
            *ressize = size - 5;
        else
            return -1;
        return 0; //no values;
    }

    *ressize = size - ((pend - data) + 2  + *len + 2);
    if(*ressize < 0)
        return 1; //not enough data
    *value = (char*)pend + 2;
    pend = *value + *len;
    if(pend[0] != '\r' && pend[1]!='\n')
        return -1; //error
    
    return 0;
}

bool myredis::analyse_multbukl()
{
    if(!m_bulklist.response_count)
    {
        const char*  pend  = get_line_end(m_buffer.data, m_buffer.datasize);
        if(!pend)
            return false;
        m_buffer.offset = pend - m_buffer.data + 2;
        m_bulklist.response_count = atoi(&m_buffer.data[1]);
        if(m_bulklist.response_count>m_bulklist.maxcount)
        {
            if(m_bulklist.response_count<DEF_REDIS_BULK_MAXCOUNT)
            {
                int newsize = m_bulklist.response_count;
                redis_bulk* p = (redis_bulk *)calloc(newsize, sizeof(redis_bulk));
                if(!p)
                {
                    sprintf(m_buffer.data, "-ERR bulk calloc fail.");
                    return true;
                }
                free(m_bulklist.bulk_list);
                m_bulklist.bulk_list = p;
                m_bulklist.maxcount = m_bulklist.response_count;
            }
            else
            {
                sprintf(m_buffer.data, "-ERR response bulk size overflow");
                return true;
            }
        }
        m_bulklist.index = -1;
    }
    int ressize = m_buffer.datasize - m_buffer.offset;
    
    while(ressize>0)
    {
        int len = 0;
        char* val = 0;
        int result = get_bulk(&m_buffer.data[m_buffer.offset], m_buffer.datasize-m_buffer.offset, &len, &val, &ressize);
        if(result < 0)
        {
            sprintf(m_buffer.data, "-ERR analize response bulk err");
            return true;
        }
        if(result > 0)
            return false;
        m_bulklist.index ++;
        if(len>0)
        {
            m_bulklist.bulk_list[m_bulklist.index].data = val;
            m_bulklist.bulk_list[m_bulklist.index].len = len;
        }
        else
        {
            m_bulklist.bulk_list[m_bulklist.index].len = 0;
            m_bulklist.bulk_list[m_bulklist.index].data = 0;
        }
        if(val)
        {
            val[len] = 0;
            val[len+1] = 0;
        }
        m_buffer.offset = m_buffer.datasize - ressize;
    }
    if(ressize<0)
    {
        sprintf(m_buffer.data, "-ERR fuck");
        return true;
    }
    if((m_bulklist.index+1) == m_bulklist.response_count)
        return true;
    return false;
}

bool myredis::is_response_complish()
{
    if(m_buffer.datasize <= 2)
        return false;
    switch(m_buffer.data[0])
    {
    case RDSACK_SINGLELINE:
    case RDSACK_ERROR:
    case RDSACK_INTEGER:
        {
            if(m_buffer.data[m_buffer.datasize-2]=='\r' && m_buffer.data[m_buffer.datasize-1]=='\n')
            {
                m_buffer.data[m_buffer.datasize-2] = 0;
                return true;
            }
        }
        return false;
    case RDSACK_BULK:
        {
            int len = 0;
            char* val = 0;
            int rest = 0;
            int result = get_bulk(m_buffer.data, m_buffer.datasize, &len, &val, &rest);
            if(result < 0)
            {
                sprintf(m_buffer.data, "-ERR recv data error");
                return true;
            }
            if(result > 0)
                return false;
            m_bulklist.index = 1;
            m_bulklist.response_count = 1;
            m_bulklist.bulk_list[0].data = val;
            m_bulklist.bulk_list[0].len = len>0?len:0;
            return true;
        }
    case RDSACK_MULTIBULK:
        {
            return analyse_multbukl();
        }
        break;
    default:
        {
            sprintf(m_buffer.data, "-ERR recv data type invalid");
        }
        return true;
    }
    return true;
}

bool myredis::send_request()
{
    REST_ERR;
    if(m_buffer.datasize <= 0)
    {
        sprintf(m_err, "request err");
        return false;
    }
    if(m_tcp.write(m_buffer.data, m_buffer.datasize) != m_buffer.datasize)
    {
        m_tcp.close();
        sprintf(m_err, "socket send err");
        return false;
    }
    return read_response();
}

//Connection----------------------------------------------------------------------------
bool myredis::auth(const char* pwd)
{
    return true;
}

int myredis::ping()
{
    unsigned int t1 = 0;
    unsigned int t2 = 0;
#ifdef _WIN32
    t1 = GetTickCount();
#else
#endif
    fmt_lcmd("PING\r\n");
    if(!send_request())
        return -1;
    char* p = &m_buffer.data[1];
    if(p[0] != 'P' || p[1]!='O' || p[2]!='N' || p[3]!='G')
        return -1;
#ifdef _WIN32
    t2 = GetTickCount();
#else
#endif
    return t2 - t1;
}

bool myredis::select(int ndb)
{
    if(ndb == m_db)
        return true;
    fmt_lcmd("SELECT %d\r\n", ndb);
    if(!send_request())
        return false;
    if(!is_ok())
        return false;
    m_db = ndb;

    return true;
}

bool myredis::echo(const char* str)
{
    return true;
}

//Keys----------------------------------------------------------------------------
int myredis::del(const char** keys)
{
    if(!keys)
        return -1;
    if(!send_cmd("DEL", keys))
        return -1;
    return get_integer();
}

int myredis::keys(char* key, redis_bulk** val)
{
    if(key)
    {
        if(!send_cmd("KEYS", key))
            return -1;
    }
    else
    {
        if(!send_cmd("KEYS", "*"))
            return -1;
    }
    return get_bulk(val);
}

int myredis::pexpire(char* key, int ms)
{
    REST_ERR;
    if(!key)
        return -1;
    fmt_lcmd("PEXPIRE %s %d\r\n", key, ms);
    if(!send_request())
        return -1;
    return get_integer();
}

int myredis::renamenx(char* key1, char* key2)
{
    REST_ERR;
    if(!key1 || key2)
        return -1;
    fmt_lcmd("RENAMENX %s %s\r\n", key1, key2);
    if(!send_request())
        return -1;
    return get_integer();
}

bool myredis::dump(char* key, redis_bulk** out)
{
    REST_ERR;
    if(!key || !out)
        return false;
    fmt_lcmd("DUMP %s\r\n", key);
    if(!send_request())
        return false;
    if(m_bulklist.response_count == 1)
    {
        *out = m_bulklist.bulk_list;
        return true;
    }

    return false;
}

int myredis::pexpireat(char* key, long long mst)
{
    REST_ERR;
    fmt_lcmd("PEXPIREAT %s %I64d\r\n", key, mst);
    if(!send_request())
        return -1;
    return get_integer();
}

bool myredis::exists(char* key)
{
    REST_ERR;
    fmt_lcmd("EXISTS %s\r\n", key);
    if(!send_request())
        return false;
    return (get_integer()==1);
}

bool myredis::move(char* key, int ndb)
{
    REST_ERR;
    if(!key || (ndb<0 || ndb>15))
        return false;
    fmt_lcmd("MOVE %s %d\r\n", key, ndb);
    if(!send_request())
        return false;
    return (get_integer()==1);
}

long long myredis::pttl(char* key)
{
    REST_ERR;
    if(!key)
        return -1;
    fmt_lcmd("PTTL %s\r\n", key);
    if(!send_request())
        return -1;
    return get_int64();
}

int myredis::expire(char* key, int t)
{
    REST_ERR;
    if(!key || t<0)
        return -1;
    fmt_lcmd("EXPIRE %s %d\r\n", key, t);
    if(!send_request())
        return -1;
    return get_integer();
}

bool myredis::randomkey(redis_bulk** val)
{
    REST_ERR;
    if(!val)
        return false;
    fmt_lcmd("RANDOMKEY\r\n");
    if(!send_request())
        return false;
    *val = m_bulklist.bulk_list;
    return true;
}

int myredis::ttl(char* key)
{
    REST_ERR;
    if(!key)
        return -1;
    fmt_lcmd("TTL %s\r\n", key);
    if(!send_request())
        return -1;
    return get_integer();
}

int myredis::expireat(char* key, int t)
{
    REST_ERR;
    if(!key)
        return -1;
    fmt_lcmd("EXPIREAT %s %d\r\n", key, t);
    if(!send_request())
        return -1;
    return get_integer();
}

int myredis::persist(char* key)
{
    REST_ERR;
    if(!key)
        return -1;
    fmt_lcmd("PERSIST %s\r\n", key);
    if(!send_request())
        return -1;
    return get_integer();
}

bool myredis::rename(char* key, char* newkey)
{
    REST_ERR;
    if(!key || !newkey)
        return false;
    fmt_lcmd("RENAME %s %s\r\n", key, newkey);
    if(!send_request())
        return false;
    return is_ok();
}

const char* myredis::type(char* key)
{
    REST_ERR;
    if(!key)
        return 0;
    fmt_lcmd("TYPE %s\r\n", key);
    if(!send_request())
        return 0;
    return get_string();
}

//Strings----------------------------------------------------------------------------
int myredis::append(const char* key, const char* value, int len)
{
    if(len)
    {
        if(!send_cmd("APPEND", key, value, len))
            return -1;
    }
    else
    {
        if(!send_cmd("APPEND", key, value))
            return -1;
    }
    return get_integer();
}

int myredis::getbit(const char* key, int offset)
{
    if(!key)
        return -1;
    fmt_lcmd("GETBIT %s %d\r\n", key, offset);
    if(!send_request())
        return -1;
    return get_integer();
}

int myredis::get(char* key, char** out)
{
    if(!send_cmd("GET", key))
        return -1;
    return get_bulk_str(out);
}

int myredis::mget(const char** keys, redis_bulk** out)
{
    if(!send_cmd("MGET", keys))
        return -1;
    return get_bulk(out);
}

bool myredis::setex(const char* key, int seconds, char* val, int len)
{
    if(!key || !val || seconds<=0)
        return false;
    char buff[32];
    if(!send_cmd("SETEX", key, i2a(buff, seconds), val, len))
        return false;
    return is_ok();
}

int myredis::bitcount(const char* key, int start, int end)
{
    if(!key || start<0 || end<0)
        return -1;
    fmt_lcmd("BITCOUNT %s %d %d\r\n", key, start, end);
    if(!send_request())
        return -1;
    return get_integer();
}

int myredis::getrange(const char* key, int start, int end, char** out)
{
    REST_ERR;
    if(!key)
        return -1;
    fmt_lcmd("GETRANGE %s %d %d\r\n", key, start, end);
    if(!send_request())
        return -1;
    return get_bulk_str(out);
}

bool myredis::mset(char** keys, char** values)
{
    if(!send_cmd("MSET", (const char**)keys, (const char**)values))
        return false;
    return is_ok();
}

bool myredis::setnx(char* key, char* val, int len)
{
    if(len)
    {
        if(!send_cmd("SETNX", key, val, len))
            return false;
    }
    else
    {
        if(!send_cmd("SETNX", key, val))
            return false;
    }
    return (get_integer()==1);
}

int myredis::getset(char** out, char* key, char* val, int len)
{
    if(len)
    {
        if(!send_cmd("GETSET", key, val, len))
            return -1;
    }
    else
    {
        if(!send_cmd("GETSET", key, val))
            return -1;
    }
    return get_bulk_str(out);
}

bool myredis::msetnx(char** keys, char** values)
{
    if(!send_cmd("MSETNX", (const char**)keys, (const char**)values))
        return false;
    return (get_integer() == 1);
}

int myredis::setrange(char* key, int offset, char* value)
{
    REST_ERR;
    if(!key || !value)
        return 0;
    fmt_lcmd("SETRANGE %s %d %s\r\n", key, offset, value);
    if(!send_request())
        return -1;
    return get_integer();
}

bool myredis::decr(char* key, int* val)
{
    if(!key || !val)
        return false;
    fmt_lcmd("DECR %s\r\n", key);
    if(!send_request())
        return false;
    *val = get_integer();
    return true;
}

bool myredis::incr(char* key, int* val)
{
    if(!key || !val)
        return false;
    fmt_lcmd("INCR %s\r\n", key);
    if(!send_request())
        return false;
    *val = get_integer();
    return true;
}

bool myredis::psetex(char* key, long long milliseconds, char* value)
{
    REST_ERR;
    if(!key || !value)
        return false;
    fmt_lcmd("PSETEX %s %I64d %s\r\n", key, milliseconds, value);
    if(!send_request())
        return false;
    return is_ok();
}

int myredis::STRLEN(char* key)
{
    REST_ERR;
    if(!key)
        return -1;
    fmt_lcmd("STRLEN %s\r\n", key);
    if(!send_request())
        return -1;
    return get_integer();
}

bool myredis::decrby(char* key, int decrement, int* result)
{
    REST_ERR;
    if(!key || !result)
        return false;
    fmt_lcmd("DECRBY %s %d\r\n", key, decrement);
    if(!send_request())
        return false;
    *result = get_integer();
    return true;
}

bool myredis::incrby(char* key, int increment, int* result)
{
    REST_ERR;
    if(!key || !result)
        return false;
    fmt_lcmd("INCRBY %s %d\r\n", key, increment);
    if(!send_request())
        return false;
    *result = get_integer();
    return true;
}

bool myredis::set(char* key, char* val, int len)
{
    if(!key || !val)
        return false;
    bool result;
    if(len)
        result = send_cmd("SET", key, val, len);
    else
        result = send_cmd("SET", key, val);
    if(!result)
        return false;
    return is_ok();
}

bool myredis::incrbyfloat(char* key, float increment, double* out)
{
    if(!key || !out)
        return false;
    fmt_lcmd("INCRBYFLOAT %s %f\r\n", key, increment);
    if(!send_request())
        return false;
    if(m_bulklist.response_count == 1)
    {
        *out = atof(m_bulklist.bulk_list[0].data);
        return true;
    }

    return false;
}

int myredis::hdel(char* key, char** fields)
{
    if(!key || !fields)
        return -1;
    if(!send_cmd("HDEL", key, (const char**)fields))
        return -1;
    return get_integer();
}

bool myredis::hincrby(char* key, char* field, int increment, int* out)
{
    if(!key || !field || !out)
        return false;
    fmt_lcmd("HINCRBY %s %s %d\r\n", key, field, increment);
    if(!send_request())
        return false;
    *out = get_integer();
    return true;
}

int myredis::hmget(char* key, char** fields, redis_bulk** out)
{
    if(!key || !fields || !out)
        return -1;
    if(!send_cmd("HMGET", key, (const char**)fields))
        return -1;
    if(!m_bulklist.response_count)
        return 0;
    return get_bulk(out);
}

int myredis::hvals(char* key, redis_bulk** out)
{
    if(!send_cmd("HVALS", key))
        return -1;
    return get_bulk(out);
}

int myredis::hexists(char* key, char* field)
{
    if(!send_cmd("HEXISTS", key, field))
        return -1;
    return get_integer();
}

bool myredis::hincrbyfloat(char* key, char* field, float increment, double* out)
{
    if(!key || !field || !out)
        return false;
    fmt_lcmd("HINCRBYFLOAT %s %s %f\r\n", key, field, increment);
    if(!send_request())
        return false;
    if(!m_bulklist.response_count)
        return false;
    *out = atof(m_bulklist.bulk_list[0].data);
    return true;
}

int myredis::hmset(char* key, char** fields, char** values)
{
    if(!send_cmd("HMSET", key, (const char**)fields, (const char**)values))
        return -1;
    return is_ok()?1:0;
}

int myredis::hget(char* key, char* field, char** out)
{
    REST_ERR;
    if(!key || !field)
        return -1;
    fmt_lcmd("HGET %s %s\r\n", key, field);
    if(!send_request())
        return -1;
    if(m_bulklist.response_count == 1 && m_bulklist.bulk_list[0].len>0)
    {
        return get_bulk_str(out);
    }
    return 0;
}

int myredis::hkeys(char* key, redis_bulk** out)
{
    REST_ERR;
    if(!key || !out)
        return -1;
    fmt_lcmd("HKEYS %s\r\n", key);
    if(!send_request())
        return -1;
    return get_bulk(out);
}

int myredis::hset(char* key, char* field, char* val, int len)
{
    if(len)
    {
        fmt_lcmd("HSET %s %s %s\r\n", key, field, val);
        if(!send_request())
            return -1;
    }
    else
    {
        if(!send_cmd("HSET", key, field, val, len))
            return -1;
    }
    return get_integer();
}

int myredis::hgetall(char* key, redis_bulk** out)
{
    if(!send_cmd("HGETALL", key))
        return -1;
    return get_bulk(out);
}

int myredis::hlen(char* key)
{
    if(!send_cmd("HLEN", key))
        return -1;
    return get_integer();
}

int myredis::hsetnx(char* key, char* field, char* value, int len)
{
    if(len)
    {
        fmt_lcmd("HSETNX %s %s %s\r\n", key, field, value);
        if(!send_request())
            return -1;
    }
    else
    {
        if(!send_cmd("HSETNX", key, field, value, len))
            return -1;
    }
    return get_integer();
}
//////////////////////////////////////////////////////////////////////////
//List
int myredis::blpop(char** keys, int timeout, redis_bulk** out)
{
    if(!send_cmd("BLPOP", (const char**)keys))
        return -1;
    return get_bulk(out);
}

int myredis::llen(char* key)
{
    if(!send_cmd("LLEN", key))
        return -1;
    return get_integer();
}

int myredis::lrem(char* key, int count, char* value)
{
    if(!key||!value)
        return -1;
    fmt_lcmd("LREM %s %d %s\r\n", key, count, value);
    if(!send_request())
        return -1;
    return get_integer();
}

int myredis::rpush(char* key, char** values)
{
    if(!send_cmd("RPUSH", key, (const char**)values))
        return -1;
    return get_integer();
}

int myredis::brpop(char** keys, int timeout, redis_bulk** out)
{
    if(!send_cmd("BRPOP", (const char**)keys))
        return -1;
    return get_bulk(out);
}

int myredis::lpop(char* key, char** out)
{
    REST_ERR;
    if(!key || out)
        return -1;
    if(!send_cmd("LPOP", key))
        return -1;
    return get_bulk_str(out);
}

bool myredis::lset(char* key, int index, char* value, int len)
{
    if(!len)
    {
        if(!key || !value) return false;
        fmt_lcmd("LSET %s %d %s\r\n");
        if(!send_request())
            return false;
    }
    else
    {
        char ibuf[32];
        if(!send_cmd("LSET", key, i2a(ibuf, index), value, len))
            return false;
    }
    return is_ok();
}

int myredis::rpushx(char* key, char* value, int len)
{
    if(len)
    {
        if(!send_cmd("RPUSHX", key, value, len))
            return false;
    }
    else
    {
        if(!send_cmd("RPUSHX", key, value))
            return false;
    }
    return get_integer();
}

int myredis::lpush(char* key, char** value)
{
    if(!send_cmd("LPUSH", key, (const char*)value))
        return -1;
    return get_integer();
}

bool myredis::ltrim(char* key, int start, int stop)
{
    REST_ERR;
    if(!key) return false;
    fmt_lcmd("LTRIM %s %d %d\r\n", key, start, stop);
    if(!send_request())
        return false;
    return is_ok();
}

int myredis::lindex(char* key, int index, char** out)
{
    REST_ERR;
    if(!key) return -1;
    fmt_lcmd("LINDEX %s %d\r\n", key, index);
    if(!send_request())
        return false;
    return get_bulk_str(out);
}

int myredis::lpushx(char* key, char* value, int len)
{
    if(len)
    {
        if(!send_cmd("LPUSHX", key, value, len))
            return -1;
    }
    else
    {
        if(!send_cmd("LPUSHX", key, value))
            return -1;
    }
    return get_integer();
}

int myredis::rpop(char* key, char** out)
{
    if(!send_cmd("RPOP", key))
        return -1;
    return get_bulk_str(out);
}

int myredis::lrange(char* key, int start, int stop, redis_bulk** out)
{
    REST_ERR;
    if(!key) return -1;
    fmt_lcmd("LRANGE %s %d %d\r\n", key, start, stop);
    if(!send_request())
        return -1;
    return get_bulk(out);
}

int myredis::rpoplpush(char* source, char* destination, char** out)
{
    if(!send_cmd("RPOPLPUSH", source, destination))
        return -1;
    return get_bulk_str(out);
}
//////////////////////////////////////////////////////////////////////////
//Set
int myredis::sadd(char* key, char** members)
{
    if(!send_cmd("SADD", key, (const char*)members))
        return -1;
    return get_integer();
}

int myredis::scard(char* key)
{
    if(!send_cmd("SCARD", key))
        return -1;
    return get_integer();
}