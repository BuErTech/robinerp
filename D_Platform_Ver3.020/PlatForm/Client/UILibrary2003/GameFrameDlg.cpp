#include "StdAfx.h"
#include <SYS/Stat.h>
#include "Resource.h"
#include "UserPower.h"
//#include "GameFrameDlg.h"
#include "AFCFunction.h"
#include "GameRoomMessage.h"
#include "afcdevhead.h"
#include "GameFrameDlg.h"
//#include "BuyPropDialog.h"
#define WM_BRING_GAMEWND_TO_TOP				WM_USER + 438				//������Ϸ����

int							CLoveSendClass::m_iMaxShortWnd=10;					//��ര��
//bool						CLoveSendClass::m_bAutoSit=true;					//�Զ�����
//��̬����
CArray<LONG,LONG>				CLoveSendClass::m_ShieldArray;			//�����û�

//�ؼ� ID
#define ID_GAME_VIEW						10							//��Ϸ��ͼ ID

//�û��˵���Ϣ����
#define IDM_COPY_USER_NAME					WM_USER+101					//�����û���
#define IDM_SET_TALK_OBJECT					WM_USER+102					//��Ϊ̸������ 
#define IDM_CANCEL_TALK						WM_USER+103					//ȡ��������� 
#define IDM_EMABLE_WATCH					WM_USER+104					//�����Թ� 
#define IDM_UNEMABLE_WATCH					WM_USER+105					//��ֹ�Թ�
#define IDM_SHIELD_USER						WM_USER+106					//�����û�
#define IDM_UNSHIELD_USER					WM_USER+107					//�����û�
#define IDM_KICK_USER						WM_USER + 108				//�����뿪(����)
#define IDM_SET_FRIEND						WM_USER+227
#define IDM_GIVE_PROP						WM_USER+109
//�궨��
#define CONTROL_SPACE						2							//�ؼ����
#define SPLIT_WIDE							2//4							//��������
#define TITLE_HIGHT							66							//�������߶�
#define BUTTON_WIDE							8							//���ذ�ť���
#define BUTTON_HIGHT						80							//���ذ�ť�߶�
#define TALK_HIGHT							53							//��������߶�
#define PIC_BT_WIDE							20							//λͼ��ť���
#define PIC_BT_HIGHT						20							//λͼ��ť�߶�
#define BT_BUTTON_WIDE						55							//��ť���
#define BT_BUTTON_HIGHT						20							//��ť�߶�
#define BUTTOM_HEIGHT						130
#define VIEW_TOP							95
const int vf_iew=140;
const int vf_ieh=226;

#define SPLIT_POINT_X						737								//�ָ���X����
#define USER_LIST_POS_Y						250//167							//
#define USER_LIST_HEIGHT					120							//�б�߶�



#define WEB_HEGIHT							110							//���߶�

#define TALK_POS_Y							304//(USER_LIST_POS_Y + USER_LIST_HEIGHT)//304
#define INPUT_POS_Y							227

#define INFO_WIDTH							275							//�ұ���Ϣ�����

#define TOP_BUTTON_WIDE						113
#define TOP_BOTTON_HEIGHT					27

#define SET_BUTTON_WIDE						73
#define SET_BOTTON_HEIGHT					25
#define USER_FRAME_WIDTH					206				//��ʾ�û���Ϣ��
#define USER_FRAME_HEIGHT					32				//��ʾ�û���Ϣ���


//#define CHECK_TIME						3000						//��ʱ�����
//#define ID_CHECK_SIGNAL                 5


//��ʱ��
//#define IDT_SHOWWIN							500
__declspec( dllexport ) CGlobal& Glb(){return g_global;}

//wushuqun 2008.10.14
#define WM_COLSE_ROOM_WITH_MSG      WM_USER+219                      //�رշ�����ô�������һ����Ϣ��


BEGIN_MESSAGE_MAP(CLoveSendClass, CLoadFileBmp)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_MESSAGE(WM_SPLIT_WND,OnSplitMessage)
	ON_MESSAGE(WM_RICH_EDIT_HIT,OnRichEditHit)
	ON_MESSAGE(WM_RELOAD_TALKINI,OnReLoadTalkINI)
	ON_MESSAGE(WM_HIT_EXPMESSTION,OnExpression)
	ON_MESSAGE(WM_LOADSKIN,OnLoadSkin)
	ON_MESSAGE(WM_CLOSE_GAME,OnCloseGame)
	ON_MESSAGE(IDM_GETPOINT,OnGetPoint)
	
//	ON_MESSAGE(IDM_BUY_ITEM,/OnBuyItemData)
//	ON_MESSAGE(IDM_USE_ITEM,OnUserUseItemData)
//	ON_MESSAGE(IDM_BROADCAST_MSG,OnUseBroadcastMsg)

	ON_BN_CLICKED(IDC_SEND, OnBnClickedSendTalk)
	ON_BN_CLICKED(IDC_EXPRESSION, OnBnClickedExpression)
	ON_BN_CLICKED(IDC_SETCOLOR,OnBnClickedSetcolor)
	//ON_BN_CLICKED(IDC_GAME_BTN_08, OnBnClickedGameRoom)
	ON_BN_CLICKED(IDC_QUIT_GAME, OnBnClickedQuitGame)
	ON_BN_CLICKED(IDC_EXIT, OnBnClickedQuitGame)
	ON_BN_CLICKED(IDC_MIN, OnBnClickedMin)
	ON_BN_CLICKED(IDC_MAX,OnBnClickedMax)
	ON_BN_CLICKED(IDC_REST,OnBnClickedRest)
	//	ON_BN_CLICKED(IDC_MAX, OnBnClickedMax)
	//ON_BN_CLICKED(IDC_BTNUP, OnBnClickedUp)
	//ON_BN_CLICKED(IDC_BTNDOWN, OnBnClickedDown)
	//ON_BN_CLICKED(IDC_BTNEND, OnBnClickedEnd)
	//====�Ҽ���Ϣ����
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()

	ON_BN_CLICKED(IDC_HIDE, OnBnClickedHideList)
	//ON_BN_CLICKED(IDC_HOME, OnBnClickedHome)
	ON_BN_CLICKED(IDC_GAME_BTN_01, OnBnClkGameBt01)
	ON_BN_CLICKED(IDC_GAME_BTN_02, OnBnClkGameBt02)
	ON_BN_CLICKED(IDC_GAME_BTN_03, OnBnClkGameBt03)
	ON_BN_CLICKED(IDC_GAME_BTN_04, OnBnClkGameBt04)
	ON_BN_CLICKED(IDC_GAME_BTN_05, OnBnClkGameBt05)
	ON_BN_CLICKED(IDC_GAME_BTN_06, OnBnClkGameBt06)
	ON_BN_CLICKED(IDC_GAME_BTN_07, OnBnClkGameBt07)
	ON_BN_CLICKED(IDC_GAME_BTN_08, OnBnClkGameBt08)
	ON_BN_CLICKED(IDC_GAME_BTN_09, OnBnClkGameBt09)
	ON_BN_CLICKED(IDC_GAME_BTN_10, OnBnClkGameBt10)

	//{add by zxj 2008/04/07
	ON_BN_CLICKED(IDC_GO_AHEAD, OnBnClkPropGoAhead)
	ON_BN_CLICKED(IDC_GO_NEXT, OnBnClkPropGoNext)
	ON_BN_CLICKED(IDC_BUTTON_PROP1, OnBnClkProp1)
	ON_BN_CLICKED(IDC_BUTTON_PROP2, OnBnClkProp2)
	ON_BN_CLICKED(IDC_BUTTON_PROP3, OnBnClkProp3)
	ON_BN_CLICKED(IDC_BUTTON_PROP4, OnBnClkProp4)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify)

	ON_BN_CLICKED(IDC_SNDPLAY,OnSndPlay)
	ON_BN_CLICKED(IDC_SNDPAUSE,OnSndPause)
	ON_BN_CLICKED(IDC_SNDVOLDOWN,OnSndVolDown)
	ON_BN_CLICKED(IDC_SNDVOLUP,OnSndVolUp)

	ON_CBN_SELCHANGE(IDC_INPUT, OnCbnSelInput)
	////////////////////////////
	///Kylin 20090122 ʹ�õ���
	ON_MESSAGE(UM_USEITEM_USERSELECTED,OnUserItem)
	////////////////////////////
	//ON_WM_MOUSEMOVE()
//	ON_MESSAGE(PLAYVIEW_LBUTTON_DOWN, OnHandlePlayViewMsg)
	//end add}

//	ON_WM_NCPAINT()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//����Ҽ�������Ϣ
void CLoveSendClass::OnLButtonDown(UINT nFlags, CPoint point)
{
	//CRect rect;
	//::SystemParametersInfo(SPI_GETWORKAREA,NULL,&rect,NULL);
	//if(rect.Width()>1024 || rect.Height()>768)
	//if(!m_bMax)
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));	
	__super::OnLButtonDown(nFlags, point);
}



void CLoveSendClass::NavigateVirFace()
{
	if(!m_CurpUserInfoClicked)
		return;
	CRect rect;
	GetClientRect(&rect);
	rect.top+=24;
	rect.left=rect.right-3-207;
	rect.right=rect.left+207;
	rect.bottom=rect.top+193;
	if(m_pGameView->m_ShowType == 0)
		InvalidateRect(&rect,false);
	else if(m_pGameView->m_ShowType == 1)
		m_pGameView->InvalidateRect(&rect,false);
}

