#ifndef __IP_AUTH_H__
#define __IP_AUTH_H__
#include "singleton.h"
#include "mutex.h"
#include <map>
#include <vector>
using namespace std;

struct common_ip_def
{
    int valid_seg;
    int ip[4];
};

typedef vector<common_ip_def> common_ip;

class ip_auth : public singleton<ip_auth>
{
public:
    ip_auth(void);
    ~ip_auth(void);
    bool load(char* file);
    void fini();
    bool check(char* ip);
    bool check(unsigned int ip);
    char* ip2a(unsigned int ip);
private:
    map<unsigned int, int> m_iplist;
    thread_mutex           m_ipmutex;
    bool                   m_all_permit;
    common_ip              m_common_ip;
};

#define ipauth_inst ip_auth::instance()

/*
ip auth config struct

<root>
    <permit ip="127.0.0.1"/>
    <permit ip="*.*"/>
    <permit ip="192.168.4.*"/>
</root>
*/

#endif//__IP_AUTH_H__