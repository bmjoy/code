
#include "ai.h"
#include "card.h"

#include <iostream>
#include <algorithm>
using namespace std;
#ifdef _WIN32
#include <windows.h>
#else

#endif

total_time::total_time(unsigned long long*p)
	{
		ptotaltime = p;
#ifdef _WIN32
		LARGE_INTEGER liPerfNow={0};
		// ����CPU���е����ڵ�ʱ��
		QueryPerformanceCounter(&liPerfNow);
		begin = liPerfNow.QuadPart;
#else

		begin = clock();
#endif
	}
total_time::~total_time()
	{
#ifdef _WIN32
		LARGE_INTEGER liPerfNow={0};
		// ����CPU���е����ڵ�ʱ��
		QueryPerformanceCounter(&liPerfNow);
		unsigned long long end = liPerfNow.QuadPart;
#else

		unsigned long long end = clock();
#endif
		*ptotaltime += (end-begin);
	}

char node::lz=0;

bool cmpOutCard(node &a,node &b)
{
	return a.lastOutCard.len>b.lastOutCard.len;
}

//�˽ڵ���Բ����ĺ��ӽڵ㣬����Ҫע�⺢�ӽڵ�������˳��
void node::forksun(my_list<node> &ln)const
{
	node tempNode;
	tempNode.opman = (opman+1)%3;   //��һ����������

	if(lastopman==opman)
	{   /// �׷�����
		my_list<outCard> lOutCard;
		firstAllOutCard(hand_haves[opman],lOutCard);
		tempNode.lastopman = opman;   //�������
		for(my_list<outCard>::iterator it = lOutCard.begin();it!=lOutCard.end();it++)
		{   //
			tempNode.lastOutCard = *it;
			memcpy(tempNode.hand_haves,hand_haves,sizeof(hand_haves));
			for(int i=0;i<it->len;i++)
			{
				char c = it->oc[i];
				if(c>16)
				{
					c = lz;
				}
				tempNode.hand_haves[opman][c]--;
			}
			ln.push_back(tempNode);
		}
	}
	else
	{   /// ����
		my_list<outCard> lOutCard;
		genAllOutCard(hand_haves[opman],lOutCard,lastOutCard);
		tempNode.lastopman = opman;  //������
		for(my_list<outCard>::iterator it = lOutCard.begin();it!=lOutCard.end();it++)
		{   //
			tempNode.lastOutCard = *it;
			memcpy(tempNode.hand_haves,hand_haves,sizeof(hand_haves));
			for(int i=0;i<it->len;i++)
			{
				char c = it->oc[i];
				if(c>16)
				{
					c = lz;
				}
				tempNode.hand_haves[opman][c]--;
			}
			ln.push_back(tempNode);
		}
		///���Ʋ��������
		tempNode.lastopman = lastopman;
		memcpy(tempNode.hand_haves,hand_haves,sizeof(hand_haves));
		tempNode.lastOutCard = lastOutCard;
		ln.push_back(tempNode);
	}

	return ;

}

DDZ_AI::DDZ_AI(int max_fork_node_num,int max_comput_num)
{
	m_maxForkNode = max_fork_node_num;
	m_maxComputNode = max_comput_num;
	m_NumNode = 0;

	m_tryTime = 0;
	m_forkNodeTime = 0;
	m_SortNodeTime = 0;
	m_InitNodeTime = 0;
    m_SetNumNode = 0;
    Mnode = new my_map<node,int>[65536];
}

DDZ_AI::~DDZ_AI()
{
    if(Mnode!=NULL)
        delete [] Mnode;
}
class SortNode
{
public:
	node *pdata;
	unsigned int zhi;
	char man;    //�Ƚ��ߣ�����������pai



