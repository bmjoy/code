

#include "card.h"
#include <iostream>
#include <algorithm>

using namespace std;

void printCard(const unsigned char card)
{
	static const char cardTable[16][8]={"","3","4","5","6","7","8","9","10","J","Q","K","A","2","小王","大王"};
	unsigned char c=card;
	if(c<16)
	{
		cout<<cardTable[c];
	}
	else
	{
		c = c&0x0f;
		cout<<"癞子"<<cardTable[c];
	}
}

void printCard(const unsigned char* card,int len)
{
	
	for(int i=0;i<len;i++)
	{
		printCard(card[i]);
		cout<<" ";
		//if((i%20)==19)
		//	cout<<endl;
	}
	//cout<<endl;
}


void SortCard(unsigned char *card,int len,char lz)
{
	unsigned char c[500];
	memcpy(c,card,len);
	sort(c,c+len);
	int index = 0;
	for(int i=0;i<len;i++)
	{
		if(c[i]!=lz)
		{
			card[index] = c[i];
			index++;
		}
	}
	while(index<=len)
	{
		card[index] = lz;
		index++;
	}
}

//N张牌中选择M张牌的所有排列组合
void getCN_M(unsigned char *setCard,int n,int index,int xNum,my_set<outCard> &lOutCard,unsigned char *xuan,int xlen)
{
	outCard temp;
	if(xNum<=0)
	{
		temp.len = xlen;
		memcpy(temp.oc,xuan,xlen);
		lOutCard.insert(temp);
	}
	else if(index<n)
	{
		xuan[xlen] = setCard[index];
		getCN_M(setCard,n,index+1,xNum-1,lOutCard,xuan,xlen+1);
		getCN_M(setCard,n,index+1,xNum,lOutCard,xuan,xlen);
	}
}

void getCN_M(unsigned char *setCard,int n,int xNum,my_set<outCard> &lOutCard)
{
	unsigned char xuan[100];
	getCN_M(setCard,n,0,xNum,lOutCard,xuan,0);
}


///首发所有单牌
void firstAllDanOutCard(const char *have,my_list<outCard> &lOutCard,char lz)
{
	outCard temp;
	temp.len=1;
	temp.type = DDZ_CT_ONE;
	for(int i=c3;i<=cbk;i++)
	{
		if(have[i]>=1)
		{
			temp.oc[0] = i;
			temp.quan = i;
			lOutCard.push_back(temp);
		}
	}
}

///首发所有对子
void firstAllDuiOutCard(const char *have,my_list<outCard> &lOutCard,char lz)
{
	outCard temp;
	temp.len=2;
	temp.type = DDZ_CT_PAIR;
	int lznum = have[lz];
	for(int i=c3;i<=c2;i++)
	{
		temp.quan = i;
		if(have[i]>=2)
		{
			temp.oc[0] = i;
			temp.oc[1] = i;
			lOutCard.push_back(temp);
		}
		if(have[i]>=1&&i!=lz&&lznum>=1&&i<=c2)
		{
			temp.oc[0] = i;
			unsigned char c = (lz<<4)|i;
			
			temp.oc[1] = c;
			lOutCard.push_back(temp);			
		}

	}
}

///首发所有三张
void firstAllSanOutCard(const char *have,my_list<outCard> &lOutCard,char lz)
{
	outCard temp;
	temp.len=3;
	temp.type = DDZ_CT_THREE;
	int lznum = have[lz];
	for(int i=c3;i<=c2;i++)
	{
		temp.quan = i;
		if(have[i]>=3)
		{
			temp.oc[0] = i;
			temp.oc[1] = i;
			temp.oc[2] = i;
			lOutCard.push_back(temp);
		}
		if(have[i]>=2&&i!=lz&&lznum>=1)
		{

			unsigned char c = (lz<<4)|i;
			temp.oc[0] = i;
			temp.oc[1] = i;
			temp.oc[2] = c;
			lOutCard.push_back(temp);			
		}
		if(have[i]>=1&&i!=lz&&lznum>=2)
		{
			
			unsigned char c = (lz<<4)|i;
			temp.oc[0] = i;
			temp.oc[1] = c;
			temp.oc[2] = c;
			lOutCard.push_back(temp);			
		}


	}
}

///单顺 5张以上,size是顺子的个数
void firstAllDanShunOutCard(const char *have,my_list<outCard> &lOutCard,char lz,int size)
{
	outCard temp;
	temp.len=size;
	temp.type = DDZ_CT_STRAIGHT;
	int lznum = have[lz];	
	for(int i=c3;i<=ca-(size-1);i++)
	{
		temp.quan = i+size-1;

		unsigned char setCard[16];   //可选集合
		int num = 0;
		for(int j=i;j<i+size;j++)
		{
			if(have[j]>0&&j!=lz)
			{
				setCard[num] = j;
				num++;
			}
		}
		int canlznum = lznum;

		for(int l = 0;l<=canlznum;l++)
		{
			int mustusenum = size-num;   //必须使用癞子的数量
			if(l<mustusenum)
			{
				continue;
			}
			if(l==0)
			{   //不使用癞子
				for(int j=i;j<i+size;j++)
				{
					temp.oc[j-i] = j ;
				}
				lOutCard.push_back(temp);
			}
			else
			{   
				if(mustusenum==l)
				{   /// 需要用癞子的用癞子
					for(int j=i;j<i+size;j++)
					{
						if((have[j]>0))
						{
							temp.oc[j-i] = j ;
						}
						else
						{
							unsigned char c = (lz<<4)|j;
							temp.oc[j-i] = c ;
						}
					}
					lOutCard.push_back(temp);
				}
				else
				{   ///一些有牌的被癞子替换
					int needH = l-mustusenum;   //被癞子替换的牌
					my_set<outCard> lXuanOutCard;
					getCN_M(setCard,num,needH,lXuanOutCard);
					for(my_set<outCard>::iterator itx = lXuanOutCard.begin();itx!=lXuanOutCard.end();itx++)
					{
						for(int j=i;j<i+size;j++)
						{
							if((have[j]==0))
							{   //必须被癞子替换
								char c = (lz<<4)|j;
								temp.oc[j-i] = c ;
							}
							else
							{
								bool tihuan = false;
								for(int m = 0;m<needH;m++)
								{
									if(itx->oc[m] == j)
									{
										tihuan = true;
										break;
									}
								}
								if(tihuan)
								{
									char c = (lz<<4)|j;
									temp.oc[j-i] = c ;
								}
								else
								{
									temp.oc[j-i] = j;
								}
							}
						}
						lOutCard.push_back(temp);
					}					
				}
			}
		}
	}
}

