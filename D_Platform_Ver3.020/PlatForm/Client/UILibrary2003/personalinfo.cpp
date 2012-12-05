// PersonalInfo.cpp : ʵ���ļ�
//
#include <winuser.h>
#include <windows.h>
#include "stdafx.h"
#include "resource.h"
#include "PersonalInfo.h"
//#include "GameRoomEx.h"
#include "gameframedlg.h"
#include "../MainFrame/GameRoomEx.h"

// CPersonalInfo �Ի���

//Avatarϵͳ���
#define BzGetAvatarIndex(obj) \
	GetAvatarIndexInRoom((obj)->bLogoID,(obj)->i64Bank,(obj)->i64Money,\
		(obj)->dwPoint,((CGameRoomEx*)m_roomWnd)->m_GameInfo.uNameID,\
		((CGameRoomEx*)m_roomWnd)->m_GameInfo.uComType)

IMPLEMENT_DYNAMIC(CPersonalInfo, CDialog)

CPersonalInfo::CPersonalInfo(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PERSONINFO, pParent)
	//, m_szpNiCheng(NULL)
	, m_i64CaiFu(0)
	, m_lJiFeng(0)
	, m_szJiBie(_T(""))
	, m_szNiCheng(_T(""))
	, m_time(0)
	, pOrderName(NULL)
	, m_roomWnd(NULL)
{
	m_FontClr = RGB(146,220,255);
	m_bkBrush=NULL;
}

CPersonalInfo::~CPersonalInfo()
{
	DeleteObject(m_bkBrush);
}

void CPersonalInfo::getRoomPtr(CWnd* room)
{
	m_roomWnd=room;
	
};

bool CPersonalInfo::UpdateUserItem(UserInfoStruct user)
{
	if(user.dwUserID!=0)
	{
		m_userInfoItem=user;
		m_szNiCheng=m_userInfoItem.nickName;
		m_i64CaiFu=m_userInfoItem.i64Money;
		if(pOrderName!=NULL)
			m_szJiBie=pOrderName(m_userInfoItem.dwPoint,((CGameRoomEx*)m_roomWnd)->m_GameInfo.uNameID);
		else m_szJiBie="";
		m_lJiFeng=m_userInfoItem.dwPoint-GetNormalOrderBasePoint(m_userInfoItem.dwPoint);
		//��������ͷ��

		return true;
	}else return false;
}

