#ifndef __cflog_h__
#define __cflog_h__

#include <stdio.h>
#include <sys/types.h>
#include <stdarg.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

#include"ring_buffer_thread.h"

enum LOG_LEVEL
{
	LLEVEL_DEBUG,
	LLEVEL_INFO,
	LLEVEL_WARNING,
	LLEVEL_ERROR,
	LLEVEL_COUNT,
};

#define LOG_BUF_MAX_SIZE 10240
#define MAX_LOG_LINE     50000

#ifndef _WIN32
#ifndef MAX_PATH
#define MAX_PATH 256  
#endif
#endif

class cflog: public ring_buffer_thread
{
public:
	cflog(void);
    ~cflog(void);

public:
    void init(const char* pathname, int level = LLEVEL_WARNING, bool is_asyn = true, bool is_console=true, bool is_file=true);
    void fini();
    void modify_level(int l);
    int  get_level();
    void set_console_enable(bool b);
    void set_file_enalble(bool b);
    bool is_enable_console();

    void info(const char* log,...);
	void debug(const char* log,...);
	void warning(const char* log,...);
	void error(const char* log,...);

public:
	virtual void on_data(void* pdata,int len);

private:
    void write_file_log(char* s, int len);
    void write_console_log(char* s, int len, int l);
    void create_log_file();
    void out_log(char* log, int level);
    void set_console_color(int l);

private:
	char m_path[MAX_PATH];
	int  m_level;
	bool m_is_console;
	bool m_is_file;
	bool m_is_asyn;
	FILE* f;
	int  file_log_counter;
	int  m_last_console_level;
#ifdef _WIN32
    HANDLE m_hconsole;
#endif
};

extern  cflog g_log_2012;
#define LOG_INIT(path) g_log_2012.init(path)
#define LOG_FINI g_log_2012.fini
#define LOG_LEVEL(l) g_log_2012.modify_level(l)
#define LOG_GET_LEVEL() g_log_2012.get_level()
#define LOG_IS_CONSULE() g_log_2012.is_enable_console();
#define LOG_ENABLE_CONSOLE(b) g_log_2012.set_console_enable(b)
#define LOG_ENABLE_FILE(b) g_log_2012.set_file_enalble(b)
#define LOG_INFO g_log_2012.info
#define LOG_DEBUG g_log_2012.debug
#define LOG_WARNING g_log_2012.warning
#define LOG_ERROR g_log_2012.error
#define LOG_ERR_LINE() LOG_ERROR("%s %d", __FILE__, __LINE__)

extern  void MODIFY_LOG_CONSOLE();
extern  bool CAN_EXIT();

#endif


