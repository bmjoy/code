#include "card_mission.h"
#include "tinyxml_tool.h"
#include "myrandom.h"

card_mission::card_mission(void)
{
}

card_mission::~card_mission(void)
{
}

bool card_mission::init()
{
    m_cm.clear();
    m_rate.clear();
    m_mode = 0;

    TiXmlDocument xmlDoc;
    TiXmlElement* pElmRoot;
    if(!xmlDoc.LoadFile("card_mission.xml"))
        return false;
    pElmRoot = xmlDoc.FirstChildElement("root");
    if(!pElmRoot)
        return false;
    TiXmlElement* pElmCfg = pElmRoot->FirstChildElement("config");
    if(!pElmCfg ||
        !tinyxml_tool::get_int_attribute(pElmCfg, "mode", m_mode) ||
        !tinyxml_tool::get_str_attribute(pElmCfg, "broad_cast_ip", m_ip, sizeof(m_ip)) ||
        !tinyxml_tool::get_int_attribute(pElmCfg, "broad_cast_port", m_port))
        return false;
    if (m_mode == 0)
    {
        return true;
    }
    TiXmlElement* pElmGroup = pElmRoot->FirstChildElement("group");
    if(!pElmGroup)
        return false;
    TiXmlElement* pElmItem = pElmGroup->FirstChildElement("item");
    if(!pElmItem)
        return false;

    while (pElmItem)
    {
        cm_item item;
        if(!tinyxml_tool::get_int64_attribute(pElmItem, "coin", item.coin) ||
            !tinyxml_tool::get_str_attribute(pElmItem, "desc", item.desc, sizeof(item.desc)) ||
            !tinyxml_tool::get_str_attribute(pElmItem, "reward", item.reward, sizeof(item.reward)) ||
            !tinyxml_tool::get_str_attribute(pElmItem, "msg", item.msg, sizeof(item.msg)) ||
            !tinyxml_tool::get_int_attribute(pElmItem, "wt", item.wt) ||
            !tinyxml_tool::get_int_attribute(pElmItem, "ct", item.ct) ||
            !tinyxml_tool::get_int_attribute(pElmItem, "rate", item.rate) ||
            !tinyxml_tool::get_str_attribute(pElmItem, "templ", item.templ, sizeof(item.templ)) ||
            !tinyxml_tool::get_int_attribute(pElmItem, "cn", item.cn))
            return false;
        if(!tinyxml_tool::get_int_attribute(pElmItem, "broadcast", item.broadcast))
            item.broadcast = 0;
        if(m_mode == 2 && item.rate == 0)
            continue;
        update_templ(&item);
        TiXmlElement* pElmProp = pElmItem->FirstChildElement("prop");
        while (pElmProp)
        {
            PropRewardItem prop;
            if(!tinyxml_tool::get_int_attribute(pElmProp, "pid", prop.pid) ||
                !tinyxml_tool::get_int_attribute(pElmProp, "pn", prop.pn) ||
                !tinyxml_tool::get_int_attribute(pElmProp, "term", prop.term))
                return false;
            item.prop.push_back(prop);
            pElmProp = pElmProp->NextSiblingElement("prop");
        }
        if(m_rate.size() == 0)
        {
            m_rate.push_back(item.rate);
        }
        else
        {
            int r = item.rate + m_rate[m_rate.size()-1];
            m_rate.push_back(r);
        }
        m_cm.push_back(item);
        pElmItem = pElmItem->NextSiblingElement("item");
    }
    return true;
}

int card_mission::get_random_index()
{
    int r = random_int(0, m_rate[m_rate.size()-1]);
    for(int i=0; i<(int)m_cm.size(); i++)
    {
        if(r < m_rate[i])
            return i;
    }
    return -1;
}

int card_mission::get_size()
{
    return m_cm.size();
}

cm_item* card_mission::get_mission(int index)
{
    if(index < 0 || index >= (int)m_cm.size())
        return 0;
    return &m_cm[index];
}

void card_mission::update_templ(cm_item* item)
{
    item->templlen = strlen(item->templ);
    if(item->templlen == 0)
    {
        return;
    }
    for(int i=0; i<item->templlen; i++)
    {
        if(item->templ[i] >= '1' && item->templ[i] <= '9')
            item->templ[i] = 1 + (item->templ[i] - '1');
        else if(item->templ[i] >= 'a' && item->templ[i] <= 'f')
            item->templ[i] = 10 + (item->templ[i] - 'a');
        else
            item->templ[i] = 0;
    }
}