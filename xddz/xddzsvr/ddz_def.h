#ifndef __X_DDZ_DEF_H__
#define __X_DDZ_DEF_H__
#include "game_user_def.h"


#define DDZ_SEATS 3
#define DDZ_PK_SEATS 2
/*
enum GAME_MODE_TYPE
{
    GAME_MODE_NORMAL,       //��ͨ������
    GAME_MODE_HUANLE,       //���ֶ�����
    GAME_MODE_GAZI,         //���Ӷ�����
    GAME_MODE_XIAOBING,     //С��������
	GAME_MODE_PK			//Լս
};
*/
struct ddz_game_config
{
    int game_mode;         //��Ϸģʽ GAME_MODE_TYPE
    int surport_lz;        //�Ƿ�֧�����
    int call_time;         //�з�ʱ��
    int out_time;          //����ʱ��
    int pass_time;         //��������ʱpassʱ��
    int first_out_time;    //�����״γ���ʱ��
    int base_point;        //������
    int auto_ready;        //�Զ�׼��
    int tax;               //��λ��
	int ready_time;		   //׼��ʱ��
    int jipaiqi;           //������ 
    int renshu;            //����
};

struct glory_def
{
    int base;              //������Ӯ
    int bomb;              //ը������
    int spring;            //���콱��
};

struct access_condition
{
    __int64 less_coin;
    __int64 max_coin;
};
/*
struct online_mission
{
	int uid;				//�û�ID
	int levels;				//��ǰ����ȼ�
	int seconds;			//��ǰ����
	int time;				//��������
	int pid;				//����ID
	int pn;					//��������
	int coin;				//��������
	char desc[128];			//��������
};
*/
//<item t="300" pid="1" pn="1" coin="10" desc="XXX"/>
struct count_box_item
{
	int t;
	int pid;
	int pn;
	__int64 coin;
	char desc[128];
};

//��ʱ����ID
enum TID_FLAG
{
	TID1_PLATFORM,			//ƽ̨��ʱ��
	TID1_ROOM,				//���䶨ʱ��
	TID1_GAME				//��Ϸ��ʱ��
};
//��Ϸ��״̬
enum DESK_STATE
{
	DS_GAME_END,            //��Ϸ����
	DS_CALL_POINT,          //�зֽ׶�
	DS_CALL_DOUBLE_FARMER,  //ũ��ӱ��׶�
	DS_CALL_DOUBLE_LANDER,  //�����ӱ��׶�
	DS_GAMING               //��Ϸ�׶�
};

#define MAX_DELAY 10

//��Ϸ��ʱ��ID
enum DESK_TIMER_ID
{
	TID_CALL_POINT,          //�з�
	TID_CALL_DOUBLE_FARMER,  //ũ��ӱ�
	TID_CALL_DOUBLE_LANDER,  //�����ӱ�
	TID_OUT_CARD,            //����

	TID_READY1,				 //�����˿���ͬʱ����, ���Զ���������ʱ��ID
	TID_READY2,
	TID_READY3,
    TID_DESK_START_MATCH,
	TID_MATCH_ROBOT_WAIT,
};

//���ƴ������Ͷ���
enum SEND_CARD_ERR_TYPE
{
	CARD_ERR_INVALID_CARDS = 1,  //�Ƿ�����
	CARD_ERR_INVALID_OPERATOR    //�Ƿ�������Ա
};

enum SITDOWN_ERR_TYPE
{
	SITDOWN_ABNORMAL_ERR,		//���´���
	SEAT_NOT_EMPTY,				//��λ�ǿ�
	GAMEING_CANNOT_SIT,			//������Ϸ
};

#endif//__X_DDZ_DEF_H__