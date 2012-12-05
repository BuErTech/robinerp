#pragma once

#ifndef GAME_ROOM_LOGON_DT_HEAD_FILE
#define GAME_ROOM_LOGON_DT_HEAD_FILE

#include "../common/ComStruct.h"
#include "../common/GameRoomMessage.h"
//�������㴦��ͷ�ļ�
//#include "itemdatemessage.h"
/********************************************************************************************/

//	���ݿ�ͨ�����ݰ����� 

/********************************************************************************************/

//��½��������½��Ϣ
struct DL_GR_I_LogonByIDStruct
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	MSG_GR_S_RoomLogon					LogonInfo;							//��½��Ϣ
	ULONG								uAccessIP;							//��½ IP
};

//wushuqun 2009.5.6
//������ߺ�ı�־
struct DL_GR_I_ClearOnlineFlag
{
	DataBaseLineHead					DataBaseHead;
	long								lUserID;
};


//�û���������дW��¼ 2009-4-28
struct DL_GR_I_UserRecome
{
	DataBaseLineHead		DataBaseHead;
	long int				lUserID;
};

// PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ�
struct DL_GR_I_GetNickNameOnID
{
    DataBaseLineHead					DataBaseHead;
    MSG_GR_S_GetNickNameOnID_t          stGetNickNameOnID;
};



//��½��������½��Ϣ
struct DL_GR_I_RegisterLogon
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
	char								szAccessIP[16];						//�û� IP
};

/// �����������������û����ݿ�����ṹ
struct UpdateUserInfo_t
{
	long int							dwUserID;							//�û� ID
	long int							dwOnLineTimeCount;					//����ʱ��
	__int64								dwChangePoint;						//���ķ���
	__int64								dwChangeMoney;						//���Ľ��
	__int64								dwChangeTaxCom;						//����˰��
	UINT								uWinCount;							//ʤ����Ŀ
	UINT								uLostCount;							//�����Ŀ
	UINT								uCutCount;							//ǿ����Ŀ
	UINT								uMidCount;							//�;���Ŀ
	//UINT								uContestRank;						//��������
	UpdateUserInfo_t()
	{
		ZeroMemory(this, sizeof(UpdateUserInfo_t));
	}
};



/// ���������û�������Ϣ�ýṹ
/// Ϊ�˸Ľ����ݿ�洢���̵���Ч�ʣ����ٲ���Ҫ�ĵ��á����ø���Ч�İ취������û����ݵļ�¼
/// ���ݿɱ䣬�����Ƕ����Ľṹ�����ݲ���ָ�����ʽ������
struct DL_GR_I_AllUserData_t
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	int									nUserCount;							///< �м�����ҵ�����
	int									nPlayTime;			///< ��Ϸʱ��
	int									iRoundPlayTime;		///< ���ֵ�ʱ��
	UpdateUserInfo_t					*pUpdateUserInfo;					///< �������ָ�룬�ɵ����߱�֤���ݵĺϷ���
	DL_GR_I_AllUserData_t()
	{
		ZeroMemory(this, sizeof(DL_GR_I_AllUserData_t));
	}
};


struct DL_GR_I_UserContestData
{
	DataBaseLineHead					DataBaseHead;
	int iUserID;
	int iContestID;
	int ChangeScore;
	int ChangePoint;
	int iDeskNo;
	int iSitNo;
	int iRoomID;
};

struct DL_GR_I_UserContestRank
{
	DataBaseLineHead					DataBaseHead;
	int* pUserIDs;						//�û�ID��
	int iIDNums;						//ID��������
};

struct DL_GR_O_UserContestRank
{
	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
	MSG_GR_ContestChange				_data;
};