///双顺 6张以上,size是顺子牌的个数
void firstAllShuangShunOutCard(const char *have,my_list<outCard> &lOutCard,char lz,int size)
{
	outCard temp;
	temp.len=size;
	temp.type = DDZ_CT_TWO_STRAIGHT;
	int lznum = have[lz];	
	
	for(int i=c3;i<=ca-(size/2-1);i++)
	{
		temp.quan = i+(size/2-1);
		unsigned char setCard[100];   //可选集合
		int num = 0;
		for(int j=i;j<i+size/2;j++)
		{
			if(have[j]==1&&j!=lz)
			{   //可选择一张
				setCard[num] = j;
				num++;
			}
			else if(have[j]>=2&&j!=lz)
			{   //两张牌均可以供选择
				setCard[num] = j;
				num++;
				setCard[num] = j;
				num++;
			}
		}
		int canlznum = lznum;

		for(int l = 0;l<=canlznum;l++)
		{
			int mustusenum = size-num;   //必须使用癞子的数量
			if(l<mustusenum)
			{
				continue;
			}
			if(l==0)
			{   //不使用癞子
				int index = 0;
				for(int j=i;j<i+size/2;j++)
				{
					temp.oc[index] = j ;
					index++;
					temp.oc[index] = j ;
					index++;
				}
				lOutCard.push_back(temp);
			}
			else
			{   
				if(mustusenum==l)
				{   /// 需要用癞子的用癞子
					int index = 0;
					for(int j=i;j<i+size/2;j++)
					{
						if(have[j]==0)
						{
							unsigned char c = (lz<<4)|j;
							temp.oc[index] = c;
							index++;
							temp.oc[index] = c;
							index++;
						}
						else if(have[j]==1)
						{
							unsigned char c = (lz<<4)|j;
							temp.oc[index] = j;
							index++;
							temp.oc[index] = c;
							index++;
						}
						else
						{
							temp.oc[index] = j;
							index++;
							temp.oc[index] = j;
							index++;
						}
					}
					lOutCard.push_back(temp);
				}
				else
				{   ///一些有牌的被癞子替换
					int needH = l-mustusenum;   //被癞子替换的牌
					my_set<outCard> lXuanOutCard;
					getCN_M(setCard,num,needH,lXuanOutCard);
					for(my_set<outCard>::iterator itx = lXuanOutCard.begin();itx!=lXuanOutCard.end();itx++)
					{
						int index = 0;
						for(int j=i;j<i+size/2;j++)
						{
							int tihuan = 0;
							for(int m = 0;m<needH;m++)
							{
								if(itx->oc[m] == j)
								{
									tihuan++;
								}
							}
							int nolz = (have[j]>=2?2:have[j])-tihuan;

							for(int k=0;k<2;k++)
							{
								if(k<nolz)
								{
									temp.oc[index] = j;
									index++;
								}
								else
								{
									unsigned char c = (lz<<4)|j;
									temp.oc[index] = c;
									index++;
								}
							}
						}
						lOutCard.push_back(temp);
					}					
				}
			}
		}
	}
}

