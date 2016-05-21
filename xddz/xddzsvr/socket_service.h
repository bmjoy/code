#ifndef __GAME_SERVER_SOCKET_SERVICE_H__
#define __GAME_SERVER_SOCKET_SERVICE_H__
#include "singleton.h"
#include "tcp_server_engine_win.h"
#include "msg_object_base.h"

class socket_service
    : public singleton<socket_service>
    , public i_tcp_server_service
{
public:
    socket_service(void);
    ~socket_service(void);
    
    bool init();
    void fini();
    bool start();
    void stop();


    virtual void on_start();
    virtual void on_stop();
    virtual void on_pause();
    virtual void on_resume();
	virtual bool on_connect(socket_id sid, unsigned int ip, unsigned short port, bool& enable_batch_send);
	virtual void on_close(socket_id sid);
    virtual int  check_pack(socket_id sid, void* p_data, int len);
	virtual bool on_recive(socket_id sid, void* p_data, int len);

    void send(socket_id sid, char* pdata, int len);
    void send_obj(socket_id sid, msg_object_base* obj);
    bool is_online(socket_id sid);
    void close_client(socket_id sid);
    int get_onlines(){return m_socket.get_online_connections();}
private:
    tcp_server_engine_win m_socket;
};


#endif//__GAME_SERVER_SOCKET_SERVICE_H__