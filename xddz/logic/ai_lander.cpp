#include "ddz_ai_strategies.h"
#include "ai_helper.h"

//地主AI简化处理
//////////////////////////////////////////////////////////////////////////
//地主首发
int get_robot_land_first_out(GET_ROBOT_PARAM_DEF)
{
    int next = next_id(id);
    int pre  = pre_id(id);

    return get_robot_normal_first_out(GET_ROBOT_PARAM_USE);
    //////////////////////////////////////////////////////////////////////////
    if(hand_card[next].size>6 && hand_card[pre].size>6)
        return get_robot_normal_first_out(GET_ROBOT_PARAM_USE);
    split_info split[3];
    for(int i=0; i<3; i++)
    {
        split_optimal_cards(hand_card[i].cards, hand_card[i].size, split[i]);
    }

    return 0;
}

//地主跟牌
int get_robot_lander_follow_out(GET_ROBOT_PARAM_FOLLOW_DEF)
{
    out = bigger[0];
    return out.size;
}