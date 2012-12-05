#include "Stdafx.h"
#include "PlaySetDlg.h"
#include "ClientGameDlg.h"
#include "Resource.h"


BEGIN_MESSAGE_MAP(CPlaySetDlg, CCoolView)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/**
 * ���캯��
 */
CPlaySetDlg::CPlaySetDlg(CWnd * pParent) : CCoolView(CPlaySetDlg::IDD,pParent)
{	
	m_pPP=pParent;
	m_GameSetInfo.iJuTimeMinute = 10;
	m_GameSetInfo.iJuTimeSecond = 30;
	m_GameSetInfo.iBuTimeMinute = 1;
	m_GameSetInfo.iBuTimeSecond = 30;
	m_GameSetInfo.iDuMiaoMinute = 1;
	m_GameSetInfo.iDuMiaoSecond = 30;
	m_bRequireCoachMoeny = 0;
	TCHAR path[MAX_PATH];

	wsprintf(path,TEXT(".\\image\\set_bk.bmp"));
	m_bk.SetLoadInfo(path,true);
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
	//DDX_Control(pDX, IDC_SPIN, m_SpinCtrl);
}

void CPlaySetDlg::AutoSize()
{
	MoveWindow(0,0,m_bk.GetWidth(),m_bk.GetHeight());	
}

/**
 * ��ʼ������
 */
