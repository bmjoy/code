#ifndef __DDZ_LOGIC_UTIL_H__
#define __DDZ_LOGIC_UTIL_H__
#include "mddz.h"


extern void zerom(char* s, int len);
extern int  calculate_cards_stat(char* buf, int ncard, card_stat stat_buf[16]);
extern void change_12_2_A2(char* buf, int len);
extern void change_A2_2_12(char* buf, int len);
extern char change_12_2_A2(char c);
extern char change_A2_2_12(char c);
extern void change_A2_2_12(ddz_lz_change_item* lz, int size);
extern void sort_by_num(char* cards, int len);
extern int out_card(char* handcards, int len, char* outcards, int ol);
extern int get_remain(char* src, int slen, char* out, int olen, char* remain);
extern void shuffle(char* c1, char* c2, char* c3, char* b, bool* robot);


#endif//__DDZ_LOGIC_UTIL_H__
