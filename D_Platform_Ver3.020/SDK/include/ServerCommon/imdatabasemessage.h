#pragma once

#ifndef AFC_IM_DATAMESSAGE_HEAD_FILE_
#define AFC_IM_DATAMESSAGE_HEAD_FILE_

#include "AFCDataBase.h"
#include "MainManage.h"
//���ݿ���Ϣ
#define DTK_GP_IM_CLEAR					200						//����������ʱ���û���0��������������
#define DTK_GP_IM_GETCOUNT				201						//ȡ���û�������
#define DTK_GP_IM_USERLIST				202						//�����û��ĺ�����Ϣ(�б�)
#define DTK_GP_IM_USERONLINE			203						//�ϡ����߷����û�����
#define DTK_GP_IM_ONLINE				204						//�������û�������
#define DTK_GP_IM_OFFLINE				205						//���������ݿ�д��¼
#define DTK_GP_IM_SETUSER				206						//���ú��ѷ���


typedef struct  
{
	DataBaseLineHead					DataBaseHead;				//���ݰ�ͷ
	long								dwUserID;					//�û�ID
	long								dwRemoteID;					//���ѵ�ID
	int									groupID;					//��ID��1-���ѣ�3-��������0-ɾ��
}IM_GP_I_SETUSER;
//�û�����
typedef struct  
{
	DataBaseLineHead					DataBaseHead;				//���ݰ�ͷ
	long								dwUserID;					//�����û���ID��
}IM_GP_I_USEROFFLINE;

//ȡ����������δʹ�ã�����
typedef struct
{
	DataBaseLineHead					DataBaseHead;				//���ݰ�ͷ
	long								dwUserID;					//�û�ID��
	int									dwUserCount;				//������
} IM_GP_I_GETUSERCOUNT;

//ȡ���ѵ���������δʹ�ã�����
typedef struct  
{
	DataBaseResultLine					ResultHead;					//���ݰ�ͷ
	long								dwUserID;					//�û�ID��
	int									dwUserCount;				//������
	int									dwUserCountReturn;			//���صĺ�����
}IM_GP_O_GETUSERCOUNT;

//ȡ�����б�
typedef struct
{
	DataBaseResultLine					ResultHead;
	int									dwListCount;				//����Ϣ���м����û�
	int									dwLength;					//��Ϣ�峤��
	char								szData[MAX_BLOCK_MSG_SIZE];	//��󳤶�
}IM_GP_O_USERLIST;

//�����û���Ϣ
typedef struct  
{
	DataBaseResultLine					ResultHead;
	MSG_IM_S_GETUSERINFO				userInfo;
}IM_GP_O_USERINFO;


//�����б���Ϣ
struct DL_GP_O_SellGameListResult
{
	DataBaseResultLine					ResultHead;
	MSG_GP_R_SellGameListResult         SellGameListResult;
};
#endif