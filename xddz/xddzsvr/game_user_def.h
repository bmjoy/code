#ifndef __X_GAME_USER_DEF_H__
#define __X_GAME_USER_DEF_H__
#include "i_tcp_socket_engine.h"
#include "ddzprot.h"
#include <string.h>
#include <time.h>

enum DDZ_PLAYER_STATE
{
    PLAYER_STATE_WAIT,      //�ȴ�
    PLAYER_STATE_READY,     //׼��
    PLAYER_STATE_GAMING     //��Ϸ
};

struct round_stat
{
    int win;                //Ӯ����
    int lost;               //�����
    int flee;               //���ܾ���
    int glory;              //����
};

struct vip_info
{
	char vip_tag;			//vip��ʾ
	int vip_level;          //vip�ȼ�
	__int64 exp;			//vip����ʱ��
};

struct desk_user
{
    unsigned int uid;       //�û�ID
    socket_id    sid;       //socket_id
    int          state;     //��Ϸ״̬ DDZ_PLAYER_STATE
	bool         is_auto;   //�й�״̬
    bool         is_robot;  //�Ƿ������
};

struct count_box_info
{
	int level;
	int seconds;
};

struct room_user
{
    unsigned int   uid;         //�û�ID
    socket_id      sid;         //socket_id
    char           nickname[64];//�û��ǳ�
    int            roomid;      //����ID
    int            deskid;      //����ID
    int            seatid;      //��λID
    round_stat     rs;          //�û���Ϸ�ɼ�
    __int64        coin;        //��Ϸ��
    int            logoid;      //ͷ��ID
	count_box_info count_box;   //��ʱ����
	char			sex;		//�Ա�
	int             count_box_timestamp;//��ʱ����ʱ���
	vip_info		vip;
    int             card_mission_index;
    bool            isrobot;    //�Ƿ�����ˣ�������ʹ��
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