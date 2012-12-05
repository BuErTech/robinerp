#include "StdAfx.h"
#include "LogonDialog.h"
#include "GamePlaceDlg.h"
#include <io.h>
#include <algorithm>

#include <MMSystem.h>
#pragma comment(lib,"winmm.lib")

using namespace BzDui;

//������Կ����
#define KEY_LENGTH		10
#define IDT_SET_FOCUS	2

// PengJiLin, 2010-6-2, Ÿ�� ��¼
extern BOOL g_bRunBySoftware;
extern BOOL g_bAutoConnect;
extern TCHAR g_chUserName[100];
extern TCHAR g_chMD5PSW[100];

CString g_strAppName;

//ע������
#define REG_NAME		TEXT("Software\\BZWϵͳ\\Login\\%s\\UserName")
#define REG_ACC			TEXT("Software\\BZWϵͳ\\Login\\%s\\ACC")
#define REG_MOBILE		TEXT("Software\\BZWϵͳ\\Login\\%s\\MOBILE")
#define REG_ID			TEXT("Software\\BZWϵͳ\\Login\\%s\\ID")
#define REG_SERVER		TEXT("Software\\BZWϵͳ\\Login\\%s\\Logon_Server")

/*******************************************************************************************************/

//�û���Ϣ
bool		CLogonDialog::m_bInit=false;					//�Ƿ��ʼ����־
BYTE		CLogonDialog::m_bRemberPass=FALSE;				//��ס����
BYTE		CLogonDialog::m_bUseProxy=FALSE;				//ʹ�ô���
BYTE		CLogonDialog::m_bRegUser=FALSE;					//�����û�
BYTE		CLogonDialog::m_bLogonType=LK_BY_NAME;			//��½ģʽ
BYTE		CLogonDialog::m_bBoy=TRUE;						//�Ƿ��к�
UINT		CLogonDialog::m_bLogoID=5;						//ͷ�� ID
TCHAR		CLogonDialog::m_szName[61];						//�û�����
TCHAR		CLogonDialog::m_szScrPass[61];					//�û�����
TCHAR		CLogonDialog::m_szGameServer[51];				//��Ϸ IP
char		CLogonDialog::m_szRegSQL[210];					//",email='123'"
char		CLogonDialog::m_szMD5Pass[50];					//���ܺ������
int			CLogonDialog::m_dwAccID=0;						//ACC ����
int			CLogonDialog::m_dwMobile=0;						//�ֻ�����
TCHAR		CLogonDialog::m_nickname[100];					//�س�
TCHAR		CLogonDialog::m_szRecommenderName[61];			//���Ƽ���������
TCHAR		CLogonDialog::m_szIDCard[64];					//֤�����
TCHAR		CLogonDialog::m_szMobileVCode[8];				//�ֻ���֤��
int			CLogonDialog::m_iUserID = -1;					//�û�ID
//������Ϣ
BYTE		CLogonDialog::m_bUserCheck=FALSE;				//��Ҫ����
BYTE		CLogonDialog::m_bSock5=TRUE;					//SOCK 5 ��־
UINT		CLogonDialog::m_uProxyPort=1080;				//����˿�
TCHAR		CLogonDialog::m_szProxyServer[51];				//���� IP
TCHAR		CLogonDialog::m_szProxyName[61];				//��������
TCHAR		CLogonDialog::m_szProxyPass[61];				//��������
bool		CLogonDialog::m_bIsShow=false;
bool		CLogonDialog::m_bUpdatedConnFile=false;

//�������б�
std::vector<CLogonDialog::TSvrNode> CLogonDialog::m_vtServer;

CLogonDialog::CLogonDialog(void)
{
	m_bshow = true;
	LoadRegisterData();//��ȡע�����Ϣ
	
	bMouseDown=false;
	szLogAdvUrl="";
	m_bkBrush = NULL;
	m_bIsShow = true;

	::memset(m_szIDCard,NULL,sizeof(m_szIDCard));
	::memset(m_szMobileVCode,NULL,sizeof(m_szMobileVCode));

	//1��ʾȷ����0��ʾȡ��
	m_nRetFlag = 0;

	m_vtServer.clear();

	//��ȡexe��
	char szAppName[MAX_PATH] = {0};
	GetModuleFileName(theApp.m_hInstance,szAppName,MAX_PATH);
	g_strAppName = szAppName;
	g_strAppName = g_strAppName.Right(g_strAppName.GetLength() - g_strAppName.ReverseFind('\\') - 1);
	g_strAppName = g_strAppName.Left(g_strAppName.ReverseFind('.'));


}

CLogonDialog::~CLogonDialog(void)
{
	m_bIsShow = false;
}

LPCTSTR CLogonDialog::GetWindowClassName() const 
{ 
	return _T("UIFrame");
};

UINT CLogonDialog::GetClassStyle() const 
{ 
	return UI_CLASSSTYLE_DIALOG; 
};

void CLogonDialog::OnFinalMessage(HWND hWnd) 
{ 
	m_pm.RemovePreMessageFilter(this);
};

LRESULT CLogonDialog::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	m_pm.AddPreMessageFilter(this);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("logon\\LogonDlg.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	m_bLoginByID = CGamePlaceApp::GetModuleStatus("UserSecurity","LoginByID",0);

	Init();
	return 0;
}

LRESULT CLogonDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CLogonDialog::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CLogonDialog::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CLogonDialog::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CLogonDialog::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pm.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
	{
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
			if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CLogonDialog::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CLogonDialog::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == IDT_SET_FOCUS)
	{
		::KillTimer(m_hWnd,IDT_SET_FOCUS);
		m_pIDEdit->SetFocus();
		m_pIDEdit->SetSel(-1);
	}

	return 0;
}

LRESULT CLogonDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch(uMsg) 
	{
		case WM_CREATE:
			{
				 lRes = OnCreate(uMsg, wParam, lParam, bHandled); 
				 ChangeSkin();
			}
			break;

		case WM_TIMER:
			{
				lRes = OnTimer(uMsg, wParam, lParam, bHandled);
			}
			break;

		case WM_DESTROY:       
			{
				 lRes = OnDestroy(uMsg, wParam, lParam, bHandled);
			}
			break;

		case WM_NCACTIVATE:    
			{
				 lRes = OnNcActivate(uMsg, wParam, lParam, bHandled);
			}
			break;

		case WM_NCCALCSIZE:    
			{
				 lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled);
			}
			break;

		case WM_NCPAINT:       
			{
				lRes = OnNcPaint(uMsg, wParam, lParam, bHandled);
			}
			break;

		case WM_NCHITTEST:     
			{
				 lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled);
			}
			break;

		case WM_SIZE:          
			{
				lRes = OnSize(uMsg, wParam, lParam, bHandled);
			}
			break;

		default:
			bHandled = FALSE;
	}

	if(bHandled)
	{
		return lRes;
	}

	if(m_pm.MessageHandler(uMsg,wParam,lParam,lRes))
	{
		return lRes;
	}

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CLogonDialog::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_KEYDOWN ) 
	{
		if( wParam == VK_RETURN ) 
		{
			OnOK();
			return true;
		}
		else if( wParam == VK_ESCAPE ) 
		{
			m_nRetFlag = 0;
			Close();
			return true;
		}
	}
	return false;
}

