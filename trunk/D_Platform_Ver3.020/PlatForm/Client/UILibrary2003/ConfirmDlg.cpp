// E:\WebKT\WebKT\UI\ConfirmDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ConfirmDlg.h"
#include ".\confirmdlg.h"
#include "AFCFunction.h"

// ConfirmDlg �Ի���

CString ConfirmDlg::m_skinKey = "skin0";

IMPLEMENT_DYNAMIC(ConfirmDlg, CDialog)
ConfirmDlg::ConfirmDlg(CString strInfo,CString strTitle/* = NULL*/,UINT style /*= AFC_CONFIRM*/,CWnd* pParent /*=NULL*/)
: CDialog(ConfirmDlg::IDD, pParent)
{
	m_infoStr = strInfo;
	m_titelStr = strTitle;
	m_msgBoxStyle = style;
	m_bkBrush = NULL;
	//    m_backPicFile = MSGBX_BK;
}

ConfirmDlg::~ConfirmDlg()
{
	m_fontCaption.DeleteObject();
	DeleteObject(m_bkBrush);
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

	wsprintf(path,"%simage\\client\\%s\\dialog\\msg_bk.bmp",Glb().m_Path,m_skinKey);
	CGameImage bt;
	CRect cRect;
	GetWindowRect(&cRect);
	bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//MoveWindow(cRect.left, cRect.top, bt.GetWidth(), bt.GetHeight(), TRUE);
	CenterWindow(AfxGetApp()->m_pMainWnd);

	//m_backPicFile.Format("image\\client\\%s\\dialog\\win_close.bmp",Glb().m_skin) = path;
	
	//*-------��ʼ�� m_configDlg--------- 2012.10.17 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%simage\\client\\%s\\dialog\\configDlg\\ConfirmDlgConfig\\configDlg.bcf",Glb().m_Path,m_skinKey);// 
	wsprintf(PathDefault,"%simage\\client\\%s\\",Glb().m_Path,m_skinKey);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	//-----------------------------------------------------*/
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

	/*// 2012.12.17 yyf ����
	m_backPic.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper ImageHandle(&m_backPic);
	HRGN hRgn=AFCBmpToRgn(ImageHandle,0xFF00FF,RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		///{{ Added by zxd 20100709 �ͷ��������
		DeleteObject(hRgn);
		///Added by zxd 20100709 �ͷ��������}}
	}*/

	//���ر���ͼ
	m_rectTitle = CRect(0,0,rt.right,26);    //����������
	//��������������
	m_rectTitleText = CRect(0,0,50,33);
	m_rectTitleText.OffsetRect(18,6);
	//��ʾͼƬλ��
	GetDlgItem(IDC_STATIC_PIC)->GetWindowRect( & m_rectInfoPic);
	ScreenToClient(& m_rectInfoPic);
	wsprintf(path,"%simage\\client\\%s\\dialog\\log_exit_bt.bmp",Glb().m_Path, m_skinKey);//win_close.bmp
	m_closeBtn.LoadButtonBitmap(path,false);
	CRect ClientRect;
	GetClientRect(&ClientRect);
	bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_closeBtn.MoveWindow(ClientRect.right-ClientRect.left-bt.GetWidth()/4 - 15, 0, bt.GetWidth()/4, bt.GetHeight());

	wsprintf(path,"%simage\\client\\%s\\dialog\\msg_button.bmp",Glb().m_Path, m_skinKey);
	if (m_msgBoxStyle == AFC_CONFIRM)  //ȷ�϶Ի���
	{  
		//		wsprintf(path,"image\\client\\%s\\dialog\\msg_button.bmp",Glb().m_skin);
		// m_infoBmpFile = MSG_KT_ORDPIC;
		m_infoBmpFile.Format("image\\client\\%s\\dialog\\msg_Ordi.bmp",m_skinKey);
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
		m_infoBmpFile.Format("image\\client\\%s\\dialog\\msg_quest.bmp",m_skinKey);
		//m_infoBmpFile = MSG_KT_INFOPIC;
		m_confirmBtn.LoadButtonBitmap(path,false);
		m_cancelBtn.LoadButtonBitmap(path,false);
		//m_closeBtn.ShowWindow(SW_HIDE);
	}
	else if (m_msgBoxStyle == AFC_WARN)   //����Ի���
	{
		m_infoBmpFile.Format("image\\client\\%s\\dialog\\msg_Info.bmp",m_skinKey);
		//m_infoBmpFile = MSG_KT_WAINPIC;
		m_confirmBtn.ShowWindow(SW_HIDE);
		m_cancelBtn.LoadButtonBitmap(path,false);
		CRect rt;
		m_cancelBtn.GetWindowRect(& rt);
		ScreenToClient(& rt);
		rt.OffsetRect(-48,0);
		m_cancelBtn.MoveWindow(& rt);
	} 
	else
	{
		m_infoBmpFile.Format("image\\client\\%s\\dialog\\msg_Info.bmp",m_skinKey);
		//m_infoBmpFile = MSG_KT_WAINPIC;
		m_confirmBtn.ShowWindow(SW_HIDE);
		m_cancelBtn.LoadButtonBitmap(path,false);
		CRect rt;
		m_cancelBtn.GetWindowRect(& rt);
		ScreenToClient(& rt);
		rt.OffsetRect(-48,0);
		m_cancelBtn.MoveWindow(& rt);
		GetDlgItem(IDC_INFORSTR)->SetWindowText("AFCMessageBox Error!");
		m_infoBmp.m_hObject = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
		m_infoBmpFile,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE);

		return TRUE;
	}
	GetDlgItem(IDC_INFORSTR)->SetWindowText(m_infoStr);
	m_infoBmp.m_hObject = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
		m_infoBmpFile,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE);
	
	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void ConfirmDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//����ͼ
	CRect wndRt;
	GetWindowRect(& wndRt);

	/*// 2012.12.17 yyf ����
	CGameImageHelper helprb(&m_backPic);
	helprb.BitBlt(dc.GetSafeHdc(),0,0);*/
	
	//�滭��ܵ�ͼ 2012.10.17 yyf
	m_configDlg.DrawViewFrame(&dc);

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
		/// {{ Modified by zxd 20100709 �޸�Ϊ���ݱ���ͼ������ˢ��
		if (m_bkBrush==NULL)
		{
			m_bkBrush = CreateSolidBrush(m_configDlg.GetBKImageEX().GetPixel(145,35));//m_backPic
		}
		/// Modified by zxd 20100709 �޸�Ϊ���ݱ���ͼ������ˢ�� }}
		return m_bkBrush;
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

