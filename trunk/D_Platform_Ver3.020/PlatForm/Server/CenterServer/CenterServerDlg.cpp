// CenterServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CenterServer.h"
#include "CenterServerDlg.h"

#include ".\centerserverdlg.h"
#include "BuildTime.h"

#define IDM_TRAY_NOTIFY_MSG (WM_USER +13)

// CCenterServerDlg �Ի���


void CCenterServerDlg::CheckBanBen()
{
	return ;
	CString sn ="20071030";////��ֹ����
	
	long in=atol(sn.GetBuffer (sn.GetLength ()));
	if(in<=0)return ;
	int y1=atoi(sn.Mid (0,4)),
		m1   =atoi(sn.Mid (4,2)),
		d1=atoi(sn.Mid (6,2));

	CTime t1(y1,m1,d1,0,0,0);
	time_t ct;  
    time( &ct ) ;

	CTime t2(ct);

	CTimeSpan tsp;
	tsp=t2-t1;//// ��ǰ����  - ��ֹ����

	LONGLONG dd=tsp.GetDays ();

	if(t2>t1)//dd > 0)
	{
		KillTimer(1);
		KillTimer(2);
		KillTimer(3);
		m_CenterServerModule.StoptService();
		AfxMessageBox("������������ڵ��ڣ���������ϵwww.bzw.cn��ȡ�����汾��������ʾ����ʱ�����ƣ�����˵�����������ѵ�����ȡ�����汾����ѵġ���");
		CDialog::OnOK ();//theApp.m_pMainWnd->DestroyWindow ();
		return ;
	}
}




void OutputFun(char* message)
{
	CString s=message;
	CCenterServerDlg* p=(CCenterServerDlg*)AfxGetApp()->m_pMainWnd ;
	if(p)
		p->OutputFun(s);

}


void CCenterServerDlg::OutputFun(CString message)
{

	
	m_listSysRunInfo.DisplayMsg(COutputDisplay::Message,
		message.GetBuffer (message.GetLength ()));
		

}

CCenterServerDlg::CCenterServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCenterServerDlg::IDD, pParent), m_TrayIcon(10)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCenterServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SYS_RUN_INFO, m_listSysRunInfo);
	DDX_Control(pDX, IDC_STATIC_STATUSBAR, m_strStatusBar);
}

BEGIN_MESSAGE_MAP(CCenterServerDlg, CDialog)
	ON_WM_CREATE()////////////
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()//////////
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_MESSAGE(IDM_TRAY_NOTIFY_MSG,OnTrayIconNotify)///////////
	ON_REGISTERED_MESSAGE(WM_CREATETRAYBAR, OnCreateTray)

END_MESSAGE_MAP()
// CCenterServerDlg ��Ϣ�������
//��������
int CCenterServerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
#define APP_TITLE "���������ϵͳ"//"���뼯Ⱥ����"
	//ϵͳ�ڲ�����
	if (CDialog::OnCreate(lpCreateStruct)==-1) return -1;
	//if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,CRect(0,0,0,0),this,AFX_IDW_PANE_FIRST,NULL))	return -1;

	//����������ͼ��
	HICON hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_TrayIcon.SetNotifyWnd(this,IDM_TRAY_NOTIFY_MSG);//�����Ի���Ϳ�ʼ״̬���ϵ�icon��ϵ��
	m_TrayIcon.SetIcon(hIcon,APP_TITLE);
	//m_TrayIcon.ShowBalloonTip("�����������뼯Ⱥ����ϵͳ�����Ժ�...   ",APP_TITLE);
	SetIcon(hIcon,FALSE);
	DestroyIcon(hIcon);
	m_TrayIcon.ShowBalloonTip("�������뼯Ⱥ����ϵͳ�ɹ�",APP_TITLE);
	ShowWindow(SW_SHOW);
	return 0;
}


