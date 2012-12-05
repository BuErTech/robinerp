#include "Stdafx.h"
#include "GamePlace.h"
#include "LongonDialog.h"
#include "MainRoomEx.h"
#include "sha1.h"
#include ".\longondialog.h"
#include "NewControl\\IEHelper.h"

//������Կ����
#define KEY_LENGTH		10

// PengJiLin, 2010-6-2, Ÿ�� ��¼
extern BOOL g_bRunBySoftware;
extern BOOL g_bAutoConnect;
extern TCHAR g_chUserName[100];
extern TCHAR g_chMD5PSW[100];

//ע������
#define REG_NAME		TEXT("Software\\BZWϵͳ\\Login\\UserName")
#define REG_ACC			TEXT("Software\\BZWϵͳ\\Login\\ACC")
#define REG_MOBILE		TEXT("Software\\BZWϵͳ\\Login\\MOBILE")
#define REG_SERVER		TEXT("Software\\BZWϵͳ\\Login\\Logon_Server")
 
BEGIN_MESSAGE_MAP(CLongonDialog, CLoadFileBmp)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ACCOUNTS, OnBnClickedAccounts)
//	ON_BN_CLICKED(IDC_ACC, OnBnClickedAccID)
//	ON_BN_CLICKED(IDC_MOBILE, OnBnClickedMobile)
	ON_BN_CLICKED(IDC_REGISTER, OnBnClickedRegUser)
//	ON_BN_CLICKED(IDC_USER_PROXY, OnBnClickedUseProxy)
//	ON_BN_CLICKED(IDC_USE_PASS, OnBnClickedUsePass)
	ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
	ON_CBN_SELCHANGE(IDC_NAME_INFO, OnCbnSelchangeInput)
//	ON_CBN_SELCHANGE(IDC_ACC_INFO, OnCbnSelchangeInput)
//	ON_CBN_SELCHANGE(IDC_MOBILE_INFO, OnCbnSelchangeInput)
//	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_FIND_PASSWORD, OnBnClickedFindPassword)
	ON_BN_CLICKED(IDC_NEWHELP, OnBnClickedNewHelp)
	ON_BN_CLICKED(IDC_APPLY_PASSWORD, OnBnClickedApplyPassword)
	ON_BN_CLICKED(IDC_SOFTKEY,OnBnClickedSoftkey)
	ON_WM_LBUTTONDOWN()
//	ON_WM_SETCURSOR()
ON_WM_CREATE()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/*******************************************************************************************************/

//�û���Ϣ
bool		CLongonDialog::m_bInit=false;					//�Ƿ��ʼ����־
BYTE		CLongonDialog::m_bRemberPass=FALSE;				//��ס����
BYTE		CLongonDialog::m_bUseProxy=FALSE;				//ʹ�ô���
BYTE		CLongonDialog::m_bRegUser=FALSE;				//�����û�
BYTE		CLongonDialog::m_bLogonType=LK_BY_NAME;			//��½ģʽ
BYTE		CLongonDialog::m_bBoy=TRUE;						//�Ƿ��к�
UINT		CLongonDialog::m_bLogoID=5;						//ͷ�� ID
TCHAR		CLongonDialog::m_szName[61];					//�û�����
TCHAR		CLongonDialog::m_szScrPass[61];					//�û�����
TCHAR		CLongonDialog::m_szGameServer[51];				//��Ϸ IP
char		CLongonDialog::m_szRegSQL[210];					//",email='123'"
char		CLongonDialog::m_szMD5Pass[50];					//���ܺ������
int			CLongonDialog::m_dwAccID=0;						//ACC ����
int			CLongonDialog::m_dwMobile=0;					//�ֻ�����
TCHAR		CLongonDialog::m_nickname[100];					//�س�
TCHAR		CLongonDialog::m_szRecommenderName[61];			//���Ƽ���������
//������Ϣ
BYTE		CLongonDialog::m_bUserCheck=FALSE;				//��Ҫ����
BYTE		CLongonDialog::m_bSock5=TRUE;					//SOCK 5 ��־
UINT		CLongonDialog::m_uProxyPort=1080;				//����˿�
TCHAR		CLongonDialog::m_szProxyServer[51];				//���� IP
TCHAR		CLongonDialog::m_szProxyName[61];				//��������
TCHAR		CLongonDialog::m_szProxyPass[61];				//��������
bool		CLongonDialog::m_bIsShow=false;

//���캯��
CLongonDialog::CLongonDialog() : CLoadFileBmp(IDD_LOGON)
{
//	if(Glb().m_CenterServerIPAddr.Compare("127.0.0.1")==0)
		m_bshow=true;//false����ʾ�û���������
//	else
//		m_bshow=false;//false����ʾ�û���������
	//m_bshow=false;
	//m_bshow=true;
	LoadRegisterData();//��ȡע�����Ϣ
	bMouseDown=false;
	szLogAdvUrl="";
	hSoftKey=NULL;
	m_bkBrush = NULL;
	m_bIsShow = true;
}

