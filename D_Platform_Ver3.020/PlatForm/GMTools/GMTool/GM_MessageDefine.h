//////////////////////////////////////////////////////////////////////////
/// ����GM���ܵ�Socket��Ϣ���壬���ݷ���Э�鶨��
/// ���������ܶ�
/// ���ڣ�20100722
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __GM_MESSAGE_DEFINE_H
#define __GM_MESSAGE_DEFINE_H

#include <windows.h>
#include "basemessage.h"	///< ���struct NetMessageHead����

/// �����ڽṹ�����ʼ����Ա�ĺ궨�壬�����г�Ա������ֵ0
/// ����ʹ��ע�����ʹ�ñ���Ľṹ���ڲ������麯������
///
#define INIT_STRUCT( T ) \
	T()\
	{\
	ZeroMemory(this, sizeof(T));\
	}
/////////////// ʾ������ ///////////////////
// struct Test_t
// {
// 	 int _nLen;
//	 char _pStr[256];
//	 INIT_STRUCT( Test_t );	///< �ṹ�����������꣬���൱�������һ����ʼ����Ա�Ĺ��캯��
// };

#ifndef STRUCT_DATALINEHEAD
#define STRUCT_DATALINEHEAD
//���ݶ�����Ϣͷ
struct DataLineHead
{
	UINT						uSize;								//���ݴ�С
	UINT						uDataKind;							//��������
	INIT_STRUCT(DataLineHead);
};
#else
struct DataLineHead;
#endif//STRUCT_DATALINEHEAD

#ifndef STRUCT_DATABASERESULTLINE
#define STRUCT_DATABASERESULTLINE
///���ݿ�����Ϣ�ṹ����
struct DataBaseResultLine
{
	DataLineHead						LineHead;					///����ͷ
	UINT								uHandleRusult;				///������
	UINT								uHandleKind;				///��������
	UINT								uIndex;						///��������
	DWORD								dwHandleID;					///�����ʶ
	INIT_STRUCT(DataBaseResultLine);
};
#endif //STRUCT_DATABASERESULTLINE

#ifndef STRUCT_DATABASELINEHEAD
#define STRUCT_DATABASELINEHEAD
///���ݿ����ݰ�ͷ�ṹ
struct DataBaseLineHead
{
	DataLineHead					DataLineHead;							///����ͷ
	UINT							uHandleKind;							///��������
	UINT							uIndex;									///��������
	DWORD							dwHandleID;								///�����ʶ
	INIT_STRUCT(DataBaseLineHead);
};
#endif //STRUCT_DATABASELINEHEAD

///---------------------
/// �������Ͷ���
///---------------------
#define GM_ACT_AWORD		1	///< �������������浽�����У���ʵʱд���û�Ǯ���������Ϣ���͵�Z������
#define GM_ACT_NO_TALK		2	///< ���ԣ���������������ǰ��ĳ�����У�GM�����߿ͻ�����ͬʱ���������ڵ�W��������������Ϣ
#define GM_ACT_KICK_OUT		3	///< �߳����䣬��������������ǰ��ĳ�����У�GM�����߿ͻ�����ͬʱ���������ڵ�W��������������Ϣ
#define GM_ACT_BANDON_ID	4	///< ��ֹ��¼������ţ���������������ǰ��ĳ�����У�GM�����߿ͻ���ͬʱ���������ڵ�W��������������Ϣ


///=======================================================================
/// SocketЭ�鶨��
///=======================================================================

//{{ Added by zxd 20100805 ����GM���ܣ������ͨ�����GM���ͨѶЭ��
/// ͨ������ʶ
#ifndef MDM_GP_PLAYER_2_GM
#define MDM_GP_PLAYER_2_GM				107								/// ��ͨ�����GM֮���ͨ��Э��
#	define ASS_GP_GET_GM_LIST			1									///<��ȡ��ǰ����GM�б�
#	define ASS_GP_TALK_2_GM				2									///<��ָ����ŵ�GM������Ϣ
#	define ASS_GP_MSG_FROM_GM			3									///<���մ�GM����������Ϣ
#endif//MDM_GP_PLAYER_2_GM
// Added by zxd 20100805 ����GM���ܣ������ͨ�����GM���ͨѶЭ��}}


#define MDM_GAMEMASTER_MESSAGE	255	///< GM��Ϣ��ID