///三顺 6张以上,size是顺子牌的个数
void firstAllSanShunOutCard(const char *have,my_list<outCard> &lOutCard,char lz,int size)
{
	outCard temp;
	temp.len=size;
	temp.type = DDZ_CT_THREE_STRAIGHT;
	int lznum = have[lz];	

	for(int i=c3;i<=ca-(size/3-1);i++)
	{
		unsigned char setCard[100];   //可选集合
		int num = 0;
		temp.quan = i+(size/3-1);
		for(int j=i;j<i+size/3;j++)
		{
			if(have[j]==1&&j!=lz)
			{   //可选择一张
				setCard[num] = j;
				num++;
			}
			else if(have[j]==2&&j!=lz)
			{   //两张牌均可以供选择
				setCard[num] = j;
				num++;
				setCard[num] = j;
				num++;
			}
			else if(have[j]>=3&&j!=lz)
			{   //两张牌均可以供选择
				setCard[num] = j;
				num++;
				setCard[num] = j;
				num++;
				setCard[num] = j;
				num++;
			}
		}
		int canlznum = lznum;

		for(int l = 0;l<=canlznum;l++)
		{
			int mustusenum = size-num;   //必须使用癞子的数量
			if(l<mustusenum)
			{
				continue;
			}
			if(l==0)
			{   //不使用癞子
				int index = 0;
				for(int j=i;j<i+size/3;j++)
				{
					temp.oc[index] = j ;
					index++;
					temp.oc[index] = j ;
					index++;
					temp.oc[index] = j ;
					index++;
				}
				lOutCard.push_back(temp);
			}
			else
			{   
				if(mustusenum==l)
				{   /// 需要用癞子的用癞子
					int index = 0;
					for(int j=i;j<i+size/3;j++)
					{
						if(have[j]==0)
						{
							unsigned char c = (lz<<4)|j;
							temp.oc[index] = c;
							index++;
							temp.oc[index] = c;
							index++;
							temp.oc[index] = c;
							index++;
						}
						else if(have[j]==1)
						{
							unsigned char c = (lz<<4)|j;
							temp.oc[index] = j;
							index++;
							temp.oc[index] = c;
							index++;
							temp.oc[index] = c;
							index++;
						}
						else if(have[j]==2)
						{
							unsigned char c = (lz<<4)|j;
							temp.oc[index] = j;
							index++;
							temp.oc[index] = j;
							index++;
							temp.oc[index] = c;
							index++;
						}
						else
						{
							temp.oc[index] = j;
							index++;
							temp.oc[index] = j;
							index++;
							temp.oc[index] = j;
							index++;
						}
					}
					lOutCard.push_back(temp);
				}
				else
				{   ///一些有牌的被癞子替换
					int needH = l-mustusenum;   //被癞子替换的牌
					my_set<outCard> lXuanOutCard;
					getCN_M(setCard,num,needH,lXuanOutCard);
					for(my_set<outCard>::iterator itx = lXuanOutCard.begin();itx!=lXuanOutCard.end();itx++)
					{
						int index = 0;
						for(int j=i;j<i+size/3;j++)
						{
							int tihuan = 0;
							for(int m = 0;m<needH;m++)
							{
								if(itx->oc[m] == j)
								{
									tihuan++;
								}
							}
							int nolz = (have[j]>=3?3:have[j])-tihuan;

							for(int k=0;k<3;k++)
							{
								if(k<nolz)
								{
									temp.oc[index] = j;
									index++;
								}
								else
								{
									char c = (lz<<4)|j;
									temp.oc[index] = c;
									index++;
								}
							}
						}
						lOutCard.push_back(temp);
					}					
				}
			}
		}
	}
}


///首发 三带一
void firstAllSanOneOutCard(const char *have,my_list<outCard> &lOutCard,char lz)
{
	outCard temp;
	temp.len=4;
	temp.type = DDZ_CT_THREE_WITH_ONE;

	my_list<outCard> lSanOutCard;
	char temphave[16];
	
	firstAllSanOutCard(have,lSanOutCard,lz);
	for(my_list<outCard>::iterator it=lSanOutCard.begin();it!=lSanOutCard.end();it++)
	{
		memcpy(temphave,have,16);
		for(int i=0;i<3;i++)
		{   ///去掉此三张
			if(it->oc[i]<16)
			{
				temp.quan = it->oc[i];  //权值
				temphave[it->oc[i]]--;
			}
			else
			{
				temphave[lz]--;
			}
			temp.oc[i] = it->oc[i];
		}

		for(int i=c3;i<=cbk;i++)
		{
			if(i==it->quan)
				continue;  //不能等于自己
			if(temphave[i]>0)
			{
				temp.oc[3] = i;
				lOutCard.push_back(temp);
			}
		}
	}
}


///首发 三带对
void firstAllSanDuiOutCard(const char *have,my_list<outCard> &lOutCard,char lz)
{
	outCard temp;
	temp.len=5;
	temp.type = DDZ_CT_THREE_WITH_PAIR;

	my_list<outCard> lSanOutCard;
	char temphave[16];

	firstAllSanOutCard(have,lSanOutCard,lz);
	for(my_list<outCard>::iterator it=lSanOutCard.begin();it!=lSanOutCard.end();it++)
	{
		memcpy(temphave,have,16);
		for(int i=0;i<3;i++)
		{   ///去掉此三张
			if(it->oc[i]<16)
			{
				temp.quan = it->oc[i];
				temphave[it->oc[i]]--;
			}
			else
			{
				temphave[lz]--;
			}
			temp.oc[i] = it->oc[i];
		}
		//////////////////////////////////////////////////////////////////////////
		for(int i=c3;i<=c2;i++)
		{
			if(i==it->quan)
				continue;  //不能等于自己
			if(temphave[i]>0)
			{
				if(temphave[i]>=1&&i!=lz&&temphave[lz]>=1)
				{  
					unsigned char c = (lz<<4)|i;
					temp.oc[3] = i;
					temp.oc[4] = c;
					lOutCard.push_back(temp);
				}
				if(temphave[i]>=2)
				{
					temp.oc[3] = i;
					temp.oc[4] = i;
					lOutCard.push_back(temp);
				}
			}
		}
	}
}


