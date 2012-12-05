#include "Stdafx.h"
#include <Windows.h>
#include <comutil.h>
#include "GamePlace.h"
#include "MainRoomEx.h"
#include "FileVersion.h"
#include "LongonDialog.h"
#include "LogonDialog.h"
#include "GamePlaceDlg.h"
#include "GameRoomEx.h"
#include "GamePlaceMessage.h"
#include "LockAccountDlg.h"
#include "BuildTime.h"
#include "IEAdvertise.h"
#include "GMMsgDlg.h"
#include "DlgRoomFee.h"
#include "GameListTip.h"
#include "IDVerifyWnd.h"
#include "MobileVerifyWnd.h"
#include "bank4ZMessage.h"
#include "MiniPopWnd.h"
  
#include "PassDesk.h"	/// Add by lxl 2010-10-08 ����ԭ���������Ի��򣬻�ͼƬ���ɣ������������뷿��
#include "iphlpapi.h"	//�������� zxj 2009-11-12
#pragma comment(lib,"Iphlpapi.lib")	//�������� zxj 2009-11-12

#include "detours.h"
#include "skinsb.h"


//�˵���Ϣ
#define IDM_IN_ROOM						100	
#define IDM_SET_USUAL_SERVER			101
#define IDM_DOWN_NEW					102
#define IDM_NOW_PEOPLE					103
#define IDM_MAX_PEOPLE					104
#define IDM_NOW_VER						105
#define IDM_MAX_VER						106

//�궨��
#define CONTROL_SPACE					0							//�ؼ����
#define SPLIT_WIDE						7							//��������
#define BT_BUTTON_WIDE					50							//��ť���
#define BT_BUTTON_HIGHT					20							//��ť�߶�
#define TITLE_HIGHT						26							//�������߶�
//#define BUTTON_WIDE					8							//���ذ�ť���
//#define BUTTON_HIGHT					80							//���ذ�ť�߶�
#define BUTTON_WIDE						9							//���ذ�ť���
#define BUTTON_HIGHT					109							//���ذ�ť�߶�
#define BT_BIG_BUTTON_WIDE				65							//��ť���
#define BT_BIG_BUTTON_HIGHT				20							//��ť�߶�
#define ROOM_BAR_HEIGHT					8							//�����ܸ߶�
#define MESSAGE_TITLE					TEXT("ϵͳ��Ϣ")		//��Ϸ��Ϣ����

//��ܴ�С 
#define ROOM_FRAME_TOP					104//56							//��Ϸ���ܿ������Ķ����ĸ߶�
#define ROOM_FRAME_LEFT					8//3							//������
#define ROOM_FRAME_RIGHT				0//4							//����ҿ�
#define ROOM_FRAME_BOTTOM				0//12							//����¸�

//����궨��
#define MS_LOGON						1							//��½����
#define MS_GET_ROOM						2							//��ȡ����
#define ID_CHECK_SEND					50							//��ⶨʱ��
#define ID_AI_ENTERROOM					51							//�����˽��뷿��
#define CHECK_TIME						3000						//��ʱ�����
#define CHECK_UPDATE					3
#define ID_CHECK_SIGNAL                 5
#define	IDT_CHANGE_LOCK					55							//������������״̬��ʱ�� zxj 2009-11-12
#define CHANGE_LOCK_TIME				30*1000						//������������״̬��ʱ����� 30 �� zxj 2009-11-12
#define TIME_KEEP_ALIVE					56

#ifdef MY_QUICK_GAME_ON

#define IDT_QUICK_ENTER_ROOM			10010						//JianGK,���ٽ���Ϸ���䶨ʱ��
#define IDT_QUICK_GAME					10012						//JianGK,���ٽ���Ϸ��ʱ��

#endif // MY_QUICK_GAME_ON
//��ť
#define BUTTON_TOP_HIGHT				46							//�ܿ�ܶ�����ťͼƬ�ĸ߶�
#define BUTTON_TOP_WIDE					58							//������ťͼƬ�Ŀ��
//====================================================================================
//====================================================================================
#define ROOM_LIST_BAR_POS_X					8							//
#define ROOM_LIST_BAR_POS_Y					0//286					//����������λ��
#define ROOM_LIST_BAR_WIDTH					230
#define ROOM_WEB_POS_X						(ROOM_LIST_BAR_WIDTH+SPLIT_WIDE+4+4)

#define VK_B								66						 //'B'key
#define VK_Z								90						 //'Z'key
#define VK_W								87						 //'W'key
//��Ϸ����ָ��
static CMainRoomEx * pGamePlace=NULL;
CPtrArray m_ShortMessage;						//��Ϣ����

// PengJiLin, 2010-6-2, DLL ���ھ��
#define WM_GAME_NOTIFY_TO_DLL   WM_USER + 100   // ���� DLL ���ھ������Ϣ
#define    WPARAM_GAME_LOGON_SUCCESS    0x10    // ��¼�ɹ�
#define    WPARAM_GAME_LOGON_FAIL       0x13    // ��¼ʧ��
#define NOTINSTALL						99     //δ������Ϸ
extern HWND g_hWndDLL;
extern BOOL g_bRunBySoftware;

bool CMainRoomEx::bTipInit = false;
bool CMainRoomEx::m_bBankLogoned = false;   //�ѵ�½����?
bool CMainRoomEx::m_bLockQuit = false;

char CMainRoomEx::m_szMobileVCode[36];	//�ӷ����̷��������ֻ���֤��MD5����

// ��������ַ����Ŀո�
void ClearNumberSpace(char* pchInput, char* pchOutput)
{
	char* ptr1 = NULL;
	char* ptr2 = NULL;
	ptr1 = pchInput;
	while(*ptr1 != '\0' && (*ptr1 < '0' || *ptr1 > '9')) ++ptr1;
	ptr2 = ptr1;
	while(*ptr2 != '\0' && *ptr2 >= '0' && *ptr2 <= '9') ++ptr2;
	++ptr2;
	memcpy(pchOutput, ptr1, ptr2-ptr1);
}

string GetAppPath()
{
	char _tmpbuf[MAX_PATH + 1];
	ZeroMemory(_tmpbuf, sizeof(_tmpbuf));
	GetModuleFileNameA(NULL, _tmpbuf, MAX_PATH);

	string r = _tmpbuf;
	int l = r.find_last_of('\\');
	return r.substr(0, l);
} 

// ��ȡ��Դ��İ汾����ʽ���ɣ� BZW.3.001
CString GetResourceFileVer()
{
	char cPath[200];
	char szVersionBuffer[200];
	DWORD dwHandle,InfoSize;
	CString strVersion = "";

	::GetModuleFileName(NULL,cPath,sizeof(cPath));      //���Ȼ�ð汾��Ϣ��Դ�ĳ���
	InfoSize = GetFileVersionInfoSize(cPath,&dwHandle); //���汾��Ϣ��Դ���뻺����
	if(InfoSize==0) return _T("None VerSion Supprot");

	char *InfoBuf = new char[InfoSize];
	GetFileVersionInfo(cPath,0,InfoSize,InfoBuf);       //��������ļ�ʹ�õĴ���ҳ���ļ��汾

	unsigned int  cbTranslate = 0;
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate;

	VerQueryValue(InfoBuf, TEXT("\\VarFileInfo\\Translation"),(LPVOID*)&lpTranslate,&cbTranslate);

	// Read the file description for each language and code page.
	for( int i=0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ )
	{
		char  SubBlock[200];
		wsprintf( SubBlock, TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"),
			lpTranslate[i].wLanguage, lpTranslate[i].wCodePage);
		void *lpBuffer = NULL;
		unsigned int dwBytes = 0;
		VerQueryValue(InfoBuf, SubBlock, &lpBuffer, &dwBytes);
		CString strTemp = (char *)lpBuffer;
		strVersion += strTemp;
		break;
	}

	// ���Ӧ���������� 3, 0, 2, 5  ������ȫ��ȡ������
	// ����пո������
	char chData_1[10] = {0};
	char chVer_1[10] = {0};
	char chData_2[10] = {0};
	char chVer_2[10] = {0};
	char chData_3[10] = {0};
	char chVer_3[10] = {0};
	char chData_4[10] = {0};
	char chVer_4[10] = {0};


	int iFind1 = strVersion.Find(',');
	if(iFind1 < 0)
	{
		iFind1 = strVersion.Find('.');
		if(iFind1 < 0)
		{
			AfxMessageBox("�汾1��������");
			return _T("None VerSion Found");
		}
	}
	strcpy(chData_1, strVersion.Left(iFind1));

	int iFind2 = strVersion.Find(',', iFind1+1);
	if(iFind2 < 0)
	{
		iFind2 = strVersion.Find('.', iFind1+1);
		if(iFind2 < 0)
		{
			AfxMessageBox("�汾2��������");
			return _T("None VerSion Found");
		}
	}
	strcpy(chData_2, strVersion.Mid(iFind1+1, iFind2-iFind1-1));

	int iFind3 = strVersion.Find(',', iFind2+1);
	if(iFind3 < 0)
	{
		iFind3 = strVersion.Find('.', iFind2+1);
		if(iFind3 < 0)
		{
			AfxMessageBox("�汾3��������");
			return _T("None VerSion Found");
		}
	}
	strcpy(chData_3, strVersion.Mid(iFind2+1, iFind2-iFind1-1));

	strcpy(chData_4, strVersion.Mid(iFind3+1));

	ClearNumberSpace(chData_1, chVer_1);
	ClearNumberSpace(chData_2, chVer_2);
	ClearNumberSpace(chData_3, chVer_3);
	ClearNumberSpace(chData_4, chVer_4);

	// �汾��ʽ���ַ���
	strVersion = "";
	strVersion.Format("BZW.%s.%s%s%s", chVer_1, chVer_2, chVer_3, chVer_4);

	delete[] InfoBuf;

	return strVersion;
}

BEGIN_MESSAGE_MAP(CMainRoomEx, CBaseRoom)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDM_IN_ROOM, OnInGame)  
	ON_COMMAND(IDM_CONNECT_CENTER_SERVER, OnConnectToCenterServer)
	ON_COMMAND(IDM_LOGON_PLACE, OnLogonPlace)
	ON_MESSAGE(WM_CLOSE_ROOM,OnCloseGameRoom)
	ON_MESSAGE(WM_COLSE_ROOM_WITH_MSG,OnCloseGameRoomWithMsg)
	ON_MESSAGE(WM_LOADSKIN,OnLoadSkin)
	ON_MESSAGE(IDM_SHOW_LEFT,OnShowLeft)
	ON_MESSAGE(IDM_TAB_SELECT,OnTopTabSelected)
	ON_MESSAGE(IDM_TAB_SELECT_LOOST,OnTopTabLostSelected)
	ON_MESSAGE(IDM_OPEN_IE,OnOpenIE)
	ON_MESSAGE(IDM_OPEN_BANK, OnOpenBank)

	ON_MESSAGE(IDM_CALL_GM, OnCallGM)

	ON_MESSAGE(IDM_MESSAGE_APPEAL,OnAppeal)
	ON_MESSAGE(IDM_LOCK_ACCOUNT, OnLockAccount)	//�������� zxj 2009-11-12
	ON_MESSAGE(WM_EXCHANGE_SKIN,OnExchangeSkin)
	
	//ON_NOTIFY(NM_LCLICK, IDC_GAME_LIST, OnNMLclickGameList)//�����Ϸ�б�
	//	ON_NOTIFY(NM_RCLICK, IDC_GAME_LIST, OnNMRclickGameList)//�һ���Ϸ�б�
	//	ON_NOTIFY(NM_DBLCLK, IDC_GAME_LIST, OnNMDblclkGameList)//˫����Ϸ�б�
	//	ON_NOTIFY(TVN_SELCHANGED, IDC_GAME_LIST, OnTvnSelchangedGameList)
	//	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_GAME_LIST, OnTvnItemexpandingGameList)
	//	ON_BN_CLICKED(IDC_HIDE_LIST, OnBnClickedHideList)
	//	ON_BN_CLICKED(IDC_HIDE_NEWS, OnBnClickedHideNews)
	ON_WM_PAINT()
	//ON_BN_CLICKED(IDC_BUTTON_MIN, OnBnClickedMin)
	//ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnBnClickedClose)
	//ON_BN_CLICKED(IDC_BUTTON_BT1, OnBnClicked1)///////////
	//ON_BN_CLICKED(IDC_BUTTON_BT2, OnBnClicked2)///////////
	//ON_BN_CLICKED(IDC_BUTTON_BT3, OnBnClicked3)///////////
	//ON_BN_CLICKED(IDC_BUTTON_BT4, OnBnClicked4)///////////
	//ON_BN_CLICKED(IDC_BUTTON_BT5, OnBnClicked5)///////////
	//	ON_BN_CLICKED(IDC_SKIN,OnBnClickedSkin)

	//	ON_WM_CTLCOLOR()
	//	ON_BN_CLICKED(IDC_SHOWMENU, &CMainRoomEx::OnBnClickedShowmenu)
	//	ON_BN_CLICKED(IDC_BT6, &CMainRoomEx::OnBnClickedBt6)
	//	ON_BN_CLICKED(IDC_BT7, &CMainRoomEx::OnBnClickedBt7)
	ON_BN_CLICKED(IDC_BTN_MAIN1,OnBnClickedBt1)
	ON_BN_CLICKED(IDC_BTN_MAIN2,OnBnClickedBt2)
	ON_BN_CLICKED(IDC_BTN_MAIN3,OnBnClickedBt3)
	ON_BN_CLICKED(IDC_BTN_MAIN4,OnBnClickedBt4)
	ON_BN_CLICKED(IDC_BTN_MAIN5,OnBnClickedBt5)
	ON_BN_CLICKED(IDC_BTN_MAIN6,OnBnClickedBt6)
	ON_BN_CLICKED(IDC_BTN_MAIN7,OnBnClickedBt7)
	ON_BN_CLICKED(IDC_BTN_MAIN8,OnBnClickedBt8)
	ON_BN_CLICKED(IDC_BTN_MAIN9,OnBnClickedBt9)
	ON_BN_CLICKED(IDC_BTN_MAIN10,OnBnClickedBt10)

	
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CMainRoomEx, CBaseRoom)
	ON_EVENT(CMainRoomEx, IDC_WEB, 250, CMainRoomEx::BeforeNavigate2Explorer, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CMainRoomEx, IDC_WEB, 271, CMainRoomEx::NavigateErrorExplorer, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

UINT CMainRoomEx::m_onlineUserCount = 0;
//���캯��
CMainRoomEx::CMainRoomEx(CGameListWnd *pGameListWnd,CActWnd *pActWnd,CTopWndClass *pTopWnd,CTopDuWndClass *pTopDuWnd)
: CBaseRoom(IDD_GAME_PLACE), m_NavigationDlg(this)

{
	m_HaveSellList = false;
	m_pListTip = NULL;
	m_bListmessage = false;
	m_bInit = false;
	m_bConnectingToCenterServer=false;
	m_bGetPara = false;
	//��ʼ������
	//m_width=442;
	m_bLogon=false;
	m_bCheckVer=false;
	m_bHideList=false;
	pGamePlace=this;
	memset(m_Mission,0,sizeof(m_Mission));
	memset(m_RoomInfo,0,sizeof(m_RoomInfo));
	memset(&m_PlaceUserInfo,0,sizeof(m_PlaceUserInfo));
	//lstrcpy(m_title,Glb().m_title);
	::InitializeCriticalSection(&m_Section);

	m_pGameListWnd = pGameListWnd;
	m_pActWnd = pActWnd;
	m_TopWnd = pTopWnd;
	m_TopDuWnd = pTopDuWnd;
	m_bShowSysmsg = true;
	logoUpload=NULL;
	hBulletin=NULL;
	propPannel=NULL;

	m_pAppealDlg = NULL;
	m_pGMListDlg = NULL;

	//�������źŵ�  yjj 090227
	img=NULL;
    m_NetSignalLevel=0;
    m_SavedSingalIndex=0;
	m_bLockMathine = FALSE;						//��ʾ�������������Ƿ����� zxj 2009-11-12
	m_bCanClick = TRUE;							//��ʾ�������������Ƿ����� zxj 2009-11-12

    // PengJiLin, 2010-9-13
    m_iKickPropTotalTime = 0;
    m_iAntiKickPropTotalTime = 0;
	m_nKeepAliveCount = 0;

	m_pBankWnd = NULL;
	m_pBankVerifyWnd = NULL;
	m_pBankWndEx = NULL;
	::memset(m_szMobileVCode,NULL,sizeof(m_szMobileVCode));

	m_pSafeCenterWnd = NULL;

#ifdef MY_QUICK_GAME_ON

	//Jiangk
	m_hQGNamedPipe = INVALID_HANDLE_VALUE;
	m_hQGThread = INVALID_HANDLE_VALUE;
	m_uQGRoomID = 0;

#endif // MY_QUICK_GAME_ON

}
//���캯��
CMainRoomEx::CMainRoomEx() : CBaseRoom(IDD_GAME_PLACE), m_NavigationDlg(this)
{
	m_HaveSellList = false;
	m_pListTip = NULL;
	m_bListmessage = false;
	m_bInit = false;
	m_bConnectingToCenterServer=false;
	m_bGetPara = false;
	//��ʼ������
	//m_width=442;
	m_bLogon=false;
	m_bCheckVer=false;
	m_bHideList=false;
	pGamePlace=this;
	memset(m_Mission,0,sizeof(m_Mission));
	memset(m_RoomInfo,0,sizeof(m_RoomInfo));
	memset(&m_PlaceUserInfo,0,sizeof(m_PlaceUserInfo));
	//lstrcpy(m_title,Glb().m_title);
	::InitializeCriticalSection(&m_Section);
	m_bShowSysmsg = true;
	logoUpload=NULL;
	propPannel=NULL;
	m_bLockMathine = FALSE;						//��ʾ�������������Ƿ����� zxj 2009-11-12
	m_bCanClick = TRUE;							//��ʾ�������������Ƿ����� zxj 2009-11-12
	m_pAppealDlg = NULL;
	m_pGMListDlg = NULL;

    // PengJiLin, 2010-9-13
    m_iKickPropTotalTime = 0;
    m_iAntiKickPropTotalTime = 0;

	m_pMobileVerifyWnd = NULL;

}
//�滭���
void CMainRoomEx::DrawViewFrame(CDC * pDC, int iWidth, int iHeight)
{
	//�滭���¿��
	//h
	CGameImageHelper helpt(&m_maintc);
	CGameImageHelper helpb(&m_mainbc);
	if(helpt.GetHeight()>0 && helpb.GetHeight()>0)
	{
		for(int i=0;i<iWidth;i+=helpt.GetWidth())
		{
			helpt.BitBlt(pDC->GetSafeHdc(),i,0);
			helpb.BitBlt(pDC->GetSafeHdc(),i,iHeight-helpb.GetHeight());
		}
	}
	//v
	CGameImageHelper helpl(&m_mainlc);
	CGameImageHelper helpr(&m_mainrc);
	if(helpl.GetHeight()>0 && helpr.GetHeight()>0)
	{
		for(int i=0;i<iHeight;i+=helpl.GetHeight())
		{
			helpl.BitBlt(pDC->GetSafeHdc(),0,i);
			helpr.BitBlt(pDC->GetSafeHdc(),iWidth-helpr.GetWidth(),i);
		}
	}
	//����ܱ߽�
	CGameImageHelper helplt(&m_mainlt);
	helplt.BitBlt(pDC->GetSafeHdc(),2,0);
	CGameImageHelper helprt(&m_mainrt);
	helprt.BitBlt(pDC->GetSafeHdc(),iWidth-helprt.GetWidth() - 2,0);
	CGameImageHelper helplb(&m_mainlb);
	helplb.BitBlt(pDC->GetSafeHdc(),0,iHeight-helplb.GetHeight());
	CGameImageHelper helprb(&m_mainrb);
	helprb.BitBlt(pDC->GetSafeHdc(),iWidth-helprb.GetWidth(),iHeight-helprb.GetHeight());

	CGameListWnd* pGameListWnd = GetMainRoom()->m_pGameListWnd;
	if (pGameListWnd->m_UserTabDuWnd.GetHWND())
	{
		if(pGameListWnd->GetIsHide())
		{
			pGameListWnd->m_UserTabDuWnd.ShowWindow(SW_HIDE);
		}
		else
		{
			pGameListWnd->m_UserTabDuWnd.ShowWindow(SW_SHOW);
		}

	}


	//drawSignal();
	return;
}

////�б�����չ��
//void CMainRoomEx::OnTvnItemexpandingGameList(NMHDR * pNMHDR, LRESULT * pResult)
//{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//	if (pNMTreeView->action==TVE_EXPAND)
//	{
//		CAFCGameListItem * pItem=(CAFCGameListItem *)m_GameList.GetItemData(pNMTreeView->itemNew.hItem);
//		if ((pItem!=NULL)&&(pItem->m_uDataType==GLK_GAME_NAME))///����Ϸ�ķ���
//		{
//			CAFCNameItem * pNameItem=static_cast<CAFCNameItem *>(pItem);
//			if ((pNameItem!=NULL)&&(pNameItem->IsNeedUpdate())
//				&&(AddMission(MS_GET_ROOM,pNameItem->m_NameInfo.uKindID,pNameItem->m_NameInfo.uNameID)))
//			{
//				pNameItem->m_uUpdateTime=(LONG)time(NULL);
//			}
//		}
//	}
//	*pResult=0;
//	return;
//}
//��������
bool CMainRoomEx::HandleMission()
{
	if (m_Mission[0].bAccess==true)
	{
		switch (m_Mission[0].uMissionID)
		{
		case MS_LOGON:		//���͵�½��Ϣ
			{
				SendUserInfo();////�����û���Ϣ
				DeleteMission();
				
				return true;
			}
		case MS_GET_ROOM:	//��ȡ��Ϸ�б�
			{
				
				MSG_GP_SR_GetRoomStruct GetList;
				GetList.uKindID=m_Mission[0].uKindID;
				GetList.uNameID=m_Mission[0].uNameID;
				SendData(&GetList,sizeof(GetList),MDM_GP_LIST,ASS_GP_LIST_ROOM,0);
				DeleteMission();
				return true;
			}
		}
	}
	//���öϿ�ʱ��
	SetTimer(ID_CHECK_SEND,CHECK_TIME,NULL);
	return false;
}

LRESULT CMainRoomEx::OnCloseGameRoomWithMsg(WPARAM wparam, LPARAM lparam)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString strMessage;
	strMessage = fMsg.GetKeyVal("MainRoom","PlayerQuitGameOver","��������ͬ������˳���Ϸ�����ֽ�����");
	
	//AFCMessageBox(strMessage);
    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,strMessage);
	return 0;
}
//�رշ�������
LRESULT CMainRoomEx::OnCloseGameRoom(WPARAM wparam, LPARAM lparam)
{

	RoomInfoStruct * pRoomInfo=(RoomInfoStruct *)wparam;
	if ((pRoomInfo!=NULL)&&(pRoomInfo->bAccess)&&(pRoomInfo->pGameRoomWnd!=NULL)&&(IsWindow(pRoomInfo->pGameRoomWnd->GetSafeHwnd())))
	{

		if(m_pGameListWnd != NULL)
			m_pGameListWnd->m_GameList.IEnterARoomOrLeave(pRoomInfo->pComRoomInfo.uRoomID,false);//�رշ���

		CNewGameClass * pRoomTab=&((CGamePlaceDlg *)AfxGetMainWnd())->m_RoomTabWnd;

		if(pRoomTab != NULL)
		{
			pRoomTab->ChangeSkin(m_skinmgr.GetSkinBcfFileName(),
				m_skinmgr.GetKeyVal(m_skinmgr.GetSkinBcfFileName()),
				m_skinmgr.GetSkinPath());
			pRoomTab->RemoveTabPage(pRoomInfo->uTabIndex);
		}

		// �˳�����ʱӦ�����ٷ��䴰��
		memset(pRoomInfo,0,sizeof(RoomInfoStruct));

// 		if (pRoomInfo)
// 		{
// 			delete pRoomInfo;//zht 2011-10-26 �˳���������
// 		}
 		pRoomInfo = NULL;


		BZSoundPlay(this, "music/�ر���.mp3", 0);

		if(m_PlaceUserInfo.dwTimeIsMoney > 0)
		{
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
			CString strMessage;
			TCHAR szMsg[MAX_PATH];
			TCHAR szNum[128]; 

            GetNumString(szNum, m_PlaceUserInfo.dwTimeIsMoney, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);

			strMessage = fMsg.GetKeyVal("MainRoom","GiftGoldToBank","���ε�¼ʱ���������͵Ľ����[%s],�ѵ�����������,�����!");
			wsprintf(szMsg,strMessage,szNum);
			
			//AFCMessageBox(szMsg);
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,szMsg);
		}
	}

	OutputDebugString("DClient:CMainRoomEx::OnCloseGameRoom3");

	GetLastLoginGame();


	//m_pGameListWnd->OnBnClickedBt1();//PostMessage(WM_COMMAND,MAKELONG(BN_CLICKED,IDC_BTN_LIST1),0);
	return 0;
}

//���������Ľ�����Ϸ���� add by lxl 2010-10-08
bool CMainRoomEx::EnterRoom(CAFCRoomItem * pGameRoomItem)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;
	CString strTitle;
	strTitle = fMsg.GetKeyVal("MainRoom","MessageTitle","ϵͳ��Ϣ");

	if ((pGameRoomItem==NULL)||(pGameRoomItem->m_uDataType!=GLK_GAME_ROOM))
	{
		str = fMsg.GetKeyVal("MainRoom","SelectGameRoom","����ѡ����Ϸ����");
		
		//AFCMessageBox(str,strTitle);//,MB_ICONINFORMATION,this);
        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,strTitle,false,str);
		return false;
	}

	//�ж��Ƿ����ⷿ��
	ComRoomInfo * pComRoomInfo=&pGameRoomItem->m_RoomInfo;
	if (((pComRoomInfo->uComType!=TY_NORMAL_GAME)&&(pComRoomInfo->uComType!=TY_MATCH_GAME)
		&&(pComRoomInfo->uComType!=TY_MONEY_GAME)))
	{
		str = fMsg.GetKeyVal("MainRoom","DownloadNewComponent",
			"�������������µĹ���ʵ��������������,Ȼ��װ.�Ƿ��������أ�");
		//if(IDOK==AFCMessageBox(str,strTitle,AFC_YESNO))//,MB_ICONQUESTION|MB_YESNO,this)==IDYES)
		if(IDYES==DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,strTitle,true,str))
		{
			ShellExecute(NULL,TEXT("open"),TEXT(Glb().m_CenterServerPara.m_strDownLoadUpdatepADDR),NULL,NULL,SW_MAXIMIZE);
		}
		CloseSocket (true);
		AfxGetApp( )->m_pMainWnd->DestroyWindow ();
		return true;		
		/*if (AFCMessageBox(TEXT("���İ汾���ɣ����ܽ������Ϸ�������Ƿ��������أ�"),MESSAGE_TITLE,MB_ICONQUESTION|MB_YESNO,this)==IDYES)
		{
		//�����ļ�
		CString strBuffer;
		strBuffer="http://www.kuaile.net/down/games.exe";
		//ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		PostQuitMessage(0);
		}
		return false;*/
	}

	//Ѱ����Ϸ����
	CString strMessage;
	BYTE m_bNowRoomIndex=255;
	CNewGameClass * pRoomTab=&((CGamePlaceDlg *)AfxGetMainWnd())->m_RoomTabWnd;

	bool bInMoneyRoom = false; 
	for (BYTE i=0;i<sizeof(m_RoomInfo)/sizeof(m_RoomInfo[0]);i++)
	{
		//////////////////
		//wushuqun 2009.5.26
		//���Ѿ�������Ϸʱ����ֹ˫����Ϸ�б�ر���Ϸ
		//already in same game room process
		if(pGameRoomItem->m_RoomInfo.uRoomID==m_RoomInfo[i].pComRoomInfo.uRoomID)
		{
			if (m_RoomInfo[i].pComRoomInfo.uComType != TY_MATCH_GAME ||
                (m_RoomInfo[i].pComRoomInfo.dwRoomRule & GRR_QUEUE_GAME))   // PengJiLin, 2010-9-19, �����Ŷӻ���
			{
				pRoomTab->SetActivePageByExtIndex(m_RoomInfo[i].uTabIndex);
			}
			return true;
		}
		////////////////////
		if(m_RoomInfo[i].bAccess && m_RoomInfo[i].pComRoomInfo.uComType == TY_MONEY_GAME)//�Ƿ��Ѿ������ҳ�
			bInMoneyRoom = true;
		if (m_RoomInfo[i].bAccess&&(m_RoomInfo[i].pComRoomInfo.uRoomID==pComRoomInfo->uRoomID))
		{
			
			if (m_RoomInfo[i].pComRoomInfo.uComType != TY_MATCH_GAME || 
                (m_RoomInfo[i].pComRoomInfo.dwRoomRule & GRR_QUEUE_GAME))   // PengJiLin, 2010-9-19, �����Ŷӻ���
			{
				pRoomTab->SetActivePageByExtIndex(m_RoomInfo[i].uTabIndex);
			}
			m_RoomInfo[i].pGameRoomWnd->PostMessage(WM_BRING_GAMEWND_TO_TOP,0,0);
			return true;
		}
		else if (m_RoomInfo[i].bAccess==false) 
		{
			m_bNowRoomIndex=((m_bNowRoomIndex==255)?i:m_bNowRoomIndex);
		}
	}
// 	if (m_bNowRoomIndex==255)
// 	{
// 		//CloseAllGameRoom();
// 		//�رշ���
// 		str = fMsg.GetKeyVal("MainRoom","QuitBeforeChangeRoom",
// 			"��������Ϸ������,Ҫ������,�����˳�,�ٻ���Ϸ����!");
// 		AFCMessageBox(str,strTitle);//,MB_ICONQUESTION,this);
// 		return false;
// 	}
	//��ҳ�ֻ�ܽ�һ��
// 	if(pComRoomInfo->uComType == TY_MONEY_GAME && bInMoneyRoom == true)
// 	{
// 		str = fMsg.GetKeyVal("MainRoom","CoinRoomAllowOne","��Ϸ�ҳ�ֻ�ܽ�һ��!");
// 		AFCMessageBox(str,strTitle);//,MB_ICONQUESTION,this);
// 		return false;
// 	
// 	}
	//�ж���Ϸ�Ƿ�װ��Ч��汾
	CFileVersion FileVer;
	bool needupdate=false;
	TCHAR szProessName[31],szGameName[61];

	//wushuqun 2009.5.21
	memset(szProessName,0,sizeof(szProessName));
	memset(szGameName,0,sizeof(szGameName));

	m_pGameListWnd->m_GameList.GetGameName(pGameRoomItem,szGameName,sizeof(szGameName));
	m_pGameListWnd->m_GameList.GetProcessName(pGameRoomItem,szProessName,sizeof(szProessName));
	//{{Modified by zxd ��ȷ�ж���Ϸ�����ļ���

	TCHAR szIcoName[MAX_PATH];
	int nLen = _tcslen(szProessName);
	if ((szProessName[nLen-3] == TCHAR('e') || szProessName[nLen-3] == TCHAR('E'))
		&&(szProessName[nLen-2] == TCHAR('x') || szProessName[nLen-2] == TCHAR('X'))
		&&(szProessName[nLen-1] == TCHAR('e') || szProessName[nLen-1] == TCHAR('E'))
		)
	{
		_tcscpy(szIcoName, CBcfFile::GetAppPath());
		_tcscat(szIcoName, szProessName);
		// ������ǰ��һ�θĳ�Ŀ¼�����ټ����ļ���
		nLen = _tcslen(szIcoName);
		szIcoName[nLen-4] = TCHAR('\\');
		szIcoName[nLen-3] = TCHAR('\0');
		_tcscat(szIcoName, szProessName);	//�ٸ����ļ���

		//��U3d��Ϸ�����ð�".exe"�ĳ�".ico"
		if (!(pGameRoomItem->m_RoomInfo.dwRoomRule & GRR_GAME_U3D))
		{
			nLen = _tcslen(szIcoName);
			szIcoName[nLen-3] =TCHAR('i');
			szIcoName[nLen-2] =TCHAR('c');
			szIcoName[nLen-1] =TCHAR('o');
		}
	}
	else
	{
		_tcscpy(szIcoName, szProessName);
	}
	//DebugPrintf(szIcoName);
	
	//�жϴ���Ϸ�Ƿ����ڸ���
	try
	{
		POSITION pos;
		::EnterCriticalSection(&m_Section);
		//pos=m_UpdateList.GetHeadPosition ();	
		for(vector<LPUPDATE>::iterator it = m_UpdateList.begin(); it != m_UpdateList.end();it++)
		{
			LPUPDATE lp = *it;
			if(!lp) continue;
			if(lp->uNameID == pGameRoomItem->m_RoomInfo.uNameID)
			{
				//if(::WaitForSingleObject(lp->hProcess,0)==WAIT_TIMEOUT)
				{
					::LeaveCriticalSection(&m_Section);
					str = fMsg.GetKeyVal("MainRoom","EnterAfterUpdate","����Ϸ���ڸ���, �����ڸ�������ٽ�����Ϸ");
					
					//AFCMessageBox(str,szGameName);
                    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,szGameName,false,str);
					return false;
				}
/*				else
				{
					::CloseHandle(lp->hProcess);
					it = m_UpdateList.erase(it);
					m_UpdateList.RemoveAt(temp);
				}	*/		
			}	
		}
		::LeaveCriticalSection(&m_Section);
	}
	catch(...)
	{
		::LeaveCriticalSection(&m_Section);
	}

	if (FileVer.Open(szIcoName)==false)
		//if (FileVer.Open(szProessName)==false)
		//Modified by zxd}}
	{			

		str = fMsg.GetKeyVal("MainRoom","OutOfRoomBeforeDownload",
			"�������� %s��Ϸ,��ȷ��û�н������Ϸ����!  ");
		strMessage.Format(str,szGameName);
		//if(AFCMessageBox(TEXT(strMessage),MESSAGE_TITLE, AFC_YESNO)==IDOK)//,MB_ICONQUESTION|MB_YESNO,this)==IDYES)
		if(DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,MESSAGE_TITLE,true,strMessage)==IDYES)
		{
			//wushuqun 2009.5.21
			needupdate = true;
			try
			{     
				StartDownLoad(szProessName,pGameRoomItem);
			}
			catch(CException* e)
			{
				str = fMsg.GetKeyVal("MainRoom","DownloadError","���ط������쳣��");
				AfxMessageBox(str);
				e->Delete();
				return false;
			}
			catch(...)
			{
				str = fMsg.GetKeyVal("MainRoom","DownloadError","���ط������쳣��");
				AfxMessageBox(str);
				return false;
			}
			/// CloseSocket (true); ///< ����ʱ����Ҫ�Ͽ���Z�����������ӣ�zxdע����20100315
			return true;
		}
		else
		{
			return false;
		}
	}

	//����update���и���
	TCHAR szApp[61];//,sz[100];
	TCHAR path[MAX_PATH];
	lstrcpy(szApp,szProessName);
	szApp[lstrlen(szApp)-lstrlen("ico")-1]='\0';
	wsprintf(path,"%s\\%s\\update.ini",GetAppPath().c_str(),szApp);
	CINIFile f(path);
	CString m_key="config";
	CString v,s,ul,uv,n;
	int sm;
	//wushuqun 2009.5.20
	//�汾����Ĭ��ֵ����Ȼ������update.exe ����
	CString strOut;
	str = fMsg.GetKeyVal("MainRoom","GameNameOfUpdate","��Ϸ�� %s,%s,update�����ļ� %s");
	strOut.Format(str,szGameName,szProessName,path);
	::OutputDebugString(strOut);

	v=f.GetKeyVal(m_key,"-v","0");//�汾	
	s=f.GetKeyVal(m_key,"-s","./");//·��
	ul=f.GetKeyVal(m_key,"-ul","");//�����б��ַ	
	uv=f.GetKeyVal(m_key,"-uv","");//���°汾��ַ
	n=f.GetKeyVal(m_key,"-n","��Ϸ����");//��Ϸ����	
	sm=f.GetKeyVal(m_key,"-sm",0);//��ʾ��ʽ
	bool getver=false;
	TCHAR ser[20];
	wsprintf(ser,"%u.%u",pComRoomInfo->uVer>>16,pComRoomInfo->uVer&0x0000ffff);


	if (ul =="")
	{
		ul.Format("%s/update_version.txt",szApp);
		uv = ul;
	}

