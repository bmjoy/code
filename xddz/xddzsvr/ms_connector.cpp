#include "ms_connector.h"
#include "game_main_thread.h"
#include "config_manager.h"
#pragma warning(disable:4996)

ms_connector::ms_connector(void)
{
    m_state = 0;
}

ms_connector::~ms_connector(void)
{
}

void ms_connector::set_addr(char* ip, int port)
{
    m_socket.init(this, 102400);
    strcpy(m_ip, ip);
    m_port = port;
}

bool ms_connector::connect()
{
    return m_socket.connect(m_ip, m_port);
}

void ms_connector::on_connect(int result)
{
    if(result == 0)
        game_main_thread::instance()->on_ms_connect();
    else
        game_main_thread::instance()->on_ms_close();
}

void ms_connector::on_close(int reason)
{
    game_main_thread::instance()->on_ms_close();
}

int ms_connector::check_pack(void* p_data, int len)
{
    return msg_object_head::check_pack(p_data, len, 40960);
}

bool ms_connector::on_recive(void* p_data, int len)
{
    game_main_thread::instance()->on_ms_recive((char*)p_data, len);
    return true;
}

void ms_connector::send(msg_object_base* obj)
{
    if(!obj)
        return;
    char buf[10240];
    int  sendlen = sizeof(buf);
    if(obj->serialize_out(buf, sendlen))
        m_socket.send(buf, sendlen);
}

void ms_connector::send_ping()
{
    msping obj;
    obj.m_t = (int)time(0);
    send(&obj);
}

void ms_connector::send_reg()
{
    ms_reg_request obj;
    obj.m_state = m_state;
    obj.m_id = config_manager::instance()->serverid();
    send(&obj);
    m_state = 1;
}

void ms_connector::send_load_result(int sn, int result)
{
    ms_match_ready obj;
    obj.m_sn = sn;
    obj.m_result = result;
    send(&obj);
}

void ms_connector::send_user_end(int sn, unsigned int uid, int rank)
{
    ms_user_end obj;
    obj.m_sn = sn;
    obj.m_uid = uid;
    obj.m_rank = rank;
    send(&obj);
}

void ms_connector::send_match_end(int sn)
{
    ms_match_end obj;
    obj.m_sn = sn;
    send(&obj);
}

void ms_connector::send_syn_match(int* sns, int size)
{
    ms_match_info obj;
    obj.m_info.size(0);
    if(size > obj.m_info.max_size())
        size = obj.m_info.max_size();
    if(sns && size > 0)
    {
        for(int i=0; i<size; i++)
        {
            obj.m_info[i].m_sn = sns[i];
        }
    }
    obj.m_info.size(size);
    send(&obj);
}