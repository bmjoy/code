#include "ai_helper.h"
#include <string.h>

void cal_cards_info(char* c, int size, cards_info& info)
{
    for(int i=0; i<16; i++)info.cards[i].clear();
    for(int i=0; i<size; i++)
    {
        int v = C_N(c[i]);
        info.cards[v].add(c[i]);
    }
}

bool has_bigger(char* c, int size, int type, char* candidate, int len)
{
    cards_buf src(c, size, type);
    cards_buf candi(candidate, len);
    cards_buf out[MAX_CANDIDATE];
    int n = ddz_get_bigger_compose(src, candi, 0, out);
    return (n > 0);
}

void analyze_handcard(char* cards, int len, int lz, HandAnalyzInfo& info)
{
    SET_ZERO(info);
    if(len == 1)
    {
        if(cards[0] == 0x4f)
        {
            info.king[0] = 0x4f;
            return;
        }
        if(cards[0] == 0x4e)
        {
            info.king[0] = 0x4e;
            return;
        }
        if(C_N(cards[0]) == lz)
        {
            info.lzbuf[0] = cards[0];
            info.nlz = 1;
            return;
        }
        info.stat_buf[0].c[0] = cards[0];
        info.stat_buf[0].n = 1;
        info.stat_buf[0].v = C_N(cards[0]);
        info.n_stat = 1;
        info.normal[0] = cards[0];
        info.nsize = 1;
        return;
    }
    int from = 0;
    if(cards[0] == 0x4f && cards[1] == 0x4e)
    {
        from = 2;
        info.king[0] = 0x4f; info.king[1] = 0x4e;
    }
    else if(cards[0] == 0x4f)
    {
        from = 1;
        info.king[0] = 0x4f;
    }
    else if(cards[0] == 0x4e)
    {
        from = 1;
        info.king[0] = 0x4e;
    }
    if(from == len)
        return;
    if(C_N(cards[from]) == lz)
    {
        info.lzbuf[0] = cards[from];
        info.nlz = 1;
    }
    else
    {
        info.normal[0] = cards[from];
        info.nsize++;
        info.stat_buf[0].c[0] = cards[from];
        info.stat_buf[0].v = C_N(cards[from]);
        info.stat_buf[0].n = 1;
    }

    for(int i=from+1; i<len; i++)
    {
        char test = C_N(cards[i]);
        if(test == lz)
        {
            info.lzbuf[info.nlz] = cards[i];
            info.nlz ++;
            continue;
        }
        info.normal[info.nsize] = cards[i];
        info.nsize ++;
        if(info.stat_buf[info.n_stat].n == 0)
        {
            info.stat_buf[info.n_stat].c[0] = cards[i];
            info.stat_buf[info.n_stat].v = C_N(cards[i]);
            info.stat_buf[info.n_stat].n = 1;
            continue;
        }
        if(test == info.stat_buf[info.n_stat].v)
        {
            info.stat_buf[info.n_stat].c[info.stat_buf[info.n_stat].n] = cards[i];
            info.stat_buf[info.n_stat].n ++;
            continue;
        }
        info.n_stat ++;
        info.stat_buf[info.n_stat].c[0] = cards[i];
        info.stat_buf[info.n_stat].v = C_N(cards[i]);
        info.stat_buf[info.n_stat].n = 1;
    }
    info.n_stat++;
}

int split_info::get_cards(char* buf, int len)
{
    int ret = 0;
    ret += single.get_cards(&buf[ret], len-ret);
    ret += pair.get_cards(&buf[ret], len-ret);
    ret += three.get_cards(&buf[ret], len-ret);
    ret += bomb.get_cards(&buf[ret], len-ret);
    ret += straight.get_cards(&buf[ret], len-ret);
    ret += straight2.get_cards(&buf[ret], len-ret);
    ret += straight3.get_cards(&buf[ret], len-ret);
    ret += king.get_cards(&buf[ret], len-ret);
    sort_by_num(buf, ret);
    return ret;
}

void split_info::frush_size()
{
    size = 0;
    if(king.size > 0) size ++;
    size += single.size;
    size += pair.size;
    size += three.size;
    size += bomb.size;
    size += straight.size;
    size += straight2.size;
    size += straight3.size;

    single.sort();
    pair.sort();
    three.sort();
    straight.sort();
    straight2.sort();
    bomb.sort();
    straight3.sort();
}

