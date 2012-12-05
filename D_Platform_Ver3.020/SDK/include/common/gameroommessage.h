#pragma once

#ifndef AFC_GAME_ROOM_MESSAGE_HEAD_FILE
#define AFC_GAME_ROOM_MESSAGE_HEAD_FILE

/********************************************************************************************/

#include "BaseMessage.h"
#include "ComStruct.h"
#include <vector>
using namespace std;
///��Ϸ�����½
struct MSG_GR_S_RoomLogon
{
	UINT								uNameID;							///���� ID
	LONG								dwUserID;							///�û� ID
	UINT								uRoomVer;							///�����汾
	UINT								uGameVer;							///��Ϸ�汾
	CHAR								szMD5Pass[50];						///��������
};

///��Ϸ�����½
struct MSG_GR_R_LogonResult
{
	LONG								dwGamePower;						///�û�Ȩ��
	LONG								dwMasterPower;						///����Ȩ��
	LONG								dwRoomRule;							///���ù���
	UINT								uLessPoint;							///���پ���ֵ
	UINT								uMaxPoint;							///��ྭ��ֵ
	UserInfoStruct						pUserInfoStruct;						///�û���Ϣ

	///wushuqun 2009.6.6
	///��¼����ʱ��ʱ��ȡ�����������
	int									nVirtualUser;
	int									nPresentCoinNum;  ///< ���ͽ������


	
	//����ר��
	int									iContestID;
	int									iLowCount;
	__int64								i64Chip;
	__int64								i64TimeStart;
	__int64								i64TimeEnd;
	__int64								i64LowChip;
	int									iTimeout;
	int									iBasePoint;


};

///��Ϸ�����½
struct MSG_GR_R_OtherRoom
{
	int									iRoomID;
	CHAR								szGameRoomName[61];					///��������
};

///��Ϸ����״̬
struct MSG_GR_DeskStation
{
	BYTE								bDeskStation[100];					///����״̬
	BYTE								bDeskLock[100];						///����״̬
	int									iBasePoint[100];					///���ӱ���
	BYTE                                bVirtualDesk[100];                 ///����״̬  yjj 090325
};

struct CDeskStation
{
	vector<BYTE> vLocks;
	vector<BYTE> vVirtuals;
	vector<BYTE> vPlayings;
};

///��Ǯ��
struct MSG_GR_UserBank
{
	long int						dwUserID;
	__int64						    i64Bank;								///Ǯ��
	char							szTwoPassword[50];					//��������
	int								nVer;								//���а汾
};
///������(ʹ�ö�������)
struct MSG_GR_OpenBank
{
	char							szMD5Pass[50];						///��������
};

///ȡǮ
struct MSG_GR_CheckMoney
{
	long int						dwUserID;							///�û� ID
	__int64							i64CheckOut;						///ȡ�����
	__int64							i64MoneyInBank;						///ʣ��
	__int64							i64MoneyInRoom;
	int								uGameID;							///��ϷID
};
///�û����뷿��
struct MSG_GR_R_UserCome
{
	UserInfoStruct						pUserInfoStruct;						///�û���Ϣ
};

///�û��뿪����
struct MSG_GR_R_UserLeft
{
	long int							dwUserID;							///�û� ID
};

///�û�����
struct MSG_GR_S_UserSit
{
	BYTE								bDeskIndex;							///��������
	BYTE								bDeskStation;						///����λ��
	TCHAR								szPassword[61];						///��������
};

///ȡǮ
struct MSG_GR_S_CheckMoneyWithPwd_t
{
	__int64								i64Count;							///���
	long int							UserID;								///�û�ID
	TCHAR								szMD5Pass[50];						///��������
};

///��Ǯ
struct MSG_GR_S_CheckMoney
{
	__int64								i64Count;							///���
	long int							UserID;								///�û�ID
};

///ת��
struct MSG_GR_S_TransferMoney
{
	long int							UserID;								///�û�ID
	long int							destUserID;							///ת��˭
	__int64								i64Money;								///��ת�ʽ��
	__int64								i64ActualTransfer;					///ʵ�ʵ��ʽ��
	int                                 bTranTax;                           ///�Ƿ��˰����0����˰
	TCHAR								szMD5Pass[50];						///��������
	TCHAR								szNickName[50];						///�û��ǳ�		add by lxl 2010-11-10
	TCHAR								szDestNickName[50];					///��ת���û��ǳ�
	bool								bUseDestID;
    UINT                                uHandleCode;        // PengJiLin, 2010-8-23, ���������Ƿ��ǡ�ȫת������
	MSG_GR_S_TransferMoney()
	{
		ZeroMemory(this, sizeof(MSG_GR_S_TransferMoney));
	}
};
///�޸�����
struct MSG_GR_S_ChangePasswd_t
{
	long int							UserID;								///�û�ID
	TCHAR								szMD5PassOld[50];					///���������
	TCHAR								szMD5PassNew[50];					///����������
	MSG_GR_S_ChangePasswd_t()
	{
		ZeroMemory(this, sizeof(MSG_GR_S_ChangePasswd_t));
	}
};

// PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ�
struct MSG_GR_S_GetNickNameOnID_t
{
    long int    iUserID;        // �û�ID
    TCHAR       szNickName[61]; // �û��ǳ�
    MSG_GR_S_GetNickNameOnID_t()
    {
        ZeroMemory(this, sizeof(MSG_GR_S_GetNickNameOnID_t));
    }
};

