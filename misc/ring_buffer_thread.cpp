#include "ring_buffer_thread.h"
#ifdef _WIN32
#include <process.h>
#include <iostream>
#else
#include <unistd.h>
#endif
using namespace std;

ring_buffer_thread::ring_buffer_thread()
{
#ifdef _WIN32
//	hDataEvent_ = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    hDataEvent_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
#else
	pthread_attr_init(&attr);	
	pthread_cond_init(&data_cond, 0);
	pthread_mutex_init(&data_mutex, 0);
#endif
	is_init = false;
	threads = 0;
	max_block_size = 0;
	is_run = false;
}

ring_buffer_thread::~ring_buffer_thread()
{
#ifdef _WIN32
	if(hDataEvent_ != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hDataEvent_);
		hDataEvent_ = INVALID_HANDLE_VALUE;
	}
#else
	pthread_attr_destroy(&attr);
	pthread_cond_destroy(&data_cond);
	pthread_mutex_destroy(&data_mutex);
#endif
	fini();
}

bool ring_buffer_thread::init(int _threads, int _init_size, int _step_size,
							  int _max_size, int _max_block_size)
{
	if (is_init)
	{
		return false;
	}
	threads = _threads;
	if (threads < 0)
	{
		return false;
	}
	max_block_size = _max_block_size;
	if(data_buff.init(_init_size,_step_size,_max_size,_max_block_size))
	{
		is_init = true;
	}
	return is_init;
}

bool ring_buffer_thread::fini()
{
	if(!is_init || is_run)
	{
		return false;
	}
	data_buff.fini();
	is_init = false;
	return true;
}

bool ring_buffer_thread::start()
{
	if(!is_init || is_run)
	{
		return false;
	}
	is_run = true;
	for(int i=0; i<threads; i++)
	{
#ifdef _WIN32
		unsigned int dwThreadID = 0;
		HANDLE hThread;
		hThread = (HANDLE)_beginthreadex(NULL, 0, run0, this, 0, &dwThreadID);
		Sleep(100);
#else
		pthread_t tid;
		if(pthread_create(&tid, &attr, run0, this) != 0)
			return false;
		usleep(10000);
#endif
	}
	return true;
}

bool ring_buffer_thread::stop()
{
	if(!is_run)
	{
		return false;
	}
	is_run = false;
	for(int i=0; i<threads; i++)
	{
#ifdef _WIN32
//		::SetEvent(hDataEvent_);
        ::PostQueuedCompletionStatus(hDataEvent_, 0, 0, 0);
#else
		pthread_cond_signal(&data_cond);
#endif
	}
#ifdef _WIN32
	Sleep(1000);
#else
	sleep(1);
#endif
	return true;
}

bool ring_buffer_thread::put_data(void* pdata, int len)
{
	if(!is_run)
	{
		return false;
	}
	bool ret = data_buff.putd(pdata,len);
	if(ret)
	{
#ifdef _WIN32
//		::SetEvent(hDataEvent_);
        ::PostQueuedCompletionStatus(hDataEvent_, 0, 0, 0);
#else
		pthread_cond_signal(&data_cond);
#endif
	}
	return ret;
}

#ifdef _WIN32
unsigned int ring_buffer_thread::run0(void * pVoid)
#else
void* ring_buffer_thread::run0(void* pVoid)
#endif
{
	ring_buffer_thread* pthis = (ring_buffer_thread*)pVoid;
	pthis->run();
	return 0;
}

int ring_buffer_thread::run()
{
	char* pbuf = new char[max_block_size];
	while(is_run)
	{
#ifdef _WIN32
//		::WaitForSingleObject(hDataEvent_, INFINITE);
        DWORD dw_size = 0;
        long p1 = 0;
        LPOVERLAPPED p2;
        BOOL QB = ::GetQueuedCompletionStatus(hDataEvent_, &dw_size, (PULONG_PTR)&p1, &p2, INFINITE);

		if(!QB)
			break;
#else
		pthread_mutex_lock(&data_mutex);
		pthread_cond_wait(&data_cond, &data_mutex);
		pthread_mutex_unlock(&data_mutex);
#endif
		if(!is_run)
		{
			break;
		}
		bool b = true;
		while(b)
		{
			int l = max_block_size;
			b = data_buff.getd(pbuf, l);
			if(b)
			{
				on_data(pbuf, l);
			}
		}
	}
	delete[] pbuf;
	pbuf = 0;
	return 0;
}

