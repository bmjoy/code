
#ifndef __BYTE_STREAM_H__
#define __BYTE_STREAM_H__

#ifdef _WIN32
#pragma warning(disable:4018)
#include <Windows.h>
#else
#include <string.h>
#endif


///////////////////////////////////////////////////////////////////////////////
// class byte_stream
class byte_stream
{
// construction & destruction
protected:
public:
	byte_stream(char* p, int len);
	~byte_stream();
// attributes
private:
	char*					p_ptr;
	int						length;
	int						position;
protected:
public:
	template<class T>
	bool					write(T& val);

	template<class T>
	bool					read(T& val);

	bool					write_v(char* p, int len);
	bool					read_v(char* p, int len);

	bool					write_s1(char* str, int len);
	bool					read_s1(char* str, int& len, bool zero_end = false);

	bool					write_s2(char* str, int len);
	bool					read_s2(char* str, int& len, bool zero_end = false);

	int						get_position();
	void					set_position(int pos);

	template<class T>
	static void				htol(T v1, T& v2);
};
///////////////////////////////////////////////////////////////////////////////
// byte_stream inline functions
inline byte_stream::byte_stream(char* p, int len)
{
	p_ptr = p;
	length = len;
	position = 0;
}
//---------------------------------------------------------------------------
inline byte_stream::~byte_stream()
{
}
//---------------------------------------------------------------------------
template<class T>
inline bool byte_stream::write(T& val)
{
	if(length < (sizeof(T) + position))
	{
		return false;
	}

	memcpy(&p_ptr[position], &val, sizeof(T));
	position += sizeof(T);

	return true;
}
//---------------------------------------------------------------------------
template<class T>
inline bool byte_stream::read(T& val)
{
	if(length < (sizeof(T) + position))
	{
		return false;
	}

	memcpy(&val, &p_ptr[position], sizeof(T));
	position += sizeof(T);

	return true;
}
//---------------------------------------------------------------------------
inline bool byte_stream::write_v(char* p, int len)
{
	if(length < (position + len))
	{
		return false;
	}
	memcpy(&p_ptr[position], p, len);
	position += len;

	return true;
}
//---------------------------------------------------------------------------
inline bool byte_stream::read_v(char* p, int len)
{
	if(length < (position + len))
	{
		return false;
	}
	memcpy(p, &p_ptr[position], len);
	position += len;

	return true;
}
//---------------------------------------------------------------------------
inline bool byte_stream::write_s1(char* str, int len)
{
	if(length < (position + len + 2))
	{
		return false;
	}

	unsigned char sl = len;
	write(sl);
	memcpy(&p_ptr[position], str, len);
	position += len;

	return true;
}
//---------------------------------------------------------------------------
inline bool byte_stream::read_s1(char* str, int& len, bool zero_end)
{
	unsigned char			sl = 0;

	if(!read(sl))
		return false;
	if(sl > len)
		return false;
	len = sl;
	if(length < (position + len))
		return false;
	memcpy(str, &p_ptr[position], len);
    position += len;
	if(zero_end)
		str[len] = 0;

	return true;
}
//---------------------------------------------------------------------------
inline bool byte_stream::write_s2(char* str, int len)
{
	if(length < (position+len+2))
		return false;

	short sl = len;
	write(sl);
	memcpy(&p_ptr[position], str, len);
	position += len;

	return true;
}
//---------------------------------------------------------------------------
inline bool byte_stream::read_s2(char* str, int& len, bool zero_end)
{
	short					sl = 0;

	if(!read(sl))
		return false;
	if(sl > len)
		return false;
	len = sl;
	if(length < (position + len))
		return false;
	memcpy(str, &p_ptr[position], len);
	position += sl;
	if(zero_end)
		str[len] = 0;

	return true;
}
//---------------------------------------------------------------------------
inline int byte_stream::get_position()
{
	return position;
}
//---------------------------------------------------------------------------
inline void byte_stream::set_position(int pos)
{
	position = pos;
}
//---------------------------------------------------------------------------
template<class T>
inline void byte_stream::htol(T v1, T& v2)
{
	char* p1 = (char*)&v1;
	char* p2 = (char*)&v2;
	for(int i = 0; i < sizeof(T); i++)
	{
		p2[i] = p1[sizeof(T) - i - 1];
	}
}





#ifdef _WIN32
#pragma warning(default:4018)
#endif

#endif//__BYTE_STREAM_H__

