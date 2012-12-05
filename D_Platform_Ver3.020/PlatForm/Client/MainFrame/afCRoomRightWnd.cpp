#include "afCRoomRightWnd.h"
#include "afCRoomRightWnd.h"
#include "StdAfx.h"
#include "AfCRoomRightWnd.h"
#include "GamePlace.h"
#include "PlaceResource.h"
#include "MainRoomEx.h"
#include "FileVersion.h"
#include "resource.h"

#include "detours.h"
#include "skinsb.h"

BEGIN_MESSAGE_MAP(CRoomRightWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_MESSAGE(WM_LOADSKIN,OnLoadSkin)
	ON_MESSAGE(IDM_SEND_ROOMMSG,OnSendRoomMsg)
	ON_MESSAGE(WM_EXCHANGE_SKIN,OnExchangeSkin)
	ON_NOTIFY(NM_RCLICK, IDC_USER_LIST, OnNMRclickUserList)
	ON_NOTIFY(NM_CLICK, IDC_USER_LIST, OnNMLblclkUserList)
	ON_NOTIFY(NM_DBLCLK, IDC_USER_LIST, OnNMDblclkUserList)

	ON_BN_CLICKED(IDC_PROPBUY,OnBnClickedPropBuy)
	ON_BN_CLICKED(IDC_PROPPREV,OnBnClickedPropPrev)
	ON_BN_CLICKED(IDC_PROPNEXT,OnBnClickedPropNext)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_ARROW,OnBnClickedLeftArrow)
    ON_BN_CLICKED(IDC_BUTTON_RIGHT_ARROW,OnBnClickedRightArrow)
	ON_BN_CLICKED(IDC_PROP1,OnBnClickedProp1)
	ON_BN_CLICKED(IDC_PROP2,OnBnClickedProp2)
	ON_BN_CLICKED(IDC_PROP3,OnBnClickedProp3)
    ON_BN_CLICKED(IDC_PROP4,OnBnClickedProp4)
	ON_BN_CLICKED(IDC_PROP5,OnBnClickedProp5)
	ON_BN_CLICKED(IDC_PROP6,OnBnClickedProp6)
	ON_BN_CLICKED(IDC_PROP7,OnBnClickedProp7)
	ON_BN_CLICKED(IDC_PROP8,OnBnClickedProp8)

	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify)


	//ON_NOTIFY(NM_RCLICK, IDC_FRIEND_LIST, OnNMRclickFriendList)
	//ON_NOTIFY(NM_DBLCLK, IDC_FRIEND_LIST, OnNMDblclkFriendList)
	ON_BN_CLICKED(IDC_HIDE_RIGHT,OnBnClickedHideList)

	ON_MESSAGE(WM_RELOAD_TALKINI,OnReLoadTalkINI)

    // PengJiLin, 2010-10-12, �̳�����
    ON_BN_CLICKED(IDC_BUTTON_PROP1,OnBnClickedBtnProp1S)
    ON_BN_CLICKED(IDC_BUTTON_PROP2,OnBnClickedBtnProp2S)
    ON_BN_CLICKED(IDC_BUTTON_PROP3,OnBnClickedBtnProp3S)

END_MESSAGE_MAP()

/******************************************************************************************************/
//���캯��
CRoomRightWnd::CRoomRightWnd() : CGameFaceGo(IDD_ROOM_RIGHT_WND),m_MessageHandle(&m_TalkMessage)
{	
	m_bHideList=false;
	m_bInit = false;
	m_strAdvertise="";
	m_advHeight=0;
	webWnd=NULL;
	//pannelHeight=27;								// modify by wyl   11-5-13
	PropBgcolor=RGB(237,231,255);
	m_curPropPage=0;
	m_pMainWnd=0;
	m_Rgn.CreateRectRgn(0,0,0,0);

    m_curPropPage_S = 0;

}
//���캯��
CRoomRightWnd::CRoomRightWnd(UINT uDialogTemplata)
: CGameFaceGo(uDialogTemplata),m_MessageHandle(&m_TalkMessage)
{	
	m_bHideList=false;
	m_strAdvertise="";
	m_advHeight=0;
	webWnd=NULL;
	//pannelHeight=27;										// modify by wyl   11-5-13
	PropBgcolor=RGB(237,231,255);
	m_curPropPage=0;
	m_pMainWnd=0;
	m_Rgn.CreateRectRgn(0,0,0,0);

    m_curPropPage_S = 0;
}
//��������
CRoomRightWnd::~CRoomRightWnd()
{
	m_ChatControl.DestroyWindow();
	if (webWnd != NULL)
	{
		webWnd->DestroyWindow();
		delete webWnd;
	}
	m_Rgn.DeleteObject();
	m_ToolTip.DestroyWindow();
}
//��ʼ������
BOOL CRoomRightWnd::OnInitDialog()
{
	CGameFaceGo::OnInitDialog();
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	m_ChatControl.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,11);

	CString s=CBcfFile::GetAppPath ();
	CBcfFile f( s + "bzgame.bcf");
	//CString m_key="BZW";
	m_strAdvertise=f.GetKeyVal("BZW","RoomAdvertise","");

	webWnd=new CWebWnd;
	webWnd->Create(IDD_WEB_WND,this);
	webWnd->ShowWindow(SW_SHOW);
	LoadSkin();
	OnSetPropImage();
    OnSetPropImage_S();     // PengJiLin, 2010-10-12
	m_bInit = true;
	FixControlStation();
	m_TalkMessage.Init();

	if (!IsWindow(m_ToolTip.m_hWnd))
	{
		m_ToolTip.Create(this);
		m_ToolTip.AddTool(GetDlgItem(IDC_PROPBUY), TEXT("�������"));
		m_ToolTip.AddTool(GetDlgItem(IDC_PROPPREV), TEXT("��ǰ�鿴����")); //��������һ��
		m_ToolTip.AddTool(GetDlgItem(IDC_PROPNEXT), TEXT("���鿴����"));
        m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_LEFT_ARROW), TEXT("��ǰ�鿴����"));//�������ڶ���
        m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_RIGHT_ARROW), TEXT("���鿴����"));
		m_ToolTip.AddTool(GetDlgItem(IDC_PROP1), LPSTR_TEXTCALLBACK);
		m_ToolTip.AddTool(GetDlgItem(IDC_PROP2), LPSTR_TEXTCALLBACK);
		m_ToolTip.AddTool(GetDlgItem(IDC_PROP3), LPSTR_TEXTCALLBACK);
		m_ToolTip.AddTool(GetDlgItem(IDC_PROP4), LPSTR_TEXTCALLBACK);        // PengJiLin, 2010-10-12
		m_ToolTip.AddTool(GetDlgItem(IDC_PROP5), LPSTR_TEXTCALLBACK);
		m_ToolTip.AddTool(GetDlgItem(IDC_PROP6), LPSTR_TEXTCALLBACK);
		m_ToolTip.AddTool(GetDlgItem(IDC_PROP7), LPSTR_TEXTCALLBACK);
		m_ToolTip.AddTool(GetDlgItem(IDC_PROP8), LPSTR_TEXTCALLBACK);


        m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP1), LPSTR_TEXTCALLBACK);
        m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP2), LPSTR_TEXTCALLBACK);
        m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP3), LPSTR_TEXTCALLBACK);

		m_ToolTip.Activate(TRUE);
	}

    // PengJiLin, 2010-6-11, �������Ƿ���ʾ��������Ÿ��
    CString strPath = CBcfFile::GetAppPath ();
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
    CBcfFile fBcf( strPath + strSkin);
    BYTE byShow = fBcf.GetKeyVal("RoomRightWnd", "ShowPropPannel", 0);
    if(0 == byShow)
    {
        m_btPropBuy.ShowWindow(SW_HIDE);
        m_btPropPrev.ShowWindow(SW_HIDE);
        m_btProp1.ShowWindow(SW_HIDE);
        m_btProp2.ShowWindow(SW_HIDE);
        m_btProp3.ShowWindow(SW_HIDE);
        m_btProp4.ShowWindow(SW_HIDE);
		m_btProp5.ShowWindow(SW_HIDE);
		m_btProp6.ShowWindow(SW_HIDE);
		m_btProp7.ShowWindow(SW_HIDE);
		m_btProp8.ShowWindow(SW_HIDE);
        m_btPropNext.ShowWindow(SW_HIDE);

        // ���������߶���0
        pannelHeight = 0;

        // PengJiLin, 2010-10-11
        m_btPropPrev_S.ShowWindow(SW_HIDE);;
        m_btPropNext_S.ShowWindow(SW_HIDE);;
        m_btProp1_S.ShowWindow(SW_HIDE);;
        m_btProp2_S.ShowWindow(SW_HIDE);;
        m_btProp3_S.ShowWindow(SW_HIDE);;
        m_adjPropHeight = 0;
    }

    // PengJiLin, 2010-9-27, ������ʾ����
    m_btProp1.SetTextColor(RGB(255, 255, 255), true);
    m_btProp2.SetTextColor(RGB(255, 255, 255), true);
    m_btProp3.SetTextColor(RGB(255, 255, 255), true);
    m_btProp4.SetTextColor(RGB(255, 255, 255), true);
    m_btProp5.SetTextColor(RGB(255, 255, 255), true);
    m_btProp6.SetTextColor(RGB(255, 255, 255), true);
    m_btProp7.SetTextColor(RGB(255, 255, 255), true);
    m_btProp8.SetTextColor(RGB(255, 255, 255), true);

	PostMessage(WM_RELOAD_TALKINI,0,0);

	{
		CString s=CBcfFile::GetAppPath ();/////����·��
		CBcfFile f( s + strSkin);
		TCHAR path[MAX_PATH];
		CString skinfolder;
		if ("skin0.bcf" == strSkin)
			skinfolder = f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
		else
			skinfolder = f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());

		wsprintf(path,"%sDialog\\scroll.bmp",skinfolder);
		HBITMAP hbmScorll = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
		SkinSB_Init(m_UserList.GetSafeHwnd(), hbmScorll);
		SkinSB_Init(m_TalkMessage.GetSafeHwnd(), hbmScorll);
	}

	return TRUE;
}