///飞机带单翅膀
void firstAllFeiDanOutCard(const char *have,my_list<outCard> &lOutCard,char lz,int size)
{
	char temphave[16];
	int sansunsize = (size/4)*3;
	my_list<outCard> templOutCard;
	///利用三顺计算飞机带翅膀，计算出所有三顺的组合
	firstAllSanShunOutCard(have,templOutCard,lz,sansunsize);
	outCard temp;
	temp.len=size;	
	temp.type = DDZ_CT_THREE_STRAIGHT_WITH_N;
	for(my_list<outCard>::iterator it = templOutCard.begin();it!=templOutCard.end();it++)
	{//遍历所有的三顺情况
		memcpy(temphave,have,sizeof(temphave));
		temp.quan = it->quan+(size/4-1);
		for(int i=0;i<it->len;i++)
		{   ///去除三顺的牌
			unsigned char c = it->oc[i];
			if(c>16)
			{
				c=c>>4;
				temphave[c]--;
			}
			else
			{
				temphave[c] = 0;
			}
		}
		int dannum = size/4;
		unsigned char setCard[16];   //可选集合
		int num = 0;
		for(int i=c3;i<=cbk;i++)
		{
			if(temphave[i]>0)
			{   //可选集合
				setCard[num] = i;
				num++;
			}
		}
		
		my_set<outCard> lXuanOutCard;
		getCN_M(setCard,num,dannum,lXuanOutCard);
		memcpy(temp.oc,it->oc,sansunsize);
		for(my_set<outCard>::iterator itx = lXuanOutCard.begin();itx!=lXuanOutCard.end();itx++)
		{
			memcpy(temp.oc+sansunsize,itx->oc,dannum);
			lOutCard.push_back(temp);
		}
	}
}