void CLoveSendClass::DrawCurUserInfo(CDC *pDC)
{
	if(m_CurpUserInfoClicked==NULL)
		return ;
	CRect ClientRect;
	if(m_pGameView->m_ShowType == 1)
		m_pGameView->GetClientRect(&ClientRect);
	else
		GetClientRect(&ClientRect);
	int x,y,h;
	TCHAR sz[100], szNum[128];
	y=0;
	//ͷ��
	m_pGameView->DrawLogo(pDC,m_CurpUserInfoClicked->bLogoID,ClientRect.right-m_logx,m_logy+8,false,m_CurpUserInfoClicked->dwUserID,false,true);

	//��Ϣ
	x=ClientRect.right-m_u1x;
	//	SetRect(&rect,x,
	y=m_u1y;
	h=m_u1h;
	pDC->SetTextColor(m_usercr);
	char ch[20];
	GetChar(m_CurpUserInfoClicked->nickName,ch,20);
	pDC->TextOut(x,y,sz,wsprintf(sz,"�ǳ�:%s",ch));
	pDC->TextOut(x,y+h*1,sz,wsprintf(sz,"�ȼ�:%s",m_pGameInfo->pOrderName(m_CurpUserInfoClicked->dwPoint,0)));
		/////////////////////////////////
	////Kylin 20090119 ���ҳ���ʾ����
	if(m_pGameInfo->uComType==TY_MATCH_GAME)
		pDC->TextOut(x,y+h*2,sz,wsprintf(sz,"����:%d",m_CurpUserInfoClicked->dwPoint-GetNormalOrderBasePoint(m_CurpUserInfoClicked->dwPoint)));
	else pDC->TextOut(x,y+h*2,sz,wsprintf(sz,"����:%d",m_CurpUserInfoClicked->dwPoint-GetNormalOrderBasePoint(m_CurpUserInfoClicked->dwPoint)));
	/////////////////////////////////
	GetNumString(szNum, m_CurpUserInfoClicked->i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
	pDC->TextOut(x,y+h*3,sz,wsprintf(sz,"���:%s",szNum));
	pDC->TextOut(x,y+h*4,sz,wsprintf(sz,"����:%d",m_CurpUserInfoClicked->dwFascination));
	/*pDC->TextOut(x,y+(2+k)*h,sz,wsprintf(sz,"Ӯ:%d",m_CurpUserInfoClicked->uWinCount));
	pDC->TextOut(x,y+(3+k)*h,sz,wsprintf(sz,"��:%d",m_CurpUserInfoClicked->uMidCount));
	pDC->TextOut(x,y+(4+k)*h,sz,wsprintf(sz,"��:%d",m_CurpUserInfoClicked->uLostCount));
	unsigned int count=m_CurpUserInfoClicked->uLostCount+m_CurpUserInfoClicked->uMidCount+m_CurpUserInfoClicked->uWinCount+m_CurpUserInfoClicked->uCutCount;
	pDC->TextOut(x,y+(5+k)*h,sz,wsprintf(sz,"�ܾ���:%d",count));
	UINT uCutPercent=(m_CurpUserInfoClicked->uCutCount>0)?(UINT)((double)(m_CurpUserInfoClicked->uCutCount*100)/(double)count):0;
	pDC->TextOut(x,y+(6+k)*h,sz,wsprintf(sz,"������:%d%%",uCutPercent));
	*/
	h = m_u2h;
	y = m_u2y;
	x = ClientRect.right - m_u2x;
	wsprintf(sz, "���ּ���:%s", GetMoneyOrderName(m_CurpUserInfoClicked->i64Money,0));
	pDC->TextOut(x, y + 1 * h, sz, strlen(sz));	
	x += m_u2w;	

	//if(m_iPaintCount<1)
	//	SetTimer(1,250,NULL);
}
//�ػ�����
void CLoveSendClass::OnPaint()
{
	if(!this->m_hWnd)return;
	PAINTSTRUCT lp;
	CDC *dc=BeginPaint(&lp);
	CFont font,*pOldFont;

	dc->SetBkMode(TRANSPARENT);	
	font.CreateFont(-12,0,0,0,400,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
	pOldFont=dc->SelectObject(&font);	

	CRect ClientRect;
	GetClientRect(&ClientRect);

	int iWidth = ClientRect.Width();
	int iHeight = ClientRect.Height();
	DrawViewFrame(dc,iWidth,iHeight);
	//����
	TCHAR title[100];
	GetWindowText(title,99);
	dc->SetTextColor(m_titlecr);
	dc->TextOut(m_titlex,m_titley,title,lstrlen(title));
	if(m_pGameView->m_ShowType == 0)
		DrawCurUserInfo(dc);
	dc->SetTextColor(RGB(0,0,0));
	
	dc->SelectObject(pOldFont);
	font.DeleteObject();
	EndPaint(&lp);
     
	//�����źŵ�
	drawSignal();

	if(m_ie.m_hWnd && szAdvUrl!="" && m_pGameView->m_bShowRight && !m_ie.get_Busy())
		m_ie.Refresh();
	if(m_UserListDlg)
	{
		if(m_UserListDlg.m_UserList.m_hWnd)
		{
			//m_UserListDlg.m_UserList.Invalidate();
			m_UserListDlg.m_UserList.ShowWindow(SW_HIDE);
			m_UserListDlg.m_UserList.ShowWindow(SW_SHOW);
		}
	}



	return ;
}
//���ƿ��
void CLoveSendClass::DrawViewFrame(CDC * pDC, int iWidth, int iHeight)
{

	//����
	CGameImageHelper helptc(&m_tc);
	CGameImageHelper helpbc(&m_bc);
	//����
	CGameImageHelper helplc(&m_lc);
	CGameImageHelper helprc(&m_rc);
	//����
	//////////////////////////////////////
	///Kylin 20090104 ��ֹ����ͼƬ������ȫ��ˢ�²���������
	pDC->FillSolidRect(iWidth - m_vlistx,0,m_vlistx -10 /*m_tc.GetWidth()*/-m_lc.GetWidth(),
		iHeight-m_tc.GetHeight()-m_bc.GetHeight(),m_FrameColor);//m_bc.GetPixel(1,1));
	//////////////////////////////////////

	if(helptc.GetWidth() > 0)
	{
		//////////////////////////////////////
		int i = 0;
		for(; i < iWidth - helptc.GetWidth(); i+=helptc.GetWidth())
			helptc.BitBlt(pDC->GetSafeHdc(),i,0);
		
		///Kylin 20090104 ��ֹ����ͼƬ������ȫ��ˢ�²���������
		helptc.BitBlt(pDC->GetSafeHdc(),i,0,iWidth-i,helptc.GetHeight(),0,0);
		//////////////////////////////////////
	}	
	if(helpbc.GetWidth() > 0)
	{
		for(int i = 0; i < iWidth -helpbc.GetWidth() ; i += helpbc.GetWidth())
			helpbc.BitBlt(pDC->GetSafeHdc(),i,iHeight - helpbc.GetHeight());
	}
	if(helplc.GetWidth() > 0)
	{
		for(int i = 0; i < iHeight; i+=helplc.GetHeight())
		{
			helplc.BitBlt(pDC->GetSafeHdc(),0,i);
			helprc.BitBlt(pDC->GetSafeHdc(),iWidth-helprc.GetWidth(),i);
		}
	}
	//�߽�
	//��
	CGameImageHelper helplt(&m_lt);
	CGameImageHelper helprt(&m_rt);
	helplt.BitBlt(pDC->GetSafeHdc(),0,0);	
	helprt.BitBlt(pDC->GetSafeHdc(),iWidth - helprt.GetWidth(),0);	
	//�߽�down
	CGameImageHelper helplb(&m_lb);
	CGameImageHelper helprb(&m_rb);
	helplb.BitBlt(pDC->GetSafeHdc(),0,iHeight - helplb.GetHeight());	
	helprb.BitBlt(pDC->GetSafeHdc(),iWidth - helprb.GetWidth(),iHeight - helprb.GetHeight());	

	//�߽�
	//��
	CGameImageHelper helprlt(&m_right_lt);
	CGameImageHelper helprrt(&m_right_rt);

	//�ұ�����	
	//�߽�down
	CGameImageHelper helprlb(&m_right_lb);
	CGameImageHelper helprrb(&m_right_rb);
	//����
	CGameImageHelper helprbc(&m_right_bc);
	CGameImageHelper helprtc(&m_right_tc);
	if(helprtc.GetWidth() > 0)
	{
		for(int i = iWidth - m_vlistx; i < iWidth - m_rc.GetWidth() - helprrb.GetWidth(); i += helprbc.GetWidth())
			helprtc.BitBlt(pDC->GetSafeHdc(),i,m_tc.GetHeight());
	}
	if(helprbc.GetWidth() > 0)
	{
		for(int i = iWidth - m_vlistx; i < iWidth - m_rc.GetWidth() - helprrb.GetWidth(); i += helprbc.GetWidth())
			helprbc.BitBlt(pDC->GetSafeHdc(),i,iHeight - helpbc.GetHeight()- helprbc.GetHeight());
	}

	//����
	CGameImageHelper helprlc(&m_right_lc);
	CGameImageHelper helprrc(&m_right_rc);
	if(helprlc.GetWidth() > 0)
	{
		for(int i = m_tc.GetHeight(); i < iHeight - m_bc.GetHeight()- helprlt.GetHeight(); i += helprlc.GetHeight())
		{
			helprlc.BitBlt(pDC->GetSafeHdc(),iWidth - m_vlistx,i);
			helprrc.BitBlt(pDC->GetSafeHdc(),iWidth - helprc.GetWidth() - helprrc.GetWidth(),i);
		}
	}
	helprlt.BitBlt(pDC->GetSafeHdc(),iWidth - m_vlistx,m_tc.GetHeight());	
	helprrt.BitBlt(pDC->GetSafeHdc(),iWidth - helprt.GetWidth() - helprrt.GetWidth(),m_tc.GetHeight());	
	helprlb.BitBlt(pDC->GetSafeHdc(),iWidth - m_vlistx ,iHeight - helplb.GetHeight() - helprlb.GetHeight() );	
	helprrb.BitBlt(pDC->GetSafeHdc(),iWidth - helprb.GetWidth()- helprrb.GetWidth()+2,iHeight - helprb.GetHeight()- helprrb.GetHeight());	

	//�����������ο��

	m_UserInfoRect.top = m_tc.GetHeight()+ m_right_tc.GetHeight();
	m_UserInfoRect.left = iWidth - m_vlistx + m_right_lc.GetWidth();
	m_UserInfoRect.right = iWidth - m_rc.GetWidth() - m_right_rc.GetWidth();
	m_UserInfoRect.bottom = m_UserInfoRect.top + 160;
	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	Rectangle(pDC->GetSafeHdc(),m_UserInfoRect.left,m_UserInfoRect.top,m_UserInfoRect.right,m_UserInfoRect.bottom);
	
	m_UserInfoRect.top +=1;
	m_UserInfoRect.left +=1;
	m_UserInfoRect.right -=1;
	m_UserInfoRect.bottom -=1;
	/////////////////////////////
	//Kylin 20090205 �����û���Ϣ����

	FillRect(pDC->GetSafeHdc(),&m_UserInfoRect,m_bkBrush);
	/////////////////////////////
	//
	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	m_UserListRect.top = m_UserInfoRect.bottom + 14 + 50;
	m_UserListRect.left = iWidth - m_vlistx + m_right_lc.GetWidth();
	m_UserListRect.right = iWidth - m_rc.GetWidth() - m_right_rc.GetWidth();
	m_UserListRect.bottom = m_UserListRect.top + m_userlistH + 2;
//	Rectangle(pDC->GetSafeHdc(),m_UserListRect.left,m_UserListRect.top,m_UserListRect.right,m_UserListRect.bottom);

	m_TalkRect.top = m_talkY -1;
	m_TalkRect.left = iWidth - m_vlistx + m_right_lc.GetWidth();
	m_TalkRect.right = iWidth - m_rc.GetWidth() - m_right_rc.GetWidth();
	m_TalkRect.bottom = m_TalkRect.top + m_talkH + 2;
	Rectangle(pDC->GetSafeHdc(),m_TalkRect.left,m_TalkRect.top,m_TalkRect.right,m_TalkRect.bottom);
	
}

void CLoveSendClass::OnLBClickUserList(UserItemStruct * pUserItem)
{
	if(m_CurpUserInfoClicked==&pUserItem->GameUserInfo)
		return ;
	m_CurpUserInfoClicked=&pUserItem->GameUserInfo;
	NavigateVirFace();
}

void CLoveSendClass::OnLBDBClickUserList(UserItemStruct * pUserItem)
{
	if(GetMeUserInfo()->dwUserID==pUserItem->GameUserInfo.dwUserID)
		return;
	m_CurpUserInfoClicked=&pUserItem->GameUserInfo;
	CString nick=pUserItem->GameUserInfo.nickName;
	m_InputMessage.SetWindowText(nick+":");
	m_InputMessage.SetFocus();
	int l=nick.GetLength()+1;
	m_InputMessage.SetEditSel(l,l);

}

//���Ҽ��˵�
void CLoveSendClass::OpenUserMenu(UserItemStruct * pUserItem)
{
	//Ч������
	if (pUserItem==NULL) 
	{
		m_dwCurrentUserID=0L;
		return;
	}
	//��������
	UserInfoStruct * pUserInfo=&pUserItem->GameUserInfo;
	m_dwCurrentUserID=pUserItem->GameUserInfo.dwUserID;

	//�����˵�
	CString strBuffer;
	CMenu UserInfoMenu;
	UserInfoMenu.CreatePopupMenu();

	if(!(m_pGameInfo->dwRoomRule & GRR_AUTO_SIT_DESK))//20081127
	{
		//���ܲ˵�
		UserInfoMenu.AppendMenu(0,IDM_COPY_USER_NAME,TEXT("�����û���"));
		if(m_dwCurrentUserID!=GetMeUserInfo()->dwUserID)
		{
			UserInfoMenu.AppendMenu(0,IDM_SET_FRIEND,TEXT("��Ӻ���"));
			UserInfoMenu.AppendMenu(0,IDM_GIVE_PROP,TEXT("���͵���..."));
		}
		//�������˹���
	}
	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
	int curtime=atoi(stime);                        //ת��Ϊint
	if(GetMeUserInfo()->iVipTime>curtime)//��Ա���д˹���
		UserInfoMenu.AppendMenu(0,IDM_KICK_USER,TEXT("�����뿪"));

	//if (m_dwCurrentUserID!=GetMeUserInfo()->dwUserID)
	//{
	//	strBuffer.Format(TEXT("�� %s ����"),pUserInfo->nickName);
	//	//UserInfoMenu.AppendMenu(0,IDM_SET_TALK_OBJECT,strBuffer);
	//}
	//int iStation=m_TargetPeople.GetCurSel();
	//if ((iStation!=CB_ERR)&&(iStation!=0))
	//{
	//	CString strName;
	//	m_TargetPeople.GetLBText(iStation,strName);
	//	strBuffer.Format(TEXT("ȡ���� %s ������"),strName);
	//	//UserInfoMenu.AppendMenu(0,IDM_CANCEL_TALK,strBuffer);
	//}
	//�������ι���
	INT_PTR i;
	for (i=0;i<m_ShieldArray.GetCount();i++)
	{
		if (m_ShieldArray.GetAt(i)==m_dwCurrentUserID) break;
	}
	if (i==m_ShieldArray.GetCount()) 
	{
		if (m_dwCurrentUserID!=GetMeUserInfo()->dwUserID)
		{
			strBuffer.Format(TEXT("���� %s ����Ϣ"),pUserInfo->nickName);
			UserInfoMenu.AppendMenu(0,IDM_SHIELD_USER,strBuffer);
		}
	}
	else
	{
		strBuffer.Format(TEXT("��� %s ����Ϣ����"),pUserInfo->nickName);
		UserInfoMenu.AppendMenu(0,IDM_UNSHIELD_USER,strBuffer);
	}

	//���������Թ�
	if ((m_bWatchMode==false)&&(pUserInfo->bDeskStation==GetMeUserInfo()->bDeskStation)
		&&(m_dwCurrentUserID!=GetMeUserInfo()->dwUserID))
	{
		UserInfoMenu.AppendMenu(0,MF_SEPARATOR);
		strBuffer.Format(TEXT("���� %s �Թ���Ϸ"),pUserInfo->nickName);
		UserInfoMenu.AppendMenu(0,IDM_EMABLE_WATCH,strBuffer);
		strBuffer.Format(TEXT("������ %s �Թ���Ϸ"),pUserInfo->nickName);
		UserInfoMenu.AppendMenu(0,IDM_UNEMABLE_WATCH,strBuffer);
	}


	//�û���Ϣ�˵�
	UINT uAllCount=pUserInfo->uWinCount+pUserInfo->uLostCount+pUserInfo->uMidCount;
	UINT uCutPercent=(pUserInfo->uCutCount>0)?(UINT)((double)(pUserInfo->uCutCount*100)/(double)uAllCount):0;
	UserInfoMenu.AppendMenu(0,MF_SEPARATOR);
	
	
	if(pUserInfo->iVipTime>curtime)
		strBuffer.Format("�û�����%s ( VIP ��Ա )",pUserInfo->nickName);
	else
		strBuffer.Format("�û�����%s",pUserInfo->nickName);
/*
	if (pUserInfo->bMember==0) strBuffer.Format(TEXT("�û�����%s"),pUserInfo->nickName);
	else 
		if (pUserInfo->bMember==1) strBuffer.Format(TEXT("�û�����%s ( ��ͨ��Ա )"),pUserInfo->nickName);
	else 
		if (pUserInfo->bMember==2) strBuffer.Format(TEXT("�û�����%s ( �߼���Ա )"),pUserInfo->nickName);
	else
		if (pUserInfo->bMember==3) strBuffer.Format(TEXT("�û�����%s ( �����α� )"),pUserInfo->nickName);
	else 
		if (pUserInfo->bMember==4) strBuffer.Format(TEXT("�û�����%s ( ���û�Ա )"),pUserInfo->nickName);
	else 
		strBuffer.Format(TEXT("�û�����%s ( Ԫ����� )"),pUserInfo->nickName);*/
	UserInfoMenu.AppendMenu(0,0,strBuffer);



	strBuffer.Format(TEXT("���֣�%d"),pUserInfo->dwPoint-GetNormalOrderBasePoint(pUserInfo->dwPoint));
	UserInfoMenu.AppendMenu(0,0,strBuffer);
	strBuffer.Format(TEXT("�ȼ���%s"),m_pGameInfo->pOrderName(pUserInfo->dwPoint,m_pGameInfo->uNameID));
	UserInfoMenu.AppendMenu(0,0,strBuffer);
	//if(m_pGameInfo->uComType==TY_MONEY_GAME)
	{
		TCHAR szNum[128];
        GetNumString(szNum, pUserInfo->i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
		strBuffer.Format(TEXT("��ң�%s"), szNum);
		UserInfoMenu.AppendMenu(0,0,strBuffer);
		strBuffer.Format(TEXT("�Ƹ�����%s"),GetMoneyOrderName(pUserInfo->i64Money,m_pGameInfo->uNameID));
		UserInfoMenu.AppendMenu(0,0,strBuffer);
	}
	strBuffer.Format(TEXT("�ܾ�����%d"),uAllCount);
	UserInfoMenu.AppendMenu(0,0,strBuffer);
	strBuffer.Format(TEXT("�����ʣ�%d"),uCutPercent);
	UserInfoMenu.AppendMenu(0,0,strBuffer);
	/*	if (pUserInfo->szClassName[0]!=0)
	{
	strBuffer.Format(TEXT("���ţ�%s"),pUserInfo->szClassName);
	UserInfoMenu.AppendMenu(0,0,strBuffer);
	}*/
	if (CUserPower::CanLookIP(m_pGameInfo->dwMasterPower)==true)
	{
		ULONG dwUserIP=pUserInfo->dwUserIP;
		strBuffer.Format(TEXT("�û� IP��%d.%d.%d.%d"),FOURTH_IPADDRESS(dwUserIP),THIRD_IPADDRESS(dwUserIP),
			SECOND_IPADDRESS(dwUserIP),FIRST_IPADDRESS(dwUserIP));
		UserInfoMenu.AppendMenu(0,0,strBuffer);
	}
	if (pUserInfo->bGameMaster!=0)
	{
		if (pUserInfo->bGameMaster==1) UserInfoMenu.AppendMenu(0,0,TEXT("����ְ�ƣ��ڲ�����Ա"));
		else if (pUserInfo->bGameMaster==2) UserInfoMenu.AppendMenu(0,0,TEXT("����ְ�ƣ��ⲿ����Ա"));
		else if (pUserInfo->bGameMaster==3) UserInfoMenu.AppendMenu(0,0,TEXT("����ְ�ƣ���������Ա"));
		else if (pUserInfo->bGameMaster==4) UserInfoMenu.AppendMenu(0,0,TEXT("����ְ�ƣ�����"));
		else if (pUserInfo->bGameMaster==5) UserInfoMenu.AppendMenu(0,0,TEXT("����ְ�ƣ���̫������"));
		else if (pUserInfo->bGameMaster==6) UserInfoMenu.AppendMenu(0,0,TEXT("����ְ�ƣ��ϵ�"));
	}

	//��ʾ�˵�
	POINT MousePoint;
	GetCursorPos(&MousePoint);
	UserInfoMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,MousePoint.x,MousePoint.y,this);
	return;
}
//��������Ҽ���Ϣ
void CLoveSendClass::OnRButtonDown(UINT   nFlags,   CPoint   point) 
{
	if(GetComType()==TY_MATCH_GAME)
		return ;
	/*	UserInfoStruct * pUserInfo;
	if(m_pUserInfo[GetMeUserInfo()->bDeskStation]==NULL)
	return ;
	for(int i=0;i<4;i++)
	if(m_UserInfoRect[i].PtInRect(point)&&1==m_bDeskStation[i])
	{
	pUserInfo=&m_pUserInfo[i]->GameUserInfo;		//��ʾ��Ϣλ��
	UserInfoMenu(pUserInfo);
	}*/
	return ;
}

void CLoveSendClass::UserInfoMenu(UserInfoStruct * pUserInfo)		//��ʾ�Ҽ�
{
	//�����˵�
	CString strBuffer;
	CMenu UserInfoMenu;
	UserInfoMenu.CreatePopupMenu();



	/*if (pUserInfo->bMember==0) 
		strBuffer.Format(TEXT("�û�����%s"),pUserInfo->nickName);
	else 
		if (pUserInfo->bMember==1) 
			strBuffer.Format(TEXT("�û�����%s ( ��ͨ��Ա )"),pUserInfo->nickName);
		else 
			if (pUserInfo->bMember==2) 
				strBuffer.Format(TEXT("�û�����%s ( �߼���Ա )"),pUserInfo->nickName);
			else 
				if (pUserInfo->bMember==3) 
					strBuffer.Format(TEXT("�û�����%s ( �����α� )"),pUserInfo->nickName);
				else 
					if (pUserInfo->bMember==4) 
						strBuffer.Format(TEXT("�û�����%s ( ���û�Ա )"),pUserInfo->nickName);
					else 
						strBuffer.Format(TEXT("�û�����%s ( Ԫ����� )"),pUserInfo->nickName);*/

	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());
	int curtime=atoi(stime);
	if(pUserInfo->iVipTime>curtime)
		strBuffer.Format("�û�����%s (VIP��Ա)", pUserInfo->nickName);
	else
		strBuffer.Format("�û�����%s", pUserInfo->nickName);

	UserInfoMenu.AppendMenu(0,0,strBuffer);
	UserInfoMenu.AppendMenu(0,MF_SEPARATOR);

	strBuffer.Format(TEXT("���֣�%d"),pUserInfo->dwPoint-GetNormalOrderBasePoint(pUserInfo->dwPoint));
	UserInfoMenu.AppendMenu(0,0,strBuffer);

	strBuffer.Format(TEXT("�ȼ���%s"),m_pGameInfo->pOrderName(pUserInfo->dwPoint,m_pGameInfo->uNameID));
	UserInfoMenu.AppendMenu(0,0,strBuffer);

	//if(m_pGameInfo->uComType==TY_MONEY_GAME)
	{
		TCHAR szNum[128];
        GetNumString(szNum, pUserInfo->i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
		strBuffer.Format(TEXT("��ң�%s"),szNum);
		UserInfoMenu.AppendMenu(0,0,strBuffer);
		strBuffer.Format(TEXT("�Ƹ�����%s"),GetMoneyOrderName(pUserInfo->i64Money,m_pGameInfo->uNameID));
		UserInfoMenu.AppendMenu(0,0,strBuffer);
	}

	UINT uAllCount=pUserInfo->uWinCount+pUserInfo->uLostCount+pUserInfo->uMidCount;
	UINT uCutPercent=(pUserInfo->uCutCount>0)?(UINT)((double)(pUserInfo->uCutCount*100)/(double)uAllCount):0;
	UserInfoMenu.AppendMenu(0,MF_SEPARATOR);

	strBuffer.Format(TEXT("�ܾ�����%d"),uAllCount);
	UserInfoMenu.AppendMenu(0,0,strBuffer);

	strBuffer.Format(TEXT("ʤ������%d"),pUserInfo->uWinCount);
	UserInfoMenu.AppendMenu(0,0,strBuffer);


	strBuffer.Format(TEXT("ƽ������%d"),pUserInfo->uMidCount);
	UserInfoMenu.AppendMenu(0,0,strBuffer);

	strBuffer.Format(TEXT("�������%d"),pUserInfo->uLostCount);
	UserInfoMenu.AppendMenu(0,0,strBuffer);

	strBuffer.Format(TEXT("�����ʣ�%d%%"),uCutPercent);
	UserInfoMenu.AppendMenu(0,0,strBuffer);
	//��ʾ�˵�
	POINT MousePoint;
	GetCursorPos(&MousePoint);
	UserInfoMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,MousePoint.x,MousePoint.y,this);
}
//009����λ��
void CLoveSendClass::FixControlStation(int iWidth, int iHeight)
{
	if(!m_bInit)
		return ;
	if(!m_bMax)
	{
		CRect rcRect;
		GetWindowRect(rcRect);
		if(rcRect.Width()>320 && rcRect.Height()>200)
		{
			m_SavedScreen.x=rcRect.Width();
			m_SavedScreen.y=rcRect.Height();
		}
	}
	CRect Rect,hsplitRect;
	GetClientRect(&Rect);
	//m_HorSplitBar.GetClientRect(&hsplitRect);
	//m_HorSplitBar.MapWindowPoints(this,&hsplitRect);

	CString s=CBcfFile::GetAppPath ();
	CBcfFile f( s + m_iniFile);
	CString key=TEXT("config");
	bool showright=m_pGameView->m_bShowRight;	//���λ
	int m_AdvX=-100;
	int	m_AdvY=-100;
	int m_AdvWidth=10;
	int m_AdvHeight=f.GetKeyVal(key,"advh",0);
	if(m_ie.m_hWnd && szAdvUrl!="" && showright)//ֻ����IE��Ч�͵�ַ��Ч������²���Ч
	{
		if(m_AdvHeight>0)
		{
			m_AdvX=f.GetKeyVal(key,"advx",-100);
			m_AdvY=f.GetKeyVal(key,"advy",-100);
			m_AdvWidth=f.GetKeyVal(key,"advw",10);
			m_ie.MoveWindow(iWidth-m_AdvX,m_AdvY,m_AdvWidth,m_AdvHeight);
		}
		else
		{
			m_ie.MoveWindow(-100,-100,10,10);
			//m_AdvHeight=0;
		}
	}
	else
	{
		m_ie.MoveWindow(-100,-100,10,10);
		//m_AdvHeight=0;
	}

	int x,y,w,h;
	x=f.GetKeyVal(key,"ulistx",0);
	y=f.GetKeyVal(key,"ulisty",0);
	w=f.GetKeyVal(key,"ulistw",0);
	h=f.GetKeyVal(key,"ulisth",0);


	//�û��б�
	SafeMoveWindow(	&m_UserListDlg, iWidth-x,y,w,h);
	m_userlistW = w;
	m_userlistH = h;
	x=f.GetKeyVal(key,"chatx",0);
	y=f.GetKeyVal(key,"chaty",0);
	w=f.GetKeyVal(key,"chatw",0);
	//h=f.GetKeyVal(key,"chath",0);
	//ע�⣺h�������߶�-�����ĸ߶ȣ�����Ҫͨ��iHeight-h��ȡ�������ʵ�ʵĸ߶�
	//���û��б��Ҳ���������ģ�����Ҫ��ȥ�û��б��ĸ߶�
	//Fred Huang 2008-05-29
	int chath=f.GetKeyVal(key,"chath",480);
	chath=iHeight-chath-h-m_AdvHeight;
	
	//�����
	SafeMoveWindow(	&m_TalkMessage, iWidth-x,y,w,chath);	
	m_talkY = y;
	m_talkW = w;
	m_talkH = chath;

	///////////////////////////////////////
	////Kylin 20090209 �Զ���ӦͼƬ��С
	CRect rect;
	//���ϰ�ť
	//��С��
	x=f.GetKeyVal(key,"minx",0);
	y=f.GetKeyVal(key,"miny",0);	
	//m_BtMin.SetWindowPos(NULL,iWidth-x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_BtMin.GetClientRect(&rect);
	m_BtMin.MoveWindow(iWidth-x,y,rect.Width(),rect.Height());
	//�˳�
	x=f.GetKeyVal(key,"quitx",0);
	y=f.GetKeyVal(key,"quity",0);
	//m_BtGameQuit.SetWindowPos(NULL,iWidth-x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_BtGameQuit.GetClientRect(&rect);
	m_BtGameQuit.MoveWindow(iWidth-x,y,rect.Width(),rect.Height());

	//��󻯺ͻ�ԭ
	x=f.GetKeyVal(key,"restx",0);
	y=f.GetKeyVal(key,"resty",0);
	m_btMax.GetClientRect(&rect);
	m_btMax.MoveWindow(iWidth-x,y,rect.Width(),rect.Height());
	m_btRest.GetClientRect(&rect);
	m_btRest.MoveWindow(iWidth-x,y,rect.Width(),rect.Height());
	///////////////////////////////////////
	////��ҳ��ť
	//x=f.GetKeyVal(key,"homex",0);
	//y=f.GetKeyVal(key,"homey",0);
	//m_Btbt3.SetWindowPos(NULL,iWidth-x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	
	//m_Btbt3.GetClientRect(&rect);


	//����
	//{modify by zxj 2008/04/08
	//x=f.GetKeyVal(key,"gamebt1x",188);
	x=f.GetKeyVal(key,"gamebt1x",190);
	//end modify}
	y=f.GetKeyVal(key,"gamebt1y",368);

	//{modify by zxj 2008/04/03
	//m_Btbt1.SetWindowPos(NULL,iWidth-x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_Btbt1.SetWindowPos(NULL,iWidth-x,y-10,0,0,SWP_NOSIZE|SWP_NOZORDER);
	//end modify}

	m_Btbt1.GetClientRect(&rect);
	m_iStatic1X = iWidth-x+10;
	m_iStatic1Y = y+rect.Height()+2;	

	//{add by zxj 2008/04/07
	x=f.GetKeyVal(key,"itemprevx",150);
	y=f.GetKeyVal(key,"itemprevy",368);
	m_BtPropGoAhead.SetWindowPos(NULL, iWidth -x, y , 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	x=f.GetKeyVal(key,"item1x",190);
	y=f.GetKeyVal(key,"item1y",368);
	m_BtProp1.SetWindowPos(NULL, iWidth-x, y , 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	x=f.GetKeyVal(key,"item2x",150);
	y=f.GetKeyVal(key,"item2y",368);
	m_BtProp2.SetWindowPos(NULL,iWidth-x, y , 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	x=f.GetKeyVal(key,"item3x",110);
	y=f.GetKeyVal(key,"item3y",368);
	m_BtProp3.SetWindowPos(NULL, iWidth-x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	x=f.GetKeyVal(key,"item4x",70);
	y=f.GetKeyVal(key,"item4y",368);
	m_BtProp4.SetWindowPos(NULL, iWidth-x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	x=f.GetKeyVal(key,"itemnextx",30);
	y=f.GetKeyVal(key,"itemnexty",368);
	m_BtPropGoNext.SetWindowPos(NULL, iWidth-x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	//end add}

	//{modify by zxj 2008/04/07
	//x=f.GetKeyVal(key,"gamebt2x",148);
	x=f.GetKeyVal(key,"gamebt2x",150);
	//end modify}
	y=f.GetKeyVal(key,"gamebt2y",368);

	//{modify by zxj 2008/04/03
	//m_Btbt2.SetWindowPos(NULL,iWidth-x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_Btbt2.SetWindowPos(NULL,iWidth-x,y-10,0,0,SWP_NOSIZE|SWP_NOZORDER);
	//end modify}

	//m_Btbt2.GetClientRect(&rect);

	//{modify by zxj 2008/04/07
	//x=f.GetKeyVal(key,"gamebt3x",108);
	x=f.GetKeyVal(key,"gamebt3x",110);
	//end modify}
	y=f.GetKeyVal(key,"gamebt3y",368);

	//{modify by zxj 2008/04/03
	//m_Btbt3.SetWindowPos(NULL,iWidth-x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_Btbt3.SetWindowPos(NULL,iWidth-x,y-10,0,0,SWP_NOSIZE|SWP_NOZORDER);
	//end modify}
	//m_Btbt3.GetClientRect(&rect);

	//{modify by zxj 2008/04/07 
	//x=f.GetKeyVal(key,"gamebt4x",68);
	x=f.GetKeyVal(key,"gamebt4x",70);
	//end modify}
	y=f.GetKeyVal(key,"gamebt4y",368);

	//{modify by zxj 2008/04/03
	//m_Btbt4.SetWindowPos(NULL,iWidth-x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_Btbt4.SetWindowPos(NULL,iWidth-x,y-10,0,0,SWP_NOSIZE|SWP_NOZORDER);
	//end modify}


	//����ؼ�
	x=f.GetKeyVal(key,"expx",0);
	y=f.GetKeyVal(key,"expy",0);
	//SafeMoveWindow(&m_TargetPeople,x,iHeight-BUTTOM_HEIGHT+5,iWidth-x-50,200);	//Ŀ��
	//m_BtExpression.SetWindowPos(NULL,iWidth-x,iHeight-y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_BtExpression.MoveWindow(iWidth-x,iHeight-y,18,18);

	//������ɫ��ť
	x=f.GetKeyVal(key,"setcolorx",0);
	y=f.GetKeyVal(key,"setcolory",0);
	m_BtSetColor.MoveWindow(iWidth-x,iHeight-y,18,18);

	x=f.GetKeyVal(key,"inputx",0);
	y=f.GetKeyVal(key,"inputy",0);
	w=f.GetKeyVal(key,"inputw",0);
	h=f.GetKeyVal(key,"inputh",0);
	SafeMoveWindow(&m_InputMessage,iWidth-x,iHeight-y,w,h);	//����
	x=f.GetKeyVal(key,"sendx",0);
	y=f.GetKeyVal(key,"sendy",0);
	//m_BtSend.SetWindowPos(NULL,iWidth-x,iHeight-y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_BtSend.GetClientRect(&rect);
	m_BtSend.MoveWindow(iWidth-x,iHeight-y,rect.Width(),rect.Height());


	if(showright)
	{//���ذ�ť
		m_BtHide.SetWindowPos(NULL,iWidth-m_vlistx,m_vlisty,0,0,SWP_NOSIZE|SWP_NOZORDER);
	}
	else
	{
		CRect rect;
		m_BtHide.GetClientRect(&rect);
		m_BtHide.SetWindowPos(NULL,iWidth-rect.Width(),m_vlisty,0,0,SWP_NOSIZE|SWP_NOZORDER);
	}
	m_UserListDlg.ShowWindow(showright);
	m_TalkMessage.ShowWindow(showright);
	//	m_BtGameSet.ShowWindow(showright);
	m_Btbt1.ShowWindow(showright);
	m_Btbt2.ShowWindow(showright);
	m_Btbt3.ShowWindow(showright);
	m_Btbt4.ShowWindow(showright);

	//{add by zxj 2008/04/07
	m_BtPropGoAhead.ShowWindow(showright);
	m_BtPropGoNext.ShowWindow(showright);
	m_BtProp1.ShowWindow(showright);
	m_BtProp2.ShowWindow(showright);
	m_BtProp3.ShowWindow(showright);
	m_BtProp4.ShowWindow(showright);
	//end add}

	//m_Btbt5.ShowWindow(showright);
	//m_Btbt6.ShowWindow(showright);
	//m_Btbt7.ShowWindow(showright);
	m_BtExpression.ShowWindow(showright);
	m_InputMessage.ShowWindow(showright);
	m_BtSend.ShowWindow(showright);
	//��Ϸ����
	x=f.GetKeyVal(key,"viewx",0);
	y=f.GetKeyVal(key,"viewy",0);
	w=f.GetKeyVal(key,"vieww",0);
	h=f.GetKeyVal(key,"viewh",0);

	if(m_pGameView->m_ShowType == 0)
	{
		if(showright)
			SafeMoveWindow(m_pGameView,x,y,iWidth-x-m_vlistx,iHeight-h-y);
		else
			SafeMoveWindow(m_pGameView,x,y,iWidth-w-x,iHeight-h-y);
	}
	else if(m_pGameView->m_ShowType == 1)
	{
		SafeMoveWindow(m_pGameView,x,y,iWidth-w-x,iHeight-h-y);
		CRect rect;
		m_pGameView->GetClientRect(&rect);
		m_pGameView->OnSize(SIZE_RESTORED,rect.Width(),rect.Height());
		m_pGameView->UpdateViewFace(NULL);
	}
	//m_Btbt5.ShowWindow(SW_HIDE);
	//m_Btbt6.ShowWindow(SW_HIDE);
	//m_Btbt7.ShowWindow(SW_HIDE);
	//m_Btbt8.ShowWindow(SW_HIDE);
	//m_Btbt9.ShowWindow(SW_HIDE);
	//m_Btbt10.ShowWindow(SW_HIDE);

	//{add by zxj 2008/04/09
	for( int i=0; i<MAX_PEOPLE; i++)
	{
		m_UserFrameRectArray[i].SetRect(m_pGameView->m_PtLogo[i].x, m_pGameView->m_PtLogo[i].y,\
			m_pGameView->m_PtLogo[i].x + m_pGameView->m_iUserFrame.GetWidth(), \
			m_pGameView->m_PtLogo[i].y + m_pGameView->m_iUserFrame.GetHeight());
	}
	//end add}

	SafeMoveWindow(&m_btSndPlay,sndPlayX>0?sndPlayX:iWidth+sndPlayX,sndPlayY>0?sndPlayY:iHeight+sndPlayY,m_sndBtnCx,m_sndBtnCy);
	SafeMoveWindow(&m_btSndPause,sndPauseX>0?sndPauseX:iWidth+sndPauseX,sndPauseY>0?sndPauseY:iHeight+sndPauseY,m_sndBtnCx,m_sndBtnCy);
	SafeMoveWindow(&m_btVolDown,sndVolDownX>0?sndVolDownX:iWidth+sndVolDownX,sndVolDownY>0?sndVolDownY:iHeight+sndVolDownY,m_sndBtnCx,m_sndBtnCy);
	SafeMoveWindow(&m_btVolUp,sndVolUpX>0?sndVolUpX:iWidth+sndVolUpX,sndVolUpY>0?sndVolUpY:iHeight+sndVolUpY,m_sndBtnCx,m_sndBtnCy);
	/////////////////////////////////////
	///////Kylin 20090122 ��ӻ��ְ�������ɫ����
	int r=f.GetKeyVal(key,"showscorer",0);
	int g=f.GetKeyVal(key,"showscoreg",0);
	int b=f.GetKeyVal(key,"showscoreb",0);

	m_pGameView->m_showcoredlg.SetTextColor(RGB(r,g,b));
	/////////////////////////////////////
}

//��������
bool CLoveSendClass::AFCCreateFrame()
{
	
	if (GetSafeHwnd()==NULL) 
	{
		/*if(m_DlgShowLeft.GetSafeHwnd()!=NULL)
		{
		AfxMessageBox("d");
		m_DlgShowLeft.ShowWindow(0);
		}*/
		/*CWnd * pWnd = GetDesktopWindow();*/
		Create(IDD_GAME_FRAME,GetDesktopWindow());
		//ShowWindow(SW_HIDE);
		//MoveWindow(0,0,900,650);
		//CenterWindow();	
		//OnBnClickedMax();
		//	ShowWindow(SW_SHOW);
	}	
	/*CRect r;
	GetWindowRect(&r);*/
	m_SavedScreen.x=1024;//r.Width();
	m_SavedScreen.y=736;//r.Height();
	m_bShowMax=true;
	m_CurpUserInfoClicked=GetMeUserInfo();
	ResetGameStation();
	// ��������
	//m_PlaySound.PlayMusic(NULL,m_BackMusicIni,m_BMusicKey,m_pGameInfo->bEnableBackSound);
	if(this->m_hWnd)
	{
		ShowWindow(SW_SHOW);
		SetForegroundWindow();
	}
	if(!m_bSndPause)
		BZSoundContinue(m_sndListID);
	AfxGetApp()->m_pMainWnd->PostMessage(WM_USER+100,2,0);

//OnBnClickedMin();
	//m_iPaintCount=0;
//	SetTimer(IDT_SHOWWIN,500,NULL);
	return true;
}
//013���캯��
CLoveSendClass::CLoveSendClass(CUsingSkinClass * pGameView) : CLoadFileBmp(IDD_GAME_FRAME), m_pGameView(pGameView), m_MessageHandle(&m_TalkMessage)
{
	m_bInit = false;
	m_CurpUserInfoClicked=NULL;
	m_pGameRoom=NULL;
	m_pGameInfo=NULL;
	m_uTimeID=0;
	m_uTimeCount=0;
	m_bTimeStation=255;
	m_bSaveView=false;
	m_bWatchMode=true;
	m_bWatchOther=false;
	m_dwLastTalkTime=0L;
	m_dwCurrentUserID=0L;
	memset(m_pUserInfo,0,sizeof(m_pUserInfo));	
	m_iniFile="image\\client\\"+Glb().m_skin+"\\game\\gameskini.bcf";// TEXT("image\\client\\skin0\\game\\skin.ini");

	//{add by zxj 2008/04/08
	//memset(&m_PropItems, 0, sizeof(m_PropItems));
	m_BtPropArray[0] = IDC_BUTTON_PROP1;
	m_BtPropArray[1] = IDC_BUTTON_PROP2;
	m_BtPropArray[2] = IDC_BUTTON_PROP3;
	m_BtPropArray[3] = IDC_BUTTON_PROP4;
	//end add}

	////Fred Huang,08-04-16
	//for(int i=0;i<PLATFORM_ITEM_KIND;i++)
	//{
	//	m_itemIndex[i]=-1;  //�Ե���������-1����ʾ������û�е���
	//	ZeroMemory(m_szPropToolTip[i],sizeof(m_szPropToolTip[i]));	//BY Fred huang,2008-04-30
	//}
	//m_useMaxItem=0;			//�����û���ʹ�õ�����Ϊ0
	m_curPropPage=0;		//��ǰ����ҳ=0��ÿҳ4��
	hPropSelUser=NULL;//2008-04-18
//	m_pBuyPropDlg=NULL;
	//end

	//SetFillColor(RGB(45, 80, 133));					//��ʼ�����ɫ
	//SetAllColor();									//������ɫ�ṹ
	m_MessageHandle.SetColor(colorresource);
	//m_pBuyItemdlg = NULL;								//��������б�
//	m_pHoldItemdlg = NULL;								//ʹ�õ����б�
//	m_pBroadcastDlg = NULL;								//�㲥����
	szAdvUrl="";


	m_sndListID=0;
	m_sndBtnCx=m_sndBtnCy=16;

	//�������źŵ�  yjj 090227
	pSingnalimg=NULL;
	m_NetSignalLevel=0;
	m_SavedSingalIndex=0;


	m_bkBrush = NULL;

	return;
}

//��������
CLoveSendClass::~CLoveSendClass()
{
	//if(m_pBuyItemdlg)
	//	delete m_pBuyItemdlg;

//	if(m_pHoldItemdlg)
///		delete m_pHoldItemdlg;

	//if(m_pBroadcastDlg)
	//	delete m_pBroadcastDlg;

	if(pSingnalimg && pSingnalimg->GetFlags()!=0)
	{
		delete pSingnalimg;
	}

	BZSoundDestoryList(m_sndListID);
	DeleteObject (m_bkBrush);

}

void CLoveSendClass::DoDataExchangeWebVirFace(CDataExchange * pDX)
{



}

//008�ż��󶨺���
void CLoveSendClass::DoDataExchange(CDataExchange * pDX)
{
	CLoadFileBmp::DoDataExchange(pDX);
//
	//DDX_Control(pDX, IDC_HOR_SPLIT, m_HorSplitBar);

	DDX_Control(pDX, IDC_INPUT, m_InputMessage);
	DDX_Control(pDX, IDC_SEND, m_BtSend);
	DDX_Control(pDX, IDC_EXPRESSION, m_BtExpression);
	DDX_Control(pDX, IDC_TALK_MESSAGE, m_TalkMessage);
	DDX_Control(pDX, IDC_QUIT_GAME, m_BtGameQuit);
	DDX_Control(pDX, IDC_SETCOLOR, m_BtSetColor);

	DDX_Control(pDX, IDC_HIDE, m_BtHide);
	DDX_Control(pDX, IDC_EXIT, m_BtExit);
	DDX_Control(pDX, IDC_MIN, m_BtMin);
	DDX_Control(pDX, IDC_MAX, m_btMax);
	DDX_Control(pDX, IDC_REST, m_btRest);

	DDX_Control(pDX, IDC_GAME_BTN_01, m_Btbt1);
	DDX_Control(pDX, IDC_GAME_BTN_02, m_Btbt2);
	DDX_Control(pDX, IDC_GAME_BTN_03, m_Btbt3);
	DDX_Control(pDX, IDC_GAME_BTN_04, m_Btbt4);
	DDX_Control(pDX, IDC_GAME_BTN_05, m_Btbt5);
	DDX_Control(pDX, IDC_GAME_BTN_06, m_Btbt6);
	DDX_Control(pDX, IDC_GAME_BTN_07, m_Btbt7);
	DDX_Control(pDX, IDC_GAME_BTN_08, m_Btbt8);
	DDX_Control(pDX, IDC_GAME_BTN_09, m_Btbt9);
	DDX_Control(pDX, IDC_GAME_BTN_10, m_Btbt10);
	//{add by zxj 2008/04/07
	DDX_Control(pDX, IDC_GO_AHEAD, m_BtPropGoAhead);
	DDX_Control(pDX, IDC_GO_NEXT, m_BtPropGoNext);
	DDX_Control(pDX, IDC_BUTTON_PROP1, m_BtProp1);
	DDX_Control(pDX, IDC_BUTTON_PROP2, m_BtProp2);
	DDX_Control(pDX, IDC_BUTTON_PROP3, m_BtProp3);
	DDX_Control(pDX, IDC_BUTTON_PROP4, m_BtProp4);
	//end add}
	DDX_Control(pDX, IDC_EXPLORER1, m_ie);
	//DDX_Control(pDX, IDC_GAME_BTN_08, m_Btbt8);
	//DDX_Control(pDX, IDC_GAME_BTN_09, m_Btbt9);
	//DDX_Control(pDX, IDC_GAME_BTN_10, m_Btbt10);
	DDX_Control(pDX, IDC_SNDPLAY,m_btSndPlay);
	DDX_Control(pDX, IDC_SNDPAUSE,m_btSndPause);
	DDX_Control(pDX, IDC_SNDVOLDOWN,m_btVolDown);
	DDX_Control(pDX, IDC_SNDVOLUP,m_btVolUp);



}
//����ini�ļ�·��
void CLoveSendClass::SetInifile(LPCTSTR path)
{
	m_iniFile=path;
	m_UserListDlg.m_UserList.SetIniFile(path);
}
//007��ʼ������
BOOL CLoveSendClass::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_PlaySound.CreateWnd(this,IDD_NULL);
	m_pGameView->m_showcoredlg.m_ucomtype=GetComType();
	m_pGameView->m_showcoredlg.m_count=m_pGameInfo->uDeskPeople;
	//��ȡ���ò���
	m_pGameInfo->bEnableWatch=(GetProfileInt(TEXT("CanLooking"),FALSE)!=FALSE);
	m_pGameInfo->bEnableSound=(GetProfileInt(TEXT("PlaySound"),TRUE)!=FALSE);
	m_pGameInfo->bEnableBackSound=(GetProfileInt(TEXT("BackSound"),TRUE)!=FALSE);
	m_pGameInfo->bShowUserInfo=(GetProfileInt(TEXT("ShowUserInfo"),TRUE)!=FALSE);

	m_CurpUserInfoClicked=GetMeUserInfo();
	LoadSkin();
	//������Ϸ��ͼ
	HINSTANCE hInstance=GetModuleHandle(FACE_DLL_NAME);
	AfxSetResourceHandle(hInstance);
	//m_ADDialog.Create(IDD_FLASH_DLG,this);
	m_UserListDlg.Create(IDD_USER_LIST,this);

	m_UserListDlg.InitListCtrl(m_pGameInfo->pOrderName,m_pGameInfo->uComType);
	m_UserListDlg.SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE);
//	m_ie.SetWindowPos(&m_UserListDlg,0,0,0,0,SWP_NOSIZE);
	//m_ADDialog.ShowWindow(SW_HIDE);
	m_UserListDlg.ShowWindow(SW_SHOW);
	//m_pGameView->Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE,CRect(0,0,0,0),this,ID_GAME_VIEW,NULL);

	//���ÿؼ�
	m_TalkMessage.Init();
	m_TalkMessage.SetTextMode(TRANSPARENT);
	m_TalkMessage.IsGameMessage();						//�����������Ϣ���������治ͬ��ini�ļ����б��ڴ���

	PostMessageA(WM_RELOAD_TALKINI);

	////////////////////
	if (CUserPower::CanLookIP(m_pGameInfo->dwMasterPower)) 
		m_UserListDlg.EnableIPView(true);
	m_InputMessage.LimitText(CUserPower::CanSendMuch(m_pGameInfo->dwGamePower)?MAX_TALK_LEN:NORMAL_TALK_LEN);
	//���ù��
	
	//////////////////////////////////////////////////////////////////////////
	//��ȡ���ڹ���ַ,Fred Huang,2008-06-02
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	szAdvUrl=f.GetKeyVal (Glb().m_key,"gameadvertise","");
	if(szAdvUrl=="")
		m_ie.MoveWindow(-10,-10,5,5);
	else
	{
		CString surl;
		if(szAdvUrl.Find("?")!=-1)
			surl.Format("%s&roomid=%d",szAdvUrl,m_pGameInfo->uRoomID);
		else
			surl.Format("%s?roomid=%d",szAdvUrl,m_pGameInfo->uRoomID);
		m_ie.Navigate(surl,0,0,0,0);
	}
	//////////////////////////////////////////////////////////////////////////	
	UpdateGameTitle();
	if(GetComType()==TY_MATCH_GAME)
	{
		m_InputMessage.EnableWindow(false);
		m_BtExpression.EnableWindow(false);
		m_BtSend.EnableWindow(false);
	}	
	//�����
	m_TalkMessage.SetFocus();

	AfxSetResourceHandle(GetModuleHandle(NULL));
	LoadSkin();
	//���ÿؼ�
	m_TalkMessage.Init();
	m_bInit = true;
	//{add by zxj 2008/04/07
	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool(GetDlgItem(IDC_GAME_BTN_01), TEXT("������Ϸ���ѡ��"));
	m_ToolTip.AddTool(GetDlgItem(IDC_GAME_BTN_02), TEXT("�������"));
	m_ToolTip.AddTool(GetDlgItem(IDC_GAME_BTN_03), TEXT("��ֵ"));
	m_ToolTip.AddTool(GetDlgItem(IDC_GAME_BTN_04), TEXT("����ҳ"));
	m_ToolTip.AddTool(GetDlgItem(IDC_GO_AHEAD), TEXT("��ǰ�鿴����"));
	m_ToolTip.AddTool(GetDlgItem(IDC_GO_NEXT), TEXT("���鿴����"));
	m_ToolTip.AddTool(GetDlgItem(IDC_EXPRESSION),TEXT("ѡ�����ͷ��"));
	m_ToolTip.AddTool(GetDlgItem(IDC_SEND),TEXT("����������Ϣ"));
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP1), LPSTR_TEXTCALLBACK);
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP2), LPSTR_TEXTCALLBACK);
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP3), LPSTR_TEXTCALLBACK);
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP4), LPSTR_TEXTCALLBACK);
    
	//wushuqun ���ֲ��Ű�ť����ʾ

	m_ToolTip.AddTool(GetDlgItem(IDC_SNDPLAY),TEXT("��������")); 
	m_ToolTip.AddTool(GetDlgItem(IDC_SNDPAUSE),TEXT("ֹͣ��������")); 
	m_ToolTip.AddTool(GetDlgItem(IDC_SNDVOLDOWN),TEXT("��С����")); 
	m_ToolTip.AddTool(GetDlgItem(IDC_SNDVOLUP),TEXT("��������"));
	//end add}

	LoadSoundList();

	m_pGameView->Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE,CRect(0,0,0,0),this,ID_GAME_VIEW,NULL);
	m_pGameView->UpdateViewFace(NULL);
	CRect rect;
	::SystemParametersInfo(SPI_GETWORKAREA,NULL,&rect,NULL);
	FixControlStation(rect.Width(),rect.Height());

	OnInitPropList();

	 // added by yjj for record 090223
	if (m_pGameInfo->bIsInRecord) 
	{
		m_bWatchMode = true;
		m_bWatchOther = true;
	}

	checkNetSignal();                        //�ȼ��һ��
	//SetTimer(ID_CHECK_SIGNAL,CHECK_TIME*2,NULL);        //ÿ6����һ�Σ�CHECK_TIME��ԭ����� 3000 ��ôһ��ֵ

	m_bkBrush = CreateSolidBrush (m_userbkcr);


	return false;
}
LRESULT CLoveSendClass::OnReLoadTalkINI(WPARAM wparam, LPARAM lparam)
{
	//���ԭ������
	m_InputMessage.ResetContent();
	//��talkInGame.ini�ļ��ж�ȡ������
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "talkInGame.bcf");
	CString key="BZW_TALK";
	//	TCHAR path[MAX_PATH];
	CString szTalk;
	CString szNumber;
	m_nTalkDefaultCount=0;
	do
	{
		szNumber.Format("%d",++m_nTalkDefaultCount);
		szTalk=f.GetKeyVal(key,"T"+szNumber,"");
		if(!szTalk.IsEmpty())
			m_InputMessage.AddString(szTalk);
	}while(!szTalk.IsEmpty());
	m_nTalkDefaultCount--;
	return 0;
}
//010���±���
void CLoveSendClass::UpdateGameTitle()
{
	if (m_pGameInfo!=NULL)
	{
		CString strBuffer;
		//strBuffer.Format(TEXT("%s %d.%02d"),m_pGameInfo->szGameName,HIWORD(m_pGameInfo->dwGameMSVer),LOWORD(m_pGameInfo->dwGameLSVer));
		strBuffer.Format(TEXT("%s %d.%02d"),m_szGameRoomName,HIWORD(m_pGameInfo->dwGameMSVer),LOWORD(m_pGameInfo->dwGameLSVer));

		if (m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskNO!=255)
		{
			CString strDeskStation;
			strDeskStation.Format(TEXT(" - [%ld]����Ϸ��"),m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskNO+1);
			strBuffer+=strDeskStation;
		}
		SetWindowTitle(strBuffer);
		m_pGameView->SetViewTitle(strBuffer);
	}
	return;
}

