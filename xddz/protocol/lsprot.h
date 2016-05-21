/*�������ɹ������ɣ������ֶ��޸ģ��мǣ��мǣ�����Ҫ����Ҫ͵����*/
#ifndef __LSPROT_MAKE_BY_PT_26639_H__
#define __LSPROT_MAKE_BY_PT_26639_H__
#include "ustring.h"
#include "binary.h"
#include "msg_object_base.h"
#include "mvector.h"

#pragma warning(disable:4996)
/*
�޶���¼��
---2014-5-3------------------------------------------------------
1. ���Ӱ汾����Э��
---2014-5-2------------------------------------------------------
2. ���Ӿ�������Э����  ZHANG
1. �����̳�����
*/

#define MAX_NAME_LEN             64  //������Ƴ���
#define CURRENCE_COIN            1  //�������ͽ��
#define CURRENCE_MONEY           2  //�������������
#define MISSION_UNDONE           0  //����δ���״̬
#define MISSION_GIFT             1  //������콱״̬
#define MISSION_DONE             2  //�������콱
#define GAME_LIST_HOT            0x01  //�����ȱ�ʶ
#define GAME_LIST_RECOMMEND      0x02  //�Ƽ������ʶ
#define LOGIN_FAIL_SYS_BUSY      1  //��¼ʧ�ܣ�ϵͳæ
#define LOGIN_FAIL_PWD_ACCOUNT   2  //��¼ʧ�ܣ��ʺŲ����ڻ��������
#define LOGIN_FAIL_TOKEN_ERROR   3  //��¼ʧ�ܣ����ƴ���
#define LOGIN_FAIL_BAN_ACCOUNT   4  //�ʺű���
#define GUEST_USER               0  //�ο��û�
#define REGIST_USER              1  //ע���û�
#define BINDER_USER              2  //���û�
#define REG_FAIL_TYPE_REGISTED   2  //ע��ʧ�ܣ���2014/5/27����ע���ʺ�
#define REG_FAIL_TYPE_BINDED     3  //ע��ʧ�ܣ����ڰ��ʺ�
#define REG_FAIL_TYPE_BUSY       4  //ע��ʧ�ܣ�ϵͳæ
#define ELO_USER_MSG             0  //������Ϣ
#define ELO_SYSTEM_MSG           1  //ϵͳ��Ϣ
#define ELO_UPAY                 0  //upay�̴�
#define ELO_ZHIFUBAO             1  //֧����
#define ELO_WANGYIN              2  //����
#define ELO_PAY_IOS              3  //IOS֧��
#define ELO_PAY_SKY              4  //˹��֧��
#define ELO_PAY_BAIDU            5  //�ٶ�֧��
#define ELO_PAY_9U               6  //����֧��
#define ELO_PAY_MM               7  //MM֧��
#define ELO_PAY_WEIMI            8  //΢��֧��
#define ELO_PAY_SMS              10  //����֧��
#define EXCHANGE_DEFAULT         0  //��ͨ���߶һ�
#define EXCHANGE_HUAFEI          1  //���Ѷһ�
#define EXCHANGE_SHIWU           2  //ʵ��һ�
#define POP_EVERYIMES            0  //ÿ�ε�½����
#define POP_LOGINTIMES           1  //ÿ�յ�½ǰN�ε���
#define EXCHANGE_QUAN            1101  //�һ�ȯID
#define CARD_COUNTER_ID          1102  //������ID
#define IS_HOT_LABEL(mask)       (mask & 0x01)  //�ȷ����ʶ
#define IS_RECOMMOND_LABEL(mask) (mask & 0x02)  //�Ƽ������ʶ
#define IS_ANTICHEAT_LABEL(mask) (mask & 0x04)  //�����׷����ʶ
#define IS_ACTIVITY_LABEL(mask)  (mask & 0x08)  //������ʶ
#define IS_TICKET_LABEL(mask)    (mask & 0x10)  //��ȯ�����ʶ
#define HTTP_TYPE_RES            1  //http��Դ��ַ����
#define WATER_PID                1103  //¶ˮ����ID
#define MATCH_TYPE_FULL          1  //������
#define MATCH_TYPE_TIME          3  //��ʱ��
#define MATCH_TYPE_TIME_POINT    4  //������
#define PAY_STATE_ALI            0x01  //����֧��
#define PAY_STATE_SMS            0x02  //����֧��
#define PAY_STATE_APPSTORE       0x04  //ƻ��֧��
#define PROP_ID_MALAMALIH_1      1400  //�μ����˷�����ID
#define PROP_ID_MALAMALIH_2      1401  //���˷�����ID
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
//��������ַ
struct ServerAddress
{
    int m_id;//����ID
    ustring<16,unsigned short> m_ip;//����IP
    int m_port;//����˿�
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
//��Ϸ�б�ڵ���Ϣ
struct GameListItem
{
    int m_id;//�ڵ�ID
    ustring<128,unsigned short> m_name;//��������
    int m_less;//��С�����
    int m_max;//����������
    unsigned int m_mask;//�ڵ�����
    int m_icon;//ͼ��ID
    int m_base;//����׷�
    mvector<ServerAddress,30> m_addr;//
    int m_defaultid;//��ݹ������ID
    ustring<64,unsigned short> m_ids;//��ݹ������ID���У���|����
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
//��Ϸ�ڵ㶯̬��Ϣ
struct GameListSubInfo
{
    int m_id;//�ڵ�ID
    int m_n;//�ڵ�����
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
//�����ڵ���Ϣ
struct MatchItem
{
    int m_id;//����ID
    ustring<32,unsigned short> m_name;//��������
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
//֧��֧�ֵĵ���
struct PropShopPayContent
{
    int m_type;//֧��ģʽ����
    ustring<16,unsigned short> m_scale;//֧����Ӧ��������������
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
//�̳ǵ�����Ϣ
struct PropGoodsItem
{
    int m_id;//����ID
    int m_rprice;//��ʵ�۸�
    ustring<16,unsigned short> m_price;//�۸�ֱ����ʾ�ַ���
    ustring<16,unsigned short> m_name;//��������
    ustring<64,unsigned short> m_title;//����
    int m_icon;//ͼƬ
    int m_mask;//��������
    ustring<16,unsigned short> m_scale;//������������
    ustring<128,unsigned short> m_desc;//������Ϣ
    mvector<PropShopPayContent,16> m_shopinfo;//֧�ֵ�֧����ʽ
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
//�̳ǵ�����Ϣ
struct PropGoodsItem2
{
    int m_id;//����ID
    int m_rprice;//��ʵ�۸�
    ustring<64,unsigned short> m_price;//�۸�ֱ����ʾ�ַ���
    ustring<64,unsigned short> m_name;//��������
    ustring<64,unsigned short> m_title;//����
    int m_icon;//ͼƬ
    int m_mask;//��������
    ustring<16,unsigned short> m_scale;//������������
    ustring<128,unsigned short> m_desc;//������Ϣ
    mvector<PropShopPayContent,16> m_shopinfo;//֧�ֵ�֧����ʽ
    char m_show_shop;//�Ƿ����̳�����ʾ
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
//���߱�����Ϣ
struct PropBagInfo
{
    unsigned int m_pid;//����ID
    int m_n;//����
    long long m_expire;//����ʱ�����-1��ʾ��������
    int m_mask;//��������
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
//�û�������Ϣ
struct ScoreInfo
{
    int m_id;//����ID
    int m_n;//��������
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
//������Ϣ
struct MissionItem
{
    int m_id;//����ID
    int m_icon;//ͼ��ID
    ustring<16,unsigned short> m_name;//��������
    char m_state;//״̬0δ��ɣ�1δ��ȡ��2����ȡ
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
//�һ���Ϣ
struct ExchangeItem
{
    int m_id;//�һ�ID
    char m_type;//�һ�����
    char m_state;//0�ɶһ���1������
    ustring<128,unsigned short> m_desc;//������Ϣ
    ustring<32,unsigned short> m_name;//����
    long long m_coin;//�������
    int m_srcid;//�һ�������ƷID
    int m_srcn;//�һ����ĸ���
    int m_dstid;//�һ���ƷID
    int m_dstn;//�һ�����
    int m_icon;//ͼ��ID
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
//��Ϣ
struct elo_message
{
    char m_type;//��Ϣ����
    int m_id;//��Ϣ����
    long long m_sendt;//����ʱ��
    long long m_expire;//����ʱ��
    ustring<256,unsigned short> m_msg;//��Ϣ����
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
//�����������״̬
struct WinCountMissionStateItem
{
    int m_id;//����ID
    int m_count;//��ɼ���
    char m_state;//״̬��0���ڽ��У�1���콱��2���콱
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
//��������������Ϣ
struct WinCountMissionConfigItem
{
    int m_id;//����ID
    int m_need;//��Ҫ��ɵ���
    int m_icon;//ͼ��ID
    ustring<64,unsigned short> m_desc;//��������
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
//��������������Ϣ
struct WinCountMissionConfigItem2
{
    int m_id;//����ID
    int m_need;//��Ҫ��ɵ���
    int m_icon;//ͼ��ID
    ustring<64,unsigned short> m_desc;//��������
    int m_type;//��������
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
//���������Ϣ
struct RankUserInfo
{
    int m_rankid;//����ID
    long long m_value;//�û�����ֵ
    char m_logoid;//ͷ��ID
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
//����������Ϣ
struct RankConfigInfo
{
    int m_type;//��������
    ustring<16,unsigned short> m_name;//��������
    int m_icon[3];//���Ǽ���ͼ��ID
    int m_pages;//ҳ��
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
//�һ���¼
struct exchange_record
{
    int m_id;//�һ�����ID
    int m_type;//�һ�����
    int m_state;//�һ�״̬0����У�1�ѷ���
    ustring<32,unsigned short> m_name;//�ջ�������
    ustring<16,unsigned short> m_phone;//�ֻ�����
    ustring<256,unsigned short> m_address;//�ջ���ַ
    long long m_sendt;//����ʱ��
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
//���ð汾��Ϣ��Ԫ
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
//ǿ����Ϣ����
struct pop_msg_item
{
    short m_poptype;//ǿ������
    unsigned int m_sortid;//��ʾ˳��
    short m_logintimes;//���յ�½����
    ustring<64,unsigned short> m_title;//ǿ������
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
//������Ϸ���õ�Ԫ
struct quick_game_item
{
    int m_from;//��С���
    int m_to;//�����
    int m_onlinelimit;//��������-1�����
    int m_nodeid;//�ڵ�id
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
//ע�ά������
struct reg_reward_item
{
    char m_type;//0:�οͽ�����1:ע�ά����2:�󶨽���
    ustring<128,unsigned short> m_desc;//����������Ϣ
    int m_icon;//ͼ��ID
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
//http��ַ����
struct http_config
{
    int m_type;//��ַ����
    ustring<256,unsigned short> m_url;//��ַ
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
//ҡǮ�����õ�Ԫ
struct MoneyTreeItem
{
    int m_level;//�ȼ�
    int m_up_point;//������
    int m_yield;//ÿСʱ����
    int m_max_yield;//������
    int m_consume;//������¶ˮ
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
//ҡǮ��״̬��Ϣ
struct TreeInfo
{
    int m_point;//�ȼ�����
    int m_yield;//��ǰ���ջ���
    int m_next_time;//��һ�β���ʱ�䣬0ʱֹͣ��ʱ�������������𣬵�0��ʱ��������������Ϣ
    int m_water_count;//���콽��¶ˮ
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
//�����������б�ڵ�
struct MatchServerItem
{
    int m_id;//��������ID
    char m_type;//��������
    ustring<16,unsigned short> m_ip;//��Ϸ������IP
    int m_port;//��Ϸ������port
    ustring<64,unsigned short> m_name;//��������
    int m_icon;//ͼ��ID
    unsigned int m_mask;//��ǩ����
    ustring<128,unsigned short> m_cdesc;//������������
    ustring<512,unsigned short> m_rdesc;//��������
    int m_coin;//������ң�0�Ļ�������
    int m_pid;//�������ߣ�0�Ļ����۷�
    int m_pn;//�������߸���
    ustring<128,unsigned short> m_fastbuy;//���ٹ���id1|id2|id3
    int m_defaultbuy;//�Ƽ�����ID
    int m_min_user;//��С��������
    int m_max_user;//���������
    int m_timespan;//���ʱ��
    ustring<32,unsigned short> m_duration;//����ʱ��
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
//������Ϣ
struct MatchInfo
{
    int m_id;//��������ID
    int m_online;//���߱�������
    int m_enroll;//��������
    int m_rest_time;//��������ʱ���룬����������
    long long m_begin_time;//����ʱ���������������
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
//������Ϣ
struct EnrollInfo
{
    int m_id;//��������ID
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
//��ֵ�ṹ��Ϣ
struct RechargeInfo
{
    ustring<64,unsigned short> m_title;//��ǩ��
    ustring<32,unsigned short> m_ids;//����ID
    int m_defaultid;//ȱʡ����ĵ���ID
    ustring<1024,unsigned short> m_desc;//�����
    char m_type;//֧������
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
//���Ϣ
struct ActionItem
{
    int m_pic;//ͼƬID
    ustring<128,unsigned short> m_title;//�����
    ustring<1024,unsigned short> m_desc;//�����<br>Ϊ���У�֧����ɫ��ǩ
    int m_click_type;//CLICK����
    ustring<256,unsigned short> m_click;//�����ת
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
//�û�����ͳ����Ϣ
struct txStaticsInfo
{
    int m_allcount;//���о���
    int m_wincount;//ʤ����
    int m_beforebet;//����ǰ��Ҹ�ע���ע�Ĵ���,��ע���μ�עֻ��һ��,��������äע����
    int m_beforeraise;//����ǰ��ע����
    int m_beforecall;//����ǰ��ע����
    int m_flopraise;//����ǰ�ܼ�ע����
    int m_flopcall;//����ǰ��ע����
    int m_turnraise;//ת�Ƽ�ע����
    int m_turncall;//ת�Ƹ�ע����
    int m_riverraise;//���Ƽ�ע����
    int m_rivercall;//���Ƹ�ע����
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
//����ͳ��
struct txCardStatics
{
    int m_royal_flush;//�ʼ�ͬ��˳
    int m_straight_flush;//ͬ��˳
    int m_four;//����
    int m_full_house;//��«
    int m_flush;//ͬ��
    int m_straight;//˳��
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
//���֧������
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
//�ɾ����
struct AchiveType
{
    int m_type;//����
    ustring<64,unsigned short> m_name;//����
    int m_logo;//ͼ��
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
//�ɾ�����
struct AchiveItem
{
    int m_type;//����
    int m_id;//�ɾ�ID
    ustring<64,unsigned short> m_name;//����
    int m_logo;//ͼ��
    int m_need_val;//�����
    ustring<128,unsigned short> m_achive_desc;//�ɾ�����
    ustring<128,unsigned short> m_reward_desc;//������ɱ
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
//�ɾ�״̬
struct AchiveState
{
    int m_id;//�ɾ�ID
    char m_state;//0δ��� 1����� 2���콱 3����
    int m_progress;//����, ��ֵΪ-1ʱ������ʾ���ȣ�ֻ��ʾδ���
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
//���������ٹ�������
struct JipaiqiFastBuyItem
{
    int m_id;//��������ID
    int m_prop_id;//����ID
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
//��ע�ά������
struct NewRegItem
{
    int m_id;//
    int m_icon;//
    ustring<32,unsigned short> m_name;//
    ustring<128,unsigned short> m_reward;//
    char m_state;//0ʱ��δ����1���ڣ�2���콱��3���콱
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
#define MJ_SYSTEM                      0 //����Э��

#define MN_LSPING                      1 //
#define MN_LSPONG                      2 //
#define MN_CHECK_VER                   3 //
#define MN_VER_RESULT                  4 //
#define MN_CONFIG_VER_INFO             5 //
#define MN_GET_REG_REWARD              6 //��ȡע�ά������
#define MN_REG_REWARD_INFO             7 //ע�ά��������Ϣ
#define MN_REG_UMENG_TOKEN             8 //ע���οͳɹ�ʱ��һ��
#define MN_GM_INFO                     9 //�ͷ���Ϣ��û���������Ӻ������·�
#define MN_REG_SOCKET_INFO            10 //�ͻ���ע��socket��Ϣ
#define MN_SYSTEM_PUSH_INFO           11 //ϵͳ��Ϣ����
#define MN_REG_SOCKET_INFO2           12 //
#define MN_REG_SOCKET_INFO3           13 //

/*---------------------------------------- ----------------------------------------*/
#define MJ_USER_ACCOUNT                1 //�˺���Э��

#define MN_REGIST                      1 //ע��
#define MN_LOGIN_BY_NAME               2 //�û�����½
#define MN_LOGIN_BY_OPENID             3 //���Ƶ�½
#define MN_LOGIN_SUCESS                4 //��½�ɹ�
#define MN_LOGIN_FAIL                  5 //��½ʧ��
#define MN_MODIFY_ACCOUNT              6 //�޸��˺�,���ο͵�ע���ʺ�����
#define MN_MODIFY_NICKNAME             7 //�޸��ǳ�
#define MN_MODIFY_LOGOID               8 //�޸�ͷ��
#define MN_MODIFY_PWD                  9 //�޸Ŀ���
#define MN_MODIFY_INFO_RESULT         10 //�޸Ľ��
#define MN_REGIST_FAIL                11 //ע��ʧ��
#define MN_ACCOUND_MODIFY_OK          12 //���ο�������ע���ʺųɹ�
#define MN_ACCOUND_MODIFY_FAIL        13 //���ο�������ע���ʺ�ʧ��
#define MN_BIND_REQUEST               14 //�ֻ�������
#define MN_BIND_RESPONSE              15 //
#define MN_BIND_COMIT                 16 //�ύ��֤��
#define MN_BIND_RESULT                17 //�󶨽��
#define MN_PWD_FIND_REQUEST           18 //�����һ�����
#define MN_PWD_FIND_RESPONSE          19 //�����һط���
#define MN_PWD_FIND_RESET             20 //������������
#define MN_PWD_FIND_RESULT            21 //��������������
#define MN_MODIFY_SEX                 22 //�޸��ǳ�
#define MN_MODIFY_SEX_RESULT          23 //�޸��ǳƽ��
#define MN_LOGIN_CONFLICT             24 //�ظ���¼

/*---------------------------------------- ----------------------------------------*/
#define MJ_USER_INFO                   2 //�û���Ϣ��Э��

#define MN_GET_MONEY_INFO              1 //��ȡ�Ƹ���Ϣ
#define MN_MONEY_INFO                  2 //�Ƹ���Ϣ
#define MN_GET_SCORE                   3 //��ȡ������Ϣ
#define MN_SCORE_INFO                  4 //������Ϣ
#define MN_GET_PROP                    5 //��ȡ������Ϣ
#define MN_PROP_INFO                   6 //������Ϣ��
#define MN_GET_ROUND_STAT              7 //��ȡ��Ϸ��Ϣ
#define MN_ROUND_STAT_INFO             8 //��Ϸ��Ϣ
#define MN_GET_RECHARGE_STAE           9 //��ȡ�ͻ���ֵ״̬
#define MN_RECHARGE_STAE              10 //�ͻ���ֵ״̬
#define MN_GET_CHARGE_STATE           11 //��ȡ�û���ֵ״̬
#define MN_CHARGE_INFO                12 //�û���ֵ״̬
#define MN_USE_PROP                   13 //
#define MN_USE_PROP_RESPONSE          14 //���ʹ�óɹ���ˢһ�±���

/*---------------------------------------- ----------------------------------------*/
#define MJ_MISSION                     4 //������Э��

#define MN_LOGIN_MISSION_STATE         2 //��¼������Ϣ
#define MN_GET_NORMAL_MISSION          3 //��ѯ��ͨ����
#define MN_NORMAL_MISSION_STATE        4 //������Ϣ
#define MN_GET_NORMAL_REWORD           5 //��ȡ������
#define MN_NORMAL_MISSION_REWORD       6 //���������
#define MN_GET_WIN_COUNT_CONFIG        7 //��ȡ������������
#define MN_WIN_COUNT_CONFIG            8 //������������
#define MN_GET_WIN_COUNT_STATE         9 //��ѯ��������״̬
#define MN_WIN_COUNT_STATE            10 //��������״̬��Ϣ
#define MN_GET_WIN_COUNT_REWARD       11 //
#define MN_WIN_COUNT_REWARD           12 //��������������
#define MN_GET_ACTION_INFO            13 //��ȡ��������
#define MN_ACTION_INFO                14 //��������
#define MN_GET_WIN_COUNT_CONFIG2      15 //��ȡ������������2
#define MN_WIN_COUNT_CONFIG2          16 //������������2
#define MN_NEW_REG_MISSION_INFO       17 //��ע��������Ϣ����¼�������·�
#define MN_GET_NEW_REG_REWARD         18 //
#define MN_NEW_REG_REWARD             19 //

/*---------------------------------------- ----------------------------------------*/
#define MJ_EXCHANGE                    6 //�һ�Э����

#define MN_GET_EXCHANGE_INFO           1 //��ȡ�һ�������Ϣ
#define MN_EXCHANGE_INFO               2 //�һ���Ʒ������Ϣ
#define MN_GET_EXCHANGE                3 //
#define MN_EXCHANGE_RESULT             4 //
#define MN_GET_ADDRESS                 5 //
#define MN_ADDRESS_INFO                6 //
#define MN_GET_EXCHANGE_RECORD         7 //
#define MN_EXCHANGE_RECORD             8 //

/*---------------------------------------- ----------------------------------------*/
#define MJ_GAME_LIST                   7 //��Ϸ�б�Э����

#define MN_GET_GAME_LIST_INFO          1 //��ȡ������Ϣ
#define MN_GAME_LIST_INFO              2 //������Ϣ
#define MN_NET_CUT_INFO                3 //����������Ϣ
#define MN_GET_GAME_SUB_INFO           4 //
#define MN_GAME_SUB_INFO               5 //
#define MN_GET_PK_GAME_INFO            6 //PK������Ϣ
#define MN_PK_GAME_INFO                7 //PK������Ϣ
#define MN_QUICK_GAME_INFO             8 //������Ϸ������Ϣ
#define MN_GET_LZ_GAME_LIST_INFO       9 //��ȡ��ӷ�����Ϣ
#define MN_LZ_GAME_LIST_INFO          10 //��ӷ�����Ϣ
#define MN_PK_GAME_LIST_INFO          11 //Լս������Ϣ
#define MN_GET_FAST_PAY_INFO          12 //
#define MN_FAST_PAY_INFO              13 //

/*---------------------------------------- ----------------------------------------*/
#define MJ_MESSAGE                     8 //��Ϣϵͳ

#define MN_GET_USER_MES                1 //��ȡ�û�������Ϣ
#define MN_USER_MES_RESULT             2 //��Ϣ����
#define MN_BULLETIN_BOARD              3 //���������Ϣ
#define MN_POP_MSG_RESULT              4 //��Ϣ����

/*---------------------------------------- ----------------------------------------*/
#define MJ_SHOP                        9 //�̳���Ϣ

#define MN_GET_SHOP_INFO               1 //��ȡ�̳ǽṹ��Ϣ
#define MN_SHOP_INFO                   2 //�̳ǵ��߽ṹ��Ϣ
#define MN_GET_RECHARGE_INFO           3 //��ȡ��ֵ����ṹ
#define MN_RECHARGE_INFO               4 //��ֵ�ṹ��Ϣ
#define MN_GET_SHOP_INFO2              5 //��ȡ�̳ǽṹ��Ϣ
#define MN_GET_RECHARGE_INFO2          6 //��ȡ��ֵ����ṹ
#define MN_GET_SHOP_INFO3              7 //��ȡ�̳ǽṹ��Ϣ
#define MN_SHOP_INFO2                  8 //�̳ǵ��߽ṹ��Ϣ
#define MN_FORCE_BUY                   9 //�ͻ���ǿ������
#define MN_GET_FAST_BUY_INFO          10 //
#define MN_FAST_BUY_INFO              11 //

/*---------------------------------------- ----------------------------------------*/
#define MJ_MASTER                     10 //������Ϣ

#define MN_GET_RANK_CONFIG             1 //��ȡ�������ͣ���Э���е�¼�ɹ�������һ��
#define MN_RANK_CONFIG                 2 //������������
#define MN_GET_RANK                    3 //��ȡ���нṹ��Ϣ
#define MN_MASTER_INFO                 4 //�������ݣ�������pageΪ0ʱ���·�����sc_rank_info��pageΪ-1�ı�ʾ�Լ�������

/*---------------------------------------- ----------------------------------------*/
#define MJ_PAYMENT                    11 //֧������

#define MN_CONSUME_CODE_REQ            1 //����������
#define MN_CONSUME_CODE_RESULT         2 //�����뷵��
#define MN_PAYMENT_RESULT              3 //web�ص�������

/*---------------------------------------- ----------------------------------------*/
#define MJ_FAQ                        12 //�û�����

#define MN_WRITE_FAQ                   1 //�ύ�û���������

/*---------------------------------------- ----------------------------------------*/
#define MJ_BANK_RUPT                  13 //�Ʋ�Э��

#define MN_BANKRUPT_REQUEST            1 //�����Ʋ���
#define MN_BANKRUPT_RESULT             2 //�Ʋ��ͽ��

/*---------------------------------------- ----------------------------------------*/
#define MJ_MONEY_TREE                 14 //ҡǮ��Э��

#define MN_GET_TREE_CONFIG             1 //��ȡҡǮ������
#define MN_TREE_CONFIG_INFO            2 //ҡǮ����Ϣ
#define MN_GET_MY_TREE                 3 //��ȡ������
#define MN_MY_TREE_INFO                4 //ҡǮ������
#define MN_YIELD_TREE_REQUEST          5 //�ջ�����
#define MN_YIELD_TREE_RESPONSE         6 //�ջ񷵻أ��ջ�ɹ�����sc_my_tree_info
#define MN_WATER_TREE                  7 //��ˮ
#define MN_WATER_RESULT                8 //��ˮ���

/*---------------------------------------- ----------------------------------------*/
#define MJ_MATCH                      15 //����Э����

#define MN_GET_MATCH_LIST              1 //��������б�
#define MN_MATCH_LIST_INFO             2 //�����б�
#define MN_MATCH_INFO                  3 //
#define MN_ATTENTION_MATCH             4 //��ע������ֻ�б����˵ı����͹�ע�ı����Ż��յ���ʱ��Ϣ�仯
#define MN_ENROLL_MATCH                5 //����
#define MN_ENROLL_RESULT               6 //�������������ɹ��������������·�sc_match_info
#define MN_WITHDRAW_MATCH              7 //����
#define MN_WITHDRAW_RESULT             8 //�������
#define MN_ENROLL_INFO                 9 //�����б�,�״�����ʱ�������������·�
#define MN_MATCH_BEGIN                10 //
#define MN_GET_MATCH_INFO             11 //

/*---------------------------------------- ----------------------------------------*/
#define MJ_SHARE                      16 //����

#define MN_GET_SHARE_INFO              1 //���������Ϣ
#define MN_SHARE_INFO                  2 //
#define MN_GET_SHARE_REWARD            3 //���������
#define MN_SHARE_REWARD_INFO           4 //

/*---------------------------------------- ----------------------------------------*/
#define MJ_TXHole_em                  17 //�����˿�Э����

#define MN_GET_SKILL_STATIC_INFO       1 //��ȡ����ͳ��
#define MN_TX_SKILL_STATIC_INFO        2 //���ݼ���ͳ����Ϣ
#define MN_GET_TX_CARD_STATIC_INFO     3 //��ȡ����ͳ��
#define MN_TX_CARD_STATIC_INFO         4 //����ͳ����Ϣ

/*---------------------------------------- ----------------------------------------*/
#define MJ_ACHIVEMENT                 18 //�ɾ�Э����

#define MN_GET_ACHIVEMENT_CFG          1 //��ȡ�ɾ�����
#define MN_ACHIVEMENT_CFG              2 //�ɾ�����
#define MN_GET_ACHIVEMENT_STATE        3 //��ȡ�ɾ�״̬
#define MN_ACHIVEMENT_STATE            4 //�ɾ�״̬��Ϣ
#define MN_GET_ACHIVEMENT_REWARD       5 //�ɾ��콱
#define MN_ACHIVEMENT_REWARD_RESULT    6 //�ɾͽ�����Ϣ

//
class mn_lsping : public msg_object_base
{
public:
    int m_t;//ʱ���
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
    int m_t;//ʱ���
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
    int m_channelid;//����ID
    char m_os;//����ϵͳ 0.android, 1.Iphone
    ustring<32,unsigned short> m_ver;//�ͻ��˰汾
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
    char m_result;//0����������1�Ƽ�������2ǿ������
    ustring<1024,unsigned short> m_desc;//�汾˵����<br>��ǩΪ���з�
    char m_urltype;//���ص�ַ���ͣ�0ֱ�����أ�1�������
    ustring<256,unsigned short> m_url;//���ص�ַ
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

//��ȡע�ά������
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

//ע�ά��������Ϣ
class sc_reg_reward_info : public msg_object_base
{
public:
    mvector<reg_reward_item,4> m_info;//������Ϣ
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

//ע���οͳɹ�ʱ��һ��
class cs_reg_umeng_token : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//����
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

//�ͷ���Ϣ��û���������Ӻ������·�
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

//�ͻ���ע��socket��Ϣ
class cs_reg_socket_info : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//����
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

//ϵͳ��Ϣ����
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
    ustring<36,unsigned short> m_openid;//����
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
    ustring<36,unsigned short> m_openid;//����
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

//ע��
class mn_regist : public msg_object_base
{
public:
    char m_force;//0Ϊ��ͨע�ᣬ1Ϊǿ��ע��
    int m_channelid;//����ID
    ustring<32,unsigned short> m_osver;//����ϵͳ�汾
    ustring<32,unsigned short> m_pixel;//�ֱ���
    ustring<32,unsigned short> m_opcode;//��Ӫ�̴���
    ustring<256,unsigned short> m_hid;//Ӳ��Ψһ��
    char m_hid_type;//Ψһ������
    ustring<32,unsigned short> m_ver;//�汾
    char m_os;//����ϵͳ 0.android, 1.Iphone
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

//�û�����½
class mn_login_by_name : public msg_object_base
{
public:
    ustring<32,unsigned short> m_name;//�û���
    char m_pwd[32];//����
    int m_channelid;//����ID
    ustring<32,unsigned short> m_ver;//�汾
    char m_os;//����ϵͳ 0.android, 1.Iphone
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

//���Ƶ�½
class mn_login_by_openid : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//����
    int m_channelid;//����ID
    ustring<32,unsigned short> m_ver;//�汾
    char m_os;//����ϵͳ 0.android, 1.Iphone
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

//��½�ɹ�
class mn_login_sucess : public msg_object_base
{
public:
    int m_type;//�ʺ�����
    unsigned int m_uid;//�û�ID
    ustring<32,unsigned short> m_nickname;//
    int m_logoid;//ͷ��ID
    ustring<36,unsigned short> m_openid;//����
    char m_vip;//VIP��־
    long long m_vipexpire;//VIP��Чʱ�䣨Сʱ��
    char m_sex;//�Ա�0Ů��1��
    ustring<16,unsigned short> m_mobile;//��֤�ֻ�
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

//��½ʧ��
class mn_login_fail : public msg_object_base
{
public:
    char m_result;//ʧ������
    ustring<128,unsigned short> m_msg;//ʧ������
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

//�޸��˺�,���ο͵�ע���ʺ�����
class mn_modify_account : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//����
    ustring<32,unsigned short> m_name;//�û���
    char m_pwd[32];//����
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

//�޸��ǳ�
class mn_modify_nickname : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//����
    ustring<32,unsigned short> m_nickname;//�ǳ�
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

//�޸�ͷ��
class mn_modify_logoid : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//����
    int m_logoid;//ͷ��ID
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

//�޸Ŀ���
class mn_modify_pwd : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//����
    char m_oldpwd[32];//�ɿ���
    char m_pwd[32];//�¿���
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

//�޸Ľ��
class mn_modify_info_result : public msg_object_base
{
public:
    char m_result;//0�ɹ�������ʧ�ܣ�255Ϊmsg
    ustring<128,unsigned short> m_msg;//ʧ������
    int m_id;//�޸�����
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

//ע��ʧ��
class mn_regist_fail : public msg_object_base
{
public:
    char m_type;//ʧ������
    ustring<128,unsigned short> m_msg;//ʧ������
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

//���ο�������ע���ʺųɹ�
class sc_accound_modify_ok : public msg_object_base
{
public:
    int m_type;//�ʺ�����
    long long m_coin;//�������
    int m_pid;//��������ID
    int m_pn;//������������
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

//���ο�������ע���ʺ�ʧ��
class sc_accound_modify_fail : public msg_object_base
{
public:
    char m_result;//ʧ������,openidʧЧ��1�Ѿ���ע���ʺ�, 2�������� 3ϵͳæ 4�û�����ͻ
    ustring<128,unsigned short> m_msg;//ʧ������
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

//�ֻ�������
class cs_bind_request : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//����
    ustring<16,unsigned short> m_phone;//�ֻ�����
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
    char m_result;//0�ɹ�������ʧ��,1�ʺ��Ѱ󶨣�2�ֻ��Ѱ󶨣�3δ֪����4���������ʺſ���
    ustring<128,unsigned short> m_msg;//ʧ������
    int m_codetime;//���ŵ���ʱ
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

//�ύ��֤��
class cs_bind_comit : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//����
    ustring<8,unsigned short> m_code;//��֤��
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

//�󶨽��
class sc_bind_result : public msg_object_base
{
public:
    char m_result;//0�ɹ�������ʧ��,1��֤�����,2��֤���Ѿ����ڣ�3δ֪����
    int m_type;//�ʺ�����
    long long m_coin;//��ҽ���
    int m_pid;//����ID
    int m_pn;//��������
    ustring<128,unsigned short> m_msg;//����
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

//�����һ�����
class cs_pwd_find_request : public msg_object_base
{
public:
    ustring<16,unsigned short> m_phone;//�ֻ�����
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

//�����һط���
class sc_pwd_find_response : public msg_object_base
{
public:
    char m_result;//0�ɹ���1�ֻ��Ų�����, 2�ظ��ύ��3δ֪����
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

//������������
class cs_pwd_find_reset : public msg_object_base
{
public:
    ustring<16,unsigned short> m_phone;//�ֻ�����
    ustring<8,unsigned short> m_code;//��֤��
    char m_pwd[32];//�¿���
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

//��������������
class sc_pwd_find_result : public msg_object_base
{
public:
    char m_result;//0�ɹ���1.��֤����ڣ�2.��֤����� 3.δ֪����
    ustring<36,unsigned short> m_name;//�û���
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

//�޸��ǳ�
class cs_modify_sex : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//����
    char m_sex;//�Ա�
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

//�޸��ǳƽ��
class sc_modify_sex_result : public msg_object_base
{
public:
    char m_result;//0�ɹ���1ʧ��
    char m_sex;//�Ա�
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

//�ظ���¼
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

//��ȡ�Ƹ���Ϣ
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

//�Ƹ���Ϣ
class sc_money_info : public msg_object_base
{
public:
    long long m_coin;//���
    int m_vmoney;//�����
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

//��ȡ������Ϣ
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

//������Ϣ
class sc_score_info : public msg_object_base
{
public:
    unsigned int m_uid;//�û�ID
    mvector<ScoreInfo,30> m_scoreinfo;//������Ϣ��
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

//��ȡ������Ϣ
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

//������Ϣ��
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

//��ȡ��Ϸ��Ϣ
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

//��Ϸ��Ϣ
class sc_round_stat_info : public msg_object_base
{
public:
    int m_win;//ʤ����
    int m_loss;//ʧ�ܾ���
    int m_flee;//���ܾ���
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

//��ȡ�ͻ���ֵ״̬
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

//�ͻ���ֵ״̬
class sc_recharge_stae : public msg_object_base
{
public:
    char m_smspay;//�����׳� 0�׳� ��������
    char m_alipay;//֧�����׳� 0�׳� ��������
    int m_recharge_total;//��ֵ�ܶ�
    int m_vip_month;//������VIP����
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

//��ȡ�û���ֵ״̬
class cs_get_charge_state : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    char m_os;//�ͻ������ͣ�0��׿��1ƻ����2WP
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

//�û���ֵ״̬
class sc_charge_info : public msg_object_base
{
public:
    unsigned int m_charge_mask;//��ֵ״̬λ��
    int m_total;//��ֵ�ܶ�
    int m_vip_month;//������VIP����
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

//���ʹ�óɹ���ˢһ�±���
class sc_use_prop_response : public msg_object_base
{
public:
    char m_result;//0�ɹ���1���߲��㣬2�����Ѿ�ʹ�ù�������ϵͳ����
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

//��¼������Ϣ
class mn_online_mission_state : public msg_object_base
{
public:
    mvector<MissionItem,10> m_mission_normal;//��ͨ��½����
    mvector<MissionItem,10> m_mission_vip;//vip��½����
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

//��ѯ��ͨ����
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

//������Ϣ
class mn_normal_mission_state : public msg_object_base
{
public:
    mvector<MissionItem,20> m_mission;//��������
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

//��ȡ������
class mn_get_normal_mission_reword : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_id;//����ID
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

//���������
class mn_normal_mission_reword : public msg_object_base
{
public:
    int m_id;//����ID
    int m_result;//0�ɹ���1ʧ��
    int m_num;//�콱���(���)
    ustring<32,unsigned short> m_desc;//��������
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

//��ȡ������������
class cs_win_count_config : public msg_object_base
{
public:
    unsigned int m_ver;//���ð汾
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

//������������
class sc_win_count_config : public msg_object_base
{
public:
    unsigned int m_ver;//���ð汾
    mvector<WinCountMissionConfigItem,10> m_config;//������Ϣ
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

//��ѯ��������״̬
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

//��������״̬��Ϣ
class sc_win_count_state : public msg_object_base
{
public:
    mvector<WinCountMissionStateItem,10> m_info;//����״̬��Ϣ
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

//��������������
class sc_win_count_reward : public msg_object_base
{
public:
    char m_result;//0�ɹ���1δ��ɣ�2�Ѿ����콱
    int m_id;//����ID
    long long m_coin;//��ҽ���
    int m_pid;//����ID
    int m_pn;//��������
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

//��ȡ��������
class cs_get_action_info : public msg_object_base
{
public:
    char m_os;//����ϵͳ����
    unsigned int m_ver;//���ð汾
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

//��������
class sc_action_info : public msg_object_base
{
public:
    unsigned int m_ver;//���ð汾
    mvector<ActionItem,10> m_info;//������Ϣ
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

//��ȡ������������2
class cs_win_count_config2 : public msg_object_base
{
public:
    unsigned int m_ver;//���ð汾
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

//������������2
class sc_win_count_config2 : public msg_object_base
{
public:
    unsigned int m_ver;//���ð汾
    mvector<WinCountMissionConfigItem2,10> m_config;//������Ϣ
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

//��ע��������Ϣ����¼�������·�
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
    int m_id;//����ID
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

//��ȡ�һ�������Ϣ
class cs_get_exchange_info : public msg_object_base
{
public:
    unsigned int m_ver;//�汾
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

//�һ���Ʒ������Ϣ
class sc_exchange_info : public msg_object_base
{
public:
    unsigned int m_ver;//�汾
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
    int m_id;//�һ�����ID
    int m_n;//�һ�����
    ustring<16,unsigned short> m_phone;//�ֻ�����
    ustring<32,unsigned short> m_name;//�ջ�������
    ustring<256,unsigned short> m_address;//�ջ���ַ
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
    char m_result;//0�ɹ���1ʧ��
    long long m_coin;//
    int m_srcid;//�һ�������ƷID
    int m_srcn;//�һ����ĸ���
    int m_dstid;//�һ���ƷID
    int m_dstn;//�һ�����
    ustring<128,unsigned short> m_msg;//������Ϣ
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
    char m_result;//0�ɹ���1ʧ��
    ustring<32,unsigned short> m_name;//�ջ�������
    ustring<16,unsigned short> m_phone;//�ֻ�����
    ustring<256,unsigned short> m_address;//�ջ���ַ
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
    char m_result;//0�ɹ���1ʧ��
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

//��ȡ������Ϣ
class mn_get_game_list_info : public msg_object_base
{
public:
    unsigned int m_ver;//�汾
    char m_code;//ƽ̨�룬0��׿��1ƻ��
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

//������Ϣ
class mn_game_list_info : public msg_object_base
{
public:
    unsigned int m_ver;//�汾
    mvector<GameListItem,30> m_list;//������Ϣ�б�
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

//����������Ϣ
class mn_net_cut_info : public msg_object_base
{
public:
    int m_gameid;//��ϷID
    int m_gamename;//��Ϸ����
    int m_id;//����ID
    ustring<32,unsigned short> m_name;//��������
    ustring<16,unsigned short> m_ip;//����IP
    int m_port;//����˿�
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

//PK������Ϣ
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

//PK������Ϣ
class mn_pk_game_info : public msg_object_base
{
public:
    ustring<16,unsigned short> m_ip;//IP
    int m_port;//����˿�
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

//������Ϸ������Ϣ
class mn_quick_game_info : public msg_object_base
{
public:
    mvector<quick_game_item,30> m_list;//������Ϸ�����б�
    char m_type;//0��ʾ��ͨ����1��ʾ��ӳ�
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

//��ȡ��ӷ�����Ϣ
class cs_get_lz_game_list_info : public msg_object_base
{
public:
    unsigned int m_ver;//�汾
    char m_code;//ƽ̨�룬0��׿��1ƻ��
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

//��ӷ�����Ϣ
class sc_lz_game_list_info : public msg_object_base
{
public:
    unsigned int m_ver;//�汾
    mvector<GameListItem,30> m_list;//������Ϣ�б�
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

//Լս������Ϣ
class sc_pk_game_list_info : public msg_object_base
{
public:
    mvector<GameListItem,30> m_list;//������Ϣ�б�
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

//��ȡ�û�������Ϣ
class cs_get_user_mes : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_umsgid;//������Ϣ��ʼ����
    int m_sysmsgid;//ϵͳ��Ϣ��ʼ����
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

//��Ϣ����
class sc_user_mes_result : public msg_object_base
{
public:
    mvector<elo_message,30> m_mlist;//��Ϣ�б�
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

//���������Ϣ
class sc_bulletin : public msg_object_base
{
public:
    int m_channelid;//���channelidΪ-1����ֱ����ʾ���������0�����ж��Ƿ����Լ���һ������һ���Ķ���
    ustring<256,unsigned short> m_msg;//������Ϣ
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

//��Ϣ����
class sc_pop_msg_result : public msg_object_base
{
public:
    mvector<pop_msg_item,5> m_pop_msg_vec;//ǿ����Ϣ����
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

//��ȡ�̳ǽṹ��Ϣ
class cs_get_shop_info : public msg_object_base
{
public:
    unsigned int m_ver;//���ر�������ð汾��û�еĻ���0
    char m_type;//����ϵͳ���ͣ�0��׿��1ƻ��
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

//�̳ǵ��߽ṹ��Ϣ
class sc_shop_info : public msg_object_base
{
public:
    unsigned int m_ver;//���ð汾
    mvector<PropGoodsItem,50> m_goodsinfo;//���߶���
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

//��ȡ��ֵ����ṹ
class cs_get_recharge_info : public msg_object_base
{
public:
    unsigned int m_ver;//���ر�������ð汾��û�еĻ���0
    char m_type;//����ϵͳ���ͣ�0��׿��1ƻ��
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

//��ֵ�ṹ��Ϣ
class sc_recharge_info : public msg_object_base
{
public:
    unsigned int m_ver;//���ð汾
    mvector<RechargeInfo,8> m_rechargeinfo;//��ֵ����
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

//��ȡ�̳ǽṹ��Ϣ
class cs_get_shop_info2 : public msg_object_base
{
public:
    unsigned int m_ver;//���ر�������ð汾��û�еĻ���0
    ustring<32,unsigned short> m_cver;//�ͻ��˰汾
    char m_type;//����ϵͳ���ͣ�0��׿��1ƻ����2 WP8
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

//��ȡ��ֵ����ṹ
class cs_get_recharge_info2 : public msg_object_base
{
public:
    unsigned int m_ver;//���ر�������ð汾��û�еĻ���0
    char m_type;//����ϵͳ���ͣ�0��׿��1ƻ����2 WP8
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

//��ȡ�̳ǽṹ��Ϣ
class cs_get_shop_info3 : public msg_object_base
{
public:
    unsigned int m_ver;//���ر�������ð汾��û�еĻ���0
    ustring<32,unsigned short> m_cver;//�ͻ��˰汾
    char m_type;//����ϵͳ���ͣ�0��׿��1ƻ����2 WP8
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

//�̳ǵ��߽ṹ��Ϣ
class sc_shop_info2 : public msg_object_base
{
public:
    unsigned int m_ver;//���ð汾
    mvector<PropGoodsItem2,100> m_goodsinfo;//���߶���
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

//�ͻ���ǿ������
class sc_force_buy : public msg_object_base
{
public:
    ustring<256,unsigned short> m_ids;//����ID�б�1|2|3
    int m_did;//Ĭ��ѡ��
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
    int m_os;//ƽ̨���ͣ�0��׿��1IOS��2WP8
    int m_chid;//����ID
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

//��ȡ�������ͣ���Э���е�¼�ɹ�������һ��
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

//������������
class sc_rank_config : public msg_object_base
{
public:
    int m_ver;//
    mvector<RankConfigInfo,10> m_config;//��������
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

//��ȡ���нṹ��Ϣ
class cs_get_rank : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_type;//��������
    int m_page;//ҳ�룬Ĭ��Ϊ0
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

//�������ݣ�������pageΪ0ʱ���·�����sc_rank_info��pageΪ-1�ı�ʾ�Լ�������
class sc_rank_info : public msg_object_base
{
public:
    int m_page;//ҳ��
    int m_type;//��������
    mvector<RankUserInfo,50> m_ranklist;//��������
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

//����������
class cs_consume_code : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_type;//֧������
    unsigned int m_propid;//����ID
    int m_channelid;//����ID
    int m_child_id;//������ID
    ustring<32,unsigned short> m_opcode;//��Ӫ�̴���
    int m_areaid;//����ID
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

//�����뷵��
class sc_consume_code_result : public msg_object_base
{
public:
    char m_result;//0�ɹ���1ʧ��
    int m_type;//֧������
    unsigned int m_propid;//����ID
    ustring<10240,unsigned short> m_cscode;//���Ѵ���
    ustring<256,unsigned short> m_ordernum;//elo������
    ustring<256,unsigned short> m_msg;//ʧ�ܻ��߳ɹ���Ϣ��ʾ
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

//web�ص�������
class sc_web_payment : public msg_object_base
{
public:
    char m_result;//���ݾ�����ʾӳ����أ�������
    ustring<256,unsigned short> m_msg;//֧��������ʾ
    unsigned int m_uid;//�û�ID
    char m_open;//�Ƿ���ʾ�򿪱�����ť��0����ʾ��1��ʾ
    ustring<256,unsigned short> m_ordernum;//������
    long long m_coin;//�û���ǰ����,���Ϊ-1������ᣬ�������0���滻�ͻ�����ʾ
    int m_vip_hour;//�����VIPСʱ����0Ϊû��
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

//�ύ�û���������
class cs_write_faq : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    ustring<512,unsigned short> m_msg;//������Ϣ
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

//�����Ʋ���
class cs_bankrupt_request : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    long long m_curcoin;//�û���ǰ������
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

//�Ʋ��ͽ��
class sc_bankrupt_result : public msg_object_base
{
public:
    char m_result;//0�ɹ���1ʧ��,2û�Ʋ�
    long long m_coin;//�û���������
    ustring<128,unsigned short> m_msg;//��ʾ��
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

//��ȡҡǮ������
class cs_get_tree_config : public msg_object_base
{
public:
    unsigned int m_ver;//���ð汾
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

//ҡǮ����Ϣ
class sc_tree_config_info : public msg_object_base
{
public:
    unsigned int m_ver;//���ð汾
    mvector<MoneyTreeItem,20> m_config;//����������
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

//��ȡ������
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

//ҡǮ������
class sc_my_tree_info : public msg_object_base
{
public:
    char m_result;//0�ɹ���1ʧ��
    ustring<128,unsigned short> m_msg;//��Ϣ
    TreeInfo m_tree;//������
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

//�ջ�����
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

//�ջ񷵻أ��ջ�ɹ�����sc_my_tree_info
class sc_yield_tree_response : public msg_object_base
{
public:
    char m_result;//0�ɹ�,����ʧ��
    ustring<128,unsigned short> m_msg;//��Ϣ
    int m_coin;//�ջ���
    int m_pid;//�ջ����ID
    int m_pn;//�ջ��������
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

//��ˮ
class cs_water_tree : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_num;//��ˮ��
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

//��ˮ���
class sc_water_result : public msg_object_base
{
public:
    char m_result;//0�ɹ�
    ustring<128,unsigned short> m_msg;//ʧ��ԭ��ʧ��ʱ����������ݺ���
    int m_point;//���ľ������
    int m_count;//���콽�����
    int m_num;//��ˮ��
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

//��������б�
class cs_get_match_list : public msg_object_base
{
public:
    unsigned int m_ver;//�б�汾
    char m_code;//ƽ̨�룬0��׿��1ƻ��
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

//�����б�
class sc_match_list : public msg_object_base
{
public:
    unsigned int m_ver;//�б�汾
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
    mvector<MatchInfo,30> m_info;//�����б�̬��Ϣ
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

//��ע������ֻ�б����˵ı����͹�ע�ı����Ż��յ���ʱ��Ϣ�仯
class cs_attention_match : public msg_object_base
{
public:
    int m_id;//һ��ֻ�ܹ�עһ��������-1ȡ����ע
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

//����
class cs_enroll_match : public msg_object_base
{
public:
    int m_id;//��������ID
    ustring<36,unsigned short> m_openid;//
    char m_paytype;//֧����ʽ��0��ң�1����
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

//�������������ɹ��������������·�sc_match_info
class sc_enroll_result : public msg_object_base
{
public:
    int m_id;//��������ID
    char m_result;//�����0�ɹ���1�ʷѲ��㣬����ʧ�ܿ�MSG
    ustring<128,unsigned short> m_msg;//ʧ����Ϣ
    int m_coin;//���Ľ��
    int m_pid;//���ĵ���ID
    int m_pn;//���ĵ��߸���
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

//����
class cs_withdraw_match : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_id;//��������ID
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

//�������
class sc_withdraw_result : public msg_object_base
{
public:
    int m_id;//��������ID
    char m_result;//0�����ɹ���������ʾmsg
    ustring<128,unsigned short> m_msg;//����ʧ����Ϣ
    int m_coin;//�������
    int m_pid;//��������ID
    int m_pn;//�������߸���
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

//�����б�,�״�����ʱ�������������·�
class sc_enroll_info : public msg_object_base
{
public:
    mvector<EnrollInfo,30> m_info;//ֻ���Ѿ�����������û�У�size=0
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
    int m_id;//��������ID���ͻ��˸������id���ҷ�����IP
    int m_sn;//�������кţ���¼��Ϸ������ʱʹ��
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

//���������Ϣ
class cs_get_share_info : public msg_object_base
{
public:
    char m_code;//ƽ̨�룬0��׿��1ƻ��
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
    ustring<512,unsigned short> m_desc;//������˵��
    ustring<128,unsigned short> m_title;//����
    ustring<256,unsigned short> m_content;//����
    ustring<256,unsigned short> m_url;//���ӵ�ַ
    ustring<256,unsigned short> m_pic;//ͼƬ
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

//���������
class cs_get_share_reward : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//����
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
    ustring<128,unsigned short> m_msg;//����˵��
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

//��ȡ����ͳ��
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

//���ݼ���ͳ����Ϣ
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

//��ȡ����ͳ��
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

//����ͳ����Ϣ
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

//��ȡ�ɾ�����
class cs_get_achivement_cfg : public msg_object_base
{
public:
    unsigned int m_ver;//���ð汾
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

//�ɾ�����
class sc_achivement_info : public msg_object_base
{
public:
    unsigned int m_ver;//���ð汾
    mvector<AchiveType,10> m_type;//����
    mvector<AchiveItem,100> m_item;//��ϸ
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

//��ȡ�ɾ�״̬
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

//�ɾ�״̬��Ϣ
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

//�ɾ��콱
class cs_get_achivement_reward : public msg_object_base
{
public:
    ustring<36,unsigned short> m_openid;//
    int m_id;//�ɾ�ID
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

//�ɾͽ�����Ϣ
class sc_achivement_reward_result : public msg_object_base
{
public:
    char m_result;//0�ɹ� 1δ��� 2���콱
    int m_id;//�ɾ�ID
    ustring<256,unsigned short> m_msg;//
    long long m_coin;//�������
    char m_prop;//0�޵��� 1�е��߽���
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