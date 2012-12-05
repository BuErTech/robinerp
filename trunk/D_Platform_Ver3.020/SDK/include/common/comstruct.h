#pragma once

#ifndef AFC_COM_STRUCT_HEAD_FILE
#define AFC_COM_STRUCT_HEAD_FILE

/********************************************************************************************/
#include "PropMessage.h"

///�����������
#define GRR_MEMBER_ROOM			0x00000001L							///��Ա����
#define GRR_IP_LIMITED			0x00000002L							///��ַ����
#define GRR_ENABLE_WATCH		0x00000004L							///�����Թ�
#define GRR_UNENABLE_WATCH		0x00000008L							///�����Թ�
#define GRR_AUTO_SIT_DESK		0x00000010L							///�Զ����£��������ڷ����׳�ʹ��
#define GRR_LIMIT_DESK			0x00000020L							///������λ
#define GRR_LIMIT_SAME_IP		0x00000040L							///����ͬIP
#define GRR_RECORD_GAME			0x00000080L							///��¼��Ϸ
#define GRR_STOP_TIME_CONTROL	0x00000100L							///ֹͣʱ�����
#define GRR_ALL_ON_DESK			0x00000200L							///�Ƿ����������²ſ�ʼ����
#define GRR_FORBID_ROOM_TALK	0x00000400L							///��ֹ��������
#define GRR_FORBID_GAME_TALK	0x00000800L							///��ֹ��Ϸ����
#define GRR_MATCH_REG			0x00001000L							///��������
#define GRR_EXPERCISE_ROOM		0x00002000L							///ѵ����
#define GRR_VIDEO_ROOM			0x00004000L							///��Ƶ����
///
///Kylin 20090116 �޸ķ����׳��ɿ����������������ͷ�����⣡
#define GRR_NOTFUFENG			0x00008000L		///����������
#define GRR_NOTCHEAT			0x00010000L		///������
///
///yjj   ̨�ѳ�
#define GRR_ALL_NEED_TAX		0x00020000L		///��̨�ѳ�,�����˶������һ������̨��
#define GRR_QUEUE_GAME			0x00040000L		/// �Ŷӻ�

#define GRR_NOT_COST_POINT		0x00080000L		///��ҳ����ۻ���  add by lxl 2010-10-26

#define GRR_CONTEST				0x00100000L		///��ʱ��̭������

#define GRR_GAME_U3D			0x00200000L		///U3D�[��

///�ټ��� add by wlr 20090714
#define DESK_TYPE_NORMAL		0		///
#define DESK_TYPE_BJL			1		///

#define		HUNDRED_PEOPLE_DESK_GAME_NAME   10301800					///��������Ϸ��� add by ramon
#define		SUOHA_PEOPLE_DESK_GAME_NAME		30501800					///�ٱ��������ID
#define		NIUNIU_PEOPLE_DESK_GAME_NAME	10901800					///�ٱ�ţţID
#define		ERZHANGPAI_PEOPLE_DESK_GAME_NAME	11901800				///�ٱ������ID


///end of �ټ��� add by wlr 20090714
///
///
///��Ϸ�����Ϣ�ṹ �������� DLL �����ȡ��
struct ServerDllInfoStruct
{
	///��Ϸ�����Ϣ
	UINT						uServiceVer;						///����汾
	UINT						uNameID;							///��Ϸ���� ID
	UINT						uSupportType;						///֧������
	UINT						uDeskPeople;						///��Ϸ����
	char						szGameName[61];						///��Ϸ����
	char						szGameTable[61];					///���ݿ������
	char						szDLLFileName[61];					///�ļ�����

	///������Ϣ
	char						szWriter[61];						///���������
	char						szDllNote[51];						///��ע��Ϣ
};

///�������������Ϣ�ṹ	���������������ݣ�
struct ManageInfoStruct
{
    int                         iRoomFlag[2];       // PengJiLin, 2011-4-2, ��ʹ�õ��ֶΣ�ռλ��

	///��Ϸ��Ϣ
	UINT						dwRoomRule;							///��Ϸ���ù���
	UINT						dwUserPower;						///��Ϸ����Ȩ��
	char						szLockTable[31];					///����������
	char						szIPRuleTable[31];					///IP ���Ʊ�����
	char						szNameRuleTable[31];				///UserName���Ʊ�����

    int                         iRoomInfoFlag[2];   // PengJiLin, 2011-4-2, ��ʹ�õ��ֶΣ�ռλ��

	///������Ϣ
	UINT						uComType;							///��Ϸ����
	UINT						uDeskType;							///��Ϸ�����ͣ���ҪΪ��Ӱټ����� add by wlr 20090714
	UINT						uBasePoint;							///��Ϸ������
	UINT						uLessPoint;							///��Ϸ�����ٷ���
	UINT						uMaxPoint;							///��Ϸ��������
	UINT						uMaxLimite;							///���ⶥֵ
	UINT						uRunPublish;						///���ܿ۷�
	UINT						uTax;								///˰�ձ���
	UINT						uListenPort;						///�����˿�
	UINT						uMaxPeople;							///���������Ŀ
	int							iSocketSecretKey;					///socket���ܵ���Կ

//����ר��
	int									iContestID;
	int									iLowCount;
	__int64								i64Chip;
	__int64								i64LowChip;
	__int64								i64TimeStart;
	__int64								i64TimeEnd;
	int									iTimeout;

	//���޷���ID��ʱ��
	int									iLeaseID;
	__int64								i64LeaseTimeStart;
	__int64								i64LeaseTimeEnd;


	//ÿ�ֳ���ʱ���Ƿ����ͽ��
	int									iTimeLength;
	__int64								iSendCount;

	///
	///Kylin 20090116 Vip��½����
	UINT uVipReseverCount;///Vip������½��Ŀ
	///

	bool                        bVIPRoom;                            ///< ���«�������Ƿ�VIP���䣨VIP�����������룩

	UINT						uDeskCount;							///��Ϸ����Ŀ
	char						szGameTable[31];					///��Ϣ������
	UINT						uStopLogon;							///ֹͣ��½����
	
	int						uVirtualUser;						///20081211 , Fred Huang
	int						uVirtualGameTime;					///20081211 , Fred Huang

	///wushuqun 2009.6.5
	///��ս������ID       --begin
	UINT                        uBattleRoomID;
	///��ս��������Ϣ��
	char						szBattleGameTable[31];	

	///-------------------  end

	char			szRoomPassWord[61];						//��������	Add By lxl 2010-10-08��������ܹ���
	///��������Ϣ
	UINT						uRoomID;							///��Ϸ������ ID ����
	char						szGameRoomName[61];					///����������
	
	///�������ݿ���Ϣ
	char						szSQLIP[16];						///�������ݿ��ַ
	char						szSQLName[61];						///�������ݿ��û�����
	char						szSQLPass[61];						///�������ݿ�����
	char						szSQLDataName[61];					///�������ݿ�����

	///��½���ݿ���Ϣ
	char						szLogonSQLIP[16];					///��½���ݿ��ַ
	char						szLogonSQLName[61];					///��½���ݿ��û�����
	char						szLogonSQLPass[61];					///��½���ݿ�����
	char						szLogonSQLDataName[61];				///��½���ݿ�����

	///�������ݿ���Ϣ
	char						szNativeSQLIP[61];					///�������ݿ��ַ
	char						szNativeSQLName[61];				///�������ݿ��û�����
	char						szNativeSQLPass[61];				///�������ݿ�����
	char						szNativeSQLDataName[61];			///�������ݿ�����

	ManageInfoStruct()
	{
		//iSocketSecretKey = SECRET_KEY; 
	}
};

///�ں����� �������� DLL ����������ã�
struct KernelInfoStruct
{
	///��Ϸ����
	UINT						uNameID;							///��Ϸ���� ID ����
	UINT						uDeskPeople;						///��Ϸ����


	UINT						uMinDeskPeople;						///������Ϸ֧��M-N������Ϸ

	///�ں�����
	BYTE						bMaxVer;							///�����߰汾
	BYTE						bLessVer;							///�����Ͱ汾
	BYTE						bStartSQLDataBase;					///ʹ�����ݿ�����
	BYTE						bNativeDataBase;					///ʹ�ñ������ݿ�
	BYTE						bLogonDataBase;						///ʹ�õ�½���ݿ�
	BYTE						bStartTCPSocket;					///�Ƿ���������
	UINT						uAcceptThreadCount;					///Ӧ���߳���Ŀ
	UINT						uSocketThreadCount;					///SOCKET �߳���Ŀ
};

/********************************************************************************************/

///�б���ҽ���������
#define GLK_NO_DATA				0									///û������
#define GLK_GAME_KIND			1									///��Ϸ����
#define GLK_GAME_NAME			2									///��Ϸ����
#define GLK_GAME_ROOM			3									///��Ϸ����
#define GLK_GAME_USE			4									///��Ϸʹ��
#define GLK_GAME_WEB			5									///��INI����WEB
#define GLK_GAME_ADD			12									///��ϵ����

//{add by rongqiufen 2010.7.2
#define GLK_GAME_KING_WEBGAME	6									///��bcf����WEB
#define GLK_GAME_WEBGAME		7									///��bcf����WEB
#define GLK_GAME_KING_EXEGAME	8									///��bcf����WEB
#define GLK_GAME_EXEGAME		9									///��bcf����WEB
#define GLK_GAME_SELLLIST		10									///�����б�
#define GLK_GAME_OPENINFO		11									///����˵��
//add by rongqiufen 2010.7.2}

///��Ϸ�б����ṹ
struct AssistantHead
{
	UINT						uSize;								///���ݴ�С
	UINT						bDataType;							///���ͱ�ʶ
};


///��Ϸ����de�ṹ
struct ComKindInfo///������Ϸ����AddTreeData
{
	AssistantHead				Head;
	UINT						uKindID;							///��Ϸ���� ID ����
	char						szKindName[61];						///��Ϸ��������

	// duanxiaohui 20111107 ��Ϸ��������Ŀ¼����
	UINT                        uParentKindID;                      ///����Ϸ����ID����
	// end duanxiaohui
};

///��Ϸ���ƽṹ
struct ComNameInfo
{
	AssistantHead				Head;
	UINT						uKindID;							///��Ϸ���� ID ����
	UINT						uNameID;							///��Ϸ���� ID ����
	char						szGameName[61];						///��Ϸ����
	char						szGameProcess[61];					///��Ϸ������
	UINT						m_uOnLineCount;						///��������
	///
	///Kylin 20090202 ��Ϸ������ʼ������
	ComNameInfo()
	{
		m_uOnLineCount=0;
	}
	///

};

///��Ϸ�����б�ṹ
struct ComRoomInfo
{
	AssistantHead				Head;
	UINT						uComType;							///��Ϸ����
	UINT						uKindID;							///��Ϸ���� ID ����
	UINT						uNameID;							///��Ϸ���� ID ����
	UINT						uRoomID;							///��Ϸ���� ID ����
	UINT						uPeopleCount;						///��Ϸ��������
	UINT						uDeskPeople;						///ÿ����Ϸ����
	UINT						uDeskCount;							///��Ϸ����������Ŀ
	UINT						uServicePort;						///��������˿�
	char						szServiceIP[25];					///������ IP ��ַ
	char						szGameRoomName[61];					///��Ϸ��������
	int							uVirtualUser;						///20081211 , Fred Huang
	int							uVirtualGameTime;					///20081211 , Fred Huang
	UINT						uVer;								///�汾
	UINT						dwRoomRule;							///��Ϸ�������
	bool                        bVIPRoom;                            ///< ���«�������Ƿ�VIP���䣨VIP�����������룩

	int									iContestID;
	__int64								i64TimeStart;
	__int64								i64TimeEnd;

	//���޷���ID��ʱ��
	int									iLeaseID;
	__int64								i64LeaseTimeStart;
	__int64								i64LeaseTimeEnd;
	///wushuqun 2009.6.5
	///��ս������ID       --begin
	UINT                        uBattleRoomID;
	///��ս��������Ϣ��
	char						szBattleGameTable[31];	
	bool			bHasPassword;			// ��������	Add by lxl 2010-10-08
	UINT			dwTax;					// ���䷿��	Add by wyl 11-5-16
	///
	///Kylin 20090202 ��Ϸ������ʼ������

	ComRoomInfo()
	{
		uPeopleCount=0;
		bHasPassword = false;
	}
	///
};

///��Ϸ���������ṹ
struct GameInsideInfo
{
	AssistantHead				Head;								///�б�ͷ
	UINT						uTrunID;							///ת�� ID
	char						szDisplayName[61];					///��ʾ����
};

/********************************************************************************************/

///�û�״̬����
#define USER_NO_STATE		 	0									///û��״̬�������Է���
#define USER_LOOK_STATE		 	1									///�����˴���û������
#define USER_SITTING		 	2									///������Ϸ��
#define USER_ARGEE				3									///ͬ��״̬
#define USER_WATCH_GAME		 	4									///�Թ���Ϸ
#define USER_DESK_AGREE			5									///����ͬ��
#define USER_CUT_GAME		 	20									///����״̬			����Ϸ��״̬��
#define USER_PLAY_GAME		 	21									///��Ϸ������״̬	����Ϸ��״̬��


static CString USER_STATESTRING[8] = {"", "������", "����", "ͬ��", "�Թ�", "����ͬ��", "����", "��Ϸ��"};