///飞机带对翅膀
void firstAllFeiDuiOutCard(const char *have,my_list<outCard> &lOutCard,char lz,int size)
{   
	char temphave[16];
	int sansunsize = (size/5)*3;
	my_list<outCard> templOutCard;
	///利用三顺计算飞机带翅膀，计算出所有三顺的组合
	firstAllSanShunOutCard(have,templOutCard,lz,sansunsize);
	outCard temp;
	temp.len=size;	
	temp.type = DDZ_CT_THREE_STRAIGHT_WITH_NP;	

	for(my_list<outCard>::iterator it = templOutCard.begin();it!=templOutCard.end();it++)
	{//遍历所有的三顺情况
		memcpy(temphave,have,sizeof(temphave));
		temp.quan = it->quan+(size/5-1);
		for(int i=0;i<it->len;i++)
		{   ///去除三顺的牌
			unsigned char c = it->oc[i];
			if(c>16)
			{
				c=c>>4;
				temphave[c]--;
			}
			else
			{
				temphave[c] = 0;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		int needduinum = size/5;
		int templz = temphave[lz];
		///可选单牌集合
		unsigned char setCard[16];   //可选集合
		int num = 0;
		for(int i=c3;i<=c2;i++)
		{
			if(temphave[i]>0&&(i<(it->quan)||i>(it->quan+needduinum-1)))
			{   //可选集合
				setCard[num] = i;
				num++;
			}
		}

		my_set<outCard> lXuanOutCard;
		getCN_M(setCard,num,needduinum,lXuanOutCard);
		memcpy(temp.oc,it->oc,sansunsize);
		for(my_set<outCard>::iterator itx = lXuanOutCard.begin();itx!=lXuanOutCard.end();itx++)
		{
			int temphuanlz = templz;
			unsigned char setDanCard[16];   //可选集合
			int dannum = 0;
			for(int k=0;k<itx->len;k++)
			{
				if(itx->oc[k]==lz)
				{   /// 癞对
					temphuanlz = temphuanlz-2;
					continue;
				}
				if(temphave[itx->oc[k]]>=2)
				{   ///代表既可以一对此牌也可以单牌加癞子
					setDanCard[dannum] = itx->oc[k];
					dannum++;
				}
			}
			
			int leasthavelz = needduinum-dannum;
			if(temphuanlz<leasthavelz)
				continue;
			
			
			{
				int index = sansunsize;
				for(int k=0;k<itx->len;k++)
				{
					if(temphave[itx->oc[k]]>=2)
					{
						temp.oc[index] = itx->oc[k];
						index++;
						temp.oc[index] = itx->oc[k];
						index++;
					}
					else
					{
						unsigned char c = (lz<<4)|itx->oc[k];
						temp.oc[index] = itx->oc[k];
						index++;
						temp.oc[index] = c;
						index++;
					}
				}
				lOutCard.push_back(temp);	
			}
			
			{   /// 有些对子牌中会用癞子
				int tinum = temphuanlz - leasthavelz;
				
				for(int i=1;i<=tinum;i++)
				{
					my_set<outCard> lXuanDanOutCard;
					getCN_M(setDanCard,dannum,i,lXuanDanOutCard);
					for(my_set<outCard>::iterator itxx = lXuanDanOutCard.begin();itxx!=lXuanDanOutCard.end();itxx++)
					{
						int index = sansunsize;
						for(int k=0;k<itx->len;k++)
						{
							bool needhuanlz = false;
							for(int m=0;m<itxx->len;m++)
							{
								if(itx->oc[k]==itxx->oc[m])
								{
									needhuanlz = true;
								}
							}
							if(!needhuanlz&&temphave[itx->oc[k]]>=2)
							{
								temp.oc[index] = itx->oc[k];
								index++;
								temp.oc[index] = itx->oc[k];
								index++;
							}
							else
							{
								char c = (lz<<4)|itx->oc[k];
								temp.oc[index] = itx->oc[k];
								index++;
								temp.oc[index] = c;
								index++;
							}
						}
						lOutCard.push_back(temp);
					}

				}
			}
			//
				
		}



		//////////////////////////////////////////////////////////////////////////

	}
}

///首发所有炸弹
void firstAllBombOutCard(const char *have,my_list<outCard> &lOutCard,char lz)
{
	outCard temp;
	temp.len=4;
	int lznum = have[lz];
	for(int i=c3;i<=c2;i++)
	{
		temp.quan = i;
		
		if(have[i]>=1&&i!=lz&&lznum>=3)
		{
			
			unsigned char c = (lz<<4)|i;
			temp.oc[0] = i;
			temp.oc[1] = c;
			temp.oc[2] = c;
			temp.oc[3] = c;
			temp.type = DDZ_CT_HUN_BOMB;
			lOutCard.push_back(temp);			
		}
		if(have[i]>=2&&i!=lz&&lznum>=2)
		{
			
			unsigned char c = (lz<<4)|i;
			temp.oc[0] = i;
			temp.oc[1] = i;
			temp.oc[2] = c;
			temp.oc[3] = c;
			temp.type = DDZ_CT_HUN_BOMB;
			lOutCard.push_back(temp);			
		}
		if(have[i]>=3&&i!=lz&&lznum>=1)
		{
			
			unsigned char c = (lz<<4)|i;
			temp.oc[0] = i;
			temp.oc[1] = i;
			temp.oc[2] = i;
			temp.oc[3] = c;
			temp.type = DDZ_CT_HUN_BOMB;
			lOutCard.push_back(temp);			
		}
		if(have[i]>=4)
		{
			temp.oc[0] = i;
			temp.oc[1] = i;
			temp.oc[2] = i;
			temp.oc[3] = i;
			if(i==lz)
			{
				temp.type = DDZ_CT_LZ_BOMB;
			}
			else
			{
				temp.type = DDZ_CT_CUN_BOMB;
			}
			lOutCard.push_back(temp);
		}
	}
}
void firstAllRocketOutCard(const char *have,my_list<outCard> &lOutCard,char lz)
{
	if(have[csk]>0&&have[cbk]>0)
	{
		outCard temp;
		temp.len=2;	
		temp.quan = cbk;
		temp.oc[0] = csk;
		temp.oc[1] = cbk;
		temp.type = DDZ_CT_ROCKET;
		lOutCard.push_back(temp);
	}
}


void firstAllFourDanOutCard(const char *have,my_list<outCard> &lOutCard,char lz)
{
	outCard temp;
	temp.len=6;
	temp.type = DDZ_CT_FOUR_WITH_TWO;

	my_list<outCard> lFourOutCard;
	char temphave[16];

	//取出所有炸弹
	firstAllBombOutCard(have,lFourOutCard,lz);
	
	for(my_list<outCard>::iterator it = lFourOutCard.begin();it!=lFourOutCard.end();it++)
	{
		memcpy(temphave,have,sizeof(temphave));
		temp.quan = it->quan;
		for(int i=0;i<it->len;i++)
		{   ///去除三顺的牌
			unsigned char c = it->oc[i];
			if(c>16)
			{
				c=c>>4;
				temphave[c]--;
			}
			else
			{
				temphave[c] = 0;
			}
		}
		unsigned char setCard[16];   //可选集合
		int num = 0;
		for(int i=c3;i<=cbk;i++)
		{
			if(temphave[i]>0)
			{   //可选集合
				setCard[num] = i;
				num++;
			}
		}

		my_set<outCard> lXuanOutCard;
		getCN_M(setCard,num,2,lXuanOutCard);
		memcpy(temp.oc,it->oc,4);
		for(my_set<outCard>::iterator itx = lXuanOutCard.begin();itx!=lXuanOutCard.end();itx++)
		{
			temp.oc[4] = itx->oc[0];
			temp.oc[5] = itx->oc[1];
			lOutCard.push_back(temp);
		}
		
	}
}

//四张带两对
void firstAllFourDuiOutCard(const char *have,my_list<outCard> &lOutCard,char lz)
{
	outCard temp;
	temp.len=8;
	temp.type = DDZ_CT_FOUR_WITH_TWOPAIR;

	my_list<outCard> lFourOutCard;
	char temphave[16];

	//取出所有炸弹
	firstAllBombOutCard(have,lFourOutCard,lz);

	for(my_list<outCard>::iterator it = lFourOutCard.begin();it!=lFourOutCard.end();it++)
	{
		memcpy(temphave,have,sizeof(temphave));
		temp.quan = it->quan;
		for(int i=0;i<it->len;i++)
		{   ///去除四张的牌
			unsigned char c = it->oc[i];
			if(c>16)
			{
				c=c>>4;
				temphave[c]--;
			}
			else
			{
				temphave[c] = 0;
			}
		}
		unsigned char setCard[16];   //可选集合
		int num = 0;
		for(int i=c3;i<=c2;i++)
		{
			if(temphave[i]>0)
			{   //可选集合
				setCard[num] = i;
				num++;
			}
		}

		my_set<outCard> lXuanOutCard;
		getCN_M(setCard,num,2,lXuanOutCard);
		memcpy(temp.oc,it->oc,4);
		int lznum = temphave[lz];
		for(my_set<outCard>::iterator itx = lXuanOutCard.begin();itx!=lXuanOutCard.end();itx++)
		{
			if(temphave[itx->oc[0]]>=2&&temphave[itx->oc[1]]>=2)
			{
				temp.oc[4] = itx->oc[0];
				temp.oc[5] = itx->oc[0];
				temp.oc[6] = itx->oc[1];
				temp.oc[7] = itx->oc[1];
				lOutCard.push_back(temp);
			}
			if(lznum>=1)
			{
				if(temphave[itx->oc[1]]>=2)
				{
					unsigned char c = (lz<<4)|itx->oc[0];
					temp.oc[4] = itx->oc[0];
					temp.oc[5] = c;
					temp.oc[6] = itx->oc[1];
					temp.oc[7] = itx->oc[1];
					lOutCard.push_back(temp);
				}
				if(temphave[itx->oc[0]]>=2)
				{
					unsigned char c = (lz<<4)|itx->oc[1];
					temp.oc[4] = itx->oc[0];
					temp.oc[5] = itx->oc[0];
					temp.oc[6] = itx->oc[1];
					temp.oc[7] = c;
					lOutCard.push_back(temp);
				}
			}
			if(lznum>=2)
			{
				unsigned char c = (lz<<4)|itx->oc[0];
				temp.oc[4] = itx->oc[0];
				temp.oc[5] = c;
				c = (lz<<4)|itx->oc[1];
				temp.oc[6] = itx->oc[1];
				temp.oc[7] = c;
				lOutCard.push_back(temp);
			}
		}

	}
}

void firstAllOutCard(const char*have,my_list<outCard> &lOutCard,char lz)
{
	int len = 0;
	for(int i=c3;i<=cbk;i++)
	{
		len += have[i];
	}
	/// 飞机带单翅膀
	for(int size = 8;size<=len;size+=4)
	{
		firstAllFeiDanOutCard(have,lOutCard,lz,size);
	}
	/// 飞机带对翅膀
	for(int size = 10;size<=len;size+=5)
	{
		firstAllFeiDuiOutCard(have,lOutCard,lz,size);
	}
	/// 双顺
	for(int size = 6;size<=len;size+=2)
	{
		firstAllShuangShunOutCard(have,lOutCard,lz,size);
	}
	/// 单顺
	for(int size = 5;size<=len;size++)
	{
		firstAllDanShunOutCard(have,lOutCard,lz,size);
	}

	/// 三顺
	for(int size = 6;size<=len;size+=3)
	{
		firstAllSanShunOutCard(have,lOutCard,lz,size);
	}
	/// 三带一
	if(len>=4)
	{
		firstAllSanOneOutCard(have,lOutCard,lz);
	}
	/// 三带对
	if(len>=5)
	{
		firstAllSanDuiOutCard(have,lOutCard,lz);
	}


	/// 对子
	firstAllDuiOutCard(have,lOutCard,lz);
	/// 三张
	firstAllSanOutCard(have,lOutCard,lz);




	/// 单牌
	firstAllDanOutCard(have,lOutCard,lz);
	/// 四张带两单
	firstAllFourDanOutCard(have,lOutCard,lz);
	
	/// 四张带两对
	firstAllFourDuiOutCard(have,lOutCard,lz);
	///炸弹
	firstAllBombOutCard(have,lOutCard,lz);
	firstAllRocketOutCard(have,lOutCard,lz);	
}

void firstAllOutCard(unsigned char *card,int len,my_list<outCard> &lOutCard,char lz)
{
	char have[16];
	memset(have,0,sizeof(have));
	
	for(int i=0;i<len;i++)
	{
		char index = card[i]<16?card[i]:(card[i]&0x0f);
		have[index]++;
	}
	
	firstAllOutCard(have,lOutCard,lz);
}

void getBigOneOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// 附加上炸弹
	firstAllBombOutCard(have,lOutCard,lz);
	firstAllRocketOutCard(have,lOutCard,lz);
	my_list<outCard> lTempOutCard;
	firstAllDanOutCard(have,lTempOutCard,lz);
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		if(it->quan>lastOutCard.quan)
		{
			lOutCard.push_back(*it);
		}
	}
	
}

void getBigDuiOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// 附加上炸弹
	firstAllBombOutCard(have,lOutCard,lz);
	firstAllRocketOutCard(have,lOutCard,lz);	
	my_list<outCard> lTempOutCard;
	firstAllDuiOutCard(have,lTempOutCard,lz);
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		if(it->quan>lastOutCard.quan)
		{
			lOutCard.push_back(*it);
		}
	}

}

void getBigSanOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// 附加上炸弹
	firstAllBombOutCard(have,lOutCard,lz);
	firstAllRocketOutCard(have,lOutCard,lz);	
	my_list<outCard> lTempOutCard;
	firstAllSanOutCard(have,lTempOutCard,lz);
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		if(it->quan>lastOutCard.quan)
		{
			lOutCard.push_back(*it);
		}
	}

}
//单顺子
void getBigDanShunOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// 附加上炸弹
	firstAllBombOutCard(have,lOutCard,lz);
	firstAllRocketOutCard(have,lOutCard,lz);	
	my_list<outCard> lTempOutCard;
	firstAllDanShunOutCard(have,lTempOutCard,lz,lastOutCard.len);
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		if(it->quan>lastOutCard.quan)
		{
			lOutCard.push_back(*it);
		}
	}

}
//双顺子
void getBigShuangShunOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// 附加上炸弹
	firstAllBombOutCard(have,lOutCard,lz);
	firstAllRocketOutCard(have,lOutCard,lz);	
	my_list<outCard> lTempOutCard;
	firstAllShuangShunOutCard(have,lTempOutCard,lz,lastOutCard.len);
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		if(it->quan>lastOutCard.quan)
		{
			lOutCard.push_back(*it);
		}
	}

}
//三顺子
void getBigSanShunOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// 附加上炸弹
	firstAllBombOutCard(have,lOutCard,lz);
	firstAllRocketOutCard(have,lOutCard,lz);	
	my_list<outCard> lTempOutCard;
	firstAllSanShunOutCard(have,lTempOutCard,lz,lastOutCard.len);
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		if(it->quan>lastOutCard.quan)
		{
			lOutCard.push_back(*it);
		}
	}

}

//三带一
void getBigSanOneOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// 附加上炸弹
	firstAllBombOutCard(have,lOutCard,lz);
	firstAllRocketOutCard(have,lOutCard,lz);	
	my_list<outCard> lTempOutCard;
	firstAllSanOneOutCard(have,lTempOutCard,lz);
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		if(it->quan>lastOutCard.quan)
		{
			lOutCard.push_back(*it);
		}
	}

}

