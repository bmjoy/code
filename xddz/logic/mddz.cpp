#include "mddz.h"
#include <stdlib.h>
#ifdef _WIN32
#include <time.h>
#include <Windows.h>
#endif
#include <assert.h>
#include "ai_helper.h"

//扑克数据
const char CARD_DATA[CARD_COUNT]=
{
    0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,	//方块 a - k
    0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,	//梅花 a - k
    0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,	//红桃 a - k
    0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,	//黑桃 a - k
    0x4e,0x4f
};
#define VALID_LZ_PARAM if(lz>0 && !out) return 0;

out_cards::out_cards()
{
    clear();
}

void out_cards::clear()
{
    size = 0;
    memset(turns, 0, sizeof(turns));
    set_first(0);
}

void out_cards::set_first(int n)
{
    first = n;
}

void out_cards::add(out_cards_buf d)
{
    if(size >= 60)
        return;
    turns[size] = d;
    size ++;
}

void out_cards::get_last(out_cards_buf& d)
{
    d.id = -1;
    d.size = 0;
    if(size < 1)
        return;
    d = turns[size - 1];

    if(d.size == 0 && size > 1)
    {
        d = turns[size - 2];
    }
    if(d.size == 0 && size > 2)
    {
        d = turns[size - 3];
    }
    if(d.size == 0)
        d.id = -1;
}

int out_cards::get_next_operator(int& new_turn)
{
    new_turn = 1;
    if(size == 0)
        return first;
    int ret = turns[size-1].id;
    ret ++;
    if(ret > 2) ret = 0;
    if(size < 3)
        new_turn = 0;
    else
    {
        if(turns[size-1].size == 0 && turns[size-2].size == 0)
            new_turn = 1;
        else
            new_turn = 0;
    }
    return ret;
}

void out_cards::change_A2_2_12()
{
    for(int i=0; i<size; i++)
    {
        if(turns[i].size == 0)
            continue;
        ::change_A2_2_12(turns[i].cards, turns[i].size);
    }
}

void out_cards::change_12_2_A2()
{
    for(int i=0; i<size; i++)
    {
        if(turns[i].size == 0)
            continue;
        ::change_12_2_A2(turns[i].cards, turns[i].size);
    }
}

//////////////////////////////////////////////////////////////////////////
int out_card(char* handcards, int len, char* outcards, int ol)
{
    char temp[20];
    int  count = 0;
    for(int i=0; i<len; i++)
    {
        if(count == (len - ol))
            break;
        bool exist = false;
        for(int j=0; j<ol; j++)
        {
            if(handcards[i] == outcards[j])
            {
                exist = true;
                break;
            }
        }
        if(!exist)
        {
            temp[count] = handcards[i];
            count ++;
        }
    }
    for(int i=0; i<count; i++)handcards[i] = temp[i];
    return count;

}

void sort_by_num(char* cards, int len)
{
    //assert(cards && len >= 0 && len <= 20);
    for(int i=0; i<len; i++)
    {
        for(int j=i+1; j<len; j++)
        {
            if((C_N(cards[j]) > C_N(cards[i])) ||
                (C_N(cards[j]) == C_N(cards[i]) && C_T(cards[j]) > C_T(cards[i])))
            {
                char t = cards[i];
                cards[i] = cards[j];
                cards[j] = t;
            }
        }
    }
}

void sort_cards(char* cards, int len, int lz)
{
    assert(cards && len>= 0 && len <= 20);
    if(len <= 0 || len > 54)
        return;
    char buf[64];
    memcpy(buf, cards, len);
    //先将癞子抽出来排序
    char lzbuf[4];
    char rbuf[20];
    int  nlz = 0;
    int  nn = 0;
    for(int i=0; i<len; i++)
    {
        if(C_N(buf[i]) == lz)
        {
            lzbuf[nlz] = buf[i];
            nlz ++;
        }
        else
        {
            rbuf[nn] = buf[i];
            nn ++;
        }
    }
    //再排序剩余牌
    for(int i=0; i<nn; i++)
    {
        for (int j=i+1; j<nn; j++)
        {
            if((C_N(rbuf[j]) > C_N(rbuf[i])) ||
                (C_N(rbuf[j]) == C_N(rbuf[i]) && C_T(rbuf[j]) > C_T(rbuf[i])))
            {
                char t = rbuf[i];
                rbuf[i] = rbuf[j];
                rbuf[j] = t;
            }
        }
    }
    //合并癞子与普通牌
    memcpy(buf, lzbuf, nlz);
    memcpy(&buf[nlz], rbuf, nn);

    memcpy(cards, buf, len);
}

bool analyze(char* cards, int size, int lz, CardAnalyzInfo* info)
{
    char buf[54];
    memset(info, 0, sizeof(CardAnalyzInfo));
    memcpy(buf, cards, size);
    sort_by_num(buf, size);
    memset(info, 0, sizeof(CardAnalyzInfo));
    for(int i=0; i<size; i++)
    {
        char cv = C_N(buf[i]);
        if(cv == lz)
        {
            info->lzbuf[info->nlz] = buf[i];
            info->nlz++;
            if(info->nlz>4)
                return false;
        }
        else
        {
            info->normal[info->nsize] = buf[i];
            info->nsize ++;
        }
    }
    info->n_stat = 0;
    if(info->nsize<=0)
        return true;
    char check_card = C_N(info->normal[0]);
    info->stat_buf[info->n_stat].v = check_card;
    info->stat_buf[info->n_stat].n = 1;
    info->stat_buf[info->n_stat].c[0] = info->normal[0];
    for(int i=1; i<info->nsize; i++)
    {
        check_card = C_N(info->normal[i]);
        if(check_card == info->stat_buf[info->n_stat].v)
        {
            info->stat_buf[info->n_stat].c[info->stat_buf[info->n_stat].n] = info->normal[i];
            info->stat_buf[info->n_stat].n ++;
            if(info->stat_buf[info->n_stat].n > 4)
                return false;
        }
        else
        {
            info->n_stat ++;
            if(info->n_stat >= 15)
                return false;
            info->stat_buf[info->n_stat].v = check_card;
            info->stat_buf[info->n_stat].n = 1;
            info->stat_buf[info->n_stat].c[0] = info->normal[i];
        }
        if(info->stat_buf[info->n_stat].n > info->maxn)
            info->maxn = info->stat_buf[info->n_stat].n;
    }
    info->n_stat ++;
    return true;
}

bool has_king(char* cards, int len)
{
    assert(cards && len >= 0 && len <= 20);
    for(int i=0; i<len; i++)
    {
        if(C_T(cards[i]) == C_TYPE_KING)
            return true;
    }
    return false;
}

void shift_random()
{
#ifdef _WIN32
    LARGE_INTEGER lg;
    QueryPerformanceCounter(&lg);
    int cc = lg.LowPart % 3;
    for(int i=0; i<cc; i++)
        rand();
#endif
}

bool g_is_init = false;

int ddz_shuffle(char hand_cards[3][17], char back[3])
{
    if(!g_is_init)
    {
#ifdef _WIN32
        srand(_time32(0));
#endif
    }
    shift_random();
    char buf[CARD_COUNT];
    memcpy(buf, CARD_DATA, CARD_COUNT);
    for(int i=0; i<CARD_COUNT; i++)
    {
        int r = rand()%CARD_COUNT;
        if(r != i)
        {
            char t = buf[i];
            buf[i] = buf[r];
            buf[r] = t;
        }
    }
    int index = 0;
    for(int i=0; i<17; i++)
    {
        for(int j=0; j<3; j++)
        {
            hand_cards[j][i] = buf[index];
            index ++;
        }
    }
    memcpy(back, &buf[index], 3);

    return 1+rand()%0x0d;
}

void get_lz_size(char* cards, int len, int lz, ddz_lz_item* out)
{
    if(!out)
        return;
    int size = 0;
    for(int i=0; i<len; i++)
    {
        if( (cards[i]&0x0f) == lz )
            size ++;
    }
    out->size = size;
}

bool check_exist_cards(char* handcards, int len, char* outcards, int ol)
{
    bool exist = false;
    for(int i=0; i<ol; i++)
    {
        exist = false;
        for(int j=0; j<len; j++)
        {
            if(outcards[i] == handcards[j])
            {
                exist = true;
                break;
            }
        }
        if(!exist)
            return false;
    }
    return true;
}

char is_pair(CT_PARAM_DEF)                  //一对
{
    VALID_LZ_PARAM;
    if(len != 2)
        return 0;
    if(has_king(cards, len))
        return 0;

    if(C_N(cards[0]) == C_N(cards[1]))
    {
        if(!out)
            out->size = 0;
        return C_N(cards[0]);
    }

    if (info.n_stat > 2)
    {
        return 0;
    }
    if(info.nlz == 1 && info.n_stat == 1)
    {
        cards[0] = info.normal[0];
        cards[1] = LZ(cards[0]);
        out->size = 1;
        out->lz_card[0].card = info.lzbuf[0];
        out->lz_card[0].change = cards[1];
        return C_N(cards[0]);
    }

    return 0;
}

char is_three(CT_PARAM_DEF)                 //三条
{
    VALID_LZ_PARAM;
    if(len != 3)
        return 0;
    if(has_king(cards, len))
        return 0;

    if(C_N(cards[0]) == C_N(cards[1]) && C_N(cards[0]) == C_N(cards[2]))
    {
        if(!out)
            out->size = 0;
        return C_N(cards[0]);
    }

    if(lz == 0)
        return 0;
    if(info.n_stat > 1)
        return 0;
    memcpy(cards, info.normal, info.nsize);
    for(int i=0; i<info.nlz; i++)
    {
        cards[i + info.nsize] = LZ(cards[0]);
        out->lz_card[i].card = info.lzbuf[i];
        out->lz_card[i].change = cards[i + info.nsize];
    }
    if(out)
    {
        out->size = info.nlz;
    }
    return C_N(cards[0]);
}

char is_straight(CT_PARAM_DEF)              //单顺
{
    VALID_LZ_PARAM;
    if(len<5 || len>12)
        return 0;
    if(info.maxn > 1)
        return 0;

    for(int i=0; i<info.n_stat; i++)
    {
        if(info.stat_buf[i].v > CARD_A)
            return 0;
    }
    int dn = len - (info.stat_buf[0].v - info.stat_buf[info.n_stat-1].v+1);
    if(dn > info.nlz)
        return 0;
    int rn = info.nlz - ((info.stat_buf[0].v - info.stat_buf[info.n_stat-1].v) - (info.n_stat-1));
    char expv = info.stat_buf[0].v;
    while(rn > 0)
    {
        if(expv<CARD_A)
            expv++;
        else
            break;
        rn --;
    }
    int sn = 0;
    int cindex = 0;
    int lzindex = 0;
    for(int i=0; i<len; i++)
    {
        if(expv == info.stat_buf[sn].v)
        {
            cards[cindex] = info.stat_buf[sn].c[0];
            cindex ++;
            sn ++;
        }
        else
        {
            if(lzindex >= info.nlz)
                return 0;
            cards[cindex] = LZ(expv);
            out->lz_card[lzindex].card = info.lzbuf[lzindex];
            out->lz_card[lzindex].change = cards[cindex];
            cindex ++;
            lzindex ++;
        }
        expv --;
    }

    if(out)
    {
        out->size = info.nlz;
    }
    return C_N(cards[0]);
}

char is_two_straight(CT_PARAM_DEF)          //双顺
{
    VALID_LZ_PARAM;
    if(len<6 || len%2 != 0)
        return 0;
    //if(has_king_2(cards, len, true))
    //    return 0;
    if(info.maxn > 2)
        return 0;
    for(int i=0; i<info.n_stat; i++)
    {
        if(info.stat_buf[i].v >= CARD_2)
            return 0;
    }
    int  nx = 0; //空穴数

    for(int i=0; i<info.n_stat-1; i++)
    {
        int dx = info.stat_buf[i].v - info.stat_buf[i+1].v;
        nx += (2 - info.stat_buf[i].n);
        if(dx > 1)
        {
            nx += (dx - 1) * 2;
            //断点空位过多,无法插入补齐
        }
        if(nx > info.nlz)
            return 0;
    }
    nx += (2 - info.stat_buf[info.n_stat-1].n);

    int rn = info.nlz - nx; // 剩余癞子牌
    if(rn % 2 != 0)
        return 0;
    rn /= 2;
    int cindex = 0;
    int lzindex = 0;
    char expv = info.stat_buf[0].v;
    while(rn>0)
    {
        if(expv < CARD_A)
            expv ++;
        else
            break;
        rn--;
    }
    int wings = len / 2;
    int sn = 0;
    for(int i=0; i<wings; i++)
    {
        if(expv == info.stat_buf[sn].v)
        {
            memcpy(&cards[cindex], info.stat_buf[sn].c, info.stat_buf[sn].n);
            cindex += info.stat_buf[sn].n;
            if(info.stat_buf[sn].n == 1)
            {
                if(lzindex>= info.nlz)
                    return 0;
                cards[cindex] = LZ(expv);
                out->lz_card[lzindex].card = info.lzbuf[lzindex];
                out->lz_card[lzindex].change = cards[cindex];
                cindex++;
                lzindex++;
            }
            sn ++;
        }
        else
        {
            for(int j=0; j<2; j++)
            {
                if(lzindex>= info.nlz)
                    return 0;
                cards[cindex] = LZ(expv);
                out->lz_card[lzindex].card = info.lzbuf[lzindex];
                out->lz_card[lzindex].change = cards[cindex];
                cindex++;
                lzindex++;
            }
        }
        expv --;
    }

    if(out)
    {
        out->size = info.nlz;
    }
    return C_N(cards[0]);
}