void CLogonDialog::Init() 
{

	::SetWindowPos(m_hWnd,CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

	//�����ش�������
	CGamePlaceDlg* pPlaceDlg = (CGamePlaceDlg*)AfxGetMainWnd();
	pPlaceDlg->ShowWindow(SW_HIDE);

	LoadNormalCtrlSkin();

	CBcfFile f(CBcfFile::GetAppPath() + "bzgame.bcf");

	m_bisgetid = false;

	//Serverѡ���
	m_pServerCob = static_cast<CComboUI*>(m_pm.FindControl("Cob_Servers"));

	//ID�����
	m_pUserIDCob = static_cast<CComboUI*>(m_pm.FindControl("Cob_ID"));
	m_pIDEdit = static_cast<CEditUI*>(m_pm.FindControl("Edt_ID"));

	m_pBkImgae = static_cast<CControlUI*>(m_pm.FindControl("ControlUI1"));


	//���������
	m_pPwdEdit = static_cast<CEditUI*>(m_pm.FindControl("Edt_PWD"));

	m_pThreetName = static_cast<CButtonUI*>(m_pm.FindControl("But_LogonByName"));
	m_pThreetID = static_cast<CButtonUI*>(m_pm.FindControl("But_LogonByID"));

	if (!m_bLoginByID)
	{
		m_pThreetID->SetEnabled(false);
		m_pThreetID->SetVisible(false);
	}
	
		

	m_pIDEdit->SetMaxChar(32);
	if (m_bLogonType == LK_BY_ID)
	{
		m_pIDEdit->SetMaxChar(10);
		m_pBkImgae->SetBkImage("logon\\idbk.png");
		m_pThreetName->SetEnabled(true);
		m_pThreetID->SetEnabled(false);
	}
	if (m_bLogonType == LK_BY_NAME)
	{
		m_pIDEdit->SetMaxChar(32);
		m_pBkImgae->SetBkImage("logon\\namebk.png");
		m_pThreetName->SetEnabled(false);
		m_pThreetID->SetEnabled(true);
	}

	//��ס����ѡ���
	m_pSavePwd  = static_cast<COptionUI*>(m_pm.FindControl("Opt_SavePwd"));

	m_pSavePwd->Selected(m_bRemberPass);

	if(!Glb().m_release)
	{
		m_pSavePwd->Selected(true);
		m_bRemberPass = true;
	}

	//ͬ������
	COptionUI* pAgreeCtrl  = static_cast<COptionUI*>(m_pm.FindControl("Opt_Argeement"));
	pAgreeCtrl->Selected(true);

	//����ṩ������ѡ������ط������б�/////////////////////////////////////////////////////
	//JianGuankun 2012.03.27

	bool bIPSelect = f.GetKeyVal("BZW","LoginIPSelect",false);
	if (!bIPSelect)
	{
		m_pServerCob->SetVisible(false);
	}

	//���������Ϣ
	FillServer(m_pServerCob);

	if (!m_bUpdatedConnFile)
	{
		if (bIPSelect)
		{
			m_pServerCob->SetEnabled(false);
			m_bUpdatedConnFile = true;

			//��������bzconn.bcf�ļ��߳�
			//JianGuankun 2012.3.22
			DWORD dwThreadID = 0;
			HANDLE hThread = CreateThread(0,0,ThreadDownConnFile,this,0,&dwThreadID);
			if (hThread)
			{
				::CloseHandle(hThread);
			}
		}
	}
	else
	{
		m_pServerCob->SetEnabled(true);
	}

	//End Add////////////////////////////////////////

	//���ؼ���Ϣ
	FillUserName(m_pUserIDCob, m_pIDEdit);

	SetLogonType(m_bLogonType,m_bLogonType);

	::SetTimer(m_hWnd,IDT_SET_FOCUS,200,NULL);

	// PengJiLin, 2010-6-2, ���� Ÿ�� ��¼����
	if((TRUE == g_bRunBySoftware) && (TRUE == g_bAutoConnect))
	{
		m_pServerCob->SetText(g_chUserName);
		m_pPwdEdit->SetText(TEXT("123456"));       // �����ٵģ��������

		//OnOK();     // �Զ���¼

		lstrcpy(m_szMD5Pass, g_chMD5PSW);   // ���ں��棬���ⱻ�޸�
		g_bAutoConnect = FALSE;             // ֻ��һ���Զ���¼

		return;
	}

	//���ص�½�򶥲���ʾ����ҳ
	CString szLogAdvUrl = f.GetKeyVal (Glb().m_key,"logonadvertise","about:blank");
	CActiveXUI* pActiveXUI = static_cast<CActiveXUI*>(m_pm.FindControl(_T("IE_Logon")));
	if( pActiveXUI ) {
		IWebBrowser2* pWebBrowser = NULL;
		pActiveXUI->GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);
		if( pWebBrowser != NULL ) 
		{
			pWebBrowser->Navigate(CA2W(szLogAdvUrl),NULL,NULL,NULL,NULL);  
			pWebBrowser->Release();
		}
	}

}

void CLogonDialog::InitSkin()
{
	LoadNormalCtrlSkin();
	CBcfFile f(CBcfFile::GetAppPath() + "bzgame.bcf");
	m_bisgetid = false;

	//m_pServerCob = static_cast<CComboUI*>(m_pm.FindControl("Cob_Servers"));//Serverѡ���
	//m_pUserIDCob = static_cast<CComboUI*>(m_pm.FindControl("Cob_ID"));//ID�����
	//m_pIDEdit = static_cast<CEditUI*>(m_pm.FindControl("Edt_ID"));
	//m_pBkImgae = static_cast<CControlUI*>(m_pm.FindControl("ControlUI1"));
	//m_pPwdEdit = static_cast<CEditUI*>(m_pm.FindControl("Edt_PWD"));//���������
	//m_pThreetName = static_cast<CButtonUI*>(m_pm.FindControl("But_LogonByName"));
	//m_pThreetID = static_cast<CButtonUI*>(m_pm.FindControl("But_LogonByID"));

	if (!m_bLoginByID)
	{
		m_pThreetID->SetEnabled(false);
		m_pThreetID->SetVisible(false);
	}

	m_pIDEdit->SetMaxChar(32);
	if (m_bLogonType == LK_BY_ID)
	{
		m_pIDEdit->SetMaxChar(10);
		m_pBkImgae->SetBkImage("logon\\idbk.png");
		m_pThreetName->SetEnabled(true);
		m_pThreetID->SetEnabled(false);
	}
	if (m_bLogonType == LK_BY_NAME)
	{
		m_pIDEdit->SetMaxChar(32);
		m_pBkImgae->SetBkImage("logon\\namebk.png");
		m_pThreetName->SetEnabled(false);
		m_pThreetID->SetEnabled(true);
	}

	//��ס����ѡ���
	//m_pSavePwd  = static_cast<COptionUI*>(m_pm.FindControl("Opt_SavePwd"));
	m_pSavePwd->Selected(m_bRemberPass);
	if(!Glb().m_release)
	{
		m_pSavePwd->Selected(true);
		m_bRemberPass = true;
	}

	//ͬ������
	COptionUI* pAgreeCtrl  = static_cast<COptionUI*>(m_pm.FindControl("Opt_Argeement"));
	pAgreeCtrl->Selected(true);

	//����ṩ������ѡ������ط������б�/////////////////////////////////////////////////////
	//JianGuankun 2012.03.27

	bool bIPSelect = f.GetKeyVal("BZW","LoginIPSelect",false);
	if (!bIPSelect)
	{
		m_pServerCob->SetVisible(false);
	}

	//���������Ϣ
	FillServer(m_pServerCob);

	if (!m_bUpdatedConnFile)
	{
		if (bIPSelect)
		{
			m_pServerCob->SetEnabled(false);
			m_bUpdatedConnFile = true;

			//��������bzconn.bcf�ļ��߳�
			//JianGuankun 2012.3.22
			DWORD dwThreadID = 0;
			HANDLE hThread = CreateThread(0,0,ThreadDownConnFile,this,0,&dwThreadID);
			if (hThread)
			{
				::CloseHandle(hThread);
			}
		}
	}
	else
	{
		m_pServerCob->SetEnabled(true);
	}

	//End Add////////////////////////////////////////

	//���ؼ���Ϣ
	//FillUserName(m_pUserIDCob, m_pIDEdit);

	//SetLogonType(m_bLogonType,m_bLogonType);

	::SetTimer(m_hWnd,IDT_SET_FOCUS,200,NULL);

	// PengJiLin, 2010-6-2, ���� Ÿ�� ��¼����
	if((TRUE == g_bRunBySoftware) && (TRUE == g_bAutoConnect))
	{
		m_pServerCob->SetText(g_chUserName);
		m_pPwdEdit->SetText(TEXT("123456"));       // �����ٵģ��������

		//OnOK();     // �Զ���¼

		lstrcpy(m_szMD5Pass, g_chMD5PSW);   // ���ں��棬���ⱻ�޸�
		g_bAutoConnect = FALSE;             // ֻ��һ���Զ���¼

		return;
	}

	//���ص�½�򶥲���ʾ����ҳ
	CString szLogAdvUrl = f.GetKeyVal (Glb().m_key,"logonadvertise","about:blank");
	CActiveXUI* pActiveXUI = static_cast<CActiveXUI*>(m_pm.FindControl(_T("IE_Logon")));
	if( pActiveXUI ) {
		IWebBrowser2* pWebBrowser = NULL;
		pActiveXUI->GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);
		if( pWebBrowser != NULL ) 
		{
			pWebBrowser->Navigate(CA2W(szLogAdvUrl),NULL,NULL,NULL,NULL);  
			pWebBrowser->Release();
		}
	}

}
//��ȡע�����Ϣ
bool CLogonDialog::LoadRegisterData()
{
	if (m_bInit==false)
	{
		//��ȡ����
		srand((unsigned int)time(NULL));
		m_bLogoID=rand()%(LOGO_COUNT-1)+1;
		m_bUseProxy=AfxGetApp()->GetProfileInt(TEXT("LogonInfo"),TEXT("UseProxy"),FALSE);
		m_bLogonType=AfxGetApp()->GetProfileInt(TEXT("LogonInfo"),TEXT("LogonType"),LK_BY_NAME);
		m_bRemberPass=AfxGetApp()->GetProfileInt(TEXT("LogonInfo"),TEXT("RemPass"),FALSE);

		//��Ϸ����������
		CString RegString=AfxGetApp()->GetProfileString(TEXT("LogonInfo"),TEXT("LastLogonServer"),NULL);


		//((CComboBox *)GetDlgItem(IDC_SERVER_LIST))->AddString(RegString);
		//lstrcpy(m_szGameServer,RegString.GetBuffer());
		//RegString.ReleaseBuffer();
		//m_szGameServer[sizeof(m_szGameServer)/sizeof(m_szGameServer[0])-1]=0;

		//�û�����
		RegString=AfxGetApp()->GetProfileString(TEXT("LogonInfo"),TEXT("UserName"),NULL);
		lstrcpy(m_szName,RegString.GetBuffer());
		RegString.ReleaseBuffer();
		m_szName[sizeof(m_szName)/sizeof(m_szName[0])-1]=0;

		//if (m_bUseProxy)	//������Ϣ
		//{
		//	m_uProxyPort=AfxGetApp()->GetProfileInt(TEXT("ProxyInfo"),TEXT("ProxyPort"),1080);
		//	m_bUserCheck=AfxGetApp()->GetProfileInt(TEXT("ProxyInfo"),TEXT("ProxyCheck"),FALSE);
		//	m_bSock5=AfxGetApp()->GetProfileInt(TEXT("ProxyInfo"),TEXT("SOCK5"),TRUE);

		//	//�������������
		//	RegString=AfxGetApp()->GetProfileString(TEXT("ProxyInfo"),TEXT("ProxyServer"),TEXT("192.168.0.1"));
		//	lstrcpy(m_szProxyServer,RegString.GetBuffer());
		//	RegString.ReleaseBuffer();
		//	m_szProxyServer[sizeof(m_szProxyServer)/sizeof(m_szProxyServer[0])-1]=0;

		//	if (m_bUserCheck)
		//	{
		//		//�����û�����
		//		RegString=AfxGetApp()->GetProfileString(TEXT("ProxyInfo"),TEXT("ProxyUserName"),TEXT("Name"));
		//		lstrcpy(m_szProxyName,RegString.GetBuffer());
		//		RegString.ReleaseBuffer();
		//		m_szProxyName[sizeof(m_szProxyName)/sizeof(m_szProxyName[0])-1]=0;

		//		//�����û�����
		//		RegString=AfxGetApp()->GetProfileString(TEXT("ProxyInfo"),TEXT("ProxyUserPass"),TEXT(""));
		//		lstrcpy(m_szProxyPass,RegString.GetBuffer());
		//		RegString.ReleaseBuffer();
		//		m_szProxyPass[sizeof(m_szProxyPass)/sizeof(m_szProxyPass[0])-1]=0;
		//	}
		//}
		m_bInit=true;
	}
	return true;
}

