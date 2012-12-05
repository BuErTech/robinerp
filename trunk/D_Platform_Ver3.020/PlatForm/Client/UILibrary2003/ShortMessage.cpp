#include "Stdafx.h"
#include "ShortMessage.h"
#include "..\MainFrame\GameRoomEx.h"//���ִ����ǲ��ص�??
#include "..\MainFrame\IMMain.h"//���ִ����ǲ��ص�??
#include ".\shortmessage.h"
#include ".\gameframedlg.h"
//#include "userpower.h"

#include "detours.h"
#include "skinsb.h"


#define CONTROL_SPACE						2							//�ؼ����
//#define PIC_BT_WIDE							20							//��ť���
#define PIC_BT_WIDE							20							//��ť���
#define PIC_BT_HIGHT						20							//��ť�߶�
#define BT_BUTTON_WIDE						65							//��ť���
#define BT_BUTTON_HIGHT						21							//��ť�߶�

BEGIN_MESSAGE_MAP(CShortMessage, CGameFaceGo)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_GETMINMAXINFO()
	ON_MESSAGE(WM_HIT_LOGO,OnHitLogo)
	ON_MESSAGE(WM_HIT_EXPMESSTION,OnExpression)
	ON_BN_CLICKED(IDC_SEND_SHORT, OnBnClickedSendShort)
	ON_BN_CLICKED(IDC_CLOSESM, OnBnClickedClose)
	ON_BN_CLICKED(IDC_MINSESMT, OnBnClickedMin)
	ON_BN_CLICKED(IDC_CLOSESMT, OnBnClickedClose)
///	ON_BN_CLICKED(IDC_HISTORY, OnBnClickedHistory)
	ON_BN_CLICKED(IDC_EXPRESSION, OnBnClickedExpression)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//���캯��
CShortMessage::CShortMessage(CWnd * pParentWnd) : CGameFaceGo(IDD_SHORT_MESSAGE), m_MessageHandle(&m_RecvMessage)
{
	m_pParentWnd=NULL;
	m_dwTargerAccID=0;
	m_dwTargetUserID=0L;
	m_dwLastTalkTime=0L;
	m_pParentWnd=pParentWnd;
	m_bIsInRoom = true;
	//m_bIsInRoom = false;
	m_bkBrush=NULL;
	m_dwGamePower = 0;
}

//��������
CShortMessage::~CShortMessage()
{
	DeleteObject(m_bkBrush);
}

//�ؼ��󶨺���
void CShortMessage::DoDataExchange(CDataExchange* pDX)
{
	CGameFaceGo::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEND_SHORT, m_BtSend);
	DDX_Control(pDX, IDC_EXPRESSION, m_BtExpression);
///	DDX_Control(pDX, IDC_HISTORY, m_BtHistory);
	DDX_Control(pDX, IDC_CLOSESM, m_BtClose2);
	DDX_Control(pDX, IDC_CLOSESMT, m_BtClose);
	DDX_Control(pDX, IDC_MINSESMT, m_BtMin);
	DDX_Control(pDX, IDC_SEND_MESSAGE, m_SendMessage);
	DDX_Control(pDX, IDC_RECV_MESSAGE, m_RecvMessage);
}