//��Ϣ���ͺ���
BOOL CLoveSendClass::PreTranslateMessage(MSG * pMsg)
{
	if(pMsg->message==WM_SYSCOMMAND && pMsg->wParam==SC_RESTORE)
	{
		if(pMsg->wParam==SC_MAXIMIZE)
		{
			m_bMax=true;
			CRect rect;
			::SystemParametersInfo(SPI_GETWORKAREA,NULL,&rect,NULL);
			MoveWindow(0,0,rect.Width(),rect.Height());
			return TRUE;
		}
		if(pMsg->wParam==SC_RESTORE)
		{
			ShowWindow(SW_RESTORE);
			if(m_bShowMax)
			{
				m_bMax=true;
				CRect rect;
				::SystemParametersInfo(SPI_GETWORKAREA,NULL,&rect,NULL);
				MoveWindow(0,0,rect.Width(),rect.Height());}
			else
			{
				m_bMax=false;

				MoveWindow(0,0,m_SavedScreen.x,m_SavedScreen.y);
				CenterWindow();
			}
			SetForegroundWindow();
			return TRUE;
		}
	}
	////////////////////////////
	///Kylin 20090122 ʹ�õ���
	//����ȡ����Ϣ�ͻس���Ϣ
	//if(pMsg->message==UM_USEITEM_USERSELECTED)
	//{
	//	for(int i=0; i<MAX_PEOPLE; i++)
	//	{
	//		///////////////////////////////////////////////////////////
	//		//////Kylin 20090121 ���ܸ������û�ʹ�õ���
	//		if(m_pUserInfo[i])
	//		{
	//			if(m_pUserInfo[i]->GameUserInfo.dwUserID == pMsg->lParam )
	//			{
	//				if(m_pUserInfo[i]->GameUserInfo.bUserState==USER_CUT_GAME)
	//				{
	//					AFCMessageBox("���ܶԶ����û�ʹ�øõ���","��ʾ",AFC_YESNO);
	//				}
	//				else	int nRet = OnUseProp(pMsg->wParam, m_pUserInfo[i]->GameUserInfo.dwUserID);
	//				break;
	//			}
	//		}
	//		///////////////////////////////////////////////////////////
	//	}
	//	return TRUE;
	//}
	////////////////////////////
	if (pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_RETURN)
		{	
			SendTalkMessage();
			return TRUE;
		}
		else if (pMsg->wParam == VK_F1)
		{
			return TRUE;
		}
		else 
			if (pMsg->wParam==VK_ESCAPE) 
				return TRUE;
	}
	//{add by HuangYin 2008/04/07
	if(pMsg->message==WM_MOUSEMOVE)
	{
		if(this->GetFocus()==(CWnd*)&m_BtPropGoAhead || 
			this->GetFocus()==(CWnd*)&m_BtPropGoNext)
			this->SetFocus();
	}
	//end add}
	m_ToolTip.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//�˳�����
void CLoveSendClass::OnCancel()
{
	//��ѯ״̬
	if ((m_bWatchMode==false)&&GetStationParameter() >= 20 && GetStationParameter() < 23)//!CanLeftDesk())
	{
		//if (IDCANCEL!=AFCMessageBox("��������Ϸ�У������˳����ᱻ�۷֣�ȷʵҪ�˳���","��ʾ",AFC_YESNO))//,GetModuleHandle(FACE_DLL_NAME),m_pGameInfo->szGameName,MB_YESNO|MB_DEFBUTTON2|MB_ICONINFORMATION,this))
		if (IDCANCEL!=DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION, "��ʾ",true,"��������Ϸ�У������˳����ᱻ�۷֣�ȷʵҪ�˳���"))
			return;
		SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	}
	if (m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskNO!=255) 
		SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);

	//�رմ���
	AFCCloseFrame();
	return;
}
//��Ƶ���øı�
LRESULT CLoveSendClass::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DISPLAYCHANGE:
		//ShowWindow(SW_NORMAL);
		ShowMaxSize();
		ShowWindow(SW_MINIMIZE);
		return true;
		/*case WM_CTLCOLORDLG:
		//case WM_CTLCOLORSTATIC :
		SetTextColor((HDC)wParam, RGB(0,0,0)) ;
		SetBkColor((HDC)wParam, m_PicShowMeBg.GetPixel(0,0));
		return (LRESULT)CreateSolidBrush(m_PicShowMeBg.GetPixel(0,0));*/
	}
	return CLoadFileBmp::WindowProc(message, wParam, lParam);
}

