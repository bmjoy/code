/*
-----------------------------------------------------------------------------

File name       :   character_interface.h         
Author          :   侯伽健     
Version         :   1.0            
Date            :   2013.03.09     
Description     :   字符集转换  

-----------------------------------------------------------------------------
*/
#ifndef _CHARACTER_SET_HEAD___
#define _CHARACTER_SET_HEAD___


/***************************************************************************** 
* 将一个字符的unicode编码转换成gbk编码. 
* 
* 参数: 
*    unic_str    指向输入缓冲区, 以unicode编码 
*    str_len     unicode字符个数, 
*    gbk_buf     指向输出缓冲区, 以gbk编码 
*    gbk_buf_len 输出缓冲区大小, 
* 
* 返回值: 
*    成功则返回该字符的gbk编码所占用的字节数; 失败则返回0. 
****************************************************************************/ 
int _unicode_to_gbk(const wchar_t* unic_str,int str_len,char* gbk_buf,int gbk_buf_len);

/***************************************************************************** 
* 将一个字符的gbk编码转换成unicode编码. 
* 
* 参数: 
*    gbk_str      指向输入缓冲区, 以gbk编码 
*    str_len      输入缓冲区大小, 
*    unic_buf     指向输出缓冲区, 以unicode编码 
*    unic_buf_len 输出缓冲区最大支持字符个数（不是SIZEOF（））, 
* 
* 返回值: 
*    成功则返回该字符的unicode编码字符数; 失败则返回0. 
****************************************************************************/ 
int _gbk_to_unicode(const char* gbk_str,int str_len,wchar_t* unic_buf,int unic_buf_len);



/***************************************************************************** 
* 将一个字符的unicode编码转换成utf8编码. 
* 
* 参数: 
*    unic_str      指向输入缓冲区, 以unicode编码 
*    str_len       unicode字符个数, 
*    utf8_buf      指向输出缓冲区, 以utf8编码 
*    utf8_buf_size 输出缓冲区大小, 
* 
* 返回值: 
*    成功则返回该字符的utf8编码所占用的字节数; 失败则返回0. 
****************************************************************************/ 
int _unicode_to_utf8(const wchar_t* unic_str,int str_len,char* utf8_buf,int utf8_buf_size);

/***************************************************************************** 
* 将一个字符的utf8编码转换成unicode编码. 
* 
* 参数: 
*    utf8_str      指向输入缓冲区, 以utf8编码 
*    str_len       输入缓冲区大小, 
*    unic_buf      指向输出缓冲区, 以unicode编码 
*    unic_buf_len  输出缓冲区最大支持字符个数（不是SIZEOF（））, 
* 
* 返回值: 
*    成功则返回该字符的unicode编码字符数; 失败则返回0. 
****************************************************************************/ 
int _utf8_to_unicode(const char* utf8_str,int utf8_size,wchar_t* unic_buf,int unic_buf_len);



/***************************************************************************** 
* 将一个字符的utf8编码转换成gbk编码. 
* 
* 参数: 
*    utf8_str      指向输入缓冲区, 以utf8编码 
*    utf8_size     输入缓冲区大小, 
*    gbk_buf       指向输出缓冲区, 以gbk编码 
*    gbk_buf_size  输出缓冲区大小 
* 
* 返回值: 
*    成功则返回该字符的gbk编码所占用的字节数; 失败则返回0. 
****************************************************************************/ 
int _utf8_to_gbk(const char* utf8_str,int utf8_size,char* gbk_buf,int gbk_buf_size);

/***************************************************************************** 
* 将一个字符的gbk编码转换成utf8编码. 
* 
* 参数: 
*    gbk_str        指向输入缓冲区, 以gbk编码 
*    gbk_size       输入缓冲区大小, 
*    utf8_buf       指向输出缓冲区, 以utf8编码 
*    utf8_buf_size  输出缓冲区大小 
* 
* 返回值: 
*    成功则返回该字符的utf8编码所占用的字节数; 失败则返回0. 
****************************************************************************/ 
int _gbk_to_utf8(const char* gbk_str,int gbk_size,char* utf8_buf,int utf8_buf_size);



#endif//_CHARACTER_SET_HEAD___