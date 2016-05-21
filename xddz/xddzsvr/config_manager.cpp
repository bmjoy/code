#include "config_manager.h"
#include "tinyxml_tool.h"
#include "cflog.h"
#include "token2.h"
#include "db_con_manager.h"
#include "record.h"
#include "db_thread.h"
#include "ddzprot.h"

#pragma warning(disable:4996)

config_manager::config_manager(void)
{
    m_ac.less_coin = 0;
    m_ac.max_coin = 0;
    m_serverid = 0;
    m_updatetime = 0;
    memset(&m_glory, 0, sizeof(m_glory));
    memset(&m_game_config, 0, sizeof(m_game_config));
    memset(&m_ac, 0, sizeof(m_ac));
    m_robot_work_limit = 900;
    m_login_vip = false;
    m_water_num = 0;
    m_water_winner = 0;
    m_enable_shuffle = false;
    memset(&m_shuffle_config, 0, sizeof(m_shuffle_config));
    m_water_vip_times = 1;
}

config_manager::~config_manager(void)
{
}

bool config_manager::init()
{
    TiXmlDocument xmlDoc;
    TiXmlElement* pElmRoot;
    TiXmlElement* pElmItem;
	if(!load_prop_def())
	{
		LOG_ERROR("prop_def config fail");
		return false;
	}
    if(!xmlDoc.LoadFile("server_config.xml"))
        goto logic_config_error;
    pElmRoot = xmlDoc.FirstChildElement("root");
    if(!pElmRoot)
        goto logic_config_error;
    pElmItem = pElmRoot->FirstChildElement("config");
    if(!pElmItem)
        goto logic_config_error;
    if( !tinyxml_tool::get_int_attribute(pElmItem, "mode", m_game_config.game_mode) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "lz", m_game_config.surport_lz) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "call_time", m_game_config.call_time) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "pass_time", m_game_config.pass_time) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "first_out_time", m_game_config.first_out_time) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "out_time", m_game_config.out_time) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "auto_ready", m_game_config.auto_ready)||
		!tinyxml_tool::get_int_attribute(pElmItem, "ready_time",m_game_config.ready_time) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "tax", m_game_config.tax) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "anticheat", m_anti_cheat) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "jipaiqi", m_game_config.jipaiqi) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "renshu", m_game_config.renshu))
        goto logic_config_error;
    pElmItem = pElmRoot->FirstChildElement("arrange");
    if(!pElmItem ||
        !tinyxml_tool::get_int_attribute(pElmItem, "time", m_arrange_time))
        goto logic_config_error;
    //pElmItem = pElmRoot->FirstChildElement("bankrupt");
    //if(!pElmItem ||
    //    !tinyxml_tool::get_int_attribute(pElmItem, "max", m_bankrupt_count) ||
    //    !tinyxml_tool::get_int64_attribute(pElmItem, "coin", m_bankrupt_coin) ||
    //    !tinyxml_tool::get_int_attribute(pElmItem, "less", m_bankrupt_less))
    //    goto server_config_error;

    pElmItem = pElmRoot->FirstChildElement("win_mission");
    if(!pElmItem ||
        !tinyxml_tool::get_int_attribute(pElmItem, "type", m_win_mission_type) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "max", m_win_mission_max) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "exp", m_redis_win_exp))
        goto server_config_error;
    if(m_redis_win_exp < 1)
        m_redis_win_exp = 1;

    pElmItem = pElmRoot->FirstChildElement("socket");
    if(!pElmItem)
        goto server_config_error;
    if(!tinyxml_tool::get_int_attribute(pElmItem, "max_con", m_max_con) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "serverid", m_serverid) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "updatetime", m_updatetime))
        goto server_config_error;
    if(m_arrange_time<3)
        m_arrange_time=3;
    if(m_updatetime<10)
        m_updatetime = 10;
    if(m_updatetime>180)
        m_updatetime = 180;
    pElmItem = pElmRoot->FirstChildElement("redis_win");
    if(!pElmItem ||
        !tinyxml_tool::get_str_attribute(pElmItem, "ip", m_redis_win_ip, sizeof(m_redis_win_ip)) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "port", m_redis_win_port) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "db", m_redis_win_db) ||
        !tinyxml_tool::get_str_attribute(pElmItem, "pwd", m_redis_win_pwd, sizeof(m_redis_win_pwd)))
        goto server_config_error;

    pElmItem = pElmRoot->FirstChildElement("redis_count_box");
    if(!pElmItem ||
        !tinyxml_tool::get_str_attribute(pElmItem, "ip", m_redis_count_box_ip, sizeof(m_redis_count_box_ip)) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "port", m_redis_count_box_port) ||
        !tinyxml_tool::get_int_attribute(pElmItem, "db", m_redis_count_box_db) ||
        !tinyxml_tool::get_str_attribute(pElmItem, "pwd", m_redis_count_box_pwd, sizeof(m_redis_count_box_pwd)))
        goto server_config_error;

    pElmItem = pElmRoot->FirstChildElement("vip_login");
    {
        if(pElmItem)
        {
            int v = 0;
            if(tinyxml_tool::get_int_attribute(pElmItem, "value", v))
                m_login_vip = v==0?false:true;
        }
    }

    pElmItem = pElmRoot->FirstChildElement("water_configer");
    if(pElmItem)
    {
        if(!tinyxml_tool::get_int_attribute(pElmItem, "winer", m_water_winner) ||
            !tinyxml_tool::get_int_attribute(pElmItem, "num", m_water_num))
        {
            m_water_num = 0;
            m_water_winner = 0;
        }
        if(!tinyxml_tool::get_int_attribute(pElmItem, "water_vip_times", m_water_vip_times))
            m_water_vip_times = 1;
    }
    m_is_ai_deep = true;
    m_is_ai_disturb = false;
    pElmItem = pElmRoot->FirstChildElement("ai");
    if(pElmItem)
    {
        int disturb = 0;
        int is_deep = 1;
        if(!tinyxml_tool::get_int_attribute(pElmItem, "disturb", disturb) ||
            !tinyxml_tool::get_int_attribute(pElmItem, "is_deep", is_deep))
        {
            m_is_ai_deep = (is_deep == 1);
            m_is_ai_disturb = (disturb == 1);
        }
        else
        {
            m_is_ai_deep = true;
            m_is_ai_disturb = false;
        }
    }

    if(!load_key())
        goto key_load_error;
    init_token2(m_ckey, m_tkey);


	m_online_mission.clear();
    pElmItem = pElmRoot->FirstChildElement("timer_box");
    if(!pElmItem)
        goto online_mission_err;
    pElmItem = pElmItem->FirstChildElement("item");
    if(!pElmItem)
        goto online_mission_err;
    while(pElmItem)
    {
        count_box_item olm;
        if(!tinyxml_tool::get_int_attribute(pElmItem, "t", olm.t) ||
            !tinyxml_tool::get_int_attribute(pElmItem, "pid", olm.pid) ||
            !tinyxml_tool::get_int_attribute(pElmItem, "pn", olm.pn) ||
            !tinyxml_tool::get_int64_attribute(pElmItem, "coin", olm.coin) ||
            !tinyxml_tool::get_str_attribute(pElmItem, "desc", olm.desc,sizeof(olm.desc)))
            goto online_mission_err;
        pElmItem = pElmItem->NextSiblingElement("item");
        m_online_mission.push_back(olm);
    }

    if(m_game_config.game_mode == GAME_TYPE_MATCH)
    {
        if(!load_match_config())
        {
            LOG_ERROR("load match config error.");
            return false;
        }
    }

    pElmItem = pElmRoot->FirstChildElement("ai_auto_coin");
    if(pElmItem)
    {
        int enable = 0;
        if(tinyxml_tool::get_int_attribute(pElmItem, "enable", enable) &&
           tinyxml_tool::get_int_attribute(pElmItem, "from", m_ai_supplement_from) &&
           tinyxml_tool::get_int_attribute(pElmItem, "to", m_ai_supplement_to))
        {
            m_ai_auto_supplement = enable==1?true:false;
        }
        else
        {
            m_ai_auto_supplement = false;
            m_ai_supplement_from = 0;
            m_ai_supplement_to   = 0;
        }
    }
    else
    {
        m_ai_auto_supplement = false;
        m_ai_supplement_from = 0;
        m_ai_supplement_to   = 0;
    }
    m_enable_shuffle = false;
    do 
    {
        pElmItem = pElmRoot->FirstChildElement("shuffle");
        if(!pElmItem)
            break;
        int enable = 0;
        if(!tinyxml_tool::get_int_attribute(pElmItem, "enable", enable) || enable!=1)
            break;
        TiXmlElement* pElmRate = pElmItem->FirstChildElement("rate");
        if(!pElmRate) break;
        if(!tinyxml_tool::get_int_attribute(pElmRate, "normal", m_shuffle_config.normal_rate) ||
            !tinyxml_tool::get_int_attribute(pElmRate, "bomb", m_shuffle_config.bomb_rate) ||
            !tinyxml_tool::get_int_attribute(pElmRate, "straight", m_shuffle_config.str_rate))
            break;
        TiXmlElement* pElmBomb = pElmItem->FirstChildElement("bomb");
        if(!pElmBomb ||
            !tinyxml_tool::get_int_attribute(pElmBomb, "from", m_shuffle_config.bomb_from) ||
            !tinyxml_tool::get_int_attribute(pElmBomb, "to", m_shuffle_config.bomb_to))
            break;

        m_enable_shuffle = true;
    } while (false);

    return true;