//��ʼ��
bool CLoveSendClass::InitFrame(GameInfoStruct * pGameInfo, CWnd * pGameRoom)
{
	m_pGameInfo=pGameInfo;
	m_pGameRoom=pGameRoom;
	m_pGameRoom->SendMessage(IDM_GET_ROOMNAME,0,(LPARAM)&(m_szGameRoomName[0]));
	return true;
}

//�رմ���
bool CLoveSendClass::AFCCloseFrame()
{
	//�������ò���
	WriteProfileInt(TEXT("CanLooking"),m_pGameInfo->bEnableWatch?1:0);
	WriteProfileInt(TEXT("PlaySound"),m_pGameInfo->bEnableSound?1:0);
	WriteProfileInt(TEXT("BackSound"),m_pGameInfo->bEnableBackSound?1:0);
	WriteProfileInt(TEXT("ShowUserInfo"),m_pGameInfo->bShowUserInfo?1:0);
	StopBackMusic();
	//m_DlgGetPoint.ShowWindow(false);
	//���ò���
	SetStationParameter(0);
	//ȥ����ʾ
	if(m_DlgShowLeft.Showing)
	{
		m_DlgShowLeft.KillTimer(1);
		m_DlgShowLeft.ShowWindow(0);
		m_DlgShowLeft.Showing=0;
		m_DlgShowLeft.DestroyWindow();
	}
	for (INT_PTR i=0;i<m_ShortMessage.GetCount();i++) 
	{
		delete ((CShortMessage *)m_ShortMessage.GetAt(i));
		m_ShortMessage.SetAt(i,NULL);
	}
	//���͹ر���Ϣ��������
	//::SendMessage(m_pGameRoom->m_hWnd,IDM_CLOSE_GAME,0,0);
	
	if (GetSafeHwnd()!=NULL) 
	{
		////////////////////////////////
		/////Kylin 20090120 ������
		m_pGameView->ClearGif();//add 20090120 by Shao;����gif�������ţ�
		////////////////////////////////
		m_pGameView->ResetGameView();
		ResetGameFrame();
		this->ShowWindow (SW_HIDE);
		//this->DestroyWindow();
	}
	BZSoundPause(m_sndListID);
	CWnd * winApp=AfxGetApp()->m_pMainWnd;
	if(winApp && winApp->m_hWnd)
		winApp->PostMessage(WM_USER+100,3,0);

	//yjj add for leave save record msg 090223
	if (m_pGameRoom != NULL && IsWindow(m_pGameRoom->GetSafeHwnd()))
	{
		m_pGameRoom->PostMessage(IDM_CLOSE_GAME,0,0);
	}
	
	return true;
}
//====�������ɫ
void CLoveSendClass::SetFillColor(COLORREF rgb)
{
	if(!m_TalkMessage.m_hWnd)
		return;
	m_TalkMessage.SetBackgroundColor(false,rgb);
	m_TalkMessage.m_bkColor=rgb;
}


//������Ϣ 
UINT CLoveSendClass::OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize)
{
	//�жϴ����Ƿ���Ч
	if (GetSafeHwnd()==NULL) 
		return 0;

	//��������
	switch (uControlCode)
	{
	case CM_USER_SEND_TIMES_MONEY:
		{
            // PengJiLin, 2011-4-15
			/*if (uDataSize!=sizeof(CM_UserState_Send_Times_Money)) return 1;
			CM_UserState_Send_Times_Money* pMoneyChange = (CM_UserState_Send_Times_Money*)pControlData;

			if (pMoneyChange->dwUserID != m_pGameInfo->pMeUserInfo->GameUserInfo.dwUserID)
				return 0;

			TCHAR str[100];
			TCHAR szNum[32];GetNumString(szNum, pMoneyChange->iSendMoney, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
			if(pMoneyChange->iLastTimes<=0)
			{
			wsprintf(str,"/:54����%d��ң��ѻ���%s�Σ����޻�������/:54",szNum,
				pMoneyChange->iSendTimes,pMoneyChange->iLastTimes);
			}
			else
			{
			wsprintf(str,"/:54����%d��ң��ѻ���%d�Σ�����%s�λ�������/:54",szNum,
				pMoneyChange->iSendTimes,pMoneyChange->iLastTimes);
			}

			m_MessageHandle.InsertNormalMessage(str);*/
			return 0;

		}
		break;
	case CM_USER_STATE:		//�û�״̬�ı�
		{
			//Ч������
			if (uDataSize!=sizeof(CM_UserState_Change)) return 1;
			CM_UserState_Change * pStateChange=(CM_UserState_Change *)pControlData;

			//��������
			switch (pStateChange->bActionCode)
			{
			case ACT_USER_UP:		//�û�����
			case ACT_WATCH_UP:		//�Թ�����
				{
					//��������
					if (pStateChange->pUserItem!=m_pGameInfo->pMeUserInfo)//ͬ�����������뿪
					{
						GameUserLeft(pStateChange->bDeskStation,pStateChange->pUserItem,pStateChange->bActionCode==ACT_WATCH_UP);

						if ((GetComType()!=TY_MATCH_GAME)&&m_pGameInfo->bShowUserInfo==true)
						{	
							m_MessageHandle.InsertUserName(pStateChange->pUserItem->GameUserInfo.nickName);
							if (pStateChange->bActionCode == ACT_WATCH_UP) 
								m_MessageHandle.InsertNormalMessage(TEXT("�뿪��"));
							else 
							{
								m_MessageHandle.InsertNormalMessage(TEXT("�뿪��"));
								//��ʾ�����ڣ���ʾ��ʾ���ڣ�ʲô��������
								if(!m_DlgShowLeft.Showing && GetSafeHwnd()!=NULL
									&&(m_pGameInfo->dwRoomRule & GRR_ALL_ON_DESK)== 0L)//��Ҫȫ�����뿪
								{
									m_DlgShowLeft.GetNickName(pStateChange->pUserItem->GameUserInfo.nickName);
									m_DlgShowLeft.Create(IDD_SHOW_LEFT,this);
									m_DlgShowLeft.ShowWindow(1);
									m_DlgShowLeft.MoveWindow(400,300,300,200);
									m_DlgShowLeft.Showing=1;
									m_DlgShowLeft.BeginTimer();
									EnableWindow(FALSE);
								}
							}
						}
					}
					else//�Լ��뿪
					{
						if (m_uTimeID!=0L) 
							KillGameTimer(m_uTimeID);
						if (m_bSaveView==false)
						{
							this->ResetGameStation();
							m_pGameView->ResetGameView();
							ResetGameFrame();
						}
						//�����뿪
						if(!m_DlgShowLeft.Showing)
							AFCCloseFrame();
					}

					OnBnClickedEnd();
					return 0;
				}
			case ACT_USER_SIT:		//�û�����
			case ACT_WATCH_SIT:		//�Թ�����
				{
					//��������
					if (pStateChange->pUserItem!=m_pGameInfo->pMeUserInfo)
					{
						GameUserCome(pStateChange->bDeskStation,pStateChange->pUserItem,pStateChange->bActionCode==ACT_WATCH_SIT);
						if (m_pGameInfo->bShowUserInfo==true&&(GetComType()!=TY_MATCH_GAME))
						{
							m_MessageHandle.InsertUserName(pStateChange->pUserItem->GameUserInfo.nickName);
							if (pStateChange->bActionCode==ACT_WATCH_SIT)
								m_MessageHandle.InsertNormalMessage(TEXT("������"));
							else 
								m_MessageHandle.InsertNormalMessage(TEXT("������"));
						}
					}
					else
					{
						OnBnClickedMax();
						//Ч��״̬
						BYTE bMeDeskStation=m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskNO;
						if (bMeDeskStation==255)
							return 0;

						//��ȡ�û�����
						CPtrArray PlayUser,WatchUser;
						m_pGameInfo->pIFindUser->FindOnLineUser(bMeDeskStation,PlayUser,WatchUser);

						for (INT_PTR i=0;i<PlayUser.GetCount();i++)
						{
							UserItemStruct * pUserItem=(UserItemStruct *)PlayUser.GetAt(i);
							if (pUserItem!=NULL) 
								GameUserCome(pUserItem->GameUserInfo.bDeskStation,pUserItem,false);
						}
						for (INT_PTR i=0;i<WatchUser.GetCount();i++)
						{
							UserItemStruct * pUserItem=(UserItemStruct *)WatchUser.GetAt(i);
							if (pUserItem!=NULL) 
								GameUserCome(pUserItem->GameUserInfo.bDeskStation,pUserItem,true);
						}

						//���ͻ�ȡ��Ϣ
						MSG_GM_S_ClientInfo ClientInfo;
						ClientInfo.bEnableWatch=m_pGameInfo->bEnableWatch;
						SendGameData(&ClientInfo,sizeof(ClientInfo),MDM_GM_GAME_FRAME,ASS_GM_GAME_INFO,0);
						UpdateGameTitle();
					}

					OnBnClickedEnd();
					return 0;
				}
			case ACT_USER_CUT:		//�û�����
				{

					//��������
					m_UserListDlg.UpdateGameUser(pStateChange->pUserItem);
					m_pGameView->UpdateUserInfo(pStateChange->bDeskStation);
					//��ʾ��Ϣ
					if(GetComType()!=TY_MATCH_GAME)
						m_MessageHandle.InsertUserName(pStateChange->pUserItem->GameUserInfo.nickName);
					m_MessageHandle.InsertNormalMessage(TEXT("������"));

					OnBnClickedEnd();
					return 0;
				}
			case ACT_USER_AGREE:	//�û�ͬ��
				{
					UserAgreeGame(pStateChange->bDeskStation);
					return 0;
				}
			case ACT_GAME_BEGIN:	//��Ϸ��ʼ
			case ACT_GAME_END:		//��Ϸ����
				{
					for (BYTE i=0;i<m_pGameInfo->uDeskPeople;i++)
					{
						if (m_pUserInfo[i]!=NULL) 
						{
							m_UserListDlg.UpdateGameUser(m_pUserInfo[i]);
						}
					}
					m_pGameView->UpdateUserInfo(ViewStation(m_pGameInfo->uDeskPeople));
					OnBnClickedEnd();
					return 0;
				}
			case ACT_USER_POINT:	//�û�����ֵ
				{
					m_UserListDlg.UpdateGameUser(pStateChange->pUserItem);
					if(pStateChange->pUserItem->GameUserInfo.dwUserID==m_CurpUserInfoClicked->dwUserID)
					{
						NavigateVirFace();
					}

					if (pStateChange->pUserItem->GameUserInfo.bUserState!=USER_WATCH_GAME)
					{
						m_pGameView->UpdateUserInfo(pStateChange->bDeskStation);
						//�����û�����ֵ
						/*for(int i=0;i<MAX_PEOPLE;i++)
						{
						if(this->m_pUserInfo[i]!=NULL)
						{
						if(m_pUserInfo[i]->GameUserInfo.dwUserID==pStateChange->pUserItem->GameUserInfo.dwUserID)
						{
						m_pUserInfo[i]->GameUserInfo.dwPoint==pStateChange->pUserItem->GameUserInfo.dwPoint;
						if(pStateChange->pUserItem->GameUserInfo.bDeskStation==GetMeUserInfo()->bDeskStation)
						InvalidateRect(m_UserInfoRect[0]);
						else if(i==0)
						InvalidateRect(m_UserInfoRect[GetMeUserInfo()->bDeskStation]);
						else
						InvalidateRect(m_UserInfoRect[i]);
						}

						}
						}*/
						//for(int i=0;i<4;i++)
						//	InvalidateRect(m_UserInfoRect[i]);

					}
					//���˻��ָ����ж��Ƿ���Ҫ�뿪
					if(GetMeUserInfo()->dwUserID == pStateChange->pUserItem->GameUserInfo.dwUserID)//�Ƿ���Ҫ�뿪��Ϸ��
					{
						if(GetMeUserInfo()->bUserState!= USER_PLAY_GAME && GetMeUserInfo()->i64Money < m_pGameInfo->uLessPoint)
						{
							OnBnClickedQuitGame();
						}	
					}
					//OnBnClickedEnd();
					return 0;
				}
			case ACT_USER_CHARM://��������ֵ
				{
					m_UserListDlg.UpdateGameUser(pStateChange->pUserItem);
					if(pStateChange->pUserItem->GameUserInfo.dwUserID==m_CurpUserInfoClicked->dwUserID)
					{
						NavigateVirFace();
					}
					if (pStateChange->pUserItem->GameUserInfo.bUserState!=USER_WATCH_GAME)
					{
						m_pGameView->UpdateUserInfo(pStateChange->bDeskStation);
					}
					return 0;
				}
			}
		}
	}

	return 0;
}

//016�û�����
bool CLoveSendClass::GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	//====�û�����
	if (bDeskStation<MAX_PEOPLE)
	{
		//���ñ���
		if (bWatchUser==true) 
		{
			INT_PTR i=0;
			for (i=0;i<m_WatchPtrArray.GetCount();i++)
			{
				if (m_WatchPtrArray.GetAt(i)==NULL) break;
			}
			m_WatchPtrArray.SetAtGrow(i,pUserItem);
		}
		else
		{
			m_pUserInfo[bDeskStation]=pUserItem;
			//m_bDeskStation[bDeskStation]=1;	
			//this->InvalidateRect(m_UserInfoRect[bDeskStation]);
		}

		if(pUserItem->GameUserInfo.dwUserID==GetMeUserInfo()->dwUserID)
			m_UserListDlg.AddGameUser(pUserItem);
		//���ý���
		if(GetComType()!=TY_MATCH_GAME)
		{//����ʱ,ֻ��ʾ�Լ�
			//if(pUserItem->GameUserInfo.dwUserID==GetMeUserInfo()->dwUserID)
				m_UserListDlg.AddGameUser(pUserItem);
		}
		//else
		//s	m_UserListDlg.AddGameUser(pUserItem);

		if (bWatchUser==true) 
		{
			BYTE bDeskStation=pUserItem->GameUserInfo.bDeskStation;
			if (m_pUserInfo[bDeskStation]!=NULL)
				m_UserListDlg.SetWatchUser(pUserItem,m_pUserInfo[bDeskStation]->GameUserInfo.nickName);
		}
		else 
		{
			m_pGameView->SetUserInfo(ViewStation(bDeskStation),pUserItem);
		}

		//		if ((m_pGameInfo->pMeUserInfo!=pUserItem)
		//			&&(m_TargetPeople.FindString(0,pUserItem->GameUserInfo.nickName)==LB_ERR))
		//		{
		//			m_TargetPeople.AddString(pUserItem->GameUserInfo.nickName);
		//		}
		//{add by zxj for get the user's prop list 2008/04/07
		//OnGetHoldItemData(0,0);
		//end add}

		return true;
	}
	return false;
}

//015�û��뿪
bool CLoveSendClass::GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	if (bDeskStation<MAX_PEOPLE)
	{
		//���ñ���
		if (bWatchUser==true) 
		{
			UserItemStruct * pFindUserItem=NULL;
			for (INT_PTR i=0;i<m_WatchPtrArray.GetCount();i++)
			{
				pFindUserItem=(UserItemStruct *)m_WatchPtrArray.GetAt(i);
				if (pUserItem==pFindUserItem)
				{
					m_WatchPtrArray.SetAt(i,NULL);
					break;
				}
			}
		}
		else {
			m_pUserInfo[bDeskStation]=NULL;
			//	m_bDeskStation[bDeskStation]=0;
			//this->InvalidateRect(m_UserInfoRect[bDeskStation]);
			//	m_pGameView->SetUserInfo(ViewStation(bDeskStation),NULL);
		}

		//���ý���
		m_UserListDlg.DeleteGameUser(pUserItem);
		if(pUserItem->GameUserInfo.dwUserID==m_CurpUserInfoClicked->dwUserID)
		{
			m_CurpUserInfoClicked=GetMeUserInfo();
			NavigateVirFace();
		}
		if (bWatchUser==false) 
			m_pGameView->SetUserInfo(ViewStation(bDeskStation),NULL);
		//int iFindCode=m_TargetPeople.FindString(0,pUserItem->GameUserInfo.nickName);
		//if (iFindCode!=LB_ERR) 
		//{
		//	if (m_TargetPeople.GetCurSel()==iFindCode)
		//		m_TargetPeople.SetCurSel(0);
		//	m_TargetPeople.DeleteString(iFindCode);
		//}

		return true;
	}
	return false;
}

//�û�ͬ��
bool CLoveSendClass::UserAgreeGame(BYTE bDeskStation)
{
	if ((bDeskStation<MAX_PEOPLE)&&(m_pUserInfo[bDeskStation]!=NULL))
	{
		//���ý���
		m_UserListDlg.UpdateGameUser(m_pUserInfo[bDeskStation]);
		m_pGameView->UpdateUserInfo(ViewStation(bDeskStation));
		return true;
	}
	return false;
}

//�����ȡ��Ϣ
bool CLoveSendClass::OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	//���˴���
	if (m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskNO==255) 
		return true;
	//������Ϣ
	switch (pNetHead->bMainID)
	{
	case MDM_GR_NETSIGNAL:
		{
			getNetSignal(pNetData);
			return true;
		}
	case MDM_GM_GAME_FRAME:		//�����Ϣ
		{
			if (GetSafeHwnd()==NULL)
				return true;
			return OnFrameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GM_MESSAGE:		//��Ϣ��Ϣ
		{
			if (GetSafeHwnd()==NULL) 
				return true;
			return OnSystemMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GM_GAME_NOTIFY:	//��Ϸ��Ϣ
		{
			if (this == NULL)
			{
				AFCMessageBox("o");
				return true;
			}
			if (!this->m_hWnd) 
				return true;
			return HandleGameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	case MDM_GR_POINT://�����
		{
			if(NULL == GetSafeHwnd())
				return true;
			return HandlePointMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	//case MDM_GR_USE_ITEM://ʹ�õ���
	//	{
	//		if(NULL == GetSafeHwnd())
	//			return true;
	//		return OnUserUseItemMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	//	}

	case MDM_GR_PROP:
		{
			if(!GetSafeHwnd())
				return true;
			return OnPropMessage(pNetHead,pNetData,uDataSize,pClientSocket);
		}
	//case MDM_GR_ITEM_DATA_MSG://������Ϣ
	//	{
	//		if (GetSafeHwnd() == NULL)
	//			return true;
	//		return OnItemDataMsg(pNetHead, pNetData, uDataSize, pClientSocket);
	//	}
	//case MDM_GR_VIP_MSG://vip����
	//	{
	//		if (GetSafeHwnd() == NULL)
	//			return true;
	//		return OnVipOperationMsg(pNetHead, pNetData, uDataSize, pClientSocket);
	//	}
	}
	return false;
}

//������Ϣ
bool CLoveSendClass::OnPropMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch(pNetHead->bAssistantID)
	{
		//wushuqun 2008.9.28
		//�޸�С����ʹ�ú�û�и��¿ͻ��˵��������ʾ 
	case ASS_PROP_SMALL_BOARDCASE:
		{
			OnSetPropImages();
			this->Invalidate(FALSE);
			break;
		}
	case ASS_PROP_USEPROP:
		return OnUsePropResult(pNetHead,pNetData,uDataSize,pClientSocket);
		break;
	case ASS_PROP_VIP_CHECKTIME:
		return OnPropVipCheckTime(pNetHead,pNetData,uDataSize,pClientSocket);
		break;
	//case ASS_S_GET_ITEM_DATA:	//��ȡ
	//	return OnUpdateHoldItemResult(pNetHead,pNetData,uDataSize,pClientSocket);
	//	break;
	//case ASS_S_USE_ITEM:		//ʹ�õ���
	//	return OnUserUseItemResult(pNetHead,pNetData,uDataSize,pClientSocket);
	//	break;
	//case ASS_S_GET_BUY_ITEM_DATA://�����б�
	//	return OnGetBuyItemResult(pNetHead,pNetData,uDataSize,pClientSocket);
	//	break;
	//case ASS_S_BUY_ITEM_DATA:	//������
	//	return OnBuyItemResult(pNetHead,pNetData,uDataSize,pClientSocket);
	//	break;
	}
	return true;
}


//���������Ϣ
bool CLoveSendClass::OnUsePropResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if(pNetHead->bHandleCode!=DTK_GR_PROP_USE_SUCCEED)
		return true;
	if(sizeof(_TAG_USINGPROP)!= uDataSize)
		return false;
	_TAG_USINGPROP * usingProp = (_TAG_USINGPROP *)pNetData;
	UserItemStruct *pUserInfo =m_pGameInfo->pIFindUser->FindOnLineUser(usingProp->dwTargetUserID);
	UserItemStruct *pUserInfoUsed =m_pGameInfo->pIFindUser->FindOnLineUser(usingProp->dwUserID);
	if(pUserInfo == NULL)
		return true;
	//ʹ���ߵ��ǳ�
	CString name1=pUserInfoUsed->GameUserInfo.nickName;
	//��ʹ���ߵ��ǳ�
	CString name2=pUserInfo->GameUserInfo.nickName;
	
	if(usingProp->dwUserID==m_pGameInfo->pMeUserInfo->GameUserInfo.dwUserID)
		name1="��";
	if(usingProp->dwTargetUserID==m_pGameInfo->pMeUserInfo->GameUserInfo.dwUserID)
		name2="��";
	CString name3=name2;
	if(usingProp->dwUserID==usingProp->dwTargetUserID)
		name2="�Լ�";

	int attribAction=usingProp->nPropActionAttrib;
	int attribValue=usingProp->nPropValueAttrib;
	
	CString stip;
	stip.Format("%s �� %s ʹ���� %s ����",name1,name2,usingProp->szPropName);
	m_MessageHandle.InsertSystemMessage(stip.GetBuffer());
	
	//if(m_hWnd)
	//	Invalidate(FALSE);

	//˫������
	if(attribAction & 0x4)
	{
		pUserInfo->GameUserInfo.iDoublePointTime=usingProp->iDoubleTime;
		if(usingProp->dwTargetUserID==m_pGameInfo->pMeUserInfo->GameUserInfo.dwUserID)
		{
			time_t t=(time_t)usingProp->iDoubleTime;
			CTime expiresTime(t);
			stip.Format(TEXT("����˫�����ֹ��ܿ���ʹ�õ� %s��"),expiresTime.Format("%Y-%m-%d %H:%M:%S"));
			m_MessageHandle.InsertSystemMessage(stip.GetBuffer());
		}
	}
	if(attribAction & 0x8)
	{
		pUserInfo->GameUserInfo.iProtectTime=usingProp->iProtectTime;
		if(usingProp->dwUserID == m_pGameInfo->pMeUserInfo->GameUserInfo.dwUserID)
		{
			time_t t=(time_t)usingProp->iProtectTime;
			CTime expiresTime(t);
			stip.Format(TEXT("���Ļ��������(���˲��ۻ���)����ʹ�õ� %s��"),expiresTime.Format("%Y-%m-%d %H:%M:%S"));
			m_MessageHandle.InsertSystemMessage(stip.GetBuffer());
		}
	}

	if(attribAction & 0x10)
	{
		int fasciChange=(attribValue & 0xFF0000)>>16;
		stip.Format("%s ������ֵ���� %d ��",name3,fasciChange);
		if(attribAction & 0x20)
		{
			stip.Format("%s ������ֵ���� %d ��",name3,fasciChange);
			fasciChange=-fasciChange;
		}
	//	pUserInfo->GameUserInfo.dwFascination+=fasciChange;
		m_MessageHandle.InsertSystemMessage(stip.GetBuffer());
	}
	if(attribAction & 0x2)
	{
		
		if(usingProp->dwTargetUserID==m_pGameInfo->pMeUserInfo->GameUserInfo.dwUserID)
		{
			if(m_pGameInfo->pMeUserInfo->GameUserInfo.dwPoint<0)
				m_pGameInfo->pMeUserInfo->GameUserInfo.dwPoint=0;
		}
		if (pUserInfoUsed->GameUserInfo.dwPoint<0)
		{
			pUserInfoUsed->GameUserInfo.dwPoint = 0;
		}
		if (pUserInfoUsed->GameUserInfo.dwUserID == m_CurpUserInfoClicked->dwUserID)
		{
			NavigateVirFace();
		}

	}

	//��������б�
	m_UserListDlg.UpdateGameUser(pUserInfo);
	OnSetPropImages();
//	this->Invalidate(FALSE);

	return true;
}

//vipʱ����
bool CLoveSendClass::OnPropVipCheckTime(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if(uDataSize!=sizeof(_TAG_PROP_TIME))
		return false;
	_TAG_PROP_TIME * pData = (_TAG_PROP_TIME *)pNetData;
	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
	int curtime=atoi(stime);                        //ת��Ϊint

	if(GetMeUserInfo()->iVipTime-curtime < 10*60 && GetMeUserInfo()->iVipTime > curtime)
	{
		//��������
		CString strMessage;
		strMessage.Format(TEXT("����Vip��ݽ��� %d ���Ӻ����!"),GetMeUserInfo()->iVipTime/60);
		m_MessageHandle.InsertSystemMessage(strMessage.GetBuffer());
	}
	else
	{
		if(GetMeUserInfo()->iVipTime <=curtime)
		{
			//��������
			CString strMessage;
			strMessage.Format(TEXT("����Vip����Ѿ��������������¹���"));
			m_MessageHandle.InsertSystemMessage(strMessage.GetBuffer());
		}
	}
	OnSetPropImages();
	return true;
}

////vip
//bool CLoveSendClass::OnVipOperationMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
//{
//	switch(pNetHead->bAssistantID)
//	{
//		case ASS_S_VIP_CHECK_TIME:	//vipʱ����	
//			return OnVipCheckMsg(pNetHead,pNetData,uDataSize,pClientSocket);
//		break;
//	}
//	return true;
//}

//��Ϣ����������Ҫ���أ�
bool CLoveSendClass::OnSystemMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	ASSERT(pNetHead->bMainID==MDM_GM_MESSAGE);
	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_SYSTEM_MESSAGE:		//ϵͳ��Ϣ
		{
			// add xqm 2010-11-19
			AFCPlaySound(GetModuleHandle(FACE_DLL_NAME),TEXT("SYSTEM_MSG"));

			//Ч������
			MSG_GA_S_Message * pMessage=(MSG_GA_S_Message *)pNetData;
			//��������
			if ((pMessage->bShowStation&SHS_TALK_MESSAGE)!=0)
			{
				m_MessageHandle.InsertSystemMessage(pMessage->szMessage,pMessage->bFontSize);
			}
			if ((pMessage->bShowStation&SHS_MESSAGE_BOX)!=0)
			{
				m_bSaveView=true;
				SetStationParameter(20);

				//AFCMessageBox(pMessage->szMessage,m_pGameInfo->szGameName);//,MB_ICONQUESTION,this);
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_pGameInfo->szGameName,false,pMessage->szMessage);

				m_bSaveView=false;
				if (pMessage->bCloseFace==TRUE)
					AFCCloseFrame();
			}
			OnBnClickedEnd();
			return true;	
		}
	}
	return true;
}

