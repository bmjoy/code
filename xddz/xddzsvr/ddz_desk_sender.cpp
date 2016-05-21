#include "ddz_desk.h"
#include "ddzprot.h"
#include "cflog.h"
#include "user_manager.h"
#include "config_manager.h"

void ddz_desk::send(socket_id sid, msg_object_base* obj)
{
	char buf[4096];
	int  len = sizeof(buf);
	if(sid != INVALID_SOCKET_ID && obj && obj->serialize_out(buf, len))
	{
		m_controler->send(sid, buf, len);
	}
}

void ddz_desk::send_all(msg_object_base* obj)
{
	char buf[4096];
	int  len = sizeof(buf);
	if(obj && obj->serialize_out(buf, len))
	{
		send_all(buf, len);
	}
}

void ddz_desk::send(int seatid, char* buf, int len)
{
    if(seatid<0 || seatid>=DDZ_SEATS)
    {
        for(int i=0; i<DDZ_SEATS; i++)
		{
			if(m_seat[i].sid != INVALID_SOCKET_ID)
				m_controler->send(m_seat[i].sid, buf, len);
		}
    }
    else if(m_seat[seatid].sid != INVALID_SOCKET_ID)
    {
        m_controler->send(m_seat[seatid].sid, buf, len);
    }
}

void ddz_desk::send_watch(char* buf, int len)
{
    list<socket_id>::iterator i;
    if (m_watcher.size() == 0)
        return;
    for(i=m_watcher.begin(); i!=m_watcher.end(); i++)
    {
        m_controler->send((*i), buf, len);
    }
}

void ddz_desk::send_all(char* buf, int len)
{
    send(-1, buf, len);
    send_watch(buf, len);
}

void ddz_desk::send_game_begin()
{
	//将配置，发牌，操作合并到一个包发送，减少客户端接收延迟
	for(int i=0; i<DDZ_SEATS; i++)
	{
		sc_deal obj;
		obj.m_cards.set((unsigned char*)m_hand[i].cards, m_hand[i].len);
        obj.m_seatid = i;
        send(m_seat[i].sid, &obj);
	}
    send_set_call_point(m_first_call);
    do 
    {
        nm_user_state obj;
        for(int i=0; i<DDZ_SEATS; i++)
        {
            if(m_seat[i].is_robot)
            {
                obj.m_net_cut[i] = 1;
                obj.m_auto[i] = 0;
            }
            else
            {
                obj.m_net_cut[i] = (m_seat[i].sid == INVALID_SOCKET_ID)?0:1;
                obj.m_auto[i] = m_seat[i].is_auto?1:0;
            }
        }
        send_all(&obj);
    } while (false);
}

void ddz_desk::send_handcards()
{
    for(int i=0; i<DDZ_SEATS; i++)
    {
		sc_deal obj;
		obj.m_cards.set((unsigned char*)m_hand[i].cards, m_hand[i].len);
		send(m_seat[i].sid, &obj);
    }
}

void ddz_desk::send_backcards()
{
    char buf[256];
    int  len = 0;
    send_all(buf, len);
}

void ddz_desk::send_lz()
{
    mn_lz_card obj;
    obj.m_lz = m_lz;
    send_all(&obj);
}

