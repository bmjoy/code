#include "ddzai_analizer.h"
#include <string.h>
#include "mddz_util.h"

int pre_split_straight(char* c, int len, card_split_result* result)
{
    card_stat stat_buf[16];
    int n_stat = calculate_cards_stat(c, len, stat_buf);

    for(int i=n_stat-1; i>=4; i--)
    {
        if((stat_buf[i-4].v-stat_buf[i].v)==4 && stat_buf[i-4].v<=CARD_A)
        {
            int index = result->size;
            for(int j=0; j<5; j++)
            {
                result->item[index].cards[j] = stat_buf[i-(4-j)].c[0];
            }
            result->item[index].type = MDDZ_CT_STRAIGHT;
            result->item[index].size = 5;
            result->item[index].maxv = C_N(result->item[index].cards[0]);

            char tmp[20];
            len = get_remain(c, len, result->item[index].cards, 5, tmp);
            memcpy(c, tmp, len);
            result->size ++;
            return len;
        }
    }
    return len;
}

int split_straight(char* c, int len, card_split_result* result)
{
    char buf[20];
    memcpy(buf, c, len);
    card_split_item* item = result->item;
    int from = result->size - 1;
    int l1 = len;
    int l = pre_split_straight(buf, l1, result);
    while (l >= 5 && l1>l)
    {
        l1 = l;
        l = pre_split_straight(buf, l1, result);
    }
    int size = result->size;
    if((size-from) == 3)
    {
        if(item[from+1].maxv == item[from+2].maxv)
        {
            char buf[20];
            int  index = 0;
            for(int i=0; i<5; i++)
            {
                buf[index] = item[from+1].cards[i];
                index ++;
                buf[index] = item[from+2].cards[i];
                index ++;
            }
            item[from+1].size = 10;
            item[from+1].type = MDDZ_CT_TWO_STRAIGHT;
            memcpy(item[from].cards, buf, 10);

            result->size --;
            goto end;
        }
    }
    int n = l;

    for(int i=0; i<n; i++)
    {
        for(int j=from+1; j<size; j++)
        {

            if((C_N(buf[i])-item[j].maxv) == 1)
            {
                for(int k=item[j].size; k>0; k--)
                {
                    item[j].cards[k] = item[j].cards[k-1];
                }
                item[j].cards[0] = buf[j];
                item[j].size++;
                item[j].maxv = C_N(buf[j]);
                break;
            }
            if((C_N(item[j].cards[item[j].size-1])-C_N(buf[i])) == 1)
            {
                item[j].cards[item[j].size] = buf[i];
                item[j].size ++;
                break;
            }
        }

    }
end:
    char tmp[20];
    char tmp1[20];
    memcpy(tmp, c, len);
    l = len;
    if(result->size>from)
    {
        for(int i=from+1; i<result->size; i++)
        {
            l = get_remain(tmp, l, item[i].cards, item[i].size, tmp1);
            memcpy(tmp, tmp1, l);
        }
    }
    memcpy(c, tmp, l);
    return l;
}

int has_three_straight(char* c, int len)
{
    card_stat stat_buf[16];
    int n_stat = calculate_cards_stat(c, len, stat_buf);
    for(int i=0; i<n_stat-1; i++)
    {
        if(stat_buf[i].v<=CARD_A && stat_buf[i].n>2 && 
            stat_buf[i+1].n>2 && (stat_buf[i].v - stat_buf[i+1].v) == 1)
        {
            if(i+2<n_stat &&
                stat_buf[i+2].n > 2 && stat_buf[i+1].v - stat_buf[i+2].v == 1)
                return 3;
            return 2;
        }
    }
    return 0;
}

int has_tow_straight(char* c, int len)
{
    card_stat stat_buf[16];
    int n_stat = calculate_cards_stat(c, len, stat_buf);
    for(int i=0; i<n_stat-2; i++)
    {
        if(stat_buf[i].v <= CARD_A && stat_buf[i].n > 1 &&
            (stat_buf[i].v-stat_buf[i+2].v) == 2 &&
            stat_buf[i+1].n>1 && stat_buf[i+2].n>1)
        {
            if((i+3)<n_stat)
            {
                if(stat_buf[i].v-stat_buf[i+3].v == 3 && stat_buf[i+3].n>1)
                    return 4;
            }
            return 3;
        }
    }
    return 0;
}

int split_rocket(char* c, int len, card_split_result* result)
{
    if(len < 2) return len;
    if(c[0] == 0x4f && c[1] == 0x4e)
    {
        result->item[result->size].cards[0] = 0x4f;
        result->item[result->size].cards[1] = 0x4e;
        result->item[result->size].type = MDDZ_CT_ROCKET;
        result->item[result->size].size = 2;
        result->item[result->size].maxv = 0x0f;
        for(int i=0; i<len; i++) c[i-2] = c[i];
        result->size ++;
        return len - 2;
    }
    return len;
}

