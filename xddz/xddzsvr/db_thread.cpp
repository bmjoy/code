#include "db_thread.h"
#include "db_con_manager.h"
#include "socket_service.h"
#include "game_user_def.h"
#include "record.h"
#include "game_main_thread.h"
#include "config_manager.h"
#include "ddzprot.h"
#include "cflog.h"
#include "util.h"
#include "tinyxml_tool.h"
#include "user_manager.h"
#include "pptr_buffer.h"
#include "redis_thread.h"
#include <map>
#include <list>
using namespace std;

#pragma warning(disable:4996)
db_type_item g_db_type[DB_TYPE_COUNTER];

class item_init_class_t
{
public:
    item_init_class_t()
    {
        db_type(USERINFO_DB,"userdb");
        db_type(PROP_DB, "propdb");
        db_type(GAME_DB, "gamedb");
        db_type(TASK_DB, "task");
        db_type(MONEY_DB, "vmoney");
		db_type(SERVER_DB, "server");
		db_type(LOG_DB, "log");
		db_type(MESSAGE_DB,"message");
        db_type(MATCH_DB,"match");
    }
private:
    void db_type(int t, const char* name)
    {
        strcpy(g_db_type[t].name, name);
    }
};
item_init_class_t g_init_db_type;


db_thread::db_thread(void)
{
}

db_thread::~db_thread(void)
{
}

enum DB_THREAD_TYPE
{
    DB_TYPE_LOGIN,
    DB_TYPE_LOGOUT,
    DB_TYPE_WRITE_SCORE,
    DB_TYPE_UPDATE_ONLINE,
	DB_TYPE_ADD_PROP,
	DB_TYPE_ADD_COIN,
	DB_TYPE_WRITE_GAMELOG,
	DB_TYPE_AWARD_MSG,
    DB_TYPE_UPDATE_ROBOT,
    DB_TYPE_LOAD_MATCH_USER,
    DBT_MATCH_LOG,
    DB_CARD_MISSION_LOG
};

struct db_thread_type
{
    int id;
    socket_id sid;
    unsigned  int uid;
};

struct db_write_score
{
    __int64 coin;
    __int64 glory;
    int win;
    int lost;
    int flee;
};

struct update_online_info
{
    int serverid;
    int n;
};

struct logout_info
{
    unsigned int uid;
    int serverid;
};

struct add_prop_info
{
	int type;
	int pid;
	int pn;
    int term;
};

struct add_coin_info
{
	int type;
	__int64 coin;
};

struct update_count_box_info
{
	int level;
	int seconds;
};

struct write_game_log
{
	unsigned int uid1;
	unsigned int uid2;
	unsigned int uid3;
	int u1s1;
	int u1s2;
	int u2s1;
	int u2s2;
	int u3s1;
	int u3s2;
	int tax;
};

struct match_log_item
{
    unsigned int uid;
    int matchid;
    int matchtype;
    int cointype;
    __int64 coin;
};

#define MAKE_DB_TYPE(_type, _id, _sid, _uid) db_thread_type _type;_type.id=_id;_type.uid=_uid;_type.sid=_sid;


