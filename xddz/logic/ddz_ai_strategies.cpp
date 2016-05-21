#include "ddz_ai_strategies.h"
#include "mddz_util.h"
#include <string.h>
#include "ai_helper.h"


//通用首发牌
int get_robot_normal_first_out(GET_ROBOT_PARAM_DEF)
{
    out.size = 0;
    split_info split;
    cards_info info;
    int power;
    int turns;
    split_optimal_cards(hand_card[id].cards, hand_card[id].size, split);
    cal_cards_info(hand_card[id].cards, hand_card[id].size, info);
    cal_power(split, power, turns);

    int next_farmer = record.first + 1;
    if(next_farmer>2)next_farmer=0;
    //优先出控牌

    do 
    {
        if(id == next_farmer)
        {
            int nthree = split.three.size;
            for(int i=0; i<split.straight3.size; i++)
            {
                nthree += split.straight3[i].size / 3;
            }

            //计算控牌与小牌
            do 
            {
                if(split.single.size == 0) break;
                if(nthree == 0) break;
                if(nthree < split.single.size)
                {
                    if(split.pair.size>0 && split.single.last().val() < split.pair.last().val())
                    {
                        return set_out_cards(out, split.single.last().cards, 1, MDDZ_CT_ONE);
                    }
                }
            } while (false);
        }

        if(split.straight.size>0)
        {//顺子
            return set_out_cards(out, split.straight.last().cards, split.straight.last().size, MDDZ_CT_STRAIGHT);
        }
        if(split.straight2.size>0)
        {//连对
            return set_out_cards(out, split.straight2.last().cards, split.straight2.last().size, MDDZ_CT_TWO_STRAIGHT);
        }
        if(split.straight3.size>0)
        {//飞机
            char obuf[20];
            int  olen = 0;
            int n = split.straight3.last().size / 3;
            olen = split.straight3.last().get_cards(obuf, 20);
            if(split.pair.size>=n && split.single.size>=n)
            {
                int bigdan = 0;
                int bigshuang = 0;
                int sn = split.single.size - 1;
                int pn = split.pair.size-1;
                for(int i=0; i<n; i++)
                {
                    char dn = split.single[sn-i].val();
                    char sn = split.pair[pn-i].val();
                    if(dn>sn)
                    {
                        bigdan++;
                    }
                    else
                    {
                        bigshuang ++;
                    }
                }
                if(bigdan>bigshuang)
                {
                    int pn = split.pair.size-1;
                    for(int i=0; i<n; i++)
                    {
                        memcpy(&obuf[olen], split.pair[pn-i].cards, 2); olen += 2;
                    }
                    return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_STRAIGHT_WITH_NP);
                }
                else
                {
                    int sn = split.single.size - 1;
                    for(int i=0; i<n; i++)
                    {
                        obuf[olen] = split.single[sn-i].cards[0]; olen += 1;
                    }
                    return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_STRAIGHT_WITH_N);
                }
            }
            if(split.pair.size>=n)
            {
                int pn = split.pair.size-1;
                for(int i=0; i<n; i++)
                {
                    memcpy(&obuf[olen], split.pair[pn-i].cards, 2); olen += 2;
                }
                return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_STRAIGHT_WITH_NP);
            }
            else if(split.single.size>=n)
            {
                int sn = split.single.size - 1;
                for(int i=0; i<n; i++)
                {
                    obuf[olen] = split.single[sn-i].cards[0]; olen += 1;
                }
                return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_STRAIGHT_WITH_N);
            }
            else
            {
                char remain[20];
                int rn = get_remain(hand_card[id].cards, hand_card[id].size, obuf, olen, remain);
                if(rn>=n)
                {
                    memcpy(&obuf[olen], &remain[rn-n], n);
                    olen += n;
                    return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_STRAIGHT_WITH_N);
                }
                else
                {
                    return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_STRAIGHT);
                }
            }
        }
        if(split.three.size>0)
        {//三带x
            char obuf[20];
            int  olen = 0;
            if(split.three.last().val() != CARD_2 ||
                (split.three.last().val() == CARD_2 && hand_card[id].size<6 && split.single.size<2))
            {
                olen = split.three.last().get_cards(obuf, 20);
                if(split.single.size>0 && split.pair.size>0)
                {//谁小带谁
                    if(C_N(split.single.last().cards[0]) < C_N(split.pair.last().cards[0]))
                    {
                        obuf[olen] = split.single[split.single.size-1].cards[0];
                        olen ++;
                        return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_WITH_ONE);
                    }
                    else
                    {
                        memcpy(&obuf[olen], split.pair.last().cards, 2);
                        olen += 2;
                        return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_WITH_PAIR);
                    }
                }
                if(split.single.size>0)
                {//三带1
                    obuf[olen] = split.single.last().cards[0];
                    olen ++;
                    return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_WITH_ONE);
                }
                if(split.pair.size>0)
                {//三带2
                    memcpy(&obuf[olen], split.pair.last().cards, 2);
                    olen += 2;
                    return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_WITH_PAIR);
                }
                {//三条
                    return set_out_cards(out, obuf, olen, MDDZ_CT_THREE);
                }
            }
        }
        if(split.single.size>0 && split.pair.size>0)
        {
            if(split.single.last().val() < split.pair.last().val())
            {
                return set_out_cards(out, split.single.last().cards, 1, MDDZ_CT_ONE);
            }
            else
            {
                return set_out_cards(out, split.pair.last().cards, 2, MDDZ_CT_PAIR);
            }
        }
        if(split.single.size>0)
        {
            return set_out_cards(out, split.single.last().cards, 1, MDDZ_CT_ONE);
        }
        if(split.pair.size>0)
        {
            return set_out_cards(out, split.pair.last().cards, 2, MDDZ_CT_PAIR);
        }
    } while (false);

    if(out.size == 0)
    {
        return set_out_cards(out, &hand_card[id].cards[hand_card[id].size-1], 1, MDDZ_CT_ONE);
    }
    return out.size;
}


