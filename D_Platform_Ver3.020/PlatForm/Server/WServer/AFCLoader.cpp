#include "StdAfx.h"
#include "AFCLoader.h"
#include "MainFrm.h"

BEGIN_MESSAGE_MAP(CAFCLoaderApp, CWinApp)
END_MESSAGE_MAP()

//Ӧ�ó������
CAFCLoaderApp theApp;

//���캯��
CAFCLoaderApp::CAFCLoaderApp()
{
}

bool CAFCLoaderApp::CheckBanBen()
{
	return true;
	CString sn ="20071030";////��ֹ����
	
	long in=atol(sn.GetBuffer (sn.GetLength ()));
	if(in<=0) return false;
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
		//m_GameLogon.StoptService();
		AfxMessageBox("������������ڵ��ڣ���������ϵwww.bzw.cn��ȡ�����汾��������ʾ����ʱ�����ƣ�����˵�����������ѵ�����ȡ�����汾����ѵġ���");
		//CDialog::OnOK ();//theApp.m_pMainWnd->DestroyWindow ();
		return false;
	}
	return true;
}

//��ʼ������
BOOL CAFCLoaderApp::InitInstance()
{
	if(!CheckBanBen()) return true;
	//��ʼ��
	InitCommonControls();
	CWinApp::InitInstance();

	//��ʼ��
	if (!AfxOleInit()||!AfxSocketInit()) return FALSE;
	AfxEnableControlContainer();
	SetRegistryKey(_T("�ӷֲ�����������ϵͳ"));

	//���ó���·��
	TCHAR szModuleName[MAX_PATH];
	DWORD dwLength=GetModuleFileName(AfxGetInstanceHandle(),szModuleName,sizeof(szModuleName));
	szModuleName[dwLength-lstrlen(m_pszExeName)-lstrlen(TEXT(".EXE"))-1]=0;
	SetCurrentDirectory(szModuleName);


	//��������
	
	CMainFrame * pFrame=new CMainFrame;
	m_pMainWnd=pFrame;
	pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW,NULL,NULL);
	//pFrame->SetWindowTextA("hepengyu");//SetWindowTitle(TMLcopyright);
	//pFrame->SetTitle(TMLcopyright);
	return TRUE;
}
