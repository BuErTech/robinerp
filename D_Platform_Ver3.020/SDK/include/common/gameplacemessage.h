#pragma once

#ifndef AFC_GAME_PLACE_MESSAGE_HEAD_FILE
#define AFC_GAME_PLACE_MESSAGE_HEAD_FILE

/********************************************************************************************/

///	����ͨ�����ݰ����� 

/********************************************************************************************/

///�汾����
#define GAME_PLACE_MAX_VER				4									///��Ϸ�������°汾
#define GAME_PLACE_LESS_VER				4									///��Ϸ������Ͱ汾

///�û���½���ʺţ��ṹ
struct MSG_GP_S_LogonByNameStruct
{
	UINT								uRoomVer;							///�����汾
	char								szName[64];							///��½����
	char								TML_SN[128];						
	char								szMD5Pass[52];						///��½����
	char								szMathineCode[64];					///���������� zxj 2009-11-10 ��������

    // PengJiLin, 2011-7-21, CPU��Ӳ�� ID
    char                                szCPUID[24];
    char                                szHardID[24];

	//֤���ţ�JianGuankun, 2011.11.10
	char								szIDcardNo[64];

	//�ֻ���֤�룬JianGuankun,2012.5.16
	char								szMobileVCode[8];

	int									gsqPs;
	//�û�ID��¼�����ID>0��ID��¼
	int									iUserID;

};

///�������� zxj 2009-11-12
struct MSG_GP_S_LockAccount
{
	UINT								dwUserID;				///�û�ID
	UINT								dwCommanType;			///������������,1��ʾҪ��������0��ʾҪ��������
	char								szMathineCode[64];		///������
	char								szMD5Pass[50];			///��������
	char								szMobileNo[50];			///�ֻ�����  Added by Jianguankun 2012.5.16
};

///���ֻ�����
struct MSG_GP_S_BindMobile
{
	UINT								dwUserID;				///�û�ID
	UINT								dwCommanType;			///������������,1��ʾҪ��󶨣�0��ʾҪ������
	char								szMobileNo[50];			///�ֻ�����  Added by Jianguankun 2012.5.16
};

///������  yjj
struct MSG_GP_NETSignal
{
        UINT                                                                dwUserID;
        UINT                                                                dwIndex;
        UINT                                                                dwTimeFlag;
};

///���������غ������Ϸ�����ʱ���ǩ������Ϸʹ�� yjj
struct MSG_GP_Game_NETSignal
{
	UINT                                                                dwUserID;
	UINT                                                                dwIndex;
	UINT                                                                dwTimeFlag;
	UINT                                                                dwSignalIndex;
};

///�û���½��ACC��
struct MSG_GP_S_LogonByAccStruct
{
	UINT								uRoomVer;							///�����汾
	int								dwAccID;							///ACC ����
	char								szMD5Pass[50];						///��½����
};

///�û���½���ֻ���
struct MSG_GP_S_LogonByMobileStruct
{
	UINT								uRoomVer;							///�����汾
	int								dwMobile;							///�ֻ�����
	char								szMD5Pass[50];						///��½����
};

///�û�ע����Ϣ���ݰ�
struct MSG_GP_S_RegisterStruct
{
	UINT								uRoomVer;							///�����汾
	BYTE								bBoy;								///�Ƿ����Ա�
	UINT								bLogoID;							///�û�ͷ�� ID ����
	char								szName[61];							///�û���¼��
	char								nickName[32];						///�û��ǳ�
	char								szRecommenderName[61];				///�Ƽ���
	char								szMD5Pass[50];						///�û���������
	char								szRegSQL[210];						///�û�ע�ḽ����Ϣ��SQL
};

//Added by JianGuankun 20111108
struct MSG_GP_R_IDVerify
{
	long int							dwUserID;							///�û� ID 
	char								szMD5IDCardNo[36];						///֤����
};

