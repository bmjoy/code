#ifndef __GAME_ROOM_H__
#define __GAME_ROOM_H__
#include "singleton.h"
#include "ddz_desk.h"
#include "safe_queue.h"
#include <map>
#include "user_manager.h"
using namespace std;

class desk_manager : public singleton <desk_manager>
{
public:
    desk_manager();
    ~desk_manager();
    void init(int maxdesk);
    void fini();

    ddz_desk* get_desk(int roomid, int deskid);
    void put_desk(ddz_desk* desk);

private:
    safe_queue<ddz_desk*> m_desk_queue;
};

enum ENUM_ROOM_TID
{
    TID_ARRANGE_SITE,
    TID_WAIT_START_MATCH,
};


class game_room : public i_gamedesk_controler
{
public:
    game_room(void);
    ~game_room(void);
    void init(int maxdesk);
	void init_users();
    void fini();
    void start();

    virtual void send(socket_id sid, char* buf, int len);
    virtual room_user* get_userinfo(socket_id sid, unsigned int uid);
    virtual room_user* get_userinfo(socket_id sid);
    virtual room_user* get_netcut_userinfo(unsigned int uid);
    virtual ddz_game_config get_config();
    virtual void set_timer(int deskid, unsigned short tid, int delay, int data);
    virtual void kill_timer(int deskid, unsigned short tid);
    virtual void clear_net_cut(unsigned int uid);
    virtual int  get_base_point();

    void on_room_timer(short tid, int data);
    void on_desk_timer(short deskid, short tid, int data);
    void add_queue(socket_id sid);
    void on_left_desk_request(socket_id sid, int type);
    void on_user_ready(socket_id sid);
    void on_game_message(socket_id sid, msg_object_base* obj);

    ddz_desk* get_desk(int deskid);

    void on_create_pk_desk(socket_id sid);
    void on_add_pk_desk(socket_id sid, char* code);
    void send_arrange_desk(socket_id sid);
    void roomid(int n){m_roomid = n;}
	int  get_roomid(){return m_roomid;}
    void on_match_msg(socket_id sid, msg_object_base* obj);
	void on_login(socket_id sid, unsigned int uid);
	void on_db_login_ok(socket_id sid, unsigned int uid, room_user& userinfo);
	void on_recome(socket_id sid, room_user* p_user);
	room_user* is_net_cut(socket_id sid, unsigned int uid);
	room_user* is_online_user(unsigned int uid);
private:
    void arrange_desk();
    ddz_desk* get_empty_desk(int& fromindex);
    void arrange_robot();
private:
    vector<ddz_desk*> m_desks;
    svector<socket_id> m_queue;
    svector<socket_id> m_queue_back;

    int m_roomid;
    int m_robot_arrange_ticker;

public:
    virtual bool on_match_play_end(unsigned int uids[3], int score[3], ddz_desk* desk);
    void init_match(list<room_user>* l);
    bool is_matching(){return m_matching;}
    void set_matching(int sn){m_matching = true;m_sn = sn;m_match_turns=0;}
    void end_match();
    void start_match();
    void on_match_user_login_ok(unsigned int uid, socket_id sid);
    int  get_login_size();
    int  get_enroll_size();
    void check_start_match();
    match_user_item* get_match_user(unsigned int uid);
    void send_rank_info(socket_id sid, unsigned int uid, int page=0);
    void send_wait_match(socket_id sid);
    void arrange_match_seat();
    bool is_match_user(unsigned int uid);
    bool on_client_close(socket_id sid);
    int  match_id(){return m_sn;}
    void sort_rank();
    sc_match_rank_info get_rank_info(int page);
    virtual int  get_match_users() {return (int)m_rankuser.size();}
    virtual RuleTurnItem* get_turn_item();
    void do_end_user_match(unsigned int uid);
    bool has_matching_desk();
    void next_turns();
    void on_get_rank_info(socket_id sid, int page);
private:
    //比赛信息
    map<unsigned int, match_user_item> m_enroll_user;
    vector<match_user_item*>           m_rankuser;
    int    m_sn;
    bool   m_matching;
    bool   m_wait_start_match;
    int    m_match_turns;
    time_t m_turns_time_stamp;
    time_t m_start_time;

	//////////////////////////////////////////////////////////////////////////
	user_manager m_users;
};

typedef singleton<game_room> single_game_room;

class match_room_manager : public singleton<match_room_manager>
{
public:
    match_room_manager();
    ~match_room_manager();
    game_room* get_empty_room(int sn);
    void match_end(int sn);
    game_room* get_matching_room(int sn);
    game_room* get_room(int roomid);
    int get_matching_room_sn(int* sns, int size);
private:
    vector<game_room*>   m_room;
    map<int,game_room*> m_matching_room;
};
#define match match_room_manager::instance()

#define DESK_USER(user, desk) \
    room_user* user = m_users.get_userinfo(sid);\
    if(!user || user->deskid < 0 || user->deskid >= (int)m_desks.size() || !m_desks[user->deskid]|| user->seatid<0 || user->seatid>=DDZ_SEATS)\
    {\
        socket_service::instance()->close_client(sid);\
        return;\
    }\
    ddz_desk* desk = m_desks[user->deskid];

#endif//__GAME_ROOM_H__