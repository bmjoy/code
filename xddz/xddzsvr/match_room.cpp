#include "game_room.h"
#include "config_manager.h"
#include "timer.h"
#include "socket_service.h"
#include "user_manager.h"
#include "cflog.h"
#include <vector_helper.h>
#include "string_res.h"
#include "ms_connector.h"
#include "timer.h"
#include <algorithm>
#include "log_type.h"
#include "db_thread.h"
using namespace std;

match_room_manager::match_room_manager()
{

}

match_room_manager::~match_room_manager()
{

}

game_room* match_room_manager::get_empty_room(int sn)
{
    for(size_t i=0; i<m_room.size(); i++)
    {
        if(!m_room[i]->is_matching())
        {
            m_room[i]->set_matching(sn);
            m_matching_room[sn] = m_room[i];
            return m_room[i];
        }
    }
	int maxdesk = (config_manager::instance()->get_max_con()+4) / 3;
    game_room* room = new game_room;
    room->roomid(m_room.size());
	room->init(maxdesk);
	room->init_users();
    room->set_matching(sn);
    m_room.push_back(room);

    m_matching_room[sn] = room;
    return room;
}

void match_room_manager::match_end(int sn)
{
    map<int,game_room*>::iterator i = m_matching_room.find(sn);
    if(i != m_matching_room.end())
    {
        m_matching_room.erase(i);
    }
}

game_room* match_room_manager::get_matching_room(int sn)
{
    map<int,game_room*>::iterator i = m_matching_room.find(sn);
    if(i == m_matching_room.end())
        return 0;
    return i->second;
}

game_room* match_room_manager::get_room(int roomid)
{
    if(roomid<0 || roomid >= (int)m_room.size() || !m_room[roomid] || !m_room[roomid]->is_matching())
        return 0;
    return m_room[roomid];
}

int match_room_manager::get_matching_room_sn(int* sns, int size)
{
    size = size<(int)m_matching_room.size()?size:m_matching_room.size();
    map<int,game_room*>::iterator i;
    int index = 0;
    for(i=m_matching_room.begin(); i!=m_matching_room.end(); i++)
    {
        sns[index] = i->first;
        index ++;
    }
    return size;
}

//////////////////////////////////////////////////////////////////////////

void game_room::init_match(list<room_user>* l)
{
	m_roomid;
    list<room_user>& ll = *l;
    list<room_user>::iterator i;
    LOG_DEBUG("init_match sn=%d", m_sn);
    int rank = 1;
    for(i=ll.begin(); i!=ll.end(); i++)
    {
        match_user_item user;
        user.uid = i->uid;
        user.isrobot = i->isrobot;
        strcpy(user.nickname, i->nickname);
        user.point = cfg_inst->match_point();
        user.rank = rank;
        user.state = MATCH_USER_STATE_WAIT;
        user.plays = 0;
        m_enroll_user[user.uid] = user;
        m_rankuser.push_back(&m_enroll_user[user.uid]);
        rank ++;

        i->roomid = m_roomid;
        i->deskid = -1;
        i->seatid = -1;
        m_users.add_net_cut_user((*i));
    }
    int desks = l->size() / 3;
    m_desks.clear();
    for(int i=0; i<desks; i++)
    {
        ddz_desk* desk = desk_manager::instance()->get_desk(m_roomid, i);
        desk->init(this, config_manager::instance()->get_game_config());
        m_desks.push_back(desk);
    }
    m_start_time = time(0);
    m_wait_start_match = true;
    m_match_turns = 0;
    ms_inst->send_load_result(m_sn, 1);
    timer_singleton::instance()->set_timer(TID1_ROOM, 0, 0, TID_WAIT_START_MATCH, config_manager::instance()->wait_match_start_time(), 0);
}

void game_room::start_match()
{
    if(!m_wait_start_match)
        return;
    m_wait_start_match = false;
    timer_singleton::instance()->kill_timer(TID1_ROOM, 0, 0, TID_WAIT_START_MATCH);
    m_match_turns = 0;
    m_turns_time_stamp = time(0);
    arrange_match_seat();
}

void game_room::on_match_user_login_ok(unsigned int uid, socket_id sid)
{
    map<unsigned int, match_user_item>::iterator i = m_enroll_user.find(uid);
    if(i == m_enroll_user.end())
        return;
    i->second.sid = sid;
    send_rank_info(sid, uid);
    if(m_wait_start_match)
    {
        check_start_match();
    }
    else
    {
        room_user* user = m_users.get_userinfo(sid);
        if(!user)
        {
            LOG_ERROR("It's imposible");
            return;
        }
        if(user->deskid >= 0 && user->deskid<(int)m_desks.size() && m_desks[user->deskid])
        {
            m_desks[user->deskid]->on_user_recome(sid, uid, user->seatid);
        }
        else
        {
            send_wait_match(sid);
        }
    }
}

