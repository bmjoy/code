#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template < class T >
class singleton
{
public:
    static T* instance() { static T _instance ; return &_instance ; }
protected:
    singleton() {}
private:
    singleton(const singleton &) ;
    singleton& operator=(const singleton&) ;
} ;

#endif//__SINGLETON_H__
