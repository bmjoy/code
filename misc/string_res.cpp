#include "string_res.h"
#include "vector_helper.h"
#include <sys/stat.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
#ifdef _WIN32
#pragma warning(disable:4996)
#else
#include <string.h>
#endif

string_res::string_res()
{
    m_mode = -1;
}

string_res::~string_res()
{
    fini();
}

bool string_res::init(char* filename, int mode)
{
    m_mode = mode;
    if(m_mode != STRING_INDEX_VECTOR &&
       m_mode != STRING_INDEX_MAP)
       return false;
    if(!filename)
        return false;
    struct stat buf;
    int iresult = stat(filename, &buf);
    if(iresult != 0)
    {
        return false;
    }

    FILE* file = fopen(filename, "rb");
    if(!file)
        return false;

    bool result = false;
    switch(m_mode)
    {
    case STRING_INDEX_VECTOR:
        result = parse_vector_mode(file);
        break;
    case STRING_INDEX_MAP:
        result = parse_map_mode(file);
        break;
    default:
        break;
    }
    fclose(file);
    return result;
}

bool string_res::parse_vector_mode(FILE* file)
{
    char buf[4096];
    char sz_id[32];
    char sz_string[4000];
    int max_id = 0;
    while(fgets(buf, sizeof(buf), file))
    {
        int l = strlen(buf);
        int si = 0;
        for(int i=0; i<32; i++)
        {
            if(buf[i] != '=')
                sz_id[i] = buf[i];
            else
            {
                sz_id[i] = 0;
                si = i+1;
                break;
            }
        }
        if(strlen(sz_id) == 0)
            return false;
        int id = atoi(sz_id);
        if(id > max_id)
            max_id = id;
    }
    m_str_vector.resize(max_id+1);
    fseek(file, 0, SEEK_SET);
    while(fgets(buf, sizeof(buf), file))
    {
        int l = strlen(buf);
        int si = 0;
        for(int i=0; i<32; i++)
        {
            if(buf[i] != '=')
                sz_id[i] = buf[i];
            else
            {
                sz_id[i] = 0;
                si = i+1;
                break;
            }
        }
        if(strlen(sz_id) == 0)
            return false;
        int id = atoi(sz_id);
        int index = 0;
        for(int i=si; i<l; i++)
        {
            if(buf[i] != '\r' && buf[i]!='\n')
            {
                sz_string[index] = buf[i];
                index ++;
            }
        }
        sz_string[index] = 0;
        m_str_vector[id] = string(sz_string);
    }

    return true;
}

bool string_res::parse_map_mode(FILE* file)
{
    char buf[4096];
    char sz_id[32];
    char sz_string[4000];
    while(fgets(buf, sizeof(buf), file))
    {
        int l = strlen(buf);
        int si = 0;
        for(int i=0; i<32; i++)
        {
            if(buf[i] != '=')
                sz_id[i] = buf[i];
            else
            {
                sz_id[i] = 0;
                si = i+1;
                break;
            }
        }
        if(strlen(sz_id) == 0)
            return false;
        int id = atoi(sz_id);
        int index = 0;
        for(int i=si; i<l; i++)
        {
            if(buf[i] != '\r' && buf[i]!='\n')
            {
                sz_string[index] = buf[i];
                index ++;
            }
        }
        sz_string[index] = 0;
        m_str_map[id] = string(sz_string);
    }
    return true;
}

void string_res::fini()
{
    release_vector(m_str_vector);
    m_str_map.clear();
}

const char* string_res::get_string(int id)
{
    switch(m_mode)
    {
    case STRING_INDEX_VECTOR:
        if(id>=0 && id < (int)m_str_vector.size() && m_str_vector[id].length()>0)
            return m_str_vector[id].c_str();
        return 0;
    case STRING_INDEX_MAP:
        {
            map<int,string>::iterator i;
            i = m_str_map.find(id);
            if(i != m_str_map.end() && i->second.length() > 0)
                return i->second.c_str();
            return 0;
        }
        break;
    default:
        break;
    }
    return 0;
}
