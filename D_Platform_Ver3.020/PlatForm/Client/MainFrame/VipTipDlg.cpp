#include "VipTipDlg.h"
#include "VipTipDlg.h"
#include "VipTipDlg.h"
// VipTipDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VipTipDlg.h"


// CVipTipDlg �Ի���

IMPLEMENT_DYNAMIC(CVipTipDlg, CGameFaceGo)

CVipTipDlg::CVipTipDlg(CWnd* pParent /*=NULL*/)
	: CGameFaceGo(CVipTipDlg::IDD)
{

}

CVipTipDlg::~CVipTipDlg()
{
}

void CVipTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_buyBtn);
	DDX_Control(pDX, IDQUXIAO, m_cancelBtn);
	//DDX_Control(pDX, IDC_CLOSEBTN, m_closeBtn);
}


BEGIN_MESSAGE_MAP(CVipTipDlg, CGameFaceGo)
	ON_BN_CLICKED(IDOK, &CVipTipDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDQUXIAO, &CVipTipDlg::OnBnClickedQuxiao)
	ON_BN_CLICKED(IDC_CLOSEBTN, &CVipTipDlg::OnBnClickedClosebtn)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_EXCHANGE_SKIN,&CVipTipDlg::OnExchangeSkin)
END_MESSAGE_MAP()

//����
LRESULT CVipTipDlg::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	LoadSkin();
	//OnPaint();
	Invalidate();
	return LRESULT(0);
}
// CVipTipDlg ��Ϣ�������

void CVipTipDlg::OnBnClickedOk()
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	CString url;
	url=f.GetKeyVal("BZW","BuyVipURL","/Manage/VipApply.aspx ");
	url = Glb().m_CenterServerPara.m_strWebRootADDR + url;
	ShellExecute(NULL,"open",url,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
	OnOK();
}

void CVipTipDlg::OnBnClickedQuxiao()
{
	OnOK();
}

