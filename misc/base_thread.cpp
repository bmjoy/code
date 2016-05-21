#include "base_thread.h"
#include<stdio.h>
#ifndef _WIN32
#include<pthread.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#else
#include <process.h>
#endif
#include <iostream>
#include <stdlib.h>





base_thread::base_thread(const char* name)
{
#ifdef _WIN32
#pragma warning(disable:4996)
#endif
    strcpy(thread_name, name);
#ifdef _WIN32
#pragma warning(default:4996)
#endif
#ifndef _WIN32
	pthread_attr_init(&attr);
#endif
}

base_thread::~base_thread()
{
#ifndef _WIN32
	pthread_attr_destroy(&attr);
#endif
    id_list.clear();
}

bool base_thread::activate(int threads)
{
//	guard g(id_mutex);
	if(threads <= 0)
	{
		return false;
	}
	for(int i=0; i<threads; i++)
	{
#ifndef _WIN32
		pthread_t tid;
		int err = pthread_create(&tid, &attr, run0, this);
		if( err != 0)
		{
			printf("can't create thread: %s\n",strerror(err));
			return false;
		}
		usleep(10000);
		id_list.push_back(tid);
#else
		unsigned int dwThreadID = 0;
		HANDLE hThread;
		hThread = (HANDLE)_beginthreadex(NULL, 0, run0, this, 0, &dwThreadID);
		if (hThread == 0)
		{
			printf("can't create thread");
			return false;
		}
		Sleep(10);
		id_list.push_back(hThread);
#endif
	}
	return true;
}

bool base_thread::kill_all()
{
	guard g(id_mutex);
	if(id_list.size() == 0)
	{
		return false;
	}
	thread_id_iterator it;
	for(it=id_list.begin(); it!=id_list.end(); it++)
	{
#ifndef _WIN32
		pthread_cancel((*it));
#else
		TerminateThread((*it), 0);
#endif
	}
	id_list.clear();
	return true;
}

bool base_thread::join()
{
#ifndef _WIN32
	guard g(id_mutex);
	if(id_list.size() == 0)
	{
		return true;
	}
	thread_id_iterator it;
	for(it=id_list.begin(); it!=id_list.end(); it++)
	{
		int s = pthread_join((*it), 0);
		if(s != 0)
		{
			cout << "pthread_join s=" << s << endl;
		}
	}
	id_list.clear();
	return false;
#else
	return true;
#endif
}
#ifndef _WIN32
void* base_thread::run0(void* pVoid)
#else
unsigned int base_thread::run0(void* pVoid)
#endif
{
	base_thread* p_this = (base_thread*)pVoid;
	try
	{
		p_this->svc();
	}
	catch(...)
	{
        cout << "task->svc exception: " << p_this->thread_name << endl;
	}
	return 0;
}