//////////////////////////////////////////////////////////////////////////
bool can_all_out(cards_buf& hand_card, int lz, cards_buf& out)
{
    cards_buf buf[MAX_CANDIDATE];
    int n = ddz_get_cards_type_list(hand_card, lz, buf);
    if(n == 0)
        return false;
    for(int i=0; i<n; i++)
    {
        if(buf[i].card_type == MDDZ_CT_BOMB)
        {
            out = buf[i];
            return true;
        }
    }
    out = buf[0];
    return true;
}

int ddzai_get_robot_out_(int id, int lz, cards_buf hand_card[DDZ_SEATS], out_cards& record, cards_buf& out, bool is_robot[3])
{
    memset(&out, 0, sizeof(out));
    out_cards_buf last;
    record.get_last(last);
    if(id == last.id)
        last.id = -1;
    int landid = record.first;
    int nextid = next_id(id);
    int preid = pre_id(id);
    for(int i=0; i<DDZ_SEATS; i++)
    {
        sort_by_num(hand_card[i].cards, hand_card[i].size);
    }
    if (last.id < 0)
    {
        //首发能一把出完则直接出完
        if(can_all_out(hand_card[id], lz, out))
        {
            if(out.card_type == MDDZ_CT_FOUR_WITH_TWO || out.card_type == MDDZ_CT_FOUR_WITH_TWOPAIR)
                out.size = 0;
            else
                return out.size;
        }
    }
    HandAnalyzInfo hai[3];
    for(int i=0; i<3; i++)
    {
        analyze_handcard(hand_card[i].cards, hand_card[i].size, lz, hai[i]);
    }
    if(last.id < 0)
    {
        //首发
        if(id == record.first)
        {
            return get_robot_land_first_out(GET_ROBOT_PARAM_USE);
        }
        else
        {
            if(nextid == landid)
            {
                //地主上家农民先发
                return get_robot_farmer_next_lander(GET_ROBOT_PARAM_USE);
            }
            else
            {
                //地主下家农民先发
                return get_robot_farmer_next_farmar(GET_ROBOT_PARAM_USE);
            }
        }
    }
    else
    {
        //跟牌
        cards_buf bigger[MAX_CANDIDATE];
        cards_buf lastbuf(last.cards, last.size, last.card_type);
        int biglen = ddz_get_bigger_compose(lastbuf, hand_card[id], lz, bigger);
        if(biglen == 0) return 0;
        for(int i=0; i<biglen; i++)
        {
            if(bigger[i].size == hand_card[id].size)
            {
                out = bigger[i];
                return out.size;
            }
        }
        if(bigger[0].card_type == MDDZ_CT_BOMB || bigger[0].card_type == MDDZ_CT_ROCKET)
            return 0;
        if(id == record.first)
        {
            //地主跟牌
            return get_robot_lander_follow_out(GET_ROBOT_PARAM_FOLLOW_USE);
        }
        else
        {
            if(preid == landid)
            {
                //农民跟地主
                return get_robot_farmar_follow_land(GET_ROBOT_PARAM_FOLLOW_USE);
            }
            else
            {
                //农民跟农民
                return get_robot_farmar_folow_farmar(GET_ROBOT_PARAM_FOLLOW_USE);
            }
        }
    }
    return 0;
}

//获取机器人出牌
int ddzai_get_robot_out(int id, int lz, cards_buf hand_card[DDZ_SEATS], out_cards& record, cards_buf& out, bool is_robot[3])
{
#ifdef CARD_A_START
    for (int i=0; i<DDZ_SEATS; i++)
    {
        change_12_2_A2(hand_card[i].cards, hand_card[i].size);
    }
    record.change_12_2_A2();
#endif


    int size = ddzai_get_robot_out_(id, lz, hand_card, record, out, is_robot);

#ifdef CARD_A_START
    for (int i=0; i<DDZ_SEATS; i++)
    {
        change_A2_2_12(hand_card[i].cards, hand_card[i].size);
    }
    record.change_A2_2_12();
    if(size > 0)
        change_A2_2_12(out.cards, size);
#endif
    return size;
}