BOOL CPlaySetDlg::OnInitDialog()
{
	CCoolView::OnInitDialog();

	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
	TCHAR path[MAX_PATH];	

	wsprintf(path,TEXT(".\\image\\set_bk.bmp"));
	LoadDialogBitmap(path,RGB(255,0,255),RGB(1,1,1));
	//AfxSetResourceHandle(GetModuleHandle(NULL));

	wsprintf(path,TEXT(".\\image\\ok.bmp"));
	m_btOk.LoadButtonBitmap(path,false);

	wsprintf(path,TEXT(".\\image\\cancel.bmp"));
	m_btCancel.LoadButtonBitmap(path,false);

	wsprintf(path,TEXT(".\\image\\set_bk.bmp"));
	m_bk.SetLoadInfo(path,true);

	m_GameSetInfo.iJuTimeMinute = 10;
	m_GameSetInfo.iJuTimeSecond = 30;
	m_GameSetInfo.iBuTimeMinute = 1;
	m_GameSetInfo.iBuTimeSecond = 30;
	m_GameSetInfo.iDuMiaoMinute = 1;
	m_GameSetInfo.iDuMiaoSecond = 30;
	m_GameSetInfo.iGameMoney = 0;

	//���ÿؼ�
	//���ÿؼ���ʼ��
	SetDlgItemInt(IDC_EDIT_JU_TIME_MINUTE,m_GameSetInfo.iJuTimeMinute);
	SetDlgItemInt(IDC_EDIT_JU_TIME_SECOND,m_GameSetInfo.iJuTimeSecond);
	SetDlgItemInt(IDC_EDIT_BU_TIME_MINUTE,m_GameSetInfo.iBuTimeMinute);
	SetDlgItemInt(IDC_EDIT_BU_TIME_SECOND,m_GameSetInfo.iBuTimeSecond);
	SetDlgItemInt(IDC_EDIT_DU_MIAO_MINUTE,m_GameSetInfo.iDuMiaoMinute);
	SetDlgItemInt(IDC_EDIT_DU_MIAO_SECOND,m_GameSetInfo.iDuMiaoSecond);
	SetDlgItemInt(IDC_EDIT_MONEY,m_GameSetInfo.iGameMoney);

	//CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATIC_1);
	//if(NULL != pStatic)
	//{
	//	//pStatic->set
	//}

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

	pEdit = (CEdit *)GetDlgItem(IDC_EDIT_MONEY);
	if (pEdit!=NULL)
		pEdit->SetLimitText(10);

	if (false == m_bRequireCoachMoeny)
	{
		pEdit = (CEdit *)GetDlgItem(IDC_EDIT_MONEY);
		if (pEdit)
		{
			pEdit->ShowWindow(SW_HIDE);
		}
		CTransparentStc *pStatic = (CTransparentStc *)GetDlgItem(IDC_STATIC_TYPE_NAME);	
		if (pStatic)
		{
			pStatic->ShowWindow(SW_HIDE);
		}
		pStatic = (CTransparentStc *)GetDlgItem(IDC_STATIC_MONEY_TYPE);
		if (pStatic)
		{
			pStatic->ShowWindow(SW_HIDE);
		}
	}	
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
	BOOL bJTMTrancelate,bJTSTrancelate,bBTMTrancelate,bBTSTrancelate,bDMMTrancelate,bDMSTrancelate, bZDFTrancelate;
	bJTMTrancelate = bJTSTrancelate = bBTMTrancelate = bBTSTrancelate = bDMMTrancelate = bDMSTrancelate = bZDFTrancelate = FALSE;

	m_GameSetInfo.iJuTimeMinute = GetDlgItemInt(IDC_EDIT_JU_TIME_MINUTE,&bJTMTrancelate);
	m_GameSetInfo.iJuTimeSecond = GetDlgItemInt(IDC_EDIT_JU_TIME_SECOND,&bJTSTrancelate);
	m_GameSetInfo.iBuTimeMinute = GetDlgItemInt(IDC_EDIT_BU_TIME_MINUTE,&bBTMTrancelate);
	m_GameSetInfo.iBuTimeSecond = GetDlgItemInt(IDC_EDIT_BU_TIME_SECOND,&bBTSTrancelate);
	m_GameSetInfo.iDuMiaoMinute = GetDlgItemInt(IDC_EDIT_DU_MIAO_MINUTE,&bDMMTrancelate);
	m_GameSetInfo.iDuMiaoSecond = GetDlgItemInt(IDC_EDIT_DU_MIAO_SECOND,&bDMSTrancelate);
	m_GameSetInfo.iGameMoney    = GetDlgItemInt(IDC_EDIT_MONEY,&bZDFTrancelate);

	//m_GameSetInfo.iBeginStation=((CComboBox *)GetDlgItem(IDC_BEGIN_STATION))->GetCurSel()+2;
	//m_GameSetInfo.iEndStation=((CComboBox *)GetDlgItem(IDC_END_STATION))->GetCurSel()+2;
	//m_GameSetInfo.iCardCount=(((CButton *)GetDlgItem(IDC_TWO))->GetCheck()==BST_CHECKED)?1:2;
	//m_GameSetInfo.iPlayCount=GetDlgItemInt(IDC_COUNT,&bTrancelate);

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
	//��ʱ��������5����
	if(m_GameSetInfo.iJuTimeMinute*60+m_GameSetInfo.iJuTimeSecond < 630)
	{
		MessageBox("�����ú��ʵ�ʱ��(һ����Ϸ����10��30��)","��ʾ",0);
		GetDlgItem(IDC_EDIT_JU_TIME_SECOND)->SetFocus();
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
	//ÿ��ʱ������10��
	if(m_GameSetInfo.iBuTimeMinute*60+m_GameSetInfo.iBuTimeSecond < 90)
	{
		MessageBox("�����ú��ʵ�ʱ��(һ������1��30��)","��ʾ",0);
		GetDlgItem(IDC_EDIT_BU_TIME_SECOND)->SetFocus();
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
	//ÿ��ʱ������10��
	if(m_GameSetInfo.iDuMiaoMinute*60+m_GameSetInfo.iDuMiaoSecond < 90)
	{
		MessageBox("�����ú��ʵ�ʱ��(��������1��30��)","��ʾ",0);
		GetDlgItem(IDC_EDIT_DU_MIAO_MINUTE)->SetFocus();
		return ;
	}	

	//����ָ����
	if(m_GameSetInfo.iGameMoney < 0 || m_GameSetInfo.iGameMoney > 100000000)
	{
		MessageBox("�����ú��ʵ�ָ����(��0��100000000)","��ʾ",0);
		GetDlgItem(IDC_EDIT_MONEY)->SetFocus();
		return ;
	}
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
	///m_GameSetInfo.iCardCount=iCardCount;
	/*if (GetSafeHwnd()==NULL) 
		Create(IDD_SET_PLAY,m_pPP);*/	
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
	//case WM_CTLCOLORSTATIC :
	//	SetBkMode((HDC)wParam,TRANSPARENT);
	//	HBRUSH brush = (HBRUSH)::GetStockObject(NULL_BRUSH);
	//	return (LRESULT)brush;
	}
	return CCoolView::WindowProc(message, wParam, lParam);
}

HBRUSH CPlaySetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch(pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_0:
	case IDC_STATIC_1:
	case IDC_STATIC_2:
	case IDC_STATIC_3:
	case IDC_STATIC_4:
	case IDC_STATIC_5:
	case IDC_STATIC_6:
	case IDC_STATIC_7:
	case IDC_STATIC_8:
	case IDC_STATIC_TYPE_NAME:
	case IDC_STATIC_MONEY_TYPE:
		{
			pDC-> SetTextColor(RGB(255,255,255));
			pDC->SetBkMode(TRANSPARENT); 
			HBRUSH hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
			return  hbr; 
		}
	default: break;
	}

	return CWnd::OnCtlColor(pDC,pWnd,nCtlColor);
}