#ifndef MY_DEV
	//wushuqun 2009.5.20
	if(v!="" && ! needupdate)

	{//�����汾

		getver=true;
		//if(strcmp(ser,v.GetBuffer())!=0)
		if (CheckUpdate(szProessName, pGameRoomItem))
		{
			str = fMsg.GetKeyVal("MainRoom","UpdateNewGame",
				"��Ϸ���и��°汾, ����Ҫ������Ϸ, �Ƿ����ڸ���?");
			//if(AFCMessageBox(str,szGameName,AFC_YESNO)==IDOK)
			if(DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,szGameName,true,str)==IDYES)
				needupdate=true;
			else
			{
				needupdate=false;
				return false;
			}
		}
	}
	//wushuqun 2009.5.20
	if(!getver && ! needupdate)
	{
		str = fMsg.GetKeyVal("MainRoom","UnknowVersionReinstall",
			"�޷���֪��ǰ��Ϸ�汾,���°�װ����Ϸ���ܻ��޸�������\n�Ƿ���������?");
//		s.Format("�޷���֪��ǰ��Ϸ�汾,���°�װ����Ϸ���ܻ��޸�������\n�Ƿ���������?");
		//if(AFCMessageBox(str,szGameName,AFC_YESNO)==IDOK)
		if(DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,szGameName,true,str)==IDYES)
			SendMessage(WM_COMMAND,MAKELONG(IDM_DOWN_NEW,0),0);
		return false;
	}
	/*if(!getver)
	{
		str = fMsg.GetKeyVal("MainRoom","UnknowVersionReinstall",
		"�޷���֪��ǰ��Ϸ�汾,���°�װ����Ϸ���ܻ��޸�������\n�Ƿ���������?");
	//	s.Format("�޷���֪��ǰ��Ϸ�汾,���°�װ����Ϸ���ܻ��޸�������\n�Ƿ���������?");
		if(AFCMessageBox(str,szGameName,AFC_YESNO)==IDOK)
		SendMessage(WM_COMMAND,MAKELONG(IDM_DOWN_NEW,0),0);
		return false;
	}
	else*/ 

	if(needupdate)
	{
		try
		{
			::EnterCriticalSection(&m_Section);
			if(m_UpdateList.size() >= 64)
			{
				::LeaveCriticalSection(&m_Section);
				str = fMsg.GetKeyVal("MainRoom","TooManyUpdateTryLater","���ڸ��µ���Ϸ̫��,�������.");
	//			s.Format("���ڸ��µ���Ϸ̫��,�������.");
				
				//AFCMessageBox(str,szGameName);
                DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,szGameName,false,str);
				return false;
			}
			else
			{
				::LeaveCriticalSection(&m_Section);
			}
		}
		catch(...)
		{
			::LeaveCriticalSection(&m_Section);
		}
		STARTUPINFO StartupInfo;
		ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
		StartupInfo.cb = sizeof(STARTUPINFO);
		PROCESS_INFORMATION ProcessInfo;
		char up[1000];
		//wsprintf(up," -n %s -s %s -v %s -ul %s%s -uv %s%s -sm %d",n,s,v,Glb().m_UpdateUrl,ul,Glb().m_UpdateUrl,uv,sm);
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		bool setup;

		setup=true;
		hFind = FindFirstFile(path, &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE) 
		{

		} 
		else 
		{
			setup=false;
			FindClose(hFind);
		}

		//wushuqun 2009.5.21
		//��ԭ��ԭ�и��·�ʽ
		/*if(setup)
			wsprintf(up," -u %s//%s//update_version.txt -l %s -v %s -n",Glb().m_UpdateUrl,szApp,path,ser);
		else wsprintf(up," -u %s//%s//update_version.txt -l %s -v %s",Glb().m_UpdateUrl,szApp,path,ser);*/


		string _path;
		_path = GetAppPath() + "\\" + szApp;
		sprintf(up, "-n %s -s %s\\ -v %s -ul %s%s -uv %s%s -sm %d", n, _path.c_str(), v, Glb().m_UpdateUrl, ul,Glb().m_UpdateUrl,uv, 1);
		//wsprintf(up," -n %s -s %s -v %s -ul %s%s -uv %s%s -sm %d",n,s,v,Glb().m_UpdateUrl,ul,Glb().m_UpdateUrl,ul,sm);
		//OutputDebugString(up);

		//2012.08.22 yyf ����
		//	if(!CreateProcess(TEXT("update.exe"), up,0,0,FALSE,0,0,0,&StartupInfo,&ProcessInfo))
		//	{
		////		TCHAR sz[200];
		////		wsprintf(sz,"������������ʧ��. ���°�װ�������ܻ��޸�������.");
		//		str = fMsg.GetKeyVal("MainRoom","UpdateErrorReinstall",
		//			"������������ʧ��. ���°�װ�������ܻ��޸�������.");
		//		AFCMessageBox(str,szGameName);
		//	}

		////-ͬ������- update.exe -------------------------2012.08.24 yyf
		////if(!m_AnonymousPipe.OpenProcess("update.exe"))
		//if(!m_CSocket32.OpenProcess("update.exe"))
		//{ 
		//	str = fMsg.GetKeyVal("MainRoom","UpdateErrorReinstall",
		//		"������������ʧ��. ���°�װ�������ܻ��޸�������.");
		//	AFCMessageBox(str,szGameName); 
		//} 
		////else if(m_AnonymousPipe.GetProcessIdFromName("update.exe"))
		//else if(m_CSocket32.GetProcessIdFromName("update.exe"))
		//{
		//	UpdateStructMsg updateMsg;
		//	memset(&updateMsg,0,sizeof(UpdateStructMsg));
		//	 
		//	updateMsg.command = 1;
		//	updateMsg.nRoomID = pGameRoomItem->m_RoomInfo.uNameID;
		//	wsprintf(updateMsg.g_GameName,"%s",szGameName);
		//	wsprintf(updateMsg.g_url,"%s%s",Glb().m_UpdateUrl,uv);
		//	wsprintf(updateMsg.g_ver,"%s",v); 
		//	  
		//	DWORD dwWrite=0;
		//	//m_AnonymousPipe.Write( &updateMsg ,sizeof(UpdateStructMsg),dwWrite);
		//	m_CSocket32.Send(&updateMsg ,sizeof(UpdateStructMsg));

		//	UPDATE *lp=(LPUPDATE)malloc(sizeof(UPDATE));
		//	lp->hProcess=ProcessInfo.hProcess;
		//	lp->uNameID=pGameRoomItem->m_RoomInfo.uNameID;
		//	lp->pGameRoomItem=pGameRoomItem;

		//	try
		//	{
		//		::EnterCriticalSection(&m_Section);
		//		if(m_UpdateList.size()<64)
		//			m_UpdateList.push_back(lp);
		//		//if(m_UpdateList.GetCount()==1)
		//		if(needupdate)
		//			SetTimer(CHECK_UPDATE,2000,NULL);
		//		::LeaveCriticalSection(&m_Section);
		//	}
		//	catch(...)
		//	{
		//		::LeaveCriticalSection(&m_Section);
		//	}
		//}
		////-------------------------------------------------------------

		//-�첽����- update.exe -------------------------2012.08.24 yyf
		//��ʼ�� sDownLoadUpdateMsg Ȼ���ٴ��� ���ظ����߳�   DownLoadUpdate(LPVOID lparam)
		sDownLoadUpdateMsg sUpdateMsg; 
		//-----------------
		UpdateStructMsg updateMsg;
		memset(&updateMsg,0,sizeof(UpdateStructMsg));
		updateMsg.command = 1;
		updateMsg.nRoomID = pGameRoomItem->m_RoomInfo.uNameID;
		wsprintf(updateMsg.g_GameName,"%s",szGameName);
		wsprintf(updateMsg.g_url,"%s%s",Glb().m_UpdateUrl,uv);
		wsprintf(updateMsg.g_ver,"%s",v); 
		sUpdateMsg.pUpdateStructMsg = &updateMsg;
		//-----------------
		UPDATE *lp=(LPUPDATE)malloc(sizeof(UPDATE));
		lp->hProcess=ProcessInfo.hProcess;
		lp->uNameID=pGameRoomItem->m_RoomInfo.uNameID;
		lp->pGameRoomItem=pGameRoomItem;
		sUpdateMsg.pUPDATE = lp;
		//-----------------
		sUpdateMsg.pCMainRoomEx = this;
		//-----------------
		sUpdateMsg.pCBcfFile = &fMsg;
		//-----------------

		DWORD dwThreadID = 0;
		HANDLE hThread = CreateThread(0,0,DownLoadUpdate,&sUpdateMsg,0,&dwThreadID);

		if (WaitForSingleObject(hThread,5000)==WAIT_TIMEOUT) 
		{//��ʱ update.exe ��û������
			if(m_CSocket32.GetProcessInfo().hProcess)
			{
				::TerminateProcess( m_CSocket32.GetProcessInfo().hProcess, 0 );//�������� update.exe
			}
			m_CSocket32.Close();

			CString str;
			CString strTitle;
			str = fMsg.GetKeyVal("MainRoom","DownLoadOver","����ʧ��");
			strTitle = fMsg.GetKeyVal("MainRoom","MessageTitle","��ʾ");
			
			//AFCMessageBox(str,strTitle);
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,strTitle,false,str);
		}
		if (hThread)
		{
			::CloseHandle(hThread);
		}
		//----------------------------------------------------------

		return false;
	}
#endif
	//���ܴ��������ͬ��Ϸ�ķ���
	//JianGuankun 2011.12.28
	CGameRoomEx* pGameRoom = NULL;
	int iRoomOpened = 0;
	for (BYTE i = 0;i < MAX_GAME_ROOM; ++i)
	{
		pGameRoom = (CGameRoomEx*)m_RoomInfo[i].pGameRoomWnd;
		if(NULL != pGameRoom)
		{
			iRoomOpened++;
			if (pGameRoom->m_GameInfo.uNameID== pGameRoomItem->m_RoomInfo.uNameID)
			{
				CString strTip;
				strTip.Format("���Ѿ����롾%s���ķ��䣬ͬһ����Ϸֻ�ܽ���һ�����䡣",pGameRoom->m_GameInfo.szGameName);
				
				//AFCMessageBox(strTip);
                DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,strTip);
				return 0;
			}
		}
	}
	//End

	//�򿪷��������������ж�
	//JianGuankun 2011.12.28
	int iMaxRoomNum = CGamePlaceApp::GetModuleStatus("GameRoom","MaxOpenNums",1);

	if (iRoomOpened >= iMaxRoomNum)
	{
		CString strTip;
		strTip.Format("���ѽ�����%d����Ϸ���䣬�����ٽ������ķ���",iMaxRoomNum);
		
		//AFCMessageBox(strTip);
        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,strTip);
		return 0;
	}
	//End

	//�ж��Ƿ����ⷿ��
	//ComRoomInfo * pComRoomInfo=&pGameRoomItem->m_RoomInfo;
	///{{ Modified by lxl 2010-10-08	���뷿��ʱ�жϸ÷����Ƿ�������룬����У�����ʾ���������

	if(pGameRoomItem->m_RoomInfo.dwRoomRule & GRR_CONTEST)
	{
		CTime _t = CTime::GetCurrentTime();
		if (_t.GetTime() < pGameRoomItem->m_RoomInfo.i64TimeStart)
		{
			//AFCMessageBox("������û��ʼ");

// 			CString s;
// 			s.Format("%s/manage/matchapply.aspx?id=%d", TEXT(Glb().m_CenterServerPara.m_strWebRootADDR),pGameRoomItem->m_RoomInfo.uContestID);
// 			GetMainRoom()->IE(s);

			//return false;
		}
// 		if (_t.GetTime() > pGameRoomItem->m_RoomInfo.i64TimeEnd)
// 		{
// 			AFCMessageBox("�����Ѿ�����");
// 		}
	}

	if(pGameRoomItem->m_RoomInfo.iLeaseID != 0)
	{
		CTime _t = CTime::GetCurrentTime();
		if (_t.GetTime() > pGameRoomItem->m_RoomInfo.i64LeaseTimeEnd)
		{
			//AFCMessageBox("���������Ѿ�����");
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"���������Ѿ�����");
			return false;
		}
	}

	if (pComRoomInfo->bHasPassword)
	{
		/// ��������Ի���
		CPassDesk pdDlg;
		if(pdDlg.DoModal()==IDCANCEL)
			return 0;

		char szSrcPass[MAX_PATH];
		strcpy(szSrcPass, pdDlg.m_pass.GetBuffer());

		/// �����ַ���
		MD5_CTX Md5;
		Md5.MD5Update((unsigned char *)szSrcPass,lstrlen(szSrcPass));
		unsigned char szMDTemp[16];
		Md5.MD5Final(szMDTemp);
		for (int i=0;i<16;i++) wsprintf(&szSrcPass[i*2],"%02x",szMDTemp[i]);

		/// �ύ�����������
		MSG_GP_CheckRoomPasswd crp;
		crp.uRoomID = pComRoomInfo->uRoomID;
		strcpy(crp.szMD5PassWord, szSrcPass);

		SendData(&crp,sizeof(MSG_GP_CheckRoomPasswd),MDM_GP_LIST, ASS_GP_ROOM_PASSWORD,0);
	}
	//������Ϸ����
	RoomInfoStruct * pRoomInfoItem = &m_RoomInfo[m_bNowRoomIndex];
	try
	{
		pRoomInfoItem->bAccess=true;
		pRoomInfoItem->pComRoomInfo=*pComRoomInfo;
		pRoomInfoItem->pRoomItem = pGameRoomItem;
		lstrcpy(pRoomInfoItem->szProcessName,szProessName);
		FileVer.GetFixedProductVersion(pRoomInfoItem->dwGameMSVer,pRoomInfoItem->dwGameLSVer);
		////�ұ�
		pRoomInfoItem->pRoomInfoWnd=new CRoomRightWnd(IDD_ROOM_RIGHT_WND);//szGameName,pRoomInfoItem,&m_PlaceUserInfo);
		
		if(pRoomInfoItem->pRoomInfoWnd->Create(IDD_ROOM_RIGHT_WND,pRoomTab)==FALSE)
		{
			str = fMsg.GetKeyVal("MainRoom","InstallComponentError","����A�����ʧ��");
			throw str;
		}
		

		pRoomInfoItem->pGameRoomWnd=new CGameRoomEx(szGameName,pRoomInfoItem,&m_PlaceUserInfo,pRoomInfoItem->pRoomInfoWnd);
		pRoomInfoItem->pRoomInfoWnd->m_pMainWnd=(CWnd*)pRoomInfoItem->pGameRoomWnd;
		pRoomInfoItem->pRoomInfoWnd->SetGameRoomWnd((CWnd*)pRoomInfoItem->pGameRoomWnd);

        // PengJiLin, 2010-9-13, ���˿������߿�ʹ��ʱ�����ù���
        CGameRoomEx* pGameRoom = (CGameRoomEx*)pRoomInfoItem->pGameRoomWnd;
        pGameRoom->SetKickPropTime(m_iKickPropTotalTime);
        pGameRoom->SetAntiKickPropTime(m_iAntiKickPropTotalTime);


		if (pRoomInfoItem->pGameRoomWnd->Create(IDD_GAME_ROOM,pRoomTab)==FALSE) 
		{
			str = fMsg.GetKeyVal("MainRoom","InstallComponentError","����A�����ʧ��");
			throw str;
		}

		pRoomInfoItem->pRoomInfoWnd->ShowWindow(SW_SHOW);

        // PengJiLin, 2010-9-20, ����Ǳ�����������û���Ŷӻ��������Ҳ�ؼ�
        if(pRoomInfoItem->pComRoomInfo.uComType == TY_MATCH_GAME &&
           !(pRoomInfoItem->pComRoomInfo.dwRoomRule & GRR_QUEUE_GAME))
        {
            pRoomInfoItem->pRoomInfoWnd->ShowWindow(SW_HIDE);
        }

		AfxSetResourceHandle(GetModuleHandle(NULL));
		
		//wushuqun 2009.5.20
		//ˢ�±���
		//pRoomInfoItem->pRoomInfoWnd->Invalidate(false);

	}
	catch (...)
	{
		//������Ϣ
		delete pRoomInfoItem->pRoomInfoWnd;
		delete pRoomInfoItem->pGameRoomWnd;
		memset(pRoomInfoItem,0,sizeof(RoomInfoStruct));
		return false; 
	}

	//���½���
	HICON hRoomIcon=CPlaceResource::GetRoomIcon(pGameRoomItem,szProessName);
	if (pRoomInfoItem->pComRoomInfo.uComType != TY_MATCH_GAME ||
        (pRoomInfoItem->pComRoomInfo.dwRoomRule & GRR_QUEUE_GAME))   // PengJiLin, 2010-9-19, �����Ŷӻ���
	{
		pRoomInfoItem->uTabIndex=pRoomTab->AddTabPage(m_RoomInfo[m_bNowRoomIndex].pGameRoomWnd,
			m_RoomInfo[m_bNowRoomIndex].pRoomInfoWnd,szGameName,hRoomIcon,true,true);
		pRoomTab->SetActivePageByExtIndex(pRoomInfoItem->uTabIndex);
	}
	else
	{
		pRoomInfoItem->uTabIndex=pRoomTab->AddTabPage(m_RoomInfo[m_bNowRoomIndex].pGameRoomWnd,
			m_RoomInfo[m_bNowRoomIndex].pRoomInfoWnd,szGameName,hRoomIcon,false,false);
	}
	/// ������Ŷӻ����䣬��ر�������¼���������������¼�
	pRoomTab->EnableLBClick((pRoomInfoItem->pComRoomInfo.dwRoomRule & GRR_QUEUE_GAME) == 0);
	/////pComRoomInfo
	m_pGameListWnd->m_GameList.IEnterARoomOrLeave(pComRoomInfo->uRoomID,true);
	if(m_bShowSysmsg)
	{
		m_bShowSysmsg = false;
		// ���������߳�
		::CreateThread(NULL,0,DownLoadNewIni,this,0,NULL);
		return true;
	}
	if(!m_bShowSysmsg)
		WriteNews();
	return true;
}

bool CMainRoomEx::OnCentranceGameRoom(CAFCRoomItem * pGameRoomItem)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;
	CString strTitle;
	strTitle = fMsg.GetKeyVal("MainRoom","MessageTitle","ϵͳ��Ϣ");

	if ((pGameRoomItem==NULL)||(pGameRoomItem->m_uDataType!=GLK_GAME_ROOM))
	{
		str = fMsg.GetKeyVal("MainRoom","SelectGameRoom","����ѡ����Ϸ����");
		
		//AFCMessageBox(str,strTitle);//,MB_ICONINFORMATION,this);
        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,strTitle,false,str);
		return false;
	}

	////�ж��Ƿ����ⷿ��
	//ComRoomInfo * pComRoomInfo=&pGameRoomItem->m_RoomInfo;

	/////{{ Modified by lxl 2010-10-08	���뷿��ʱ�жϸ÷����Ƿ�������룬����У�����ʾ���������
	//if (pComRoomInfo->bHasPassword)
	//{
	//	/// ��������Ի���
	//	CPassDesk pdDlg;
	//	if(pdDlg.DoModal()==IDCANCEL)
	//		return 0;

	//	char szSrcPass[MAX_PATH];
	//	strcpy(szSrcPass, pdDlg.m_pass.GetBuffer());

	//	/// �����ַ���
	//	MD5_CTX Md5;
	//	Md5.MD5Update((unsigned char *)szSrcPass,lstrlen(szSrcPass));
	//	unsigned char szMDTemp[16];
	//	Md5.MD5Final(szMDTemp);
	//	for (int i=0;i<16;i++) wsprintf(&szSrcPass[i*2],"%02x",szMDTemp[i]);

	//	/// �ύ�����������
	//	MSG_GP_CheckRoomPasswd crp;
	//	crp.uRoomID = pComRoomInfo->uRoomID;
	//	strcpy(crp.szMD5PassWord, szSrcPass);

	//	SendData(&crp,sizeof(MSG_GP_CheckRoomPasswd),MDM_GP_LIST, ASS_GP_ROOM_PASSWORD,0);

	//	return 1;
	//}
	//else
	//{
		return 	EnterRoom(pGameRoomItem);
	//}
}

void CMainRoomEx::BeforeNavigate2Explorer(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	if(URL->vt!=VT_BSTR)
		return;
	CString surl=(char*)_bstr_t(URL->bstrVal);
	surl.MakeLower();

	if(surl.Left(6)=="bzw://")
	{
		*Cancel=TRUE;
		if(surl.Find("avatar/change")==6)
		{
			UINT uNewLogoID = atoi(GetUrlValue(surl,"newlogoid"));
			__int64 i64BankChangeMoney = atoi(GetUrlValue(surl,"price"));
			m_pGameListWnd->m_PlaceUserInfo->i64Bank -= i64BankChangeMoney;

			//��������
			UpdateBankMoneyToAllRoom(m_pGameListWnd->m_PlaceUserInfo->i64Bank,NULL);
			if(m_pBankWndEx)
			{
				m_pBankWndEx->SetWealthInfor(m_pGameListWnd->m_PlaceUserInfo->i64Bank,0);
			}

			//��������
			ProcessMyAvatarChange(uNewLogoID);
		}
	}
}

void CMainRoomEx::NavigateErrorExplorer(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Frame, VARIANT* StatusCode, BOOL* Cancel)
{
	//this->ShowWindow(SW_HIDE);
	//CMainRoomEx *pWnd=(CMainRoomEx*)pParentWnd;
	//for(int i = 0; i < MAX_GAME_ROOM; i ++)
	//{
	//	if(pWnd->m_RoomInfo[i].pGameRoomWnd != NULL)
	//	{
	//		((CGameRoomEx*)pWnd->m_RoomInfo[i].pGameRoomWnd)->InsertSystemMessageWithGame("�Բ��𣬴򿪵��������");
	//	}
	//}
}

//��������仯��JianGuankun 2012.06.26
void CMainRoomEx::ProcessMyAvatarChange(UINT uNewLogoID)
{
	m_pGameListWnd->m_PlaceUserInfo->bLogoID = uNewLogoID;
	m_pGameListWnd->SetUserInfoWnd(m_pGameListWnd->m_PlaceUserInfo);

	//��Ϸ���������
	for (BYTE i = 0; i < MAX_GAME_ROOM; ++i)
	{
		// ��Ϸ����Ϸ�����ұ���Ϣ��
		CGameRoomEx* pGameRoomEx = (CGameRoomEx*)m_RoomInfo[i].pGameRoomWnd;

		if (NULL != pGameRoomEx)
		{
			DWORD dwUserID = m_pGameListWnd->m_PlaceUserInfo->dwUserID;

			//���·�������
			pGameRoomEx->OnAvatarChange(dwUserID,uNewLogoID);

			MSG_GR_AvatarChange tagAvatarChange;
			tagAvatarChange.dwUserID = dwUserID;
			tagAvatarChange.nNewLogoID = uNewLogoID;
			pGameRoomEx->SendData(&tagAvatarChange,sizeof(MSG_GR_AvatarChange),MDM_GR_ROOM,ASS_GR_AVATAR_LOGO_CHANGE,0);
		}	
	}

	return;


}

//�б���Ϣ
bool CMainRoomEx::OnListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	//����Ƿ��ؽ������뷿��ɹ�����Ϣ������뷿�䣬����ԭ�з�ʽ���� add by lxl 2010-10-08
	if (ASS_GP_ROOM_PASSWORD == pNetHead->bAssistantID)
	{
		DL_CheckRoomPasswd *p = (DL_CheckRoomPasswd *)pNetData;
		if (p->bRet)
		{
			CAFCRoomItem * pGameRoomItem  = m_pGameListWnd->m_GameList.FindRoomItem(p->uRoomID);
			this->EnterRoom(pGameRoomItem);
		}
		else
		{
			//AFCMessageBox("�����������������ʱ���ܽ���÷�����Ϸ!");
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�����������������ʱ���ܽ���÷�����Ϸ!");
			CGameRoomEx* pGameRoom = GetGameRoom(p->uRoomID);
			if (pGameRoom)
			{
				pGameRoom->OnBnClickedClose();
			}
		}
		return TRUE;
	}
	//������������
	m_pGameListWnd->OnListMessage( pNetHead,  pNetData,  uDataSize,  pClientSocket);

	//�����Ϸ�б��������Ϸ�б���ʾ
	CString strPath = CBcfFile::GetAppPath();
	strPath += "\\ListTip.ini";
	UINT uNotify = GetPrivateProfileInt("IsNotify", "notify", 0, strPath);

	if (m_bListmessage && !bTipInit &&uNotify && m_HaveSellList)
	{
		bTipInit = true;
		m_pListTip = new CGameListTip();
		m_pListTip->DoModal();
	}
	m_bListmessage = true;


	return TRUE;

	//	switch (pNetHead->bAssistantID)
	//	{
	//	case ASS_GP_LIST_KIND:		//��ȡ������Ϸ����
	//		{
	//			//��������
	//			m_GameList.HandleKindData((ComKindInfo *)pNetData,uDataSize/sizeof(ComKindInfo));//�����ϼ�����Ϸ����
	//			m_GameList.ExpandItem(2);
	//			return true;
	//		}
	//	case ASS_GP_LIST_NAME:		//��ȡ����Ϸ����
	//		{
	//			//��������
	//			m_GameList.HandleNameData((ComNameInfo *)pNetData,uDataSize/sizeof(ComNameInfo));
	//
	//			//������ʾ
	//			if (pNetHead->bHandleCode==ERR_GP_LIST_FINISH) //������Ϸ�����б������־
	//			{
	//				m_bLogon=true;
	//				if (m_Connect.GetSafeHwnd()!=NULL) m_Connect.DestroyWindow();
	//
	//				m_GameList.FillGetListItem();
	//				m_GameList.UpdateOnLineCount();
	//				//m_GameList.ExpandItemFirstRoom();
	//				//m_GameList.joinlist();
	//				m_GameList.ExpandAll(false);
	//#ifdef	AI_MACHINE
	//				m_GameList.ExpandAll(true);
	//				SetTimer(ID_AI_ENTERROOM, 500, NULL);
	//#endif
	//				HandleMission();
	//			}
	//
	//			return true;
	//		}
	//	case ASS_GP_LIST_ROOM:		//��ȡ����Ϸ����
	//		{
	//
	//			//Ч������ 
	//			if (uDataSize<sizeof(MSG_GP_SR_GetRoomStruct)) return false;
	//			MSG_GP_SR_GetRoomStruct * pNetRoomBuf=(MSG_GP_SR_GetRoomStruct *)pNetData;
	//
	//			//��������
	//			m_GameList.HandleRoomData((ComRoomInfo *)((char *)pNetData+sizeof(MSG_GP_SR_GetRoomStruct)),
	//				(uDataSize-sizeof(MSG_GP_SR_GetRoomStruct))/sizeof(ComRoomInfo),pNetRoomBuf->uKindID,pNetRoomBuf->uNameID);
	//
	//
	//			if (pNetHead->bHandleCode==ERR_GP_LIST_FINISH) //������Ϸ�б������־
	//			{
	//				m_GameList.DeleteOutTimeRoom(pNetRoomBuf->uKindID,pNetRoomBuf->uNameID);
	//				HandleMission();
	//			}
	//
	//			m_GameList.UpdateOnLineCount();
	//			m_GameList.ExpandItem(2);//չ����Ŀ¼
	//			return true;
	//		}
	//	case ASS_GP_LIST_COUNT:		//��������
	//		{
	//			if (uDataSize<sizeof(DL_GP_RoomListPeoCountStruct)) return false;
	//
	//			if(pNetHead->bHandleCode == 0)
	//			{////������������
	//				m_GameList.HandleRoomOnLineData((DL_GP_RoomListPeoCountStruct *)pNetData,uDataSize/sizeof(DL_GP_RoomListPeoCountStruct));
	//				m_GameList.UpdateOnLineCount();
	//			}
	//			else
	//			{////��Ϸ��������
	//				m_GameList.HandleNameOnLineData((DL_GP_RoomListPeoCountStruct *)pNetData,uDataSize/sizeof(DL_GP_RoomListPeoCountStruct));
	//			}
	//
	//			//m_GameList.ExpandItem(2);
	//			return true;
	//		}
	//	}
	//	return true;
}


//����news.ini�ļ�
DWORD WINAPI CMainRoomEx::DownLoadNewIni(LPVOID lparam)
{
	CMainRoomEx * p =(CMainRoomEx *)lparam;
	CString s=CBcfFile::GetAppPath ();//����·��
	TCHAR szUrl[1000];
	CString NewUrl;
	CBcfFile f( s + "bzgame.bcf");
	CString key="BZW";
	NewUrl=f.GetKeyVal(key,"newurl","");
	if(NewUrl == "")
		return 0;
	lstrcpy(szUrl,NewUrl);
	CString name;
	if(InternetGetFile(szUrl,TEXT("news.ini"),name) == 0)
		p->WriteNews();
	return 0;
}
//֪ͨ������ʾ����
void CMainRoomEx::WriteNews()
{
	for (BYTE i=0;i<MAX_GAME_ROOM;i++)
	{
		if (m_RoomInfo[i].bAccess)
		{
			m_RoomInfo[i].pGameRoomWnd->SendMessage(WM_REFLASNEWS,0,0);
			m_RoomInfo[i].pGameRoomWnd->SendMessage(WM_REFLAS_SYS_MSG,0,0);
		}
	}
}