//��������
CLongonDialog::~CLongonDialog()
{
	m_bIsShow = false;
	DeleteObject(m_bkBrush);
}
//DDX/DDV ֧��
void CLongonDialog::DoDataExchange(CDataExchange * pDX)
{
	CLoadFileBmp::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btLogon);
	DDX_Control(pDX, IDC_APPLY_PASSWORD, m_btApplyPassword);
	DDX_Control(pDX, IDC_FIND_PASSWORD, m_btFindPassword);
	DDX_Control(pDX, IDC_NEWHELP, m_btNewHelp);
	DDX_Control(pDX, IDC_PASS, m_PassWord);
	DDX_Control(pDX, IDCANCEL, m_btQuit);
	DDX_Control(pDX, IDC_REGISTER, m_btRegister);
	DDX_Control(pDX, IDC_DELETE, m_btDeleteName);
	DDX_Control(pDX, IDC_STATIC_2, m_static_2);
	DDX_Control(pDX, IDC_STATIC_3, m_static_3);
	//DDX_Control(pDX, IDC_KIND_NAME, m_static_Name);
	//DDX_Control(pDX, IDC_KIND_NAME2, m_static_Name);
	//DDX_Control(pDX, IDC_USER_PROXY, m_check_1);
	DDX_Control(pDX, IDC_REM_PASS, m_check_2);
	DDX_Control(pDX, IDC_WEB,m_Web);
	//DDX_Control(pDX, IDC_COPYRIGHT,m_copyright);
	DDX_Control(pDX, IDC_STATIC10, m_state10);
	DDX_Control(pDX, IDC_STATIC5, m_state5);
	DDX_Control(pDX, IDC_SOFTKEY ,m_btSoftkey);
}
//��Ϣ����
BOOL CLongonDialog::PreTranslateMessage(MSG * pMsg)
{
	if(pMsg->message==WM_LBUTTONUP)
		bMouseDown=false;

	if(pMsg->message==WM_SYSCOMMAND && pMsg->wParam==SC_RESTORE)
	{
		ShowWindow(SW_RESTORE);
		return TRUE;
	}
	if(pMsg->message==WM_USER+100)
	{
		CString pass;
		m_PassWord.GetWindowText(pass);
		CString sChar;
		sChar.Format("%c",pMsg->wParam);
		pass+=sChar;
		m_PassWord.SetWindowText(pass);
		return TRUE;
	}
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_F1)
			return TRUE;
	}
	//if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	//{
	//	return TRUE;
	//}


	m_ToolTip.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//ȷ��
void CLongonDialog::OnOK()
{
#ifdef AI_MACHINE
	AILogin();
	
	//��������
		MD5_CTX Md5;
		Md5.MD5Update((unsigned char *)m_szScrPass,lstrlen(m_szScrPass));
		unsigned char szMDTemp[16];
		Md5.MD5Final(szMDTemp);
		for (int i=0;i<16;i++) 
			wsprintf(&m_szMD5Pass[i*2],"%02x",szMDTemp[i]);

		return __super::OnOK();
#endif

	if(!m_bshow)
	{
		m_bRemberPass=false;
		if(!m_bisgetid)
			return ;
		KillTimer(1);
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
		__super::OnOK();
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
				if (GetDlgItemText(IDC_NAME_INFO,m_szName,sizeof(m_szName))==0)
				{
					BZSoundPlay(this, "music/������ʾ.mp3", 0);
					strMessage = fMsg.GetKeyVal("LogonDlg","InPutUserName","����д�û�����");
					DUIOkeyMsgBox(m_hWnd,false,strMessage);
					GetDlgItem(IDC_NAME_INFO)->SetFocus();
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

		//Ч���û�����
		if (m_PassWord.GetWindowText(m_szScrPass,sizeof(m_szScrPass))==0)
		{
			BZSoundPlay(this, "music/������ʾ.mp3", 0);
			strMessage = fMsg.GetKeyVal("LogonDlg","InPutPW","����д���룡");
			
			//AFCMessageBox(strMessage);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,strMessage);

			GetDlgItem(IDC_PASS)->SetFocus();
			return;
		}

		//Ч���½������
		if (GetDlgItemText(IDC_SERVER_LIST,m_szGameServer,sizeof(m_szGameServer))==0)
		{
			BZSoundPlay(this, "music/������ʾ.mp3", 0);
			strMessage = fMsg.GetKeyVal("LogonDlg","ServerAddrError","��������ַ����ȷ��");
			
			//AFCMessageBox(strMessage);
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,strMessage);

			GetDlgItem(IDC_SERVER_LIST)->SetFocus();
			return;
		}


		GetDlgItemText(IDC_SERVER_LIST,Glb().m_CenterServerPara.m_strMainserverIPAddr,sizeof(Glb().m_CenterServerPara.m_strMainserverIPAddr));//���� �����IP
		//((CComboBox *)GetDlgItem(IDC_SERVER_LIST))->GetWindowText(Glb().m_CenterServerPara.m_strMainserverIPAddr);

		//��¼��Ϣ
		m_bRemberPass=(m_check_2.GetCheck()==BST_CHECKED);
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
		__super::OnOK();
	}
}

