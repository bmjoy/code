#include "game_main_thread.h"
#include "msg_object_base.h"
#include "socket_service.h"
#include "ddz_def.h"
#include "ddzprot.h"
#include "token2.h"
#include "string_res.h"
#include "user_manager.h"
#include "ddz_desk.h"
#include "db_thread.h"
#include "ddzprot.h"
#include "game_room.h"
#include "config_manager.h"
#include "cflog.h"
#include "mission.h"
#include "log_type.h"
#include <time.h>
#include "redis_thread.h"
#include "ms_connector.h"
#include "card_mission.h"

struct main_thread_type_base
{
    int type;
    socket_id sid;
};

#define MAKE_TYPE(d,_type,_sid) main_thread_type_base d;d.type=_type;d.sid=_sid;

enum THREAD_TYPE
{
	T_CLIENT_CONNECT,	//链接成功
    T_CLIENT_CLOSE,     //用户断线
    T_CLIENT_RECIVE,    //用户消息
    T_TIMER,            //定时器
    T_USERINFO_OK,      //用户异步信息加载成功
    T_USERINFO_FAIL,    //用户一部信息加载失败
    T_BANCKRUPT,        //破产赠送结果
    T_LOAD_COUNT_BOX,   //加载计时宝箱
    T_MS_CONNECT,
    T_MS_CLOSE,
    T_MS_RECIVE,
    T_DB_LOAD_MATCH_USER,
    T_CARD_MISSION_INDEX
    
};

struct timer_data_struct
{
    unsigned short id1;
    unsigned short id2;
    unsigned short id3;
    unsigned short tid;
    int data;
};

struct bankrupt_info
{
    unsigned int uid;
    int			 count;
	int			 result;
	__int64		 coin;

};

struct t_count_box_info: public count_box_info
{
    unsigned int uid;
};

struct t_load_db_match_user
{
    int sn;
    list<room_user>* l;
};

struct t_card_mission_index
{
    unsigned int uid;
    int index;
};

//////////////////////////////////////////////////////////////////////////
game_main_thread::game_main_thread(void)
{
}

game_main_thread::~game_main_thread(void)
{
}

void game_main_thread::init_size()
{
	int size = config_manager::instance()->get_max_con();
	m_user_room.resize(size);
	for(int i=0; i<size; i++)
		m_user_room[i] = -1;
}

void game_main_thread::on_data(void* phead, int headlen, void* pdata, int datalen)
{
    main_thread_type_base* d = (main_thread_type_base*)phead;
    switch(d->type)
    {
	case T_CLIENT_CONNECT:
		{
			do_client_connect(d->sid);
		}
		break;
    case T_CLIENT_CLOSE:
        {
            do_client_close(d->sid);
        }
        break;
    case T_CLIENT_RECIVE:
        {
            do_client_recive(d->sid, (char*)pdata, datalen);
        }
        break;
    case T_TIMER:
        {
            timer_data_struct* t = (timer_data_struct*)pdata;
            do_timer(t->id1, t->id2, t->id3, t->tid, t->data);
        }
        break;
    case T_USERINFO_OK:
        {
            room_user* user = (room_user*)pdata;
            on_db_login_ok(d->sid, user->uid, *user);
        }
        break;
    case T_USERINFO_FAIL:
        {
            db_login_fail_result result= *(db_login_fail_result*)pdata;
            on_db_login_fail(d->sid, 0, result);
        }
        break;
    case T_LOAD_COUNT_BOX:
        {
            t_count_box_info box = *(t_count_box_info*)pdata;
            do_count_box(d->sid, box.uid, box.seconds, box.level);
        }
        break;
    case T_MS_CONNECT:
        do_ms_connect();
        break;
    case T_MS_CLOSE:
        do_ms_close();
        break;
    case T_MS_RECIVE:
        do_ms_recive((char*)pdata, datalen);
        break;
    case T_DB_LOAD_MATCH_USER:
        {
            t_load_db_match_user* t = (t_load_db_match_user*)pdata;
            do_load_match(t->sn, t->l);
        }
        break;
    case T_CARD_MISSION_INDEX:
        {
            t_card_mission_index item = *(t_card_mission_index*)pdata;
            do_card_mission_index(item.uid, item.index);
        }
        break;
    default:
        break;
    }
}

