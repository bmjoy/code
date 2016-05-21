#ifndef __DDZ_AI_ANALIZER_H__
#define __DDZ_AI_ANALIZER_H__
#include "mddz.h"

struct card_split_item
{
    char cards[20];
    int  size;
    int  type;
    int  maxv;
};

struct card_split_result
{
    card_split_item item[20];
    int size;
};
int split_rocket(char* c, int len, card_split_result* result);
int split_bomb(char* c, int len, card_split_result* result);
int split_three(char* c, int len, card_split_result* result);
int split_straight(char* c, int len, card_split_result* result);
int split_three_with_one(char* c, int len, card_split_result* result);
int split_two_straight(char* c, int len, card_split_result* result);
int split_three_with_pair(char* c, int len, card_split_result* result);
int split_plane(char* c, int len, card_split_result* result);
int split_pair(char* c, int len, card_split_result* result);
void split_one(char* c, int len, card_split_result* result);


void split_analize(char* buf, int len, card_split_result* result);

#endif//__DDZ_AI_ANALIZER_H__