//��ʼ������
BOOL CLongonDialog::OnInitDialog()
{
	__super::OnInitDialog();
	HICON hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);/////////ϵͳͼ��
	SetIcon(hIcon,TRUE);
	

	//�����ؼ�
	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	///��Ȩ
	//���õ�¼����ı���
	SetWindowTitle(TEXT(Glb().m_LoginTit));
	m_bisgetid=false;

	if (m_bRemberPass) 
		m_check_2.SetCheck(BST_CHECKED);
	else
		m_check_2.SetCheck(BST_UNCHECKED);
	
	//((CEdit *)GetDlgItem(IDC_SERVER_LIST))->LimitText(sizeof(m_szGameServer)/sizeof(m_szGameServer[0])-1);
	((CEdit *)GetDlgItem(IDC_SERVER_LIST))->LimitText(15);
	
	if (m_bRegUser)
	{
		m_bLogonType=LK_BY_NAME;
		//GetDlgItem(IDC_ACC)->EnableWindow(FALSE);
		//GetDlgItem(IDC_MOBILE)->EnableWindow(FALSE);
	}


	SetDlgItemText(IDC_SERVER_LIST,Glb().m_CenterServerPara.m_strMainserverIPAddr);//"�Զ�ѡ���ٽ�������");

	//���ؼ���Ϣ
	FillUserName((CComboBox *)GetDlgItem(IDC_NAME_INFO));
	FillServer((CComboBox *)GetDlgItem(IDC_SERVER_LIST));
	SetLogonType(m_bLogonType,m_bLogonType);


	CenterWindow();
	m_bRegUser=false;

	{
		GetDlgItem(IDC_NAME_INFO)->ShowWindow(1);
		GetDlgItem(IDOK)->ShowWindow(1);
		GetDlgItem(IDC_PASS)->ShowWindow(1);
		GetDlgItem(IDC_STATIC10)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_2)->ShowWindow(1);
		GetDlgItem(IDC_DELETE)->ShowWindow(1);
		GetDlgItem(IDC_REGISTER)->ShowWindow(1);
		GetDlgItem(IDC_REM_PASS)->ShowWindow(1);
		GetDlgItem(IDC_STATIC5)->ShowWindow(1);
		GetDlgItem(IDC_CODE)->ShowWindow(1);
		((CEdit *)GetDlgItem(IDC_CODE))->SetLimitText(4);
		wsprintf(m_code,"%d%d%d%d",rand()%10,rand()%10,rand()%10,rand()%10);
		//this->m_Web.ShowWindow(0);

		if(!Glb().m_release)
		{
			m_check_2.SetCheck(1);
			m_bRemberPass=true;
			((CEdit *)GetDlgItem(IDC_CODE))->SetWindowText(m_code);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	//��ȡ��¼���ڹ���ַ,Fred Huang,2008-05-15
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	szLogAdvUrl=f.GetKeyVal (Glb().m_key,"logonadvertise","");
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//��ʼ�������,Fred Huang,2008-05-15
	hSoftKey=new CSoftKey;
	hSoftKey->Create(IDD_SOFTKEY,this);
	hSoftKey->pWnd=this;
	hSoftKey->ShowWindow(SW_HIDE);
	//////////////////////
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString strMessage;
	strMessage = fMsg.GetKeyVal("LogonDlg","Logon","��½");
	m_btLogon.SetWindowText(strMessage);
	strMessage = fMsg.GetKeyVal("LogonDlg","Register","ע��");
	m_btRegister.SetWindowText(strMessage);
	strMessage = fMsg.GetKeyVal("LogonDlg","FindPW","�һ�����");
	m_btFindPassword.SetWindowText(strMessage);
	strMessage = fMsg.GetKeyVal("LogonDlg","NewHelp","����ָ��");
	m_btNewHelp.SetWindowText(strMessage);
	strMessage = fMsg.GetKeyVal("LogonDlg","DeleteName","���");
	m_btDeleteName.SetWindowText(strMessage);
	//////////////////////////////////////////////////////////////////////////

	LoadSkin();

	m_bkBrush = CreateSolidBrush(m_bkcolor); 

	CenterWindow();

	GetDlgItem(IDC_NAME_INFO)->SetFocus();
	::PostMessage(this->m_hWnd,WM_KEYDOWN,VK_TAB,0);//����tab����Ϣ add by ��ǿ�� 2010-11-8

    // PengJiLin, 2010-6-2, ���� Ÿ�� ��¼����
    if((TRUE == g_bRunBySoftware) && (TRUE == g_bAutoConnect))
    {
        SetDlgItemText(IDC_NAME_INFO, g_chUserName);        
        m_PassWord.SetWindowText(TEXT("123456"));       // �����ٵģ��������

        OnOK();     // �Զ���¼

        lstrcpy(m_szMD5Pass, g_chMD5PSW);   // ���ں��棬���ⱻ�޸�
        g_bAutoConnect = FALSE;             // ֻ��һ���Զ���¼

        return FALSE;
    }


#ifdef AI_MACHINE
	OnOK();
#endif
	return FALSE;
}

////ʹ�ô��������
//void CLongonDialog::OnBnClickedUseProxy()
//{
//	//�ƶ�����
//	m_bUseProxy=!m_bUseProxy;
//	int iWidth=m_bRegUser?m_uMaxWidth:m_NormalWindowRect.Width();
//	int iHidth=m_bUseProxy?m_uMaxHide:m_NormalWindowRect.Height();
//	SetWindowPos(GetParent(),0,0,iWidth,iHidth,SWP_NOMOVE);
//
//	//���ÿؼ�
//	BOOL bUserPass=(((CButton *)GetDlgItem(IDC_USE_PASS))->GetCheck()==BST_CHECKED);
//	GetDlgItem(IDC_PROXY_SERVER)->EnableWindow(m_bUseProxy);
//	GetDlgItem(IDC_SERVER_PORT)->EnableWindow(m_bUseProxy);
//	GetDlgItem(IDC_USE_PASS)->EnableWindow(m_bUseProxy);
//	GetDlgItem(IDC_SOCKS5)->EnableWindow(m_bUseProxy);
//	GetDlgItem(IDC_SOCKS4)->EnableWindow(m_bUseProxy);
//	GetDlgItem(IDC_PROXY_NAME)->EnableWindow(bUserPass&&m_bUseProxy);
//	GetDlgItem(IDC_PROXY_PASS)->EnableWindow(bUserPass&&m_bUseProxy);
//
//	//���ý���
//	if (m_bUseProxy==FALSE) 
//	{
//		if (m_bLogonType==LK_BY_NAME) GetDlgItem(IDC_NAME_INFO)->SetFocus();
//		else if (m_bLogonType==LK_BY_ACC) GetDlgItem(IDC_ACC_INFO)->SetFocus();
//		else if (m_bLogonType==LK_BY_MOBILE) GetDlgItem(IDC_MOBILE_INFO)->SetFocus();
//	}
//	else GetDlgItem(IDC_PROXY_SERVER)->SetFocus();
//
//	return;
//}