void analyze_card_info(char* c, int len, CandAnalyzInfo& info)
{
    memset(&info, 0, sizeof(info));
    char buf[20];
    memcpy(buf, c, len);
    sort_by_num(buf, len);
    char chk = 0;
    info.size = 0;
    for(int i=0; i<len; i++)
    {
        char tc = C_N(buf[i]);
        if(tc>CARD_2)
        {
            info.king.add(buf[i]);
            continue;
        }
        if(tc == chk)
        {
            info.point[info.size-1].add(buf[i]);
        }
        else
        {
            info.size ++;
            info.point[info.size-1].add(buf[i]);
            chk = tc;
        }
    }
    if(info.size < 0) info.size=0;
}

void add_split_card(split_info& split, card_point& card)
{
    switch(card.size)
    {
    case 1:
        {
            card_single single;
            single.add(card.cards[0]);
            split.single.add(single);
        }
        break;
    case 2:
        {
            card_pair pair;
            for(int i=0; i<2; i++)
            {
                pair.add(card.cards[i]);
            }
            split.pair.add(pair);
        }
        break;
    case 3:
        {
            card_three three;
            for(int i=0; i<3; i++)
            {
                three.add(card.cards[i]);
            }
            split.three.add(three);
        }
        break;
    case 4:
        {
            card_bomb bomb;
            for(int i=0; i<4; i++)
            {
                bomb.add(card.cards[i]);
            }
            split.bomb.add(bomb);
        }
        break;
    }
}

void split_independent_cards(CandAnalyzInfo& info, split_info& split)
{
    for(int i=0; i<info.king.size; i++)
        split.king.add(info.king.cards[i]);
    for(int i=0; i<info.size; i++)
    {
        if(info.point[i].val() == 0)
            continue;
        if(info.point[i].size == 4)
        {
            add_split_card(split, info.point[i]);
            continue;
        }
        if(info.point[i].val() == CARD_2)
        {
            add_split_card(split, info.point[i]);
            continue;
        }
        if(info.point[i].val() == CARD_A)
        {
            if((i<info.size-1 && (info.point[i].val()-info.point[i+1].val())>1) ||
                i == info.size-1)
            {
                add_split_card(split, info.point[i]);
            }
            continue;
        }
        if(i == 0)
        {
            if(1 == info.size ||
                (i<info.size-1 && (info.point[i].val()-info.point[i+1].val())>1))
            {
                add_split_card(split, info.point[i]);
            }
            continue;
        }
        if(i==info.size-1)
        {
            if((info.point[i-1].val()-info.point[i].val()) > 1)
            {
                add_split_card(split, info.point[i]);
            }
            continue;
        }
        if((info.point[i].val()-info.point[i+1].val())>1 &&
            (info.point[i-1].val()-info.point[i].val())>1)
            add_split_card(split, info.point[i]);
    }
}

bool split_straight3_cards(CandAnalyzInfo& info, split_info& split)
{
    card_straight3  ts[4];
    int ts_size = 0;
    for(int i=0; i<info.size-1; )
    {
        if(info.point[i].val() <= CARD_A &&
            (info.point[i].val()-info.point[i+1].val()) == 1 &&
            info.point[i].size == 3 && info.point[i+1].size == 3)
        {
            int to = i+1;
            while(to<info.size-1 &&
                (info.point[to].val()-info.point[to+1].val()) == 1 &&
                info.point[to+1].size == 3)
            {
                to ++;
            }
            for(int j=i; j<=to; j++)
            {
                for(int k=0; k<3; k++)
                {
                    char c;
                    info.point[j].get(c);
                    ts[ts_size].add(c);
                }
            }
            ts_size ++;
            i = to + 1;
        }
        else
        {
            i++;
        }
    }
    for(int i=0; i<ts_size; i++)
    {
        split.straight3.add(ts[i]);
    }
    return (ts_size>0);
}

bool split_straight2_cards(CandAnalyzInfo& info, split_info& split)
{
    card_straight2  ts[4];
    int ts_size = 0;

    if(split.straight.size > 1)
    {
        for(int i=0; i<split.straight.size-1; i++)
        {
            if(split.straight[i].val() == split.straight[i+1].val() &&
                split.straight[i].size == split.straight[i+1].size)
            {
                for(int j=0; j<split.straight[i].size; j++)
                {
                    ts[ts_size].add(split.straight[i].cards[j]);
                    ts[ts_size].add(split.straight[i+1].cards[j]);
                }
                ts_size ++;
                split.straight[i].size = 0;
                split.straight[i+1].size = 0;
                split.straight.resort();
                break;
            }
        }
    }

    for(int i=0; i<info.size-2; )
    {
        if(info.point[i].val() <= CARD_A &&
            (info.point[i].val()-info.point[i+2].val())==2 &&
            info.point[i].size>=2 && info.point[i+1].size>=2 && info.point[i+2].size>=2)
        {
            int to = i+2;
            while(to<info.size-1 &&
                (info.point[to].val()-info.point[to+1].val()) == 1 &&
                info.point[to+1].size >= 2)
            {
                to ++;
            }
            for(int j=i; j<=to; j++)
            {
                for(int k=0; k<2; k++)
                {
                    char c;
                    info.point[j].get(c);
                    ts[ts_size].add(c);
                }
            }
            ts_size ++;
            i = to + 1;
        }
        else
        {
            i++;
        }
    }
    for(int i=0; i<ts_size; i++)
    {
        split.straight2.add(ts[i]);
    }
    return (ts_size>0);
}

