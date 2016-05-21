#ifndef __GAME_SERVER_CONFIG_MANAGER_H__
#define __GAME_SERVER_CONFIG_MANAGER_H__
#include "singleton.h"
#include "ddz_def.h"
#include "ddzprot.h"
#include <vector>
using namespace std;


struct PropDef
{
	int m_id;//道具ID
	char m_title[32];//标题或者名称
};

typedef vector<PropDef> prop_def_vec;

struct PropRewardItem
{
    int pid;
    int pn;
    int term;
    PropRewardItem()
    {
        pid=0;
        pn=0;
        term=0;
    }
};

struct RankRewardItem
{
    int from;
    int to;
    int coin;
    vector<PropRewardItem> prop;
};

struct RewardConfig
{
    vector<RankRewardItem> reward;
};

struct RuleTurnItem
{
    int less_user;
    int max_user;
    int base;
    int plays;
    int less_point;
    int max_time;
    char desc[32];
};

struct MatchRule
{
    int point;
    vector<RuleTurnItem> turns;
};

struct shuffle_config
{
    int normal_rate;
    int bomb_rate;
    int str_rate;
    int bomb_from;
    int bomb_to;
};

class config_manager : public singleton<config_manager>
{
public:
    config_manager(void);
    ~config_manager(void);
    bool init();
    bool fini();
    bool load_shuffle();
    int get_port();
    int get_max_con();
    int serverid(){return m_serverid;}
    int server_type(){return m_server_type;}
	ddz_game_config& get_game_config();
    access_condition& get_ac();
    glory_def& get_glory();
	count_box_item* get_online_mission(unsigned int levels);
	int get_online_mission_levels(){return m_online_mission.size();}
    int arrange_time(){return m_arrange_time;}
    bool auto_ready(){return m_auto_ready;}
    int update_time(){return m_updatetime;}
    bool anti_cheat(){return m_anti_cheat==1;}
    int win_mission_type(){return m_win_mission_type;}
    int max_win_mission(){return m_win_mission_max;}
    char* redis_win_ip(){return m_redis_win_ip;}
    int redis_win_port(){return m_redis_win_port;}
    int redis_win_db(){return m_redis_win_db;}
    char* redis_win_pwd(){return m_redis_win_pwd;}
    int redis_win_exp(){return m_redis_win_exp;}
	char* get_prop_name(int id);

    char* redis_count_box_ip(){return m_redis_count_box_ip;}
    int   redis_count_box_port(){return m_redis_count_box_port;}
    int   redis_count_box_db(){return m_redis_count_box_db;}
    char* redis_count_box_pwd(){return m_redis_count_box_pwd;}

    bool load_db_config();
    bool login_vip(){return m_login_vip;}

    int  water_num(){return m_water_num;}
    int  water_winner(){return m_water_winner;}
    int  water_vip_times(){return m_water_vip_times;}

    char* msip(){return m_msip;}
    int  msport(){return m_msport;}
    RankRewardItem* get_reward(int rank);
    void ai_deep(int d){m_ai_deep = d;}
    int  ai_deep(){return m_ai_deep;}
    bool is_ai_disturb(){return m_is_ai_disturb;}
    bool is_ai_deep(){return m_is_ai_deep;}

    bool is_ai_auto_supplement(){return m_ai_auto_supplement;}
    int  ai_supplement_from(){return m_ai_supplement_from;}
    int  ai_supplement_to(){return m_ai_supplement_to;}

    bool is_enable_shuffle(){return m_enable_shuffle;}
    shuffle_config& get_shuffle_config() {return m_shuffle_config;}


private:
    bool load_key();
	bool load_prop_def();
    bool load_match_config();
private:
    int m_serverid;
    int m_server_type;
    int m_updatetime;
    access_condition m_ac;
    glory_def  m_glory;
	vector<count_box_item>  m_online_mission;
    int m_port;
    int m_max_con;
	ddz_game_config m_game_config;
    char m_ckey[32];
    char m_tkey[32];

    int  m_arrange_time;
    bool m_auto_ready;
    int  m_anti_cheat;

    int  m_win_mission_type;
    int  m_win_mission_max;

    char m_redis_win_ip[16];
    int  m_redis_win_port;
    int  m_redis_win_db;
    char m_redis_win_pwd[64];
    int  m_redis_win_exp;

    char m_redis_count_box_ip[16];
    int  m_redis_count_box_port;
    int  m_redis_count_box_db;
    char m_redis_count_box_pwd[64];

	prop_def_vec m_prop_def_vec;

    bool m_login_vip;

public:
    int  m_robot_work_limit;
    int  m_water_winner;
    int  m_water_num;
    int  m_water_vip_times;

public:
    int  match_point(){return m_matchrule.point;}
    int  wait_match_start_time(){return m_wait_match_start_time;}
    MatchRule& match_rule(){return m_matchrule;}
private:
    // 比赛配置
    char m_msip[32];
    int  m_msport;
    int  m_wait_match_start_time;
    RewardConfig m_reward;
    vector<RankRewardItem*> m_reward_table;
    MatchRule    m_matchrule;
    int          m_ai_deep;
    bool         m_is_ai_disturb;
    bool         m_is_ai_deep;
    bool         m_ai_auto_supplement;
    int          m_ai_supplement_from;
    int          m_ai_supplement_to;

    //////////////////////////////////////////////////////////////////////////
    //洗牌控制
    bool         m_enable_shuffle;
    shuffle_config m_shuffle_config;
};

#define GAME_MODE_CFG config_manager::instance()->get_game_config().game_mode
#define IS_PK_MODE (GAME_MODE_CFG==GAME_TYPE_PK)
#define IS_MATCH_MODE (GAME_MODE_CFG == GAME_TYPE_MATCH)
#define IS_NORMAL_MODE (GAME_MODE_CFG == GAME_TYPE_NORMAL)
#define cfg_inst config_manager::instance()
#endif//__GAME_SERVER_CONFIG_MANAGER_H__