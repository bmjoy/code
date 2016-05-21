#ifndef __MY_REDIS_DEF_H__
#define __MY_REDIS_DEF_H__

struct redis_buf
{
    char* data;
    int   bufsize;
    int   datasize;
    int   offset;
};

struct redis_bulk
{
    char* data;
    int   len;
};

struct redis_bulk_list
{
    int maxcount;
    int response_count;
    int index;
    redis_bulk* bulk_list;
};

#endif//__MY_REDIS_DEF_H__