//������Ϣ
LRESULT CRoomRightWnd::OnSendRoomMsg(WPARAM wparam, LPARAM lparam)
{
	this->GetParent()->SendMessage(IDM_SEND_ROOMMSG,wparam,lparam);
	return 0;
}
LRESULT CRoomRightWnd::OnExchangeSkin(WPARAM wparam, LPARAM lparam)
{
	CString s=CBcfFile::GetAppPath();
	CBcfFile f( s + "bzgame.bcf");
	m_strAdvertise=f.GetKeyVal("BZW","RoomAdvertise","");

	if (webWnd == NULL)
	{
		webWnd=new CWebWnd;
		webWnd->Create(IDD_WEB_WND,this);
	}
	webWnd->ShowWindow(SW_SHOW);

	LoadSkin();
	m_UserList.ChangeSkin();
	OnSetPropImage();
	OnSetPropImage_S();     // PengJiLin, 2010-10-12
	m_bInit = true;
	//FixControlStation();
	//m_TalkMessage.Init();

	// PengJiLin, 2010-6-11, �������Ƿ���ʾ��������Ÿ��
	CString strPath = CBcfFile::GetAppPath ();
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile fBcf( strPath + strSkin);
	BYTE byShow = fBcf.GetKeyVal("RoomRightWnd", "ShowPropPannel", 0);
	if(0 == byShow)
	{
		m_btPropBuy.ShowWindow(SW_HIDE);
		m_btPropPrev.ShowWindow(SW_HIDE);
		m_btProp1.ShowWindow(SW_HIDE);
		m_btProp2.ShowWindow(SW_HIDE);
		m_btProp3.ShowWindow(SW_HIDE);
		m_btProp4.ShowWindow(SW_HIDE);
		m_btProp5.ShowWindow(SW_HIDE);
		m_btProp6.ShowWindow(SW_HIDE);
		m_btProp7.ShowWindow(SW_HIDE);
		m_btProp8.ShowWindow(SW_HIDE);
		m_btPropNext.ShowWindow(SW_HIDE);
		// ���������߶���0
		pannelHeight = 0;
		// PengJiLin, 2010-10-11
		m_btPropPrev_S.ShowWindow(SW_HIDE);;
		m_btPropNext_S.ShowWindow(SW_HIDE);;
		m_btProp1_S.ShowWindow(SW_HIDE);;
		m_btProp2_S.ShowWindow(SW_HIDE);;
		m_btProp3_S.ShowWindow(SW_HIDE);;
		m_adjPropHeight = 0;
	}

	//PostMessage(WM_RELOAD_TALKINI,0,0);

	{
		CString s=CBcfFile::GetAppPath ();/////����·��
		CBcfFile f( s + strSkin);
		TCHAR path[MAX_PATH];
		CString skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());

		wsprintf(path,"%sDialog\\scroll.bmp",skinfolder);
		HBITMAP hbmScorll = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
		
		SkinSB_Uninit(m_UserList.GetSafeHwnd());
		SkinSB_Uninit(m_TalkMessage.GetSafeHwnd());
		SkinSB_Init(m_UserList.GetSafeHwnd(), hbmScorll);
		SkinSB_Init(m_TalkMessage.GetSafeHwnd(), hbmScorll);
	}
	m_UserList.Invalidate();
	//m_TalkMessage.Invalidate();
	Invalidate();
	return LRESULT(0);
}

//����Ҽ��û��б���Ϣ
void CRoomRightWnd::OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	if (pListNotify->iItem!=-1)
	{
		UserItemStruct * pUserItem=(UserItemStruct *)m_UserList.GetItemData(pListNotify->iItem);
		if (pUserItem!=NULL)
		{
			//OpenUserMenu(pUserItem);
			this->GetParent()->SendMessage(IDM_SEND_ROOMMSG,3,(LPARAM)pUserItem);
		}
	}
	*pResult=0;
}

///�����������б� ����������������������û��ǳ�
void CRoomRightWnd::OnNMLblclkUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	if (pListNotify->iItem!=-1)
	{
		UserItemStruct * pUserItem=(UserItemStruct *)m_UserList.GetItemData(pListNotify->iItem);
		if (pUserItem!=NULL) 
		{
			CPropPannel	*pPropPannel = GetMainRoom()->propPannel;
			if(pPropPannel != NULL)
				pPropPannel->m_ctlGiveUser.SetWindowText(pUserItem->GameUserInfo.nickName);
		}
	}
	*pResult=0;
}


///���˫������б���ָ����ҷ���Ϣ
void CRoomRightWnd::OnNMDblclkUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	if (pListNotify->iItem!=-1)
	{
		UserItemStruct * pUserItem=(UserItemStruct *)m_UserList.GetItemData(pListNotify->iItem);
		if (pUserItem!=NULL) 
			this->GetParent()->SendMessage(IDM_SEND_ROOMMSG,4,(LPARAM)pUserItem);
			//SendShortMessage(pUserItem);
	}
	*pResult=0;
}