server_config_error:
    LOG_ERROR("server_config.xml 加载失败");
    return false;
logic_config_error:
    LOG_ERROR("ddz_logic_config.xml 加载失败");
    return false;
key_load_error:
    LOG_ERROR("load_key() ERROR");
    return false;
online_mission_err:
    LOG_ERROR("计时宝箱配置加载失败");
	return false;
}

bool config_manager::load_shuffle()
{
    TiXmlDocument xmlDoc;
    TiXmlElement* pElmRoot;
    TiXmlElement* pElmItem;
    if(!xmlDoc.LoadFile("server_config.xml"))
        return false;
    pElmRoot = xmlDoc.FirstChildElement("root");
    if(!pElmRoot)
        return false;
    do 
    {
        pElmItem = pElmRoot->FirstChildElement("shuffle");
        if(!pElmItem)
            break;
        int enable = 0;
        if(!tinyxml_tool::get_int_attribute(pElmItem, "enable", enable) || enable!=1)
            break;
        TiXmlElement* pElmRate = pElmItem->FirstChildElement("rate");
        if(!pElmRate) break;
        if(!tinyxml_tool::get_int_attribute(pElmRate, "normal", m_shuffle_config.normal_rate) ||
            !tinyxml_tool::get_int_attribute(pElmRate, "bomb", m_shuffle_config.bomb_rate) ||
            !tinyxml_tool::get_int_attribute(pElmRate, "straight", m_shuffle_config.str_rate))
            break;
        TiXmlElement* pElmBomb = pElmItem->FirstChildElement("bomb");
        if(!pElmBomb ||
            !tinyxml_tool::get_int_attribute(pElmBomb, "from", m_shuffle_config.bomb_from) ||
            !tinyxml_tool::get_int_attribute(pElmBomb, "to", m_shuffle_config.bomb_to))
            break;

        m_enable_shuffle = true;
    } while (false);


    return true;
}

