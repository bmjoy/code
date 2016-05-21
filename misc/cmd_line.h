#ifndef __CMD_LINE_H__
#define __CMD_LINE_H__
#include "pptr_buffer.h"
#include <stdlib.h>
#include <string.h>

/*
两个'-'开头的作为键值对
--key=value

一个'-'开头的，每个字符作为一个开关
-abcd

其他每个字符串作为一个开关或标识

*/

template<int MAX_BUF_SIZE, int MAX_PTR_SIZE>
class cmd_line_parser
{
public:
    cmd_line_parser(){};
    ~cmd_line_parser(){};
    int parse_cmd(int argc, char** argv);
    int has_tag(const char* tag);
    int get_int(const char* key, int defaultvalue);
    int get_string(const char* key, char* value, int buflen, const char* defaultvalue);
    int get_keys();
//private:
    pptr_buffer<MAX_BUF_SIZE, MAX_PTR_SIZE> m_key;
    pptr_buffer<MAX_BUF_SIZE, MAX_PTR_SIZE> m_value;
    pptr_buffer<MAX_BUF_SIZE, MAX_PTR_SIZE> m_tag;
};

template<int MAX_BUF_SIZE, int MAX_PTR_SIZE>
int cmd_line_parser<MAX_BUF_SIZE, MAX_PTR_SIZE>::parse_cmd(int argc, char** argv)
{
    for(int i=1; i<argc; i++)
    {
        if(argv[i][0]=='-' && argv[i][1] == '-')
        {
            char key[1024];
            char value[1024];
            int len = strlen(argv[i]);
            int j = 0;
            for(j=2; j<len; j++)
            {
                if(argv[i][j] == '=')
                {
                    key[j-2] = 0;
                    break;
                }
                key[j-2] = argv[i][j];
            }
            strcpy(value, &argv[i][j+1]);
            m_key.add(key);
            m_value.add(value);            
        }
        else if(argv[i][0] == '-')
        {
            if(strlen(argv[i]) < 2)
                return -1;
            char t[2] = {0};
            for(int j=1; j<strlen(argv[i]); j++)
            {
                t[0] = argv[i][j];
                m_tag.add(t);
            }
        }
        else
        {
            m_tag.add(argv[i]);
        }
    }
    return 0;
}

template<int MAX_BUF_SIZE, int MAX_PTR_SIZE>
int cmd_line_parser<MAX_BUF_SIZE, MAX_PTR_SIZE>::has_tag(const char* tag)
{
    for(int i=0; i<m_tag.datacount; i++)
    {
        if(strcmp(tag, m_tag.pptr[i]) == 0)
            return 1;
    }
    return 0;
}

template<int MAX_BUF_SIZE, int MAX_PTR_SIZE>
int cmd_line_parser<MAX_BUF_SIZE, MAX_PTR_SIZE>::get_int(const char* key, int defaultvalue)
{
    for(int i=0; i<m_tag.datacount; i++)
    {
        if(strcmp(key, m_key.pptr[i]) == 0)
            return atoi(m_value.pptr[i]);
    }
    
    return defaultvalue;
}

template<int MAX_BUF_SIZE, int MAX_PTR_SIZE>
int cmd_line_parser<MAX_BUF_SIZE, MAX_PTR_SIZE>::get_string(const char* key, char* value, int buflen, const char* defaultvalue)
{
    for(int i=0; i<m_tag.datacount; i++)
    {
        if(strcmp(key, m_key.pptr[i]) == 0)
        {
            strncpy(value, m_value.pptr[i], buflen);
            return 0;
        }
    }
    strncpy(value, defaultvalue, buflen);
    return 1;
}

template<int MAX_BUF_SIZE, int MAX_PTR_SIZE>
int cmd_line_parser<MAX_BUF_SIZE, MAX_PTR_SIZE>::get_keys()
{
    return m_key.datacount;
}

#endif//__CMD_LINE_H__