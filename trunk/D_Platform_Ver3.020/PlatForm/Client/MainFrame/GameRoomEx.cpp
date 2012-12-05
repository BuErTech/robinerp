#include "StdAfx.h"
#include <comutil.h>
#include "GamePlace.h"
#include "GameRoomEx.h"
#include "MainRoomEx.h"
#include "RoomSetDlg.h"
#include "RoomSetWnd.h"
#include "FindUserDlg.h"
#include "GamePlaceDlg.h"
#include "SendWarnDlg.h"
//#include ".\gameroomex.h"
#include "BankDlg.h"
#include "PBankDlg.h"  /// ��ʽ����
#include "setdeskBasePoint.h"
#include "checkpsw.h"

#include "ExtRobot.h"

#include "PassDesk.h"  //Fred Huang,20081205
#include "gameplacemessage.h"
#include "Bank4ZMessage.h"
#include "DlgRoomFee.h"

#include "detours.h"
#include "skinsb.h"


//��̬��������
int							CGameRoomEx::m_iMaxShortWnd=10;					//��ര��
//bool						CGameRoomEx::m_bAutoSit=true;					//�Զ�����
bool						CGameRoomEx::m_bAutoSit=false;					//�Զ�����
BYTE						CGameRoomEx::m_bSameIP=FALSE;					//��ͬ IP
BYTE						CGameRoomEx::m_bSameIPNumber=FALSE;				//����ǰ��λIP��ͬ�������Ϸ
BYTE						CGameRoomEx::m_bInvaliMode=INVALI_ALL;			//����ģʽ
BYTE						CGameRoomEx::m_bSaveTalk=TRUE;					//�����¼
BYTE						CGameRoomEx::m_bShowMessage=TRUE;				//��ʾ��Ϣ
BYTE						CGameRoomEx::m_bShowUser=TRUE;					//��ʾ�û�
CArray<LONG,LONG>			CGameRoomEx::m_ShieldArray;						//�����û�
BYTE						CGameRoomEx::m_deskStation=255;					//20081211
BYTE						CGameRoomEx::m_deskIndex=255;					//20081211
//������Ϸ��ܺ�������
typedef IFrameInterface * (BeginInitGoGame)();

//��ȡ�ȼ���������
//typedef GetOrderName * (GetOrderFunction)(int dwPoint);
typedef GetOrderName * (SetGameInitClass)(int dwPoint);

//�궨��
#define CONTROL_SPACE						2							//�ؼ����
#define SPLIT_WIDE							7							//��������
//#define BUTTON_WIDE						8							//���ذ�ť���
//#define BUTTON_HIGHT						80							//���ذ�ť�߶�
#define BUTTON_WIDE							9							//�����ذ�ť�����		,�����ذ�ť�����Ǳ߿����Ǹ�����
#define BUTTON_HIGHT						109							//�����ذ�ť���߶�
#define BT_BUTTON_WIDE						63							//��ť���
#define BT_BUTTON_HIGHT						39							//��ť�߶�
#define BUTTON_MORE_WIDE					0							//���Ҹ��Ӱ�ť
#define BUTTON_Y_POS						10							//��ť Y ����
#define BUTTON_START_POS					160							//��ť��ʼ��
#define BUTTON_END_POS						115							//��ť��ֹ��
#define ROOM_BAR_HEIGHT						8							//�����ܸ߶�
//#define BOTTOM_HIDE_WIDTH					80							//���ذ�ť���							
//#define BOTTOM_HIDE_HEIGHT					8							//���ذ�ť�߶�
#define BOTTOM_HIDE_WIDTH					109							//�����ذ�ť�����		,�����ذ�ť�����Ǳ߿����Ǹ�����					
#define BOTTOM_HIDE_HEIGHT					9							//�����ذ�ť���߶�
#define CHAR_HEIGHT							22							//����߶�

//��ܴ�С 
#define ROOM_FRAME_TOP						30//56							//��Ϸ����Ŀ���ϸ�
#define ROOM_FRAME_LEFT						0							//������
#define ROOM_FRAME_RIGHT					0							//����ҿ�
#define ROOM_FRAME_BOTTOM					0							//����¸�

//�������
#define ROOM_FRAME_BLUE_HEIGHT				25
//��ܺ���
#define ROOM_FRAME_RED_HEIGHT				31

#define CHECK_TIME						3000						//��ʱ�����
#define ID_CHECK_SIGNAL                5

//��ť
#define BUTTON_TOP_HIGHT_S				28//��������İ�ţ
#define BUTTON_TOP_WIDE_S					57//��������İ�ţ

#define IDM_SKIN							WM_USER+3
//�û��˵���Ϣ����
#define IDM_SEND_MESSAGE					WM_USER+220					//������Ϣ
#define IDM_SEND_PRESENT	 				WM_USER+221					//��������
#define IDM_COPY_USER_NAME					WM_USER+222					//�����û�
#define IDM_SET_TALK_TARGET					WM_USER+223					//̸������
#define IDM_CANCEL_TALK						WM_USER+224					//ȡ������
#define IDM_WATHC_GAME						WM_USER+225					//�Թ���Ϸ
#define IDM_INVITE_PLAY						WM_USER+226					//�������
#define IDM_SET_FRIEND						WM_USER+227					//��Ϊ����
#define IDM_SET_EMENY						WM_USER+228					//���ܻ�ӭ
#define IDM_LEFT_USER						WM_USER+229					//�����뿪
#define IDM_SHIELD_USER						WM_USER+230					//�����û�
#define IDM_UNSHIELD_USER					WM_USER+231					//�����û�
#define IDM_USER_INFO						WM_USER+232					//�û�����
#define IDM_ROOM_SYSTEM						WM_USER+233					//������Ϣ
#define IDM_GAME_SYSTEM						WM_USER+234					//��Ϸ��Ϣ
//#define IDM_CALL_GM							WM_USER+235					//��������
#define IDM_ONE_LEFT_ROOM                   WM_USER+236 // ���������������䣬PengJiLin, 2010-8-26
#define IDM_USER_INFO_INDESK				WM_USER+237					//�û�����
#define IDM_TALK_OBJECT						WM_USER+300					//�����û�

//��������
#define IDM_MANAGER_USER					WM_USER+250					//�������
#define IDM_SEND_WARNNING					WM_USER+251					//���;���
#define IDM_CUT_USER						WM_USER+252					//�û�����
#define IDM_LIMIT_ACCOUN					WM_USER+253					//��ֹ�ʻ�
#define IDM_LIMIT_IP						WM_USER+254					//��ֹ��ַ
#define IDM_LIMIT_OTHER_ROOM_TALK			WM_USER+255					//��ֹ�����˴�������
#define IDM_LIMIT_OTHER_GAME_TALK			WM_USER+256					//��ֹ��������Ϸ����
#define IDM_LIMIT_OTHER_SEND				WM_USER+257					//��ֹ�����˷�����
#define IDM_LIMIT_OTHER_PLAY				WM_USER+258					//��ֹ��������Ϸ
#define IDM_LIMIT_OTHER_WATCH				WM_USER+259					//��ֹ�������Թ�
#define IDM_SAFE_FINISH						WM_USER+260					//�����Ϸ
#define IDM_LOOK_USER_IP					WM_USER+261					//�鿴 IP
#define IDM_HIDE_IP							WM_USER+262					//���� IP
#define IDM_LOOK_POWER						WM_USER+263					//��ѯȨ��
#define IDM_SET_POWER						WM_USER+264					//����Ȩ��
#define IDM_BIND_GAME						WM_USER+265					//����Ϸ
#define IDM_BIND_ACCOUN						WM_USER+266					//���ʺ�
#define IDM_HIDE_ACTION						WM_USER+267					//���ض���
#define IDM_CONTROL_USER					WM_USER+268					//�����û�
#define IDM_MUCH_CLIENT						WM_USER+269					//�����ͻ���
#define IDM_SET_SERVER						WM_USER+270					//���÷�����
#define IDM_LISTEN_MESSAGE					WM_USER+271					//��������Ϣ
#define IDM_LISTEN_GAME						WM_USER+272					//������Ϸ����
#define IDM_ADD_PIONT						WM_USER+273					//�Ӽ�����ֵ
#define IDM_LOGON_LIMIT						WM_USER+274					//�����½���Ʒ���
#define IDM_ALL_POWER						WM_USER+275					//��������Ȩ��
#define IDM_ALONE_DESK						WM_USER+276					//����			20081212 Fred Huang
#define IDM_DEALONE_DESK					WM_USER+277					//�������		20081212 Fred Huang
#define IDM_ALONE_DESK_ALL					WM_USER+278					//����			20081212 Fred Huang
#define IDM_DEALONE_DESK_ALL				WM_USER+279					//�������		20081212 Fred Huang
#define IDM_GETPOINT						WM_USER + 430				//����
#define IDM_GET_ROOMNAME					WM_USER+435					//�õ���������

#define IDM_CLOSE_GAME						WM_USER+436					//�ر���Ϸ����
#define WM_GAME_LAUNCHED					WM_USER+439					///< ��Ϸ�������̽���

// PengJiLin, 2010-6-29, ��ս��ʱ�䵽, �ر���Ϸ��
#define WM_GAME_TIMEOUT                     WM_USER+442

bool bExtendRobot=false;


#define IDR_EXIT							1							//�Զ��뿪��ʱ��
#define LOAD_NEWS							2							//�����Ŷ�ʱ��
#define LOAD_SYS_MSG						3							//��ϵͳ��Ϣ��ʱ��
#define ID_TIME_ADD_SEQUE					10							//�����ʱ��
#define TIME_KEEP_ALIVE						100							//KeepAlive ��ʱ��
#define TIME_SLEEP_EXIT_LACK				1013						//KeepAlive ��ʱ��
#define TIME_SLEEP_EXIT_OVERFLOW			1014						//KeepAlive ��ʱ��
#define TIME_NOACTION_EXIT					20							//ָ��ʱ����û�н�����Ϸ�������������
#define TIME_CHECK_MAX_POINT				101							//�������޼�ⶨʱ�����
#define TIME_ENABLE_EXIT_BTN				102							//����Ϸ�󣬹�һ��ʱ���÷����˳���ť����\
																			Ϊ��ֹGameLauncher���ط�IPC��Ϣ�����·����˳���ťһֱ������

#define TIME_QUEUEROOM_DISMISS				103							//�Ŷӻ����������˳���ɢ		
#define TIME_QUEUEROOM_TIMEOUT				104							//���˷��Ŷ�ʱ�����, �ɿ�����������
#define TIME_CHECK_SOCKETCLOSE				105							//���˷����ܻ����л�ʱ�����䣬�Զ���⣬������

#define ADD_SEQUE_TIMER						10000						//�������ʱ��

#define EXPERCISE_MONEY						5000000						//���ѵ������Ǯ��

#define WM_REFLASHMONEY						WM_USER+301					//ˢ����Ϸ����ֵ

// PengJiLin, 2011-7-8, WM_REFLASHMONEY ����ˢ��ͬ�����ʱ����Ϊʹ����64λ�����ò������ͣ�ʹ��ȫ�ֱ�����
__int64 g_i64MyMoney = 0;

///����ָ�룬Ϊ�Ŷӻ����
__int64                  g_PointArray[20];                  ///�ȼ���������
map<int,string>          g_LevelNameArray;                  ///�ȼ���������

BEGIN_MESSAGE_MAP(CGameRoomEx, CBaseRoom)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_MESSAGE(WM_HIT_EXPMESSTION,OnExpression)
	ON_MESSAGE(WM_HIT_DESK_BEGIN,OnHitDeskBegin)
	ON_MESSAGE(WM_HIT_BJL_BEGIN,OnHitBJLDeskBegin)// �ټ��� add by wlr 20090713

	ON_MESSAGE(WM_HIT_QUEUE_STOP, OnHitQueueStop)// 

	ON_MESSAGE(WM_LEFT_MOUSE_HIT_DESK,OnLeftHitDesk)
	ON_MESSAGE(WM_RIGHT_MOUSE_HIT_DESK,OnRightHitDesk)
	ON_MESSAGE(WM_HITDESK,OnHitDesk)
	ON_MESSAGE(WM_SHOWGAMEWINDOW,OnShowGameWindow)
	ON_MESSAGE(WM_LOADSKIN,OnLoadSkin)
	ON_MESSAGE(WM_REFLASHMONEY,OnReflashMoney)
	//	ON_MESSAGE(WM_RELOAD_TALKINI,OnReLoadTalkINI)
	ON_MESSAGE(WM_REFLASNEWS,OnReflashNews)
	ON_MESSAGE(WM_REFLAS_SYS_MSG,OnReflashSys)
	ON_MESSAGE(IDM_SHOW_LEFT,OnShowLeft)
	ON_MESSAGE(IDM_TAB_SELECT,OnTopTabSelected)
	ON_MESSAGE(IDM_TAB_SELECT_LOOST,OnTopTabLostSelected)

	ON_MESSAGE(IDM_GET_ROOMNAME,OnGetRoomName)
	ON_MESSAGE(IDM_CLOSE_GAME,OnCloseGameClient)
	//	ON_MESSAGE(IDM_BIG_BROADCAST_MSG,SendBigBoardcastMsg)
	ON_MESSAGE(IDM_QUIT_ROOM_MSG,OnCutNetQuitRoom)

	ON_MESSAGE(WM_VIPROOM_MSG,OnVIPRoomMsg)

	//ON_NOTIFY(NM_RCLICK, IDC_USER_LIST, OnNMRclickUserList)
	//ON_NOTIFY(NM_DBLCLK, IDC_USER_LIST, OnNMDblclkUserList)
	//ON_NOTIFY(NM_RCLICK, IDC_USER_FRIEND, OnNMRclickUserListFriend)
	//ON_NOTIFY(NM_DBLCLK, IDC_USER_FRIEND, OnNMDblclkUserListFriend)

	//ON_BN_CLICKED(IDC_BUTTON_MIN, OnBnClickedMin)
	//ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnBnClickedClose)
	//ON_BN_CLICKED(IDC_BUTTON_rBT1, OnBnClickedr1)
	//ON_BN_CLICKED(IDC_BUTTON_rBT2, OnBnClickedr2)
	//ON_BN_CLICKED(IDC_BUTTON_rBT3, OnBnClickedr3)
	//ON_BN_CLICKED(IDC_BUTTON_rBT4, OnBnClickedr4)
	//ON_BN_CLICKED(IDC_BUTTON_rBT5, OnBnClickedr5)
	//ON_BN_CLICKED(IDC_BT10, OnBnClickedbt10)
	//ON_BN_CLICKED(IDC_BT11, OnBnClickedbt11)
	//ON_BN_CLICKED(IDC_BT12, OnBnClickedBt8)
	//ON_BN_CLICKED(IDC_BT13, OnBnClickedbt13)
	//ON_BN_CLICKED(IDC_BT14, OnBnClickedbt14)
	//ON_BN_CLICKED(IDC_BT15, OnBnClickedbt15)

	//ON_BN_CLICKED(IDC_LISTPAGEDOWN, OnBnClickedListpagedown)
	//ON_BN_CLICKED(IDC_LISTPAGEUP, OnBnClickedListpageup)
	//ON_BN_CLICKED(IDC_BT7, OnBnClickedBt7)
	//ON_BN_CLICKED(IDC_BT6, OnBnClickedBt6)
	//ON_BN_CLICKED(IDC_SHOWMENU, OnBnClickedShowmenu)
	//ON_BN_CLICKED(IDC_SKIN,OnBnClickedSkin)
	//ON_BN_CLICKED(IDC_GAME_FACE, OnBnClickedGameFace)
	//ON_BN_CLICKED(IDC_FIND, OnFindNullDesk)
	//ON_BN_CLICKED(IDC_ROOM_SET, OnBnClickedRoomSet)
	//ON_BN_CLICKED(IDC_HIDE_RIGHT, OnBnClickedHideRight)
	//ON_BN_CLICKED(IDC_BT9, &CGameRoomEx::OnBnClickedBt9)
	//ON_BN_CLICKED(IDC_BT8, &CGameRoomEx::OnBnClickedBt8)
	ON_MESSAGE(IDM_GETPOINT,OnGetPoint)
	ON_MESSAGE(IDM_SEND_ROOMMSG,OnSendRoomMsg)
	//��ť����
	ON_BN_CLICKED(IDC_BTN_ROOM1, OnBnClickedBt1)
	ON_BN_CLICKED(IDC_BTN_ROOM2, OnBnClickedBt2)
	ON_BN_CLICKED(IDC_BTN_ROOM3, OnBnClickedBt3)
	ON_BN_CLICKED(IDC_BTN_ROOM4, OnBnClickedBt4)
	ON_BN_CLICKED(IDC_BTN_ROOM5, OnBnClickedBt5)
	ON_BN_CLICKED(IDC_BTN_ROOM6, OnBnClickedBt6)
	ON_BN_CLICKED(IDC_BTN_ROOM7, OnBnClickedBt7)
	ON_BN_CLICKED(IDC_BTN_ROOM8, OnBnClickedBt8)
	ON_BN_CLICKED(IDC_BTN_ROOM9, OnBnClickedBt9)
	ON_BN_CLICKED(IDC_BTN_ROOM10, OnBnClickedBt10)

	ON_MESSAGE(WM_INVITE_PLAY,OnInvite)
	ON_MESSAGE(WM_CHATING_WITH,OnChating)
	ON_MESSAGE(WM_WATCH_GAME,OnWatch)
	ON_MESSAGE(IDM_OPEN_IE,OnOpenIE)

	ON_WM_CREATE()
	ON_MESSAGE(WM_EXCHANGE_SKIN,OnExchangeSkin)
	//wushuqun 2009.7.13
	//�޸���Ϸ�������˳���Ϸʱ��������
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CGameRoomEx, CBaseRoom)
	ON_EVENT(CGameRoomEx, IDC_WEB, 250, CGameRoomEx::BeforeNavigate2Explorer, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CGameRoomEx, IDC_WEB, 271, CGameRoomEx::NavigateErrorExplorer, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

//���캯��
CGameRoomEx::CGameRoomEx(TCHAR * szGameName, RoomInfoStruct * pRoomInfo, MSG_GP_R_LogonResult * pLogonInfo,CRoomRightWnd *pRightWnd) 
: CBaseRoom(IDD_GAME_ROOM)
{
	m_bIsSitting = false;
	m_nKeepAliveCount=0;
	m_bInit = false;
	//��ʼ������
	m_bWaitSit=false;
	m_bHideRight=false;
	m_bHideBottom=false;
	m_bRoomSystem=false;
	m_bGameSystem=false;
	m_bLogonFinish=false;

	m_bLastFindDesk=255;
	m_dwTalkTarget=0L;
	m_dwTalkTarget=0L;
	m_dwLastTalkTime=0L;
	m_dwCurrentUserID=0L;
	m_pManageDlg=NULL;
	m_hGameInstance=NULL;
	m_pMatchUserInfo=NULL;
	m_pRoomInfo=pRoomInfo;
	m_pLogonInfo=pLogonInfo;
	m_bPreSitDeskIndex=255;
	m_bPreSitDeskStation=255;
	m_bMatchDeskIndex=255;
	m_bMatchDeskStation=255;
	m_pRightWnd = pRightWnd;//�ұߴ���ָ��
	m_colorresource=pRightWnd->m_colorresource;//ʹ�������ļ��е����ã�Fred Huang 2008-07-02
	m_iNewsNO=0;
	m_iSysMsgNO=0;
	memset(m_RecentlyUser,0,sizeof(m_RecentlyUser));

	boardCast=NULL;//2008-08-18

	//��ʼ�����ñ���
	m_szPass[0]=0;
	m_bCutPercent=10;
	m_bPass=FALSE;
	m_bLimitCut=FALSE;
	m_bLimitPoint=0x00;
	m_dwLowPoint=-100000L;
	m_dwHighPoint=100000L;

	m_dwLowMoney=-100000L;
	m_dwHighMoney=100000L;

	//���ÿؼ�ָ��
	m_pFindUser=NULL;
	m_pUserList=NULL;
	//	m_pFriendList=NULL;
	m_IGameFrame=NULL;
	m_pInputMessage=NULL;
	m_pPersonBank = NULL;
	m_pBankWnd = NULL;
	m_pBankVerifyWnd = NULL;
	m_pBankWndEx = NULL;
	m_pUserDetailsDlg = NULL;
	//	m_pCallGMDlg = NULL;

	//��ʼ����������
	memset(&m_GameInfo,0,sizeof(m_GameInfo));
	lstrcpy(m_GameInfo.szGameName,szGameName);
	lstrcpy(m_GameInfo.szProcessName,pRoomInfo->szProcessName);
	m_GameInfo.pISocketSend=this;
	gpGameRoomEx = this;
	m_GameInfo.pIFindUser=&m_UserManage;
	m_GameInfo.bShowUserInfo=true;
	m_GameInfo.bEnableSound=true;
	m_GameInfo.bEnableWatch=false;
	m_GameInfo.uNameID=pRoomInfo->pComRoomInfo.uNameID;
	m_GameInfo.uRoomID=pRoomInfo->pComRoomInfo.uRoomID;
	m_GameInfo.dwGameMSVer=pRoomInfo->dwGameMSVer;
	m_GameInfo.dwGameLSVer=pRoomInfo->dwGameLSVer;
	m_GameInfo.uComType=pRoomInfo->pComRoomInfo.uComType;
	m_GameInfo.uDeskPeople=pRoomInfo->pComRoomInfo.uDeskPeople;
	m_GameInfo.uVirtualUser=pRoomInfo->pComRoomInfo.uVirtualUser;	//20081211 , Fred Huang
	m_GameInfo.uVirtualGameTime=pRoomInfo->pComRoomInfo.uVirtualGameTime;   //20081211 , Fred Huang
	if (m_GameInfo.uVirtualGameTime == 0)
	{
		m_GameInfo.uVirtualGameTime = 50;
	}
	m_GameInfo.dwRoomRule=pRoomInfo->pComRoomInfo.dwRoomRule;//20081211 , Fred Huang
	m_bAddQueue = false;

	m_GameInfo.bIsInRecord = false;  //added by yjj for record 090223

	//wushuqun 2009.7.1
	strcpy(m_GameInfo.szGameRoomName,pRoomInfo->pComRoomInfo.szGameRoomName);

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	m_GameInfo.uNoActionKickTimer = fMsg.GetKeyVal("GameRoom","KickOutTimer", 0);

	
	m_roomNameFont = new CFont();
	m_roomNameFont->CreateFont(-12,0,0,0,400,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
	
		//�������źŵ�  yjj 090227
	img=NULL;
    m_NetSignalLevel=0;
    m_SavedSingalIndex=0;

	m_pQueueInRoomDlg = NULL;
	//m_pQueueInRoomDlg = new CQueueInRoomDlg();
	//m_pQueueInRoomDlg->m_uRoomID = m_pRoomInfo->pComRoomInfo.uRoomID;
	m_Rgn.CreateRectRgn(0,0,0,0);
	m_RoomOwner = 0;

    // PengJiLin, 2010-6-29
    m_bGameTimeOut = false;
	m_bAllowJoin = FALSE;

    // PengJiLin, 2010-9-13
    m_bCheckKickPropTime = FALSE;
    m_bCheckAntiKickPropTime = FALSE;	

    m_pGetHelpMoneyDlg = NULL;
	m_bActive = TRUE;
	m_DeskFrame.m_bActive = m_bActive;

	m_bLoginRoom = false;

	m_bQueueRoomDisMiss = false;
	m_iQueueNo = 0;
	m_iQueueStation = -1;
	m_bCloseIsAutoQueue = false;
	m_iQueueTime = 60;
	m_bEnterAutoQueue = false;
	m_bContinueExit = false;
	m_bQueueing = false;


	TCHAR sz[100] = {0},szPath[MAX_PATH] ={0};
	lstrcpy(sz,m_pRoomInfo->szProcessName);
	sz[lstrlen(sz)-lstrlen(".ico")]='\0';
	wsprintf(szPath,"%s\\GameOption.bcf",sz);
	CBcfFile f(szPath);
	//���˷�����
	m_iContestRoomIndex = -1;
	m_bChangeRoom = false;
	m_iChangeRooming = 0;
	m_iPeopleLimit1 = f.GetKeyVal("ContestConfig","m_PeopleLimit1", int(m_pRoomInfo->pComRoomInfo.uDeskPeople * m_pRoomInfo->pComRoomInfo.uDeskCount / 2));//; 
	m_iPeopleLimit2 = f.GetKeyVal("ContestConfig","m_PeopleLimit2", m_iPeopleLimit1 * 5 / 4);//m_iPeopleLimit1 * 5 / 4;
	m_iQueueTimeOut = f.GetKeyVal("ContestConfig","m_QueueTimeOut", 30);
	m_iSocketClose = 0;
	if (m_pRoomInfo->pRoomItem->m_bIsContest)
	{
		SetTimer(TIME_CHECK_SOCKETCLOSE, 1000, NULL);
	}


	m_bCloseIsAutoQueue = f.GetKeyVal("QueueRoom","CloseIsAutoQueue", 0);
	m_iQueueTime = f.GetKeyVal("QueueRoom","QueueTime", 0);
	m_bEnterAutoQueue = f.GetKeyVal("QueueRoom","EnterAutoQueue", 0);
	m_bContinueExit = f.GetKeyVal("QueueRoom","ContiueEnter", 0);
// 	m_BzLog.CreateGameRoomLog(&pRoomInfo->pComRoomInfo);
// 
// 	m_BzLog.OutputTraceStr("DClient:Room ptr = 0x%08x",this);
// 	m_BzLog.OutputTraceStr("");

	return;
}




bool CGameRoomEx::m_bIsAsk = true;
//���캯��
CGameRoomEx::CGameRoomEx(TCHAR * szGameName, RoomInfoStruct * pRoomInfo, MSG_GP_R_LogonResult * pLogonInfo) 
: CBaseRoom(IDD_GAME_ROOM)
{
	m_bIsSitting = false;
	m_nKeepAliveCount=0;
	m_bInit = false;
	//��ʼ������
	m_bWaitSit=false;
	m_bHideRight=false;
	m_bHideBottom=false;
	m_bRoomSystem=false;
	m_bGameSystem=false;
	m_bLogonFinish=false;

	m_bLastFindDesk=255;
	m_dwTalkTarget=0L;
	m_dwTalkTarget=0L;
	m_dwLastTalkTime=0L;
	m_dwCurrentUserID=0L;
	m_pManageDlg=NULL;
	m_hGameInstance=NULL;
	m_pMatchUserInfo=NULL;
	m_pRoomInfo=pRoomInfo;
	m_pLogonInfo=pLogonInfo;
	m_bPreSitDeskIndex=255;
	m_bPreSitDeskStation=255;
	m_bMatchDeskIndex=255;
	m_bMatchDeskStation=255;
	m_iNewsNO=0;
	m_iSysMsgNO=0;
	memset(m_RecentlyUser,0,sizeof(m_RecentlyUser));



	//��ʼ�����ñ���
	m_szPass[0]=0;
	m_bCutPercent=10;
	m_bPass=FALSE;
	m_bLimitCut=FALSE;
	m_bLimitPoint=0x00;
	m_dwLowPoint=-100000L;
	m_dwHighPoint=100000L;

	//���ÿؼ�ָ��
	m_pFindUser=NULL;
	m_pUserList=NULL;
	//	m_pFriendList=NULL;
	m_IGameFrame=NULL;
	m_pInputMessage=NULL;
	m_pPersonBank = NULL;
	m_pBankWnd = NULL;
	m_pBankVerifyWnd = NULL;
	m_pBankWndEx = NULL;
	m_pUserDetailsDlg = NULL;
	//	m_pCallGMDlg = NULL;
	//��ʼ����������
	memset(&m_GameInfo,0,sizeof(m_GameInfo));
	lstrcpy(m_GameInfo.szGameName,szGameName);
	m_GameInfo.pISocketSend=this;
	gpGameRoomEx = this;
	m_GameInfo.pIFindUser=&m_UserManage;
	m_GameInfo.bShowUserInfo=true;
	m_GameInfo.bEnableSound=true;
	m_GameInfo.bEnableWatch=false;
	m_GameInfo.uNameID=pRoomInfo->pComRoomInfo.uNameID;
	m_GameInfo.uRoomID=pRoomInfo->pComRoomInfo.uRoomID;
	m_GameInfo.dwGameMSVer=pRoomInfo->dwGameMSVer;
	m_GameInfo.dwGameLSVer=pRoomInfo->dwGameLSVer;
	m_GameInfo.uComType=pRoomInfo->pComRoomInfo.uComType;
	m_GameInfo.uDeskPeople=pRoomInfo->pComRoomInfo.uDeskPeople;
	m_GameInfo.uVirtualUser=pRoomInfo->pComRoomInfo.uVirtualUser;//20081211 , Fred Huang
	m_GameInfo.dwRoomRule=pRoomInfo->pComRoomInfo.dwRoomRule;//20081211 , Fred Huang
	m_bAddQueue = false;

		//�������źŵ�  yjj 090227
	img=NULL;
    m_NetSignalLevel=0;
    m_SavedSingalIndex=0;
	m_GameInfo.bIsInRecord = false;  //added by yjj for record 090223

	m_pQueueInRoomDlg = NULL;
	//m_pQueueInRoomDlg = new CQueueInRoomDlg();
	//m_pQueueInRoomDlg->m_uRoomID = m_pRoomInfo->pComRoomInfo.uRoomID;

	m_Rgn.CreateRectRgn(0,0,0,0);

    // PengJiLin, 2010-6-29
    m_bGameTimeOut = false;
	m_bAllowJoin = FALSE;

    // PengJiLin, 2010-9-13
    m_bCheckKickPropTime = FALSE;
    m_bCheckAntiKickPropTime = FALSE;

    m_pGetHelpMoneyDlg = NULL;

	m_bLoginRoom = false;

	m_bQueueRoomDisMiss = false;
	m_iQueueNo = 0;
	m_iQueueStation = -1;
	m_bCloseIsAutoQueue = false;
	m_iQueueTime = 60;
	m_bEnterAutoQueue = false;
	m_bContinueExit = false;
	m_bQueueing = false;

// 	m_BzLog.CreateGameRoomLog(&pRoomInfo->pComRoomInfo);
// 
// 	m_BzLog.OutputTraceStr("DClient:Room ptr = 0x%08x",this);
// 	m_BzLog.OutputTraceStr("");


	TCHAR sz[100] = {0},szPath[MAX_PATH] ={0};
	lstrcpy(sz,m_pRoomInfo->szProcessName);
	sz[lstrlen(sz)-lstrlen(".ico")]='\0';
	wsprintf(szPath,"%s\\GameOption.bcf",sz);
	CBcfFile f(szPath);
	//���˷�����
	m_iContestRoomIndex = -1;
	m_bChangeRoom = false;
	m_iChangeRooming = 0;
	m_iPeopleLimit1 = f.GetKeyVal("ContestConfig","m_PeopleLimit1", int(m_pRoomInfo->pComRoomInfo.uDeskPeople * m_pRoomInfo->pComRoomInfo.uDeskCount / 2));//; 
	m_iPeopleLimit2 = f.GetKeyVal("ContestConfig","m_PeopleLimit2", m_iPeopleLimit1 * 5 / 4);//m_iPeopleLimit1 * 5 / 4;
	m_iQueueTimeOut = f.GetKeyVal("ContestConfig","m_QueueTimeOut", 30);
	m_iSocketClose = 0;
	if (m_pRoomInfo->pRoomItem->m_bIsContest)
	{
		SetTimer(TIME_CHECK_SOCKETCLOSE, 1000, NULL);
	}

	m_bCloseIsAutoQueue = f.GetKeyVal("QueueRoom","CloseIsAutoQueue", 0);
	m_iQueueTime = f.GetKeyVal("QueueRoom","QueueTime", 0);
	m_bEnterAutoQueue = f.GetKeyVal("QueueRoom","EnterAutoQueue", 0);
	m_bContinueExit = f.GetKeyVal("QueueRoom","ContiueEnter", 0);

	return;
}


//Ϊ�����˳��ܹ��ڽ��治���������л����䣬�����ù����������ֱ�Ӽ����·���
void CGameRoomEx::RelaodRoom(int iIndex)
{
	KillTimer(TIME_KEEP_ALIVE);
	for (INT_PTR i=0;i<m_ShortMessage.GetCount();i++) 
	{
		delete ((CShortMessage *)m_ShortMessage.GetAt(i));
		m_ShortMessage.SetAt(i,NULL);
	}
	m_ShortMessage.RemoveAll();

	if(m_pFindUser !=NULL)
	{
		delete m_pFindUser;
	}

	delete [] m_pMatchUserInfo;

	m_UserManage.CleanUserInfo();

	m_pUserList->DeleteAllItems();

	m_pRightWnd->m_MessageHandle.m_pRichEcit->OnClearAll();
	//if (m_UserManage.GetOnLineUserCount() > 3)
	//{
	m_iChangeRooming = 1;

	//CloseSocket(false);
	delete m_TCPSocket;
	m_TCPSocket = new CTCPClientSocket(this);
	Sleep(100);
	if (iIndex != -1)
		EnterRoom(iIndex);
	//}
}


//���ӽ��뷿��
void CGameRoomEx::EnterRoom(int iIndex)
{
	if (0 <= iIndex && iIndex < m_pRoomInfo->pRoomItem->m_RoomContest.size())
	{
		m_iContestRoomIndex = iIndex;
		if (m_TCPSocket != NULL && m_TCPSocket->Connect(m_pRoomInfo->pRoomItem->m_RoomContest[iIndex].szServiceIP,m_pRoomInfo->pRoomItem->m_RoomContest[iIndex].uServicePort)==false)
		{
			m_Connect.DestroyWindow();
			//AFCMessageBox("���ӳ���");//,MB_ICONQUESTION,this);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"���ӳ���");
			return;
		}
	}
}

//���˳���ʱ����Ҫ�л����䣬-1Ϊ����������Ϊ�л����������
int CGameRoomEx::GetChangeRoomIndex()
{
	if (m_pRoomInfo->pRoomItem->m_RoomContest.size() <= 1) return -1;
	int _rst = -1;
	if (m_UserManage.GetOnLineUserCount() > m_iPeopleLimit1)
	{
		_rst = FindNewRoom();
		if (_rst == -1) _rst = FindMinPeopleRoom(0);
	}
	return _rst;
}

//��������
bool CGameRoomEx::ChangeRoom()
{
	int _index = GetChangeRoomIndex();
	if (_index != -1 && _index != m_iContestRoomIndex)
	{
		RelaodRoom(_index);
		return true;
	}
	return false;
}

//�ҵ���ǰ�������ٵ���С��iDownLimit�ķ���
int CGameRoomEx::FindMinPeopleRoom(int iDownLimit)
{
	int _mini = -1; 
	int _minp = 99999;
	for (int i = 0; i < m_pRoomInfo->pRoomItem->m_RoomContest.size(); i++)
	{
		int _people = m_pRoomInfo->pRoomItem->m_RoomContest[i].uPeopleCount;
		if (_people > iDownLimit && _people < _minp)
		{
			_mini = i;
			_minp = _people;
		}
	}
	return _mini;
}
//�ҵ���ǰ������൫������iUpLimit�ķ���
int	CGameRoomEx::FindMaxPeopleRoom(int iUpLimit)
{
	int _maxi = -1; 
	int _maxp = -1;
	for (int i = 0; i < m_pRoomInfo->pRoomItem->m_RoomContest.size(); i++)
	{
		int _people = m_pRoomInfo->pRoomItem->m_RoomContest[i].uPeopleCount;
		if (_people < iUpLimit && _people > _maxp)
		{
			_maxi = i;
			_maxp = _people;
		}
	}
	return _maxi;
}
//�Ƿ����еķǿշ��䶼����iLimit
bool CGameRoomEx::IsAllRoomPass(int iUpLimit)
{
	for (int i = 0; i < m_pRoomInfo->pRoomItem->m_RoomContest.size(); i++)
	{
		int _people = m_pRoomInfo->pRoomItem->m_RoomContest[i].uPeopleCount;
		if (_people > 0 && _people < iUpLimit)
		{
			return false;
		}
	}
	return true;
}

//���ǿշ��䶼�Ѿ������˿����޿����·���
int CGameRoomEx::FindNewRoom()
{
	int _rst = -1;
	if (IsAllRoomPass(m_iPeopleLimit2))
	{
		_rst = FindMinPeopleRoom(-1);
	}
	return _rst;
}

///��ȡ��Ϸ����ָ��
///@return ������Ϸ�����ָ��
CGameRoomEx * GetGameRoom(void)//Ϊ�Ŷӻ����
{
	if (NULL != gpGameRoomEx)
	{
		return IsWindow(gpGameRoomEx->GetSafeHwnd()) ? gpGameRoomEx : NULL;
	}
	
	return NULL;
}

//��ȡ��Ϸ����ָ��
//�࿪����汾ƽ̨ʹ��
//Added by JianGuankun 2012.02.14
CGameRoomEx * GetGameRoom(UINT uRoomID)
{
	CMainRoomEx* pMainRoom = GetMainRoom();
	RoomInfoStruct* pRoomInfo = NULL;

	if(pMainRoom) 
	{
		if(pMainRoom->GetRoomInfoStruct(&pRoomInfo))
		{
			for (BYTE i = 0; i < MAX_GAME_ROOM; i++)
			{
				if (pRoomInfo[i].bAccess && (pRoomInfo[i].pComRoomInfo.uRoomID == uRoomID))
				{
					CGameRoomEx* pGameRoom = (CGameRoomEx*)pRoomInfo[i].pGameRoomWnd;
					if (IsWindow(pGameRoom->GetSafeHwnd()))
					{
						return pGameRoom;
					}
				}
			}
		}
	}

	return NULL;
}

//��������
CGameRoomEx::~CGameRoomEx()
{
	for (INT_PTR i=0;i<m_ShortMessage.GetCount();i++) 
	{
		delete ((CShortMessage *)m_ShortMessage.GetAt(i));
		m_ShortMessage.SetAt(i,NULL);
	}
	m_ShortMessage.RemoveAll();

	if(m_pFindUser !=NULL)
	{
		delete m_pFindUser;
	}

	if(m_pManageDlg != NULL)
		delete m_pManageDlg;

	if(m_pPersonBank != NULL)
	{
		delete m_pPersonBank;
		m_pPersonBank = NULL;
	}

	if (m_pBankWnd)
	{
		m_pBankWnd->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	if (m_pBankVerifyWnd)
	{
		m_pBankVerifyWnd->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	if (m_pBankWndEx)
	{
		m_pBankWndEx->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	delete [] m_pMatchUserInfo;

	if (m_IGameFrame!=NULL)
	{
		//SendSocketMsg(IPC_MAIN_GAMEBASEINFO, IPC_SUB_GAMEBASEINFO, pControlData, uDataSize);
		OutputDebugString("quit:  ~CGameRoomEx() before");
		if (m_IGameFrame) {
			OutputDebugString("quit:  ~CGameRoomEx()");
			m_IGameFrame->OnControlMessage(CM_U3D_APPLICATION_QUIT, 0, 0);
		}
		m_IGameFrame->DeleteFrame();
		m_IGameFrame = NULL;
	}
	if (m_hGameInstance!=NULL)
		::FreeLibrary(m_hGameInstance);

		//�����źŵ�   yjj 090227
	if(img && img->GetFlags()!=0)
    {
        delete img;
    }

	//wushuqun 2009.7.7
	//�޸�bzwcore.bcf���ط�ʽ
	/*if(dwRandWords)
		cfgClose(dwRandWords);*/

	if (m_roomNameFont)
	{
		m_roomNameFont->DeleteObject();
		delete m_roomNameFont;
		m_roomNameFont = NULL;
	}

	if (m_pQueueInRoomDlg)
	{
		delete m_pQueueInRoomDlg;
		m_pQueueInRoomDlg = NULL;
	}
	m_Rgn.DeleteObject();

	g_LevelNameArray.clear();

    if(NULL != m_pGetHelpMoneyDlg)
    {
        delete m_pGetHelpMoneyDlg;
    }
	if (m_pUserDetailsDlg != NULL)
	{
		delete m_pUserDetailsDlg;
		m_pUserDetailsDlg = NULL;
	}

//	m_BzLog.ReleaseBzLog();
}

//���ݰ󶨺���
void CGameRoomEx::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_BTN_ROOM1,m_bt1);
	DDX_Control(pDX, IDC_BTN_ROOM2,m_bt2);
	DDX_Control(pDX, IDC_BTN_ROOM3,m_bt3);
	DDX_Control(pDX, IDC_BTN_ROOM4,m_bt4);
	DDX_Control(pDX, IDC_BTN_ROOM5,m_bt5);
	DDX_Control(pDX, IDC_BTN_ROOM6,m_bt6);
	DDX_Control(pDX, IDC_BTN_ROOM7,m_bt7);
	DDX_Control(pDX, IDC_BTN_ROOM8,m_bt8);
	DDX_Control(pDX, IDC_BTN_ROOM9,m_bt9);
	DDX_Control(pDX, IDC_BTN_ROOM10,m_bt10);
	DDX_Control(pDX, IDC_WEB, m_WebView);

}

//��ʼ������
BOOL CGameRoomEx::OnInitDialog()
{
	__super::OnInitDialog();

	CString _key;
	_key.Format("%d", m_pRoomInfo->pComRoomInfo.uNameID);

    // PengJiLin, 2010-5-18, ��ȡ¼����������
    CBcfFile fBcf(CBcfFile::GetAppPath()+"bzgame.bcf");
    m_byVideoUse = fBcf.GetKeyVal("CustomSet","VideoUse",1);

	int _isgameVideo = fBcf.GetKeyVal("CustomSet", _key, 1);
	m_byVideoUse = (m_byVideoUse != 0 && _isgameVideo == 1);

    // PengJiLin, 2010-6-11, �Ҳ���û��б���Ŀ����ʽ�����á�Ÿ��
    m_byUserListDefineType = fBcf.GetKeyVal("CustomSet", "UserListDefine", 0);

	//��Ӵ���¼��ģ�鲿��  yjj 090223
	CString appPath = CBcfFile::GetAppPath ();
    if(m_byVideoUse != 0)  // PengJiLin, 2010-5-18, ���¼�����ù���
        m_AddRecordModule.CreatRecorMoudle(appPath,true);

	LoadSkin();
	m_bInit = true;
	
	//wushuqun 2009.7.7
	//�޸�bzwcore.bcf���ط�ʽ
	dwRandWords = Glb().m_hRandVirUser;
	
	nFlagMax = 1;

	{
		CString s=CBcfFile::GetAppPath ();/////����·��
		CString strSkin = m_skinmgr.GetSkinBcfFileName();
		CBcfFile f( s + strSkin);
		TCHAR path[MAX_PATH];
		CString skinfolder;
		skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());
		wsprintf(path,"%sDialog\\scroll.bmp",skinfolder);
		HBITMAP hbmScorll = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
		SkinSB_Uninit(m_WebView.GetSafeHwnd());
		SkinSB_Init(m_WebView.GetSafeHwnd(), hbmScorll);
		SkinSB_Uninit(m_DeskFrame.GetSafeHwnd());
		SkinSB_Init(m_DeskFrame.GetSafeHwnd(), hbmScorll);
	}

	return TRUE;
}

//�ػ�����
void CGameRoomEx::OnPaint() 
{
	//--------------------------------------------
	/// ˫�����沿�ִ���
	CPaintDC realdc(this);

   // PengJiLin, 2011-6-23, ������Ϣ
   /* if(FALSE == m_bActive) 
    {
        return;
    }*/

	//CRgn rgn1, rgn2;
	//rgn1.CreateRectRgn(0,0,0,0);
	//rgn1.SetRectRgn(150, 0, 300, 21);
	//rgn2.CreateRectRgn(0,0,0,0);
	//rgn2.SetRectRgn(200, 0, 250, 21);
	//rgn1.CombineRgn(&rgn1, &rgn2, RGN_XOR);
	//realdc.SelectClipRgn(&rgn1, RGN_AND);
	realdc.SelectClipRgn(&m_Rgn, RGN_AND);

	CRect ClientRect, clipRect;
	GetClipBox( realdc.GetSafeHdc(), &clipRect );
	GetClientRect(&ClientRect);
	int iWidth = ClientRect.Width();
	int iHeight = ClientRect.Height();
	//Sleep(500);

	CBitmap BufBmp;
	CDC dc;
	CRect rc;
	GetClientRect(&rc);
	BufBmp.CreateCompatibleBitmap(&realdc, rc.Width(), rc.Height());
	dc.CreateCompatibleDC(&realdc);
	CBitmap *pOldBmp=dc.SelectObject(&BufBmp);
	CFont *currentFont = realdc.GetCurrentFont();
	CFont *oldFont = dc.SelectObject(currentFont);
	//--------------------------------------------
	/// ��ͼ������ʹ��GDIֱ��
	DrawViewFrame(&dc,ClientRect.Width(),ClientRect.Height());
	//--------------------------------------------
	/// ˫������沿�ִ���
	realdc.BitBlt(
		0,
		0,
		ClientRect.Width(), 
		ClientRect.Height(),
		&dc,
		0,
		0,
		SRCCOPY);

	dc.SelectObject(pOldBmp);
	dc.SelectObject(oldFont);

	BufBmp.DeleteObject();
	currentFont->DeleteObject();
	dc.DeleteDC();
	/// ˫������沿�ִ���
	//--------------------------------------------
}

void CGameRoomEx::OnParentMove(int x,int y )
{
	if(m_pQueueInRoomDlg != NULL)
	{
		CRect ClientRect, WindowRect;
		m_DeskFrame.GetClientRect(&ClientRect);
		m_DeskFrame.GetWindowRect(&WindowRect);
		m_pQueueInRoomDlg->MoveWindow(WindowRect.left+ClientRect.Width()/2-90,WindowRect.top+ClientRect.Height()-100,260,100);
	}
}

//ѯ�ʹر�
bool CGameRoomEx::CanCloseNow()
{
	if (m_GameInfo.pMeUserInfo==NULL)
	{
		return true;
	}
	if (m_GameInfo.pMeUserInfo->GameUserInfo.bUserState == USER_WATCH_GAME || m_GameInfo.pMeUserInfo->GameUserInfo.bUserState == USER_SITTING)
	{
		return true;
	}
	if (m_GameInfo.bGameStation < 20)
	{
		return true;
	}
	if (m_GameInfo.bGameStation == 23) 
	{
		return true;
	}
	return false;
}

//�رմ���
bool CGameRoomEx::CloseGameRoom()
{
	if (m_Connect.GetSafeHwnd())
		m_Connect.DestroyWindow();

	///�����Ŷӻ��Ի���
	if (m_pQueueInRoomDlg != NULL)
	{
		if (m_pQueueInRoomDlg->GetSafeHwnd() != NULL)
		{
			m_pQueueInRoomDlg->DestroyDlg();
		}
	}

	///�������жԻ���
	if (m_pPersonBank != NULL)
	{
		m_pPersonBank->DestroyWindow();
		delete m_pPersonBank;
		m_pPersonBank = NULL;
	}

	if (m_pBankWnd)
	{
		m_pBankWnd->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	if (m_pBankVerifyWnd)
	{
		m_pBankVerifyWnd->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	if (m_pBankWndEx)
	{
		m_pBankWndEx->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	/// ���ӽ���
	m_DeskFrame.DestroyWindow();
	m_ToolTip.DestroyWindow();
	CloseSocket(false);

	//�ر���Ϸ����  add by xqm 2010-11-12
	CloseGameClient();

	return true;
}

//λ�ñ仯
bool CGameRoomEx::OnSizeEvent(UINT nSizeType, int cx, int cy)
{
	FixControlStation(cx,cy);
	return true;
}

//�������Ϣ
bool CGameRoomEx::OnSplitEvent(UINT uSplitID, UINT uXPos, UINT uYPos)
{
	//if (uSplitID==IDC_VOR_SPLIT)	//�������Ϣ
	//{
	//	//�ƶ������
	//	CRect vorRect;
	//       m_HorSplitBar.GetClientRect(&vorRect);
	//	CRect ClientRect,HorSplitRect;
	//	GetClientRect(&ClientRect);
	//	m_HorSplitBar.GetClientRect(&HorSplitRect);
	//	m_HorSplitBar.MapWindowPoints(this,&HorSplitRect);
	//	SafeMoveWindow(&m_VorSplitBar,uXPos,m_listy,m_vl.GetWidth(),ClientRect.Height()-m_listy-2);
	//	SafeMoveWindow(&m_HorSplitBar,uXPos+m_vl.GetWidth()+m_lcll.GetWidth(),HorSplitRect.top,ClientRect.Width()-uXPos-m_vl.GetWidth()-m_r.GetWidth()-+m_lcll.GetWidth(),m_hl.GetHeight());

	//	//�ƶ������ؼ�
	//	FixControlStation(ClientRect.Width(),ClientRect.Height());
	//}
	//if (uSplitID==IDC_HOR_SPLIT)
	//{
	//    CRect ClientRect,HorSplitRect;
	//	GetClientRect(&ClientRect);

	//	m_HorSplitBar.GetClientRect(&HorSplitRect);
	//	m_HorSplitBar.MapWindowPoints(this,&HorSplitRect);
	//	SafeMoveWindow(&m_HorSplitBar,HorSplitRect.left,uYPos,ClientRect.right-HorSplitRect.left-m_r.GetWidth()-2 ,m_hl.GetHeight());

	//	FixControlStation(ClientRect.Width(),ClientRect.Height());
	//}
	//if(uSplitID==IDC_HOR_SPLIT2)
	//{
	//    CRect ClientRect,HorSplitRect;
	//	GetClientRect(&ClientRect);

	//	m_HorSplitBar.GetClientRect(&HorSplitRect);
	//	m_HorSplitBar.MapWindowPoints(this,&HorSplitRect);
	//	SafeMoveWindow(&m_HorSplitBar,HorSplitRect.left,uYPos-m_lcl.GetHeight(),ClientRect.right-HorSplitRect.left-m_r.GetWidth()-2 ,m_hl.GetHeight());

	//	FixControlStation(ClientRect.Width(),ClientRect.Height());
	//}
	return true;
}


//������Ϸ�ͻ���
bool CGameRoomEx::StartGameClient(bool bShow)
{
	if (m_GameInfo.uComType == TY_MATCH_GAME && !IsQueueGameRoom()) // PengJiLin, 2010-9-19, �����Ŷӻ���
	{
		if (NULL != m_pRightWnd)
		{
			m_pRightWnd->ShowWindow(SW_HIDE);
		}
	}

	try
	{
		//��ȡ���� 
		if (m_IGameFrame==NULL)
			//if (pCreateInterface == NULL) 
		{
			// ��ȡ��չ�����жϵ�����ICO����EXE
			CString szExtName;
			TCHAR *p = m_GameInfo.szProcessName;
			szExtName.Format(_TEXT("%s"),  p + strlen(p)-3 );
			// ����չ��תΪ��д������Ƚ�
			szExtName.MakeUpper();
			szExtName.CompareNoCase(_TEXT("ICO"));

			if (0==szExtName.CompareNoCase(_TEXT("ICO")))
			{
				BeginInitGoGame * pCreateInterface=(BeginInitGoGame *)GetProcAddress(m_hGameInstance,TEXT("BeginInitGoGame"));
				m_IGameFrame = pCreateInterface();
				
			}
			else if (0==szExtName.CompareNoCase(TEXT("EXE")))
			{
				m_IGameFrame = new CLoveSendClassForEXE();
			}
			else
			{
				m_IGameFrame = NULL;
				return false;
			}
			m_IGameFrame->InitFrame(&m_GameInfo,this);

			//¼������ʼ��������Ϣ  yjj 090223
            if(m_byVideoUse != 0)  // PengJiLin, 2010-5-18, ���¼�����ù���
			    m_AddRecordModule.InitRecordMoudle(&m_GameInfo,m_pRoomInfo->szProcessName,m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation);
		}
		//�������� 
		m_IGameFrame->AFCCreateFrame();

		return true;
	}
	catch (...) {}

	//������� 
	delete m_IGameFrame; 
	m_IGameFrame=NULL; 

	return false;
}

//�ر���Ϸ�ͻ���
bool CGameRoomEx::CloseGameClient()
{
	try
	{
		if (m_IGameFrame!=NULL) 
			m_IGameFrame->AFCCloseFrame();
		return true;
	}
	catch (...) {}
	return false;
}



//�����û���Ϣ
bool CGameRoomEx::SendUserInfo()
{
	//������Ϣ��ʾ
	if (m_Connect.GetSafeHwnd())
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		m_Connect.SetWindowText(fMsg.GetKeyVal("GameRoom","CheckUserID","������֤�û����..."));
	}

	//�����û���¼��Ϣ
	MSG_GR_S_RoomLogon RoomLogon;
	RoomLogon.uGameVer=m_GameInfo.dwGameLSVer;
	RoomLogon.uRoomVer=GAME_PLACE_MAX_VER;
	RoomLogon.uNameID=m_pRoomInfo->pComRoomInfo.uNameID;
	RoomLogon.dwUserID=m_pLogonInfo->dwUserID;
	lstrcpy(RoomLogon.szMD5Pass,m_pLogonInfo->szMD5Pass);
	//���͵�½
	SendData(&RoomLogon,sizeof(RoomLogon),MDM_GR_LOGON,ASS_GR_LOGON_BY_ID,0);

	return true;
}

LRESULT CGameRoomEx::OnShowGameWindow(WPARAM wparam, LPARAM lparam)
{
	if(m_IGameFrame)
		m_IGameFrame->AFCShowFrame((int)wparam);
	return true;
}

//ͬ�����
LRESULT CGameRoomEx::OnReflashMoney(WPARAM wparam, LPARAM lparam)
{
// 	MSG_GR_S_RefalshMoney reflashMoney;
// 	memset(&reflashMoney,0,sizeof(reflashMoney));
// 	if(lparam==0)//ͬ���˷�����
// 	{
// 		reflashMoney.i64Money=g_i64MyMoney;//int(wparam);
// 		reflashMoney.dwUserID=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID;
// 		SendGameData(&reflashMoney,sizeof(reflashMoney),MDM_GR_MONEY,ASS_GR_REFLASH_MONEY,0);
// 	}else if(lparam==1)//ͬ���˷��侭��ֵ
// 	{
// 		reflashMoney.i64Money=int(wparam);
// 		reflashMoney.dwUserID=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID;
// 		SendGameData(&reflashMoney,sizeof(reflashMoney),MDM_GR_MONEY,ASS_GR_REFLASH_MONEY,1);
// 	}else if(lparam==2 && m_GameInfo.uComType == TY_MONEY_GAME)//ͬ����������Ľ��
// 	{
//         g_i64MyMoney = m_GameInfo.pMeUserInfo->GameUserInfo.i64Money;
// 		RoomInfoStruct* pRoomInfo;
// 		if(GetMainRoom()->GetRoomInfoStruct(&pRoomInfo) 
// 			&& int(wparam) < MAX_GAME_ROOM
// 			&& pRoomInfo[int(wparam)].bAccess)//����Ѿ�����ķ�������
// 			pRoomInfo[int(wparam)].pGameRoomWnd->SendMessage(WM_REFLASHMONEY,0x80000001,0);
// 	}else if(lparam==3)//ͬ����������ĺ;���ֵ
// 	{
// 		RoomInfoStruct* pRoomInfo;
// 		if(GetMainRoom()->GetRoomInfoStruct(&pRoomInfo) 
// 			&& int(wparam) < MAX_GAME_ROOM
// 			&& pRoomInfo[int(wparam)].bAccess)//����Ѿ�����ķ�������
// 			pRoomInfo[int(wparam)].pGameRoomWnd->SendMessage(WM_REFLASHMONEY,m_GameInfo.pMeUserInfo->GameUserInfo.dwPoint,1);
// 	}
	return true;
}
//����
LRESULT CGameRoomEx::OnChating(WPARAM wparam, LPARAM lparam)
{
	SetCurrentUser((int)wparam);
	OnSendShortMessage();//	UserItemStruct * pUserItem=m_UserManage.FindOnLineUser((int)wparam);
	//if(pUserItem != NULL)
	//{
	//	SendShortMessage(pUserItem);
	//}
	return true;
}
//�Թ�
LRESULT CGameRoomEx::OnWatch(WPARAM wparam, LPARAM lparam)
{
	UserItemStruct * pDeskUserItem=m_DeskFrame.GetUserItem((BYTE)wparam,(BYTE)lparam);
	if (pDeskUserItem!=NULL)
	{
		return OnLeftHitDesk(wparam,lparam);
	}
	return true;
}
//����
LRESULT CGameRoomEx::OnInvite(WPARAM wparam, LPARAM lparam)
{
	SetCurrentUser((int)wparam);
	OnInvitePlay();
	return true;
}
//�����е������
LRESULT CGameRoomEx::OnHitDesk(WPARAM wparam, LPARAM lparam)
{
	//OnLeftHitDesk(wparam,lparam);
	OnFindNullDesk();
	return true;
}
//�������
LRESULT CGameRoomEx::OnLeftHitDesk(WPARAM wparam, LPARAM lparam)
{
	/// ����������Ϸ�������������ӷ�Χ
	if (wparam == 255 || lparam == 255)
	{
		return 0;
	}
	if ((m_bLogonFinish==false)||(m_bWaitSit==true)) //Ϊ�Ŷӻ����
		return 0;

	if (IsQueueGameRoom())
	{
		if (m_iQueueNo != -1 && m_iQueueStation != -1)
			m_DeskFrame.SetUserItem(m_iQueueNo,m_iQueueStation, NULL);
		m_iQueueStation = (BYTE)lparam;

		if (!IsEnoughMoney())
		{
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
			TCHAR szNum[128]; 
			GetNumString(szNum, m_GameInfo.uLessPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
			int r = DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,true,
				fMsg.GetKeyVal("GameRoom","LackCoins","����Ϸ��������Ҫ��%s�Ľ��,���Ľ�Ҳ���,�쵽Ǯ��ȡ��Ұ�!"),
				szNum);

			if (r == IDOK) NoEnoughMoneyEvent();

			return 0;
		}

		if (m_GameInfo.uMaxPoint != 0L)
		{
			if (m_pRoomInfo->pComRoomInfo.uComType == TY_NORMAL_GAME)
			{
				if (m_GameInfo.pMeUserInfo->GameUserInfo.dwPoint>(int)(m_GameInfo.uMaxPoint))
				{	
					TCHAR szNum[128]; 
					GetNumString(szNum, m_GameInfo.uMaxPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
					CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

					DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,
						fMsg.GetKeyVal("GameRoom","OverflowCoints","���Ľ���Ѷ���%s�������ڴ˷�����Ϸ��"),szNum);

					return 0;
				}
			}
			else if (m_pRoomInfo->pComRoomInfo.uComType == TY_MONEY_GAME)
			{
				if (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money >(int)m_GameInfo.uMaxPoint)
				{
					TCHAR szNum[128]; 
					GetNumString(szNum, m_GameInfo.uMaxPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
					CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

					DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,
						fMsg.GetKeyVal("GameRoom","OverflowCoints","���Ľ���Ѷ���%s�������ڴ˷�����Ϸ��"),szNum);

					return 0;
				}
			}
		}

		m_DeskFrame.SetUserItem(m_iQueueNo,m_iQueueStation,m_GameInfo.pMeUserInfo);

		m_DeskFrame.showQueueBtn(true);
		m_DeskFrame.SetQueueTime(m_iQueueTime);
		SendData(MDM_GR_USER_ACTION,ASS_GR_JOIN_QUEUE,0);
		//m_pQueueInRoomDlg->OnQueue();
		SetTimer(TIME_QUEUEROOM_TIMEOUT, m_iQueueTimeOut * 1000, NULL);
		return 0;
	}

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	//������ҿ�
	if (m_bLastFindDesk!=255) m_DeskFrame.SetFindFrame(255);
	//��ȡ�û�����
	UserItemStruct * pDeskUserItem=m_DeskFrame.GetUserItem((BYTE)wparam,(BYTE)lparam);
	if (pDeskUserItem!=NULL)///�Ѿ�����λ����
	{
		if(pDeskUserItem->GameUserInfo.dwUserID<0) //20081212
		{
			//AFCMessageBox(fMsg.GetKeyVal("GameRoom","NotAllowToSee","��Ǹ,����Ҳ��������Թۣ�"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","NotAllowToSee","��Ǹ,����Ҳ��������Թۣ�"));

			return 0;
		}

		if (pDeskUserItem->GameUserInfo.dwUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
		{
			/// ����Լ����ڵ�λ�ã��뿪��Ϸ��
			if (CanCloseNow()==false)
			{
				//AFCMessageBox(fMsg.GetKeyVal("GameRoom","PlayingNotLeaving","��������Ϸ�У������뿪���ӡ�"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","PlayingNotLeaving","��������Ϸ�У������뿪���ӡ�"));
			}
			else 
			{
				SendData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);

				// add xqm 2010-11-25 ����������Ӻ�, �޸ķ�����밴ť��״̬
				if (!IsQueueGameRoom())
					GetDlgItem(IDC_BTN_ROOM3)->EnableWindow(true);
			}
		}
		else
		{
			if (m_bIsSitting)
			{
				return 0;
			}
			//�Ƿ�����Ϸ��
			if (CanCloseNow()==false)
			{
				//AFCMessageBox(fMsg.GetKeyVal("GameRoom","PlayingNoChangeDesk","��������Ϸ,���ܸ�������."),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","PlayingNoChangeDesk","��������Ϸ,���ܸ�������."));
				return 0;
			}

			//�Ƿ��ڴ�λ���Թ�
			if ((m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==(BYTE)wparam)&&
				(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation==(BYTE)lparam))
			{
				//���ڴ���λ�Թ�,��������//zfx
				//StartGameClient();
				return 0;
			}

			//�Ƿ�����Թ�
			if (CUserPower::CanWatchGame(m_GameInfo.dwGamePower)==false)
			{
				//AFCMessageBox(fMsg.GetKeyVal("GameRoom","NoSeeGame","��Ǹ,�����ڲ����Թ������Ϸ!"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","NoSeeGame","��Ǹ,�����ڲ����Թ������Ϸ!"));
				return 0;
			}

			if(m_pRoomInfo->pComRoomInfo.dwRoomRule & GRR_GAME_U3D)
			{
				/*if (m_GameInfo.uNameID == 20100400)*/
				{
					//AFCMessageBox(fMsg.GetKeyVal("GameRoom","NoSeeGame","��Ǹ,�����ڲ����Թ������Ϸ!"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	                DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","NoSeeGame","��Ǹ,�����ڲ����Թ������Ϸ!"));
					return 0;
				}
			}

			//�������
			if ((m_DeskFrame.IsPlayGame((BYTE)wparam)==false)&&(CUserPower::CanWatchAnyTime(m_GameInfo.dwGamePower)==false)) 
				return 0;

			CString spwd;
			if ((CUserPower::CanThrowPass(m_GameInfo.dwGamePower)==false)&&
				(m_DeskFrame.IsSetPass((BYTE)wparam))&&(m_bPass==FALSE)&&
				((BYTE)wparam!=m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO))
			{
				CPassDesk pdDlg;
				if(pdDlg.DoModal()==IDCANCEL)
				{
					m_bt3.EnableWindow(TRUE);
					return 0;
				}
				spwd=pdDlg.m_pass;
			}

			//StartGameClient();

			//�����Թ���Ϣ
			m_bWaitSit=true;
			MSG_GR_S_UserSit UserWatch;
			memset(&UserWatch,0,sizeof(MSG_GR_S_UserSit));
			UserWatch.bDeskIndex=(BYTE)wparam;
			UserWatch.bDeskStation=(BYTE)lparam;
			strcpy_s(UserWatch.szPassword,spwd);
			SendData(&UserWatch,sizeof(UserWatch),MDM_GR_USER_ACTION,ASS_GR_WATCH_SIT,0);
		}
		return 0;
	}
	if (m_bIsSitting)
	{
		return 0;
	}

	//�ж��Ƿ������Ϸ
	if ((m_GameInfo.uLessPoint!=0L)&&
		(m_GameInfo.pMeUserInfo->GameUserInfo.i64Money<(int)(m_GameInfo.uLessPoint))&&
		m_pRoomInfo->pComRoomInfo.uComType==TY_MONEY_GAME)
	{
		CString strMessage;
		TCHAR szNum[128]; 
        GetNumString(szNum, m_GameInfo.uLessPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);

		int r = DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,true,
			fMsg.GetKeyVal("GameRoom","LackCoins","����Ϸ��������Ҫ��%s�Ľ��,���Ľ�Ҳ���,�쵽Ǯ��ȡ��Ұ�!"),
			szNum);

		if (r == IDOK) NoEnoughMoneyEvent();

		return 0;
	}

	if (m_GameInfo.uMaxPoint != 0L)
	{
		if (m_pRoomInfo->pComRoomInfo.uComType == TY_NORMAL_GAME)
		{
			if (m_GameInfo.pMeUserInfo->GameUserInfo.dwPoint>(int)(m_GameInfo.uMaxPoint))
			{	
				CString strMessage;
				TCHAR szNum[128]; 
				GetNumString(szNum, m_GameInfo.uMaxPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
				CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

				DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,
					fMsg.GetKeyVal("GameRoom","OverflowCoints","���Ľ���Ѷ���%s�������ڴ˷�����Ϸ��"),szNum);

				return 0;
			}
		}
		else if (m_pRoomInfo->pComRoomInfo.uComType == TY_MONEY_GAME)
		{
			if (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money >(int)m_GameInfo.uMaxPoint)
			{
				CString strMessage;
				TCHAR szNum[128]; 
				GetNumString(szNum, m_GameInfo.uMaxPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);

				DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,
					fMsg.GetKeyVal("GameRoom","OverflowCoints","���Ľ���Ѷ���%s�������ڴ˷�����Ϸ��"),szNum);

				return 0;
			}
		}
	}

	//�ж��Ƿ�����λ��
	if (((m_GameInfo.dwRoomRule&GRR_LIMIT_DESK)!=0L)&&(m_DeskFrame.GetMatchInfo((BYTE)wparam,(BYTE)lparam)!=m_pLogonInfo->dwUserID))
	{
		if ((m_bMatchDeskIndex!=255)&&(m_bMatchDeskStation!=255))
		{
			CString strMessage;
			CString str = fMsg.GetKeyVal("GameRoom","WhetherSitDown","ѡ������,���Ĳ���λ���� %d ����Ϸ��,�Ƿ���������?");
			strMessage.Format(str,m_bMatchDeskIndex+1);
			//if (IDOK==AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_YESNO))//MB_ICONQUESTION,this)==IDYES)
			if (IDYES == DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION, m_GameInfo.szGameName,true,strMessage)) 
			{

				//
				PostMessage(WM_LEFT_MOUSE_HIT_DESK,m_bMatchDeskIndex,m_bMatchDeskStation);

				// add xqm 2010-11-25 ����������Ӻ�, �޸ķ�����밴ť��״̬
				if (!IsQueueGameRoom())
					GetDlgItem(IDC_BTN_ROOM3)->EnableWindow(false);
			}
			else	//(----�ر����������δ���----)����Ҳ���OK��ťʱ���� add xqm 2010-11-25
			{
				if (!IsQueueGameRoom())
					GetDlgItem(IDC_BTN_ROOM3)->EnableWindow(true);
				return 0;
			}
		}
		else 
		{
			//AFCMessageBox(fMsg.GetKeyVal("GameRoom","OnlyAllowGamePlayer","������Ϸ��������,ֻ�б���ѡ�ֲ������£�"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","OnlyAllowGamePlayer","������Ϸ��������,ֻ�б���ѡ�ֲ������£�"));
			return 0;
		}
	}


	//������Ϸ
	if (CanCloseNow()==false)
	{
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","PlayingNotLeaving","��������Ϸ��,�����뿪���ӡ�"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","PlayingNotLeaving","��������Ϸ��,�����뿪���ӡ�"));
		return 0;
	}

	//�ж���Ϸ�Ƿ�ʼ
	if (m_DeskFrame.IsPlayGame((BYTE)wparam)==true && m_DeskFrame.GetNullCount((BYTE)wparam) == 0)
	{
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","PlayingNotJoining","��Ϸ�Ѿ���ʼ��,���ܼ�����Ϸ!"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","PlayingNotJoining","��Ϸ�Ѿ���ʼ��,���ܼ�����Ϸ!"));
		return 0;
	}

	//�ж��Ƿ������Ϸ
	if (CUserPower::CanPlayGame(m_GameInfo.dwGamePower)==false)
	{
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","NoAuthentification","��Ǹ,��û����Ϸ��Ȩ��!"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","NoAuthentification","��Ǹ,��û����Ϸ��Ȩ��!"));
		return 0;
	}

	CString spwd="";
	//�ж�����
	if ((CUserPower::CanThrowPass(m_GameInfo.dwGamePower)==false)&&(m_DeskFrame.IsSetPass((BYTE)wparam))/*&&(m_bPass==FALSE)*/)
	{
		CPassDesk pdDlg;
		if(pdDlg.DoModal()==IDCANCEL)
		{
			m_bt3.EnableWindow(TRUE);
			return 0;
		}
			
		spwd=pdDlg.m_pass;
	}


	TCHAR szPath[MAX_PATH] ={0};
	wsprintf(szPath,"%d\\GameOption.bcf",m_GameInfo.uNameID);
	CBcfFile fileBcf(szPath);
	int iPlayingSitNullChair = fileBcf.GetKeyVal("DESK", "PlayingSitNullChair", 1);
	bool bIsClickDeskHasPeople = false;

	//�жϹ���
	if (CUserPower::CanThrowSet(m_GameInfo.dwGamePower)==false&&(m_pRoomInfo->pComRoomInfo.uComType!=TY_MATCH_GAME))
	{
		for (BYTE i=0;i<m_DeskFrame.GetEveryDeskCount();i++)
		{
			UserItemStruct * pOtherDeskUserInfo=m_DeskFrame.GetUserItem((BYTE)wparam,i);
			if (pOtherDeskUserInfo!=NULL)
			{
				UserInfoStruct * pUserInfo=&pOtherDeskUserInfo->GameUserInfo;

				//�жϵ������λ�Ƿ�����
				if (iPlayingSitNullChair == 0)
				{
					if (pUserInfo->bDeskStation == (BYTE)lparam)
					{
						bIsClickDeskHasPeople = true;
					}
				}

				if (m_bLimitPoint)
				{
                    // PengJiLin, 2010-9-14, �����ֱȽϸ�Ϊ��ұȽ�
//                     CString s=CBcfFile::GetAppPath ();//����·��
//                     CBcfFile f( s + "skin.bcf");
//                     int iGold = f.GetKeyVal("GameRoom","UseGoldName",0);
//                     if(iGold > 0)
//                     {
//                         if (pUserInfo->i64Money>m_dwHighPoint || pUserInfo->i64Money<m_dwLowPoint)
//                         {
// 							DUIMessageBox(m_hWnd,MB_ICONWARNING|MB_OK,m_GameInfo.szGameName,true,
// 								fMsg.GetKeyVal("GameRoom","DifferentScore","����Ϸ������ҵĻ��ֺ������õĽ�ҷ�Χ��һ��!"));
// 							GetDlgItem(IDC_BTN_ROOM3)->EnableWindow(TRUE);
// 							return 0;
//                         }
//                     }
//                     else
//                     {
//                         if (pUserInfo->dwPoint>m_dwHighPoint || pUserInfo->dwPoint<m_dwLowPoint)
//                         {
// 							DUIMessageBox(m_hWnd,MB_ICONWARNING|MB_OK,m_GameInfo.szGameName,true,
// 								fMsg.GetKeyVal("GameRoom","DifferentScore","����Ϸ������ҵĻ��ֺ������õĻ��ַ�Χ��һ��!"));
// 							GetDlgItem(IDC_BTN_ROOM3)->EnableWindow(TRUE);
// 							return 0;
//                         }
//                     }
					
				}
				if ((m_bLimitCut)&&(pUserInfo->uCutCount!=0))
				{
					UINT uAllCount=pUserInfo->uWinCount+pUserInfo->uLostCount+pUserInfo->uMidCount;
					UINT uCutPercent=(pUserInfo->uCutCount>0)?(UINT)((double)(pUserInfo->uCutCount*100)/(double)uAllCount):0;
					if (uCutPercent>m_bCutPercent)
					{
						//AFCMessageBox(fMsg.GetKeyVal("GameRoom","EscapeRateTooHight","����Ϸ������ҵ�������̫��!"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	                    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","EscapeRateTooHight","����Ϸ������ҵ�������̫��!"));
						
						GetDlgItem(IDC_BTN_ROOM3)->EnableWindow(TRUE);
						return 0;
					}
				}
				if ((m_bSameIP)&&
					(m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID!=pUserInfo->dwUserID)&&
					(m_GameInfo.pMeUserInfo->GameUserInfo.dwUserIP==pUserInfo->dwUserIP))
				{
					//AFCMessageBox(fMsg.GetKeyVal("GameRoom","SameIP","���� IP ��ַ�ʹ���Ϸ���е�ĳһ�����ͬ!"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	                DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","SameIP","���� IP ��ַ�ʹ���Ϸ���е�ĳһ�����ͬ!"));

					GetDlgItem(IDC_BTN_ROOM3)->EnableWindow(TRUE);
					return 0;
				}
			}
		}
	}

	//��Ϸ��ʼ�󣬿�λ������
	if ( (iPlayingSitNullChair == 0) && (bIsClickDeskHasPeople == false) && ((m_DeskFrame.IsPlayGame((BYTE)wparam))==true) )
	{
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,"��Ǹ,��Ϸ�Ѿ���ʼ����������λ!");
		return 0;
	}

	//����Ԥ����ʾ
	m_bWaitSit=true;
	if ((m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==255)&&(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation==255))
	{
		m_bPreSitDeskIndex=(BYTE)wparam;
		m_bPreSitDeskStation=(BYTE)lparam;
		m_DeskFrame.SetUserItem(m_bPreSitDeskIndex,m_bPreSitDeskStation,m_GameInfo.pMeUserInfo);
	}

	//������Ϸ����
	//StartGameClient();

	//����������Ϣ
	m_bWaitSit=true;
	MSG_GR_S_UserSit UserSit;
	memset(&UserSit,0,sizeof(MSG_GR_S_UserSit));
	UserSit.bDeskIndex=(BYTE)wparam;
	UserSit.bDeskStation=(BYTE)lparam;
	strcpy_s(UserSit.szPassword,spwd);

	m_bIsSitting = true;
	//���淢�����µ���Ϣ
	SendData(&UserSit,sizeof(UserSit),MDM_GR_USER_ACTION,ASS_GR_USER_SIT,0);
	// add xqm ����������Ӻ�, �޸ķ�����밴ť��״̬
	if (!IsQueueGameRoom())
		GetDlgItem(IDC_BTN_ROOM3)->EnableWindow(false);


	//Ϊ��ֹ��Ϸ��ܼ���ʱ����ҵ���˳���������IPCͨѶ�쳣����ʹ�˳�����İ�ť������
	//����Ϸ������ɺ�GameLauncher��֪ͨƽ̨���ٰ��˳����䰴ť���óɿ���
	//Addad by JianGuankun 2012.2.7
	m_bt1.EnableWindow(false);

	SetTimer(TIME_ENABLE_EXIT_BTN,10000,NULL);

	return 0;
}

/// ��ӦVIP�������Ϣ
/// @param wparam �Ƿ�VIP����
/// @return ��
LRESULT CGameRoomEx::OnVIPRoomMsg(WPARAM wparam,LPARAM lparam)
{
	DWORD HandleCode = (DWORD)wparam;

	m_Connect.ShowWindow(SW_HIDE);

	switch (HandleCode)
	{
	case ASS_GR_VIP_NO_PW: ///< VIP���䲻��Ҫ����
		{
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

			if (1&Glb().m_CenterServerPara.m_nFunction) ///< �Ƿ���«2������
			{
				int iVIP = GetIDInformation(m_pLogonInfo->dwUserID,
					Glb().m_CenterServerPara.m_lNomalIDFrom,
					Glb().m_CenterServerPara.m_lNomalIDEnd); ///< ����ID����VIP�ȼ�

				if ((iVIP>0 && iVIP<7) || (iVIP>-9 && iVIP<-4))
				{
					SendUserInfo();
					m_bt5.ShowWindow(SW_SHOW);
					m_RoomOwner = 1;
				}
				else
				{
					//AFCMessageBox(fMsg.GetKeyVal("GameRoom","VIPNOLogon","������VIP��ң��޷�����÷��䡣"));
	                DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("GameRoom","VIPNOLogon","������VIP��ң��޷�����÷��䡣"));

					GetMainRoom()->PostMessage(WM_CLOSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
				}
			}
			break;
		}

	case ASS_GR_VIP_PW: ///< VIP������Ҫ����
		{
			CPassDesk pdDlg;
			INT_PTR re = pdDlg.DoModal();
			if(re == IDCANCEL || pdDlg.m_pass.IsEmpty())
			{
				m_bt3.EnableWindow(TRUE);
				CloseSocket(false);
				GetMainRoom()->PostMessage(WM_CLOSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
				return 0;
			}

			char szVIPPassWord[50] = {0};
			sprintf_s(szVIPPassWord,"%s",pdDlg.m_pass);
			
			MD5_CTX md5;
			md5.MD5Update((unsigned char*)szVIPPassWord,strlen(szVIPPassWord));
			md5.MD5Final((unsigned char*)szVIPPassWord);
			SendData(szVIPPassWord,16,MDM_GR_LOGON,ASS_GR_VIPROOM_PW,0);
			m_bt5.ShowWindow(SW_HIDE);
			m_RoomOwner = 0;
			break;
		}

	case ASS_GR_NO_VIP: ///< ����VIP����
		{
			SendUserInfo();
			break;
		}

	case ASS_GR_VIPROOM_PW_RIGHT:
		{
			SendUserInfo();
			break;
		}

	default:
		break;
	}

	return 0;
}

/// �������뷿����Ϣ
/// @param HandleCode ��Ϣ����
/// @return void
BOOL CGameRoomEx::OnPassWordRoomMsg(DWORD HandleCode)
{
	if (m_GameInfo.dwRoomRule & GRR_CONTEST)
	{
		return FALSE;
	}

	switch (HandleCode)
	{
	case ASS_GR_VIP_NO_PW: ///< VIP���䲻��Ҫ����
		{
			PostMessage(WM_VIPROOM_MSG,ASS_GR_VIP_NO_PW,0);
			return TRUE;
		}

	case ASS_GR_VIP_PW: ///< VIP������Ҫ����
		{
			PostMessage(WM_VIPROOM_MSG,ASS_GR_VIP_PW,0);
			return TRUE;
		}

	case ASS_GR_NO_VIP:
		{
			PostMessage(WM_VIPROOM_MSG,ASS_GR_NO_VIP,0);
			return TRUE;
		}

	case ASS_GR_VIPROOM_PW_RIGHT:
		{
			PostMessage(WM_VIPROOM_MSG,ASS_GR_VIPROOM_PW_RIGHT,0);
			return TRUE;
		}

	default:
		return FALSE;
	}
}

//001,�����½����,һ���뷿��  
bool CGameRoomEx::OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_LOGON_SUCCESS:		//��½�ɹ�
		{
			if (OnPassWordRoomMsg(pNetHead->bHandleCode)) ///< �������뷿����Ϣ
					return true;

			SetGameID(m_GameInfo.uNameID);

			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_LogonResult)) return false;
			MSG_GR_R_LogonResult * pLogonInfo=(MSG_GR_R_LogonResult *)pNetData;

			//������ٵ�  yjj 090227
		    m_NetSignalIndex=0;                        //��0
            m_SavedSingalIndex=0;                //��0
            checkNetSignal();                        //�ȼ��һ��
            SetTimer(ID_CHECK_SIGNAL,CHECK_TIME*2,NULL);        //ÿ6����һ�Σ�CHECK_TIME��ԭ����� 3000 ��ôһ��ֵ

			//��������
			m_GameInfo.dwRoomRule=(long int)pLogonInfo->dwRoomRule;
			m_GameInfo.uLessPoint=pLogonInfo->uLessPoint;
			m_GameInfo.uMaxPoint=pLogonInfo->uMaxPoint;
			m_GameInfo.dwGamePower=pLogonInfo->dwGamePower;
			m_GameInfo.dwMasterPower=pLogonInfo->dwMasterPower;

			m_GameInfo.iBasePoint = pLogonInfo->iBasePoint;
			m_GameInfo.iLowCount = pLogonInfo->iLowCount;
			m_GameInfo.i64LowChip = pLogonInfo->i64LowChip;
			m_GameInfo.i64Chip = pLogonInfo->i64Chip;
			m_GameInfo.i64TimeStart = pLogonInfo->i64TimeStart;
			m_GameInfo.i64TimeEnd = pLogonInfo->i64TimeEnd;
			
			if (CUserPower::CanLookIP(m_GameInfo.dwMasterPower)) 
			{
				if(m_pUserList!=NULL)
					m_pUserList->EnableIPView(true);
			}

			if(m_pInputMessage!=NULL)
				m_pInputMessage->LimitText(CUserPower::CanSendMuch(m_GameInfo.dwGamePower)?MAX_TALK_LEN:NORMAL_TALK_LEN);

			m_GameInfo.pMeUserInfo=m_UserManage.AddGameUser(&pLogonInfo->pUserInfoStruct,SK_ME_USER,CM_NORMAL);
			m_GameInfo.pMeUserInfo->GameUserInfo.dwTax = pLogonInfo->pUserInfoStruct.dwTax;
			
			m_DeskFrame.Invalidate(FALSE);
			
 			m_GameTax = m_GameInfo.pMeUserInfo->GameUserInfo.dwTax;
			m_RoomRule = m_GameInfo.dwRoomRule;
			
			// duanxiaohui 20111103 �û���½����ʱ�ж��Ƿ�Ϊ���鳡(��ѷ�), ���鳡�������и���Ϣ����
			////////////////////////////////////////////////////////////////////////////
			if(GRR_EXPERCISE_ROOM & m_GameInfo.dwRoomRule || m_GameInfo.dwRoomRule & GRR_CONTEST)
			{
				// ѵ���������ֵ��Ϊ������ֵ
				//m_GameInfo.pMeUserInfo->GameUserInfo.i64Money=EXPERCISE_MONEY;
				m_GameInfo.pMeUserInfo->GameUserInfo.i64Money = pLogonInfo->pUserInfoStruct.i64Money;
			}
			else
			{
					//���¸�����Ϣ����Ϣ
					GetMainRoom()->m_pGameListWnd->UpdateMeFortune(pLogonInfo->pUserInfoStruct.i64Money,pLogonInfo->pUserInfoStruct.i64Bank);
					GetMainRoom()->m_pGameListWnd->Invalidate();
			}
			////////////////////////////////////////////////////////////////////////////
			// end duanxiaohui

			if (NULL!=m_pUserList && false == m_pUserList->AddGameUser(m_GameInfo.pMeUserInfo))
			{
				CloseSocket(false);
				GetMainRoom()->PostMessage(WM_CLOSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
				return false;
			}
			m_pUserList->ShowWindow(SW_SHOW); ///< ��ʾ�û��б�


			//			if(m_pFriendList!=NULL)
			//				m_pFriendList->AddGameUser(m_GameInfo.pMeUserInfo);

			SetTimer(TIME_KEEP_ALIVE,5000,NULL);

			SetTimer(TIME_CHECK_MAX_POINT,200,NULL);

			if ( (1<<1) & Glb().m_CenterServerPara.m_nFunction) ///< �ӷ�������ȡ���ã���Ҫ�ͽ�����á�
			{
				if (pLogonInfo->nPresentCoinNum > 0)
				{
					CString str;
					str = fMsg.GetKeyVal("MainRoom","PresentCoin","���������ε�¼ǰ���ۼ�����ʱ�䣬ϵͳ������%s����ҡ�");
					TCHAR szNum[128];
					GetNumString(szNum, pLogonInfo->nPresentCoinNum, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
					CString strTem;
					strTem.Format(str,szNum);

					//AFCMessageBox(strTem);
	                DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,strTem);
				}
			}

			if ( (0x03 == pLogonInfo->pUserInfoStruct.bGameMaster) &&
				((2<<1) & Glb().m_CenterServerPara.m_nFunction) ) ///< ��Ϸ����Ա�������������ҡ�
			{
				bExtendRobot = true;
			}

			m_GameInfo.uVirtualUser = pLogonInfo->nVirtualUser; ///< ����������
			m_pUserList->fillVirtualUser(m_GameInfo.uVirtualUser);

			//��������������п����Ƿ�û�رգ�ǿ�ƹر�
			CPBankWndEx* pBankWndEx = GetMainRoom()->m_pBankWndEx;
			if (GetMainRoom()->m_pBankWndEx)
			{
				pBankWndEx->Close();
				GetMainRoom()->PostMessage(WM_BANK_CLOSE,0,0);
			}

			//����Ǳ����������û�б������߱���δ��ʼ����������ҽ�����Ϸ���䣬
			//������ʾ���ӣ�������Ϸ������Ϣ����ҳ
			//JianGuankun,2012.7.10

			if (pNetHead->bHandleCode == ERR_GR_CONTEST_NOSIGNUP 
				|| pNetHead->bHandleCode == ERR_GR_CONTEST_NOTSTRAT)
			{
				//strMessage = "�뱨�����������ȷ��ǰ��������";

				m_DeskFrame.ShowWindow(SW_HIDE);
				m_WebView.ShowWindow(SW_SHOW);

				RoomInfoStruct* pRoomInfo = NULL;
				if(GetMainRoom()->GetRoomInfoStruct(&pRoomInfo))
				{
					CString str;
					str.Format("%s/manage/matchapply2.aspx?id=%d&userid=%d&token=%s", 
						TEXT(Glb().m_CenterServerPara.m_strWebRootADDR),pRoomInfo->pComRoomInfo.iContestID,
						m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID,Glb().m_TML_SN);
					OutputDebugString(str);
					IE(str);
				}
			}
			else if (pNetHead->bHandleCode == ERR_GR_CONTEST_KICK)
			{	
				//AFCMessageBox("���ź�,�㱻��̭����,��л���Ĳ���!");
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"���ź�,�㱻��̭����,��л���Ĳ���!");

				GetMainRoom()->PostMessage(WM_CLOSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
				return true;
			}
			else
			{
				m_DeskFrame.ShowWindow(SW_SHOW);
				m_WebView.ShowWindow(SW_HIDE);

				if ((m_bEnterAutoQueue && IsQueueGameRoom())  || m_iChangeRooming != 0)
				{
					JoinQueue();
				}
			}

			m_bLoginRoom = true;
			

			return true;
		}
	case ASS_GR_LOGON_ERROR:		//��½ʧ��
		{
			if  (pNetHead->bHandleCode == ERR_GR_IN_OTHER_ROOM)
			{
				MSG_GR_R_OtherRoom * pRoomName=(MSG_GR_R_OtherRoom *)pNetData;
				if (m_pRoomInfo->pRoomItem->m_bIsContest)
				{
					for (int i = 0; i < m_pRoomInfo->pRoomItem->m_RoomContest.size(); i++)
					{
						if (pRoomName->iRoomID == m_pRoomInfo->pRoomItem->m_RoomContest[i].uRoomID)
						{
							RelaodRoom(i);
							return true;
						}
					}	
				}
			}

			//���ÿؼ�
			if (m_Connect.GetSafeHwnd()!=NULL) 
				m_Connect.DestroyWindow();

			///�����Ŷӻ��Ի���
			if (m_pQueueInRoomDlg != NULL)
			{
				if (m_pQueueInRoomDlg->GetSafeHwnd() != NULL)
				{
					m_pQueueInRoomDlg->DestroyDlg();
				}
			}

			pClientSocket->CloseSocket(false);

			//��ʾ������Ϣ
			CString strMessage;
			switch (pNetHead->bHandleCode)
			{
			case ERR_GR_USER_NO_FIND:
			case ERR_GR_USER_PASS_ERROR:
				{
				//	strMessage=TEXT("�����������û����������,��ȷ������������ٴε�½!");
					strMessage = fMsg.GetKeyVal("GameRoom","PWOrUserNameError",
						"�����������û����������,��ȷ������������ٴε�½!");
					break;
				}
			case ERR_GR_USER_VALIDATA:
				{
					//strMessage=TEXT("�����ʺ��ѱ���ֹ������Ϸ!");
				//	strMessage=TEXT("��ʾ����ǿ�˻�������˳����ʺű��йܡ������Է�����Ϸ���Ժ��ٽ���������Ϸ����!����Ȼ��Ч������ϵ��վ�ͷ�!");
					strMessage = fMsg.GetKeyVal("GameRoom","ForceQuit",
						"��ʾ����ǿ�˻�������˳����ʺű��йܡ������Է�����Ϸ���Ժ��ٽ���������Ϸ����!����Ȼ��Ч������ϵ��վ�ͷ�!");
					break;
				}
			case ERR_GR_USER_IP_LIMITED:
				{
				//	strMessage=TEXT("�����ڵ� IP ��ַ����ֹ��¼!");
					strMessage = fMsg.GetKeyVal("GameRoom","ForbidIP","�����ڵ� IP ��ַ����ֹ��¼!");
					break;
				}
			case ERR_GR_IP_NO_ORDER:
				{
				//	strMessage=TEXT("���ʺ���Ҫ��ָ���� IP ��ַ��ϵͳ����!");
					strMessage = fMsg.GetKeyVal("GameRoom","LoginSpecifiedIP","���ʺ���Ҫ��ָ���� IP ��ַ��ϵͳ����!");
					break;
				}
			case ERR_GR_ONLY_MEMBER:
				{
					//wushuqun 2009.6.18
					strMessage.Empty();

					CVipTipDlg vipDlg;
					vipDlg.DoModal();
					//strMessage=TEXT("��Ǹ,�˷���ֻ�����Ա�û�����!");
					break;
				}
			case ERR_GR_MATCH_LOGON:
				{
				//	strMessage=TEXT("��Ǹ,�˷���ֻ��������û�����,���ȱ�������!");
					strMessage = fMsg.GetKeyVal("GameRoom","RoomForGamePlayer","��Ǹ,�˷���ֻ��������û�����,���ȱ�������!");
					break;
				}
			case ERR_GR_IN_OTHER_ROOM:
				{
					MSG_GR_R_OtherRoom * pRoomName=(MSG_GR_R_OtherRoom *)pNetData;
					CString str = fMsg.GetKeyVal("GameRoom","NotAllowEnterThisRoom","���Ѿ���( %s )��Ϸ������,�������ٽ������Ϸ����!");
					strMessage.Format(str,pRoomName->szGameRoomName);
					break;
				}
			case ERR_GR_ACCOUNTS_IN_USE:
				{
				//	strMessage=TEXT("���ʺ�����ʹ����!");
					strMessage = fMsg.GetKeyVal("GameRoom","AccountOnline","���ʺ�����ʹ����!");
					break;
				}
			case ERR_GR_PEOPLE_FULL:
				{
				//	strMessage=TEXT("�÷�������Ա,�볢�Խ���������Ϸ����!");
					strMessage = fMsg.GetKeyVal("GameRoom","FullRoom","�÷�������Ա,�볢�Խ���������Ϸ����!");
					break;
				}
			case ERR_GR_STOP_LOGON:
				{
				//	strMessage=TEXT("��Ϸ�����Ѿ���ͣ����!");
					strMessage = fMsg.GetKeyVal("GameRoom","ForbidEnterRoom","��Ϸ�����Ѿ���ͣ����!");
					break;
				}
			case ERR_GR_TIME_OVER:
				{
				//	strMessage = TEXT("�������ʱ���Ѿ���!���ֵ!");
					strMessage = fMsg.GetKeyVal("GameRoom","FreeTimeOut","�������ʱ���Ѿ���!���ֵ!");
					break;
				}
			//wushuqun 2009.6.5
			case ERR_GR_BATTLEROOM_OUTTIME:
				{
			//		strMessage = TEXT("     �Բ���,����ʱ���ѹ����޷�����÷���!\r\n    ����������������¼��Ϸ�ٷ���վ�鿴���Ϣ��лл!");
					strMessage = fMsg.GetKeyVal("GameRoom","ActiveRoomTimeOut",
						"     �Բ���,����ʱ���ѹ����޷�����÷���!\r\n    ����������������¼��Ϸ�ٷ���վ�鿴���Ϣ��лл!");
					break;
				}
// 			case ERR_GR_CONTEST_NOSIGNUP:
// 				{
// 					strMessage = "�뱨�����������ȷ��ǰ��������";
// 
// 					m_DeskFrame.ShowWindow(SW_HIDE);
// 					m_WebView.ShowWindow(SW_SHOW);
// 
// 					RoomInfoStruct* pRoomInfo = NULL;
// 					if(GetMainRoom()->GetRoomInfoStruct(&pRoomInfo))
// 					{
// 						CString str;
// 						str.Format("%s/manage/matchapply.aspx?id=%d", TEXT(Glb().m_CenterServerPara.m_strWebRootADDR),
// 							pRoomInfo->pComRoomInfo.uContestID);
// 						IE(str);
// 					}
// 
// 					break;
// 				}
			case ERR_GR_CONTEST_BEGUN:
				{
					strMessage = "�����ѿ�ʼ��������û���������ܽ��뷿�䡣";
					break;
				}
			case ERR_GR_CONTEST_TIMEOUT:
				{
					strMessage = "�˳�������ʱ";
					break;
				}
			case ERR_GR_CONTEST_KICK:
				{
					strMessage = "���ź�,�㱻��̭����,��л���Ĳ���!";
					break;
				}
// 			case ERR_GR_CONTEST_NOTSTRAT:
// 				{
// 					strMessage = "������û��ʼ,���ȷ���鿴������Ϣ��";
// 
// 					if (AFCMessageBox(strMessage) == IDOK)
// 					{
// 						//Add By JianGuankun 2012.7.6,������û��ʼ,��MainFrame��ҳ����ʾ������Ϣ;
// 						RoomInfoStruct* pRoomInfo = NULL;
// 						if(GetMainRoom()->GetRoomInfoStruct(&pRoomInfo))
// 						{
// 							CString str;
// 							str.Format("%s/manage/matchapply.aspx?id=%d", TEXT(Glb().m_CenterServerPara.m_strWebRootADDR),
// 								pRoomInfo->pComRoomInfo.uContestID);
// 							GetMainRoom()->IE(str);
// 						}
// 					}
// 
// 					GetMainRoom()->PostMessage(WM_CLOSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
// 					return false;
// 
// 					break;
// 				}
			case ERR_GR_CONTEST_OVER:
				{
					strMessage = "�����Ѿ�����";
					break;
				}
			default:strMessage = fMsg.GetKeyVal("GameRoom","AccountAbnormal","�����ʺ��쳣,����ϵ��Ϸ����Ա!");//strMessage=TEXT("�����ʺ��쳣,����ϵ��Ϸ����Ա!");
			}

			//wushuqun 2009.6.18
			if (! strMessage.IsEmpty())
			{
				//AFCMessageBox(strMessage,m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,strMessage);
			}
			
			CloseSocket(false);

			GetMainRoom()->PostMessage(WM_CLOSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
			//GetMainRoom()->PostMessage(WM_COLSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
			return false;
		}
	case ASS_GR_SEND_FINISH:		//��½���
		{
			//̨����ʾ
			CString GameName;
			GameName.Format("%d",m_GameInfo.uNameID);

			CString strPath = CBcfFile::GetAppPath() + GameName;
			strPath += "\\bzGame.ini";

			CTime tm = CTime::GetCurrentTime();
			CString csCurTime = tm.Format("%Y%m%d");
			UINT uCurTime = atoi(csCurTime);
 
			CString RoomID;
			RoomID.Format("ROOM%d",m_pRoomInfo->pComRoomInfo.uRoomID);

			UINT uLastTime = GetPrivateProfileInt(RoomID, "LastLoginTime", 0, strPath);
			if(uLastTime != uCurTime && m_GameTax && (m_RoomRule & 0x00020000L))
			{	
				CDlgRoomFee RoomFeeDlg(this);						
				//if (m_bIsAsk)
				{
					if(IDOK != RoomFeeDlg.DoModal())
					{
						OnBnClickedClose();
						return true;
					}	
				}
				WritePrivateProfileString(RoomID, "LastLoginTime", csCurTime, strPath);
			}

			//��������
			m_bLogonFinish=true;
			SendUserSetInfo();
			//���ƽ���
			m_DeskFrame.EnableWindow(TRUE);

			////�ټ��� add by wlr 20090717
			//if (m_DeskFrame.IsPlayGame(0))
			//{
			//	m_DeskFrame.m_bShowPlaying = true;
			//	m_DeskFrame.ShowBJLBeginBtn();
			//}
			//end of add by wlr 20090717
			if (m_Connect.GetSafeHwnd()) 
				m_Connect.DestroyWindow();

			bool bIsNotEnoughMoney =  (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money<(int)m_GameInfo.uLessPoint&&m_pRoomInfo->pComRoomInfo.uComType==TY_MONEY_GAME);

			//��̭ʱ�䳡û�н������ JianGuankun 2012.2.20
			if (m_pRoomInfo->pComRoomInfo.dwRoomRule & GRR_CONTEST)
			{
				bIsNotEnoughMoney = false;
			}

			///�Ŷӻ�����,��¼�����ʾ�Ŷӻ��Ի���
			if (IsQueueGameRoom())
			{
				if (m_pQueueInRoomDlg != NULL)
				{
					if (m_pQueueInRoomDlg->GetSafeHwnd() != NULL)
					{
						//m_pQueueInRoomDlg->ShowWindow(bIsNotEnoughMoney?SW_HIDE:SW_SHOW);
						m_pQueueInRoomDlg->ShowWindow(SW_SHOW);
						//m_pQueueInRoomDlg->EnableWindow(bIsNotEnoughMoney?SW_HIDE:SW_SHOW);
					}
					else
					{
						CRect ClientRect;
						m_DeskFrame.GetClientRect(&ClientRect);

						m_pQueueInRoomDlg->Create(IDD_QUEUE_ROOM, this);
						m_pQueueInRoomDlg->SetMessageText(_T("���ŶӲ�����Ϸ��"));

						//���Ŷӻ��Ի��������ڵײ��м�λ�� add by lxl 2011-1-17
						m_pQueueInRoomDlg->MoveWindow(ClientRect.Width()/2-90,ClientRect.Height()-20,260,100);
						//m_pQueueInRoomDlg->ShowWindow(bIsNotEnoughMoney?SW_HIDE:SW_SHOW);
						m_pQueueInRoomDlg->ShowWindow(SW_SHOW);
						//m_pQueueInRoomDlg->EnableWindow(bIsNotEnoughMoney?SW_HIDE:SW_SHOW);
					}
				}
			}

			if(m_pRoomInfo->pComRoomInfo.uComType==TY_MATCH_GAME)
				m_GameInfo.uLessPoint=0;
			//�жϻ���
			if ((m_GameInfo.uLessPoint!=0))
			{
				if (!(m_GameInfo.dwRoomRule & GRR_CONTEST))
				{
					CString strBuffer;
					TCHAR szNum[128], szCur[32];
					GetNumString(szNum, m_GameInfo.uLessPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
					CString str = fMsg.GetKeyVal("GameRoom","NeedCoins","�˷�����Ҫ�������� %s �Ľ��");
					strBuffer.Format(str, szNum);
					m_pRightWnd->m_MessageHandle.InsertSystemMessage(strBuffer.GetBuffer());
					strBuffer.ReleaseBuffer();
				}
				
// 				if (bIsNotEnoughMoney)
// 				{
// 					str = fMsg.GetKeyVal("GameRoom","NeedCoinsRecharge",
// 						"�˷�����Ҫ���� %s �Ľ�Ҳ��ܿ�ʼ��Ϸ,�����ڵĽ����Ϊ%s��\n��������Ǯ��ȡ�����,��ʼ���ʴ̼�����Ϸ��!");
// 					GetNumString(szCur, m_GameInfo.pMeUserInfo->GameUserInfo.dwMoney, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
// 					strBuffer.Format(str,
// 						szNum, szCur);
// 					if(AFCMessageBox(strBuffer,m_GameInfo.szGameName,AFC_YESNO)==IDOK)
// 					{
// 						//wushuqun 2009.7.10 �޸��û����С�ڷ�����ʱ����ʾ��ʽ
// 						if (m_lessPointType == 1)
// 						{
// 							OnBnClickedBt8();
// 						}
// 						else
// 						{
// 							//������վ��ֵҳ��
// 							ShellExecute(NULL,"open",m_strLessPointTipURL,NULL,NULL,SW_MAXIMIZE);
// 						}
// 					}
// 					else	/// �����ͬ��ȡǮ�����˳�����
// 					{
// 						OnBnClickedClose();
// 					}
//				}
			}

			//���ñ���
			UpdateRoomTitle();
			m_DeskFrame.GetMeUserID(m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID);
			//������Ϣ
			if ((m_bMatchDeskIndex!=255)&&(m_bMatchDeskStation!=255))
			{
				CString strMessage;
				CString str = fMsg.GetKeyVal("GameRoom","PlayerSeat","ѡ������,���Ĳ���λ���� %d ����Ϸ��");
				strMessage.Format(str,m_bMatchDeskIndex+1);
				m_pRightWnd->m_MessageHandle.InsertSystemMessage(strMessage.GetBuffer());
				strMessage.ReleaseBuffer();
			}


			//ͬ����Һ;���ֵ
			SyncMoneyAndPoint();

			//����ϵͳ��Ϣ������
			SendMessage(WM_REFLASNEWS,0,0);
			SendMessage(WM_REFLAS_SYS_MSG,0,0);

			if(m_pRoomInfo->pComRoomInfo.uComType==TY_MATCH_GAME || (m_GameInfo.dwRoomRule&GRR_AUTO_SIT_DESK))
			{
                if(!IsQueueGameRoom()) // PengJiLin, 2010-9-19, ���Ŷӻ���
				    OnFindNullDesk();//�Զ�����
			}
#ifdef AI_MACHINE		// ������
			OnFindNullDesk();
#endif
			//ѡ�񷿼��ұ���
			//ChoiseDeskBasePoint();

			m_bLoginRoom = true;

			return true;
		}
	}
	return false;
}

//ѡ�񷿼��ұ���
/*
�û���һ�ν��뷿��������,�����Ի���,
ѡ�����ҵı�����0��1��10��100��200��500��1000��2000��5000��10000��,
���ѡ��0�Ļ�,��Ϊ�澭��ֵ,��Ϸ����ֻ�о���ֵ����Ӯ��
���ѡ������ѡ��,��Ϊ����ѡ�����Ľ����Ϸ,��Ϸ����,ͬʱ�н�Һ;���ֵ����Ӯ,
ԭ����,��Ҳ�����Ϊ����,����ֵ����Ϊ���������û����ر���Ϸ�����ǰ����,
���û�������������Ĭ��Ϊ��һ��ѡ��ı���,ͬʱ�û�Ҳ���Ե�+���������ñ�����ı�����
���û��˳�������ߴ�����,�ٽ�����Ϸ��Ҫ����ѡ��
����ֻ�е�һ���������ӵ�����Ҫ����,����3���˲���Ҫ���á�
�����һ�����˳�����,����������Ҳͬʱ�˳�����
*/
void CGameRoomEx::ChoiseDeskBasePoint()
{
	if(m_GameInfo.uComType == TY_MONEY_GAME)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

		if(!CanCloseNow())
		{
			CString strMessage;
		//	strMessage=TEXT("������Ϸ�޷��ı䱶��!");
			strMessage = fMsg.GetKeyVal("GameRoom","PlayingNotChangeRate","������Ϸ�޷��ı䱶��!");
			
			//AFCMessageBox(strMessage,m_GameInfo.szGameName,MB_ICONQUESTION);//,this);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,strMessage);

			return;
		}
		if(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=255)
		{
			CString strMessage;
		//	strMessage=TEXT("���º��޷��޸ı���!");
			strMessage = fMsg.GetKeyVal("GameRoom","SittingNotChangeRate","���º��޷��޸ı���!");
			
			//AFCMessageBox(strMessage,m_GameInfo.szGameName,MB_ICONQUESTION);//,this);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,strMessage);
			return;
		}

		//��ʾ���ÿ�
		CSetDeskBasePoint setBasePointDlg;
		if(setBasePointDlg.DoModal()==IDCANCEL) return;
		int iBasePoint=0,RadioButtonID=0;
		RadioButtonID=setBasePointDlg.GetBasePoint();//�õ�ѡ��
		if(RadioButtonID==0) goto SendBasePoint;//ȡ��������
		else//�����ַ��͸�������
		{	
			switch (RadioButtonID)
			{
			case IDC_RADIO_0:
				{
					iBasePoint=0;
					break;
				}
			case IDC_RADIO_1:
				{
					iBasePoint=1;
					break;
				}
			case IDC_RADIO_10:
				{
					iBasePoint=10;
					break;
				}
			case IDC_RADIO_100:
				{
					iBasePoint=100;
					break;
				}			
			case IDC_RADIO_200:
				{
					iBasePoint=200;
					break;
				}
			case IDC_RADIO_500:
				{
					iBasePoint=500;
					break;
				}														
			case IDC_RADIO_1000:
				{
					iBasePoint=1000;
					break;
				}														
			case IDC_RADIO_2000:
				{
					iBasePoint=2000;
					break;
				}
			case IDC_RADIO_5000:
				{
					iBasePoint=5000;
					break;
				}
			case IDC_RADIO_10000:
				{
					iBasePoint=10000;
					break;
				}
			default:
				{
					CString strMessage;
				//	strMessage=TEXT("���÷�������,�������Ա��ϵ!");
					strMessage = fMsg.GetKeyVal("GameRoom","ConfigError","���÷�������,�������Ա��ϵ!");
					
				  //AFCMessageBox(strMessage,m_GameInfo.szGameName,MB_ICONQUESTION);//,this);
	               DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,strMessage);
				}
			}
			while(m_GameInfo.pMeUserInfo->GameUserInfo.i64Money<100*iBasePoint) 
			{
				CString strMessage;
				TCHAR szNum[128]; 
                GetNumString(szNum, 100*iBasePoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);

				CString str = fMsg.GetKeyVal("GameRoom","LackCoinsToGet",
					"��Ҫ%s(100�������ñ�����)����,�������Ӳ���,��Ǯ��ȡ������!");
				strMessage.Format(str,szNum);
// 				if(AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_YESNO)==IDOK)
// 				{
// 					//Modify by JianGuankun 2012.1.3 ������������
// 					OnBnClickedBt8();
// 					//AfxMessageBox(TEXT("�ٴμ��������!"));
// 				}else 
				{
				   //AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_CONFIRM);
	               DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,strMessage);

					iBasePoint=0;
					goto SendBasePoint;
				}
			}
SendBasePoint:
			if(iBasePoint>=0)
			{
				MSG_GR_S_SetDeskBasePoint SetTableBasePoint;
				memset(&SetTableBasePoint,0,sizeof(SetTableBasePoint));
				SetTableBasePoint.dwUserID=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID;
				SetTableBasePoint.iDeskBasePoint=iBasePoint;
				SendData(&SetTableBasePoint,sizeof(SetTableBasePoint),MDM_GR_USER_ACTION,ASS_GR_SET_TABLE_BASEPOINT,0);
			}
		}
	}
}

//ͬ������Ľ�Һ;���ֵ
void CGameRoomEx::SyncMoneyAndPoint()
{
	RoomInfoStruct* pRoomInfo;
	if(GetMainRoom()->GetRoomInfoStruct(&pRoomInfo))//����Ѿ�����ķ�������
	{
		bool bMoney=false, bPoint=false;
		UINT uTab;
		if(m_GameInfo.uComType!=TY_MONEY_GAME) bMoney=true;
		for (BYTE i=0;i<MAX_GAME_ROOM;i++)//�ҳ��˷���ı��
		{
			if (pRoomInfo[i].bAccess &&
				(pRoomInfo[i].pComRoomInfo.uRoomID==m_pRoomInfo->pComRoomInfo.uRoomID))
			{
				uTab=i;
				break;
			}
		}
		for (BYTE i=0;i<MAX_GAME_ROOM;i++)//ͬ�����
		{
			if (pRoomInfo[i].bAccess &&
				(pRoomInfo[i].pComRoomInfo.uRoomID!=m_pRoomInfo->pComRoomInfo.uRoomID))//�����˶��ͬ�෿��
			{
				//��Ҫͬ���˷����ң�����������һ�£�
				if(m_GameInfo.uComType == TY_MONEY_GAME && !bMoney &&
					pRoomInfo[i].pComRoomInfo.uComType == TY_MONEY_GAME)
				{
					bMoney=true;
					pRoomInfo[i].pGameRoomWnd->SendMessage(WM_REFLASHMONEY,uTab,2);
				}
				if(pRoomInfo[i].pComRoomInfo.uNameID == m_pRoomInfo->pComRoomInfo.uNameID)
				{
					bPoint=true;
					pRoomInfo[i].pGameRoomWnd->SendMessage(WM_REFLASHMONEY,uTab,3);
				}
				if(bMoney && bPoint) return ;
			}
		}
	}
}





//�����ȡ��Ϣ
bool CGameRoomEx::OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	m_nKeepAliveCount=0;
	//if(m_pUserList->m_hWnd)
	//	m_pUserList->Invalidate();
	switch (pNetHead->bMainID)
	{
	 case MDM_GR_NETSIGNAL:
        {
			if (m_IGameFrame != NULL)
			{
				MSG_GP_NETSignal* pNetMsg = (MSG_GP_NETSignal*)pNetData;
				MSG_GP_Game_NETSignal SingNalMsg;
				SingNalMsg.dwIndex = pNetMsg->dwIndex;
				SingNalMsg.dwTimeFlag = pNetMsg->dwTimeFlag;
				SingNalMsg.dwUserID = pNetMsg->dwUserID;
				SingNalMsg.dwSignalIndex = m_NetSignalIndex;
				m_IGameFrame->OnSocketReadEvent(pNetHead,&SingNalMsg,sizeof(MSG_GP_Game_NETSignal),pClientSocket);
			}
            getNetSignal(pNetData);
            return true;
        }
	case MDM_GR_MONEY:			//Ǯ����,��Ǯ
		{
			return OnAboutMoney(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_CONNECT:			//������Ϣ,׼�����뷿�� 
		{
			return OnConnectMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_LOGON:			//�����½
		{
			return OnLogonMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_USER_LIST:		//�û��б�
		{
			return OnUserListMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_USER_ACTION:	//�û�����
		{
			return OnUserActionMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_ROOM:			//������Ϣ
		{
			return OnRoomMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_MATCH_INFO:		//������Ϣ
		{
			//
			return OnMatchMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_MANAGE:			//������Ϣ
		{

			return OnManageMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_MESSAGE:		//ϵͳ��Ϣ
		{

			return OnSystemMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_POINT:			//�����
		{
			return HandlePointMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
		//2008.3.5wing
	case MDM_GR_PROP:
		{
			return OnPropMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_FRIEND_MANAGE://������Ϣ
		{
			return OnFriendManage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_QUEUE_MSG://�Ŷӵȴ���Ϣ
		{
			return OnQueueMsg(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	//case MDM_GR_VIP_MSG:	//vip��Ϣ
	//	{
	//		return OnVipOperationMsg(pNetHead,pNetData,uDataSize,pClientSocket);
	//	}
	case MDM_GR_DESK://20081212 Fred Huang
		{
			return OnAloneDesk(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_BANK:
		{
			return OnNeBankMsg(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	}

	//��Ϸ��Ϣ
	if (m_IGameFrame!=NULL)
	{
		//��¼�û���Ϣ 090223
        if(m_byVideoUse != 0 && !(m_pRoomInfo->pComRoomInfo.dwRoomRule & GRR_GAME_U3D))  // PengJiLin, 2010-5-18, ���¼�����ù���
		    m_AddRecordModule.AddMsgToRecord(pNetHead,pNetData,uDataSize);

		bool bRet = m_IGameFrame->OnSocketReadEvent(pNetHead,pNetData,uDataSize,pClientSocket);

        // PengJiLin, 2010-9-13, ���˿������߿�����, ʹ��
        if(MDM_GM_GAME_FRAME == pNetHead->bMainID && ASS_GM_USE_KICK_PROP == pNetHead->bAssistantID)
        {
            MSG_GR_RS_KickProp* pKickProp = (MSG_GR_RS_KickProp*)pNetData;

            // ������ʱ���������ʱ��
            long int dwNowTime=(long int)time(NULL);
            SetKickPropTime(pKickProp->iTotalTime+dwNowTime);
            GetMainRoom()->SetKickPropTime(pKickProp->iTotalTime+dwNowTime);

            // ���ٵ�������
            int propCount=Glb().userPropLibrary.GetCount();
            _TAG_USERPROP *userProp=NULL;
            for(int i=0;i<propCount;i++)
            {
                userProp=Glb().userPropLibrary.GetAt(i);
                if(userProp->nPropID == pKickProp->iPropID)
                {
                    userProp->nHoldCount-=1;
                    if(userProp->nHoldCount < 0)userProp->nHoldCount = 0;
                    //if(userProp->nHoldCount<1)    // PengJiLin, 2010-10-14, �µĵ���ϵͳ��������Ϊ0������
                    //{
                    //    Glb().userPropLibrary.RemoveAt(i);
                    //    delete userProp;
                    //    userProp=NULL;
                    //}
                    break;
                }
            }

            // ˢ��
            m_pRoomInfo->pRoomInfoWnd->OnSetPropImage();
        }
        if(MDM_GM_GAME_FRAME == pNetHead->bMainID && ASS_GM_USE_ANTI_KICK_PROP == pNetHead->bAssistantID)
        {
            MSG_GR_RS_KickProp* pKickProp = (MSG_GR_RS_KickProp*)pNetData;

            // ������ʱ���������ʱ��
            long int dwNowTime=(long int)time(NULL);
            SetAntiKickPropTime(pKickProp->iTotalTime+dwNowTime);
            GetMainRoom()->SetAntiKickPropTime(pKickProp->iTotalTime+dwNowTime);

            // ���ٵ�������
            int propCount=Glb().userPropLibrary.GetCount();
            _TAG_USERPROP *userProp=NULL;
            for(int i=0;i<propCount;i++)
            {
                userProp=Glb().userPropLibrary.GetAt(i);
                if(userProp->nPropID == pKickProp->iPropID)
                {
                    userProp->nHoldCount-=1;
                    if(userProp->nHoldCount < 0)userProp->nHoldCount = 0;
                    //if(userProp->nHoldCount<1)    // PengJiLin, 2010-10-14, �µĵ���ϵͳ��������Ϊ0������
                    //{
                    //    Glb().userPropLibrary.RemoveAt(i);
                    //    delete userProp;
                    //    userProp=NULL;
                    //}
                    break;
                }
            }

            // ˢ��
            m_pRoomInfo->pRoomInfoWnd->OnSetPropImage();
        }
        

		//�ж��Ƿ���Ҫ����������û���Ϣ�������Ҫ�������  yjj 090223
        if(m_byVideoUse != 0 && !(m_pRoomInfo->pComRoomInfo.dwRoomRule & GRR_GAME_U3D))  // PengJiLin, 2010-5-18, ���¼�����ù���
        {
		    if (m_AddRecordModule.GetUserGameState(&m_GameInfo,pNetHead))
		    {
			    AddUserInfoToRecord();			
		    }
        }
		
		return bRet;
		
		
	}

	return true;
}




//����λ��
void CGameRoomEx::FixControlStation(int iWidth, int iHeight)
{
	if(!m_bInit)
		return ;
	//LoadSkin();
	//���Ӵ���
	SafeMoveWindow(&m_DeskFrame,
		m_roomlc.GetWidth(),
		m_roomtc.GetHeight(),
		iWidth-m_roomlc.GetWidth()-m_roomrc.GetWidth(),
		iHeight-m_roomtc.GetHeight()-m_roombc.GetHeight()
		);
	/// �趨��Ҫ���Ƶ������Ӵ���������Ҫ����
	m_Rgn.SetRectRgn(0,0,iWidth, iHeight);
		
	CRgn rgn1;
	rgn1.CreateRectRgn(m_roomlc.GetWidth(),
		m_roomtc.GetHeight(),
		iWidth-m_roomrc.GetWidth(),
		iHeight-m_roombc.GetHeight()
		);
	rgn1.CombineRgn(&rgn1, &m_Rgn, RGN_AND);
	m_Rgn.CombineRgn(&rgn1, &m_Rgn, RGN_XOR);
	//DebugPrintf("DeskFrameViewλ�ô�С[%d][%d][%d][%d]",
	//	m_roomlc.GetWidth(),
	//	m_roomtc.GetHeight(),
	//	iWidth-m_roomlc.GetWidth()-m_roomrc.GetWidth(),
	//	iHeight-m_roomtc.GetHeight()-m_roombc.GetHeight()
	//	);

	//ֻ�йر�,����,���밴ť
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("GameRoom");

	CRect btnrect;
	m_bt1.GetClientRect(&btnrect);
	int width = 54;//btnrect.Width();
	int height= 16;//btnrect.Height();
	///////////////////////////////////////////
	//////Kylin 20090122 �����ð�ťλ�ã��Զ���Ӧ��ť��С
	int y = (m_roomtc.GetHeight()-height)/2+f.GetKeyVal(key,"Room_BtnDY",2);
	int x = iWidth-m_iBtnX - width+f.GetKeyVal(key,"Room_BtnDX",0);
	SafeMoveWindow(&m_bt1,x -15,y,width,height+3);
	
	//����
	if (IsBJLRoom())
	{
		m_bt2.ShowWindow(SW_HIDE);
	}
	else
	{
		m_bt2.GetClientRect(&btnrect);
		width=btnrect.Width();
		height=btnrect.Height();
		x = x - width - m_iBtnSpace;
		SafeMoveWindow(&m_bt2,x - 10,y,width,height);
	}

// 	////����
// 	m_bt6.GetClientRect(&btnrect);
// 	width=btnrect.Width();
// 	height=btnrect.Height();
// 	x = x - width - m_iBtnSpace;
// 	SafeMoveWindow(&m_bt6,x,y,width,height);
	m_bt6.ShowWindow(SW_HIDE);

	////�ĳ����У�ԭ����С���ȵ�λ��
// 	m_bt4.GetClientRect(&btnrect);
// 	width=btnrect.Width();
// 	height=btnrect.Height();
// 	x = x - width - m_iBtnSpace;
// 	SafeMoveWindow(&m_bt4,x,y,width,height);
	m_bt4.ShowWindow(SW_HIDE);

	rgn1.DeleteObject();

     // PengJiLin, 2011-4-13, m_bt9, ��ȡ��� ���ܰ�ť
     m_bt9.GetClientRect(&btnrect);
     width=btnrect.Width();
     height=btnrect.Height();
     //x = x - width - m_iBtnSpace;
     SafeMoveWindow(&m_bt9,x,y,width,height);
	 m_bt9.ShowWindow(SW_HIDE);

	///< ������������
	if (0 == m_RoomOwner)
	{
		m_bt5.ShowWindow(SW_HIDE);
	}
	else
	{
		m_bt5.GetClientRect(&btnrect);
		width=btnrect.Width();
		height=btnrect.Height();
		x = x - width - m_iBtnSpace;
		SafeMoveWindow(&m_bt5,x,y,width,height);
		m_bt5.ShowWindow(SW_SHOW);
	}

	/// �Ŷӻ����䲻��ʾ���밴ť
	if (!IsQueueGameRoom())
	{
		//����
		m_bt3.GetClientRect(&btnrect);
		width=btnrect.Width();
		height=btnrect.Height();
		x  = x - width - m_iBtnSpace;
		SafeMoveWindow(&m_bt3,x - 5,y,width,height);
		m_bt3.EnableWindow(TRUE);
		m_bt3.ShowWindow(SW_SHOW);
	}
	else
	{
		m_bt3.EnableWindow(FALSE);
		m_bt3.ShowWindow(SW_HIDE);

		m_bt2.EnableWindow(FALSE);
		m_bt2.ShowWindow(SW_HIDE);

		//if(m_pQueueInRoomDlg != NULL)
		//{
		//	m_pQueueInRoomDlg->MoveWindow(m_pQueueInRoomDlg->m_ParentX-90,m_pQueueInRoomDlg->m_ParentY+5,260,100);
		//}
	}

	/// ��Ч��ť������
	m_bt7.ShowWindow(0);
	m_bt8.ShowWindow(0);
	m_bt10.ShowWindow(0);


	/// ���ǲ��ǰټ������͵���Ϸ
	char cKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "SpecialRule.bcf");
	sprintf(cKey, "%d", m_pRoomInfo->pComRoomInfo.uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), cKey, 0);

	if (iResult)
	{
		m_DeskFrame.ShowBJLBeginBtn();
	}
	else if ((NULL != m_GameInfo.pMeUserInfo)
		&& (m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO != 255)
		&& (m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation != 255)
		&& (m_GameInfo.pMeUserInfo->GameUserInfo.bUserState == USER_SITTING))
	{
		m_DeskFrame.ShowBeginBtn(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO,
			m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation,
			0==(m_GameInfo.dwRoomRule & GRR_ALL_ON_DESK));
	}

	if(m_pQueueInRoomDlg != NULL)
	{
		CRect ClientRect, WindowRect;
		m_DeskFrame.GetClientRect(&ClientRect);
		m_DeskFrame.GetWindowRect(&WindowRect);
		m_pQueueInRoomDlg->MoveWindow(WindowRect.left+ClientRect.Width()/2-90,WindowRect.top+ClientRect.Height()-100,260,100);
	}
	if (m_GameInfo.dwRoomRule & GRR_CONTEST)
	{
		CRect rcRect;
		GetClientRect(&rcRect);
		SafeMoveWindow(&m_WebView,m_roomlc.GetWidth(),
			m_roomtc.GetHeight(),
			iWidth-m_roomlc.GetWidth()-m_roomrc.GetWidth(),
			iHeight-m_roomtc.GetHeight()-m_roombc.GetHeight());
	}
	return;
}


//����������Ϣ
bool CGameRoomEx::OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket)
{
	if (uErrorCode!=0)	//���ӳ��ִ���
	{
		if (m_Connect.GetSafeHwnd())
			m_Connect.DestroyWindow();

		///�����Ŷӻ��Ի���
		if (m_pQueueInRoomDlg != NULL)
		{
			if (m_pQueueInRoomDlg->GetSafeHwnd() != NULL)
			{
				m_pQueueInRoomDlg->DestroyDlg();
			}
		}
	if(m_TCPSocket)
	{
		//AFCMessageBox(m_TCPSocket->TranslateError(uErrorCode),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,m_TCPSocket->TranslateError(uErrorCode));
	}

		CloseSocket(false);
		GetMainRoom()->PostMessage(WM_CLOSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
		return true;
	}
	return true;
}

//����ر���Ϣ
bool CGameRoomEx::OnSocketCloseEvent()
{	
	KillTimer(TIME_KEEP_ALIVE);
	m_nKeepAliveCount=0xFF;	
	m_GameInfo.bGameStation=0;
	if (m_Connect.GetSafeHwnd()!=NULL)
		m_Connect.DestroyWindow();

	///�����Ŷӻ��Ի���
	if (m_pQueueInRoomDlg != NULL)
	{
		if (m_pQueueInRoomDlg->GetSafeHwnd() != NULL)
		{
			m_pQueueInRoomDlg->DestroyDlg();
		}
	}
	
	///�������жԻ���
	if (m_pPersonBank != NULL)
	{
		delete m_pPersonBank;
		m_pPersonBank = NULL;
	}

	if (m_pBankWnd)
	{
		m_pBankWnd->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	if (m_pBankVerifyWnd)
	{
		m_pBankVerifyWnd->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	if (m_pBankWndEx)
	{
		m_pBankWndEx->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	CloseGameClient();
	
	//if(closeType==0)
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	if (m_iChangeRooming != 1)
	{
		OnBnClickedClose();
		AfxGetMainWnd()->PostMessage(QUIT_GAMEROOM, (WPARAM)3, 0);
	}
	m_iChangeRooming = 2;
//	AFCMessageBox(fMsg.GetKeyVal("GameRoom","ConnectionDown","�Բ������������֮��������ж��ˣ�"),
//		fMsg.GetKeyVal("GameRoom","Error","����"));
	//else
	//	MessageBox("�Բ����������֮����������жϡ�\n�����������Ϸ���У������������½��뷿�䣬��Ϸ�����Լ�����\n","��ʾ",MB_ICONINFORMATION);
	return true;
}

//ȡ��������Ϣ
bool CGameRoomEx::OnCancelConnectEvent()
{
	CloseSocket(false);
	GetMainRoom()->PostMessage(WM_CLOSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
	return true;
}


//���鰴ť
void CGameRoomEx::OnBnClickedExpression()
{
	//�������鴰��
	if (m_ExpressWnd.GetSafeHwnd()==NULL) 
		m_ExpressWnd.CreateExpressionWnd(this);

	//�ƶ�����
	CRect ButtonRect;
	CSize ExpressSize;
	CRect ClientRect;
	GetClientRect(&ClientRect);
	m_ExpressWnd.GetFixSize(ExpressSize);
	m_pRightWnd->m_ChatControl.m_BtExpression.GetWindowRect(&ButtonRect);
	SafeMoveWindow(&m_ExpressWnd,max(ButtonRect.left-120,max(0,ClientRect.right-ExpressSize.cx)),ButtonRect.top-ExpressSize.cy,ExpressSize.cx,ExpressSize.cy);
	m_ExpressWnd.ShowWindow(SW_SHOW);
	m_ExpressWnd.SetFocus();
	return;
}

//������Ϣ
LRESULT CGameRoomEx::OnExpression(WPARAM wparam, LPARAM lparam)
{
	CTrueItem * pExpItem=m_ExpressWnd.GetExpressItem((UINT)wparam);
	if (pExpItem!=NULL)
	{
		CString strBuffer;
		m_pInputMessage->GetWindowText(strBuffer);
		strBuffer+=pExpItem->m_szTrancelate;
		m_pInputMessage->SetWindowText(strBuffer);
		m_pInputMessage->SetFocus();
		m_pInputMessage->SetEditSel(strBuffer.GetLength(),strBuffer.GetLength());
	}
	return 0;
}

//��ʼ��ť
LRESULT CGameRoomEx::OnHitDeskBegin(WPARAM wparam,LPARAM lparam)
{
	if(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=255)
	{
		m_DeskFrame.ShowBeginBtn(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO,m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation,false);
		SendData(MDM_GR_USER_ACTION,ASS_GR_USER_HIT_BEGIN,FALSE);

		//wushuqun 2009.9.5
		if(m_IGameFrame !=NULL)
		{
			CM_UserState_Change ControlMessage;
			ControlMessage.bActionCode=ACT_USER_HITDESKBEGIN;
			ControlMessage.bDeskStation=m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
			ControlMessage.pUserItem=m_GameInfo.pMeUserInfo;
			ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
		}
		
	}
	return 0;
}

//�ټ��� add by wlr 20090713
//desc:��ʼ��ť
LRESULT CGameRoomEx::OnHitBJLDeskBegin(WPARAM wparam,LPARAM lparam)
{
	//m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO = 0;
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;

	if ((m_bLogonFinish==false)||(m_bWaitSit==true)) return 0;


	//�ж��Ƿ������Ϸ
	if ((m_GameInfo.uLessPoint!=0L)&&
		(m_GameInfo.pMeUserInfo->GameUserInfo.i64Money<(int)(m_GameInfo.uLessPoint))&&
		m_pRoomInfo->pComRoomInfo.uComType==TY_MONEY_GAME)
	{
		CString strMessage;
		TCHAR szNum[128]; 
        GetNumString(szNum, m_GameInfo.uLessPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
//		strMessage.Format(TEXT("����Ϸ��������Ҫ��%ld�Ľ��,���Ľ�Ҳ���,�쵽Ǯ��ȡ��Ұ�!"),
//			m_GameInfo.uLessPoint);
// 		if(AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_YESNO)==IDOK)
// 		{
// 			//Modify by JianGuankun 2012.1.3 ������������
// 			OnBnClickedBt8();
// 		}
		int r = DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,true,
			fMsg.GetKeyVal("GameRoom","LackCoins","����Ϸ��������Ҫ��%s�Ľ��,���Ľ�Ҳ���,�쵽Ǯ��ȡ��Ұ�!"),
			szNum);

		if (r == IDOK) NoEnoughMoneyEvent();

		return 0;
	}

	if (m_GameInfo.uMaxPoint != 0L)
	{
		if (m_pRoomInfo->pComRoomInfo.uComType == TY_NORMAL_GAME)
		{
			if (m_GameInfo.pMeUserInfo->GameUserInfo.dwPoint>(int)(m_GameInfo.uMaxPoint))
			{
				CString strMessage;
				TCHAR szNum[128]; 
				GetNumString(szNum, m_GameInfo.uMaxPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);

				DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,
					fMsg.GetKeyVal("GameRoom","OverflowCoints","���Ľ���Ѷ���%s�������ڴ˷�����Ϸ��"),szNum);

				return 0;
			}
		}
		else if (m_pRoomInfo->pComRoomInfo.uComType == TY_MONEY_GAME)
		{
			if (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money>(int)(m_GameInfo.uMaxPoint))
			{
				CString strMessage;
				TCHAR szNum[128]; 
				GetNumString(szNum, m_GameInfo.uMaxPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);

				DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,
					fMsg.GetKeyVal("GameRoom","OverflowCoints","���Ľ���Ѷ���%s�������ڴ˷�����Ϸ��"),szNum);

				return 0;
			}
		}
	}


	//�ж��Ƿ�����λ��    -- ��Ҫ�޸��Ӻ���� ramon
	if (((m_GameInfo.dwRoomRule&GRR_LIMIT_DESK)!=0L)&&(m_DeskFrame.GetMatchInfo((BYTE)wparam,(BYTE)lparam)!=m_pLogonInfo->dwUserID))
	{
		if ((m_bMatchDeskIndex!=255)&&(m_bMatchDeskStation!=255))
		{
			CString strMessage;
			str = fMsg.GetKeyVal("GameRoom","WhetherSitDown","ѡ������,���Ĳ���λ���� %d ����Ϸ��,�Ƿ���������?");
			strMessage.Format(str,m_bMatchDeskIndex+1);
			//if (IDOK==AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_YESNO))//MB_ICONQUESTION,this)==IDYES)
			if (IDYES == DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strMessage))
			{
				PostMessage(WM_LEFT_MOUSE_HIT_DESK,m_bMatchDeskIndex,m_bMatchDeskStation);
			}
		}
		else 
		{
			str = fMsg.GetKeyVal("GameRoom","OnlyAllowGamePlayer","������Ϸ��������,ֻ�б���ѡ�ֲ������£�");
			
			//AFCMessageBox(str,m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,str);
			return 0;
		}
	}
	////�ж���Ϸ�Ƿ�ʼ
	//if (m_DeskFrame.IsPlayGame((BYTE)wparam)==true)
	//{
	//	str = fMsg.GetKeyVal("GameRoom","PlayingNotJoining","��Ϸ�Ѿ���ʼ��,���ܼ�����Ϸ!");
	//	AFCMessageBox(str,m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	//	return 0;
	//}

	//�ж��Ƿ������Ϸ
	if (CUserPower::CanPlayGame(m_GameInfo.dwGamePower)==false)
	{
		str = fMsg.GetKeyVal("GameRoom","NoAuthentification","��Ǹ,��û����Ϸ��Ȩ��!");
		
		//AFCMessageBox(str,m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,str);
		return 0;
	}

	if (m_pBankWnd)
	{
		m_pBankWnd->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	if (m_pBankVerifyWnd)
	{
		m_pBankVerifyWnd->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	if (m_pBankWndEx)
	{
		m_pBankWndEx->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	//����Ԥ����ʾ
	m_bWaitSit=true;

	//����������Ϣ
	MSG_GR_S_UserSit UserSit;
	memset(&UserSit,0,sizeof(MSG_GR_S_UserSit));
	UserSit.bDeskIndex=(BYTE)wparam;
	UserSit.bDeskStation=(BYTE)lparam;
	UserSit.szPassword[0]= '\0';

	//���淢�����µ���Ϣ
	SendData(&UserSit,sizeof(UserSit),MDM_GR_USER_ACTION,ASS_GR_USER_SIT,0);

	//Ϊ��ֹ��Ϸ��ܼ���ʱ����ҵ���˳���������IPCͨѶ�쳣����ʹ�˳�����İ�ť������
	//����Ϸ������ɺ�GameLauncher��֪ͨƽ̨���ٰ��˳����䰴ť���óɿ���
	//Addad by JianGuankun 2012.2.7
	m_bt1.EnableWindow(false);

	SetTimer(TIME_ENABLE_EXIT_BTN,10000,NULL);

	return 0;
}
//end of �ټ��� add by wlr 20090713


//�Ŷӻ��뿪��ť
LRESULT CGameRoomEx::OnHitQueueStop(WPARAM wparam,LPARAM lparam)
{
	m_DeskFrame.showQueueBtn(false);
	m_DeskFrame.SetQueueTime(0);
	//m_pQueueInRoomDlg->OnNoQueue();
	KillTimer(TIME_QUEUEROOM_TIMEOUT);
	SendData(MDM_GR_USER_ACTION,ASS_GR_QUIT_QUEUE,0);
	m_DeskFrame.SetUserItem(m_iQueueNo, m_iQueueStation, NULL);
	return 0;
}


//���Ѱ�ť
void CGameRoomEx::OnBnClickedFriend()
{
	//if (m_pFriendList->IsWindowVisible()==FALSE)
	//{
	//	m_pUserList->ShowWindow(SW_HIDE);
	//	m_pFriendList->ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	m_pUserList->ShowWindow(SW_SHOW);
	//	m_pFriendList->ShowWindow(SW_HIDE);
	//}
	return;
}

//���ð�ť
void CGameRoomEx::OnBnClickedRoomSet()
{
	CRoomSetWnd* pRoomSetWnd = new CRoomSetWnd();

	if(!pRoomSetWnd)
	{
		return;
	}

	//Modifed by JianGuankun 20111108
	CGamePlaceDlg* pGamePlace = (CGamePlaceDlg*)AfxGetMainWnd();

	if (!pGamePlace)
	{
		return;
	}

	//��Ϸ����
	pRoomSetWnd->m_bSameIP = m_bSameIP;
	pRoomSetWnd->m_bSameIPNumber = m_bSameIPNumber;
	pRoomSetWnd->m_bPass = m_bPass;
	pRoomSetWnd->m_bLimitCut = m_bLimitCut;
	pRoomSetWnd->m_bLimitPoint = m_bLimitPoint;
	pRoomSetWnd->m_bCutPercent = m_bCutPercent;
	pRoomSetWnd->m_bInvaliMode = m_bInvaliMode;
	pRoomSetWnd->m_dwLowPoint = m_dwLowPoint;
	pRoomSetWnd->m_dwHighPoint = m_dwHighPoint;
	pRoomSetWnd->m_dwLowMoney = m_dwLowMoney;
	pRoomSetWnd->m_dwHighMoney = m_dwHighMoney;
	lstrcpy(pRoomSetWnd->m_szPass,m_szPass);

	//��������
	pRoomSetWnd->m_bSaveTalk = m_bSaveTalk;
	pRoomSetWnd->m_bShowUser = m_bShowUser;
	pRoomSetWnd->m_bShowMessage = m_bShowMessage;
	pRoomSetWnd->m_bPlayBkMusic = !pGamePlace->m_bSndPause;
	pRoomSetWnd->m_bAllowSetDeskPwd = true;

	if(m_pRoomInfo->pComRoomInfo.dwRoomRule & 
		(GRR_QUEUE_GAME | GRR_CONTEST | GRR_MATCH_REG | GRR_EXPERCISE_ROOM) || 
		IsBJLRoom())
	{
		pRoomSetWnd->m_bAllowSetDeskPwd = false;
	}

	pRoomSetWnd->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
	pRoomSetWnd->CenterWindow();
	pRoomSetWnd->ShowModal();

	if (pRoomSetWnd->m_nRetFlag == 1)
	{
		//��Ϸ����
		m_bSameIP = pRoomSetWnd->m_bSameIP;
		m_bSameIPNumber = pRoomSetWnd->m_bSameIPNumber;
		m_bPass = pRoomSetWnd->m_bPass;
		m_bLimitCut = pRoomSetWnd->m_bLimitCut;
		m_bLimitPoint = pRoomSetWnd->m_bLimitPoint;
		if(m_pRoomInfo->pComRoomInfo.uComType == TY_MATCH_GAME)
		{
			m_bSameIP = m_bPass = m_bLimitCut = m_bLimitPoint = false;
		}
		m_bCutPercent = pRoomSetWnd->m_bCutPercent;
		m_bInvaliMode = pRoomSetWnd->m_bInvaliMode;
		m_dwLowPoint = pRoomSetWnd->m_dwLowPoint;
		m_dwHighPoint = pRoomSetWnd->m_dwHighPoint;
		m_dwLowMoney = pRoomSetWnd->m_dwLowMoney;
		m_dwHighMoney = pRoomSetWnd->m_dwHighMoney;
		lstrcpy(m_szPass,pRoomSetWnd->m_szPass);
		m_bSaveTalk = pRoomSetWnd->m_bSaveTalk;
		m_bShowUser = pRoomSetWnd->m_bShowUser;
		m_bShowMessage = pRoomSetWnd->m_bShowMessage;

		//д��ע���
		TCHAR szRoomSetReg[100];
		wsprintf(szRoomSetReg,TEXT("Room%ld"),m_pRoomInfo->pComRoomInfo.uNameID);
		AfxGetApp()->WriteProfileInt(szRoomSetReg,TEXT("Point1"),m_dwLowPoint);
		AfxGetApp()->WriteProfileInt(szRoomSetReg,TEXT("Point2"),m_dwHighPoint);
		AfxGetApp()->WriteProfileInt(szRoomSetReg,TEXT("Money1"),m_dwLowMoney);
		AfxGetApp()->WriteProfileInt(szRoomSetReg,TEXT("Money2"),m_dwHighMoney);
		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("Mode"),m_bInvaliMode);
		AfxGetApp()->WriteProfileInt(szRoomSetReg,TEXT("LimitCut"),m_bLimitCut);
		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("CanSameIP"),m_bSameIP);
		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("SameIPNumber"),m_bSameIPNumber);
		AfxGetApp()->WriteProfileInt(szRoomSetReg,TEXT("LimitPoint"),m_bLimitPoint);
		AfxGetApp()->WriteProfileInt(szRoomSetReg,TEXT("Cut"),m_bCutPercent);
		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("AutoUpdate"),1);
		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("SetMessage"),1);
		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("CanSaveTalk"),m_bSaveTalk);
		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("ChatHead"),1);
		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("ShowUser"),m_bShowUser);
		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("ShowMessage"),m_bShowMessage);

		if (pRoomSetWnd->m_bPlayBkMusic && pGamePlace->m_bSndPause)
		{
			pGamePlace->OnSndPlay();
		}
		else
		{
			pGamePlace->OnSndPause();
		}

		//������·��Ϣ
		SendUserSetInfo();
	}

	delete pRoomSetWnd;
	pRoomSetWnd = NULL;

// 	CRoomSetDlg SetDialog;
// 
// 	//��Ϸ����
// 	SetDialog.m_GameRuleSet.m_bSameIP=m_bSameIP;
// 	SetDialog.m_GameRuleSet.m_bSameIPNumber=m_bSameIPNumber;
// 	SetDialog.m_GameRuleSet.m_bPass=m_bPass;
// 	SetDialog.m_GameRuleSet.m_bLimitCut=m_bLimitCut;
// 	SetDialog.m_GameRuleSet.m_bLimitPoint=m_bLimitPoint;
// 	SetDialog.m_GameRuleSet.m_bCutPercent=m_bCutPercent;
// 	SetDialog.m_GameRuleSet.m_bInvaliMode=m_bInvaliMode;
// 	SetDialog.m_GameRuleSet.m_dwLowPoint=m_dwLowPoint;
// 	SetDialog.m_GameRuleSet.m_dwHighPoint=m_dwHighPoint;
// 	lstrcpy(SetDialog.m_GameRuleSet.m_szPass,m_szPass);
// 
// 	//��������
// 	SetDialog.m_GameRuleSet.m_bSaveTalk=m_bSaveTalk;
// 	SetDialog.m_GameRuleSet.m_bShowUser=m_bShowUser;
// 	SetDialog.m_GameRuleSet.m_bShowMessage=m_bShowMessage;
// 
// 	//��ʾ�Ի���
// 	if (SetDialog.DoModal()==IDOK)
// 	{
// 		//��Ϸ����
// 		m_bSameIP=SetDialog.m_GameRuleSet.m_bSameIP;
// 		m_bSameIPNumber=SetDialog.m_GameRuleSet.m_bSameIPNumber;
// 		m_bPass=SetDialog.m_GameRuleSet.m_bPass;
// 		m_bLimitCut=SetDialog.m_GameRuleSet.m_bLimitCut;
// 		m_bLimitPoint=SetDialog.m_GameRuleSet.m_bLimitPoint;
// 		if(m_pRoomInfo->pComRoomInfo.uComType==TY_MATCH_GAME)
// 		{
// 			m_bSameIP=m_bPass=m_bLimitCut=m_bLimitPoint=false;
// 		}
// 		m_bCutPercent=SetDialog.m_GameRuleSet.m_bCutPercent;
// 		m_bInvaliMode=SetDialog.m_GameRuleSet.m_bInvaliMode;
// 		m_dwLowPoint=SetDialog.m_GameRuleSet.m_dwLowPoint;
// 		m_dwHighPoint=SetDialog.m_GameRuleSet.m_dwHighPoint;
// 		lstrcpy(m_szPass,SetDialog.m_GameRuleSet.m_szPass);
// 		m_bSaveTalk=SetDialog.m_GameRuleSet.m_bSaveTalk;
// 		m_bShowUser=SetDialog.m_GameRuleSet.m_bShowUser;
// 		m_bShowMessage=SetDialog.m_GameRuleSet.m_bShowMessage;
// 
// 		//д��ע���
// 		TCHAR szRoomSetReg[100];
// 		wsprintf(szRoomSetReg,TEXT("Room%ld"),m_pRoomInfo->pComRoomInfo.uNameID);
// 		AfxGetApp()->WriteProfileInt(szRoomSetReg,TEXT("Point1"),m_dwLowPoint);
// 		AfxGetApp()->WriteProfileInt(szRoomSetReg,TEXT("Point2"),m_dwHighPoint);
// 		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("Mode"),m_bInvaliMode);
// 		AfxGetApp()->WriteProfileInt(szRoomSetReg,TEXT("LimitCut"),m_bLimitCut);
// 		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("CanSameIP"),m_bSameIP);
// 		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("SameIPNumber"),m_bSameIPNumber);
// 		AfxGetApp()->WriteProfileInt(szRoomSetReg,TEXT("LimitPoint"),m_bLimitPoint);
// 		AfxGetApp()->WriteProfileInt(szRoomSetReg,TEXT("Cut"),m_bCutPercent);
// 		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("AutoUpdate"),1);
// 		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("SetMessage"),1);
// 		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("CanSaveTalk"),m_bSaveTalk);
// 		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("ChatHead"),1);
// 		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("ShowUser"),m_bShowUser);
// 		AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("ShowMessage"),m_bShowMessage);
// 
// 		//������·��Ϣ
// 		SendUserSetInfo();
// 	}
	return;
}

////��С����ť
//void CGameRoomEx::OnBnClickedMin()
//{
//	::SendMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_SYSCOMMAND,SC_MINIMIZE,0);
//}

//�رհ�ť�����˳����ᱻ�۷�,�Ƿ����ȷ��Ҫ�˳�?
void CGameRoomEx::OnBnClickedClose()
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	try
	{
		if (GetSafeHwnd())
		{
			/// �ж��Ƿ��������Ϸ
			char cKey[10];
			CString sPath=CBcfFile::GetAppPath();
			CBcfFile fsr( sPath + "SpecialRule.bcf");
			sprintf(cKey, "%d", m_GameInfo.uNameID);
			int iResult = fsr.GetKeyVal (_T("BJLType"), cKey, 0);

			if (iResult!=0)
			{
				if(m_IGameFrame)
				{
					// �޸��˳�ʱ������˷���������˳�����Ϣ
					//SendData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
					m_IGameFrame->OnControlMessage(CM_U3D_APPLICATION_QUIT, 0, 0);
					m_IGameFrame->AFCCloseFrame();
					m_IGameFrame = NULL;
					/*m_IGameFrame->AFCCloseFrame();
					m_IGameFrame = NULL;*/
				}

				CloseGameRoom();
				CloseSocket(false);
				GetMainRoom()->PostMessage(WM_CLOSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
				return;
			}

			if (!CanCloseNow())
			{
				//wushuqun 2009.6.30
				//֧�ֶ����йܵ���Ϸ��ʾ��ʽ��ͬ
				CString strTip = fMsg.GetKeyVal("GameRoom","QuitWillDeductMark","�����˳����ᱻ�۷�,�Ƿ����ȷ��Ҫ�˳�?");
				if (::g_global.m_bAllAbet)
				{
					//ȫ��֧�ֶ����й�
					strTip = fMsg.GetKeyVal("GameRoom","QuitWillTookBySystem",
						"�������˳����ᱻϵͳ�й�������Ϸ���Ƿ����Ҫ�˳�?");
				}
				else
				{
					int nArySize = ::g_global.m_cutAutoInSteadAry.GetCount();
					for(int i = 0;i<nArySize;i++)
					{
						if (m_GameInfo.uNameID == ::g_global.m_cutAutoInSteadAry.GetAt(i))
						{
							//�����Ϸ֧�ֶ����й�

							strTip = fMsg.GetKeyVal("GameRoom","QuitWillTookBySystem",
								"�������˳����ᱻϵͳ�й�������Ϸ���Ƿ����Ҫ�˳�?");
							break;
						}
					}
					
				}
				//UINT uCode=AFCMessageBox(TEXT("�����˳����ᱻ�۷�,�Ƿ����ȷ��Ҫ�˳�?"),m_GameInfo.szGameName,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2,this);
		
				//ConfirmDlg confirmDlg(strTip,m_GameInfo.szGameName,AFC_YESNO);
				//UINT uCode = confirmDlg.DoModal();
				UINT uCode = DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strTip);
			//	UINT uCode=AFCMessageBox(TEXT("�����˳����ᱻ�۷�,�Ƿ����ȷ��Ҫ�˳�?"),m_GameInfo.szGameName,AFC_YESNO);
				//if (uCode!=IDOK) 
				if (uCode!=IDYES) 
					return;
				SendData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
				CloseSocket(false); 
			}
			//try{
				if(m_IGameFrame)
				{
					SendData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
					m_IGameFrame->OnControlMessage(CM_U3D_APPLICATION_QUIT, 0, 0);
					m_IGameFrame->AFCCloseFrame();
					m_IGameFrame = NULL;
				}
			//	}

			if(m_pPersonBank != NULL)
			{
				delete m_pPersonBank;
				m_pPersonBank = NULL;
			}

			if (m_pBankWnd)
			{
				m_pBankWnd->Close();
				PostMessage(WM_BANK_CLOSE,0,0);
			}

			if (m_pBankVerifyWnd)
			{
				m_pBankVerifyWnd->Close();
				PostMessage(WM_BANK_CLOSE,0,0);
			}

			if (m_pBankWndEx)
			{
				m_pBankWndEx->Close();
				PostMessage(WM_BANK_CLOSE,0,0);
			}

			if (IsQueueGameRoom())
			{
				//SendData(MDM_GR_USER_ACTION,ASS_GR_QUIT_QUEUE,0);

				if (m_pQueueInRoomDlg != NULL && m_pQueueInRoomDlg->GetSafeHwnd() != NULL)
				{
					m_pQueueInRoomDlg->DestroyDlg();
				}
			}

			CloseSocket(false);

			CloseGameRoom();

			GetMainRoom()->PostMessage(WM_CLOSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
		}
	}
	catch(...)
	{
		//m_IGameFrame = NULL;
	}
	return;
}

/// ����������ͷ�������������Ϣ
/// @param PassWord Ҫ���͵�����
/// @return �ޣ�ֻ��VIP��Ҳ�����������
void CGameRoomEx::SendVIPRoomSetInfo(char * PassWord,int len)
{
	unsigned char BufMD5[16];
	MD5_CTX md5;
	md5.MD5Update((unsigned char*)PassWord,len);
	md5.MD5Final(BufMD5);
	SendData(BufMD5,16,MDM_GR_ROOM,ASS_GR_ROOM_PASSWORD_SET,0);
}

//����������Ϣ
void CGameRoomEx::SendUserSetInfo()
{
	if (IsQueueGameRoom())
	{
		return;
	}

	//���ñ���
	MSG_GR_S_UserSet UserSetInfo;
	memset(&UserSetInfo,0,sizeof(UserSetInfo));
	UserSetInfo.m_Rule.bPass=m_bPass;
	UserSetInfo.m_Rule.bLimitCut=m_bLimitCut;	
	UserSetInfo.m_Rule.bCutPercent=m_bCutPercent;
	UserSetInfo.m_Rule.bLimitPoint = m_bLimitPoint;	

	if (m_GameInfo.uComType == TY_MONEY_GAME)
	{
		UserSetInfo.m_Rule.bLimitPoint &= 0x80;
		UserSetInfo.m_Rule.dwLowPoint=m_dwLowMoney;
		UserSetInfo.m_Rule.dwHighPoint=m_dwHighMoney;
	}
	else if (m_GameInfo.uComType == TY_NORMAL_GAME)
	{
		UserSetInfo.m_Rule.bLimitPoint &= 0x40;
		UserSetInfo.m_Rule.dwLowPoint=m_dwLowPoint;
		UserSetInfo.m_Rule.dwHighPoint=m_dwHighPoint;
	}

	UserSetInfo.m_Rule.bSameIP=m_bSameIP;
	UserSetInfo.m_Rule.bIPLimite=m_bSameIPNumber;
	lstrcpy(UserSetInfo.m_Rule.szPass,m_szPass);

	//������Ϣ
	SendData(&UserSetInfo,sizeof(UserSetInfo),MDM_GR_ROOM,ASS_GR_ROOM_SET,0);

	return;
}

///Ǯ���˸�Ǯ
bool CGameRoomEx::OnAboutMoney(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	if (m_GameInfo.pMeUserInfo == NULL)
	{
		return true;
	}
	
	TCHAR szNum[128];
	/// �����Ϸ�����ˣ����͵���Ϸ�ͻ���

	if (NULL != m_IGameFrame)
	{
		m_IGameFrame->OnSocketReadEvent(pNetHead, pNetData, uDataSize, pClientSocket);
	}

	if (!m_bLoginRoom) //û��½���䲻�ܽ����������
	{
		return true;
	}

	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_TRANSFER_RECORD:	///< ת�ʼ�¼
		{
			//Ч������
			if(uDataSize!=sizeof(MSG_GR_R_TransferRecord_t))
			{
				return false;
			}
			MSG_GR_R_TransferRecord_t *pRecord = (MSG_GR_R_TransferRecord_t *)pNetData;
			if (m_pPersonBank)
			{
				m_pPersonBank->OnReceivedTransferRecord( pRecord );
			}
			if (m_pBankWnd)
			{
				m_pBankWnd->OnReceivedTransferRecord(pRecord);
			}
			if (m_pBankWndEx)
			{
				m_pBankWndEx->OnReceivedTransferRecord(pRecord);
			}
			break;
		}
	case ASS_GR_CHANGE_PASSWD:	///< �޸�����
		{
			/// ֻ��ҪbHandelCode�Ϳ�����
			CString strBuffer;
			CString str;
			if (pNetHead->bHandleCode == ASS_GR_OPEN_WALLET_SUC)
			{
				//BZSoundPlay(this, "music/������ʾ.mp3", 0);
				BZSoundPlay(this, "music/���ȷ����ť������Ϣ��ʾ��.mp3", 0);
				str = fMsg.GetKeyVal("GameRoom","ChangePasswdOK","�޸���������ɹ��������Ʊ��������롣");
				strBuffer.Format(str);
				DUIOkeyMsgBox(m_hWnd,false,strBuffer);
				break;
			}
			else
			{
				BZSoundPlay(this, "music/������ʾ.mp3", 0);
				str = fMsg.GetKeyVal("GameRoom","ChangePasswdFailed","�޸���������ʧ�ܣ�");
			}
			strBuffer.Format(str);
			DUIOkeyMsgBox(m_hWnd,false,strBuffer);
			break;
		}
	case ASS_GR_PAY_MONEY:	///< ֧��Ǯ��
		{
			switch(pNetHead->bHandleCode)
			{
			case 1://�ɹ�
				{
					//Ч������
					if(uDataSize!=sizeof(MSG_GR_S_Pay)) 
						return false;

					MSG_GR_S_Pay* pPayMoney=(MSG_GR_S_Pay*)pNetData;
					//ˢ�½����
					FlushUserMoney(pPayMoney->UserID,pPayMoney->uCount);
					if(pPayMoney->UserID!=m_pLogonInfo->dwUserID) 
						break;//���˵���Ϣ

					break;
				}
			case 0:
				{
					CString strBuffer;
					CString str;
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					str = fMsg.GetKeyVal("GameRoom","LackCoinsToPay","���Ľ�Ҷ���˴�֧��!");
					strBuffer.Format(str);
					
					//AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
	                DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,strBuffer);
					break;
				}
			}
			break;
		}
	case ASS_GR_OPEN_WALLET:	///< ������
		{
			switch (pNetHead->bHandleCode)
			{
			case ASS_GR_OPEN_WALLET_ERROR:
				{
					if (m_Connect.GetSafeHwnd()!=NULL)
						m_Connect.DestroyWindow();

					CString strBuffer;
					CString str;
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					str = fMsg.GetKeyVal("GameRoom","ErrorInGettingMoney","��Ǯ��ʧ��,���Ժ�����!");
					strBuffer.Format(str);
					
					//AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
	                DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,strBuffer);
					break;
				}
			case ASS_GR_OPEN_WALLET_ERROR_PSW:
				{
					if (m_Connect.GetSafeHwnd()!=NULL) 
						m_Connect.DestroyWindow();
					CString strBuffer;
					CString str;
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					str = fMsg.GetKeyVal("GameRoom","L2PWError","�����������,������!");
					strBuffer.Format(str);
					
					//AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
	                DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,strBuffer);
					break;
				}
			case ASS_GR_OPEN_WALLET_SUC:
				{
					//Ч������
					if (m_Connect.GetSafeHwnd()!=NULL)
						m_Connect.DestroyWindow();

					MSG_GR_UserBank * pBank=(MSG_GR_UserBank *)pNetData;
					CString strBuffer;
					if(uDataSize!=sizeof(MSG_GR_UserBank)) 
					{
						CString str;
						BZSoundPlay(this, "music/������ʾ.mp3", 0);
						str = fMsg.GetKeyVal("GameRoom","ErrorInGettingMoney","��Ǯ��ʧ��,���Ժ�����!");
						strBuffer.Format(str);
						
						//AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
	                    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,strBuffer);
					}

					//��������
// 					if (m_pPersonBank == NULL) 
// 					{
// 						try
// 						{
// 							/// ����һ��������Ϊ�Լ��ĶԻ���
// 							m_pPersonBank = new CPBankDlg(this);
// 							m_pPersonBank->Create(IDD_BANK,this);
// 						}
// 						catch (...) { return false; }
// 					}

					GetMainRoom()->m_pGameListWnd->UpdateMeFortune(m_GameInfo.pMeUserInfo->GameUserInfo.i64Money,pBank->i64Bank);
					GetMainRoom()->m_pGameListWnd->Invalidate();

					if (pBank->nVer == 1) //�ɰ�����
					{
						if (m_pBankWnd == NULL)
						{
							m_pBankWnd = new CPBankWnd(this);
							m_pBankWnd->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
							m_pBankWnd->SetWealthInfor(pBank->i64Bank, m_GameInfo.pMeUserInfo->GameUserInfo.i64Money);
							m_pBankWnd->CenterWindow();
							m_pBankWnd->ShowWindow(SW_SHOW);
						}
					}
					
// 					if (m_pPersonBank != NULL)
// 					{	
// 						/// �ȸ�ֵ
// 						m_pPersonBank->SetWealthInfor(pBank->i64Bank, m_GameInfo.pMeUserInfo->GameUserInfo.i64Money);
// 						if (NULL != m_pPersonBank->GetSafeHwnd())
// 						{
// 							/// �����еĳ�Ա��ֵ
// 						//	AfxSetResourceHandle(GetModuleHandle(NULL));
// 						//	m_pPersonBank->DoModal();//Create(IDD_DIALOGBANK,this);
// 							m_pPersonBank->CenterWindow();
// 							m_pPersonBank->ShowWindow(SW_SHOW);
// 						}
// 					}


					if (pBank->nVer == 2) //�°�����
					{
						if (m_pBankWndEx == NULL && m_pBankVerifyWnd == NULL)
						{
							//�ټ������������������򿪴�������
							if (m_IGameFrame != NULL) 
							{
								char cKey[10];
								CBcfFile fsr( CBcfFile::GetAppPath() + "SpecialRule.bcf");
								_stprintf(cKey,"%d",m_GameInfo.uNameID);
								int iResult = fsr.GetKeyVal(_T("BJLType"),cKey,0);
								if (iResult)
								{
									return true;
								}
							}

							int nSelect = AfxGetApp()->GetProfileInt(TEXT("BankSafe"),TEXT("Option"),1);

							int nRetVal = 0;

							if (nSelect == 1 || nSelect == 2)
							{
								nRetVal = 0;
							}
							else if (nSelect == 3)
							{
								if (CMainRoomEx::m_bBankLogoned)
								{
									nRetVal = 1;
								}
							}

							//�ڲ��԰汾�а���Shift�������������֤
							#ifdef MY_BANK_PASSWORD_OFF
								if (::GetKeyState(VK_LSHIFT) & 0x8000)
								{
									nRetVal = 1;
									CMainRoomEx::m_bBankLogoned = true;
								}
							#endif

							if (nRetVal == 0 || !CMainRoomEx::m_bBankLogoned)
							{
								m_pBankVerifyWnd = new CBankVerifyWnd();
								m_pBankVerifyWnd->m_pVerifyInfo = pBank;
								m_pBankVerifyWnd->m_pMyInfo = GetMeUserInfo();
								m_pBankVerifyWnd->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
								m_pBankVerifyWnd->CenterWindow();
								m_pBankVerifyWnd->ShowModal();
								nRetVal = m_pBankVerifyWnd->m_nRetFlag;
								delete m_pBankVerifyWnd;
								m_pBankVerifyWnd = NULL;

								if (nRetVal == 1)
								{
									CMainRoomEx::m_bBankLogoned = true;
								}
							}

							if (nRetVal == 1)
							{
								m_pBankWndEx = new CPBankWndEx(this);
								m_pBankWndEx->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
								m_pBankWndEx->SetWealthInfor(pBank->i64Bank,m_GameInfo.pMeUserInfo->GameUserInfo.i64Money);
								m_pBankWndEx->CenterWindow();
								m_pBankWndEx->ShowWindow(SW_SHOW);
								PostMessage(WM_BANK,2,m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID);
							}
						}
					}

					break;
				}
			}
			break;
		}
	case ASS_GR_CHECK_OUT://ȡǮ
	case ASS_GR_CHECK_OUT_INGAME:	// ��Ϸ��ȡǮҲ��Ҫ���£�������ʾ�Ի���
		{
			if(pNetHead->bHandleCode==ASS_GR_CHECKOUT_ERROR)
			{
				if (ASS_GR_CHECK_OUT == pNetHead->bAssistantID)
				{
					CString strBuffer;
					CString str;
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					str = fMsg.GetKeyVal("GameRoom","NetWorkBusy","ȡ��ʧ�ܣ�����ԭ�������벻��ȷ�����㣡");
					strBuffer.Format(str);
					DUIMessageBox(m_hWnd,MB_ICONERROR|MB_OK,m_GameInfo.szGameName,false,strBuffer);
				}
			}

			if(pNetHead->bHandleCode==ASS_GR_CHECKOUT_ERROR_PLAYING) //��Ϸ�в���ȡǮ add by lxl 2010-12-6
			{
				if (ASS_GR_CHECK_OUT == pNetHead->bAssistantID)
				{
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					CString strBuffer;
					CString str;
					str = fMsg.GetKeyVal("GameRoom","NoDrawForPlaying","��������Ϸ�У�����ȡ�");
					strBuffer.Format(str);
					DUIMessageBox(m_hWnd,MB_ICONERROR|MB_OK,m_GameInfo.szGameName,false,strBuffer);
				}
			}

			if(pNetHead->bHandleCode==ASS_GR_CHECKOUT_SUC)
			{
				//Ч������
				if (uDataSize!=sizeof(MSG_GR_CheckMoney)) return false;
				MSG_GR_CheckMoney * pCheckOut=(MSG_GR_CheckMoney *)pNetData;
				if(pCheckOut->dwUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)//�Լ�
				{
					//���¶Ի���
					if (m_pPersonBank!=NULL) 
					{
						if (m_pPersonBank->GetSafeHwnd()!=NULL) 
						{
							m_pPersonBank->UpdateUserMoney(0, pCheckOut->i64CheckOut);
						}
					}

					if (m_pBankWnd)
					{
						m_pBankWnd->UpdateUserMoney(0, pCheckOut->i64CheckOut);
					}

					//���½����Ϣ������ǵ�ǰ�������Ϸ�������̸���
					if (pCheckOut->uGameID == m_GameInfo.uNameID)
					{
						FlushUserMoney(pCheckOut->dwUserID,pCheckOut->i64CheckOut);
					}
					else
					{
						//���ǵ�ǰ���䣬��֪ͨ���з�������н������ͬ��
						UserItemStruct* pUserItem=m_UserManage.FindOnLineUser(pCheckOut->dwUserID);
						if(pUserItem)
						{
							pUserItem->GameUserInfo.i64Bank -= pCheckOut->i64CheckOut;
							GetMainRoom()->UpdateBankMoneyToAllRoom(pUserItem->GameUserInfo.i64Bank,this);
						}
					}
	
					//���ȡǮ���㹻Ǯ����Ϸ���򡰿��ٽ��롱��ťҪ����
					if (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money >= m_GameInfo.uLessPoint)
					{
						m_bt3.EnableWindow(TRUE);
					}
				}
				else
				{
					//���½����Ϣ������ǵ�ǰ�������Ϸ�������̸���
					UserItemStruct* pUserItem=m_UserManage.FindOnLineUser(pCheckOut->dwUserID);
					if (pCheckOut->uGameID == m_GameInfo.uNameID || pUserItem->GameUserInfo.isVirtual)
					{
						FlushUserMoney(pCheckOut->dwUserID,pCheckOut->i64CheckOut);
					}
				}

				if(m_IGameFrame == NULL) //û�д���Ϸ
				{
					bool bIsNotEnoughMoney =  (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money<(int)m_GameInfo.uLessPoint&&m_pRoomInfo->pComRoomInfo.uComType==TY_MONEY_GAME);
					///�Ŷӻ�����,��¼�����ʾ�Ŷӻ��Ի���
					if (IsQueueGameRoom())
					{
						if (m_pQueueInRoomDlg != NULL)
						{
							if (m_pQueueInRoomDlg->GetSafeHwnd() != NULL)
							{
								m_pQueueInRoomDlg->ShowWindow(bIsNotEnoughMoney?SW_HIDE:SW_SHOW);
							}
							else
							{
								CRect ClientRect;
								m_DeskFrame.GetClientRect(&ClientRect);

								m_pQueueInRoomDlg->Create(IDD_QUEUE_ROOM, this);
								m_pQueueInRoomDlg->SetMessageText(_T("���ŶӲ�����Ϸ��"));

								//���Ŷӻ��Ի��������ڵײ��м�λ�� add by lxl 2011-1-17
								m_pQueueInRoomDlg->MoveWindow(ClientRect.Width()/2-90,ClientRect.Height()-20,260,100);
								m_pQueueInRoomDlg->ShowWindow(bIsNotEnoughMoney?SW_HIDE:SW_SHOW);
							}		

						}
					}
				} // end if
				
			}
			break;
		}
	case ASS_GR_CHECK_IN://��Ǯ
	case ASS_GR_CHECK_IN_INGAME://��Ϸ�д�Ǯ
		{
			if(pNetHead->bHandleCode==ASS_GR_CHECKIN_ERROR)
			{
				if (ASS_GR_CHECK_IN == pNetHead->bAssistantID)
				{
					CString strBuffer;
					CString str;
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					str = fMsg.GetKeyVal("GameRoom","NetWorkBusySave","����ӵ��,����ʧ��!���Ժ�����!");
					strBuffer.Format(str);
					DUIMessageBox(m_hWnd,MB_ICONERROR|MB_OK,m_GameInfo.szGameName,false,strBuffer);
				}
			}
			if(pNetHead->bHandleCode==ASS_GR_CHECKIN_ERROR_PLAYING)
			{
				if (ASS_GR_CHECK_IN == pNetHead->bAssistantID)
				{
					CString strBuffer;
					CString str;
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					str = fMsg.GetKeyVal("GameRoom","NoSaveForPlaying","��������Ϸ�У����ܴ�");
					strBuffer.Format(str);
					DUIMessageBox(m_hWnd,MB_ICONERROR|MB_OK,m_GameInfo.szGameName,false,strBuffer);
				}
			}

			if(pNetHead->bHandleCode==ASS_GR_CHECKIN_ZHUANGJIA)  //�ټ�������Ϸ ��ׯ�в��ܴ�� add by lxl 2010-11-5
			{
				if (ASS_GR_CHECK_IN == pNetHead->bAssistantID)
				{
					CString strBuffer;
					CString str;
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					str = fMsg.GetKeyVal("GameRoom","NoSaveForZhuangjia","��������ׯ�У����ܴ�");
					strBuffer.Format(str);
					DUIMessageBox(m_hWnd,MB_ICONERROR|MB_OK,m_GameInfo.szGameName,false,strBuffer);
				}
			}

			if(pNetHead->bHandleCode==ASS_GR_CHECKIN_SUC)
			{
				//Ч������
				if (uDataSize!=sizeof(MSG_GR_CheckMoney)) return false;
				MSG_GR_CheckMoney * pCheckIn=(MSG_GR_CheckMoney *)pNetData;

				if(pCheckIn->dwUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)//�Լ�
				{
					//���¶Ի���
					if (m_pPersonBank != NULL)
					{
						if (m_pPersonBank->GetSafeHwnd() != NULL) 
						{
							m_pPersonBank->UpdateUserMoney(1, pCheckIn->i64CheckOut);
						}
					}

					if (m_pBankWnd)
					{
						m_pBankWnd->UpdateUserMoney(1, pCheckIn->i64CheckOut);
					}

					//���½����Ϣ������ǵ�ǰ�������Ϸ�������̸���
					if (pCheckIn->uGameID == m_GameInfo.uNameID)
					{
						FlushUserMoney(pCheckIn->dwUserID,0-pCheckIn->i64CheckOut);
					}
					else
					{
						//���ǵ�ǰ���䣬��֪ͨ���з�������н������ͬ��
						UserItemStruct* pUserItem=m_UserManage.FindOnLineUser(pCheckIn->dwUserID);
						if(pUserItem)
						{
							pUserItem->GameUserInfo.i64Bank += pCheckIn->i64CheckOut;
							GetMainRoom()->UpdateBankMoneyToAllRoom(pUserItem->GameUserInfo.i64Bank,this);
						}
					}
				}
				else
				{
					//���½����Ϣ������ǵ�ǰ�������Ϸ�������̸���
					UserItemStruct* pUserItem=m_UserManage.FindOnLineUser(pCheckIn->dwUserID);
					if (pCheckIn->uGameID == m_GameInfo.uNameID || pUserItem->GameUserInfo.isVirtual)
					{
						FlushUserMoney(pCheckIn->dwUserID,0-pCheckIn->i64CheckOut);
					}
				}

				if(m_IGameFrame == NULL) //û�д���Ϸ
				{
					bool bIsNotEnoughMoney =  (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money<(int)m_GameInfo.uLessPoint&&m_pRoomInfo->pComRoomInfo.uComType==TY_MONEY_GAME);
					///�Ŷӻ�����,��¼�����ʾ�Ŷӻ��Ի���
					if (IsQueueGameRoom())
					{
						if (m_pQueueInRoomDlg != NULL)
						{
							if (m_pQueueInRoomDlg->GetSafeHwnd() != NULL)
							{
								m_pQueueInRoomDlg->ShowWindow(bIsNotEnoughMoney?SW_HIDE:SW_SHOW);
							}
							else
							{
								CRect ClientRect;
								m_DeskFrame.GetClientRect(&ClientRect);

								m_pQueueInRoomDlg->Create(IDD_QUEUE_ROOM, this);
								m_pQueueInRoomDlg->SetMessageText(_T("���ŶӲ�����Ϸ��"));

								//���Ŷӻ��Ի��������ڵײ��м�λ�� add by lxl 2011-1-17
								m_pQueueInRoomDlg->MoveWindow(ClientRect.Width()/2-90,ClientRect.Height()-20,260,100);
								m_pQueueInRoomDlg->ShowWindow(bIsNotEnoughMoney?SW_HIDE:SW_SHOW);
							}
						}
					}
				} // end if
			}
			break;
		}
	case ASS_GR_TRANSFER_MONEY:		   //ת��
	case ASS_GR_TRANSFER_MONEY_INGAME: //��Ϸ��ת��
		{
			CString strBuffer;
			CString str;
			TCHAR szNum[128], szBank[128];
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_S_TransferMoney)) return false;
			MSG_GR_S_TransferMoney * pTransfer=(MSG_GR_S_TransferMoney *)pNetData;
			/// ���ת�ʲ��ɹ���������ƽ̨������ת��ָ��򵯳��Ի���������Ϸ�з���ָ���ƽֻ̨��Ӧ�Ի����¼�
			
			if(ASS_GR_TRANSFER_MONEY_INGAME == pNetHead->bAssistantID)
			{
				switch(pNetHead->bHandleCode)
				{
				case ASS_GR_TRANSFER_SUC:	///< ת�ʳɹ�					
					///Added by xqm 20101109
					///��Z����������ת�ʳɹ�����Ϣ,Ȼ����Z��������ת��Ŀ��ͻ��˷���һ��ת�ʹ�������Ϣ
					{
						OutputDebugString("-------ƽ̨���ͻ����յ��û�ת�ʳɹ�����Ϣ��-----------");
						GetMainRoom()->SendData(pNetData, uDataSize, MDM_GR_MONEY, ASS_GR_TRANSFER_MONEY, ASS_GR_TRANSFER_SUC);
					}
					break;
				}
			}

			if (ASS_GR_TRANSFER_MONEY == pNetHead->bAssistantID)
			{
				switch(pNetHead->bHandleCode)
				{
				case ASS_GR_TRANSFER_SUC:	///< ת�ʳɹ�					
					{
						///Added by xqm 20101109
						///��Z����������ת�ʳɹ�����Ϣ,Ȼ����Z��������ת��Ŀ��ͻ��˷���һ��ת�ʹ�������Ϣ
						GetMainRoom()->SendData(pNetData, uDataSize, MDM_GR_MONEY, ASS_GR_TRANSFER_MONEY, ASS_GR_TRANSFER_SUC);
						break;
					}
				case ASS_GR_PASSWORD_ERROR:												///< ת���������
					{
						BZSoundPlay(this, "music/������ʾ.mp3", 0);
						str = fMsg.GetKeyVal("GameRoom","TransferPassWdError","�����������û��ͨ����֤���������������룡");
						strBuffer.Format(str, szNum);
						DUIMessageBox(m_hWnd,MB_OK|MB_ICONERROR,m_GameInfo.szGameName,false,strBuffer);
						return true;
					}
				case ASS_GR_TRANSFER_TOTAL_LESS:										///< �����ܶ�̫С�������ʸ�
					{
						BZSoundPlay(this, "music/������ʾ.mp3", 0);
						GetNumString(szNum, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
						str = fMsg.GetKeyVal("GameRoom","TransferTotalLess","����ʱ��������ת�ʣ����д��ﵽ�򳬹�%s��Ҳſ���ת�ʣ�");
						strBuffer.Format(str, szNum);
						DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,m_GameInfo.szGameName,false,strBuffer);
						return true;
					}
					break;
				case ASS_GR_TRANSFER_TOO_LESS:											///< ����ת����Ŀ̫��
					{
						BZSoundPlay(this, "music/������ʾ.mp3", 0);
						GetNumString(szNum, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
						str = fMsg.GetKeyVal("GameRoom","TransferTooLess","ת�ʲ��ɹ�������ת�ʽ��������%s��ҡ�");
						strBuffer.Format(str, szNum);
						DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,m_GameInfo.szGameName,false,strBuffer);
						return true;
					}
					break;
				case ASS_GR_TRANSFER_MULTIPLE:											///< ����ת����Ŀ������ĳ���ı���
					{
						BZSoundPlay(this, "music/������ʾ.mp3", 0);
						GetNumString(szNum, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
						str = fMsg.GetKeyVal("GameRoom","TransferMultiple","ת�ʲ��ɹ���ת�ʽ�������%s����������");
						strBuffer.Format(str, szNum);
						DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,m_GameInfo.szGameName,false,strBuffer);
						return true;
					}
				case ASS_GR_TRANSFER_NOT_ENOUGH:										///< ���н�������ת��
					{
						BZSoundPlay(this, "music/������ʾ.mp3", 0);
						GetNumString(szNum, pTransfer->i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
						GetNumString(szBank, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
						str = fMsg.GetKeyVal("GameRoom","TransferNotEnoughMoney","ת�ʲ��ɹ�����������ֻ��%s��ң�����֧������ת��%s��ҡ�");
						strBuffer.Format(str, szBank, szNum);
						DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strBuffer);
						return true;
					}
				case ASS_GR_TRANSFER_TOO_MANY_TIME:
					{
						BZSoundPlay(this, "music/������ʾ.mp3", 0);
						str = fMsg.GetKeyVal("GameRoom","TransferTooMany","������ת�˵�����ﵽ������޶�! ��������СһЩ����ֵ����ת�����ԡ�"); // PengJiLin, 2010-8-25
						strBuffer.Format(str);
						DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strBuffer);
						return true;
					}
				case ASS_GR_TRANSFER_NO_DEST:											///< ת��Ŀ�겻����
					{
						BZSoundPlay(this, "music/������ʾ.mp3", 0);
						str = fMsg.GetKeyVal("GameRoom","AccountNotExist","ת��Ŀ�겻����,���ʵ�ʺ�����!");
						strBuffer.Format(str);
						DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,m_GameInfo.szGameName,false,strBuffer);
						return true;
					}
					break;
				default:
					{
						CString strBuffer;
						CString str;
						BZSoundPlay(this, "music/������ʾ.mp3", 0);
						str = fMsg.GetKeyVal("GameRoom","NetWorkBusySave","����ӵ��,����ʧ��!���Ժ�����!");
						strBuffer.Format(str);
						DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,m_GameInfo.szGameName,false,strBuffer);
						return true;
					}
				}
			}
			/// ת�ʳɹ������ô�����Һ����н�����
			if(pNetHead->bHandleCode==ASS_GR_TRANSFER_SUC)
			{

				TCHAR szAct[128];
				CString strBuffer;
				CString str;
				GetNumString(szNum, pTransfer->i64Money, Glb().m_nPowerOfGold/*, false*/,Glb().m_bUseSpace, Glb().m_strSpaceChar);
				GetNumString(szAct, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold/*, false*/,Glb().m_bUseSpace, Glb().m_strSpaceChar);
				CMainRoomEx * pMainRoom = GetMainRoom();
				/// ���Լ�ת������
				if(pTransfer->UserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)//�Լ�
				{
					if (ASS_GR_TRANSFER_MONEY == pNetHead->bAssistantID)
					{
						//��ʾ�û�
						if(pTransfer->bUseDestID)
						{
							str = fMsg.GetKeyVal("GameRoom","TransferCoinsId",
								"<p 5>����֪ͨ�����Ѹ��û� <b>%s</b>(%d) ת�ʡ�<n><y 30>      ת�����<b>%s</b> ���</y><n>������ʵ�ʵ��ˣ�<b>%s</b> ���</p>");
							strBuffer.Format(str, pTransfer->szDestNickName, pTransfer->destUserID,szNum, szAct);
							
						}
						else
						{
							str = fMsg.GetKeyVal("GameRoom","TransferCoinsNn",
								"<p 5>����֪ͨ�����Ѹ��û� <b>%s</b> ת�ʡ�<n><y 30>      ת�����<b>%s</b> ���</y><n>������ʵ�ʵ��ˣ�<b>%s</b> ���</p>");
							strBuffer.Format(str, pTransfer->szDestNickName, szNum, szAct);
						}

						BZSoundPlay(this, "music/���ȷ����ť������Ϣ��ʾ��.mp3", 0);
						//AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
						DUIMessageBox(AfxGetMainWnd()->m_hWnd,MB_OK|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strBuffer);
						
					}
					//���¶Ի���
					if (m_pPersonBank!=NULL) 
					{
						m_pPersonBank->UpdateUserMoneyByTransfer(0, pTransfer->i64Money);
					}
					if (m_pBankWnd)
					{
						m_pBankWnd->UpdateUserMoneyByTransfer(0, pTransfer->i64Money);
					}

					if (m_pBankWndEx)
					{
						m_pBankWndEx->UpdateUserMoneyByTransfer(0,pTransfer->i64Money);
					}

					pMainRoom->m_PlaceUserInfo.i64Bank-=pTransfer->i64Money;
					pMainRoom->UpdateMeInfo(0, false/*true*/, 0, false);         // PengJiLin, 2010-10-9, ��������ʾΪ0
				}
			}
			break;
		}
	case ASS_GR_SENDMONEY_TIMES: 
		{
		    SendMoneyOnTimesAndCounts*  pSendResult = (SendMoneyOnTimesAndCounts*)pNetData;

            UserItemStruct* pUserInfo = NULL;
            BOOL bUserIsMe = FALSE;
            if(pSendResult->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
            {
                pUserInfo = m_GameInfo.pMeUserInfo;
                bUserIsMe = TRUE;
            }
            else
            {
                pUserInfo = m_UserManage.FindOnLineUser(pSendResult->dwUserID);
            }

            if(NULL != pUserInfo)       // �û��ڷ�����
            {
                pUserInfo->GameUserInfo.i64Money += pSendResult->dwGetMoney;
                if(TRUE == bUserIsMe)
                {
                    CString strMessage;
                    TCHAR str[100];

                    if(pSendResult->dwGetMoney > 0)
                    {
                        GetNumString(szNum, pSendResult->dwGetMoney, Glb().m_nPowerOfGold,
                            Glb().m_bUseSpace, Glb().m_strSpaceChar);

                        strMessage = fMsg.GetKeyVal("GameRoom","NoGiftCoins","/:54�𾴵�[%s]����Ϸ�л��ϵͳ����%s���/:54");
                        wsprintf(str,strMessage,m_GameInfo.pMeUserInfo->GameUserInfo.nickName,szNum);
                        m_pRightWnd->m_MessageHandle.InsertSystemMessage(str);
						BZSoundPlay(this, "music/���ȷ����ť������Ϣ��ʾ��.mp3", 0);
                    }

                    if (m_IGameFrame != NULL) 
                    {
                        // ���¶���������ֶ�
                        
                        CM_UserState_Send_Times_Money ControlMessage;
                        memset(&ControlMessage,0,sizeof(ControlMessage));

                        ControlMessage.bActionCode = ACT_USER_SEND_TIMES_MONEY;

                        ControlMessage.dwUserID = pSendResult->dwUserID;
                        ControlMessage.dwGetMoney = pSendResult->dwGetMoney;
                        ControlMessage.dwMoneyOnTimes = pSendResult->dwMoneyOnTimes;
                        ControlMessage.dwMoneyOnCounts = pSendResult->dwMoneyOnCounts;
                        ControlMessage.dwTimesNeed = pSendResult->dwTimesNeed;
                        ControlMessage.dwCountsNeed = pSendResult->dwCountsNeed;

                        if (m_IGameFrame!=NULL)
                            m_IGameFrame->OnControlMessage(CM_USER_SEND_TIMES_MONEY,&ControlMessage,sizeof(ControlMessage));

                    }
                }
            }

            break;

            // PengJiLin, 2011-4-15, ����Ϊ�ɵ��ͽ�ҹ��ܣ���ʹ�á�
            
		    SendUserMoneyByTimes* pSendMoneyResult = (SendUserMoneyByTimes*)pNetData;
			
			UserItemStruct* pGameUserInfo = NULL;
			bool bIsMe = false;
			if (pSendMoneyResult->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID) //�����Ľ���
			{
				pGameUserInfo = m_GameInfo.pMeUserInfo;
				bIsMe = true;
			}
			else
			{
				pGameUserInfo = m_UserManage.FindOnLineUser(pSendMoneyResult->dwUserID);
			}

			if (pGameUserInfo != NULL) //�û��ڷ�����
			{
				pGameUserInfo->GameUserInfo.i64Money += pSendMoneyResult->dwSendGameMoney;
				if(bIsMe)
				{
					CString strMessage;
					TCHAR str[100];

					int iTimes;
					
					iTimes = pSendMoneyResult->dwAllSendTImes - pSendMoneyResult->dwSendTimes;
					GetNumString(szNum, pSendMoneyResult->dwSendGameMoney, Glb().m_nPowerOfGold/*, false*/,Glb().m_bUseSpace, Glb().m_strSpaceChar);

					if(iTimes<=0)
					{
						strMessage = fMsg.GetKeyVal("GameRoom","NoGiftCoins","/:54�𾴵�[%s]����Ϸ�л���%s��ң��ѻ���%d�Σ����޻�������/:54");
						wsprintf(str,strMessage,
						m_GameInfo.pMeUserInfo->GameUserInfo.nickName,szNum,
						pSendMoneyResult->dwSendTimes);
					}
					else
					{
						strMessage = fMsg.GetKeyVal("GameRoom","GiftCoins","/:54�𾴵�[%s]����Ϸ�л���%s��ң��ѻ���%d�Σ�����%d�λ�������/:54");
						wsprintf(str,strMessage,
						m_GameInfo.pMeUserInfo->GameUserInfo.nickName,szNum,
						pSendMoneyResult->dwSendTimes,iTimes);
					}
					BZSoundPlay(this, "music/���ȷ����ť������Ϣ��ʾ��.mp3", 0);
					m_pRightWnd->m_MessageHandle.InsertSystemMessage(str);//send gameroom;
					if (m_IGameFrame != NULL) 
					{
						CM_UserState_Send_Times_Money ControlMessage;
						memset(&ControlMessage,0,sizeof(ControlMessage));

						ControlMessage.bActionCode = ACT_USER_SEND_TIMES_MONEY;

						//ControlMessage.iLastTimes = iTimes;
						//ControlMessage.dwUserID = pSendMoneyResult->dwUserID;
						//ControlMessage.iSendTimes = pSendMoneyResult->dwSendTimes;
						//ControlMessage.iSendMoney = pSendMoneyResult->dwSendGameMoney;

						if (m_IGameFrame!=NULL)
							m_IGameFrame->OnControlMessage(CM_USER_SEND_TIMES_MONEY,&ControlMessage,sizeof(ControlMessage));

					}
				}
			}
		}
		break;
	}
	return true;
}
//������Ϣ����
bool CGameRoomEx::OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bAssistantID==ASS_NET_TEST)	//���������Ϣ
	{
		pClientSocket->SendData(MDM_CONNECT,ASS_NET_TEST,0);
		return true;
	}
	else if ((pNetHead->bAssistantID==ASS_CONNECT_SUCCESS)||(pNetHead->bAssistantID==3))	//���ӳɹ�
	{
		MSG_S_ConnectSuccess * _p = (MSG_S_ConnectSuccess *)pNetData;
		if (_p != NULL)
		{
			pClientSocket->SetCheckCode(_p->i64CheckCode, SECRET_KEY);
		}

		//�汾���
		bool bNewVer=false;
		union Union
		{
			BYTE bb[4];
			DWORD dwFFFF;
		};
		struct Message
		{
			Union ddd[4];
		};
		Message * pMessage=(Message *)pNetData;
		int i=9;

		//�����°汾
		bNewVer=false;
		if (bNewVer==true)
		{
			//��ʾ����
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
			CString str;
			CString strBuffer;
			str = fMsg.GetKeyVal("GameRoom","NewVersion","��%s���Ѿ�������,�˰汾�����Լ�����Ϸ,�����Ƿ��������°汾?");
			strBuffer.Format(str,m_GameInfo.szGameName);
			//if (AFCMessageBox(strBuffer,m_GameInfo.szGameName))//,MB_ICONQUESTION|MB_YESNO,this)==IDYES)
			if (DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strBuffer)==IDYES)
			{
				strBuffer.Format(TEXT("%s"),Glb().m_CenterServerPara.m_strDownLoadUpdatepADDR,m_GameInfo.uNameID,DEV_LIB_VER);
				//ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
				CloseSocket(false);

				GetMainRoom()->PostMessage(WM_CLOSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
				return false;
			}
		}
		//���ɹ�
		
		if (1&Glb().m_CenterServerPara.m_nFunction) ///< �Ƿ���«2������
		{
			char bufTem[16] ={0};
			sprintf(bufTem,"%ld",m_pLogonInfo->dwUserID);
			SendData(bufTem,strlen(bufTem),MDM_GR_LOGON,ASS_GR_IS_VIPROOM,0);
		}
		else
			SendUserInfo();
		
        // PengJiLin, 2010-9-15, ���ӳɹ�
        m_bAllowJoin = TRUE;

		return true;
	}
	return false;
}


//�û��б���
bool CGameRoomEx::OnUserListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;

	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_ONLINE_USER:
	case ASS_GR_NETCUT_USER:	//�û��б�
		{
			UINT uUserCount=uDataSize/sizeof(UserInfoStruct);
			if (uUserCount>0L)
			{
				//�������
				UserItemStruct * pUserItem=NULL;
				UserInfoStruct * pUserInfo=(UserInfoStruct *)pNetData;
				//����ͷ��
				////////////////////////////////
				///Kylin 20081212 �޸ķ����׳��ɿ����������������ͷ�����⣡
				if(m_GameInfo.dwRoomRule & GRR_NOTCHEAT)
				{

					if(pUserInfo->dwUserID!=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
					{
						str = fMsg.GetKeyVal("Game","Player","���");
						wsprintf(pUserInfo->nickName,str);
						pUserInfo->bLogoID=1;

					}
				}
				////////////////////////////////

				//��������
				m_pUserList->SetRedraw(FALSE);
				for (UINT i=0;i<uUserCount;i++)
				{
					////////////////////////////////
					///Kylin 20090116 �޸ķ����׳��ɿ����������������ͷ�����⣡
					UserInfoStruct * pUserInfoTemp;

					pUserInfoTemp=pUserInfo+i;
					if(m_GameInfo.dwRoomRule & GRR_NOTCHEAT)
					{

						if(pUserInfoTemp->dwUserID!=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
						{
							str = fMsg.GetKeyVal("Game","Player","���");
							wsprintf(pUserInfoTemp->nickName,str);
							pUserInfoTemp->bLogoID=1;

						}
					}
					pUserItem=m_UserManage.AddGameUser(pUserInfoTemp,GetUserSortIndex(pUserInfo+i),GetUserConnection((pUserInfo+i)->dwUserID));
					////////////////////////////////////////////////////////////////////////////
					if(pUserItem->GameUserInfo.bLogoID==0xFF)
						pUserItem->GameUserInfo.bLogoID=0x100;

					if(m_GameInfo.dwRoomRule & GRR_NOTCHEAT) //20081127
					{
						//wushuqun 2009.7.2
						//�����γ�Ҫ�������������Ϊ��ͨ�������
						if(pUserInfoTemp->dwUserID!=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
						{
							str = fMsg.GetKeyVal("Game","Player","���");
							strcpy(pUserItem->GameUserInfo.nickName,str);
							pUserItem->GameUserInfo.bLogoID%=0x100;

							pUserItem->GameUserInfo.userType = 0;
						}
					}

					//�Ŷӻ���������ǳƺ�ͷ����һ����
					if (m_GameInfo.dwRoomRule & GRR_NOTCHEAT) // PengJiLin, 2010-9-20, ����������ʾ��������ǳ�
					{
						if(pUserInfoTemp->dwUserID!=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
						{
							strcpy(pUserItem->GameUserInfo.nickName,"���");
							pUserItem->GameUserInfo.bLogoID = 1;
							pUserItem->GameUserInfo.userType = 0;
							pUserItem->GameUserInfo.bBoy = true;
						}
					}

					////////////////////////////////////////////////////////////////////////////
					if(GRR_EXPERCISE_ROOM & m_GameInfo.dwRoomRule || m_GameInfo.dwRoomRule & GRR_CONTEST)
					{
						//ѵ���������ֵ��Ϊ������ֵ
						//pUserItem->GameUserInfo.i64Money=EXPERCISE_MONEY;
					}
					////////////////////////////////////////////////////////////////////////////
					if (m_pUserList->AddGameUser(pUserItem)==true)
					{
						if (!IsQueueGameRoom()) 
						{
							if(m_GameInfo.uComType!=TY_MATCH_GAME)
							{
								if ((pUserItem->GameUserInfo.bDeskNO!=255)&&(pUserItem->GameUserInfo.bDeskStation!=255)
									&&(pUserItem->GameUserInfo.bUserState!=USER_WATCH_GAME))
								{
									m_DeskFrame.SetUserItem(pUserItem->GameUserInfo.bDeskNO,pUserItem->GameUserInfo.bDeskStation,pUserItem);
								}
							}else
							{
								if ((pUserItem->GameUserInfo.bDeskNO!=255)&&(pUserItem->GameUserInfo.bDeskStation!=255)
									&&(pUserItem->GameUserInfo.bUserState!=USER_WATCH_GAME&&pUserItem->GameUserInfo.bUserState!=USER_LOOK_STATE))
								{
									m_DeskFrame.SetUserItem(pUserItem->GameUserInfo.bDeskNO,pUserItem->GameUserInfo.bDeskStation,pUserItem);
								}
							}
						}
						else
						{
							int a1, b1, a2, b2;
							a1 = m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO;
							b1 = m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
							a2 = pUserItem->GameUserInfo.bDeskNO;
							b2 = pUserItem->GameUserInfo.bDeskStation;
							if (a1 == a2 && a1 != 255 && b1 != 255 && a2 != 255 && b2 != 255)
							{
								m_DeskFrame.SetUserItem(0, pUserItem->GameUserInfo.bDeskStation, pUserItem);
							}
						}
						if(pUserItem->GameUserInfo.bLogoID>=0xFF && pUserItem->GameUserInfo.bLogoID<0x200)
							GetMainRoom()->checkUserLogo(pUserItem->GameUserInfo.dwUserID);
					}
				}
				m_pUserList->SetRedraw(TRUE);
			}
			return true;
		}
	case ASS_GR_DESK_STATION:	//����״̬ 
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_DeskStation)) return false;
			MSG_GR_DeskStation * pDeskStation=(MSG_GR_DeskStation *)pNetData;
			for (UINT i=0;i<pNetHead->bHandleCode;i++)
			{
				if ((pDeskStation->bVirtualDesk[i/8]&(1<<(i%8)))!=0)    //change by yjj 090325
				{
					UserItemStruct *pUserInfo=NULL;      //�����Ϣ
					int c=0;
					for(int k=0;k<m_pRightWnd->m_UserList.GetItemCount();k++)
					{
						pUserInfo=m_pRightWnd->m_UserList.GetIdelVirtualUser();
						if(!pUserInfo)
						{
							//wushuqun 2009.6.6
							/*MSG_GR_SR_MatchDelete aloneDesk;
							aloneDesk.dwUserID=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID;
							aloneDesk.bDeskIndex=i;
							aloneDesk.bDeskStation=c;
							SendData(&aloneDesk,sizeof(aloneDesk),MDM_GR_MANAGE,ASS_GR_DEALONE_DESK,0);*/
							if(bExtendRobot)
							{
								str = fMsg.GetKeyVal("Game","NoEnoughVirtualPlayer","û���㹻�����������!");
								m_pRightWnd->m_MessageHandle.InsertSystemMessage(str.GetBuffer());
							}
							break;
						}

						if(pUserInfo->GameUserInfo.bUserState!=USER_PLAY_GAME && pUserInfo->GameUserInfo.dwUserID<0)
						{
							pUserInfo->GameUserInfo.bUserState=USER_PLAY_GAME;
							pUserInfo->GameUserInfo.bDeskNO=i;
							pUserInfo->GameUserInfo.bDeskStation=c;
							m_DeskFrame.SetUserItem(i,c,pUserInfo);
							m_pRightWnd->m_UserList.UpdateGameUser(pUserInfo);
							if(++c>=m_GameInfo.uDeskPeople)
								break;
						}
					}
					//m_DeskFrame.SetLock(i,true);   //������ʾ����  yjj 090306
					
					SetTimer(2000+i,(rand()%m_GameInfo.uVirtualGameTime)*1000,NULL);
					CBaseDeskView * pDesk=m_DeskFrame.GetDeskArrayPtr(i);
					
					//pDesk->SetLock(true,&m_DeskFrame);   ������ʾ���� yjj 090306
					
					m_DeskFrame.SetPlayGame(i,true);
				}
				if ((pDeskStation->bDeskLock[i/8]&(1<<(i%8)))!=0) 
				{
					m_DeskFrame.SetLock(i,true);   //������ʾ����  yjj 090306
				}
				if ((pDeskStation->bDeskStation[i/8]&(1<<(i%8)))!=0) 
					m_DeskFrame.SetPlayGame(i,true);
				if(pDeskStation->iBasePoint[i]!=0) 
					m_DeskFrame.SetBasePoint(i,pDeskStation->iBasePoint[i],true);
			}

			return true;
		}
	}
	return false;
}

//002 ϵͳ��Ϣ����
bool CGameRoomEx::OnSystemMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	//ASSERT(pNetHead->bMainID==MDM_GR_MESSAGE);

	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_SYSTEM_MESSAGE:		//ϵͳ��Ϣ
		{
			//Ч������
			MSG_GA_S_Message * pMessage=(MSG_GA_S_Message *)pNetData;
			AFCPlaySound(GetModuleHandle(FACE_DLL_NAME),TEXT("SYSTEM_MSG"));

			//��������
			if ((pMessage->bShowStation&SHS_TALK_MESSAGE)!=0) 
				m_pRightWnd->m_MessageHandle.InsertSystemMessage(pMessage->szMessage,pMessage->bFontSize);
			if (pMessage->bCloseFace==TRUE)
				pClientSocket->CloseSocket();
			if ((pMessage->bShowStation&SHS_MESSAGE_BOX)!=0)
			{
				//AFCMessageBox(pMessage->szMessage,m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,pMessage->szMessage);
			}

			SendMessageToGame(pMessage->szMessage);

			return true;	
		}
	}

	return true;
}

//������Ϣ����
bool CGameRoomEx::OnManageMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	ASSERT(pNetHead->bMainID==MDM_GR_MANAGE);
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;
	//״̬�ж�
	if (m_bLogonFinish==false) return true;

	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_CALL_GM_SUCCESS: //�������ܳɹ�
		{
			//if (uDataSize!=0L) return false;
			//��������
			//CString strMessage;
			//strMessage.Format(TEXT("ϵͳ���棺%s"),pWarnning->szWarnMsg);
			str = fMsg.GetKeyVal("GameRoom","CallNetAdminSuccess","�������ܳɹ�!");
			m_pRightWnd->m_MessageHandle.InsertNormalMessage(str.GetBuffer());
			//SendMessageToGame("�������ܳɹ�!");
			SendMessageToGame(str);
			return true;
		}
	case ASS_GR_CALL_GM_FAIL://��������ʧ��
		{
			//if (uDataSize!=0L) return false;
			//��������
			//CString strMessage;
			//strMessage.Format(TEXT("ϵͳ���棺%s"),pWarnning->szWarnMsg);
			if(pNetHead->bHandleCode==0)
			{
				str = fMsg.GetKeyVal("GameRoom","CallNetAdminFailed","��������ʧ��!");
				m_pRightWnd->m_MessageHandle.InsertNormalMessage(str.GetBuffer());

				SendMessageToGame(str);
			}
			else if(pNetHead->bHandleCode==1)
			{
				str = fMsg.GetKeyVal("GameRoom","OnMinACall","һ������ֻ�ܺ���һ��!");
				m_pRightWnd->m_MessageHandle.InsertNormalMessage(str.GetBuffer());
				SendMessageToGame(str);
			}
			return true;
		}

	case ASS_GR_CUT_USER:		//���û�����//��ȫ����û���Ϸ
		{
			//Ч������
			//��������
			if (uDataSize!=0L) return false;

			if(pNetHead->bHandleCode==0)//���û�����
			{
				if (CanCloseNow()==false)
					SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
				CloseGameClient();
				SendData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
				//OnCancel();
				//CString strMessage = fMsg.GetKeyVal("GameRoom","KickOutCallAdmin","�����߳���Ϸ����,��������,�������Ա��ϵ!");
				//m_pRightWnd->m_MessageHandle.InsertSystemMessage(strMessage.GetBuffer());
				//strMessage.ReleaseBuffer();
				//AFCMessageBox(strMessage.GetBuffer(),m_GameInfo.szGameName);
				//CloseSocket(false);
				//if(m_pPersonBank != NULL)
				//	if(m_pPersonBank->GetSafeHwnd()!=NULL)
				//		//m_pPersonBank->OnCancel();
				//GetMainRoom()->PostMessage(WM_COLSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);

				OnBnClickedClose();
				AfxGetMainWnd()->PostMessage(QUIT_GAMEROOM,(WPARAM)4,0);

			}
			else if(pNetHead->bHandleCode==1)//��ȫ����û���Ϸ
			{
				SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,1);
				CloseGameClient();
				//CString strMessage = fMsg.GetKeyVal("GameRoom","SafeOutCallAdmin","������ȫ�����Ϸ����,��������,�������Ա��ϵ!");
				//m_pRightWnd->m_MessageHandle.InsertSystemMessage(strMessage.GetBuffer());
				//strMessage.ReleaseBuffer();
				//AFCMessageBox(strMessage.GetBuffer(),m_GameInfo.szGameName);
				//OnCancel();

				OnBnClickedClose();
				AfxGetMainWnd()->PostMessage(QUIT_GAMEROOM,(WPARAM)4,0);
			}
			return true;
		}
	case ASS_GR_FORBID_USER:		//�����
		{
			//Ч������
			if (uDataSize!=0L) return false;

			//�ر���Ϸ
			if (CanCloseNow()==false) SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
			pClientSocket->CloseSocket();
			CloseGameClient();

			//��������
			CString strMessage = fMsg.GetKeyVal("GameRoom","ForbidAccount","�����ʺŽ�ֹ��¼,��������,�������Ա��ϵ!");
			m_pRightWnd->m_MessageHandle.InsertSystemMessage(strMessage.GetBuffer());
			strMessage.ReleaseBuffer();
			
			//AFCMessageBox(strMessage,m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,strMessage);

			return false;
		}
	case ASS_GR_FORBID_USER_IP:		//����IP
		{
			//Ч������
			if (uDataSize!=0L) return false;

			//�ر���Ϸ
			if (CanCloseNow()==false) SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
			pClientSocket->CloseSocket();
			CloseGameClient();

			//��������
			CString strMessage = fMsg.GetKeyVal("GameRoom","ForbidIPCallAdmin","����IP�ѱ���ֹ��¼,��������,�������Ա��ϵ!");
			m_pRightWnd->m_MessageHandle.InsertSystemMessage(strMessage.GetBuffer());
			strMessage.ReleaseBuffer();
			
			//AFCMessageBox(strMessage,m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,strMessage);

			return false;
		}
	case ASS_GR_WARN_MSG:	//������Ϣ
		{
			//Ч������
			MSG_GR_SR_Warning * pWarnning=(MSG_GR_SR_Warning *)pNetData;

			//��������
			CString strMessage;
			str = fMsg.GetKeyVal("GameRoom","SystemWarning","ϵͳ���棺%s");
			strMessage.Format(str,pWarnning->szWarnMsg);
			m_pRightWnd->m_MessageHandle.InsertSystemMessage(pWarnning->szWarnMsg);
			
			//AFCMessageBox(strMessage,m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,strMessage);

			return true;
		}
	case ASS_GR_POWER_SET:		//Ȩ������
		{
			//TCHAR sz[20];
			//wsprintf(sz,"%d",pNetHead->bHandleCode);
			//AfxMessageBox(sz);
			if(pNetHead->bHandleCode==ASS_GR_GAME_POWER_SET_SUCCESS)
			{
				str = fMsg.GetKeyVal("GameRoom","ConfigUserRightSuccess","�����û���ϷȨ�޳ɹ�!");
				m_pRightWnd->m_MessageHandle.InsertNormalMessage(str.GetBuffer());
				SendMessageToGame(str);
			}
			else if(pNetHead->bHandleCode==ASS_GR_GAME_POWER_SET_FAIL)
			{
				str = fMsg.GetKeyVal("GameRoom","ConfigUserRightFailed","�����û���ϷȨ��ʧ��!");
				m_pRightWnd->m_MessageHandle.InsertNormalMessage(str.GetBuffer());
				SendMessageToGame(str);
			}
			else if(pNetHead->bHandleCode==ASS_GR_GAME_POWER_SET_RELEASE)
			{
				str = fMsg.GetKeyVal("GameRoom","RemoveUserRightSuccess","����û���ϷȨ�����Ƴɹ�!");
				m_pRightWnd->m_MessageHandle.InsertNormalMessage(str.GetBuffer());
				SendMessageToGame(str);
			}
			else if(pNetHead->bHandleCode==ASS_GR_GAME_POWER_SET_RETURN)//�������
			{
				//Ч������
				if (uDataSize!=sizeof(MSG_GR_SR_GamePowerSet)) return false;
				MSG_GR_SR_GamePowerSet * pPowerSet=(MSG_GR_SR_GamePowerSet *)pNetData;

				//��������
				if (pPowerSet->dwUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
				{
					m_GameInfo.dwGamePower=pPowerSet->dwGamePowerID;
					m_GameInfo.dwMasterPower=pPowerSet->dwMasterPowerID;
				}
				TCHAR sz[300],sz_temp[300];
				if(m_GameInfo.dwGamePower==0)
				{
					str = fMsg.GetKeyVal("GameRoom","RightRestrictWasRemove","��ϲ��,������ϷȨ�������ѱ�����Ա���!");
					wsprintf(sz,str);
					m_pRightWnd->m_MessageHandle.InsertNormalMessage(sz);
					SendMessageToGame(sz);
					return true;
				}
				if(pPowerSet->bBindKind&0x04)
				{
					str = fMsg.GetKeyVal("GameRoom","LostAllRight",
						"�Բ�����%d������,���������з���ʧȥ����Ȩ��,�������Ա��ϵ��");
					wsprintf(sz,str,pPowerSet->uLockTime);
					m_pRightWnd->m_MessageHandle.InsertNormalMessage(sz);
					lstrcpy(sz_temp,sz);
				}
				else if(pPowerSet->bBindKind&0x01)
				{
					str = fMsg.GetKeyVal("GameRoom","LostRoomRight",
						"�Բ�����%d������,�����ڴ˷���ʧȥ����Ȩ��,�������Ա��ϵ��");
					wsprintf(sz,str,pPowerSet->uLockTime);
					m_pRightWnd->m_MessageHandle.InsertNormalMessage(sz);
					lstrcpy(sz_temp,sz);
				}
				if(!CUserPower::CanRoomTalk(m_GameInfo.dwGamePower))
				{
					str = fMsg.GetKeyVal("GameRoom","HallChat","<��������>");
					m_pRightWnd->m_MessageHandle.InsertNormalMessage(str.GetBuffer());
					lstrcat(sz_temp,"\n");
					lstrcat(sz_temp,str.GetBuffer());
				}
				if(!CUserPower::CanGameTalk(m_GameInfo.dwGamePower))
				{
					str = fMsg.GetKeyVal("GameRoom","GameChat","<��Ϸ����>");
					m_pRightWnd->m_MessageHandle.InsertNormalMessage(str.GetBuffer());
					lstrcat(sz_temp,"\n");
					lstrcat(sz_temp,str.GetBuffer());
				}
				if(!CUserPower::CanSendMessage(m_GameInfo.dwGamePower))
				{
					str = fMsg.GetKeyVal("GameRoom","SendMsg","<������Ϣ>");
					m_pRightWnd->m_MessageHandle.InsertNormalMessage(str.GetBuffer());
					lstrcat(sz_temp,"\n");
					lstrcat(sz_temp,str.GetBuffer());
				}
				if(!CUserPower::CanPlayGame(m_GameInfo.dwGamePower))
				{
					str = fMsg.GetKeyVal("GameRoom","PlayingGame","<������Ϸ>");
					m_pRightWnd->m_MessageHandle.InsertNormalMessage(str.GetBuffer());
					lstrcat(sz_temp,"\n");
					lstrcat(sz_temp,str.GetBuffer());
				}
				if(!CUserPower::CanWatchGame(m_GameInfo.dwGamePower))
				{
					str = fMsg.GetKeyVal("GameRoom","SideSee","<�Թ���Ϸ>");
					m_pRightWnd->m_MessageHandle.InsertNormalMessage(str.GetBuffer());
					lstrcat(sz_temp,"\n");
					lstrcat(sz_temp,str.GetBuffer());
				}
				str = fMsg.GetKeyVal("GameRoom","AfterMinsToLogin","����%d���Ӻ����µ�½��Ϸ�����Խ����ֹ!!!");
				wsprintf(sz,str,pPowerSet->uLockTime);
				m_pRightWnd->m_MessageHandle.InsertNormalMessage(sz);
				lstrcat(sz_temp,"\n");lstrcat(sz_temp,sz);
				SendMessageToGame(sz_temp);
			}
			return true;
		}
	}

	return true;
}

// PengJiLin, 2010-10-14, ģ�����������Ϸ�˷�����Ϣ
void CGameRoomEx::SendDataToGame(void * pNetData, UINT uDataSize, 
                                 DWORD dwMainID, DWORD dwAssistantID, DWORD dwHandleCode)
{
    NetMessageHead nethead;
    memset(&nethead,0,sizeof(nethead));
    nethead.bMainID = dwMainID;
    nethead.bAssistantID = dwAssistantID;
    nethead.bHandleCode = dwHandleCode;
    if (m_IGameFrame!=NULL)
    {
        m_IGameFrame->OnSocketReadEvent(&nethead,pNetData,uDataSize,NULL);
    }
}
void CGameRoomEx::SendDataToGame(DWORD dwMainID, DWORD dwAssistantID, DWORD dwHandleCode)
{
    SendDataToGame(NULL, 0, dwMainID, dwAssistantID, dwHandleCode);
}

//JianGuankun, 2012-2-16, �����˳����䰴ť����
void CGameRoomEx::SetEnableExitRoomBtn()
{
	//TIME_ENABLE_EXIT_BTN�Ƿ�ֹGameLauncherû�лط����˳����䰴ť���á�֪ͨ�����õı�����\
	����m_bt1һֱ���ڲ����õ�״̬������ñ�������m_bt1�����ã���û�б�Ҫ������������TIME_ENABLE_EXIT_BTN
	KillTimer(TIME_ENABLE_EXIT_BTN);

	m_bt1.EnableWindow(true);

	return;
}

// PengJiLin, 2011-6-23, ������Ϣ
void CGameRoomEx::SetActiveToGameRoom(UINT uState)
{
	return;//zht 2012-4-23 GDI+�޸Ŀ�������

	if (GetSafeHwnd() == NULL)///zht  2011-10-13 �޸ľ���ƽ̨�л����˳�ʱ��������
	{
		return;
	}

    if(NULL == m_IGameFrame)
    {
        m_bActive = TRUE;
        m_DeskFrame.m_bActive = m_bActive;
        return;
    }

    m_bActive = FALSE;
    if(uState > 0) m_bActive = TRUE;
    m_DeskFrame.m_bActive = m_bActive;

    Invalidate(FALSE);
    m_DeskFrame.Invalidate(FALSE);
}

//ģ�����������ϵͳ��Ϣ
bool CGameRoomEx::SendMessageToGame(CString szMessage)
{
	if(szMessage.GetLength()>995)
		return false;
	//ģ�����������Ϸ����ϵͳ��Ϣ
	NetMessageHead nethead;
	MSG_GA_S_Message	msg;
	memset(&nethead,0,sizeof(nethead));
	memset(&msg,0,sizeof(msg));
	nethead.bMainID=MDM_GM_MESSAGE;
	nethead.bAssistantID=ASS_GM_SYSTEM_MESSAGE;
	lstrcpy(msg.szMessage,szMessage);
	msg.bShowStation=0x01;
	if (m_IGameFrame!=NULL)
	{
		m_IGameFrame->OnSocketReadEvent(&nethead,&msg,sizeof(MSG_GA_S_Message),NULL);
	}
	return true;
}

//ģ�����������ϵͳ��Ϣ
bool CGameRoomEx::SendMessageToGame(TCHAR *szMessage)
{
	if(lstrlen(szMessage)>995)
		return false;
	//ģ�����������Ϸ����ϵͳ��Ϣ
	NetMessageHead nethead;
	MSG_GA_S_Message	msg;
	memset(&nethead,0,sizeof(nethead));
	memset(&msg,0,sizeof(msg));
	nethead.bMainID=MDM_GM_MESSAGE;
	nethead.bAssistantID=ASS_GM_SYSTEM_MESSAGE;
	lstrcpy(msg.szMessage,szMessage);
	msg.bShowStation=0x01;
	if (m_IGameFrame!=NULL)
	{
		m_IGameFrame->OnSocketReadEvent(&nethead,&msg,sizeof(MSG_GA_S_Message),NULL);
	}
	return true;
}

//������Ϣ����
bool CGameRoomEx::OnMatchMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	ASSERT(pNetHead->bMainID==MDM_GR_MATCH_INFO);

	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_MATCH_USER:		//�����б�
		{
			//Ч������
			UINT uUserCount=uDataSize/sizeof(MSG_GR_SR_MatchUser);
			if (uUserCount>0L)
			{
				UserItemStruct * pUserItem=NULL;
				MSG_GR_SR_MatchUser * pNowMatchUser=NULL;
				MSG_GR_SR_MatchUser * pMatchUser=(MSG_GR_SR_MatchUser *)pNetData;
				for (UINT i=0;i<uUserCount;i++)
				{
					//��������
					pNowMatchUser=pMatchUser+i;
					m_UserManage.AddMatchUser(pNowMatchUser->dwUserID);
					m_DeskFrame.SetMatchInfo(pNowMatchUser->bDeskIndex,pNowMatchUser->bDeskStation,pNowMatchUser->dwUserID,pNowMatchUser->szUserName);

					//���½���
					pUserItem=m_UserManage.FindOnLineUser(pNowMatchUser->dwUserID);
					if (pUserItem!=NULL)
					{
						pUserItem->bMatchUser=true;
						m_pUserList->UpdateGameUser(pUserItem);

						//if(m_pFriendList!=NULL)
						//	m_pFriendList->UpdateGameUser(pUserItem);
						if ((m_pFindUser!=NULL)&&(m_pFindUser->GetSafeHwnd())) 
							m_pFindUser->UpdateUserItem(pUserItem);
					}

					//�ж��Ƿ��Լ�
					if (pNowMatchUser->dwUserID==m_pLogonInfo->dwUserID)
					{
						m_bMatchDeskIndex=pNowMatchUser->bDeskIndex;
						m_bMatchDeskStation=pNowMatchUser->bDeskStation;
					}
				}
			}

			return true;
		}
	case ASS_GR_MATCH_DELETE:	//ȡ������
		{
			//Ч������
			UINT uUserCount=uDataSize/sizeof(MSG_GR_SR_MatchDelete);
			if (uUserCount>0L)
			{
				UserItemStruct * pUserItem=NULL;
				MSG_GR_SR_MatchDelete * pNowMatchUser=NULL;
				MSG_GR_SR_MatchDelete * pMatchDelete=(MSG_GR_SR_MatchDelete *)pNetData;
				for (UINT i=0;i<uUserCount;i++)
				{
					//��������
					pNowMatchUser=pMatchDelete+i;
					m_UserManage.DeleteMatchUser(pNowMatchUser->dwUserID);
					m_DeskFrame.SetMatchInfo(pNowMatchUser->bDeskIndex,pNowMatchUser->bDeskStation,0L,NULL);

					//���½���
					pUserItem=m_UserManage.FindOnLineUser(pNowMatchUser->dwUserID);
					if (pUserItem!=NULL)
					{
						pUserItem->bMatchUser=false;
						m_pUserList->UpdateGameUser(pUserItem);
						//if(m_pFriendList!=NULL)
						//	m_pFriendList->UpdateGameUser(pUserItem);
						if ((m_pFindUser!=NULL)&&(m_pFindUser->GetSafeHwnd())) 
							m_pFindUser->UpdateUserItem(pUserItem);
					}

					//������Ϣ
					if (pNowMatchUser->dwUserID==m_pLogonInfo->dwUserID)
					{
						m_bMatchDeskIndex=255;
						m_bMatchDeskStation=255;
						CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
						CString str;
						str = fMsg.GetKeyVal("GameRoom","GameQualificationRemoved","��ע�⣺���Ĳ����ʸ�ȡ����");
						m_pRightWnd->m_MessageHandle.InsertSystemMessage(str.GetBuffer());
					}
				}
			}
			return true;
		}

	}

	return true;
}

//������Ϣ����
bool CGameRoomEx::OnRoomMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if(!this->m_hWnd)
		return true;
	if(pNetHead->bMainID!=MDM_GR_ROOM)
		return true;
	//ASSERT(pNetHead->bMainID==MDM_GR_ROOM);

	//״̬�ж�
	if (m_bLogonFinish==false)
		return true;

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	switch (pNetHead->bAssistantID)
	{
		//wushuqun 2009.6.5
		//��ս�����¼��� -- begin
	case ASS_GR_BATTLEROOM_RESULT:
		{
			if (m_bLogonFinish==false) 
			{
				return true;
			}
			BattleRoomRecordRes * pBattleRoomResult = (BattleRoomRecordRes * )pNetData;
			if (pBattleRoomResult->uRecordResult == DTR_GR_BATTLEROOM_TIMEOUT_ERROR)
			{
				//��ս����ʱ���Ѿ���Ч
				if (m_GameInfo.uRoomID == pBattleRoomResult->uRoomID)
				{
                    // PengJiLin, 2010-6-29, �Ȳ��رգ��ȴ���Ϸ���涯�����ٹر�
                    if(NULL != m_IGameFrame )
                        m_IGameFrame->OnSocketReadEvent(pNetHead,pNetData,uDataSize,pClientSocket);

                    m_bGameTimeOut = true;
                    return true;
				}
			}
			return true;
			
		} //--end
	case ASS_GR_NORMAL_TALK:	//��ͨ����
		{
			//Ч������
			MSG_GR_RS_NormalTalk * pNormalTalk=(MSG_GR_RS_NormalTalk *)pNetData;
			if (m_bLogonFinish==false) return true;

			//��������
			UserItemStruct * pSendUserItem=m_UserManage.FindOnLineUser(pNormalTalk->dwSendID);
			if (pSendUserItem==NULL) return true;

			//���˲��ܻ�ӭ�û�
			if (pSendUserItem->uConnection==CM_ENEMY) return true;

			//���������û�
			for (INT_PTR i=0;i<m_ShieldArray.GetCount();i++)
			{
				if (pSendUserItem->GameUserInfo.dwUserID==m_ShieldArray.GetAt(i)) return true;
			}

			//������Ϣ
			if (pNormalTalk->dwTargetID!=0L)
			{
				UserItemStruct * pTargerUserItem=m_UserManage.FindOnLineUser(pNormalTalk->dwTargetID);
				if (pTargerUserItem!=NULL) 
				{
					m_pRightWnd->m_MessageHandle.InsertUserTalk(pSendUserItem->GameUserInfo.nickName,pTargerUserItem->GameUserInfo.nickName,
						pNormalTalk->szMessage,pNormalTalk->crColor,false);
					return true;
				}
			}
			m_pRightWnd->m_MessageHandle.InsertUserTalk(pSendUserItem->GameUserInfo.nickName,NULL,pNormalTalk->szMessage,pNormalTalk->crColor,false);

			return true;
		}
	case ASS_GR_SHORT_MSG:		//����Ϣ����
		{
			//Ч������
			MSG_GR_RS_NormalTalk * pNormalTalk=(MSG_GR_RS_NormalTalk *)pNetData;
			if (m_bLogonFinish==false) return true;

			//��������
			UserItemStruct * pSendUserItem=m_UserManage.FindOnLineUser(pNormalTalk->dwSendID);
			UserItemStruct * pTargerUserItem=m_UserManage.FindOnLineUser(pNormalTalk->dwTargetID);
			if ((pTargerUserItem==NULL)||(pSendUserItem==NULL)) return true;

			//���˴���
			UserItemStruct * pPassUserItem=NULL;
			if (pNormalTalk->dwSendID==m_pLogonInfo->dwUserID) pPassUserItem=pTargerUserItem;
			else pPassUserItem=pSendUserItem;

			//���˲��ܻ�ӭ�û�
			if (pPassUserItem->uConnection==CM_ENEMY) return true;

			//���������û�
			for (INT_PTR i=0;i<m_ShieldArray.GetCount();i++)
			{
				if (pPassUserItem->GameUserInfo.dwUserID==m_ShieldArray.GetAt(i)) return true;
			}
			//if(m_IGameFrame != NULL)	//������Ϸ�ͻ��˱㷢��������
			//{
			//	m_IGameFrame->SendShortMessage(pNormalTalk);	
			//}else

			CIMMain *pIMList = &(GetMainRoom()->m_pGameListWnd->m_IMList);
			HTREEITEM htiFriend = pIMList->htiFamilier;

			HTREEITEM hti = pIMList->GetParentItem(pIMList->FindUserITEM(pNormalTalk->dwSendID + 1000));

			int charType = GetMainRoom()->m_PlaceUserInfo.iAddFriendType / 65535;

			if (charType == 1)
			{
				return true;
			}
			if (charType == 2 &&  hti != htiFriend)
			{
				return true;
			}	

			{
				//��ʾ��Ϣ
				CShortMessage * pShortMessageWnd=NULL;
				if (pNormalTalk->dwSendID==m_pLogonInfo->dwUserID)
				{
					//�Լ����͵���Ϣ
					pShortMessageWnd=ActiveShortWnd(pNormalTalk->dwTargetID,pTargerUserItem,true);
					if (pShortMessageWnd!=NULL) 
						pShortMessageWnd->OnRecvMessage(pSendUserItem->GameUserInfo.nickName,pNormalTalk->szMessage,pNormalTalk->crColor);
				}
				else	//������Ϸ�߷�����Ϣ	
				{
					if (m_bShowMessage==TRUE)
					{
						pShortMessageWnd=ActiveShortWnd(pNormalTalk->dwSendID,pSendUserItem,true);
						if (pShortMessageWnd!=NULL)
							pShortMessageWnd->OnRecvMessage(pSendUserItem->GameUserInfo.nickName,pNormalTalk->szMessage,pNormalTalk->crColor);
						else m_pRightWnd->m_MessageHandle.InsertUserTalk(pSendUserItem->GameUserInfo.nickName,m_GameInfo.pMeUserInfo->GameUserInfo.nickName,
							pNormalTalk->szMessage,pNormalTalk->crColor,false);
					}
					else m_pRightWnd->m_MessageHandle.InsertUserTalk(pSendUserItem->GameUserInfo.nickName,m_GameInfo.pMeUserInfo->GameUserInfo.nickName,
						pNormalTalk->szMessage,pNormalTalk->crColor,false);

					if(GetMainRoom()->m_pGameListWnd->m_IMList.GetUserGroup(pNormalTalk->dwSendID)==2)
					{
						GetMainRoom()->m_pGameListWnd->m_IMList.SetUserGroup(pNormalTalk->dwSendID, pSendUserItem->GameUserInfo.nickName, 2);
					}
				}
			}

			return true;
		}
	case ASS_GR_USER_AGREE:		//�û�ͬ��
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_UserAgree)) return false;
			MSG_GR_R_UserAgree * pAgreeGame=(MSG_GR_R_UserAgree *)pNetData;

			//��������
			UserItemStruct * pUserItem=m_DeskFrame.GetUserItem(pAgreeGame->bDeskNO,pAgreeGame->bDeskStation);
			if (pUserItem!=NULL)
			{
				if (pAgreeGame->bAgreeGame==TRUE) 
					pUserItem->GameUserInfo.bUserState=USER_ARGEE;
				else 
					pUserItem->GameUserInfo.bUserState=USER_SITTING;

				m_DeskFrame.UpdateDeskView(pAgreeGame->bDeskNO);
				m_pUserList->UpdateGameUser(pUserItem);
				//				if(m_pFriendList!=NULL)
				//					m_pFriendList->UpdateGameUser(pUserItem);
				if ((m_pFindUser!=NULL)&&(m_pFindUser->GetSafeHwnd()))
					m_pFindUser->UpdateUserItem(pUserItem);
			}

			//wushuqun 2009.9.5
			if(NULL == m_GameInfo.pMeUserInfo)
			{
				return true;
			}

			//���ƿͻ���
			if ((m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=255)&&(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==pAgreeGame->bDeskNO))
			{
				CM_UserState_Change ControlMessage;
				ControlMessage.bActionCode=ACT_USER_AGREE;
				ControlMessage.bDeskStation=pAgreeGame->bDeskStation;
				ControlMessage.pUserItem=pUserItem;
				ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
				//zfx add//����30�벻��ʼ�Զ��뿪
				if(pAgreeGame->bDeskStation==m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation)
				{
					m_itime=60;
					KillTimer(IDR_EXIT);
				}
				if(m_GameInfo.pMeUserInfo->GameUserInfo.bUserState==USER_SITTING)
				{
					int bDeskIndex=m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO;
					CBaseDeskView * pDesk=m_DeskFrame.GetDeskArrayPtr(bDeskIndex);
					int agreepeople=0;
					if (pDesk!=NULL) 
					{
						for(int i=0;i<4;i++)
						{
							if(i==m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation)
								continue;
							if((pDesk->GetUserItem(i)!=NULL)&&pDesk->GetUserItem(i)->GameUserInfo.bUserState==USER_ARGEE)
								agreepeople++;
						}
						if(agreepeople==m_pRoomInfo->pComRoomInfo.uDeskPeople-1)
						{
							m_itime=min(m_itime,20);
						}
					}
				}//zfx add end
				//m_GameInfo.pMeUserInfo->GameUserInfo.bUserState = USER_ARGEE;
			}

			return true;
		}
	case ASS_GR_GAME_BEGIN:		//��Ϸ��ʼ
		{
			//Ч������
			if (uDataSize!=0) return false;

			//��������
			BYTE bDeskIndex=pNetHead->bHandleCode;
			for (UINT i=0;i<m_GameInfo.uDeskPeople;i++)
			{
				UserItemStruct * pUserItem=m_DeskFrame.GetUserItem(bDeskIndex,i);
				if (pUserItem!=NULL) 
				{
					pUserItem->GameUserInfo.bUserState=USER_PLAY_GAME;
					m_pUserList->UpdateGameUser(pUserItem);
					//if(m_pFriendList!=NULL)
					//	m_pFriendList->UpdateGameUser(pUserItem);
				}
			}

			//�ټ��� add by wlr 20090713
			//if ( HUNDRED_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID		//�ټ���
			//	||SUOHA_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID			//�ٱ����
			//	||NIUNIU_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID		//�ٱ�ţţ
			//	||ERZHANGPAI_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID )	//�ٱ������
			char cKey[10];
			CString sPath=CBcfFile::GetAppPath();
			CBcfFile fsr( sPath + "SpecialRule.bcf");
			sprintf(cKey, "%d", m_GameInfo.uNameID);
			int iResult = fsr.GetKeyVal (_T("BJLType"), cKey, 0);

			if (iResult)
			{
				//m_DeskFrame.m_bShowPlaying = true;
				m_DeskFrame.ShowBJLBeginBtn();//���ƽ�����Ϸ��ť
			}

			//end of �ټ��� add by wlr 20090713

			m_DeskFrame.SetPlayGame(bDeskIndex,true);
			m_DeskFrame.UpdateDeskView(bDeskIndex);

			//���ƿͻ���
			if ((m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=255)&&(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==bDeskIndex))
			{

				m_TipSendMoney.clear();
				m_iDeskPeopleMe = 0;
				for (UINT i=0;i<m_GameInfo.uDeskPeople;i++)
				{
					UserItemStruct * pUserItem=m_DeskFrame.GetUserItem(bDeskIndex,i);
					if (pUserItem!=NULL) 
					{
						m_iDeskPeopleMe++;
					}
				}


				CM_UserState_Change ControlMessage;
				ControlMessage.bActionCode=ACT_GAME_BEGIN;
				ControlMessage.bDeskStation=255;
				ControlMessage.pUserItem=NULL;
				ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));

				//����û���Ϣ  yjj 090224
				AddUserInfoToRecord();

				if (m_GameInfo.pMeUserInfo->GameUserInfo.bUserState != USER_WATCH_GAME)
				{
					m_GameInfo.pMeUserInfo->GameUserInfo.bUserState = USER_PLAY_GAME;
				}
				m_DeskFrame.SetCanInput(0,false);
			}

			
			return true;
		}
	case ASS_GR_GAME_END:		//��Ϸ����
		{
			//Ч������
			if (uDataSize!=0) return false;

			//������������Կ����л�����
			m_bChangeRoom = true;

			//��������
			BYTE bDeskIndex=pNetHead->bHandleCode;
			for (UINT i=0;i<m_GameInfo.uDeskPeople;i++)
			{
				UserItemStruct * pUserItem=m_DeskFrame.GetUserItem(bDeskIndex,i);
				if (pUserItem!=NULL) 
				{
					if (pUserItem->GameUserInfo.bUserState != USER_WATCH_GAME)
					{
						pUserItem->GameUserInfo.bUserState = USER_SITTING;
					}
					//��Ϸ�ս����������ŵ�״̬
					m_pUserList->UpdateGameUser(pUserItem);
					//if(m_pFriendList!=NULL)
					//	m_pFriendList->UpdateGameUser(pUserItem);

					if (pUserItem->GameUserInfo.dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
					{
						if (m_GameInfo.uComType==TY_MONEY_GAME)
						{
							if(m_GameInfo.pMeUserInfo->GameUserInfo.i64Money < m_GameInfo.uLessPoint)
							{
								SetTimer(TIME_SLEEP_EXIT_LACK, 5000, NULL);
							}


							if (m_GameInfo.uMaxPoint > 0L)
							{
								if(m_GameInfo.pMeUserInfo->GameUserInfo.i64Money > m_GameInfo.uMaxPoint)
								{
									SetTimer(TIME_SLEEP_EXIT_OVERFLOW, 5000, NULL);
								}
							}
						}
					}
					
				}
			}
			m_DeskFrame.SetPlayGame(bDeskIndex,false);
			m_DeskFrame.UpdateDeskView(bDeskIndex);

			//���ƿͻ���
			if ((m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=255)&&(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==bDeskIndex))
			{
				//��Ϸ����������ʷ�ļ�  ������Ϸ�����������ļ� yjj 090225
				CString appPath = CBcfFile::GetAppPath ();
				if(m_byVideoUse != 0)  // PengJiLin, 2010-5-18, ���¼�����ù���
                    m_AddRecordModule.SaveRecordFile(appPath,m_GameInfo.pMeUserInfo->GameUserInfo.szName,m_GameInfo.uNameID);

				CM_UserState_Change ControlMessage;
				ControlMessage.bActionCode=ACT_GAME_END;
				ControlMessage.bDeskStation=255;
				ControlMessage.pUserItem=NULL;
				ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));

				if (m_GameInfo.pMeUserInfo->GameUserInfo.bUserState != USER_WATCH_GAME)
				{
					m_GameInfo.pMeUserInfo->GameUserInfo.bUserState = USER_SITTING;
				}				

				m_DeskFrame.SetCanInput(0,true);
			}
			else
			{
				//�ټ��� add by wlr 20090717
				//if (  HUNDRED_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID		//�ټ���
				//	||SUOHA_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID			//�ٱ����
				//	||NIUNIU_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID		//�ٱ�ţţ
				//	||ERZHANGPAI_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID )	//�ٱ������
				char cKey[10];
				CString sPath=CBcfFile::GetAppPath();
				CBcfFile fsr( sPath + "SpecialRule.bcf");
				sprintf(cKey, "%d", m_GameInfo.uNameID);
				int iResult = fsr.GetKeyVal (_T("BJLType"), cKey, 0);

				if (iResult)
				{
					//m_DeskFrame.m_bShowPlaying = false;//showBJLBeginOrPlayingBtn(false);//���ƽ�����Ϸ��ť
					m_DeskFrame.ShowBJLBeginBtn();
				}
				//end of �ټ��� add by wlr 20090717
			}

			return true;
		}
	case ASS_GR_INSTANT_UPDATE:		//��ʱ���·������
		{
			if (uDataSize!=sizeof(MSG_GR_R_InstantUpdate)) return false;
			MSG_GR_R_InstantUpdate * pInstantUpdate=(MSG_GR_R_InstantUpdate*)pNetData;
			//��������
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pInstantUpdate->dwUserID);
			if (pUserItem==NULL)
				return false;
			//if(m_pRoomInfo->ComRoomInfo.uComType == TY_MONEY_GAME) 
			pUserItem->GameUserInfo.i64Money += pInstantUpdate->dwMoney ;
			pUserItem->GameUserInfo.dwPoint += pInstantUpdate->dwPoint ;
			//���˱������߸���
			//ע�⣺���ѵ��������,Fred Huang 2008-07-17
			//wushuqun 2009.9.5
			if(NULL == m_GameInfo.pMeUserInfo)
			{
				return true;
			}

			if(pInstantUpdate->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
			{
				if(!(m_GameInfo.dwRoomRule & GRR_EXPERCISE_ROOM || m_GameInfo.dwRoomRule & GRR_CONTEST))
					GetMainRoom()->UpdateMeInfo(pUserItem->GameUserInfo.i64Money,true,0,0);
				GetMainRoom()->UpdateMeInfo(0,0,pUserItem->GameUserInfo.dwFascination,true);
			}

			//�����û�
			m_pUserList->UpdateGameUser(pUserItem);
			//if(m_pFriendList!=NULL)
			//	m_pFriendList->UpdateGameUser(pUserItem);
			if ((m_pFindUser!=NULL)&&(m_pFindUser->GetSafeHwnd())) 
				m_pFindUser->UpdateUserItem(pUserItem);

			//���ƿͻ���
			if ((m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=255)
				&&(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==pUserItem->GameUserInfo.bDeskNO))
			{
				CM_UserState_Change ControlMessage;
				ControlMessage.bActionCode=ACT_USER_POINT;
				ControlMessage.bDeskStation=pUserItem->GameUserInfo.bDeskStation;
				ControlMessage.pUserItem=pUserItem;
				ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
			}

			//�Ƿ���Ҫ�뿪��Ϸ��
			//CheckLeaveDesk();
			return TRUE;
		}
	case ASS_GR_USER_POINT:		//�û�����ֵ
		{
			if(pNetHead->bHandleCode==10)//ͬ�����
			{
				//Ч������
				if (uDataSize!=sizeof(MSG_GR_S_RefalshMoney)) return false;
				MSG_GR_S_RefalshMoney * pReflashMoney=(MSG_GR_S_RefalshMoney*)pNetData;

				//��������
				UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pReflashMoney->dwUserID);
				if (pUserItem==NULL) return false;
				if(m_pRoomInfo->pComRoomInfo.uComType!=TY_MONEY_GAME) 
					return false;

				pUserItem->GameUserInfo.i64Money=pReflashMoney->i64Money;

				//�����û�
				m_pUserList->UpdateGameUser(pUserItem);
				//if(m_pFriendList!=NULL)
				//	m_pFriendList->UpdateGameUser(pUserItem);
				if ((m_pFindUser!=NULL)&&(m_pFindUser->GetSafeHwnd()))  m_pFindUser->UpdateUserItem(pUserItem);

				//���ƿͻ���
				if ((m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=255)
					&&(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==pUserItem->GameUserInfo.bDeskNO))
				{
					CM_UserState_Change ControlMessage;
					ControlMessage.bActionCode=ACT_USER_POINT;
					ControlMessage.bDeskStation=pUserItem->GameUserInfo.bDeskStation;
					ControlMessage.pUserItem=pUserItem;
					ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
				}
				return true;
			}
			else if(pNetHead->bHandleCode==11)//ͬ������ֵ
			{
				//Ч������
				if (uDataSize!=sizeof(MSG_GR_S_RefalshMoney)) return false;
				MSG_GR_S_RefalshMoney * pReflashMoney=(MSG_GR_S_RefalshMoney*)pNetData;

				//��������
				UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pReflashMoney->dwUserID);
				if (pUserItem==NULL) return false;
				pUserItem->GameUserInfo.dwPoint=(int)pReflashMoney->i64Money;

				//�����û�
				m_pUserList->UpdateGameUser(pUserItem);
				//if(m_pFriendList!=NULL)
				//	m_pFriendList->UpdateGameUser(pUserItem);
				if ((m_pFindUser!=NULL)&&(m_pFindUser->GetSafeHwnd()))
					m_pFindUser->UpdateUserItem(pUserItem);

				//���ƿͻ���
				if ((m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=255)
					&&(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==pUserItem->GameUserInfo.bDeskNO))
				{
					CM_UserState_Change ControlMessage;
					ControlMessage.bActionCode=ACT_USER_POINT;
					ControlMessage.bDeskStation=pUserItem->GameUserInfo.bDeskStation;
					ControlMessage.pUserItem=pUserItem;
					ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
				}
				return true;
			}

			//Ч������
			if (uDataSize!=sizeof(MSG_GR_R_UserPoint)) return false;
			MSG_GR_R_UserPoint * pUserPoint=(MSG_GR_R_UserPoint *)pNetData;

			//��������
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserPoint->dwUserID);
			if (pUserItem!=NULL)
			{
				//��������
				//pUserItem->GameUserInfo.dwExperience++;
				pUserItem->GameUserInfo.dwPoint+=pUserPoint->dwPoint;
				pUserItem->GameUserInfo.i64Money+=pUserPoint->dwMoney;
				pUserItem->GameUserInfo.uWinCount+=pUserPoint->bWinCount;
				pUserItem->GameUserInfo.uLostCount+=pUserPoint->bLostCount;
				pUserItem->GameUserInfo.uMidCount+=pUserPoint->bMidCount;
				pUserItem->GameUserInfo.uCutCount+=pUserPoint->bCutCount;
				//��������
				//ע�⣺���ѵ��������,Fred Huang 2008-07-17
				if( (pUserItem->GameUserInfo.dwUserID  == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
					&& !(m_GameInfo.dwRoomRule & GRR_EXPERCISE_ROOM || m_GameInfo.dwRoomRule & GRR_CONTEST))
				{
					GetMainRoom()->UpdateMeInfo(pUserItem->GameUserInfo.i64Money,true,0,0);
					m_GameInfo.pMeUserInfo->GameUserInfo.dwPoint = pUserItem->GameUserInfo.dwPoint;
					m_GameInfo.pMeUserInfo->GameUserInfo.i64Money = pUserItem->GameUserInfo.i64Money;
					m_GameInfo.pMeUserInfo->GameUserInfo.uWinCount = pUserItem->GameUserInfo.uWinCount;
					m_GameInfo.pMeUserInfo->GameUserInfo.uLostCount = pUserItem->GameUserInfo.uLostCount;
					m_GameInfo.pMeUserInfo->GameUserInfo.uMidCount = pUserItem->GameUserInfo.uMidCount;
					m_GameInfo.pMeUserInfo->GameUserInfo.uCutCount = pUserItem->GameUserInfo.uCutCount;
				}

				//�����û�
				m_pUserList->UpdateGameUser(pUserItem);
				//if(m_pFriendList!=NULL)
				//	m_pFriendList->UpdateGameUser(pUserItem);
				if ((m_pFindUser!=NULL)&&(m_pFindUser->GetSafeHwnd())) 
					m_pFindUser->UpdateUserItem(pUserItem);

				//���ƿͻ���
				if ((m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=255)&&(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==pUserItem->GameUserInfo.bDeskNO))
				{
					CM_UserState_Change ControlMessage;
					ControlMessage.bActionCode=ACT_USER_POINT;
					ControlMessage.bDeskStation=pUserItem->GameUserInfo.bDeskStation;
					ControlMessage.pUserItem=pUserItem;
					ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));

					if (pUserPoint->dwSend >= 0)
					{
						CString s;
						s.Format("%-21s  %6I64d  %6I64d", pUserItem->GameUserInfo.nickName, pUserPoint->dwPoint, pUserPoint->dwSend);
						m_TipSendMoney.push_back(s);

						if (m_iDeskPeopleMe == m_TipSendMoney.size())
						{
							s.Format("%-21s  %6s  %6s", "�û���", "����", "���");
							m_TipSendMoney.insert(m_TipSendMoney.begin(), s);
							CString stsend = "\n";
							for (int i = 0; i < m_TipSendMoney.size(); i++)
							{
								stsend += m_TipSendMoney[i];
								stsend += "\n";
							}
							SendMessageToGame(stsend);
						}
					}

				}

// 				if(m_GameInfo.pMeUserInfo->GameUserInfo.i64Money < m_GameInfo.uLessPoint
// 					&& m_GameInfo.uComType==TY_MONEY_GAME &&
// 					pUserItem->GameUserInfo.dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
// 				{
// 					SetTimer(TIME_SLEEP_EXIT_LACK, 5000, NULL);
// 				}
// 
// 				if (m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID == 1012)
// 				{
// 					SetTimer(TIME_SLEEP_EXIT_LACK, 10000, NULL);
// 				}
// 
// 				if (m_GameInfo.uMaxPoint > 0L)
// 				{
// 					if(m_GameInfo.pMeUserInfo->GameUserInfo.i64Money > m_GameInfo.uMaxPoint
// 						&& m_GameInfo.uComType==TY_MONEY_GAME &&
// 						pUserItem->GameUserInfo.dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
// 					{
// 						SetTimer(TIME_SLEEP_EXIT_OVERFLOW, 5000, NULL);
// 					}
// 				}
				

				//��������У���Ҫ�������д��ڵķ���
				//Add by JianGuankun 2012.1.2
				if (m_pBankWndEx)
				{
					if (pUserItem->GameUserInfo.dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
					{
						m_pBankWndEx->SetGameWalletMoney(m_GameInfo.uNameID,m_GameInfo.pMeUserInfo->GameUserInfo.i64Money);
						m_pBankWndEx->UpdateWealthInfor();
					}
				}
				//End add

				//////////////////////////ͬ��������������
				if(pUserItem->GameUserInfo.dwUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
				{
					CMainRoomEx * pMainRoom;
					RoomInfoStruct* pRoomInfo;
					pMainRoom=GetMainRoom();
					if(pMainRoom!=NULL) 
					{
						if(pMainRoom->GetRoomInfoStruct(&pRoomInfo))
						{
							//TCHAR sz[100];
							//wsprintf(sz,"%d",sizeof(pRoomInfo)/sizeof(pRoomInfo[0]));
							//AfxMessageBox(pRoomInfo[0].szProcessName); 

							for (BYTE i=0;i<MAX_GAME_ROOM;i++)
							{
                                g_i64MyMoney = pUserItem->GameUserInfo.i64Money;
								if (pRoomInfo[i].bAccess
									&&(pRoomInfo[i].pComRoomInfo.uRoomID!=m_pRoomInfo->pComRoomInfo.uRoomID)
									&&pRoomInfo[i].pComRoomInfo.uComType==TY_MONEY_GAME)
								{
									pRoomInfo[i].pGameRoomWnd->SendMessage(WM_REFLASHMONEY,0x80000001,0);
									return true;
								}
								if (pRoomInfo[i].bAccess
									&&(pRoomInfo[i].pComRoomInfo.uRoomID!=m_pRoomInfo->pComRoomInfo.uRoomID)
									&&(pRoomInfo[i].pComRoomInfo.uNameID==m_pRoomInfo->pComRoomInfo.uNameID))
								{
									pRoomInfo[i].pGameRoomWnd->SendMessage(WM_REFLASHMONEY,pUserItem->GameUserInfo.dwPoint,1);
									return true;
								}
							}
						}
						else
						{
							return true;
						}
					}
				}
			}

			///////////////////////////

			return true;
		}
	case ASS_GR_USER_CONTEST:
		{
			if (m_GameInfo.dwRoomRule & GRR_CONTEST)
			{
				if (uDataSize != sizeof(MSG_GR_ContestChange)) return false;
				MSG_GR_ContestChange* _p = (MSG_GR_ContestChange*)pNetData;

				//��������
				UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(_p->dwUserID);
				if (pUserItem != NULL)
				{
					pUserItem->GameUserInfo.iContestCount = _p->iContestCount;
					pUserItem->GameUserInfo.i64ContestScore = _p->i64ContestScore;
					pUserItem->GameUserInfo.iRankNum = _p->iRankNum;

					if( pUserItem->GameUserInfo.dwUserID  == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
					{
						m_GameInfo.pMeUserInfo->GameUserInfo.iContestCount = pUserItem->GameUserInfo.iContestCount;
						m_GameInfo.pMeUserInfo->GameUserInfo.i64ContestScore = pUserItem->GameUserInfo.i64ContestScore;
						m_GameInfo.pMeUserInfo->GameUserInfo.iRankNum = pUserItem->GameUserInfo.iRankNum;

						CString str;
						str.Format("DClient:id = %d rank = %d",
							m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID,
							m_GameInfo.pMeUserInfo->GameUserInfo.iRankNum);
						OutputDebugString(str);

						//���ƿͻ���
						if ((m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=255)&&(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==pUserItem->GameUserInfo.bDeskNO))
						{
							CM_UserState_Change ControlMessage;
							ControlMessage.bActionCode = ACT_USER_CONTESTPOINT;
							ControlMessage.bDeskStation=pUserItem->GameUserInfo.bDeskStation;
							ControlMessage.pUserItem=pUserItem;
							ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
						}

					}
					 m_DeskFrame.Invalidate(FALSE);
				}
			}
			

			return true;
		}
	case ASS_GR_UPDATE_CHARM:			//����ֵ
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_Charm_Struct)) return false;
			MSG_GR_Charm_Struct * pCharm=(MSG_GR_Charm_Struct *)pNetData;
			//��������
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pCharm->dwUserID);

			if (pUserItem!=NULL)
			{
				pUserItem->GameUserInfo.dwFascination+=pCharm->iCharmValue;
				////��������
				//if(pUserItem->GameUserInfo.dwUserID  == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
				//	GetMainRoom()->UpdateMeInfo(pUserItem->GameUserInfo.dwMoney,true,0,0);

				//�����û�
				m_pUserList->UpdateGameUser(pUserItem);
				//if(m_pFriendList!=NULL)
				//	m_pFriendList->UpdateGameUser(pUserItem);
				if ((m_pFindUser!=NULL)&&(m_pFindUser->GetSafeHwnd())) 
					m_pFindUser->UpdateUserItem(pUserItem);

				//���ƿͻ���
				if ((m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=255)
					&&(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==pUserItem->GameUserInfo.bDeskNO))
				{
					CM_UserState_Change ControlMessage;
					ControlMessage.bActionCode=ACT_USER_CHARM;
					ControlMessage.bDeskStation=pUserItem->GameUserInfo.bDeskStation;
					ControlMessage.pUserItem=pUserItem;
					ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
				}
			}
			return true;
		}
	case ASS_GR_INVITEUSER:		//�����û�
		{
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_SR_InviteUser)) return false;
			MSG_GR_SR_InviteUser * pInviteUser=(MSG_GR_SR_InviteUser *)pNetData;

			//�ж�����
			if (m_bInvaliMode==INVALI_NO) return true;
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pInviteUser->dwUserID);
			if ((pUserItem==NULL)||(pUserItem->uConnection==CM_ENEMY)) return true;
			if ((m_bInvaliMode==INVALI_ONLY_FRIEND)&&(pUserItem->uConnection!=CM_FRIEND)) return true;

			//�ܾ�����ʱ�����ж�
			CString GameName;
			GameName.Format("%d",m_GameInfo.uNameID);
			CString strPath = CBcfFile::GetAppPath() + GameName;
			strPath += "\\bzGame.ini";
			CTime tm = CTime::GetCurrentTime();
			__int64 tCurTime = tm.GetTime();
			CString RefuseID;
			RefuseID.Format("REFUSE%d",pUserItem->GameUserInfo.dwUserID);
			__int64 tLastTime = GetPrivateProfileInt("RefuseInvite", RefuseID, 0, strPath);
			if(tCurTime < tLastTime) {return true;}

			//�ж�����
			CString strBuffer;
			CString str = fMsg.GetKeyVal("GameRoom","CompetitorInfor1","�û��ǳƣ� %s \n���֣�%d   �ȼ���%s\n�ڵ� %d ����������ͬ��Ϸ,���Ƿ�ͬ��?");
			strBuffer.Format(str,
				pUserItem->GameUserInfo.nickName,pUserItem->GameUserInfo.dwPoint-GetNormalOrderBasePoint(pUserItem->GameUserInfo.dwPoint),
				m_GameInfo.pOrderName(pUserItem->GameUserInfo.dwPoint,m_GameInfo.uNameID),pUserItem->GameUserInfo.bDeskNO+1);
			if(m_pRoomInfo->pComRoomInfo.uComType==TY_MONEY_GAME)
			{
				TCHAR szNum[128]; 
                GetNumString(szNum, pUserItem->GameUserInfo.i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
				str = fMsg.GetKeyVal("GameRoom","CompetitorInfor2","�û��ǳƣ� %s \n���֣�%d   �ȼ���%s ��ң�%s   �Ƹ�����%s\n�ڵ� %d ����������ͬ��Ϸ,���Ƿ�ͬ��?");
				strBuffer.Format(str,
					pUserItem->GameUserInfo.nickName,pUserItem->GameUserInfo.dwPoint-GetNormalOrderBasePoint(pUserItem->GameUserInfo.dwPoint),
					m_GameInfo.pOrderName(pUserItem->GameUserInfo.dwPoint,m_GameInfo.uNameID), szNum,
					GetMoneyOrderName(pUserItem->GameUserInfo.i64Money,m_GameInfo.uNameID),pUserItem->GameUserInfo.bDeskNO+1);
			}
			BZSoundPlay(this, "music/t��ʾ2.mp3", 0);
			//if (AFCMessageBox(strBuffer,m_GameInfo.szGameName,AFC_YESNO)==IDOK)//,MB_ICONQUESTION|MB_YESNO,this)==IDYES)
			if (DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strBuffer)==IDYES)//,MB_ICONQUESTION|MB_YESNO,this)==IDYES)
			{
				BYTE bDeskStation=m_DeskFrame.GetNullStation(pNetHead->bHandleCode);
				if (bDeskStation==255) 
				{
					str = fMsg.GetKeyVal("GameRoom","JoinGameFailed","���� %d ����Ϸ��ʧ��,�´ζ�����һ���");
					strBuffer.Format(str,pNetHead->bHandleCode+1);
					m_pRightWnd->m_MessageHandle.InsertSystemMessage(strBuffer.GetBuffer());
					strBuffer.ReleaseBuffer();
				}
				else 
				{
					BZSoundPlay(this, "music/���ȷ����ť������Ϣ��ʾ��.mp3", 0);
					OnLeftHitDesk(pNetHead->bHandleCode,bDeskStation);
				}
			}
			else
			{
				// �ܾ�����Ϸ���룬���ú���ʱ��
				int nRefuseTimer = fMsg.GetKeyVal("GameRoom","InviteRefuseTimer",5);
				__int64 uEndTime = tCurTime + nRefuseTimer*60;

				CString szEndTime;
				szEndTime.Format("%I64d", uEndTime);
				WritePrivateProfileString("RefuseInvite", RefuseID, szEndTime, strPath);

				CString szFmt = fMsg.GetKeyVal("GameRoom","InviteRefuseTip","���ܾ�������ǳ�[%s]����Ϸ���룬%d����֮�ڣ�����Ҷ��������붼�ᱻ���ԡ�");
				CString szMsg;szMsg.Format(szFmt, pUserItem->GameUserInfo.nickName, nRefuseTimer);
				m_pRightWnd->m_MessageHandle.InsertSystemMessage(szMsg.GetBuffer());
			}

			return true;
		}
	case ASS_GR_ROOM_PASSWORD_SET:
		{
			if (uDataSize < 13)
			{
				char BufID[12] = {0};
				memcpy(BufID,pNetData,uDataSize);
				long UserID = atol(BufID);
				if (m_pLogonInfo->dwUserID != UserID)
				{
					CloseGameClient();
					OnBnClickedClose();
					AfxGetMainWnd()->PostMessage(QUIT_GAMEROOM,(WPARAM)1,(LPARAM)UserID);
				}
				else
				{
					CString str;
					str.Format(fMsg.GetKeyVal("GameRoom","VIPRoomSetPWSucess","�޸ķ�������ɹ������֪�������ѡ�"));
					
					//AFCMessageBox(str);
	                DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,str);
				}
				m_RoomOwner = 2;
				return true;
			}
			return false;
		}
	//{add by rongqiufen
	///�����Ŷ�׼��״̬,�����Ի�����ģ�����ŶӰ�ť
	case ASS_GR_ROOM_QUEUE_READY:
		{
			int useid = *(int*)pNetData;
			
			if (m_GameInfo.pMeUserInfo != NULL && m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID == useid)
			{
				if (m_GameInfo.pMeUserInfo->GameUserInfo.bUserState == USER_ARGEE)
				{
					SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
					//if (m_pQueueInRoomDlg != NULL)
					//{
					//	m_pQueueInRoomDlg->ShowWindow(SW_SHOW);
					//	m_pQueueInRoomDlg->OnQueue();
					//}
				}
				else
				{
					CM_UserState_Change ControlMessage;
					ControlMessage.bActionCode = ACT_USER_QUEUE_DISMISS;
					ControlMessage.bDeskStation=m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
					ControlMessage.pUserItem=m_GameInfo.pMeUserInfo;
					ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));

					m_bQueueRoomDisMiss = true;
					if (!m_bQueueRoomDisMiss)
					{
					//	KillTimer(TIME_QUEUEROOM_DISMISS);
					//	SetTimer(TIME_QUEUEROOM_DISMISS, 5000, NULL);
					}
				}
			}
			//AfxGetMainWnd()->ShowWindow(SW_SHOWNORMAL);
			//if (m_pQueueInRoomDlg != NULL)
			//{
			//	if (m_pQueueInRoomDlg->GetSafeHwnd() != NULL)
			//	{
			//		m_pQueueInRoomDlg->ShowWindow(SW_SHOW);
			//		m_pQueueInRoomDlg->OnQueue();
			//	}
			//	else
			//	{
			//		CRect ClientRect;
			//		m_DeskFrame.GetClientRect(&ClientRect);
			//		m_pQueueInRoomDlg->Create(IDD_QUEUE_ROOM, this);
			//		//���Ŷӻ��Ի��������ڵײ��м�λ�� add by lxl 2011-1-17
			//		m_pQueueInRoomDlg->MoveWindow(ClientRect.Width()/2-90,ClientRect.Height()-100,260,100);
			//		m_pQueueInRoomDlg->ShowWindow(SW_SHOW);
			//		m_pQueueInRoomDlg->OnQueue();
			//	}	
			//}
			return true;
		}
    case ASS_GR_GET_NICKNAME_ONID:      // PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ�
        {
            if(ASS_GR_GETNICKNAME_SUCCESS == pNetHead->bHandleCode) // �����ɹ�
            {
                // Ч������
                if (uDataSize != sizeof(MSG_GR_S_GetNickNameOnID_t)) return false;
                MSG_GR_S_GetNickNameOnID_t* pGetNickNameOnID = (MSG_GR_S_GetNickNameOnID_t *)pNetData;

				if (m_pPersonBank)
				{
					m_pPersonBank->SetNickName(pGetNickNameOnID->szNickName);
				}
                
				if (m_pBankWnd)
				{
					m_pBankWnd->SetNickName(pGetNickNameOnID->szNickName);
				}

                return true;
            }
            return true;
        }
    break;
    case ASS_GR_OWNER_T_ONE_LEFT_ROOM:  // PengJiLin, 2010-8-26, ����������뿪����
        {
            char BufID[12] = {0};
            memcpy(BufID,pNetData,uDataSize);
            long UserID = atol(BufID);
            if (m_pLogonInfo->dwUserID == UserID)
            {
                SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,1);
                CloseGameClient();
                OnBnClickedClose();
                AfxGetMainWnd()->PostMessage(QUIT_GAMEROOM,(WPARAM)5, 0);
            }
        }
    break;

	case ASS_GR_AVATAR_LOGO_CHANGE: //JianGuankun 2012.6.28,�����ĳ���logo����ı�
		{
			// Ч������
			if (uDataSize != sizeof(MSG_GR_AvatarChange)) return false;
			MSG_GR_AvatarChange* pAvatarChange = (MSG_GR_AvatarChange*)pNetData;

			//������Լ�logo����ı䣬�������������Ϊ��ǰ�Ѵ���logo����ı䣬��ϸ�鿴
			//CMainRoomEx::ProcessMyAvatarChange
			if (m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID == pAvatarChange->dwUserID)
			{
				return true;
			}

			OnAvatarChange(pAvatarChange->dwUserID,pAvatarChange->nNewLogoID);

			return true;
		}
	}
	return false;
}
/// �Ŷӻ��������
bool CGameRoomEx::OnUserActionQueueSitDown(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	int* pData = (int*)pNetData;
	if (pNetHead->uMessageSize - sizeof(NetMessageHead) != sizeof(int) * (m_GameInfo.uDeskPeople + 1)) return false;
	int _DeskIndex = *pData;
	pData++;

	bool bIsSelf = false;
	for (int i = 1; i < m_GameInfo.uDeskPeople + 1; i++)
	{
		UserItemStruct * pUserItem = m_UserManage.FindOnLineUser((long)*pData);
		if (pUserItem != NULL)
		{
			// PengJiLin, 2010-5-19���Ŷӻ�������������ʾ�����Ϣ
			if(pUserItem->GameUserInfo.dwUserID !=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID && m_GameInfo.dwRoomRule & GRR_NOTCHEAT)
					strcpy(pUserItem->GameUserInfo.nickName, "���");

			pUserItem->GameUserInfo.bDeskNO= _DeskIndex;
			pUserItem->GameUserInfo.bDeskStation = i - 1;
			pUserItem->GameUserInfo.bUserState = USER_SITTING;
			
			if ((pUserItem->GameUserInfo.dwUserID == m_pLogonInfo->dwUserID) && (m_bLogonFinish==true))
			{
				bIsSelf = true;
				m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO = _DeskIndex;
				m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation = i - 1;
				m_iQueueStation = i - 1;
			}
		}
		pData++;
	}
	
	if (bIsSelf)
	{
		pData = (int*)pNetData;
		pData++;

		for (int i = 0; i < m_GameInfo.uDeskPeople; i++)
		{
			m_DeskFrame.SetUserItem(0, i, NULL);
		}

		for (int i = 1; i < m_GameInfo.uDeskPeople + 1; i++)
		{
			UserItemStruct * pUserItem = m_UserManage.FindOnLineUser((long)*pData);
			if (pUserItem != NULL)
			{
				m_DeskFrame.SetUserItem(0, pUserItem->GameUserInfo.bDeskStation, pUserItem);
			}
			pData++;
		}

		m_bQueueing = false;
		
		m_DeskFrame.showQueueBtn(false);
		m_DeskFrame.SetQueueTime(0, true);

		//�ر���Ϸ��
		if(m_IGameFrame!=NULL)
		{
			m_IGameFrame->AFCCloseFrame();
		}

		//��ʾ��ʼ��ť
		m_bWaitSit=false;

		m_DeskFrame.SetUserFrame(_DeskIndex);
		//m_GameInfo.bGameStation = 20; 
		StartGameClient();
	
		CM_UserState_Change ControlMessage;
		ControlMessage.bActionCode= ACT_USER_SIT;//USER_ARGEE;
		ControlMessage.bDeskStation= m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
		ControlMessage.pUserItem=m_GameInfo.pMeUserInfo;
		ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));

		AddUserInfoToRecord();
	}
	/*
	//Ч������
	if (uDataSize!=sizeof(MSG_GR_R_UserSit)) 
	{
		return false;
	}

	MSG_GR_R_UserSit * pUserSit=(MSG_GR_R_UserSit *)pNetData;

	if (pUserSit == NULL)
	{
		return true;
	}

	//��������
	UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserSit->dwUserID);

	if (pUserItem!=NULL)
	{
        // PengJiLin, 2010-5-19���Ŷӻ�������������ʾ�����Ϣ
        if(pUserItem->GameUserInfo.dwUserID !=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID && m_GameInfo.dwRoomRule & GRR_NOTCHEAT)
            strcpy(pUserItem->GameUserInfo.nickName, "���");

		//��������
		pUserItem->GameUserInfo.bDeskNO=pUserSit->bDeskIndex;
		pUserItem->GameUserInfo.bDeskStation=pUserSit->bDeskStation;
		pUserItem->GameUserInfo.bUserState=pUserSit->bUserState;

		int a1, b1, a2, b2;
		a1 = m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO;
		b1 = m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
		a2 = pUserItem->GameUserInfo.bDeskNO;
		b2 = pUserItem->GameUserInfo.bDeskStation;
		if (a1 == a2 && a1 != 255 && b1 != 255 && a2 != 255 && b2 != 255)
		{
			m_DeskFrame.SetUserItem(0, pUserItem->GameUserInfo.bDeskStation, pUserItem);
		}
		//m_DeskFrame.SetUserItem(pUserSit->bDeskIndex,pUserSit->bDeskStation,pUserItem);


		//������ʾ
		m_pUserList->UpdateGameUser(pUserItem);

		if ((m_pFindUser!=NULL)&&(m_pFindUser->GetSafeHwnd()))
			m_pFindUser->UpdateUserItem(pUserItem);

		//wushuqun 2009.9.5
		if(NULL == m_GameInfo.pMeUserInfo)
		{
			return true;
		}

		//�ж��Ƿ��Լ�����Ϣ
		if ((pUserSit->dwUserID==m_pLogonInfo->dwUserID)&&(m_bLogonFinish==true))
		{

			m_DeskFrame.showQueueBtn(false);
			m_DeskFrame.SetQueueTime(0, false);

			//�ر���Ϸ��
			if(m_IGameFrame!=NULL)
			{
				m_IGameFrame->AFCCloseFrame();
			}

			//��ʾ��ʼ��ť
			m_bWaitSit=false;

			m_DeskFrame.SetUserFrame(pUserSit->bDeskIndex);

			//m_GameInfo.bGameStation = 20; 
			StartGameClient();
		//}
		//ͬ�����մ�����Ϣ,�����Լ�
		//if(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==pUserSit->bDeskIndex)
		//{
			CM_UserState_Change ControlMessage;
			ControlMessage.bActionCode= ACT_USER_SIT;//USER_ARGEE;
			ControlMessage.bDeskStation=pUserSit->bDeskStation;
			ControlMessage.pUserItem=pUserItem;
			ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
		}

		//������Լ����£�������û���Ϣ��ʱ�� �����Ϣ�ȳ�����ϢҪ����������޷���������Լ����û���Ϣ��yjj 090224 
		if (m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID == pUserSit->dwUserID)
		{
			AddUserInfoToRecord();
		}
	}
	*/
	return true;
}
/// �������
bool CGameRoomEx::OnUserActionSitDown(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	//Ч������
	if (uDataSize!=sizeof(MSG_GR_R_UserSit)) return false;
	MSG_GR_R_UserSit * pUserSit=(MSG_GR_R_UserSit *)pNetData;	

	//��������
	UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserSit->dwUserID);
	if (pUserItem!=NULL)
	{
        // PengJiLin, 2010-9-19, ����������ʾ�����Ϣ
        if(m_GameInfo.dwRoomRule & GRR_NOTCHEAT && pUserItem->GameUserInfo.dwUserID != m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
            strcpy(pUserItem->GameUserInfo.nickName, "���");

		/////////////////////////////////////////
		//��������
		pUserItem->GameUserInfo.bDeskNO=pUserSit->bDeskIndex;
		pUserItem->GameUserInfo.bDeskStation=pUserSit->bDeskStation;
		if (pNetHead->bAssistantID==ASS_GR_USER_SIT) 
		{
			m_DeskFrame.SetLock(pUserSit->bDeskIndex,(pUserSit->bLock==TRUE));
			m_DeskFrame.SetUserItem(pUserSit->bDeskIndex,pUserSit->bDeskStation,pUserItem);
		}
		pUserItem->GameUserInfo.bUserState=pUserSit->bUserState;

		//������ʾ
		m_pUserList->UpdateGameUser(pUserItem);
		//if(m_pFriendList!=NULL)
		//	m_pFriendList->UpdateGameUser(pUserItem);
		if (m_pFindUser!=NULL) m_pFindUser->UpdateUserItem(pUserItem);

		//wushuqun 2009.9.5
		if(NULL == m_GameInfo.pMeUserInfo)
		{
			return true;
		}

		//�ж��Ƿ��Լ�����Ϣ
		if ((pUserSit->dwUserID==m_pLogonInfo->dwUserID)&&(m_bLogonFinish==true))
		{
			//�ж��Ƿ�̨��
			if(pUserSit->bIsDeskOwner) m_GameInfo.bDeskOnwer=true;

			//�ر��Ŷӻ��Ի���,�����ػ�ʱ���ж��Ŷӻ������Ƿ�����ʾ��
			if (m_pQueueInRoomDlg != NULL && m_pQueueInRoomDlg->GetSafeHwnd() != NULL)
			{
				//m_pQueueInRoomDlg->DestroyDlg();
				m_pQueueInRoomDlg->ShowWindow(SW_HIDE);
			}

			//�ر���Ϸ��
			if(m_IGameFrame!=NULL)
			{
				m_IGameFrame->AFCCloseFrame();
				//m_IGameFrame = NULL;
			}

			//m_DeskFrame.ShowBeginBtn(pUserSit->bDeskIndex,pUserSit->bDeskStation,true);

			//��ʾ��ʼ��ť
			m_bWaitSit=false;

			//�ټ��� add by wlr 20090713
			//if (  HUNDRED_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID		//�ټ���
			//	||SUOHA_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID			//�ٱ����
			//	||NIUNIU_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID		//�ٱ�ţţ
			//	||ERZHANGPAI_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID )	//�ٱ������
			char cKey[10];
			CString sPath=CBcfFile::GetAppPath();
			CBcfFile fsr( sPath + "SpecialRule.bcf");
			sprintf(cKey, "%d", m_GameInfo.uNameID);
			int iResult = fsr.GetKeyVal (_T("BJLType"), cKey, 0);

			if (iResult)
			{
				//m_DeskFrame.SetUserFrame(pUserSit->bDeskIndex);
				StartGameClient(); // ��Ϸ����ʼ���̿���win0000.8
				//--------------rewrite by ramon ��Ϸ����ʼ���̿���----------------------------------------------
				//���ƿͻ���
				CM_UserState_Change ControlMessage;
				ControlMessage.bActionCode=ACT_USER_SIT; //ASS_GR_USER_SIT
				ControlMessage.bDeskStation=m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
				ControlMessage.pUserItem=m_GameInfo.pMeUserInfo;
				ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));//win0005
				//--------------------------------------------------------------------

				KillTimer(TIME_NOACTION_EXIT);
				return true;

			}
			//end of �ټ��� add by wlr 20090713

			m_DeskFrame.SetUserFrame(pUserSit->bDeskIndex);
			if(ASS_GR_WATCH_SIT!=pNetHead->bAssistantID && !m_DeskFrame.IsPlayGame(pUserSit->bDeskIndex)
				&& (m_GameInfo.dwRoomRule&GRR_ALL_ON_DESK)==0L)//��Ҫȫ�������������ͻ���
			{
				m_DeskFrame.ShowBeginBtn(pUserSit->bDeskIndex,pUserSit->bDeskStation,true);
				m_bIsSitting = false;
			}
			else 
			{
				//�ŶӺ�
				StartGameClient();
				//���ƿͻ���
				CM_UserState_Change ControlMessage;
				ControlMessage.bActionCode=ACT_WATCH_SIT;
				ControlMessage.bDeskStation=m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
				ControlMessage.pUserItem=m_GameInfo.pMeUserInfo;
				ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
			}

			//20�벻��ʼ,�Զ��뿪
			m_itime=60;
			KillTimer(IDR_EXIT);
			if(m_GameInfo.pMeUserInfo->GameUserInfo.bUserState==USER_SITTING)
			{
				int bDeskIndex=m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO;
				CBaseDeskView * pDesk=m_DeskFrame.GetDeskArrayPtr(bDeskIndex);
				int agreepeople=0;
				if (pDesk!=NULL) 
				{
					m_itime=60;

					for(int i=0;i<m_GameInfo.uDeskPeople;i++)
					{
						if(i==m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation)
							continue;
						if((pDesk->GetUserItem(i)!=NULL)&&pDesk->GetUserItem(i)->GameUserInfo.bUserState==USER_ARGEE)
							agreepeople++;
					}

					//wushuqun 2009.6.5
					//�޸Ĵ˴�������ʼ�󣬵��ĸ���û��ʱ����Զ�T��ȥ��

					/*if(agreepeople==m_pRoomInfo->pComRoomInfo.uDeskPeople-1)
					{
					m_itime=20;								
					}*/
					if (!(m_GameInfo.bGameStation < 20 || m_GameInfo.bGameStation == 23))
						SetTimer(IDR_EXIT,1000,NULL);
				}
			}
		}
		else if(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==pUserSit->bDeskIndex)//ͬ�����մ�����Ϣ
		{
			CM_UserState_Change ControlMessage;
			ControlMessage.bActionCode=(pNetHead->bAssistantID==ASS_GR_USER_SIT)?ACT_USER_SIT:ACT_WATCH_SIT;
			ControlMessage.bDeskStation=pUserSit->bDeskStation;
			ControlMessage.pUserItem=pUserItem;
			ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));

			//������Լ����£�������û���Ϣ��ʱ�� �����Ϣ�ȳ�����ϢҪ����������޷���������Լ����û���Ϣ��yjj 090224 
			if (m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID == pUserSit->dwUserID)
			{
				AddUserInfoToRecord();						
			}

		}

		//�ж��Ƿ�̨������,����ˢ�½���
		if(pUserSit->bIsDeskOwner) 
		{
			m_DeskFrame.SetBasePoint(pUserSit->bDeskIndex,pUserItem->GameUserInfo.uDeskBasePoint,true);
		}

		if(pUserSit->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
		{
			KillTimer(TIME_NOACTION_EXIT);
		}
	}

	return true;
}
/// ���վ��
bool CGameRoomEx::OnUserActionStandUp(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	OutputDebugString("-----�����յ������վ��������Ϣ!-----");
	//Ч������
	if (uDataSize!=sizeof(MSG_GR_R_UserSit)) return false;
	MSG_GR_R_UserSit * pUserUp=(MSG_GR_R_UserSit *)pNetData;

	/// �ж��Ƿ��������Ϸ
	char cKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "SpecialRule.bcf");
	sprintf(cKey, "%d", m_GameInfo.uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), cKey, 0);

	//��������
	UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserUp->dwUserID);

	/// ��������Ƿ��ҵ�������Ҫ������¼�
	if (pUserUp->dwUserID==m_pLogonInfo->dwUserID)
	{
		m_bWaitSit=false;
		m_GameInfo.bDeskOnwer=false;
		m_DeskFrame.SetUserFrame(255);
		GetDlgItem(IDC_BTN_ROOM3)->EnableWindow(TRUE);
		m_bIsSitting = false;
		m_GameInfo.bGameStation = 0;
		
		//if (iResult!=0)
		//{
		//	m_DeskFrame.m_bShowPlaying = false;
		//	OnBnClickedClose();
		//	return true;
		//}
	}
	if (NULL == pUserItem)
	{
		if ((pNetHead->bAssistantID==ASS_GR_USER_UP)
			&& (pUserUp->bDeskIndex != 255 && pUserUp->bDeskStation != 255))
		{
			m_DeskFrame.SetLock(pUserUp->bDeskIndex,(pUserUp->bLock==TRUE));
			m_DeskFrame.SetUserItem(pUserUp->bDeskIndex,pUserUp->bDeskStation,NULL);
		}
		if ((pUserUp->dwUserID != m_pLogonInfo->dwUserID)
			&&(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==pUserUp->bDeskIndex)
			&&(pNetHead->bAssistantID==ASS_GR_USER_UP)////ͬ����������,��һ�����Լ�
			&&(m_GameInfo.dwRoomRule & GRR_ALL_ON_DESK) == 0L)//��Ҫ�����˶�վ����
		{
			m_GameInfo.bGameStation = 0; // Ϊ������ҿ���ȡǮ�����Ǳ��߳���������
			//SendData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
		}
		return true;
	}

	//wushuqun 2008.10.14
	//��ֹ��Ϣ���ε���
	static bool msgBx = false;

	bool bAllAgree=false;
	//wushuqun 2009.9.5
	if(NULL == m_GameInfo.pMeUserInfo)
	{
		return true;
	}
	if (pUserItem!=NULL)
	{
		//ͬ��������
		if ((pUserUp->dwUserID!=m_pLogonInfo->dwUserID)&&
			(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==pUserUp->bDeskIndex)&&
			(pNetHead->bAssistantID==ASS_GR_USER_UP)////ͬ����������,�����Լ�
			&&(m_GameInfo.dwRoomRule & GRR_ALL_ON_DESK) == 0L)//��Ҫ�����˶�վ����
		{
			//ȫͬ���״̬���Լ�Ҳ����
			if(m_DeskFrame.IsAllAgree(pUserUp->bDeskIndex))
			{
				bAllAgree=true;
			}

			if(pUserUp->bIsDeskOwner && pNetHead->bAssistantID==ASS_GR_USER_UP) 
			{// �����뿪, ������һ������
				for(int i=0; i<m_GameInfo.uDeskPeople; i++)
				{
					UserItemStruct * pUserItem = m_DeskFrame.GetUserItem(pUserUp->bDeskIndex, i);
					if(pUserItem)
					{
						if(pUserItem->GameUserInfo.dwUserID == pUserUp->dwUserID)
							continue;
						if(pUserItem->GameUserInfo.dwUserID == m_pLogonInfo->dwUserID)
						{	// ��������
							m_GameInfo.bDeskOnwer = true;
						}
						break;
					}
				}
			}
		}

		//������ʾ
		if ((pUserItem->GameUserInfo.bDeskNO!=255)&&(pUserItem->GameUserInfo.bDeskStation!=255))
		{
			if (pNetHead->bAssistantID==ASS_GR_USER_UP)
			{
				m_DeskFrame.SetLock(pUserUp->bDeskIndex,(pUserUp->bLock==TRUE));
				m_DeskFrame.SetUserItem(pUserUp->bDeskIndex,pUserUp->bDeskStation,NULL);
			}
			if (pUserUp->dwUserID==m_pLogonInfo->dwUserID)
			{
				m_bWaitSit = false;
				m_GameInfo.bDeskOnwer = false;
				m_DeskFrame.SetUserFrame(255);
			}
		}

		if(pUserUp->dwUserID == m_pLogonInfo->dwUserID || //�Լ�����
            ((IsQueueGameRoom()) &&         // PengJiLin, 2010-5-20, �Ŷӻ������������㣬�����Ŷ�
            (pUserUp->dwUserID != m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID) &&
            (USER_ARGEE == m_GameInfo.pMeUserInfo->GameUserInfo.bUserState)) )
		{
			if((m_GameInfo.dwRoomRule & GRR_ALL_ON_DESK) == 0L)
				m_DeskFrame.ShowBeginBtn(pUserUp->bDeskIndex,pUserUp->bDeskStation,false);
			else
			{
				CM_UserState_Change ControlMessage;
				ControlMessage.bActionCode=(pNetHead->bAssistantID==ASS_GR_USER_UP)?ACT_USER_UP:ACT_WATCH_UP;
				ControlMessage.bDeskStation=pUserUp->bDeskStation;
				ControlMessage.pUserItem=pUserItem;
				ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));

				//	//������Լ�վ��������д��Ϣ��ӦΪ��ʱ�����᲻������Ϸ����ֱ�ӻ�����һ���������ڴ˱�����Ϣ yjj 090224
				// if(m_byVideoUse != 0)  // PengJiLin, 2010-5-18, ���¼�����ù���
                //	m_AddRecordModule.SaveRecordFile(Glb().appPath,m_GameInfo.pMeUserInfo->GameUserInfo.szName,m_GameInfo.uNameID);
			}


			if (IsQueueGameRoom())
			{
				m_DeskFrame.SetUserItem(0, pUserUp->bDeskStation, NULL);
			}

			// add xqm 2010-11-24
			if (pUserUp->dwUserID == m_pLogonInfo->dwUserID)
			{
				OutputDebugString("-----�����յ�����Լ��뿪��Ϸ���ӵ���Ϣ!-----");	

				if (IsQueueGameRoom())
				{
					for (int i = 0; i < m_GameInfo.uDeskPeople; i++)
						m_DeskFrame.SetUserItem(0, i, NULL);
					if (m_bQueueing) JoinQueue();
				}

				if (m_IGameFrame!=NULL)
				{			
					m_IGameFrame->AFCCloseFrame();			
				}

				// add xqm 2010-11-25 ����������Ӻ�, �޸ķ�����밴ť��״̬
				if (!IsQueueGameRoom())
					GetDlgItem(IDC_BTN_ROOM3)->EnableWindow(true);
			}

			//wushuqun 2008.10.16
			//�����׳�Ҫֱ��ɾ����Ϸ����

			bool bIsNotEnoughMoney =  (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money<(int)m_GameInfo.uLessPoint&&m_pRoomInfo->pComRoomInfo.uComType==TY_MONEY_GAME);

			//��̭ʱ�䳡û�н������ JianGuankun 2012.2.20
			if (m_pRoomInfo->pComRoomInfo.dwRoomRule & GRR_CONTEST)
			{
				bIsNotEnoughMoney = false;
			}

			if (bIsNotEnoughMoney)
			{
				// xqm 2010-11-24 ���û�����Ǯ���������ֻ���ڸ����û�����Ǵ��� ����Ͳ���������
				//SetTimer(TIME_SLEEP_EXIT_LACK, 5000, NULL);	
				;
			}
			else
			{
				if (m_GameInfo.dwRoomRule & GRR_AUTO_SIT_DESK  ||
					IsQueueGameRoom())     // PengJiLin, 2010-5-20, �Ŷӻ������������㣬�����Ŷ�
				{
					if(m_IGameFrame)
					{
						m_IGameFrame->OnControlMessage(CM_U3D_APPLICATION_QUIT, 0, 0);
						m_IGameFrame->DeleteFrame();
						m_IGameFrame = NULL;
					}
				}
				else
				{
					if(m_IGameFrame)
						m_IGameFrame->AFCCloseFrame();
				}
				if (m_GameInfo.uComType == TY_MATCH_GAME &&
                    !IsQueueGameRoom())     // PengJiLin, 2010-9-17, �����Ŷӻ��������ر�
				{
					OnBnClickedClose();
				}
			}
			
			///�����Ŷӻ��Ի���
			if ((GetSafeHwnd() != NULL && IsWindowVisible()) ||
                (m_GameInfo.uComType == TY_MATCH_GAME))     // PengJiLin, 2010-9-17, ���������Ŷӻ�����
			{
                // PengJiLin, 2010-6-29, ����ս��ʱ�䵽ʱ�����ٽ����Ŷ�
				if (m_pQueueInRoomDlg != NULL && IsQueueGameRoom() && false == m_bGameTimeOut)
				{
					if (m_pQueueInRoomDlg->GetSafeHwnd() == NULL)
					{
						CRect ClientRect;
						m_DeskFrame.GetClientRect(&ClientRect);
						AfxGetMainWnd()->ShowWindow(SW_RESTORE);
						m_pQueueInRoomDlg->Create(IDD_QUEUE_ROOM, this);
						m_pQueueInRoomDlg->SetMessageText(_T("���ŶӲ�����Ϸ��"));
						//���Ŷӻ��Ի��������ڵײ��м�λ�� add by lxl 2011-1-17
						m_pQueueInRoomDlg->MoveWindow(ClientRect.Width()/2-90,ClientRect.Height()-100,260,100);
						//m_pQueueInRoomDlg->ShowWindow(bIsNotEnoughMoney?SW_HIDE:SW_SHOW);
						m_pQueueInRoomDlg->ShowWindow(SW_SHOW);

                        // PengJiLin, 2010-5-20, �Ѿ�ͬ���˾�ֱ�Ӽ����Ŷ�
                        if((false == bIsNotEnoughMoney) &&
                            (USER_ARGEE == m_GameInfo.pMeUserInfo->GameUserInfo.bUserState))
                        {
                            m_pRightWnd->m_MessageHandle.InsertSystemMessage("���������������Ŷ�");
							//m_pQueueInRoomDlg->OnQueue();
							JoinQueue();
                        }
					}
				}
			}
		}
		//���ƿͻ���
		if (pUserUp->dwUserID != m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID &&
			m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO == pUserUp->bDeskIndex)
		{
			CM_UserState_Change ControlMessage;
			ControlMessage.bActionCode=(pNetHead->bAssistantID==ASS_GR_USER_UP)?ACT_USER_UP:ACT_WATCH_UP;
			ControlMessage.bDeskStation=pUserUp->bDeskStation;
			ControlMessage.pUserItem=pUserItem;
			ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
		}

		if(pUserItem->GameUserInfo.i64Money < m_GameInfo.uLessPoint&&m_GameInfo.uComType==TY_MONEY_GAME)
		{
			if(pUserUp->dwUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID  )
			{
				if (! msgBx)
				{
					msgBx = true;
					TCHAR szNum[128];
					GetNumString(szNum, m_GameInfo.uLessPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
					{
						//CString strMessage;
						//strMessage.Format(TEXT("����Ϸ��������Ҫ��%s�Ľ��,���Ľ�Ҳ���!"), szNum);
						//AFCMessageBox(strMessage,m_GameInfo.szGameName);
						///*ConfirmDlg confirmDlg(strMessage,m_GameInfo.szGameName,AFC_YESNO);
						//confirmDlg.DoModal();*/
						//m_GameInfo.bGameStation = 0; // Ϊ������ҿ���ȡǮ�����Ǳ��߳���������
						//OnBnClickedClose();
						return true;
					}
				}
			}
			/// ����ͬ������ҽ��������������
			else if(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO == pUserUp->bDeskIndex) 
			{
				CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

				CString strtemp;
				strtemp = fMsg.GetKeyVal("GameRoom","LackCoinsSystemKick","%s ������ڽ��������,��ϵͳ���������!");
				CString strMessage;
				strMessage.Format(TEXT(strtemp.GetBuffer()),
					pUserItem->GameUserInfo.nickName);
				m_pRightWnd->m_MessageHandle.InsertNormalMessage(strMessage.GetBuffer());
				SendMessageToGame(strMessage);
			}
		}

		//��������
		pUserItem->GameUserInfo.bDeskNO=255;
		pUserItem->GameUserInfo.bDeskStation=255;
		pUserItem->GameUserInfo.bUserState=pUserUp->bUserState;

		if (m_pUserList)
		{
			//���½���
			m_pUserList->UpdateGameUser(pUserItem);
		}
		if (m_pFindUser!=NULL) 
			m_pFindUser->UpdateUserItem(pUserItem);


		//̨���뿪ʱ��ˢ�����汶����ʾ
		if(pUserUp->bIsDeskOwner) 
		{
			m_DeskFrame.SetBasePoint(pUserUp->bDeskIndex,0,false);
		}

		////�Լ�Ҫ����,��Ҫ��ʾ�Լ���˭�뿪
		//if(bAllAgree)
		//{
		//	SendData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
		//}
	}
	if(pUserItem->GameUserInfo.i64Money < m_GameInfo.uLessPoint && pUserUp->dwUserID!=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID )
	{
		if (m_GameInfo.dwRoomRule & GRR_AUTO_SIT_DESK)
		{			
			//SetTimer(TIME_SLEEP_EXIT_LACK, 5000, NULL);
			
			//if(m_IGameFrame)
			//{
			//	m_IGameFrame->DeleteFrame();
			//	m_IGameFrame = NULL;
			//}
			//return true;
		}
	}
	//�����׳���վ�����������˳����䣬Fred Huang 2008-05-16
	//wushuqun 2009.6.6
	if( (m_GameInfo.dwRoomRule & GRR_AUTO_SIT_DESK)&& 
		(pUserUp->bDeskIndex == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO && 
		pUserUp->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID))//�Լ���ͬ�����˳����Źر�
	{
		if(pUserItem->GameUserInfo.i64Money < m_GameInfo.uLessPoint )
		{
			// xqm 2010-11-24 ���û�����Ǯ���������ֻ���ڸ����û�����Ǵ��� ����Ͳ���������
			;//SetTimer(TIME_SLEEP_EXIT_LACK, 5000, NULL);		
		}
		else
			OnBnClickedClose();
	}

	if(pUserUp->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID && m_GameInfo.uNoActionKickTimer > 0)
	{
		SetTimer(TIME_NOACTION_EXIT, 1000*60*m_GameInfo.uNoActionKickTimer, NULL);
	}

	return true;
}
/// ��Ҷ���
bool CGameRoomEx::OnUserActionOffLine(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	//Ч������
	if (uDataSize!=sizeof(MSG_GR_R_UserCut)) return false;
	MSG_GR_R_UserCut * pUserCut=(MSG_GR_R_UserCut *)pNetData;

	//��������
	if (m_dwCurrentUserID==pUserCut->dwUserID) m_dwCurrentUserID=0L;
	if (m_dwTalkTarget==pUserCut->dwUserID)
	{
		m_dwTalkTarget=0L;
		m_strTalkName.Empty();
	}

	//��������
	UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserCut->dwUserID);
	if (pUserItem!=NULL)
	{
		//��������
		pUserItem->GameUserInfo.bUserState=USER_CUT_GAME;

		//������ʾ
		m_pUserList->UpdateGameUser(pUserItem);
		//if(m_pFriendList!=NULL)
		//	m_pFriendList->UpdateGameUser(pUserItem);
		if (m_pFindUser!=NULL) m_pFindUser->UpdateUserItem(pUserItem);
		m_DeskFrame.UpdateDeskView(pUserCut->bDeskNO);

		//���ƿͻ���
		if (m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==pUserCut->bDeskNO)
		{
			CM_UserState_Change ControlMessage;
			ControlMessage.bActionCode=ACT_USER_CUT;
			ControlMessage.bDeskStation=pUserCut->bDeskStation;
			ControlMessage.pUserItem=pUserItem;
			ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
		}
	}

	return true;
}
/// ��ҽ��뷿��
bool CGameRoomEx::OnUserActionCome(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	//Ч������
	if (uDataSize!=sizeof(MSG_GR_R_UserCome)) return false;
	MSG_GR_R_UserCome * pUserCome=(MSG_GR_R_UserCome *)pNetData;

	//wushuqun 2009.9.5
	if(NULL == m_GameInfo.pMeUserInfo)
	{
		return true;
	}
	////////////////////////////////
	///Kylin 20090116 �޸ķ����׳��ɿ����������������ͷ�����⣡
	if(m_GameInfo.dwRoomRule & GRR_NOTCHEAT)
	{
		if(pUserCome->pUserInfoStruct.dwUserID!=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
		{
			wsprintf(pUserCome->pUserInfoStruct.nickName,"���");
			pUserCome->pUserInfoStruct.bLogoID=1;

		}

	}
	//wushuqun 2009.5.19
	//�޸ķ����׳�
	if((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) &&(pUserCome->pUserInfoStruct.dwUserID!=m_pLogonInfo->dwUserID)) 
	{
		strcpy(pUserCome->pUserInfoStruct.nickName,"���");
		pUserCome->pUserInfoStruct.bLogoID = 1;

		//wushuqun 2009.7.2
		//�����γ�Ҫ�������������Ϊ��ͨ�������
		pUserCome->pUserInfoStruct.userType = 0;
	}

	//�Ŷӻ���������ǳƺ�ͷ����һ����
	if (m_GameInfo.dwRoomRule & GRR_NOTCHEAT)
	{
		if(pUserCome->pUserInfoStruct.dwUserID!=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
		{
			strcpy(pUserCome->pUserInfoStruct.nickName,"���");
			pUserCome->pUserInfoStruct.bLogoID = 1;
			pUserCome->pUserInfoStruct.userType = 0;
			pUserCome->pUserInfoStruct.bBoy = true;
		}
	}

	////////////////////////////////
	//��������
	UserItemStruct * pUserItem=m_UserManage.AddGameUser(&pUserCome->pUserInfoStruct,GetUserSortIndex(&pUserCome->pUserInfoStruct),
		GetUserConnection(pUserCome->pUserInfoStruct.dwUserID));
	////////////////////////////////////////////////////////////////////////////
	if(GRR_EXPERCISE_ROOM & m_GameInfo.dwRoomRule || m_GameInfo.dwRoomRule & GRR_CONTEST)
	{
		//ѵ���������ֵ��Ϊ������ֵ
		//pUserItem->GameUserInfo.i64Money=EXPERCISE_MONEY;
	}
	////////////////////////////////////////////////////////////////////////////
	if(pUserItem->GameUserInfo.bLogoID>=0xFF && pUserItem->GameUserInfo.bLogoID<0x200)
		GetMainRoom()->checkUserLogo(pUserItem->GameUserInfo.dwUserID);

	m_pUserList->AddGameUser(pUserItem);

	//��ʾ��Ϣ
	if ((m_pRoomInfo->pComRoomInfo.uComType!=TY_MATCH_GAME)&&(m_bShowUser==TRUE)&&(pUserCome->pUserInfoStruct.dwUserID!=m_pLogonInfo->dwUserID))
	{
		m_pRightWnd->m_MessageHandle.InsertUserName(pUserCome->pUserInfoStruct.nickName);
		m_pRightWnd->m_MessageHandle.InsertNormalMessage(TEXT(" ������"));
	}

	//������ʾ
	UpdateRoomTitle();

	if(pUserCome->pUserInfoStruct.dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID && m_GameInfo.uNoActionKickTimer > 0)
	{
		SetTimer(TIME_NOACTION_EXIT, 1000*60*m_GameInfo.uNoActionKickTimer, NULL);
	}

	return true;
}
/// ����뿪
bool CGameRoomEx::OnUserActionLeave(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	//Ч������
	if (uDataSize!=sizeof(MSG_GR_R_UserLeft)) return false;
	MSG_GR_R_UserLeft * pUserLeft=(MSG_GR_R_UserLeft *)pNetData;

	//��������
	if (m_dwCurrentUserID==pUserLeft->dwUserID) m_dwCurrentUserID=0L;
	if (m_dwTalkTarget==pUserLeft->dwUserID)
	{
		m_dwTalkTarget=0L;
		m_strTalkName.Empty();
	}
	UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserLeft->dwUserID);
	if (pUserItem!=NULL)
	{
		//��ʾ��Ϣ
		if ((m_pRoomInfo->pComRoomInfo.uComType!=TY_MATCH_GAME)&&(m_bShowUser==TRUE)&&(pUserLeft->dwUserID!=m_pLogonInfo->dwUserID))
		{
			m_pRightWnd->m_MessageHandle.InsertUserName(pUserItem->GameUserInfo.nickName);
			m_pRightWnd->m_MessageHandle.InsertNormalMessage(TEXT(" �뿪��"));
		}

		//��������
		if (m_pFindUser!=NULL) m_pFindUser->DeleteUserItem(pUserItem);
		m_pUserList->DeleteGameUser(pUserItem);
		//if(m_pFriendList!=NULL)
		//	m_pFriendList->DeleteGameUser(pUserItem);
		m_UserManage.DeleteGameUser(pUserItem);
	}

	//������ʾ
	UpdateRoomTitle();

	if(pUserLeft->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
	{
		KillTimer(TIME_NOACTION_EXIT);
	}

	return true;
}
/// ��ҵ����ʼ
bool CGameRoomEx::OnUserActionHitBegin(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if(uDataSize!=sizeof(MSG_GM_WatchSet))
		return false;

	//�ټ��� add by wlr 20090713
	//if (  HUNDRED_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID		//�ټ���
	//	||SUOHA_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID			//�ٱ����
	//	||NIUNIU_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID		//�ٱ�ţţ
	//	||ERZHANGPAI_PEOPLE_DESK_GAME_NAME == m_GameInfo.uNameID )	//�ٱ������
	char cKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "SpecialRule.bcf");
	sprintf(cKey, "%d", m_GameInfo.uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), cKey, 0);

	if (iResult)
	{ 
		return true;
	}
	//end of �ټ��� add by wlr 20090713

	MSG_GM_WatchSet* pUserID = (MSG_GM_WatchSet*) pNetData;

	//��������
	UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(pUserID->dwUserID);
	pUserItem->GameUserInfo.bUserState=USER_DESK_AGREE;
	m_DeskFrame.UpdateDeskView(pUserItem->GameUserInfo.bDeskNO);//������ʾ

	//wushuqun 2009.9.5
	if(NULL == m_GameInfo.pMeUserInfo)
	{
		return true;
	}
	//�Ƿ��Լ���������
	if(pUserItem->GameUserInfo.bDeskNO != m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO) 
		return true;
	if(m_DeskFrame.IsAllAgree(pUserItem->GameUserInfo.bDeskNO))//�Ƿ�ȫ����ͬ��
	{
		StartGameClient();
		//���ƿͻ���
		CM_UserState_Change ControlMessage;
		for(int i=0;i<m_GameInfo.uDeskPeople;i++)
		{
			UserItemStruct * pUser = NULL;
			CBaseDeskView * pBaseDesk = m_DeskFrame.GetDeskArrayPtr(pUserItem->GameUserInfo.bDeskNO);
			pUser = pBaseDesk->GetUserItem(i);
			if (pUser != NULL)
			{
				ControlMessage.bActionCode=ACT_USER_SIT;
				ControlMessage.bDeskStation=m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
				ControlMessage.pUserItem=pUser;
				ControlGameClient(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));
			}
		}
	}
	return true;
}
/// ������ñ���
bool CGameRoomEx::OnUserActionSetBasePoint(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if(pNetHead->bHandleCode==0)
	{
		//AFCMessageBox("���ó�����!");
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"���ó�����!");
	}
	if(pNetHead->bHandleCode ==1)
	{	
		if(uDataSize!=sizeof(MSG_GR_S_SetDeskBasePoint)) return true;
		MSG_GR_S_SetDeskBasePoint* deskBasePtr=(MSG_GR_S_SetDeskBasePoint*) pNetData;
		//��������
		UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(deskBasePtr->dwUserID);
		if(pUserItem!=NULL) 
		{
			pUserItem->GameUserInfo.uDeskBasePoint = deskBasePtr->iDeskBasePoint;
			if(deskBasePtr->bDeskOwner==0||pUserItem->GameUserInfo.bDeskNO==255)
				return true;
			int iBasePoint=deskBasePtr->iDeskBasePoint;
			m_DeskFrame.SetBasePoint(pUserItem->GameUserInfo.bDeskNO,deskBasePtr->iDeskBasePoint,true);
			if(iBasePoint*100 > m_GameInfo.pMeUserInfo->GameUserInfo.i64Money)
			{
				CString strMessage;
				TCHAR szNum[128]; 
                GetNumString(szNum, 100*iBasePoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
				strMessage.Format(TEXT("��������������Ҫ%s��100�������ñ����ģ����Ӳ�������Ϸ,��Ǯ��ȡ������!"),
					szNum);
				
				//AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_CONFIRM);
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,strMessage);
				//if(AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_YESNO)==IDOK)//,MB_ICONQUESTION|MB_OKCANCEL,this)==IDOK)
// 				{
// 					//wushuqun 2009.7.10 �޸��û����С�ڷ�����ʱ����ʾ��ʽ
// 					if (m_lessPointType == 1)
// 					{
// 						//Modify by JianGuankun 2012.1.3 ������������
// 						//OnBnClickedBt8();
// 					}
// 					else
// 					{
// 						//������վ��ֵҳ��
// 						ShellExecute(NULL,"open",m_strLessPointTipURL,NULL,NULL,SW_MAXIMIZE);
// 					}
// 				}else 
				{
					SendData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);//վ��
				}
			}
		}
	}
	return true;
}

/// ������´���
bool CGameRoomEx::OnUserActionSitDownError(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	GetDlgItem(IDC_BTN_ROOM3)->EnableWindow(TRUE);
	m_bIsSitting = false;
	//����Ԥ�����½���
	BYTE m_bPreSitDeskIndex_t=m_bPreSitDeskIndex,m_bPreSitDeskStation_t=m_bPreSitDeskStation;
	//if(pNetHead->bHandleCode!=ERR_GR_MATCH_WAIT)//�������Ŷ��в�Ҫ����
	//{
	m_bWaitSit=false;
	if ((m_bPreSitDeskIndex!=255)&&(m_bPreSitDeskStation!=255))
	{
		UserItemStruct * pUserInfo=m_DeskFrame.GetUserItem(m_bPreSitDeskIndex,m_bPreSitDeskStation);
		if ((pUserInfo!=NULL)&&(pUserInfo->GameUserInfo.dwUserID==m_pLogonInfo->dwUserID))
		{
			m_DeskFrame.SetUserItem(m_bPreSitDeskIndex,m_bPreSitDeskStation,NULL);
		}
		m_bPreSitDeskIndex=255;
		m_bPreSitDeskStation=255;
	}
	//}

	//��ʾ������Ϣ
	CString strMessage;
	switch (pNetHead->bHandleCode)
	{
	case ERR_GR_BEGIN_GAME:
		{
			strMessage=TEXT("���´�λ��ʧ��,��Ϸ�Ѿ���ʼ��!");
			break;
		}
	case ERR_GR_ALREAD_USER:
		{
			strMessage=TEXT("���´�λ��ʧ��,�´ζ�����һ���!");
			break;
		}
	case ERR_GR_PASS_ERROR:
		{
			strMessage=TEXT("��Ϸ���������,������Ϸ������������������Я������!");
			break;
		}
	case ERR_GR_IP_SAME:
		{
			strMessage=TEXT("ͬ����Ҳ���������ͬ IP ��ַ�����һ�������Ϸ!");
			break;
		}
	case ERR_GR_IP_SAME_3:
		{
			strMessage=TEXT("ͬ����Ҳ������� IP ��ַǰ��λ��ͬ�����һ�������Ϸ!");
			break;
		}
	case ERR_GR_IP_SAME_4:
		{
			strMessage=TEXT("ͬ����Ҳ�������IP ��ַǰ��λ��ͬ�����һ�������Ϸ!");
			break;
		}
	case ERR_GR_CUT_HIGH:
		{
			strMessage=TEXT("ͬ�������Ϊ����������̫��,��Ը�������Ϸ!");
			break;
		}
	case ERR_GR_POINT_LOW:
		{
			strMessage = _TEXT("����������õĽ����������������ϱ�����������!");
			break;
		}
	case ERR_GR_POINT_HIGH:		
		{
			strMessage = _TEXT("����������õĽ����������������ϱ�����������!");
			break;
		}
	case ERR_GR_NO_FRIEND:
		{
			strMessage=TEXT("������������ӭ�����!");
			break;
		}
	case ERR_GR_POINT_LIMIT:
		{
			strMessage.Format(TEXT("����Ϸ����Ҫ���� %ld ����Ϸ����,���Ļ��ֲ���,������Ϸ!"),
				m_GameInfo.uLessPoint);
			break;
		}
	case ERR_GR_CAN_NOT_LEFT:
		{
			//Mod-�����й����⴦��-20090512
			//if (m_GameInfo.uNameID == 10003300
			//	|| m_GameInfo.uNameID == 11200405
			//	|| m_GameInfo.uNameID == 11200305
			//	|| m_GameInfo.uNameID == 20013400
			//	|| m_GameInfo.uNameID == 20014410
			//	|| m_GameInfo.uNameID == 10301800	//  lym1023��Ӱټ���֧��
			//	||m_GameInfo.uNameID == 30501800	//��Ӱٱ��������֧��
			//	||m_GameInfo.uNameID == 10901800	//��Ӱٱ�ţţ֧��
			//	||m_GameInfo.uNameID == 11901800	//��Ӱٱ������֧��
			//	||m_GameInfo.uNameID == 10409400)
			char cKey[10];
			CString sPath=CBcfFile::GetAppPath();/////����·��
			CBcfFile fsr( sPath + "SpecialRule.bcf");
			sprintf(cKey, "%d", m_GameInfo.uNameID);
			int iResult = fsr.GetKeyVal (_T("ForceQuitAsAuto"), cKey, 0);

			if (iResult)
			{
				return true;
			}

			strMessage=TEXT("��������Ϸ��...");
			break;
		}
	case ERR_GR_NOT_FIX_STATION:
		{
			strMessage=TEXT("�����ܼ������Ϸ����Ϸ!");
			break;
		}
	case ERR_GR_MATCH_FINISH:
		{
			strMessage=TEXT("���ı����Ѿ�������,���ܼ����μӱ���!");
			break;
		}
	case ERR_GR_MATCH_WAIT://�������Ŷ���
		{
			/*if(m_GameInfo.uComType==TY_MATCH_GAME)
			{
			m_pRightWnd->m_MessageHandle.InsertSystemMessage("Ϊ��ֹ����ϵͳ���Զ�Ϊ��������Ϸ��,�������������Ŀ��ܲ���������ʵλ��,��ȴ�!");
			return true;
			}*/
			break;
		}
	case ERR_GR_UNENABLE_WATCH:
		{
			strMessage=TEXT("�������Թ���Ϸ!");
			break;
		}
		//wushuqun 2009.6.30
		//�˴������ڷ��������緱æ���𣬸�����ʾ��Ϣ
		//default:strMessage=TEXT("������벻��ʶ��,�������Ա��ϵ!");
	default:strMessage=TEXT("��Ǹ,���������緱æ,�����½�����Ϸ�����ѡ����Ϸ��λ��!");
	}

	DUIMessageBox(m_hWnd,MB_ICONERROR|MB_OK,m_GameInfo.szGameName,false,strMessage);
	m_bt3.EnableWindow(TRUE);
	m_bt1.EnableWindow(TRUE);

	return true;
}

//�û���������
bool CGameRoomEx::OnUserActionMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	//״̬�ж�
	if (m_bLogonFinish==false) return true;

	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_USER_HIT_BEGIN:
		{
			return OnUserActionHitBegin(pNetHead, pNetData, uDataSize, pClientSocket);
		}
	case ASS_GR_USER_UP:		//�û�����
	case ASS_GR_WATCH_UP:		//�Թ�����
		{
			return OnUserActionStandUp(pNetHead, pNetData, uDataSize, pClientSocket);
		}
	case ASS_GR_QUEUE_USER_SIT:	//�Ŷӻ��е��û�����
		{
			return OnUserActionQueueSitDown(pNetHead, pNetData, uDataSize, pClientSocket);

		}
	case ASS_GR_USER_SIT:		//�û�����
	case ASS_GR_WATCH_SIT:		//�Թ�����
		{
			return OnUserActionSitDown(pNetHead, pNetData, uDataSize, pClientSocket);
		}
	case ASS_GR_USER_COME:		//�û�����
		{
			return OnUserActionCome(pNetHead, pNetData, uDataSize, pClientSocket);
		}
	case ASS_GR_USER_LEFT:		//�û��뿪
		{
			return OnUserActionLeave(pNetHead, pNetData, uDataSize, pClientSocket);
		}
	case ASS_GR_USER_CUT:		//�û�����
		{
			return OnUserActionOffLine(pNetHead, pNetData, uDataSize, pClientSocket);
		}
	case ASS_GR_SET_TABLE_BASEPOINT:	//�����û����ӱ���
		{	
			return OnUserActionSetBasePoint(pNetHead, pNetData, uDataSize, pClientSocket);
		}
	case ASS_GR_SIT_ERROR:		//���´���
		{
			return OnUserActionSitDownError(pNetHead, pNetData, uDataSize, pClientSocket);
		}
	case ASS_GR_CONTEST_GAMEOVER:
		{
			//AFCMessageBox("�����Ѿ�����,�Ժ󹫲��������!");
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�����Ѿ�����,�Ժ󹫲��������!");
			OnBnClickedClose();
			return true;
		}
	case ASS_GR_CONTEST_KICK:
		{
			//AFCMessageBox("���ź�,�㱻��̭����,��л���Ĳ���!");
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"���ź�,�㱻��̭����,��л���Ĳ���!");
			OnBnClickedClose();
			return true;
		}
	}
	return false;
}



//�Ҽ���������
LRESULT CGameRoomEx::OnRightHitDesk(WPARAM wparam, LPARAM lparam)
{
	if(bExtendRobot)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		//#ifdef _SUPER_CLIENT_
		//����20081212 Fred Huang
		bool bEmptyDesk=true;
		for(int i=0;i<m_GameInfo.uDeskPeople;i++)
		{
			UserItemStruct * pCheckDeskUserItem=m_DeskFrame.GetUserItem((BYTE)wparam,(BYTE)i);
			if (pCheckDeskUserItem!=NULL) //�����������޷�����
				bEmptyDesk=false;
		}
		if (bEmptyDesk)
		{
			m_deskIndex=(BYTE)wparam;
			m_deskStation=(BYTE)lparam;
			CMenu UserInfoMenu;
			UserInfoMenu.CreatePopupMenu();
			UserInfoMenu.AppendMenu(0,IDM_ALONE_DESK,fMsg.GetKeyVal("GameRoom","LockThisDesk","�������!"));
			UserInfoMenu.AppendMenu(0,IDM_ALONE_DESK_ALL,fMsg.GetKeyVal("GameRoom","LockAllDesks","��������!"));
			//��ʾ�˵�
			POINT MousePoint;
			::GetCursorPos(&MousePoint);
			UserInfoMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,MousePoint.x,MousePoint.y,this);
			return 0;
		}
		//�������20081212 Fred Huang
		UserItemStruct *pCheckDeskUserItem=m_DeskFrame.GetUserItem((BYTE)wparam,(BYTE)lparam);
		if(pCheckDeskUserItem && pCheckDeskUserItem->GameUserInfo.dwUserID<0)
		{
			m_deskIndex=(BYTE)wparam;
			m_deskStation=(BYTE)lparam;
			CMenu UserInfoMenu;
			UserInfoMenu.CreatePopupMenu();
			UserInfoMenu.AppendMenu(0,IDM_DEALONE_DESK,fMsg.GetKeyVal("GameRoom","UnlockThisDesk","���������!"));
			UserInfoMenu.AppendMenu(0,IDM_DEALONE_DESK_ALL,fMsg.GetKeyVal("GameRoom","UnlockAllDesk","������з���!"));
			//��ʾ�˵�
			POINT MousePoint;
			::GetCursorPos(&MousePoint);
			UserInfoMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,MousePoint.x,MousePoint.y,this);
			return 0;
		}
		//#endif
	}



	
	//��ȡ�û�����
	if(m_GameInfo.dwMasterPower==0)
		return 0;
	UserItemStruct * pDeskUserItem=m_DeskFrame.GetUserItem((BYTE)wparam,(BYTE)lparam);
	if (pDeskUserItem!=NULL) 
	{
		///�Ŷӻ�����ʾ�����Ϣ
		if (!IsQueueGameRoom())
		{
			OpenUserMenu(pDeskUserItem);
		}
	}
	return 0;
}

//���Ϳ�����Ϣ 
UINT CGameRoomEx::ControlGameClient(UINT uControlCode, void * pControlData, UINT uDataSize)
{
	try
	{
		if (m_IGameFrame!=NULL) m_IGameFrame->OnControlMessage(uControlCode,pControlData,uDataSize);
	}
	catch (...) {}
	return 0;
}

//�汾���
bool CGameRoomEx::GameRoomVerCheck(MSG_S_ConnectSuccess * pVerInfo, bool & bNewVer)
{
	//�汾��
	bNewVer=m_pRoomInfo->dwGameLSVer<pVerInfo->bMaxVer;
	return m_pRoomInfo->dwGameLSVer>=pVerInfo->bLessVer;
}

//���Ҽ��˵�
bool CGameRoomEx::OpenUserMenu(UserItemStruct * pUserItem)
{
	// add duanxiaohui 20111111 ����������ҹ���(����ʹ��, �����ο�����), �ڴ�����һ���µĺ���
	return OpenUserMenuEx(pUserItem,false);
	// end duanxiaohui

	//�ж�״̬
	if (m_bLogonFinish==false) return false;

	if (pUserItem!=NULL)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		//�������
		UserInfoStruct * pUserInfo=&pUserItem->GameUserInfo;
		UserInfoStruct * pMeUserInfo=&m_GameInfo.pMeUserInfo->GameUserInfo;
		m_dwCurrentUserID=pUserInfo->dwUserID;

		//�����˵�
		CString strBuffer;
		CMenu UserInfoMenu;
		UserInfoMenu.CreatePopupMenu();
		MENUINFO mi;
		UserInfoMenu.GetMenuInfo(&mi);
		mi.cbSize=sizeof(MENUINFO);
		mi.fMask=MIM_BACKGROUND; 
		mi.hbrBack = CreateSolidBrush(RGB(0xD1,0xF1,0xFF));
		UserInfoMenu.SetMenuInfo(&mi);

        // PengJiLin, 2010-8-17, �Ƿ��ֹ˽��
        int iForbitSay = 8 & Glb().m_CenterServerPara.m_nFunction;
        BOOL bFirstMenu = FALSE;    // ��һ��λ���ǲ˵�

		//���ܲ˵�
		if (!iForbitSay && m_pLogonInfo->dwUserID!=m_dwCurrentUserID  && !( m_GameInfo.dwRoomRule&GRR_AUTO_SIT_DESK)) //20081127
		{
			CString str;

			UserInfoMenu.AppendMenu(0,IDM_SEND_MESSAGE,fMsg.GetKeyVal("GameRoom","SendMessage","������Ϣ..."));
			UserInfoMenu.AppendMenu(0,IDM_SET_FRIEND,TEXT("��Ϊ����"));
			UserInfoMenu.AppendMenu(0,IDM_SET_EMENY,TEXT("��Ϊ���"));
			UserInfoMenu.SetDefaultItem(IDM_SEND_MESSAGE,MF_BYCOMMAND);
			if(m_GameInfo.uComType==TY_MATCH_GAME&&pMeUserInfo->bGameMaster==0)
				UserInfoMenu.EnableMenuItem(IDM_SEND_MESSAGE,MF_BYCOMMAND|MF_GRAYED);
			if (m_dwTalkTarget!=m_dwCurrentUserID)
			{
				str = fMsg.GetKeyVal("GameRoom","ChatWith","�� %s ����");
				strBuffer.Format(str,pUserInfo->nickName);
			}
			if (m_dwTalkTarget!=0L)
			{
				UserInfoMenu.AppendMenu(0,IDM_CANCEL_TALK,strBuffer);
			}
// 			INT_PTR i;
// 			for (i=0;i<m_ShieldArray.GetCount();i++)
// 			{
// 				if (m_ShieldArray.GetAt(i)==m_dwCurrentUserID) break;
// 			}
// 			if (i==m_ShieldArray.GetCount()) 
// 			{
// 				str = fMsg.GetKeyVal("GameRoom","HideMsg","���� %s ����Ϣ");
// 				strBuffer.Format(str,pUserInfo->nickName);
// 				UserInfoMenu.AppendMenu(0,IDM_SHIELD_USER,strBuffer);
// 			}
// 			else
// 			{
// 				str = fMsg.GetKeyVal("GameRoom","UnHideMsg","��� %s ����Ϣ����");
// 				strBuffer.Format(str,pUserInfo->nickName);
// 				UserInfoMenu.AppendMenu(0,IDM_UNSHIELD_USER,strBuffer);
// 			}
		}
		if (m_pLogonInfo->dwUserID!=m_dwCurrentUserID)
		{
			CString str;
			if ((m_GameInfo.bDeskOnwer==true)&&(pMeUserInfo->bDeskNO==pUserInfo->bDeskNO)
				&&(m_DeskFrame.IsPlayGame(pMeUserInfo->bDeskNO)==false))
			{
				str = fMsg.GetKeyVal("GameRoom","GetOutOfGame","�� %s �뿪����Ϸ��");
				strBuffer.Format(str,pUserInfo->nickName);
				UserInfoMenu.AppendMenu(0,IDM_LEFT_USER,strBuffer);
				UserInfoMenu.EnableMenuItem(IDM_LEFT_USER,MF_BYCOMMAND|MF_GRAYED);
				UserInfoMenu.AppendMenu(0,MF_SEPARATOR);
			}
			if ((pUserInfo->bUserState>=US_PLAY_GAME)&&(pMeUserInfo->bUserState<US_PLAY_GAME)
				&&(CUserPower::CanWatchGame(m_GameInfo.dwGamePower)))
			{
				str = fMsg.GetKeyVal("GameRoom","BesideSee","�Թ���Ϸ");
				//if (!( HUNDRED_PEOPLE_DESK_GAME_NAME == m_pRoomInfo->pComRoomInfo.uNameID		//�ټ���
				//	||SUOHA_PEOPLE_DESK_GAME_NAME == m_pRoomInfo->pComRoomInfo.uNameID			//�ٱ����
				//	||NIUNIU_PEOPLE_DESK_GAME_NAME == m_pRoomInfo->pComRoomInfo.uNameID		//�ٱ�ţţ
				//	||ERZHANGPAI_PEOPLE_DESK_GAME_NAME == m_pRoomInfo->pComRoomInfo.uNameID))	//�ٱ������)// add by ramon 09.05 �ټ���
				char cKey[10];
				CString sPath=CBcfFile::GetAppPath();
				CBcfFile fsr( sPath + "SpecialRule.bcf");
				wsprintf(cKey, "%d", m_pRoomInfo->pComRoomInfo.uNameID);
				int iResult = fsr.GetKeyVal (_T("BJLType"), cKey, 0);

				if (iResult)
                {
					UserInfoMenu.AppendMenu(0,IDM_WATHC_GAME,str);
                    bFirstMenu = TRUE;
                }
			}

            // PengJiLin, 2010-8-26, ���뷿��������������, ���«��������
            if(m_pRoomInfo->pComRoomInfo.bVIPRoom && 2 == m_RoomOwner && 8&Glb().m_CenterServerPara.m_nFunction)
            {
                str = fMsg.GetKeyVal("GameRoom","OwnerTOneLeftRoomMenu","�� %s �뿪����");
                strBuffer.Format(str,pUserInfo->nickName);
                UserInfoMenu.AppendMenu(0,IDM_ONE_LEFT_ROOM,strBuffer);
            }

			if ((pMeUserInfo->bDeskNO!=255)&&(pUserInfo->bDeskNO!=pMeUserInfo->bDeskNO)&&
				(pMeUserInfo->bUserState<US_PLAY_GAME)&&(pUserInfo->bUserState<US_PLAY_GAME))
			{
				str = fMsg.GetKeyVal("GameRoom","Invite","�������");
				UserInfoMenu.AppendMenu(0,IDM_INVITE_PLAY,str);
                bFirstMenu = TRUE;
			}
			/// ���ѹ����Ѿ�ȥ����������Ӻ���
			//str = fMsg.GetKeyVal("GameRoom","AddFriend","��Ӻ���");
			//UserInfoMenu.AppendMenu(0,IDM_SET_FRIEND,str);

			////////////////////////////////////
			////Kylin 20090116 ���β��ܻ�ӭ����
			//UserInfoMenu.AppendMenu(0,IDM_SET_EMENY,TEXT("����Ϊ���ܻ�ӭ"));
			////////////////////////////////////
			if (GetMainRoom()->m_pGameListWnd->m_IMList.GetUserGroup(m_dwCurrentUserID) == 1)
			{
				UserInfoMenu.CheckMenuItem(IDM_SET_FRIEND,MF_BYCOMMAND|MF_CHECKED);
			}
			if (GetMainRoom()->m_pGameListWnd->m_IMList.GetUserGroup(m_dwCurrentUserID) == 3)
			{
				UserInfoMenu.CheckMenuItem(IDM_SET_EMENY,MF_BYCOMMAND|MF_CHECKED);
			}

            if(TRUE == bFirstMenu)
            {
			    UserInfoMenu.AppendMenu(0,MF_SEPARATOR);
            }

//             // PengJiLin, 2010-6-11, Ÿ�ӵ���ʽ��Щ�˵�û��
//             switch(m_byUserListDefineType)
//             {
//             case 1:     // Ÿ��
//                 break;
//             default:
//                 str = fMsg.GetKeyVal("GameRoom","GiftTool","���͵���...");
//                 UserInfoMenu.AppendMenu(0,IDM_GIVE_PROP,str);
//                 break;
//             }
		}
// 		UserInfoMenu.AppendMenu(0,IDM_FIND_ONLINE_USER,fMsg.GetKeyVal("GameRoom","FindUser","�����û�..."));
		if(m_pRoomInfo->pComRoomInfo.uComType==TY_MATCH_GAME || m_GameInfo.dwRoomRule&GRR_AUTO_SIT_DESK)
		{
// 			UserInfoMenu.EnableMenuItem(IDM_FIND_ONLINE_USER,MF_BYCOMMAND|MF_GRAYED);
			UserInfoMenu.EnableMenuItem(IDM_INVITE_PLAY,MF_BYCOMMAND|MF_GRAYED);
			UserInfoMenu.EnableMenuItem(IDM_WATHC_GAME,MF_BYCOMMAND|MF_GRAYED);
		}
		//�û���Ϣ�˵�
		//pMeUserInfo->bGameMaster=9;
		//pUserInfo->bMember=9;


		//UINT uAllCount=pUserInfo->uWinCount+pUserInfo->uLostCount+pUserInfo->uCutCount+pUserInfo->uMidCount;
		////���ڶ��ߡ�ǿ�ˣ������ݿ��������֣����У����ܾ����ﲻ�ٰѶ��ߡ�ǿ��������㷶Χ  modify by lxl 2011-1-4
		UINT uAllCount=pUserInfo->uWinCount+pUserInfo->uLostCount+pUserInfo->uMidCount;
		UINT uCutPercent=(pUserInfo->uCutCount>0)?(UINT)((double)(pUserInfo->uCutCount*100)/(double)uAllCount):0;

		if(UserInfoMenu.GetMenuItemCount() > 0)
		{
			UserInfoMenu.AppendMenu(0,MF_SEPARATOR);
		}

		CString str;

		if (pUserInfo->bMember==0)
		{
			str = fMsg.GetKeyVal("GameRoom","IDNormalPlayer","��ݣ�һ�����");
			strBuffer.Format(str);
		}
		else if (pUserInfo->bMember==1)
		{
			str = fMsg.GetKeyVal("GameRoom", "IDNormalMember", "��ݣ���ͨ��Ա");
			strBuffer.Format(str);
		}
		else if (pUserInfo->bMember==2)
		{
			str = fMsg.GetKeyVal("GameRoom","IDSeniorMember","��ݣ��߼���Ա");
			strBuffer.Format(str);
		}
		else if (pUserInfo->bMember==3)
		{
			str = fMsg.GetKeyVal("GameRoom","IDInvitedGuests","��ݣ������α�");
			strBuffer.Format(str);
		}
		else if (pUserInfo->bMember==4)
		{
			str = fMsg.GetKeyVal("GameRoom","IDForeverMember","��ݣ����û�Ա");
			strBuffer.Format(str);
		}
		else
		{
			str = fMsg.GetKeyVal("GameRoom","IDOldPlayer","��ݣ�Ԫ�����");
			strBuffer.Format(str);
		}

		//////////////////////////////////////////////////////////////////////////
		//Modify By Fred Huang,2008-04-23
		//ԭ���Ǹ���bMember����ʾ�û�����ݣ���member���������޴ӿ�֤������ֻ��VIP���
		//UserInfoMenu.AppendMenu(0,0,strBuffer);

		CString stime;
		stime.Format("%d",CTime::GetCurrentTime());
		int curtime=atoi(stime);
		if(pUserInfo->iVipTime>curtime)
		{
			str = fMsg.GetKeyVal("GameRoom","IDVIPMember","��ݣ�VIP��Ա");
			UserInfoMenu.AppendMenu(0,0,str);
		}
		else
		{
			str = fMsg.GetKeyVal("GameRoom","IDPlayer","��ݣ���ͨ���");
			UserInfoMenu.AppendMenu(0,0,str);
		}
		//////////////////////////////////////////////////////////////////////////

		if (pUserInfo->bDeskNO!=255&&m_pRoomInfo->pComRoomInfo.uComType!=TY_MATCH_GAME)
		{
			str = fMsg.GetKeyVal("GameRoom","DeskNumber","���ţ�%d");
			strBuffer.Format(str,pUserInfo->bDeskNO+1);
			UserInfoMenu.AppendMenu(0,0,strBuffer);
		}
		str = fMsg.GetKeyVal("GameRoom","Score","���֣�%d");
		strBuffer.Format(str,pUserInfo->dwPoint-GetNormalOrderBasePoint(pUserInfo->dwPoint));
		UserInfoMenu.AppendMenu(0,0,strBuffer);
// 		str = fMsg.GetKeyVal("GameRoom","Level","�ȼ���%s");
// 		strBuffer.Format(str,m_GameInfo.pOrderName(pUserInfo->dwPoint));
// 		UserInfoMenu.AppendMenu(0,0,strBuffer);

        // PengJiLin, 2010-6-11, Ÿ�ӵ���ʽ��Щ�˵�û��
        switch(m_byUserListDefineType)
        {
        case 1:     // Ÿ��
            break;
        default:
		if(m_pRoomInfo->pComRoomInfo.uComType==TY_MONEY_GAME)
		{
			TCHAR szNum[128]; 
            GetNumString(szNum, pUserInfo->i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
			str = fMsg.GetKeyVal("GameRoom","Coins","��ң�%s");
			strBuffer.Format(str,szNum);
			UserInfoMenu.AppendMenu(0,0,strBuffer);
			str = fMsg.GetKeyVal("GameRoom","WealthLevel","�Ƹ�����%s");
			strBuffer.Format(str,GetMoneyOrderName(pUserInfo->i64Money,m_GameInfo.uNameID));
			UserInfoMenu.AppendMenu(0,0,strBuffer);
		}
		str = fMsg.GetKeyVal("GameRoom","AllMatch","�ܾ�����%d");
		strBuffer.Format(str,uAllCount);
		UserInfoMenu.AppendMenu(0,0,strBuffer);
		str = fMsg.GetKeyVal("GameRoom","EscapeRate","�����ʣ�%d%%");
		strBuffer.Format(str,uCutPercent);
		UserInfoMenu.AppendMenu(0,0,strBuffer);
            break;
        }
		

		/*		if (pUserInfo->szClassName[0]!=0)
		{
		strBuffer.Format(TEXT("���ţ�%s"),pUserInfo->szClassName);
		UserInfoMenu.AppendMenu(0,0,strBuffer);
		}*/
		if (CUserPower::CanLookIP(m_GameInfo.dwMasterPower)==true)
		{
			ULONG dwUserIP=pUserInfo->dwUserIP;
			str = fMsg.GetKeyVal("GameRoom","UserIP","�û� IP��%d.%d.%d.%d");
			strBuffer.Format(str,FOURTH_IPADDRESS(dwUserIP),THIRD_IPADDRESS(dwUserIP),
				SECOND_IPADDRESS(dwUserIP),FIRST_IPADDRESS(dwUserIP));
			UserInfoMenu.AppendMenu(0,0,strBuffer);
		}
		if (pUserInfo->bGameMaster!=0)
		{
			if (pUserInfo->bGameMaster==1)
			{
				str = fMsg.GetKeyVal("GameRoom","Manage1","����ְ�ƣ���Ϸ�ڹ�");
				UserInfoMenu.AppendMenu(0,0,str);
			}
			else if (pUserInfo->bGameMaster==2)
			{
//				str = fMsg.GetKeyVal("GameRoom","Manage2","����ְ�ƣ���Ϸ���");
//				UserInfoMenu.AppendMenu(0,0,str);
			}
			else if (pUserInfo->bGameMaster==3)
			{
				str = fMsg.GetKeyVal("GameRoom","Manage3","����ְ�ƣ���������Ա");
				UserInfoMenu.AppendMenu(0,0,str);
			}
			else if (pUserInfo->bGameMaster==4)
			{
				str = fMsg.GetKeyVal("GameRoom","Manage4","����ְ�ƣ�����");
				UserInfoMenu.AppendMenu(0,0,str);
			}
			else if (pUserInfo->bGameMaster==5)
			{
				str = fMsg.GetKeyVal("GameRoom","Manage5","����ְ��: ��̫������");
				UserInfoMenu.AppendMenu(0,0,TEXT(""));
			}
			else if (pUserInfo->bGameMaster==6)
			{
				str = fMsg.GetKeyVal("GameRoom","Manage6","����ְ�ƣ��ϵ�");
				UserInfoMenu.AppendMenu(0,0,str);
			}
		}

		//if(m_pLogonInfo->dwUserID==m_dwCurrentUserID)
		//{
		//	UserInfoMenu.AppendMenu(0,MF_SEPARATOR);
		//	UserInfoMenu.AppendMenu(0,IDM_CALL_GM,TEXT("��������"));
		//	UserInfoMenu.AppendMenu(0,MF_SEPARATOR);
		//}
		//��������###########################$$$$$$$$$$$$$$$$$$$$$$$$$
		//if ((m_pLogonInfo->dwUserID!=m_dwCurrentUserID)&&(pMeUserInfo->bGameMaster!=0))
		//TCHAR szBuffer[50];
		//AfxMessageBox(ltoa(pMeUserInfo->bGameMaster,szBuffer,16));
		if (pMeUserInfo->bGameMaster!=0)
		{
			long int dwMasterPorwer=m_GameInfo.dwMasterPower;
			if(m_pLogonInfo->dwUserID!=m_dwCurrentUserID)
			{
				//����˵�
				UserInfoMenu.AppendMenu(0,MF_SEPARATOR);///////////////////////////////////////
				str = fMsg.GetKeyVal("GameRoom","UserRight","�û�Ȩ�޹���...");
				UserInfoMenu.AppendMenu(0,IDM_MANAGER_USER,str);

				if (CUserPower::CanSendWarning(dwMasterPorwer))
				{
					str = fMsg.GetKeyVal("GameRoom","SendWarningToUser","���û����;�����Ϣ...");
					UserInfoMenu.AppendMenu(0,IDM_SEND_WARNNING,str);
				}

				//if (CUserPower::CanListenMessage(dwMasterPorwer))
				//	UserInfoMenu.AppendMenu(0,IDM_LISTEN_MESSAGE,TEXT("��������Ϣ..."));
				//if (CUserPower::CanListenGame(dwMasterPorwer))
				//	UserInfoMenu.AppendMenu(0,IDM_LISTEN_GAME,TEXT("������Ϸ����..."));
				//UserInfoMenu.AppendMenu(0,MF_SEPARATOR);////////////////////////////////////////

				if (CUserPower::CanCutUser(dwMasterPorwer))////////////////////////////////////
				{
					str = fMsg.GetKeyVal("GameRoom","KickUserDown","���û�����...");
					UserInfoMenu.AppendMenu(0,IDM_CUT_USER,str);
				}
				//if (CUserPower::CanSafeFinish(dwMasterPorwer))
				//	UserInfoMenu.AppendMenu(0,IDM_SAFE_FINISH,TEXT("��ȫ�����Ϸ..."));

				//if (CUserPower::CanLookIP(dwMasterPorwer))///////////////////////////
				//	UserInfoMenu.AppendMenu(0,IDM_LOOK_USER_IP,TEXT("�鿴 IP..."));

				if (CUserPower::CanLogonLimit(dwMasterPorwer))
				{
					str = fMsg.GetKeyVal("GameRoom","AllowLogin","�����½���Ʒ���...");
					UserInfoMenu.AppendMenu(0,IDM_LOGON_LIMIT,str);
					UserInfoMenu.EnableMenuItem(IDM_LOGON_LIMIT,MF_BYCOMMAND|MF_GRAYED);
				}

				//if (CUserPower::CanLookPower(dwMasterPorwer))////////////////////////////////
				//	UserInfoMenu.AppendMenu(0,IDM_LOOK_POWER,TEXT("��ѯȨ��..."));
				//if (CUserPower::CanSetPower(dwMasterPorwer))
				//UserInfoMenu.AppendMenu(0,IDM_SET_POWER,TEXT("����Ȩ��..."));

				//if (CUserPower::CanBindGame(dwMasterPorwer))//////////////////////////
				//	UserInfoMenu.AppendMenu(0,IDM_BIND_GAME,TEXT("����Ϸ..."));
				//	if (CUserPower::CanBindAccoun(dwMasterPorwer))
				//		UserInfoMenu.AppendMenu(0,IDM_BIND_ACCOUN,TEXT("���ʺ�..."));
				//	UserInfoMenu.AppendMenu(0,MF_SEPARATOR);////////////////////////////////////

				//if (CUserPower::CanHideAction(dwMasterPorwer))///////////////////////////
				//	UserInfoMenu.AppendMenu(0,IDM_HIDE_ACTION,TEXT("���ض���..."));
				//	if (CUserPower::CanControlUser(dwMasterPorwer))
				//		UserInfoMenu.AppendMenu(0,IDM_CONTROL_USER,TEXT("�����û�..."));
				//	if (CUserPower::CanMuchClient(dwMasterPorwer))
				//		UserInfoMenu.AppendMenu(0,IDM_MUCH_CLIENT,TEXT("�����ͻ���..."));
				//	UserInfoMenu.AppendMenu(0,MF_SEPARATOR);////////////////////////////////////

				//if (CUserPower::CanAddPoint(dwMasterPorwer))//////////////////////////
				//	UserInfoMenu.AppendMenu(0,IDM_ADD_PIONT,TEXT("�Ӽ�����ֵ..."));
				//	UserInfoMenu.AppendMenu(0,MF_SEPARATOR);///////////////////////////////////

				//	if (CUserPower::CanAllPower(dwMasterPorwer))
				//		UserInfoMenu.AppendMenu(0,IDM_ALL_POWER,TEXT("��������Ȩ��..."));
			}
			else
			{

				UserInfoMenu.AppendMenu(0,MF_SEPARATOR);
				if (CUserPower::CanSendSystem(dwMasterPorwer))
				{
					str = fMsg.GetKeyVal("GameRoom","SendMsgToHall","�������ϵͳ��Ϣ");
					UserInfoMenu.AppendMenu(0,IDM_ROOM_SYSTEM,str);
					if (m_bRoomSystem) UserInfoMenu.CheckMenuItem(IDM_ROOM_SYSTEM,MF_BYCOMMAND|MF_CHECKED);
				}
				if (CUserPower::CanSendGame(dwMasterPorwer))
				{
					str = fMsg.GetKeyVal("GameRoom","SendMsgToRoom","����Ϸ���䷢ϵͳ��Ϣ");
					UserInfoMenu.AppendMenu(0,IDM_GAME_SYSTEM,str);
					if (m_bGameSystem) UserInfoMenu.CheckMenuItem(IDM_GAME_SYSTEM,MF_BYCOMMAND|MF_CHECKED);
				}	

				//if (CUserPower::CanSendWarning(dwMasterPorwer))
				//{
				//	UserInfoMenu.AppendMenu(0,IDM_SEND_WARNNING,TEXT("���û����;�����Ϣ..."));
				//}
				//UserInfoMenu.AppendMenu(0,MF_SEPARATOR);
				//if (CUserPower::CanSetServer(dwMasterPorwer))
				//	UserInfoMenu.AppendMenu(0,IDM_SET_SERVER,TEXT("���÷�����..."));
				//if (CUserPower::CanLookIP(dwMasterPorwer))
				//	UserInfoMenu.AppendMenu(0,IDM_LOOK_USER_IP,TEXT("�鿴 IP..."));
				//if (CUserPower::CanLimiteIP(dwMasterPorwer))
				//	UserInfoMenu.AppendMenu(0,IDM_HIDE_IP,TEXT("���� IP..."));
				//if (CUserPower::CanLookPower(dwMasterPorwer))
				//	UserInfoMenu.AppendMenu(0,IDM_LOOK_POWER,TEXT("��ѯȨ��..."));
				//if (CUserPower::CanAddPoint(dwMasterPorwer))
				//	UserInfoMenu.AppendMenu(0,IDM_ADD_PIONT,TEXT("�Ӽ�����ֵ..."));
			}


			//��ֹ�˵�
			//if (CUserPower::CanSendWarning(dwMasterPorwer)==false) UserInfoMenu.EnableMenuItem(IDM_SEND_WARNNING,MF_BYCOMMAND|MF_GRAYED);
			//if (CUserPower::CanSendSystem(dwMasterPorwer)==false) UserInfoMenu.EnableMenuItem(IDM_ROOM_SYSTEM,MF_BYCOMMAND|MF_GRAYED);
			//if (CUserPower::CanSendGame(dwMasterPorwer)==false) UserInfoMenu.EnableMenuItem(IDM_GAME_SYSTEM,MF_BYCOMMAND|MF_GRAYED);

			//if (CUserPower::CanCutUser(dwMasterPorwer)==false) UserInfoMenu.EnableMenuItem(IDM_CUT_USER,MF_BYCOMMAND|MF_GRAYED);
			//if (CUserPower::CanLimiteAccoun(dwMasterPorwer)==false) UserInfoMenu.EnableMenuItem(IDM_LIMIT_ACCOUN,MF_BYCOMMAND|MF_GRAYED);
			//if (CUserPower::CanLimiteIP(dwMasterPorwer)==false) UserInfoMenu.EnableMenuItem(IDM_LIMIT_IP,MF_BYCOMMAND|MF_GRAYED);
			//new

		}

		//��ʾ�˵�
		POINT MousePoint;
		::GetCursorPos(&MousePoint);
		UserInfoMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,MousePoint.x,MousePoint.y,this);
		DeleteObject(mi.hbrBack);
		UserInfoMenu.DestroyMenu();
		return true;
	}

	//�ָ�����
	m_dwCurrentUserID=0;

	return false;
}

//���Ҽ��˵�
bool CGameRoomEx::OpenUserMenuEx(UserItemStruct * pUserItem,bool bInDesk)
{
	// �ж�״̬
	if (m_bLogonFinish == false)
	{
		return false;
	}

	if (pUserItem != NULL)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");

		// �������
		UserInfoStruct * pUserInfo = &pUserItem->GameUserInfo;
		UserInfoStruct * pMeUserInfo = &m_GameInfo.pMeUserInfo->GameUserInfo;
		m_dwCurrentUserID = pUserInfo->dwUserID;

		// �����˵�
		CGameMenu * pMenu = new CGameMenu;
		pMenu->CreatePopupMenu();
		pMenu->SetSelectedBarColor(RGB(0x66, 0x99, 0xff));
		pMenu->SetBackColor(RGB(227, 243, 251));
		pMenu->SetSideBarStartColor(RGB(40, 119, 183));
		pMenu->SetSideBarEndColor(RGB(40, 119, 183));
		pMenu->SetTextSpace(-10);
		pMenu->AddSideBar(new CGameMenuSideBar(14, ""));

		// PengJiLin, 2010-8-17, �Ƿ��ֹ˽��
		int iForbitSay = 8 & Glb().m_CenterServerPara.m_nFunction;

		CString str, strBuffer;

		if (!bInDesk)
		{
			pMenu->AppendODMenu(IDM_USER_INFO, "��ϸ��Ϣ");
		}
		else
		{
			pMenu->AppendODMenu(IDM_USER_INFO_INDESK, "��ϸ��Ϣ");
		}
		
		pMenu->AppendODMenu(IDM_COPY_USER_NAME, "�����û���");
		
		//���ܲ˵�
		if (!iForbitSay && m_pLogonInfo->dwUserID != m_dwCurrentUserID  && !(m_GameInfo.dwRoomRule & GRR_AUTO_SIT_DESK)) // 20081127
		{
			pMenu->AppendSeparator();

			if ((pMeUserInfo->bDeskNO != 255) && (pUserInfo->bDeskNO != pMeUserInfo->bDeskNO)
				&& (pMeUserInfo->bUserState < US_PLAY_GAME) && (pUserInfo->bUserState < US_PLAY_GAME))
			{
				str = fMsg.GetKeyVal("GameRoom", "Invite", "�������");
				pMenu->AppendODMenu(IDM_INVITE_PLAY, str);
				pMenu->AppendSeparator();
			}

			if ((pUserInfo->bUserState >= US_PLAY_GAME) && (pMeUserInfo->bUserState < US_PLAY_GAME)
				&&(CUserPower::CanWatchGame(m_GameInfo.dwGamePower)))
			{
				str = fMsg.GetKeyVal("GameRoom","BesideSee","�Թ۸����");
				pMenu->AppendODMenu(IDM_WATHC_GAME, str);

				char cKey[10];
				CString sPath=CBcfFile::GetAppPath();
				CBcfFile fsr( sPath + "SpecialRule.bcf");
				wsprintf(cKey, "%d", m_pRoomInfo->pComRoomInfo.uNameID);
				int iResult = fsr.GetKeyVal (_T("BJLType"), cKey, 0);
				if (iResult)
				{
					pMenu->EnableMenuItem(IDM_WATHC_GAME, MF_BYCOMMAND | MF_GRAYED);
				}

				pMenu->AppendSeparator();
			}

			str = fMsg.GetKeyVal("GameRoom", "AddFriend", "��Ϊ����");
			pMenu->AppendODMenu(IDM_SET_FRIEND, str);

			str = fMsg.GetKeyVal("GameRoom", "AddEmeny", "���������");
			pMenu->AppendODMenu(IDM_SET_EMENY, str);
		}

		// ��ʾ�˵�
		POINT point;
		::GetCursorPos(&point);
		pMenu->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);
		delete pMenu;
		pMenu = NULL;

		return true;
	}

	//�ָ�����
	m_dwCurrentUserID=0;

	return false;
}

bool CGameRoomEx::IsEnoughMoneyToPlay()
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	bool bIsNotEnoughMoney =  (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money<(int)m_GameInfo.uLessPoint&&m_pRoomInfo->pComRoomInfo.uComType==TY_MONEY_GAME);

	//��̭ʱ�䳡û�н������ JianGuankun 2012.2.20
	if (m_pRoomInfo->pComRoomInfo.dwRoomRule & GRR_CONTEST)
	{
		bIsNotEnoughMoney = false;
	}

	///�Ŷӻ�����,��¼�����ʾ�Ŷӻ��Ի���
	if (IsQueueGameRoom())
	{
		if (m_pQueueInRoomDlg != NULL)
		{
			if (m_pQueueInRoomDlg->GetSafeHwnd() != NULL)
			{
				//m_pQueueInRoomDlg->ShowWindow(bIsNotEnoughMoney?SW_HIDE:SW_SHOW);
				m_pQueueInRoomDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				CRect ClientRect;
				m_DeskFrame.GetClientRect(&ClientRect);
				m_pQueueInRoomDlg->Create(IDD_QUEUE_ROOM, this);
				m_pQueueInRoomDlg->SetMessageText(_T("���ŶӲ�����Ϸ��"));
				//���Ŷӻ��Ի��������ڵײ��м�λ�� add by lxl 2011-1-17
				m_pQueueInRoomDlg->MoveWindow(ClientRect.Width()/2-90,ClientRect.Height()-100,260,100);
				//m_pQueueInRoomDlg->ShowWindow(bIsNotEnoughMoney?SW_HIDE:SW_SHOW);
				m_pQueueInRoomDlg->ShowWindow(SW_SHOW);
			}
		}
	}

	//�жϻ���
	if ((m_GameInfo.uLessPoint!=0))
	{
		CString strBuffer;
		TCHAR szNum[128], szCur[128];
		GetNumString(szNum, m_GameInfo.uLessPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
		CString str = fMsg.GetKeyVal("GameRoom","NeedCoins","�˷�����Ҫ�������� %s �Ľ��");

		if (bIsNotEnoughMoney)
		{
			GetNumString(szCur, m_GameInfo.pMeUserInfo->GameUserInfo.i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
			DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,m_GameInfo.szGameName,true,
				fMsg.GetKeyVal("GameRoom","NeedCoinsRecharge","�˷�����Ҫ���� %s �Ľ�Ҳ��ܿ�ʼ��Ϸ,�����ڵĽ����Ϊ%s����������Ǯ��ȡ�����,��ʼ���ʴ̼�����Ϸ��!"),
				szNum,szCur);
			//if(AFCMessageBox(strBuffer,m_GameInfo.szGameName,AFC_YESNO)==IDOK)
// 			{
// 				//wushuqun 2009.7.10 �޸��û����С�ڷ�����ʱ����ʾ��ʽ
// 				if (m_lessPointType == 1)
// 				{
// 					//Modify by JianGuankun 2012.1.3 ������������
// 					//OnBnClickedBt8();
// 				}
// 				else
// 				{
// 					//������վ��ֵҳ��
// 					ShellExecute(NULL,"open",m_strLessPointTipURL,NULL,NULL,SW_MAXIMIZE);
// 				}
// 			}
// 			else	/// �����ͬ��ȡǮ�����˳�����
			{
				OnBnClickedClose();
			}
		}
	}
	return bIsNotEnoughMoney;
}


bool CGameRoomEx::IsEnoughMoney()
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	bool bIsNotEnoughMoney =  (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money<(int)m_GameInfo.uLessPoint&&m_pRoomInfo->pComRoomInfo.uComType==TY_MONEY_GAME);

	//��̭ʱ�䳡û�н������ JianGuankun 2012.2.20
	if (m_pRoomInfo->pComRoomInfo.dwRoomRule & GRR_CONTEST)
	{
		bIsNotEnoughMoney = false;
	}
	return !bIsNotEnoughMoney;
}
/// ����ȫ�ֱ����еļ��ܷ�ʽ��ȡ�ü��ܺ�������ַ���������sha������MD5
/// �����ﲻ��ָ��ĺϷ������ж�
/// @param szMD5Pass ���ܺ���ַ���
/// @param szSrcPass Դ�ַ���
/// @return ���ܺ���ַ���ָ��
TCHAR *CGameRoomEx::GetCryptedPasswd(TCHAR *szMD5Pass, TCHAR *szSrcPass)
{
	CenterServerMsg& par=Glb().m_CenterServerPara;
	//md5��ʽ
	if( par.m_nEncryptType == 1)
	{
		unsigned char szMDTemp[16];
		MD5_CTX Md5;
		Md5.MD5Update((unsigned char *)szSrcPass,lstrlen(szSrcPass));
		Md5.MD5Final(szMDTemp);
		for (int i = 0; i < 16; i ++) 
			wsprintf(&szMD5Pass[i * 2], "%02x", szMDTemp[i] );
	}
	//sha���ܷ�ʽ
	if(par.m_nEncryptType == 2)
	{
		unsigned szMDTemp[5]={0};
		CString strSha=szSrcPass;
		SHA1 sha;
		sha.Reset();
		sha<<strSha;
		sha.Result(szMDTemp);

		CString strSingle;
		strSha="";
		for (int i=0;i<5;i++) 
		{
			strSingle.Format("%.8x",szMDTemp[i]);
			strSha+=strSingle;
		}
		wsprintf(szMD5Pass,"%s",strSha);
	}
	return szMD5Pass;
}
/// ���н��淢��ָ�����ת�ʼ�¼
/// @return ��
void CGameRoomEx::OnTransferRecord(CTCPClientSocket* pClientSocket,bool bInMainRoom)
{
	MSG_GR_S_TransferRecord_t transferRecord;
	transferRecord.dwUserID = GetMainRoom()->m_PlaceUserInfo.dwUserID;
	/// ֻ���û�ID�Ĵ���������Ϣ
	if (bInMainRoom)
	{
		pClientSocket->SendData(&transferRecord, sizeof(transferRecord),MDM_GP_BANK,ASS_GP_BANK_TRANS_RECORD,0);
	}
	else
	{
		pClientSocket->SendData(&transferRecord, sizeof(transferRecord),MDM_GR_MONEY,ASS_GR_TRANSFER_RECORD,0);
	}
}
/// ���н��淢��ָ��޸�����
/// @param pChangePasswd �޸�����ṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
/// @return ��
void CGameRoomEx::OnChangePasswd( ChangePasswd_t *pChangePasswd )
{
	MSG_GR_S_ChangePasswd_t changePasswd;
	changePasswd.UserID = m_pLogonInfo->dwUserID;
	GetCryptedPasswd(changePasswd.szMD5PassOld, pChangePasswd->szPwdOld);
	GetCryptedPasswd(changePasswd.szMD5PassNew, pChangePasswd->szPwdNew);

	SendData(&changePasswd, sizeof(changePasswd), MDM_GR_MONEY, ASS_GR_CHANGE_PASSWD, 0);
	delete pChangePasswd;
	pChangePasswd = NULL;
}
/// ���н��淢��ָ�ת��
/// @param pTransferMoney ת�ʽṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
/// @return ��
void CGameRoomEx::OnTransferMoney(CTCPClientSocket* pClientSocket,TransferMoney_t *pTransferMoney,bool bInMainRoom,UINT uHandleCode)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	MSG_GR_S_TransferMoney transferMoney;
	transferMoney.i64Money = pTransferMoney->i64Money;
	transferMoney.destUserID = pTransferMoney->uDestUserID;
	transferMoney.UserID = GetMainRoom()->m_PlaceUserInfo.dwUserID;
	transferMoney.bUseDestID = pTransferMoney->bUseDestID;
	strcpy(transferMoney.szNickName,GetMainRoom()->m_PlaceUserInfo.nickName);   //add by lxl 2010-11-17 ת���ǳ���ʾ
	strcpy(transferMoney.szDestNickName, pTransferMoney->szDestNickName);
	if (transferMoney.UserID == transferMoney.destUserID)
	{
		delete pTransferMoney;
		pTransferMoney = NULL;
		CString str = fMsg.GetKeyVal("BankDlg","TransferToSelf","������ת�ʸ��Լ����Բ���ϵͳ�ݲ��ṩ�������");
		TCHAR szMsg[MAX_PATH];
		wsprintf(szMsg, str.GetBuffer());
		DUIOkeyMsgBox(AfxGetMainWnd()->m_hWnd,false,str);
		return;
	}

	if (1&Glb().m_CenterServerPara.m_nFunction) ///< �Ƿ���«2������
	{
		int re = TransferIDCompare(transferMoney.UserID,///< ����ID�Ƿ����ת��
			transferMoney.destUserID,
			Glb().m_CenterServerPara.m_lNomalIDFrom,
			Glb().m_CenterServerPara.m_lNomalIDEnd); 

		if (0 == re)
		{
			delete pTransferMoney;
			return;
		}
	}

	GetCryptedPasswd(transferMoney.szMD5Pass, pTransferMoney->szPWD);

    // HandleCode = 1 ��ʾȫת���ܣ�������ͨת��
	if (bInMainRoom)
	{
		pClientSocket->SendData(&transferMoney,sizeof(transferMoney),MDM_GP_BANK,ASS_GP_BANK_TRANSFER,uHandleCode);
	}
	else
	{
		pClientSocket->SendData(&transferMoney,sizeof(transferMoney),MDM_GR_MONEY,ASS_GR_TRANSFER_MONEY,uHandleCode);
	}
	
	delete pTransferMoney;
	pTransferMoney = NULL;
}

// PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ�
void CGameRoomEx::OnGetNickNameOnID(GetNickNameOnID_t* pGetNickNameOnID)
{
    MSG_GR_S_GetNickNameOnID_t stGetNickNameOnID;
    stGetNickNameOnID.iUserID = pGetNickNameOnID->uUserID;

    SendData(&stGetNickNameOnID, sizeof(stGetNickNameOnID), MDM_GR_ROOM, ASS_GR_GET_NICKNAME_ONID, 0);
    delete pGetNickNameOnID;
    pGetNickNameOnID = NULL;
}

/// ת���û�ID�Ƚ�
/// @param UserID ����Լ���ID��UserIDDestĿ��ID,ɢ��ID NomalIDFrom��ʼ��ɢ��ID NomalIDEnd����
///@return  0����ת�ˣ�1 ����ת��,��˰��2 ����ת��,����˰.3 ����ת�˶�Ϊ��ָ�������ת�˽��
int CGameRoomEx::TransferIDCompare(long UserID,long UserIDDest,long NomalIDFrom,long NomalIDEnd)
{
	char BufID[12]     = {0};
	char BufIDDest[12] = {0};

	BOOL specificID     = TRUE;  ///< �Ƿ�ָ��ID��21111100 21111000���Լ���ID��
	BOOL specificIDDest = TRUE;  ///< �Ƿ�ָ��ID��21111100 21111000���Է���ID��
	BOOL sameNumID      = TRUE;  ///< �Ƿ�����ID��11111 444444���Լ���ID��
	BOOL sameNumIDDest  = TRUE;  ///< �Ƿ�����ID��11111 444444���Է���ID��
	BOOL sameID         = TRUE;  ///< ����ID�Ƿ���ͬ
	int CntZero         = 0;   ///< 0�ĸ���
	int CntZeroDest     = 0;   ///< 0�ĸ���
	int Len             = 0;   ///< ID����
	int LenDest         = 0;   ///< ID����
	int CntSame         = 0;   ///< �Լ�ID���Է�ID��ͬ�ĸ���

	if ((UserID>NomalIDFrom && UserID<NomalIDEnd) && (UserIDDest>NomalIDFrom && UserIDDest<NomalIDEnd))///< ɢ��
	{
		return 3;
	}

	if (UserID<NomalIDFrom && UserIDDest<NomalIDFrom)
	{
		sprintf(BufID,"%ld",UserID);
		sprintf(BufIDDest,"%ld",UserIDDest);

		for (int i=0; 0!=BufID[i] || 0!=BufIDDest[i]; i++)
		{
			if (0!=BufID[i])
			{
				if (specificID)
				{
					if ('0' == BufID[i])
					{
						CntZero++;
					}
					else if ('0' != BufID[i] && 0 != CntZero)
					{
						specificID = FALSE; ///< ����ָ��ID(���ǿͻ�ָ��ID)
					}

					if (i > 0 && BufID[0] != BufID[i] && sameNumID) ///< �Ƿ�����ID
					{
						sameNumID = FALSE;
					}
				}
				Len++;

				if (BufID[i] == BufIDDest[i] && sameID)
				{
					CntSame++;
				}
				else
				{
					sameID = FALSE;
				}
			}

			if (0!=BufIDDest[i])
			{
				if (specificIDDest)
				{
					if ('0' == BufIDDest[i])
					{
						CntZeroDest++;
					}
					else if ('0'!=BufIDDest[i] && 0!=CntZeroDest)
					{
						specificIDDest = FALSE; ///< ����ָ��ID(���ǿͻ�ָ��ID)
					}

					if (i > 0 && BufIDDest[0] != BufIDDest[i] && sameNumIDDest) ///< �Ƿ�����ID
					{
						sameNumIDDest = FALSE;
					}
				}
				LenDest++;
			}
		}

		char BufFromID[12] = {0};
		int  LenFromID     = 0;

		sprintf(BufFromID,"%ld",NomalIDFrom);
		LenFromID = strlen(BufFromID);
		if (LenFromID != LenDest) ///< ���Ȳ�������ָ��ID
		{
			specificIDDest = FALSE;
		}

		if (LenFromID != Len) ///< ���Ȳ�������ָ��ID
		{
			specificID = FALSE;
		}

		if (sameNumID && sameNumIDDest) ///< ����ID��������
		{
		//	if (Len>=LenDest && LenDest>4)
			if (Len>4 && LenDest>4) ///< ��������ID������໥ת��--�߻� ��
			{
				return 2;
			}
		}
		else if (sameNumID && Len>4 && !sameNumIDDest && specificIDDest) ///< �Լ�ID�����ţ��Է�ID��ָ���˺�
		{
			if (CntZeroDest < Len-1)
			{
				if (2 == Len-CntZeroDest)
				{
					if ('4' == BufIDDest[LenFromID-CntZeroDest-1])
					{
						//AFCMessageBox("����ת�ˡ�");
						DUIMessageBox(NULL,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"����ת�ˡ�");
						return 0;
					}
				}

				if (CntZeroDest>0 || '4'==BufIDDest[LenFromID-1])
					return 2;
				else 
					return 1; ///< ����ת�˿�˰
			}
			else
			{
				//AFCMessageBox("���Ų������ϼ�ת�ˡ�");
	            DUIMessageBox(NULL,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"���Ų������ϼ�ת�ˡ�");
				return 0;
			}
		}
		else if (sameNumIDDest && LenDest>4 && !sameNumID && specificID) ///< �Է�������ID���Լ���ָ���˺�
		{
			if (CntZero < LenDest-1)
			{
				if (2 == LenDest-CntZero)
				{
					if ('4' == BufID[LenFromID-CntZero-1])
					{
						//AFCMessageBox("����ת�ˡ�");
	                    DUIMessageBox(NULL,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"����ת�ˡ�");
						return 0;
					}
				}

				if (CntZero>0 || '4'==BufID[LenFromID-1])
					return 2;
				else
					return 2;
			}
			else
			{
				//AFCMessageBox("�Է����ż���С�ڻ�������ļ��𣬲���ת�ˡ�");
	            DUIMessageBox(NULL,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�Է����ż���С�ڻ�������ļ��𣬲���ת�ˡ�");
				return 0;
			}
		}

		if (specificID && specificIDDest)
		{
			for (int i=Len-1; i>=CntSame; i--)
			{
				if ((BufID[i] != BufIDDest[i]) && CntSame == i)
				{
					if ('0' == BufID[i])///< ��ְ
					{
						if ('4' == BufIDDest[i]) ///< ��ְ��ְת��
						{
							return 2;
						}
						else ///< ���¼�
						{
							if (CntZeroDest > 0) ///< �ڼ�����ת��
								return 2;
							else
								return 1;
						}
					}
					else if ('4' == BufID[i]) ///< ��ְ
					{
						if ('0' == BufIDDest[i]) ///< ��ְ����ְת�ˣ�����˰
						{
							return 2;
						}
						else ///< ��ְ���¼�
						{
							if (CntZeroDest > 0) ///< �ڼ�����ת��
								return 2;
							else
								return 1;
						}
					}
					else
					{
						if ('0' == BufIDDest[i] || '4' == BufIDDest[i]) ///< ���ϼ�ת��
						{
							if (CntZero > 0) ///< �ڼ�����ת��
								return 2;
							else
								return 2; ///< �����ת�˲���˰ 2010 5.7 �޸�����;
						}
						else ///< ƽ��
						{
							if (Len-1 - CntSame) ///< ������ƽ��ת��
							{
								return 2;
							}
							else
							{
								//AFCMessageBox("����Ϊ�䣬ͬ������ת�ˡ�");
	                            DUIMessageBox(NULL,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"����Ϊ�䣬ͬ������ת�ˡ�");
								return 0;
							}
						}
					}
				}
				else if (BufID[i] != BufIDDest[i] && CntSame != i)///< ����ͬһ�ϼ�
				{
					//AFCMessageBox("�������µȼ�����ת�ˡ�");
	                DUIMessageBox(NULL,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�������µȼ�����ת�ˡ�");
					return 0;
				}
				else if (BufID[i]==BufIDDest[i] && '0'!=BufID[i])
				{
					//AFCMessageBox("����ת�ˡ�");
	                DUIMessageBox(NULL,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"����ת�ˡ�");
					return 0;
				}
			}
		}
		else if (!specificID && !specificIDDest && specificID && specificIDDest)
		{
			return 3; ///< ���������ر�ID��ת�ˡ�
		}
		else
		{
			//AFCMessageBox("����ת�ˡ�");
	        DUIMessageBox(NULL,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"����ת�ˡ�");
			return 0;
		}
	}

	///< ������ɢ��������֮��ת��
	int RetUserID;
	int RetUserIDDest;
	RetUserID = GetIDInformation(UserID,NomalIDFrom,NomalIDEnd);
	RetUserIDDest = GetIDInformation(UserIDDest,NomalIDFrom,NomalIDEnd);

	if (-1==RetUserID && RetUserIDDest<-1) ///< ɢ��������ת��
	{
		return 2; ///< ����˰
	}
	else if (RetUserID<-1 && -1==RetUserIDDest) ///< ���Ÿ�ɢ��ת��
	{
		return 1; ///< ��˰
	}

	//AFCMessageBox("����ת�ˡ�");
	DUIMessageBox(NULL,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"����ת�ˡ�");
	return 0;
}

/// ��ȡ���ID��Ϣ���Ƿ�ɢ�����ر��˺ţ�ָ���˺�
/// @param UserID ���ID,NomalIDFromɢ��ID��ʼ��NomalIDEndɢ��ID����
/// @return 0��10��ָ���˺ź����0�ĸ�����-1Ϊɢ����-2Ϊ��Ч�˺ţ�-5Ϊ5���ţ�-6Ϊ6����
int CGameRoomEx::GetIDInformation(long UserID,long NomalIDFrom,long NomalIDEnd)
{
	char BufID[12]     = {0};
	int CntZero        = 0;   ///< 0�ĸ���
	int len            = 0;
	BOOL sameNumID     = TRUE;  ///< �Ƿ�����ID��11111 444444���Լ���ID��

	if (UserID>NomalIDFrom && UserID<NomalIDEnd) ///< ɢ��
	{
		return -1;
	}

	if (UserID < NomalIDFrom)
	{
		sprintf(BufID,"%ld",UserID);

		for (int i=0; 0!=BufID[i]; i++)
		{
			if ('0'==BufID[i])
			{
				CntZero++;
			}
			else if ('0'!=BufID[i] && 0!=CntZero) ///< ��Ч�˺�
			{
				return -2;
			}
			
			if (i>0 && BufID[0]!=BufID[i] && sameNumID)
			{
				sameNumID = FALSE;
			}
			len++;
		}

		if (sameNumID) ///< ������ID
		{
			if (len >= 5)
				return -len;
			else
				return -2; ///< ��Ч�˺�
		}

		char BufIDFrom[12] = {0};
		sprintf(BufIDFrom,"%ld",NomalIDFrom);
		if (strlen(BufIDFrom) == len)
		{
			if ('4' == BufID[len-CntZero-1]) ///< ��ְ
			{
				return CntZero+1; ///< 4�ڵ�����CntZero+1λ
			}
			return CntZero;
		}
		else
			return -2; ///< ��Ч�˺�
	}

	return -2; ///< ��Ч�˺�
}


/// @param pCheckMoney ��ȡǮ�ṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
/// @return ��
void CGameRoomEx::OnCheckMoney( CheckMoney_t *pCheckMoney )
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	/// �ж��Ǵ滹��ȡ
	if (pCheckMoney->uType==0) /// ��Ǯ������Ҫ����
	{
		__int64 i64Left = m_GameInfo.pMeUserInfo->GameUserInfo.i64Money - pCheckMoney->i64Money;
		if (i64Left < 0 || i64Left < m_GameInfo.uLessPoint)
		{
			CString strBuffer;
			TCHAR szNum[128];
			GetNumString(szNum, m_GameInfo.uLessPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
			CString str = fMsg.GetKeyVal("GameRoom","NeedCoins","�˷�����Ҫ�������� %s �Ľ��");
			strBuffer.Format(str, szNum);
			
			//AFCMessageBox(strBuffer,m_GameInfo.szGameName);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,strBuffer);
			
			/// �ǵ�ɾ��
			delete pCheckMoney;
			pCheckMoney = NULL;
			return;
		}
		MSG_GR_S_CheckMoney cm;
		cm.i64Count = pCheckMoney->i64Money;
		cm.UserID = m_pLogonInfo->dwUserID;
		SendData(&cm,sizeof(cm),MDM_GR_MONEY,ASS_GR_CHECK_IN,0);
	}
	else ///< ȡǮ����Ҫ����
	{
		MSG_GR_S_CheckMoneyWithPwd_t cm;
		cm.i64Count = pCheckMoney->i64Money;
		cm.UserID = m_pLogonInfo->dwUserID;
		GetCryptedPasswd(cm.szMD5Pass, pCheckMoney->szPWD);
		SendData(&cm,sizeof(cm),MDM_GR_MONEY,ASS_GR_CHECK_OUT,0);
	}
	/// �ǵ�ɾ��
	delete pCheckMoney;
	pCheckMoney = NULL;
}
//��Ϣ����
BOOL CGameRoomEx::PreTranslateMessage(MSG * pMsg)
{
	///��Ϸ���������صĵ�����Ϣ
	if (pMsg->message == WM_GP_PROP)
	{
		if (m_IGameFrame!=NULL) m_IGameFrame->OnControlMessage(CM_USER_PROP,(void *)pMsg->wParam,pMsg->lParam);
	}

	/// ���н���ر�ʱ������Ϣ
	if (pMsg->message == WM_BANK_CLOSE)
	{
		/// �ж�Ǯ�Ƿ񻹲������������ٴ���ʾ
// 		if (m_GameInfo.bGameStation < 20)
// 			IsEnoughMoneyToPlay();

		if (m_pBankWnd)
		{
			delete m_pBankWnd;
			m_pBankWnd = NULL;
		}

		if (m_pBankVerifyWnd)
		{
			delete m_pBankVerifyWnd;
			m_pBankVerifyWnd = NULL;
		}

		if (m_pBankWndEx)
		{	
			delete m_pBankWndEx;
			m_pBankWndEx = NULL;

			if (pMsg->wParam == 1) //ע��,���´򿪵�½��
			{
				OnBnClickedBt8();
			}
		}

		return true;
	}
	
	/// ���н��淢�����޸�����
	if (pMsg->message == WM_CHANGE_PASSWD)
	{
		OnChangePasswd( (ChangePasswd_t *)(pMsg->lParam) );
		return true;
	}
	
	/// ���н��淢����ת�ʼ�¼����
	if (pMsg->message == WM_TRANSFER_RECORD)
	{
		OnTransferRecord(m_TCPSocket,false);
		return true;
	}

	/// ���н��淢����ת��
	if (pMsg->message == WM_TRANSFER_MONEY)
	{
		OnTransferMoney(m_TCPSocket,(TransferMoney_t*)(pMsg->lParam),false);
		return true;
	}
	
	/// ���н��淢���Ĵ�ȡǮ��Ϣ
	if (pMsg->message == WM_CHECK_MONEY)
	{
		OutputDebugString("�����յ���ȡǮ��Ӧ��Ϣlxl");
		OnCheckMoney( (CheckMoney_t *)(pMsg->lParam) );
		return true;
	}

    // PengJiLin, 2010-8-23, ���н��淢���Ļ�ȡ�û��ǳƵ���Ϣ
    if(WM_BANK_GET_NICKNAME_ONID == pMsg->message)
    {
		OutputDebugString("�����յ���ȡ�ǳ���Ӧ��Ϣlxl");
        OnGetNickNameOnID((GetNickNameOnID_t*)(pMsg->lParam));
        return true;
    }

    // PengJiLin, 2010-8-23, ���н��淢����ȫת����
    if(WM_TRANSFER_ALL_MONEY == pMsg->message)
    {
        OnTransferMoney(m_TCPSocket,(TransferMoney_t *)(pMsg->lParam), 1);
        return true;
    }

	//rongqiufen2011.12.7
	if (WM_BANK == pMsg->message)
	{
		if (pMsg->wParam == 0)
		{
			bank_op_normal* p = (bank_op_normal*)(pMsg->lParam);
			SendData(p,sizeof(bank_op_normal),MDM_BANK,ASS_BANK_NORMAL,0);
			delete p;
			p = NULL;
		}
		else if (pMsg->wParam == 1)
		{
			bank_op_deposit* p = (bank_op_deposit*)(pMsg->lParam);
			SendData(p, sizeof(bank_op_deposit), MDM_BANK, ASS_BANK_DEPOSIT, 0);
			delete p;
			p = NULL;
		}
		else if (pMsg->wParam == 2)
		{
			int id = pMsg->lParam;
			SendData(&id, sizeof(int), MDM_BANK, ASS_BANK_WALLET, 0);
		}
		else if (pMsg->wParam == 3)
		{
			int id = pMsg->lParam;
			SendData(&id, sizeof(int), MDM_BANK, ASS_BANK_QUERY_DEPOITS, 0);
		}
		
		return true;
	}

	/// ��Ϸ�����Ѿ��������
	if (pMsg->message == WM_GAME_LAUNCHED)
	{
		m_bIsSitting = false;
		m_DeskFrame.m_bShowPlaying = true;
		return true;
	}
	/// �رմ�����Ϣ
	if (pMsg->message == WM_COLSE_ROOM_WITH_MSG)
	{
		m_DeskFrame.m_bShowPlaying = false;
		OnBnClickedClose();
		return true;
	}
	if (pMsg->message == WM_QUEUEROOM_EXIT)
	{

		if (m_bContinueExit || m_bCloseIsAutoQueue)
		{
			m_bContinueExit = false;
			m_bQueueing = true;
			JoinQueue();
		}
		return true;
	}
	if (pMsg->message == WM_CLOSEGAME)
	{
		 //�ȹر���Ϸ
        if(m_IGameFrame)
        {
            m_IGameFrame->AFCCloseFrame();
        }
		if (m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=255) 
			SendData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
		JoinQueue();
	}
	if(pMsg->message==WM_USER+1000)
	{//�յ���ϷTAB������˫����Ϣ
		m_DeskFrame.m_bShowPlaying = false;
		OnBnClickedClose();
		return TRUE;
	}
	if(pMsg->message==WM_USER+150)
	{//�����������ת����Main Room
		GetMainRoom()->PostMessage(WM_USER+150,pMsg->wParam,pMsg->lParam);
		return TRUE;
	}

    // PengJiLin, 2010-6-29, ��ս��ʱ�䵽���ر���Ϸ
    if(WM_GAME_TIMEOUT == pMsg->message)
    {
        //�ȹر���Ϸ
        if(m_IGameFrame)
        {
            SendData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
            m_IGameFrame->AFCCloseFrame();
        }

        //�����ڻ�ս����Ϸ���䣬��Ҫ�˳��÷���
        CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
        
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","RoomTimeOut","����Ϸ����ʱ���ѵ�����ӭ�´��ٴβμ���Ϸ�"));
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("GameRoom","RoomTimeOut","����Ϸ����ʱ���ѵ�����ӭ�´��ٴβμ���Ϸ�"));
       
		OnBnClickedClose();
        return TRUE;
    }

	//���µ������
	//151:�����˵��ߣ�������Ϣ
	//152:�ڵ��������ʹ�õ���
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;
	if(pMsg->message>WM_USER+150 && pMsg->message<WM_USER+155)
	{
		if(pMsg->message==WM_USER+151)
			m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank-=(int)pMsg->wParam;
		if (m_IGameFrame!=NULL)
			return m_IGameFrame->SendWindowMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
		return TRUE;
	}
	if(pMsg->message==WM_USER+155)
	{
		_TAG_USERPROP *userProp=(_TAG_USERPROP*)pMsg->wParam;
		_TAG_USINGPROP  usingProp;
		ZeroMemory(&usingProp,sizeof(_TAG_USINGPROP));
		////////////////////////////////////////////////////
		///2008 1216 Kylin ����ʱʹ�ø�������û����ʾ
		CString stip;
		str = fMsg.GetKeyVal("GameRoom","WhetherUserTool","��ȷ��Ҫʹ�õ��� %s ��?");
		stip.Format(str,userProp->szPropName);
		if(userProp->attribAction & 0x2)
		{
			if(m_GameInfo.pMeUserInfo->GameUserInfo.dwPoint>=0)
			{
				str = fMsg.GetKeyVal("GameRoom","UseToolWillCostOtherTool",
					"���Ļ���Ϊ����ʹ�õ��� %s ����������һ�����߶�û��Ч����ȷ��ʹ����?");
				stip.Format(str,userProp->szPropName);
			}
			else
			{
				stip += "\n";
				stip += fMsg.GetKeyVal("GameRoom","ClearAllScore","�Ὣ���ĸ���������!");
			}
		}
		
		str = fMsg.GetKeyVal("GameRoom","Tip","��ʾ");
		//if(AFCMessageBox(stip,str,AFC_YESNO)==IDOK)
		if(DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION, str,true,stip)==IDYES)
		{
			usingProp.dwUserID=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID;
			usingProp.dwTargetUserID=usingProp.dwUserID;
			usingProp.nPropID=userProp->nPropID;
			usingProp.nPropActionAttrib=userProp->attribAction;
			usingProp.nPropValueAttrib=userProp->attribValue;
			memcpy(usingProp.szPropName,userProp->szPropName,sizeof(userProp->szPropName));
			SendGameData(&usingProp,sizeof(_TAG_USINGPROP),MDM_GR_PROP,ASS_PROP_USEPROP,0);

		}
		////////////////////////////////////////////////////
		return TRUE;
	}
	if(pMsg->message==WM_USER+156)
	{//�����ȣ�С����
		UseBoardCast((_TAG_USERPROP *)pMsg->wParam);
		return TRUE;
	}
	if(pMsg->message==WM_USER+157)
	{
		OnGiveProp((long)pMsg->wParam,pMsg->lParam);
		return TRUE;
	}

	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_F1))
	{
		return TRUE;
	}

	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN)&&(m_bLogonFinish==true))
	{
		OnSendRoomTalk();
		return TRUE;
	}
	if (pMsg->message==WM_BRING_GAMEWND_TO_TOP)
	{
		if (NULL != m_IGameFrame)
		{
			m_IGameFrame->SendWindowMessage(WM_BRING_GAMEWND_TO_TOP,0,0);
		}
	}
	///GM��Ϣ
	if (pMsg->message == WM_GM_MSG)
	{
		TCHAR *strmsg = (TCHAR*)(pMsg->wParam);
		CString s;
		s.Format("%s", strmsg);
		m_pRightWnd->m_MessageHandle.InsertSystemMessage(s.GetBuffer());
		
		NetMessageHead nethead;
		MSG_GA_S_Message	msg;
		memset(&nethead,0,sizeof(nethead));
		memset(&msg,0,sizeof(msg));
		nethead.bMainID = MDM_GM_MESSAGE;
		nethead.bAssistantID = ASS_GM_SYSTEM_MESSAGE;
		//str = msg//fMsg.GetKeyVal("GameRoom","IfNotStartWillLeave","����������ʼ��Ϸ,10����Զ��뿪.");
		_tcscpy(msg.szMessage, strmsg);
		msg.bShowStation=0x01;
		if (m_IGameFrame!=NULL)
		{
			m_IGameFrame->OnSocketReadEvent(&nethead,&msg,sizeof(MSG_GA_S_Message),NULL);
		}

		delete[] strmsg;
	}
	///�߳�����
	if (pMsg->message == WM_GM_KICK)
	{
		CloseGameClient();
		SendData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
		CloseSocket(false);
		GetMainRoom()->PostMessage(WM_CLOSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
		//OnBnClickedClose();
	}

	m_ToolTip.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//���Ͷ���Ϣ
void CGameRoomEx::SendShortMessage(UserItemStruct * pUserItem)
{
	//Ч������
	if ((pUserItem==NULL)||(pUserItem->GameUserInfo.dwUserID==m_pLogonInfo->dwUserID))
		return;

    // PengJiLin, 2010-9-1, �Ƿ��ֹ˽��
    if (8 & Glb().m_CenterServerPara.m_nFunction)   // ��ֹ˽��
    {
        return;
    }

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	//�Ƿ���Է�����
	if (CUserPower::CanSendMessage(m_GameInfo.dwGamePower)==false)
	{
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","NoSendMsgAuthentification","��Ǹ,������û�з��Ͷ���Ϣ��Ȩ��!"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","NoSendMsgAuthentification","��Ǹ,������û�з��Ͷ���Ϣ��Ȩ��!"));
		return;
	}

	//�ж��Ƿ���ͬһ��Ϸ��,���ϵۿ��Է���
	if ((pUserItem->GameUserInfo.bDeskNO!=255)&&(pUserItem->GameUserInfo.bDeskNO==m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)&&
		(m_GameInfo.pMeUserInfo->GameUserInfo.bGameMaster!=6&&pUserItem->GameUserInfo.bGameMaster!=6))
	{
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","NoChatInSameRoom","��ͬһ����Ϸ�����,�����໥������Ϣ!"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","NoChatInSameRoom","��ͬһ����Ϸ�����,�����໥������Ϣ!"));
		return;
	}

	//������Ϣ����
	CShortMessage * pShortMessageWnd=ActiveShortWnd(pUserItem->GameUserInfo.dwUserID,pUserItem,true);
	if (pShortMessageWnd==NULL)	
	{
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","ClosePartWindows","����̫��,��رղ��ִ���!"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","ClosePartWindows","����̫��,��رղ��ִ���!"));
	}
	return;
}

//���Ͷ���Ϣ��Ϣ
void CGameRoomEx::OnSendShortMessage()
{
	//�ж��Ƿ���Լ�����Ϣ
	if (0L == m_dwCurrentUserID)
		return;
	UserItemStruct * pUserItem = m_UserManage.FindOnLineUser(m_dwCurrentUserID);
	if (NULL == pUserItem)
		pUserItem = m_pUserList->GetUserFromID(m_dwCurrentUserID);
	if (NULL != pUserItem)
		SendShortMessage(pUserItem);

	return;
}

//��������
void CGameRoomEx::OnShieldUser()
{
	//Ч������
	if ((m_dwCurrentUserID==0L)||(m_dwCurrentUserID==m_pLogonInfo->dwUserID)) return;

	//��������
	INT_PTR i;
	for (i=0;i<m_ShieldArray.GetCount();i++)
	{
		if ((m_ShieldArray.GetAt(i)==0L)||(m_ShieldArray.GetAt(i)==m_dwCurrentUserID)) break;
	}
	if (i==m_ShieldArray.GetCount()) m_ShieldArray.SetAtGrow(i,m_dwCurrentUserID);

	//������Ϣ
	UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
	if (pUserItem!=NULL)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString str;
		CString strBuffer;
		str = fMsg.GetKeyVal("GameRoom","ShieldPlayerSuccess","�������[%s]�ɹ�");
		strBuffer.Format(str,pUserItem->GameUserInfo.nickName);
		m_pRightWnd->m_MessageHandle.InsertSystemMessage(strBuffer.GetBuffer());
		strBuffer.ReleaseBuffer();
	}

	return;
}

//�������
void CGameRoomEx::OnUnShieldUser()
{
	//Ч������
	if ((m_dwCurrentUserID==0L)||(m_dwCurrentUserID==m_pLogonInfo->dwUserID)) return;

	//��������
	for (INT_PTR i=0;i<m_ShieldArray.GetCount();i++)
	{
		if (m_ShieldArray.GetAt(i)==m_dwCurrentUserID) 
		{
			m_ShieldArray.RemoveAt(i);
			break;
		}
	}

	//������Ϣ
	UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
	if (pUserItem!=NULL)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString str;
		CString strBuffer;
		str = fMsg.GetKeyVal("GameRoom","UnShieldPlayerSuccess","�������[%s]�ɹ�");
		strBuffer.Format(str,pUserItem->GameUserInfo.nickName);
		m_pRightWnd->m_MessageHandle.InsertSystemMessage(strBuffer.GetBuffer());
		strBuffer.ReleaseBuffer();
	}

	return;
}

//����������Ϣ
void CGameRoomEx::OnSendRoomTalk()
{
	//�ж���Ϣ
	m_pInputMessage->SetFocus();
	if ((m_pInputMessage->GetWindowTextLength()==0)||(m_bLogonFinish==false)) return;

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	TCHAR szMessage[512];
	CString strMsg;
	try
	{
		//�ж��Ƿ��������
		if ((m_GameInfo.dwRoomRule&GRR_FORBID_ROOM_TALK)	
			&&(!m_bRoomSystem &&!m_bGameSystem))		//�ǹ���Ա
		{
			strMsg = fMsg.GetKeyVal("GameRoom","NoChatThisRoom","��Ǹ,�˷����ֹ����!");
			_tcscpy(szMessage, strMsg.GetBuffer());
			m_pRightWnd->m_MessageHandle.InsertSystemMessage(szMessage);
			return;
		}

		//�ж��Ƿ��������
		if (CUserPower::CanRoomTalk(m_GameInfo.dwGamePower)==false)
		{
			strMsg = fMsg.GetKeyVal("GameRoom","NoChatAuthentification","��Ǹ,����ʱû�������Ȩ��!");
			_tcscpy(szMessage, strMsg.GetBuffer());
			m_pRightWnd->m_MessageHandle.InsertSystemMessage(szMessage);
			return;
		}

		//�ж�Ŀ�����
		UserItemStruct * pTargetUser=NULL;
		if ((m_dwTalkTarget!=0L)&&((pTargetUser=m_UserManage.FindOnLineUser(m_dwTalkTarget))==NULL))
		{
			OnCancelTalkObject();
			strMsg = fMsg.GetKeyVal("GameRoom","ErrorLeave","���Ͷ������뿪,����ʧ��!");
			_tcscpy(szMessage, strMsg.GetBuffer());
			m_pRightWnd->m_MessageHandle.InsertSystemMessage(szMessage);
			return;
		}

		//��ȡ��Ϣ
		CString strBuffer;
		m_pInputMessage->GetWindowText(strBuffer);

		//�ж���Ϣ����
		int iMaxSendLen=CUserPower::CanSendMuch(m_GameInfo.dwGamePower)?MAX_TALK_LEN:NORMAL_TALK_LEN;
		if (strBuffer.GetLength()>iMaxSendLen)
		{
			strMsg = fMsg.GetKeyVal("GameRoom","ContentTooLong","���͵�����̫��,��ֶη���!");
			_tcscpy(szMessage, strMsg.GetBuffer());
			m_pRightWnd->m_MessageHandle.InsertSystemMessage(szMessage);
			return;
		}

		//������Ϣ
		if (strBuffer.Find(TEXT("{\\rtf"))!=-1)
		{
			strMsg = fMsg.GetKeyVal("GameRoom","IllegalCharacter","�Ƿ����ַ�,����ʧ��");
			_tcscpy(szMessage, strMsg.GetBuffer());
			m_pRightWnd->m_MessageHandle.InsertSystemMessage(szMessage);
			return;
		}
		for (int i=strBuffer.GetLength();i>0;i--)
		{
			if ((strBuffer[i-1]>=-1)&&(strBuffer[i-1]<30))
			{
				strMsg = fMsg.GetKeyVal("GameRoom","IllegalCharacter","�Ƿ����ַ�,����ʧ��");
				_tcscpy(szMessage, strMsg.GetBuffer());
				m_pRightWnd->m_MessageHandle.InsertSystemMessage(szMessage);
				return;
			}
		}

		//�ж��ٶ�
		if (CUserPower::CanTalkFast(m_GameInfo.dwGamePower)==false)
		{
			long int dwNowTime=(long int)time(NULL);
			if ((m_dwLastTalkTime+1L)>dwNowTime)
			{
				strMsg = fMsg.GetKeyVal("GameRoom","SendTooFast","����˵���ٶ�̫����,��ȿڲ���Ϣ��Ϣ!");
				_tcscpy(szMessage, strMsg.GetBuffer());
				m_pRightWnd->m_MessageHandle.InsertSystemMessage(szMessage);
				return;
			}
			if (((m_dwLastTalkTime+3L)>dwNowTime)&&(m_strLastTalk.Compare(strBuffer)==0))
			{
				strMsg = fMsg.GetKeyVal("GameRoom","SendTooFast","����˵���ٶ�̫����,��ȿڲ���Ϣ��Ϣ!");
				_tcscpy(szMessage, strMsg.GetBuffer());
				m_pRightWnd->m_MessageHandle.InsertSystemMessage(szMessage);
				return;
			}
		}

		//����������Ϣ
		MSG_GR_RS_NormalTalk Talk;
		ZeroMemory(&Talk,sizeof(MSG_GR_RS_NormalTalk));
		Talk.iLength=strBuffer.GetLength();
		Talk.dwSendID=m_pLogonInfo->dwUserID;
		Talk.dwTargetID=m_dwTalkTarget;
		lstrcpy(Talk.szMessage,strBuffer);
		Talk.crColor=m_colorresource.m_crTalkTxColor;

		if(m_bRoomSystem || m_bGameSystem)//������Ϣ
		{
			//wushuqun 2009.6.9
			//�޸Ĵ����ͷ�����Ϣ����

			//if(m_bRoomSystem)
			//	SendData(&Talk,sizeof(Talk)-sizeof(Talk.szMessage)+Talk.iLength+1,MDM_GR_MESSAGE,ASS_GR_SYSTEM_MESSAGE,FALSE);
			//if(m_bGameSystem)//��Ϸ��Ϣ
			//	SendData(&Talk,sizeof(Talk)-sizeof(Talk.szMessage)+Talk.iLength+1,MDM_GM_MESSAGE,ASS_GM_SYSTEM_MESSAGE,FALSE);

			if (m_bRoomSystem)
			{
				// �����е���Ϸ�з���ϵͳ��Ϣ
				GetMainRoom()->SendData(&Talk,sizeof(Talk)-sizeof(Talk.szMessage)+Talk.iLength+1,MDM_GP_MESSAGE,ASS_GP_NEWS_SYSMSG,0);
			}
			else if(m_bGameSystem)//��Ϸ��Ϣ
			{
				SendData(&Talk,sizeof(Talk)-sizeof(Talk.szMessage)+Talk.iLength+1,MDM_GR_MESSAGE,ASS_GR_SYSTEM_MESSAGE,FALSE);			
				SendData(&Talk,sizeof(Talk)-sizeof(Talk.szMessage)+Talk.iLength+1,MDM_GM_MESSAGE,ASS_GM_SYSTEM_MESSAGE,FALSE);
			}

		}
		else
		{
			SendData(&Talk,sizeof(Talk)-sizeof(Talk.szMessage)+Talk.iLength+1,MDM_GR_ROOM,ASS_GR_NORMAL_TALK,0);
		}

		//������Ϣ
		m_strLastTalk=Talk.szMessage;
		if (m_pInputMessage->FindString(0,m_strLastTalk)==-1) m_pInputMessage->InsertString(0,m_strLastTalk);
		if (m_pInputMessage->GetCount()>40) m_pInputMessage->DeleteString(40);

		//���ò���
		m_dwLastTalkTime=(ULONG)time(NULL);
		m_pInputMessage->SetWindowText(TEXT(""));
	}

	catch (...) 
	{
		CString str = fMsg.GetKeyVal("GameRoom","UnknowError","δ֪�쳣����,��Ϣ����ʧ��");
		m_pRightWnd->m_MessageHandle.InsertSystemMessage(str.GetBuffer()); 
	}

	return;
}

//��ȡ��������
UINT CGameRoomEx::GetUserSortIndex(UserInfoStruct * pUserItem)
{
	if (pUserItem->dwUserID==m_pLogonInfo->dwUserID) return SK_ME_USER;
	if (pUserItem->bGameMaster!=0) return SK_MASTER_USER;
	if (pUserItem->bMember==1) return SK_NORMAL_MEMBER;
	if (pUserItem->bMember==2) return SK_HIGHT_MEMBER;
	if (pUserItem->bMember==3) return SK_HIGHT_MEMBER;	                   
	if (pUserItem->bMember==4) return SK_SPECIAL_MEMBER;				
	if (pUserItem->bMember==5) return SK_FOREVER_MEMBER;					
	if (pUserItem->bMember==6) return SK_SENIOR_MEMBER;
	return SK_NORMAL_USER;
}

//��ȡ��ҹ�ϵ
UINT CGameRoomEx::GetUserConnection(long int dwUserID)
{
	return CM_NORMAL;
}

//��ʾ���Ҵ���
bool CGameRoomEx::ShowFindUserWnd()
{
	//��������
	if (m_pFindUser==NULL) 
	{
		try
		{
			m_pFindUser=new CFindUserDlg(this,m_pRoomInfo->pComRoomInfo.uComType);
			if (m_pFindUser==NULL) return false;
		}
		catch (...) { return false; }
	}
	if (m_pFindUser->GetSafeHwnd()==NULL)
	{
		AfxSetResourceHandle(GetModuleHandle(NULL));
		m_pFindUser->Create(IDD_FIND_USER,this);

	}
	m_pFindUser->CenterWindow(GetParent());
	m_pFindUser->ShowWindow(SW_SHOW);
	m_pFindUser->BringWindowToTop();
	m_pFindUser->SetFocus();

	return true;
}

//��ʾ����
void CGameRoomEx::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//��ʾ���ش���
	if (m_Connect.GetSafeHwnd()!=NULL) m_Connect.ShowWindow(bShow?SW_SHOW:SW_HIDE);
	if ((m_pFindUser!=NULL)&&(m_pFindUser->GetSafeHwnd()!=NULL)) m_pFindUser->ShowWindow(bShow?SW_SHOW:SW_HIDE);
    if ((m_pGetHelpMoneyDlg!=NULL)&&(m_pGetHelpMoneyDlg->GetSafeHwnd()!=NULL)) m_pGetHelpMoneyDlg->ShowWindow(bShow?SW_SHOW:SW_HIDE);

	//������ʾ
	if (bShow==TRUE) 
	{
		UpdateRoomTitle();

		///�Ŷӻ�����
		if (IsQueueGameRoom())
		{
			/// �Ŷӻ���Ϸ�У����ش����ٽ���Ϸʱ������ʾ�Ŷӻ�����
			if (NULL == m_GameInfo.pMeUserInfo || m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO != 255)
			{
				return;
			}

			bool bIsNotEnoughMoney =  (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money<(int)m_GameInfo.uLessPoint&&m_pRoomInfo->pComRoomInfo.uComType==TY_MONEY_GAME);

			//��̭ʱ�䳡û�н������ JianGuankun 2012.2.20
			if (m_pRoomInfo->pComRoomInfo.dwRoomRule & GRR_CONTEST)
			{
				bIsNotEnoughMoney = false;
			}

			if (m_pQueueInRoomDlg != NULL)
			{
				///��ʾ�Ŷӻ��Ի���
				if (m_pQueueInRoomDlg->GetSafeHwnd() != NULL)
				{
					//m_pQueueInRoomDlg->ShowWindow(bIsNotEnoughMoney?SW_HIDE:SW_SHOW);
					m_pQueueInRoomDlg->ShowWindow(SW_SHOW);
				}
				else
				{
					CRect ClientRect;
					m_DeskFrame.GetClientRect(&ClientRect);
					m_pQueueInRoomDlg->Create(IDD_QUEUE_ROOM, this);
					m_pQueueInRoomDlg->SetMessageText(_T("���ŶӲ�����Ϸ��"));
					//���Ŷӻ��Ի��������ڵײ��м�λ�� add by lxl 2011-1-17
					m_pQueueInRoomDlg->MoveWindow(ClientRect.Width()/2-90,ClientRect.Height()-100,260,100);
					//m_pQueueInRoomDlg->ShowWindow(bIsNotEnoughMoney?SW_HIDE:SW_SHOW);
					m_pQueueInRoomDlg->ShowWindow(SW_SHOW);
				}				
			}
		}
	}


	return;
}

//�鿴�û�����
void CGameRoomEx::OnUserDetails(bool bInDesk)
{
	if (m_dwCurrentUserID != 0L)
	{
		UserItemStruct * pUserInfo=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
		if (pUserInfo == NULL) 
		{
			return;
		}
			
		m_pUserDetailsDlg->SetUserItem(pUserInfo);
		
		POINT point;
		::GetCursorPos(&point);
		
		CRect rect, listRect;
		m_pUserList->GetWindowRect(&listRect);
		m_pUserDetailsDlg->GetClientRect(&rect);
		if (!bInDesk)
		{
			m_pUserDetailsDlg->MoveWindow(listRect.left - rect.Width() + 15, point.y - 10, rect.Width(), rect.Height());
		}
		else
		{
			m_pUserDetailsDlg->MoveWindow(point.x + 30, point.y - 10, rect.Width(), rect.Height());
		}
		
		m_pUserDetailsDlg->ShowWindow(SW_SHOW);
	}
}

//�����û�����
void CGameRoomEx::OnCopyUserName()
{
	if (m_dwCurrentUserID!=0L)
	{
		UserItemStruct * pUserInfo=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
		if ((pUserInfo==NULL)||(OpenClipboard()==FALSE)) return;
		if (EmptyClipboard()==FALSE) 
		{
			CloseClipboard();
			return;
		}

		//��������
		HANDLE hData=GlobalAlloc(GMEM_MOVEABLE,sizeof(pUserInfo->GameUserInfo.nickName));
		if (hData==NULL) 
		{
			CloseClipboard();
			return;
		}
		LPTSTR szMemName=(LPTSTR)GlobalLock(hData);

		// duanxiaohui 2011-11-21 ���Ƶ�ճ��������Ҫת����Unicode
		//lstrcpy(szMemName,pUserInfo->GameUserInfo.nickName);

		CString strText;
		strText.Format("%s", pUserInfo->GameUserInfo.nickName);
		if (0 == MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strText, strText.GetLength() + 1, (WCHAR*)szMemName,  1024)) 
		{ 
			GlobalUnlock(hData); 
			CloseClipboard(); 
			GlobalFree(hData); 
			return; 
		} 
		// end duanxiaohui

		SetClipboardData(CF_UNICODETEXT, hData);
		GlobalUnlock(hData); 
		GlobalFree(hData);
		CloseClipboard();

		//��ʾ����
		CString strBuffer;
		m_pInputMessage->GetWindowText(strBuffer);
		strBuffer+=pUserInfo->GameUserInfo.nickName;
		m_pInputMessage->SetWindowText(strBuffer);
		m_pInputMessage->SetFocus();
	}

	return;
}

//����ĳ�����Ӻ�λ�Ӻŵ��û�Ϊ��ǰ�û�
void CGameRoomEx::SetCurrentUser(long dwUserID)
{
	m_dwCurrentUserID=dwUserID;
}
//��Ǯ����
bool CGameRoomEx::PayMoney(int iCount,int iPayID,long dwUserID)
{
	//��ֱ�ӿ����ݿ��Ǯ,�����ϵ�
	UserItemStruct* pUserItem;
	if(dwUserID==-1)
		pUserItem=m_GameInfo.pMeUserInfo;
	else
		pUserItem= m_UserManage.FindOnLineUser(dwUserID);
	if(pUserItem==NULL || iCount==0)
		return false;

	if(iCount > m_GameInfo.pMeUserInfo->GameUserInfo.i64Money)//��Ҳ���
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","NotEnoughCoin","��Ҳ���"));
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("GameRoom","NotEnoughCoin","��Ҳ���"));
		return true;
	}
	MSG_GR_S_Pay PayMoney;
	memset(&PayMoney,0,sizeof(PayMoney));

	PayMoney.UserID = pUserItem->GameUserInfo.dwUserID;
	PayMoney.uCount = iCount;
	PayMoney.iPayID = iPayID;

	SendData(&PayMoney,sizeof(PayMoney),MDM_GR_MONEY,ASS_GR_PAY_MONEY,0);
	return true;
}
//�������
void CGameRoomEx::OnInvitePlay()
{
	//Ч������
	if (m_bLogonFinish==false) return;
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;

	if (m_dwCurrentUserID==0) 
	{
		str = fMsg.GetKeyVal("GameRoom","InviteFailedPlayerLeave","������������Ѿ��뿪��,����ʧ��");
		m_pRightWnd->m_MessageHandle.InsertSystemMessage(str.GetBuffer());
		return;
	}

	//�������
	UserInfoStruct * pMeUserInfo=&m_GameInfo.pMeUserInfo->GameUserInfo;
	UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);

	if(m_dwCurrentUserID<0)
	{//������ң�Ҫ����Լ��ǲ��ǿ���������ˣ������������ʱ��Ҫ��ʾ 20081212
		if(pMeUserInfo->bDeskNO==255 || (pMeUserInfo->bUserState!=USER_SITTING && pMeUserInfo->bUserState!=USER_ARGEE))
		{
			str = fMsg.GetKeyVal("GameRoom","InviteFailedNotWaiting","�����Ǵ��ڵȴ���Ϸ״̬,��������������Ҽ���");
			m_pRightWnd->m_MessageHandle.InsertSystemMessage(str.GetBuffer());
		}
		else
		{
			///<��ʾ��ʾ
			str = fMsg.GetKeyVal("GameRoom","InviteConfirm","���Ѿ��������롣");
			m_pRightWnd->m_MessageHandle.InsertSystemMessage(str.GetBuffer());
		}
		return;
	}
	//�ж��û�
	if (pUserItem==NULL) 
	{
		str = fMsg.GetKeyVal("GameRoom","InviteFailedPlayerLeave","������������Ѿ��뿪��,����ʧ��");
		m_pRightWnd->m_MessageHandle.InsertSystemMessage(str.GetBuffer());
		return;
	}

	//�������� 
	if ((pMeUserInfo->bDeskNO!=255)&&(pUserItem->GameUserInfo.bDeskNO!=pMeUserInfo->bDeskNO)&&
		(pMeUserInfo->bUserState<US_PLAY_GAME)&&(pUserItem->GameUserInfo.bUserState<US_PLAY_GAME))
	{
		//��������
		MSG_GR_SR_InviteUser InviteUser;
		InviteUser.dwUserID=pMeUserInfo->dwUserID;
		InviteUser.dwTargetID=pUserItem->GameUserInfo.dwUserID;
		SendData(&InviteUser,sizeof(InviteUser),MDM_GR_ROOM,ASS_GR_INVITEUSER,pMeUserInfo->bDeskNO);
		///<��ʾ��ʾ
		str = fMsg.GetKeyVal("GameRoom","InviteConfirm","���Ѿ��������롣");
		m_pRightWnd->m_MessageHandle.InsertSystemMessage(str.GetBuffer());
		return;
	}

	//��ʾ��ʾ
	str = fMsg.GetKeyVal("GameRoom","InviteFailedNotWaiting","�����Ǵ��ڵȴ���Ϸ״̬,��������������Ҽ���");
	m_pRightWnd->m_MessageHandle.InsertSystemMessage(str.GetBuffer());
	return;
}

//����Ϊ�������
void CGameRoomEx::OnSetTalkObject()
{
	if (m_dwCurrentUserID!=0L) 
	{
		UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
		if (pUserItem==NULL) return;

		//��������
		m_dwTalkTarget=m_dwCurrentUserID;
		m_strTalkName=pUserItem->GameUserInfo.nickName;

		//�����Ƿ����
		BYTE bNullStation=255;
		for (BYTE i=0;i<sizeof(m_RecentlyUser)/sizeof(m_RecentlyUser[0]);i++)
		{
			if (m_RecentlyUser[i].bAccess==true)
			{
				if (m_RecentlyUser[i].dwUserID==m_dwCurrentUserID) return;
			}
			else bNullStation=(bNullStation==255)?i:bNullStation;
		}

		//����λ��
		if (bNullStation==255)
		{
			bNullStation=sizeof(m_RecentlyUser)/sizeof(m_RecentlyUser[0])-1;
			MoveMemory(&m_RecentlyUser[0],&m_RecentlyUser[1],sizeof(m_RecentlyUser)/(sizeof(m_RecentlyUser)/sizeof(m_RecentlyUser[0])-1));
		}

		//������������б�
		m_RecentlyUser[bNullStation].bAccess=true;
		m_RecentlyUser[bNullStation].dwUserID=m_dwTalkTarget;
		lstrcpy(m_RecentlyUser[bNullStation].szUserName,pUserItem->GameUserInfo.nickName);
	}
	m_pInputMessage->SetFocus();

	return;
}

//ȡ���������
void CGameRoomEx::OnCancelTalkObject()
{
	m_dwTalkTarget=0L;
	m_strTalkName.Empty();
	m_pInputMessage->SetFocus();
	return;
}

///�ж��Ƿ����Ŷӻ�����
///@return true:���Ŷӻ����䣻false�������Ŷӻ�����
bool CGameRoomEx::IsQueueGameRoom()
{
	return(((m_GameInfo.dwRoomRule & GRR_QUEUE_GAME) > 0) ? true : false);
}

//���ҿ�����
void CGameRoomEx::OnFindNullDesk()
{
	//Ч��״̬
    if (m_bLogonFinish==false) return;
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;

	if (CanCloseNow()==false)
	{
		str = fMsg.GetKeyVal("GameRoom","PlayingNoChangeSeat","������Ϸ��,���ܻ�λ��!");
		
		//AFCMessageBox(str,m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,str);
		return;
	}
	
	//�ж��Ƿ�����λ��
	if ((m_GameInfo.dwRoomRule&GRR_LIMIT_DESK)!=0L)
	{
		if ((m_bMatchDeskIndex!=255)&&(m_bMatchDeskStation!=255))
		{
			if ((m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=m_bMatchDeskIndex)&&
				(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation!=m_bMatchDeskStation))
			{
				if (m_bAutoSit==false)
				{
					CString strMessage;
					str = fMsg.GetKeyVal("GameRoom","WhetherPlayerSitDown",
						"���ǲ���ѡ��,���Ĳ���λ���� %d ����Ϸ��,�Ƿ����ھ���?");
					strMessage.Format(str,m_bMatchDeskIndex+1);
					//if (AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_YESNO)!=IDOK) return;//,MB_ICONQUESTION|MB_YESNO,this)!=IDYES) return;
					if (DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strMessage)!=IDYES) return;
					if ((m_bMatchDeskIndex==255)||(m_bMatchDeskStation==255)) return;
				}
				PostMessage(WM_LEFT_MOUSE_HIT_DESK,m_bMatchDeskIndex,m_bMatchDeskStation);
			}
			else 
			{
				str = fMsg.GetKeyVal("GameRoom","U_R_PlayerAndSit","���ǲ���ѡ��,���Ѿ�������!");
				
				//AFCMessageBox(str,m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,str);
			}
		}
		else 
		{
			str = fMsg.GetKeyVal("GameRoom","NoGameQualification","��Ǹ,������Ϸ��������,��û�в���Ȩ��!");
			
			//AFCMessageBox(str,m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,str);
		}
		return;
	}


	bool isnull[4]={false,false,false,false};
	int lastfind=-1;
	for(int i=0;i<4;i++)
	{
		if(i==0)
			isnull[i]=true;
		else
		{
			isnull[i-1]=false;
			isnull[i]=true;
		}

		//����λ��
		m_bLastFindDesk=255;
		int _tindex;
		m_bLastFindDesk=m_DeskFrame.FindGameDesk(m_bLastFindDesk,isnull[0],isnull[1],isnull[2],isnull[3]);
		_tindex = m_bLastFindDesk;
		while(m_bLastFindDesk < 255 && !IsCanSit(m_GameInfo.pMeUserInfo, m_bLastFindDesk))
		{
			m_bLastFindDesk=m_DeskFrame.FindGameDesk(m_bLastFindDesk,isnull[0],isnull[1],isnull[2],isnull[3]);
			if (m_bLastFindDesk == _tindex)
			{
				m_bLastFindDesk = 255;
				break;
			}
		}
		
		if (m_bLastFindDesk!=255)
		{

			// add xqm 2010-11-25 ��֪��Ϊ���Ѿ��ҵ�λ������, ȴ��Ҫ���ϴ�����λ��, ��ע�ʹ˶δ���
			/*if(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO == m_bLastFindDesk)
			{
				if(lastfind != m_bLastFindDesk)
				{
					lastfind = m_bLastFindDesk;
					CString str;
					str.Format("�ҵ�һ����������λ��%d, �����ǰ����λ��%d", m_bLastFindDesk, m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO);
					AfxMessageBox(str);
					goto find;
				}
				else
				{
					return ;
				}
			}*/
			//�ҵ�λ��
			m_dwCurrentUserID=0L;
			if(m_pRoomInfo->pComRoomInfo.uComType!=TY_MATCH_GAME)
			{
				m_DeskFrame.SetFindFrame(m_bLastFindDesk);
				m_DeskFrame.SetViewDesk(m_bLastFindDesk);
			}
			//if (m_bAutoSit) 
			{
				BYTE bNullStation=m_DeskFrame.GetNullStation(m_bLastFindDesk);
				PostMessage(WM_LEFT_MOUSE_HIT_DESK,m_bLastFindDesk,bNullStation);
			}
			return ;
		}
	}

	return;
}


bool CGameRoomEx::IsCanSit(UserItemStruct* pUserInfo, int index)
{
	//for (int i = 0; i < m_UserManage.)
	if (pUserInfo == NULL) return false;

	TUserList t;
	m_UserManage.FindOnLineUser(index, t);

	if (m_GameInfo.dwRoomRule & GRR_LIMIT_SAME_IP)
	{
		for (int i = 0; i < t.size(); i++)
		{
			if (t[i]->GameUserInfo.bGameMaster == 0)
			{
				if (t[i]->GameUserInfo.dwUserIP == pUserInfo->GameUserInfo.dwUserIP)
					return false;
			}
		}
	}

	return true;
}


//��������
/************************
bsit: �Ƿ�����
bOneNULL: һ������λ
bTwoNULl: ��������λ
************************/
void CGameRoomEx::OnFindDesk(bool bsit,bool bOneNull, bool bTwoNull, bool bNoAllNull, bool bAllNull)
{
	//Ч��״̬
	if (m_bLogonFinish==false) return;

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	if (CanCloseNow()==false)
	{
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","PlayingNoChangeSeat","������Ϸ��,���ܻ�λ��!"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","PlayingNoChangeSeat","������Ϸ��,���ܻ�λ��!"));
		return;
	}

	//�ж��Ƿ�����λ��
	if ((m_GameInfo.dwRoomRule&GRR_LIMIT_DESK)!=0L)
	{
		if ((m_bMatchDeskIndex!=255)&&(m_bMatchDeskStation!=255))
		{
			if ((m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=m_bMatchDeskIndex)&&
				(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation!=m_bMatchDeskStation))
			{
				if (m_bAutoSit==false)
				{
					CString strMessage;
					CString str = fMsg.GetKeyVal("GameRoom","WhetherPlayerSitDown","���ǲ���ѡ��,���Ĳ���λ���� %d ����Ϸ��,�Ƿ����ھ���?");
					strMessage.Format(str,m_bMatchDeskIndex+1);
					//if (AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_YESNO)!=IDOK)
					if (DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strMessage)!=IDYES)
						return;//,MB_ICONQUESTION|MB_YESNO,this)!=IDYES) return;
					if ((m_bMatchDeskIndex==255)||(m_bMatchDeskStation==255)) return;
				}
				PostMessage(WM_LEFT_MOUSE_HIT_DESK,m_bMatchDeskIndex,m_bMatchDeskStation);
			}
			else
			{
				//AFCMessageBox(fMsg.GetKeyVal("GameRoom","U_R_PlayerAndSit","���ǲ���ѡ��,���Ѿ�������!"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","U_R_PlayerAndSit","���ǲ���ѡ��,���Ѿ�������!"));
			}
		}
		else
		{
			//AFCMessageBox(fMsg.GetKeyVal("GameRoom","NoGameQualification","��Ǹ,������Ϸ��������,��û�в���Ȩ��!"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","NoGameQualification","��Ǹ,������Ϸ��������,��û�в���Ȩ��!"));
		}
		return;
	}

	//����λ��
	m_bLastFindDesk=m_DeskFrame.FindGameDesk(0,bOneNull,  bTwoNull,  bNoAllNull, bAllNull);
	if (m_bLastFindDesk!=255)
	{
		if(!bsit)
			return ;
		//�ҵ�λ��
		m_dwCurrentUserID=0L;
		m_DeskFrame.SetFindFrame(m_bLastFindDesk);
		m_DeskFrame.SetViewDesk(m_bLastFindDesk);
		if (m_bAutoSit==true) 
		{
			BYTE bNullStation=m_DeskFrame.GetNullStation(m_bLastFindDesk);
			PostMessage(WM_LEFT_MOUSE_HIT_DESK,m_bLastFindDesk,bNullStation);
		}
	}
	else
	{
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","FindNoSeat","�Ҳ����������µĿ�λ��!"),m_GameInfo.szGameName);//,MB_ICONINFORMATION,this);
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","FindNoSeat","�Ҳ����������µĿ�λ��!"));
	}
	return;
}

/// ����ʵ�ֵĲ���GDI+�Ļ�ͼ������Ϊ��ԭ�к�������һ�£�����Ҳ��һ����
/// @param pDC ����DCָ��
/// @param iWidth ���ڿ��
/// @param iHeight ���ڸ߶�
void CGameRoomEx::DrawViewFramePlus(CDC * pDC, int iWidth, int iHeight)
{
	Bitmap *bmp = new Bitmap(iWidth, iHeight);
	Graphics graph(bmp);

	// ��ʼ��ͼ

	// ����д�Կ��Ĳ���ֻ��һ��
	Graphics graphThis(pDC->GetSafeHdc());
	graphThis.DrawImage(bmp, 0, 0);
	delete bmp;
	bmp = NULL;
}

/// ԭ�л���GDI��CGameImageHelper�Ļ�ͼ����
/// @param pDC ����DCָ��
/// @param iWidth ���ڿ��
/// @param iHeight ���ڸ߶�
void CGameRoomEx::DrawViewFrame(CDC * pDC, int iWidth, int iHeight)
{
	////////////////////////////
	//////Kylin 20090209 �����Ϸ���乤��������
	//h
	CGameImageHelper helptc(&m_roomtc);	///< �����м�
	CGameImageHelper helpbc(&m_roombc);	///< �׿��м�
	if (helptc.GetWidth()>0)
	{
		helptc.StretchBlt(pDC->GetSafeHdc(), m_toolBarX, 0, iWidth+m_toolBarW, helptc.GetHeight(), 0, 0, helptc.GetWidth(), helptc.GetHeight());
	}
	if(helpbc.GetWidth()>0)
	{
		helpbc.StretchBlt(pDC->GetSafeHdc(), m_toolBarX,
			iHeight-helpbc.GetHeight(), iWidth+m_toolBarW, helpbc.GetHeight(),
			0, 0, helpbc.GetWidth(), helpbc.GetHeight());
	}
	

	//v
	CGameImageHelper helplc(&m_roomlc);	///< ����
	CGameImageHelper helprc(&m_roomrc); ///< ����
	if(helplc.GetWidth()>0)
	{
		helplc.StretchBlt(pDC->GetSafeHdc(), 0, 0, helplc.GetWidth(), iHeight, 0, 0, helplc.GetWidth(), helplc.GetHeight());
	}
	if(helprc.GetWidth()>0)
	{
		helprc.StretchBlt(pDC->GetSafeHdc(), iWidth-helprc.GetWidth(), 0, helplc.GetWidth(), iHeight, 0, 0, helprc.GetWidth(), helprc.GetHeight());
	}
	//helptc.BitBlt(pDC->GetSafeHdc(),m_toolBarX,0);
	//helpbc.BitBlt(pDC->GetSafeHdc(),m_toolBarX,iHeight-helpbc.GetHeight());

	//helptc.BitBlt(pDC->GetSafeHdc(),iWidth+m_toolBarW,0);
	//helpbc.BitBlt(pDC->GetSafeHdc(),iWidth+m_toolBarW,iHeight-helpbc.GetHeight());

	//helptc.BitBlt(pDC->GetSafeHdc(),1,0);
	//helptc.BitBlt(pDC->GetSafeHdc(),iWidth-helptc.GetWidth()-1,0);
	////////////////////////////
	//4
	CGameImageHelper helplt(&m_roomlt);	///< ����
	m_roomlt.BitBlt(pDC->GetSafeHdc(),0,0);
	CGameImageHelper helprt(&m_roomrt); ///< ����
	helprt.BitBlt(pDC->GetSafeHdc(),iWidth-helprt.GetWidth(),0);
	CGameImageHelper helplb(&m_roomlb); ///< ����
	helplb.BitBlt(pDC->GetSafeHdc(),0,iHeight-helplb.GetHeight());
	CGameImageHelper helprb(&m_roomrb); ///< ����
	helprb.BitBlt(pDC->GetSafeHdc(),iWidth-helprb.GetWidth(),iHeight-helprb.GetHeight());

	CGameImageHelper help_main_lt(&m_mainlt);
	m_mainlt.BitBlt(pDC->GetSafeHdc(), m_toolBarX - help_main_lt.GetWidth(), 0);
	CGameImageHelper help_main_rt(&m_mainrt);
	m_mainrt.BitBlt(pDC->GetSafeHdc(), iWidth+m_toolBarW-m_toolBarX-help_main_rt.GetWidth(),0);

//	drawSignal(pDC);

	//wushuqun 2009.7.1
	//���Ϸ�������
	CRect rect;
	rect.SetRect(m_roomNameX,m_roomNameY,400,helptc.GetHeight());
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_roomNameClr);
	CFont * oldFont = pDC->GetCurrentFont();
	pDC->SelectObject(m_roomNameFont);
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	pDC->DrawText(fMsg.GetKeyVal("GameRoom","CurrentRoom","��ǰ����:")+m_GameInfo.szGameRoomName,& rect,DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE);
	pDC->SelectObject(oldFont);

	CGameListWnd* pGameListWnd = GetMainRoom()->m_pGameListWnd;
	if (pGameListWnd->m_UserTabDuWnd.GetHWND())
	{
		pGameListWnd->m_UserTabDuWnd.ShowWindow(SW_HIDE);
	}


	return;
}

//������Ϣ����
CShortMessage * CGameRoomEx::ActiveShortWnd(long int dwUserID, UserItemStruct * pUserItem, bool bCreate)
{
	//��������
	INT_PTR iCloseIndex=-1,iHideIndex=-1;

	//Ѱ�����ϴ���
	for (INT_PTR i=0;i<m_ShortMessage.GetCount();i++)
	{
		CShortMessage * pShortMessageWnd=(CShortMessage *)m_ShortMessage.GetAt(i);
		ASSERT(pShortMessageWnd!=NULL);
		if (pShortMessageWnd!= NULL)
		{
			if(dwUserID==pShortMessageWnd->m_dwTargetUserID)
			{
				if (pShortMessageWnd->GetSafeHwnd()==NULL)
				{
					AfxSetResourceHandle(GetModuleHandle(NULL));
					pShortMessageWnd->Create(IDD_SHORT_MESSAGE,GetDesktopWindow());
					pShortMessageWnd->CenterWindow();
				}
				pShortMessageWnd->ShowWindow(SW_SHOWNOACTIVATE);
				return pShortMessageWnd;
			}
		}
		else
		{
			m_ShortMessage.RemoveAt(i);
		}
		if (bCreate==true)
		{
			if ((iCloseIndex==-1)&&(pShortMessageWnd->GetSafeHwnd()==NULL))
				iCloseIndex=i;
			if ((iHideIndex==-1)&&(pShortMessageWnd->GetSafeHwnd()!=NULL)&&(pShortMessageWnd->IsWindowVisible()==FALSE)) iHideIndex=i;
		}
	}

	//ʹ�ùرյĴ���
	if ((bCreate==true)&&(iCloseIndex>=0))
	{
		CShortMessage * pShortMessageWnd=(CShortMessage *)m_ShortMessage.GetAt(iCloseIndex);
		ASSERT(pShortMessageWnd!=NULL);
		if (pShortMessageWnd->GetSafeHwnd()==NULL)
		{
			AfxSetResourceHandle(GetModuleHandle(NULL));
			pShortMessageWnd->Create(IDD_SHORT_MESSAGE,GetDesktopWindow());
			pShortMessageWnd->CenterWindow();
		}
		SendMsgUserStruct user;
		pShortMessageWnd->TransUserInfo(pUserItem, &user);
		pShortMessageWnd->SetTalkInfo(&user,m_GameInfo.dwGamePower,m_pRoomInfo->pComRoomInfo.szGameRoomName);
		pShortMessageWnd->ShowWindow(SW_SHOWNOACTIVATE);
		return pShortMessageWnd;
	}

	//ʹ�����ش���
	if ((bCreate==true)&&(iHideIndex>=0))
	{
		CShortMessage * pShortMessageWnd=(CShortMessage *)m_ShortMessage.GetAt(iHideIndex);
		ASSERT(pShortMessageWnd!=NULL);
		SendMsgUserStruct user;
		pShortMessageWnd->TransUserInfo(pUserItem, &user);
		pShortMessageWnd->SetTalkInfo(&user,m_GameInfo.dwGamePower,m_pRoomInfo->pComRoomInfo.szGameRoomName);
		pShortMessageWnd->ShowWindow(SW_SHOWNOACTIVATE);
		return pShortMessageWnd;
	}

	//�����´���
	if ((bCreate==true)&&(m_ShortMessage.GetCount()<m_iMaxShortWnd))
	{
		try
		{
			CShortMessage * pShortMessageWnd=new CShortMessage(this);
			AfxSetResourceHandle(GetModuleHandle(NULL));
			SendMsgUserStruct user;
			pShortMessageWnd->TransUserInfo(pUserItem, &user);
			pShortMessageWnd->SetTalkInfo(&user,m_GameInfo.dwGamePower,m_pRoomInfo->pComRoomInfo.szGameRoomName);
			pShortMessageWnd->Create(IDD_SHORT_MESSAGE,GetDesktopWindow());
			pShortMessageWnd->CenterWindow();
			pShortMessageWnd->ShowWindow(SW_SHOWNOACTIVATE);
			m_ShortMessage.Add(pShortMessageWnd);
			return pShortMessageWnd;
		}
		catch (...) { return NULL; }
	}

	return NULL;
}

//�˵�����
BOOL CGameRoomEx::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	switch (LOWORD(wParam))
	{
	case IDM_SAVE_MESSAGE:				//����������Ϣ
		{
			m_pRightWnd->m_TalkMessage.OnSave();
			return TRUE;
		}
	case IDM_CLEAR_SCREEN:				//���������Ļ
		{
			m_pRightWnd->m_TalkMessage.OnClearAll();
			return TRUE;
		}
	case IDM_AUTO_SCROLL:				//�Զ�������Ļ
		{
			m_pRightWnd->m_TalkMessage.OnAutoScroll();
			return TRUE;
		}
	case IDM_USER_INFO:                 //�鿴�û�����
		{
			OnUserDetails(false);
			return TRUE;
		}
	case IDM_USER_INFO_INDESK:                 //�鿴�û�����
		{
			OnUserDetails(true);
			return TRUE;
		}
	case IDM_COPY_USER_NAME:			//�����û�����
		{
			OnCopyUserName();
			return TRUE;
		}
	case IDM_FIND_ONLINE_USER:			//�����û�
		{
			ShowFindUserWnd();
			return TRUE;
		}
// 	case IDM_GIVE_PROP:
// 		{
// 			OnGiveProp(m_dwCurrentUserID,1);
// 			return TRUE;
// 		}
	case IDM_FINDED_SIT:				//�Զ�����
		{
			m_bAutoSit=!m_bAutoSit;
			AfxGetApp()->WriteProfileInt(TEXT("RoomInfo"),TEXT("AutoSitDesk"),m_bAutoSit);
			return TRUE;
		}
	case IDM_FIND_DESK:					//����λ��
		{
			OnFindNullDesk();
			return TRUE;
		}
	case IDM_SEND_MESSAGE:				//���Ͷ���Ϣ
		{
			if (m_bLogonFinish==false) return TRUE;
			if (GetMainRoom()->m_pGameListWnd->m_IMList.GetUserGroup(m_dwCurrentUserID) == 3)
			{
				CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
				
				//AFCMessageBox(fMsg.GetKeyVal("IMDlg","NoChatWithBadGuyList","�Բ�����������������е��û�����"),fMsg.GetKeyVal("IMDlg","Tip","��ʾ"));
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,fMsg.GetKeyVal("IMDlg","Tip","��ʾ"),false,fMsg.GetKeyVal("IMDlg","NoChatWithBadGuyList","�Բ�����������������е��û�����"));
			}
			else
			{
				OnSendShortMessage();
			}
			return TRUE;
		}
	case IDM_ALONE_DESK://20081212 Fred Huang
		{
			if(!bExtendRobot)
				return TRUE;

			if ((m_deskIndex==255)||(m_deskStation==255)) return TRUE;
			MSG_GR_SR_MatchDelete aloneDesk;
			aloneDesk.dwUserID=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID;
			aloneDesk.bDeskIndex=m_deskIndex;
			aloneDesk.bDeskStation=1;
			SendData(&aloneDesk,sizeof(aloneDesk),MDM_GR_MANAGE,ASS_GR_ALONE_DESK,0);
			return TRUE;
		}
	case IDM_ALONE_DESK_ALL:
		{
			if(!bExtendRobot)
				return TRUE;
			CExtRobot er;
			er.m_extRobot=0;
			if(er.DoModal()!=IDOK)
				return TRUE;



			int deskCount=m_pRoomInfo->pComRoomInfo.uDeskCount;
			int uDeskCount = 0;

			
			for(int i=0;i<deskCount;i++)
			{
				bool found=false;
				CBaseDeskView * pDesk=m_DeskFrame.GetDeskArrayPtr(i);
				for(int j=0;j<m_pRoomInfo->pComRoomInfo.uDeskPeople;j++)
				{
					if(pDesk->GetUserItem(j))
					{
						found=true;
						break;
					}
				}
				if(!found)
					uDeskCount++;
			}
			if(uDeskCount>er.m_extRobot)
				uDeskCount=er.m_extRobot;

			int iToalRoomCount = deskCount;

			deskCount=0;

			CByteArray aryDesk;
		

			while(deskCount<uDeskCount)
			{
				m_pRoomInfo;
				m_GameInfo;
				int uDeskIndex=rand() % iToalRoomCount;

				bool bHaveSetIndx = false;
				for (int i=0; i<aryDesk.GetCount(); i++)
				{
					if (aryDesk[i] == uDeskIndex)
					{
						bHaveSetIndx = true;
						break;
					}
				}
				if (!bHaveSetIndx)
				{
					aryDesk.Add(uDeskIndex);
				}
				else
				{
					continue;
				}
				CBaseDeskView * pDesk=m_DeskFrame.GetDeskArrayPtr(uDeskIndex);
				bool found=false;
				for(int j=0;j<m_pRoomInfo->pComRoomInfo.uDeskPeople;j++)
				{
					if(pDesk->GetUserItem(j))
					{
						found = true;
						break;
					}
				}
				if(!found)
				{
					deskCount++;
					MSG_GR_SR_MatchDelete aloneDesk;
					aloneDesk.dwUserID=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID;
					aloneDesk.bDeskIndex=uDeskIndex;
					aloneDesk.bDeskStation=1;
					SendData(&aloneDesk,sizeof(aloneDesk),MDM_GR_MANAGE,ASS_GR_ALONE_DESK,0);
				}
			}
		return TRUE;
		}
	case IDM_DEALONE_DESK://20081212 Fred Huang
		{
			if(!bExtendRobot)
				return TRUE;
			m_bDeAloneAll = false;  ///< �������������
			if ((m_deskIndex==255)||(m_deskStation==255)) return TRUE;
			MSG_GR_SR_MatchDelete aloneDesk;
			aloneDesk.dwUserID=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID;
			aloneDesk.bDeskIndex=m_deskIndex;
			aloneDesk.bDeskStation=2;
			SendData(&aloneDesk,sizeof(aloneDesk),MDM_GR_MANAGE,ASS_GR_DEALONE_DESK,0);
			return TRUE;
		}
	case IDM_DEALONE_DESK_ALL://20081212 Fred Huang
		{
			if(!bExtendRobot)
				return TRUE;
			m_bDeAloneAll = true;  ///< �����������
			int deskCount=m_pRoomInfo->pComRoomInfo.uDeskCount;
			int uDeskCount=0;
			for(int i=0;i<deskCount;i++)
			{
				bool found=false;
				CBaseDeskView * pDesk=m_DeskFrame.GetDeskArrayPtr(i);
				for(int j=0;j<m_pRoomInfo->pComRoomInfo.uDeskPeople;j++)
				{
					UserItemStruct *uItem=pDesk->GetUserItem(j);
					if(!uItem)
					{
						found=true;
						break;
					}
					else if(uItem->GameUserInfo.dwUserID>=0)
					{
						found=true;
						break;
					}
				}
				if(!found)
				{
					MSG_GR_SR_MatchDelete aloneDesk;
					aloneDesk.dwUserID=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID;
					aloneDesk.bDeskIndex=i;
					aloneDesk.bDeskStation=2;
					SendData(&aloneDesk,sizeof(aloneDesk),MDM_GR_MANAGE,ASS_GR_DEALONE_DESK,0);
				}
			}
			return TRUE;
		}
	case IDM_SHIELD_USER:				//�����û�
		{
			OnShieldUser();	
			return TRUE;
		}
	case IDM_UNSHIELD_USER:				//��������û�
		{
			OnUnShieldUser();
			return TRUE;
		}
	case IDM_INVITE_PLAY:				//�����û�
		{
			OnInvitePlay();
			return TRUE;
		}
	case IDM_SET_FRIEND:				//����Ϊ����
		{
			long dwUserID=lParam;
			if(dwUserID==0)
				dwUserID=m_dwCurrentUserID;
			//Ԥ����im����
			if ((dwUserID==0L)||(dwUserID==m_pLogonInfo->dwUserID)) 
				return TRUE;
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(dwUserID);
			if (pUserItem==NULL) return TRUE;
			if (pUserItem->uConnection==CM_FRIEND) 
				pUserItem->uConnection=CM_NORMAL;
			else 
				pUserItem->uConnection=CM_FRIEND;
			m_pUserList->UpdateGameUser(pUserItem);
			if (m_pFindUser!=NULL) 
				m_pFindUser->UpdateUserItem(pUserItem);
			GetMainRoom()->m_pGameListWnd->m_IMList.SetUser(dwUserID,pUserItem->GameUserInfo.nickName);
			return TRUE;
		}
	case IDM_SET_EMENY:					//����Ϊ����
		{
			//Ԥ����im����
			if ((m_dwCurrentUserID==0L)||(m_dwCurrentUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)) return TRUE;
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
			if (pUserItem==NULL) return TRUE;
			if (pUserItem->uConnection==CM_ENEMY) 
				pUserItem->uConnection=CM_NORMAL;
			else 
				pUserItem->uConnection=CM_ENEMY;
			m_pUserList->UpdateGameUser(pUserItem);
			if (m_pFindUser!=NULL) 
				m_pFindUser->UpdateUserItem(pUserItem);
			GetMainRoom()->m_pGameListWnd->m_IMList.SetUserGroup(m_dwCurrentUserID, NULL, 3);

			CString szMsg;
			szMsg.Format(fMsg.GetKeyVal("IMDlg","MoveOneToBadList","���[%s]�Ѿ����뵽��ĺ�������"), pUserItem->GameUserInfo.nickName);
			AFCTimerMsgBox(5, szMsg);

			return TRUE;
		}
	case IDM_SET_TALK_TARGET:			//����̸������
		{
			OnSetTalkObject();
			return TRUE;
		}
	case IDM_CANCEL_TALK:				//ȡ��̸������
		{
			OnCancelTalkObject();
			return TRUE;
		}
	case IDM_TALK_COLOR:				//����������ɫ
		{
			CColorDialog Color(CGameImageLink::m_ColorResource.m_crTalkTxColor,CC_FULLOPEN);
			if (Color.DoModal()==IDOK)
				CGameImageLink::m_ColorResource.m_crTalkTxColor=Color.GetColor();
			m_pInputMessage->SetFocus();
			return TRUE;
		}
	case IDM_WATHC_GAME:				//�Թ���Ϸ
		{
			if ((m_dwCurrentUserID==0L)||(m_bLogonFinish==false))
				return TRUE;
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
			if ((pUserItem==NULL)||(pUserItem->GameUserInfo.bDeskNO==255)) 
				return TRUE;
			OnLeftHitDesk(pUserItem->GameUserInfo.bDeskNO,pUserItem->GameUserInfo.bDeskStation);
			return TRUE;
		}
	case IDM_LEFT_USER:					//������뿪
		{
			//Ч��״̬
			if ((m_dwCurrentUserID==0L)||(m_bLogonFinish==false)) 
				return TRUE;
			if ((m_GameInfo.bDeskOnwer==false)||(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO==255))
				return TRUE;
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
			if (pUserItem==NULL) 
				return TRUE;
			if (pUserItem->GameUserInfo.bDeskNO!=m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)
				return TRUE;
			//������Ϣ
			return TRUE;
		}
	case IDM_MANAGER_USER:				//�������Ȩ��
		{
			//Ч��״̬
			if ((m_dwCurrentUserID==0L)||(m_bLogonFinish==false)) return TRUE;
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
			if (pUserItem==NULL) return TRUE;

			//Ч������
			ASSERT(m_GameInfo.pMeUserInfo->GameUserInfo.bGameMaster>0);

			//��������
			if (m_pManageDlg==NULL)
				m_pManageDlg=new CManageDlg(this,&m_GameInfo);
			if (m_pManageDlg->GetSafeHwnd()==NULL)
			{
				AfxSetResourceHandle(GetModuleHandle(NULL));
				m_pManageDlg->Create(IDD_MANAGE_DLG,this);
			}
			m_pManageDlg->SetManageUser(pUserItem);
			m_pManageDlg->ShowWindow(SW_SHOW);

			return TRUE;
		}
	case IDM_CUT_USER:					//���������
		{
			//Ч��״̬
			if ((m_dwCurrentUserID==0L)||(m_bLogonFinish==false)) return TRUE;
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
			if (pUserItem==NULL) return TRUE;

			//Ч������
			ASSERT(CUserPower::CanCutUser(m_GameInfo.dwMasterPower)==true);

			CString strMessage;
			CString str;

			if (USER_PLAY_GAME == pUserItem->GameUserInfo.bUserState || USER_CUT_GAME == pUserItem->GameUserInfo.bUserState)
			{
				str = fMsg.GetKeyVal("GameRoom","CanNotKickPlayer","%s������Ϸ���������߸�������ߡ�");
				strMessage.Format(str,pUserItem->GameUserInfo.nickName);
				
				//AFCMessageBox(strMessage);
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,strMessage);
				return TRUE;
			}

			//��ʾ��Ϣ
			str = fMsg.GetKeyVal("GameRoom","WhetherKickPlayer","ȷʵҪ����ң�[ %s ] �뿪��Ϸ������?");
			strMessage.Format(str,pUserItem->GameUserInfo.nickName);
			//if (AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_YESNO)==IDOK)//,MB_ICONQUESTION|MB_YESNO,this)==IDYES)
			if (DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strMessage)==IDYES)//,MB_ICONQUESTION|MB_YESNO,this)==IDYES)
			{
				MSG_GR_S_CutUser CutUser;
				CutUser.dwTargetID=m_dwCurrentUserID;
				CutUser.dwUserID=m_pLogonInfo->dwUserID;
				SendData(&CutUser,sizeof(CutUser),MDM_GR_MANAGE,ASS_GR_CUT_USER,0);
			}
			return TRUE;
		}
    case IDM_ONE_LEFT_ROOM:         // PengJiLin, 2010-8-26, ����������뿪����
        {
            //Ч��״̬
            if ((m_dwCurrentUserID==0L)||(m_bLogonFinish==false)) return TRUE;
            UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
            if (pUserItem==NULL) return TRUE;

            // ��ʾ��Ϣ
            CString strMessage;
            CString str;
            str = fMsg.GetKeyVal("GameRoom","OwnerTOneLeftRoomMsg","ȷʵҪ����ң�[ %s ] �뿪������?");
            strMessage.Format(str,pUserItem->GameUserInfo.nickName);
            //if(AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_YESNO)==IDOK)
            if(DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strMessage)==IDYES)
            { 
                MSG_GR_S_CutUser CutUser;
                CutUser.dwTargetID=m_dwCurrentUserID;
                CutUser.dwUserID=m_pLogonInfo->dwUserID;
                SendData(&CutUser,sizeof(CutUser),MDM_GR_ROOM,ASS_GR_OWNER_T_ONE_LEFT_ROOM,0);
            }
            return TRUE;
        }
	case IDM_SAFE_FINISH://��ȫ�����Ϸ
		{
			//Ч��״̬
			if ((m_dwCurrentUserID==0L)||(m_bLogonFinish==false)) return TRUE;
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
			if (pUserItem==NULL) return TRUE;

			//Ч������
			ASSERT(CUserPower::CanCutUser(m_GameInfo.dwMasterPower)==true);

			//��ʾ��Ϣ
			CString strMessage;
			CString str;
			str = fMsg.GetKeyVal("GameRoom","WhetherLetPlayerOut","ȷʵҪ��ң�[ %s ] ��ȫ�뿪��Ϸ������?");
			strMessage.Format(str,pUserItem->GameUserInfo.nickName);
			//if (AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_YESNO)==IDOK)//MB_ICONQUESTION|MB_YESNO,this)==IDYES)
			if (DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strMessage)==IDYES) 
			{
				MSG_GR_S_CutUser CutUser;
				CutUser.dwTargetID=m_dwCurrentUserID;
				CutUser.dwUserID=m_pLogonInfo->dwUserID;
				SendData(&CutUser,sizeof(CutUser),MDM_GR_MANAGE,ASS_GR_CUT_USER,1);
			}
			return TRUE;
		}
	case IDM_SEND_WARNNING://���;�����Ϣ
		{
			CSendWarnDlg SendWarnDlg;
			if ((m_dwCurrentUserID==0L)||(m_bLogonFinish==false)) return TRUE;
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
			if (pUserItem==NULL) return TRUE;
			SendWarnDlg.m_targetid=m_dwCurrentUserID;
			SendWarnDlg.m_name=pUserItem->GameUserInfo.nickName;
			if(SendWarnDlg.DoModal()==IDOK)
			{
				MSG_GR_SR_Warning warnmsg;
				warnmsg.dwTargetID=SendWarnDlg.m_targetid;
				lstrcpy(warnmsg.szWarnMsg,SendWarnDlg.m_warnmsg);
				warnmsg.uLength=SendWarnDlg.m_warnmsg.GetLength();
				SendGameData(&warnmsg,sizeof(MSG_GR_SR_Warning),MDM_GR_MANAGE,ASS_GR_WARN_MSG,0);
			}
			return true;
		}
		//case IDM_ADD_PIONT:
		//	{
		//		return true;
		//	}
	case IDM_LIMIT_IP://��ֹ��IP
		{
			//Ч��״̬
			if ((m_dwCurrentUserID==0L)||(m_bLogonFinish==false)) return TRUE;
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
			if (pUserItem==NULL) return TRUE;

			//Ч������
			ASSERT(CUserPower::CanCutUser(m_GameInfo.dwMasterPower)==true);

			//��ʾ��Ϣ
			CString strMessage;
			CString str;
			str = fMsg.GetKeyVal("GameRoom","ForbidPlayerIP","ȷʵҪ��ֹ��ң�[ %s ] ��IP��?");
			strMessage.Format(str,pUserItem->GameUserInfo.nickName);
			//if (AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_YESNO)==IDOK)//MB_ICONQUESTION|MB_YESNO,this)==IDYES)
			if (DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strMessage)==IDYES) 
			{
				MSG_GR_S_CutUser CutUser;
				CutUser.dwTargetID=m_dwCurrentUserID;
				CutUser.dwUserID=m_pLogonInfo->dwUserID;
				SendData(&CutUser,sizeof(CutUser),MDM_GR_MANAGE,ASS_GR_FORBID_USER_IP,0);
			}
			return TRUE;
		}
	case IDM_LIMIT_ACCOUN://��ֹ�ʻ�
		{
			//Ч��״̬
			if ((m_dwCurrentUserID==0L)||(m_bLogonFinish==false)) return TRUE;
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
			if (pUserItem==NULL) return TRUE;

			//Ч������
			ASSERT(CUserPower::CanCutUser(m_GameInfo.dwMasterPower)==true);

			//��ʾ��Ϣ
			CString strMessage;
			CString str;
			str = fMsg.GetKeyVal("GameRoom","ForbidPlayerAccount","ȷʵҪ��ֹ��ң�[ %s ] ���ʺ���?");
			strMessage.Format(str,pUserItem->GameUserInfo.nickName);
			//if (AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_YESNO)==IDOK)//,MB_ICONQUESTION|MB_YESNO,this)==IDYES)
			if (DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strMessage)==IDYES)
			{
				MSG_GR_S_CutUser CutUser;
				CutUser.dwTargetID=m_dwCurrentUserID;
				CutUser.dwUserID=m_pLogonInfo->dwUserID;
				SendData(&CutUser,sizeof(CutUser),MDM_GR_MANAGE,ASS_GR_FORBID_USER,0);
			}
			return true;
		}
	case IDM_RED:						
	case IDM_BULE:
	case IDM_GRAY:
	case IDM_GREEN:
	case IDM_WHITE:
	case IDM_YELLOW:
	case IDM_POWER_RED:					//����������ɫ
		{
			UINT uColorMenuID[7]={IDM_RED,IDM_YELLOW,IDM_BULE,IDM_GREEN,IDM_WHITE,IDM_POWER_RED,IDM_GRAY};
			COLORREF rcColor[8]={RGB(255,0,0),RGB(255,255,0),RGB(0,255,255),RGB(0,255,0),RGB(255,255,255),RGB(255,0,255),RGB(192,192,192),RGB(255,255,255)};
			UINT i;
			for (i=0;i<sizeof(uColorMenuID)/sizeof(uColorMenuID[0]);i++)
			{
				if (LOWORD(wParam)==uColorMenuID[i]) break;
			}
			CGameImageLink::m_ColorResource.m_crTalkTxColor=rcColor[i];
			m_pInputMessage->SetFocus();
			return TRUE;
		}
	case IDM_TALK_OBJECT:
	case IDM_TALK_OBJECT+1:
	case IDM_TALK_OBJECT+2:
	case IDM_TALK_OBJECT+3:
	case IDM_TALK_OBJECT+4:				//�����������
		{
			BYTE bObjectID=LOWORD(wParam)-IDM_TALK_OBJECT;
			if (m_RecentlyUser[bObjectID].bAccess==true)
			{
				UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_RecentlyUser[bObjectID].dwUserID);
				if (pUserItem!=NULL)
				{
					m_dwTalkTarget=m_RecentlyUser[bObjectID].dwUserID;
					m_strTalkName=m_RecentlyUser[bObjectID].szUserName;
					m_pInputMessage->SetFocus();
				}
				else
				{
					m_RecentlyUser[bObjectID].bAccess=false;
					m_RecentlyUser[bObjectID].dwUserID=0;
					m_RecentlyUser[bObjectID].szUserName[0]=0;
					CString str;
					str = fMsg.GetKeyVal("GameRoom","PlayerLeaveNotConfig","���û��Ѿ��뿪,��������Ϊ�������");
					m_pRightWnd->m_MessageHandle.InsertSystemMessage(str.GetBuffer());
					m_pInputMessage->SetFocus();
				}
			}
			return TRUE;
		}
	case IDM_ROOM_SYSTEM://������Ϣ
		{
			m_bRoomSystem=!m_bRoomSystem;
			return true;
		}
	case IDM_GAME_SYSTEM://������Ϣ
		{
			m_bGameSystem=!m_bGameSystem;
			return true;
		}
	case IDM_CALL_GM: //����GM
		{
			//Ч��״̬
			/*if ((m_dwCurrentUserID==0L)||(m_bLogonFinish==false)) return TRUE;
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
			if (pUserItem==NULL) return TRUE;

			//��������
			CDialogCallGM m_pCallGMDlg;
			if(m_pCallGMDlg.DoModal()==IDOK)
			{	
			MSG_GR_Call_GM s_callGM;
			s_callGM.bCallReason=0;
			s_callGM.szMsg[200]=0;
			if(m_pCallGMDlg.game_error)
			s_callGM.bCallReason|=0x1;
			if(m_pCallGMDlg.court)
			s_callGM.bCallReason|=0x2;
			if(m_pCallGMDlg.publicPoison)
			s_callGM.bCallReason|=0x4;
			if(m_pCallGMDlg.others)
			s_callGM.bCallReason|=0x8;
			s_callGM.uLength=m_pCallGMDlg.szCallGMmsg.GetLength();
			lstrcpy(s_callGM.szMsg,m_pCallGMDlg.szCallGMmsg);
			//TCHAR sz[20];
			//AfxMessageBox(ltoa(s_callGM.bCallReason,sz,10));
			//AfxMessageBox(m_pCallGMDlg.szCallGMmsg);
			//AfxMessageBox(s_callGM.szMsg);
			SendData(&s_callGM,sizeof(s_callGM),MDM_GR_MANAGE,ASS_GR_CALL_GM,0);
			}
			return true;*/
		}
		//case IDM_LOOK_USER_IP://�鿴IP
		//	{
		//	}
	case IDM_LIMIT_OTHER_ROOM_TALK://��ֹ��������
		{
			//Ч��״̬
			if ((m_dwCurrentUserID==0L)||(m_bLogonFinish==false)) return TRUE;
			UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
			if (pUserItem==NULL) return TRUE;

			//Ч������
			ASSERT(CUserPower::CanLimiteRoomTalk(m_GameInfo.dwMasterPower)==true);

			//��ʾ��Ϣ
			CString strMessage;
			CString str;
			str = fMsg.GetKeyVal("GameRoom","WhetherForbidChatInHall","ȷʵҪ��ֹ��ң�[ %s ]����������?");
			strMessage.Format(str,pUserItem->GameUserInfo.nickName);
			//if (AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_YESNO)==IDOK)//,MB_ICONQUESTION|MB_YESNO,this)==IDYES)
			if (DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strMessage)==IDYES)
			{
				MSG_GR_S_CutUser CutUser;
				CutUser.dwTargetID=m_dwCurrentUserID;
				CutUser.dwUserID=m_pLogonInfo->dwUserID;
				SendData(&CutUser,sizeof(CutUser),MDM_GR_MANAGE,ASS_GR_LIMIT_OTHER_ROOM_TALK,0);
			}
			return true;
		}
	case IDM_SKIN:
	case IDM_SKIN+1:
	case IDM_SKIN+2:
	case IDM_SKIN+3:
	case IDM_SKIN+4:				//�����������
		{
			int basesk=IDM_SKIN;
			int sk=LOWORD(wParam)-basesk;
			CNewGameClass * pRoomTab=&((CGamePlaceDlg *)AfxGetMainWnd())->m_RoomTabWnd;
			//pRoomTab->ChangeSkin(m_skinmgr.GetSkinBcfFileName(),
			//	m_skinmgr.GetKeyVal(m_skinmgr.GetSkinBcfFileName()),
			//	m_skinmgr.GetSkinPath());
			pRoomTab->PostMessage(WM_LOADSKIN,(WPARAM)sk,0);
			//pRoomTab->PostMessage(WM_EXCHANGE_SKIN,0,0);
		}
		return true;
	}
	return __super::OnCommand(wParam, lParam);
}

//������ʾ����
void CGameRoomEx::UpdateRoomTitle()///������Ϸ�����ʱ��,����
{	
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;

	if (IsWindowVisible()==TRUE)
	{
		CString strBuffer;
		INT_PTR uOnLineCount=__min(m_UserManage.GetOnLineUserCount(),m_DeskFrame.GetDeskCount()*m_DeskFrame.GetEveryDeskCount());
		if (m_bLogonFinish==false)
		{
			str = fMsg.GetKeyVal("GameRoom","FillString","�� %s �� %s �� -- �� %s ��");
			strBuffer.Format(str,m_GameInfo.szGameName,m_pRoomInfo->pComRoomInfo.szGameRoomName,m_pLogonInfo->nickName);
		}
		else
		{
			str = fMsg.GetKeyVal("GameRoom","OnLineCount","�� %s �� %s �� -- �� %s �� -- �� ����������%ld ��");
			strBuffer.Format(str,m_GameInfo.szGameName,m_pRoomInfo->pComRoomInfo.szGameRoomName,m_pLogonInfo->nickName,uOnLineCount);
		}
		//		((CGamePlaceDlg *)AfxGetMainWnd())->SetWindowTitle(strBuffer);
	}
	return;
}

void CGameRoomEx::OnTimer(UINT nIDEvent)
{
	CBaseRoom::OnTimer(nIDEvent);
	if(nIDEvent>=2000 && nIDEvent<3000)
	{//20081212 Fred Huang
		KillTimer(nIDEvent);
		int nDeskNo=nIDEvent-2000;
		//m_DeskFrame.SetLock(pNetHead->bAssistantID,false);
		UserItemStruct *pUserInfo=NULL;
		for(int i=0;i<m_GameInfo.uDeskPeople;i++)
		{
			pUserInfo=(UserItemStruct*)m_DeskFrame.GetUserItem(nDeskNo,i);
			int p=rand()%5;
			if(pUserInfo)
			{
				//m_DeskFrame.SetUserItem(pNetHead->bAssistantID,i,NULL);
				pUserInfo->GameUserInfo.bUserState=USER_SITTING;
				int w=rand()%3-1;
				if(w==1)
				{
					pUserInfo->GameUserInfo.dwPoint+=p;
					pUserInfo->GameUserInfo.uWinCount++;
					if(m_GameInfo.uComType==TY_MONEY_GAME)
						pUserInfo->GameUserInfo.i64Money+=p;
				}
				else if(w==-1)
				{
					pUserInfo->GameUserInfo.dwPoint-=p;
					pUserInfo->GameUserInfo.uLostCount++;
					if(m_GameInfo.uComType==TY_MONEY_GAME)
						pUserInfo->GameUserInfo.i64Money+=p;
				}
				else
					pUserInfo->GameUserInfo.uMidCount++;
				w=rand()%100;
				if(w<15)//10%�ĸ��ʸ�����ֵ
				{
					w=rand()%3-1;
					pUserInfo->GameUserInfo.dwFascination+=w;

				}
				//pUserInfo->GameUserInfo.bDeskNO=255;
				//pUserInfo->GameUserInfo.bDeskStation=255;
				m_pRightWnd->m_UserList.UpdateGameUser(pUserInfo);
			}
		}
		CBaseDeskView * pDesk=m_DeskFrame.GetDeskArrayPtr(int(nDeskNo));
		//pDesk->SetLock(false,&m_DeskFrame);
		m_DeskFrame.SetPlayGame(nDeskNo,false);
		m_DeskFrame.UpdateDeskView(nDeskNo);
		SetTimer(3000+nDeskNo,(rand()%15+2)*1000,NULL);
		return;
	}
	if(nIDEvent>=3000 && nIDEvent<4000)
	{//20081212 Fred Huang
		KillTimer(nIDEvent);
		int nDeskNo=nIDEvent-3000;
		//m_DeskFrame.SetLock(pNetHead->bAssistantID,false);
		UserItemStruct *pUserInfo=NULL;
		for(int i=0;i<m_GameInfo.uDeskPeople;i++)
		{
			pUserInfo=(UserItemStruct*)m_DeskFrame.GetUserItem(nDeskNo,i);
			if(pUserInfo)
			{
				//m_DeskFrame.SetUserItem(pNetHead->bAssistantID,i,NULL);
				pUserInfo->GameUserInfo.bUserState=USER_ARGEE;
				//pUserInfo->GameUserInfo.bDeskNO=255;
				//pUserInfo->GameUserInfo.bDeskStation=255;
				m_pRightWnd->m_UserList.UpdateGameUser(pUserInfo);
			}
		}
		CBaseDeskView * pDesk=m_DeskFrame.GetDeskArrayPtr(int(nDeskNo));
		//pDesk->SetLock(true,&m_DeskFrame);
		m_DeskFrame.UpdateDeskView(nDeskNo);
		//m_DeskFrame.SetPlayGame(nDeskNo,false);
		SetTimer(4000+nDeskNo,(rand()%3)*1000,NULL);
		return;
	}
	if(nIDEvent>=4000)
	{//20081212 Fred Huang
		KillTimer(nIDEvent);
		int nDeskNo=nIDEvent-4000;
		//m_DeskFrame.SetLock(pNetHead->bAssistantID,false);
		UserItemStruct *pUserInfo=NULL;
		for(int i=0;i<m_GameInfo.uDeskPeople;i++)
		{
			pUserInfo=(UserItemStruct*)m_DeskFrame.GetUserItem(nDeskNo,i);
			if(pUserInfo)
			{
				//m_DeskFrame.SetUserItem(pNetHead->bAssistantID,i,NULL);
				pUserInfo->GameUserInfo.bUserState=USER_PLAY_GAME;
				//pUserInfo->GameUserInfo.bDeskNO=255;
				//pUserInfo->GameUserInfo.bDeskStation=255;
				m_pRightWnd->m_UserList.UpdateGameUser(pUserInfo);
			}
		}
		CBaseDeskView * pDesk=m_DeskFrame.GetDeskArrayPtr(int(nDeskNo));
		//pDesk->SetLock(true,&m_DeskFrame);   //ȥ��������־   yjj 090306
		m_DeskFrame.SetPlayGame(nDeskNo,true);
		m_DeskFrame.UpdateDeskView(nDeskNo);
		SetTimer(2000+nDeskNo,(m_GameInfo.uVirtualGameTime+rand()%20-10)*1000,NULL);
		return;
	}
	if(nIDEvent==TIME_KEEP_ALIVE)
	{
		//if(m_nKeepAliveCount>=2)//�Ѿ��Ͽ�����,10��
		if(m_nKeepAliveCount>=5)//20090928 ZLP �ͻ��˶��������޸�
		{
			//KillTimer(TIME_KEEP_ALIVE);
			OnSocketCloseEvent();
		}
		else
		{
			m_nKeepAliveCount++;
			SendData(MDM_CONNECT,ASS_NET_TEST,0);//������Ϣ����
		}
	}
	else if(nIDEvent==ID_CHECK_SIGNAL)
    {
        checkNetSignal();

        // PengJiLin, 2010-9-13, ���˿������߿�ʹ��ʱ�䵽�ڼ��
        long int dwNowTime = 0;
        if(TRUE == m_bCheckKickPropTime)
        {
            dwNowTime = (long int)time(NULL);
            if(m_iKickPropTotalTime < dwNowTime)    // ʱ�䵽
            {
                m_bCheckKickPropTime = FALSE;

                // �ڷ�����Ϣ������Ϸ��Ϣ����ʾϵͳ��Ϣ
                TCHAR chMessage[] = "��ʹ�õ�[���˿�]�ѹ���, ���Լ������ʹ�ã��������¹���";
                m_pRightWnd->m_MessageHandle.InsertSystemMessage(chMessage);
                SendMessageToGame(chMessage);
            }

        }
        if(TRUE == m_bCheckAntiKickPropTime)
        {
            dwNowTime = (long int)time(NULL);
            if(m_iAntiKickPropTotalTime < dwNowTime)    // ʱ�䵽
            {
                m_bCheckAntiKickPropTime = FALSE;

                // �ڷ�����Ϣ������Ϸ��Ϣ����ʾϵͳ��Ϣ
                TCHAR chMessage[] = "��ʹ�õ�[���߿�]�ѹ���, ���Լ������ʹ�ã��������¹���";
                m_pRightWnd->m_MessageHandle.InsertSystemMessage(chMessage);
                SendMessageToGame(chMessage);
            }
        }
    }

	else if(nIDEvent == ID_TIME_ADD_SEQUE)//������Ϸ����
	{
		OnSitDesk();
	}
	else if(nIDEvent==LOAD_NEWS)//��������
	{
		ShowNewsOrSysMsg(LOAD_NEWS);
	}
	else if(nIDEvent==LOAD_SYS_MSG)//ϵͳ��Ϣ
	{
		ShowNewsOrSysMsg(LOAD_SYS_MSG);
	}
	else if(nIDEvent==IDR_EXIT)
	{
		int agreepeople=0;
		if(m_GameInfo.pMeUserInfo->GameUserInfo.bUserState!=USER_SITTING)
		{
			m_itime=60;
			KillTimer(IDR_EXIT);
			return ;
		}
		m_itime--;
		if(m_itime<=0)
		{
			SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
			if (m_IGameFrame!=NULL)
			{
				m_IGameFrame->AFCCloseFrame();
				//m_IGameFrame = NULL;
			}
		}
		//TCHAR sz[10];
		//wsprintf(sz,"%d",m_itime);
		//m_pRightWnd->m_MessageHandle.InsertSystemMessage(sz);
		if(m_itime==10)
		{//ģ�����������Ϸ����ϵͳ��Ϣ
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
			CString str;
			str = fMsg.GetKeyVal("GameRoom","IfNotStartWillLeave","����������ʼ��Ϸ,10����Զ��뿪.");
			m_pRightWnd->m_MessageHandle.InsertSystemMessage(str.GetBuffer());
			NetMessageHead nethead;
			MSG_GA_S_Message	msg;
			memset(&nethead,0,sizeof(nethead));
			memset(&msg,0,sizeof(msg));
			nethead.bMainID=MDM_GM_MESSAGE;
			nethead.bAssistantID=ASS_GM_SYSTEM_MESSAGE;
			str = fMsg.GetKeyVal("GameRoom","IfNotStartWillLeave","����������ʼ��Ϸ,10����Զ��뿪.");
			lstrcpy(msg.szMessage,str.GetBuffer());
			msg.bShowStation=0x01;
			if (m_IGameFrame!=NULL)
			{
				//{{modified by zxd
				//m_IGameFrame->OnSocketReadEvent(&nethead,&msg,0,NULL); 
				m_IGameFrame->OnSocketReadEvent(&nethead,&msg,sizeof(MSG_GA_S_Message),NULL);
				//modified by zxd}}
			}

		}

	}
	else if (nIDEvent == TIME_SLEEP_EXIT_LACK)
	{
		// add xqm 2010-11-24
		KillTimer(TIME_SLEEP_EXIT_LACK);				
		m_GameInfo.bGameStation = 0;
		SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0); ///< ��Ҳ���Ǯ����Ϸ�����߳���

		//if (m_IGameFrame)
			//AfxGetMainWnd()->PostMessage(QUIT_GAMEROOM,(WPARAM)2,(LPARAM)m_GameInfo.uLessPoint);
		if (m_IGameFrame!=NULL)
		{			
			m_IGameFrame->OnControlMessage(CM_U3D_APPLICATION_QUIT, 0, 0);
			m_IGameFrame->AFCCloseFrame();
		}	

		// add xqm 2010-11-25 ����������Ӻ�, �޸ķ�����밴ť��״̬
		if (!IsQueueGameRoom())
			GetDlgItem(IDC_BTN_ROOM3)->EnableWindow(true);
		
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString strBuffer;
		TCHAR szNum[128], szCur[128];
		GetNumString(szNum, m_GameInfo.uLessPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
		CString str = fMsg.GetKeyVal("GameRoom","NeedCoins","�˷�����Ҫ�������� %s �Ľ��");
		strBuffer.Format(str, szNum);
		m_pRightWnd->m_MessageHandle.InsertSystemMessage(strBuffer.GetBuffer());
		strBuffer.ReleaseBuffer();

		GetNumString(szCur, m_GameInfo.pMeUserInfo->GameUserInfo.i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);

		UINT uMsgRet = DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,
			fMsg.GetKeyVal("GameRoom","NeedCoinsRecharge","�˷�����Ҫ���� %s �Ľ�Ҳ��ܿ�ʼ��Ϸ,�����ڵĽ����Ϊ%s����������Ǯ��ȡ�����,��ʼ���ʴ̼�����Ϸ��!"),
			szNum,szCur);

		if(uMsgRet == IDYES)
		{
			//wushuqun 2009.7.10 �޸��û����С�ڷ�����ʱ����ʾ��ʽ
			if (m_lessPointType == 1)
			{
				OnBnClickedBt8();
			}
			else
			{
				//������վ��ֵҳ��
				ShellExecute(NULL,"open",m_strLessPointTipURL,NULL,NULL,SW_MAXIMIZE);
			}
		}
		else	/// �����ͬ��ȡǮ�����˳�����
		{
			OnBnClickedClose();
		}			
	}

	else if (nIDEvent == TIME_SLEEP_EXIT_OVERFLOW)
	{
		// Add By JianGuankun 2012.8.14

		KillTimer(TIME_SLEEP_EXIT_OVERFLOW);				
		m_GameInfo.bGameStation = 0;
		SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0); ///< ��Ҳ���Ǯ����Ϸ�����߳���

		if (m_IGameFrame!=NULL)
		{			
			m_IGameFrame->AFCCloseFrame();			
		}	

		if (!IsQueueGameRoom())
			GetDlgItem(IDC_BTN_ROOM3)->EnableWindow(true);

		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		TCHAR szNum[128], szCur[128];
		GetNumString(szNum, m_GameInfo.uMaxPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
		GetNumString(szCur, m_GameInfo.pMeUserInfo->GameUserInfo.i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);

		UINT uMsgRet = DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,
			fMsg.GetKeyVal("GameRoom","NeedCoinsRechargeMax","<p 5>�˷�����Ҫ�������Ϊ <b>%s</b>�������ڵĽ����Ϊ <b>%s</b> ��</p>"),
			szNum,szCur);

		if(uMsgRet == IDYES)
		{
			if (m_lessPointType == 1)
			{
				OnBnClickedBt8();
			}
			else
			{
				ShellExecute(NULL,"open",m_strLessPointTipURL,NULL,NULL,SW_MAXIMIZE);
			}
		}
		else	/// �����ͬ��ȡǮ�����˳�����
		{
			OnBnClickedClose();
		}			
	}

	else if (nIDEvent == TIME_NOACTION_EXIT)
	{
		KillTimer(TIME_NOACTION_EXIT);
		CloseGameClient();
		OnBnClickedClose();
		AfxGetMainWnd()->PostMessage(QUIT_GAMEROOM,(WPARAM)6, (LPARAM)m_GameInfo.uNoActionKickTimer);
	}
	else if(nIDEvent == TIME_CHECK_MAX_POINT)
	{
		KillTimer(TIME_CHECK_MAX_POINT);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//���������ж�JianGK 2011.10.28
		if (m_GameInfo.uMaxPoint != 0L)
		{
// 			if (m_pRoomInfo->pComRoomInfo.uComType == TY_NORMAL_GAME)
// 			{
// 				if (m_GameInfo.pMeUserInfo->GameUserInfo.dwPoint>(int)(m_GameInfo.uMaxPoint))
// 				{
// 					CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
// 					TCHAR szNum[128]; 
// 					GetNumString(szNum, m_GameInfo.uMaxPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
// 
// 					DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,
// 						fMsg.GetKeyVal("GameRoom","OverflowCoints","���Ľ���Ѷ���%s���뵽�������䡣"),szNum);
// 
// 					OnBnClickedClose();
// 				}
// 			}
// 			else if (m_pRoomInfo->pComRoomInfo.uComType == TY_MONEY_GAME)
// 			{
// 				if (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money>(int)(m_GameInfo.uMaxPoint))
// 				{
// 					CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
// 					TCHAR szNum[128]; 
// 					GetNumString(szNum, m_GameInfo.uMaxPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
// 
// 					DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,
// 						fMsg.GetKeyVal("GameRoom","OverflowCoints","���Ľ���Ѷ���%s���뵽�������䡣"),szNum);
// 
// 					OnBnClickedClose();
// 				}
// 			}
		}
	}
	else if (nIDEvent == TIME_ENABLE_EXIT_BTN)
	{
		KillTimer(TIME_ENABLE_EXIT_BTN);
		m_bt1.EnableWindow(true);
	}
	else if (nIDEvent == TIME_QUEUEROOM_DISMISS)
	{
		KillTimer(TIME_QUEUEROOM_DISMISS);
		m_bQueueRoomDisMiss = false;
		if (NULL != m_GameInfo.pMeUserInfo && m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO != 255)
			SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
		if (m_bCloseIsAutoQueue)
			JoinQueue();
	}
	//�Ŷ�ʱ��������л����������ķ���
	else if (nIDEvent == TIME_QUEUEROOM_TIMEOUT)
	{
		KillTimer(TIME_QUEUEROOM_TIMEOUT);
		if (m_IGameFrame == NULL && CanCloseNow())
		{
			int _index = FindMaxPeopleRoom();
			if (_index != -1 && _index != m_iContestRoomIndex)
			{
				RelaodRoom(_index);
			}
		}
		SetTimer(TIME_QUEUEROOM_TIMEOUT, m_iQueueTimeOut * 1000, NULL);
	}
	//�л�ʱ�������ˣ�������������ķ���
	else if (nIDEvent == TIME_CHECK_SOCKETCLOSE)
	{
		KillTimer(TIME_CHECK_SOCKETCLOSE);
		if ((m_TCPSocket != NULL && m_TCPSocket->GetSocketState()==NO_CONNECT) || m_pUserList->GetItemCount() == 0)
			m_iSocketClose++;
		if (m_iSocketClose > 3)
		{
			int _index = FindMaxPeopleRoom();
			if (_index != -1)
			{
				m_iSocketClose = 0;
				RelaodRoom(_index);
			}
		}
		SetTimer(TIME_CHECK_SOCKETCLOSE, 1000, NULL);
	}
}

//void CGameRoomEx::OnBnClickedListpagedown()
//{
//	m_pRightWnd->m_UserList.SendMessage(WM_VSCROLL,SB_PAGEDOWN,0);
//}
//
//void CGameRoomEx::OnBnClickedListpageup()
//{
//	m_pRightWnd->m_UserList.SendMessage(WM_VSCROLL,SB_PAGEUP,0);
//}
////�˳�
//void CGameRoomEx::OnBnClickedBt7()
//{
//	OnBnClickedClose();
//}
////���ⷴ��
//void CGameRoomEx::OnBnClickedBt6()
//{
//	TCHAR url[300];
//	lstrcpy(url,"www.wpgame.com/bbsxp/index.asp?boardid=8");
//	ShellExecute(NULL,"open",url,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
//}

//void CGameRoomEx::OnBnClickedShowmenu()
//{
//	//CRect rect;
//	//m_showmenu.GetClientRect(&rect);
//	//m_showmenu.MapWindowPoints(this,&rect);
//	//m_menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_BOTTOMALIGN,rect.right,rect.top,this);
//}
//
//void CGameRoomEx::OnBnClickedSkin()
//{
//	//CRect rect;
//	//m_Btsk.GetClientRect(&rect);
//	//m_Btsk.MapWindowPoints(this,&rect);
//	////�����˵�
//	//CString strBuffer;
//	//CMenu UserInfoMenu;
//	//UserInfoMenu.CreatePopupMenu();
//	//int cursk=Glb().m_skin.GetAt(4);
//	//cursk -='0';
//	//for(int i=0;i<5;i++)
//	//{
//	//	if(Glb().m_sktext[i]=="")
//	//		break;
//	//	UserInfoMenu.AppendMenu(0,IDM_SKIN+i,Glb().m_sktext[i]);
//	//	if(i==cursk)
//	//		UserInfoMenu.CheckMenuItem(IDM_SKIN+i,MF_BYCOMMAND|MF_CHECKED);
//	//}
//	//UserInfoMenu.TrackPopupMenu(TPM_RIGHTALIGN|TPM_TOPALIGN,rect.left,rect.top,this);
//}

void CGameRoomEx::LoadSkin(void)
{
	int r,g,b;
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("GameRoom");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder = f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());


	//LoadButtonBitmap
	//ֻ�ô˹��ܰ�ť
	wsprintf(path,"%sroom_mid\\room_bt01.bmp",skinfolder);
	m_bt1.LoadButtonBitmap(path,false);
	wsprintf(path,"%sroom_mid\\room_bt02.bmp",skinfolder);
	m_bt2.LoadButtonBitmap(path,false);
	wsprintf(path,"%sroom_mid\\room_bt03.bmp",skinfolder);
	m_bt3.LoadButtonBitmap(path,false);
	wsprintf(path,"%sroom_mid\\room_bt04.bmp",skinfolder);
	m_bt4.LoadButtonBitmap(path,false);
	wsprintf(path,"%sroom_mid\\room_bt05.bmp",skinfolder);
	m_bt5.LoadButtonBitmap(path,false);
	wsprintf(path,"%sroom_mid\\room_bt06.bmp",skinfolder);  
	m_bt6.LoadButtonBitmap(path,false);
	//wsprintf(path,"%sroom_mid\\room_bt07.bmp",skinfolder);
	//m_bt7.LoadButtonBitmap(path,false);
	//wsprintf(path,"%sroom_mid\\room_bt08.bmp",skinfolder);
	//m_bt8.LoadButtonBitmap(path,false);
	wsprintf(path,"%sroom_mid\\room_bt09.bmp",skinfolder);
	m_bt9.LoadButtonBitmap(path,false);
	//wsprintf(path,"%sroom_mid\\room_bt10.bmp",skinfolder);
	//m_bt10.LoadButtonBitmap(path,false);
	//���
	wsprintf(path,"%sroom_mid\\room_lc.bmp",skinfolder);
	m_roomlc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_mid\\room_rc.bmp",skinfolder);
	m_roomrc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_mid\\room_tc.bmp",skinfolder);
	m_roomtc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_mid\\room_bc.bmp",skinfolder);
	m_roombc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//�߽�
	wsprintf(path,"%sroom_mid\\room_lt.bmp",skinfolder);
	m_roomlt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_mid\\room_rt.bmp",skinfolder);
	m_roomrt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_mid\\room_lb.bmp",skinfolder);
	m_roomlb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_mid\\room_rb.bmp",skinfolder);
	m_roomrb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	wsprintf(path, "%sroom_mid\\main_lt.bmp", skinfolder);
	m_mainlt.SetLoadInfo(path, CGameImageLink::m_bAutoLock);
	wsprintf(path, "%sroom_mid\\main_rt.bmp", skinfolder);
	m_mainrt.SetLoadInfo(path, CGameImageLink::m_bAutoLock);

	//��ťλ�úͼ�϶
	m_iBtnX = f.GetKeyVal(key,"Room_BtnX",10);
	m_iBtnY = f.GetKeyVal(key,"Room_BtnY",10);
	m_iBtnSpace = f.GetKeyVal(key,"Room_Space",10);

	wchar_t wpath[MAX_PATH];
	swprintf(wpath,m_skinmgr.GetSkinPath() + L"signal.bmp");
	
#ifndef BZ_ZLIB  //< add by wxx 
	img=Image::FromFile(wpath);
#else
	char  pchar[MAX_PATH];
	sprintf(pchar,m_skinmgr.GetSkinPath() + "signal.bmp");
	CUnZipRes unZipRes;
	IStream * pStream=NULL;
	pStream=unZipRes.LoadFileResFromZip(pchar);

	img=new Image(pStream);
	if(pStream!=NULL)
	{
		pStream->Release();
		pStream=NULL;
	}
#endif
	
	
    //�ȴ�����ͼ yjj 090227

	//�����źŵ���ʾλ��
	m_netSignX = f.GetKeyVal(key, _T("game_room_signalx"),5);
	m_netSignY = f.GetKeyVal(key, _T("game_room_signaly"),0);

	//����������ʾλ��
	m_roomNameX = f.GetKeyVal(key, _T("roomNameX"),142);
	m_roomNameY = f.GetKeyVal(key, _T("roomNameY"),2);

	m_roomNameClr = f.GetKeyVal(key,"roomNameColor",(int)RGB(0,0,255));

	m_toolBarX = f.GetKeyVal(key,"gametoolbardx",1);
	m_toolBarW = f.GetKeyVal(key,"gametoolbardw",1);

	//wushuqun 2009.7.10 �޸��û����С�ڷ�����ʱ����ʾ��ʽ
	CBcfFile f1( s + "bzgame.bcf");
	//Ĭ��Ϊ�����жԻ���
	m_lessPointType = f1.GetKeyVal("BZW","LessPointTipType",0);
	m_strLessPointTipURL = f1.GetKeyVal("BZW","LessPointTipURL",Glb().m_CenterServerPara.m_strHomeADDR);
}

//���¼���û���Ϣ
void  CGameRoomEx::AddUserInfoToRecord()
{
	if ((m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO!=255))
	{
		for (UINT i=0;i<m_GameInfo.uDeskPeople;i++)
		{
			UserItemStruct * pUserItem=m_DeskFrame.GetUserItem(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO,i);
			if (pUserItem!=NULL) 
			{				
                if(m_byVideoUse != 0)  // PengJiLin, 2010-5-18, ���¼�����ù���
				    m_AddRecordModule.SetUserInfo2Record(pUserItem,m_GameInfo.pMeUserInfo);   //Ϊ¼��ϵͳ�������			
			}
		}
	}
}

LRESULT CGameRoomEx::OnLoadSkin(WPARAM wparam, LPARAM lparam)
{
	LoadSkin();
	//m_pRightWnd->m_UserList.LoadSkin();
	//m_UserListFriend.LoadSkin();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	int cx,cy;
	cx=ClientRect.Width();
	cy=ClientRect.Height();
	OnSizeEvent(0,cx,cy);
	if(m_IGameFrame)
	{//��Ϸ�Ѳ���Ҫ���׽���
		//m_IGameFrame->SendWindowMessage(WM_LOADSKIN,wparam,lparam);
	}
	return true;
}

////�ض�talk.ini
//LRESULT  CGameRoomEx::OnReLoadTalkINI(WPARAM wparam, LPARAM lparam)
//{
//	//m_pRightWnd->m_ChatControl.ReLoadTalkINI();
//	return 0;
//}
//��ȡ����
LRESULT CGameRoomEx::OnReflashNews(WPARAM wparam, LPARAM lparam)
{	
	KillTimer(LOAD_NEWS);
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString szTitleNumber;
	CINIFile f( s + "news.ini");
	CString sec="BZW_NEWS";
	int bStop=0;
	int time=10;//��
	CString szTitle;
	bStop=f.GetKeyVal(sec,"Stop",0);
	m_iNewsNO=1;
	m_iNewCurCircle = 0;
	time=f.GetKeyVal(sec,"Time",10);
	if(bStop==1||time==0)//ֹͣ��ʾ
	{
		KillTimer(LOAD_NEWS);
		return 0;
	}

	SetTimer(LOAD_NEWS,time*1000,NULL);
	ShowNewsOrSysMsg(LOAD_NEWS);
	return 0;
}
//��ȡϵͳ��Ϣ
LRESULT CGameRoomEx::OnReflashSys(WPARAM wparam, LPARAM lparam)
{
	KillTimer(LOAD_SYS_MSG);
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString szTitleNumber;
	CINIFile f( s + "news.ini");
	CString sec="BZW_SYS_MSG";
	int bStop=0;
	int time=10;//��
	CString szTitle;
	bStop=f.GetKeyVal(sec,"Stop",0);
	m_iSysMsgNO=1;
	m_iSysCurCircle = 0;
	time=f.GetKeyVal(sec,"Time",10);
	if(bStop==1||time==0)//ֹͣ��ʾ
	{
		KillTimer(LOAD_SYS_MSG);
		return 0;
	}

	SetTimer(LOAD_SYS_MSG,time*1000,NULL);
	ShowNewsOrSysMsg(LOAD_SYS_MSG);
	return 0;
}

//��ʾϵͳ��Ϣ������
void CGameRoomEx::ShowNewsOrSysMsg(BYTE newsOrSysMsg)
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString szTitleNumber;
	CINIFile f( s + "news.ini");
	CString sec;
	CString szTitle;
	int iNO=0,iTotleNumber=0,iNumber=1;
	int iTotal = 0; // ѭ������
	if(newsOrSysMsg==LOAD_NEWS)//����
	{
		sec="BZW_NEWS";
		iTotleNumber=f.GetKeyVal(sec,"Number",0);//����
		iNumber=f.GetKeyVal(sec,"Every",1);//ÿ������
		iTotal = f.GetKeyVal(sec,"circle",1);//ÿ������
		for(iNO=m_iNewsNO;((iNO<m_iNewsNO+iNumber)&&(iNO<=iTotleNumber));iNO++)
		{
			szTitleNumber.Format("%d",iNO);
			szTitle=f.GetKeyVal(sec,"Title"+szTitleNumber,"");

			CString strFontName;
			strFontName=f.GetKeyVal(sec,"FontName"+szTitleNumber,"����");

			int iFontSize=0;
			iFontSize=f.GetKeyVal(sec,"FontSize"+szTitleNumber,0);

			int iGameShow=0,iRoomShow=0;
			iGameShow=f.GetKeyVal(sec,"GameShow"+szTitleNumber,0);
			iRoomShow=f.GetKeyVal(sec,"RoomShow"+szTitleNumber,0);

			if(!szTitle.IsEmpty())
			{
				TCHAR  szFontName[16];
				::lstrcpy(szFontName,strFontName);
				if(iRoomShow!=0)
				{
					if(iFontSize==0)
						m_pRightWnd->m_MessageHandle.InsertNews(szTitle,iFontSize);
					else
						m_pRightWnd->m_MessageHandle.InsertNews(szTitle,iFontSize,szFontName);
				}
				if(iGameShow!=0&&m_IGameFrame!=NULL)
				{
					TCHAR  sz[1000];
					::lstrcpy(sz,szTitle);
					if(iFontSize==0)
						m_IGameFrame->ShowMsgInGame(sz, 1);
					else
						m_IGameFrame->ShowMsgInGame(sz,1,iFontSize,szFontName);
				}
			}
		}
		//����λ��
		if(iNO>iTotleNumber)
		{
			m_iNewsNO=1;
			m_iNewCurCircle++;
		}
		else m_iNewsNO=iNO;
		if (m_iNewCurCircle >= iTotal)
		{
			KillTimer(LOAD_NEWS);
		}
	}
	else if(newsOrSysMsg==LOAD_SYS_MSG)
	{
		sec="BZW_SYS_MSG";
		iTotleNumber=f.GetKeyVal(sec,"Number",0);//����
		iNumber=f.GetKeyVal(sec,"Every",1);//ÿ������
		iTotal = f.GetKeyVal(sec,"circle",1);//ÿ������
		for(iNO=m_iSysMsgNO;((iNO<m_iSysMsgNO+iNumber)&&(iNO<=iTotleNumber));iNO++)
		{
			szTitleNumber.Format("%d",iNO);
			szTitle=f.GetKeyVal(sec,"Title"+szTitleNumber,"");

			CString strFontName;
			strFontName=f.GetKeyVal(sec,"FontName"+szTitleNumber,"����");

			int iFontSize=0;
			iFontSize=f.GetKeyVal(sec,"FontSize"+szTitleNumber,0);

			int iGameShow=0,iRoomShow=0;
			iGameShow=f.GetKeyVal(sec,"GameShow"+szTitleNumber,0);
			iRoomShow=f.GetKeyVal(sec,"RoomShow"+szTitleNumber,0);

			if(!szTitle.IsEmpty())
			{
				//������Ϣ
				TCHAR  sz[1000],szFontName[16];
				::lstrcpy(sz,szTitle);
				::lstrcpy(szFontName,strFontName);
				if(iRoomShow!=0)
				{
					if(iFontSize==0)
						m_pRightWnd->m_MessageHandle.InsertSystemMessage(sz,iFontSize);
					else
						m_pRightWnd->m_MessageHandle.InsertSystemMessage(sz,iFontSize,szFontName);
				}
				if(iGameShow!=0&&m_IGameFrame!=NULL)
				{
					if(iFontSize==0)
						m_IGameFrame->ShowMsgInGame(sz,0);
					else
						m_IGameFrame->ShowMsgInGame(sz,0,iFontSize,szFontName);
				}
			}
		}
		//����λ��
		if(iNO>iTotleNumber)
		{
			m_iSysMsgNO=1;
			m_iSysCurCircle++;
		}
		else m_iSysMsgNO=iNO;
		if (m_iSysCurCircle >= iTotal)
		{
			KillTimer(LOAD_SYS_MSG);
		}
	}
}

//����Ƿ�Ҫ�뿪��Ϸ��
bool CGameRoomEx::CheckLeaveDesk()//��Ҹ���ʱ�����ж�
{
	if(m_GameInfo.pMeUserInfo->GameUserInfo.i64Money < m_GameInfo.uLessPoint)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		//�Ƿ�������Ϸ��
		if(m_GameInfo.pMeUserInfo->GameUserInfo.bUserState != USER_PLAY_GAME)
		{
			if(m_IGameFrame!=NULL)
			{
				m_IGameFrame->AFCCloseFrame();
				//m_IGameFrame = NULL;

			}
		}
		TCHAR szNum[128]; 
        GetNumString(szNum, m_GameInfo.uLessPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
		if (m_GameInfo.dwRoomRule & GRR_AUTO_SIT_DESK)
		{
			//wushuqun 2008.10.13
			//ר�ŶԷ����׳����д���ֱ���ߵ���������ֹ��Ϸ�������
			OnBnClickedClose();

			int r = DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,true,
				fMsg.GetKeyVal("GameRoom","LackCoins1",_T("����Ϸ��������Ҫ��%s�Ľ��,���Ľ�Ҳ���")),
				szNum);

			if (r == IDOK) NoEnoughMoneyEvent();
		}
		else
		{

			int r = DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,true,
				fMsg.GetKeyVal("GameRoom","LackCoins","����Ϸ��������Ҫ��%s�Ľ��,���Ľ�Ҳ���,�쵽Ǯ��ȡ��Ұ�!"),
				szNum);

			if (r == IDOK) NoEnoughMoneyEvent();
		}

		
		//��Ҫ�뿪
		return true;
	}

	//��Ҵ�࣬ҲҪ�뿪
	if (m_GameInfo.uMaxPoint != 0L)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		if (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money>(int)(m_GameInfo.uMaxPoint))
		{
			CString strMessage;
			TCHAR szNum[128]; 
			GetNumString(szNum, m_GameInfo.uMaxPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);

			DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,
				fMsg.GetKeyVal("GameRoom","OverflowCoints","���Ľ���Ѷ���%s�������ڴ˷�����Ϸ��"),szNum);

			return true;
		}
	}

	return false;
}
/// ˢ���û��б��еĽ��
/// @param dwUserID �û�ID��
/// @param uCount ��Ϊ��������ֵ����˼Ϊ�ӻ��
bool CGameRoomEx::FlushUserMoney(long dwUserID,__int64 i64Count)
{
	UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(dwUserID);
	if(pUserItem==NULL)
		return false;
	//��������
	pUserItem->GameUserInfo.i64Money += i64Count;
	pUserItem->GameUserInfo.i64Bank -= i64Count;//���н��
	//���´�����ʾ
	//��������
	//ע�⣺���ѵ��������,Fred Huang 2008-07-17
	if(dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID && !(m_GameInfo.dwRoomRule & GRR_EXPERCISE_ROOM || m_GameInfo.dwRoomRule & GRR_CONTEST))
		GetMainRoom()->UpdateMeInfo(pUserItem->GameUserInfo.i64Money,true,0,0);

	//������λ���û�
	m_pUserList->UpdateGameUser(pUserItem);
	if ((m_pFindUser!=NULL)&&(m_pFindUser->GetSafeHwnd()!=NULL))  
		m_pFindUser->UpdateUserItem(pUserItem);

	//���·����û�////
	CM_UserState_Change ControlMessage;
	ControlMessage.bActionCode=ACT_USER_POINT;
	ControlMessage.bDeskStation=pUserItem->GameUserInfo.bDeskStation;
	ControlMessage.pUserItem=pUserItem;
	if (m_IGameFrame != NULL)
		m_IGameFrame->OnControlMessage(CM_USER_STATE,&ControlMessage,sizeof(ControlMessage));

	//Add by JianGuankun 2012.1.5
	if (m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID == pUserItem->GameUserInfo.dwUserID) //�Լ�����Ϣ�б仯
	{
		GetMainRoom()->UpdateBankMoneyToAllRoom(pUserItem->GameUserInfo.i64Bank,this);
	}

	/////
	//////////////////////////ͬ��������������
	//ע�⣺���ѵ��������,Fred Huang 2008-07-17
	if(dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID && !(m_GameInfo.dwRoomRule & GRR_EXPERCISE_ROOM))
	{
		CMainRoomEx * pMainRoom;
		RoomInfoStruct* pRoomInfo;
		pMainRoom = GetMainRoom();
		if(pMainRoom!=NULL) 
		{
			pMainRoom->m_pGameListWnd->UpdateMeInfo(-i64Count,true);
			if(pMainRoom->propPannel)
			{
				if(::IsWindowVisible(pMainRoom->propPannel->m_hWnd))
					pMainRoom->propPannel->openPannel(-1);
			}
			//pMainRoom->m_PlaceUserInfo.dwBank-=i64Count;
			//pMainRoom->m_PlaceUserInfo.dwMoney+=i64Count;
			if(pMainRoom->GetRoomInfoStruct(&pRoomInfo))
			{
				for (BYTE i = 0; i < MAX_GAME_ROOM; i ++)
				{
					if (pRoomInfo[i].bAccess&&(pRoomInfo[i].pComRoomInfo.uRoomID!=m_pRoomInfo->pComRoomInfo.uRoomID))
					{
                        g_i64MyMoney = pUserItem->GameUserInfo.i64Money;
						pRoomInfo[i].pGameRoomWnd->SendMessage(WM_REFLASHMONEY,0x80000001,0);
						return true;
					}
				}
			}else return true;
		}
	}
	return true;
}

///ˢ���û��Լ������н��
/// @param i64Count ���º�����н��
bool CGameRoomEx::FlushMyBankMoney(__int64 i64Count)
{
	m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank = i64Count;
	return true;
}

bool CGameRoomEx::Dudu(_TAG_BOARDCAST* pDudu,int iDuduType)
{	
	//������Ϣ
	TCHAR sz[255];
	lstrcpy(sz,pDudu->szUserName);
	//wushuqun 2009.5.19
	//�޸ķ����γ� ʹ�� ���� ����ʱ�ܿ����������
	if ((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) &&  
		strcmp(sz,m_GameInfo.pMeUserInfo->GameUserInfo.nickName) != 0)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString str = fMsg.GetKeyVal("GameRoom","Player","���");
		wsprintf(sz,str.GetBuffer());
	}

	CHAR szMessage[BROADCAST_MESSAGE_MAX+1] = {0};//��������
	::memcpy(szMessage,pDudu->szMessage,::strlen(pDudu->szMessage));

	//lstrcat(sz,TEXT("����С����"));
	m_pRightWnd->m_MessageHandle.InsertDudu(sz,TEXT("��"),
		szMessage,pDudu->crColor,false,0,0,iDuduType);

	//lstrcat(sz,"�Դ��˵");
	if(m_IGameFrame!=NULL)
		m_IGameFrame->ShowMsgDudu(sz,TEXT("��"),
		pDudu->szMessage,pDudu->crColor,false,0,0,iDuduType);
	

	return true;
}

/*ˢ����ҵ�����
	@Author:JianGuankun 2012.6.27
	@param dwUserID ����������û�ID,
	@param uNewLogoID �������logo ID
*/
bool CGameRoomEx::OnAvatarChange(long dwUserID,UINT uNewLogoID)
{
	UserItemStruct* pUserItem = m_UserManage.FindOnLineUser(dwUserID);
	if (!pUserItem)
	{
		return false;
	}

	pUserItem->GameUserInfo.bLogoID = uNewLogoID;

	if (pUserItem->GameUserInfo.bDeskNO == 255)
	{
		return false;
	}

	//ˢ�����������ϵ�����
	m_DeskFrame.UpdateDeskView(pUserItem->GameUserInfo.bDeskNO);

	//ͬ�����ˢ������Ϸ�е�����
	if (m_IGameFrame)
	{
		if (pUserItem->GameUserInfo.bDeskNO == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)
		{
			CM_UserState_Change ControlMessage;
			ControlMessage.bActionCode = ACT_USER_AVATAR_CHANGE;

			UserItemStruct* pUis = new UserItemStruct;
			::memset(pUis,NULL,sizeof(UserItemStruct));
			*pUis = *pUserItem;
			ControlMessage.pUserItem = pUis;

			m_IGameFrame->OnControlMessage(CM_USER_STATE,&ControlMessage,sizeof(CM_UserState_Change));

			delete pUis;
		}
	}

	return true;
}

//���ͻ���
LRESULT CGameRoomEx::OnGetPoint(WPARAM wparam,LPARAM lparam)
{
	if((int)lparam != 1)
		return 0;
	MSG_GR_S_GetPoint getpoint;
	::memset(&getpoint,0,sizeof(getpoint));
	getpoint.dwPoint = (int)wparam;
	SendGameData(&getpoint, sizeof(getpoint), MDM_GR_POINT, ASS_GR_GET_POINT, 0);
	return 0;
}

//��Ϣ
LRESULT CGameRoomEx::OnSendRoomMsg(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case 1://��������
		OnSendRoomTalk();		
		break;
	case 2://���鴰��
		OnBnClickedExpression();
		break;
	case 3://���Ҽ��˵����Ҽ����������е�����б�ʱ
		{
			if (!IsQueueGameRoom())///�Ŷӻ�����ʾ�����Ϣ
			{
				OpenUserMenu((UserItemStruct*)lparam);
			}
			
			break;
		}
	case 4://����Ϣ��˫�������е�����б�ʱ��ָ�������������Ϣ
		{
			if (!IsQueueGameRoom())//Ϊ�Ŷӻ����
			{
				SendShortMessage((UserItemStruct*)lparam);
			}
		
			break;
		}
	case 10://����б��г�������Ϣ //20081211 , Fred Huang
		RandVirtualMessage((UserItemStruct*)lparam);
		break;
	default:
		break;
	}
	return 0;
}
//10�����ܰ�ť

//�˳�
void  CGameRoomEx::OnBnClickedBt1()
{
	OnBnClickedClose();
}
//����
void  CGameRoomEx::OnBnClickedBt2()
{
	OnBnClickedRoomSet();
	return ;
}
//����
void  CGameRoomEx::OnBnClickedBt3()
{
    // PengJiLin, 2010-9-15�����ӳɹ����������������λ
    // �Ż�������û�л������Ѿ����Լ�����Ϸ������
    if(FALSE == m_bAllowJoin) return;

	GetDlgItem(IDC_BTN_ROOM3)->EnableWindow(FALSE);
	//yinyi -080913 -�ڷ����׳��������ֱ���˳�����
	if((m_GameInfo.dwRoomRule & GRR_AUTO_SIT_DESK)!=0L)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString strMessage;
		//strMessage.Format(TEXT("�Բ������Ƿ����׷��䣬�������Լ�ѡ����λ"));
		strMessage.Format("%s%s%s",
			fMsg.GetKeyVal("GameRoom","NoChoose","�Բ������Ƿ����׷��䣬�������Լ�ѡ����λ��"),
			fMsg.GetKeyVal("GameRoom","CloseRoomToJoin","��Ҫ������Ϸ�����Թرշ�����ٽ��룬"),
			fMsg.GetKeyVal("GameRoom","AutoAssignSeat","ϵͳ����Ϊ���Զ�������λ�����ȷ���رձ����䣡"));
		//if(AFCMessageBox(strMessage,m_GameInfo.szGameName,AFC_YESNO)==IDOK)//,MB_ICONQUESTION|MB_OKCANCEL,this)==IDOK)
		if(DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,m_GameInfo.szGameName,true,strMessage)==IDYES)
		{
			OnBnClickedBt1();
		}
		return;
	}
	char cKey[10];
	CString sPath=CBcfFile::GetAppPath (false);
	CBcfFile fsr( sPath + "SpecialRule.bcf");
	sprintf(cKey, "%d", m_pRoomInfo->pComRoomInfo.uNameID);
	int iResult = fsr.GetKeyVal(_T("BJLType"), cKey, 0);

	if (iResult)
	{
		OnHitBJLDeskBegin(0,0);
	}
	else
	{
		OnFindNullDesk();
	}
}
/// ���а�ť�����
void  CGameRoomEx::OnBnClickedBt4()
{
	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�����й���

	if (m_GameInfo.dwRoomRule & GRR_CONTEST)
	{
		//AFCMessageBox("�Բ���, ����������ʹ�����й���", "��ܰ��ʾ");
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�Բ���, ����������ʹ�����й���");
		return ;
	}

	if (m_GameInfo.dwRoomRule & GRR_EXPERCISE_ROOM )
	{
		//AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ�����й���", "��ܰ��ʾ");
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�Բ���, ���鳡(��ѳ�)����ʹ�����й���");
		return ;
	}
	// end duanxiaohui

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	/// ��ʾ��ʾ�Ի����ֱ�ӷ��ʹ�������Ϣ������Ҫ��������
	if (m_Connect.GetSafeHwnd()==NULL) 
		m_Connect.Create(IDD_CONNECT,this);
	m_Connect.SetMessageText(fMsg.GetKeyVal("GameRoom","OpenningBankWait","���ڴ�Ǯ��,���Ժ�..."));
	m_Connect.GetDlgItem(IDC_QUIT)->EnableWindow(false);
	m_Connect.ShowWindow(SW_SHOW);
	SendData(MDM_GR_MONEY,ASS_GR_OPEN_WALLET,0);
}

/// �����������ð�����Ӧ
/// @param ��
/// @return ��
void CGameRoomEx::OnBnClickedBt5()
{
	////�������
	///*ShowFindUserWnd();
	//return ;
	////��������
	//if(m_finddesk.GetSafeHwnd()!=NULL)
	//{
	//	m_finddesk.ShowWindow(SW_SHOW);
	//	return ;
	//}
	//m_finddesk.m_pDeskFrame=&m_DeskFrame;
	//m_finddesk.m_pGameRoomEx=this;
	//m_finddesk.Create(IDD_FINDNULLDESK,this);
	//m_finddesk.CenterWindow();
	//m_finddesk.ShowWindow(SW_SHOW);
	////ChoiseDeskBasePoint();
	//*/

	//int propCout=Glb().userPropLibrary.GetCount();
	//int s=0;
	//for(int i=0;i<propCout;i++)
	//{
	//	_TAG_USERPROP *userProp=Glb().userPropLibrary.GetAt(i);
	//	if(!(userProp->attribAction & 0x01))
	//	{//��������
	//		////////////////////////////////////
	//		////////Kylin �޸� #2595 ����
	//		if(userProp->attribAction & 0x2)
	//		{
	//			////////////////////////////////////////////////////
	//			///2008 1216 Kylin ����ʱʹ�ø�������û����ʾ
	//			/*CString stip;
	//			
	//			if(m_GameInfo.pMeUserInfo->GameUserInfo.dwPoint>=0)
	//			{
	//				stip.Format("���Ļ���Ϊ����ʹ�õ��� %s ����������һ�����߶�û��Ч����ȷ��ʹ����?",userProp->szPropName);
	//			}
	//			else
	//			{
	//				stip.Format("��ȷ��Ҫʹ�õ��� %s ��?\n�Ὣ���ĸ���������!",userProp->szPropName);
	//			}
	//			
	//			if(AFCMessageBox(stip,"��ʾ",AFC_YESNO)==IDOK)*/
	//				//m_pMainWnd->PostMessage(WM_USER+155,(WPARAM)userProp);
	//				::PostMessage(m_hWnd,WM_USER+155,(WPARAM)userProp,0);
	//			////////////////////////////////////////////////////
	//			return;

	//		}
	//		////////////////////////////////////
	//	}
	//	
	//}
	//if (s==0)
	//{
	//	//С����
	//	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	//	
	//	if(IDOK == AFCMessageBox(fMsg.GetKeyVal("GameRoom","WhetherBuyToolNoScore","�����޸���������ߣ��Ƿ���?"),
	//		fMsg.GetKeyVal("GameRoom","BuyTool","���߹���"),AFC_YESNO))
	//	{
	//		m_pRightWnd->OnPropBuy();		
	//	}
	//	
	//}

	if (IDOK == dlg.DoModal())
	{
		if (1 == m_RoomOwner)
		{
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
			CString str = fMsg.GetKeyVal("GameRoom","MidifyRoomPW","��ȷ���޸�������");
			//if (IDOK == AFCMessageBox(str,"ϵͳ��ʾ",AFC_YESNO))
			if (IDYES == DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,"ϵͳ��ʾ",true,str))
			{
				SendVIPRoomSetInfo(dlg.m_szPassWord,_tcslen(dlg.m_szPassWord));
			}
		}
		else if (m_RoomOwner > 1)
		{
			SendVIPRoomSetInfo(dlg.m_szPassWord,_tcslen(dlg.m_szPassWord));
		}
	}
}
//���� modify by lxl 2010-12-28
void CGameRoomEx::OnBnClickedBt6()
{
	ShowFindUserWnd();
	////UpdateHoldItem();
	///*if(m_IGameFrame!=NULL)
	//{
	//	CLoveSendClass * m_pGameFrame = (CLoveSendClass*)m_IGameFrame;
	//	m_pGameFrame->UsePropItem((m_pGameFrame->m_curPropPage)*(m_pGameFrame->PEER_PAGE)+3);
	//}
	//else 
	//{
	//	m_pRightWnd->OnBnClickedPropBuy();
	//}*/

	//int propCout=Glb().userPropLibrary.GetCount();
	//int s=0;
	//for(int i=0;i<propCout;i++)
	//{
	//	_TAG_USERPROP *userProp=Glb().userPropLibrary.GetAt(i);
	//	if(userProp->attribAction & 0x80 )
	//	{//�����Ⱥ�С����
	//		::PostMessage(m_hWnd,WM_USER+156,(WPARAM)userProp,0);
	//		return;
	//	}
	//	
	//}
	//if (s==0)
	//{
	//	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	//	
	//	if(IDOK == AFCMessageBox(fMsg.GetKeyVal("GameRoom","WhetherBuyToolNoBigTool","�����޴����ȵ��ߣ��Ƿ���"),
	//		fMsg.GetKeyVal("GameRoom","BuyTool","���߹���"),AFC_YESNO))
	//	{
	//		m_pRightWnd->OnPropBuy();		
	//	}
	//}
}
//��������
void  CGameRoomEx::OnBnClickedBt7()
{
	//Ч��״̬
	//if ((m_dwCurrentUserID==0L)||(m_bLogonFinish==false)) return;
	//UserItemStruct * pUserItem=m_UserManage.FindOnLineUser(m_dwCurrentUserID);
	//if (pUserItem==NULL) return ;

	//��������
	if(m_pCallGMDlg.DoModal()==IDOK)
	{	
		MSG_GR_Call_GM s_callGM;
		s_callGM.bCallReason=0;
		s_callGM.szMsg[200]=0;
		if(m_pCallGMDlg.game_error)
			s_callGM.bCallReason|=0x1;
		if(m_pCallGMDlg.court)
			s_callGM.bCallReason|=0x2;
		if(m_pCallGMDlg.publicPoison)
			s_callGM.bCallReason|=0x4;
		if(m_pCallGMDlg.others)
			s_callGM.bCallReason|=0x8;
		s_callGM.uLength=m_pCallGMDlg.szCallGMmsg.GetLength();
		lstrcpy(s_callGM.szMsg,m_pCallGMDlg.szCallGMmsg);
		//TCHAR sz[20];
		//AfxMessageBox(ltoa(s_callGM.bCallReason,sz,10));
		//AfxMessageBox(m_pCallGMDlg.szCallGMmsg);
		//AfxMessageBox(s_callGM.szMsg);
		SendGameData(&s_callGM,sizeof(s_callGM),MDM_GR_MANAGE,ASS_GR_CALL_GM,0);

	}
	return ;
}
//Ǯ��	�����й���
void  CGameRoomEx::OnBnClickedBt8()
{
	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�����й���
	if (m_GameInfo.dwRoomRule & GRR_CONTEST)
	{
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"��ܰ��ʾ",false,"�Բ���, ����������ʹ�����й���");
		return ;
	}

	if (m_GameInfo.dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"��ܰ��ʾ",false,"�Բ���, ���鳡(��ѳ�)����ʹ�����й���");
		return ;
	}
	// end duanxiaohui


	////////////////////////////////////////////////////////////////////////////
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");


	if(GRR_EXPERCISE_ROOM & m_GameInfo.dwRoomRule)
	{
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,
			fMsg.GetKeyVal("GameRoom","TrainingNoGetCoin","�����ڴ���ѵ�����У����ܴ�Ǯ��"));
		return ;
	}

	////////////////////////////////////////////////////////////////////////////

	if (CanCloseNow() == false)
	{
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,
			fMsg.GetKeyVal("GameRoom","PlayingNoGetCoin","�����ڴ���ѵ�����У����ܴ�Ǯ��"));
		return ;
	}

	if (m_IGameFrame)
	{
		if (m_IGameFrame->GetGameIsStarting())
		{
			DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,
				fMsg.GetKeyVal("GameRoom","InGameNoGetCoin","������Ϸ�У����ܴ򿪷���Ǯ��"));
			return ;
		}
	}

	/// ��ʾ��ʾ�Ի����ֱ�ӷ��ʹ�������Ϣ������Ҫ��������
	if (m_Connect.GetSafeHwnd()==NULL) 
		m_Connect.Create(IDD_CONNECT,this);
	m_Connect.SetMessageText(fMsg.GetKeyVal("GameRoom","OpenningBankWait","���ڴ�Ǯ��,���Ժ�..."));
	m_Connect.GetDlgItem(IDC_QUIT)->EnableWindow(false);
	m_Connect.ShowWindow(SW_SHOW);
	SendData(MDM_GR_MONEY,ASS_GR_OPEN_WALLET,0);

}

// PengJiLin, 2011-4-13, ��ȡ��� ����
void  CGameRoomEx::OnBnClickedBt9()
{
    //��������
    if (m_pGetHelpMoneyDlg==NULL) 
    {
        try
        {
            m_pGetHelpMoneyDlg=new CGetHelpMoneyDlg(this);
            if (m_pGetHelpMoneyDlg==NULL) return ;
        }
        catch (...) { return; }
    }
    if (m_pGetHelpMoneyDlg->GetSafeHwnd()==NULL)
    {
        AfxSetResourceHandle(GetModuleHandle(NULL));
        m_pGetHelpMoneyDlg->Create(IDD_GETHELPMONEY,this);

    }

    CString s=CBcfFile::GetAppPath ();
    CBcfFile f( s + "bzgame.bcf");
    CString key="BZW";
    CString url;
    url=f.GetKeyVal(key,"GetHelpMoneyUrl",Glb().m_CenterServerPara.m_strHomeADDR);

    CString loginURL;
    loginURL = f.GetKeyVal("BZW", "UserLoginURL", "userlogin.aspx"); 

    CString surl;
    surl.Format("%s/app/%s?userid=%d&token=%s&url=%s",
        Glb().m_CenterServerPara.m_strWebRootADDR,loginURL,
        m_pLogonInfo->dwUserID,Glb().m_TML_SN,url);

    m_pGetHelpMoneyDlg->SetWebAddr(surl);

    m_pGetHelpMoneyDlg->CenterWindow(GetParent());
    m_pGetHelpMoneyDlg->ShowWindow(SW_SHOW);
    m_pGetHelpMoneyDlg->BringWindowToTop();
    m_pGetHelpMoneyDlg->SetFocus();
}

//
void  CGameRoomEx::OnBnClickedBt10()
{

}

LRESULT CGameRoomEx::OnShowLeft(WPARAM wparam,LPARAM lparam)
{
	GetMainRoom()->PostMessage(IDM_SHOW_LEFT,wparam,lparam);
	return 0;
}

LRESULT CGameRoomEx::OnTopTabSelected(WPARAM wparam,LPARAM lparam)
{
	TCHAR sz[100],szPath[MAX_PATH];
	lstrcpy(sz,m_pRoomInfo->szProcessName);
	sz[lstrlen(sz)-lstrlen(".ico")]='\0';
	wsprintf(szPath,"%s\\GameOption.bcf",sz);

	CBcfFile f(szPath);
	CString strDeskClass = f.GetKeyVal("DESK","CLASS","");
	if (IsQueueGameRoom()) strDeskClass = "queue";
	wchar_t* wszDeskClass = CA2W(strDeskClass.GetBuffer(8));
	int nVip = f.GetKeyVal("DESK", "IsVip", 0);
	bool bIsVip = (nVip != 0);	///< �Ƿ�ΪVIP����

	int deskFlat=1;
	if(strDeskClass != "")
		deskFlat=f.GetKeyVal("DESK","FLATLOGO",0);

	SetGameID(m_GameInfo.uNameID);
	

	gpGameRoomEx = this;

	CBaseDeskView::InitDesk(strDeskClass,deskFlat,m_pRoomInfo->pComRoomInfo.uNameID,bIsVip);

	

	return 0;
}

LRESULT CGameRoomEx::OnTopTabLostSelected(WPARAM wparam,LPARAM lparam)
{
	if (m_pBankVerifyWnd)
	{
		m_pBankVerifyWnd->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	if (m_pBankWndEx)
	{
		m_pBankWndEx->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	if (m_pBankWnd)
	{
		m_pBankWnd->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	if (m_pUserDetailsDlg)
	{
		m_pUserDetailsDlg->ShowWindow(SW_HIDE);
	}

	return 0;
}

LRESULT CGameRoomEx::OnGetRoomName(WPARAM wparam,LPARAM lparam)
{
	lstrcpy((TCHAR*)lparam,m_pRoomInfo->pComRoomInfo.szGameRoomName);
	return 0;
}

LRESULT CGameRoomEx::OnCloseGameClient(WPARAM wparam,LPARAM lparam)
{
	//m_IGameFrame = NULL;
	//������Ϸ��Ϣ   yjj 090225
	BZSoundPlay(this, "music/�ر���.mp3", 0);
	
	CString appPath = CBcfFile::GetAppPath ();
	if(m_byVideoUse != 0)  // PengJiLin, 2010-5-18, ���¼�����ù���
        m_AddRecordModule.SaveRecordFile(appPath,m_GameInfo.pMeUserInfo->GameUserInfo.szName,m_GameInfo.uNameID);
	return 0;
}


LRESULT CGameRoomEx::OnCutNetQuitRoom(WPARAM wparam,LPARAM lparam)
{
	if (NULL != m_pPersonBank)
	{
		delete m_pPersonBank;
		m_pPersonBank = NULL;
	}

	if (m_pBankVerifyWnd)
	{
		m_pBankVerifyWnd->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	if (m_pBankWndEx)
	{
		delete m_pBankWndEx;
		m_pBankWndEx = NULL;
	}

	if (m_pBankWnd)
	{
		m_pBankWnd->Close();
		PostMessage(WM_BANK_CLOSE,0,0);
	}

	GetMainRoom()->PostMessage(WM_CLOSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
	//CloseGameClient();
	return 0;
}

//��ҳURL�仯��⣬JianGuankun 2012.7.10
void CGameRoomEx::BeforeNavigate2Explorer(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	if(URL->vt!=VT_BSTR)
		return;
	CString surl=(char*)_bstr_t(URL->bstrVal);
	surl.MakeLower();

	if(surl.Left(6)=="bzw://")
	{
		CString str;
		str.Format("DClient:CGameRoomEx::BeforeNavigate2Explorer %s",surl);
		OutputDebugString(str);

		*Cancel=TRUE;
		if(surl.Find("contest/begin") == 6)
		{
			CString strIsApply = CMainRoomEx::GetUrlValue(surl,"isapply");
			if (strIsApply == "1")
			{
				m_DeskFrame.ShowWindow(SW_SHOW);
				m_WebView.ShowWindow(SW_HIDE);

				if (m_bEnterAutoQueue && IsQueueGameRoom())
				{
					JoinQueue();
				}

				return;
			}
			else
			{
				AfxMessageBox("�����ѿ�ʼ��������δ�������Զ��˳����䡣");
				GetMainRoom()->PostMessage(WM_CLOSE_ROOM,(WPARAM)m_pRoomInfo,(LPARAM)m_pRoomInfo->uTabIndex);
				return;
			}
		}
	}
}

void CGameRoomEx::NavigateErrorExplorer(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Frame, VARIANT* StatusCode, BOOL* Cancel)
{

}

int CGameRoomEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseRoom::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	if (m_Connect.GetSafeHwnd()==NULL) 
		m_Connect.Create(IDD_CONNECT,this);
	
	m_Connect.SetMessageText(fMsg.GetKeyVal("GameRoom","Connecting","��������,���Ժ�..."));
	m_Connect.ShowWindow(SW_SHOW);

	//{{Modified by zxd ��ȷװ����Դ������
/*	TCHAR szIcoName[MAX_PATH];
	int nLen = _tcslen(m_pRoomInfo->szProcessName);
	//�ж��Ƿ���EXE��ʽ������Ϸ
	if ((m_pRoomInfo->szProcessName[nLen-3] == TCHAR('e') || m_pRoomInfo->szProcessName[nLen-3] == TCHAR('E'))
		&&(m_pRoomInfo->szProcessName[nLen-2] == TCHAR('x') || m_pRoomInfo->szProcessName[nLen-2] == TCHAR('X'))
		&&(m_pRoomInfo->szProcessName[nLen-1] == TCHAR('e') || m_pRoomInfo->szProcessName[nLen-1] == TCHAR('E'))
		)
	{
		_tcscpy(szIcoName, CBcfFile::GetAppPath());
		_tcscat(szIcoName, m_pRoomInfo->szProcessName);
		// ������ǰ��һ�θĳ�Ŀ¼�����ټ����ļ���
		nLen = _tcslen(szIcoName);
		szIcoName[nLen-4] = TCHAR('\\');
		szIcoName[nLen-3] = TCHAR('\0');
		_tcscat(szIcoName, m_pRoomInfo->szProcessName);	//�ٸ����ļ���
		nLen = _tcslen(szIcoName);
		szIcoName[nLen-3] =TCHAR('i');
		szIcoName[nLen-2] =TCHAR('c');
		szIcoName[nLen-1] =TCHAR('o');

	}
	//��ԭ��ico��ʽ������Ϸ
	else
	{
		_tcscpy(szIcoName, CBcfFile::GetAppPath());
		_tcscat(szIcoName, m_pRoomInfo->szProcessName);
	}*/
	//Modified by zxd ��ȷװ����Դ������}}
	//������Ϸ����
	//CString icopath=szIcoName;
	//int p=icopath.ReverseFind('\\');
	//icopath=icopath.Left(p+1);
	//CString apppath=CBcfFile::GetAppPath(true);
	//::SetCurrentDirectory(icopath);
	
//	m_hGameInstance=::LoadLibrary(szIcoName); ///�������ټ�����ϷDLL  Y.C.YAN 2010 6 28
	//m_hGameInstance=::LoadLibrary("E:\\������Aƽ̨\\����\\����\\�ͻ���\\release\\10003300\\10003300.ico");
//	DWORD error = GetLastError();
	//Modified by zxd}}

//	::SetCurrentDirectory(apppath);
//	if (m_hGameInstance==NULL) 
//		throw fMsg.GetKeyVal("GameRoom","NotFindGame","���ִ���!�Ҳ�����Ϸ!"); ///�������ټ�����ϷDLL  Y.C.YAN 2010 6 28

	//Ч����Ϸ����

	//��ȡ�ȼ�����ָ��
//	m_GameInfo.pOrderName=NULL;

	//if(m_pRoomInfo->ComRoomInfo.uComType==TY_NORMAL_GAME)
	//{
//	SetGameInitClass * pGetOrderFunc=(SetGameInitClass *)GetProcAddress(m_hGameInstance,TEXT("SetGameInitClass")); ///�������ټ�����ϷDLL  Y.C.YAN 2010 6 28
//	if (pGetOrderFunc!=NULL)
//		m_GameInfo.pOrderName=pGetOrderFunc(m_pRoomInfo->pComRoomInfo.uNameID);
//	else
//		m_GameInfo.pOrderName=GetNormalOrderName;
	
	ReadLevelData(); /// ��ȡ���ֵȼ����ݣ�GetGameOrderName����ʹ��
	m_GameInfo.pOrderName = GetGameOrderName;
	if (m_GameInfo.pOrderName==NULL)
	{
		switch (m_pRoomInfo->pComRoomInfo.uComType)
		{
		case TY_MATCH_GAME: 
			{
				m_GameInfo.pOrderName=GetMatchOrderName; 
				//m_rBt5.EnableWindow(false);
				break; 
			}
		case TY_MONEY_GAME: { m_GameInfo.pOrderName=GetMoneyOrderName; break; }
		default: { m_GameInfo.pOrderName=GetNormalOrderName; }
		}
	}
	m_DeskFrame.pOrderName=m_GameInfo.pOrderName;

	//��ȡ���ò��� 
	TCHAR szRoomSetReg[100];
	wsprintf(szRoomSetReg,TEXT("Room%ld"),m_pRoomInfo->pComRoomInfo.uNameID);
	m_bLimitCut=AfxGetApp()->GetProfileInt(szRoomSetReg,TEXT("LimitCut"),FALSE);
	m_bLimitPoint=AfxGetApp()->GetProfileInt(szRoomSetReg,TEXT("LimitPoint"),FALSE);
	m_bCutPercent=AfxGetApp()->GetProfileInt(szRoomSetReg,TEXT("Cut"),10);
	m_dwLowPoint=AfxGetApp()->GetProfileInt(szRoomSetReg,TEXT("Point1"),-100000L);
	m_dwHighPoint=AfxGetApp()->GetProfileInt(szRoomSetReg,TEXT("Point2"),100000L);
	m_dwLowMoney=AfxGetApp()->GetProfileInt(szRoomSetReg,TEXT("Money1"),-100000L);
	m_dwHighMoney=AfxGetApp()->GetProfileInt(szRoomSetReg,TEXT("Money2"),100000L);
	m_bSameIP=AfxGetApp()->GetProfileInt(TEXT("RoomInfo"),TEXT("CanSameIP"),FALSE);//JMod-Ĭ��Ϊѡ��-20090506
	m_bSameIPNumber=AfxGetApp()->GetProfileInt(TEXT("RoomInfo"),TEXT("SameIPNumber"),FALSE);
	m_bInvaliMode=AfxGetApp()->GetProfileInt(TEXT("RoomInfo"),TEXT("Mode"),INVALI_ALL);
	m_bAutoSit=(AfxGetApp()->GetProfileInt(TEXT("RoomInfo"),TEXT("AutoSitDesk"),TRUE)>0);
	m_bSaveTalk=AfxGetApp()->GetProfileInt(TEXT("RoomInfo"),TEXT("CanSaveTalk"),TRUE);
	m_bShowMessage=AfxGetApp()->GetProfileInt(TEXT("RoomInfo"),TEXT("ShowMessage"),TRUE);

	//�״ΰ�װ��������ʾ�û�������Ĭ�����ø��������ļ�����
	//JianGuankun 2012.3.29
	bool bShowUserDefault = fMsg.GetKeyVal("GameRoom","ShowUserComeInDefault",TRUE);
	m_bShowUser=AfxGetApp()->GetProfileInt(TEXT("RoomInfo"),TEXT("ShowUser"),bShowUserDefault);

	//��ʼ���ؼ�
	//ȡ��������

	CGameImage		desk,begin;
	TCHAR sz[100],szPath[MAX_PATH];
	lstrcpy(sz,m_pRoomInfo->szProcessName);
	sz[lstrlen(sz)-lstrlen(".ico")]='\0';
	wsprintf(szPath,"%s\\GameOption.bcf",sz);

	CBcfFile f(szPath);
	CString strDeskClass = f.GetKeyVal("DESK","CLASS","");
	if (IsQueueGameRoom()) strDeskClass = "queue";
	wchar_t* wszDeskClass = CA2W(strDeskClass.GetBuffer(8));
	int nVip = f.GetKeyVal("DESK", "IsVip", 0);
	bool bIsVip = (nVip != 0);	///< �Ƿ�ΪVIP����

	int		deskFlat=1;
	if(strDeskClass != "")
		deskFlat=f.GetKeyVal("DESK","FLATLOGO",0);

	

	int r,g,b;
	COLORREF	SitDeskFrameLeftTop,SitDeskRightBottom;
	r=f.GetKeyVal("DESK","SitDeskFrameLeftTopColorR",0);
	g=f.GetKeyVal("DESK","SitDeskFrameLeftTopColorG",0);
	b=f.GetKeyVal("DESK","SitDeskFrameLeftTopColorB",255);
	SitDeskFrameLeftTop=RGB(r,g,b);
	r=f.GetKeyVal("DESK","SitDeskFrameRightBottomColorR",0);
	g=f.GetKeyVal("DESK","SitDeskFrameRightBottomColorG",0);
	b=f.GetKeyVal("DESK","SitDeskFrameRightBottomColorB",128);
	SitDeskRightBottom=RGB(r,g,b);

	CBaseDeskView::InitDesk(strDeskClass,deskFlat,m_pRoomInfo->pComRoomInfo.uNameID, bIsVip);

	//�ټ��� add by wlr 20090713
	//if ( HUNDRED_PEOPLE_DESK_GAME_NAME == m_pRoomInfo->pComRoomInfo.uNameID		//�ټ���
	//	||SUOHA_PEOPLE_DESK_GAME_NAME == m_pRoomInfo->pComRoomInfo.uNameID			//�ٱ����
	//	||NIUNIU_PEOPLE_DESK_GAME_NAME == m_pRoomInfo->pComRoomInfo.uNameID		//�ٱ�ţţ
	//	||ERZHANGPAI_PEOPLE_DESK_GAME_NAME == m_pRoomInfo->pComRoomInfo.uNameID )	//�ٱ������)// add by ramon 09.05 �ټ���
	char cKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "SpecialRule.bcf");
	sprintf(cKey, "%d", m_pRoomInfo->pComRoomInfo.uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), cKey, 0);

	if (iResult)
	{

		wchar_t wcsDesk[MAX_PATH];
		wchar_t wcsBegin[MAX_PATH];
		wsprintfW(wcsDesk, L".\\%d\\image\\desk\\desk.bmp", m_pRoomInfo->pComRoomInfo.uNameID);
		wsprintfW(wcsBegin, L".\\%d\\image\\Desk\\playing.bmp", m_pRoomInfo->pComRoomInfo.uNameID);
		m_DeskFrame.InitDeskFramePng(m_pRoomInfo->pComRoomInfo.uDeskCount,m_pRoomInfo->pComRoomInfo.uDeskPeople,wcsDesk,NULL,NULL,wcsBegin,NULL,BJL_DESK_FACE,
			m_pRoomInfo->pComRoomInfo.uComType,&m_GameInfo,this,strDeskClass,deskFlat,SitDeskFrameLeftTop,SitDeskRightBottom);
	}//end of �ټ��� add by wlr 20090713
	else
	{
		// {{Added by zxd 20091126 ���Ϊ��png��ʽͼƬ
		wchar_t wcsDesk[MAX_PATH];
		wchar_t wcsBegin[MAX_PATH];
		wchar_t wcsLock[MAX_PATH];
		wchar_t wcsAgree[MAX_PATH];


		if (bIsVip)	/// �����VIP���䣬����VIP���������ͼƬ
		{
			wsprintfW(wcsDesk, L".\\image\\client\\alluse\\%s\\vip-desk.png",wszDeskClass);
			wsprintfW(wcsBegin, L".\\image\\client\\alluse\\%s\\vip-deskplaying.png",wszDeskClass);
		}
		else
		{
			wsprintfW(wcsDesk, L".\\image\\client\\alluse\\%s\\desk%d.png",wszDeskClass,m_pRoomInfo->pComRoomInfo.uDeskPeople);
			
			/// ��ȡ��Ϸ�����ļ��е���Ϸ������ͼƬ�ļ���
			CString strFileName = f.GetKeyVal("DESK","ImgPlayingDesk","");

			if (strFileName!="")///< ��ʾ��ȡ�����ļ���
			{
				wsprintfW(wcsBegin, L".\\image\\client\\alluse\\%s\\%s",wszDeskClass,CA2W(strFileName.GetBuffer()));
			}
			else
			{
				/// �齫�����Ϸ������һ��ͼƬ
				if (m_pRoomInfo->pComRoomInfo.uKindID == 2)
				{
					wsprintfW(wcsBegin, L".\\image\\client\\alluse\\%s\\majiang%dplaying.png",wszDeskClass,m_pRoomInfo->pComRoomInfo.uDeskPeople);
				}
				else if (m_pRoomInfo->pComRoomInfo.uKindID == 1)
				{
					wsprintfW(wcsBegin, L".\\image\\client\\alluse\\%s\\desk%dplaying.png",wszDeskClass,m_pRoomInfo->pComRoomInfo.uDeskPeople);
				}
				else
				{
					wsprintfW(wcsBegin, L".\\image\\client\\alluse\\%s\\desk%dplaying.png",wszDeskClass,m_pRoomInfo->pComRoomInfo.uDeskPeople);
				}
			}
		}
		wsprintfW(wcsLock, L".\\image\\client\\alluse\\lock.bmp");
		wsprintfW(wcsAgree, L".\\image\\client\\alluse\\agree.png");
        if(IsQueueGameRoom())    // �Ŷӻ�����ֻ��ʾһ������
		{
			//wsprintfW(wcsDesk, L".\\%d\\image\\Queuedesk.bmp", m_pRoomInfo->pComRoomInfo.uNameID);
	  //  	wsprintfW(wcsBegin, L".\\%d\\image\\Queueplaying.bmp", m_pRoomInfo->pComRoomInfo.uNameID);
		
		//wsprintfW(wcsDesk, L".\\image\\client\\alluse\\2d\\Queuedesk%d.bmp", m_pRoomInfo->pComRoomInfo.uDeskPeople);
	  	//wsprintfW(wcsBegin, L".\\image\\client\\alluse\\2d\\Queuedesk%dplaying.bmp", m_pRoomInfo->pComRoomInfo.uDeskPeople);
			//m_DeskFrame.InitDeskFramePng(1,m_pRoomInfo->pComRoomInfo.uDeskPeople,wcsDesk,NULL,NULL,wcsBegin,NULL,BJL_DESK_FACE,
			//    m_pRoomInfo->pComRoomInfo.uComType,&m_GameInfo,strDeskClass,deskFlat,SitDeskFrameLeftTop,SitDeskRightBottom);

			//wsprintfW(wcsDesk, L".\\image\\client\\alluse\\%s\\desk%d.bmp", "queue",m_pRoomInfo->pComRoomInfo.uDeskPeople);
			//wsprintfW(wcsBegin, L".\\image\\client\\alluse\\%s\\desk%dplaying.bmp","queue",m_pRoomInfo->pComRoomInfo.uDeskPeople);

		m_DeskFrame.InitDeskFramePng(1,m_pRoomInfo->pComRoomInfo.uDeskPeople,wcsDesk,wcsLock,NULL,wcsBegin,wcsAgree,YH_DESK_FACE,
			m_pRoomInfo->pComRoomInfo.uComType,&m_GameInfo,this,strDeskClass,deskFlat,SitDeskFrameLeftTop,SitDeskRightBottom, bIsVip);
		}
		else
		m_DeskFrame.InitDeskFramePng(m_pRoomInfo->pComRoomInfo.uDeskCount,m_pRoomInfo->pComRoomInfo.uDeskPeople,wcsDesk,wcsLock,NULL,wcsBegin,wcsAgree,YH_DESK_FACE,
			m_pRoomInfo->pComRoomInfo.uComType,&m_GameInfo,this,strDeskClass,deskFlat,SitDeskFrameLeftTop,SitDeskRightBottom, bIsVip);
		// Added by zxd 20091126 ���Ϊ��png��ʽͼƬ}}
	}

	//����������ʾ
	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);

	//�����ؼ�
	AfxSetResourceHandle(GetModuleHandle(NULL));
	m_DeskFrame.CreateDeskFrame(this,10);


	if(m_pRightWnd!=NULL)
	{
		m_pUserList = &(m_pRightWnd->m_UserList);
		m_pUserList->InitListCtrl(m_GameInfo.pOrderName,m_pRoomInfo->pComRoomInfo.uComType,&m_GameInfo);
		m_pUserList->ShowWindow(SW_HIDE);

		m_pInputMessage=m_pRightWnd->m_ChatControl.GetTalkInput();
		m_pInputMessage->LimitText(0);
		m_pUserList->SetMSGReceiveWnd(this);
	}

	//���ÿؼ�
	CString str = fMsg.GetKeyVal("GameRoom","WelcomeTo","��ӭ����%s - %s");
	wsprintf(m_title,str.GetBuffer(),m_pRoomInfo->pComRoomInfo.szGameRoomName,
		m_pLogonInfo->nickName);

	//this->PostMessage(WM_RELOAD_TALKINI);
	//������ʾ
	
	if (m_pRoomInfo->pRoomItem->m_bIsContest)
	{
		//�������˳�����һ�����˵����ٵķ��䣬�����û�˾���һ��û�˵ķ����ȥ
		int _index = FindNewRoom();
		if (_index == -1) _index = FindMinPeopleRoom(0);
		if (_index == -1) _index = FindMinPeopleRoom(-1);
		if (_index != -1)  EnterRoom(_index);
	}
	else
	{
		if (m_TCPSocket != NULL && m_TCPSocket->Connect(m_pRoomInfo->pComRoomInfo.szServiceIP,m_pRoomInfo->pComRoomInfo.uServicePort)==false)
		{
			m_Connect.DestroyWindow();
			
			//AFCMessageBox(fMsg.GetKeyVal("GameRoom","ConnectError","���ӳ���"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","ConnectError","���ӳ���"));
			return TRUE;
		}
	}
	//�����
	//m_DlgGetPoint.Create(IDD_GETPOINT, this);

	///�����Ŷӻ��Ի���
	//if (m_pQueueInRoomDlg != NULL && IsQueueGameRoom())
	//{
	//	if (m_pQueueInRoomDlg->GetSafeHwnd() == NULL)
	//	{
	//		CRect rect,ClientRect;
	//		m_DeskFrame.GetClientRect(&ClientRect);
	//		int iClientWidth = ClientRect.Width();
	//		int iClientHeight = ClientRect.Height();
	//		m_pQueueInRoomDlg->GetClientRect(&rect);
	//		int iWidth = 264;
	//		int iHeight = 150;

	//		m_pQueueInRoomDlg->Create(IDD_QUEUE_ROOM, this);
	//		m_pQueueInRoomDlg->SetMessageText(_T("���ŶӲ�����Ϸ��"));
	//		m_pQueueInRoomDlg->MoveWindow(ClientRect.Width()-iWidth,ClientRect.Height()-iHeight,iWidth,iHeight);
	//		m_pQueueInRoomDlg->ShowWindow(SW_HIDE);
	//	}
	//}

	{
		CString s=CBcfFile::GetAppPath ();/////����·��
		CString strSkin = m_skinmgr.GetSkinBcfFileName();
		CBcfFile f( s + strSkin);
		TCHAR path[MAX_PATH];
		CString skinfolder;
		skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());
		wsprintf(path,"%sDialog\\scroll.bmp",skinfolder);
		HBITMAP hbmScorll = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
		SkinSB_Uninit(m_DeskFrame.GetSafeHwnd());
		SkinSB_Init(m_DeskFrame.GetSafeHwnd(), hbmScorll);
	}

	// �����û���ϸ�������
	m_pUserDetailsDlg = new CUserDetailsDlg(this);
	m_pUserDetailsDlg->Create(IDD_USER_DETAILS, this);
	return 0;
}

///< ��ȡ�ȼ���Ϣ��GetGameOrderNameʹ��
void CGameRoomEx::ReadLevelData()
{
	CString strKey;
	CString strConfig = CBcfFile::GetAppPath();
	strConfig += m_pRoomInfo->szProcessName;
	strConfig = strConfig.Left(strConfig.ReverseFind('.'));
	strConfig += _T("\\GameOption.bcf");
	CBcfFile fLevel(strConfig);

	for(int i=1; i<21; i++)
	{
		strKey.Format(_T("Order%d"),i);
		g_PointArray[i] = fLevel.GetKeyVal(_T("ScoreOrderName"),strKey,0);
		strKey = fLevel.GetKeyMemo(_T("ScoreOrderName"),strKey,_T("")); /// �����strKey��ֵֻ�ǽ��ô˱���
		g_LevelNameArray.insert(pair<int,string>(i,strKey.GetBuffer()));
	}
}

//����Ƿ�ټ��ַ���
//JianGuankun 2012.3.29
bool CGameRoomEx::IsBJLRoom()
{
	char cKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "SpecialRule.bcf");
	_stprintf(cKey, "%d", m_GameInfo.uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), cKey, 0);
	return (iResult!=0);
}

//���뵽�ų���
void CGameRoomEx::JoinQueue()
{
	//if (m_pQueueInRoomDlg != NULL)
	//{
	//	m_pQueueInRoomDlg->ShowWindow(SW_SHOW);
	//	m_pQueueInRoomDlg->OnQueue();
	//}

	//��Ҫ�����䴦����ƽ�������������������������Ϸ���ٴ��Ŷ�

	//if (m_bChangeRooming) return;
	//if (m_bChangeRoom)
	//{
	//	m_bChangeRoom = false;
	//	if (ChangeRoom()) return;
	//}


	if (!IsEnoughMoney())
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString strMessage;
		TCHAR szNum[128]; 
		GetNumString(szNum, m_GameInfo.uLessPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
		int r = DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,true,
			fMsg.GetKeyVal("GameRoom","LackCoins","����Ϸ��������Ҫ��%s�Ľ��,���Ľ�Ҳ���,�쵽Ǯ��ȡ��Ұ�!"),
			szNum);

		if (r == IDOK) NoEnoughMoneyEvent();

		return;
	}

	if (m_bChangeRoom)
	{
		m_bChangeRoom = false;
		int _index = GetChangeRoomIndex();
		if (_index != -1 && _index != m_iContestRoomIndex)
		{
			RelaodRoom(_index);
			return;
		}
	}

	for (int i = 0; i < m_GameInfo.uDeskPeople; i++)
		m_DeskFrame.SetUserItem(m_iQueueNo, i, NULL);

	if (m_iQueueStation < 0 || m_iQueueStation >= m_GameInfo.uDeskPeople - 1)
		m_iQueueStation = 0;
	m_DeskFrame.SetUserItem(m_iQueueNo,m_iQueueStation,m_GameInfo.pMeUserInfo);

	m_DeskFrame.showQueueBtn(true);
	m_DeskFrame.SetQueueTime(m_iQueueTime);



	SendData(MDM_GR_USER_ACTION,ASS_GR_JOIN_QUEUE,0);
	SetTimer(TIME_QUEUEROOM_TIMEOUT, m_iQueueTimeOut * 1000, NULL);
}

///< ��ȡ��Ϸ����Ļ��ּ���
///< @dwPoint��һ���
///< ��һ�������
TCHAR * CGameRoomEx::GetGameOrderName(__int64 dwPoint,UINT uGameNameID)
{
	int low = 1;
	int high = 19;
	int mid;
	UINT point;
	TCHAR OrderName[MAX_PATH] = {0};

	dwPoint = dwPoint<0?0:dwPoint; ///����С����Ҳ����ͼ���

	while(low <= high)
	{
		mid = low + (high-low)/2;

		if (dwPoint<g_PointArray[mid])
		{
			high = mid-1;
		}
		else
		{
			low = mid+1;
		}
	}

    // PengJiLin, 2010-8-2, ����Խ�紦��
    int iFirst = 1;         // Key �� 1 ��ʼ
    int iLast = g_LevelNameArray.size();
    int iIndex = low-1;
    if(iIndex < iFirst)
    {
        iIndex = iFirst;
    }
    else if(iIndex > iLast)
    {
        iIndex = iLast;
    }

	if (g_LevelNameArray.size() > 0)
	{
		map<int,string>::iterator ite = g_LevelNameArray.find(iIndex);	
		return (TCHAR*)(*ite).second.c_str();
	}
	
	return " ";
}

BOOL CGameRoomEx::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	cs.style &= ~WS_VISIBLE;
	cs.style |= WS_CLIPSIBLINGS;
	cs.style |= WS_CLIPCHILDREN;

	return CBaseRoom::PreCreateWindow(cs);
}
//����
LRESULT CGameRoomEx::OnOpenIE(WPARAM wparam, LPARAM lparam)
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	CString key="BZW";
	CString url;
	TCHAR szUrl[20];
	wsprintf(szUrl,"hallurl%d",(int)wparam);
	url=f.GetKeyVal(key,szUrl,Glb().m_CenterServerPara.m_strHomeADDR);
	
	CString loginURL;//JMod-�û���¼ҳ�� -20090506
	loginURL = f.GetKeyVal("BZW", "UserLoginURL", "userlogin.aspx"); 

	CString surl;
	surl.Format("%s/app/%s?userid=%d&token=%s&url=%s",
		Glb().m_CenterServerPara.m_strWebRootADDR,loginURL,m_pLogonInfo->dwUserID,Glb().m_TML_SN,url);
	ShellExecute(NULL,"open",surl,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
	return true;
}

//����ɫ
COLORREF CGameRoomEx::GetDuduRGB()
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("BoardCast");
	//	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder = f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder = f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());

	int r,g,b;
	r=f.GetKeyVal(key,"dudurgbr",255);
	g=f.GetKeyVal(key,"dudurgbg",0);
	b=f.GetKeyVal(key,"dudurgbb",0);
	return RGB(r,g,b);
}


//�������Ϣ������
bool CGameRoomEx::HandlePointMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	return true;
}
//������ҵ���
bool CGameRoomEx::OnPropMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch(pNetHead->bAssistantID)
	{
	case ASS_PROP_USEPROP:		//ʹ�õ���
		return OnUserUsePropResult(pNetHead,pNetData,uDataSize,pClientSocket);
		break;
	case ASS_PROP_VIP_CHECKTIME:	//vipʱ����	
		return OnVipCheckMsg(pNetHead,pNetData,uDataSize,pClientSocket);
		break;
	case ASS_PROP_SMALL_BOARDCASE:
		return OnUseSmallBoardcast(pNetHead,pNetData,uDataSize,pClientSocket);
		break;
	case ASS_PROP_KICKUSER://����
		OnKickUserMsg(pNetHead,pNetData,uDataSize,pClientSocket);
        break;  // PengJiLin, 2010-9-13
    case ASS_PROP_NEW_KICKUSER: // PengJiLin, 2010-9-13, ʹ�����˿����˹���
        OnNewKickUserMsg(pNetHead,pNetData,uDataSize,pClientSocket);
        break;
	}
	return true;
}
bool CGameRoomEx::OnUseSmallBoardcast(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	_TAG_BOARDCAST *boardCast=(_TAG_BOARDCAST*)pNetData;
	if(pNetHead->bHandleCode!=DTK_GR_PROP_USE_SUCCEED && boardCast->dwUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","UseToolFailed","�Բ���ʹ�����ȵ���ʧ�ܣ�"),fMsg.GetKeyVal("GameRoom","Tip","��ʾ"));
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,fMsg.GetKeyVal("GameRoom","Tip","��ʾ"),false,fMsg.GetKeyVal("GameRoom","UseToolFailed","�Բ���ʹ�����ȵ���ʧ�ܣ�"));
		return true;
	}	
	//wushuqun 2009.9.5
	if(NULL == m_GameInfo.pMeUserInfo)
	{
		return true;
	}

    //��С������Ϣ��ʾ����
    // PengJiLin, 2010-10-15, ����Ҫʹ�����Ƚṹ���ݵ��ߵ������������Ƚ���Ϣ��ʾ����
    Dudu(boardCast,BRD_MSG_SMALL);
	
	// add xqm 2010-11-19
	BZSoundPlay(this, "image\\prop\\Prop4A.mp3", 0);

	if(boardCast->dwUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
	{//���ٵ�������
		int propCount=Glb().userPropLibrary.GetCount();
		_TAG_USERPROP *userProp=NULL;
		for(int i=0;i<propCount;i++)
		{
			userProp=Glb().userPropLibrary.GetAt(i);
			if(userProp->nPropID==boardCast->nPropID)
			{
				userProp->nHoldCount-=1;
                if(userProp->nHoldCount < 0)userProp->nHoldCount = 0;
				//if(userProp->nHoldCount<1)    // PengJiLin, 2010-10-14, �µĵ���ϵͳ��������Ϊ0������
				//{
				//	Glb().userPropLibrary.RemoveAt(i);
				//	delete userProp;
				//	userProp=NULL;
				//}
				m_pRoomInfo->pRoomInfoWnd->OnSetPropImage();
				if(NULL != m_IGameFrame )
                {
                    // PengJiLin, 2010-10-15, С���ȵ���������Ϸ�˲���ʵʱ���£�
                    // ʹ�� boardCast->iLength �������������ݳ�ȥ
                    boardCast->iLength = userProp->nHoldCount;
					m_IGameFrame->OnSocketReadEvent(pNetHead,pNetData,uDataSize,pClientSocket);
                }

				break;
			}
		}
	}
	
	return true;
}
bool CGameRoomEx::OnUserUsePropResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if(pNetHead->bHandleCode!=DTK_GR_PROP_USE_SUCCEED)
		return true;
	if(sizeof(_TAG_USINGPROP)!= uDataSize)
		return false;
	_TAG_USINGPROP * usingProp = (_TAG_USINGPROP *)pNetData;
	UserItemStruct *pUserInfo =m_UserManage.FindOnLineUser(usingProp->dwTargetUserID);
	UserItemStruct *pUserInfoUsed =m_UserManage.FindOnLineUser(usingProp->dwUserID);
	if(pUserInfo == NULL)
		return true;
	//ʹ���ߵ��ǳ�
	//wushuqun 2009.6.30
	//ʹ�õ��ߵ�����������ǳƱ�ʾ 
	//CString name1=pUserInfoUsed->GameUserInfo.szName;

	CString name1=pUserInfoUsed->GameUserInfo.nickName;
	//��ʹ���ߵ��ǳ�
	//CString name2=pUserInfo->GameUserInfo.szName;
	CString name2=pUserInfo->GameUserInfo.nickName;

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;

	//wushuqun 2009.9.5
	if(NULL == m_GameInfo.pMeUserInfo)
	{
		return true;
	}
	if(usingProp->dwUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
		name1 = fMsg.GetKeyVal("GameRoom","You","��");
	if(usingProp->dwTargetUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
		name2 = fMsg.GetKeyVal("GameRoom","You","��");
	CString name3=name2;
	if(usingProp->dwUserID==usingProp->dwTargetUserID)
		name2 = fMsg.GetKeyVal("GameRoom","Self","�Լ�");

	int attribAction=usingProp->nPropActionAttrib;
	int attribValue=usingProp->nPropValueAttrib;

	CString stip;
	str = fMsg.GetKeyVal("GameRoom","UseTool","%s �� %s ʹ���� %s ����");
	stip.Format(str,name1,name2,usingProp->szPropName);
	m_pRightWnd->m_MessageHandle.InsertSystemMessageM(stip.GetBuffer());

	if(m_hWnd)
		Invalidate(FALSE);

	//˫������
	if(attribAction & 0x4)
	{
		pUserInfo->GameUserInfo.iDoublePointTime=usingProp->iDoubleTime;
		if(usingProp->dwTargetUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
		{
			time_t t=(time_t)usingProp->iDoubleTime;
			CTime expiresTime(t);
			str = fMsg.GetKeyVal("GameRoom","UseSocreFunction","����˫�����ֹ��ܿ���ʹ�õ� %s��");
			stip.Format(str,expiresTime.Format("%Y-%m-%d %H:%M:%S"));
			m_pRightWnd->m_MessageHandle.InsertSystemMessageM(stip.GetBuffer());
		}
	}
	if(attribAction & 0x8)
	{
		pUserInfo->GameUserInfo.iProtectTime=usingProp->iProtectTime;
		if(usingProp->dwTargetUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
		{
			time_t t=(time_t)usingProp->iProtectTime;
			CTime expiresTime(t);
			str = fMsg.GetKeyVal("GameRoom","UseCoverFunction","���Ļ��������(���˲��ۻ���)����ʹ�õ� %s��");
			stip.Format(str,expiresTime.Format("%Y-%m-%d %H:%M:%S"));
			m_pRightWnd->m_MessageHandle.InsertSystemMessageM(stip.GetBuffer());
		}
	}

	if(attribAction & 0x10)
	{
		int fasciChange=(attribValue & 0xFF0000)>>16;
		str = fMsg.GetKeyVal("GameRoom","AddCharm","%s ������ֵ���� %d ��");
		stip.Format(str,name3,fasciChange);
		if(attribAction & 0x20)
		{
			str = fMsg.GetKeyVal("GameRoom","ReduceCharm","%s ������ֵ���� %d ��");
			stip.Format(str,name3,fasciChange);
			fasciChange=-fasciChange;
		}
		pUserInfo->GameUserInfo.dwFascination+=fasciChange;
		if(pUserInfo->GameUserInfo.dwUserID==m_pLogonInfo->dwUserID)
			m_pLogonInfo->dwFascination+=fasciChange;
		m_pRightWnd->m_MessageHandle.InsertSystemMessageM(stip.GetBuffer());
	}
	if(attribAction & 0x2)
	{
		
		if(usingProp->dwTargetUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
		{
			if(m_GameInfo.pMeUserInfo->GameUserInfo.dwPoint<0)
				m_GameInfo.pMeUserInfo->GameUserInfo.dwPoint=0;
		}
	}

	//���µ�������
	if(usingProp->dwUserID==m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
	{
		int propCount=Glb().userPropLibrary.GetCount();
		_TAG_USERPROP *userProp=NULL;
		for(int i=0;i<propCount;i++)
		{
			userProp=Glb().userPropLibrary.GetAt(i);
			if(userProp->nPropID==usingProp->nPropID)
			{
				userProp->nHoldCount--;
                if(userProp->nHoldCount < 0)userProp->nHoldCount = 0;
				//if(userProp->nHoldCount<1)    // PengJiLin, 2010-10-14, �µĵ���ϵͳ��������Ϊ0������
				//	Glb().userPropLibrary.RemoveAt(i);
				break;
			}
		}
	}

	//��������б�
	m_pUserList->UpdateGameUser(pUserInfo);
	//���Լ�ͬ��
	if(pUserInfo->GameUserInfo.bDeskNO == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO
		&& m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO != 255)
	{
		if(NULL != m_IGameFrame )
			m_IGameFrame->OnSocketReadEvent(pNetHead,pNetData,uDataSize,pClientSocket);
	}
	m_pRoomInfo->pRoomInfoWnd->OnSetPropImage();

	return true;

}

bool CGameRoomEx::OnFriendManage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if(uDataSize!=sizeof(tagIM_MSG))
		return false;
	tagIM_MSG * pIM = (tagIM_MSG *)pNetData;
	//tagIM_MSG *pIM = reinterpret_cast<tagIM_MSG>(pNetData);//������һ���Ƚ���һ����??
	UserItemStruct *pUser = m_UserManage.FindOnLineUser(pIM->dwSendUserID);
	if(pUser == NULL)//����Ѿ��뿪��������
	{
		return true;
	}

	switch(pNetHead->bAssistantID)//�����ʹ���
	{

	}
	return true;
}
//������Ϣ
bool CGameRoomEx::OnQueueMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	//�Ŷӽ��
	if(uDataSize!=sizeof(MSG_GR_Queue))
		return false;

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	//��������
	CString strMessage;
	strMessage.Format(fMsg.GetKeyVal("GameRoom","SystemRecvQueueRequest","ϵͳ�Ѿ����������Ŷ�����!"));
	m_pRightWnd->m_MessageHandle.InsertSystemMessage(strMessage.GetBuffer());
	m_bAddQueue = true;
	//SetCursor(LoadCursor(NULL,IDC_WAIT));
	SetTimer(ID_TIME_ADD_SEQUE,ADD_SEQUE_TIMER,NULL);
	return true;
}
//vip���
bool CGameRoomEx::OnVipCheckMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if(uDataSize!=sizeof(_TAG_PROP_TIME))
		return false;
	_TAG_PROP_TIME * pData = (_TAG_PROP_TIME *)pNetData;


	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
	int curtime=atoi(stime);                        //ת��Ϊint
	if(m_GameInfo.pMeUserInfo->GameUserInfo.iVipTime<curtime)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString strMessage;
		strMessage.Format(fMsg.GetKeyVal("GameRoom","VIPExpired","����VIP����ѵ��ڣ�"));
		m_pRightWnd->m_MessageHandle.InsertSystemMessage(strMessage.GetBuffer());

	}

	if(m_IGameFrame != NULL)
		m_IGameFrame->OnSocketReadEvent(pNetHead,pNetData,uDataSize,pClientSocket);

	return true;
}
//vip����
//bool CGameRoomEx::OnVipOperationMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
//{
//	ASSERT(pNetHead->bMainID == MDM_GR_VIP_MSG);
//	switch(pNetHead->bAssistantID)
//	{
//	case ASS_S_VIP_CHECK_TIME:	//vipʱ����	
//		return OnVipCheckMsg(pNetHead,pNetData,uDataSize,pClientSocket);
//		break;
//	case ASS_S_USE_BROADCAST://�㲥
//		OnBroadcastMsg(pNetHead,pNetData,uDataSize,pClientSocket);
//		break;
//	case ASS_S_KICK_USER://����
//		OnKickUserMsg(pNetHead,pNetData,uDataSize,pClientSocket);
//		break;
//	}
//	return true;
//}
//�㲥��Ϣ
//bool CGameRoomEx::OnBroadcastMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
//{
//	if(uDataSize!=sizeof(tagBroadcastMsg))
//		return false;
//	
//	switch(pNetHead->bAssistantID)
//	{
//	case ASS_S_USE_BROADCAST:
//		tagBroadcastMsg * pData = (tagBroadcastMsg*) pNetData;	
//		Dudu(pData);
//		break;
//	}
//	return true;
//}
//������Ϣ
bool CGameRoomEx::OnKickUserMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if(sizeof(_TAG_KICKUSER)!= uDataSize)
		return false;
	_TAG_KICKUSER *pKick = (_TAG_KICKUSER*)pNetData;
	UserItemStruct *pUserInfo = m_UserManage.FindOnLineUser(pKick->dwUserID);
	if(pUserInfo == NULL)
		return true;


	BZSoundPlay(this, "image\\prop\\Prop9B.mp3", 0);
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	TCHAR szMsg[200];
	wsprintf(szMsg,fMsg.GetKeyVal("GameRoom","������� %s �߳�����","������� %s �߳�����"),pUserInfo->GameUserInfo.nickName);
	
	//AFCMessageBox(szMsg);
	DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,szMsg);
	return true;
}

// PengJiLin, 2010-9-13, ʹ�����˿����˹���
bool CGameRoomEx::OnNewKickUserMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, 
                                    CTCPClientSocket * pClientSocket)
{
    if(uDataSize != sizeof(_TAG_NEW_KICKUSER)) return false;
    _TAG_NEW_KICKUSER* pNewKickProp = (_TAG_NEW_KICKUSER*)pNetData;

    // �Լ��߱���
    if(pNewKickProp->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
    {
        if(NULL != m_IGameFrame)
        {
			BZSoundPlay(this, "image\\prop\\Prop9A.mp3", 0);
            m_IGameFrame->OnSocketReadEvent(pNetHead,pNetData,uDataSize,NULL);
        }
    }
    // �������Լ�
    else if(pNewKickProp->dwDestID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
    {
        if(ASS_PROP_NEW_KICKUSER == pNetHead->bAssistantID)
        {
            switch(pNetHead->bHandleCode)
            {
            case ERR_GR_NEW_KICKUSER_SUCCESS:       // �ɹ�
                {
                    UserItemStruct* pUserItem = NULL;
                    pUserItem = m_UserManage.FindOnLineUser(pNewKickProp->dwUserID);

                    CString strInfo = "";
                    if(NULL != pUserItem)strInfo.Format("�������%sʹ��\"���˿�\"�߳���Ϸ��", pUserItem->GameUserInfo.nickName);
                    else strInfo.Format("�������%dʹ��\"���˿�\"�߳���Ϸ��", pNewKickProp->dwUserID);
                    BZSoundPlay(this, "image\\prop\\Prop9B.mp3", 0);

                    //�Ƚ�������
                    if(!boardCast)
                    {
                        boardCast=new CBoardCast();
                        boardCast->FuncForProp(TRUE, TRUE, FALSE);
                        boardCast->SetPropInfo(strInfo);
                        boardCast->Create(IDD_BOARDCAST,GetDesktopWindow());
                        boardCast->pParentWnd=(CWnd*)this;
                    }
                    boardCast->FuncForProp(TRUE, TRUE, FALSE);
                    boardCast->SetPropInfo(strInfo);
                    boardCast->ShowWindow(SW_SHOW);
                }
                break;
            case ERR_GR_NEW_KICKUSER_NOTIME:        // �Է��ĵ����ѹ��ڣ�����û�е���
                break;
            case ERR_GR_NEW_KICKUSER_HAD_ANTI:      // �Լ��з��߿�ʱ��
                break;
            case ERR_GR_NEW_KICKUSER_HAD_VIP:       // �Լ���VIPʱ��
                break;
            case ERR_GR_NEW_KICKUSER_PLAYING:       // ��Ϸ�в�����
                break;
            default:
                break;
            }
        }
    }
}

bool CGameRoomEx::OnNeBankMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{

	// �����Ϸ�����ˣ����͵���Ϸ�ͻ���
	if (NULL != m_IGameFrame)
	{
		m_IGameFrame->OnSocketReadEvent(pNetHead, pNetData, uDataSize, pClientSocket);
	}

	if (m_pBankWndEx != NULL)
	{
		m_pBankWndEx->OnNetMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}

	return true;
}

//ˢ��ĳ��������
BOOL CGameRoomEx::UpdateHoldItem(int ItemID)
{
	//SendGameData(MDM_GR_UPDATE_HOLD_ITEM_DATA, 0, ItemID);
	return TRUE;
}

//�Ŷ�
bool CGameRoomEx::AddQueue()
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;

	//�ж��Ƿ������Ϸ
	if ((m_GameInfo.uLessPoint!=0L)&&
		(m_GameInfo.pMeUserInfo->GameUserInfo.i64Money<(int)(m_GameInfo.uLessPoint))&&
		m_pRoomInfo->pComRoomInfo.uComType==TY_MONEY_GAME)
	{
		CString strMessage;
		TCHAR szNum[128]; 
        GetNumString(szNum, m_GameInfo.uLessPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
		int r = DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,true,
			fMsg.GetKeyVal("GameRoom","LackCoins","����Ϸ��������Ҫ��%s�Ľ��,���Ľ�Ҳ���,�쵽Ǯ��ȡ��Ұ�!"),
			szNum);

		if (r == IDOK) NoEnoughMoneyEvent();

		return 0;
	}

	if (m_GameInfo.uMaxPoint != 0L)
	{
		if (m_pRoomInfo->pComRoomInfo.uComType == TY_NORMAL_GAME)
		{
			if (m_GameInfo.pMeUserInfo->GameUserInfo.dwPoint>(int)(m_GameInfo.uMaxPoint))
			{
				CString strMessage;
				TCHAR szNum[128]; 
				GetNumString(szNum, m_GameInfo.uMaxPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);

				DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,
					fMsg.GetKeyVal("GameRoom","OverflowCoints","���Ľ���Ѷ���%s�������ڴ˷�����Ϸ��"),szNum);

				return 0;
			}
		}
		else if (m_pRoomInfo->pComRoomInfo.uComType == TY_MONEY_GAME)
		{
			if (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money>(int)(m_GameInfo.uMaxPoint))
			{
				CString strMessage;
				TCHAR szNum[128]; 
				GetNumString(szNum, m_GameInfo.uMaxPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);

				DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,
					fMsg.GetKeyVal("GameRoom","OverflowCoints","���Ľ���Ѷ���%s�������ڴ˷�����Ϸ��"),szNum);

				return 0;
			}
		}
	}

	if(m_bAddQueue)
	{
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","NoRepeatJoin","���Ѽ�����Ϸ����,�����ظ�����!"),fMsg.GetKeyVal("GameRoom","Tip","��ʾ"));
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,fMsg.GetKeyVal("GameRoom","Tip","��ʾ"),false,fMsg.GetKeyVal("GameRoom","NoRepeatJoin","���Ѽ�����Ϸ����,�����ظ�����!"));
		return true;
	}
	if(m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO != 255)
	{
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","LeaveToQueue","���Ѿ�����,�����뿪��Ϸ��,�����Ŷ�?"),fMsg.GetKeyVal("GameRoom","Tip","��ʾ"));
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,fMsg.GetKeyVal("GameRoom","Tip","��ʾ"),false,fMsg.GetKeyVal("GameRoom","LeaveToQueue","���Ѿ�����,�����뿪��Ϸ��,�����Ŷ�?"));
		return true;
	}
#ifdef MY_DEV
	OnSitDesk();
#else
	//if(IDOK == AFCMessageBox(fMsg.GetKeyVal("GameRoom","InQueueSystemAutoAssignDesk","����������Ϸ����,ϵͳ���Զ��������ŵ���Ϸ��!"),
	//	fMsg.GetKeyVal("GameRoom","Tip","��ʾ"),AFC_YESNO))
	if(IDYES == DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,fMsg.GetKeyVal("GameRoom","Tip","��ʾ"),true,fMsg.GetKeyVal("GameRoom","InQueueSystemAutoAssignDesk","����������Ϸ����,ϵͳ���Զ��������ŵ���Ϸ��!")))
		SendGameData(MDM_GR_QUEUE_MSG, ASS_GR_ADD_QUEUE, 0);
#endif
	return true;
}

//����������
bool CGameRoomEx::OnSitDesk()
{
	//Ч��״̬
	if (m_bLogonFinish==false)
		return true;
	if (CanCloseNow()==false)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","PlayingNoChangeSeat","������Ϸ��,���ܻ�λ��!"),m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_GameInfo.szGameName,false,fMsg.GetKeyVal("GameRoom","PlayingNoChangeSeat","������Ϸ��,���ܻ�λ��!"));
		return true;
	}
	//���ҿ���λ
	//OnFindNullDesk();
	//���������λ
	BYTE bDeskIndex = 255,bDeskStation = 255;
	OnFindRandDesk(bDeskIndex,bDeskStation);
	if(bDeskIndex == 255 || bDeskStation == 255)
	{
		//MessageBox("��Ϸ������Ч");
		return true;
	}

	MSG_GR_S_UserSit stUserSit;
	stUserSit.bDeskIndex=bDeskIndex;
	stUserSit.bDeskStation=bDeskStation;
	SendData(&stUserSit,sizeof(stUserSit),MDM_GR_USER_ACTION,ASS_GR_USER_SIT,0);
	return true;
}

//��������������λ(���)
bool CGameRoomEx::OnFindRandDesk(BYTE &bDeskIndex,BYTE &bDeskStation)
{
	//srand(GetTickCount());
	//int findStart  = rand() % m_pRoomInfo->ComRoomInfo.uDeskCount;
	//ͳ��������������
	BYTE iCount = 0,desk = 255;
	for(int i = 0; i <  m_pRoomInfo->pComRoomInfo.uDeskCount; i ++)
	{
		//��Ϸ�Ѿ���ʼ
		if(m_DeskFrame.IsPlayGame(i))
			continue;
		BYTE iValue = CountAmount(i);
		if(iValue > iCount)
		{
			iCount = iValue;
			desk = i;
		}
	}
	BYTE station = 255;
	//���ҽ��(ȫ���˻�û���κ���)
	if(iCount == 0 && desk == 255)
	{
		desk = 0;
	}
	station = m_DeskFrame.GetNullStation(desk);

	bDeskIndex = desk;
	bDeskStation = station;
	return true;
}

//����
void CGameRoomEx::IE(LPCTSTR URL)
{
	if (m_WebView.GetSafeHwnd()!=NULL)
		m_WebView.Navigate(URL,NULL,NULL,NULL,NULL);
}


//ͳ��ĳ������
BYTE CGameRoomEx::CountAmount(BYTE bDeskIndex)
{
	int iCount = 0;
	for(int i = 0 ; i < m_UserManage.GetOnLineUserCount(); i ++)
	{
		UserItemStruct * pFindUserItem =  m_UserManage.FindOnLineUserByIndex(i);
		if(pFindUserItem == NULL)
			continue;
		if(pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex)
		{
			iCount ++ ;
		}
	}
	return iCount;
}

void CGameRoomEx::UseBoardCast(_TAG_USERPROP * userProp)
{
	//�Ƚ�������
	if(!boardCast)
	{
		boardCast=new CBoardCast();
		boardCast->Create(IDD_BOARDCAST,GetDesktopWindow());
		boardCast->pParentWnd=(CWnd*)this;
	}

	boardCast->m_nPropID=userProp->nPropID;
	//Ҫ�������������
	if(GetMainRoom()->propPannel)
		GetMainRoom()->propPannel->ShowWindow(SW_HIDE);

	if(userProp->attribAction & 0x80)
		boardCast->SetValue(BRD_MSG_BIG);
	else
		boardCast->SetValue(BRD_MSG_SMALL);
	boardCast->GetDlgItem(IDC_EDIT_INPUT)->SetWindowText(_T(""));
	boardCast->ShowWindow(SW_SHOW);
}

void CGameRoomEx::SendBoardCast(_TAG_BOARDCAST * boardCastMsg, int BRD_MSG_TYPE)
{
	if(BRD_MSG_TYPE==BRD_MSG_BIG)
	{//������
		GetMainRoom()->SendData(boardCastMsg,sizeof(_TAG_BOARDCAST),MDM_GP_PROP,ASS_PROP_BIG_BOARDCASE,0);
	}
	else
	{//С����
		SendGameData(boardCastMsg,sizeof(_TAG_BOARDCAST),MDM_GR_PROP,ASS_PROP_SMALL_BOARDCASE,0);
	}
}

void CGameRoomEx::OnGiveProp(long dwUserID,int openFrom)
{
	if (dwUserID!=0L)
	{
		UserItemStruct * pUserInfo=m_UserManage.FindOnLineUser(dwUserID);
		if (pUserInfo==NULL) 
			return;
		//////////////////////////////////////
		///Kylin 20090104 ���ӵ��ߴ���δ��ʼ��������
		if(!GetMainRoom()->propPannel)
		{
			GetMainRoom()->propPannel=new CPropPannel();
			GetMainRoom()->propPannel->pParentWnd=GetMainRoom();//(CWnd*)this;
			GetMainRoom()->propPannel->Create(IDD_PROPPANNEL,GetMainRoom());//GetDesktopWindow());
			GetMainRoom()->propPannel->CenterWindow();
		}
		GetMainRoom()->propPannel->openPannel(openFrom,pUserInfo->GameUserInfo.nickName);
		//////////////////////////////////////
	}
}

void CGameRoomEx::InsertSystemMessageWithGame(CString strMsg)
{
	///////////////////////////
	//Kylin 20090211  ��ֹ��Ϣ���޸�
	CString csTemp = strMsg; 

	// �ڷ��ͱ����ʱ���ı�csTemp������ 
	m_pRightWnd->m_MessageHandle.InsertSystemMessage(csTemp.GetBuffer()); 
	csTemp.ReleaseBuffer(); 
	if (m_IGameFrame!=NULL) 
	{ 
		csTemp = strMsg;   // ���³�ʼ������ 
		m_IGameFrame->ShowMsgInGame(csTemp.GetBuffer()); 
		csTemp.ReleaseBuffer(); 

	}
	///////////////////////////
}
//20081211
void CGameRoomEx::RandVirtualMessage(UserItemStruct * userItem)
{
	if(!dwRandWords)
		return;
	int wcount=cfgGetValue(dwRandWords,"RANDWORDS","COUNT",0);
	if(wcount<=0)
		return;
	srand((unsigned)time(NULL));
	int r=rand()%wcount;
	CString itemid;
	itemid.Format("WORD%d",r);
	CString words=cfgGetValue(dwRandWords,"RANDWORDS",itemid,"");
	if(words!="")
		m_pRightWnd->m_MessageHandle.InsertUserTalk(userItem->GameUserInfo.nickName,NULL,
		words.GetBuffer(words.GetLength()),m_colorresource.m_crNormalTxColor,false);

}

//����Ϸ�����������Ϣ
//JianGuankun 2012.1.12
void CGameRoomEx::BoardcastTrottingMsg(TMailItem* pTrottingMsg)
{
	if (m_IGameFrame)
	{
		m_IGameFrame->OnControlMessage(CM_USER_TROTTING,pTrottingMsg,sizeof(TMailItem));
	}
	return;
}

//��Ҳ����������л��߳�ֵ����
void CGameRoomEx::NoEnoughMoneyEvent()
{
	if (m_lessPointType)
	{
		if (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money < (int)m_GameInfo.uLessPoint && m_pRoomInfo->pComRoomInfo.uComType==TY_MONEY_GAME)
		{
			if (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money + m_GameInfo.pMeUserInfo->GameUserInfo.i64Bank < (int)m_GameInfo.uLessPoint)
			{
				//������վ��ֵҳ��
				ShellExecute(NULL,"open",m_strLessPointTipURL,NULL,NULL,SW_MAXIMIZE);
			}
			else
			{
				OnBnClickedBt8();
			}
		}
	}
}

/************************************************************************/
//Function:checkNetSignal()
//Parameter:none
//Author:Fred Huang 2008-06-27
//Note:�������������������Ϣ
/************************************************************************/
void CGameRoomEx::checkNetSignal(void)
{
	if (m_GameInfo.pMeUserInfo == NULL)
	{
		return;
	}
	if(m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID==0)        //û��ID�ţ�����
        return;
    //����ϴε���Ϣ��û���յ��������񼫲����Ϣ����Ҫ��
    if(m_NetSignalIndex>m_SavedSingalIndex)
    {
		m_NetSignalLevel=0;
    }
	CRect rc;
	rc.left = m_netSignX;
	rc.top = m_netSignY;
	rc.right = rc.left + img->GetWidth();
	rc.bottom = rc.top + img->GetHeight();
	InvalidateRect(&rc, FALSE);
    //���ṹ��
    MSG_GP_NETSignal netSignal;
    netSignal.dwIndex=++m_NetSignalIndex;
    netSignal.dwUserID=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID;
    netSignal.dwTimeFlag=GetTickCount();
    //������Ϣ
    SendData(&netSignal,sizeof(MSG_GP_NETSignal),MDM_GR_NETSIGNAL,0,0);
}

/************************************************************************/
//Function:checkNetSignal()
//Parameter:
//                pNetData        :���������ص���Ϣ��
//Author:Fred Huang 2008-06-27
//Note:�������������ص���Ϣ
/************************************************************************/
void CGameRoomEx::getNetSignal(void * pNetData)
{
        MSG_GP_NETSignal *netSignal=(MSG_GP_NETSignal*)pNetData;
        //������Ƿ����Լ��ģ�����
        if(m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID!=netSignal->dwUserID)
                return;
        //�Ƚ�������0
        m_NetSignalLevel=0;
        //����ʱ��
        UINT uTimeDiff=GetTickCount()-netSignal->dwTimeFlag;
        //ֻ���ǵ�ǰ����Ϣ�����ż��㼶��
        if(netSignal->dwIndex==m_NetSignalIndex)
        {
                if(uTimeDiff<=50)
                        m_NetSignalLevel=1;
                else if(uTimeDiff>50 && uTimeDiff<=200)
                        m_NetSignalLevel=2;
                else if(uTimeDiff>200 && uTimeDiff<=500)
                        m_NetSignalLevel=3;
                else if(uTimeDiff>500 && uTimeDiff<=1000)
                        m_NetSignalLevel=4;
                else                 
                        m_NetSignalLevel=5;
			
        }
        //���浱ǰ�յ������
        m_SavedSingalIndex=netSignal->dwIndex;
        //��ͼ
		CRect rc;
		rc.left = m_netSignX;
		rc.top = m_netSignY;
		rc.right = rc.left + img->GetWidth();
		rc.bottom = rc.top + img->GetHeight();
		InvalidateRect(&rc, FALSE);
}

/************************************************************************/
//Function:drawSignal()
//Parameter:��
//Author:Fred Huang 2008-06-27
//Note:��ͼ
/************************************************************************/
void CGameRoomEx::drawSignal(CDC *pDC)
{
	if(!img || img->GetFlags()==0)
		return;
	Graphics g(pDC->GetSafeHdc());
	g.DrawImage(img,Rect(m_netSignX,m_netSignY,img->GetWidth(),img->GetHeight()/6),0,m_NetSignalLevel*img->GetHeight()/6,img->GetWidth(),img->GetHeight()/6,UnitPixel);

}
//20081211
bool CGameRoomEx::OnAloneDesk(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{

	ASSERT(pNetHead->bMainID==MDM_GR_DESK);
	UserItemStruct *pUserInfo=NULL;      //�����Ϣ
	//if(uDataSize!=sizeof(MSG_GR_SR_MatchDelete)) return false;
	MSG_GR_SR_MatchDelete* pDeskAlone=(MSG_GR_SR_MatchDelete *)pNetData;
	if(1 == pNetHead->bHandleCode)//��
	{
		//wuhsuqun 2009.6.6
		//��ʱ�����������
		m_pUserList->fillVirtualUser(m_GameInfo.uDeskPeople);
		//
		int c=0;
		for(int i=0;i<m_pRightWnd->m_UserList.GetItemCount();i++)
		{
			pUserInfo=m_pRightWnd->m_UserList.GetIdelVirtualUser();
			if(!pUserInfo)
			{
			//	MSG_GR_SR_MatchDelete aloneDesk;
			//	aloneDesk.dwUserID=m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID;
			//	aloneDesk.bDeskIndex=pNetHead->bAssistantID;
			//	aloneDesk.bDeskStation=c;
			//	SendData(&aloneDesk,sizeof(aloneDesk),MDM_GR_MANAGE,ASS_GR_DEALONE_DESK,0);
				if(bExtendRobot)
				{
					CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
					CString str = fMsg.GetKeyVal("GameRoom","NoEnoughVirtualPlayer","û���㹻�����������!");
					m_pRightWnd->m_MessageHandle.InsertSystemMessage(str.GetBuffer());
				}
				return true;
			}
			if(pUserInfo->GameUserInfo.bUserState!=USER_PLAY_GAME && pUserInfo->GameUserInfo.dwUserID<0)
			{
				pUserInfo->GameUserInfo.bUserState=USER_PLAY_GAME;
				pUserInfo->GameUserInfo.bDeskNO=pNetHead->bAssistantID;
				pUserInfo->GameUserInfo.bDeskStation=c;
				m_pRightWnd->m_UserList.UpdateGameUser(pUserInfo);
				m_DeskFrame.SetUserItem(pNetHead->bAssistantID,c,pUserInfo);
				if(++c>=m_GameInfo.uDeskPeople)
					break;
			}
			
		}
		//m_DeskFrame.SetLock(pNetHead->bAssistantID,true);

		m_DeskFrame.SetLock(pNetHead->bAssistantID,false);  //ȥ��������־  yjj 090306
		CBaseDeskView * pDesk=m_DeskFrame.GetDeskArrayPtr(int(pNetHead->bAssistantID));
		
		//pDesk->SetLock(true,&m_DeskFrame);   ȥ�����ı�־   yjj 090306
		
		m_DeskFrame.SetPlayGame(pNetHead->bAssistantID,true);

		SetTimer(2000+pNetHead->bAssistantID,(m_GameInfo.uVirtualGameTime+rand()%20-10)*1000,NULL);

		//	m_DeskFrame.UpdateDeskView(bDeskIndex);
	}
	else
	{
		m_DeskFrame.SetLock(pNetHead->bAssistantID,false);
		for(int i=0;i<m_GameInfo.uDeskPeople;i++)
		{
			pUserInfo=(UserItemStruct*)m_DeskFrame.GetUserItem(pNetHead->bAssistantID,i);

			if(pUserInfo)
			{
				m_pRightWnd->m_UserList.DeleteGameUser(pUserInfo);

				m_DeskFrame.SetUserItem(pNetHead->bAssistantID,i,NULL);
				pUserInfo->GameUserInfo.bUserState=USER_LOOK_STATE;
				pUserInfo->GameUserInfo.bDeskNO=255;
				pUserInfo->GameUserInfo.bDeskStation=255;
//				m_pRightWnd->m_UserList.UpdateGameUser(pUserInfo);
			}
		}

		if (m_bDeAloneAll && ///< �����������
			pNetHead->bAssistantID+1 == m_pRoomInfo->pComRoomInfo.uDeskCount) ///< ������һ������
		{
			for (pUserInfo=m_pUserList->GetIdelVirtualUser(); NULL!=pUserInfo; pUserInfo=m_pUserList->GetIdelVirtualUser()) ///< ����������Ӻ󣬽��û��б���������������ɾ����
			{
				m_pRightWnd->m_UserList.DeleteGameUser(pUserInfo);
			}
		}


		CBaseDeskView * pDesk=m_DeskFrame.GetDeskArrayPtr(int(pNetHead->bAssistantID));
		pDesk->SetLock(false,&m_DeskFrame);
		m_DeskFrame.SetPlayGame(pNetHead->bAssistantID,false);
		KillTimer(2000+pNetHead->bAssistantID);
		KillTimer(3000+pNetHead->bAssistantID);
		KillTimer(4000+pNetHead->bAssistantID);
	}
	return true;
}

// PengJiLin, 2010-9-13, ���˿������߿�ʹ��ʱ������
void CGameRoomEx::SetKickPropTime(int iKickTime)
{
    m_iKickPropTotalTime = iKickTime;
    if(iKickTime > 0) m_bCheckKickPropTime = TRUE;
}
void CGameRoomEx::SetAntiKickPropTime(int iAntiKickTime)
{
    m_iAntiKickPropTotalTime = iAntiKickTime;
    if(iAntiKickTime > 0) m_bCheckAntiKickPropTime = TRUE;
}

bool CGameRoomEx::GetSendMsgUserStruct(long int lUserID, SendMsgUserStruct * user)
{
	bool ret = false;
	UserItemStruct *p = m_UserManage.FindOnLineUser(lUserID);
	if(p != NULL)
	{
		user->bDeskNO = p->GameUserInfo.bDeskNO;
		user->bGameMaster = p->GameUserInfo.bGameMaster;
		user->dwUserID = p->GameUserInfo.dwUserID;
		user->dwAccID = p->GameUserInfo.dwAccID;
		strcpy(user->nickName, p->GameUserInfo.nickName);
		ret = true;
	}
	return ret;
}

SendMsgUserStruct * CGameRoomEx::GetMeUserInfo()
{
	m_SortMsgUserInfo.bDeskNO = m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO;
	m_SortMsgUserInfo.bGameMaster = m_GameInfo.pMeUserInfo->GameUserInfo.bGameMaster;
	m_SortMsgUserInfo.dwUserID = m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID;
	m_SortMsgUserInfo.dwAccID = m_GameInfo.pMeUserInfo->GameUserInfo.dwAccID;
	strcpy(m_SortMsgUserInfo.nickName, m_GameInfo.pMeUserInfo->GameUserInfo.nickName);

	return &m_SortMsgUserInfo;
}

//���ͺ���
int CGameRoomEx::SendData(void * pData, UINT uSize, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return CBaseRoom::SendData(pData, uSize, bMainID, bAssistantID, bHandleCode);
}

//������ͺ���
int CGameRoomEx::SendData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return CBaseRoom::SendData(bMainID, bAssistantID, bHandleCode);
}
//////////////////////////////////////////////////////////////////////////
// YangJie 2012-05-14 9:23 AM
// Unity3d��Ϣ���ݺ���
int CGameRoomEx::u3dSendGameBaseInfo() 
{
	u3dSGameBaseInfo gamebaseinfo;
	gamebaseinfo.iDeskPeople		= m_pRoomInfo->pComRoomInfo.uDeskPeople;
	gamebaseinfo.iLessExperience	= 0;
	gamebaseinfo.iPower				= 0;
	gamebaseinfo.lRoomRule			= m_pRoomInfo->pComRoomInfo.dwRoomRule;
	gamebaseinfo.iGameNameID		= m_pRoomInfo->pComRoomInfo.uNameID;
	u3dchar2Wchar(m_pRoomInfo->pComRoomInfo.szGameRoomName, gamebaseinfo.szGameRoomName);
	gamebaseinfo.iContestType		= m_pRoomInfo->pComRoomInfo.uComType;
	

	CString str;
	str.Format("flow - u3dSendGameBaseInfo Unity3d�����Ϸ��Ϣ���� -> %d uNameID -> %d", sizeof(u3dSGameBaseInfo), m_pRoomInfo->pComRoomInfo.uNameID);
	OutputDebugString(str);
	if (NULL != m_IGameFrame) {
		
		OutputDebugString("flow - u3dSendGameBaseInfo m_IGameFrame != NULL");
		m_IGameFrame->OnControlMessage(CM_U3D_GAMEBASEINFO, &gamebaseinfo, sizeof(u3dSGameBaseInfo));
	}
	else {
		
		OutputDebugString("flow - u3dSendGameBaseInfo m_IGameFrame == NULL");
	}
	return 0;
}

void u3dchar2Wchar(char* pchar,wchar_t* wchar)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP,0,pchar,-1,NULL,0);
	MultiByteToWideChar(CP_ACP,0,pchar,-1,wchar,nwLen);
}

LRESULT CGameRoomEx::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	//��Ƥ��
	LoadSkin();

	//��ʼ
	TCHAR path[MAX_PATH];
	CString skinfolder = "";
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	
	//��ȡbcf���ü�����ͼƬ
	CBcfFile f( s + strSkin);
	skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());
	wsprintf(path,"%sDialog\\scroll.bmp",skinfolder);
	HBITMAP hbmScorll = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
	
	//ж�ع�����ԭƤ��,������Ƥ��
	SkinSB_Uninit(m_WebView.GetSafeHwnd());
	SkinSB_Init(m_WebView.GetSafeHwnd(), hbmScorll);
	SkinSB_Uninit(m_DeskFrame.GetSafeHwnd());
	SkinSB_Init(m_DeskFrame.GetSafeHwnd(), hbmScorll);

	//ˢ��
	Invalidate();

	return LRESULT(0);
}