//������ͼ����Ϣ
LRESULT CCenterServerDlg::OnTrayIconNotify(WPARAM wID, LPARAM lEvent)
{
	switch (lEvent)
	{
	case WM_LBUTTONDBLCLK:	//˫���¼�
		{
			ShowWindow(SW_SHOW);//������ʾ
			//PostQuitMessage(0);
			return 1;
		}
	}
	return 0;
}
//λ����Ϣ
void CCenterServerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (nType==SIZE_MINIMIZED)	ShowWindow(SW_HIDE);
}

BOOL CCenterServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	///RegOutPutFun(::OutputFun);
	SetStatusBar();
	SetTimer(1,1000 ,NULL);
	SetTimer(2,2000  ,NULL);
	SetTimer(3,2000  ,NULL);

	ManageInfoStruct Init;
	memset(&Init,0,sizeof(Init));
	
	//��������
	if (!m_CenterServerModule.InitService(&Init))
	{
		AfxMessageBox("InitService Error !");
		return false;
	}
	UINT errCode;
	if (!m_CenterServerModule.StartService(errCode))
	{
		CString stip;
		stip.Format("Start Service Failed ,Error Code:%X",errCode);
		AfxMessageBox(stip);
		return false;
	}

	CheckBanBen();

	// TODO���ڴ���Ӷ���ĳ�ʼ������
	 GetDlgItem(IDC_STATIC)->SetWindowText("ϵͳ������Ϣ��");
	 GetDlgItem(IDC_STATIC1)->SetWindowText(APP_TITLE);
	// GetDlgItem(IDC_STATIC1)->SetFont
	 CString s,scaption;
	 GetWindowText(s);
	 scaption.Format("%s v%d.%d.%d %s",s,VER_MAIN,VER_MIDDLE,VER_RESVERSE,VER_BUILDTIME);
	 SetWindowText(scaption);

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCenterServerDlg::OnPaint() 
{
	//if (IsIconic())
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		///��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		dc.SetBkMode(TRANSPARENT);//͸����, ��Ȼ��, �����ı�����ɫ
		dc.SetTextColor(RGB(255, 255, 255));//��ɫ
		dc.DrawText(TMLcopyright,lstrlen(TMLcopyright),CRect(2,457,720,836),DT_TOP|DT_RIGHT|DT_NOCLIP|DT_SINGLELINE);

		dc.SetTextColor(RGB(200, 200, 200));//��ɫ
		dc.DrawText(TMLcopyright,lstrlen(TMLcopyright),CRect(1,456,720,836),DT_TOP|DT_RIGHT|DT_NOCLIP|DT_SINGLELINE);

		dc.SetTextColor(RGB(0, 0, 0));//��ɫ
		dc.DrawText(TMLcopyright,lstrlen(TMLcopyright),CRect(0,455,720,836),DT_TOP|DT_RIGHT|DT_NOCLIP|DT_SINGLELINE);
		CDialog::OnPaint();
	}

	
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CCenterServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCenterServerDlg::OnDestroy()
{
	CDialog::OnDestroy();
		// TODO: �ڴ������Ϣ����������
}

void CCenterServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch(nID)
	{
	case SC_CLOSE:
//		server.Stop();
		KillTimer(1);
		CDialog::OnSysCommand(nID, lParam);
		return;;
	}

	CDialog::OnSysCommand(nID, lParam);
}


void CCenterServerDlg::SetStatusBar()
{
	CString s1;
//	s1.Format ("��������:%d",server.GetClientCount ());
	m_strStatusBar.SetWindowText (s1);
	
}

void CCenterServerDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1)
		SetStatusBar();

	if(nIDEvent == 2)
	{
		KillTimer(2);
		ShowWindow(SW_HIDE);
	}
	if(nIDEvent ==3)
	{
		CheckBanBen();
	}

	CDialog::OnTimer(nIDEvent);
}


void CCenterServerDlg::OnBnClickedOk()
{
//	server.Stop();
		KillTimer(1);
		KillTimer(2);
	OnOK();
}

LRESULT CCenterServerDlg::OnCreateTray(WPARAM wp, LPARAM lp)
{
#define APP_TITLE "���������ϵͳ"//"���뼯Ⱥ����"
	m_TrayIcon.SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),APP_TITLE,TRUE);
	return 0;
}