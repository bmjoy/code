#include "game_room.h"
#include "config_manager.h"
#include "timer.h"
#include "socket_service.h"
#include "user_manager.h"
#include "cflog.h"
#include <vector_helper.h>
#include "string_res.h"
#include "db_thread.h"
#include "game_main_thread.h"
#include "redis_thread.h"

desk_manager::desk_manager()
{
}

desk_manager::~desk_manager()
{

}

void desk_manager::init(int maxdesk)
{
    m_desk_queue.init(maxdesk);
}

void desk_manager::fini()
{
    m_desk_queue.fini();
}

ddz_desk* desk_manager::get_desk(int roomid, int deskid)
{
    ddz_desk* ret = 0;
    if(!m_desk_queue.pop(ret) || ret)
    {
        ret = new ddz_desk;
    }
    ret->set_index(roomid, deskid);
    return ret;
}

void desk_manager::put_desk(ddz_desk* desk)
{
    if(!m_desk_queue.put(desk))
    {
        delete desk;
    }
}
//////////////////////////////////////////////////////////////////////////


game_room::game_room(void)
{
    m_roomid = -1;
    m_matching = false;
}

game_room::~game_room(void)
{
}

void game_room::init(int maxdesk)
{
    m_desks.resize(maxdesk, 0);
    for(int i=0; i<maxdesk; i++)m_desks[0] = 0;
    m_queue.init(maxdesk*DDZ_SEATS);
    m_queue_back.init(maxdesk*DDZ_SEATS);
}

void game_room::init_users()
{
	m_users.init(config_manager::instance()->get_max_con());
}


void game_room::fini()
{
    release_vector(m_desks);
    m_queue_back.fini();
    m_queue.fini();
}

void game_room::start()
{
    if(GAME_MODE_CFG == GAME_TYPE_NORMAL)
        timer_singleton::instance()->set_timer(TID1_ROOM, 0, 0, TID_ARRANGE_SITE, config_manager::instance()->arrange_time(), 0);
}

void game_room::send(socket_id sid, char* buf, int len)
{
    socket_service::instance()->send(sid, buf, len);
}

room_user* game_room::get_userinfo(socket_id sid, unsigned int uid)
{
    room_user* user = m_users.get_userinfo(sid, uid);
    if(!user)
    {
        user = m_users.get_net_cut_userinfo(uid);
    }
    return user;
}

room_user* game_room::get_userinfo(socket_id sid)
{
    return m_users.get_userinfo(sid);
}

room_user* game_room::get_netcut_userinfo(unsigned int uid)
{
    return m_users.get_net_cut_userinfo(uid);
}

ddz_game_config game_room::get_config()
{
    return config_manager::instance()->get_game_config();
}

void game_room::set_timer(int deskid, unsigned short tid, int delay, int data)
{
    //LOG_DEBUG("set_timer: id1=%d, id2=%d, id3=%d, tid=%d, delay=%d, data=%d", TID1_GAME, m_roomid, deskid, tid, delay, data);

    timer_singleton::instance()->set_timer(TID1_GAME, m_roomid, deskid, tid, delay, data);
}

void game_room::kill_timer(int deskid, unsigned short tid)
{
    timer_singleton::instance()->kill_timer(TID1_GAME, m_roomid, deskid, tid);
}

void game_room::clear_net_cut(unsigned int uid)
{
    m_users.clear_net_cut(uid);
}

void game_room::on_room_timer(short tid, int data)
{
    switch(tid)
    {
    case TID_ARRANGE_SITE:
        if(IS_PK_MODE)
            return;
        arrange_desk();
        timer_singleton::instance()->set_timer(TID1_ROOM, 0, 0, TID_ARRANGE_SITE, config_manager::instance()->arrange_time(), 0);
        break;
    case TID_WAIT_START_MATCH:
        start_match();
        break;
    }
}

void game_room::on_desk_timer(short deskid, short tid, int data)
{
    if(deskid >= (int)m_desks.size() || deskid < 0)
    {
        LOG_ERROR("%s %d roomid:%d, deskid:%d", __FILE__, __LINE__, m_roomid, deskid);
        return;
    }
    if(m_desks[deskid] == 0)
    {
        LOG_ERROR("%s %d roomid:%d, deskid:%d", __FILE__, __LINE__, m_roomid, deskid);
        return;
    }
    m_desks[deskid]->on_timer(tid, data);
}

