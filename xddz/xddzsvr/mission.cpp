#include "mission.h"
#include "ddzprot.h"
#include "config_manager.h"
#include "ddz_def.h"
#include "user_manager.h"
#include "db_thread.h"
#include <time.h>
#include "cflog.h"
#include "socket_service.h"
#include "game_main_thread.h"
#include "log_type.h"
#include "string_res.h"
#include "redis_thread.h"

mission::mission(void)
{
	
}

mission::~mission(void)
{
}

void mission::on_mission(socket_id sid, msg_object_base* obj)
{
	switch (obj->minor_id())
	{
	case MN_GET_AWARD:
		on_get_count_box_reward(sid);
		break;
	default:
		break;
	}
	return;
}

void mission::on_get_count_box_reward(socket_id sid)
{
    if(GAME_MODE_CFG==GAME_TYPE_PK)
        return;

	room_user* user = single_game_room::instance()->get_userinfo(sid);
	if(!user)
		return;
	int ct = _time32(0);
	int dt = ct - user->count_box_timestamp;
	user->count_box.seconds += dt;
	user->count_box_timestamp = _time32(0);

	count_box_item* box = config_manager::instance()->get_online_mission(user->count_box.level);
	if(!box)
    {
        LOG_ERR_LINE();
		return;
    }

	sc_get_award_result obj;
	award_msg item;	
	strcpy_s(item.mname, sizeof(item.mname), "计时宝箱");
	strcpy_s(item.nicktext, sizeof(item.nicktext), user->nickname);
	item.userid = user->uid;
    char amsg[1024] = {0};
	if(user->count_box.seconds >= box->t)
	{
		if(box->pid > 0 && box->pn > 0)
		{
			strcpy_s(item.msg, sizeof(item.msg), STRING_RES(13));
			item.num = box->pn;
			char* pname = config_manager::instance()->get_prop_name(box->pid);
			if (pname == NULL)
			{
				strcpy_s(item.propname, sizeof(item.propname), "道具");
			}
			else
				strcpy_s(item.propname, sizeof(item.propname), pname);
			db_thread::instance()->on_add_prop(COUNT_BOX_REWARD, user->uid, box->pid, box->pn);
            //%s完成了<c=ff00ff>%s</c>任务获得了<c=ff00ff>%d</c>个<c=ff00ff>%s</c>道具
            sprintf(amsg, STRING_RES(13), user->nickname, item.mname, box->pn, pname);
            do_award_string(user->uid, amsg);
		}
		if(box->coin > 0)
		{
			strcpy_s(item.msg, sizeof(item.msg), STRING_RES(14));
			item.coin = box->coin;
			user->coin += box->coin;
            //14=%s完成了<c=ff00ff>%s</c>任务获得了<c=ff00ff>%d</c>金币
            sprintf(amsg, STRING_RES(14), user->nickname, item.mname, box->coin);
            do_award_string(user->uid, amsg);

			db_thread::instance()->on_add_coin(COUNT_BOX_REWARD, user->uid, box->coin);
		}
		obj.m_result = 0;
		obj.m_msg = box->desc;
		socket_service::instance()->send_obj(sid, &obj);
		

		user->count_box.seconds = 0;
		user->count_box.level ++;
		redis_thread::instance()->on_update_count_box(user->uid, user->count_box.level, user->count_box.seconds);
	}
	else
	{
		obj.m_result = 1;
		char msg[64];
		sprintf_s(msg, sizeof(msg), "the timer is not ok, please wait for a moment. thanks.");
		obj.m_msg = msg;
		socket_service::instance()->send_obj(sid, &obj);
	}
	game_main_thread::instance()->send_count_box(sid, user->count_box.level, user->count_box.seconds);
}

void mission::update_mission(socket_id sid)
{
    if(GAME_MODE_CFG==GAME_TYPE_PK ||
        GAME_MODE_CFG == GAME_TYPE_MATCH)
        return;
	room_user* user = single_game_room::instance()->get_userinfo(sid);
	if(!user)
		return;
	int ct = _time32(0);
	int dt = ct - user->count_box_timestamp;
	user->count_box.seconds += dt;
	user->count_box_timestamp = _time32(0);

	count_box_item* box = config_manager::instance()->get_online_mission(user->count_box.level);
	if(!box)
    {
		return;
    }
	redis_thread::instance()->on_update_count_box(user->uid, user->count_box.level, user->count_box.seconds);
}

void mission::do_award_string(unsigned int uid, char* msg)
{
    if(!msg || !uid)
        return;
	award_record _msg;
	memset(&_msg,0,sizeof(_msg));
	_msg.uid = uid;
	strcpy_s(_msg.text, sizeof(_msg.text), msg);
	db_thread::instance()->on_award_msg_log(_msg);

	return;
}
