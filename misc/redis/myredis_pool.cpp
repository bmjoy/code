#include "myredis_pool.h"
#include <WinSock2.h>

myredis_pool::myredis_pool()
{
    m_init = false;
}

struct pool_key
{
    union{
        struct {
            unsigned int ip;
            int port;
        };
        unsigned __int64 key;
    };
};

bool myredis_pool::init(int maxcon, char* pwd)
{
    if(pwd)
        strcpy(m_pwd, pwd);
    else
        m_pwd[0] = 0;
    m_maxcon = maxcon;
    m_init = true;
    return true;
}

myredis* myredis_pool::get(char* ip, int port, int ndb)
{
    if(!m_init)
        return 0;
    myredis* p = 0;
    pool_key key;
    key.port = port;
    key.ip = inet_addr(ip);
    safe_queue<myredis*>* q=0;
    redis_iterator i;
    i = m_pool.find(key.key);
    if(i == m_pool.end())
    {
        q = new safe_queue<myredis*>;
        q->init(m_maxcon);
        guard g(m_mutex);
        m_pool[key.key] = q;
        i = m_pool.find(key.key);
    }
    q = i->second;
    
    if(!q->pop(p) || p==0)
    {
        p = new myredis();
        if(!p->init())
        {
            delete p;
            return 0;
        }
        if(!p->connect(ip, port, m_pwd, ndb))
        {
            delete p;
            p = 0;
        }
    }
    else
    {
        if(!p->is_connected())
        {
            if(!p->connect(ip, port, m_pwd, ndb))
            {
                delete p;
                p = 0;
            }
        }
        else
        {
            p->select(ndb);
        }
    }

    return p;
}

void myredis_pool::put(char* ip, int port, myredis* p)
{
    pool_key key;
    key.port = port;
    key.ip = inet_addr(ip);
    safe_queue<myredis*>* q=0;
    redis_iterator i;
    i = m_pool.find(key.key);
    if(i == m_pool.end())
    {
        p->close();
        delete p;
        return;
    }
    q = i->second;
    if(!q->put(p))
    {
        p->close();
        delete p;
    }
}

void myredis_pool::dump()
{
    redis_iterator i;
    int size = 0;
    for(i=m_pool.begin(); i!=m_pool.end(); i++)
    {
        size += i->second->get_size();
    }
    printf("pool size=%d\n", size);
}