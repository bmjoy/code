#ifndef __MY_REDIS_CLIENT_H__
#define __MY_REDIS_CLIENT_H__
#include "ctcp.h"
#include "myredis_def.h"

#define MYREDIS_ERR -1
#define MYREDIS_OK  0

#define DEF_REDIS_BUFFER_SIZE     16384             //默认内存大小
#define DEF_REDIS_BUFFER_MAXSIZE  1024*1024*10      //最大内存大小
#define DEF_REDIS_BULK_COUNT      1000              //默认信息块个数
#define DEF_REDIS_BULK_MAXCOUNT   100000            //信息块最大个数


class myredis
{
public:
    myredis(void);
    ~myredis(void);
    bool init();
    void fini();
    bool connect(const char* ip, int port, const char* pwd=0, int ndb=0);
    bool close();
    bool is_connected();
    const char* get_err();
    bool is_error();
    int  get_db();
    //Connection
    bool auth(const char* pwd);
    int  ping();
    bool select(int ndb);
    bool echo(const char* str);
    //Keys
    int del(const char** keys);
    int keys(char* key, redis_bulk** val);
    int pexpire(char* key, int ms);
    int renamenx(char* key1, char* key2);
    bool dump(char* key, redis_bulk** out);
    int pexpireat(char* key, long long mst);
    bool exists(char* key);
    bool move(char* key, int ndb);
    long long pttl(char* key);
    void sort();
    int  expire(char* key, int t);
    void object();
    bool randomkey(redis_bulk** val);
    int  ttl(char* key);
    int  expireat(char* key, int t);
    int  persist(char* key);
    bool rename(char* key, char* newkey);
    const char* type(char* key);
    //Strings
    int  append(const char* key, const char* value, int len=0);
    int  getbit(const char* key, int offset);
    int  get(char* key, char** out);
    int  mget(const char** keys, redis_bulk** out);
    bool setex(const char* key, int seconds, char* val, int len=0);
    int  bitcount(const char* key, int start, int end);
    int  getrange(const char* key, int start, int end, char** out);
    bool mset(char** keys, char** values);
    bool setnx(char* key, char* val, int len=0);
    int  getset(char** out, char* key, char* val, int len=0);
    bool msetnx(char** keys, char** values);
    int  setrange(char* key, int offset, char* value);
    bool decr(char* key, int* val);
    bool incr(char* key, int* val);
    bool psetex(char* key, long long milliseconds, char* value);
    int  STRLEN(char* key);
    bool decrby(char* key, int decrement, int* result);
    bool incrby(char* key, int increment, int* result);
    bool set(char* key, char* val, int len=0);
    bool incrbyfloat(char* key, float increment, double* out);
    //Hashes
    int  hdel(char* key, char** fields);
    bool hincrby(char* key, char* field, int increment, int* out);
    int  hmget(char* key, char** fields, redis_bulk** out);
    int  hvals(char* key, redis_bulk** out);
    int  hexists(char* key, char* field);
    bool hincrbyfloat(char* key, char* field, float increment, double* out);
    int  hmset(char* key, char** field, char** value);
    int  hget(char* key, char* field, char** out);
    int  hkeys(char* key, redis_bulk** out);
    int  hset(char* key, char* field, char* val, int len=0);
    int  hgetall(char* key, redis_bulk** out);
    int  hlen(char* key);
    int  hsetnx(char* key, char* field, char* value, int len=0);
    //List
    int  blpop(char** keys, int timeout, redis_bulk** out);
    int  llen(char* key);
    int  lrem(char* key, int count, char* value);
    int  rpush(char* key, char** values);
    int  brpop(char** keys, int timeout, redis_bulk** out);
    int  lpop(char* key, char** out);
    bool lset(char* key, int index, char* value, int len=0);
    int  rpushx(char* key, char* value, int len=0);
    int  lpush(char* key, char** value);
    bool ltrim(char* key, int start, int stop);
    int  lindex(char* key, int index, char** out);
    int  lpushx(char* key, char* value, int len=0);
    int  rpop(char* key, char** out);
    //int linsert key BEFORE|AFTER pivot value
    int  lrange(char* key, int start, int stop, redis_bulk** out);
    int  rpoplpush(char* source, char* destination, char** out);
    //Set
    int  sadd(char* key, char** members);
    int  scard(char* key);
    void clear_buf();
private:
    int  get_val_count(const char** val);
    bool fmt_param(const char* val);
    bool fmt_param(const char* val, int len);
    bool send_cmd(const char* cmd, const char* val);
    bool send_cmd(const char* cmd, const char** val);
    bool send_cmd(const char* cmd, const char* val1, const char* val2);
    bool send_cmd(const char* cmd, const char* val1, const char* val2, const char* val3, int len3);
    bool send_cmd(const char* cmd, const char* val1, const char* val2, int l2);
    bool send_cmd(const char* cmd, const char* val1, const char** val2);
    bool send_cmd(const char* cmd, const char** val1, const char** val2);
    bool send_cmd(const char* cmd, const char* val, const char** val1, const char** val2);
    
    //bool fmt_cmd(const char* cmd, var_list* vals);
    bool read_response();
    bool send_request();
    bool is_response_complish();
    bool is_ok();
    int  get_integer();
    int  get_bulk(redis_bulk** out);
    int  get_bulk_str(char** out);
    long long get_int64();
    const char* get_string();
    char* get_line_end(const char* data,int size);
    int  get_bulk(const char* data,int size,int* len, char**value, int* ressize);
    bool analyse_multbukl();
private:
    ctcp      m_tcp;
    redis_buf m_buffer;
    redis_bulk_list m_bulklist;

    int       m_db;
    char      m_err[4096];
};

#endif//__MY_REDIS_CLIENT_H__