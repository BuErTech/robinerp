// EXEClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EXEClient.h"
#include "EXEClientDlg.h"
#include "../DllForGame/NewGameFrame/IGameEngine.h"
#include "IGameImpl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct MSG_PROP_RESULT
{
	int dwResult;
};

//������Ϸ��ܺ�������
typedef IFrameInterface * (BeginInitGoGame)();
typedef GetOrderName * (SetGameInitClass)(int dwPoint);

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CEXEClientDlg �Ի���




CEXEClientDlg::CEXEClientDlg(CWnd* pParent /*=NULL*/) 
: CDialog(CEXEClientDlg::IDD, pParent)
{
	m_IGameFrame = NULL;
	m_ChannelService = NULL;
	m_dwIPCHeartBeatTick = 0;
	//m_bNeedRefleshProp = false;
	//m_nPropCount = 0;

    // �¿��
    m_pNewGame = NULL;
}
CEXEClientDlg::~CEXEClientDlg() 
{
	if (m_IGameFrame!=NULL) 
	{
		m_IGameFrame->DeleteFrame();
		m_IGameFrame = NULL;
	}
	if (m_hGameInstance!=NULL)
		::FreeLibrary(m_hGameInstance);
}

void CEXEClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEXEClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_GAME_QUIT,				OnGameQuit)
	ON_MESSAGE(WM_REQUEST_PLAYERLIST,		OnRequestPlayList)
	ON_MESSAGE(WM_SEND_GAME_DATA,			OnSendGameData)
	ON_MESSAGE(WM_SET_STATION,				OnSetStation)
	ON_MESSAGE(WM_USE_BOARDCAST,			OnSendBroadData)
	ON_MESSAGE(WM_PROP_BUY,					OnSendBuyProp)
    ON_MESSAGE(WM_PROP_BUY_NEW,				OnSendBuyPropNew)   // PengJiLin, 2010-10-13
	ON_MESSAGE(WM_GIVE_PROP,				OnSendGiveProp)
	ON_MESSAGE(WM_BUY_VIP,					OnSendBuyVIP)		//���ߴ��ص�VIP������Ϣ   add by wyl   11-5-21
    ON_MESSAGE(WM_ACTIVE_TO_ROOM,			OnActiveToRoom)     // PengJiLin, 2011-6-24, ������Ϣ
	ON_MESSAGE(WM_FRIEND,					OnSendAddFriend)
	ON_MESSAGE(WM_FRIEND_R,					OnSendAddFriend_R)
	ON_MESSAGE(WM_GETFRIENDLIST,			OnSendGetFriendList)
	ON_MESSAGE(WM_WINDOW_MOVE,			    OnGameWinMove)
	ON_MESSAGE(WM_SCREEN_SIZE_CHANGE,		OnScreenSizeChange)	//JianGuankun,2012.6.29 ��Ļ�ֱ��ʱ仯

	// duanxiaohui 2011-11-11 ���������
	ON_MESSAGE(WM_EMENY,                    OnSendAddEmeny)     
	ON_MESSAGE(WM_EMENY_R,					OnSendAddEmeny_R)
	// end duanxiaohui

	//ON_MESSAGE(WM_SEND_GAME_DATA_NO_PARAM,	OnSendGameDataNoParam)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CEXEClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEXEClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CEXEClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CEXEClientDlg::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()


extern void DebugPrintfA(const char *p, ...);

// CEXEClientDlg ��Ϣ�������
void CEXEClientDlg::OnDestroy()
{
	if (NULL != m_IGameFrame)
	{
		m_IGameFrame->DeleteFrame();
		m_IGameFrame = NULL;
	}
	CUnZipRes unZipRes;   ///< add by wxx 2010.7.2   �ͷŶ������Դ�ļ�
	unZipRes.ClearMap();
}

/*
BOOL CEXEClientDlg::OnInitDialog()
{
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//����ͼ��	
	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);//ϵͳͼ��
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	MoveWindow(0,0,0,0);
	//CenterWindow();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(SetupIPC())
	{
		SetTimer(TIMER_HEART_BEAT, HEART_BEAT_SECOND*1000, NULL);
		m_dwIPCHeartBeatTick = GetTickCount();
	}
	else
	{
		//OutputDebugString("CEXEClientDlg::OnInitDialog");
		GetDlgItem(IDC_BUTTON1)->PostMessage(WM_LBUTTONDOWN);
		GetDlgItem(IDC_BUTTON1)->PostMessage(WM_LBUTTONUP);
		//OnBnClickedButton1();
	}
#ifdef BZ_ZLIB
	///<�Ȼ�ȡ����  add by wxx 
	CBcfFile fMsg(CBcfFile::GetAppPath()+"..//bzgame.bcf");
	CString strPassWord;
	strPassWord = fMsg.GetKeyVal("BZW","LoginIP1","www.szbzw.com");
	char *password = strPassWord.GetBuffer(strPassWord.GetLength()+1);

	///<��ȡ�ļ�
	unsigned char * pPW=new unsigned char[strlen(password)+1];

	memcpy(pPW,password,strlen(password)+1);

	CUnZipRes unZipRes;
	if(!CBcfFile::IsFileExist("image.r") )
	{
		MessageBox("��Դ�ļ�����");
		OnDestroy();
		return false;
	}
	unZipRes.ReadFile("image.r",pPW);
	unZipRes.ReadFile("../image.r",pPW);
		//DebugPrintf("����Ϊ:%s",pPW);
#endif

	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
*/

BOOL CEXEClientDlg::OnInitDialog()
{
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//����ͼ��	
	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);//ϵͳͼ��
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	MoveWindow(0,0,0,0);
	//CenterWindow();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(SetupIPC())
	{
		SetTimer(TIMER_HEART_BEAT, HEART_BEAT_SECOND*1000, NULL);
		m_dwIPCHeartBeatTick = GetTickCount();
	}
	else
	{
		//OutputDebugString("CEXEClientDlg::OnInitDialog");
		GetDlgItem(IDC_BUTTON1)->PostMessage(WM_LBUTTONDOWN);
		GetDlgItem(IDC_BUTTON1)->PostMessage(WM_LBUTTONUP);
		//OnBnClickedButton1();
	}