///�û�ת�ʼ�¼
struct MSG_GR_S_TransferRecord_t
{
	long int							dwUserID;							///�û� ID
};
///�û�ת�ʼ�¼���
struct MSG_GR_R_TransferRecord_t
{
	int								nSeriNo;							///< ���
	UINT							dwUserID;							///< �û� ID
	UINT							destUserID;							///< ת��˭
	__int64							i64Money;							///< ��ת�ʽ��
	__int64							i64ActualTransfer;					///< ʵ�ʵ��ʽ��
	COleDateTime					oleDateTime;							///< ʱ��
	MSG_GR_R_TransferRecord_t()
	{
		ZeroMemory(this, sizeof(MSG_GR_R_TransferRecord_t));
	}
};
///��Ǯ
struct MSG_GR_S_Pay
{
	int									uCount;								///���
	long int							UserID;								///�û�ID
	int									iPayID;								///��ǮID��Ϊʲô��Ǯ��
};
///�û�����
struct MSG_GR_R_UserCut
{
	long int							dwUserID;							///�û� ID
	BYTE								bDeskNO;							///��Ϸ����
	BYTE								bDeskStation;						///λ�ú���
};

///�û����»�������
struct MSG_GR_R_UserSit
{
	LONG								dwUserID;							///�û� ID
	BYTE								bLock;								///�Ƿ�����
	BYTE								bDeskIndex;							///��������
	BYTE								bDeskStation;						///����λ��
	BYTE								bUserState;							///�û�״̬
	BYTE								bIsDeskOwner;						///̨���뿪
};

///����ṹ 
struct MSG_GR_RS_NormalTalk
{
	COLORREF							crColor;							///������ɫ
	SHORT								iLength;							///��Ϣ����
	LONG								dwSendID;							///�û� ID
	LONG								dwTargetID;							///Ŀ�� ID
	int									nDefaultIndex;						///=0����������ݣ�>0��ѡ�������
	CHAR								szMessage[MAX_TALK_LEN+1];			///��������
};

// PengJiLin, 2010-9-10, ���˿������߿� �ṹ
struct MSG_GR_RS_KickProp
{
    long int    dwUserID;
    int         iPropID;
    int         iTotalTime;     // ��ʱ��s
};

///�û�ͬ��ṹ
struct MSG_GR_R_UserAgree
{
	BYTE								bDeskNO;							///��Ϸ����
	BYTE								bDeskStation;						///λ�ú���
	BYTE								bAgreeGame;							///ͬ���־
};
///�û�����
struct MSG_GR_R_InstantUpdate
{
	long int							dwUserID;							///�û� ID
	int									dwPoint;							///�û�����
	int									dwMoney;							///�û����
};
///�û�����ֵ
struct MSG_GR_R_UserPoint
{
	long int							dwUserID;							///�û� ID
	__int64									dwPoint;							///�û�����ֵ
	__int64									dwMoney;							///�û����
	__int64									dwSend;								///����
	BYTE								bWinCount;							///ʤ��
	BYTE								bLostCount;							///���
	BYTE								bMidCount;							///ƽ��
	BYTE								bCutCount;							///�Ӿ�
};

///�û�������Ϣ
struct MSG_GR_ContestChange
{
	long int					dwUserID;							///�û� ID	
	int							iContestCount;						///��������
	__int64						i64ContestScore;					///��������
	int							iRankNum;							///��������
};

///�û����������Ϣ
struct MSG_GR_AvatarChange
{
	long int						dwUserID;						///�û� ID	
	UINT							nNewLogoID;						///�µ�ͷ��ID
};

///�޸��û�����ֵ
struct MSG_GR_S_RefalshMoney
{
	long int							dwUserID;							///�û� ID
	__int64								i64Money;							///�û����
};

///�򿪾����
struct MSG_GR_S_OpenPoint
{
	long int							dwUserID;							///�û� ID
	int									dwPoint;							///����
};

///ȡ�����
struct MSG_GR_S_GetPoint
{
	long int							dwUserID;							///�û� ID
	int									dwPoint;							///����
};

///�޸��û����ӱ����趨
struct MSG_GR_S_SetDeskBasePoint
{
	long int							dwUserID;							///�û�ID
	int									iDeskBasePoint;						///�û��趨�����ӱ���
	BYTE								bDeskOwner;							///�Ƿ�̨��
};
///�������ݰ�
struct MSG_GR_S_UserSet
{
	UserRuleStruct						m_Rule;								///�û�����
};

///����������ݰ�
struct MSG_GR_SR_InviteUser
{
	long int							dwUserID;							///�û� ID
	long int							dwTargetID;							///Ŀ���û� ID
};

///�����û����ݰ�
struct MSG_GR_SR_MatchUser
{
	long int							dwUserID;							///�û� ID
	char								szUserName[61];						///�û�����
	BYTE								bDeskIndex;							///���Ӻ���
	BYTE								bDeskStation;						///����λ��
};

///ȡ���������ݰ�
struct MSG_GR_SR_MatchDelete
{
	long int							dwUserID;							///�û� ID
	BYTE								bDeskIndex;							///���Ӻ���
	BYTE								bDeskStation;						///����λ��
};