void game_room::arrange_desk()
{
    int n_user = m_queue.get_size();
    //LOG_DEBUG("队列人数：%d", n_user);

    //清理队列中的掉线用户
    m_queue_back.resize(0);
    for(int i=0; i<n_user; i++)
    {
        socket_id sid = m_queue[i];
        if(socket_service::instance()->is_online(sid))
        {
            room_user* user = m_users.get_userinfo(sid);
            if(user && user->deskid<0)
                m_queue_back.add(sid);
        }
    }
    m_queue.resize(0);
    for(int i=0; i<m_queue_back.get_size(); i++)
    {
        m_queue.add(m_queue_back[i]);
    }

    m_queue_back.resize(0);
    for(int i=0; i<n_user; i++)
    {
        int r = rand() % n_user;
        if(r != i)
        {
            socket_id id = m_queue[i];
            m_queue[i] = m_queue[r];
            m_queue[r] = id;
        }
    }
    int fromindex = 0;
    ddz_desk* p_desk = 0;
    for(int i=0; i<n_user; i++)
    {
        socket_id sid = m_queue[i];
        if(!socket_service::instance()->is_online(sid))
            continue;
        if(!p_desk)
        {
            p_desk = get_empty_desk(fromindex);
            if(!p_desk)
            {
                for(int j=i; j<n_user; j++)
                {
                    if(socket_service::instance()->is_online(m_queue[j]))
                        m_queue_back.add(m_queue[i]);
                }
                m_queue.resize(0);
                for(int j=0; j<m_queue_back.get_size(); j++)
                    m_queue.add(m_queue_back[j]);
                return;
            }
            //LOG_DEBUG("alloc desk id=%d", p_desk->get_deskid());
        }
        p_desk->on_sitdown(sid);
        if(p_desk->get_empty_size() == 0)
        {
            p_desk = 0;
        }
    }
    m_queue.resize(0);
    for(int j=0; j<m_queue_back.get_size(); j++)
        m_queue.add(m_queue_back[j]);

    arrange_robot();
}

void game_room::arrange_robot()
{
    if(m_users.online_users() > config_manager::instance()->m_robot_work_limit)
        return;

    if(m_robot_arrange_ticker < 0)m_robot_arrange_ticker=0;
    m_robot_arrange_ticker++;
    if(m_robot_arrange_ticker < 2) return;
    m_robot_arrange_ticker = 0;

    for(size_t i=0; i<m_desks.size(); i++)
    {
        if(!m_desks[i])
            continue;
        if(m_desks[i]->get_desk_state() != DS_GAME_END)
            continue;
        int empty_size = m_desks[i]->get_empty_size();
        if(empty_size<1 || empty_size>2)
            continue;
        room_user* user = robot_inst->get_idle();
        if(!user)
            return;
        if(!m_desks[i]->set_robot_sitdown(user))
            robot_inst->put(user->uid);
    }
}

void game_room::add_queue(socket_id sid)
{
    for(int i=0; i<m_queue.get_size(); i++)
    {
        if(sid == m_queue[i])
            return;
    }
    m_queue.add(sid);
    send_arrange_desk(sid);
}

void game_room::send_arrange_desk(socket_id sid)
{
    sc_arrange_desk obj;
    socket_service::instance()->send_obj(sid, &obj);
}

void game_room::on_create_pk_desk(socket_id sid)
{
    room_user* user = m_users.get_userinfo(sid);
    if(!user || user->deskid >= 0)
    {
        //在桌上用户，忽略该消息
        return;
    }

    for(int i=0; i<(int)m_desks.size(); i++)
    {
        if(m_desks[i] == NULL)
        {
            m_desks[i] = desk_manager::instance()->get_desk(m_roomid, i);
			m_desks[i]->init(this, config_manager::instance()->get_game_config());
        }
        if(m_desks[i]->get_empty_size() == DDZ_SEATS)
        {
            m_desks[i]->create_pk(sid);
            return;
        }
    }
    sc_create_pk_result obj;
    obj.m_result = 1;
    socket_service::instance()->send_obj(sid, &obj);
}