char is_three_straight(CT_PARAM_DEF)        //三顺
{
    VALID_LZ_PARAM;
    if(len<6 || len%3 != 0)
        return 0;
    if(info.maxn > 3)
        return 0;

    int lzindex = 0;
    int wings = len / 3;  //翅膀个数
    if(info.n_stat > wings)
        return 0;
    int cindex = 0;
    int hole = 0;
    for(int i=0; i<info.n_stat; i++)
    {
        if(info.stat_buf[i].v >= CARD_2)
            return 0;
    }
    for(int i=0; i<info.n_stat-1; i++)
    {
        if((info.stat_buf[i].v - info.stat_buf[i+1].v) > 2)
            return 0;
        if((info.stat_buf[i].v - info.stat_buf[i+1].v) == 2)
            hole ++;
        if(hole>1)
            return 0;
    }
    int expv = info.stat_buf[0].v;
    if(info.n_stat < wings)
    {
        if(info.stat_buf[0].v < CARD_A && hole == 0)
            expv ++;
    }
    int sn = 0;
    for(int i=0; i<wings; i++)
    {
        if(expv == info.stat_buf[sn].v)
        {
            memcpy(&cards[cindex], info.stat_buf[sn].c, info.stat_buf[sn].n);
            cindex += info.stat_buf[sn].n;
            if(sn < 3)
            {
                for(int j=0; j<(3-info.stat_buf[sn].n); j++)
                {
                    if(lzindex>=info.nlz)
                        return 0;
                    cards[cindex] = LZ(expv);
                    out->lz_card[lzindex].card = info.lzbuf[lzindex];
                    out->lz_card[lzindex].change = cards[cindex];
                    lzindex ++;
                    cindex ++;
                }
            }
            sn++;
        }
        else
        {
            for(int j=0; j<3; j++)
            {
                if(lzindex>=info.nlz)
                    return 0;
                cards[cindex] = LZ(expv);
                out->lz_card[lzindex].card = info.lzbuf[lzindex];
                out->lz_card[lzindex].change = cards[cindex];
                lzindex ++;
                cindex ++;
            }
        }
        expv --;
    }

    if(out)
    {
        out->size = info.nlz;
    }
    return C_N(cards[0]);
}

char is_three_with_one(CT_PARAM_DEF)        //三带一单
{
    VALID_LZ_PARAM;
    if(len != 4)
        return 0;

    if(info.maxn > 3)
        return 0;
    if(info.nlz == 3)
    {
        if(C_T(info.normal[0]) == C_TYPE_KING || lz > C_N(info.normal[0]))
        {
            memcpy(cards, info.lzbuf, 3);
            cards[3] = info.normal[0];
            out->size = 0;
            return C_N(lz);
        }
        cards[0] = info.normal[0];
        cards[1] = LZ(info.normal[0]);
        cards[2] = LZ(info.normal[0]);
        cards[3] = info.lzbuf[0];

        out->size = 2;
        out->lz_card[0].card = info.lzbuf[1];
        out->lz_card[0].change = cards[1];
        out->lz_card[1].card = info.lzbuf[2];
        out->lz_card[1].change = cards[2];
        return C_N(cards[0]);
    }
    if(info.n_stat == 1)
    {
        if(info.nsize>3 || info.nsize<1)
            return 0;
        memcpy(cards, info.stat_buf[0].c, info.stat_buf[0].n);
        for(int i=info.stat_buf[0].n; i<3; i++)
        {
            cards[i] = LZ(info.stat_buf[0].v);
            out->lz_card[i-info.stat_buf[0].n].card = info.lzbuf[i-info.stat_buf[0].n];
            out->lz_card[i-info.stat_buf[0].n].change = cards[i];
        }
        out->size = 3 - info.stat_buf[0].n;
        cards[3] = info.lzbuf[info.nlz-1];
        return C_N(cards[0]);
    }
    if(info.n_stat != 2)
        return 0;
    //不能替王
    if(((info.stat_buf[0].n + info.nlz) != 3) ||
        (info.stat_buf[0].v > (CARD_A+1)) )
    {
        card_stat t = info.stat_buf[0];
        info.stat_buf[0] = info.stat_buf[1];
        info.stat_buf[1] = t;
    }
    if(((info.stat_buf[0].n + info.nlz) != 3) ||
        (info.stat_buf[0].v > (CARD_A+1)) )
        return 0;
    memcpy(cards, info.stat_buf[0].c, info.stat_buf[0].n);
    int lzindex = 0;
    for(int i=info.stat_buf[0].n; i<3; i++)
    {
        cards[i] = LZ(info.stat_buf[0].v);
        out->lz_card[lzindex].card = info.lzbuf[lzindex];
        out->lz_card[lzindex].change = cards[i];
        lzindex ++;
    }
    cards[3] = info.stat_buf[1].c[0];
    if(out)
    {
        out->size = info.nlz;
    }
    return C_N(cards[0]);
}

char is_three_with_pair(CT_PARAM_DEF)       //三带一对
{
    VALID_LZ_PARAM;
    if(len != 5)
        return 0;
    if(has_king(cards, len))
        return 0;

    int lzindex = 0;
    if(info.n_stat == 1)
    {
        if(info.stat_buf[0].n == 4)
        {
            return 0;
            memcpy(cards, info.stat_buf[0].c, 4);
            cards[4] = LZ(info.stat_buf[0].v);
            out->lz_card[0].card = info.lzbuf[0];
            out->lz_card[0].change = cards[4];
            out->size =1;
        }
        else if((info.stat_buf[0].n+info.nlz) != 5)
            return 0;
        else if(info.stat_buf[0].n == 3)
        {
            if(info.nlz != 2) return 0;
            memcpy(cards, info.stat_buf[0].c, info.stat_buf[0].n);
            memcpy(&cards[3], info.lzbuf, 2);
            out->size = 0;
        }
        else if(info.stat_buf[0].v > lz)
        {
            memcpy(cards, info.stat_buf[0].c, info.stat_buf[0].n);
            for(int i=info.stat_buf[0].n; i<3; i++)
            {
                cards[i] = LZ(info.stat_buf[0].v);
                out->lz_card[lzindex].card = info.lzbuf[lzindex];
                out->lz_card[lzindex].change = cards[i];
                out->size ++;
                lzindex ++;
            }
            memcpy(&cards[3], &info.lzbuf[lzindex], 2);
        }
        else
        {
            memcpy(cards, info.lzbuf, 3);
            memcpy(&cards[3], info.stat_buf[0].c, info.stat_buf[0].n);
            if(info.stat_buf[0].n == 1)
            {
                cards[4] = LZ(cards[3]);
                out->lz_card[0].card = info.lzbuf[0];
                out->lz_card[0].change = cards[4];
                out->size = 1;
            }
            else
            {
                out->size = 0;
            }
        }
        return C_N(cards[0]);
    }
    if(info.n_stat != 2)
        return 0;
    lzindex = 0;
    if(((info.stat_buf[0].n + info.nlz) < 3) ||
        (info.stat_buf[0].v > (CARD_A+1)) )
    {
        card_stat t = info.stat_buf[0];
        info.stat_buf[0] = info.stat_buf[1];
        info.stat_buf[1] = t;
    }
    if(((info.stat_buf[0].n + info.nlz) < 3) ||
        (info.stat_buf[0].v > (CARD_A+1)) )
        return 0;
    memcpy(cards, info.stat_buf[0].c, info.stat_buf[0].n);
    for(int i=info.stat_buf[0].n; i<3; i++)
    {
        cards[i] = LZ(info.stat_buf[0].v);
        out->lz_card[lzindex].card = info.lzbuf[lzindex];
        out->lz_card[lzindex].change = cards[i];
        lzindex ++;
    }
    memcpy(&cards[3], info.stat_buf[1].c, info.stat_buf[1].n);
    if(info.stat_buf[1].n == 1)
    {
        if(info.nlz<1)
            return 0;
        cards[4] = LZ(info.stat_buf[1].v);
        out->lz_card[lzindex].card = info.lzbuf[lzindex];
        out->lz_card[lzindex].change = cards[4];
        lzindex ++;
    }
    if(out)
    {
        out->size = info.nlz;
    }
    return C_N(cards[0]);
}

char is_three_straight_with_n(CT_PARAM_DEF) //飞机带单翅膀
{
    VALID_LZ_PARAM;
    if(len < 8 || (len%4) != 0)
        return 0;

    int wings = len / 4;  //翅膀个数
    int n_wing = 0;
    int nx = -1;
    int n = len / 4;
    int startindex = -1;
    int is_replace3 = false;
    if(info.n_stat > (wings*2))
        return 0;

    //找到飞机残骸起止位置
    for(int i=0; i<info.n_stat; i++)
    {
        nx = 0;
        is_replace3 = false;
        if(info.stat_buf[i].v > CARD_A)
            continue;

        startindex = i;
        n_wing = 1;
        nx += (3-info.stat_buf[i].n);
        if(nx>info.nlz)
        {
            nx = 0;
            continue;
        }

        if(n_wing == 0)
            continue;
        for(int j=i+1; j<info.n_stat; j++)
        {
            int dx = info.stat_buf[j-1].v - info.stat_buf[j].v;
            nx += (3-info.stat_buf[j].n);

            if(dx > 1)
            {
                is_replace3 = true;
                nx += (dx-1)*3;
            }
            if(nx > info.nlz)
            {
                n_wing = 0;
                break;
            }
            n_wing ++;
            if(n_wing == wings)
            {
                break;
            }
        }
        if(n_wing == wings)
        {
            break;
        }
    }
    if((wings - n_wing) == 1)
    {
        nx += 3;
        if(nx > info.nlz)
            return 0;
        n_wing ++;
        is_replace3 = true;
    }
    if(n_wing != wings || startindex < 0) return 0;
    
    int cindex = 0;
    int lzindex = 0;
    int fj_index = startindex;

    //先输出飞机
    if(is_replace3)
    {//需要补三张一样的牌
        char v = info.stat_buf[startindex].v;
        if(v < CARD_A)
        {//补到前面
            for(int i=0; i<3; i++)
            {
                cards[i] = LZ(v+1);
                out->lz_card[i].card = info.lzbuf[i];
                out->lz_card[i].change = cards[i];
            }
            cindex = 3;
            lzindex = 3;
            for(int i=1; i<n; i++)
            {
                memcpy(&cards[cindex], info.stat_buf[fj_index].c, info.stat_buf[fj_index].n);
                cindex += info.stat_buf[fj_index].n;
                for(int j=0; j<3-info.stat_buf[fj_index].n; j++)
                {
                    cards[cindex] = LZ(info.stat_buf[fj_index].v);
                    out->lz_card[lzindex].card = info.lzbuf[lzindex];
                    out->lz_card[lzindex].change = cards[cindex];
                    cindex ++;
                    lzindex ++;
                }
                fj_index ++;
            }
        }
        else
        {//补到后面
            for(int i=0; i<n-1; i++)
            {
                memcpy(&cards[cindex], info.stat_buf[fj_index].c, info.stat_buf[fj_index].n);
                cindex += info.stat_buf[fj_index].n;
                for(int j=0; j<3-info.stat_buf[fj_index].n; j++)
                {
                    cards[cindex] = LZ(info.stat_buf[fj_index].v);
                    out->lz_card[lzindex].card = info.lzbuf[lzindex];
                    out->lz_card[lzindex].change = cards[cindex];
                    cindex ++;
                    lzindex ++;
                }
                v = info.stat_buf[fj_index].v - 1;
                fj_index ++;
            }
            for(int i=0; i<3; i++)
            {
                cards[cindex] = LZ(v);
                out->lz_card[lzindex].card = info.lzbuf[lzindex];
                out->lz_card[lzindex].change = cards[cindex];
                cindex ++;
                lzindex ++;
            }
        }
    }
    else
    {
        for(int i=0; i<n; i++)
        {
            memcpy(&cards[cindex], info.stat_buf[fj_index].c, info.stat_buf[fj_index].n);
            cindex += info.stat_buf[fj_index].n;
            for(int j=0; j<3-info.stat_buf[fj_index].n; j++)
            {
                cards[cindex] = LZ(info.stat_buf[fj_index].v);
                out->lz_card[lzindex].card = info.lzbuf[lzindex];
                out->lz_card[lzindex].change = cards[cindex];
                cindex ++;
                lzindex ++;
            }
            fj_index ++;
        }
    }
    for(int i=0; i<startindex; i++)
    {
        memcpy(&cards[cindex], info.stat_buf[i].c, info.stat_buf[i].n);
        cindex += info.stat_buf[i].n;
    }
    for (int i=startindex+n; i<info.n_stat; i++)
    {
        memcpy(&cards[cindex], info.stat_buf[i].c, info.stat_buf[i].n);
        cindex += info.stat_buf[i].n;
    }
    if(out)
    {
        out->size = lzindex;
    }
    if(info.nlz > lzindex)
    {
        for(int i=lzindex; i<info.nlz; i++)
        {
            cards[cindex] = info.lzbuf[i];
            cindex ++;
        }
    }
    if(cindex != len)
        return 0;
    return C_N(cards[0]);
}

