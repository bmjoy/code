#include "util.h"
#ifdef _WIN32
#include<winsock2.h>
#include<Iphlpapi.h>
#include<stdio.h>
#include <string>
#include "crc32.h"
using namespace std;
#pragma comment(lib,"Iphlpapi.lib")
#pragma warning(disable:4996)
#endif
int shrink_xml(char* xml, int len)
{
    int index=0;
    bool find_begin = true;
    //删除空格 换行 制表符
    for(int i=0; i<len; i++)
    {
        if(find_begin)
        {
            if(xml[i] == '>')
                find_begin = false;
            xml[index] = xml[i];
            index ++;
        }
        else
        {
            if(xml[i] == '<')
            {
                find_begin = true;
                xml[index] = xml[i];
                index ++;
            }
            else
            {
                if(xml[i]=='\r' || xml[i]=='\n' || xml[i] == '\t' || xml[i] == ' ')
                    continue;
                xml[index] = xml[i];
                index ++;
            }
        }
    }
    xml[index] = 0;
    //删除注释
    int size = index;
    index = 0;
    find_begin = true;
    for(int i=0; i<size; i++)
    {
        if(find_begin)
        {
            if((i+4)<size && xml[i]=='<'&&xml[i+1]=='!'&&xml[i+2]=='-'&&xml[i+3]=='-')
            {
                find_begin = false;
                i+=3;
            }
            else
            {
                xml[index] = xml[i];
                index ++;
            }
        }
        else
        {
            if((i+3)<size && xml[i]=='-'&&xml[i+1]=='-'&&xml[i+2]=='>')
            {
                find_begin = true;
                i += 2;
            }
        }
    }
    xml[index] = 0;

    return index;
}

#ifdef _WIN32
const char* get_mac_address()
{
    static char mac[128] = {0};
    if(mac[0] == 0)
    {
        PIP_ADAPTER_INFO pAdapterInfo; 
        PIP_ADAPTER_INFO pAdapter = NULL; 
        DWORD dwRetVal = 0; 
        ULONG ulOutBufLen; 
        pAdapterInfo=(PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO)); 
        ulOutBufLen = sizeof(IP_ADAPTER_INFO); 

        // 第一次调用GetAdapterInfo获取ulOutBufLen大小 
        if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) 
        { 
            free(pAdapterInfo); 
            pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
        } 

        if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) { 
            pAdapter = pAdapterInfo;
            while (pAdapter) 
            {
                /*
                printf("Adapter Name: \t%s\n", pAdapter->AdapterName); 
                printf("Adapter Desc: \t%s\n", pAdapter->Description); 
                printf("MAC Addr: \t%02x-%02x-%02x-%02x-%02x-%02x\n", 
                    pAdapter->Address[0], 
                    pAdapter->Address[1], 
                    pAdapter->Address[2], 
                    pAdapter->Address[3], 
                    pAdapter->Address[4], 
                    pAdapter->Address[5]); 
                printf("IP Address: \t%s\n", pAdapter->IpAddressList.IpAddress.String); 
                printf("IP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String); 
                printf("Gateway: \t%s\n", pAdapter->GatewayList.IpAddress.String); 
                printf("------------------------------------------------------\n");
                */
                if(strcmp(pAdapter->IpAddressList.IpAddress.String,"0.0.0.0") != 0)
                {
                    sprintf(mac, "%02x-%02x-%02x-%02x-%02x-%02x", 
                        pAdapter->Address[0], 
                        pAdapter->Address[1], 
                        pAdapter->Address[2], 
                        pAdapter->Address[3], 
                        pAdapter->Address[4], 
                        pAdapter->Address[5]); 
                    break;
                }
                pAdapter = pAdapter->Next; 
            } 
        } 
        else 
        { 
            printf("Call to GetAdaptersInfo failed.\n"); 
        }
    }
    return mac;
}
#else
const char* get_mac_address()
{
    static char mac[128] = {0};

    return mac;
}
#endif


char* strtok_c(char* src, char d, char** context)
{
    char* ptr = 0;
    char* ret = 0;
    if(src == 0)
    {
        ptr = *context;
    }
    else
    {
        ptr = src;
    }

    if(!ptr)
        return ret;

    while(*ptr)
    {
        if(*ptr == d)
        {
            *ptr = 0;
            if(ret)
            {
                ptr++;
                *context = ptr;
                return ret;
            }
        }
        else if(!ret)
        {
            ret = ptr;
        }
        ptr ++;
    }
    *context = 0;
    return ret;
}

char* strtok_w(char* src, short d, char** context)
{
    char* ptr = 0;
    char* ret = 0;
    if(src == 0)
    {
        ptr = *context;
    }
    else
    {
        ptr = src;
    }

    if(!ptr)
        return ret;
    if(*(ptr+1) == 0)
    {
        *context = 0;
        return ptr;
    }
    while(*ptr && *(ptr+1))
    {
        if(*((short*)ptr) == d)
        {
            *((short*)ptr) = 0;
            ptr += 2;
            if(ret)
            {
                *context = ptr;
                return ret;
            }
        }
        else
        {
            if(!ret)
                ret = ptr;
            ptr ++;
        }
    }
    *context = 0;
    return ret;
}