//�����ȡ��Ϣ
bool CMainRoomEx::OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	m_nKeepAliveCount = 0;
	//TRACE("%d GET Message MAINID:%d\n",GetTickCount(),pNetHead->bMainID);
	switch (pNetHead->bMainID)
	{
	 case MDM_GP_NETSIGNAL:
        {
            getNetSignal(pNetData);
            return true;
        }

	case MDM_CONNECT:	//������Ϣ 
		{
			return OnConnectMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}

	case MDM_GP_REQURE_GAME_PARA:	//���ķ���ȫ�ֲ���//������Ϸȫ�ֲ���
		{

			CenterServerMsg * msg=(CenterServerMsg *)pNetData;
			if(msg)
			{
				CenterServerMsg& par=Glb().m_CenterServerPara;
				////���������˿ں�
				par.m_is_haveZhuanZhang = msg->m_is_haveZhuanZhang;
				par.m_iMainserverPort=msg->m_iMainserverPort;
				par.m_nHallInfoShowClass=msg->m_nHallInfoShowClass;
				memcpy(par.m_strGameSerialNO,msg->m_strGameSerialNO,sizeof(par.m_strGameSerialNO));
				memcpy(par.m_strMainserverIPAddr,msg->m_strMainserverIPAddr,sizeof(par.m_strMainserverIPAddr)  );
				memcpy(par.m_strHomeADDR,msg->m_strHomeADDR,sizeof(par.m_strHomeADDR)  );
				memcpy(par.m_strWebRootADDR,msg->m_strWebRootADDR,sizeof(par.m_strWebRootADDR)  );
				memcpy(par.m_strHelpADDR,msg->m_strHelpADDR,sizeof(par.m_strHelpADDR)  );
				memcpy(par.m_strDownLoadSetupADDR,msg->m_strDownLoadSetupADDR,sizeof(par.m_strDownLoadSetupADDR)  );
				memcpy(par.m_strDownLoadUpdatepADDR,msg->m_strDownLoadUpdatepADDR,sizeof(par.m_strDownLoadUpdatepADDR));
				memcpy(par.m_strRallAddvtisFlashADDR,msg->m_strRallAddvtisFlashADDR,sizeof(par.m_strRallAddvtisFlashADDR));
				memcpy(par.m_strRoomRollADDR,msg->m_strRoomRollADDR,sizeof(par.m_strRoomRollADDR));
				par.m_nEncryptType = msg->m_nEncryptType;		//ƽ̨���ܷ�ʽ
				par.m_nFunction = msg->m_nFunction;
				par.m_lNomalIDFrom = msg->m_lNomalIDFrom;
				par.m_lNomalIDEnd = msg->m_lNomalIDEnd;

				m_bGetPara = true;
			}	
			if(Glb().m_UpdateUrl=="")
			{
				Glb().m_UpdateUrl=Glb().m_CenterServerPara.m_strDownLoadUpdatepADDR;
			}
			//�Ϸ�flash
			if(lstrlen(Glb().m_CenterServerPara.m_strRallAddvtisFlashADDR)>0)
			{
				m_TopWnd->SetFlashAd();
				if (m_TopDuWnd->GetHWND())
				{
					m_TopDuWnd->SetFlashAd();
				}
			}
			CString strBuffer;
			strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strRallAddvtisFlashADDR),1,1);
			m_WebView.Navigate(strBuffer,NULL,NULL,NULL,NULL);
			//������
			if(lstrlen(Glb().m_CenterServerPara.m_strRoomRollADDR)>0)
			{
				//CNewGameClass * pRoomTab=&((CGamePlaceDlg *)AfxGetMainWnd())->m_RoomTabWnd;
				//pRoomTab->SetAd(msg->m_strRoomRollADDR);
				//m_pActWnd->SetAd(msg->m_strRoomRollADDR);
			}	
			//�������ķ������ɹ�����ʾҳ��
			CString url;
			url.Format("%s",Glb().m_CenterServerPara.m_strHomeADDR);
			IE(url);

			if(msg->m_nIsUsingIMList == 0)
			{
				m_pGameListWnd->m_btIMList.ShowWindow(SW_HIDE);
			}
			else
			{
				m_pGameListWnd->m_btIMList.ShowWindow(SW_SHOW);
			}
#ifndef MY_DEV
			//CString strVersion;
			//strVersion.Format("BZW.%d.%d",VER_MAIN,VER_MIDDLE);
			//if(strcmp(Glb().m_CenterServerPara.m_strGameSerialNO,strVersion)!=0)
			//{
			//	STARTUPINFO StartupInfo;
			//	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
			//	StartupInfo.cb = sizeof(STARTUPINFO);
			//	PROCESS_INFORMATION ProcessInfo;

			//	////Kylin 20090311 ���Զ�����
			//	char up[1000];
			//	TCHAR path[MAX_PATH];
			//	wsprintf(path,"%supdate\\update_%s.ini",Glb().m_Path);

			//	wsprintf(up," -u %s//0//update_version.txt -l %s\\update\\update\\update_0.ini -v %d.%d",Glb().m_UpdateUrl,Glb().m_Path,VER_MAIN,VER_MIDDLE);

			//	OutputDebugString(up);
			//	if(!CreateProcess("update.exe",up,0,0,FALSE,0,0,0,&StartupInfo,&ProcessInfo))
			//	{
			//		TCHAR sz[200];
			//		wsprintf(sz,TEXT("������������ʧ��,��������Ҫ���°�װ�˳���."));
			//		AFCMessageBox(sz,TEXT("����ʧ��"));
			//	}
			//	CloseSocket (true);
			//	AfxGetApp( )->m_pMainWnd->DestroyWindow();
			//	return true;		
			//}

            //wushuqun 2009.5.19
			//kylin ��update ���⣬������ԭ�����Զ����·�ʽ 


			// PengJiLin, 2011-5-31, ʹ����Դ����ļ��汾�ţ�����ʹ���ı��ļ��еġ�
			CString strVersion = GetResourceFileVer();

			//strVersion.Format("BZW.%d.%03d",VER_MAIN,VER_MIDDLE);
			CString s = Glb().m_CenterServerPara.m_strGameSerialNO;
			if(strcmp(s.GetBuffer(),strVersion)!=0)
			{
				STARTUPINFO StartupInfo;
				ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
				StartupInfo.cb = sizeof(STARTUPINFO);
				PROCESS_INFORMATION ProcessInfo;
				TCHAR line[256];
				ZeroMemory(line, sizeof(line));
				char cline[256];
				ZeroMemory(cline, sizeof(cline));

				string _t1 = Glb().m_CenterServerPara.m_strDownLoadUpdatepADDR;
				string ul = JoinUrl(_t1, "update_version.txt");
				string uv = ul;
				//////////////////////////////////////////////////////////////////////////
				//string _path;
				//_path = GetAppPath();
				//sprintf(up, "-n %s -s %s\\ -v %s -ul %s%s -uv %s%s -sm %d", "�������Ϸ", _path.c_str(), "0", Glb().m_UpdateUrl, ul,Glb().m_UpdateUrl,uv, 1);

				
				char path[256];
				wsprintf(path,"%s\\updateconfig.ini",GetAppPath().c_str());
				CINIFile f(path);
				CString m_key="config";
				CString n;
				CString v; 
				int  sm = 1;

				n=f.GetKeyVal(m_key,"-n","��Ϸ����");//��Ϸ����	
				sm=f.GetKeyVal(m_key,"-sm",1);//��ʾ��ʽ
				v=f.GetKeyVal(m_key,"-v","0");//�汾

				sprintf(cline, "-n %s -s %s -v %s -ul %s -uv %s -sm %d", n, /*CBcfFile::GetAppPath()*/GetAppPath().c_str(), v, ul.c_str(), ul.c_str(), sm);
				char2Tchar(cline, line);

				//2012.08.22 yyf ����
				//if(!CreateProcess(TEXT("update.exe"), line,0,0,FALSE,0,0,0,&StartupInfo,&ProcessInfo))
				//{
				//	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
				//	CString str;
				//	CString strTitle;

				//	strTitle = fMsg.GetKeyVal("MainRoom","UpdateError","����ʧ��");
				//	str = fMsg.GetKeyVal("MainRoom","UpdateErrorReinstall",
				//		"������������ʧ��. ���°�װ�������ܻ��޸�������.");

				////	TCHAR sz[200];
				////	wsprintf(sz,TEXT("������������ʧ��,��������Ҫ���°�װ�˳���."));
				//	AFCMessageBox(str,strTitle);
				//}

				//      //-ͬ������- update.exe -------------------------2012.08.24 yyf
				////if(!m_AnonymousPipe.OpenProcess("update.exe"))
				//if(!m_CSocket32.OpenProcess("update.exe"))
				//{ 
				//	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
				//	CString str;
				//	CString strTitle;

				//	strTitle = fMsg.GetKeyVal("MainRoom","UpdateError","����ʧ��");
				//	str = fMsg.GetKeyVal("MainRoom","UpdateErrorReinstall",
				//		"������������ʧ��. ���°�װ�������ܻ��޸�������.");

				//	//	TCHAR sz[200];
				//	//	wsprintf(sz,TEXT("������������ʧ��,��������Ҫ���°�װ�˳���."));
				//	AFCMessageBox(str,strTitle);
				//} 
				////else  if(m_AnonymousPipe.GetProcessIdFromName("update.exe"))
				//else  if(m_CSocket32.GetProcessIdFromName("update.exe"))
				//{
				//	UpdateStructMsg updateMsg;
				//	memset(&updateMsg,0,sizeof(UpdateStructMsg));

				//	updateMsg.command = 1;
				//	updateMsg.nRoomID = -1;//-1 ��ʾ���� 
				//	wsprintf(updateMsg.g_GameName,"%s","��Ϸ����");
				//	wsprintf(updateMsg.g_url,"%s%s",Glb().m_UpdateUrl,uv);
				//	wsprintf(updateMsg.g_ver,"%s",v); 

				//	DWORD dwWrite=0;
				//	//m_AnonymousPipe.Write( &updateMsg ,sizeof(UpdateStructMsg),dwWrite);
				//	m_CSocket32.Send( &updateMsg ,sizeof(UpdateStructMsg));
				//}
				////-------------------------------------------------------------

				//-�첽����- update.exe -------------------------2012.08.24 yyf
				//��ʼ�� sDownLoadUpdateMsg Ȼ���ٴ��� ���ظ����߳�   DownLoadUpdate(LPVOID lparam)
				sDownLoadUpdateMsg sUpdateMsg; 
				//-----------------
				UpdateStructMsg updateMsg;
				memset(&updateMsg,0,sizeof(UpdateStructMsg));
				updateMsg.command = 1;
				updateMsg.nRoomID =-1;//-1 ��ʾ���� 
				wsprintf(updateMsg.g_GameName,"%s","��Ϸ����");
				wsprintf(updateMsg.g_url,"%supdate_version.txt",Glb().m_UpdateUrl);//,uv
				wsprintf(updateMsg.g_ver,"%s",v); 
				sUpdateMsg.pUpdateStructMsg = &updateMsg;
				//-----------------
				UPDATE *lp=(LPUPDATE)malloc(sizeof(UPDATE));
				lp->hProcess=ProcessInfo.hProcess;
				lp->uNameID=-1;
				lp->pGameRoomItem=NULL;//pGameRoomItem;
				sUpdateMsg.pUPDATE = lp;
				//-----------------
				sUpdateMsg.pCMainRoomEx = this;
				//-----------------
				CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
				sUpdateMsg.pCBcfFile = &fMsg;
				//-----------------

				DWORD dwThreadID = 0;
				HANDLE hThread = CreateThread(0,0,DownLoadUpdate,&sUpdateMsg,0,&dwThreadID);

				if (WaitForSingleObject(hThread,5000)==WAIT_TIMEOUT) 
				{//��ʱ update.exe ��û������
					if(m_CSocket32.GetProcessInfo().hProcess)
					{
						::TerminateProcess( m_CSocket32.GetProcessInfo().hProcess, 0 );//�������� update.exe
					}
					m_CSocket32.Close();

					CString str;
					CString strTitle;
					str = fMsg.GetKeyVal("MainRoom","DownLoadOver","����ʧ��");
					strTitle = fMsg.GetKeyVal("MainRoom","MessageTitle","��ʾ");
					
					//AFCMessageBox(str,strTitle);
                    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,strTitle,false,str);
				}
				if (hThread)
				{
					::CloseHandle(hThread);
				}
				//----------------------------------------------------------
				CloseSocket (true);
				AfxGetApp( )->m_pMainWnd->DestroyWindow();
				return true;		
			}
#endif
			CloseSocket (true);		

			return true;
		}
	case MDM_GP_LOGON:	//��½��Ϣ
		{
			return OnLogonMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GP_USERINFO: //�û����Ϲ��� JianGuankun 2011.11.13
		{
			if (ASS_GP_USERINFO_ACCEPT == pNetHead->bAssistantID)
			{
				if (m_pSafeCenterWnd)
				{
					m_pSafeCenterWnd->SetItemSafeState(CSafeCenterWnd::CIT_IDVerify,
						strlen(m_PlaceUserInfo.szIDCardNo) > 0);
				}
				//�ڴ����򷿼䵯����ʾ��Ϣ
				//AFCMessageBox("�޸ĳɹ�","ϵͳ��ʾ��Ϣ");//,MB_OK,this);
                DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�޸ĳɹ�","ϵͳ��ʾ��Ϣ");

				if (m_pSafeCenterWnd)
				{
					CUserInfoWnd** pUserInfoWnd = &m_pGameListWnd->m_pUserInfoWnd;
					if (*pUserInfoWnd)
					{
						(*pUserInfoWnd)->Close();
					}
				}
			}
			else if (ASS_GP_USERINFO_NOTACCEPT == pNetHead->bAssistantID)
			{
				//�ڴ����򷿼䵯����ʾ��Ϣ
				//AFCMessageBox("�޸�ʧ��","ϵͳ��ʾ��Ϣ");//,MB_OK,this);
                DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�޸�ʧ��","ϵͳ��ʾ��Ϣ");
			}

			//Add by JianGuankun 2012.1.4 ��ѯ�û�ID���ǳƽ��
			else if (ASS_GP_USERINFO_NICKNAMEID == pNetHead->bAssistantID)
			{
				GetNickNameID_t* p = (GetNickNameID_t*)pNetData;

				//��������
				if (m_pBankWndEx)
				{
					m_pBankWndEx->UpdateNicknameIDResult(p);
					return true;
				}

				if (m_pBankWnd)
				{
					m_pBankWnd->UpdateNicknameIDResult(p);
				}

				//��Ϸ���������
				for (BYTE i = 0; i < MAX_GAME_ROOM; ++i)
				{
					if (m_RoomInfo[i].bAccess)
					{
						// ��Ϸ����Ϸ�����ұ���Ϣ��
						CGameRoomEx* pGameRoomEx = (CGameRoomEx*)m_RoomInfo[i].pGameRoomWnd;
						if (NULL != pGameRoomEx)
						{
							if (pGameRoomEx->m_pBankWndEx)
							{
								pGameRoomEx->m_pBankWndEx->UpdateNicknameIDResult(p);
								break;
							}

							if (pGameRoomEx->m_pBankWnd)
							{
								pGameRoomEx->m_pBankWnd->UpdateNicknameIDResult(p);
								break;
							}

						}					
					}//bAccess
				}
			}
			//End Add

			return true;
		}
	case MDM_GP_LIST:	//��Ϸ�б�
	case MDM_GP_IM:
		{
			return OnListMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GP_SYSMSG:	// ϵͳ��Ϣ
		{
			return OnSystemMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GP_MESSAGE://�������Ϣ ���ţ�С����
		{
			//wushuqun 2009.6.9
			// qy �޸Ĵ���ϵͳ��Ϣ��Χ 20090212
			if (MDM_GP_SYSMSG == pNetHead->bAssistantID)
			{
				MSG_GR_RS_NormalTalk * pNormalTalk=(MSG_GR_RS_NormalTalk *)pNetData;
				if ((uDataSize<=(sizeof(MSG_GR_RS_NormalTalk)-sizeof(pNormalTalk->szMessage)))
					||(uDataSize>sizeof(MSG_GR_RS_NormalTalk)))
				{
					return true;
				}
				if ((pNormalTalk->iLength+1)!=(uDataSize+sizeof(pNormalTalk->szMessage)-sizeof(MSG_GR_RS_NormalTalk)))
				{
					return true;
				}

				// ��ȡ��Ч����
				pNormalTalk->szMessage[pNormalTalk->iLength]=0;
				for (BYTE i=0;i<MAX_GAME_ROOM;i++)
				{
					if (m_RoomInfo[i].bAccess)
					{
						// ��Ϸ����Ϸ�����ұ���Ϣ��
						if (NULL != m_RoomInfo[i].pGameRoomWnd)
						{
							CGameRoomEx *  pT = (CGameRoomEx * )m_RoomInfo[i].pGameRoomWnd;
							pT->InsertSystemMessageWithGame(pNormalTalk->szMessage);
						}	// ��Ϸ�����ұ���Ϣ��
						else if (NULL != m_RoomInfo[i].pRoomInfoWnd)
						{
							AFCPlaySound(GetModuleHandle(FACE_DLL_NAME),TEXT("SYSTEM_MSG"));
							m_RoomInfo[i].pRoomInfoWnd->m_MessageHandle.InsertSystemMessage(pNormalTalk->szMessage); 
						}						
					}
				}
				return true;
			}
			// end

			//��Ϣ ����
			if(pNetHead->bAssistantID==ASS_GP_NEWS_SYSMSG)
			{				
				//����news.ini
				::CreateThread(NULL,0,DownLoadNewIni,this,0,NULL);
				return true;
			}
			//if(pNetHead->bAssistantID==ASS_GP_DUDU)
			//{	
			//	if(uDataSize!=sizeof(tagBroadcastMsg)) return true;
			//	tagBroadcastMsg* pDudu = (tagBroadcastMsg*)pNetData;
			//	for(int i=0;i<MAX_GAME_ROOM;i++)
			//	{
			//		if(m_RoomInfo[i].bAccess)
			//		{
			//			((CGameRoomEx*)(m_RoomInfo[i].pGameRoomWnd))->Dudu(pDudu,1);
			//		}
			//	}
			//}
			return true;
		}
	case MDM_GR_USER_LOGO:
	{
		if(!logoUpload)
		{
			logoUpload=new CLogoUpload;
			logoUpload->Create(IDD_LOGOUPLOAD,this);
			logoUpload->SetParentWnd(this);
		}
		logoUpload->OnNetMessage(pNetHead,pNetData,uDataSize);
		break;
	}
	case MDM_GP_NOTICE_MSG:
		{
			if(ASS_GR_TRANSFER_MONEY == pNetHead->bAssistantID)		//ת�˳ɹ�����ʾ��Ϣ����ת���� add by xqm,lxl 2010-11-10  
			{
				//Ч������
				if (uDataSize!=sizeof(MSG_GR_S_TransferMoney)) return false;
				MSG_GR_S_TransferMoney * pTransfer=(MSG_GR_S_TransferMoney *)pNetData;
				/// ת�ʳɹ�
				if(ASS_GR_TRANSFER_SUC == pNetHead->bHandleCode)
				{					
					if (pTransfer->destUserID==m_PlaceUserInfo.dwUserID)
					{
						CString strBuffer;
						CString str;
						TCHAR szNum[128];
						TCHAR szAct[128];				
						GetNumString(szNum, pTransfer->i64Money, Glb().m_nPowerOfGold/*, false*/,Glb().m_bUseSpace, Glb().m_strSpaceChar);
						GetNumString(szAct, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold/*, false*/,Glb().m_bUseSpace, Glb().m_strSpaceChar);
						
						CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
						bool bUseNickName = fMsg.GetKeyVal("PBankDlg","UseNickName",0);  //ת����ʾ��Ϣ�����Ƿ���ʾ�ǳ� add by lxl 2010-11-10
						if(bUseNickName)
						{
							str = fMsg.GetKeyVal("GameRoom","BeTransfered",
								"<p 5>�𾴵��û���<b>%s</b>(%d) ����ת�ʡ�<n><y 30>      ת�����<b>%s</b> ���</y><n>������ʵ�ʵ��ˣ�<b>%s</b> ���</p>");
							strBuffer.Format(str,pTransfer->szNickName,pTransfer->UserID,szNum,szAct);
						}
						else
						{
							str = fMsg.GetKeyVal("GameRoom","BeTransfered",
								"<p 5>�𾴵��û���<b>%d</b> ����ת�ʡ�<n><y 30>      ת�����<b>%s</b> ���</y><n>������ʵ�ʵ��ˣ�<b>%s</b> ���</p>");
							strBuffer.Format(str, pTransfer->UserID,szNum,szAct);
						}

						m_PlaceUserInfo.i64Bank+=pTransfer->i64ActualTransfer;
						UpdateMeInfo(0, false/*true*/, 0, false);         // PengJiLin, 2010-10-9, ��������ʾΪ0
						
						//��Ϸ������½���ʾϵͳ��Ϣ
						if (GetGameRoom() != NULL)
						{
							//�ڷ������½Ƿ���һ��ϵͳ��ʾ��Ϣ
							//GetGameRoom()->m_pRightWnd->m_MessageHandle.InsertSystemMessage(strBuffer.GetBuffer());
							
							NetMessageHead nethead;
							MSG_GA_S_Message	msg;
							memset(&nethead,0,sizeof(nethead));
							memset(&msg,0,sizeof(msg));
							nethead.bMainID = MDM_GM_MESSAGE;
							nethead.bAssistantID = ASS_GM_SYSTEM_MESSAGE;
							strcpy(msg.szMessage, strBuffer);
						
							msg.bShowStation=0x01;
						
							if (GetGameRoom()->GetGameFrame() != NULL)
							{
								GetGameRoom()->GetGameFrame()->OnSocketReadEvent(&nethead,&msg,sizeof(MSG_GA_S_Message),NULL);
							}							
						}
						else
						{
							AFCPlaySound(GetModuleHandle(FACE_DLL_NAME),TEXT("SYSTEM_MSG"));
						}

						//�ڴ����򷿼䵯����ʾ��Ϣ
						DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,"ϵͳ��ʾ��Ϣ",true,strBuffer);
					}		
				}
			}
			return true;
		}

	//�û�����
	case MDM_GP_PROP:
		{
			if(!propPannel)
			{
				propPannel=new CPropPannel();
				propPannel->pParentWnd=(CWnd*)this;
				propPannel->Create(IDD_PROPPANNEL,this);
			}

			propPannel->OnNetMessage(pNetHead,pNetData,uDataSize);


			MSG_PROP_RESULT *ResultInfo = new MSG_PROP_RESULT;
			ZeroMemory(ResultInfo,sizeof(MSG_PROP_RESULT));
			switch (pNetHead->bAssistantID)
			{
			case ASS_PROP_BUY_VIP:
				{
					if( pNetHead->bHandleCode == DTK_GR_VIP_BUY_SUCCEED)									//VIP����ʧ�ܣ�
					{
						ResultInfo->dwResult = 7;
					}
					else
					{	
						if(pNetHead->bHandleCode==DTK_GP_VIP_BUY_NOMONEY)									//vipǮ����
						{
							ResultInfo->dwResult = 6;
						}
						else if (pNetHead->bHandleCode==DTK_GR_VIP_BUY_NOUSER)								//���͵���Ҳ�����
						{
							ResultInfo->dwResult = 8;
						}
						else if (pNetHead->bHandleCode==DTK_GR_VIP_GIVE_SUCCESS)								//����VIP�ɹ�
						{
							ResultInfo->dwResult = 9;
						}
						else																				//VIP����ʧ��
						{
							ResultInfo->dwResult = 5;
						}
					}
					if (m_RoomInfo[0].pGameRoomWnd != NULL)
						m_RoomInfo[0].pGameRoomWnd->PostMessage(WM_GP_PROP,(WPARAM)ResultInfo, sizeof(MSG_PROP_RESULT));
				}
				break;
				
			case ASS_PROP_BUY:
			case ASS_PROP_BUY_NEW:
				if (DTK_GP_PROP_BUY_NOMONEY == pNetHead->bHandleCode)										//�������ûǮ
				{
					ResultInfo->dwResult = 10;
					if (m_RoomInfo[0].pGameRoomWnd != NULL)
						m_RoomInfo[0].pGameRoomWnd->PostMessage(WM_GP_PROP,(WPARAM)ResultInfo, sizeof(MSG_PROP_RESULT));
				}
				else if( pNetHead->bHandleCode!=DTK_GP_PROP_BUY_SUCCEED && pNetHead->bHandleCode != DTK_GP_PROP_BUYANDUSE_SUCCEED)									//���߹���ʧ�ܣ�
				{
					ResultInfo->dwResult = 1;
					if (m_RoomInfo[0].pGameRoomWnd != NULL)
						m_RoomInfo[0].pGameRoomWnd->PostMessage(WM_GP_PROP,(WPARAM)ResultInfo, sizeof(MSG_PROP_RESULT));
				}
				else if (pNetHead->bHandleCode != DTK_GP_PROP_BUYANDUSE_SUCCEED)
				{	
					ResultInfo->dwResult = 0;
					if (m_RoomInfo[0].pGameRoomWnd != NULL)											//������߳ɹ�										
						m_RoomInfo[0].pGameRoomWnd->PostMessage(WM_GP_PROP, (WPARAM)ResultInfo, sizeof(MSG_PROP_RESULT));
				}
				break;
			case ASS_PROP_GIVE:
				if(pNetHead->bHandleCode==DTK_GP_PROP_GIVE_SUCCEED_GIVER)							//���ͳɹ�
				{
					ResultInfo->dwResult = 2;
					if (m_RoomInfo[0].pGameRoomWnd != NULL)
						m_RoomInfo[0].pGameRoomWnd->PostMessage(WM_GP_PROP, (WPARAM)ResultInfo, sizeof(MSG_PROP_RESULT));
				}
				else
				{
					_TAG_PROP_GIVE* propOGive = (_TAG_PROP_GIVE*)pNetData;

					//���������ֻ���͵��ߵ���Ҳ���ʾ��������ʧ�ܵ���Ϣ
					//Added by JianGuankun 2012.4.16

					if (m_PlaceUserInfo.dwUserID == propOGive->dwUserID)
					{
						if(pNetHead->bHandleCode==DTK_GP_PROP_GIVE_NOTARGETUSER)						//���͵���Ҳ�����
						{
							ResultInfo->dwResult = 3;
							if (m_RoomInfo[0].pGameRoomWnd != NULL)
								m_RoomInfo[0].pGameRoomWnd->PostMessage(WM_GP_PROP, (WPARAM)ResultInfo, sizeof(MSG_PROP_RESULT));
						}
						else 																			//���͵���ʧ��
						{
							ResultInfo->dwResult = 4;
							if (m_RoomInfo[0].pGameRoomWnd != NULL)
								m_RoomInfo[0].pGameRoomWnd->PostMessage(WM_GP_PROP, (WPARAM)ResultInfo, sizeof(MSG_PROP_RESULT));
						}
					}
		
				}
				break;
			default:
				break;
			}
		}
		break;
	case MDM_GP_LOCK_ACCOUNT:	//���������ͽ��������� zxj 2009-11-16
		{
			OnLockAccountResult(pNetHead, pNetData, uDataSize, pClientSocket);
			break;
		}
	case MDM_GP_BIND_MOBILE:
		{
			MSG_GP_R_BindMobile* pBind = (MSG_GP_R_BindMobile*)pNetData;

			if( pBind == NULL)
			{
				return true;
			}

			if( pBind->dwUserID != m_PlaceUserInfo.dwUserID )
			{
				return true;
			}

			if( pBind->dwCommanType == 0 )	//���������ֻ����
			{
				if (m_pSafeCenterWnd)
				{
					CUserInfoWnd** pUserInfoWnd = &m_pGameListWnd->m_pUserInfoWnd;
					if (*pUserInfoWnd)
					{
						(*pUserInfoWnd)->Close();
					}
					m_pSafeCenterWnd->SetItemSafeState(CSafeCenterWnd::CIT_BindMobile,false);
				}
				//end add

				m_PlaceUserInfo.iBindMobile = 0;
				CGamePlaceDlg* pGamePlace = (CGamePlaceDlg*)AfxGetMainWnd();
				if (pGamePlace)
				{
					CUserInfoWnd* pUserInfoWnd = pGamePlace->m_GameListWnd.m_pUserInfoWnd;
					if (pUserInfoWnd)
					{
						pUserInfoWnd->OnBindMobileResult(false);
					}
				}

				if (!m_pSafeCenterWnd)
				{
					DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,"ϵͳ��ʾ��Ϣ",false,"�ѽ���ֻ��󶨡�");
				}
			}
			else if( pBind->dwCommanType == 1)//������ֻ����
			{
				m_PlaceUserInfo.iBindMobile = 1;
				CGamePlaceDlg* pGamePlace = (CGamePlaceDlg*)AfxGetMainWnd();
				if (pGamePlace)
				{
					CUserInfoWnd* pUserInfoWnd = pGamePlace->m_GameListWnd.m_pUserInfoWnd;
					if (pUserInfoWnd)
					{
						pUserInfoWnd->OnBindMobileResult(true);
					}
				}

				if (m_pSafeCenterWnd)
				{
					CUserInfoWnd** pUserInfoWnd = &m_pGameListWnd->m_pUserInfoWnd;
					if (*pUserInfoWnd)
					{
						(*pUserInfoWnd)->Close();
					}
					m_pSafeCenterWnd->SetItemSafeState(CSafeCenterWnd::CIT_BindMobile,true);
				}
				else
				{
					DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,"ϵͳ��ʾ��Ϣ",false,"���ֻ��ɹ���");
				}
				//end add
			}

			break;
		}
	case MDM_GP_PLAYER_2_GM:
		{
			MSG_GP_S_Talk2GM_t *msg = (MSG_GP_S_Talk2GM_t*)pNetData;
			TCHAR*  s = new TCHAR[256];
			_tcscpy(s, msg->szMsg);
			switch (pNetHead->bAssistantID)
			{
			case ASS_GP_MSG_FROM_GM	:									///<���մ�GM����������Ϣ
				{
					if (msg->iUserID == 0)
					{
						if (m_RoomInfo[0].pGameRoomWnd != NULL)
							m_RoomInfo[0].pGameRoomWnd->PostMessage(WM_GM_MSG, WPARAM(s), 0);
					}
					else if (msg->iUserID == m_PlaceUserInfo.dwUserID)
					{
						for (map<int, CShortMessage*>::iterator i = m_pGMMsgDlgs.begin(); i != m_pGMMsgDlgs.end();)
						{
							if (i->second->m_iGmID == 0) 
							{
								delete i->second;
								i = m_pGMMsgDlgs.erase(i);
							}
							else i++;
						}
						if (m_pGMMsgDlgs.count(msg->iGMID) <= 0)
						{
							CShortMessage* pWnd = new CShortMessage(this);
							pWnd->SetTalkInfo(msg->iGMID);
							pWnd->Create(IDD_SHORT_MESSAGE, GetDesktopWindow());
							pWnd->CenterWindow();
							pWnd->m_BtExpression.ShowWindow(SW_HIDE);
							pWnd->ShowWindow(SW_SHOWNOACTIVATE);
							CString s;
							if (pNetHead->bHandleCode != 0)
								pWnd->m_MessageHandle.InsertSystemMessage(msg->szMsg);
							else 
							{
								s.Format("����Ա%d", msg->iGMID);
								pWnd->OnRecvMessage(s.GetBuffer(), msg->szMsg, RGB(255, 0, 0));
							}
							m_pGMMsgDlgs[msg->iGMID] = pWnd;
						}
						else
						{
							CShortMessage* pWnd = m_pGMMsgDlgs[msg->iGMID];
							pWnd->ShowWindow(SW_SHOWNOACTIVATE);
							CString s;
							if (pNetHead->bHandleCode != 0)
								pWnd->m_MessageHandle.InsertSystemMessage(msg->szMsg);
							else 
							{
								s.Format("����Ա%d", msg->iGMID);
								pWnd->OnRecvMessage(s.GetBuffer(), msg->szMsg, RGB(255, 0, 0));
							}
						}
					}
					break;
				}
			case ASS_GR_MSG_BE_AWARD:
				{
					CGMMsgDlg dlg(s, 0);
					dlg.DoModal();
					delete []s;
					break;
				}
			case ASS_GR_MSG_BE_KICKED:									///<��GM�߳�����
				{
					CGMMsgDlg dlg(s);
					dlg.DoModal();
					delete []s;
					if (m_RoomInfo->pGameRoomWnd != NULL)
						m_RoomInfo[0].pGameRoomWnd->PostMessage(WM_GM_KICK, 0, 0);
					break;
				}
			case ASS_GR_MSG_BE_BANDID:									///<��GM���
				{
					CGMMsgDlg dlg(s);
					dlg.DoModal();
					delete []s;
					if (m_RoomInfo->pGameRoomWnd != NULL)
						m_RoomInfo[0].pGameRoomWnd->PostMessage(WM_GM_KICK, 0, 0);
					PostQuitMessage(0);
					break;
				}
			case ASS_GP_GET_GM_LIST:
				{					
					int n = uDataSize / sizeof(MSG_GP_R_GmList_t);
					MSG_GP_R_GmList_t* pdata = (MSG_GP_R_GmList_t*)pNetData;
					if (m_pGMListDlg == NULL)
					{
						m_pGMListDlg = new CGMListDlg(this);
						m_pGMListDlg->Create(IDD_DLG_GMLIST, this);
					}
					m_pGMListDlg->OpenWindow(*pdata);
					break;
				}
			case ASS_GP_TALK_2_GM:
				{
					if (msg->iUserID == m_PlaceUserInfo.dwUserID)
					{
						for (map<int, CShortMessage*>::iterator i = m_pGMMsgDlgs.begin(); i != m_pGMMsgDlgs.end();)
						{
							if (i->second->m_iGmID == 0) 
							{
								delete i->second;
								i = m_pGMMsgDlgs.erase(i);
							}
							else i++;
						}
						if (m_pGMMsgDlgs.count(msg->iGMID) <= 0)
						{
							CShortMessage* pWnd = new CShortMessage(this);
							AfxSetResourceHandle(GetModuleHandle(NULL));
							pWnd->SetTalkInfo(msg->iGMID);
							pWnd->Create(IDD_SHORT_MESSAGE, GetDesktopWindow());
							pWnd->CenterWindow();
							pWnd->m_BtExpression.ShowWindow(SW_HIDE);
							pWnd->ShowWindow(SW_SHOWNOACTIVATE);
							CString s;
							s.Format("%d", msg->iUserID);
							pWnd->OnRecvMessage(s.GetBuffer(), msg->szMsg, RGB(255, 0, 0));
							m_pGMMsgDlgs[msg->iGMID] = pWnd;
						}
						else
						{
							CShortMessage* pWnd = m_pGMMsgDlgs[msg->iGMID];
							pWnd->ShowWindow(SW_SHOWNOACTIVATE);
							CString s;
							s.Format("%d", msg->iUserID);
							pWnd->OnRecvMessage(s.GetBuffer(), msg->szMsg, RGB(255, 0, 0));
						}
					}
				}
			}
			break;
		}
	case MDM_GP_RECEIVEMONEY:
		{
			TReceiveMoney* _p = (TReceiveMoney*)pNetData;
			if (_p != NULL)
			{
				if (pNetHead->bAssistantID == ASS_RECEIVE)
				{

					m_PlaceUserInfo.i64Bank += _p->i64Money;
					UpdateMeInfo(m_PlaceUserInfo.i64Money,TRUE, 0, 0);
					CString s;
					
					s.Format("��ȡ��ҳɹ�,�����컹��%d����ȡ����", _p->iTotal - _p->iCount);
					DUIOkeyMsgBox(m_hWnd,false,s);
				}
				else if (pNetHead->bAssistantID == ASS_RECEIVE_PASS)
				{
					CString s;
					s.Format("������Ϸ�ҵ���%d��ʱ��ſ�����ȡŶ", _p->i64MinMoney);
					DUIOkeyMsgBox(m_hWnd,false,s);
				}
				else if (pNetHead->bAssistantID == ASS_RECEIVE_OVER)
				{
					CString s;
					s.Format("����������ȡ���");
					DUIOkeyMsgBox(m_hWnd,false,s);
				}
				else if (pNetHead->bAssistantID == ASS_RECEIVE_TIMELESS)
				{
					CString s;
					s.Format("������Ҫ�� %d ���Ӳ�����ȡ",_p->iTime);
					DUIOkeyMsgBox(m_hWnd,false,s);
				}
				else if (pNetHead->bAssistantID == ASS_RECEIVE_ERROR)
				{
					CString s;
					s.Format("��ȡ����");
					DUIOkeyMsgBox(m_hWnd,false,s);
				}
			}

			break;
		}
	case MDM_GP_CHARMEXCHANGE:
		{
			if (pNetHead->bAssistantID == ASS_GETLIST)
			{
				if (CharmPannel != NULL)
					CharmPannel->OnNetMessage(pNetHead, pNetData, uDataSize);
			}
			else if (pNetHead->bAssistantID == ASS_EXCHANGE)
			{
				TCharmExchange_Item *_p = (TCharmExchange_Item*)pNetData;
				if (_p != NULL)
				{
					m_PlaceUserInfo.dwFascination -= _p->iPoint;
					UpdateMeInfo(m_PlaceUserInfo.i64Money, TRUE, m_PlaceUserInfo.dwFascination, TRUE);

					if (CharmPannel != NULL)
					{
						CharmPannel->OnNetMessage(pNetHead, pNetData, uDataSize);
					}

					if (_p->iPropID == 0)
					{
						m_PlaceUserInfo.i64Bank += _p->iPropCount;
						UpdateMeInfo(m_PlaceUserInfo.i64Money,TRUE, 0, 0);
					}
					else
					{
						_TAG_USERPROP *userProp=NULL;
						for (int i = 0; i < Glb().userPropLibrary.GetCount(); i++)
						{
							userProp = Glb().userPropLibrary.GetAt(i);
							if (userProp != NULL && userProp->nPropID == _p->iPropID)
							{
								userProp->nHoldCount += _p->iPropCount;
							}
						}
					}
					if (pNetHead->bHandleCode == 1)
						DUIMessageBox(m_hWnd,MB_OK|MB_ICONERROR,"ϵͳ��ʾ��Ϣ",false,"�һ�����");
					if (pNetHead->bHandleCode == 2)
						DUIMessageBox(m_hWnd,MB_OK|MB_ICONERROR,"ϵͳ��ʾ��Ϣ",false,"�һ�����");
					if (pNetHead->bHandleCode == 3)
						DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,"ϵͳ��ʾ��Ϣ",false,"����ֵ��������!");
					if (pNetHead->bHandleCode == 4)
						DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,"ϵͳ��ʾ��Ϣ",false,"����ֵ��������!");
					if (pNetHead->bHandleCode == 5)
						DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,"ϵͳ��ʾ��Ϣ",false,"�һ��ɹ�!");
				}
			}

			break;
		}
	case MDM_GP_MAIL:
		{
			TMailItem* _p = (TMailItem*)pNetData;
			if (_p != NULL)
			{
				//AFCMessageBox(_p->szMsg);
				if (_p->iMsgType == 0) //�����ʼ���Ϣ
				{
					if (m_pGameListWnd != NULL)
						m_pGameListWnd->AddMail(*_p);
				}
				else if (_p->iMsgType == 1) //����֪ͨ��Ϸ��ʾ�������Ϣ
				{
					CGameRoomEx* pGameRoom = (CGameRoomEx*)m_RoomInfo[0].pGameRoomWnd;
					if (pGameRoom)
					{
						pGameRoom->BoardcastTrottingMsg(_p);
					}
				}
				else if (_p->iMsgType == 2) //������������Ϣ
				{
					CMiniPopWnd* pMiniWnd = new CMiniPopWnd(this);
					pMiniWnd->SetMessage(_p);
					pMiniWnd->Create(NULL,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
					pMiniWnd->ShowWindow(SW_SHOW);
					pMiniWnd->MoveWindowToRightBottom();
				}
				else if (_p->iMsgType == 3) //����������Ϣ
				{
					if (m_pActWnd)
					{
						m_pActWnd->SetAd(_p->szMsg);
					}
				}
			}
			
			break;
		}
	case MDM_GP_LOGONUSERS:
		{
			ONLINEUSERSCOUNT* totalOnlineUsers = static_cast<ONLINEUSERSCOUNT*>(pNetData);
			if (totalOnlineUsers != NULL)
			{
				SetonlineUserCount(totalOnlineUsers->uiLogonPeopCount);
			}
		}
		break;
	case MDM_GP_BANK: //������Ϣ
		{
			OnBankMessage(pNetHead,pNetData,uDataSize,pClientSocket);
			break;
		}

	case MDM_GP_SMS: //�ֻ���������
		{
			if (pNetHead->bAssistantID == ASS_GP_SMS_VCODE) //�յ�������֤��
			{
				MSG_GP_SmsVCode* _p = (MSG_GP_SmsVCode*)pNetData;

				if (_p != NULL)
				{
					if (::strlen(_p->szVCode) == 0)
					{
						#ifdef MY_MOBILE_VCODE_OFF
							return true;
						#endif

						DUIOkeyMsgBox(m_hWnd,false,"δ����ȷ��ȡ�ֻ���֤�룬�����ԡ�");

						if (m_pMobileVerifyWnd)
						{
							m_pMobileVerifyWnd->ResetReVCodeUI();
						}

						if (m_pGameListWnd->m_pUserInfoWnd)
						{
							m_pGameListWnd->m_pUserInfoWnd->ResetReVCodeUI();
						}
					
						return true;
					}
					::lstrcpy(m_szMobileVCode,_p->szVCode);
				}

			}

			break;
		}
	case MDM_GP_LASTLOINGGAME:
		{
			if (pNetHead->bAssistantID == ASS_GP_LASTLOINGGAME)
			{
				if (pNetHead->bHandleCode == 0)
				{
					ComNameInfo* _p = (ComNameInfo*)pNetData;
					if (_p != NULL)
					{
						m_pGameListWnd->m_GameList.HandleLoginGameName(_p);
						m_pGameListWnd->m_GameList.FillGetListItem();
					}
				}
			}
			break;
		}
	}

	return true;
}
//�б���Ϣ
bool CMainRoomEx::OnSystemMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GP_NEWS_SYSMSG:		//����
		{
			//����news.ini
			::CreateThread(NULL,0,DownLoadNewIni,this,0,NULL);
			return true;
		}
	}
	return true;
}

