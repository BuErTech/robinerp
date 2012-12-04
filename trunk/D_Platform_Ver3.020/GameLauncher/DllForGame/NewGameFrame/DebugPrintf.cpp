//////////////////////////////////////////////////////////////////////////
/// ������Ϣ��������Ϣ�����������
/// ���У�DebugPrintf���ڵ��Ը���ʱ�����Ϣ������ʱ�����޸ĺ����壬��д�ļ�ֱ�ӷ���
/// ErrorPrintf���ڳ������쳣״�������������ʱҲһ������������ٴ���ʱ�ṩ��Ϣ

#include <stdafx.h>
#include <tchar.h>
#include <stdio.h>
//#include "Debug_new.h"


/// ��ȡ���г�������·�������ַ���ĩβ���ӷ�б��
char *G_GetAppPathA()
{
    static char s_Path[MAX_PATH];
    static bool s_bIsReady = false;
    if (!s_bIsReady)
    {
        ZeroMemory(s_Path,sizeof(s_Path));
        DWORD dwLength=GetModuleFileNameA(GetModuleHandle(NULL), s_Path, sizeof(s_Path));
        char *p = strrchr(s_Path, '\\');
        *p = '\0';
        s_bIsReady = true;
    }
    return s_Path;
}

void DebugPrintfA(const char *p, ...)
{
    char szTimeStr[32];
    char szDateStr[32];
    char szFilename[256];
    sprintf( szFilename, "%s\\Log\\GameLauncher.txt", G_GetAppPathA() );
    va_list arg;
    va_start( arg, p );
    FILE *fp = NULL;
    fp = fopen(szFilename, "a" );
    if (NULL == fp)
    {
        return;
    }
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    GetTimeFormatA( LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &sysTime, "HH:mm:ss", szTimeStr, 32);
    GetDateFormatA( LOCALE_USER_DEFAULT, LOCALE_USE_CP_ACP, &sysTime, "yyyy-MM-dd", szDateStr, 32);
    fprintf(fp,"[%s %s]--", szDateStr, szTimeStr);
    vfprintf(fp, p, arg );
    fprintf(fp,"\n");
    fclose(fp);
}

void ErrorPrintfA(const char *p, ...)
{
    char szTimeStr[32];
    char szDateStr[32];
    char szFilename[256];
    sprintf( szFilename, ("%s\\Log\\GameLauncherError.txt"), G_GetAppPathA() );
    va_list arg;
    va_start( arg, p );
    FILE *fp = NULL;
    fp = fopen(szFilename, "a" );
    if (NULL == fp)
    {
        return;
    }
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    GetTimeFormatA( LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &sysTime, "HH:mm:ss", szTimeStr, 32);
    GetDateFormatA( LOCALE_USER_DEFAULT, LOCALE_USE_CP_ACP, &sysTime, "yyyy-MM-dd", szDateStr, 32);
    fprintf(fp,"[%s %s]--", szDateStr, szTimeStr);
    vfprintf(fp, p, arg );
    fprintf(fp,"\n");
    fclose(fp);
}