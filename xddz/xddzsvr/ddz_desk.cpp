#include "ddz_desk.h"
#include "mddz.h"
#include "ddz_def.h"
#include "ddzprot.h"
#include "cflog.h"
#include "config_manager.h"
#include "db_thread.h"
#include "socket_service.h"
#include "user_manager.h"
#include "myrandom.h"
#include "redis_thread.h"
#include "string_res.h"
#include "mddz_util.h"
#include "mddzc.h"
#include "ddz_ai_strategies.h"
#include "log_type.h"
#include "ai.h"
#include "convert.h"

#define NEXT_OPERATOR() m_operator ++; if(m_operator >= DDZ_SEATS) m_operator = 0
#define MAX_CODE_LEN  4

ddz_desk::ddz_desk(void)
{
    set_desk_state(DS_GAME_END);
    for(int i=0; i<DDZ_SEATS; i++)
    {
        m_seat[i].is_auto = false;
        m_seat[i].sid = INVALID_SOCKET_ID;
        m_seat[i].state = PLAYER_STATE_WAIT;
        m_seat[i].uid = 0;
        m_seat[i].is_robot = false;
        m_mission_index[i] = 0;
    }
}

ddz_desk::~ddz_desk(void)
{
}

void ddz_desk::shuffle()
{
    if(cfg_inst->is_enable_shuffle())
    {
        shuffle2();
    }
    else
    {
        shuffle1();
    }
}

void ddz_desk::shuffle1()
{
    char hand_cards[3][17];

    m_lz = ddz_shuffle(hand_cards, m_back);
    if (config_manager::instance()->get_game_config().surport_lz == 0)
    {
        m_lz = 0;
    }
    for(int i=0; i<DDZ_SEATS; i++)
    {
        memcpy(m_hand[i].cards, hand_cards[i], 17);
        m_hand[i].len = 17;

        change_12_2_A2(m_hand[i].cards, m_hand[i].len);
        sort_by_num(m_hand[i].cards, m_hand[i].len);
        change_A2_2_12(m_hand[i].cards, m_hand[i].len);
    }
}

void ddz_desk::init(i_gamedesk_controler* controler, ddz_game_config gameconfig)
{
    m_controler = controler;
	m_gameconfig = gameconfig;
    set_desk_state(DS_GAME_END);
    clear_gameinfo();
    for(int i=0; i<DDZ_SEATS; i++)
    {
        m_seat[i].is_auto = false;
        m_seat[i].sid = INVALID_SOCKET_ID;
        m_seat[i].state = PLAYER_STATE_WAIT;
        m_seat[i].uid = 0;
        m_seat[i].is_robot = false;
    }
}

void ddz_desk::set_index(int roomid, int deskid)
{
    m_roomid = roomid;
    m_deskid = deskid;
}

int ddz_desk::get_deskid()
{
    return m_deskid;
}

void ddz_desk::kill_all_timer()
{
    kill_timer(TID_CALL_POINT);
    kill_timer(TID_CALL_DOUBLE_FARMER);
    kill_timer(TID_CALL_DOUBLE_LANDER);
    kill_timer(TID_OUT_CARD);
    kill_timer(TID_READY1);
    kill_timer(TID_READY2);
    kill_timer(TID_READY3);
    kill_timer(TID_DESK_START_MATCH);
}

void ddz_desk::on_timer(unsigned short tid, int data)
{
	int readyid = 0;
	switch(tid)
	{
	case TID_CALL_POINT:          //叫分
        if(m_seat[data].is_robot)
            on_robot_call(data);
        else
            on_call_point(data, 0);
		break;
	case TID_CALL_DOUBLE_FARMER:  //农民加倍
        on_call_double(data, 0);
		break;
	case TID_CALL_DOUBLE_LANDER:  //地主加倍
        on_call_double(m_loader, 0);
		break;
	case TID_OUT_CARD:            //出牌
        if(m_seat[data].is_robot || m_seat[data].is_auto)
        {
            robot_out_card(data);
        }
        else
        {
            on_outcards(data, 0, 0, 0, true);
        }
		break;
	case TID_READY1:
	case TID_READY2:
	case TID_READY3:
		{
			int seatid = tid - TID_READY1;
			on_ready_timer(seatid);
		}
        break;
    case TID_DESK_START_MATCH:
        on_game_start();
        break;
	case TID_MATCH_ROBOT_WAIT:
		on_game_end(data);
		break;
	default:
		break;
	}
}

void ddz_desk::on_recive(socket_id sid, msg_object_base* msg)
{
	LOG_DEBUG("ddz_desk::on_recive mid=%d nid=%d", msg->major_id(), msg->minor_id());
    switch(msg->major_id())
    {
	case MJ_MESSAGE_DDZ_GAME:
    case MJ_GAME_LOGIC_DDZ:
        do_game_msg(sid, msg);
        break;
	case MJ_PK_GAME:
		do_pkroom_msg(sid,msg);
		break;
    default:
        break;
    }
}

void ddz_desk::do_game_msg(socket_id sid, msg_object_base* msg)
{
    int seatid = get_seatid(sid);
    if(seatid < 0)
    {
        LOG_WARNING("%s , %d", __FILE__, __LINE__);
        socket_service::instance()->close_client(sid);
        return;
    }
    switch(msg->minor_id())
    {
    //服务器下发case MN_GAME_CONFIG: //游戏桌配置信息
    //服务器下发case MN_DEAL: //发牌
    //服务器下发case MN_CALL_POINT: //通知用户叫分,如果operator与lastoperator相同,标示为第一个叫分
    case MN_USER_CALL_POINT: //用户叫分
        {
            cs_call_point* obj = (cs_call_point*)msg;
            on_call_point(seatid, obj->m_point);
        }
        break;
    //服务器下发case MN_LAND_RESULT: //确认地主
    //服务器下发case MN_LAND_BACK_CARDS: //确认地主
    //服务器下发case MN_GAME_CALL_DOUBLE: //通知用户加倍
    case MN_GAME_DOUBLE_RESULT: //加倍结果,客户端服务器共用协议
        {
            double_result* obj = (double_result*)msg;
            on_call_double(seatid, obj->m_value);
        }
        break;
    //服务器下发case MN_GAME_OUT_CARD_RESULT: //通知出牌
    case MN_GAME_OUT_CARD: //玩家出牌
        {
            cs_out_cards* obj = (cs_out_cards*)msg;
            on_outcards(seatid, (char*)obj->m_cards.m_data, obj->m_cards.m_len, obj->m_cardtype, false);
        }
        break;
    //服务器下发case MN_GAME_OUT_CARD_ERR: //出牌错误提示
    //服务器下发case MN_GAME_END: //游戏结束
    //服务器下发case MN_RECOME_STATE: //断线重连状态
    //服务器下发case MN_RECOME_STATE_CALL_POINT: //叫分阶段重连场景信息
    //服务器下发case MN_RECOME_STATE_CALL_DOUBLE: //加倍阶段断线重连场景信息
    //服务器下发case MN_RECOME_STATE_OUTCARDS: //出牌阶段断线重连场景信息
    case MN_USER_AUTO_STATE: //修改托管状态,服务器客户端共用
        {
            on_auto(seatid);
            break;
        }
    //服务器下发case MN_LZ_CARD: //下发癞子牌型
    case MN_CALL_MINGPAI:
        {
            on_call_mingpai(seatid);
        }
		break;
    //服务器下发case MN_MINGPAI_CARD: //下发用户明牌
	case MN_CHAT_MESSAGE:
		{
            if(config_manager::instance()->anti_cheat())
                break;
			sc_chat_message* obj = (sc_chat_message*)msg;
			obj->m_userid = m_seat[seatid].uid;
			send_all(obj);
		}
		break;
    default:
        break;
    }
}

void ddz_desk::on_auto(int seatid)
{
    if(m_desk_state == DS_GAME_END)
        return;
    m_seat[seatid].is_auto = !m_seat[seatid].is_auto;
    send_auto_tag(seatid, m_seat[seatid].is_auto?1:0);

    if(m_seat[seatid].is_auto && seatid == m_operator)
    {
        switch (m_desk_state)
        {
        case DS_GAME_END:
            return;
        case DS_CALL_POINT:
            set_timer(TID_CALL_POINT, 0, seatid);
            break;
        case DS_CALL_DOUBLE_FARMER:
            set_timer(TID_CALL_DOUBLE_FARMER, 0, seatid);
            break;
        case DS_CALL_DOUBLE_LANDER:
            set_timer(TID_CALL_DOUBLE_LANDER, 0, seatid);
            break;
        case DS_GAMING:
            set_timer(TID_OUT_CARD, 0, seatid);
            break;
        default:
            break;
        }
    }
}

