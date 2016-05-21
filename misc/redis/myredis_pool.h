#ifndef __MYREDIS_POOL_H__
#define __MYREDIS_POOL_H__
#include "safe_queue.h"
#include "myredis.h"
#include "singleton.h"
#include <map>
#include "mutex.h"
using namespace std;
#pragma warning(disable:4996)

typedef map<unsigned __int64,safe_queue<myredis*>* > redis_map;
typedef redis_map::iterator redis_iterator;

class myredis_pool : public singleton<myredis_pool>
{
public:
    myredis_pool();
    bool init(int maxcon, char* pwd);
    myredis* get(char* ip, int port, int ndb);
    void put(char* ip, int port, myredis* p);
    void dump();
private:

    redis_map m_pool;
    thread_mutex m_mutex;

    char m_pwd[64];
    bool m_init;
    int  m_maxcon;
};

class myredis_helper
{
public:
    myredis_helper(myredis_pool* pool, char* ip, int port, int ndb)
    {
        m_port = 0;
        m_ip[0] = 0;
        m_p = 0;
        m_pool = pool;
        if(!m_pool || !ip)
            return;
        strcpy(m_ip, ip);
        m_port = port;
        m_p = m_pool->get(ip, port, ndb);
    }
    ~myredis_helper()
    {
        if(!m_pool || !m_p || strlen(m_ip)==0)
            return;
        m_pool->put(m_ip, m_port, m_p);
    }
    bool is_null()
    {
        return (m_p == 0);
    }
    myredis* operator -> ()
    {
        return m_p;
    }
    myredis* handle(){return m_p;}
private:
    myredis* m_p;
    char     m_ip[32];
    int      m_port;
    myredis_pool* m_pool;
};

#endif//__MYREDIS_POOL_H__