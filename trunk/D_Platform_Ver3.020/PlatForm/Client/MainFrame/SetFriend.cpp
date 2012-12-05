#include "SetFriend.h"
// SetFriend.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SetFriend.h"

#include "MainRoomEx.h"


// CSetFriend �Ի���

IMPLEMENT_DYNAMIC(CSetFriend, CDialog)

CSetFriend::CSetFriend(CWnd* pParent /*=NULL*/)
	: CDialog(CSetFriend::IDD, pParent)
{
	m_bkBrush = NULL;
}

CSetFriend::~CSetFriend()
{
	DeleteObject(m_bkBrush);
}

void CSetFriend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_Ok);

	DDX_Control(pDX, IDCANCEL, m_Cancel);

	DDX_Control(pDX, IDC_BUTTON1, m_Close);
}


BEGIN_MESSAGE_MAP(CSetFriend, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CSetFriend::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetFriend::OnBnClickedClose)
	ON_BN_CLICKED(IDCANCEL, &CSetFriend::OnBnClickedCancel)
	ON_MESSAGE(WM_EXCHANGE_SKIN,&CSetFriend::OnExchangeSkin)
END_MESSAGE_MAP()


// CSetFriend ��Ϣ�������

BOOL CSetFriend::OnInitDialog()
{
	CDialog::OnInitDialog();
	Init();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CSetFriend::Init()
{
	//m_bnBuy.LoadButtonBitmap(szButton,false);
	//��ͼ
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("Bank");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());
	CGameImage	m_bt;
	/*2012.10.15 yyf //����
	wsprintf(path,"%sdialog\\SetFriend.png",skinfolder);
	m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper ImageHandle(&m_bkimage);
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_bkimage.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		///{{ Added by zxd 20100709 �ͷ��������
		DeleteObject(hRgn);
		///Added by zxd 20100709 �ͷ��������}}
	}
	int cx,cy;
	cx=ImageHandle.GetWidth();
	cy=ImageHandle.GetHeight();//*/
	
	//*-------��ʼ�� m_configDlg--------- 2012.10.15 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CSetFriendConfig\\configDlg.bcf",s,skinfolder);//Glb().m_Path
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	//-----------------------------------------------------*/
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);

	SetWindowText("��������");

	TCHAR szButton[MAX_PATH];
	sprintf(szButton, "%s\\%sdialog\\normal_bt.BMP",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());
	m_Ok.LoadButtonBitmap(szButton,false);
	m_Cancel.LoadButtonBitmap(szButton, false);

	CRect rc;
	GetClientRect(&rc);
	wsprintf(szButton,"%s\\%sdialog\\log_exit_bt.bmp",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());//win_close.bmp
	m_Close.LoadButtonBitmap(szButton,false);
	CGameImage bt;
	bt.SetLoadInfo(szButton,CGameImageLink::m_bAutoLock);
	m_Close.MoveWindow(rc.right-rc.left-bt.GetWidth()/4, 0, bt.GetWidth()/4, bt.GetHeight());

	m_bkBrush = CreateSolidBrush( m_configDlg.GetBKImageEX().GetPixel(32,55));

	m_iType = GetMainRoom()->m_PlaceUserInfo.iAddFriendType;

	int h = m_iType / 65535;
	int l = m_iType % 65535;

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(false);

	if (l == 0) ((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(true);
	if (l == 1) ((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(true);
	if (h == 0) ((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(true);
	if (h == 1) ((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(true);
	if (h == 2) ((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(true);

	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
}
//����Ƥ��
void CSetFriend::LoadSkin()
{
	//��ͼ
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("Bank");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());

	CGameImage	m_bt;
/*2012.10.15 yyf //����
	wsprintf(path,"%sdialog\\SetFriend.png",skinfolder);
	m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);*/
	
	//*-------��ʼ�� m_configDlg--------- 2012.10.15 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CSetFriendConfig\\configDlg.bcf",s,skinfolder);//Glb().m_Path
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	
	 
	//-----------------------------------------------------*/
	TCHAR szButton[MAX_PATH];
	sprintf(szButton, "%s\\%sdialog\\normal_bt.BMP",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());
	m_Ok.LoadButtonBitmap(szButton,false);
	m_Cancel.LoadButtonBitmap(szButton, false);

	CRect rc;
	GetClientRect(&rc);
	wsprintf(szButton,"%s\\%sdialog\\log_exit_bt.bmp",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());//win_close.bmp
	m_Close.LoadButtonBitmap(szButton,false);
	CGameImage bt;
	bt.SetLoadInfo(szButton,CGameImageLink::m_bAutoLock);

	SetWndRgn();// ���ô���Բ��2012.10.15 yyf
}
LRESULT CSetFriend::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	LoadSkin();
	Invalidate();
	return LRESULT(0);
}
void CSetFriend::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	CDialog::OnOK();
}

void CSetFriend::SetType(int Type)
{
	m_iType = Type;
}

void CSetFriend::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CLoadFileBmp::OnPaint()

	/* //2012.10.15 yyf����
	CGameImageHelper	help(&m_bkimage);
	help.BitBlt(dc.GetSafeHdc(),0,0);*/

	//�滭��ܵ�ͼ 2012.10.15 yyf
	m_configDlg.DrawViewFrame(&dc);
}

LRESULT CSetFriend::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(message)
	{
	case WM_CTLCOLORSTATIC:
		{
			SetTextColor((HDC)wParam, RGB(0,0,0)) ;
			SetBkColor((HDC)wParam,  m_configDlg.GetBKImageEX().GetPixel(32,55));
			return (LRESULT)m_bkBrush;
		}
	default:
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}


afx_msg void CSetFriend::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	CDialog::OnLButtonDown(nFlags, point);
}



afx_msg void CSetFriend::OnBnClickedOk()
{
	UpdateData();

	int h,l;
	if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck() == true)
	{
		l = 0;
	}
	if (((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck() == true)
	{
		l = 1;
	}
	if (((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck() == true)
	{
		h = 0;
	}
	if (((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck() == true)
	{
		h = 1;
	}

	if (((CButton*)GetDlgItem(IDC_RADIO5))->GetCheck() == true)
	{
		h = 2;
	}

	int frendtype = h * 65535 + l;

	GetMainRoom()->SendData(&frendtype, sizeof(int), MDM_GP_IM, ASS_SET_FRIENDTYPE, 0);
	CDialog::OnOK();
}
afx_msg void CSetFriend::OnBnClickedCancel()
{
	OnCancel();
}
afx_msg void CSetFriend::OnBnClickedClose()
{
	OnCancel();
}
// ���ô���Բ�� 2012.10.15 yyf
void CSetFriend::SetWndRgn(void)
{
	POINT m_arPoints[8];
	CRect winRect;
	GetWindowRect(&winRect);
	ScreenToClient(&winRect);

	//m_arPoints[0].x = winRect.left;
	//m_arPoints[0].y = winRect.top+4;
	//m_arPoints[1].x = winRect.left+4;
	//m_arPoints[1].y = winRect.top;
	//m_arPoints[2].x = winRect.right-4;
	//m_arPoints[2].y = winRect.left;
	//m_arPoints[3].x = winRect.right;
	//m_arPoints[3].y = winRect.left+4;
	//m_arPoints[4].x = winRect.right;
	//m_arPoints[4].y = winRect.bottom-4;
	//m_arPoints[5].x = winRect.right-4;
	//m_arPoints[5].y = winRect.bottom;
	//m_arPoints[6].x = winRect.left+4;
	//m_arPoints[6].y = winRect.bottom;
	//m_arPoints[7].x = winRect.left;
	//m_arPoints[7].y = winRect.bottom-4;

    //���Ͻ�
	m_arPoints[0].x = winRect.left;
	m_arPoints[0].y = winRect.top+3;
	m_arPoints[1].x = winRect.left+3;
	m_arPoints[1].y = winRect.top;

	//���Ͻ�
	m_arPoints[2].x = winRect.right-3;
	m_arPoints[2].y = winRect.top;
	m_arPoints[3].x = winRect.right;
	m_arPoints[3].y = winRect.top+3;

	//���½�
	m_arPoints[4].x = winRect.right;
	m_arPoints[4].y = winRect.bottom-4;
	m_arPoints[5].x = winRect.right-4;
	m_arPoints[5].y = winRect.bottom;

	//���½�
	m_arPoints[6].x = winRect.left+4;
	m_arPoints[6].y = winRect.bottom;
	m_arPoints[7].x = winRect.left;
	m_arPoints[7].y = winRect.bottom-4;

	HRGN hRgn = CreatePolygonRgn(m_arPoints, 8, WINDING);
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		DeleteObject(hRgn);
	}
}
