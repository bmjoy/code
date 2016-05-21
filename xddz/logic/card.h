#ifndef _CARD_H__
#define _CARD_H__

#include <cstring>
#include <iostream>

#include "stlmem.h"
using namespace std;
///不用管花色，全部使用权值,
enum CardType {c3=1,c4=2,c5=3,c6=4,c7=5,c8=6,c9=7,c10=8,cj=9,cq=10,ck=11,ca=12,c2=13,csk=14,cbk=15}; 

///普通牌直接用字节低四位即可，如果遇到癞子顶某牌的情况，字节的低四位用于存顶替的牌高四位用于存癞子的牌,例如癞子是1顶替6那么 0x16就代表1是癞子顶替6。
void printCard(const unsigned char card);
void printCard(const unsigned char* card,int len);

enum DDZ_CARD_TYPE
{
	DDZ_CT_ERROR,
    DDZ_CT_ONE,                   //单牌
    DDZ_CT_PAIR,                  //一对
    DDZ_CT_THREE,                 //三条
    DDZ_CT_STRAIGHT,              //单顺
    DDZ_CT_TWO_STRAIGHT,          //双顺
    DDZ_CT_THREE_STRAIGHT,        //三顺
    DDZ_CT_THREE_WITH_ONE,        //三带一单
    DDZ_CT_THREE_WITH_PAIR,       //三带一对
    DDZ_CT_THREE_STRAIGHT_WITH_N, //飞机带单翅膀
    DDZ_CT_THREE_STRAIGHT_WITH_NP,//飞机带对翅膀
    DDZ_CT_FOUR_WITH_TWO,         //四带两单
    DDZ_CT_FOUR_WITH_TWOPAIR,     //四带两对
    DDZ_CT_HUN_BOMB,              //混合炸弹
	DDZ_CT_CUN_BOMB,              //纯炸弹
    DDZ_CT_ROCKET,                //火箭
	DDZ_CT_LZ_BOMB,               //癞子炸弹
};

class outCard
{
public:    //出牌类型
	outCard()
	{
		memset(this,0,sizeof(outCard));
	}
	void print() const
	{	
		const static char tableCardType[][100] = {"","单牌","一对","三条","单顺","双顺","三顺","三带一","三带对"
				,"飞机带单","飞机带对","四带两单","四带两对","混合炸弹","纯炸弹","火箭","癞子炸弹"};
		cout<<tableCardType[type]<<"| ";
		printCard(oc,len);
	}
	bool operator<(const outCard&other)const
	{
		if(len<other.len)
			return true;
		if(len>other.len)
			return false;
		return memcmp(oc,other.oc,len)<0;
	}
	
	bool smallThan(const outCard&other)const
	{
		if(quan!=other.quan)
		{
			return quan<other.quan;
		}
		if(type!=other.type)
		{
			return type<other.type;
		}

		return len<other.len;

	}
	unsigned char oc[20];
	int len;
	int quan;   //标记此牌型大小的牌中最小的
	int type;
};
///首发出牌的所有可能
void firstAllOutCard(unsigned char *card,int len,my_list<outCard> &lOutCard,char lz=0);

//have为 char have[16]分别代表c3~cbk的数量
void firstAllOutCard(const char*have,my_list<outCard> &lOutCard,char lz=0);

///跟牌的所有可能
void genAllOutCard(unsigned char *card,int len,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz=0);
//have为 char have[16]分别代表c3~cbk的数量
void genAllOutCard(const char*have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz=0);

///计算手牌得分
///根据配牌结果计算手中牌状况的得分
///4个字节分布
// XXYZ   X代表预留暂时为0，Y代表 128+(炸弹数)-(10以下小牌数量)-（7以下小对子） Z代表 255-有多少手
///此模块冗余计算量较大，以时间换空间，提高效率
class haveHandCard
{
public:
	char have[16];
	bool operator<(const haveHandCard& other)const
	{
		return memcmp(have,other.have,sizeof(have))<0;
	}
};
unsigned int getHandCardQuan(const char*have,char lz,my_list<outCard> &lGoodOutCard,my_map<haveHandCard,unsigned int>& mHaveZhi);
unsigned int getHandCardQuan(const unsigned char *card,int len,char lz,my_list<outCard> &lGoodOutCard,my_map<haveHandCard,unsigned int>& mHaveZhi);
/////////////////////////////////
//其它辅助接口定义
/////////////////////////////////

///牌的排序，除了癞子其它按照大小排序
void SortCard(unsigned char *card,int len,char lz=0);


#endif