void game_main_thread::on_client_connect(socket_id sid)
{
	MAKE_TYPE(d, T_CLIENT_CONNECT, sid);
	put_data(&d, sizeof(d), 0, 0);
	if (sid.index>=0 && sid.index<m_user_room.size())
	{
		m_user_room[sid.index] = -1;
	}
}

void game_main_thread::on_client_close(socket_id sid)
{
    MAKE_TYPE(d, T_CLIENT_CLOSE, sid);
    put_data(&d, sizeof(d), 0, 0);
}

void game_main_thread::on_client_recive(socket_id sid, char* pdata, int len)
{
    msg_object_head* head = (msg_object_head*)pdata;
    if(head->m_major_id == MJ_GAME_SERVER && head->m_minor_id == MN_PING)
    {
        // 心跳协议直接返回，不再往主线程队列中压数据
        // LOG_DEBUG("心跳");
        mn_ping ping;
        if(!ping.serialize_in(pdata, len))
            return;
        mn_pong pong;
        pong.m_value = ping.m_value;
        socket_service::instance()->send_obj(sid, &pong);
        return;
    }

    main_thread_type_base d = {T_CLIENT_RECIVE, sid};
    put_data(&d, sizeof(d), pdata, len);
}

void game_main_thread::on_timer(unsigned short id1, unsigned short id2, unsigned short id3, unsigned short tid, int data)
{
    MAKE_TYPE(d, T_TIMER, INVALID_SOCKET_ID);
    timer_data_struct t = {id1, id2, id3, tid, data};
    put_data(&d, sizeof(d), &t, sizeof(t));
}

void game_main_thread::on_db_login_result(socket_id sid, unsigned int uid, int result, room_user& userinfo, int serverid)
{
    if(result == LOGIN_RESULT_OK)
    {
        MAKE_TYPE(d, T_USERINFO_OK, sid);
        put_data(&d, sizeof(d), &userinfo, sizeof(userinfo));
        return;
    }
    else
    {
        MAKE_TYPE(d, T_USERINFO_FAIL, sid);
        db_login_fail_result r;
        r.result = result;
        r.serverid = serverid;
        put_data(&d, sizeof(d), &r, sizeof(r));
    }
}
//////////////////////////////////////////////////////////////////////////
void game_main_thread::do_client_connect(socket_id sid)
{
	if (IS_MATCH_MODE && sid.index>=0 && sid.index<m_user_room.size())
	{
		m_user_room[sid.index] = -1;
	}
}

void game_main_thread::do_client_close(socket_id sid)
{
    //LOG_DEBUG("do_client_close");
	int index = sid.index;
	game_room* room = get_room(sid);
	if(room)room->on_client_close(sid);
	if (sid.index>=0 && sid.index<m_user_room.size())
	{
		m_user_room[sid.index] = -1;
	}
}

game_room* game_main_thread::get_room(socket_id sid)
{
	if(sid.index<0 || sid.index>=m_user_room.size())
		return 0;
	if(IS_MATCH_MODE)
	{
		return match->get_room(m_user_room[sid.index]);
	}
	return single_game_room::instance();
}

#define IN_OJB(classtype,obj) classtype obj; if(!obj.serialize_in(pdata, len)) return;