void db_thread::on_data(void* phead, int headlen, void* pdata, int datalen)
{
    db_thread_type* type = (db_thread_type*)phead;
    switch(type->id)
    {
    case DB_TYPE_LOGIN:
        do_login(type->sid, type->uid);
        break;
    case DB_TYPE_LOGOUT:
        do_logout(type->uid);
        break;
    case DB_TYPE_WRITE_SCORE:
        {
            db_write_score* s = (db_write_score*)pdata;
            do_write_score(type->uid, s->coin, s->glory, s->win, s->lost, s->flee);
        }
        break;
    case DB_TYPE_UPDATE_ONLINE:
        {
            update_online_info* info = (update_online_info*)pdata;
            do_update_online(info->serverid, info->n);
        }
        break;
	case DB_TYPE_ADD_PROP:
		{
			add_prop_info* info = (add_prop_info*)pdata;
			do_add_prop(info->type, type->uid, info->pid, info->pn, info->term);
		}
		break;
	case DB_TYPE_ADD_COIN:
		{
			add_coin_info* info = (add_coin_info*)pdata;
			do_add_coin(info->type, type->uid, info->coin);
		}
		break;
	case DB_TYPE_WRITE_GAMELOG:
		{
            game_log_record& glr = *(game_log_record*)pdata;
			do_write_game_log(glr);
		}
		break;
	case DB_TYPE_AWARD_MSG:
		{
			award_record& info = *(award_record*)pdata;
			do_award_msg_log(info);
		}
        break;
    case DB_TYPE_UPDATE_ROBOT:
        do_update_robot();
        break;
    case DB_TYPE_LOAD_MATCH_USER:
        {
            on_load_match_user(type->uid);
        }
        break;
    case DBT_MATCH_LOG:
        {
            match_log_item& item = *(match_log_item*)pdata;
            on_match_log(item.uid, item.matchid, item.matchtype, item.cointype, item.coin);
        }
        break;
    case DB_CARD_MISSION_LOG:
        {
            int& mid = *(int*)pdata;
            on_card_mission_log(type->uid, mid);
        }
        break;
	default:
		break;
    }
}

void db_thread::on_login(socket_id sid, unsigned int uid)
{
    MAKE_DB_TYPE(type, DB_TYPE_LOGIN, sid, uid);
    this->put_data(&type, sizeof(type), 0, 0, 0);
}

void db_thread::on_logout(unsigned int uid)
{
    if(GAME_MODE_CFG == GAME_TYPE_MATCH)
        return;
    MAKE_DB_TYPE(type, DB_TYPE_LOGOUT, INVALID_SOCKET_ID, uid);
    this->put_data(&type, sizeof(type), 0, 0, 0);
}

void db_thread::on_award_msg_log(award_record item)
{
	MAKE_DB_TYPE(type, DB_TYPE_AWARD_MSG, INVALID_SOCKET_ID, 0);
	this->put_data(&type, sizeof(type), &item, sizeof(item), 2);
}

void db_thread::on_write_score(unsigned int uid, __int64 coin, __int64 glory, int win, int lost, int flee)
{
    MAKE_DB_TYPE(type, DB_TYPE_WRITE_SCORE, INVALID_SOCKET_ID, uid);
    db_write_score score;
    score.coin = coin;
	if(glory<0) glory=0;
	score.glory = glory;
    score.win = win;
    score.lost = lost;
    score.flee = flee;
    this->put_data(&type, sizeof(type), &score, sizeof(score), 1);

}

void db_thread::on_update_online(int serverid, int n)
{
    MAKE_DB_TYPE(type, DB_TYPE_UPDATE_ONLINE, INVALID_SOCKET_ID, 0);
    update_online_info info;
    info.serverid = serverid;
    info.n = n;
    this->put_data(&type, sizeof(type), &info, sizeof(info), 2);
}

void db_thread::on_add_prop(int addtype, unsigned int uid, int pid, int pn, int term)
{
	MAKE_DB_TYPE(type, DB_TYPE_ADD_PROP, INVALID_SOCKET_ID, uid);

	add_prop_info info;
	info.pid = pid;
	info.pn = pn;
	info.type = addtype;
    info.term = term;
	put_data(&type, sizeof(type), &info, sizeof(info), 2);
}

void db_thread::on_add_coin(int addtype, unsigned int uid, __int64 coin)
{
	MAKE_DB_TYPE(type, DB_TYPE_ADD_COIN, INVALID_SOCKET_ID, uid);
	add_coin_info info;
	info.type = addtype;
	info.coin = coin;
	put_data(&type, sizeof(type), &info, sizeof(info), 2);
}

void db_thread::on_write_game_log(game_log_record glr)
{
	MAKE_DB_TYPE(type, DB_TYPE_WRITE_GAMELOG, INVALID_SOCKET_ID, 0);
	put_data(&type,sizeof(type),&glr,sizeof(glr), 2);
}

void db_thread::on_update_robot()
{
    if(IS_PK_MODE)
        return;
    MAKE_DB_TYPE(type, DB_TYPE_UPDATE_ROBOT, INVALID_SOCKET_ID, 0);
    this->put_data(&type, sizeof(type), 0, 0, 2);
}