char is_three_straight_with_np(CT_PARAM_DEF)//飞机带对翅膀
{
    VALID_LZ_PARAM;
    if(len < 10 || (len%5) != 0)
        return 0;

    //飞机带对子翅膀时, 不能带王出现
    if(has_king(cards, len))
        return 0;

    int wings = len / 5;  //翅膀个数
    int n_wing = 0;
    int nx = -1;
    int n = len / 5;
    int startindex = -1;
    int is_replace3 = false;
    if(info.n_stat > (wings*2))
        return 0;

    bool find = false;
    for(int search=0; search<info.n_stat; search++)
    {
        find = true;
        //找到飞机残骸起止位置
        for(int i=search; i<info.n_stat; i++)
        {
            is_replace3 = false;
            nx = 0;
            if(info.stat_buf[i].v > CARD_A)
                continue;
            startindex = i;
            n_wing = 1;
            nx += (3-info.stat_buf[i].n);
            if(nx>info.nlz)
            {
                nx = 0;
                continue;
            }
            for(int j=i+1; j<info.n_stat; j++)
            {
                int dx = info.stat_buf[j-1].v - info.stat_buf[j].v;
                nx += (3-info.stat_buf[j].n);
                if(dx > 1)
                {
                    is_replace3 = true;
                    nx += (dx-1)*3;
                    n_wing ++;
                }
                if(nx > info.nlz)
                {
                    n_wing = 0;
                    break;
                }
                n_wing ++;
                if(n_wing == wings)
                {
                    break;
                }
            }
            if(n_wing == wings)
            {
                break;
            }
        }
        if((wings - n_wing) == 1)
        {
            nx += 3;
            if(nx > info.nlz)
                continue;
            n_wing ++;
            is_replace3 = true;
        }
        if(n_wing != wings || startindex < 0) continue;

        int cindex = 0;
        int lzindex = 0;
        int fj_index = startindex;

        //先输出飞机
        if(is_replace3)
        {//需要补三张一样的牌
            char v = info.stat_buf[startindex].v;
            if(v < CARD_A)
            {//补到前面
                for(int i=0; i<3; i++)
                {
                    cards[i] = LZ(v+1);
                    out->lz_card[i].card = info.lzbuf[i];
                    out->lz_card[i].change = cards[i];
                }
                cindex = 3;
                lzindex = 3;
                for(int i=1; i<n; i++)
                {
                    memcpy(&cards[cindex], info.stat_buf[fj_index].c, info.stat_buf[fj_index].n);
                    cindex += info.stat_buf[fj_index].n;
                    for(int j=0; j<3-info.stat_buf[fj_index].n; j++)
                    {
                        cards[cindex] = LZ(info.stat_buf[fj_index].v);
                        out->lz_card[lzindex].card = info.lzbuf[lzindex];
                        out->lz_card[lzindex].change = cards[cindex];
                        cindex ++;
                        lzindex ++;
                    }
                    fj_index ++;
                }
            }
            else
            {//补到后面
                for(int i=0; i<n-1; i++)
                {
                    memcpy(&cards[cindex], info.stat_buf[fj_index].c, info.stat_buf[fj_index].n);
                    cindex += info.stat_buf[fj_index].n;
                    for(int j=0; j<3-info.stat_buf[fj_index].n; j++)
                    {
                        cards[cindex] = LZ(info.stat_buf[fj_index].v);
                        out->lz_card[lzindex].card = info.lzbuf[lzindex];
                        out->lz_card[lzindex].change = cards[cindex];
                        cindex ++;
                        lzindex ++;
                    }
                    v = info.stat_buf[fj_index].v - 1;
                    fj_index ++;
                }
                for(int i=0; i<3; i++)
                {
                    cards[cindex] = LZ(v);
                    out->lz_card[lzindex].card = info.lzbuf[lzindex];
                    out->lz_card[lzindex].change = cards[cindex];
                    cindex ++;
                    lzindex ++;
                }
            }
        }
        else
        {
            for(int i=0; i<n; i++)
            {
                memcpy(&cards[cindex], info.stat_buf[fj_index].c, info.stat_buf[fj_index].n);
                cindex += info.stat_buf[fj_index].n;
                for(int j=0; j<3-info.stat_buf[fj_index].n; j++)
                {
                    cards[cindex] = LZ(info.stat_buf[fj_index].v);
                    out->lz_card[lzindex].card = info.lzbuf[lzindex];
                    out->lz_card[lzindex].change = cards[cindex];
                    cindex ++;
                    lzindex ++;
                }
                fj_index ++;
            }
        }
        for(int i=0; i<startindex; i++)
        {
            if(info.stat_buf[i].n > 2)
            {
                find = false;
                break;
            }
            memcpy(&cards[cindex], info.stat_buf[i].c, info.stat_buf[i].n);
            cindex += info.stat_buf[i].n;
            if(info.stat_buf[i].n < 2)
            {
                cards[cindex] = LZ(info.stat_buf[i].v);
                out->lz_card[lzindex].card = info.lzbuf[lzindex];
                out->lz_card[lzindex].change = cards[cindex];
                cindex ++;
                lzindex ++;
            }
        }
        if(!find)
            continue;

        for (int i=startindex+n; i<info.n_stat; i++)
        {
            if(info.stat_buf[i].n > 2)
            {
                find = false;
                break;
            }
            memcpy(&cards[cindex], info.stat_buf[i].c, info.stat_buf[i].n);
            cindex += info.stat_buf[i].n;
            if(info.stat_buf[i].n < 2)
            {
                cards[cindex] = LZ(info.stat_buf[i].v);
                out->lz_card[lzindex].card = info.lzbuf[lzindex];
                out->lz_card[lzindex].change = cards[cindex];
                cindex ++;
                lzindex ++;
            }
        }
        if(!find)
            continue;
        if(out)
        {
            out->size = lzindex;
        }
        if(info.nlz > lzindex)
        {
            for(int i=lzindex; i<info.nlz; i++)
            {
                cards[cindex] = info.lzbuf[i];
                cindex ++;
            }
        }
        if(cindex != len)
            continue;
        return C_N(cards[0]); 
    }
    return 0;
}

char is_four_with_two(CT_PARAM_DEF)         //四带两单
{
    VALID_LZ_PARAM;
    if(len != 6)
        return 0;
    if(info.n_stat>3)
        return 0;
    int cindex = 0;
    int lzindex = 0;
    for(int i=0; i<info.n_stat; i++)
    {
        if(info.stat_buf[i].v <= CARD_2 && (info.stat_buf[i].n+info.nlz) >= 4)
        {
            memcpy(cards, info.stat_buf[i].c, info.stat_buf[i].n);
            cindex += info.stat_buf[i].n;
            for(int j=info.stat_buf[i].n; j<4; j++)
            {
                cards[cindex] = LZ(info.stat_buf[i].v);
                out->lz_card[lzindex].card = info.lzbuf[lzindex];
                out->lz_card[lzindex].change = cards[cindex];
                cindex ++;
                lzindex ++;
            }
            for(int j=0; j<i; j++)
            {
                memcpy(&cards[cindex], info.stat_buf[j].c, info.stat_buf[j].n);
                cindex += info.stat_buf[j].n;
            }
            for(int j=i+1; j<info.n_stat; j++)
            {
                memcpy(&cards[cindex], info.stat_buf[j].c, info.stat_buf[j].n);
                cindex += info.stat_buf[j].n;
            }
            for(int j=lzindex; j<info.nlz; j++)
            {
                cards[cindex] = info.lzbuf[j];
                cindex ++;
            }
            if(cindex != len)
                return 0;
            if(out)
            {
                out->size = lzindex;
            }
            return C_N(cards[0]);
        }
    }

    return 0;
}

char is_four_with_twopair(CT_PARAM_DEF)     //四带两对
{
    VALID_LZ_PARAM;
    if(len != 8)
        return 0;
    if(has_king(cards, len))
        return 0;

    if(info.n_stat>3)
        return 0;
    for(int i=0; i<info.n_stat; i++)
    {
        int cindex = 0;
        int lzindex = 0;
        if((info.stat_buf[i].n+info.nlz) >= 4)
        {
            memcpy(cards, info.stat_buf[i].c, info.stat_buf[i].n);
            cindex += info.stat_buf[i].n;
            for(int j=info.stat_buf[i].n; j<4; j++)
            {
                cards[cindex] = LZ(info.stat_buf[i].v);
                out->lz_card[lzindex].card = info.lzbuf[lzindex];
                out->lz_card[lzindex].change = cards[cindex];
                cindex ++;
                lzindex ++;
            }
            bool bok = true;
            for(int j=0; j<i; j++)
            {
                if(info.stat_buf[j].n > 2)
                {
                    bok = false;
                    break;
                }
                memcpy(&cards[cindex], info.stat_buf[j].c, info.stat_buf[j].n);
                cindex += info.stat_buf[j].n;
                for(int k=info.stat_buf[j].n; k<2; k++)
                {
                    cards[cindex] = LZ(info.stat_buf[j].v);
                    out->lz_card[lzindex].card = info.lzbuf[lzindex];
                    out->lz_card[lzindex].change = cards[cindex];
                    cindex ++;
                    lzindex ++;
                }
            }
            if(!bok) continue;
            for(int j=i+1; j<info.n_stat; j++)
            {
                if(info.stat_buf[j].n > 2)
                {
                    bok = false;
                    break;
                }
                memcpy(&cards[cindex], info.stat_buf[j].c, info.stat_buf[j].n);
                cindex += info.stat_buf[j].n;
                if(info.stat_buf[j].n < 2)
                {
                    cards[cindex] = LZ(info.stat_buf[j].v);
                    out->lz_card[lzindex].card = info.lzbuf[lzindex];
                    out->lz_card[lzindex].change = cards[cindex];
                    cindex ++;
                    lzindex ++;
                }
            }
            if(!bok) continue;
            for(int j=lzindex; j<info.nlz; j++)
            {
                cards[cindex] = info.lzbuf[j];
                cindex ++;
            }
            if(cindex != len)
                return 0;
            if(out)
            {
                out->size = lzindex;
            }
            return C_N(cards[0]);
        }
    }
    return 0;
}

char is_bomb(CT_PARAM_DEF)                  //炸弹
{
    VALID_LZ_PARAM;
    if(len != 4)
        return 0;
    for(int i=0; i<len; i++)
    {
        if(C_T(cards[i]) == C_TYPE_KING)
            return 0;
    }

    if(info.nlz == 4)
    {
        out->size = 0;
        return lz;
    }
    if(info.n_stat != 1)
        return 0;
    if((info.stat_buf[0].n+info.nlz) != 4)
        return 0;

    memcpy(cards, info.stat_buf[0].c, info.stat_buf[0].n);
    int cindex = info.stat_buf[0].n;
    int lzindex= 0;
    for(int i=info.stat_buf[0].n; i<len; i++)
    {
        cards[i] = LZ(info.stat_buf[0].v);
        out->lz_card[lzindex].card = info.lzbuf[lzindex];
        out->lz_card[lzindex].change = cards[i];
        lzindex ++;
    }
    if(out)
    {
        out->size = info.nlz;
    }

    return C_N(cards[0]);
}

bool is_rocket(char* cards, int len)
{
    if(len != 2)
        return false;
    if((cards[0] == 0x4f && cards[1] == 0x4e) ||
       (cards[0] == 0x4e && cards[1] == 0x4f) )
        return true;
    return false;
}

bool check_cards_type(int type, char* cards, int len, int lz, ddz_lz_item* out)
{
    CardAnalyzInfo info;
    if(!analyze(cards, len, lz, &info))
        return 0;
    switch(type)
    {
    case MDDZ_CT_ONE:                   //单牌
        return (len == 1);
    case MDDZ_CT_PAIR:                  //一对
        return (0 < is_pair(CT_PARAM_USE));
    case MDDZ_CT_THREE:                 //三条
        return (0 < is_three(CT_PARAM_USE));
    case MDDZ_CT_STRAIGHT:              //单顺
        return (0 < is_straight(CT_PARAM_USE));
    case MDDZ_CT_TWO_STRAIGHT:          //双顺
        return (0 < is_two_straight(CT_PARAM_USE));
    case MDDZ_CT_THREE_STRAIGHT:        //三顺
        return (0 < is_three_straight(CT_PARAM_USE));
    case MDDZ_CT_THREE_WITH_ONE:        //三带一单
        return (0 < is_three_with_one(CT_PARAM_USE));
    case MDDZ_CT_THREE_WITH_PAIR:       //三带一对
        return (0 < is_three_with_pair(CT_PARAM_USE));
    case MDDZ_CT_THREE_STRAIGHT_WITH_N: //飞机带单翅膀
        return (0 < is_three_straight_with_n(CT_PARAM_USE));
    case MDDZ_CT_THREE_STRAIGHT_WITH_NP://飞机带对翅膀
        return (0 < is_three_straight_with_np(CT_PARAM_USE));
    case MDDZ_CT_FOUR_WITH_TWO:         //四带两单
        return (0 < is_four_with_two(CT_PARAM_USE));
    case MDDZ_CT_FOUR_WITH_TWOPAIR:     //四带两对
        return (0 < is_four_with_twopair(CT_PARAM_USE));
    case MDDZ_CT_BOMB:                  //炸弹
        return (0 < is_bomb(CT_PARAM_USE));
    case MDDZ_CT_ROCKET:                //火箭
        return is_rocket(cards, len);
    default:
        break;
    }
    return MDDZ_CT_ERROR;
}

int ddz_get_cards_type_list(char* cards, int len, int lz, char maxcard[MAX_CANDIDATE], char outcard[MAX_CANDIDATE][20], int type[MAX_CANDIDATE], ddz_lz_item out[MAX_CANDIDATE])
{
    int count = 0;
    CardAnalyzInfo info;
    if(!analyze(cards, len, lz, &info))
        return 0;
    switch(len)
    {
    case 1://单牌
        memcpy(outcard[0], cards, len);
        type[0] = MDDZ_CT_ONE;
        out[0].size = 0;
        count = 1;
        break;
    case 2://对子
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_pair(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_PAIR;
                count ++;
            }
            if(is_rocket(outcard[count], len))
            {
                maxcard[count] = 0x0d;
                type[count] = MDDZ_CT_ROCKET;
                count ++;
            }
        }
        break;
    case 3://三条
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE;
                count ++;
            }
        }
        break;
    case 4://炸弹,三带一
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three_with_one(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE_WITH_ONE;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_bomb(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_BOMB;
                count ++;
            }
        }
        break;
    case 5://顺子, 三带对
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three_with_pair(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE_WITH_PAIR;
                count ++;
            }
        }
        break;
    case 6://顺子, 双顺, 三顺, 四带二
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_two_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_TWO_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_four_with_two(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_FOUR_WITH_TWO;
                count ++;
            }
        }
        break;
    case 7://顺子
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_STRAIGHT;
                count ++;
            }
        }
        break;
    case 8://顺子, 双顺, 三顺带单, 四带两对
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_two_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_TWO_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three_straight_with_n(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE_STRAIGHT_WITH_N;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_four_with_twopair(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_FOUR_WITH_TWOPAIR;
                count ++;
            }
        }
        break;
    case 9://顺子,三顺
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE_STRAIGHT;
                count ++;
            }
        }
        break;
    case 10://顺子, 双顺
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_two_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_TWO_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three_straight_with_np(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE_STRAIGHT_WITH_NP;
                count ++;
            }
        }
        break;
    case 11://顺子
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_STRAIGHT;
                count ++;
            }
        }
        break;
    case 12://顺子, 双顺, 三顺, 三顺带三
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_two_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_TWO_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three_straight_with_n(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE_STRAIGHT_WITH_N;
                count ++;
            }
        }
        break;
    case 13://无牌型
        break;
    case 14://双顺
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_two_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_TWO_STRAIGHT;
                count ++;
            }
        }
        break;
    case 15://三顺,三顺带对
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three_straight_with_np(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE_STRAIGHT_WITH_NP;
                count ++;
            }
        }
        break;
    case 16://双顺, 三顺带单
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_two_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_TWO_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three_straight_with_n(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE_STRAIGHT_WITH_N;
                count ++;
            }
        }
        break;
    case 17:
        break;
    case 18://双顺,三顺
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_two_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_TWO_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE_STRAIGHT;
                count ++;
            }
        }
        break;
    case 19:
        break;
    case 20://双顺, 三顺带单, 三顺带对
        {
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_two_straight(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_TWO_STRAIGHT;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three_straight_with_n(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE_STRAIGHT_WITH_N;
                count ++;
            }
            memcpy(outcard[count], cards, len);
            if(maxcard[count] = is_three_straight_with_np(outcard[count], len, lz, &out[count], info))
            {
                type[count] = MDDZ_CT_THREE_STRAIGHT_WITH_NP;
                count ++;
            }
        }
        break;
    default:
        break;
    }
    return count;
}

