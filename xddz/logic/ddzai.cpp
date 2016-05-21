#include "ddzai.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ddzai_analizer.h"
#include "ddz_ai_strategies.h"
#include "mddz_util.h"
#include "ai.h"
#include "convert.h"

#ifdef _WIN32
#include <Windows.h>
//#include "cocos2d.h"
#endif


ddzai::ddzai()
{
    m_maxProcessNodeNum = 200000;
}

ddzai::~ddzai()
{

}

void ddzai::clear()
{
    memset(m_hand_card, 0, sizeof(m_hand_card));
    memset(&m_back_card, 0, sizeof(m_back_card));
    memset(&m_out_card, 0, sizeof(m_out_card));
    memset(m_bomb, 0, sizeof(m_bomb));
    m_landload = -1;
    m_user_win = false;
}

int ddzai::get_win_id()
{
    for(int i=0; i<3; i++)
    {
        if(m_hand_card[i].size == 0)
            return i;
    }
    return -1;
}

int ddzai::set_out_cards(int id, char* buf, int len)
{
    cards_buf src(buf, len);
    cards_buf out[MAX_CANDIDATE];
    int n = ddz_get_cards_type_list(src, 0, out);
    if(n == 0)
    {
        out_cards_buf outdata;
        memset(&outdata, 0, sizeof(outdata));
        outdata.id = id;
        m_out_card.add(outdata);
        return -2;
    }
    //记录出牌
    out_cards_buf outdata;
    outdata.id = id;
    memcpy(outdata.cards, out[0].cards, len);
    outdata.size = len;
    outdata.card_type = out[0].card_type;
    m_out_card.add(outdata);

    //删除手牌
    m_hand_card[id].size = out_card(m_hand_card[id].cards, m_hand_card[id].size, buf, len);

    //炸弹计数
    if(out[0].card_type == MDDZ_CT_BOMB || out[0].card_type == MDDZ_CT_ROCKET)
        m_bomb[id] ++;
    //返回出牌牌型
    return out[0].card_type;
}

int  ddzai::set_new_out_cards(int id, char* buf, int len)
{
    if(len<0||len>20)
        return -1;
    char tempbuf[20];
    memcpy(tempbuf,buf,len);
    change_12_2_A2(tempbuf,len);

    //sort_by_num(tempbuf,len);
    return ddzai::set_out_cards(id,tempbuf,len);
}

unsigned int ddzai::handCardEstimate(char *card,int len)
{
    if(len<0||len>20)
        return -1;
    char tempbuf[20];
    memcpy(tempbuf,card,len);
    change_12_2_A2(tempbuf,len);
    for(int i=0;i<len;i++)
    {
        tempbuf[i]= convertCard(tempbuf[i]);
    }
    my_list<outCard> lGoodOutCard;
    my_map<haveHandCard,unsigned int> mHaveZhi;
    return getHandCardQuan((const unsigned char *)tempbuf,len,0,lGoodOutCard,mHaveZhi);
}

int ddzai::get_next_operator(int& new_turns)
{
    return m_out_card.get_next_operator(new_turns);
}

int ddzai::get_remain(int id, char* buf)
{
    memcpy(buf, m_hand_card[id].cards, m_hand_card[id].size);
    return m_hand_card[id].size;
}

int ddzai::is_game_end()
{
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(m_hand_card[i].size == 0)
            return i;
    }
    return -1;
}

void ddzai::init(bool win)
{
    bool robot[3] = {false, true, true};
    ::shuffle(m_hand_card[0].cards, m_hand_card[1].cards, m_hand_card[2].cards, m_back_card.cards, robot);
    for(int i=0; i<3; i++)m_hand_card[i].size = 17;
    for(int i=0; i<3; i++)
    {
        sort_by_num(m_hand_card[i].cards, m_hand_card[i].size);
    }
    m_back_card.size = 3;
    m_begin_call = 0;
}



void ddzai::setHandBackCard(char hand[][20],char back[3])
{
    m_out_card.clear();
    for(int i=0;i<3;i++)
    {
        memcpy(m_hand_card[i].cards,hand[i],17);
        m_hand_card[i].size = 17;
        change_12_2_A2(m_hand_card[i].cards,17);
        sort_by_num(m_hand_card[i].cards,17);
    }
    memcpy(m_back_card.cards,back,3);
    change_12_2_A2(m_back_card.cards,3);
   
}

void ddzai::setHandCard(char hand[][20],char hl[3])
{
    m_out_card.clear();
    for(int i=0;i<3;i++)
    {
        memcpy(m_hand_card[i].cards,hand[i],hl[i]);
        m_hand_card[i].size = hl[i];
        change_12_2_A2(m_hand_card[i].cards,hl[i]);
        sort_by_num(m_hand_card[i].cards,hl[i]);
    }
}

void ddzai::get_last_out(out_cards_buf& d)
{
    m_out_card.get_last(d);
}

int ddzai::get_first_call()
{
    m_begin_call = rand()%3;
    return m_begin_call;
}

void ddzai::set_land_load(int id)
{
    m_landload = id;
    memcpy(&m_hand_card[id].cards[17], m_back_card.cards, 3);
    m_hand_card[id].size = 20;
    sort_by_num(m_hand_card[id].cards, 20);
    m_out_card.set_first(id);
}