void db_thread::match_log(unsigned int uid, int matchid, int matchtype, int cointype, __int64 coin)
{
    db_thread_type head = {0};
    head.id = DBT_MATCH_LOG;

    match_log_item item;
    item.uid = uid;
    item.coin = coin;
    item.cointype = cointype;
    item.matchid = matchid;
    item.matchtype = matchtype;
    put_data(&head, sizeof(head), &item, sizeof(item));
}

void db_thread::do_logout(unsigned int uid)
{
    do 
    {
        mysql_con_helper con(MONEY_DB);
        if(con.is_empty())
        {
            LOG_ERROR("%s,%d", __FILE__, __LINE__);
            break;
        }
        char sql[1024];
        sprintf(sql, "call sp_release_coin_lock(%d,%d)", config_manager::instance()->serverid(), uid);
        con->execute_no_record(sql);
    } while (false);
}

void db_thread::do_login(socket_id sid, unsigned int uid)
{
    if(!socket_service::instance()->is_online(sid))
        return;
    room_user userinfo;

    userinfo.uid = uid;
    userinfo.sid = sid;
    char* msg = 0;
    char sql[1024];
    //加载帐号信息
    do 
    {
        mysql_con_helper con(USERINFO_DB);
        if(con.is_empty())
        {
            LOG_ERROR("%s, %d", __FILE__, __LINE__);
            game_main_thread::instance()->on_db_login_result(sid, uid, LOGIN_RESULT_SYS_BUSY, userinfo);
            return;
        }
        sprintf(sql, "call sp_get_account_by_uid(%d)", uid);
        record rec;
        rec.init(con.handle());
        if(!rec.open(sql) || rec.get_record_count() == 0)
        {
            LOG_WARNING("%s, %d", __FILE__, __LINE__);
            game_main_thread::instance()->on_db_login_result(sid, uid, LOGIN_RESULT_SYS_BUSY, userinfo);
            return;
        }
        int enable = 0;
        int sex;
        if(!rec.get_field_value("nickname", userinfo.nickname, sizeof(userinfo.nickname)) ||
            !rec.get_field_value("logoid", userinfo.logoid) ||
            !rec.get_field_value("enable", enable)||
			!rec.get_field_value("sex", sex))
        {
            LOG_WARNING("%s, %d", __FILE__, __LINE__);
            game_main_thread::instance()->on_db_login_result(sid, uid, LOGIN_RESULT_SYS_BUSY, userinfo);
            return;
        }
        userinfo.sex = sex;
        if(!enable)
        {
            game_main_thread::instance()->on_db_login_result(sid, uid, LOGIN_RESULT_DISENABLE, userinfo);
            return;
        }
    } while (false);
	//加载vip 信息
	do 
	{
		mysql_con_helper con(USERINFO_DB);
		if(con.is_empty())
		{
			LOG_ERROR("%s, %d", __FILE__, __LINE__);
			game_main_thread::instance()->on_db_login_result(sid, uid, LOGIN_RESULT_SYS_BUSY, userinfo);
			return;
		}
		sprintf(sql, "call sp_get_vip_info(%d)", uid);
		record rec;
		rec.init(con.handle());
		char exp[32];
        userinfo.vip.exp = 0;
        userinfo.vip.vip_tag = 0;
		if(rec.open(sql) && rec.get_record_count() > 0)
		{
			if(rec.get_field_value("vip", userinfo.vip.vip_level) && rec.get_field_value("expire", exp,sizeof(exp)))
			{
				userinfo.vip.exp = time_stoi(exp);
				userinfo.vip.vip_tag = userinfo.vip.exp>time(0)?1:0;
			}
		}
	} while (false);
    //加载游戏统计
    do 
    {
        mysql_con_helper con(GAME_DB);
        if(con.is_empty())
        {
            LOG_ERROR("%s, %d", __FILE__, __LINE__);
            game_main_thread::instance()->on_db_login_result(sid, uid, LOGIN_RESULT_SYS_BUSY, userinfo);
            return;
        }
        sprintf(sql, "call sp_get_round_stat(%d)", uid);
        record rec;
        rec.init(con.handle());
        if(!rec.open(sql) ||
            rec.get_record_count() == 0 ||
            !rec.get_field_value("win", userinfo.rs.win) ||
            !rec.get_field_value("lost", userinfo.rs.lost) ||
            !rec.get_field_value("flee", userinfo.rs.flee) ||
            !rec.get_field_value("glory", userinfo.rs.glory))
        {
            LOG_WARNING("sp_get_round_stat(%d)", uid);
            game_main_thread::instance()->on_db_login_result(sid, uid, LOGIN_RESULT_SYS_BUSY, userinfo);
            return;
        }
    } while (false);
    //加载游戏币
    do 
    {
        mysql_con_helper con(MONEY_DB);
        if(con.is_empty())
        {
            LOG_ERROR("%s, %d", __FILE__, __LINE__);
            game_main_thread::instance()->on_db_login_result(sid, uid, LOGIN_RESULT_SYS_BUSY, userinfo);
            return;
        }
        record rec;
        rec.init(con.handle());
        sprintf(sql, "call sp_get_coin_lock(%d,%d)", uid, config_manager::instance()->serverid());
        int result = 0;
        int roomid = 0;
        if(!rec.open(sql) ||
            rec.get_record_count() == 0 ||
            !rec.get_field_value("coin", userinfo.coin) ||
            !rec.get_field_value("roomid", roomid) ||
            !rec.get_field_value("result", result))
        {
            LOG_WARNING("sp_get_coin_lock(%d)", uid);
            game_main_thread::instance()->on_db_login_result(sid, uid, LOGIN_RESULT_SYS_BUSY, userinfo);
            return;
        }
        if(result == 2)
        {
            game_main_thread::instance()->on_db_login_result(sid, uid, LOGIN_RESULT_IN_OTHER_ROOM, userinfo, roomid);
            return;
        }
        if(result == 3)
        {
            LOG_WARNING("sp_get_coin_lock(%d), 玩家负分：%lld", uid, userinfo.coin);
            game_main_thread::instance()->on_db_login_result(sid, uid, LOGIN_RESULT_SYS_BUSY, userinfo);
        }
    } while (false);
	//加载计时宝箱
	do{
        userinfo.count_box.seconds = -1;
        userinfo.count_box.level = -1;
        break;
	}while(false);
	//LOG_DEBUG("登录成功%d", uid);
    game_main_thread::instance()->on_db_login_result(sid, uid, LOGIN_RESULT_OK, userinfo);
}