//�����û�������Ϣ
struct DL_GR_I_UserLeft
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
	long int							uMaster;							//����Ա�ȼ�
	long int							dwAccessLogID;						//��¼ ID
	long int							dwPlayTimeCount;					//��Ϸʱ��
	long int							dwOnLineTimeCount;					//����ʱ��
	int									dwScrPoint;							//ԭ������
	int									dwChangePoint;						//���ķ���
	int									dwChangeMoney;						//���Ľ��
	int									dwChangeTaxCom;						//����˰��
	int									dwResultPoint;						//�������
	UINT								uWinCount;							//ʤ����Ŀ
	UINT								uLostCount;							//�����Ŀ
	UINT								uCutCount;							//ǿ����Ŀ
	UINT								uMidCount;							//�;���Ŀ
	ULONG								uAccessIP;							//���ʵ�ַ
};

//���·�����������Ϣ
struct DL_GR_I_UpdateServerInfo
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	UINT								uNameID;							//��Ϸ ID
	UINT								uRoomID;							//���� ID
	UINT								uOnLineUserCount;					//��������
};

struct DL_GR_I_SendMonet_By_Times         //ĳ�������£����������������Ϸ��
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ  
	long int							dwUserID;							//�û� ID
	__int64                             i64SrcGameMoney;                     //ԭ���Ľ��                    ��Ҫ�ͽ���ǰ���
	__int64                             i64SrcMoneyInBank;                    //������Ľ��
	BYTE                                dwGameFinishCount;                   //�ͽ�ҵĴ���
	//int                                 dwSendGameMoney;                    //���͵Ľ��  
	//int                                 dwSendTimes;                        //�����Ѿ����͵Ĵ���
	//int                                 dwAllSendTImes;                     //�����ܹ������ʹ���	
	//int                                 dwSendFailedCause;                   //����ʧ��ԭ�� 0�ɹ� 
		                                                                    //1��������2δ�ﵽ���ͱ�׼
		                                                                                 
};
struct DL_GR_O_SendMonet_By_Times         //ĳ�������£����������������Ϸ��
{
	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
	__int64                             i64SrcGameMoney;                     //ԭ���Ľ��                    ��Ҫ�ͽ���ǰ���
	__int64                             i64SrcMoneyInBank;                    //������Ľ��
	int                                 dwSendGameMoney;                    //���͵Ľ��  
	int                                 dwSendTimes;                        //�����Ѿ����͵Ĵ���
	int                                 dwAllSendTImes;                     //�����ܹ������ʹ���	
	int                                 dwSendFailedCause;                   //����ʧ��ԭ�� 0�ɹ� 
                                                                     //1��������2δ�ﵽ���ͱ�׼
};

// PengJiLin, 2011-4-16, ����ʱ���������ͽ�ҹ���
struct DL_GR_O_SendMoney_On_TC
{
    DataBaseResultLine					ResultHead;
    int                                 dwUserID;
    int                                 dwGetMoney;             // �����Ľ����
    int                                 dwMoneyOnTimes;         // ���ö���ʱ�����͵Ľ��
    int                                 dwMoneyOnCounts;        // ���ö��پ����͵Ľ��
    int                                 dwTimesNeed;            // ��һ�β����ʱ��
    int                                 dwCountsNeed;           // ��һ�β���پ�
};


//��Ϸ��¼����
struct DL_GR_I_GameRecord
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	UINT								uRoomID;							//���� ID
	BYTE								bDeskIndex;							//��Ϸ����
	long int							dwTax;								//ÿ��ʵ��˰��
	long int							dwUserID[MAX_PEOPLE];				//�û� ID
	__int64								dwChangePoint[MAX_PEOPLE];			//�û��ı����
	__int64								dwChangeMoney[MAX_PEOPLE];			//�û��ı���
	__int64								dwScrPoint[MAX_PEOPLE];				//�û��𲽷���
	__int64								i64ScrMoney[MAX_PEOPLE];				//�û���ǰ���
	__int64								dwTaxCom[MAX_PEOPLE];				//ÿ��˰��
	long int							dwBeginTime;						//��ʼʱ��
};