void ddz_desk::do_pkroom_msg(socket_id sid, msg_object_base* msg)
{
	int seatid = get_seatid(sid);
	if(seatid < 0 || seatid > 1)
	{
		LOG_WARNING("%s , %d", __FILE__, __LINE__);
		socket_service::instance()->close_client(sid);
		return;
	}
	switch(msg->minor_id())
	{
	case MN_SET_PK_POINT:
		{
			//判断是否是房主，并且设置额度不能大于房间玩家最小额度
            if (seatid != 0)
            {
                LOG_WARNING("MN_SET_PK_POINT error");
                break;
            }
            sc_set_pk_point* obj = (sc_set_pk_point*)msg;
            on_set_pk_point(sid, obj->m_point);
		}
		break;
	case MN_AGREE_SET_PK_POINT:
		{
			//确认pk分数，协议下发给房主
			//send sc_agree_pk_point
			sc_agree_pk_point* obj = (sc_agree_pk_point*)msg;
            on_agree_pk_point(sid, obj->m_type==1);
		}
		break;
	case MN_GIVEUP:
		{
            int win = seatid == 1? 0:1;
			on_game_end(win,GAMEEND_GIVEUP);
		}
		break;
	default:
		break;
	}
	return;
}

void ddz_desk::on_sitdown(socket_id sid, int seatid)
{
	//游戏中不能坐下
	if(m_desk_state != DS_GAME_END)
	{
		send_sitdown_err(sid, GAMEING_CANNOT_SIT);
		return;
	}
	//指定座位非空
	if (seatid>=0 && seatid<DDZ_SEATS)
	{
		if(m_seat[seatid].uid != 0)
		{
			send_sitdown_err(sid, SEAT_NOT_EMPTY);
			return;
		}
	}
	//用户信息非法
	room_user* user = 0;
    user = m_controler->get_userinfo(sid);
	if(!user || user->deskid != -1)
	{
		send_sitdown_err(sid, SITDOWN_ABNORMAL_ERR);
		return;
	}
	//自动分配座位检测是否座位为空
	if(seatid<0 || seatid>=DDZ_SEATS)
	{
		for(int i=0; i<DDZ_SEATS; i++)
		{
			if(m_seat[i].uid == 0)
			{
				seatid = i;
				break;
			}
		}
	}
	if(seatid<0 || seatid>=DDZ_SEATS)
	{
		send_sitdown_err(sid, SEAT_NOT_EMPTY);
		return;
	}
	set_user_sitdown(sid, seatid, user);
}

void ddz_desk::set_match_user(socket_id sids[3], unsigned int uids[3], bool robot[3])
{
    for(int i=0; i<3; i++)
    {
        m_seat[i].sid = sids[i];
        m_seat[i].uid = uids[i];
        m_seat[i].state = PLAYER_STATE_READY;
        m_seat[i].is_robot = robot[i];

        if(sids[i] == INVALID_SOCKET_ID)
            m_seat[i].is_auto = true;
        else
            m_seat[i].is_auto = false;
        room_user* user = m_controler->get_userinfo(sids[i], uids[i]);
        if(!user)  user = m_controler->get_netcut_userinfo(uids[i]);
        if(user)
        {
            user->deskid = m_deskid;
            user->seatid = i;
        }
    }
    for (int i=0; i<3; i++)
    {
        send_user_sitdown(i);
    }
    on_game_start();
}

void ddz_desk::clear_match_user()
{
    clear_gameinfo();
    for(int i=0; i<DDZ_SEATS; i++)
    {
        m_seat[i].uid = 0;
        m_seat[i].sid = INVALID_SOCKET_ID;
    }
}

void ddz_desk::set_user_sitdown(socket_id sid, int seatid, room_user* user)
{
	m_seat[seatid].sid = sid;
	m_seat[seatid].uid = user->uid;
	m_seat[seatid].state = PLAYER_STATE_WAIT;
    m_seat[seatid].is_robot = false;
	
	user->deskid = m_deskid;
	user->roomid = 0;
	user->seatid = seatid;

	send_user_sitdown(seatid);
    if(!IS_PK_MODE)
    {
        if(m_gameconfig.auto_ready)
        {
            on_ready(seatid);
        }
        else
        {
            set_timer(TID_READY1+seatid, m_gameconfig.ready_time+MAX_DELAY, 0);
        }
    }
}

bool ddz_desk::set_robot_sitdown(room_user* user)
{
    int seatid = -1;
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(m_seat[i].uid == 0)
        {
            seatid = i;
            break;
        }
    }
    if(seatid < 0)
        return false;
    m_seat[seatid].sid = INVALID_SOCKET_ID;
    m_seat[seatid].uid = user->uid;
    m_seat[seatid].state = PLAYER_STATE_WAIT;
    m_seat[seatid].is_robot = true;
    m_seat[seatid].is_auto = false;

    user->deskid = m_deskid;
    user->roomid = 0;
    user->seatid = seatid;

    send_user_sitdown(seatid);

    on_ready(seatid);

    return true;
}

void ddz_desk::clear_robot()
{
	if(IS_MATCH_MODE)
		return;
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(!m_seat[i].is_robot || m_seat[i].uid == 0)
            continue;
        send_user_left(i, m_seat[i].uid);
        robot_inst->put(m_seat[i].uid);
        m_seat[i].uid = 0;
        m_seat[i].sid = INVALID_SOCKET_ID;
        m_seat[i].state = 0;
        m_seat[i].is_robot = false;
    }
}

void ddz_desk::set_user_left(int seatid)
{
    if(seatid<0 || seatid>=DDZ_SEATS || m_seat[seatid].uid == 0)
    {
        LOG_WARNING("%s %d, seatid=%d, uid=%d", __FILE__, __LINE__, seatid, (seatid>=0&&seatid<DDZ_SEATS)?m_seat[seatid].uid:0);
        return;
    }
    if(!m_seat[seatid].is_robot)
    {
        clear_robot();
    }
	send_user_left(seatid, m_seat[seatid].uid);
	room_user* user = user_info(seatid);
	if(user)
	{
		user->deskid = -1;
		user->seatid = -1;
	}
	
	m_seat[seatid].uid = 0;
	m_seat[seatid].sid = INVALID_SOCKET_ID;
	m_seat[seatid].state = 0;
    m_seat[seatid].is_robot = false;
}

void ddz_desk::set_loader(int seatid, int point)
{
    m_call_point = point;
    m_loader = seatid;
    for(int i=0; i<3; i++)
    {
        m_hand[seatid].cards[m_hand[seatid].len+i] = m_back[i];
    }

    m_hand[seatid].len += 3;
    sort_by_num(m_hand[seatid].cards, m_hand[seatid].len);

    send_set_loader();

    //根据配置控制流程
    switch(m_gameconfig.game_mode)
    {
    case GAME_TYPE_NORMAL:
    case GAME_TYPE_MATCH:
        {
            m_operator = seatid;
            set_desk_state(DS_CALL_DOUBLE_FARMER);
            NEXT_OPERATOR();
            send_call_double(m_operator);
            if(m_seat[m_operator].is_auto || m_seat[m_operator].sid == INVALID_SOCKET_ID)
                set_timer(TID_CALL_DOUBLE_FARMER, random_int(2, 4), m_operator);
            else
            {
                if(m_seat[m_operator].is_robot)
                    set_timer(TID_CALL_DOUBLE_FARMER, random_int(3, 6), m_operator);
                else
                    set_timer(TID_CALL_DOUBLE_FARMER, m_gameconfig.call_time+MAX_DELAY, m_operator);
            }
        }
        return;
    default:
        break;
    }
    set_desk_state(DS_GAMING);
    m_operator = seatid;
    send_back_cards();
    send_out_cards(m_operator, 0, 0, 0, 0, m_operator, 0, 0, 0);
    set_out_timer(m_operator);
}

void ddz_desk::set_out_timer(int seatid)
{
    if(m_seat[seatid].is_auto)
        set_timer(TID_OUT_CARD, 0, seatid);
    else
    {
        if(m_seat[seatid].is_robot)
            set_timer(TID_OUT_CARD, random_int(2, 5), seatid);
        else
        {
            if(m_seat[seatid].sid == INVALID_SOCKET_ID)
            {
                set_timer(TID_OUT_CARD, 3, seatid);
            }
            else
            {
                set_timer(TID_OUT_CARD, m_gameconfig.out_time + MAX_DELAY, seatid);
            }
        }
    }
}