bool config_manager::load_key()
{
    FILE* file = 0;
    file = fopen("token_key.ini", "r+b");
    if(!file)
        return false;
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* p = new char[size+1];
    fread(p, size, 1, file);
    p[size] = 0;
    fclose(file);

    for(int i=0; i<size; i++)
    {
        if(p[i]=='c' && p[i+1]=='k' && p[i+2] == 'e' && p[i+3] == 'y' && p[i+4] == '=')
        {
            if((i+5+16) > size)
            {
                delete p;
                return false;
            }
            memcpy(m_ckey, &p[i+5], 16);
            break;
        }
    }

    for(int i=0; i<size; i++)
    {
        if(p[i]=='t' && p[i+1]=='k' && p[i+2] == 'e' && p[i+3] == 'y' && p[i+4] == '=')
        {
            if((i+5+8) > size)
            {
                delete p;
                return false;
            }
            memcpy(m_tkey, &p[i+5], 8);
            break;
        }
    }

    delete p;
    return true;
}

bool config_manager::load_prop_def()
{
	m_prop_def_vec.clear();
	TiXmlDocument xmlDoc;
	TiXmlElement* pElmRoot;
	TiXmlElement* pElmItem;
	if(!xmlDoc.LoadFile("prop_def.xml"))
		return false;
	pElmRoot = xmlDoc.FirstChildElement("root");
	if(!pElmRoot)
		return false;
	pElmItem = pElmRoot->FirstChildElement("item");
	if(!pElmItem)
		return false;

	while(pElmItem)
	{
		PropDef item;
		if(!tinyxml_tool::get_int_attribute(pElmItem, "id", item.m_id) ||
			!tinyxml_tool::get_str_attribute(pElmItem, "title", item.m_title,sizeof(item.m_title)))
		{
			LOG_ERROR("加载道具定义配置失败");		
			return false;
		}
		pElmItem = pElmItem->NextSiblingElement("item");
		m_prop_def_vec.push_back(item);
	}
	return true;
}

