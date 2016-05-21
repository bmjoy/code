#ifndef _RING_BUFFER_D_THREAD_H__
#define _RING_BUFFER_D_THREAD_H__
#include "mutex.h"
#include "ring_buffer_d.h"

class ring_buffer_d_thread
{
public:
	ring_buffer_d_thread();
	~ring_buffer_d_thread();

	bool init(int nthread,int block_number, int max_block_size,
			  int init_buffer_size, int inc_step, int max_buffer_size);
	bool fini();
	bool start();
	bool stop();

	virtual void on_data(void* p_data[], int lens[], int size) = 0;
	bool putd(void* p_data[], int lens[]);
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
	int             blocks;
#ifdef _WIN32
	HANDLE			hDataEvent_;
#else
	pthread_attr_t  attr;
	pthread_cond_t  data_cond;
	pthread_mutex_t data_mutex;
#endif
	ring_buffer_d   data_buff;

};
#endif//_RING_BUFFER_D_THREAD_H__