//����Combol���б���
void CLogonDialog::LoadComboItems(CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect/* = 0*/)
{
	if (!pCtrl || nums <= 0)
	{
		return;
	}

	for (int i = 0; i < nums; ++i)
	{
		CListLabelElementUI* p = new CListLabelElementUI();
		p->SetText(items[i]);
		pCtrl->Add(p);
	}

	return;
}


//���õ�½ģʽ
bool CLogonDialog::SetLogonType(BYTE bNewLogonType, BYTE bOldLogonType)
{
// 	BYTE uLogonKind[3]={LK_BY_NAME,LK_BY_ACC,LK_BY_MOBILE};
// 	UINT uButtonConID[3]={IDC_ACCOUNTS,IDC_ACC,IDC_MOBILE};
// 	UINT uInputConID[3]={IDC_NAME_INFO,IDC_ACC_INFO,IDC_MOBILE_INFO};
	if (bNewLogonType == 0)
	{

	
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		//	TCHAR * szTipText[3]={TEXT("�û����ƣ�"),TEXT("�û����ƣ�"),TEXT("�û����ƣ�")};
		TCHAR szTipText[3][128];
		CString strMessage = fMsg.GetKeyVal("LogonDlg","Name","�û����ƣ�");
		StrCpyN(&szTipText[0][0],strMessage.GetBuffer(),strMessage.GetLength()+1);
		StrCpyN(&szTipText[1][0],strMessage.GetBuffer(),strMessage.GetLength()+1);
		StrCpyN(&szTipText[2][0],strMessage.GetBuffer(),strMessage.GetLength()+1);

	// 	for (BYTE i=0;i<3;i++)
	// 	{
	// 		//��������
	// 		if ((bNewLogonType!=bOldLogonType)&&(bOldLogonType==uLogonKind[i]))
	// 		{
	// 			((CButton *)GetDlgItem(uButtonConID[i]))->SetCheck(BST_UNCHECKED);
	// 			GetDlgItem(uInputConID[i])->ShowWindow(SW_HIDE);
	// 		}
	// 
	// 		//��ʾ����
	// 		if (bNewLogonType==uLogonKind[i])
	// 		{
	// 			//���ÿؼ�
	// 			m_bLogonType=bNewLogonType;
	// 			CComboBox * pCommboBoxInput=(CComboBox *)GetDlgItem(uInputConID[i]);
	// 			((CButton *)GetDlgItem(uButtonConID[i]))->SetCheck(BST_CHECKED);
	// 			pCommboBoxInput->ShowWindow(SW_SHOW);
	// 			pCommboBoxInput->SetFocus();
	// 
	// 			//��������
	// 			CString strInputInfo;
	// 			if ((pCommboBoxInput->GetWindowTextLength()==0)&&(pCommboBoxInput->GetCount()>0)) pCommboBoxInput->SetCurSel(0);
	// 			pCommboBoxInput->GetWindowText(strInputInfo);
	// 			UpdatePassWord(strInputInfo);
	// 		}
	// 	}

		//���ÿؼ�
		m_bLogonType=bNewLogonType;
		m_pIDEdit->SetFocus();

		//��������
		CString strInputInfo;
	
		if ((m_pUserIDCob->GetText().GetLength() == 0) && (m_pUserIDCob->GetCount() > 0))
		{
			m_pUserIDCob->SelectItem(0);
		}
		strInputInfo = m_pIDEdit->GetText();
		UpdatePassWord(strInputInfo);

		return true;
	}
	else if (bNewLogonType == 3)
	{
		//���ÿؼ�
		m_bLogonType=bNewLogonType;
		m_pIDEdit->SetFocus();

		//��������
		CString strInputInfo;

		if ((m_pUserIDCob->GetText().GetLength() == 0) && (m_pUserIDCob->GetCount() > 0))
		{
			m_pUserIDCob->SelectItem(0);
		}
		strInputInfo = m_pIDEdit->GetText();
		UpdatePassWord(strInputInfo);
	}
}

//��д��½������
//Modified By JianGuankun 2012.3.21
bool CLogonDialog::FillServer(BzDui::CComboUI* pComBoxName)
{
	//��½�����п����ٹرյ�״̬�»��ᱻ����FillServer,\
	  ����·������б���߳�ThreadDownConnFile�����FillServer,����pComBoxNameʹ���쳣��\
	  ������m_bIsShow = false����������ܱ�������

	if (!pComBoxName || !m_bIsShow)
	{
		return false;
	}

	pComBoxName->RemoveAll();

	m_vtServer.clear();
	CString sApp = CBcfFile::GetAppPath();
	TCHAR path[MAX_PATH];

	_stprintf(path,"%sbzconn.bcf",sApp.GetBuffer());

	//���û��bconn.bcf�ļ�����ô��bagame.bcf�еı�����LoginIP1////////////////////////////
	bool bSuccessGet = false;
	if (_access(path,0) != -1)
	{
		DWORD fHandle = ::cfgOpenFile(path);
		int iSvrNums = ::cfgGetValue(fHandle,_T("Info"),_T("ServerNums"),0);
		if (iSvrNums > 0)
		{
			bSuccessGet = true;
			for (int i = 0; i < iSvrNums; ++i)
			{
				char szKey[32];
				_stprintf(szKey,_T("Server_%02d"),i + 1);

				TSvrNode tagNode;

				CString strIP;
				CString strName;
				strIP   = ::cfgGetValue(fHandle,szKey,_T("LoginIP"),"127.0.0.1");
				strName = ::cfgGetValue(fHandle,szKey,_T("Name"),"(δ֪������)");

				if (strIP.GetLength() >= sizeof(tagNode.szLoginIP)
						|| strName.GetLength() >= sizeof(tagNode.szName))
				{
					continue;
				}
				
				::strcpy(tagNode.szLoginIP,strIP.GetBuffer());
				::strcpy(tagNode.szName,strName.GetBuffer());
				m_vtServer.push_back(tagNode);
			}
		}
		::cfgClose(fHandle);

		//�������ʹ��Bcf��API����ʹ��CBcfFile�࣬��������CBcfFile�����ڴ滺�壬\
			��������������bzconn.bcf�ļ�һ�£�marked by JianGuankun 2012.3.22
	}

	if (!bSuccessGet)
	{
		_stprintf(path,"%sbzgame.bcf",sApp.GetBuffer());
		CBcfFile f(path);

		CString strIP = f.GetKeyVal(_T("BZW"),_T("LoginIP1"),"127.0.0.1");
		TSvrNode tagNode;
		if (strIP.GetLength() < sizeof(tagNode.szLoginIP))
		{
			::strcpy(tagNode.szLoginIP,strIP.GetBuffer());
			::strcpy(tagNode.szName,_T(" Ĭ�Ϸ�����"));
			m_vtServer.push_back(tagNode);
		}
	}
	/////////////

	//��m_lstServer��˳����ӵ�combol��
	std::vector<TSvrNode>::iterator it = m_vtServer.begin();
	while(it != m_vtServer.end())
	{
		TSvrNode* pNode = (TSvrNode*)&*(it++);
		if (pNode)
		{
			CListLabelElementUI* p = new CListLabelElementUI();
			char szSrvInfo[128];
			if (!CLogonDialog::m_bUpdatedConnFile)
			{
				_stprintf(szSrvInfo,"��ȡ�������б�...");
			}
			else
			{
				_stprintf(szSrvInfo,"%s",pNode->szName);
			}
			
			p->SetText(szSrvInfo);
			pComBoxName->Add(p);
		}
	}

	if (CLogonDialog::m_bUpdatedConnFile)
	{
		pComBoxName->SetEnabled(true);
	}

	//ѡ��B�������ṩ��ѵ�Z��������ַ
	int nSelIdx = 0;
	it = m_vtServer.begin();
	while(it != m_vtServer.end())
	{
		TSvrNode* pNode = (TSvrNode*)&*(it++);
		if (pNode)
		{
			if(::strcmp(pNode->szLoginIP,Glb().m_CenterServerPara.m_strMainserverIPAddr) == 0)
			{
				break;
			}
			nSelIdx++;
		}
	}
	pComBoxName->SelectItem(nSelIdx);

	return true;
}