//Added by JianGuankun 2012.05.17
struct MSG_GP_R_MobileVerify
{
	long int							dwUserID;							///�û� ID 
	char								szMobileNo[50];						///�ֻ���
};
//ͳ�Ƶ�¼����
struct ONLINEUSERSCOUNT
{
	UINT								uiLogonPeopCount;					///��¼����
};

///������½�������ݰ�
struct MSG_GP_R_LogonResult
{
	long int							dwUserID;							///�û� ID 
	long int							dwGamePower;						///��ϷȨ��
	long int							dwMasterPower;						///����Ȩ��
	int									dwMobile;							///�ֻ�����
	int									dwAccID;							///Acc ����
	ULONG								dwLastLogonIP;						///�ϴε�½ IP
	ULONG								dwNowLogonIP;						///���ڵ�½ IP
	UINT								bLogoID;							///�û�ͷ��
	bool								bBoy;								///�Ա�
	char								szName[61];							///�û���¼��
	char								TML_SN[128];						///����ǩ��
	char								szMD5Pass[50];						///�û�����
	char								nickName[32];						///�û��ǳ�
	__int64								i64Money;							///�û����
	__int64								i64Bank;							///�û��Ƹ�
	int									dwFascination;						///����
	
	//JianGK 20111107���û�����
	char								szSignDescr[128];					///����ǩ��
	char								szRealName[20];						///��ʵ����
	char								szIDCardNo[36];						///֤����
	char								szMobileNo[50];						///�ƶ��绰
	char								szQQNum[20];						///QQ����
	char								szAdrNation[50];					///��ҵĹ���
	char								szAdrProvince[50];					///������ڵ�ʡ��
	char								szAdrCity[50];						///������ڵĳ���
	char								szZipCode[10];						///��������
	char								szEmail[50];						///�����ʼ�
	char								szAddress[128];						///��ϵ��ַ
	char								szSchooling[20];					///�Ļ��̶�
	char								szHomePage[128];					///������ҳ
	char								szTelNo[20];						///�̶��绰
	char								szMSNID[50];						//MSN�ʺ�
	//end JianGK 20111107

	int									dwTimeIsMoney;						///�ϴε�½ʱ������ȡ�Ľ��
	int									iVipTime;							///
	int									iDoublePointTime;					///˫������ʱ��
	int									iProtectTime;						///�����ʱ�䣬����
	bool								bLoginBulletin;						///�Ƿ��е�¼���棬Fred Huang,2008-05-20
	int									iLockMathine;						///��ǰ�ʺ��Ƿ�������ĳ̨������1Ϊ������0Ϊδ���� zxj 2009-11-13
	int									iBindMobile;						///��ǰ�ʺ��Ƿ���ֻ����룬1Ϊ�󶨣�0Ϊδ�� jianguankun 2012.10.10

	int									iAddFriendType;						///�Ƿ������κ��˼�Ϊ����
};

//Added by JianGuankun 20111108,�û���Ϣ�ṹ���޸���Ϣ�����ݰ�����
typedef MSG_GP_R_LogonResult MSG_GP_UserInfo;

struct MSG_GP_S_UserInfo
		: public MSG_GP_UserInfo
{
	MSG_GP_S_UserInfo(){::memset(this,0,sizeof(MSG_GP_S_UserInfo));}
};

struct MSG_GP_S_ChPassword
{
	long int dwUserID;		//�û�ID 
	char szHardID[24];		//Ӳ��ID
	char szMD5OldPass[80];  //�û�������
	char szMD5NewPass[80];  //�û�������
	MSG_GP_S_ChPassword(){::memset(this,0,sizeof(MSG_GP_S_ChPassword));}
};

//end JianGuankun add

// add wyl  �����б���Ϣ
struct MSG_GP_R_SellGameListResult
{
	char szGameName[61];						//��Ϸ����
	char szKindName[61];						//��������
	long NameID;								//��ϷID
	long KindID;								//����ID
	long ParamID;								//��Ϸ����ID
};

///��������������������������ݰ�	zxj 2009-11-16
struct MSG_GP_R_LockAndUnlock
{
	UINT	dwUserID;
	UINT	dwCommanType;			///������������
	UINT	dwCommanResult;			///����Ľ��
};

