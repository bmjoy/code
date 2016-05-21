#ifndef __DDZ_AI_H__
#define __DDZ_AI_H__
#include "mddz.h"
class ddzai
{
public:
    ddzai();
    ~ddzai();

    //////////////////////////////////////////////////////////////////////////
    ///�̶��߼�
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
    ///AI����
    ///�������Ʒ���
    unsigned int handCardEstimate(char *card,int len);

    int  get_robot_call(int id);
    int  get_robot_out(int seatid, cards_buf& out, bool is_robot[3]);
	int  get_win_robot_out(int seatid, cards_buf& out, bool is_robot[3]);

    int  get_new_robot_out(int seatid, cards_buf& out, bool is_robot[3]);
    void setHandBackCard(char hand[][20],char back[3]);
    int  set_new_out_cards(int id, char* buf, int len);    //Ϊ������ת��

    void setHandCard(char hand[][20],char hl[3]);          //�����ػ���������
    void set_land_load_dis(int index);                     //�����ػ����õ���
private:
    int  evalutate_call(int id);
public:
    bool           m_user_win;
    cards_buf      m_hand_card[DDZ_SEATS];      //���ƣ����ʼ����0����λ
    cards_buf      m_back_card;                 //����
    out_cards      m_out_card;                  //����
    int            m_call[DDZ_SEATS];           //�з�
    int            m_landload;                  //����
    int            m_bomb[3];                   //ը������
    int            m_begin_call;
    int            m_maxProcessNodeNum;
};

#endif//__DDZ_AI_H__