void game_main_thread::do_client_recive(socket_id sid, char* pdata, int len)
{
	if(sid.index<0 || sid.index>=m_user_room.size())
		return;
	char buf[4096];
	msg_object_base* obj = ddzprotObjectManager::CreateObject(pdata, len, buf);
	if(!obj)
		return;
	LOG_DEBUG("ddz_desk::on_recive mid=%d nid=%d", obj->major_id(), obj->minor_id());
    msg_object_head* head = (msg_object_head*)pdata;
	game_room* room = get_room(sid);
	if(!room)
	{
		if(IS_MATCH_MODE && head->m_major_id == MJ_MATCH_DDZ && MN_LOGIN_MATCH == head->m_minor_id)
		{
			CAST_OBJ(cs_login_match);
			room = match->get_matching_room(o->m_sn);
			if(!room)
			{
				LOG_DEBUG("登录比赛不存在1 sn=%d", o->m_sn);
				send_login_result(sid, 0, LOGIN_RESULT_MATCH_END);
				return;
			}
			on_login_match(sid, o->m_openid.m_buf, o->m_openid.m_len, o->m_sn);
		}
		return;
	}
	switch (head->m_major_id)
	{
	case MJ_GAME_SERVER:
		{
			switch (head->m_minor_id)
			{
			case MN_LOGIN:
				{
					if(IS_MATCH_MODE)
						return;
					CAST_OBJ(cs_login);
					on_login(sid, o->m_openid.m_buf, o->m_openid.m_len);
				}
				break;
			default:
				break;
			}
		}
		return;
		break;
	case MJ_GAME_CONTROL_DDZ:
		{
			switch(head->m_minor_id)
			{
			case MN_GAME_USER_SITDOWN://暂时不支持用户坐下，服务器自动分配座位
				break;
			case MN_LEFT_DESK_REQUEST:
				{
					CAST_OBJ(left_desk_request);
					room->on_left_desk_request(sid, o->m_type);
				}
				break;
			case MN_USER_READY:
				{
					CAST_OBJ(sc_user_ready);
					room->on_user_ready(sid);
				}
				break;
			default:
				break;
			}
		}
		break;
	case MJ_PK_GAME:
		{
			switch(head->m_minor_id)
			{
			case MN_SELECT_MODE:
				{
					sc_select_mode obj;
					if(!obj.serialize_in(pdata, len))
						return;
					if(obj.m_type == CREATE_PK_GAME)
					{
						room->on_create_pk_desk(sid);
					}
					else if(obj.m_type == ADD_PK_GAME)
					{
						room->on_add_pk_desk(sid, obj.m_code);
					}
					else
					{
						socket_service::instance()->close_client(sid);
					}
				}
				break;
			default:
				{
					room->on_game_message(sid, obj);
				}
				break;
			}
		}
		break;
	case MJ_GAME_LOGIC_DDZ:
	case MJ_MESSAGE_DDZ_GAME:
		{
			room->on_game_message(sid, obj);
		}
		break;
	case MJ_MISSION_DDZ_SERVER:
		{
			mission::instance()->on_mission(sid, obj);
		}
		break;
	case MJ_MATCH_DDZ:
		{
			room->on_match_msg(sid, obj);
		}
		break;
	default:
		break;
	}
}

void game_main_thread::do_timer(unsigned short id1, unsigned short id2, unsigned short id3, unsigned short tid, int data)
{
    switch(id1)
    {
    case TID1_PLATFORM:
        {
            switch(id2)
            {
            case 0:
                {
                    int id = config_manager::instance()->serverid();
                    int n = socket_service::instance()->get_onlines();
                    db_thread::instance()->on_update_online(id, n);
                    db_thread::instance()->on_update_robot();
                    start_update_timer();
                    card_mission::instance()->init();
                    config_manager::instance()->load_shuffle();
                    //LOG_DEBUG("onlines: %d(%d)", n, id);
                }
                break;
            case 1:
                ms_inst->connect();
                break;
            case 2:
                ms_inst->send_ping();
                timer_inst->set_timer(TID1_PLATFORM, 2, 0, 0, 10, 0);
                break;
            }
        }
        break;
    case TID1_ROOM:
        if(GAME_MODE_CFG == GAME_TYPE_MATCH)
        {
            int roomid = id2;
            game_room* room =  match->get_room(roomid);
            if(room)
                room->on_room_timer(tid, data);
        }
        else
        {
            single_game_room::instance()->on_room_timer(tid, data);
        }
        break;
    case TID1_GAME:
        if(GAME_MODE_CFG == GAME_TYPE_MATCH)
        {
            int roomid = id2;
            game_room* room =  match->get_room(roomid);
            if(room)
                room->on_desk_timer(id3, tid, data);
        }
        else
        {
            single_game_room::instance()->on_desk_timer(id3, tid, data);
        }
        break;
    default:
        break;
    }
}