void ddz_desk::on_socket_close(socket_id sid, int seatid)
{
    if(seatid<0 || seatid>=DDZ_SEATS)
    {
        LOG_WARNING("ddz_desk::on_socket_close seatid=%d", seatid);
        return;
    }
    m_seat[seatid].sid = INVALID_SOCKET_ID;
    m_seat[seatid].is_auto = true;
    send_user_net_cut(m_seat[seatid].uid, seatid);
}

void ddz_desk::on_user_recome(socket_id sid, unsigned int uid, int seatid)
{
    if(seatid<0 || seatid>=DDZ_SEATS)
    {
        LOG_WARNING("ddz_desk::on_user_recome:[%s:%d] %d", __FILE__, __LINE__, seatid);
        return;
    }
    if(m_seat[seatid].uid != uid)
    {
        LOG_WARNING("ddz_desk::on_user_recome:[%s:%d] %d %d", __FILE__, __LINE__, seatid, uid);
        return;
    }
    m_seat[seatid].sid = sid;
    m_seat[seatid].is_auto = false;
    send_user_recome(sid, uid, seatid);
    send_auto_tag(seatid, 0);
}

void ddz_desk::on_watcher_in(socket_id sid)
{
	list<socket_id>::iterator i;
	for(i=m_watcher.begin(); i!=m_watcher.end(); i++)
	{
		if((*i) == sid)
			return;
	}
	m_watcher.push_back(sid);
}

void ddz_desk::on_watcher_left(socket_id sid)
{
	list<socket_id>::iterator i;
	for (i=m_watcher.begin(); i!=m_watcher.end(); i++)
	{
		if((*i) == sid)
		{
			m_watcher.erase(i);
			return;
		}
	}
}

void ddz_desk::clear_watcher()
{
	m_watcher.clear();
}

void ddz_desk::on_standup(int seatid)
{
	if (seatid<0 || seatid>=DDZ_SEATS || m_desk_state != DS_GAME_END)
	{
		return;
	}
	set_user_left(seatid);
	kill_timer(TID_READY1+seatid);
}

void ddz_desk::on_ready_timer(int seatid)
{
    if(m_desk_state != DS_GAME_END)
        return;
	if(m_seat[seatid].state == PLAYER_STATE_READY)
		return;
	socket_id sid = m_seat[seatid].sid;

	socket_service::instance()->close_client(sid);
}

void ddz_desk::on_ready(int seatid)
{
	if(m_desk_state != DS_GAME_END)
		return;
	if(0>seatid || seatid >= DDZ_SEATS)
		return;
    if(m_seat[seatid].uid == 0)
        return;
	if(PLAYER_STATE_WAIT != m_seat[seatid].state)
		return;
	m_seat[seatid].state = PLAYER_STATE_READY;

	kill_timer(TID_READY1 + seatid);

	send_ready(seatid);
	check_begin();
}

void ddz_desk::on_outcards(int seatid, char* buf, int len, int type, int istimer)
{
	if(m_desk_state != DS_GAMING || seatid != m_operator)
	{
		send_outcard_error(seatid, CARD_ERR_INVALID_OPERATOR);
        return;
	}
    if(IS_MATCH_MODE && is_all_ai())
    {
		set_timer(TID_MATCH_ROBOT_WAIT, random_int(60, 80), random_int(0,2));
		m_bomb_count = random_int(0, 2);
        return;
    }
    if(!m_seat[seatid].is_robot)
        m_ai_mode_debug = 0;
    int cards_type = MDDZ_CT_ERROR;
    bool is_pass = (len == 0);
    bool is_first = is_first_out(seatid);


    //如果首发牌玩家弃牌，则强制出最小的一张
    if(is_pass && is_first)
    {
        sort_cards2(m_hand[seatid].cards, m_hand[seatid].len, m_lz);
        char t = m_hand[seatid].cards[m_hand[seatid].len-1];
        on_outcards(seatid, &t, 1, MDDZ_CT_ONE, true);
        return;
    }

    cards_buf buf_type[MAX_CANDIDATE];
    memset(buf_type, 0, sizeof(buf_type));
    int  tsize = 0;
    bool err = true;
    int  ttype_index = -1;
    do 
    {
        if(is_pass)
        {
            err = false;
            break;
        }
        //检查是否为手牌
        if(!check_exist_cards(m_hand[seatid].cards, m_hand[seatid].len, buf, len))
        {
            LOG_WARNING("check_exist_cards FAIL");
            break;
        }
        //检查是否为合法牌型
        //tsize = ddz_get_cards_type_list(buf, len, m_lz, maxcard, outcard, ttype, out);
        //int ddz_get_cards_type_list(cards_buf& src, int lz, cards_buf buf[MAX_CANDIDATE])
        cards_buf src(buf, len);
        tsize = ddz_get_cards_type_list2(src, m_lz, buf_type);
        if(tsize == 0)
        {
            LOG_ERR_LINE();
            break;
        }
        for(int i=0; i<tsize; i++)
        {
            if(buf_type[i].card_type == type)
            {
                ttype_index = i;
                break;
            }
        }
        if(ttype_index < 0)
        {
            char dbuf[1024];
            int n = len;

            if(n > 60) n = 60;
            int flen = sprintf(dbuf, "2out type error: len=%d mode=%d, type=%d cards=", len, m_ai_mode_debug, type);
            for(int i=0; i<n; i++)
            {
                int fl = sprintf(&dbuf[flen], "%02x,", buf[i]);
                flen += fl;
                if(flen > 500)
                    break;
            }
            dbuf[flen] = 0;
            LOG_ERROR(dbuf);
            
            break;
        }
        if(is_first)
        {
            err = false;
            break;
        }
        if(!compare_out_cards(seatid, buf, len, buf_type[ttype_index].card_type))
        {
            LOG_WARNING("out cards less than before");
            break;
        }
        err = false;
    } while (false);
    if(err)
    {
        LOG_WARNING("out cards error");
        if(m_seat[seatid].is_robot)
            on_root_out_error(seatid);
        else
            send_outcard_error(seatid, CARD_ERR_INVALID_CARDS);
        return;
    }

    if(!istimer)
        kill_timer(TID_OUT_CARD);

    if(!is_pass)
    {
        m_hand[seatid].len = out_card(m_hand[seatid].cards, m_hand[seatid].len, buf, len);
        int index = m_out_record.size;
        if (index < 0)
        {
            index = 0;
        }
        memcpy(m_out_record.out_cards[index].cards, buf, len);
        m_out_record.out_cards[index].seatid = seatid;
        m_out_record.out_cards[index].len = len;
        m_out_record.out_cards[index].cardtype = type;
        m_out_record.out_cards[index].nlz = buf_type[ttype_index].clz.size;
        for(int i=0; i<buf_type[ttype_index].clz.size; i++)
        {
            m_out_record.out_cards[index].slz[i] = buf_type[ttype_index].clz.lz_card[i].card;
            m_out_record.out_cards[index].dlz[i] = buf_type[ttype_index].clz.lz_card[i].change;
        }
        index ++;
        m_out_record.size = index;
        m_out_counter[seatid] ++;
        //炸弹计数器
        switch(buf_type[ttype_index].card_type)
        {
        case MDDZ_CT_BOMB:
        case MDDZ_CT_ROCKET:
            m_bomb_count ++;
            m_user_bomb[seatid] ++;
            break;
        default:
            break;
        }
    }

    if(IS_PK_MODE)
    {
        m_operator ++; 
        if(m_operator >= 2)
            m_operator = 0;
    }
    else
    {
        NEXT_OPERATOR();
    }

    //客户端收到两个连续的pass时，清空牌桌上的牌
    if(len == 0)
        send_out_cards(seatid, 0, 0, m_hand[seatid].len, 0, m_operator, 0, 0, istimer?1:0);
    else
        send_out_cards(seatid, buf, len, m_hand[seatid].len, type, m_operator,
        buf_type[ttype_index].clz.size, buf_type[ttype_index].clz.lz_card, istimer?1:0);
    if(!is_pass)
    {
        if(check_game_end(seatid))
        {
            on_game_end(seatid);
            return;
        }
    }
    set_out_timer(m_operator);
}


bool ddz_desk::is_first_out(int seatid)
{
    if(m_out_record.size <= 0)
        return true;
    if(m_out_record.out_cards[m_out_record.size-1].seatid == seatid)
        return true;
    return false;
}