void ddz_desk::send_user_sitdown(int seatid)
{
	room_user* user = user_info(seatid);
	if(!user)
    {
        LOG_ERROR("%s:%d", __FILE__, __LINE__);
		return;
    }
    //发送玩家给桌上所有人
	game_user_sitdown_sc obj;
	obj.m_userid = m_seat[seatid].uid;
	obj.m_tableid = m_deskid;
	obj.m_seatid = seatid;

	obj.m_nickname = user->nickname;
    obj.m_coin = user->coin;
    obj.m_logoid = user->logoid;
    obj.m_glory = user->rs.glory;
    obj.m_win = user->rs.win;
    obj.m_loss = user->rs.lost;
    obj.m_flee = user->rs.flee;
	obj.m_state = m_seat[seatid].state;
	obj.m_vip = user->vip.vip_tag;
	obj.m_vipexpire = user->vip.exp;
    obj.m_sex = user->sex;
	send_all(&obj);
    //发送桌上其他玩家给该玩家
    if(!m_seat[seatid].is_robot)
    {
        for(int i=0; i<DDZ_SEATS; i++)
        {
            if(i==seatid || m_seat[i].uid == 0)
                continue;
            room_user* su = user_info(i);
            if(!su)
            {
                LOG_ERROR("%s:%d, uid=%d", __FILE__, __LINE__, m_seat[i].uid);
                continue;
            }
            obj.m_userid = m_seat[i].uid;
            obj.m_seatid = i;

            obj.m_nickname = su->nickname;
            obj.m_coin = su->coin;
            obj.m_logoid = su->logoid;
            obj.m_glory = su->rs.glory;
            obj.m_win = su->rs.win;
            obj.m_loss = su->rs.lost;
            obj.m_flee = su->rs.flee;
            obj.m_sex = su->sex;
            obj.m_vipexpire = su->vip.exp;
            obj.m_vip = su->vip.vip_tag;
            if(m_seat[i].sid == INVALID_SOCKET_ID)
                obj.m_state = USER_STATE_NET_CUT;
            else
                obj.m_state = m_seat[i].state;

            send(user->sid, &obj);
        }
    }
}

void ddz_desk::send_user_left(int seatid, unsigned int uid)
{
    user_left_desk obj;
    obj.m_userid = uid;
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(i != seatid && m_seat[i].sid != INVALID_SOCKET_ID)
        {
            send(m_seat[i].sid, &obj);
        }
    }
}

void ddz_desk::send_left_fail(int seatid)
{

}

void ddz_desk::send_ready(int seatid)
{
	sc_user_ready obj;
    obj.m_userid = m_seat[seatid].uid;
    send_all(&obj);
}

void ddz_desk::send_outcard_error(int seatid, int errid)
{
    send_user_recome(m_seat[seatid].sid, m_seat[seatid].uid, seatid);
/*
    do 
    {
        sc_out_card_err obj;
        obj.m_reason = errid;
        if(seatid<0 || seatid>2)return;
        obj.m_cards.set((unsigned char*)m_hand[seatid].cards, m_hand[seatid].len);
        send(m_seat[seatid].sid, &obj);
    } while (false);
    send_user_recome(m_seat[seatid].sid, m_seat[seatid].uid, seatid);
    do 
    {
        mn_syn_outcard obj;
        player_cards c[3];
        memset(c, 0, sizeof(c));
        syn_outcard(c);
        obj.m_sit0.set((unsigned char *)c[0].cards, c[0].len);
        obj.m_sit1.set((unsigned char *)c[1].cards, c[1].len);
        obj.m_sit2.set((unsigned char *)c[2].cards, c[2].len);
        send(m_seat[seatid].sid, &obj);
    } while (false);
*/
}

void ddz_desk::send_sitdown_err(socket_id sid, int errid)
{
	game_user_sitdown_err obj;
	obj.m_errid = errid;
	send(sid, &obj);
}
/*
void ddz_desk::send_call_point(int ls, int lp, int next, int cp)
{
    sc_call_point obj;
    obj.m_lastoperator = ls;
    obj.m_lastpoint = lp;
    obj.m_operator = next;
    obj.m_currentpoint = cp;
    send_all(&obj);
}
*/
void ddz_desk::send_set_loader()
{
    sc_land_result obj;
    obj.m_landload = m_loader;
    obj.m_point = m_call_point;
    send_all(&obj);
}

void ddz_desk::send_call_double(int seatid)
{
    //LOG_DEBUG("ddz_desk::send_call_double(%d)", seatid);
    sc_call_double obj;
    obj.m_seat = seatid;
    send_all(&obj);
}

void ddz_desk::send_back_cards()
{
    land_back_cards obj;
    memcpy(obj.m_backcards, m_back, 3);
    obj.m_landload = m_loader;
    send_all(&obj);
    if(m_gameconfig.surport_lz)
    {
        send_lz();
    }
}