bool split_straight1_cards(CandAnalyzInfo& info, split_info& split)
{
    card_straight  ts[4];
    int ts_size = 0;
    //找出最小串
    for(int i=0; i<info.size-4; i++)
    {
        if(info.point[i].val() == CARD_2)
            continue;

        if((info.point[i].val()-info.point[i+4].val()) == 4)
        {
            bool check_exist = true;
            for(int j=i; j<=i+4; j++)
            {
                if(info.point[j].size < 1)
                {
                    check_exist = false;
                }
            }
            if(!check_exist)
                continue;
            int to = i+4;
            for(int j=i; j<=to; j++)
            {
                char c;
                info.point[j].get(c);
                ts[ts_size].add(c);
            }
            ts_size ++;
        }
    }
    //前后续串
    for(int i=0; i<info.size; i++)
    {
        if(info.point[i].size == 0)
            continue;
        if(info.point[i].val() == CARD_2)
            continue;
        for(int j=0; j<ts_size; j++)
        {
            if(info.point[i].val() == (C_N(ts[j].cards[0])+1) ||
                info.point[i].val() == (C_N(ts[j].cards[ts[j].size-1])-1))
            {
                char c;
                info.point[i].get(c);
                ts[j].add(c);
                sort_by_num(ts[j].cards, ts[j].size);
                break;
            }
        }
    }
    //合并串
    if(ts_size>1)
    {
        for(int i=0; i<ts_size-1; i++)
        {
            if(ts[i].size == 0)
                continue;
            if(C_N(ts[i].cards[ts[i].size-1]) == C_N(ts[i+1].cards[0])+1)
            {
                for(int j=0; j<ts[i+1].size; j++)
                    ts[i].add(ts[i+1].cards[j]);
                ts[i+1].size = 0;
            }
        }
    }

    //输出
    for(int i=0; i<ts_size; i++)
    {
        if(ts[i].size>0)
        {
            split.straight.add(ts[i]);
        }
    }

    return (ts_size>0);
}

bool re_analyze_card_info(char* c, int len, split_info& split, CandAnalyzInfo& info)
{
    char used[32] = {0};
    int  ulen = 0;
    char remain[20] = {0};
    int  rlen;

    ulen = split.get_cards(used, sizeof(used));
    if(ulen == len)
        return false;
    rlen = get_remain(c, len, used, ulen, remain);
    analyze_card_info(remain, rlen, info);

    return true;
}

void cal_simple_split(char* cards, int size, simple_split& split)
{
    SET_ZERO(split);
    CandAnalyzInfo info;
    analyze_card_info(cards, size, info);
    for(int i=0; i<info.size; i++)
    {
        switch(info.point[i].size)
        {
        case 1:
            {
                card_single single;
                single.add(info.point[i].cards[0]);
                split.single.add(single);
            }
            break;
        case 2:
            {
                card_pair pair;
                for(int i=0; i<2; i++)
                {
                    pair.add(info.point[i].cards[i]);
                }
                split.pair.add(pair);
            }
            break;
        case 3:
            {
                card_three three;
                for(int i=0; i<3; i++)
                {
                    three.add(info.point[i].cards[i]);
                }
                split.three.add(three);
            }
            break;
        case 4:
            {
                card_bomb bomb;
                for(int i=0; i<4; i++)
                {
                    bomb.add(info.point[i].cards[i]);
                }
                split.bomb.add(bomb);
            }
            break;
        }
    }
    split.single.sort();
    split.pair.sort();
    split.three.sort();
    split.bomb.sort();
}

void split_optimal_cards1(char* c, int len, split_info& split)
{
    SET_ZERO(split);
    CandAnalyzInfo info;
    analyze_card_info(c, len, info);
    split_independent_cards(info, split);
    re_analyze_card_info(c, len, split, info);
    do
    {
        if(!re_analyze_card_info(c, len, split, info))
            break;
        //飞机，连对，顺子，三代1，三代对，对子，单张
        if(split_straight1_cards(info, split))
            if(!re_analyze_card_info(c, len, split, info))
                break;

        if(split_straight3_cards(info, split))
            if(!re_analyze_card_info(c, len, split, info))
                break;

        if(split_straight2_cards(info, split))
            if(!re_analyze_card_info(c, len, split, info))
                break;

        for(int i=0; i<info.size; i++)
        {
            add_split_card(split, info.point[i]);
        }
    }while (false);

    split.frush_size();
}

