#ifndef __DDZ_DB_THREAD_H__
#define __DDZ_DB_THREAD_H__
#include "ring_buffer2_thread.h"
#include "singleton.h"
#include "i_tcp_socket_engine.h"
#include "redis/myredis.h"

#include "db_con_manager.h"
enum DB_TYPE
{
    USERINFO_DB,
    PROP_DB,
    GAME_DB,
    TASK_DB,
    MONEY_DB,
	SERVER_DB,
	LOG_DB,
	MESSAGE_DB,
    MATCH_DB,
    DB_TYPE_COUNTER
};

extern db_type_item g_db_type[DB_TYPE_COUNTER];

/*
数据库线程为单线程，3个优先级分别针对如下三类优先级
1. 登录
2. 回写数据
3. 日志
*/
struct game_log
{
    unsigned int uid;
    __int64 score1;
    __int64 score2;
};

struct game_log_record
{
    int tax;
    game_log user[3];
};

struct award_record
{
	unsigned int uid;
	char text[256];
};

struct db_login_fail_result
{
    int result;
    int serverid;
};

class db_thread :
    public ring_buffer2_thread<3> ,
    public singleton<db_thread>
{
public:
    db_thread(void);
    ~db_thread(void);
    void on_login(socket_id sid, unsigned int uid);
    void on_write_score(unsigned int uid, __int64 coin, __int64 glory, int win, int lost, int flee);
    void on_update_online(int serverid, int n);
    void on_logout(unsigned int uid);
	//void on_get_online_mission(socket_id sid, unsigned int uid);
	void on_add_prop(int addtype, unsigned int uid, int pid, int pn, int term=0);
	void on_add_coin(int addtype, unsigned int uid, __int64 coin);
	void on_write_game_log(game_log_record glr);
    //void on_check_bankrupt(socket_id sid, unsigned int uid);
	void on_award_msg_log(award_record item);
    void match_log(unsigned int uid, int matchid, int matchtype, int cointype, __int64 coin);

    virtual void on_data(void* phead, int headlen, void* pdata, int datalen);

    void on_update_robot();
    void load_match_user(int sn);
    void card_mission_log(unsigned int uid, int mid);
private:
    void do_login(socket_id sid, unsigned int uid);
    void do_logout(unsigned int uid);
    void do_write_score(unsigned int uid, __int64 coin, __int64 glory, int win, int lost, int flee);
    void do_update_online(int serverid, int n);
	//void do_get_online_mission(socket_id sid, int uid);
	void do_add_prop(int type, unsigned int uid, int pid, int pn, int term);
	void do_add_coin(int type, unsigned int uid, __int64 coin);
	void do_write_game_log(game_log_record& glr);
    //void do_check_bankrupt(socket_id sid, unsigned int uid);
	void do_award_msg_log(award_record& item);
    void do_update_robot();
    void load_robot(int uid);
    void load_robot_coin(int uid);
    void on_load_match_user(int sn);
    void on_match_log(unsigned int uid, int matchid, int matchtype, int cointype, __int64 coin);
    void on_card_mission_log(unsigned int uid, int mid);
};


#endif//__DDZ_DB_THREAD_H__