void game_room::check_start_match()
{
    map<unsigned int, match_user_item>::iterator i;
    for(i=m_enroll_user.begin(); i!=m_enroll_user.end(); i++)
    {
		if(i->second.isrobot) continue;
        if(i->second.sid == INVALID_SOCKET_ID)
            return;
    }
    start_match();
}

int game_room::get_login_size()
{
    int size = 0;
    map<unsigned int, match_user_item>::iterator i;
    for(i=m_enroll_user.begin(); i!=m_enroll_user.end(); i++)
    {
        if(i->second.sid != INVALID_SOCKET_ID)
            size ++;
    }
    return size;
}

int game_room::get_enroll_size()
{
    return (int)m_enroll_user.size();
}

match_user_item* game_room::get_match_user(unsigned int uid)
{
    map<unsigned int, match_user_item>::iterator i = m_enroll_user.find(uid);
    if(i == m_enroll_user.end())
        return 0;
    if(i->second.state == MATCH_USER_STATE_OUT)
        return 0;
    return &i->second;
}

sc_match_rank_info game_room::get_rank_info(int page)
{
    sc_match_rank_info obj;

    obj.m_users = m_rankuser.size();
    obj.m_all_turns = cfg_inst->match_rule().turns.size();
    obj.m_current_turns = m_match_turns + 1;
    obj.m_time_span = (int)(time(0) - m_start_time);

    int all_page = m_rankuser.size() / obj.m_rank.max_size() + 1;
    if(page>all_page) page = all_page;
    if(page<0)page = 0;
    obj.m_all_page = all_page;
    obj.m_page = page;
    int size = 0;
    obj.m_rank.size(0);
    for(int i=page*obj.m_rank.max_size(); i<(page+1)*(obj.m_rank.max_size()); i++)
    {
        if(i >= (int)m_rankuser.size())
            break;
        obj.m_rank[size].m_nickname = m_rankuser[i]->nickname;
        obj.m_rank[size].m_uid = m_rankuser[i]->uid;
        obj.m_rank[size].m_point = m_rankuser[i]->point;
        obj.m_rank[size].m_rank = m_rankuser[i]->rank;
        obj.m_rank[size].m_state = m_rankuser[i]->state;
        size ++;
        obj.m_rank.size(size);
    }
    obj.m_gaming_desks = 0;
    for(int i=0; i<(int)m_desks.size(); i++)
    {
        if(!m_desks[i]) continue;
        if(m_desks[i]->is_gaming())
            obj.m_gaming_desks ++;
    }
    return obj;
}

void game_room::send_rank_info(socket_id sid, unsigned int uid, int page)
{
    sc_match_rank_info obj = get_rank_info(page);
    map<unsigned int, match_user_item>::iterator i = m_enroll_user.find(uid);
    if(i == m_enroll_user.end())
    {
        LOG_ERR_LINE();
        return;
    }
    obj.m_myrank.m_rank = i->second.rank;
    obj.m_myrank.m_point = i->second.point;
    obj.m_myrank.m_nickname.set(i->second.nickname);
    obj.m_myrank.m_state = i->second.state;
    socket_service::instance()->send_obj(sid, &obj);
}

void game_room::send_wait_match(socket_id sid)
{
    sc_match_waiting obj;
    socket_service::instance()->send_obj(sid, &obj);
}

int game_room::get_base_point()
{
    return cfg_inst->match_rule().turns[m_match_turns].base;
}

void game_room::arrange_match_seat()
{
    MatchRule& rule = cfg_inst->match_rule();
    match_user_item* users[1000];
    int size = 0;
	if(m_rankuser.size() < 3)
	{
		end_match();
		return;
	}
    for(int i=0; i<(int)m_rankuser.size(); i++)
    {
        users[size] = 0;
        if(!m_rankuser[i])
            continue;
        if(m_rankuser[i]->state != MATCH_USER_STATE_WAIT)
            continue;
        if(m_rankuser[i]->plays >= rule.turns[m_match_turns].plays)
            continue;
        users[size] = m_rankuser[i];
        size ++;
        if(size >= 1000)
            break;
    }
    if(size < 3)
        return;
    
    int ndesk = size / 3;
    int arrang_users = 0;
    ddz_desk* desk = 0;
    int from_index = 0;
    for(int i=0; i<ndesk; i++)
    {
        desk = get_empty_desk(from_index);
        if(!desk)
        {
            LOG_ERR_LINE();
            break;
        }
        socket_id sids[3];
        bool      robot[3];
        unsigned int uids[3];
        for(int j=0; j<3; j++)
        {
            uids[j] = users[i*3+j]->uid;
            sids[j] = users[i*3+j]->sid;
            robot[j]= users[i*3+j]->isrobot;
            users[i*3+j]->state = MATCH_USER_STATE_GAMING;
        }
        desk->set_match_user(sids, uids, robot);
        arrang_users += 3;
    }
    for(int i=arrang_users; i<size; i++)
    {
        if(users[i] && users[i]->sid != INVALID_SOCKET_ID)
            send_wait_match(users[i]->sid);
    }
}