//��������
bool CLogonDialog::UpdatePassWord(CString strInputInfo)
{
	//������ʾ����
	CString strSourcePass = GetUserPass(strInputInfo,m_bLogonType);

	if (strSourcePass.GetLength()<sizeof(m_szScrPass)/sizeof(m_szScrPass[0])) lstrcpy(m_szScrPass,strSourcePass);
	else m_szScrPass[0]=0;

	m_pPwdEdit->SetText(m_szScrPass);

	return true;
}

//��д�û�
bool CLogonDialog::FillUserName(BzDui::CComboUI * pComBoxName, BzDui::CEditUI* pEditName)
{
	CRegKey NameList;

	CString strRegKey;
	if (m_bLogonType==LK_BY_NAME)	strRegKey.Format(REG_NAME,g_strAppName.GetBuffer());
	else if (m_bLogonType==LK_BY_ACC) strRegKey.Format(REG_ACC,g_strAppName.GetBuffer());
	else if (m_bLogonType==LK_BY_MOBILE) strRegKey.Format(REG_MOBILE,g_strAppName.GetBuffer());
	else if (m_bLogonType == LK_BY_ID) strRegKey.Format(REG_ID,g_strAppName.GetBuffer());

	NameList.Create(HKEY_CURRENT_USER, strRegKey);/////////����ע����û�

	std::vector<std::pair<string,DWORD>> vtUList;
	vtUList.clear();

	if (NameList != NULL)
	{
		DWORD uIndex=0;
		do
		{
			TCHAR szSubItemName[61]=TEXT("");
			DWORD dwBufferSize=sizeof(szSubItemName);
			if (NameList.EnumKey(uIndex++,szSubItemName,&dwBufferSize) != ERROR_SUCCESS) break;
			if (szSubItemName[0]!=0) 
			{
				vtUList.push_back(std::pair<string,DWORD>(szSubItemName,GetUserLogonTime(szSubItemName,m_bLogonType)));
			}
		} while (TRUE);
	}

	auto cmp_fun = [](std::pair<string,DWORD> u1,std::pair<string,DWORD> u2)
	{
		return u1.second > u2.second;
	};
	sort(vtUList.begin(),vtUList.end(),cmp_fun);

	if (vtUList.size() > 0)
	{
		pComBoxName->RemoveAll();

		if (pEditName != NULL)
			pEditName->SetText("");

		for (int i = 0; i < vtUList.size(); ++i)
		{
			CListLabelElementUI* p = new CListLabelElementUI();
			p->SetText(vtUList[i].first.c_str());
			pComBoxName->Add(p);
		}

		::strcpy(m_szName,vtUList[0].first.c_str());
	}
	
	return true;
}

//��ȡע���������Ϣ
CString CLogonDialog::GetUserPass(const TCHAR * szKindInfo, UINT uType)
{
	//��ȡע���
	if ((szKindInfo!=NULL)&&(szKindInfo[0]!=0))
	{
		//����ע����ַ�
		CString str;
		if (uType==LK_BY_NAME)	str.Format(REG_NAME,g_strAppName.GetBuffer());
		else if (uType==LK_BY_ACC) str.Format(REG_ACC,g_strAppName.GetBuffer());
		else if (uType==LK_BY_MOBILE) str.Format(REG_MOBILE,g_strAppName.GetBuffer());
		else if (uType == LK_BY_ID) str.Format(REG_ID,g_strAppName.GetBuffer());
		else return NULL;

		CString strRegKey;
		strRegKey.Format(TEXT("%s\\%s"),str.GetBuffer(),szKindInfo);

		//��ע���
		CRegKey PassWord;
		PassWord.Open(HKEY_CURRENT_USER,strRegKey);
		if (PassWord!=NULL)
		{

			TCHAR szPassBuffer[81];
			ULONG uBufferLength=sizeof(szPassBuffer);
			LONG uErrorCode=PassWord.QueryStringValue(TEXT("EncryptPass"),szPassBuffer,&uBufferLength);
			if (uErrorCode==ERROR_SUCCESS)
			{
				return CrevassePass(szPassBuffer);
			}
		}

		//���������IP
		PassWord.Close();
// 		strRegKey.Format(REG_SERVER,g_strAppName);
// 		strRegKey.Format(TEXT("%s\\%s"),strRegKey,Glb().m_CenterServerPara.m_strMainserverIPAddr);
// 		PassWord.Create(HKEY_CURRENT_USER,strRegKey);


	}
	return NULL;
}

//��ȡ��½ʱ����Ϣ
DWORD CLogonDialog::GetUserLogonTime(const TCHAR* szKindInfo, UINT uType)
{
	//��ȡע���
	if ((szKindInfo!=NULL)&&(szKindInfo[0]!=0))
	{
		//����ע����ַ�
		CString str;
		if (uType==LK_BY_NAME)	str.Format(REG_NAME,g_strAppName.GetBuffer());
		else if (uType==LK_BY_ACC) str.Format(REG_ACC,g_strAppName.GetBuffer());
		else if (uType==LK_BY_MOBILE) str.Format(REG_MOBILE,g_strAppName.GetBuffer());
		else if (uType == LK_BY_ID) str.Format(REG_ID,g_strAppName.GetBuffer());
		else return NULL;

		CString strRegKey;
		strRegKey.Format(TEXT("%s\\%s"),str.GetBuffer(),szKindInfo);

		//��ע���
		CRegKey LogonTime;
		LogonTime.Open(HKEY_CURRENT_USER,strRegKey);
		if (LogonTime!=NULL)
		{

			TCHAR szPassBuffer[81];
			DWORD dwLogonTime = 0;
			LONG uErrorCode=LogonTime.QueryDWORDValue(TEXT("LogonTime"),dwLogonTime);
			if (uErrorCode==ERROR_SUCCESS)
			{
				return dwLogonTime;
			}
		}

		//���������IP
		LogonTime.Close();


	}
	return 0;
}

//д��ע���������Ϣ
bool CLogonDialog::WriteUserPass(const TCHAR * szKindInfo, TCHAR * szPass, UINT uType)
{
	//д��ע���
	if ((szKindInfo!=NULL)&&(szKindInfo[0]!=0))
	{
		//����ע����ַ�
		CString str;
		CRegKey PassWord;
// 		strRegKey.Format(REG_SERVER,g_strAppName.GetBuffer());
// 		CString sAddr = Glb().m_CenterServerPara.m_strMainserverIPAddr;
// 		strRegKey.Format(TEXT("%s\\%s"),strRegKey,sAddr.GetBuffer());
// 		PassWord.Create(HKEY_CURRENT_USER,strRegKey);
// 		PassWord.Close();
		//AfxMessageBox(strRegKey);
		if (uType==LK_BY_NAME)	str.Format(REG_NAME,g_strAppName.GetBuffer());
		else if (uType==LK_BY_ACC) str.Format(REG_ACC,g_strAppName.GetBuffer());
		else if (uType==LK_BY_MOBILE) str.Format(REG_MOBILE,g_strAppName.GetBuffer());
		else if (uType==LK_BY_ID) str.Format(REG_ID,g_strAppName.GetBuffer());
		else return NULL;

		CString strRegKey;
		strRegKey.Format(TEXT("%s\\%s"),str.GetBuffer(),szKindInfo);

		//��ע���

		PassWord.Create(HKEY_CURRENT_USER,strRegKey);
		if (PassWord!=NULL)	
		{
			PassWord.SetStringValue(TEXT("EncryptPass"),EncryptPass(szPass));
			return true;
		}

		//���������IP

	}
	return false;
}

//д���½ʱ����Ϣ
bool CLogonDialog::WriteUserLogonTime(const TCHAR* szKindInfo, UINT uType)
{
	//д��ע���
	if ((szKindInfo!=NULL)&&(szKindInfo[0]!=0))
	{
		//����ע����ַ�
		CString str;
		CRegKey PassWord;
		// 		strRegKey.Format(REG_SERVER,g_strAppName.GetBuffer());
		// 		CString sAddr = Glb().m_CenterServerPara.m_strMainserverIPAddr;
		// 		strRegKey.Format(TEXT("%s\\%s"),strRegKey,sAddr.GetBuffer());
		// 		PassWord.Create(HKEY_CURRENT_USER,strRegKey);
		// 		PassWord.Close();
		//AfxMessageBox(strRegKey);
		if (uType==LK_BY_NAME)	str.Format(REG_NAME,g_strAppName.GetBuffer());
		else if (uType==LK_BY_ACC) str.Format(REG_ACC,g_strAppName.GetBuffer());
		else if (uType==LK_BY_MOBILE) str.Format(REG_MOBILE,g_strAppName.GetBuffer());
		else if (uType==LK_BY_ID) str.Format(REG_ID,g_strAppName.GetBuffer());
		else return NULL;

		CString strRegKey;
		strRegKey.Format(TEXT("%s\\%s"),str.GetBuffer(),szKindInfo);

		//��ע���

		PassWord.Create(HKEY_CURRENT_USER,strRegKey);
		if (PassWord!=NULL)	
		{
			time_t t = time(NULL);
			PassWord.SetDWORDValue(TEXT("LogonTime"),t);
			return true;
		}

		//���������IP

	}
	return false;
}