#ifdef BZ_ZLIB
	///<�Ȼ�ȡ����  add by wxx 
	CBcfFile fMsg(CBcfFile::GetAppPath()+"..//bzgame.bcf");
	CString strPassWord;
	strPassWord = fMsg.GetKeyVal("BZW","LoginIP1","www.szbzw.com");
	char *password = strPassWord.GetBuffer(strPassWord.GetLength()+1);

	///<��ȡ�ļ�
	unsigned char * pPW=new unsigned char[strlen(password)+1];

	memcpy(pPW,password,strlen(password)+1);

	CUnZipRes unZipRes;
	if(!CBcfFile::IsFileExist("image.r") )
	{
		MessageBox("��Դ�ļ�����");
		OnDestroy();
		return false;
	}

	CString strPackage = CBcfFile::GetAppPath() + "..//image.r";
	BzDui::CPaintManagerUI::SetResourcePackage(strPackage.GetBuffer(),password);

	unZipRes.ReadFile("image.r",pPW);
	unZipRes.ReadFile("../image.r",pPW);
	//DebugPrintf("����Ϊ:%s",pPW);
#endif

	//����DirectUI֧�֣���ʼ��DirectUI
	BzDui::CPaintManagerUI::SetInstance(m_hGameInstance);

	CString s = CBcfFile::GetAppPath ();/////����·��
	CBcfFile f(s + "skin.bcf");
	TCHAR szUIPath[MAX_PATH];
	CString skinfolder;

	skinfolder = f.GetKeyVal("skin0","skinfolder","..\\image\\client\\skin0");
	wsprintf(szUIPath,"%s",skinfolder);
	BzDui::CPaintManagerUI::SetResourcePath(szUIPath);
	BzDui::CPaintManagerUI::MessageLoop();

	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CEXEClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		/*CAboutDlg dlgAbout;
		dlgAbout.DoModal();*/
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEXEClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	//else
	{
		__super::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CEXEClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CEXEClientDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CEXEClientDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

LRESULT CEXEClientDlg::OnGameQuit(WPARAM wParam, LPARAM lParam)
{
    if (NULL!=m_pNewGame)
    {
        CloseGameEngine(&m_pNewGame);
    }

	//OutputDebugString("[aa]SDK CEXEClientDlg::OnGameQuit(");
	if (wParam == 2)
	{
		//OutputDebugString("[aa]SDK CEXEClientDlg::OnGameQuit( 2");
		OnBnClickedButton2();
		return 0;
	}
	if(NULL != this->m_IGameFrame)
	{
		m_IGameFrame->DeleteFrame();
		m_IGameFrame = NULL;
		::FreeLibrary(m_hGameInstance);
	}
	if(m_ChannelService)
	{
		// �����������أ��Ƿ�Ҫ֪ͨ������һ���뿪��Ϣ
		m_ChannelService->CloseChannel(wParam==0?true:false,true);
		delete m_ChannelService;
		m_ChannelService = NULL;
	}
	KillTimer(TIMER_HEART_BEAT);
	//OutputDebugString("[aa]SDK CEXEClientDlg::OnGameQuit( end 0");
	exit(0);
	OnCancel();
	//OutputDebugString("[aa]SDK CEXEClientDlg::OnGameQuit( end 1");
	return LRESULT(0);
}
// ����ͬ���������
LRESULT CEXEClientDlg::OnRequestPlayList(WPARAM wParam, LPARAM lParam)
{
	// ͨ��IPC��ƽ̨����Ϣ
	//��������
	BYTE bDeskNo = (BYTE)wParam;
	WORD wSendSize=sizeof(BYTE);
	if (NULL != m_ChannelService)
	{
		m_ChannelService->SendData(IPC_MAIN_USER, IPC_SUB_ASK_USER_LIST, &bDeskNo,wSendSize);
	}
	return LRESULT(0);

}
// ����������Ϸ��Ϣ
LRESULT CEXEClientDlg::OnSendGameData(WPARAM wParam, LPARAM lParam)
{
	// �������ģ��������޲�����Ϣ����������ݾ����ˡ�
	WORD wBufferLen = (WORD)wParam;
	BYTE *buffer = (BYTE *)lParam;

	SendGameDataStructWithoutParam *pHead = (SendGameDataStructWithoutParam *)buffer;
	// ���������Ϣ
	////DebugPrintf("Dlg��OnSendGameData--��[%d]��[%d]��[%d]��[%d]",
	//	pHead->bMainID, pHead->bAssistantID, pHead->bHandleCode,
	//	wBufferLen-sizeof(SendGameDataStructWithoutParam));
	if (NULL != m_ChannelService)
	{
		m_ChannelService->SendData(IPC_MAIN_SOCKET, IPC_SUB_SOCKET_SEND, buffer, wBufferLen);
	}
	delete []buffer;
	buffer = NULL;
	return LRESULT(0);
}
// ��������״̬����
LRESULT CEXEClientDlg::OnSetStation(WPARAM wParam, LPARAM lParam)
{
	// �����lParam����һ��BYTE
	BYTE bStation = lParam;
	if (NULL != m_ChannelService)
	{
		m_ChannelService->SendData(IPC_MAIN_USER, IPC_SUB_GAME_START, &bStation, sizeof(BYTE));
	}
	return LRESULT(0);
}
// ���͹㲥��Ϣ
LRESULT CEXEClientDlg::OnSendBroadData(WPARAM wParam, LPARAM lParam)
{
	// �����lParam��һ��_TAG_BOARDCAST���渽��һ��int
	BYTE *buffer = (BYTE *)lParam;
	if (NULL != m_ChannelService)
	{
		m_ChannelService->SendData(IPC_MAIN_PROP, IPC_SUB_BROADCAST_BIG, buffer, wParam);
	}
	delete buffer;
	buffer = NULL;
	return LRESULT(0);
}
// ���͹��������Ϣ
LRESULT CEXEClientDlg::OnSendBuyProp(WPARAM wParam, LPARAM lParam)
{
	// ֱ�Ӽ�����Ϣͷ�������Ӵ��ڷ�������Ϣ���͵�ƽ̨
	
	BYTE *buffer = (BYTE *)lParam;
	if (NULL != m_ChannelService)
	{
		m_ChannelService->SendData(IPC_MAIN_PROP, IPC_SUB_BUY_PROP, buffer, wParam);
		_TAG_PROP_BUY *pPropBuy = (_TAG_PROP_BUY *)lParam;
	}
	// �Ӵ��ڷ�������new������ָ�룬��ʱɾ��
	delete buffer;
	buffer = NULL;
	return LRESULT(0);
}

LRESULT CEXEClientDlg::OnSendBuyVIP(WPARAM wParam, LPARAM lParam)
{
	// ֱ�Ӽ�����Ϣͷ�������Ӵ��ڷ�������Ϣ���͵�ƽ̨

	BYTE *buffer = (BYTE *)lParam;
	if (NULL != m_ChannelService)
	{
		m_ChannelService->SendData(IPC_PROP_BUY_VIP, IPC_ASS_BUY_VIP, buffer, wParam);
		_TAG_PROP_BUY_VIP *pPropBuy = (_TAG_PROP_BUY_VIP *)lParam;
	}
	// �Ӵ��ڷ�������new������ָ�룬��ʱɾ��
	delete buffer;
	buffer = NULL;
	return LRESULT(0);
}

LRESULT CEXEClientDlg::OnSendAddFriend(WPARAM wParam, LPARAM lParam)      //��Ӻ���
{
	int buffer = lParam;
	if (NULL != m_ChannelService)
	{
		m_ChannelService->SendData(IPC_FRIEND, IPC_ASS_CHECKFRIEND, &buffer, sizeof(int));
	}
	// �Ӵ��ڷ�������new������ָ�룬��ʱɾ��
	return LRESULT(0);
}
LRESULT CEXEClientDlg::OnSendAddFriend_R(WPARAM wParam, LPARAM lParam)      //��Ӻ���
{
	BYTE *buffer = (BYTE *)lParam;
	if (NULL != m_ChannelService)
	{
		m_ChannelService->SendData(IPC_FRIEND, IPC_ASS_ADDFRIEND, buffer, wParam);
	}
	// �Ӵ��ڷ�������new������ָ�룬��ʱɾ��
	delete buffer;
	buffer = NULL;
	return LRESULT(0);
}
LRESULT CEXEClientDlg::OnSendGetFriendList(WPARAM wParam, LPARAM lParam)      //��Ӻ���
{
	if (NULL != m_ChannelService)
	{
		m_ChannelService->SendData(IPC_FRIEND, IPC_ASS_GETFRIENDLIST);
	}
	return LRESULT(0);
}

// ��Ӻ�����
LRESULT CEXEClientDlg::OnSendAddEmeny(WPARAM wParam, LPARAM lParam)
{
	int buffer = lParam;
	if (NULL != m_ChannelService)
	{
		m_ChannelService->SendData(IPC_EMENY, IPC_ASS_CHECKEMENY, &buffer, sizeof(int));
	}

	// �Ӵ��ڷ�������new������ָ�룬��ʱɾ��
	return LRESULT(0);
}

// ���������
LRESULT CEXEClientDlg::OnSendAddEmeny_R(WPARAM wParam, LPARAM lParam)
{
	BYTE *buffer = (BYTE *)lParam;
	if (NULL != m_ChannelService)
	{
		m_ChannelService->SendData(IPC_EMENY, IPC_ASS_ADDEMENY, buffer, wParam);
	}

	// �Ӵ��ڷ�������new������ָ�룬��ʱɾ��
	delete buffer;
	buffer = NULL;

	return LRESULT(0);
}

// PengJiLin, 2010-10-13, �̵���ߵļ�ʱ������
LRESULT CEXEClientDlg::OnSendBuyPropNew(WPARAM wParam, LPARAM lParam)
{
    BYTE *buffer = (BYTE *)lParam;
    if (NULL != m_ChannelService)
    {
        m_ChannelService->SendData(IPC_MAIN_PROP, IPC_SUB_BUY_PROP_NEW, buffer, wParam);
    }
    // �Ӵ��ڷ�������new������ָ�룬��ʱɾ��
    delete buffer;
    buffer = NULL;
    return LRESULT(0);
}

// PengJiLin, 2011-6-24, ������Ϣ
LRESULT CEXEClientDlg::OnActiveToRoom(WPARAM wParam, LPARAM lParam)
{


		
	if (wParam != WA_INACTIVE && wParam != 4)
	{
		/*CString str;
		str.Format("zhtlog:wParam=%d lParam=%d",wParam,lParam);
		OutputDebugString(str);*/
		BYTE Active = 0;
		m_ChannelService->SendData(IPC_MAIN_USER, IPC_SUB_ACTIVE_TO_ROOM,&Active,sizeof(BYTE));//zht2011-11-21
	}
	if (wParam == 4 )
	{
		BYTE Active = 1;
		m_ChannelService->SendData(IPC_MAIN_USER, IPC_SUB_ACTIVE_TO_ROOM,&Active,sizeof(BYTE));
	}

    return LRESULT(0);
}

// zht, 2011-11-21, ��Ϸ��������Ϣ
LRESULT CEXEClientDlg::OnGameWinMove(WPARAM wParam, LPARAM lParam)
{
	BYTE Active = 1;
    if (NULL != m_ChannelService)
    {
        m_ChannelService->SendData(IPC_MAIN_USER, IPC_SUB_ACTIVE_TO_ROOM,&Active,sizeof(BYTE));//zht2011-11-21
		Sleep(10);
		Active = 0;
		m_ChannelService->SendData(IPC_MAIN_USER, IPC_SUB_ACTIVE_TO_ROOM,&Active,sizeof(BYTE));//zht2011-11-21
    }

    return LRESULT(0);
}

//JianGuankun,2016-6-28,��Ϸ�ı�ֱ�����Ϣ
LRESULT CEXEClientDlg::OnScreenSizeChange(WPARAM wParam, LPARAM lParam)
{
	BYTE ChangeType = 1;
	ChangeType = (lParam == 1) ? 1 : 2;
	if (NULL != m_ChannelService)
	{
		m_ChannelService->SendData(IPC_MAIN_USER, IPC_SUB_SCREEN_SIZE_CHANGE,&ChangeType,sizeof(BYTE));
	}

	return LRESULT(0);
}

// �������͵�����Ϣ
LRESULT CEXEClientDlg::OnSendGiveProp(WPARAM wParam, LPARAM lParam)
{
	// ֱ�Ӽ�����Ϣͷ�������Ӵ��ڷ�������Ϣ���͵�ƽ̨
	BYTE *buffer = (BYTE *)lParam;
	if (NULL != m_ChannelService)
	{
		m_ChannelService->SendData(IPC_MAIN_PROP, IPC_SUB_PRESENT_PROP, buffer, wParam);
		_TAG_PROP_GIVE *pProp = (_TAG_PROP_GIVE *)lParam;
		////DebugPrintf("[%s]��[%s][%d]��[%d]��ֵ[%d]",
		//	pProp->szUser, pProp->szTargetUser, pProp->nGiveCount, pProp->nPropID, pProp->iPropValue);
	}
	// �Ӵ��ڷ�������new������ָ�룬��ʱɾ��
	delete buffer;
	buffer = NULL;

	return LRESULT(0);
}

bool CEXEClientDlg::SetupIPC()
{
	{
		CString timeStr;
		SYSTEMTIME st;
		GetLocalTime(&st);
		timeStr.Format("���յ�������ϢSetupIPC����ǰʱ��=%d-%d-%d %02d:%02d:%02d:%02d",st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute,st.wSecond,st.wMilliseconds);
		//OutputDebugString(timeStr);
	}

	LPCTSTR lpszCmdLine=AfxGetApp()->m_lpCmdLine;

	//�����д���
	if (lpszCmdLine[0]!=0)
	{
		//��� TOKEN
		int nStringLength=0;
		CString strRoomToken;
		LPCTSTR pszRoomToken=TEXT("/RoomToken:");
		LPCTSTR lpszBeginString=lpszCmdLine;

		while (true)
		{
			LPCTSTR lpszEndString=_tcschr(lpszBeginString,TEXT(' '));
			nStringLength=(lpszEndString==NULL)?lstrlen(lpszBeginString):(int)(lpszEndString-lpszBeginString);

			//�жϱ�ʶ
			const int nTokenLength=lstrlen(pszRoomToken);
			if ((nStringLength>=nTokenLength)&&(memcmp(lpszBeginString,pszRoomToken,nTokenLength*sizeof(TCHAR))==0))
			{
				CopyMemory(strRoomToken.GetBufferSetLength(nStringLength-nTokenLength),lpszBeginString+nTokenLength,
					(nStringLength-nTokenLength)*sizeof(TCHAR));//�ڴ�ӳ�������
				strRoomToken.ReleaseBuffer();
				break;
			}

			//���ñ���
			if (lpszEndString==NULL) break;
			lpszBeginString=(lpszEndString+1);
		}//ѭ��2��

		//�����ڴ�
		if (strRoomToken.GetLength()>0)
		{
			m_hShareMemory=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,strRoomToken);

			if (m_hShareMemory==NULL) 
				return false;

			m_pShareMemory=(tagShareMemory *)MapViewOfFile(m_hShareMemory,FILE_MAP_ALL_ACCESS,0,0,0);

			if (m_pShareMemory==NULL) 
				return false;

			if (m_pShareMemory->wDataSize<sizeof(tagShareMemory)) 
				return false;

			m_pShareMemory->hWndGameFrame=m_hWnd;

			TRACE("���ӹ����ڴ� [%s] ��ɡ�",strRoomToken.GetBuffer(0) );
		}

		//�ŵ�ģ��
		if (m_hShareMemory!=NULL)
		{
			ASSERT(m_pShareMemory->hWndGameServer!=NULL);
			m_ChannelService = new CChannelService;

			if (m_ChannelService->SetChannelMessageSink(this)==false) 
				return false;

			if (m_ChannelService->CreateChannel(m_pShareMemory->hWndGameServer)==false) 
				return false;

		}
		//char szMsg[128];
		//sprintf( szMsg, "m_ChannelService->CreateChannel(%x)", m_pShareMemory->hWndGameServer );
		//m_list.AddString(szMsg);

		{
			CString timeStr;
			SYSTEMTIME st;
			GetLocalTime(&st);
			timeStr.Format("���յ�������ϢSetupIPC�����ʱ��=%d-%d-%d %02d:%02d:%02d:%02d",st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute,st.wSecond,st.wMilliseconds);
			//OutputDebugString(timeStr);
		}

		return true;
	}else
		return false;

}

