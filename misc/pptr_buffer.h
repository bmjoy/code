#ifndef __PPTR_BUFFER_H__
#define __PPTR_BUFFER_H__
#include <string.h>
template<int MAX_BUF_SIZE, int MAX_PTR_SIZE>
struct pptr_buffer
{
	/*
	方便在栈中使用char**类型临时变量, 减少服务器内存碎片/缓存复杂度
	例:
	pptr_buffer<1024, 10> ppbuf;
	ppbuf.add("val1");
	ppbuf.add("val2");
	ppbuf.add("val3");
	ppbuf.add("val4");
	char** pptr = ppbuf.pptr;
	*/
    char*  pptr[MAX_PTR_SIZE];
    int    plen[MAX_PTR_SIZE];
    int    datacount;

    pptr_buffer();
    bool add(char* val, int len=0);
    void reset();
private:
    char  buff[MAX_BUF_SIZE];
};
///////////////////////////////////////////////////////////////////////////
template<int MAX_BUF_SIZE, int MAX_PTR_SIZE>
pptr_buffer<MAX_BUF_SIZE, MAX_PTR_SIZE>::pptr_buffer()
{
    reset();
}
template<int MAX_BUF_SIZE, int MAX_PTR_SIZE>
inline bool pptr_buffer<MAX_BUF_SIZE, MAX_PTR_SIZE>::add(char* val, int len=0)
{
    if(datacount >= MAX_PTR_SIZE)
        return false;
    if(!val)
        return false;
    if(len<=0)
        len = strlen(val);
    char* pw;
    int   wl = len + 1;
    int   rl = 0;
    if(datacount == 0)
    {
        rl = MAX_BUF_SIZE;
        if(wl>rl)
            return false;
        pw = buff;
    }
    else
    {
        rl = MAX_BUF_SIZE - (pptr[datacount-1] - buff + plen[datacount]);
        if(wl>rl)
            return false;
        pw = pptr[datacount-1] + plen[datacount-1] + 1;
    }
    pptr[datacount] = pw;
    memcpy(pw, val, len);
    pw[len] = 0;
    plen[datacount] = len;

    datacount ++;
    return true;
}
template<int MAX_BUF_SIZE, int MAX_PTR_SIZE>
inline void pptr_buffer<MAX_BUF_SIZE, MAX_PTR_SIZE>::reset()
{
    for(int i=0; i<MAX_PTR_SIZE+1; i++)
    {
        pptr[i] = 0;
        plen[i] = 0;
    }
    datacount = 0;
}
#endif//__PPTR_BUFFER_H__