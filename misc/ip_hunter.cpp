#include "ip_hunter.h"
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include "vector_helper.h"
using namespace std;
#pragma warning(disable:4996)

struct ip_info_item
{
    unsigned int from;
    unsigned int to;
    char site1[32];
    char site2[32];
    char site3[32];
    char site4[32];
    int  sitecode;
};

vector<ip_info_item> ip_data;

unsigned int inet_addr_1(const char* ip)
{
    unsigned int ret = 0;
    unsigned int t[4];
    int n = sscanf(ip, "%d.%d.%d.%d", &t[0], &t[1], &t[2], &t[3]);
    if(n != 4)
        return 0;
    ret = (t[0] << 24) + (t[1] << 16) + (t[2] << 8) + t[3];
    return ret;
}

int read_line(FILE* f, char* buf, int buflen)
{
    int index = 0;
    char c = 0;
    //过滤空行
    do 
    {
        if(1 != fread(&c, 1, 1, f))
            return -1;
        if(c == '\r' || c == '\n')
            continue;
        break;
    } while (true);
    do 
    {
        buf[index] = c;
        if(c == '\n' || c == '\r')
        {
            buf[index] = 0;
            break;
        }
        index ++;
        if(index >= buflen)
            return -1;
        if(1 != fread(&c, 1, 1, f))
            break;
    } while (true);
    buf[index] = 0;
    return index;
}

void split_site_info(const char* s, char split[6][32])
{
    int index = 0;
    int ii = 0;
    int fl = 0;
    char* p = (char*) s;
    for(int i=0; i<6; i++) split[i][0] = 0;
    
    while(*p)
    {
        split[ii][index] = *p;
        if((*p) == '\t')
        {
            split[ii][index] = 0;
            ii ++;
            if(ii >= 6)
                return;
            index = 0;
        }
        else
        {
            index ++;
        }
        p++;
    }
    split[5][index] = 0;
}

int read_line(char* src, int& from, int len, char* buf, int buflen)
{
    int index = 0;
    char c = 0;
    //过滤空行
    do 
    {
        c = src[from];
        if(c == '\r' || c == '\n')
        {
            from ++;
            continue;
        }
        break;
    } while (src[from]);

    do 
    {
        if(from >= len)
            return -1;
        c = src[from];
        if(c == '\n' || c == '\r')
        {
            buf[index] = 0;
            return index;
        }
        buf[index] = src[from];
        index ++;
        if(index >= buflen)
            return -1;
        from ++;
        if(from >= len)
            break;
    } while (src[from]);
    buf[index] = 0;
    return index;
}

bool get_ip_info(int index, char** site1, char** site2, char** site3, char** site4, int* sitecode)
{
    if(index <=0 || index >= (int)ip_data.size())
        return false;
    *site1 = ip_data[index].site1;
    *site2 = ip_data[index].site2;
    *site3 = ip_data[index].site3;
    *site4 = ip_data[index].site4;
    *sitecode = ip_data[index].sitecode;

    return true;
}

bool hunter_ip_site(unsigned int uip, char** site1, char** site2, char** site3, char** site4, int* sitecode)
{
    //二分法快速查找
    int size = ip_data.size();
    int n1 = size / 2;
    int n2 = 0;
    int n3 = size;
    *sitecode = 0;
    do 
    {
        if(uip < ip_data[n1].from)
        {
            if((n1-n2) < 3)
            {
                for(int i=n2; i<n1; i++)
                {
                    if(uip>=ip_data[i].from && uip<=ip_data[i].to)
                    {
                        return get_ip_info(i, site1, site2, site3, site4, sitecode);
                    }
                }
                return false;
            }
            n3 = n1;
            n1 = (n1 + n2) / 2;
            continue;
        }
        if(uip > ip_data[n1].to)
        {
            if((n3-n1) < 3)
            {
                for(int i=n1+1; i<=n3,i<(int)ip_data.size(); i++)
                {
                    if(uip>=ip_data[i].from && uip<=ip_data[i].to)
                    {
                        return get_ip_info(i, site1, site2, site3, site4, sitecode);
                    }
                }
                return false;
            }
            n2 = n1;
            n1 = (n1 + n3) / 2;
            continue;
        }
        return get_ip_info(n1, site1, site2, site3, site4, sitecode);

    } while (true);
    return false;
}

//////////////////////////////////////////////////////////////////////////
bool init_ip_hunter(const char* filename)
{
    struct stat sbuf;
    memset(&sbuf, 0, sizeof(sbuf));
    int n = stat(filename, &sbuf);
    if(n)
        return false;
    int flen = sbuf.st_size;

    char* fbuf = new char[flen + 1];

    FILE* file = fopen(filename, "rb");
    if(!file)
        return false;
    n = fread(fbuf, 1, flen, file);
    fclose(file);
    if(n != flen)
    {
        delete fbuf;
        return false;
    }
    fbuf[flen] = 0;


    char buf[1024];
    int  ll = 0;
    char split[6][32];

    ip_data.resize(200000);
    ip_data.resize(0);
    int index = 0;
    int from = 0;
    while(ll >= 0)
    {
        ll = read_line(fbuf, from, flen, buf, 1024);
        if(ll < 0)
            break;
        split_site_info(buf, split);

        ip_info_item data = {0};
        data.from = inet_addr_1(split[0]);
        data.to   = inet_addr_1(split[1]);
        strcpy(data.site1, split[2]);
        strcpy(data.site2, split[3]);
        strcpy(data.site3, split[4]);
        strcpy(data.site4, split[5]);
        data.sitecode = 0;
        ip_data.push_back(data);
    }
    delete fbuf;
    return true;
}

void fini_ip_hunter()
{
    release_vector(ip_data);
}

bool hunter_ip_site(const char* ip,  char** site1, char** site2, char** site3, char** site4, int* sitecode)
{
    if(!ip)
        return false;
    unsigned int uip = inet_addr_1(ip);
    return hunter_ip_site(uip, site1, site2, site3, site4, sitecode);
}