#include "strtools.h"
#ifdef _WIN32
#include <Windows.h>
int utf8_2_gb2312(const char* src, int len, char* out, int bufsize)
{
    if(bufsize<1)
        return -1;
    int i = 0;
    int j = 0;
    out[bufsize-1] = 0;
    while(i < len)
    {
        if(src[i] > 0)
        {
            if(j<(bufsize-1))
            {
                out[j] = src[i];
            }
            i++; j++;
        }
        else
        {
            if(j<(bufsize-3))
            {
                wchar_t t;
                char* uchar = (char *)&t;

                uchar[1] = ((src[i] & 0x0F) << 4) + ((src[i+1] >> 2) & 0x0F);
                uchar[0] = ((src[i+1] & 0x03) << 6) + (src[i+2] & 0x3F);
                WideCharToMultiByte(CP_ACP,NULL,&t,1,&out[j], sizeof(wchar_t),NULL,NULL);
            }
            j+=2;
            i+=3;
        }
    }
    return j;
}

int gb2312_2_utf8(const char* src, int len, char* out, int bufsize)
{
    if(bufsize<1)
        return -1;
    int i = 0;
    int j = 0;
    out[bufsize-1] = 0;
    while(i<len)
    {
        if(src[i] > 0)
        {
            if(j<(bufsize-1))
            {
                out[j] = src[i];
            }
            i ++;
            j ++;
        }
        else
        {
            if(j<(bufsize-4))
            {
                wchar_t t;
                char* pchar = (char*)&t;
                ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED, &src[i], 2, &t, 1);
                out[j]   = (0xE0 | ((pchar[1] & 0xF0) >> 4));
                out[j+1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
                out[j+2] = (0x80 |  (pchar[0] & 0x3F));
            }
            j += 3;
            i += 2;
        }
    }
    if(j<(bufsize-1))
        out[j] = 0;
    return j;
}

#else

int utf8_2_gb2312(char* src, int len, char* out, int bufsize)
{
    return -1;
}

int gb2312_2_utf8(char* src, int len, char* out, int bufsize)
{
    return -1;
}

#endif