void db_thread::do_write_score(unsigned int uid, __int64 coin, __int64 glory, int win, int lost, int flee)
{
    char sql[1024];
	if (glory<0)
		glory =0;
    do 
    {
        mysql_con_helper con(GAME_DB);
        if(con.is_empty())
            break;
        //v_uid INT,v_win INT,v_lost INT,v_flee INT,v_glory INT
        sprintf(sql, "call sp_update_round_stat(%d,%d,%d,%d,%I64d)", uid, win, lost, flee, glory);
        con->execute_no_record(sql);
    } while (false);
    do 
    {
        mysql_con_helper con(MONEY_DB);
        if(con.is_empty())
            break;
        if(coin > 0)
            sprintf(sql, "call sp_add_coin(%d,%I64d)", uid, coin);
        else if(coin < 0)
        {
            sprintf(sql, "call sp_sub_coin(%d,%I64d)", uid, -coin);
        }
        else
            break;
        con->execute_no_record(sql);
    } while (false);
}

void db_thread::do_update_online(int serverid, int n)
{
    char sql[1024];
    do 
    {
        mysql_con_helper con(SERVER_DB);
        if(con.is_empty())
            break;
        if(GAME_MODE_CFG == GAME_TYPE_MATCH)
            sprintf(sql, "update t_match_list set online=%d, updatetime=NOW() where id=%d", n, serverid);
        else
            sprintf(sql, "call sp_update_onlines(%d,%d)", serverid, n);
        con->execute_no_record(sql);
    } while (false);
}