BOOL CRoomRightWnd::OnToolTipNotify(UINT id, NMHDR * pTTTStruct, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;    
	UINT nID =pTTTStruct->idFrom;			//�õ���Ӧ����ID���п�����HWND 
	CString stip="����������";
	int curPropIndex=m_curPropPage*PROP_PEER_PAGE;
    int curPropIndex_S = m_curPropPage_S * PROP_PEER_PAGE_S;    // PengJiLin, 2010-10-12, �̳�����
    BOOL bIsNormalProp = TRUE;     // ��ͨ����
	_TAG_USERPROP *userProp=NULL;
	int propCount=Glb().userPropLibrary.GetCount();
	if (pTTT->uFlags & TTF_IDISHWND)		//����nID�Ƿ�ΪHWND 
	{ 
		nID = ::GetDlgCtrlID((HWND)nID);	//��HWND�õ�IDֵ����Ȼ��Ҳ����ͨ��HWNDֵ���ж� 
		switch(nID) 
		{	
		case IDC_PROP1:
			break;
		case IDC_PROP2:
			curPropIndex+=1;
			break;
		case IDC_PROP3:
			curPropIndex+=2;
			break;
        case IDC_PROP4:
            curPropIndex+=3;
            break;
        case IDC_PROP5:
            curPropIndex+=4;
            break;
        case IDC_PROP6:
            curPropIndex+=5;
            break;
        case IDC_PROP7:
            curPropIndex+=6;
            break;
        case IDC_PROP8:
            curPropIndex+=7;
            break;
        case IDC_BUTTON_PROP1:
            bIsNormalProp = FALSE;
            break;
        case IDC_BUTTON_PROP2:
            bIsNormalProp = FALSE;
            curPropIndex_S += 1;
            break;
        case IDC_BUTTON_PROP3:
            bIsNormalProp = FALSE;
            curPropIndex_S += 2;
            break;
		default:
			break;
		}
		if(curPropIndex<propCount && TRUE == bIsNormalProp)
		{
			userProp=Glb().userPropLibrary.GetAt(curPropIndex);
			stip.Format("%s [%d]",userProp->szPropName,userProp->nHoldCount);
		}
        else if(curPropIndex_S < propCount && FALSE == bIsNormalProp)
        {
            userProp=Glb().userPropLibrary.GetAt(curPropIndex_S);
            stip.Format("%s",userProp->szPropName);
        }
		stip.Trim();
		lstrcpy(pTTT->lpszText, stip);	//������Ӧ����ʾ�ִ� 
	} 
	return FALSE; 
}

