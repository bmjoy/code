#ifndef __X_DDZ_DESK_H__
#define __X_DDZ_DESK_H__
#include "mddz.h"
#include "ddz_def.h"
#include <vector>
#include <list>
#include <algorithm>
#include "msg_object_base.h"
#include "ddzprot.h"
#include "config_manager.h"
#include "card_mission.h"

using namespace std;

struct player_cards
{
    char cards[20];
    int  len;
};

struct out_cards_item
{
	char cards[20];
    char nlz;//��Ӹ���
    char slz[4];//���ԭ��
    char dlz[4];//�滻��
	int  len;
    int  seatid;
    int  cardtype;
};

struct out_record
{
	out_cards_item out_cards[54];
	int size;
};

class ddz_desk;

class i_gamedesk_controler
{
public:
    virtual void send(socket_id sid, char* buf, int len) = 0;
    virtual room_user* get_userinfo(socket_id sid, unsigned int uid) = 0;
	virtual room_user* get_userinfo(socket_id sid) = 0;
    virtual room_user* get_netcut_userinfo(unsigned int uid) = 0;
    virtual ddz_game_config get_config() = 0;
    virtual void set_timer(int deskid, unsigned short tid, int delay, int data) = 0;
    virtual void kill_timer(int deskid, unsigned short tid) = 0;
    virtual void clear_net_cut(unsigned int uid) = 0;
    virtual int  get_base_point() = 0;
    virtual bool on_match_play_end(unsigned int uids[3], int score[3], ddz_desk* desk) = 0;
    virtual match_user_item* get_match_user(unsigned int uid) = 0;
    virtual int  get_match_users() = 0;
    virtual RuleTurnItem* get_turn_item() = 0;
};

class ddz_desk
{
public:
    ddz_desk(void);
    ~ddz_desk(void);
    void init(i_gamedesk_controler* controler, ddz_game_config gameconfig);
    void set_index(int roomid, int deskid);
    int  get_deskid();
    
    void on_timer(unsigned short tid, int data);
    void on_recive(socket_id sid, msg_object_base* msg);
    void do_game_msg(socket_id sid, msg_object_base* msg);
	void do_pkroom_msg(socket_id sid, msg_object_base* msg);

    void on_sitdown(socket_id sid, int seatid=-1);
    void on_socket_close(socket_id sid, int seatid);
    void on_user_recome(socket_id sid, unsigned int uid, int seatid);
    void on_watcher_in(socket_id sid);
    void on_watcher_left(socket_id sid);
	void clear_watcher();

    void on_standup(int seatid);
    void on_ready(int seatid);
	void on_ready_timer(int seatid);

	int  get_empty_size();
    int  get_desk_state();
    bool is_gaming();
    void user_left(socket_id sid, unsigned int uid);
    void create_pk(socket_id sid);
    bool add_pk(socket_id sid, char* code);
    void on_dismiss();
    void set_match_user(socket_id sids[3], unsigned int uids[3], bool robot[3]);
    void clear_match_user();
    void send_rank_change();
    void on_auto(int seatid);
private:
    void kill_all_timer();
    int  get_seatid(socket_id sid);
    int  get_seatid(int uid);
    void on_game_start();
    void on_game_end(int seatid,int reason=0);
    void on_match_end(int seatid);
    void on_outcards(int seatid, char* buf, int len, int type, int istimer=false);
    void on_call_point(int seatid, int point);
    void on_robot_call(int seatid);
    void on_call_double(int seatid, int point);
    void on_call_mingpai(int seatid);
	void clear_gameinfo();
    void set_timer(unsigned short tid, int delay, int data);
    void kill_timer(unsigned short tid);
    void shuffle();
    void shuffle1();
    void shuffle2();
    void shuffle_bomb();
    void shuffle_straight();
    bool check_game_end(int seatid);
	void check_begin();
	void set_user_sitdown(socket_id sid, int seatid, room_user* user);
	void set_user_left(int seatid);
    void set_loader(int seatid, int point);
    void set_out_timer(int seatid);
    bool is_first_out(int seatid);
    bool compare_out_cards(int seatid, char* cards, int len, int cardtype);
    void clear_net_cut();
    void caculate_result(sc_game_end& endobj, int win);
    void calulate_glory(sc_game_end& endobj);
    void write_score(sc_game_end& endobj,int players = DDZ_SEATS);
    int  is_spring(int winner);
    void set_ready_timer();
    void on_game_mission(char winner);
    bool is_deep_ai();
    bool is_all_ai();
	room_user* user_info(int seatid);

