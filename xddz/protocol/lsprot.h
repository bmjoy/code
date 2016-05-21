/*本代码由工具生成，请勿手动修改，切记！切记！很重要，不要偷懒！*/
#ifndef __LSPROT_MAKE_BY_PT_26639_H__
#define __LSPROT_MAKE_BY_PT_26639_H__
#include "ustring.h"
#include "binary.h"
#include "msg_object_base.h"
#include "mvector.h"

#pragma warning(disable:4996)
/*
修订记录：
---2014-5-3------------------------------------------------------
1. 增加版本控制协议
---2014-5-2------------------------------------------------------
2. 增加局数任务协议族  ZHANG
1. 增加商城配置
*/

#define MAX_NAME_LEN             64  //最大名称长度
#define CURRENCE_COIN            1  //货币类型金币
#define CURRENCE_MONEY           2  //货币类型人民币
#define MISSION_UNDONE           0  //任务未完成状态
#define MISSION_GIFT             1  //任务可领奖状态
#define MISSION_DONE             2  //任务已领奖
#define GAME_LIST_HOT            0x01  //房间热标识
#define GAME_LIST_RECOMMEND      0x02  //推荐房间标识
#define LOGIN_FAIL_SYS_BUSY      1  //登录失败，系统忙
#define LOGIN_FAIL_PWD_ACCOUNT   2  //登录失败，帐号不存在或密码错误
#define LOGIN_FAIL_TOKEN_ERROR   3  //登录失败，令牌错误
#define LOGIN_FAIL_BAN_ACCOUNT   4  //帐号被封
#define GUEST_USER               0  //游客用户
#define REGIST_USER              1  //注册用户
#define BINDER_USER              2  //绑定用户
#define REG_FAIL_TYPE_REGISTED   2  //注册失败，存2014/5/27在已注册帐号
#define REG_FAIL_TYPE_BINDED     3  //注册失败，存在绑定帐号
#define REG_FAIL_TYPE_BUSY       4  //注册失败，系统忙
#define ELO_USER_MSG             0  //个人消息
#define ELO_SYSTEM_MSG           1  //系统消息
#define ELO_UPAY                 0  //upay短代
#define ELO_ZHIFUBAO             1  //支付宝
#define ELO_WANGYIN              2  //网银
#define ELO_PAY_IOS              3  //IOS支付
#define ELO_PAY_SKY              4  //斯凯支付
#define ELO_PAY_BAIDU            5  //百度支付
#define ELO_PAY_9U               6  //九幽支付
#define ELO_PAY_MM               7  //MM支付
#define ELO_PAY_WEIMI            8  //微米支付
#define ELO_PAY_SMS              10  //短信支付
#define EXCHANGE_DEFAULT         0  //普通道具兑换
#define EXCHANGE_HUAFEI          1  //话费兑换
#define EXCHANGE_SHIWU           2  //实物兑换
#define POP_EVERYIMES            0  //每次登陆弹出
#define POP_LOGINTIMES           1  //每日登陆前N次弹出
#define EXCHANGE_QUAN            1101  //兑换券ID
#define CARD_COUNTER_ID          1102  //记牌器ID
#define IS_HOT_LABEL(mask)       (mask & 0x01)  //热房间标识
#define IS_RECOMMOND_LABEL(mask) (mask & 0x02)  //推荐房间标识
#define IS_ANTICHEAT_LABEL(mask) (mask & 0x04)  //防作弊房间标识
#define IS_ACTIVITY_LABEL(mask)  (mask & 0x08)  //活动房间标识
#define IS_TICKET_LABEL(mask)    (mask & 0x10)  //奖券房间标识
#define HTTP_TYPE_RES            1  //http资源地址类型
#define WATER_PID                1103  //露水道具ID
#define MATCH_TYPE_FULL          1  //人满赛
#define MATCH_TYPE_TIME          3  //定时赛
#define MATCH_TYPE_TIME_POINT    4  //定点赛
#define PAY_STATE_ALI            0x01  //阿里支付
#define PAY_STATE_SMS            0x02  //短信支付
#define PAY_STATE_APPSTORE       0x04  //苹果支付
#define PROP_ID_MALAMALIH_1      1400  //次级好运符道具ID
#define PROP_ID_MALAMALIH_2      1401  //好运符道具ID
/*---------------------------------------- ----------------------------------------*/
struct ServerAddress;
struct GameListItem;
struct GameListSubInfo;
struct MatchItem;
struct PropShopPayContent;
struct PropGoodsItem;
struct PropGoodsItem2;
struct PropIDDef;
struct PropBagInfo;
struct ScoreInfo;
struct MissionItem;
struct ExchangeItem;
struct elo_message;
struct WinCountMissionStateItem;
struct WinCountMissionConfigItem;
struct WinCountMissionConfigItem2;
struct RankUserInfo;
struct RankConfigInfo;
struct exchange_record;
struct config_ver_item;
struct pop_msg_item;
struct quick_game_item;
struct reg_reward_item;
struct http_config;
struct MoneyTreeItem;
struct TreeInfo;
struct MatchServerItem;
struct MatchInfo;
struct EnrollInfo;
struct RechargeInfo;
struct ActionItem;
struct txStaticsInfo;
struct txCardStatics;
struct FastPayItem;
struct AchiveType;
struct AchiveItem;
struct AchiveState;
struct JipaiqiFastBuyItem;
struct NewRegItem;
//服务器地址
struct ServerAddress
{
    int m_id;//房间ID
    ustring<16,unsigned short> m_ip;//房间IP
    int m_port;//房间端口
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!m_ip.serialize_in(bs)) return false;
        if(!bs.read(m_port)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!m_ip.serialize_out(bs)) return false;
        if(!bs.write(m_port)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//游戏列表节点信息
struct GameListItem
{
    int m_id;//节点ID
    ustring<128,unsigned short> m_name;//房间名称
    int m_less;//最小进入币
    int m_max;//最大进入限制
    unsigned int m_mask;//节点属性
    int m_icon;//图标ID
    int m_base;//房间底分
    mvector<ServerAddress,30> m_addr;//
    int m_defaultid;//快捷购买道具ID
    ustring<64,unsigned short> m_ids;//快捷购买道具ID序列，以|隔开
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!bs.read(m_less)) return false;
        if(!bs.read(m_max)) return false;
        if(!bs.read(m_mask)) return false;
        if(!bs.read(m_icon)) return false;
        if(!bs.read(m_base)) return false;
        if(!m_addr.serialize_in(bs)) return false;
        if(!bs.read(m_defaultid)) return false;
        if(!m_ids.serialize_in(bs)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!bs.write(m_less)) return false;
        if(!bs.write(m_max)) return false;
        if(!bs.write(m_mask)) return false;
        if(!bs.write(m_icon)) return false;
        if(!bs.write(m_base)) return false;
        if(!m_addr.serialize_out(bs)) return false;
        if(!bs.write(m_defaultid)) return false;
        if(!m_ids.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//游戏节点动态信息
struct GameListSubInfo
{
    int m_id;//节点ID
    int m_n;//节点人数
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_n)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_n)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//比赛节点信息
struct MatchItem
{
    int m_id;//比赛ID
    ustring<32,unsigned short> m_name;//比赛名称
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!m_name.serialize_in(bs)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!m_name.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//支付支持的道具
struct PropShopPayContent
{
    int m_type;//支付模式类型
    ustring<16,unsigned short> m_scale;//支付对应的增减比例描述
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        if(!m_scale.serialize_in(bs)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        if(!m_scale.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//商城道具信息
struct PropGoodsItem
{
    int m_id;//道具ID
    int m_rprice;//真实价格
    ustring<16,unsigned short> m_price;//价格，直接显示字符串
    ustring<16,unsigned short> m_name;//道具名称
    ustring<64,unsigned short> m_title;//标题
    int m_icon;//图片
    int m_mask;//道具属性
    ustring<16,unsigned short> m_scale;//增减比例描述
    ustring<128,unsigned short> m_desc;//描述信息
    mvector<PropShopPayContent,16> m_shopinfo;//支持的支付方式
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_rprice)) return false;
        if(!m_price.serialize_in(bs)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!m_title.serialize_in(bs)) return false;
        if(!bs.read(m_icon)) return false;
        if(!bs.read(m_mask)) return false;
        if(!m_scale.serialize_in(bs)) return false;
        if(!m_desc.serialize_in(bs)) return false;
        if(!m_shopinfo.serialize_in(bs)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_rprice)) return false;
        if(!m_price.serialize_out(bs)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!m_title.serialize_out(bs)) return false;
        if(!bs.write(m_icon)) return false;
        if(!bs.write(m_mask)) return false;
        if(!m_scale.serialize_out(bs)) return false;
        if(!m_desc.serialize_out(bs)) return false;
        if(!m_shopinfo.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//商城道具信息
struct PropGoodsItem2
{
    int m_id;//道具ID
    int m_rprice;//真实价格
    ustring<64,unsigned short> m_price;//价格，直接显示字符串
    ustring<64,unsigned short> m_name;//道具名称
    ustring<64,unsigned short> m_title;//标题
    int m_icon;//图片
    int m_mask;//道具属性
    ustring<16,unsigned short> m_scale;//增减比例描述
    ustring<128,unsigned short> m_desc;//描述信息
    mvector<PropShopPayContent,16> m_shopinfo;//支持的支付方式
    char m_show_shop;//是否在商城中显示
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_rprice)) return false;
        if(!m_price.serialize_in(bs)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!m_title.serialize_in(bs)) return false;
        if(!bs.read(m_icon)) return false;
        if(!bs.read(m_mask)) return false;
        if(!m_scale.serialize_in(bs)) return false;
        if(!m_desc.serialize_in(bs)) return false;
        if(!m_shopinfo.serialize_in(bs)) return false;
        if(!bs.read(m_show_shop)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_rprice)) return false;
        if(!m_price.serialize_out(bs)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!m_title.serialize_out(bs)) return false;
        if(!bs.write(m_icon)) return false;
        if(!bs.write(m_mask)) return false;
        if(!m_scale.serialize_out(bs)) return false;
        if(!m_desc.serialize_out(bs)) return false;
        if(!m_shopinfo.serialize_out(bs)) return false;
        if(!bs.write(m_show_shop)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//
struct PropIDDef
{
    int m_id;//
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//道具背包信息
struct PropBagInfo
{
    unsigned int m_pid;//道具ID
    int m_n;//个数
    long long m_expire;//过期时间戳，-1表示永不过期
    int m_mask;//道具属性
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_pid)) return false;
        if(!bs.read(m_n)) return false;
        if(!bs.read(m_expire)) return false;
        if(!bs.read(m_mask)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_pid)) return false;
        if(!bs.write(m_n)) return false;
        if(!bs.write(m_expire)) return false;
        if(!bs.write(m_mask)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//用户积分信息
struct ScoreInfo
{
    int m_id;//积分ID
    int m_n;//积分数量
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_n)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_n)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//任务信息
struct MissionItem
{
    int m_id;//任务ID
    int m_icon;//图标ID
    ustring<16,unsigned short> m_name;//奖励名称
    char m_state;//状态0未完成，1未领取，2已领取
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_icon)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!bs.read(m_state)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_icon)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!bs.write(m_state)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//兑换信息
struct ExchangeItem
{
    int m_id;//兑换ID
    char m_type;//兑换类型
    char m_state;//0可兑换，1备货中
    ustring<128,unsigned short> m_desc;//描述信息
    ustring<32,unsigned short> m_name;//名称
    long long m_coin;//金币数量
    int m_srcid;//兑换消耗物品ID
    int m_srcn;//兑换消耗个数
    int m_dstid;//兑换物品ID
    int m_dstn;//兑换个数
    int m_icon;//图标ID
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_type)) return false;
        if(!bs.read(m_state)) return false;
        if(!m_desc.serialize_in(bs)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_srcid)) return false;
        if(!bs.read(m_srcn)) return false;
        if(!bs.read(m_dstid)) return false;
        if(!bs.read(m_dstn)) return false;
        if(!bs.read(m_icon)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_type)) return false;
        if(!bs.write(m_state)) return false;
        if(!m_desc.serialize_out(bs)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_srcid)) return false;
        if(!bs.write(m_srcn)) return false;
        if(!bs.write(m_dstid)) return false;
        if(!bs.write(m_dstn)) return false;
        if(!bs.write(m_icon)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//消息
struct elo_message
{
    char m_type;//消息类型
    int m_id;//消息索引
    long long m_sendt;//发送时间
    long long m_expire;//过期时间
    ustring<256,unsigned short> m_msg;//消息内容
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_sendt)) return false;
        if(!bs.read(m_expire)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_sendt)) return false;
        if(!bs.write(m_expire)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//局数任务完成状态
struct WinCountMissionStateItem
{
    int m_id;//任务ID
    int m_count;//完成计数
    char m_state;//状态，0正在进行，1可领奖，2已领奖
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_count)) return false;
        if(!bs.read(m_state)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_count)) return false;
        if(!bs.write(m_state)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//局数任务配置信息
struct WinCountMissionConfigItem
{
    int m_id;//任务ID
    int m_need;//需要完成的量
    int m_icon;//图标ID
    ustring<64,unsigned short> m_desc;//奖励描述
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_need)) return false;
        if(!bs.read(m_icon)) return false;
        if(!m_desc.serialize_in(bs)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_need)) return false;
        if(!bs.write(m_icon)) return false;
        if(!m_desc.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//局数任务配置信息
struct WinCountMissionConfigItem2
{
    int m_id;//任务ID
    int m_need;//需要完成的量
    int m_icon;//图标ID
    ustring<64,unsigned short> m_desc;//奖励描述
    int m_type;//房间类型
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_need)) return false;
        if(!bs.read(m_icon)) return false;
        if(!m_desc.serialize_in(bs)) return false;
        if(!bs.read(m_type)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_need)) return false;
        if(!bs.write(m_icon)) return false;
        if(!m_desc.serialize_out(bs)) return false;
        if(!bs.write(m_type)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//玩家排行信息
struct RankUserInfo
{
    int m_rankid;//排行ID
    long long m_value;//用户排行值
    char m_logoid;//头像ID
    ustring<32,unsigned short> m_nickname;//
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_rankid)) return false;
        if(!bs.read(m_value)) return false;
        if(!bs.read(m_logoid)) return false;
        if(!m_nickname.serialize_in(bs)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_rankid)) return false;
        if(!bs.write(m_value)) return false;
        if(!bs.write(m_logoid)) return false;
        if(!m_nickname.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//排行配置信息
struct RankConfigInfo
{
    int m_type;//排行类型
    ustring<16,unsigned short> m_name;//排行名称
    int m_icon[3];//冠亚季军图标ID
    int m_pages;//页数
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!bs.read(m_icon)) return false;
        if(!bs.read(m_pages)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!bs.write(m_icon)) return false;
        if(!bs.write(m_pages)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//兑换记录
struct exchange_record
{
    int m_id;//兑换方案ID
    int m_type;//兑换类型
    int m_state;//兑换状态0审核中，1已发送
    ustring<32,unsigned short> m_name;//收货人姓名
    ustring<16,unsigned short> m_phone;//手机号码
    ustring<256,unsigned short> m_address;//收货地址
    long long m_sendt;//发放时间
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_type)) return false;
        if(!bs.read(m_state)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!m_phone.serialize_in(bs)) return false;
        if(!m_address.serialize_in(bs)) return false;
        if(!bs.read(m_sendt)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_type)) return false;
        if(!bs.write(m_state)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!m_phone.serialize_out(bs)) return false;
        if(!m_address.serialize_out(bs)) return false;
        if(!bs.write(m_sendt)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//配置版本信息单元
struct config_ver_item
{
    unsigned short m_mid;//
    unsigned short m_nid;//
    unsigned int m_ver;//
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_mid)) return false;
        if(!bs.read(m_nid)) return false;
        if(!bs.read(m_ver)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_mid)) return false;
        if(!bs.write(m_nid)) return false;
        if(!bs.write(m_ver)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//强弹消息内容
struct pop_msg_item
{
    short m_poptype;//强弹类型
    unsigned int m_sortid;//显示顺序
    short m_logintimes;//当日登陆次数
    ustring<64,unsigned short> m_title;//强弹标题
    ustring<512,unsigned short> m_msg;//
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_poptype)) return false;
        if(!bs.read(m_sortid)) return false;
        if(!bs.read(m_logintimes)) return false;
        if(!m_title.serialize_in(bs)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_poptype)) return false;
        if(!bs.write(m_sortid)) return false;
        if(!bs.write(m_logintimes)) return false;
        if(!m_title.serialize_out(bs)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//快速游戏配置单元
struct quick_game_item
{
    int m_from;//最小金币
    int m_to;//最大金币
    int m_onlinelimit;//人数限制-1不检测
    int m_nodeid;//节点id
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_from)) return false;
        if(!bs.read(m_to)) return false;
        if(!bs.read(m_onlinelimit)) return false;
        if(!bs.read(m_nodeid)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_from)) return false;
        if(!bs.write(m_to)) return false;
        if(!bs.write(m_onlinelimit)) return false;
        if(!bs.write(m_nodeid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//注册奖励配置
struct reg_reward_item
{
    char m_type;//0:游客奖励，1:注册奖励，2:绑定奖励
    ustring<128,unsigned short> m_desc;//奖励描述信息
    int m_icon;//图标ID
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        if(!m_desc.serialize_in(bs)) return false;
        if(!bs.read(m_icon)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        if(!m_desc.serialize_out(bs)) return false;
        if(!bs.write(m_icon)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//http地址配置
struct http_config
{
    int m_type;//地址类型
    ustring<256,unsigned short> m_url;//地址
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        if(!m_url.serialize_in(bs)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        if(!m_url.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//摇钱树配置单元
struct MoneyTreeItem
{
    int m_level;//等级
    int m_up_point;//升级量
    int m_yield;//每小时产量
    int m_max_yield;//最大产量
    int m_consume;//日消耗露水
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_level)) return false;
        if(!bs.read(m_up_point)) return false;
        if(!bs.read(m_yield)) return false;
        if(!bs.read(m_max_yield)) return false;
        if(!bs.read(m_consume)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_level)) return false;
        if(!bs.write(m_up_point)) return false;
        if(!bs.write(m_yield)) return false;
        if(!bs.write(m_max_yield)) return false;
        if(!bs.write(m_consume)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//摇钱树状态信息
struct TreeInfo
{
    int m_point;//等级点数
    int m_yield;//当前可收获量
    int m_next_time;//下一次产出时间，0时停止定时器。如果界面挂起，到0点时，重新请求树信息
    int m_water_count;//当天浇灌露水
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_point)) return false;
        if(!bs.read(m_yield)) return false;
        if(!bs.read(m_next_time)) return false;
        if(!bs.read(m_water_count)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_point)) return false;
        if(!bs.write(m_yield)) return false;
        if(!bs.write(m_next_time)) return false;
        if(!bs.write(m_water_count)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//比赛服务器列表节点
struct MatchServerItem
{
    int m_id;//比赛类型ID
    char m_type;//比赛类型
    ustring<16,unsigned short> m_ip;//游戏服务器IP
    int m_port;//游戏服务器port
    ustring<64,unsigned short> m_name;//比赛名称
    int m_icon;//图标ID
    unsigned int m_mask;//标签掩码
    ustring<128,unsigned short> m_cdesc;//报名条件描述
    ustring<512,unsigned short> m_rdesc;//奖励描述
    int m_coin;//报名金币，0的话检测道具
    int m_pid;//报名道具，0的话不扣费
    int m_pn;//报名道具个数
    ustring<128,unsigned short> m_fastbuy;//快速购买id1|id2|id3
    int m_defaultbuy;//推荐购买ID
    int m_min_user;//最小报名人数
    int m_max_user;//最大报名人数
    int m_timespan;//间隔时间
    ustring<32,unsigned short> m_duration;//持续时间
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_type)) return false;
        if(!m_ip.serialize_in(bs)) return false;
        if(!bs.read(m_port)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!bs.read(m_icon)) return false;
        if(!bs.read(m_mask)) return false;
        if(!m_cdesc.serialize_in(bs)) return false;
        if(!m_rdesc.serialize_in(bs)) return false;
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_pid)) return false;
        if(!bs.read(m_pn)) return false;
        if(!m_fastbuy.serialize_in(bs)) return false;
        if(!bs.read(m_defaultbuy)) return false;
        if(!bs.read(m_min_user)) return false;
        if(!bs.read(m_max_user)) return false;
        if(!bs.read(m_timespan)) return false;
        if(!m_duration.serialize_in(bs)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_type)) return false;
        if(!m_ip.serialize_out(bs)) return false;
        if(!bs.write(m_port)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!bs.write(m_icon)) return false;
        if(!bs.write(m_mask)) return false;
        if(!m_cdesc.serialize_out(bs)) return false;
        if(!m_rdesc.serialize_out(bs)) return false;
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_pid)) return false;
        if(!bs.write(m_pn)) return false;
        if(!m_fastbuy.serialize_out(bs)) return false;
        if(!bs.write(m_defaultbuy)) return false;
        if(!bs.write(m_min_user)) return false;
        if(!bs.write(m_max_user)) return false;
        if(!bs.write(m_timespan)) return false;
        if(!m_duration.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//比赛信息
struct MatchInfo
{
    int m_id;//比赛类型ID
    int m_online;//在线比赛人数
    int m_enroll;//报名人数
    int m_rest_time;//开赛倒计时，秒，人满赛忽略
    long long m_begin_time;//开赛时间戳，人满赛忽略
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_online)) return false;
        if(!bs.read(m_enroll)) return false;
        if(!bs.read(m_rest_time)) return false;
        if(!bs.read(m_begin_time)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_online)) return false;
        if(!bs.write(m_enroll)) return false;
        if(!bs.write(m_rest_time)) return false;
        if(!bs.write(m_begin_time)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//报名信息
struct EnrollInfo
{
    int m_id;//比赛类型ID
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//充值结构信息
struct RechargeInfo
{
    ustring<64,unsigned short> m_title;//标签名
    ustring<32,unsigned short> m_ids;//道具ID
    int m_defaultid;//缺省购买的道具ID
    ustring<1024,unsigned short> m_desc;//活动描述
    char m_type;//支付类型
    bool serialize_in(byte_stream& bs)
    {
        if(!m_title.serialize_in(bs)) return false;
        if(!m_ids.serialize_in(bs)) return false;
        if(!bs.read(m_defaultid)) return false;
        if(!m_desc.serialize_in(bs)) return false;
        if(!bs.read(m_type)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!m_title.serialize_out(bs)) return false;
        if(!m_ids.serialize_out(bs)) return false;
        if(!bs.write(m_defaultid)) return false;
        if(!m_desc.serialize_out(bs)) return false;
        if(!bs.write(m_type)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//活动信息
struct ActionItem
{
    int m_pic;//图片ID
    ustring<128,unsigned short> m_title;//活动标题
    ustring<1024,unsigned short> m_desc;//活动描述<br>为换行，支持颜色标签
    int m_click_type;//CLICK类型
    ustring<256,unsigned short> m_click;//点击跳转
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_pic)) return false;
        if(!m_title.serialize_in(bs)) return false;
        if(!m_desc.serialize_in(bs)) return false;
        if(!bs.read(m_click_type)) return false;
        if(!m_click.serialize_in(bs)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_pic)) return false;
        if(!m_title.serialize_out(bs)) return false;
        if(!m_desc.serialize_out(bs)) return false;
        if(!bs.write(m_click_type)) return false;
        if(!m_click.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//用户技术统计信息
struct txStaticsInfo
{
    int m_allcount;//所有局数
    int m_wincount;//胜局数
    int m_beforebet;//翻牌前玩家跟注或加注的次数,跟注或多次加注只计一次,不包括大盲注看牌
    int m_beforeraise;//翻牌前加注次数
    int m_beforecall;//翻牌前跟注次数
    int m_flopraise;//翻牌前总加注次数
    int m_flopcall;//翻牌前跟注次数
    int m_turnraise;//转牌加注次数
    int m_turncall;//转牌跟注次数
    int m_riverraise;//河牌加注次数
    int m_rivercall;//河牌跟注次数
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_allcount)) return false;
        if(!bs.read(m_wincount)) return false;
        if(!bs.read(m_beforebet)) return false;
        if(!bs.read(m_beforeraise)) return false;
        if(!bs.read(m_beforecall)) return false;
        if(!bs.read(m_flopraise)) return false;
        if(!bs.read(m_flopcall)) return false;
        if(!bs.read(m_turnraise)) return false;
        if(!bs.read(m_turncall)) return false;
        if(!bs.read(m_riverraise)) return false;
        if(!bs.read(m_rivercall)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_allcount)) return false;
        if(!bs.write(m_wincount)) return false;
        if(!bs.write(m_beforebet)) return false;
        if(!bs.write(m_beforeraise)) return false;
        if(!bs.write(m_beforecall)) return false;
        if(!bs.write(m_flopraise)) return false;
        if(!bs.write(m_flopcall)) return false;
        if(!bs.write(m_turnraise)) return false;
        if(!bs.write(m_turncall)) return false;
        if(!bs.write(m_riverraise)) return false;
        if(!bs.write(m_rivercall)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//大牌统计
struct txCardStatics
{
    int m_royal_flush;//皇家同花顺
    int m_straight_flush;//同花顺
    int m_four;//四条
    int m_full_house;//葫芦
    int m_flush;//同花
    int m_straight;//顺子
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_royal_flush)) return false;
        if(!bs.read(m_straight_flush)) return false;
        if(!bs.read(m_four)) return false;
        if(!bs.read(m_full_house)) return false;
        if(!bs.read(m_flush)) return false;
        if(!bs.read(m_straight)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_royal_flush)) return false;
        if(!bs.write(m_straight_flush)) return false;
        if(!bs.write(m_four)) return false;
        if(!bs.write(m_full_house)) return false;
        if(!bs.write(m_flush)) return false;
        if(!bs.write(m_straight)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//快捷支付配置
struct FastPayItem
{
    int m_id;//
    ustring<64,unsigned short> m_ids;//
    int m_defaultid;//
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!m_ids.serialize_in(bs)) return false;
        if(!bs.read(m_defaultid)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!m_ids.serialize_out(bs)) return false;
        if(!bs.write(m_defaultid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//成就类别
struct AchiveType
{
    int m_type;//类型
    ustring<64,unsigned short> m_name;//名称
    int m_logo;//图标
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!bs.read(m_logo)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!bs.write(m_logo)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//成就配置
struct AchiveItem
{
    int m_type;//类型
    int m_id;//成就ID
    ustring<64,unsigned short> m_name;//名称
    int m_logo;//图标
    int m_need_val;//完成数
    ustring<128,unsigned short> m_achive_desc;//成就描述
    ustring<128,unsigned short> m_reward_desc;//奖励秒杀
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        if(!bs.read(m_id)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!bs.read(m_logo)) return false;
        if(!bs.read(m_need_val)) return false;
        if(!m_achive_desc.serialize_in(bs)) return false;
        if(!m_reward_desc.serialize_in(bs)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        if(!bs.write(m_id)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!bs.write(m_logo)) return false;
        if(!bs.write(m_need_val)) return false;
        if(!m_achive_desc.serialize_out(bs)) return false;
        if(!m_reward_desc.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//成就状态
struct AchiveState
{
    int m_id;//成就ID
    char m_state;//0未完成 1已完成 2已领奖 3隐藏
    int m_progress;//进度, 该值为-1时，不显示进度，只显示未完成
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_state)) return false;
        if(!bs.read(m_progress)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_state)) return false;
        if(!bs.write(m_progress)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//记牌器快速购买配置
struct JipaiqiFastBuyItem
{
    int m_id;//房间类型ID
    int m_prop_id;//道具ID
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_prop_id)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_prop_id)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
//新注册奖励任务
struct NewRegItem
{
    int m_id;//
    int m_icon;//
    ustring<32,unsigned short> m_name;//
    ustring<128,unsigned short> m_reward;//
    char m_state;//0时间未到，1过期，2可领奖，3已领奖
    ustring<256,unsigned short> m_reward_icon;//ID1:N1|ID2:N2|...|IDn:Nn
    bool serialize_in(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_icon)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!m_reward.serialize_in(bs)) return false;
        if(!bs.read(m_state)) return false;
        if(!m_reward_icon.serialize_in(bs)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_icon)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!m_reward.serialize_out(bs)) return false;
        if(!bs.write(m_state)) return false;
        if(!m_reward_icon.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/
/*---------------------------------------- ----------------------------------------*/
#define MJ_SYSTEM                      0 //控制协议

#define MN_LSPING                      1 //
#define MN_LSPONG                      2 //
#define MN_CHECK_VER                   3 //
#define MN_VER_RESULT                  4 //
#define MN_CONFIG_VER_INFO             5 //
#define MN_GET_REG_REWARD              6 //获取注册奖励配置
#define MN_REG_REWARD_INFO             7 //注册奖励配置信息
#define MN_REG_UMENG_TOKEN             8 //注册游客成功时发一次
#define MN_GM_INFO                     9 //客服信息，没有请求，链接后主动下发
#define MN_REG_SOCKET_INFO            10 //客户端注册socket信息
#define MN_SYSTEM_PUSH_INFO           11 //系统消息推送
#define MN_REG_SOCKET_INFO2           12 //
#define MN_REG_SOCKET_INFO3           13 //

/*---------------------------------------- ----------------------------------------*/
#define MJ_USER_ACCOUNT                1 //账号主协议

#define MN_REGIST                      1 //注册
#define MN_LOGIN_BY_NAME               2 //用户名登陆
#define MN_LOGIN_BY_OPENID             3 //令牌登陆
#define MN_LOGIN_SUCESS                4 //登陆成功
#define MN_LOGIN_FAIL                  5 //登陆失败
#define MN_MODIFY_ACCOUNT              6 //修改账号,从游客到注册帐号升级
#define MN_MODIFY_NICKNAME             7 //修改昵称
#define MN_MODIFY_LOGOID               8 //修改头像
#define MN_MODIFY_PWD                  9 //修改口令
#define MN_MODIFY_INFO_RESULT         10 //修改结果
#define MN_REGIST_FAIL                11 //注册失败
#define MN_ACCOUND_MODIFY_OK          12 //从游客升级到注册帐号成功
#define MN_ACCOUND_MODIFY_FAIL        13 //从游客升级到注册帐号失败
#define MN_BIND_REQUEST               14 //手机绑定请求
#define MN_BIND_RESPONSE              15 //
#define MN_BIND_COMIT                 16 //提交验证码
#define MN_BIND_RESULT                17 //绑定结果
#define MN_PWD_FIND_REQUEST           18 //密码找回请求
#define MN_PWD_FIND_RESPONSE          19 //密码找回反馈
#define MN_PWD_FIND_RESET             20 //重新设置密码
#define MN_PWD_FIND_RESULT            21 //设置密码结果返回
#define MN_MODIFY_SEX                 22 //修改昵称
#define MN_MODIFY_SEX_RESULT          23 //修改昵称结果
#define MN_LOGIN_CONFLICT             24 //重复登录

/*---------------------------------------- ----------------------------------------*/
#define MJ_USER_INFO                   2 //用户信息主协议

#define MN_GET_MONEY_INFO              1 //获取财富信息
#define MN_MONEY_INFO                  2 //财富信息
#define MN_GET_SCORE                   3 //获取积分信息
#define MN_SCORE_INFO                  4 //积分信息
#define MN_GET_PROP                    5 //获取背包信息
#define MN_PROP_INFO                   6 //道具信息流
#define MN_GET_ROUND_STAT              7 //获取游戏信息
#define MN_ROUND_STAT_INFO             8 //游戏信息
#define MN_GET_RECHARGE_STAE           9 //获取客户充值状态
#define MN_RECHARGE_STAE              10 //客户充值状态
#define MN_GET_CHARGE_STATE           11 //获取用户充值状态
#define MN_CHARGE_INFO                12 //用户充值状态
#define MN_USE_PROP                   13 //
#define MN_USE_PROP_RESPONSE          14 //如果使用成功，刷一下背包

/*---------------------------------------- ----------------------------------------*/
#define MJ_MISSION                     4 //任务奖励协议

#define MN_LOGIN_MISSION_STATE         2 //登录任务信息
#define MN_GET_NORMAL_MISSION          3 //查询普通任务
#define MN_NORMAL_MISSION_STATE        4 //任务信息
#define MN_GET_NORMAL_REWORD           5 //获取任务奖励
#define MN_NORMAL_MISSION_REWORD       6 //任务奖励结果
#define MN_GET_WIN_COUNT_CONFIG        7 //获取局数任务配置
#define MN_WIN_COUNT_CONFIG            8 //局数任务配置
#define MN_GET_WIN_COUNT_STATE         9 //查询局数任务状态
#define MN_WIN_COUNT_STATE            10 //局数任务状态信息
#define MN_GET_WIN_COUNT_REWARD       11 //
#define MN_WIN_COUNT_REWARD           12 //局数任务奖励反馈
#define MN_GET_ACTION_INFO            13 //获取任务配置
#define MN_ACTION_INFO                14 //任务配置
#define MN_GET_WIN_COUNT_CONFIG2      15 //获取局数任务配置2
#define MN_WIN_COUNT_CONFIG2          16 //局数任务配置2
#define MN_NEW_REG_MISSION_INFO       17 //新注册任务信息，登录后主动下发
#define MN_GET_NEW_REG_REWARD         18 //
#define MN_NEW_REG_REWARD             19 //

/*---------------------------------------- ----------------------------------------*/
#define MJ_EXCHANGE                    6 //兑换协议族

#define MN_GET_EXCHANGE_INFO           1 //获取兑换配置信息
#define MN_EXCHANGE_INFO               2 //兑换物品配置信息
#define MN_GET_EXCHANGE                3 //
#define MN_EXCHANGE_RESULT             4 //
#define MN_GET_ADDRESS                 5 //
#define MN_ADDRESS_INFO                6 //
#define MN_GET_EXCHANGE_RECORD         7 //
#define MN_EXCHANGE_RECORD             8 //

/*---------------------------------------- ----------------------------------------*/
#define MJ_GAME_LIST                   7 //游戏列表协议族

#define MN_GET_GAME_LIST_INFO          1 //获取房间信息
#define MN_GAME_LIST_INFO              2 //房间信息
#define MN_NET_CUT_INFO                3 //断线重连信息
#define MN_GET_GAME_SUB_INFO           4 //
#define MN_GAME_SUB_INFO               5 //
#define MN_GET_PK_GAME_INFO            6 //PK房间信息
#define MN_PK_GAME_INFO                7 //PK房间信息
#define MN_QUICK_GAME_INFO             8 //快速游戏配置信息
#define MN_GET_LZ_GAME_LIST_INFO       9 //获取癞子房间信息
#define MN_LZ_GAME_LIST_INFO          10 //癞子房间信息
#define MN_PK_GAME_LIST_INFO          11 //约战房间信息
#define MN_GET_FAST_PAY_INFO          12 //
#define MN_FAST_PAY_INFO              13 //

/*---------------------------------------- ----------------------------------------*/
#define MJ_MESSAGE                     8 //消息系统

#define MN_GET_USER_MES                1 //获取用户个人消息
#define MN_USER_MES_RESULT             2 //消息返回
#define MN_BULLETIN_BOARD              3 //公告滚动消息
#define MN_POP_MSG_RESULT              4 //消息返回

/*---------------------------------------- ----------------------------------------*/
#define MJ_SHOP                        9 //商城信息

#define MN_GET_SHOP_INFO               1 //获取商城结构信息
#define MN_SHOP_INFO                   2 //商城道具结构信息
#define MN_GET_RECHARGE_INFO           3 //获取充值界面结构
#define MN_RECHARGE_INFO               4 //充值结构信息
#define MN_GET_SHOP_INFO2              5 //获取商城结构信息
#define MN_GET_RECHARGE_INFO2          6 //获取充值界面结构
#define MN_GET_SHOP_INFO3              7 //获取商城结构信息
#define MN_SHOP_INFO2                  8 //商城道具结构信息
#define MN_FORCE_BUY                   9 //客户端强弹购买窗
#define MN_GET_FAST_BUY_INFO          10 //
#define MN_FAST_BUY_INFO              11 //

/*---------------------------------------- ----------------------------------------*/
#define MJ_MASTER                     10 //排行信息

#define MN_GET_RANK_CONFIG             1 //获取排行类型，该协议中登录成功后请求一次
#define MN_RANK_CONFIG                 2 //排行类型配置
#define MN_GET_RANK                    3 //获取排行结构信息
#define MN_MASTER_INFO                 4 //排行数据，当请求page为0时，下发两条sc_rank_info，page为-1的表示自己的排行

/*---------------------------------------- ----------------------------------------*/
#define MJ_PAYMENT                    11 //支付请求

#define MN_CONSUME_CODE_REQ            1 //消费码请求
#define MN_CONSUME_CODE_RESULT         2 //消费码返回
#define MN_PAYMENT_RESULT              3 //web回调购买结果

/*---------------------------------------- ----------------------------------------*/
#define MJ_FAQ                        12 //用户反馈

#define MN_WRITE_FAQ                   1 //提交用户反馈内容

/*---------------------------------------- ----------------------------------------*/
#define MJ_BANK_RUPT                  13 //破产协议

#define MN_BANKRUPT_REQUEST            1 //请求破产送
#define MN_BANKRUPT_RESULT             2 //破产送结果

/*---------------------------------------- ----------------------------------------*/
#define MJ_MONEY_TREE                 14 //摇钱树协议

#define MN_GET_TREE_CONFIG             1 //获取摇钱树配置
#define MN_TREE_CONFIG_INFO            2 //摇钱树信息
#define MN_GET_MY_TREE                 3 //获取树数据
#define MN_MY_TREE_INFO                4 //摇钱树数据
#define MN_YIELD_TREE_REQUEST          5 //收获请求
#define MN_YIELD_TREE_RESPONSE         6 //收获返回，收获成功，后发sc_my_tree_info
#define MN_WATER_TREE                  7 //浇水
#define MN_WATER_RESULT                8 //浇水结果

/*---------------------------------------- ----------------------------------------*/
#define MJ_MATCH                      15 //比赛协议组

#define MN_GET_MATCH_LIST              1 //请求比赛列表
#define MN_MATCH_LIST_INFO             2 //比赛列表
#define MN_MATCH_INFO                  3 //
#define MN_ATTENTION_MATCH             4 //关注比赛，只有报名了的比赛和关注的比赛才会收到及时信息变化
#define MN_ENROLL_MATCH                5 //报名
#define MN_ENROLL_RESULT               6 //报名结果，如果成功，服务器主动下发sc_match_info
#define MN_WITHDRAW_MATCH              7 //退赛
#define MN_WITHDRAW_RESULT             8 //退赛结果
#define MN_ENROLL_INFO                 9 //报名列表,首次链接时，服务器主动下发
#define MN_MATCH_BEGIN                10 //
#define MN_GET_MATCH_INFO             11 //

/*---------------------------------------- ----------------------------------------*/
#define MJ_SHARE                      16 //分享

#define MN_GET_SHARE_INFO              1 //请求分享信息
#define MN_SHARE_INFO                  2 //
#define MN_GET_SHARE_REWARD            3 //请求分享奖励
#define MN_SHARE_REWARD_INFO           4 //

/*---------------------------------------- ----------------------------------------*/
#define MJ_TXHole_em                  17 //德州扑克协议族

#define MN_GET_SKILL_STATIC_INFO       1 //获取技术统计
#define MN_TX_SKILL_STATIC_INFO        2 //德州技术统计信息
#define MN_GET_TX_CARD_STATIC_INFO     3 //获取牌型统计
#define MN_TX_CARD_STATIC_INFO         4 //牌型统计信息

/*---------------------------------------- ----------------------------------------*/
#define MJ_ACHIVEMENT                 18 //成就协议组

#define MN_GET_ACHIVEMENT_CFG          1 //获取成就配置
#define MN_ACHIVEMENT_CFG              2 //成就配置
#define MN_GET_ACHIVEMENT_STATE        3 //获取成就状态
#define MN_ACHIVEMENT_STATE            4 //成就状态信息
#define MN_GET_ACHIVEMENT_REWARD       5 //成就领奖
#define MN_ACHIVEMENT_REWARD_RESULT    6 //成就奖励信息

//
class mn_lsping : public msg_object_base
{
public:
    int m_t;//时间戳
    mn_lsping()
    {
        m_major_id = MJ_SYSTEM;
        m_minor_id = MN_LSPING;
    }
    ~mn_lsping(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_t)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_t)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class mn_lspong : public msg_object_base
{
public:
    int m_t;//时间戳
    mn_lspong()
    {
        m_major_id = MJ_SYSTEM;
        m_minor_id = MN_LSPONG;
    }
    ~mn_lspong(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_t)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_t)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class cs_check_ver : public msg_object_base
{
public:
    int m_channelid;//渠道ID
    char m_os;//操作系统 0.android, 1.Iphone
    ustring<32,unsigned short> m_ver;//客户端版本
    cs_check_ver()
    {
        m_major_id = MJ_SYSTEM;
        m_minor_id = MN_CHECK_VER;
    }
    ~cs_check_ver(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_channelid)) return false;
        if(!bs.read(m_os)) return false;
        if(!m_ver.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_channelid)) return false;
        if(!bs.write(m_os)) return false;
        if(!m_ver.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class sc_ver_result : public msg_object_base
{
public:
    char m_result;//0无需升级，1推荐升级，2强制升级
    ustring<1024,unsigned short> m_desc;//版本说明，<br>标签为换行符
    char m_urltype;//下载地址类型：0直接下载，1打开浏览器
    ustring<256,unsigned short> m_url;//下载地址
    sc_ver_result()
    {
        m_major_id = MJ_SYSTEM;
        m_minor_id = MN_VER_RESULT;
    }
    ~sc_ver_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!m_desc.serialize_in(bs)) return false;
        if(!bs.read(m_urltype)) return false;
        if(!m_url.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!m_desc.serialize_out(bs)) return false;
        if(!bs.write(m_urltype)) return false;
        if(!m_url.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class sc_config_ver_info : public msg_object_base
{
public:
    mvector<config_ver_item,100> m_versions;//
    mvector<http_config,30> m_url;//
    sc_config_ver_info()
    {
        m_major_id = MJ_SYSTEM;
        m_minor_id = MN_CONFIG_VER_INFO;
    }
    ~sc_config_ver_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_versions.serialize_in(bs)) return false;
        if(!m_url.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_versions.serialize_out(bs)) return false;
        if(!m_url.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取注册奖励配置
class cs_get_reg_reard : public msg_object_base
{
public:
    cs_get_reg_reard()
    {
        m_major_id = MJ_SYSTEM;
        m_minor_id = MN_GET_REG_REWARD;
    }
    ~cs_get_reg_reard(){}
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

//注册奖励配置信息
class sc_reg_reward_info : public msg_object_base
{
public:
    mvector<reg_reward_item,4> m_info;//奖励信息
    sc_reg_reward_info()
    {
        m_major_id = MJ_SYSTEM;
        m_minor_id = MN_REG_REWARD_INFO;
    }
    ~sc_reg_reward_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_info.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_info.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//注册游客成功时发一次
class cs_reg_umeng_token : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//令牌
    ustring<64,unsigned short> m_umeng_token;//UMENG_token
    cs_reg_umeng_token()
    {
        m_major_id = MJ_SYSTEM;
        m_minor_id = MN_REG_UMENG_TOKEN;
    }
    ~cs_reg_umeng_token(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!m_umeng_token.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!m_umeng_token.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//客服信息，没有请求，链接后主动下发
class sc_gm_info : public msg_object_base
{
public:
    ustring<256,unsigned short> m_info;//
    sc_gm_info()
    {
        m_major_id = MJ_SYSTEM;
        m_minor_id = MN_GM_INFO;
    }
    ~sc_gm_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_info.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_info.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//客户端注册socket信息
class cs_reg_socket_info : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//令牌
    cs_reg_socket_info()
    {
        m_major_id = MJ_SYSTEM;
        m_minor_id = MN_REG_SOCKET_INFO;
    }
    ~cs_reg_socket_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//系统消息推送
class sc_sys_push_info : public msg_object_base
{
public:
    long long m_coin;//
    char m_open;//
    ustring<256,unsigned short> m_msg;//
    long long m_vip;//
    sc_sys_push_info()
    {
        m_major_id = MJ_SYSTEM;
        m_minor_id = MN_SYSTEM_PUSH_INFO;
    }
    ~sc_sys_push_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_open)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        if(!bs.read(m_vip)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_open)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        if(!bs.write(m_vip)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class cs_reg_socket_info2 : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//令牌
    ustring<64,unsigned short> m_nickname;//
    int m_chid;//
    int m_logoid;//
    char m_sex;//
    char m_vip;//
    cs_reg_socket_info2()
    {
        m_major_id = MJ_SYSTEM;
        m_minor_id = MN_REG_SOCKET_INFO2;
    }
    ~cs_reg_socket_info2(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!m_nickname.serialize_in(bs)) return false;
        if(!bs.read(m_chid)) return false;
        if(!bs.read(m_logoid)) return false;
        if(!bs.read(m_sex)) return false;
        if(!bs.read(m_vip)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!m_nickname.serialize_out(bs)) return false;
        if(!bs.write(m_chid)) return false;
        if(!bs.write(m_logoid)) return false;
        if(!bs.write(m_sex)) return false;
        if(!bs.write(m_vip)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class cs_reg_socket_info3 : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//令牌
    ustring<64,unsigned short> m_nickname;//
    int m_chid;//
    int m_logoid;//
    char m_sex;//
    char m_vip;//
    char m_os;//
    cs_reg_socket_info3()
    {
        m_major_id = MJ_SYSTEM;
        m_minor_id = MN_REG_SOCKET_INFO3;
    }
    ~cs_reg_socket_info3(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!m_nickname.serialize_in(bs)) return false;
        if(!bs.read(m_chid)) return false;
        if(!bs.read(m_logoid)) return false;
        if(!bs.read(m_sex)) return false;
        if(!bs.read(m_vip)) return false;
        if(!bs.read(m_os)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!m_nickname.serialize_out(bs)) return false;
        if(!bs.write(m_chid)) return false;
        if(!bs.write(m_logoid)) return false;
        if(!bs.write(m_sex)) return false;
        if(!bs.write(m_vip)) return false;
        if(!bs.write(m_os)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//注册
class mn_regist : public msg_object_base
{
public:
    char m_force;//0为普通注册，1为强制注册
    int m_channelid;//渠道ID
    ustring<32,unsigned short> m_osver;//操作系统版本
    ustring<32,unsigned short> m_pixel;//分辨率
    ustring<32,unsigned short> m_opcode;//运营商代码
    ustring<256,unsigned short> m_hid;//硬件唯一码
    char m_hid_type;//唯一码类型
    ustring<32,unsigned short> m_ver;//版本
    char m_os;//操作系统 0.android, 1.Iphone
    mn_regist()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_REGIST;
    }
    ~mn_regist(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_force)) return false;
        if(!bs.read(m_channelid)) return false;
        if(!m_osver.serialize_in(bs)) return false;
        if(!m_pixel.serialize_in(bs)) return false;
        if(!m_opcode.serialize_in(bs)) return false;
        if(!m_hid.serialize_in(bs)) return false;
        if(!bs.read(m_hid_type)) return false;
        if(!m_ver.serialize_in(bs)) return false;
        if(!bs.read(m_os)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_force)) return false;
        if(!bs.write(m_channelid)) return false;
        if(!m_osver.serialize_out(bs)) return false;
        if(!m_pixel.serialize_out(bs)) return false;
        if(!m_opcode.serialize_out(bs)) return false;
        if(!m_hid.serialize_out(bs)) return false;
        if(!bs.write(m_hid_type)) return false;
        if(!m_ver.serialize_out(bs)) return false;
        if(!bs.write(m_os)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//用户名登陆
class mn_login_by_name : public msg_object_base
{
public:
    ustring<32,unsigned short> m_name;//用户名
    char m_pwd[32];//口令
    int m_channelid;//渠道ID
    ustring<32,unsigned short> m_ver;//版本
    char m_os;//操作系统 0.android, 1.Iphone
    mn_login_by_name()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_LOGIN_BY_NAME;
    }
    ~mn_login_by_name(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_name.serialize_in(bs)) return false;
        if(!bs.read(m_pwd)) return false;
        if(!bs.read(m_channelid)) return false;
        if(!m_ver.serialize_in(bs)) return false;
        if(!bs.read(m_os)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_name.serialize_out(bs)) return false;
        if(!bs.write(m_pwd)) return false;
        if(!bs.write(m_channelid)) return false;
        if(!m_ver.serialize_out(bs)) return false;
        if(!bs.write(m_os)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//令牌登陆
class mn_login_by_openid : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//令牌
    int m_channelid;//渠道ID
    ustring<32,unsigned short> m_ver;//版本
    char m_os;//操作系统 0.android, 1.Iphone
    mn_login_by_openid()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_LOGIN_BY_OPENID;
    }
    ~mn_login_by_openid(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_channelid)) return false;
        if(!m_ver.serialize_in(bs)) return false;
        if(!bs.read(m_os)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_channelid)) return false;
        if(!m_ver.serialize_out(bs)) return false;
        if(!bs.write(m_os)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//登陆成功
class mn_login_sucess : public msg_object_base
{
public:
    int m_type;//帐号类型
    unsigned int m_uid;//用户ID
    ustring<32,unsigned short> m_nickname;//
    int m_logoid;//头像ID
    ustring<36,unsigned short> m_openid;//令牌
    char m_vip;//VIP标志
    long long m_vipexpire;//VIP有效时间（小时）
    char m_sex;//性别0女，1男
    ustring<16,unsigned short> m_mobile;//认证手机
    mn_login_sucess()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_LOGIN_SUCESS;
    }
    ~mn_login_sucess(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        if(!bs.read(m_uid)) return false;
        if(!m_nickname.serialize_in(bs)) return false;
        if(!bs.read(m_logoid)) return false;
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_vip)) return false;
        if(!bs.read(m_vipexpire)) return false;
        if(!bs.read(m_sex)) return false;
        if(!m_mobile.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        if(!bs.write(m_uid)) return false;
        if(!m_nickname.serialize_out(bs)) return false;
        if(!bs.write(m_logoid)) return false;
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_vip)) return false;
        if(!bs.write(m_vipexpire)) return false;
        if(!bs.write(m_sex)) return false;
        if(!m_mobile.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//登陆失败
class mn_login_fail : public msg_object_base
{
public:
    char m_result;//失败类型
    ustring<128,unsigned short> m_msg;//失败描述
    mn_login_fail()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_LOGIN_FAIL;
    }
    ~mn_login_fail(){}
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

//修改账号,从游客到注册帐号升级
class mn_modify_account : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//令牌
    ustring<32,unsigned short> m_name;//用户名
    char m_pwd[32];//口令
    mn_modify_account()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_MODIFY_ACCOUNT;
    }
    ~mn_modify_account(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!bs.read(m_pwd)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!bs.write(m_pwd)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//修改昵称
class mn_modify_nickname : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//令牌
    ustring<32,unsigned short> m_nickname;//昵称
    mn_modify_nickname()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_MODIFY_NICKNAME;
    }
    ~mn_modify_nickname(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!m_nickname.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!m_nickname.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//修改头像
class mn_modify_logoid : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//令牌
    int m_logoid;//头像ID
    mn_modify_logoid()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_MODIFY_LOGOID;
    }
    ~mn_modify_logoid(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_logoid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_logoid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//修改口令
class mn_modify_pwd : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//令牌
    char m_oldpwd[32];//旧口令
    char m_pwd[32];//新口令
    mn_modify_pwd()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_MODIFY_PWD;
    }
    ~mn_modify_pwd(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_oldpwd)) return false;
        if(!bs.read(m_pwd)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_oldpwd)) return false;
        if(!bs.write(m_pwd)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//修改结果
class mn_modify_info_result : public msg_object_base
{
public:
    char m_result;//0成功，其他失败，255为msg
    ustring<128,unsigned short> m_msg;//失败描述
    int m_id;//修改类型
    mn_modify_info_result()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_MODIFY_INFO_RESULT;
    }
    ~mn_modify_info_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        if(!bs.read(m_id)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        if(!bs.write(m_id)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//注册失败
class mn_regist_fail : public msg_object_base
{
public:
    char m_type;//失败类型
    ustring<128,unsigned short> m_msg;//失败描述
    mn_regist_fail()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_REGIST_FAIL;
    }
    ~mn_regist_fail(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//从游客升级到注册帐号成功
class sc_accound_modify_ok : public msg_object_base
{
public:
    int m_type;//帐号类型
    long long m_coin;//奖励金币
    int m_pid;//奖励道具ID
    int m_pn;//奖励道具数量
    ustring<128,unsigned short> m_msg;//
    sc_accound_modify_ok()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_ACCOUND_MODIFY_OK;
    }
    ~sc_accound_modify_ok(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_type)) return false;
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_pid)) return false;
        if(!bs.read(m_pn)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_type)) return false;
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_pid)) return false;
        if(!bs.write(m_pn)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//从游客升级到注册帐号失败
class sc_accound_modify_fail : public msg_object_base
{
public:
    char m_result;//失败类型,openid失效，1已经有注册帐号, 2输入有误 3系统忙 4用户名冲突
    ustring<128,unsigned short> m_msg;//失败描述
    sc_accound_modify_fail()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_ACCOUND_MODIFY_FAIL;
    }
    ~sc_accound_modify_fail(){}
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

//手机绑定请求
class cs_bind_request : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//令牌
    ustring<16,unsigned short> m_phone;//手机号码
    cs_bind_request()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_BIND_REQUEST;
    }
    ~cs_bind_request(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!m_phone.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!m_phone.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class sc_bind_response : public msg_object_base
{
public:
    char m_result;//0成功，其他失败,1帐号已绑定，2手机已绑定，3未知错误，4请先设置帐号口令
    ustring<128,unsigned short> m_msg;//失败描述
    int m_codetime;//短信倒计时
    sc_bind_response()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_BIND_RESPONSE;
    }
    ~sc_bind_response(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        if(!bs.read(m_codetime)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        if(!bs.write(m_codetime)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//提交验证码
class cs_bind_comit : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//令牌
    ustring<8,unsigned short> m_code;//验证码
    cs_bind_comit()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_BIND_COMIT;
    }
    ~cs_bind_comit(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!m_code.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!m_code.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//绑定结果
class sc_bind_result : public msg_object_base
{
public:
    char m_result;//0成功，其他失败,1验证码错误,2验证码已经过期，3未知错误
    int m_type;//帐号类型
    long long m_coin;//金币奖励
    int m_pid;//道具ID
    int m_pn;//道具数量
    ustring<128,unsigned short> m_msg;//描述
    sc_bind_result()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_BIND_RESULT;
    }
    ~sc_bind_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!bs.read(m_type)) return false;
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_pid)) return false;
        if(!bs.read(m_pn)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!bs.write(m_type)) return false;
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_pid)) return false;
        if(!bs.write(m_pn)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//密码找回请求
class cs_pwd_find_request : public msg_object_base
{
public:
    ustring<16,unsigned short> m_phone;//手机号码
    cs_pwd_find_request()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_PWD_FIND_REQUEST;
    }
    ~cs_pwd_find_request(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_phone.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_phone.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//密码找回反馈
class sc_pwd_find_response : public msg_object_base
{
public:
    char m_result;//0成功，1手机号不存在, 2重复提交，3未知错误
    int m_codetime;//
    sc_pwd_find_response()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_PWD_FIND_RESPONSE;
    }
    ~sc_pwd_find_response(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!bs.read(m_codetime)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!bs.write(m_codetime)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//重新设置密码
class cs_pwd_find_reset : public msg_object_base
{
public:
    ustring<16,unsigned short> m_phone;//手机号码
    ustring<8,unsigned short> m_code;//验证码
    char m_pwd[32];//新口令
    cs_pwd_find_reset()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_PWD_FIND_RESET;
    }
    ~cs_pwd_find_reset(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_phone.serialize_in(bs)) return false;
        if(!m_code.serialize_in(bs)) return false;
        if(!bs.read(m_pwd)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_phone.serialize_out(bs)) return false;
        if(!m_code.serialize_out(bs)) return false;
        if(!bs.write(m_pwd)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//设置密码结果返回
class sc_pwd_find_result : public msg_object_base
{
public:
    char m_result;//0成功，1.验证码过期，2.验证码错误 3.未知错误
    ustring<36,unsigned short> m_name;//用户名
    sc_pwd_find_result()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_PWD_FIND_RESULT;
    }
    ~sc_pwd_find_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!m_name.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!m_name.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//修改昵称
class cs_modify_sex : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//令牌
    char m_sex;//性别
    cs_modify_sex()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_MODIFY_SEX;
    }
    ~cs_modify_sex(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_sex)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_sex)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//修改昵称结果
class sc_modify_sex_result : public msg_object_base
{
public:
    char m_result;//0成功，1失败
    char m_sex;//性别
    sc_modify_sex_result()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_MODIFY_SEX_RESULT;
    }
    ~sc_modify_sex_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!bs.read(m_sex)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!bs.write(m_sex)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//重复登录
class sc_login_conflict : public msg_object_base
{
public:
    sc_login_conflict()
    {
        m_major_id = MJ_USER_ACCOUNT;
        m_minor_id = MN_LOGIN_CONFLICT;
    }
    ~sc_login_conflict(){}
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

//获取财富信息
class cs_get_money_info : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    cs_get_money_info()
    {
        m_major_id = MJ_USER_INFO;
        m_minor_id = MN_GET_MONEY_INFO;
    }
    ~cs_get_money_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//财富信息
class sc_money_info : public msg_object_base
{
public:
    long long m_coin;//金币
    int m_vmoney;//虚拟币
    sc_money_info()
    {
        m_major_id = MJ_USER_INFO;
        m_minor_id = MN_MONEY_INFO;
    }
    ~sc_money_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_vmoney)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_vmoney)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取积分信息
class cs_get_score : public msg_object_base
{
public:
    unsigned int m_uid;//
    cs_get_score()
    {
        m_major_id = MJ_USER_INFO;
        m_minor_id = MN_GET_SCORE;
    }
    ~cs_get_score(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_uid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_uid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//积分信息
class sc_score_info : public msg_object_base
{
public:
    unsigned int m_uid;//用户ID
    mvector<ScoreInfo,30> m_scoreinfo;//积分信息流
    sc_score_info()
    {
        m_major_id = MJ_USER_INFO;
        m_minor_id = MN_SCORE_INFO;
    }
    ~sc_score_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_uid)) return false;
        if(!m_scoreinfo.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_uid)) return false;
        if(!m_scoreinfo.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取背包信息
class cs_get_prop : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    cs_get_prop()
    {
        m_major_id = MJ_USER_INFO;
        m_minor_id = MN_GET_PROP;
    }
    ~cs_get_prop(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//道具信息流
class sc_prop_info : public msg_object_base
{
public:
    mvector<PropBagInfo,50> m_propinfo;//
    sc_prop_info()
    {
        m_major_id = MJ_USER_INFO;
        m_minor_id = MN_PROP_INFO;
    }
    ~sc_prop_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_propinfo.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_propinfo.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取游戏信息
class cs_get_round_stat : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    cs_get_round_stat()
    {
        m_major_id = MJ_USER_INFO;
        m_minor_id = MN_GET_ROUND_STAT;
    }
    ~cs_get_round_stat(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//游戏信息
class sc_round_stat_info : public msg_object_base
{
public:
    int m_win;//胜局数
    int m_loss;//失败局数
    int m_flee;//逃跑局数
    sc_round_stat_info()
    {
        m_major_id = MJ_USER_INFO;
        m_minor_id = MN_ROUND_STAT_INFO;
    }
    ~sc_round_stat_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_win)) return false;
        if(!bs.read(m_loss)) return false;
        if(!bs.read(m_flee)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_win)) return false;
        if(!bs.write(m_loss)) return false;
        if(!bs.write(m_flee)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取客户充值状态
class cs_get_recharge_stae : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    cs_get_recharge_stae()
    {
        m_major_id = MJ_USER_INFO;
        m_minor_id = MN_GET_RECHARGE_STAE;
    }
    ~cs_get_recharge_stae(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//客户充值状态
class sc_recharge_stae : public msg_object_base
{
public:
    char m_smspay;//短信首充 0首充 其他不是
    char m_alipay;//支付宝首充 0首充 其他不是
    int m_recharge_total;//充值总额
    int m_vip_month;//已赠送VIP月数
    sc_recharge_stae()
    {
        m_major_id = MJ_USER_INFO;
        m_minor_id = MN_RECHARGE_STAE;
    }
    ~sc_recharge_stae(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_smspay)) return false;
        if(!bs.read(m_alipay)) return false;
        if(!bs.read(m_recharge_total)) return false;
        if(!bs.read(m_vip_month)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_smspay)) return false;
        if(!bs.write(m_alipay)) return false;
        if(!bs.write(m_recharge_total)) return false;
        if(!bs.write(m_vip_month)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取用户充值状态
class cs_get_charge_state : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    char m_os;//客户端类型：0安卓，1苹果，2WP
    cs_get_charge_state()
    {
        m_major_id = MJ_USER_INFO;
        m_minor_id = MN_GET_CHARGE_STATE;
    }
    ~cs_get_charge_state(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_os)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_os)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//用户充值状态
class sc_charge_info : public msg_object_base
{
public:
    unsigned int m_charge_mask;//充值状态位码
    int m_total;//充值总额
    int m_vip_month;//已赠送VIP月数
    sc_charge_info()
    {
        m_major_id = MJ_USER_INFO;
        m_minor_id = MN_CHARGE_INFO;
    }
    ~sc_charge_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_charge_mask)) return false;
        if(!bs.read(m_total)) return false;
        if(!bs.read(m_vip_month)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_charge_mask)) return false;
        if(!bs.write(m_total)) return false;
        if(!bs.write(m_vip_month)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class cs_use_prop : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_pid;//
    int m_pn;//
    cs_use_prop()
    {
        m_major_id = MJ_USER_INFO;
        m_minor_id = MN_USE_PROP;
    }
    ~cs_use_prop(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_pid)) return false;
        if(!bs.read(m_pn)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_pid)) return false;
        if(!bs.write(m_pn)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//如果使用成功，刷一下背包
class sc_use_prop_response : public msg_object_base
{
public:
    char m_result;//0成功，1道具不足，2当天已经使用过，其他系统错误
    ustring<512,unsigned short> m_msg;//
    long long m_coin;//
    sc_use_prop_response()
    {
        m_major_id = MJ_USER_INFO;
        m_minor_id = MN_USE_PROP_RESPONSE;
    }
    ~sc_use_prop_response(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        if(!bs.read(m_coin)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        if(!bs.write(m_coin)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//登录任务信息
class mn_online_mission_state : public msg_object_base
{
public:
    mvector<MissionItem,10> m_mission_normal;//普通登陆任务
    mvector<MissionItem,10> m_mission_vip;//vip登陆任务
    mn_online_mission_state()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_LOGIN_MISSION_STATE;
    }
    ~mn_online_mission_state(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_mission_normal.serialize_in(bs)) return false;
        if(!m_mission_vip.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_mission_normal.serialize_out(bs)) return false;
        if(!m_mission_vip.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//查询普通任务
class mn_get_normal_mission : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    mn_get_normal_mission()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_GET_NORMAL_MISSION;
    }
    ~mn_get_normal_mission(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//任务信息
class mn_normal_mission_state : public msg_object_base
{
public:
    mvector<MissionItem,20> m_mission;//任务描述
    mn_normal_mission_state()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_NORMAL_MISSION_STATE;
    }
    ~mn_normal_mission_state(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_mission.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_mission.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取任务奖励
class mn_get_normal_mission_reword : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_id;//任务ID
    mn_get_normal_mission_reword()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_GET_NORMAL_REWORD;
    }
    ~mn_get_normal_mission_reword(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_id)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_id)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//任务奖励结果
class mn_normal_mission_reword : public msg_object_base
{
public:
    int m_id;//任务ID
    int m_result;//0成功，1失败
    int m_num;//领奖额度(金币)
    ustring<32,unsigned short> m_desc;//奖励描述
    mn_normal_mission_reword()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_NORMAL_MISSION_REWORD;
    }
    ~mn_normal_mission_reword(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_result)) return false;
        if(!bs.read(m_num)) return false;
        if(!m_desc.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_result)) return false;
        if(!bs.write(m_num)) return false;
        if(!m_desc.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取局数任务配置
class cs_win_count_config : public msg_object_base
{
public:
    unsigned int m_ver;//配置版本
    cs_win_count_config()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_GET_WIN_COUNT_CONFIG;
    }
    ~cs_win_count_config(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//局数任务配置
class sc_win_count_config : public msg_object_base
{
public:
    unsigned int m_ver;//配置版本
    mvector<WinCountMissionConfigItem,10> m_config;//配置信息
    sc_win_count_config()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_WIN_COUNT_CONFIG;
    }
    ~sc_win_count_config(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_config.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_config.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//查询局数任务状态
class cs_get_win_count_state : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    cs_get_win_count_state()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_GET_WIN_COUNT_STATE;
    }
    ~cs_get_win_count_state(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//局数任务状态信息
class sc_win_count_state : public msg_object_base
{
public:
    mvector<WinCountMissionStateItem,10> m_info;//任务状态信息
    sc_win_count_state()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_WIN_COUNT_STATE;
    }
    ~sc_win_count_state(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_info.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_info.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class cs_get_win_count_reward : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_id;//
    cs_get_win_count_reward()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_GET_WIN_COUNT_REWARD;
    }
    ~cs_get_win_count_reward(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_id)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_id)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//局数任务奖励反馈
class sc_win_count_reward : public msg_object_base
{
public:
    char m_result;//0成功，1未完成，2已经过领奖
    int m_id;//任务ID
    long long m_coin;//金币奖励
    int m_pid;//道具ID
    int m_pn;//道具数量
    sc_win_count_reward()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_WIN_COUNT_REWARD;
    }
    ~sc_win_count_reward(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_pid)) return false;
        if(!bs.read(m_pn)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_pid)) return false;
        if(!bs.write(m_pn)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取任务配置
class cs_get_action_info : public msg_object_base
{
public:
    char m_os;//操作系统类型
    unsigned int m_ver;//配置版本
    cs_get_action_info()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_GET_ACTION_INFO;
    }
    ~cs_get_action_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_os)) return false;
        if(!bs.read(m_ver)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_os)) return false;
        if(!bs.write(m_ver)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//任务配置
class sc_action_info : public msg_object_base
{
public:
    unsigned int m_ver;//配置版本
    mvector<ActionItem,10> m_info;//配置信息
    sc_action_info()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_ACTION_INFO;
    }
    ~sc_action_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_info.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_info.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取局数任务配置2
class cs_win_count_config2 : public msg_object_base
{
public:
    unsigned int m_ver;//配置版本
    cs_win_count_config2()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_GET_WIN_COUNT_CONFIG2;
    }
    ~cs_win_count_config2(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//局数任务配置2
class sc_win_count_config2 : public msg_object_base
{
public:
    unsigned int m_ver;//配置版本
    mvector<WinCountMissionConfigItem2,10> m_config;//配置信息
    sc_win_count_config2()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_WIN_COUNT_CONFIG2;
    }
    ~sc_win_count_config2(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_config.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_config.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//新注册任务信息，登录后主动下发
class sc_new_reg_mission_info : public msg_object_base
{
public:
    ustring<128,unsigned short> m_head_title;//
    ustring<128,unsigned short> m_foot_title;//
    mvector<NewRegItem,8> m_info;//
    sc_new_reg_mission_info()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_NEW_REG_MISSION_INFO;
    }
    ~sc_new_reg_mission_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_head_title.serialize_in(bs)) return false;
        if(!m_foot_title.serialize_in(bs)) return false;
        if(!m_info.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_head_title.serialize_out(bs)) return false;
        if(!m_foot_title.serialize_out(bs)) return false;
        if(!m_info.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class cs_get_new_reg_reward : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    cs_get_new_reg_reward()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_GET_NEW_REG_REWARD;
    }
    ~cs_get_new_reg_reward(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class sc_new_reg_reward : public msg_object_base
{
public:
    int m_id;//任务ID
    char m_result;//
    ustring<256,unsigned short> m_msg;//
    ustring<256,unsigned short> m_pids;//ID1:N1|ID2:N2|...|IDn:Nn
    sc_new_reg_reward()
    {
        m_major_id = MJ_MISSION;
        m_minor_id = MN_NEW_REG_REWARD;
    }
    ~sc_new_reg_reward(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_result)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        if(!m_pids.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_result)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        if(!m_pids.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取兑换配置信息
class cs_get_exchange_info : public msg_object_base
{
public:
    unsigned int m_ver;//版本
    cs_get_exchange_info()
    {
        m_major_id = MJ_EXCHANGE;
        m_minor_id = MN_GET_EXCHANGE_INFO;
    }
    ~cs_get_exchange_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//兑换物品配置信息
class sc_exchange_info : public msg_object_base
{
public:
    unsigned int m_ver;//版本
    mvector<ExchangeItem,30> m_info;//
    sc_exchange_info()
    {
        m_major_id = MJ_EXCHANGE;
        m_minor_id = MN_EXCHANGE_INFO;
    }
    ~sc_exchange_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_info.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_info.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class cs_get_exchange : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_id;//兑换方案ID
    int m_n;//兑换个数
    ustring<16,unsigned short> m_phone;//手机号码
    ustring<32,unsigned short> m_name;//收货人姓名
    ustring<256,unsigned short> m_address;//收货地址
    cs_get_exchange()
    {
        m_major_id = MJ_EXCHANGE;
        m_minor_id = MN_GET_EXCHANGE;
    }
    ~cs_get_exchange(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_n)) return false;
        if(!m_phone.serialize_in(bs)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!m_address.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_n)) return false;
        if(!m_phone.serialize_out(bs)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!m_address.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class sc_exchange_result : public msg_object_base
{
public:
    char m_result;//0成功，1失败
    long long m_coin;//
    int m_srcid;//兑换消耗物品ID
    int m_srcn;//兑换消耗个数
    int m_dstid;//兑换物品ID
    int m_dstn;//兑换个数
    ustring<128,unsigned short> m_msg;//描述信息
    sc_exchange_result()
    {
        m_major_id = MJ_EXCHANGE;
        m_minor_id = MN_EXCHANGE_RESULT;
    }
    ~sc_exchange_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_srcid)) return false;
        if(!bs.read(m_srcn)) return false;
        if(!bs.read(m_dstid)) return false;
        if(!bs.read(m_dstn)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_srcid)) return false;
        if(!bs.write(m_srcn)) return false;
        if(!bs.write(m_dstid)) return false;
        if(!bs.write(m_dstn)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class cs_get_address : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    cs_get_address()
    {
        m_major_id = MJ_EXCHANGE;
        m_minor_id = MN_GET_ADDRESS;
    }
    ~cs_get_address(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class sc_address_info : public msg_object_base
{
public:
    char m_result;//0成功，1失败
    ustring<32,unsigned short> m_name;//收货人姓名
    ustring<16,unsigned short> m_phone;//手机号码
    ustring<256,unsigned short> m_address;//收货地址
    sc_address_info()
    {
        m_major_id = MJ_EXCHANGE;
        m_minor_id = MN_ADDRESS_INFO;
    }
    ~sc_address_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!m_phone.serialize_in(bs)) return false;
        if(!m_address.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!m_phone.serialize_out(bs)) return false;
        if(!m_address.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class cs_get_exchange_record : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    cs_get_exchange_record()
    {
        m_major_id = MJ_EXCHANGE;
        m_minor_id = MN_GET_EXCHANGE_RECORD;
    }
    ~cs_get_exchange_record(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class sc_exchange_record : public msg_object_base
{
public:
    char m_result;//0成功，1失败
    mvector<exchange_record,25> m_record_vec;//
    sc_exchange_record()
    {
        m_major_id = MJ_EXCHANGE;
        m_minor_id = MN_EXCHANGE_RECORD;
    }
    ~sc_exchange_record(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!m_record_vec.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!m_record_vec.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取房间信息
class mn_get_game_list_info : public msg_object_base
{
public:
    unsigned int m_ver;//版本
    char m_code;//平台码，0安卓，1苹果
    mn_get_game_list_info()
    {
        m_major_id = MJ_GAME_LIST;
        m_minor_id = MN_GET_GAME_LIST_INFO;
    }
    ~mn_get_game_list_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!bs.read(m_code)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!bs.write(m_code)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//房间信息
class mn_game_list_info : public msg_object_base
{
public:
    unsigned int m_ver;//版本
    mvector<GameListItem,30> m_list;//房间信息列表
    mn_game_list_info()
    {
        m_major_id = MJ_GAME_LIST;
        m_minor_id = MN_GAME_LIST_INFO;
    }
    ~mn_game_list_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_list.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_list.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//断线重连信息
class mn_net_cut_info : public msg_object_base
{
public:
    int m_gameid;//游戏ID
    int m_gamename;//游戏名称
    int m_id;//房间ID
    ustring<32,unsigned short> m_name;//房间名称
    ustring<16,unsigned short> m_ip;//房间IP
    int m_port;//房间端口
    mn_net_cut_info()
    {
        m_major_id = MJ_GAME_LIST;
        m_minor_id = MN_NET_CUT_INFO;
    }
    ~mn_net_cut_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_gameid)) return false;
        if(!bs.read(m_gamename)) return false;
        if(!bs.read(m_id)) return false;
        if(!m_name.serialize_in(bs)) return false;
        if(!m_ip.serialize_in(bs)) return false;
        if(!bs.read(m_port)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_gameid)) return false;
        if(!bs.write(m_gamename)) return false;
        if(!bs.write(m_id)) return false;
        if(!m_name.serialize_out(bs)) return false;
        if(!m_ip.serialize_out(bs)) return false;
        if(!bs.write(m_port)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class mn_get_game_sub_info : public msg_object_base
{
public:
    mn_get_game_sub_info()
    {
        m_major_id = MJ_GAME_LIST;
        m_minor_id = MN_GET_GAME_SUB_INFO;
    }
    ~mn_get_game_sub_info(){}
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

//
class mn_game_sub_info : public msg_object_base
{
public:
    mvector<GameListSubInfo,60> m_info;//
    mn_game_sub_info()
    {
        m_major_id = MJ_GAME_LIST;
        m_minor_id = MN_GAME_SUB_INFO;
    }
    ~mn_game_sub_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_info.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_info.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//PK房间信息
class mn_get_pk_game_info : public msg_object_base
{
public:
    mn_get_pk_game_info()
    {
        m_major_id = MJ_GAME_LIST;
        m_minor_id = MN_GET_PK_GAME_INFO;
    }
    ~mn_get_pk_game_info(){}
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

//PK房间信息
class mn_pk_game_info : public msg_object_base
{
public:
    ustring<16,unsigned short> m_ip;//IP
    int m_port;//房间端口
    mn_pk_game_info()
    {
        m_major_id = MJ_GAME_LIST;
        m_minor_id = MN_PK_GAME_INFO;
    }
    ~mn_pk_game_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_ip.serialize_in(bs)) return false;
        if(!bs.read(m_port)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_ip.serialize_out(bs)) return false;
        if(!bs.write(m_port)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//快速游戏配置信息
class mn_quick_game_info : public msg_object_base
{
public:
    mvector<quick_game_item,30> m_list;//快速游戏配置列表
    char m_type;//0表示普通场，1表示癞子场
    mn_quick_game_info()
    {
        m_major_id = MJ_GAME_LIST;
        m_minor_id = MN_QUICK_GAME_INFO;
    }
    ~mn_quick_game_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_list.serialize_in(bs)) return false;
        if(!bs.read(m_type)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_list.serialize_out(bs)) return false;
        if(!bs.write(m_type)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取癞子房间信息
class cs_get_lz_game_list_info : public msg_object_base
{
public:
    unsigned int m_ver;//版本
    char m_code;//平台码，0安卓，1苹果
    cs_get_lz_game_list_info()
    {
        m_major_id = MJ_GAME_LIST;
        m_minor_id = MN_GET_LZ_GAME_LIST_INFO;
    }
    ~cs_get_lz_game_list_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!bs.read(m_code)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!bs.write(m_code)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//癞子房间信息
class sc_lz_game_list_info : public msg_object_base
{
public:
    unsigned int m_ver;//版本
    mvector<GameListItem,30> m_list;//房间信息列表
    sc_lz_game_list_info()
    {
        m_major_id = MJ_GAME_LIST;
        m_minor_id = MN_LZ_GAME_LIST_INFO;
    }
    ~sc_lz_game_list_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_list.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_list.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//约战房间信息
class sc_pk_game_list_info : public msg_object_base
{
public:
    mvector<GameListItem,30> m_list;//房间信息列表
    sc_pk_game_list_info()
    {
        m_major_id = MJ_GAME_LIST;
        m_minor_id = MN_PK_GAME_LIST_INFO;
    }
    ~sc_pk_game_list_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_list.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_list.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class cs_get_fast_pay : public msg_object_base
{
public:
    int m_chid;//
    int m_os;//
    cs_get_fast_pay()
    {
        m_major_id = MJ_GAME_LIST;
        m_minor_id = MN_GET_FAST_PAY_INFO;
    }
    ~cs_get_fast_pay(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_chid)) return false;
        if(!bs.read(m_os)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_chid)) return false;
        if(!bs.write(m_os)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class sc_fast_pay_info : public msg_object_base
{
public:
    mvector<FastPayItem,30> m_info;//
    sc_fast_pay_info()
    {
        m_major_id = MJ_GAME_LIST;
        m_minor_id = MN_FAST_PAY_INFO;
    }
    ~sc_fast_pay_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_info.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_info.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取用户个人消息
class cs_get_user_mes : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_umsgid;//个人消息起始索引
    int m_sysmsgid;//系统消息起始索引
    cs_get_user_mes()
    {
        m_major_id = MJ_MESSAGE;
        m_minor_id = MN_GET_USER_MES;
    }
    ~cs_get_user_mes(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_umsgid)) return false;
        if(!bs.read(m_sysmsgid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_umsgid)) return false;
        if(!bs.write(m_sysmsgid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//消息返回
class sc_user_mes_result : public msg_object_base
{
public:
    mvector<elo_message,30> m_mlist;//消息列表
    sc_user_mes_result()
    {
        m_major_id = MJ_MESSAGE;
        m_minor_id = MN_USER_MES_RESULT;
    }
    ~sc_user_mes_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_mlist.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_mlist.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//公告滚动消息
class sc_bulletin : public msg_object_base
{
public:
    int m_channelid;//如果channelid为-1，则直接显示，如果大于0，则判断是否与自己的一样，不一样的丢弃
    ustring<256,unsigned short> m_msg;//公告消息
    sc_bulletin()
    {
        m_major_id = MJ_MESSAGE;
        m_minor_id = MN_BULLETIN_BOARD;
    }
    ~sc_bulletin(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_channelid)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_channelid)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//消息返回
class sc_pop_msg_result : public msg_object_base
{
public:
    mvector<pop_msg_item,5> m_pop_msg_vec;//强弹消息内容
    sc_pop_msg_result()
    {
        m_major_id = MJ_MESSAGE;
        m_minor_id = MN_POP_MSG_RESULT;
    }
    ~sc_pop_msg_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_pop_msg_vec.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_pop_msg_vec.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取商城结构信息
class cs_get_shop_info : public msg_object_base
{
public:
    unsigned int m_ver;//本地保存的配置版本，没有的话填0
    char m_type;//操作系统类型：0安卓，1苹果
    cs_get_shop_info()
    {
        m_major_id = MJ_SHOP;
        m_minor_id = MN_GET_SHOP_INFO;
    }
    ~cs_get_shop_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!bs.read(m_type)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!bs.write(m_type)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//商城道具结构信息
class sc_shop_info : public msg_object_base
{
public:
    unsigned int m_ver;//配置版本
    mvector<PropGoodsItem,50> m_goodsinfo;//道具定义
    sc_shop_info()
    {
        m_major_id = MJ_SHOP;
        m_minor_id = MN_SHOP_INFO;
    }
    ~sc_shop_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_goodsinfo.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_goodsinfo.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取充值界面结构
class cs_get_recharge_info : public msg_object_base
{
public:
    unsigned int m_ver;//本地保存的配置版本，没有的话填0
    char m_type;//操作系统类型：0安卓，1苹果
    cs_get_recharge_info()
    {
        m_major_id = MJ_SHOP;
        m_minor_id = MN_GET_RECHARGE_INFO;
    }
    ~cs_get_recharge_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!bs.read(m_type)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!bs.write(m_type)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//充值结构信息
class sc_recharge_info : public msg_object_base
{
public:
    unsigned int m_ver;//配置版本
    mvector<RechargeInfo,8> m_rechargeinfo;//充值定义
    sc_recharge_info()
    {
        m_major_id = MJ_SHOP;
        m_minor_id = MN_RECHARGE_INFO;
    }
    ~sc_recharge_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_rechargeinfo.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_rechargeinfo.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取商城结构信息
class cs_get_shop_info2 : public msg_object_base
{
public:
    unsigned int m_ver;//本地保存的配置版本，没有的话填0
    ustring<32,unsigned short> m_cver;//客户端版本
    char m_type;//操作系统类型：0安卓，1苹果，2 WP8
    int m_chid;//
    cs_get_shop_info2()
    {
        m_major_id = MJ_SHOP;
        m_minor_id = MN_GET_SHOP_INFO2;
    }
    ~cs_get_shop_info2(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_cver.serialize_in(bs)) return false;
        if(!bs.read(m_type)) return false;
        if(!bs.read(m_chid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_cver.serialize_out(bs)) return false;
        if(!bs.write(m_type)) return false;
        if(!bs.write(m_chid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取充值界面结构
class cs_get_recharge_info2 : public msg_object_base
{
public:
    unsigned int m_ver;//本地保存的配置版本，没有的话填0
    char m_type;//操作系统类型：0安卓，1苹果，2 WP8
    int m_chid;//
    cs_get_recharge_info2()
    {
        m_major_id = MJ_SHOP;
        m_minor_id = MN_GET_RECHARGE_INFO2;
    }
    ~cs_get_recharge_info2(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!bs.read(m_type)) return false;
        if(!bs.read(m_chid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!bs.write(m_type)) return false;
        if(!bs.write(m_chid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取商城结构信息
class cs_get_shop_info3 : public msg_object_base
{
public:
    unsigned int m_ver;//本地保存的配置版本，没有的话填0
    ustring<32,unsigned short> m_cver;//客户端版本
    char m_type;//操作系统类型：0安卓，1苹果，2 WP8
    int m_chid;//
    cs_get_shop_info3()
    {
        m_major_id = MJ_SHOP;
        m_minor_id = MN_GET_SHOP_INFO3;
    }
    ~cs_get_shop_info3(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_cver.serialize_in(bs)) return false;
        if(!bs.read(m_type)) return false;
        if(!bs.read(m_chid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_cver.serialize_out(bs)) return false;
        if(!bs.write(m_type)) return false;
        if(!bs.write(m_chid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//商城道具结构信息
class sc_shop_info2 : public msg_object_base
{
public:
    unsigned int m_ver;//配置版本
    mvector<PropGoodsItem2,100> m_goodsinfo;//道具定义
    sc_shop_info2()
    {
        m_major_id = MJ_SHOP;
        m_minor_id = MN_SHOP_INFO2;
    }
    ~sc_shop_info2(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_goodsinfo.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_goodsinfo.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//客户端强弹购买窗
class sc_force_buy : public msg_object_base
{
public:
    ustring<256,unsigned short> m_ids;//道具ID列表，1|2|3
    int m_did;//默认选项
    sc_force_buy()
    {
        m_major_id = MJ_SHOP;
        m_minor_id = MN_FORCE_BUY;
    }
    ~sc_force_buy(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_ids.serialize_in(bs)) return false;
        if(!bs.read(m_did)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_ids.serialize_out(bs)) return false;
        if(!bs.write(m_did)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class cs_get_fast_buy_info : public msg_object_base
{
public:
    int m_os;//平台类型：0安卓，1IOS，2WP8
    int m_chid;//渠道ID
    cs_get_fast_buy_info()
    {
        m_major_id = MJ_SHOP;
        m_minor_id = MN_GET_FAST_BUY_INFO;
    }
    ~cs_get_fast_buy_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_os)) return false;
        if(!bs.read(m_chid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_os)) return false;
        if(!bs.write(m_chid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class sc_fast_buy_info : public msg_object_base
{
public:
    mvector<JipaiqiFastBuyItem,32> m_jpq;//
    int m_vip;//
    sc_fast_buy_info()
    {
        m_major_id = MJ_SHOP;
        m_minor_id = MN_FAST_BUY_INFO;
    }
    ~sc_fast_buy_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_jpq.serialize_in(bs)) return false;
        if(!bs.read(m_vip)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_jpq.serialize_out(bs)) return false;
        if(!bs.write(m_vip)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取排行类型，该协议中登录成功后请求一次
class cs_get_rank_config : public msg_object_base
{
public:
    int m_ver;//
    cs_get_rank_config()
    {
        m_major_id = MJ_MASTER;
        m_minor_id = MN_GET_RANK_CONFIG;
    }
    ~cs_get_rank_config(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//排行类型配置
class sc_rank_config : public msg_object_base
{
public:
    int m_ver;//
    mvector<RankConfigInfo,10> m_config;//排行类型
    sc_rank_config()
    {
        m_major_id = MJ_MASTER;
        m_minor_id = MN_RANK_CONFIG;
    }
    ~sc_rank_config(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_config.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_config.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取排行结构信息
class cs_get_rank : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_type;//排行类型
    int m_page;//页码，默认为0
    cs_get_rank()
    {
        m_major_id = MJ_MASTER;
        m_minor_id = MN_GET_RANK;
    }
    ~cs_get_rank(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_type)) return false;
        if(!bs.read(m_page)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_type)) return false;
        if(!bs.write(m_page)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//排行数据，当请求page为0时，下发两条sc_rank_info，page为-1的表示自己的排行
class sc_rank_info : public msg_object_base
{
public:
    int m_page;//页码
    int m_type;//排行类型
    mvector<RankUserInfo,50> m_ranklist;//排行数据
    sc_rank_info()
    {
        m_major_id = MJ_MASTER;
        m_minor_id = MN_MASTER_INFO;
    }
    ~sc_rank_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_page)) return false;
        if(!bs.read(m_type)) return false;
        if(!m_ranklist.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_page)) return false;
        if(!bs.write(m_type)) return false;
        if(!m_ranklist.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//消费码请求
class cs_consume_code : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_type;//支付类型
    unsigned int m_propid;//道具ID
    int m_channelid;//渠道ID
    int m_child_id;//子渠道ID
    ustring<32,unsigned short> m_opcode;//运营商代码
    int m_areaid;//地区ID
    cs_consume_code()
    {
        m_major_id = MJ_PAYMENT;
        m_minor_id = MN_CONSUME_CODE_REQ;
    }
    ~cs_consume_code(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_type)) return false;
        if(!bs.read(m_propid)) return false;
        if(!bs.read(m_channelid)) return false;
        if(!bs.read(m_child_id)) return false;
        if(!m_opcode.serialize_in(bs)) return false;
        if(!bs.read(m_areaid)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_type)) return false;
        if(!bs.write(m_propid)) return false;
        if(!bs.write(m_channelid)) return false;
        if(!bs.write(m_child_id)) return false;
        if(!m_opcode.serialize_out(bs)) return false;
        if(!bs.write(m_areaid)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//消费码返回
class sc_consume_code_result : public msg_object_base
{
public:
    char m_result;//0成功，1失败
    int m_type;//支付类型
    unsigned int m_propid;//道具ID
    ustring<10240,unsigned short> m_cscode;//消费代码
    ustring<256,unsigned short> m_ordernum;//elo订单号
    ustring<256,unsigned short> m_msg;//失败或者成功消息提示
    sc_consume_code_result()
    {
        m_major_id = MJ_PAYMENT;
        m_minor_id = MN_CONSUME_CODE_RESULT;
    }
    ~sc_consume_code_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!bs.read(m_type)) return false;
        if(!bs.read(m_propid)) return false;
        if(!m_cscode.serialize_in(bs)) return false;
        if(!m_ordernum.serialize_in(bs)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!bs.write(m_type)) return false;
        if(!bs.write(m_propid)) return false;
        if(!m_cscode.serialize_out(bs)) return false;
        if(!m_ordernum.serialize_out(bs)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//web回调购买结果
class sc_web_payment : public msg_object_base
{
public:
    char m_result;//根据具体提示映射表返回，错误码
    ustring<256,unsigned short> m_msg;//支付额外提示
    unsigned int m_uid;//用户ID
    char m_open;//是否显示打开背包按钮，0不显示，1显示
    ustring<256,unsigned short> m_ordernum;//订单号
    long long m_coin;//用户当前货币,如果为-1，不理会，如果大于0，替换客户端显示
    int m_vip_hour;//购买的VIP小时数，0为没有
    sc_web_payment()
    {
        m_major_id = MJ_PAYMENT;
        m_minor_id = MN_PAYMENT_RESULT;
    }
    ~sc_web_payment(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        if(!bs.read(m_uid)) return false;
        if(!bs.read(m_open)) return false;
        if(!m_ordernum.serialize_in(bs)) return false;
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_vip_hour)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        if(!bs.write(m_uid)) return false;
        if(!bs.write(m_open)) return false;
        if(!m_ordernum.serialize_out(bs)) return false;
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_vip_hour)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//提交用户反馈内容
class cs_write_faq : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    ustring<512,unsigned short> m_msg;//反馈信息
    cs_write_faq()
    {
        m_major_id = MJ_FAQ;
        m_minor_id = MN_WRITE_FAQ;
    }
    ~cs_write_faq(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//请求破产送
class cs_bankrupt_request : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    long long m_curcoin;//用户当前货币量
    cs_bankrupt_request()
    {
        m_major_id = MJ_BANK_RUPT;
        m_minor_id = MN_BANKRUPT_REQUEST;
    }
    ~cs_bankrupt_request(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_curcoin)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_curcoin)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//破产送结果
class sc_bankrupt_result : public msg_object_base
{
public:
    char m_result;//0成功，1失败,2没破产
    long long m_coin;//用户货币数量
    ustring<128,unsigned short> m_msg;//提示语
    sc_bankrupt_result()
    {
        m_major_id = MJ_BANK_RUPT;
        m_minor_id = MN_BANKRUPT_RESULT;
    }
    ~sc_bankrupt_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!bs.read(m_coin)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!bs.write(m_coin)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取摇钱树配置
class cs_get_tree_config : public msg_object_base
{
public:
    unsigned int m_ver;//配置版本
    cs_get_tree_config()
    {
        m_major_id = MJ_MONEY_TREE;
        m_minor_id = MN_GET_TREE_CONFIG;
    }
    ~cs_get_tree_config(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//摇钱树信息
class sc_tree_config_info : public msg_object_base
{
public:
    unsigned int m_ver;//配置版本
    mvector<MoneyTreeItem,20> m_config;//树配置数组
    sc_tree_config_info()
    {
        m_major_id = MJ_MONEY_TREE;
        m_minor_id = MN_TREE_CONFIG_INFO;
    }
    ~sc_tree_config_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_config.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_config.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取树数据
class cs_get_my_tree : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    cs_get_my_tree()
    {
        m_major_id = MJ_MONEY_TREE;
        m_minor_id = MN_GET_MY_TREE;
    }
    ~cs_get_my_tree(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//摇钱树数据
class sc_my_tree_info : public msg_object_base
{
public:
    char m_result;//0成功，1失败
    ustring<128,unsigned short> m_msg;//信息
    TreeInfo m_tree;//树数据
    sc_my_tree_info()
    {
        m_major_id = MJ_MONEY_TREE;
        m_minor_id = MN_MY_TREE_INFO;
    }
    ~sc_my_tree_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        if(!m_tree.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        if(!m_tree.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//收获请求
class cs_yield_tree_request : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    cs_yield_tree_request()
    {
        m_major_id = MJ_MONEY_TREE;
        m_minor_id = MN_YIELD_TREE_REQUEST;
    }
    ~cs_yield_tree_request(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//收获返回，收获成功，后发sc_my_tree_info
class sc_yield_tree_response : public msg_object_base
{
public:
    char m_result;//0成功,其他失败
    ustring<128,unsigned short> m_msg;//信息
    int m_coin;//收获金币
    int m_pid;//收获道具ID
    int m_pn;//收获道具数量
    sc_yield_tree_response()
    {
        m_major_id = MJ_MONEY_TREE;
        m_minor_id = MN_YIELD_TREE_RESPONSE;
    }
    ~sc_yield_tree_response(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_pid)) return false;
        if(!bs.read(m_pn)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_pid)) return false;
        if(!bs.write(m_pn)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//浇水
class cs_water_tree : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_num;//浇水量
    cs_water_tree()
    {
        m_major_id = MJ_MONEY_TREE;
        m_minor_id = MN_WATER_TREE;
    }
    ~cs_water_tree(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_num)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_num)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//浇水结果
class sc_water_result : public msg_object_base
{
public:
    char m_result;//0成功
    ustring<128,unsigned short> m_msg;//失败原因，失败时，后面的数据忽略
    int m_point;//树的经验点数
    int m_count;//当天浇灌次数
    int m_num;//浇水量
    sc_water_result()
    {
        m_major_id = MJ_MONEY_TREE;
        m_minor_id = MN_WATER_RESULT;
    }
    ~sc_water_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        if(!bs.read(m_point)) return false;
        if(!bs.read(m_count)) return false;
        if(!bs.read(m_num)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        if(!bs.write(m_point)) return false;
        if(!bs.write(m_count)) return false;
        if(!bs.write(m_num)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//请求比赛列表
class cs_get_match_list : public msg_object_base
{
public:
    unsigned int m_ver;//列表版本
    char m_code;//平台码，0安卓，1苹果
    cs_get_match_list()
    {
        m_major_id = MJ_MATCH;
        m_minor_id = MN_GET_MATCH_LIST;
    }
    ~cs_get_match_list(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!bs.read(m_code)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!bs.write(m_code)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//比赛列表
class sc_match_list : public msg_object_base
{
public:
    unsigned int m_ver;//列表版本
    mvector<MatchServerItem,30> m_list;//
    sc_match_list()
    {
        m_major_id = MJ_MATCH;
        m_minor_id = MN_MATCH_LIST_INFO;
    }
    ~sc_match_list(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_list.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_list.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class sc_match_info : public msg_object_base
{
public:
    mvector<MatchInfo,30> m_info;//比赛列表动态信息
    sc_match_info()
    {
        m_major_id = MJ_MATCH;
        m_minor_id = MN_MATCH_INFO;
    }
    ~sc_match_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_info.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_info.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//关注比赛，只有报名了的比赛和关注的比赛才会收到及时信息变化
class cs_attention_match : public msg_object_base
{
public:
    int m_id;//一次只能关注一个比赛，-1取消关注
    cs_attention_match()
    {
        m_major_id = MJ_MATCH;
        m_minor_id = MN_ATTENTION_MATCH;
    }
    ~cs_attention_match(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//报名
class cs_enroll_match : public msg_object_base
{
public:
    int m_id;//比赛类型ID
    ustring<36,unsigned short> m_openid;//
    char m_paytype;//支付方式：0金币，1道具
    cs_enroll_match()
    {
        m_major_id = MJ_MATCH;
        m_minor_id = MN_ENROLL_MATCH;
    }
    ~cs_enroll_match(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_paytype)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_paytype)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//报名结果，如果成功，服务器主动下发sc_match_info
class sc_enroll_result : public msg_object_base
{
public:
    int m_id;//比赛类型ID
    char m_result;//结果：0成功，1资费不足，其他失败看MSG
    ustring<128,unsigned short> m_msg;//失败信息
    int m_coin;//消耗金币
    int m_pid;//消耗道具ID
    int m_pn;//消耗道具个数
    sc_enroll_result()
    {
        m_major_id = MJ_MATCH;
        m_minor_id = MN_ENROLL_RESULT;
    }
    ~sc_enroll_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_result)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_pid)) return false;
        if(!bs.read(m_pn)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_result)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_pid)) return false;
        if(!bs.write(m_pn)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//退赛
class cs_withdraw_match : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_id;//比赛类型ID
    cs_withdraw_match()
    {
        m_major_id = MJ_MATCH;
        m_minor_id = MN_WITHDRAW_MATCH;
    }
    ~cs_withdraw_match(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_id)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_id)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//退赛结果
class sc_withdraw_result : public msg_object_base
{
public:
    int m_id;//比赛类型ID
    char m_result;//0退赛成功，其他显示msg
    ustring<128,unsigned short> m_msg;//退赛失败信息
    int m_coin;//返还金币
    int m_pid;//返还道具ID
    int m_pn;//返还道具个数
    sc_withdraw_result()
    {
        m_major_id = MJ_MATCH;
        m_minor_id = MN_WITHDRAW_RESULT;
    }
    ~sc_withdraw_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_result)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_pid)) return false;
        if(!bs.read(m_pn)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_result)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_pid)) return false;
        if(!bs.write(m_pn)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//报名列表,首次链接时，服务器主动下发
class sc_enroll_info : public msg_object_base
{
public:
    mvector<EnrollInfo,30> m_info;//只有已经报名的项，如果没有，size=0
    sc_enroll_info()
    {
        m_major_id = MJ_MATCH;
        m_minor_id = MN_ENROLL_INFO;
    }
    ~sc_enroll_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_info.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_info.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class sc_match_begin : public msg_object_base
{
public:
    int m_id;//比赛类型ID，客户端根据这个id查找服务器IP
    int m_sn;//比赛序列号，登录游戏服务器时使用
    sc_match_begin()
    {
        m_major_id = MJ_MATCH;
        m_minor_id = MN_MATCH_BEGIN;
    }
    ~sc_match_begin(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_id)) return false;
        if(!bs.read(m_sn)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_id)) return false;
        if(!bs.write(m_sn)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class cs_get_match_info : public msg_object_base
{
public:
    cs_get_match_info()
    {
        m_major_id = MJ_MATCH;
        m_minor_id = MN_GET_MATCH_INFO;
    }
    ~cs_get_match_info(){}
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

//请求分享信息
class cs_get_share_info : public msg_object_base
{
public:
    char m_code;//平台码，0安卓，1苹果
    cs_get_share_info()
    {
        m_major_id = MJ_SHARE;
        m_minor_id = MN_GET_SHARE_INFO;
    }
    ~cs_get_share_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_code)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_code)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class sc_share_info : public msg_object_base
{
public:
    ustring<512,unsigned short> m_desc;//分享奖励说明
    ustring<128,unsigned short> m_title;//标题
    ustring<256,unsigned short> m_content;//内容
    ustring<256,unsigned short> m_url;//链接地址
    ustring<256,unsigned short> m_pic;//图片
    sc_share_info()
    {
        m_major_id = MJ_SHARE;
        m_minor_id = MN_SHARE_INFO;
    }
    ~sc_share_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_desc.serialize_in(bs)) return false;
        if(!m_title.serialize_in(bs)) return false;
        if(!m_content.serialize_in(bs)) return false;
        if(!m_url.serialize_in(bs)) return false;
        if(!m_pic.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_desc.serialize_out(bs)) return false;
        if(!m_title.serialize_out(bs)) return false;
        if(!m_content.serialize_out(bs)) return false;
        if(!m_url.serialize_out(bs)) return false;
        if(!m_pic.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//请求分享奖励
class cs_get_share_reward : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//令牌
    cs_get_share_reward()
    {
        m_major_id = MJ_SHARE;
        m_minor_id = MN_GET_SHARE_REWARD;
    }
    ~cs_get_share_reward(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//
class sc_share_reward_info : public msg_object_base
{
public:
    ustring<128,unsigned short> m_msg;//奖励说明
    long long m_coin;//
    int m_pid;//
    int m_pn;//
    sc_share_reward_info()
    {
        m_major_id = MJ_SHARE;
        m_minor_id = MN_SHARE_REWARD_INFO;
    }
    ~sc_share_reward_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_msg.serialize_in(bs)) return false;
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_pid)) return false;
        if(!bs.read(m_pn)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_msg.serialize_out(bs)) return false;
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_pid)) return false;
        if(!bs.write(m_pn)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取技术统计
class cs_get_tx_skill_static_info : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    cs_get_tx_skill_static_info()
    {
        m_major_id = MJ_TXHole_em;
        m_minor_id = MN_GET_SKILL_STATIC_INFO;
    }
    ~cs_get_tx_skill_static_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//德州技术统计信息
class sc_tx_skill_static_info : public msg_object_base
{
public:
    txStaticsInfo m_info;//
    sc_tx_skill_static_info()
    {
        m_major_id = MJ_TXHole_em;
        m_minor_id = MN_TX_SKILL_STATIC_INFO;
    }
    ~sc_tx_skill_static_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_info.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_info.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取牌型统计
class cs_get_tx_card_static_info : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    cs_get_tx_card_static_info()
    {
        m_major_id = MJ_TXHole_em;
        m_minor_id = MN_GET_TX_CARD_STATIC_INFO;
    }
    ~cs_get_tx_card_static_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//牌型统计信息
class sc_tx_card_static_info : public msg_object_base
{
public:
    txCardStatics m_info;//
    sc_tx_card_static_info()
    {
        m_major_id = MJ_TXHole_em;
        m_minor_id = MN_TX_CARD_STATIC_INFO;
    }
    ~sc_tx_card_static_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_info.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_info.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取成就配置
class cs_get_achivement_cfg : public msg_object_base
{
public:
    unsigned int m_ver;//配置版本
    cs_get_achivement_cfg()
    {
        m_major_id = MJ_ACHIVEMENT;
        m_minor_id = MN_GET_ACHIVEMENT_CFG;
    }
    ~cs_get_achivement_cfg(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//成就配置
class sc_achivement_info : public msg_object_base
{
public:
    unsigned int m_ver;//配置版本
    mvector<AchiveType,10> m_type;//分类
    mvector<AchiveItem,100> m_item;//明细
    sc_achivement_info()
    {
        m_major_id = MJ_ACHIVEMENT;
        m_minor_id = MN_ACHIVEMENT_CFG;
    }
    ~sc_achivement_info(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_ver)) return false;
        if(!m_type.serialize_in(bs)) return false;
        if(!m_item.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_ver)) return false;
        if(!m_type.serialize_out(bs)) return false;
        if(!m_item.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//获取成就状态
class cs_get_achivement_state : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    cs_get_achivement_state()
    {
        m_major_id = MJ_ACHIVEMENT;
        m_minor_id = MN_GET_ACHIVEMENT_STATE;
    }
    ~cs_get_achivement_state(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//成就状态信息
class sc_achivement : public msg_object_base
{
public:
    mvector<AchiveState,100> m_state;//
    sc_achivement()
    {
        m_major_id = MJ_ACHIVEMENT;
        m_minor_id = MN_ACHIVEMENT_STATE;
    }
    ~sc_achivement(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_state.serialize_in(bs)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_state.serialize_out(bs)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//成就领奖
class cs_get_achivement_reward : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_id;//成就ID
    cs_get_achivement_reward()
    {
        m_major_id = MJ_ACHIVEMENT;
        m_minor_id = MN_GET_ACHIVEMENT_REWARD;
    }
    ~cs_get_achivement_reward(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!m_openid.serialize_in(bs)) return false;
        if(!bs.read(m_id)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!m_openid.serialize_out(bs)) return false;
        if(!bs.write(m_id)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

//成就奖励信息
class sc_achivement_reward_result : public msg_object_base
{
public:
    char m_result;//0成功 1未完成 2已领奖
    int m_id;//成就ID
    ustring<256,unsigned short> m_msg;//
    long long m_coin;//奖励金币
    char m_prop;//0无道具 1有道具奖励
    sc_achivement_reward_result()
    {
        m_major_id = MJ_ACHIVEMENT;
        m_minor_id = MN_ACHIVEMENT_REWARD_RESULT;
    }
    ~sc_achivement_reward_result(){}
    virtual bool serialize_in_item(byte_stream& bs)
    {
        if(!bs.read(m_result)) return false;
        if(!bs.read(m_id)) return false;
        if(!m_msg.serialize_in(bs)) return false;
        if(!bs.read(m_coin)) return false;
        if(!bs.read(m_prop)) return false;
        return true;
    }
	virtual bool serialize_out_item(byte_stream& bs)
    {
        if(!bs.write(m_result)) return false;
        if(!bs.write(m_id)) return false;
        if(!m_msg.serialize_out(bs)) return false;
        if(!bs.write(m_coin)) return false;
        if(!bs.write(m_prop)) return false;
        return true;
    }
};
/*---------------------------------------- ----------------------------------------*/

/*---------------------------------------- ----------------------------------------*/
#define LSPROT_MAKE_CLASS(classname)\
{\
    if(buf) ret = new (buf)classname();\
    else ret = new classname();\
    if(ret && !ret->serialize_in(pdata, len))\
    {\
        if(!buf) delete ret;\
        ret = 0;\
    }\
}

class lsprotObjectManager
{
public:
    static msg_object_base* CreateObject(char* pdata, int len, char* buf = 0)
    {
        msg_object_base* ret=0;
        msg_object_head* head = (msg_object_head*)pdata;
        switch(head->m_major_id)
        {
            case MJ_SYSTEM:
            {
                switch(head->m_minor_id)
                {
                    case MN_LSPING:
                        LSPROT_MAKE_CLASS(mn_lsping);
                        break;
                    case MN_LSPONG:
                        LSPROT_MAKE_CLASS(mn_lspong);
                        break;
                    case MN_CHECK_VER:
                        LSPROT_MAKE_CLASS(cs_check_ver);
                        break;
                    case MN_VER_RESULT:
                        LSPROT_MAKE_CLASS(sc_ver_result);
                        break;
                    case MN_CONFIG_VER_INFO:
                        LSPROT_MAKE_CLASS(sc_config_ver_info);
                        break;
                    case MN_GET_REG_REWARD:
                        LSPROT_MAKE_CLASS(cs_get_reg_reard);
                        break;
                    case MN_REG_REWARD_INFO:
                        LSPROT_MAKE_CLASS(sc_reg_reward_info);
                        break;
                    case MN_REG_UMENG_TOKEN:
                        LSPROT_MAKE_CLASS(cs_reg_umeng_token);
                        break;
                    case MN_GM_INFO:
                        LSPROT_MAKE_CLASS(sc_gm_info);
                        break;
                    case MN_REG_SOCKET_INFO:
                        LSPROT_MAKE_CLASS(cs_reg_socket_info);
                        break;
                    case MN_SYSTEM_PUSH_INFO:
                        LSPROT_MAKE_CLASS(sc_sys_push_info);
                        break;
                    case MN_REG_SOCKET_INFO2:
                        LSPROT_MAKE_CLASS(cs_reg_socket_info2);
                        break;
                    case MN_REG_SOCKET_INFO3:
                        LSPROT_MAKE_CLASS(cs_reg_socket_info3);
                        break;
                }
            }
            break;
            case MJ_USER_ACCOUNT:
            {
                switch(head->m_minor_id)
                {
                    case MN_REGIST:
                        LSPROT_MAKE_CLASS(mn_regist);
                        break;
                    case MN_LOGIN_BY_NAME:
                        LSPROT_MAKE_CLASS(mn_login_by_name);
                        break;
                    case MN_LOGIN_BY_OPENID:
                        LSPROT_MAKE_CLASS(mn_login_by_openid);
                        break;
                    case MN_LOGIN_SUCESS:
                        LSPROT_MAKE_CLASS(mn_login_sucess);
                        break;
                    case MN_LOGIN_FAIL:
                        LSPROT_MAKE_CLASS(mn_login_fail);
                        break;
                    case MN_MODIFY_ACCOUNT:
                        LSPROT_MAKE_CLASS(mn_modify_account);
                        break;
                    case MN_MODIFY_NICKNAME:
                        LSPROT_MAKE_CLASS(mn_modify_nickname);
                        break;
                    case MN_MODIFY_LOGOID:
                        LSPROT_MAKE_CLASS(mn_modify_logoid);
                        break;
                    case MN_MODIFY_PWD:
                        LSPROT_MAKE_CLASS(mn_modify_pwd);
                        break;
                    case MN_MODIFY_INFO_RESULT:
                        LSPROT_MAKE_CLASS(mn_modify_info_result);
                        break;
                    case MN_REGIST_FAIL:
                        LSPROT_MAKE_CLASS(mn_regist_fail);
                        break;
                    case MN_ACCOUND_MODIFY_OK:
                        LSPROT_MAKE_CLASS(sc_accound_modify_ok);
                        break;
                    case MN_ACCOUND_MODIFY_FAIL:
                        LSPROT_MAKE_CLASS(sc_accound_modify_fail);
                        break;
                    case MN_BIND_REQUEST:
                        LSPROT_MAKE_CLASS(cs_bind_request);
                        break;
                    case MN_BIND_RESPONSE:
                        LSPROT_MAKE_CLASS(sc_bind_response);
                        break;
                    case MN_BIND_COMIT:
                        LSPROT_MAKE_CLASS(cs_bind_comit);
                        break;
                    case MN_BIND_RESULT:
                        LSPROT_MAKE_CLASS(sc_bind_result);
                        break;
                    case MN_PWD_FIND_REQUEST:
                        LSPROT_MAKE_CLASS(cs_pwd_find_request);
                        break;
                    case MN_PWD_FIND_RESPONSE:
                        LSPROT_MAKE_CLASS(sc_pwd_find_response);
                        break;
                    case MN_PWD_FIND_RESET:
                        LSPROT_MAKE_CLASS(cs_pwd_find_reset);
                        break;
                    case MN_PWD_FIND_RESULT:
                        LSPROT_MAKE_CLASS(sc_pwd_find_result);
                        break;
                    case MN_MODIFY_SEX:
                        LSPROT_MAKE_CLASS(cs_modify_sex);
                        break;
                    case MN_MODIFY_SEX_RESULT:
                        LSPROT_MAKE_CLASS(sc_modify_sex_result);
                        break;
                    case MN_LOGIN_CONFLICT:
                        LSPROT_MAKE_CLASS(sc_login_conflict);
                        break;
                }
            }
            break;
            case MJ_USER_INFO:
            {
                switch(head->m_minor_id)
                {
                    case MN_GET_MONEY_INFO:
                        LSPROT_MAKE_CLASS(cs_get_money_info);
                        break;
                    case MN_MONEY_INFO:
                        LSPROT_MAKE_CLASS(sc_money_info);
                        break;
                    case MN_GET_SCORE:
                        LSPROT_MAKE_CLASS(cs_get_score);
                        break;
                    case MN_SCORE_INFO:
                        LSPROT_MAKE_CLASS(sc_score_info);
                        break;
                    case MN_GET_PROP:
                        LSPROT_MAKE_CLASS(cs_get_prop);
                        break;
                    case MN_PROP_INFO:
                        LSPROT_MAKE_CLASS(sc_prop_info);
                        break;
                    case MN_GET_ROUND_STAT:
                        LSPROT_MAKE_CLASS(cs_get_round_stat);
                        break;
                    case MN_ROUND_STAT_INFO:
                        LSPROT_MAKE_CLASS(sc_round_stat_info);
                        break;
                    case MN_GET_RECHARGE_STAE:
                        LSPROT_MAKE_CLASS(cs_get_recharge_stae);
                        break;
                    case MN_RECHARGE_STAE:
                        LSPROT_MAKE_CLASS(sc_recharge_stae);
                        break;
                    case MN_GET_CHARGE_STATE:
                        LSPROT_MAKE_CLASS(cs_get_charge_state);
                        break;
                    case MN_CHARGE_INFO:
                        LSPROT_MAKE_CLASS(sc_charge_info);
                        break;
                    case MN_USE_PROP:
                        LSPROT_MAKE_CLASS(cs_use_prop);
                        break;
                    case MN_USE_PROP_RESPONSE:
                        LSPROT_MAKE_CLASS(sc_use_prop_response);
                        break;
                }
            }
            break;
            case MJ_MISSION:
            {
                switch(head->m_minor_id)
                {
                    case MN_LOGIN_MISSION_STATE:
                        LSPROT_MAKE_CLASS(mn_online_mission_state);
                        break;
                    case MN_GET_NORMAL_MISSION:
                        LSPROT_MAKE_CLASS(mn_get_normal_mission);
                        break;
                    case MN_NORMAL_MISSION_STATE:
                        LSPROT_MAKE_CLASS(mn_normal_mission_state);
                        break;
                    case MN_GET_NORMAL_REWORD:
                        LSPROT_MAKE_CLASS(mn_get_normal_mission_reword);
                        break;
                    case MN_NORMAL_MISSION_REWORD:
                        LSPROT_MAKE_CLASS(mn_normal_mission_reword);
                        break;
                    case MN_GET_WIN_COUNT_CONFIG:
                        LSPROT_MAKE_CLASS(cs_win_count_config);
                        break;
                    case MN_WIN_COUNT_CONFIG:
                        LSPROT_MAKE_CLASS(sc_win_count_config);
                        break;
                    case MN_GET_WIN_COUNT_STATE:
                        LSPROT_MAKE_CLASS(cs_get_win_count_state);
                        break;
                    case MN_WIN_COUNT_STATE:
                        LSPROT_MAKE_CLASS(sc_win_count_state);
                        break;
                    case MN_GET_WIN_COUNT_REWARD:
                        LSPROT_MAKE_CLASS(cs_get_win_count_reward);
                        break;
                    case MN_WIN_COUNT_REWARD:
                        LSPROT_MAKE_CLASS(sc_win_count_reward);
                        break;
                    case MN_GET_ACTION_INFO:
                        LSPROT_MAKE_CLASS(cs_get_action_info);
                        break;
                    case MN_ACTION_INFO:
                        LSPROT_MAKE_CLASS(sc_action_info);
                        break;
                    case MN_GET_WIN_COUNT_CONFIG2:
                        LSPROT_MAKE_CLASS(cs_win_count_config2);
                        break;
                    case MN_WIN_COUNT_CONFIG2:
                        LSPROT_MAKE_CLASS(sc_win_count_config2);
                        break;
                    case MN_NEW_REG_MISSION_INFO:
                        LSPROT_MAKE_CLASS(sc_new_reg_mission_info);
                        break;
                    case MN_GET_NEW_REG_REWARD:
                        LSPROT_MAKE_CLASS(cs_get_new_reg_reward);
                        break;
                    case MN_NEW_REG_REWARD:
                        LSPROT_MAKE_CLASS(sc_new_reg_reward);
                        break;
                }
            }
            break;
            case MJ_EXCHANGE:
            {
                switch(head->m_minor_id)
                {
                    case MN_GET_EXCHANGE_INFO:
                        LSPROT_MAKE_CLASS(cs_get_exchange_info);
                        break;
                    case MN_EXCHANGE_INFO:
                        LSPROT_MAKE_CLASS(sc_exchange_info);
                        break;
                    case MN_GET_EXCHANGE:
                        LSPROT_MAKE_CLASS(cs_get_exchange);
                        break;
                    case MN_EXCHANGE_RESULT:
                        LSPROT_MAKE_CLASS(sc_exchange_result);
                        break;
                    case MN_GET_ADDRESS:
                        LSPROT_MAKE_CLASS(cs_get_address);
                        break;
                    case MN_ADDRESS_INFO:
                        LSPROT_MAKE_CLASS(sc_address_info);
                        break;
                    case MN_GET_EXCHANGE_RECORD:
                        LSPROT_MAKE_CLASS(cs_get_exchange_record);
                        break;
                    case MN_EXCHANGE_RECORD:
                        LSPROT_MAKE_CLASS(sc_exchange_record);
                        break;
                }
            }
            break;
            case MJ_GAME_LIST:
            {
                switch(head->m_minor_id)
                {
                    case MN_GET_GAME_LIST_INFO:
                        LSPROT_MAKE_CLASS(mn_get_game_list_info);
                        break;
                    case MN_GAME_LIST_INFO:
                        LSPROT_MAKE_CLASS(mn_game_list_info);
                        break;
                    case MN_NET_CUT_INFO:
                        LSPROT_MAKE_CLASS(mn_net_cut_info);
                        break;
                    case MN_GET_GAME_SUB_INFO:
                        LSPROT_MAKE_CLASS(mn_get_game_sub_info);
                        break;
                    case MN_GAME_SUB_INFO:
                        LSPROT_MAKE_CLASS(mn_game_sub_info);
                        break;
                    case MN_GET_PK_GAME_INFO:
                        LSPROT_MAKE_CLASS(mn_get_pk_game_info);
                        break;
                    case MN_PK_GAME_INFO:
                        LSPROT_MAKE_CLASS(mn_pk_game_info);
                        break;
                    case MN_QUICK_GAME_INFO:
                        LSPROT_MAKE_CLASS(mn_quick_game_info);
                        break;
                    case MN_GET_LZ_GAME_LIST_INFO:
                        LSPROT_MAKE_CLASS(cs_get_lz_game_list_info);
                        break;
                    case MN_LZ_GAME_LIST_INFO:
                        LSPROT_MAKE_CLASS(sc_lz_game_list_info);
                        break;
                    case MN_PK_GAME_LIST_INFO:
                        LSPROT_MAKE_CLASS(sc_pk_game_list_info);
                        break;
                    case MN_GET_FAST_PAY_INFO:
                        LSPROT_MAKE_CLASS(cs_get_fast_pay);
                        break;
                    case MN_FAST_PAY_INFO:
                        LSPROT_MAKE_CLASS(sc_fast_pay_info);
                        break;
                }
            }
            break;
            case MJ_MESSAGE:
            {
                switch(head->m_minor_id)
                {
                    case MN_GET_USER_MES:
                        LSPROT_MAKE_CLASS(cs_get_user_mes);
                        break;
                    case MN_USER_MES_RESULT:
                        LSPROT_MAKE_CLASS(sc_user_mes_result);
                        break;
                    case MN_BULLETIN_BOARD:
                        LSPROT_MAKE_CLASS(sc_bulletin);
                        break;
                    case MN_POP_MSG_RESULT:
                        LSPROT_MAKE_CLASS(sc_pop_msg_result);
                        break;
                }
            }
            break;
            case MJ_SHOP:
            {
                switch(head->m_minor_id)
                {
                    case MN_GET_SHOP_INFO:
                        LSPROT_MAKE_CLASS(cs_get_shop_info);
                        break;
                    case MN_SHOP_INFO:
                        LSPROT_MAKE_CLASS(sc_shop_info);
                        break;
                    case MN_GET_RECHARGE_INFO:
                        LSPROT_MAKE_CLASS(cs_get_recharge_info);
                        break;
                    case MN_RECHARGE_INFO:
                        LSPROT_MAKE_CLASS(sc_recharge_info);
                        break;
                    case MN_GET_SHOP_INFO2:
                        LSPROT_MAKE_CLASS(cs_get_shop_info2);
                        break;
                    case MN_GET_RECHARGE_INFO2:
                        LSPROT_MAKE_CLASS(cs_get_recharge_info2);
                        break;
                    case MN_GET_SHOP_INFO3:
                        LSPROT_MAKE_CLASS(cs_get_shop_info3);
                        break;
                    case MN_SHOP_INFO2:
                        LSPROT_MAKE_CLASS(sc_shop_info2);
                        break;
                    case MN_FORCE_BUY:
                        LSPROT_MAKE_CLASS(sc_force_buy);
                        break;
                    case MN_GET_FAST_BUY_INFO:
                        LSPROT_MAKE_CLASS(cs_get_fast_buy_info);
                        break;
                    case MN_FAST_BUY_INFO:
                        LSPROT_MAKE_CLASS(sc_fast_buy_info);
                        break;
                }
            }
            break;
            case MJ_MASTER:
            {
                switch(head->m_minor_id)
                {
                    case MN_GET_RANK_CONFIG:
                        LSPROT_MAKE_CLASS(cs_get_rank_config);
                        break;
                    case MN_RANK_CONFIG:
                        LSPROT_MAKE_CLASS(sc_rank_config);
                        break;
                    case MN_GET_RANK:
                        LSPROT_MAKE_CLASS(cs_get_rank);
                        break;
                    case MN_MASTER_INFO:
                        LSPROT_MAKE_CLASS(sc_rank_info);
                        break;
                }
            }
            break;
            case MJ_PAYMENT:
            {
                switch(head->m_minor_id)
                {
                    case MN_CONSUME_CODE_REQ:
                        LSPROT_MAKE_CLASS(cs_consume_code);
                        break;
                    case MN_CONSUME_CODE_RESULT:
                        LSPROT_MAKE_CLASS(sc_consume_code_result);
                        break;
                    case MN_PAYMENT_RESULT:
                        LSPROT_MAKE_CLASS(sc_web_payment);
                        break;
                }
            }
            break;
            case MJ_FAQ:
            {
                switch(head->m_minor_id)
                {
                    case MN_WRITE_FAQ:
                        LSPROT_MAKE_CLASS(cs_write_faq);
                        break;
                }
            }
            break;
            case MJ_BANK_RUPT:
            {
                switch(head->m_minor_id)
                {
                    case MN_BANKRUPT_REQUEST:
                        LSPROT_MAKE_CLASS(cs_bankrupt_request);
                        break;
                    case MN_BANKRUPT_RESULT:
                        LSPROT_MAKE_CLASS(sc_bankrupt_result);
                        break;
                }
            }
            break;
            case MJ_MONEY_TREE:
            {
                switch(head->m_minor_id)
                {
                    case MN_GET_TREE_CONFIG:
                        LSPROT_MAKE_CLASS(cs_get_tree_config);
                        break;
                    case MN_TREE_CONFIG_INFO:
                        LSPROT_MAKE_CLASS(sc_tree_config_info);
                        break;
                    case MN_GET_MY_TREE:
                        LSPROT_MAKE_CLASS(cs_get_my_tree);
                        break;
                    case MN_MY_TREE_INFO:
                        LSPROT_MAKE_CLASS(sc_my_tree_info);
                        break;
                    case MN_YIELD_TREE_REQUEST:
                        LSPROT_MAKE_CLASS(cs_yield_tree_request);
                        break;
                    case MN_YIELD_TREE_RESPONSE:
                        LSPROT_MAKE_CLASS(sc_yield_tree_response);
                        break;
                    case MN_WATER_TREE:
                        LSPROT_MAKE_CLASS(cs_water_tree);
                        break;
                    case MN_WATER_RESULT:
                        LSPROT_MAKE_CLASS(sc_water_result);
                        break;
                }
            }
            break;
            case MJ_MATCH:
            {
                switch(head->m_minor_id)
                {
                    case MN_GET_MATCH_LIST:
                        LSPROT_MAKE_CLASS(cs_get_match_list);
                        break;
                    case MN_MATCH_LIST_INFO:
                        LSPROT_MAKE_CLASS(sc_match_list);
                        break;
                    case MN_MATCH_INFO:
                        LSPROT_MAKE_CLASS(sc_match_info);
                        break;
                    case MN_ATTENTION_MATCH:
                        LSPROT_MAKE_CLASS(cs_attention_match);
                        break;
                    case MN_ENROLL_MATCH:
                        LSPROT_MAKE_CLASS(cs_enroll_match);
                        break;
                    case MN_ENROLL_RESULT:
                        LSPROT_MAKE_CLASS(sc_enroll_result);
                        break;
                    case MN_WITHDRAW_MATCH:
                        LSPROT_MAKE_CLASS(cs_withdraw_match);
                        break;
                    case MN_WITHDRAW_RESULT:
                        LSPROT_MAKE_CLASS(sc_withdraw_result);
                        break;
                    case MN_ENROLL_INFO:
                        LSPROT_MAKE_CLASS(sc_enroll_info);
                        break;
                    case MN_MATCH_BEGIN:
                        LSPROT_MAKE_CLASS(sc_match_begin);
                        break;
                    case MN_GET_MATCH_INFO:
                        LSPROT_MAKE_CLASS(cs_get_match_info);
                        break;
                }
            }
            break;
            case MJ_SHARE:
            {
                switch(head->m_minor_id)
                {
                    case MN_GET_SHARE_INFO:
                        LSPROT_MAKE_CLASS(cs_get_share_info);
                        break;
                    case MN_SHARE_INFO:
                        LSPROT_MAKE_CLASS(sc_share_info);
                        break;
                    case MN_GET_SHARE_REWARD:
                        LSPROT_MAKE_CLASS(cs_get_share_reward);
                        break;
                    case MN_SHARE_REWARD_INFO:
                        LSPROT_MAKE_CLASS(sc_share_reward_info);
                        break;
                }
            }
            break;
            case MJ_TXHole_em:
            {
                switch(head->m_minor_id)
                {
                    case MN_GET_SKILL_STATIC_INFO:
                        LSPROT_MAKE_CLASS(cs_get_tx_skill_static_info);
                        break;
                    case MN_TX_SKILL_STATIC_INFO:
                        LSPROT_MAKE_CLASS(sc_tx_skill_static_info);
                        break;
                    case MN_GET_TX_CARD_STATIC_INFO:
                        LSPROT_MAKE_CLASS(cs_get_tx_card_static_info);
                        break;
                    case MN_TX_CARD_STATIC_INFO:
                        LSPROT_MAKE_CLASS(sc_tx_card_static_info);
                        break;
                }
            }
            break;
            case MJ_ACHIVEMENT:
            {
                switch(head->m_minor_id)
                {
                    case MN_GET_ACHIVEMENT_CFG:
                        LSPROT_MAKE_CLASS(cs_get_achivement_cfg);
                        break;
                    case MN_ACHIVEMENT_CFG:
                        LSPROT_MAKE_CLASS(sc_achivement_info);
                        break;
                    case MN_GET_ACHIVEMENT_STATE:
                        LSPROT_MAKE_CLASS(cs_get_achivement_state);
                        break;
                    case MN_ACHIVEMENT_STATE:
                        LSPROT_MAKE_CLASS(sc_achivement);
                        break;
                    case MN_GET_ACHIVEMENT_REWARD:
                        LSPROT_MAKE_CLASS(cs_get_achivement_reward);
                        break;
                    case MN_ACHIVEMENT_REWARD_RESULT:
                        LSPROT_MAKE_CLASS(sc_achivement_reward_result);
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
            case MJ_SYSTEM:
            {
                switch(nid)
                {
                    case MN_LSPING: strcpy(szMid, "MJ_SYSTEM"); strcpy(szNid, "MN_LSPING"); break;
                    case MN_LSPONG: strcpy(szMid, "MJ_SYSTEM"); strcpy(szNid, "MN_LSPONG"); break;
                    case MN_CHECK_VER: strcpy(szMid, "MJ_SYSTEM"); strcpy(szNid, "MN_CHECK_VER"); break;
                    case MN_VER_RESULT: strcpy(szMid, "MJ_SYSTEM"); strcpy(szNid, "MN_VER_RESULT"); break;
                    case MN_CONFIG_VER_INFO: strcpy(szMid, "MJ_SYSTEM"); strcpy(szNid, "MN_CONFIG_VER_INFO"); break;
                    case MN_GET_REG_REWARD: strcpy(szMid, "MJ_SYSTEM"); strcpy(szNid, "MN_GET_REG_REWARD"); break;
                    case MN_REG_REWARD_INFO: strcpy(szMid, "MJ_SYSTEM"); strcpy(szNid, "MN_REG_REWARD_INFO"); break;
                    case MN_REG_UMENG_TOKEN: strcpy(szMid, "MJ_SYSTEM"); strcpy(szNid, "MN_REG_UMENG_TOKEN"); break;
                    case MN_GM_INFO: strcpy(szMid, "MJ_SYSTEM"); strcpy(szNid, "MN_GM_INFO"); break;
                    case MN_REG_SOCKET_INFO: strcpy(szMid, "MJ_SYSTEM"); strcpy(szNid, "MN_REG_SOCKET_INFO"); break;
                    case MN_SYSTEM_PUSH_INFO: strcpy(szMid, "MJ_SYSTEM"); strcpy(szNid, "MN_SYSTEM_PUSH_INFO"); break;
                    case MN_REG_SOCKET_INFO2: strcpy(szMid, "MJ_SYSTEM"); strcpy(szNid, "MN_REG_SOCKET_INFO2"); break;
                    case MN_REG_SOCKET_INFO3: strcpy(szMid, "MJ_SYSTEM"); strcpy(szNid, "MN_REG_SOCKET_INFO3"); break;
                }
            }
            break;
            case MJ_USER_ACCOUNT:
            {
                switch(nid)
                {
                    case MN_REGIST: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_REGIST"); break;
                    case MN_LOGIN_BY_NAME: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_LOGIN_BY_NAME"); break;
                    case MN_LOGIN_BY_OPENID: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_LOGIN_BY_OPENID"); break;
                    case MN_LOGIN_SUCESS: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_LOGIN_SUCESS"); break;
                    case MN_LOGIN_FAIL: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_LOGIN_FAIL"); break;
                    case MN_MODIFY_ACCOUNT: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_MODIFY_ACCOUNT"); break;
                    case MN_MODIFY_NICKNAME: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_MODIFY_NICKNAME"); break;
                    case MN_MODIFY_LOGOID: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_MODIFY_LOGOID"); break;
                    case MN_MODIFY_PWD: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_MODIFY_PWD"); break;
                    case MN_MODIFY_INFO_RESULT: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_MODIFY_INFO_RESULT"); break;
                    case MN_REGIST_FAIL: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_REGIST_FAIL"); break;
                    case MN_ACCOUND_MODIFY_OK: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_ACCOUND_MODIFY_OK"); break;
                    case MN_ACCOUND_MODIFY_FAIL: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_ACCOUND_MODIFY_FAIL"); break;
                    case MN_BIND_REQUEST: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_BIND_REQUEST"); break;
                    case MN_BIND_RESPONSE: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_BIND_RESPONSE"); break;
                    case MN_BIND_COMIT: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_BIND_COMIT"); break;
                    case MN_BIND_RESULT: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_BIND_RESULT"); break;
                    case MN_PWD_FIND_REQUEST: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_PWD_FIND_REQUEST"); break;
                    case MN_PWD_FIND_RESPONSE: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_PWD_FIND_RESPONSE"); break;
                    case MN_PWD_FIND_RESET: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_PWD_FIND_RESET"); break;
                    case MN_PWD_FIND_RESULT: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_PWD_FIND_RESULT"); break;
                    case MN_MODIFY_SEX: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_MODIFY_SEX"); break;
                    case MN_MODIFY_SEX_RESULT: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_MODIFY_SEX_RESULT"); break;
                    case MN_LOGIN_CONFLICT: strcpy(szMid, "MJ_USER_ACCOUNT"); strcpy(szNid, "MN_LOGIN_CONFLICT"); break;
                }
            }
            break;
            case MJ_USER_INFO:
            {
                switch(nid)
                {
                    case MN_GET_MONEY_INFO: strcpy(szMid, "MJ_USER_INFO"); strcpy(szNid, "MN_GET_MONEY_INFO"); break;
                    case MN_MONEY_INFO: strcpy(szMid, "MJ_USER_INFO"); strcpy(szNid, "MN_MONEY_INFO"); break;
                    case MN_GET_SCORE: strcpy(szMid, "MJ_USER_INFO"); strcpy(szNid, "MN_GET_SCORE"); break;
                    case MN_SCORE_INFO: strcpy(szMid, "MJ_USER_INFO"); strcpy(szNid, "MN_SCORE_INFO"); break;
                    case MN_GET_PROP: strcpy(szMid, "MJ_USER_INFO"); strcpy(szNid, "MN_GET_PROP"); break;
                    case MN_PROP_INFO: strcpy(szMid, "MJ_USER_INFO"); strcpy(szNid, "MN_PROP_INFO"); break;
                    case MN_GET_ROUND_STAT: strcpy(szMid, "MJ_USER_INFO"); strcpy(szNid, "MN_GET_ROUND_STAT"); break;
                    case MN_ROUND_STAT_INFO: strcpy(szMid, "MJ_USER_INFO"); strcpy(szNid, "MN_ROUND_STAT_INFO"); break;
                    case MN_GET_RECHARGE_STAE: strcpy(szMid, "MJ_USER_INFO"); strcpy(szNid, "MN_GET_RECHARGE_STAE"); break;
                    case MN_RECHARGE_STAE: strcpy(szMid, "MJ_USER_INFO"); strcpy(szNid, "MN_RECHARGE_STAE"); break;
                    case MN_GET_CHARGE_STATE: strcpy(szMid, "MJ_USER_INFO"); strcpy(szNid, "MN_GET_CHARGE_STATE"); break;
                    case MN_CHARGE_INFO: strcpy(szMid, "MJ_USER_INFO"); strcpy(szNid, "MN_CHARGE_INFO"); break;
                    case MN_USE_PROP: strcpy(szMid, "MJ_USER_INFO"); strcpy(szNid, "MN_USE_PROP"); break;
                    case MN_USE_PROP_RESPONSE: strcpy(szMid, "MJ_USER_INFO"); strcpy(szNid, "MN_USE_PROP_RESPONSE"); break;
                }
            }
            break;
            case MJ_MISSION:
            {
                switch(nid)
                {
                    case MN_LOGIN_MISSION_STATE: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_LOGIN_MISSION_STATE"); break;
                    case MN_GET_NORMAL_MISSION: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_GET_NORMAL_MISSION"); break;
                    case MN_NORMAL_MISSION_STATE: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_NORMAL_MISSION_STATE"); break;
                    case MN_GET_NORMAL_REWORD: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_GET_NORMAL_REWORD"); break;
                    case MN_NORMAL_MISSION_REWORD: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_NORMAL_MISSION_REWORD"); break;
                    case MN_GET_WIN_COUNT_CONFIG: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_GET_WIN_COUNT_CONFIG"); break;
                    case MN_WIN_COUNT_CONFIG: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_WIN_COUNT_CONFIG"); break;
                    case MN_GET_WIN_COUNT_STATE: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_GET_WIN_COUNT_STATE"); break;
                    case MN_WIN_COUNT_STATE: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_WIN_COUNT_STATE"); break;
                    case MN_GET_WIN_COUNT_REWARD: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_GET_WIN_COUNT_REWARD"); break;
                    case MN_WIN_COUNT_REWARD: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_WIN_COUNT_REWARD"); break;
                    case MN_GET_ACTION_INFO: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_GET_ACTION_INFO"); break;
                    case MN_ACTION_INFO: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_ACTION_INFO"); break;
                    case MN_GET_WIN_COUNT_CONFIG2: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_GET_WIN_COUNT_CONFIG2"); break;
                    case MN_WIN_COUNT_CONFIG2: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_WIN_COUNT_CONFIG2"); break;
                    case MN_NEW_REG_MISSION_INFO: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_NEW_REG_MISSION_INFO"); break;
                    case MN_GET_NEW_REG_REWARD: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_GET_NEW_REG_REWARD"); break;
                    case MN_NEW_REG_REWARD: strcpy(szMid, "MJ_MISSION"); strcpy(szNid, "MN_NEW_REG_REWARD"); break;
                }
            }
            break;
            case MJ_EXCHANGE:
            {
                switch(nid)
                {
                    case MN_GET_EXCHANGE_INFO: strcpy(szMid, "MJ_EXCHANGE"); strcpy(szNid, "MN_GET_EXCHANGE_INFO"); break;
                    case MN_EXCHANGE_INFO: strcpy(szMid, "MJ_EXCHANGE"); strcpy(szNid, "MN_EXCHANGE_INFO"); break;
                    case MN_GET_EXCHANGE: strcpy(szMid, "MJ_EXCHANGE"); strcpy(szNid, "MN_GET_EXCHANGE"); break;
                    case MN_EXCHANGE_RESULT: strcpy(szMid, "MJ_EXCHANGE"); strcpy(szNid, "MN_EXCHANGE_RESULT"); break;
                    case MN_GET_ADDRESS: strcpy(szMid, "MJ_EXCHANGE"); strcpy(szNid, "MN_GET_ADDRESS"); break;
                    case MN_ADDRESS_INFO: strcpy(szMid, "MJ_EXCHANGE"); strcpy(szNid, "MN_ADDRESS_INFO"); break;
                    case MN_GET_EXCHANGE_RECORD: strcpy(szMid, "MJ_EXCHANGE"); strcpy(szNid, "MN_GET_EXCHANGE_RECORD"); break;
                    case MN_EXCHANGE_RECORD: strcpy(szMid, "MJ_EXCHANGE"); strcpy(szNid, "MN_EXCHANGE_RECORD"); break;
                }
            }
            break;
            case MJ_GAME_LIST:
            {
                switch(nid)
                {
                    case MN_GET_GAME_LIST_INFO: strcpy(szMid, "MJ_GAME_LIST"); strcpy(szNid, "MN_GET_GAME_LIST_INFO"); break;
                    case MN_GAME_LIST_INFO: strcpy(szMid, "MJ_GAME_LIST"); strcpy(szNid, "MN_GAME_LIST_INFO"); break;
                    case MN_NET_CUT_INFO: strcpy(szMid, "MJ_GAME_LIST"); strcpy(szNid, "MN_NET_CUT_INFO"); break;
                    case MN_GET_GAME_SUB_INFO: strcpy(szMid, "MJ_GAME_LIST"); strcpy(szNid, "MN_GET_GAME_SUB_INFO"); break;
                    case MN_GAME_SUB_INFO: strcpy(szMid, "MJ_GAME_LIST"); strcpy(szNid, "MN_GAME_SUB_INFO"); break;
                    case MN_GET_PK_GAME_INFO: strcpy(szMid, "MJ_GAME_LIST"); strcpy(szNid, "MN_GET_PK_GAME_INFO"); break;
                    case MN_PK_GAME_INFO: strcpy(szMid, "MJ_GAME_LIST"); strcpy(szNid, "MN_PK_GAME_INFO"); break;
                    case MN_QUICK_GAME_INFO: strcpy(szMid, "MJ_GAME_LIST"); strcpy(szNid, "MN_QUICK_GAME_INFO"); break;
                    case MN_GET_LZ_GAME_LIST_INFO: strcpy(szMid, "MJ_GAME_LIST"); strcpy(szNid, "MN_GET_LZ_GAME_LIST_INFO"); break;
                    case MN_LZ_GAME_LIST_INFO: strcpy(szMid, "MJ_GAME_LIST"); strcpy(szNid, "MN_LZ_GAME_LIST_INFO"); break;
                    case MN_PK_GAME_LIST_INFO: strcpy(szMid, "MJ_GAME_LIST"); strcpy(szNid, "MN_PK_GAME_LIST_INFO"); break;
                    case MN_GET_FAST_PAY_INFO: strcpy(szMid, "MJ_GAME_LIST"); strcpy(szNid, "MN_GET_FAST_PAY_INFO"); break;
                    case MN_FAST_PAY_INFO: strcpy(szMid, "MJ_GAME_LIST"); strcpy(szNid, "MN_FAST_PAY_INFO"); break;
                }
            }
            break;
            case MJ_MESSAGE:
            {
                switch(nid)
                {
                    case MN_GET_USER_MES: strcpy(szMid, "MJ_MESSAGE"); strcpy(szNid, "MN_GET_USER_MES"); break;
                    case MN_USER_MES_RESULT: strcpy(szMid, "MJ_MESSAGE"); strcpy(szNid, "MN_USER_MES_RESULT"); break;
                    case MN_BULLETIN_BOARD: strcpy(szMid, "MJ_MESSAGE"); strcpy(szNid, "MN_BULLETIN_BOARD"); break;
                    case MN_POP_MSG_RESULT: strcpy(szMid, "MJ_MESSAGE"); strcpy(szNid, "MN_POP_MSG_RESULT"); break;
                }
            }
            break;
            case MJ_SHOP:
            {
                switch(nid)
                {
                    case MN_GET_SHOP_INFO: strcpy(szMid, "MJ_SHOP"); strcpy(szNid, "MN_GET_SHOP_INFO"); break;
                    case MN_SHOP_INFO: strcpy(szMid, "MJ_SHOP"); strcpy(szNid, "MN_SHOP_INFO"); break;
                    case MN_GET_RECHARGE_INFO: strcpy(szMid, "MJ_SHOP"); strcpy(szNid, "MN_GET_RECHARGE_INFO"); break;
                    case MN_RECHARGE_INFO: strcpy(szMid, "MJ_SHOP"); strcpy(szNid, "MN_RECHARGE_INFO"); break;
                    case MN_GET_SHOP_INFO2: strcpy(szMid, "MJ_SHOP"); strcpy(szNid, "MN_GET_SHOP_INFO2"); break;
                    case MN_GET_RECHARGE_INFO2: strcpy(szMid, "MJ_SHOP"); strcpy(szNid, "MN_GET_RECHARGE_INFO2"); break;
                    case MN_GET_SHOP_INFO3: strcpy(szMid, "MJ_SHOP"); strcpy(szNid, "MN_GET_SHOP_INFO3"); break;
                    case MN_SHOP_INFO2: strcpy(szMid, "MJ_SHOP"); strcpy(szNid, "MN_SHOP_INFO2"); break;
                    case MN_FORCE_BUY: strcpy(szMid, "MJ_SHOP"); strcpy(szNid, "MN_FORCE_BUY"); break;
                    case MN_GET_FAST_BUY_INFO: strcpy(szMid, "MJ_SHOP"); strcpy(szNid, "MN_GET_FAST_BUY_INFO"); break;
                    case MN_FAST_BUY_INFO: strcpy(szMid, "MJ_SHOP"); strcpy(szNid, "MN_FAST_BUY_INFO"); break;
                }
            }
            break;
            case MJ_MASTER:
            {
                switch(nid)
                {
                    case MN_GET_RANK_CONFIG: strcpy(szMid, "MJ_MASTER"); strcpy(szNid, "MN_GET_RANK_CONFIG"); break;
                    case MN_RANK_CONFIG: strcpy(szMid, "MJ_MASTER"); strcpy(szNid, "MN_RANK_CONFIG"); break;
                    case MN_GET_RANK: strcpy(szMid, "MJ_MASTER"); strcpy(szNid, "MN_GET_RANK"); break;
                    case MN_MASTER_INFO: strcpy(szMid, "MJ_MASTER"); strcpy(szNid, "MN_MASTER_INFO"); break;
                }
            }
            break;
            case MJ_PAYMENT:
            {
                switch(nid)
                {
                    case MN_CONSUME_CODE_REQ: strcpy(szMid, "MJ_PAYMENT"); strcpy(szNid, "MN_CONSUME_CODE_REQ"); break;
                    case MN_CONSUME_CODE_RESULT: strcpy(szMid, "MJ_PAYMENT"); strcpy(szNid, "MN_CONSUME_CODE_RESULT"); break;
                    case MN_PAYMENT_RESULT: strcpy(szMid, "MJ_PAYMENT"); strcpy(szNid, "MN_PAYMENT_RESULT"); break;
                }
            }
            break;
            case MJ_FAQ:
            {
                switch(nid)
                {
                    case MN_WRITE_FAQ: strcpy(szMid, "MJ_FAQ"); strcpy(szNid, "MN_WRITE_FAQ"); break;
                }
            }
            break;
            case MJ_BANK_RUPT:
            {
                switch(nid)
                {
                    case MN_BANKRUPT_REQUEST: strcpy(szMid, "MJ_BANK_RUPT"); strcpy(szNid, "MN_BANKRUPT_REQUEST"); break;
                    case MN_BANKRUPT_RESULT: strcpy(szMid, "MJ_BANK_RUPT"); strcpy(szNid, "MN_BANKRUPT_RESULT"); break;
                }
            }
            break;
            case MJ_MONEY_TREE:
            {
                switch(nid)
                {
                    case MN_GET_TREE_CONFIG: strcpy(szMid, "MJ_MONEY_TREE"); strcpy(szNid, "MN_GET_TREE_CONFIG"); break;
                    case MN_TREE_CONFIG_INFO: strcpy(szMid, "MJ_MONEY_TREE"); strcpy(szNid, "MN_TREE_CONFIG_INFO"); break;
                    case MN_GET_MY_TREE: strcpy(szMid, "MJ_MONEY_TREE"); strcpy(szNid, "MN_GET_MY_TREE"); break;
                    case MN_MY_TREE_INFO: strcpy(szMid, "MJ_MONEY_TREE"); strcpy(szNid, "MN_MY_TREE_INFO"); break;
                    case MN_YIELD_TREE_REQUEST: strcpy(szMid, "MJ_MONEY_TREE"); strcpy(szNid, "MN_YIELD_TREE_REQUEST"); break;
                    case MN_YIELD_TREE_RESPONSE: strcpy(szMid, "MJ_MONEY_TREE"); strcpy(szNid, "MN_YIELD_TREE_RESPONSE"); break;
                    case MN_WATER_TREE: strcpy(szMid, "MJ_MONEY_TREE"); strcpy(szNid, "MN_WATER_TREE"); break;
                    case MN_WATER_RESULT: strcpy(szMid, "MJ_MONEY_TREE"); strcpy(szNid, "MN_WATER_RESULT"); break;
                }
            }
            break;
            case MJ_MATCH:
            {
                switch(nid)
                {
                    case MN_GET_MATCH_LIST: strcpy(szMid, "MJ_MATCH"); strcpy(szNid, "MN_GET_MATCH_LIST"); break;
                    case MN_MATCH_LIST_INFO: strcpy(szMid, "MJ_MATCH"); strcpy(szNid, "MN_MATCH_LIST_INFO"); break;
                    case MN_MATCH_INFO: strcpy(szMid, "MJ_MATCH"); strcpy(szNid, "MN_MATCH_INFO"); break;
                    case MN_ATTENTION_MATCH: strcpy(szMid, "MJ_MATCH"); strcpy(szNid, "MN_ATTENTION_MATCH"); break;
                    case MN_ENROLL_MATCH: strcpy(szMid, "MJ_MATCH"); strcpy(szNid, "MN_ENROLL_MATCH"); break;
                    case MN_ENROLL_RESULT: strcpy(szMid, "MJ_MATCH"); strcpy(szNid, "MN_ENROLL_RESULT"); break;
                    case MN_WITHDRAW_MATCH: strcpy(szMid, "MJ_MATCH"); strcpy(szNid, "MN_WITHDRAW_MATCH"); break;
                    case MN_WITHDRAW_RESULT: strcpy(szMid, "MJ_MATCH"); strcpy(szNid, "MN_WITHDRAW_RESULT"); break;
                    case MN_ENROLL_INFO: strcpy(szMid, "MJ_MATCH"); strcpy(szNid, "MN_ENROLL_INFO"); break;
                    case MN_MATCH_BEGIN: strcpy(szMid, "MJ_MATCH"); strcpy(szNid, "MN_MATCH_BEGIN"); break;
                    case MN_GET_MATCH_INFO: strcpy(szMid, "MJ_MATCH"); strcpy(szNid, "MN_GET_MATCH_INFO"); break;
                }
            }
            break;
            case MJ_SHARE:
            {
                switch(nid)
                {
                    case MN_GET_SHARE_INFO: strcpy(szMid, "MJ_SHARE"); strcpy(szNid, "MN_GET_SHARE_INFO"); break;
                    case MN_SHARE_INFO: strcpy(szMid, "MJ_SHARE"); strcpy(szNid, "MN_SHARE_INFO"); break;
                    case MN_GET_SHARE_REWARD: strcpy(szMid, "MJ_SHARE"); strcpy(szNid, "MN_GET_SHARE_REWARD"); break;
                    case MN_SHARE_REWARD_INFO: strcpy(szMid, "MJ_SHARE"); strcpy(szNid, "MN_SHARE_REWARD_INFO"); break;
                }
            }
            break;
            case MJ_TXHole_em:
            {
                switch(nid)
                {
                    case MN_GET_SKILL_STATIC_INFO: strcpy(szMid, "MJ_TXHole_em"); strcpy(szNid, "MN_GET_SKILL_STATIC_INFO"); break;
                    case MN_TX_SKILL_STATIC_INFO: strcpy(szMid, "MJ_TXHole_em"); strcpy(szNid, "MN_TX_SKILL_STATIC_INFO"); break;
                    case MN_GET_TX_CARD_STATIC_INFO: strcpy(szMid, "MJ_TXHole_em"); strcpy(szNid, "MN_GET_TX_CARD_STATIC_INFO"); break;
                    case MN_TX_CARD_STATIC_INFO: strcpy(szMid, "MJ_TXHole_em"); strcpy(szNid, "MN_TX_CARD_STATIC_INFO"); break;
                }
            }
            break;
            case MJ_ACHIVEMENT:
            {
                switch(nid)
                {
                    case MN_GET_ACHIVEMENT_CFG: strcpy(szMid, "MJ_ACHIVEMENT"); strcpy(szNid, "MN_GET_ACHIVEMENT_CFG"); break;
                    case MN_ACHIVEMENT_CFG: strcpy(szMid, "MJ_ACHIVEMENT"); strcpy(szNid, "MN_ACHIVEMENT_CFG"); break;
                    case MN_GET_ACHIVEMENT_STATE: strcpy(szMid, "MJ_ACHIVEMENT"); strcpy(szNid, "MN_GET_ACHIVEMENT_STATE"); break;
                    case MN_ACHIVEMENT_STATE: strcpy(szMid, "MJ_ACHIVEMENT"); strcpy(szNid, "MN_ACHIVEMENT_STATE"); break;
                    case MN_GET_ACHIVEMENT_REWARD: strcpy(szMid, "MJ_ACHIVEMENT"); strcpy(szNid, "MN_GET_ACHIVEMENT_REWARD"); break;
                    case MN_ACHIVEMENT_REWARD_RESULT: strcpy(szMid, "MJ_ACHIVEMENT"); strcpy(szNid, "MN_ACHIVEMENT_REWARD_RESULT"); break;
                }
            }
            break;
        }
    }
};
#endif
/*protocol maker V1.0 authed by msl*/