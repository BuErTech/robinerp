#include "Stdafx.h"
#include "resource.h"
#include "checkPSW.h"

//������Կ����
#define KEY_LENGTH		10

char		CCheckPSW::m_szScrPass[61];					//�û�����
char		CCheckPSW::m_szMD5Pass[50];					//���ܺ������

BEGIN_MESSAGE_MAP(CCheckPSW, CLoadFileBmp)

	ON_WM_PAINT()
END_MESSAGE_MAP()


//���캯��
CCheckPSW::CCheckPSW() : CLoadFileBmp(IDD_CHECKPSW)
{

}

//��������
CCheckPSW::~CCheckPSW()
{
}

BOOL CCheckPSW::OnInitDialog()
{
	CLoadFileBmp::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString s=CBcfFile::GetAppPath();/////����·��
	CBcfFile f( s + "skin0.bcf");
	CString key=Glb().m_skin;
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal(Glb().m_skin,"skinfolder","image\\client\\skin0\\");

	wsprintf(path,"%slogon\\check_psw.bmp",skinfolder);
	m_bk.SetLoadInfo(path,TRUE);
	CenterWindow();
	CGameImageHelper help(&m_bk);
	HRGN hRgn;
	hRgn = AFCBmpToRgn(help,m_bk.GetPixel(0,0),RGB(1,1,1));
	if(hRgn)
		SetWindowRgn(hRgn,TRUE);
	
	((CEdit*)GetDlgItem(IDC_EDIT_TWOPSW))->SetLimitText(20);
	((CEdit*)GetDlgItem(IDC_EDIT_TWOPSW))->MoveWindow(130,67,80,16);
	CGameImage		m_bt;
	wsprintf(path,"%slogon\\button.bmp",skinfolder);
	m_BtCancel.LoadButtonBitmap(path,false);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper bthelp(&m_bt);
	HRGN hRgn2;
	hRgn2=AFCBmpToRgn(bthelp,m_bt.GetPixel(0,0),RGB(1,1,1));
	if(hRgn2)
		m_BtCancel.SetWindowRgn(hRgn2,true);

	wsprintf(path,"%slogon\\button.bmp",skinfolder);
	m_BtOk.LoadButtonBitmap(path,false);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	hRgn2=AFCBmpToRgn(bthelp,m_bt.GetPixel(0,0),RGB(1,1,1));
	if(hRgn2)
		m_BtOk.SetWindowRgn(hRgn2,true);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CCheckPSW::OnPaint()
{
	CPaintDC dc(this); 
 
	CGameImageHelper	help(&m_bk);
	help.BitBlt(dc.GetSafeHdc(),0,0);
}

void CCheckPSW::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	GetDlgItemText(IDC_EDIT_TWOPSW, m_szScrPass, sizeof(m_szScrPass));
	if(strlen(m_szScrPass) == 0|| strlen(m_szScrPass) > 32)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		
		//AFCMessageBox(fMsg.GetKeyVal("MainRoom","InputWithdrawalPW","������ȡ������"),fMsg.GetKeyVal("MainRoom","Tip","��ʾ"));
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,fMsg.GetKeyVal("MainRoom","Tip","��ʾ"),false,fMsg.GetKeyVal("MainRoom","InputWithdrawalPW","������ȡ������"));
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

void CCheckPSW::DoDataExchange(CDataExchange* pDX)
{
	// TODO: �ڴ����ר�ô����/����û���
	CLoadFileBmp::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL,m_BtCancel);	
	DDX_Control(pDX, IDOK,m_BtOk);
}
