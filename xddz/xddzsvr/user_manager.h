#ifndef __X_USER_MANAGER_H__
#define __X_USER_MANAGER_H__
#include "game_user_def.h"
#include "singleton.h"
#include <map>
#include <vector>
#include <queue>
#include "mutex.h"
using namespace std;

class user_manager// : public singleton<user_manager>
{
public:
    user_manager(void);
    ~user_manager(void);
    /*
    初始化
    @param max_size 最大连接数
    */
    void init(int max_size);
    void fini();
    /*
    获取用户信息
    @param sid socketid
    @param uid 用户ID
    socketid与uid全部符合, 才返回用户信息. 否则返回空
    */
    room_user* get_userinfo(socket_id sid, unsigned int uid);
    room_user* get_userinfo(socket_id sid);
    /*
    获取断线用户信息
    @param uid 断线用户ID
    */
    room_user* get_net_cut_userinfo(unsigned int uid);
    /*
    添加用户
    @param uinfo 用户信息
    在登陆成功时添加
    */
    room_user* add_user(room_user uinfo);
    /*
    清除用户信息
    @param sid socket_id
    @param is_net_cut 如果是游戏中断线的玩家, 需要为true, 将用户信息保存在断线用户列表中
    */
    void clear_user_info(socket_id sid, bool is_net_cut=false);
    /*
    清除断线用户信息
    @param uid 用户ID
    游戏结束时, 用户如果为离线, 调用
    */
    void clear_net_cut(unsigned int uid);
    /*
    判断是否为断线用户, 用户认证成功时调用
    @param uid 用户ID
    @param sid socket_id
    如果为断线重入用户, 将用户信息回复到在线信息中, 并返回用户指针, 否则返回为空
    */
    room_user* is_net_cut(unsigned int uid, socket_id sid);

    /*
    判断是否为在线用户，避免相同帐号重复登录
    */
    room_user* is_online_user(unsigned uid);

    int online_users(){return m_online_user.size();}
    int offline_users(){return m_net_cut_user.size();}
    void add_net_cut_user(room_user uinfo);
	void clear_match_user(socket_id sid, unsigned int uid);
private:
    map<unsigned int, room_user> m_net_cut_user;
    vector<room_user>            m_users;
    map<unsigned int, room_user*> m_online_user;
};
//#define USER_INFO(x) user_manager::instance()->get_userinfo(x)

class robot_manager : public singleton<robot_manager>
{
public:
    void add_user(room_user uinfo);
    room_user* get_idle();
    void put(unsigned int uid);
    room_user* user_info(unsigned int uid);
    int  worker_size();
    void check_exist(map<int, int>& userids);
    void dump();
private:
    
private:
    thread_mutex                 m_mutex;
    map<unsigned int, room_user> m_robot;
    queue<room_user*>            m_idle;
};

#define robot_inst robot_manager::instance()


#endif//__X_USER_MANAGER_H__