void CRoomRightWnd::DoDataExchange(CDataExchange * pDX)
{

	DDX_Control(pDX, IDC_USER_LIST, m_UserList);
	DDX_Control(pDX, IDC_TALK_MESSAGE, m_TalkMessage);
	DDX_Control(pDX, IDC_HIDE_RIGHT, m_BtHideRight);

	DDX_Control(pDX, IDC_PROPBUY, m_btPropBuy);
	DDX_Control(pDX, IDC_PROPPREV, m_btPropPrev);	
	DDX_Control(pDX, IDC_PROPNEXT, m_btPropNext);
	DDX_Control(pDX, IDC_BUTTON_LEFT_ARROW, m_btPropPrev_S);
	DDX_Control(pDX, IDC_BUTTON_RIGHT_ARROW, m_btPropNext_S);
	DDX_Control(pDX, IDC_PROP1, m_btProp1);
	DDX_Control(pDX, IDC_PROP2, m_btProp2);
	DDX_Control(pDX, IDC_PROP3, m_btProp3);
	DDX_Control(pDX, IDC_PROP4, m_btProp4); // PengJiLin, 2010-10-12
	DDX_Control(pDX, IDC_PROP5, m_btProp5); 
	DDX_Control(pDX, IDC_PROP6, m_btProp6); 
	DDX_Control(pDX, IDC_PROP7, m_btProp7); 
	DDX_Control(pDX, IDC_PROP8, m_btProp8); 


	// PengJiLin, 2010-10-8, �����̳���ť
	DDX_Control(pDX, IDC_BUTTON_PROP1, m_btProp1_S);
	DDX_Control(pDX, IDC_BUTTON_PROP2, m_btProp2_S);
	DDX_Control(pDX, IDC_BUTTON_PROP3, m_btProp3_S);

	__super::DoDataExchange(pDX);

}
bool CRoomRightWnd::DrawViewFrame(CDC *pDC, int iWidth, int iHeight)
{
	CRect ClientRect;
	GetClientRect(&ClientRect);
	CBrush brush;
	brush.CreateSolidBrush(PropBgcolor);
	CBrush *pBrush=pDC->SelectObject(&brush);
	pDC->FillRgn(&m_Rgn, &brush);
	//pDC->FillRect(ClientRect,&brush);
	pDC->SelectObject(pBrush);
	brush.DeleteObject();
	/// �����������������ı��������������ı���

	/// ���
	CGameImageHelper helpspace(&m_rightspace);
	CGameImageHelper helpspace2(&m_rightspace2);
	helpspace2.StretchBlt(pDC->GetSafeHdc(),
		0,0,iWidth, helpspace2.GetHeight(),
		0,0,helpspace2.GetWidth(), helpspace2.GetHeight()
		);
	helpspace2.StretchBlt(pDC->GetSafeHdc(),
		0, iHeight-helpspace2.GetHeight(),iWidth, helpspace2.GetHeight(),
		0,0,helpspace2.GetWidth(), helpspace2.GetHeight()
		);
	helpspace.StretchBlt(pDC->GetSafeHdc(),
		0, helpspace2.GetHeight(), helpspace.GetWidth(), iHeight,
		0, 0, helpspace.GetWidth(), helpspace.GetHeight()
		);
	helpspace.StretchBlt(pDC->GetSafeHdc(),
		iWidth-helpspace.GetWidth(), 0, helpspace.GetWidth(), iHeight,
		0, 0, helpspace.GetWidth(), helpspace.GetHeight()
		);
	/// �û��б��
	CGameImageHelper helpuserlc(&m_userlc);
	CGameImageHelper helpuserrc(&m_userrc);
	CGameImageHelper helpusertc(&m_usertc);
	CGameImageHelper helpuserbc(&m_userbc);
	CGameImageHelper helpuserlt(&m_userlt);
	CGameImageHelper helpuserrt(&m_userrt);
	CGameImageHelper helpuserlb(&m_userlb);
	CGameImageHelper helpuserrb(&m_userrb);
	/// �ı�
	helpuserlc.StretchBlt(pDC->GetSafeHdc(),
		m_rcUserList.left-helpuserlc.GetWidth(),
		m_rcUserList.top,
		helpuserlc.GetWidth(),
		m_rcUserList.Height(),
		0,
		0,
		helpuserlc.GetWidth(),
		helpuserlc.GetHeight());
	helpuserrc.StretchBlt(pDC->GetSafeHdc(),
		m_rcUserList.right,
		m_rcUserList.top,
		helpuserrc.GetWidth(),
		m_rcUserList.Height(),
		0,
		0,
		helpuserrc.GetWidth(),
		helpuserrc.GetHeight());
	helpusertc.StretchBlt(pDC->GetSafeHdc(),
		m_rcUserList.left-helpuserlc.GetWidth()+helpuserlt.GetWidth(),
		m_rcUserList.top-helpusertc.GetHeight(),
		m_rcUserList.Width()+helpuserrc.GetWidth(),
		helpusertc.GetHeight(),
		0,
		0,
		helpusertc.GetWidth(),
		helpusertc.GetHeight());
	helpuserbc.StretchBlt(pDC->GetSafeHdc(),
		m_rcUserList.left-helpuserlc.GetWidth()+helpuserlb.GetWidth(),
		m_rcUserList.bottom, 
		m_rcUserList.Width()+helpuserrc.GetWidth(),
		helpuserbc.GetHeight(),
		0,
		0,
		helpuserbc.GetWidth(),
		helpuserbc.GetHeight());
	/// �Ľ�
	helpuserlt.BitBlt(pDC->GetSafeHdc(),
		m_rcUserList.left-helpuserlc.GetWidth(),
		m_rcUserList.top-helpusertc.GetHeight());
	helpuserrt.BitBlt(pDC->GetSafeHdc(),
		m_rcUserList.right + helpuserrc.GetWidth() - helpuserrt.GetWidth(),
		m_rcUserList.top-helpusertc.GetHeight());
	helpuserlb.BitBlt(pDC->GetSafeHdc(),
		m_rcUserList.left-helpuserlc.GetWidth(),
		m_rcUserList.bottom);
	helpuserrb.BitBlt(pDC->GetSafeHdc(),
		m_rcUserList.right + helpuserrc.GetWidth() - helpuserrb.GetWidth(),
		m_rcUserList.bottom);


	/// �����¼��
	CGameImageHelper helptalklc(&m_talklc);
	CGameImageHelper helptalkrc(&m_talkrc);
	CGameImageHelper helptalktc(&m_talktc);
	CGameImageHelper helptalkbc(&m_talkbc);
	CGameImageHelper helptalklt(&m_talklt);
	CGameImageHelper helptalkrt(&m_talkrt);
	CGameImageHelper helptalklb(&m_talklb);
	CGameImageHelper helptalkrb(&m_talkrb);
	helptalklc.StretchBlt(pDC->GetSafeHdc(),
		m_rcTalkMessage.left-helptalklc.GetWidth(),
		m_rcTalkMessage.top, 
		helptalklc.GetWidth(),
		m_rcTalkMessage.Height(),
		0,
		0,
		helptalklc.GetWidth(),
		helptalklc.GetHeight());
	helptalkrc.StretchBlt(pDC->GetSafeHdc(),
		m_rcTalkMessage.right,
		m_rcTalkMessage.top, 
		helptalkrc.GetWidth(),
		m_rcTalkMessage.Height(),
		0,
		0,
		helptalkrc.GetWidth(),
		helptalkrc.GetHeight());
	helptalktc.StretchBlt(pDC->GetSafeHdc(),
		m_rcTalkMessage.left,
		m_rcTalkMessage.top-helptalktc.GetHeight(), 
		m_rcTalkMessage.Width(),
		helptalktc.GetHeight(),
		0,
		0,
		helptalktc.GetWidth(),
		helptalktc.GetHeight());
	helptalkbc.StretchBlt(pDC->GetSafeHdc(),
		m_rcTalkMessage.left,
		m_rcTalkMessage.bottom, 
		m_rcTalkMessage.Width(),
		helptalkbc.GetHeight(),
		0,
		0,
		helptalkbc.GetWidth(),
		helptalkbc.GetHeight());
	/// �Ľ�
	helptalklt.BitBlt(pDC->GetSafeHdc(),
		m_rcTalkMessage.left-helptalklc.GetWidth(),
		m_rcTalkMessage.top-helptalktc.GetHeight());
	helptalkrt.BitBlt(pDC->GetSafeHdc(),
		m_rcTalkMessage.right + helptalkrc.GetWidth() - helptalkrt.GetWidth(),
		m_rcTalkMessage.top-helptalktc.GetHeight());
	helptalklb.BitBlt(pDC->GetSafeHdc(),
		m_rcTalkMessage.left-helptalklc.GetWidth(),
		m_rcTalkMessage.bottom);
	helptalkrb.BitBlt(pDC->GetSafeHdc(),
		m_rcTalkMessage.right + helptalkrc.GetWidth() - helptalkrb.GetWidth(),
		m_rcTalkMessage.bottom);

    // PengJiLin, 2010-9-28, ���û�е�����������Ҫ���߱���ͼ
    if(0 != pannelHeight)
    {
        CGameImageHelper helpPropBack(&m_propBackImage);


        CDC destdc;
        destdc.CreateCompatibleDC(pDC);
        destdc.SelectObject(helpPropBack);
        ::TransparentBlt(pDC->GetSafeHdc(),m_propBackRect.left, m_propBackRect.top, 
                         m_propBackRect.Width(), m_propBackRect.Height(),
                         destdc.GetSafeHdc(),0,0,helpPropBack.GetWidth(),helpPropBack.GetHeight(),RGB(255,0,255));
    }
    
	return true;
}
//�ػ�����
void CRoomRightWnd::OnNcPaint()
{
	CPaintDC realdc(this);
	realdc.SelectClipRgn(&m_Rgn, RGN_AND);
}
//�ػ�����
void CRoomRightWnd::OnPaint()
{
	bool bVisable=::IsWindowVisible(m_hWnd);
	//--------------------------------------------
	/// ˫�����沿�ִ���
	CPaintDC realdc(this);
	if (!bVisable)
	{
		return;
	}
	realdc.SelectClipRgn(&m_Rgn, RGN_AND);
	CRgn rgn;

	CRect ClientRect, clipRect;
	GetClipBox( realdc.GetSafeHdc(), &clipRect );
	if (!m_Rgn.RectInRegion(&clipRect))
	{
		return;
	}
	GetClientRect(&ClientRect);
	int iWidth = ClientRect.Width();
	int iHeight = ClientRect.Height();

	CBitmap BufBmp;
	CDC dc;
	CRect rc;
	GetClientRect(&rc);
	BufBmp.CreateCompatibleBitmap(&realdc, rc.Width(), rc.Height());
	dc.CreateCompatibleDC(&realdc);
	CBitmap *pOldBmp=dc.SelectObject(&BufBmp);
	CFont *currentFont = realdc.GetCurrentFont();
	CFont *oldFont = dc.SelectObject(currentFont);
	//--------------------------------------------
	//Sleep(500);
	DrawViewFrame(&dc, iWidth, iHeight);

	//--------------------------------------------
	/// ˫������沿�ִ���
	realdc.BitBlt(
		0,
		0,
		iWidth, 
		iHeight,
		&dc,
		0,
		0,
		SRCCOPY);
	BufBmp.DeleteObject();
	dc.SelectObject(oldFont);
	currentFont->DeleteObject();
	dc.DeleteDC();
	rgn.DeleteObject();
	/// ˫������沿�ִ���
	//--------------------------------------------
}

////λ����Ϣ
void CRoomRightWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	////MoveWindow(0,0,cx,cy);
	FixControlStation();
}