//��ʼ������
BOOL CShortMessage::OnInitDialog()
{
	__super::OnInitDialog();

	//���ÿؼ�
	m_RecvMessage.Init();
	m_SendMessage.SetFocus();

// 	TCHAR szCaption[100];
// 	if (m_iGmID == 0)
// 		wsprintf(szCaption,"�� %s ������",m_strTargerName);
// 	else 
// 		wsprintf(szCaption,"ϵͳ��Ϣ������Ա%s",m_strTargerName);
// 	

	//������ʾ�ؼ�
	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool(GetDlgItem(IDC_SEND_SHORT),TEXT("������Ϣ"));
	m_ToolTip.AddTool(GetDlgItem(IDC_EXPRESSION),TEXT("ѡ�����ͷ��"));
	m_ToolTip.AddTool(GetDlgItem(IDC_CLOSESM),TEXT("�رն���Ϣ����"));
//	m_ToolTip.AddTool(GetDlgItem(IDC_HISTORY),TEXT("�鿴������ʷ��¼"));

	//����λͼ
	ASSERT(m_pTargetUserItem!=NULL);
	int r,g,b;
	CString s=CBcfFile::GetAppPath();//����·��
	CBcfFile f( s + "skin0.bcf");
	CString key=TEXT("ShortMessage");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	CGameImage bt;
	skinfolder=f.GetKeyVal(Glb().m_skin,"skinfolder","image\\client\\skin0\\");

	wsprintf(path,"%sdialog\\sendmessage_bk.bmp",skinfolder);
	m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper ImageHandle(&m_bk);
	HRGN hRgn=AFCBmpToRgn(ImageHandle,(m_bk.GetPixel(0,0)),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		DeleteObject(hRgn);
	}

	CRect btnRc;
	wsprintf(path,"%sdialog\\log_exit_bt.bmp",skinfolder);//win_close.bmp
	m_BtClose.LoadButtonBitmap(path,false);
	m_BtClose.GetClientRect(&btnRc);
	int iCloseWidth = btnRc.Width();
	m_BtClose.MoveWindow(m_bk.GetWidth()-iCloseWidth,0,btnRc.Width(),btnRc.Height());

	wsprintf(path,"%sdialog\\top_min_bt.bmp",skinfolder);
	m_BtMin.LoadButtonBitmap(path,false);
	m_BtMin.GetClientRect(&btnRc);
	bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_BtMin.MoveWindow(m_bk.GetWidth()-btnRc.Width()-iCloseWidth,0,bt.GetWidth()/4,bt.GetHeight());
	m_BtMin.ShowWindow(SW_HIDE);

	wsprintf(path,"%sdialog\\face03.bmp",skinfolder);
	m_BtExpression.LoadButtonBitmap(path,false);//����ѡ���
	bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_BtExpression.MoveWindow(9, 259, bt.GetWidth()/4,bt.GetHeight());

	wsprintf(path,"%sdialog\\close_bt.bmp",skinfolder);
	m_BtClose2.LoadButtonBitmap(path,false);
	bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_BtClose2.MoveWindow(260, 400, bt.GetWidth()/4,bt.GetHeight());

	wsprintf(path,"%sdialog\\send_bt.bmp",skinfolder);
	m_BtSend.LoadButtonBitmap(path,false);
	bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_BtSend.MoveWindow(330, 400, bt.GetWidth()/4,bt.GetHeight());

	m_RecvMessage.MoveWindow(3, 30, 394, 229);
	m_SendMessage.MoveWindow(3, 283, 394, 107);

	r=f.GetKeyVal(key,"talkbkr",0);
	g=f.GetKeyVal(key,"talkbkg",0);
	b=f.GetKeyVal(key,"talkbkb",0);
	m_colorresource.m_crTalkBkColor=RGB(r,g,b);
	m_RecvMessage.m_bkColor=RGB(r,g,b);
	m_RecvMessage.SetBackgroundColor(false,RGB(r,g,b));
	r=f.GetKeyVal(key,"talknamer",255);
	g=f.GetKeyVal(key,"talknameg",255);
	b=f.GetKeyVal(key,"talknameb",255);
	m_colorresource.m_crTalkNameColor=RGB(r,g,b);
	r=f.GetKeyVal(key,"systemr",255);
	g=f.GetKeyVal(key,"systemg",255);
	b=f.GetKeyVal(key,"systemb",255);
	m_colorresource.m_crSystemTxColor=RGB(r,g,b);
	r=f.GetKeyVal(key,"systembdr",255);
	g=f.GetKeyVal(key,"systembdg",0);
	b=f.GetKeyVal(key,"systembdb",0);
	m_colorresource.m_crSystemBdColor=RGB(r,g,b);
	r=f.GetKeyVal(key,"talknormalr",255);
	g=f.GetKeyVal(key,"talknormalg",255);
	b=f.GetKeyVal(key,"talknormalb",255);
	m_colorresource.m_crNormalTxColor=RGB(r,g,b);	
		////////////////////////////////////
	////Kylin 20090117 ������Ϣ��ɫ����
	r=f.GetKeyVal(key,"talkr",255);
	g=f.GetKeyVal(key,"talkg",255);
	b=f.GetKeyVal(key,"talkb",255);
	m_colorresource.m_crTalkTxColor=RGB(r,g,b);	
	////////////////////////////////////
	r=f.GetKeyVal(key,"senttextr",0);
	g=f.GetKeyVal(key,"senttextg",0);
	b=f.GetKeyVal(key,"senttextb",255);
	m_colorresource.m_crSentTxColor=RGB(r,g,b);	
	r=f.GetKeyVal(key,"receivedtextr",255);
	g=f.GetKeyVal(key,"receivedtextg",0);
	b=f.GetKeyVal(key,"receivedtextb",0);
	m_colorresource.m_crReceivedTxColor=RGB(r,g,b);	

	m_MessageHandle.SetColor(m_colorresource);

	//�滭����
	CRect ClientRect;
	GetClientRect(&ClientRect);
	this->MapWindowPoints(this->GetParent(),&ClientRect);
//	DrawBackFace(&dc,0,0,ClientRect.Width(),ClientRect.Height());
	CGameImageHelper	help(&m_bk);
	this->MoveWindow(ClientRect.left,ClientRect.top,help.GetWidth(),help.GetHeight());

	m_bkBrush=CreateSolidBrush(m_bk.GetPixel(267,289));

	CString szMsg = f.GetKeyVal(key,"xtgg","��ӭ�������������Ϸ���ģ��벻Ҫ���������͸¶�ʺź����룬�мǲ�Ҫ����������е��κλ���н���Ϣ���ܾ�������ҽ��ף���ǿ���ҷ�����ʶ�������ϵ���ƭ������Ը���һ�л�Թٷ���վ��ҳ����Ϊ׼��");
	m_MessageHandle.InsertSystemBoard(szMsg.GetBuffer(szMsg.GetLength()));

	{
		CString s=CBcfFile::GetAppPath ();/////����·��
		CBcfFile f( s + "skin0.bcf");
		TCHAR path[MAX_PATH];
		CString skinfolder;
		skinfolder = f.GetKeyVal(Glb().m_skin,"skinfolder","image\\client\\skin0\\");
		wsprintf(path,"%sDialog\\scroll.bmp",skinfolder);
		HBITMAP hbmScorll = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
		SkinSB_Uninit(m_SendMessage.GetSafeHwnd());
		SkinSB_Uninit(m_RecvMessage.GetSafeHwnd());
		SkinSB_Init(m_SendMessage.GetSafeHwnd(), hbmScorll);
		SkinSB_Init(m_RecvMessage.GetSafeHwnd(), hbmScorll);
	}

	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE); 

	return false;
}

