#ifndef __M_VECTOR_H__
#define __M_VECTOR_H__
#include "byte_stream.h"


template<class T,int MAX_SIZE>
class mvector
{
public:
	mvector(){m_size =0;}
    T& operator [](int index) {return m_val[index];}
    int size() {return m_size;}
    int max_size(){return MAX_SIZE;}
    void size(int size) {m_size = size; if(m_size<0) m_size = 0; if(m_size > MAX_SIZE) m_size = MAX_SIZE;}
    bool inc_size(){m_size++; if(m_size > MAX_SIZE) m_size = MAX_SIZE; return (m_size == MAX_SIZE);}
    mvector<T,MAX_SIZE>& operator = (mvector<T,MAX_SIZE>& _t)
    {
        for(int i=0; i<_t.size(); i++)
        {
            m_val[i] = _t[i];
        }
        m_size = _t.size();
        return *this;
    }
    bool serialize_in(byte_stream& bs)
    {
        short l;
        bs.read(l);
        if(l<0 || l>MAX_SIZE)
            return false;
        for(int i=0; i<l; i++)
        {
            if(!m_val[i].serialize_in(bs))
                return false;
        }
        m_size = (int)l;
        return true;
    }
    bool serialize_out(byte_stream& bs)
    {
        short l = (short)m_size;
        if(!bs.write(l))
            return false;
        for(int i=0; i<m_size; i++)
        {
            if(!m_val[i].serialize_out(bs))
                return false;
        }
        return true;
    }
    
private:
    int m_size;
    T   m_val[MAX_SIZE];
};

#endif//__M_VECTOR_H__