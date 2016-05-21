// xddzsvr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cflog.h"
#include <string.h>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "util.h"
#include "config_manager.h"
#include "win_dump.h"
#include "mem_leck_detector.h"
#include "server_frame.h"
#include "test_mddz.h"
#include "myrandom.h"
using namespace std;

void run();
mem_leck_detector mem_leck(0);

int _tmain(int argc, _TCHAR* argv[])
{
    random_seed(GetTickCount());
#if 0
    test_mddz();
    return 0;
#endif
    LOG_INIT("log");
    LOG_LEVEL(LLEVEL_DEBUG);    
    if(!config_manager::instance()->init())
    {
        LOG_ERROR("³õÊ¼»¯Ê§°Ü");
        _getche();
        return 0;
    }
    
#ifdef _DEBUG
    LOG_LEVEL(LLEVEL_DEBUG);
#else
    LOG_LEVEL(LLEVEL_WARNING);
#endif

    mem_leck_detector m;
    run();
    LOG_FINI();
    return 0;
}

void run()
{
    reg_dump_debug("xddzsvr");
    server_frame::instance()->run();
}