//�滭����
void CShortMessage::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	CRect ClientRect;
	GetClientRect(&ClientRect);
//	this->MapWindowPoints(this->GetParent(),&ClientRect);
//	DrawBackFace(&dc,0,0,ClientRect.Width(),ClientRect.Height());
	CGameImageHelper	help(&m_bk);
//	this->MoveWindow(ClientRect.left,ClientRect.top,help.GetWidth(),help.GetHeight());
	help.BitBlt(dc.GetSafeHdc(),0,0);
	//�滭�û�
	TCHAR szBuffer[100];
	memset(szBuffer,0,sizeof(szBuffer));
	dc.SetTextColor(RGB(255,255,255));
	dc.SetBkMode(TRANSPARENT);
	CFont font;
	font.CreateFont(-12,0,0,0,700,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
	dc.SelectObject(font);
	if (m_iGmID == 0)
		wsprintf(szBuffer,"�� %s ������",m_strTargerName);
	else 
		wsprintf(szBuffer,"ϵͳ��Ϣ��%s",m_strTargerName);
	CRect rect(0,10,help.GetWidth(),40);
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_CENTER);
	this->SetWindowText(szBuffer);
	return;
}

//����λ��
void CShortMessage::FixControlSize(int iWidth, int iHegiht)
{
	return;
}

