#include "StdAfx.h"
#include "ClientGameDlg.h"
#include ".\admitdefeat.h"

CAdmitDefeat::CAdmitDefeat():CGameFaceGo(CAdmitDefeat::IDD)
{
	m_pParnet=NULL;
	TCHAR path[MAX_PATH];

	wsprintf(path,TEXT(".\\image\\renshu.bmp"));
	m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
}

CAdmitDefeat::~CAdmitDefeat(void)
{
}

BOOL CAdmitDefeat::OnInitDialog()
{
	CGameFaceGo::OnInitDialog();
	SetWindowPos(&wndBottom,0,0,m_bk.GetWidth(),m_bk.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);
	TCHAR path[MAX_PATH];

	wsprintf(path,TEXT(".\\image\\ok.bmp"));
	m_btOk.LoadButtonBitmap(path,false);

	wsprintf(path,TEXT(".\\image\\cancel.bmp"));
	m_btCancel.LoadButtonBitmap(path,false);
	CRect ContronRect;
	m_btCancel.GetClientRect(&ContronRect);
	if(m_btOk.GetSafeHwnd())
	{
		m_btOk.MoveWindow(50,120,ContronRect.Width(),ContronRect.Height(),false);//
		m_btCancel.MoveWindow(140,120,ContronRect.Width(),ContronRect.Height(),false);//
	}

	return TRUE;
}
BEGIN_MESSAGE_MAP(CAdmitDefeat, CGameFaceGo)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CAdmitDefeat::OnPaint()
{
	CPaintDC dc(this);
	CGameImageHelper handle(&m_bk);
	handle.BitBlt(dc,0,0);
}

void CAdmitDefeat::DoDataExchange(CDataExchange* pDX)
{
	// TODO: �ڴ����ר�ô����/����û���

	CGameFaceGo::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

void CAdmitDefeat::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CGameFaceGo::OnCancel();
	m_pParnet->SetFocus();
}

void CAdmitDefeat::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	
	if(m_pParnet)
		m_pParnet->SendMessage(IDM_SEND_REN_SHU,0,0);
	CGameFaceGo::OnOK();

}

BOOL CAdmitDefeat::ShowWindow(int nCmdShow)
{
	CRect rect,rect2;				
	GetParent()->GetClientRect(&rect);
	GetClientRect(&rect2);

	int Offsetx = (rect.Width()-rect2.Width())/2 - 80;
	int Offsety = (rect.Height()-rect2.Height())/2;

	MoveWindow(Offsetx,Offsety,rect2.Width(),rect2.Height(),FALSE);

	return __super::ShowWindow(nCmdShow);
}
