#include "Stdafx.h"
#include "GameSet.h"
#include "ClientGameDlg.h"


BEGIN_MESSAGE_MAP(CGameSet, CCoolView)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//���캯��
CGameSet::CGameSet(CWnd * pParent) : CCoolView(CGameSet::IDD,pParent)
{
	m_bBackMusic=TRUE;
	m_bSound=TRUE;
	m_bShowUser=TRUE;
	m_bEnableWatch=TRUE;
	m_pParent=pParent;
	//	m_bk.SetLoadInfo(".\\image\\ddz\\game_set_bg.bmp",true);
}

//��������
CGameSet::~CGameSet()
{
}

//DDX/DDV ֧��
void CGameSet::DoDataExchange(CDataExchange* pDX)
{
	CCoolView::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDC_SOUND, m_check_1);
	DDX_Control(pDX, IDC_USER_MESSAGE, m_check_2);
	DDX_Control(pDX, IDC_WATCH, m_check_3);
	DDX_Control(pDX, IDC_RIGHT_POWER, m_check_4);
	//	DDX_Control(pDX, IDC_BK_SOUND, m_check_5);
}

//ȷ����ť
void CGameSet::OnOK()
{
	m_bSound=(m_check_1.GetCheck()==BST_CHECKED);
	m_bShowUser=(m_check_2.GetCheck()==BST_CHECKED);
	m_bEnableWatch=(m_check_3.GetCheck()==BST_CHECKED);
	m_bRightPower=(m_check_4.GetCheck()==BST_CHECKED);
	//m_bBackMusic=(m_check_5.GetCheck()==BST_CHECKED);
	CCoolView::OnOK();
}

//��ʼ���Ի���
BOOL CGameSet::OnInitDialog()
{
	CCoolView::OnInitDialog();

	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));

	TCHAR path[MAX_PATH];
	wsprintf(path,TEXT(".\\image\\ok_bt.bmp"));
	m_Ok.LoadButtonBitmap(path,false);
	wsprintf(path,TEXT(".\\image\\cancel_bt.bmp"));
	m_Cancel.LoadButtonBitmap(path,false);
	wsprintf(path,TEXT(".\\image\\game_set.bmp"));
	m_bk.SetLoadInfo(path,true);

	if (m_bSound) m_check_1.SetCheck(BST_CHECKED);
	if (m_bShowUser) m_check_2.SetCheck(BST_CHECKED);
	if (m_bEnableWatch) m_check_3.SetCheck(BST_CHECKED);
	if (m_bRightPower) m_check_4.SetCheck(BST_CHECKED);
	//if (m_bBackMusic) m_check_5.SetCheck(BST_CHECKED);
	
	CRect ckRect;
	CRect okRect;
	CRect ccelRect;
	/*CPoint ckPoint;
	CPoint okPoint;*/
	m_check_1.GetWindowRect(&ckRect);
	ScreenToClient(&ckRect);
	m_Ok.GetWindowRect(&okRect);
	ScreenToClient(&okRect);

	//ckPoint.SetPoint(ckRect.left, ckRect.top);
	//::MapWindowPoints(GetDesktopWindow()->m_hWnd, m_hWnd, &ckPoint, 1);
	//MyMsgBox("%d %d %d %d", ckRect.left, ckRect.right, ckRect.top, ckRect.bottom/*, ckPoint.x, ckPoint.y*/);
	int btWidth = okRect.right - okRect.left;
	int btHeight = okRect.bottom - okRect.top;
	okRect.left = ckRect.left;
	okRect.right = okRect.left + btWidth;
	ccelRect.left = m_bk.GetWidth() - okRect.right;
	ccelRect.right = ccelRect.left + btWidth;
	ccelRect.top = okRect.top;
	ccelRect.bottom = okRect.bottom;
	//MyMsgBox("%d %d %d %d     %d %d", ccelRect.left, ccelRect.right, ccelRect.top, ccelRect.bottom, btWidth, btHeight);
	m_Ok.MoveWindow(&okRect);
	m_Cancel.MoveWindow(&ccelRect);

	MoveWindow(0,0,m_bk.GetWidth(),m_bk.GetHeight());
	CenterWindow(GetParent());
	//AutoSize();
	((CWnd*)GetDlgItem(IDC_RIGHT_POWER))->ShowWindow(SW_HIDE);
	//	((CWnd*)GetDlgItem(IDC_BK_SOUND))->ShowWindow(SW_HIDE);
	//AfxSetResourceHandle(GetModuleHandle(NULL));
	return TRUE; 
}
//�Զ�������С
void CGameSet::AutoSize()
{	
	CRect rect,clientrect;
	int cx,cy,csx,csy,x,y;
	((CClientGameDlg*)m_pParent)->m_PlayView.GetClientRect(&rect);
	cx=rect.Width();
	cy=rect.Height();
	csx = (GetSystemMetrics(SM_CXSCREEN)>1024?1024:GetSystemMetrics(SM_CXSCREEN));
	csy = (GetSystemMetrics(SM_CYSCREEN)>768?768:GetSystemMetrics(SM_CYSCREEN));
	GetClientRect(clientrect);
	//x = clientrect.Width();
	x=m_bk.GetWidth();
	//y = clientrect.Height();
	y=m_bk.GetHeight();
	MoveWindow((csx-cx)/2 + (cx-x),(csy-cy)/2 +(cy-y)/2,x,y);
}

LRESULT CGameSet::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
		HBRUSH hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
		SetBkMode((HDC)wParam,TRANSPARENT);
		SetTextColor((HDC)wParam, RGB(0,0,0));
		//	SetBkMode(h
		SetBkColor((HDC)wParam,m_bk.GetPixel(10,10));//RGB(187,104,2));
		//return (LRESULT)CreateSolidBrush(m_bk.GetPixel(10,10));//RGB(187,104,2));
		return (LRESULT)hBrush;
	}
	//InvalidateRect(NULL);	
	return CCoolView::WindowProc(message, wParam, lParam);
}

void CGameSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CGameImageHelper	help(&m_bk);	
	help.BitBlt(dc.GetSafeHdc(),0,0);

	//CDC dcTmp;
	//dcTmp.CreateCompatibleDC(&dc);
	//dcTmp.SelectObject(help);
	//::TransparentBlt(dc.GetSafeHdc(), 0,0, help.GetWidth(), help.GetHeight(),
	//	dcTmp.GetSafeHdc(), 0,0,help.GetWidth(), help.GetHeight(),RGB(255,0,255));
	//dcTmp.DeleteDC();
}