//��������
CString CLogonDialog::EncryptPass(const TCHAR * szPass)
{
	//������Կ
	unsigned char szRandKey[KEY_LENGTH];
	srand((unsigned)time(NULL));
	szRandKey[0]=lstrlen(szPass);
	for (int i=1;i<sizeof(szRandKey);i++) szRandKey[i]=rand()%255;

	//��������
	BYTE bTempCode=0;
	TCHAR szBuffer[7];
	memset(szBuffer,0,sizeof(szBuffer));
	CString strEncryptPass;
	const UINT uKeyLength=sizeof(szRandKey)/sizeof(szRandKey[0]);
	UINT uTimes=(szRandKey[0]+uKeyLength-1)/uKeyLength*uKeyLength;
	for (UINT i=0;i<uTimes;i++)
	{
		if (i<szRandKey[0]) bTempCode=szPass[i]^szRandKey[i%uKeyLength];
		else bTempCode=szRandKey[i%uKeyLength]^(rand()%255);
		wsprintf(szBuffer,TEXT("%02x%02x"),szRandKey[i%uKeyLength],bTempCode);
		strEncryptPass+=szBuffer;
	}

	return strEncryptPass;
}

//�⿪����
CString CLogonDialog::CrevassePass(const TCHAR * szEncryPass)
{
	if ((szEncryPass!=NULL)&&(lstrlen(szEncryPass)>=KEY_LENGTH*4))
	{
		//Ч�鳤��
		UINT uSoureLength=0;
		TCHAR * pEnd=NULL;
		TCHAR szSoureLengthBuf[3]={szEncryPass[0],szEncryPass[1],0};
		uSoureLength=strtol(szSoureLengthBuf,&pEnd,16);
		if (lstrlen(szEncryPass)!=((uSoureLength+KEY_LENGTH-1)/KEY_LENGTH*KEY_LENGTH*4)) return NULL;

		//�⿪����
		CString strCrevassePass;
		for (UINT i=0;i<uSoureLength;i++)
		{
			unsigned char szKeyBuffer[3]={szEncryPass[i*4],szEncryPass[i*4+1],0};
			TCHAR szEncryptBuffer[3]={szEncryPass[i*4+2],szEncryPass[i*4+3],0};
			UINT uKey,uEncrypt;
			uKey=strtol((TCHAR *)szKeyBuffer,&pEnd,16);
			uEncrypt=strtol(szEncryptBuffer,&pEnd,16);
			strCrevassePass+=(TCHAR)((BYTE)uKey^(BYTE)uEncrypt);
		}
		return strCrevassePass;
	}
	return NULL;
}

//�������û�
void CLogonDialog::OnBnClickedRegUser()
{
	CString s=CBcfFile::GetAppPath ();/////����·��

	CBcfFile fskin( s + m_skinmgr.GetSkinBcfFileName());
	CString key="LogonDialog";
	CString RegPageColor=fskin.GetKeyVal(key,"regpagebgcolor","");

	CBcfFile f( s + "bzgame.bcf");
	key="BZW";
	CString m_regurl=f.GetKeyVal (Glb().m_key,"AppRegUrl","http://localhost/app/regist.asp");
	int outSide=f.GetKeyVal(Glb().m_key,"appregoutside",0);

	if(outSide==1)
	{
		ShellExecute(NULL,TEXT("open"),m_regurl,0,0,SW_SHOW);
		return;
	}

	CRegisterNewUserDlg Dlg;//////////��ʾע�ᴰ��
	ShowWindow(SW_HIDE);
	if(Dlg.DoModal()==IDOK)//�����ע�ᰴŤ
	{
		SetLogonType(LK_BY_NAME, m_bLogonType);
		m_pIDEdit->SetText(Dlg.m_regname);//ע������
		m_pPwdEdit->SetText("");
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString strTip = fMsg.GetKeyVal("LogonDlg","SuccessTip","ע��ɹ�");
		CString strMessage = fMsg.GetKeyVal("LogonDlg","RegSuccess","��ϲ��ע���û��ɹ�\n������ղ���ע����û�����������е�¼��");
		CString strout;
		strout.Format("��ϲ��ע���û��ɹ�\n������ղ���ע����û�����������е�¼\n�����˺ţ�%s\n����ID: %s\n��ȷ��ֱ�ӵ�¼����Ϸƽ̨", Dlg.m_regname, Dlg.m_regID);
		//AFCMessageBox(strout, strTip);
	
		CRegSucDlg* dlg = new CRegSucDlg();
		dlg->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
		dlg->CenterWindow();
		dlg->SetNameID(Dlg.m_regname, Dlg.m_regID);
		dlg->ShowModal();
		

		m_pPwdEdit->SetFocus();
		ShowWindow(SW_SHOW);
		return;
	}
	ShowWindow(SW_SHOW);
}

//���߰���
void CLogonDialog::OnBnClickedQQHelp()
{
	CString s=CBcfFile::GetAppPath();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	CString strGetPassUrl = f.GetKeyVal("BZW","QQHelp","");
	ShellExecute(NULL,TEXT("open"),"IEXPLORE",strGetPassUrl,NULL,SW_HIDE);
}

//���߰���
void CLogonDialog::OnBnClickedHelp()
{

	return;
}

//�ʺŵ�½
void CLogonDialog::OnBnClickedAccounts()
{
	SetLogonType(LK_BY_NAME,m_bLogonType);
	return;
}

//ɾ����ť
void CLogonDialog::OnBnClickedDelete()
{
	//��ȡ��Ϣ
	CString strInputInfo = m_pIDEdit->GetText();

	//ɾ����Ϣ
	if (strInputInfo.GetLength()>0)
	{
		//����ע����ַ�
		CString strRegKey;
		if (m_bLogonType==LK_BY_NAME) strRegKey.Format(REG_NAME,g_strAppName.GetBuffer());
		else if (m_bLogonType==LK_BY_ACC) strRegKey.Format(REG_ACC,g_strAppName.GetBuffer());
		else if (m_bLogonType==LK_BY_MOBILE) strRegKey.Format(REG_MOBILE,g_strAppName.GetBuffer());
		else if (m_bLogonType == LK_BY_ID) strRegKey.Format(REG_ID,g_strAppName.GetBuffer());

		
		//��ע���
		CRegKey InputInfo;
		InputInfo.Open(HKEY_CURRENT_USER,strRegKey);
		if (InputInfo==NULL) return;
		InputInfo.RecurseDeleteKey(strInputInfo);
		//������ʾ
		for (int i = 0; i < m_pUserIDCob->GetCount(); ++i)
		{
			CControlUI* pFind = m_pUserIDCob->GetItemAt(i);
			if (strInputInfo == pFind->GetText())
			{
				m_pUserIDCob->RemoveAt(i);
				m_pPwdEdit->SetText("");
				break;
			}
		}
	}

	//��ʾ����Ϣ
	m_pIDEdit->SetFocus();
	if (m_pUserIDCob->GetCount()>0)
	{
		m_pUserIDCob->SelectItem(0);
		strInputInfo = m_pIDEdit->GetText();
		UpdatePassWord(strInputInfo);
	}

	return;
}

//�Ҽ�����
void CLogonDialog::OnBnClickedFindPassword()
{
	//wushuqun 2009.6.16
	//֮ǰΪʲôҪд��ҳ������

	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	CString strGetPassUrl = f.GetKeyVal("BZW","GetBackPwdURL","/Manage/Retake.aspx");
	strGetPassUrl = Glb().m_CenterServerPara.m_strWebRootADDR + strGetPassUrl;
	ShellExecute(NULL,TEXT("open"),strGetPassUrl,NULL,NULL,SW_MAXIMIZE);

	/*TCHAR url[300];
	wsprintf(url,"%sManage/Retake.aspx",Glb().m_CenterServerPara.m_strWebRootADDR);
	ShellExecute(NULL,TEXT("open"),url,NULL,NULL,SW_MAXIMIZE);*/
	return ;
}

//�鿴��������
void CLogonDialog::OnBnClickedTermOfService()
{
	CString s = CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	CString strGetPassUrl = f.GetKeyVal("BZW","TermOfServiceURL","/Manage/Regg.aspx");
	strGetPassUrl = Glb().m_CenterServerPara.m_strWebRootADDR + strGetPassUrl;
	ShellExecute(NULL,TEXT("open"),strGetPassUrl,NULL,NULL,SW_MAXIMIZE);

	return ;
}

//����ͷ��
void CLogonDialog::OnBnClickedSelectLogo()
{
// 	CRect SelcetRect,SelectDlgRect;
// 	if (m_LogoSelect.GetSafeHwnd()==NULL) m_LogoSelect.Create(IDD_LOGO_SELECT,this);
// 	m_LogoSelect.GetWindowRect(&SelectDlgRect);
// 	m_LogoSelect.MoveWindow(SelcetRect.right-SelectDlgRect.Width(),SelcetRect.top-SelectDlgRect.Height(),SelectDlgRect.Width(),SelectDlgRect.Height());
// 	m_LogoSelect.SetVisible();
// 	m_LogoSelect.ShowWindow(SW_SHOW);
// 	m_LogoSelect.BringWindowToTop();
	return;
}


