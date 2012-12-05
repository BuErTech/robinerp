#include "StdAfx.h"
#include "GMListDlg.h"

CGMListDlg::CGMListDlg(CWnd * pParentWnd)
{
	m_pParentWnd = pParentWnd;
}

CGMListDlg::~CGMListDlg(void)
{
}
BEGIN_MESSAGE_MAP(CGMListDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CGMListDlg::OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CLOSE, &CGMListDlg::OnBnClickedClose)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CGMListDlg::OnNMDblclkList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CGMListDlg::OnNMClickList1)
	ON_MESSAGE(WM_EXCHANGE_SKIN,&CGMListDlg::OnExchangeSkin)
END_MESSAGE_MAP()

void CGMListDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_GMListCtrl.SetSelectedColumn(0);
	POSITION pos = m_GMListCtrl.GetFirstSelectedItemPosition();
	int index = m_GMListCtrl.GetNextSelectedItem(pos);
	if (0 <= index && index < m_GMlist.size())
		m_pParentWnd->PostMessage(WM_TALK_2_GM, WPARAM(&(m_GMlist[index])), 0);
	//OnOK();
}

void CGMListDlg::OpenWindow(MSG_GP_R_GmList_t list)
{
	//memcpy(&m_GMlist, &list, sizeof(list));
	m_GMlist.clear();
	for (int i = 0; i < list._nCount; i++) m_GMlist.push_back(list._arGmID[i]);
	sort(m_GMlist.begin(), m_GMlist.end());
	m_GMListCtrl.DeleteAllItems();
	CString s;
	for (int i = 0; i < m_GMlist.size(); i++)
	{
		s.Format("����Ա%d", m_GMlist[i]);
		m_GMListCtrl.InsertItem(i, s);
	}
	ShowWindow(SW_SHOWNOACTIVATE);
	this->SetForegroundWindow();
}

void CGMListDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	/*// 2012.12.17 yyf ����
	CGameImageHelper helplt(&m_imgDlgBk);
	helplt.BitBlt(dc.GetSafeHdc(),0,0);*/

	//�滭��ܵ�ͼ 2012.10.15 yyf
	m_configDlg.DrawViewFrame(&dc);

	CFont font;
	font.CreateFont(-12,0,0,0,400,0,0,0,DEFAULT_CHARSET,3,2,1,2,TEXT("Arial"));
	CFont* pOldFont = (CFont*)dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(252,255,255));
	dc.TextOut(5,5,"���й���Ա");
	dc.SelectObject(pOldFont);
	font.DeleteObject();

	/*// 2012.12.17 yyf ����
	PAINTSTRUCT lp;
	CDC *pDC=BeginPaint(&lp);
	CRect cRect;
	GetWindowRect(&cRect);

	//CGameImageHelper	help(&m_imgDlgBk);
	CGameImageHelper	help(&m_configDlg.GetBKImageEX());
	CDC destdc;
	destdc.CreateCompatibleDC(pDC);
	destdc.SelectObject(help);
	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	//::TransparentBlt(pDC->GetSafeHdc(),cRect.left,cRect.top,help.GetWidth(),help.GetHeight(),destdc.GetSafeHdc(),0,0,help.GetWidth(),help.GetHeight(),0xff00ff);
	::TransparentBlt(pDC->GetSafeHdc(),cRect.left,cRect.top,cx,cy,destdc.GetSafeHdc(),0,0,cx,cy,0xff00ff);
	EndPaint(&lp);
	*/
}

void CGMListDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: �ڴ����ר�ô����/����û���
	DDX_Control(pDX, IDCANCEL, m_btnCancle);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDC_CLOSE, m_bnClose);
	DDX_Control(pDX, IDC_LIST1, m_GMListCtrl);
	CDialog::DoDataExchange(pDX);
}