void game_main_thread::start_update_timer()
{
    timer_singleton::instance()->set_timer(TID1_PLATFORM, 0, 0, 0, config_manager::instance()->update_time(), 0);
}

void game_main_thread::send_login_result(socket_id sid, unsigned int uid, int result, int serverid)
{
    sc_login_result obj;
    obj.m_result = result;
    obj.m_userid = uid;
    obj.m_serverid = serverid;
    if(result != LOGIN_RESULT_OK)
    {
        switch(result)
        {
        case LOGIN_RESULT_SYS_BUSY:      //系统繁忙
            obj.m_msg = STRING_RES(4);
            break;
        case LOGIN_RESULT_IN_OTHER_ROOM: //在其他房间
            obj.m_msg = STRING_RES(7);
            break;
        case LOGIN_RESULT_ACCOUNT_IN_USE://帐号正在使用
            obj.m_msg = STRING_RES(2);
            break;
        case LOGIN_RESULT_PAUSE:         //系统暂停服务
            obj.m_msg = STRING_RES(8);
            break;
        case LOGIN_RESULT_LESS_CONDITION://条件不足
            obj.m_msg = STRING_RES(9);
            break;
        case LOGIN_RESULT_LESS_COIN:     //游戏币不足
            obj.m_msg = STRING_RES(6);
            break;
        case LOGIN_RESULT_OPENID_FAIL:   //令牌错误
            obj.m_msg = STRING_RES(10);
            break;
        case LOGIN_RESULT_DISENABLE:     //帐号封
            obj.m_msg = STRING_RES(5);
            break;
        case LOGIN_RESULT_MORE_COIN:
            obj.m_msg = STRING_RES(11); //金币过多
            break;
        case LOGIN_RESULT_NEED_VIP:
            obj.m_msg = STRING_RES(19); //VIP身份要求
            break;
        case LOGIN_RESULT_MATCH_END:
            obj.m_msg = STRING_RES(21);
        }
    }
    socket_service::instance()->send_obj(sid, &obj);
    if(result == LOGIN_RESULT_OK)
        send_game_config(sid);
}

void game_main_thread::on_login(socket_id sid, char* openid, int len)
{
    if(GAME_MODE_CFG == GAME_TYPE_MATCH)
    {
        LOG_WARNING("match server has recive normal login");
        return;
    }
    //LOG_DEBUG("on_login:%s", openid);
    unsigned int uid;
    int t;
    if(!decrypt_token2(openid, len, &uid, &t))
    {
        send_login_result(sid, 0, LOGIN_RESULT_OPENID_FAIL);
        return;
    }
    LOG_DEBUG("on_login:%d", uid);
	game_room* room = get_room(sid);
	if(!room)
	{
		return;
	}
	room->on_login(sid, uid);
}

void game_main_thread::on_relogin(socket_id sid, room_user* p_user)
{
    //相同链接重复发送登录命令
    if(sid == p_user->sid)
        return;
    /*
    send_login_result(sid, 0, LOGIN_RESULT_ACCOUNT_IN_USE);
    mn_cut_off obj1;
    obj1.m_id = LOGIN_RESULT_ACCOUNT_IN_USE;
    obj1.m_msg = STRING_RES(3);
    socket_service::instance()->send_obj(p_user->sid, &obj1);
    */
    //踢掉第一个
    mn_cut_off obj1;
    obj1.m_id = LOGIN_RESULT_ACCOUNT_IN_USE;
    obj1.m_msg = STRING_RES(3);
    socket_service::instance()->send_obj(p_user->sid, &obj1);
    do_client_close(p_user->sid);
    socket_service::instance()->close_client(p_user->sid);

    //第二个重入
}

void game_main_thread::on_db_login_fail(socket_id sid, unsigned int uid, db_login_fail_result result)
{
    send_login_result(sid, uid, result.result, result.serverid);
}