void db_thread::do_add_prop(int type, unsigned int uid, int pid, int pn, int term)
{
	char sql[1024];
	do 
	{
		mysql_con_helper con(PROP_DB);
		if(con.is_empty())
		{
			LOG_ERROR("addprop fail %d,%d,%d,%d", type, uid, pid, pn);
			return;
		}
        if(term<=0)
		    sprintf(sql, "call sp_add_prop(%d,%d,%d,1,0)", uid, pid, pn);
        else
            sprintf(sql, "call sp_add_prop(%d,%d,%d,2,%d)", uid, pid, pn, term);
		con->execute_no_record(sql);
	} while (false);
	do 
	{
		mysql_con_helper con(LOG_DB);
		if(con.is_empty())
			return;
		sprintf(sql, "INSERT INTO login_log_prop (type,uid,pid,pn,t) VALUES (%d,%d,%d,%d,NOW())",type, uid, pid, pn);
		con->execute_no_record(sql);
	} while (false);
}

void db_thread::do_add_coin(int type, unsigned int uid, __int64 coin)
{
	char sql[1024];
	do 
	{
		mysql_con_helper con(MONEY_DB);
		if(con.is_empty())
		{
			LOG_ERROR("addcoin fail %d,%d,%I64d", type, uid, coin);
			return;
		}
        if(coin > 0)
		    sprintf(sql, "call sp_add_coin(%d,%I64d)", uid, coin);
        else
            sprintf(sql, "call sp_sub_coin(%d,%I64d)", uid, coin);
		con->execute_no_record(sql);
	} while (false);
	do 
	{
        if(type == 12)
            return;
		mysql_con_helper con(LOG_DB);
		if(con.is_empty())
			return;
		sprintf(sql, "INSERT INTO login_log_coin (type,uid,n,t) VALUES (%d,%d,%I64d,NOW())",type, uid, coin);
		con->execute_no_record(sql);
	} while (false);
}

void db_thread::do_write_game_log(game_log_record& glr)
{
    char sql[1024];
    do 
    {
        mysql_con_helper con(LOG_DB);
        if(con.is_empty())
        {
            LOG_ERR_LINE();
            break;
        }
        int serverid = config_manager::instance()->serverid();
        sprintf(sql, "INSERT INTO ddz_game_log (serverid,uid1,uid2,uid3,u1s1,u1s2,u2s1,u2s2,u3s1,u3s2,tax,t) VALUES (%d,%d,%d,%d,%lld,%lld,%lld,%lld,%lld,%lld,%d,NOW())"
            , serverid, glr.user[0].uid, glr.user[1].uid, glr.user[2].uid, 
            glr.user[0].score1, glr.user[0].score2,
            glr.user[1].score1, glr.user[1].score2,
            glr.user[2].score1, glr.user[2].score2,
            glr.tax);
        con->execute_no_record(sql);
    } while (false);
    do 
    {
        mysql_con_helper con(USERINFO_DB);
        if(con.is_empty())
        {
            LOG_ERR_LINE();
            break;
        }
        sprintf(sql, "update t_new_user set play_qty=play_qty+1 where uid in (%d,%d,%d)", glr.user[0].uid, glr.user[1].uid, glr.user[2].uid);
        con->execute_no_record(sql);
    } while (false);
}

void db_thread::do_award_msg_log(award_record& item)
{
	char sql[1024];

	mysql_con_helper con1(MESSAGE_DB);
	if(con1.is_empty())
		return;
	sprintf(sql, "call sp_cpp_add_award_record(%d,'%s')",item.uid,item.text);
	con1->execute_no_record(sql);

	return;
}

