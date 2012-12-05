#include "Stdafx.h"
#include "resource.h"
#include "queueInRoomDlg.h"
#include "GameRoomEx.h"


BEGIN_MESSAGE_MAP(CQueueInRoomDlg, CLoadFileBmp)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(ID_QUEUE, OnQueue)
	ON_BN_CLICKED(ID_NOQUEUE, OnNoQueue)
	ON_BN_CLICKED(ID_EXIT, OnExit)
	ON_MESSAGE(WM_EXCHANGE_SKIN,OnExchangeSkin)
END_MESSAGE_MAP()

///�Ŷӻ�������ʱ�� ID
#define ID_QUEUE_TIME					888
#define TIME_NOACTION_EXIT				20							//ָ��ʱ����û�н�����Ϸ�������������

///���캯��
CQueueInRoomDlg::CQueueInRoomDlg() : CLoadFileBmp(IDD_QUEUE_ROOM)
{
	m_iIndex = 0;
	::memset(&m_rc, 0, sizeof(m_rc));
	::memset(m_szShowMsg, 0, sizeof(m_szShowMsg));
	m_uRoomID = 0;
}

///��������
CQueueInRoomDlg::~CQueueInRoomDlg()
{
	m_Font.DeleteObject();
}

///DDX/DDV ֧��  ���ݶԻ��������
void CQueueInRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: �ڴ����ר�ô����/����û���
	CLoadFileBmp::DoDataExchange(pDX);
	DDX_Control(pDX, ID_QUEUE,m_BtQueue);
	DDX_Control(pDX, ID_NOQUEUE,m_BtNoQueue);
	DDX_Control(pDX, ID_EXIT,m_BtExit);
}

///����������
bool CQueueInRoomDlg::SetMessageText(const TCHAR *szMessage)
{
	if ((szMessage==NULL)||(GetSafeHwnd()==NULL)) 
		return false;
	
	strncpy_s(m_szShowMsg, _countof(m_szShowMsg), szMessage, _TRUNCATE);

	InvalidateRect(&m_rc, FALSE);
	UpdateWindow();

	return true;
}

///��Ӧ�ŶӰ�ť��Ϣ
void CQueueInRoomDlg::OnQueue()
{
	m_iIndex = 0;
	SetTimer(ID_QUEUE_TIME,500,NULL);
	SetMessageText(_T("ϵͳ��λ�����Ե�"));

	m_BtQueue.EnableWindow(FALSE);
	m_BtQueue.ShowWindow(SW_HIDE);

	m_BtNoQueue.EnableWindow(TRUE);
	m_BtNoQueue.ShowWindow(SW_SHOW);

	InvalidateRect(&m_rc, FALSE);
	UpdateWindow();

	//CGameRoomEx * pGameRoom = GetGameRoom();

	CGameRoomEx* pGameRoom = GetGameRoom(m_uRoomID);
	if (NULL != pGameRoom)
	{
		pGameRoom->SendData(MDM_GR_USER_ACTION,ASS_GR_JOIN_QUEUE,0);
		pGameRoom->KillTimer(TIME_NOACTION_EXIT);
	}

	return;
}

///��Ӧ���ŶӰ�ť��Ϣ
void CQueueInRoomDlg::OnNoQueue()
{
	KillTimer(ID_QUEUE_TIME);
	SetMessageText(_T("���ŶӲ�����Ϸ��"));

	m_BtNoQueue.EnableWindow(FALSE);
	m_BtNoQueue.ShowWindow(SW_HIDE);

	m_BtQueue.EnableWindow(TRUE);
	m_BtQueue.ShowWindow(SW_SHOW);

	InvalidateRect(&m_rc, FALSE);
	UpdateWindow();

	//CGameRoomEx * pGameRoom = GetGameRoom();

	CGameRoomEx* pGameRoom = GetGameRoom(m_uRoomID);
	if (NULL != pGameRoom)
	{
		pGameRoom->SendData(MDM_GR_USER_ACTION,ASS_GR_QUIT_QUEUE,0);
		if(pGameRoom->m_GameInfo.uNoActionKickTimer > 0)
		{
			pGameRoom->SetTimer(TIME_NOACTION_EXIT, 1000*60*pGameRoom->m_GameInfo.uNoActionKickTimer, NULL);
		}
	}

	return;
}

