#ifndef __SAFE_QUEUE_H__
#define __SAFE_QUEUE_H__
#include "mutex.h"

template <class T>
class safe_queue
{
public:
	safe_queue()
	{
		q = 0;
	}
	~safe_queue()
	{
		fini();
	}
	void init(int max_size)
	{
        guard g(m_mutex);
		size = max_size;
		q = new T[size];
		write_index = 0;
		read_index = 0;
		counter = 0;
	}
	void fini()
	{
        guard g(m_mutex);
		if(q)
		{
			delete[] q;
			q = 0;
		}
	}
	bool put(T t)
	{
        guard g(m_mutex);
		if(!q || counter == size)
		{
			return false;
		}
		counter++;
		q[write_index] = t;
		write_index ++;
		if(write_index == size)
		{
			write_index = 0;
		}
		return true;
	}
	bool pop(T& t)
	{
        guard g(m_mutex);
		if(!q || counter == 0)
		{
			return false;
		}
		t = q[read_index];
		counter --;
		read_index ++;
		if(read_index == size)
		{
			read_index = 0;
		}
		return true;
	}
    int get_max_size()
    {
        guard g(m_mutex);
        return size;
    }
    int get_size()
    {
        guard g(m_mutex);
        return counter;
    }
private:
	int write_index;
	int read_index;
	int size;
	int counter;
	T*  q;
	thread_mutex m_mutex;
};

template <class T>
class svector
{
public:
    svector()
    {
        v = 0;
        length = 0;
        size = 0;
    }
    void init(int maxsize)
    {
        size = 0;
        length = maxsize;
        v = new T[maxsize];
    }
    void fini()
    {
        if(v)
            delete[] v;
        size = 0;
        v = 0;
        length = 0;
    }
    bool add(T t)
    {
        if(size >= length)
            return false;
        v[size] = t;
        size ++;
        return true;
    }
    void clear()
    {
        size = 0;
    }
    int get_size()
    {
        return size;
    }
    T& operator [](int index)
    {
        return v[index];
    }
    void resize(int s)
    {
        size = s;
    }
private:
    T*   v;
    int  length;
    int  size;
};

#endif//__SAFE_QUEUE_H__
