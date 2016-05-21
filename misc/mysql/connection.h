#ifndef __XMYSQL_CONNECTION_H__
#define __XMYSQL_CONNECTION_H__
#ifdef _WIN32
#include <WinSock2.h>
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif
#include <string>
using namespace std;

class record;

///////////////////////////////////////////////////////////////////////////////
// connection class
class connection
{
// construction & destruction
public:
	connection();
	~connection();
// attributes
private:
	MYSQL					m_mysql;
	string					m_szhost;
	string					m_szusername;
	string					m_szpwd;
	string					m_szdbname;
	unsigned int			m_nport;
	bool					m_binit;
	bool					m_bconnected;
protected:
public:
// operations
	int						clear();
protected:
public:
	unsigned int			init(const char* charset="gbk");
	unsigned int			open(const char* host, const char* username, const char* pwd, const char* dbname, unsigned short port);
    void                    close();
	int						execute_no_record(const char* sql);
    int                     select_db(const char* db);
    MYSQL*					mysql();

};




#endif//__XMYSQL_CONNECTION_H__