//������������������� zxj 2009-11-16
void CMainRoomEx::OnLockAccountResult( NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	KillTimer(IDT_CHANGE_LOCK);
	m_TopWnd->SetLockWndState(TRUE);	//������������ť��Ч
	m_bCanClick = TRUE;					//���ź�������Ч

	MSG_GP_R_LockAndUnlock * pLockResult = (MSG_GP_R_LockAndUnlock*)pNetData;
	if( pLockResult == NULL)
		return;
	if( pLockResult->dwUserID != m_PlaceUserInfo.dwUserID )
		return;

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;

	str = fMsg.GetKeyVal("MainRoom","BackToClient","[����] ���ؿͻ��ˣ�%d");

	CString strErrorMsg = "";

	CString strMsg;
	strMsg.Format(str, pLockResult->dwCommanResult);
	OutputDebugString(strMsg);
	//������������Ϊ���֣�ֻҪ������󣬾�ֱ�ӷ���
	if( pLockResult->dwCommanResult == ERR_GP_USER_PASS_ERROR )
	{
	//	strErrorMsg = TEXT("������������������֤���ٴγ���!");
		BZSoundPlay(this, "music/������ʾ.mp3", 0);
		strErrorMsg = fMsg.GetKeyVal("MainRoom","PassWrodError","������������������֤���ٴγ���!");
		DUIMessageBox(m_hWnd,MB_OK|MB_ICONERROR,"ϵͳ��ʾ��Ϣ",false,strErrorMsg);
		return;
	}

	if( pLockResult->dwCommanType == 0 )	//����������
	{
		if( pLockResult->dwCommanResult == ERR_GP_UNLOCK_SUCCESS ) //�����ɹ�
		{
			m_bLockMathine = FALSE;
			m_TopWnd->SetLockType(FALSE);	//ֵ��ǰ״̬Ϊδ����״̬
			m_TopWnd->FixControlStation();
		//	strErrorMsg = TEXT("���������ɹ���");
			BZSoundPlay(this, "music/����.mp3", 0);
			strErrorMsg = fMsg.GetKeyVal("MainRoom","UnlockMachineSuccess","���������ɹ���");

			//Addad By JianGuankun 2011.11.13
			if (m_pSafeCenterWnd)
			{
				CUserInfoWnd** pUserInfoWnd = &m_pGameListWnd->m_pUserInfoWnd;
				if (*pUserInfoWnd)
				{
					(*pUserInfoWnd)->Close();
				}
				m_pSafeCenterWnd->SetItemSafeState(CSafeCenterWnd::CIT_LockMachine,false);
			}
			//end add

			m_PlaceUserInfo.iLockMathine = 0;
			CGamePlaceDlg* pGamePlace = (CGamePlaceDlg*)AfxGetMainWnd();
			if (pGamePlace)
			{
				CUserInfoWnd* pUserInfoWnd = pGamePlace->m_GameListWnd.m_pUserInfoWnd;
				if (pUserInfoWnd)
				{
					pUserInfoWnd->OnLockResult(0);
				}
			}
		}
		else if(pLockResult->dwCommanResult == ERR_GP_NO_LOCK ) //����δ���󶨻���
		{
		//	strErrorMsg = TEXT("����ʧ�ܡ�������ʺ��Ƿ�ʹ�ù����������Ĺ��ܡ�");
			BZSoundPlay(this, "music/������ʾ.mp3", 0);
			strErrorMsg = fMsg.GetKeyVal("MainRoom","UnlockErrorCheckAccount",
				"����ʧ�ܡ�������ʺ��Ƿ�ʹ�ù����������Ĺ��ܡ�");
		}
		else if(pLockResult->dwCommanResult == ERR_GP_CODE_DISMATCH ) //�����벻ƥ��
		{
		//	strErrorMsg = TEXT("����ʧ�ܡ����鵱���Ƿ�ʹ�øû����������ʺš�");
			BZSoundPlay(this, "music/������ʾ.mp3", 0);
			strErrorMsg = fMsg.GetKeyVal("MainRoom","UnlockErrorCheckMachine",
				"����ʧ�ܡ����鵱���Ƿ�ʹ�øû����������ʺš�");
		}
		else
		{
			return;
		}
	}
	else	//�����������
	{
		if( pLockResult->dwCommanResult == ERR_GP_LOCK_SUCCESS )	//�����ɹ�
		{
			if (CMainRoomEx::m_bLockQuit == true)
			{
				BZSoundPlay(this, "music/����.mp3",0);
				::SendMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_CLOSE,0,0);
				return;
			}
			m_bLockMathine = TRUE;
			m_TopWnd->SetLockType(TRUE);	//ֵ��ǰ״̬Ϊδ����״̬
			m_TopWnd->FixControlStation();
		//	strErrorMsg = TEXT("���������ɹ���");
			BZSoundPlay(this, "music/����.mp3",0);

 			strErrorMsg = fMsg.GetKeyVal("MainRoom","LockMachineSuccess","���������ɹ���");

			m_PlaceUserInfo.iLockMathine = 1;
			CGamePlaceDlg* pGamePlace = (CGamePlaceDlg*)AfxGetMainWnd();
			if (pGamePlace)
			{
				CUserInfoWnd* pUserInfoWnd = pGamePlace->m_GameListWnd.m_pUserInfoWnd;
				if (pUserInfoWnd)
				{
					pUserInfoWnd->OnLockResult(1);
				}
			}

			//Addad By JianGuankun 2011.11.13
			if (m_pSafeCenterWnd)
			{
				CUserInfoWnd** pUserInfoWnd = &m_pGameListWnd->m_pUserInfoWnd;
				if (*pUserInfoWnd)
				{
					(*pUserInfoWnd)->Close();
				}

				m_pSafeCenterWnd->SetItemSafeState(CSafeCenterWnd::CIT_LockMachine,true);
			}
			//end add
			
//			BZSoundPlay(this, "music/����.mp3", 0);
		}
		else if(pLockResult->dwCommanResult == ERR_GP_ACCOUNT_HAS_LOCK )	//�˻��Ѿ�����ĳ̨����
		{
		//	strErrorMsg = TEXT("����ʧ�ܡ����ʺ��Ѿ�����������������");
			BZSoundPlay(this, "music/������ʾ.mp3", 0);
			strErrorMsg = fMsg.GetKeyVal("MainRoom","LockErrorAccountUsed","����ʧ�ܡ����ʺ��Ѿ�����������������");
		}
		else
		{
			return;
		}
	}

	
	DUIMessageBox(m_hWnd,MB_OK|MB_ICONINFORMATION,"ϵͳ��ʾ��Ϣ",false,strErrorMsg);

	return;
}


void CMainRoomEx::ShowCharmPannel()
{
	if (CharmPannel != NULL) CharmPannel->ShowWindow(SW_SHOW);
}

bool CMainRoomEx::GetInGame()
{
	BYTE m_bNowRoomIndex=255;
	for (BYTE i=0;i<sizeof(m_RoomInfo)/sizeof(m_RoomInfo[0]);i++)
	{
		if (m_RoomInfo[i].bAccess==false) 
		{
			m_bNowRoomIndex=((m_bNowRoomIndex==255)?i:m_bNowRoomIndex);
		}
	}
	if (m_bNowRoomIndex==255) return true;
	else return false;
}

//������Ϣ����
bool CMainRoomEx::OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	CString path = CBcfFile::GetAppPath()+"ClientMessage.bcf";
	CString str = "ϵͳ�汾�����°汾���Ƿ��������أ�";
	if (CBcfFile::IsFileExist(path))
	{
		CBcfFile fMsg(path);
		str = fMsg.GetKeyVal("MainRoom","NewSystemVersion","ϵͳ�汾�����°汾���Ƿ��������أ�");
	}

	if (pNetHead->bAssistantID==ASS_NET_TEST)	//���������Ϣ
	{
		pClientSocket->SendData(MDM_CONNECT,ASS_NET_TEST,0);//������Ϣ����
		return true;
	}
	else if ((pNetHead->bAssistantID==2)||(pNetHead->bAssistantID==3))	//���ӳɹ�
	{
		MSG_S_ConnectSuccess * _p = (MSG_S_ConnectSuccess *)pNetData;
		if (_p != NULL)
		{
			pClientSocket->SetCheckCode(_p->i64CheckCode, SECRET_KEY);
		}

		if(m_bConnectingToCenterServer)
		{
			OnConnectToCenterServerSucceed();///������������Ϣ������ȫ������
			return true;
		}
		//�汾���
		if (m_bCheckVer==false)
		{
			m_bCheckVer=true;
			bool bNewVer=false;
			if ((uDataSize!=sizeof(MSG_S_ConnectSuccess))||(GamePlaceVerCheck((MSG_S_ConnectSuccess *)pNetData,bNewVer)==false))
			{
				int i=0;//�汾���ʧ��
			}

			//�����°汾
			//if ((bNewVer==true)&&(AFCMessageBox(str,MESSAGE_TITLE,AFC_YESNO)==IDOK))//,MB_ICONQUESTION|MB_YESNO,this)==IDYES))
			if ((bNewVer==true)&&(DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,MESSAGE_TITLE,true,str)==IDYES))
			{
				//ShellExecute(NULL,TEXT("open"),TEXT("http://www.kuaile.net/down/games.exe"),NULL,NULL,SW_MAXIMIZE);
				PostQuitMessage(0);
				return false;				
			}
		}

        // PengJiLin, 2010-6-2, �Ƿ��ǵ���������������֪ͨ��������(Ÿ��)
        if(TRUE == g_bRunBySoftware)
        {
            pClientSocket->SendData(MDM_GP_LOGON,ASS_GP_LOGON_BY_SOFTWARE,0);
        }

		//��������
		HandleMission();
		return true;
	}
	return false;
}


void CMainRoomEx::OnConnectToCenterServerSucceed()///������������Ϣ������ȫ������
{
	CBcfFile f( CBcfFile::GetAppPath() + "bzgame.bcf");
	CString strValue = f.GetKeyVal(_T("BZW"),_T("ClientCode"),"0"); ///< ���ݴ�ֵ����ȡURL
	if (0 == strValue.Compare(_T("0")))
	{
		SendData(0,0,MDM_GP_REQURE_GAME_PARA,0,0);
	}
	else
	{
		SendData(strValue.GetBuffer(),strValue.GetLength()+1,MDM_GP_REQURE_GAME_PARA,0,0);
	}
	
}


//����ر���Ϣ
bool CMainRoomEx::OnSocketCloseEvent()
{
	KillTimer(TIME_KEEP_ALIVE);
	m_nKeepAliveCount=0xFF;	
	if(m_bConnectingToCenterServer)// && m_bGetPara)
	{
		if (!m_bGetPara)
		{
			//AFCMessageBox("�Ƿ����ݰ���");
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�Ƿ����ݰ���");
			AfxGetApp( )->m_pMainWnd->DestroyWindow ();
			return true;
		}
		m_bConnectingToCenterServer = false;

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

		if(m_pBankWnd)
		{
			m_pBankWnd->Close();
			PostMessage(WM_BANK_CLOSE,0,0);
		}

		CMainRoomEx::m_bBankLogoned = false;

		PostMessage(WM_COMMAND,IDM_LOGON_PLACE,0);
		return true;
	}
	KillTimer(ID_CHECK_SEND);
	if (m_Connect.GetSafeHwnd()) m_Connect.DestroyWindow();

	for(int i = 0; i < MAX_GAME_ROOM; i ++)
	{
		if(m_RoomInfo[i].bAccess)
		{
			// �ر���Ϸ����
			CGameRoomEx* pGameRoomEx = (CGameRoomEx*)m_RoomInfo[i].pGameRoomWnd;
			if (NULL != pGameRoomEx)
			{
				pGameRoomEx->OnBnClickedClose();
			}	
		}
	}

	PostMessage(WM_COMMAND,MAKELONG(IDC_BTN_MAIN5,BN_CLICKED));
	return true;
}
void CMainRoomEx::OnConnectToCenterServer()
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;
	//������ʾ
	if (m_Connect.GetSafeHwnd()==NULL) m_Connect.Create(IDD_CONNECT,this);
	str = fMsg.GetKeyVal("MainRoom","Connecting","�����������ķ����������Ժ�...");
	m_Connect.SetMessageText(str);
	m_Connect.CenterWindow();
	m_Connect.ShowWindow(SW_SHOW);
	bool bConnect=false;
	CString ad=CBcfFile::GetAppPath ();
	CBcfFile f( ad + "bzgame.bcf");
	int static iConnect=0;
	CString s;
	s.Format ("%d",++iConnect);
	//AfxMessageBox(s);
	Glb().m_CenterServerIPAddr=f.GetKeyVal(Glb().m_key,"LoginIP"+s,"127.0.0.1");//������IP
	if(iConnect>5||Glb().m_CenterServerIPAddr=="") 
	{
		//::AfxGetMainWnd()->ShowWindow(SW_NORMAL);
		::AfxGetMainWnd()->SetForegroundWindow();
		m_Connect.DestroyWindow();
		///AFCMessageBox(IDS_CONNECT_ERROR,AfxGetInstanceHandle(),MESSAGE_TITLE,MB_ICONQUESTION,this);
		str = fMsg.GetKeyVal("MainRoom","ConnectServerError",
			"�������ķ��������ɹ�,�����˳�,���Ժ�����!");
		
		//AFCMessageBox(str);
        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,str);
		AfxGetApp( )->m_pMainWnd->DestroyWindow ();
		return;
	}

	if(m_TCPSocket != NULL && m_TCPSocket->Connect(Glb().m_CenterServerIPAddr,Glb().m_CenterServerPort)==false)
		this->OnConnectToCenterServer();
	m_bConnectingToCenterServer=true;

	//��������
	//memset(m_Mission,0,sizeof(m_Mission));
	//AddMission(MS_LOGON,0,0);
}
//����������Ϣ
bool CMainRoomEx::OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket)
{
	/*if (uErrorCode!=0)	//���ӳ��ִ���
	{
	if(m_bConnectingToCenterServer)
	{
	::AfxGetMainWnd()->ShowWindow(SW_NORMAL);
	::AfxGetMainWnd()->SetForegroundWindow();
	///AFCMessageBox(IDS_CONNECT_ERROR,AfxGetInstanceHandle(),MESSAGE_TITLE,MB_ICONQUESTION,this);
	MessageBox("�������ķ��������ɹ�,�����˳�,���Ժ�����!","����");
	AfxGetApp( )->m_pMainWnd->DestroyWindow ();
	return true;
	}
	if (m_Connect.GetSafeHwnd()) m_Connect.DestroyWindow();
	AFCMessageBox(m_TCPSocket->TranslateError(uErrorCode),MESSAGE_TITLE,MB_ICONQUESTION,this);
	if ((m_Mission[0].bAccess==true)&&(m_Mission[0].uMissionID==MS_LOGON)) PostMessage(WM_COMMAND,IDM_LOGON_PLACE,0);
	return true;
	}
	return true;*/
	if (uErrorCode!=0)	//���ӳ��ִ���
	{
		if(m_bConnectingToCenterServer)
		{
			m_bConnectingToCenterServer=false;
			this->OnConnectToCenterServer();
			//::AfxGetMainWnd()->ShowWindow(SW_NORMAL);
			//::AfxGetMainWnd()->SetForegroundWindow();
			//MessageBox("�������ķ��������ɹ�,�����˳�,���Ժ�����!");
			//AfxGetApp( )->m_pMainWnd->DestroyWindow ();

			return true;
		}
		if (m_Connect.GetSafeHwnd()) m_Connect.DestroyWindow();
		
		//AFCMessageBox(m_TCPSocket->TranslateError(uErrorCode),MESSAGE_TITLE);//,MB_ICONQUESTION,this);
        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,m_TCPSocket->TranslateError(uErrorCode));

		if ((m_Mission[0].bAccess==true)&&(m_Mission[0].uMissionID==MS_LOGON))
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

			CMainRoomEx::m_bBankLogoned = false;
			PostMessage(WM_COMMAND,IDM_LOGON_PLACE,0);
		}
			
		return true;
	}
	
	return true;//���ӳɹ�
}
//��¼��Ϸ����
void CMainRoomEx::OnLogonPlace()
{
#ifdef AI_MACHINE
	// ��ȡ����
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + _T("ai_config.bcf"));
	CString key=_T("config");
	m_Airoom = f.GetKeyVal(key, TEXT("roomid"), 0);
#endif

	if (CLogonDialog::m_bIsShow) return;

	CLogonDialog* pLogonDlg = new CLogonDialog();
	if(!pLogonDlg)
	{
		return;
	}
	m_Connect.ShowWindow(SW_HIDE);
	pLogonDlg->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
	pLogonDlg->m_bRegUser = false;
	pLogonDlg->CenterWindow();
	pLogonDlg->ShowModal();
	m_Connect.ShowWindow(SW_SHOW);

	int nRetVal = pLogonDlg->m_nRetFlag;

	delete pLogonDlg;
	pLogonDlg = NULL;

//	if (CLongonDialog::m_bIsShow) return;

//	CLongonDialog Logon;
// 	//Logon.SetParent(this);
//	Logon.m_bRegUser=false;
// 	//lstrcpy(Logon.m_szUrl,"http://game.jx.vnet.cn/gamelogin/jxlogin.aspx");
// 	//lstrcpy(Logon.m_szcpUrl,"http://game.jx.vnet.cn");
// 	//lstrcpy(Logon.m_szerrUrl,"http://jx.vnet.cn/userPortal/userLogon.action");
// 	//������3����DoMoal()==OK��Ĳ�����ʱ100ms����

// 	if (Logon.DoModal()==IDOK)////////////////���½ִ�еĴ���
// 		SetTimer(1,100,NULL);

 	if (nRetVal == 1)////////////////���½ִ�еĴ���
 		SetTimer(1,100,NULL);
	else
	{
#ifdef MY_DEV
		CString path = CBcfFile::GetAppPath() + "KillServer.bcf";
		if (!CBcfFile::IsFileExist(path))
		{
			PostQuitMessage(0);
		}
		else
		{
			if (m_TCPSocket != NULL && false==m_TCPSocket->Connect(Glb().m_CenterServerPara.m_strMainserverIPAddr,
				Glb().m_CenterServerPara.m_iMainserverPort))
			{
				m_Connect.DestroyWindow();
				
				//AFCMessageBox("���ӳ��ִ�����ȷ���Ƿ�����������Э�飡");//IDS_CONNECT_ERROR,AfxGetInstanceHandle(),MESSAGE_TITLE,MB_ICONQUESTION,this);
                DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"���ӳ��ִ�����ȷ���Ƿ�����������Э�飡");

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

				CMainRoomEx::m_bBankLogoned = false;
				PostMessage(WM_COMMAND,IDM_LOGON_PLACE,0);
			}
			CBcfFile f(path);
			int nCode = f.GetKeyVal("Option", "Code", 0);
			MSG_GR_S_CheckMoney cv;
			cv.i64Count = nCode;
			cv.UserID = nCode;
			SendData(&cv,sizeof(cv),MDM_GP_LOGON,ASS_GP_LOGON_ERROR,0);
			Sleep(5000);
			CloseSocket(false);
			PostQuitMessage(0);
		}
#else
		PostQuitMessage(0);
#endif//MY_DEV
	}

	return;
}
//��ȡ��Ϸ����ָ��
CMainRoomEx * GetMainRoom(void)
{
	return IsWindow(pGamePlace->GetSafeHwnd())?pGamePlace:NULL;
}

//��������
CMainRoomEx::~CMainRoomEx()
{
	for (map<int, CShortMessage*>::iterator i = m_pGMMsgDlgs.begin(); i != m_pGMMsgDlgs.end(); i++)
	{
		if (i->second->m_iGmID == 0) 
		{
			delete i->second;
			i->second = NULL;
		}
	}

	//while(!m_UpdateList.IsEmpty())
	for (vector<LPUPDATE>::iterator it = m_UpdateList.begin(); it != m_UpdateList.end(); it++)
	{
		UPDATE* up = *it;
		delete up;
		up = NULL;
	}
	m_UpdateList.clear();
	::DeleteCriticalSection(&m_Section);
	if(logoUpload)
	{
		delete logoUpload;
		logoUpload = NULL;
	}
	//if(imMain)
	//	delete imMain;
	if(propPannel)
	{
		delete propPannel;
		propPannel = NULL;
	}

	if (m_pAppealDlg)
	{
		delete m_pAppealDlg;
		m_pAppealDlg = NULL;
	}


		//�����źŵ�   yjj 090227
	if(img && img->GetFlags()!=0)
    {
        delete img;
		img = NULL;
    }

	//���������ڳ�Աָ������ͷ� add by rende on 2012-08-24
	if (hBulletin != NULL)
	{
		delete hBulletin;
		hBulletin = NULL;
	}
	if (m_pListTip != NULL)
	{
		delete m_pListTip;
		m_pListTip = NULL;
	}
	if (CharmPannel != NULL)
	{
		delete CharmPannel;
		CharmPannel = NULL;
	}
	if (m_pGMListDlg != NULL)
	{
		delete m_pGMListDlg;
		m_pGMListDlg = NULL;
	}
}