///-----------------------------------------------------------------------
/// ����ΪGM��Ϣ��ID����
///-----------------------------------------------------------------------
/// 1���·���������
/// 2���·�ָ����ҵ�����ID��Ϣ����ʷ������¼�͵�ǰ״̬
/// 3���ϴ�����ҵĲ���
/// 4����ĳ�ض���ҷ��ԣ����㲥��Ϣ������
/// 6������Z��������ȡ�õ���ID�����ƶ�Ӧ��ϵ
/// 7��GM��¼Э�飬����ͨ��ҵ�¼���ò�ͬЭ��
///-----------------------------------------------------------------------
#	define	ASS_GM_MESSAGE_AWARD_TYPE		1	///< ����ʱ�޲����������в����ṹ
#		define HC_GM_GET_LIST_OK				1	///< ����ʱ��ʾ��������Ϊ��Ч���ݣ�δ�����
#		define HC_GM_GET_LIST_END				2	///< ����ʱ��ʾ��������Ϊ����ָʾ
#	define	ASS_GM_MESSAGE_OPERATE_HISTORY	2	///< ����ʱ�м򵥲���������Ϊ���ͼ��û�ID���¼�����ǳƣ������в����ṹ
#	define	ASS_GM_MESSAGE_OPERATE_ACT		3	///< ����ʱ�в����������޲���
#	define	ASS_GM_MESSAGE_BROADCAST		4	///< ����ʱ�в�������������ԭ�й㲥��Ϣ
#	define	ASS_GM_MESSAGE_LOGON_USER		5	///< �����У������в����ṹ
#	define	ASS_GM_MESSAGE_PROP_INFO		6	///< ����ʱ�޲����������в����ṹ��ȡ����ID�����ƶ�Ӧ��ϵ�б�
#	define	ASS_GM_MESSAGE_GM_LOGON			7	///< ����ʱͬ��ͨ�û���¼������ʱ����ͨ�û���¼����������٣���ͷ��ID֮��
#	define  ASS_GM_MESSAGE_PRINTLOG			8	///< ֪ͨW��ӡ��־����Ϣ
/// ����ΪGM��Ϣ��ID����



///������룬���������ط��Ѿ���������Ҫ������
#ifndef ERR_GP_ERROR_UNKNOW
#	define ERR_GP_ERROR_UNKNOW				0									///δ֪����
#	define ERR_GP_LOGON_SUCCESS				1									///��½�ɹ�
#	define ERR_GP_USER_NO_FIND				2									///��½���ִ���
#	define ERR_GP_USER_PASS_ERROR			3									///�û��������
#	define ERR_GP_USER_VALIDATA				4									///�û��ʺŽ���
#	define ERR_GP_USER_IP_LIMITED			5									///��½ IP ��ֹ
#	define ERR_GP_USER_EXIST				6									///�û��Ѿ�����
#	define ERR_GP_PASS_LIMITED				7									///�����ֹЧ��
#	define ERR_GP_IP_NO_ORDER				8									///����ָ����ַ 
#	define ERR_GP_LIST_PART					9									///������Ϸ�б�
#	define ERR_GP_LIST_FINISH				10									///ȫ����Ϸ�б�
#	define ERR_GP_USER_LOGON				11									///���ʺ��Ѿ���¼
#	define ERR_GP_USERNICK_EXIST			12									///���ǳ��Ѿ�����
#	define ERR_GP_USER_BAD					13									///δ���ַ�
#	define ERR_GP_IP_FULL					14									///IP����
#	define ERR_GP_LOCK_SUCCESS				15									///���������ɹ�	2009-11-12
#	define	ERR_GP_ACCOUNT_HAS_LOCK			16									///�����Ѿ���������״̬	2009-11-12
#	define	ERR_GP_UNLOCK_SUCCESS			17									///��������ɹ� 2009-11-12
#	define	ERR_GP_NO_LOCK					18									///����������û�����������Խ���ʧ�� 2009-11-12
#	define	ERR_GP_CODE_DISMATCH			19									///�����벻ƥ�䣬����ʧ�ܡ�	2009-11-12
#	define ERR_GP_ACCOUNT_LOCKED			20									///���˺�������ĳ̨��������¼ʧ�� 2009-11-12 zxj
#endif//ERR_GP_ERROR_UNKNOW
#	define ERR_GM_OPERATOR_SUCCESS			201									///< GM����ҵĲ����ɹ�
#	define ERR_GM_OPERATOR_FAILED			202									///< GM����ҵĲ���ʧ��
///-----------------------------------------------------------------------
/// Э��ṹ����
///-----------------------------------------------------------------------

/// 1���·����������ýṹ
struct GM_AwardType_t
{
	struct PropMemo_t
	{
		int iPropID;			///< ���߱��
		int iCount;				///< ����
	};
	int iSeriNo;				///< �������
	int iGoldNum;				///< �����������Ϊ��
	PropMemo_t iPropAward[4];	///< �������ƺ�������������֣��ɶ�Ϊ0
	INIT_STRUCT(GM_AwardType_t);
};

