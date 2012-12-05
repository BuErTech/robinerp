se//�������
#include "StdAfx.h"
#include "Resource.h"
#include "ChatControl.h"
#include "GameRoomEx.h"
//#include ".\chatcontrol.h"

//�궨��
//#define PIC_BT_WIDE						20							//λͼ��ť���
#define PIC_BT_WIDE							20							//λͼ��ť���
#define PIC_BT_HIGHT						20							//λͼ��ť�߶�
#define CONTROL_SPACE						2							//�ؼ����

//�ؼ� ID
#define IDC_INPUT							10							//������Ϣ	
#define IDC_EXPRES							11							//���鰴ť
#define IDC_ACTION							12							//������ť
#define IDC_ATTRIB							13							//���԰�ť
#define IDC_SEND							14							//���Ͱ�ť

#define IDC_BOARDCAST						15							//���Ȱ���

BEGIN_MESSAGE_MAP(CChatControl, CWnd)/////////��Ϣ����ӳ�� 
	ON_WM_SIZE()////////��Ϣ��Ӧ����ϵͳ ����,�����Լ���д
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_SEND, OnBnClickedSend)
	ON_BN_CLICKED(IDC_EXPRES, OnBnClickedExpression)
	ON_BN_CLICKED(IDC_BOARDCAST, OnBnClickedBoardCast)
	ON_MESSAGE(WM_EXCHANGE_SKIN,OnExchangeSkin)
END_MESSAGE_MAP()
//���캯��
CChatControl::CChatControl()
{
	m_BoardCastWnd = NULL;
}
//��������
CChatControl::~CChatControl()
{
	m_exp.Detach();
	m_send.Detach();
	m_boardcast.Detach();
}
//������Ϣ
int CChatControl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct) == -1) return -1;
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	//�����ؼ�
	m_InputMessage.Create(CBS_DROPDOWN|CBS_AUTOHSCROLL|WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_INPUT);//��������� �����б�
	m_BtSend.Create(TEXT(""),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,CRect(0,0,0,0),this,IDC_SEND);//���͵İ�ť
	m_BtExpression.Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,CRect(0,0,0,0),this,IDC_EXPRES);//����ѡ���ͼ�ΰ�ť

	m_BtBoardCast.Create(NULL,WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,CRect(0,0,0,0),this, IDC_BOARDCAST);//����ѡ���ͼ�ΰ�ť
	

	//������ʾ�ؼ�
	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool(GetDlgItem(IDC_SEND),fMsg.GetKeyVal("MainRoom","SendChatMsg","����������Ϣ"));
	m_ToolTip.AddTool(GetDlgItem(IDC_EXPRES),fMsg.GetKeyVal("MainRoom","SelectEmotionHead","ѡ�����ͷ��"));
	//���ñ���ͼ
	m_InputMessage.SetFont(&CGameImageLink::m_HFont);//���������
	LoadSkin();
	return 0;
}
//
void CChatControl::LoadSkin()
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key= TEXT("ChatControl");
	CString skinfolder =f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());;

	//���鴰��
	m_iExpX = f.GetKeyVal(key,"ExpX",0);
	m_iExpY = f.GetKeyVal(key,"ExpY",0);
	//���ʹ���
	m_iSendX = f.GetKeyVal(key,"sendX",0);
	m_iSendY = f.GetKeyVal(key,"sendY",0);
}
//����
LRESULT CChatControl::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	LoadSkin();
	Invalidate();
	return LRESULT(0);
}