bool config_manager::fini()
{
	m_online_mission.clear();
    return true;
}

int config_manager::get_port()
{
    return m_port;
}

int config_manager::get_max_con()
{
    return m_max_con;
}

ddz_game_config& config_manager::get_game_config()
{
	return m_game_config;
}

access_condition& config_manager::get_ac()
{
    return m_ac;
}

glory_def& config_manager::get_glory()
{
    return m_glory;
}

count_box_item* config_manager::get_online_mission(unsigned int levels)
{
	if (levels < 0 || levels >= m_online_mission.size())
	{
		LOG_DEBUG("levels=%d,m_online_mission.size()=%d",levels,m_online_mission.size());
		return 0;
	}
	return &m_online_mission[levels];
}

char* config_manager::get_prop_name(int id)
{
	for (size_t i=0; i<m_prop_def_vec.size();i++)
	{
		if (m_prop_def_vec[i].m_id == id)
		{
			return m_prop_def_vec[i].m_title;
		}
	}
	LOG_ERROR("check prop xml");
	return NULL;
}

bool config_manager::load_db_config()
{
    do 
    {
        mysql_con_helper con(SERVER_DB);
        if(con.is_empty())
            return false;
        char sql[1024];
        if(GAME_MODE_CFG == GAME_TYPE_MATCH)
        {
            sprintf(sql, "SELECT * FROM t_match_list WHERE id=%d", m_serverid);
            record rec;
            rec.init(con.handle());
            if(!rec.open(sql))
                return false;
            if(!rec.get_field_value("port", m_port))
                return false;
        }
        else
        {
            sprintf(sql, "SELECT a.*,b.* FROM game_list a, server_info b WHERE b.serverid=%d AND b.listid=a.id", m_serverid);
            record rec;
            rec.init(con.handle());
            if(!rec.open(sql) || rec.get_record_count() == 0)
                return false;

            if(!rec.get_field_value("less", m_ac.less_coin) ||
                !rec.get_field_value("max", m_ac.max_coin) ||
                !rec.get_field_value("port", m_port) ||
                !rec.get_field_value("base", m_game_config.base_point)||
                !rec.get_field_value("listid", m_server_type))
                return false;
        }
    } while (false);
    do 
    {
        //清理锁表
        mysql_con_helper con(MONEY_DB);
        if(con.is_empty())
            break;
        char sql[1024] = {0};
        sprintf(sql, "delete from coin_locker where roomid=%d", m_serverid);
        con->execute_no_record(sql);
    } while (false);
    return true;
}