//Ȩ�޸�������
struct DL_GR_I_PowerSet
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	int									dwGMID;								//����ԱID
	int							        dwUserID;							//�û� ID
	int									dwGamePower;						//��ϷȨ��
	int									dwMasterPower;						//����Ȩ��
	long int							uLockTime;							//����ʱ��
	BYTE								bBindKind;						   //������Χ
};

//GM�����û���¼
struct DL_GR_I_Warn_User_Record
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	long int							dwGMID;								//����ԱID
	long int							dwTargetID;							//Ŀ���û�
	char								szWarnMsg[201];						//������Ϣ
};

//wushuqun 2009.6.6
//��ʱ��������
struct DL_GR_I_ALone_Data
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ

	long int							dwUserID;							//�û� ID
	BYTE								bDeskIndex;							//���Ӻ���
	BYTE								bDeskStation;						//����λ��
	//����ID��
	UINT                                uRoomID;

	int                                 bDeskPeople;                        //��������,����Ϊ��ʱ��������
};
///////

//GM���˼�¼��
struct DL_GR_I_Kick_User_Record
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	char						        szTargetUserName[61];				//Ŀ���û���¼��
	long int							GMID;								//����ԱID
};
//����GM����
struct DL_GR_I_CallGM
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
	char								szName[61];							//��¼��
	UINT								uDeskNO;							//����
	UINT								uDeskStation;						//���Ӻ�
	BYTE								bCallReason;						//��������
	char								szMsg[202];							//��������
};

//�����û����� 
struct DL_GR_I_UnRegisterUser
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
	//CHAR								szMD5Pass[50];						//��������
};

//�����û����� 
struct DL_GR_I_ContestUserLeft
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	int									iUserID;							//�û� ID
	int									iContestID;
	//CHAR								szMD5Pass[50];						//��������
};

//wushuqun 2009.5.22
//����д��WLoginRecord ��¼ʱ���ṹ
struct DL_GR_I_WriteWLoginRecord
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
	UINT								uRoomID;							//���� ID
};
///////


//�򿪾���� 
struct DL_GR_I_OpenPoint
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
};

//�򿪾���н�� 
struct DL_GR_O_OpenPointResult
{
	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
	DWORD								dwPoint;							//����ֵ
};

//ȡ������
struct DL_GR_I_GetPoint
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
	DWORD								dwPoint;							//����ֵ
	DWORD								dwsrcPoint;							//ԭ������ֵ
};

//ȡ������
struct DL_GR_O_GetPointResult
{
	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
	DWORD								dwPoint;							//����ֵ
};

//��Ǯ��
struct DL_GR_O_OpenWalletResult
{
	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
	struct MSG_GR_UserBank				stUserBank;
};
///��Ǯ����
struct DL_GR_I_CheckOutMoney
{
	DataBaseLineHead					DataBaseHead;							//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
	__int64								i64Count;							//���
};
///������ȡǮ����
struct DL_GR_I_CheckOutMoneyWithPwd_t
{
	DataBaseLineHead					DataBaseHead;							//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
	__int64								i64Count;							//���
	TCHAR								szMD5Pass[50];						//��������
};
//ȡǮ���
struct DL_GR_O_CheckOutMoney
{
	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
	__int64								i64CheckOut;						//ȡ�����
	__int64								i64MoneyInBank;						//ʣ��
	__int64								i64MoneyInRoom;
};
//�޸���������
struct DL_GR_I_ChangePasswd_t
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	struct	MSG_GR_S_ChangePasswd_t		stChangePasswd;						//�޸��������ݰ�
};
//�޸�������
struct DL_GR_O_ChangePasswd_t
{
	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
	int									nResult;							//�޸���������0��ʾ�ɹ���1��ʾʧ��
};
//ת������
struct DL_GR_I_TransferMoney
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	struct	MSG_GR_S_TransferMoney		stTransfer;							//ת�����ݰ�
};
//ת�ʽ��
struct DL_GR_O_TransferMoney
{
	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
	struct	MSG_GR_S_TransferMoney		stTransfer;							//ת�����ݰ�
};
///����ת�ʼ�¼
struct DL_GR_I_TransferRecord_t
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	struct	MSG_GR_S_TransferRecord_t	stTransfer;							//ת�����ݰ�
};
///����ת�ʼ�¼���
struct DL_GR_O_TransferRecord_t
{
	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
	struct	MSG_GR_R_TransferRecord_t	transferRecord;						//ת�ʼ�¼���ݰ�
};

