#include "ring_buffer_d.h"
#include <string.h>

ring_buffer_d::ring_buffer_d()
{
	data_ptr = 0;
	blocks = 0;
	init_size = 0;
	step_size = 0;
	max_buffer_size = 0;
	max_block_size = 0;

	write_tail = 0;
	write_head = 0;
	read_head = 0;

	data_count = 0;
	data_size = 0;

}

ring_buffer_d::~ring_buffer_d()
{
}

bool ring_buffer_d::init(int block_number, int max_block_size,
						 int init_buffer_size, int inc_step, int max_buffer_size)
{
	guard g(data_mutex);
	if (data_ptr)
	{
		return false;
	}
	if (block_number<1 || block_number>5)
	{
		return false;
	}
	if (max_block_size*block_number > inc_step)
	{
		return false;
	}
	blocks = block_number;
	this->max_block_size = max_block_size;
	this->max_buffer_size = max_buffer_size;
	this->init_size = init_buffer_size;
	this->step_size = inc_step;
	data_ptr = new char[init_size];
	return true;
}

bool ring_buffer_d::fini()
{
	guard g(data_mutex);
	if (!data_ptr)
	{
		return false;
	}
	delete data_ptr;
	data_ptr = 0;
	blocks = 0;
	init_size = 0;
	step_size = 0;
	max_buffer_size = 0;
	max_block_size = 0;

	write_tail = 0;
	write_head = 0;
	read_head = 0;

	data_count = 0;
	data_size = 0;

	return false;
}

bool ring_buffer_d::putd(void* p_data[], int* lens)
{
	guard g(data_mutex);
	if (!check_data(p_data, lens))
	{
		return false;
	}
	int need_size = blocks * sizeof(int);
	for (int i=0; i<blocks; i++)
	{
		need_size += lens[i];
	}
	bool ret = false;
	do
	{
		if (write_head == write_tail)
		{
			int tail_free_size = init_size - write_head;
			if (tail_free_size > need_size)// enough free size of tail
			{
				write_data(&data_ptr[write_head], p_data, lens);
				write_head += need_size;
				write_tail = write_head;
			} else if (read_head > need_size)// enough free size of head
			{
				write_data(data_ptr, p_data, lens);
				write_head = need_size;
			} else
			{
				// if can expended buff
				if (!expand_tail())
				{
					break;
				}
				write_data(&data_ptr[write_head], p_data, lens);
				write_head += need_size;
				write_tail = write_head;
			}
		} else
		{
			int midle_free_size = read_head - write_head;
			if (midle_free_size > need_size)
			{
				write_data(&data_ptr[write_head], p_data, lens);
				write_head += need_size;
			} else
			{
				if (!expand_head())
				{
					break;
				}
				write_data(&data_ptr[write_head], p_data, lens);
				write_head += need_size;
				write_tail = write_head;
			}
		}
		ret = true;
	} while (false);
	if (ret)
	{
		data_count ++;
		data_size += need_size;
	}
	return ret;
}

bool ring_buffer_d::check_data(void* p_data[], int* lens)
{
	if (!p_data || !lens)
	{
		return false;
	}
	bool ret = false;
	for (int i=0; i<blocks; i++)
	{
		if (p_data[i] == 0 && lens[i] != 0)
		{
			return false;
		}
		if (lens[i] > max_block_size)
		{
			return false;
		}
		ret = true;
	}
	return ret;
}

void ring_buffer_d::write_data(char* w_ptr, void* p_data[], int* lens)
{
	int* p_lens = (int*)w_ptr;
	char* p = w_ptr + (blocks * sizeof(int));
	for (int i=0; i<blocks; i++)
	{
		p_lens[i] = lens[i];
		memcpy(p, p_data[i], lens[i]);
		p += lens[i];
	}
}

bool ring_buffer_d::expand_tail()
{
	if ((init_size + step_size) > max_buffer_size)
	{
		return false;
	}
	init_size += step_size;
	char* p_tmp = new char[init_size];
	memcpy(p_tmp, &data_ptr[read_head], (write_head - read_head));
	write_head = (write_head - read_head);
	write_tail = write_head;
	read_head = 0;
	delete data_ptr;
	data_ptr = p_tmp;
	return true;
}

bool ring_buffer_d::expand_head()
{
	if ((init_size + step_size) > max_buffer_size)
		return false;
	init_size += step_size;
	char* p_tmp = new char[init_size];
	int tail_data_size = write_tail - read_head;
	int head_data_size = write_head;
	memcpy(p_tmp, &data_ptr[read_head], tail_data_size);
	memcpy(&p_tmp[tail_data_size], data_ptr, head_data_size);
	delete[] data_ptr;
	data_ptr = p_tmp;

	write_head = head_data_size + tail_data_size;
	write_tail = write_head;
	read_head = 0;
	return true;
}

bool ring_buffer_d::getd(void* p_buff[], int* lens)
{
	guard g(data_mutex);
	//has not initialize
	if (!data_ptr)
	{
		return false;
	}
	//empty 
	if (read_head == write_tail)
		return false;

	int* p_len = (int*)&data_ptr[read_head];
	char* p = &data_ptr[read_head + (blocks*sizeof(int))];
	int read_size = blocks * sizeof(int);
	for (int i=0; i<blocks; i++)
	{
		lens[i] = p_len[i];
		memcpy(p_buff[i], p, lens[i]);
		p += lens[i];
		read_size += lens[i];
	}
	read_head += read_size;

	if(write_tail == read_head && write_head == read_head)
	{
		read_head = 0;
		write_head= 0;
		write_tail = 0;
	}
	else if(write_tail > write_head && write_tail == read_head)
	{
		read_head = 0;
		write_tail = write_head;
	}

	data_count --;
	data_size -= read_size;

	return true;
}

int ring_buffer_d::get_data_counter()
{
	return data_count;
}

int ring_buffer_d::get_data_size()
{
	return data_size;
}

bool ring_buffer_d::reset()
{
	guard g(data_mutex);
	if(!data_ptr)
	{
		return false;
	}
	write_head = 0;
	write_tail = 0;
	read_head = 0;
	data_count = 0;
	data_size = 0;
	return true;
}
