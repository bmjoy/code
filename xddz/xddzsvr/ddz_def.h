#ifndef __X_DDZ_DEF_H__
#define __X_DDZ_DEF_H__
#include "game_user_def.h"


#define DDZ_SEATS 3
#define DDZ_PK_SEATS 2
/*
enum GAME_MODE_TYPE
{
    GAME_MODE_NORMAL,       //普通斗地主
    GAME_MODE_HUANLE,       //欢乐斗地主
    GAME_MODE_GAZI,         //嘎子斗地主
    GAME_MODE_XIAOBING,     //小兵斗地主
	GAME_MODE_PK			//约战
};
*/
struct ddz_game_config
{
    int game_mode;         //游戏模式 GAME_MODE_TYPE
    int surport_lz;        //是否支持癞子
    int call_time;         //叫分时间
    int out_time;          //出牌时间
    int pass_time;         //不够大牌时pass时间
    int first_out_time;    //地主首次出牌时间
    int base_point;        //基础分
    int auto_ready;        //自动准备
    int tax;               //座位费
	int ready_time;		   //准备时长
    int jipaiqi;           //记牌器 
    int renshu;            //认输
};

struct glory_def
{
    int base;              //基础输赢
    int bomb;              //炸弹奖励
    int spring;            //春天奖励
};

struct access_condition
{
    __int64 less_coin;
    __int64 max_coin;
};
/*
struct online_mission
{
	int uid;				//用户ID
	int levels;				//当前任务等级
	int seconds;			//当前秒数
	int time;				//所需秒数
	int pid;				//道具ID
	int pn;					//道具数量
	int coin;				//货币数量
	char desc[128];			//奖励描述
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

//定时器主ID
enum TID_FLAG
{
	TID1_PLATFORM,			//平台定时器
	TID1_ROOM,				//房间定时器
	TID1_GAME				//游戏定时器
};
//游戏桌状态
enum DESK_STATE
{
	DS_GAME_END,            //游戏结束
	DS_CALL_POINT,          //叫分阶段
	DS_CALL_DOUBLE_FARMER,  //农民加倍阶段
	DS_CALL_DOUBLE_LANDER,  //地主加倍阶段
	DS_GAMING               //游戏阶段
};

#define MAX_DELAY 10

//游戏定时器ID
enum DESK_TIMER_ID
{
	TID_CALL_POINT,          //叫分
	TID_CALL_DOUBLE_FARMER,  //农民加倍
	TID_CALL_DOUBLE_LANDER,  //地主加倍
	TID_OUT_CARD,            //出牌

	TID_READY1,				 //三个人可以同时操作, 所以定义三个定时器ID
	TID_READY2,
	TID_READY3,
    TID_DESK_START_MATCH,
	TID_MATCH_ROBOT_WAIT,
};

//出牌错误类型定义
enum SEND_CARD_ERR_TYPE
{
	CARD_ERR_INVALID_CARDS = 1,  //非法牌型
	CARD_ERR_INVALID_OPERATOR    //非法操作人员
};

enum SITDOWN_ERR_TYPE
{
	SITDOWN_ABNORMAL_ERR,		//坐下错误
	SEAT_NOT_EMPTY,				//座位非空
	GAMEING_CANNOT_SIT,			//正在游戏
};

#endif//__X_DDZ_DEF_H__