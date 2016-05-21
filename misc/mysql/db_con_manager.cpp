#include "db_con_manager.h"
#include "base64.h"
#include "tinyxml_tool.h"
#include "cflog.h"
#include "vector_helper.h"
#ifdef _WIN32
#pragma warning(disable:4996)
#endif
int mysql_con_helper::m_max_conter = 0;

mysql_con_helper::mysql_con_helper(int type)
{
    m_type = type;
    if(m_type>=0 && m_type<m_max_conter)
        m_pcon = db_con_manager::instance()->get_connection(m_type);
    else
        m_pcon = 0;
}

mysql_con_helper::~mysql_con_helper()
{
    db_con_manager::instance()->put_connection(m_type, m_pcon);
}

connection* mysql_con_helper::operator ->()
{
    return m_pcon;
}

connection* mysql_con_helper::handle()
{
    return m_pcon;
}

bool mysql_con_helper::is_empty()
{
    return (!m_pcon);
}

void mysql_con_helper::init(int max_db_conter)
{
    m_max_conter = max_db_conter;
}

//////////////////////////////////////////////////////////////////////////
db_con_manager::db_con_manager(void)
{
	m_is_init = 0;
}

db_con_manager::~db_con_manager(void)
{
    mysql_library_end();
}

bool db_con_manager::init_by_file(db_type_item* types, int tsize, int max_threads, const char* filename)
{
    char temp[10240] = {0};
    if(!filename)
        return false;
    FILE* file = fopen(filename, "rb");
    if(!file)
        return false;
    fseek(file, 0, SEEK_END);
    int size = (int)ftell(file);
    fseek(file, 0, SEEK_SET);

    fread(temp, size, 1, file);
    return init_by_string(types, tsize, max_threads, temp);
}

bool db_con_manager::init_by_string(db_type_item* types, int tsize, int max_threads, const char* config)
{
    //guard g(m_mutes);
	if(m_is_init)
	{
		return false;
	}
    mysql_con_helper::init(tsize);
    m_max_connection = max_threads;

    database_item dbitem;
    dbitem.dbname[0] = 0;
    dbitem.p_con_pool = 0;
    m_db_item.resize(tsize, dbitem);

    db_type_item dti;
    dti.name[0]= 0;
    m_types.resize(tsize, dti);

    for (int i=0; i<tsize; i++)
    {
        strcpy(m_types[i].name, types[i].name);
    }
    if(!load_db_config(config))
        return false;
    if(!check_db_connection())
    {
        return false;
    }
	m_is_init = 1;
    return true;
}

void db_con_manager::fini()
{
    guard g(m_mutes);
	if(!m_is_init)
	{
		return;
	}
    connection* pcon = 0;
    list<connection_pool*>::iterator i;
    for(i=m_pool_list.begin(); i!=m_pool_list.end(); i++)
    {
        connection* p = 0;
        (*i)->m_pool.pop(p);
        while (p) 
        {
            delete p;
            p = 0;
            (*i)->m_pool.pop(p);
        }
        delete (*i);
    }
    m_pool_list.clear();
    release_vector(m_db_item);
    release_vector(m_types);
	m_is_init = 0;
}

connection* db_con_manager::get_connection(int type)
{
    guard g(m_mutes);
    connection* pcon = 0;
    if(type < 0 || type >= (int)m_db_item.size())
        return 0;
    connection_pool* p_con_pool = m_db_item[type].p_con_pool;
    if(!p_con_pool)
        return 0;
    
    p_con_pool->m_pool.pop(pcon);
    if(!pcon)
    {
        pcon = new connection;
        pcon->init();
        if(0 != pcon->open(p_con_pool->ip, p_con_pool->user, p_con_pool->pwd, "", p_con_pool->port))
        {
            delete pcon;
            pcon = 0;
        }
    }
    if(pcon)
	{
        if(0 != pcon->select_db(m_db_item[type].dbname))
		{
			put_connection(type, pcon);
			return 0;
		}
	}
    return pcon;
}

void db_con_manager::put_connection(int type, connection* p_con)
{
    guard g(m_mutes);
    if(!p_con)
        return;
    if(type >= (int)m_types.size() || type < 0)
    {
        delete p_con;
        return;
    }
    if(!m_db_item[type].p_con_pool || !m_db_item[type].p_con_pool->m_pool.put(p_con))
    {
        delete p_con;
    }
}

