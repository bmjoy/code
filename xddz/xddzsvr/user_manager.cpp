#include "user_manager.h"
#include "vector_helper.h"
#include "db_thread.h"
#include "cflog.h"
#include "mission.h"
#include "config_manager.h"
#include "cflog.h"
#include <assert.h>
#include "log_type.h"
#include "myrandom.h"

user_manager::user_manager(void)
{
}

user_manager::~user_manager(void)
{
}

void user_manager::init(int max_size)
{
    m_users.resize(max_size);
    for(int i=0; i<max_size; i++)
    {
        m_users[i].clear();
    }
    m_net_cut_user.clear();
    m_online_user.clear();
}

void user_manager::fini()
{
    release_vector(m_users);
    m_net_cut_user.clear();
    m_online_user.clear();
}

room_user* user_manager::get_userinfo(socket_id sid, unsigned int uid)
{
    if (sid.index >= m_users.size())
    {
        //LOG_DEBUG("%s %d %d", __FILE__, __LINE__, sid.index);
        return 0;
    }
    if(m_users[sid.index].uid != uid || m_users[sid.index].sid != sid)
    {
        //LOG_ERROR("%s %d, %d %d", __FILE__, __LINE__, m_users[sid.index].uid, uid);
        return 0;
    }
    return &m_users[sid.index];
}

room_user* user_manager::get_userinfo(socket_id sid)
{
    if (sid.index >= m_users.size())
        return 0;
    if(m_users[sid.index].sid != sid)
        return 0;
    return &m_users[sid.index];
}

room_user* user_manager::get_net_cut_userinfo(unsigned int uid)
{
    map<unsigned int, room_user>::iterator i = m_net_cut_user.find(uid);
    if(i == m_net_cut_user.end())
        return 0;
    return &i->second;
}

room_user* user_manager::add_user(room_user uinfo)
{
    socket_id sid = uinfo.sid;
    unsigned int uid = uinfo.uid;
    if(sid.index >= m_users.size())
        return 0;
    if(m_users[sid.index].uid != 0)
        return 0;

    m_users[sid.index] = uinfo;

    map<unsigned int, room_user>::iterator i = m_net_cut_user.find(uid);
    if(i != m_net_cut_user.end())
    {
        m_users[sid.index].roomid = i->second.roomid;
        m_users[sid.index].deskid = i->second.deskid;
        m_users[sid.index].seatid = i->second.seatid;
        m_net_cut_user.erase(i);
    }
    m_online_user[uinfo.uid] = &m_users[sid.index];
    return &m_users[sid.index];
}

void user_manager::add_net_cut_user(room_user uinfo)
{
    m_net_cut_user[uinfo.uid] = uinfo;
}

void user_manager::clear_user_info(socket_id sid, bool is_net_cut)
{
    if (sid.index >= m_users.size())
        return;
    unsigned int uid = m_users[sid.index].uid;
    if(uid == 0)
        return;

    if(is_net_cut)
    {
        m_users[sid.index].sid = INVALID_SOCKET_ID;
        m_net_cut_user[uid] = m_users[sid.index];
    }
    else
    {
        db_thread::instance()->on_logout(uid);
		mission::instance()->update_mission(sid);
    }
    map<unsigned int, room_user*>::iterator i;
    i = m_online_user.find(uid);
    if(i != m_online_user.end()) m_online_user.erase(i);
    m_users[sid.index].clear();
}

void user_manager::clear_net_cut(unsigned int uid)
{
    map<unsigned int, room_user>::iterator i = m_net_cut_user.find(uid);
    if(i == m_net_cut_user.end())
        return;
    db_thread::instance()->on_logout(uid);
    m_net_cut_user.erase(i);
}

void user_manager::clear_match_user(socket_id sid, unsigned int uid)
{
	map<unsigned int, room_user>::iterator i = m_net_cut_user.find(uid);
	if(i != m_net_cut_user.end())
		m_net_cut_user.erase(i);
	if(sid.index>=0 && sid.index<m_users.size())
		m_users[sid.index].clear();
}

room_user* user_manager::is_net_cut(unsigned int uid, socket_id sid)
{
    if(sid.index >= m_users.size())
        return 0;

    map<unsigned int, room_user>::iterator i = m_net_cut_user.find(uid);
    if(i == m_net_cut_user.end())
        return 0;

    i->second.sid = sid;
    m_users[sid.index] = i->second;
    m_net_cut_user.erase(i);
    m_online_user[uid] = &m_users[sid.index];
    return &m_users[sid.index];
}

