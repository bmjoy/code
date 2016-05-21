#ifndef __RING_BUFFER_D_H__
#define __RING_BUFFER_D_H__

#include "mutex.h"

class ring_buffer_d
{
public:
	ring_buffer_d();
	~ring_buffer_d();
	bool init(int block_number, int max_block_size,
			  int init_buffer_size, int inc_step, int max_buffer_size);
	bool fini();
	bool putd(void* p_data[], int* lens);
	bool getd(void* p_buff[], int* lens);
	int  get_data_counter();
	int  get_data_size();
	bool reset();
private:
	bool check_data(void* p_data[], int* lens);
	void write_data(char* w_ptr, void* p_data[], int* lens);
	bool expand_tail();
	bool expand_head();
private:
	thread_mutex data_mutex;
	int blocks;

	char* data_ptr;
	int init_size;
	int step_size;
	int max_buffer_size;
	int max_block_size;

	int write_tail;
	int write_head;
	int read_head;

	int data_count;
	int data_size;
};


#endif//__RING_BUFFER_D_H__
