#ifndef __STRING_RES_H__
#define __STRING_RES_H__
#include "singleton.h"
#include <vector>
#include <map>
#include <string>
using namespace std;
enum STRING_INDEX_MODE
{
    STRING_INDEX_VECTOR,
    STRING_INDEX_MAP
};

class string_res : public singleton<string_res>
{
public:
    string_res();
    ~string_res();
    bool init(char* filename, int mode);
    void fini();
    const char* get_string(int id);
private:
    bool parse_vector_mode(FILE* file);
    bool parse_map_mode(FILE* file);
    map<int,string> m_str_map;
    vector<string>  m_str_vector;
    int m_mode;
};

typedef singleton<string_res> string_res_singleton;

#define STRING_INST string_res::instance()

#define STRING_RES STRING_INST->get_string

#endif//__STRING_RES_H__