void game_room::on_add_pk_desk(socket_id sid, char* code)
{
    room_user* user = m_users.get_userinfo(sid);
    if(!user || user->deskid >= 0)
    {
        //在桌上用户，忽略该消息
        return;
    }
    for(int i=0; i<(int)m_desks.size(); i++)
    {
        if(m_desks[i] && m_desks[i]->add_pk(sid, code))
            return;
    }
    sc_add_pk_result obj;
    obj.m_result = 1;
    obj.m_msg = STRING_RES(20);
    socket_service::instance()->send_obj(sid, &obj);
}

ddz_desk* game_room::get_empty_desk(int& fromindex)
{
    for(int i=fromindex; i<(int)m_desks.size(); i++)
    {
        if(!m_desks[i])
        {
            m_desks[i] = desk_manager::instance()->get_desk(m_roomid, i);
            if(m_desks[i])
            {
                fromindex ++;
                if(fromindex>=(int)m_desks.size())
                    fromindex = 0;
                m_desks[i]->init(this, config_manager::instance()->get_game_config());
                return m_desks[i];
            }
            continue;
        }
        if(m_desks[i]->get_empty_size()>0)
            return m_desks[i];
    }
    for(int i=0; i<fromindex; i++)
    {
        if(!m_desks[i])
        {
            m_desks[i] = desk_manager::instance()->get_desk(m_roomid, i);
            if(m_desks[i])
            {
                fromindex ++;
                if(fromindex>=(int)m_desks.size())
                    fromindex = 0;
                m_desks[i]->init(this, config_manager::instance()->get_game_config());
                return m_desks[i];
            }
            continue;
        }
        if(m_desks[i]->get_empty_size()>0)
            return m_desks[i];
    }
    return 0;
}

ddz_desk* game_room::get_desk(int deskid)
{
    if(deskid < 0 || deskid > (int)m_desks.size())
    {
        return 0;
    }
    if(!m_desks[deskid])
        return 0;
    return m_desks[deskid];
}


void game_room::on_left_desk_request(socket_id sid, int type)
{
    DESK_USER(user, desk);

    // type: 0退出房间 1换桌
    if(type == 0)
    {
        socket_service::instance()->close_client(sid);
        return;
    }
    // 游戏中无视换桌指令
    if(desk->get_desk_state()!=DS_GAME_END)
    {
        return;
    }
    desk->on_standup(user->seatid);
    add_queue(sid);
}

void game_room::on_user_ready(socket_id sid)
{
	//LOG_DEBUG("on_user_ready");
    DESK_USER(user, desk);
    desk->on_ready(user->seatid);
}

void game_room::on_game_message(socket_id sid, msg_object_base* obj)
{
    DESK_USER(user, desk);
    desk->on_recive(sid, obj);
}

bool game_room::on_client_close(socket_id sid)
{

	room_user* user = m_users.get_userinfo(sid);
	if(!user)
		return false;
	bool b_net_cut = false;

    if(GAME_MODE_CFG == GAME_TYPE_MATCH)
    {
        ddz_desk* desk = get_desk(user->deskid);
        if(desk)
        {
            desk->on_socket_close(sid, user->seatid);
            b_net_cut = true;
        }
		else
		{
			b_net_cut = is_match_user(user->uid);
		}
    }
    else
    {
        ddz_desk* desk = get_desk(user->deskid);
        if(desk)
		{
			if(desk->is_gaming())
			{
				desk->on_socket_close(sid, user->seatid);
				b_net_cut = true;
			}
			else
			{
				desk->user_left(sid, user->uid);
			}
		}
    }
	m_users.clear_user_info(sid, b_net_cut);
	return false;
}

