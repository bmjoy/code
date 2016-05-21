#ifndef __MS_CONNECTOR_H__
#define __MS_CONNECTOR_H__
#include "singleton.h"
#include "tcp_connecter.h"
#include "../../../protocol/cpp/msprot.h"

class ms_connector
    : public singleton<ms_connector>
    , public i_tcp_connector_service
{
public:
    ms_connector(void);
    ~ms_connector(void);
    void set_addr(char* ip, int port);
    bool connect();
    virtual void on_connect(int result);
    virtual void on_close(int reason);
    virtual int  check_pack(void* p_data, int len);
    virtual bool on_recive(void* p_data, int len);

    void send(msg_object_base* obj);
    int  state(){return m_state;}
    void state(int s){m_state = s;}
    void send_ping();
    void send_reg();
    void send_load_result(int sn, int result);
    void send_user_end(int sn, unsigned int uid, int rank);
    void send_match_end(int sn);
    void send_syn_match(int* sns, int size);
private:
    char m_ip[32];
    int  m_port;
    tcp_connecter m_socket;
    int  m_state;
};
#define ms_inst ms_connector::instance()

#endif//__MS_CONNECTOR_H__