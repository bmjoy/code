
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
	char opman;               //Ŀǰ���������
	char lastopman;			  //��һ�����Ƶ����
	char hand_haves[3][16];   //���������е���0�ǵ���


	outCard lastOutCard;
	static char lz;
	//�˽ڵ���Բ����ĺ��ӽڵ㣬����Ҫע�⺢�ӽڵ�������˳��
	void forksun(my_list<node> &ln)const;
	
	int hash_index() const
    {    // 65536��λ��
        unsigned int xx = XXH32(this,sizeof(node),19860823);
        unsigned short index = xx&0xffff;
        index = index&(xx>>16);
        return index;
    }

	bool operator<(const node &other) const
	{   //��ҪΪ��ȥ��
		//���ַ�����Ȼ�����һ�����ظ��ڵ㣬���ǿ��Դ����߶Ա�Ч��
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
		{   //�׷�����
			return false;
		}

		return lastOutCard.smallThan(other.lastOutCard);
	}
	
	bool lastIsWin() const
	{   ///��һ�����������Ƿ�Ӯ��
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

    ///����Χ���õĽӿ�
    int comput(const node& now,char aiman,node &sun);
	

   
	
	
	/// �жϽڵ����
	///return 0�������AI���㣬1������Ҫ�ü���2�������ʹ��
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
	///���ݺ�A*Ч���൱���ǻ��ݿ��Խ�Լ�ڴ棬��˲��û��ݵķ���
	
	/// now�ǵ�ǰ����ڵ㣬aiman��ai�����,sun��ѡ��ľ��ߣ�����level==0��Ч��
	/// return 0�������AI���㣬1�ҵ�ai��Ӯ��2ai���䣬3δ֪
	int comput(const node& now,char aiman,node &sun,int level);
protected:

    int landPolicy(const node& now,char aiman,node &sun);

protected:
     /// ���ơ�˳��ģʽ�ж��Ƿ�����б�Ӯ·��
     // ����һ�μ������ƣ�Ȼ��˳�ƣ���������˳�ƣ��Ƿ��б�Ӯ·��
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