// CenterServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MainServer.h"
#include "MainServerDlg.h"
#include "BuildTime.h"
#define IDM_TRAY_NOTIFY_MSG (WM_USER +17)

//#include "DlgSetup.h"

// CCenterServerDlg �Ի���

void CMainServerDlg::CheckBanBen()
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
		//KillTimer(1);
		//KillTimer(2);
		//KillTimer(3);
		m_GameLogon.StoptService();
		AfxMessageBox("������������ڵ��ڣ���������ϵwww.bzw.cn��ȡ�����汾��������ʾ����ʱ�����ƣ�����˵�����������ѵ�����ȡ�����汾����ѵġ���");
		CDialog::OnOK ();//theApp.m_pMainWnd->DestroyWindow ();
		return ;
	}
}


void CMainServerDlg::OnBnClickedSetup()
{
	//	CDlgSetup dlg;
	/*	dlg.m_pmsgSendToClient = &this->server.m_msgSendToClient ;
	if(dlg.DoModal () == IDOK)
	{
	server.m_msgSendToClient.m_iMainserverPort = atol(dlg.m_iMainserverPort);
	#define get_string(ss) ( (ss).GetBuffer((ss).GetLength ())   )
	memcpy(server.m_msgSendToClient.m_strGameSerialNO,get_string(dlg.m_strGameSerialNO),20);
	memcpy(server.m_msgSendToClient.m_strMainserverIPAddr,get_string(dlg.m_strMainserverIPAddr),20);
	memcpy(server.m_msgSendToClient.m_strHomeADDR,get_string(dlg.m_strHomeADDR),50);
	memcpy(server.m_msgSendToClient.m_strLoginNewUserADDR,get_string(dlg.m_strLoginNewUserADDR),50);
	memcpy(server.m_msgSendToClient.m_strHelpADDR,get_string(dlg.m_strHelpADDR),50);
	memcpy(server.m_msgSendToClient.m_strBBSADDR,get_string(dlg.m_strBBSADDR),50);
	memcpy(server.m_msgSendToClient.m_strHuaZhangADDR,get_string(dlg.m_strHuaZhangADDR),50);
	memcpy(server.m_msgSendToClient.m_strDownLoadSetupADDR,get_string(dlg.m_strDownLoadSetupADDR),50);
	memcpy(server.m_msgSendToClient.m_strDownLoadUpdatepADDR,get_string(dlg.m_strDownLoadUpdatepADDR),50);
	server.WriteINIFile();

	}*/
}



void OutputFun(char* message)
{
	CString s=message;
	CMainServerDlg* p=(CMainServerDlg*)AfxGetApp()->m_pMainWnd ;
	p->OutputFun(s);

}


void CMainServerDlg::OutputFun(const CString & message)
{
	m_listSysRunInfo.DisplayMsg(COutputDisplay::Message,message);
}

CMainServerDlg::CMainServerDlg(CWnd* pParent /*=NULL*/)
: CDialog(IDD_MAINSERVER_DIALOG, pParent),m_TrayIcon(10)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SYS_RUN_INFO, m_listSysRunInfo);
	DDX_Control(pDX, IDC_STATIC_STATUSBAR, m_strStatusBar);
}

BEGIN_MESSAGE_MAP(CMainServerDlg, CDialog)
	ON_WM_CREATE()////////////
	ON_WM_SIZE()//////////
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTNSTART, OnBnClickedSetup)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_MESSAGE(IDM_TRAY_NOTIFY_MSG,OnTrayIconNotify)///////////
	ON_BN_CLICKED(IDC_BTNNEWS, &CMainServerDlg::OnBnClickedBtnnews)
	ON_REGISTERED_MESSAGE(WM_CREATETRAYBAR, OnCreateTray)
END_MESSAGE_MAP()




//��������
int CMainServerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
#define APP_TITLEe "������MainServer"
	//ϵͳ�ڲ�����
	if (CDialog::OnCreate(lpCreateStruct)==-1) return -1;
	//if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,CRect(0,0,0,0),this,AFX_IDW_PANE_FIRST,NULL))	return -1;

	//����������ͼ��
	HICON hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_TrayIcon.SetNotifyWnd(this,IDM_TRAY_NOTIFY_MSG);
	m_TrayIcon.SetIcon(hIcon,APP_TITLEe);
	//m_TrayIcon.ShowBalloonTip("��������MainServer������ϵͳ�����Ժ�...   ",APP_TITLEe);
	SetIcon(hIcon,FALSE);
	DestroyIcon(hIcon);

	m_TrayIcon.ShowBalloonTip("����MainServer������ϵͳ�ɹ�",APP_TITLEe);
	ShowWindow(SW_SHOW);

	return 0;
}
//������ͼ����Ϣ
LRESULT CMainServerDlg::OnTrayIconNotify(WPARAM wID, LPARAM lEvent)
{
	switch (lEvent)
	{
	case WM_LBUTTONDBLCLK:	//˫���¼�
		{
			ShowWindow(SW_SHOW);
			//PostQuitMessage(0);
			return 1;
		}
	}
	return 0;
}
//λ����Ϣ
void CMainServerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (nType==SIZE_MINIMIZED)	ShowWindow(SW_HIDE);
}

// CCenterServerDlg ��Ϣ�������

BOOL CMainServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CheckBanBen();
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	///RegOutPutFun(::OutputFun);
	SetStatusBar();

	SetTimer(1,1000 ,NULL);
	SetTimer(2,2000 ,NULL);



	ManageInfoStruct Init;
	memset(&Init,0,sizeof(Init));

	//Զ�����ݿ�
	//DWORD dwIP=0;
	//if (m_SQLIP.GetAddress(dwIP)!=4)
	//{
	//	AfxMessageBox(TEXT("����дԶ�����ݿ� IP��"));
	//	m_SQLIP.SetFocus();
	//	return;
	//}
	lstrcpy(Init.szLogonSQLName,TEXT("sa"));
	//	GetDlgItemText(IDC_SQL_PASS,Init.szLogonSQLPass,sizeof(Init.szLogonSQLPass));
	//wsprintf(Init.szLogonSQLIP,TEXT("%d.%d.%d.%d"),FIRST_IPADDRESS(dwIP),SECOND_IPADDRESS(dwIP),THIRD_IPADDRESS(dwIP),FOURTH_IPADDRESS(dwIP));

	//�������ݿ�
	//if (m_NativeIP.GetAddress(dwIP)!=4)
	//{
	//	AfxMessageBox(TEXT("����д�������ݿ� IP��"));
	//	m_NativeIP.SetFocus();
	//	return;
	//}
	//lstrcpy(Init.szNativeSQLName,TEXT("sa"));
	//GetDlgItemText(IDC_SQL_PASS2,Init.szNativeSQLPass,sizeof(Init.szNativeSQLPass));
	//wsprintf(Init.szNativeSQLIP,TEXT("%d.%d.%d.%d"),FIRST_IPADDRESS(dwIP),SECOND_IPADDRESS(dwIP),THIRD_IPADDRESS(dwIP),FOURTH_IPADDRESS(dwIP));
	wsprintf(Init.szLogonSQLIP,TEXT("%d.%d.%d.%d"),127,0,0,1);
	wsprintf(Init.szNativeSQLIP,TEXT("%d.%d.%d.%d"),127,0,0,1);
	//��������
	if (!m_GameLogon.InitService(&Init))
	{
		OutputFun("��ʼ��������ʧ�ܣ�");
		AfxMessageBox("InitService Error !");
		return false;
	}
	UINT	errCode;
	if (!m_GameLogon.StartService(errCode))
	{
		CString stip;
		stip.Format("Start Service Failed ,Error Code:%X",errCode);
		OutputFun(stip);
		AfxMessageBox(stip);
		return false;
	}

	//	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	////////////////////////////////////
	////Kylin 20090316��ӷ�ֹ�˻�ͬʱ��½

	HANDLE hDatabase=m_GameLogon.m_LogonManage.m_SQLDataManage.hDBLogon;
	HANDLE hRecord=sqlSPSetName(hDatabase,"SP_ClearLoginRecord");
	if(hRecord<(HANDLE)0x1000)
		return false;
	sqlSPAddPara(hDatabase,hRecord,"@ZID",m_GameLogon.m_LogonManage.m_ID);
	sqlSPExec(hDatabase,hRecord);
	sqlSPClose(hDatabase,hRecord);
	////////////////////////////////////

	CString s;
	s.Format("MainServer�����ɹ� ֧���������:%d Port:%d   Spe:110 Tec:29001  ��ͨ����  ������� ",
		m_GameLogon.m_LogonManage.m_uMaxPeople,m_GameLogon.m_LogonManage.m_nPort );

	OutputFun(s);

	//GetDlgItem(IDC_EDIT3)->SetWindowText(s);//"�����ɹ�\r\nPort:5050\r\nSpe:110 Tec:29001\r\n��ͨ����\r\n�������\r\n");

	CString scaption;
	GetWindowText(s);
	scaption.Format("%s v%d.%d.%d %s",s,VER_MAIN,VER_MIDDLE,VER_RESVERSE,VER_BUILDTIME);
	SetWindowText(scaption);


	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMainServerDlg::OnPaint() 
{
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
	{	///��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ
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
HCURSOR CMainServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMainServerDlg::OnDestroy()
{
	m_GameLogon.UnInitService();
	CDialog::OnDestroy();

	// TODO: �ڴ������Ϣ����������
}

void CMainServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void CMainServerDlg::SetStatusBar()
{
	CString s1;
	//	s1.Format (" ��������:%d",server.GetClientCount ());
	m_strStatusBar.SetWindowText (s1);

}

void CMainServerDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1)
		SetStatusBar();

	if(nIDEvent == 2)
	{
		KillTimer(2);
		ShowWindow(SW_HIDE);
	}

	CDialog::OnTimer(nIDEvent);
}


void CMainServerDlg::OnBnClickedOk()
{
	//	server.Stop();
	KillTimer(1);
	OnOK();
}


void CMainServerDlg::OnBnClickedBtnnews()
{
	m_GameLogon.m_LogonManage.m_TCPSocket.SendDataBatch(NULL,0,MDM_GP_MESSAGE,ASS_GP_NEWS_SYSMSG,0);
	return;
}

LRESULT CMainServerDlg::OnCreateTray(WPARAM wp, LPARAM lp)
{
#define APP_TITLE "������MainServer"
	m_TrayIcon.SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),APP_TITLE,TRUE);
	return 0;
}