//����
void CRoomRightWnd::LoadSkin(void)
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("RoomRightWnd"); 
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());
	//���������
	wsprintf(path,"%sroom_right\\room_userlc.bmp",skinfolder);
	m_userlc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_right\\room_userrc.bmp",skinfolder);
	m_userrc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_right\\room_userbc.bmp",skinfolder);
	m_userbc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_right\\room_usertc.bmp",skinfolder);
	m_usertc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//��������Ľ�
	wsprintf(path,"%sroom_right\\room_userlt.bmp",skinfolder);
	m_userlt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_right\\room_userrt.bmp",skinfolder);
	m_userrt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_right\\room_userlb.bmp",skinfolder);
	m_userlb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_right\\room_userrb.bmp",skinfolder);
	m_userrb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//�����
	wsprintf(path,"%sroom_right\\room_talklc.bmp",skinfolder);
	m_talklc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_right\\room_talkrc.bmp",skinfolder);
	m_talkrc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_right\\room_talktc.bmp",skinfolder);
	m_talktc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_right\\room_talkbc.bmp",skinfolder);
	m_talkbc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//�����
	wsprintf(path,"%sroom_right\\room_talklt.bmp",skinfolder);
	m_talklt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_right\\room_talkrt.bmp",skinfolder);
	m_talkrt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_right\\room_talklb.bmp",skinfolder);
	m_talklb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_right\\room_talkrb.bmp",skinfolder);
	m_talkrb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//�߽�
	wsprintf(path,"%sroom_right\\room_rightspace.bmp",skinfolder);
	m_rightspace.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%sroom_right\\room_rightspace2.bmp",skinfolder);
	m_rightspace2.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//���ذ�ť
	wsprintf(path,"%sroom_right\\hide_bt.bmp",skinfolder);
	m_BtHideRight.LoadButtonBitmap(path,false);

	/// �������
	wsprintf(path,"%sroom_right\\PropBuy.bmp",skinfolder);
	m_btPropBuy.LoadButtonBitmap(path,false);
	/// ��һ������
	wsprintf(path,"%sroom_right\\PropPrev.bmp",skinfolder);
	m_btPropPrev.LoadButtonBitmap(path,false);
	/// �޵���ʱ��ʾ�˰�ť
	wsprintf(szPropEmptyImage,"%sroom_right\\PropEmpty.bmp",skinfolder);
	/// ��һ������
	wsprintf(path,"%sroom_right\\PropNext.bmp",skinfolder);
	m_btPropNext.LoadButtonBitmap(path,false);
		
	//�������
	//m_UserList.LoadSkin();

	//���λ�ĸ߶�
	m_advHeight=f.GetKeyVal(key,"GameRoomAdvHeight",0);
	if(m_strAdvertise=="")
		m_advHeight=0;
	//////////////////////////////////////////////////////////////////////////
	//����ע�ͱ���
	if(m_advHeight>0)
	{
		//m_advHeight+=m_rightspace2.GetHeight();
		//m_advHeight+=m_userbc.GetHeight();
		//m_advHeight+=m_talktc.GetHeight();
		webWnd->Browse(m_strAdvertise);
	}
	else
		webWnd->MoveWindow(-100,-100,1,1);

	//
	//////////////////////////////////////////////////////////////////////////
	//�����
	int r,g,b;
	r=f.GetKeyVal(key,"bkr",45);
	g=f.GetKeyVal(key,"bkg",80);
	b=f.GetKeyVal(key,"bkb",133);
	m_colorresource.m_crTalkBkColor=RGB(r,g,b);
	m_TalkMessage.m_bkColor=RGB(r,g,b);
	m_TalkMessage.SetBackgroundColor(false,RGB(r,g,b));
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
	r=f.GetKeyVal(key,"talk_r",255);
	g=f.GetKeyVal(key,"talk_g",255);
	b=f.GetKeyVal(key,"talk_b",255);
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

	//����
	r=f.GetKeyVal(key,"newscolorr",255);
	g=f.GetKeyVal(key,"newscolorg",255);
	b=f.GetKeyVal(key,"newscolorb",255);
	m_colorresource.m_crNewsTxColor=RGB(r,g,b);		
	//���������
	r=f.GetKeyVal(key,"roomc4r",0);
	g=f.GetKeyVal(key,"roomc4g",0);
	b=f.GetKeyVal(key,"roomc4b",0);
	m_ChatControl.SetBkColor(RGB(r,g,b));
	//��ֱ��
	TCHAR path1[MAX_PATH],path2[MAX_PATH], path3[MAX_PATH];
	wsprintf(path1,"%sroom_right\\face.bmp",skinfolder);
	wsprintf(path2,"%sroom_right\\sendtalk.bmp",skinfolder);
	wsprintf(path3,"%sroom_right\\boardcast.bmp",skinfolder);
	m_ChatControl.SetImage(path1,path2, path3);
	m_MessageHandle.SetColor(m_colorresource);

	//r=(COLORREF)f.GetKeyVal(key,"PropBgColor",16771023);
	//g=f.GetKeyVal(key,"PropBgG",231);
	//b=f.GetKeyVal(key,"PropBgB",255);
	PropBgcolor=(COLORREF)f.GetKeyVal(key,"PropBgColor",16771023);

	yPropRow1 = f.GetKeyVal(key,"PropRow1Y", 40);
	xPropBuy=f.GetKeyVal(key,"PropBuyX",10);
	xPropPrev=f.GetKeyVal(key,"PropPrevX",20);
	xProp1=f.GetKeyVal(key,"Prop1X",30);
	xProp2=f.GetKeyVal(key,"Prop2X",40);
	xProp3=f.GetKeyVal(key,"Prop3X",50);
    xProp4=f.GetKeyVal(key,"Prop4X",60);    // PengJiLin, 2010-10-11
	xPropNext=f.GetKeyVal(key,"PropNextX",60);

	/// ���߰�ť����һ���߶�
	pannelHeight = f.GetKeyVal(key, "PannelHeight", 0);

    // PengJiLin, 2010-9-28
    wsprintf(path,"%sroom_right\\probBack.bmp",skinfolder);
    m_propBackImage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

    // PengJiLin, 2010-10-8, ���ڵ������ĸ߶�
    m_adjPropHeight = f.GetKeyVal(key,"adjPropHeight",0);

    // PengJiLin, 2010-10-8, ����ͼ��ʾλ��
    m_iBackImageLeft = f.GetKeyVal(key,"BackImageLeft", 4);
    m_iBackImageWidth = f.GetKeyVal(key,"BackImageWidth", 222);
    m_iBackImageHeight = f.GetKeyVal(key,"BackImageHeight", 41);


    // PengJiLin, 2010-10-8, �̳����߰�ť
    wsprintf(path,"%sroom_right\\PropPrev.bmp",skinfolder);
    m_btPropPrev_S.LoadButtonBitmap(path,false);
    wsprintf(path,"%sroom_right\\PropNext.bmp",skinfolder);
    m_btPropNext_S.LoadButtonBitmap(path,false);

    // PengJiLin, 2010-10-11, ����ͼ
    wsprintf(path, TEXT("%s\\Room_Right\\Prop%d.bmp"), skinfolder, 1);
    m_btProp1_S.LoadButtonBitmap(path,false);
    wsprintf(path, TEXT("%s\\Room_Right\\Prop%d.bmp"), skinfolder, 2);
    m_btProp2_S.LoadButtonBitmap(path,false);
    wsprintf(path, TEXT("%s\\Room_Right\\Prop%d.bmp"), skinfolder, 3);
    m_btProp3_S.LoadButtonBitmap(path,false);

    // PengJiLin, 2010-10-11
	yPropRow2 = f.GetKeyVal(key,"PropRow2Y", 100);
    xPropPrev_S=f.GetKeyVal(key,"sPropPrevX",50);
    xProp1_S=f.GetKeyVal(key,"sProp1X",70);
    xProp2_S=f.GetKeyVal(key,"sProp2X",110);
    xProp3_S=f.GetKeyVal(key,"sProp3X",150);
    xPropNext_S=f.GetKeyVal(key,"sPropNextX",195);


	//�������
	//m_UserList.LoadSkin();

}