__int64 time_stoi(const char* st)
{
    __int64 ret = 0;
    struct tm tm1;

    sscanf(st, "%4d-%2d-%2d %2d:%2d:%2d",   
        &tm1.tm_year,
        &tm1.tm_mon,
        &tm1.tm_mday,
        &tm1.tm_hour,
        &tm1.tm_min,
        &tm1.tm_sec);             
    tm1.tm_year -= 1900;
    tm1.tm_mon --;
    tm1.tm_isdst=-1;
    ret = mktime(&tm1);
    return ret;
}

void time_itos(__int64 t, char* out)
{
    if(!out) return;
    tm t1 = *localtime(&t);
    int len = sprintf(out, "%04d-%02d-%02d %02d:%02d:%02d", 
        t1.tm_year+1900,
        t1.tm_mon+1,
        t1.tm_mday,
        t1.tm_hour,
        t1.tm_min,
        t1.tm_sec);
    out[len] = 0;
}

void anti_sql_invade(char* s, int len, char dangerous, char safe)
{
    for(int i=0; i<len; i++)
    {
        if(s[i] == dangerous)
            s[i] = safe;
    }
}

int url_encode(const char* src, char* buf, int buflen)
{
    if(!src || !buf || buflen==0)
        return 0;
    char baseChar = 'a';
    unsigned char c;
    int cbDest = 0; //累加
    unsigned char *pSrc = (unsigned char*)src;
    unsigned char *pDest = (unsigned char*)buf;
    while(*pSrc && cbDest < buflen - 1)
    {
        c = *pSrc;
        if(isalpha(c) || isdigit(c) || c == '-' || c == '.' || c == '~')
        {
            *pDest = c;
            ++pDest;
            ++cbDest;
        }
        else if(c == ' ')
        {
            *pDest = '+';
            ++pDest;
            ++cbDest;
        }
        else
        {
            //检查缓冲区大小是否够用？
            if(cbDest + 3 > buflen - 1)
                break;
            pDest[0] = '%';
            pDest[1] = (c >= 0xA0) ? ((c >> 4) - 10 + baseChar) : ((c >> 4) + '0');
            pDest[2] = ((c & 0xF) >= 0xA)? ((c & 0xF) - 10 + baseChar) : ((c & 0xF) + '0');
            pDest += 3;
            cbDest += 3;
        }
        ++pSrc;
    }
    //null-terminator
    *pDest = '\0';
    return cbDest;
}

int url_decode(const char* src, char* buf, int buflen)
{
    if(!src || !buf || buflen==0)
        return 0;
    int cbDest = 0; //累加
    unsigned char *pSrc = (unsigned char*)src;
    unsigned char *pDest = (unsigned char*)buf;
    while(*pSrc)
    {
        if(*pSrc == '%')
        {
            *pDest = 0;
            //高位
            if(pSrc[1] >= 'A' && pSrc[1] <= 'F')
                *pDest += (pSrc[1] - 'A' + 10) * 0x10;
            else if(pSrc[1] >= 'a' && pSrc[1] <= 'f')
                *pDest += (pSrc[1] - 'a' + 10) * 0x10;
            else
                *pDest += (pSrc[1] - '0') * 0x10;

            //低位
            if(pSrc[2] >= 'A' && pSrc[2] <= 'F')
                *pDest += (pSrc[2] - 'A' + 10);
            else if(pSrc[2] >= 'a' && pSrc[2] <= 'f')
                *pDest += (pSrc[2] - 'a' + 10);
            else
                *pDest += (pSrc[2] - '0');

            pSrc += 3;
        }
        else if(*pSrc == '+')
        {
            *pDest = ' ';
            ++pSrc;
        }
        else
        {
            *pDest = *pSrc;
            ++pSrc;
        }
        ++pDest;
        ++cbDest;
    }
    //null-terminator
    *pDest = '\0';
    return cbDest;
}

unsigned int crc32_file(const char* filename)
{
    unsigned int ret = 0;
    FILE* file = fopen(filename, "r+b");
    if(!file)
        return ret;
    long length = 0;
    fseek( file, 0, SEEK_END );
    length = ftell( file );
    fseek( file, 0, SEEK_SET );

    char* buf = new char[length+1];
    fread(buf, length, 1, file);
    fclose(file);
    buf[length] = 0;
    ret = crc32((unsigned char*)buf, length);
    delete[] buf;
    return ret;
}


bool check_valid_phone(char* buf, int len)
{
    if(len == 11)
    {
        if(buf[0] != '1')
            return false;
        for(int i=1; i<11; i++)
        {
            if(buf[i]<'0' || buf[i]>'9')
                return false;
        }
        return true;
    }
    if(len == 13)
    {
        if(buf[0]!='8' || buf[1]!='6' || buf[2]!='1')
            return false;
        for(int i=3; i<13; i++)
        {
            if(buf[i]<'0' || buf[i]>'9')
                return false;
        }
        return true;
    }
    return false;
}