void ddz_desk::send_out_cards(int seatid, char* cards, int len, int restcards, int cardtype, int next, int nlz, ddz_lz_change_item* lz_card, int state)
{
    sc_out_card_result obj;
    obj.m_lastoperator = seatid;
    obj.m_lastcards.set((unsigned char *)cards, len);
    obj.m_remaincards = restcards;
    obj.m_cardtype = cardtype;
    obj.m_operator = next;
    obj.m_nlz = nlz;
    obj.m_state = state;
    if(nlz > 4)
    {
        LOG_ERROR("逗逼，你再挂一个我看看。len=%d nlz=%d", len, nlz);
        nlz = 0;
    }
    if(nlz>0 && lz_card)
    {
        for(int i=0; i<nlz; i++)
        {
            obj.m_slz[i] = lz_card[i].card;
            obj.m_dlz[i] = lz_card[i].change;
        }
    }
    send_all(&obj);
}

void ddz_desk::send_call_double_result(int seatid, int point)
{
    double_result obj;
    obj.m_seat = seatid;
    obj.m_value = point;
    send_all(&obj);
}

void ddz_desk::send_mingpai(int seatid)
{
    mn_mingpai_card obj;
    obj.m_cards.set((unsigned char*)m_hand[seatid].cards, m_hand[seatid].len);
    obj.m_seatid = seatid;
    send_all(&obj);
}

void ddz_desk::send_user_net_cut(unsigned int uid, int seatid)
{
    sc_user_net_cut obj;
    obj.m_userid = uid;
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(i != seatid)
        {
            send(m_seat[i].sid, &obj);
        }
    }
}

#define FILL_BUF() {ol=sizeof(buf)-len;obj.serialize_out(&buf[len], ol);len += ol;}