///�û���Ϣ�ṹ
struct UserInfoStruct
{
	long int					dwUserID;							///ID ����
	long int					dwExperience;						///����ֵ
	int							dwAccID;							///ACC ����
	int							dwPoint;							///����
	__int64						i64Money;							///���
	__int64						i64Bank;							///����
	UINT						uWinCount;							///ʤ����Ŀ
	UINT						uLostCount;							///����Ŀ
	UINT						uCutCount;							///ǿ����Ŀ
	UINT						uMidCount;							///�;���Ŀ
	char						szName[61];							///��¼��
	char						szClassName[61];					///��Ϸ����
	UINT						bLogoID;							///ͷ�� ID ����
	BYTE						bDeskNO;							///��Ϸ����
	BYTE						bDeskStation;						///����λ��
	BYTE						bUserState;							///�û�״̬
	BYTE						bMember;							///��Ա�ȼ�
	BYTE						bGameMaster;						///����ȼ�
	ULONG						dwUserIP;							///��¼IP��ַ
	bool						bBoy;								///�Ա�
	char						nickName[61];						///�û��ǳ�
	UINT						uDeskBasePoint;						///���õ����ӱ���
	int							dwFascination;						///����
	int							iVipTime;							///��Աʱ��
	int							iDoublePointTime;					///˫������ʱ��
	int							iProtectTime;						///�����ʱ�䣬����
	int							isVirtual;							///�Ƿ�����չ������ ///20081211 , Fred Huang
	UINT						dwTax;								 ///����    add by wyl    11-5-16

	///�����Ϣ�ṹ����   yjj 090319
	char                        szOccupation[61];                    ///���ְҵ
	char                        szPhoneNum[61];                      ///��ҵ绰����
	char                        szProvince[61];                      ///������ڵ�ʡ
	char                        szCity[61];                          ///������ڵ���
	char                        szZone[61];                          ///������ڵĵ���
	bool                        bHaveVideo;                          ///�Ƿ��������ͷ

	///duanxiaohui 20111111  
	char						szSignDescr[128];			         ///����ǩ��
	///end duanxiaohui

	///wushuqun 2009.6.26
	///���������Ϣ
	///0 ,��ͨ���
	///1 ,���ӱ������
	///2 ,VIP���
	///3 ,���ӱ���VIP���
	int							userType;

	///��Ϊ��չ�ֶ�,Ϊ�����Ժ��¼ӹ�����
	///�˴�Ϊ�Ժ�ƽ̨�е��¼ӹ�����Ҫ�޸��û���Ϣ�ṹʱ���������±���������Ϸ

	UINT                         userInfoEx1;    ///��չ�ֶ�1������������ʯ�������ʱ�䣬�ɻ�Զ�����
	UINT						 userInfoEx2;    ///��չ�ֶ�2������GM����֮����ʱЧ����zxd�����20100805

	int							 bTrader;		 //�����ж��ǲ�������  add by lxl 2010-12-10 


///����ר��
	__int64						i64ContestScore;
	int							iContestCount;
	CTime						timeLeft;
	int							iRankNum;		//��������
};

///�û���Ϣ�ṹ (С�����)
struct UserInfoStructX
{
	long int					dwUserID;							///ID ����
	long int					dwExperience;						///����ֵ
	int							dwAccID;							///ACC ����
	int							dwPoint;							///����
	__int64						i64Money;							///���
	__int64						i64Bank;							///����
	UINT						uWinCount;							///ʤ����Ŀ
	UINT						uLostCount;							///����Ŀ
	UINT						uCutCount;							///ǿ����Ŀ
	UINT						uMidCount;							///�;���Ŀ
	WCHAR						szName[64];							///��¼��
	WCHAR						szClassName[64];					///��Ϸ����
	UINT						bLogoID;							///ͷ�� ID ����
	int							bDeskNO;							///��Ϸ����
	int							bDeskStation;						///����λ��
	int							bUserState;							///�û�״̬
	int							bMember;							///��Ա�ȼ�
	int							bGameMaster;						///����ȼ�
	ULONG						dwUserIP;							///��¼IP��ַ
	BOOL						bBoy;								///�Ա�
	WCHAR						nickName[64];						///�û��ǳ�
	UINT						uDeskBasePoint;						///���õ����ӱ���
	int							dwFascination;						///����
	int							iVipTime;							///��Աʱ��
	int							iDoublePointTime;					///˫������ʱ��
	int							iProtectTime;						///�����ʱ�䣬����
	int							isVirtual;							///�Ƿ�����չ������ ///20081211 , Fred Huang
	UINT						dwTax;								 ///����    add by wyl    11-5-16