//������ɫ
LRESULT CRoomRightWnd::OnLoadSkin(WPARAM wparam, LPARAM lparam)
{
	int sk=(int)wparam;
	CString str;
	int cursk;
	if(sk>4||sk<0)
		return false;

	cursk=m_skinmgr.GetKeyVal(m_skinmgr.GetSkinBcfFileName()).GetAt(4);
	cursk -='0';
	if(cursk==sk)
		return false;

	str.Format("skin%d",sk);
	Glb().m_skin=str;

	CString s=CBcfFile::GetAppPath ();
	CBcfFile f( s + "bzgame.bcf");
	CString m_key="BZW";
	f.SetKeyValString(m_key,"skin",str);

	m_strAdvertise=f.GetKeyVal(m_key,"RoomAdvertise","");
	LoadSkin();
	return true;
}
/// ��д�ĵ�����С����
bool CRoomRightWnd::FixControlStation(bool bShow)
{
	if(!m_bInit)// || !m_BtHideRight.m_hWnd)
		return true;
	/// ȡ�ÿͻ�����С
	/// ȷ���Ӵ��ڵ�λ�ã����߿��λ��
	/// ȷ����Ҫ���»��Ƶ������Ӵ��ھͲ���Ҫ����
	CRect clientRect;
	GetClientRect(&clientRect);
	m_Rgn.SetRectRgn(&clientRect);
	CRgn rgn;
	rgn.CreateRectRgn(0,0,0,0);
	int iWidth = clientRect.Width();
	int iHeight = clientRect.Height();

	/// ���ذ�ť
	CRect btnrect;
	m_BtHideRight.GetClientRect(&btnrect);
	int width = btnrect.Width();
	int height=btnrect.Height();
	m_BtHideRight.MoveWindow((m_rightspace.GetWidth()-width)/2,(iHeight-height)/2,width,height);
	m_BtHideRight.ShowWindow(SW_HIDE);
	/// ������������û��б����������ܸ߶�
	int nUserAndTalkHeight = iHeight 
		- m_rightspace.GetWidth() * 2	///< ���������߿�
		- m_usertc.GetHeight()			///< �û����ϱ�
		- m_userbc.GetHeight()			///< �û����±�
		- m_talktc.GetHeight()			///< ������ϱ�
		- m_talkbc.GetHeight()			///< ������±�
		- m_advHeight					///< ��洰�ڸ߶�
		- m_adjPropHeight				// PengJiLin, 2010-10-8, �������ĸ߶����ӣ����Լ����û����ĸ߶�
		- pannelHeight;					///< ����һ����

	/// �û��б��
	m_rcUserList.left = m_userlc.GetWidth() + m_rightspace.GetWidth();
	m_rcUserList.top = m_rightspace2.GetHeight() + m_usertc.GetHeight();
	m_rcUserList.right = iWidth - m_userrc.GetWidth() - m_rightspace.GetWidth();
	m_rcUserList.bottom = nUserAndTalkHeight/2 + m_rcUserList.top;						//modify by wyl  11-5-13
	m_UserList.MoveWindow(&m_rcUserList);
	rgn.SetRectRgn(&m_rcUserList);
	rgn.CombineRgn(&rgn, &m_Rgn, RGN_AND);
	m_Rgn.CombineRgn(&m_Rgn, &rgn, RGN_XOR);
	/// ���������
	m_rcSendTalk.left = m_rightspace.GetWidth()+m_talklc.GetWidth();
	m_rcSendTalk.right = iWidth - m_rightspace.GetWidth()- m_talkrc.GetWidth();
	m_rcSendTalk.bottom = iHeight - m_rightspace2.GetWidth();
	m_rcSendTalk.top = m_rcSendTalk.bottom - m_talkbc.GetHeight();
	m_ChatControl.MoveWindow(&m_rcSendTalk);
	/// �����
	m_rcTalkMessage.left = m_talklc.GetWidth() + m_rightspace.GetWidth();
	m_rcTalkMessage.top = m_rcSendTalk.top - (nUserAndTalkHeight - m_rcUserList.Height());
	m_rcTalkMessage.right = iWidth - m_talkrc.GetWidth() - m_rightspace.GetWidth();
	m_rcTalkMessage.bottom = m_rcSendTalk.top;
	m_TalkMessage.MoveWindow(&m_rcTalkMessage);
	rgn.SetRectRgn(&m_rcTalkMessage);
	rgn.CombineRgn(&rgn, &m_Rgn, RGN_AND);
	m_Rgn.CombineRgn(&m_Rgn, &rgn, RGN_XOR);
	//rgn.SetRectRgn(&m_rcSendTalk);
	//rgn.CombineRgn(&rgn, &m_Rgn, RGN_AND);
	//m_Rgn.CombineRgn(&m_Rgn, &rgn, RGN_XOR);
	/// ��洰��
	if(m_advHeight>0)
	{
		m_rcAdvertise.left = m_rightspace.GetWidth()+1;
		m_rcAdvertise.right = iWidth - m_rightspace.GetWidth()-1;
		m_rcAdvertise.top	= m_rcUserList.bottom + m_userbc.GetHeight();
		m_rcAdvertise.bottom= m_rcAdvertise.top + m_advHeight;
		webWnd->MoveWindow(&m_rcAdvertise);
		rgn.SetRectRgn(&m_rcAdvertise);
		rgn.CombineRgn(&rgn, &m_Rgn, RGN_AND);
		m_Rgn.CombineRgn(&m_Rgn, &rgn, RGN_XOR);
	}
	rgn.DeleteObject();


    if(m_advHeight>0)
    {
        // PengJiLin, 2010-9-28, ���߱���ͼλ��
        // �Թ�洰��Ϊ�ο�
        CRect rectweb;
        webWnd->GetWindowRect(&rectweb);
        ScreenToClient(&rectweb);

        m_propBackRect.left = m_iBackImageLeft;
        m_propBackRect.top = rectweb.bottom + 2;
        m_propBackRect.right = m_propBackRect.left + m_iBackImageWidth;
        m_propBackRect.bottom = m_propBackRect.top + m_iBackImageHeight;
    }
    

	/// �����߰�ť��λ�����û��б��������Ϣ֮�����
    // PengJiLin, 2010-10-11, ���߰�ť�ڵ��߱���ͼ��
    int iDistY = 7;
	int t= m_propBackRect.top + yPropRow1;/*m_rcAdvertise.bottom + (pannelHeight-34)/2 + 8*/;
	m_btPropBuy.SetWindowPos(NULL,xPropBuy,t,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btPropPrev.SetWindowPos(NULL,xPropPrev,t+iDistY,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btProp1.SetWindowPos(NULL,xProp1,t,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btProp2.SetWindowPos(NULL,xProp2,t,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btProp3.SetWindowPos(NULL,xProp3,t,0,0,SWP_NOSIZE|SWP_NOZORDER);
    m_btProp4.SetWindowPos(NULL,xProp4,t,0,0,SWP_NOSIZE|SWP_NOZORDER);  // PengJiLin, 2010-10-11

	m_btPropNext.SetWindowPos(NULL,xPropNext,t+iDistY,0,0,SWP_NOSIZE|SWP_NOZORDER);

    // PengJiLin, 2010-10-8, �̳����߰�ť
    CRect propRect;
    m_btProp1_S.GetWindowRect(&propRect);

    t = m_propBackRect.top + yPropRow2;

	m_btProp5.SetWindowPos(NULL,xProp1,t,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btProp6.SetWindowPos(NULL,xProp2,t,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btProp7.SetWindowPos(NULL,xProp3,t,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btProp8.SetWindowPos(NULL,xProp4,t,0,0,SWP_NOSIZE|SWP_NOZORDER);

    m_btPropBuy.SetWindowPos(NULL,xPropBuy,t,0,0,SWP_NOSIZE|SWP_NOZORDER);    

    m_btProp1_S.SetWindowPos(NULL,xProp1_S,t,0,0,SWP_NOSIZE|SWP_NOZORDER);
    m_btProp2_S.SetWindowPos(NULL,xProp2_S,t,0,0,SWP_NOSIZE|SWP_NOZORDER);
    m_btProp3_S.SetWindowPos(NULL,xProp3_S,t,0,0,SWP_NOSIZE|SWP_NOZORDER);
    m_btPropPrev_S.SetWindowPos(NULL,xPropPrev_S,t+iDistY,0,0,SWP_NOSIZE|SWP_NOZORDER);
    m_btPropNext_S.SetWindowPos(NULL,xPropNext_S,t+iDistY,0,0,SWP_NOSIZE|SWP_NOZORDER);

	return true;
}


//�����б�ť
void CRoomRightWnd::OnBnClickedHideList()
{
	////���������
	m_bHideList=!m_bHideList;

	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());
	if(!m_bHideList)
		wsprintf(path,"%sroom_right\\hide_bt.bmp",skinfolder);
	else
		wsprintf(path,"%sroom_right\\show_bt.bmp",skinfolder);

	m_BtHideRight.LoadButtonBitmap(path,false);

	this->GetParent()->SendMessage(IDM_HIDE_RIGHT,m_bHideList,m_bHideList);
	FixControlStation();

	return ;
}

//�˵�����
BOOL CRoomRightWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_SKIN:
	case IDM_SKIN+1:
	case IDM_SKIN+2:
	case IDM_SKIN+3:
	case IDM_SKIN+4:				//�����������
		{
			int basesk=IDM_SKIN;
			//int sk=LOWORD(wParam)-basesk;
			//CNewGameClass * pRoomTab=&((CGamePlaceDlg *)AfxGetMainWnd())->m_RoomTabWnd;
			//pRoomTab->PostMessage(WM_LOADSKIN,(WPARAM)sk,0);
			//�����Ƥ��
			//CActiveInformClass * pInform=&((CGamePlaceDlg *)AfxGetMainWnd())->m_HallInform;
			//pInform->PostMessage(WM_LOADSKIN,(WPARAM)sk,0);
		return true;
		}
	default:
		break;
	}
	return __super::OnCommand(wParam, lParam);
}

BOOL CRoomRightWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		m_ChatControl.OnBnClickedSend();
		return true;
	}
	if(pMsg->message==WM_USER+151)
	{
		OnSetPropImage();
		return TRUE;
	}
	if(pMsg->message==WM_USER+152)
	{
		UserProp((int)pMsg->wParam);
		return TRUE;
	}
	if(m_ToolTip.m_hWnd!=NULL) 
	m_ToolTip.RelayEvent(pMsg); 

	return CGameFaceGo::PreTranslateMessage(pMsg);
}


LRESULT CRoomRightWnd::OnReLoadTalkINI(WPARAM wparam, LPARAM lparam)
{
	//���ԭ������
	m_ChatControl.m_InputMessage.ResetContent();
	//��talkInGame.ini�ļ��ж�ȡ������
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "talk.bcf");
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
			m_ChatControl.m_InputMessage.AddString(szTalk);
	}while(!szTalk.IsEmpty());
	return 0;
}
void CRoomRightWnd::OnSetPropImage(void)
{
	int propCout=Glb().userPropLibrary.GetCount();

	//���÷�ҳ
	if(m_curPropPage*PROP_PEER_PAGE>=propCout)
		m_curPropPage--;
	if(m_curPropPage<0)
		m_curPropPage=0;
	if(m_curPropPage>0)
	{
		m_btPropPrev.EnableWindow(TRUE);
		m_btPropPrev_S.EnableWindow(TRUE);
	}
	else
	{
		m_btPropPrev.EnableWindow(FALSE);
		m_btPropPrev_S.EnableWindow(FALSE);
	}

	if((m_curPropPage+1)*PROP_PEER_PAGE<propCout)
	{
		m_btPropNext.EnableWindow(TRUE);
		m_btPropNext_S.EnableWindow(TRUE);
	}
	else
	{
		m_btPropNext.EnableWindow(FALSE);
		m_btPropNext_S.EnableWindow(FALSE);
	}

	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("RoomRightWnd");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());