bool ddz_desk::compare_out_cards(int seatid, char* cards, int len, int cardtype)
{
    //与上家比较牌大小
    //首发牌直接返回true
    //如果座位ID与上家ID相同，则按首发牌处理
    if(m_out_record.size <= 0)
        return true;
    int last_index = m_out_record.size - 1;
    if(m_out_record.out_cards[last_index].seatid == seatid)
        return true;
    out_cards_item& oc = m_out_record.out_cards[last_index];

    //bool ddz_compare(cards_buf& c1, cards_buf& c2, int lz)
    
    cards_buf c1(oc.cards, oc.len, oc.cardtype);
    cards_buf c2(cards, len, cardtype);
    

    bool ret =  ddz_compare2(c1, c2, m_lz);

    if(!ret)
    {
        char buf[1024];
        int flen = sprintf(buf, "mode=%d \nlen1=%d c1:", c1.size, m_ai_mode_debug);
        int n = c1.size;
        if(n > 60) n = 60;

        for(int i=0; i<n; i++)
        {
            int fl= sprintf(&buf[flen], "%02x ", c1.cards[i]);
            flen += fl;
            if(fl > 500)
                break;
        }
        buf[flen] = 0;
        LOG_ERROR(buf);


        flen = sprintf(buf, "len2=%d c2:", c2.size);
        n = c2.size;
        if(n > 60) n = 60;
        for(int i=0; i<n; i++)
        {
            int fl = sprintf(&buf[flen], "%02x ", c2.cards[i]);
            flen += fl;
            if(fl > 500)
                break;
        }
        buf[flen] = 0;
        LOG_ERROR(buf);
    }

    return ret;
}

void ddz_desk::on_robot_call(int seatid)
{
    cards_buf hand_card[3];
    for(int i=0; i<3; i++)
    {
        memcpy(hand_card[i].cards, m_hand[i].cards, 17);
        hand_card[i].size = 17;
    }
    int value = ddzai_evaluate_call_point(hand_card, seatid, m_back);

    int call = 0;
    if(value >= 14) call = 3;
    else if(value >= 9) call = 2;
    else if(value >= 7) call = 1;
    else call = 0;

    if(call <= m_last_call_point)
        call = 0;
    on_call_point(seatid, call);
}

void ddz_desk::on_call_point(int seatid, int point)
{
	if (m_desk_state != DS_CALL_POINT || seatid != m_operator)
	{
        LOG_WARNING("%s %d desk_state=%d, seatid=%d, m_operator=%d", __FILE__, __LINE__, m_desk_state, seatid, m_operator);
		return;
	}
    NEXT_OPERATOR();
    send_call_point_result(seatid, point);

    kill_timer(TID_CALL_POINT);
	if(point == 0)
	{
		m_call[seatid] = 0;
		//最后一个没叫地主
		if(m_operator == m_first_call)
		{
			if(m_last_call_point == 0)
			{
				//无人叫分, 默认第一个玩家为地主, 叫1分（机器人不叫地主）
                int land = m_first_call;
                //int n = 0;
                //while(m_seat[land].is_robot)
                //{
                //    land ++;
                //    if(land>2)land=0;
                //    n++;
                //    if(n>3)break;
                //}

				set_loader(land, 1);
			}
			else
			{
				int max_call = 0;
				int max_point = 0;
				for(int i=0; i<3; i++)
				{
					if(m_call[i] > max_point)
					{
						max_call = i;
						max_point = m_call[i];
					}
				}
				set_loader(max_call, max_point);
			}
		}
		else
		{
            send_set_call_point(m_operator);
			if(m_seat[m_operator].is_auto)
            {
                set_timer(TID_CALL_POINT, 0, m_operator);
            }
            else if(m_seat[m_operator].sid == INVALID_SOCKET_ID)
            {
				set_timer(TID_CALL_POINT, random_int(2, 4), m_operator);
            }
			else
            {
                if(m_seat[m_operator].is_robot)
                    set_timer(TID_CALL_POINT, random_int(3, 6), m_operator);
                else
				    set_timer(TID_CALL_POINT, m_gameconfig.call_time + MAX_DELAY, m_operator);
            }
		}
	}
	else if(point > 0 && point < 3)
	{
		if(point <= m_last_call_point)
			return;
		m_call[seatid] = point;
		m_last_call_point = point;

        if(m_operator == m_first_call)
		{
			set_loader(seatid, point);
		}
		else
		{
            send_set_call_point(m_operator);
            if(m_seat[m_operator].is_auto)
            {
                set_timer(TID_CALL_POINT, 0, m_operator);
            }
            else if(m_seat[m_operator].sid == INVALID_SOCKET_ID)
            {
                set_timer(TID_CALL_POINT, random_int(2, 4), m_operator);
            }
            else
            {
                if(m_seat[m_operator].is_robot)
                    set_timer(TID_CALL_POINT, random_int(3, 6), m_operator);
                else
                    set_timer(TID_CALL_POINT, m_gameconfig.call_time + MAX_DELAY, m_operator);
            }
		}
	}
	else if(point == 3)
	{
		set_loader(seatid, point);
	}
}

void ddz_desk::on_call_double(int seatid, int point)
{
    if(m_operator != seatid && (m_desk_state != DS_CALL_DOUBLE_LANDER ||  m_desk_state !=  DS_CALL_DOUBLE_FARMER))
        return;

    //目前只支持加倍或不加倍两个状态, 不支持加多倍
    point = (point==1?1:0);
    //农民加倍处理
    if (seatid != m_loader)
    {
        kill_timer(TID_CALL_DOUBLE_FARMER);
        if(m_call_double[seatid] == -1)
        {
            m_call_double[seatid] = point;
            send_call_double_result(seatid, point);
        }
        NEXT_OPERATOR();
        if(m_operator != m_loader)
        {
            send_call_double(m_operator);
            if(m_seat[m_operator].is_auto)
            {
                set_timer(TID_CALL_DOUBLE_FARMER, 0, m_operator);
            }
            else if(m_seat[m_operator].sid == INVALID_SOCKET_ID)
            {
                set_timer(TID_CALL_DOUBLE_FARMER, 3, m_operator);
            }
            else
            {
                if(m_seat[m_operator].is_robot)
                    set_timer(TID_CALL_DOUBLE_FARMER, random_int(3, 6), m_operator);
                else
                    set_timer(TID_CALL_DOUBLE_FARMER, m_gameconfig.call_time+MAX_DELAY, m_operator);
            }
            return;
        }
        int farmer_calldouble = 0;
        for(int i=0; i<DDZ_SEATS; i++)
        {
            if(i!=m_loader && m_call_double[i]>0)
            {
                farmer_calldouble ++;
            }
        }
        //如果农民有叫加倍，地主可以加倍，否则直接开牌
        if(farmer_calldouble > 0)
        {
            set_desk_state(DS_CALL_DOUBLE_LANDER);
            send_call_double(m_loader);
            if(m_seat[m_loader].is_auto)
            {
                set_timer(TID_CALL_DOUBLE_LANDER, 0, m_loader);
            }
            else if(m_seat[m_loader].sid == INVALID_SOCKET_ID)
            {
                set_timer(TID_CALL_DOUBLE_LANDER, 3, m_loader);
            }
            else
            {
                if(m_seat[m_operator].is_robot)
                    set_timer(TID_CALL_DOUBLE_LANDER, random_int(3, 6), m_loader);
                else
                    set_timer(TID_CALL_DOUBLE_LANDER, m_gameconfig.call_time+MAX_DELAY, m_loader);
            }
        }
        else
        {
            set_desk_state(DS_GAMING);
            m_operator = m_loader;
            send_back_cards();
            send_out_cards(m_operator, 0, 0, 0, 0, m_operator, 0, 0, 0);
            set_out_timer(m_operator);
        }
    }
    else
    {
		kill_timer(TID_CALL_DOUBLE_LANDER);
        //地主加倍处理
        if(m_call_double[seatid] == -1)
        {
            m_call_double[seatid] = point;
            send_call_double_result(seatid, point);
        }
        set_desk_state(DS_GAMING);
        m_operator = m_loader;
        send_back_cards();
        send_out_cards(m_operator, 0, 0, 0, 0, m_operator, 0, 0, 0);
        set_out_timer(m_operator);
    }
}

void ddz_desk::on_call_mingpai(int seatid)
{
    if(// m_gameconfig.game_mode != GAME_MODE_HUANLE || 
        m_desk_state == DS_GAMING ||
        m_desk_state == DS_GAME_END ||
        seatid <0 || seatid>=DDZ_SEATS ||
        m_mingpai[seatid])
        return;
    m_mingpai[seatid] = true;
    send_mingpai(seatid);
}

int ddz_desk::get_seatid(socket_id sid)
{
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(sid == m_seat[i].sid)
        {
            return i;
        }
    }
    return -1;
}