//�����Ϣ������������Ҫ���أ�
bool CLoveSendClass::OnFrameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	ASSERT(pNetHead->bMainID==MDM_GM_GAME_FRAME);
	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_GAME_INFO:		//��Ϸ��Ϣ
		{
			//Ч������
			MSG_GM_S_GameInfo * pGameInfo=(MSG_GM_S_GameInfo *)pNetData;

			//��������
			m_bWatchOther=(pGameInfo->bWatchOther==TRUE);
			m_bWaitTime=pGameInfo->bWaitTime;
			m_bWatchMode=(m_pGameInfo->pMeUserInfo->GameUserInfo.bUserState==USER_WATCH_GAME);
			// added by yjj for record 090224
			if (m_pGameInfo->bIsInRecord) 
			{
				m_bWatchMode = true;
				m_bWatchOther = true;
			}

			SetStationParameter(pGameInfo->bGameStation);
			//��ʾ��Ϣ
			if (uDataSize>(sizeof(MSG_GM_S_GameInfo)-sizeof(pGameInfo->szMessage)))
			{
				m_MessageHandle.InsertSystemMessage(pGameInfo->szMessage);
			}
			OnBnClickedEnd();
			return true;
		}
	case ASS_GM_GAME_STATION:	//��Ϸ״̬
		{	////	AfxMessageBox("111111111");
			return SetGameStation(pNetData,uDataSize);//����DLL�еĺ��������齫��uDataSizeΪ51
		}
	case ASS_GM_NORMAL_TALK:	//��ͨ����
		{
			//Ч������
			MSG_GR_RS_NormalTalk * pNormalTalk=(MSG_GR_RS_NormalTalk *)pNetData;

			//��������
			UserItemStruct * pSendUserItem=m_pGameInfo->pIFindUser->FindOnLineUser(pNormalTalk->dwSendID);
			if (pSendUserItem==NULL) 
				return true;

			//���˲��ܻ�ӭ�û�
			if (pSendUserItem->uConnection==CM_ENEMY) 
				return true;

			//���������û�
			for (INT_PTR i=0;i<m_ShieldArray.GetCount();i++)
			{
				if (pSendUserItem->GameUserInfo.dwUserID==m_ShieldArray.GetAt(i)) 
					return true;
			}
			TCHAR msg[502];//������Ϣ
			lstrcpy(msg,pNormalTalk->szMessage);
			//MessageBox(msg);
			//������Ϣ
			if (pNormalTalk->dwTargetID!=0L)
			{
				UserItemStruct * pTargerUserItem=m_pGameInfo->pIFindUser->FindOnLineUser(pNormalTalk->dwTargetID);
				if (pTargerUserItem!=NULL) 
				{
					m_MessageHandle.InsertUserTalk(m_pGameInfo->dwRoomRule & GRR_NOTCHEAT?pSendUserItem->GameUserInfo.nickName:"���",GetComType()!=TY_MATCH_GAME?pTargerUserItem->GameUserInfo.nickName:"���",
						pNormalTalk->szMessage,pNormalTalk->crColor,false);
					lstrcpy(pNormalTalk->szMessage,msg);
					ShowUserTalk(pNormalTalk);
					return true;
				}
			}
			m_MessageHandle.InsertUserTalk(m_pGameInfo->dwRoomRule & GRR_NOTCHEAT?pSendUserItem->GameUserInfo.nickName:"���",NULL,pNormalTalk->szMessage,pNormalTalk->crColor,false);
			lstrcpy(pNormalTalk->szMessage,msg);
			if(GetComType()!=TY_MATCH_GAME)
				ShowUserTalk(pNormalTalk);
			if(pNormalTalk->nDefaultIndex>0)
			{
				CString soundFile;
				soundFile.Format("Music\\Talk\\%d.wav",pNormalTalk->nDefaultIndex);
				BZSoundPlay(this,soundFile.GetBuffer(),0,1);
			}
			return true;
		}
	case ASS_GM_WATCH_SET:		//�Թ�����
		{
			ASSERT(m_bWatchMode==true);
			bool bNotifyClient=(m_bWatchOther!=(pNetHead->bHandleCode==TRUE));
			if (bNotifyClient==true)
			{
				m_bWatchOther=(pNetHead->bHandleCode==TRUE);
				if (m_bWatchOther==true) 
					m_MessageHandle.InsertSystemMessage(TEXT("�㱻�����Թ�"));
				else m_MessageHandle.InsertSystemMessage(TEXT("�㱻��ֹ�Թ�"));
				OnWatchSetChange();
			}
			return true;
		}
	}
	return false;
}

//��Ϸ��Ϣ������
bool CLoveSendClass::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bMainID == MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{	
		case ASS_PROP:				//����
			{
				if (uDataSize!=sizeof(_TAG_PROP_MOIVE_USE)) 
					return FALSE;
				_TAG_PROP_MOIVE_USE *propMoive=(_TAG_PROP_MOIVE_USE*)pNetData;

				UserProp(propMoive);// pstProp->sendDeskStation,pstProp->recvDeskStation,pstProp->iValue);
				return true;
			}
		}
	}
	return false;
}

//�������Ϣ������
bool CLoveSendClass::HandlePointMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	return false;
}


//����
//bool CLoveSendClass::OnBroadcastMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
//{
//	return true;
//}


//ɾ������
void CLoveSendClass::DeleteFrame()
{
	if (GetSafeHwnd() != NULL)
		OnCancel();
	delete this;
}

//006λ�ñ仯
void CLoveSendClass::OnSize(UINT nType, int cx, int cy)
{
	FixControlStation(cx,cy);
	__super::OnSize(nType, cx, cy);
	//���������
	//if (m_HorSplitBar.GetSafeHwnd()) 
	//{
	//	SafeMoveWindow(&m_HorSplitBar,cx-209,414,204,SPLIT_WIDE);
	//	m_HorSplitBar.InitSplitBar(310,cy-100,true);
	//}
	return;
}

//�������ý���
void CLoveSendClass::ResetGameFrame()
{
	//�������Ԫ��
	m_UserListDlg.DeleteAllUser();
	//for (int i=1;i<m_TargetPeople.GetCount();i++) 
	//	m_TargetPeople.DeleteString(i);
	//m_TargetPeople.SetCurSel(0);
	UpdateGameTitle();
	//�����û�����
	m_bSaveView=false;
	m_bWatchMode=true;
	m_bWatchOther=false;
	m_pGameInfo->bGameStation=0;

	// added by yjj for record 090223
	if (m_pGameInfo->bIsInRecord) 
	{
		m_bWatchMode = true;
		m_bWatchOther = true;
	}
	//m_pGameInfo->bDeskOnwer=false;
	memset(m_pUserInfo,0,sizeof(m_pUserInfo));
	for (INT_PTR i=0;i<m_WatchPtrArray.GetCount();i++) 
		m_WatchPtrArray.SetAt(i,NULL);
	m_BtGameQuit.m_bIsMouseMove = false;
	m_BtExit.m_bIsMouseMove = false;
	m_Btbt3.m_bIsMouseMove = false;
	return;
}


//���Ͱ�ť
void CLoveSendClass::OnBnClickedSendTalk()
{
	SendTalkMessage();
	return;
}

//���鰴ť
void CLoveSendClass::OnBnClickedExpression()
{
	//�������鴰��
	if (m_ExpressWnd.GetSafeHwnd()==NULL) m_ExpressWnd.CreateExpressionWnd(this);

	//�ƶ�����
	CRect ButtonRect;
	CSize ExpressSize;
	CRect ClientRect;
	GetClientRect(&ClientRect);
	m_ExpressWnd.GetFixSize(ExpressSize);
	m_BtExpression.GetWindowRect(&ButtonRect);
	SafeMoveWindow(&m_ExpressWnd,min(ButtonRect.right,max(0,ClientRect.right-ExpressSize.cx)),ButtonRect.top-ExpressSize.cy,ExpressSize.cx,ExpressSize.cy);
	m_ExpressWnd.ShowWindow(SW_SHOW);
	m_ExpressWnd.SetFocus();

	return;
}

void CLoveSendClass::OnBnClickedSetcolor()
{
	
	
}

//����
void CLoveSendClass::OnBnClickedUp()
{
	::SendMessage(m_TalkMessage.m_hWnd,WM_VSCROLL,SB_LINEUP,0);
	return;
}
//����
void CLoveSendClass::OnBnClickedDown()
{
	::SendMessage(m_TalkMessage.m_hWnd,WM_VSCROLL,SB_LINEDOWN,0);
	return;
}
//����
void CLoveSendClass::OnBnClickedEnd()
{
	::SendMessage(m_TalkMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
	return;
}

////���ð�ť
//void CLoveSendClass::OnBnClickedSetting()
//{
// 	OnGameSetting();
//	return;
//}

//�����б�ť
void CLoveSendClass::OnBnClickedHideList()
{
	CGameImage img;
	HRGN hRgn;
	if(m_pGameView->m_bShowRight)
	{
		m_pGameView->m_bShowRight = false;
		m_BtHide.LoadButtonBitmap(m_ShowbtPath.GetBuffer(),false);
		img.SetLoadInfo(m_ShowbtPath.GetBuffer(),true);
		m_BtHide.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_BtHide.SetWindowRgn(hRgn,true);
	}
	else
	{
		m_pGameView->m_bShowRight = true;
		m_BtHide.LoadButtonBitmap(m_HidebtPath.GetBuffer(),false);
		img.SetLoadInfo(m_HidebtPath.GetBuffer(),true);
		m_BtHide.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_BtHide.SetWindowRgn(hRgn,true);
	}
	CRect rect;
	GetClientRect(&rect);
	FixControlStation(rect.Width(),rect.Height());
	m_pGameView->UpdateViewFace(NULL);
	return;
}

//�˳���ť
void CLoveSendClass::OnBnClickedQuitGame()
{
	OnCancel();
	return;
}
/*void CLoveSendClass::OnBnClickedMax()
{
::SendMessage(this->m_hWnd,WM_SYSCOMMAND,SC_MAXIMIZE,0);
}*/
void CLoveSendClass::OnBnClickedMin()
{
	this->ShowWindow(SW_MINIMIZE);
	//::SendMessage(this->m_hWnd,WM_SYSCOMMAND,SC_MINIMIZE,0);
}
void CLoveSendClass::OnBnClickedMax()
{
	m_bMax=true;
	m_bShowMax=true;
	ShowWindow(SW_RESTORE);
	CRect rect;
	::SystemParametersInfo(SPI_GETWORKAREA,NULL,&rect,NULL);
	MoveWindow(0,0,rect.Width(),rect.Height());
	m_btMax.ShowWindow(SW_HIDE);
	m_btRest.ShowWindow(SW_SHOW);
	SetForegroundWindow();
	this->Invalidate(FALSE);
	//::SendMessage(this->m_hWnd,WM_SYSCOMMAND,SC_MAXIMIZE,0);
}

void CLoveSendClass::OnBnClickedRest()
{
	m_bMax=false;
	m_bShowMax=false;
	MoveWindow(0,0,m_SavedScreen.x,m_SavedScreen.y);
	//MoveWindow(0,0,1000,700);
	CenterWindow();
	m_btMax.ShowWindow(SW_SHOW);
	m_btRest.ShowWindow(SW_HIDE);

	//::SendMessage(this->m_hWnd,WM_SYSCOMMAND,SC_RESTORE,0);
}
//������ť
void CLoveSendClass::OnBnClickedGameRoom()
{
	AfxGetMainWnd()->BringWindowToTop();
	AfxGetMainWnd()->SetForegroundWindow();
	AfxGetMainWnd()->SetFocus();
	return;
}

//������Ϣ
LRESULT CLoveSendClass::OnExpression(WPARAM wparam, LPARAM lparam)
{
	CTrueItem * pExpItem=m_ExpressWnd.GetExpressItem((UINT)wparam);
	if (pExpItem!=NULL)
	{
		CString strBuffer;
		m_InputMessage.GetWindowText(strBuffer);
		strBuffer+=pExpItem->m_szTrancelate;
		m_InputMessage.SetWindowText(strBuffer);
		m_InputMessage.SetFocus();
		m_InputMessage.SetEditSel(strBuffer.GetLength(),strBuffer.GetLength());
	}
	return 0;
}
	////////////////////////////
	///Kylin 20090122 ʹ�õ���
LRESULT CLoveSendClass::OnUserItem(WPARAM wparam, LPARAM lparam)
{
	for(int i=0; i<MAX_PEOPLE; i++)
	{
		///////////////////////////////////////////////////////////
		//////Kylin 20090121 ���ܸ������û�ʹ�õ���
		if(m_pUserInfo[i])
		{
			if(m_pUserInfo[i]->GameUserInfo.dwUserID == lparam )
			{
				if(m_pUserInfo[i]->GameUserInfo.bUserState==USER_CUT_GAME)
				{
					//AFCMessageBox("���ܶԶ����û�ʹ�øõ���","��ʾ",AFC_YESNO);
		            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"���ܶԶ����û�ʹ�øõ���");
				}
				else	int nRet = OnUseProp(wparam, m_pUserInfo[i]->GameUserInfo.dwUserID);
				break;
			}
		}
		///////////////////////////////////////////////////////////
	}
	return 0;
}
////////////////////////////

//������Ϣ
LRESULT CLoveSendClass::OnRichEditHit(WPARAM wparam, LPARAM lparam)
{
	if ((wparam!=NULL)&&((lparam==WM_LBUTTONDBLCLK)||(lparam==WM_LBUTTONDOWN)))
	{
		CString * pStrBuffer=(CString *)wparam;
		//int iSelect=m_TargetPeople.FindString(0,*pStrBuffer);
		//if (iSelect!=LB_ERR) m_TargetPeople.SetCurSel(iSelect);
		m_InputMessage.SetFocus();
	}
	return 0;
}
//�ر���Ϸ
LRESULT CLoveSendClass::OnCloseGame(WPARAM wparam, LPARAM lparam)
{
	//ȥ����ʾ
	if(m_DlgShowLeft.Showing)
	{
		m_DlgShowLeft.KillTimer(1);
		m_DlgShowLeft.ShowWindow(0);
		m_DlgShowLeft.Showing=0;
		m_DlgShowLeft.DestroyWindow();
	}
	//�ر�
	AFCCloseFrame();
	return 0;
}
//�������Ϣ
LRESULT CLoveSendClass::OnSplitMessage(WPARAM wparam, LPARAM lparam)
{
	if (wparam==IDC_HOR_SPLIT)	//�������Ϣ
	{
		//�ƶ������
		CRect ClientRect;
		GetClientRect(&ClientRect);
		//SafeMoveWindow(&m_HorSplitBar,ClientRect.right-209,HIWORD(lparam),204,SPLIT_WIDE);

		//�ƶ������ؼ�
		FixControlStation(ClientRect.Width(),ClientRect.Height());
	}

	return 0;
}

//�����
void CLoveSendClass::ActiveClient() 
{ 
	BringWindowToTop(); 
	SetForegroundWindow();
	return;
}

//���Ż��㵽��ͼλ��
BYTE CLoveSendClass::ViewStation(BYTE bDeskStation)
{
	BYTE bViewStation=(bDeskStation+m_pGameInfo->uDeskPeople-m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation);
	switch (m_pGameInfo->uDeskPeople)
	{
	case 1: { bViewStation+=1; break; }
	case 2: { bViewStation+=1; break; }
	case 3: { bViewStation+=1; break; }
	case 4: { bViewStation+=2; break; }
	case 5: { bViewStation+=2; break; }
	case 6: { bViewStation+=3; break; }
	case 7: { bViewStation+=3; break; }
	case 8: { bViewStation+=4; break; }
	}
	return bViewStation%m_pGameInfo->uDeskPeople;
}

//014���ͺ���
int CLoveSendClass::SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	
	if (m_pGameInfo->bIsInRecord)  //�������²��������ⷢ��Ϣadded by yjj 090223 for record 090223
	{
		return 0 ;
	}
	if (m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskNO==255) 
		return 0;
	return m_pGameInfo->pISocketSend->SendGameData(pData,uBufLen,bMainID,bAssistantID,bHandleCode);
}
//���ͺ���
int CLoveSendClass::SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	if (m_pGameInfo->bIsInRecord)  //�������²��������ⷢ��Ϣadded by yjj 090223 for record 090223
	{
		return 0 ;
	}
	//if (m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskNO==255)
	//	return 0;
	return m_pGameInfo->pISocketSend->SendGameData(bMainID,bAssistantID,bHandleCode);
}

//��ȡ��ֵ
UINT CLoveSendClass::GetProfileInt(TCHAR * szEntry, int iDefault)
{
	return AfxGetApp()->GetProfileInt(m_pGameInfo->szGameName,szEntry,iDefault);
}

///011��ȡ�ַ���
CString CLoveSendClass::GetProfileString(TCHAR * szEntry, TCHAR * szDefault)
{
	return AfxGetApp()->GetProfileString(m_pGameInfo->szGameName,szEntry,szDefault);
}

//д����ֵ
BOOL CLoveSendClass::WriteProfileInt(TCHAR * szEntry, int nValue)
{
	return AfxGetApp()->WriteProfileInt(m_pGameInfo->szGameName,szEntry,nValue);
}

//��ȡ�ַ���
BOOL CLoveSendClass::WriteProfileString(TCHAR * szEntry, TCHAR * szValue)
{
	return AfxGetApp()->WriteProfileString(m_pGameInfo->szGameName,szEntry,szValue);
}

//��������
void CLoveSendClass::PlayGameSound(TCHAR * szFileName)
{
	if ((m_pGameInfo->bEnableSound==true)&&(IsWindowVisible()==TRUE))
	{
		struct stat FileStat;
		if (stat(szFileName,&FileStat)==0) 
			PlaySound(szFileName,AfxGetInstanceHandle(),SND_ASYNC);
	}
	return;
}

//��������
void CLoveSendClass::PlayGameSound(HINSTANCE hInstance, TCHAR * szResourceName)
{
	if ((m_pGameInfo->bEnableSound==true)&&(IsWindowVisible()==TRUE))
	{
		/*AFC::*/AFCPlaySound(hInstance,szResourceName);
	}
	return;
}

//����û�����ֵ
bool CLoveSendClass::CheckUserPoint(int dwChangePoint)
{
	if ((m_pGameInfo->uLessPoint!=0L)&&(dwChangePoint<0L)&&(m_bWatchMode==false))
	{
		if ((GetMeUserInfo()->dwPoint+dwChangePoint)<(int)m_pGameInfo->uLessPoint) 
		{
			m_bSaveView=true;
			CString strBuffer;
			strBuffer.Format(TEXT("�����Ϸ�������� %ld����Ϸ�˳���"),m_pGameInfo->uLessPoint);
			SetStationParameter(20);

			//AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_ICONQUESTION,this);
		    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,m_pGameInfo->szGameName,false,strBuffer);

			m_bSaveView=false;
			AFCCloseFrame();
			return false;
		}
	}
	return true;
}

//���ö�ʱ��
void CLoveSendClass::SetGameTimer(BYTE bDeskStation, UINT uMaxTime, UINT uTimeID)
{
	if (m_uTimeID!=0L) KillGameTimer(m_uTimeID);
	if ((bDeskStation<m_pGameInfo->uDeskPeople)&&(uMaxTime>0L))
	{
		m_uTimeID=uTimeID;
		m_uTimeCount=uMaxTime;
		m_bTimeStation=bDeskStation;
		SetTimer(uTimeID,1000,NULL);
		m_pGameView->SetUserTimer(ViewStation(bDeskStation),m_uTimeCount);
	}
	return;
}

//ɾ����ʱ��
void CLoveSendClass::KillGameTimer(UINT uTimeID)
{
	//������ʱ��
	if (m_uTimeID==0L) return;
	if (uTimeID==0L) uTimeID=m_uTimeID;
	if (m_uTimeID==uTimeID)
	{
		//�������
		KillTimer(m_uTimeID);
		if (m_bTimeStation!=255)
			m_pGameView->SetUserTimer(ViewStation(m_bTimeStation),0);

		//��������
		m_uTimeID=0L;
		m_uTimeCount=0L;
		m_bTimeStation=255;
	}
	return;
}