	///�����Ϣ�ṹ����   yjj 090319
	WCHAR                       szOccupation[64];                    ///���ְҵ
	WCHAR                       szPhoneNum[64];                      ///��ҵ绰����
	WCHAR                       szProvince[64];                      ///������ڵ�ʡ
	WCHAR                       szCity[64];                          ///������ڵ���
	WCHAR                       szZone[64];                          ///������ڵĵ���
	BOOL                        bHaveVideo;                          ///�Ƿ��������ͷ

	///duanxiaohui 20111111  
	WCHAR						szSignDescr[128];			         ///����ǩ��
	///end duanxiaohui

	///wushuqun 2009.6.26
	///���������Ϣ
	///0 ,��ͨ���
	///1 ,���ӱ������
	///2 ,VIP���
	///3 ,���ӱ���VIP���
	int							userType;

	///��Ϊ��չ�ֶ�,Ϊ�����Ժ��¼ӹ�����
	///�˴�Ϊ�Ժ�ƽ̨�е��¼ӹ�����Ҫ�޸��û���Ϣ�ṹʱ���������±���������Ϸ

	UINT                         userInfoEx1;    ///��չ�ֶ�1������������ʯ�������ʱ�䣬�ɻ�Զ�����
	UINT						 userInfoEx2;    ///��չ�ֶ�2������GM����֮����ʱЧ����zxd�����20100805

	int							 bTrader;		 //�����ж��ǲ�������  add by lxl 2010-12-10 


///����ר��
	__int64						i64ContestScore;
	int							iContestCount;
	CTime						timeLeft;
};

//�û���Ϣ�ṹ(С�����)
struct UserItemStructX
{
	BOOL							bMatchUser;							//�Ƿ����
	UINT							uSortIndex;							//��������
	UINT							uConnection;						//��ҹ�ϵ
	long int						dwLogonTime;						//��½ʱ��
	UserInfoStructX					GameUserInfo;						//�û���Ϣ
};

//С�����������Ϣ
struct XzkpNormalTalk {
	int crColor;
	int iLength;
	int	dwSendID;
	int dwTargetID;
	int nDefaultIndex;
	WCHAR szMessage[108];
};

///�û�����ṹ
struct UserRuleStruct
{
	///��������
	BYTE						bSameIP;							///��ͬ IP
	BYTE						bIPLimite;							///����IPǰ��λ�������Ϸ
	BYTE						bPass;								///��������
	BYTE						bLimitCut;							///���ƶ���
	BYTE						bLimitPoint;						///���Ʒ���
	BYTE						bCutPercent;						///������
	char						szPass[61];							///��������
	long int					dwLowPoint;							///��ͷ���
	long int					dwHighPoint;						///��߷��� 
};

struct SendUserMoneyByTimes         ///ĳ�������£����������������Ϸ��
{	
	long int							dwUserID;							///�û� ID
	int                                 dwSrcGameMoney;                     ///ԭ���Ľ��                    ��Ҫ�ͽ���ǰ���
	int                                 dwSrcMoneyInBank;                    ///������Ľ��
	int                                 dwSendGameMoney;                    ///���͵Ľ��  
	int                                 dwSendTimes;                        ///�����Ѿ����͵Ĵ���
	int                                 dwAllSendTImes;                     ///�����ܹ������ʹ���	
	int                                 dwSendFailedCause;                   ///����ʧ��ԭ�� 0�ɹ� 
                                                                             ///1��������2δ�ﵽ���ͱ�׼
};

// PengJiLin, 2011-4-15, ����ʱ���������ͽ��
struct SendMoneyOnTimesAndCounts
{
    int                                 dwUserID;
    int                                 dwGetMoney;             // �����Ľ����
    int                                 dwMoneyOnTimes;         // ���ö���ʱ�����͵Ľ��
    int                                 dwMoneyOnCounts;        // ���ö��پ����͵Ľ��
    int                                 dwTimesNeed;            // ��һ�β����ʱ��
    int                                 dwCountsNeed;           // ��һ�β���پ�
};

///wushuqun 2009.6.5
///��ս�����¼���
struct BattleRoomRecordRes
{
	///��ս����ID��
	///ע������ķ���ID ����TGameRoomInfo �е�RoomID,������BattleRoomID
	///��Ϊ��BattleRoomID=RoomIDʱ���ܻ��г�ͻ

	UINT          uRoomID;   

	///��¼���
	UINT          uRecordResult;
};
/********************************************************************************************/


struct TLoginGameInfo
{
	int					iKindID;
	int					iGameID;
	int					iLoginCount;
	COleDateTime		oelLastTime;
};


#endif
