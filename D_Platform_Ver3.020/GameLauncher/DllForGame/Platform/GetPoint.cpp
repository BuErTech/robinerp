// GetPoint.cpp : ʵ���ļ�
//

#include "../stdafx.h"
#include "GetPoint.h"
//#include "GameFrameDlg.h"

// CGetPoint �Ի���

IMPLEMENT_DYNAMIC(CGetPoint, CDialog)

CGetPoint::CGetPoint(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_GETPOINT, pParent)
{
	m_pSend = NULL;
	m_SerPoint = 0;
	m_iInWhere = 0;
	m_bkBrush=NULL;
}

CGetPoint::~CGetPoint()
{
	DeleteObject(m_bkBrush);
}

void CGetPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_btcancel);
	DDX_Control(pDX, IDOK, m_btok);
}


BEGIN_MESSAGE_MAP(CGetPoint, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CGetPoint::OnBnClickedOk)
END_MESSAGE_MAP()


// CGetPoint ��Ϣ�������

LRESULT CGetPoint::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
		SetTextColor((HDC)wParam, RGB(0,0,0)) ;
		SetBkColor((HDC)wParam, m_bkimage.GetPixel(145,35));
		return (LRESULT)m_bkBrush;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CGetPoint::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CGameImageHelper	handle(&m_bkimage);
	handle.BitBlt(dc.GetSafeHdc(),0,0);
}

BOOL CGetPoint::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString s=CINIFile::GetAppPath ();/////����·��
	CINIFile f( s + Glb().m_skin);

	CString key=TEXT("config");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal(key,"skinfolder","image\\game\\");
	skinfolder=s+"\\"+skinfolder;
	wsprintf(path,"%sgame\\point\\getpoint_bk.bmp",skinfolder);


	CGameImage	m_bt;
	//wsprintf(path,".\\image\\game\\point\\getpoint_bk.bmp");
	m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	CGameImageHelper ImageHandle(&m_bkimage);
	int cx,cy;
	HRGN hRgn=AFCBmpToRgn(ImageHandle,RGB(255, 0, 255),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
	}
	cx=ImageHandle.GetWidth();
	cy=ImageHandle.GetHeight();
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);

	wsprintf(path,"%sgame\\point\\ok_bt.bmp",skinfolder);
//	wsprintf(path,".\\image\\game\\point\\ok_bt.bmp");
	m_btok.LoadButtonBitmap(path,false);

	wsprintf(path,"%sgame\\point\\cancel_bt.bmp",skinfolder);
//	wsprintf(path,".\\image\\game\\point\\cancel_bt.bmp");
	m_btcancel.LoadButtonBitmap(path,false);


	m_bkBrush=CreateSolidBrush(m_bkimage.GetPixel(145,35));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CGetPoint::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	//CDialog::OnLButtonDown(nFlags, point);
}

void CGetPoint::OnBnClickedOk()
{
	DWORD point = GetDlgItemInt(IDC_POINT);
	if(point <= 0 || point > m_SerPoint)
	{
		AFCMessageBox("����ȷ�������ֵ!", "��ʾ");
		return ;
	}
	if(point > 1000000)
	{
		AFCMessageBox("һ�����ֻ��ȡ100��!", "��ʾ");
		return ;
	}
	//GetParent()->
	m_pSend->PostMessage(IDM_GETPOINT,point,m_iInWhere);

	ShowWindow(false);
}

void CGetPoint::SetPoint(DWORD dwPoint)
{
	TCHAR sz[100];
	m_SerPoint = dwPoint;
	wsprintf(sz, TEXT("��Ļ��ֺ����� %d ����."), dwPoint);
	SetDlgItemText(IDC_SERPOINT, sz);
}