void split_optimal_cards2(char* c, int len, split_info& split)
{
    SET_ZERO(split);
    CandAnalyzInfo info;
    analyze_card_info(c, len, info);
    split_independent_cards(info, split);
    re_analyze_card_info(c, len, split, info);
    do
    {
        if(split_straight2_cards(info, split))
            if(!re_analyze_card_info(c, len, split, info))
                break;

        if(!re_analyze_card_info(c, len, split, info))
            break;
        //飞机，连对，顺子，三代1，三代对，对子，单张
        if(split_straight1_cards(info, split))
            if(!re_analyze_card_info(c, len, split, info))
                break;

        if(split_straight3_cards(info, split))
            if(!re_analyze_card_info(c, len, split, info))
                break;

        for(int i=0; i<info.size; i++)
        {
            add_split_card(split, info.point[i]);
        }
    }while (false);

    split.frush_size();
}

void split_optimal_cards3(char* c, int len, split_info& split)
{
    SET_ZERO(split);
    CandAnalyzInfo info;
    analyze_card_info(c, len, info);
    split_independent_cards(info, split);
    re_analyze_card_info(c, len, split, info);
    do
    {
        if(split_straight3_cards(info, split))
            if(!re_analyze_card_info(c, len, split, info))
                break;

        if(split_straight2_cards(info, split))
            if(!re_analyze_card_info(c, len, split, info))
                break;

        if(!re_analyze_card_info(c, len, split, info))
            break;
        //飞机，连对，顺子，三代1，三代对，对子，单张
        if(split_straight1_cards(info, split))
            if(!re_analyze_card_info(c, len, split, info))
                break;

        for(int i=0; i<info.size; i++)
        {
            add_split_card(split, info.point[i]);
        }
    }while (false);

    split.frush_size();
}

void split_optimal_cards4(char* c, int len, split_info& split)
{
    SET_ZERO(split);
    CandAnalyzInfo info;
    analyze_card_info(c, len, info);
    split_independent_cards(info, split);
    re_analyze_card_info(c, len, split, info);
    do
    {
        if(!re_analyze_card_info(c, len, split, info))
            break;
        bool b = false;
        for(int i=0; i<info.size; i++)
        {
            if(info.point[i].size == 3)
            {
                add_split_card(split, info.point[i]);
                b = true;
            }
        }
        if(b)
            if(!re_analyze_card_info(c, len, split, info))
                break;
        //飞机，连对，顺子，三代1，三代对，对子，单张
        if(split_straight1_cards(info, split))
            if(!re_analyze_card_info(c, len, split, info))
                break;

        if(split_straight3_cards(info, split))
            if(!re_analyze_card_info(c, len, split, info))
                break;

        if(split_straight2_cards(info, split))
            if(!re_analyze_card_info(c, len, split, info))
                break;

        for(int i=0; i<info.size; i++)
        {
            add_split_card(split, info.point[i]);
        }
    }while (false);

    split.frush_size();
}

void split_optimal_cards(char* c, int len, split_info& split)
{
    const int N_SPLIT = 4;
    split_info tsplit[N_SPLIT];
    int power[N_SPLIT];
    int turns[N_SPLIT];
    split_optimal_cards1(c, len, tsplit[0]);
    split_optimal_cards2(c, len, tsplit[1]);
    split_optimal_cards3(c, len, tsplit[2]);
    split_optimal_cards4(c, len, tsplit[3]);
    for(int i=0; i<N_SPLIT; i++)
        cal_power(tsplit[i], power[i], turns[i]);
    
    int oi = 0;
    for(int i=1; i<N_SPLIT; i++)
    {
        if(turns[oi] > turns[i] ||
            (turns[oi]==turns[i] && power[oi]<power[i]))
        {
            oi = i;
        }
        else if(turns[oi] == turns[i] && power[oi]<power[i])
        {
            oi = i;
        }
    }
    split = tsplit[oi];
}

int next_id(int id)
{
    id ++;
    if(id>2)id=0;
    return id;
}

int pre_id(int id)
{
    id--;
    if(id<0)id=2;
    return id;
}

int set_out_cards(cards_buf& out, char* cards, int len, int type)
{
    memcpy(out.cards, cards, len);
    out.card_type = type;
    out.size = len;
    out.max_card = C_N(cards[0]);
    return len;
}

