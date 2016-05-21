#ifndef __BINARY_H__
#define __BINARY_H__
#include "byte_stream.h"
#include <vector>

template<int SIZE>
class binary
{
public:
    binary()
    {
        memset(m_data, 0, sizeof(m_data));
        m_len = 0;
    }
    bool serialize_in(byte_stream& bs)
    {
        m_len = 0;
        if(!bs.read(m_len) || m_len > sizeof(m_data)) return false;
        if(!bs.read_v((char*)m_data, m_len)) return false;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        if(m_len > sizeof(m_data)) return false;
        if(!bs.write(m_len)) return false;
        if(!bs.write_v((char*)m_data, m_len)) return false;
        return true;
    }
    bool set(unsigned char* pdata, int len)
    {
        if(len > sizeof(m_data)) return false;
        m_len = (short) len;
        memcpy(m_data, pdata, len);
        return true;
    }
    unsigned char& operator [] (int index)
    {
        return m_data[index];
    }
public:
    unsigned char  m_data[SIZE];
    short m_len;
};

#endif//__BINARY_H__