bool ddz_compare(char* buf1, int len1, int t1, char* buf2, int len2, int t2, int lz)
{
    if(len2 == 2)
    {
        if(is_rocket(buf2, len2))
            return true;
    }
    if(len1 == 2)
    {
        if(is_rocket(buf1, len1))
            return false;
    }

    ddz_lz_item item1, item2;
    get_lz_size(buf1, len1, lz, &item1);
    get_lz_size(buf2, len2, lz, &item2);

    char cards1[20];
    char cards2[20];

    memcpy(cards1, buf1, len1);
    memcpy(cards2, buf2, len2);

    CardAnalyzInfo info1;
    CardAnalyzInfo info2;
    memset(&info1, 0, sizeof(info1));
    memset(&info2, 0, sizeof(info2));

    analyze(cards1, len1, lz, &info1);
    analyze(cards2, len2, lz, &info2);

    char n1 = 0, n2 = 0;

    if(len1 == len2 && t1 == t2)
    {
        switch(t1)
        {
        case MDDZ_CT_ONE:                   //单牌
            {
                n1 = C_N(buf1[0]);
                n2 = C_N(buf2[0]);
            }
            break;
        case MDDZ_CT_PAIR:                  //一对
            {
                n1 = is_pair(cards1, len1, lz, &item1, info1);
                n2 = is_pair(cards2, len2, lz, &item2, info2);
            }
            break;
        case MDDZ_CT_THREE:                 //三条
            {
                n1 = is_three(cards1, len1, lz, &item1, info1);
                n2 = is_three(cards2, len2, lz, &item2, info2);
            }
            break;
        case MDDZ_CT_STRAIGHT:              //单顺
            {
                if(len1 != len2)
                    break;
                n1 = is_straight(cards1, len1, lz, &item1, info1);
                n2 = is_straight(cards2, len2, lz, &item2, info2);
            }
            break;
        case MDDZ_CT_TWO_STRAIGHT:          //双顺
            {
                if(len1 != len2)
                    break;
                n1 = is_two_straight(cards1, len1, lz, &item1, info1);
                n2 = is_two_straight(cards2, len2, lz, &item2, info2);
            }
            break;
        case MDDZ_CT_THREE_STRAIGHT:        //三顺
            {
                if(len1 != len2)
                    break;
                n1 = is_three_straight(cards1, len1, lz, &item1, info1);
                n2 = is_three_straight(cards2, len2, lz, &item2, info2);
            }
            break;
        case MDDZ_CT_THREE_WITH_ONE:        //三带一单
            {
                n1 = is_three_with_one(cards1, len1, lz, &item1, info1);
                n2 = is_three_with_one(cards2, len2, lz, &item2, info2);
            }
            break;
        case MDDZ_CT_THREE_WITH_PAIR:       //三带一对
            {
                n1 = is_three_with_pair(cards1, len1, lz, &item1, info1);
                n2 = is_three_with_pair(cards2, len2, lz, &item2, info2);
            }
            break;
        case MDDZ_CT_THREE_STRAIGHT_WITH_N: //飞机带单翅膀
            {
                if(len1 != len2)
                    break;
                n1 = is_three_straight_with_n(cards1, len1, lz, &item1, info1);
                n2 = is_three_straight_with_n(cards2, len2, lz, &item2, info2);
            }
            break;
        case MDDZ_CT_THREE_STRAIGHT_WITH_NP://飞机带对翅膀
            {
                if(len1 != len2)
                    break;
                n1 = is_three_straight_with_np(cards1, len1, lz, &item1, info1);
                n2 = is_three_straight_with_np(cards2, len2, lz, &item2, info2);
            }
            break;
        case MDDZ_CT_FOUR_WITH_TWO:         //四带两单
            {
                n1 = is_four_with_two(cards1, len1, lz, &item1, info1);
                n2 = is_four_with_two(cards2, len2, lz, &item2, info2);
            }
            break;
        case MDDZ_CT_FOUR_WITH_TWOPAIR:     //四带两对
            {
                n1 = is_four_with_twopair(cards1, len1, lz, &item1, info1);
                n2 = is_four_with_twopair(cards2, len2, lz, &item2, info2);
            }
            break;
        case MDDZ_CT_BOMB:                  //炸弹
            {
                n1 = C_N(is_bomb(cards1, len1, lz, &item1, info1));
                n2 = C_N(is_bomb(cards2, len2, lz, &item2, info2));
                
                //判断纯癞子炸弹
                if(n1 == lz)
                    return false;
                if(n2 == lz)
                    return true;

                //判断软炸弹
                bool s1=false,s2=false;
                if(item1.size>0)s1 = true;
                else{
                    for(int i=0; i<len1; i++)
                    {
                        if(C_T(cards1[i]) == C_TYPE_LZ)
                        {
                            s1 = true;
                            break;
                        }
                    }
                }
                if(item2.size > 0) s2 = true;
                else {
                    for(int i=0; i<len2; i++)
                    {
                        if(C_T(cards2[i]) == C_TYPE_LZ)
                        {
                            s2 = true;
                            break;
                        }
                    }
                }
                //都是软炸弹或都是硬炸弹，比较转换大小
                if((s1 && s2) || ((!s1) && (!s2)))
                    return (n2 > n1);
                //硬炸弹大于任何软炸弹
                if(s1 && (!s2))
                    return (n2 > 0);
                //软炸弹小于任何硬炸弹
                if(!s1 && s2)
                    return false;
            }
            break;
        case MDDZ_CT_ROCKET:                //火箭
            return false;
            break;
        default:
            return false;
        }
    }
    if(n2 > 0 && (C_N(n2) > C_N(n1)))
    {
        return true;
    }

//	return (len2==4 && is_bomb(cards2, len2, lz, &item2) > 0);
	if(t2 == MDDZ_CT_BOMB && t1 != MDDZ_CT_BOMB)
		return true;
	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CHECK_CANDIDATE_SIZE() count++; if(count == MAX_CANDIDATE) return count;
#define GET_ROCKET()\
    if(info.normal[0] == 0x4f && info.normal[1] == 0x4e)\
    {\
        ret[count][0] = 0x4f;\
        ret[count][1] = 0x4e;\
        ot[count] = MDDZ_CT_ROCKET;\
        blen[count] = 2;\
        CHECK_CANDIDATE_SIZE();\
    }

#define GET_BOMB()\
    for(int i=info.n_stat-1; i>=0; i--)\
    {\
        if((info.stat_buf[i].n+info.nlz)>=4 && C_T(info.stat_buf[i].c[0]) != C_TYPE_KING)\
        {\
            memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);\
            for(int j=info.stat_buf[i].n; j<4; j++)\
            {\
                ret[count][j] = LZ(info.stat_buf[i].v);\
                int lzindex = j-info.stat_buf[i].n;\
                lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];\
                lzout[count].lz_card[lzindex].change = ret[count][j];\
            }\
            lzout[count].size = 4 - info.stat_buf[i].n;\
            ot[count] = MDDZ_CT_BOMB;\
            blen[count] = 4;\
            CHECK_CANDIDATE_SIZE();\
        }\
    }\
    if(info.nlz == 4)\
    {\
        memcpy(ret[count], info.lzbuf, 4);\
        lzout[count].size = 0;\
        ot[count] = MDDZ_CT_BOMB;\
        blen[count] = 4;\
        CHECK_CANDIDATE_SIZE();\
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int get_big_first(char* cards, int len, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE], int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE], CardAnalyzInfo& info)
{
    if(!cards || len<0 || len>20)
        return 0;
    int count = 0;
    memset(lzout, 0, sizeof(lzout));
    //判断是否能够一手出完
    do
    {
        char maxcard[MAX_CANDIDATE];
        char outcard[MAX_CANDIDATE][20];
        int type[MAX_CANDIDATE];
        ddz_lz_item out[MAX_CANDIDATE];

        memset(maxcard, 0, sizeof(maxcard));
        memset(outcard, 0, sizeof(outcard));
        memset(type, 0, sizeof(type));
        memset(out, 0, sizeof(out));

        int n = ddz_get_cards_type_list(cards, len, lz, maxcard, outcard, type, out);

        /*
        copym(buf[i].cards, outcard[i], src.size);
        buf[i].card_type = type[i];
        buf[i].clz = out[i];
        buf[i].size = src.size;
        
        */

        for(int i=0; i<n; i++)
        {
            if(type[i] != MDDZ_CT_FOUR_WITH_TWO && type[i] != MDDZ_CT_FOUR_WITH_TWOPAIR)
            {
                memcpy(ret[count], outcard[i], len);
                ot[count] = type[i];
                blen[count] = len;
                lzout[count] = out[i];

                CHECK_CANDIDATE_SIZE();
            }
        }
    }while(false);

    //推荐单牌
    for(int i=info.n_stat-1; i>=0; i--)
    {
        if(info.stat_buf[i].n == 1)
        {
            ret[count][0] = info.stat_buf[i].c[0];
            ot[count] = MDDZ_CT_ONE;
            blen[count] = 1;
            CHECK_CANDIDATE_SIZE();
        }
    }
    //推荐2牌
    for(int i=info.n_stat-1; i>=0; i--)
    {
        if(info.stat_buf[i].n == 2)
        {
            ret[count][0] = info.stat_buf[i].c[0];
            ret[count][1] = info.stat_buf[i].c[1];
            ot[count] = MDDZ_CT_PAIR;
            blen[count] = 2;
            CHECK_CANDIDATE_SIZE();
        }
    }
    //推荐3牌
    for(int i=info.n_stat-1; i>=0; i--)
    {
        if(info.stat_buf[i].n == 3)
        {
            ret[count][0] = info.stat_buf[i].c[0];
            ret[count][1] = info.stat_buf[i].c[1];
            ret[count][2] = info.stat_buf[i].c[2];
            ot[count] = MDDZ_CT_THREE;
            blen[count] = 3;
            CHECK_CANDIDATE_SIZE();
        }
    }

    //推荐炸弹
    GET_BOMB();
    //推荐火箭
    GET_ROCKET();
    return count;
}

int get_bigger_one(char v, char* cards, int len, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE], int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE], CardAnalyzInfo& info)
{
    int count = 0;
    memset(lzout, 0, sizeof(lzout));

    for(int i=0; i<MAX_CANDIDATE; i++)
        lzout[i].size = 0;
    if(info.n_stat == 0)
    {
        if(info.nlz == 0)
            return 0;
        if(lz > v)
        {
            ret[0][0] = info.lzbuf[0];
            ot[0] = MDDZ_CT_ONE;
            blen[0] = 1;
            count = 1;
        }
        goto bomb;
    }
    //推荐单牌
    for(int i=info.n_stat-1; i>=0; i--)
    {
        if(info.stat_buf[i].n == 1 && info.stat_buf[i].v > v)
        {
            ret[count][0] = info.stat_buf[i].c[0];
            ot[count] = MDDZ_CT_ONE;
            blen[count] = 1;
            CHECK_CANDIDATE_SIZE();
        }
    }
    //推荐2-3牌
    for(int i=info.n_stat-1; i>=0; i--)
    {
        if(info.stat_buf[i].n > 1 && info.stat_buf[i].n < 4 && info.stat_buf[i].v > v)
        {
            ret[count][0] = info.stat_buf[i].c[0];
            ot[count] = MDDZ_CT_ONE;
            blen[count] = 1;
            CHECK_CANDIDATE_SIZE();
        }
    }

    if(info.nlz>0 && lz > v)
    {
        ret[count][0] = info.lzbuf[0];
        ot[count] = MDDZ_CT_ONE;
        blen[count] = 1;
        CHECK_CANDIDATE_SIZE();
    }

bomb:
    //推荐炸弹
    GET_BOMB();
    //推荐火箭
    GET_ROCKET();
    //拆炸弹
    for(int i=info.n_stat-1; i>=0; i--)
    {
        if(info.stat_buf[i].n == 4 && info.stat_buf[i].v > v)
        {
            ret[count][0] = info.stat_buf[i].c[0];
            ot[count] = MDDZ_CT_ONE;
            blen[count] = 1;
            CHECK_CANDIDATE_SIZE();
        }
    }
    return count;
}

int get_bigger_pair(char v, char* cards, int len, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE], int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE], CardAnalyzInfo& info)
{
    int count = 0;
    memset(lzout, 0, sizeof(lzout));

    //推荐对子
    for(int i=info.n_stat-1; i>=0; i--)
    {
        if(info.stat_buf[i].n==2 && info.stat_buf[i].v > v)
        {
            ot[count] = MDDZ_CT_PAIR;
            memcpy(ret[count], info.stat_buf[i].c, 2);
            blen[count] = 2;
            CHECK_CANDIDATE_SIZE();
        }
    }
    //拆3条
    for(int i=info.n_stat-1; i>=0; i--)
    {
        if(info.stat_buf[i].n == 3 && info.stat_buf[i].v > v)
        {
            ot[count] = MDDZ_CT_PAIR;
            blen[count] = 2;
            memcpy(ret[count], info.stat_buf[i].c, 2);
            CHECK_CANDIDATE_SIZE();
        }
    }
    //凑单
    for(int i=info.n_stat-1; i>=0; i--)
    {
        if(info.nlz == 0) break;
        if(info.stat_buf[i].v <= CARD_2 && info.stat_buf[i].v > v && info.stat_buf[i].n == 1)
        {
            ot[count] = MDDZ_CT_PAIR;
            blen[count] = 2;
            ret[count][0] = info.stat_buf[i].c[0];
            ret[count][1] = LZ(info.stat_buf[i].v);
            lzout[count].lz_card[0].card = info.lzbuf[0];
            lzout[count].lz_card[0].change = ret[count][1];
            lzout[count].size = 1;
            CHECK_CANDIDATE_SIZE();
        }
    }
    //推荐炸弹
    GET_BOMB();
    //推荐火箭
    GET_ROCKET();
    //纯癞子
    if((info.nlz>=2) && (lz > v))
    {
        ot[count] = MDDZ_CT_PAIR;
        blen[count] = 2;
        memcpy(ret[count], info.lzbuf, 2);
        CHECK_CANDIDATE_SIZE();
    }
    return count;
}

