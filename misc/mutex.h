#ifndef __MUTEX_H__
#define __MUTEX_H__

#ifndef _WIN32
	#include <pthread.h>
	#include <unistd.h>
#else
#include <Windows.h>
#endif

class mutex {
public:
	virtual void lock() = 0;
	virtual void un_lock() = 0;
	virtual bool try_lock() = 0;
};
#include "stdio.h"

class guard {
public:
	guard(mutex* p_mutex)
	{
		_p_mutex = p_mutex;
		if (_p_mutex) {
			_p_mutex->lock();
		}
	}
	guard(mutex& m)
	{
		_p_mutex = &m;
		_p_mutex->lock();
	}
	~guard()
	{
		if (_p_mutex) {
			_p_mutex->un_lock();
		}
	}
private:
	mutex* volatile _p_mutex;
};

#ifndef _WIN32
class thread_mutex : public mutex {
public:
	thread_mutex()
	{
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&_mutex, &attr);
	}
	~thread_mutex()
	{
		pthread_mutex_destroy(&_mutex);
	}
	virtual void lock()
	{
		pthread_mutex_lock(&_mutex);
	}
	virtual void un_lock()
	{
		pthread_mutex_unlock(&_mutex);
	}
	virtual bool try_lock()
	{
		return (pthread_mutex_trylock(&_mutex) == 0);
	}
private:
	pthread_mutexattr_t attr;
	pthread_mutex_t _mutex;
};
#else
class thread_mutex : public mutex {
public:
	thread_mutex()
	{
		::InitializeCriticalSection(&cs);
	}
	~thread_mutex()
	{
		::DeleteCriticalSection(&cs);
	}
	virtual void lock() {
		::EnterCriticalSection(&cs);
	}
	virtual void un_lock() {
		::LeaveCriticalSection(&cs);
	}
	virtual bool try_lock() {
		return true;
	}
	CRITICAL_SECTION cs;
};
#endif//thread_mutex

//----------------------------------------------------------------------
class shared_thread_mutex
{
public:
	shared_thread_mutex(unsigned int size=10)
	{
		p_mutex = new thread_mutex[size];
		pool_size = size;
		pool_counter = 0;
	}
	~shared_thread_mutex()
	{
		delete[] p_mutex;
		p_mutex = 0;
	}
	thread_mutex* operator & ()
	{
		unsigned int index = pool_counter++;
		index %= pool_size;
		return &p_mutex[index];
	}
private:
	thread_mutex* p_mutex;
	unsigned int pool_size;
	volatile unsigned int pool_counter;
};

#endif
