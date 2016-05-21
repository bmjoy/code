
#ifndef _AI_H_
#define _AI_H_


#include <cstring>
using namespace std;
#include <time.h>
#include "card.h"
#include "stlmem.h"
#include "xxhash.h"

class node
{
public:
	char opman;               //目前操作的玩家
	char lastopman;			  //上一个出牌的玩家
	char hand_haves[3][16];   //三个人手中的牌0是地主


	outCard lastOutCard;
	static char lz;
	//此节点可以产生的孩子节点，并需要注意孩子节点有优先顺序
	void forksun(my_list<node> &ln)const;
	
	int hash_index() const
    {    // 65536个位置
        unsigned int xx = XXH32(this,sizeof(node),19860823);
        unsigned short index = xx&0xffff;
        index = index&(xx>>16);
        return index;
    }

	bool operator<(const node &other) const
	{   //主要为了去重
		//此种方法虽然会多算一部分重复节点，但是可以大大提高对比效率
		return memcmp(this,&other,sizeof(node))<0;
		if(opman!=other.opman)
		{
			return opman<other.opman;
		}

		int ret = memcmp(hand_haves,other.hand_haves,sizeof(hand_haves));
		if(ret!=0)
		{
			return ret<0;
		}
		
		if(lastopman!=other.lastopman)
		{
			return lastopman<other.lastopman;
		}
		
		if(lastopman==opman)
		{   //首发出牌
			return false;
		}

		return lastOutCard.smallThan(other.lastOutCard);
	}
	
	bool lastIsWin() const
	{   ///上一个操作的人是否赢了
		int num =0;
		for(int i=c3;i<=cbk;i++)
		{
			num+=hand_haves[lastopman][i];
		}
		return num==0;
	}

	node()
	{
		memset(this,0,sizeof(node));
	}
	
};

class total_time
{
public:
	total_time(unsigned long long*p);

	~total_time();

protected:
	unsigned long long begin;
	unsigned long long* ptotaltime;
};

class DDZ_AI
{
public:

	unsigned long long m_tryTime;
	unsigned long long m_forkNodeTime;
	unsigned long long m_SortNodeTime;
	unsigned long long m_InitNodeTime;
	
	DDZ_AI(int max_fork_node_num,int max_comput_num);
	~DDZ_AI();

    ///被外围调用的接口
    int comput(const node& now,char aiman,node &sun);
	

   
	
	
	/// 判断节点情况
	///return 0代表放弃AI计算，1代表需要裁减，2代表可以使用
	int tryNode(const node &n,int &ret);

	void SetRet(const node &n,int ret);
	
	void print();
	int getNumNode()
	{
		return m_NumNode;
	}
	int getSetNode()
	{
		return m_SetNumNode;;
	}
protected:
	///回溯和A*效果相当但是回溯可以节约内存，因此采用回溯的方法
	
	/// now是当前计算节点，aiman是ai的玩家,sun是选择的决策（仅在level==0有效）
	/// return 0代表放弃AI计算，1找到ai必赢，2ai必输，3未知
	int comput(const node& now,char aiman,node &sun,int level);
protected:

    int landPolicy(const node& now,char aiman,node &sun);

protected:
     /// 拿牌、顺牌模式判断是否可以有必赢路径
     // 即第一次己方拿牌，然后顺牌，在拿牌在顺牌，是否有必赢路径
     //int computNaShun(const node& now,char aiman,node &sun,int level);
protected:
	my_map<node,int> *Mnode;
	int m_maxForkNode;
	int m_maxComputNode;
	int m_NumNode;
    int m_SetNumNode;
	my_map<haveHandCard,unsigned int> mHaveZhi;
};



#endif