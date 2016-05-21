#include "win_dump.h"
#include <Windows.h>
#include <dbghelp.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment(lib, "Dbghelp.lib")
#pragma warning(disable:4996)


char g_dump_tag[64] = {0};

LONG WINAPI ___UnhandledFilter(struct _EXCEPTION_POINTERS *lpExceptionInfo)
{
    LONG ret = EXCEPTION_EXECUTE_HANDLER;
    TCHAR szFileName[64];
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    wsprintf(szFileName, TEXT("%s-%d-%d-%d-%d-%d-%d-%d-%03d.dmp"), g_dump_tag, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, rand()%100);

    HANDLE hFile = ::CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    if (hFile != INVALID_HANDLE_VALUE)
    {
        MINIDUMP_EXCEPTION_INFORMATION ExInfo;
        ExInfo.ThreadId = ::GetCurrentThreadId();
        ExInfo.ExceptionPointers = lpExceptionInfo;
        ExInfo.ClientPointers = false;
        // write the dump
        BOOL bOK = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );
        if (bOK)
        {
            printf("Create Dump File Success!\n");
        }
        else
        {
            printf("MiniDumpWriteDump Failed: %d\n", GetLastError());
        }
        ::CloseHandle(hFile);
    }
    else
    {
        printf("Create File %s Failed %d\n", szFileName, GetLastError());
    }
    return ret;
}

void reg_dump_debug(const char* tag)
{
    strcpy(g_dump_tag, tag);
    ::SetUnhandledExceptionFilter(___UnhandledFilter);
}