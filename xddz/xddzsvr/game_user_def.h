#ifndef __X_GAME_USER_DEF_H__
#define __X_GAME_USER_DEF_H__
#include "i_tcp_socket_engine.h"
#include "ddzprot.h"
#include <string.h>
#include <time.h>

enum DDZ_PLAYER_STATE
{
    PLAYER_STATE_WAIT,      //等待
    PLAYER_STATE_READY,     //准备
    PLAYER_STATE_GAMING     //游戏
};

struct round_stat
{
    int win;                //赢局数
    int lost;               //输局数
    int flee;               //逃跑局数
    int glory;              //荣誉
};

struct vip_info
{
	char vip_tag;			//vip标示
	int vip_level;          //vip等级
	__int64 exp;			//vip过期时间
};

struct desk_user
{
    unsigned int uid;       //用户ID
    socket_id    sid;       //socket_id
    int          state;     //游戏状态 DDZ_PLAYER_STATE
	bool         is_auto;   //托管状态
    bool         is_robot;  //是否机器人
};

struct count_box_info
{
	int level;
	int seconds;
};

struct room_user
{
    unsigned int   uid;         //用户ID
    socket_id      sid;         //socket_id
    char           nickname[64];//用户昵称
    int            roomid;      //房间ID
    int            deskid;      //桌子ID
    int            seatid;      //座位ID
    round_stat     rs;          //用户游戏成绩
    __int64        coin;        //游戏币
    int            logoid;      //头像ID
	count_box_info count_box;   //计时宝箱
	char			sex;		//性别
	int             count_box_timestamp;//计时宝箱时间戳
	vip_info		vip;
    int             card_mission_index;
    bool            isrobot;    //是否机器人，比赛中使用
    void clear()
    {
        uid = 0;
        sid = INVALID_SOCKET_ID;
        nickname[0] = 0;
        roomid = -1;
        deskid = -1;
        seatid = -1;
        coin   = 0;
        logoid = 0;
		sex = 0;
        count_box_timestamp = 0;
        memset(&rs, 0, sizeof(rs));
		memset(&vip, 0, sizeof(vip));
        card_mission_index;
        isrobot = false;
    }
	void update_count_box()
	{
		int ct = _time32(0);
		int dt = ct - count_box_timestamp;
		count_box.seconds += dt;
		count_box_timestamp = _time32(0);
	}
    room_user(){clear();}
};

struct user_session_id
{
    unsigned int uid;
    socket_id    sid;
    bool operator == (user_session_id& id)
    {
        return ((id.uid==uid)&&(id.sid==sid));
    }
    bool operator != (user_session_id& id)
    {
        return !((id.uid==uid)&&(id.sid==sid));
    }
};

struct match_user_item
{
    socket_id    sid;
    char         nickname[64];
    unsigned int uid;
    int          point;
    int          state;
    int          rank;
    int          plays;
    bool         isrobot;
    match_user_item()
    {
        sid = INVALID_SOCKET_ID;
        nickname[0] = 0;
        uid = 0;
        point = 0;
        state = MATCH_USER_STATE_WAIT;
        rank = 0;
        plays = 0;
        isrobot = false;
    }
};

#endif//__X_GAME_USER_DEF_H__