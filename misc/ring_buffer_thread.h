#ifndef _RING_BUFFER_THREAD_H__
#define _RING_BUFFER_THREAD_H__

#include "ring_buffer.h"

class ring_buffer_thread
{
public:
	ring_buffer_thread();
	~ring_buffer_thread();
	bool init(int _threads, int _init_size, int _step_size,
			  int _max_size, int _max_block_size);
	bool fini();
	bool start();
	bool stop();

	virtual void on_data(void* pdata, int len) = 0;
	bool put_data(void* pdata, int len);

private:
#ifdef _WIN32
	static unsigned int __stdcall run0(void * pVoid);
#else
	static void* run0(void* pVoid);
#endif
	int run();

private:
	bool            is_init;
	bool            is_run;
	int             threads;
	int             max_block_size;
#ifdef _WIN32
	HANDLE			hDataEvent_;
#else
	pthread_attr_t  attr;
	pthread_cond_t  data_cond;
	pthread_mutex_t data_mutex;
#endif
	ring_buffer     data_buff;
};

#endif//_RING_BUFFER_THREAD_H__





