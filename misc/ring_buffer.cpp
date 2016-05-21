#include "ring_buffer.h"
#include <string.h>
#include <iostream>
using namespace std;

struct ring_buff_head
{
	int size;
};
const int BUFF_HEAD_SIZE = sizeof(ring_buff_head);

ring_buffer::ring_buffer()
{
	data_ptr = 0;
	init_size = 0;
	step_size = 0;
	max_size = 0;
	max_block_size = 0;

	write_tail = 0;
	write_head = 0;
	read_head = 0;

	data_count = 0;
	data_size = 0;
}

ring_buffer::~ring_buffer()
{
	fini();
}

bool ring_buffer::init(int _init_size, int _step_size,
					   int _max_size, int _max_block_size)
{
	if(data_ptr)
	{
		return false;
	}
	init_size = _init_size;
	step_size = _step_size;
	max_size = _max_size;
	max_block_size = _max_block_size;

	// the step_size must big than each data block size
	if(step_size<(max_block_size+BUFF_HEAD_SIZE) )
	{
		return false;
	}

	data_ptr = new char[init_size];
	return reset();
}

bool ring_buffer::fini()
{
	if(!data_ptr)
	{
		return false;
	}
	delete[] data_ptr;

	data_ptr = 0;
	init_size = 0;
	step_size = 0;
	max_size = 0;
	max_block_size = 0;

	write_tail = 0;
	write_head = 0;
	read_head = 0;
	return true;
}

bool ring_buffer::reset()
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

int ring_buffer::get_data_count()
{
	guard g(data_mutex);
	if(!data_ptr)
	{
		return -1;
	}
	return data_count;
}

int ring_buffer::get_data_size()
{
	guard g(data_mutex);
	if(!data_ptr)
	{
		return -1;
	}
	return data_size;
}

bool ring_buffer::putd(void* pdata, int len)
{
	guard g(data_mutex);
	// has not initialize
	if(!data_ptr)
	{
		return false;
	}
	// illegal paramet
	if((len > max_block_size) || (pdata == 0) || (len <= 0))
	{
		return false;
	}

	int need_size = BUFF_HEAD_SIZE + len;
	bool ret = true;
	do
	{
		if(write_head == write_tail)
		{
			int tail_free_size = init_size - write_head;
			// enough free size of tail
			if(tail_free_size > need_size)
			{
				ring_buff_head* head = (ring_buff_head*)&data_ptr[write_head];
				head->size = len;
				memcpy(&data_ptr[write_head+BUFF_HEAD_SIZE], pdata, len);
				write_head += need_size;
				write_tail = write_head;
			}
			else if(read_head > need_size)// enough free size of head
			{
				ring_buff_head* head = (ring_buff_head*)data_ptr;
				head->size = len;
				memcpy(&data_ptr[BUFF_HEAD_SIZE], pdata, len);
				write_head = need_size;
			}
			else
			{
				// if can expended buff
				if((init_size+step_size) > max_size)
				{
					ret = false;
					break;
				}
				init_size += step_size;
				char* p_tmp = new char[init_size];
				memcpy(p_tmp, &data_ptr[read_head], (write_head - read_head));
				write_head = (write_head - read_head);
				write_tail = write_head;
				read_head = 0;

				delete[] data_ptr;
				data_ptr = p_tmp;
				ring_buff_head* head = (ring_buff_head*)&data_ptr[write_head];
				head->size = len;
				memcpy(&data_ptr[write_head + BUFF_HEAD_SIZE], pdata, len);
				write_head += need_size;
				write_tail = write_head;
			}
		}
		else
		{
			int midle_free_size = read_head - write_head;
			if(midle_free_size > need_size)
			{
				ring_buff_head* head = (ring_buff_head*)&data_ptr[write_head];
				head->size = len;
				memcpy(&data_ptr[write_head+BUFF_HEAD_SIZE], pdata, len);
				write_head += need_size;
			}
			else
			{
				if((init_size+step_size) > max_size)
				{
					ret = false;
					break;
				}
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

				ring_buff_head* head = (ring_buff_head*)&data_ptr[write_head];
				head->size = len;
				memcpy(&data_ptr[write_head+BUFF_HEAD_SIZE], pdata, len);
				write_head += need_size;
				write_tail = write_head;
			}
		}
	}while(false);

	if(ret)
	{
		data_count ++;
		data_size += len;
	}

	return ret;
}

bool ring_buffer::getd(void* pbuff, int& len)
{
	guard g(data_mutex);
	if(!data_ptr)
	{
		return false;
	}
	if(read_head == write_tail)
		return false;

	ring_buff_head* head;
	head = (ring_buff_head*)&data_ptr[read_head];
	int dl = head->size;
	if(dl > len)
	{
		return false;
	}
	len = dl;

	memcpy(pbuff, &data_ptr[read_head + BUFF_HEAD_SIZE], len);
	read_head += (len + BUFF_HEAD_SIZE);

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
	data_size -= len;
	
	return true;
}