//	wsprintf(path1,"%sroom_right\\face.bmp",skinfolder);

	_TAG_USERPROP *userProp=NULL;

	TCHAR szPropImage[MAX_PATH]={0};
	for(int i=0;i<PROP_PEER_PAGE;i++)
	{
		int curPropIndex=m_curPropPage*PROP_PEER_PAGE+i;
		CGameImage img;
		HRGN hrgn;
		CNormalBitmapButtonEx *pButton=NULL;
		
		switch(i)
		{
		case 0: pButton=(CNormalBitmapButtonEx*)GetDlgItem(IDC_PROP1);break;//ȡ����BUTTON
		case 1: pButton=(CNormalBitmapButtonEx*)GetDlgItem(IDC_PROP2);break;
		case 2: pButton=(CNormalBitmapButtonEx*)GetDlgItem(IDC_PROP3);break;
		case 3: pButton=(CNormalBitmapButtonEx*)GetDlgItem(IDC_PROP4);break;
		case 4: pButton=(CNormalBitmapButtonEx*)GetDlgItem(IDC_PROP5);break;
		case 5: pButton=(CNormalBitmapButtonEx*)GetDlgItem(IDC_PROP6);break;
		case 6: pButton=(CNormalBitmapButtonEx*)GetDlgItem(IDC_PROP7);break;
		case 7: pButton=(CNormalBitmapButtonEx*)GetDlgItem(IDC_PROP8);break;
		}

        // PengJiLin, 2010-9-27
        pButton->EnableToShowText(0, false);

		if(curPropIndex<propCout)
		{
            //����е���
            pButton->EnableWindow(TRUE);    // PengJiLin, 2010-10-12
			userProp=Glb().userPropLibrary.GetAt(curPropIndex);
			//wsprintf(szPropImage, TEXT("%s\\Room_Right\\Prop%d.bmp"), skinfolder, (userProp->attribAction&0x7F000000)>>24);
			wsprintf(szPropImage, TEXT("%s\\Room_Right\\Prop%d.bmp"), skinfolder, curPropIndex+1);
			pButton->LoadButtonBitmap(szPropImage,false);
			/*if (!IsWindow(m_ToolTip.m_hWnd))
			{
				m_ToolTip.UpdateTipText(userProp->szPropName,this,m_BtPropArray[i]);
			}*/

            // PengJiLin, 2010-9-27
            pButton->EnableToShowText(userProp->nHoldCount, true);
		}
		else
		{
            //���û�е���
            pButton->EnableWindow(FALSE);   // PengJiLin, 2010-10-12
			pButton->LoadButtonBitmap(szPropEmptyImage,false);
			/*if (!IsWindow(m_ToolTip.m_hWnd))
			{
				m_ToolTip.UpdateTipText("����������",this);
			}*/
		}

		hrgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hrgn)
		{
			pButton->SetWindowRgn(hrgn,true);
			DeleteObject(hrgn);
		}
	}
}

// PengJiLin, 2010-10-12, �̳����߹���
void CRoomRightWnd::OnSetPropImage_S(void)
{
    int propCout=Glb().userPropLibrary.GetCount();

	m_BtPropArray[0] = IDC_BUTTON_PROP1;
	m_BtPropArray[1] = IDC_BUTTON_PROP2;
	m_BtPropArray[2] = IDC_BUTTON_PROP3;

	//���÷�ҳ
	if(m_curPropPage_S*PROP_PEER_PAGE_S>=propCout)
		m_curPropPage_S--;
	if(m_curPropPage_S<0)
		m_curPropPage_S=0;
	if(m_curPropPage_S>0)
		m_btPropPrev_S.EnableWindow(TRUE);
	else
		m_btPropPrev_S.EnableWindow(FALSE);

	if((m_curPropPage_S+1)*PROP_PEER_PAGE_S<propCout)
		m_btPropNext_S.EnableWindow(TRUE);
	else
		m_btPropNext_S.EnableWindow(FALSE);

	CString s=CBcfFile::GetAppPath ();
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("RoomRightWnd");
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());

	_TAG_USERPROP *userProp=NULL;

	TCHAR szPropImage[MAX_PATH]={0};
	for(int i=0;i<PROP_PEER_PAGE_S;i++)
	{
		int curPropIndex=m_curPropPage_S*PROP_PEER_PAGE_S+i;
		CGameImage img;
		HRGN hrgn;
		CNormalBitmapButtonEx *pButton=NULL;
		pButton=(CNormalBitmapButtonEx*)GetDlgItem(m_BtPropArray[i]);   // ȡ����BUTTON

        if(curPropIndex<propCout)   // �е���
        {
            pButton->EnableWindow(TRUE);
            userProp=Glb().userPropLibrary.GetAt(curPropIndex);
            wsprintf(szPropImage, TEXT("%s\\Room_Right\\Prop%d.bmp"), skinfolder, 
                (userProp->attribAction&0x7F000000)>>24);
            pButton->LoadButtonBitmap(szPropImage,false);            
        }
        else                        // û�е���
        {
            pButton->EnableWindow(FALSE);
            pButton->LoadButtonBitmap(szPropEmptyImage,false);
        }
        hrgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
        if(hrgn)
        {
            pButton->SetWindowRgn(hrgn,true);
            DeleteObject(hrgn);
        }
	}
}

