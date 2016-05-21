/*�������ɹ������ɣ������ֶ��޸ģ��мǣ��мǣ�����Ҫ����Ҫ͵����*/
#ifndef __DDZPROT_MAKE_BY_PT_13161_H__
#define __DDZPROT_MAKE_BY_PT_13161_H__
#include "ustring.h"
#include "binary.h"
#include "msg_object_base.h"
#include "mvector.h"

#pragma warning(disable:4996)
/*
�޶���¼��
---2014-6-21------------------------------------------------------
1. ���Ӽ���������
---2014-5-3------------------------------------------------------
1. ���ӽ��ͬ��ˢ��Э��
---2014-4-26-----------------------------------------------------
3. ��ͨ��������Ϸ����ʱ���Ʋ�����
2. ����PK���ȷ�У��Э�飬����Ϸ�����������·�У����ֵ����Ϊ˫�����ܴ�����һ���ķ�ֵС��PKֵ��
1. ���������ӷ������������ԣ���������׿������������죬ͷ������֣�����2Ȧ��ȡ��ͷ�����֣�һ�ֽ�������������λ
*/

#define LOGIN_RESULT_OK             0  //��¼�ɹ�
#define LOGIN_RESULT_SYS_BUSY       1  //ϵͳ��æ
#define LOGIN_RESULT_IN_OTHER_ROOM  2  //����������
#define LOGIN_RESULT_ACCOUNT_IN_USE 3  //�ʺ�����ʹ��
#define LOGIN_RESULT_PAUSE          4  //ϵͳ��ͣ����
#define LOGIN_RESULT_LESS_CONDITION 5  //��������
#define LOGIN_RESULT_LESS_COIN      6  //��Ϸ�Ҳ���
#define LOGIN_RESULT_OPENID_FAIL    7  //���ƴ���
#define LOGIN_RESULT_DISENABLE      8  //�ʺŷ�
#define LOGIN_RESULT_MORE_COIN      9  //��ҹ���
#define LOGIN_RESULT_NEED_VIP       10  //��¼��ҪVIP���
#define LOGIN_RESULT_MATCH_END      11  //��¼�ı����Ѿ�����
#define CREATE_PK_GAME              0  //����һ��PK����
#define ADD_PK_GAME                 1  //����һ��PK����
#define USER_STATE_FREE             0  //�û�����״̬
#define USER_STATE_READY            1  //�û�׼��״̬
#define USER_STATE_GAMEING          2  //�û���Ϸ״̬
#define USER_STATE_NET_CUT          3  //�û�����״̬
#define GETONLINE_RESULT_OK         0  //��ȡ��ʱ�������ݳɹ�
#define GAMEEND_NORMAL              0  //��Ϸ��������
#define GAMEEND_GIVEUP              1  //���˳�����Լս����
#define GAMEEND_USER_LEFT           2  //��Ϸ��ǿ��
#define GAMEEND_DISMISS             3  //����
#define GAME_TYPE_NORMAL            0  //��ͨ��Ϸ����
#define GAME_TYPE_MATCH             1  //������Ϸ����
#define GAME_TYPE_PK                4  //PK��Ϸ����
#define MATCH_USER_STATE_WAIT       0  //����״̬���ȴ�
#define MATCH_USER_STATE_GAMING     1  //����״̬����Ϸ��
#define MATCH_USER_STATE_OUT        2  //����״̬����̭
/*---------------------------------------- ----------------------------------------*/
struct RankInfo;
struct PropItem;
struct DeskRankItem;
//������Ϣ
struct RankInfo
{
    int m_uid;//�û�ID
    int m_point;//����
    char m_state;//״̬
    ustring<32,unsigned short> m_nickname;//�ǳ�
    short m_rank;//����
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
//������Ϣ
struct PropItem
{
    int m_pid;//����ID
    int m_pn;//��������
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
    short m_rank;//����
    int m_point;//��������
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
#define MJ_GAME_SERVER                 1 //��������Э��

#define MN_PING                        1 //����
#define MN_PONG                        2 //�����ظ�
#define MN_LOGIN                      10 //��½
#define MN_LOGIN_RESULT               11 //��½���ؽ��
#define MN_CUT_OFF                    12 //������֪ͨ�ͻ��˶Ͽ�����

/*---------------------------------------- ----------------------------------------*/
#define MJ_GAME_CONTROL_DDZ           10 //��Ϸ����Э��

#define MN_GAME_USER_SITDOWN           1 //��������
#define MN_GAME_USER_SITDOWN_SC        2 //����֪ͨ
#define MN_GAME_USER_SITDOWN_ERR       3 //����֪ͨ
#define MN_LEFT_DESK_REQUEST           4 //�û��뿪(����)����
#define MN_USER_LEFT_DESK              5 //�û��뿪����֪ͨ
#define MN_USER_NET_CUT                6 //�û�����,ֻ����ͬ���û�
#define MN_USER_RECOME                 7 //��������
#define MN_USER_READY                  8 //�û�׼��
#define MN_FORCE_USER_LEFT_DESK        9 //֪ͨ�û�ǿ���뿪��λ
#define MN_BANKRUPT_RESPONST          10 //�Ʋ�������Ӧ
#define MN_ARRANGE_DESK               11 //֪ͨ�ͻ������ڷ�����λ

/*---------------------------------------- ----------------------------------------*/
#define MJ_GAME_LOGIC_DDZ             15 //��Ϸ�߼�Э��

#define MN_GAME_CONFIG                 1 //��Ϸ��������Ϣ
#define MN_DEAL                        2 //����
#define MN_CALL_POINT                  3 //�û��зֽ��
#define MN_USER_CALL_POINT             4 //�û��з�
#define MN_LAND_RESULT                 5 //ȷ�ϵ���
#define MN_LAND_BACK_CARDS             6 //�·�����
#define MN_GAME_CALL_DOUBLE            7 //֪ͨ�û��ӱ�
#define MN_GAME_DOUBLE_RESULT          8 //�ӱ����,�ͻ��˷���������Э��
#define MN_GAME_OUT_CARD_RESULT        9 //֪ͨ����
#define MN_GAME_OUT_CARD              10 //��ҳ���
#define MN_GAME_OUT_CARD_ERR          11 //���ƴ�����ʾ
#define MN_GAME_END                   12 //��Ϸ����
#define MN_RECOME_STATE               13 //��������״̬
#define MN_RECOME_STATE_CALL_POINT    14 //�зֽ׶�����������Ϣ
#define MN_RECOME_STATE_CALL_DOUBLE   15 //�ӱ��׶ζ�������������Ϣ
#define MN_RECOME_STATE_OUTCARDS      16 //���ƽ׶ζ�������������Ϣ
#define MN_USER_AUTO_STATE            17 //�޸��й�״̬,�������ͻ��˹���
#define MN_LZ_CARD                    18 //�·��������
#define MN_CALL_MINGPAI               19 //�·��û�����
#define MN_MINGPAI_CARD               20 //�·��û�����
#define NM_USER_STATE                 21 //�û�״̬
#define MN_SYN_OUTCARD                22 //ͬ���Ѿ�������ƣ������������·�
#define MN_SYN_COIN                   23 //
#define MN_SET_CALL_POINT             24 //���ýз���
#define MN_CARD_MISSION_INFO          25 //�ƾ��������������ƺ��·�
#define MN_CARD_MISSION_COMPLISH      26 //�ƾ�����������������ʱ����û��ɲ���

/*---------------------------------------- ----------------------------------------*/
#define MJ_MESSAGE_DDZ_GAME           16 //��Ϣ��Э��

#define MN_SYSTEM_MESSAGE              1 //ϵͳ��Ϣ
#define MN_GAME_MESSAGE                2 //��Ϸ��Ϣ
#define MN_CHAT_MESSAGE                3 //������Ϣ

/*---------------------------------------- ----------------------------------------*/
#define MJ_PK_GAME                    17 //2�˳�Э��

#define MN_SELECT_MODE                 1 //ѡ��ģʽ
#define MN_CREATE_PK_RESULT            2 //��������
#define MN_ADD_PK_RESULT               3 //���뷵�ؽ��
#define MN_SET_PK_POINT                4 //����PK������ֻ�з�����������
#define MN_SET_PK_POINT_RESULT         5 //����PK�������
#define MN_AGREE_SET_PK_POINT          6 //ȷ��PK��������
#define MN_GIVEUP                      7 //Ͷ��
#define MN_PK_END                      8 //��������
#define MN_PK_POINT_ADJUST             9 //����PK��ֵ������Ϸ����ʱ�������·�PKֵ����Ϊ���˻��PKֵ��

/*---------------------------------------- ----------------------------------------*/
#define MJ_MISSION_DDZ_SERVER         18 //����

#define MN_TIME_COUNT_CFG_RESULT       1 //��ʱ��������
#define MN_GET_AWARD                   2 //�콱
#define MN_GET_AWARD_RESULT            3 //�콱
#define MN_GAME_AWARD_INFO             4 //��Ϸ�н���֪ͨ�������������·����ͻ��˲�����

/*---------------------------------------- ----------------------------------------*/
#define MJ_MATCH_DDZ                  19 //����Э����

#define MN_GET_RANK_INFO               1 //�ȴ����棬��ҳ����ʱ���ͣ������ǰҳ�������ҳʱ����������
#define MN_MATCH_RANK_INFO             2 //������Ϣ,�ȴ�������û�ֻ����ҳ���û���ҳʱ���������Ƭ��
#define MN_MATCH_END                   3 //��������֪ͨ
#define MN_MATCH_WAITING               4 //֪ͨ�û��ȴ���ʼ
#define MN_RANK_CHANGE                 5 //�������η����仯(ֻ�б����û�)
#define MN_MATCH_BASE                  6 //�����׷֣�����ǰ����
#define MN_LOGIN_MATCH                 7 //��¼����

//����
class mn_ping : public msg_object_base
{
public:
    int m_value;//PINGֵ
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

//�����ظ�
class mn_pong : public msg_object_base
{
public:
    int m_value;//PINGֵ����
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

//��½
class cs_login : public msg_object_base
{
public:
    char m_ClientTpye;//�ͻ�������
    ustring<64,unsigned short> m_openid;//����ID
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

//��½���ؽ��
class sc_login_result : public msg_object_base
{
public:
    char m_result;//0�ɹ�,����ʧ��
    unsigned int m_userid;//�û��Լ���ID,ʧ��ʱ��Ч
    ustring<256,unsigned short> m_msg;//��½ʧ����Ϣ
    int m_serverid;//��¼ʧ��ʱ������������������serverid��0��ʾ��Ч
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

//������֪ͨ�ͻ��˶Ͽ�����
class mn_cut_off : public msg_object_base
{
public:
    char m_id;//�Ͽ�����ID
    ustring<256,unsigned short> m_msg;//�Ͽ���Ϣ����
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

//��������
class game_user_sitdown : public msg_object_base
{
public:
    unsigned int m_userid;//�û�ID
    short m_tableid;//����ID,-1ʱϵͳ����
    char m_seatid;//��λID,-1ʱϵͳ����
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

//����֪ͨ
class game_user_sitdown_sc : public msg_object_base
{
public:
    unsigned int m_userid;//�û�ID
    short m_tableid;//����ID
    char m_seatid;//��λID
    ustring<64,unsigned short> m_nickname;//�û��ǳ�
    long long m_coin;//�û����
    int m_glory;//�û�����ֵ
    int m_logoid;//�û�ͷ��
    int m_win;//ʤ����
    int m_loss;//ʧ�ܾ���
    int m_flee;//���ܾ���
    char m_vip;//VIP��־
    long long m_vipexpire;//VIP��Чʱ�䣨Сʱ��
    char m_state;//״̬
    char m_sex;//�Ա�
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

//����֪ͨ
class game_user_sitdown_err : public msg_object_base
{
public:
    char m_errid;//ʧ��ID
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

//�û��뿪(����)����
class left_desk_request : public msg_object_base
{
public:
    char m_type;//0�˳����� 1����
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

//�û��뿪����֪ͨ
class user_left_desk : public msg_object_base
{
public:
    unsigned int m_userid;//�û�ID
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

//�û�����,ֻ����ͬ���û�
class sc_user_net_cut : public msg_object_base
{
public:
    unsigned int m_userid;//�û�ID
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

//��������
class sc_user_recome : public msg_object_base
{
public:
    unsigned int m_userid;//�û�ID
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

//�û�׼��
class sc_user_ready : public msg_object_base
{
public:
    unsigned int m_userid;//�û�ID
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

//֪ͨ�û�ǿ���뿪��λ
class sc_force_user_left_desk : public msg_object_base
{
public:
    char m_reson;//�뿪����:0��Ҳ��㣬1��ҳ�������
    long long m_owncoin;//�û����Ͻ��
    long long m_limit;//����������
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

//�Ʋ�������Ӧ
class sc_bankrupt_responst : public msg_object_base
{
public:
    long long m_coin;//�������
    int m_result;//��ȡ�Ʋ���0�ɹ���1ʧ��
    ustring<128,unsigned short> m_msg;//��ʾ��
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

//֪ͨ�ͻ������ڷ�����λ
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

//��Ϸ��������Ϣ
class sc_game_config : public msg_object_base
{
public:
    short m_readytime;//׼������ʱ��
    char m_calltime;//�зֺͼӱ�����ʱ��
    char m_outtime;//���Ƶ���ʱ��
    char m_passtime;//�����޷�����ϼҵĵ���ʱ��
    char m_firstouttime;//�״γ��Ƶ���ʱ��
    char m_autoready;//����ʱ���Ƿ���ʾ׼����ť
    char m_gamemode;//��Ϸ����
    char m_laizi;//�Ƿ�֧�����
    int m_basepoint;//������
    char m_anticheat;//�����׿���,���Ϊ1��ʾ��,�������죬ͷ���ϼӷ��������֣����ܲ鿴�û���Ϣ
    char m_jipaiqi;//���������أ�1�򿪣�0�ر�
    char m_renshu;//�Ƿ���������
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

//����
class sc_deal : public msg_object_base
{
public:
    char m_seatid;//��λID
    binary<17> m_cards;//����
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

//�û��зֽ��
class sc_call_point_result : public msg_object_base
{
public:
    char m_seatid;//��ǰ�з������λ
    char m_point;//��ǰ�з�ֵ
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

//�û��з�
class cs_call_point : public msg_object_base
{
public:
    char m_point;//�з�ֵ, 0ΪPASS
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

//ȷ�ϵ���
class sc_land_result : public msg_object_base
{
public:
    char m_landload;//������λ,�������2��ʶ����
    char m_point;//���з�ֵ
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

//�·�����
class land_back_cards : public msg_object_base
{
public:
    char m_landload;//������λ,�������2��ʶ����
    char m_backcards[3];//����
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

//֪ͨ�û��ӱ�
class sc_call_double : public msg_object_base
{
public:
    char m_seat;//��λID
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

//�ӱ����,�ͻ��˷���������Э��
class double_result : public msg_object_base
{
public:
    char m_seat;//��λID
    char m_value;//�ӱ�ֵ
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

//֪ͨ����
class sc_out_card_result : public msg_object_base
{
public:
    char m_operator;//��ǰ��������λ
    char m_lastoperator;//�ϼҲ�������λ
    binary<20> m_lastcards;//�ϼҳ���,�������Ϊ0��ʾPASS
    char m_remaincards;//�ϼ�ʣ����
    char m_cardtype;//����
    char m_nlz;//��Ӹ���
    char m_slz[4];//���ԭ��
    char m_dlz[4];//�滻��
    char m_state;//����״̬��1����0������
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

//��ҳ���
class cs_out_cards : public msg_object_base
{
public:
    binary<20> m_cards;//��������,����0��ʾpass
    char m_cardtype;//����
    char m_nlz;//��Ӹ���
    char m_slz[4];//���ԭ��
    char m_dlz[4];//�滻��
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

//���ƴ�����ʾ
class sc_out_card_err : public msg_object_base
{
public:
    char m_reason;//��������: 1:�Ƿ�����, 2:�Ƿ�������
    binary<20> m_cards;//�������
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

//��Ϸ����
class sc_game_end : public msg_object_base
{
public:
    char m_first;//��һ�������Ƶ����
    char m_remain1;//ʣ�����1
    char m_remain2;//ʣ�����2
    binary<20> m_remaincards1;//ʣ�����1����
    binary<20> m_remaincards2;//ʣ�����2����
    long long m_score[3];//����
    long long m_glory[3];//����
    char m_spring;//�����ʶ
    char m_bomb;//ը������
    char m_callpoint;//�з�
    char m_double[3];//�ӱ�
    char m_reason;//����ԭ��
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

//��������״̬
class sc_recome_state : public msg_object_base
{
public:
    int m_matchid;//����ID
    int m_tableid;//����ID
    unsigned int m_seats[3];//�����û�ID
    char m_state;//��Ϸ״̬:1-�з�, 2-ũ��ӱ�, 3-��ַ�ӱ��׶�, 4-��Ϸ��
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

//�зֽ׶�����������Ϣ
class sc_recome_state_call_point : public msg_object_base
{
public:
    binary<20> m_cards;//����
    char m_call[3];//��ǰ����Ѿ��з�ֵ
    char m_operator;//��ǰ������
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

//�ӱ��׶ζ�������������Ϣ
class sc_recome_state_call_double : public msg_object_base
{
public:
    char m_landload;//����λ��
    char m_point;//�����з�
    binary<20> m_cards;//����
    char m_backcards[3];//����
    char m_double[3];//��ǰ�ӱ����
    char m_operator;//��ǰ������
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

//���ƽ׶ζ�������������Ϣ
class sc_recome_state_outcards : public msg_object_base
{
public:
    char m_landload;//����λ��
    char m_point;//�����з�
    binary<20> m_cards;//����
    char m_backcards[3];//����
    char m_lz;//���
    char m_remain[3];//ʣ������
    char m_double[3];//��ǰ�ӱ����
    char m_bomb;//��ǰը������
    char m_lastoperator;//�ϴγ�����
    binary<20> m_lastcards;//�ϴγ���
    char m_lasttype;//�ϴγ�������
    char m_cardtype;//����
    char m_nlz;//��Ӹ���
    char m_slz[4];//���ԭ��
    char m_dlz[4];//�滻��
    char m_operator;//��ǰ������,����ϴγ������ǵ�ǰ�����˵��¼�,��ô��ʾ��ǰ�����˵��ϼ���PASS
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

//�޸��й�״̬,�������ͻ��˹���
class change_auto_state : public msg_object_base
{
public:
    unsigned int m_userid;//�û�ID
    char m_tag;//�й�״̬: 1-�й�  0-ȡ���й�
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

//�·��������
class mn_lz_card : public msg_object_base
{
public:
    char m_lz;//�����
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

//�·��û�����
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

//�·��û�����
class mn_mingpai_card : public msg_object_base
{
public:
    char m_seatid;//��λID
    binary<20> m_cards;//����
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

//�û�״̬
class nm_user_state : public msg_object_base
{
public:
    char m_auto[3];//�й�״̬
    char m_net_cut[3];//����״̬
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

//ͬ���Ѿ�������ƣ������������·�
class mn_syn_outcard : public msg_object_base
{
public:
    binary<20> m_sit0;//�ϴγ���
    binary<20> m_sit1;//�ϴγ���
    binary<20> m_sit2;//�ϴγ���
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
    unsigned int m_userid;//�û�ID
    long long m_coin;//���µĽ������
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

//���ýз���
class sc_set_call_point : public msg_object_base
{
public:
    char m_seatid;//��λID
    char m_min_point;//�ɽе���С��
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

//�ƾ��������������ƺ��·�
class sc_card_mission_info : public msg_object_base
{
public:
    ustring<256,unsigned short> m_desc;//��������
    ustring<256,unsigned short> m_reward;//��������
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

//�ƾ�����������������ʱ����û��ɲ���
class sc_card_mission_complish : public msg_object_base
{
public:
    ustring<256,unsigned short> m_msg;//
    long long m_coin;//
    mvector<PropItem,5> m_prop;//���߽���
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

//ϵͳ��Ϣ
class sc_system_message : public msg_object_base
{
public:
    char m_type;//��Ϣ����
    ustring<64,unsigned short> m_title;//����
    ustring<512,unsigned short> m_content;//��Ϣ����
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

//��Ϸ��Ϣ
class sc_game_message : public msg_object_base
{
public:
    char m_type;//��Ϣ����
    ustring<512,unsigned short> m_content;//��Ϣ����
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

//������Ϣ
class sc_chat_message : public msg_object_base
{
public:
    unsigned int m_userid;//�û�ID
    ustring<512,unsigned short> m_content;//��Ϣ����
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

//ѡ��ģʽ
class sc_select_mode : public msg_object_base
{
public:
    char m_type;//PK���ͣ�0������1����
    char m_code[6];//�������������룬����ʱ����
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

//��������
class sc_create_pk_result : public msg_object_base
{
public:
    char m_result;//0�ɹ���1ʧ��
    char m_code[6];//��������������
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

//���뷵�ؽ��
class sc_add_pk_result : public msg_object_base
{
public:
    char m_result;//0�ɹ���1ʧ��
    ustring<64,unsigned short> m_msg;//ʧ����Ϣ
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

//����PK������ֻ�з�����������
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

//����PK�������
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

//ȷ��PK��������
class sc_agree_pk_point : public msg_object_base
{
public:
    char m_type;//1ȷ�ϣ�0�ܾ������ܾ�ʱ����Э�鷢����������������
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

//Ͷ��
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

//��������
class sc_pk_end : public msg_object_base
{
public:
    ustring<64,unsigned short> m_msg;//����ԭ��
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

//����PK��ֵ������Ϸ����ʱ�������·�PKֵ����Ϊ���˻��PKֵ��
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

//��ʱ��������
class sc_count_cfg_result : public msg_object_base
{
public:
    int m_time_len;//Ŀ��ʱ��
    int m_pid;//����ID
    int m_pn;//��������
    long long m_coin;//��������
    int m_seconds;//�Ѽ�����
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

//�콱
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

//�콱
class sc_get_award_result : public msg_object_base
{
public:
    char m_result;//�콱0�ɹ���1ʧ��
    ustring<64,unsigned short> m_msg;//�콱����
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

//��Ϸ�н���֪ͨ�������������·����ͻ��˲�����
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

//�ȴ����棬��ҳ����ʱ���ͣ������ǰҳ�������ҳʱ����������
class cs_get_rank_info : public msg_object_base
{
public:
    short m_page;//����ҳ��
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

//������Ϣ,�ȴ�������û�ֻ����ҳ���û���ҳʱ���������Ƭ��
class sc_match_rank_info : public msg_object_base
{
public:
    RankInfo m_myrank;//�Լ�������
    int m_users;//��ǰ�����û���
    mvector<RankInfo,30> m_rank;//
    short m_all_page;//������ҳ
    short m_page;//��ǰҳ
    int m_time_span;//������ǰ����ʱ��
    int m_all_turns;//�������ִ�
    int m_current_turns;//��ǰ�ִ�
    int m_gaming_desks;//������Ϸ�е�������
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

//��������֪ͨ
class sc_match_end : public msg_object_base
{
public:
    int m_rank;//����
    int m_coin;//��ҽ���
    mvector<PropItem,5> m_prop;//���߽���
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

//֪ͨ�û��ȴ���ʼ
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

//�������η����仯(ֻ�б����û�)
class sc_rank_change : public msg_object_base
{
public:
    short m_all;//������
    mvector<DeskRankItem,3> m_rank;//������Ϣ
    ustring<64,unsigned short> m_desc;//��������
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

//�����׷֣�����ǰ����
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

//��¼����
class cs_login_match : public msg_object_base
{
public:
    ustring<64,unsigned short> m_openid;//����ID
    int m_sn;//�������к�
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