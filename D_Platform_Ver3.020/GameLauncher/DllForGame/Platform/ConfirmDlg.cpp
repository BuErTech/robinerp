// E:\WebKT\WebKT\UI\ConfirmDlg.cpp : ʵ���ļ�
//
#include "../stdafx.h"
#include "ConfirmDlg.h"
#include "confirmdlg.h"
#include "AFCFunction.h"

// ConfirmDlg �Ի���

IMPLEMENT_DYNAMIC(ConfirmDlg, CDialog)
ConfirmDlg::ConfirmDlg(CString strInfo,CString strTitle/* = NULL*/,UINT style /*= AFC_CONFIRM*/,CWnd* pParent /*=NULL*/)
: CDialog(IDD_CONFIRMDLG, pParent)
{
	m_infoStr = strInfo;
	m_titelStr = strTitle;
	m_msgBoxStyle = style;
	//    m_backPicFile = MSGBX_BK;
}

ConfirmDlg::~ConfirmDlg()
{
	m_fontCaption.DeleteObject();
}

void ConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_confirmBtn);
	DDX_Control(pDX, IDQUXIAO, m_cancelBtn);
	DDX_Control(pDX, IDC_CLOSEBTN, m_closeBtn);
}


BEGIN_MESSAGE_MAP(ConfirmDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDQUXIAO, OnBnClickedNo)
	ON_BN_CLICKED(IDC_CLOSEBTN, OnBnClickedClosebtn)
END_MESSAGE_MAP()


// ConfirmDlg ��Ϣ�������

BOOL ConfirmDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL ConfirmDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	TCHAR path[MAX_PATH];

	wsprintf(path,"..\\%sdialog\\msg_bk.bmp",m_skinmgr.GetSkinPath());
	CGameImage bt;
	CRect cRect;
	GetWindowRect(&cRect);
	bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	MoveWindow(cRect.left, cRect.top, bt.GetWidth(), bt.GetHeight(), TRUE);

	//m_backPicFile.Format("image\\client\\%s\\dialog\\win_close.bmp",Glb().m_skin) = path;

	//����������
	LOGFONT  logf;
	this->GetFont()->GetLogFont(& logf);
	logf.lfHeight -= 1;
	logf.lfWeight *= 1.5;
	m_fontCaption.CreateFontIndirect(& logf);
	///���ô���ΪԲ�Ǵ���
	CRect rt;
	GetWindowRect(& rt);
	ScreenToClient(& rt);

	m_backPic.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper ImageHandle(&m_backPic);
	HRGN hRgn=AFCBmpToRgn(ImageHandle,0xFF00FF,RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
	}
	//���ر���ͼ
	m_rectTitle = CRect(0,0,rt.right,26);    //����������
	//��������������
	m_rectTitleText = CRect(0,0,50,33);
	m_rectTitleText.OffsetRect(18,6);
	//��ʾͼƬλ��
	GetDlgItem(IDC_STATIC_PIC)->GetWindowRect( & m_rectInfoPic);
	ScreenToClient(& m_rectInfoPic);
	wsprintf(path,"..\\%sdialog\\win_close.bmp",m_skinmgr.GetSkinPath());
	m_closeBtn.LoadButtonBitmap(path,false);
	GetClientRect(&cRect);
	bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_closeBtn.MoveWindow(cRect.right-cRect.left-bt.GetWidth()/4, 0, bt.GetWidth()/4, bt.GetHeight());
	wsprintf(path,"..\\%sdialog\\msg_button.bmp",m_skinmgr.GetSkinPath());

	if (m_msgBoxStyle == AFC_CONFIRM)  //ȷ�϶Ի���
	{  
		//		wsprintf(path,"image\\client\\%s\\dialog\\msg_button.bmp",Glb().m_skin);
		// m_infoBmpFile = MSG_KT_ORDPIC;
		m_infoBmpFile.Format("..\\%sdialog\\msg_Ordi.bmp",m_skinmgr.GetSkinPath());
		m_cancelBtn.ShowWindow(SW_HIDE);
		m_confirmBtn.LoadButtonBitmap(path,false);
		CRect rt;
		m_confirmBtn.GetWindowRect(& rt);
		ScreenToClient(& rt);
		rt.OffsetRect(60,0);
		m_confirmBtn.MoveWindow(& rt);

	}
	else if (m_msgBoxStyle == AFC_YESNO)   //��ʾ���ǡ������񡱶Ի���
	{
		m_infoBmpFile.Format("..\\%sdialog\\msg_quest.bmp",m_skinmgr.GetSkinPath());
		//m_infoBmpFile = MSG_KT_INFOPIC;
		m_confirmBtn.LoadButtonBitmap(path,false);
		m_cancelBtn.LoadButtonBitmap(path,false);
		//m_closeBtn.ShowWindow(SW_HIDE);
	}
	else if (m_msgBoxStyle == AFC_WARN)   //����Ի���
	{
		m_infoBmpFile.Format("..\\%sdialog\\msg_Info.bmp",m_skinmgr.GetSkinPath());
		//m_infoBmpFile = MSG_KT_WAINPIC;
		m_confirmBtn.ShowWindow(SW_HIDE);
		m_cancelBtn.LoadButtonBitmap(path,false);
		CRect rt;
		m_cancelBtn.GetWindowRect(& rt);
		ScreenToClient(& rt);
		rt.OffsetRect(-48,0);
		m_cancelBtn.MoveWindow(& rt);
	}    
	GetDlgItem(IDC_INFORSTR)->SetWindowText(m_infoStr);
	m_infoBmp.m_hObject = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
		m_infoBmpFile,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void ConfirmDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//����ͼ
	CRect wndRt;
	GetWindowRect(& wndRt);
	CGameImageHelper helprb(&m_backPic);
	helprb.BitBlt(dc.GetSafeHdc(),0,0);

	int nOldMode = dc.SetBkMode(TRANSPARENT);   //���ñ���Ϊ͸����
	HGDIOBJ hOldFont =  dc.SelectObject(m_fontCaption);   //��������������
	COLORREF clOldText = dc.SetTextColor(RGB(242, 253, 253));  //������������ɫ
	dc.DrawText(m_titelStr,& m_rectTitleText,DT_LEFT | DT_CALCRECT);
	dc.DrawText(m_titelStr,& m_rectTitleText,DT_LEFT);
	dc.DrawState(CPoint(m_rectInfoPic.left,m_rectInfoPic.top),CSize(m_rectInfoPic.right,m_rectInfoPic.bottom),& m_infoBmp,DST_BITMAP,NULL);
	dc.SetBkMode(nOldMode);
	dc.SetTextColor(clOldText);
	dc.SelectObject(hOldFont);
}

HBRUSH ConfirmDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HBRUSH b;
	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetBkMode(TRANSPARENT);
		static HBRUSH s_hBrush = NULL;
		if (s_hBrush==NULL)
		{
			s_hBrush = CreateSolidBrush(m_backPic.GetPixel(145,35));
		}
		return (HBRUSH)s_hBrush;
	}
	return hbr;
}

void ConfirmDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_rectTitle.PtInRect( point))
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void ConfirmDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void ConfirmDlg::OnBnClickedNo()
{
	OnCancel();
}

void ConfirmDlg::OnBnClickedClosebtn()
{
	OnCancel();
}