void ddz_desk::send_user_recome(socket_id sid, unsigned int uid, int seatid)
{
    sc_user_recome obj;
    obj.m_userid = uid;
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(i == seatid) continue;
        send(m_seat[i].sid, &obj);
    }
    //统统合并到一个buf中发送----快！
    char buf[4000];
    int  len = 0;
    int  ol = 0;
    //////////////////////////////////////////////////////////////////////////
    {
        //自己的坐下消息在最前面
        game_user_sitdown_sc obj;
        room_user* su = m_controler->get_userinfo(sid, uid);
        if(!su)
        {
            LOG_ERROR("%s:%d", __FILE__, __LINE__);
            return;
        }
        obj.m_userid = uid;
        obj.m_tableid = m_deskid;
        obj.m_seatid = seatid;
        obj.m_nickname = su->nickname;
        obj.m_coin = su->coin;
        obj.m_logoid = su->logoid;
        obj.m_glory = su->rs.glory;
        obj.m_win = su->rs.win;
        obj.m_loss = su->rs.lost;
        obj.m_flee = su->rs.flee;
		obj.m_state = m_seat[seatid].state;
		obj.m_vip = su->vip.vip_tag;
		obj.m_vipexpire = su->vip.exp;
        obj.m_sex = su->sex;
        FILL_BUF();
        int n = DDZ_SEATS;
        if(GAME_MODE_CFG==GAME_TYPE_PK)
            n = 2;
        for(int i=0; i<n; i++)
        {
            if(seatid == i) continue;
            if(m_seat[i].uid == 0) continue;
            su = 0;
			su = user_info(i);
            if(!su)
            {
                LOG_ERROR("%s:%d, uid=%d", __FILE__, __LINE__, m_seat[i].uid);
                continue;
            }
            obj.m_userid = m_seat[i].uid;
            obj.m_seatid = i;
            obj.m_nickname = su->nickname;
            obj.m_coin = su->coin;
            obj.m_logoid = su->logoid;
            obj.m_glory = su->rs.glory;
			obj.m_win = su->rs.win;
			obj.m_loss = su->rs.lost;
			obj.m_flee = su->rs.flee;
			obj.m_vip = su->vip.vip_tag;
			obj.m_vipexpire = su->vip.exp;
            obj.m_sex = su->sex;
            FILL_BUF();
        }
    }
    //////////////////////////////////////////////////////////////////////////
    {
        nm_user_state obj;
        for(int i=0; i<DDZ_SEATS; i++)
        {
            if(m_seat[i].is_robot)
            {
                obj.m_net_cut[i] = 1;
                obj.m_auto[i] = 0;
            }
            else
            {
                obj.m_net_cut[i] = (m_seat[i].sid==INVALID_SOCKET_ID)?0:1;
                obj.m_auto[i] = m_seat[i].is_auto?1:0;
            }
        }
        FILL_BUF();
    }
    //////////////////////////////////////////////////////////////////////////
    {
        sc_recome_state obj;
        obj.m_matchid = 0;
        obj.m_tableid = m_deskid;
        obj.m_state = m_desk_state;
        for(int i=0; i<DDZ_SEATS; i++)
        {
            obj.m_seats[i] = m_seat[i].uid;
        }
        FILL_BUF();
    }
    //////////////////////////////////////////////////////////////////////////
    switch(m_desk_state)
    {
    case DS_GAME_END:            //游戏结束
        LOG_DEBUG("%s %d %d", __FILE__, __LINE__, m_desk_state);
        break;
    case DS_CALL_POINT:          //叫分阶段
        {
            sc_recome_state_call_point obj;
            obj.m_operator = m_operator;
            for(int i=0; i<DDZ_SEATS; i++)
            {
                obj.m_call[i] = m_call[i];
                if(obj.m_call[i]<0)obj.m_call[i]=0;
            }
            obj.m_cards.set((unsigned char*)m_hand[seatid].cards, m_hand[seatid].len);
            FILL_BUF();
        }
        break;
    case DS_CALL_DOUBLE_FARMER:  //农民加倍阶段
    case DS_CALL_DOUBLE_LANDER:  //地主加倍阶段
        {
            sc_recome_state_call_double obj;
            obj.m_landload = m_loader;
            obj.m_point = m_call_point;
            obj.m_cards.set((unsigned char*)m_hand[seatid].cards, m_hand[seatid].len);
            memcpy(obj.m_backcards, m_back, 3);
            for(int i=0; i<DDZ_SEATS; i++)
            {
                obj.m_double[i] = m_call_double[i];
                if (obj.m_double[i]<0)
                {
                    obj.m_double[i] = 0;
                }
            }
            obj.m_operator = m_operator;
            FILL_BUF();
        }
        break;
    case DS_GAMING:              //游戏阶段
        {
            sc_recome_state_outcards obj;
            obj.m_landload = m_loader;
            obj.m_point = m_call_point;
            obj.m_cards.set((unsigned char*)m_hand[seatid].cards, m_hand[seatid].len);
            memcpy(obj.m_backcards, m_back, 3);
            obj.m_lz = m_lz;
            for(int i=0; i<DDZ_SEATS; i++)
            {
                obj.m_double[i] = m_call_double[i];
                if (obj.m_double[i]<0)
                {
                    obj.m_double[i] = 0;
                }
                obj.m_remain[i] = m_hand[i].len;
            }
            obj.m_bomb = m_bomb_count;
            if(m_out_record.size < 1)
            {
                obj.m_lastoperator = -1;
                obj.m_lasttype = 0;
            }
            else
            {
                int index = m_out_record.size - 1;
                obj.m_lastoperator = m_out_record.out_cards[index].seatid;
                obj.m_lastcards.set((unsigned char*)m_out_record.out_cards[index].cards, m_out_record.out_cards[index].len);
                obj.m_nlz = m_out_record.out_cards[index].nlz;
                obj.m_cardtype = m_out_record.out_cards[index].cardtype;
                for(int i=0; i<obj.m_nlz; i++)
                {
                    obj.m_slz[i] = m_out_record.out_cards[index].slz[i];
                    obj.m_dlz[i] = m_out_record.out_cards[index].dlz[i];
                }
            }
            obj.m_operator = m_operator;
            FILL_BUF();
            
            {
                mn_syn_outcard obj;
                player_cards c[3];
                memset(c, 0, sizeof(c));
                syn_outcard(c);
                obj.m_sit0.set((unsigned char *)c[0].cards, c[0].len);
                obj.m_sit1.set((unsigned char *)c[1].cards, c[1].len);
                obj.m_sit2.set((unsigned char *)c[2].cards, c[2].len);
                FILL_BUF();
            }
        }
        break;
    default:
        LOG_ERROR("%s %d %d", __FILE__, __LINE__, m_desk_state);
        break;
    }
    m_controler->send(sid, buf, len);
    if(GAME_MODE_CFG == GAME_TYPE_MATCH)
    {
        sc_match_base obj;
        obj.m_base = m_controler->get_base_point();
        send(sid, &obj);
        send_rank_change();
    }
    send_card_mission_desc(seatid);
}