//ͨ����Ϣ
bool CEXEClientDlg::OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	//���⴦��
	//DebugPrintf("ChannelMessage: %d, %d, %X, %d", pHead->wMainCmdID, pHead->wSubCmdID, pIPCBuffer, wDataSize);

    if (NULL!=m_pNewGame)
    {
        return m_pNewGame->OnIPCMessage(pHead, pIPCBuffer, wDataSize, hWndSend);
    }    

	m_dwIPCHeartBeatTick = GetTickCount();
	//Ĭ�ϴ���
	switch (pHead->wMainCmdID)
	{
	case IPC_MAIN_SOCKET:	//�������ݣ�����������Ϸ�������˹�����Ϸ�������Ϣ
		{
			return OnIPCSocket(pHead,pIPCBuffer,wDataSize,hWndSend);
		}
	case IPC_MAIN_CONCTROL:	//������Ϣ
		{            
//            DebugPrintfA("����Ϸ������Ϣ");
			return OnIPCControl(pHead,pIPCBuffer,wDataSize,hWndSend);
		}
	case IPC_MAIN_CONFIG:	//������Ϣ
		{
//            DebugPrintfA("����Ϸ������Ϣ");
			return OnIPCConfig(pHead,pIPCBuffer,wDataSize,hWndSend);
		}
	case IPC_MAIN_USER:		//ԭOnControlMessage����
		{
//            DebugPrintfA("����Ϸ�û���Ϣ");
			return OnIPCUserAct(pHead,pIPCBuffer,wDataSize,hWndSend);
		}
	case IPC_MAIN_PROP:		//��ҵ��߳�ʼ����Ϣ
		{
//            DebugPrintfA("����Ϸ������Ϣ");
			return OnIPCProp(pHead,pIPCBuffer,wDataSize,hWndSend);
		}
	case IPC_USER_PROP_RESULT:
		{
			return this->m_IGameFrame->OnControlMessage(IPC_USER_PROP_RESULT,(void *)pIPCBuffer,wDataSize);
		}
	case IPC_FRIEND:
		{
			if (pHead->wSubCmdID == IPC_ASS_CHECKFRIEND)
			{
				this->m_IGameFrame->OnAddFriend(*((int*)pIPCBuffer) == 1);
			}
			if (pHead->wSubCmdID == IPC_ASS_GETFRIENDLIST)
			{
				this->m_IGameFrame->OnGetFriendList((void *)pIPCBuffer,wDataSize);
			}
			return true;
		}
	case IPC_EMENY:
		{
			if (pHead->wSubCmdID == IPC_ASS_CHECKEMENY)
			{
				this->m_IGameFrame->OnAddEmeny(*((int*)pIPCBuffer) == 1);
			}

			return true;
		}
	case IPC_TROTTING:
		{
			return this->m_IGameFrame->OnControlMessage(IPC_TROTTING,(void *)pIPCBuffer,wDataSize);
			return 0;
		} 

	case IPC_EX_SKIN:
		{
			return this->m_IGameFrame->OnControlMessage(IPC_EX_SKIN,(void *)pIPCBuffer,wDataSize);
		} 
	}

	return false;
}

