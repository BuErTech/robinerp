// DlgDudu.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "DlgDudu.h"
#include "GameRoomEx.h"
// CDlgDudu �Ի���

IMPLEMENT_DYNAMIC(CDlgDudu, CDialog)

CDlgDudu::CDlgDudu(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_DUDU, pParent)
	, m_InputStr(_T(""))
	, m_pGameRoom(NULL)
{
	m_bCanSend = true;
}

CDlgDudu::~CDlgDudu()
{
}

void CDlgDudu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_InputStr);
	DDV_MaxChars(pDX, m_InputStr, 500);
	DDX_Control(pDX, IDCANCEL, m_btcancel);
	DDX_Control(pDX, IDOK, m_btok);
}


BEGIN_MESSAGE_MAP(CDlgDudu, CDialog)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CDlgDudu::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_INPUT, &CDlgDudu::OnEnChangeEdit1)
	ON_BN_CLICKED(IDCANCEL, &CDlgDudu::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgDudu ��Ϣ�������

void CDlgDudu::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	
	if(m_InputStr.IsEmpty())
	{
		GetDlgItem(IDC_MSG)->SetWindowText(TEXT("0���ַ�"));
		return ;
	}
	else 
	{
		if(!m_bCanSend)
		{
			GetDlgItem(IDC_MSG)->SetWindowText(TEXT("���Ժ��ٷ�."));
			return ;
		}
		int iCount=(int)(m_InputStr.GetLength()/2),iMoney=(int)((m_InputStr.GetLength()+120)/120)*3000;
		((CGameRoomEx*)m_pGameRoom)->PayMoney(-iMoney,0);
		m_bCanSend = false;
		SetTimer(1, 2000, NULL);
	}
	//OnOK();
}

void CDlgDudu::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	UpdateData();
	TCHAR sz[100];
	int iCount=(int)(m_InputStr.GetLength()/2),iMoney=(int)((m_InputStr.GetLength()+120)/120)*3000;
	wsprintf(sz,TEXT("%d�֣���%d�����"),iCount,iMoney);
	GetDlgItem(IDC_MSG)->SetWindowText(sz);	
}

//��ʱ����Ϣ
void CDlgDudu::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1)
	{
		KillTimer(1);
		m_bCanSend = true;
		OnEnChangeEdit1();
	}
}

void CDlgDudu::OnPaint()
{
	CPaintDC dc(this);
	CGameImageHelper	handle(&m_bkimage);
	handle.BitBlt(dc.GetSafeHdc(),0,0);
}
LRESULT CDlgDudu::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
		return (LRESULT)CreateSolidBrush(m_bkimage.GetPixel(145,35));
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

// CDlgInMoney message handlers

void CDlgDudu::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
}

//��ʼ������
BOOL CDlgDudu::OnInitDialog()
{
	__super::OnInitDialog();
	
	//��ͼ
	CString s=CINIFile::GetAppPath ();/////����·��
	CINIFile f( s + "skin.ini");
	CString key=Glb().m_skin;
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal(key,"skinfolder","image\\client\\skin0\\");
	CGameImage	m_bt;

	wsprintf(path,"%sdialog\\dudu.bmp",skinfolder);
	m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper ImageHandle(&m_bkimage);
	int cx,cy;
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_bkimage.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
	}
	cx=ImageHandle.GetWidth();
	cy=ImageHandle.GetHeight();
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);

	wsprintf(path,"%sdialog\\WalletOk_bt.bmp",skinfolder);
	m_btok.LoadButtonBitmap(path,false);

	wsprintf(path,"%sdialog\\WalletCancle_bt.bmp",skinfolder);
	m_btcancel.LoadButtonBitmap(path,false);
	OnEnChangeEdit1();
	return true;
}
void CDlgDudu::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(1);
	OnCancel();
}