////���ô����Ƿ�ʹ�ð�ȫ��֤
//void CLongonDialog::OnBnClickedUsePass()
//{
//	//���ÿؼ� 
//	m_bUserCheck=(((CButton *)GetDlgItem(IDC_USE_PASS))->GetCheck()==BST_CHECKED);
//	GetDlgItem(IDC_PROXY_NAME)->EnableWindow(m_bUseProxy&&m_bUserCheck);
//	GetDlgItem(IDC_PROXY_PASS)->EnableWindow(m_bUseProxy&&m_bUserCheck);
//
//	//���ý���
//	if (m_bUserCheck) GetDlgItem(IDC_PROXY_NAME)->SetFocus();
//	else GetDlgItem(IDC_PROXY_SERVER)->SetFocus();
//	return;
//}

//�������û�
void CLongonDialog::OnBnClickedRegUser()
{
	CString s=CBcfFile::GetAppPath ();/////����·��

	CBcfFile fskin( s + "skin.bcf");
	CString key="LogonDialog";//Glb().m_skin;
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
	if(Dlg.DoModal()==IDOK)//�����ע�ᰴŤ
	{
		GetDlgItem(IDC_NAME_INFO)->SetWindowText(Dlg.m_regname);//ע������
		GetDlgItem(IDC_PASS)->SetWindowText("");//ע������
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString strTip = fMsg.GetKeyVal("LogonDlg","SuccessTip","ע��ɹ�");
		CString strMessage = fMsg.GetKeyVal("LogonDlg","RegSuccess","��ϲ��ע���û��ɹ�\n������ղ���ע����û�����������е�¼��");
		
		//AFCMessageBox(strMessage,strTip);
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,strTip,false,strMessage);
		return;
	}
}

//��ȡע�����Ϣ
bool CLongonDialog::LoadRegisterData()
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

//���õ�½ģʽ
bool CLongonDialog::SetLogonType(BYTE bNewLogonType, BYTE bOldLogonType)
{
	BYTE uLogonKind[3]={LK_BY_NAME,LK_BY_ACC,LK_BY_MOBILE};
	UINT uButtonConID[3]={IDC_ACCOUNTS,IDC_ACC,IDC_MOBILE};
	UINT uInputConID[3]={IDC_NAME_INFO,IDC_ACC_INFO,IDC_MOBILE_INFO};
	
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
//	TCHAR * szTipText[3]={TEXT("�û����ƣ�"),TEXT("�û����ƣ�"),TEXT("�û����ƣ�")};
	TCHAR szTipText[3][128];
	CString strMessage = fMsg.GetKeyVal("LogonDlg","Name","�û����ƣ�");
	StrCpyN(&szTipText[0][0],strMessage.GetBuffer(),strMessage.GetLength()+1);
	StrCpyN(&szTipText[1][0],strMessage.GetBuffer(),strMessage.GetLength()+1);
	StrCpyN(&szTipText[2][0],strMessage.GetBuffer(),strMessage.GetLength()+1);

	for (BYTE i=0;i<3;i++)
	{
		//��������
		if ((bNewLogonType!=bOldLogonType)&&(bOldLogonType==uLogonKind[i]))
		{
			((CButton *)GetDlgItem(uButtonConID[i]))->SetCheck(BST_UNCHECKED);
			GetDlgItem(uInputConID[i])->ShowWindow(SW_HIDE);
		}

		//��ʾ����
		if (bNewLogonType==uLogonKind[i])
		{
			//���ÿؼ�
			m_bLogonType=bNewLogonType;
			CComboBox * pCommboBoxInput=(CComboBox *)GetDlgItem(uInputConID[i]);
			((CButton *)GetDlgItem(uButtonConID[i]))->SetCheck(BST_CHECKED);
			pCommboBoxInput->ShowWindow(SW_SHOW);
			pCommboBoxInput->SetFocus();
			
			//��������
			CString strInputInfo;
			if ((pCommboBoxInput->GetWindowTextLength()==0)&&(pCommboBoxInput->GetCount()>0)) pCommboBoxInput->SetCurSel(0);
			pCommboBoxInput->GetWindowText(strInputInfo);
			UpdatePassWord(strInputInfo);
		}
	}
	return true;
}

//�ʺŵ�½
void CLongonDialog::OnBnClickedAccounts()
{
	SetLogonType(LK_BY_NAME,m_bLogonType);
	return;
}

////ACC ��½
//void CLongonDialog::OnBnClickedAccID()
//{
//	SetLogonType(LK_BY_ACC,m_bLogonType);
//	return;
//}
//
////�ֻ���½
//void CLongonDialog::OnBnClickedMobile()
//{
//	SetLogonType(LK_BY_MOBILE,m_bLogonType);
//	return;
//}

//��д�û�
bool CLongonDialog::FillUserName(CComboBox * pComBoxName)
{
	CRegKey NameList;
	NameList.Create(HKEY_CURRENT_USER,REG_NAME);/////////����ע����û�
	if (NameList!=NULL)
	{
		DWORD uIndex=0;
		do
		{
			TCHAR szSubItemName[61]=TEXT("");
			DWORD dwBufferSize=sizeof(szSubItemName);
			if (NameList.EnumKey(uIndex++,szSubItemName,&dwBufferSize)!=ERROR_SUCCESS) break;
			if (szSubItemName[0]!=0) 
			{
				if (m_szName[0]==0) lstrcpy(m_szName,szSubItemName);
				if (pComBoxName->FindString(0,szSubItemName)==-1) pComBoxName->AddString(szSubItemName);
			}
		} while (TRUE);
	}
	return true;
}

