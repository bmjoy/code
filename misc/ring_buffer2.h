/*
auth:  Ishaq
email: msl_cn@126.com
*/
#ifndef __RING_BUFFER2_H__
#define __RING_BUFFER2_H__
#include "mutex.h"

class ring_buffer2
{
public:
    ring_buffer2();
    ~ring_buffer2();
    bool init(int _init_size, int _step_size,
        int _max_size, int _max_block_size);
    bool fini();
    bool reset();
    bool putd(void* phead, int headlen, void* pdata, int datalen);
    bool getd(void* phead, int& headlen, void* pdata, int& datalen);
    int  get_data_count();
    int  get_data_size();

private:
    thread_mutex data_mutex;

    char* m_data_ptr;
    int m_init_size;
    int m_step_size;
    int m_max_size;
    int m_max_buffer_size;
    int m_max_block_size;

    int m_write_tail;
    int m_write_head;
    int m_read_head;

    int m_data_count;
    int m_data_size;

};
#endif //__RING_BUFFER2_H__