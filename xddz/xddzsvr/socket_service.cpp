#include "socket_service.h"
#include "game_main_thread.h"
#include "config_manager.h"

socket_service::socket_service(void)
{
}

socket_service::~socket_service(void)
{
}

bool socket_service::init()
{
    engine_init_item item = {0};
    item.con_threads = 0;
    item.init_mem_pool_size = 100;
    item.max_connects = config_manager::instance()->get_max_con();
    item.max_mem_pool_size = item.max_connects * 4;
    item.max_package_size = 4000;
    item.port = config_manager::instance()->get_port();
    item.rs_threads = 2;
    item.step_mem_pool_size = 100;
    item.time_out = 8;
    item.time_out_count = 4;
    if(!m_socket.init(item, this))
        return false;
    m_socket.set_service(this);
    return true;
}

void socket_service::fini()
{
    m_socket.fini();
}

bool socket_service::start()
{
    return m_socket.start();
}

void socket_service::stop()
{
    m_socket.stop();
}

void socket_service::on_start()
{

}

void socket_service::on_stop()
{

}

void socket_service::on_pause()
{

}

void socket_service::on_resume()
{

}

bool socket_service::on_connect(socket_id sid, unsigned int ip, unsigned short port, bool& enable_batch_send)
{
    //printf("on_connect\n");
    return true;
}

void socket_service::on_close(socket_id sid)
{
    game_main_thread::instance()->on_client_close(sid);
}

int socket_service::check_pack(socket_id sid, void* p_data, int len)
{
    msg_object_head* head = (msg_object_head*)p_data;
    if(len < MSG_OBJ_HEAD_SIZE)
        return 0;
    if(head->len > MAX_PACKGE_LEN)
        return -1;
    if(len < head->len)
        return 0;
    return head->len;
}

bool socket_service::on_recive(socket_id sid, void* p_data, int len)
{
    game_main_thread::instance()->on_client_recive(sid, (char*)p_data, len);
    return true;
}

void socket_service::send(socket_id sid, char* pdata, int len)
{
    m_socket.send(sid, 0, 0, pdata, len);
}

void socket_service::send_obj(socket_id sid, msg_object_base* obj)
{
    char buf[4096];
    int  len = sizeof(buf);
    if(!obj)
        return;
    if(obj->serialize_out(buf, len))
        send(sid, buf, len);
}

bool socket_service::is_online(socket_id sid)
{
    return m_socket.is_online(sid);
}

void socket_service::close_client(socket_id sid)
{
	if(sid != INVALID_SOCKET_ID)
	    m_socket.close_socket(sid);
}