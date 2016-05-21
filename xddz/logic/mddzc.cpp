#include "mddzc.h"
#include "mddz_util.h"
#include <string.h>

void sort_by_num2(char* cards, int len)
{
    change_12_2_A2(cards, len);
    sort_by_num(cards, len);
    change_A2_2_12(cards, len);
}

void sort_cards2(char* cards, int len, int lz)
{
    change_12_2_A2(cards, len);
    sort_cards(cards, len, change_12_2_A2(lz));
    change_A2_2_12(cards, len);
}

bool ddz_compare2(cards_buf& c1, cards_buf& c2, int lz)
{
    cards_buf cc1 = c1;
    cards_buf cc2 = c2;
    change_12_2_A2(cc1.cards, cc1.size);
    change_12_2_A2(cc2.cards, cc2.size);
    return ddz_compare(cc1, cc2, change_12_2_A2(lz));
}

int ddz_get_cards_type_list2(cards_buf& src, int lz, cards_buf buf[MAX_CANDIDATE])
{
    memset(buf, 0, sizeof(cards_buf)*MAX_CANDIDATE);
    cards_buf src1 = src;
    change_12_2_A2(src1.cards, src1.size);
    
    int n = ddz_get_cards_type_list(src1, change_12_2_A2(lz), buf);

    for(int i=0; i<n; i++)
    {
        change_A2_2_12(buf[i].cards, buf[i].size);
        change_A2_2_12(buf[i].clz.lz_card, buf[i].clz.size);
    }
    
    return n;
}

int ddz_get_bigger_compose2(cards_buf& src, cards_buf& candidate, int lz, cards_buf out[MAX_CANDIDATE])
{
    cards_buf src1 = src;
    cards_buf candi = candidate;
    change_12_2_A2(src1.cards, src1.size);
    change_12_2_A2(candi.cards, candi.size);
    memset(out, 0, sizeof(cards_buf)*MAX_CANDIDATE);

    int n = ddz_get_bigger_compose(src1, candi, change_12_2_A2(lz), out);
    
    for(int i=0; i<n; i++)
    {
        change_A2_2_12(out[i].cards, out[i].size);
        change_A2_2_12(out[i].clz.lz_card, out[i].clz.size);
    }

    return n;
}

bool ddz_has_straight2(cards_buf& src, cards_buf& out, int lz)
{
    cards_buf src1 = src;
    change_12_2_A2(src1.cards, src1.size);
    bool b = ddz_has_straight(src1, out, change_12_2_A2(lz));
    if(b)
    {
        change_A2_2_12(out.cards, out.size);
        change_A2_2_12(out.clz.lz_card, out.clz.size);
    }
    return b;
}