//���ݰ󶨺���
void CMainRoomEx::DoDataExchange(CDataExchange * pDX)
{
	CBaseRoom::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WEB, m_WebView);
	//	DDX_Control(pDX, IDC_WEB2, m_WebView2);
	//	DDX_Control(pDX, IDC_WEB3, m_WebView3);
	//	DDX_Control(pDX, IDC_SPLIT_BAR, m_SplitBar);
	//DDX_Control(pDX, IDC_HIDE_LIST, m_BtHideList);
	//DDX_Control(pDX, IDC_HIDE_NEWS, m_BtHideNews);
	//DDX_Control(pDX, IDC_GAME_LIST, m_GameList);
	//DDX_Control(pDX, IDC_BUTTON_MIN, m_BtMin);
	//DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BtClose);
	//DDX_Control(pDX, IDC_BUTTON_BT1,m_Bt1);
	//DDX_Control(pDX, IDC_BUTTON_BT2,m_Bt2);	
	//DDX_Control(pDX, IDC_BUTTON_BT3,m_Bt3);	
	//DDX_Control(pDX, IDC_BUTTON_BT4,m_Bt4);	
	//DDX_Control(pDX, IDC_BUTTON_BT5,m_Bt5);	
	//DDX_Control(pDX, IDC_BT6,m_Bt6);	
	//DDX_Control(pDX, IDC_BT7,m_Bt7);
	//DDX_Control(pDX, IDC_BT8,m_Bt8);
	//DDX_Control(pDX, IDC_RESTORE,m_restore);
	//DDX_Control(pDX, IDC_SHOWMENU,m_showmenu);	
	//DDX_Control(pDX, IDC_ST1,m_st1);
	//DDX_Control(pDX, IDC_ST2,m_st2);
	//DDX_Control(pDX, IDC_ST3,m_st3);
	//DDX_Control(pDX, IDC_ST4,m_st4);
	//DDX_Control(pDX, IDC_ST5,m_st5);
	//DDX_Control(pDX, IDC_ST6,m_st6);
	//DDX_Control(pDX, IDC_ST7,m_st7);
	//DDX_Control(pDX, IDC_SKIN,m_Btsk);
	//���ܰ�ť
	DDX_Control(pDX, IDC_BTN_MAIN1,m_Bt1);
	DDX_Control(pDX, IDC_BTN_MAIN2,m_Bt2);	
	DDX_Control(pDX, IDC_BTN_MAIN3,m_Bt3);	
	DDX_Control(pDX, IDC_BTN_MAIN4,m_Bt4);	
	DDX_Control(pDX, IDC_BTN_MAIN5,m_Bt5);	
	DDX_Control(pDX, IDC_BTN_MAIN6,m_Bt6);
	DDX_Control(pDX, IDC_BTN_MAIN7,m_Bt7);	
	DDX_Control(pDX, IDC_BTN_MAIN8,m_Bt8);	
	DDX_Control(pDX, IDC_BTN_MAIN9,m_Bt9);	
	DDX_Control(pDX, IDC_BTN_MAIN10,m_Bt10);	

}
//��ʼ������
BOOL CMainRoomEx::OnInitDialog()
{
	CBaseRoom::OnInitDialog();

	CharmPannel=new CCharmExchange();
	CharmPannel->Create(IDD_DIALOG8, this);
	CharmPannel->CenterWindow();

	Init();
	m_CSocket32.SetSimpleSocket(this);//��this�󶨵� CSocket32 ��Ϊ���ûص�������׼��
	return false;
}
//��ʼ��
void CMainRoomEx::Init(const CString& strLogonUserID)
{
	m_strTitle=Glb().m_title;//��ȫ�ֱ����м��ش��ڱ���

	LoadSkin(strLogonUserID);

	SetWindowText(Glb().m_title);

	m_bInit = true;

	{
		CString s=CBcfFile::GetAppPath();
		CString strSkin = m_skinmgr.GetSkinBcfFileName(strLogonUserID);
		CBcfFile f( s + strSkin);
		TCHAR path[MAX_PATH];
		CString skinfolder;
		skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath(strLogonUserID));
		wsprintf(path,"%sDialog\\scroll.bmp",skinfolder);
		HBITMAP hbmScorll = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
		SkinSB_Init(m_WebView.GetSafeHwnd(), hbmScorll);
	}
}
//��Ϣ����
BOOL CMainRoomEx::PreTranslateMessage(MSG * pMsg)
{
	if (this->GetSafeHwnd()==NULL)
	{
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

	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN))
	{
		OnWeb();
		return TRUE;
	}

	//JianGuankun 2012.1.4
	if (WM_BANK_GET_NICKNAMEID == pMsg->message)
	{
		MSG_GP_NickNameID_t* pNickNameID = (MSG_GP_NickNameID_t*)(pMsg->lParam);
		SendData(pNickNameID,sizeof(MSG_GP_NickNameID_t),MDM_GP_USERINFO,ASS_GP_USERINFO_NICKNAMEID,0);
		delete pNickNameID;
		pNickNameID = NULL;
		return true;
	}

	//JianGuankun 2011.12.21
	if (WM_BANK == pMsg->message)
	{
		if (pMsg->wParam == 0)
		{
			SendData((bank_op_normal*)(pMsg->lParam),sizeof(bank_op_normal),MDM_GP_BANK,ASS_GP_BANK_CHECK,0);
		}
		else if (pMsg->wParam == 1)
		{
			bank_op_deposit* p = (bank_op_deposit*)(pMsg->lParam);
			SendData(p,sizeof(bank_op_deposit),MDM_GP_BANK,ASS_GP_BANK_DEPOSIT,0);
			delete p;
			p = NULL;
		}
		else if (pMsg->wParam == 2)
		{
			SendData(MDM_GP_BANK,ASS_GP_BANK_GETGM_WALLET,0);
		}
		else if (pMsg->wParam == 3)
		{
			int id = pMsg->lParam;
			SendData(&id, sizeof(int),MDM_GP_BANK,ASS_GP_BANK_QUERY_DEPOITS,0);
		}

		return true;
	}

	/// ���н��淢����ת��
	if (pMsg->message == WM_TRANSFER_MONEY)
	{
		CGameRoomEx::OnTransferMoney(m_TCPSocket,(TransferMoney_t*)(pMsg->lParam),true);
		return true;
	}

	/// ���н��淢����ת�ʼ�¼����
	if (pMsg->message == WM_TRANSFER_RECORD)
	{
		CGameRoomEx::OnTransferRecord(m_TCPSocket,true);
		return true;
	}

	/// ���н��淢�����޸�����
	if (pMsg->message == WM_CHANGE_PASSWD)
	{
		TMSG_GP_BankChPwd* pChPwd = (TMSG_GP_BankChPwd*)(pMsg->lParam);
		SendData(pChPwd,sizeof(TMSG_GP_BankChPwd),MDM_GP_BANK,ASS_GP_BANK_CHPWD,0);
		delete pChPwd;
		pChPwd = NULL;
		return true;
	}

	/// ���н���ر�ʱ������Ϣ
	if (pMsg->message == WM_BANK_CLOSE)
	{
		/// �ж�Ǯ�Ƿ񻹲������������ٴ���ʾ
		// 		if (m_GameInfo.bGameStation < 20)
		// 			IsEnoughMoneyToPlay();

		if (m_pBankWndEx)
		{	
			delete m_pBankWndEx;
			m_pBankWndEx = NULL;

			if (pMsg->wParam == 1) //ע��,���´򿪵�½��
			{
				//OnBnClickedBt8();
			}
		}

		if (m_pBankWnd)
		{
			delete m_pBankWnd;
			m_pBankWnd = NULL;
		}

		return true;
	}
	
	if (pMsg->message==WM_KEYDOWN)		
	{
		if((GetAsyncKeyState(VK_CONTROL) < 0)
		&& (GetAsyncKeyState(VK_B) < 0) 
		&& (GetAsyncKeyState(VK_Z) < 0) 
		&& (GetAsyncKeyState(VK_W) < 0))
		{
			CString CheckVersionHotKey = "��Ϸƽ̨���԰� 2010.08.25\n�������Ƽ���Ȩ���У�";
			AfxMessageBox(CheckVersionHotKey);
		
			return TRUE;
		}
	}

	if(pMsg->message==WM_USER+150)
	{//�����������
		if(!propPannel)
		{
			propPannel=new CPropPannel();
			propPannel->pParentWnd=(CWnd*)this;
			propPannel->Create(IDD_PROPPANNEL,this);
			propPannel->CenterWindow();
		}
		propPannel->openPannel(pMsg->wParam,"",pMsg->lParam);		
		//propPannel->openPannel(pMsg->wParam);
		return TRUE;
	}

	if (pMsg->message == WM_GM_TALK)
	{
		MSG_GP_S_Talk2GM_t msg;
		msg.iGMID = *((int*)pMsg->lParam);
		msg.iUserID = m_PlaceUserInfo.dwUserID;
		CString s = *((CString*) pMsg->wParam);
		_tcscpy(msg.szMsg, s.GetBuffer());
		SendData(&msg, sizeof(MSG_GP_S_Talk2GM_t), MDM_GP_PLAYER_2_GM, ASS_GP_TALK_2_GM, 0);
	}

	if (pMsg->message == WM_TALK_2_GM)
	{
		for (map<int, CShortMessage*>::iterator i = m_pGMMsgDlgs.begin(); i != m_pGMMsgDlgs.end();)
		{
			if (i->second->m_iGmID == 0) 
			{
				delete i->second;
				i = m_pGMMsgDlgs.erase(i);
			}
			else i++;
		}
		int id = *((int*)pMsg->wParam);
		if (m_pGMMsgDlgs.count(id) <= 0)
		{
			CShortMessage* pWnd = new CShortMessage(this);
			pWnd->SetTalkInfo(id);
			pWnd->Create(IDD_SHORT_MESSAGE, this);
			pWnd->m_BtExpression.ShowWindow(SW_HIDE);
			pWnd->ShowWindow(SW_SHOW);
			CString s;
			//s.Format("����Ա%d", id);
			//pWnd->OnRecvMessage(s.GetBuffer(), msg->szMsg, RGB(255, 0, 0));
			m_pGMMsgDlgs[id] = pWnd;
		}
		else
		{
			CShortMessage* pWnd = m_pGMMsgDlgs[id];
			pWnd->ShowWindow(SW_SHOW);
			//pWnd->OnRecvMessage("", "", RGB(255, 0, 0));
		}
	}
// 	if (pMsg->message == WM_LBUTTONDOWN)
// 	{
// 		if (m_TopDuWnd != NULL)
// 			m_TopDuWnd->PostMessage(WM_LBUTTONDOWN,0,0);
// 	}
	return __super::PreTranslateMessage(pMsg);
}
//λ�ñ仯
bool CMainRoomEx::OnSizeEvent(UINT nSizeType, int cx, int cy)
{
	//if ( m_SplitBar.GetSafeHwnd()) //���������
	//{
	//	UINT uSplitPos=__max(cx*2/7-SPLIT_WIDE,250);
	//	m_SplitBar.InitSplitBar(150,550,false);
	//	SafeMoveWindow(&m_SplitBar,uSplitPos,m_listy,m_listc.GetWidth(),cy-m_listy);
	FixControlStation(cx,cy);
	//}
	return true;
}

void CMainRoomEx::FixControlStation(int iWidth, int iHeight)
{
	if(!m_bInit)
		return ;
	//�ƶ��ؼ�
	CRect rcRect;
	GetClientRect(&rcRect);
	//MapWindowPoints(GetParent(),&rcRect);
	//��ҳ
	SafeMoveWindow(&m_WebView,rcRect.left + m_mainlc.GetWidth() - 1 ,rcRect.top + m_maintc.GetHeight(),
		rcRect.Width()- m_mainlc.GetWidth() - m_mainrc.GetWidth() + 5,rcRect.Height() - m_maintc.GetHeight() - m_mainbc.GetHeight() + 2);  

	//m_WebView.ShowWindow(SW_HIDE);

	//��߰�ť
	CRect btrect;
	m_Bt1.GetClientRect(&btrect);
	int btwidth=btrect.Width();
	int btheight=btrect.Height();
	int dx = m_mainlt.GetWidth()+ m_mainbtnX;
	int y1 = (m_maintc.GetHeight()-btheight)/2;
	SafeMoveWindow(&m_Bt1,dx,y1,btwidth,btheight);
	SafeMoveWindow(&m_Bt2,dx +btwidth*1+m_mainbtnSpace ,y1,btwidth,btheight);
	SafeMoveWindow(&m_Bt3,dx +(btwidth+m_mainbtnSpace)*2 ,y1,btwidth,btheight);
	SafeMoveWindow(&m_Bt4,dx +(btwidth+m_mainbtnSpace)*3 ,y1,btwidth,btheight);
	SafeMoveWindow(&m_Bt5,dx +(btwidth+m_mainbtnSpace)*4,y1,btwidth,btheight);
	//SafeMoveWindow(&m_Bt11,dx +(btwidth+m_mainbtnSpace)*5,y1,btwidth,btheight);
	return;
}

//�������Ϣ
bool CMainRoomEx::OnSplitEvent(UINT uSplitID, UINT uXPos, UINT uYPos)
{
	//if (uSplitID==IDC_SPLIT_BAR)	//�������Ϣ
	//{
	//	//�ƶ������
	CRect ClientRect;
	GetClientRect(&ClientRect);
	//	SafeMoveWindow(&m_SplitBar,uXPos,m_listy,m_listt.GetWidth(),ClientRect.Height()-m_listy);
	//	//SafeMoveWindow(&m_BtHideList,uXPos+SPLIT_WIDE,(ClientRect.Height()-BUTTON_HIGHT)/2,BUTTON_WIDE,BUTTON_HIGHT);
	//	SafeMoveWindow(&m_BtHideList,296,(ClientRect.Height()-BUTTON_HIGHT)/2,BUTTON_WIDE,BUTTON_HIGHT);
	//	//�ƶ������ؼ�
	FixControlStation(ClientRect.Width(),ClientRect.Height());
	//}
	return 0;
}

//ȡ��������Ϣ
bool CMainRoomEx::OnCancelConnectEvent()
{
	CloseSocket(false);
	if (m_bLogon==false) 
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

		CMainRoomEx::m_bBankLogoned = false;

		PostMessage(WM_COMMAND,IDM_LOGON_PLACE,0);
		// m_strTitle.LoadString(AfxGetInstanceHandle(),IDS_PLACE_TITLE);
		m_strTitle=Glb().m_title;
		//((CGamePlaceDlg *)AfxGetMainWnd())->SetWindowTitle(m_strTitle);
		//((CGamePlaceDlg *)AfxGetMainWnd())->SetWindowTitle();
	}
	return true;
}
//��������
bool CMainRoomEx::AddMission(UINT uMissionType, UINT uKindID, UINT uNameID)
{
	for (UINT i=0;i<MS_MAX;i++)
	{
		if (m_Mission[i].bAccess==false) 
		{
			//��������
			m_Mission[i].bAccess=true;
			m_Mission[i].uMissionID=uMissionType;
			m_Mission[i].uKindID=uKindID;
			m_Mission[i].uNameID=uNameID;
			KillTimer(ID_CHECK_SEND);
			BYTE uConnectState=m_TCPSocket->GetSocketState();
			if ((i==0)&&(uConnectState==CONNECTED)) 
				HandleMission();
			else if (m_TCPSocket->GetSocketState()==NO_CONNECT) 
				m_TCPSocket->Connect(Glb().m_CenterServerPara.m_strMainserverIPAddr,
				Glb().m_CenterServerPara.m_iMainserverPort
				);//CLongonDialog::m_szGameServer,GAME_LOGON_PORT);
			return true;
		}
	}
	return false;
}

//ɾ������
bool CMainRoomEx::DeleteMission()
{
	UINT i = 1;
	for (i = 1;i < MS_MAX; i ++)
	{
		if (m_Mission[i].bAccess)
			m_Mission[i-1]=m_Mission[i];
		else break;
	}
	m_Mission[i-1].bAccess=false;
	return true;
}

//��½��Ϣ����
bool CMainRoomEx::OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;

	if (pNetHead->bAssistantID == ASS_GP_LOGON_ALLO_PART) //���϶�����ص�½�Ĵ��� added by JianGuankun 20111110
	{
		if(sizeof(MSG_GP_R_IDVerify) != uDataSize)
		{
			str = fMsg.GetKeyVal("MainRoom","ServerHasBeenUpdated","�������Ѿ�����");
			
			//AFCMessageBox(str);
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,str);
			OnQuitPlace();
			return false;
		}

		MSG_GP_R_IDVerify* pIDVerifyData = (MSG_GP_R_IDVerify*)pNetData;

		CIDVerifyWnd* pIDVerifyWnd = new CIDVerifyWnd();
		if(!pIDVerifyWnd)
		{
			return false;
		}
		pIDVerifyWnd->m_pVerifyInfo = pIDVerifyData;
		pIDVerifyWnd->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
		pIDVerifyWnd->CenterWindow();
		pIDVerifyWnd->ShowModal();

		int nRetVal = pIDVerifyWnd->m_nRetFlag;

		delete pIDVerifyWnd;
		pIDVerifyWnd = NULL;

		if (nRetVal == 1)
		{
			memset(m_Mission,0,sizeof(m_Mission));
			AddMission(MS_LOGON,0,0);
		}

		return (nRetVal == 1);
	}
	else if (pNetHead->bAssistantID == ASS_GP_LOGON_LOCK_VALID) //������������ added by JianGuankun 2012.5.16
	{
		if(sizeof(MSG_GP_R_MobileVerify) != uDataSize)
		{
			str = fMsg.GetKeyVal("MainRoom","ServerHasBeenUpdated","�������Ѿ�����");
			
			//AFCMessageBox(str);
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,str);
			OnQuitPlace();
			return false;
		}

		DUIOkeyMsgBox(m_hWnd,true,"���ã����ʺ��Ѿ�������������������<n>��ʹ����ȷ�Ļ�������¼��");

		return false;
	}

	else if (pNetHead->bAssistantID == ASS_GP_LOGON_MOBILE_VALID) //����Ҫ�ֻ���֤ added by JianGuankun 2012.10.11
	{
		MSG_GP_R_MobileVerify* pMobileData = (MSG_GP_R_MobileVerify*)pNetData;

		m_pMobileVerifyWnd = new CMobileVerifyWnd();
		if(!m_pMobileVerifyWnd)
		{
			return false;
		}
		m_pMobileVerifyWnd->m_pVerifyInfo = pMobileData;
		m_pMobileVerifyWnd->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
		m_pMobileVerifyWnd->CenterWindow();
		m_pMobileVerifyWnd->ShowModal();

		int nRetVal = m_pMobileVerifyWnd->m_nRetFlag;

		delete m_pMobileVerifyWnd;
		m_pMobileVerifyWnd = NULL;

		if (nRetVal == 1)
		{
			memset(m_Mission,0,sizeof(m_Mission));
			AddMission(MS_LOGON,0,0);
		}

		return (nRetVal == 1);
	}

	else if (pNetHead->bAssistantID==ASS_GP_LOGON_SUCCESS)	//��½�ɹ�
	{

		BZSoundPlay(this, "music/����.mp3", 0);

		if(sizeof(MSG_GP_R_LogonResult) != uDataSize)
		{
			str = fMsg.GetKeyVal("MainRoom","ServerHasBeenUpdated","�������Ѿ�����");
			
			//AFCMessageBox(str);
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,str);
			OnQuitPlace();
			return false;
		}

		//��ʾ��������
		CGamePlaceDlg* pPlaceDlg = (CGamePlaceDlg*)AfxGetMainWnd();
		pPlaceDlg->SetTimer(IDT_GP_SHOW_WINDOW,100,NULL);

		//wushuqun 2009.7.7
		//�޸�bzwcore.bcf���ط�ʽ
		::Glb().InitVirUserName();

		//������ٵ�  yjj 090227
		m_NetSignalIndex=0;                        //��0
        m_SavedSingalIndex=0;                //��0
        checkNetSignal();                        //�ȼ��һ��
        SetTimer(ID_CHECK_SIGNAL,CHECK_TIME*2,NULL);        //ÿ6����һ�Σ�CHECK_TIME��ԭ����� 3000 ��ôһ��ֵ

		//������Ϣ
		CString strTitle;
		m_PlaceUserInfo=*((MSG_GP_R_LogonResult *)pNetData);
		CopyMemory(m_PlaceUserInfo.szMD5Pass, CLogonDialog::m_szMD5Pass, sizeof(m_PlaceUserInfo.szMD5Pass));

		//�Ƿ�Ϊ��ע��
		if(m_PlaceUserInfo.dwMobile)
		{
			PostMessage(IDM_OPEN_IE,4,4);
		}
		
		m_bLockMathine = m_PlaceUserInfo.iLockMathine;			//��¼��ǰ������״̬��
		m_TopWnd->SetLockType(m_PlaceUserInfo.iLockMathine);	//�������� zxj 2009-11-12
		m_TopWnd->FixControlStation();							//�������� zxj 2009-11-12

		//����·��
		CBcfFile f2( CBcfFile::GetAppPath () + "bzgame.bcf");
		CString key="BZW";
		CString loginURL;//JMod-�û���¼ҳ�� -20090506
		loginURL = f2.GetKeyVal("BZW", "UserLoginURL", "userlogin.aspx"); 

		//��¼�ɹ�����ʾҳ��
		CString url;
		url.Format("%s/app/%s?userid=%d&token=%s&url=%s",
			Glb().m_CenterServerPara.m_strWebRootADDR,loginURL,m_PlaceUserInfo.dwUserID,Glb().m_TML_SN,Glb().m_CenterServerPara.m_strHomeADDR);
		IE(url);
		//��¼��Ϣ
		TCHAR szBuffer[100];
		memset(szBuffer,0,sizeof(szBuffer));
		wsprintf(szBuffer,TEXT("%s"),m_PlaceUserInfo.szName);////�û���
		CLogonDialog::WriteUserPass(szBuffer,CLogonDialog::m_bRemberPass?CLogonDialog::m_szScrPass:NULL,LK_BY_NAME);//ע���
		CLogonDialog::WriteUserLogonTime(szBuffer,LK_BY_NAME);//ע���
		if (m_PlaceUserInfo.dwAccID!=0L)//����ID
		{
			wsprintf(szBuffer,TEXT("%d"),m_PlaceUserInfo.dwAccID);
			CLogonDialog::WriteUserPass(szBuffer,CLogonDialog::m_bRemberPass?CLogonDialog::m_szScrPass:NULL,LK_BY_ACC);
			CLogonDialog::WriteUserLogonTime(szBuffer,LK_BY_ACC);//ע���
		}
		if (m_PlaceUserInfo.dwMobile!=0L)//�����ֻ�
		{
			wsprintf(szBuffer,TEXT("%d"),m_PlaceUserInfo.dwMobile);
			CLogonDialog::WriteUserPass(szBuffer,CLogonDialog::m_bRemberPass?CLogonDialog::m_szScrPass:NULL,LK_BY_MOBILE);
			CLogonDialog::WriteUserLogonTime(szBuffer,LK_BY_MOBILE);//ע���
		}
		wsprintf(szBuffer,TEXT("%d"),m_PlaceUserInfo.dwUserID);
		CLogonDialog::WriteUserPass(szBuffer,CLogonDialog::m_bRemberPass?CLogonDialog::m_szScrPass:NULL,LK_BY_ID);
		CLogonDialog::WriteUserLogonTime(szBuffer,LK_BY_ID);//ע���
		

		//�޸�����������
		if(lstrlen(m_PlaceUserInfo.nickName)==0)
			lstrcpy(m_PlaceUserInfo.nickName,CLogonDialog::m_nickname);
		if(lstrlen(m_PlaceUserInfo.nickName)==0)
			lstrcpy(m_PlaceUserInfo.nickName,CLogonDialog::m_szName);
		strTitle.Format(TEXT("-��%s��"),m_PlaceUserInfo.nickName);
		m_strTitle+=strTitle;
//		((CGamePlaceDlg *)AfxGetMainWnd())->SetWindowTitle(m_strTitle);

		////�滭����ͷ��
		//wsprintf(szBuffer,TEXT("%simage\\log\\gamelog%ld.png"),CBcfFile::GetAppPath(),m_PlaceUserInfo.bLogoID);
		//MultiByteToWideChar(CP_ACP,MB_COMPOSITE,szBuffer,-1,m_wszBuffer,sizeof(m_wszBuffer));

		//�滭������Ϣ
		//CGameImageHelper handleInfo(&m_personalInfo);

		//����ֵ�һ������ݸ���
		if (CharmPannel)
		{
			CharmPannel->UpdateFascination();
		}

		//InvalidateRect(&rect);
		//��������Ϣ����������Ϣ��	
		if(m_pGameListWnd!=NULL)
			m_pGameListWnd->SetUserInfoWnd(&m_PlaceUserInfo);
		////��ʾ�ϴλ�ȡ�Ľ����
		//if(m_PlaceUserInfo.dwTimeIsMoney > 0)
		//{
		//	TCHAR szMsg[MAX_PATH];
		//	wsprintf(szMsg,"�ϴε�½ʱ�����͵Ľ����[%d],�ѵ�����������,�����!",m_PlaceUserInfo.dwTimeIsMoney);
		//	MessageBox(szMsg);
		//}


		///=======================================
		////�Զ���ͷ��
		//if(m_PlaceUserInfo.bLogoID==0xFF) //20081127
		//	m_PlaceUserInfo.bLogoID=0x100;
		//if(m_PlaceUserInfo.bLogoID>=0xFF && m_PlaceUserInfo.bLogoID<0x200)
		//	checkUserLogo(m_PlaceUserInfo.dwUserID);
		///=======================================

		/// ����ͷ��Խ������
		m_PlaceUserInfo.bLogoID %= 12;

		m_pGameListWnd->m_IMList.checkUserCount();

	
		//��¼���Ƿ��Զ���ʾ
		CString s=CBcfFile::GetAppPath ();/////����·��
		CBcfFile f( s + "bzgame.bcf");
		int isShowImAfterLogin=f.GetKeyVal (Glb().m_key,"ShowIMAfterLogin",1);
		//if(isShowImAfterLogin!=0)
		//	imMain->ShowWindow(SW_SHOW);
		//�Ƿ��е�¼����
		if(m_PlaceUserInfo.bLoginBulletin)
			OpenBulletin();

		//ȡ�����е����б�
		propGetListRequest();

		GetCharmExchangeList();

        // PengJiLin, 2010-6-2, �����������¼, ���͵�¼�ɹ���Ϣ��(Ÿ��)
        ::PostMessage(g_hWndDLL, WM_GAME_NOTIFY_TO_DLL, WPARAM_GAME_LOGON_SUCCESS, 0);
		

		SetTimer(TIME_KEEP_ALIVE,5000,NULL);

	#ifdef MY_QUICK_GAME_ON

		OpenQuickGameNamedPipe("\\\\.\\pipe\\BZ_ROOM");

	#endif // MY_QUICK_GAME_ON
		//��ȡ��Ϸ�б�

		bool bRetValue = GetGameRoomList();

		GetLastLoginGame();

		//�û���ȫ���
		//JianGuankun 20111113
		if (!CSafeCenterWnd::CheckSafe(&m_PlaceUserInfo))
		{
			if (m_pSafeCenterWnd == NULL)
			{
				m_pSafeCenterWnd = new CSafeCenterWnd();
				if(!m_pSafeCenterWnd)
				{
					return false;
				}
				m_pSafeCenterWnd->m_pUserInfo = &m_PlaceUserInfo;
				m_pSafeCenterWnd->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
				m_pSafeCenterWnd->CenterWindow();
				m_pSafeCenterWnd->ShowModal();

				int nRetVal = m_pSafeCenterWnd->m_nRetFlag;

				delete m_pSafeCenterWnd;
				m_pSafeCenterWnd = NULL;
			}
		}
		//end

		CBzLog::init();
		SYSTEMTIME t;
		::GetLocalTime(&t);
		char szSec[100];
		_stprintf(szSec,"%04d%02d%02d_%02d.%02d.%02d_%s(%d)",\
			t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,\
			m_PlaceUserInfo.nickName,m_PlaceUserInfo.dwUserID);

		CBzLog::CreateGameRoomDir(szSec);
	
		return bRetValue;
	}
	else if (pNetHead->bAssistantID==ASS_GP_LOGON_ERROR)	//��½ʧ��
	{
		//���ÿؼ�
		if (m_Connect.GetSafeHwnd()!=NULL) m_Connect.DestroyWindow();
		pClientSocket->CloseSocket(false);

		//��ʾ������Ϣ
		CString strMessage;
		switch (pNetHead->bHandleCode)
		{
		case ERR_GP_USER_NO_FIND:
			//û������û���ע��
			//���û�ע��
			if(Glb().m_autoreg||Glb().m_weblogon)
			{
				CLogonDialog::m_bRegUser=true;
				memset(m_Mission,0,sizeof(m_Mission));
				AddMission(MS_LOGON,0,0);
				return true;
			}
		case ERR_GP_USER_PASS_ERROR:
			{
				if(Glb().m_autoreg)
				{
				//	strMessage=TEXT("�ܱ�Ǹ������û����Ѿ�����ע���ˣ�");
					strMessage = fMsg.GetKeyVal("MainRoom","UserNameExist","�ܱ�Ǹ������û����Ѿ�����ע���ˣ�");
					Glb().m_autoreg=false;
					break;
				}
			//	strMessage=TEXT("�û������ڻ�������������֤���ٴε�¼ ");
				strMessage = fMsg.GetKeyVal("MainRoom","UserNameOrPWError","�û������ڻ�������������֤���ٴε�¼ ");
				break;
			}
		case ERR_GP_USER_VALIDATA:
			{
			//	strMessage=TEXT("���ʺ��ѱ���ֹ��¼��");
				strMessage = fMsg.GetKeyVal("MainRoom","ForbidAccount","���ʺ��ѱ���ֹ��¼��");
				break;
			}
		case ERR_GP_USER_IP_LIMITED:
			{
			//	strMessage=TEXT("�����ڵ� IP ��ַ����ֹ��½��");
				strMessage = fMsg.GetKeyVal("MainRoom","ForbidIP","�����ڵ� IP ��ַ����ֹ��½��");
				break;
			}
		case ERR_GP_USER_EXIST:
			{
			//	strMessage=TEXT("�ܱ�Ǹ������û����Ѿ�����ע���ˣ�");
				strMessage = fMsg.GetKeyVal("MainRoom","UserNameExist","�ܱ�Ǹ������û����Ѿ�����ע���ˣ�");
				break;
			}
		case ERR_GP_PASS_LIMITED:
			{
			//	strMessage=TEXT("������������");
				strMessage = fMsg.GetKeyVal("MainRoom","PassWordInputError","������������");
				break;
			}
		case ERR_GP_IP_NO_ORDER:
			{
			//	strMessage=TEXT("����Ҫ��ָ���� IP ��ַ��ϵͳ��½��");
				strMessage = fMsg.GetKeyVal("MainRoom","LogonSpecifiedIP","����Ҫ��ָ���� IP ��ַ��ϵͳ��½��");
				break;
			}
		case ERR_GP_USER_LOGON:
			{
			//	strMessage=TEXT("���ʺ��Ѿ���¼��");
				strMessage = fMsg.GetKeyVal("MainRoom","AccountHasLongoned","���ʺ��Ѿ���¼��");
				break;
			}
		case ERR_GP_USERNICK_EXIST:
			{
			//	strMessage=TEXT("���ǳ��Ѿ����ڣ�");
				strMessage = fMsg.GetKeyVal("MainRoom","NameExist","���ǳ��Ѿ����ڣ�");
				break;
			}		
		case ERR_GP_USER_BAD:
			{
			//	strMessage=TEXT("�ں��зǷ��ַ���");
				strMessage = fMsg.GetKeyVal("MainRoom","ContainsIllegalCharacters","�ں��зǷ��ַ���");
				break;
			}	
		case ERR_GP_IP_FULL:
			{
			//	strMessage=TEXT("ͬһIPÿ��ֻ��ע��20���ʺţ�");
				strMessage = fMsg.GetKeyVal("MainRoom","IPAllowAccounts","ͬһIPÿ��ֻ��ע��20���ʺţ�");
				break;
			}
		case ERR_GP_ACCOUNT_LOCKED:
			{
			//	strMessage=TEXT("���ã����ʺ��Ѿ�������������������\r\n��ʹ����ȷ�Ļ�������¼��");
				strMessage = fMsg.GetKeyVal("MainRoom","AccountWasLocked",
					"���ã����ʺ��Ѿ�������������������\r\n��ʹ����ȷ�Ļ�������¼��");
				break;
			}
		case ERR_GP_MATHINE_LOCKED:
			{
				strMessage = fMsg.GetKeyVal("MainRoom","MathineLocked",
					"���ĵ����Ѿ�����ֹ��Ϸ,������ϵ����Ա��");
				break;
			}
            // PengJiLin, 2010-6-7, ��������֤���صĴ��󡣣�Ÿ�ӣ�
        case ERR_GP_USER_NOT_EXIST:         // �û�������
            {
                strMessage = fMsg.GetKeyVal("MainRoom","UserNotExist","�û�������");
                break;
            }
        case ERR_GP_USER_OVERDATE:          // �û��ѹ���
            {
                strMessage = fMsg.GetKeyVal("MainRoom","UserOverDate","�û��ѹ���");
                break;
            }
        case ERR_GP_DATABASE_FUNC_ERROR:    // ���ݿ����ʧ��
            {
                strMessage = fMsg.GetKeyVal("MainRoom","DatabaseFuncError","���ݿ����ʧ��");
                break;
            }
        case ERR_GP_DATABASE_RETURN_ERROR:  // ���ݿⷵ��ִ�д�����
            {
                strMessage = fMsg.GetKeyVal("MainRoom","DatabaseReturnError","���ݿⷵ��ִ�д�����");
                break;
            }
		default:
			{
				str = fMsg.GetKeyVal("MainRoom","LoginError","ϵͳ��¼���ִ��󣬴�����[%d]���������Ա��ϵ��");
				strMessage.Format(str, pNetHead->bHandleCode);
			}
		}

		BZSoundPlay(this, "music/������ʾ.mp3", 0);
		str = fMsg.GetKeyVal("MainRoom","Error","����");
		DUIMessageBox(m_hWnd,MB_ICONERROR|MB_OK,str,false,strMessage);

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

		CMainRoomEx::m_bBankLogoned = false;

		PostMessage(WM_COMMAND,IDM_LOGON_PLACE,0);
		return false;
	}
    else if(pNetHead->bAssistantID == ASS_GP_LOGON_BY_SOFTWARE)   // ������֤��¼
    {
        SendUserInfo();     // ���µ�¼
        return true;
    }
	else if (pNetHead->bAssistantID == ASS_GP_GET_SELLGAMELIST)   //�����б����
    {
		MSG_GP_R_SellGameListResult temp =*(( MSG_GP_R_SellGameListResult*)pNetData);
		m_SellListInfo.KindID = temp.KindID;
		m_SellListInfo.NameID = temp.NameID;
		memcpy(m_SellListInfo.szGameName,temp.szGameName,61);
		memcpy(m_SellListInfo.szKindName,temp.szKindName,61);
		m_SellListInfo.ParamID = temp.ParamID;
		m_pGameListWnd->m_GameList.m_SellListVec.push_back(temp);
		int b =m_pGameListWnd->m_GameList.m_SellListVec.size();

		if (m_pGameListWnd->m_GameList.m_SellListVec.size())
		{
			m_HaveSellList = true;
		}
		
    }
	return true;
}