room_user* user_manager::is_online_user(unsigned uid)
{
    map<unsigned int, room_user*>::iterator i;
    i = m_online_user.find(uid);
    if(i != m_online_user.end())
        return i->second;
    return 0;
}

//////////////////////////////////////////////////////////////////////////
void robot_manager::add_user(room_user uinfo)
{
    guard g(m_mutex);
    bool is_ok = false;
    do 
    {
        if(uinfo.coin < config_manager::instance()->get_ac().less_coin)
        {
            break;
        }
        else if(config_manager::instance()->get_ac().max_coin > 0 &&
            uinfo.coin > config_manager::instance()->get_ac().max_coin)
        {
            break;
        }
        is_ok = true;
    } while (false);

    if(!is_ok)
    {
        if(!cfg_inst->is_ai_auto_supplement())
        {
            db_thread::instance()->on_logout(uinfo.uid);
            return;
        }
        int coin = random_int(cfg_inst->ai_supplement_from(), cfg_inst->ai_supplement_to());
        int dc = (int)(coin - uinfo.coin);
        uinfo.coin = coin;
        db_thread::instance()->on_add_coin(AI_SUPPLEMENT, uinfo.uid, dc);
    }

    map<unsigned int, room_user>::iterator i;
    i = m_robot.find(uinfo.uid);
    if(i == m_robot.end())
    {
        m_robot[uinfo.uid] = uinfo;
        m_idle.push(&m_robot[uinfo.uid]);
        i = m_robot.find(uinfo.uid);
    }
    else
    {
        i->second.coin = uinfo.coin;
        i->second.vip = uinfo.vip;
    }
}

room_user* robot_manager::get_idle()
{
    guard g(m_mutex);
    room_user* user = 0;
    if(m_idle.size() == 0)
        return user;
    
    user = m_idle.front();
    m_idle.pop();

    return user;
}

void robot_manager::put(unsigned int uid)
{
    guard g(m_mutex);

    map<unsigned int, room_user>::iterator i;
    i = m_robot.find(uid);
    if(i == m_robot.end())
        return;

    bool is_delete = false;

    if(i->second.coin < config_manager::instance()->get_ac().less_coin)
        is_delete = true;
    else if(config_manager::instance()->get_ac().max_coin > 0 &&
        i->second.coin > config_manager::instance()->get_ac().max_coin)
        is_delete = true;

    if(is_delete)
    {
        if(!cfg_inst->is_ai_auto_supplement())
        {
            db_thread::instance()->on_logout(i->second.uid);
            m_robot.erase(i);
            return;
        }
        int coin = random_int(cfg_inst->ai_supplement_from(), cfg_inst->ai_supplement_to());
        int dc = (int)(coin - i->second.coin);
        i->second.coin = coin;
        db_thread::instance()->on_add_coin(AI_SUPPLEMENT, uid, dc);
    }
    m_idle.push(&(i->second));
}

int robot_manager::worker_size()
{
    guard g(m_mutex);
    int ret = m_robot.size() - m_idle.size();
    return ret;
}

room_user* robot_manager::user_info(unsigned int uid)
{
    map<unsigned int, room_user>::iterator i;
    i = m_robot.find(uid);
    if(i == m_robot.end())
        return 0;
    return &(i->second);
}

void robot_manager::check_exist(map<int, int>& userids)
{
    guard g(m_mutex);
    int size = m_idle.size();
    for(int i=0; i<size; i++)
    {
        if(m_idle.size() == 0)
            break;

        room_user* user = m_idle.front();
        m_idle.pop();
        if(!user)
            continue;

        map<unsigned int, room_user>::iterator ui;
        map<int, int>::iterator li;
        
        li = userids.find(user->uid);
        if(li == userids.end())
        {
            ui = m_robot.find(user->uid);
            if(ui != m_robot.end())
            {
                db_thread::instance()->on_logout(user->uid);
                m_robot.erase(ui);
            }
        }
        else
        {
            m_idle.push(user);
        }
    }
    LOG_INFO("robot: all size=%d, work=%d", m_robot.size(), m_robot.size()-m_idle.size());
}

void robot_manager::dump()
{
    char buf[1024];
    sprintf(buf, "robot: all size=%d, work=%d", m_robot.size(), m_robot.size()-m_idle.size());
    cout << buf << endl;
}