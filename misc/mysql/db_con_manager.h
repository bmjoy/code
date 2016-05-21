#ifndef __DB_CON_MANAGER_H__
#define __DB_CON_MANAGER_H__
#include "connection.h"
#include "singleton.h"
#include "mutex.h"
#include <vector>
#include <list>
#include "safe_queue.h"
using namespace std;

struct db_type_item
{
    char name[64];
};

typedef struct tag_connection_pool
{
    char ip[32];
    int  port;
    char user[64];
    char pwd[128];
    safe_queue<connection*> m_pool;
}connection_pool;

typedef struct tag_database_item
{
    char dbname[128];
    connection_pool* p_con_pool;
}database_item;


class db_con_manager : public singleton<db_con_manager>
{
public:
    db_con_manager(void);
    ~db_con_manager(void);

    bool init_by_file(db_type_item* types, int tsize, int max_threads, const char* filename);
    bool init_by_string(db_type_item* types, int tsize, int max_threads, const char* config);
    void fini();

    connection* get_connection(int type);
    void put_connection(int type, connection* p_con);

private:
    bool load_db_config(const char* config);
    int query_db_type_id(const char* type);
    const char* query_db_type_name(int id);
    bool check_db_connection();
    connection_pool* get_connection_pool(const char* ip);
    void add_db_config(const char* ip, int port, const char* user, const char* pwd);
private:
    thread_mutex m_mutes;
    int                          m_max_connection;
    vector<database_item>        m_db_item;
    list<connection_pool*>       m_pool_list;
    vector<db_type_item>         m_types;
	int                          m_is_init;
};

class mysql_con_helper
{
//    friend db_con_manager;
public:
    mysql_con_helper(int type);
    ~mysql_con_helper();
    connection* operator ->();
    connection* handle();
    bool is_empty();
	static void init(int max_conter);    
private:
    static int m_max_conter;
    connection* m_pcon;
    int m_type;
};

/*
数据库配置格式 DEMO
<root>
    <db type="goldendb" ip="192.168.1.118" port=3306 user="root" pwd="YXNkZmFzZGY=" dbname="test_glodendb1" />
    <db type="propdb" ip="192.168.1.118" port=3306 user="root" pwd="YXNkZmFzZGY=" dbname="test_propdb1" />
    <db type="gamedb" ip="192.168.1.118" port=3306 user="root" pwd="YXNkZmFzZGY=" dbname="test_gamedb1" />
    <db type="vmoneydb" ip="192.168.1.118" port=3306 user="root" pwd="YXNkZmFzZGY=" dbname="test_vmoney1" />
    <db type="matchdb" ip="192.168.1.118" port=3306 user="root" pwd="YXNkZmFzZGY=" dbname="test_matchdb1" />
</root>
pwd由那个啥工具生成, 名字忘记了, 回头补上.
*/
#endif//__DB_CON_MANAGER_H__