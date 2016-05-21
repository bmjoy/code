
#include "convert.h"

int convertWanID(int nowid,int dizhuid)
{
	if(nowid>=dizhuid)
	{
		return nowid-dizhuid;
	}
	return nowid+3-dizhuid;
}

bool convertOutCard(node &n,const out_cards_buf&lastOutCards)
{
	n.lastOutCard.type = convertCardType(lastOutCards);
	n.lastOutCard.len = lastOutCards.size;
	for(int i=0;i<n.lastOutCard.len;i++)
	{
		n.lastOutCard.oc[i] = convertCard(lastOutCards.cards[i]);
	}
	//权值计算
	char maxcard =convertCard(lastOutCards.cards[0]); //convertCard(lastOutCards.max_card);
	n.lastOutCard.quan = maxcard;
	return true;
}

bool convertHandCard(node &n,const cards_buf* handCard,int dizhuid)
{
	for(int i=0;i<3;i++)
	{
		int len = handCard[i].size;
		int aiid= convertWanID(i,dizhuid);
		for(int j=0;j<len;j++)
		{
			///将相应牌的个数加1
			(n.hand_haves[aiid][convertCard(handCard[i].cards[j])])++;
		}
	}
	return true;
}


char convertCard(char c)
{
#ifndef CARD_A_START
    return c&0x0f;
#endif
	const static char convertCardTable[] = {0,12,13,1,2,3,4,5,6,7,8,9,10,11,14,15};
	char cardZhi = c&0x0f;
	return convertCardTable[cardZhi];
}

int convertCardType(const out_cards_buf&lastOutCards)
{
	int oldType = lastOutCards.card_type;
	if(oldType<=MDDZ_CT_FOUR_WITH_TWOPAIR)
	{
		return oldType;
	}
	if(oldType==MDDZ_CT_ROCKET)
	{
		return DDZ_CT_ROCKET;
	}
	if(oldType==MDDZ_CT_BOMB)
	{
		return DDZ_CT_CUN_BOMB;   
	}

	return DDZ_CT_ERROR;
}

char findCard(cards_buf &handCard,char quan,bool del)
{
	for(int i=0;i<handCard.size;i++)
	{
		if(convertCard(handCard.cards[i])==quan)
		{
			///删除此牌
			char c = handCard.cards[i];
			if(del)
			{
				for(int j=i;j<(handCard.size-1);j++)
				{
					handCard.cards[j] =handCard.cards[j+1];
				}
				handCard.size--;
			}
			return c;
		}
	}
	return 0;
}

bool getResult(outCard &shouldOutCard,cards_buf handCard,cards_buf& out)
{
	int oldType = shouldOutCard.type;
	out.card_type = DDZ_CT_ERROR;
	if(oldType<=DDZ_CT_FOUR_WITH_TWOPAIR)
	{
		out.card_type = oldType;
	}
	else if(oldType==DDZ_CT_ROCKET)
	{
		out.card_type = MDDZ_CT_ROCKET;
	}	
	else
	{
		out.card_type = DDZ_CT_CUN_BOMB;
	}

	out.max_card = findCard(handCard,shouldOutCard.quan,false);
	out.size = shouldOutCard.len;
	for(int i=0;i<out.size;i++)
	{
		out.cards[i] = findCard(handCard,shouldOutCard.oc[i],true);
	}
	return true;
}
