/*
-----------------------------------------------------------------------------

File name       :   character_set.cpp        
Author          :   侯伽健     
Version         :   1.0            
Date            :   2013.03.09     
Description     :   字符集转换  

-----------------------------------------------------------------------------
*/

#include "character_interface.h" 
#include "gbk_unicode_table.h"
#include <string.h>

inline int one_unicode_to_gbk(const wchar_t unic,char* gbk_buf,int gbk_buf_size)
{
	if(unic < 0x80)
	{
		if(!gbk_buf || gbk_buf_size < 1) return 0;
		*gbk_buf = (unsigned char)unic;
		return 1;
	}
	else
	{
		if(!gbk_buf || gbk_buf_size < 2) return 0;
		const wchar_t & c = _gbk_code(unic);
		if(0 == c)
		{
			gbk_buf[0] = '?';
			gbk_buf[1] = '?';
		}
		else
		{
			gbk_buf[0] = c >> 8;
			gbk_buf[1] = c &  0xff;
		}
		return 2;
	}
}


inline int gbk_to_one_unicode(const unsigned char* gbk,int gbk_len, wchar_t *punic)
{
	if(!gbk) return 0;
	if(*gbk < 0x80)
	{
		if(gbk_len < 1) return 0;
		*punic = *gbk;
		return 1;
	}
	else
	{
		if(gbk_len < 2) return 0;

		*punic = _uni_code((int(*gbk) << 8) + *(gbk+1));
		if(*punic == 0)
		{
			*punic = 0xFF1F; //？
		}

		return 2;
	}
}

inline int one_unicode_to_utf8(wchar_t unic, unsigned char *utf8_buf,int utf8_buf_size)  
{ 
	if (unic <= 0x7F)  // * U-00 - U-7F:  0xxxxxxx 
	{ 
		if(!utf8_buf || utf8_buf_size < 1) return 0;
		*utf8_buf  = (unic & 0x7F);  
		return 1;  
	}    
	else if (unic >= 0x80 && unic <= 0x07FF)// * U-80 - U-0x07FF:  110xxxxx 10xxxxxx  
	{  
		if(!utf8_buf || utf8_buf_size < 2) return 0;

		*(utf8_buf+1) = (unic & 0x3F) | 0x80;        //10xx xxxx 
		*utf8_buf     = ((unic >> 6) & 0x1F) | 0xC0; //110x xxxx 

		return 2;  
	}  
	else if (unic >= 0x0800 && unic <= 0xFFFF) // * U-0800 - U-FFFF:  1110xxxx 10xxxxxx 10xxxxxx   
	{  
		if(!utf8_buf || utf8_buf_size < 3) return 0;
		*(utf8_buf+2) = (unic & 0x3F) | 0x80;         //10xx xxxx
		*(utf8_buf+1) = ((unic >>  6) & 0x3F) | 0x80; //10xx xxxx  
		*utf8_buf     = ((unic >> 12) & 0x0F) | 0xE0; //1110 xxxx  

		return 3;  
	}  

	return 0;  
}  

inline int get_utf8_bytes(unsigned char utfc)
{
	if(utfc <= 0x7F)      return 1; //0xxx xxxx
	else if(utfc <= 0xDF) return 2; //110x xxxx
	else if(utfc <= 0xEF) return 3; //1110 xxxx
	else if(utfc <= 0xF7) return 4; //1111 0xxx
	else if(utfc <= 0xFB) return 5; //1111 10xx
	else if(utfc <= 0xFD) return 6; //1111 110x
	return 0;
}

