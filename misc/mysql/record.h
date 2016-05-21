#ifndef __XMYSQL_RECORD_H__
#define __XMYSQL_RECORD_H__
#ifdef _WIN32
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif


class connection;

///////////////////////////////////////////////////////////////////////////////
// record class
class record
{
// construction & destruction
public:
	record();
	~record();
// attributes
private:
	connection*				m_pcon;

	MYSQL_ROW_OFFSET		m_first_row;
	MYSQL_ROW				m_row;
	MYSQL_FIELD*			m_field;
	MYSQL_RES*				m_presult;
	MYSQL_STMT*				m_mystmt;
	unsigned long			m_nfields;
	unsigned long			m_nrows;
	bool					m_beof;
    int                     m_lasterror;

protected:
public:
// operations
protected:
public:
	bool					init(connection* p_con);
	bool					open(const char* sql);
	bool					close();
	bool					is_eof();
	bool					move_next();
	bool					goto_next();
	bool					move_first();
	int						get_record_count();
	int						get_field_count();
    int                     last_error();
//---------------------------------------------------------------------------
	bool					get_field_value(const char* fieldname, bool& val);
	bool					get_field_value(const char* fieldname, char& val);
	bool					get_field_value(const char* fieldname, short& val);
    bool					get_field_value(const char* fieldname, int& val);
    bool					get_field_value(const char* fieldname, unsigned int& val);
	bool					get_field_value(const char* fieldname, float& val);
	bool					get_field_value(const char* fieldname, double& val);
	bool					get_field_value(const char* fieldname, long& val);
	bool					get_field_value(const char* fieldname, long long& val);
	bool					get_field_value(const char* fieldname, char* val, int len);
    bool					get_field_value(const char* fieldname, char** val, int& len);
//---------------------------------------------------------------------------
	bool					get_field_value(int col, bool& val);
	bool					get_field_value(int col, char& val);
	bool					get_field_value(int col, short& val);
    bool					get_field_value(int col, int& val);
    bool					get_field_value(int col, unsigned int& val);
	bool					get_field_value(int col, float& val);
	bool					get_field_value(int col, double& val);
	bool					get_field_value(int col, long& val);
	bool					get_field_value(int col, long long& val);
	bool					get_field_value(int col, char* val, int len);
	bool					get_field_value(int col, char** val, int& len);
};
///////////////////////////////////////////////////////////////////////////////
// binary_record class
class binary_record
{
// construction & destruction
public:
	binary_record();
	~binary_record();
// attributes
private:
	connection*				m_pcon;
protected:
public:
// operations
	bool					init(connection* p_con);
	bool					test_write();
	bool					test_read();

	
};

#endif//__XMYSQL_RECORD_H__



