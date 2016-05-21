#ifndef __XDDZ_CARD_MISSION_H__
#define __XDDZ_CARD_MISSION_H__
#include "singleton.h"
#include "config_manager.h"
#include <vector>
using namespace std;

enum CARD_MISSION_MODE
{
    CARD_MISSION_CLOSE,
    CARD_MISSION_SINGLE,
    CARD_MISSION_CYCLE,
    CARD_MISSION_RAMDOM
};

enum CARD_MISSION_COMPLISH_STYLE
{
    STYLE_NULL,
    STYLE_CARD_OUT,
    STYLE_ANY_WIN,
    STYLE_FIRST_FINISH,
    STYLE_END_OUT
};

struct cm_item
{
    int  wt;
    int  ct;
    char templ[20];
    int  rate;
    int  cn;
    
    char desc[256];
    char reward[256];
    char msg[256];
    __int64 coin;
    int  templlen;
    int  broadcast;
    vector<PropRewardItem> prop;
    cm_item()
    {
        wt = 0;
        ct = 0;
        memset(templ, 0, sizeof(templ));
        rate = 0;
        memset(desc, 0, sizeof(desc));
        memset(reward, 0, sizeof(reward));
        memset(msg, 0, sizeof(msg));
        coin = 0;
        templlen = 0;
        prop.clear();
        cn = 0;
    }
};

class card_mission : public singleton<card_mission>
{
public:
    card_mission(void);
    ~card_mission(void);
    bool init();
    int  mode(){return m_mode;}
    int  get_random_index();
    int  get_size();
    cm_item* get_mission(int index);
    char* get_bc_ip(){return m_ip;}
    int  get_bc_port(){return m_port;}
private:
    void update_templ(cm_item* item);
    int  m_mode;
    vector<cm_item> m_cm;
    vector<int>     m_rate;
    char m_ip[32];
    int  m_port;
};

#define cm card_mission::instance()
#define cm_mode cm->mode()

#endif//__XDDZ_CARD_MISSION_H__