///������������ݰ�
struct MSG_GR_S_CutUser
{
	long int							dwUserID;							///����Ա�û� ID
	long int							dwTargetID;							///Ŀ���û� ID
};

///����GM
struct MSG_GR_Call_GM
{
	BYTE								bCallReason;						///��������
	UINT								uLength;							///��Ϣ����
	char								szMsg[201];							///��������
};

///�������ݰ�
struct MSG_GR_SR_Warning
{
	long int							dwTargetID;							///Ŀ���û�
	UINT								uLength;							///��Ϣ����
	char								szWarnMsg[201];						///������Ϣ
};

///�û�Ȩ���������ݰ�
struct MSG_GR_SR_GamePowerSet
{
	BYTE								bShowActive;						///�Ƿ���ʾ
	BYTE								bBindKind;							///������
	UINT								uLockTime;							///����ʱ��
	long int							dwUserID;							///�û� ID
	long int							dwGamePowerID;						///��ϷȨ��
	long int							dwMasterPowerID;					///����Ȩ��
};


///����ֵ
struct MSG_GR_Charm_Struct
{
	long int							dwUserID;							///���ID
	int									iCharmValue;						///�����䶯ֵ
};

///�������
struct MSG_GR_Queue
{
	int									iCount;								///��ǰ�������ж������Ŷ�
	int									iReserver;							///����
};


/********************************************************************************************/

///��Ϸ��Ϣ
struct MSG_GM_S_ClientInfo
{
	BYTE								bEnableWatch;						///�����Թ�
};

///��Ϸ��Ϣ
struct MSG_GM_S_GameInfo
{
	BYTE								bGameStation;						///��Ϸ״̬
	BYTE								bWatchOther;						///�����Թ�
	BYTE								bWaitTime;							///�ȴ�ʱ��
	BYTE								bReserve;							///�����ֶ�
	char								szMessage[1000];					///ϵͳ��Ϣ
};

///�Թ�����
struct MSG_GM_WatchSet
{
	long int							dwUserID;							///���ö���
};

///����
struct PropStruct
{
	BYTE							sendDeskStation;				///����λ��
	BYTE							recvDeskStation;				///����λ��
	int								iValue;							///����id
};

///�������
const int DB_BUFFER_MSG_MAX_LEN = 255;

///������Ϣ
struct tagIM_MSG
{
	int								iMessageID;							///��ϢID
	long int						dwSendUserID;						///������ID
	long int						dwRecvUserID;						///������ID

	DBTIMESTAMP						tmSendTime;							///ʱ���
	int								iDataLen;							///���ݳ���
	BYTE							Data[DB_BUFFER_MSG_MAX_LEN];		///����
};
/********************************************************************************************/

///��Ϣ���ݰ�
struct MSG_GA_S_Message
{
	BYTE								bFontSize;							///�����С
	BYTE								bCloseFace;							///�رս���
	BYTE								bShowStation;						///��ʾλ��
	CHAR								szMessage[1000];					///��Ϣ����
};

// PengJiLin, 2010-9-14, ʹ�����˿����˵Ľ��
struct MSG_GA_NEW_KICKUSERPROP
{
    long int    dwUserID;
    long int    dwDestID;
    BYTE        byKickOther;        // 1=�Լ��߱���
    BYTE        byKickResult;
};



/******************************************************************************************/
//���������

//��ϷǮ���ṹ
struct bank_game_wallet
{
	int	_game_id;
	char _game_name[30];
	__int64	_money;
};

//��ͨ������ȡǮ
struct bank_op_normal
{
	int _operate_type;
	int	_user_id;
	int	_game_id;
	TCHAR _szMD5Pass[50];  ///��������
	__int64	_money;
};

//ת��
struct bank_op_transfer
{
	int	_user_id1;
	int	_user_id2;
	int _recode_id;
	__int64	_money;
	char _user_name1[60];
	char _user_name2[60];
};

//�浥,��ֵ
struct bank_op_deposit
{
	int _operate_type;  //1Ϊ�����浥,2Ϊ��ֵ
	int	_user_id;
	TCHAR _szNickName[50];  ///��ת���û��ǳ�
	TCHAR _szMD5Pass[50];   ///��������
	int	_state;
	int	_csds;
	__int64	_money;
	char _deposit_id[16];
	char _deposit_password[16];
	char _time[32];
};

//�浥��ѯ�ṹ
struct bank_op_deposit_node
{
	int	_serial_no;	    ///< ���
	char _deposit_id[16];
	char _deposit_password[16];
	__int64	_money;
	COleDateTime _create_time;
	int	_csds;
	int	_user_id;
	int	_state;
	int	_used_user_id;
};


//��Ϸ������ϢATT
struct GameNoticeMessage
{
	BYTE			bDeskIndex;			//���Ӻ�
	BYTE			bAwardCardShape;	//���ͽ���
	TCHAR			szMessage[255];		//δ������Ϸ �ڷ��� �ұ߿���ʾ��������
	GameNoticeMessage()
	{
		::memset(this,0,sizeof(GameNoticeMessage));
	}
};

/*****************************************************************************************/

/********************************************************************************************/

///	ͨ�ű�ʶ���� 

/********************************************************************************************/

