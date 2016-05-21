#ifndef __STR_TOOLS_H__
#define __STR_TOOLS_H__

extern int utf8_2_gb2312(const char* src, int len, char* out, int bufsize);
extern int gb2312_2_utf8(const char* src, int len, char* out, int bufsize);


#endif//__STR_TOOLS_H__