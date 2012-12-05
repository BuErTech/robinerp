#include "Stdafx.h"
#include "OleCtl.h"
#include "GamePlace.h"
#include "GamePlaceDlg.h"

#include "detours.h"
#include "skinsb.h"

#include "BzCrashRpt.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

// ��������
DETOUR_TRAMPOLINE(int   WINAPI SetScrollInfoT(HWND, int, LPCSCROLLINFO, BOOL), SetScrollInfo)
DETOUR_TRAMPOLINE(BOOL  WINAPI GetScrollInfoT(HWND, int, LPSCROLLINFO), GetScrollInfo)
DETOUR_TRAMPOLINE(int   WINAPI SetScrollPosT(HWND, int, int, BOOL), SetScrollPos)
DETOUR_TRAMPOLINE(int   WINAPI GetScrollPosT(HWND, int), GetScrollPos)
DETOUR_TRAMPOLINE(BOOL  WINAPI GetScrollRangeT(HWND, int, LPINT, LPINT), GetScrollRange)
DETOUR_TRAMPOLINE(BOOL  WINAPI SetScrollRangeT(HWND, int, int, int, BOOL), SetScrollRange)
DETOUR_TRAMPOLINE(BOOL  WINAPI ShowScrollBarT(HWND, int, BOOL), ShowScrollBar)
DETOUR_TRAMPOLINE(BOOL  WINAPI EnableScrollBarT(HWND, UINT, UINT), EnableScrollBar)


int WINAPI SetScrollInfoD(HWND hwnd, int fnBar, LPCSCROLLINFO lpsi, BOOL bRedraw)
{
	if( SkinSB_IsValid(hwnd) )
		return SkinSB_SetScrollInfo(hwnd, fnBar, lpsi, bRedraw);
	else
		return SetScrollInfoT(hwnd, fnBar, lpsi, bRedraw);
}

BOOL WINAPI GetScrollInfoD(HWND hwnd, int fnBar, LPSCROLLINFO lpsi)
{
	if( SkinSB_IsValid(hwnd) )
		return SkinSB_GetScrollInfo(hwnd, fnBar, lpsi);
	else
		return GetScrollInfoT(hwnd, fnBar, lpsi);
}

int WINAPI SetScrollPosD(HWND hwnd, int nBar, int nPos, BOOL bRedraw)
{
	if( SkinSB_IsValid(hwnd) )
		return SkinSB_SetScrollPos(hwnd, nBar, nPos, bRedraw);
	else
		return SetScrollPosT(hwnd, nBar, nPos, bRedraw);
}

int WINAPI GetScrollPosD(HWND hwnd, int nBar)
{
	if( SkinSB_IsValid(hwnd) )
		return SkinSB_GetScrollPos(hwnd, nBar);
	else
		return GetScrollPosT(hwnd, nBar);
}

BOOL WINAPI SetScrollRangeD(HWND hwnd, int nBar, int nMinPos, int nMaxPos, BOOL bRedraw)
{
	if( SkinSB_IsValid(hwnd) )
		return SkinSB_SetScrollRange(hwnd, nBar, nMinPos, nMaxPos, bRedraw);
	else
		return SetScrollRangeT(hwnd, nBar, nMinPos, nMaxPos, bRedraw);
}

BOOL WINAPI GetScrollRangeD(HWND hwnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos)
{
	if( SkinSB_IsValid(hwnd) )
		return SkinSB_GetScrollRange(hwnd, nBar, lpMinPos, lpMaxPos);
	else
		return GetScrollRangeT(hwnd, nBar, lpMinPos, lpMaxPos);
}

BOOL WINAPI ShowScrollBarD(HWND hwnd, int nBar, BOOL bShow)
{
	if( SkinSB_IsValid(hwnd) )
		return SkinSB_ShowScrollBar(hwnd, nBar, bShow);
	else
		return ShowScrollBarT(hwnd, nBar, bShow);
}

BOOL WINAPI EnableScrollBarD(HWND hwnd, UINT wSBflags, UINT wArrows)
{
	if( SkinSB_IsValid(hwnd) )
		return SkinSB_EnableScrollBar(hwnd, wSBflags, wArrows);
	else
		return EnableScrollBarT(hwnd, wSBflags, wArrows);
}

BEGIN_MESSAGE_MAP(CGamePlaceApp, CWinApp)

END_MESSAGE_MAP()