///�˳�����
void CQueueInRoomDlg::OnExit()
{
	//CGameRoomEx * pGameRoom=GetGameRoom();

	CGameRoomEx* pGameRoom = GetGameRoom(m_uRoomID);
	if (NULL != pGameRoom)
	{
		KillTimer(ID_QUEUE_TIME);
		DestroyWindow();

		pGameRoom->OnCancelConnectEvent();
	}
	
	return;
}

///���ٶԻ���
void CQueueInRoomDlg::DestroyDlg()
{
	KillTimer(ID_QUEUE_TIME);
	DestroyWindow();

	return;
}

///��ʼ������
BOOL CQueueInRoomDlg::OnInitDialog()
{
	CLoadFileBmp::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString s=CBcfFile::GetAppPath();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);

	///��ȡ�Ŷӻ��Ի�������Ϣ����ʾλ��
	m_rc.left = f.GetKeyVal("queueRoom", "queueX", 40);
	m_rc.top = f.GetKeyVal("queueRoom", "queueY", 72);
	m_rc.right = m_rc.left + f.GetKeyVal("queueRoom", "queueW", 190);
	m_rc.bottom = m_rc.top + f.GetKeyVal("queueRoom", "queueH", 20);

	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());
	
	wsprintf(path,"%slogon\\queue.bmp",skinfolder);
	m_bk.SetLoadInfo(path,TRUE);
	CenterWindow();
	CGameImageHelper help(&m_bk);
	HRGN hRgn;
	hRgn = AFCBmpToRgn(help,m_bk.GetPixel(0,0),RGB(1,1,1));
	if(hRgn)
		SetWindowRgn(hRgn,TRUE);

	///�ŶӰ�ť
	CGameImage		m_bt;
	wsprintf(path,"%slogon\\waiting.bmp",skinfolder);
	m_BtQueue.LoadButtonBitmap(path,false);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper bthelp(&m_bt);
	HRGN hRgn2;
	hRgn2=AFCBmpToRgn(bthelp,m_bt.GetPixel(0,0),RGB(1,1,1));
	if(hRgn2)
		m_BtQueue.SetWindowRgn(hRgn2,true);

	///���ŶӰ�ť
	wsprintf(path,"%slogon\\notwaiting.bmp",skinfolder);
	m_BtNoQueue.LoadButtonBitmap(path,false);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	hRgn2=AFCBmpToRgn(bthelp,m_bt.GetPixel(0,0),RGB(1,1,1));
	if(hRgn2)
		m_BtNoQueue.SetWindowRgn(hRgn2,true);

	///�˳���ť
	wsprintf(path,"%slogon\\button.bmp",skinfolder);
	m_BtExit.LoadButtonBitmap(path,false);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	hRgn2=AFCBmpToRgn(bthelp,m_bt.GetPixel(0,0),RGB(1,1,1));
	if(hRgn2)
		m_BtExit.SetWindowRgn(hRgn2,true);

	m_MsgColor=RGB(255,255,0);
	//SetTimer(ID_QUEUE_TIME,500,NULL);

	m_BtNoQueue.EnableWindow(FALSE);
	m_BtNoQueue.ShowWindow(SW_HIDE);

	m_BtQueue.EnableWindow(TRUE);
	m_BtQueue.ShowWindow(SW_SHOW);

	if(!m_Font.m_hObject)
		m_Font.CreateFont(18,0,0,0,FW_BOLD,0,0,0,134,3,2,1,2,TEXT("����"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CQueueInRoomDlg::ChangeSkin()
{
	CString s=CBcfFile::GetAppPath();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);

	///��ȡ�Ŷӻ��Ի�������Ϣ����ʾλ��
	m_rc.left = f.GetKeyVal("queueRoom", "queueX", 40);
	m_rc.top = f.GetKeyVal("queueRoom", "queueY", 72);
	m_rc.right = m_rc.left + f.GetKeyVal("queueRoom", "queueW", 190);
	m_rc.bottom = m_rc.top + f.GetKeyVal("queueRoom", "queueH", 20);

	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());

	wsprintf(path,"%slogon\\queue.bmp",skinfolder);
	m_bk.SetLoadInfo(path,TRUE);
	CenterWindow();
	CGameImageHelper help(&m_bk);
	HRGN hRgn;
	hRgn = AFCBmpToRgn(help,m_bk.GetPixel(0,0),RGB(1,1,1));
	if(hRgn)
		SetWindowRgn(hRgn,TRUE);

	///�ŶӰ�ť
	CGameImage		m_bt;
	wsprintf(path,"%slogon\\waiting.bmp",skinfolder);
	m_BtQueue.LoadButtonBitmap(path,false);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper bthelp(&m_bt);
	HRGN hRgn2;
	hRgn2=AFCBmpToRgn(bthelp,m_bt.GetPixel(0,0),RGB(1,1,1));
	if(hRgn2)
		m_BtQueue.SetWindowRgn(hRgn2,true);

	///���ŶӰ�ť
	wsprintf(path,"%slogon\\notwaiting.bmp",skinfolder);
	m_BtNoQueue.LoadButtonBitmap(path,false);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	hRgn2=AFCBmpToRgn(bthelp,m_bt.GetPixel(0,0),RGB(1,1,1));
	if(hRgn2)
		m_BtNoQueue.SetWindowRgn(hRgn2,true);

	///�˳���ť
	wsprintf(path,"%slogon\\button.bmp",skinfolder);
	m_BtExit.LoadButtonBitmap(path,false);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	hRgn2=AFCBmpToRgn(bthelp,m_bt.GetPixel(0,0),RGB(1,1,1));
	if(hRgn2)
		m_BtExit.SetWindowRgn(hRgn2,true);

}
///��ʱ����Ϣ
void CQueueInRoomDlg::OnTimer(UINT nIDEvent)
{
	if(m_BtQueue.IsWindowVisible())
	{
		return;
	}

	if (nIDEvent == ID_QUEUE_TIME && m_BtNoQueue.IsWindowVisible())
	{
		++m_iIndex;
		if (m_iIndex >= 5)
		{
			m_iIndex = 1;
		}

		switch (m_iIndex)
		{
		case 1:
			{
				SetMessageText(_T("ϵͳ��λ�����Ե�"));
				break;
			}
		case 2:
			{
				SetMessageText(_T("ϵͳ��λ�����Եȡ�"));
				break;
			}
		case 3:
			{
				SetMessageText(_T("ϵͳ��λ�����Եȡ���"));
				break;
			}
		case 4:
			{
				SetMessageText(_T("ϵͳ��λ�����Եȡ�����"));
				break;
			}
		default:
			break;
		}
		
	}

	//CGameFaceGo::OnTimer(nIDEvent);

	return;
}