//��ʱ����
struct DL_GR_Update_InstantMoney
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
	int									dwChangeMoney;						//���Ľ��
	int									dwChangePoint;						//���ķ���
	BYTE								bCutValue;							//�Ƿ�Ϊ����
};
//��ʱ�����������ֵ
struct DL_GR_I_AddCharm
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	long int							dwUserID;							//�û� ID
	int									iCharmValue;								//����ֵ
};

/// ��¼���������޸�(���«2��)
struct DL_GR_RoomPWChange
{
	///< ���ݰ�ͷ
	DataBaseLineHead					DataBaseHead;
	///< �û�ID
	long int                            dwUserID;
	///< ����ID
	long int                            dwRoomID; 
};

// PengJiLin, 2010-9-10, ���˿������߿�����
struct DL_GR_I_UseKickProp
{
    DataBaseLineHead					DataBaseHead;
    long int                            dwUserID;
    int                                 iPropID;
};

// PengJiLin, 2010-9-13, ���˿����˹���
struct DL_GR_I_NewKickUserProp
{
    DataBaseLineHead					DataBaseHead;
    long int                            dwUserID;
    long int                            dwDestID;
    UINT                                dwDestIndex;        // socket
    DWORD                               dwDestHandleID;     // socket
};

///////////////////////////////////////////////2008.3.5����




//����
//struct DL_GR_I_FriendManage
//{
//	DataBaseLineHead					DataBaseHead;						// ���ݰ�ͷ
//	tagIM_Msg							Data;
//};

//���ѹ���
//struct DL_GR_O_FriendManage
//{
//	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
//	tagIM_Msg							Data;
//};

////����
//struct DL_GR_I_BufferIMMessage
//{
//	DataBaseLineHead					DataBaseHead;						// ���ݰ�ͷ
//	tagIM_Msg							Data;
//};

////����
//struct DL_GR_O_BufferIMMessage
//{
//	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
//	tagIM_Msg							Data;
//};

/********************************************************************************************/

//��½��������½��Ϣ
struct DL_GR_O_RegisterLogon
{
	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
	UINT								uRoomID;							//���� ID	
	char								szGameRoomName[61];					//��������
};

//�û���½���
struct DL_GR_O_LogonResult
{
	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
	UserInfoStruct						pUserInfoStruct;						//�û���Ϣ
	ULONG								dwLastLogonIP;						//�ϴε�½ IP
	ULONG								dwNowLogonIP;						//���ε�½ IP
	long int							dwGamePower;						//�û�Ȩ��
	long int							dwMasterPower;						//����Ȩ��
	char								szMD5Pass[50];						//MD5 ����
	BYTE								bRegOnline;							//�Ƿ�ע��
	char								nickName[61];							//�û���

	//wushuqun 2009.6.6
	//��¼����ʱ��ʱ��ȡ�����������
	int									nVirtualUser;
	int									nPresentCoinNum;  ///< ���ͽ������
};

// PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ�
struct DL_GR_O_GetNickNameOnIDResult
{
    DataBaseResultLine					ResultHead;
    long int                            iUserID;
    TCHAR                               szNickName[61];
};

//wushuqun 2009.6.5
//��ս����Ϸ��Ϣ��¼�ṹ
struct DL_GR_O_BattleRecordResult
{
	DataBaseResultLine					ResultHead;							//���ݰ�ͷ


	UINT								uRoomID;							//���� ID	,��BattleRoomID
	//��¼���
	UINT          uRecordResult;
};

