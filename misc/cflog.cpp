#include"cflog.h"
#include<time.h>
#include<cstdlib>
#include<string.h>
#ifdef _WIN32
#include <sys/types.h>
#include <sys/timeb.h>
#endif

cflog g_log_2012;

#ifdef  _WIN32
#include <conio.h>
#pragma warning(disable:4996)
#endif

struct LOG_HEAD
{
    char is_console;
    char is_file;
    int  level;
};


char LOG_TAG[4] = 
{
    'D',
    'I',
    'W',
    'E'
};

char COLOR_DEF[][32] =
{	
	"\033[40;34m",
	"\033[40;37m",
    "\033[40;35m",
	"\033[40;32m",
	"\33[0m"
};

void get_time_string(char* buf,bool blong = true)
{
    time_t now;
	struct tm* timenow;
#ifdef _WIN32
    struct _timeb timebuffer;
    _ftime( &timebuffer );
    now = timebuffer.time;
#else
    time(&now);
#endif

    timenow = localtime(&now);

    if(blong)
	{
#ifdef _WIN32
        sprintf(buf, "%02d.%02d.%02d.%02d.%02d.%d",
            timenow->tm_mon+1,
            timenow->tm_mday,
            timenow->tm_hour,
            timenow->tm_min,
            timenow->tm_sec,
            timebuffer.millitm);
#else
		sprintf(buf, "%02d-%02d-%02d-%02d-%02d",
			timenow->tm_mon+1,
			timenow->tm_mday,
			timenow->tm_hour,
			timenow->tm_min,
			timenow->tm_sec);
#endif
	}
	else
	{
		sprintf(buf, "%02d-%02d-%02d-%02d",
			timenow->tm_mon+1,
			timenow->tm_mday,
			timenow->tm_hour,
            timenow->tm_min);
	}


}

cflog::cflog(void)
{
    m_level = LLEVEL_DEBUG;
    m_is_console = true;
    m_is_file = true;
    m_is_asyn = true;
    f = 0;
    file_log_counter = 0;
    m_last_console_level = -1;
#ifdef _WIN32
    m_hconsole = 0;
#endif
}

cflog::~cflog(void)
{
}

void cflog::on_data(void* pdata, int len)
{
    LOG_HEAD* head = (LOG_HEAD*)pdata;
    char* s = &((char*)pdata)[sizeof(LOG_HEAD)];
    if(head->is_file)
        write_file_log(s, len-sizeof(LOG_HEAD));
    if(head->is_console)
        write_console_log(s, len-sizeof(LOG_HEAD), head->level);
}

void cflog::init(const char* pathname, int level, bool is_asyn, bool is_console, bool is_file)
{
    strncpy(m_path,pathname,strlen(pathname));
    m_path[strlen(pathname)] = 0;

    modify_level(level);
    set_console_enable(is_console);
    set_file_enalble(is_file);
    m_is_asyn = is_asyn;

    if(m_is_asyn)
    {
        ring_buffer_thread::init(1, 1024*1024, 1024*512, 1024*1024*4, 1024);
        start();
    }
}

void cflog::fini()
{
    if(m_is_asyn)
    {
        ring_buffer_thread::stop();
        ring_buffer_thread::fini();
    }
    if(f)
    {
        fflush(f);
        fclose(f);
        f = 0;
    }
}

void cflog::modify_level(int l)
{
    m_level = l;
    if(m_level < LLEVEL_DEBUG)
        m_level = LLEVEL_DEBUG;
    if(m_level > LLEVEL_ERROR)
        m_level = LLEVEL_ERROR;
}

int cflog::get_level()
{
    return m_level;
}

void cflog::set_console_enable(bool b)
{
    m_is_console = b;
}

bool cflog::is_enable_console()
{
    return m_is_console;
}

void cflog::set_file_enalble(bool b)
{
    m_is_file = b;
}

void cflog::info(const char* log,...)
{
    if(!m_is_console && !m_is_file)
        return;
    if(m_level > LLEVEL_INFO)
        return;

    char buf[LOG_BUF_MAX_SIZE];

    va_list ap;
    va_start(ap, log);
    vsnprintf(buf,LOG_BUF_MAX_SIZE,log, ap);
    va_end(ap);
    out_log(buf, LLEVEL_INFO);
}

void cflog::debug(const char* log, ...)
{
    if(!m_is_console && !m_is_file)
        return;
    if(m_level > LLEVEL_DEBUG)
        return;
    char buf[LOG_BUF_MAX_SIZE];

    va_list ap;
    va_start(ap, log);
    vsnprintf(buf,LOG_BUF_MAX_SIZE,log, ap);
    va_end(ap);
    out_log(buf, LLEVEL_DEBUG);
}

void cflog::warning(const char* log, ...)
{
    if(!m_is_console && !m_is_file)
        return;
    if(m_level > LLEVEL_WARNING)
        return;
    char buf[LOG_BUF_MAX_SIZE];

    va_list ap;
    va_start(ap, log);
    vsnprintf(buf,LOG_BUF_MAX_SIZE,log, ap);
    va_end(ap);
    out_log(buf, LLEVEL_WARNING);
}

void cflog::error(const char* log, ...)
{
     if(!m_is_console && !m_is_file)
        return;
    if(m_level > LLEVEL_ERROR)
        return;
    char buf[LOG_BUF_MAX_SIZE];
    
    va_list ap;
    va_start(ap, log);
    vsnprintf(buf,LOG_BUF_MAX_SIZE,log, ap);
    va_end(ap);
    out_log(buf, LLEVEL_ERROR);
}