LRESULT CPersonalInfo::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CTLCOLORSCROLLBAR :
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSTATIC :
		COLORREF col=::GetBkColor((HDC)wParam);
		SetBkMode((HDC)wParam,TRANSPARENT);
		SetTextColor((HDC)wParam, RGB(255,0,0)) ;
		SetBkColor((HDC)wParam,m_bkimage.GetPixel(0,0));
		return (LRESULT)m_bkBrush;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CPersonalInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	TCHAR		path[MAX_PATH];
	CString		SkinFolder;
	CString		key = "PersonalInfo";//Glb().m_skin;

	CString		s = CBcfFile::GetAppPath ();/////����·��
	CBcfFile	f( s + "skin0.bcf");
	SkinFolder	= f.GetKeyVal("skin0", "skinfolder", "image\\client\\skin0\\");

	m_FontClr = f.GetKeyVal(key,"PersonalInfoRGB",(int)RGB(146,220,255));

	ZeroMemory(path,sizeof(path));
	wsprintf(path, "%s%sgame\\userInfo.bmp", s, SkinFolder);
	
	int cx,cy;
	CGameImage	m_bt;
	m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper ImageHandle(&m_bkimage);
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_bkimage.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		///{{ Added by zxd 20100709 �ͷ��������
		DeleteObject(hRgn);
		///Added by zxd 20100709 �ͷ��������}}
	}
	cx=ImageHandle.GetWidth();
	cy=ImageHandle.GetHeight();
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);

	ZeroMemory(path,sizeof(path));
	wsprintf(path, "%s%sgame\\Chat_bt.bmp", s, SkinFolder);
	m_bt.SetLoadInfo(path,true);
	m_btChat.LoadButtonBitmap(path,false);
	m_btChat.SetWindowPos(NULL,0,0,m_bt.GetWidth()/4,m_bt.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	ZeroMemory(path,sizeof(path));
	wsprintf(path, "%s%sgame\\Invite_bt.bmp", s, SkinFolder);
	m_bt.SetLoadInfo(path,true);
	m_btInvite.LoadButtonBitmap(path,false);
	m_btInvite.SetWindowPos(NULL,0,0,m_bt.GetWidth()/4,m_bt.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

    // PengJiLin, 2010-8-17, ����˽�İ�ť
    if (8 & Glb().m_CenterServerPara.m_nFunction)   // �Ƿ��ֹ˽��
    {
        CRect rect;
        m_btChat.GetWindowRect(&rect);
        ScreenToClient(&rect);
        m_btChat.ShowWindow(SW_HIDE);
        m_btInvite.MoveWindow(&rect);
    }

	ZeroMemory(path,sizeof(path));
	wsprintf(path, "%s%sgame\\watchgame_bt.bmp", s, SkinFolder);
	//::lstrcpy(path,"image\\game\\watchgame_bt.bmp");
	m_bt.SetLoadInfo(path,true);
	m_btWatch.LoadButtonBitmap(path,false);
	m_btWatch.SetWindowPos(NULL,0,0,m_bt.GetWidth()/4,m_bt.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	ZeroMemory(path,sizeof(path));
	wsprintf(path, "%s%sgame\\gift_bt.bmp", s, SkinFolder);
	//::lstrcpy(path,"image\\game\\watchgame_bt.bmp");
	m_bt.SetLoadInfo(path,true);
	m_btGift.LoadButtonBitmap(path,false);
	m_btGift.SetWindowPos(NULL,0,0,m_bt.GetWidth()/4,m_bt.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	memset(&m_userInfoItem,0,sizeof(m_userInfoItem));
	m_szNiCheng=m_userInfoItem.nickName;
	m_i64CaiFu=m_userInfoItem.i64Money;

	
	if(pOrderName!=NULL)
		m_szJiBie=pOrderName(m_userInfoItem.dwPoint,((CGameRoomEx*)m_roomWnd)->m_GameInfo.uNameID);
	m_lJiFeng=m_userInfoItem.dwPoint-GetNormalOrderBasePoint(m_userInfoItem.dwPoint);
	
	m_time=0;
	;;SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
 	//-------------------------����͸��----------------------
	HINSTANCE hInst = LoadLibrary("User32.DLL");
	if(hInst != NULL) 
	{ 
	typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		//ȡ��SetLayeredWindowAttributes����ָ�� 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)fun(this->GetSafeHwnd(),RGB(255,0,255),0,2); 
		FreeLibrary(hInst); 
	} 
	SetTimer(WINDOW_APP,15,NULL);

	m_bkBrush=CreateSolidBrush(m_bkimage.GetPixel(0,0));

	return FALSE;  // return TRUE unless you set the focus to a control
 }

void CPersonalInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CAIFU, m_i64CaiFu);
	DDX_Text(pDX, IDC_JIFENG, m_lJiFeng);
	DDX_Text(pDX, IDC_JIBIE, m_szJiBie);
	DDX_Text(pDX, IDC_NICHENG, m_szNiCheng);

	DDX_Control(pDX, IDC_CHAT, m_btChat);
	DDX_Control(pDX, IDC_INVITE_PLAY, m_btInvite);
	DDX_Control(pDX, IDC_WATCH_GAME, m_btWatch);
	DDX_Control(pDX, IDC_GIFT, m_btGift);
}

void CPersonalInfo::OnPaint()
{
	//wushuqun 2009.5.19
	//�޸ĳ���"δ֪״̬"BUG
	if (m_userInfoItem.dwUserID == 0)
	{
		CString str;
		str.Format("���ڻ�ȡ���״̬ %d",m_userInfoItem.dwUserID);
		::OutputDebugString(str);

		ShowWindow(0);
	}
	CPaintDC dc(this);
	CGameImageHelper	handle(&m_bkimage);
	handle.BitBlt(dc.GetSafeHdc(),0,0);

	int y=22;

	TCHAR szBuffer[200];
	WCHAR wszBuffer[200];
	memset(szBuffer,0,sizeof(szBuffer));
	memset(wszBuffer,0,sizeof(wszBuffer));

	COLORREF oldClr = dc.SetTextColor(m_FontClr);//JMod-20090506
	////////////////////////////////////
	//Kylin 20090325 �޸�������ɫΪ������
	CString		key = TEXT("PersonalInfo");//Glb().m_skin;
	CString		s = CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "skin0.bcf");

	dc.SetTextColor(RGB(f.GetKeyVal(key, "Userinfor", 255),
		f.GetKeyVal(key, "Userinfog", 255),
		f.GetKeyVal(key, "Userinfob", 255)));
	////////////////////////////////////

	dc.SetBkMode(TRANSPARENT);
	int iCountTotal=m_userInfoItem.uLostCount+m_userInfoItem.uMidCount+m_userInfoItem.uWinCount;
	int temprun=(iCountTotal==0)?0:m_userInfoItem.uCutCount*100/iCountTotal,
		tempwin=(iCountTotal==0)?0:m_userInfoItem.uWinCount*100/iCountTotal;

	//����ͷ��
	//////////////////////////////////////////////////////////////////////////
	//Modify By Fred Huang,2008-04-23
	wsprintf(szBuffer,TEXT("%simage\\log\\%s\\gamelog%ld.png"),CBcfFile::GetAppPath(),m_userInfoItem.bBoy?"Boy":"Girl",BzGetAvatarIndex(&m_userInfoItem));
	if(m_userInfoItem.bLogoID>=0xFF && m_userInfoItem.bLogoID<0x200)
	{
		wsprintf(szBuffer,"CustomFace\\%d.png",m_userInfoItem.dwUserID);
		CFileFind ff;
		BOOL bFoundFile=ff.FindFile(szBuffer);
		ff.Close();
		if(!bFoundFile)
			wsprintf(szBuffer,"%s\\image\\log\\%s\\gamelog%d.png",CBcfFile::GetAppPath(),m_userInfoItem.bBoy?"Boy":"Girl",BzGetAvatarIndex(&m_userInfoItem));
	}
	if (!CBcfFile::IsFileExist(szBuffer))
	{
		wsprintf(szBuffer,TEXT("%simage\\log\\%s\\gamelog%ld.png"),CBcfFile::GetAppPath(),m_userInfoItem.bBoy?"Boy":"Girl", 0/*m_userInfoItem.bLogoID%12*/); ///< ���û�и�IDͼƬ����Ĭ��Ϊ0
	}

//////////////////////////////////////////////////////////////////////////
	MultiByteToWideChar(CP_ACP,MB_COMPOSITE,szBuffer,-1,wszBuffer,sizeof(wszBuffer));
	Graphics graphics(dc.GetSafeHdc()); // Create a GDI+ graphics object
#ifndef BZ_ZLIB
	Image image(wszBuffer); // Construct an image
#else
	CUnZipRes unZipRes;
	IStream * pStream=NULL;
	pStream=unZipRes.LoadFileResFromZip(szBuffer);
	Image image(pStream);
	if(pStream!=NULL)
	{
		pStream->Release();
		pStream=NULL;
	}
#endif
	

	//�ȱ����� 20090419 Shao
	int w=image.GetWidth();
	int h=image.GetHeight();
	if(w > 51
		|| h > 73)
	{
		if(w >0 && h >0)
		{
			if(w/51 > h/71)//��ȳ����Ķ࣬����ȱ������ţ�
			{
				w = 51;
				h = image.GetHeight() * 51 /image.GetWidth();
			}
			else
			{
				//�߶ȳ����Ķࣻ���߶ȱ������ţ�
				h = 71;
				w = 71 * image.GetWidth() / image.GetHeight();
			}
		}
	}
	//graphics.DrawImage(&image, 125, 6, 51,73);// image.GetWidth(), image.GetHeight());
	graphics.DrawImage(&image, 125+12, 6+18, w,h);
	//20090219 end;

	CFont font;
	font.CreateFont(12,0,0,0,700,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
	CFont *pOldFont = dc.SelectObject(&font);

	//�ݳ�
	wsprintf(szBuffer,TEXT("%s"),m_szNiCheng);
	
	//CRect rect(47,29,m_bkimage.GetWidth()-w-47,299);
	CRect rect(47,29,m_bkimage.GetWidth(),m_bkimage.GetHeight());
	rect.right -= w+8;
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_LEFT|DT_TOP|DT_END_ELLIPSIS);
	rect.right += w+8;

	//JMod-���"ְҵ"-20090507
	wsprintf(szBuffer,TEXT("%s"),m_userInfoItem.szOccupation);
	rect.left=69-20;
	rect.top+=y;
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_LEFT|DT_TOP);

	////add by yjj 090323
	wsprintf(szBuffer,TEXT("%s"),m_userInfoItem.szPhoneNum);
	rect.left=69;

	//wushuqun 2009.5.26
	rect.top+=y;
	rect.top -= 2;
	
	//rect.right = rect.left + 55;
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_LEFT|DT_TOP);	

	//wsprintf(szBuffer,TEXT("%s%s"),m_userInfoItem.szCity,m_userInfoItem.szZone);
	//rect.top+=y;
	//dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_LEFT|DT_TOP);

	//wushuqun 2009.5.19
	//���������Ϊ��ʡ.��"
	CString strAre;
	strAre = m_userInfoItem.szProvince + CString(".") + m_userInfoItem.szCity;
	rect.top+=y;
	dc.DrawText(strAre,strAre.GetLength(),&rect,DT_LEFT|DT_TOP);


	switch(m_userInfoItem.bUserState)
	{	
	case USER_NO_STATE:
		lstrcpy(szBuffer,"���ڻ�ȡ���״̬");
		break;
	case USER_LOOK_STATE:
		lstrcpy(szBuffer,"δ����");
		break;
	case USER_SITTING:
		lstrcpy(szBuffer,"����Ϸ��");
		break;
	case USER_ARGEE:
		lstrcpy(szBuffer,"ͬ��״̬");
		break;
	case USER_WATCH_GAME:
		lstrcpy(szBuffer,"�Թ���Ϸ");
		break;
	case USER_CUT_GAME:
		lstrcpy(szBuffer,"����״̬");
		break;
	case USER_PLAY_GAME:
		lstrcpy(szBuffer,"��Ϸ��");
		break;
	default:
		lstrcpy(szBuffer,"���ڻ�ȡ���״̬");
		break;
	}
	
	rect.top+=y;
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_LEFT|DT_TOP);	

	//�Ƹ�����
	wsprintf(szBuffer,TEXT("%s"),GetMoneyOrderName(m_i64CaiFu,((CGameRoomEx*)m_roomWnd)->m_GameInfo.uNameID));
	rect.top+=y;
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_LEFT|DT_TOP);	

	//����
	wsprintf(szBuffer,TEXT("%d"),m_userInfoItem.dwPoint-GetNormalOrderBasePoint(m_userInfoItem.dwPoint));
	rect.left=47;
	rect.top+=y;
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_LEFT|DT_TOP);	

	//����
	TCHAR szNum[128];
    GetNumString(szNum, m_userInfoItem.i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
	wsprintf(szBuffer,TEXT("%s"),szNum);
	rect.top+=y;
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_LEFT|DT_TOP);	

	//����
	wsprintf(szBuffer,TEXT("%d"),90);
	rect.top+=y;
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_LEFT|DT_TOP);	
	
	//������
	wsprintf(szBuffer,TEXT("%2d%%"),temprun);
	rect.left+=20;
	rect.top+=y;
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_LEFT|DT_TOP);

	//��
	wsprintf(szBuffer,TEXT("%d"),m_userInfoItem.uLostCount);
	rect.left-=20;
	rect.top+=y;
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_LEFT|DT_TOP);	

	//��
	wsprintf(szBuffer,TEXT("%d"),m_userInfoItem.uWinCount);
	rect.left+=60;
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_LEFT|DT_TOP);	

	//ʤ��
	wsprintf(szBuffer,TEXT("%2d%%"),tempwin);
	rect.left-=60;
	rect.top+=y;
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_LEFT|DT_TOP);	

	//����
	wsprintf(szBuffer,"%d",m_userInfoItem.dwFascination);
	rect.left+=70;
	dc.DrawText(szBuffer,lstrlen(szBuffer),&rect,DT_LEFT|DT_TOP);

	dc.SetTextColor(oldClr);
	::DeleteObject(dc.SelectObject(pOldFont));

}
void CPersonalInfo::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==WINDOW_APP)
	{
		KillTimer(WINDOW_DISAPP);
		if(m_time==20) 
		{
			KillTimer(WINDOW_APP);
		}
		else 
		{
			m_time++;
			//;;SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
			//-------------------------����͸��----------------------
			HINSTANCE hInst = LoadLibrary("User32.DLL");
			if(hInst != NULL) 
			{ 
				typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
				MYFUNC fun = NULL;
				//ȡ��SetLayeredWindowAttributes����ָ�� 
				fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
				if(fun)fun(this->GetSafeHwnd(),RGB(255,0,255),12*m_time,2); 
				FreeLibrary(hInst); 
			} 
		}
	}else if(nIDEvent==WINDOW_DISAPP)
	{
		KillTimer(WINDOW_APP);
		if(m_time==0)
		{
			KillTimer(WINDOW_DISAPP);
			DestroyWindow();
		}else 
		{
			m_time--;
			//;;SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
			//-------------------------����͸��----------------------
			HINSTANCE hInst = LoadLibrary("User32.DLL");
			if(hInst != NULL) 
			{ 
				typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
				MYFUNC fun = NULL;
				//ȡ��SetLayeredWindowAttributes����ָ�� 
				fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
				if(fun)fun(this->GetSafeHwnd(),RGB(255,0,255),12*m_time,2); 
				FreeLibrary(hInst); 
			} 
		}
	}
	CWnd::OnTimer(nIDEvent);
}
void CPersonalInfo::close()
{
	KillTimer(WINDOW_APP);
	SetTimer(WINDOW_DISAPP,10,NULL);
}

