#include "stdafx.h"
#include "StopMessage.h"
#include "ClientGameDlg.h"
#include ".\stopmessage.h"


BEGIN_MESSAGE_MAP(CStopMessage, CGameFaceGo)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/**
 * ���캯��
 */
CStopMessage::CStopMessage() : CGameFaceGo(CStopMessage::IDD)
{
	m_pParent=NULL;
	TCHAR path[MAX_PATH];

	wsprintf(path,TEXT(".\\image\\stopmsg.bmp"));
	m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	
}

/**
 * ��������
 */
CStopMessage::~CStopMessage()
{
}

/**
 * DDX/DDV ֧��
 */
void CStopMessage::DoDataExchange(CDataExchange* pDX)
{
	CGameFaceGo::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

/**
 * ��ʼ������
 */
BOOL CStopMessage::OnInitDialog()
{
	CGameFaceGo::OnInitDialog();
	SetWindowPos(&wndBottom,0,0,m_bk.GetWidth(),m_bk.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);
	TCHAR path[MAX_PATH];

	wsprintf(path,TEXT(".\\image\\agree_bt.bmp"));
	m_btOk.LoadButtonBitmap(path,false);

	wsprintf(path,TEXT(".\\image\\oppose.bmp"));
	m_btCancel.LoadButtonBitmap(path,false);
	CRect ContronRect;
	m_btCancel.GetClientRect(&ContronRect);
	if(m_btOk.GetSafeHwnd())
	{
		m_btOk.MoveWindow(54,120,ContronRect.Width(),ContronRect.Height(),false);//
		m_btCancel.MoveWindow(143,120,ContronRect.Width(),ContronRect.Height(),false);//
	}
	return TRUE;
}

/**
 * ȡ������
 */
void CStopMessage::OnCancel()
{
	if(m_pParent)
		m_pParent->SendMessage(IDM_ARGEE_STOP,0,0);
	CGameFaceGo::OnCancel();
}

/**
 * ȷ������
 */
void CStopMessage::OnOK()
{
	if(m_pParent)
		m_pParent->SendMessage(IDM_ARGEE_STOP,1,1);
	CGameFaceGo::OnOK();
}

void CStopMessage::OnPaint()
{
	CPaintDC dc(this);
	CGameImageHelper handle(&m_bk);
	handle.BitBlt(dc,0,0);
}

BOOL CStopMessage::ShowWindow(int nCmdShow)
{
	CRect rect,rect2;				
	GetParent()->GetClientRect(&rect);
	GetClientRect(&rect2);

	int Offsetx = (rect.Width()-rect2.Width())/2 - 80;
	int Offsety = (rect.Height()-rect2.Height())/2;

	MoveWindow(Offsetx,Offsety,rect2.Width(),rect2.Height(),FALSE);

	return __super::ShowWindow(nCmdShow);
}