bool db_con_manager::load_db_config(const char* config)
{
    TiXmlDocument xmlDoc;
    TiXmlElement* pElmRoot = 0;
    TiXmlElement* pElmItem = 0;
    xmlDoc.Parse(config, 0, TIXML_ENCODING_LEGACY);
    if(xmlDoc.Error())
    {
        LOG_ERROR("load db config file failed.");
        return false;
    }
    pElmRoot = xmlDoc.FirstChildElement("root");
    if(!pElmRoot)
    {
        LOG_ERROR("load db config file failed.");
        return false;
    }
    pElmItem = pElmRoot->FirstChildElement("db");
    while(pElmItem)
    {
        char type[128] = {0};
        char pwd64[256] = {0};
        char ip[32] = {0};
        int  port;
        char user[64] = {0};
        char pwd[128] = {0};

        if(!tinyxml_tool::get_str_attribute(pElmItem, "type", type, sizeof(type))) return false;
        int id = query_db_type_id(type);
        if(id<0 || id>=(int)m_types.size())
        {
            LOG_WARNING("冗余数据库类型: %s", type);
            pElmItem = pElmItem->NextSiblingElement("db");
            continue;
        }

        if(!tinyxml_tool::get_str_attribute(pElmItem, "ip", ip, sizeof(ip))) return false;
        if(!tinyxml_tool::get_int_attribute(pElmItem, "port", port)) return false;
        if(!tinyxml_tool::get_str_attribute(pElmItem, "user", user, sizeof(user))) return false;
        if(!tinyxml_tool::get_str_attribute(pElmItem, "pwd", pwd64, sizeof(pwd64))) return false;
        if(!tinyxml_tool::get_str_attribute(pElmItem, "dbname", m_db_item[id].dbname, sizeof(m_db_item[id].dbname))) return false;

        int len = sizeof(pwd);
        base64_decode((const unsigned char*)pwd64, strlen(pwd64), (unsigned char *)pwd, &len);
        add_db_config(ip, port, user, pwd);
        m_db_item[id].p_con_pool = get_connection_pool(ip);
        
        pElmItem = pElmItem->NextSiblingElement("db");
    }

    for(int i=0; i<(int)m_types.size(); i++)
    {
        if(strlen(m_db_item[i].dbname) == 0 || !m_db_item[i].p_con_pool)
        {
            LOG_ERROR("Missing db config: %s", query_db_type_name(i));
            return false;
        }
    }
    LOG_INFO("load db success");

    return true;
}

int db_con_manager::query_db_type_id(const char* type)
{
    for (int i=0; i<(int)m_types.size(); i++)
    {
        if(strcmp(type, m_types[i].name) == 0)
            return i;
    }
    return -1;
}

const char* db_con_manager::query_db_type_name(int id)
{
    if(id>=0 && id<(int)m_types.size())
        return m_types[id].name;
    return 0;
}

connection_pool* db_con_manager::get_connection_pool(const char* ip)
{
    list<connection_pool*>::iterator i;
    for(i=m_pool_list.begin(); i!=m_pool_list.end(); i++)
    {
        if(strcmp((*i)->ip, ip) == 0)
            return (*i);
    }
    return 0;
}

void db_con_manager::add_db_config(const char* ip, int port, const char* user, const char* pwd)
{
    list<connection_pool*>::iterator i;
    for(i=m_pool_list.begin(); i!=m_pool_list.end(); i++)
    {
        if(strcmp((*i)->ip, ip) == 0)
            return;
    }
    connection_pool* pool = new connection_pool;
    strcpy(pool->ip, ip);
    strcpy(pool->user, user);
    strcpy(pool->pwd, pwd);
    pool->port = port;
    pool->m_pool.init(m_max_connection);
    m_pool_list.push_back(pool);
}

bool db_con_manager::check_db_connection()
{
    LOG_INFO("检测数据库连接");
    for(int i=0; i<(int)m_db_item.size(); i++)
    {
        connection* pcon = get_connection(i);
        if(!pcon)
        {
            LOG_ERROR("db check failed: %s", query_db_type_name(i));
            return false;
        }
        put_connection(i, pcon);
    }
    LOG_INFO("数据库连接检测完毕");
    return true;
}
