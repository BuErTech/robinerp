// LockAccountDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "LockAccountDlg.h"
#include "MainRoomEx.h"


char		CLockAccountDlg::m_szScrPass[61];					//�û�����
char		CLockAccountDlg::m_szMD5Pass[50];					//���ܺ������

// CLockAccountDlg �Ի���

IMPLEMENT_DYNAMIC(CLockAccountDlg, CLoadFileBmp)

CLockAccountDlg::CLockAccountDlg(CWnd* pParent /*=NULL*/, int nType)
	: CLoadFileBmp(CLockAccountDlg::IDD, pParent)
{
	m_nShowType = nType;
	m_bkBrush = NULL;
}

CLockAccountDlg::~CLockAccountDlg()
{
	DeleteObject(m_bkBrush);
}

void CLockAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CLoadFileBmp::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL,m_BtCancel);	
	DDX_Control(pDX, IDOK,m_BtOk);
	DDX_Control(pDX, IDC_CLOSE, m_BtClose);
}


BEGIN_MESSAGE_MAP(CLockAccountDlg, CLoadFileBmp)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CLockAccountDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CLOSE, &CLockAccountDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDCANCEL, &CLockAccountDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLockAccountDlg ��Ϣ�������

BOOL CLockAccountDlg::OnInitDialog()
{
	CLoadFileBmp::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString s=CBcfFile::GetAppPath();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	SetDlgItemText(IDC_STATIC_BP,fMsg.GetKeyVal("LockAccountDlg","BankPW","�������룺"));
	
	// 2012.12.17 yyf ����
	////����ͼƬ
	//wsprintf(path,"%sdialog\\msg_bk.bmp",skinfolder);
	//m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//CenterWindow();
	//CGameImageHelper help(&m_bk);
	//HRGN hRgn;
	//hRgn = AFCBmpToRgn(help,m_bk.GetPixel(0,0),RGB(1,1,1));
	//if(hRgn)
	//	SetWindowRgn(hRgn,TRUE);

	//*-------��ʼ�� m_configDlg--------- 2012.10.15 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CLockAccountDlgConfig\\configDlg.bcf",s,skinfolder);// 
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	//-----------------------------------------------------*/
	CRect ClientRect;
	GetClientRect(&ClientRect);

	//this->MoveWindow(ClientRect.left,ClientRect.top,help.GetWidth(),help.GetHeight());
	this->MoveWindow(ClientRect.left,ClientRect.top,cx,cy);// 2012.12.17 yyf
	CenterWindow(GetParent());


	//ȡ����ť
	CGameImage		m_bt;
	wsprintf(path,"%sdialog\\msg_button.bmp",skinfolder);
	m_BtCancel.LoadButtonBitmap(path,false);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper bthelp(&m_bt);
	HRGN hRgn2;
	hRgn2=AFCBmpToRgn(bthelp,m_bt.GetPixel(0,0),RGB(1,1,1));
	if(hRgn2)
		m_BtCancel.SetWindowRgn(hRgn2,true);
	m_BtCancel.SetWindowText(fMsg.GetKeyVal("LockAccountDlg","Cancel","ȡ��"));

	CStatic * pStatic = (CStatic*)GetDlgItem(IDC_STATIC_TIP);

	//ȷ����ť
	wsprintf(path,"%sdialog\\msg_button.bmp",skinfolder);
	m_BtOk.LoadButtonBitmap(path,false);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	hRgn2=AFCBmpToRgn(bthelp,m_bt.GetPixel(0,0),RGB(1,1,1));
	if(hRgn2)
		m_BtOk.SetWindowRgn(hRgn2,true);
	if( m_nShowType == 0 )
	{
		m_BtOk.SetWindowText(fMsg.GetKeyVal("LockAccountDlg","LockMachine","����"));
		if( pStatic != NULL)
			pStatic->SetWindowText(fMsg.GetKeyVal("LockAccountDlg","LockAccountToMachine","���������ť������ʺ�ֻ���ڴ˼�\r\n�������Ϸ������������ڴ˼����\r\n�Ͻ��н���������"));
	}
	else
	{
		m_BtOk.SetWindowText(fMsg.GetKeyVal("LockAccountDlg","Unlock","����"));
		if( pStatic != NULL )
		pStatic->SetWindowText(fMsg.GetKeyVal("LockAccountDlg","LockAccountSuccess","�ʺ��Ѿ�ʹ�����������ܣ��������\r\n��ť���Խ��������"));
	}
	m_BtOk.SetPlayMusic(false);

	//�رհ�ť
	wsprintf(path,"%sdialog\\log_exit_bt.bmp",skinfolder);
	m_BtClose.LoadButtonBitmap(path,false);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_BtClose.MoveWindow(284, 0, m_bt.GetWidth()/4, m_bt.GetHeight(), FALSE);
	CGameImageHelper cHelper(&m_bt);

	//m_bkBrush = CreateSolidBrush(m_bk.GetPixel(32,55));
	m_bkBrush = CreateSolidBrush(m_configDlg.GetBKImageEX().GetPixel(32,55));//2012.10.17 yyf
	
	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
	return TRUE;   // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLockAccountDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CLoadFileBmp::OnPaint()

	/*// 2012.12.17 yyf ����
	CGameImageHelper	help(&m_bk);
	help.BitBlt(dc.GetSafeHdc(),0,0);*/

	//�滭��ܵ�ͼ 2012.10.17 yyf
	m_configDlg.DrawViewFrame(&dc);

	drawSkin();

}
void CLockAccountDlg::drawSkin()
{
	try
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CDC *pdc=this->GetWindowDC();
		if(!pdc)return;
		CRect rc;
		this->GetWindowRect(&rc);
		Graphics g(pdc->m_hDC);

		CString s="����";
		Font font(s.AllocSysString(),12,0,UnitPixel);
		SolidBrush brush(Color(255,255,255));
		if( m_nShowType == 0 )
		{
			s = fMsg.GetKeyVal("LockAccountDlg","LockThisMachine","��������");
		}
		else
		{
			s = fMsg.GetKeyVal("LockAccountDlg","UnlockThisMachine","��������");
		}
		g.DrawString(s.AllocSysString(),-1,&font,PointF(8,8),0,&brush);
		g.ReleaseHDC(pdc->m_hDC);
		m_BtClose.Invalidate(FALSE);
		
	    SetWndRgn();// ���ô���Բ��2012.10.17 yyf
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}
}
void CLockAccountDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ����ר�ô����/����û���
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	GetDlgItemText(IDC_BANK_PWD, m_szScrPass, sizeof(m_szScrPass));
	if(strlen(m_szScrPass) == 0|| strlen(m_szScrPass) > 32)
	{
		BZSoundPlay(this, "music/������ʾ.mp3", 0);
		
		//AFCMessageBox(fMsg.GetKeyVal("LockAccountDlg","InputBankPW","��������������"),fMsg.GetKeyVal("LockAccountDlg","Tip","��ʾ"));
	     DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,fMsg.GetKeyVal("LockAccountDlg","Tip","��ʾ"),false,fMsg.GetKeyVal("LockAccountDlg","InputBankPW","��������������"));
		return;
	}	
	//wushuqun 2009.6.1
	//�޸�֧��MD5 ��SHA ���ܷ�ʽ
	CenterServerMsg& par=Glb().m_CenterServerPara;
	//md5��ʽ
	if( par.m_nEncryptType == 1)
	{
		unsigned char szMDTemp[16];
		MD5_CTX Md5;
		Md5.MD5Update((unsigned char *)m_szScrPass,lstrlen(m_szScrPass));
		Md5.MD5Final(szMDTemp);
		for (int i = 0; i < 16; i ++) 
			wsprintf(&m_szMD5Pass[i * 2], "%02x", szMDTemp[i] );
	}
	//sha���ܷ�ʽ
	if(par.m_nEncryptType == 2)
	{
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
	//////////////////////////

	CLoadFileBmp::OnOK();
}

void CLockAccountDlg::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CLoadFileBmp::OnCancel();

	CMainRoomEx::m_bLockQuit = false;
}

LRESULT CLockAccountDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(message)
	{
	case WM_CTLCOLORSTATIC:
		{
			SetTextColor((HDC)wParam, RGB(0,0,0)) ;
			//SetBkColor((HDC)wParam, m_bk.GetPixel(32,55));
			SetBkColor((HDC)wParam, m_configDlg.GetBKImageEX().GetPixel(32,55));//2012.10.17 yyf
			return (LRESULT)m_bkBrush;
		}
	default:
		break;
	}
	return CLoadFileBmp::WindowProc(message, wParam, lParam);
}

void CLockAccountDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CMainRoomEx::m_bLockQuit = false;
	CLoadFileBmp::OnCancel();
}

// ���ô���Բ�� 2012.10.15 yyf
void CLockAccountDlg::SetWndRgn(void)
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