	bool operator<(const SortNode& other) const
	{   //���մӴ�С����
		//if(zhi!=other.zhi)
			return zhi>other.zhi;
		//return you>other.you;

	}
    void initLandPolicy(node *p,char m,my_map<haveHandCard,unsigned int> &mHaveZhi,int index)
    {
		pdata = p;
		man = m;
		my_list<outCard> lGoodOutCard;

		
		zhi = getHandCardQuan(p->hand_haves[m],0,lGoodOutCard,mHaveZhi);
        if(p->lastopman != m)
        { //���� ��������
            zhi -= 2;
        }
        zhi <<= 8;
        zhi |= (255-(index&0xff));
        //zhi>>=8;// ������������
    }
	void init(node *p,char m,my_map<haveHandCard,unsigned int> &mHaveZhi)
	{
		pdata = p;
		man = m;
        ////�򵥷���
        {
            zhi = 0;
            if(m==p->lastopman)
            {
                zhi = p->lastOutCard.len;
                if(p->lastOutCard.type>=DDZ_CT_HUN_BOMB)
                {
                    zhi+=256;
                }
            }
        }


        ///������Լ������ϴ���ʱ����
 

		//my_list<outCard> lGoodOutCard;
		//
		//zhi = getHandCardQuan(p->hand_haves[m],0,lGoodOutCard,mHaveZhi);
		//int len = 0;
		//for(int i=c3;i<=cbk;i++)
		//{
		//	len+=p->hand_haves[m][i];
		//}
		//zhi = (zhi<<8)+255-len;
		/////�Ի�Ҫ������ƣ���ǰ��

		//you = 0;

		//if(m==0)
		//{
		//	if(m==p->lastopman)
		//	{   ///������
		//		my_list<outCard> lOutCard;
		//		genAllOutCard(p->hand_haves[1],lOutCard,p->lastOutCard);
		//		if(!lOutCard.empty())
		//			return;
		//		genAllOutCard(p->hand_haves[2],lOutCard,p->lastOutCard);
		//		if(lOutCard.empty())
		//		{
		//			you = 1;
		//		}
		//	}
		//}
		//else
		//{
		//	if(m==p->lastopman)
		//	{   ///������
		//		my_list<outCard> lOutCard;
		//		genAllOutCard(p->hand_haves[0],lOutCard,p->lastOutCard);
		//		if(lOutCard.empty())
		//		{
		//			you = 1;
		//		}
		//	}
		//}

	}
};
int DDZ_AI::comput(const node &now,char aiman,node &sun,int level)
{
	if(now.lastIsWin())
	{
		if (now.lastopman==aiman||(now.lastopman!=0&&aiman!=0))
			return 1;
		else 
			return 2;
		return 3;
	}
	my_list<node> ln;
	{
		total_time temp(&m_forkNodeTime);
		now.forksun(ln);
	}
	my_vector<SortNode> vsNode;
	SortNode tempSortNode;
	{
		
        vsNode.resize(ln.size());
        total_time temp(&m_SortNodeTime);
        int index = 0;
		for(my_list<node>::iterator it = ln.begin();it!=ln.end();it++)
		{
			{
			total_time temp(&m_InitNodeTime);
            vsNode[index].init(&(*it),now.opman,mHaveZhi);
			//tempSortNode.init(&(*it),now.opman,mHaveZhi);
			}
            vsNode[index].pdata =  &(*it);
            index++;
			//tempSortNode.pdata = &(*it);
			//vsNode.push_back(tempSortNode);
		}
		sort(vsNode.begin(),vsNode.end());
	}
	if (now.opman==aiman||(now.opman!=0&&aiman!=0))
	{
		bool canying = false;

		for(my_vector<SortNode>::iterator it = vsNode.begin();it!=vsNode.end();it++)
		{
			static unsigned int zhi=0;
			if(level==0)
			{
			    zhi= it->zhi;
			}
			int tryRet=0;
			int ret = tryNode(*(it->pdata),tryRet);
			if(ret==0)
			{
				return 0;
			}
			else if(ret==1)
			{
				//continue;
				ret = tryRet;
			}
			else
			{
				ret = comput(*(it->pdata),aiman,sun,level+1);
				SetRet(*(it->pdata),ret);
			}

			if(ret==0)
			{
				return 0;
			}
			else if(ret==1)
			{   //��Ӯ ֱ�ӷ��ؽ��
				if(level==0)
				{
					sun = *(it->pdata);
				}
				
				return 1;
			}
			else if(ret==2)
			{	//����	
			
			}
			else
			{
				canying = true;
			}
		}
		if(ln.empty())
		{
			return 3;
		}

		
		if(canying)
		{
			
			return 3;
		}
		else
		{
			
			return 2;
		}
	}
	else
	{
		bool canshu = false;
		for(my_vector<SortNode>::iterator it = vsNode.begin();it!=vsNode.end();it++)
		{
			int tryRet=0;
			int ret = tryNode(*(it->pdata),tryRet);
			if(ret==0)
			{
				return 0;
			}
			else if(ret==1)
			{
				//continue;
				ret = tryRet;
			}
			else
			{
				ret = comput(*(it->pdata),aiman,sun,level+1);
				SetRet(*(it->pdata),ret);
			}

			if(ret==0)
			{
				return 0;
			}
			else if(ret==1)
			{   //��Ӯ 
				
			
			}
			else if(ret==2)
			{	//����	
				

				return 2;
			}
			else
			{
				canshu = true;
			}
		}
		if(ln.empty())
		{
			return 3;
		}

		if(canshu)
		{
			
			return 3;
		}
		else
		{
			if(level==0)
			{
				sun = *(vsNode.front().pdata);
			}			
			return 1;
		}		
	}
	return 3;   //�����������bug
}


