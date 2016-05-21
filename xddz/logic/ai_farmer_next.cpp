#include "ddz_ai_strategies.h"
#include <string.h>

//下家农民首发
int get_robot_farmer_next_farmar(GET_ROBOT_PARAM_DEF)
{
    int lander = pre_id(id);
    if(is_robot[lander])
        return get_robot_normal_first_out(GET_ROBOT_PARAM_USE);
    int farmer = next_id(id);
    split_info split[3];
    cards_info info[3];
    int power[3];
    int turns[3];
    char obuf[20];
    int  olen = 0;
    for(int i=0; i<3; i++)
    {
        split_optimal_cards(hand_card[i].cards, hand_card[i].size, split[i]);
        cal_cards_info(hand_card[i].cards, hand_card[i].size, info[i]);
        cal_power(split[i], power[i], turns[i]);
    }

    if(turns[id] == 2)
    {
        if(split[id].single.size>0)
        {
            if(!has_bigger(split[id].single.first().cards, 1, MDDZ_CT_ONE, hand_card[lander].cards, hand_card[lander].size))
            {
                return set_out_cards(out, split[id].single.first().cards, 1, MDDZ_CT_ONE);
            }
        }
        if(split[id].pair.size > 0)
        {
            if(!has_bigger(split[id].pair.first().cards, 2, MDDZ_CT_PAIR, hand_card[lander].cards, hand_card[lander].size))
            {
                return set_out_cards(out, split[id].pair.first().cards, 2, MDDZ_CT_PAIR);
            }
        }
    }
    //下家剩一手牌时，拿牌权放下家+++++
    if(hand_card[farmer].size == 1 && C_N(hand_card[farmer].cards[0]) > C_N(hand_card[id].cards[hand_card[id].size-1]))
    {
        //农民剩单张时，先炸再出单
        if(split[id].king.size == 2)
        {
            return set_out_cards(out, split[id].king.cards, 2, MDDZ_CT_ROCKET);
        }
        if(split[id].bomb.size > 0 && !has_bigger(split[id].bomb.first().cards, 4, MDDZ_CT_BOMB, hand_card[lander].cards, hand_card[farmer].size))
        {
            return set_out_cards(out, split[id].bomb[0].cards, 4, MDDZ_CT_BOMB);
        }
        return  set_out_cards(out, &hand_card[id].cards[hand_card[id].size-1], 1, MDDZ_CT_ONE);
    }

    if(turns[farmer] == 1 && split[farmer].pair.size>0 && has_small_pair(split[farmer].pair[0].val(), info[id], out.cards, true))
    {
        if(split[id].king.size == 2)
        {
            return set_out_cards(out, split[id].king.cards, 2, MDDZ_CT_ROCKET);
        }
        if(split[id].bomb.size > 0 && !has_bigger(split[id].bomb[0].cards, 4, MDDZ_CT_BOMB, hand_card[lander].cards, hand_card[lander].size))
        {
            return set_out_cards(out, split[id].bomb[0].cards, 4, MDDZ_CT_BOMB);
        }
        out.size = 2;
        out.card_type = MDDZ_CT_PAIR;
        return 2;
    }
    if(turns[farmer] == 1 && split[farmer].pair.size>0 && has_small_pair(split[farmer].pair[0].val(), info[id], out.cards, false))
    {
        out.size = 2;
        out.card_type = MDDZ_CT_PAIR;
        return 2;
    }
    if(turns[farmer] == 1 && split[farmer].three.size>0 && split[id].three.size>0 && hand_card[id].size>3)
    {
        if(split[farmer].three[0].val() > split[id].three.last().val())
        {
            memcpy(obuf, split[id].three.last().cards, 3);
            olen = 3;
            char remain[20];
            int n = get_remain(hand_card[id].cards, hand_card[id].size, obuf, olen, remain);
            obuf[3] = remain[n-1];
            olen ++;
            return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_WITH_ONE);
        }
    }
    //下家剩一手牌时，拿牌权放下家-----
    if(hand_card[farmer].size == 2 && split[farmer].single.size == 2)
    {
        if(!has_bigger(hand_card[id].cards, 1, MDDZ_CT_ONE, hand_card[lander].cards, hand_card[lander].size))
        {
            return set_out_cards(out, &hand_card[id].cards[hand_card[id].size-1], 1, MDDZ_CT_ONE);
        }
    }
    //地主剩一手牌,拆散地主，防止地走走掉
    if(turns[lander] == 1)
    {
        //地主剩单张
        if (split[lander].single.size > 0 || split[lander].king.size > 0)
        {
            char landv = C_N(hand_card[lander].cards[0]);
            if(get_straight_with1(split[id], out.cards, out.size, out.card_type))
                return out.size;

            if(can_control_single(split[id], landv))
            {
                if(split[id].pair.size > 0)
                    return set_out_cards(out, split[id].pair.last().cards, 2, MDDZ_CT_PAIR);
                if(split[id].three.size > 0)
                {
                    char obuf[20];
                    memcpy(out.cards, split[id].three.last().cards, 3);
                    if(split[id].single.size > 0)
                    {
                        obuf[3] = split[id].single.last().cards[0];
                        return set_out_cards(out, obuf, 4, MDDZ_CT_THREE_WITH_ONE);
                    }
                }
                if (split[id].bomb.size>0)
                {
                    return set_out_cards(out, split[id].bomb.last().cards, 4, MDDZ_CT_BOMB);
                }
                if(split[id].king.size == 2)
                {
                    return set_out_cards(out, split[id].king.cards, 2, MDDZ_CT_ROCKET);
                }

                return set_out_cards(out, hand_card[id].cards, 1, MDDZ_CT_ONE);
            }
            if(split[id].single.size>0 && C_N(hand_card[farmer].cards[0])>=landv)
            {
                return set_out_cards(out, split[id].single.last().cards, 1, MDDZ_CT_ONE);
            }
            if(split[id].pair.size > 0)
            {
                return set_out_cards(out, split[id].pair.last().cards, 2, MDDZ_CT_PAIR);
            }
            if(get_three_with(split[id], out.cards, out.size, out.card_type))
                return out.size;

            if(has_bigger(hand_card[lander].cards, 1, MDDZ_CT_ONE, hand_card[farmer].cards, hand_card[farmer].size))
            {
                return set_out_cards(out, &hand_card[id].cards[hand_card[id].size-1], 1, MDDZ_CT_ONE);
            }
            else
            {
                return set_out_cards(out, hand_card[id].cards, 1, MDDZ_CT_ONE);
            }
        }
        //地主剩一对
        if(split[lander].pair.size > 0)
        {
            if(get_three_with(split[id], out.cards, out.size, out.card_type))
                return out.size;
            if(get_straight(split[id], out.cards, out.size, out.card_type))
                return out.size;

            if (C_N(hand_card[farmer].cards[0]) > split[lander].pair.first().val() ||
                C_N(hand_card[id].cards[0]) > split[lander].pair.first().val())
            {
                if(split[id].single.size > 0)
                {
                    return set_out_cards(out, split[id].single.last().cards, 1, MDDZ_CT_ONE);
                }
                return set_out_cards(out, &hand_card[id].cards[hand_card[id].size-1], 1, MDDZ_CT_ONE);
            }
        }
        if(split[lander].three.size > 0)
        {
            if(split[id].pair.size>=2)
            {
                if(split[id].pair.first().val() > split[lander].three.first().val())
                {
                    return set_out_cards(out, split[id].pair.last().cards, 2, MDDZ_CT_PAIR);
                }
            }
            if(C_N(hand_card[id].cards[0]) > split[lander].three.first().val())
            {
                if(split[id].single.size > 0)
                {
                    return set_out_cards(out, split[id].single.last().cards, 1, MDDZ_CT_ONE);
                }
            }
        }
    }
    if(hand_card[farmer].size == 2 && split[farmer].single.size == 2)
    {
        if(split[id].single.size > 0)
        {
            return set_out_cards(out, split[id].single.last().cards, 1, MDDZ_CT_ONE);
        }
    }
    if(hand_card[lander].size == 2)
    {
        if(split[lander].single.size > 0)
        {
            if(split[id].single.size>0 && C_N(hand_card[farmer].cards[0]) > C_N(hand_card[lander].cards[0]))
            {
                return set_out_cards(out, split[id].single.last().cards, 1, MDDZ_CT_ONE);
            }
            if(split[id].pair.size>0)
            {
                return set_out_cards(out, split[id].pair.last().cards, 2, MDDZ_CT_PAIR);
            }
            if(get_straight_with1(split[id], out.cards, out.size, out.card_type))
                return out.size;
            if(split[id].three.size>0)
            {
                if(split[id].single.size>0)
                {
                    memcpy(out.cards, split[id].three.last().cards, 3);
                    out.cards[3] = split[id].single.last().cards[0];
                    out.size = 4;
                    out.card_type = MDDZ_CT_THREE_WITH_ONE;
                    return 4;
                }
            }
        }
    }


    //下家农民自己跑路
    if(out.size == 0)
        return get_robot_normal_first_out(GET_ROBOT_PARAM_USE);
    return out.size;
}