int ddz_desk::get_seatid(int uid)
{
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(uid == m_seat[i].uid)
        {
            return i;
        }
    }
    return -1;
}

void ddz_desk::on_game_start()
{
    clear_gameinfo();
    
    for (int i=0; i<DDZ_SEATS; i++)
    {
        m_seat[i].state = PLAYER_STATE_GAMING;
        m_seat[i].is_auto = false;
    }
	//洗牌
    shuffle();
	//设置叫分操作
	m_operator = rand() % DDZ_SEATS;
	m_first_call = m_operator;
	m_last_call_point = 0;

    if(IS_MATCH_MODE)
    {
        sc_match_base obj;
        obj.m_base = m_controler->get_base_point();
        send_all(&obj);
        send_rank_change();
    }

	//发送数据
	send_game_begin();

    set_desk_state(DS_CALL_POINT);

	//设置叫分定时器
	if(m_seat[m_operator].is_auto || m_seat[m_operator].sid == INVALID_SOCKET_ID)
		set_timer(TID_CALL_POINT, random_int(2, 4), m_operator);
	else
    {
        if(m_seat[m_operator].is_robot)
        {
            set_timer(TID_CALL_POINT, random_int(2, 5), m_operator);
        }
        else
        {
            set_timer(TID_CALL_POINT, m_gameconfig.call_time + MAX_DELAY, m_operator);
        }
    }
    gen_card_mission();
}

int ddz_desk::get_empty_size()
{
    if(m_desk_state != DS_GAME_END)
        return 0;
	int size = 0;
	for(int i=0; i<DDZ_SEATS; i++)
	{
		if(m_seat[i].uid == 0)
		{
			size ++;
		}
	}
	return size;
}

int ddz_desk::get_desk_state()
{
    return m_desk_state;
}

bool ddz_desk::is_gaming()
{
    if(IS_PK_MODE)
        return false;
    return (m_desk_state != DS_GAME_END);
}

void ddz_desk::user_left(socket_id sid, unsigned int uid)
{
    int seatid = get_seatid(uid);
    if(seatid<0)
        return;
    if(GAME_MODE_CFG != GAME_TYPE_PK)
        on_standup(seatid);
    else
    {
//        m_seat[seatid].sid = INVALID_SOCKET_ID;

        if(m_desk_state == DS_GAMING)
        {
            int win = seatid==0?1:0;
            on_game_end(win,GAMEEND_USER_LEFT);
        }
        else
        {
            on_dismiss();
        }
    }
}

void ddz_desk::set_desk_state(int state)
{
    m_desk_state = state;
}

void ddz_desk::clear_gameinfo()
{
    ///出牌信息
    memset(m_hand, 0, sizeof(m_hand));
    memset(m_back, 0, 3);
    memset(&m_out_record, 0, sizeof(m_out_record));
    m_last_bigger = -1;

    ///叫分信息
    m_first_call = -1;
    m_last_call_point = 0;
    m_loader = -1;
    for(int i=0; i<DDZ_SEATS; i++)
    {
        m_call_double[i] = -1;
        m_mingpai[i] = false;
        m_call[i] = -1;
        m_seat[i].state = PLAYER_STATE_WAIT;
        m_seat[i].is_auto = false;
        send_auto_tag(i, 0);
    }

    ///游戏统计
    m_call_point = 0;
    m_base_point = 0;
    m_bomb_count = 0;
    m_operator = -1;
    memset(m_out_counter, 0, sizeof(m_out_counter));
    memset(m_user_bomb, 0, sizeof(m_user_bomb));
}

void ddz_desk::set_timer(unsigned short tid, int delay, int data)
{
    m_controler->set_timer(m_deskid, tid, delay, data);
}

void ddz_desk::kill_timer(unsigned short tid)
{
    m_controler->kill_timer(m_deskid, tid);
}

void ddz_desk::check_begin()
{
	int size =DDZ_SEATS;
	if(GAME_MODE_CFG == GAME_TYPE_PK) size =2;
	for (int i=0; i<size; i++)
	{
		if(m_seat[i].state != PLAYER_STATE_READY)
			return;
	}
    if(IS_PK_MODE)
        on_pk_start();
    else
	    on_game_start();
}

bool ddz_desk::check_game_end(int seatid)
{
    return (m_hand[seatid].len == 0);
}
//bool on_match_play_end(unsigned int uids[3], int score[3])
void ddz_desk::on_match_end(int seatid)
{
    m_desk_state = DS_GAME_END;

    sc_game_end endobj;
    endobj.m_bomb = m_bomb_count;
    endobj.m_first = seatid;
    memset(endobj.m_glory,0,sizeof(endobj.m_glory));

    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(i == m_loader)
        {
            endobj.m_score[i] = 0;
        }
        else
        {
            endobj.m_score[i] = m_call_point;
            if(endobj.m_first == m_loader)
            {
                endobj.m_score[i] *= (-1);
            }
        }
    }

    //输家和剩余牌
    endobj.m_remain1 = endobj.m_first + 1;
    if(endobj.m_remain1 > 2) endobj.m_remain1 = 0;
    endobj.m_remain2 = endobj.m_remain1 + 1;
    if(endobj.m_remain2 > 2) endobj.m_remain2 = 0;
    endobj.m_remaincards1.set((unsigned char*)m_hand[endobj.m_remain1].cards, m_hand[endobj.m_remain1].len);
    endobj.m_remaincards2.set((unsigned char*)m_hand[endobj.m_remain2].cards, m_hand[endobj.m_remain2].len);
    //计算春天
    endobj.m_spring = is_spring(endobj.m_first);
    int double_n = m_bomb_count + endobj.m_spring;
    //计算炸弹
    for(int i=0; i<double_n; i++)
    {
        for(int i=0; i<DDZ_SEATS; i++)
        {
            endobj.m_score[i] *= 2;
        }
    }
    //计算加倍
    for(int i=0; i<DDZ_SEATS; i++)
    {
        endobj.m_double[i] = m_call_double[i];
        if(endobj.m_double[i]<0)endobj.m_double[i]=0;
        if(i == m_loader) continue;
        if(m_call_double[i] == 1)
        {
            if(m_call_double[m_loader] == 1)
            {
                endobj.m_score[i] *= 4;
            }
            else
            {
                endobj.m_score[i] *= 2;
            }
        }
        endobj.m_score[m_loader] -= endobj.m_score[i];
    }
    //计算金币
    int basepoint = m_controler->get_base_point();
    for(int i=0; i<DDZ_SEATS; i++)
    {
        endobj.m_score[i] *= basepoint;
    }
    send_game_result(&endobj);

    unsigned int uids[3];
    int score[3];
    for(int i=0; i<3; i++)
    {
        uids[i] = m_seat[i].uid;
        score[i] = (int)endobj.m_score[i];
    }
    if(!m_controler->on_match_play_end(uids, score, this))
    {
        return;
    }
    set_timer(TID_DESK_START_MATCH, 5, 0);
}

void ddz_desk::on_game_end(int seatid,int reason)
{
    kill_all_timer();
    if(IS_MATCH_MODE)
    {
        on_match_end(seatid);
        return;
    }
    m_desk_state = DS_GAME_END;
    sc_game_end obj;
    if(!IS_PK_MODE)
    {
        caculate_result(obj, seatid);
        write_score(obj);
        send_game_result(&obj);
        on_game_mission(seatid);
        check_card_mission(seatid);
        
        clear_gameinfo();
        clear_net_cut();
        clear_robot();
        check_limit();
        set_ready_timer();
		//LOG_DEBUG("game_end(%d)", seatid);
    }
    else
    {
        int lostid = seatid==0?1:0;
        obj.m_first = seatid;
        obj.m_remain1 = lostid;
        obj.m_remaincards1.set((unsigned char*)m_hand[lostid].cards, m_hand[lostid].len);
        obj.m_score[seatid] = m_pk_point;
        obj.m_score[lostid] = -m_pk_point;
        write_score(obj,DDZ_PK_SEATS);
        send_game_result(&obj);
        clear_gameinfo();
        bool dismiss = false;
        for(int i=0; i<DDZ_PK_SEATS; i++)
        {
            if(m_seat[i].sid == INVALID_SOCKET_ID)
            {
                //只要有一个人离开断线，就解散桌子
                dismiss = true;
                break;
            }
        }
        adjust_pk_point();
        if(m_pk_point<=0)
            dismiss = true;
        if(dismiss)
        {
		    on_dismiss();
		    return;
        }
        set_ready_timer();
		//LOG_DEBUG("game_end1(%d)", seatid);
    }
}

