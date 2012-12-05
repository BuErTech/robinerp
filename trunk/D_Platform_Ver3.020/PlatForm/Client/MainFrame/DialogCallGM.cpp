// DialogCallGM.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "DialogCallGM.h"

// CDialogCallGM dialog

IMPLEMENT_DYNAMIC(CDialogCallGM, CDialog)

CDialogCallGM::CDialogCallGM(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CALL_GM, pParent)
	, szCallGMmsg(_T(""))
	, game_error(false)
	, court(false)
	, publicPoison(false)
	, others(false)
{
	m_bkBrush = NULL;
}

CDialogCallGM::~CDialogCallGM()
{
	DeleteObject(m_bkBrush);
}

void CDialogCallGM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_btcancel);
	DDX_Control(pDX, IDOK, m_btok);
	DDX_Text(pDX, IDC_GM_MSG, szCallGMmsg);
	DDV_MaxChars(pDX, szCallGMmsg, 200);
}


BEGIN_MESSAGE_MAP(CDialogCallGM, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CDialogCallGM::OnBnClickedOk)
//	ON_BN_CLICKED(IDC_GM_GAME_ERROR, &CDialogCallGM::OnBnClickedGmGameError)
	ON_MESSAGE(WM_EXCHANGE_SKIN,&CDialogCallGM::OnExchangeSkin)
END_MESSAGE_MAP()


// CDialogCallGM message handlers

void CDialogCallGM::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (((CButton *)GetDlgItem(IDC_GM_GAME_ERROR))->GetCheck()==BST_CHECKED) game_error=true;
	if (((CButton *)GetDlgItem(IDC_GM_COURT))->GetCheck()==BST_CHECKED) court=true;
	if (((CButton *)GetDlgItem(IDC_PUBLICPOISON))->GetCheck()==BST_CHECKED) publicPoison=true;
	if (((CButton *)GetDlgItem(IDC_OTHERS))->GetCheck()==BST_CHECKED) others=true;
	//if (((CButton *)GetDlgItem(IDC_PPOISON))->GetCheck()==BST_CHECKED) publicPoison=true;

	OnOK();
}


BOOL CDialogCallGM::OnInitDialog()
{
	CDialog::OnInitDialog();

	Init();
	return TRUE; 
}