    //���ͽӿڷ�װ
    void send(socket_id sid, msg_object_base* obj);
	void send_all(msg_object_base* obj);
	void send(int seatid, char* buf, int len);
    void send_watch(char* buf, int len);
    void send_all(char* buf, int len);
    void set_desk_state(int state);

    //���������װ
	void send_game_begin();		//������Ϸ��ʼϵ��
    void send_game_config(socket_id sid=INVALID_SOCKET_ID);    //���ͳ�����Ϣ
    void send_handcards();      //��������
    void send_backcards();      //���͵���
    void send_lz();             //�������
    void send_user_sitdown(int seatid);
    void send_user_left(int seatid, unsigned int uid);
    void send_left_fail(int seatid);
	void send_ready(int seatid);
	void send_outcard_error(int seatid, int errid);
	void send_sitdown_err(socket_id sid, int errid);
    //void send_call_point(int ls, int lp, int next, int cp);
    void send_set_loader();
    void send_call_double(int seatid);
    void send_back_cards();
    void send_out_cards(int seatid, char* cards, int len, int restcards, int cardtype, int next, int nlz, ddz_lz_change_item* lz_card, int state);
    void send_call_double_result(int seatid, int point);
    void send_mingpai(int seatid);
    void send_user_net_cut(unsigned int uid, int seatid);
    void send_user_recome(socket_id sid, unsigned int uid, int seatid);
    void send_game_result(sc_game_end* obj);
    void syn_outcard(player_cards c[3]);
    void send_create_pk(socket_id sid);
    void send_add_pk(socket_id sid, int result);
    void send_force_left(socket_id sid, int reson, __int64 coin, __int64 limit);
    void send_syn_coin(unsigned int uid[3], __int64 coin[3], int size);
    void send_auto_tag(int seatid, int tag);


    void on_set_pk_point(socket_id sid, int point);
    void on_agree_pk_point(socket_id sid, bool agree);
    void on_pk_start();

    void send_call_point_result(int seatid, int point);
    void send_set_call_point(int seatid);
public:
    void coin_adjust(__int64 src[3], __int64 dst[3], int loader);
    void check_limit();
    void adjust_pk_point();

    bool set_robot_sitdown(room_user* user);
    void clear_robot();
    void robot_out_card(int seatid);
    int  get_win_robot_out(int seatid, out_cards& record, cards_buf hand_card[DDZ_SEATS], cards_buf& out);
    bool get_ai_out_record(out_cards& record);
    void get_ai_hand_card(cards_buf hand_card[DDZ_SEATS]);
    void on_root_out_error(int seatid);
    void gen_card_mission();
    void send_card_mission_desc(int seatid);
    void check_card_mission(int winner);
    void arrange_out_record(out_cards_item record[3][20], int size[3]);
    bool check_card_mission_match(out_cards_item record, cm_item* item);
private:
    i_gamedesk_controler*   m_controler;	//���ݽӿ�
	ddz_game_config 	m_gameconfig;
    desk_user           m_seat[DDZ_SEATS];
    list<socket_id>     m_watcher;
    int                 m_desk_state;
    int                 m_roomid;
    int                 m_deskid;

	//////////////////////////////////////////////////////////////////////////
	///������Ϣ
	player_cards        m_hand[DDZ_SEATS];   //�������
	char                m_back[3];   //����
	out_record          m_out_record;        //�ϼҳ���
	int                 m_last_bigger;       //��һ��������
    int                 m_lz;

	//////////////////////////////////////////////////////////////////////////
	///�з���Ϣ
	int m_first_call;           //��һ���зֵ����
	int m_call[DDZ_SEATS];      //�зּ�����
	int m_last_call_point;      //��һ���зֶ�
	int m_loader;               //����
	int m_call_double[DDZ_SEATS];//�ӱ�����
    int m_mingpai[DDZ_SEATS];    //���Ƽ�¼

	//////////////////////////////////////////////////////////////////////////
	///��Ϸͳ��
	int m_call_point;               //�����з�
	int m_base_point;               //������
	int m_bomb_count;               //ը��������
	int m_operator;                 //��ǰ����Ա
	int m_out_counter[DDZ_SEATS];   //���ƴ���
    int m_user_bomb[3];             //�û�ը��ͳ��

	/////////////////////////////////////////////////////////////////////////
	///���˳�����
    char m_code[6];
    __int64  m_pk_point;

	//////////////////////////////////////////////////////////////////////////
    ///����������

    int  m_ai_mode_debug;

    int  m_mission_index[DDZ_SEATS];
};


#endif//__X_DDZ_DESK_H__