#include "block_buffer.h"
#include <string.h>

void block_buffer::clear()
{
	next_ = 0;
	is_auto_delete_ = false;
	init_len_ = 0;
	rd_ptr_ = 0;
	wr_ptr_ = 0;
	p_data_ = 0;
	data_type_ = BLOCK_TYPE_DATA;
	is_read_only = false;
}

block_buffer::block_buffer()
{
	clear();
}

block_buffer::block_buffer(int len)
{
	clear();
	create(len);
}

block_buffer::block_buffer(block_buffer& bb)
{
	clear();
	create(bb.get_length());
	reset();
	write(bb.base_ptr(), (bb.get_length()));
	wr_ptr((int)(bb.wr_ptr() - bb.base_ptr()));
	rd_ptr((int)(bb.rd_ptr() - bb.base_ptr()));
}

block_buffer::block_buffer(char* p_data, int data_len, bool is_copy)
{
	clear();
	if (is_copy)
	{
		create(data_len);
		write(p_data, data_len);
		reset();
	} else
	{
		p_data_ = p_data;
		init_len_ = data_len;
		is_auto_delete_ = false;
	}
}

block_buffer::~block_buffer(void)
{
	if (is_auto_delete_ && (p_data_ != 0))
	{
		delete p_data_;
		p_data_ = 0;
	}
}

bool block_buffer::create(int data_len)
{
	if (is_auto_delete_ && (p_data_ != 0))
	{
		delete p_data_;
		p_data_ = 0;
        init_len_ = 0;
	} else
	{
		if (p_data_ != 0)
			return false;
	}
	is_auto_delete_ = false;
    if(data_len == 0)
        return true;
	p_data_ = new char[data_len];
	memset(p_data_, 0, data_len);

	if (p_data_ == 0)
		return false;
	init_len_ = data_len;
	is_auto_delete_ = true;

	return true;
}

int block_buffer::get_length()
{
	return init_len_;
}

int block_buffer::data_len()
{
	return wr_ptr_;
}

char* block_buffer::base_ptr()
{
	return p_data_;
}

void block_buffer::reset()
{
	rd_ptr_ = 0;
	wr_ptr_ = 0;
	data_type_ = BLOCK_TYPE_DATA;
}

void block_buffer::reset_wr_ptr(int l)
{
	wr_ptr_ = l;
}

void block_buffer::reset_rd_ptr(int l)
{
	rd_ptr_ = l;
}

void block_buffer::arrange_data()
{
	int l = rest_data_len();
	if (l>0)
	{
		if(rd_ptr_>0)
		{
			memcpy(p_data_, &p_data_[rd_ptr_], l);
			wr_ptr_ = l;
			rd_ptr_ = 0;
		}
	} else
	{
		wr_ptr_ = 0;
		rd_ptr_ = 0;
	}
}

block_buffer* block_buffer::next()
{
	return next_;
}

void block_buffer::next(block_buffer* p_net)
{
	next_ = p_net;
}

char* block_buffer::rd_ptr()
{
	return(p_data_ + rd_ptr_);
}

bool block_buffer::rd_ptr(char* p_rd_ptr)
{
	if (((int)(p_rd_ptr - p_data_)) > init_len_)
		return false;
	rd_ptr_ = (int)(p_rd_ptr - p_data_);
	return true;
}

bool block_buffer::rd_ptr(int l)
{
	if ((rd_ptr_ + l) > init_len_)
		return false;
	rd_ptr_ += l;
	return true;
}

char* block_buffer::wr_ptr()
{
	return(p_data_ + wr_ptr_);
}

bool block_buffer::wr_ptr(char* p_wr_ptr)
{
	if (((p_wr_ptr - p_data_)) > init_len_)
		return 0;
	wr_ptr_ = (int)(p_wr_ptr - p_data_);
	return true;
}

bool block_buffer::wr_ptr(int l)
{
	if ((wr_ptr_ + l) > init_len_)
		return false;
	wr_ptr_+= l;
	return true;
}

