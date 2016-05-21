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
    ��ʼ��
    @param max_size ���������
    */
    void init(int max_size);
    void fini();
    /*
    ��ȡ�û���Ϣ
    @param sid socketid
    @param uid �û�ID
    socketid��uidȫ������, �ŷ����û���Ϣ. ���򷵻ؿ�
    */
    room_user* get_userinfo(socket_id sid, unsigned int uid);
    room_user* get_userinfo(socket_id sid);
    /*
    ��ȡ�����û���Ϣ
    @param uid �����û�ID
    */
    room_user* get_net_cut_userinfo(unsigned int uid);
    /*
    ����û�
    @param uinfo �û���Ϣ
    �ڵ�½�ɹ�ʱ���
    */
    room_user* add_user(room_user uinfo);
    /*
    ����û���Ϣ
    @param sid socket_id
    @param is_net_cut �������Ϸ�ж��ߵ����, ��ҪΪtrue, ���û���Ϣ�����ڶ����û��б���
    */
    void clear_user_info(socket_id sid, bool is_net_cut=false);
    /*
    ��������û���Ϣ
    @param uid �û�ID
    ��Ϸ����ʱ, �û����Ϊ����, ����
    */
    void clear_net_cut(unsigned int uid);
    /*
    �ж��Ƿ�Ϊ�����û�, �û���֤�ɹ�ʱ����
    @param uid �û�ID
    @param sid socket_id
    ���Ϊ���������û�, ���û���Ϣ�ظ���������Ϣ��, �������û�ָ��, ���򷵻�Ϊ��
    */
    room_user* is_net_cut(unsigned int uid, socket_id sid);

    /*
    �ж��Ƿ�Ϊ�����û���������ͬ�ʺ��ظ���¼
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