//������Ϣ
void CShortMessage::SetTalkInfo(SendMsgUserStruct * pTargerUserItem, long int dwGamePower, TCHAR * szRoomName)
{
	m_dwGamePower = dwGamePower;
	bool bSendMunh=CUserPower::CanSendMuch(m_dwGamePower);
	m_SendMessage.LimitText(bSendMunh?MAX_TALK_LEN:NORMAL_TALK_LEN);

	//���ñ���
	bool bCleanMessage=((m_dwTargetUserID!=0L)&&(m_dwTargetUserID!=pTargerUserItem->dwUserID));
	m_strRoomName=szRoomName;

	m_TargetUserItem.bDeskNO = pTargerUserItem->bDeskNO;
	m_TargetUserItem.bGameMaster = pTargerUserItem->bGameMaster;
	m_TargetUserItem.dwUserID = pTargerUserItem->dwUserID;
	m_TargetUserItem.dwAccID = pTargerUserItem->dwAccID;
	strcpy(m_TargetUserItem.nickName , pTargerUserItem->nickName);
	m_strTargerName=m_TargetUserItem.nickName;
	m_dwTargerAccID=m_TargetUserItem.dwAccID;
	m_dwTargetUserID=m_TargetUserItem.dwUserID;

	//���ÿؼ�
	if (GetSafeHwnd()!=NULL)
	{
		if (bCleanMessage==true)
		{
			m_RecvMessage.OnClearAll();
			m_SendMessage.SetWindowText(TEXT(""));
		}
		m_SendMessage.SetFocus();
		Invalidate(FALSE);
	}
	m_iGmID = 0;
	return;
}

void CShortMessage::SetTalkInfo(int iGmID)
{
	m_iGmID = iGmID;
	m_strTargerName.Format("����Ա%d", m_iGmID);
}

//����Ҽ�������Ϣ
void CShortMessage::OnLButtonDown(UINT nFlags, CPoint point)
{
	//ģ�ⰴ����
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	return;
}

//ȷ����ť
void CShortMessage::OnOK()
{
	OnBnClickedSendShort();
}

//�˳���ť
void CShortMessage::OnBnClickedClose()
{
	if (m_iGmID != 0) m_iGmID = 0;
	DestroyWindow();
	return;
}

//�˳���ť
void CShortMessage::OnBnClickedMin()
{
	OnSize(SIZE_MINIMIZED, 0, 0);
	return;
}


//ȡ����ť
void CShortMessage::OnCancel()
{
	ShowWindow(SW_HIDE);
	return;
}

//��ʷ��¼
void CShortMessage::OnBnClickedHistory()
{
	CString strBuffer;
	strBuffer.Format(TEXT("%s\\%s.TXT"),"Talk",m_strTargerName);
	ShellExecute(NULL,TEXT("open"),TEXT("Notepad.exe"),strBuffer,NULL,SW_SHOW);
	return;
}