int block_buffer::write(const char* data, int len)
{
	if (is_read_only)
		return 0;
	if (wr_ptr_ == init_len_)
		return 0;
	if ((len + wr_ptr_) > init_len_)
		len = init_len_ - wr_ptr_;
	memcpy((char*)(p_data_ + wr_ptr_), data, len);
	wr_ptr_ += len;
	return len;
}

int block_buffer::read(char* buff, int len)
{
	if (rd_ptr_ == init_len_)
		return 0;
	if ((len + rd_ptr_) > init_len_)
		len = init_len_ - rd_ptr_;
	memcpy(buff, (char*)(p_data_+rd_ptr_), len);
	rd_ptr_ += len;
	return len;
}

int block_buffer::write(char* psz_data)
{
	if (0 == psz_data)
		return 0;
	int len = (int)strlen(psz_data);
	if (wr_ptr_ == init_len_)
		return 0;
	if ((len + wr_ptr_ + (int)sizeof(int)) > init_len_)
		return 0;
	write((char*)&len, sizeof(len));
	return write((char*)psz_data, (int)len);
}

int block_buffer::read(char* psz_buff)
{
	if (0 == psz_buff)
		return 0;
	if (rd_ptr_ == init_len_)
		return 0;
	int len;
	read((char*)&len, sizeof(len));
	int ret = read((char*)psz_buff, (int)len);
	psz_buff[len] = 0;
	return ret;
}

int block_buffer::rest_data_len()
{
	return(wr_ptr_ - rd_ptr_);
}

int block_buffer::rest_free_len()
{
	return(init_len_ - wr_ptr_);
}

int block_buffer::data_type()
{
	return data_type_;
}
void block_buffer::data_type(DATATYPEENUM t)
{
	data_type_ = t;
}
//---------------------------------------------------------
block_buffer_pool::block_buffer_pool()
{
	is_init = false;
	p_head = 0;
	used_count = 0;
}

block_buffer_pool::~block_buffer_pool()
{
	if (is_init)
	{
		fini();
	}
}

bool block_buffer_pool::init(int buffer_size, int init_block_number, int inc_step)
{
	guard g(mutex);
	if (is_init)
	{
		return false;
	}
	m_buff_size = buffer_size;
	expand_step = inc_step;
	if (expand_step<10)
	{
		expand_step = 10;
	}

	for (int i=0; i<init_block_number; i++)
	{
		block_buffer *p = new block_buffer(m_buff_size);
		backup.push_back(p);
		p->next(p_head);
		p_head = p;
	}
	return true;
}

void block_buffer_pool::fini()
{
	guard g(mutex);
	block_buffer_iterator it;
	for (it=backup.begin(); it!=backup.end(); it++)
	{
		delete (*it);
	}
	backup.clear();
	p_head = 0;
	used_count = 0;
	is_init = false;
}

block_buffer* block_buffer_pool::get()
{
	guard g(mutex);
	block_buffer* p = 0;
	if (p_head)
	{
		p = p_head;
		p_head = p->next();
		p->next(0);
	} else
	{
		p = expand();
	}
	if (p)
	{
		used_count ++;
	}
	p->reset();
	return p;
}

void block_buffer_pool::put(block_buffer* pb)
{
	guard g(mutex);
	if (!pb)
	{
		return;
	}
	pb->next(p_head);
	p_head = pb;
	used_count --;
}

int block_buffer_pool::get_used_count()
{
	return used_count;
}

block_buffer* block_buffer_pool::expand()
{
	block_buffer* p = 0;
	for (int i=0; i<expand_step; i++)
	{
		p = new block_buffer(m_buff_size);
		backup.push_back(p);
		
		if (i < (expand_step-1))
		{
			p->next(p_head);
			p_head = p;
		} else
		{
			break;
		}
	}
	return p;
}

void block_buffer_pool::shrink()
{
}