///��������ʶ
#define	MDM_GR_LOGON					100									///������½
#define MDM_GR_USER_LIST				101									///�û��б�
#define MDM_GR_USER_ACTION				102									///�û�����
#define MDM_GR_ROOM						103									///������Ϣ
#define MDM_GR_MATCH_INFO				104									///������Ϣ
#define MDM_GR_MANAGE					105									///������Ϣ
#define MDM_GR_MESSAGE					106									///��Ϣ��Ϣ
#define MDM_GR_MONEY					107									///���ˣ���Ǯ
#define MDM_GR_POINT					108									///�������

///#define MDM_GR_BROADCAST_MESSAGE		109									/// �㲥��Ϣ
///#define MDM_GR_UPDATE_HOLD_ITEM_DATA	110									/// ������Ʒ���ݣ�
///#define MDM_GR_GET_ITEM_COUNT			111									/// ��ȡ��������
#define MDM_GR_FRIEND_MANAGE			113									///������Ϣ
#define MDM_GR_QUEUE_MSG				114									///�Ŷ���Ϣ
#define MDM_GR_DESK						115
#define MDM_GR_NETSIGNAL                116                                 ///�����ź���Ϣ	

// PengJiLin, 2010-10-14, �ɷ��䣨��ʼ�㣩������Ϸ�ˣ������յ㣩����Ϣ
#define MDM_GR_ROOM2GAME                120
#define ASS_GR_R2G_USEPROP              1       // PengJiLin, 2010-10-14, ��Ϸ�˵��ߵļ����ù���


/********************************************************************************************/

///��Ϸ����ʶ
#define MDM_GM_GAME_FRAME				150									///�����Ϣ
#define MDM_GM_MESSAGE					151									///��Ϣ��Ϣ
#define MDM_GM_GAME_NOTIFY				180									///��Ϸ��Ϣ
#define ASS_GM_MESSAGE_EX				180									///��Ϸ��Ϣ������ϷATT
/********************************************************************************************/

///������½
#define ASS_GR_LOGON_BY_ID				5									///ͨ���û� ID ��½
#define ASS_GR_LOGON_SUCCESS			2									///��½�ɹ�
#define ASS_GR_LOGON_ERROR				3									///��½ʧ��
#define ASS_GR_SEND_FINISH				4									///��½���
#define ASS_GR_IS_VIPROOM               16                                   ///<�Ƿ�VIP����
#define ASS_GR_VIP_PW                   17                                   ///<VIP������Ҫ����
#define ASS_GR_VIP_NO_PW                18                                   ///< VIP���䲻��Ҫ����(��һ�����벻��Ҫ����)
#define ASS_GR_NO_VIP                   19                                   ///< ����VIP����
#define ASS_GR_VIPROOM_PW               20                                   ///< VIP��������
#define ASS_GR_VIPROOM_PW_RIGHT         21                                   ///< VIP����������ȷ

///�û��б�
#define ASS_GR_ONLINE_USER				1									///�����û�
#define ASS_GR_NETCUT_USER				2									///�����û�
#define ASS_GR_DESK_STATION				3									///����״̬

///�û�����
#define ASS_GR_USER_UP					1									///�û�����
#define ASS_GR_USER_SIT					2									///�û�����
#define ASS_GR_WATCH_UP					3									///�Թ�����
#define ASS_GR_WATCH_SIT				4									///�Թ�����
#define ASS_GR_USER_COME				5									///�û�����
#define ASS_GR_USER_LEFT				6									///�û��뿪
#define ASS_GR_USER_CUT					7									///�û�����
#define ASS_GR_SIT_ERROR				8									///���´���
#define ASS_GR_SET_TABLE_BASEPOINT		9									///�ı����ӱ���
#define ASS_GR_USER_HIT_BEGIN			10									///�û�ͬ�⿪ʼ
#define ASS_GR_JOIN_QUEUE				11									///<�����Ŷӻ�
#define ASS_GR_QUIT_QUEUE				12									///<�˳��Ŷӻ�
#define ASS_GR_QUEUE_USER_SIT			13									///<�Ŷ��û�����
#define ASS_GR_CONTEST_GAMEOVER			14									///<�˳��Ŷӻ�
#define ASS_GR_CONTEST_KICK				15									///<�Ŷ��û�����
#define ASS_GR_LEASE_TIMEOVER			16									///<�Ŷ��û�����


///������Ϣ
#define ASS_GR_NORMAL_TALK				1									///��ͨ����
#define ASS_GR_HIGH_TALK				2									///�߼�����
#define ASS_GR_USER_AGREE				3									///�û�ͬ��
#define ASS_GR_GAME_BEGIN				4									///��Ϸ��ʼ
#define ASS_GR_GAME_END					5									///��Ϸ����
#define ASS_GR_USER_POINT				6									///�û�����ֵ
#define ASS_GR_USER_CONTEST				17									///�û�������Ϣ
#define ASS_GR_AVATAR_LOGO_CHANGE		18									///�û����������Ϣ