void ddz_desk::syn_outcard(player_cards c[3])
{
    for(int i=0; i<m_out_record.size; i++)
    {
        int n = m_out_record.out_cards[i].len;
        int id = m_out_record.out_cards[i].seatid;
        char* cards = m_out_record.out_cards[i].cards;
        memcpy(&c[id].cards[c[id].len], cards, n);
        c[id].len += n;
    }
}

void ddz_desk::send_game_result(sc_game_end* obj)
{
    char buf[1024];
    int  len = sizeof(buf);
    if(!obj)
    {
        LOG_ERROR("ddz_desk::send_game_result %s %d", __FILE__, __LINE__);
        return;
    }
    if(!obj->serialize_out(buf, len))
    {
        LOG_ERROR("ddz_desk::send_game_result %s %d", __FILE__, __LINE__);
        return;
    }
    send_all(buf, len);
}

void ddz_desk::send_create_pk(socket_id sid)
{
    sc_create_pk_result obj;
    obj.m_result = 0;
    memcpy(obj.m_code, m_code, sizeof(m_code));
    send(sid, &obj);
}

void ddz_desk::send_add_pk(socket_id sid, int result)
{
    sc_add_pk_result obj;
    obj.m_result = result;

    send(sid, &obj);
}

void ddz_desk::send_force_left(socket_id sid, int reson, __int64 coin, __int64 limit)
{
    sc_force_user_left_desk obj;
    obj.m_reson = reson;
    obj.m_owncoin = coin;
    obj.m_limit = limit;
    send(sid, &obj);
}

void ddz_desk::send_syn_coin(unsigned int uid[3], __int64 coin[3], int size)
{
    char buf[1024];
    int  sendlen = 0;
    for(int i=0; i<size; i++)
    {
        sc_syn_coin obj;
        obj.m_userid = uid[i];
        obj.m_coin = coin[i];
        int slen = sizeof(buf) - sendlen;
        if(!obj.serialize_out(&buf[sendlen], slen))
            continue;
        sendlen += slen;
    }
    send_all(buf, sendlen);
}

void ddz_desk::send_call_point_result(int seatid, int point)
{
    sc_call_point_result obj;
    obj.m_seatid = seatid;
    obj.m_point = point;
    send_all(&obj);
}

void ddz_desk::send_set_call_point(int seatid)
{
    sc_set_call_point obj;
    obj.m_seatid = seatid;
    obj.m_min_point = m_last_call_point+1;
    send_all(&obj);
}

void ddz_desk::send_rank_change()
{
    if(get_empty_size() != 0)
        return;
    sc_rank_change obj;
    obj.m_all = m_controler->get_match_users();
    
    for(int i=0; i<3; i++)
    {
        obj.m_rank[i].m_rank = -1;
        if(m_seat[i].uid == 0)
            continue;
        match_user_item* mu = m_controler->get_match_user(m_seat[i].uid);
        if(mu == 0)
            continue;
        obj.m_rank[i].m_rank = mu->rank;
        obj.m_rank[i].m_point = mu->point;
    }
    obj.m_rank.size(3);
    char buf[64];
    RuleTurnItem* item = m_controler->get_turn_item();
    for(int i=0; i<DDZ_SEATS; i++)
    {
        match_user_item* mu = m_controler->get_match_user(m_seat[i].uid);
        if(!item)
            buf[0] = 0;
        else
            sprintf(buf, "%s(%d/%d)", item->desc, mu?mu->plays+1:1, item->plays);
        obj.m_desc.set(buf);
        send(m_seat[i].sid, &obj);
    }

}

void ddz_desk::send_auto_tag(int seatid, int tag)
{
    change_auto_state obj;
    obj.m_userid = m_seat[seatid].uid;
    obj.m_tag = tag;
    send_all(&obj);
}
