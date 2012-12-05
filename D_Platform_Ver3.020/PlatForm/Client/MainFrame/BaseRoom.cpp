///��������
#include "StdAfx.h"
#include "BaseRoom.h"

BEGIN_MESSAGE_MAP(CBaseRoom, CGameFaceGo)
	ON_WM_SIZE()
	ON_MESSAGE(WM_SPLIT_WND,OnSplitMessage)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CConnectDlg, CGameFaceGo)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_MESSAGE(WM_CTLCOLORSTATIC,OnControlColor)
	ON_MESSAGE(WM_EXCHANGE_SKIN,OnExchangeSkin)
END_MESSAGE_MAP()

//��������ʱ�� ID
#define ID_PRO_TIME					100			
/*******************************************************************************************************/

BOOL CConnectDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return true;
	return CGameFaceGo::OnEraseBkgnd(pDC);
}

//���캯����CConnectDlg��
CConnectDlg::CConnectDlg() : CGameFaceGo(IDD_CONNECT)
{
}

//����������CConnectDlg��
CConnectDlg::~CConnectDlg()
{
}
//DDX/DDV ֧��  ���ݶԻ��������
void CConnectDlg::DoDataExchange(CDataExchange* pDX)//�Ի�������ݽ���
{
	CGameFaceGo::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_QUIT, m_Quit);
	DDX_Control(pDX, IDC_TEXT, m_Static_1);
}
//����������
bool CConnectDlg::SetMessageText(const TCHAR *szMessage)
{
	return true;	///< ���ֱ�����ף����Բ�������
	if ((szMessage==NULL)||(GetSafeHwnd()==NULL)) return false;
	GetDlgItem(IDC_TEXT)->SetWindowText(szMessage);
	return true;
}

//���д���
void CConnectDlg::CenterWindow(CWnd* pAlternateOwner/* = NULL*/)
{
	RECT rc;
	::GetWindowRect(m_hWnd,&rc);
	int iSrcW = GetSystemMetrics(SM_CXSCREEN);
	int iSrcH = GetSystemMetrics(SM_CYSCREEN);
	int iWndW = rc.right - rc.left;
	int iWndH = rc.bottom - rc.top;
	::MoveWindow(m_hWnd,(iSrcW - iWndW)/2,(iSrcH - iWndH)/2,iWndW,iWndH,true);
}

//�˳���ť
void CConnectDlg::OnQuit()
{
	CBaseRoom * pBaseRoom=CONTAINING_RECORD(this,CBaseRoom,m_Connect);
	if (pBaseRoom->OnCancelConnectEvent()) DestroyWindow();
}
//��ʼ������
BOOL CConnectDlg::OnInitDialog()
{
	CGameFaceGo::OnInitDialog();

	Init();
	return TRUE;
}
//��ʼ��
void CConnectDlg::Init()
{
	ShowWindow(SW_HIDE);    // PengJiLin, 2010-5-14

	int r,g,b;
	CString s=CBcfFile::GetAppPath ();//����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("ConnectDlg");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());

	wsprintf(path,"%sdialog\\cancel_bt.bmp",skinfolder);
	m_Quit.LoadButtonBitmap(path,false);
	r=f.GetKeyVal(key,"cntbkr",0);
	g=f.GetKeyVal(key,"cntbkg",0);
	b=f.GetKeyVal(key,"cntbkb",0);
	m_bkcolor=RGB(r,g,b);
	SetTimer(ID_PRO_TIME,100,NULL);//��������ʱ�� ID

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString strMessage;
	strMessage = fMsg.GetKeyVal("MainRoom","Connecting","�����������ķ����������Ժ�...");
	//SetDlgItemText(IDC_TEXT,strMessage);

	// ����ͼ PengJiLin, 2010-5-14
	CRect rect, rectQ;
	wsprintf(path,"%sdialog\\connect.bmp",skinfolder);
	m_CbackPic.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	GetWindowRect(&rect);
	MoveWindow(rect.left,rect.top,m_CbackPic.GetWidth(),m_CbackPic.GetHeight());
	CenterWindow();

	GetWindowRect(&rect);
	m_Quit.GetWindowRect(&rectQ);   
	m_Quit.MoveWindow(rect.Width()/2-rectQ.Width()/2, rect.Height()-rectQ.Height()-6, rectQ.Width(), rectQ.Height());

	ShowWindow(SW_SHOW);
	OnPaint();
}

