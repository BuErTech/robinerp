// RegisterNewUserDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RegisterNewUserDlg.h"
#include "comutil.h"

// CRegisterNewUserDlg �Ի���

CRegisterNewUserDlg::CRegisterNewUserDlg(CWnd* pParent /*=NULL*/)
	: CLoadFileBmp(CRegisterNewUserDlg::IDD, pParent)
{
	m_bkBrush=NULL;
}

CRegisterNewUserDlg::~CRegisterNewUserDlg()
{
	DeleteObject(m_bkBrush);
}

void CRegisterNewUserDlg::DoDataExchange(CDataExchange* pDX)
{//�����͸��
	//��̬�ؼ�
	//DDX_Control(pDX, IDC_STATIC_1234567, m_static_CopyRight);
	
	DDX_Control(pDX, IDC_EXPLORER1, m_ie);
	DDX_Control(pDX,IDCANCEL,m_btQuit);
	__super::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegisterNewUserDlg, CLoadFileBmp)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
END_MESSAGE_MAP()


		//const TCHAR * szLimitName[]={TEXT("����"),TEXT("������"),TEXT("����ˮ"),TEXT("ë��"),TEXT("ϵͳ"),TEXT("����"),
		//	TEXT("����"),TEXT("��"),TEXT("����"),TEXT("��"),TEXT("����"),TEXT("FUCK"),TEXT("��B"),TEXT("��"),TEXT("��"),
		//	TEXT("���"),TEXT("�ڹ�"),TEXT("ϵͳ"),TEXT("���"),TEXT("��Ϸ����"),TEXT("��Ϣ"),
		//	TEXT("�н�"),TEXT("��Ϣ"),TEXT("�ͷ�"),TEXT("�ڰ�"),TEXT("�ڰ�"),TEXT("�ڰ�"),TEXT("����Ա"),TEXT("�ٷ�"),TEXT("game0437"),TEXT("�ͷ�����"),TEXT("�ͷ�����")
		//	TEXT("GM"),TEXT(" "),TEXT("����"),TEXT("�ǳ�"),TEXT("����")


BOOL CRegisterNewUserDlg::OnInitDialog()
{
	CLoadFileBmp::OnInitDialog();
//	SetWindowTitle("ע�����û�");

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	SetWindowTitle(fMsg.GetKeyVal("LogonDlg","RegNewUser","ע�����û�"));
	LoadSkin();

	//��ʼ����Ϸ
	CenterWindow();


	CRect ClientRect;
	GetClientRect(&ClientRect);
	this->MapWindowPoints(this->GetParent(),&ClientRect);
	CGameImageHelper	help(&m_bk);
	this->SetWindowPos(this->GetParent(),0,0,help.GetWidth(),help.GetHeight(),SWP_NOMOVE);

	CString s=CBcfFile::GetAppPath ();/////����·��

	CBcfFile fskin( s + "skin0.bcf");
	CString key=TEXT("RegisterUser");
	CString RegPageColor=fskin.GetKeyVal(key,"regpagebgcolor","");
	CBcfFile f( s + "bzgame.bcf");
	key="BZW";
	CString m_regurl=f.GetKeyVal (Glb().m_key,"AppRegUrl","http://localhost/app/regist.asp");
	CString m_ReCommend=f.GetKeyVal ("CustomSet","ReCommend","");
	m_regurl+="?bgcolor="+RegPageColor+"&recommend="+m_ReCommend;
	m_ie.Navigate(m_regurl,0,0,0,0);

	m_regname="";

	CenterWindow(AfxGetApp()->m_pMainWnd);

	
	return false;  // return TRUE unless you set the focus to a control
}


void CRegisterNewUserDlg::OnBnClickedCancel()
{
	OnCancel();
}

LRESULT CRegisterNewUserDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
		if (m_bkBrush == NULL) m_bkBrush = CreateSolidBrush(m_bkcolor);
		return (LRESULT)m_bkBrush;
	}
	return CLoadFileBmp::WindowProc(message, wParam, lParam);
}

void CRegisterNewUserDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect ClientRect;
	GetClientRect(&ClientRect);
	//this->MapWindowPoints(this->GetParent(),&ClientRect);
	CGameImageHelper	help(&m_bk);
	//this->SetWindowPos(this->GetParent(),0,0,help.GetWidth(),help.GetHeight(),SWP_NOMOVE);
	help.BitBlt(dc.GetSafeHdc(),0,0);
	
}



void CRegisterNewUserDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(point.y<=30)
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	//CLoadFileBmp::OnLButtonDown(nFlags, point);
}

void CRegisterNewUserDlg::LoadSkin(void)
{
	CRect rect;
//	int r,g,b;
	int cx,cy;
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("RegisterUser");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());

	wsprintf(path,"%slogon\\reg_bk.bmp",skinfolder);
	m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	bool needsetrgn=f.GetKeyVal(key,"regrgn",0);
	CGameImageHelper ImageHandle(&m_bk);
	if(needsetrgn)
	{
		HRGN hRgn=AFCBmpToRgn(ImageHandle,m_bk.GetPixel(0,0),RGB(1,1,1));
		if (hRgn!=NULL)
		{
			SetWindowRgn(hRgn,TRUE);
			///{{ Added by zxd 20100709 �ͷ��������
			DeleteObject(hRgn);
			///Added by zxd 20100709 �ͷ��������}}
		}
	}
	cx=ImageHandle.GetWidth();
	cy=ImageHandle.GetHeight();
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);
	//�ƶ�IE
	//m_ie.MoveWindow(15,45,cx-30,cy-65,false);
	m_ie.MoveWindow(6,28,cx-12,cy-33,false);//JMod-�����û�ע�ᴰ��-20090506
	////���漸���ǹر�ע�ᴰ���õ�
	wsprintf(path,"%slogon\\log_exit_bt.bmp",skinfolder);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_btQuit.LoadButtonBitmap(path,false);
	m_btQuit.MoveWindow(cx-36,0,m_bt.GetWidth()/4,m_bt.GetHeight(),false);
	m_btQuit.ShowWindow(SW_SHOW);

	return;

}

int CRegisterNewUserDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CLoadFileBmp::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


BEGIN_EVENTSINK_MAP(CRegisterNewUserDlg, CLoadFileBmp)
	ON_EVENT(CRegisterNewUserDlg, IDC_EXPLORER1, 250, CRegisterNewUserDlg::BeforeNavigate2Explorer1, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CRegisterNewUserDlg, IDC_EXPLORER1, 271, CRegisterNewUserDlg::NavigateErrorExplorer1, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

void CRegisterNewUserDlg::BeforeNavigate2Explorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	// TODO: �ڴ˴������Ϣ����������
	CString surl=(char*)_bstr_t(URL->bstrVal);
	if(surl.Find("bzwp://regsucceed")==0)
	{
		int uindex=surl.Find("username=");
		int uindex1 = surl.Find("userid=");
		m_regname=surl.Mid(uindex+9, uindex1 - uindex - 9 - 1);
		m_regID = surl.Mid(uindex1+7);
		CDialog::OnOK();
		*Cancel=true;
	}
}

void CRegisterNewUserDlg::NavigateErrorExplorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Frame, VARIANT* StatusCode, BOOL* Cancel)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString strTip = fMsg.GetKeyVal("LogonDlg","ErrorTip","������ʾ");
	CString strMessage = fMsg.GetKeyVal("LogonDlg","RegLinkError","����ע�������ʧ��");

	m_ie.Navigate("about:blank",0,0,0,0);
	
	//AFCMessageBox(strMessage,strTip);
	DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,strTip,false,strMessage);
	CDialog::OnCancel();
	*Cancel=true;
}
