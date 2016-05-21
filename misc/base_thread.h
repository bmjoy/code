#ifndef __BASE_THREAD_H__
#define __BASE_THREAD_H__
#ifndef _WIN32
#include <pthread.h>
#include <unistd.h>
#else
#include <Windows.h>
#endif
#include <list>
#include "mutex.h"


using namespace std;

#ifndef _WIN32
typedef list<pthread_t> thread_id_list;
#else
typedef list<HANDLE> thread_id_list;
#endif

typedef thread_id_list::iterator thread_id_iterator;


///////////////////////////////////////////////////////////////////////////////
// base_thread class
class base_thread
{
// Construction & Destruction
protected:
public:
	base_thread(const char* name);
	~base_thread();
// Attributes
private:
#ifndef _WIN32
	pthread_attr_t			attr;
#else
#endif
	thread_mutex			id_mutex;
	thread_id_list			id_list;
protected:
	char					thread_name[128];
public:
// Operations
	virtual int				svc() = 0;
	bool					activate(int threads=1);
	bool					kill_all();
	bool					join();
private:
#ifndef _WIN32
	static void*					run0(void* pVoid);
#else
	static unsigned int __stdcall	run0(void* pVoid);
#endif
protected:
	


};

#endif//__BASE_THREAD_H__