BOOL CGMListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CGMListDlg::Init()
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder;

	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());

	/*// 2012.12.17 yyf ����
	sprintf_s(path,sizeof(path),"%sdialog\\manager_bk.bmp",skinfolder);
	m_imgDlgBk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	CGameImageHelper	ImageHandle(&m_imgDlgBk);
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_imgDlgBk.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		DeleteObject(hRgn);
	}*/
	//*-------��ʼ�� m_configDlg--------- 2012.10.15 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CGMListDlgConfig\\configDlg.bcf",s,skinfolder);// 
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	//-----------------------------------------------------*/

	sprintf_s(path,sizeof(path),"%sdialog\\normal_bt.BMP",skinfolder);
	m_btnOk.LoadButtonBitmap(path,false);
	sprintf_s(path,sizeof(path),"%sdialog\\normal_bt.BMP",skinfolder);
	m_btnCancle.LoadButtonBitmap(path,false);

	/*// 2012.12.17 yyf ����
	int iwith = m_imgDlgBk.GetWidth();
	int iHeight = m_imgDlgBk.GetHeight();

	MoveWindow(0,0,m_imgDlgBk.GetWidth(),m_imgDlgBk.GetHeight());*/

	// 2012.12.17 yyf
	int iwith = cx;
	int iHeight = cy;
	MoveWindow(0,0,cx,cy);

	CenterWindow(GetParent());  //ʹ�Ի��������ʾ


	CString str;
	m_GMListCtrl.InsertColumn(0, "���ߵĹ���Ա", LVCFMT_LEFT,258);
	m_GMListCtrl.SetRedraw(true);
	m_GMListCtrl.SetExtendedStyle((m_GMListCtrl.GetStyle() | LVS_EX_FULLROWSELECT) & ~LVS_EX_CHECKBOXES);
	//m_GMListCtrl.SetBkColor(RGB(255,0,0));
	CString strPath = CBcfFile::GetAppPath();
	CRect rc;
	GetClientRect(&rc);
	wsprintf(path,"%s\\%sdialog\\log_exit_bt.bmp",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());//win_close.bmp
	m_bnClose.LoadButtonBitmap(path,false);
	CGameImage bt;
	bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_bnClose.MoveWindow(rc.right-rc.left-bt.GetWidth()/4, 0, bt.GetWidth()/4, bt.GetHeight());

	SetWindowText("���й���Ա");
	::SetWindowPos(m_hWnd,CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
}

void CGMListDlg::LoadSkin()
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder;

	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());

	/*// 2012.12.17 yyf ����
	sprintf_s(path,sizeof(path),"%sdialog\\manager_bk.bmp",skinfolder);
	m_imgDlgBk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	CGameImageHelper	ImageHandle(&m_imgDlgBk);
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_imgDlgBk.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		DeleteObject(hRgn);
	}*/
	
	//*-------��ʼ�� m_configDlg--------- 2012.10.15 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CGMListDlgConfig\\configDlg.bcf",s,skinfolder);// 
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	//-----------------------------------------------------*/
	sprintf_s(path,sizeof(path),"%sdialog\\normal_bt.BMP",skinfolder);
	m_btnOk.LoadButtonBitmap(path,false);
	sprintf_s(path,sizeof(path),"%sdialog\\normal_bt.BMP",skinfolder);
	m_btnCancle.LoadButtonBitmap(path,false);

	//int iwith = m_imgDlgBk.GetWidth();
	//int iHeight = m_imgDlgBk.GetHeight();

	//MoveWindow(0,0,m_imgDlgBk.GetWidth(),m_imgDlgBk.GetHeight());
	//CenterWindow(GetParent());  //ʹ�Ի��������ʾ


	CString str;
	//m_GMListCtrl.InsertColumn(0, "���ߵĹ���Ա", LVCFMT_LEFT,258);
	//m_GMListCtrl.SetRedraw(true);
	//m_GMListCtrl.SetExtendedStyle((m_GMListCtrl.GetStyle() | LVS_EX_FULLROWSELECT) & ~LVS_EX_CHECKBOXES);
	//m_GMListCtrl.SetBkColor(RGB(255,0,0));
	CString strPath = CBcfFile::GetAppPath();
	CRect rc;
	GetClientRect(&rc);
	wsprintf(path,"%s\\%sdialog\\log_exit_bt.bmp",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());//win_close.bmp
	m_bnClose.LoadButtonBitmap(path,false);
	CGameImage bt;
	bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_bnClose.MoveWindow(rc.right-rc.left-bt.GetWidth()/4, 0, bt.GetWidth()/4, bt.GetHeight());

	//SetWindowText("���й���Ա");
	//::SetWindowPos(m_hWnd,CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
}
LRESULT CGMListDlg::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	LoadSkin();
	//OnPaint();
	Invalidate();
	return LRESULT(0);
}
HBRUSH CGMListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CGMListDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

void CGMListDlg::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CGMListDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPoint Point;
	GetCursorPos(&Point);
	m_GMListCtrl.ScreenToClient(&Point);
	int index = m_GMListCtrl.HitTest(Point);
	if (0 <= index && index < m_GMlist.size())
		m_pParentWnd->PostMessage(WM_TALK_2_GM, WPARAM(&(m_GMlist[index])), 0);
	*pResult = 0;
}

void CGMListDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPoint Point;
	GetCursorPos(&Point);
	m_GMListCtrl.ScreenToClient(&Point);
	int index = m_GMListCtrl.HitTest(Point);
	if (0 <= index && index < m_GMlist.size())
		m_btnOk.EnableWindow(TRUE);
	else 
		m_btnOk.EnableWindow(FALSE);
	*pResult = 0;
}

// ���ô���Բ�� 2012.10.15 yyf
void CGMListDlg::SetWndRgn(void)
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