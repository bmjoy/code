#ifndef __AI_HELPER_H__
#define __AI_HELPER_H__
#include "mddz.h"
#include "mddz_util.h"

#define ZERO(x)memset(&x,0,sizeof(x))
#define GET_ROBOT_PARAM_DEF int id, int lz, cards_buf hand_card[DDZ_SEATS], out_cards& record, HandAnalyzInfo hai[3], cards_buf& out, bool is_robot[3]
#define GET_ROBOT_PARAM_USE id, lz, hand_card, record, hai, out, is_robot
#define GET_ROBOT_PARAM_FOLLOW_DEF int id, int lz, cards_buf hand_card[DDZ_SEATS], out_cards& record, cards_buf& out, cards_buf bigger[MAX_CANDIDATE], int biglen, HandAnalyzInfo hai[DDZ_SEATS], bool is_robot[3]
#define GET_ROBOT_PARAM_FOLLOW_USE id, lz, hand_card, record, out,bigger, biglen, hai, is_robot
#define SET_ZERO(x) memset(&x, 0, sizeof(x))

#define CARDS_POWER_ONE         1
#define CARDS_POWER_PAIR        2
#define CARDS_POWER_THREE_WITH  3
#define CARDS_POWER_STRAIGHT    4
#define CARDS_POWER_STRAIGHT2   5
#define CARDS_POWER_STRAIGHT3   6
#define CARDS_POWER_BOMB_ROCKET 7


struct ai_cards_buf
{
    char cards[16];
    int  size;
    ai_cards_buf(){clear();}
    void clear(){size=0;for(int i=0;i<16;i++)cards[i]=0;}
    char& operator[](int index){return cards[index];}
    void add(char c){if(size>=16)return;cards[size]=c;size++;}
    bool get(char& c){if(size<=0)return false;size--;c=cards[size];return true;}
    int  get_cards(char* buf, int len){for(int i=0;i<size;i++)buf[i]=cards[i];return size;}
};

struct card_point
{
    char cards[4];
    int  size;
    card_point(){clear();}
    void clear(){size=0; for(int i=0;i<4; i++)cards[i]=0;}
    char& operator[](int index){return cards[index];}
    void add(char c) {if(size>=4)return;cards[size]=c;size++;}
    bool get(char& c){if(size<=0)return false;size--;c=cards[size];return true;}
    char val(){if(size<=0)return 0; return (0x0f&cards[0]);}
    int  get_cards(char* buf, int len){for(int i=0;i<size;i++)buf[i]=cards[i];return size;}
};
template<int SIZE>
struct card_item
{
    char cards[SIZE];
    int  size;
    card_item(){size=0;}
    void add(char c){if(size>=SIZE)return; cards[size]=c;size++;}
    bool get(char& c){if(size<=0)return false;size--;c=cards[size];return true;}
    int  get_cards(char* buf, int len){for(int i=0;i<size;i++)buf[i]=cards[i];return size;}
    char val(){return C_N(cards[0]);}
};

typedef card_item<1> card_single;
typedef card_item<2> card_pair;
typedef card_item<3> card_three;
typedef card_item<4> card_bomb;
typedef card_item<2> card_king;
typedef card_item<13> card_straight;
typedef card_item<20> card_straight2;
typedef card_item<16> card_straight3;


template<class T, int SIZE>
struct cards_vector
{
    T cards[SIZE];
    int size;
    cards_vector(){size=0;}
    void clear(){size=0;}
    T& operator[](int index){return cards[index];}
    void add(T c){if(size>=SIZE)return;cards[size]=c;size++;}
    int get_cards(char* buf, int len)
    {
        int ret = 0;
        for(int i=0; i<size; i++)
            ret += cards[i].get_cards(&buf[ret], len-ret);
        return ret;
    }
    void sort()
    {
        for(int i=0; i<size; i++)
        {
            for(int j=i+1; j<size; j++)
            {
                if(cards[j].val()>cards[i].val())
                {
                    T t = cards[j];
                    cards[j] = cards[i];
                    cards[i] = t;
                }
            }
        }
    }
    void resort()
    {
        int nsize = 0;
        for(int i=0; i<size; i++)
        {
            if(cards[i].size>0)
            {
                cards[nsize] = cards[i];
                nsize ++;
            }
        }
        size = nsize;
    }
    T& last(){return cards[size-1];}
    T& first(){return cards[0];}
    char maxv(){if(size==0)return 0;return cards[0].val();}
};