bool game_room::on_match_play_end(unsigned int uids[3], int score[3], ddz_desk* desk)
{
    map<unsigned int, match_user_item>::iterator ui;
    match_user_item* users[3];
    for(int i=0; i<3; i++)
    {
        users[i] = 0;
        ui = m_enroll_user.find(uids[i]);
        if(ui == m_enroll_user.end())
        {
            LOG_ERR_LINE();
            continue;
        }
        users[i] = &ui->second;
        users[i]->point += score[i];
        users[i]->plays ++;
    }
    sort_rank();
    //计算淘汰
    MatchRule& rule = cfg_inst->match_rule();
    bool ret = true;
    for(int i=0; i<3; i++)
    {
        if(users[i]->point < rule.turns[m_match_turns].less_point)
        {
            if ((int)m_rankuser.size() > rule.turns[m_match_turns].less_user)
            {
                do_end_user_match(users[i]->uid);
                users[i] = 0;
                ret = false;
            }
        }
    }
    //判断局数，是否继续比赛，或等待下一轮
    if(ret)
    {
        for(int i=0; i<3; i++)
        {
            if(users[i]->plays >= rule.turns[m_match_turns].plays)
            {
                ret = false;
                break;
            }
        }
    }
    //判断是否到了当前轮最大时间
    if(ret)
    {
        time_t dt = time(0) - m_turns_time_stamp;
        if((int)dt > rule.turns[m_match_turns].max_time)
        {
            ret = false;
        }
    }
    sort_rank();
    //更新排名
    for(int i=0; i<(int)m_desks.size(); i++)
    {
        if(m_desks[i])
        {
            m_desks[i]->send_rank_change();
        }
    }
    sc_match_rank_info obj = get_rank_info(0);
    for(size_t i=0; i<m_rankuser.size(); i++)
    {
        if(!m_rankuser[i]) continue;
        if(m_rankuser[i]->state != MATCH_USER_STATE_WAIT) continue;
        if(m_rankuser[i]->sid == INVALID_SOCKET_ID) continue;
        obj.m_myrank.m_rank = m_rankuser[i]->rank;
        obj.m_myrank.m_point = m_rankuser[i]->point;
        obj.m_myrank.m_nickname.set(m_rankuser[i]->nickname);
        obj.m_myrank.m_state = m_rankuser[i]->state;
        socket_service::instance()->send_obj(m_rankuser[i]->sid, &obj);
    }
    if(!ret)
    {
        if(m_rankuser.size() < 3)
        {
            end_match();
        }
        else
        {
            for(int i=0; i<3; i++)
            {
                if(!users[i]) continue;
                users[i]->state = MATCH_USER_STATE_WAIT;
                obj.m_myrank.m_rank = users[i]->rank;
                obj.m_myrank.m_point = users[i]->point;
                obj.m_myrank.m_nickname.set(users[i]->nickname);
                obj.m_myrank.m_state = users[i]->state;
                send_wait_match(users[i]->sid);
                socket_service::instance()->send_obj(users[i]->sid, &obj);
                room_user* user = m_users.get_userinfo(users[i]->sid);
                if(!user)  user = m_users.get_net_cut_userinfo(users[i]->uid);
                if(user)
                {
                    user->deskid = -1;
                    user->seatid = -1;
                }
            }
            desk->clear_match_user();
            if(!has_matching_desk())
            {
                next_turns();
            }
            else
            {
                arrange_match_seat();
            }
        }
    }
    return ret;
}

RuleTurnItem* game_room::get_turn_item()
{
    MatchRule& rule = cfg_inst->match_rule();
    if(m_match_turns >= (int)rule.turns.size())
        return 0;
    return &rule.turns[m_match_turns];
}