void ddz_desk::check_limit()
{
    if(IS_PK_MODE)
        return;
    access_condition& ac = config_manager::instance()->get_ac();
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(m_seat[i].uid == 0 || m_seat[i].sid == INVALID_SOCKET_ID)
            continue;
        room_user* user = m_controler->get_userinfo(m_seat[i].sid);
        if(!user)
            continue;
        if(ac.less_coin!=0 && user->coin < ac.less_coin)
        {
            send_force_left(m_seat[i].sid, 0, user->coin, ac.less_coin);
        }
        else if(ac.max_coin != 0 && user->coin > ac.max_coin)
        {
            send_force_left(m_seat[i].sid, 1, user->coin, ac.max_coin);
        }
        else
            continue;
        user_left(m_seat[i].sid, m_seat[i].uid);
    }
}

void ddz_desk::adjust_pk_point()
{
    __int64 min = m_pk_point;
    for(int i=0; i<DDZ_PK_SEATS; i++)
    {
        if(m_seat[i].sid == INVALID_SOCKET_ID || m_seat[i].uid == 0)
            continue;
        room_user* user = m_controler->get_userinfo(m_seat[i].sid);
        if(!user)
            continue;
        if(user->coin < min)
            min = user->coin;
    }
    m_pk_point = min;
    sc_pk_point_adjust obj;
    obj.m_point = m_pk_point;
    send_all(&obj);
}

void ddz_desk::on_game_mission(char winner)
{
    int water_num = cfg_inst->water_num();
    room_user* user = m_controler->get_userinfo(m_seat[winner].sid);
    if(user && user->vip.vip_tag)
    {
        water_num *= cfg_inst->water_vip_times();
    }
    if(water_num>0)
    {
        if(config_manager::instance()->water_winner() == 1)
        {
            if(winner == m_loader)
            {
                db_thread::instance()->on_add_prop(GAME_MISSION_REWARD, m_seat[winner].uid, 1103, water_num);
                sc_game_award_info obj;
                obj.m_icon = 123;
                obj.m_uid = m_seat[winner].uid;
                obj.m_num = water_num;
                obj.m_name.set("露水");
                send_all(&obj);
            }
        }
        else if(config_manager::instance()->water_winner() == 2)
        {
            if(winner == m_loader)
            {
                db_thread::instance()->on_add_prop(GAME_MISSION_REWARD, m_seat[winner].uid, 1103, water_num);
                sc_game_award_info obj;
                obj.m_uid = m_seat[winner].uid;
                obj.m_icon = 123;
                obj.m_num = water_num;
                obj.m_name.set("露水");
                send_all(&obj);
            }
            else
            {
                for(int i=0; i<DDZ_SEATS; i++)
                {
                    if(i == m_loader) continue;
                    db_thread::instance()->on_add_prop(GAME_MISSION_REWARD, m_seat[i].uid, 1103, water_num);
                    sc_game_award_info obj;
                    obj.m_uid = m_seat[i].uid;
                    obj.m_icon = 123;
                    obj.m_num = water_num;
                    obj.m_name.set("露水");
                    send_all(&obj);
                }
            }
        }
    }
}

void ddz_desk::set_ready_timer()
{
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(m_seat[i].uid != 0)
        {
            set_timer(TID_READY1+i, m_gameconfig.ready_time+MAX_DELAY, 0);
        }
    }
}

void ddz_desk::caculate_result(sc_game_end& endobj, int win)
{
    endobj.m_bomb = m_bomb_count;
    endobj.m_first = win;
	memset(endobj.m_glory,0,sizeof(endobj.m_glory));
    
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(i == m_loader)
        {
            endobj.m_score[i] = 0;
        }
        else
        {
            endobj.m_score[i] = m_call_point;
            if(endobj.m_first == m_loader)
            {
                endobj.m_score[i] *= (-1);
            }
        }
    }

    //输家和剩余牌
    endobj.m_remain1 = endobj.m_first + 1;
    if(endobj.m_remain1 > 2) endobj.m_remain1 = 0;
    endobj.m_remain2 = endobj.m_remain1 + 1;
    if(endobj.m_remain2 > 2) endobj.m_remain2 = 0;
    endobj.m_remaincards1.set((unsigned char*)m_hand[endobj.m_remain1].cards, m_hand[endobj.m_remain1].len);
    endobj.m_remaincards2.set((unsigned char*)m_hand[endobj.m_remain2].cards, m_hand[endobj.m_remain2].len);
    //计算春天
    endobj.m_spring = is_spring(endobj.m_first);
    int double_n = m_bomb_count + endobj.m_spring;
    //计算炸弹
    for(int i=0; i<double_n; i++)
    {
        for(int i=0; i<DDZ_SEATS; i++)
        {
            endobj.m_score[i] *= 2;
        }
    }
    //计算加倍
    for(int i=0; i<DDZ_SEATS; i++)
    {
        endobj.m_double[i] = m_call_double[i];
        if(endobj.m_double[i]<0)endobj.m_double[i]=0;
        if(i == m_loader) continue;
        if(m_call_double[i] == 1)
        {
            if(m_call_double[m_loader] == 1)
            {
                endobj.m_score[i] *= 4;
            }
            else
            {
                endobj.m_score[i] *= 2;
            }
        }
        endobj.m_score[m_loader] -= endobj.m_score[i];
    }
    //计算金币
    ddz_game_config& config = config_manager::instance()->get_game_config();
    for(int i=0; i<DDZ_SEATS; i++)
    {
        endobj.m_score[i] *= config.base_point;
    }
    //计算荣誉增值
    calulate_glory(endobj);
}

void ddz_desk::calulate_glory(sc_game_end& endobj)
{
    return;
    for(int i=0; i<DDZ_SEATS; i++)
    {
        endobj.m_glory[i] = 0;
    }
    return;
    int double_n = m_bomb_count + endobj.m_spring;
    glory_def& g = config_manager::instance()->get_glory();
    endobj.m_glory[m_loader] = 0;
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(i != m_loader)
        {
            endobj.m_glory[i] = g.base + double_n;
            if(endobj.m_first == m_loader)
            {
                endobj.m_glory[i] *= (-1);
            }
            endobj.m_glory[m_loader] -= endobj.m_glory[i];
        }
    }
}

void ddz_desk::write_score(sc_game_end& endobj,int players)
{
    ddz_game_config& config = config_manager::instance()->get_game_config();
    game_log_record log;
	memset(&log,0,sizeof(log));
    unsigned int uid[DDZ_SEATS];
    int win[DDZ_SEATS];
    int lost[DDZ_SEATS];
    int flee[DDZ_SEATS];
    __int64 src[DDZ_SEATS];
    __int64 dst[DDZ_SEATS];
    room_user* user[DDZ_SEATS];
    for(int i=0; i<DDZ_SEATS; i++)
    {
        uid[i] = 0;
        win[i] = 0;
        lost[i] = 0;
        flee[i] = 0;
        src[i] = 0;
        dst[i] = 0;
    }
    //顺便计算税收
    for(int i=0; i<players; i++)
    {
        uid[i] = m_seat[i].uid;
		user[i] = user_info(i);
        if(!user[i])
        {
            LOG_WARNING("write_score  %s %d", __FILE__, __LINE__);
            continue;
        }
        log.user[i].score1 = user[i]->coin;
        src[i] = user[i]->coin - config.tax;
        dst[i] = endobj.m_score[i];

        win[i]  = dst[i]>0?1:0;
        lost[i] = dst[i]<0?1:0;
        flee[i] = user[i]->sid==INVALID_SOCKET_ID?1:0;
        user[i]->rs.win += win[i];
        user[i]->rs.lost += lost[i];
        user[i]->rs.flee += flee[i];
    }
    coin_adjust(src, dst, m_loader);

	LOG_DEBUG("coin_adjust %lld , %lld, %lld ",dst[0],dst[1],dst[2]);

    unsigned int syn_uid[DDZ_SEATS] = {0};
    __int64 syn_coin[DDZ_SEATS] = {0};

    for(int i=0; i<players; i++)
    {
        syn_uid[i] = 0;
        syn_coin[i] = 0;
        if(!user[i])
        {
            continue;
        }
        log.user[i].uid = user[i]->uid;
        user[i]->coin = src[i] + dst[i];
        endobj.m_score[i] = dst[i];

        dst[i] -= config.tax;
        log.user[i].score2 = user[i]->coin;

        syn_uid[i] = user[i]->uid;
        syn_coin[i] = user[i]->coin;
        endobj.m_glory[i] = 0;
        if(user[i]->coin < 0)
        {
            LOG_ERROR("结算负分：uid=%d coin=%lld src=%lld dst=%lld", user[i]->uid, user[i]->coin, src[i], dst[i]);
            for(int j=0; j<players; j++)
            {
                if(!user[j])continue;
                LOG_ERROR("uid=%d seatid=%d coin=%lld src=%lld dst=%lld", user[j]->uid, j, user[j]->coin, src[j], dst[j]);
            }
        }
    }

    send_syn_coin(syn_uid, syn_coin, players);

    for(int i=0; i<players; i++)
    {
        db_thread::instance()->on_write_score(uid[i], dst[i], endobj.m_glory[i], win[i], lost[i], flee[i]);

        if((!IS_PK_MODE) && config_manager::instance()->win_mission_type()>0 &&
            config_manager::instance()->max_win_mission()>0 &&
            win[i] == 1)
        {
            redis_thread::instance()->on_win_mission(uid[i]);
        }
    }

    log.tax = config.tax * players;
	db_thread::instance()->on_write_game_log(log);
}

