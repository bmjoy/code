#ifndef __DDZ_AI_H__
#define __DDZ_AI_H__
#include "mddz.h"
class ddzai
{
public:
    ddzai();
    ~ddzai();

    //////////////////////////////////////////////////////////////////////////
    ///固定逻辑
    void clear();
    void init(bool win=true);
    int  set_out_cards(int id, char* buf, int len);
    int  is_game_end();
    void set_land_load(int index);
    void SetMaxProNodeNum(int n);
    int  set_call(int id, int call);
    int  get_max_call();
    int  caculate_land();
    void get_land(int& id, int& point);
    int  get_rest_cards(int id);
    int  get_win_id();
    int  get_beishu();
    int  get_next_operator(int& new_turns);
    int  get_remain(int id, char* buf);
    void get_last_out(out_cards_buf& d);
    int  get_first_call();

    //////////////////////////////////////////////////////////////////////////
    ///AI策略
    ///计算手牌分数
    unsigned int handCardEstimate(char *card,int len);

    int  get_robot_call(int id);
    int  get_robot_out(int seatid, cards_buf& out, bool is_robot[3]);
	int  get_win_robot_out(int seatid, cards_buf& out, bool is_robot[3]);

    int  get_new_robot_out(int seatid, cards_buf& out, bool is_robot[3]);
    void setHandBackCard(char hand[][20],char back[3]);
    int  set_new_out_cards(int id, char* buf, int len);    //为了牌型转换

    void setHandCard(char hand[][20],char hl[3]);          //断线重回设置手牌
    void set_land_load_dis(int index);                     //断线重回设置地主
private:
    int  evalutate_call(int id);
public:
    bool           m_user_win;
    cards_buf      m_hand_card[DDZ_SEATS];      //手牌，玩家始终是0号座位
    cards_buf      m_back_card;                 //底牌
    out_cards      m_out_card;                  //出牌
    int            m_call[DDZ_SEATS];           //叫分
    int            m_landload;                  //地主
    int            m_bomb[3];                   //炸弹计数
    int            m_begin_call;
    int            m_maxProcessNodeNum;
};

#endif//__DDZ_AI_H__