#define ASS_GR_SHORT_MSG				7									///�Ķ���Ϣ
#define ASS_GR_ROOM_SET					8									///��������
#define ASS_GR_INVITEUSER				9									///�����û�
#define ASS_GR_INSTANT_UPDATE			10									///��ʱ���·������
#define ASS_GR_UPDATE_CHARM				11									///��ʱ��������
#define ASS_GR_ROOM_PASSWORD_SET		12									///��������
#define ASS_GR_ROOM_QUEUE_READY			13									///�Ŷӻ�׼��
#define ASS_GR_GET_NICKNAME_ONID        14          // ����ID��ȡ�ǳƣ�PengJiLin, 2010-8-23
#define ASS_GR_OWNER_T_ONE_LEFT_ROOM    15          // ����������뿪����, PengJiLin, 2010-8-26
#define ASS_GR_GET_NICKNAME_ONID_INGAME 16          // ����ID��ȡ�ǳ� ��Ϸ�� lxl 2010-11-17
///������Ϣ
#define ASS_GR_MATCH_USER				1									///�����û�
#define ASS_GR_MATCH_DELETE				2									///ȡ������

///������Ϣ
#define ASS_GR_CUT_USER					1									///���û�����
#define ASS_GR_WARN_MSG					2									///������Ϣ
#define ASS_GR_POWER_SET				3									///Ȩ������
#define	ASS_GR_FORBID_USER				4									///��ֹ�û��ʺ�
#define	ASS_GR_FORBID_USER_IP			5									///��ֹ�û�IP
#define ASS_GR_LIMIT_OTHER_ROOM_TALK	6									///��ֹ�û���������
#define ASS_GR_CALL_GM					7									///����GM
#define ASS_GR_CALL_GM_SUCCESS			8									///����GM�ɹ�
#define ASS_GR_CALL_GM_FAIL				9									///����GMʧ��
#define ASS_GR_GAME_POWER_SET_SUCCESS	10									///������ϷȨ�޳ɹ�
#define ASS_GR_GAME_POWER_SET_FAIL		11									///������ϷȨ��ʧ��
#define ASS_GR_GAME_POWER_SET_RETURN	12									///������ϷȨ�޷���
#define ASS_GR_GAME_POWER_SET_RELEASE	13									///�����ϷȨ�޳ɹ�����
#define ASS_GR_OPT_USER					14									///��Ӻ���	

#define ASS_GR_ALONE_DESK				15									///����20081212 Fred Huang
#define ASS_GR_DEALONE_DESK				16									///�������20081212 Fred Huang

///��Ϣ��Ϣ
#define ASS_GR_SYSTEM_MESSAGE			1									///ϵͳ��Ϣ

///Ǯ����
#define ASS_GR_OPEN_WALLET				1									///�쿴���
#define ASS_GR_CHECK_OUT				2									///ȡ�����
#define ASS_GR_CHECK_IN 				3									///������
#define ASS_GR_REFLASH_MONEY			4									///ͬ����������
#define ASS_GR_PAY_MONEY				5									///��Ǯ
#define ASS_GR_TRANSFER_MONEY			6									///ת��
#define ASS_GR_SENDMONEY_TIMES          7                                   ///������Ǯ
#define ASS_GR_OPEN_WALLET_INGAME		8									///��Ϸ�д�ƽ̨�쿴���
#define ASS_GR_CHECK_OUT_INGAME			9									///��Ϸ��ȡ�����
#define ASS_GR_TRANSFER_RECORD			10									///< ת����ʷ��¼
#define ASS_GR_CHECK_IN_INGAME 			11									///< ��Ϸ�д�����
#define ASS_GR_TRANSFER_MONEY_INGAME	12									///< ��Ϸ��ת��
#define ASS_GR_TRANSFER_RECORD_INGAME	13									///< ��Ϸ��ת����ʷ��¼
#define ASS_GR_CHANGE_PASSWD			14									///< �޸�����
#define ASS_GR_CHANGE_PASSWD_INGAME		15									///< ��Ϸ���޸�����

///wushuqun 2009.6.5
///��ս��������¼��Ϣ
#define ASS_GR_BATTLEROOM_RESULT       8

///���ڻ�ս���ʱ����
#define DTR_GR_BATTLEROOM_TIMEOUT_ERROR   21

///
// PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ� ���
#define ASS_GR_GETNICKNAME_SUCCESS      0           // �����ɹ�
#define ASS_GR_GETNICKNAME_NOTEXIST     1           // ID ������
#define ASS_GR_GETNICKNAME_ISNULL       2           // �ǳ�Ϊ��

///Ǯ���˽��
#define ASS_GR_OPEN_WALLET_ERROR		1									///��Ǯ�����
#define ASS_GR_OPEN_WALLET_SUC			2									///��Ǯ��ɹ�
#define ASS_GR_OPEN_WALLET_ERROR_PSW	3									///��Ǯ�����
///���䣭��Ǯ��
#define ASS_GR_CHECKIN_ERROR			1									///��Ǯʧ��
#define ASS_GR_CHECKIN_SUC				2									///��Ǯ�ɹ�
#define ASS_GR_CHECKIN_ERROR_PLAYING	3								    ///��Ϸ�У����ܴ��
#define ASS_GR_CHECKIN_ZHUANGJIA		4									///��������Ϸ��ׯ�Ҳ��ܴ�� add by lxl 2010-11-5
#define ASS_GR_CHECKOUT_ERROR_PLAYING   5									///��Ϸ��  ����ȡ��	add by lxl 2010-12-6
///����< ��Ǯ��
#define ASS_GR_CHECKOUT_ERROR			1									///ȡǮ����
#define ASS_GR_CHECKOUT_SUC				2									///ȡǮ�ɹ�

///ת��
#define ASS_GR_TRANSFER_ERROR			1									///ת�ʴ���
#define ASS_GR_TRANSFER_SUC				2									///ת�ʳɹ�
#define ASS_GR_TRANSFER_NO_DEST			3									///ת��Ŀ�겻����
#define ASS_GR_PASSWORD_ERROR			4									///ת���������
#define ASS_GR_TRANSFER_TOTAL_LESS		5									///�����ܶ�̫С�������ʸ�
#define ASS_GR_TRANSFER_TOO_LESS		6									///����ת����Ŀ̫��
#define ASS_GR_TRANSFER_MULTIPLE		7									///����ת����Ŀ������ĳ���ı���
#define ASS_GR_TRANSFER_NOT_ENOUGH		8									///���н�������ת��
#define ASS_GR_TRANSFER_TOO_MANY_TIME   9                                   ///< ����ת�˵Ĵ���̫����
/********************************************************************************************/

///��Ϸȡ����
#define ASS_GR_OPEN_POINT				1									///�򿪾����
#define ASS_GR_OPEN_SUC					2									///�����
#define ASS_GR_OPEN_ERROR				3									///�򿪴���
#define ASS_GR_GET_POINT				4									///ȡ������
#define ASS_GR_GETP_SUC					5									///ȡ���������
#define ASS_GR_GETP_ERROR				6									///ȡ������

///�����Ϣ
#define ASS_GM_GAME_INFO				1									///��Ϸ��Ϣ
#define ASS_GM_GAME_STATION				2									///��Ϸ״̬
#define ASS_GM_FORCE_QUIT				3									///ǿ���˳�
#define ASS_GM_NORMAL_TALK				4									///��ͨ����
#define ASS_GM_HIGH_TALK				5									///�߼�����
#define ASS_GM_WATCH_SET				6									///�Թ�����				

// PengJiLin, 2010-9-10, ���˿������߿���Ϣ
#define ASS_GM_USE_KICK_PROP            7       // ʹ�����˿�
#define ASS_GM_USE_ANTI_KICK_PROP       8       // ʹ�÷��߿�

// PengJiLin, 2010-9-10, ���˿������߿�������Ϣ
#define ERR_GR_KICK_PROP_NULL           1       // û�е���
#define ERR_GR_KICK_SET_TIME_FAIL       2       // ����ʱ�����
#define ERR_GR_KICK_SET_TIME_SUCCESS    3       // ����ʱ��ɹ�

// PengJiLin, 2010-9-13, ʹ�����˿����˴�����Ϣ
#define ERR_GR_NEW_KICKUSER_SUCCESS             1       // �ɹ�
#define ERR_GR_NEW_KICKUSER_NOTIME              2       // �Լ��ĵ����ѹ��ڣ�����û�е���
#define ERR_GR_NEW_KICKUSER_HAD_ANTI            3       // �Է��з��߿�ʱ��
#define ERR_GR_NEW_KICKUSER_HAD_VIP             4       // �Է���VIPʱ��
#define ERR_GR_NEW_KICKUSER_PLAYING             5       // ��Ϸ�в�����

///��Ϣ��Ϣ
#define ASS_GM_SYSTEM_MESSAGE			1									///ϵͳ��Ϣ

#define ASS_GM_SYSTEM_MESSAGE_EX		2									///ϵͳ��Ϣ������Ϸר�ã�ATT��

///��Ϸ��Ϣ
#define ASS_GM_AGREE_GAME				1									///ͬ����Ϸ
#define ASS_PROP						2									///ͬ����Ϸ

///������Ϣ
#define ASS_GR_QUEUE_ERROR				0									///����
#define ASS_GR_ADD_QUEUE				1									///�����Ŷ�
#define ASS_GR_ADD_QUEUE_RESULT			2									///�Ŷӽ��
#define ASS_GR_PLAN_SIT					3									///��������
#define ASS_GR_AGREE_PLAN_SIT			4									///�����������
#define ASS_GR_REFUSE_PLAN_SIT			5									///�����ӷ�������

/********************************************************************************************/

///�������
#define ERR_GR_ERROR_UNKNOW				0									///δ֪����
#define ERR_GR_LOGON_SUCCESS			1									///��½�ɹ�
#define ERR_GR_USER_NO_FIND				2									///�û�������
#define ERR_GR_USER_PASS_ERROR			3									///�û��������
#define ERR_GR_USER_VALIDATA			4									///�û��ʺŽ���
#define ERR_GR_USER_IP_LIMITED			5									///��½ IP ��ֹ
#define ERR_GR_IP_NO_ORDER				6									///����ָ����ַ
#define ERR_GR_ONLY_MEMBER				7									///��Ա��Ϸ����
#define ERR_GR_IN_OTHER_ROOM			8									///������������
#define ERR_GR_ACCOUNTS_IN_USE			9									///�ʺ�����ʹ��
#define ERR_GR_PEOPLE_FULL				10									///�����Ѿ���
#define ERR_GR_LIST_PART				11									///�����û��б�
#define ERR_GR_LIST_FINISH				12									///ȫ���û��б�
#define ERR_GR_STOP_LOGON				13									///��ͣ��½����

#define ERR_GR_CONTEST_NOSIGNUP			14
#define ERR_GR_CONTEST_TIMEOUT			15
#define ERR_GR_CONTEST_KICK				16
#define ERR_GR_CONTEST_NOTSTRAT			17
#define ERR_GR_CONTEST_OVER				18
#define ERR_GR_CONTEST_BEGUN			19