//���Ͱ�ť
void CShortMessage::OnBnClickedSendShort()
{
	if (m_iGmID != 0)
	{
		//������Ϣ
		CString strBuffer;
		m_SendMessage.GetWindowText(strBuffer);
		if (strBuffer.Find(TEXT("{\\rtf"))!=-1) throw TEXT("����ʧ�ܣ�");
		for (int i=strBuffer.GetLength();i>0;i--)
			if ((strBuffer[i-1]!=TEXT('\n'))&&(strBuffer[i-1]!=TEXT('\r'))&&(strBuffer[i-1]>=-1)&&(strBuffer[i-1]<30)) throw TEXT("���ܷ��ͷǷ��ַ�������ʧ�ܣ�");

		m_strLastTalk = strBuffer;
		m_pParentWnd->PostMessage(WM_GM_TALK, WPARAM(&m_strLastTalk), LPARAM(&m_iGmID));
		m_SendMessage.SetWindowText(TEXT(""));
	}
	else
	{
		
		//�ж���Ϣ
		m_SendMessage.SetFocus();
		if (m_SendMessage.GetWindowTextLength()==0)	return;
		try
		{
			//��ȡʱ��
			long int dwNowTime=(long int)time(NULL);
			//�ж�Ŀ�����
			ASSERT(m_dwTargetUserID!=0L);
			if (m_dwTargetUserID!=m_TargetUserItem.dwUserID)
			{
				//Modify By Fred Huang 2008-06-17
				bool ret = false;
				if(m_bIsInRoom)
					ret = ((CGameRoomEx *)m_pParentWnd)->GetSendMsgUserStruct(m_dwTargetUserID, &m_TargetUserItem);
				else
					ret = ((CIMMain *)m_pParentWnd)->GetSendMsgUserStruct(m_dwTargetUserID, &m_TargetUserItem);

				if (!ret) 
				{
					m_MessageHandle.InsertSystemMessage(TEXT("��Ϣ����ʧ�ܣ��Է��������뿪��"));
					return;
				}
			}

			//��ȡ��Ϣ
			CString strBuffer;
			m_SendMessage.GetWindowText(strBuffer);

			//�Ƿ���Է�����
			if (CUserPower::CanSendMessage(m_dwGamePower)==false)
			{
				m_MessageHandle.InsertSystemMessage(TEXT("��Ϣ����ʧ�ܣ�"));
				return;
			}

			//�ж��Ƿ���ͬһ��Ϸ��,
			if(m_bIsInRoom)
			{
				if ((m_TargetUserItem.bDeskNO!=255)&&(m_TargetUserItem.bDeskNO==
					((CGameRoomEx *)m_pParentWnd)->m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)&&
					(((CGameRoomEx *)m_pParentWnd)->m_GameInfo.pMeUserInfo->GameUserInfo.bGameMaster!=6&&m_TargetUserItem.bGameMaster!=6))//�ϵۿ���
				{
					m_MessageHandle.InsertSystemMessage(TEXT("Ϊ��ֹ���ף����ܺ�ͬ����ҷ�����Ϣ��"));
					return;
				}
			}

			//�ж���Ϣ����
			int iMaxSendLen=CUserPower::CanSendMuch(m_dwGamePower)?MAX_TALK_LEN:NORMAL_TALK_LEN;
			if (strBuffer.GetLength()>iMaxSendLen) throw TEXT("���͵���Ϣ̫������������Ϣ��ֶ�η��ͣ�");

			//������Ϣ
			if (strBuffer.Find(TEXT("{\\rtf"))!=-1) throw TEXT("����ʧ�ܣ�");
			for (int i=strBuffer.GetLength();i>0;i--)
				if ((strBuffer[i-1]!=TEXT('\n'))&&(strBuffer[i-1]!=TEXT('\r'))&&(strBuffer[i-1]>=-1)&&(strBuffer[i-1]<30)) throw TEXT("���ܷ��ͷǷ��ַ�������ʧ�ܣ�");

			//�ж��ٶ�
			if (CUserPower::CanTalkFast(m_dwGamePower)==false)
			{
				long int dwNowTime=(long int)time(NULL);
				if ((m_dwLastTalkTime+1L)>dwNowTime) throw TEXT("�벻ҪƵ�����Ͷ���Ϣ��");
				if (((m_dwLastTalkTime+3L)>dwNowTime)&&(m_strLastTalk.Compare(strBuffer)==0))
					throw TEXT("�벻Ҫ�ظ����ԣ�");
			}


			//////////////////////////////////////////////////////////////////////////
			//��չ�����˷���Ϣ
			//20081211 , Fred Huang
			if(m_dwTargetUserID<0)
			{
				m_MessageHandle.InsertUserTalk(((CGameRoomEx *)m_pParentWnd)->m_pLogonInfo->nickName,NULL,
					strBuffer.GetBuffer(strBuffer.GetLength()),
					m_colorresource.m_crNormalTxColor,true);
				m_strLastTalk=strBuffer;
				m_dwLastTalkTime=(long int)time(NULL);
				m_SendMessage.SetWindowText(TEXT(""));
				::SendMessage(m_RecvMessage.m_hWnd,WM_VSCROLL,SB_PAGEDOWN,NULL);
				return;
			}
			//////////////////////////////////////////////////////////////////////////


			//����������Ϣ
			if(m_bIsInRoom)
			{
				MSG_GR_RS_NormalTalk Talk;
				ZeroMemory(&Talk,sizeof(MSG_GR_RS_NormalTalk));
				Talk.iLength=strBuffer.GetLength();
				Talk.dwSendID=((CGameRoomEx *)m_pParentWnd)->m_pLogonInfo->dwUserID;
				Talk.dwTargetID=m_dwTargetUserID;
				lstrcpy(Talk.szMessage,strBuffer);
				Talk.crColor=m_colorresource.m_crTalkTxColor;
				((CGameRoomEx *)m_pParentWnd)->SendData(&Talk,sizeof(Talk)-sizeof(Talk.szMessage)+Talk.iLength+1,MDM_GR_ROOM,ASS_GR_SHORT_MSG,0);

				m_strLastTalk=Talk.szMessage;
			}
			else
			{
				MSG_IM_C_MSG msg;
				ZeroMemory(&msg,sizeof(msg));
				msg.dwUserID=((CIMMain*)m_pParentWnd)->GetMeUserInfo()->dwUserID;
				memcpy(&msg.szUserName,((CIMMain*)m_pParentWnd)->GetMeUserInfo()->nickName ,sizeof(((CIMMain*)m_pParentWnd)->GetMeUserInfo()->nickName));
				msg.dwRemoteUserID=m_dwTargetUserID;
// 				memcpy(&msg.cf,&charFormat,sizeof(CHARFORMAT));
// 				memcpy(&msg.szFontname,m_charFontName,m_charFontName.GetLength());
				msg.szMsgLength=strBuffer.GetLength();
				memcpy(&msg.szMsg,strBuffer,strBuffer.GetLength());
				msg.cbSize=sizeof(MSG_IM_C_MSG)-sizeof(msg.szMsg)+msg.szMsgLength+1;
				((CIMMain *)m_pParentWnd)->SendData(&msg,msg.cbSize,MDM_GP_IM,ASS_IMC_MSG,0);

				m_MessageHandle.InsertUserTalk(((CIMMain*)m_pParentWnd)->GetMeUserInfo()->nickName, NULL, msg.szMsg,m_colorresource.m_crSentTxColor,true);

				m_strLastTalk=msg.szMsg;
			}

			//���ò���
			m_dwLastTalkTime=(long int)time(NULL);
			m_SendMessage.SetWindowText(TEXT(""));
		}
		catch (TCHAR * szErrorMessage)
		{ 
			m_MessageHandle.InsertSystemMessage(szErrorMessage); 
		}
		
		catch (...) 
		{
			m_MessageHandle.InsertSystemMessage(TEXT("��Ϣ����ʧ�ܣ�")); 
		}
	}
	return;
}

