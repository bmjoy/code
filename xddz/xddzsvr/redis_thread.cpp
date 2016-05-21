#include "redis_thread.h"
#include "config_manager.h"
#include "pptr_buffer.h"
#include "game_main_thread.h"
#include "cflog.h"
#include "redis/myredis_pool.h"
#include "card_mission.h"

#pragma warning(disable:4996)

redis_thread::redis_thread(void)
{
}

redis_thread::~redis_thread(void)
{
}

bool redis_thread::init()
{
    if(!ring_buffer2_thread::init(1, 10240, 10240, 1024000, 1000))
        return false;

    do 
    {
        myredis_helper redis(myredis_pool::instance(), 
            config_manager::instance()->redis_win_ip(),
            config_manager::instance()->redis_win_port(),
            config_manager::instance()->redis_win_db());
        if(redis.is_null())
        {
            LOG_ERROR("connect reids fail %s:%d:%d",
                config_manager::instance()->redis_win_ip(),
                config_manager::instance()->redis_win_port(),
                config_manager::instance()->redis_win_db());
            return false;
        }
    } while (false);

    do 
    {
        myredis_helper redis(myredis_pool::instance(), config_manager::instance()->redis_count_box_ip(),
            config_manager::instance()->redis_count_box_port(),
            config_manager::instance()->redis_count_box_db());
        if(redis.is_null())
        {
            LOG_ERROR("connect redis fail %s:%d:%d",
                config_manager::instance()->redis_count_box_ip(),
                config_manager::instance()->redis_count_box_port(),
                config_manager::instance()->redis_count_box_db());
            return false;
        }
    } while (false);

    return true;
}

enum REDIS_TYPE
{
    WIN_COUNT_MISSION,
    LOAD_COUNT_BOX,
    UPDATE_COUNT_BOX,
    GET_CARD_MISSION_INDEX,
    SET_CARD_MISSION_INDEX
};

struct load_count_box_item
{
    socket_id sid;
    unsigned int uid;
};

struct update_count_box_item
{
    unsigned int uid;
    int level;
    int seconds;
};

struct set_card_mission_index_item
{
    unsigned int uid;
    int index;
};

void redis_thread::on_data(void* phead, int headlen, void* pdata, int datalen)
{
    int& type = *(int*)phead;
    switch(type)
    {
    case WIN_COUNT_MISSION:
        {
            unsigned int uid = *(unsigned int*)pdata;
            do_win_mission(uid);
        }
        break;
    case LOAD_COUNT_BOX:
        {
            load_count_box_item item = *(load_count_box_item*)pdata;
            do_load_count_box(item.sid, item.uid);
        }
        break;
    case UPDATE_COUNT_BOX:
        {
            update_count_box_item item = *(update_count_box_item*)pdata;
            do_update_count_box(item.uid, item.level, item.seconds);
        }
        break;
    case GET_CARD_MISSION_INDEX:
        {
            unsigned int uid = *(unsigned int*)pdata;
            do_get_card_mission_index(uid);
        }
        break;
    case SET_CARD_MISSION_INDEX:
        {
            set_card_mission_index_item* item = (set_card_mission_index_item*)pdata;
            do_set_card_mission_index(item->uid, item->index);
        }
        break;
    default:
        break;
    }
}

void redis_thread::on_win_mission(unsigned int uid)
{
    int type = WIN_COUNT_MISSION;
    put_data(&type, sizeof(type), &uid, sizeof(uid));
}

int get_wincount_expire_time(int h=4)
{
    time_t ct = time(0);
    tm t = *localtime(&ct);

    int ds = 0;

    if(t.tm_hour < h)
    {
        ds = h * 3600;
    }
    else
    {
        ds = (h + 24) * 3600;
    }

    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_hour = 0;

    time_t tt = mktime(&t) + ds;

    int ret = (int)(tt - ct);
    if(ret < 0) ret = 0;

    return ret;
}

void redis_thread::do_win_mission(unsigned int uid)
{
    myredis_helper redis(myredis_pool::instance(), 
        config_manager::instance()->redis_win_ip(),
        config_manager::instance()->redis_win_port(),
        config_manager::instance()->redis_win_db());
    if(redis.is_null())
    {
        LOG_ERROR("%s %d", __FILE__, __LINE__);
        return;
    }
    char key[32];
    sprintf(key, "win%d_%d", config_manager::instance()->server_type(), uid);
    if(redis->exists(key))
    {
        int out = 0;
        redis->hincrby(key, "wincount", 1, &out);
    }
    else
    {
        pptr_buffer<128, 4> field;
        pptr_buffer<128, 4> val;
        field.add("wincount");
        field.add("giftcount");
        val.add("1");
        val.add("0");
        redis->hmset(key, field.pptr, val.pptr);
    }
    redis->expire(key, get_wincount_expire_time());
}

void redis_thread::on_load_count_box(socket_id sid, unsigned int uid)
{
    int type = LOAD_COUNT_BOX;
    load_count_box_item item;
    item.uid = uid;
    item.sid = sid;
    put_data(&type, sizeof(type), &item, sizeof(item));
}

