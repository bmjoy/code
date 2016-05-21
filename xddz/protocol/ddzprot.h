/*本代码由工具生成，请勿手动修改，切记！切记！很重要，不要偷懒！*/
#ifndef __DDZPROT_MAKE_BY_PT_13161_H__
#define __DDZPROT_MAKE_BY_PT_13161_H__
#include "ustring.h"
#include "binary.h"
#include "msg_object_base.h"
#include "mvector.h"

#pragma warning(disable:4996)
/*
修订记录：
---2014-6-21------------------------------------------------------
1. 增加记牌器开关
---2014-5-3------------------------------------------------------
1. 增加金币同步刷新协议
---2014-4-26-----------------------------------------------------
3. 普通场增加游戏结束时的破产赠送
2. 增加PK场比分校正协议，当游戏结束后，重新下发校正分值，因为双方可能存在有一方的分值小于PK值。
1. 配置中增加防作弊配置属性，如果防作弊开启，不能聊天，头像加遮罩，开局2圈后取消头像遮罩，一局结束后重新排座位
*/

#define LOGIN_RESULT_OK             0  //登录成功
#define LOGIN_RESULT_SYS_BUSY       1  //系统繁忙
#define LOGIN_RESULT_IN_OTHER_ROOM  2  //在其他房间
#define LOGIN_RESULT_ACCOUNT_IN_USE 3  //帐号正在使用
#define LOGIN_RESULT_PAUSE          4  //系统暂停服务
#define LOGIN_RESULT_LESS_CONDITION 5  //条件不足
#define LOGIN_RESULT_LESS_COIN      6  //游戏币不足
#define LOGIN_RESULT_OPENID_FAIL    7  //令牌错误
#define LOGIN_RESULT_DISENABLE      8  //帐号封
#define LOGIN_RESULT_MORE_COIN      9  //金币过多
#define LOGIN_RESULT_NEED_VIP       10  //登录需要VIP身份
#define LOGIN_RESULT_MATCH_END      11  //登录的比赛已经结束
#define CREATE_PK_GAME              0  //创建一个PK桌子
#define ADD_PK_GAME                 1  //加入一个PK桌子
#define USER_STATE_FREE             0  //用户自由状态
#define USER_STATE_READY            1  //用户准备状态
#define USER_STATE_GAMEING          2  //用户游戏状态
#define USER_STATE_NET_CUT          3  //用户断线状态
#define GETONLINE_RESULT_OK         0  //获取计时任务数据成功
#define GAMEEND_NORMAL              0  //游戏正常结束
#define GAMEEND_GIVEUP              1  //二人场放弃约战结算
#define GAMEEND_USER_LEFT           2  //游戏中强退
#define GAMEEND_DISMISS             3  //断线
#define GAME_TYPE_NORMAL            0  //普通游戏类型
#define GAME_TYPE_MATCH             1  //比赛游戏类型
#define GAME_TYPE_PK                4  //PK游戏类型
#define MATCH_USER_STATE_WAIT       0  //比赛状态：等待
#define MATCH_USER_STATE_GAMING     1  //比赛状态：游戏中
#define MATCH_USER_STATE_OUT        2  //比赛状态：淘汰
/*---------------------------------------- ----------------------------------------*/
struct RankInfo;
struct PropItem;
struct DeskRankItem;
//名次信息
struct RankInfo
{
    int m_uid;//用户ID
    int m_point;//积分
    char m_state;//状态
    ustring<32,unsigned short> m_nickname;//昵称
    short m_rank;//名次
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_uid)) return false;
        if(!bs.read(m_point)) return false;
        if(!bs.read(m_state)) return false;
        if(!m_nickname.serialize_in(bs)) return false;
        if(!bs.read(m_rank)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_uid)) return false;
        if(!bs.write(m_point)) return false;
        if(!bs.write(m_state)) return false;
        if(!m_nickname.serialize_out(bs)) return false;
        if(!bs.write(m_rank)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//道具信息
struct PropItem
{
    int m_pid;//道具ID
    int m_pn;//道具数量
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_pid)) return false;
        if(!bs.read(m_pn)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_pid)) return false;
        if(!bs.write(m_pn)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//
struct DeskRankItem
{
    short m_rank;//名次
    int m_point;//比赛积分
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_rank)) return false;
        if(!bs.read(m_point)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_rank)) return false;
        if(!bs.write(m_point)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
/*---------------------------------------- ----------------------------------------*/
#define MJ_GAME_SERVER                 1 //服务器主协议

#define MN_PING                        1 //心跳
#define MN_PONG                        2 //心跳回复
#define MN_LOGIN                      10 //登陆
#define MN_LOGIN_RESULT               11 //登陆返回结果
#define MN_CUT_OFF                    12 //服务器通知客户端断开网络

/*---------------------------------------- ----------------------------------------*/
#define MJ_GAME_CONTROL_DDZ           10 //游戏控制协议

#define MN_GAME_USER_SITDOWN           1 //坐下请求
#define MN_GAME_USER_SITDOWN_SC        2 //坐下通知
#define MN_GAME_USER_SITDOWN_ERR       3 //坐下通知
#define MN_LEFT_DESK_REQUEST           4 //用户离开(换桌)请求
#define MN_USER_LEFT_DESK              5 //用户离开桌子通知
#define MN_USER_NET_CUT                6 //用户断线,只发给同桌用户
#define MN_USER_RECOME                 7 //断线重连
#define MN_USER_READY                  8 //用户准备
#define MN_FORCE_USER_LEFT_DESK        9 //通知用户强迫离开座位
#define MN_BANKRUPT_RESPONST          10 //破产赠送响应
#define MN_ARRANGE_DESK               11 //通知客户端正在分配座位

/*---------------------------------------- ----------------------------------------*/
#define MJ_GAME_LOGIC_DDZ             15 //游戏逻辑协议

#define MN_GAME_CONFIG                 1 //游戏桌配置信息
#define MN_DEAL                        2 //发牌
#define MN_CALL_POINT                  3 //用户叫分结果
#define MN_USER_CALL_POINT             4 //用户叫分
#define MN_LAND_RESULT                 5 //确认地主
#define MN_LAND_BACK_CARDS             6 //下发底牌
#define MN_GAME_CALL_DOUBLE            7 //通知用户加倍
#define MN_GAME_DOUBLE_RESULT          8 //加倍结果,客户端服务器共用协议
#define MN_GAME_OUT_CARD_RESULT        9 //通知出牌
#define MN_GAME_OUT_CARD              10 //玩家出牌
#define MN_GAME_OUT_CARD_ERR          11 //出牌错误提示
#define MN_GAME_END                   12 //游戏结束
#define MN_RECOME_STATE               13 //断线重连状态
#define MN_RECOME_STATE_CALL_POINT    14 //叫分阶段重连场景信息
#define MN_RECOME_STATE_CALL_DOUBLE   15 //加倍阶段断线重连场景信息
#define MN_RECOME_STATE_OUTCARDS      16 //出牌阶段断线重连场景信息
#define MN_USER_AUTO_STATE            17 //修改托管状态,服务器客户端共用
#define MN_LZ_CARD                    18 //下发癞子牌型
#define MN_CALL_MINGPAI               19 //下发用户明牌
#define MN_MINGPAI_CARD               20 //下发用户明牌
#define NM_USER_STATE                 21 //用户状态
#define MN_SYN_OUTCARD                22 //同步已经打出的牌，断线重连后下发
#define MN_SYN_COIN                   23 //
#define MN_SET_CALL_POINT             24 //设置叫分人
#define MN_CARD_MISSION_INFO          25 //牌局任务描述，发牌后下发
#define MN_CARD_MISSION_COMPLISH      26 //牌局任务奖励，结算后，完成时发，没完成不发

/*---------------------------------------- ----------------------------------------*/
#define MJ_MESSAGE_DDZ_GAME           16 //消息类协议

#define MN_SYSTEM_MESSAGE              1 //系统消息
#define MN_GAME_MESSAGE                2 //游戏消息
#define MN_CHAT_MESSAGE                3 //聊天消息

/*---------------------------------------- ----------------------------------------*/
#define MJ_PK_GAME                    17 //2人场协议

#define MN_SELECT_MODE                 1 //选择模式
#define MN_CREATE_PK_RESULT            2 //创建返回
#define MN_ADD_PK_RESULT               3 //加入返回结果
#define MN_SET_PK_POINT                4 //设置PK分数，只有房主可以设置
#define MN_SET_PK_POINT_RESULT         5 //设置PK分数结果
#define MN_AGREE_SET_PK_POINT          6 //确认PK分数设置
#define MN_GIVEUP                      7 //投降
#define MN_PK_END                      8 //比赛结束
#define MN_PK_POINT_ADJUST             9 //修正PK分值，当游戏结束时，重新下发PK值，因为有人会比PK值低

/*---------------------------------------- ----------------------------------------*/
#define MJ_MISSION_DDZ_SERVER         18 //任务

#define MN_TIME_COUNT_CFG_RESULT       1 //计时数据请求
#define MN_GET_AWARD                   2 //领奖
#define MN_GET_AWARD_RESULT            3 //领奖
#define MN_GAME_AWARD_INFO             4 //游戏中奖励通知，服务器主动下发，客户端不请求

/*---------------------------------------- ----------------------------------------*/
#define MJ_MATCH_DDZ                  19 //比赛协议族

#define MN_GET_RANK_INFO               1 //等待界面，翻页排名时发送，如果当前页等于最大页时，不再请求
#define MN_MATCH_RANK_INFO             2 //赛况信息,等待界面的用户只发首页，用户翻页时再请求后续片段
#define MN_MATCH_END                   3 //比赛结束通知
#define MN_MATCH_WAITING               4 //通知用户等待开始
#define MN_RANK_CHANGE                 5 //比赛名次发生变化(只有本桌用户)
#define MN_MATCH_BASE                  6 //比赛底分，发牌前发送
#define MN_LOGIN_MATCH                 7 //登录比赛

//心跳
class mn_ping : public msg_object_base
{
public:
    int m_value;//PING值
    mn_ping()
    {
        m_major_id = MJ_GAME_SERVER;
        m_minor_id = MN_PING;
    }
    ~mn_ping(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_value)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_value)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//心跳回复
class mn_pong : public msg_object_base
{
public:
    int m_value;//PING值返回
    mn_pong()
    {
        m_major_id = MJ_GAME_SERVER;
        m_minor_id = MN_PONG;
    }
    ~mn_pong(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_value)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_value)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//登陆
class cs_login : public msg_object_base
{
public:
    char m_ClientTpye;//客户端类型
    ustring<64,unsigned short> m_openid;//开放ID
    cs_login()
    {
        m_major_id = MJ_GAME_SERVER;
        m_minor_id = MN_LOGIN;
    }
    ~cs_login(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ClientTpye)) return false;
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ClientTpye)) return false;
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//登陆返回结果
class sc_login_result : public msg_object_base
{
public:
    char m_result;//0成功,其他失败
    unsigned int m_userid;//用户自己的ID,失败时无效
    ustring<256,unsigned short> m_msg;//登陆失败信息
    int m_serverid;//登录失败时，正在其他服务器的serverid，0表示无效
    sc_login_result()
    {
        m_major_id = MJ_GAME_SERVER;
        m_minor_id = MN_LOGIN_RESULT;
    }
    ~sc_login_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!bs.read(m_userid)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        if(!bs.read(m_serverid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!bs.write(m_userid)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        if(!bs.write(m_serverid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//服务器通知客户端断开网络
class mn_cut_off : public msg_object_base
{
public:
    char m_id;//断开类型ID
    ustring<256,unsigned short> m_msg;//断开信息描述
    mn_cut_off()
    {
        m_major_id = MJ_GAME_SERVER;
        m_minor_id = MN_CUT_OFF;
    }
    ~mn_cut_off(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//坐下请求
class game_user_sitdown : public msg_object_base
{
public:
    unsigned int m_userid;//用户ID
    short m_tableid;//桌子ID,-1时系统分配
    char m_seatid;//座位ID,-1时系统分配
    game_user_sitdown()
    {
        m_major_id = MJ_GAME_CONTROL_DDZ;
        m_minor_id = MN_GAME_USER_SITDOWN;
    }
    ~game_user_sitdown(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_userid)) return false;
        if(!bs.read(m_tableid)) return false;
        if(!bs.read(m_seatid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_userid)) return false;
        if(!bs.write(m_tableid)) return false;
        if(!bs.write(m_seatid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//坐下通知
class game_user_sitdown_sc : public msg_object_base
{
public:
    unsigned int m_userid;//用户ID
    short m_tableid;//桌子ID
    char m_seatid;//座位ID
    ustring<64,unsigned short> m_nickname;//用户昵称
    long long m_coin;//用户金币
    int m_glory;//用户荣誉值
    int m_logoid;//用户头像
    int m_win;//胜局数
    int m_loss;//失败局数
    int m_flee;//逃跑局数
    char m_vip;//VIP标志
    long long m_vipexpire;//VIP有效时间（小时）
    char m_state;//状态
    char m_sex;//性别
    game_user_sitdown_sc()
    {
        m_major_id = MJ_GAME_CONTROL_DDZ;
        m_minor_id = MN_GAME_USER_SITDOWN_SC;
    }
    ~game_user_sitdown_sc(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_userid)) return false;
        if(!bs.read(m_tableid)) return false;
        if(!bs.read(m_seatid)) return false;
        if(!m_nickname.serialize_in(bs)) return false;
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_glory)) return false;
        if(!bs.read(m_logoid)) return false;
        if(!bs.read(m_win)) return false;
        if(!bs.read(m_loss)) return false;
        if(!bs.read(m_flee)) return false;
        if(!bs.read(m_vip)) return false;
        if(!bs.read(m_vipexpire)) return false;
        if(!bs.read(m_state)) return false;
        if(!bs.read(m_sex)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_userid)) return false;
        if(!bs.write(m_tableid)) return false;
        if(!bs.write(m_seatid)) return false;
        if(!m_nickname.serialize_out(bs)) return false;
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_glory)) return false;
        if(!bs.write(m_logoid)) return false;
        if(!bs.write(m_win)) return false;
        if(!bs.write(m_loss)) return false;
        if(!bs.write(m_flee)) return false;
        if(!bs.write(m_vip)) return false;
        if(!bs.write(m_vipexpire)) return false;
        if(!bs.write(m_state)) return false;
        if(!bs.write(m_sex)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//坐下通知
class game_user_sitdown_err : public msg_object_base
{
public:
    char m_errid;//失败ID
    game_user_sitdown_err()
    {
        m_major_id = MJ_GAME_CONTROL_DDZ;
        m_minor_id = MN_GAME_USER_SITDOWN_ERR;
    }
    ~game_user_sitdown_err(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_errid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_errid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//用户离开(换桌)请求
class left_desk_request : public msg_object_base
{
public:
    char m_type;//0退出房间 1换桌
    left_desk_request()
    {
        m_major_id = MJ_GAME_CONTROL_DDZ;
        m_minor_id = MN_LEFT_DESK_REQUEST;
    }
    ~left_desk_request(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//用户离开桌子通知
class user_left_desk : public msg_object_base
{
public:
    unsigned int m_userid;//用户ID
    user_left_desk()
    {
        m_major_id = MJ_GAME_CONTROL_DDZ;
        m_minor_id = MN_USER_LEFT_DESK;
    }
    ~user_left_desk(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_userid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_userid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//用户断线,只发给同桌用户
class sc_user_net_cut : public msg_object_base
{
public:
    unsigned int m_userid;//用户ID
    sc_user_net_cut()
    {
        m_major_id = MJ_GAME_CONTROL_DDZ;
        m_minor_id = MN_USER_NET_CUT;
    }
    ~sc_user_net_cut(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_userid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_userid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//断线重连
class sc_user_recome : public msg_object_base
{
public:
    unsigned int m_userid;//用户ID
    sc_user_recome()
    {
        m_major_id = MJ_GAME_CONTROL_DDZ;
        m_minor_id = MN_USER_RECOME;
    }
    ~sc_user_recome(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_userid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_userid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//用户准备
class sc_user_ready : public msg_object_base
{
public:
    unsigned int m_userid;//用户ID
    sc_user_ready()
    {
        m_major_id = MJ_GAME_CONTROL_DDZ;
        m_minor_id = MN_USER_READY;
    }
    ~sc_user_ready(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_userid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_userid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//通知用户强迫离开座位
class sc_force_user_left_desk : public msg_object_base
{
public:
    char m_reson;//离开类型:0金币不足，1金币超过本房
    long long m_owncoin;//用户身上金币
    long long m_limit;//本房间限制
    sc_force_user_left_desk()
    {
        m_major_id = MJ_GAME_CONTROL_DDZ;
        m_minor_id = MN_FORCE_USER_LEFT_DESK;
    }
    ~sc_force_user_left_desk(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_reson)) return false;
        if(!bs.read(m_owncoin)) return false;
        if(!bs.read(m_limit)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_reson)) return false;
        if(!bs.write(m_owncoin)) return false;
        if(!bs.write(m_limit)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//破产赠送响应
class sc_bankrupt_responst : public msg_object_base
{
public:
    long long m_coin;//获赠金币
    int m_result;//获取破产送0成功，1失败
    ustring<128,unsigned short> m_msg;//提示语
    sc_bankrupt_responst()
    {
        m_major_id = MJ_GAME_CONTROL_DDZ;
        m_minor_id = MN_BANKRUPT_RESPONST;
    }
    ~sc_bankrupt_responst(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_result)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_result)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//通知客户端正在分配座位
class sc_arrange_desk : public msg_object_base
{
public:
    sc_arrange_desk()
    {
        m_major_id = MJ_GAME_CONTROL_DDZ;
        m_minor_id = MN_ARRANGE_DESK;
    }
    ~sc_arrange_desk(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//游戏桌配置信息
class sc_game_config : public msg_object_base
{
public:
    short m_readytime;//准备倒计时长
    char m_calltime;//叫分和加倍倒计时长
    char m_outtime;//出牌倒计时长
    char m_passtime;//明显无法大过上家的倒计时长
    char m_firstouttime;//首次出牌倒计时长
    char m_autoready;//坐下时，是否显示准备按钮
    char m_gamemode;//游戏类型
    char m_laizi;//是否支持癞子
    int m_basepoint;//基础分
    char m_anticheat;//防作弊开关,如果为1表示打开,不能聊天，头像上加防作弊遮罩，不能查看用户信息
    char m_jipaiqi;//记牌器开关，1打开，0关闭
    char m_renshu;//是否允许认输
    sc_game_config()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_GAME_CONFIG;
    }
    ~sc_game_config(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_readytime)) return false;
        if(!bs.read(m_calltime)) return false;
        if(!bs.read(m_outtime)) return false;
        if(!bs.read(m_passtime)) return false;
        if(!bs.read(m_firstouttime)) return false;
        if(!bs.read(m_autoready)) return false;
        if(!bs.read(m_gamemode)) return false;
        if(!bs.read(m_laizi)) return false;
        if(!bs.read(m_basepoint)) return false;
        if(!bs.read(m_anticheat)) return false;
        if(!bs.read(m_jipaiqi)) return false;
        if(!bs.read(m_renshu)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_readytime)) return false;
        if(!bs.write(m_calltime)) return false;
        if(!bs.write(m_outtime)) return false;
        if(!bs.write(m_passtime)) return false;
        if(!bs.write(m_firstouttime)) return false;
        if(!bs.write(m_autoready)) return false;
        if(!bs.write(m_gamemode)) return false;
        if(!bs.write(m_laizi)) return false;
        if(!bs.write(m_basepoint)) return false;
        if(!bs.write(m_anticheat)) return false;
        if(!bs.write(m_jipaiqi)) return false;
        if(!bs.write(m_renshu)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//发牌
class sc_deal : public msg_object_base
{
public:
    char m_seatid;//座位ID
    binary<17> m_cards;//手牌
    sc_deal()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_DEAL;
    }
    ~sc_deal(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_seatid)) return false;
        if(!m_cards.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_seatid)) return false;
        if(!m_cards.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//用户叫分结果
class sc_call_point_result : public msg_object_base
{
public:
    char m_seatid;//当前叫分玩家座位
    char m_point;//当前叫分值
    sc_call_point_result()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_CALL_POINT;
    }
    ~sc_call_point_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_seatid)) return false;
        if(!bs.read(m_point)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_seatid)) return false;
        if(!bs.write(m_point)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//用户叫分
class cs_call_point : public msg_object_base
{
public:
    char m_point;//叫分值, 0为PASS
    cs_call_point()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_USER_CALL_POINT;
    }
    ~cs_call_point(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_point)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_point)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//确认地主
class sc_land_result : public msg_object_base
{
public:
    char m_landload;//地主座位,如果大于2标识流局
    char m_point;//最后叫分值
    sc_land_result()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_LAND_RESULT;
    }
    ~sc_land_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_landload)) return false;
        if(!bs.read(m_point)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_landload)) return false;
        if(!bs.write(m_point)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//下发底牌
class land_back_cards : public msg_object_base
{
public:
    char m_landload;//地主座位,如果大于2标识流局
    char m_backcards[3];//底牌
    land_back_cards()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_LAND_BACK_CARDS;
    }
    ~land_back_cards(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_landload)) return false;
        if(!bs.read(m_backcards)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_landload)) return false;
        if(!bs.write(m_backcards)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//通知用户加倍
class sc_call_double : public msg_object_base
{
public:
    char m_seat;//座位ID
    sc_call_double()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_GAME_CALL_DOUBLE;
    }
    ~sc_call_double(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_seat)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_seat)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//加倍结果,客户端服务器共用协议
class double_result : public msg_object_base
{
public:
    char m_seat;//座位ID
    char m_value;//加倍值
    double_result()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_GAME_DOUBLE_RESULT;
    }
    ~double_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_seat)) return false;
        if(!bs.read(m_value)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_seat)) return false;
        if(!bs.write(m_value)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//通知出牌
class sc_out_card_result : public msg_object_base
{
public:
    char m_operator;//当前操作人座位
    char m_lastoperator;//上家操作人座位
    binary<20> m_lastcards;//上家出牌,如果个数为0标示PASS
    char m_remaincards;//上家剩余牌
    char m_cardtype;//牌型
    char m_nlz;//癞子个数
    char m_slz[4];//癞子原牌
    char m_dlz[4];//替换牌
    char m_state;//出牌状态：1代打，0不代打
    sc_out_card_result()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_GAME_OUT_CARD_RESULT;
    }
    ~sc_out_card_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_operator)) return false;
        if(!bs.read(m_lastoperator)) return false;
        if(!m_lastcards.serialize_in(bs)) return false;
        if(!bs.read(m_remaincards)) return false;
        if(!bs.read(m_cardtype)) return false;
        if(!bs.read(m_nlz)) return false;
        if(!bs.read(m_slz)) return false;
        if(!bs.read(m_dlz)) return false;
        if(!bs.read(m_state)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_operator)) return false;
        if(!bs.write(m_lastoperator)) return false;
        if(!m_lastcards.serialize_out(bs)) return false;
        if(!bs.write(m_remaincards)) return false;
        if(!bs.write(m_cardtype)) return false;
        if(!bs.write(m_nlz)) return false;
        if(!bs.write(m_slz)) return false;
        if(!bs.write(m_dlz)) return false;
        if(!bs.write(m_state)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//玩家出牌
class cs_out_cards : public msg_object_base
{
public:
    binary<20> m_cards;//出牌数据,长度0标示pass
    char m_cardtype;//牌型
    char m_nlz;//癞子个数
    char m_slz[4];//癞子原牌
    char m_dlz[4];//替换牌
    cs_out_cards()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_GAME_OUT_CARD;
    }
    ~cs_out_cards(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_cards.serialize_in(bs)) return false;
        if(!bs.read(m_cardtype)) return false;
        if(!bs.read(m_nlz)) return false;
        if(!bs.read(m_slz)) return false;
        if(!bs.read(m_dlz)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_cards.serialize_out(bs)) return false;
        if(!bs.write(m_cardtype)) return false;
        if(!bs.write(m_nlz)) return false;
        if(!bs.write(m_slz)) return false;
        if(!bs.write(m_dlz)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//出牌错误提示
class sc_out_card_err : public msg_object_base
{
public:
    char m_reason;//错误类型: 1:非法牌型, 2:非法操作人
    binary<20> m_cards;//玩家手牌
    sc_out_card_err()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_GAME_OUT_CARD_ERR;
    }
    ~sc_out_card_err(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_reason)) return false;
        if(!m_cards.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_reason)) return false;
        if(!m_cards.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//游戏结束
class sc_game_end : public msg_object_base
{
public:
    char m_first;//第一个出完牌的玩家
    char m_remain1;//剩余玩家1
    char m_remain2;//剩余玩家2
    binary<20> m_remaincards1;//剩余玩家1的牌
    binary<20> m_remaincards2;//剩余玩家2的牌
    long long m_score[3];//积分
    long long m_glory[3];//荣誉
    char m_spring;//春天标识
    char m_bomb;//炸弹个数
    char m_callpoint;//叫分
    char m_double[3];//加倍
    char m_reason;//结算原因
    sc_game_end()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_GAME_END;
    }
    ~sc_game_end(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_first)) return false;
        if(!bs.read(m_remain1)) return false;
        if(!bs.read(m_remain2)) return false;
        if(!m_remaincards1.serialize_in(bs)) return false;
        if(!m_remaincards2.serialize_in(bs)) return false;
        if(!bs.read(m_score)) return false;
        if(!bs.read(m_glory)) return false;
        if(!bs.read(m_spring)) return false;
        if(!bs.read(m_bomb)) return false;
        if(!bs.read(m_callpoint)) return false;
        if(!bs.read(m_double)) return false;
        if(!bs.read(m_reason)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_first)) return false;
        if(!bs.write(m_remain1)) return false;
        if(!bs.write(m_remain2)) return false;
        if(!m_remaincards1.serialize_out(bs)) return false;
        if(!m_remaincards2.serialize_out(bs)) return false;
        if(!bs.write(m_score)) return false;
        if(!bs.write(m_glory)) return false;
        if(!bs.write(m_spring)) return false;
        if(!bs.write(m_bomb)) return false;
        if(!bs.write(m_callpoint)) return false;
        if(!bs.write(m_double)) return false;
        if(!bs.write(m_reason)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//断线重连状态
class sc_recome_state : public msg_object_base
{
public:
    int m_matchid;//比赛ID
    int m_tableid;//桌子ID
    unsigned int m_seats[3];//三个用户ID
    char m_state;//游戏状态:1-叫分, 2-农民加倍, 3-地址加倍阶段, 4-游戏中
    sc_recome_state()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_RECOME_STATE;
    }
    ~sc_recome_state(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_matchid)) return false;
        if(!bs.read(m_tableid)) return false;
        if(!bs.read(m_seats)) return false;
        if(!bs.read(m_state)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_matchid)) return false;
        if(!bs.write(m_tableid)) return false;
        if(!bs.write(m_seats)) return false;
        if(!bs.write(m_state)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//叫分阶段重连场景信息
class sc_recome_state_call_point : public msg_object_base
{
public:
    binary<20> m_cards;//手牌
    char m_call[3];//当前玩家已经叫分值
    char m_operator;//当前操作人
    sc_recome_state_call_point()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_RECOME_STATE_CALL_POINT;
    }
    ~sc_recome_state_call_point(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_cards.serialize_in(bs)) return false;
        if(!bs.read(m_call)) return false;
        if(!bs.read(m_operator)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_cards.serialize_out(bs)) return false;
        if(!bs.write(m_call)) return false;
        if(!bs.write(m_operator)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//加倍阶段断线重连场景信息
class sc_recome_state_call_double : public msg_object_base
{
public:
    char m_landload;//地主位置
    char m_point;//地主叫分
    binary<20> m_cards;//手牌
    char m_backcards[3];//底牌
    char m_double[3];//当前加倍情况
    char m_operator;//当前操作人
    sc_recome_state_call_double()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_RECOME_STATE_CALL_DOUBLE;
    }
    ~sc_recome_state_call_double(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_landload)) return false;
        if(!bs.read(m_point)) return false;
        if(!m_cards.serialize_in(bs)) return false;
        if(!bs.read(m_backcards)) return false;
        if(!bs.read(m_double)) return false;
        if(!bs.read(m_operator)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_landload)) return false;
        if(!bs.write(m_point)) return false;
        if(!m_cards.serialize_out(bs)) return false;
        if(!bs.write(m_backcards)) return false;
        if(!bs.write(m_double)) return false;
        if(!bs.write(m_operator)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//出牌阶段断线重连场景信息
class sc_recome_state_outcards : public msg_object_base
{
public:
    char m_landload;//地主位置
    char m_point;//地主叫分
    binary<20> m_cards;//手牌
    char m_backcards[3];//底牌
    char m_lz;//癞子
    char m_remain[3];//剩余牌数
    char m_double[3];//当前加倍情况
    char m_bomb;//当前炸弹个数
    char m_lastoperator;//上次出牌人
    binary<20> m_lastcards;//上次出牌
    char m_lasttype;//上次出牌类型
    char m_cardtype;//牌型
    char m_nlz;//癞子个数
    char m_slz[4];//癞子原牌
    char m_dlz[4];//替换牌
    char m_operator;//当前操作人,如果上次出牌人是当前出牌人的下家,那么标示当前出牌人的上家是PASS
    sc_recome_state_outcards()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_RECOME_STATE_OUTCARDS;
    }
    ~sc_recome_state_outcards(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_landload)) return false;
        if(!bs.read(m_point)) return false;
        if(!m_cards.serialize_in(bs)) return false;
        if(!bs.read(m_backcards)) return false;
        if(!bs.read(m_lz)) return false;
        if(!bs.read(m_remain)) return false;
        if(!bs.read(m_double)) return false;
        if(!bs.read(m_bomb)) return false;
        if(!bs.read(m_lastoperator)) return false;
        if(!m_lastcards.serialize_in(bs)) return false;
        if(!bs.read(m_lasttype)) return false;
        if(!bs.read(m_cardtype)) return false;
        if(!bs.read(m_nlz)) return false;
        if(!bs.read(m_slz)) return false;
        if(!bs.read(m_dlz)) return false;
        if(!bs.read(m_operator)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_landload)) return false;
        if(!bs.write(m_point)) return false;
        if(!m_cards.serialize_out(bs)) return false;
        if(!bs.write(m_backcards)) return false;
        if(!bs.write(m_lz)) return false;
        if(!bs.write(m_remain)) return false;
        if(!bs.write(m_double)) return false;
        if(!bs.write(m_bomb)) return false;
        if(!bs.write(m_lastoperator)) return false;
        if(!m_lastcards.serialize_out(bs)) return false;
        if(!bs.write(m_lasttype)) return false;
        if(!bs.write(m_cardtype)) return false;
        if(!bs.write(m_nlz)) return false;
        if(!bs.write(m_slz)) return false;
        if(!bs.write(m_dlz)) return false;
        if(!bs.write(m_operator)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//修改托管状态,服务器客户端共用
class change_auto_state : public msg_object_base
{
public:
    unsigned int m_userid;//用户ID
    char m_tag;//托管状态: 1-托管  0-取消托管
    change_auto_state()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_USER_AUTO_STATE;
    }
    ~change_auto_state(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_userid)) return false;
        if(!bs.read(m_tag)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_userid)) return false;
        if(!bs.write(m_tag)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//下发癞子牌型
class mn_lz_card : public msg_object_base
{
public:
    char m_lz;//癞子牌
    mn_lz_card()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_LZ_CARD;
    }
    ~mn_lz_card(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_lz)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_lz)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//下发用户明牌
class mn_call_mingpai : public msg_object_base
{
public:
    mn_call_mingpai()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_CALL_MINGPAI;
    }
    ~mn_call_mingpai(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//下发用户明牌
class mn_mingpai_card : public msg_object_base
{
public:
    char m_seatid;//座位ID
    binary<20> m_cards;//手牌
    mn_mingpai_card()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_MINGPAI_CARD;
    }
    ~mn_mingpai_card(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_seatid)) return false;
        if(!m_cards.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_seatid)) return false;
        if(!m_cards.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//用户状态
class nm_user_state : public msg_object_base
{
public:
    char m_auto[3];//托管状态
    char m_net_cut[3];//断线状态
    nm_user_state()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = NM_USER_STATE;
    }
    ~nm_user_state(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_auto)) return false;
        if(!bs.read(m_net_cut)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_auto)) return false;
        if(!bs.write(m_net_cut)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//同步已经打出的牌，断线重连后下发
class mn_syn_outcard : public msg_object_base
{
public:
    binary<20> m_sit0;//上次出牌
    binary<20> m_sit1;//上次出牌
    binary<20> m_sit2;//上次出牌
    mn_syn_outcard()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_SYN_OUTCARD;
    }
    ~mn_syn_outcard(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_sit0.serialize_in(bs)) return false;
        if(!m_sit1.serialize_in(bs)) return false;
        if(!m_sit2.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_sit0.serialize_out(bs)) return false;
        if(!m_sit1.serialize_out(bs)) return false;
        if(!m_sit2.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class sc_syn_coin : public msg_object_base
{
public:
    unsigned int m_userid;//用户ID
    long long m_coin;//更新的金币数量
    sc_syn_coin()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_SYN_COIN;
    }
    ~sc_syn_coin(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_userid)) return false;
        if(!bs.read(m_coin)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_userid)) return false;
        if(!bs.write(m_coin)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//设置叫分人
class sc_set_call_point : public msg_object_base
{
public:
    char m_seatid;//座位ID
    char m_min_point;//可叫的最小分
    sc_set_call_point()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_SET_CALL_POINT;
    }
    ~sc_set_call_point(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_seatid)) return false;
        if(!bs.read(m_min_point)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_seatid)) return false;
        if(!bs.write(m_min_point)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//牌局任务描述，发牌后下发
class sc_card_mission_info : public msg_object_base
{
public:
    ustring<256,unsigned short> m_desc;//任务描述
    ustring<256,unsigned short> m_reward;//奖励描述
    sc_card_mission_info()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_CARD_MISSION_INFO;
    }
    ~sc_card_mission_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_desc.serialize_in(bs)) return false;
        if(!m_reward.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_desc.serialize_out(bs)) return false;
        if(!m_reward.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//牌局任务奖励，结算后，完成时发，没完成不发
class sc_card_mission_complish : public msg_object_base
{
public:
    ustring<256,unsigned short> m_msg;//
    long long m_coin;//
    mvector<PropItem,5> m_prop;//道具奖励
    sc_card_mission_complish()
    {
        m_major_id = MJ_GAME_LOGIC_DDZ;
        m_minor_id = MN_CARD_MISSION_COMPLISH;
    }
    ~sc_card_mission_complish(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_msg.serialize_in(bs)) return false;
        if(!bs.read(m_coin)) return false;
        if(!m_prop.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_msg.serialize_out(bs)) return false;
        if(!bs.write(m_coin)) return false;
        if(!m_prop.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//系统消息
class sc_system_message : public msg_object_base
{
public:
    char m_type;//消息类型
    ustring<64,unsigned short> m_title;//标题
    ustring<512,unsigned short> m_content;//消息内容
    sc_system_message()
    {
        m_major_id = MJ_MESSAGE_DDZ_GAME;
        m_minor_id = MN_SYSTEM_MESSAGE;
    }
    ~sc_system_message(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        if(!m_title.serialize_in(bs)) return false;
        if(!m_content.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        if(!m_title.serialize_out(bs)) return false;
        if(!m_content.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//游戏消息
class sc_game_message : public msg_object_base
{
public:
    char m_type;//消息类型
    ustring<512,unsigned short> m_content;//消息内容
    sc_game_message()
    {
        m_major_id = MJ_MESSAGE_DDZ_GAME;
        m_minor_id = MN_GAME_MESSAGE;
    }
    ~sc_game_message(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        if(!m_content.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        if(!m_content.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//聊天消息
class sc_chat_message : public msg_object_base
{
public:
    unsigned int m_userid;//用户ID
    ustring<512,unsigned short> m_content;//消息内容
    sc_chat_message()
    {
        m_major_id = MJ_MESSAGE_DDZ_GAME;
        m_minor_id = MN_CHAT_MESSAGE;
    }
    ~sc_chat_message(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_userid)) return false;
        if(!m_content.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_userid)) return false;
        if(!m_content.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//选择模式
class sc_select_mode : public msg_object_base
{
public:
    char m_type;//PK类型，0创建，1加入
    char m_code[6];//加入桌子邀请码，创建时忽略
    sc_select_mode()
    {
        m_major_id = MJ_PK_GAME;
        m_minor_id = MN_SELECT_MODE;
    }
    ~sc_select_mode(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        if(!bs.read(m_code)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        if(!bs.write(m_code)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//创建返回
class sc_create_pk_result : public msg_object_base
{
public:
    char m_result;//0成功，1失败
    char m_code[6];//创建桌子邀请码
    sc_create_pk_result()
    {
        m_major_id = MJ_PK_GAME;
        m_minor_id = MN_CREATE_PK_RESULT;
    }
    ~sc_create_pk_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!bs.read(m_code)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!bs.write(m_code)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//加入返回结果
class sc_add_pk_result : public msg_object_base
{
public:
    char m_result;//0成功，1失败
    ustring<64,unsigned short> m_msg;//失败消息
    sc_add_pk_result()
    {
        m_major_id = MJ_PK_GAME;
        m_minor_id = MN_ADD_PK_RESULT;
    }
    ~sc_add_pk_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//设置PK分数，只有房主可以设置
class sc_set_pk_point : public msg_object_base
{
public:
    int m_point;//
    sc_set_pk_point()
    {
        m_major_id = MJ_PK_GAME;
        m_minor_id = MN_SET_PK_POINT;
    }
    ~sc_set_pk_point(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_point)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_point)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//设置PK分数结果
class sc_set_pk_point_result : public msg_object_base
{
public:
    long long m_point;//
    sc_set_pk_point_result()
    {
        m_major_id = MJ_PK_GAME;
        m_minor_id = MN_SET_PK_POINT_RESULT;
    }
    ~sc_set_pk_point_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_point)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_point)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//确认PK分数设置
class sc_agree_pk_point : public msg_object_base
{
public:
    char m_type;//1确认，0拒绝。当拒绝时，该协议发给房主，重新设置
    sc_agree_pk_point()
    {
        m_major_id = MJ_PK_GAME;
        m_minor_id = MN_AGREE_SET_PK_POINT;
    }
    ~sc_agree_pk_point(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//投降
class sc_giveup : public msg_object_base
{
public:
    sc_giveup()
    {
        m_major_id = MJ_PK_GAME;
        m_minor_id = MN_GIVEUP;
    }
    ~sc_giveup(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//比赛结束
class sc_pk_end : public msg_object_base
{
public:
    ustring<64,unsigned short> m_msg;//结束原因
    sc_pk_end()
    {
        m_major_id = MJ_PK_GAME;
        m_minor_id = MN_PK_END;
    }
    ~sc_pk_end(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//修正PK分值，当游戏结束时，重新下发PK值，因为有人会比PK值低
class sc_pk_point_adjust : public msg_object_base
{
public:
    long long m_point;//
    sc_pk_point_adjust()
    {
        m_major_id = MJ_PK_GAME;
        m_minor_id = MN_PK_POINT_ADJUST;
    }
    ~sc_pk_point_adjust(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_point)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_point)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//计时数据请求
class sc_count_cfg_result : public msg_object_base
{
public:
    int m_time_len;//目标时长
    int m_pid;//道具ID
    int m_pn;//道具数量
    long long m_coin;//货币数量
    int m_seconds;//已计秒数
    sc_count_cfg_result()
    {
        m_major_id = MJ_MISSION_DDZ_SERVER;
        m_minor_id = MN_TIME_COUNT_CFG_RESULT;
    }
    ~sc_count_cfg_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_time_len)) return false;
        if(!bs.read(m_pid)) return false;
        if(!bs.read(m_pn)) return false;
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_seconds)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_time_len)) return false;
        if(!bs.write(m_pid)) return false;
        if(!bs.write(m_pn)) return false;
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_seconds)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//领奖
class cs_get_award : public msg_object_base
{
public:
    cs_get_award()
    {
        m_major_id = MJ_MISSION_DDZ_SERVER;
        m_minor_id = MN_GET_AWARD;
    }
    ~cs_get_award(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//领奖
class sc_get_award_result : public msg_object_base
{
public:
    char m_result;//领奖0成功，1失败
    ustring<64,unsigned short> m_msg;//领奖描述
    sc_get_award_result()
    {
        m_major_id = MJ_MISSION_DDZ_SERVER;
        m_minor_id = MN_GET_AWARD_RESULT;
    }
    ~sc_get_award_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//游戏中奖励通知，服务器主动下发，客户端不请求
class sc_game_award_info : public msg_object_base
{
public:
    unsigned int m_uid;//
    int m_icon;//
    ustring<32,unsigned short> m_name;//
    int m_num;//
    sc_game_award_info()
    {
        m_major_id = MJ_MISSION_DDZ_SERVER;
        m_minor_id = MN_GAME_AWARD_INFO;
    }
    ~sc_game_award_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_uid)) return false;
        if(!bs.read(m_icon)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!bs.read(m_num)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_uid)) return false;
        if(!bs.write(m_icon)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!bs.write(m_num)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//等待界面，翻页排名时发送，如果当前页等于最大页时，不再请求
class cs_get_rank_info : public msg_object_base
{
public:
    short m_page;//请求页码
    cs_get_rank_info()
    {
        m_major_id = MJ_MATCH_DDZ;
        m_minor_id = MN_GET_RANK_INFO;
    }
    ~cs_get_rank_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_page)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_page)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//赛况信息,等待界面的用户只发首页，用户翻页时再请求后续片段
class sc_match_rank_info : public msg_object_base
{
public:
    RankInfo m_myrank;//自己的排名
    int m_users;//当前参赛用户数
    mvector<RankInfo,30> m_rank;//
    short m_all_page;//排名总页
    short m_page;//当前页
    int m_time_span;//比赛当前持续时长
    int m_all_turns;//比赛总轮次
    int m_current_turns;//当前轮次
    int m_gaming_desks;//还在游戏中的桌子数
    sc_match_rank_info()
    {
        m_major_id = MJ_MATCH_DDZ;
        m_minor_id = MN_MATCH_RANK_INFO;
    }
    ~sc_match_rank_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_myrank.serialize_in(bs)) return false;
        if(!bs.read(m_users)) return false;
        if(!m_rank.serialize_in(bs)) return false;
        if(!bs.read(m_all_page)) return false;
        if(!bs.read(m_page)) return false;
        if(!bs.read(m_time_span)) return false;
        if(!bs.read(m_all_turns)) return false;
        if(!bs.read(m_current_turns)) return false;
        if(!bs.read(m_gaming_desks)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_myrank.serialize_out(bs)) return false;
        if(!bs.write(m_users)) return false;
        if(!m_rank.serialize_out(bs)) return false;
        if(!bs.write(m_all_page)) return false;
        if(!bs.write(m_page)) return false;
        if(!bs.write(m_time_span)) return false;
        if(!bs.write(m_all_turns)) return false;
        if(!bs.write(m_current_turns)) return false;
        if(!bs.write(m_gaming_desks)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//比赛结束通知
class sc_match_end : public msg_object_base
{
public:
    int m_rank;//名次
    int m_coin;//金币奖励
    mvector<PropItem,5> m_prop;//道具奖励
    sc_match_end()
    {
        m_major_id = MJ_MATCH_DDZ;
        m_minor_id = MN_MATCH_END;
    }
    ~sc_match_end(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_rank)) return false;
        if(!bs.read(m_coin)) return false;
        if(!m_prop.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_rank)) return false;
        if(!bs.write(m_coin)) return false;
        if(!m_prop.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//通知用户等待开始
class sc_match_waiting : public msg_object_base
{
public:
    sc_match_waiting()
    {
        m_major_id = MJ_MATCH_DDZ;
        m_minor_id = MN_MATCH_WAITING;
    }
    ~sc_match_waiting(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//比赛名次发生变化(只有本桌用户)
class sc_rank_change : public msg_object_base
{
public:
    short m_all;//总人数
    mvector<DeskRankItem,3> m_rank;//名次信息
    ustring<64,unsigned short> m_desc;//赛况描述
    sc_rank_change()
    {
        m_major_id = MJ_MATCH_DDZ;
        m_minor_id = MN_RANK_CHANGE;
    }
    ~sc_rank_change(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_all)) return false;
        if(!m_rank.serialize_in(bs)) return false;
        if(!m_desc.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_all)) return false;
        if(!m_rank.serialize_out(bs)) return false;
        if(!m_desc.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//比赛底分，发牌前发送
class sc_match_base : public msg_object_base
{
public:
    int m_base;//
    sc_match_base()
    {
        m_major_id = MJ_MATCH_DDZ;
        m_minor_id = MN_MATCH_BASE;
    }
    ~sc_match_base(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_base)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_base)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//登录比赛
class cs_login_match : public msg_object_base
{
public:
    ustring<64,unsigned short> m_openid;//开放ID
    int m_sn;//比赛序列号
    cs_login_match()
    {
        m_major_id = MJ_MATCH_DDZ;
        m_minor_id = MN_LOGIN_MATCH;
    }
    ~cs_login_match(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_sn)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_sn)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

/*---------------------------------------- ----------------------------------------*/
#define DDZPROT_MAKE_CLASS(classname)\
{\
    if(buf) ret = new (buf)classname();\
    else ret = new classname();\
    if(ret && !ret->serialize_in(pdata, len))\
    {\
        if(!buf) delete ret;\
        ret = 0;\
    }\
}

class ddzprotObjectManager
{
public:
    static msg_object_base* CreateObject(char* pdata, int len, char* buf = 0)
    {
        msg_object_base* ret=0;
        msg_object_head* head = (msg_object_head*)pdata;
        switch(head->m_major_id)
        {
            case MJ_GAME_SERVER:
            {
                switch(head->m_minor_id)
                {
                    case MN_PING:
                        DDZPROT_MAKE_CLASS(mn_ping);
                        break;
                    case MN_PONG:
                        DDZPROT_MAKE_CLASS(mn_pong);
                        break;
                    case MN_LOGIN:
                        DDZPROT_MAKE_CLASS(cs_login);
                        break;
                    case MN_LOGIN_RESULT:
                        DDZPROT_MAKE_CLASS(sc_login_result);
                        break;
                    case MN_CUT_OFF:
                        DDZPROT_MAKE_CLASS(mn_cut_off);
                        break;
                }
            }
            break;
            case MJ_GAME_CONTROL_DDZ:
            {
                switch(head->m_minor_id)
                {
                    case MN_GAME_USER_SITDOWN:
                        DDZPROT_MAKE_CLASS(game_user_sitdown);
                        break;
                    case MN_GAME_USER_SITDOWN_SC:
                        DDZPROT_MAKE_CLASS(game_user_sitdown_sc);
                        break;
                    case MN_GAME_USER_SITDOWN_ERR:
                        DDZPROT_MAKE_CLASS(game_user_sitdown_err);
                        break;
                    case MN_LEFT_DESK_REQUEST:
                        DDZPROT_MAKE_CLASS(left_desk_request);
                        break;
                    case MN_USER_LEFT_DESK:
                        DDZPROT_MAKE_CLASS(user_left_desk);
                        break;
                    case MN_USER_NET_CUT:
                        DDZPROT_MAKE_CLASS(sc_user_net_cut);
                        break;
                    case MN_USER_RECOME:
                        DDZPROT_MAKE_CLASS(sc_user_recome);
                        break;
                    case MN_USER_READY:
                        DDZPROT_MAKE_CLASS(sc_user_ready);
                        break;
                    case MN_FORCE_USER_LEFT_DESK:
                        DDZPROT_MAKE_CLASS(sc_force_user_left_desk);
                        break;
                    case MN_BANKRUPT_RESPONST:
                        DDZPROT_MAKE_CLASS(sc_bankrupt_responst);
                        break;
                    case MN_ARRANGE_DESK:
                        DDZPROT_MAKE_CLASS(sc_arrange_desk);
                        break;
                }
            }
            break;
            case MJ_GAME_LOGIC_DDZ:
            {
                switch(head->m_minor_id)
                {
                    case MN_GAME_CONFIG:
                        DDZPROT_MAKE_CLASS(sc_game_config);
                        break;
                    case MN_DEAL:
                        DDZPROT_MAKE_CLASS(sc_deal);
                        break;
                    case MN_CALL_POINT:
                        DDZPROT_MAKE_CLASS(sc_call_point_result);
                        break;
                    case MN_USER_CALL_POINT:
                        DDZPROT_MAKE_CLASS(cs_call_point);
                        break;
                    case MN_LAND_RESULT:
                        DDZPROT_MAKE_CLASS(sc_land_result);
                        break;
                    case MN_LAND_BACK_CARDS:
                        DDZPROT_MAKE_CLASS(land_back_cards);
                        break;
                    case MN_GAME_CALL_DOUBLE:
                        DDZPROT_MAKE_CLASS(sc_call_double);
                        break;
                    case MN_GAME_DOUBLE_RESULT:
                        DDZPROT_MAKE_CLASS(double_result);
                        break;
                    case MN_GAME_OUT_CARD_RESULT:
                        DDZPROT_MAKE_CLASS(sc_out_card_result);
                        break;
                    case MN_GAME_OUT_CARD:
                        DDZPROT_MAKE_CLASS(cs_out_cards);
                        break;
                    case MN_GAME_OUT_CARD_ERR:
                        DDZPROT_MAKE_CLASS(sc_out_card_err);
                        break;
                    case MN_GAME_END:
                        DDZPROT_MAKE_CLASS(sc_game_end);
                        break;
                    case MN_RECOME_STATE:
                        DDZPROT_MAKE_CLASS(sc_recome_state);
                        break;
                    case MN_RECOME_STATE_CALL_POINT:
                        DDZPROT_MAKE_CLASS(sc_recome_state_call_point);
                        break;
                    case MN_RECOME_STATE_CALL_DOUBLE:
                        DDZPROT_MAKE_CLASS(sc_recome_state_call_double);
                        break;
                    case MN_RECOME_STATE_OUTCARDS:
                        DDZPROT_MAKE_CLASS(sc_recome_state_outcards);
                        break;
                    case MN_USER_AUTO_STATE:
                        DDZPROT_MAKE_CLASS(change_auto_state);
                        break;
                    case MN_LZ_CARD:
                        DDZPROT_MAKE_CLASS(mn_lz_card);
                        break;
                    case MN_CALL_MINGPAI:
                        DDZPROT_MAKE_CLASS(mn_call_mingpai);
                        break;
                    case MN_MINGPAI_CARD:
                        DDZPROT_MAKE_CLASS(mn_mingpai_card);
                        break;
                    case NM_USER_STATE:
                        DDZPROT_MAKE_CLASS(nm_user_state);
                        break;
                    case MN_SYN_OUTCARD:
                        DDZPROT_MAKE_CLASS(mn_syn_outcard);
                        break;
                    case MN_SYN_COIN:
                        DDZPROT_MAKE_CLASS(sc_syn_coin);
                        break;
                    case MN_SET_CALL_POINT:
                        DDZPROT_MAKE_CLASS(sc_set_call_point);
                        break;
                    case MN_CARD_MISSION_INFO:
                        DDZPROT_MAKE_CLASS(sc_card_mission_info);
                        break;
                    case MN_CARD_MISSION_COMPLISH:
                        DDZPROT_MAKE_CLASS(sc_card_mission_complish);
                        break;
                }
            }
            break;
            case MJ_MESSAGE_DDZ_GAME:
            {
                switch(head->m_minor_id)
                {
                    case MN_SYSTEM_MESSAGE:
                        DDZPROT_MAKE_CLASS(sc_system_message);
                        break;
                    case MN_GAME_MESSAGE:
                        DDZPROT_MAKE_CLASS(sc_game_message);
                        break;
                    case MN_CHAT_MESSAGE:
                        DDZPROT_MAKE_CLASS(sc_chat_message);
                        break;
                }
            }
            break;
            case MJ_PK_GAME:
            {
                switch(head->m_minor_id)
                {
                    case MN_SELECT_MODE:
                        DDZPROT_MAKE_CLASS(sc_select_mode);
                        break;
                    case MN_CREATE_PK_RESULT:
                        DDZPROT_MAKE_CLASS(sc_create_pk_result);
                        break;
                    case MN_ADD_PK_RESULT:
                        DDZPROT_MAKE_CLASS(sc_add_pk_result);
                        break;
                    case MN_SET_PK_POINT:
                        DDZPROT_MAKE_CLASS(sc_set_pk_point);
                        break;
                    case MN_SET_PK_POINT_RESULT:
                        DDZPROT_MAKE_CLASS(sc_set_pk_point_result);
                        break;
                    case MN_AGREE_SET_PK_POINT:
                        DDZPROT_MAKE_CLASS(sc_agree_pk_point);
                        break;
                    case MN_GIVEUP:
                        DDZPROT_MAKE_CLASS(sc_giveup);
                        break;
                    case MN_PK_END:
                        DDZPROT_MAKE_CLASS(sc_pk_end);
                        break;
                    case MN_PK_POINT_ADJUST:
                        DDZPROT_MAKE_CLASS(sc_pk_point_adjust);
                        break;
                }
            }
            break;
            case MJ_MISSION_DDZ_SERVER:
            {
                switch(head->m_minor_id)
                {
                    case MN_TIME_COUNT_CFG_RESULT:
                        DDZPROT_MAKE_CLASS(sc_count_cfg_result);
                        break;
                    case MN_GET_AWARD:
                        DDZPROT_MAKE_CLASS(cs_get_award);
                        break;
                    case MN_GET_AWARD_RESULT:
                        DDZPROT_MAKE_CLASS(sc_get_award_result);
                        break;
                    case MN_GAME_AWARD_INFO:
                        DDZPROT_MAKE_CLASS(sc_game_award_info);
                        break;
                }
            }
            break;
            case MJ_MATCH_DDZ:
            {
                switch(head->m_minor_id)
                {
                    case MN_GET_RANK_INFO:
                        DDZPROT_MAKE_CLASS(cs_get_rank_info);
                        break;
                    case MN_MATCH_RANK_INFO:
                        DDZPROT_MAKE_CLASS(sc_match_rank_info);
                        break;
                    case MN_MATCH_END:
                        DDZPROT_MAKE_CLASS(sc_match_end);
                        break;
                    case MN_MATCH_WAITING:
                        DDZPROT_MAKE_CLASS(sc_match_waiting);
                        break;
                    case MN_RANK_CHANGE:
                        DDZPROT_MAKE_CLASS(sc_rank_change);
                        break;
                    case MN_MATCH_BASE:
                        DDZPROT_MAKE_CLASS(sc_match_base);
                        break;
                    case MN_LOGIN_MATCH:
                        DDZPROT_MAKE_CLASS(cs_login_match);
                        break;
                }
            }
            break;
        }
        return ret;
    }
    static void query_id(short mid, short nid, char* szMid, char* szNid)
    {
        szMid[0] = 0;
        szNid[0] = 0;
        switch(mid)
        {
            case MJ_GAME_SERVER:
            {
                switch(nid)
                {
                    case MN_PING: strcpy(szMid, "MJ_GAME_SERVER"); strcpy(szNid, "MN_PING"); break;
                    case MN_PONG: strcpy(szMid, "MJ_GAME_SERVER"); strcpy(szNid, "MN_PONG"); break;
                    case MN_LOGIN: strcpy(szMid, "MJ_GAME_SERVER"); strcpy(szNid, "MN_LOGIN"); break;
                    case MN_LOGIN_RESULT: strcpy(szMid, "MJ_GAME_SERVER"); strcpy(szNid, "MN_LOGIN_RESULT"); break;
                    case MN_CUT_OFF: strcpy(szMid, "MJ_GAME_SERVER"); strcpy(szNid, "MN_CUT_OFF"); break;
                }
            }
            break;
            case MJ_GAME_CONTROL_DDZ:
            {
                switch(nid)
                {
                    case MN_GAME_USER_SITDOWN: strcpy(szMid, "MJ_GAME_CONTROL_DDZ"); strcpy(szNid, "MN_GAME_USER_SITDOWN"); break;
                    case MN_GAME_USER_SITDOWN_SC: strcpy(szMid, "MJ_GAME_CONTROL_DDZ"); strcpy(szNid, "MN_GAME_USER_SITDOWN_SC"); break;
                    case MN_GAME_USER_SITDOWN_ERR: strcpy(szMid, "MJ_GAME_CONTROL_DDZ"); strcpy(szNid, "MN_GAME_USER_SITDOWN_ERR"); break;
                    case MN_LEFT_DESK_REQUEST: strcpy(szMid, "MJ_GAME_CONTROL_DDZ"); strcpy(szNid, "MN_LEFT_DESK_REQUEST"); break;
                    case MN_USER_LEFT_DESK: strcpy(szMid, "MJ_GAME_CONTROL_DDZ"); strcpy(szNid, "MN_USER_LEFT_DESK"); break;
                    case MN_USER_NET_CUT: strcpy(szMid, "MJ_GAME_CONTROL_DDZ"); strcpy(szNid, "MN_USER_NET_CUT"); break;
                    case MN_USER_RECOME: strcpy(szMid, "MJ_GAME_CONTROL_DDZ"); strcpy(szNid, "MN_USER_RECOME"); break;
                    case MN_USER_READY: strcpy(szMid, "MJ_GAME_CONTROL_DDZ"); strcpy(szNid, "MN_USER_READY"); break;
                    case MN_FORCE_USER_LEFT_DESK: strcpy(szMid, "MJ_GAME_CONTROL_DDZ"); strcpy(szNid, "MN_FORCE_USER_LEFT_DESK"); break;
                    case MN_BANKRUPT_RESPONST: strcpy(szMid, "MJ_GAME_CONTROL_DDZ"); strcpy(szNid, "MN_BANKRUPT_RESPONST"); break;
                    case MN_ARRANGE_DESK: strcpy(szMid, "MJ_GAME_CONTROL_DDZ"); strcpy(szNid, "MN_ARRANGE_DESK"); break;
                }
            }
            break;
            case MJ_GAME_LOGIC_DDZ:
            {
                switch(nid)
                {
                    case MN_GAME_CONFIG: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_GAME_CONFIG"); break;
                    case MN_DEAL: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_DEAL"); break;
                    case MN_CALL_POINT: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_CALL_POINT"); break;
                    case MN_USER_CALL_POINT: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_USER_CALL_POINT"); break;
                    case MN_LAND_RESULT: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_LAND_RESULT"); break;
                    case MN_LAND_BACK_CARDS: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_LAND_BACK_CARDS"); break;
                    case MN_GAME_CALL_DOUBLE: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_GAME_CALL_DOUBLE"); break;
                    case MN_GAME_DOUBLE_RESULT: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_GAME_DOUBLE_RESULT"); break;
                    case MN_GAME_OUT_CARD_RESULT: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_GAME_OUT_CARD_RESULT"); break;
                    case MN_GAME_OUT_CARD: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_GAME_OUT_CARD"); break;
                    case MN_GAME_OUT_CARD_ERR: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_GAME_OUT_CARD_ERR"); break;
                    case MN_GAME_END: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_GAME_END"); break;
                    case MN_RECOME_STATE: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_RECOME_STATE"); break;
                    case MN_RECOME_STATE_CALL_POINT: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_RECOME_STATE_CALL_POINT"); break;
                    case MN_RECOME_STATE_CALL_DOUBLE: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_RECOME_STATE_CALL_DOUBLE"); break;
                    case MN_RECOME_STATE_OUTCARDS: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_RECOME_STATE_OUTCARDS"); break;
                    case MN_USER_AUTO_STATE: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_USER_AUTO_STATE"); break;
                    case MN_LZ_CARD: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_LZ_CARD"); break;
                    case MN_CALL_MINGPAI: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_CALL_MINGPAI"); break;
                    case MN_MINGPAI_CARD: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_MINGPAI_CARD"); break;
                    case NM_USER_STATE: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "NM_USER_STATE"); break;
                    case MN_SYN_OUTCARD: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_SYN_OUTCARD"); break;
                    case MN_SYN_COIN: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_SYN_COIN"); break;
                    case MN_SET_CALL_POINT: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_SET_CALL_POINT"); break;
                    case MN_CARD_MISSION_INFO: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_CARD_MISSION_INFO"); break;
                    case MN_CARD_MISSION_COMPLISH: strcpy(szMid, "MJ_GAME_LOGIC_DDZ"); strcpy(szNid, "MN_CARD_MISSION_COMPLISH"); break;
                }
            }
            break;
            case MJ_MESSAGE_DDZ_GAME:
            {
                switch(nid)
                {
                    case MN_SYSTEM_MESSAGE: strcpy(szMid, "MJ_MESSAGE_DDZ_GAME"); strcpy(szNid, "MN_SYSTEM_MESSAGE"); break;
                    case MN_GAME_MESSAGE: strcpy(szMid, "MJ_MESSAGE_DDZ_GAME"); strcpy(szNid, "MN_GAME_MESSAGE"); break;
                    case MN_CHAT_MESSAGE: strcpy(szMid, "MJ_MESSAGE_DDZ_GAME"); strcpy(szNid, "MN_CHAT_MESSAGE"); break;
                }
            }
            break;
            case MJ_PK_GAME:
            {
                switch(nid)
                {
                    case MN_SELECT_MODE: strcpy(szMid, "MJ_PK_GAME"); strcpy(szNid, "MN_SELECT_MODE"); break;
                    case MN_CREATE_PK_RESULT: strcpy(szMid, "MJ_PK_GAME"); strcpy(szNid, "MN_CREATE_PK_RESULT"); break;
                    case MN_ADD_PK_RESULT: strcpy(szMid, "MJ_PK_GAME"); strcpy(szNid, "MN_ADD_PK_RESULT"); break;
                    case MN_SET_PK_POINT: strcpy(szMid, "MJ_PK_GAME"); strcpy(szNid, "MN_SET_PK_POINT"); break;
                    case MN_SET_PK_POINT_RESULT: strcpy(szMid, "MJ_PK_GAME"); strcpy(szNid, "MN_SET_PK_POINT_RESULT"); break;
                    case MN_AGREE_SET_PK_POINT: strcpy(szMid, "MJ_PK_GAME"); strcpy(szNid, "MN_AGREE_SET_PK_POINT"); break;
                    case MN_GIVEUP: strcpy(szMid, "MJ_PK_GAME"); strcpy(szNid, "MN_GIVEUP"); break;
                    case MN_PK_END: strcpy(szMid, "MJ_PK_GAME"); strcpy(szNid, "MN_PK_END"); break;
                    case MN_PK_POINT_ADJUST: strcpy(szMid, "MJ_PK_GAME"); strcpy(szNid, "MN_PK_POINT_ADJUST"); break;
                }
            }
            break;
            case MJ_MISSION_DDZ_SERVER:
            {
                switch(nid)
                {
                    case MN_TIME_COUNT_CFG_RESULT: strcpy(szMid, "MJ_MISSION_DDZ_SERVER"); strcpy(szNid, "MN_TIME_COUNT_CFG_RESULT"); break;
                    case MN_GET_AWARD: strcpy(szMid, "MJ_MISSION_DDZ_SERVER"); strcpy(szNid, "MN_GET_AWARD"); break;
                    case MN_GET_AWARD_RESULT: strcpy(szMid, "MJ_MISSION_DDZ_SERVER"); strcpy(szNid, "MN_GET_AWARD_RESULT"); break;
                    case MN_GAME_AWARD_INFO: strcpy(szMid, "MJ_MISSION_DDZ_SERVER"); strcpy(szNid, "MN_GAME_AWARD_INFO"); break;
                }
            }
            break;
            case MJ_MATCH_DDZ:
            {
                switch(nid)
                {
                    case MN_GET_RANK_INFO: strcpy(szMid, "MJ_MATCH_DDZ"); strcpy(szNid, "MN_GET_RANK_INFO"); break;
                    case MN_MATCH_RANK_INFO: strcpy(szMid, "MJ_MATCH_DDZ"); strcpy(szNid, "MN_MATCH_RANK_INFO"); break;
                    case MN_MATCH_END: strcpy(szMid, "MJ_MATCH_DDZ"); strcpy(szNid, "MN_MATCH_END"); break;
                    case MN_MATCH_WAITING: strcpy(szMid, "MJ_MATCH_DDZ"); strcpy(szNid, "MN_MATCH_WAITING"); break;
                    case MN_RANK_CHANGE: strcpy(szMid, "MJ_MATCH_DDZ"); strcpy(szNid, "MN_RANK_CHANGE"); break;
                    case MN_MATCH_BASE: strcpy(szMid, "MJ_MATCH_DDZ"); strcpy(szNid, "MN_MATCH_BASE"); break;
                    case MN_LOGIN_MATCH: strcpy(szMid, "MJ_MATCH_DDZ"); strcpy(szNid, "MN_LOGIN_MATCH"); break;
                }
            }
            break;
        }
    }
};
#endif
/*protocol maker V1.0 authed by msl*/