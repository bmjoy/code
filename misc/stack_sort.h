#ifndef __STACK_SORT_H__
#define __STACK_SORT_H__
/*
优化说明:
1. 去掉递归
2. 排序栈可以由外部管理, 不需要每次都new delete
3. 支持双向排序
4. 排序携带数据类型
*/
template <class T, class datatype=void*>
struct sort_item
{
    T value;
    datatype d;
    inline bool operator < (const sort_item<T,datatype>& v)
    {
        return (value < v.value);
    }
    inline bool operator > (const sort_item<T,datatype>& v)
    {
        return (value > v.value);
    }
    inline bool operator ==(const sort_item<T,datatype>& v)
    {
        return (value == v.value);
    }
    inline bool operator <=(const sort_item<T,datatype>& v)
    {
        return (value <= v.value);
    }
    inline bool operator >=(const sort_item<T,datatype>& v)
    {
        return (value >= v.value);
    }
};

template <class T, class datatype>
inline void swap(sort_item<T, datatype>* a, sort_item<T, datatype>* b)
{
    if(a == b)
        return;
    sort_item<T, datatype> t;
    t = *a;
    *a = *b;
    *b = t;
}

template<class T>
class sort_stack
{
public:
    sort_stack(int size)
    {
        m_size = size;
        m_p = new T[m_size];
        m_index = -1;
    }
    ~sort_stack()
    {
        if(m_p)
        {
            delete m_p;
        }
        m_p = 0;
        m_size = 0;
        m_index = 0;
    }
    inline void pop()
    {
        if(m_index >= 0)
            m_index --;
    }
    inline T top()
    {
        if(m_index>=0)
            return m_p[m_index];
        T t;
        memset(&t, 0, sizeof(t));
        return t;
    }
    inline void push(T t)
    {
        if((m_index+1) < m_size)
        {
            m_index ++;
            m_p[m_index] = t;
        }
    }
    inline int size()
    {
        if(m_index>=0)
            return (m_index+1);
        return 0;
    }
    inline bool empty()
    {
        return (m_index<0);
    }
    inline void reset()
    {
        m_index = -1;
    }
private:
    T*  m_p;
    int m_size;
    int m_index;
};

template <class T, class datatype>
struct sort_param{
    sort_item<T,datatype>* a;
    int n;
};

template <class T, class datatype>
inline void stack_sort(sort_item<T,datatype>* a, int n, bool dir=true, sort_stack< sort_param<T,datatype> >* p_stack=0)
{
    sort_stack< sort_param<T,datatype> >* ptemp = 0;
    if(p_stack)
        ptemp = p_stack;
    else
        ptemp = new sort_stack< sort_param<T,datatype> >(n);

    sort_stack< sort_param<T,datatype> >& sp = *ptemp;
    sort_param<T,datatype> pm;

    if(dir)
    {
        for(;;){
            if( n<=2 ){
                if( n==2 && a[1] < a[0])
                    swap(a, a+1);
                if(sp.empty())
                    break;
                pm = sp.top();
                sp.pop();
                a = pm.a;
                n = pm.n;
                continue;
            }
            swap( a, a+(n>>1) );
            sort_item<T,datatype> pivot=*a;
            sort_item<T,datatype>* l=a+1;
            sort_item<T,datatype>* r=a+n-1;
            while( l<r )
            {
                while(l<r && *l<pivot)
                    l++;
                while( r>a && *r>=pivot)
                    r--;
                if( l<r )
                    swap( l, r );
            }
            if(*a > *r)
                swap( a, r );
            pm.a=r+1;
            pm.n=n-1-(r-a);
            sp.push(pm);
            n = r-a;
        }
    }
    else
    {
        for(;;){
            if( n<=2 ){
                if(n==2 && a[1] > a[0])
                    swap(a, a+1);
                if(sp.empty())
                    break;
                pm = sp.top();
                sp.pop();
                a = pm.a;
                n = pm.n;
                continue;
            }
            swap( a, a+(n>>1) );
            sort_item<T,datatype> pivot=*a;
            sort_item<T,datatype>* l=a+1;
            sort_item<T,datatype>* r=a+n-1;
            while(l < r)
            {
                while(l<r && *l>pivot)
                    l++;
                while( r>a && *r<=pivot)
                    r--;
                if( l<r )
                    swap( l, r );
            }
            if(*a < *r)
                swap( a, r );
            pm.a=r+1;
            pm.n=n-1-(r-a);
            sp.push(pm);
            n = r-a;
        }
    }
    if(!p_stack)
        delete ptemp;
    ptemp = 0;
}

template<class T, class datatype=void*>
class sort_buff_helper
{
public:
    sort_item<T,datatype>*      m_sort_buff;
    int                         m_max;
    sort_buff_helper()
    {
        m_max = 0;
        m_sort_buff = 0;
        m_sort_stack = 0;
    }
    void create(int n)
    {
        if(n<m_max)
            return;
        if(m_sort_buff)
            delete m_sort_buff;
        if(m_sort_stack)
            delete m_sort_stack;
        m_max = n;
        m_sort_stack = new sort_stack< sort_param<T,datatype>>(m_max);
        m_sort_buff  = new sort_item<T,datatype>[m_max];
    }
    void clear()
    {
        if(m_sort_buff)
            delete m_sort_buff;
        if(m_sort_stack)
            delete m_sort_stack;
        m_max = 0;
        m_sort_buff = 0;
        m_sort_stack = 0;
    }
    inline sort_item<T,datatype>& operator [] (int index)
    {
        return m_sort_buff[index];
    }
    sort_stack< sort_param<T,datatype>>* get_stack()
    {
        return m_sort_stack;
    }
private:
    sort_stack< sort_param<T,datatype>>* m_sort_stack;
};

template <class T, class datatype>
void stack_sort(sort_buff_helper<T,datatype>& buf, int size, bool dir=true)
{
    buf.get_stack()->reset();
    stack_sort(buf.m_sort_buff, size, dir, buf.get_stack());
}

/*
使用方法Demo：
 
//模拟排序数据
struct item_tag{
    int value;
    int grade;
};
 
//定义排序数据存储
map<unsigned int,item_tag> sort_data;
//...load sort_data;
//创建排序缓存
sort_buff_helper<int,map<unsigned int,item_tag>::iterator> sbuff;
sbuff.create(sort_data.size()+1);

//保存迭代器,回写排序结果时用到
map<unsigned int,item_tag>::iterator i;
int index = 0;
for(i=sort_data.begin(); i!=sort_data.end(); i++)
{
    sbuff[index].d = i;
	sbuff[index].value = i->second.value;
    index ++;
}

bool dir = true;
 
//排序
stack_sort(sbuff, sort_data.size(), dir);

//回写排序结果
for(int i=0; i<size; i++)
{
	sbuff[i].d->second.grade = i;
}

*/
#endif//__STACK_SORT_H__