void CRoomRightWnd::OnBnClickedPropBuy()
{
	m_pMainWnd->PostMessage(WM_USER+150,1,0);
}
//���ݵ��ߺţ�����λ����ͬ�ĵ���
void CRoomRightWnd::OnPropBuy(UINT uPropId)
{
	m_pMainWnd->PostMessage(WM_USER+150,uPropId,0);

}
// PengJiLin, 2010-10-12, �̳����߹���
void CRoomRightWnd::OnBnClickedPropPrev()
{
	m_curPropPage--;
	OnSetPropImage();
}

void CRoomRightWnd::OnBnClickedPropNext()
{
	m_curPropPage++;
	OnSetPropImage();
}

void CRoomRightWnd::OnBnClickedLeftArrow(void)
{
	m_curPropPage--;
	OnSetPropImage();
}

void CRoomRightWnd::OnBnClickedRightArrow(void)
{
	m_curPropPage++;
	OnSetPropImage();
}

void CRoomRightWnd::OnBnClickedProp1()
{
	m_pMainWnd->PostMessage(WM_USER+150,1,1+(m_curPropPage*PROP_PEER_PAGE));
}

void CRoomRightWnd::OnBnClickedProp2()
{
	m_pMainWnd->PostMessage(WM_USER+150,1,2+(m_curPropPage*PROP_PEER_PAGE));
}

void CRoomRightWnd::OnBnClickedProp3()
{
	m_pMainWnd->PostMessage(WM_USER+150,1,3+(m_curPropPage*PROP_PEER_PAGE));
}

void CRoomRightWnd::OnBnClickedProp4()
{
	m_pMainWnd->PostMessage(WM_USER+150,1,4+(m_curPropPage*PROP_PEER_PAGE));
}

void CRoomRightWnd::OnBnClickedProp5()
{
	m_pMainWnd->PostMessage(WM_USER+150,1,5+(m_curPropPage*PROP_PEER_PAGE));
}

void CRoomRightWnd::OnBnClickedProp6()
{
	m_pMainWnd->PostMessage(WM_USER+150,1,6+(m_curPropPage*PROP_PEER_PAGE));
}

void CRoomRightWnd::OnBnClickedProp7()
{
	m_pMainWnd->PostMessage(WM_USER+150,1,7+(m_curPropPage*PROP_PEER_PAGE));
}

void CRoomRightWnd::OnBnClickedProp8()
{
	m_pMainWnd->PostMessage(WM_USER+150,1,8+(m_curPropPage*PROP_PEER_PAGE));
}
// PengJiLin, 2010-10-12, �̳����߹���
void CRoomRightWnd::OnBnClickedBtnProp1S(void)
{
    BuyAndUseProp(m_curPropPage_S*PROP_PEER_PAGE_S+0);
}
void CRoomRightWnd::OnBnClickedBtnProp2S(void)
{
    BuyAndUseProp(m_curPropPage_S*PROP_PEER_PAGE_S+1);
}
void CRoomRightWnd::OnBnClickedBtnProp3S(void)
{
    BuyAndUseProp(m_curPropPage_S*PROP_PEER_PAGE_S+2);
}

// PengJiLin, 2010-10-12, �̳����߹��ܣ�������
void CRoomRightWnd::BuyAndUseProp(int nPropIndex)
{
    int propCount=Glb().userPropLibrary.GetCount();
    if(nPropIndex < 0 || nPropIndex >= propCount)
    {
        return;
    }

    _TAG_USERPROP *userProp=Glb().userPropLibrary.GetAt(nPropIndex);
    if(NULL == userProp) return;

    if(userProp->nHoldCount > 0)    // ��������Ϊ0��ʹ�ü����ù���
    {
        TCHAR chInfo[256] = {0};
        wsprintf(chInfo, "[%s]��������Ϊ0������ʹ���̵�ļ����ù���", userProp->szPropName);
        m_MessageHandle.InsertSystemMessage(chInfo);  
        return;
    }

    // ���͹�����Ϣ
    CGameRoomEx* pGameRoomEx = (CGameRoomEx*)m_pMainWnd;
    CMainRoomEx* pMainRoomEx = GetMainRoom();
    _TAG_PROP_BUY propBuy;
    propBuy.dwUserID = pMainRoomEx->m_PlaceUserInfo.dwUserID;
    propBuy.nPropID = userProp->nPropID;
    propBuy.iPropPayMoney = 0;
    propBuy.nPropBuyCount = 1;

    // 0��ʾ�ڷ��乺��1��ʾ����Ϸ�˹���
    pMainRoomEx->SendData(&propBuy,sizeof(_TAG_PROP_BUY),MDM_GP_PROP,ASS_PROP_BUY_NEW,0);

}

void CRoomRightWnd::UserProp(int nPropIndex)
{
	int propCount=Glb().userPropLibrary.GetCount();
	if(nPropIndex<0 || nPropIndex>=propCount)
	{
		m_pMainWnd->PostMessage(WM_USER+150,1,0);
		return;
	}

	_TAG_USERPROP *userProp=Glb().userPropLibrary.GetAt(nPropIndex);
	if(userProp==NULL) return;	

    // PengJiLin, 2010-10-14, ���ߵ���������Ϊ0���봦��
    if(userProp->nHoldCount <= 0)
    {
        TCHAR chInfo[256] = {0};
        wsprintf(chInfo, "��Ǹ��[%s]�Ѿ�ʹ����ϣ��Ƿ����ڹ���ʹ��?", userProp->szPropName);
        //if(IDOK == AFCMessageBox(chInfo, TEXT("��ʾ"), AFC_YESNO))  // ȷ������
        if(IDYES == DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONINFORMATION, "��ʾ",true,chInfo))  // ȷ������
        {
            BuyAndUseProp(nPropIndex);
            return;
        }
        else        // ȡ������
        {
            return;
        }
    }

	if(userProp->attribAction & 0x80 || userProp->attribAction &0x100)
	{//�����Ⱥ�С����
		m_pMainWnd->PostMessage(WM_USER+156,(WPARAM)userProp,0);
		return;
	}

	if(userProp->attribAction&0x1)
	{
		//AFCMessageBox("�Բ��𣬸õ���ֻ������Ϸ��ʹ��","��ʾ");
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�Բ��𣬸õ���ֻ������Ϸ��ʹ��");
		return;
	}


	//���Լ�ʹ��
	////////////////////////////////////////////////////
	///2008 1216 Kylin ����ʱʹ�ø�������û����ʾ
	//���Լ�ʹ��
	//CString stip;
	//stip.Format("��ȷ��Ҫʹ�õ��� %s ��?",userProp->szPropName);
	//if(userProp->attribAction & 0x2)
	//{
	//	if(GetMeUserInfo()->dwPoint>=0)
	//	{
	//		stip.Format("���Ļ���Ϊ����ʹ�õ��� %s ����������һ�����߶�û��Ч����ȷ��ʹ����?",userProp->szPropName);
	//	}
	//	else stip+="\n�Ὣ���ĸ���������!";
	//}


	//if(AFCMessageBox(stip,"��ʾ",AFC_YESNO)==IDOK)
		m_pMainWnd->PostMessage(WM_USER+155,(WPARAM)userProp);
	////////////////////////////////////////////////////

	return;
}


void CRoomRightWnd::UserProp_Buy(int nPropIndex)
{
	m_ChatControl.UseProp_Buy(nPropIndex);
}

void CRoomRightWnd::SetGameRoomWnd(CWnd* pWnd)
{
	m_pGameRoom = pWnd;
	m_ChatControl.SetGameRoomWnd(pWnd);
}