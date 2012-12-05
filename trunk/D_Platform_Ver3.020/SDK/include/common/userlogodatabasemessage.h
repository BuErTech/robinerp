#pragma once

#ifndef AFC_USER_LOGO_DATAMESSAGE_HEAD_FILE_
#define AFC_USER_LOGO_DATAMESSAGE_HEAD_FILE_

#include "MainManage.h"

//���ݿ���Ϣ
#define DTK_GP_USERLOGO_UPDATE			100						//����ͷ��ID
#define DTK_GP_USERLOGO_GETMD5			110						//ȡͷ���MD5ֵ

//�����û�ͷ�����ݿ�����ṹ
typedef struct
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	long								dwUserID;							//�û�ID��
	int									nIconIndex;							//==0xFF���Զ��壬������ϵͳĬ��
	char								szLogoMD5[33];						//�û�ͷ���ļ���MD5ֵ
} UL_GP_I_UpdateUserLogo;

//�����ݿ���ȡ��ͷ���MD5ֵ
typedef struct  
{
	DataBaseLineHead					DataBaseHead;
	long								dwRequestUserID;					//����ȡͷ��MD5ֵ���û�ID��
	long								dwUserID;							//������ȡͷ��MD5ֵ���û�ID��
}UL_GP_I_RequestInformationLogoMD5;

typedef struct  
{
	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
	MSG_UL_S_INFORMATION				UserLogoInformation;

}UL_GP_I_UserLogoInformation;

#endif