//����֪ͨ
bool CEXEClientDlg::OnIPCControl(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	ASSERT(pHead->wMainCmdID==IPC_MAIN_CONCTROL);
	switch (pHead->wSubCmdID)
	{
	case IPC_SUB_START_FINISH:		//�������
		{
			return true;
		}
	case IPC_SUB_CLOSE_FRAME:		//�رտ��
		{
			m_ChannelService->SendData(IPC_MAIN_IPC_KERNEL, IPC_SUB_CLOSE_FRAME, NULL, 0);
			//OnBnClickedOk();
			OnBnClickedButton2();
			return true;
		}
	case IPC_SUB_SHOW_MSG_IN_GAME:
		{
			MsgInGameStruct *pMsg = (MsgInGameStruct *)pIPCBuffer;
			// ���ݴ����������ݣ��жϴ�������Ƿ�ӦΪ��
			this->m_IGameFrame->ShowMsgInGame(pMsg->szMsg,
				pMsg->bNewOrSys,
				pMsg->uSize,
				pMsg->bIsFontValid?pMsg->szFontName:NULL,
				pMsg->bIsTitleValid?pMsg->szTitle:NULL);
			return true;
		}
	case IPC_SUB_SHOW_DUDU:
		{
			MsgDuduStruct *pMsg = (MsgDuduStruct *)pIPCBuffer;
			this->m_IGameFrame->ShowMsgDudu(pMsg->szSendName,
				pMsg->szTargetName,
				pMsg->szCharString,
				pMsg->crTextColor,
				pMsg->bShowTime,
				pMsg->uSize,
				pMsg->bIsFontValid?pMsg->szFontName:NULL,
				pMsg->iDuduType);
			return true;
		}
	case IPC_SUB_JOIN_IN_GAME:
		{
			if (NULL != m_IGameFrame)
			{
				//m_IGameFrame->SendWindowMessage(WM_BRING_GAMEWND_TO_TOP,0,0);
				m_IGameFrame->ShowMsgInGame(NULL);
			}
			return true;
		}
	}

	return false;
}
//�������ݣ�����������Ϸ�������˹�����Ϸ�������Ϣ
bool CEXEClientDlg::OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	ASSERT(pHead->wMainCmdID==IPC_MAIN_SOCKET);

	// ������Ϣ������¼ʱ��
	m_dwIPCHeartBeatTick = GetTickCount();
	m_ChannelService->SendData(IPC_MAIN_SOCKET, IPC_SUB_IPC_HEART_BEAT, NULL, 0);

	switch (pHead->wSubCmdID)
	{
	case IPC_SUB_SOCKET_SEND:	//������Ϣ
		{
			//Ч������
			ASSERT(wDataSize>=sizeof(CMD_Command));
			if (wDataSize<sizeof(CMD_Command)) 
			{
				return false;
			}

			//��������
			NetMessageHead * pNetHead=(NetMessageHead *)pIPCBuffer;
			WORD wSendSize=wDataSize-sizeof(NetMessageHead);
			void *pNetData = NULL;
			if (wSendSize >= 0)
			{
				pNetData = (BYTE*)pIPCBuffer + sizeof(NetMessageHead);
			}
			if (NULL != m_IGameFrame)
			{
				m_IGameFrame->OnSocketReadEvent(pNetHead, pNetData, wSendSize);
			}
//            DebugPrintfA("����Ϸ������Ϣ MainID=%d SubID=%d", pNetHead->bMainID, pNetHead->bAssistantID);


			return true;
		}
	case IPC_SUB_IPC_HEART_BEAT:
		{
			// ������Ϣ������¼ʱ��
			m_dwIPCHeartBeatTick = GetTickCount();
			m_ChannelService->SendData(IPC_MAIN_SOCKET, IPC_SUB_IPC_HEART_BEAT, NULL, 0);
			return true;
		}
	}

	return false;
}