//���ְ���
void CLogonDialog::OnBnClickedNewHelp()
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	CString strUrl = f.GetKeyVal("BZW","GetHelpURL","NewsList.aspx?tid=9");
	strUrl = Glb().m_CenterServerPara.m_strWebRootADDR + strUrl;
	ShellExecute(NULL,TEXT("open"),strUrl,NULL,NULL,SW_MAXIMIZE);

	//TCHAR url[300];
	//wsprintf(url,"%sNews.aspx?Cid=9",Glb().m_CenterServerPara.m_strWebRootADDR);
	//ShellExecute(NULL,TEXT("open"),url,NULL,NULL,SW_MAXIMIZE);
	return ;
}


#ifdef AI_MACHINE
void CLogonDialog::AILogin()
{
	// ��ȡ����
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + _T("ai_config.bcf"));
	CString key=_T("config");
	//CString userid,userpsw;
	//char szSrcpsw[21],szDestpsw[33];
	int curid;
	int count;
	CString userName;		//��½��ID
	CString userpsw;		//��½������
	count = f.GetKeyVal(key, TEXT("count"), 0);
	curid = f.GetKeyVal(key, TEXT("curid"), 0);

	if(count<=0 || curid<=0)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString strMessage = fMsg.GetKeyVal("LogonDlg","RobotConfigError","�����������ļ�����.");
		
		//AFCMessageBox(strMessage);
	     DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,strMessage);
		PostQuitMessage(-1);
		return ;
	}
	if(curid > count)
	{
		curid = 0;
	}
	CString str;
	str.Format(_T("%d"), curid+1);
	f.SetKeyValString(key, _T("curid"), str);

	str.Format(_T("username%d"),curid);
	userName= f.GetKeyVal(key,str,"bozong1000");

	str.Format(_T("userpsd%d"),curid);
	//lstrcpy(m_PlaceUserInfo.szName,userid);
	//���������md5����
	userpsw = f.GetKeyVal(key,str,"112233");
	//	wsprintf(szSrcpsw,"%s",userpsw);
	//����û���������
	strcpy(m_szName,userName);
	strcpy(m_szScrPass,userpsw);
	//	m_PassWord.SetWindowText(m_szScrPass);
	//	m_static_Name.SetWindowText(m_szName);
}
#endif

void CLogonDialog::CenterWindow()
{
	RECT rc;
	::GetWindowRect(m_hWnd,&rc);
	int iSrcW = GetSystemMetrics(SM_CXSCREEN);
	int iSrcH = GetSystemMetrics(SM_CYSCREEN);
	int iWndW = rc.right - rc.left;
	int iWndH = rc.bottom - rc.top;
	::MoveWindow(m_hWnd,(iSrcW - iWndW)/2,(iSrcH - iWndH)/2,iWndW,iWndH,true);
}

//�û�����¼
void CLogonDialog::OnBnClickedLogonByName()
{
	SetLogonType(LK_BY_NAME, m_bLogonType);
	Init();
}
//�û�ID��¼
void CLogonDialog::OnBnClickedLogonByID()
{
	SetLogonType(LK_BY_ID, m_bLogonType);
	Init();
}

void CLogonDialog::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click")) 
	{
		if( msg.pSender->GetName() == _T("Bnt_Close")) 
		{
			m_nRetFlag = 0;
			Close();
		}
		else if( msg.pSender->GetName() == _T("Bnt_Logon")) 
		{
			OnOK();

			ChangeSkin();

			//֪ͨ������
			m_skinmgr.Notify();

			//������ʾ���Ƥ��
			CString strSkin = m_skinmgr.GetSkinBcfFileName();
			ConfirmDlg::SetSkinKey(m_skinmgr.GetKeyVal(strSkin));

			//ʹ�������
			((CGamePlaceDlg*)AfxGetMainWnd())->m_topDuWnd.OnHitMax();
		}
		else if( msg.pSender->GetName() == _T("Bnt_reg")) 
		{
			OnBnClickedRegUser();
		}
		else if( msg.pSender->GetName() == _T("Bnt_QQHelp")) 
		{
			OnBnClickedQQHelp();
		}
		else if( msg.pSender->GetName() == _T("Bnt_Clear")) 
		{
			OnBnClickedDelete();
		}
		else if( msg.pSender->GetName() == _T("Bnt_FindPwd")) 
		{
			OnBnClickedFindPassword();
		}
		else if (msg.pSender->GetName() == _T("Bnt_Argeement"))
		{
			OnBnClickedTermOfService();
		}
		else if (msg.pSender->GetName() == _T("But_LogonByName"))
		{
			OnBnClickedLogonByName();
		}
		else if (msg.pSender->GetName() == _T("But_LogonByID"))
		{
			OnBnClickedLogonByID();
		}
	}
	else if( msg.sType == _T("itemselect") ) 
	{
		if( msg.pSender->GetName() == _T("Cob_ID")) 
		{	
			m_pIDEdit->SetText(msg.pSender->GetText());
			ChangeSkin();
			UpdatePassWord(msg.pSender->GetText().GetData());
		}
	}
	else if (msg.sType == _T("textchanged"))
	{
		if (msg.pSender->GetName() == _T("Edt_ID") && m_bLogonType == LK_BY_ID)
		{
			//ת�˽��Сдת��Ϊ���Ĵ�д
			CString strOut = m_pIDEdit->GetText();
			CString strFilet = "";
			if (strOut != "")
			{
				for (int i = 0; i < strOut.GetLength(); i++)
				{
					if (strOut[i] >= '0' && strOut[i] <= '9')
						strFilet += strOut[i];
				}
				if (strOut != strFilet)
				{
					m_pIDEdit->SetText(strFilet);
					m_pIDEdit->SetSel(strFilet.GetLength());
				}
			}


		}
	}
	
}