//三带对
void getBigSanDuiOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// 附加上炸弹
	firstAllBombOutCard(have,lOutCard,lz);
	firstAllRocketOutCard(have,lOutCard,lz);
	my_list<outCard> lTempOutCard;
	firstAllSanDuiOutCard(have,lTempOutCard,lz);
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		if(it->quan>lastOutCard.quan)
		{
			lOutCard.push_back(*it);
		}
	}
	
}

//飞机带单翅膀
void getBigFeiDanOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// 附加上炸弹
	firstAllBombOutCard(have,lOutCard,lz);
	firstAllRocketOutCard(have,lOutCard,lz);
	my_list<outCard> lTempOutCard;
	firstAllFeiDanOutCard(have,lTempOutCard,lz,lastOutCard.len);
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		if(it->quan>lastOutCard.quan)
		{
			lOutCard.push_back(*it);
		}
	}
	
}

//飞机带对翅膀
void getBigFeiDuiOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// 附加上炸弹
	firstAllBombOutCard(have,lOutCard,lz);
	firstAllRocketOutCard(have,lOutCard,lz);	
	my_list<outCard> lTempOutCard;
	firstAllFeiDuiOutCard(have,lTempOutCard,lz,lastOutCard.len);
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		if(it->quan>lastOutCard.quan)
		{
			lOutCard.push_back(*it);
		}
	}

}

//四张带两单
void getBigFourDanOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// 附加上炸弹
	firstAllBombOutCard(have,lOutCard,lz);
	firstAllRocketOutCard(have,lOutCard,lz);
	my_list<outCard> lTempOutCard;
	firstAllFourDanOutCard(have,lTempOutCard,lz);
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		if(it->quan>lastOutCard.quan)
		{
			lOutCard.push_back(*it);
		}
	}
	
}

//四张带两对
void getBigFourDuiOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// 附加上炸弹
	firstAllBombOutCard(have,lOutCard,lz);
	firstAllRocketOutCard(have,lOutCard,lz);	
	my_list<outCard> lTempOutCard;
	firstAllFourDuiOutCard(have,lTempOutCard,lz);
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		if(it->quan>lastOutCard.quan)
		{
			lOutCard.push_back(*it);
		}
	}

}

//炸弹处理
void getBigBombOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	my_list<outCard> lTempOutCard;
	firstAllBombOutCard(have,lTempOutCard,lz);
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		if(it->type>lastOutCard.type)
		{   //炸弹类型大
			lOutCard.push_back(*it);
		}		
		if(it->type==lastOutCard.type&&it->quan>lastOutCard.quan)
		{   //炸弹类型相等，牌型大
			lOutCard.push_back(*it);
		}
	}
	/// 如果不是癞炸，不是火箭,加上火箭
	if(DDZ_CT_LZ_BOMB!=lastOutCard.type&&DDZ_CT_ROCKET!=lastOutCard.type)
		firstAllRocketOutCard(have,lOutCard,lz);	
}

void genAllOutCard(const char*have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz)
{
	switch(lastOutCard.type)
	{
		case DDZ_CT_ONE:
		{
			getBigOneOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}
		case DDZ_CT_PAIR:
		{
			getBigDuiOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}		
		case DDZ_CT_THREE:
		{
			getBigSanOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}	
		case DDZ_CT_STRAIGHT:
		{   //跟顺子
			getBigDanShunOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}			
		case DDZ_CT_TWO_STRAIGHT:
		{   //跟双顺子
			getBigShuangShunOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}	
		case DDZ_CT_THREE_STRAIGHT:
		{   //跟三顺子
			getBigSanShunOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}	
		case DDZ_CT_THREE_WITH_ONE:
		{   //跟三带一
			getBigSanOneOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}
		case DDZ_CT_THREE_WITH_PAIR:
		{   //跟三带对
			getBigSanDuiOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}	
		case DDZ_CT_THREE_STRAIGHT_WITH_N:
		{   //跟飞机带翅膀
		
			getBigFeiDanOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}			
		case DDZ_CT_THREE_STRAIGHT_WITH_NP:
		{   //跟飞机带对翅膀
		
			getBigFeiDuiOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}	
		case DDZ_CT_FOUR_WITH_TWO:
		{   //跟四张带两单
			
			getBigFourDanOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}	
		case DDZ_CT_FOUR_WITH_TWOPAIR:
		{   //跟四张带两对
			
			getBigFourDuiOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}	
		case DDZ_CT_HUN_BOMB:
		case DDZ_CT_CUN_BOMB:
		case DDZ_CT_LZ_BOMB:
		case DDZ_CT_ROCKET:
		{   //炸弹 处理
			
			getBigBombOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}	
		default:
		{
			cout<<"error"<<endl;
		}
	}
}