//��д��½������
bool CLongonDialog::FillServer(CComboBox * pComBoxServer)
{
	return true;
	CRegKey ServerList;
	ServerList.Create(HKEY_CURRENT_USER,REG_SERVER);/////////����ע���REG_SERVER
	if (ServerList!=NULL)
	{
		DWORD uIndex=0;
		do
		{
			TCHAR szSubItemName[51]=TEXT("");
			DWORD dwBufferSize=sizeof(szSubItemName);
			if (ServerList.EnumKey(uIndex++,szSubItemName,&dwBufferSize)!=ERROR_SUCCESS) break;
			if (szSubItemName[0]!=0) 
			{
				if (m_szGameServer[0]==0)
						lstrcpy(m_szGameServer,szSubItemName);
				lstrcpy(Glb().m_CenterServerPara.m_strMainserverIPAddr,szSubItemName);//ȡ���ϴε�½��IP
				pComBoxServer->AddString(szSubItemName);
				if (pComBoxServer->FindString(0,szSubItemName)==-1)	pComBoxServer->AddString(szSubItemName);
			}
		} while (TRUE);
	}

	//��������
	if ((pComBoxServer->GetWindowTextLength()==0)&&(pComBoxServer->GetCount()>0)) 
		pComBoxServer->SetCurSel(0);

	return true;
}

//��������
bool CLongonDialog::UpdatePassWord(CString strInputInfo)
{
	//������ʾ����
	CString strSourcePass=GetUserPass(strInputInfo,m_bLogonType);
	if (strSourcePass.GetLength()<sizeof(m_szScrPass)/sizeof(m_szScrPass[0])) lstrcpy(m_szScrPass,strSourcePass);
	else m_szScrPass[0]=0;
	m_PassWord.SetWindowText(m_szScrPass);

	return true;
}