//ע��ؼ�����
typedef void (RegisterOCX)(void);

//Ӧ�ó������
CGamePlaceApp theApp;

//���캯�� 
CGamePlaceApp::CGamePlaceApp()
{
}

CGamePlaceApp::~CGamePlaceApp()
{
	CUnZipRes unZipRes;   ///<add by wxx 2010.7.2 �ͷŶ������Դ�ĸ���
	unZipRes.ClearMap();
}
//��ʼ������
BOOL CGamePlaceApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	Glb().m_release=false;//�Ƿ񷢲���
	Glb().m_weblogon=false;	//�Ƿ����ǿյ�¼
	Glb().m_autoreg=false;	//��������,�˱��������Զ��ı�


#ifndef MY_DEBUG
	Glb().m_release=true;//�Ƿ񷢲���
//	try
#endif
	{
		//�������
#ifndef MY_DEBUG
		CBcfFile _f(CBcfFile::GetAppPath()+"bzgame.bcf");
		CString _mutexId = _f.GetKeyVal("BZW","mutexid","GamePlaceChangeed10001000");

		CMutex Mutex(FALSE, _mutexId, NULL);////����
		if (Mutex.Lock(0)==FALSE)
			return FALSE;
		SetUnhandledExceptionFilter(ExceptionFilter);
#endif		


#ifdef BZ_ZLIB
		///<��ȡ��Դ�ļ�
		///<�Ȼ�ȡ����  add by wxx 
		CBcfFile fMsg(CBcfFile::GetAppPath()+"bzgame.bcf");
		CString strPassWord;
		strPassWord = fMsg.GetKeyVal("BZW","LoginIP1","www.szbzw.com");
		char *password = strPassWord.GetBuffer(strPassWord.GetLength()+1);

		///<��ȡ�ļ�
		CUnZipRes unZipRes;
		char * pPW=new char[strlen(password)+1];
		memcpy(pPW,password,strlen(password)+1);
		unZipRes.SetPassWord(pPW);

		if(!CBcfFile::IsFileExist("image.r"))///add by wxx 0712
		{
			MessageBox(NULL,"��Դ�ļ�����","��ʾ",MB_OK);
			return FALSE;
		}

		CString strPackage = CBcfFile::GetAppPath() + "image.r";
		BzDui::CPaintManagerUI::SetResourcePackage(strPackage.GetBuffer(),password);

		unZipRes.ReadFile("image.r",(unsigned char*)pPW);
        delete []pPW;
#endif
		//��ʼ��
		InitCommonControls();
		CWinApp::InitInstance();

#ifndef MY_DEV

		// ��ʼBzCrashRpt
		BzCrash_Initiation();
		BzCrash_DisableSetUnhandledExceptionFilter();
		BzCrash_SetProjectName("Dƽ̨����");
		BzCrash_SetEmailSender("bzbugrev@sina.com");
		BzCrash_SetEmailReceiver("bzbugrev@sina.com");
		BzCrash_SetSmtpServer("smtp.sina.com");
		BzCrash_SetSmtpUser("bzbugrev");
		BzCrash_SetSmtpPassword("1122334455");
		BzCrash_DeleteSended(false);
#endif

		//��ʼ�� SOCKET
		if (!AfxSocketInit())
		{
			AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
			return FALSE;
		}
		AfxEnableControlContainer();
		SetRegistryKey(IDS_REG_KEY);

		//���ó���·��
		TCHAR szModuleName[MAX_PATH];
		DWORD dwLength=GetModuleFileName(AfxGetInstanceHandle(),szModuleName,sizeof(szModuleName));
		szModuleName[dwLength-lstrlen(m_pszExeName)-lstrlen(TEXT(".EXE"))-1]=0;
		SetCurrentDirectory(szModuleName);
		CreateDirectory("CustomFace",NULL);

		CString str;
		int len=strlen(szModuleName);
		szModuleName[len]='\\';
		szModuleName[len+1]='\0';
		AfxGetApp()->WriteProfileString(TEXT("LogonInfo"),TEXT("Path"),szModuleName);
		Glb().m_Path=szModuleName;

		//��ʼ��ȫ����Դ
		AfxInitRichEdit();
		CGameImageLink::InitResource();

		// ���ع���
		DetourFunctionWithTrampoline((PBYTE)SetScrollInfoT, (PBYTE)SetScrollInfoD);
		DetourFunctionWithTrampoline((PBYTE)GetScrollInfoT, (PBYTE)GetScrollInfoD);
		DetourFunctionWithTrampoline((PBYTE)SetScrollPosT, (PBYTE)SetScrollPosD);
		DetourFunctionWithTrampoline((PBYTE)GetScrollPosT, (PBYTE)GetScrollPosD);
		DetourFunctionWithTrampoline((PBYTE)SetScrollRangeT, (PBYTE)SetScrollRangeD);
		DetourFunctionWithTrampoline((PBYTE)GetScrollRangeT, (PBYTE)GetScrollRangeD);
		DetourFunctionWithTrampoline((PBYTE)ShowScrollBarT, (PBYTE)ShowScrollBarD);
		DetourFunctionWithTrampoline((PBYTE)EnableScrollBarT, (PBYTE)EnableScrollBarD);


		//����DirectUI֧�֣���ʼ��DirectUI
		BzDui::CPaintManagerUI::SetInstance(m_hInstance);

		CString s = CBcfFile::GetAppPath ();/////����·��
		CString strSkin = m_skinmgr.GetSkinBcfFileName();
		CBcfFile f(s + strSkin);
		TCHAR szUIPath[MAX_PATH];
		CString skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());
		wsprintf(szUIPath,"%s",skinfolder);

		BzDui::CPaintManagerUI::SetResourcePath(szUIPath);

		//�����Ի��� 
		CGamePlaceDlg dlg;
		m_pMainWnd=&dlg;
		//BZUIInitial();
		dlg.DoModal();
		///////////////////////////////////////////////////////////
		///Kylin 20090107  ��������Ϸ�б�

		//CString s=CBcfFile::GetAppPath (false);/////����·��
		//CBcfFile f( s + "bzgame.bcf");
		//CString temp="";


		//try
		//{
		//	for(int i=0;i<Glb().m_baFavorSave.GetCount();i++)
		//	{
		//		temp.Format("%s%i;",temp,Glb().m_baFavorSave[i]);
		//	}
		//	f.SetKeyValString(Glb().m_key,"Favor",temp);
		//}
		//catch (...)
		//{
		//	return FALSE;
		//}
		///////////////////////////////////////////////////////////
		//BZUIShutDown();
	}