// PengJiLin, 2010-9-10, ���˿������߿�����
struct DL_GR_O_UseKickProp
{
    DataBaseResultLine					ResultHead;
    long int                            dwUserID;
    int                                 iPropID;
    int                                 iTotalTime;     // ��ʱ��
};

// PengJiLin, 2010-9-13, ���˿����˹���
struct DL_GR_O_UseNewKickUserProp
{
    DataBaseResultLine					ResultHead;
    long int                            dwUserID;
    long int                            dwDestID;
    UINT                                dwDestIndex;
    DWORD                               dwDestHandleID;
};


//��½��������½��Ϣ���󷵻�
struct DL_GR_O_LogonError
{
	DataBaseResultLine					ResultHead;							//���ݰ�ͷ
	UINT								uRoomID;							//���� ID	
	char								szGameRoomName[61];					//��������
};

///
/********************************************************************************************/
//Ǯ����
#define DTK_GR_DEMAND_MONEY				32									//��Ǯ��
#define DTK_GR_CHECKOUT_MONEY			33									//ȡ��Ǯ
#define DTK_GR_CHECKIN_MONEY			34									//��Ǯ
#define DTK_GR_TRANSFER_MONEY			40									//ת��
#define DTK_GR_SENDMONEY_TIMES          41                                  //��������Ǯ  yjj add for send money
#define DTK_GR_TRANSFER_RECORD			42									//ת�ʼ�¼
#define DTK_GR_CHANGE_PASSWD			43									///< �޸�����
//����Ȩ�޼�¼
#define DTK_GR_GM_KICK_USER_RECORD      30									//����Ա���˼�¼
#define DTK_GR_GM_WARNING_USER_RECORD   31									//����Ա��������Ϣ��¼

//����Ȩ�޴���
#define DTK_GR_GAME_POWER               25									//����Ȩ���޸�
#define DTK_GR_GAME_POWER_RESULT		26									//����Ȩ���޸Ľ��
#define DTK_GR_GAME_POWER_FAIL			27									//����Ȩ���޸�ʧ��
#define DTK_GR_GAME_POWER_SUCCESS		28									//����Ȩ���޸ĳɹ�
#define DTK_GR_GAME_POWER_RELEASE		29									//�������Ȩ�����Ƴɹ�

//���䶯�������ʾ
#define DTK_GR_CALL_GM					20									//����GM����
#define DTK_GR_CALL_GM_RESULT			21									//����GM���ݿ⴦����
#define DTK_GR_CALL_GM_SUCCESS			22									//���гɹ�
#define DTK_GR_CALL_GM_FAIL				23									//����ʧ��
#define DTK_GR_CALL_GM_IN_ONE_MI		24									//�����ڣ������ڳ�������

//����в���
#define DTK_GR_OPEN_POINT				35									//�򿪾����
#define DTK_GR_GET_POINT				36									//ȡ������
#define DTK_GR_OPEN_POINT_RESULT		37									//������
#define DTK_GR_GET_POINT_RESULT			38									//ȡ�����
//#define DTK_GR_USER_OPT					39									//��� ����


#define DTK_GR_OPEN_ERROR				1									//��ʧ��
#define DTK_GR_GET_ERROR				2									//ȡ��ʧ��
#define DTK_GR_OPEN_SUC					3									//�򿪳ɹ�
#define DTK_GR_GET_SUC					4									//ȡ���ɹ�

//�����½�����ʶ
#define DTK_GR_LOGON_BY_ID				1									//ͨ�� ID ��½
#define DTK_GR_USER_LEFT				2									//�û��뿪����
#define DTK_GR_UPDATE_INFO				3									//����������Ϣ
#define DTK_GR_RECORD_GAME				4									//��¼��Ϸ��Ϣ
#define DTK_GR_CHECK_CONNECT			5									//����������
#define DTK_GR_POWER_SET				6									//�û�Ȩ������
#define DTK_GR_UNREGISTER_ONLINE		7									//ȡ��ע���û�
//#define DTK_GR_RE_WRITE_DATA			8									//��д���ݿ�

