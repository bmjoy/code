#ifndef __M_DDZ_LOGIC_H__
#define __M_DDZ_LOGIC_H__
/********************************************************************
	author:		Ishaq
	
	purpose:	�������߼�, ֧������㷨
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
    MDDZ_CT_ERROR,                 //��������
    MDDZ_CT_ONE,                   //����
    MDDZ_CT_PAIR,                  //һ��
    MDDZ_CT_THREE,                 //����
    MDDZ_CT_STRAIGHT,              //��˳
    MDDZ_CT_TWO_STRAIGHT,          //˫˳
    MDDZ_CT_THREE_STRAIGHT,        //��˳
    MDDZ_CT_THREE_WITH_ONE,        //����һ��
    MDDZ_CT_THREE_WITH_PAIR,       //����һ��
    MDDZ_CT_THREE_STRAIGHT_WITH_N, //�ɻ��������
    MDDZ_CT_THREE_STRAIGHT_WITH_NP,//�ɻ����Գ��
    MDDZ_CT_FOUR_WITH_TWO,         //�Ĵ�����
    MDDZ_CT_FOUR_WITH_TWOPAIR,     //�Ĵ�����
    MDDZ_CT_BOMB,                  //ը��
    MDDZ_CT_ROCKET,                //���
};

//���ת���ṹ
struct ddz_lz_change_item
{
    char card;                //�����
    char change;              //ת�������
};
//���ת����
struct ddz_lz_item
{
    ddz_lz_change_item lz_card[4];
    int size;                 //ת������
};

//�˿˻���ṹ
struct cards_buf
{
    char cards[20]; // �ƻ���
    int  size;      // �Ƹ���
    int  card_type; // ����
    char max_card;  // �����
    ddz_lz_item clz;// ���ת����

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
ϴ�Ʋ�����
���������  
*/
int ddz_shuffle(char hand_cards[3][17], char back[3]);

/*
��ȡ����cards�����������, ��������out��
*/
void get_lz_size(char* cards, int len, int lz, ddz_lz_item* out);

/*
�ж�outcards�Ƿ�Ϊ�����д��ڵ�
*/
bool check_exist_cards(char* handcards, int len, char* outcards, int ol);
/*
���������޳�����, ����ʣ��������
*/
int  out_card(char* handcards, int len, char* outcards, int ol);

//////////////////////////////////////////////////////////////////////////
/*
������ͼ��
���ͳ������ظ���������Ƶ���, ����cards��Ϊ�滻�õ�������, out��Ϊ����滻��¼
���򷵻�0, ԭ�����в���
�����֧�����, ��lz��out��Ϊ0
*/

#define CT_PARAM_DEF char* cards, int len, int lz, ddz_lz_item* out, CardAnalyzInfo& info
#define CT_PARAM_USE cards, len, lz, out, info

char is_pair(CT_PARAM_DEF);                  //һ��
char is_three(CT_PARAM_DEF);                 //����
char is_straight(CT_PARAM_DEF);              //��˳
char is_two_straight(CT_PARAM_DEF);          //˫˳
char is_three_straight(CT_PARAM_DEF);        //��˳
char is_three_with_one(CT_PARAM_DEF);        //����һ��
char is_three_with_pair(CT_PARAM_DEF);       //����һ��
char is_three_straight_with_n(CT_PARAM_DEF); //�ɻ��������
char is_three_straight_with_np(CT_PARAM_DEF);//�ɻ����Գ��
char is_four_with_two(CT_PARAM_DEF);         //�Ĵ�����
char is_four_with_twopair(CT_PARAM_DEF);     //�Ĵ�����
char is_bomb(CT_PARAM_DEF);                  //ը��
bool is_rocket(char* cards, int len);                                              //������ܴ����, ���践�ش�Сֵ

#define MAX_CANDIDATE 100

bool check_cards_type(int type, CT_PARAM_DEF);
bool ddz_has_straight(char* cards, int len, char* buf, int& blen, int lz);
bool ddz_has_two_straight(char* cards, int len, char* buf, int& blen, int lz);

void sort_by_num(char* cards, int len);
bool analyze(char* cards, int size, int lz, CardAnalyzInfo* info);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ӿڷ�װ//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void sort_cards(char* cards, int len, int lz=0);
bool ddz_compare(cards_buf& c1, cards_buf& c2, int lz);//buf��cards,size��Ч����������
bool check_exist_cards(cards_buf& handcards, cards_buf& out);//buf��cards,size��Ч����������
int  ddz_get_cards_type_list(cards_buf& src, int lz, cards_buf buf[MAX_CANDIDATE]);
int  ddz_get_bigger_compose(cards_buf& src, cards_buf& candidate, int lz, cards_buf out[MAX_CANDIDATE]);
bool ddz_has_straight(cards_buf& src, cards_buf& out, int lz);





#endif//__M_DDZ_LOGIC_H__