struct split_info
{
    cards_vector<card_single, 16>   single;
    cards_vector<card_pair, 10>     pair;
    cards_vector<card_three, 8>     three;
    cards_vector<card_bomb, 5>      bomb;
    cards_vector<card_straight, 4>  straight;
    cards_vector<card_straight2,4>  straight2;
    cards_vector<card_straight3,4>  straight3;
    card_king                       king;
    int                             size;
    int get_cards(char* buf, int len);
    void frush_size();
};

struct simple_split
{
    cards_vector<card_single, 16>   single;
    cards_vector<card_pair, 10>     pair;
    cards_vector<card_three, 8>     three;
    cards_vector<card_bomb, 5>      bomb;
    card_king                       king;
};

struct CandAnalyzInfo
{
    card_point king;
    card_point point[16];
    int size;
};

struct HandAnalyzInfo
{
    char king[2];
    char normal[20];
    int  nsize;
    char lzbuf[4];
    int  nlz;
    card_stat stat_buf[16];
    int  n_stat;
};

struct cards_info
{
    card_point cards[16];
};

void cal_cards_info(char* c, int size, cards_info& info);
void analyze_card_info(char* c, int len, CandAnalyzInfo& info);
void split_independent_cards(CandAnalyzInfo& info, split_info& split);
void split_optimal_cards(char* c, int len, split_info& split);
bool has_bigger(char* c, int size, int type, char* candidate, int len);
void analyze_handcard(char* cards, int len, int lz, HandAnalyzInfo& info);
int next_id(int id);
int pre_id(int id);
int set_out_cards(cards_buf& out, char* cards, int len, int type);
bool has_small_pair(char v, cards_info& info, char* buf, bool include_bomb=false);
int get_mini_bigger_string(char* src, int ls, char* candidate, int lc, char* buf);
void cal_power(split_info& info, int& power, int& turns);
bool get_three_with(split_info& info, char* buf, int& size, int& type);
bool get_straight(split_info& info, char* buf, int& size, int& type);
bool get_straight_with1(split_info& info, char* buf, int& size, int& type);
int get_n_three(split_info& info);
bool has_less_single(split_info& split, char v);
bool has_2(char* cards, int size);
bool has_king1(char* cards, int size);
char get_less_single(char* cards, int size, char v);
bool can_control_single(split_info& split, char v);
void cal_simple_split(char* cards, int size, simple_split& split);
bool get_bigger_three_with_one(char* cards, int size, char v, char* buf);
bool get_bigger_three_with_pair(char* cards, int size, char v, char* buf);
int  get_bigger_straight3_1(split_info& split, char v, int len, char* buf);
int  get_bigger_straight3_2(split_info& split, char v, int len, char* buf);

#define UP_PARAM_DEF int id, int lz, cards_buf hand_card[DDZ_SEATS], out_cards& record, cards_buf& out, cards_buf bigger[MAX_CANDIDATE], int biglen, HandAnalyzInfo hai[DDZ_SEATS], bool is_robot[3], split_info split[3], int power[3], int turns[3]
#define UP_PARAM_USE id, 0, hand_card, record, out, bigger, biglen, hai, is_robot, split, power, turns

bool can_next_follow_single(UP_PARAM_DEF);
bool can_pre_up_single(UP_PARAM_DEF);
bool can_next_follow_pair(UP_PARAM_DEF);

bool can_hold_all(GET_ROBOT_PARAM_DEF);
bool is_bigger_1_unless_king(char* cards, int size, char v);
bool is_bigger_2_unless_king(split_info& split, char v);
#endif//__AI_HELPER_H__