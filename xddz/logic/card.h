#ifndef _CARD_H__
#define _CARD_H__

#include <cstring>
#include <iostream>

#include "stlmem.h"
using namespace std;
///���ùܻ�ɫ��ȫ��ʹ��Ȩֵ,
enum CardType {c3=1,c4=2,c5=3,c6=4,c7=5,c8=6,c9=7,c10=8,cj=9,cq=10,ck=11,ca=12,c2=13,csk=14,cbk=15}; 

///��ͨ��ֱ�����ֽڵ���λ���ɣ����������Ӷ�ĳ�Ƶ�������ֽڵĵ���λ���ڴ涥����Ƹ���λ���ڴ���ӵ���,���������1����6��ô 0x16�ʹ���1����Ӷ���6��
void printCard(const unsigned char card);
void printCard(const unsigned char* card,int len);

enum DDZ_CARD_TYPE
{
	DDZ_CT_ERROR,
    DDZ_CT_ONE,                   //����
    DDZ_CT_PAIR,                  //һ��
    DDZ_CT_THREE,                 //����
    DDZ_CT_STRAIGHT,              //��˳
    DDZ_CT_TWO_STRAIGHT,          //˫˳
    DDZ_CT_THREE_STRAIGHT,        //��˳
    DDZ_CT_THREE_WITH_ONE,        //����һ��
    DDZ_CT_THREE_WITH_PAIR,       //����һ��
    DDZ_CT_THREE_STRAIGHT_WITH_N, //�ɻ��������
    DDZ_CT_THREE_STRAIGHT_WITH_NP,//�ɻ����Գ��
    DDZ_CT_FOUR_WITH_TWO,         //�Ĵ�����
    DDZ_CT_FOUR_WITH_TWOPAIR,     //�Ĵ�����
    DDZ_CT_HUN_BOMB,              //���ը��
	DDZ_CT_CUN_BOMB,              //��ը��
    DDZ_CT_ROCKET,                //���
	DDZ_CT_LZ_BOMB,               //���ը��
};

class outCard
{
public:    //��������
	outCard()
	{
		memset(this,0,sizeof(outCard));
	}
	void print() const
	{	
		const static char tableCardType[][100] = {"","����","һ��","����","��˳","˫˳","��˳","����һ","������"
				,"�ɻ�����","�ɻ�����","�Ĵ�����","�Ĵ�����","���ը��","��ը��","���","���ը��"};
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
	int quan;   //��Ǵ����ʹ�С��������С��
	int type;
};
///�׷����Ƶ����п���
void firstAllOutCard(unsigned char *card,int len,my_list<outCard> &lOutCard,char lz=0);

//haveΪ char have[16]�ֱ����c3~cbk������
void firstAllOutCard(const char*have,my_list<outCard> &lOutCard,char lz=0);

///���Ƶ����п���
void genAllOutCard(unsigned char *card,int len,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz=0);
//haveΪ char have[16]�ֱ����c3~cbk������
void genAllOutCard(const char*have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz=0);

///�������Ƶ÷�
///�������ƽ������������״���ĵ÷�
///4���ֽڷֲ�
// XXYZ   X����Ԥ����ʱΪ0��Y���� 128+(ը����)-(10����С������)-��7����С���ӣ� Z���� 255-�ж�����
///��ģ������������ϴ���ʱ�任�ռ䣬���Ч��
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
//���������ӿڶ���
/////////////////////////////////

///�Ƶ����򣬳�������������մ�С����
void SortCard(unsigned char *card,int len,char lz=0);


#endif