void game_room::on_login(socket_id sid, unsigned int uid)
{
	room_user* p_user = m_users.is_net_cut(uid, sid);
	if(p_user)
	{
		on_recome(sid, p_user);
		return;
	}
	p_user = m_users.is_online_user(uid);
	if(p_user)
	{
		if(p_user->sid == sid)
		{
			return;
		}
		//on_relogin(sid, p_user);
		mn_cut_off obj1;
		obj1.m_id = LOGIN_RESULT_ACCOUNT_IN_USE;
		obj1.m_msg = STRING_RES(3);
		socket_service::instance()->send_obj(p_user->sid, &obj1);
		game_main_thread::instance()->do_client_close(p_user->sid);
		socket_service::instance()->close_client(p_user->sid);
		p_user = m_users.is_net_cut(uid, sid);
		if(p_user)
		{
			on_recome(sid, p_user);
			return;
		}
	}
	p_user = robot_inst->user_info(uid);
	if(p_user)
	{
		game_main_thread::instance()->send_login_result(sid, 0, LOGIN_RESULT_ACCOUNT_IN_USE);
		return;
	}
	db_thread::instance()->on_login(sid, uid);

}

void game_room::on_db_login_ok(socket_id sid, unsigned int uid, room_user& userinfo)
{

	//判断重复登录
	room_user* p_user = m_users.is_online_user(uid);
	if(p_user)
	{
		//相同链接重复发送登录命令
		if(p_user->sid == sid)
			return;
		mn_cut_off obj1;
		obj1.m_id = LOGIN_RESULT_ACCOUNT_IN_USE;
		obj1.m_msg = STRING_RES(3);
		socket_service::instance()->send_obj(p_user->sid, &obj1);
		game_main_thread::instance()->do_client_close(p_user->sid);
		socket_service::instance()->close_client(p_user->sid);
		p_user = m_users.is_net_cut(uid, sid);
		if(p_user)
		{
			on_recome(sid, p_user);
			return;
		}
		return;
	}

	{
		//判断积分条件
		//判断金币条件
		if(config_manager::instance()->get_ac().less_coin != 0 &&
			config_manager::instance()->get_ac().less_coin > userinfo.coin)
		{
			db_thread::instance()->on_logout(uid);
			game_main_thread::instance()->send_login_result(sid, uid, LOGIN_RESULT_LESS_COIN);
			return;
		}
		if(config_manager::instance()->get_ac().max_coin != 0 &&
			config_manager::instance()->get_ac().max_coin < userinfo.coin)
		{
			db_thread::instance()->on_logout(uid);
			game_main_thread::instance()->send_login_result(sid, uid, LOGIN_RESULT_MORE_COIN);
			return;
		}
		//判断VIP身份
		if(config_manager::instance()->login_vip())
		{
			if(userinfo.vip.vip_tag == 0)
			{
				db_thread::instance()->on_logout(uid);
				game_main_thread::instance()->send_login_result(sid, uid, LOGIN_RESULT_NEED_VIP);
				return;
			}
		}
	}
	//判断

	userinfo.roomid = m_roomid;
	m_users.add_user(userinfo);

	game_main_thread::instance()->send_login_result(sid, uid, LOGIN_RESULT_OK);
	if(!IS_PK_MODE)
		redis_thread::instance()->on_load_count_box(sid, uid);

	//PK房不自动安排座位
	if(!IS_PK_MODE)
		add_queue(sid);

	if(cm->mode() != CARD_MISSION_RAMDOM && cm->mode() != 0)
	{
		redis_thread::instance()->on_get_card_mission_index(uid);
	}

}

void game_room::on_recome(socket_id sid, room_user* p_user)
{
	if(!p_user)
		return;
	if(GAME_MODE_CFG != GAME_TYPE_MATCH)
	{
		ddz_desk* desk = get_desk(p_user->deskid);
		if(!desk)
		{
			return;
		}
		game_main_thread::instance()->send_login_result(sid, p_user->uid, LOGIN_RESULT_OK);
		p_user->update_count_box();
		game_main_thread::instance()->send_count_box(sid, p_user->count_box.level, p_user->count_box.seconds);

		desk->on_user_recome(sid, p_user->uid, p_user->seatid);
		return;
	}
}

room_user* game_room::is_net_cut(socket_id sid, unsigned int uid)
{
	return m_users.is_net_cut(uid, sid);
}

room_user* game_room::is_online_user(unsigned int uid)
{
	return m_users.is_online_user(uid);
}