BEGIN_MESSAGE_MAP(CPersonalInfo, CDialog)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHAT, &CPersonalInfo::OnBnClickedChat)
	ON_BN_CLICKED(IDC_INVITE_PLAY, &CPersonalInfo::OnBnClickedCall)
	ON_BN_CLICKED(IDC_WATCH_GAME, &CPersonalInfo::OnBnClickedWatchGame)
	ON_BN_CLICKED(IDC_GIFT, &CPersonalInfo::OnBnClickedGift)
END_MESSAGE_MAP()


// CPersonalInfo ��Ϣ�������

void CPersonalInfo::OnBnClickedChat()
{
	 //TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_roomWnd!=NULL)
	{
		//UserItemStruct * pUserItem=((CGameRoomEx*)m_roomWnd)->m_UserManage.FindOnLineUser(m_userInfoItem.dwUserID);
		//((CGameRoomEx*)m_roomWnd)->SendShortMessage(pUserItem);
		m_roomWnd->PostMessage(WM_CHATING_WITH,(WPARAM)m_userInfoItem.dwUserID,(LPARAM)m_userInfoItem.dwUserID);
	}
}

///�����Ѿ��������������һ����Ϸ
void CPersonalInfo::OnBnClickedCall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���ô����Ӻ�λ�Ӻŵ��û�Ϊ��ǰ�û�
	if(m_roomWnd!=NULL)
	{
		//((CGameRoomEx*)m_roomWnd)->SetCurrentUser(m_userInfoItem.dwUserID);
		//((CGameRoomEx*)m_roomWnd)->OnInvitePlay();
		m_roomWnd->PostMessage(WM_INVITE_PLAY,(WPARAM)m_userInfoItem.dwUserID,(LPARAM)m_userInfoItem.dwUserID);
	}
}

void CPersonalInfo::OnBnClickedWatchGame()
{
	if(m_roomWnd!=NULL)
	{
		//((CGameRoomEx*)m_roomWnd)->OnLeftHitDesk(m_userInfoItem.bDeskNO,m_userInfoItem.bDeskStation);
		m_roomWnd->PostMessage(WM_WATCH_GAME,(WPARAM)m_userInfoItem.bDeskNO,(LPARAM)m_userInfoItem.bDeskStation);
	}
}

void CPersonalInfo::OnBnClickedGift()
{
	if(m_roomWnd!=NULL)
	{
		//((CGameRoomEx*)m_roomWnd)->OnLeftHitDesk(m_userInfoItem.bDeskNO,m_userInfoItem.bDeskStation);
		m_roomWnd->PostMessage(WM_USER+157,(WPARAM)m_userInfoItem.dwUserID,1);
	}

}