void ddz_desk::coin_adjust(__int64 src[3], __int64 dst[3], int loader)
{
    __int64 src1[3];
    __int64 dst1[3];
    for(int i=0; i<3; i++)
    {
        src1[i] = src[i];
        dst1[i] = dst[i];
    }
    if(IS_PK_MODE)
        return;
    int  losers = 0;
    // 修正赢得的数是否大于身上携带
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(dst[i] < 0)
            losers ++;
    }
    if(losers == 1)
    {
        //地主输
        //修正农民赢数不能超过自己的金币数
        for(int i=0; i<DDZ_SEATS; i++)
        {
            if(dst[i] > 0)
            {
                if(dst[i] > src[i])
                {
                    __int64 dc = dst[i] - src[i];
                    dst[i] = src[i];
                    dst[loader] += dc;
                }
            }
        }
    }
    else if(losers == 2)
    {
        //农民输
        //修正地主不能赢每个农民大于自己身上的金币
        __int64 score = 0;
        for(int i=0; i<DDZ_SEATS; i++)
        {
            if(i == loader)
                continue;
            if((dst[i]+src[loader]) < 0)
            {
                dst[i] = -src[loader];
            }
            score -= dst[i];
        }
        dst[loader] = score;
    }
    else
    {
        LOG_ERROR("loser=%d", losers);
    }
    //////////////////////////////////////////////////////////////////////////
    // 判断输家身上钱是否够输
    bool enough = true;
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if((src[i] + dst[i]) < 0)
        {
            enough = false;
            dst[i] = -src[i];
        }
    }
    if(enough)
        return;
    if(losers == 1)
    {
        __int64 score = dst[loader] / 2;
        __int64 remain = 0;
        for(int i=0; i<DDZ_SEATS; i++)
        {
            if(i == loader)
                continue;
            dst[i] = -score;
            if(dst[i] > src[i])
            {
                remain += (dst[i] - src[i]);
                dst[i] = src[i];
            }
        }
        for(int i=0; i<DDZ_SEATS; i++)
        {
            if(i == loader)continue;
            if(src[i]>(remain+dst[i]))
            {
                dst[i] += remain;
                break;
            }
        }
    }
    else if(losers == 2)
    {
        __int64 score = 0;
        //农民不够输的话输光为止
        for(int i=0; i<DDZ_SEATS; i++)
        {
            if(i == loader)
                continue;
            if((src[i]+dst[i]) < 0)
            {
                dst[i] = -src[i];
            }
            score += dst[i];
        }
        dst[loader] = -score;
    }
}

int ddz_desk::is_spring(int winner)
{
    if(winner == m_loader)
    {
        for(int i=0; i<DDZ_SEATS; i++)
        {
            if(i==winner)
                continue;
            if(m_out_counter[i] > 0)
                return 0;
        }
    }
    else
    {
        if(m_out_counter[winner] > 1)
            return 0;
    }
    return 1;
}

void ddz_desk::clear_net_cut()
{
    //LOG_DEBUG("ddz_desk::clear_net_cut()");
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(m_seat[i].is_robot)
            continue;
        if(m_seat[i].sid == INVALID_SOCKET_ID)
        {
            send_user_left(i, m_seat[i].uid);
            m_controler->clear_net_cut(m_seat[i].uid);
            m_seat[i].uid = 0;
            m_seat[i].state = 0;
        }
    }
}

void ddz_desk::create_pk(socket_id sid)
{
    m_desk_state = DS_GAME_END;
    m_pk_point = 0;
    //坐下
    on_sitdown(sid, 0);
    //生成code
    memset(m_code, 0, sizeof(m_code));
    for(int i=0; i<MAX_CODE_LEN; i++)
    {
        m_code[i] = '0' + random_int(0, 9);
    }
    //返回创建
    send_create_pk(sid);
    LOG_INFO("PK创建成功");
}

bool ddz_desk::add_pk(socket_id sid, char* code)
{
    LOG_INFO("开始加入PK");
    //非等待状态的桌子
    if(m_desk_state != DS_GAME_END)
        return false;
    //桌主不能为空
    if(m_seat[0].uid == 0)
        return false;
	if(m_seat[1].uid != 0)
		return false;
    //比对code
    for(int i=0; i<MAX_CODE_LEN; i++)
    {
        if(m_code[i] != code[i])
            return false;
    }
    //code失效
    memset(m_code, 0, sizeof(m_code));
    //坐下
    on_sitdown(sid, 1);
    send_add_pk(sid, 0);
    LOG_INFO("PK加入成功");
    return true;
}
void ddz_desk::on_set_pk_point(socket_id sid, int point)
{
    if(m_desk_state != DS_GAME_END)
        return;
    room_user* u1 = m_controler->get_userinfo(m_seat[0].sid);
    room_user* u2 = m_controler->get_userinfo(m_seat[1].sid);

    if(!u1 || !u2)
    {
        //这里做解散牌局处理，用户信息异常了
		sc_pk_end obj;
		obj.m_msg = STRING_RES(16);
		send_all(&obj);
		for(int i=0; i<2; i++)
			on_standup(i);
        return;
    }

    //计算合法pk底分
    __int64 min_coin = u1->coin;
    if(u2->coin < min_coin)
        min_coin = u2->coin;
    if(min_coin>point)
        min_coin = point;
    if(min_coin <= 0)
        return;

    m_pk_point = min_coin;

    sc_set_pk_point_result obj;
    obj.m_point = m_pk_point;
    send_all(&obj);
}

void ddz_desk::on_agree_pk_point(socket_id sid, bool agree)
{
    if(m_desk_state != DS_GAME_END)
        return;
    sc_agree_pk_point obj;
    if(!agree)
    {
        m_pk_point = 0;
        obj.m_type = 0;
        send_all(&obj);
        return;
    }
    if(m_pk_point == 0)
    {
        return;
    }
    obj.m_type = 1;
    send_all(&obj);
    on_pk_start();
}

void ddz_desk::on_pk_start()
{
    clear_gameinfo();

    for (int i=0; i<DDZ_SEATS; i++)
    {
        m_seat[i].state = PLAYER_STATE_GAMING;
        m_seat[i].is_auto = false;
    }
    //洗牌
    shuffle();
    //设置操作人
    m_operator = rand() % DDZ_PK_SEATS;
    m_first_call = m_operator;
    m_last_call_point = 0;

    for(int i=0; i<DDZ_PK_SEATS; i++)
    {
        sc_deal obj;
        obj.m_cards.set((unsigned char*)m_hand[i].cards, 17);
        send(m_seat[i].sid, &obj);
    }

    set_desk_state(DS_GAMING);
    send_out_cards(m_operator, 0, 0, 0, 0, m_operator, 0, 0, 0);
    set_out_timer(m_operator);
}

void ddz_desk::on_dismiss()
{
	sc_pk_end obj;
	obj.m_msg = STRING_RES(15);
	send_all(&obj);
	for(int i=0; i<DDZ_PK_SEATS; i++)
    {
        if(m_seat[i].uid != 0)
		    on_standup(i);
    }
	return;
}