void db_thread::do_update_robot()
{
    LOG_DEBUG("begin load robot , serverid=%d", cfg_inst->serverid());

	if(IS_MATCH_MODE)
	{
		int deep = 5000;
		cfg_inst->ai_deep(deep);
        mysql_con_helper con(SERVER_DB);
        if(con.is_empty())
        {
            LOG_DEBUG("load robot return 2");
            return;
        }

        char sql[1024];

        sprintf(sql, "select aideep from t_match_list where serverid=%d", cfg_inst->serverid());

        record rec;
        rec.init(con.handle());

		if(!rec.open(sql))
		{
			return;
		}
		if(!rec.get_field_value("aideep", deep))
			return;
		cfg_inst->ai_deep(deep);
		return;
	}

    if(GAME_MODE_CFG != GAME_TYPE_NORMAL)
        return;
    if(cfg_inst->get_game_config().surport_lz == 1)
    {
        LOG_DEBUG("load robot return 1");
        return;
    }
    map<int, int> userids;
    do 
    {
        mysql_con_helper con(SERVER_DB);
        if(con.is_empty())
        {
            LOG_DEBUG("load robot return 2");
            return;
        }

        char sql[1024];

        sprintf(sql, "select robot_id_from,robot_id_to,robot_deep from server_info where serverid=%d", cfg_inst->serverid());

        record rec;
        rec.init(con.handle());
        
        if(!rec.open(sql) || rec.get_record_count() == 0)
        {
            LOG_DEBUG("load robot return 3");
            return;
        }
        int fromid;
        int toid;
        int deep;
        if(!rec.get_field_value("robot_id_from", fromid) ||
            !rec.get_field_value("robot_id_to", toid) ||
            !rec.get_field_value("robot_deep", deep))
        {
            LOG_DEBUG("load robot return 4");
            return;
        }
        cfg_inst->ai_deep(deep);
        if((toid-fromid)<0 || (toid-fromid) > 500 || toid == 0)
        {
            LOG_DEBUG("load robot return 5 toid=%d fromid=%d to-from=%d", toid, fromid, toid-fromid);
            return;
        }
        for(int i=fromid; i<=toid; i++)
            userids[i] = i;
    } while (false);
    

    int uids[1000];
    int size = 0;
    map<int, int>::iterator i;
    for(i=userids.begin(); i!=userids.end(); i++)
    {
        uids[size] = i->second;
        size ++;
    }
    for(int i=0; i<size; i++)
    {
        int n = rand() % size;
        if(n == i) continue;
        int t = uids[n];
        uids[n] = uids[i];
        uids[i] = t;
    }
    for(int i=0; i<size; i++)
    {
        if(robot_inst->user_info(uids[i]))
        {
            //load_robot_coin(uids[i]);
        }
        else
            load_robot(uids[i]);
    }

    robot_inst->check_exist(userids);
    LOG_DEBUG("end load robot");
}

void db_thread::load_robot(int uid)
{
    room_user userinfo;

    userinfo.uid = uid;
    char* msg = 0;
    char sql[1024];
    //加载帐号信息
    do 
    {
        mysql_con_helper con(USERINFO_DB);
        if(con.is_empty())
        {
            LOG_ERROR("%s, %d", __FILE__, __LINE__);
            return;
        }
        sprintf(sql, "call sp_get_account_by_uid(%d)", uid);
        record rec;
        rec.init(con.handle());
        if(!rec.open(sql) || rec.get_record_count() == 0)
        {
            LOG_WARNING("Robot has not exist:%d", uid);
            return;
        }
        int enable = 0;
        int sex = 0;
        if(!rec.get_field_value("nickname", userinfo.nickname, sizeof(userinfo.nickname)) ||
            !rec.get_field_value("logoid", userinfo.logoid) ||
            !rec.get_field_value("enable", enable)||
            !rec.get_field_value("sex", sex))
        {
            LOG_WARNING("robot not exist uid=%d", uid);
            return;
        }
        userinfo.sex = sex;
        if(!enable)
        {
            LOG_WARNING("robot enable=0 uid=%d", uid);
            return;
        }
    } while (false);
    //加载vip 信息
    userinfo.vip.vip_tag = 0;
    userinfo.vip.exp = 0;//time(0)+9999999;
    //加载游戏统计
    do 
    {
        mysql_con_helper con(GAME_DB);
        if(con.is_empty())
        {
            LOG_ERROR("%s, %d", __FILE__, __LINE__);
            return;
        }
        sprintf(sql, "call sp_get_round_stat(%d)", uid);
        record rec;
        rec.init(con.handle());
        if(!rec.open(sql) ||
            rec.get_record_count() == 0 ||
            !rec.get_field_value("win", userinfo.rs.win) ||
            !rec.get_field_value("lost", userinfo.rs.lost) ||
            !rec.get_field_value("flee", userinfo.rs.flee) ||
            !rec.get_field_value("glory", userinfo.rs.glory))
        {
            LOG_WARNING("load_robot::sp_get_round_stat failed uid=%d", uid);
            return;
        }
    } while (false);
    //加载游戏币
    do 
    {
        mysql_con_helper con(MONEY_DB);
        if(con.is_empty())
        {
            LOG_ERROR("%s, %d", __FILE__, __LINE__);
            return;
        }
        record rec;
        rec.init(con.handle());
        sprintf(sql, "call sp_get_coin_lock(%d,%d)", uid, config_manager::instance()->serverid());
        int result = 0;
        int roomid = 0;
        if(!rec.open(sql) ||
            rec.get_record_count() == 0 ||
            !rec.get_field_value("coin", userinfo.coin) ||
            !rec.get_field_value("roomid", roomid) ||
            !rec.get_field_value("result", result))
        {
            LOG_WARNING("load_robot::sp_get_coin_lock failed uid=%d", uid);
            return;
        }
        if(result == 2 || roomid != config_manager::instance()->serverid() || result == 3)
            return;
    } while (false);
    robot_inst->add_user(userinfo);
}