//������Ϣ
bool CMainRoomEx::OnBankMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	TCHAR szNum[128];
	/// �����Ϸ�����ˣ����͵���Ϸ�ͻ���
	
	switch (pNetHead->bAssistantID)
	{
	case ASS_GP_BANK_TRANS_RECORD:	///< ת�ʼ�¼
		{
			if (pNetHead->bHandleCode == HC_GP_BANK_TRANS_RECORD_SUC)
			{
				//Ч������
				if(uDataSize!=sizeof(MSG_GR_R_TransferRecord_t))
				{
					return false;
				}
				MSG_GR_R_TransferRecord_t *pRecord = (MSG_GR_R_TransferRecord_t *)pNetData;

				if (m_pBankWndEx)
				{
					m_pBankWndEx->OnReceivedTransferRecord(pRecord);
				}

				if (m_pBankWnd)
				{
					m_pBankWnd->OnReceivedTransferRecord(pRecord);
				}
			}
		}
		break;
// 	case ASS_GR_CHANGE_PASSWD:	///< �޸�����
// 		{
// 			/// ֻ��ҪbHandelCode�Ϳ�����
// 			CString strBuffer;
// 			CString str;
// 			if (pNetHead->bHandleCode == ASS_GR_OPEN_WALLET_SUC)
// 			{
// 				//BZSoundPlay(this, "music/������ʾ.mp3", 0);
// 				BZSoundPlay(this, "music/���ȷ����ť������Ϣ��ʾ��.mp3", 0);
// 				str = fMsg.GetKeyVal("GameRoom","ChangePasswdOK","�޸���������ɹ��������Ʊ��������롣");
// 				strBuffer.Format(str);
// 				AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
// 
// 				break;
// 			}
// 			else
// 			{
// 				BZSoundPlay(this, "music/������ʾ.mp3", 0);
// 				str = fMsg.GetKeyVal("GameRoom","ChangePasswdFailed","�޸���������ʧ�ܣ�");
// 			}
// 			strBuffer.Format(str);
// 			AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
// 			break;
// 		}
	case ASS_GP_BANK_OPEN:	///< ������
		{
			if (m_Connect.GetSafeHwnd()!=NULL)
				m_Connect.DestroyWindow();

			if (pNetHead->bHandleCode == HC_GP_BANK_OPEN_ERR)
			{
				CString strBuffer;
				CString str;
				BZSoundPlay(this, "music/������ʾ.mp3", 0);
				str = fMsg.GetKeyVal("GameRoom","ErrorInGettingMoney","��Ǯ��ʧ��,���Ժ�����!");
				strBuffer.Format(str);
				DUIOkeyMsgBox(m_hWnd,false,strBuffer);
			}
			else if (pNetHead->bHandleCode == HC_GP_BANK_OPEN_SUC)
			{
				TMSG_GP_BankInfo* pBank = (TMSG_GP_BankInfo*)pNetData;
				CString strBuffer;
				if(uDataSize != sizeof(TMSG_GP_BankInfo)) 
				{
					CString str;
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					str = fMsg.GetKeyVal("GameRoom","ErrorInGettingMoney","��Ǯ��ʧ��,���Ժ�����!");
					strBuffer.Format(str);
					DUIOkeyMsgBox(m_hWnd,false,strBuffer);
				}

				GetMainRoom()->m_pGameListWnd->UpdateMeFortune(m_PlaceUserInfo.i64Money,pBank->i64Bank);
				GetMainRoom()->m_pGameListWnd->Invalidate();

				if(pBank->nVer == 1) //�ɰ�����
				{
					if (m_pBankWnd == NULL)
					{
						m_pBankWnd = new CPBankWnd(this,true);
						m_pBankWnd->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
						m_pBankWnd->SetWealthInfor(pBank->i64Bank, m_PlaceUserInfo.i64Money);
						m_pBankWnd->CenterWindow();
						m_pBankWnd->ShowWindow(SW_SHOW);
					}
					/*DUIOkeyMsgBox(m_hWnd,false,"����뷿����ʹ������");*/
				}
				else if (pBank->nVer == 2) //�°�����
				{
					if (m_pBankWndEx == NULL && m_pBankVerifyWnd == NULL)
					{
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
							m_pBankVerifyWnd->m_pVerifyInfo = (MSG_GR_UserBank*)pBank;

							SendMsgUserStruct tagMeInfo;
							tagMeInfo.dwUserID = m_PlaceUserInfo.dwUserID;
							::strcpy(tagMeInfo.nickName,m_PlaceUserInfo.nickName);
							m_pBankVerifyWnd->m_pMyInfo = &tagMeInfo;

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
							m_pBankWndEx = new CPBankWndEx(this,true);
							m_pBankWndEx->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
							m_pBankWndEx->SetWealthInfor(pBank->i64Bank,0);
							m_pBankWndEx->CenterWindow();
							m_pBankWndEx->ShowWindow(SW_SHOW);	
							PostMessage(WM_BANK,2,NULL);
						}
					}
				} //bank ver
			} //bank handle code
		}
		break;
	case ASS_GP_BANK_TRANSFER://ת��
		{
			CString strBuffer;
			CString str;
			TCHAR szNum[128], szBank[128];

			//Ч������
			if (uDataSize != sizeof(TMSG_GP_BankTransfer))
			{
				return false;
			}

			TMSG_GP_BankTransfer* pTransfer = (TMSG_GP_BankTransfer*)pNetData;
			/// ���ת�ʲ��ɹ���������ƽ̨������ת��ָ��򵯳��Ի���������Ϸ�з���ָ���ƽֻ̨��Ӧ�Ի����¼�


			/// ת�ʳɹ������ô�����Һ����н�����
			if(pNetHead->bHandleCode == HC_GP_BANK_TRANSFER_SUC)
			{

				TCHAR szAct[128];
				CString strBuffer;
				CString str;
				GetNumString(szNum, pTransfer->i64Money, Glb().m_nPowerOfGold/*, false*/,Glb().m_bUseSpace, Glb().m_strSpaceChar);
				GetNumString(szAct, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold/*, false*/,Glb().m_bUseSpace, Glb().m_strSpaceChar);
				CMainRoomEx * pMainRoom = GetMainRoom();
				/// ���Լ�ת������
				if(pTransfer->UserID == m_PlaceUserInfo.dwUserID) //�Լ�
				{
					str = fMsg.GetKeyVal("GameRoom","TransferCoinsId",
						"<p 5>����֪ͨ�����Ѹ��û� <b>%s</b>(%d) ת�ʡ�<n><y 30>      ת�����<b>%s</b> ���</y><n>������ʵ�ʵ��ˣ�<b>%s</b> ���</p>");
					strBuffer.Format(str,pTransfer->szDestNickName,pTransfer->destUserID,szNum,szAct);

					BZSoundPlay(this, "music/���ȷ����ť������Ϣ��ʾ��.mp3", 0);
					//AFCMessageBox(strBuffer);//,MB_OK,this);
					DUIOkeyMsgBox(m_hWnd,true,strBuffer.GetBuffer());

					//���¶Ի���

					if (m_pBankWndEx)
					{
						m_pBankWndEx->UpdateUserMoneyByTransfer(0,pTransfer->i64Money);
					}

					if (m_pBankWnd)
					{
						m_pBankWnd->UpdateUserMoneyByTransfer(0,pTransfer->i64Money);
					}

					m_PlaceUserInfo.i64Bank -= pTransfer->i64Money;
					UpdateMeInfo(0, false/*true*/, 0, false);         // PengJiLin, 2010-10-9, ��������ʾΪ0
				}

				return true;
			}

			switch(pNetHead->bHandleCode)
			{
			case HC_GP_BANK_TRANSFER_PASSWORD_ERROR://ת���������
				{
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					str = fMsg.GetKeyVal("GameRoom","TransferPassWdError","�����������û��ͨ����֤���������������룡");
					strBuffer.Format(str, szNum);
					DUIOkeyMsgBox(m_hWnd,false,strBuffer);
					return true;
				}
			case HC_GP_BANK_TRANSFER_TOTAL_LESS://�����ܶ�̫С�������ʸ�
				{
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					GetNumString(szNum, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
					str = fMsg.GetKeyVal("GameRoom","TransferTotalLess","����ʱ��������ת�ʣ����д��ﵽ�򳬹�%s��Ҳſ���ת�ʣ�");
					strBuffer.Format(str, szNum);
					DUIOkeyMsgBox(m_hWnd,true,strBuffer);
					return true;
				}
				break;
			case HC_GP_BANK_TRANSFER_TOO_LESS://����ת����Ŀ̫��
				{
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					GetNumString(szNum, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
					str = fMsg.GetKeyVal("GameRoom","TransferTooLess","ת�ʲ��ɹ�������ת�ʽ��������%s��ҡ�");
					strBuffer.Format(str, szNum);
					DUIOkeyMsgBox(m_hWnd,true,strBuffer);
					return true;
				}
				break;
			case HC_GP_BANK_TRANSFER_MULTIPLE://����ת����Ŀ������ĳ���ı���
				{
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					GetNumString(szNum, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
					str = fMsg.GetKeyVal("GameRoom","TransferMultiple","ת�ʲ��ɹ���ת�ʽ�������%s����������");
					strBuffer.Format(str, szNum);
					DUIOkeyMsgBox(m_hWnd,true,strBuffer);
					return true;
				}
			case HC_GP_BANK_TRANSFER_NOT_ENOUGH://���н�������ת��
				{
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					GetNumString(szNum, pTransfer->i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
					GetNumString(szBank, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
					str = fMsg.GetKeyVal("GameRoom","TransferNotEnoughMoney","ת�ʲ��ɹ�����������ֻ��%s��ң�����֧������ת��%s��ҡ�");
					strBuffer.Format(str, szBank, szNum);
					DUIOkeyMsgBox(m_hWnd,true,strBuffer);
					return true;
				}
			case HC_GP_BANK_TRANSFER_TOO_MANY_TIME:
				{
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					str = fMsg.GetKeyVal("GameRoom","TransferTooMany","������ת�˵�����ﵽ������޶�! ��������СһЩ����ֵ����ת�����ԡ�"); // PengJiLin, 2010-8-25
					strBuffer.Format(str);
					DUIOkeyMsgBox(m_hWnd,true,strBuffer);
					return true;
				}
			case HC_GP_BANK_TRANSFER_NO_DEST:											///< ת��Ŀ�겻����
				{
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					str = fMsg.GetKeyVal("GameRoom","AccountNotExist","ת��Ŀ�겻����,���ʵ�ʺ�����!");
					strBuffer.Format(str);
					DUIOkeyMsgBox(m_hWnd,false,strBuffer);
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
					DUIOkeyMsgBox(m_hWnd,false,strBuffer);
					return true;
				}
			}
			break;
		}
	case ASS_GP_BANK_FINANCE_INFO:
		{
			//Ч������
			if (uDataSize != sizeof(TMSG_GP_BankFinanceInfo))
			{
				return false;
			}

			TMSG_GP_BankFinanceInfo* pFinanceInfo = (TMSG_GP_BankFinanceInfo*)pNetData;

			if (m_pGameListWnd)
			{
				if (m_pGameListWnd->m_pFinanceWnd)
				{
					if(pNetHead->bHandleCode == HC_GP_BANK_FINANCE_INFO_SUC)
					{
						m_pGameListWnd->m_pFinanceWnd->ShowFinanceInfo(pFinanceInfo);
					}
					else if(pNetHead->bHandleCode == HC_GP_BANK_FINANCE_INFO_ERR)
					{
						m_pGameListWnd->m_pFinanceWnd->ShowFinanceInfo(NULL);
					}
				}
			}
			

		}
		break;
// 	case ASS_GR_SENDMONEY_TIMES: 
// 		{
// 			SendMoneyOnTimesAndCounts*  pSendResult = (SendMoneyOnTimesAndCounts*)pNetData;
// 
// 			UserItemStruct* pUserInfo = NULL;
// 			BOOL bUserIsMe = FALSE;
// 			if(pSendResult->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID)
// 			{
// 				pUserInfo = m_GameInfo.pMeUserInfo;
// 				bUserIsMe = TRUE;
// 			}
// 			else
// 			{
// 				pUserInfo = m_UserManage.FindOnLineUser(pSendResult->dwUserID);
// 			}
// 
// 			if(NULL != pUserInfo)       // �û��ڷ�����
// 			{
// 				pUserInfo->GameUserInfo.i64Money += pSendResult->dwGetMoney;
// 				if(TRUE == bUserIsMe)
// 				{
// 					CString strMessage;
// 					TCHAR str[100];
// 
// 					if(pSendResult->dwGetMoney > 0)
// 					{
// 						GetNumString(szNum, pSendResult->dwGetMoney, Glb().m_nPowerOfGold,
// 							Glb().m_bUseSpace, Glb().m_strSpaceChar);
// 
// 						strMessage = fMsg.GetKeyVal("GameRoom","NoGiftCoins","/:54�𾴵�[%s]����Ϸ�л��ϵͳ����%s���/:54");
// 						wsprintf(str,strMessage,m_GameInfo.pMeUserInfo->GameUserInfo.nickName,szNum);
// 						m_pRightWnd->m_MessageHandle.InsertSystemMessage(str);
// 						BZSoundPlay(this, "music/���ȷ����ť������Ϣ��ʾ��.mp3", 0);
// 					}
// 
// 					if (m_IGameFrame != NULL) 
// 					{
// 						// ���¶���������ֶ�
// 
// 						CM_UserState_Send_Times_Money ControlMessage;
// 						memset(&ControlMessage,0,sizeof(ControlMessage));
// 
// 						ControlMessage.bActionCode = ACT_USER_SEND_TIMES_MONEY;
// 
// 						ControlMessage.dwUserID = pSendResult->dwUserID;
// 						ControlMessage.dwGetMoney = pSendResult->dwGetMoney;
// 						ControlMessage.dwMoneyOnTimes = pSendResult->dwMoneyOnTimes;
// 						ControlMessage.dwMoneyOnCounts = pSendResult->dwMoneyOnCounts;
// 						ControlMessage.dwTimesNeed = pSendResult->dwTimesNeed;
// 						ControlMessage.dwCountsNeed = pSendResult->dwCountsNeed;
// 
// 						if (m_IGameFrame!=NULL)
// 							m_IGameFrame->OnControlMessage(CM_USER_SEND_TIMES_MONEY,&ControlMessage,sizeof(ControlMessage));
// 
// 					}
// 				}
// 			}
// 
// 			break;
// 
// 			// PengJiLin, 2011-4-15, ����Ϊ�ɵ��ͽ�ҹ��ܣ���ʹ�á�
// 
// 			SendUserMoneyByTimes* pSendMoneyResult = (SendUserMoneyByTimes*)pNetData;
// 
// 			UserItemStruct* pGameUserInfo = NULL;
// 			bool bIsMe = false;
// 			if (pSendMoneyResult->dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID) //�����Ľ���
// 			{
// 				pGameUserInfo = m_GameInfo.pMeUserInfo;
// 				bIsMe = true;
// 			}
// 			else
// 			{
// 				pGameUserInfo = m_UserManage.FindOnLineUser(pSendMoneyResult->dwUserID);
// 			}
// 
// 			if (pGameUserInfo != NULL) //�û��ڷ�����
// 			{
// 				pGameUserInfo->GameUserInfo.i64Money += pSendMoneyResult->dwSendGameMoney;
// 				if(bIsMe)
// 				{
// 					CString strMessage;
// 					TCHAR str[100];
// 
// 					int iTimes;
// 
// 					iTimes = pSendMoneyResult->dwAllSendTImes - pSendMoneyResult->dwSendTimes;
// 					GetNumString(szNum, pSendMoneyResult->dwSendGameMoney, Glb().m_nPowerOfGold/*, false*/,Glb().m_bUseSpace, Glb().m_strSpaceChar);
// 
// 					if(iTimes<=0)
// 					{
// 						strMessage = fMsg.GetKeyVal("GameRoom","NoGiftCoins","/:54�𾴵�[%s]����Ϸ�л���%s��ң��ѻ���%d�Σ����޻�������/:54");
// 						wsprintf(str,strMessage,
// 							m_GameInfo.pMeUserInfo->GameUserInfo.nickName,szNum,
// 							pSendMoneyResult->dwSendTimes);
// 					}
// 					else
// 					{
// 						strMessage = fMsg.GetKeyVal("GameRoom","GiftCoins","/:54�𾴵�[%s]����Ϸ�л���%s��ң��ѻ���%d�Σ�����%d�λ�������/:54");
// 						wsprintf(str,strMessage,
// 							m_GameInfo.pMeUserInfo->GameUserInfo.nickName,szNum,
// 							pSendMoneyResult->dwSendTimes,iTimes);
// 					}
// 					BZSoundPlay(this, "music/���ȷ����ť������Ϣ��ʾ��.mp3", 0);
// 					m_pRightWnd->m_MessageHandle.InsertSystemMessage(str);//send gameroom;
// 					if (m_IGameFrame != NULL) 
// 					{
// 						CM_UserState_Send_Times_Money ControlMessage;
// 						memset(&ControlMessage,0,sizeof(ControlMessage));
// 
// 						ControlMessage.bActionCode = ACT_USER_SEND_TIMES_MONEY;
// 
// 						//ControlMessage.iLastTimes = iTimes;
// 						//ControlMessage.dwUserID = pSendMoneyResult->dwUserID;
// 						//ControlMessage.iSendTimes = pSendMoneyResult->dwSendTimes;
// 						//ControlMessage.iSendMoney = pSendMoneyResult->dwSendGameMoney;
// 
// 						if (m_IGameFrame!=NULL)
// 							m_IGameFrame->OnControlMessage(CM_USER_SEND_TIMES_MONEY,&ControlMessage,sizeof(ControlMessage));
// 
// 					}
// 				}
// 			}
// 		}
// 		break;
	}

	//ĳЩ��Ϣֱ�ӽ������д��ڶ����ڲ�����
	if (m_pBankWndEx)
	{
		m_pBankWndEx->OnNetMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		return true;
	}

	if (m_pBankWnd)
	{
		m_pBankWnd->OnNetMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		return true;
	}

	//��Ϸ���������
	for (BYTE i = 0; i < MAX_GAME_ROOM; ++i)
	{
		if (m_RoomInfo[i].bAccess)
		{
			// ��Ϸ����Ϸ�����ұ���Ϣ��
			CGameRoomEx* pGameRoomEx = (CGameRoomEx*)m_RoomInfo[i].pGameRoomWnd;
			if (NULL != pGameRoomEx)
			{
				if (pGameRoomEx->m_pBankWndEx)
				{
					pGameRoomEx->m_pBankWndEx->OnNetMessage(pNetHead,pNetData,uDataSize,pClientSocket);
					return true;
				}

				if (pGameRoomEx->m_pBankWnd)
				{
					pGameRoomEx->m_pBankWnd->OnNetMessage(pNetHead,pNetData,uDataSize,pClientSocket);
					return true;
				}
			}					
		}//bAccess
	}

	return true;
}

//��ȡ��������غ��� zxj 2009-11-12 ��������
CString CMainRoomEx::innerGetMac()
{
	CString retMac="";
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter=NULL;
	DWORD dwRetVal=0;
	pAdapterInfo=(IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	ULONG ulOutBufLen=sizeof(IP_ADAPTER_INFO);
	if(GetAdaptersInfo(pAdapterInfo,&ulOutBufLen)!=ERROR_SUCCESS)
	{
		free(pAdapterInfo);
		pAdapterInfo=(IP_ADAPTER_INFO*)malloc(ulOutBufLen);
	}
	if((dwRetVal=GetAdaptersInfo(pAdapterInfo,&ulOutBufLen))==NO_ERROR)
	{
		pAdapter=pAdapterInfo;
		CString temp;
		while(pAdapter)
		{
			if(pAdapter->Type==6)//pAdapter->Description�а���"PCI"Ϊ����������//pAdapter->Type��71Ϊ����������
			{
				for(UINT i=0;i<pAdapter->AddressLength;i++)
				{
					temp.Format("%02X",pAdapter->Address[i]);
					retMac+=temp;
					//	printf("%02X%c",pAdapter->Address[i],i==pAdapter->AddressLength-1?��n��:��-��);
				}
				break;
			}
			pAdapter=pAdapter->Next;
		}
	}
	if(pAdapterInfo)
		free(pAdapterInfo);
	return retMac;
}

//��ȡ��������غ��� zxj 2009-11-12 ��������
CString CMainRoomEx::coreGetCode()
{
	CString strRet="";
	CString str=CMainRoomEx::innerGetMac(),s;
	if(str=="")
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString strMessage;
		strMessage = fMsg.GetKeyVal("MainRoom","GetEthernetError","δ��⵽ϵͳ����Ethernet�豸��");
		return strMessage;
	}

	int r=0;
	int l=str.GetLength();
	for(int i=0;i<l;i+=2)
	{
		int r1=0,r2=0;
		s=str.Mid(i,1);
		if(s>="0" && s<="9")
			r1=atoi(s);
		if(s=="A" || s=="a")	r1=10;
		if(s=="B" || s=="b")	r1=11;
		if(s=="C" || s=="c")	r1=12;
		if(s=="D" || s=="d")	r1=13;
		if(s=="E" || s=="e")	r1=14;
		if(s=="F" || s=="f")	r1=15;
		s=str.Mid(i+1,1);
		if(s>="0" && s<="9")
			r2=atoi(s);
		if(s=="A" || s=="a")	r2=10;
		if(s=="B" || s=="b")	r2=11;
		if(s=="C" || s=="c")	r2=12;
		if(s=="D" || s=="d")	r2=13;
		if(s=="E" || s=="e")	r2=14;
		if(s=="F" || s=="f")	r2=15;

		CString t;
		r+=r1*16+r2;
		srand(r);
		t.Format("%s%04X%s-",str.Mid(l-i-1,1),rand(),str.Mid(l-i-2,1));
		strRet+=t;
	}
	if(strRet.Right(1)=="-")strRet=strRet.Left(strRet.GetLength()-1);
	return strRet;
}

void CMainRoomEx::OnSwitchLockButton()	//�л�����������ť״̬
{
// 	CString strCode = coreGetCode();
// 	MSG_GP_S_LockAccount lockMsg;
// 	::ZeroMemory(&lockMsg, sizeof(lockMsg));
// 	lockMsg.dwUserID = m_PlaceUserInfo.dwUserID;
// 	lstrcpy(lockMsg.szMathineCode, strCode);
// 	CLockAccountDlg LockDlg(this, m_bLockMathine);
// 	if( LockDlg.DoModal() == IDCANCEL )
// 		return;
// 	lstrcpy(lockMsg.szMD5Pass, CLockAccountDlg::m_szMD5Pass);
// 
// 	//���������ǰ��������״̬����ô�������Ķ������ǽ������
// 	if( m_bCanClick)
// 	{
// 		m_bCanClick = FALSE;	//����ź���ʱ�رա�����ʱ����ʱ�����˷�����һ������Ľ���󣬴��ź�������Ч��
// 		if( m_bLockMathine )
// 		{
// 			lockMsg.dwCommanType = 0;	//����������
// 		}
// 		else//���������ǰ����δ����״̬����ô�������Ķ���������������
// 		{
// 			lockMsg.dwCommanType = 1;	//��������
// 		}
// 		SendData(&lockMsg, sizeof(lockMsg), MDM_GP_LOCK_ACCOUNT,ASS_LOCK_ACCOUNT, 0);	//����������������
// 		m_TopWnd->SetLockWndState(FALSE);	//��ʱ��������ťʧЧ
// 		SetTimer(IDT_CHANGE_LOCK, CHANGE_LOCK_TIME, NULL);
// 	}

	CUserInfoWnd** pUserInfoWnd = &m_pGameListWnd->m_pUserInfoWnd;
	if (*pUserInfoWnd)
	{
		(*pUserInfoWnd)->Close();
// 		delete *pUserInfoWnd;
// 		*pUserInfoWnd = NULL;
	}

	*pUserInfoWnd = new CUserInfoWnd();

	if(!*pUserInfoWnd)
	{
		return;
	}
	(*pUserInfoWnd)->m_nDefaultPage		= 2;
	(*pUserInfoWnd)->m_nDefaultSubPage	= 1;
	CMainRoomEx::m_bLockQuit = true;

	(*pUserInfoWnd)->SetPlaceUserInfo(m_pGameListWnd->m_PlaceUserInfo);
	(*pUserInfoWnd)->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
	(*pUserInfoWnd)->CenterWindow();
	(*pUserInfoWnd)->ShowModal();

	CMainRoomEx::m_bLockQuit = false;

	delete (*pUserInfoWnd);
	(*pUserInfoWnd) = NULL;

}

//�����ֻ���֤����� JianGuankun 2012.5.16
void CMainRoomEx::OnSendMobileVCodeSms(char* szMobileNo,UINT uType)
{
	::memset(m_szMobileVCode,NULL,sizeof(m_szMobileVCode));

	MSG_GP_SmsVCode tagSmsVCode;
	::memset(&tagSmsVCode,0,sizeof(MSG_GP_SmsVCode));
	::strcpy(tagSmsVCode.szName,CLogonDialog::m_szName);
	::strcpy(tagSmsVCode.szMobileNo,szMobileNo);
	tagSmsVCode.nType = uType;

	SendData(&tagSmsVCode,sizeof(MSG_GP_SmsVCode),MDM_GP_SMS,ASS_GP_SMS_VCODE,0);
}

//��֤�û�������ֻ���֤���Ƿ���ȷ JianGuankun 2012.5.16
bool CMainRoomEx::ValidMobileVCode(char* szVCode)
{
#ifdef MY_MOBILE_VCODE_OFF
	return (::strcmp(szVCode,_T("000000")) == 0);
#endif

	MD5_CTX Md5;
	char szMD5Input[36] = {0};
	Md5.MD5Update((unsigned char*)szVCode,lstrlen(szVCode));
	unsigned char szMDTemp[16];
	Md5.MD5Final(szMDTemp);
	for (int i=0;i<16;i++) 
	{
		wsprintf(&szMD5Input[i*2],"%02x",szMDTemp[i]);
	}

	if (strcmp((const char*)szMD5Input,m_szMobileVCode) == 0)
	{
		return true;
	}

	return false;
}

//�����ֻ���֤�룬ʹ��֤����� JianGuankun 2012.5.16
void CMainRoomEx::ClearMobileVCode()
{
	::memset(m_szMobileVCode,NULL,sizeof(m_szMobileVCode));
}

// PengJiLin, 2011-7-21, CPU��Ӳ�� ID
extern CString g_strCPUID;
extern CString g_strHardID;

//���͵�½��Ϣ///////////*****************************************//////////////////////////
bool CMainRoomEx::SendUserInfo()
{	//������Ϣ��ʾ
	if (m_Connect.GetSafeHwnd()==NULL) m_Connect.Create(IDD_CONNECT,this);

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString strMessage;
	strMessage = fMsg.GetKeyVal("MainRoom","CheckingUserInfor","������֤�û���Ϣ...");
	m_Connect.SetMessageText(strMessage);
	m_Connect.ShowWindow(SW_SHOW);
	//int RanCheckNum = 0;
	//char out[129]={0};
	//char keytemp[5]={0};
	//memset(out,0x30,18);
	//srand((unsigned)timeGetTime());//�������������
	//for(int i=0;i<32;i++){ 
	//RanCheckNum = rand();//���������
	//_itoa(RanCheckNum,keytemp,16);//�������ת����16����
	//memcpy(&out[i*4],keytemp,strlen(keytemp));
	//}
	//out[128]=0x00;
	//TML_//SN=out;
	int rNum=0;
	CString TML_SN = "";
	CString MatchineCode= coreGetCode();	//ȡ�ñ��������� zxj 2009-11-12 ��������

	CString allword="0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	for (int i=0;i<=120;i++)
	{
		// rNum = max(1, rand() * 1000 % 62);
		rNum =(rand() * 1000)% 62;
		//   rNum=1+(int)((rand()/(double)RAND_MAX)*36); //�����ֵ
		//	RanCheckNum = rand();//���������
		//	AfxMessageBox(RanCheckNum);
		TML_SN+=allword[rNum];
	}

	Glb().m_TML_SN=TML_SN;
	//�����û���¼��Ϣ
	if (CLogonDialog::m_bRegUser==FALSE)
	{
		//�����û���¼
		switch (CLogonDialog::m_bLogonType)
		{
		case LK_BY_NAME:
			{
				MSG_GP_S_LogonByNameStruct LogonByName;///����һ���ṹ
				memset(&LogonByName, 0, sizeof(LogonByName));
				LogonByName.iUserID = -1;

				LogonByName.uRoomVer=GAME_PLACE_MAX_VER;//�ṹ�İ汾
				lstrcpy(LogonByName.TML_SN,TML_SN);
				lstrcpy(LogonByName.szName,CLogonDialog::m_szName);//�ṹ���û���
				if(Glb().m_autoreg)
					lstrcpy(LogonByName.szMD5Pass,"");//�ṹ������
				else
					lstrcpy(LogonByName.szMD5Pass,CLogonDialog::m_szMD5Pass);//�ṹ������
				LogonByName.gsqPs=5471;
				lstrcpy(LogonByName.szMathineCode, MatchineCode);	//�����������ѱ��������봫�ݵ���¼���ݰ��� zxj 2009-11-12
				
                // PengJiLin, 2011-7-21, CPU��Ӳ�� ID
                lstrcpy(LogonByName.szCPUID, g_strCPUID);
                lstrcpy(LogonByName.szHardID, g_strHardID);

				//JianGuankun, 2011-11-10��֤��ID
				lstrcpy(LogonByName.szIDcardNo,CLogonDialog::m_szIDCard);
				lstrcpy(LogonByName.szMobileVCode,CLogonDialog::m_szMobileVCode);

				LogonByName.iUserID  = -1;

				//LogonByName.iUserID = atoi(CLogonDialog::m_szName);

				//���û�������֤ģ�飬ֱ����ƭ������
				if (CGamePlaceApp::GetModuleStatus("UserSecurity","IDCardVerify",0) == 0)
				{
					strcpy(LogonByName.szIDcardNo,"*");
				}

				//���û���ֻ���֤ģ�飬ֱ����ƭ������
				if (CGamePlaceApp::GetModuleStatus("UserSecurity","MobileVerify",0) == 0)
				{
					strcpy(LogonByName.szMobileVCode,"*");
				}
                
				SendData(&LogonByName,sizeof(LogonByName),MDM_GP_LOGON,ASS_GP_LOGON_BY_NAME,0);
	
				Glb().m_URL=TEXT(Glb().m_CenterServerPara.m_strWebRootADDR);
				Glb().m_URL+=Glb().m_key+":"+TML_SN+"$";

				return true;
			}
		case LK_BY_ID:
			{
				MSG_GP_S_LogonByNameStruct LogonByName;///����һ���ṹ
				memset(&LogonByName, 0, sizeof(LogonByName));
				LogonByName.iUserID = -1;

				LogonByName.uRoomVer=GAME_PLACE_MAX_VER;//�ṹ�İ汾
				lstrcpy(LogonByName.TML_SN,TML_SN);
				lstrcpy(LogonByName.szName,CLogonDialog::m_szName);//�ṹ���û���
				if(Glb().m_autoreg)
					lstrcpy(LogonByName.szMD5Pass,"");//�ṹ������
				else
					lstrcpy(LogonByName.szMD5Pass,CLogonDialog::m_szMD5Pass);//�ṹ������
				LogonByName.gsqPs=5471;
				lstrcpy(LogonByName.szMathineCode, MatchineCode);	//�����������ѱ��������봫�ݵ���¼���ݰ��� zxj 2009-11-12

				// PengJiLin, 2011-7-21, CPU��Ӳ�� ID
				lstrcpy(LogonByName.szCPUID, g_strCPUID);
				lstrcpy(LogonByName.szHardID, g_strHardID);

				//JianGuankun, 2011-11-10��֤��ID
				lstrcpy(LogonByName.szIDcardNo,CLogonDialog::m_szIDCard);
				lstrcpy(LogonByName.szMobileVCode,CLogonDialog::m_szMobileVCode);

				LogonByName.iUserID = atoi(CLogonDialog::m_szName);

				//���û�������֤ģ�飬ֱ����ƭ������
				if (CGamePlaceApp::GetModuleStatus("UserSecurity","IDCardVerify",0) == 0)
				{
					strcpy(LogonByName.szIDcardNo,"*");
				}

				//���û���ֻ���֤ģ�飬ֱ����ƭ������
				if (CGamePlaceApp::GetModuleStatus("UserSecurity","MobileVerify",0) == 0)
				{
					strcpy(LogonByName.szMobileVCode,"*");
				}

				SendData(&LogonByName,sizeof(LogonByName),MDM_GP_LOGON,ASS_GP_LOGON_BY_NAME,0);

				Glb().m_URL=TEXT(Glb().m_CenterServerPara.m_strWebRootADDR);
				Glb().m_URL+=Glb().m_key+":"+TML_SN+"$";

				return true;
			}
			/*		case LK_BY_ACC:
			{
			MSG_GP_S_LogonByAccStruct LogonByAcc;
			LogonByAcc.uRoomVer=GAME_PLACE_MAX_VER;
			LogonByAcc.dwAccID=CLogonDialog::m_dwAccID;
			lstrcpy(LogonByAcc.szMD5Pass,CLogonDialog::m_szMD5Pass);
			SendData(&LogonByAcc,sizeof(LogonByAcc),MDM_GP_LOGON,ASS_GP_LOGON_BY_ACC,0);
			return true;
			}
			case LK_BY_MOBILE:
			{
			MSG_GP_S_LogonByMobileStruct LogonByMoblile;
			LogonByMoblile.uRoomVer=GAME_PLACE_MAX_VER;
			LogonByMoblile.dwMobile=CLogonDialog::m_dwMobile;
			lstrcpy(LogonByMoblile.szMD5Pass,CLogonDialog::m_szMD5Pass);
			SendData(&LogonByMoblile,sizeof(LogonByMoblile),MDM_GP_LOGON,ASS_GP_LOGON_BY_MOBILE,0);
			return true;
			}
			*/		}
	}
	else 
	{
		//���û�ע��
		MSG_GP_S_RegisterStruct UserRegister;
		UserRegister.uRoomVer=GAME_PLACE_MAX_VER;
		UserRegister.bBoy=CLogonDialog::m_bBoy;
		UserRegister.bLogoID=CLogonDialog::m_bLogoID;
		//UserRegister.szRegSQL="inser";
		lstrcpy(UserRegister.szRegSQL,CLogonDialog::m_szRegSQL);
		lstrcpy(UserRegister.szName,CLogonDialog::m_szName);
		lstrcpy(UserRegister.szMD5Pass,CLogonDialog::m_szMD5Pass);
		lstrcpy(UserRegister.nickName,CLogonDialog::m_nickname);
		lstrcpy(UserRegister.szRecommenderName, CLogonDialog::m_szRecommenderName);

		SendData(&UserRegister,sizeof(UserRegister),MDM_GP_LOGON,ASS_GP_LOGON_REG,0);

		Glb().m_URL=TEXT(Glb().m_CenterServerPara.m_strWebRootADDR);
		Glb().m_URL+=Glb().m_key+":"+UserRegister.szName+UserRegister.szMD5Pass+"$";
		CLogonDialog::m_bRegUser=false;
	}
	return true;
}
//��ȡ��Ϸ�б�
bool CMainRoomEx::GetGameRoomList()
{
	//������Ϣ��ʾ
	if (m_Connect.GetSafeHwnd()!=NULL) 
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		m_Connect.SetMessageText(fMsg.GetKeyVal("MainRoom","GetGameInfor","���ڻ�ȡ��Ϸ�����б���Ϣ..."));
		m_Connect.ShowWindow(SW_SHOW);
	}
	//�������ݰ�
	//if (Glb().m_CS==1)//1��ʹ��CS�ṹ
	//{
	SendData(MDM_GP_LIST,ASS_GP_LIST_KIND,0);//������Ϸ�б�
	//}
	//else 
	//{//;0�ǽ�ֹCS�ṹ��
	//	m_Connect.DestroyWindow();
	//}
	return true;
}


//��ȡ�����¼����Ϸ
bool CMainRoomEx::GetLastLoginGame()
{
	//������Ϣ��ʾ
	if (m_Connect.GetSafeHwnd()!=NULL) 
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		m_Connect.SetMessageText(fMsg.GetKeyVal("MainRoom","GetGameInfor","���ڻ�ȡ��Ϸ�����б���Ϣ..."));
		m_Connect.ShowWindow(SW_SHOW);
	}

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	bool bShow = fMsg.GetKeyVal("MainRoom","ShowLastLoginGame", 0);
	if (bShow)
	{
		m_pGameListWnd->m_GameList.ClearLoginGameName();
		SendData(MDM_GP_LASTLOINGGAME, ASS_GP_LASTLOINGGAME, 0);//������Ϸ�б�
	}
	
	return true;
}

///��ȡ�һ��б�
bool CMainRoomEx::GetCharmExchangeList()
{
	if (CharmPannel != NULL) CharmPannel->Clear();
	SendData(MDM_GP_CHARMEXCHANGE, ASS_GETLIST, 0);//������Ϸ�б�
	return true;
}


//��ʱ����Ϣ
void CMainRoomEx::OnTimer(UINT nIDEvent)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;
	

	if (nIDEvent == TIME_KEEP_ALIVE)
	{
		if(m_nKeepAliveCount>=5)//20090928 ZLP �ͻ��˶��������޸�
		{
			//KillTimer(TIME_KEEP_ALIVE);
			OnSocketCloseEvent();
			//GetMainRoom()->PostMessage(WM_COMMAND,MAKELONG(IDC_BTN_MAIN5,BN_CLICKED));
		}
		else
		{
			m_nKeepAliveCount++;
			SendData(MDM_CONNECT,ASS_NET_TEST,0);//������Ϣ����
		}
	}
#ifdef MY_QUICK_GAME_ON

	if (nIDEvent == IDT_QUICK_ENTER_ROOM)
	{
		KillTimer(nIDEvent);
		CAFCRoomItem* pRoomItem = m_pGameListWnd->m_GameList.FindRoomItem(m_uQGRoomID);	

		if (pRoomItem)
		{
			OnCentranceGameRoom(pRoomItem);
			SetTimer(IDT_QUICK_GAME,500,NULL);
		}
		
	}

	if (nIDEvent == IDT_QUICK_GAME)
	{
		KillTimer(nIDEvent);

		AfxGetMainWnd()->ShowWindow(SW_NORMAL);

		CGameRoomEx* pGameRoom = (CGameRoomEx*)m_RoomInfo[0].pGameRoomWnd;
		if (pGameRoom)
		{
			pGameRoom->OnFindNullDesk();
		}
	}

#endif // MY_QUICK_GAME_ON
	if (nIDEvent==ID_CHECK_SEND)
	{
		KillTimer(nIDEvent);
		///CloseSocket();
	}
	else if(nIDEvent==ID_CHECK_SIGNAL)
    {
        checkNetSignal();
    }
	else if(nIDEvent==CHECK_UPDATE)
	{
		//2012.08.22 yyf ����  
		//try
		//{
		//	bool result;
		//	::EnterCriticalSection(&m_Section);
		//	result=m_UpdateList.size() == 0;
		//	::LeaveCriticalSection(&m_Section);
		//	if(result)
		//	{			
		//		KillTimer(nIDEvent);
		//		return ;
		//	}	
		//	::EnterCriticalSection(&m_Section);
		//	::LeaveCriticalSection(&m_Section);
		//	for(vector<LPUPDATE>::iterator pos = m_UpdateList.begin(); pos != m_UpdateList.end();)
		//	{
		//		::EnterCriticalSection(&m_Section);
		//		UPDATE* lp = *pos;			
		//		if(!lp)
		//		{
		//			::LeaveCriticalSection(&m_Section);
		//			continue;
		//		}
		//		DWORD result=::WaitForSingleObject(lp->hProcess,0);
		//		if(result!=WAIT_TIMEOUT)
		//		{
		//			DWORD exitcode;
		//			::GetExitCodeProcess(lp->hProcess,&exitcode);
		//			::CloseHandle(lp->hProcess);
		//			CAFCRoomItem *RoomItem=lp->pGameRoomItem;
		//			m_pGameListWnd->m_GameList.UpdataItem(RoomItem->m_pParentItem);

		//			lp->pGameRoomItem=NULL;
		//			delete lp;
		//			pos = m_UpdateList.erase(pos);
		//			lp = NULL;

		//			::LeaveCriticalSection(&m_Section);
		//			if(result==WAIT_OBJECT_0)
		//			{
		//				///////////////////////////////
		//				////Kylin 20090117 �رո�����ɺ��Զ���½����
		//				if(exitcode!=-1)
		//				{
		//					if (m_UpdateList.size() == 0)
		//					{
		//						CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		//						CString str;
		//						CString strTitle;
		//						str = fMsg.GetKeyVal("MainRoom","DownLoadOver","�������");
		//						strTitle = fMsg.GetKeyVal("MainRoom","MessageTitle","��ʾ");
		//						AFCMessageBox(str,strTitle);
		//					}
		//				}
		//					//OnCentranceGameRoom(RoomItem);
		//				///////////////////////////////
		//			}
		//		}
		//		else
		//		{
		//			pos++;
		//			::LeaveCriticalSection(&m_Section);
		//		}
		//	}
		//	::EnterCriticalSection(&m_Section);
		//	if(m_UpdateList.size() == 0)
		//		KillTimer(nIDEvent);
		//	::LeaveCriticalSection(&m_Section);
		//}
		//catch(...)
		//{
		//	::LeaveCriticalSection(&m_Section);
		//}

		//--------------------------------------------------
		//�жϸ��½��̹ر�û�У��ر��ˣ��Ѹ����б� m_UpdateList ���һ��.
		::EnterCriticalSection(&m_Section); 
		//DWORD result=::WaitForSingleObject(m_AnonymousPipe.GetProcessInfo().hProcess,0);
		DWORD result=::WaitForSingleObject(m_CSocket32.GetProcessInfo().hProcess,0);
		if(result!=WAIT_TIMEOUT)
		{
			DWORD exitcode;
			//::GetExitCodeProcess(m_AnonymousPipe.GetProcessInfo().hProcess,&exitcode);
			//::CloseHandle(m_AnonymousPipe.GetProcessInfo().hProcess);
			//::CloseHandle(m_AnonymousPipe.GetProcessInfo().hThread);
			::GetExitCodeProcess(m_CSocket32.GetProcessInfo().hProcess,&exitcode);

			//if(m_CSocket32.GetProcessInfo().hProcess)
			//{
			//::CloseHandle(m_CSocket32.GetProcessInfo().hProcess);
			//}
			//if(m_CSocket32.GetProcessInfo().hThread)
			//{
			//::CloseHandle(m_CSocket32.GetProcessInfo().hThread);
			//}
			m_CSocket32.Close();

			for (vector<LPUPDATE>::iterator it = m_UpdateList.begin(); it != m_UpdateList.end(); it++)
			{
				UPDATE* up = *it;
				delete up;
				up = NULL;
			}
			m_UpdateList.clear();

			//if(result==WAIT_OBJECT_0)
			//{
			//	///////////////////////////////
			//	////Kylin 20090117 �رո�����ɺ��Զ���½����
			//	if(exitcode!=-1)
			//	{
			//		if (m_UpdateList.size() == 0)
			//		{
			//			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
			//			CString str;
			//			CString strTitle;
			//			str = fMsg.GetKeyVal("MainRoom","DownLoadOver","�������");
			//			strTitle = fMsg.GetKeyVal("MainRoom","MessageTitle","��ʾ");
			//			AFCMessageBox(str,strTitle);
			//		}
			//	} 
			//}
		} 
		::LeaveCriticalSection(&m_Section);
	}
#ifdef AI_MACHINE
	else if(ID_AI_ENTERROOM == nIDEvent)
	{
		if(m_Airoom < 0)
			return ;
		if(EnterRoom(m_Airoom))
		{
			KillTimer(ID_AI_ENTERROOM);
			m_Airoom = -1;
		}
	}
#endif
	//������4������¼����OK��Ĳ�����ʱ100ms����
	if(nIDEvent==1)
	{	//������ʾ
		KillTimer(1);
		if (m_Connect.GetSafeHwnd()==NULL) 
			m_Connect.Create(IDD_CONNECT,this);
		str = fMsg.GetKeyVal("MainRoom","ConnectingLoginServer","�������ӵ�¼�����������Ժ�...");
		m_Connect.SetMessageText(str);
		//m_Connect.CenterWindow();
		m_Connect.ShowWindow(SW_SHOW);


		if (m_TCPSocket != NULL && m_TCPSocket->Connect(Glb().m_CenterServerPara.m_strMainserverIPAddr,Glb().m_CenterServerPara.m_iMainserverPort)==false)////GAME_LOGON_PORT///���ӷ�����ʧ��
		{	
			m_Connect.DestroyWindow();
			str = fMsg.GetKeyVal("MainRoom","ConnectingError","���ӳ��ִ�����ȷ���Ƿ�����������Э�飡");
			
			//AFCMessageBox(str);//IDS_CONNECT_ERROR,AfxGetInstanceHandle(),MESSAGE_TITLE,MB_ICONQUESTION,this);
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,str);

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

			CMainRoomEx::m_bBankLogoned = false;

			PostMessage(WM_COMMAND,IDM_LOGON_PLACE,0);
		}

		//��������
		memset(m_Mission,0,sizeof(m_Mission));
		AddMission(MS_LOGON,0,0);

	}

	//����������ش��� zxj 2009-11-13
	if( nIDEvent == IDT_CHANGE_LOCK )
	{
		KillTimer(IDT_CHANGE_LOCK);
		m_TopWnd->SetLockWndState(TRUE);	//������������ť��Ч
		m_bCanClick = TRUE;					//���ź�������Ч
	}
	CBaseRoom::OnTimer(nIDEvent);
}

