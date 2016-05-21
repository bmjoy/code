

#include "card.h"
#include <iostream>
#include <algorithm>

using namespace std;

void printCard(const unsigned char card)
{
	static const char cardTable[16][8]={"","3","4","5","6","7","8","9","10","J","Q","K","A","2","С��","����"};
	unsigned char c=card;
	if(c<16)
	{
		cout<<cardTable[c];
	}
	else
	{
		c = c&0x0f;
		cout<<"���"<<cardTable[c];
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

//N������ѡ��M���Ƶ������������
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


///�׷����е���
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

///�׷����ж���
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

///�׷���������
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

///��˳ 5������,size��˳�ӵĸ���
void firstAllDanShunOutCard(const char *have,my_list<outCard> &lOutCard,char lz,int size)
{
	outCard temp;
	temp.len=size;
	temp.type = DDZ_CT_STRAIGHT;
	int lznum = have[lz];	
	for(int i=c3;i<=ca-(size-1);i++)
	{
		temp.quan = i+size-1;

		unsigned char setCard[16];   //��ѡ����
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
			int mustusenum = size-num;   //����ʹ����ӵ�����
			if(l<mustusenum)
			{
				continue;
			}
			if(l==0)
			{   //��ʹ�����
				for(int j=i;j<i+size;j++)
				{
					temp.oc[j-i] = j ;
				}
				lOutCard.push_back(temp);
			}
			else
			{   
				if(mustusenum==l)
				{   /// ��Ҫ����ӵ������
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
				{   ///һЩ���Ƶı�����滻
					int needH = l-mustusenum;   //������滻����
					my_set<outCard> lXuanOutCard;
					getCN_M(setCard,num,needH,lXuanOutCard);
					for(my_set<outCard>::iterator itx = lXuanOutCard.begin();itx!=lXuanOutCard.end();itx++)
					{
						for(int j=i;j<i+size;j++)
						{
							if((have[j]==0))
							{   //���뱻����滻
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

///˫˳ 6������,size��˳���Ƶĸ���
void firstAllShuangShunOutCard(const char *have,my_list<outCard> &lOutCard,char lz,int size)
{
	outCard temp;
	temp.len=size;
	temp.type = DDZ_CT_TWO_STRAIGHT;
	int lznum = have[lz];	
	
	for(int i=c3;i<=ca-(size/2-1);i++)
	{
		temp.quan = i+(size/2-1);
		unsigned char setCard[100];   //��ѡ����
		int num = 0;
		for(int j=i;j<i+size/2;j++)
		{
			if(have[j]==1&&j!=lz)
			{   //��ѡ��һ��
				setCard[num] = j;
				num++;
			}
			else if(have[j]>=2&&j!=lz)
			{   //�����ƾ����Թ�ѡ��
				setCard[num] = j;
				num++;
				setCard[num] = j;
				num++;
			}
		}
		int canlznum = lznum;

		for(int l = 0;l<=canlznum;l++)
		{
			int mustusenum = size-num;   //����ʹ����ӵ�����
			if(l<mustusenum)
			{
				continue;
			}
			if(l==0)
			{   //��ʹ�����
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
				{   /// ��Ҫ����ӵ������
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
				{   ///һЩ���Ƶı�����滻
					int needH = l-mustusenum;   //������滻����
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

///��˳ 6������,size��˳���Ƶĸ���
void firstAllSanShunOutCard(const char *have,my_list<outCard> &lOutCard,char lz,int size)
{
	outCard temp;
	temp.len=size;
	temp.type = DDZ_CT_THREE_STRAIGHT;
	int lznum = have[lz];	

	for(int i=c3;i<=ca-(size/3-1);i++)
	{
		unsigned char setCard[100];   //��ѡ����
		int num = 0;
		temp.quan = i+(size/3-1);
		for(int j=i;j<i+size/3;j++)
		{
			if(have[j]==1&&j!=lz)
			{   //��ѡ��һ��
				setCard[num] = j;
				num++;
			}
			else if(have[j]==2&&j!=lz)
			{   //�����ƾ����Թ�ѡ��
				setCard[num] = j;
				num++;
				setCard[num] = j;
				num++;
			}
			else if(have[j]>=3&&j!=lz)
			{   //�����ƾ����Թ�ѡ��
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
			int mustusenum = size-num;   //����ʹ����ӵ�����
			if(l<mustusenum)
			{
				continue;
			}
			if(l==0)
			{   //��ʹ�����
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
				{   /// ��Ҫ����ӵ������
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
				{   ///һЩ���Ƶı�����滻
					int needH = l-mustusenum;   //������滻����
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


///�׷� ����һ
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
		{   ///ȥ��������
			if(it->oc[i]<16)
			{
				temp.quan = it->oc[i];  //Ȩֵ
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
				continue;  //���ܵ����Լ�
			if(temphave[i]>0)
			{
				temp.oc[3] = i;
				lOutCard.push_back(temp);
			}
		}
	}
}


///�׷� ������
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
		{   ///ȥ��������
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
				continue;  //���ܵ����Լ�
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


///�ɻ��������
void firstAllFeiDanOutCard(const char *have,my_list<outCard> &lOutCard,char lz,int size)
{
	char temphave[16];
	int sansunsize = (size/4)*3;
	my_list<outCard> templOutCard;
	///������˳����ɻ�����򣬼����������˳�����
	firstAllSanShunOutCard(have,templOutCard,lz,sansunsize);
	outCard temp;
	temp.len=size;	
	temp.type = DDZ_CT_THREE_STRAIGHT_WITH_N;
	for(my_list<outCard>::iterator it = templOutCard.begin();it!=templOutCard.end();it++)
	{//�������е���˳���
		memcpy(temphave,have,sizeof(temphave));
		temp.quan = it->quan+(size/4-1);
		for(int i=0;i<it->len;i++)
		{   ///ȥ����˳����
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
		unsigned char setCard[16];   //��ѡ����
		int num = 0;
		for(int i=c3;i<=cbk;i++)
		{
			if(temphave[i]>0)
			{   //��ѡ����
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

///�ɻ����Գ��
void firstAllFeiDuiOutCard(const char *have,my_list<outCard> &lOutCard,char lz,int size)
{   
	char temphave[16];
	int sansunsize = (size/5)*3;
	my_list<outCard> templOutCard;
	///������˳����ɻ�����򣬼����������˳�����
	firstAllSanShunOutCard(have,templOutCard,lz,sansunsize);
	outCard temp;
	temp.len=size;	
	temp.type = DDZ_CT_THREE_STRAIGHT_WITH_NP;	

	for(my_list<outCard>::iterator it = templOutCard.begin();it!=templOutCard.end();it++)
	{//�������е���˳���
		memcpy(temphave,have,sizeof(temphave));
		temp.quan = it->quan+(size/5-1);
		for(int i=0;i<it->len;i++)
		{   ///ȥ����˳����
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
		///��ѡ���Ƽ���
		unsigned char setCard[16];   //��ѡ����
		int num = 0;
		for(int i=c3;i<=c2;i++)
		{
			if(temphave[i]>0&&(i<(it->quan)||i>(it->quan+needduinum-1)))
			{   //��ѡ����
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
			unsigned char setDanCard[16];   //��ѡ����
			int dannum = 0;
			for(int k=0;k<itx->len;k++)
			{
				if(itx->oc[k]==lz)
				{   /// ��
					temphuanlz = temphuanlz-2;
					continue;
				}
				if(temphave[itx->oc[k]]>=2)
				{   ///����ȿ���һ�Դ���Ҳ���Ե��Ƽ����
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
			
			{   /// ��Щ�������л������
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

///�׷�����ը��
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

	//ȡ������ը��
	firstAllBombOutCard(have,lFourOutCard,lz);
	
	for(my_list<outCard>::iterator it = lFourOutCard.begin();it!=lFourOutCard.end();it++)
	{
		memcpy(temphave,have,sizeof(temphave));
		temp.quan = it->quan;
		for(int i=0;i<it->len;i++)
		{   ///ȥ����˳����
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
		unsigned char setCard[16];   //��ѡ����
		int num = 0;
		for(int i=c3;i<=cbk;i++)
		{
			if(temphave[i]>0)
			{   //��ѡ����
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

//���Ŵ�����
void firstAllFourDuiOutCard(const char *have,my_list<outCard> &lOutCard,char lz)
{
	outCard temp;
	temp.len=8;
	temp.type = DDZ_CT_FOUR_WITH_TWOPAIR;

	my_list<outCard> lFourOutCard;
	char temphave[16];

	//ȡ������ը��
	firstAllBombOutCard(have,lFourOutCard,lz);

	for(my_list<outCard>::iterator it = lFourOutCard.begin();it!=lFourOutCard.end();it++)
	{
		memcpy(temphave,have,sizeof(temphave));
		temp.quan = it->quan;
		for(int i=0;i<it->len;i++)
		{   ///ȥ�����ŵ���
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
		unsigned char setCard[16];   //��ѡ����
		int num = 0;
		for(int i=c3;i<=c2;i++)
		{
			if(temphave[i]>0)
			{   //��ѡ����
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
	/// �ɻ��������
	for(int size = 8;size<=len;size+=4)
	{
		firstAllFeiDanOutCard(have,lOutCard,lz,size);
	}
	/// �ɻ����Գ��
	for(int size = 10;size<=len;size+=5)
	{
		firstAllFeiDuiOutCard(have,lOutCard,lz,size);
	}
	/// ˫˳
	for(int size = 6;size<=len;size+=2)
	{
		firstAllShuangShunOutCard(have,lOutCard,lz,size);
	}
	/// ��˳
	for(int size = 5;size<=len;size++)
	{
		firstAllDanShunOutCard(have,lOutCard,lz,size);
	}

	/// ��˳
	for(int size = 6;size<=len;size+=3)
	{
		firstAllSanShunOutCard(have,lOutCard,lz,size);
	}
	/// ����һ
	if(len>=4)
	{
		firstAllSanOneOutCard(have,lOutCard,lz);
	}
	/// ������
	if(len>=5)
	{
		firstAllSanDuiOutCard(have,lOutCard,lz);
	}


	/// ����
	firstAllDuiOutCard(have,lOutCard,lz);
	/// ����
	firstAllSanOutCard(have,lOutCard,lz);




	/// ����
	firstAllDanOutCard(have,lOutCard,lz);
	/// ���Ŵ�����
	firstAllFourDanOutCard(have,lOutCard,lz);
	
	/// ���Ŵ�����
	firstAllFourDuiOutCard(have,lOutCard,lz);
	///ը��
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
	/// ������ը��
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
	/// ������ը��
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
	/// ������ը��
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
//��˳��
void getBigDanShunOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// ������ը��
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
//˫˳��
void getBigShuangShunOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// ������ը��
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
//��˳��
void getBigSanShunOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// ������ը��
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

//����һ
void getBigSanOneOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// ������ը��
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

//������
void getBigSanDuiOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// ������ը��
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

//�ɻ��������
void getBigFeiDanOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// ������ը��
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

//�ɻ����Գ��
void getBigFeiDuiOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// ������ը��
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

//���Ŵ�����
void getBigFourDanOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// ������ը��
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

//���Ŵ�����
void getBigFourDuiOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	/// ������ը��
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

//ը������
void getBigBombOutCard(const char *have,my_list<outCard> &lOutCard,const outCard &lastOutCard,char lz) 
{
	my_list<outCard> lTempOutCard;
	firstAllBombOutCard(have,lTempOutCard,lz);
	for(my_list<outCard>::iterator it = lTempOutCard.begin();it!=lTempOutCard.end();it++)
	{
		if(it->type>lastOutCard.type)
		{   //ը�����ʹ�
			lOutCard.push_back(*it);
		}		
		if(it->type==lastOutCard.type&&it->quan>lastOutCard.quan)
		{   //ը��������ȣ����ʹ�
			lOutCard.push_back(*it);
		}
	}
	/// ��������ը�����ǻ��,���ϻ��
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
		{   //��˳��
			getBigDanShunOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}			
		case DDZ_CT_TWO_STRAIGHT:
		{   //��˫˳��
			getBigShuangShunOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}	
		case DDZ_CT_THREE_STRAIGHT:
		{   //����˳��
			getBigSanShunOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}	
		case DDZ_CT_THREE_WITH_ONE:
		{   //������һ
			getBigSanOneOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}
		case DDZ_CT_THREE_WITH_PAIR:
		{   //��������
			getBigSanDuiOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}	
		case DDZ_CT_THREE_STRAIGHT_WITH_N:
		{   //���ɻ������
		
			getBigFeiDanOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}			
		case DDZ_CT_THREE_STRAIGHT_WITH_NP:
		{   //���ɻ����Գ��
		
			getBigFeiDuiOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}	
		case DDZ_CT_FOUR_WITH_TWO:
		{   //�����Ŵ�����
			
			getBigFourDanOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}	
		case DDZ_CT_FOUR_WITH_TWOPAIR:
		{   //�����Ŵ�����
			
			getBigFourDuiOutCard(have,lOutCard,lastOutCard,lz);
			break;
		}	
		case DDZ_CT_HUN_BOMB:
		case DDZ_CT_CUN_BOMB:
		case DDZ_CT_LZ_BOMB:
		case DDZ_CT_ROCKET:
		{   //ը�� ����
			
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
///���������Ƶķ������
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
				return 0;   //����������� �������һ�����ǱȽϺõ����Ʒ�ʽ
		}
		if(have[csk]>0&&have[cbk]>0)
		{   //�����ǻ��
			return 0;
		}
		
		int handNum = len+lHaveHandCard.size();
		int bombNum = 0;
		int sdui = 0;
		int sdan = 0;
		for(my_list<outCard>::iterator it = lHaveHandCard.begin();it!=lHaveHandCard.end();it++)
		{
			if(it->type>DDZ_CT_FOUR_WITH_TWOPAIR)
			{   //ը������
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
		
		/// �޸ķ���ֵ����,Ϊhave�еķ����������������Ƶķ���
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
		{   //ը������
			bombNum++;
		}
	}
	
	unsigned int ret2 = 0;
	if(can)
	{
		
		///��������
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
	///����������
	
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
		//ȥ���������
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
