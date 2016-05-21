/*
auth:  Ishaq
email: msl_cn@126.com
*/
#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__
#include "mutex.h"

class ring_buffer
{
public:
	ring_buffer();
	~ring_buffer();
	bool init(int _init_size, int _step_size,
			  int _max_size, int _max_block_size);
	bool fini();
	bool reset();
	bool putd(void* pdata, int len);
	bool getd(void* pbuff, int& len);
	int  get_data_count();
	int  get_data_size();
private:
	thread_mutex data_mutex;
	char* data_ptr;
	int init_size;
	int step_size;
	int max_size;
	int max_block_size;

	int write_tail;
	int write_head;
	int read_head;

	int data_count;
	int data_size;
};


#endif//__RING_BUFFER_H__