bool has_small_pair(char v, cards_info& info, char* buf, bool include_bomb)
{
    for(int i=0; i<v; i++)
    {
        if(include_bomb && info.cards[i].size == 4)
            continue;
        if(info.cards[i].size>=2)
        {
            memcpy(buf, info.cards[i].cards, 2);
            return true;
        }
    }
    return false;
}

int get_mini_bigger_string(char* src, int ls, char* candidate, int lc, char* buf)
{
    int len = 0;

    if(lc < ls)
        return 0;
    char vs = C_N(src[0]);
    int  begin = -1;
    for(int i=0; i<lc; i++)
    {
        if((lc-i) < ls)
            break;
        if(C_N(candidate[i]) < vs)
            break;
        begin = i;
    }
    if(begin<0)
        return 0;
    for(int i=0; i<ls; i++)
    {
        buf[i] = candidate[i+begin];
    }

    return len;
}

void cal_power(split_info& info, int& power, int& turns)
{
    power = 0;
    turns = 0;
    for(int i=0; i<info.straight.size; i++)
    {
        power += (4 + info.straight[i].size - 5);
    }
    for(int i=0; i<info.straight2.size; i++)
    {
        power += (5 + info.straight2[i].size - 6);
    }
    power += info.three.size * 3;
    if(info.king.size == 1)
        power += 1;
    if(info.king.size == 2)
        power += 7;
    power += info.bomb.size * 7;
    for(int i=0; i<info.straight3.size; i++)
    {
        power += info.straight3[i].size;
    }

    int dan = info.single.size + info.pair.size;
    int nsan = info.three.size;
    for(int i=0; i<info.straight3.size; i++)
    {
        nsan += (info.straight3[i].size/3);
    }
    dan -= nsan;
    if(dan < 0) dan = 0;
    power += dan;

    turns += info.three.size;
    turns += info.straight.size;
    turns += info.straight2.size;
    turns += info.straight3.size;
    turns += info.bomb.size;
    turns += info.king.size>0?1:0;
    turns += dan;
}

bool get_three_with(split_info& info, char* buf, int& size, int& type)
{
    if(info.three.size > 0)
    {
        memcpy(buf, info.three.last().cards, 3);
        if(info.single.size > 0)
        {
            buf[3] = info.single.last().cards[0];
            type = MDDZ_CT_THREE_WITH_ONE;
            size = 4;
        }
        else if(info.pair.size > 0)
        {
            memcpy(&buf[3], info.pair.last().cards, 2);
            type = MDDZ_CT_THREE_WITH_PAIR;
            size = 5;
        }
        else
        {
            type = MDDZ_CT_THREE;
            size = 3;
        }
        return true;
    }
    return false;
}

bool get_straight_with1(split_info& info, char* buf, int& size, int& type)
{
    if(info.straight.size > 0)
    {
        size = info.straight.last().size;
        type = MDDZ_CT_STRAIGHT;
        memcpy(buf, info.straight.last().cards, size);
        return true;
    }
    if(info.straight2.size > 0)
    {
        size = info.straight2.last().size;
        type = MDDZ_CT_TWO_STRAIGHT;
        memcpy(buf, info.straight2.cards, size);
        return true;
    }
    char cards[20];
    int  len = info.get_cards(cards, 20);
    if(info.straight3.size > 0)
    {
        int n = info.straight3.last().size / 3;
        memcpy(buf, info.straight3.last().cards, info.straight3.last().size);
        if(len >= n*4)
        {
            int nn = n*3;
            for(int i=info.single.size-1; i>=0; i--)
            {
                buf[nn] = info.single[i].cards[0];
                nn ++;
                if(nn >= n*4)
                {
                    type = MDDZ_CT_THREE_STRAIGHT_WITH_N;
                    return true;
                }
            }
            for(int i=info.pair.size-1; i>=0; i--)
            {
                int dn = n*4-nn;
                if(dn>2)dn=2;
                memcpy(&buf[nn], info.pair[i].cards, dn);
                nn += dn;
                if(nn >= n*4)
                {
                    type = MDDZ_CT_THREE_STRAIGHT_WITH_N;
                    return true;
                }
            }
            char remain[20];
            int  rlen = get_remain(cards, len, buf, nn, remain);
            for(int i=rlen-1; i>=0; i--)
            {
                buf[nn] = remain[i]; 
                nn ++;
                if(nn >= n*4)
                {
                    type = MDDZ_CT_THREE_STRAIGHT_WITH_N;
                    return true;
                }
            }
        }
        else
        {
            memcpy(buf, info.straight3.last().cards, 3);
            if(info.single.size>0)
            {
                buf[3] = info.single.last().cards[0];
                size = 4;
                type = MDDZ_CT_THREE_WITH_ONE;
                return true;
            }
            if(info.king.size == 1)
            {
                buf[3] = info.king.cards[0];
                size = 4;
                type = MDDZ_CT_THREE_WITH_ONE;
                return true;
            }
            size = n*3;
            type = MDDZ_CT_THREE_STRAIGHT;
            return true;
        }
        return true;
    }
    return false;
}