void genAllOutCard(unsigned char *card,int len,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz)
{
	char have[16];
	memset(have,0,sizeof(have));
	
	for(int i=0;i<len;i++)
	{
		char index = card[i]<16?card[i]:(card[i]&0x0f);
		have[index]++;
	}
	genAllOutCard(have,lOutCard,lastOutCard,lz);
}
///不计算手牌的分数情况
unsigned int getHandCardQuan(const char*have,char lz,my_list<outCard> &lHaveHandCard,my_list<outCard> &lOutCard,unsigned int &maxret,my_list<outCard> &lGoodOutCard,my_map<haveHandCard,unsigned int>& mHaveZhi)
{
	//if(lOutCard.empty())
	//	return 0;
	haveHandCard tempHaveHandCard;
	memcpy(tempHaveHandCard.have,have,sizeof(tempHaveHandCard.have));

	my_map<haveHandCard,unsigned int>::iterator itfind = mHaveZhi.find(tempHaveHandCard);
	if(itfind!=mHaveZhi.end())
	{
		return itfind->second;
	}
	if(lOutCard.empty())
	{
		int len = 0;
		for(int i=c3;i<=cbk;i++)
		{
			if(have[i])
				len++;
			if(have[i]>2)
				return 0;   //不合理的配牌 此种情况一定不是比较好的配牌方式
		}
		if(have[csk]>0&&have[cbk]>0)
		{   //可以是火箭
			return 0;
		}
		
		int handNum = len+lHaveHandCard.size();
		int bombNum = 0;
		int sdui = 0;
		int sdan = 0;
		for(my_list<outCard>::iterator it = lHaveHandCard.begin();it!=lHaveHandCard.end();it++)
		{
			if(it->type>DDZ_CT_FOUR_WITH_TWOPAIR)
			{   //炸弹或火箭
				bombNum+=3;
			}
		}
		
		for(int i=c3;i<=c10;i++)
		{
			if(have[i]==1)
			{
				sdan++;
			}
		}
		for(int i=c3;i<=c7;i++)
		{
			if(have[i]==2)
			{
				sdui++;
			}
		}		
		
		int Y = 128+bombNum-sdui-sdan;
		int Z = 255-handNum;
		int ret = (Y<<8)+Z;
		
		if((unsigned int)ret>maxret)
		{
			maxret = ret;

			lGoodOutCard = lHaveHandCard;

			outCard temp;
			
			
			for(int i=c3;i<=cbk;i++)
			{
				if(have[i]==1)
				{
					temp.len = 1;
					temp.type = DDZ_CT_ONE;
					temp.oc[0] = i;
					lGoodOutCard.push_back(temp);
				}
				else if(have[i]==2)
				{
					temp.len = 2;
					temp.type = DDZ_CT_PAIR;
					temp.oc[0] = i;
					temp.oc[1] = i;
					lGoodOutCard.push_back(temp);		
				}
			}
			//cout<<endl;
		}
		
		/// 修改返回值类型,为have中的分数，不考虑手上牌的分数
		Y = 128-sdui-sdan;
		Z = 255-len;
		ret = (Y<<8)+Z;
		mHaveZhi[tempHaveHandCard] = ret;
		return ret;
	}
	char tempHave[16];
	memcpy(tempHave,have,sizeof(tempHave));
	my_list<outCard>::iterator it = lOutCard.begin();
	bool can = true;
	for(int i=0;i<it->len;i++)
	{
		unsigned char c = it->oc[i];
		if(c>16)
			c = lz;
		if(tempHave[c]<=0)
		{
			can = false;
			break;
		}
		tempHave[c]--;
	}
	outCard tempOutCard= lOutCard.front();
	
	int handNum = lHaveHandCard.size();
	int bombNum = 0;

	for(my_list<outCard>::iterator it = lHaveHandCard.begin();it!=lHaveHandCard.end();it++)
	{
		if(it->type>DDZ_CT_FOUR_WITH_TWOPAIR)
		{   //炸弹或火箭
			bombNum++;
		}
	}
	
	unsigned int ret2 = 0;
	if(can)
	{
		
		///包括此牌
		lHaveHandCard.push_back(tempOutCard);
		ret2 = getHandCardQuan(tempHave,lz,lHaveHandCard,lOutCard,maxret,lGoodOutCard,mHaveZhi);
		lHaveHandCard.pop_back();
		
		if(ret2!=0)
		{
			int Y = 0;
			int Z = 1;
			if(tempOutCard.type>DDZ_CT_FOUR_WITH_TWOPAIR)
			{
				Y += 3;
			}
			
			ret2 += ((Y<<8)-Z);	
		}		
	}
	lOutCard.pop_front();
	///不包括此牌
	
	unsigned int ret1 = getHandCardQuan(have,lz,lHaveHandCard,lOutCard,maxret,lGoodOutCard,mHaveZhi);
	lOutCard.push_front(tempOutCard);
	

		
	unsigned int ret =  ret1<ret2?ret2:ret1;
	mHaveZhi[tempHaveHandCard] = ret;
	return ret;
	
}

unsigned int getHandCardQuan(const char*have,char lz,my_list<outCard> &lGoodOutCard,my_map<haveHandCard,unsigned int>& mHaveZhi)
{
	my_list<outCard> lHaveHandCard;
	my_list<outCard> lOutCard;
	my_list<outCard> lTempOutCard;


	firstAllOutCard(have,lTempOutCard,lz);
	
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		//去掉单牌组合
		if(DDZ_CT_ONE!=it->type&&DDZ_CT_PAIR!=it->type)
		{
			lOutCard.push_back(*it);
		}
	}
	
	
	unsigned int maxret=0;
	unsigned int ret = getHandCardQuan(have,lz,lHaveHandCard,lOutCard,maxret,lGoodOutCard,mHaveZhi);
	return ret;
}

unsigned int getHandCardQuan(const unsigned char *card,int len,char lz,my_list<outCard> &lGoodOutCard,my_map<haveHandCard,unsigned int>& mHaveZhi)
{
	char have[16];
	memset(have,0,sizeof(have));
	
	for(int i=0;i<len;i++)
	{
		char index = card[i]<16?card[i]:(card[i]&0x0f);
		have[index]++;
	}	
	return getHandCardQuan(have,lz,lGoodOutCard,mHaveZhi);
}
