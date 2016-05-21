#include "ddz_desk.h"
#include "mddz.h"
#include "ddz_def.h"
#include "ddzprot.h"
#include "cflog.h"
#include "config_manager.h"
#include "db_thread.h"
#include "socket_service.h"
#include "user_manager.h"
#include "myrandom.h"
#include "redis_thread.h"
#include "string_res.h"
#include "mddz_util.h"
#include "mddzc.h"
#include "ddz_ai_strategies.h"
#include "log_type.h"
#include "ai.h"
#include "convert.h"
#include "card_mission.h"
#include "db_thread.h"
#include "user_manager.h"
#include "http_thread.h"
#include "util.h"


void ddz_desk::gen_card_mission()
{
    int mode = cm->mode();
    if(mode == 0)
        return;
    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(m_seat[i].is_robot)
            continue;
        if(mode == CARD_MISSION_RAMDOM)
        {
            m_mission_index[i] = cm->get_random_index();
        }
        else
        {
            room_user* user = m_controler->get_userinfo(m_seat[i].sid);
            if(!user)
                continue;
            m_mission_index[i] = user->card_mission_index;
            if(m_mission_index[i] >= cm->get_size())
                m_mission_index[i] = -1;
        }
        send_card_mission_desc(i);
    }
}

void ddz_desk::send_card_mission_desc(int seatid)
{
    int mode = cm->mode();
    if(mode == 0)
        return;
    sc_card_mission_info obj;
    cm_item* item = cm->get_mission(m_mission_index[seatid]);
    if(!item)
        return;
    obj.m_desc = item->desc;
    obj.m_reward = item->reward;
    send(m_seat[seatid].sid, &obj);
}

void ddz_desk::check_card_mission(int winner)
{
    int mode = cm->mode();
    if(mode == 0)
        return;
    out_cards_item record[3][20];
    int size[3];
    arrange_out_record(record, size);

    for(int i=0; i<DDZ_SEATS; i++)
    {
        if(m_seat[i].is_robot)
            continue;
        cm_item* item = cm->get_mission(m_mission_index[i]);
        if(!item)
            continue;
        bool is_ok = false;
        switch (item->wt)
        {
        case STYLE_CARD_OUT:
            {
                for(int j=0; j<size[i]; j++)
                {
                    if(check_card_mission_match(record[i][j], item))
                    {
                        is_ok = true;
                        break;
                    }
                }
            }
            break;
        case STYLE_ANY_WIN:
            {
                if(winner == m_loader && i != m_loader)
                    break;
                if(winner != m_loader && i == m_loader)
                    break;
                for(int j=0; j<size[i]; j++)
                {
                    if(check_card_mission_match(record[i][j], item))
                    {
                        is_ok = true;
                        break;
                    }
                }
            }
            break;
        case STYLE_FIRST_FINISH:
            {
                if(i != winner)
                    break;
                for(int j=0; j<size[i]; j++)
                {
                    if(check_card_mission_match(record[i][j], item))
                    {
                        is_ok = true;
                        break;
                    }
                }
            }
            break;
        case STYLE_END_OUT:
            {
                if(i != winner)
                    break;
                if(check_card_mission_match(record[i][size[i]-1], item))
                {
                    is_ok = true;
                    break;
                }
            }
            break;
        default:
            break;
        }
        if(!is_ok)
            continue;
        room_user* user = m_controler->get_userinfo(m_seat[i].sid);
        if(!user)
            continue;
        db_thread::instance()->card_mission_log(m_seat[i].uid, m_mission_index[i]);
        sc_card_mission_complish obj;
        obj.m_msg = item->msg;
        obj.m_coin = item->coin;
        if(item->coin > 0)
        {
            user->coin += item->coin;
            db_thread::instance()->on_add_coin(CARD_MISSION, user->uid, item->coin);
        }
        for(int j=0; j<(int)item->prop.size(); j++)
        {
            db_thread::instance()->on_add_prop(CARD_MISSION, user->uid,
                item->prop[j].pid, item->prop[j].pn, item->prop[j].term);
            obj.m_prop[j].m_pid = item->prop[j].pid;
            obj.m_prop[j].m_pn = item->prop[j].pn;
        }
        obj.m_prop.size(item->prop.size());
        send(m_seat[i].sid, &obj);
        if (mode == CARD_MISSION_SINGLE || mode == CARD_MISSION_CYCLE)
        {
            user->card_mission_index ++;
            if(mode == CARD_MISSION_CYCLE && user->card_mission_index>=cm->get_size())
                user->card_mission_index = 0;
            redis_thread::instance()->on_set_card_mission_index(user->uid, user->card_mission_index);
        }
        if(item->broadcast)
        {
            char url[1024];
            int len = sprintf(url, "http://%s:%d/?cmd=bulletin&pid=-1&msg=", cm->get_bc_ip(), cm->get_bc_port());
            char msg[1024];
            int msglen = sprintf(msg, "%s完成牌型任务，奖励：%s", user->nickname, item->reward);

            char utf8buf[1000];
            int utf8len = gb2312_2_utf8(msg,msglen,utf8buf,1000);
            utf8buf[utf8len] = 0;

            char urlencodebuf[102400];
            url_encode(utf8buf, urlencodebuf, sizeof(urlencodebuf));

            sprintf(&url[len], "%s", urlencodebuf);

            http->http_get(url);
        }
    }
}

void ddz_desk::arrange_out_record(out_cards_item record[3][20], int size[3])
{
    for(int i=0; i<DDZ_SEATS; i++) size[i] = 0;
    for(int i=0; i<m_out_record.size; i++)
    {
        out_cards_item item = m_out_record.out_cards[i];
        int seatid = item.seatid;
        if(seatid < 0 || seatid>= DDZ_SEATS)
            continue;
        record[seatid][size[seatid]] = item;
        size[seatid] ++;
    }
}

bool ddz_desk::check_card_mission_match(out_cards_item record, cm_item* item)
{
    if(item->ct == 0)
        return true;
    if(record.cardtype != item->ct)
        return false;
    if(item->cn > 0)
    {
        if(item->cn < 100)
        {
            if(item->cn != record.len)
                return false;
        }
        else if(item->cn > 100)
        {
            int n = item->cn % 100;
            if(record.len < n)
                return false;
        }
    }
    if(item->templlen > 0)
    {
        if(item->templlen != record.len)
            return false;
        for(int i=0; i<item->templlen; i++)
        {
            if(item->templ[i] != (0x0f & record.cards[i]))
                return false;
        }
    }
    return true;
}

