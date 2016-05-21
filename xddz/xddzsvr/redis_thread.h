#ifndef __DDZ_REDIS_THREAD_H__
#define __DDZ_REDIS_THREAD_H__
#include "singleton.h"
#include "ring_buffer2_thread.h"
#include "redis/myredis.h"
#include "socket_service.h"
#include "i_tcp_socket_engine.h"

struct bankrupt
{
	socket_id sid;
	unsigned int uid;
	__int64 coin;
};

class redis_thread : public singleton<redis_thread>, public ring_buffer2_thread<1>
{
public:
    redis_thread(void);
    ~redis_thread(void);

    bool init();

    virtual void on_data(void* phead, int headlen, void* pdata, int datalen);

    void on_win_mission(unsigned int uid);
    void on_load_count_box(socket_id sid, unsigned int uid);
    void on_update_count_box(unsigned int uid, int level, int seconds);
    void on_set_card_mission_index(unsigned int uid, int index);
    void on_get_card_mission_index(unsigned int uid);

private:
    void do_win_mission(unsigned int uid);
    void do_load_count_box(socket_id sid, unsigned int uid);
    void do_update_count_box(unsigned int uid, int level, int seconds);
    void do_get_card_mission_index(unsigned int uid);
    void do_set_card_mission_index(unsigned int uid, int index);
};


#endif//__DDZ_REDIS_THREAD_H__