//��Ϣ����
BOOL CShortMessage::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN))
	{
		if ((::GetAsyncKeyState(VK_LCONTROL)&0xFFF0)||(::GetAsyncKeyState(VK_RCONTROL)&0xFFF0))
		{
			OnBnClickedSendShort();
			return TRUE;
		}
	}
	m_ToolTip.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//�յ���Ϣ
void CShortMessage::OnRecvMessage(TCHAR * szSendName, TCHAR * szMessage, COLORREF crColor)
{
	BZSoundPlay(this, "music/������Ϣ��ʾ��.mp3", 0);
	m_MessageHandle.InsertUserTalk(szSendName,NULL,szMessage,crColor,true);
	::SendMessage(m_RecvMessage.m_hWnd,WM_VSCROLL,SB_PAGEDOWN,NULL);
	this->SetForegroundWindow();
	return;
}

//λ����Ϣ
void CShortMessage::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�ƶ��ؼ�
	FixControlSize(cx,cy);

	return;
}

//��ȡλ��
void CShortMessage::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
	lpMMI->ptMinTrackSize.x=300;
	lpMMI->ptMinTrackSize.y=250;
	return;
}

//ͷ����Ϣ
LRESULT CShortMessage::OnHitLogo(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

//������Ϣ
LRESULT CShortMessage::OnExpression(WPARAM wparam, LPARAM lparam)
{
	CTrueItem * pExpItem=m_ExpressWnd.GetExpressItem((UINT)wparam);
	if (pExpItem!=NULL)
	{
		CString strBuffer;
		m_SendMessage.GetWindowText(strBuffer);
		strBuffer+=pExpItem->m_szTrancelate;
		m_SendMessage.SetWindowText(strBuffer);
		m_SendMessage.SetFocus();
		m_SendMessage.SetSel(strBuffer.GetLength(),strBuffer.GetLength());	
	}
	return 0;
}

//���鰴ť
void CShortMessage::OnBnClickedExpression()
{
	//�������鴰��
	if (m_ExpressWnd.GetSafeHwnd()==NULL) m_ExpressWnd.CreateExpressionWnd(this);

	//�ƶ�����
	CRect ButtonRect;
	CSize ExpressSize;
	m_ExpressWnd.GetFixSize(ExpressSize);
	m_BtExpression.GetWindowRect(&ButtonRect);
	SafeMoveWindow(&m_ExpressWnd,ButtonRect.left,ButtonRect.top-ExpressSize.cy,ExpressSize.cx,ExpressSize.cy);
	m_ExpressWnd.ShowWindow(SW_SHOW);
	m_ExpressWnd.SetFocus();

	return;
}

void CShortMessage::OnDestroy()
{
	CGameFaceGo::OnDestroy();
	//���������¼
	CString strBuffer;

	CString str;
	char Temp[MAX_PATH];
	str.Format("talk\\%s.txt",m_strTargerName);

	bool b=false;
	CFile file;
	if(file.Open(str,CFile::modeWrite))
	{
		b=true;
	}
	else
	{
		if(file.Open(str,CFile::modeCreate|CFile::modeWrite))
		{
			b=true;
		}
	}

	if(b)
	{
		file.SeekToEnd();
		for(int i=0;i<m_RecvMessage.GetLineCount();i++)
		{
			memset(Temp,0,sizeof(Temp));
			m_RecvMessage.GetLine(i,Temp,MAX_PATH);
			if(strlen(Temp)>0)
				file.Write(Temp,strlen(Temp)-1);
			file.Write("\r\n",strlen("\r\n"));
		}

		file.Close();
	}


}

LRESULT CShortMessage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
		SetBkColor((HDC)wParam, m_bk.GetPixel(267,289));
		return (LRESULT)m_bkBrush;
	}
	return CGameFaceGo::WindowProc(message, wParam, lParam);
}

void CShortMessage::TransUserInfo(UserItemStruct *pIn, SendMsgUserStruct *pOut)
{
	pOut->bDeskNO = pIn->GameUserInfo.bDeskNO;
	pOut->bGameMaster = pIn->GameUserInfo.bGameMaster;
	pOut->dwUserID = pIn->GameUserInfo.dwUserID;
	pOut->dwAccID = pIn->GameUserInfo.dwAccID;
	strcpy(pOut->nickName , pIn->GameUserInfo.nickName);
}