bool get_straight(split_info& info, char* buf, int& size, int& type)
{
    if(info.straight.size > 0)
    {
        size = info.straight.last().size;
        type = MDDZ_CT_STRAIGHT;
        memcpy(buf, info.straight.last().cards, size);
        return true;
    }
    if(info.straight2.size > 0)
    {
        size = info.straight2.last().size;
        type = MDDZ_CT_TWO_STRAIGHT;
        memcpy(buf, info.straight2.cards, size);
        return true;
    }
    if(info.straight3.size > 0)
    {
        int n = info.straight3.last().size / 3;
        memcpy(buf, info.straight3.last().cards, info.straight3.last().size);
        if(info.pair.size >= n)
        {
            for(int i=0; i<n; i++)
            {
                memcpy(&buf[info.straight3.last().size+i*2], info.pair[info.pair.size - 1 - i].cards, 2);
            }
            type = MDDZ_CT_THREE_STRAIGHT_WITH_NP;
            size = n * 5;
        }
        else if(info.single.size >= n)
        {
            for(int i=0; i<n; i++)
            {
                buf[info.straight3.last().size+i] = info.single[info.single.size - 1 - i].cards[0];
            }
            type = MDDZ_CT_THREE_STRAIGHT_WITH_N;
            size = n * 4;
        }
        else
        {
            type = MDDZ_CT_THREE_STRAIGHT;
            size = n * 3;
        }
        return true;
    }
    return false;
}

int get_n_three(split_info& info)
{
    int n = info.three.size;
    for(int i=0; i<info.straight3.size; i++)
    {
        n += (info.straight3[i].size / 3);
    }
    return n;
}

bool has_less_single(split_info& split, char v)
{
    if(split.single.size == 0)
        return false;
    int nthree = get_n_three(split);
    int rest_1 = split.single.size - nthree;
    if(rest_1 < 1)
        return false;
    for(int i=0; i<split.single.size-rest_1; i++)
    {
        if(split.single[i].val() < v)
            return true;
    }
    return false;
}

bool has_2(char* cards, int size)
{
    for(int i=0; i<size; i++)
    {
        if(C_N(cards[size]) == CARD_2)
            return true;
        if(C_N(cards[size]) <= CARD_A)
            break;
    }
    return false;
}

bool has_king1(char* cards, int size)
{
    if(C_T(cards[0]) == 0x40)
        return true;
    return false;
}

char get_less_single(char* cards, int size, char v)
{
    return 0;
}

bool can_control_single(split_info& split, char v)
{
    if(split.single.size == 0)
        return true;
    int nthree = get_n_three(split);
    if(nthree >= split.single.size)
        return true;
    for(int i=nthree; i<split.single.size-1; i++)
    {
        if(split.single[i].val() < v)
            return false;
    }
    return true;
}

bool get_bigger_three_with_one(char* cards, int size, char v, char* buf)
{
    if(size<4)
        return false;
    simple_split split;
    cal_simple_split(cards, size, split);
    for(int i=0; i<split.three.size; i++)
    {
        if(split.three[i].val() > v)
        {
            memcpy(buf, split.three[i].cards, 3);
            if(split.single.size>0)
            {
                buf[3] = split.single.last().cards[0];
                return true;
            }
            if(split.pair.size>0)
            {
                buf[3] = split.pair.last().cards[0];
                return true;
            }
            char remain[20];
            int rl = get_remain(cards, size, buf, 3, remain);
            buf[3] = remain[rl-1];
            return true;
        }
    }
    for(int i=0; i<split.bomb.size; i++)
    {
        if(split.bomb[i].val() > v)
        {
            memcpy(buf, split.bomb[i].cards, 3);
            if(split.single.size>0)
            {
                buf[3] = split.single.last().cards[0];
                return true;
            }
            if(split.pair.size>0)
            {
                buf[3] = split.pair.last().cards[0];
                return true;
            }
            char remain[20];
            int rl = get_remain(cards, size, buf, 3, remain);
            buf[3] = remain[rl-1];
            return true;
        }
    }
    return false;
}

