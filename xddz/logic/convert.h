
#ifndef _CONVERT_H_
#define _CONVERT_H_
#include "mddz.h"
#include "ai.h"
int convertWanID(int nowid,int dizhuid);
bool convertOutCard(node &n,const out_cards_buf&lastOutCards);
bool convertHandCard(node &n,const cards_buf* handCard,int dizhuid);
char convertCard(char c);   //�� ֮ǰ������������ת��ai�е�����
int convertCardType(const out_cards_buf&lastOutCards);

bool getResult(outCard &shouldOutCard,cards_buf handCard,cards_buf& out);
#endif