void CConnectDlg::LoadSkin()
{
	int r,g,b;
	CString s=CBcfFile::GetAppPath ();//����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("ConnectDlg");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());

	wsprintf(path,"%sdialog\\cancel_bt.bmp",skinfolder);
	m_Quit.LoadButtonBitmap(path,false);
	r=f.GetKeyVal(key,"cntbkr",0);
	g=f.GetKeyVal(key,"cntbkg",0);
	b=f.GetKeyVal(key,"cntbkb",0);
	m_bkcolor=RGB(r,g,b);

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString strMessage;
	strMessage = fMsg.GetKeyVal("MainRoom","Connecting","�����������ķ����������Ժ�...");

	// ����ͼ PengJiLin, 2010-5-14
	CRect rect, rectQ;
	wsprintf(path,"%sdialog\\connect.bmp",skinfolder);
	m_CbackPic.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	GetWindowRect(&rect);
	//MoveWindow(rect.left,rect.top,m_CbackPic.GetWidth(),m_CbackPic.GetHeight());
	CenterWindow();

	GetWindowRect(&rect);
	m_Quit.GetWindowRect(&rectQ);   
	//m_Quit.MoveWindow(rect.Width()/2-rectQ.Width()/2, rect.Height()-rectQ.Height()-6, rectQ.Width(), rectQ.Height());

	ShowWindow(SW_SHOW);
	Invalidate();
}
//��ʱ����Ϣ
void CConnectDlg::OnTimer(UINT nIDEvent)
{
	CGameFaceGo::OnTimer(nIDEvent);
}
//�ؼ���ɫ��Ϣ
LRESULT CConnectDlg::OnControlColor(WPARAM wparam, LPARAM lparam)
{
	::SetBkMode((HDC)wparam,TRANSPARENT);///����ģʽ  ͸����, ��Ȼ��, ������
	::SetTextColor((HDC)wparam,CGameImageLink::m_DialogResource.m_crTextColor);//������ɫ
	::SetBkColor((HDC)wparam,CGameImageLink::m_DialogResource.m_crBackColor);//������ɫ
	return (LRESULT)CGameImageLink::m_DialogResource.m_BackBrush.GetSafeHandle();
}
//����
LRESULT CConnectDlg::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	LoadSkin();
	return LRESULT(0);
}
void CConnectDlg::OnPaint()
{
	CPaintDC dc(this);
    //CRect ClientRect;             // PengJiLin, 2010-5-14
    //GetClientRect(&ClientRect);
    //dc.FillSolidRect(&ClientRect,m_bkcolor);

    // PengJiLin, 2010-5-14
    CGameImageHelper helprb(&m_CbackPic);
    helprb.BitBlt(dc.GetSafeHdc(),0,0);
}

/*******************************************************************************************************/

//���캯��
CBaseRoom::CBaseRoom(UINT uDialogTemplata) : CGameFaceGo(uDialogTemplata)
{
	m_TCPSocket = NULL;
	m_TCPSocket = new CTCPClientSocket(this);
}

//��������
CBaseRoom::~CBaseRoom()
{
	if (m_TCPSocket != NULL)
		delete m_TCPSocket;
}

//��ʼ������
BOOL CBaseRoom::OnInitDialog()
{
	CGameFaceGo::OnInitDialog();

	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return TRUE;
}

//λ�ñ仯
void CBaseRoom::OnSize(UINT nType, int cx, int cy)
{
	CGameFaceGo::OnSize(nType, cx, cy);
	if ((cx!=0)&&(cy!=0)) OnSizeEvent(nType,cx,cy);
	return;
}
//�������Ϣ
LRESULT CBaseRoom::OnSplitMessage(WPARAM wparam, LPARAM lparam)
{
	OnSplitEvent((UINT)wparam,LOWORD(lparam),HIWORD(lparam));
	return 0;
}
//����������Ϣ����
int CBaseRoom::SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	if (m_TCPSocket != NULL)
		return m_TCPSocket->SendData(bMainID,bAssistantID,bHandleCode);//����������
	return 0;
}

//����������Ϣ����
int CBaseRoom::SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	if (m_TCPSocket != NULL)
		return m_TCPSocket->SendData(pData,uBufLen,bMainID,bAssistantID,bHandleCode);
	return 0;
}


int CBaseRoom::CloseSocket(bool bNotify)
{
	if (m_TCPSocket != NULL)
		return m_TCPSocket->CloseSocket(bNotify);
}

//���ͺ���
int CBaseRoom::SendData(void * pData, UINT uSize, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	if (m_TCPSocket != NULL)
		return m_TCPSocket->SendData(pData, uSize, bMainID, bAssistantID, bHandleCode);
	return 0;
}
//������ͺ���
int CBaseRoom::SendData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	if (m_TCPSocket != NULL)
		return m_TCPSocket->SendData(bMainID, bAssistantID, bHandleCode);
	return 0;
}

/*******************************************************************************************************/