bool get_bigger_three_with_pair(char* cards, int size, char v, char* buf)
{
    if(size<5)
        return false;
    simple_split split;
    cal_simple_split(cards, size, split);
    for(int i=0; i<split.three.size; i++)
    {
        if(split.three[i].val() > v)
        {
            memcpy(buf, split.three[i].cards, 3);
            if(split.pair.size > 0)
            {
                memcpy(&buf[3], split.pair.last().cards, 2);
                return true;
            }
            if(split.three.size > 1)
            {
                for(int j=split.three.size-1; j>=0; j--)
                {
                    if(i == j) continue;
                    memcpy(&buf[3], split.three[j].cards, 2);
                    return true;
                }
            }
            if(split.bomb.size>0)
            {
                if(split.bomb.last().val()<split.three[i].val())
                {
                    memcpy(&buf[3], split.bomb.last().cards, 2);
                }
                else
                {
                    memcpy(buf, split.bomb.last().cards, 3);
                    memcpy(&buf[3], split.three[i].cards, 2);
                }
                return true;
            }
        }
    }
    for(int i=0; i<split.bomb.size; i++)
    {
        if(split.bomb[i].val() > v)
        {
            memcpy(buf, split.bomb[i].cards, 3);
            if(split.pair.size > 0)
            {
                memcpy(&buf[3], split.pair.last().cards, 2);
                return true;
            }
            if(split.three.size > 1)
            {
                memcpy(&buf[3], split.three.last().cards, 2);
                return true;
            }
        }
    }
    return false;
}

int get_bigger_straight3_1(split_info& split, char v, int len, char* buf)
{
    int n = len / 4;
    for(int i=split.straight3.size-1; i>=0; i--)
    {
        if(split.straight3[i].val()>v && split.straight3[i].size/3 == n)
        {
            int size = split.straight3[i].size;
            memcpy(buf, split.straight3[i].cards, size);
            if (split.single.size > n)
            {
                for(int j=split.single.size-1; j>=split.single.size-n; j--)
                {
                    buf[size] = split.single[j].cards[0];
                    size ++;
                }
                return len;
            }
            char temp[20];int tlen = 0;
            for(int j=0; j<split.single.size; j++)
            {
                temp[j] = split.single[j].cards[0];
            }
            tlen = split.single.size;
            for(int j=0; j<split.pair.size; j++)
            {
                memcpy(&temp[tlen], split.pair[j].cards, 2);
                tlen += 2;
            }
            if(tlen >= n)
            {
                sort_by_num(temp, tlen);
                memcpy(&buf[size], &temp[tlen-n], n);
                return len;
            }
        }
    }
    return 0;
}

int get_bigger_straight3_2(split_info& split, char v, int len, char* buf)
{
    int n = len / 5;
    for(int i=split.straight3.size-1; i>=0; i--)
    {
        if(split.straight3[i].val()>v && split.straight3[i].size/3 == n)
        {

        }
    }
    return 0;
}

bool can_next_follow_pair(UP_PARAM_DEF)
{
    out_cards_buf last;
    record.get_last(last);
    int farmer = next_id(id);
    int lander = pre_id(id);
    int nthree = get_n_three(split[farmer]);
    char v_st = C_N(last.cards[0]);
    if(v_st >= CARD_2)
        return false;
    if(hand_card[farmer].size>hand_card[id].size && turns[farmer]<turns[id])
        return false;
    if(split[id].pair.size == 0)
        return false;

    int  vi = -1;
    char vc = 0;
    char vn = 0;
    for(int i=split[id].pair.size-1; i>=0; i--)
    {
        if(split[id].pair[i].val() > v_st)
        {
            vc = split[id].pair[i].val();
            vi = i;
            break;
        }
    }
    if(vi<0)
        return false;
    if(split[farmer].pair.size>0)
    {
        for(int i=split[farmer].pair.size-1; i>=0; i--)
        {
            if(split[farmer].pair[i].val() > v_st)
            {
                vn = split[farmer].pair[i].val();
                break;
            }
        }
    }
    if(vn == 0)
    {
        if(split[id].pair[vi].val() == CARD_2)
        {
            if(hand_card[farmer].size > 6 && hand_card[lander].size > 14)
                return false;
        }
        set_out_cards(out, split[id].pair[vi].cards, 2, MDDZ_CT_PAIR);
        return true;
    }
    if(vc>0 && vn>0 && vn>vc)
    {
        set_out_cards(out, split[id].pair[vi].cards, 2, MDDZ_CT_PAIR);
        return true;
    }
    if(split[id].straight.size > 0 || split[id].straight2.size>0 || split[id].straight3.size>0)
    {
        set_out_cards(out, split[id].pair[vi].cards, 2, MDDZ_CT_PAIR);
        return true;
    }
    if(hand_card[id].size < hand_card[farmer].size)
    {
        set_out_cards(out, split[id].pair[vi].cards, 2, MDDZ_CT_PAIR);
        return true;
    }
    return false;
}