#define ERR_GR_MATCH_LOGON				160									///��Ϸ����
#define ERR_GR_TIME_OVER				161									///ʱ�䵽��

///wushuqun 2009.6.5
///���ڻ�ս���ʱ����
#define ERR_GR_BATTLEROOM_OUTTIME       162

///�û����´�����
#define ERR_GR_SIT_SUCCESS				50									///�ɹ�����
#define ERR_GR_BEGIN_GAME				51									///��Ϸ�Ѿ���ʼ
#define ERR_GR_ALREAD_USER				52									///�Ѿ����˴���
#define ERR_GR_PASS_ERROR				53									///�������
#define ERR_GR_IP_SAME					54									///IP ��ͬ
#define ERR_GR_CUT_HIGH					55									///������̫��
#define ERR_GR_POINT_LOW				56									///����ֵ̫��
#define ERR_GR_POINT_HIGH				57									///����ֵ̫��
#define ERR_GR_NO_FRIEND				58									///���ܻ�ӭ
#define ERR_GR_POINT_LIMIT				59									///����ֵ����
#define ERR_GR_CAN_NOT_LEFT				60									///�����뿪
#define ERR_GR_NOT_FIX_STATION			61									///������λ��
#define ERR_GR_MATCH_FINISH				62									///��������
#define ERR_GR_MONEY_LIMIT				63									///���̫��
#define ERR_GR_MATCH_WAIT				64									///�������Ŷ���ʾ
#define ERR_GR_IP_SAME_3				65									///IPǰ3 ��ͬ
#define ERR_GR_IP_SAME_4				66									///IPǰ4 ��ͬ
#define ERR_GR_UNENABLE_WATCH			67									///�������Թ�
#define ERR_GR_DESK_FULL				68									/// �ټ��� ������λ���ˣ��޷�������λ����� add by wlr 09.07.14


//����λ��
#define SK_ENEMY_USER						0								//������������
#define SK_NORMAL_USER						1								//�����������

#define SK_NORMAL_MEMBER					2								//��ͨ��Ա����
#define SK_HIGHT_MEMBER	                    3								//�߼���Ա����
#define SK_SPECIAL_MEMBER					4								//�����α�����
#define SK_FOREVER_MEMBER					5								//���û�Ա����
#define SK_SENIOR_MEMBER					6								//Ԫ���������

#define SK_FRIEND_USER						7								//������������
#define SK_MASTER_USER						8								//������������
#define SK_ME_USER							9								//�Լ���������
#define SK_ALWAYS_USER						10								//�̶���������

//��ϵ����
#define CM_NORMAL							0								//��ͨ��ϵ
#define CM_ENEMY							1								//���˹�ϵ
#define CM_FRIEND							2								//���ѹ�ϵ

//������Ϣ����
#define MDM_BANK				131

#define ASS_BANK_NORMAL			1
#define ASS_BANK_TRANSFER		2
#define ASS_BANK_DEPOSIT		3
#define ASS_BANK_WALLET			4
#define ASS_BANK_QUERY_DEPOITS	5

#define HC_BANK_OP_SUC			0			//�����ɹ�
#define HC_BANK_OP_NO_USER		1			//û���ҵ��û�
#define HC_BANK_OP_ERR_PWD		2			//��������
#define HC_BANK_OP_NOT_MONEY	4			//����
#define HC_BANK_OP_NO_SERIAL	5			//���к�������
#define HC_BANK_OP_INVDEP		8			//��ֵ������
#define HC_BANK_OP_CHECKIN_PLAYING		9	//��Ϸ�У����ܴ��
#define HC_BANK_OP_CHECKOUT_PLAYING		10	//��Ϸ�У�����ȡ��
#define HC_BANK_OP_CHECKIN_ZHUANG		11  //��������Ϸ��ׯ�Ҳ��ܴ��
#define HC_BANK_OP_CHECKMONEY_INV		5	//�Ƿ���ȡǮ


// Unity3d��ϢID ��ʼ������
#define IPC_MAIN_GAMEBASEINFO				8
#define IPC_SUB_GAMEBASEINFO				0
#define IPC_SUB_DATAOVER					1
#define IPC_SUB_REINIT						2

#define IPC_MAIN_USERINFO					10
#define IPC_SUB_USERINFOLIST				0								//�û��б�
#define IPC_SUB_USERAGREE					1								//�û�ͬ��
#define IPC_SUB_USERSITDOWN					2								//�û�����
#define IPC_SUB_USERSTANDUP					3								//�û�����
#define IPC_SUB_USEOFFLINE					4								//�û�����
#define IPC_SUB_CLICKBEGIN					5								//�û������ʼ��Ϸ
#define IPC_SUB_USERDETAILINFO				6								//�û���ϸ��Ϣ
#define IPC_SUB_UPDATEUSERINFO				7								//ˢ���û���Ϣ
#define IPC_SUB_MATCHRANK					8								//��������

#define IPC_SUB_USERSTANDUP_STATION			9								//�û�վ��λ��
#define IPC_SUB_USERSIT_STATION				10								//�û�����λ��

//���½�Ҿ�������
#define IPC_MAIN_UPDATE						11								//���½����ID
#define IPC_SUB_USERPOINT					1								//���½�ҷ���
#define IPC_SUB_USEREXPERIENCE				2								//�����û�����
#define IPC_SUB_USERFASCINATION				3								//�����û�����

#define IPC_MAIN_QUEUE						13
#define IPC_SUB_JOIN						1
#define IPC_SUB_QUIT						2
#define IPC_SUB_BEGIN						3

#define IPC_MAIN_TASKAWARDLIST				14
#define IPC_SUB_TASKAWARDLIST				1
#define IPC_SUB_TASKAWARD					2

#define IPC_MAIN_TASKDAILY					15
#define IPC_SUB_TASKDAILY_LIST				1
#define IPC_SUB_TASKDIALY_UPDATE			2

#define IPC_MAIN_CONTESTINFO				16
#define IPC_SUB_CONTEST_REALLOTDESK			1
#define IPC_SUB_CONTEST_AUTO				2
#define IPC_SUB_CONTEST_FINISH_INFO			3
#define IPC_SUB_OPEN_RANK_PAGE				4
#define IPC_SUB_CONTEST_NOSCORE				5
#define IPC_SUB_CONTEST_FINISH_TIME			6

#define IPC_MAIN_BANK						17
#define	IPC_SUB_CHECK_IN_MONEY				1			//��Ǯ
#define IPC_SUB_CHECK_OUT_MONEY				2			//ȡǮ
#define IPC_SUB_CHECKIN_ERROR				3			//��Ǯʧ��
#define IPC_SUB_CHECKIN_ERROR_PLAYING		4			//��Ϸ�в��ܴ�
#define IPC_SUB_CHECKOUT_ERROR				5			//ȡ��ʧ��
#define IPC_SUB_CHECKOUT_ERROR_PASSWORD		6			//�����������

#define IPC_SYSTEMMSG						18
#define IPC_SUB_PUREMSG						1
#define IPC_SUB_SYSTEMMSG					2

#define IPC_MAIN_ACHIEVE					19
#define IPC_SUB_SET_USERINFO				1

#define IPC_MAIN_GAMEMASTER					20
#define IPC_SUB_GM_LIST						1
#define IPC_SUB_GM_TALK_TOGM				2
#define IPC_SUB_GM_TALK_FROMGM				3

//�Լ������Э��
#define IPC_MAIN_GAMEMASSAGE				180
#define IPC_FRAME_GAMEMASSAGE				150
#define IPC_SYSTEM_GAMEMESSAGE				151
#define IPC_SUB_GAMEINFO					1

//һ����Ϸǿ���˳�Ϊ��ȫ�˳�
#define ASS_ONE_FINISH						96

// Unity3d��Ϣ�ṹ
struct u3dSGameBaseInfo {
	u3dSGameBaseInfo() {
		ZeroMemory(this, sizeof(u3dSGameBaseInfo));
	}
	int					iDeskPeople;								//��������
	int					iLessExperience;							//���پ���ֵ
	int					iPower;										//����
	long				lRoomRule;									//�������
	int					iGameNameID;								//��ϷID
	BOOL				bCanTalk;									//�Ƿ��������
	UINT				uTaskAwardType;								//���佱���������к� �Դ�Ϊ�±�����
	UINT				uTaskAwardUnit;								//���佱�����ͱ�����ͨ����������
	WCHAR				szGameRoomName[64];							//��������	
	WCHAR				money[320];									
	WCHAR				virtualMoney[320];	
	int					iContestType;								//��������
	int					iDeskNO;									//���Ӻ�
};

//�û�������Ϣ
struct u3dUserBaseInfo {
	u3dUserBaseInfo() {ZeroMemory(this, sizeof(u3dUserBaseInfo));}
	~u3dUserBaseInfo(){}
	
	UINT			uiUserID;										//ID����
	int				iImageNO;										//��������ͼƬ�ļ���
	UINT			uiAge;											//����
	int				bBoy;											//0��ʾŮ
	int				StarTag;										//����
	int				BornTag;										//��Ф
	int				BloodTag;										//Ѫ��
	int				iLevel;											//�û��ȼ�
	int				iDeskNO;										//��Ϸ����
	int				iDeskStation;									//����λ��
	int				iRoomID;										//�����
	int				iUserState;										//�û�״̬
	int				iMoney;											//�û����
	int				iBank;											//�û��Ƹ�
	int				iTreasure;										//Ԫ��
	int				iFlag;											//0�Լ���1��ң�2�Թ�
	int				iWinCount;										//ʤ����Ŀ
	int				iLostCount;										//����Ŀ
	int				iDrawCount;										//�;���Ŀ
	int				iPoint;											//���� 
	int				nMedalID;										
	int				bUsingBank;
	WCHAR			szNickName[64];									//�ǳ�
	WCHAR			szCountry[32];									//����
	WCHAR			szProvince[64];									//ʡ��
	WCHAR			szCity[64];										//����
	WCHAR			szGameSign[64];									//��ע
	__int64			dwBirthday;										//����
};

//�û�ͬ����Ϣ�ṹ
struct u3dUserAgree {
	int		bDeskNO;						//��Ϸ����
	int		bDeskStation;					//λ�ú���
	int		bAgreeGame;						//ͬ���־
	int		bNotUse;						//�������ڶ���ռλ��
};

/********************************************************************************************/

#endif
