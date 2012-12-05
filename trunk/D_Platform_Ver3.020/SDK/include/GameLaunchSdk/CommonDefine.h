///////////////////////////////////////////////////////////////////////////////
// ��������ͷ�ļ�
// �ѶദҪ�õ��Ķ���ŵ�����ļ��������Ա���ѭ������ͷ�ļ������

#ifndef _COMMON_DEFINE_H
#define _COMMON_DEFINE_H

//�ȼ��жϺ���
extern EXT_CLASS TCHAR * GetGameOrderNameInExe(__int64 dwPoint);
// ���������Ϣ���ļ�
extern EXT_CLASS void DebugPrintf(const char *p, ...);
extern EXT_CLASS void DebugPrintH(const void *buf, int nSize);
/// ��ȡָ����ֵ���ַ���
/// ������ͬʱ֧��UNICODE��ANSI
/// @param[out] str �ַ������飬���ɵ����߷���ռ�
/// @param[in] nNum ����ʾ��������ֵ
/// @param[in] nPower ��10�ļ�������ʾ����123��ʾ��123000����˲���Ϊ3
/// @return ����str���ڵ�λ��
///
extern EXT_CLASS TCHAR *GlbGetNumString(TCHAR str[], __int64 nNum, int nPower, bool bHasSpace, TCHAR* chSpace);



//������Ϣ����
#define CM_USER_STATE					1						//״̬�ı�
#define CM_USER_SEND_TIMES_MONEY		2						//�û���ð������͵Ľ��
//��������
#define ACT_USER_UP						1						//�û�����
#define ACT_WATCH_UP					2						//�Թ�����
#define ACT_USER_SIT					3						//�û�����
#define ACT_WATCH_SIT					4						//�Թ�����
#define ACT_USER_CUT					5						//�û�����
#define ACT_USER_AGREE					6						//�û�ͬ��
#define ACT_GAME_BEGIN					7						//��Ϸ��ʼ
#define ACT_GAME_END					8						//��Ϸ����
#define ACT_USER_POINT					9						//�û�����ֵ
#define ACT_USER_CHARM					10						//�û�����ֵ
#define ACT_USER_SEND_TIMES_MONEY		11						//�û���ð����͵Ľ��
#define ACT_USER_BANKMONEY				14						//�����ܽ�����ĸ���
#define CM_USER_PROP					12						//��Ϸ����ʹ����Ϣ

#define	IPC_USER_PROP_RESULT			13						//������Ϣ

//JianGuankun 2012-1-12 �������Ϣ
#define	IPC_TROTTING					17						//�������Ϣ
//end

#define		IPC_EX_SKIN						18	

#define ACT_USER_CONTESTPOINT			15
#define ACT_USER_QUEUE_DISMISS			18						//�Ŷӻ���ɢ
#define ACT_USER_AVATAR_CHANGE			19						//����������

#define SPLIT_WIDE							2//4						//��������
#define WM_GAME_QUIT				WM_USER + 1001					//��Ϸ����
#define WM_REQUEST_PLAYERLIST		WM_USER + 1002				//��ƽ̨����ͬ���������
#define WM_SEND_GAME_DATA			WM_USER + 1003				// ������Ϣ����������
#define WM_SEND_GAME_DATA_NO_PARAM	WM_USER + 1004				// ������Ϣ����������
#define WM_USE_PROP					WM_USER + 1005				// �û��ӵ��������ʹ�õ���
#define WM_PROP_BUY					WM_USER + 1006				// �û��ӵ�������й������
#define WM_GIVE_PROP				WM_USER + 1007				// �û��ӵ�����������͵���
#define WM_USE_BOARDCAST			WM_USER + 1008				// �û�ʹ�ô�С����
#define WM_SET_STATION				WM_USER + 1009				// ������Ϸ״̬



// PengJiLin, 2010-10-13, �̵���ߵļ�ʱ������
#define WM_PROP_BUY_NEW			WM_USER + 1010
// VIP����    11-5-21     add wyl;
#define WM_BUY_VIP				WM_USER + 1011				// VIP����    //add wyl;

#define WM_PROP_BUYANDUSE				WM_USER + 1022				// VIP����    //add wyl;



// PengJiLin, 2011-6-24, ������Ϣ
#define WM_ACTIVE_TO_ROOM       WM_USER + 1012

#define WM_FRIEND				WM_USER + 1013				// ��Ӻ���
#define WM_FRIEND_R				WM_USER + 1014				// ��Ӻ���
#define WM_GETFRIENDLIST		WM_USER + 1015				// ��ȡ�����б�
#define WM_WINDOW_MOVE			WM_USER + 1016              // 	
#define WM_SCREEN_SIZE_CHANGE	WM_USER + 1017              // ��Ļ�ֱ��ʱ仯
// duanxiaohui 2011-11-11 ���������
#define WM_EMENY                WM_USER + 1020              // ���������
#define WM_EMENY_R              WM_USER + 1021              // ���������    
// end duanxiaohui

//�°����д�����Ϣ
//Add by JianGuankun 2011-12-30
#define WM_BANK2_CLOSE				WM_USER + 1030				//�ر�������Ϣ
#define WM_BANK2_TEST				WM_USER + 5500				//�ر�������Ϣ
#define WM_BANK2_CHECK_SUC			WM_USER + 1031			//���д�ȡ�ɹ�
//End Add