void game_main_thread::on_db_login_ok(socket_id sid, unsigned int uid, room_user& userinfo)
{
	game_room* room = get_room(sid);
    if(!socket_service::instance()->is_online(sid) ||
		!room)
    {
        LOG_DEBUG("登录成功，用户已经掉线");
        db_thread::instance()->on_logout(uid);
        return;
    }
	room->on_db_login_ok(sid, uid, userinfo);
}

void game_main_thread::send_game_config(socket_id sid)
{
    sc_game_config obj;
    ddz_game_config& m_gameconfig = config_manager::instance()->get_game_config();

	obj.m_readytime = m_gameconfig.ready_time;
	obj.m_autoready = m_gameconfig.auto_ready;
    obj.m_calltime = m_gameconfig.call_time;
    obj.m_outtime = m_gameconfig.out_time;
    obj.m_passtime = m_gameconfig.pass_time;
    obj.m_firstouttime = m_gameconfig.first_out_time;
    obj.m_gamemode = m_gameconfig.game_mode;
    obj.m_laizi = m_gameconfig.surport_lz;
    obj.m_basepoint = m_gameconfig.base_point;
    obj.m_jipaiqi = m_gameconfig.jipaiqi;
    obj.m_renshu = m_gameconfig.renshu==1?1:0;
    obj.m_anticheat = config_manager::instance()->anti_cheat()?1:0;
    
    socket_service::instance()->send_obj(sid, &obj);
}

void game_main_thread::send_count_box(socket_id sid, int level, int seconds)
{
	sc_count_cfg_result obj;
	obj.m_seconds = seconds;
	count_box_item* olm = config_manager::instance()->get_online_mission(level);
	if(!olm)
    {
        LOG_ERROR("count_box_item IS NULL:level=%d", level);
        return;
    }
	if(seconds > olm->t)
		seconds = olm->t;
	obj.m_coin = olm->coin;
	obj.m_pid = olm->pid;
	obj.m_pn = olm->pn;
	obj.m_seconds = seconds;
	obj.m_time_len = olm->t;
	socket_service::instance()->send_obj(sid,&obj);
}

void game_main_thread::on_count_box(socket_id sid, unsigned int uid, int seconds, int level)
{
    MAKE_TYPE(d, T_LOAD_COUNT_BOX, sid);
    t_count_box_info info;
    info.uid = uid;
    info.seconds = seconds;
    info.level = level;
    put_data(&d, sizeof(d), &info, sizeof(info));
}

void game_main_thread::do_count_box(socket_id sid, unsigned int uid, int seconds, int level)
{
    if(GAME_MODE_CFG==GAME_TYPE_PK)
        return;
	game_room* room = get_room(sid);
	if(!room) return;
    room_user* user = room->get_userinfo(sid, uid);
    if(!user)
        return;
    user->count_box_timestamp = _time32(0);
    user->count_box.seconds = seconds;
    user->count_box.level = level;
    send_count_box(sid, level, seconds);
}

void game_main_thread::on_ms_connect()
{
    MAKE_TYPE(d, T_MS_CONNECT, INVALID_SOCKET_ID);
    put_data(&d, sizeof(d), 0, 0);
}

void game_main_thread::on_ms_recive(char* data, int len)
{
    MAKE_TYPE(d, T_MS_RECIVE, INVALID_SOCKET_ID);
    put_data(&d, sizeof(d), data, len);
}

void game_main_thread::on_ms_close()
{
    MAKE_TYPE(d, T_MS_CLOSE, INVALID_SOCKET_ID);
    put_data(&d, sizeof(d), 0, 0);
}

void game_main_thread::on_load_match(int sn, list<room_user>* l)
{
    MAKE_TYPE(d, T_DB_LOAD_MATCH_USER, INVALID_SOCKET_ID);
    t_load_db_match_user t;
    t.sn = sn;
    t.l = l;
    put_data(&d, sizeof(d), &t, sizeof(t));
}

void game_main_thread::do_ms_connect()
{
    ms_inst->send_reg();
    timer_inst->set_timer(TID1_PLATFORM, 2, 0, 0, 10, 0);
}

