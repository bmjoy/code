#ifndef __IP_HUNTER_H__
#define __IP_HUNTER_H__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
功能： IP 地址查询
auth： msl
*/
//测试使用，读取日志文件
extern int read_line(FILE* f, char* buf, int buflen);

//////////////////////////////////////////////////////////////////////////
/*
初始化IP库。
ip库文件格式必须为
IP_FROM \t IP_TO \t 省 \t 市 \t 区 \t 网段
*/
extern bool init_ip_hunter(const char* filename);
extern void fini_ip_hunter();
/*
查找IP归属地
site1: 省
site2: 市
site3: 区/县
site4: 网段
如果找到返回true, 否则返回false
*/
extern bool hunter_ip_site(const char* ip,  char** site1, char** site2, char** site3, char** site4, int* sitecode);

#endif//__IP_HUNTER_H__