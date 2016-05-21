#ifndef __RING_BUFFER2_THREAD_H__
#define __RING_BUFFER2_THREAD_H__

#include "ring_buffer2.h"
#ifdef _WIN32
#include <process.h>
#endif


template<int MAX_PRIORITY_SIZE=1>
class ring_buffer2_thread
{
public:
    ring_buffer2_thread()
    {
#ifdef _WIN32
        m_hDataEvent_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
#else
        pthread_attr_init(&m_attr);	
        pthread_cond_init(&m_data_cond, 0);
        pthread_mutex_init(&m_data_mutex, 0);
#endif
        m_is_init = false;
        m_threads = 0;
        m_max_block_size = 0;
        m_is_run = false;
    }
    ~ring_buffer2_thread()
    {
#ifdef _WIN32
        if(m_hDataEvent_ != INVALID_HANDLE_VALUE)
        {
            ::CloseHandle(m_hDataEvent_);
            m_hDataEvent_ = INVALID_HANDLE_VALUE;
        }
#else
        pthread_attr_destroy(&m_attr);
        pthread_cond_destroy(&m_data_cond);
        pthread_mutex_destroy(&m_data_mutex);
#endif
    }

    bool init(int _threads, int _init_size, int _step_size,
        int _max_size, int _max_block_size)
    {
        if (m_is_init)
        {
            return false;
        }
        if (_threads <= 0)
        {
            return false;
        }
        m_threads = _threads;
        m_max_block_size = _max_block_size;
        for(int i=0; i<MAX_PRIORITY_SIZE; i++)
        {
            if(m_data_buff[i].init(_init_size, _step_size, _max_size, _max_block_size))
            {
                m_is_init = true;
            }
        }
        return m_is_init;
    }

    bool fini()
    {
        if(!m_is_init || m_is_run)
        {
            return false;
        }
        for(int i=0; i<MAX_PRIORITY_SIZE; i++)
        {
            m_data_buff[i].fini();
        }
        m_is_init = false;
        return true;
    }

    bool start()
    {
        if(!m_is_init || m_is_run)
        {
            return false;
        }
        m_is_run = true;
        for(int i=0; i<m_threads; i++)
        {
#ifdef _WIN32
            unsigned int dwThreadID = 0;
            HANDLE hThread;
            hThread = (HANDLE)_beginthreadex(NULL, 0, run0, this, 0, &dwThreadID);
            Sleep(100);
#else
            pthread_t tid;
            if(pthread_create(&tid, &m_attr, run0, this) != 0)
                return false;
#endif
        }
        return true;
    }

    bool stop()
    {
        if(!m_is_run)
        {
            return false;
        }
        m_is_run = false;
        for(int i=0; i<m_threads; i++)
        {
#ifdef _WIN32
            ::PostQueuedCompletionStatus(m_hDataEvent_, 0, 0, 0);
#else
            pthread_cond_signal(&m_data_cond);
#endif
        }
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
        return true;
    }

    bool put_data(void* phead, int headlen, void* pdata, int datalen, int pri=0)
    {
        if(!m_is_run)
        {
            return false;
        }
        if(pri<0 || pri>=MAX_PRIORITY_SIZE)
            pri = MAX_PRIORITY_SIZE - 1;
        bool ret = m_data_buff[pri].putd(phead, headlen, pdata, datalen);
        if(ret)
        {
#ifdef _WIN32
            //		::SetEvent(hDataEvent_);
            ::PostQueuedCompletionStatus(m_hDataEvent_, 0, 0, 0);
#else
            pthread_cond_signal(&m_data_cond);
#endif
        }
        return ret;
    }

    int get_data_count()
    {
        int ret = 0;
        for(int i=0; i<MAX_PRIORITY_SIZE; i++)
            ret += m_data_buff[i].get_data_count();
        return ret;
    }

    virtual void on_data(void* phead, int headlen, void* pdata, int datalen) = 0;

private:
#ifdef _WIN32
    static unsigned int __stdcall run0(void * pVoid)
#else
    static void* run0(void* pVoid)
#endif
    {
        ring_buffer2_thread* pthis = (ring_buffer2_thread*)pVoid;
        pthis->run();
        return 0;
    }

    int run()
    {
        char* phead = new char[m_max_block_size];
        char* pdata = new char[m_max_block_size];
        while(m_is_run)
        {
#ifdef _WIN32
            //		::WaitForSingleObject(hDataEvent_, INFINITE);
            DWORD dw_size = 0;
            long p1 = 0;
            LPOVERLAPPED p2;
            BOOL QB = ::GetQueuedCompletionStatus(m_hDataEvent_, &dw_size, (PULONG_PTR)&p1, &p2, INFINITE);

            if(!QB)
                break;
#else
            pthread_mutex_lock(&m_data_mutex);
            pthread_cond_wait(&m_data_cond, &m_data_mutex);
            pthread_mutex_unlock(&m_data_mutex);
#endif
            if(!m_is_run)
                break;
            bool b_hasdata = true;
            while(b_hasdata)
            {
                int headlen = m_max_block_size;
                int datalen = m_max_block_size;
                b_hasdata = false;
                for(int i=0; i<MAX_PRIORITY_SIZE; i++)
                {
                    bool b = true;

                    b = m_data_buff[i].getd(phead, headlen, pdata, datalen);
                    if(b)
                    {
                        b_hasdata = true;
                        try
                        {
                            on_data(phead, headlen, pdata, datalen);
                        }
                        catch (...)
                        {

                        }
                        break;
                    }
                }
            }
        }
        delete phead;
        delete pdata;
        return 0;
    }


private:
    bool            m_is_init;
    bool            m_is_run;
    int             m_threads;
    int             m_max_block_size;
#ifdef _WIN32
    HANDLE			m_hDataEvent_;
#else
    pthread_attr_t  m_attr;
    pthread_cond_t  m_data_cond;
    pthread_mutex_t m_data_mutex;
#endif
    ring_buffer2    m_data_buff[MAX_PRIORITY_SIZE];
};


#endif//__RING_BUFFER2_THREAD_H__
