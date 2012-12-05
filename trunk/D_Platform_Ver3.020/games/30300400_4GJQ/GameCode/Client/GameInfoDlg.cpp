#include "Stdafx.h"
#include "GameInfoDlg.h"
#include "ClientGameDlg.h"
#include ".\gameinfodlg.h"


BEGIN_MESSAGE_MAP(CGameInfoDlg, CCoolView)
END_MESSAGE_MAP()

/**
 * ���캯��
 */
CGameInfoDlg::CGameInfoDlg(CWnd * pParent) : CCoolView(CGameInfoDlg::IDD,pParent)
{
	m_pPP=pParent;
}

/**
 * ��������
 */
CGameInfoDlg::~CGameInfoDlg()
{
}

/**
 * DDX/DDV ֧��
 */
void CGameInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CCoolView::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

/**
 * ȡ����ť
 */
void CGameInfoDlg::OnCancel()
{
	CClientPlayView *p=CONTAINING_RECORD(this,CClientPlayView,m_GameInfo);
	CClientGameDlg *pp=CONTAINING_RECORD(p,CClientGameDlg,m_PlayView);
	::PostMessage(pp->GetSafeHwnd(),WM_CLOSE,0,0); 
	CCoolView::OnClose();
}

/**
 * ȷ����ť
 */
void CGameInfoDlg::OnOK()
{
	CClientPlayView *p=CONTAINING_RECORD(this,CClientPlayView,m_GameInfo);
	CClientGameDlg *pp=CONTAINING_RECORD(p,CClientGameDlg,m_PlayView);
	pp->PostMessage(IDM_BEGIN,0,0);
	CCoolView::OnClose();
	return;
}

/**
 * ������Ϸ����
 */
void CGameInfoDlg::SetGameSetInfo(int iCardCount, int iPlayCount, int iBeginStation, int iEndStation)
{
	if (GetSafeHwnd()==NULL) Create(IDD_GAME_INFO,m_pPP);
	
	//������Ϣ
//	TCHAR szBuffer[100];//,* szCard[]={"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
	SetDlgItemInt(IDC_COUNT,iPlayCount);
	//sprintf(szBuffer,TEXT("%d ���˿�"),iCardCount);//,szCard[iBeginStation-2],szCard[iEndStation-2]);
///	GetDlgItem(IDC_MESSAGE)->SetWindowText(szBuffer);

	CRect rect;
	m_pPP->GetClientRect(rect);
	int cx=rect.Width();
	int cy=rect.Height();
	GetClientRect(rect);
	MoveWindow((cx-rect.Width())/2,(cy-rect.Height())/2,rect.Width(),rect.Height());

	//��ʾ����
	ShowWindow(SW_SHOW);

	return;
}

BOOL CGameInfoDlg::OnInitDialog()
{
	CCoolView::OnInitDialog();

	AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
	//LoadDialogBitmap(GetModuleHandle(CLIENT_DLL_NAME),IDB_MODE_BMP,RGB(255,0,255),RGB(1,1,1));
	AfxSetResourceHandle(GetModuleHandle(NULL));

	return TRUE;
}
