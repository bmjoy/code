#include "ddz_ai_strategies.h"
#include "ddzai_analizer.h"
#include <string.h>
#include "mddz_util.h"

int evalute_king(char c[17])
{
    if(c[0] == 0x4f && c[1] == 0x4e)
        return 8;
    if(c[0] == 0x4f)
        return 4;
    if(c[0] == 0x4e)
        return 3;
    return 0;
}

int evalute_2(char c[17])
{
    int ret = 0;
    for(int i=0; i<17; i++)
    {
        if(C_N(c[i]) == 2)
        {
            ret += 2;
        }
    }
    return ret;
}

int evalute_bomb(char c[17])
{
    card_stat buf[20];
    int n = calculate_cards_stat(c, 17, buf);
    int ret = 0;
    for(int i=0; i<n; i++)
    {
        if(buf[i].n == 4)
            ret += 6;
    }
    return ret;
}

int evalute_straight(char c[17], char* backcards)
{
    return 0;
}



int ddzai_evaluate_call_point(cards_buf hand_card[DDZ_SEATS], int id, char backcards[3])
{
    int value = 0;
    value += evalute_king(hand_card[id].cards);
    value += evalute_2(hand_card[id].cards);
    value += evalute_bomb(hand_card[id].cards);
    value += evalute_straight(hand_card[id].cards, backcards);

    return value;
}