bool config_manager::load_match_config()
{
    TiXmlDocument xmlDoc;
    TiXmlElement* pElmRoot;
    if(!xmlDoc.LoadFile("match.xml"))
    {
        LOG_ERROR("open match.xml error");
        return false;
    }
    pElmRoot = xmlDoc.FirstChildElement("root");
    if(!pElmRoot)
    {
        LOG_ERROR("mathc.xml has not root");
        return false;
    }
    TiXmlElement* pElmMs = pElmRoot->FirstChildElement("ms");
    if(!pElmMs ||
        !tinyxml_tool::get_int_attribute(pElmMs, "port", m_msport) ||
        !tinyxml_tool::get_str_attribute(pElmMs, "ip", m_msip, sizeof(m_msip)) ||
        !tinyxml_tool::get_int_attribute(pElmMs, "wait_time", m_wait_match_start_time))
    {
        LOG_ERROR("match.xml has not ms config");
        return false;
    }
    TiXmlElement* pElmReward = pElmRoot->FirstChildElement("reward");
    if(!pElmReward)
    {
        LOG_ERROR("match.xml reward config error");
        return false;
    }
    TiXmlElement* pElmRank = pElmReward->FirstChildElement("rank");
    if(!pElmRank)
    {
        LOG_ERROR("match.xml has no rank config");
        return false;
    }
    while (pElmRank)
    {
        RankRewardItem item;
        if(!tinyxml_tool::get_int_attribute(pElmRank, "from", item.from) ||
            !tinyxml_tool::get_int_attribute(pElmRank, "to", item.to) ||
            !tinyxml_tool::get_int_attribute(pElmRank, "coin", item.coin))
        {
            LOG_ERROR("match.xml error:  rank");
            return false;
        }
        TiXmlElement* pElmProp = pElmRank->FirstChildElement("prop");
        while (pElmProp)
        {
            PropRewardItem prop;
            if(!tinyxml_tool::get_int_attribute(pElmProp, "pid", prop.pid) ||
                !tinyxml_tool::get_int_attribute(pElmProp, "pn", prop.pn) ||
                !tinyxml_tool::get_int_attribute(pElmProp, "term", prop.term))
            {
                LOG_ERROR("match.xml error prop");
                return false;
            }
            item.prop.push_back(prop);
            pElmProp = pElmProp->NextSiblingElement("prop");
        }
        m_reward.reward.push_back(item);
        pElmRank = pElmRank->NextSiblingElement("rank");
    }
    TiXmlElement* pElmRule = pElmRoot->FirstChildElement("rule");
    if(!pElmRule)
    {
        LOG_ERROR("match.xml error: rule");
        return false;
    }
    if(!tinyxml_tool::get_int_attribute(pElmRule, "point", m_matchrule.point))
    {
        LOG_ERROR("match.xml error rule.point");
        return false;
    }
    TiXmlElement* pElmTurns = pElmRule->FirstChildElement("turns");
    if(!pElmTurns)
    {
        LOG_ERROR("match.xml error : turns");
        return false;
    }
    while (pElmTurns)
    {
        RuleTurnItem item;

        if(!tinyxml_tool::get_int_attribute(pElmTurns, "less_user", item.less_user) ||
            !tinyxml_tool::get_int_attribute(pElmTurns, "max_user", item.max_user) ||
            !tinyxml_tool::get_int_attribute(pElmTurns, "base", item.base) ||
            !tinyxml_tool::get_int_attribute(pElmTurns, "plays", item.plays) ||
            !tinyxml_tool::get_int_attribute(pElmTurns, "less_point", item.less_point) ||
            !tinyxml_tool::get_int_attribute(pElmTurns, "max_time", item.max_time) ||
            !tinyxml_tool::get_str_attribute(pElmTurns, "desc", item.desc, sizeof(item.desc)))
        {
            LOG_ERROR("match.xml error turns");
            return false;
        }

        m_matchrule.turns.push_back(item);
        pElmTurns = pElmTurns->NextSiblingElement("turns");
    }
    m_reward_table.push_back(0);
    for(int i=0; i<(int)m_reward.reward.size(); i++)
    {
        int n = m_reward.reward[i].to - m_reward.reward[i].from + 1;
        for(int j=0; j<n; j++)
            m_reward_table.push_back(&m_reward.reward[i]);
    }
    return true;
}

RankRewardItem* config_manager::get_reward(int rank)
{
    if(rank<1 || rank>=(int)m_reward_table.size())
        return 0;
    return m_reward_table[rank];
}