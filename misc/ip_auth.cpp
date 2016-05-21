#include "ip_auth.h"
#include "tinyxml_tool.h"
#include <WinSock2.h>
#pragma warning(disable:4996)

ip_auth::ip_auth(void)
{
    m_all_permit = false;
}

ip_auth::~ip_auth(void)
{
}

common_ip_def parse_com(const char* ip)
{
    common_ip_def ret;
    memset(&ret, 0, sizeof(ret));
    char sip[128] = {0};
    strcpy(sip, ip);
    char* tok;
    char* next;
    tok = strtok_s(sip, ".", &next);

    while(tok)
    {
        if(*tok == '*') break;
        ret.ip[ret.valid_seg] = atoi(tok);
        ret.valid_seg ++;
        if(ret.valid_seg >= 4)
            break;
        tok = strtok_s(0, ".", &next);
    }

    return ret;
}

bool ip_auth::load(char* file)
{
    TiXmlDocument xmlDoc;
    TiXmlElement* pElmRoot;
    TiXmlElement* pElmItem;
    map<unsigned int, int> temp;
    common_ip tmpcom;

    if(!file || !xmlDoc.LoadFile(file))
        return false;
    pElmRoot = xmlDoc.FirstChildElement("root");
    if(!pElmRoot)
        return false;
    pElmItem = pElmRoot->FirstChildElement("permit");
    while(pElmItem)
    {
        char ip[32] = {0};
        if(tinyxml_tool::get_str_attribute(pElmItem, "ip", ip, sizeof(ip)))
        {
            if(strcmp("*.*", ip) == 0)
            {
                m_all_permit = true;
                return true;
            }
            if(strstr(ip, "*"))
            {
                tmpcom.push_back(parse_com(ip));
            }
            else
            {
                unsigned iip = inet_addr(ip);
                temp[iip] = 1;
            }
        }
        pElmItem = pElmItem->NextSiblingElement("permit");
    }
    map<unsigned int,int>::iterator i;
    guard g(m_ipmutex);

    m_common_ip.swap(tmpcom);

    m_iplist.clear();
    for(i=temp.begin(); i!=temp.end(); i++)
    {
        m_iplist[i->first] = 1;
    }
    temp.clear();
    return true;
}

void ip_auth::fini()
{
    m_iplist.clear();
}

bool ip_auth::check(char* ip)
{
    unsigned iip = inet_addr(ip);
    return check(iip);
}

bool ip_auth::check(unsigned int ip)
{
    if(m_all_permit)
        return false;

    guard g(m_ipmutex);

    map<unsigned int,int>::iterator i = m_iplist.find(ip);
    if (i != m_iplist.end())
        return true;
    unsigned char* sip = (unsigned char*)& ip;
    for(size_t i=0; i<m_common_ip.size(); i++)
    {
        bool b = true;
        for(int j=0; j<m_common_ip[i].valid_seg; j++)
        {
            if(sip[j] != m_common_ip[i].ip[j])
            {
                b = false;
                break;
            }
        }
        if(b) return true;
    }

    return false;
}
#pragma warning(disable:4996)
char* ip_auth::ip2a(unsigned int ip)
{
    static char buf[128];
    unsigned char* aip = (unsigned char*) &ip;
    int index = sprintf(buf, "%d", aip[0]);
    for(int i=1; i<4; i++)
        index += sprintf(&buf[index], ".%d", aip[i]);

    return buf;
}