// ��ƽ̨��������Ϣ������wDataSize = pIPCBuffer���ݵĳ���
bool CEXEClientDlg::OnIPCConfig(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	ASSERT(pHead->wMainCmdID==IPC_MAIN_CONFIG);
	switch (pHead->wSubCmdID)
	{
	case IPC_SUB_SERVER_INFO:
		{
			// ֱ�Ӿ���m_GameInfo�����ݣ�û�б������
			//��������
			IPC_SocketPackage * pSocketPackage=(IPC_SocketPackage *)pIPCBuffer;
			CMD_Command *pCommand= &pSocketPackage->Command;
			WORD wSendSize=wDataSize;
			if (wSendSize != sizeof(GameInfoStructInExe)) 
			{
				MessageBox("ƽ̨��GameLancher�е�GameInfoStructInExe��һ��");
				return false;
			}
			else 
			{
				memcpy( &m_GameInfo, pIPCBuffer, sizeof(m_GameInfo));
				if (m_GameInfo.pOrderName = NULL)
				{
					m_GameInfo.pOrderName = GetGameOrderNameInExe;
				}
				ShowWindow(SW_HIDE);
				//OutputDebugString("CEXEClientDlg::OnIPCConfig");
				OnBnClickedButton1();
				m_ChannelService->SendData(IPC_MAIN_CONFIG,IPC_SUB_SERVER_INFO,NULL,0);
			}
			return true;
		}
	case IPC_SUB_ENCRYPT_TYPE:
		{
			int *pEncrypt = (int*)pIPCBuffer;
			Glb().m_CenterServerPara.m_nEncryptType = *pEncrypt;
			return true;
		}
	}

	return false;
}
// ��ƽ̨��������Ϣ������pIPCBuffer�е�һ��UINT���ǿ�����UINT uControlCode, void * pControlData, UINT uDataSize
bool CEXEClientDlg::OnIPCUserAct(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	ASSERT(pHead->wMainCmdID==IPC_MAIN_USER);
	if (NULL == m_IGameFrame)
	{
//		DebugPrintfA("���ڴ���û������������Ϣ");
		return false;
	}
	//DebugPrintf("��������");
	switch(pHead->wSubCmdID)
	{
	case IPC_SUB_REPLY_USER_LIST:
		{
			//DebugPrintf("��������IPC_SUB_REPLY_USER_LIST");
			if(wDataSize<sizeof(UserItemStruct)) return false;
			this->m_IGameFrame->OnUserListAtDesk((UserItemStruct *)pIPCBuffer);
			return true;
		}
	case IPC_SUB_USER_STATUS:
	case IPC_SUB_USER_SCORE:		//�û�����
		{
			//DebugPrintf("��������IPC_SUB_USER_STATUS��IPC_SUB_USER_SCORE");
			UINT *pControlCode = (UINT*)pIPCBuffer;
			void *pControlData = (BYTE*)pIPCBuffer + sizeof(UINT);
			//DebugPrintf("m_IGameFrame->OnControlMessageǰ, uControlCode = [%d]", *pControlCode);
			this->m_IGameFrame->OnControlMessage( *pControlCode, pControlData, wDataSize-sizeof(UINT) );
			//DebugPrintf("m_IGameFrame->OnControlMessage��");
			break;
		}
	case IPC_SUB_USER_COME:
		{
			//DebugPrintf("��������IPC_SUB_USER_COME");
			UserItemStruct *pUserInfo = (UserItemStruct *)pIPCBuffer;
			this->m_IGameFrame->ResetUserInfo(pUserInfo);
			break;
		}
	default:
		//DebugPrintf("��������default");
		break;
	}
	return true;
}
// ��ƽ̨������������йص���Ϣ
bool CEXEClientDlg::OnIPCProp(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	ASSERT(pHead->wMainCmdID==IPC_MAIN_PROP);
	switch(pHead->wSubCmdID)
	{
	case IPC_SUB_INIT_PROP_FINISH:
		{
			//DebugPrintf("�յ�IPC_SUB_INIT_PROP_FINISH��Ϣ");
			MsgGlbData *pGD = (MsgGlbData*)pIPCBuffer;
			Glb().m_CenterServerPara.m_nEncryptType = pGD->m_nEncryType;
			Glb().m_NewUrl = pGD->m_NewUrl;
			Glb().m_TML_SN = pGD->m_szToken;
			//DebugPrintf("�յ��ļ��ܷ�ʽ=%d, NewUrl=[%s], Token=[%s]", pGD->m_nEncryType,
			//	pGD->m_NewUrl, pGD->m_szToken);
			//DebugPrintf("���õ�Glb()�м��ܷ�ʽ=%d, NewUrl=[%s], Token=[%s]", Glb().m_CenterServerPara.m_nEncryptType,
			//	Glb().m_NewUrl, Glb().m_TML_SN);
			// ���һ�������Ѿ����꣬�ж��Ƿ�ˢ�½���
			//DebugPrintf("���һ�������Ѿ����꣬�ж��Ƿ�ˢ�½���");
			// ���������������ԭ��������ͬ����ˢ��֮
			int nPropCountNew = m_userPropLibrary.GetCount();
			int nPropCountOld = Glb().userPropLibrary.GetCount();
			// �����Ƿ��и��£�����Ҫ���и���
			for(int i=0; i<nPropCountOld; ++i)
			{
				_TAG_USERPROP *userProp=Glb().userPropLibrary.GetAt(i);
				//Glb().userPropLibrary.RemoveAt(i);
				delete userProp;
				userProp=NULL;
			}
			Glb().userPropLibrary.RemoveAll();
			for (int i=0; i<nPropCountNew; ++i)
			{
				_TAG_USERPROP *userProp=m_userPropLibrary.GetAt(i);
				Glb().userPropLibrary.Add(userProp);
			}
			m_userPropLibrary.RemoveAll();
			// ���������һ�£�����Ҫˢ�½���
            // PengJiLin, 2010-10-13, ÿ�ζ����½��棬����ע�����²��ִ���
			//if (nPropCountOld != nPropCountNew)
			{
				if (m_IGameFrame)
				{
					m_IGameFrame->OnInitPropList();
					//DebugPrintf("ˢ�µ���");
				}
			}
			//else
			//{
			//	m_IGameFrame->OnInitPropList(1);	// ֻ���������������»���
			//}
			//m_bNeedRefleshProp = false;
			//m_nPropCount = 0;
			break;
		}
	case IPC_SUB_INIT_PROP:
		{
			//���ڴ�����
			MSG_PROP_S_GETPROP * msg=(MSG_PROP_S_GETPROP*)pIPCBuffer;
			//_TAG_USERPROP *userProp=NULL;
			//int propCount=Glb().userPropLibrary.GetCount();
			//bool bPropExist=false;
			//for(int i=0;i<propCount;i++)
			//{
			//	userProp=Glb().userPropLibrary.GetAt(i);
			//	if(userProp->nPropID==msg->nPropID)
			//	{
			//		// �ҵ��ˣ�ֱ���޸�����
			//		bPropExist=true;
			//		// ����ʱ��Ҫ�Ƴ�����
			//		if(msg->nHoldCount<1)
			//		{
			//			Glb().userPropLibrary.RemoveAt(i);
			//			delete userProp;
			//			userProp=NULL;
			//			m_bNeedRefleshProp = true;
			//		}
			//		else
			//		{
			//			userProp->nHoldCount=msg->nHoldCount;
			//		}
			//		break;
			//	}
			//}
			//// û�ҵ������������0ʱ���������
			//if(!bPropExist && msg->nHoldCount>0)
			//{
			//	userProp=new _TAG_USERPROP;
			//	memcpy(userProp,msg,sizeof(_TAG_USERPROP));
			//	Glb().userPropLibrary.Add(userProp);
			//	m_bNeedRefleshProp = true;
			//}
			_TAG_USERPROP *userProp=new _TAG_USERPROP;
			memcpy(userProp,msg,sizeof(_TAG_USERPROP));
			m_userPropLibrary.Add(userProp);
//			m_nPropCount += 1;
			return true;
		}
	default:
		break;
	}
	return true;
}
//��������
bool CEXEClientDlg::SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//Ч��״̬
	if (m_ChannelService==NULL) return false;

	//Ч������
	ASSERT(wDataSize<=SOCKET_PACKAGE);
	if (wDataSize>SOCKET_PACKAGE) return false;

	//��������
	IPC_SocketPackage SocketPackage;
	memset(&SocketPackage,0,sizeof(SocketPackage));
	SocketPackage.Command.wMainCmdID=wMainCmdID;
	SocketPackage.Command.wSubCmdID=wSubCmdID;
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketPackage.cbBuffer,pData,wDataSize);
	}

	//��������
	WORD wSendSize=sizeof(SocketPackage.Command)+wDataSize;
	return m_ChannelService->SendData(IPC_MAIN_SOCKET,IPC_SUB_SOCKET_SEND,&SocketPackage,wSendSize);
}