//�汾���
bool CMainRoomEx::GamePlaceVerCheck(MSG_S_ConnectSuccess * pVerInfo, bool & bNewVer)
{
	//�汾��
	bNewVer=GAME_PLACE_MAX_VER<pVerInfo->bMaxVer;
	return GAME_PLACE_MAX_VER>=pVerInfo->bLessVer;
}

////�޸�����
//void CMainRoomEx::OnChangeInfo()
//{
//	ShellExecute(NULL,TEXT("open"),TEXT("http://www.EYUL.COM/?�޸�����"),NULL,NULL,SW_MAXIMIZE);	
//	return;
//}

//�˳���Ϸ
void CMainRoomEx::OnQuitPlace()
{
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
	return;
}

//������Ϸ����
void CMainRoomEx::OnInGame()
{
	if (m_bLogon==true)
	{
		HTREEITEM Item=m_pGameListWnd->m_GameList.GetSelectedItem();
		if (Item!=NULL)
		{
			CAFCGameListItem * pItem=(CAFCGameListItem *)m_pGameListWnd->m_GameList.GetItemData(Item);
			if ((pItem!=NULL)&&(pItem->m_uDataType==GLK_GAME_ROOM))
			{
				OnCentranceGameRoom((CAFCRoomItem *)pItem);
				return;
			}
		}

		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString str;
		CString strTitle;
		str = fMsg.GetKeyVal("MainRoom","SelectGameRoom","����ѡ����Ϸ����");
		strTitle = fMsg.GetKeyVal("MainRoom","MessageTitle","ϵͳ��Ϣ");
		
		//AFCMessageBox(str,strTitle);//,MB_ICONINFORMATION,this);
        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,strTitle,false,str);
	}
	return;
}
//��ҳ��ť
void CMainRoomEx::OnWeb()
{
	CString strBuffer;
	//if (strBuffer.IsEmpty()==false) IE(strBuffer);

	return;
}

//��ʾ����
void CMainRoomEx::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBaseRoom::OnShowWindow(bShow, nStatus);

	//������ʾ
//	if (bShow==TRUE) ((CGamePlaceDlg *)AfxGetMainWnd())->SetWindowTitle(m_strTitle);

	return;
}
//�ر����з���
bool CMainRoomEx::CloseAllGameRoom()
{
	for (UINT i=0;i<MAX_GAME_ROOM;i++)
	{
		if ((m_RoomInfo[i].pGameRoomWnd!=NULL)&&(m_RoomInfo[i].pGameRoomWnd->CanCloseNow()==false)) 
		{
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
			CString str;
			CString strTitle;
			str = fMsg.GetKeyVal("MainRoom","QuitGameBeforeClosing","���˳�����Ϸ���ٹر�����ϵͳ��");
			strTitle = fMsg.GetKeyVal("MainRoom","MessageTitle","ϵͳ��Ϣ");
			
			//AFCMessageBox(str,strTitle);//,MB_ICONQUESTION,this);
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,strTitle,false,str);
			return false;
		}
		else		//��ҽ�����Ϸ������δ��ʼ��Ϸ ��ر���Ϸadd by xqm 2010-11-12
		{
			if((m_RoomInfo[i].pGameRoomWnd!=NULL) && (m_RoomInfo[i].pGameRoomWnd->CloseGameRoom()));
		}
	}

#ifdef MY_QUICK_GAME_ON

	//JianGK,�رչܵ�
	CloseNamedPipe();

#endif // MY_QUICK_GAME_ON
	return true;
}

//�ػ�����
void CMainRoomEx::OnPaint()
{
	CPaintDC dc(this);
	CRect ClientRect;
	GetClientRect(&ClientRect);
	int iWidth = ClientRect.Width();
	int iHeight = ClientRect.Height();
	CRgn rgn1, rgn2;
	rgn1.CreateRectRgn(0, 0, iWidth, m_maintc.GetHeight());
	rgn2.CreateRectRgn(0, iHeight-m_mainbc.GetHeight(), iWidth, iHeight);
	rgn1.CombineRgn(&rgn1, &rgn2, RGN_OR);
	rgn2.SetRectRgn(0, 0, m_mainlc.GetWidth(), iHeight);
	rgn1.CombineRgn(&rgn1, &rgn2, RGN_OR);
	rgn2.SetRectRgn(iWidth-m_mainrc.GetWidth(), 0, iWidth, iHeight);
	rgn1.CombineRgn(&rgn1, &rgn2, RGN_OR);
	dc.SelectClipRgn(&rgn1);
	//�滭���
	DrawViewFrame(&dc,ClientRect.Width(),ClientRect.Height());
	rgn2.DeleteObject();
	rgn1.DeleteObject();

	return;
}


//�˵�����
BOOL CMainRoomEx::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_SKIN:
	case IDM_SKIN+1:
	case IDM_SKIN+2:
	case IDM_SKIN+3:
	case IDM_SKIN+4:				//�����������
		{
			int basesk=IDM_SKIN;
			int sk=LOWORD(wParam)-basesk;
			CNewGameClass * pRoomTab=&((CGamePlaceDlg *)AfxGetMainWnd())->m_RoomTabWnd;
			pRoomTab->PostMessage(WM_LOADSKIN,(WPARAM)sk,0);
		}
		return true;
	case IDM_DOWN_NEW:				//����������Ϣ
		{
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
			CString str;

			CAFCGameListItem * pGameItem;
			HTREEITEM hItem=m_pGameListWnd->m_GameList.GetSelectedItem();
			if (hItem!=NULL)
			{
				pGameItem=(CAFCGameListItem *)m_pGameListWnd->m_GameList.GetItemData(hItem);
				if ((pGameItem==NULL)||(pGameItem->m_uDataType!=GLK_GAME_ROOM)) 
					return true;//��Ϸ����,,�����·�
			}
			CAFCRoomItem * pGameRoomItem=(CAFCRoomItem *)pGameItem;
			//�ж���Ϸ�Ƿ�װ��Ч��汾
			CFileVersion FileVer;
			TCHAR szProessName[31],szGameName[61];
			m_pGameListWnd->m_GameList.GetGameName(pGameRoomItem,szGameName,sizeof(szGameName));
			m_pGameListWnd->m_GameList.GetProcessName(pGameRoomItem,szProessName,sizeof(szProessName));
			CString strMessage;
			str = fMsg.GetKeyVal("MainRoom","DownloadingDontEnter","�������� %s��Ϸ�����¹���������������Ϸ! ");
			strMessage.Format(str,szGameName);
			
			if (CheckUpdate(szProessName, pGameRoomItem))
			{
				//wushuqun 2009.5.21
				//if(IDCANCEL==AFCMessageBox(TEXT(strMessage),MESSAGE_TITLE,AFC_YESNO))//,MB_ICONQUESTION|MB_YESNO,this)!=IDYES)
				if(IDCANCEL==DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION,MESSAGE_TITLE,true,strMessage)) 
					return true;
				try
				{     
					StartDownLoad(szProessName,pGameRoomItem);
				}
				catch(CException* e)
				{
					str = fMsg.GetKeyVal("MainRoom","DownloadError","���ط������쳣��");
					
					//AFCMessageBox(str);
                    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,str);
					e->Delete();
					return false;
				}
				catch(...)
				{
					str = fMsg.GetKeyVal("MainRoom","DownloadError","���ط������쳣��");
					
					//AFCMessageBox(str);
                    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,str);
					return false;
				}
			}


			return true;
		}
	default:
		break;
	}
	return __super::OnCommand(wParam, lParam);
}

void CMainRoomEx::LoadSkin(const CString& strLogonUserID)
{
	int r,g,b;
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkinBcfFileName = m_skinmgr.GetSkinBcfFileName(strLogonUserID);
	CBcfFile f( s + strSkinBcfFileName);
	CString key=TEXT("MainRoom");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkinBcfFileName)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath(strLogonUserID));
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath(strLogonUserID));

	m_adx=f.GetKeyVal(key,"adx",15);
	m_ady=f.GetKeyVal(key,"ady",10);
	m_adheight=f.GetKeyVal(key,"adwidth",63);

	m_skx=f.GetKeyVal(key,"hskx",30);
	m_sky=f.GetKeyVal(key,"hsky",10);

	m_utitle=f.GetKeyVal(key,"halltitle","%s -- %s");
	r=f.GetKeyVal(key,"halltitler",0);
	g=f.GetKeyVal(key,"halltitleg",0);
	b=f.GetKeyVal(key,"halltitleb",0);
	m_titlecolor=RGB(r,g,b);


	//�������Ϸ������б�imagelist
	r=f.GetKeyVal(key,"listbkclr",0);
	g=f.GetKeyVal(key,"listbkclg",0);
	b=f.GetKeyVal(key,"listbkclb",0);	
	//ChangeImageList(RGB(r,g,b));


	//�Ϸ���ťλ��
	m_mainbtnX = f.GetKeyVal(key,"mainbtnx",10);
	m_mainbtnSpace = f.GetKeyVal(key,"mainbtnspace",20);


	//������
	wsprintf(path,"%smain\\main_btn01.bmp",skinfolder);
	//	m_button.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_Bt1.LoadButtonBitmap(path,false);
	wsprintf(path,"%smain\\main_btn02.bmp",skinfolder);
	m_Bt2.LoadButtonBitmap(path,false);
	wsprintf(path,"%smain\\main_btn03.bmp",skinfolder);
	m_Bt3.LoadButtonBitmap(path,false);
	wsprintf(path,"%smain\\main_btn04.bmp",skinfolder);
	m_Bt4.LoadButtonBitmap(path,false);
	//wsprintf(path,"%smain\\main_btn05.bmp",skinfolder);
	//m_Bt5.LoadButtonBitmap(path,false);
	//wsprintf(path,"%smain\\main_btn06.bmp",skinfolder);
	//m_Bt6.LoadButtonBitmap(path,false);
	//wsprintf(path,"%smain\\main_btn07.bmp",skinfolder);
	//m_Bt7.LoadButtonBitmap(path,false);
	//wsprintf(path,"%smain\\main_btn08.bmp",skinfolder);
	//m_Bt8.LoadButtonBitmap(path,false);
	//wsprintf(path,"%smain\\main_btn09.bmp",skinfolder);
	//m_Bt9.LoadButtonBitmap(path,false);
	//wsprintf(path,"%smain\\main_btn10.bmp",skinfolder);
	//m_Bt10.LoadButtonBitmap(path,false);	//wsprintf(path,"%shall\\5.bmp",skinfolder);
	//m_Bt5.LoadButtonBitmap(path,false);
	//wsprintf(path,"%shall\\6.bmp",skinfolder);
	//m_Bt6.LoadButtonBitmap(path,false);
	//wsprintf(path,"%shall\\7.bmp",skinfolder);
	//m_Bt7.LoadButtonBitmap(path,false);

	m_Bt5.ShowWindow(SW_HIDE);
	m_Bt6.ShowWindow(SW_HIDE);
	m_Bt7.ShowWindow(SW_HIDE);
	m_Bt8.ShowWindow(SW_HIDE);
	m_Bt9.ShowWindow(SW_HIDE);
	m_Bt10.ShowWindow(SW_HIDE);


	//�����
	wsprintf(path,"%smain\\main_lc.bmp",skinfolder);
	m_mainlc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%smain\\main_rc.bmp",skinfolder);
	m_mainrc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%smain\\main_tc.bmp",skinfolder);
	m_maintc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%smain\\main_bc.bmp",skinfolder);
	m_mainbc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//�Ͽ���Ľ�
	wsprintf(path,"%smain\\main_lt.bmp",skinfolder);
	m_mainlt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%smain\\main_rt.bmp",skinfolder);
	m_mainrt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%smain\\main_lb.bmp",skinfolder);
	m_mainlb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%smain\\main_rb.bmp",skinfolder);
	m_mainrb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	
	//WCHAR path1[MAX_PATH];
	wchar_t wpath[MAX_PATH];
	
	
	
	
	
#ifndef BZ_ZLIB  //< add by wxx 
	img=Image::FromFile(wpath);
#else


	TCHAR szPath[MAX_PATH];
	sprintf(szPath,m_skinmgr.GetSkinPath(strLogonUserID) + "signal.bmp");
	CUnZipRes unZipRes;
	IStream * pStream=NULL;
	pStream=unZipRes.LoadFileResFromZip(szPath);

	img=new Image(pStream);
	if(pStream!=NULL)
	{
		pStream->Release();
		pStream=NULL;
	}

#endif

    //�ȴ�����ͼ yjj 090227
}


LRESULT CMainRoomEx::OnLoadSkin(WPARAM wparam, LPARAM lparam)
{
	LoadSkin();
	m_pGameListWnd->m_GameList.UpdateAllIcon();
	CRect ClientRect;
	GetClientRect(&ClientRect);
	OnSizeEvent(0,ClientRect.Width(),ClientRect.Height());
	return true;
}
LRESULT CMainRoomEx::OnExchangeSkin(WPARAM wparam, LPARAM lparam)
{
	m_strTitle=Glb().m_title;//��ȫ�ֱ����м��ش��ڱ���
	LoadSkin();

	m_pGameListWnd->m_GameList.UpdateAllIcon();
	m_pGameListWnd->m_GameList.Invalidate();

	SetWindowText(m_strTitle);
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder = 
		f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());
	wsprintf(path,"%sDialog\\scroll.bmp",skinfolder);
	HBITMAP hbmScorll = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
	SkinSB_Uninit(m_WebView.GetSafeHwnd());
	SkinSB_Init(m_WebView.GetSafeHwnd(), hbmScorll);

	Invalidate();
	return LRESULT(0);
}

bool CMainRoomEx::GetRoomInfoStruct(RoomInfoStruct** pRoomInfoStruct)
{
	if(m_RoomInfo!=NULL) 
	{
		*pRoomInfoStruct=m_RoomInfo;
		return true;
	}
	return false;
}

bool CMainRoomEx::CheckUpdate(TCHAR filename[],CAFCRoomItem * pGameRoomItem)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	TCHAR szProessName[31],szGameName[61];
	m_pGameListWnd->m_GameList.GetGameName(pGameRoomItem,szGameName,sizeof(szGameName));
	m_pGameListWnd->m_GameList.GetProcessName(pGameRoomItem,szProessName,sizeof(szProessName));
	ComRoomInfo * pComRoomInfo=&pGameRoomItem->m_RoomInfo;

	//����update���и���
	TCHAR szApp[61];//,sz[100];
	TCHAR path[MAX_PATH];
	lstrcpy(szApp,szProessName);
	szApp[lstrlen(szApp)-lstrlen("ico")-1]='\0';

	
	wsprintf(path,"%s\\%s\\update.ini",GetAppPath().c_str(),szApp);
	//CINIFile f(path);
	CString m_key="config";
	TCHAR v[128];
	ZeroMemory(v, sizeof(v));

	//v=f.GetKeyVal(m_key,"-v","0");//�汾

	GetPrivateProfileString("config", "-v", "", v, 128, path);
//	DebugPrintf()
	TCHAR ser[64];
	ZeroMemory(ser, sizeof(ser));
	wsprintf(ser,"%u.%u",pComRoomInfo->uVer>>16,pComRoomInfo->uVer&0x0000ffff);

	if (_tcscmp(ser, v) != 0)
		return true;
	else
		return false;
}

bool CMainRoomEx::StartDownLoad(TCHAR filename[],CAFCRoomItem * pGameRoomItem)
{

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	TCHAR szProessName[31],szGameName[61];
	m_pGameListWnd->m_GameList.GetGameName(pGameRoomItem,szGameName,sizeof(szGameName));
	m_pGameListWnd->m_GameList.GetProcessName(pGameRoomItem,szProessName,sizeof(szProessName));

	//����update���и���
	TCHAR szApp[61];//,sz[100];
	TCHAR path[MAX_PATH];
	lstrcpy(szApp,szProessName);
	szApp[lstrlen(szApp)-lstrlen("ico")-1]='\0';
	//wsprintf(path,"%supdate\\update_%s.ini",Glb().m_Path,szApp);
	wsprintf(path,"%s\\%s\\update.ini",GetAppPath().c_str(),szApp);
	CINIFile f(path);
	CString m_key="config";
	CString v,s,ul,uv,n;
	int sm;
	CString str;
	//wushuqun 2009.5.20
	//�汾����Ĭ��ֵ����Ȼ������update.exe ����
	ComRoomInfo * pComRoomInfo=&pGameRoomItem->m_RoomInfo;

	v=f.GetKeyVal(m_key,"-v","0");//�汾	
	s=f.GetKeyVal(m_key,"-s","./");//·��
	ul=f.GetKeyVal(m_key,"-ul","");//�����б��ַ	
	uv=f.GetKeyVal(m_key,"-uv","");//���°汾��ַ
	n=f.GetKeyVal(m_key,"-n","��Ϸ����");//��Ϸ����	
	sm=f.GetKeyVal(m_key,"-sm",1);//��ʾ��ʽ


	if (ul =="")
	{
		ul.Format("%s/update_version.txt",szApp);
		uv = ul;
	}

	bool getver=false;
	TCHAR ser[20];
	wsprintf(ser,"%u.%u",pComRoomInfo->uVer>>16,pComRoomInfo->uVer&0x0000ffff);


	{
		try
		{
			::EnterCriticalSection(&m_Section);
			if(m_UpdateList.size()>=64)
			{
				::LeaveCriticalSection(&m_Section);
				str = fMsg.GetKeyVal("MainRoom","TooManyUpdateTryLater","���ڸ��µ���Ϸ̫��,�������.");
	//			s.Format("���ڸ��µ���Ϸ̫��,�������.");
				
				//AFCMessageBox(str,szGameName);
                DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,szGameName,false,str);
				return false;
			}
			else
			{
				::LeaveCriticalSection(&m_Section);
			}
		}
		catch(...)
		{
			::LeaveCriticalSection(&m_Section);
		}
		STARTUPINFO StartupInfo;
		ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
		StartupInfo.cb = sizeof(STARTUPINFO);
		PROCESS_INFORMATION ProcessInfo;
		char up[1000];
		//wsprintf(up," -n %s -s %s -v %s -ul %s%s -uv %s%s -sm %d",n,s,v,Glb().m_UpdateUrl,ul,Glb().m_UpdateUrl,uv,sm);
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		bool setup;

		setup=true;
		hFind = FindFirstFile(path, &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE) 
		{

		} 
		else 
		{
			setup=false;
			FindClose(hFind);
		}

		//wushuqun 2009.5.21
		//��ԭ��ԭ�и��·�ʽ
		/*if(setup)
			wsprintf(up," -u %s//%s//update_version.txt -l %s -v %s -n",Glb().m_UpdateUrl,szApp,path,ser);
		else wsprintf(up," -u %s//%s//update_version.txt -l %s -v %s",Glb().m_UpdateUrl,szApp,path,ser);*/

		//wsprintf(up," -n %s -s %s -v %s -ul %s%s -uv %s%s -sm %d",n,s,v,Glb().m_UpdateUrl,ul,Glb().m_UpdateUrl,uv,sm);
		//OutputDebugString(up);
		string _path;
		_path = GetAppPath() + "\\" + szApp;
		sprintf(up, "-n %s -s %s\\ -v %s -ul %s%s -uv %s%s -sm %d", n, _path.c_str(),v, Glb().m_UpdateUrl, ul,Glb().m_UpdateUrl,uv, 1);
		//AfxMessageBox(up);

		// 2012.08.20 yyf ����
		//	if(!CreateProcess(TEXT("update.exe"), up,0,0,FALSE,0,0,0,&StartupInfo,&ProcessInfo))
		//	{
		////		TCHAR sz[200];
		////		wsprintf(sz,"������������ʧ��. ���°�װ�������ܻ��޸�������.");
		//		str = fMsg.GetKeyVal("MainRoom","UpdateErrorReinstall",
		//			"������������ʧ��. ���°�װ�������ܻ��޸�������.");
		//		AFCMessageBox(str,szGameName);
		//	}

		////-ͬ������- update.exe -------------------------2012.08.24 yyf
		////if(!m_AnonymousPipe.OpenProcess("update.exe"))
		//if(!m_CSocket32.OpenProcess("update.exe"))
		//{ 
		//	str = fMsg.GetKeyVal("MainRoom","UpdateErrorReinstall",
		//		"������������ʧ��. ���°�װ�������ܻ��޸�������.");
		//	AFCMessageBox(str,szGameName); 
		//} 
		////else if(m_AnonymousPipe.GetProcessIdFromName("update.exe"))
		//else if(m_CSocket32.GetProcessIdFromName("update.exe"))
		//{
		//	UpdateStructMsg updateMsg;
		//	memset(&updateMsg,0,sizeof(UpdateStructMsg));
		//	 
		//	updateMsg.command = 1;
		//	updateMsg.nRoomID = pGameRoomItem->m_RoomInfo.uNameID;
		//	wsprintf(updateMsg.g_GameName,"%s",szGameName);
		//	wsprintf(updateMsg.g_url,"%s%s",Glb().m_UpdateUrl,uv);
		//	wsprintf(updateMsg.g_ver,"%s",v); 
		//	  
		//	DWORD dwWrite=0;
		//	//m_AnonymousPipe.Write( &updateMsg ,sizeof(UpdateStructMsg),dwWrite);//���͵��ӽ���
		//	m_CSocket32.Send( &updateMsg ,sizeof(UpdateStructMsg));//���͵��ӽ���

		//	//�ڸ����б�  m_UpdateList ���ӽڵ�
		//	UPDATE *lp=(LPUPDATE)malloc(sizeof(UPDATE));
		//	lp->hProcess=ProcessInfo.hProcess;
		//	lp->uNameID=pGameRoomItem->m_RoomInfo.uNameID;
		//	lp->pGameRoomItem=pGameRoomItem;

		//	try
		//	{
		//		::EnterCriticalSection(&m_Section);
		//		if(m_UpdateList.size()<64)
		//			m_UpdateList.push_back(lp);
		//		//if(m_UpdateList.GetCount()==1)
		//		//if(needupdate)
		//		SetTimer(CHECK_UPDATE,2000,NULL);
		//		::LeaveCriticalSection(&m_Section);
		//	}
		//	catch(...)
		//	{
		//		::LeaveCriticalSection(&m_Section);
		//	}
		//}
		////--------------------------------------------------------

		//-�첽����- update.exe -------------------------2012.08.24 yyf
		//��ʼ�� sDownLoadUpdateMsg Ȼ���ٴ��� ���ظ����߳�   DownLoadUpdate(LPVOID lparam)
		sDownLoadUpdateMsg sUpdateMsg; 
		//-----------------
		UpdateStructMsg updateMsg;
		memset(&updateMsg,0,sizeof(UpdateStructMsg));
		updateMsg.command = 1;
		updateMsg.nRoomID = pGameRoomItem->m_RoomInfo.uNameID;
		wsprintf(updateMsg.g_GameName,"%s",szGameName);
		wsprintf(updateMsg.g_url,"%s%s",Glb().m_UpdateUrl,uv);
		wsprintf(updateMsg.g_ver,"%s",v); 
		sUpdateMsg.pUpdateStructMsg = &updateMsg;
		//-----------------
		UPDATE *lp=(LPUPDATE)malloc(sizeof(UPDATE));
		lp->hProcess=ProcessInfo.hProcess;
		lp->uNameID=pGameRoomItem->m_RoomInfo.uNameID;
		lp->pGameRoomItem=pGameRoomItem;
		sUpdateMsg.pUPDATE = lp;
		//-----------------
		sUpdateMsg.pCMainRoomEx = this;
		//-----------------
		sUpdateMsg.pCBcfFile = &fMsg;
		//-----------------

		DWORD dwThreadID = 0;
		HANDLE hThread = CreateThread(0,0,DownLoadUpdate,&sUpdateMsg,0,&dwThreadID);

		if (WaitForSingleObject(hThread,5000)==WAIT_TIMEOUT) 
		{//��ʱ update.exe ��û������
			if(m_CSocket32.GetProcessInfo().hProcess)
			{
				::TerminateProcess( m_CSocket32.GetProcessInfo().hProcess, 0 );//�������� update.exe
			}
			m_CSocket32.Close();

			CString str;
			CString strTitle;
			str = fMsg.GetKeyVal("MainRoom","DownLoadOver","����ʧ��");
			strTitle = fMsg.GetKeyVal("MainRoom","MessageTitle","��ʾ");
			
			//AFCMessageBox(str,strTitle);
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,strTitle,false,str);
		}
		if (hThread)
		{
			::CloseHandle(hThread);
		}
		//----------------------------------------------------------
		return false;
	}
	return true;