bool can_next_follow_single(UP_PARAM_DEF)
{
    out_cards_buf last;
    record.get_last(last);
    int farmer = next_id(id);
    int lander = pre_id(id);
    int nthree = get_n_three(split[farmer]);
    char v_st = C_N(last.cards[0]);
    if(hand_card[id].size < hand_card[farmer].size && hand_card[id].size < hand_card[lander].size)
    {
        for(int i=split[id].single.size-1; i>=0; i--)
        {
            if(split[id].single[i].val() > v_st)
            {
                set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                return true;
            }
        }
    }
    if(v_st >= CARD_2)
        return false;
    if(nthree >= split[farmer].single.size-1 ||
        split[farmer].single.size == 0 || split[farmer].single.first().val()<=v_st)
    {
        for(int i=split[id].single.size-1; i>=0; i--)
        {
            if(split[id].single[i].val() > v_st)
            {
                set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                return true;
            }
        }
    }

    if(hand_card[lander].cards[0] == 0x4e || hand_card[lander].cards[0] == 0x4f)
    {
        if(v_st==CARD_A)
        {
            if(!has_2(hand_card[farmer].cards, hand_card[farmer].size))
            {
                for(int i=0; i<hand_card[id].size; i++)
                {
                    if(C_N(hand_card[id].cards[i]) == CARD_2)
                    {
                        set_out_cards(out, &hand_card[id].cards[i], 1, MDDZ_CT_ONE);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool can_pre_up_single(UP_PARAM_DEF)
{
    out_cards_buf last;
    record.get_last(last);
    int lander = next_id(id);
    int farmer = pre_id(id);
    char v_st = C_N(last.cards[0]);
    int nnthree = get_n_three(split[farmer]);
    nnthree++;
    int restnn = split[farmer].single.size - nnthree;

    if(restnn > 0)
    {//拦住地主顺走小单
        char vstop = split[farmer].single[split[farmer].single.size-restnn].val();
        for(int i=split[id].single.size-1; i>=0; i--)
        {
            if(split[id].single[i].val() > v_st && split[id].single[i].val()>= vstop)
            {
                set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                return true;
            }
        }
    }
    if (v_st < CARD_A && has_2(hand_card[farmer].cards, hand_card[farmer].size))
    {//顶牌，逼地主出2
        if( split[id].bomb.size == 0 ||
            (split[id].bomb.size >0 && split[id].bomb.first().val() != CARD_A))
        {
            for(int i=0; i<hand_card[id].size; i++)
            {
                if(C_N(hand_card[id].cards[i]) == CARD_A)
                {
                    set_out_cards(out, &hand_card[id].cards[i], 1, MDDZ_CT_ONE);
                    return true;
                }
            }
        }
        for(int i=0; i<hand_card[id].size; i++)
        {
            if(C_N(hand_card[id].cards[i])<CARD_2 && C_N(hand_card[id].cards[i]) > v_st)
            {
                set_out_cards(out, &hand_card[id].cards[i], 1, MDDZ_CT_ONE);
                return true;
            }
        }
    }

    if(hand_card[lander].cards[0] == 0x4f &&
        hand_card[farmer].cards[0] == 0x4e || hand_card[id].cards[0] == 0x4e)
    {
        if(C_N(last.cards[0]) == CARD_2)
            return false;
        if (split[id].straight.size > 0 || split[id].straight2.size>0 || split[id].straight3.size>0 || turns[id] < 4)
        {
            char stopv = 0;
            for(int i=0; i<hand_card[lander].size; i++)
            {
                stopv = C_N(hand_card[lander].cards[i]);
                if(stopv > CARD_2)
                    continue;
                break;
            }
            for(int i=0; i<hand_card[id].size; i++)
            {
                char cv = C_N(hand_card[id].cards[i]);
                if(cv>=stopv && cv>v_st)
                {
                    set_out_cards(out, &hand_card[id].cards[i], 1, MDDZ_CT_ONE);
                    return true;
                }
            }
        }
    }
    return false;
}

bool can_hold_all(GET_ROBOT_PARAM_DEF)
{
    return false;
}

bool is_bigger_1_unless_king(char* cards, int size, char v)
{
    for(int i=0; i<size; i++)
    {
        char n = C_N(cards[i]);
        if(n > v)
        {
            if(C_TYPE_KING != C_T(cards[i]))
                return false;
        }
    }
    return true;
}

bool is_bigger_2_unless_king(split_info& split, char v)
{
    for(int i=0; i<split.pair.size; i++)
    {
        if(split.pair[i].val() > v)
            return false;
    }

    return true;
}