void ddzai::set_land_load_dis(int index)
{
    m_landload = index;
    m_out_card.set_first(index);
}

void ddzai::SetMaxProNodeNum(int n)
{
    m_maxProcessNodeNum = n;
}

int ddzai::set_call(int id, int call)
{
    m_call[id] = call;
    id ++;
    if(id>2)id=0;
    if(call==3) return -1;
    if(id == m_begin_call) return -1;
    return id;
}

int ddzai::get_max_call()
{
    int c = 0;
    for(int i=0; i<3; i++)
    {
        if(m_call[i] > c) c = m_call[i];
    }
    return c;
}

void ddzai::get_land(int& id, int& point)
{
    point = 0;
    id = -1;
    for(int i=0; i<3; i++)
    {
        if(m_call[i]>point){ point = m_call[i]; id = i;}
    }
    if(point == 0)
    {
        point = 1;
        id = 0;
    }
}

int ddzai::get_beishu()
{
    int id, pp;
    get_land(id, pp);
    int nbomb = 0;
    for(int i=0; i<3; i++)nbomb += m_bomb[i];
    for(int i=0; i<nbomb; i++)pp*=2;
    return pp;
}

int ddzai::caculate_land()
{
    int ret = -1;
    int max = 0;
    for(int i=0; i<3; i++)
    {
        if(m_call[i] == 3) return i;
        if(m_call[i]>max) {max = m_call[i]; ret = i;}
    }
    return ret;
}

int ddzai::get_rest_cards(int id)
{
    return m_hand_card[id].size;
}

//////////////////////////////////////////////////////////////////////////

int ddzai::get_robot_call(int id)
{
    int value = ddzai_evaluate_call_point(m_hand_card, id, m_back_card.cards);

    int call = 0;
    if(value >= 7) call = 3;
    else if(value >= 5) call = 2;
    else if(value >= 3) call = 1;
    else call = 0;

    int cc = 0;
    for(int i=0; i<3; i++)
    {
        if(m_call[i] > cc) cc = m_call[i];
    }
    if(call > cc) return call;
    return 0;
}

//////////////////////////////////////////////////////////////////////////
int ddzai::get_robot_out(int seatid, cards_buf& out, bool is_robot[3])
{
	int ret = get_win_robot_out(seatid,out,is_robot);
	if(0==ret)
	{

		return out.size;
	}
    return ddzai_get_robot_out(seatid, 0, m_hand_card, m_out_card, out, is_robot);
}

int ddzai::get_new_robot_out(int seatid, cards_buf& out, bool is_robot[3])
{
    int ret = get_robot_out(seatid,out,is_robot);
    if(out.size > 0)
    {
        change_A2_2_12(out.cards, out.size);
    }
    return ret;
}


int  ddzai::get_win_robot_out(int seatid, cards_buf& out, bool is_robot[3])
{
	node root;

	out_cards_buf lastOutCards;
	get_last_out(lastOutCards);

	root.lastopman = convertWanID(lastOutCards.id,m_landload);    //上次出牌玩家
	
	root.opman = convertWanID(seatid,m_landload);                 //当前操作玩

	if(lastOutCards.id<0)
	{
		root.lastopman = root.opman;
	}

	if(root.lastopman != root.opman&&!convertOutCard(root,lastOutCards))
	{   //出牌转换失败
		return 1;
	}
	if(!convertHandCard(root,m_hand_card,m_landload))
	{   //手牌转换失败
		return 2;
	}
	//for(int i=0;i<3;i++)
	//{
	//	for(int j=1;j<16;j++)
	//	{
	//		for(int k=0;k<root.hand_haves[i][j];k++)
	//		{
	//			printCard(j);
	//			printf(" ");
	//		}

	//	}
	//	printf("\n");
	//}
	node sun;
	DDZ_AI logic_ai(1000000,m_maxProcessNodeNum);
	char aiman = convertWanID(seatid,m_landload)==0?0:1;

    //unsigned int t1 = GetTickCount();
	int ret =0;
	unsigned long long totaltime = 0;
	{
		total_time temp(&totaltime);
		ret= logic_ai.comput(root,aiman,sun);
	}
    //t1 = GetTickCount() - t1;
	int num = logic_ai.getSetNode();

	char szlog[1000];
	sprintf(szlog, "ret=%d num=%d\n",ret,num);
	OutputDebugStringA(szlog);

	sprintf(szlog, "totaltime=%I64d trytime = %I64d forkNodeTime = %I64d SortNodeTime = %I64d\n",totaltime,logic_ai.m_tryTime,logic_ai.m_forkNodeTime,logic_ai.m_SortNodeTime);
	OutputDebugStringA(szlog);

	sprintf(szlog, "SortNodeTime = %I64d InitNodeTime = %I64d\n",logic_ai.m_SortNodeTime,logic_ai.m_InitNodeTime);
	OutputDebugStringA(szlog);
    //printf("-------------ret=%d------------t:%d\n", ret, t1);
	if(ret==1)
	{
		///将结果放入
		if(sun.lastopman == root.lastopman&&root.lastopman!=root.opman)
		{
			//跟牌选择不出
			return 0;
		}
		getResult(sun.lastOutCard,m_hand_card[seatid],out);
		return 0;
	}
	if(ret==2)
	{
		
		int num = logic_ai.getNumNode();
		int setnum = logic_ai.getSetNode();
		return 3;
	}
	return 5;
}