///��/������ֻ��������ݰ�
struct MSG_GP_R_BindMobile
{
	UINT	dwUserID;
	UINT	dwCommanType;			///������������
	UINT	dwCommanResult;			///����Ľ��
};

///��ȡ��Ϸ�������ݰ�
struct MSG_GP_SR_GetRoomStruct
{
	UINT								uKindID;							///���� ID
	UINT								uNameID;							///���� ID
};
///����б����
struct MSG_GP_User_Opt_Struct
{
	LONG								dwTargetID;							///��������
	LONG								dwUserID;							///����id
	UINT								uType;								///��������
};

///��ȡ��Ϸ�������ݰ�
struct MSG_GP_SR_OnLineStruct
{
	///UINT								uType;							///���� ID
	///UINT								uID;							///���� ID
	UINT								uKindID;							///���� ID
	UINT								uNameID;							///���� ID
	UINT								uOnLineCount;						///��������
};

//{{ Added by zxd 20100805 ����GM���ܣ������ͨ�����GM���ͨѶЭ��
/// ��ȡ����GM�б�
struct MSG_GP_R_GmList_t
{
	int _nCount;				///< ����
	int _arGmID[8];				///< ����GM��ID
	MSG_GP_R_GmList_t(){memset(this, 0, sizeof(MSG_GP_R_GmList_t));}
};

/// ��ͨ���������GM����
struct MSG_GP_S_Talk2GM_t
{
	int iUserID;		///< ������ID
	int iGMID;			///< GMID
	TCHAR szMsg[256];	///< �����͵���Ϣ
	MSG_GP_S_Talk2GM_t(){memset(this, 0, sizeof(MSG_GP_S_Talk2GM_t));}
};
// Added by zxd 20100805 ����GM���ܣ������ͨ�����GM���ͨѶЭ��}}

struct MSG_PROP_RESULT
{
	DWORD dwResult;
};

// JianGuankun,2012-1-4,�����û�IDȡ�ǳƣ�������ǳ�ȡ�û�IDר�ýṹ
struct MSG_GP_NickNameID_t
{
	long int _user_id;			//�û�ID
	TCHAR	 _nickname[61];		//�û��ǳ�
	BYTE	 _nType;			//0����ͨ���û�IDȡ�û��ǳ�\
								//1����ͨ���û��ǳ�ȡ�û�ID
	BYTE	 _nUseful;			//��;��ʶ	
	MSG_GP_NickNameID_t()
	{
		ZeroMemory(this, sizeof(MSG_GP_NickNameID_t));
	}
};
//End Add


///��½��������½��Ϣ
struct DL_GP_RoomListPeoCountStruct
{
	UINT							uID;							///�û� ID 
	UINT							uOnLineCount;						///��������
	int								uVirtualUser;					///��չ����������///20081211 , Fred Huang
};

//�������뷿��ʱ��������ύ�������� , add by lxl , 2010-10-08  
struct MSG_GP_CheckRoomPasswd 
{
	UINT uRoomID;			 //����ID
	char szMD5PassWord[50];  //����MD5����
};


struct DL_CheckRoomPasswd
{
	bool bRet;		// �ɹ����
	UINT uRoomID;	// �����
};

///��ȡ���
struct TReceiveMoney
{
	int			iUserID;
	__int64		i64Money;
	__int64		i64MinMoney;
	int			iCount;
	int			iTotal;
	int			iTime;
};

//�һ��б�
struct TCharmExchange_Item
{
	int iID;
	int iPoint;
	int iPropID;
	int iPropCount;
};

struct TCharmExchange
{
	int iUserID;
	int iID;
};

struct TMailItem
{
	int iMsgID;
	int	iTargetUserID;
	int iIsSend;
	int iMsgType;
	__int64 i64TimeSend;
	char szSName[50];
	char szTitle[100];
	char szMsg[500];
};

