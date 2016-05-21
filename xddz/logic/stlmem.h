
#ifndef _STL_MEM_H_
#define _STL_MEM_H_

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include "mutex.h"
using namespace std;


class memNode
{
public:
    memNode *nextnode;
};

class my_mempool
{
public:
    static void * alloc(int size)
    {
        //guard ag(m_tmutex);
        int index = getindex(size);
        int realsize = 1<<(index+1);
        if(m_free_head[index]==NULL)
        {
            return malloc(realsize);
        }
        else
        {
            void * p = m_free_head[index];
            m_free_head[index] = m_free_head[index]->nextnode; 
            return p;
        }

        return NULL;
    }
    static void delloc(void *ptr,int size)
    {
        //guard dg(m_tmutex);
        int index = getindex(size);
        memNode* pNew = (memNode*)ptr;
        pNew->nextnode = m_free_head[index];
        m_free_head[index] = pNew;

    }
protected:
    
    static int getindex(int size)
    {
        static const unsigned int sizetable[32] = {1<<1,1<<2,1<<3,1<<4,1<<5,1<<6,1<<7,1<<8,1<<9,1<<10,1<<11,
            1<<12,1<<13,1<<14,1<<15,1<<16,1<<17,1<<18,1<<19,1<<20,1<<21,1<<22,1<<23,1<<24,1<<25,1<<26,
            1<<27,1<<28,1<<29,1<<30,1<<31,(unsigned int)-1};
        static const unsigned int indextable[32] = {2,2,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,
            24,25,26,27,28,29,30,31};
        const unsigned int* p = lower_bound(sizetable,sizetable+32,(unsigned int)size);
        
        return indextable[(p-sizetable)];


    }
protected:
    static memNode* m_free_head[32];   //32个指针
    static thread_mutex m_tmutex;
};



template<class T>  
class my_allocator : public std::allocator<T>  
{  
public:  
    typedef std::allocator<T> base_type;  
    typedef size_t     size_type;
    typedef T*       pointer;
  
    // 必须要重新定义，否则容器如 list, set, map 使用时作用域只能到达 std::allocator  
    template<class Other>  
    struct rebind  
    {  
        typedef my_allocator<Other> other;  
    };  
  
    // 构造函数必须实现  
    my_allocator() {}  
  
    my_allocator(my_allocator<T> const&) {}  
  
    my_allocator<T>& operator=(my_allocator<T> const&) { return (*this); }  
  
    // idiom: Coercion by Member Template  
    template<class Other>  
    my_allocator(my_allocator<Other> const&) {}  
  
    // idiom: Coercion by Member Template  
    template<class Other>  
    my_allocator<T>& operator=(my_allocator<Other> const&) { return (*this); }  
  
    // 内存的分配与释放可以实现为自定义的算法，替换函数体即可  
    pointer allocate(size_type count) 
    { 
        //if(count!=1)
        //{
        //    cout<<"count error"<<endl;
        //}
        return (pointer)my_mempool::alloc(count*sizeof(T));
    }  
  
    void deallocate(pointer ptr, size_type count) 
    { 
        my_mempool::delloc(ptr,count*sizeof(T));
    }  
};  


///仅仅将list、map改为此种模式
template<class _Ty,class _Alloc = my_allocator<_Ty> >
class my_list:public list<_Ty,_Alloc>
{
public:
};


template<class _Kty,
	class _Ty,
	class _Pr = less<_Kty>,
	class _Alloc = my_allocator<pair<const _Kty, _Ty> > >
class my_map: public map<_Kty,_Ty,_Pr,_Alloc> 
{	
public:
};

template<class _Ty,
	class _Alloc = my_allocator<_Ty> >
class my_vector:public vector<_Ty,_Alloc>
{
public:
};


template<class _Kty,
	class _Pr = less<_Kty>,
	class _Alloc = my_allocator<_Kty> >
class my_set: public set<_Kty,_Pr,_Alloc>
{
public:
};

#endif
