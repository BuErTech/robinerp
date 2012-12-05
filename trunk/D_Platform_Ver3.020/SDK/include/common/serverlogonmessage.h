#pragma once

#ifndef AFC_SERVER_LOGON_MESSAGE_HEAD_FILE
#define AFC_SERVER_LOGON_MESSAGE_HEAD_FILE

#include "MainManage.h"
/********************************************************************************************/

//�汾����
#define GAME_MAX_VER					0								//������߰汾
#define GAME_LESS_VER					0								//������Ͱ汾

//��������
#define MAX_CONNECT						300								//���������
#define DATA_BASE_NAME					TEXT("AFCService")		//TEXT("ServerAccess")			//���ݿ�����

/********************************************************************************************/

//	����ͨ�����ݰ����� 

/********************************************************************************************/

//��½��������½��Ϣ
struct MSG_SL_S_LogonByNameInfo
{
	char								szName[61];							//��½����
	char								szMD5Pass[50];						//��½����
	char								szServerGUID[37];					//������ GUID
};

//��½��������½��Ϣ
struct MSG_SL_R_ConnectInfo
{
	long int								dwUserPower;						//�û�Ȩ��
	char								szSQLName[61];						//SQL ����
	char								szSQLPass[61];						//SQL ����
	char								szSQLAddr[16];						//SQL ��ַ
	char								szServiceIP[16];					//���� IP
};

/********************************************************************************************/

//	���ݿ�ͨ�����ݰ����� 

/********************************************************************************************/

//��½��������½��Ϣ
struct DL_SL_I_ServerLogonInfo
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	MSG_SL_S_LogonByNameInfo			LogonInfo;							//��½��Ϣ
	char								szAccessIP[16];						//��½ IP
};

//��½��������½���
struct DL_SL_O_ServerLogonInfo
{
	DataBaseResultLine					ResultHead;							//������ݰ�ͷ
	int								dwUserPower;						//�û�Ȩ��
	char								szSQLName[61];						//SQL ����
	char								szSQLPass[61];						//SQL ����
	char								szSQLAddr[16];						//SQL ��ַ
	char								szServiceIP[16];					//���� IP
};

/********************************************************************************************/

//	ͨ�ű�ʶ���� 

/********************************************************************************************/

//ͨ������ʶ
#define	MDM_SL_SERVICE					10									//��������ʶ

//ͨ�Ÿ�����ʶ
#define ASS_SL_LOGON_BY_NAME			1									//ͨ���û����ֵ�½
#define ASS_SL_LOGON_SUCCESS			2									//��½�ɹ�
#define ASS_SL_LOGON_ERROR				3									//��½ʧ��

//�������
#define ERR_SL_ERROR_UNKNOW				0									//δ֪����
#define ERR_SL_GUID_ERROR				1									//GUID ����
#define ERR_SL_LOGON_IP_ERROR			2									//��½ IP ���� 
#define ERR_SL_LOGON_NAME_ERROR			3									//��½���ִ���
#define ERR_SL_LOGON_NAME_LIMIT			4									//��½��������
#define ERR_SL_LOGON_PASS_ERROR			5									//��½�������
#define ERR_SL_LOGON_SUCCESS			6									//��½�ɹ�

/********************************************************************************************/

//�����½�����ʶ
#define DTK_SL_LOGON_BY_NAME			1									//����ͨ�����ֵ�½
#define DTK_SL_CHECK_CONNECT			2									//����������

//�����½�������
#define DTR_SL_ERROR_UNKNOW				0									//δ֪����
#define DTR_SL_GUID_ERROR				1									//GUID ���� 
#define DTR_SL_LOGON_IP_ERROR			2									//��½ IP ���� 
#define DTR_SL_LOGON_NAME_ERROR			3									//��½���ִ���
#define DTR_SL_LOGON_NAME_LIMIT			4									//��½��������
#define DTR_SL_LOGON_PASS_ERROR			5									//��½�������
#define DTR_SL_LOGON_SUCCESS			6									//��½�ɹ�

/********************************************************************************************/

#endif