//��ʱ����Ϣ
void CLoveSendClass::OnTimer(UINT nIDEvent)
{

	//if(nIDEvent==1)
	//{
	//	KillTimer(1);
	//	Invalidate(FALSE);
	//}
	switch(nIDEvent)//����
	{
	case 1:
		{
			KillTimer(1);
			int nDefaultIndex=m_InputMessage.GetCurSel();
			int nTalkCount=m_InputMessage.GetCount();
			if(nTalkCount>m_nTalkDefaultCount)
				nDefaultIndex-=nTalkCount-m_nTalkDefaultCount;
			if(nDefaultIndex>=0)
				SendTalkMessage(nDefaultIndex+1);
		}
		break;
	//case ID_CHECK_SIGNAL:
	//{
	//	checkNetSignal();
	//	break;
	//}
	case ID_GIF_TIMER:
		if(m_pGameView->m_pGif == NULL)
		{
			KillTimer(ID_GIF_TIMER);
			m_pGameView->UpdateViewFace(NULL);
			PlayNextGif();
		}
		else
		{
			m_pGameView->UpdateViewFace(NULL);
		}
		break;
	case ID_GIF_TIMER2:
		if(m_pGameView->m_pGif == NULL)
		{
			KillTimer(ID_GIF_TIMER2);
			m_pGameView->UpdateViewFace(NULL);
		}
		else
		{
			m_pGameView->UpdateViewFace(NULL);
		}
		break;
	}
	//ɾ����ʱ��
	if (nIDEvent==m_uTimeID)
	{
		if ((m_uTimeCount>0L)&&(m_bTimeStation!=255))
		{
			m_uTimeCount--;
			bool bHandle=OnGameTimer(m_bTimeStation,m_uTimeID,m_uTimeCount);
			if ((bHandle==false)||(m_uTimeCount==0))
				KillGameTimer(m_uTimeID);
			else m_pGameView->SetUserTimer(ViewStation(m_bTimeStation),m_uTimeCount);
		}
		else KillGameTimer(m_uTimeID);
	}
	__super::OnTimer(nIDEvent);
	return;
}


//�˵���Ϣ
BOOL CLoveSendClass::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_SAVE_MESSAGE:				//����������Ϣ
		{
			m_TalkMessage.OnSave();
			return TRUE;
		}
	case IDM_CLEAR_SCREEN:				//���������Ļ
		{
			m_TalkMessage.OnClearAll();
			return TRUE;
		}
	case IDM_AUTO_SCROLL:				//�Զ�������Ļ
		{
			m_TalkMessage.OnAutoScroll();
			return TRUE;
		}
	case IDM_SET_FRIEND:		//��Ӻ���
		{
			m_pGameRoom->PostMessage(WM_COMMAND,IDM_SET_FRIEND,m_dwCurrentUserID);
			return TRUE;
		}
		break;
	case IDM_GAME_RULE:					//��Ϸ����
		{
			CString strBuffer;
			ShellExecute(NULL,TEXT("open"),TEXT(Glb().m_CenterServerPara.m_strHelpADDR),NULL,NULL,SW_MAXIMIZE);
			return TRUE;
		}
	case IDM_TALK_COLOR:				//����������ɫ
		{
			CColorDialog Color(CGameImageLink::m_ColorResource.m_crTalkTxColor,CC_FULLOPEN,this);
			if (Color.DoModal()==IDOK) 
				CGameImageLink::m_ColorResource.m_crTalkTxColor=Color.GetColor();
			m_InputMessage.SetFocus();
			return TRUE;
		}
	case IDM_COPY_USER_NAME:			//�����û�����
		{
			if (m_dwCurrentUserID!=0L)
			{
				//��������
				UserItemStruct * pUserItem=m_pGameInfo->pIFindUser->FindOnLineUser(m_dwCurrentUserID);
				if ((pUserItem==NULL)||(OpenClipboard()==FALSE))
					return TRUE;
				if (EmptyClipboard()==FALSE) 
				{
					CloseClipboard();
					return TRUE;
				}

				//��������
				HANDLE hData=GlobalAlloc(GMEM_MOVEABLE,sizeof(pUserItem->GameUserInfo.nickName));
				if (hData==NULL) 
				{
					CloseClipboard();
					return TRUE;
				}

				LPTSTR szMemName=(LPTSTR)GlobalLock(hData);

				// duanxiaohui 2011-11-21 ���Ƶ�ճ��������Ҫת����Unicode
				//lstrcpy(szMemName,pUserItem->GameUserInfo.nickName);

				CString strText;
				strText.Format("%s", pUserItem->GameUserInfo.nickName);
				if (0 == MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strText, strText.GetLength() + 1, (WCHAR*)szMemName,  1024)) 
				{ 
					GlobalUnlock(hData); 
					CloseClipboard(); 
					GlobalFree(hData); 
					return TRUE; 
				} 
				// end duanxiaohui

				SetClipboardData(CF_UNICODETEXT, hData);
				GlobalUnlock(hData); 
				GlobalFree(hData);
				CloseClipboard();

				//��ʾ����
				CString strBuffer;
				m_InputMessage.GetWindowText(strBuffer);
				strBuffer+=pUserItem->GameUserInfo.nickName;
				m_InputMessage.SetWindowText(strBuffer);
				m_InputMessage.SetFocus();
			}
			return TRUE;
		}
	case IDM_GIVE_PROP:
		{
			::PostMessage(m_pGameRoom->GetSafeHwnd(),WM_USER+157,m_dwCurrentUserID,0);
			return TRUE;
		}
	case IDM_KICK_USER://����
		{
			if (m_dwCurrentUserID!=0L)
			{
				//wushuqun 2009.5.19
				//�Թ�ʱ����T��
				if (GetMeUserInfo()->bUserState == USER_WATCH_GAME)
				{
					//AFCMessageBox("�Բ���,�������Թ���Ϸ�����ܽ��д˲���","��ʾ");
		            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�Բ���,�������Թ���Ϸ�����ܽ��д˲���");
					return true;
				}

				if(GetMeUserInfo()->dwUserID == m_dwCurrentUserID)
				{
					//AFCMessageBox("�Բ���,�޷����Լ����д������","��ʾ");
		            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�Բ���,�޷����Լ����д������");
					return true;
				}
				CString stime;
				stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
				int curtime=atoi(stime);                        //ת��Ϊint
				if(GetMeUserInfo()->iVipTime < curtime)
				{
					//AFCMessageBox("�Բ���,ֻ��vip���д����","��ʾ");
		            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�Բ���,ֻ��vip���д����");
					return true;
				}
				//��������
				UserItemStruct * pUserItem=m_pGameInfo->pIFindUser->FindOnLineUser(m_dwCurrentUserID);
				if (pUserItem==NULL) 
					return TRUE;
				if(pUserItem->GameUserInfo.iVipTime>curtime)
				{
					//AFCMessageBox("�Բ���,�Է�Ҳ��Vip,����ʧ��!","��ʾ");
		            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�Բ���,�Է�Ҳ��Vip,����ʧ��!");
					return TRUE;
				}
				_TAG_KICKUSER stKick;
				stKick.dwUserID = GetMeUserInfo()->dwUserID;
				stKick.dwDestID = m_dwCurrentUserID;
				SendGameData(&stKick,sizeof(stKick),MDM_GR_PROP,ASS_PROP_KICKUSER,0);
			}
			return TRUE;
		}
	case IDM_SET_TALK_OBJECT:			//����̸������
		{
			if (m_dwCurrentUserID!=0L)
			{
				//��������
				UserItemStruct * pUserItem=m_pGameInfo->pIFindUser->FindOnLineUser(m_dwCurrentUserID);
				if (pUserItem==NULL) return TRUE;

				//�����������
				//if (pUserItem!=NULL)
				//{
				//	int iIndex=m_TargetPeople.FindString(1,pUserItem->GameUserInfo.nickName);
				//	if (iIndex!=CB_ERR) 
				//		m_TargetPeople.SetCurSel(iIndex);
				//}
			}
			m_InputMessage.SetFocus();
			return TRUE;
		}
	case IDM_CANCEL_TALK:				//ȡ��̸������
		{
			//m_TargetPeople.SetCurSel(0);
			m_InputMessage.SetFocus();
			return TRUE;
		}
	case IDM_JOIN_GAME:					//������Ϸ
		{
			//Ч������
			if ((m_bWatchMode==false)||(m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskNO==255)) 
				return TRUE;

			//��������

			return TRUE;
		}
	case IDM_EMABLE_WATCH:				//�����Թ�
	case IDM_UNEMABLE_WATCH:			//�������Թ�
		{
			if ((m_dwCurrentUserID!=0)&&(m_bWatchMode==false))
			{
				MSG_GM_WatchSet WatchSet;
				WatchSet.dwUserID=m_dwCurrentUserID;
				SendGameData(&WatchSet,sizeof(WatchSet),MDM_GM_GAME_FRAME,ASS_GM_WATCH_SET,LOWORD(wParam)==IDM_EMABLE_WATCH);
			}
			return TRUE;
		}
	case IDM_SHIELD_USER:				//�����û�
		{
			//Ч������
			if ((m_dwCurrentUserID==0L)||(m_dwCurrentUserID==GetMeUserInfo()->dwUserID)) 
				return TRUE;

			//��������
			INT_PTR i=0;
			for (i=0;i<m_ShieldArray.GetCount();i++)
			{
				if ((m_ShieldArray.GetAt(i)==0L)||(m_ShieldArray.GetAt(i)==m_dwCurrentUserID))
					break;
			}
			if (i==m_ShieldArray.GetCount()) 
				m_ShieldArray.SetAtGrow(i,m_dwCurrentUserID);

			//������Ϣ
			UserItemStruct * pUserItem=m_pGameInfo->pIFindUser->FindOnLineUser(m_dwCurrentUserID);
			if (pUserItem!=NULL)
			{
				CString strBuffer;
				
				strBuffer.Format(TEXT("������� [ %s ] �ɹ�"),pUserItem->GameUserInfo.nickName);
				m_MessageHandle.InsertSystemMessage(strBuffer.GetBuffer());
				strBuffer.ReleaseBuffer();
			}

			return TRUE;
		}
	case IDM_UNSHIELD_USER:				//��������û�
		{
			//Ч������
			if ((m_dwCurrentUserID==0L)||(m_dwCurrentUserID==GetMeUserInfo()->dwUserID)) 
				return TRUE;

			//��������
			for (INT_PTR i=0;i<m_ShieldArray.GetCount();i++)
			{
				if (m_ShieldArray.GetAt(i)==m_dwCurrentUserID) 
				{
					m_ShieldArray.RemoveAt(i);
					break;
				}
			}

			//������Ϣ
			UserItemStruct * pUserItem=m_pGameInfo->pIFindUser->FindOnLineUser(m_dwCurrentUserID);
			if (pUserItem!=NULL)
			{
				CString strBuffer;
				strBuffer.Format(TEXT("����������[%s]�ɹ�"),pUserItem->GameUserInfo.nickName);
				m_MessageHandle.InsertSystemMessage(strBuffer.GetBuffer());
				strBuffer.ReleaseBuffer();
			}

			return TRUE;
		}
	case IDM_RED:						
	case IDM_BULE:
	case IDM_GRAY:
	case IDM_GREEN:
	case IDM_WHITE:
	case IDM_YELLOW:
	case IDM_POWER_RED:					//����������ɫ
		{
			UINT uColorMenuID[7]={IDM_RED,IDM_YELLOW,IDM_BULE,IDM_GREEN,IDM_WHITE,IDM_POWER_RED,IDM_GRAY};
			COLORREF rcColor[8]={RGB(255,0,0),RGB(255,255,0),RGB(0,255,255),RGB(0,255,0),RGB(255,255,255),RGB(255,0,255),RGB(192,192,192),RGB(255,255,255)};
			UINT i=0;
			for (i=0;i<sizeof(uColorMenuID)/sizeof(uColorMenuID[0]);i++)
			{
				if (LOWORD(wParam)==uColorMenuID[i]) break;
			}
			CGameImageLink::m_ColorResource.m_crTalkTxColor=rcColor[i];
			m_InputMessage.SetFocus();
			return TRUE;
		}
	}
	return __super::OnCommand(wParam, lParam);
}


int CLoveSendClass::GetComType(void)
{
	return m_pGameInfo->uComType;
}

//��ʾ��󻯴��� 
void CLoveSendClass::ShowMaxSize()
{
//	CRect rect;
//	::SystemParametersInfo(SPI_GETWORKAREA,NULL,&rect,NULL);
//	MoveWindow(0,0,rect.Width(),rect.Height());
	return;
}

//��ʾ����
void CLoveSendClass::AFCShowFrame(int nShow)
{
	ShowWindow(nShow);
	return ;
}

LRESULT  CLoveSendClass::SendWindowMessage(UINT msg,WPARAM wParam,LPARAM lParam)
{
	//�����˵��ߺ󣬵��߸���
	if(msg==WM_USER+151)
	{
		OnSetPropImages();
		return 0;
	}
	//�ڵ������ʹ���˵���
	if(msg==WM_USER+152)
	{
		UsePropItem(wParam);
		return 0;
	}
	if (msg==WM_BRING_GAMEWND_TO_TOP)
	{
		BringWindowToTop();
		return 0;
	}

	return SendMessage(msg,wParam,lParam);
}

//������Ϸ������Ϣ
void CLoveSendClass::ShowMsgInGame(TCHAR * szMsg, BYTE bNewOrSys,UINT uSize, TCHAR * szFontName,TCHAR *Title)
{
	m_MessageHandle.InsertTitleMessage(szMsg,bNewOrSys,uSize,szFontName,Title);
}

//��������Ϣ
void CLoveSendClass::ShowMsgDudu(TCHAR * szSendName, TCHAR * szTargetName,TCHAR * szCharString, COLORREF crTextColor, bool bShowTime, UINT uSize, TCHAR * szFontName,int iDuduType)
{
	m_MessageHandle.InsertDudu(szSendName,szTargetName,szCharString,crTextColor,false,0,0,iDuduType);
}

//����������Ϣ
void CLoveSendClass::SendShortMessage(MSG_GR_RS_NormalTalk *pShortMessage)
{
	//��������
	UserItemStruct * pSendUserItem=m_pGameInfo->pIFindUser->FindOnLineUser(pShortMessage->dwSendID);
	UserItemStruct * pTargerUserItem=m_pGameInfo->pIFindUser->FindOnLineUser(pShortMessage->dwTargetID);	//��ʾ��Ϣ
	CShortMessage * pShortMessageWnd=NULL;
	if (pShortMessage->dwSendID==GetMeUserInfo()->dwUserID)
	{
		//�Լ����͵���Ϣ
		pShortMessageWnd=ActiveShortWnd(pShortMessage->dwTargetID,pTargerUserItem,true);
		if (pShortMessageWnd!=NULL) 
			pShortMessageWnd->OnRecvMessage(pSendUserItem->GameUserInfo.nickName,pShortMessage->szMessage,pShortMessage->crColor);
	}
	else	//������Ϸ�߷�����Ϣ	
	{
		//if (m_bShowMessage==TRUE)
		//{
			pShortMessageWnd=ActiveShortWnd(pShortMessage->dwSendID,pSendUserItem,true);
			if (pShortMessageWnd!=NULL)
				pShortMessageWnd->OnRecvMessage(pSendUserItem->GameUserInfo.nickName,pShortMessage->szMessage,pShortMessage->crColor);
			//else m_pRightWnd->m_MessageHandle.InsertUserTalk(pSendUserItem->GameUserInfo.nickName,m_pGameInfo->pMeUserInfo->GameUserInfo.nickName,
			//	pNormalTalk->szMessage,pNormalTalk->crColor,false);
		//}
		//else m_pRightWnd->m_MessageHandle.InsertUserTalk(pSendUserItem->GameUserInfo.nickName,m_GameInfo.pMeUserInfo->GameUserInfo.nickName,
		//	pNormalTalk->szMessage,pNormalTalk->crColor,false);
	}

}