//�ֻ�������֤���ȡ��Added by JianGuankun 2012.5.16
struct MSG_GP_SmsVCode
{
	char szName[61];		//�û���
	char szMobileNo[50];	//�ֻ�����
	char szVCode[36];		//��������֤�룬ʹ��MD5����
	UINT nType;				//������֤�����ͣ�0Ϊ��½��֤\
											//1Ϊ������֤\
											//2Ϊ��������֤\
											//3Ϊ�޸��ֻ���֤
											//4Ϊ���ֻ���֤
											//5Ϊ������ֻ���֤
};

//��ȡ�û��Ĳ�����Ϣ��Added by JianGuankun 2012.7.12
struct TMSG_GP_BankFinanceInfo
{
	int		iUserID;			//�û�ID
	__int64 i64BankMoney;		//�û����е�Ǯ
	__int64 i64WalletMoney;		//�û�ȡ����Ǯ
	int     iLotteries;			//�û���ȯ��
};

/********************************************************************************************/

///	ͨ�ű�ʶ���� 

/********************************************************************************************/

///ͨ������ʶ
#define	MDM_GP_LOGON					100								///������½

///ͨ�Ÿ�����ʶ
#define ASS_GP_LOGON_BY_NAME			1									///ͨ���û����ֵ�½
#define ASS_GP_LOGON_BY_ACC				2									///ͨ���û�ACC ��½
#define ASS_GP_LOGON_BY_MOBILE			3									///ͨ���û��ֻ���½
#define ASS_GP_LOGON_REG				4									///�û�ע��
#define ASS_GP_LOGON_SUCCESS			5									///��½�ɹ�
#define ASS_GP_LOGON_ERROR				6									///��½ʧ��
#define ASS_GP_LOGON_ALLO_PART			7									///��ص�½
#define ASS_GP_LOGON_LOCK_VALID			8									///������֤

// PengJiLin, 2010-6-2, ����ǵ�����������������Ϣ֪ͨ����ˡ�(Ÿ��)
#define ASS_GP_LOGON_BY_SOFTWARE        10

#define ASS_GP_LOGON_MOBILE_VALID		11									///�ֻ���֤

///ͨ������ʶ
#define	MDM_GP_LIST						101								///��Ϸ�б�

///ͨ�Ÿ�����ʶ
#define ASS_GP_LIST_KIND				1									///��ȡ��Ϸ�����б�
#define ASS_GP_LIST_NAME				2									///��ȡ��Ϸ�����б�
#define ASS_GP_LIST_ROOM				3									///��ȡ��Ϸ�����б�
#define ASS_GP_LIST_COUNT				4									///��ȡ��Ϸ�����б�
#define ASS_GP_ROOM_LIST_PEOCOUNT		5									///��ȡ��Ϸ�����б�
#define ASS_GP_ROOM_PASSWORD			6									//���ͷ������� Add by lxl 2010-10-08����ͼ�������뷿��ʱ���ʹ���Ϣ
#define	ASS_GP_GET_SELLGAMELIST			7									//��ȡ��Ϸ�����б�

///ͨ������ʶ
#define	MDM_GP_REQURE_GAME_PARA					(MDM_GP_LIST+1	)		///������Ϸȫ�ֲ���

///ͨ������ʶ
#define	MDM_GP_MESSAGE					103								///��Ϣ�ӷ�

#define	MDM_GP_SYSMSG					104								///ϵͳ��Ϣ

#define	MDM_GP_USER_OPT					105								///�������Ӻ���
#define ASS_UPDATE_FRIEND				1									///���º���
#define ASS_UPDATE_EMENY				2									///���»���

///��������������ID zxj 2009-11-12
#define MDM_GP_LOCK_ACCOUNT				106									
#define ASS_LOCK_ACCOUNT				1


//��ȡ���
#define MDM_GP_RECEIVEMONEY					108
#define ASS_RECEIVE						1		//�ɹ�
#define ASS_RECEIVE_PASS				2		//���ϵ�Ǯ����
#define ASS_RECEIVE_OVER				3		//�Ѿ�ȫ����ȡ���
#define ASS_RECEIVE_TIMELESS			4		//ʱ�仹û��
#define ASS_RECEIVE_ERROR				5		//��ȡ����


///�����һ�
#define MDM_GP_CHARMEXCHANGE			109
#define ASS_GETLIST						1
#define ASS_EXCHANGE					2

///ͨ�Ÿ�����ʶ
#define ASS_GP_NEWS_SYSMSG				1								///���ź�ϵͳ��Ϣ
#define ASS_GP_DUDU						2									///С����

//{{ Added by zxd 20100805 ����GM���ܣ������ͨ�����GM���ͨѶЭ��
/// ͨ������ʶ
#ifndef MDM_GP_PLAYER_2_GM
#define MDM_GP_PLAYER_2_GM				107								/// ��ͨ�����GM֮���ͨ��Э��
#	define ASS_GP_GET_GM_LIST			1									///<��ȡ��ǰ����GM�б�
#	define ASS_GP_TALK_2_GM				2									///<��ָ����ŵ�GM������Ϣ
#	define ASS_GP_MSG_FROM_GM			3									///<���մ�GM����������Ϣ
#	define ASS_GR_MSG_BE_KICKED			4									///<��GM�߳�����
#	define ASS_GR_MSG_BE_BANDID			5									///<��GM���
#	define ASS_GR_MSG_BE_AWARD			6									///<��GM����
#	define ASS_GR_MSG_PRINTLOG			7									///<GM��ӡ��־
#endif//MDM_GP_PLAYER_2_GM
// Added by zxd 20100805 ����GM���ܣ������ͨ�����GM���ͨѶЭ��}}


//{{ Added by JianGuankun 20111108 ����û����Ϲ���ͨѶЭ��
#define MDM_GP_USERINFO	 				115
#define	ASS_GP_USERINFO_UPDATE_BASE		1		//�û����»�����Ϣ
#define ASS_GP_USERINFO_UPDATE_DETAIL	2		//�û�������ϸ��Ϣ
#define ASS_GP_USERINFO_UPDATE_PWD		3		//�û��޸�����
#define ASS_GP_USERINFO_ACCEPT			5		//������ѽ���
#define ASS_GP_USERINFO_NOTACCEPT		6		//�����δ�ܽ���
#define ASS_GP_USERINFO_NICKNAMEID		10		//��������ǳ���ID��ID���ǳ�
//Added by JianGuankun 20111108}}

//{{ Added by JianGuankun 20111108 ����û����Ϲ���ͨѶЭ��
#define MDM_GP_MAIL	 				116
#define	ASS_GP_MAILINFO				1			//�û����»�����Ϣ

//{{ Added by JianGuankun 2012.5.16 ����ֻ���ѶЭ��
#define MDM_GP_SMS					117
#define ASS_GP_SMS_VCODE			1			//�ֻ���֤�����
//End Added by JianGuankun}}

#define MDM_GP_LASTLOINGGAME		118
#define ASS_GP_LASTLOINGGAME		1

//=======================================�ֲ�ʽʱ��Z��B��ͨѶЭ�� rongqf20120425
#define MDM_ZD_PACKAGE					123
#define		ASS_ZD_CONNECT					1
#define		ASS_ZD_LOGON					2
#define			HDC_ZD_CONNECT				1
#define			HDC_ZD_DISCONNECT			2
#define			HDC_ZD_KICK					3
#define		ASS_ZD_BATCH					3
#define		ASS_ZD_SINGLE					4
#define			HDC_ZD_FAIL					2

//{{ Added by rende 20120920 ͳ�Ƶ�¼����
#define MDM_GP_LOGONUSERS	 				119
#define	ASS_GP_LOGONUSERS_COUNT					1

//{{ Added by JianGuankun 2012.10.10 ���ֻ�
#define MDM_GP_BIND_MOBILE	 				112
#define	ASS_GP_BIND_MOBILE					1



struct TZServerInfo
{
	int iZid;
	int	iServerPort;
};

