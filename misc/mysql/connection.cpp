#ifdef _WIN32
#include <WinSock2.h>
#endif
#include "connection.h"
#include "record.h"
#include "stdlib.h"
#include <iostream>
#include <string.h>
#include "cflog.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// connection class
connection::connection()
{
	m_binit = false;
	m_bconnected = false;
}

connection::~connection()
{
    if(m_binit)
        close();
}
///////////////////////////////////////////////////////////////////////////////
// connection operations
int connection::clear()
{
	int						ret = 0;
	bool					bnext = true;
	MYSQL_RES*				pres = NULL;

	while(bnext)
	{
		bnext = false;
		pres = mysql_store_result(&m_mysql);
		if(pres)
		{
			mysql_free_result(pres);
			if(mysql_next_result(&m_mysql) == 0)
			{
				bnext = true;
			}
		} // if(pres)
	} // while(bnext)

	return ret;
}
//---------------------------------------------------------------------------
unsigned int connection::init(const char* charset)
{
	if(mysql_init(&m_mysql))
	{
		mysql_options(&m_mysql, MYSQL_SET_CHARSET_NAME, charset);
		m_binit = true;
		return 0;
	}
	else
	{
		m_binit = false;
		return -1;
	}
}
//---------------------------------------------------------------------------
unsigned int connection::open(const char* host, const char* username, const char* pwd, const char* dbname, unsigned short port)
{
	unsigned int			err = -1;

	if(m_binit)
	{
		m_szhost = host;
		m_szusername = username;
		m_szpwd = pwd;
		m_szdbname = dbname;
		m_nport = port;
		m_szhost.c_str();
		m_mysql.reconnect = 1;
        if(mysql_real_connect(&m_mysql, m_szhost.c_str(), m_szusername.c_str(), m_szpwd.c_str(), m_szdbname.c_str(), m_nport, NULL, CLIENT_MULTI_STATEMENTS))
		{
			m_bconnected = true;
			err = 0;
		}
		else
		{
			m_bconnected = false;
			err = mysql_errno(&m_mysql);
		}
	}

	return err;
}

//---------------------------------------------------------------------------
void connection::close()
{
    if(!m_binit)
        return;
    mysql_close(&m_mysql);
    m_binit = false;
}
//---------------------------------------------------------------------------
int connection::execute_no_record(const char* sql)
{
	int						ret = -1;

	ret = mysql_real_query(&m_mysql, sql, strlen(sql));
	int err = mysql_errno(&m_mysql);
	if(ret == 0)
	{
		clear();
	}
	else
	{
        char szTmp[256] = {0};
        int len = strlen(sql);
        if(len>255)
            len = 255;

        memcpy(szTmp, sql, len);

        LOG_ERROR("execute_no_record : %d", err);
        LOG_ERROR("execute_no_record : %s", szTmp);
	}

	return ret;
}

//---------------------------------------------------------------------------
int connection::select_db(const char* db)
{
    if(!db)
        return -1;
    if(m_mysql.db && strcmp(m_mysql.db, db) == 0)
        return 0;
    int ret = mysql_select_db(&m_mysql, db);
    if(ret != 0)
    {
        int err = mysql_errno(&m_mysql);
        err = err;
    }
    return ret;
}
//---------------------------------------------------------------------------
MYSQL* connection::mysql()
{
	return &m_mysql;
}
