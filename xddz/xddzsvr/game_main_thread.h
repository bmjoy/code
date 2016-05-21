#ifndef __GAME_MAIN_THREAD_H__
#define __GAME_MAIN_THREAD_H__
#include "singleton.h"
#include "ring_buffer2_thread.h"
#include "timer.h"
#include "i_tcp_socket_engine.h"
#include "ddz_def.h"
#include "db_thread.h"
#include <vector>
#include "game_room.h"
using namespace std;

class game_main_thread
    : public singleton<game_main_thread>
    , public ring_buffer2_thread<1>
    , public i_timer_service
{
public:
    game_main_thread(void);
    ~game_main_thread(void);
	void init_size();
	void on_client_connect(socket_id sid);
    void on_client_close(socket_id sid);
    void on_client_recive(socket_id sid, char* pdata, int len);

    virtual void on_data(void* phead, int headlen, void* pdata, int datalen);
    virtual void on_timer(unsigned short id1, unsigned short id2, unsigned short id3, unsigned short tid, int data);

    void on_db_login_result(socket_id sid, unsigned int uid, int result, room_user& userinfo, int serverid=0);

    void start_update_timer();

	void send_count_box(socket_id sid, int level, int seconds);

    void on_count_box(socket_id sid, unsigned int uid, int seconds, int level);

    void on_ms_connect();
    void on_ms_recive(char* data, int len);
    void on_ms_close();
    void on_load_match(int sn, list<room_user>* l);
    void on_card_mission_index(unsigned int uid, int index);

	void do_client_connect(socket_id sid);
    void do_client_close(socket_id sid);
    void do_client_recive(socket_id sid, char* pdata, int len);
    void do_timer(unsigned short id1, unsigned short id2, unsigned short id3, unsigned short tid, int data);
    void on_login(socket_id sid, char* openid, int len);

	void on_relogin(socket_id sid, room_user* p_user);
    void on_db_login_fail(socket_id sid, unsigned int uid, db_login_fail_result result);
    void on_db_login_ok(socket_id sid, unsigned int uid, room_user& userinfo);
    void send_login_result(socket_id sid, unsigned int uid, int result, int serverid=0);
    void send_game_config(socket_id sid);
    void do_count_box(socket_id sid, unsigned int uid, int seconds, int level);

    void do_ms_connect();
    void do_ms_recive(char* data, int len);
    void do_ms_close();
    void do_load_match(int sn, list<room_user>* l);
    void on_login_match(socket_id sid, char* openid, int len, int sn);
    void do_card_mission_index(unsigned int uid, int index);

	vector<int> m_user_room;
	game_room* get_room(socket_id sid);
};


#endif//__GAME_MAIN_THREAD_H__