#include "Stdafx.h"
#include "PlaySetDlg.h"
#include "ClientGameDlg.h"
#include "Resource.h"


BEGIN_MESSAGE_MAP(CPlaySetDlg, CCoolView)
END_MESSAGE_MAP()

//���캯��
CPlaySetDlg::CPlaySetDlg(CWnd * pParent) : CCoolView(CPlaySetDlg::IDD,pParent)
{
	m_pPP = pParent;
	m_GameSetInfo.iJuTimeMinute = 30;
	m_GameSetInfo.iJuTimeSecond = 15;
	m_GameSetInfo.iBuTimeMinute = 3;
	m_GameSetInfo.iBuTimeSecond = 15;
	m_GameSetInfo.iDuMiaoMinute = 5;
	m_GameSetInfo.iDuMiaoSecond = 25;
}

//��������
CPlaySetDlg::~CPlaySetDlg()
{
}

// DDX/DDV ֧��
void CPlaySetDlg::DoDataExchange(CDataExchange* pDX)
{
	CCoolView::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//��ʼ������
BOOL CPlaySetDlg::OnInitDialog()
{
	CCoolView::OnInitDialog();
	AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
	TCHAR szPath[MAX_PATH];
	wsprintf(szPath,".\\image\\%s\\set_play.bmp",SKIN_FOLDER);
	LoadDialogBitmap(szPath,RGB(255,0,255),RGB(1,1,1));
	
	wsprintf(szPath,".\\image\\%s\\set_ok.bmp",SKIN_FOLDER);
	m_btOk.LoadButtonBitmap(szPath,false);
	wsprintf(szPath,".\\image\\%s\\set_cancel.bmp",SKIN_FOLDER);
	m_btCancel.LoadButtonBitmap(szPath,false);
	//���ÿؼ���ʼ��
	SetDlgItemInt(IDC_EDIT_JU_TIME_MINUTE,10);
	SetDlgItemInt(IDC_EDIT_JU_TIME_SECOND,30);
	SetDlgItemInt(IDC_EDIT_BU_TIME_MINUTE,1);
	SetDlgItemInt(IDC_EDIT_BU_TIME_SECOND,30);
	SetDlgItemInt(IDC_EDIT_DU_MIAO_MINUTE,1);
	SetDlgItemInt(IDC_EDIT_DU_MIAO_SECOND,30);

	CenterWindow(m_pPP);

	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_JU_TIME_MINUTE);
	if (pEdit!=NULL)
		pEdit->SetLimitText(2);

	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_JU_TIME_SECOND);
	if (pEdit!=NULL)
		pEdit->SetLimitText(2);

	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_BU_TIME_MINUTE);
	if (pEdit!=NULL)
		pEdit->SetLimitText(2);

	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_BU_TIME_SECOND);
	if (pEdit!=NULL)
		pEdit->SetLimitText(2);

	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_DU_MIAO_MINUTE);
	if (pEdit!=NULL)
		pEdit->SetLimitText(2);

	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_DU_MIAO_SECOND);
	if (pEdit!=NULL)
		pEdit->SetLimitText(2);
	
	AfxSetResourceHandle(GetModuleHandle(NULL));
	return TRUE;
}

//ȡ����ť
void CPlaySetDlg::OnCancel()
{
	CClientPlayView *p=CONTAINING_RECORD(this,CClientPlayView,m_GameSet);
	CClientGameDlg *pp=CONTAINING_RECORD(p,CClientGameDlg,m_PlayView);
	CCoolView::OnClose();
	::PostMessage(pp->GetSafeHwnd(),WM_CLOSE,0,0); 
}

