#ifndef _BZ_CRASH_RPT_H_INCLUDE
#define _BZ_CRASH_RPT_H_INCLUDE

/**
 * BzCrashRpt ��������
 *
 * ������Ŀ�����ڳ����г����쳣�����ʱ�����ʹ��󱨸浽ָ�������䣬�Ա���
 * ������Ա����
 *
 * ����: akinggw
 * ����: 2010.3.12
 */

#include "BzCommon.h"

/// ��ʼ�������Լ����쳣�������
EXT_CLASS void          BzCrash_Initiation(void); 
/// ����windowsϵͳ�е��쳣�������(��Ҫ���VS2005),���������Զ�����쳣������ƺ�ʹ��
EXT_CLASS void          BzCrash_DisableSetUnhandledExceptionFilter();
/// ����������Ŀ����
EXT_CLASS void          BzCrash_SetProjectName(const char* name);
/// �õ��쳣��Ϣ
EXT_CLASS const char*   BzCrash_GetExceptionString(void);
/// �õ�������Ŀ����
EXT_CLASS const char*   BzCrash_GetProjectName(void);
/// ����ѹ������(��ʱ����)
EXT_CLASS void          BzCrash_SetZipPassword(const char* pass);
/// �õ�ѹ������
EXT_CLASS const char*   BzCrash_GetZipPassword(void);
/// ���÷������ʼ���ַ
EXT_CLASS void          BzCrash_SetEmailSender(const char* address);
/// ���ý������ʼ���ַ
EXT_CLASS void          BzCrash_SetEmailReceiver(const char* address);
/// ����Smtp������
EXT_CLASS void          BzCrash_SetSmtpServer(const char* address);
/// ����Smtp�û�
EXT_CLASS void          BzCrash_SetSmtpUser(const char* name);
/// ����Smtp�û�����
EXT_CLASS void          BzCrash_SetSmtpPassword(const char* pass);
/// �����Ƿ�����ɺ�ɾ���ļ�����ʼ��ɾ����
EXT_CLASS void          BzCrash_DeleteSended(bool isDel);

#endif