struct SendGameDataStruct
{
	BYTE bMainID;
	BYTE bAssistantID;
	BYTE bHandleCode;
	void *pData;
	UINT uBufLen;
};
struct SendGameDataStructWithoutParam
{
	BYTE bMainID;
	BYTE bAssistantID;
	BYTE bHandleCode;
};

#define IDM_GETPOINT						WM_USER + 430				//����
#define IDM_GET_ROOMNAME					WM_USER+435					//�õ���������
#define IDM_CLOSE_GAME						WM_USER+436					//���߸������Ѿ��ر�
//��Ϣ����
#define WM_HIT_LOGO				WM_USER+30								//����ͷ��

//��ʱ��
#define ID_GIF_TIMER					50						//gif����
#define ID_GIF_TIMER2					51						//gif����


//�û�״̬�ı�
struct CM_UserState_Change_ForExe
{
	BYTE								bActionCode;			//��������
	BYTE								bDeskStation;			//����λ��
	UserItemStruct						uisUserItem;			//�û�ָ��
};
//send to game talking
struct CM_UserState_Send_Times_Money_For_Exe
{
	int dwUserID;
    int                                 dwGetMoney;             // �����Ľ����
    int                                 dwMoneyOnTimes;         // ���ö���ʱ�����͵Ľ��
    int                                 dwMoneyOnCounts;        // ���ö��پ����͵Ľ��
    int                                 dwTimesNeed;            // ��һ�β����ʱ��
    int                                 dwCountsNeed;           // ��һ�β���پ�
};

// ��Ϸ��Ϣ
struct GameInfoStructInExe
{
	GameInfoStructInExe()
	{
		bDeskOnwer = true;							//�Ƿ�̨��
		bGameStation = 0;						//��Ϸ״̬
		memset(&uisMeUserInfo, 0, sizeof(UserItemStruct));			//�����Ϣ
		_tcscpy(uisMeUserInfo.GameUserInfo.nickName, TEXT("����"));
		_tcscpy(uisMeUserInfo.GameUserInfo.szName, TEXT("������"));
		uisMeUserInfo.GameUserInfo.dwUserID = 100001;
		uisMeUserInfo.GameUserInfo.bDeskNO = 3;
		uisMeUserInfo.GameUserInfo.bDeskStation = 255;

		dwGamePower = 0;						//�û�Ȩ��
		dwMasterPower = 0;						//����Ȩ��
		dwRoomRule = 0;							//���ù���
		pOrderName = GetGameOrderNameInExe;						//�ȼ�����

		//���ò���
		bEnableSound = true;						//��������
		bEnableWatch = false;						//�����Թ�
		bShowUserInfo = true;						//��ʾ����
		bEnableBackSound = true;					//��������

		//��������
		_tcscpy(szProcessName, TEXT("10901800.exe"));					//��������
		uDeskPeople = 180;						//��Ϸ����
		uRoomID = 2;							//�����ʾ
		dwGameMSVer = 2;						//���汾��
		dwGameLSVer = 0;						//���汾��
		uComType = 1;							//��Ϸ����
		uNameID = 10901800;							//���ֱ�ʾ
		//uNameID = 10003300;							//���ֱ�ʾ
		_tcscpy(szGameName, TEXT("������"));		//��Ϸ����
		uLessPoint = 0;							//���پ���ֵ
		memset(szGameRoomName, 0, sizeof(szGameRoomName));

		iBasePoint = 1;
		iLowCount = 0;
		i64Chip = 0;
		i64TimeStart = 0;
		i64TimeEnd = 0;
		i64LowChip = 0;
	}
	//IFindUserInterface				* pIFindUser;						//���ҽӿ�
	//������Ϣ
	bool							bDeskOnwer;							//�Ƿ�̨��
	BYTE							bGameStation;						//��Ϸ״̬
	UserItemStruct					uisMeUserInfo;						//�����Ϣ
	long int						dwGamePower;						//�û�Ȩ��
	long int						dwMasterPower;						//����Ȩ��
	long int						dwRoomRule;							//���ù���
	GetOrderName					* pOrderName;						//�ȼ�����

	//���ò���
	bool							bEnableSound;						//��������
	bool							bEnableWatch;						//�����Թ�
	bool							bShowUserInfo;						//��ʾ����
	bool							bEnableBackSound;					//��������

	//��������
	TCHAR							szProcessName[31];					//��������
	UINT							uDeskPeople;						//��Ϸ����
	UINT							uRoomID;							//�����ʾ
	DWORD							dwGameMSVer;						//���汾��
	DWORD							dwGameLSVer;						//���汾��
	UINT							uComType;							//��Ϸ����
	UINT							uNameID;							//���ֱ�ʾ
	TCHAR							szGameName[61];						//��Ϸ����
	UINT							uLessPoint;							//���پ���ֵ

	bool                            bIsInRecord;                        //�Ƿ���¼���� yjj  090223


	int								iBasePoint;
	//����ר��
	int									iLowCount;
	__int64								i64Chip;
	__int64								i64TimeStart;
	__int64								i64TimeEnd;
	__int64								i64LowChip;

	char						szGameRoomName[61];					//��Ϸ��������
};


#endif // _COMMON_DEFINE_H