//д��ע���������Ϣ
bool CLongonDialog::WriteUserPass(const TCHAR * szKindInfo, TCHAR * szPass, UINT uType)
{
	//д��ע���
	if ((szKindInfo!=NULL)&&(szKindInfo[0]!=0))
	{
		//����ע����ַ�
		CString strRegKey;
		CRegKey PassWord;
		strRegKey.Format(TEXT("%s\\%s"),REG_SERVER,Glb().m_CenterServerPara.m_strMainserverIPAddr);
		PassWord.Create(HKEY_CURRENT_USER,strRegKey);
		PassWord.Close();
		//AfxMessageBox(strRegKey);
		if (uType==LK_BY_NAME)	strRegKey.Format(TEXT("%s\\%s"),REG_NAME,szKindInfo);
		else if (uType==LK_BY_ACC) strRegKey.Format(TEXT("%s\\%s"),REG_ACC,szKindInfo);
		else if (uType==LK_BY_MOBILE) strRegKey.Format(TEXT("%s\\%s"),REG_MOBILE,szKindInfo);
		else return NULL;

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

//��ȡע���������Ϣ
CString CLongonDialog::GetUserPass(const TCHAR * szKindInfo, UINT uType)
{
	//��ȡע���
	if ((szKindInfo!=NULL)&&(szKindInfo[0]!=0))
	{
		//����ע����ַ�
		CString strRegKey;
		if (uType==LK_BY_NAME)	strRegKey.Format(TEXT("%s\\%s"),REG_NAME,szKindInfo);
		else if (uType==LK_BY_ACC) strRegKey.Format(TEXT("%s\\%s"),REG_ACC,szKindInfo);
		else if (uType==LK_BY_MOBILE) strRegKey.Format(TEXT("%s\\%s"),REG_MOBILE,szKindInfo);
		else return NULL;

		//��ע���
		CRegKey PassWord;
		PassWord.Open(HKEY_CURRENT_USER,strRegKey);
		if (PassWord!=NULL)
		{
			TCHAR szPassBuffer[81];
			ULONG uBufferLength=sizeof(szPassBuffer);
			LONG uErrorCode=PassWord.QueryStringValue(TEXT("EncryptPass"),szPassBuffer,&uBufferLength);
			if (uErrorCode==ERROR_SUCCESS) return CrevassePass(szPassBuffer);
		}

		//���������IP
PassWord.Close();
strRegKey.Format(TEXT("%s\\%s"),REG_SERVER,Glb().m_CenterServerPara.m_strMainserverIPAddr);
PassWord.Create(HKEY_CURRENT_USER,strRegKey);


	}
	return NULL;
}

//��������
CString CLongonDialog::EncryptPass(const TCHAR * szPass)
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
CString CLongonDialog::CrevassePass(const TCHAR * szEncryPass)
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
//ѡ��ı�
void CLongonDialog::OnCbnSelchangeInput()
{
	BYTE uLogonKind[3]={LK_BY_NAME,LK_BY_ACC,LK_BY_MOBILE};
	UINT uInputConID[3]={IDC_NAME_INFO,IDC_ACC_INFO,IDC_MOBILE_INFO};
	for (int i=0;i<3;i++)
	{
		if (m_bLogonType==uLogonKind[i])
		{
			CComboBox * pCommboBoxInput=(CComboBox *)GetDlgItem(uInputConID[i]);
			int iSelItem=pCommboBoxInput->GetCurSel();
			if (iSelItem!=CB_ERR)
			{
				CString strInputInfo;
				pCommboBoxInput->GetLBText(iSelItem,strInputInfo);
				UpdatePassWord(strInputInfo);
				return;
			}
		}
	}
	return;
}

//ɾ����ť
void CLongonDialog::OnBnClickedDelete()
{
	BYTE uLogonKind[3]={LK_BY_NAME,LK_BY_ACC,LK_BY_MOBILE};
	UINT uInputConID[3]={IDC_NAME_INFO,IDC_ACC_INFO,IDC_MOBILE_INFO};
	for (int i=0;i<3;i++)
	{
		if (m_bLogonType==uLogonKind[i])
		{
			//��ȡ��Ϣ
			CString strInputInfo;
			CComboBox * pCommboBoxInput=(CComboBox *)GetDlgItem(uInputConID[i]);
			pCommboBoxInput->GetWindowText(strInputInfo);

			//ɾ����Ϣ
			if (strInputInfo.GetLength()>0)
			{
				//����ע����ַ�
				CString strRegKey;
				if (m_bLogonType==LK_BY_NAME) strRegKey=REG_NAME;
				else if (m_bLogonType==LK_BY_ACC) strRegKey=REG_ACC;
				else if (m_bLogonType==LK_BY_MOBILE) strRegKey=REG_MOBILE;

				//��ע���
				CRegKey InputInfo;
				InputInfo.Open(HKEY_CURRENT_USER,strRegKey);
				if (InputInfo==NULL) return;
				InputInfo.RecurseDeleteKey(strInputInfo);

				//������ʾ
				int iItem=pCommboBoxInput->FindString(0,strInputInfo);
				if (iItem!=CB_ERR) pCommboBoxInput->DeleteString(iItem);
				pCommboBoxInput->SetWindowText(TEXT(""));
				m_PassWord.SetWindowText(TEXT(""));
			}

			//��ʾ����Ϣ
			pCommboBoxInput->SetFocus();
			if (pCommboBoxInput->GetCount()>0)
			{
				pCommboBoxInput->SetCurSel(0);
				pCommboBoxInput->GetLBText(0,strInputInfo);
				UpdatePassWord(strInputInfo);
			}

			return;
		}
	}
	return;
}

//����ͷ��
void CLongonDialog::OnBnClickedSelectLogo()
{
	CRect SelcetRect,SelectDlgRect;
	if (m_LogoSelect.GetSafeHwnd()==NULL) m_LogoSelect.Create(IDD_LOGO_SELECT,this);
	m_LogoSelect.GetWindowRect(&SelectDlgRect);
	m_LogoSelect.MoveWindow(SelcetRect.right-SelectDlgRect.Width(),SelcetRect.top-SelectDlgRect.Height(),SelectDlgRect.Width(),SelectDlgRect.Height());
	m_LogoSelect.SetVisible();
	m_LogoSelect.ShowWindow(SW_SHOW);
	m_LogoSelect.BringWindowToTop();
	return;
}

//������Ϣ
LRESULT CLongonDialog::OnHitLogoMessage(WPARAM wparam, LPARAM lparam)
{
	m_bLogoID=(UINT)wparam;
	return 0;
}

/*******************************************************************************************************/
void CLongonDialog::OnPaint()
{
//	CLoadFileBmp::OnPaint();
	CPaintDC dc(this);
	dc.SetBkMode(TRANSPARENT);
	CGameImageHelper TopListBarBottomHandle(&m_bkimage);
	AFCStretchImage(&dc,
		0,0,		
		m_bkimage.GetWidth(),m_bkimage.GetHeight(),
		m_bkimage,
		0,0,
		m_bkimage.GetWidth(),m_bkimage.GetHeight(),
		m_bkimage.GetPixel(0,0));
	if(!Glb().m_weblogon)
	{
		CRect rect;
		GetDlgItem(IDC_CODE)->GetClientRect(&rect);
		GetDlgItem(IDC_CODE)->MapWindowPoints(this,&rect);
		CFont font, *oldFont = NULL;
		font.CreateFont(-12,0,0,0,400,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
		oldFont = dc.SelectObject(&font);
		dc.SetTextColor(RGB(0,0,0));
		//dc.TextOut(rect.right +10,rect.top,m_code);

		///{{ Added by zxd 20100709 �ͷ�δ�ͷŵ�GDI��Դ
		dc.SelectObject(oldFont);
		font.DeleteObject();
		/// Added by zxd 20100709 �ͷ�δ�ͷŵ�GDI��Դ}}
	}

/*	
	CGameImage image;
	image.SetLoadInfo(".\\image\\logmin.bmp",CGameImageLink::m_bAutoLock);
	CGameImageHelper img(&image);
	AFCStretchImage(&dc,59,246,image.GetWidth(),image.GetHeight(),img,0,0,image.GetWidth(),image.GetHeight(),image.GetPixel(1,1));
	

	///��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ��Ȩ
//dc.SetBkColor(RGB(0, 0, 0));//��ɫ
dc.SetBkMode(TRANSPARENT);//͸����, ��Ȼ��, �����ı�����ɫ
dc.SelectObject(&CGameImageLink::m_HFont);//��ʾ����
dc.SetTextColor(RGB(0, 0, 0));//��ɫ
dc.DrawText(TMLcopyright,lstrlen(TMLcopyright),CRect(142,196,251,315),DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);
/*dc.SetTextColor(RGB(10, 20, 10));//��ɫ
dc.DrawText(TMLcopyright,lstrlen(TMLcopyright),CRect(141,207,251,327),DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);
dc.SetTextColor(RGB(0, 0, 0));//��ɫ
dc.DrawText(TMLcopyright,lstrlen(TMLcopyright),CRect(140,206,250,326),DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);
		//	pDC->SetTextColor(RGB(255, 255, 0));//��ɫ
		//pDC->DrawText(szBuffer,lstrlen(szBuffer),&m_IndexRect,DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);
*/
}
void CLongonDialog::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(1);
	CLoadFileBmp::OnCancel();
}


void CLongonDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	bMouseDown=true;
	savePoint=point;
	CLoadFileBmp::OnLButtonDown(nFlags, point);
}


LRESULT CLongonDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
		SetBkColor((HDC)wParam, m_bkcolor);
		return (LRESULT)m_bkBrush;
	}
	return CLoadFileBmp::WindowProc(message, wParam, lParam);
}
//
void CLongonDialog::LoadSkin(void)
{
	CRect rect;
	int r,g,b;
	int cx,cy;
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "skin.bcf");
	CString key="LogonDialog";//Glb().m_skin;
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal("skin0","skinfolder","image\\client\\skin0\\");

	wsprintf(path,"%slogon\\logon_bk.bmp",skinfolder);
	m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper ImageHandle(&m_bkimage);
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_bkimage.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		DeleteObject(hRgn);
	}
	cx=ImageHandle.GetWidth();
	cy=ImageHandle.GetHeight();
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);

	CGameImage		m_bt;
	int x,y,w1,w2,w,h,exitx,exity,okx,oky,regx,regy;
	int findx,findy,applyx,applyy,newhelpx,newhelpy;
	x=f.GetKeyVal(key,"x",32);
	y=f.GetKeyVal(key,"y",107);
	w=f.GetKeyVal(key,"w",143);
	w1=f.GetKeyVal(key,"w1",55);
	w2=f.GetKeyVal(key,"w2",4);
	h=f.GetKeyVal(key,"h",35);
	exitx=f.GetKeyVal(key,"exitx",326);
	exity=f.GetKeyVal(key,"exity",6);
	okx=f.GetKeyVal(key,"okx",85);
	oky=f.GetKeyVal(key,"oky",184);
	regx=f.GetKeyVal(key,"logx",218);
	regy=f.GetKeyVal(key,"logy",184);	

	newhelpx=f.GetKeyVal(key,"newhelpx",180);
	newhelpy=f.GetKeyVal(key,"newhelpy",180);
	
	findx=f.GetKeyVal(key,"findx",85);
	findy=f.GetKeyVal(key,"findy",184);
	applyx=f.GetKeyVal(key,"applyx",-200);
	applyy=f.GetKeyVal(key,"applyy",-200);	
	
	r=f.GetKeyVal(key,"lbkr",255);	
	g=f.GetKeyVal(key,"lbkg",255);	
	b=f.GetKeyVal(key,"lbkb",191);
	int nIsShowBtnText = f.GetKeyVal(key,"IsShowBtnText",0);
	m_bkcolor=RGB(r,g,b);

	wsprintf(path,"%slogon\\button_bt1.bmp",skinfolder);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_btLogon.LoadButtonBitmap(path,false);
	HRGN hRgn2;
	hRgn2=AFCBmpToRgn(m_bt,RGB(255,0,255),RGB(1,1,1));
	if(hRgn2)
	{
		m_btLogon.SetWindowRgn(hRgn2,true);
		DeleteObject(hRgn2);
	}
	m_btLogon.MoveWindow(okx,oky,m_bt.GetWidth()/4,m_bt.GetHeight(),false);
	m_btLogon.SetPlayMusic(false);

	wsprintf(path,"%slogon\\button_bt2.bmp",skinfolder);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_btRegister.LoadButtonBitmap(path,false);
	hRgn2=AFCBmpToRgn(m_bt,RGB(255,0,255),RGB(1,1,1));
	if(hRgn2)
	{
		m_btRegister.SetWindowRgn(hRgn2,true);
		DeleteObject(hRgn2);
	}
	m_btRegister.MoveWindow(regx,regy,m_bt.GetWidth()/4,m_bt.GetHeight(),false);

	wsprintf(path,"%slogon\\log_exit_bt.bmp",skinfolder);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_btQuit.LoadButtonBitmap(path,false);
	hRgn2=AFCBmpToRgn(m_bt,RGB(255,0,255),RGB(1,1,1));
	if(hRgn2)
	{
		m_btQuit.SetWindowRgn(hRgn2,true);
		DeleteObject(hRgn2);
	}
	m_btQuit.MoveWindow(exitx,exity,m_bt.GetWidth()/4,m_bt.GetHeight(),false);

	wsprintf(path,"%slogon\\button_bt5.bmp",skinfolder);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_btDeleteName.LoadButtonBitmap(path,false);
	hRgn2=AFCBmpToRgn(m_bt,RGB(255,0,255),RGB(1,1,1));
	if(hRgn2)
	{
		m_btDeleteName.SetWindowRgn(hRgn2,true);
		DeleteObject(hRgn2);
	}
	m_btDeleteName.MoveWindow(x+w1+w+w2,y-(m_bt.GetHeight()-20)/2,m_bt.GetWidth()/4,m_bt.GetHeight(),false);

	wsprintf(path,"%slogon\\button.bmp",skinfolder);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_btApplyPassword.LoadButtonBitmap(path,false);

	hRgn2=AFCBmpToRgn(m_bt,RGB(255,0,255),RGB(1,1,1));
	if(hRgn2)
	{
		m_btApplyPassword.SetWindowRgn(hRgn2,true);
		DeleteObject(hRgn2);
	}
	m_btApplyPassword.MoveWindow(applyx,applyy,m_bt.GetWidth()/4,m_bt.GetHeight(),false);

	wsprintf(path,"%slogon\\button_bt3.bmp",skinfolder);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_btFindPassword.LoadButtonBitmap(path,false);
	hRgn2=AFCBmpToRgn(m_bt,RGB(255,0,255),RGB(1,1,1));
	if(hRgn2)
	{
		m_btFindPassword.SetWindowRgn(hRgn2,true);
		DeleteObject(hRgn2);
	}
	m_btFindPassword.MoveWindow(findx,findy,m_bt.GetWidth()/4,m_bt.GetHeight(),false);

	// yinyi -080911 - begin ���ְ�����ť
	wsprintf(path,"%slogon\\button_bt4.bmp",skinfolder);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_btNewHelp.LoadButtonBitmap(path,false);
	hRgn2=::AFCBmpToRgn(m_bt,RGB(255,0,255),RGB(1,1,1));
	if(hRgn2)
	{
		m_btNewHelp.SetWindowRgn(hRgn2,true);
		DeleteObject(hRgn2);
	}
	m_btNewHelp.MoveWindow(newhelpx,newhelpy,m_bt.GetWidth()/4,m_bt.GetHeight(),false);
	
	// yinyi -080911 - end  ���ְ�����ť

	/// �������ã���ʾ����ʾ��ť�ϵ�����
	if (0==nIsShowBtnText)
	{
		SetDlgItemText(IDOK, TEXT(""));
		SetDlgItemText(IDC_REGISTER, TEXT(""));
		SetDlgItemText(IDC_APPLY_PASSWORD, TEXT(""));
		SetDlgItemText(IDC_FIND_PASSWORD, TEXT(""));
		SetDlgItemText(IDC_NEWHELP, TEXT(""));
		SetDlgItemText(IDC_DELETE, TEXT(""));
	}

	//��������б�
	int serverlist=f.GetKeyVal(key,"serverlist",1);
	if(serverlist==0)
	{
		GetDlgItem(IDC_KIND_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SERVER_LIST)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_KIND_NAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SERVER_LIST)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_KIND_NAME)->MoveWindow(x,y+4-h,w1,12,false);
		GetDlgItem(IDC_SERVER_LIST)->MoveWindow(x+w1,y-h,w,20,false);
	}

	//////////////////////////////////////////////////////////////////////////
	//����̰�ť,Fred Huang,2008-05-15
	int softkeyx=f.GetKeyVal(key,"softkeyx",20);
	int softkeyy=f.GetKeyVal(key,"softkeyy",140);
	wsprintf(path,"%slogon\\softkey_bt.bmp",skinfolder);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_btSoftkey.LoadButtonBitmap(path,false);
	hRgn2=AFCBmpToRgn(m_bt,RGB(255,0,255),RGB(1,1,1));
	if(hRgn2)
	{
		m_btSoftkey.SetWindowRgn(hRgn2,true);
		DeleteObject(hRgn2);
	}
	m_btSoftkey.MoveWindow(softkeyx,softkeyy,m_bt.GetWidth()/4,m_bt.GetHeight(),false);
	//////////////////////////////////////////////////////////////////////////

	//����ʺ�
	GetDlgItem(IDC_STATIC10)->MoveWindow(x,y+4,w1,12,false);
	GetDlgItem(IDC_NAME_INFO)->MoveWindow(x+w1,y,w,20,false);

	GetDlgItem(IDC_STATIC_2)->MoveWindow(x,y+4+h,w1,12,false);
	GetDlgItem(IDC_PASS)->MoveWindow(x+w1,y+h,w,20,false);
	GetDlgItem(IDC_REM_PASS)->MoveWindow(x+w1,y+h+h,13,13,false);

	//��ʹ����֤��
	GetDlgItem(IDC_STATIC5)->MoveWindow(x,y+4+h*2,w1,12,false);
	GetDlgItem(IDC_CODE)->MoveWindow(x+w1,y+h*2,53,20,false);
	GetDlgItem(IDC_STATIC5)->ShowWindow(0);
	GetDlgItem(IDC_CODE)->ShowWindow(0);


	//////////////////////////////////////////////////////////////////////////
	//��ȡIE���λ��,Fred Huang,2008-05-15
	if(szLogAdvUrl=="")
	{//���û�й���ַ���򲻼���
		logadvx=-1;
		logadvy=-1;
		logadvw=0;
		logadvh=0;
	}
	else
	{//��ȡ���λ��
		logadvx=f.GetKeyVal(key,"logadvx",5);
		logadvy=f.GetKeyVal(key,"logadvy",20);
		logadvw=f.GetKeyVal(key,"logadvw",100);
		logadvh=f.GetKeyVal(key,"logadvh",50);
		//����URL
		m_Web.Navigate(szLogAdvUrl,0,0,0,0);
	}
	m_Web.MoveWindow(logadvx,logadvy,logadvw,logadvh);
	//////////////////////////////////////////////////////////////////////////


}
#ifdef AI_MACHINE
void CLongonDialog::AILogin()
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
int CLongonDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CLoadFileBmp::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	//m_btLogon.Create(TEXT("��½"),WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,80,25),this,IDOK);
	//m_btRegister.Create(TEXT("ע��"),WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,80,25),this,IDC_REGISTER);
	//m_btDeleteName.Create(TEXT("���"),WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,80,25),this,IDC_DELETE);

	return 0;
}

//�Ҽ�����
void CLongonDialog::OnBnClickedFindPassword()
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

//���ְ���
void CLongonDialog::OnBnClickedNewHelp()
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


//�������뱣��
void CLongonDialog::OnBnClickedApplyPassword()
{
	TCHAR url[300];
	wsprintf(url,"%sapp/applypsw.%s",Glb().m_CenterServerPara.m_strWebRootADDR,Glb().m_urlpostfix);
	ShellExecute(NULL,TEXT("open"),url,NULL,NULL,SW_MAXIMIZE);
	return ;
}

LRESULT CLongonDialog::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{

	return CLoadFileBmp::DefWindowProc(message, wParam, lParam);
}

void CLongonDialog::OnBnClickedSoftkey()
{
	static bool bShowSoftKey=false;
	bShowSoftKey=!bShowSoftKey;
	if(bShowSoftKey)
	{
		CRect rc;
		GetWindowRect(&rc);
		hSoftKey->MoveWindow(rc.left,rc.bottom,348,130);
		hSoftKey->ShowWindow(SW_SHOW);
	}
	else
		hSoftKey->ShowWindow(SW_HIDE);
}

void CLongonDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	CLoadFileBmp::OnMouseMove(nFlags, point);
}