inline int utf8_to_unicode_one(unsigned char* utf8,int utf8_size, wchar_t *unic)  
{  
	*unic = 0;      
	switch (get_utf8_bytes(*utf8))  
	{  
	case 0:  //0xxx xxxx
	case 1:  //0xxx xxxx
		{
			if(!utf8 || utf8_size < 1) return 0;

			*unic = *utf8;  
			return 1; 
		}
		break;  

	case 2: //110x xxxx 10xx xxxx  
		{
			if(!utf8 || utf8_size < 2) return 0;

			unsigned char &b1 = *utf8;  
			unsigned char &b2 = *(utf8 + 1);  
			if ( (b2 & 0xC0) != 0x80 ) return 0;  

			*unic = ((b1 & 0x1F) << 6) + (b2 & 0x3F); //110x xxxx 10xx xxxx
			return 2; 
		}
		break;  
	case 3:  //1110 xxxx 10xx xxxx 10xx xxxx 
		{
			if(!utf8 || utf8_size < 3) return 0;

			unsigned char &b1 = *utf8;  
			unsigned char &b2 = *(utf8 + 1);  
			unsigned char &b3 = *(utf8 + 2);  
			if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )  return 0;  

			*unic = ((b1 & 0x0F) << 12) + ((b2 & 0x3F) << 6) + (b3 & 0x3F); 
			return 3;
		}
		break;

	case 4:  //1111 0xxx  10xx xxxx  10xx xxxx  10xx xxxx 
	case 5:  //1111 10xx  10xx xxxx  10xx xxxx  10xx xxxx 10xx xxxx 
	case 6:  //1111 110x  10xx xxxx  10xx xxxx  10xx xxxx 10xx xxxx 10xx xxxx 
		break;  
	}  

	return 0;  
} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



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
int _unicode_to_gbk(const wchar_t* unic_str,int str_len,char* gbk_buf,int gbk_buf_len)
{
	if(!unic_str || str_len <= 0 || !gbk_buf || gbk_buf_len <= 0) return 0;
	int  len   = 0,l;
	int  index = 0;
	while(index < str_len && gbk_buf_len - len > 0)
	{
		l = one_unicode_to_gbk(unic_str[index++],gbk_buf + len,gbk_buf_len - len); 
		if(l <= 0) return len;
		len += l;
	}

	if(len < gbk_buf_len) gbk_buf[len] = '\0';

	return len;
}


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
int  _gbk_to_unicode(const char* gbk_str,int str_len,wchar_t* unic_buf,int unic_buf_len)
{
	if(!gbk_str || !unic_buf || unic_buf_len <= 0) return 0;
	if(str_len <= 0) str_len = strlen(gbk_str);
	if(str_len <= 0) return 0;

	int  len   = 0,l;
	int  index = 0;
	while(index < unic_buf_len && str_len - len >0)
	{
		l = gbk_to_one_unicode((unsigned char*)gbk_str + len,str_len - len,&unic_buf[index]); 
		if(l <= 0) return index;
		len += l;
		index++;
	}
    unic_buf[index] = 0;
	return index;
}

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
int  _unicode_to_utf8(const wchar_t* unic_str,int str_len,char* utf8_buf,int utf8_buf_size)
{
	if(!unic_str || str_len <= 0 || !utf8_buf || utf8_buf_size <= 0) return 0;
	int  len   = 0,l;
	int  index = 0;
	while(index < str_len && utf8_buf_size - len > 0)
	{
		l = one_unicode_to_utf8(unic_str[index++],(unsigned char*)utf8_buf + len,utf8_buf_size - len); 
		if(l <= 0) return len;
		len += l;
	}

	if(len < utf8_buf_size) utf8_buf[len] = '\0';

	return len;
}

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
int  _utf8_to_unicode(const char* utf8_str,int utf8_size,wchar_t* unic_buf,int unic_buf_len)
{
	if(!utf8_str || utf8_size <= 0 || !unic_buf || unic_buf_len <= 0) return 0;
	int  len   = 0,l;
	int  index = 0;
	while(index < unic_buf_len && utf8_size - len > 0)
	{
		l = utf8_to_unicode_one((unsigned char*)utf8_str + len,utf8_size - len,&unic_buf[index]); 
		if(l <= 0) return index;
		len += l;
		index++;
	}
	unic_buf[index] = 0;
	return index;
}

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
int  _utf8_to_gbk(const char* utf8_str,int utf8_size,char* gbk_buf,int gbk_buf_size)
{
	if(!utf8_str || utf8_size <= 0 || !gbk_buf || gbk_buf_size <= 0) return 0;
	int  l = 0;
	int  utf8_off = 0;
	int  gbk_off  = 0;

	while(utf8_size - utf8_off > 0 && gbk_buf_size - gbk_off > 0)
	{
		wchar_t unic;
		l = utf8_to_unicode_one((unsigned char*)utf8_str + utf8_off,utf8_size - utf8_off,&unic); 
		if(l <= 0) break;
		utf8_off += l;

		l = one_unicode_to_gbk(unic,gbk_buf + gbk_off,gbk_buf_size - gbk_off); 
		if(l <= 0) break;
		gbk_off += l;
	}

	if(gbk_off < gbk_buf_size) gbk_buf[gbk_off] = '\0';

	return gbk_off;
}

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
int  _gbk_to_utf8(const char* gbk_str,int gbk_size,char* utf8_buf,int utf8_buf_size)
{
	if(!gbk_str || !utf8_buf || utf8_buf_size <= 0) return 0;
	if(gbk_size <= 0) gbk_size = strlen(gbk_str);
	if(gbk_size <= 0) return 0;

	int  l = 0;
	int  utf8_off = 0;
	int  gbk_off  = 0;

	while(gbk_size - gbk_off > 0 && utf8_buf_size - utf8_off > 0)
	{
		wchar_t unic;
		l = gbk_to_one_unicode((unsigned char*)gbk_str + gbk_off,gbk_size - gbk_off,&unic); 
		if(l <= 0) break;
		gbk_off += l;

		l = one_unicode_to_utf8(unic,(unsigned char*)utf8_buf + utf8_off,utf8_buf_size - utf8_off); 
		if(l <= 0) break;
		utf8_off += l;
	}

	if(utf8_off < utf8_buf_size) utf8_buf[utf8_off] = '\0';

	return utf8_off;
}