void game_room::next_turns()
{
    sort_rank();

    m_turns_time_stamp = time(0);
    MatchRule& rule = cfg_inst->match_rule();
    m_match_turns ++;
    if(m_match_turns >= (int)rule.turns.size())
    {
        end_match();
        return;
    }
    while ((int)m_rankuser.size() > rule.turns[m_match_turns].max_user)
    {
        do_end_user_match(m_rankuser[m_rankuser.size()-1]->uid);
    }
    for(int i=0; i<(int)m_rankuser.size(); i++)
    {
        if(!m_rankuser[i]) continue;
        m_rankuser[i]->plays = 0;
    }
    arrange_match_seat();
}

bool game_room::has_matching_desk()
{
    for(int i=0; i<(int)m_desks.size(); i++)
    {
        if(!m_desks[i]) continue;
        if(m_desks[i]->get_desk_state() != DS_GAME_END)
            return true;
    }
    return false;
}

void game_room::do_end_user_match(unsigned int uid)
{
	LOG_DEBUG("do_end_user_match:%d", uid);
    map<unsigned int, match_user_item>::iterator ui;
	int rank = -1;
	do 
	{
		ui = m_enroll_user.find(uid);
		if(ui == m_enroll_user.end())
			break;
		rank = ui->second.rank;
		RankRewardItem* reward = cfg_inst->get_reward(rank);
		sc_match_end obj;
		obj.m_rank = rank;
		if(reward == 0)
		{
			obj.m_coin = 0;
			obj.m_prop.size(0);
		}
		else
		{
			//发奖
			obj.m_coin = reward->coin;
			if(obj.m_coin > 0)
			{
				db_thread::instance()->on_add_coin(MATCH_REWARD, uid, obj.m_coin);
				db_thread::instance()->match_log(uid, cfg_inst->serverid(), m_sn, 3, obj.m_coin);
			}
			for(int i=0; i<(int)reward->prop.size(); i++)
			{
				obj.m_prop[i].m_pid = reward->prop[i].pid;
				obj.m_prop[i].m_pn  = reward->prop[i].pn;
				if(reward->prop[i].pid>0 && reward->prop[i].pn>0)
				{
					db_thread::instance()->on_add_prop(MATCH_REWARD, uid, reward->prop[i].pid, reward->prop[i].pn, reward->prop[i].term);
				}
			}
			obj.m_prop.size(reward->prop.size());
		}
		//删除排行
		vector<match_user_item*>::iterator ri;
		for(ri=m_rankuser.begin(); ri!=m_rankuser.end(); ri++)
		{
			if((*ri) == &ui->second)
			{
				m_rankuser.erase(ri);
				break;
			}
		}
		//修改状态
		ui->second.state = MATCH_USER_STATE_OUT;

		//发送通知
		socket_service::instance()->send_obj(ui->second.sid, &obj);
	} while (false);
    ms_inst->send_user_end(m_sn, uid, rank);
	m_users.clear_match_user(ui->second.sid, uid);
}

bool game_room::is_match_user(unsigned int uid)
{
    map<unsigned int, match_user_item>::iterator i = m_enroll_user.find(uid);
    if(i == m_enroll_user.end())
        return false;
    if(i->second.state == MATCH_USER_STATE_OUT)
        return false;
    return true;
}

bool rank_sort_fun(const match_user_item* s1, const match_user_item* s2)
{
    return (s1->point > s2->point);
}

void game_room::sort_rank()
{
    sort(m_rankuser.begin(), m_rankuser.end(), rank_sort_fun);
    for(int i=0; i<(int)m_rankuser.size(); i++)
    {
        m_rankuser[i]->rank = i+1;
    }
}

void game_room::end_match()
{
    m_matching = false;
    ms_inst->send_match_end(m_sn);
    while (m_rankuser.size() > 0)
    {
        if(!m_rankuser[m_rankuser.size()-1])
        {
            m_rankuser.pop_back();
        }
        else
        {
            do_end_user_match(m_rankuser[m_rankuser.size()-1]->uid);
        }
    }
    m_enroll_user.clear();
    for(int i=0; i<(int)m_desks.size(); i++)
    {
        if(m_desks[i])
        {
            desk_manager::instance()->put_desk(m_desks[i]);
        }
    }
    m_desks.resize(0);
    match->match_end(m_sn);
}

void game_room::on_match_msg(socket_id sid, msg_object_base* obj)
{
    switch (obj->minor_id())
    {
    case MN_GET_RANK_INFO:
        {
            CAST_OBJ(cs_get_rank_info);
            on_get_rank_info(sid, o->m_page);
        }
        break;
    default:
        break;
    }
}

void game_room::on_get_rank_info(socket_id sid, int page)
{
    room_user* user = m_users.get_userinfo(sid);
    if(!user)return;
    
    send_rank_info(sid, user->uid, page);
}
