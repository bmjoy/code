#include "ddz_ai_strategies.h"
#include <string.h>

//上家农民首发
int get_robot_farmer_next_lander(GET_ROBOT_PARAM_DEF)
{
    int lander = next_id(id);
    if(is_robot[lander])
        return get_robot_normal_first_out(GET_ROBOT_PARAM_USE);
    int farmer  = pre_id(id);
    split_info split[3];
    int power[3];
    int turns[3];
    char obuf[20];
    int  olen = 0;
    for(int i=0; i<3; i++)
    {
        split_optimal_cards(hand_card[i].cards, hand_card[i].size, split[i]);
        cal_power(split[i], power[i], turns[i]);
    }

    if(turns[id] == 2)
    {//自己剩两手牌
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
    if(split[lander].king.size == 2)
    {//地主如果有双王
        if(hand_card[lander].size == 4)
        {
            if(split[lander].pair.size > 0)
            {//地主必赢，随便出
                return get_robot_normal_first_out(GET_ROBOT_PARAM_USE);
            }
            //出小对，地主不能接收
            for(int i=split[id].pair.size-1; i>=0; i--)
            {
                if(split[id].pair[i].val() < split[lander].single.first().val())
                {
                    return set_out_cards(out, split[id].pair[i].cards, 2, MDDZ_CT_PAIR);
                }
            }
            if(get_straight(split[id], out.cards, out.size, out.card_type))
                return out.size;
            //出单，地主不能接手
            for(int i=split[id].single.size-1; i>=0; i--)
            {
                if(split[id].single[i].val() > split[lander].single.first().val())
                {
                    return set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                }
            }
        }
        if (hand_card[lander].size == 5)
        {
            if(split[lander].pair.size > 0)
            {
                for(int i=split[id].pair.size-1; i>=0; i--)
                {
                    if(split[id].pair[i].val() >= split[id].pair.maxv())
                    {
                        return set_out_cards(out, split[id].pair[i].cards, 2, MDDZ_CT_PAIR);
                    }
                }
            }
            if(split[lander].single.size>0)
            {
                for(int i=split[id].single.size-1; i>=0; i--)
                {
                    if(split[id].single[i].val() >= split[lander].single.maxv())
                    {
                        set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                    }
                }
            }
        }
    }

    switch(hand_card[lander].size)
    {
    case 1:
        {
            char nv = C_N(hand_card[lander].cards[0]);
            if(get_straight_with1(split[id], out.cards, out.size, out.card_type))
                return out.size;
            if(can_control_single(split[id], nv))
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
            else
            {
                if(split[id].pair.size>0 && split[farmer].pair.size>0 &&
                    split[id].pair.last().val() < split[farmer].pair.first().val())
                {
                    return set_out_cards(out, split[id].pair.last().cards, 2, MDDZ_CT_PAIR);
                }
                if(split[id].three.size>0)
                {
                    return set_out_cards(out, split[id].three.last().cards, 2, MDDZ_CT_PAIR);
                }

                if(split[id].single.size>0)
                {
                    char maxnv = C_N(hand_card[farmer].cards[0]);
                    for(int i=0; i<split[id].single.size; i++)
                    {
                        if(split[id].single[i].val() >= nv && split[id].single[i].val()<maxnv)
                        {
                            return set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                        }
                    }
                }
                if(split[id].pair.size > 0)
                {
                    if(split[id].pair.last().val() < nv)
                    {
                        return set_out_cards(out, split[id].pair.last().cards, 2, MDDZ_CT_PAIR);
                    }
                }
                if(split[id].pair.size > 0)
                {
                    for(int i=0; i<split[id].pair.size; i++)
                    {
                        if(split[id].pair[i].val() == nv)
                        {
                            return set_out_cards(out, split[id].pair[i].cards, 1, MDDZ_CT_ONE);
                        }
                    }
                }
                if(split[id].straight.size>0)
                {//顺子
                    return set_out_cards(out, split[id].straight.last().cards, split[id].straight.last().size, MDDZ_CT_STRAIGHT);
                }
                if(split[lander].king.size > 0)
                {//判断王
                    if(split[id].king.size>0)
                    {
                        if(split[id].king.val()>split[lander].king.val())
                        {
                            return set_out_cards(out, split[id].king.cards, 1, MDDZ_CT_ONE);
                        }
                    }
                }
                if(split[id].three.size > 0)
                {
                    memcpy(obuf, split[id].three.last().cards, 3);
                    if(split[id].single.size>0)
                    {
                        obuf[3] = split[id].single.last().cards[0];
                        return set_out_cards(out, obuf, 4, MDDZ_CT_THREE_WITH_ONE);
                    }
                    else
                    {
                        return set_out_cards(out, obuf, 3, MDDZ_CT_THREE);
                    }
                }
                if(split[id].single.size>0 && split[lander].single.size>0)
                {
                    for(int i=split[id].single.size-1; i>=0; i--)
                    {
                        if(split[id].single.cards[i].val() >= split[lander].single.first().val())
                        {
                            return set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                        }
                    }
                }
                return set_out_cards(out, hand_card[id].cards, 1, MDDZ_CT_ONE);
            }
        }
        break;
    case 2:
        {
            if(split[lander].pair.size > 0)
            {//找大过的对子先出
                if(split[id].pair.size > 0)
                {
                    for(int i=split[id].pair.size-1; i>=0; i--)
                    {
                        if(split[id].pair[i].val() > split[lander].pair[0].val())
                        {
                            return set_out_cards(out, split[id].pair[i].cards, 2, MDDZ_CT_PAIR);
                        }
                    }
                }
                //没找到大于的对子
            }
            if(split[lander].king.size>0)
            {
                if(split[id].pair.size > 0)
                {
                    return set_out_cards(out, split[id].pair.last().cards, 2, MDDZ_CT_PAIR);
                }
                else if (split[id].three.size>0)
                {
                    memcpy(obuf, split[id].three.last().cards, 3);
                    olen = 3;
                    if(split[id].single.size>0)
                    {
                        obuf[3] = split[id].single.last().cards[0];
                        olen = 4;
                        return set_out_cards(out, obuf, olen, MDDZ_CT_THREE_WITH_ONE);
                    }
                    else
                    {
                        return set_out_cards(out, obuf, olen, MDDZ_CT_THREE);
                    }
                }
            }
            if(split[id].single.size > 0 && split[lander].single.size > 0)
            {
                if(split[id].bomb.size > 0 || split[id].single[0].val() > split[lander].single[0].val())
                    return set_out_cards(out, split[id].single.last().cards, 1, MDDZ_CT_ONE);
            }
            if(split[lander].pair.size > 0)
            {
                if(split[id].single.size>0 && split[id].single[0].val() > split[lander].pair[0].val())
                {
                    return set_out_cards(out, split[id].single.last().cards, 1, MDDZ_CT_ONE);
                }
            }
        }
        break;
    case 3:
        {
            if(split[lander].pair.size > 0)
            {
                if(split[id].pair.size>1&&split[id].pair[0].val()>split[lander].pair[0].val())
                {
                    return set_out_cards(out, split[id].pair.last().cards, 2, MDDZ_CT_PAIR);
                }
            }
            else if (split[id].pair.size > 0)
            {
                return set_out_cards(out, split[id].pair.last().cards, 2, MDDZ_CT_PAIR);
            }
        }
        break;
    case 4:
        break;
    }//分析地主牌
    if(out.size == 0)
    {//分析下家农民牌型
        switch(hand_card[farmer].size)
        {
        case 1:
            {
                return set_out_cards(out, &hand_card[id].cards[hand_card[id].size-1], 1, MDDZ_CT_ONE);
            }
            break;
        case 2:
            {
                if(split[farmer].pair.size > 0 && split[id].pair.size > 0 &&
                    split[farmer].pair[0].val() > split[id].pair.last().val())
                {
                    return set_out_cards(out, split[id].pair.last().cards, 2, MDDZ_CT_PAIR);
                }
            }
            break;
        }
    }
    switch (hand_card[id].size)
    {
    case 2:
        {
            if(split[id].single.size==2)
            {
                if(!has_bigger(split[id].single.first().cards, 1, MDDZ_CT_ONE, hand_card[lander].cards, hand_card[lander].size))
                {
                    return set_out_cards(out, split[id].single.first().cards, 1, MDDZ_CT_ONE);
                }
                else
                {
                    return set_out_cards(out, split[id].single.last().cards, 1, MDDZ_CT_ONE);
                }
            }
        }
    case 3:
        {
            if(split[id].single.size>=2)
            {//全单张，出最小
                return set_out_cards(out, split[id].single.last().cards, 1, MDDZ_CT_ONE);
            }
            if(split[id].pair.size>0)
            {//对子带单，或王
                if(!has_bigger(split[id].pair.first().cards, 2, MDDZ_CT_PAIR, hand_card[lander].cards, hand_card[lander].size))
                {//对子最大，出对子
                    return set_out_cards(out, split[id].pair.first().cards, 2, MDDZ_CT_PAIR);
                }
                if (split[id].single.size>0)
                {
                    if(!has_bigger(split[id].single.first().cards, 1, MDDZ_CT_ONE, hand_card[lander].cards, hand_card[lander].size))
                    {//单牌最大出单牌
                        return set_out_cards(out, split[id].single.first().cards, 1, MDDZ_CT_ONE);
                    }
                    if(split[id].single.first().val() < split[id].pair.first().val())
                    {
                        return set_out_cards(out, split[id].single.first().cards, 1, MDDZ_CT_ONE);
                    }
                    else
                    {
                        return set_out_cards(out, split[id].pair.first().cards, 2, MDDZ_CT_PAIR);
                    }
                }
                if(split[id].king.size > 0)
                {
                    if(!has_bigger(split[id].king.cards, 1, MDDZ_CT_ONE, hand_card[lander].cards, hand_card[lander].size))
                    {
                        return set_out_cards(out, split[id].king.cards, 1, MDDZ_CT_ONE);
                    }
                    else
                    {
                        return set_out_cards(out, split[id].pair.first().cards, 2, MDDZ_CT_PAIR);
                    }
                }
            }
            if (split[id].king.size == 2)
            {//火箭带单
                return set_out_cards(out, split[id].single.first().cards, 1, MDDZ_CT_ONE);
            }
        }
        break;
    case 4:
        {
            if(split[id].single.size == 4)
            {//全单出最小
                return set_out_cards(out, split[id].single.last().cards, 1, MDDZ_CT_ONE);
            }
            if(split[id].pair.size == 2)
            {//两对，能大出大，否则出小
                if(!has_bigger(split[id].pair.first().cards, 2, MDDZ_CT_PAIR, hand_card[lander].cards, hand_card[lander].size))
                {
                    return set_out_cards(out, split[id].pair.first().cards, 2, MDDZ_CT_PAIR);
                }
                else
                {
                    return set_out_cards(out, split[id].pair.last().cards, 2, MDDZ_CT_PAIR);
                }
                break;
            }
            if(split[id].pair.size == 1)
            {
                if(split[id].single.size>0)
                {
                    if(split[id].pair.last().val() < split[id].single.last().val())
                    {
                        return set_out_cards(out, split[id].pair.first().cards, 2, MDDZ_CT_PAIR);
                    }
                    else
                    {
                        return set_out_cards(out, split[id].single.last().cards, 1, MDDZ_CT_ONE);
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    if(turns[id]>turns[lander])
    {
        if(split[id].single.size>0)
        {
            if(split[id].three.size == 0 && split[id].straight3.size==0)
            {
                if(C_N(hand_card[id].cards[0]) > C_N(hand_card[lander].cards[0]) ||
                    C_N(hand_card[farmer].cards[0]) > C_N(hand_card[lander].cards[0]))
                {
                    return set_out_cards(out, split[id].single.last().cards, 1, MDDZ_CT_ONE);
                }
            }
        }
        return get_robot_normal_first_out(GET_ROBOT_PARAM_USE);
    }
    cards_buf outt;
    if(get_straight(split[id], outt.cards, outt.size, outt.card_type))
    {
        cards_buf candi(hand_card[lander].cards, hand_card[id].size);
        cards_buf bbbb[MAX_CANDIDATE];
        int n = ddz_get_bigger_compose(outt, candi, 0, bbbb);

        if(n==0 || bbbb[0].card_type == MDDZ_CT_BOMB || bbbb[0].card_type == MDDZ_CT_ROCKET)
        {
            out = outt;
            return out.size;
        }
    }
    if(hand_card[id].size > hand_card[lander].size)
    {
        if(split[lander].single.size > 0)
        {
            for(int i=split[id].single.size-1; i>=0; i--)
            {
                if(split[id].single[i].val()>=split[lander].single.last().val() &&
                    split[id].single[i].val()<=CARD_A)
                {
                    return set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                }
            }
        }
        if(split[lander].pair.size > 0)
        {
            for(int i=split[id].pair.size-1; i>=0; i--)
            {
                if(split[id].pair[i].val() >= split[lander].pair.last().val() &&
                    split[id].pair[i].val() < CARD_A)
                {
                    return set_out_cards(out, split[id].pair[i].cards, 2, MDDZ_CT_PAIR);
                }
            }
        }
    }
    
    if(out.size == 0)
    {
        get_robot_normal_first_out(GET_ROBOT_PARAM_USE);
        switch (out.card_type)
        {
        case MDDZ_CT_THREE_WITH_ONE:
            break;
        case MDDZ_CT_THREE_WITH_PAIR:
            break;
        default:
            break;
        }
    }
    return out.size;
}

//上家农民跟牌
int get_robot_farmar_folow_farmar(GET_ROBOT_PARAM_FOLLOW_DEF)
{
    out_cards_buf last;
    record.get_last(last);
    int lander = next_id(id);
    int farmer = pre_id(id);
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
    for(int i=0; i<3; i++)
    {
        split_optimal_cards(hand_card[i].cards, hand_card[i].size, split[i]);
        cal_cards_info(hand_card[i].cards, hand_card[i].size, info[i]);
        cal_power(split[i], power[i], turns[i]);
    }
    //最后剩两张，如果能控牌，就出大的
    char v_st = C_N(last.cards[0]);

    if(last.card_type == MDDZ_CT_THREE_STRAIGHT_WITH_N)
    {
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
    {
        if(split[id].pair.first().val()>v_st)
        {
            if(!has_bigger(split[id].pair.first().cards, 2, MDDZ_CT_PAIR, hand_card[lander].cards, hand_card[lander].size))
            {
                return set_out_cards(out, split[id].pair.first().cards, 2, MDDZ_CT_PAIR);
            }
        }
    }
    if(split[lander].king.size == 2)
    {
        if (hand_card[lander].size == 4)
        {
            if(split[lander].pair.size > 0)
            {
                if(bigger[0].card_type == MDDZ_CT_BOMB)
                    return 0;
                out = bigger[0];
                return out.size;
            }
            if(last.card_type == MDDZ_CT_ONE)
            {
                for(int i=split[id].single.size-1; i>=0; i--)
                {
                    if(split[id].single[i].val() > split[lander].single.first().val() &&
                        split[id].single[i].val() > v_st)
                    {
                        return set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                    }
                }
                if(C_N(hand_card[id].cards[0]) > v_st)
                {
                    return set_out_cards(out, hand_card[id].cards, 1, MDDZ_CT_ONE);
                }
            }
        }
        if (hand_card[lander].size == 5)
        {
            if(split[id].three.size > 0)
            {
                if(bigger[0].card_type == MDDZ_CT_BOMB)
                    return 0;
                out = bigger[0];
                return out.size;
            }
            if(last.card_type == MDDZ_CT_ONE)
            {
                if(split[lander].single.size > 0)
                {
                    for(int i=split[id].single.size-1; i>=0; i--)
                    {
                        if(split[id].single[i].val()>v_st && split[id].single[i].val()>=split[lander].single.first().val())
                        {
                            return set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                        }
                    }
                    for(int i=split[id].pair.size-1; i>=0; i--)
                    {
                        if(split[id].pair[i].val()>v_st && split[id].pair[i].val()>=split[lander].single.first().val())
                        {
                            return set_out_cards(out, split[id].pair[i].cards, 1, MDDZ_CT_ONE);
                        }
                    }
                }
                if(C_N(hand_card[id].cards[0])>v_st)
                {
                    return set_out_cards(out, hand_card[id].cards, 1, MDDZ_CT_ONE);
                }
            }
            if(last.card_type == MDDZ_CT_PAIR)
            {
                if(split[lander].pair.size>0)
                {
                    for(int i=split[id].pair.size-1; i>=0; i--)
                    {
                        if(split[id].pair[i].val() > v_st && split[id].pair[i].val()>=split[id].pair.maxv())
                        {
                            set_out_cards(out, split[id].pair[i].cards, 2, MDDZ_CT_PAIR);
                        }
                    }
                }
            }
        }
    }

    if(last.card_type == MDDZ_CT_ONE)
    {
        //两单，能控出大
        if(hand_card[id].size == 2)
        {
            if(!has_bigger(hand_card[id].cards, 1, MDDZ_CT_ONE, hand_card[lander].cards, hand_card[lander].size))
            {
                set_out_cards(out, hand_card[id].cards, 1, MDDZ_CT_ONE);
                return 1;
            }
        }
        //两手，有炸出炸
        if (turns[id] == 2)
        {
            if(split[id].king.size == 2)
            {
                set_out_cards(out, split[id].king.cards, 2, MDDZ_CT_ROCKET);
                return 2;
            }
            if(split[id].bomb.size > 0)
            {
                if(!has_bigger(split[id].bomb[0].cards, 4, MDDZ_CT_BOMB, hand_card[lander].cards, hand_card[lander].size))
                {
                    set_out_cards(out, split[id].bomb[0].cards, 4, MDDZ_CT_BOMB);
                    return 4;
                }
            }
            //对子能控牌，拆单
            if(split[id].pair.size > 0 && split[id].pair[0].val() > C_N(last.cards[0]))
            {
                if(!has_bigger(split[id].pair[0].cards, 1, MDDZ_CT_ONE, hand_card[lander].cards, hand_card[lander].size))
                {
                    set_out_cards(out, split[id].pair[0].cards, 1, MDDZ_CT_ONE);
                    return 1;
                }
            }
        }
    }
    if(hand_card[lander].size == 1)
    {
        char nv = C_N(hand_card[lander].cards[0]);
        if(last.id == farmer)
        {
            //下家农民可控牌出完，放
            if(!has_bigger(last.cards, last.size, last.card_type, hand_card[lander].cards, hand_card[lander].size) &&
                can_control_single(split[farmer], nv))
                return 0;
            if(last.card_type == MDDZ_CT_ONE)
            {
                if (nv > v_st)
                {
                    for(int i=split[id].single.size-1; i>=0; i--)
                    {
                        if(split[id].single[i].val() >= nv)
                        {
                            set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                            return 1;
                        }
                    }
                    for(int i=split[id].pair.size-1; i>=0; i--)
                    {
                        if(split[id].pair[i].val() >= nv)
                        {
                            set_out_cards(out, split[id].pair[i].cards, 1, MDDZ_CT_ONE);
                            return 1;
                        }
                    }
                    if(split[id].king.size==1)
                    {
                        set_out_cards(out, split[id].king.cards, 1, MDDZ_CT_ONE);
                        return 1;
                    }
                    if(split[id].king.size == 2 && can_control_single(split[id], nv))
                    {
                        set_out_cards(out, split[id].king.cards, 2, MDDZ_CT_ROCKET);
                        return 2;
                    }
                    if(split[id].bomb.size>0 && can_control_single(split[id], nv))
                    {
                        set_out_cards(out, split[id].bomb.last().cards, 4, MDDZ_CT_BOMB);
                        return 4;
                    }
                    if(C_N(hand_card[id].cards[0]) > v_st)
                    {
                        set_out_cards(out, hand_card[id].cards, 1, MDDZ_CT_ONE);
                        return 1;
                    }
                    if(bigger[0].card_type != MDDZ_CT_BOMB)
                    {
                        out = bigger[0];
                        return out.size;
                    }
                    return 0;//必输
                }
                else
                {
                    if(can_control_single(split[id], nv))
                    {
                        out = bigger[0];
                        return out.size;
                    }
                    if(split[id].pair.size > 0 && split[farmer].pair.size > 0 &&
                        split[id].pair.last().val() < split[farmer].pair.first().val())
                    {
                        out = bigger[0];
                        return out.size;
                    }
                    return 0;//放下家农民继续出牌
                }
            }
            else if(last.card_type == MDDZ_CT_PAIR)
            {
                if(can_control_single(split[id], nv))
                {
                    out = bigger[0];
                    return out.size;
                }
                char maxmv = C_N(hand_card[id].cards[0]);
                if(maxmv>nv && split[farmer].single.size>0 && maxmv > split[farmer].single.last().val())
                    return 0;
            }
            else
            {//下家农民出其他牌型
                if(can_control_single(split[id], nv))
                {
                    out = bigger[0];
                    return out.size;
                }
                return 0;
            }
        }
        else
        {//下家农民不要
            if(last.card_type == MDDZ_CT_ONE)
            {
                for(int i=split[id].single.size-1; i>=0; i--)
                {
                    if(split[id].single[i].val() > v_st && split[id].single[i].val() >= nv)
                    {
                        return set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                    }
                }
                for(int i=split[id].pair.size-1; i>=0; i--)
                {
                    if(split[id].pair[i].val()>v_st && split[id].pair[i].val() >= nv)
                    {
                        return set_out_cards(out, split[id].pair[i].cards, 1, MDDZ_CT_ONE);
                    }
                }
                for(int i=split[id].three.size-1; i>=0; i--)
                {
                    if(split[id].three[i].val() > v_st)
                    {
                        return set_out_cards(out, split[id].three[i].cards, 1, MDDZ_CT_ONE);
                    }
                }
                if(C_N(hand_card[id].cards[0]) > v_st && C_N(hand_card[id].cards[0]) > nv)
                {
                    //检查炸弹，火箭
                    return set_out_cards(out, hand_card[id].cards, 1, MDDZ_CT_ONE);
                }
                if(bigger[0].card_type == MDDZ_CT_BOMB)
                {
                    //判断炸弹是否可出
                }
                out = bigger[0];
                return out.size;
            }
            else
            {
                out = bigger[0];
                return out.size;
            }
        }
    }//地主剩一张牌

    if(farmer == last.id)
    {//下家农民出牌+++
        //if(!has_bigger(bigger[0].cards, bigger[0].size, bigger[0].card_type, hand_card[lander].cards, hand_card[lander].size) &&
        //    has_bigger(last.cards, last.size, last.card_type, hand_card[lander].cards, hand_card[lander].size))
        //    return 0;
        if(bigger[0].card_type == MDDZ_CT_BOMB)
            return 0;
        char lv = C_N(last.cards[0]);
        char nv = C_N(hand_card[lander].cards[0]);
        if(!has_bigger(last.cards, last.size, last.card_type, hand_card[lander].cards, hand_card[lander].size))
        {//地主要不起下家农民的牌
            if(turns[id] == 2)
            {
                out = bigger[0];
                return out.size;
            }
            if(hand_card[lander].size == 1 && has_less_single(split[farmer], nv))
            {
                out = bigger[0];
                return out.size;
            }
            return 0;
        }
        if(hand_card[lander].size == 1)
        {//last.type=ONE
            if(!has_bigger(hand_card[lander].cards, 1, MDDZ_CT_ONE, hand_card[id].cards, hand_card[id].size))
            {
                //拦不住地主
                out = bigger[0];
                return out.size;
            }
            for(int i=split[id].single.size-1; i>=0; i--)
            {
                if(split[id].single[i].val()>lv &&
                    split[id].single[i].val()>nv)
                {
                    return set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                }
            }
            return set_out_cards(out, hand_card[id].cards, 1, MDDZ_CT_ONE);
        }
        //顶牌为主
        if(last.card_type == MDDZ_CT_ONE)
        {
            if(last.id == farmer && is_bigger_1_unless_king(hand_card[lander].cards, hand_card[lander].size, v_st))
                return 0;
            //顶牌策略
            if(can_pre_up_single(UP_PARAM_USE))
                return out.size;
            for(int i=split[id].single.size-1; i>=0; i--)
            {
                if(split[id].single[i].val() > v_st)
                {
                    return set_out_cards(out, split[id].single[i].cards, 1, MDDZ_CT_ONE);
                }
            }
        }
        if(last.card_type == MDDZ_CT_PAIR)
        {//顶牌策略
            if(v_st >= CARD_A)
                return 0;
            if( split[lander].pair.size == 0)
                return 0;
            for(int i=split[id].pair.size-1; i>=0; i--)
            {
                if(split[id].pair[i].val()>v_st)
                {
                    if(split[id].pair[i].val()>split[lander].pair.last().val())
                    {
                        if(split[id].pair[i].val()>split[lander].pair.first().val())
                            return 0;
                        if(split[id].pair[i].val() == CARD_2)
                            return 0;
                        return set_out_cards(out, split[id].pair[i].cards, 2, MDDZ_CT_PAIR);
                    }
                }
            }
        }
        //顺牌为主
        if(bigger[0].card_type == MDDZ_CT_BOMB)
            return 0;
        if(bigger[0].cards[0] == 0x4f && hand_card[lander].cards[0] == 0x4e)
            return 0;
        if(hand_card[lander].size>14 && bigger[0].card_type == MDDZ_CT_ONE && C_N(bigger[0].cards[0])>=CARD_2)
            return 0;
        if(bigger[0].card_type == MDDZ_CT_BOMB || bigger[0].card_type == MDDZ_CT_ROCKET)
            return 0;
        out = bigger[0];
        return out.size;
    }//下家农民出牌---
    //下家农民不要
    switch (last.card_type)
    {
    case MDDZ_CT_ONE:
        if(bigger[0].cards[0] == 0x4f && split[lander].king.size>0)
        {
            if(hand_card[id].size > 6 && hand_card[lander].size > 6)
                return 0;
        }
        break;
    case MDDZ_CT_PAIR:
        {
            if (C_N(bigger[0].cards[0]) == CARD_2)
            {
                if(hand_card[id].size<hand_card[lander].size ||
                    turns[id] < turns[lander])
                {
                    out = bigger[0];
                    return out.size;
                }
                if (hand_card[lander].size > 12)
                {
                    return 0;
                }
            }
            for(int i=split[id].pair.size-1; i>=0; i--)
            {
                if(split[id].pair[i].val() > v_st)
                {
                    return set_out_cards(out, split[id].pair[i].cards, 2, MDDZ_CT_PAIR);
                }
            }
            if(turns[lander] > 5)
                return 0;
        }
    default:
        break;
    }
    if(out.size == 0)
    {
        if(bigger[0].card_type == MDDZ_CT_ROCKET)
        {
            if(turns[id]>turns[lander])
                return 0;
            int n = hand_card[id].size - 2;
            char tmp[20];
            memcpy(tmp, &hand_card[id].cards[2], hand_card[id].size-2);
            if(n<hand_card[lander].size) n = hand_card[lander].size;
            int nbig = 0;
            int nsmall= 0;
            for(int i=0; i<n; i++)
            {
                if(C_N(tmp[i]) > C_N(hand_card[lander].cards[i]))
                    nbig ++;
                else
                    nsmall ++;
            }
            if(nsmall>nbig)
                return 0;
        }
        if (bigger[0].card_type == MDDZ_CT_BOMB)
        {
            if(turns[id]>turns[lander])
                return 0;
            int n = hand_card[id].size - 4;
            char tmp[20];
            get_remain(hand_card[id].cards, hand_card[id].size, bigger[0].cards, 4, tmp);
            if(n<hand_card[lander].size) n = hand_card[lander].size;
            int nbig = 0;
            int nsmall= 0;
            for(int i=0; i<n; i++)
            {
                if(C_N(tmp[i]) > C_N(hand_card[lander].cards[i]))
                    nbig ++;
                else
                    nsmall ++;
            }
            if(nsmall>nbig)
                return 0;
        }
        out = bigger[0];
        return out.size;
    }

    return out.size;
}