int split_bomb(char* c, int len, card_split_result* result)
{
    card_stat stat_buf[16];
    int n_stat = calculate_cards_stat(c, len, stat_buf);
    int retlen = 0;
    for(int i=0; i<n_stat; i++)
    {
        if(stat_buf[i].n == 4)
        {
            memcpy(result->item[result->size].cards, stat_buf[i].c, 4);
            result->item[result->size].maxv = stat_buf[i].v;
            result->item[result->size].size = 4;
            result->item[result->size].type = MDDZ_CT_BOMB;
            result->size ++;
        }
        else
        {
            memcpy(&c[retlen], stat_buf[i].c, stat_buf[i].n);
            retlen += stat_buf[i].n;
        }
    }
    
    return retlen;
}

int split_three(char* c, int len, card_split_result* result)
{
    card_stat stat_buf[16];
    int n_stat = calculate_cards_stat(c, len, stat_buf);
    int retlen = 0;
    for(int i=0; i<n_stat; i++)
    {
        if(stat_buf[i].n == 3)
        {
            memcpy(result->item[result->size].cards, stat_buf[i].c, 3);
            result->item[result->size].maxv = stat_buf[i].v;
            result->item[result->size].size = 3;
            result->item[result->size].type = MDDZ_CT_THREE;
            result->size ++;
        }
        else
        {
            memcpy(&c[retlen], stat_buf[i].c, stat_buf[i].n);
            retlen += stat_buf[i].n;
        }
    }

    return retlen;
}


int split_two_straight(char* c, int len, card_split_result* result)
{
    int size = result->size;
    card_split_item* item = result->item;
    card_stat stat_buf[16];
    int n_stat = calculate_cards_stat(c, len, stat_buf);
    int from=-1;
    for(int i=0; i<n_stat-2;)
    {
        if(stat_buf[i].v<=CARD_A && stat_buf[i].n>=2 && stat_buf[i+1].n>=2 && stat_buf[i+2].n>=2 &&
            (stat_buf[i].v-stat_buf[i+2].v) == 2)
        {
            memcpy(item[size].cards, stat_buf[i].c, 2);
            memcpy(&item[size].cards[2], stat_buf[i+1].c, 2);
            memcpy(&item[size].cards[4], stat_buf[i+1].c, 2);
            item[size].maxv = stat_buf[i].v;
            item[size].type = MDDZ_CT_TWO_STRAIGHT;
            item[size].size = 6;
            for(int j=i+3; j<n_stat; j++)
            {
                if(stat_buf[j].n>=2 && (stat_buf[i].v-stat_buf[j].v)==(j-i))
                {
                    memcpy(&item[size].cards[6+(j-3)*2], stat_buf[j].c, 2);
                    item[size].size += 2;
                }
                else
                {
                    break;
                }
            }
            i += (item[size].size / 2);
            if(from<0)from=size;
            size ++;
        }
        else
        {
            i++;
        }
    }
    char tmp[20];
    char tmp1[20];
    int  l = 0;
    memcpy(tmp, c, len);
    l = len;
    if(from>=0)
    {
        for(int i=from; i<size; i++)
        {
            l = get_remain(tmp, l, item[i].cards, item[i].size, tmp1);
            memcpy(tmp, tmp1, l);
        }
    }
    memcpy(c, tmp, l);

    result->size = size;
    return l;
}

int split_three_with_one(char* c, int len, card_split_result* result)
{
    int size = result->size;
    card_split_item* item = result->item;
    card_stat stat_buf[16];
    int n_stat = calculate_cards_stat(c, len, stat_buf);
    int from = -1;
    for(int i=0; i<n_stat; i++)
    {
        if(stat_buf[i].n == 3)
        {
            bool b = false;
            for(int j=n_stat-1; j>=0; j--)
            {
                if(i==j)continue;
                if(stat_buf[j].n == 1)
                {
                    if(from<0)from=size;
                    b = true;
                    memcpy(item[size].cards, stat_buf[i].c, 3);
                    item[size].cards[3] = stat_buf[j].c[0];
                    item[size].size = 4;
                    item[size].type = MDDZ_CT_THREE_WITH_ONE;
                    size ++;
                    break;
                }
            }
            if(b)continue;
            for(int j=n_stat-1; j>=0; j--)
            {
                if(i==j)continue;
                if(stat_buf[j].n > 1)
                {
                    if(from<0)from=size;
                    memcpy(item[size].cards, stat_buf[i].c, 3);
                    item[size].cards[3] = stat_buf[j].c[0];
                    item[size].size = 4;
                    item[size].type = MDDZ_CT_THREE_WITH_ONE;
                    size ++;
                    break;
                }
            }
        }
    }
    char tmp[20];
    char tmp1[20];
    int  l = 0;
    memcpy(tmp, c, len);
    l = len;
    if(from>=0)
    {
        for(int i=from; i<size; i++)
        {
            l = get_remain(tmp, l, item[i].cards, item[i].size, tmp1);
            memcpy(tmp, tmp1, l);
        }
    }
    memcpy(c, tmp, l);

    result->size = size;
    return l;
}

