#ifndef __U_STRING_H__
#define __U_STRING_H__
#include "byte_stream.h"


#define SURPORT_GBK_2_UTF8


#ifndef _WIN32
#include <wchar.h>
#else
#endif

#ifdef SURPORT_GBK_2_UTF8
#include "strtools.h"
#endif


template<int MAX_SIZE, typename T>
class ustring{
public:
    ustring()
    {
        m_buf[0] = 0;
        m_len = 0;
    }
    const ustring& operator = (const char* s)
    {
        set(s);
        return *this;
    }
    const ustring & operator=(const ustring &s){
        m_len = s.m_len < MAX_SIZE ? s.m_len :(MAX_SIZE-1);
        
        memcpy(m_buf, s.m_buf, m_len);
        m_buf[m_len] = 0;
        return *this;
    }

    void set(const char* s)
    {
        if(!s)
        {
            m_len = 0;
            m_buf[m_len] = 0;
            return;
        }
#ifdef SURPORT_GBK_2_UTF8
        m_len = gb2312_2_utf8(s, strlen(s), m_buf, MAX_SIZE-1);
        if(m_len >= MAX_SIZE)
            m_len = MAX_SIZE - 1;
        m_buf[m_len] = 0;
#else
        m_len = strlen(s);
        if(m_len >= MAX_SIZE)
            m_len = MAX_SIZE - 1;
        memcpy(m_buf, s, m_len);
        m_buf[m_len] = 0;
#endif
    }
    int length()
    {
        return m_len;
    }
    bool serialize_in(byte_stream& bs)
    {
        T l;
        if(!bs.read(l) || l > (sizeof(m_buf)-2)) return false;
        if(!bs.read_v(m_buf, l)) return false;
        m_len = l;
        m_buf[m_len] = 0;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        T l = m_len;
        if(l>(sizeof(m_buf)-2))
            l = (T)sizeof(m_buf)-2;

        if(!bs.write(l)) return false;
        if(!bs.write_v(m_buf, m_len)) return false;
        return true;
    }

public:
    char m_buf[MAX_SIZE+2];
    unsigned int  m_len;
};


#endif//__U_STRING_H__