//ȷ����ť
void CPlaySetDlg::OnOK()
{
	//��ȡ����
	BOOL bJTMTrancelate,bJTSTrancelate,bBTMTrancelate,bBTSTrancelate,bDMMTrancelate,bDMSTrancelate;
	bJTMTrancelate = bJTSTrancelate = bBTMTrancelate = bBTSTrancelate = bDMMTrancelate = bDMSTrancelate =FALSE;

	m_GameSetInfo.iJuTimeMinute = GetDlgItemInt(IDC_EDIT_JU_TIME_MINUTE,&bJTMTrancelate);
	m_GameSetInfo.iJuTimeSecond = GetDlgItemInt(IDC_EDIT_JU_TIME_SECOND,&bJTSTrancelate);
	m_GameSetInfo.iBuTimeMinute = GetDlgItemInt(IDC_EDIT_BU_TIME_MINUTE,&bBTMTrancelate);
	m_GameSetInfo.iBuTimeSecond = GetDlgItemInt(IDC_EDIT_BU_TIME_SECOND,&bBTSTrancelate);
	m_GameSetInfo.iDuMiaoMinute = GetDlgItemInt(IDC_EDIT_DU_MIAO_MINUTE,&bDMMTrancelate);
	m_GameSetInfo.iDuMiaoSecond = GetDlgItemInt(IDC_EDIT_DU_MIAO_SECOND,&bDMSTrancelate);

	//Ч������
	if (bJTMTrancelate==FALSE || bJTSTrancelate == FALSE || (m_GameSetInfo.iJuTimeMinute == 0 && m_GameSetInfo.iJuTimeSecond == 0))
	{
		MessageBox(TEXT(" ��������Ϸ�ľ���ʱʱ�䣡"),"��ʾ",MB_ICONQUESTION);
		GetDlgItem(IDC_EDIT_JU_TIME_SECOND)->SetFocus();
		return;
	}
	if (bBTMTrancelate==FALSE || bBTSTrancelate == FALSE || (m_GameSetInfo.iBuTimeMinute == 0 && m_GameSetInfo.iBuTimeSecond == 0))
	{
		MessageBox(TEXT("��������Ϸ��ÿ����ʱʱ�䣡"),"��ʾ",MB_ICONQUESTION);
		GetDlgItem(IDC_EDIT_BU_TIME_SECOND)->SetFocus();
		return;
	}
	if (bBTMTrancelate==FALSE || bBTSTrancelate == FALSE || (m_GameSetInfo.iDuMiaoMinute == 0 && m_GameSetInfo.iDuMiaoSecond == 0))
	{
		MessageBox(TEXT("��������Ϸ��ÿ����ʱʱ�䣡"),"��ʾ",MB_ICONQUESTION);
		GetDlgItem(IDC_EDIT_DU_MIAO_MINUTE)->SetFocus();
		return;
	}
	if(m_GameSetInfo.iJuTimeMinute<0||m_GameSetInfo.iJuTimeMinute>999)
	{
		MessageBox("�����ú��ʵ�ʱ��","��ʾ",0);
		return ;
	}
	if(m_GameSetInfo.iJuTimeSecond<0||m_GameSetInfo.iJuTimeSecond>59)
	{
		MessageBox("����ȷ����ʱ��","��ʾ",0);
		return ;
	}
	if(m_GameSetInfo.iBuTimeMinute<0||m_GameSetInfo.iBuTimeMinute>999)
	{
		MessageBox("�����ú��ʵ�ʱ��","��ʾ",0);
		return ;
	}
	if(m_GameSetInfo.iBuTimeSecond<0||m_GameSetInfo.iBuTimeSecond>59)
	{
		MessageBox("����ȷ����ʱ��","��ʾ",0);
		return ;
	}
	if(m_GameSetInfo.iDuMiaoMinute<0||m_GameSetInfo.iDuMiaoMinute>999)
	{
		MessageBox("�����ú��ʵ�ʱ��","��ʾ",0);
		return ;
	}
	if(m_GameSetInfo.iDuMiaoSecond<0||m_GameSetInfo.iDuMiaoSecond>59)
	{
		MessageBox("����ȷ����ʱ��","��ʾ",0);
		return ;
	}
	if((m_GameSetInfo.iJuTimeMinute==10 && m_GameSetInfo.iJuTimeSecond<30) || m_GameSetInfo.iJuTimeMinute<10)
	{
		MessageBox("��ʱ��������10��30��.","��ʾ",0);
		return ;
	}
	if((m_GameSetInfo.iBuTimeMinute==1 && m_GameSetInfo.iBuTimeSecond<30) || m_GameSetInfo.iBuTimeMinute<1)
	{
		MessageBox("��ʱ��������1��30��.","��ʾ",0);
		return ;
	}
	if((m_GameSetInfo.iDuMiaoMinute==1 && m_GameSetInfo.iDuMiaoSecond<30) || m_GameSetInfo.iDuMiaoMinute<1)
	{
		MessageBox("���벻������1��30��.","��ʾ",0);
		return ;
	}

	//������Ϣ
	CClientPlayView *p=CONTAINING_RECORD(this,CClientPlayView,m_GameSet);
	CClientGameDlg *pp=CONTAINING_RECORD(p,CClientGameDlg,m_PlayView);
	pp->PostMessage(IDM_BEGIN,0,0);
	CCoolView::OnClose();
	return;
}

//������Ϸ����
void CPlaySetDlg::SetGameSetInfo(int iCardCount)
{
	//���ñ���
	if (GetSafeHwnd()==NULL) 
		Create(IDD_SET_PLAY,m_pPP);

	CRect rect;
	m_pPP->GetClientRect(rect);
	int cx=rect.Width();
	int cy=rect.Height();
	GetClientRect(rect);
	MoveWindow((cx - rect.Width())/2,(cy-rect.Height())/2,rect.Width(),rect.Height());

	ShowWindow(SW_SHOW);

	return;
}

//��ȡ��Ϸ����
tagGameSetInfo * CPlaySetDlg::GetGameSetInfo()
{
	return &m_GameSetInfo;
}


LRESULT CPlaySetDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	//case WM_CTLCOLORSCROLLBAR :
	//case WM_CTLCOLORBTN:
	//case WM_CTLCOLORMSGBOX:
	//case WM_CTLCOLOREDIT:
	//case WM_CTLCOLORLISTBOX:
	//case WM_CTLCOLORDLG:
	case WM_CTLCOLORSTATIC :
		SetBkMode((HDC)wParam,TRANSPARENT);
		HBRUSH brush = (HBRUSH)::GetStockObject(NULL_BRUSH);
		return (LRESULT)brush;
	}
	return CCoolView::WindowProc(message, wParam, lParam);
}