//λ�ñ仯
void CChatControl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//����
	if (m_BtBoardCast.GetSafeHwnd() != NULL)
	{
		m_BtBoardCast.MoveWindow(0, (cy - m_boardcast.GetHeight()) / 2, m_boardcast.GetWidth() / 4, m_boardcast.GetHeight());
	}
	//�ƶ��ؼ�
	if (m_BtExpression.GetSafeHwnd()!=NULL)
	{
		m_BtExpression.MoveWindow(m_boardcast.GetWidth() / 4,(cy-m_exp.GetHeight())/2,m_exp.GetWidth()/4,m_exp.GetHeight());
	}
	if (m_InputMessage.GetSafeHwnd()!=NULL) 
	{
		CRect rect;
		m_InputMessage.GetClientRect(&rect);
		m_InputMessage.MoveWindow((m_boardcast.GetWidth() + m_exp.GetWidth())/4, (cy-20)/2, cx-(m_boardcast.GetWidth() + m_exp.GetWidth())/4-m_send.GetWidth()/4,200);
	}
	if (m_BtSend.GetSafeHwnd()!=NULL)
	{
		m_BtSend.MoveWindow(cx-m_send.GetWidth()/4,(cy-m_send.GetHeight())/2,m_send.GetWidth()/4,m_send.GetHeight());
	}

	return;
}
//�ػ�����
void CChatControl::OnPaint()
{
	CPaintDC dc(this);
	//�滭����
	CRect ClientRect;
	GetClientRect(&ClientRect);

	//dc.FillSolidRect(&ClientRect,m_bkColor);
	//CGameImage						temp;
	//temp.SetLoadInfo("image\\client\\skin0\\room_right\\room_talkbc.bmp",CGameImageLink::m_bAutoLock);
	//CGameImageHelper	help(&temp);
	//help.BitBlt(dc.GetSafeHdc(),0,0);
	//CRect ClientRect;
	//GetClientRect(&ClientRect);
	//CGameImageHelper handle(&m_bkImg);
	//for(int i=0;i<ClientRect.Width();i+=handle.GetWidth())
	//	handle.BitBlt(dc.GetSafeHdc(),i,0);
	
	return;
}
//���Ͱ�ť
void CChatControl::OnBnClickedSend()
{
	GetParent()->SendMessage(IDM_SEND_ROOMMSG,1,1);
//	CGameRoomEx * pGameRoom=(CGameRoomEx *)GetParent();
//	pGameRoom->OnSendRoomTalk();
	return;
}
//���鰴ť
void CChatControl::OnBnClickedExpression()
{
	GetParent()->SendMessage(IDM_SEND_ROOMMSG,2,2);

//	CGameRoomEx * pGameRoom=(CGameRoomEx *)GetParent();
//	pGameRoom->OnBnClickedExpression();
	return;
}
//��Ϣ����
BOOL CChatControl::PreTranslateMessage(MSG * pMsg)
{
	m_ToolTip.RelayEvent(pMsg);
	return CWnd::PreTranslateMessage(pMsg);
}
void CChatControl::SetBkColor(COLORREF color)
{
	m_bkColor=color;
}

//���Ȱ�ť
void CChatControl::OnBnClickedBoardCast()
{
	m_BoardCastWnd = new CUseBoardCastWnd();
	m_BoardCastWnd->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
	m_BoardCastWnd->SetParentWnd(m_pGameRoom);
	m_BoardCastWnd->CenterWindow();
	m_BoardCastWnd->ShowModal();
	if (m_BoardCastWnd != NULL)
		delete m_BoardCastWnd;
}

void CChatControl::UseProp_Buy(int nPropIndex)
{
	if (m_BoardCastWnd != NULL)
	{
		m_BoardCastWnd->UseProp_Buy(nPropIndex);
	}
}

void CChatControl::SetImage(TCHAR path1[],TCHAR path2[], TCHAR path3[])
{
	m_exp.SetLoadInfo(path1,CGameImageLink::m_bAutoLock);
	m_send.SetLoadInfo(path2,CGameImageLink::m_bAutoLock);

	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f(s + m_skinmgr.GetSkinBcfFileName());
	bool _use = f.GetKeyVal("BoardCast","TalkUseBoardCast", 0);
	if (_use)
		m_boardcast.SetLoadInfo(path3, CGameImageLink::m_bAutoLock);
	else
		m_boardcast.SetLoadInfo("", CGameImageLink::m_bAutoLock);

	m_BtExpression.LoadButtonBitmap(path1,false);
	m_BtSend.LoadButtonBitmap(path2,false);
	m_BtBoardCast.LoadButtonBitmap(path3, false);
}

void CChatControl::SetImage(TCHAR bkimg[],TCHAR path1[],TCHAR path2[], TCHAR path3[])
{
	m_bkImg.SetLoadInfo(bkimg,CGameImageLink::m_bAutoLock);
	m_exp.SetLoadInfo(path1,CGameImageLink::m_bAutoLock);
	m_send.SetLoadInfo(path2,CGameImageLink::m_bAutoLock);
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f(s + m_skinmgr.GetSkinBcfFileName());
	bool _use = f.GetKeyVal("BoardCast","TalkUseBoardCast", 0);
	if (_use)
		m_boardcast.SetLoadInfo(path3, CGameImageLink::m_bAutoLock);
	else
		m_boardcast.SetLoadInfo("", CGameImageLink::m_bAutoLock);

	m_BtExpression.LoadButtonBitmap(path1,false);
	m_BtSend.LoadButtonBitmap(path2,false);
	m_BtBoardCast.LoadButtonBitmap(path3, false);
}

//��������
void CChatControl::ReLoadTalkINI()
{
	m_InputMessage.ResetContent();
	//��talk.ini�ļ��ж�ȡ������
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "Talk.bcf");
	CString key="BZW_TALK";
//	TCHAR path[MAX_PATH];
	CString szTalk;
	CString szNumber;
	int i=1;
	do
	{
		szNumber.Format("%d",i++);
		szTalk=f.GetKeyVal(key,"T"+szNumber,"");
		if(!szTalk.IsEmpty())
			m_InputMessage.AddString(szTalk);
	}while(!szTalk.IsEmpty());
}


void CChatControl::SetGameRoomWnd(CWnd* pWnd)
{
	m_pGameRoom = pWnd;
}