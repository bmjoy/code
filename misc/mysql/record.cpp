#ifdef _WIN32
#include <WinSock2.h>
#endif
#include "record.h"
#include "connection.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// record class
record::record()
{
	m_pcon = 0;
	m_field = 0;
	m_presult = 0;
	m_mystmt = 0;
	m_nfields = 0;
	m_nrows = 0;
	m_first_row = 0;
	m_beof = true;
    m_lasterror = 0;
}

record::~record()
{
	close();
}
///////////////////////////////////////////////////////////////////////////////
// record operations
bool record::init(connection* p_con)
{
	if(!p_con)
	{
		return false;
	}
	m_pcon = p_con;
    m_pcon->clear();
	return true;
}
//---------------------------------------------------------------------------
bool record::open(const char* sql)
{
	bool					ret = false;
	MYSQL*					pmysql = NULL;

	if(m_pcon)
	{
		close();
        pmysql = m_pcon->mysql();

		int flag = mysql_real_query(pmysql, sql, (unsigned int)strlen(sql));
        if(flag != 0)
        {
            int err = mysql_errno(pmysql);
            return false;
        }

        m_presult = mysql_store_result(pmysql);

		if(m_presult)
		{
			m_nrows = (unsigned long)mysql_num_rows(m_presult);
			m_nfields = (unsigned long)mysql_num_fields(m_presult);
			m_beof = (m_nrows == 0);
			m_first_row = mysql_row_tell(m_presult);
			m_row = mysql_fetch_row(m_presult);
			if(!m_row)
			{
				m_beof = true;
			}
			else
			{
				m_beof = false;
			}
			ret = true;
		}
        else
        {
            m_lasterror = mysql_errno(pmysql);
        }
	}

	return ret;
}
//---------------------------------------------------------------------------
int record::last_error()
{
    return m_lasterror;
}
//---------------------------------------------------------------------------
bool record::close()
{
	bool					ret = false;
	MYSQL*					pmysql = NULL;
    m_lasterror = 0;
	if(m_pcon)
	{
		pmysql = m_pcon->mysql();
		while(m_presult)
		{
			mysql_free_result(m_presult);
			m_presult = NULL;
			if(mysql_next_result(pmysql) == 0)
			{
				m_presult = mysql_store_result(pmysql);
			}
		}
		//if(m_presult != NULL)
		//{
		//	mysql_free_result(m_presult);
		//}
		m_presult = NULL;
		m_nfields = 0;
		m_nrows = 0;
		m_beof = true;
		ret = true;
	}

	return ret;
}
//---------------------------------------------------------------------------
bool record::is_eof()
{
	return m_beof;
}
//---------------------------------------------------------------------------
bool record::move_next()
{
	if(!m_presult)
	{
		return false;
	}
	m_row = mysql_fetch_row(m_presult);
	if(!m_row)
	{
		m_beof = true;
	}

	return !m_beof;
}
//---------------------------------------------------------------------------
bool record::goto_next()
{
	bool					ret = false;
	MYSQL*					pmysql = NULL;

	if(m_pcon)
	{
		pmysql = m_pcon->mysql();
		if(m_presult)
		{
			mysql_free_result(m_presult);
			if(mysql_next_result(pmysql) == 0)
			{
				m_presult = mysql_store_result(pmysql);
			}
			else
			{
				m_presult = NULL;
			}
		}

		if(m_presult)
		{
			m_nrows = (unsigned long)mysql_num_rows(m_presult);
			m_nfields = (unsigned long)mysql_num_fields(m_presult);
			m_beof = (m_nrows == 0);
			m_first_row = mysql_row_tell(m_presult);
			m_row = mysql_fetch_row(m_presult);

			m_beof = false;
	/*		if(!m_row)
			{
				m_beof = true;
			}
			else
			{
				m_beof = false;
			} */
			ret = true;
		}
        else
        {
            m_nrows = 0;
            m_nfields = 0;
            m_beof = true;
            m_first_row = 0;
            m_row = 0;
        }
	}

	return ret;
}
//---------------------------------------------------------------------------
bool record::move_first()
{
	if(!m_presult || !m_first_row)
		return false;
	mysql_row_seek(m_presult, m_first_row);
	m_row = mysql_fetch_row(m_presult);
	m_beof = false;
	return true;
}
//---------------------------------------------------------------------------
int record::get_record_count()
{
	return m_nrows;
}
//---------------------------------------------------------------------------
int record::get_field_count()
{
	return m_nfields;
}
//---------------------------------------------------------------------------
bool record::get_field_value(const char* fieldname, bool& val)
{
	if(!m_row)
		return false;
	for(unsigned long i = 0; i < m_nfields; i++)
	{
		m_field = mysql_fetch_field_direct(m_presult, i);
		if(strcmp(m_field->name, fieldname) == 0)
		{
            if(!m_row[i])
                return false;
			val = m_row[i][0] == 1;
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
bool record::get_field_value(const char* fieldname, char& val)
{
	if(!m_row)
		return false;
	for(unsigned long i = 0; i < m_nfields; i++)
	{
		m_field = mysql_fetch_field_direct(m_presult, i);
		if(strcmp(m_field->name, fieldname) == 0)
		{
            if(!m_row[i])
                return false;
			val = m_row[i][0];
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
bool record::get_field_value(const char* fieldname, short& val)
{
	if(!m_row)
		return false;
	for(unsigned long i = 0; i < m_nfields; i ++)
	{
		m_field = mysql_fetch_field_direct(m_presult, i);
		if(strcmp(m_field->name, fieldname) == 0)
		{
            if(!m_row[i])
                return false;
			val = atoi(m_row[i]);
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
bool record::get_field_value(const char* fieldname, int& val)
{
	if(!m_row)
		return false;
	for(unsigned long i = 0; i < m_nfields; i++)
	{
		m_field = mysql_fetch_field_direct(m_presult, i);
		if(strcmp(m_field->name, fieldname) == 0)
		{
            if(!m_row[i])
                return false;
			val = atoi(m_row[i]);
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
bool record::get_field_value(const char* fieldname, unsigned int& val)
{
    if(!m_row)
        return false;
    for(unsigned long i = 0; i < m_nfields; i++)
    {
        m_field = mysql_fetch_field_direct(m_presult, i);
        if(strcmp(m_field->name, fieldname) == 0)
        {
            if(!m_row[i])
                return false;
            val = (unsigned int)atol(m_row[i]);
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool record::get_field_value(const char* fieldname, float& val)
{
	if(!m_row)
		return false;
	for(unsigned long i = 0; i < m_nfields; i++)
	{
		m_field = mysql_fetch_field_direct(m_presult, i);
		if(strcmp(m_field->name, fieldname) == 0)
		{
            if(!m_row[i])
                return false;
			val = (float)atof(m_row[i]);
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
bool record::get_field_value(const char* fieldname, double& val)
{
	if(!m_row)
		return false;
	for(unsigned long i = 0; i < m_nfields; i++)
	{
		m_field = mysql_fetch_field_direct(m_presult, i);
		if(strcmp(m_field->name, fieldname) == 0)
		{
            if(!m_row[i])
                return false;
			val = (double)atof(m_row[i]);
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
bool record::get_field_value(const char* fieldname, long& val)
{
	if(!m_row)
		return false;
	for(unsigned long i = 0; i < m_nfields; i++)
	{
		m_field = mysql_fetch_field_direct(m_presult, i);
		if(strcmp(m_field->name, fieldname) == 0)
		{
            if(!m_row[i])
                return false;
			val = atol(m_row[i]);
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
bool record::get_field_value(const char* fieldname, long long & val)
{
	if(!m_row)
		return false;
	for(unsigned long i = 0; i < m_nfields; i++)
	{
		m_field = mysql_fetch_field_direct(m_presult, i);
		if(strcmp(m_field->name, fieldname) == 0)
		{
            if(!m_row[i])
                return false;
#ifdef _WIN32
			val = _atoi64(m_row[i]);
#else
			val = atoll(m_row[i]);
#endif
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
bool record::get_field_value(const char* fieldname, char* val, int len)
{
	if(!m_row)
		return false;
	for(unsigned long i = 0; i < m_nfields; i++)
	{
		m_field = mysql_fetch_field_direct(m_presult, i);
		if(strcmp(m_field->name, fieldname) == 0)
		{
            if(!m_row[i])
                return false;
			int l1=len-1;
			int l2=strlen(m_row[i]);
			int minlen = l1<l2?l1:l2;
			memcpy(val, m_row[i], minlen);
			val[minlen] = 0;
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
bool record::get_field_value(const char* fieldname, char** val, int& len)
{
	if(m_row)
		return false;
	for(unsigned long i = 0; i < m_nfields; i++)
	{
		m_field = mysql_fetch_field_direct(m_presult, i);
		if(strcmp(m_field->name, fieldname) == 0)
		{
            if(!m_row[i])
                return false;
			unsigned long* lengths = mysql_fetch_lengths(m_presult);
			if(!lengths)
			{
				return false;
			}
			len  = lengths[i];
			*val = m_row[i]; 
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
bool record::get_field_value(int col, bool& val)
{
    if(!m_row || (col>=(int)m_nfields) || !m_row[col])
        return false;
	val = (m_row[col][0]==1);
	return true;
}
//---------------------------------------------------------------------------
bool record::get_field_value(int col, char& val)
{
    if(!m_row || (col>=(int)m_nfields) || !m_row[col])
        return false;
	val = m_row[col][0];
	return true;
}
//---------------------------------------------------------------------------
bool record::get_field_value(int col, short& val)
{
    if(!m_row || (col>=(int)m_nfields) || !m_row[col])
        return false;
	val = atoi(m_row[col]);
	return true;
}
//---------------------------------------------------------------------------
bool record::get_field_value(int col, int& val)
{
    if(!m_row || (col>=(int)m_nfields) || !m_row[col])
        return false;
	val = atoi(m_row[col]);
	return true;
}
//---------------------------------------------------------------------------
bool record::get_field_value(int col, unsigned int& val)
{
    if(!m_row || (col>=(int)m_nfields) || !m_row[col])
        return false;
    val = (unsigned int)atol(m_row[col]);
    return true;
}
//---------------------------------------------------------------------------
bool record::get_field_value(int col, float& val)
{
    if(!m_row || (col>=(int)m_nfields) || !m_row[col])
        return false;
	val = (float)atof(m_row[col]);
	return true;
}
//---------------------------------------------------------------------------
bool record::get_field_value(int col, double& val)
{
    if(!m_row || (col>=(int)m_nfields) || !m_row[col])
        return false;
	val = atof(m_row[col]);
	return true;
}
//---------------------------------------------------------------------------
bool record::get_field_value(int col, long& val)
{
    if(!m_row || (col>=(int)m_nfields) || !m_row[col])
        return false;
	val = atol(m_row[col]);
	return true;
}
//---------------------------------------------------------------------------
bool record::get_field_value(int col, long long& val)
{
    if(!m_row || (col>=(int)m_nfields) || !m_row[col])
        return false;
#ifdef _WIN32
	val = _atoi64(m_row[col]);
#else
	val = atoll(m_row[col]);
#endif

	return true;
}
//---------------------------------------------------------------------------
bool record::get_field_value(int col, char* val, int len)
{
    if(!m_row || (col>=(int)m_nfields) || !m_row[col])
        return false;
	int l1=len-1;
	int l2=strlen(m_row[col]);
	int minlen = l1<l2?l1:l2;
	memcpy(val, m_row[col], minlen);
	val[minlen] = 0;

	return true;
}
//---------------------------------------------------------------------------
bool record::get_field_value(int col, char** val, int& len)
{
    if(!m_row || (col>=(int)m_nfields) || !m_row[col])
        return false;
	unsigned long* lengths = mysql_fetch_lengths(m_presult);
	if(!lengths)
		return false;
	len  = lengths[col];
	*val = m_row[col];
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// binary_record class
binary_record::binary_record()
{
	m_pcon = 0;
}

binary_record::~binary_record()
{

}
///////////////////////////////////////////////////////////////////////////////
// binary_record operations
bool binary_record::init(connection* p_con)
{
	m_pcon = p_con;
	return true;
}
//---------------------------------------------------------------------------
#ifdef _WIN32
#pragma warning(disable:4996)
#endif
bool binary_record::test_write()
{
	if(!m_pcon)
	{
		return false;
	}
	char* sql = new char[1024*10];
	memset(sql, 0, 1024*10);
	char* buf = new char[1024*2];
	char* end;
	for(int i=0; i<1024*2; i++)
	{
		buf[i] = i % 255;
	}
	sprintf(sql, "INSERT INTO GameRecord (records) VALUES ('");
	end = sql + strlen(sql);
	end += mysql_real_escape_string(m_pcon->mysql(), end, (const char*)buf, 1024+100);

	sprintf(end, "')");
	end +=2;

	int flag = mysql_real_query(m_pcon->mysql(), sql, (unsigned int)(end-sql));
	cout << "flag=" << flag << endl;
	cout << sql << endl;
	delete []buf;
	delete []sql;
	return (flag == 0);
}
//---------------------------------------------------------------------------
bool binary_record::test_read()
{
	char sql[1024] = {0};
	sprintf(sql, "select records from GameRecord");
//	mysql_real_query(_p_con->mysql(), sql, strlen(sql));
	mysql_query(m_pcon->mysql(), sql);
	MYSQL_RES* res = mysql_store_result(m_pcon->mysql());
	unsigned long n_rows = (unsigned long)mysql_num_rows(res);
	unsigned long n_fields = (unsigned long)mysql_num_fields(res);
	cout << "rows=" << n_rows << endl;
	cout << "fields=" << n_fields << endl;
	MYSQL_ROW row;
	row = mysql_fetch_row(res);
	while(row != NULL)
	{
		unsigned long* len = mysql_fetch_lengths(res);
		unsigned long l = (unsigned long)len[0];
		cout << "len=" << l << endl;
		cout << row[0] << endl;
		row = mysql_fetch_row(res);
	}
	return true;
}
#ifdef _WIN32
#pragma warning(default:4996)
#endif