int get_bigger_three(char v, char* cards, int len, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE], int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE], CardAnalyzInfo& info)
{
    int count = 0;
    memset(lzout, 0, sizeof(lzout));

    //推荐三条
    for(int i=info.n_stat-1; i>=0; i--)
    {
        //真三条
        if(info.stat_buf[i].n == 3 && info.stat_buf[i].v > v)
        {
            ot[count] = MDDZ_CT_THREE;
            blen[count] = 3;
            memcpy(ret[count], info.stat_buf[i].c, 3);
            CHECK_CANDIDATE_SIZE();
        }//癞子三条
        else if(info.stat_buf[i].v <= CARD_2 && info.stat_buf[i].v > v && (info.stat_buf[i].n < 3 && (info.stat_buf[i].n+info.nlz) > 2))
        {
            ot[count] = MDDZ_CT_THREE;
            blen[count] = 3;
            memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);
            for(int j=info.stat_buf[i].n; j<3; j++)
            {
                ret[count][j] = LZ(info.stat_buf[i].v);
                int lzindex = j-info.stat_buf[i].n;
                lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                lzout[count].lz_card[lzindex].change = ret[count][j];
            }
            lzout[count].size = 3 - info.stat_buf[i].n;
            CHECK_CANDIDATE_SIZE();
        }
    }

    //推荐炸弹
    GET_BOMB();
    //推荐火箭
    GET_ROCKET();
    //纯癞子
    if((info.nlz>=3) && (lz > v))
    {
        ot[count] = MDDZ_CT_THREE;
        blen[count] = 3;
        memcpy(ret[count], info.lzbuf, 3);
        CHECK_CANDIDATE_SIZE();
    }
    return count;
}

int get_bigger_straight(char v, int clen, char* cards, int len, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE], int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE], CardAnalyzInfo& info)
{
    int count = 0;

    memset(lzout, 0, sizeof(lzout));
    
    if(v < CARD_A && (info.n_stat+info.nlz) >= clen)
    {
        //寻找普通顺子牌
        for(int i=info.n_stat-1; i>=0; i--)
        {
            if(((info.n_stat-i) >= clen) &&
                (info.stat_buf[i].v < CARD_2) &&
                (info.stat_buf[i].v > v) &&
                (info.stat_buf[i+clen-1].v == (info.stat_buf[i].v-clen+1)))
            {
                for(int j=i; j<i+clen; j++)
                {
                    ret[count][j-i] = info.stat_buf[j].c[0];
                }
                ot[count] = MDDZ_CT_STRAIGHT;
                blen[count] = clen;
                CHECK_CANDIDATE_SIZE();
            }
        }
        //寻找癞子替换顺子
        if(info.nlz > 0)
        {
            int vv = v;
            for(int v=CARD_A-1; v>=vv; v--)
            for(int i=info.n_stat-1; i>=0; i--)
            {
                char expv = 0;
                int  lzindex = 0;
                int  cindex = 0;
                if(info.stat_buf[i].v > CARD_A)
                    continue;
                if(info.stat_buf[i].v > v)
                {
                    expv = info.stat_buf[i].v;
                }
                else
                {
                    expv = v+1;
                }
                if(expv > CARD_A)
                    continue;
                bool has_exist = false;
                for(int j=0; j<count; j++)
                {
                    if(expv == C_N(ret[j][0]))
                    {
                        has_exist = true;
                        break;
                    }
                }
                if(has_exist)
                    continue;

                for(int j=i; j<info.n_stat;)
                {
                    if(expv == info.stat_buf[j].v)
                    {
                        ret[count][cindex] = info.stat_buf[j].c[0];
                        j++;
                    }
                    else
                    {
                        if(lzindex >= info.nlz)
                            break;
                        ret[count][cindex] = LZ(expv);
                        lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                        lzout[count].lz_card[lzindex].change = ret[count][cindex];
                        lzindex ++;
                    }
                    expv--;
                    cindex ++;
                    if(cindex == clen)
                    {
                        lzout[count].size = lzindex;
                        ot[count] = MDDZ_CT_STRAIGHT;
                        blen[count] = clen;
                        CHECK_CANDIDATE_SIZE();
                        break;
                    }
                }
                if(cindex < clen && (cindex+(info.nlz-lzindex)) >= clen && (expv-(clen-cindex)) > 0)
                {
                    for(int j=cindex; j<clen; j++)
                    {
                        ret[count][j] = LZ(expv); expv--;
                        lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                        lzout[count].lz_card[lzindex].change = ret[count][cindex];
                        lzindex ++;
                    }
                    lzout[count].size = lzindex;
                    ot[count] = MDDZ_CT_STRAIGHT;
                    blen[count] = clen;
                    CHECK_CANDIDATE_SIZE();
                }
            }
        }
    }

    //推荐炸弹
    GET_BOMB();
    //推荐火箭
    GET_ROCKET();
    return count;
}

int get_bigger_two_straight(char v, int clen, char* cards, int len, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE], int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE], CardAnalyzInfo& info)
{
    int count = 0;
    memset(lzout, 0, sizeof(lzout));

    int sn = clen / 2;
    if((v < CARD_A) && (len >= clen))
    {
        //查找普通双顺
        for(int i=info.n_stat-1; i>=0; i--)
        {
            if((info.n_stat-i) >= sn &&
                info.stat_buf[i].v < CARD_2 &&
                info.stat_buf[i].v > v &&
                (info.stat_buf[i+sn-1].v == (info.stat_buf[i].v-sn+1)))
            {
                bool check_ok = true;
                for(int j=i; j<i+sn; j++)
                {
                    if(info.stat_buf[j].n < 2)
                    {
                        check_ok = false;
                        break;
                    }
                }
                if(check_ok)
                {
                    int cindex = 0;
                    for(int j=i; j<i+sn; j++)
                    {
                        memcpy(&ret[count][cindex], info.stat_buf[j].c, 2);
                        cindex += 2;
                    }
                    ot[count] = MDDZ_CT_TWO_STRAIGHT;
                    blen[count] = cindex;
                    CHECK_CANDIDATE_SIZE();
                }
            }
        }
        
        //寻找癞子替换顺子
        if(info.nlz > 0)
        {
            int vv = v;
            for(int v=CARD_A-1; v>=vv; v--)
            for(int i=info.n_stat-1; i>=0; i--)
            {
                char expv = 0;
                int  lzindex = 0;
                int  cindex = 0;
                if(info.stat_buf[i].v > v)
                {
                    expv = info.stat_buf[i].v;
                }
                else
                {
                    expv = v+1;
                }
                if(expv > CARD_A)
                    continue;
                bool has_exist = false;
                for(int j=0; j<count; j++)
                {
                    if(expv == C_N(ret[j][0]))
                    {
                        has_exist = true;
                        break;
                    }
                }
                if(has_exist)
                    continue;

                for(int j=i; j<info.n_stat;)
                {
                    if(expv == info.stat_buf[j].v)
                    {
                        int copy_n = info.stat_buf[j].n;
                        if(copy_n>2)
                            copy_n = 2;
                        memcpy(&ret[count][cindex], info.stat_buf[j].c, copy_n);
                        cindex += copy_n;
                        if(copy_n<2)
                        {
                            if(lzindex >= info.nlz)
                                break;
                            ret[count][cindex] = LZ(expv);
                            lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                            lzout[count].lz_card[lzindex].change = ret[count][cindex];
                            lzindex ++;
                            cindex ++;
                        }
                        j++;
                    }
                    else
                    {
                        if((lzindex+1) >= info.nlz)
                            break;
                        for(int k=0; k<2; k++)
                        {
                            ret[count][cindex] = LZ(expv);
                            lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                            lzout[count].lz_card[lzindex].change = ret[count][cindex];
                            lzindex ++;
                            cindex ++;
                        }
                    }
                    expv--;

                    if(cindex == clen)
                    {
                        lzout[count].size = lzindex;
                        ot[count] = MDDZ_CT_TWO_STRAIGHT;
                        blen[count] = clen;
                        CHECK_CANDIDATE_SIZE();
                        break;
                    }
                }
                if(cindex<clen && (clen-cindex)<=(info.nlz-lzindex) && (clen-cindex) % 2 == 0)
                {
                    int n = (clen-cindex) / 2;
                    for(int j=0; j<n; j++)
                    {
                        ret[count][cindex] = LZ(expv); 
                        lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                        lzout[count].lz_card[lzindex].change = ret[count][cindex];
                        cindex++;
                        lzindex ++;
                        ret[count][cindex] = LZ(expv); 
                        lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                        lzout[count].lz_card[lzindex].change = ret[count][cindex];
                        cindex++;
                        lzindex ++;
						expv --;
                    }
                    lzout[count].size = lzindex;
                    ot[count] = MDDZ_CT_TWO_STRAIGHT;
                    blen[count] = clen;
                    CHECK_CANDIDATE_SIZE();
                }
            }
        }
    }
    //推荐炸弹
    GET_BOMB();
    //推荐火箭
    GET_ROCKET();
    return count;
}

int get_bigger_three_straight(char v, int clen, char* cards, int len, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE], int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE], CardAnalyzInfo& info)
{
    int count = 0;
    memset(lzout, 0, sizeof(lzout));

    int sn = clen / 3;
    if(v < CARD_A && len >= clen)
    {
        for(int i=info.n_stat-1; i>=0; i--)
        {
            if((info.n_stat-i) >= sn &&
                info.stat_buf[i].v < CARD_2 &&
                info.stat_buf[i].v > v &&
                (info.stat_buf[i+sn-1].v == (info.stat_buf[i].v-sn+1)))
            {
                bool check_ok = true;
                for(int j=i; j<i+sn; j++)
                {
                    if(info.stat_buf[j].n < 3)
                    {
                        check_ok = false;
                        break;
                    }
                }
                if(check_ok)
                {
                    int cindex = 0;
                    for(int j=i; j<i+sn; j++)
                    {
                        memcpy(&ret[count][cindex], info.stat_buf[j].c, 3);
                        cindex += 3;
                    }
                    ot[count] = MDDZ_CT_THREE_STRAIGHT;
                    blen[count] = cindex;
                    CHECK_CANDIDATE_SIZE();
                }
            }
        }

        //寻找癞子替换顺子
        if(info.nlz > 0)
        { 
            int vv = v;
            for(int v=CARD_A-1; v>=vv; v--)
            for(int i=info.n_stat-1; i>=0; i--)
            {
                char expv = 0;
                int  lzindex = 0;
                int  cindex = 0;
                if(info.stat_buf[i].v > v)
                {
                    expv = info.stat_buf[i].v;
                }
                else
                {
                    expv = v+1;
                }
                if(expv > CARD_A)
                    continue;
                bool has_exist = false;
                for(int j=0; j<count; j++)
                {
                    if(expv == C_N(ret[j][0]))
                    {
                        has_exist = true;
                        break;
                    }
                }
                if(has_exist)
                    continue;

                for(int j=i; j<info.n_stat;)
                {
                    if(expv == info.stat_buf[j].v)
                    {
                        int copy_n = info.stat_buf[j].n;
                        if(copy_n>3)
                            copy_n = 3;
                        memcpy(&ret[count][cindex], info.stat_buf[j].c, copy_n);
                        cindex += copy_n;
                        if(copy_n<3)
                        {
                            if((lzindex + (2-copy_n)) >= info.nlz)
                                break;
                            for(int k=0; k<(3-copy_n); k++)
                            {
                                ret[count][cindex] = LZ(expv);
                                lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                                lzout[count].lz_card[lzindex].change = ret[count][cindex];
                                lzindex ++;
                                cindex ++;
                            }
                        }
                        j++;
                    }
                    else
                    {
                        if((lzindex+2) >= info.nlz)
                            break;
                        for(int k=0; k<3; k++)
                        {
                            ret[count][cindex] = LZ(expv);
                            lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                            lzout[count].lz_card[lzindex].change = ret[count][cindex];
                            lzindex ++;
                            cindex ++;
                        }
                    }
                    expv--;

                    if(cindex == clen)
                    {
                        lzout[count].size = lzindex;
                        ot[count] = MDDZ_CT_THREE_STRAIGHT;
                        blen[count] = clen;
                        CHECK_CANDIDATE_SIZE();
                        break;
                    }
                }
                if((clen-cindex)==3 && (info.nlz-lzindex)>=3)
                {
                    for(;cindex<clen; cindex++)
                    {
                        ret[count][cindex] = LZ(expv);
                        lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                        lzout[count].lz_card[lzindex].change = ret[count][cindex];
                        lzindex ++;
                    }
                    lzout[count].size = lzindex;
                    ot[count] = MDDZ_CT_THREE_STRAIGHT;
                    blen[count] = clen;
                    CHECK_CANDIDATE_SIZE();
                }
            }
        }
    }

    //推荐炸弹
    GET_BOMB();
    //推荐火箭
    GET_ROCKET();
    return count;
}

