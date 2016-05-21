#include "ring_buffer_d_thread.h"
#ifdef _WIN32
#include <process.h>
#endif

ring_buffer_d_thread::ring_buffer_d_thread()
{
#ifdef _WIN32
    //	hDataEvent_ = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    hDataEvent_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 4);
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

ring_buffer_d_thread::~ring_buffer_d_thread()
{
#ifdef _WIN32
	::CloseHandle(hDataEvent_);
	hDataEvent_ = INVALID_HANDLE_VALUE;
#else
	pthread_attr_destroy(&attr);
	pthread_cond_destroy(&data_cond);
	pthread_mutex_destroy(&data_mutex);
#endif
	fini();
}

bool ring_buffer_d_thread::init(int nthread,int block_number, int max_block_size,
		  int init_buffer_size, int inc_step, int max_buffer_size)
{
	if(is_init)
	{
		return false;
	}
	if(!data_buff.init(block_number,max_block_size,init_buffer_size,inc_step,max_buffer_size))
	{
		return false;
	}
	this->max_block_size = max_block_size;
	this->blocks = block_number;
	is_init = true;
	threads = nthread;
	return true;
}

bool ring_buffer_d_thread::fini()
{
	if(!is_init || is_run)
	{
		return false;
	}
	data_buff.fini();
	is_init = false;
	return true;
}

bool ring_buffer_d_thread::start()
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

bool ring_buffer_d_thread::stop()
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

bool ring_buffer_d_thread::putd(void* p_data[], int lens[])
{
	if(!is_run)
	{
		return false;
	}
	bool ret = data_buff.putd(p_data, lens);
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
unsigned int ring_buffer_d_thread::run0(void * pVoid)
#else
void* ring_buffer_d_thread::run0(void* pVoid)
#endif
{
	ring_buffer_d_thread* pthis = (ring_buffer_d_thread*)pVoid;
	pthis->run();
	return 0;
}

int ring_buffer_d_thread::run()
{
	char** p_buff = new char*[blocks];
	int* lens = new int[blocks];
	for(int i=0; i<blocks; i++)
	{
		p_buff[i] = new char[max_block_size];
	}
	while(is_run)
	{
#ifdef _WIN32
        //		::WaitForSingleObject(hDataEvent_, INFINITE);
        DWORD dw_size = 0;
        long p1 = 0;
        LPOVERLAPPED p2;
        BOOL bb = ::GetQueuedCompletionStatus(hDataEvent_, &dw_size, (PULONG_PTR)&p1, &p2, INFINITE);
        if(!bb)
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
		while(data_buff.getd((void**)p_buff,lens))
		{
			on_data((void**)p_buff, lens, blocks);
		}
	}

	delete lens;
	for(int i=0; i<blocks; i++)
	{
		delete p_buff[i];
	}
	delete p_buff;
	return 0;
}

