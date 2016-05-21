#include "server_frame.h"
#include "cflog.h"
#include <conio.h>
#include "config_manager.h"
#include "string_res.h"
#include "socket_service.h"
#include "game_main_thread.h"
#include "db_con_manager.h"
#include "db_thread.h"
#include "user_manager.h"
#include "game_room.h"
#include "mission.h"
#include "redis_thread.h"
#include "redis/myredis_pool.h"
#include "ms_connector.h"
#include "card_mission.h"
#include "http_thread.h"

#pragma warning(disable:4996)

server_frame::server_frame(void)
{
}

server_frame::~server_frame(void)
{
}

void set_title()
{
#ifdef _WIN32
    char title[1024] = {0};
    char path[256] = {0};
    GetCurrentDirectory(256, path);
    sprintf(title, "ddz:%d:[%s]", 
        config_manager::instance()->get_port(), path);

    SetConsoleTitle(title);
#endif
}

void server_frame::run()
{
    if(!init())
    {
        fini();
        LOG_ERROR("init fail.");
#ifdef _WIN32
        _getche();
#else
        int c; cin >> c;
#endif
        return;
    }
    LOG_INFO("main init success");
    if(!start())
    {
        LOG_ERROR("start fail.\n");
#ifdef _WIN32
        _getche();
#else
        int c; cin >> c;
#endif
        return;
    }
    LOG_INFO("main start success");

    wait_running();
    stop();
    fini();
}

bool server_frame::init()
{
    if(!cm->init())
    {
        LOG_ERROR("牌型任务初始化失败");
        return false;
    }
    if(!db_con_manager::instance()->init_by_file(g_db_type, DB_TYPE_COUNTER, 1, "db.xml"))
    {
        LOG_ERROR("数据块初始化失败");
        return false;
    }
    if(!config_manager::instance()->load_db_config())
    {
        LOG_ERROR("加载DB中server配置失败");
        return false;
    }
    if(!myredis_pool::instance()->init(1, 0))
    {
        LOG_ERROR("redis pool 初始化失败");
        return false;
    }

    if(!string_res_singleton::instance()->init("string.txt", STRING_INDEX_VECTOR))
    {
        LOG_ERROR("字符串资源加载失败");
        return false;
    }
    if(!socket_service::instance()->init())
    {
        LOG_ERROR("SOCKET 模块初始化失败");
        return false;
    }
    if(!game_main_thread::instance()->init(1, 1024*1024*4, 1024*1024*1024, 1024*1024*10, 1024*10))
    {
        LOG_ERROR("主线程初始化失败");
        return false;
    }
    if(!db_thread::instance()->init(1, 1024*1024, 1024*1024, 1024*1024*5, 4096))
    {
        LOG_ERROR("数据线程初始化失败");
        return false;
    }
    if(!redis_thread::instance()->init())
    {
        LOG_ERROR("redis 链接失败");
        return false;
    }
    if(!http->init(1, 1024*1024, 1024*1024, 1024*1024*5, 4096))
    {
        LOG_ERROR("HTTP线程初始化失败");
        return false;
    }
	game_main_thread::instance()->init_size();
    timer_singleton::instance()->init(game_main_thread::instance());
    int maxdesk = (config_manager::instance()->get_max_con()+50) / 3;
    desk_manager::instance()->init(maxdesk);

	if(!IS_MATCH_MODE)
	{
		single_game_room::instance()->init(maxdesk);
		single_game_room::instance()->init_users();
	}
    if(GAME_MODE_CFG == GAME_TYPE_MATCH)
        ms_inst->set_addr(config_manager::instance()->msip(), config_manager::instance()->msport());

    return true;
}

bool server_frame::fini()
{
    socket_service::instance()->fini();
    game_main_thread::instance()->fini();
    db_con_manager::instance()->fini();
    db_thread::instance()->fini();
    single_game_room::instance()->fini();
    desk_manager::instance()->fini();
    http->fini();

    return true;
}

bool server_frame::start()
{
    timer_singleton::instance()->start();
    if(!db_thread::instance()->start())
    {
        LOG_ERROR("数据线程启动失败");
        return false;
    }
    if(!game_main_thread::instance()->start())
    {
        LOG_ERROR("主线程启动失败");
        return false;
    }
    if(!socket_service::instance()->start())
    {
        LOG_ERROR("socket 模块启动失败");
        return false;
    }
    if(!redis_thread::instance()->start())
    {
        LOG_ERROR("redis 线程启动失败");
        return false;
    }
    if(!http->start())
    {
        LOG_ERROR("http 线程启动失败");
        return false;
    }
    single_game_room::instance()->start();
    game_main_thread::instance()->start_update_timer();
    if(GAME_MODE_CFG == GAME_TYPE_MATCH)
    {
        ms_inst->connect();
    }
    else
    {
        db_thread::instance()->on_update_robot();
        db_thread::instance()->on_logout(0);
    }
    return true;
}

bool server_frame::stop()
{
    timer_singleton::instance()->stop();
    socket_service::instance()->stop();
    game_main_thread::instance()->stop();
    db_thread::instance()->stop();
    http->stop();
    return true;
}

void server_frame::prompt()
{
    printf("please input command:\n[c]:clear screen\n[m]:set log\n[d]:dump info\n[q]:exit\n");
}

void server_frame::wait_running()
{
#ifdef _WIN32
    Sleep(100);
#else
    usleep(10000);
#endif
    char cmd;
    bool brun = true;
    while(brun)
    {
        prompt();
#ifdef _WIN32
        Sleep(100);
        cmd = tolower(_getche());
#else
        usleep(100000);
        cin >> cmd;
        cmd = tolower(cmd);
#endif
        cout << endl;
        switch(cmd)
        {
        case 'c':
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            break;
        case 'q':
            if(CAN_EXIT())
                brun = false;
            break;
        case 'm':
            MODIFY_LOG_CONSOLE();
            break;
            break;
        case 'd':
            {
#ifdef _WIN32
                Sleep(200);
#else
                usleep(200000);
#endif
                dump();
            }
            break;
        default:
            break;
        }
    }
}

void server_frame::dump()
{
    bool b = LOG_IS_CONSULE();
    LOG_ENABLE_CONSOLE(false);
    robot_inst->dump();
    LOG_ENABLE_CONSOLE(b);
}