int get_bigger_three_with_one(char v, char* cards, int len, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE], int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE], CardAnalyzInfo& info)
{
    int count = 0;
    memset(lzout, 0, sizeof(lzout));

    for(int i=info.n_stat-1; i>=0; i--)
    {
        if(info.stat_buf[i].n == 3 && info.stat_buf[i].v > v)
        {
            for(int j=info.n_stat-1; j>=0; j--)
            {
                if(i == j)
                    continue;
                if(info.stat_buf[j].n == 1)
                {
                    memcpy(ret[count], info.stat_buf[i].c, 3);
                    ot[count] = MDDZ_CT_THREE_WITH_ONE;
                    blen[count] = 4;
                    ret[count][3] = info.stat_buf[j].c[0];
                    CHECK_CANDIDATE_SIZE();
                }
            }
            for(int j=info.n_stat-1; j>=0; j--)
            {
                if(i == j)
                    continue;
                if(info.stat_buf[j].n > 1)
                {
                    memcpy(ret[count], info.stat_buf[i].c, 3);
                    ot[count] = MDDZ_CT_THREE_WITH_ONE;
                    blen[count] = 4;
                    ret[count][3] = info.stat_buf[j].c[0];
                    CHECK_CANDIDATE_SIZE();
                }
            }
        }
    }
    for(int i=info.n_stat-1; i>=0; i--)
    {
        if(info.stat_buf[i].v <= CARD_2 && info.stat_buf[i].v > v && info.stat_buf[i].n < 3 && (info.stat_buf[i].n+info.nlz) >=3)
        {
            for(int j=info.n_stat-1; j>=0; j--)
            {
                if(i == j)
                    continue;
                if(info.stat_buf[j].n == 1)
                {
                    memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);
                    for(int k=info.stat_buf[i].n; k<3; k++)
                    {
                        int lzindex = k - info.stat_buf[i].n;
                        ret[count][k] = LZ(info.stat_buf[i].v);
                        lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                        lzout[count].lz_card[lzindex].change = ret[count][k];
                    }
                    lzout[count].size = 3 - info.stat_buf[i].n;
                    ot[count] = MDDZ_CT_THREE_WITH_ONE;
                    blen[count] = 4;
                    ret[count][3] = info.stat_buf[j].c[0];
                    CHECK_CANDIDATE_SIZE();
                }
            }
            for(int j=info.n_stat-1; j>=0; j--)
            {
                if(i == j)
                    continue;
                if(info.stat_buf[j].n > 1 && info.stat_buf[j].n < 4)
                {
                    memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);
                    for(int k=info.stat_buf[i].n; k<3; k++)
                    {
                        int lzindex = k - info.stat_buf[i].n;
                        ret[count][k] = LZ(info.stat_buf[i].v);
                        lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                        lzout[count].lz_card[lzindex].change = ret[count][k];
                    }
                    lzout[count].size = 3 - info.stat_buf[i].n;
                    ot[count] = MDDZ_CT_THREE_WITH_ONE;
                    blen[count] = 4;
                    ret[count][3] = info.stat_buf[j].c[0];
                    CHECK_CANDIDATE_SIZE();
                }
            }
        }
    }

    //推荐炸弹
    GET_BOMB();
    //推荐火箭
    GET_ROCKET();
    return count;
}

int get_bigger_three_with_pair(char v, char* cards, int len, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE], int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE], CardAnalyzInfo& info)
{
    int count = 0;
    memset(lzout, 0, sizeof(lzout));

    for(int i=info.n_stat-1; i>=0; i--)
    {
        if(info.stat_buf[i].n == 3 && info.stat_buf[i].v > v)
        {
            for(int j=info.n_stat-1; j>=0; j--)
            {
                if(i == j)
                    continue;
                if(info.stat_buf[j].n == 2)
                {
                    memcpy(ret[count], info.stat_buf[i].c, 3);
                    ot[count] = MDDZ_CT_THREE_WITH_PAIR;
                    blen[count] = 5;
                    ret[count][3] = info.stat_buf[j].c[0];
                    ret[count][4] = info.stat_buf[j].c[1];
                    CHECK_CANDIDATE_SIZE();
                }
            }
            for(int j=info.n_stat-1; j>=0; j--)
            {
                if(i == j)
                    continue;
                if(info.stat_buf[j].n > 2)
                {
                    memcpy(ret[count], info.stat_buf[i].c, 3);
                    ot[count] = MDDZ_CT_THREE_WITH_PAIR;
                    blen[count] = 5;
                    ret[count][3] = info.stat_buf[j].c[0];
                    ret[count][4] = info.stat_buf[j].c[1];
                    CHECK_CANDIDATE_SIZE();
                }
            }
        }
    }
    for(int i=info.n_stat-1; i>=0; i--)
    {
        if(info.stat_buf[i].v <= CARD_2 && info.stat_buf[i].v > v && info.stat_buf[i].n < 3 && (info.stat_buf[i].n+info.nlz) >=3)
        {
            for(int j=info.n_stat-1; j>=0; j--)
            {
                if(i == j)
                    continue;
                if(info.stat_buf[j].n == 2)
                {
                    memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);
                    for(int k=info.stat_buf[i].n; k<3; k++)
                    {
                        int lzindex = k - info.stat_buf[i].n;
                        ret[count][k] = LZ(info.stat_buf[i].v);
                        lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                        lzout[count].lz_card[lzindex].change = ret[count][k];
                    }
                    lzout[count].size = 3 - info.stat_buf[i].n;
                    ot[count] = MDDZ_CT_THREE_WITH_PAIR;
                    blen[count] = 5;
                    ret[count][3] = info.stat_buf[j].c[0];
                    ret[count][4] = info.stat_buf[j].c[1];
                    CHECK_CANDIDATE_SIZE();
                }
            }
            for(int j=info.n_stat-1; j>=0; j--)
            {
                if(i == j)
                    continue;
                if(info.stat_buf[j].n > 2)
                {
                    memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);
                    for(int k=info.stat_buf[i].n; k<3; k++)
                    {
                        int lzindex = k - info.stat_buf[i].n;
                        ret[count][k] = LZ(info.stat_buf[i].v);
                        lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                        lzout[count].lz_card[lzindex].change = ret[count][k];
                    }
                    lzout[count].size = 3 - info.stat_buf[i].n;
                    ot[count] = MDDZ_CT_THREE_WITH_PAIR;
                    blen[count] = 5;
                    ret[count][3] = info.stat_buf[j].c[0];
                    ret[count][4] = info.stat_buf[j].c[1];
                    CHECK_CANDIDATE_SIZE();
                }
            }
            if((info.stat_buf[i].n+info.nlz) >= 4)
            {
                for(int j=info.n_stat-1; j>=0; j--)
                {
                    if(i == j)
                        continue;
                    if(info.stat_buf[j].n == 1 && info.stat_buf[j].v <= CARD_2)
                    {
                        int lzindex = 0;
                        memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);
                        for(int k=info.stat_buf[i].n; k<3; k++)
                        {
                            lzindex = k - info.stat_buf[i].n;
                            ret[count][k] = LZ(info.stat_buf[i].v);
                            lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                            lzout[count].lz_card[lzindex].change = ret[count][k];
                        }
                        ot[count] = MDDZ_CT_THREE_WITH_PAIR;
                        blen[count] = 5;
                        ret[count][3] = info.stat_buf[j].c[0];
                        ret[count][4] = LZ(info.stat_buf[j].v);
                        lzindex = 3-info.stat_buf[i].n;
                        lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                        lzout[count].lz_card[lzindex].change = ret[count][4];

                        lzout[count].size = 4 - info.stat_buf[i].n;
                        CHECK_CANDIDATE_SIZE();
                    }
                }
            }
        }
    }

    //推荐炸弹
    GET_BOMB();
    //推荐火箭
    GET_ROCKET();
    return count;
}

int get_bigger_three_straight_with_n(char v, int clen, char* cards, int len, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE], int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE], CardAnalyzInfo& info)
{
    int count = 0;
    memset(lzout, 0, sizeof(lzout));

    int sn = clen / 4;
    if(v < CARD_A && len >= clen)
    {
        //查找普通双顺
        for(int i=info.n_stat-1; i>=0; i--)
        {
            if((info.n_stat-i) >= sn &&
                info.stat_buf[i].v < CARD_2 &&
                info.stat_buf[i].v > v &&
                (info.stat_buf[i+sn-1].v == (info.stat_buf[i].v-sn+1)))
            {
                bool check_ok = true;
                for(int j=i; j<i+sn; j++)
                {
                    if(info.stat_buf[j].n < 3)
                    {
                        check_ok = false;
                        break;
                    }
                }
                if(check_ok)
                {
                    int dan_count = 0;
                    char dan_buf[16] = {0};
                    int nnn = 0;
                    for(int j=info.n_stat-1; j>=0; j--)
                    {
                        if(j>=i && j<(i+sn))
                            continue;
                        if(info.stat_buf[j].n >= 2)
                            continue;
                        memcpy(&dan_buf[dan_count], info.stat_buf[j].c, info.stat_buf[j].n);
                        dan_count += info.stat_buf[j].n;

                        if(dan_count >= sn)
                        {
                            dan_count = sn;
                            int cindex = 0;
                            for(int k=i; k<i+sn; k++)
                            {
                                memcpy(&ret[count][cindex], info.stat_buf[k].c, 3);
                                cindex += 3;
                            }
                            memcpy(&ret[count][cindex], dan_buf, dan_count);
                            cindex += dan_count;

                            ot[count] = MDDZ_CT_THREE_STRAIGHT_WITH_N;
                            blen[count] = cindex;
                            CHECK_CANDIDATE_SIZE();
                            dan_count = 0;
                            nnn ++;
                        }
                    }
                    if (nnn == 0)
                    {
                        for(int j=info.n_stat-1; j>=0; j--)
                        {
                            if(j>=i && j<(i+sn))
                                continue;
                            if(info.stat_buf[j].n >= 3)
                                continue;
                            memcpy(&dan_buf[dan_count], info.stat_buf[j].c, info.stat_buf[j].n);
                            dan_count += info.stat_buf[j].n;

                            if(dan_count >= sn)
                            {
                                dan_count = sn;
                                int cindex = 0;
                                for(int k=i; k<i+sn; k++)
                                {
                                    memcpy(&ret[count][cindex], info.stat_buf[k].c, 3);
                                    cindex += 3;
                                }
                                memcpy(&ret[count][cindex], dan_buf, dan_count);
                                cindex += dan_count;

                                ot[count] = MDDZ_CT_THREE_STRAIGHT_WITH_N;
                                blen[count] = cindex;
                                CHECK_CANDIDATE_SIZE();
                                dan_count = 0;
                                nnn ++;
                            }
                        }
                    }
                    if (nnn == 0)
                    {
                        for(int j=info.n_stat-1; j>=0; j--)
                        {
                            if(j>=i && j<(i+sn))
                                continue;
                            if(info.stat_buf[j].n >= 4)
                                continue;
                            memcpy(&dan_buf[dan_count], info.stat_buf[j].c, info.stat_buf[j].n);
                            dan_count += info.stat_buf[j].n;

                            if(dan_count >= sn)
                            {
                                dan_count = sn;
                                int cindex = 0;
                                for(int k=i; k<i+sn; k++)
                                {
                                    memcpy(&ret[count][cindex], info.stat_buf[k].c, 3);
                                    cindex += 3;
                                }
                                memcpy(&ret[count][cindex], dan_buf, dan_count);
                                cindex += dan_count;

                                ot[count] = MDDZ_CT_THREE_STRAIGHT_WITH_N;
                                blen[count] = cindex;
                                CHECK_CANDIDATE_SIZE();
                                dan_count = 0;
                                nnn ++;
                            }
                        }
                    }
                }
            }
        }

        //寻找癞子替换顺子
        if(info.nlz > 0)
        {
            int vv = v;
            for(int v=CARD_A-1; v>=vv; v--)
            for(int i=info.n_stat-1; i>=0; i--)
            {
                char expv = 0;
                int  lzindex = 0;
                int  cindex = 0;
                if(info.stat_buf[i].v > v)
                {
                    expv = info.stat_buf[i].v;
                }
                else
                {
                    expv = v+1;
                }
                if(expv > CARD_A)
                    continue;

                bool has_exist = false;
                for(int j=0; j<count; j++)
                {
                    if(expv == C_N(ret[j][0]))
                    {
                        has_exist = true;
                        break;
                    }
                }
                if(has_exist)
                    continue;
                int index_buf[16];
                int index_count = 0;
                char shun_buf[20];
                ddz_lz_item lztemp = {0};
                

                for(int j=i; j<info.n_stat;)
                {
                    if(expv == info.stat_buf[j].v)
                    {
                        int copy_n = info.stat_buf[j].n;
                        if(copy_n>3)
                            copy_n = 3;
                        memcpy(&shun_buf[cindex], info.stat_buf[j].c, copy_n);
                        cindex += copy_n;
                        if(copy_n<3)
                        {
                            if((lzindex + (2-copy_n)) >= info.nlz)
                                break;
                            for(int k=0; k<(3-copy_n); k++)
                            {
                                shun_buf[cindex] = LZ(expv);
                                lztemp.lz_card[lzindex].card = info.lzbuf[lzindex];
                                lztemp.lz_card[lzindex].change = shun_buf[cindex];
                                lzindex ++;
                                cindex ++;
                            }
                        }
                        index_buf[index_count] = j;
                        j++;
                        index_count ++;
                    }
                    else
                    {
                        if((lzindex+2) >= info.nlz)
                            break;
                        for(int k=0; k<3; k++)
                        {
                            shun_buf[cindex] = LZ(expv);
                            lztemp.lz_card[lzindex].card = info.lzbuf[lzindex];
                            lztemp.lz_card[lzindex].change = shun_buf[cindex];
                            lzindex ++;
                            cindex ++;
                        }
                    }
                    expv--;

                    if(cindex == (3*sn))
                    {
                        int dan_count = 0;
                        char dan_buf[16] = {0};
                        for(int k=info.n_stat-1; k>=0; k--)
                        {
                            char conflict = false;
                            for(int m=0; m<index_count; m++)
                            {
                                if(k == index_buf[m])
                                {
                                    conflict = true;
                                    break;
                                }
                            }
                            if(conflict)
                                continue;
                            memcpy(&dan_buf[dan_count], info.stat_buf[k].c, info.stat_buf[k].n);
                            dan_count += info.stat_buf[k].n;
                            lztemp.size = lzindex;
                            if(dan_count >= sn)
                            {
                                dan_count = sn;
                                memcpy(ret[count], shun_buf, cindex);
                                memcpy(&ret[count][cindex], dan_buf, dan_count);
                                lzout[count] = lztemp;
                                ot[count] = MDDZ_CT_THREE_STRAIGHT_WITH_N;
                                blen[count] = clen;
                                CHECK_CANDIDATE_SIZE();
                                dan_count = 0;
                            }
                        }
                    }
                    else if((3*sn-cindex)==3 && (info.nlz-lzindex)>=3)
                    {
                        for(int x=0; x<3; x++)
                        {
                            shun_buf[cindex] = LZ(expv);
                            lztemp.lz_card[lzindex].card = info.lzbuf[lzindex];
                            lztemp.lz_card[lzindex].change = shun_buf[cindex];
                            lzindex ++;
                            cindex ++;
                        }
                        int dan_count = 0;
                        char dan_buf[16] = {0};
                        for(int k=info.n_stat-1; k>=0; k--)
                        {
                            char conflict = false;
                            for(int m=0; m<index_count; m++)
                            {
                                if(k == index_buf[m])
                                {
                                    conflict = true;
                                    break;
                                }
                            }
                            if(conflict)
                                continue;
                            memcpy(&dan_buf[dan_count], info.stat_buf[k].c, info.stat_buf[k].n);
                            dan_count += info.stat_buf[k].n;
                            lztemp.size = lzindex;
                            if(dan_count >= sn)
                            {
                                dan_count = sn;
                                memcpy(ret[count], shun_buf, cindex);
                                memcpy(&ret[count][cindex], dan_buf, dan_count);
                                lzout[count] = lztemp;
                                ot[count] = MDDZ_CT_THREE_STRAIGHT_WITH_N;
                                blen[count] = clen;
                                CHECK_CANDIDATE_SIZE();
                                dan_count = 0;
                            }
                        }
                    }
                }
            }
        }
    }

    //推荐炸弹
    GET_BOMB();
    //推荐火箭
    GET_ROCKET();
    return count;
}

