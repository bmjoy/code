#ifndef __M_DDZ_LOGIC_H__
#define __M_DDZ_LOGIC_H__
/********************************************************************
	author:		Ishaq
	
	purpose:	斗地主逻辑, 支持癞子算法
*********************************************************************/
#define INVALID_CARD (-1)
#define CARD_COUNT 54
#define CARD_Q 10
#define CARD_K 11
#define CARD_A 12
#define CARD_2 13
#define LIT_KING 14
#define BIG_KING 15


#define C_N(x) (x&0x0f)
#define C_T(x) (x&0xf0)
#define C_TYPE_LZ    0x50
#define C_TYPE_KING  0x40
#define C_TYPE_SPADE 0x30
#define C_TYPE_HEART 0x20
#define C_TYPE_CLUB  0x10
#define C_TYPE_DIAMOND  0
#define DDZ_SEATS    3

#define LZ(x) (((x)&0x0f)|0x50)


extern const char CARD_DATA[CARD_COUNT];


struct card_stat
{
    char v;
    char n;
    char c[4];
};

struct CardAnalyzInfo
{
    char normal[20];
    int  nsize;
    char lzbuf[4];
    int  nlz;
    card_stat stat_buf[16];
    int  n_stat;
    int  maxn;
};

enum DDZ_LOGIC_TYPE
{
    MDDZ_CT_ERROR,                 //错误类型
    MDDZ_CT_ONE,                   //单牌
    MDDZ_CT_PAIR,                  //一对
    MDDZ_CT_THREE,                 //三条
    MDDZ_CT_STRAIGHT,              //单顺
    MDDZ_CT_TWO_STRAIGHT,          //双顺
    MDDZ_CT_THREE_STRAIGHT,        //三顺
    MDDZ_CT_THREE_WITH_ONE,        //三带一单
    MDDZ_CT_THREE_WITH_PAIR,       //三带一对
    MDDZ_CT_THREE_STRAIGHT_WITH_N, //飞机带单翅膀
    MDDZ_CT_THREE_STRAIGHT_WITH_NP,//飞机带对翅膀
    MDDZ_CT_FOUR_WITH_TWO,         //四带两单
    MDDZ_CT_FOUR_WITH_TWOPAIR,     //四带两对
    MDDZ_CT_BOMB,                  //炸弹
    MDDZ_CT_ROCKET,                //火箭
};

//癞子转换结构
struct ddz_lz_change_item
{
    char card;                //癞子牌
    char change;              //转换后的牌
};
//癞子转换表
struct ddz_lz_item
{
    ddz_lz_change_item lz_card[4];
    int size;                 //转换个数
};

//扑克缓存结构
struct cards_buf
{
    char cards[20]; // 牌缓存
    int  size;      // 牌个数
    int  card_type; // 牌型
    char max_card;  // 最大牌
    ddz_lz_item clz;// 癞子转换表

    cards_buf()
    {
        for(int i=0; i<20; i++)cards[i] = 0;
        for(int i=0; i<sizeof(clz); i++)((char*)&clz)[i] = 0;
        size = 0;
        card_type = 0;
        max_card = 0;
    }
    cards_buf(char* c, int l)
    {
        card_type = 0;
        for(int i=0; i<l; i++)
            cards[i] = c[i];
        size = l;
    }
    cards_buf(char* c, int l, int t)
    {
        for(int i=0; i<l; i++)
            cards[i] = c[i];
        size = l;
        card_type = t;
    }
};

struct out_cards_buf : public cards_buf
{
    int   id;
};
struct out_cards
{
    out_cards_buf turns[60];
    int  size;
    int  first;
    out_cards();
    void set_first(int n);
    void add(out_cards_buf d);
    void get_last(out_cards_buf& d);
    void clear();
    int  get_next_operator(int& new_turn);
    void change_A2_2_12();
    void change_12_2_A2();
};

/*
洗牌并发牌
返回癞子牌  
*/
int ddz_shuffle(char hand_cards[3][17], char back[3]);

/*
获取牌型cards中癞子牌数量, 并保存在out中
*/
void get_lz_size(char* cards, int len, int lz, ddz_lz_item* out);

/*
判断outcards是否为手牌中存在的
*/
bool check_exist_cards(char* handcards, int len, char* outcards, int ol);
/*
从手牌中剔除出牌, 返回剩余牌张数
*/
int  out_card(char* handcards, int len, char* outcards, int ol);

//////////////////////////////////////////////////////////////////////////
/*
癞子牌型检测
牌型成立返回该牌型最大牌点数, 并且cards中为替换好的牌序列, out中为癞子替换记录
否则返回0, 原牌序列不变
如果不支持癞子, 则lz与out置为0
*/

#define CT_PARAM_DEF char* cards, int len, int lz, ddz_lz_item* out, CardAnalyzInfo& info
#define CT_PARAM_USE cards, len, lz, out, info

char is_pair(CT_PARAM_DEF);                  //一对
char is_three(CT_PARAM_DEF);                 //三条
char is_straight(CT_PARAM_DEF);              //单顺
char is_two_straight(CT_PARAM_DEF);          //双顺
char is_three_straight(CT_PARAM_DEF);        //三顺
char is_three_with_one(CT_PARAM_DEF);        //三带一单
char is_three_with_pair(CT_PARAM_DEF);       //三带一对
char is_three_straight_with_n(CT_PARAM_DEF); //飞机带单翅膀
char is_three_straight_with_np(CT_PARAM_DEF);//飞机带对翅膀
char is_four_with_two(CT_PARAM_DEF);         //四带两单
char is_four_with_twopair(CT_PARAM_DEF);     //四带两对
char is_bomb(CT_PARAM_DEF);                  //炸弹
bool is_rocket(char* cards, int len);                                              //火箭不能带癞子, 无需返回大小值

#define MAX_CANDIDATE 100

bool check_cards_type(int type, CT_PARAM_DEF);
bool ddz_has_straight(char* cards, int len, char* buf, int& blen, int lz);
bool ddz_has_two_straight(char* cards, int len, char* buf, int& blen, int lz);

void sort_by_num(char* cards, int len);
bool analyze(char* cards, int size, int lz, CardAnalyzInfo* info);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//接口封装//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void sort_cards(char* cards, int len, int lz=0);
bool ddz_compare(cards_buf& c1, cards_buf& c2, int lz);//buf中cards,size有效，其他忽略
bool check_exist_cards(cards_buf& handcards, cards_buf& out);//buf中cards,size有效，其他忽略
int  ddz_get_cards_type_list(cards_buf& src, int lz, cards_buf buf[MAX_CANDIDATE]);
int  ddz_get_bigger_compose(cards_buf& src, cards_buf& candidate, int lz, cards_buf out[MAX_CANDIDATE]);
bool ddz_has_straight(cards_buf& src, cards_buf& out, int lz);





#endif//__M_DDZ_LOGIC_H__