void CLogonDialog::OnOK()
{
	if (!IsApproveAgreement())
	{
		AFCMessageBox("���Ķ�����ѡͬ������.",MESSAGE_TITLE);
		return;
	}
#ifdef AI_MACHINE
	AILogin();

	//��������
	MD5_CTX Md5;
	Md5.MD5Update((unsigned char *)m_szScrPass,lstrlen(m_szScrPass));
	unsigned char szMDTemp[16];
	Md5.MD5Final(szMDTemp);
	for (int i=0;i<16;i++) 
		wsprintf(&m_szMD5Pass[i*2],"%02x",szMDTemp[i]);

	m_nRetFlag = 1;
	Close();
	return;// __super::OnOK();
#endif

	if(!m_bshow)
	{
		m_bRemberPass=false;
		if(!m_bisgetid)
			return ;
		::KillTimer(m_hWnd,1);
		//����ע��
		m_bBoy=true;//��ȡע�ᴰ�ڵ��Ա�		
		m_bLogoID=rand()%12;//��ȡע�ᴰ��ͷ�� ID
		lstrcpy(m_szRegSQL,"");//�û�ע�ḽ����Ϣ��SQL

		CenterServerMsg& par=Glb().m_CenterServerPara;
		if( par.m_nEncryptType == 1)	//MD5���ܷ�ʽ
		{
			MD5_CTX Md5;
			Md5.MD5Update((unsigned char *)m_szScrPass,lstrlen(m_szScrPass));
			unsigned char szMDTemp[16];
			Md5.MD5Final(szMDTemp);
			for (int i=0;i<16;i++) wsprintf(&m_szMD5Pass[i*2],"%02x",szMDTemp[i]);
		}
		if( par.m_nEncryptType == 2 )	//sha1���ܷ�ʽ
		{
			//JMod-�޸�ΪSHA��ʽ-20090429
			unsigned szMDTemp[5];
			CString s=m_szScrPass;
			SHA1 sha;
			sha.Reset();
			sha<<s;
			sha.Result(szMDTemp);
			for (int i=0;i<5;i++) 
				wsprintf(&m_szMD5Pass[i*8],"%08x",szMDTemp[i]);
		}
		//__super::OnOK();
		m_nRetFlag = 1;
		Close();
		return ;
	}
	else
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString strMessage;
		//if (GetDlgItemText(IDC_CODE,m_szName,sizeof(m_szName))==0)
		//{
		//	AfxMessageBox(TEXT("����д��֤�룡"),MB_ICONWARNING);
		//	GetDlgItem(IDC_CODE)->SetFocus();
		//	return;
		//}
		//if(lstrcmp(m_szName,m_code)!=0)
		//{
		//	AfxMessageBox(TEXT("��֤�����"),MB_ICONWARNING);
		//	wsprintf(m_code,"%d%d%d%d",rand()%10,rand()%10,rand()%10,rand()%10);
		//	this->InvalidateRect(NULL);
		//	GetDlgItem(IDC_CODE)->SetFocus();
		//	return;
		//}
		//Ч���û������ʺ�
		switch (m_bLogonType)
		{
		case LK_BY_NAME:///ʹ���ʻ���½
			{
				strcpy(m_szName,m_pIDEdit->GetText().GetData());

				if (strlen(m_szName) == 0)
				{
					BZSoundPlay(NULL,"music/������ʾ.mp3", 0);
					strMessage = fMsg.GetKeyVal("LogonDlg","InPutUserName","����д�û�����");
					DUIOkeyMsgBox(m_hWnd,false,strMessage);
					m_pIDEdit->SetFocus();
					return;
				}
				break;		
			}
		case LK_BY_ID:
			{
				strcpy(m_szName,m_pIDEdit->GetText().GetData());
				int x = atoi(m_szName);
				if (strlen(m_szName) == 0)
				{
					BZSoundPlay(NULL,"music/������ʾ.mp3", 0);
					strMessage = fMsg.GetKeyVal("LogonDlg","InPutUserID","����д�û���ID��");
					DUIOkeyMsgBox(m_hWnd,false,strMessage);
					m_pIDEdit->SetFocus();
					return;
				}
				
				break;	
			}
			//case LK_BY_ACC://ʹ��ID��½
			//	{
			//		CString strBuffer;
			//		GetDlgItemText(IDC_ACC_INFO,strBuffer);
			//		m_dwAccID=_atoi64(strBuffer);
			//		if (m_dwAccID<=0)
			//		{
			//			AfxMessageBox(TEXT("����ȷ�����½ID���룡"),MB_ICONWARNING);
			//			GetDlgItem(IDC_ACC_INFO)->SetFocus();
			//			return;
			//		}
			//		break;
			//	}
			//case LK_BY_MOBILE://ʹ���ֻ���½
			//	{
			//		CString strBuffer;
			//		GetDlgItemText(IDC_MOBILE_INFO,strBuffer);
			//		m_dwMobile=_atoi64(strBuffer);
			//		if (m_dwMobile<=0)
			//		{
			//			AfxMessageBox(TEXT("����ȷ�����ֻ����룡"),MB_ICONWARNING);
			//			GetDlgItem(IDC_MOBILE_INFO)->SetFocus();
			//			return;
			//		}
			//		break;
			//	}
		default:return;
		}

		strcpy(m_szScrPass,m_pPwdEdit->GetText().GetData());

		//Ч���û�����
		if (strlen(m_szScrPass) == 0)
		{
			BZSoundPlay(NULL, "music/������ʾ.mp3", 0);
			strMessage = fMsg.GetKeyVal("LogonDlg","InPutPW","����д���룡");
			DUIOkeyMsgBox(m_hWnd,false,strMessage);
			m_pPwdEdit->SetFocus();
			return;
		}

		//Ч���½������
		int nSelIdx = m_pServerCob->GetCurSel();
		if (nSelIdx >= m_vtServer.size() || nSelIdx < 0)
		{
// 			AFCMessageBox("��ѡ�������");
// 			return;
			nSelIdx = 0;
		}

		char* pszLoginIP = m_vtServer[nSelIdx].szLoginIP;

		if (::strlen(pszLoginIP) == 0)
		{
			BZSoundPlay(NULL,"music/������ʾ.mp3", 0);
			strMessage = fMsg.GetKeyVal("LogonDlg","ServerAddrError","��������ַ����ȷ��");
			DUIOkeyMsgBox(m_hWnd,false,strMessage);
			m_pServerCob->SetFocus();
			return;
		}

		//���� �����IP
		strcpy(Glb().m_CenterServerPara.m_strMainserverIPAddr,pszLoginIP);
		//GetDlgItemText(IDC_SERVER_LIST,,sizeof(Glb().m_CenterServerPara.m_strMainserverIPAddr));
		//((CComboBox *)GetDlgItem(IDC_SERVER_LIST))->GetWindowText(Glb().m_CenterServerPara.m_strMainserverIPAddr);
		//��¼��Ϣ
		COptionUI* pSaveOpt = static_cast<COptionUI*>(m_pm.FindControl(_T("Opt_SavePwd")));
		m_bRemberPass = pSaveOpt->IsSelected();
		AfxGetApp()->WriteProfileInt(TEXT("LogonInfo"),TEXT("ExpandedList"),1);
		AfxGetApp()->WriteProfileString(TEXT("LogonInfo"),TEXT("UserName"),m_szName);
		AfxGetApp()->WriteProfileInt(TEXT("LogonInfo"),TEXT("RemPass"),m_bRemberPass);
		AfxGetApp()->WriteProfileInt(TEXT("LogonInfo"),TEXT("AutoGameKind"),2);
		AfxGetApp()->WriteProfileString(TEXT("LogonInfo"),TEXT("LastLogonServer"),Glb().m_CenterServerPara.m_strMainserverIPAddr);
		AfxGetApp()->WriteProfileInt(TEXT("LogonInfo"),TEXT("UseProxy"),m_bUseProxy);

		////////////
		AfxGetApp()->WriteProfileInt(TEXT("LogonInfo"),TEXT("Immediately"),1);
		///////////////
		//if (m_bUseProxy)
		//{
		//	//������Ϣ
		//	AfxGetApp()->WriteProfileInt(TEXT("ProxyInfo"),TEXT("ProxyPort"),m_uProxyPort);
		//	AfxGetApp()->WriteProfileInt(TEXT("ProxyInfo"),TEXT("SOCK5"),m_bSock5);
		//	AfxGetApp()->WriteProfileInt(TEXT("ProxyInfo"),TEXT("ProxyCheck"),m_bUserCheck);
		//	AfxGetApp()->WriteProfileString(TEXT("ProxyInfo"),TEXT("ProxyServer"),m_szProxyServer);
		//	if (m_bUserCheck)
		//	{
		//		AfxGetApp()->WriteProfileString(TEXT("ProxyInfo"),TEXT("ProxyUserName"),m_szProxyName);
		//		AfxGetApp()->WriteProfileString(TEXT("ProxyInfo"),TEXT("ProxyUserPass"),m_szProxyPass);
		//	}
		//}

		////��������

		CenterServerMsg& par=Glb().m_CenterServerPara;

		if( par.m_nEncryptType == 1)
		{
			
			MD5_CTX Md5;
			Md5.MD5Update((unsigned char *)m_szScrPass,lstrlen(m_szScrPass));
			unsigned char szMDTemp[16];
			Md5.MD5Final(szMDTemp);
			for (int i=0;i<16;i++) wsprintf(&m_szMD5Pass[i*2],"%02x",szMDTemp[i]);
		}
		if(par.m_nEncryptType == 2)
		{
			//JMod-���ܴ���-20090429
			unsigned szMDTemp[5]={0};
			CString strSha=m_szScrPass;
			SHA1 sha;
			sha.Reset();
			sha<<strSha;
			sha.Result(szMDTemp);

			CString strSingle;
			strSha="";
			for (int i=0;i<5;i++) 
			{
				strSingle.Format("%.8x",szMDTemp[i]);
				strSha+=strSingle;
			}
			wsprintf(m_szMD5Pass,"%s",strSha);
		}
		m_nRetFlag = 1;
		Close();
		//__super::OnOK();
	}
}
//�Ƿ�ͬ������
bool CLogonDialog::IsApproveAgreement()
{
	COptionUI* pAgreeCtrl  = static_cast<COptionUI*>(m_pm.FindControl("Opt_Argeement"));
	return pAgreeCtrl->IsSelected();
}

//���س���ؼ�Ƥ��
void CLogonDialog::LoadNormalCtrlSkin()
{
	int idx = 0;
	CStdPtrArray* pCtrls = NULL;

	//���������ı��༭���Ƥ��
	pCtrls = m_pm.FindSubControlsByClass(m_pm.GetRoot(),_TEXT("EditUI"));
	CEditUI* pEditUI = NULL;
	do 
	{
		pEditUI = static_cast<CEditUI*>(pCtrls->GetAt(idx++));
		if (pEditUI)
		{
			if (pEditUI->GetName() == _T("Edt_ID"))
			{
				continue;
			}
			pEditUI->SetNormalImage("file='dialog\\edit.png' dest='0,0,500,30' source='0,0,500,22' corner='3,4,2,2'");
			pEditUI->SetHotImage("file='dialog\\edit.png' dest='0,0,500,30' source='0,22,500,44' corner='3,4,2,2'");
			pEditUI->SetFocusedImage("file='dialog\\edit.png' dest='0,0,500,30' source='0,44,500,66' corner='3,4,2,2'");
			pEditUI->SetDisabledImage("file='dialog\\edit.png' dest='0,0,500,30' source='0,66,500,88' corner='3,4,2,2'");
		}
	} while (pEditUI);

	//�������������б���Ƥ��
	idx = 0;
	pCtrls = m_pm.FindSubControlsByClass(m_pm.GetRoot(),_TEXT("ComboUI"));
	CComboUI* pComboUI = NULL;
	do 
	{
		pComboUI = static_cast<CComboUI*>(pCtrls->GetAt(idx++));
		if (pComboUI)
		{
			pComboUI->SetNormalImage("file='dialog\\combo.png' dest='0,0,500,30' source='0,0,500,22' corner='5,2,22,0'");
			pComboUI->SetHotImage("file='dialog\\combo.png' dest='0,0,500,30' source='0,22,500,44' corner='5,2,22,0'");
			pComboUI->SetPushedImage("file='dialog\\combo.png' dest='0,0,500,30' source='0,44,500,66' corner='5,2,22,0'");
			pComboUI->SetDisabledImage("file='dialog\\combo.png' dest='0,0,500,30' source='0,66,500,88' corner='5,2,22,0'");
			RECT rc = {6,0,0,0};
			pComboUI->SetItemTextPadding(rc);

// 			CScrollBarUI* pVScroll = pComboUI->GetVerticalScrollBar();
// 			if (pVScroll)
// 			{
// 				pVScroll->SetBkNormalImage("file='dialog\\vscrollbar_back.png' dest='0,0,14,100' source='0,0,14,100' corner='0,0,0,0'");
// 				pVScroll->SetButton1NormalImage("file='dialog\\vscrollbar_up.png' dest='0,0,16,16' source='0,0,16,16' corner='0,0,0,0'");
// 			}
		}
	} while (pComboUI);

	//�������ж�ѡ���Ƥ��
	idx = 0;
	pCtrls = m_pm.FindSubControlsByClass(m_pm.GetRoot(),_TEXT("OptionUI"));
	COptionUI* pOptionUI = NULL;
	do 
	{
		pOptionUI = static_cast<COptionUI*>(pCtrls->GetAt(idx++));
		if (pOptionUI)
		{
			pOptionUI->SetNormalImage("file='dialog\\check.png' dest='0,0,15,15' source='0,0,15,15' corner='0,0,0,0'");
			pOptionUI->SetHotImage("file='dialog\\check.png' dest='0,0,15,15' source='15,0,30,15' corner='0,0,0,0'");
			pOptionUI->SetSelectedImage("file='dialog\\check.png' dest='0,0,15,15' source='30,0,45,15' corner='0,0,0,0'");
		}
	} while (pOptionUI);

}