int split_three_with_pair(char* c, int len, card_split_result* result)
{
    int size = result->size;
    card_split_item* item = result->item;
    card_stat stat_buf[16];
    int n_stat = calculate_cards_stat(c, len, stat_buf);
    int from = -1;
    for(int i=0; i<n_stat; i++)
    {
        if(stat_buf[i].n == 3)
        {
            bool b = false;
            for(int j=n_stat-1; j>=0; j--)
            {
                if(i==j)continue;
                if(stat_buf[j].n == 2)
                {
                    if(from<0)from=size;
                    b = true;
                    memcpy(item[size].cards, stat_buf[i].c, 3);
                    item[size].cards[3] = stat_buf[j].c[0];
                    item[size].cards[4] = stat_buf[j].c[1];
                    item[size].size = 5;
                    item[size].type = MDDZ_CT_THREE_WITH_PAIR;
                    size ++;
                    break;
                }
            }
            if(b)continue;
            for(int j=n_stat-1; j>=0; j--)
            {
                if(i==j)continue;
                if(stat_buf[j].n > 2)
                {
                    if(from<0)from=size;
                    memcpy(item[size].cards, stat_buf[i].c, 3);
                    item[size].cards[3] = stat_buf[j].c[0];
                    item[size].cards[4] = stat_buf[j].c[1];
                    item[size].size = 5;
                    item[size].type = MDDZ_CT_THREE_WITH_PAIR;
                    size ++;
                    break;
                }
            }
        }
    }
    char tmp[20];
    char tmp1[20];
    int  l = 0;
    memcpy(tmp, c, len);
    l = len;
    if(from>=0)
    {
        for(int i=from; i<size; i++)
        {
            l = get_remain(tmp, l, item[i].cards, item[i].size, tmp1);
            memcpy(tmp, tmp1, l);
        }
    }
    memcpy(c, tmp, l);

    result->size = size;
    return l;
}

int split_plane(char* c, int len, card_split_result* result)
{
    int size = result->size;
    card_split_item* item = result->item;
    card_stat stat_buf[16];
    int n_stat = calculate_cards_stat(c, len, stat_buf);
    int  pn = 0;
    int from=-1;
    for(int i=0; i<n_stat-1;)
    {
        if(stat_buf[i].v<=CARD_A && stat_buf[i].n > 2 && stat_buf[i+1].n>2 && (stat_buf[i].v-stat_buf[i+1].v)==1)
        {
            memcpy(item[size].cards, stat_buf[i].c, 3);
            memcpy(&item[size].cards[3], stat_buf[i+1].c, 3);
            item[size].maxv = stat_buf[i].v;
            item[size].size = 6;
            item[size].type = MDDZ_CT_THREE_STRAIGHT;
            if((i+2)<n_stat && stat_buf[i+2].n>2 && (stat_buf[i].v-stat_buf[i+2].v) == 2)
            {
                memcpy(&item[size].cards[6], stat_buf[i+2].c, 3);
                item[size].size = 9;
                i+=3;
            }
            else
            {
                i+=2;
            }
            if(from<0)from=size;
            size ++;
        }
        else
        {
            i++;
        }
    }
    result->size = size;
    char tmp[20];
    char tmp1[20];
    int l = len;
    memcpy(tmp, c, len);
    if(from>=0)
    {
        for(int i=from; i<size; i++)
        {
            l = get_remain(tmp, l, item[i].cards, item[i].size, tmp1);
            memcpy(tmp, tmp1, l);
        }
    }
    memcpy(c, tmp, l);
    return l;
}

int split_pair(char* c, int len, card_split_result* result)
{
    int size = result->size;
    card_split_item* item = result->item;
    card_stat stat_buf[16];
    int n_stat = calculate_cards_stat(c, len, stat_buf);
    int  l = 0;
    for(int i=0; i<n_stat; i++)
    {
        if(stat_buf[i].n == 2)
        {
            memcpy(item[size].cards, stat_buf[i].c, 2);
            item[size].size = 2;
            item[size].maxv = C_N(stat_buf[i].c[0]);
            item[size].type = MDDZ_CT_PAIR;
            size ++;
            continue;
        }
        memcpy(&c[l], stat_buf[i].c, stat_buf[i].n);
        l += stat_buf[i].n;
    }
    result->size = size;
    return l;
}

void split_one(char* c, int len, card_split_result* result)
{
    int size = result->size;
    card_split_item* item = result->item;

    for(int i=0; i<len; i++)
    {
        item[size].cards[0] = c[i];
        item[size].maxv = C_N(c[i]);
        item[size].size = 1;
        item[size].type = MDDZ_CT_ONE;
        size ++;
    }
    result->size = size;
}

void split_analize(char* buf, int len, card_split_result* result)
{
    char c[20];
    memcpy(c, buf, len);
    zerom((char*)result, sizeof(card_split_result));
    sort_by_num(c, len);
    len = split_rocket(c, len, result);
    len = split_bomb(c, len, result);
    len = split_straight(c, len, result);
    len = split_plane(c, len, result);
    len = split_two_straight(c, len, result);
    len = split_three_with_pair(c, len, result);
    len = split_three_with_one(c, len, result);
    len = split_three(c, len, result);
    if(len == 0) return;
    split_one(c, len, result);
}