void db_thread::load_robot_coin(int uid)
{
    room_user userinfo;

    userinfo.uid = uid;
    char* msg = 0;
    char sql[1024];
    //加载游戏币
    do 
    {
        mysql_con_helper con(MONEY_DB);
        if(con.is_empty())
        {
            LOG_ERROR("%s, %d", __FILE__, __LINE__);
            return;
        }
        record rec;
        rec.init(con.handle());
        sprintf(sql, "call sp_get_coin_lock(%d,%d)", uid, config_manager::instance()->serverid());
        int result = 0;
        int roomid = 0;
        if(!rec.open(sql) ||
            rec.get_record_count() == 0 ||
            !rec.get_field_value("coin", userinfo.coin) ||
            !rec.get_field_value("roomid", roomid) ||
            !rec.get_field_value("result", result))
        {
            LOG_WARNING("load_robot::sp_get_coin_lock failed uid=%d", uid);
            return;
        }
        if(result == 2 || roomid != config_manager::instance()->serverid() || result == 3)
            return;
    } while (false);
    robot_inst->add_user(userinfo);
}

void db_thread::load_match_user(int sn)
{
    MAKE_DB_TYPE(type, DB_TYPE_LOAD_MATCH_USER, INVALID_SOCKET_ID, sn);
    put_data(&type, sizeof(type), 0, 0);
}

void db_thread::card_mission_log(unsigned int uid, int mid)
{
    MAKE_DB_TYPE(type, DB_CARD_MISSION_LOG, INVALID_SOCKET_ID, uid);
    put_data(&type, sizeof(type), &mid, sizeof(mid));
}


void set_vip_data(list<room_user>* l, unsigned int uid, char* sz_time)
{
    time_t ct = time(0);
    time_t vt = time_stoi(sz_time);
    list<room_user>::iterator i;
    for(i=(*l).begin(); i!=(*l).end(); i++)
    {
        if((*i).uid == uid)
        {
            i->vip.exp = vt;
            i->vip.vip_tag = ct > time(0) ? 1 : 0;
            return;
        }
    }
}