#define DTK_GR_USER_CONTEST_LEFT		8									//�û��뿪����


//wushuqun 2009.6.5
#define DTK_GR_BATTLEROOM_RES           9									//��ս����¼��Ϣ

//�����½�������
#define DTR_GR_ERROR_UNKNOW				0									//δ֪����
#define DTR_GR_DATABASE_CUT				1									//���ݿ�Ͽ�
#define DTR_GR_LOGON_SUCCESS			10									//��½�ɹ�
#define DTR_GR_USER_NO_FIND				11									//��½���ִ���
#define DTR_GR_USER_PASS_ERROR			12									//�û��������
#define DTR_GR_USER_VALIDATA			13									//�û��ʺŽ���
#define DTR_GR_USER_IP_LIMITED			14									//��½ IP ��ֹ
#define DTR_GR_IP_NO_ORDER				15									//����ָ����ַ 
#define DTR_GR_ONLY_MEMBER				16									//��Ա��Ϸ����
#define DTR_GR_IN_OTHER_ROOM			17									//������������	
#define DTR_GR_MATCH_LOGON    		    18									//��Ϸ����ע�᷿��	
#define DTR_GR_OVER_TIMER				19									//������ѵ�

#define DTR_GR_CONTEST_NOSIGNUP			23									//����δ����
#define DTR_GR_CONTEST_TIMEROUT			24									//�����˳���ʱ
#define DTR_GR_CONTEST_NOSTART			25									//������û��ʼ
#define DTR_GR_CONTEST_OVER				26									//�����Ѿ�����
#define DTR_GR_CONTEST_BEGUN			27									//�����Ѿ���ʼ


// PengJiLin, 2010-8-23, ��������ID��ȡ�ǳ� �������
#define DTR_GR_GETNICKNAME_SUCCESS      1           // �����ɹ�
#define DTR_GR_GETNICKNAME_NOTEXIST     2           // ID ������
#define DTR_GR_GETNICKNAME_ISNULL       3           // �ǳ�Ϊ��

//wushuqun 2009.6.5
//���ڻ�ս���ʱ����
#define DTR_GR_BATTLEROOM_TIMEOUT         20 

//wushuqun 2009.6.6
//����Ա��ʱ��������
#define DTR_GR_ALONEDESK_INTIME           21
#define DTR_GR_UNALONEDESK_INTIME         22   //��ʱ�������
//////////////////////////////

//Ǯ����
#define DTR_GR_OPEN_WALLET_ERROR		40									//��Ǯ�����
#define DTR_GR_OPEN_WALLET_SUC			41									//��Ǯ��ɹ�
#define DTR_GR_CHECKOUT_MONEY_SUC		42									//�ɹ�ȡ��Ǯ������
#define DTR_GR_CHECKIN_MONEY_SUC		43									//�ɹ�ȡ��Ǯ������
#define DTR_GR_OPEN_WALLET_ERROR_PSW	44									//��Ǯ������������
#define DTR_GR_TRANSFER_MONEY_SUC		45									//�ɹ�ת��
#define DTR_GR_TRANSFER_MONEY_NO_DEST	46									//ת��Ŀ�겻����
#define DTR_GR_TRANSFER_TOTAL_NOT_ENOUGH	47								//���д��̫�٣�������ת��
#define DTR_GR_TRANSFER_TOO_LESS		48									//����ת�ʶ�̫С
#define DTR_GR_TRANSFER_MULTIPLE		49									//����ת�ʶ������ĳ����������
#define DTR_GR_TRANSFER_NO_ENOUGH_MONEY	50									//���д��㱾��ת��
#define DTR_GR_TRANSFER_TOO_MANY_TIME	51									///<����ת�˵Ĵ���̫����

#define DTK_GR_FRIEND_MANAGE_SUC					53						//���Ѳ����ɹ�
#define DTR_GR_CHECKIN_MONEY_INV					54						//�Ƿ���Ǯ�쳣
#define DTR_GR_CHECKOUT_MONEY_INV					55						//�Ƿ�ȡǮ�쳣