void game_main_thread::do_ms_close()
{
    timer_inst->kill_timer(TID1_PLATFORM, 2, 0, 0);
    timer_inst->set_timer(TID1_PLATFORM, 1, 0, 0, 2, 0);
    LOG_WARNING("lost ms connect, try reconnect.");
}

void game_main_thread::do_ms_recive(char* data, int len)
{
    char buf[102400];
    msg_object_base* obj = msprotObjectManager::CreateObject(data, len, buf);
    if(!obj || obj->major_id() != MJ_MATCH_SERVER)
    {
        msg_object_head* head = (msg_object_head*) data;
        LOG_ERROR("ms unknow protocol  mid=%d, nid=%d", head->m_major_id, head->m_minor_id);
        return;
    }
    switch (obj->minor_id())
    {
    case MN_MS_MATCH_BEGIN:
        {
            CAST_OBJ(ms_match_begin)
            db_thread::instance()->load_match_user(o->m_sn);
        }
        break;
    case MN_MS_REG_RESPONSE:
        {
            int sns[1000];
            int size = match->get_matching_room_sn(sns, 1000);
            ms_inst->send_syn_match(sns, size);
        }
        break;
    default:
        break;
    }
}

void game_main_thread::do_load_match(int sn, list<room_user>* l)
{
    if(!l)
    {
        LOG_ERROR("match load error sn=%d", sn);
        ms_inst->send_load_result(sn, 0);
        return;
    }
    game_room* room = match->get_empty_room(sn);
    if(!room)
    {
        LOG_ERROR("match load has no empty room sn=%d", sn);
        ms_inst->send_load_result(sn, 0);
    }
    else
    {
        room->init_match(l);
    }
    delete l;
}

void game_main_thread::on_login_match(socket_id sid, char* openid, int len, int sn)
{
    unsigned int uid;
    int t;
    if(!decrypt_token2(openid, len, &uid, &t))
    {
        send_login_result(sid, 0, LOGIN_RESULT_OPENID_FAIL);
        return;
    }
    game_room* room = match->get_matching_room(sn);
    if(!room)
    {
        LOG_DEBUG("登录比赛不存在2 sn=%d", sn);
        send_login_result(sid, 0, LOGIN_RESULT_MATCH_END);
        return;
    }
	m_user_room[sid.index] = room->get_roomid();

    match_user_item * muser = room->get_match_user(uid);
    if(!muser)
    {
        LOG_DEBUG("比赛用户已经淘汰: sn=%d  uid=%d", sn, uid);
        send_login_result(sid, 0, LOGIN_RESULT_MATCH_END);
        ms_inst->send_user_end(room->match_id(), uid, -1);
        return;
    }
    
    room_user* user = room->get_userinfo(sid);
    if(user)
    {
        //相同链接重复登录
        if(user->sid == sid)
		{
			LOG_DEBUG("user->sid == sid");
            return;
		}
        socket_id t_sid = user->sid;
        socket_service::instance()->close_client(t_sid);
        do_client_close(t_sid);
        user = room->is_net_cut(sid, uid);
    }
    else
    {
        user = room->is_net_cut(sid, uid);
    }
    if(!user)
    {
		LOG_WARNING("%s %d", __FILE__, __LINE__);
        ms_inst->send_user_end(room->match_id(), uid, -1);
        send_login_result(sid, 0, LOGIN_RESULT_MATCH_END);
        return;
    }
    send_login_result(sid, uid, LOGIN_RESULT_OK);

    room->on_match_user_login_ok(uid, sid);
}

void game_main_thread::on_card_mission_index(unsigned int uid, int index)
{
    MAKE_TYPE(d, T_CARD_MISSION_INDEX, INVALID_SOCKET_ID);
    t_card_mission_index item;
    item.uid = uid;
    item.index = index;
    put_data(&d, sizeof(d), &item, sizeof(item));
}

void game_main_thread::do_card_mission_index(unsigned int uid, int index)
{
    room_user* user = single_game_room::instance()->is_online_user(uid);
    if(!user)
        return;
    user->card_mission_index = index;
}