/// 2���·�ָ������������������ļ�¼������״̬
/// �������У������ID, ���¼�����ǳ�ȡ������ʷ
struct GM_GetOperationByPlayer_t
{
	union PlayerInfo_u
	{
		int UserID;				///< ���ID��
		TCHAR szName[32];		///< ��¼�������ǳ�
	};
	BYTE bType;					///< ���ID����¼�������ǳƣ�0��ʾID�ţ�1��¼����2��ʾ�ǳ�
	PlayerInfo_u playerInfo;	///< �����Ϣ����bType������������ID�����ǵ�¼���������ǳ�
	INIT_STRUCT(GM_GetOperationByPlayer_t);
};
/// �������У�������ֵĶ�Ӧ��ϵ��������ҵ�ǰ��״̬��������Ӧ��¼����Ϊ0ֵ��""���ַ���
struct GM_OperatedPlayerInfo_t
{
	int iUserID;				///< �û�ID
	TCHAR szUserName[32];		///< �û���¼��
	TCHAR szNickName[32];		///< �û��ǳ�
	/// ��ǰ��¼״̬
	int ZID;					///< ��ǰ��¼��ZServer��ţ�0��ʾ��
	int WID;					///< ��ǰ��¼��WServer��ţ�������ţ�0��ʾ��
	/// GM������¼��Ϣ�����޼�¼�������ֵΪ0
	int iGMID;					///< ���и��������GM���
	int iOperation;				///< �Ը��û��Ĳ���
	int dtDate;					///< ����ʱ��
	int iPeriod;				///< ����ʱ�䣬��λ��
	TCHAR szReason[64];			///< �����β�����ԭ��
	INIT_STRUCT(GM_OperatedPlayerInfo_t);
};

/// 3���ϴ�����ҵĲ���
struct GM_OperateByID_t
{
	int iOperation;		///< �������ͱ�ţ��ɴ���ȷ���ͷ����ǽ���
	int iUserID;		///< �������ID
	int iGMID;			///< ����ԱID
	int iWorkMinutes;	///< ���ڳͷ�ʱ����ʾ����ʱ�䣻���ڽ���ʱ����д�������
	TCHAR szReason[64];	///< ԭ��
	TCHAR szNickName[50]; ///< �û��ǳ�
	INIT_STRUCT(GM_OperateByID_t);
};

/// 4����ĳ�ض���ҷ��ԣ����㲥��Ϣ������
struct GM_Broadcast_t
{
	int iUserID;		///< ���ID����Ϊ�㣬��ʾ�㲥
	int iGMID;			///< GMID����㲥��Ϣ����Ϊ�㣬ָ����Ŀ�����ID������Ϊ�㣬����Ŀ����Ҳ�֪�����ĸ�GM������Ϣ
	TCHAR szMsg[256];	///< ��Ҫ�㲥���͵���Ϣ
	INIT_STRUCT(GM_Broadcast_t);
};

/// 5������Z����������GM�Լ���¼ʱ�������·���ǰ�Ѿ���¼������б���Ҫ�ڿͻ��˱����û�ID���ǳƵĶ�Ӧ��ϵ
/// 6������ID�����ƶ�Ӧ��ϵ
struct GM_GetPropInfo_t
{
	int iPropID;			///< ����ID
	TCHAR szPropName[32];	///< ��������
	INIT_STRUCT(GM_GetPropInfo_t);
};
/// 7��GM��¼Э�飬�ṹ������ͨ��ҵ�¼��������

/// GM��¼���ʺţ��ṹ
struct GM_MSG_S_LogonByName_t
{
	UINT								uRoomVer;							///< �����汾
	char								szName[61];							///< ��¼����
	char								szMD5Pass[50];						///< ��¼����
	char								szMathineCode[32];					///< ���������� zxj 2009-11-10 ��������
	INIT_STRUCT(GM_MSG_S_LogonByName_t);
};

///��ӡ��־��֪ͨ
struct GM_MSG_PrintLog_t
{
	UINT uRoomID;	///< ����ID
	INIT_STRUCT(GM_MSG_PrintLog_t);
};

///=======================================================================
/// ���ݿ����Э�鶨��
///=======================================================================
#define GM_DT_MESSAGE_START	220	///< ��ʼ
#define GM_DT_MESSAGE_END	225	///< ����

/// 1����ȡ��������
/// 2����ȡ��ʷ������������б�
/// 3��д�����ҵĲ�������¼����
/// 4����ȡ����ID�����ƶ�Ӧ��ϵ
/// 5��GM�ʺŵ�¼��Ϣ

///-----------------------------------------------------------------------
/// ��GM��ص����ݿ����ָ��궨�尲����200�Ժ�
/// �μ�GameLogonDT.h, GameRoomLogonDT.h�ļ�

#define DTK_GET_AWARD_TYPE_GM			221			///< ��ȡ��������
#define DTK_GET_OPEATED_USER_GM			222			///< ��ȡָ������ҵı�����������Ϣ����ǰ״̬
#define DTK_PUT_OPEATION_USER_GM		223			///< д�����ҵĲ���
#define DTK_GET_PROP_INFO_GM			224			///< ��ȡ����ID�����ƶ�Ӧ��ϵ�б�
#define DTK_LOGON_BY_NAME_GM			225			///< GM����GM���ߵ�¼


/// ���ݿ�洢���̷���ֵ����
//-- 0 ��¼�ɹ�
//-- 1 �û�������
//-- 2 ���벻��ȷ
//-- 3 û��GMȨ��
//-- 4 �����벻��ȷ
//-- 5 �Ѿ�����һ��ͬ��GM��¼��
#define DTR_GM_LOGON_SUCCESS		0
#define DTR_GM_USER_NOT_FOUND		1
#define DTR_GM_PASSWORD_ERROR		2
#define DTR_GM_HAS_NOT_RIGHT		3
#define DTR_GM_MACHINECODE_ERR		4
#define DTR_GM_ALREADY_LOGON		5
#define DTR_GM_OPERATE_SUCCESS		6

/// ȡ���ݼ�ָʾֵ����
#	define DTR_GM_GET_RECORD_OK		0
#	define DTR_GM_GET_RECORD_END	1
///--------------------------------------
/// GM��ȡ����������
///--------------------------------------

/// 1�����ؽ��������ýṹ�����ж������������ݼ���ѭ�������Ͷ�η��ؽ��
struct GM_DT_O_AwardType_t
{
	DataBaseResultLine					ResultHead;							///< ������ݰ�ͷ
	GM_AwardType_t						awardType;							///< ��������
	INIT_STRUCT(GM_DT_O_AwardType_t);
};
///--------------------------------------
/// 2��GM��ȡָ����ҵ�������Ϣ����ǰ����״̬����ʷ������¼
///--------------------------------------
/// �������
struct GM_DT_I_GetOperatedUser_t
{
	DataBaseLineHead					DataBaseHead;						///< ���ݰ�ͷ
	GM_GetOperationByPlayer_t			requestPlayer;						///< ����������Ϣ
};
/// �����
struct GM_DT_O_OperatedUser_t
{
	DataBaseResultLine					ResultHead;							///< ������ݰ�ͷ
	GM_OperatedPlayerInfo_t				operatedPlayer;						///< ����ҵĲ�����¼
	INIT_STRUCT(GM_DT_O_OperatedUser_t);
};
///--------------------------------------
/// 3��д�����ҵĲ�����¼
///--------------------------------------
struct GM_DT_I_OperateUser_t
{
	DataBaseLineHead					DataBaseHead;						///< ���ݰ�ͷ
	GM_OperateByID_t					operation;							///< ����ҵĲ�������
	INIT_STRUCT(GM_DT_I_OperateUser_t);
};

///--------------------------------------
/// 5������ID�����ƶ�Ӧ��ϵ��
///--------------------------------------
struct GM_DT_O_GetPropInfoResult_t
{
	DataBaseResultLine					ResultHead;							///< ������ݰ�ͷ
	GM_GetPropInfo_t					propInfo;							///< ������Ϣ
};
///--------------------------------------
/// 6��GM��¼��
///--------------------------------------

/// GM��¼��������¼��Ϣ
struct GM_DT_I_LogonByName_t
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	GM_MSG_S_LogonByName_t				logonInfo;							//��¼��Ϣ
	char								szAccessIP[16];						//��¼ IP
	INIT_STRUCT(GM_DT_I_LogonByName_t);
};
///GM������½�������ݰ�
struct GM_MSG_O_LogonResult_t
{
	UINT							dwUserID;							///< �û� ID 
	int								iMasterRight;						///< ����Ȩ��
	ULONG								dwLastLogonIP;						///< �ϴε�½ IP
	ULONG								dwNowLogonIP;						///< ���ڵ�½ IP
	char								szName[61];							///< �û���¼��
	char								szMD5Pass[50];						///< �û�����
	INIT_STRUCT(GM_MSG_O_LogonResult_t);
};
/// GM��¼���ݿⷵ�ؽ��
struct GM_DT_O_LogonResult_t
{
	DataBaseResultLine					ResultHead;							///< ������ݰ�ͷ
	GM_MSG_O_LogonResult_t				logonResult;						///< ��¼����������ݽṹ
	INIT_STRUCT(GM_DT_O_LogonResult_t);
};
/// 


#endif//__GM_MESSAGE_DEFINE_H
