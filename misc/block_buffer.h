#ifndef __BLOCK_BUFFER_H__
#define __BLOCK_BUFFER_H__
#include <list>
#include "mutex.h"
using namespace std;

#define DEFAULT_BUFFER_SIZE 4096

class block_buffer
{
public:
	block_buffer(void);
	block_buffer(int len);
	block_buffer(block_buffer& bb);
	block_buffer(char* p_data, int data_len, bool is_copy = false);
	~block_buffer(void);

	bool create(int data_len);
	int get_length();
	int data_len();
	char* base_ptr();
	void reset();
	void reset_wr_ptr(int l=0);
	void reset_rd_ptr(int l=0);
	void arrange_data();
	block_buffer* next();
	void next(block_buffer* next);
	char* rd_ptr();
	bool rd_ptr(char* p_rd_ptr);
	bool rd_ptr(int l);
	char* wr_ptr();
	bool wr_ptr(char* pWrPtr);
	bool wr_ptr(int l);
	int write(const char* p_data, int len);
	int read(char* p_buff, int len);
	int rest_free_len();
	int rest_data_len();
	int write(char* psz_data);
	int read(char* psz_buff);
	template<class T> inline int write(T val)
	{
		return write((char *)&val,sizeof(T));
	}
	template<class T> inline int read(T &val)
	{
		return read((char *)&val,sizeof(T));
	}
	template<class T> block_buffer& operator << (T &val)
	{
		write((char*)&val, sizeof(val));
		return *this;
	}
	template<class T> block_buffer& operator >> (T &val)
	{
		read((char*)&val, sizeof(val));
		return *this;
	}
	block_buffer& operator << (char* data){write(data); return *this;}
	block_buffer& operator >> (char* buff){read(buff); return *this;}
	enum DATATYPEENUM
	{
		BLOCK_TYPE_DATA,
		BLOCK_TYPE_END,
	}; 
	int data_type();
	void data_type(DATATYPEENUM t);

protected:
	void clear();

private:
	bool is_read_only;
	block_buffer* next_;
	bool is_auto_delete_;
	int init_len_;
	int rd_ptr_;
	int wr_ptr_;
	char* p_data_;
	int data_type_;

};

typedef list<block_buffer*> block_buffer_list;
typedef block_buffer_list::iterator block_buffer_iterator;

class block_buffer_pool
{
public:
	block_buffer_pool();
	~block_buffer_pool();
	bool init(int buffer_size, int init_block_number, int inc_step);
	void fini();
	block_buffer* get();
	void put(block_buffer* pb);
	int get_used_count();
private:
	block_buffer* expand();
	void shrink();
private:
	bool is_init;
	int  expand_step;
	int  used_count;
	int  m_buff_size;
	block_buffer* p_head;
	thread_mutex  mutex;
	block_buffer_list backup;
};

#endif//__BLOCK_BUFFER_H__