int get_bigger_three_straight_with_np(char v, int clen, char* cards, int len, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE], int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE], CardAnalyzInfo& info)
{
    int count = 0;
    memset(lzout, 0, sizeof(lzout));

    int sn = clen / 5;

    if(v < CARD_A && len >= clen)
    {
        //查找普通双顺
        for(int i=info.n_stat-1; i>=0; i--)
        {
            if((info.n_stat-i) >= sn &&
                info.stat_buf[i].v < CARD_2 &&
                info.stat_buf[i].v > v &&
                (info.stat_buf[i+sn-1].v == (info.stat_buf[i].v-sn+1)))
            {
                bool check_ok = true;
                for(int j=i; j<i+sn; j++)
                {
                    if(info.stat_buf[j].n < 3)
                    {
                        check_ok = false;
                        break;
                    }
                }
                if(check_ok)
                {
                    int dan_count = 0;
                    char dan_buf[16] = {0};
                    for(int j=info.n_stat-1; j>=0; j--)
                    {
                        if(j>=i && j<(i+sn))
                            continue;
                        if(info.stat_buf[j].n < 2)
                            continue;
                        memcpy(&dan_buf[dan_count], info.stat_buf[j].c, 2);
                        dan_count += 2;

                        if(dan_count == (sn*2))
                        {
                            int cindex = 0;
                            for(int k=i; k<i+sn; k++)
                            {
                                memcpy(&ret[count][cindex], info.stat_buf[k].c, 3);
                                cindex += 3;
                            }
                            memcpy(&ret[count][cindex], dan_buf, dan_count);
                            cindex += dan_count;

                            ot[count] = MDDZ_CT_THREE_STRAIGHT_WITH_NP;
                            blen[count] = cindex;
                            CHECK_CANDIDATE_SIZE();
                            dan_count = 0;
                        }
                    }
                }
            }
        }

        //寻找癞子替换顺子
        if(info.nlz > 0)
        {
            int vv = v;
            for(int v=CARD_A-1; v>=vv; v--)
            for(int i=info.n_stat-1; i>=0; i--)
            {
                char expv = 0;
                int  lzindex = 0;
                int  cindex = 0;
                if(info.stat_buf[i].v > v)
                {
                    expv = info.stat_buf[i].v;
                }
                else
                {
                    expv = v+1;
                }
                if(expv > CARD_A)
                    continue;
                bool has_exist = false;
                for(int j=0; j<count; j++)
                {
                    if(expv == C_N(ret[j][0]))
                    {
                        has_exist = true;
                        break;
                    }
                }
                if(has_exist)
                    continue;
                int index_buf[16];
                int index_count = 0;
                char shun_buf[20];
                ddz_lz_item lztemp = {0};


                for(int j=i; j<info.n_stat;)
                {
                    if(expv == info.stat_buf[j].v)
                    {
                        int copy_n = info.stat_buf[j].n;
                        if(copy_n>3)
                            copy_n = 3;
                        memcpy(&shun_buf[cindex], info.stat_buf[j].c, copy_n);
                        cindex += copy_n;
                        if(copy_n<3)
                        {
                            if((lzindex + (2-copy_n)) >= info.nlz)
                                break;
                            for(int k=0; k<(3-copy_n); k++)
                            {
                                shun_buf[cindex] = LZ(expv);
                                lztemp.lz_card[lzindex].card = info.lzbuf[lzindex];
                                lztemp.lz_card[lzindex].change = shun_buf[cindex];
                                lzindex ++;
                                cindex ++;
                            }
                        }
                        index_buf[index_count] = j;
                        j++;
                        index_count ++;
                    }
                    else
                    {
                        if((lzindex+2) >= info.nlz)
                            break;
                        for(int k=0; k<3; k++)
                        {
                            shun_buf[cindex] = LZ(expv);
                            lztemp.lz_card[lzindex].card = info.lzbuf[lzindex];
                            lztemp.lz_card[lzindex].change = shun_buf[cindex];
                            lzindex ++;
                            cindex ++;
                        }
                    }
                    expv--;

                    if(cindex == (3*sn))
                    {
                        int dan_count = 0;
                        char dan_buf[16] = {0};
                        int lzindex2 = lzindex;

                        for(int k=info.n_stat-1; k>=0; k--)
                        {
                            char conflict = false;
                            for(int m=0; m<index_count; m++)
                            {
                                if(k == index_buf[m])
                                {
                                    conflict = true;
                                    break;
                                }
                            }
                            if(conflict)
                                continue;
							for(int m=0; m<cindex; m++)
							{
								if(info.stat_buf[k].v == C_N(shun_buf[m]))
								{
									conflict = true;
									break;
								}
							}
							if(conflict)
								continue;
							if(info.stat_buf[k].v > CARD_2)
								continue;
                            if(info.stat_buf[k].n<2 && lzindex2>=info.nlz)
                                continue;
                            int copy_n = info.stat_buf[k].n;
                            if(copy_n>2)
                                copy_n = 2;

                            memcpy(&dan_buf[dan_count], info.stat_buf[k].c, copy_n);
                            dan_count += 2;
                            if (copy_n < 2)
                            {
                                dan_buf[dan_count] = LZ(info.stat_buf[k].v);
                                lztemp.lz_card[lzindex2].card = info.lzbuf[lzindex2];
                                lztemp.lz_card[lzindex2].change = dan_buf[dan_count];
                                lzindex2 ++;
                            }
                            if(dan_count == (sn*2))
                            {
                                lztemp.size = lzindex2;
                                memcpy(ret[count], shun_buf, cindex);
                                memcpy(&ret[count][cindex], dan_buf, dan_count);
                                lzout[count] = lztemp;
                                ot[count] = MDDZ_CT_THREE_STRAIGHT_WITH_NP;
                                blen[count] = clen;
                                CHECK_CANDIDATE_SIZE();
                                dan_count = 0;
                                lzindex2 = lzindex;
                            }
							if(dan_count+2 == sn*2 && (info.nlz-lzindex2)>=2)
							{
                                lztemp.size = lzindex2;
                                memcpy(ret[count], shun_buf, cindex);
                                memcpy(&ret[count][cindex], dan_buf, dan_count);
								memcpy(&ret[count][cindex+dan_count], &info.lzbuf[lzindex2], 2);
                                lzout[count] = lztemp;
                                ot[count] = MDDZ_CT_THREE_STRAIGHT_WITH_NP;
                                blen[count] = clen;
                                CHECK_CANDIDATE_SIZE();
                                dan_count = 0;
                                lzindex2 = lzindex;
							}
                        }
                    }
                }
            }
        }
    }

    //推荐炸弹
    GET_BOMB();
    //推荐火箭
    GET_ROCKET();
    return count;
}

int get_bigger_four_with_two(char v, char* cards, int len, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE], int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE], CardAnalyzInfo& info)
{
    int count = 0;
    memset(lzout, 0, sizeof(lzout));

    if(len >= 6)
    {
        for(int i=info.n_stat-1; i>=0; i--)
        {
            //搜索适合的四张牌
            if( (info.stat_buf[i].v <= CARD_2 && info.stat_buf[i].v > v && (info.stat_buf[i].n+info.nlz) >= 4) )
            {
                int dan_index[2] = {0};
                int dan_count = 0;
                //搜索适合的单牌, 从小开始搜索
                for(int j=info.n_stat-1; j>=0; j--)
                {
                    if(i == j)
                        continue;
                    if(info.stat_buf[j].n == 1)
                    {
                        dan_index[dan_count] = j;
                        dan_count ++;
                        if(dan_count == 2)
                        {
                            memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);
                            for(int k=info.stat_buf[i].n; k<4; k++)
                            {
                                int lzindex = k - info.stat_buf[i].n;
                                ret[count][k] = LZ(info.stat_buf[i].v);
                                lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                                lzout[count].lz_card[lzindex].change = ret[count][k];
                            }
                            ret[count][4] = info.stat_buf[dan_index[0]].c[0];
                            ret[count][5] = info.stat_buf[dan_index[1]].c[0];
                            lzout[count].size = 4 - info.stat_buf[i].n;
                            ot[count] = MDDZ_CT_FOUR_WITH_TWO;
                            blen[count] = 6;
                            CHECK_CANDIDATE_SIZE();

                            dan_index[0] = j;
                            dan_count = 1;
                        }
                    }
                }
                for(int j=info.n_stat-1; j>=0; j--)
                {
                    if(i == j)
                        continue;
                    if(info.stat_buf[j].n > 1)
                    {
                        dan_index[dan_count] = j;
                        dan_count ++;
                        if(dan_count == 1)
                        {
                            memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);
                            for(int k=info.stat_buf[i].n; k<4; k++)
                            {
                                int lzindex = k - info.stat_buf[i].n;
                                ret[count][k] = LZ(info.stat_buf[i].v);
                                lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                                lzout[count].lz_card[lzindex].change = ret[count][k];
                            }
                            ret[count][4] = info.stat_buf[dan_index[0]].c[0];
                            ret[count][5] = info.stat_buf[dan_index[0]].c[1];
                            lzout[count].size = 4 - info.stat_buf[i].n;
                            ot[count] = MDDZ_CT_FOUR_WITH_TWO;
                            blen[count] = 6;
                            CHECK_CANDIDATE_SIZE();

                            dan_index[0] = j;
                            dan_count = 0;
                        }
                        if(dan_count == 2)
                        {
                            memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);
                            for(int k=info.stat_buf[i].n; k<4; k++)
                            {
                                int lzindex = k - info.stat_buf[i].n;
                                ret[count][k] = LZ(info.stat_buf[i].v);
                                lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                                lzout[count].lz_card[lzindex].change = ret[count][k];
                            }
                            ret[count][4] = info.stat_buf[dan_index[0]].c[0];
                            ret[count][5] = info.stat_buf[dan_index[1]].c[0];
                            lzout[count].size = 4 - info.stat_buf[i].n;
                            ot[count] = MDDZ_CT_FOUR_WITH_TWO;
                            blen[count] = 6;
                            CHECK_CANDIDATE_SIZE();

                            dan_index[0] = j;
                            dan_count = 1;
                        }
                    }
                }
                if(dan_count == 1 && (info.stat_buf[i].n+info.nlz) > 4)
                {
                    memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);
                    for(int k=info.stat_buf[i].n; k<4; k++)
                    {
                        int lzindex = k - info.stat_buf[i].n;
                        ret[count][k] = LZ(info.stat_buf[i].v);
                        lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                        lzout[count].lz_card[lzindex].change = ret[count][k];
                    }
                    ret[count][4] = info.stat_buf[dan_index[0]].c[0];
                    ret[count][5] = info.lzbuf[4-info.stat_buf[i].n];
                    ot[count] = MDDZ_CT_FOUR_WITH_TWO;
                    blen[count] = 6;
                    CHECK_CANDIDATE_SIZE();
                }
                else if(dan_count == 0 && (info.stat_buf[i].n + info.nlz) > 5)
                {
                    memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);
                    for(int k=info.stat_buf[i].n; k<4; k++)
                    {
                        int lzindex = k - info.stat_buf[i].n;
                        ret[count][k] = LZ(info.stat_buf[i].v);
                        lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                        lzout[count].lz_card[lzindex].change = ret[count][k];
                    }
                    ret[count][4] = info.lzbuf[4 - info.stat_buf[i].n];
                    ret[count][5] = info.lzbuf[5 - info.stat_buf[i].n];
                    ot[count] = MDDZ_CT_FOUR_WITH_TWO;
                    blen[count] = 6;
                    CHECK_CANDIDATE_SIZE();
                }
            }
        }
    }

    //推荐炸弹
    GET_BOMB();
    //推荐火箭
    GET_ROCKET();
    return count;
}

