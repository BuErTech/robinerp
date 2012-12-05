#include "Stdafx.h"
#include "PlaySetDlg.h"
#include "ClientGameDlg.h"
#include "Resource.h"


BEGIN_MESSAGE_MAP(CPlaySetDlg, CCoolView)
END_MESSAGE_MAP()

/**
 * ���캯��
 */
CPlaySetDlg::CPlaySetDlg(CWnd * pParent) : CCoolView(CPlaySetDlg::IDD,pParent)
{
	m_pPP=pParent;
//	m_GameSetInfo.iBeginStation=2;
//	m_GameSetInfo.iEndStation=14;
	m_GameSetInfo.iPlayCount=30;		//˼��ʱ��
}

/**
 * ��������
 */
CPlaySetDlg::~CPlaySetDlg()
{
}

/**
 * DDX/DDV ֧��
 */
void CPlaySetDlg::DoDataExchange(CDataExchange* pDX)
{
	CCoolView::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_SPIN, m_SpinCtrl);
}

/**
 * ��ʼ������
 */
BOOL CPlaySetDlg::OnInitDialog()
{
	CCoolView::OnInitDialog();
	AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
	//LoadDialogBitmap(GetModuleHandle(CLIENT_DLL_NAME),IDB_MODE_BMP,RGB(255,0,255),RGB(1,1,1));
	AfxSetResourceHandle(GetModuleHandle(NULL));
	
	//���ÿؼ�
	((CEdit *)GetDlgItem(IDC_COUNT))->LimitText(2);
	m_SpinCtrl.SetBuddy(GetDlgItem(IDC_COUNT));
	m_SpinCtrl.SetRange(10,60);

	return TRUE;
}

/**
 * ȡ����ť
 */
void CPlaySetDlg::OnCancel()
{
	CClientPlayView *p=CONTAINING_RECORD(this,CClientPlayView,m_GameSet);
	CClientGameDlg *pp=CONTAINING_RECORD(p,CClientGameDlg,m_PlayView);
	CCoolView::OnClose();
	::PostMessage(pp->GetSafeHwnd(),WM_CLOSE,0,0); 
}

/**
 * ȷ����ť
 */
void CPlaySetDlg::OnOK()
{
	//��ȡ����
	BOOL bTrancelate=FALSE;
//	m_GameSetInfo.iBeginStation=((CComboBox *)GetDlgItem(IDC_BEGIN_STATION))->GetCurSel()+2;
//	m_GameSetInfo.iEndStation=((CComboBox *)GetDlgItem(IDC_END_STATION))->GetCurSel()+2;
	m_GameSetInfo.iCardCount=1;//(((CButton *)GetDlgItem(IDC_TWO))->GetCheck()==BST_CHECKED)?1:2;
	m_GameSetInfo.iPlayCount=GetDlgItemInt(IDC_COUNT,&bTrancelate);

	//Ч������
	if (bTrancelate==FALSE || m_GameSetInfo.iPlayCount==0)
	{
		AfxMessageBox(TEXT(" ��������Ϸ��˼��ʱ�䣨10-60����"),MB_ICONQUESTION);
		GetDlgItem(IDC_COUNT)->SetFocus();
		return;
	}
/*	if (m_GameSetInfo.iBeginStation>m_GameSetInfo.iEndStation)
	{
		AfxMessageBox(TEXT(" ����ļ������ܱȽ����ļ�����"),MB_ICONQUESTION);
		GetDlgItem(IDC_BEGIN_STATION)->SetFocus();
		return;
	}*/

	//������Ϣ
	CClientPlayView *p=CONTAINING_RECORD(this,CClientPlayView,m_GameSet);
	CClientGameDlg *pp=CONTAINING_RECORD(p,CClientGameDlg,m_PlayView);
	pp->PostMessage(IDM_BEGIN,0,0);
	CCoolView::OnClose();
	return;
}

/**
 * ������Ϸ����
 */
void CPlaySetDlg::SetGameSetInfo(int iCardCount)
{
	//���ñ���
	m_GameSetInfo.iCardCount=iCardCount;
	if (GetSafeHwnd()==NULL) 
		Create(IDD_SET_PLAY,m_pPP);

	//���ÿؼ�
//	GetDlgItem(IDC_TWO)->EnableWindow(iCardCount&1);
//	GetDlgItem(IDC_THREE)->EnableWindow(iCardCount&2);
//	if (iCardCount&1) ((CButton *)GetDlgItem(IDC_TWO))->SetCheck(BST_CHECKED);
	//else if (iCardCount&2) ((CButton *)GetDlgItem(IDC_THREE))->SetCheck(BST_CHECKED);
	SetDlgItemInt(IDC_COUNT,m_GameSetInfo.iPlayCount);
//	((CComboBox *)GetDlgItem(IDC_BEGIN_STATION))->SetCurSel(m_GameSetInfo.iBeginStation-2);
	//((CComboBox *)GetDlgItem(IDC_END_STATION))->SetCurSel(m_GameSetInfo.iEndStation-2);

	CRect rect;
	m_pPP->GetClientRect(rect);
	int cx=rect.Width();
	int cy=rect.Height();
	GetClientRect(rect);
	MoveWindow((cx-rect.Width())/2,(cy-rect.Height())/2,rect.Width(),rect.Height());

	ShowWindow(SW_SHOW);

	return;
}

/**
 * ��ȡ��Ϸ����
 */
tagGameSetInfo * CPlaySetDlg::GetGameSetInfo()
{
//	if (GetSafeHwnd()) return NULL;
	return &m_GameSetInfo;
}