void redis_thread::do_load_count_box(socket_id sid, unsigned int uid)
{
    myredis_helper redis(myredis_pool::instance(), config_manager::instance()->redis_count_box_ip(),
        config_manager::instance()->redis_count_box_port(),
        config_manager::instance()->redis_count_box_db());
    if(redis.is_null())
    {
        LOG_ERROR("redis connect error");
        return;
    }

    char key[64];
    sprintf(key, "cb_%d_%d", config_manager::instance()->server_type(), uid);
    pptr_buffer<128, 4> field;
    field.add("second");
    field.add("level");
    int seconds = 0;
    int level = 0;
    if(!redis->exists(key))
    {
        pptr_buffer<128, 4> value;
        char buf[64];
        sprintf(buf, "0");
        value.add(buf);
        value.add(buf);
        redis->hmset(key, field.pptr, value.pptr);
        redis->expire(key, get_wincount_expire_time());
    }
    else
    {
        redis_bulk* out;
        int n = redis->hmget(key, field.pptr, &out);
        if(n == 2)
        {
            seconds = atoi(out[0].data);
            level = atoi(out[1].data);
        }
    }
    game_main_thread::instance()->on_count_box(sid, uid, seconds, level);
}

void redis_thread::on_update_count_box(unsigned int uid, int level, int seconds)
{
    update_count_box_item item;
    item.uid = uid;
    item.level = level;
    item.seconds = seconds;
    int type = UPDATE_COUNT_BOX;
    put_data(&type, sizeof(type), &item, sizeof(item));
}

void redis_thread::do_update_count_box(unsigned int uid, int level, int seconds)
{
    myredis_helper redis(myredis_pool::instance(), config_manager::instance()->redis_count_box_ip(),
        config_manager::instance()->redis_count_box_port(),
        config_manager::instance()->redis_count_box_db());
    if(redis.is_null())
        return;

    char key[64];
    sprintf(key, "cb_%d_%d", config_manager::instance()->server_type(), uid);
    pptr_buffer<128, 4> field;
    field.add("second");
    field.add("level");
    if(redis->exists(key))
    {
        pptr_buffer<128, 4> value;
        char sz_seconds[64];
        char sz_level[64];
        sprintf(sz_seconds, "%d", seconds);
        value.add(sz_seconds);
        sprintf(sz_level, "%d", level);
        value.add(sz_level);
        redis->hmset(key, field.pptr, value.pptr);
    }
}

void redis_thread::on_get_card_mission_index(unsigned int uid)
{
    int type = GET_CARD_MISSION_INDEX;
    put_data(&type, sizeof(type), &uid, sizeof(uid));
}

void redis_thread::on_set_card_mission_index(unsigned int uid, int index)
{
    int type = SET_CARD_MISSION_INDEX;
    set_card_mission_index_item item;
    item.uid = uid;
    item.index = index;
    put_data(&type, sizeof(type), &item, sizeof(item));
}

void redis_thread::do_get_card_mission_index(unsigned int uid)
{
    myredis_helper redis(myredis_pool::instance(), config_manager::instance()->redis_count_box_ip(),
        config_manager::instance()->redis_count_box_port(),
        config_manager::instance()->redis_count_box_db());
    int index = 0;
    do 
    {
        if(redis.is_null())
            break;
        char key[64];
        sprintf(key, "cm_%d_%d", config_manager::instance()->server_type(), uid);
        pptr_buffer<128, 4> field;
        redis_bulk* out;
        field.add("id");
        field.add("lt");
        if(!redis->exists(key))
            break;
        int n = redis->hmget(key, field.pptr, &out);
        if(n != 2 || !out || out[0].len == 0 || out[1].len == 0)
            break;
        time_t t;
        index = atoi(out[0].data);
        t = atol(out[1].data);
        int mode = cm->mode();
        if(mode == CARD_MISSION_SINGLE)
        {
            time_t ct = time(0);
            tm t1 = *localtime(&ct);
            tm t2 = *localtime(&t);
            if(t1.tm_yday != t2.tm_yday)
            {
                index = 0;
                do_set_card_mission_index(uid, index);
            }
        }
    } while (false);
    game_main_thread::instance()->on_card_mission_index(uid, index);
}

void redis_thread::do_set_card_mission_index(unsigned int uid, int index)
{
    myredis_helper redis(myredis_pool::instance(), config_manager::instance()->redis_count_box_ip(),
        config_manager::instance()->redis_count_box_port(),
        config_manager::instance()->redis_count_box_db());
    if(redis.is_null())
        return;
    char key[64];
    sprintf(key, "cm_%d_%d", config_manager::instance()->server_type(), uid);
    pptr_buffer<128, 4> field;
    pptr_buffer<128, 4> value;
    field.add("id");
    field.add("lt");

    char tmp[32];
    sprintf(tmp, "%d", index);
    value.add(tmp);
    time_t ct = time(0);
    sprintf(tmp, "%lld", ct);
    value.add(tmp);
    redis->hmset(key, field.pptr, value.pptr);
}