int get_bigger_four_with_twopair(char v, char* cards, int len, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE], int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE], CardAnalyzInfo& info)
{
    int count = 0;
    memset(lzout, 0, sizeof(lzout));

    if(len >= 8)
    {
        for(int i=info.n_stat-1; i>=0; i--)
        {
            //搜索适合的四张牌
            if( (info.stat_buf[i].v <= CARD_2 && info.stat_buf[i].v > v && (info.stat_buf[i].n+info.nlz) >= 4) )
            {
                int dan_index[2] = {0};
                int dan_count = 0;
                int rnlz = info.nlz - (4 - info.stat_buf[i].n);
                for(int j=info.n_stat-1; j>=0; j--)
                {
                    if(i == j)
                        continue;
                    if(info.stat_buf[j].n > 1 || ((info.stat_buf[j].n + rnlz) > 1) && info.stat_buf[j].v <= CARD_2)
                    {
                        dan_index[dan_count] = j;
                        dan_count ++;
                        if(info.stat_buf[j].n == 1)
                            rnlz --;
                        if(dan_count == 2)
                        {
                            int lzindex = 0;
                            memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);
                            for(int k=info.stat_buf[i].n; k<4; k++)
                            {
                                lzindex = k - info.stat_buf[i].n;
                                ret[count][k] = LZ(info.stat_buf[i].v);
                                lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                                lzout[count].lz_card[lzindex].change = ret[count][k];
                            }
                            lzindex = 4 - info.stat_buf[i].n;

                            int n = info.stat_buf[dan_index[0]].n;
                            if(n > 2) n= 2;
                            memcpy(&ret[count][4], info.stat_buf[dan_index[0]].c, n);
                            n = info.stat_buf[dan_index[1]].n;
                            if(n > 2) n= 2;
                            memcpy(&ret[count][6], info.stat_buf[dan_index[1]].c, n);
                            if(info.stat_buf[dan_index[0]].n == 1)
                            {
                                ret[count][5] = LZ(info.stat_buf[dan_index[0]].v);
                                lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                                lzout[count].lz_card[lzindex].change = ret[count][5];
                                lzindex ++;
                            }
                            if(info.stat_buf[dan_index[1]].n == 1)
                            {
                                ret[count][7] = LZ(info.stat_buf[dan_index[1]].v);
                                lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                                lzout[count].lz_card[lzindex].change = ret[count][7];
                                lzindex ++;
                            }

                            lzout[count].size = lzindex;
                            ot[count] = MDDZ_CT_FOUR_WITH_TWOPAIR;
                            blen[count] = 8;
                            CHECK_CANDIDATE_SIZE();

                            rnlz = info.nlz - (4 - info.stat_buf[i].n);
                            dan_index[0] = j;
                            dan_count = 1;
                        }
                    }
                }
            }
        }
    }

    //推荐炸弹
    GET_BOMB();
    //推荐火箭
    GET_ROCKET();
    return count;
}

int get_bigger_bomb(char v, bool is_soft, char* cards, int len, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE], int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE], CardAnalyzInfo& info)
{
    int count = 0;
    memset(lzout, 0, sizeof(lzout));

    for(int i=info.n_stat-1; i>=0; i--)
    {
        if(is_soft)
        {
            //软炸比较大小, 硬炸弹不比大小
            if( (info.stat_buf[i].v <= CARD_2 && info.stat_buf[i].v > v && info.stat_buf[i].n<4 && (info.stat_buf[i].n+info.nlz) >= 4) ||
                (info.stat_buf[i].n == 4) )
            {
                memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);
                for(int j=info.stat_buf[i].n; j<4; j++)
                {
                    int lzindex = j-info.stat_buf[i].n;
                    ret[count][j] = LZ(info.stat_buf[i].v);
                    lzout[count].lz_card[lzindex].card = info.lzbuf[lzindex];
                    lzout[count].lz_card[lzindex].change = ret[count][j];
                }
                lzout[count].size = 4 - info.stat_buf[i].n;
                ot[count] = MDDZ_CT_BOMB;
                blen[count] = 4;
                CHECK_CANDIDATE_SIZE();
            }
        }
        else if(info.stat_buf[i].n == 4 && info.stat_buf[i].v > v)
        {
            //硬炸弹需要比大小
            memcpy(ret[count], info.stat_buf[i].c, info.stat_buf[i].n);
            ot[count] = MDDZ_CT_BOMB;
            blen[count] = 4;
            CHECK_CANDIDATE_SIZE();
        }
    }

    //推荐火箭
    GET_ROCKET();
    return count;
}

int ddz_get_bigger_compose(char* cards, int clen, int ct, char* candidate,
                           int dlen, char ret[MAX_CANDIDATE][20], int blen[MAX_CANDIDATE],
						   int ot[MAX_CANDIDATE], int lz, ddz_lz_item lzout[MAX_CANDIDATE])
{
    int count = 0;
    char v = 0;
    ddz_lz_item out = {0};
    CardAnalyzInfo info;
    analyze(candidate, dlen, lz, &info);

    CardAnalyzInfo infosrc;
    analyze(cards, clen, lz, &infosrc);

    switch(ct)
    {
    case MDDZ_CT_ERROR:                 //首发牌提示
        count = get_big_first(candidate, dlen, ret, blen, ot, lz, lzout, info);
        break;
    case MDDZ_CT_ONE:                   //单牌
        v = C_N(cards[0]);
        count = get_bigger_one(v, candidate, dlen, ret, blen, ot, lz, lzout, info);
        break;
    case MDDZ_CT_PAIR:                  //一对
        v = C_N(is_pair(cards, clen, lz, &out, infosrc));
        count = get_bigger_pair(v, candidate, dlen, ret, blen, ot, lz, lzout, info);
        break;
    case MDDZ_CT_THREE:                 //三条
        v = C_N(is_three(cards, clen, lz, &out, infosrc));
        count = get_bigger_three(v, candidate, dlen, ret, blen, ot, lz, lzout, info);
        break;
    case MDDZ_CT_STRAIGHT:              //单顺
        v = C_N(is_straight(cards, clen, lz, &out, infosrc));
        count = get_bigger_straight(v, clen, candidate, dlen, ret, blen, ot, lz, lzout, info);
        break;
    case MDDZ_CT_TWO_STRAIGHT:          //双顺
        v = C_N(is_two_straight(cards, clen, lz, &out, infosrc));
        count = get_bigger_two_straight(v, clen, candidate, dlen, ret, blen, ot, lz, lzout, info);
        break;
    case MDDZ_CT_THREE_STRAIGHT:        //三顺
        v = C_N(is_three_straight(cards, clen, lz, &out, infosrc));
        count = get_bigger_three_straight(v, clen, candidate, dlen, ret, blen, ot, lz, lzout, info);
        break;
    case MDDZ_CT_THREE_WITH_ONE:        //三带一单
        v = C_N(is_three_with_one(cards, clen, lz, &out, infosrc));
        count = get_bigger_three_with_one(v, candidate, dlen, ret, blen, ot, lz, lzout, info);
        break;
    case MDDZ_CT_THREE_WITH_PAIR:       //三带一对
        v = C_N(is_three_with_pair(cards, clen, lz, &out, infosrc));
        count = get_bigger_three_with_pair(v, candidate, dlen, ret, blen, ot, lz, lzout, info);
        break;
    case MDDZ_CT_THREE_STRAIGHT_WITH_N: //飞机带单翅膀
        v = C_N(is_three_straight_with_n(cards, clen, lz, &out, infosrc));
        count = get_bigger_three_straight_with_n(v, clen, candidate, dlen, ret, blen, ot, lz, lzout, info);
        break;
    case MDDZ_CT_THREE_STRAIGHT_WITH_NP://飞机带对翅膀
        v = C_N(is_three_straight_with_np(cards, clen, lz, &out, infosrc));
        count = get_bigger_three_straight_with_np(v, clen, candidate, dlen, ret, blen, ot, lz, lzout, info);
        break;
    case MDDZ_CT_FOUR_WITH_TWO:         //四带两单
        v = C_N(is_four_with_two(cards, clen, lz, &out, infosrc));
        count = get_bigger_four_with_two(v, candidate, dlen, ret, blen, ot, lz, lzout, info);
        break;
    case MDDZ_CT_FOUR_WITH_TWOPAIR:     //四带两对
        v = C_N(is_four_with_twopair(cards, clen, lz, &out, infosrc));
        count = get_bigger_four_with_twopair(v, candidate, dlen, ret, blen, ot, lz, lzout, info);
        break;
    case MDDZ_CT_BOMB:                  //炸弹
        {
            v = is_bomb(cards, clen, lz, &out, infosrc);
            if(v == 0) break;
            bool is_soft = false;
            if(out.size > 0)
                is_soft = true;
            else
            {
                for(int i=0; i<clen; i++)
                {
                    if(C_T(cards[i]) == C_TYPE_LZ)
                    {
                        is_soft = true;
                        break;
                    }
                }
            }
            count = get_bigger_bomb(v, is_soft, candidate, dlen, ret, blen, ot, lz, lzout, info);

        }
        break;
    case MDDZ_CT_ROCKET:                //火箭
        return 0;
    default:
        break;
    }

    return count;
}

bool ddz_has_straight(char* cards, int len, char* buf, int& blen, int lz)
{
    CardAnalyzInfo info;
    analyze(cards, len, lz, &info);

    if(info.n_stat < 5)
        return false;
    char temp[20];
    int  count = 0;
    for(int i=0; i<info.n_stat; i++)
    {
        if(info.stat_buf[i].v > CARD_A)
            continue;
        temp[0] = info.stat_buf[i].c[0];
        count = 1;
        for(int j=i+1; j<info.n_stat; j++)
        {
            if((info.stat_buf[j-1].v - info.stat_buf[j].v) == 1)
            {
                temp[count] = info.stat_buf[j].c[0];
                count ++;
            }
            else
                break;
        }
        if(count >= 5)
        {
            blen = count;
            memcpy(buf, temp, count);
            return true;
        }
    }
    
    return false;
}

bool ddz_has_two_straight(char* cards, int len, char* buf, int& blen, int lz)
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ddz_compare(cards_buf& c1, cards_buf& c2, int lz)
{
    return ddz_compare(c1.cards, c1.size, c1.card_type, c2.cards, c2.size, c2.card_type, lz);
}

bool check_exist_cards(cards_buf& handcards, cards_buf& out)
{
    return check_exist_cards(handcards.cards, handcards.size, out.cards, out.size);
}

int ddz_get_cards_type(char* cards, int len, int& maxv)
{
    return 0;
}

int ddz_get_cards_type_list(cards_buf& src, int lz, cards_buf buf[MAX_CANDIDATE])
{
    int ret = 0;
    char maxcard[MAX_CANDIDATE];
    char outcard[MAX_CANDIDATE][20];
    int type[MAX_CANDIDATE];
    ddz_lz_item out[MAX_CANDIDATE];

    memset(maxcard, 0, sizeof(maxcard));
    memset(outcard, 0, sizeof(outcard));
    memset(type, 0, sizeof(type));
    memset(out, 0, sizeof(out));
    memset(buf, 0, sizeof(cards_buf) * MAX_CANDIDATE);

    char temp[20];
    memcpy(temp, src.cards, src.size);
    //change_12_2_A2(temp, src.size);
    char lztemp = lz;//change_12_2_A2(lz);
    
    ret = ddz_get_cards_type_list(temp, src.size, lztemp, maxcard, outcard, type, out);
    for(int i=0; i<ret; i++)
    {
        buf[i].max_card = maxcard[i];
        memcpy(buf[i].cards, outcard[i], src.size);
        buf[i].card_type = type[i];
        buf[i].clz = out[i];
        buf[i].size = src.size;
        //change_A2_2_12(buf[i].cards, src.size);
        //change_A2_2_12(buf[i].clz.lz_card, buf[i].clz.size);
    }
    return ret;
}

int ddz_get_bigger_compose(cards_buf& src, cards_buf& candidate, int lz, cards_buf out[MAX_CANDIDATE])
{
    char ret[MAX_CANDIDATE][20];
    int blen[MAX_CANDIDATE];
    int ot[MAX_CANDIDATE];
    ddz_lz_item lzout[MAX_CANDIDATE];

    memset(ret, 0, sizeof(ret));
    memset(blen, 0, sizeof(blen));
    memset(ot, 0, sizeof(ot));
    memset(lzout, 0, sizeof(lzout));
    memset(out, 0, sizeof(cards_buf)*MAX_CANDIDATE);

    char srctemp[20];
    char candtemp[20];
    memcpy(srctemp, src.cards, src.size);
    memcpy(candtemp, candidate.cards, candidate.size);


    int n = ddz_get_bigger_compose(srctemp, src.size, src.card_type, candtemp, candidate.size, ret, blen, ot, lz, lzout);
    for(int i=0; i<n; i++)
    {
        memcpy(out[i].cards, ret[i], blen[i]);
        out[i].size = blen[i];
        out[i].card_type = ot[i];
        out[i].clz = lzout[i];
    }
    return n;
}

bool ddz_has_straight(cards_buf& src, cards_buf& out, int lz)
{
    char buf[20];
    memcpy(buf, src.cards, src.size);
    sort_by_num(buf, src.size);
    bool ret = ddz_has_straight(src.cards, src.size, out.cards, out.size, lz);

    return ret;
}

int calculate_cards_stat(char* buf, int ncard, card_stat stat_buf[16])
{
    int n_stat = 0;
    if(ncard<=0)
        return true;
    char check_card = C_N(buf[0]);
    stat_buf[n_stat].v = check_card;
    stat_buf[n_stat].n = 1;
    stat_buf[n_stat].c[0] = buf[0];
    for(int i=1; i<ncard; i++)
    {
        check_card = C_N(buf[i]);
        if(check_card == stat_buf[n_stat].v)
        {
            stat_buf[n_stat].c[stat_buf[n_stat].n] = buf[i];
            stat_buf[n_stat].n ++;
        }
        else
        {
            (n_stat)++;
            if((n_stat)>=15)
                return false;
            stat_buf[n_stat].v = check_card;
            stat_buf[n_stat].n = 1;
            stat_buf[n_stat].c[0] = buf[i];
        }
    }
    n_stat++;
    return n_stat;
}

int get_remain(char* src, int slen, char* out, int olen, char* remain)
{
    if(slen == olen)
        return 0;
    int check_len = 0;
    int remain_len = 0;
    for(int i=0; i<slen; i++)
    {
        if(check_len>=olen)
        {
            remain[remain_len] = src[i];
            remain_len ++;
        }
        else
        {
            bool b_check = false;
            for(int j=check_len; j<olen; j++)
            {
                if(src[i] == out[j])
                {
                    check_len ++;
                    b_check = true;
                    break;
                }
            }
            if(!b_check)
            {
                remain[remain_len] = src[i];
                remain_len ++;
            }
        }
    }
    return remain_len;
}