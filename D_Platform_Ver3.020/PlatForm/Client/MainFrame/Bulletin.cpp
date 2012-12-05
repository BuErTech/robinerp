// Bulletin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Bulletin.h"


// CBulletin �Ի���

IMPLEMENT_DYNAMIC(CBulletin, CDialog)

CBulletin::CBulletin(CWnd* pParent /*=NULL*/)
	: CDialog(CBulletin::IDD, pParent)
{

}

CBulletin::~CBulletin()
{
}

void CBulletin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_ie);
	DDX_Control(pDX, IDC_BUTTON3, m_btClose);
	DDX_Control(pDX, IDC_BUTTON4, m_btClose2);
}


BEGIN_MESSAGE_MAP(CBulletin, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON3, OnCancel)
	ON_BN_CLICKED(IDC_BUTTON4, OnCancel)
	ON_MESSAGE(WM_EXCHANGE_SKIN,OnExchangeSkin)
END_MESSAGE_MAP()


// CBulletin ��Ϣ�������

BOOL CBulletin::OnInitDialog()
{
	CDialog::OnInitDialog();

	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
//����
void CBulletin::LoadSkin()
{
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CGameImage btn;
	TCHAR path[MAX_PATH];
	int nWidth = 0,nHeight = 0,timeCount = 0;

	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);

	wsprintf(path,strSkin + "dialog\\NoticePanel_bk_gr.bmp");	
	m_BackBitmap.SetLoadInfo(path,TRUE);

	nWidth = m_BackBitmap.GetWidth();
	nHeight = m_BackBitmap.GetHeight();
	SetWindowPos(NULL,0,0,nWidth,nHeight,SWP_NOMOVE|SWP_NOZORDER);	//���ñ���

	wsprintf(path,strSkin + "dialog\\close_bt.bmp"); 	//���ùرհ�ť
	btn.SetLoadInfo(path,true);

	wsprintf(path,strSkin + "dialog\\close_bt.bmp");
	m_btClose.LoadButtonBitmap(path,false);
	m_btClose.MoveWindow(nWidth - btn.GetWidth()/4-10, nHeight - btn.GetHeight()-3,btn.GetWidth()/4,btn.GetHeight());

	wsprintf(path,strSkin + "dialog\\log_exit_bt.bmp");//Rect_Close.bmp
	btn.SetLoadInfo(path,true);
	m_btClose2.LoadButtonBitmap(path,false);
	m_btClose2.MoveWindow(nWidth - btn.GetWidth()/4-10, 3,btn.GetWidth()/4,btn.GetHeight());

	ShowWindow(SW_HIDE);
}
void CBulletin::Init()
{
	CGameImage btn;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];

	int  nX, nY, nWidth, nHeight;
	CString strUrl;
	int timeCount;

	wsprintf(path,strSkin + "dialog\\NoticePanel_bk_gr.bmp");	
	m_BackBitmap.SetLoadInfo(path,TRUE);

	CBcfFile fInit( s + "bzgame.bcf");

	CString key= "IENotice";
	strUrl = fInit.GetKeyVal(key, "URL", _T("www.baidu.com"));
	nX = nY = 0;
	nWidth = m_BackBitmap.GetWidth();
	nHeight = m_BackBitmap.GetHeight();

	//���ñ���
	SetWindowPos(NULL,0,0,nWidth,nHeight,SWP_NOMOVE|SWP_NOZORDER);

	//���ùرհ�ť
	wsprintf(path,strSkin + "dialog\\close_bt.bmp");
	btn.SetLoadInfo(path,true);

	wsprintf(path,strSkin + "dialog\\close_bt.bmp");
	m_btClose.LoadButtonBitmap(path,false);
	m_btClose.MoveWindow(nWidth - btn.GetWidth()/4-10, nHeight - btn.GetHeight()-3,btn.GetWidth()/4,btn.GetHeight());

	wsprintf(path,strSkin + "dialog\\log_exit_bt.bmp");//Rect_Close.bmp
	btn.SetLoadInfo(path,true);
	m_btClose2.LoadButtonBitmap(path,false);
	m_btClose2.MoveWindow(nWidth - btn.GetWidth()/4-10, 3,btn.GetWidth()/4,btn.GetHeight());

	int nTopH = fInit.GetKeyVal(key, "TopH", 29);//�ϸߣ�
	int nBottomH = fInit.GetKeyVal(key, "BottomH", 39);//�¸�
	int nLeftW = fInit.GetKeyVal(key, "nLeftW", 3);//���
	int nRightW = fInit.GetKeyVal(key, "nRightW", 3);//�ҿ�
	//����IE
	m_ie.MoveWindow(nLeftW,nTopH,nWidth-nLeftW-nRightW,nHeight-nTopH-nBottomH);
	m_ie.Navigate(strUrl,0,0,0,0);
	ShowWindow(SW_HIDE);
}

void CBulletin::OpenBulletin(CString surl)
{
	m_ie.Navigate(surl,0,0,0,0);
}
void CBulletin::OnCancel()
{

	ShowWindow(SW_HIDE);
	//return CGameFaceGo::OnCancel();
}
//�ػ�����
void CBulletin::OnPaint()
{
	CPaintDC dc(this);

	//�滭��ͼ
	if (m_BackBitmap.IsNull()==false)
	{
		CRect ClientRect;
		GetClientRect(&ClientRect);
		CGameImageHelper ImageHandle(&m_BackBitmap);
		ImageHandle.BitBlt(dc.GetSafeHdc(),0,0);
		OutputDebugString("zhtlog:BitBlt");
	}

	return;
}
//����
LRESULT CBulletin::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	LoadSkin();
	//OnPaint();
	Invalidate();
	return LRESULT(0);
}