int ddz_desk::get_win_robot_out(int seatid, out_cards& record, cards_buf hand_card[DDZ_SEATS], cards_buf& out)
{
    node root;

    out_cards_buf lastOutCards;
    record.get_last(lastOutCards);

    root.lastopman = convertWanID(lastOutCards.id, m_loader);    //上次出牌玩家

    root.opman = convertWanID(seatid, m_loader);                 //当前操作玩

    LOG_DEBUG("lastOutCards.id=%d", lastOutCards.id);

    if(lastOutCards.id<0)
    {
        root.lastopman = root.opman;
    }

    if(root.lastopman != root.opman&&!convertOutCard(root,lastOutCards))
    {   //出牌转换失败
        return 1;
    }
    if(!convertHandCard(root,hand_card,m_loader))
    {   //手牌转换失败
        return 2;
    }

    node sun;
    int deep = 0;
    if(m_seat[seatid].is_robot)
        deep = cfg_inst->ai_deep();
    DDZ_AI logic_ai(1000000, deep);
    char aiman = convertWanID(seatid,m_loader)==0?0:1;
    int ret = logic_ai.comput(root,aiman,sun);
    //int num = logic_ai.getSetNode();
    //CCLOG("ret=%d num=%d",ret,num);
    //char szlog[1000];
    //sprintf(szlog, "ret=%d num=%d\n",ret,num);
    //OutputDebugStringA(szlog);

    if(ret==1)
    {
        ///将结果放入
        if(sun.lastopman == root.lastopman&&root.lastopman!=root.opman)
        {
            //跟牌选择不出
            return 0;
        }
        getResult(sun.lastOutCard,hand_card[seatid],out);
        cards_buf buf[MAX_CANDIDATE];
        int n = ddz_get_cards_type_list2(out, 0, buf);
        if(n>0)
            out = buf[0];
        out.clz.size = 0;
        return 0;
    }
    if(ret==2)
    {
        int num = logic_ai.getNumNode();
        int setnum = logic_ai.getSetNode();
        return 3;
    }
    return 5;
}

bool ddz_desk::is_deep_ai()
{
    if(m_operator == m_loader)
        return true;
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(m_hand[i].len < 14)
            return true;
    }
    return false;
}

bool ddz_desk::is_all_ai()
{
    for (int i=0; i<DDZ_SEATS; i++)
    {
        if(!m_seat[i].is_robot)
            return false;
    }
    return true;
}

void ddz_desk::robot_out_card(int seatid)
{
    if(m_desk_state != DS_GAMING || seatid != m_operator)
    {
        return;
    }
    m_ai_mode_debug = 1;
    cards_buf hand_card[DDZ_SEATS];
    out_cards record;
    cards_buf out;
    memset(&out, 0, sizeof(out));
    if(!get_ai_out_record(record))
    {
        LOG_ERROR("get_ai_out_record fail");
        on_root_out_error(seatid);
        return;
    }
    if(record.size>0 && record.turns[0].id == seatid)
        record.clear();
    get_ai_hand_card(hand_card);

    bool is_robot[DDZ_SEATS];
    for(int i=0; i<DDZ_SEATS; i++)
    {
        is_robot[i] = m_seat[i].is_robot;
        if(m_seat[i].is_auto)
            is_robot[i] = true;
    }
    int ret = -1;
    int biglen = 0;
    out_cards_buf last;
    record.get_last(last);
    if(last.id == seatid)
        biglen = 1;
    cards_buf lastbuf(last.cards, last.size, last.card_type);
    cards_buf bigger[MAX_CANDIDATE];
    if(biglen == 0)
        biglen = ddz_get_bigger_compose2(lastbuf, hand_card[seatid], m_lz, bigger);
    if(biglen > 0 && is_deep_ai() && cfg_inst->is_ai_deep())
    {
        bool disturb = cfg_inst->is_ai_disturb() && m_seat[m_loader].is_robot;
        if(seatid == m_loader || seatid != m_loader && !disturb)
        {
            cards_buf hand_card_[DDZ_SEATS];
            out_cards record_;
            record_ = record;
            memcpy(hand_card_, hand_card, sizeof(hand_card));
            unsigned int t = GetTickCount();
            ret = get_win_robot_out(seatid, record_, hand_card_, out);
            t = GetTickCount() - t;
            LOG_DEBUG("get_win_robot_out=%d   t=%d", ret, t);
            if(ret == 0)
            {
                m_ai_mode_debug = 2;
                if (out.size > 0)
                {
                    char buf[1024];
                    int flen = sprintf(buf, "ai out type=%d size=%d :",out.card_type, out.size);
                    int n = out.size;
                    if(n>20)n=20;
                    for(int i=0; i<n; i++)
                    {
                        int fl = sprintf(&buf[flen], "%02x ", out.cards[i]);
                        flen += fl;
                        if(flen>500) break;
                    }
                    buf[flen] = 0;
                    LOG_DEBUG(buf);
                }
            }
        }
    }
    if(0 != ret)
    {
        int size = ddzai_get_robot_out(seatid, 0, hand_card, record, out, is_robot);
        if(size < 0 || size>20)
        {
            LOG_ERROR("robot out size=%d",size);
            //char buf[1024];
            //int index = 0;
            //for(int i=0; i<hand_card[seatid].size; i++)
            //{
            //    int fl = sprintf(&buf[index], "%02u,", hand_card[seatid].cards[i]);
            //    index += fl;
            //}
            //buf[index] = 0;
            //LOG_ERROR("card: %s", buf);
            on_root_out_error(seatid);
            return;
        }
        else
        {
            //char buf[1024];
            //int index = 0;
            //for(int i=0; i<hand_card[seatid].size; i++)
            //{
            //    int fl = sprintf(&buf[index], "%02u,", hand_card[seatid].cards[i]);
            //    index += fl;
            //}
            //buf[index] = 0;
            //LOG_ERROR("robot hand: %s, len=%d", buf, hand_card[seatid].size);
            //index = 0;
            //for(int i=0; i<out.size; i++)
            //{
            //    int fl = sprintf(&buf[index], "%02u,", out.cards[i]);
            //    index += fl;
            //}
            //LOG_ERROR("robot  out: %s, type=%d, len=%d", buf, out.card_type, out.size);
        }
    }

    char buf[1280] = {0};
    int len = 0;
    for(int i=0; i<out.size; i++)
    {
        char n = out.cards[i];
        int fl = sprintf(&buf[len], "%02x,", n);
        len += fl;
    }
    buf[len] = 0;
    out.clz.size = 0;
    on_outcards(seatid, out.cards, out.size, out.card_type);
}

bool ddz_desk::get_ai_out_record(out_cards& record)
{
    record.clear();
    record.set_first(m_loader);

    if(m_out_record.size == 0)
    {
        return true;
    }

    out_cards_item item = m_out_record.out_cards[m_out_record.size-1];
    out_cards_buf buf;
    buf.id = item.seatid;
    buf.card_type = item.cardtype;
    buf.size = item.len;
    memcpy(buf.cards, item.cards, item.len);
    record.add(buf);
    return true;

    int op = m_loader;
    for(int i=0; i<m_out_record.size; i++)
    {
        out_cards_item item = m_out_record.out_cards[i];
        out_cards_buf buf;
        memset(&buf, 0, sizeof(buf));
        if(item.seatid<0 || item.seatid>2)
        {
            LOG_ERROR("item.seatid=%d", item.seatid);
            continue;
        }
        while(item.seatid != op)
        {
            buf.id = op;
            buf.size = 0;
            buf.card_type = 0;
            record.add(buf);

            op++;if(op>2)op=0;
        }
        buf.id = op;
        buf.card_type = item.cardtype;
        buf.size = item.len;
        memcpy(buf.cards, item.cards, item.len);
        record.add(buf);
        
        op++;if(op>2)op=0;
        if(record.size >= 60)
            return false;
    }
    return true;
}

void ddz_desk::get_ai_hand_card(cards_buf hand_card[DDZ_SEATS])
{
    for(int i=0; i<DDZ_SEATS; i++)
    {
        memset(&hand_card[i], 0, sizeof(cards_buf));
        memcpy(hand_card[i].cards, m_hand[i].cards, m_hand[i].len);
        hand_card[i].size = m_hand[i].len;
    }
}

void ddz_desk::on_root_out_error(int seatid)
{
    LOG_WARNING("robot ai error");
    bool is_first = is_first_out(seatid);
    char buf[1] = {0};
    int  size = 0;
    if(is_first)
    {
        buf[0] = m_hand[seatid].cards[m_hand[seatid].len-1];
        size = 1;
    }
    on_outcards(seatid, buf, size, MDDZ_CT_ONE);
}

room_user* ddz_desk::user_info(int seatid)
{
	room_user* user = 0;
	if(seatid<0 || seatid>=DDZ_SEATS)
		return 0;
	user = m_controler->get_userinfo(m_seat[seatid].sid, m_seat[seatid].uid);
	if(user) return user;
	if(m_seat[seatid].is_robot)
		user = robot_inst->user_info(m_seat[seatid].uid);
	return user;
}