void db_thread::on_load_match_user(int sn)
{
    unsigned int uid[1024];
    map<unsigned int,int> robotmap;
    int size = 0;
    bool result = false;
    list<room_user>* l = new list<room_user>;
    do 
    {
        char sql[102400];
        {
            mysql_con_helper con(MATCH_DB);
            if(con.is_empty())
                break;
            sprintf(sql, "select uid,isrobot from t_sign where sn=%d and state=0 limit 1000", sn);
            record rec;
            rec.init(con.handle());
            if(!rec.open(sql) || rec.get_record_count() == 0)
            {
                LOG_ERR_LINE();
                break;
            }
            bool err = false;
            while (!rec.is_eof())
            {
                int isrobot;
                if( !rec.get_field_value("uid", uid[size]) ||
                    !rec.get_field_value("isrobot", isrobot))
                {
                    LOG_ERR_LINE();
                    err = true;
                    break;
                }
                robotmap[uid[size]] = isrobot;
                size ++;
                rec.move_next();
            }
            if(err)
                break;
            sprintf(sql, "update t_sign set state=1 where sn=%d", sn);
            con->execute_no_record(sql);
        }
        {
            mysql_con_helper con(USERINFO_DB);
            if(con.is_empty())
            {
                LOG_ERR_LINE();
                break;
            }
            int fl = 0;
            int flen = sprintf(sql, "select uid,nickname,logoid,sex from account where uid in(");
            for(int i=0; i<size; i++)
            {
                if(i == 0)
                    fl = sprintf(&sql[flen], "%d", uid[i]);
                else
                    fl = sprintf(&sql[flen], ",%d", uid[i]);
                flen += fl;
            }
            fl = sprintf(&sql[flen], ")");
            flen += fl;
            record rec;
            rec.init(con.handle());
            if(!rec.open(sql) || rec.get_record_count() == 0)
            {
                LOG_ERR_LINE();
                break;
            }
            bool err = false;
            while (!rec.is_eof())
            {
                room_user item;
                int sex;
                if(!rec.get_field_value("uid", item.uid) ||
                    !rec.get_field_value("nickname", item.nickname, sizeof(item.nickname)) ||
                    !rec.get_field_value("logoid", item.logoid) ||
                    !rec.get_field_value("sex", sex))
                {
                    LOG_ERR_LINE();
                    err = true;
                    break;
                }
                item.sex = sex;
                map<unsigned int,int>::iterator ii = robotmap.find(item.uid);
                if(ii == robotmap.end())
                {
                    item.isrobot = false;
                }
                else
                {
                    item.isrobot = ii->second == 1 ? true : false;
                }
                (*l).push_back(item);
                rec.move_next();
            }
            if(err)
                break;

            flen = sprintf(sql, "select uid,expire from vip where uid in(");
            for(int i=0; i<size; i++)
            {
                if(i == 0)
                    fl = sprintf(&sql[flen], "%d", uid[i]);
                else
                    fl = sprintf(&sql[flen], ",%d", uid[i]);
                flen += fl;
            }
            fl = sprintf(&sql[flen], ")");
            flen += fl;

            rec.open(sql);
            while (!rec.is_eof())
            {
                unsigned int uid;
                char sz_time[32];
                if(rec.get_field_value("uid", uid) &&
                   rec.get_field_value("expire", sz_time, sizeof(sz_time)))
                {
                    set_vip_data(l, uid, sz_time);
                }
                rec.move_next();
            }
        }
        result = true;
    } while (false);

    if(!result)
    {
        delete l;
        l = 0;
    }
    game_main_thread::instance()->on_load_match(sn, l);
}

void db_thread::on_match_log(unsigned int uid, int matchid, int matchtype, int cointype, __int64 coin)
{
    //sp_insert_match_coin_log`(v_uid INT,v_matchid INT,v_type TINYINT,v_flag TINYINT,v_coin BIGINT) 
    char sql[1024];
    mysql_con_helper con(LOG_DB);
    if(con.is_empty())
        return;
    sprintf(sql, "call sp_insert_match_coin_log(%d,%d,%d,%d,%lld)", uid, matchid, matchtype, cointype, coin);
    con->execute_no_record(sql);
}

void db_thread::on_card_mission_log(unsigned int uid, int mid)
{
    char sql[1024];
    mysql_con_helper con(LOG_DB);
    if(con.is_empty())
        return;
    sprintf(sql, "insert into t_card_mission_log(servertype,serverid,uid,mission_id,t) values (%d,%d,%d,%d,NOW())",
        cfg_inst->server_type(), cfg_inst->serverid(), uid, mid);
    con->execute_no_record(sql);
}