void CVipTipDlg::OnBnClickedClosebtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CVipTipDlg::LoadSkin()
{
	CString s=CBcfFile::GetAppPath ();//����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());
	
	//*-------��ʼ�� m_configDlg--------- 2012.10.17 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CVipTipDlgConfig\\configDlg.bcf",s,strSkin);// 
	wsprintf(PathDefault,"%s%s",s,strSkin);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	//-----------------------------------------------------*/

	wsprintf(path,"%sdialog\\normal_bt.BMP",skinfolder);

	m_buyBtn.LoadButtonBitmap(path,true);
	m_cancelBtn.LoadButtonBitmap(path,true);
	//m_closeBtn.LoadButtonBitmap(path,true);

	SetWindowText("VIP��ʾ");
	m_titelStr = "VIP��ʾ";    //����
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	SetDlgItemText(IDC_INFORSTR,
		fMsg.GetKeyVal("VipTipDlg","RoomForVIP","     ��Ǹ,�˷���ֻ�����Ա�û�����!��������������VIP��"));
	SetDlgItemText(IDC_STATIC_BUY,
		fMsg.GetKeyVal("VipTipDlg","BuyVIP","�������򣬻��¼�ٷ���վ����VIP��Ϊ��Ա!"));
	SetDlgItemText(IDOK,
		fMsg.GetKeyVal("VipTipDlg","BTNBuy","��������VIP"));
	SetDlgItemText(IDQUXIAO,
		fMsg.GetKeyVal("VipTipDlg","BTNCancel","ȡ ��"));
/*
	wsprintf(path,"%sdialog\\msg_bk.bmp",m_skinmgr.GetSkinPath());
	m_backPic.SetLoadInfo(path,CGameImageLink::m_bAutoLock);*/

	CRect rt;
	GetWindowRect(& rt);
	//MoveWindow(rt.top,rt.left,m_backPic.GetWidth(),m_backPic.GetHeight());
	MoveWindow(rt.top,rt.left,cx,cy);// 2012.12.17 yyf 
	SetFocus();
	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
}
BOOL CVipTipDlg::OnInitDialog()
{
	CGameFaceGo::OnInitDialog();

	CString s=CBcfFile::GetAppPath ();//����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());
	
	//*-------��ʼ�� m_configDlg--------- 2012.10.17 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CVipTipDlgConfig\\configDlg.bcf",s,strSkin);// 
	wsprintf(PathDefault,"%s%s",s,strSkin);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	//-----------------------------------------------------*/

	wsprintf(path,"%sdialog\\normal_bt.BMP",skinfolder);

	m_buyBtn.LoadButtonBitmap(path,true);
	m_cancelBtn.LoadButtonBitmap(path,true);
	//m_closeBtn.LoadButtonBitmap(path,true);

	SetWindowText("VIP��ʾ");
	m_titelStr = "VIP��ʾ";    //����
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	SetDlgItemText(IDC_INFORSTR,
		fMsg.GetKeyVal("VipTipDlg","RoomForVIP","     ��Ǹ,�˷���ֻ�����Ա�û�����!��������������VIP��"));
	SetDlgItemText(IDC_STATIC_BUY,
		fMsg.GetKeyVal("VipTipDlg","BuyVIP","�������򣬻��¼�ٷ���վ����VIP��Ϊ��Ա!"));
	SetDlgItemText(IDOK,
		fMsg.GetKeyVal("VipTipDlg","BTNBuy","��������VIP"));
	SetDlgItemText(IDQUXIAO,
		fMsg.GetKeyVal("VipTipDlg","BTNCancel","ȡ ��"));

	/*// 2012.12.17 yyf ����
	wsprintf(path,"%sdialog\\msg_bk.bmp",m_skinmgr.GetSkinPath());
	m_backPic.SetLoadInfo(path,CGameImageLink::m_bAutoLock);*/

	CRect rt;
	GetWindowRect(& rt);
	//MoveWindow(rt.top,rt.left,m_backPic.GetWidth(),m_backPic.GetHeight());
	MoveWindow(rt.top,rt.left,cx,cy);// 2012.12.17 yyf 
	CenterWindow();

	SetFocus();
	
	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CVipTipDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect wndRt;
	GetWindowRect(& wndRt);

	/*// 2012.12.17 yyf ����
	CGameImageHelper helprb(&m_backPic);
	helprb.BitBlt(dc.GetSafeHdc(),0,0);*/
	
	//�滭��ܵ�ͼ 2012.10.17 yyf
	m_configDlg.DrawViewFrame(&dc);

	m_rectTitleText = CRect(0,0,50,33);
	m_rectTitleText.OffsetRect(18,6);

	int nOldMode = dc.SetBkMode(TRANSPARENT);   //���ñ���Ϊ͸����
	COLORREF clOldText = dc.SetTextColor(RGB(242, 253, 253));  //������������ɫ
	dc.DrawText(m_titelStr,& m_rectTitleText,DT_LEFT | DT_CALCRECT);
	dc.DrawText(m_titelStr,& m_rectTitleText,DT_LEFT);
	dc.SetBkMode(nOldMode);
	dc.SetTextColor(clOldText);
}

HBRUSH CVipTipDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CGameFaceGo::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetBkMode(TRANSPARENT);
		///{{ Modified by zxd 20100709 �ͷ�GDI��Դ
		return (HBRUSH)GetSysColorBrush(HOLLOW_BRUSH);
		//CBrush brush;
		//brush.CreateStockObject(HOLLOW_BRUSH);
		//b = (HBRUSH)brush.m_hObject;
		//return (HBRUSH)b;
		/// Modified by zxd 20100709 �ͷ�GDI��Դ}}
	}
	return hbr;
}

void CVipTipDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	CGameFaceGo::OnLButtonDown(nFlags, point);
}

// ���ô���Բ�� 2012.10.15 yyf
void CVipTipDlg::SetWndRgn(void)
{
	POINT m_arPoints[8];
	CRect winRect;
	GetWindowRect(&winRect);
	ScreenToClient(&winRect);
 
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