// 	char path[MAX_PATH];
// 	lstrcpy(path, Glb().m_CenterServerPara.m_strDownLoadSetupADDR);
// 	int ilength = strlen(path);
// 	int iCount  = strlen(filename);
// 	iCount -= strlen(".ico");
// 	for(int i = 0,j = ilength; (i< iCount)&&(j < 150); i++,j++ )
// 	{
// 		path[j] = filename[i];
// 	}
// 	char chTemp[] = ".exe";
// 	for (int i = 0; i < 4;i++)
// 	{
// 		path[ilength+iCount+i] = chTemp[i];
// 	}
// 	path[ilength+iCount+4] = '\0';
// 
// 	// �����ǰ�����������ھͽ�������������������������ļ�����
// 	CHttpDownloadDlg *pdlg=new CHttpDownloadDlg(this);
// 	pdlg->m_Gamelist=&(m_pGameListWnd->m_GameList);
// 	pdlg->m_itemdata=pGameRoomItem->m_pParentItem;
// 	pdlg->m_sURLToDownload = path;
// 	CString filepath=path;
// 	int nSlash = filepath.ReverseFind(_T('/'));
// 	if (nSlash == -1)
// 		nSlash = filepath.ReverseFind(_T('\\'));
// 	::CreateDirectory("download",NULL);
// 	lstrcpy(path,"download\\");
// 	if (nSlash != -1 && filepath.GetLength() > 1)
// 		lstrcat(path , filepath.Right(filepath.GetLength() - nSlash - 1));
// 	else
// 		lstrcat(path, filepath);
// 	pdlg->m_sFileToDownloadInto=path;
// 	/*if (m_sUserName.GetLength())
// 	{
// 	dlg->m_sUserName = m_sUserName;
// 	dlg->m_sPassword = m_sPassword;
// 	}*/
// 	pdlg->Create(IDD_HTTPDOWNLOAD,this);
// 	pdlg->CenterWindow();
// 	pdlg->ShowWindow(1);	
// 
// 	return true;
}

#ifdef AI_MACHINE
bool CMainRoomEx::EnterRoom(int roomid)
{
	CAFCRoomItem *pRoomItem =m_pGameListWnd->m_GameList.FindRoomItem(m_Airoom);
	if(!pRoomItem)
	{
	//	TCHAR sz[100];
	//	wsprintf(sz, _T("�Ҳ������� %d"), m_Airoom);
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString str;
		str = fMsg.GetKeyVal("MainRoom","NoRoom","�Ҳ� ������ %d");
		
		//AFCMessageBox(str);
        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,str);
		return false;
	}
	return OnCentranceGameRoom(pRoomItem);
}
#endif

//���±�����Ϣ
BOOL CMainRoomEx::UpdateMeInfo(__int64 i64Money,BOOL IsUpdateMoney,int dwFascination,BOOL IsUpDateFascination)
{
	if(IsUpdateMoney)
		m_PlaceUserInfo.i64Money = i64Money ;

	if(IsUpDateFascination)
		m_PlaceUserInfo.dwFascination = dwFascination;

	/// ֻ����ʾ�б�ʱ��ˢ�£�����ʱ��ˢ��
	CRect rc;
	m_pGameListWnd->GetClientRect(&rc);
	if (rc.Width()>100)
	{
		m_pGameListWnd->Invalidate(FALSE);
	}
	return TRUE;
}

//��������GameFrame�����н������Ϣ
//������Ҵ򿪶����Ϸ���У�����һ�������б仯���ѱ仯ͬ����������������д���
//JianGuankun 2012.1.5
void CMainRoomEx::UpdateBankMoneyToAllRoom(__int64 i64Money,CBaseRoom* pLaunchRoom/* = NULL*/)
{
	//���·����û�////
	CM_UserState_Change ControlMessage;
	ControlMessage.bActionCode = ACT_USER_BANKMONEY;

	UserItemStruct* pUis = new UserItemStruct;
	::memset(pUis,NULL,sizeof(UserItemStruct));
	pUis->GameUserInfo.dwUserID = m_PlaceUserInfo.dwUserID;
	pUis->GameUserInfo.i64Bank = i64Money;
	ControlMessage.pUserItem = pUis;

	CGameRoomEx* p = (CGameRoomEx*)pLaunchRoom;
	/*CString str;
	str.Format("DClient:GameID = %d,Send bank money to all room.Money = %I64d",p->m_GameInfo.uNameID,i64Money);
	OutputDebugString(str);*/

	//��Ϸ���������
	for (BYTE i = 0; i < MAX_GAME_ROOM; ++i)
	{
		// ��Ϸ����Ϸ�����ұ���Ϣ��
		CGameRoomEx* pGameRoomEx = (CGameRoomEx*)m_RoomInfo[i].pGameRoomWnd;
		if (pLaunchRoom == pGameRoomEx) //������Ϣ�ķ��������ٸ��½����
		{
			continue;
		}
		if (NULL != pGameRoomEx)
		{
			//������Ϸ�������
			if (pGameRoomEx->m_IGameFrame)
			{
				ControlMessage.bDeskStation = pGameRoomEx->m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
				pGameRoomEx->m_IGameFrame->OnControlMessage(CM_USER_STATE,&ControlMessage,sizeof(CM_UserState_Change));
			}

			//���´������������
			pGameRoomEx->FlushMyBankMoney(i64Money);

			//���´����򿪵����н������
			if (pGameRoomEx->m_pBankWndEx && pLaunchRoom)
			{
				CGameRoomEx* pLRoom = (CGameRoomEx*)pLaunchRoom;
				pGameRoomEx->m_pBankWndEx->SetGameWalletMoney(pLRoom->m_GameInfo.uNameID,pLRoom->m_GameInfo.pMeUserInfo->GameUserInfo.i64Money);
				pGameRoomEx->m_pBankWndEx->SetWealthInfor(i64Money,0);
			}
		}	
	}

	delete pUis;

	return;
}

//
void CMainRoomEx::ChangeImageList(COLORREF color)
{
	//CImageList *m_UserStateList=&CHappyGoDlg::m_UserStateList;

	//m_UserStateList->DeleteImageList();
	////װ��λͼ
	//BITMAP BmpInfo;
	//CGameImage	UserStateBmp;
	//CBitmap NewUserStateBmp;
	//AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
	//UserStateBmp.SetLoadInfo(".\\image\\user_state.bmp",true);;
	//AfxSetResourceHandle(GetModuleHandle(NULL));
	////UserStateBmp.GetBitmap(&BmpInfo);
	//BmpInfo.bmWidth=UserStateBmp.GetWidth();
	//BmpInfo.bmHeight=UserStateBmp.GetHeight();

	////����λͼ
	//CDC TargetDC, * pDC=GetDC();
	//TargetDC.CreateCompatibleDC(pDC);
	//NewUserStateBmp.CreateCompatibleBitmap(pDC,BmpInfo.bmWidth,BmpInfo.bmHeight);
	//CBitmap * pOldBmp=TargetDC.SelectObject(&NewUserStateBmp);
	//TargetDC.FillSolidRect(0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,color);
	//AFCStretchImage(&TargetDC,0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,UserStateBmp,0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,RGB(255,255,255));
	//TargetDC.SelectObject(pOldBmp);
	//TargetDC.DeleteDC();

	////������Դ��
	//m_UserStateList->Create(USER_STATE_WIDTH,USER_STATE_HEIGHT,ILC_COLOR16,0,0);
	//m_UserStateList->Add(&NewUserStateBmp,RGB(0,0,255));

	////������Դ
	//UserStateBmp.Detach();
	//NewUserStateBmp.Detach();
	//ReleaseDC(pDC);
}

//�����书�ܰ�ť
//
void CMainRoomEx::OnBnClickedBt1()
{
	if (m_WebView.GetSafeHwnd()!=NULL)
		m_WebView.GoBack();
}
//
void CMainRoomEx::OnBnClickedBt2()
{
	if (m_WebView.GetSafeHwnd()!=NULL)
		m_WebView.GoForward();
}
//
void CMainRoomEx::OnBnClickedBt3()
{
	if (m_WebView.GetSafeHwnd()!=NULL)
		m_WebView.Refresh();
}
//
void CMainRoomEx::OnBnClickedBt4()
{
	if (m_WebView.GetSafeHwnd()!=NULL)
		m_WebView.Stop();
}
//
void CMainRoomEx::OnBnClickedBt5()
{
	for(int i = 0; i < MAX_GAME_ROOM; i ++)
	{
		if(m_RoomInfo[i].bAccess)
		{
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
			CString str;
			CString strTitle;
			str = fMsg.GetKeyVal("MainRoom","CloseAllRooms","���ȹر����з��䣡");
			strTitle = fMsg.GetKeyVal("MainRoom","LoginAgain","���µ�½");
			
			//AFCMessageBox(str,strTitle);//,MB_ICONQUESTION,this);
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,strTitle,false,str);
			return ;
		}
	}
	
	_TAG_USERPROP *userProp=NULL;
	int propCount=Glb().userPropLibrary.GetCount();
	for(int i=0;i<propCount;i++)
	{
		userProp=Glb().userPropLibrary.GetAt(0);
		Glb().userPropLibrary.RemoveAt(0);
		delete userProp;
	}
	if(propPannel)
		propPannel->ShowWindow(SW_HIDE);

	if(m_pGameListWnd && m_pGameListWnd->personalSet)
		m_pGameListWnd->personalSet->ShowWindow(SW_HIDE);


	//if(AFCMessageBox(TEXT("�����ȷ�����������л�����½��ȷ����"),TEXT("���µ�½"),MB_ICONQUESTION|MB_OKCANCEL,this)==IDOK)
	CloseSocket(true);        
	//if(imMain)
	//{
	//	imMain->clearAllSession();
	//	imMain->closeIMDialog();
	//}

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

	CMainRoomEx::m_bBankLogoned = false;

	PostMessage(WM_COMMAND,IDM_LOGON_PLACE,0);
	return;
}
//
void CMainRoomEx::OnBnClickedBt6()
{
	;
}
//
void CMainRoomEx::OnBnClickedBt7()
{
	;
}
//
void CMainRoomEx::OnBnClickedBt8()
{
	MessageBox("CMainRoomEx::OnBnClickedBt8()");
}
//
void CMainRoomEx::OnBnClickedBt9()
{
	;
}
//
void CMainRoomEx::OnBnClickedBt10()
{
	;
}
//��ʾ�����������
LRESULT CMainRoomEx::OnShowLeft(WPARAM wparam,LPARAM lparam)
{
	if((BOOL)wparam&&m_pGameListWnd->GetLeftShowOrHide())
	{
		m_TopWnd->m_bHide=true;
		m_TopWnd->FixControlStation();
		m_pGameListWnd->OnBnClickedBt1();
		return 0;
	}

	if(!(BOOL)wparam &&!(m_pGameListWnd->GetLeftShowOrHide()))
	{
		m_TopWnd->m_bHide=false;
		m_TopWnd->FixControlStation();
		m_pGameListWnd->OnBnClickedBt1();
	}

	return 0;
}

LRESULT CMainRoomEx::OnTopTabLostSelected(WPARAM wparam,LPARAM lparam)
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

	return 0;
}

LRESULT CMainRoomEx::OnTopTabSelected(WPARAM wparam,LPARAM lparam)
{
	//�����MainRoom,ʱgpGameRoomEx��������һ��GameRoom�У����û��GameRoom���򿪣���gpGameRoomExΪNULL
	//Added by JianGuankun 2012.2.14

	CMainRoomEx* pMainRoom = GetMainRoom();
	RoomInfoStruct* pRoomInfo = NULL;

	bool bHaveGameRoom = false;

	if(pMainRoom) 
	{
		if(pMainRoom->GetRoomInfoStruct(&pRoomInfo))
		{
			for (BYTE i = 0; i < MAX_GAME_ROOM; i++)
			{
				CGameRoomEx* pGameRoom = (CGameRoomEx*)pRoomInfo[i].pGameRoomWnd;
				if (pGameRoom)
				{
					if (IsWindow(pGameRoom->GetSafeHwnd()))
					{
						gpGameRoomEx = pGameRoom;
						bHaveGameRoom = true;
					}
				}
			}
		}
	}

	if (!bHaveGameRoom)
	{
		gpGameRoomEx = NULL;
	}

	return 0;
}
//����
void CMainRoomEx::IE(LPCTSTR URL)
{
	if (m_WebView.GetSafeHwnd()!=NULL)
		m_WebView.Navigate(URL,NULL,NULL,NULL,NULL);
}

LRESULT CMainRoomEx::OnCallGM(WPARAM wparam, LPARAM lparam)
{
	SendData(MDM_GP_PLAYER_2_GM, ASS_GP_GET_GM_LIST, 0);
	return 0;
}

//��IE
LRESULT CMainRoomEx::OnOpenIE(WPARAM wparam,LPARAM lparam)
{
	CString url;
	switch((int)wparam)
	{
	case 11://ͷ���ϴ�
		//OnUpLoadUserLogo(0,0);
		break;
	default:
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
				Glb().m_CenterServerPara.m_strWebRootADDR,loginURL,m_PlaceUserInfo.dwUserID,Glb().m_TML_SN,url);
			IE(surl);
			
			for(int i = 0; i < MAX_GAME_ROOM; i ++)
				if(m_RoomInfo[i].pGameRoomWnd != NULL && m_RoomInfo[i].pGameRoomWnd->IsWindowVisible())
					m_RoomInfo[i].pGameRoomWnd->PostMessage(IDM_OPEN_IE,wparam,lparam);
			break;
		}
	}
	return 0;
}

LRESULT CMainRoomEx::OnOpenBank(WPARAM wParam, LPARAM lParam)
{
	for(int i = 0; i < MAX_GAME_ROOM; ++i)
	{
		if(m_RoomInfo[i].pGameRoomWnd != NULL)
		{
			//AFCMessageBox("�뵽��Ϸ������ʹ�����С�");
            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�뵽��Ϸ������ʹ�����С�");
			return 0;
		}
	}

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	/// ��ʾ��ʾ�Ի����ֱ�ӷ��ʹ�������Ϣ������Ҫ��������
	if (m_Connect.GetSafeHwnd()==NULL)
		m_Connect.Create(IDD_CONNECT,this);
	m_Connect.SetMessageText(fMsg.GetKeyVal("GameRoom","OpenningBankWait","���ڴ�Ǯ��,���Ժ�..."));
	m_Connect.GetDlgItem(IDC_QUIT)->EnableWindow(false);
	m_Connect.ShowWindow(SW_SHOW);
	SendData(MDM_GP_BANK,ASS_GP_BANK_OPEN,0);

	return 0;
}

LRESULT CMainRoomEx::OnAppeal(WPARAM wparam,LPARAM lparam)
{
	if(!m_pAppealDlg)
	{
		m_pAppealDlg=new CAppealDlg(this);
		m_pAppealDlg->Create(IDD_DLG_APPEAL,this);
	}
	m_pAppealDlg->OpenAppealDlg(m_PlaceUserInfo.szName);
	return 0;

}
LRESULT CMainRoomEx::OnLockAccount(WPARAM wParam, LPARAM lParam)
{
	//MessageBox("�ڴ˴���������������");
	OnSwitchLockButton();
	return 0;
}
//�ϴ�ͷ��
LRESULT CMainRoomEx::OnUpLoadUserLogo(WPARAM wparam,LPARAM lparam)
{
	return true;
	if(!logoUpload)
	{
		logoUpload=new CLogoUpload;
		logoUpload->Create(IDD_LOGOUPLOAD,this);
		logoUpload->SetParentWnd(this);
	}
	logoUpload->ShowWindow(SW_SHOW);
	logoUpload->CenterWindow();

	return true;
}

//������ͷ��
void CMainRoomEx::checkUserLogo(long dwUserID)
{
	if(!logoUpload)
	{
		logoUpload=new CLogoUpload;
		logoUpload->Create(IDD_LOGOUPLOAD,this);
		logoUpload->SetParentWnd(this);
	}
	logoUpload->checkUserLogo(dwUserID);
}

void CMainRoomEx::OpenBulletin(void)
{
	if(!hBulletin)
	{
		hBulletin=new CBulletin;
		hBulletin->Create(IDD_BULLETIN,this);
	}
	/*CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	CString surl=f.GetKeyVal("BZW","LogonBulletin","about:blank");*/
	//hBulletin->OpenBulletin(surl);
	hBulletin->ShowWindow(SW_SHOW);
	hBulletin->CenterWindow();
}

void CMainRoomEx::propGetListRequest(void)
{
	MSG_PROP_C_GETSAVED msgPropGetSaved;
	msgPropGetSaved.dwUserID=m_PlaceUserInfo.dwUserID;
	SendData(&msgPropGetSaved,sizeof(MSG_PROP_C_GETSAVED),MDM_GP_PROP,ASS_PROP_GETUSERPROP,0);

}


/************************************************************************/
//Function:checkNetSignal()
//Parameter:none
//Author:Fred Huang 2008-06-27
//Note:�������������������Ϣ
/************************************************************************/
void CMainRoomEx::checkNetSignal(void)
{
        if(m_PlaceUserInfo.dwUserID==0)        //û��ID�ţ�����
                return;
        //����ϴε���Ϣ��û���յ��������񼫲����Ϣ����Ҫ��
        if(m_NetSignalIndex>m_SavedSingalIndex)
        {
                m_NetSignalLevel=0;
                
        }
		//drawSignal();
        //���ṹ��
        MSG_GP_NETSignal netSignal;
        netSignal.dwIndex=++m_NetSignalIndex;
        netSignal.dwUserID=m_PlaceUserInfo.dwUserID;
        netSignal.dwTimeFlag=GetTickCount();
        //������Ϣ
        SendData(&netSignal,sizeof(MSG_GP_NETSignal),MDM_GP_NETSIGNAL,0,0);
}

/************************************************************************/
//Function:checkNetSignal()
//Parameter:
//                pNetData        :���������ص���Ϣ��
//Author:Fred Huang 2008-06-27
//Note:�������������ص���Ϣ
/************************************************************************/
void CMainRoomEx::getNetSignal(void * pNetData)
{
        MSG_GP_NETSignal *netSignal=(MSG_GP_NETSignal*)pNetData;
        //������Ƿ����Լ��ģ�����
        if(m_PlaceUserInfo.dwUserID!=netSignal->dwUserID)
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
        //drawSignal();
		//Invalidate(true);
}

/************************************************************************/
//Function:drawSignal()
//Parameter:��
//Author:Fred Huang 2008-06-27
//Note:��ͼ
/************************************************************************/
void CMainRoomEx::drawSignal(void)
{
        if(!img || img->GetFlags()==0)
                return;
		CString s=CBcfFile::GetAppPath();
		CBcfFile f( s + "skin0.bcf");
		CString key=TEXT("MainRoom");

		CRect rc;
		GetClientRect(rc);
     
		int x = rc.Width() - f.GetKeyVal(key, _T("game_hall_signalx"),10) - img->GetWidth();
		int y = f.GetKeyVal(key, _T("game_hall_signaly"),0);
		Graphics g(this->m_hWnd);
        g.DrawImage(img,Rect(x,y,img->GetWidth(),img->GetHeight()/6),0,m_NetSignalLevel*img->GetHeight()/6,img->GetWidth(),img->GetHeight()/6,UnitPixel);
}

// PengJiLin, 2010-9-13, ���˿������߿����ܣ�����ʹ��ʱ��
void CMainRoomEx::SetKickPropTime(int iKickTime)
{
    m_iKickPropTotalTime = iKickTime;
}
void CMainRoomEx::SetAntiKickPropTime(int iAntiKickTime)
{
    m_iAntiKickPropTotalTime = iAntiKickTime;
}

// PengJiLin, 2011-6-23, ������Ϣ
void CMainRoomEx::SetActiveToMainRoom(UINT uState)
{
	return; //����ˢ�������ѽ����û�м���ı�Ҫ��JianGuankun 2012.5.11
    CGameRoomEx* pGameRoom = NULL;
    for (BYTE i=0;i<MAX_GAME_ROOM;i++)
    {
		if (m_RoomInfo[i].bAccess) //���bAccess�ж���Ƿ�pGameRoomָ���жϣ�Add by JianGuankun 2012.2.15
		{
			try
			{
				pGameRoom = (CGameRoomEx*)m_RoomInfo[i].pGameRoomWnd;
				if(NULL != pGameRoom)
				{
					pGameRoom->SetActiveToGameRoom(uState);
				}
			}
			catch (...)
			{
				return;
			}
		}
    }
}

#ifdef MY_QUICK_GAME_ON

//JianGK,����ͨѶ�ܵ�
bool CMainRoomEx::OpenQuickGameNamedPipe(char* szPrefixName)
{
	if (m_hQGNamedPipe != INVALID_HANDLE_VALUE)
	{
		return false;
	}

	TCHAR szPipeName[32];

	for (int i = 0; i < 10; ++i)
	{
		::sprintf(szPipeName,"%s_%02d",szPrefixName,i);

		m_hQGNamedPipe = ::CreateNamedPipe(szPipeName,PIPE_ACCESS_DUPLEX|FILE_FLAG_WRITE_THROUGH,
			PIPE_WAIT|PIPE_TYPE_BYTE,1,128,128,NULL,NULL);

		if (m_hQGNamedPipe != INVALID_HANDLE_VALUE)
		{
			break;
		}
	}

	if (m_hQGNamedPipe != INVALID_HANDLE_VALUE)
	{
		DWORD dwThreadID = 0;
		//m_hNPThread = ::CreateThread(NULL,0,NamedPipeThread,this,NULL,&dwThreadID);
		CWinThread* pThread = AfxBeginThread(NamedPipeThread,this);
		m_hQGThread = pThread->m_hThread;
		if (m_hQGThread)
		{
			return true;
		}
	}
	else
	{
		CloseNamedPipe();
	}

	return false;
}

//JianGK,�ر�ͨѶ�ܵ�
bool CMainRoomEx::CloseNamedPipe()
{
	if (m_hQGNamedPipe != INVALID_HANDLE_VALUE)
	{
		if (m_hQGThread != INVALID_HANDLE_VALUE)
		{
			::TerminateThread(m_hQGThread,EXIT_FAILURE);
			::CloseHandle(m_hQGThread);

			::DisconnectNamedPipe(m_hQGNamedPipe);
			::CloseHandle(m_hQGNamedPipe);

			m_hQGNamedPipe = INVALID_HANDLE_VALUE;
			m_hQGThread = INVALID_HANDLE_VALUE;
		}
	}

	return true;
}

//JianGK,�ܵ�ͨѶ�߳�
//DWORD WINAPI CMainRoomEx::NamedPipeThread(LPVOID param)
UINT AFX_CDECL CMainRoomEx::NamedPipeThread(LPVOID param)
{
	CMainRoomEx* pMainRoom = (CMainRoomEx*)param;

	while (true)
	{
		if (::ConnectNamedPipe(pMainRoom->m_hQGNamedPipe,NULL)) 
		{
			TQGInfo tagQG;
			DWORD dwReadNums = 0;

			::ReadFile(pMainRoom->m_hQGNamedPipe,&tagQG,sizeof(TQGInfo),&dwReadNums,NULL);

			pMainRoom->m_uQGRoomID = tagQG.uRoomID;
			
			::DisconnectNamedPipe(pMainRoom->m_hQGNamedPipe);

			CGameRoomEx* pGameRoom = (CGameRoomEx*)pMainRoom->m_RoomInfo[0].pGameRoomWnd;

			if (!pGameRoom)
			{
				CGameListCtrl* pGameList = &pMainRoom->m_pGameListWnd->m_GameList;
				
				CAFCNameItem* pNameItem = pGameList->FindNameItem(tagQG.uNameID);

				if (pNameItem)
				{
					pGameList->Expand(pNameItem->m_hCurrentItem,TVE_EXPAND);

					pMainRoom->SetTimer(IDT_QUICK_ENTER_ROOM,100,NULL);
				}
			}
			else
			{
				pGameRoom->OnFindNullDesk();
			}
		}
	}

	return 0;
}

#endif // MY_QUICK_GAME_ON
void CMainRoomEx::char2Tchar(char *pchar,TCHAR * tchar) 
{
#ifndef UNICODE
	strcpy(tchar, pchar);
#else
	int   nwLen  =MultiByteToWideChar(CP_ACP,0,pchar,-1,NULL,0); 
	MultiByteToWideChar(CP_ACP,0,pchar,-1,tchar,nwLen);
#endif

}

string CMainRoomEx::JoinUrl(string p1, string p2)
{
	int i = p1.size();
	while (p1[i - 1] == '/' && i - 1 >= 0)
	{
		i--;
	}
	p1 = p1.substr(0, i);

	i = 0;
	while (p2[i] == '/' && i < p2.size())
	{
		i++;
	}
	p2 = p2.substr(i, p2.size() - i);

	return p1 + "/" + p2;
}

RoomInfoStruct* CMainRoomEx::GetCurrentGameRoom()
{
	return &m_RoomInfo[0];
}

CString CMainRoomEx::GetUrlValue(CString szUrl, CString szField)
{
	szField=szField+"=";
	CString sValue="";
	int nStart=szUrl.Find(szField);
	if(nStart==-1)
		return "";
	nStart+=szField.GetLength();
	int nStop=szUrl.Find("&",nStart);
	if(nStop==-1)
		nStop=szUrl.GetLength();
	sValue= szUrl.Mid(nStart,nStop-nStart);
	return sValue;
}

//�����ܵ��ص� �� �ӿ� 2012.08.20 yyf
void CMainRoomEx::ReadAnonymousPipe(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead)
{  
	UpdateStructMsg *pmsg = (UpdateStructMsg*)lpBuffer;
	CString str;
	str.Format("yyf: nRoomID = %d ,g_GameName = %s,  g_url = %s, g_ver = %s",
		pmsg->nRoomID,pmsg->g_GameName,pmsg->g_url,pmsg->g_ver);
	OutputDebugString("yyf: CMainRoomEx::ReadAnonymousPipe(..)");
	OutputDebugString(str);

	if(NULL != pmsg)
	{ //�յ��ӽ��̹�������Ϣ��˵���Ǹ������߳��Ѿ���ɡ���߿��԰���ͬ roomid �Ĵ� ���ڸ����б� m_UpdateList �� ɾ��
		for(vector<LPUPDATE>::iterator it = m_UpdateList.begin(); it != m_UpdateList.end();it++)
		{
			LPUPDATE lp = *it;
			if(!lp) continue;
			if(NULL != lp && lp->uNameID == pmsg->nRoomID )
			{
				//�Ѵ����б�ˢ��һ��
				CAFCRoomItem *RoomItem=lp->pGameRoomItem;
				if(NULL != RoomItem)
				{
					m_pGameListWnd->m_GameList.UpdataItem(RoomItem->m_pParentItem);
				}
				lp->uNameID = -2;
			}
		}
	}

	switch(pmsg->command)
	{
	case 0:
		{
			break;
		}
	case 2:
		{ 
			break;
		}
	} 
}
//�������̼�����ص��ӿ�
void CMainRoomEx::ReadSimpleSocket(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead)
{   
	UpdateStructMsg *pmsg = (UpdateStructMsg*)lpBuffer;
	CString str;
	str.Format("yyf: nRoomID = %d ,g_GameName = %s,  g_url = %s, g_ver = %s",
		pmsg->nRoomID,pmsg->g_GameName,pmsg->g_url,pmsg->g_ver);
	OutputDebugString("yyf: CMainRoomEx::ReadSimpleSocket(..)");
	OutputDebugString(str);

	if(NULL != pmsg)
	{ //�յ��ӽ��̹�������Ϣ��˵���Ǹ������߳��Ѿ���ɡ���߿��԰���ͬ roomid �Ĵ� ���ڸ����б� m_UpdateList �� ɾ��
		for(vector<LPUPDATE>::iterator it = m_UpdateList.begin(); it != m_UpdateList.end();it++)
		{
			LPUPDATE lp = *it;
			if(!lp) continue;
			if(NULL != lp && lp->uNameID == pmsg->nRoomID )
			{
				//�Ѵ����б�ˢ��һ��
				CAFCRoomItem *RoomItem=lp->pGameRoomItem;
				if(NULL != RoomItem)
				{
					m_pGameListWnd->m_GameList.UpdataItem(RoomItem->m_pParentItem);
				}
				lp->uNameID = -2;
			}
		}
	}

	switch(pmsg->command)
	{
	case 0:
		{
			break;
		}
	case 2:
		{ 
			break;
		}
	} 
}

////���ظ����߳�  2012.08.24 yyf
DWORD WINAPI CMainRoomEx::DownLoadUpdate(LPVOID lparam)
{ 
	sDownLoadUpdateMsg *psDownLoadUpdateMsg = (sDownLoadUpdateMsg *)lparam; 

	if(NULL == psDownLoadUpdateMsg)
	{
		return -1;
	}

	if(NULL == psDownLoadUpdateMsg->pCMainRoomEx)
	{
		return -1;
	}

	if(! (psDownLoadUpdateMsg->pCMainRoomEx->m_CSocket32.OpenProcess("update.exe")))
	{ 
		CString str;
		str = psDownLoadUpdateMsg->pCBcfFile->GetKeyVal("MainRoom","UpdateErrorReinstall",
			"������������ʧ��. ���°�װ�������ܻ��޸�������.");
		
		//AFCMessageBox(str, psDownLoadUpdateMsg->pUpdateStructMsg->g_GameName);  
        DUIMessageBox(NULL,MB_ICONINFORMATION|MB_OK,psDownLoadUpdateMsg->pUpdateStructMsg->g_GameName,false,str);
	}  
	else if(psDownLoadUpdateMsg->pCMainRoomEx->m_CSocket32.GetProcessIdFromName("update.exe"))
	{  
		psDownLoadUpdateMsg->pCMainRoomEx->m_CSocket32.Send(psDownLoadUpdateMsg->pUpdateStructMsg ,sizeof(UpdateStructMsg));

		UPDATE *lp= psDownLoadUpdateMsg->pUPDATE;

		if(NULL != lp && lp->uNameID >=0)
		{
			try
			{
				::EnterCriticalSection(&(psDownLoadUpdateMsg->pCMainRoomEx->m_Section));
				if(psDownLoadUpdateMsg->pCMainRoomEx->m_UpdateList.size()<64)
				{
					psDownLoadUpdateMsg->pCMainRoomEx->m_UpdateList.push_back(lp); 
				}
				psDownLoadUpdateMsg->pCMainRoomEx->SetTimer(CHECK_UPDATE,2000,NULL);
				::LeaveCriticalSection(&(psDownLoadUpdateMsg->pCMainRoomEx->m_Section));
			}
			catch(...)
			{
				::LeaveCriticalSection(&(psDownLoadUpdateMsg->pCMainRoomEx->m_Section));
			}
		}

	}

	return 0;
}

//��ȡ��������
UINT CMainRoomEx::GetonlineUserCount()
{
	return m_onlineUserCount;
}
//������������
void CMainRoomEx::SetonlineUserCount(UINT onlineUserNum)
{
	m_onlineUserCount = onlineUserNum;
}