void CQueueInRoomDlg::OnPaint()
{
	CPaintDC dc(this); 

	CRect ClientRect;
	GetClientRect(&ClientRect);

	CDC tempCDC;
	tempCDC.CreateCompatibleDC(&dc);

	CBitmap	BufferBmp;
	int width = ClientRect.Width();
	int height = ClientRect.Height();
	BufferBmp.CreateCompatibleBitmap(&dc,width,height);
	CBitmap* pOldBitmap = tempCDC.SelectObject(&BufferBmp);

	CGameImageHelper	help(&m_bk);
	help.BitBlt(tempCDC.GetSafeHdc(),0,0);

	tempCDC.SelectObject(&m_Font);
	tempCDC.SetBkMode(TRANSPARENT);
	tempCDC.SetTextColor(m_MsgColor);
	tempCDC.TextOut(m_rc.left, m_rc.top, m_szShowMsg);
		
	dc.BitBlt(0,0,width,height,&tempCDC,0,0,SRCCOPY);

	tempCDC.SelectObject(pOldBitmap);
	BufferBmp.DeleteObject();
	tempCDC.DeleteDC();

	return;
}

void CQueueInRoomDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MOVE || nID==0xF012)
	return;
	else
	CDialog::OnSysCommand(nID, lParam);
}

//����
LRESULT CQueueInRoomDlg::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	ChangeSkin();
	//OnPaint();
	Invalidate();
	return LRESULT(0);
}