//下家农民跟牌
int get_robot_farmar_follow_land(GET_ROBOT_PARAM_FOLLOW_DEF)
{
    out_cards_buf last;
    record.get_last(last);
    int farmer = next_id(id);
    int lander = pre_id(id);
    if(is_robot[lander])
    {
        out = bigger[0];
        return out.size;
    }
    split_info split[3];
    cards_info info[3];
    int power[3];
    int turns[3];
    char obuf[20] = {0};
    int  olen = 0;
    char v_st = C_N(last.cards[0]);
    for(int i=0; i<3; i++)
    {
        split_optimal_cards(hand_card[i].cards, hand_card[i].size, split[i]);
        cal_cards_info(hand_card[i].cards, hand_card[i].size, info[i]);
        cal_power(split[i], power[i], turns[i]);
    }
    if(last.card_type == MDDZ_CT_THREE_STRAIGHT_WITH_N)
    {//飞机带翅膀的处理
        if(last.id == farmer)
            return 0;
        char obuf[20];
        int  olen = get_bigger_straight3_1(split[id], v_st, last.size, obuf);
        if(olen == last.size)
        {
            return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_STRAIGHT_WITH_N);
        }
    }
    
    if(hand_card[id].size == 4 && split[id].pair.size == 2 && last.card_type == MDDZ_CT_PAIR)
    {//地主双王两对
        if(split[id].pair.first().val()>v_st)
        {
            if(!has_bigger(split[id].pair.first().cards, 2, MDDZ_CT_PAIR, hand_card[lander].cards, hand_card[lander].size))
            {
                return set_out_cards(out, split[id].pair.first().cards, 2, MDDZ_CT_PAIR);
            }
        }
    }


    if(hand_card[lander].size == 1 && last.id == farmer)
    {//地主剩下单张，要不起上家农民的单张
        char pv = C_N(hand_card[lander].cards[0]);
        if(can_control_single(split[farmer], pv))
            return 0;
        simple_split ssp;
        cal_simple_split(hand_card[id].cards, hand_card[id].size, ssp);
        if(last.card_type == MDDZ_CT_ONE)
        {
            for(int i=split[id].single.size-1; i>=0; i--)
            {
                if(split[id].single[i].val() > v_st)
                {
                    return set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                }
            }
            for(int i=split[id].pair.size-1; i>=0; i--)
            {
                if(split[id].pair[i].val() > v_st)
                {
                    return set_out_cards(out, split[id].pair[i].cards, 1, MDDZ_CT_ONE);
                }
            }
        }
        else if(last.card_type == MDDZ_CT_PAIR)
        {
            if(can_control_single(split[id], pv))
            {
                out = bigger[0];
                return out.size;
            }
            else
            {
                char maxnv = C_N(hand_card[farmer].cards[0]);
                if(maxnv >= pv && split[id].single.size > 0 && maxnv > split[id].single.last().val())
                {
                    out = bigger[0];
                    return out.size;
                }
                return 0;
            }
        }
        else
        {
            if(can_control_single(split[id], pv))
            {
                out = bigger[0];
                return out.size;
            }
        }
    }////end 地主剩下单张，要不起上家农民的单张

    //最后剩两张，如果能控牌，就出大的++++
    if(last.card_type == MDDZ_CT_ONE)
    {
        //两单，能控出大
        if(hand_card[id].size == 2)
        {
            if(!has_bigger(hand_card[id].cards, 1, MDDZ_CT_ONE, hand_card[lander].cards, hand_card[lander].size))
            {
                return set_out_cards(out, hand_card[id].cards, 1, MDDZ_CT_ONE);
            }
            if(split[id].pair.size > 0)
            {
                if(!has_bigger(split[id].pair.first().cards, 2, MDDZ_CT_PAIR, hand_card[lander].cards, hand_card[lander].size) &&
                    C_N(hand_card[farmer].cards[0]) > v_st &&
                    C_N(hand_card[farmer].cards[0])>= C_N(hand_card[lander].cards[0]) &&
                    split[farmer].pair.size > 0 &&
                    split[farmer].pair.last().val() < split[id].pair.first().val())
                    return 0;
            }
            int bign = 0;
            for(int i=0; i<split[lander].single.size; i++)
            {
                if(split[id].pair.first().val() > split[lander].single[i].val())
                    bign ++;
            }
            if(bign > 2)
            {
                return set_out_cards(out, hand_card[id].cards, 1, MDDZ_CT_ONE);
            }
        }
        //两手，有炸出炸
        if (turns[id] == 2)
        {
            if(split[id].king.size == 2)
            {
                return set_out_cards(out, split[id].king.cards, 2, MDDZ_CT_ROCKET);
            }
            if(split[id].bomb.size > 0)
            {
                if(!has_bigger(split[id].bomb[0].cards, 4, MDDZ_CT_BOMB, hand_card[lander].cards, hand_card[lander].size))
                {
                    return set_out_cards(out, split[id].bomb[0].cards, 4, MDDZ_CT_BOMB);
                }
            }
            //对子能控牌，拆单
            if(split[id].pair.size > 0 && split[id].pair[0].val() > C_N(last.cards[0]))
            {
                if(!has_bigger(split[id].pair[0].cards, 1, MDDZ_CT_ONE, hand_card[lander].cards, hand_card[lander].size))
                {
                    return set_out_cards(out, split[id].pair[0].cards, 1, MDDZ_CT_ONE);
                }
            }
            else if(split[id].single.size>0 && split[id].single.first().val() > v_st)
            {
                return set_out_cards(out, split[id].single.first().cards, 1, MDDZ_CT_ONE);
            }
            if(split[id].king.size > 0 && split[id].king.val() > v_st)
            {
                return set_out_cards(out, split[id].king.cards, 1, MDDZ_CT_ONE);
            }
        }
    }//最后剩两张，如果能控牌，就出大的----
    

    //上家农民剩一手牌++++
    if(turns[farmer] == 1)
    {
        if(split[id].king.size == 2)
        {//上家农民剩一手牌，炸掉出小
            if(split[farmer].single.size>0)
            {
                if(split[farmer].single[0].val() > C_N(hand_card[id].cards[hand_card[id].size-1]))
                {
                    return set_out_cards(out, split[id].king.cards, 2, MDDZ_CT_ROCKET);
                }
            }
            if(has_small_pair(split[farmer].pair.last().val(), info[id], out.cards))
            {
                return set_out_cards(out, split[id].king.cards, 2, MDDZ_CT_ROCKET);
            }
        }
        if(split[id].bomb.size > 0 && !has_bigger(split[id].bomb.last().cards, 4, MDDZ_CT_BOMB, hand_card[lander].cards, hand_card[lander].size))
        {
            if(split[farmer].single.size>0)
            {
                if(split[farmer].single[0].val() > C_N(hand_card[id].cards[hand_card[id].size-1]))
                {
                    return set_out_cards(out, split[id].bomb.last().cards, 4, MDDZ_CT_BOMB);
                }
            }
            if(has_small_pair(split[farmer].pair.last().val(), info[id], out.cards))
            {
                return set_out_cards(out, split[id].bomb.last().cards, 4, MDDZ_CT_BOMB);
            }
        }
        if(farmer == last.id)
            return 0;
        cards_buf c1(last.cards, last.size, last.card_type);
        cards_buf c2(hand_card[farmer].cards, hand_card[farmer].size);
        cards_buf cbuf[MAX_CANDIDATE];
        int n = ddz_get_cards_type_list(c2, 0, cbuf);
        if(n>0)
        {
            c2.card_type = cbuf[0].card_type;
            if(ddz_compare(c1, c2, 0))
                return 0;
        }
        switch (last.card_type)
        {
        case MDDZ_CT_THREE_WITH_ONE:
            {
                char obuf[20];
                if(get_bigger_three_with_one(hand_card[id].cards, hand_card[id].size, v_st, obuf))
                {
                    return set_out_cards(out, obuf, 4, MDDZ_CT_THREE_WITH_ONE);
                }
            }
            break;
        case MDDZ_CT_THREE_WITH_PAIR:
            {
                char obuf[20];
                if(get_bigger_three_with_pair(hand_card[id].cards, hand_card[id].size, v_st, obuf))
                {
                    return set_out_cards(out, obuf, 5, MDDZ_CT_THREE_WITH_PAIR);
                }
            }
            break;
        default:
            break;
        }
        out = bigger[biglen-1];
        return out.size;
    }//上家农民剩一手牌----
    

    //地主剩一手牌
    if(last.id == lander && turns[lander] == 1 && split[lander].bomb.size == 0 && split[lander].king.size != 2)
    {//地主剩非王/炸的最后一手牌，用最大的顶
        if(hand_card[lander].size == 2 && split[id].pair.size > 0 && last.id == lander)
        {//地主出牌，且剩下个对子

        }
        //判断下家是否要得起
        if(split[lander].single.size == 1 || split[lander].king.size>0)
        {
            if(has_bigger(hand_card[lander].cards, 1, MDDZ_CT_ONE, hand_card[farmer].cards, hand_card[farmer].size))
            {
                out = bigger[0];
                return out.size;
            }
            else
            {
                switch (last.card_type)
                {
                case MDDZ_CT_ONE:
                    {
                        return set_out_cards(out, hand_card[id].cards, 1, MDDZ_CT_ONE);
                    }
                case MDDZ_CT_THREE_WITH_ONE:
                case MDDZ_CT_THREE_WITH_PAIR:
                    out = bigger[0];
                    return out.size;
                default:
                    out = bigger[biglen-1];
                    return out.size;
                }
                return out.size;
            }
        }
        if(!has_bigger(last.cards, last.size, last.card_type, hand_card[farmer].cards, hand_card[farmer].size))
        {
            switch (last.card_type)
            {
            case MDDZ_CT_THREE_WITH_ONE:
            case MDDZ_CT_THREE_WITH_PAIR:
                out = bigger[0];
                break;
            default:
                out = bigger[biglen-1];
                break;
            }
            return out.size;
        }
    }//地主剩非王/炸的最后一手牌，用最大的顶
    if(hand_card[farmer].cards[0] == 0x4f && hand_card[farmer].cards[1] == 0x4e && last.id == lander && last.card_type == MDDZ_CT_ONE)
    {
        if (C_N(bigger[0].cards[0]) == CARD_2)
        {
            out = bigger[0];
            return out.size;
        }
    }
    //地主不要++++
    if(last.id == farmer)
    {
        //放下家跑路
        if(turns[farmer] == 1)
            return 0;

        if(hand_card[lander].size == 1)
        {
            if(can_control_single(split[farmer], C_N(hand_card[lander].cards[0])))
                return 0;
            if(bigger[0].card_type == MDDZ_CT_BOMB || bigger[0].card_type == MDDZ_CT_ROCKET)
            {
                if(can_control_single(split[id], C_N(hand_card[lander].cards[0])))
                {
                    out = bigger[0];
                    return out.size;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                out = bigger[0];
                return out.size;
            }
        }

        //不压自己人
        if (C_N(last.cards[0]) >= CARD_A || hand_card[farmer].size < 6)
        {
            return 0;
        }
        switch(last.card_type)
        {
        case MDDZ_CT_ONE:
            {
                if(split[farmer].single.size ==0)
                {
                    if(turns[id] < turns[farmer])
                    {
                        for(int i=split[id].single.size-1; i>=0; i--)
                        {
                            if(split[id].single[i].val() > v_st)
                            {
                                return set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                            }
                        }
                        for(int i=split[id].pair.size-1; i>=0; i--)
                        {
                            if(split[id].pair[i].val() > v_st)
                            {
                                return set_out_cards(out, split[id].pair[i].cards, 1, MDDZ_CT_ONE);
                            }
                        }
                    }
                    return 0;
                }
                for(int i=split[id].single.size-1; i>=0; i--)
                {
                    if(split[id].single[i].val() > v_st)
                    {
                        if (split[id].single[i].val() > split[farmer].single.last().val())
                            return 0;
                        return set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                    }
                }
            }
            break;
        case MDDZ_CT_PAIR:
            {
                if(split[farmer].pair.size == 0)
                {
                    if(turns[id] < turns[farmer])
                    {
                        for(int i=split[id].pair.size-1; i>=0; i--)
                        {
                            if(split[id].pair[i].val() > v_st)
                            {
                                return set_out_cards(out, split[id].pair[i].cards, 2, MDDZ_CT_PAIR);
                            }
                        }
                    }
                    return 0;
                }
                for(int i=split[id].pair.size-1; i>=0; i--)
                {
                    if(split[id].pair[i].val() > C_N(last.cards[0]))
                    {
                        if(split[id].pair[i].val() > split[farmer].pair.last().val())
                            return 0;
                        return set_out_cards(out, split[id].pair[i].cards, 2, MDDZ_CT_PAIR);
                    }
                }
            }
            break;
        case MDDZ_CT_THREE_WITH_ONE:
            {
                //没有单张，或单张过大(大于A)
                if(split[id].single.size <= 0)
                    return 0;
                if(split[id].single.last().val() >= CARD_A)
                    return 0;
                for(int i=split[id].three.size-1; i>=0; i--)
                {
                    if(split[id].three[i].val() > C_N(last.cards[0]))
                    {
                        memcpy(obuf, split[id].three[i].cards, 3);
                        obuf[3] = split[id].single[split[id].single.size-1].cards[0];
                        olen = 4;
                        return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_WITH_ONE);
                    }
                }
            }
            break;
        case MDDZ_CT_STRAIGHT:
            {
                int rest_len = hand_card[id].size - last.size;
                if(rest_len > hand_card[farmer].size)
                    return 0;
                for(int i=split[id].straight.size-1; i>=0; i--)
                {
                    olen = get_mini_bigger_string(last.cards, last.size, split[id].straight[i].cards, split[id].straight[i].size, obuf);
                    if(olen > 0)
                    {
                        return set_out_cards(out, obuf, olen, MDDZ_CT_STRAIGHT);
                    }
                }
            }
            break;
        default:
            break;
        }
        return 0;
    }//地主不要----
    //判断是否要地主牌
    cards_buf next_bigger[MAX_CANDIDATE];
    cards_buf lastbuf(last.cards, last.size, last.card_type);
    int next_biglen = ddz_get_bigger_compose(lastbuf, hand_card[farmer], 0, next_bigger);

    if(next_biglen == 0)
    {//上家农民要不起++++
        if(split[farmer].size < 3)
        {//上家农民牌少，出大+++
            switch(last.card_type)
            {
            case MDDZ_CT_ONE:
                if(split[id].single.size>0 && split[id].single[0].val() > v_st)
                {
                    return set_out_cards(out, split[id].single[0].cards, 1, MDDZ_CT_ONE);
                }
                if(split[id].king.size>0 && C_N(split[id].king.cards[0])>v_st)
                {
                    return set_out_cards(out, split[id].king.cards, 1, MDDZ_CT_ONE);
                }
                break;
            case MDDZ_CT_PAIR:
                {
                    if(split[id].pair.size>0 && split[id].pair[0].val() > v_st)
                    {
                        return set_out_cards(out, split[id].pair[0].cards, 2, MDDZ_CT_PAIR);
                    }
                }
                break;
            case MDDZ_CT_THREE:
                {
                    if(split[id].three.size>0 && split[id].three[0].val()>v_st)
                    {
                        return set_out_cards(out, split[id].three[0].cards, 3, MDDZ_CT_THREE);
                    }
                }
                break;
            default:
                {
                    if(bigger[0].card_type != MDDZ_CT_BOMB && bigger[0].card_type != MDDZ_CT_ROCKET)
                    {
                        out = bigger[0];
                        return out.size;
                    }
                }
                break;
            }
        }//上家农民牌少，出大---
        else
        {
            if(bigger[0].card_type != MDDZ_CT_BOMB && bigger[0].card_type != MDDZ_CT_ROCKET)
            {
                if(bigger[0].cards[0] == 0x4f && hand_card[lander].cards[0] == 0x4e)
                    return 0;
                out = bigger[0];
                return out.size;
            }
        }
    }//上家农民要不起----
    else
    {//上家农民要得起
        switch (hand_card[farmer].size)
        {//判断是否拦截下家
        case 1:
            return 0;
        case 2:
            if(split[id].pair.size > 0 || split[id].king.size == 2)
                return 0;
            break;
        default:
            break;
        }
        if(next_bigger[0].card_type == MDDZ_CT_ROCKET || next_bigger[0].card_type == MDDZ_CT_BOMB)
        {
            out = bigger[0];
            return out.size;
        }
        //判断是否对上家农民是否形成阻挡
        switch(last.card_type)
        {
        case MDDZ_CT_ONE:
            {
                //顶牌策略
                if(can_next_follow_single(UP_PARAM_USE))
                    return out.size;

                int iv = -1;
                char cv = 0;
                if(split[id].single.size>0)
                {
                    for(int i=split[id].single.size-1; i>=0; i--)
                    {
                        cv = split[id].single[i].val();
                        if(cv > v_st)
                        {
                            iv = i;
                            break;
                        }
                    }
                    if(iv >= 0)
                    {
                        if(split[farmer].single.size>0)
                        {
                            if(split[farmer].single.last().val() > cv)
                            {
                                //保留大牌
                                if(cv < CARD_A)
                                {
                                    return set_out_cards(out, split[id].single[iv].cards, 1, MDDZ_CT_ONE);
                                }
                                else
                                    return 0;
                            }
                            else
                            {
                                return set_out_cards(out, split[id].single[iv].cards, 1, MDDZ_CT_ONE);
                            }
                            return 0;//不拦截上家地主
                        }
                        else
                        {
                            //上家牌型炸弹/火箭/拆对子
                            return set_out_cards(out, split[id].single[iv].cards, 1, MDDZ_CT_ONE);
                            return out.size;
                        }
                    }
                }
                return 0;//上家要得起，不拆对，不炸
            }
            break;
        case MDDZ_CT_PAIR:
            {
                if(can_next_follow_pair(UP_PARAM_USE))
                    return out.size;
                if(next_bigger[0].card_type == MDDZ_CT_BOMB || next_bigger[0].card_type == MDDZ_CT_ROCKET)
                {
                    if (bigger[0].card_type != MDDZ_CT_ROCKET && bigger[0].card_type != MDDZ_CT_BOMB)
                    {
                        out = bigger[0];
                        return out.size;
                    }
                }
            }
        default:
            {
                if(next_bigger[0].card_type == MDDZ_CT_BOMB || next_bigger[0].card_type == MDDZ_CT_ROCKET)
                {
                    if (bigger[0].card_type != MDDZ_CT_ROCKET && bigger[0].card_type != MDDZ_CT_BOMB)
                    {
                        out = bigger[0];
                        return out.size;
                    }
                }
            }
            break;
        }
    }
    if (out.size == 0)
    {
        switch (last.card_type)
        {
        case MDDZ_CT_THREE_WITH_ONE:
        case MDDZ_CT_THREE_WITH_PAIR:
            {
                char obuf[10];
                int  olen = 0;
                for(int i=split[id].three.size-1; i>=0; i--)
                {
                    if(split[id].three[i].val() > v_st)
                    {
                        olen = 3;
                        memcpy(obuf, split[id].three[i].cards, 3);
                        break;
                    }
                }
                if(olen == 0)
                    break;
                if(last.card_type == MDDZ_CT_THREE_WITH_ONE)
                {
                    if(split[id].single.size > 0)
                    {
                        obuf[3] = split[id].single.last().cards[0];
                        olen = 4;
                        return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_WITH_ONE);
                    }
                }
                else
                {
                    if(split[id].pair.size > 0)
                    {
                        memcpy(&obuf[3], split[id].pair.last().cards, 2);
                        olen = 5;
                        return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_WITH_PAIR);
                    }
                }
            }
            break;
        default:
            break;
        }
    }
    return out.size;
}