void CDialogCallGM::Init()
{
	CString s=CBcfFile::GetAppPath();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());
	CGameImage	m_bt;

	/*// 2012.12.17 yyf ����
	wsprintf(path,"%sdialog\\callgm_bk.bmp",skinfolder);
	m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper ImageHandle(&m_bkimage);
	int cx,cy;
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_bkimage.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		///{{ Added by zxd 20100709 �ͷ��������
		DeleteObject(hRgn);
		///Added by zxd 20100709 �ͷ��������}}
	}
	cx=ImageHandle.GetWidth();
	cy=ImageHandle.GetHeight();*/
	
	//*-------��ʼ�� m_configDlg--------- 2012.10.17 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CDialogCallGMConfig\\configDlg.bcf",s,skinfolder);// 
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	//-----------------------------------------------------*/
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);

	wsprintf(path,"%sdialog\\cancel_bt.bmp",skinfolder);
	m_bt.SetLoadInfo(path,true);
	m_btcancel.LoadButtonBitmap(path,false);
	m_btcancel.SetWindowPos(NULL,0,0,m_bt.GetWidth()/4,m_bt.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	wsprintf(path,"%sdialog\\ok_bt.bmp",skinfolder);
	m_bt.SetLoadInfo(path,true);
	m_btok.LoadButtonBitmap(path,false);
	m_btok.SetWindowPos(NULL,0,0,m_bt.GetWidth()/4,m_bt.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	SetDlgItemText(IDC_STATIC_CALL,fMsg.GetKeyVal("CallGMDlg","ReasonCall","����ԭ��"));
	SetDlgItemText(IDC_GM_GAME_ERROR,fMsg.GetKeyVal("CallGMDlg","GameFunError","��Ϸ���ܹ���"));
	SetDlgItemText(IDC_GM_COURT,fMsg.GetKeyVal("CallGMDlg","ReportCheater","�ٱ�������Ϊ"));
	SetDlgItemText(IDC_PUBLICPOISON,fMsg.GetKeyVal("CallGMDlg","IllegalWords",
		"����������Ⱦ���������໰����/����ˢ��/ɢ������˼��/���棩"));
	SetDlgItemText(IDC_OTHERS,fMsg.GetKeyVal("CallGMDlg","Other","������"));
	SetDlgItemText(IDC_STATIC_MSG,fMsg.GetKeyVal("CallGMDlg","LeaveMsg","�û����ԣ�"));
	SetDlgItemText(IDC_STATIC_GMCALL,fMsg.GetKeyVal("CallGMDlg","GMWillCallU",
		"��ȴ������ǵ����߹���Ա��������Ӧ���ĺ��У�"));

	m_bkBrush = CreateSolidBrush(m_configDlg.GetBKImageEX().GetPixel(32,55));//m_bkimage
	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
}

void CDialogCallGM::LoadSkin()
{
	CString s=CBcfFile::GetAppPath();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());

	CGameImage	m_bt;
	/*// 2012.12.17 yyf ����
	wsprintf(path,"%sdialog\\callgm_bk.bmp",skinfolder);
	m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);*/

	//CGameImageHelper ImageHandle(&m_bkimage);
	//int cx,cy;
	//HRGN hRgn=AFCBmpToRgn(ImageHandle,m_bkimage.GetPixel(0,0),RGB(1,1,1));
	//if (hRgn!=NULL)
	//{
	//	SetWindowRgn(hRgn,TRUE);
	//	///{{ Added by zxd 20100709 �ͷ��������
	//	DeleteObject(hRgn);
	//	///Added by zxd 20100709 �ͷ��������}}
	//}
	//cx=ImageHandle.GetWidth();
	//cy=ImageHandle.GetHeight();
	//SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);
	
	//*-------��ʼ�� m_configDlg--------- 2012.10.17 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CDialogCallGMConfig\\configDlg.bcf",s,skinfolder);// 
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	
	//-----------------------------------------------------*/

	wsprintf(path,"%sdialog\\cancel_bt.bmp",skinfolder);
	m_bt.SetLoadInfo(path,true);
	m_btcancel.LoadButtonBitmap(path,false);
	m_btcancel.SetWindowPos(NULL,0,0,m_bt.GetWidth()/4,m_bt.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	wsprintf(path,"%sdialog\\ok_bt.bmp",skinfolder);
	m_bt.SetLoadInfo(path,true);
	m_btok.LoadButtonBitmap(path,false);
	m_btok.SetWindowPos(NULL,0,0,m_bt.GetWidth()/4,m_bt.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	//CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	//SetDlgItemText(IDC_STATIC_CALL,fMsg.GetKeyVal("CallGMDlg","ReasonCall","����ԭ��"));
	//SetDlgItemText(IDC_GM_GAME_ERROR,fMsg.GetKeyVal("CallGMDlg","GameFunError","��Ϸ���ܹ���"));
	//SetDlgItemText(IDC_GM_COURT,fMsg.GetKeyVal("CallGMDlg","ReportCheater","�ٱ�������Ϊ"));
	//SetDlgItemText(IDC_PUBLICPOISON,fMsg.GetKeyVal("CallGMDlg","IllegalWords",
	//	"����������Ⱦ���������໰����/����ˢ��/ɢ������˼��/���棩"));
	//SetDlgItemText(IDC_OTHERS,fMsg.GetKeyVal("CallGMDlg","Other","������"));
	//SetDlgItemText(IDC_STATIC_MSG,fMsg.GetKeyVal("CallGMDlg","LeaveMsg","�û����ԣ�"));
	//SetDlgItemText(IDC_STATIC_GMCALL,fMsg.GetKeyVal("CallGMDlg","GMWillCallU",
	//	"��ȴ������ǵ����߹���Ա��������Ӧ���ĺ��У�"));

	//m_bkBrush = CreateSolidBrush(m_bkimage.GetPixel(32,55));
	
	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
}
//����
LRESULT CDialogCallGM::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	Init();
	Invalidate();
	return LRESULT(0);
}
LRESULT CDialogCallGM::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
		SetBkColor((HDC)wParam, m_configDlg.GetBKImageEX().GetPixel(32,55));//m_bkimage
		return (LRESULT)m_bkBrush;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}


void CDialogCallGM::OnPaint()
{
	CPaintDC dc(this);

	/*// 2012.12.17 yyf ����
	CGameImageHelper	handle(&m_bkimage);
	handle.BitBlt(dc.GetSafeHdc(),0,0);*/
	
	//�滭��ܵ�ͼ 2012.10.17 yyf
	m_configDlg.DrawViewFrame(&dc);
}
// ���ô���Բ�� 2012.10.17 yyf
void CDialogCallGM::SetWndRgn(void)
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