#ifndef MY_DEBUG
	//catch (...)
	//{
	//	//����������Ϸ
	//	STARTUPINFO StartInfo;
	//	PROCESS_INFORMATION Info;
	//	::memset(&Info,0,sizeof(Info));
	//	::memset(&StartInfo,0,sizeof(StartInfo));
	//	StartInfo.cb=sizeof(StartInfo);
	//	StartInfo.wShowWindow=SW_SHOWMAXIMIZED;
	//	CreateProcess(NULL,TEXT("BZW.exe"),NULL,NULL,TRUE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartInfo,&Info);
	//}
#endif

	return FALSE;
}

int CGamePlaceApp::ExitInstance()
{
	// ж�ع���
	DetourRemove((PBYTE)SetScrollInfoT, (PBYTE)SetScrollInfoD);
	DetourRemove((PBYTE)SetScrollPosT, (PBYTE)SetScrollPosD);
	DetourRemove((PBYTE)GetScrollInfoT, (PBYTE)GetScrollInfoD);
	DetourRemove((PBYTE)GetScrollPosT, (PBYTE)GetScrollPosD);
	DetourRemove((PBYTE)SetScrollRangeT, (PBYTE)SetScrollRangeD);
	DetourRemove((PBYTE)GetScrollRangeT, (PBYTE)GetScrollRangeD);
	DetourRemove((PBYTE)ShowScrollBarT, (PBYTE)ShowScrollBarD);
	DetourRemove((PBYTE)EnableScrollBarT, (PBYTE)EnableScrollBarD);

	return CWinApp::ExitInstance();
}


int CGamePlaceApp::GetModuleStatus(char* module,char* compoment,int defval/* = 0*/)
{
	//����·��
	DWORD dwCfgTalk = ::cfgOpenFile("Module.bcf",_T("szbzw!!@@##"));

	if ( dwCfgTalk >= 0x10)
	{
		int retVal = ::cfgGetValue(dwCfgTalk,module,compoment,defval); 
		::cfgClose(dwCfgTalk);
		return retVal;
		
	}
	
	return 0;
}