void cflog::out_log(char* log, int level)
{
    char sz_time[64];
    char buf[LOG_BUF_MAX_SIZE + sizeof(LOG_HEAD)];
    LOG_HEAD* head = (LOG_HEAD*)buf;
    get_time_string(sz_time);
 
#ifndef _WIN32
    snprintf(&buf[sizeof(LOG_HEAD)], LOG_BUF_MAX_SIZE, "%s[%c[:%s\n", sz_time, LOG_TAG[level], log);
#else
    sprintf_s(&buf[sizeof(LOG_HEAD)], LOG_BUF_MAX_SIZE, "%s[%c]:%s\n", sz_time, LOG_TAG[level], log);
#endif

    if(m_is_asyn)
    {
        head->is_console = m_is_console?1:0;
        head->is_file = m_is_file?1:0;
        head->level = level;
        int len = strlen(&buf[sizeof(LOG_HEAD)]) + sizeof(LOG_HEAD);
        put_data(buf, len);
    }
    else
    {
        if(m_is_file)
            write_file_log(&buf[sizeof(LOG_HEAD)], strlen(&buf[sizeof(LOG_HEAD)]));
		if(m_is_console)
			write_console_log(&buf[sizeof(LOG_HEAD)], strlen(&buf[sizeof(LOG_HEAD)]),level);
	}
}

void cflog::write_file_log(char* s, int len)
{
    if(!f)
    {
        create_log_file();
    }
    file_log_counter ++;
    if(file_log_counter > MAX_LOG_LINE)
    {
        create_log_file();
    }
    if(!f)
        return;
    fwrite(s, len, 1, f);
    fflush(f);
}

void cflog::write_console_log(char* s, int len, int l)
{	
   if(l != m_last_console_level)
    {
        set_console_color(l);
		m_last_console_level = l;
    }

#ifdef _WIN32
   printf("%*.*s",len,len,s);
#else
   printf("%s %*.*s",COLOR_DEF[l],len,len,s);
   printf("%s",COLOR_DEF[LLEVEL_COUNT]);
#endif
}

#ifdef _WIN32
unsigned int LEVEL_COLOR[LLEVEL_COUNT] = 
{
    FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    FOREGROUND_INTENSITY | FOREGROUND_RED
};
#endif

void cflog::set_console_color(int l)
{
    if(l<LLEVEL_DEBUG || l>LLEVEL_ERROR || m_last_console_level==l)
        return;

	m_last_console_level = l;
#ifdef _WIN32
    if(!m_hconsole)
        m_hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(m_hconsole, LEVEL_COLOR[m_last_console_level]);
#endif
}

void cflog::create_log_file()
{
#ifdef _WIN32
	::CreateDirectoryA(m_path, NULL);
#else	
	mkdir(m_path,S_IRWXU);
#endif
    char sz_time[64];
    char sz_file_name[256];
    if(f)
    {
        fflush(f);
        fclose(f);
        f = 0;
    }
    get_time_string(sz_time,true);
    sprintf(sz_file_name, "%s/%s.log", m_path, sz_time);
    f = fopen(sz_file_name, "a");
    file_log_counter = 0;
}

char LOG_LEVEL_DEF[][32] = 
{
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR"
};

void MODIFY_LOG_CONSOLE()
{
	bool b = LOG_IS_CONSULE();
	LOG_ENABLE_CONSOLE(false);
	while(true)
	{
        printf("choose:\n[1]:modify_log_level\n[2]:modify_log_swtich\n");
        char c;
#ifdef _WIN32
        c = _getche();
#else
        cin >> c;
#endif
		printf("\n");
        if(c<'1' || c>'2')
            continue;
        if(c == '1')
        {
            while(true)
            {
                printf("choose log_lever:\n[1]:DEBUG\n[2]:INFO\n[3]:WARNING\n[4]:ERROR\n");
                char c;
#ifdef _WIN32
                c = _getche();
#else
                cin >> c;
#endif
                printf("\n");
                if(c<'1' || c>'4')
                    continue;
                int cl = c - '1';
                LOG_LEVEL(cl);
                printf("cur_log_level: %s\n", LOG_LEVEL_DEF[cl]);
                break;
            };
        }
        else
        {
            while(true)
            {
                printf("choose_log_swtich:\n[1]:open_console_log\n[2]:close_console_log\n");
                char c;
#ifdef _WIN32
                c = _getche();
#else
                cin >> c;
#endif
                printf("\n");
                if(c<'1' || c>'2')
                    continue;
                b = (c-'1')==0?true:false;
                printf("console_log_set: %s\n",b?"open":"close");
                break;
            }
        }
        break;
	}
    LOG_ENABLE_CONSOLE(b);
}

bool CAN_EXIT()
{
    bool b = LOG_IS_CONSULE();
    LOG_ENABLE_CONSOLE(false);
    char cmd;
    bool ret;
    while(true)
    {
        cout << "是否确认退出？[Y:N]" << endl;

#ifdef _WIN32
        Sleep(1);
        cmd = tolower(_getche());
#else
        usleep(1000000);
        cin >> cmd;
        cmd = tolower(cmd);
#endif

        cout << endl;

        if(cmd == 'y')
        {
            ret = true;
            break;;
        }
        if(cmd == 'n')
        {
            ret = false;
            break;;
        }
    }
    LOG_ENABLE_CONSOLE(b);
    if(ret)
        cout << "正在退出..." << endl;
    return ret;
}