int DDZ_AI::comput(const node& now,char aiman,node &sun)
{
    /// ����Ѱ·�㷨�����׼ȷ���
    int ret = comput(now,aiman,sun,0);
    if(1==ret)
    {    //�ҵ���Ӯ����
        return ret;
    }
    else if(2 == ret)
    {   //�Ѿ�֪������
        if(aiman != 0)
        {   //ʹ�����ܲ���
            return 2;
        }
        ///ʹ�õ�������
        landPolicy(now,aiman,sun);
        return 1;
    }
    
    if(aiman == 0)
    {
        ///ʹ�õ�������
        landPolicy(now,aiman,sun);
        return 1;
    }

    //

    return 0; 

}

int DDZ_AI::landPolicy(const node& now,char aiman,node &sun)
{
    my_list<node> ln;
	{
		total_time temp(&m_forkNodeTime);
		now.forksun(ln);
	}
	my_vector<SortNode> vsNode;
	SortNode tempSortNode;
    my_map<haveHandCard,unsigned int> mlandHaveZhi;
	{
		
        vsNode.resize(ln.size());
        total_time temp(&m_SortNodeTime);
        int index = 0;
		for(my_list<node>::iterator it = ln.begin();it!=ln.end();it++)
		{
			{
			total_time temp(&m_InitNodeTime);
            vsNode[index].initLandPolicy(&(*it),now.opman,mlandHaveZhi,index);

			}
            vsNode[index].pdata =  &(*it);
            index++;

		}
		sort(vsNode.begin(),vsNode.end());
	}

    sun = *(vsNode.begin()->pdata);
    return 0;
}

void DDZ_AI::SetRet(const node &n,int ret)
{
    total_time temp(&m_tryTime);
    int index = n.hash_index();
	Mnode[index][n] = ret;
    m_SetNumNode++;
}

int DDZ_AI::tryNode(const node &n,int &ret)
{   
	total_time temp(&m_tryTime);
    int index = n.hash_index();
	m_NumNode++;
	if(m_NumNode>=m_maxForkNode)
		return 0;
	if(m_SetNumNode>=m_maxComputNode)
		return 0;
	my_map<node,int>::iterator it = Mnode[index].find(n);
	if(it==Mnode[index].end())
	{
		Mnode[index][n] = 0;
        m_SetNumNode++;
		return 2;
	}
	ret = it->second;
	return 1;
}

void DDZ_AI::print()
{
	//cout<<"fork node nums = "<<m_NumNode<<" compute node nums = "<<Mnode.size()<<endl;
}