void CEXEClientDlg::OnBnClickedButton1()
{
	// PengJiLin, 2011-6-30, �ٱ���༭����Ҫ�����Ӳ�����ټ���Ϊ2(0���)���粻�ǣ����ͣ����Ϊ2��
    // Ϊ�˼��ٴ��룬�����ڴ˴��жϼ��ټ����Ƕ��٣��¿��ֱ����2���ú������ں��������жϴ���
	try
	{
		int nLength = _tcslen(m_GameInfo.szProcessName);
		m_GameInfo.szProcessName[nLength-3] = TCHAR('i');
		m_GameInfo.szProcessName[nLength-2] = TCHAR('c');
		m_GameInfo.szProcessName[nLength-1] = TCHAR('o');
		char szIcoName[MAX_PATH];
		strcpy(szIcoName, m_GameInfo.szProcessName);
		//sprintf(szIcoName, "%d/%s", m_GameInfo.uNameID, m_GameInfo.szProcessName);
		//������Ϸ����
//		m_hGameInstance=::LoadLibrary("10003300/10003300.ico");
        m_hGameInstance=::LoadLibrary(szIcoName);
		if (m_hGameInstance==NULL) 
			throw TEXT("���ִ���!�Ҳ�����Ϸ!");

        //�ڴ��ж��Ƿ��°���Ϸ
        /// ��ȡ����ָ��
        CREATEGAMEIMPLEMENT CreateGame= (CREATEGAMEIMPLEMENT)GetProcAddress(m_hGameInstance, ("CreateNewGameImpl"));
        if (NULL!=CreateGame && 1)
        {
            //����Ӳ������
            SetAcceleration(2);

            // �¿��
            m_pNewGame = CreateGameEngine();
            EngineInit_t init;
            init.hInstance = m_hGameInstance;
            init.pIPC = m_ChannelService;
            init.pGameInfo = &m_GameInfo;
            if (m_pNewGame->Initial(init)<0)
            {
                OnGameQuit(0, 0);
                return;
            }

            SetTimer(TIMER_DRAW, FRAME_INTERVAL, NULL);
        }
        else
        {
            //������Դ
            AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));

            //Ч����Ϸ����

            //if(m_pRoomInfo->ComRoomInfo.uComType==TY_NORMAL_GAME)
            //{
            SetGameInitClass * pGetOrderFunc=(SetGameInitClass *)GetProcAddress(m_hGameInstance,TEXT("SetGameInitClass"));
            if (pGetOrderFunc!=NULL)
                m_GameInfo.pOrderName=pGetOrderFunc(m_GameInfo.uNameID);
            else
                m_GameInfo.pOrderName=GetNormalOrderName;

            // TODO: �ڴ���ӿؼ�֪ͨ����������
            BeginInitGoGame * pCreateInterface=(BeginInitGoGame *)GetProcAddress(m_hGameInstance,TEXT("BeginInitGoGame"));
            m_IGameFrame = pCreateInterface();
            m_IGameFrame->InitFrame(&m_GameInfo, this);
            m_IGameFrame->CreateFrame();//IDD_EXE_DIALOG, this);
            AfxSetResourceHandle(GetModuleHandle(NULL));
        }

		//֪ͨ������Ϸ��ܼ������
		//Added by JianGuankun 2012.2.7
		m_ChannelService->SendData(IPC_MAIN_IPC_KERNEL,IPC_SUB_GAMEFRAME_INITED,&m_GameInfo.uRoomID,sizeof(m_GameInfo.uRoomID));
	}
	catch (...)
	{
		return;
	}
}

void CEXEClientDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_pLoveSendClassTest = new CLoveSendClassTest();
	//m_pLoveSendClassTest->CreateFrame();//IDD_EXE_DIALOG, this);//�и��ؼ�ûע��
	if(NULL != this->m_IGameFrame)
	{
		//OutputDebugString("[aa]SDK CEXEClientDlg::OnBnClickedButton2");
		m_IGameFrame->AFCCloseFrame(false);
		m_IGameFrame = NULL;
	}
	exit(0);
}

void CEXEClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
    case TIMER_DRAW:
        {
            if (m_pNewGame!=NULL)
            {
                m_pNewGame->DrawAll();
            }
        }
        break;
	case TIMER_HEART_BEAT:
		{
            if (m_pNewGame!=NULL)
            {
                if (!m_pNewGame->OnIPCHeartBeat())
                {
                    KillTimer(TIMER_HEART_BEAT);
                }
                return;
            }
			DWORD dwThisTick = GetTickCount();
			////DebugPrintf("dwThisTick[%d]-m_dwIPCHeartBeatTick[%d]=[%d]", dwThisTick, m_dwIPCHeartBeatTick, dwThisTick - m_dwIPCHeartBeatTick);
			if (NULL==m_ChannelService)
			{
				return;
			}
			if (dwThisTick - m_dwIPCHeartBeatTick > (HEART_BEAT_DELAY+HEART_BEAT_SECOND)*1000) // ������ʱ
			{
				//��ʾ�������Ѿ�ֹͣ��
				OnGameQuit(2,0);
			}
			else
			{
				this->m_ChannelService->SendData(IPC_MAIN_SOCKET, IPC_SUB_IPC_HEART_BEAT, NULL, 0);
			}
			break;
		}
	default:
		break;
	}
}