//д��Ϸ����
#define DTK_GR_UPDATE_GAME_RESULT					10						//��Ϸ����
#define DTK_GR_UPDATE_INSTANT_MONEY					11						//��ʱ���½�ҷ���
#define DTK_GR_UPDATE_CHARM							12						//��������ֵ
#define DTK_GR_UPDATE_GAME_RESULT_ALLUSER			13						//һ���Ը���������ҵ���Ϸ����

#define DTK_GR_UPDATE_CONTEST_RESULT				14						//һ���Ը���������ҵ���Ϸ����
#define DTK_GR_GET_CONTEST_RESULT					15						//һ���Ի�ȡ������ҵ���Ϸ����

#define DTK_GR_FRIEND_MANAGE						49						//���Ѳ�����Ϣ
#define DTK_GR_BUFFER_IM_MESSAGE					50						//IM�������ݰ�


/********************************************************************************************/
#define DTK_GR_CLEAR_ONLINE_FLAG	   51									//������߱�־,2009-4-17���
#define DTK_GR_USER_RECOME			   52									//�û���������дW��¼ 2009-4-28

//wushuqun 2009.5.22
//����д��WLoginRecord ��¼ʱ��
#define DTK_GR_WRITE_WLOGINRECORD      53                       

/// ����Ϸ�д����н���Added by zxd 20091215
#define DTK_GR_DEMAND_MONEY_IN_GAME				54									///<����Ϸ�д����н���
#define DTK_GR_CHECKOUT_MONEY_INGAME			55									///<����Ϸ��ȡǮ
#define DTK_GR_CHECKIN_MONEY_INGAME				56									///<����Ϸ�д�Ǯ
#define DTK_GR_TRANSFER_MONEY_INGAME			57									///<����Ϸ��ת��
#define DTK_GR_CHANGE_PASSWD_INGAME				58									///<����Ϸ���޸�����
#define DTK_GR_TRANSFER_RECORD_INGAME			59									///<����Ϸ��ȡת�ʼ�¼

#define DTK_GR_ROOM_PW_CHANGE                   60                                  ///< ��¼���������޸�

#define DTK_GR_GETNICKNAME_ONID                 70      // PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ�

#define DTK_GR_USE_KICK_PROP                    71      // PengJiLin, 2010-9-10, ʹ�����˿�����
#define DTK_GR_USE_ANTI_KICK_PROP               72      // PengJiLin, 2010-9-10, ʹ�÷��߿�����

#define DTK_GR_USER_NEW_KICK_USER_PROP          73      // PengJiLin, 2010-9-13, ���˿����˹���

#define DTK_GR_GETNICKNAME_ONID_INGAME          74      // PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ�

// PengJiLin, 2011-4-15, ����ʱ���������ͽ��
#define DTK_GR_SEND_MONEY_ON_TIMES_COUNTS       75

// PengJiLin, 2010-9-10, ���˿������߿�ʹ�� ������
#define DTR_GR_USE_KICKPROP_SUCCESS             1       // ʹ�óɹ�
#define DTR_GR_USE_KICKPROP_NULL                2       // û�е���
#define DTR_GR_USE_KICKPROP_SET_SUCCESS         3       // ���õ��ߵ�ʱ��ɹ�
#define DTR_GR_USE_KICKPROP_SET_ERROR           4       // ���õ��ߵ�ʱ��ʧ��

// PengJiLin, 2010-9-13, ���˿����˽��
#define DTR_GR_NEW_KICKUSER_SUCCESS             1       // �ɹ�
#define DTR_GR_NEW_KICKUSER_NOTIME              2       // �Լ��ĵ����ѹ��ڣ�����û�е���
#define DTR_GR_NEW_KICKUSER_HAD_ANTI            3       // �Է��з��߿�ʱ��
#define DTR_GR_NEW_KICKUSER_HAD_VIP             4       // �Է���VIPʱ��

#endif