//������Ϣ����
CShortMessage * CLoveSendClass::ActiveShortWnd(long int dwUserID, UserItemStruct * pUserItem, bool bCreate)
{
	//��������
	INT_PTR iCloseIndex=-1,iHideIndex=-1;

	//Ѱ�����ϴ���
	for (INT_PTR i=0;i<m_ShortMessage.GetCount();i++)
	{
		CShortMessage * pShortMessageWnd=(CShortMessage *)m_ShortMessage.GetAt(i);
		ASSERT(pShortMessageWnd!=NULL);
		if(pShortMessageWnd == NULL)
			return NULL;
		if (dwUserID==pShortMessageWnd->m_dwTargetUserID)
		{
			if (pShortMessageWnd->GetSafeHwnd()==NULL)
			{
				AfxSetResourceHandle(GetModuleHandle(NULL));
				pShortMessageWnd->Create(IDD_SHORT_MESSAGE,this);
			}
			pShortMessageWnd->ShowWindow(SW_SHOWNOACTIVATE);
			return pShortMessageWnd;
		}
		if (bCreate==true)
		{
			if ((iCloseIndex==-1)&&(pShortMessageWnd->GetSafeHwnd()==NULL))
				iCloseIndex=i;
			if ((iHideIndex==-1)&&(pShortMessageWnd->GetSafeHwnd()!=NULL)&&(pShortMessageWnd->IsWindowVisible()==FALSE)) iHideIndex=i;
		}
	}

	//ʹ�ùرյĴ���
	if ((bCreate==true)&&(iCloseIndex>=0))
	{
		CShortMessage * pShortMessageWnd=(CShortMessage *)m_ShortMessage.GetAt(iCloseIndex);
		ASSERT(pShortMessageWnd!=NULL);
		if (pShortMessageWnd->GetSafeHwnd()==NULL)
		{
			AfxSetResourceHandle(GetModuleHandle(NULL));
			pShortMessageWnd->Create(IDD_SHORT_MESSAGE,this);
		}
		SendMsgUserStruct user;
		pShortMessageWnd->TransUserInfo(pUserItem, &user);
		pShortMessageWnd->SetTalkInfo(&user,m_pGameInfo->dwGamePower,m_szGameRoomName);
		pShortMessageWnd->ShowWindow(SW_SHOWNOACTIVATE);
		return pShortMessageWnd;
	}

	//ʹ�����ش���
	if ((bCreate==true)&&(iHideIndex>=0))
	{
		CShortMessage * pShortMessageWnd=(CShortMessage *)m_ShortMessage.GetAt(iHideIndex);
		ASSERT(pShortMessageWnd!=NULL);
		SendMsgUserStruct user;
		pShortMessageWnd->TransUserInfo(pUserItem, &user);
		pShortMessageWnd->SetTalkInfo(&user,m_pGameInfo->dwGamePower,m_szGameRoomName);
		pShortMessageWnd->ShowWindow(SW_SHOWNOACTIVATE);
		return pShortMessageWnd;
	}

	//�����´���
	if ((bCreate==true)&&(m_ShortMessage.GetCount()<m_iMaxShortWnd))
	{
		try
		{
			CShortMessage * pShortMessageWnd=new CShortMessage(this);
			AfxSetResourceHandle(GetModuleHandle(NULL));
			pShortMessageWnd->m_bIsInRoom = false;
			SendMsgUserStruct user;
			pShortMessageWnd->TransUserInfo(pUserItem, &user);
			pShortMessageWnd->SetTalkInfo(&user,m_pGameInfo->dwGamePower,m_szGameRoomName);
			pShortMessageWnd->Create(IDD_SHORT_MESSAGE,this);
			pShortMessageWnd->ShowWindow(SW_SHOWNOACTIVATE);
			m_ShortMessage.Add(pShortMessageWnd);
			return pShortMessageWnd;
		}
		catch (...) { return NULL; }
	}

	return NULL;
}
//����
void CLoveSendClass::LoadSkin(const CString& strFileName,const CString& strSkinKey,const CString& strSkinPath)
{
	int r,g,b;
	CString s=CBcfFile::GetAppPath ();/////����·��
	if (0 != strSkinKey.Compare(_T("")))
		m_iniFile="image\\client\\" + strSkinKey + "\\game\\gameskini.bcf";
	else
		m_iniFile="image\\client\\" + Glb().m_skin + "\\game\\gameskini.bcf";

	CBcfFile f( s + m_iniFile);

	CString key=TEXT("config");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if (0 != strSkinPath.Compare(_T("")))
		skinfolder=f.GetKeyVal(key,"skinfolder",strSkinPath + "game\\");
	else
		skinfolder=f.GetKeyVal(key,"skinfolder","image\\client\\skin0\\game\\");
	skinfolder=s+"\\"+skinfolder;

	CString ph;
	//����ͼ
	if (0 != strSkinPath.Compare(_T("")))
		ph=f.GetKeyVal(key,"backimages",strSkinPath + "game\\backimages.bmp");
	else
		ph=f.GetKeyVal(key,"backimages","image\\client\\skin0\\game\\backimages.bmp");

	m_pGameView->m_bkimgs.SetLoadInfo(ph.GetBuffer(),true);
	m_backImage.SetLoadInfo(ph.GetBuffer(),true);
	//�����ĸ���
	ZeroMemory(path,sizeof(path));
	wsprintf(path,"%s\\game_tc.bmp",skinfolder);
	m_tc.SetLoadInfo(path,true);//CGameImageLink::m_bAutoLock);
	wsprintf(path,"%s\\game_bc.bmp",skinfolder);
	m_bc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%s\\game_rc.bmp",skinfolder);
	m_rc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%s\\game_lc.bmp",skinfolder);
	m_lc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//�Ľ�
	wsprintf(path,"%s\\game_lt.bmp",skinfolder);
	m_lt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%s\\game_lb.bmp",skinfolder);
	m_lb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%s\\game_rt.bmp",skinfolder);
	m_rt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%s\\game_rb.bmp",skinfolder);
	m_rb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//�����ұ�
	wsprintf(path,"%s\\game_right_tc.bmp",skinfolder);
	m_right_tc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%s\\game_right_bc.bmp",skinfolder);
	m_right_bc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%s\\game_right_lc.bmp",skinfolder);
	m_right_lc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%s\\game_right_rc.bmp",skinfolder);
	m_right_rc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//�ĸ���
	wsprintf(path,"%s\\game_right_lt.bmp",skinfolder);
	m_right_lt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%s\\game_right_lb.bmp",skinfolder);
	m_right_lb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%s\\game_right_rt.bmp",skinfolder);
	m_right_rt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%s\\game_right_rb.bmp",skinfolder);
	m_right_rb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//�м�
//	wsprintf(path,"%s\\mid_c.bmp",skinfolder);
//	m_iMid.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//
	if (0 != strSkinPath.Compare(_T("")))
		ph=f.GetKeyVal(key,"backimageh",strSkinPath + "game\\backimageh.bmp");
	else
		ph=f.GetKeyVal(key,"backimageh","image\\client\\skin0\\game\\backimageh.bmp");
	m_pGameView->m_bkimgh.SetLoadInfo(ph.GetBuffer(),true);
	m_pGameView->m_ShowType=f.GetKeyVal(key,"showtype",0);
	m_pGameView->m_bShowRight=f.GetKeyVal(key,"showright",0);

	m_titlex=f.GetKeyVal(key,"titlex",12);
	m_titley=f.GetKeyVal(key,"titley",7);
	r=f.GetKeyVal(key,"titler",255);
	g=f.GetKeyVal(key,"titleg",234);
	b=f.GetKeyVal(key,"titleb",0);
	m_titlecr=RGB(r,g,b);
	m_logx=f.GetKeyVal(key,"flogx",198);
	m_logy=f.GetKeyVal(key,"flogy",40);
	m_u1x=f.GetKeyVal(key,"u1x",110);
	m_u1y=f.GetKeyVal(key,"u1y",64);
	m_u1w=f.GetKeyVal(key,"u1w",75);
	m_u1h=f.GetKeyVal(key,"u1h",18);
	m_u2x=f.GetKeyVal(key,"u2x",189);
	m_u2y=f.GetKeyVal(key,"u2y",158);
	m_u2w=f.GetKeyVal(key,"u2w",86);
	m_u2h=f.GetKeyVal(key,"u2h",18);
	m_vlistx=f.GetKeyVal(key,"vlistx",0);
	m_pGameView->m_vlistx=m_vlistx;
	m_vlisty=f.GetKeyVal(key,"vlisty",0);

	r=f.GetKeyVal(key,"userr",0);
	g=f.GetKeyVal(key,"userg",0);
	b=f.GetKeyVal(key,"userbkb",0);
	m_usercr=RGB(r,g,b);
	/////////////////////////////
	//Kylin 20090205 �����û���Ϣ����
	r=f.GetKeyVal(key,"userbkr",0);
	g=f.GetKeyVal(key,"userbkg",0);
	b=f.GetKeyVal(key,"userbkb",0);
	m_userbkcr=RGB(r,g,b);
	/////////////////////////////

	r=f.GetKeyVal(key,"bkr",45);
	g=f.GetKeyVal(key,"bkg",80);
	b=f.GetKeyVal(key,"bkb",133);
	m_colorresource.m_crTalkBkColor=RGB(r,g,b);
	r=f.GetKeyVal(key,"namer",255);
	g=f.GetKeyVal(key,"nameg",255);
	b=f.GetKeyVal(key,"nameb",255);
	m_colorresource.m_crTalkNameColor=RGB(r,g,b);
	r=f.GetKeyVal(key,"systemr",255);
	g=f.GetKeyVal(key,"systemg",255);
	b=f.GetKeyVal(key,"systemb",255);
	m_colorresource.m_crSystemTxColor=RGB(r,g,b);
	r=f.GetKeyVal(key,"systembdr",255);
	g=f.GetKeyVal(key,"systembdg",0);
	b=f.GetKeyVal(key,"systembdb",0);
	m_colorresource.m_crSystemBdColor=RGB(r,g,b);
	r=f.GetKeyVal(key,"normalr",255);
	g=f.GetKeyVal(key,"normalg",255);
	b=f.GetKeyVal(key,"normalb",255);
	m_colorresource.m_crNormalTxColor=RGB(r,g,b);
	////////////////////////////////////
	////Kylin 20090117 ������Ϣ��ɫ����
	r=f.GetKeyVal(key,"talkr",255);
	g=f.GetKeyVal(key,"talkg",255);
	b=f.GetKeyVal(key,"talkb",255);
	m_colorresource.m_crTalkTxColor=RGB(r,g,b);	
	////////////////////////////////////
	//�����ɫ
	r=f.GetKeyVal(key,"framecolorr",150);
	g=f.GetKeyVal(key,"framecolorg",191);
	b=f.GetKeyVal(key,"framecolorb",239);
	m_FrameColor = RGB(r,g,b);		
	//����
	r=f.GetKeyVal(key,"newscolorr",255);
	g=f.GetKeyVal(key,"newscolorg",255);
	b=f.GetKeyVal(key,"newscolorb",255);
	m_colorresource.m_crNewsTxColor=RGB(r,g,b);		

	//m_colorresource.m_crNormalTxColor=RGB(r,g,b);
	SetFillColor(m_colorresource.m_crTalkBkColor);	
	m_MessageHandle.SetColor(m_colorresource);

	CGameImage img;
	HRGN hRgn;
	int w = 18;
	int h = 18;
	wsprintf(path,"%sgame_left_face.bmp",skinfolder);
	m_BtExpression.LoadButtonBitmap(path,false);
	img.SetLoadInfo(path,true);
	hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
	if(m_BtExpression)
	{
		if(hRgn)
			m_BtExpression.SetWindowRgn(hRgn,true);
		m_BtExpression.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
	}

	//������ɫ
	wsprintf(path,"%sgame_setcolor.bmp",skinfolder);
	m_BtSetColor.LoadButtonBitmap(path,false);
	img.SetLoadInfo(path,true);
	hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
	if(m_BtExpression)
	{
		if(hRgn)
			m_BtSetColor.SetWindowRgn(hRgn,true);
		m_BtSetColor.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
	}


	wsprintf(path,"%sgame_left_send.bmp",skinfolder);
	m_BtSend.LoadButtonBitmap(path,false);
	img.SetLoadInfo(path,true);
	if(m_BtSend.m_hWnd)
	{
		m_BtSend.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_BtSend.SetWindowRgn(hRgn,true);
	}
	//��ָ���,δ��
	//wsprintf(path,"%sgame_left_hsplit.bmp",skinfolder);
	//m_HorSplitBar.SetbkImage(path,path,path);

	wsprintf(path,"%shide_bt.bmp",skinfolder);
	m_BtHide.LoadButtonBitmap(path,false);	
	img.SetLoadInfo(path,true);
	if(m_BtHide.m_hWnd)
	{
		m_BtHide.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_BtHide.SetWindowRgn(hRgn,true);
	}
	m_HidebtPath = path;
	wsprintf(path,"%sshow_bt.bmp",skinfolder);
	m_ShowbtPath = path;


	w = 20;
	h = 20;
	wsprintf(path,"%sgame_min_bt.bmp",skinfolder);
	m_BtMin.LoadButtonBitmap(path,false);	
	wsprintf(path,"%sgame_max_bt.bmp",skinfolder);
	m_btMax.LoadButtonBitmap(path,false);	
	wsprintf(path,"%sgame_rest_bt.bmp",skinfolder);
	m_btRest.LoadButtonBitmap(path,false);	

	m_btRest.ShowWindow(SW_HIDE);
	m_btMax.ShowWindow(SW_SHOW);

	//img.SetLoadInfo(path,true);
	//m_BtMin.SetWindowPos(NULL,0,0,w,h,SWP_NOZORDER|SWP_NOMOVE);
	//hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
	//if(hRgn)
	//	m_BtMin.SetWindowRgn(hRgn,true);

	wsprintf(path,"%slog_exit_bt.bmp",skinfolder);//game_close_bt.bmp
	m_BtGameQuit.LoadButtonBitmap(path,false);
	//img.SetLoadInfo(path,true);
	//m_BtGameQuit.SetWindowPos(NULL,0,0,w,h,SWP_NOZORDER|SWP_NOMOVE);
	//hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
	//if(hRgn)
	//	m_BtGameQuit.SetWindowRgn(hRgn,true);

	wsprintf(path,"%sgame_bt01.bmp",skinfolder);
	m_Btbt1.LoadButtonBitmap(path,false);
	img.SetLoadInfo(path,true);
	//m_Btbt1.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
	if(m_Btbt1.m_hWnd)
	{
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_Btbt1.SetWindowRgn(hRgn,true);
	}

	wsprintf(path,"%sgame_bt02.bmp",skinfolder);
	m_Btbt2.LoadButtonBitmap(path,false);
	img.SetLoadInfo(path,true);
	//m_Btbt2.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
	if(m_Btbt2.m_hWnd)
	{
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_Btbt2.SetWindowRgn(hRgn,true);
	}

	wsprintf(path,"%sgame_bt03.bmp",skinfolder);
	m_Btbt3.LoadButtonBitmap(path,false);
	img.SetLoadInfo(path,true);
	//m_Btbt3.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
	if(m_Btbt3.m_hWnd)
	{
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_Btbt3.SetWindowRgn(hRgn,true);
	}

	wsprintf(path,"%sgame_bt04.bmp",skinfolder);
	m_Btbt4.LoadButtonBitmap(path,false);
	img.SetLoadInfo(path,true);
	//m_Btbt4.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
	if(m_Btbt4.m_hWnd)
	{
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_Btbt4.SetWindowRgn(hRgn,true);
	}

	wsprintf(path,"%s\\SndPlay.bmp",skinfolder);
	m_btSndPlay.LoadButtonBitmap(path,false);

	CGameImage sndImg;
	sndImg.SetLoadInfo(path,true);
	m_sndBtnCx=sndImg.GetWidth()/4;
	m_sndBtnCy=sndImg.GetHeight();

	wsprintf(path,"%s\\SndPause.bmp",skinfolder);
	m_btSndPause.LoadButtonBitmap(path,false);

	wsprintf(path,"%s\\SndVolUp.bmp",skinfolder);
	m_btVolUp.LoadButtonBitmap(path,false);

	wsprintf(path,"%s\\SndVolDown.bmp",skinfolder);
	m_btVolDown.LoadButtonBitmap(path,false);


	sndPlayX=f.GetKeyVal(key,"SndPlayX",100);
	sndPlayY=f.GetKeyVal(key,"SndPlayY",-200);

	sndPauseX=f.GetKeyVal(key,"SndPauseX",100);
	sndPauseY=f.GetKeyVal(key,"SndPauseY",-200);

	sndVolDownX=f.GetKeyVal(key,"SndVolDownX",100);
	sndVolDownY=f.GetKeyVal(key,"SndVolDownY",-200);

	sndVolUpX=f.GetKeyVal(key,"SndVolUpX",100);
	sndVolUpY=f.GetKeyVal(key,"SndVolUpY",-200);


	if(m_Btbt5.m_hWnd)m_Btbt5.ShowWindow(SW_HIDE);
	if(m_Btbt6.m_hWnd)m_Btbt6.ShowWindow(SW_HIDE);
	if(m_Btbt7.m_hWnd)m_Btbt7.ShowWindow(SW_HIDE);
	if(m_Btbt8.m_hWnd)m_Btbt8.ShowWindow(SW_HIDE);
	if(m_Btbt9.m_hWnd)m_Btbt9.ShowWindow(SW_HIDE);
	if(m_Btbt10.m_hWnd)m_Btbt10.ShowWindow(SW_HIDE);

	m_MessageHandle.SetColor(m_colorresource);


	wchar_t wpath[MAX_PATH];
	swprintf(wpath,L"image\\client\\skin0\\game_signal.bmp");
	
#ifndef BZ_ZLIB
	pSingnalimg = Image::FromFile(wpath);
#else
	TCHAR szpath[MAX_PATH];
	sprintf(szpath,"image\\client\\skin0\\game_signal.bmp");
	CUnZipRes unZipRes;
	IStream * pStream=NULL;
	pStream=unZipRes.LoadFileResFromZip(szpath);
	pSingnalimg=new Image(pStream);
	if(pStream!=NULL)
	{
		pStream->Release();
		pStream=NULL;
	}
#endif
	
	//�ȴ�����ͼ yjj 090227
}

LRESULT CLoveSendClass::OnLoadSkin(WPARAM wparam, LPARAM lparam)
{//�˺�����ʱ��������
	LoadSkin();
	m_UserListDlg.m_UserList.LoadSkin();

	CRect ClientRect;
	GetClientRect(&ClientRect);
	int cx,cy;
	cx=ClientRect.Width();
	cy=ClientRect.Height();
	OnSize(0,cx,cy);
	return true;
}

void CLoveSendClass::StopBackMusic()
{
	m_PlaySound.StopMusic();
}

void CLoveSendClass::ContinueMusic()
{
	m_PlaySound.PlayMusic(NULL,NULL,NULL,true);
}

// ���ñ�������
void CLoveSendClass::SetBackMusic(LPCTSTR filepath,LPCTSTR key)
{
	if(!filepath)
		return ;
	CString s=CBcfFile::GetAppPath ();//����·��
	s += filepath;
	lstrcpy(m_BackMusicIni, s.GetBuffer());
	lstrcpy(m_BMusicKey,key);
}

//���ͻ���
LRESULT CLoveSendClass::OnGetPoint(WPARAM wparam,LPARAM lparam)
{
	TCHAR url[300];
	wsprintf(url,"%sapp/huodong.asp",Glb().m_CenterServerPara.m_strWebRootADDR);
	//wsprintf(url,"%schongzhi.%s?roomid=%d&un=%s",Glb().m_CenterServerPara.m_strHomeADDR,Glb().m_urlpostfix,m_pGameInfo->uRoomID,m_pGameInfo->pMeUserInfo->GameUserInfo.szName);
	ShellExecute(NULL,"open",url,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
	return 0;
	//	if((int)lparam != 0)
	//		return 0;
	MSG_GR_S_GetPoint getpoint;
	::memset(&getpoint,0,sizeof(getpoint));
	getpoint.dwPoint = (int)wparam;
	SendGameData(&getpoint, sizeof(getpoint), MDM_GR_POINT, ASS_GR_GET_POINT, 0);
	return 0;
}
int CLoveSendClass::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CLoadFileBmp::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}


BOOL CLoveSendClass::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	cs.style &= ~WS_VISIBLE;
	cs.style |= WS_CLIPSIBLINGS;
	cs.style |= WS_CLIPCHILDREN;

	return __super::PreCreateWindow(cs);
}
//��Ϸ����
void CLoveSendClass::OnBnClkGameBt01()
{
	OnGameSetting();
	return ;
}

//������� 
void CLoveSendClass::OnBnClkGameBt02()
{
	::PostMessage(m_pGameRoom->GetSafeHwnd(),WM_USER+150,0,0);
	//OnGetBuyItemData(0,0);
	return ;
}
//��ֵ 
void CLoveSendClass::OnBnClkGameBt03()
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	CString url;
	TCHAR szUrl[20];
	url=f.GetKeyVal("BZW","gameurl1",Glb().m_CenterServerPara.m_strHomeADDR);
	
	CString loginURL;//JMod-�û���¼ҳ�� -20090506
	loginURL = f.GetKeyVal("BZW", "UserLoginURL", "userlogin.aspx"); 

	CString surl;
	surl.Format("%s/app/%s?userid=%d&token=%s&url=%s",
		Glb().m_CenterServerPara.m_strWebRootADDR,loginURL,m_pGameInfo->pMeUserInfo->GameUserInfo.dwUserID,Glb().m_TML_SN,url);

	ShellExecute(NULL,"open",surl,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
	return ;

	//SendGameData(MDM_GR_POINT, ASS_GR_OPEN_POINT, 0);
}

//��ҳ
void CLoveSendClass::OnBnClkGameBt04()
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	CString url;
	TCHAR szUrl[20];
	url=f.GetKeyVal("BZW","gameurl2",Glb().m_CenterServerPara.m_strHomeADDR);

	CString loginURL;//JMod-�û���¼ҳ�� -20090506
	loginURL = f.GetKeyVal("BZW", "UserLoginURL", "userlogin.aspx"); 

	CString surl;
	surl.Format("%s/app/%s?userid=%d&token=%s&url=%s",
		Glb().m_CenterServerPara.m_strWebRootADDR,loginURL,m_pGameInfo->pMeUserInfo->GameUserInfo.dwUserID,Glb().m_TML_SN,url);

	ShellExecute(NULL,"open",surl,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
}

void CLoveSendClass::OnBnClkGameBt05()
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	CString url;
	TCHAR szUrl[20];
	url=f.GetKeyVal("BZW","gameurl3",Glb().m_CenterServerPara.m_strHomeADDR);

	CString loginURL;//JMod-�û���¼ҳ�� -20090506
	loginURL = f.GetKeyVal("BZW", "UserLoginURL", "userlogin.aspx"); 

	CString surl;
	surl.Format("%s/app/%s?userid=%d&token=%s&url=%s",
		Glb().m_CenterServerPara.m_strWebRootADDR,loginURL,m_pGameInfo->pMeUserInfo->GameUserInfo.dwUserID,Glb().m_TML_SN,url);

	ShellExecute(NULL,"open",surl,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
}

void CLoveSendClass::OnBnClkGameBt06()
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	CString url;
	TCHAR szUrl[20];
	url=f.GetKeyVal("BZW","gameurl4",Glb().m_CenterServerPara.m_strHomeADDR);

	CString loginURL;//JMod-�û���¼ҳ�� -20090506
	loginURL = f.GetKeyVal("BZW", "UserLoginURL", "userlogin.aspx"); 

	CString surl;
	surl.Format("%s/app/%s?userid=%d&token=%s&url=%s",
		Glb().m_CenterServerPara.m_strWebRootADDR,loginURL,m_pGameInfo->pMeUserInfo->GameUserInfo.dwUserID,Glb().m_TML_SN,url);

	ShellExecute(NULL,"open",surl,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
}

void CLoveSendClass::OnBnClkGameBt07()
{

}
void CLoveSendClass::OnBnClkGameBt08()
{

}
void CLoveSendClass::OnBnClkGameBt09()
{

}
void CLoveSendClass::OnBnClkGameBt10()
{

}

//{add by zxj 2008/04/07
void CLoveSendClass::OnBnClkPropGoAhead()
{
	//if(m_curPropPage>0)
	//	m_curPropPage--;
	//else
	//	m_curPropPage=0;
	//if(m_curPropPage>0)
	//	m_BtPropGoAhead.EnableWindow(TRUE);
	//else
	//	m_BtPropGoAhead.EnableWindow(FALSE);
	m_curPropPage--;
	OnSetPropImages();
	//if(Glb().userPropLibrary.GetCount()>(m_curPropPage+1)*PEER_PAGE)
	//	m_BtPropGoNext.EnableWindow(TRUE);
	//else
	//	m_BtPropGoNext.EnableWindow(FALSE);
}

void CLoveSendClass::OnBnClkPropGoNext()
{
	m_curPropPage++;
	OnSetPropImages();
	//if(Glb().userPropLibrary.GetCount()>(m_curPropPage+1)*PEER_PAGE)
	//	m_curPropPage++;
	//if(Glb().userPropLibrary.GetCount()>(m_curPropPage+1)*PEER_PAGE)
	//	m_BtPropGoNext.EnableWindow(TRUE);
	//else
	//	m_BtPropGoNext.EnableWindow(FALSE);

	//if(m_curPropPage>0)
	//	m_BtPropGoAhead.EnableWindow(TRUE);
	//else
	//	m_BtPropGoAhead.EnableWindow(FALSE);

	//���һҳ��,����Ϸ�ҳ��ť������Ч��,����Ҫ������Ч
	//if( !m_BtPropGoAhead.IsWindowEnabled())
	//	m_BtPropGoAhead.EnableWindow(TRUE);
}
void CLoveSendClass::OnBnClkProp1()
{
	UsePropItem(m_curPropPage*PEER_PAGE);
	return;

}
void CLoveSendClass::OnBnClkProp2()
{
	UsePropItem(m_curPropPage*PEER_PAGE+1);
	return;
}
void CLoveSendClass::OnBnClkProp3()
{
	UsePropItem(m_curPropPage*PEER_PAGE+2);
	return;

}
void CLoveSendClass::OnBnClkProp4()
{
	UsePropItem(m_curPropPage*PEER_PAGE+3);
	return;

}
//end add}


//�򿪸��˵�����
//LRESULT CLoveSendClass::OnGetHoldItemData(WPARAM wparam,LPARAM lparam)
//{
//	tagUpdateItemCountResultMsgData stGetHoldItem;
//	::memset(&stGetHoldItem,0,sizeof(stGetHoldItem));
//	stGetHoldItem.iItemID = -1;
//	SendGameData(&stGetHoldItem,sizeof(stGetHoldItem),MDM_GR_ITEM_DATA_MSG,ASS_C_GET_ITEM_DATA,0);
//	return true;
//}
////ʹ�õ���
//LRESULT CLoveSendClass::OnUserUseItemData(WPARAM wparam,LPARAM lparam)
//{
//	tagUserUseItem stUseItem;
//	::memset(&stUseItem,0,sizeof(stUseItem));
//	stUseItem.iItemID = (int)wparam;
//	stUseItem.dwUserID = GetMeUserInfo()->dwUserID;
//	stUseItem.dwDestUserID = GetIDByNickName((LPSTR)lparam);	//��������ǳ�
//	if(stUseItem.iItemID != ITEM_FLOWER && stUseItem.iItemID != ITEM_BLOCK)//�ж��Ƿ�Ϊ�͸��Լ�
//	{
//		if(stUseItem.dwUserID  != stUseItem.dwDestUserID)
//		{
//			MessageBox("�˵��߲����Զ�����ʹ��!","��ʾ",MB_OK);
//			return false;
//		}
//	}
//	SendGameData(&stUseItem,sizeof(stUseItem),MDM_GR_ITEM_DATA_MSG,ASS_C_USE_ITEM,0);
//	return true;
//}
////��ȡ�ܹ���ĵ����б�
//LRESULT CLoveSendClass::OnGetBuyItemData(WPARAM wparam,LPARAM lparam)
//{
//	SendGameData(MDM_GR_ITEM_DATA_MSG,ASS_C_GET_BUY_ITEM_DATA,0);
//	return true;
//}
////�������
//LRESULT CLoveSendClass::OnBuyItemData(WPARAM wparam,LPARAM lparam)
//{
//	//������Ƿ��㹻
//	tagBuyItemData stBuyItem;
//	::memset(&stBuyItem,0,sizeof(stBuyItem));
//	CopyMemory(&stBuyItem,(tagBuyItemData*)wparam,sizeof(stBuyItem));
//	//������id
//	stBuyItem.dwUserID = GetMeUserInfo()->dwUserID;
//	//�����
//	//stBuyItem.iPayMoney = GetMoneyCount();
//	DWORD dwBank=GetMeUserInfo()->dwBank;
//	if(stBuyItem.iPayMoney > dwBank)// ->dwMoney)
//	{
//		MessageBox("�Բ��������еĽ�Ҳ���","��ʾ");
//		return true;
//	}
////	stBuyItem.dwUserID = GetMeUerInfo()->dwUserID;
//	SendGameData(&stBuyItem,sizeof(stBuyItem),MDM_GR_ITEM_DATA_MSG,ASS_C_BUY_ITEM_DATA,0);
//	//{add by zxj 2008/04/09
//	for( int i=0; i<PLATFORM_ITEM_KIND; i++)
//	{
//		if(stBuyItem.stItemData.aItemCount[i]!=0)
//			OnUpdateUserPropOnLine(i, stBuyItem.stItemData.aItemCount[i]);
//	}
//	//end add}
//	return true;
//}


//��ǰ˭��Ŀ������
BYTE CLoveSendClass::GetDestStation(TCHAR szUserName[])
{
	for(int i = 0; i < MAX_PEOPLE; i ++)
	{
		if(m_pUserInfo[i] == NULL)
			continue;
		if(0 == lstrcmp(szUserName,m_pUserInfo[i]->GameUserInfo.nickName))
			return i;
	}
	return 0;
}

//������ǰ�ǳƻ�ȡ���ID��
long int CLoveSendClass::GetIDByNickName(TCHAR *pszNickName)
{
	for(int i = 0; i < MAX_PEOPLE; i ++)
	{
		if(m_pUserInfo[i] == NULL)
			continue;
		if(0 == lstrcmp(pszNickName,m_pUserInfo[i]->GameUserInfo.nickName))
			return m_pUserInfo[i]->GameUserInfo.dwUserID;
	}
	return GetMeUserInfo()->dwUserID;
}