BOOL CEXEClientDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.dwExStyle |= WS_EX_TOOLWINDOW;

	return __super::PreCreateWindow(cs);
}

//����Ӳ������
void CEXEClientDlg::SetAcceleration(int iLevel)
{	   
	DISPLAY_DEVICE  dv;   
	int i = 0;

	ZeroMemory(&dv,sizeof(DISPLAY_DEVICE));
	dv.cb=sizeof(DISPLAY_DEVICE);
	EnumDisplayDevices(0,0,&dv,0); 		   
	while(dv.DeviceKey[i])
	{
		dv.DeviceKey[i++]=toupper(dv.DeviceKey[i]);
	}

	CString strScr, strDeviceKeyName, strSub;
	strScr = dv.DeviceKey;	
	int iPos = 0, iLen = 0;
	CString sz;	
	sz = "\\SYSTEM";
	iPos = strScr.Find(sz, 0);
	iLen = strScr.GetLength();		
	iPos = iLen - iPos - 1;
	strSub = strScr.Right(iPos);		

	strDeviceKeyName = "Acceleration.Level";		
	HKEY hKey;   
	//�򿪳ɹ���ִ��	
	if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSub, NULL, KEY_ALL_ACCESS, &hKey)) 
	{
		DWORD dValue = 4, dVal = 4, dValData = 0; 
		RegQueryValueEx(hKey, strDeviceKeyName, 0, &dVal,(LPBYTE)&dValData, (LPDWORD)&dValue);		
		//����ԭ����Ӳ�����ټ���
		m_iLevel = dValData;	
		if (m_iLevel <= 0)
			m_iLevel = 0;
		if (m_iLevel >= 5)
			m_iLevel = 5;

        // PengJiLin, 2011-6-30, ���ԭ���ļ�����߻�����ͬ����������
        if(m_iLevel > iLevel)
        {
		    RegSetValueEx(hKey, strDeviceKeyName, 0, REG_DWORD, (BYTE*)&iLevel, sizeof(iLevel));
		    ChangeDisplaySettings(0, 0x40);  //0x40 ��MSDNû���ҵ�ʲô��˼������ֱ����OD���հᡣ
        }
		
		RegCloseKey(hKey);		
	}	   
}

void CEXEClientDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	BYTE Active = 1;
	if (NULL != m_ChannelService)
	{
		m_ChannelService->SendData(IPC_MAIN_USER, IPC_SUB_ACTIVE_TO_ROOM,&Active,sizeof(BYTE));//zht2011-11-21
		Sleep(10);
		Active = 0;
		m_ChannelService->SendData(IPC_MAIN_USER, IPC_SUB_ACTIVE_TO_ROOM,&Active,sizeof(BYTE));//zht2011-11-21
	}
	// TODO: �ڴ˴������Ϣ����������
}
