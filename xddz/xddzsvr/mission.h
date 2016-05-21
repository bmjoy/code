#ifndef __MISSION_H__
#define __MISSION_H__

#include "singleton.h"
#include "i_tcp_socket_engine.h"
#include "msg_object_base.h"
#include "user_manager.h"

struct award_msg
{
	unsigned int userid;
	char nicktext[64] ;
	char propname[32];
	char mname[32];
	int  num;
	long long coin;
	char msg[256];
};

class mission : public singleton<mission>
{
public:
	mission(void);
	~mission(void);

	void on_mission(socket_id sid, msg_object_base* obj);
	void update_mission(socket_id sid);
private:
	void on_get_count_box_reward(socket_id sid);
    //void do_award_string(award_msg item);
    void do_award_string(unsigned int uid, char* msg);
	//void on_count_box_reward(room_user* user, int level);
};

#endif//__MISSION_H__