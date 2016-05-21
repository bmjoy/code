#ifndef __msg_object_base_H__
#define __msg_object_base_H__
#include "byte_stream.h"

struct msg_object_head
{
    int 					len;
    short					m_major_id;
    short					m_minor_id;

    static int check_pack(void* p_data, int len, int max_len)
    {
        msg_object_head* head = (msg_object_head*) p_data;
        if(!head || len<=0)
            return -1;
        if(len < sizeof(msg_object_head))
            return 0;
        if(head->len < sizeof(msg_object_head) || head->len > max_len)
            return -1;
        if(head->len > len)
            return 0;
        return head->len;
    }

};

const int MSG_OBJ_HEAD_SIZE = sizeof(msg_object_head);

///////////////////////////////////////////////////////////////////////////////
// class msg_object_base
class msg_object_base
{
// construction & destruction
protected:
public:
	msg_object_base();
	~msg_object_base();
// attributes
protected:
	short					m_major_id;
	short					m_minor_id;
// operations
protected:
public:
	bool					serialize_in(char* p_data, int data_len);
	bool					serialize_out(char* p_buf, int& buf_len);

	short					major_id();
	short					minor_id();
private:
	virtual bool			serialize_in_item(byte_stream& bs) = 0;
	virtual bool			serialize_out_item(byte_stream& bs) = 0;
};
///////////////////////////////////////////////////////////////////////////////
// msg_object_base inline functions
inline msg_object_base::msg_object_base()
{
	m_major_id = -1;
	m_major_id = -1;
}
//---------------------------------------------------------------------------
inline msg_object_base::~msg_object_base()
{
}
//---------------------------------------------------------------------------
inline bool msg_object_base::serialize_in(char* p_data, int data_len)
{
	byte_stream				bs(p_data, data_len);
	int 					pack_len = 0;

	if(!bs.read(pack_len)) return false;
	if(pack_len != data_len) return false;
	if(!bs.read(m_major_id)) return false;
	if(!bs.read(m_minor_id)) return false;
	
	return serialize_in_item(bs);
}
//---------------------------------------------------------------------------
inline bool msg_object_base::serialize_out(char* p_buf, int& buf_len)
{
	int 					data_len = 0;
	byte_stream				bs(p_buf, buf_len);

	if(!bs.write(data_len)) return false;
	if(!bs.write(m_major_id)) return false;
	if(!bs.write(m_minor_id)) return false;
	if(!serialize_out_item(bs)) return false;
	data_len = bs.get_position();
	bs.set_position(0);
	bs.write(data_len);
	bs.set_position(data_len);
	buf_len = data_len;

	return true;
}

//---------------------------------------------------------------------------
inline short msg_object_base::major_id()
{
	return m_major_id;
}

//---------------------------------------------------------------------------
inline short msg_object_base::minor_id()
{
	return m_minor_id;
}

//---------------------------------------------------------------------------
/*
该函数用于批量对象序列化到一个缓冲区一次性发送, 减少socket的I/O调用次数.
*/
inline bool packobj(char* buf, int buflen, int& datalen, int& last_len, msg_object_base* obj)
{
    if(datalen >= buflen || (datalen+last_len)>buflen || obj == 0)
        return false;

    int tl = buflen - datalen;

    if(!obj->serialize_out(&buf[datalen], tl))
        return false;

    last_len = tl;
    datalen += tl;

    return true;
}

inline int check_pack_complish(void* p_data, int len)
{
    msg_object_head* head = (msg_object_head*)p_data;
    if(!head) return -1;
    if(len <= 0) return -1;

    if(len < MSG_OBJ_HEAD_SIZE)
        return 0;
    if(head->len < MSG_OBJ_HEAD_SIZE)
        return -1;
    if(head->len > 2000)
        return -1;
    if(len >= head->len)
        return head->len;
    return 0;
}
#define CAST_OBJ(msg_object_type) msg_object_type* o = (msg_object_type*)obj;

#endif//__msg_object_base_H__

