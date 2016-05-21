#ifndef __DDZ_AI_STRATEGIES_H__
#define __DDZ_AI_STRATEGIES_H__
#include "mddz.h"
#include "ai_helper.h"
//////////////////////////////////////////////////////////////////////////
int ddzai_get_robot_out(int id, int lz, cards_buf hand_card[DDZ_SEATS], out_cards& record, cards_buf& out, bool is_robot[3]);
int ddzai_evaluate_call_point(cards_buf hand_card[DDZ_SEATS], int id, char backcards[3]);


//////////////////////////////////////////////////////////////////////////
//通用出牌策略
int get_robot_normal_first_out(GET_ROBOT_PARAM_DEF);

//地主出牌策略
int get_robot_land_first_out(GET_ROBOT_PARAM_DEF);
int get_robot_lander_follow_out(GET_ROBOT_PARAM_FOLLOW_DEF);

//上家农民出牌策略
int get_robot_farmer_next_lander(GET_ROBOT_PARAM_DEF);
int get_robot_farmar_folow_farmar(GET_ROBOT_PARAM_FOLLOW_DEF);

//下家农民出牌策略
int get_robot_farmer_next_farmar(GET_ROBOT_PARAM_DEF);
int get_robot_farmar_follow_land(GET_ROBOT_PARAM_FOLLOW_DEF);

#endif//__DDZ_AI_STRATEGIES_H__