DWORD __stdcall CLogonDialog::ThreadDownConnFile(LPVOID param)
{

	DWORD   dwCurPos = 0;
	CString strFileName;//Դ�ļ���
	CString strDestName = "bzconn.temp";//Ŀ���ļ���
	CString strFullpath;//Ŀ��ȫ·��
	CString strPath;	//Ŀ��ȫ·���������ļ���
	TCHAR   szMsg[1000];
	DWORD   err;
	//��ȡԴ�ļ���

	CLogonDialog* pLogonDlg = (CLogonDialog*)param;
	
    DWORD dwFlags;
	::InternetGetConnectedState(&dwFlags, 0);
	TCHAR strAgent[64];
	memset(strAgent,0,sizeof(strAgent));
	wsprintf(strAgent,TEXT("Agent%ld"),timeGetTime());

	HINTERNET hOpen;
	if(!(dwFlags & INTERNET_CONNECTION_PROXY))
	{
		hOpen = ::InternetOpen(strAgent,INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY,NULL,NULL,0);
	}
	else
	{
		hOpen = ::InternetOpen(strAgent,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	}
		
	if(!hOpen)
	{
		//��ʾ������Ϣ
		err = GetLastError();
		wsprintf(szMsg,TEXT("DDownload:InternetOpen����. %d."),err);
		OutputDebugString(szMsg);
		CLogonDialog::FillServer(pLogonDlg->m_pServerCob);
		return -1;
	}

	DWORD dwSize;
	TCHAR szHead[] = TEXT("Accept: */*\r\n\r\n");
	VOID* szTemp[16384];
	HINTERNET hConnect;
	CFile file;

	CBcfFile f(CBcfFile::GetAppPath() + "bzgame.bcf");
	CString strDownloadUrl = f.GetKeyVal("BZW","ConnFileUrl","");
	if (strDownloadUrl.GetLength() == 0)
	{
		wsprintf(szMsg,TEXT("DDownload:urlΪ��. %d."),err);
		if (pLogonDlg->GetHWND())
		{
			CLogonDialog::FillServer(pLogonDlg->m_pServerCob);
		}
		return -1;
	}

	if (!(hConnect = ::InternetOpenUrl(hOpen,strDownloadUrl.GetBuffer(),szHead,lstrlen(szHead),
							INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_RELOAD,0)))
	{
		//��ʾ������Ϣ
		err = ::GetLastError();		
		::InternetCloseHandle(hOpen);
		TCHAR szError[1000];
		DWORD len=999;
		if(::InternetGetLastResponseInfo(&err,szError,&len) && lstrcmp(szError,TEXT("")) != 0)
		{
			wsprintf(szMsg,TEXT("DDownload:InternetOpenUrl ����.\n��ַ:%s.\n������Ϣ:%s."),strDownloadUrl.GetBuffer(),szError);
		}
		else
		{
			wsprintf(szMsg,TEXT("DDownload:InternetOpenUrl ����.\n��ַ:%s.\n�����:%d."),strDownloadUrl.GetBuffer(),err);
		}
		OutputDebugString(szMsg);
		if (pLogonDlg->GetHWND())
		{
			CLogonDialog::FillServer(pLogonDlg->m_pServerCob);
		}
		return -1;
	}
	bool CreateDir = false;

	if(file.Open(strDestName.GetBuffer(),CFile::modeWrite|CFile::modeCreate) == FALSE)
	{
		//��ʾ������Ϣ
		wsprintf(szMsg,TEXT("DDownload:�����ļ�%s����. �����:%d."),strDestName.GetBuffer(),err);
		OutputDebugString(szMsg);
		if (pLogonDlg->GetHWND())
		{
			CLogonDialog::FillServer(pLogonDlg->m_pServerCob);
		}
		return -1;
	} 

	DWORD dwByteToRead = 0;
	DWORD dwSizeOfRq = 4;
	DWORD dwBytes = 0;

	if (!::HttpQueryInfo(hConnect,HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,(LPVOID)&dwByteToRead,&dwSizeOfRq,NULL))
	{
		dwByteToRead = 0;
		wsprintf(szMsg,TEXT("DDownload:��������%s.... ��Сδ֪."),strFileName.GetBuffer());
		OutputDebugString(szMsg);
	}

	//���ý�����
	if(dwByteToRead > 1024 * 100)
	{
		wsprintf(szMsg,TEXT("DDownload:��������%s.... ��С%dkbytes."),strFileName.GetBuffer(),dwByteToRead/1024);
	}
	else
	{
		wsprintf(szMsg,TEXT("DDownload:��������%s.... ��С%dbytes."),strFileName.GetBuffer(),dwByteToRead/1024);
	}
	OutputDebugString(szMsg);

	memset(szTemp,0,sizeof(szTemp));

	DWORD dwCurrentTicks = ::GetTickCount();
	DWORD dwTotalBytesRead = 0;
	DWORD dwLastTotalBytes = 0;
	do
	{
		if (!::InternetReadFile(hConnect,szTemp,16384,&dwSize))
		{
			//��ʾ������Ϣ
			file.Close();
			file.Remove(strDestName.GetBuffer());
			err = ::GetLastError();
			::InternetCloseHandle(hConnect);
			::InternetCloseHandle(hOpen);
			TCHAR szError[1000];
			DWORD len=999;
			if(::InternetGetLastResponseInfo(&err,szError,&len) && lstrcmp(szError,TEXT("")) != 0)
			{
				wsprintf(szMsg,TEXT("DDownload:InternetReadFile ����.\n ������Ϣ:%s."),szError);
			}
			else
			{ 
				wsprintf(szMsg,TEXT("DDownload:InternetReadFile ����.\n �����:%d."),err);
			}
			OutputDebugString(szMsg);
			if (pLogonDlg->GetHWND())
			{
				CLogonDialog::FillServer(pLogonDlg->m_pServerCob);
			}
			return -1;
		}

		dwTotalBytesRead += dwSize;

		DWORD dwNowTicks = GetTickCount();
		DWORD dwTimeTaken = dwNowTicks - dwCurrentTicks;
		if (dwTimeTaken > 1000)
		{
			double KbPerSecond = ((double)(dwTotalBytesRead) - (double)(dwLastTotalBytes)) / ((double)(dwTimeTaken));

			dwCurrentTicks = dwNowTicks;
			dwLastTotalBytes = dwTotalBytesRead;
		}

		if (dwSize == 0)
			break;
		else
		{
			file.Write(szTemp,dwSize);
			dwCurPos += dwSize;
		}
	}
	while (TRUE);

	//У���ļ�
	bool bCheckSuc = false;

	file.Close();

	::InternetCloseHandle(hConnect);
	::InternetCloseHandle(hOpen);

	if(file.Open(strDestName.GetBuffer(),CFile::modeRead))
	{
		if (file.SeekToEnd() > 6)
		{
			file.SeekToBegin();
			char szFileType[10];
			::memset(szFileType,0,sizeof(szFileType));
			file.Read(szFileType,6);
			if (strcmp(szFileType,"BZWCFG") == 0)
			{
				bCheckSuc = true;
			}
		}
		file.Close();
	}

	if (bCheckSuc) //����ϸ�����滻
	{
		::DeleteFile("bzconn.bcf");
		::MoveFile("bzconn.temp","bzconn.bcf");
	}
	else
	{
		::DeleteFile("bzconn.temp");
	}

#ifdef MY_DEV

	//������ģ��������ʱ
	::Sleep(800);

#endif

	if (pLogonDlg->GetHWND())
	{
		CLogonDialog::FillServer(pLogonDlg->m_pServerCob);
	}

	return 0;
}
//����
void CLogonDialog::ChangeSkin()
{
	CString strCurUser =(CString)(m_pIDEdit->GetText().GetData());
	if (strCurUser != "")
		m_skinmgr.SetLogOnUserName(strCurUser);
	else
		return;

	//�Ƿ��һ�ε�¼
	if (m_skinmgr.IsUserCountFirstLogOn(strCurUser))
		m_skinmgr.SetCurUserSelSkin(m_skinmgr.GetDefaultSelSkin());

	CString LocalPath = CBcfFile::GetAppPath();
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile filePath( LocalPath + strSkin);
	CString strKey = m_skinmgr.GetKeyVal(strSkin);
	CString strSkinPath = m_skinmgr.GetSkinPath();
	CString skinfolder = filePath.GetKeyVal(strKey,"skinfolder",strSkinPath);

	TCHAR szUIPath[MAX_PATH];
	wsprintf(szUIPath,"%s",skinfolder);
	BzDui::CPaintManagerUI::SetResourcePath(szUIPath);
	BzDui::CPaintManagerUI::ReloadSkin();
}