void ConfirmDlg::SetSkinKey(const CString& strkey)
{
	m_skinKey = strkey;
}

// ���ô���Բ�� 2012.10.15 yyf
void ConfirmDlg::SetWndRgn(void)
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
TimerDlg::TimerDlg(int sec, CString strInfo,CString strTitle/* = "ϵͳ��ʾ"*/,UINT style/* = AFC_CONFIRM*/,CWnd* pParent/* = NULL*/)
:ConfirmDlg(strInfo, strTitle, style, pParent)
,m_sec(sec)
{
}

TimerDlg::~TimerDlg(void)
{
}

BOOL TimerDlg::OnInitDialog()
{
	ConfirmDlg::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	if (m_sec != 0)
	{
		CString szBtn;
		szBtn.Format("ȷ��(%d)",m_sec);
		m_confirmBtn.SetWindowText(szBtn);

		SetTimer(10000, 1000, NULL);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
BEGIN_MESSAGE_MAP(TimerDlg, ConfirmDlg)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void TimerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 10000)
	{
		if(m_sec == 0)
		{
			OnOK();
		}
		else
		{
			m_sec--;
			CString szBtn;
			szBtn.Format("ȷ��(%d)",m_sec);
			m_confirmBtn.SetWindowText(szBtn);
		}
	}
	ConfirmDlg::OnTimer(nIDEvent);
}

void TimerDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	ConfirmDlg::OnOK();
}