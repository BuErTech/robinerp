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



//������Ϣ����
#define CM_USER_STATE					1						//״̬�ı�
#define CM_USER_SEND_TIMES_MONEY       2                       //�û���ð������͵Ľ��
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
#define ACT_USER_SEND_TIMES_MONEY      11                      //�û���ð����͵Ľ��

#define SPLIT_WIDE							2//4						//��������
#define WM_GAME_QUIT				WM_USER + 1001					//��Ϸ����
#define WM_REQUEST_PLAYERLIST		WM_USER + 1002				//��ƽ̨����ͬ���������
#define WM_SEND_GAME_DATA			WM_USER + 1003				// ������Ϣ����������
#define WM_SEND_GAME_DATA_NO_PARAM	WM_USER + 1004				// ������Ϣ����������
#define WM_USE_PROP					WM_USER + 1005				// �û��ӵ��������ʹ�õ���
#define WM_PROP_BUY					WM_USER + 1006				// �û��ӵ�������й������
#define WM_GIVE_PROP				WM_USER + 1007				// �û��ӵ�����������͵���
#define WM_USE_BOARDCAST			WM_USER + 1008				// �û�ʹ�ô�С����

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
	int iSendMoney;
	int iSendTimes;
	int iLastTimes;	
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
		uisMeUserInfo.GameUserInfo.bDeskStation = 1;

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
		_tcscpy(szProcessName, TEXT("zjh.ico"));					//��������
		uDeskPeople = 3;						//��Ϸ����
		uRoomID = 2;							//�����ʾ
		dwGameMSVer = 2;						//���汾��
		dwGameLSVer = 0;						//���汾��
		uComType = 1;							//��Ϸ����
		uNameID = 100600302;							//���ֱ�ʾ
		_tcscpy(szGameName, TEXT("����"));						//��Ϸ����
		uLessPoint = 0;							//���پ���ֵ
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
};


#endif // _COMMON_DEFINE_H