struct MSG_ZDistriInfo
{
	DWORD	dwUserID;
	int		nZID;
};
//=======================================�ֲ�ʽʱ��Z��B��ͨѶЭ�� rongqf20120425

/********************************************************************************************/

///�������
#define ERR_GP_ERROR_UNKNOW				0									///δ֪����
#define ERR_GP_LOGON_SUCCESS			1									///��½�ɹ�
#define ERR_GP_USER_NO_FIND				2									///��½���ִ���
#define ERR_GP_USER_PASS_ERROR			3									///�û��������
#define ERR_GP_USER_VALIDATA			4									///�û��ʺŽ���
#define ERR_GP_USER_IP_LIMITED			5									///��½ IP ��ֹ
#define ERR_GP_USER_EXIST				6									///�û��Ѿ�����
#define ERR_GP_PASS_LIMITED				7									///�����ֹЧ��
#define ERR_GP_IP_NO_ORDER				8									///����ָ����ַ 
#define ERR_GP_LIST_PART				9									///������Ϸ�б�
#define ERR_GP_LIST_FINISH				10									///ȫ����Ϸ�б�
#define ERR_GP_USER_LOGON				11									///���ʺ��Ѿ���¼
#define ERR_GP_USERNICK_EXIST			12									///���ǳ��Ѿ�����
#define ERR_GP_USER_BAD					13									///δ���ַ�
#define ERR_GP_IP_FULL					14									///IP����
#define ERR_GP_LOCK_SUCCESS				15									///���������ɹ�	2009-11-12
#define	ERR_GP_ACCOUNT_HAS_LOCK			16									///�����Ѿ���������״̬	2009-11-12
#define	ERR_GP_UNLOCK_SUCCESS			17									///��������ɹ� 2009-11-12
#define	ERR_GP_NO_LOCK					18									///����������û�����������Խ���ʧ�� 2009-11-12
#define	ERR_GP_CODE_DISMATCH			19									///�����벻ƥ�䣬����ʧ�ܡ�	2009-11-12
#define ERR_GP_ACCOUNT_LOCKED			20									///���˺�������ĳ̨��������¼ʧ�� 2009-11-12 zxj

#define ERR_GP_MATHINE_LOCKED			21

// PengJiLin, 2010-6-7, ��������֤���صĴ����롣Ÿ��
#define ERR_GP_USER_NOT_EXIST           30      // �û�������
#define ERR_GP_USER_OVERDATE            31      // �û��ѹ���
#define ERR_GP_DATABASE_FUNC_ERROR      32      // ���ݿ����ʧ��
#define ERR_GP_DATABASE_RETURN_ERROR    33      // ���ݿⷵ��ִ�д�����

/********************************************************************************************/
///�쳣����

//JianGuankun 20111110
#define EXP_GP_ALLO_PARTY				50								///���˺���ص�½

/********************************************************************************************/

#define MDM_GP_NETSIGNAL                110                                ///�����ź���Ϣ	

#define MDM_GP_NOTICE_MSG				111									////��ʾ��Ϣ  add by lxl 2010-11-10

static CString ERR_RP_STRING[21] = {"δ֪����", "��¼�ɹ�", "��¼�û�������", "�û��������", "�û��ʺű���", 
"��½IP����", "�û��Ѿ�����", "�����ֹЧ��", "����ָ����ַ", "������Ϸ�б�", 
"ȫ����Ϸ�б�", "���ʺ��Ѿ���¼", "���ǳ��Ѿ�����", "�ں��зǷ��ַ�", "ͬһIPÿ��ֻ��ע��20���ʺ�", 
"���������ɹ�", "�����Ѿ���������״̬", "��������ɹ�", "����û������������ʧ��", "�����벻ƥ�䣬����ʧ��", 
"���˺�������ĳ̨��������¼ʧ��"};

/********************************************************************************************/

///Center Server///
///ͨ������ʶ
#define	MDM_GP_REQURE_GAME_PARA					(MDM_GP_LIST+1	)		///������Ϸȫ�ֲ���



#include"CenterServerhead.h"


///

#endif
