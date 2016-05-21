#ifndef __MDDZ_C_H__
#define __MDDZ_C_H__
#include "mddz.h"


extern void sort_by_num2(char* cards, int len);
extern void sort_cards2(char* cards, int len, int lz=0);
extern bool ddz_compare2(cards_buf& c1, cards_buf& c2, int lz);//buf��cards,size��Ч����������
extern int  ddz_get_cards_type_list2(cards_buf& src, int lz, cards_buf buf[MAX_CANDIDATE]);
extern int  ddz_get_bigger_compose2(cards_buf& src, cards_buf& candidate, int lz, cards_buf out[MAX_CANDIDATE]);
extern bool ddz_has_straight2(cards_buf& src, cards_buf& out, int lz);


#endif//__MDDZ_C_H__