bool CLoveSendClass::UserProp(_TAG_PROP_MOIVE_USE *propMoive)
{
	int attribAction=propMoive->nPropActionAttrib;
	int attribValue=propMoive->nPropValueAttrib;

	if(attribAction & 0x40)
	{
		int iValue=(attribValue & 0x7F000000)>>24;
		m_stProp.iValue=iValue;
		m_stProp.sendDeskStation=propMoive->nUserStation;
		m_stProp.recvDeskStation=propMoive->nTargetUserStation;
		m_pGameView->SetGif(ViewStation(propMoive->nUserStation),iValue,2000);
		SetTimer(ID_GIF_TIMER,150,NULL);	
	}
	return true;
}

//������һ��gif����
bool CLoveSendClass::PlayNextGif()
{
	m_pGameView->SetGif(ViewStation(m_stProp.recvDeskStation),m_stProp.iValue + 1000,2000);
	SetTimer(ID_GIF_TIMER2,150,NULL);
	return true;
}


void CLoveSendClass::OnInitPropList()
{

	CGameImage	img;
	HRGN	hRgn;
	CString s=CBcfFile::GetAppPath ();/////����·��

	CBcfFile f( s + m_iniFile);

	CString key=TEXT("config");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal(key,"skinfolder","image\\client\\skin0\\game\\");
	skinfolder=s+"\\"+skinfolder;

	TCHAR	szPropImagePath[MAX_PATH];
	wsprintf(szPropImagePath,TEXT("%s\\prop\\"),skinfolder);


	//////////////////////////////////////////////////////////////////////////
	//2008-08-11
	//������ǰ�����ť��ͼƬ
	m_curPropPage=0;
	TCHAR szTemp[MAX_PATH];
	wsprintf(szTemp, TEXT("%sGoAhead.bmp"), szPropImagePath);
	m_BtPropGoAhead.LoadButtonBitmap(szTemp, false);
	img.SetLoadInfo(szTemp, true);
	hRgn =AFCBmpToRgn(img, RGB(255, 0, 255), RGB(1,1,1));
	if( hRgn )
		m_BtPropGoAhead.SetWindowRgn(hRgn, true);
	m_BtPropGoAhead.EnableWindow(FALSE);	//��ʼ��ʱǰ��ҳ��ť��Ч

	wsprintf(szTemp, TEXT("%sGoNext.bmp"), szPropImagePath);
	m_BtPropGoNext.LoadButtonBitmap(szTemp, false);
	img.SetLoadInfo(szTemp, true);
	hRgn =AFCBmpToRgn(img, RGB(255, 0, 255), RGB(1,1,1));
	if( hRgn )
		m_BtPropGoNext.SetWindowRgn(hRgn, true);


	Invalidate(false);



	wsprintf(m_szPropEmptyImage, TEXT("%sPropEmpty.bmp"), szPropImagePath);
	OnSetPropImages();

	//////////////////////////////////////////////////////////////////////////

}
void CLoveSendClass::OnSetPropImages()
{
	int propCout=Glb().userPropLibrary.GetCount();

	//���÷�ҳ
	if(m_curPropPage*PEER_PAGE>=propCout)
		m_curPropPage--;
	if(m_curPropPage<0)
		m_curPropPage=0;
	if(m_curPropPage>0)
		m_BtPropGoAhead.EnableWindow(TRUE);
	else
		m_BtPropGoAhead.EnableWindow(FALSE);

	if((m_curPropPage+1)*PEER_PAGE<propCout)
		m_BtPropGoNext.EnableWindow(TRUE);
	else
		m_BtPropGoNext.EnableWindow(FALSE);


	_TAG_USERPROP *userProp=NULL;
	CString key=TEXT("config");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + m_iniFile);
	skinfolder=f.GetKeyVal(key,"skinfolder","image\\client\\skin0\\game\\");
	skinfolder=s+"\\"+skinfolder;

	TCHAR	szPropImagePath[MAX_PATH];
	wsprintf(szPropImagePath,TEXT("%s\\prop\\"),skinfolder);

	TCHAR szPropImage[MAX_PATH]={0};
	for(int i=0;i<PEER_PAGE;i++)
	{
		int curPropIndex=m_curPropPage*PEER_PAGE+i;
		CGameImage img;
		HRGN hrgn;
		CNormalBitmapButtonEx *pButton=(CNormalBitmapButtonEx*)GetDlgItem(m_BtPropArray[i]);//ȡ4������BUTTON

		if(curPropIndex<propCout)
		{//����е���
			userProp=Glb().userPropLibrary.GetAt(curPropIndex);
			wsprintf(szPropImage, TEXT("%sProp%d.bmp"), szPropImagePath, (userProp->attribAction&0x7F000000)>>24);
			img.SetLoadInfo(szPropImage,true);
			pButton->LoadButtonBitmap(szPropImage,false);
			m_ToolTip.UpdateTipText(userProp->szPropName,this,m_BtPropArray[i]);
		}
		else
		{//���û�е���
			img.SetLoadInfo(m_szPropEmptyImage,true);
			pButton->LoadButtonBitmap(m_szPropEmptyImage,false);
			m_ToolTip.UpdateTipText("����������",this,m_BtPropArray[i]);
		}

		hrgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hrgn)
			pButton->SetWindowRgn(hrgn,true);
	}

}

BOOL CLoveSendClass::OnToolTipNotify(UINT id, NMHDR * pTTTStruct, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;    
	UINT nID =pTTTStruct->idFrom;			//�õ���Ӧ����ID���п�����HWND 
	CString stip="����������";
	int curPropIndex=m_curPropPage*PEER_PAGE;
	_TAG_USERPROP *userProp=NULL;
	int propCount=Glb().userPropLibrary.GetCount();
	if (pTTT->uFlags & TTF_IDISHWND)		//����nID�Ƿ�ΪHWND 
	{ 
		nID = ::GetDlgCtrlID((HWND)nID);	//��HWND�õ�IDֵ����Ȼ��Ҳ����ͨ��HWNDֵ���ж� 
		switch(nID) 
		{	
		case IDC_BUTTON_PROP1:
			break;
		case IDC_BUTTON_PROP2:
			curPropIndex+=1;
			break;
		case IDC_BUTTON_PROP3:
			curPropIndex+=2;
			break;
		case IDC_BUTTON_PROP4:
			curPropIndex+=3;
			break;
		default:
			break;
		}
		if(curPropIndex<propCount)
		{
			userProp=Glb().userPropLibrary.GetAt(curPropIndex);
			stip.Format("%s [%d]",userProp->szPropName,userProp->nHoldCount);
		}
		stip.Trim();
		lstrcpy(pTTT->lpszText, stip);	//������Ӧ����ʾ�ִ� 
	} 
	return FALSE; 
}

BOOL CLoveSendClass::OnUseProp(int nPropIndex, int nDeskUserID)
{

	//////////////////////////////////////////////////////////////////////////
	//2008-08-11
	if(nPropIndex<0 || nPropIndex>=Glb().userPropLibrary.GetCount())
		return FALSE;

	_TAG_USERPROP * userProp=Glb().userPropLibrary.GetAt(nPropIndex);
	if(userProp->nHoldCount<=0)
		return FALSE;
	_TAG_USINGPROP  usingProp;
	ZeroMemory(&usingProp,sizeof(_TAG_USINGPROP));
	usingProp.dwUserID=GetMeUserInfo()->dwUserID;
	usingProp.dwTargetUserID=nDeskUserID;
	usingProp.nPropID=userProp->nPropID;
	usingProp.nPropActionAttrib=userProp->attribAction;
	usingProp.nPropValueAttrib=userProp->attribValue;
	
	memcpy(usingProp.szPropName,userProp->szPropName,sizeof(userProp->szPropName));
	SendGameData(&usingProp,sizeof(_TAG_USINGPROP),MDM_GR_PROP,ASS_PROP_USEPROP,0);
	return TRUE;
}

void CLoveSendClass::UsePropItem(int propIndex)
{
	//////////////////////////////////////////////////////////////////////////
	//Modify Fred Huang,2008-04-23
	if(GetMeUserInfo()->bUserState==USER_WATCH_GAME)
	{
		//AFCMessageBox("�Բ����Թ�ʱ����ʹ�õ��ߣ�","��ʾ");
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�Բ����Թ�ʱ����ʹ�õ��ߣ�");
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//2008-08-11
	int propCount=Glb().userPropLibrary.GetCount();
	if(propIndex<0 || propIndex>=propCount)
	{
		::PostMessage(m_pGameRoom->GetSafeHwnd(),WM_USER+150,0,0);
		//û�е��ߣ��������
		//OnGetBuyItemData(0,0);
		return;
	}
	_TAG_USERPROP * userProp=Glb().userPropLibrary.GetAt(propIndex);
	if(userProp==NULL)
		return;

	if(userProp->attribAction & 0x80 || userProp->attribAction &0x100)
	{//�����Ⱥ�С����
		::PostMessage(m_pGameRoom->m_hWnd,WM_USER+156,(WPARAM)userProp,0);
		return;
	}
	
	//���ֶ��Լ�ʹ�ã����Ƕ�������ʹ��
	if(userProp->attribAction & 0x01)
	{//��������ʹ��
		if(hPropSelUser==NULL)
		{
			hPropSelUser=new CPropSelUser;
			hPropSelUser->Create(IDD_PROP_SEL_USER,this);
		}
		for(int i=0;i<MAX_PEOPLE;i++)
			hPropSelUser->m_pUserItem[i]=m_pUserInfo[i];
		hPropSelUser->dwUsedPropID=propIndex;
		hPropSelUser->dwLocalUserID=GetMeUserInfo()->dwUserID;
		hPropSelUser->propItemName=userProp->szPropName;
		hPropSelUser->resetInformation();
		hPropSelUser->CenterWindow();
		hPropSelUser->ShowWindow(SW_SHOW);		
	}
	else
	{//���Լ�ʹ��
		////////////////////////////////////////////////////
		///2008 1216 Kylin ����ʱʹ�ø�������û����ʾ
		CString stip;
		stip.Format("��ȷ��Ҫʹ�õ��� %s ��?",userProp->szPropName);
		if(userProp->attribAction & 0x2)
		{
			if(GetMeUserInfo()->dwPoint>=0)
			{
				stip.Format("���Ļ���Ϊ����ʹ�õ��� %s ����������һ�����߶�û��Ч����ȷ��ʹ����?",userProp->szPropName);
			}
			else stip+="\n�Ὣ���ĸ���������!";
		}
			
		////////////////////////////////////////////////////
		//if(AFCMessageBox(stip,"��ʾ",AFC_YESNO)==IDOK)
		if(DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION, "��ʾ",true,stip)==IDYES)
			OnUseProp(propIndex,GetMeUserInfo()->dwUserID);
	}
	return;

}


void CLoveSendClass::LoadSoundList(void)
{
	TCHAR sz[61],szPath[MAX_PATH];
	lstrcpy(sz,m_pGameInfo->szProcessName);
	sz[lstrlen(sz)-lstrlen(".ico")]='\0';
	wsprintf(szPath,"Music\\%s.bcf",sz,sz);
	CBcfFile f(szPath);
	int		soundCount=f.GetKeyVal("BGSOUND","SoundCount",0);
	if(soundCount==0)
		return;

	m_sndListID=BZSoundCreateList(this,0);
	if(m_sndListID==0)
		return;
	CString soundFile;
	for(int i=0;i<soundCount;i++)
	{
		wsprintf(sz,"Sound%d",i);
		soundFile=f.GetKeyVal("BGSOUND",sz,"");
		wsprintf(szPath,"%s",soundFile);
		BZSoundAddList(m_sndListID,szPath);
	}
	int nVol=f.GetKeyVal("BGSOUND","SoundVolume",300);
	BZSoundSetVolume(m_sndListID,nVol);
	m_bSndPause=f.GetKeyVal("BGSOUND","PauseSound",0);
	if(m_bSndPause)
	{
		BZSoundPause(m_sndListID);
		m_btSndPlay.ShowWindow(SW_SHOW);
		m_btSndPause.ShowWindow(SW_HIDE);
	}
	else
	{
		BZSoundPlayList(m_sndListID);
		m_btSndPlay.ShowWindow(SW_HIDE);
		m_btSndPause.ShowWindow(SW_SHOW);
	}
	if(AfxGetApp()->m_pMainWnd)
		AfxGetApp()->m_pMainWnd->PostMessage(WM_USER+100,2,0);
}

void CLoveSendClass::OnSndPlay()
{
	m_bSndPause=false;
	BZSoundContinue(m_sndListID);
	m_btSndPlay.ShowWindow(SW_HIDE);
	m_btSndPause.ShowWindow(SW_SHOW);

	TCHAR sz[61],szPath[MAX_PATH];
	lstrcpy(sz,m_pGameInfo->szProcessName);
	sz[lstrlen(sz)-lstrlen(".ico")]='\0';
	wsprintf(szPath,"Music\\%s.bcf",sz,sz);
	CBcfFile f(szPath);
	f.SetKeyValString("BGSOUND","PauseSound","0");

}
void CLoveSendClass::OnSndPause()
{
	m_bSndPause=true;
	BZSoundPause(m_sndListID);
	m_btSndPlay.ShowWindow(SW_SHOW);
	m_btSndPause.ShowWindow(SW_HIDE);

	TCHAR sz[61],szPath[MAX_PATH];
	lstrcpy(sz,m_pGameInfo->szProcessName);
	sz[lstrlen(sz)-lstrlen(".ico")]='\0';
	wsprintf(szPath,"Music\\%s.bcf",sz,sz);
	CBcfFile f(szPath);
	f.SetKeyValString("BGSOUND","PauseSound","1");
}
void CLoveSendClass::OnSndVolDown()
{
	int nVol=BZSoundGetVolume(m_sndListID);
	BZSoundSetVolume(m_sndListID,nVol-50);

	TCHAR sz[61],szPath[MAX_PATH];
	lstrcpy(sz,m_pGameInfo->szProcessName);
	sz[lstrlen(sz)-lstrlen(".ico")]='\0';
	wsprintf(szPath,"Music\\%s.bcf",sz,sz);
	CBcfFile f(szPath);
	CString sini;
	sini.Format("%d",(nVol-50)>1?nVol-50:1);
	f.SetKeyValString("BGSOUND","SoundVolume",sini);

}
void CLoveSendClass::OnSndVolUp()
{
	int nVol=BZSoundGetVolume(m_sndListID);
	BZSoundSetVolume(m_sndListID,nVol+50);

	TCHAR sz[61],szPath[MAX_PATH];
	lstrcpy(sz,m_pGameInfo->szProcessName);
	sz[lstrlen(sz)-lstrlen(".ico")]='\0';
	wsprintf(szPath,"Music\\%s.bcf",sz,sz);
	CBcfFile f(szPath);
	CString sini;
	sini.Format("%d",(nVol+50)<1000?nVol+50:1000);
	f.SetKeyValString("BGSOUND","SoundVolume",sini);
}

void CLoveSendClass::OnCbnSelInput()
{
	SetTimer(1,10,NULL);
}
void CLoveSendClass::SendTalkMessage(int nDefaultIndex)
{
	//�ж�״̬
	if ((GetMeUserInfo()->bDeskNO==255)
		||(GetMeUserInfo()->bDeskStation==255)) 
		return;

	//�ж���Ϣ
	m_InputMessage.SetFocus();
	if (m_InputMessage.GetWindowTextLength()==0) return;

	try
	{
		//��ֹ��Ϸ����
		if (m_pGameInfo->dwRoomRule & GRR_FORBID_GAME_TALK)
			throw TEXT("��Ǹ,����Ϸ��ֹ����!");
		//�ж��Ƿ��������
		if (CUserPower::CanGameTalk(m_pGameInfo->dwGamePower)==false) 
			throw TEXT("��Ǹ������ʱû�������Ȩ��!");

		//��ȡ��Ϣ
		CString strBuffer;
		m_InputMessage.GetWindowText(strBuffer);
		////�ж�Ŀ�����
		long int dwTargerID=0;

		if(m_CurpUserInfoClicked)
		{
			CString nick=m_CurpUserInfoClicked->nickName;
			nick+=":";
			int l=nick.GetLength();
			if(strBuffer.Left(l)==nick)
			{
				dwTargerID=m_CurpUserInfoClicked->dwUserID;
				strBuffer=strBuffer.Mid(l);
			}
		}

		if(strBuffer=="")
			return;

		//�ж���Ϣ����
		int iMaxSendLen=CUserPower::CanSendMuch(m_pGameInfo->dwGamePower)?MAX_TALK_LEN:NORMAL_TALK_LEN;
		if (strBuffer.GetLength()>iMaxSendLen)
			throw TEXT("���ڷ��͵���Ϣ̫��������ʧ�ܣ�");

		//������Ϣ
		if (strBuffer.Find(TEXT("{\\rtf"))!=-1) 
			throw TEXT("���ܷ��ͷǷ��ַ�������ʧ�ܣ�");

		for (int i=strBuffer.GetLength();i>0;i--)
			if ((strBuffer[i-1]>=-1)&&(strBuffer[i-1]<30))
				throw TEXT("���ܷ��ͷǷ��ַ�������ʧ�ܣ�");

		//�ж��ٶ�
		if (CUserPower::CanTalkFast(m_pGameInfo->dwGamePower)==false)
		{
			long int dwNowTime=(long int)time(NULL);
			if ((CUserPower::CanTalkFast(m_pGameInfo->dwGamePower)==false)&&((m_dwLastTalkTime+1L)>dwNowTime))
				throw TEXT("�밮�����컷�����벻ҪƵ��������Ϣ��");
			if (((m_dwLastTalkTime+3L)>dwNowTime)&&(m_strLastTalk.Compare(strBuffer)==0)) 
				throw TEXT("�밮�����컷�����벻Ҫ�ظ����ԣ�");
		}

		//����������Ϣ
		MSG_GR_RS_NormalTalk Talk;
		ZeroMemory(&Talk,sizeof(MSG_GR_RS_NormalTalk));
		Talk.iLength=strBuffer.GetLength();
		Talk.dwSendID=GetMeUserInfo()->dwUserID;
		Talk.dwTargetID=dwTargerID;
		Talk.nDefaultIndex=nDefaultIndex;
		lstrcpy(Talk.szMessage,strBuffer);
		Talk.crColor=m_colorresource.m_crTalkTxColor;

		SendGameData(&Talk,sizeof(Talk)-sizeof(Talk.szMessage)+Talk.iLength+1,MDM_GM_GAME_FRAME,ASS_GM_NORMAL_TALK,0);

		//������Ϣ
		m_strLastTalk=Talk.szMessage;
		if (m_InputMessage.FindString(0,m_strLastTalk)==-1) 
			m_InputMessage.InsertString(0,m_strLastTalk);

		if (m_InputMessage.GetCount()>40) 
			m_InputMessage.DeleteString(40-m_nTalkDefaultCount);

		//���ò���
		m_dwLastTalkTime=(ULONG)time(NULL);
		m_InputMessage.SetWindowText(TEXT(""));
	}
	catch (TCHAR * szErrorMessage) 
	{
		m_MessageHandle.InsertSystemMessage(szErrorMessage);
	}
	catch (...) 
	{ 
		m_MessageHandle.InsertSystemMessage(TEXT("��Ϣ����ʧ�ܣ�"));
	}
	::SendMessage(m_TalkMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
	return;
}

BOOL CLoveSendClass::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return true;
	//return __super::OnEraseBkgnd(pDC);
}

/************************************************************************/
//Function:checkNetSignal()
//Parameter:none
//Author:Fred Huang 2008-06-27
//Note:�������������������Ϣ
/************************************************************************/
void CLoveSendClass::checkNetSignal(void)
{
	if (m_pGameInfo->pMeUserInfo == NULL)
	{
		return;
	}
	if(m_pGameInfo->pMeUserInfo->GameUserInfo.dwUserID==0)        //û��ID�ţ�����
		return;
	//����ϴε���Ϣ��û���յ��������񼫲����Ϣ����Ҫ��
	if(m_NetSignalIndex>m_SavedSingalIndex)
	{
		m_NetSignalLevel=0;

	}
	drawSignal();
	//����ǲ���Ҫ����Ϣ��
	
}

/************************************************************************/
//Function:checkNetSignal()
//Parameter:
//                pNetData        :���������ص���Ϣ��
//Author:Fred Huang 2008-06-27
//Note:�������������ص���Ϣ
/************************************************************************/
void CLoveSendClass::getNetSignal(void * pNetData)
{
	MSG_GP_Game_NETSignal *netSignal=(MSG_GP_Game_NETSignal*)pNetData;
	//������Ƿ����Լ��ģ�����
	if(m_pGameInfo->pMeUserInfo->GameUserInfo.dwUserID!=netSignal->dwUserID)
		return;
	//�Ƚ�������0
	m_NetSignalLevel=0;
	//����ʱ��
	UINT uTimeDiff=GetTickCount()-netSignal->dwTimeFlag;

	m_NetSignalIndex = netSignal->dwSignalIndex;
	//ֻ���ǵ�ǰ����Ϣ�����ż��㼶��
	if(netSignal->dwIndex==m_NetSignalIndex)
	{
		if(uTimeDiff<=50)
			m_NetSignalLevel=1;
		else if(uTimeDiff>50 && uTimeDiff<=200)
			m_NetSignalLevel=2;
		else if(uTimeDiff>200 && uTimeDiff<=500)
			m_NetSignalLevel=3;
		else if(uTimeDiff>500 && uTimeDiff<=1000)
			m_NetSignalLevel=4;
		else                 
			m_NetSignalLevel=5;

	}
	//���浱ǰ�յ������
	m_SavedSingalIndex=netSignal->dwIndex;
	//��ͼ
	drawSignal();
	//Invalidate(true);
}

/************************************************************************/
//Function:drawSignal()
//Parameter:��
//Author:Fred Huang 2008-06-27
//Note:��ͼ
/************************************************************************/
void CLoveSendClass::drawSignal(void)
{
	if(!pSingnalimg || pSingnalimg->GetFlags()==0)
		return;
	CString s=CBcfFile::GetAppPath();
	CBcfFile f( s + "skin0.bcf");
	CString key=Glb().m_skin;

	CRect rc;
	GetClientRect(rc);
	int x = rc.Width() - f.GetKeyVal(key, _T("game_signalx"),160) - pSingnalimg->GetWidth();

	Graphics g(this->m_hWnd);
	g.DrawImage(pSingnalimg,Rect(x,f.GetKeyVal(key, _T("game_signaly"),2),pSingnalimg->GetWidth(),pSingnalimg->GetHeight()/6),0,m_NetSignalLevel*pSingnalimg->GetHeight()/6,pSingnalimg->GetWidth(),pSingnalimg->GetHeight()/6,UnitPixel);
}

void char2Wchar(char* pchar,wchar_t* wchar)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP,0,pchar,-1,NULL,0);
	MultiByteToWideChar(CP_ACP,0,pchar,-1,wchar,nwLen);
}

void CLoveSendClass::OnExchangeSkin(const CString& strFileName,const CString& strSkinKey,const CString& strSkinPath)
{
	LoadSkin(strFileName,strSkinKey,strSkinPath);
	Invalidate();
}
