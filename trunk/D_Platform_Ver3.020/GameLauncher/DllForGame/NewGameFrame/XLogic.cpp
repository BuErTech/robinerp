#include "stdafx.h"
#include "XLogic.h"
#include "IPC\ChannelService.h"
#include "UI_ForLogic/IUserInterface.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "Common/UIMessageDefines.h"
#include "IGameImpl.h"
#include "igameengine.h"
#include "LoveSendClassInExe.h"
#include "gameplacemessage.h"
#include "IMMessage.h"
#include "IMSetUser.h"
#include "Platform/AFCDevHead.h"
#include "bzsound.h"
#include "XRightFrame.h"			///��Ϸ�ұ߿�
#include "../DllForGame/UseBoardCastWnd.h"

extern void DebugPrintfA(const char *p, ...);

#define GETCTRL(CTRL_TYPE,PRE,pUI,CTRL_ID)\
    PRE = dynamic_cast<CTRL_TYPE *>(pUI->GetIControlbyID(CTRL_ID));\

XLogic *g_pXLogic = NULL;

//Avatarϵͳ���
#define BzGetAvatarIndex(obj) \
	GetAvatarIndex((obj)->GameUserInfo.bLogoID,(obj)->GameUserInfo.i64Bank,(obj)->GameUserInfo.i64Money,\
	(obj)->GameUserInfo.dwPoint,m_GameInfo.uNameID,m_GameInfo.uComType)

void char2wchar(char *pchar,wchar_t * tchar) 
{
	int   nwLen  =MultiByteToWideChar(CP_ACP,0,pchar,-1,NULL,0); 
	MultiByteToWideChar(CP_ACP,0,pchar,-1,tchar,nwLen);
}
//Ĭ���������п��
const TCHAR COLOMN_NAME[][14] = 
{
	"�ǳ�,100", "���,60","�Ƹ�����,90", "����,60", "���ּ���,90", "����,60", "��������,90", "�Ա�,40",
	"�ܾ���,50", "ʤ����,50", "�����,50", "�;���,50", "������,50","�Թ۶���,100"
};

//�ж�һ���ļ��Ƿ����
bool FileExists(LPCTSTR lpszFileName,bool bIsDirCheck)
{
	DWORD dwAttributes = GetFileAttributes(lpszFileName);
	if(dwAttributes == 0xFFFFFFFF)
	{
		return false;
	}
	if((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		return bIsDirCheck;
	}
	else
	{
		return !bIsDirCheck;
	}
}

XGameLogicBase* CreateGameLogic()
{
    if (NULL == g_pXLogic)
    {
        g_pXLogic = new XLogic();
    }    
    return g_pXLogic;
}
void DestroyGameLogic(XGameLogicBase **ppLogic)
{
    if (*ppLogic != NULL)
    {
        delete *ppLogic;
        *ppLogic = NULL;
    }
}


XLogic::XLogic()
{
    m_pIPC = NULL;
    m_hGameInstance = NULL;
    m_pGameImpl = NULL;
    m_pIUI = NULL;
    m_hWnd = NULL;
    m_dwIPCHeartBeatTick = 0;
    m_pEngine = NULL;
    m_nPropPage = 0;

    memset(m_pPropContain, 0, sizeof(m_pPropContain));

    m_pPropPannel = NULL;
    m_pPropSelUser = NULL;
    m_dlgBroadcast = NULL;
    m_pPersonBank = NULL;

    m_hAudioMod = NULL;
    m_pBackAudio = NULL;

    pCreateAudioPlay = NULL;
    pDeleteAudioPlay = NULL;

    char psz[255]={0};
    sprintf(psz, "Audio.dll");
    m_hAudioMod = ::LoadLibrary(psz);
    if (m_hAudioMod)
    {
        pCreateAudioPlay = (CREATEAUDIOPLAYC)::GetProcAddress(m_hAudioMod, "CreateAudioPlayC");
        pDeleteAudioPlay = (DELETEAUDIOPLAYC)::GetProcAddress(m_hAudioMod, "DeleteAudioPlayC");
    }
    if (pCreateAudioPlay)
    {
        m_pBackAudio = pCreateAudioPlay();
    }

    m_NetSignalLevel=0;
    m_SavedSingalIndex=0;     //��һ�η��͵�������Ϣ�����ţ��յ���Ϣ����m_NetSingalIndexͬ��
    m_NetSignalIndex=0;

	m_bIsAutoScroll=true;
	m_bIsAcceptChatMsg=true;

	//�ڶ�����Ϸ���жԻ���
	m_pBank2Wnd = NULL;	

	//��ȡԭ���ڳߴ�
	m_nWiondwMetricsX = ::GetSystemMetrics(SM_CXSCREEN); 
	m_nWiondwMetricsY = ::GetSystemMetrics(SM_CYSCREEN);

	m_pPmdWnd = NULL;

	m_bIsQueueDisMiss = false;

	m_pRightFrame = NULL;

	//�û��б�
	m_pUserListDu = NULL;	

	//��ʾ�ұ߿�İ�ť
	m_pShowRightFrameBtn = NULL; 
}

XLogic::~XLogic()
{
    if (NULL!=m_hGameInstance && m_pGameImpl!=NULL)
    {
        DESTROYGAMEIMPLEMENT DestroyGame = (DESTROYGAMEIMPLEMENT)GetProcAddress(m_hGameInstance, ("DestroyNewGameImpl"));
        if (DestroyGame!=NULL)
        {
            DestroyGame(&m_pGameImpl);
        }
    }

    delete m_pPropPannel;
    m_pPropPannel = NULL;
    delete m_pPropSelUser;
    m_pPropSelUser = NULL;
    delete m_dlgBroadcast;
    m_dlgBroadcast = NULL;
    delete m_pPersonBank;
    m_pPersonBank = NULL;

    //if (m_pBackAudio)
    //{
    //    if (pDeleteAudioPlay)
    //        pDeleteAudioPlay(&m_pBackAudio);
    //}
    //if (m_hAudioMod)
    //{
    //    ::FreeLibrary(m_hAudioMod);
    //    m_hAudioMod = NULL;
    //}
}


/// ��ʼ��
int XLogic::Initial(LogicInitParam_t &lip)
{ 
    DebugPrintfA("%s", __FUNCTION__);


    for (int i=0; i<m_GameInfo.uDeskPeople; ++i)
    {
        m_IsUserSmall[i] = false;
    }


    ////////////////////////////////////////////////////////////////////////////////
    CBcfFile forder( CBcfFile::GetAppPath() + "..\\Order.bcf");
    Glb().m_nPowerOfGold = forder.GetKeyVal("VirtualMoney", "PowerOfGold", 0);
    // PengJiLin, 2010-8-3, �����ʾ�ָ�������
    Glb().m_bUseSpace = forder.GetKeyVal("VirtualMoney", "UseSpace", 0);
    CString strSpace = forder.GetKeyVal("VirtualMoney", "SpaceChar", " ");
    memcpy(Glb().m_strSpaceChar, strSpace, sizeof(Glb().m_strSpaceChar)*sizeof(TCHAR));
    ////////////////////////////////////////////////////////////////////////////////

	int _bit = forder.GetKeyVal("VirtualMoney", "SpaceBits", 3);
	if (_bit == 0) Glb().m_iMaxBit = 18;
	else Glb().m_iMaxBit = 18 + 18 / _bit - 1;

    m_pEngine = (IGameEngine *)lip.pEngine;
    memcpy(&m_GameInfo, lip.pGameInfo, sizeof(m_GameInfo));
    CLoveSendClassInExe::ReadLevelData();
    m_GameInfo.pOrderName = GetPointOrderName;

	CString str;
	str.Format("DGameL: 1 m_GameInfo.pOrderName = 0x%08x",GetPointOrderName);
	OutputDebugString(str);

	CString s;
	s.Format("xxxdxy Initial %d %I64d", m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID, m_GameInfo.uisMeUserInfo.GameUserInfo.i64Money);
	OutputDebugString(s);


    m_dwIPCHeartBeatTick = GetTickCount();
    m_hWnd = lip.hwndHall;
    m_pIPC = (CChannelService *)lip.pIpc;
    m_pIUI = (IUserInterface *)lip.pUI;

    m_hGameInstance = lip.hInstance;
    if (NULL!=m_hGameInstance)
    {
        /// ��ȡ����ָ��
        CREATEGAMEIMPLEMENT CreateGame= (CREATEGAMEIMPLEMENT)GetProcAddress(lip.hInstance, ("CreateNewGameImpl"));
        if (NULL!=CreateGame)
        {
            m_pGameImpl = CreateGame(dynamic_cast<IGameFrame *>(this), m_pIUI);
        }
    }    

	RECT rcShrink = {0,0,0,0};
	m_pIUI->SetControlsShrinkArea(rcShrink);

    m_pGameImpl->Initial();
    m_pGameImpl->InitUI();

	//�����ҿ��
	//JianGuankun 2012.8.30
	m_pRightFrame = new XRightFrame(this);
	m_pRightFrame->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
	m_pUserListDu = m_pRightFrame->m_pUsersList;

	//��ʾ��Ϸ���Ҵ���
	if (m_pRightFrame)
	{
		m_pRightFrame->HideWindow();
	}

	WINDOWPLACEMENT place;
	m_pEngine->GetCWnd()->GetWindowPlacement(&place);

	IButton *pBtn = NULL ; 

	CBcfFile bcffile(CBcfFile::GetAppPath() + _T("GameOption.bcf"));
	UINT uMaxType = bcffile.GetKeyVal("FrameControl","max_type",1);
	if (0 == uMaxType)
	{
		pBtn = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_SET_FRAME)) ; 
		if(NULL != pBtn)
		{
			pBtn->SetControlVisible(false); 
		}
		pBtn = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_RESET_FRAME)) ; 
		if(NULL != pBtn)
		{
			pBtn->SetControlVisible(false); 
		}
	}
	if(place.showCmd == SW_SHOWMAXIMIZED)
	{
		pBtn = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_SET_FRAME)) ; 
		if(NULL != pBtn)
		{
			pBtn->SetControlVisible(false) ; 
		}
		pBtn = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_RESET_FRAME)) ; 
		if(NULL != pBtn)
		{
			pBtn->SetControlVisible(true) ; 
		}
	}
	else
	{
		pBtn = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_RESET_FRAME)) ; 
		if(NULL != pBtn)
		{
			pBtn->SetControlVisible(false) ; 
		}
		pBtn = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_SET_FRAME)) ; 
		if(NULL != pBtn)
		{
			pBtn->SetControlVisible(true) ; 
		}
	}
    BYTE bMeDeskStation=m_GameInfo.uisMeUserInfo.GameUserInfo.bDeskNO;
    GetOnLineUser(bMeDeskStation);

    UIInitBiaoQingKuang();
    InitPlayerListCtrl();
    InitPropPannel();
    // ��ʼ�����н���
    m_pPersonBank = new CPBankDlg(m_pEngine->GetCWnd());
	m_pPersonBank->m_pLogic = this;

    SetTimer(OLEDRAW_TIMER_ID, OLEDRAW_INTERVAL);

    LoadBkSoundList();

    IImage *pNetSignal = dynamic_cast<IImage*>(m_pIUI->GetIControlbyID(IMAGE_NET_SIGNAL));
    if (pNetSignal)
    {
        pNetSignal->SetSrcY(0);
        pNetSignal->SetControlVisible(true);
    }

    IEditorText* _P=NULL;
    GETCTRL(IEditorText,_P,m_pIUI, ETEXT_TALK_INPUT);

    if (_P)
        _P->SetMaxTypeIn(30);

    SetFocus(m_pIUI->GetWindowsHwnd());

	//m_pEngine->GetCWnd()->ShowWindow(SW_MINIMIZE);
	// duanxiaohui 2011-11-14 ������ϸ���Ͽ�
	m_pUserDetailsDlg = new CUserDetailsDlg();
	m_pUserDetailsDlg->m_uGameNameID = m_GameInfo.uNameID;
	m_pUserDetailsDlg->m_uComType = m_GameInfo.uComType;
	AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
	m_pUserDetailsDlg->Create(IDD_USER_DETAILS, NULL);
	AfxSetResourceHandle(GetModuleHandle(NULL));
	// end duanxiaohui

	m_pPmdWnd = new CGameTrottingWnd(m_pEngine->GetCWnd());
	m_pPmdWnd->init(m_pEngine->GetCWnd()->GetSafeHwnd());
	m_pPmdWnd->AdjustPos((HWND)m_pIUI->GetWindowsHwnd());
	m_pPmdWnd->SetLabelTextFont("����", 16);
	m_pPmdWnd->ShowWindow(SW_HIDE);
	m_GameInfo.bEnableWatch=AfxGetApp()->GetProfileInt(m_GameInfo.szGameName,TEXT("CanLooking"),FALSE);

	m_GameInfo.bEnableWatch=AfxGetApp()->GetProfileInt(m_GameInfo.szGameName,TEXT("CanLooking"),FALSE);//(GetProfileInt(TEXT("CanLooking"),FALSE)!=FALSE);
	m_GameInfo.bEnableSound=AfxGetApp()->GetProfileInt(m_GameInfo.szGameName,TEXT("PlaySound"),FALSE);//(GetProfileInt(TEXT("PlaySound"),TRUE)!=FALSE);
	m_GameInfo.bEnableBackSound=AfxGetApp()->GetProfileInt(m_GameInfo.szGameName,TEXT("BackSound"),FALSE);//(GetProfileInt(TEXT("BackSound"),TRUE)!=FALSE);
	m_GameInfo.bShowUserInfo=AfxGetApp()->GetProfileInt(m_GameInfo.szGameName,TEXT("ShowUserInfo"),FALSE);//(GetProfileInt(TEXT("ShowUserInfo"),FALSE)!=FALSE);  //PengJiLin, 2010-10-15, ȱʡ����ʾ��ҽ�����Ϣ


	

	OnMessageProc(UM_FIXRIGHTFRAME,0,0);

	//��ʼ�������õ�Combo�ؼ������Combo�ؼ����ڣ���ԭ�����ļ��������
	//Added by JianGuankun 2012.2.24
	ICombox* pCobBox = dynamic_cast<ICombox*>(m_pIUI->GetIControlbyID(ETEXT_TALK_COMBO));
	if (pCobBox)
	{
		IEditorText* pEdtCtrl = dynamic_cast<IEditorText*>(m_pIUI->GetIControlbyID(ETEXT_TALK_INPUT));
		if(pEdtCtrl) 
		{
			pEdtCtrl->SetControlVisible(false);
		}
		

		pCobBox->SetControlMoveDirect(1);

		pCobBox->SetAddItemWidth(30);

		//��������Ĭ�Ͼ���
		OnReLoadTalkINI();
	}

	IContain* pRightCtn = dynamic_cast<IContain*>(m_pIUI->GetIControlbyID(800));
	if (pRightCtn)
	{
		pRightCtn->SetControlVisible(false);
	}


	//����һЩ���пؼ���������Χ�봰��ͬ���������ұ߿�������ʾ��Ӱ��/////////////////////////////////

	ISudoku* pSoduku = dynamic_cast<ISudoku*>(m_pIUI->GetIControlbyID(1));
	if (pSoduku)
	{
		pSoduku->SetHaveShrinkArea(false);

		RECT rc = {0,0,0,0};

		Image* pImage = Image::FromFile(L"resources\\image\\GameBg.png");
		if(pImage)
		{
			rc.right = pImage->GetWidth();
			rc.bottom = pImage->GetHeight();
			delete pImage;
			pImage = NULL;
		}
		pSoduku->SetPicByIndex(8,L"resources\\image\\GameBg.png",rc);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(1003500));
	if (pBtn)
	{
		pBtn->SetHaveShrinkArea(false);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_MIN_FRAME));
	if (pBtn)
	{
		pBtn->SetHaveShrinkArea(false);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_RESET_FRAME));
	if (pBtn)
	{
		pBtn->SetHaveShrinkArea(false);
	}


	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_CLOSE_FRAME));
	if (pBtn)
	{
		pBtn->SetHaveShrinkArea(false);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_SET_FRAME));
	if (pBtn)
	{
		pBtn->SetHaveShrinkArea(false);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_VOLUM_UP));
	if (pBtn)
	{
		pBtn->SetHaveShrinkArea(false);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_VOLUM_DOWN));
	if (pBtn)
	{
		pBtn->SetHaveShrinkArea(false);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_VOLUM_OFF));
	if (pBtn)
	{
		pBtn->SetHaveShrinkArea(false);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_VOLUM_ON));
	if (pBtn)
	{
		pBtn->SetHaveShrinkArea(false);
	}

	IImage* pImage = NULL;
	pImage = dynamic_cast<IImage*>(m_pIUI->GetIControlbyID(IMAGE_NET_SIGNAL));
	if (pImage)
	{
		pImage->SetHaveShrinkArea(false);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�������ؿɱ߿�İ�ť
	IButton* pCloseBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_CLOSE_FRAME));
	IContain* pCtn = dynamic_cast<IContain*>(m_pIUI->GetIControlbyID(m_pIUI->GetControlBlongedId(BN_CLOSE_FRAME)));

	if (pCloseBtn)
	{
		m_pShowRightFrameBtn = dynamic_cast<IButton*>(pCloseBtn->Clone(BN_SHOW_RIGHTFRAME));
		pCtn->IInsertControl(m_pShowRightFrameBtn);
		char szBuff[256] = {0};
		sprintf(szBuff,"..\\%sshow_bt.png",m_skinmgr.GetGameSkinPath());
		m_pShowRightFrameBtn->LoadPic(CA2W(szBuff));
		m_pShowRightFrameBtn->SetGameW(10);
		m_pShowRightFrameBtn->SetGameH(98);
		m_pShowRightFrameBtn->SetGameXY(1008,350);
		m_pShowRightFrameBtn->SetHaveShrinkArea(false);
		m_pShowRightFrameBtn->SetControlVisible(false);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_VOLUM_UP));
	if (pBtn)
	{
		pBtn->SetControlVisible(false);
	}
	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_VOLUM_DOWN));
	if (pBtn)
	{
		pBtn->SetControlVisible(false);
	}
	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_VOLUM_ON));
	if (pBtn)
	{
		pBtn->SetControlVisible(false);
	}
	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_VOLUM_OFF));
	if (pBtn)
	{
		pBtn->SetControlVisible(false);
	}
    return 0;
}
/// IPC��Ϣ
bool XLogic::OnIPCMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
    //Ĭ�ϴ���
    switch (pHead->wMainCmdID)
    {
    case IPC_MAIN_SOCKET:	//�������ݣ�����������Ϸ�������˹�����Ϸ�������Ϣ
        {   
            return OnIPCSocket(pHead,pIPCBuffer,wDataSize,hWndSend);
        }
    case IPC_MAIN_CONCTROL:	//������Ϣ
        {            
            return OnIPCControl(pHead,pIPCBuffer,wDataSize,hWndSend);
        }
    case IPC_MAIN_CONFIG:	//������Ϣ
        {   
            return OnIPCConfig(pHead,pIPCBuffer,wDataSize,hWndSend);
        }
    case IPC_MAIN_USER:		//ԭOnControlMessage����
        {            
            return OnIPCUserAct(pHead,pIPCBuffer,wDataSize,hWndSend);
        }
    case IPC_MAIN_PROP:		//��ҵ��߳�ʼ����Ϣ
        {            
            return OnIPCProp(pHead,pIPCBuffer,wDataSize,hWndSend);
        }
	case IPC_USER_PROP_RESULT:   //��ҵ��߲������
		{
			return OnIPCUserPropResult(pHead,pIPCBuffer,wDataSize,hWndSend);
		}
	case IPC_FRIEND:
		{
			if (pHead->wSubCmdID == IPC_ASS_CHECKFRIEND)
			{

				UserItemStruct *pUser = (UserItemStruct *)m_ThisDeskPtrArray.GetAt(m_iSelectIndex);
				if (pUser == NULL) return 0;
				CString s;
				bool bIsHave = *((int*)pIPCBuffer) == 1;
				s.Format("xxx CLoveSendClassInExe OnAddFriend %d, %d", bIsHave, pUser->GameUserInfo.dwUserID);
				OutputDebugString(s);
				if (bIsHave)
				{
					AFCMessageBox("�Բ��𣬸��û��Ѿ������ĺ��ѣ�����Ҫ����ӣ�", "��ʾ");
				}
				else
				{

					//Ctest _test;
					//�����������
					CIMSetUser imsu;
					imsu.dwUser = pUser->GameUserInfo.dwUserID;
					imsu.szName = pUser->GameUserInfo.nickName;
					AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
					if(imsu.DoModal()==IDCANCEL)
						return true;
					AfxSetResourceHandle(GetModuleHandle(NULL));

					MSG_IM_C_ADDREQUEST* msg = new MSG_IM_C_ADDREQUEST;
					ZeroMemory(msg,sizeof(MSG_IM_C_ADDREQUEST));

					//CMainRoomEx *pWnd=(CMainRoomEx*)pParentWnd;
					msg->dwUserID= GetMeUserInfo()->dwUserID;
					msg->dwRequestedUserID= pUser->GameUserInfo.dwUserID;
					memcpy(&(msg->sUserName), GetMeUserInfo()->nickName,sizeof(GetMeUserInfo()->nickName));
					memcpy(&(msg->sRequestNotes),imsu.imWords,imsu.imWords.GetLength());
					msg->nMsgLength=imsu.imWords.GetLength();
					msg->cbSize=sizeof(MSG_IM_C_ADDREQUEST)-sizeof(msg->sRequestNotes)+msg->nMsgLength+1;//ʵ�ʳ���

					//MSG_IM_C_ADDREQUEST_Game t;
					//t.dwUserID1 = GetMeUserInfo()->dwUserID;
					//t.dwUserID = m_dwCurrentUserID;
					//m_pGameRoom->PostMessage(WM_FRIEND_R, sizeof(MSG_IM_C_ADDREQUEST), LPARAM(msg));
					m_pIPC->SendData(IPC_FRIEND, IPC_ASS_ADDFRIEND, msg, msg->cbSize);
					return true;
				}
			}
			if (pHead->wSubCmdID == IPC_ASS_GETFRIENDLIST)
			{
				MSG_IM_C_GETFRIENDLIST *msg = (MSG_IM_C_GETFRIENDLIST*)pIPCBuffer;

				if(wDataSize == 0)
				{
					m_FriendList.erase(m_FriendList.begin(), m_FriendList.end());
				}
				else if(wDataSize == sizeof(MSG_IM_C_GETFRIENDLIST))
				{
					m_FriendList.push_back(*msg);
				}
			}
		}
		break;

	case IPC_TROTTING:
		{
			OnIPCTrottingMsg(pHead,pIPCBuffer,wDataSize,hWndSend);
		}
		break;

	case IPC_EX_SKIN:
		{
			OnExchageSkin(pHead,pIPCBuffer,wDataSize,hWndSend);
		}
		break;
    }

    return false;
}


//��talkIngame.ini
void XLogic::OnReLoadTalkINI()
{
	//���ԭ������
	ICombox* pComb = dynamic_cast<ICombox*>(m_pIUI->GetIControlbyID(ETEXT_TALK_COMBO));
	if (!pComb)
	{
		return;
	}

	pComb->Clear();
	CBcfFile f("talkInGame.bcf");
	CString key="BZW_TALK";
	CString szTalk;
	CString szNumber;
	int nIdx = 0;
	do
	{
		szNumber.Format("%d",++nIdx);
		szTalk = f.GetKeyVal(key,"T"+szNumber,"");
		if(!szTalk.IsEmpty())
			pComb->AddString(CA2W(szTalk.GetBuffer()));
	}
	while(!szTalk.IsEmpty());

	pComb->SetText(L"");

	return;
}

bool XLogic::OnIPCControl(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
    ASSERT(pHead->wMainCmdID==IPC_MAIN_CONCTROL);
    switch (pHead->wSubCmdID)
    {
    case IPC_SUB_START_FINISH:		//�������
        {
            return true;
        }
    case IPC_SUB_CLOSE_FRAME:		//�رտ��
        {
            m_pIPC->SendData(IPC_MAIN_IPC_KERNEL, IPC_SUB_CLOSE_FRAME, NULL, 0);
            QuitGame(false);

            DebugPrintfA("�յ�IPC_SUB_CLOSE_FRAME���˳���Ϸ");
            return true;
        }
    case IPC_SUB_SHOW_MSG_IN_GAME:
        {
            MsgInGameStruct *pMsg = (MsgInGameStruct *)pIPCBuffer;
            // ���ݴ����������ݣ��жϴ�������Ƿ�ӦΪ��
            /*this->m_IGameFrame->ShowMsgInGame(pMsg->szMsg,
            pMsg->bNewOrSys,
            pMsg->uSize,
            pMsg->bIsFontValid?pMsg->szFontName:NULL,
            pMsg->bIsTitleValid?pMsg->szTitle:NULL);*/

			//��ҵ��߲��������ʾ
			InsertSystemMessage(pMsg->szMsg);
            return true;
        }
    case IPC_SUB_SHOW_DUDU:
        {
            MsgDuduStruct *pMsg = (MsgDuduStruct *)pIPCBuffer;
            // ������Ϣ
            InsertDuduMsg(pMsg->szSendName,
            pMsg->szTargetName,
            pMsg->szCharString,
            pMsg->crTextColor,
            pMsg->bShowTime,
            pMsg->uSize,
            pMsg->bIsFontValid?pMsg->szFontName:NULL,
            pMsg->iDuduType);
            return true;
        }
    case IPC_SUB_JOIN_IN_GAME:
        {
            //if (NULL != m_IGameFrame)
            //{            //    
            //    m_IGameFrame->ShowMsgInGame(NULL);
            //}
            return true;
        }
    }

    return false;
}
//�������ݣ�����������Ϸ�������˹�����Ϸ�������Ϣ
bool XLogic::OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
    ASSERT(pHead->wMainCmdID==IPC_MAIN_SOCKET);

    // ������Ϣ������¼ʱ��
    m_dwIPCHeartBeatTick = GetTickCount();
    m_pIPC->SendData(IPC_MAIN_SOCKET, IPC_SUB_IPC_HEART_BEAT, NULL, 0);

    switch (pHead->wSubCmdID)
    {
    case IPC_SUB_SOCKET_SEND:	//������Ϣ
        {
            //Ч������
            ASSERT(wDataSize>=sizeof(CMD_Command));
            if (wDataSize<sizeof(CMD_Command)) 
            {
                return false;
            }

            //��������
            NetMessageHead * pNetHead=(NetMessageHead *)pIPCBuffer;
            WORD wSendSize=wDataSize-sizeof(NetMessageHead);
            void *pNetData = NULL;
            if (wSendSize >= 0)
            {
                pNetData = (BYTE*)pIPCBuffer + sizeof(NetMessageHead);
            }
            //if (NULL != m_IGameFrame)
            //{
            //    m_IGameFrame->OnSocketReadEvent(pNetHead, pNetData, wSendSize);
            //}
            OnSocketReadEvent(pNetHead, pNetData, wSendSize);
            return true;
        }
    case IPC_SUB_IPC_HEART_BEAT:
        {
            // ������Ϣ������¼ʱ��
            m_dwIPCHeartBeatTick = GetTickCount();
            m_pIPC->SendData(IPC_MAIN_SOCKET, IPC_SUB_IPC_HEART_BEAT, NULL, 0);
            return true;
        }
    }

    return false;
}
/// IPC�������
bool XLogic::OnIPCHeartBeat()
{
    DWORD dwThisTick = GetTickCount();
    if (NULL==m_pIPC)
    {
        return false;
    }
    if (dwThisTick - m_dwIPCHeartBeatTick > (HEART_BEAT_DELAY+HEART_BEAT_SECOND)*1000) // ������ʱ
    {
        DebugPrintfA("IPC �������ʧ�ܣ��˳���Ϸ");
        //��ʾ�������Ѿ�ֹͣ��
        QuitGame();
        return false;
    }
    else
    {
        m_pIPC->SendData(IPC_MAIN_SOCKET, IPC_SUB_IPC_HEART_BEAT, NULL, 0);
    }
    return true;
}

/************************************************************************/
//Function:checkNetSignal()
//Parameter:
//                pNetData        :���������ص���Ϣ��
//Author:Fred Huang 2008-06-27
//Note:�������������ص���Ϣ
/************************************************************************/
void XLogic::getNetSignal(void * pNetData)
{
    MSG_GP_Game_NETSignal *netSignal=(MSG_GP_Game_NETSignal*)pNetData;
    //������Ƿ����Լ��ģ�����
    if(m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID!=netSignal->dwUserID)
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
void XLogic::drawSignal()
{
    IImage *pNetSignal = dynamic_cast<IImage*>(m_pIUI->GetIControlbyID(IMAGE_NET_SIGNAL));
    if (pNetSignal)
    {
        pNetSignal->SetSrcY(0);
        pNetSignal->SetControlVisible(true);
        pNetSignal->SetSrcY(m_NetSignalLevel*pNetSignal->GetPicHeight()/6);
    }

    //if (NULL == pDc || NULL == pDc->m_hDC )
    //    return;

    //if(!pSingnalimg || pSingnalimg->GetFlags()==0)
    //    return;
    //CString s="..\\";// CINIFile::GetAppPath();

    //// ----- ����ָ����Ϸ�������ļ� -------- dxh
    //char GameCode[20];
    //::memset(GameCode,0,sizeof(GameCode));
    //GetGameCode(GameCode);
    ////if (str == "11901800"
    ////	|| str == "10901800"
    ////	|| str == "30501800")
    //TCHAR szKey[10];
    //CString sPath=CBcfFile::GetAppPath();
    //CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
    //wsprintf(szKey, "%d", m_pGameInfo->uNameID);
    //int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

    //if (iResult>0)
    //{
    //    s = ".\\config\\";  ///<add by wxx 2010.7.1
    //}
    ////else
    ////{
    ////	s="..//config//"; ///<add by wxx 2010.07.14
    ////	//return;
    ////}
    //// ----- ����ָ����Ϸ�������ļ� --------

    //CBcfFile f( s + "GameOption.bcf");

    //CString key= Glb().m_skin;

    //CRect rc;
    //GetClientRect(rc);
    //int x = rc.Width() - f.GetKeyVal(key, _T("game_signalx"), 184) - pSingnalimg->GetWidth();

    //Graphics g(this->m_hWnd);
    //g.DrawImage(pSingnalimg,Rect(x,f.GetKeyVal(key, _T("game_signaly"),2),pSingnalimg->GetWidth(),pSingnalimg->GetHeight()/6),0,m_NetSignalLevel*pSingnalimg->GetHeight()/6,pSingnalimg->GetWidth(),pSingnalimg->GetHeight()/6,UnitPixel);
}

//�׽�����Ϣ
bool XLogic::OnSocketReadEvent(NetMessageHead * pNetHead, void *pNetData, WORD wSendSize)
{
    //���˴���
    if (m_GameInfo.uisMeUserInfo.GameUserInfo.bDeskNO==255) 
        return true;
    //������Ϣ
    switch (pNetHead->bMainID)
    {
    case MDM_GR_NETSIGNAL:      //����״��
        {
            //wushuqun 2009.6.18
            //�˴���MDM_GR_NETSIGNAL��ϢΪIPCģ����Ϣ
            //Ϊ����������л�������Թ�ģʽBUG
            if (pNetHead->bAssistantID == MDM_GR_NETSIGNAL && pNetHead->bHandleCode == MDM_GR_NETSIGNAL)
            {
                m_bWatchMode = false;				
                m_bWatchOther = true;
                return true;
            }
            //////////////////////////////////////
            getNetSignal(pNetData);
            return true;
        }
        break;
    case MDM_GM_GAME_NOTIFY:	//��Ϸ��Ϣ
        {
            if (ASS_PROP==pNetHead->bAssistantID)
            {

				
				if (wSendSize!=sizeof(_TAG_PROP_MOIVE_USE)) {
					//WriteInfo(_T("CLoveSendClassInExe::HandleGameMessage 00"));
					return FALSE;
				}
				_TAG_PROP_MOIVE_USE *propMoive=(_TAG_PROP_MOIVE_USE*)pNetData;

				UserProp(propMoive);

				//AfxMessageBox("�ͻ�");
                // ����Ҫ����Ϣ��ռ����2�����Ϣ��
                return 0;
            }

			CString s;
			s.Format("xxxdxy %d %d %I64d",(WORD)pNetHead->bAssistantID, m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID, m_GameInfo.uisMeUserInfo.GameUserInfo.i64Money);
			OutputDebugString(s);

            int nRet = m_pGameImpl->HandleGameMessage(MDM_GM_GAME_NOTIFY,(WORD)pNetHead->bAssistantID,(BYTE*)pNetData, wSendSize);
            if (m_GameInfo.bIsInRecord)
            {
                //SetRecordView();
            }
            return 0==nRet;
        }
        break;
    case MDM_GM_GAME_FRAME:		//�����Ϣ
        {
            return OnFrameMessage(pNetHead,pNetData,wSendSize);
        }
    case MDM_GM_MESSAGE:		//��Ϣ��Ϣ
        {
            return OnSystemMessage(pNetHead,pNetData,wSendSize);
        }
    case MDM_GR_PROP:		//ʹ�õ���
        {
            return OnPropMessage(pNetHead,pNetData,wSendSize);
        }
    case MDM_GR_ROOM2GAME:
        {
            switch (pNetHead->bAssistantID)
            {
			case ASS_GR_R2G_USEPROP:    // PengJiLin, 2010-10-14, ��Ϸ�˵��ߵļ����ù���
				{
					if(wSendSize != sizeof(_TAG_USERPROP)) return false;

					_TAG_USERPROP* pSendProp = (_TAG_USERPROP*)pNetData;

					int propCount=Glb().userPropLibrary.GetCount();
					_TAG_USERPROP *userProp=NULL;
					bool bPropExist=false;
					int iPropIndex = 0;
					for(int i=0;i<propCount;i++)
					{
						userProp=Glb().userPropLibrary.GetAt(i);
						if(userProp->nPropID==pSendProp->nPropID)
						{
							bPropExist=true;
							iPropIndex = i;
							break;
						}
					}

					// �ҵ�
					if(true == bPropExist)
					{
						if (pNetHead->bHandleCode == DTK_GP_PROP_BUYANDUSE_SUCCEED)
						{
							if (m_pRightFrame && m_pRightFrame->m_pBoardCastWnd)
								m_pRightFrame->m_pBoardCastWnd->UseProp_Buy(iPropIndex);
						}
						else
							UsePropItem(iPropIndex, FALSE);
					}

					return true;
				}
				break;

                case ASS_GR_GET_NICKNAME_ONID_INGAME:      // lxl, 2010-11-17, �����û�ID��ȡ�ǳ�
                    {
                        OutputDebugString("��Ϸ�гɹ��յ��������������ǳ�");
                        if(ASS_GR_GETNICKNAME_SUCCESS == pNetHead->bHandleCode) // �����ɹ�
                        {
                            // Ч������
                            if (wSendSize != sizeof(MSG_GR_S_GetNickNameOnID_t)) return false;
                            MSG_GR_S_GetNickNameOnID_t* pGetNickNameOnID = (MSG_GR_S_GetNickNameOnID_t *)pNetData;

                            m_pPersonBank->SetNickName(pGetNickNameOnID->szNickName); 
                            return true;
                        }
                        return true;
                    }
                    break;
                default:
                    break;
            }
        }
    case MDM_GR_MONEY:
        {   
            return OnAboutMoney(pNetHead,pNetData,wSendSize);
        }
        break;
	case MDM_BANK:
		{
			if (m_pBank2Wnd)
			{
				m_pBank2Wnd->OnNetMessage(pNetHead,pNetData,wSendSize);
			}
		}
		break;
    default:
        break;
    }
    return true;
}

// ��ƽ̨��������Ϣ������wDataSize = pIPCBuffer���ݵĳ���
bool XLogic::OnIPCConfig(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
    ASSERT(pHead->wMainCmdID==IPC_MAIN_CONFIG);
    switch (pHead->wSubCmdID)
    {
    case IPC_SUB_SERVER_INFO:
        {
            // ֱ�Ӿ���m_GameInfo�����ݣ�û�б������
            //��������

            IPC_SocketPackage * pSocketPackage=(IPC_SocketPackage *)pIPCBuffer;
            CMD_Command *pCommand= &pSocketPackage->Command;
            WORD wSendSize=wDataSize;
            if (wSendSize != sizeof(GameInfoStructInExe)) 
            {
                AfxMessageBox("ƽ̨��GameLancher�е�GameInfoStructInExe��һ��");
                return false;
            }
            else 
            {
                memcpy( &m_GameInfo, pIPCBuffer, sizeof(m_GameInfo));
                //if (m_GameInfo.pOrderName = NULL)
                //{
                //    //m_GameInfo.pOrderName = GetGameOrderNameInExe;
                //}
                m_GameInfo.pOrderName = GetPointOrderName;

				CString str;
				str.Format("DGameL: 1.1 m_GameInfo.pOrderName = 0x%08x",GetPointOrderName);
				OutputDebugString(str);

                /*ShowWindow(SW_HIDE);                
                OnBnClickedButton1();*/
                m_pIPC->SendData(IPC_MAIN_CONFIG,IPC_SUB_SERVER_INFO,NULL,0);
            }
            return true;
        }
    case IPC_SUB_ENCRYPT_TYPE:
        {
            int *pEncrypt = (int*)pIPCBuffer;
            Glb().m_CenterServerPara.m_nEncryptType = *pEncrypt;
            return true;
        }
    }

    return false;
}
// ��ƽ̨��������Ϣ������pIPCBuffer�е�һ��UINT���ǿ�����UINT uControlCode, void * pControlData, UINT uDataSize
bool XLogic::OnIPCUserAct(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
    ASSERT(pHead->wMainCmdID==IPC_MAIN_USER);
    if (NULL == m_pGameImpl)
    {
        //DebugPrintf("���ڴ���û������������Ϣ");
        return false;
    }
    switch(pHead->wSubCmdID)
    {
    case IPC_SUB_REPLY_USER_LIST:
        {
            if(wDataSize<sizeof(UserItemStruct)) return false;
            OnUserListAtDesk((UserItemStruct *)pIPCBuffer);
            return true;
        }
    case IPC_SUB_USER_STATUS:
    case IPC_SUB_USER_SCORE:		//�û�����
        {
            UINT *pControlCode = (UINT*)pIPCBuffer;
            void *pControlData = (BYTE*)pIPCBuffer + sizeof(UINT);
            OnControlMessage( *pControlCode, pControlData, wDataSize-sizeof(UINT) );
            break;
        }
    case IPC_SUB_USER_COME:
        {
            UserItemStruct *pUserInfo = (UserItemStruct *)pIPCBuffer;

			if (GetMeUserInfo()->dwUserID == pUserInfo->GameUserInfo.dwUserID)
			{
				GetMeUserInfo()->i64Bank = pUserInfo->GameUserInfo.i64Bank;
			}
            //this->m_IGameFrame->ResetUserInfo(pUserInfo);
            break;
        }
    default:
        break;
    }
    return true;
}
// ��ƽ̨������������йص���Ϣ
bool XLogic::OnIPCProp(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
    ASSERT(pHead->wMainCmdID==IPC_MAIN_PROP);
    switch(pHead->wSubCmdID)
    {
    case IPC_SUB_INIT_PROP_FINISH:
        {
            MsgGlbData *pGD = (MsgGlbData*)pIPCBuffer;
            Glb().m_CenterServerPara.m_nEncryptType = pGD->m_nEncryType;
            Glb().m_NewUrl = pGD->m_NewUrl;
            Glb().m_TML_SN = pGD->m_szToken;            
            // ���������������ԭ��������ͬ����ˢ��֮
            int nPropCountNew = (int)m_userPropLibrary.GetCount();
            int nPropCountOld = (int)Glb().userPropLibrary.GetCount();
            // �����Ƿ��и��£�����Ҫ���и���
            for(int i=0; i<nPropCountOld; ++i)
            {
                _TAG_USERPROP *userProp=Glb().userPropLibrary.GetAt(i);
                delete userProp;
                userProp=NULL;
            }
            Glb().userPropLibrary.RemoveAll();
            for (int i=0; i<nPropCountNew; ++i)
            {
                _TAG_USERPROP *userProp=m_userPropLibrary.GetAt(i);
                Glb().userPropLibrary.Add(userProp);
            }
            m_userPropLibrary.RemoveAll();
            // ���������һ�£�����Ҫˢ�½���
            // PengJiLin, 2010-10-13, ÿ�ζ����½��棬����ע�����²��ִ���
            //if (nPropCountOld != nPropCountNew)
//{
//    if (m_IGameFrame)
//    {
//        m_IGameFrame->OnInitPropList();
//        //DebugPrintf("ˢ�µ���");
//    }
//}
            //else
            //{
            //	m_IGameFrame->OnInitPropList(1);	// ֻ���������������»���
            //}
            //m_bNeedRefleshProp = false;
            //m_nPropCount = 0;


            m_nPropPage = 0;
            RefreshPropPannel();
            break;
        }
    case IPC_SUB_INIT_PROP:
        {
            //���ڴ�����
            MSG_PROP_S_GETPROP * msg=(MSG_PROP_S_GETPROP*)pIPCBuffer;
            _TAG_USERPROP *userProp=new _TAG_USERPROP;
            memcpy(userProp,msg,sizeof(_TAG_USERPROP));
            m_userPropLibrary.Add(userProp);
            return true;
        }
    default:
        break;
    }

    return true;
}


//ƽ̨�����ĵ��߲������
bool XLogic::OnIPCUserPropResult(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	NetMessageHead * pNetHead = new NetMessageHead;
	pNetHead->bAssistantID = ASS_PROP_RESULT;
	m_pPropPannel->OnNetMessage(pNetHead,(void *)pIPCBuffer,wDataSize);
	return true;
}

//�������Ϣ����
bool XLogic::OnIPCTrottingMsg(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	TMailItem* pMailMsg = (TMailItem*)pIPCBuffer;
	m_pPmdWnd->StartPmdLabel(120000,pMailMsg->szMsg,20,true);
	m_pPmdWnd->ShowCtrl();
	return true;
}

//������Ϣ Add by JianGuankun 2012.10.19
bool XLogic::OnExchageSkin(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	if (FileExists("GameComm",true))
	{
		return false;
	}
	//����֪ͨ
	MSG_GP_R_LogonResult* _p = (MSG_GP_R_LogonResult*)pIPCBuffer;
	CString strUserName = _p->szName;
	m_skinmgr.SetLogOnUserName(strUserName);
	m_skinmgr.Notify();

	wchar_t wszPath[MAX_PATH] = {0};
	wchar_t wszName[MAX_PATH] = {0};
	swprintf(wszPath,L"..\\%sgame",CA2W(m_skinmgr.GetSkinPath()));

	ISudoku* pSoduku = dynamic_cast<ISudoku*>(m_pIUI->GetIControlbyID(1));
	if(pSoduku)
	{
		swprintf(wszName,L"%s\\GameWnd.png",wszPath);

		char szRcFile[64] = {0};
		_stprintf(szRcFile,"..\\config\\%s",m_skinmgr.GetGameSkinBcfFileName().GetBuffer());
		CBcfFile f(szRcFile);

		RECT rc;

		auto ldGetRect = [&f,&rc](char* key)
		{
			CString strPos;
			strPos = f.GetKeyVal(_T("Sudoku"),key,"0,0,0,0");
			rc.left	  = ::atoi(::strtok(strPos.GetBuffer(),","));
			rc.top	  = ::atoi(::strtok(NULL,","));
			rc.right  = rc.left + ::atoi(::strtok(NULL,","));
			rc.bottom = rc.top +  ::atoi(::strtok(NULL,","));
			return;
		};

		ldGetRect(_T("4_LC"));
		pSoduku->SetPicByIndex(0,wszName,rc);

		ldGetRect(_T("5_RC"));
		pSoduku->SetPicByIndex(1,wszName,rc);

		ldGetRect(_T("2_TC"));
		pSoduku->SetPicByIndex(2,wszName,rc);
 
		ldGetRect(_T("1_TL"));
 		pSoduku->SetPicByIndex(3,wszName,rc);

		ldGetRect(_T("3_TR"));
 		pSoduku->SetPicByIndex(4,wszName,rc);

		ldGetRect(_T("6_BL"));
		pSoduku->SetPicByIndex(5,wszName,rc);

		ldGetRect(_T("8_BR"));
 		pSoduku->SetPicByIndex(6,wszName,rc);

		ldGetRect(_T("7_BC"));
		pSoduku->SetPicByIndex(7,wszName,rc);

	}

	IButton* pBtn = NULL;
	IImage* pImg = NULL;

	int iX = 670;
	int iTopH = pSoduku ? (pSoduku->GetSrcRect(2).bottom - pSoduku->GetSrcRect(2).top) : 2;

	pImg = dynamic_cast<IImage*>(m_pIUI->GetIControlbyID(IMAGE_NET_SIGNAL));
	if (pImg != NULL)
	{
		swprintf(wszName,L"%s\\game_signal.png",wszPath);
		pImg->SetGameXY(iX,(iTopH - pImg->GetPicHeight()/6 - 2)/2);
		pImg->LoadPic(wszName);
		pImg->SetGameW(pImg->GetPicWidth());
		pImg->SetGameH(pImg->GetPicHeight()/6);
		pImg->SetSrcW(pImg->GetPicWidth());
		pImg->SetSrcH(pImg->GetPicHeight()/6);
		iX += pImg->GetPicWidth() + 15;
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(1003500));
	if (pBtn != NULL)
	{
		swprintf(wszName,L"%s\\game_bt01.bmp",wszPath);
		pBtn->SetGameW(pBtn->GetPicWidth()/4);
		pBtn->SetGameH(pBtn->GetPicHeight());
		pBtn->SetGameXY(iX,(iTopH - pBtn->GetPicHeight() - 2)/2);
		iX += pBtn->GetPicWidth()/4 + 15;
		pBtn->LoadPic(wszName);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_VOLUM_UP));
	if (pBtn != NULL)
	{
		//swprintf(wszName,L"%s\\SndVolUp.bmp",wszPath);
		//pBtn->SetGameH(pBtn->GetPicHeight());
		//pBtn->SetGameXY(iX,(iTopH - pBtn->GetPicHeight() - 2)/2);
		//pBtn->SetGameW(pBtn->GetPicWidth()/4);
		//iX += pBtn->GetPicWidth()/4 + 2;
		//pBtn->LoadPic(wszName);
		pBtn->SetControlVisible(false);
		pBtn->SetEnable(false);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_VOLUM_ON));
	if (pBtn != NULL)
	{
		//swprintf(wszName,L"%s\\SndPlay.bmp",wszPath);
		//pBtn->SetGameW(pBtn->GetPicWidth()/4);
		//pBtn->SetGameH(pBtn->GetPicHeight());
		//pBtn->SetGameXY(iX,(iTopH - pBtn->GetPicHeight() - 2)/2);
		//pBtn->LoadPic(wszName);
		pBtn->SetControlVisible(false);
		pBtn->SetEnable(false);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_VOLUM_OFF));
	if (pBtn != NULL)
	{
		//swprintf(wszName,L"%s\\SndPause.bmp",wszPath);
		//pBtn->SetGameW(pBtn->GetPicWidth()/4);
		//pBtn->SetGameH(pBtn->GetPicHeight());
		//pBtn->SetGameXY(iX,(iTopH - pBtn->GetPicHeight() - 2)/2);
		//iX += pBtn->GetPicWidth()/4 + 2;
		//pBtn->LoadPic(wszName);
		pBtn->SetControlVisible(false);
		pBtn->SetEnable(false);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_VOLUM_DOWN));
	if (pBtn != NULL)
	{
		//swprintf(wszName,L"%s\\SndVolDown.bmp",wszPath);
		//pBtn->SetGameW(pBtn->GetPicWidth()/4);
		//pBtn->SetGameH(pBtn->GetPicHeight());
		//pBtn->SetGameXY(iX,(iTopH - pBtn->GetPicHeight() - 2)/2);
		//pBtn->LoadPic(wszName);
		pBtn->SetControlVisible(false);
		pBtn->SetEnable(false);
	}

	iX = 932;

	CBcfFile f(CBcfFile::GetAppPath() + _T("GameOption.bcf"));
	UINT uMaxType = f.GetKeyVal("FrameControl","max_type",1);
	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_MIN_FRAME));
	if (pBtn != NULL)
	{
		swprintf(wszName,L"%s\\game_min_bt.bmp",wszPath);
		pBtn->SetGameW(pBtn->GetPicWidth()/4);
		pBtn->SetGameH(pBtn->GetPicHeight());
		pBtn->SetGameXY(iX,0);
		iX += pBtn->GetPicWidth()/4;
		pBtn->LoadPic(wszName);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_RESET_FRAME));
	if (pBtn != NULL && 0 != uMaxType)
	{
		swprintf(wszName,L"%s\\game_rest_bt.bmp",wszPath);
		pBtn->SetGameW(pBtn->GetPicWidth()/4);
		pBtn->SetGameH(pBtn->GetPicHeight());
		pBtn->SetGameXY(iX,0);
		pBtn->LoadPic(wszName);
	}
	else if (pBtn != NULL && 0 == uMaxType)
	{
		pBtn->SetControlVisible(false);
		pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_MIN_FRAME));
		if (pBtn != NULL)
		{
			pBtn->SetGameXY(iX,0);
		}
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_SET_FRAME));
	if (pBtn != NULL && uMaxType != 0)
	{
		swprintf(wszName,L"%s\\game_max_bt.bmp",wszPath);
		pBtn->SetGameW(pBtn->GetPicWidth()/4);
		pBtn->SetGameH(pBtn->GetPicHeight());
		pBtn->SetGameXY(iX,0);
		iX += pBtn->GetPicWidth()/4;
		pBtn->LoadPic(wszName);
	}
	else if (pBtn != NULL && 0 == uMaxType)
	{
		pBtn->SetControlVisible(false);
	}

	pBtn = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_CLOSE_FRAME));
	if (pBtn != NULL)
	{
		swprintf(wszName,L"%s\\game_close_bt.bmp",wszPath);
		pBtn->SetGameW(pBtn->GetPicWidth()/4);
		pBtn->SetGameH(pBtn->GetPicHeight());
		pBtn->SetGameXY(iX,0);
		iX += pBtn->GetPicWidth()/4;
		pBtn->LoadPic(wszName);
	}

	WINDOWPLACEMENT place;
	::GetWindowPlacement(m_pEngine->GetCWnd()->GetSafeHwnd(),&place);
	OnMessageProc(UM_FIXRIGHTFRAME,place.showCmd,0);

	return true;
}

/// UI��Ϣ
int XLogic::OnUIMessage(TUIMessage *pMessage)
{
	// ������Ϣ
	if (((pMessage->_uControlID == ETEXT_TALK_INPUT && pMessage->_ch == 13 && UI_KEYUP==pMessage->_uMessageType)
		|| (pMessage->_uControlID == ETEXT_TALK_COMBO && pMessage->_ch == 13 && UI_KEYUP==pMessage->_uMessageType)
		|| (pMessage->_uControlID == BN_TALK_SEND && UI_LBUTTONUP==pMessage->_uMessageType))
		/*&& CanTalk()*/)
	{
		ICombox* _pCob = NULL;
		GETCTRL(ICombox,_pCob,m_pIUI,ETEXT_TALK_COMBO);
		if (_pCob != NULL)
		{
			wchar_t *p = _pCob->GetText();
			TCHAR* _Msg = CW2T(p);
			{
				SendTalkMsg(_Msg,0);
				_pCob->TextClear();
			}
		}
		else
		{
			IEditorText* _P=NULL;
			GETCTRL(IEditorText,_P,m_pIUI, ETEXT_TALK_INPUT);

			if (_P != NULL)
			{
				wchar_t *p = _P->GetText();
				TCHAR* _Msg = CW2T(p);
				if (_Msg != NULL)
				{
					SendTalkMsg(_Msg, 0);
					_P->Clear();
				}
			}
		}

		return 0;
	}

	if (pMessage->_uControlID == ETEXT_TALK_COMBO && UI_COMBOBOX_SELITEM == pMessage->_uMessageType)
	{
		ICombox* _pCob = NULL;
		GETCTRL(ICombox,_pCob,m_pIUI,ETEXT_TALK_COMBO);
		if (_pCob != NULL)
		{
			wchar_t *p = _pCob->GetCurSelValue();
			TCHAR* _Msg = CW2T(p);
			{
				SendTalkMsg(_Msg,0);
				_pCob->TextClear();
			}
		}
	}

    if (pMessage->_uControlID>=801 && pMessage->_uControlID<=850 && UI_LBUTTONUP==pMessage->_uMessageType)      // ���߹�������
    {
		// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�õ���
		if (m_GameInfo.dwRoomRule & GRR_CONTEST)
		{
			AFCMessageBox("�Բ���, ����������ʹ����Ϸ����", "��ܰ��ʾ");
			return 0;
		}
		if (m_GameInfo.dwRoomRule & GRR_EXPERCISE_ROOM)
		{
			AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ����Ϸ����", "��ܰ��ʾ");
			return 0;
		}
		// end duanxiaohui

        if (pMessage->_uControlID==BN_BUY_VIP)
        {
            if (UI_LBUTTONUP==pMessage->_uMessageType)
            {
                m_pPropPannel->setSkinIndex(0);
                m_pPropPannel->openPannel(0,"", 9999);
            }            
        }
        else
        {
            OnBuyPropUIMessage(pMessage);
        }
        return 0;
    }

    if (BTN_SET==pMessage->_uControlID ||
        (pMessage->_uControlID>=SET_ID_BEGIN && pMessage->_uControlID<=SET_ID_END) )
    {
        OnSetUIMessage(pMessage);
        return 0;
    }
	if (UI_LBUTTONDOWN == pMessage->_uMessageType && RIGHTBOARD_USERLIST==pMessage->_uControlID)
	{
		// ��ѡ�е��û�
		IListControl *pList = dynamic_cast<IListControl *>(m_pIUI->GetIControlbyID(RIGHTBOARD_USERLIST));
		if (NULL!=pList)
		{
			m_iSelectIndex = pList->GetSelectRowIndex();
			if (m_iSelectIndex>=0 && m_iSelectIndex<m_ThisDeskPtrArray.GetCount())
			{
				UserItemStruct *pUser = (UserItemStruct *)m_ThisDeskPtrArray.GetAt(m_iSelectIndex);
				if (pUser != NULL/* && pUser->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID*/)
				{
					ShowCurrentUserInfo(pUser , true) ; 
				}					
			}
		}

	}

    if (UI_RBUTTONDOWN==pMessage->_uMessageType && RIGHTBOARD_USERLIST==pMessage->_uControlID)
	{
		// ��ѡ�е��û�
		IListControl *pList = dynamic_cast<IListControl *>(m_pIUI->GetIControlbyID(RIGHTBOARD_USERLIST));
		if (pList == NULL)
		{
			return 0;
		}

		m_iSelectIndex = pList->GetSelectRowIndex();
		if (m_iSelectIndex < 0 || m_iSelectIndex >= m_ThisDeskPtrArray.GetCount())
		{
			return 0;
		}


		if (m_GameInfo.dwRoomRule & GRR_NOTCHEAT) return 0;

		UserItemStruct *pUser = (UserItemStruct *)m_ThisDeskPtrArray.GetAt(m_iSelectIndex);
		IMenu *pMenu = dynamic_cast<IMenu *>(m_pIUI->GetIControlbyID(RIGHTBOARD_USERMENU));

		if (pUser != NULL && pMenu != NULL)
		{
			pMenu->Clear();

			// duanxiaohui 2011-11-14 �Ҽ��˵�����
			pMenu->AddString(L"  ��ϸ����");
			pMenu->AddString(L"  �����û���");

			if (pUser->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID)
			{
				pMenu->AddString(L"  ��Ϊ����");
				pMenu->AddString(L"  ���������");
				pMenu->AddString(L"  �����뿪");
			}
			// end duanxiaohui

			//////////////////////////////////////////////////////////////////////////////////
			//UserItemStruct *pUser = (UserItemStruct *)m_ThisDeskPtrArray.GetAt(m_iSelectIndex);
			//char szFor[200]={0};
			//// �û���                    
			//sprintf(szFor, "  �û���:%s", pUser->GameUserInfo.nickName);
			//pMenu->AddString(CA2W(szFor));
			//// ����
			//sprintf(szFor, "  ����:%d", pUser->GameUserInfo.dwPoint-GetNormalOrderBasePoint(pUser->GameUserInfo.dwPoint));
			//pMenu->AddString(CA2W(szFor));
			//// �ȼ�                    
			//sprintf(szFor, "  �ȼ�:%s", m_GameInfo.pOrderName(pUser->GameUserInfo.dwPoint));
			//pMenu->AddString(CA2W(szFor));
			//// ���
			//char szMoney[200]={0};
			//GlbGetNumString(szMoney, pUser->GameUserInfo.i64Money, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
			//sprintf(szFor, "  ���:%s", szMoney);
			//pMenu->AddString(CA2W(szFor));
			//// �Ƹ�����
			//sprintf(szFor, "  �Ƹ�����:%s", GetMoneyOrderName(pUser->GameUserInfo.i64Money));
			//pMenu->AddString(CA2W(szFor));
			//// �ܾ���
			//int nTotalCount = pUser->GameUserInfo.uMidCount+pUser->GameUserInfo.uWinCount+pUser->GameUserInfo.uLostCount;
			//sprintf(szFor, "  �ܾ���:%d", nTotalCount);
			//pMenu->AddString(CA2W(szFor));
			//// ������                    
			//sprintf(szFor, "  ������:%f", pUser->GameUserInfo.uCutCount*1.0f/nTotalCount);
			//pMenu->AddString(CA2W(szFor));
			////////////////////////////////////////////////////////////////////////////////////

			pMenu->SetGameXY(pMessage->_mouseX, pMessage->_mouseY);
			pMenu->SetControlVisible(true);
		}
	}
//�����Ҽ��˵�
	if (UI_RBUTTONDOWN==pMessage->_uMessageType && TALK_SHOW==pMessage->_uControlID)
	{
		IMenu *pMenu = NULL;
		GETCTRL(IMenu,pMenu,m_pIUI,TALK_MENU);
		if (pMenu)
		{	
			pMenu->Clear();

			pMenu->AddString(L"  �����Ļ");
			pMenu->AddString(L"  ����");
			if (m_bIsAutoScroll)
			{
				pMenu->AddString(L"�� �������");
			} 
			else
			{
				pMenu->AddString(L"  �������");
			}			

			if (m_bIsAcceptChatMsg)
			{
				pMenu->AddString(L"�� ����������Ϣ");	
			} 
			else
			{
				pMenu->AddString(L"  ����������Ϣ");
			}

			pMenu->SetControlVisible(true);

			RECT r;
			GetClientRect(m_pIUI->GetWindowsHwnd(), &r);
			int x, y;
			x = pMessage->_mouseX;
			y = pMessage->_mouseY;
			if (pMessage->_mouseX - pMenu->GetGameW() < 0)
				x = pMenu->GetGameW();
			else if (pMessage->_mouseX + pMenu->GetGameW() > r.right)
				x = r.right - pMenu->GetGameW();
			if (pMessage->_mouseY - pMenu->GetGameH() < 0)
				y = pMenu->GetGameH();
			else if (pMessage->_mouseY + pMenu->GetGameH() > r.bottom)
				y = r.bottom - pMenu->GetGameH();

			pMenu->SetGameXY(x, y);
		}
	}
	if (UI_LBUTTONDOWN==pMessage->_uMessageType && TALK_MENU==pMessage->_uControlID)
	{
		OnTalkMenuMessage(pMessage);
	}
	if (pMessage->_uControlID!=TALK_MENU && UI_LBUTTONDOWN==pMessage->_uMessageType)
	{
		IMenu *pMenu = dynamic_cast<IMenu *>(m_pIUI->GetIControlbyID(TALK_MENU));
		if (NULL!=pMenu)
		{
			pMenu->SetControlVisible(false);
		}
	}

    if (pMessage->_uControlID!=RIGHTBOARD_USERMENU && UI_LBUTTONDOWN==pMessage->_uMessageType)
    {
        IMenu *pMenu = dynamic_cast<IMenu *>(m_pIUI->GetIControlbyID(RIGHTBOARD_USERMENU));
        if (NULL!=pMenu)
        {
            pMenu->SetControlVisible(false);
        }
    }

	if (pMessage->_uControlID == BN_SHOW_RIGHTFRAME && UI_LBUTTONDOWN == pMessage->_uMessageType)
	{
		if (m_pRightFrame->m_uShowState == SW_HIDE)
		{
			m_pRightFrame->m_uShowState = SW_SHOW;
			OnBnClickedRightFrameShow();
			m_pShowRightFrameBtn->SetControlVisible(false);
		}
	}

    if (UI_LBUTTONUP==pMessage->_uMessageType)
    {
        if (BN_CLOSE_FRAME==pMessage->_uControlID)
        {
            if (!m_pGameImpl->OnGameQuiting())
            {
                //if (IDNO==::MessageBox(m_pEngine->GetCWnd()->GetSafeHwnd(), "��Ϸ���ڽ��У���ȷ��Ҫ�˳���", "��ʾ", MB_YESNO))
                {
                    return 0;
                }
            }
            QuitGame();
            return 0;
        }  
        else if (BN_MIN_FRAME==pMessage->_uControlID)
        {
            m_pEngine->GetCWnd()->ShowWindow(SW_MINIMIZE);                        
        }
        else if (BN_SET_FRAME==pMessage->_uControlID ||BN_RESET_FRAME == pMessage->_uControlID)
        {
			CBcfFile f(CBcfFile::GetAppPath() + _T("GameOption.bcf"));
			UINT uMaxType = f.GetKeyVal("FrameControl","max_type",1);

			if (uMaxType == 1) //��ͨ���
			{
				m_pEngine->SwitchSize();
			}
			else if(uMaxType == 2) 
			{
				DEVMODE lpDevMode;
				lpDevMode.dmBitsPerPel=32;

				if (BN_SET_FRAME==pMessage->_uControlID)//�ı�ֱ��ʷ�ʽ�����
				{

					lpDevMode.dmPelsWidth = 1024;
					lpDevMode.dmPelsHeight = 768;

				}
				else if (BN_RESET_FRAME == pMessage->_uControlID)
				{
					lpDevMode.dmPelsWidth = m_nWiondwMetricsX;
					lpDevMode.dmPelsHeight = m_nWiondwMetricsY;
				}

				lpDevMode.dmSize = sizeof(lpDevMode);
				lpDevMode.dmFields = DM_PELSWIDTH|DM_PELSHEIGHT|DM_BITSPERPEL;
				LONG result;
				result = ChangeDisplaySettings(&lpDevMode,0);
				if (result == DISP_CHANGE_SUCCESSFUL)
				{
					ChangeDisplaySettings(&lpDevMode,CDS_UPDATEREGISTRY);
					// ����//ʹ��CDS_UPDATEREGISTRY��ʾ���޸��ǳ־õģ�
					// ����//����ע�����д������ص����� 
				}
				else
				{
					//AfxMessageBox(L"�޸�ʧ�ܣ��ָ�ԭ������!");
					ChangeDisplaySettings(NULL,0);
				}

				SendIpcData(IPC_MAIN_USER, IPC_SUB_SCREEN_SIZE_CHANGE,0,0);
			}
            

			IButton *pBtn = NULL ; 
			if(BN_SET_FRAME==pMessage->_uControlID)
			{
				pBtn = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_SET_FRAME)) ; 
				if(NULL != pBtn)
				{
					pBtn->SetControlVisible(false) ; 
				}
				pBtn = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_RESET_FRAME)) ; 
				if(NULL != pBtn)
				{
					pBtn->SetControlVisible(true) ; 
				}
			}
			else
			{
				pBtn = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_RESET_FRAME)) ; 
				if(NULL != pBtn)
				{
					pBtn->SetControlVisible(false) ; 
				}
				pBtn = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_SET_FRAME)) ; 
				if(NULL != pBtn)
				{
					pBtn->SetControlVisible(true) ; 
				}
			}

			///��󻯺���С�������ز˵��е������Ϣ
			IMenu *pMenu = dynamic_cast<IMenu *>(m_pIUI->GetIControlbyID(RIGHTBOARD_USERMENU));
			if (NULL!=pMenu)
			{
				pMenu->SetControlVisible(false);
			}
        }
        else if (203==pMessage->_uControlID)
        {
            // ���������
            UIShowBiaoQingKuang(pMessage);
            return 0;
        }
        else if (pMessage->_uControlID>=IMG_BIAOQING_0 && pMessage->_uControlID<=IMG_BIAOQING_49 )
        {
            // ��������
            UIHitBiaoQing(pMessage);
            return 0;
        }        
        else if (BN_START_GAME==pMessage->_uControlID)
        {
            SendGameData(ASS_GM_AGREE_GAME,NULL, 0);
            return 0;
        }
        else if (RIGHTBOARD_PROP_BTN_NEXT1==pMessage->_uControlID || RIGHTBOARD_PROP_BTN_NEXT2==pMessage->_uControlID) 
        {
            m_nPropPage++;
            RefreshPropPannel();
            return 0;
        }
        else if (RIGHTBOARD_PROP_BTN_PRE1==pMessage->_uControlID || RIGHTBOARD_PROP_BTN_PRE2==pMessage->_uControlID)
        {
            m_nPropPage--;
            RefreshPropPannel();
            return 0;
        }
        else if (pMessage->_uControlID>=PROP_BTN_BEGIN && pMessage->_uControlID<PROP_BTN_END)
        {
			if (m_GameInfo.dwRoomRule & GRR_CONTEST)
			{
				AFCMessageBox("�Բ���,  ����������ʹ����Ϸ����", "��ܰ��ʾ");
				return 0;
			}
            // �����������һ��
            int nIndex = (pMessage->_uControlID-PROP_BTN_BEGIN)/PROP_IDS;

            OnHitPropBtn(nIndex);
            return 0;
        }
        else if (BN_DLALOG_OK==pMessage->_uControlID
            || BN_DLALOG_CANCEL==pMessage->_uControlID
            || BN_DLALOG_CLOSE==pMessage->_uControlID)
        {
            IContain *pContain = dynamic_cast<IContain *>(m_pIUI->GetIControlbyID(CTN_DIALOG));
            if (NULL!=pContain)
            {
                pContain->SetControlVisible(false);
            }
        }        
        else if (BN_VOLUM_UP==pMessage->_uControlID)
        {
            if (m_pBackAudio)
            {
                int nVol=m_pBackAudio->GetVolume();
                nVol+=5;
                if (nVol>100)
                {
                    nVol = 100;
                }
                m_pBackAudio->SetVolume(nVol);
            }
        }        
        else if (BN_VOLUM_DOWN==pMessage->_uControlID)
        {
            if (m_pBackAudio)
            {
                int nVol=m_pBackAudio->GetVolume();
                nVol-=5;
                if (nVol<0)
                {
                    nVol = 0;
                }
                m_pBackAudio->SetVolume(nVol);
            }
            
        }        
        else if (BN_VOLUM_OFF==pMessage->_uControlID)
        {
            if (m_pBackAudio)
            {
                m_pBackAudio->Pause();
                IButton *pSoundOn = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_VOLUM_ON));
                IButton *pSoundOFF = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_VOLUM_OFF));
                if (pSoundOn && pSoundOFF)
                {
                    pSoundOn->SetControlVisible(true);
                    pSoundOFF->SetControlVisible(false);
                }
            }
        }        
        else if (BN_VOLUM_ON==pMessage->_uControlID)
        {
            if (m_pBackAudio)
            {   
                m_pBackAudio->Continue();
                IButton *pSoundOn = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_VOLUM_ON));
                IButton *pSoundOFF = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_VOLUM_OFF));
                if (pSoundOn && pSoundOFF)
                {
                    pSoundOn->SetControlVisible(false);
                    pSoundOFF->SetControlVisible(true);
                }
            }
        }
		else if (pMessage->_uControlID == RIGHTBOARD_USERMENU)
		{
			IMenu *pMenu;
			GETCTRL(IMenu, pMenu, m_pIUI, RIGHTBOARD_USERMENU);
			int iSel = pMenu->GetSelectIndex();
			if (iSel >= 0)
			{
				UserItemStruct *pUser = (UserItemStruct *)m_ThisDeskPtrArray.GetAt(m_iSelectIndex);
				if (pUser == NULL)
				{
					return 0;
				}

				// duanxiaohui 2011-11-14 �Ҽ��˵�����
				if (!wcscmp(pMenu->GetItemText(iSel), L"  ��ϸ����"))
				{	
					if (m_GameInfo.dwRoomRule & GRR_NOTCHEAT) return 0;
					m_pUserDetailsDlg->SetUserItem(pUser);
				
					IListControl *pList = dynamic_cast<IListControl *>(m_pIUI->GetIControlbyID(RIGHTBOARD_USERLIST));
					if (pList != NULL)
					{
						POINT point;
						::GetCursorPos(&point);

						CRect rect;
						m_pUserDetailsDlg->GetClientRect(&rect);
						m_pUserDetailsDlg->MoveWindow(point.x - rect.Width(), point.y, rect.Width(), rect.Height());
						m_pUserDetailsDlg->ShowWindow(SW_SHOW);
					}
				}
				else if (!wcscmp(pMenu->GetItemText(iSel), L"  �����û���"))
				{
					if (OpenClipboard(NULL) == FALSE)
					{	
						return TRUE;
					}

					if (EmptyClipboard() == FALSE) 
					{
						CloseClipboard();
						return TRUE;
					}

					// ��������
					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(pUser->GameUserInfo.nickName));
					if (hData == NULL) 
					{
						CloseClipboard();
						return TRUE;
					}

					LPTSTR szMemName = (LPTSTR)GlobalLock(hData);

					// duanxiaohui 2011-11-21 ���Ƶ�ճ��������Ҫת����Unicode
					//lstrcpy(szMemName, pUser->GameUserInfo.nickName);

					CString strText;
					strText.Format("%s", pUser->GameUserInfo.nickName);
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

					// ��ʾ����
					IEditorText* _P = NULL;
					GETCTRL(IEditorText, _P, m_pIUI, ETEXT_TALK_INPUT);
					if (_P != NULL)
					{
						_P->SetText(CA2W(pUser->GameUserInfo.nickName));
					}
				}
				else if (!wcscmp(pMenu->GetItemText(iSel), L"  ��Ϊ����"))
				{
					m_pIPC->SendData(IPC_FRIEND, IPC_ASS_CHECKFRIEND, &(pUser->GameUserInfo.dwUserID), sizeof(int));
				}
				else if (!wcscmp(pMenu->GetItemText(iSel), L"  ���������"))
				{
					m_pIPC->SendData(IPC_EMENY, IPC_ASS_CHECKEMENY, &(pUser->GameUserInfo.dwUserID), sizeof(int));
				}
				else if (!wcscmp(pMenu->GetItemText(iSel), L"  �����뿪"))
				{
					long dwCurrentUserID = pUser->GameUserInfo.dwUserID ; 
					if (dwCurrentUserID!=0L)
					{
						//wushuqun 2009.5.19
						//�Թ�ʱ����T��
						if (GetMeUserInfo()->bUserState == USER_WATCH_GAME)
						{
							AFCMessageBox("�Բ����������Թ���Ϸ�����ܽ��д˲���", "��ʾ");
							return true;
						}

						if(GetMeUserInfo()->dwUserID == dwCurrentUserID)
						{
							AFCMessageBox("�Բ���,�޷����Լ����д������","��ʾ");
							return true;
						}

						CString stime;
						stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
						int curtime=atoi(stime);                        //ת��Ϊint
						//if(GetMeUserInfo()->iVipTime < curtime)
						//{
						//	AFCMessageBox("�Բ���,ֻ��vip���д����","��ʾ");
						//	return true;
						//}
						///����Ƿ�Ϊ�Թ� add by rmaon 09.04.27
						if( m_bWatchMode )
						{
							AFCMessageBox("�Բ���,�Թ�״̬��������","��ʾ");
							return true;
						}
						//��������
						UserItemStruct * pUserItem=FindOnLineUser(dwCurrentUserID);
						if (pUserItem==NULL) 
							return TRUE;
						if(pUserItem->GameUserInfo.iVipTime>curtime)
						{
							AFCMessageBox("�Բ���,�Է���Vip,����ʧ��!","��ʾ");
							return TRUE;
						}

						// PengJiLin, 2010-9-10, �������Ƿ������˿������˹���
						CString sPath=CBcfFile::GetAppPath();
						CBcfFile bcfFile( sPath + "..\\Order.bcf");
						int iValue = bcfFile.GetKeyVal(_T("ClassInExe"), _T("UseKickUserProp"), 0);
						if(iValue > 0)      // ��������˿������߿����ܣ�ʹ���µ����˹���
						{
							_TAG_NEW_KICKUSER stKick;
							stKick.dwUserID = GetMeUserInfo()->dwUserID;
							stKick.dwDestID = dwCurrentUserID;
							SendGameData(&stKick,sizeof(stKick),MDM_GR_PROP,ASS_PROP_NEW_KICKUSER,0);
						}
						else
						{
							_TAG_KICKUSER stKick;
							stKick.dwUserID = GetMeUserInfo()->dwUserID;
							stKick.dwDestID = dwCurrentUserID;
							SendGameData(&stKick,sizeof(stKick),MDM_GR_PROP,ASS_PROP_KICKUSER,0);
						}
					}
					
				}
			}
		}
    }

	//������ϢFF
	if(pMessage->_uMessageType == UI_CATOONFINISH)
	{
		if (pMessage->_uControlID >= USERPROP_ANIMAL_0 && pMessage->_uControlID <= USERPROP_ANIMAL_0+10)
		{
			CString strFilePathName; 
			strFilePathName.Format("..\\image\\prop\\PropGif%dB.bani",m_stProp.iValue%1000); //modify by lxl  ͼƬ��ʽ�޸�gif->spr

			//�͵���λ�ý���
			if (DeskStation2View(m_stProp.sendDeskStation) == pMessage->_uControlID - USERPROP_ANIMAL_0)
			{
				PlayProp(false,strFilePathName);
			}
		}
	}
    m_pGameImpl->OnUIMessage(pMessage);
    return 0;
}

void XLogic::PlayProp(const bool& isSend,const CString& strFilePathName)
{
	IExAnimate* pExAnimate=NULL;
	GETCTRL(IExAnimate,pExAnimate,m_pIUI,
		USERPROP_ANIMAL_0+DeskStation2View((isSend) ? m_stProp.sendDeskStation : m_stProp.recvDeskStation));
	
	if (NULL != pExAnimate)
	{
		pExAnimate->SetExtFilePath(CA2W(strFilePathName));
		pExAnimate->LoadFromFile();
		pExAnimate->SetPlayState(1);
		pExAnimate->SetIsSymmetrical(false);
		pExAnimate->SetIsLoop(false) ; 
		pExAnimate->SetControlVisible(true);
	}
}

/// ʱ��
void XLogic::OnTimer(int nEventID)
{
    if (OLEDRAW_TIMER_ID==nEventID)
    {
        IWeb *pweb = dynamic_cast<IWeb *>(m_pIUI->GetIControlbyID(BUYPROP_WEB_PROPINFO));
        if (pweb!=NULL)
        {
            pweb->Draw();
        }
        
        return;
    }
    m_pGameImpl->OnTimer(nEventID);
}




// ��ʼ�������
void XLogic::UIInitBiaoQingKuang()
{
    // �����
    IContain *pContain = NULL;
    GETCTRL(IContain, pContain, m_pIUI, CTN_BIAOQING_KUANG);
    if (NULL==pContain)
    {
        return;
    }
    pContain->SetControlVisible(false);
    // ���鱳��
    IImage *pImgBK = NULL;
    GETCTRL(IImage, pImgBK, m_pIUI, IMG_BIAOQING_BJ);
    if (NULL==pImgBK)
    {
        return;
    }
    // ����ID
    int nBiaoQingID[50];
    // ����ͼƬ 
    IImage *pBiaoQingImg[50];
    // ��ȡ����ͼƬ
    for (int i=0; i<50; ++i)
    {
        nBiaoQingID[i] = IMG_BIAOQING_0+i;
        pBiaoQingImg[i] = NULL;
        GETCTRL(IImage, pBiaoQingImg[i], m_pIUI, nBiaoQingID[i]);
        if (NULL==pBiaoQingImg[i])
        {
            return;
        }
    }    
    // ��һ�������λ�ã����Ͻǣ�
    int nStartX = pBiaoQingImg[0]->GetGameX();
    int nStartY = pBiaoQingImg[0]->GetGameY();

    int nOffsetX = pImgBK->GetPicWidth()/10;
    int nOffsetY = nOffsetX;//pImgBK->GetPicHeight()/5;

    int nSrcOffsetX = pBiaoQingImg[0]->GetPicWidth()/10;
    int nSrcOffsetY = nSrcOffsetX;//pBiaoQingImg[0]->GetPicHeight()/5;

    // ��ʼ��������ͼƬλ��
    for (int i=1; i<50; ++i)
    {
        pBiaoQingImg[i]->SetGameXY(nStartX+(i%10)*nOffsetX, nStartY+(i/10)*nOffsetY);
        pBiaoQingImg[i]->SetSrcX(nSrcOffsetX*(i%10));
        pBiaoQingImg[i]->SetSrcY(nSrcOffsetY*(i/10));
    }

}
// ���������
void XLogic::UIShowBiaoQingKuang(TUIMessage *pMessage)
{
    // �����
    static IContain *pContain = NULL;
    if (NULL==pContain)
    {
        GETCTRL(IContain, pContain, m_pIUI, CTN_BIAOQING_KUANG);
        if (NULL==pContain)
        {
            return;
        }
    }

    pContain->SetControlVisible(!pContain->GetVisible());
}

// ��������
void XLogic::UIHitBiaoQing(TUIMessage *pMessage)
{
    // ����Index
    int nIndex = pMessage->_uControlID-IMG_BIAOQING_0;

    IEditorText* _P=NULL;
    GETCTRL(IEditorText,_P,m_pIUI, ETEXT_TALK_INPUT);
    TCHAR szText[255];
    TCHAR* _Msg = NULL;
    if (_P != NULL)
    {
        _Msg = CW2T(_P->GetText()); 
    }

    if (NULL==_Msg)
    {
        _stprintf(szText, TEXT("/:%3d"), nIndex);
        SendTalkMsg(szText, 0);
        //_P->SetText(CA2W(szText));
    }
    else
    {
        _stprintf(szText, TEXT("%s/:%3d"), _Msg, nIndex);
        SendTalkMsg(szText, 0);
        _P->Clear();
        //_P->SetText(CA2W(szText));        
    }

    // �����
    UIShowBiaoQingKuang(NULL);
}

void XLogic::SendTalkMsg(TCHAR* pMsg, int iUserID)
{
	if (!pMsg)
	{
		return;
	}

	if (strlen(pMsg) == 0)
	{
		return;
	}

	m_pRightFrame->SetFocusToTalkInput();
	if (m_pRightFrame->GetTalkInputMessage(NULL) == 0) return;

	//��ֹ����ķ��䲻���ͽ�ֹ��Ϸ����
	if (m_GameInfo.dwRoomRule & GRR_FORBID_GAME_TALK)
	{
		InsertSystemMessage(TEXT("��Ǹ���������ֹ����"));
		return;
	}

	//�ж��Ƿ��������
	if (CUserPower::CanGameTalk(m_GameInfo.dwGamePower)==false) 
	{
		InsertSystemMessage( TEXT("��Ǹ������ʱû�������Ȩ��!"));
		return;
	}
    //����������Ϣ
    MSG_GR_RS_NormalTalk Talk;
    ZeroMemory(&Talk,sizeof(MSG_GR_RS_NormalTalk));
    Talk.iLength=(short)strlen(pMsg);
    Talk.dwSendID=GetMeUserInfo()->dwUserID;
    Talk.dwTargetID=iUserID;
    Talk.nDefaultIndex=0;
    strcpy(Talk.szMessage,pMsg);
    //Talk.crColor=m_colorresource.m_crNormalTxColor;
    SendGameData(&Talk,sizeof(Talk)-sizeof(Talk.szMessage)+Talk.iLength+1,MDM_GM_GAME_FRAME,ASS_GM_NORMAL_TALK,0);
}

int XLogic::ShowOneUserInfo(int nControlIndex,UserItemStruct * pUserInfo,bool bVisible)
{
	UserInfoStruct *pMeInfo = GetMeUserInfo();
	if (pUserInfo == NULL || pMeInfo == NULL) return 0;
	if (pUserInfo->GameUserInfo.dwUserID == pMeInfo->dwUserID)//(nControlIndex==m_GameInfo.uDeskPeople/2)        // ���Լ�
    {
        UserInfoStruct *pMeInfo = GetMeUserInfo();

        IText *pTextName = NULL;
        IText *pTextID = NULL;
        IText *pTextJifen = NULL;
        IText *pTextJingbi = NULL;
        IText *pTextMeili = NULL;
        IText *pTextDenji = NULL;
        GETCTRL(IText, pTextName, m_pIUI, RIGHTBOARD_TEXT_NICKNAME);
        GETCTRL(IText, pTextID, m_pIUI,    RIGHTBOARD_TEXT_ID    ); 
        GETCTRL(IText, pTextJifen, m_pIUI, RIGHTBOARD_TEXT_JIFEN );
        GETCTRL(IText, pTextJingbi, m_pIUI,RIGHTBOARD_TEXT_JINGBI);
        GETCTRL(IText, pTextMeili, m_pIUI, RIGHTBOARD_TEXT_MEILI );
        GETCTRL(IText, pTextDenji, m_pIUI, RIGHTBOARD_TEXT_DENJI );

		//��ȡ��ʾ�û���Ϣ�����ļ�zht2011-11-07 zht�޸�Ϊ������
		CBcfFile bcfFile( CBcfFile::GetAppPath () + "..\\GameUserInfor.bcf");

		//��ʼ��Ĭ����ʾ��Ŀ�ı�ʶ
		bool bUserName = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveUserName"), 0); //�û���
		bool bUserID = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveUserID"), 0);		//ID
		bool bNickName = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveNickName"), 0);	//�ǳ�
		bool bScore = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveScore"), 0);	//����
		bool bCoin = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveCoin"), 0);			//���	
		bool bCharm = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveCharm"), 0);		//����
		bool bWealthLevel = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveWealthLevel"), 0);	//�Ƹ��ȼ�

		CString szCharm = bcfFile.GetKeyVal(_T("InfoName"), _T("Charm"),_T("��  ����"));
		CString szCoin = bcfFile.GetKeyVal(_T("InfoName"), _T("Coin"),_T("��  �ң�"));
		CString szNickName = bcfFile.GetKeyVal(_T("InfoName"), _T("NickName"),_T("��  �ƣ�"));
		CString szScore = bcfFile.GetKeyVal(_T("InfoName"), _T("Score"),_T("��  �֣�"));
		CString szUserID = bcfFile.GetKeyVal(_T("InfoName"), _T("UserID"),_T("��  �ģ�"));
		CString szUserName = bcfFile.GetKeyVal(_T("InfoName"), _T("UserName"),_T("�û�����"));
		CString szWealthLevel = bcfFile.GetKeyVal(_T("InfoName"), _T("WealthLevel"),_T("��  ����"));

        
        char path[MAX_PATH]={0};
		int y = 0;
		int x = 0;
        if (pTextName!=NULL)
        {
			if (bNickName)
			{
				sprintf(path,"%s%s",szNickName,pMeInfo->nickName);
				pTextName->SetText(CA2W(path));
				y = pTextName->GetGameY();
				x = pTextName->GetGameX();
				y += 18;
			}
			else if(bUserName)
			{
				sprintf(path,"%s%s",szUserName,pMeInfo->szName);
				pTextName->SetText(CA2W(path));
				y = pTextName->GetGameY();
				x = pTextName->GetGameX();
				y += 18;
			}
			else
			{
				pTextName->SetControlVisible(false);
			}
		
        }
        if (pTextID!=NULL)
		{
			if (bUserID)
			{
				sprintf(path,"%s%d",szUserID,pMeInfo->dwUserID);
				pTextID->SetText(CA2W(path));
				if (y>0)
				{
					pTextID->SetGameXY(x,y);
					y += 18;
				}
				else
				{
					y = pTextID->GetGameY();
					x = pTextID->GetGameX();
					pTextID->SetGameXY(x,y);
					y += 18;
				}
			}
			else
			{
				pTextID->SetControlVisible(false);
			}
        }
		if (pTextJifen!=NULL)
		{
			if (bScore)
			{
				if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
				{
					sprintf(path,"%s???",szScore);
				}
				else
				{
					sprintf(path,"%s%d",szScore,pMeInfo->dwPoint);
				}
				
				pTextJifen->SetText(CA2W(path));
				if (y>0)
				{
					pTextJifen->SetGameXY(x,y);
					y += 18;
				}
				else
				{
					y = pTextJifen->GetGameY();
					x = pTextJifen->GetGameX();
					pTextJifen->SetGameXY(x,y);
					y += 18;
				}
			}
			else
			{
				pTextJifen->SetControlVisible(false);
			}
        }
        if (pTextJingbi!=NULL)
        {
            /*CString strMoneyName;
            static bool bConfigReaded = false;
            if (!bConfigReaded)
            {
                CString szAppPath = CINIFile::GetAppPath();
                szAppPath += "/GameOption.bcf";
                CBcfFile f(szAppPath.GetBuffer());
                strMoneyName=f.GetKeyVal("NewGameFrame","MoneyName","���");
				}*/
			if (bCoin)
			{
				GlbGetNumString(path, pMeInfo->i64Money, Glb().m_nPowerOfGold, Glb().m_bUseSpace, TEXT(","));
				char sz[300]={0};
				sprintf(sz,"%s%s",szCoin.GetBuffer(),path);
				if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
				{
					sprintf(sz,"%s%s",szCoin.GetBuffer(),"???");
				}
				pTextJingbi->SetText(CA2W(sz));
				if (y>0)
				{
					pTextJingbi->SetGameXY(x,y);
					y += 18;
				}
				else
				{
					y = pTextJingbi->GetGameY();
					x = pTextJingbi->GetGameX();
					pTextJingbi->SetGameXY(x,y);
					y += 18;
				}
			}
			else
			{
				pTextJingbi->SetControlVisible(false);
			}
        }
        if (pTextMeili!=NULL)
        {
			if (bCharm)
			{
				if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
				{
					sprintf(path,"%s???",szCharm);
				}
				else
				{
					sprintf(path,"%s%d",szCharm,pMeInfo->dwFascination);
				}
				
				pTextMeili->SetText(CA2W(path));
				if (y>0)
				{
					pTextMeili->SetGameXY(x,y);
					y += 18;
				}
				else
				{
					y = pTextMeili->GetGameY();
					x = pTextMeili->GetGameX();
					pTextMeili->SetGameXY(x,y);
					y += 18;
				}
			}
			else
			{
				pTextMeili->SetControlVisible(false);
			}
		}
        if (pTextDenji!=NULL)
        {
			if (bWealthLevel)
			{
				if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
				{
					sprintf(path,"%s???",szWealthLevel);
				}
				else
				{
					sprintf(path,"%s%s",szWealthLevel,GetMoneyOrderName(pMeInfo->i64Money));
				}
				
				pTextDenji->SetText(CA2W(path));
				if (y>0)
				{
					pTextDenji->SetGameXY(x,y);
					y += 18;
				}
				else
				{
					y = pTextDenji->GetGameY();
					x = pTextDenji->GetGameX();
					pTextDenji->SetGameXY(x,y);
					y += 18;
				}
			}
			else
			{
				pTextDenji->SetControlVisible(false);
			}
        }
    }


    char path[MAX_PATH]={0};
    ///�ȼ�
    IText *pText;
    GETCTRL(IText,pText,m_pIUI,TEXT_LEV+nControlIndex);
    if(pText!=NULL)
    {
        sprintf(path,"%d",0);
        pText->SetText(CA2W(path));
        pText->SetFontShowStyle(AlignmentCenter);
        char tip[512];
        memset(tip, 0, 512 * sizeof(TCHAR));
       /* sprintf(tip, "<font><color=0,size=16,style=����>��ҵȼ�</font>");
        pText->SetTooltip(CA2W(tip));*/
        pText->SetControlVisible(bVisible);
    }

    ///�ʺ�
    GETCTRL(IText,pText,m_pIUI,TEXT_ACCOUNT+nControlIndex);
    if(pText!=NULL)
    {
		if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID))
		{
			 sprintf(path,"���",pUserInfo->GameUserInfo.dwUserID);
		}
		else
		{
			 sprintf(path,"%d",pUserInfo->GameUserInfo.dwUserID);
		}
       
        pText->SetText(CA2W(path));
        pText->SetFontShowStyle(AlignmentCenter);
        wchar_t tip[512];
        memset(tip, 0, 512 * sizeof(TCHAR));
        /*wsprintfW(tip, L"<font><color=0,size=16,style=����>����ʺ�</font>");
        pText->SetTooltip(tip);*/
        pText->SetControlVisible(bVisible);
    }

	if (m_GameInfo.uDeskPeople > 10)
	{
		return 0; //����10�˵Ĳ�����
	}
    ///�ǳ�
    GETCTRL(IText,pText,m_pIUI,TEXT_NICKNAME+nControlIndex);
    if(pText!=NULL)
    {
		if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID))
		{
			 sprintf(path,"���");
		}
		else
		{
			 sprintf(path,"%s",pUserInfo->GameUserInfo.nickName);
		}
      
        pText->SetText(CA2W(path));
        pText->SetFontShowStyle(AlignmentCenter);
        pText->SetControlVisible(bVisible);
    }
    ///�ǳƵ�ͼ
    IImage * pImage=NULL;
    GETCTRL(IImage,pImage,m_pIUI,IMAGE_NICKNAME_BK+nControlIndex);
    if(pImage!=NULL)
    {
        pImage->SetControlVisible(bVisible);
    }

    //const TCHAR C_LogoSex[2][8] = {TEXT("boy"), TEXT("girl")};
    //const TCHAR C_LogoName[2][8] = {TEXT("left"), TEXT("right")};

    
    //����ͷ��
    CString szAppPath = CINIFile::GetAppPath();
    szAppPath.SetAt(szAppPath.GetLength()-1, TCHAR('\0'));
    int nPosOfSlash = szAppPath.ReverseFind(TCHAR('\\'));
    CString szAppPathParent = szAppPath.Left(nPosOfSlash);
    TCHAR szBuffer[200]={0};
   /* if (m_IsUserSmall[nControlIndex])
    {
        DebugPrintfA("ʹ��Сͷ��");
        wsprintf(szBuffer,TEXT("%s/image\\log\\%s\\gamelog%ld.png"), szAppPathParent,pUserInfo->GameUserInfo.bBoy?"Boy":"Girl", 
            (pUserInfo->GameUserInfo.bLogoID%0x100)%6);
    }
    else
    {
        DebugPrintfA("ʹ�ô�ͷ��");
        wsprintf(szBuffer,TEXT("%s/image\\log\\log2\\%s\\gamelog%ld.png"), szAppPathParent,pUserInfo->GameUserInfo.bBoy?"Boy":"Girl", 
            (pUserInfo->GameUserInfo.bLogoID%0x100)%6);
    }*/
    
// 	int iSitGrade = GetMoneyGrade(pUserInfo->GameUserInfo.i64Money);
// 
// 	wsprintf(szBuffer,"..\\image\\log\\2d\\%s_%d.png",pUserInfo->GameUserInfo.bBoy?"boy":"girl",iSitGrade - 1);

	int iSitGrade = 0;//GetMoneyGrade(pUserInfo->GameUserInfo.i64Money);
	//iSitGrade = GetMoneyGrade(pUserInfo->GameUserInfo.i64Money);
	iSitGrade = BzGetAvatarIndex(pUserInfo);

	wsprintf(szBuffer,"..\\image\\log\\%s\\gamelog%d_big.png",pUserInfo->GameUserInfo.bBoy?"boy":"girl",iSitGrade);


    //����ͷ��
    IImage * pImageLogo=NULL;
    GETCTRL(IImage,pImageLogo,m_pIUI,IMAGE_LOGO+nControlIndex);
    if(pImageLogo!=NULL)
    {
        pImageLogo->LoadPic(CT2W(szBuffer));
        pImageLogo->SetControlVisible(bVisible);
    }
    IImage *pImageLogoFrame = NULL;
    GETCTRL(IImage, pImageLogoFrame, m_pIUI, IMAGE_LOGO_FRAME+nControlIndex);
    if (pImageLogoFrame!=NULL)
    {
        pImageLogoFrame->SetControlVisible(bVisible);
    }

    //int nWhichSide = 0;
    //if (nControlIndex<(m_GameInfo.uDeskPeople/2))
    //{
    //    nWhichSide = 1;
    //}
    //IExAnimate *pExAmimate=NULL;
    //GETCTRL(IExAnimate,pExAmimate,m_pIUI,IMAGE_LOGO+nControlIndex);
    //if(pExAmimate)
    //{
    //    pExAmimate->SetPlayState(bVisible);
    //    if (bVisible)
    //    {
    //        int index = pUserInfo->bBoy? 0: 1;
    //        int nWhichSide = (nControlIndex<(m_GameInfo.uDeskPeople/2))? 1: 0;
    //        wsprintf(path, _T("client\\anim\\%s_%d%s.bani"), C_LogoSex[index], pUserInfo->iImageNO % GAME_LOG_COUNT, C_LogoName[nWhichSide]);
    //        pExAmimate->SetExtFilePath(path);
    //        pExAmimate->LoadFromFile();
    //        pExAmimate->SetCurveInstance(CT_NONE);//����
    //        pExAmimate->SetIsLoop(true);//ѭ������
    //    }

    //}


    ///���1
    GETCTRL(IText,pText,m_pIUI,TEXT_MONEY1+nControlIndex);
    if(pText!=NULL)
    {
		GlbGetNumString(path, pUserInfo->GameUserInfo.i64Money, Glb().m_nPowerOfGold, Glb().m_bUseSpace, TEXT(","));

		if(pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID)
		{
			if (m_GameInfo.dwRoomRule & GRR_NOTCHEAT)
			{
				sprintf(path,"???");
			}
		}
		else
		{
			if (m_GameInfo.dwRoomRule & GRR_CONTEST)
			{
				GlbGetNumString(path, pUserInfo->GameUserInfo.i64ContestScore, Glb().m_nPowerOfGold, Glb().m_bUseSpace, TEXT(","));
			}
		}

		CString str;
		str.Format("DClient:path = %s",path);
		OutputDebugString(str);

        pText->SetText(CA2W(path));
        pText->SetFontShowStyle(AlignmentCenter);
        char tip[512];
        memset(tip, 0, 512 * sizeof(TCHAR));
       /* sprintf(tip, "<font><color=0,size=16,style=����>��ҽ��</font>");
        pText->SetTooltip(CA2W(tip));*/
        pText->SetControlVisible(bVisible);
    }

    ///���2
    GETCTRL(IText,pText,m_pIUI,TEXT_MONEY2+nControlIndex);
    if(pText!=NULL)
    {
		GlbGetNumString(path, pUserInfo->GameUserInfo.i64Money, Glb().m_nPowerOfGold, Glb().m_bUseSpace, TEXT(","));

		if(pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID)
		{
			if (m_GameInfo.dwRoomRule & GRR_NOTCHEAT)
			{
				sprintf(path,"???");
			}
		}
		else
		{
			if (m_GameInfo.dwRoomRule & GRR_CONTEST)
			{
				GlbGetNumString(path, pUserInfo->GameUserInfo.i64ContestScore, Glb().m_nPowerOfGold, Glb().m_bUseSpace, TEXT(","));
			}
		}

		CString str;
		str.Format("DClient:path = %s",path);
		OutputDebugString(str);
// 		if (m_GameInfo.dwRoomRule & GRR_CONTEST)
// 		{
// 			
// 		}
        
        pText->SetText(CA2W(path));
        pText->SetFontShowStyle(AlignmentCenter);
        char tip[512];
        memset(tip, 0, 512 * sizeof(TCHAR));
      /*  sprintf(tip, "<font><color=0,size=16,style=����>���Ԫ��</font>");
        pText->SetTooltip(CA2W(tip));*/
        pText->SetControlVisible(bVisible);
    }

   // RefreshPlayList();

    return 0;
}
int XLogic::ShowCurrentUserInfo(UserItemStruct * pUserInfo,bool bVisible)
{
	UserInfoStruct *pMeInfo = &pUserInfo->GameUserInfo;

	if (pUserInfo == NULL || pMeInfo == NULL) return 0;
	if (pUserInfo->GameUserInfo.dwUserID == pMeInfo->dwUserID)//(nControlIndex==m_GameInfo.uDeskPeople/2)        // ���Լ�
	{
		IText *pTextName = NULL;
		IText *pTextID = NULL;
		IText *pTextJifen = NULL;
		IText *pTextJingbi = NULL;
		IText *pTextMeili = NULL;
		IText *pTextDenji = NULL;
		GETCTRL(IText, pTextName, m_pIUI, RIGHTBOARD_TEXT_NICKNAME);
		GETCTRL(IText, pTextID, m_pIUI,    RIGHTBOARD_TEXT_ID    ); 
		GETCTRL(IText, pTextJifen, m_pIUI, RIGHTBOARD_TEXT_JIFEN );
		GETCTRL(IText, pTextJingbi, m_pIUI,RIGHTBOARD_TEXT_JINGBI);
		GETCTRL(IText, pTextMeili, m_pIUI, RIGHTBOARD_TEXT_MEILI );
		GETCTRL(IText, pTextDenji, m_pIUI, RIGHTBOARD_TEXT_DENJI );

		//��ȡ��ʾ�û���Ϣ�����ļ�zht2011-11-07 zht�޸�Ϊ������
		CBcfFile bcfFile( CBcfFile::GetAppPath () + "..\\GameUserInfor.bcf");

		//��ʼ��Ĭ����ʾ��Ŀ�ı�ʶ
		bool bUserName = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveUserName"), 0); //�û���
		bool bUserID = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveUserID"), 0);		//ID
		bool bNickName = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveNickName"), 0);	//�ǳ�
		bool bScore = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveScore"), 0);	//����
		bool bCoin = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveCoin"), 0);			//���	
		bool bCharm = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveCharm"), 0);		//����
		bool bWealthLevel = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveWealthLevel"), 0);	//�Ƹ��ȼ�

		CString szCharm = bcfFile.GetKeyVal(_T("InfoName"), _T("Charm"),_T("��  ����"));
		CString szCoin = bcfFile.GetKeyVal(_T("InfoName"), _T("Coin"),_T("��  �ң�"));
		CString szNickName = bcfFile.GetKeyVal(_T("InfoName"), _T("NickName"),_T("��  �ƣ�"));
		CString szScore = bcfFile.GetKeyVal(_T("InfoName"), _T("Score"),_T("��  �֣�"));
		CString szUserID = bcfFile.GetKeyVal(_T("InfoName"), _T("UserID"),_T("��  �ģ�"));
		CString szUserName = bcfFile.GetKeyVal(_T("InfoName"), _T("UserName"),_T("�û�����"));
		CString szWealthLevel = bcfFile.GetKeyVal(_T("InfoName"), _T("WealthLevel"),_T("��  ����"));


		char path[MAX_PATH]={0};
		int y = 0;
		int x = 0;
		if (pTextName!=NULL)
		{
			if (bNickName)
			{
				sprintf(path,"%s%s",szNickName,pMeInfo->nickName);
				if ((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID)//�����׳�:�����Լ�����ʾ���� lym 2011- 12-1
				{			
					sprintf(path,"%s���",szNickName);
				}
				pTextName->SetText(CA2W(path));
				y = pTextName->GetGameY();
				x = pTextName->GetGameX();
				y += 18;
			}
			else if(bUserName)
			{
				if ((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID)//�����׳�:�����Լ�����ʾ���� lym 2011- 12-1
				{			
					sprintf(path,"%s���",szNickName);
				}
				sprintf(path,"%s%s",szUserName,pMeInfo->szName);
				pTextName->SetText(CA2W(path));
				y = pTextName->GetGameY();
				x = pTextName->GetGameX();
				y += 18;
			}
			else
			{
				pTextName->SetControlVisible(false);
			}

		}
		if (pTextID!=NULL)
		{
			if (bUserID)
			{
				sprintf(path,"%s%d",szUserID,pMeInfo->dwUserID);
				if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID))//�����׳�:�����Լ�����ʾ���� lym 2011- 12-1
				{			
					sprintf(path,"%s???",szUserID);
				}
				pTextID->SetText(CA2W(path));
				if (y>0)
				{
					pTextID->SetGameXY(x,y);
					y += 18;
				}
				else
				{
					y = pTextID->GetGameY();
					x = pTextID->GetGameX();
					pTextID->SetGameXY(x,y);
					y += 18;
				}
			}
			else
			{
				pTextID->SetControlVisible(false);
			}
		}
		if (pTextJifen!=NULL)
		{
			if (bScore)
			{
				sprintf(path,"%s%d",szScore,pMeInfo->dwPoint);
				if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
				{	
					sprintf(path,"%s???",szScore);
				}
				if (m_GameInfo.dwRoomRule & GRR_CONTEST)
				{
					sprintf(path,"%s???",szScore);
				}
				pTextJifen->SetText(CA2W(path));
				if (y>0)
				{
					pTextJifen->SetGameXY(x,y);
					y += 18;
				}
				else
				{
					y = pTextJifen->GetGameY();
					x = pTextJifen->GetGameX();
					pTextJifen->SetGameXY(x,y);
					y += 18;
				}
			}
			else
			{
				pTextJifen->SetControlVisible(false);
			}
		}
		if (pTextJingbi!=NULL)
		{
			/*CString strMoneyName;
			static bool bConfigReaded = false;
			if (!bConfigReaded)
			{
			CString szAppPath = CINIFile::GetAppPath();
			szAppPath += "/GameOption.bcf";
			CBcfFile f(szAppPath.GetBuffer());
			strMoneyName=f.GetKeyVal("NewGameFrame","MoneyName","���");
			}*/
			if (bCoin)
			{
				GlbGetNumString(path, pMeInfo->i64Money, Glb().m_nPowerOfGold, Glb().m_bUseSpace, TEXT(","));
				char sz[300]={0};
				sprintf(sz,"%s%s",szCoin.GetBuffer(),path);
				if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
				{			
					sprintf(sz,"%s???",szCoin.GetBuffer());
				}
				if (m_GameInfo.dwRoomRule & GRR_CONTEST)
				{
					sprintf(sz,"%s???",szCoin.GetBuffer());
				}
				pTextJingbi->SetText(CA2W(sz));
				if (y>0)
				{
					pTextJingbi->SetGameXY(x,y);
					y += 18;
				}
				else
				{
					y = pTextJingbi->GetGameY();
					x = pTextJingbi->GetGameX();
					pTextJingbi->SetGameXY(x,y);
					y += 18;
				}
			}
			else
			{
				pTextJingbi->SetControlVisible(false);
			}
		}
		if (pTextMeili!=NULL)
		{
			if (bCharm)
			{
				sprintf(path,"%s%d",szCharm,pMeInfo->dwFascination);
				if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
				{			
					sprintf(path,"%s???",szCharm);
				}
				pTextMeili->SetText(CA2W(path));
				if (y>0)
				{
					pTextMeili->SetGameXY(x,y);
					y += 18;
				}
				else
				{
					y = pTextMeili->GetGameY();
					x = pTextMeili->GetGameX();
					pTextMeili->SetGameXY(x,y);
					y += 18;
				}
			}
			else
			{
				pTextMeili->SetControlVisible(false);
			}
		}
		if (pTextDenji!=NULL)
		{
			if (bWealthLevel)
			{
				sprintf(path,"%s%s",szWealthLevel,GetMoneyOrderName(pMeInfo->i64Money));
				if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
				{			
					sprintf(path,"%s???",szWealthLevel);
				}
				if (m_GameInfo.dwRoomRule & GRR_CONTEST)
				{
					sprintf(path,"%s???",szWealthLevel);
				}
				pTextDenji->SetText(CA2W(path));
				if (y>0)
				{
					pTextDenji->SetGameXY(x,y);
					y += 18;
				}
				else
				{
					y = pTextDenji->GetGameY();
					x = pTextDenji->GetGameX();
					pTextDenji->SetGameXY(x,y);
					y += 18;
				}
			}
			else
			{
				pTextDenji->SetControlVisible(false);
			}
		}

		//ShowOneUserInfo(pUserInfo,bVisible);
		/*IText *pTextName = NULL;
		IText *pTextID = NULL;
		IText *pTextJifen = NULL;
		IText *pTextJingbi = NULL;
		IText *pTextMeili = NULL;
		IText *pTextDenji = NULL;
		GETCTRL(IText, pTextName, m_pIUI, RIGHTBOARD_TEXT_NICKNAME);
		GETCTRL(IText, pTextID, m_pIUI,    RIGHTBOARD_TEXT_ID    ); 
		GETCTRL(IText, pTextJifen, m_pIUI, RIGHTBOARD_TEXT_JIFEN );
		GETCTRL(IText, pTextJingbi, m_pIUI,RIGHTBOARD_TEXT_JINGBI);
		GETCTRL(IText, pTextMeili, m_pIUI, RIGHTBOARD_TEXT_MEILI );
		GETCTRL(IText, pTextDenji, m_pIUI, RIGHTBOARD_TEXT_DENJI );

		char path[MAX_PATH]={0};
		if (pTextName!=NULL)
		{
			sprintf(path,"�ǳ�:%s",pMeInfo->nickName);
			pTextName->SetText(CA2W(path));
		}
		if (pTextID!=NULL)
		{
			sprintf(path,"I  D:%d",pMeInfo->dwUserID);
			pTextID->SetText(CA2W(path));
		}
		if (pTextJifen!=NULL)
		{
			sprintf(path,"����:%d",pMeInfo->dwPoint);
			pTextJifen->SetText(CA2W(path));
		}
		if (pTextJingbi!=NULL)
		{
			CString strMoneyName;
			static bool bConfigReaded = false;
			if (!bConfigReaded)
			{
				CString szAppPath = CINIFile::GetAppPath();
				szAppPath += "/GameOption.bcf";
				CBcfFile f(szAppPath.GetBuffer());
				strMoneyName=f.GetKeyVal("NewGameFrame","MoneyName","���");
			}
			GlbGetNumString(path, pMeInfo->i64Money, Glb().m_nPowerOfGold, false, TEXT(","));
			char sz[300]={0};
			sprintf(sz,"%s:%s",strMoneyName.GetBuffer(),path);
			pTextJingbi->SetText(CA2W(sz));
		}
		if (pTextMeili!=NULL)
		{
			sprintf(path,"����:%d",pMeInfo->dwFascination);
			pTextMeili->SetText(CA2W(path));
		}
		if (pTextDenji!=NULL)
		{
			sprintf(path,"�ȼ�:%s",m_GameInfo.pOrderName(pMeInfo->dwPoint));
			pTextDenji->SetText(CA2W(path));
		}*/
	}
	return 0;
}


/// ����Ϣ
bool XLogic::SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
    if (m_GameInfo.uisMeUserInfo.GameUserInfo.bDeskNO==255) 
        return 0;

    UINT nBufferLen = uBufLen + sizeof(SendGameDataStructWithoutParam);
    BYTE *buffer = new BYTE[nBufferLen];
    SendGameDataStructWithoutParam *pHead = (SendGameDataStructWithoutParam *)buffer;
    pHead->bMainID		= bMainID;
    pHead->bAssistantID = bAssistantID;
    pHead->bHandleCode	= bHandleCode;
    memcpy(buffer+sizeof(SendGameDataStructWithoutParam), pData, uBufLen);


    bool bRet = false;
    if (NULL != m_pIPC)
    {
        bRet = m_pIPC->SendData(IPC_MAIN_SOCKET, IPC_SUB_SOCKET_SEND, buffer, nBufferLen);
    }
    delete []buffer;
    buffer = NULL;
    return bRet;

    return true;
}

//���ͺ���
int XLogic::SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
    if (m_GameInfo.uisMeUserInfo.GameUserInfo.bDeskNO==255)
        return 0;
    SendGameDataStructWithoutParam *pHead = new SendGameDataStructWithoutParam;
    pHead->bMainID		= bMainID;
    pHead->bAssistantID = bAssistantID;
    pHead->bHandleCode	= bHandleCode;
    WORD nLen = sizeof(SendGameDataStructWithoutParam);

    bool bRet = false;
    if (NULL != m_pIPC)
    {
        bRet = m_pIPC->SendData(IPC_MAIN_SOCKET, IPC_SUB_SOCKET_SEND, pHead, nLen);
    }
    delete []pHead;
    pHead = NULL;
    return bRet;

    //m_pGameRoom->PostMessage(WM_SEND_GAME_DATA, (WPARAM)nLen, (LPARAM)pHead);
    return 0;
}

//�뿪��Ϸ
void XLogic::QuitGame(bool bNotify,bool bNeedCheck)
{
    //SetStationParameter(0);
    //CWnd * winApp=AfxGetApp()->m_pMainWnd;
    //if(winApp && winApp->m_hWnd){
    //    winApp->PostMessage(WM_GAME_QUIT,0,0);	// �̲β�Ϊ0����ʾ��֪ͨ����
    //}

	if (bNeedCheck)
	{
		if (!m_pGameImpl->OnGameQuiting())
		{
			return;
		}
	}

	CBcfFile f(CBcfFile::GetAppPath() + _T("GameOption.bcf"));
	UINT uMaxType = f.GetKeyVal("FrameControl","max_type",1);

	if(uMaxType == 2) 
	{
		DEVMODE lpDevMode;
		lpDevMode.dmBitsPerPel=32;

		lpDevMode.dmPelsWidth = m_nWiondwMetricsX;
		lpDevMode.dmPelsHeight = m_nWiondwMetricsY;

		lpDevMode.dmSize = sizeof(lpDevMode);
		lpDevMode.dmFields = DM_PELSWIDTH|DM_PELSHEIGHT|DM_BITSPERPEL;
		LONG result;
		result = ChangeDisplaySettings(&lpDevMode,0);
		if (result == DISP_CHANGE_SUCCESSFUL)
		{
			ChangeDisplaySettings(&lpDevMode,CDS_UPDATEREGISTRY);
			// ����//ʹ��CDS_UPDATEREGISTRY��ʾ���޸��ǳ־õģ�
			// ����//����ע�����д������ص����� 
		}
		else
		{
			//AfxMessageBox(L"�޸�ʧ�ܣ��ָ�ԭ������!");
			ChangeDisplaySettings(NULL,0);
		}

		SendIpcData(IPC_MAIN_USER, IPC_SUB_SCREEN_SIZE_CHANGE,0,0);
	}

    SetStationParameter(0);
    CloseGameEngine(&m_pEngine);
    if(m_pIPC)
    {
        // �����������أ��Ƿ�Ҫ֪ͨ������һ���뿪��Ϣ
        m_pIPC->CloseChannel(bNotify,true);
        delete m_pIPC;
        m_pIPC = NULL;
    }
	//�������ò���
	AfxGetApp()->WriteProfileInt(m_GameInfo.szGameName,TEXT("CanLooking"),m_GameInfo.bEnableWatch?1:0);
	AfxGetApp()->WriteProfileInt(m_GameInfo.szGameName,TEXT("PlaySound"),m_GameInfo.bEnableSound?1:0);
	AfxGetApp()->WriteProfileInt(m_GameInfo.szGameName,TEXT("BackSound"),m_GameInfo.bEnableBackSound?1:0);
	AfxGetApp()->WriteProfileInt(m_GameInfo.szGameName,TEXT("ShowUserInfo"),m_GameInfo.bShowUserInfo?1:0);
	/*WriteProfileInt(TEXT("CanLooking"),m_GameInfo.bEnableWatch?1:0);
	WriteProfileInt(TEXT("PlaySound"),m_GameInfo.bEnableSound?1:0);
	WriteProfileInt(TEXT("BackSound"),m_GameInfo.bEnableBackSound?1:0);

	WriteProfileInt(TEXT("ShowUserInfo"),m_GameInfo.bShowUserInfo?1:0);*/

    exit(0);
}
/// ���ڴ�С�ı�
void XLogic::OnSize(RECT &rect)
{
    if (NULL==m_pIUI)
    {
        return;
    }
    UIInitBiaoQingKuang();

    ITalk *pTalk = NULL;
    GETCTRL(ITalk, pTalk, m_pIUI, TALK_SHOW);
    if (NULL!=pTalk)
    {
        //pTalk->SetGameW(rect.right-pTalk->GetGameX()-15);
        pTalk->SetGameH(rect.bottom-pTalk->GetGameY()-50);
    }
}

/// ʹ�õ���
void XLogic::OnPannelUseProp(WPARAM wParam, LPARAM lParam)
{
    UsePropItem(lParam);
}
/// �������
void XLogic::OnPannelBuyProp(WPARAM wParam, LPARAM lParam)
{
    // ֱ�Ӽ�����Ϣͷ�������Ӵ��ڷ�������Ϣ���͵�ƽ̨
    BYTE *buffer = (BYTE *)lParam;
    if (NULL != m_pIPC)
    {
        m_pIPC->SendData(IPC_MAIN_PROP, IPC_SUB_BUY_PROP, buffer, wParam);
        _TAG_PROP_BUY *pPropBuy = (_TAG_PROP_BUY *)lParam;
    }
    // �Ӵ��ڷ�������new������ָ�룬��ʱɾ��
    delete buffer;
    buffer = NULL;
}
/// ���͵���
void XLogic::OnPannelGiveProp(WPARAM wParam, LPARAM lParam)
{
    // ֱ�Ӽ�����Ϣͷ�������Ӵ��ڷ�������Ϣ���͵�ƽ̨
    BYTE *buffer = (BYTE *)lParam;
    if (NULL != m_pIPC)
    {
        m_pIPC->SendData(IPC_MAIN_PROP, IPC_SUB_PRESENT_PROP, buffer, wParam);
        _TAG_PROP_GIVE *pProp = (_TAG_PROP_GIVE *)lParam;
        ////DebugPrintf("[%s]��[%s][%d]��[%d]��ֵ[%d]",
        //	pProp->szUser, pProp->szTargetUser, pProp->nGiveCount, pProp->nPropID, pProp->iPropValue);
    }
    // �Ӵ��ڷ�������new������ָ�룬��ʱɾ��
    delete buffer;
    buffer = NULL;
}
/// ������ʹ�õ���
void XLogic::OnSelOtherUseProp(WPARAM wParam, LPARAM lParam)
{
    UserItemStruct *pUser = FindOnLineUser(lParam);
    if (NULL!=pUser)
    {
        int nRet = OnUseProp((int)wParam, pUser->GameUserInfo.dwUserID);
    }    
}

//״̬�ı�
void XLogic::SetStationParameter(BYTE bGameStation)
{

	BYTE Temp = m_GameInfo.bGameStation ;
	
	m_GameInfo.bGameStation = bGameStation;
    
    if (NULL != m_pIPC)
    {
        m_pIPC->SendData(IPC_MAIN_USER, IPC_SUB_GAME_START, &bGameStation, sizeof(BYTE));
    }

	if (bGameStation != Temp)
	{
		m_pGameImpl->SetStationParameter(bGameStation);
	}

    /// ��״̬ �� GameLauncher ����
    //m_pGameImpl->SetStationParameter(bGameStation);
}

////////////////////////////////////////////////////////////////////////////////
// ���
///��Ϸ�ӿ���л�ȡ����Ϣ
int XLogic::GetMyDeskStation()
{
    return GetMeUserInfo()->bDeskStation;
}
///����Լ������Ӻ�
int XLogic::GetMyDeskNum()
{
    return true;
}
///���ﴫ������ͼ����
int XLogic::GetUserStation(int nIndex)
{
    return ViewStation2DeskStation(nIndex);
}  
///����Ϣ���ݷ��͵���Ϸ������
int XLogic::SendGameData(WORD wSubId,void *pData, int nLen)
{
    if (m_GameInfo.uisMeUserInfo.GameUserInfo.bDeskNO==255) 
        return 0;

    UINT nBufferLen = nLen + sizeof(SendGameDataStructWithoutParam);
    BYTE *newbuffer = new BYTE[nBufferLen];
    SendGameDataStructWithoutParam *pHead = (SendGameDataStructWithoutParam *)newbuffer;
    pHead->bMainID		= MDM_GM_GAME_NOTIFY;
    pHead->bAssistantID = (BYTE)wSubId;
    pHead->bHandleCode	= 0;
    memcpy(newbuffer+sizeof(SendGameDataStructWithoutParam), (BYTE*)pData, nLen);


    bool bRet = false;
    if (NULL != m_pIPC)
    {
        bRet = m_pIPC->SendData(IPC_MAIN_SOCKET, IPC_SUB_SOCKET_SEND, newbuffer, nBufferLen);
    }
    delete []newbuffer;
    newbuffer = NULL;
    return bRet;
}
///ֻ����Ϣ���͵��ͻ��˴���
int XLogic::SendIpcData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	if (NULL != m_pIPC)
	{
		if (wDataSize == 0)
		{
			m_pIPC->SendData(wMainCmdID,wSubCmdID);
		}
		else
		{
			m_pIPC->SendData(wMainCmdID,wSubCmdID,pBuffer,wDataSize);
		}
	}
    return true;
}

///��ͼת��������λ ��Ϊ������Ϸ��Ҫ�õ��������ڿ���д���
int XLogic::ViewStation2DeskStation(int nViewIndex)
{
    if(GetMeUserInfo()==NULL || m_GameInfo.uDeskPeople==0)
    {
        return 0;
    }
    BYTE bMyDeskStation = GetMeUserInfo()->bDeskStation ;//m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
    BYTE bDeskStation =  ((nViewIndex -m_GameInfo.uDeskPeople/2)+bMyDeskStation+m_GameInfo.uDeskPeople)%m_GameInfo.uDeskPeople ;	// ����PLAY_COUNT��Ϊ�˷�ֹ���ָ���

    return bDeskStation;
}
///��λ��ת��ͼ��
int XLogic::DeskStation2View(int nDeskStation)
{
    if(GetMeUserInfo()==NULL || m_GameInfo.uDeskPeople==0 || m_GameInfo.uDeskPeople > PLAYNUM)
    {
        return 0;
    }
    BYTE bMyDeskStation = GetMeUserInfo()->bDeskStation ;//m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
    BYTE bViewStation=((nDeskStation-bMyDeskStation)+m_GameInfo.uDeskPeople/2+m_GameInfo.uDeskPeople)%m_GameInfo.uDeskPeople;

    return bViewStation;
}
///��ȡ����ǳ�
TCHAR * XLogic::GetUserNickName(int nDeskStation)
{
    INT_PTR uActiveCount=m_ThisDeskPtrArray.GetCount();
    for (int i=0;i<uActiveCount;i++)
    {
        UserItemStruct * pFindUserItem=(UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
        if ((pFindUserItem!=NULL)&&(pFindUserItem->GameUserInfo.bDeskStation==nDeskStation)) 
        {
            return pFindUserItem->GameUserInfo.szName;
        }
    }
    return NULL;
}
///�ر���Ϸ�ͻ���
void XLogic::CloseClient()
{
    QuitGame();
}
///�������
int XLogic::ClickContinue()
{
	CBcfFile f(CBcfFile::GetAppPath() + _T("GameOption.bcf"));
	UINT _bExit = f.GetKeyVal("QueueRoom", "ContinueExit",1);
	if (m_GameInfo.dwRoomRule & GRR_QUEUE_GAME && _bExit == 1 || m_bIsQueueDisMiss || m_GameInfo.dwRoomRule & GRR_CONTEST)
	{
		SendGameData(MDM_GM_GAME_NOTIFY, ASS_GM_AGREE_GAME, 1);
		QuitGame();
	}
	else
		SendGameData(MDM_GM_GAME_NOTIFY, ASS_GM_AGREE_GAME, 0);
    return true;
}
///ǿ�ƹر���Ϸ
void XLogic::CloseClientforce()
{
    QuitGame();
}
///����ʱ��
void XLogic::SetTimer(int nEventID, DWORD dwTime)
{
    m_pEngine->SetTimer(nEventID, dwTime);
}
///�ر�ʱ��
void XLogic::KillTimer(int nEventID)
{
    m_pEngine->KillTimer(nEventID);
}
///��ȡ��Ϸ����
int XLogic::GetVolume()
{
    return true;
} 
///��ȡ��Ϸ����
int XLogic::GetTalkKind()
{
    return true;
}
///��ȡ����·��
/// param _path[in, out] ������׼�����ڴ�ռ�
/// param nSize[in] _path���ڵ�ַӵ�еĿռ��С����λΪsizeof(TCHAR)
/// return ��_path���ڿռ丳ֵ�󣬷���_path
char* XLogic::GetAudioPath(int nDeskStation,char *_path, int nSize)
{
    return NULL;
}
///��ȡ��������
bool XLogic::GetAwardType(AwardTypeForGame &Type)
{
    return true;
}	
///���Ž�������
bool XLogic::PlayAwardAnimation(const AwardTypeForGame &type)
{
    return true;
}
///��ȡ����
bool XLogic::GetGameBaseInfo(GameInfoForGame &Info)
{
    Info.iPower = 0;

    return true;
}
///��ȡ�û���Ϣ
bool XLogic::GetUserInfo(int nDeskStation, UserInfoForGame &Info)
{
	INT_PTR uActiveCount=m_ThisDeskPtrArray.GetCount();
	UserItemStruct * pFindUserItem = NULL;
	UserItemStruct * pTmp = NULL;
	if (nDeskStation == m_GameInfo.uisMeUserInfo.GameUserInfo.bDeskStation)
	{
		pFindUserItem =  &m_GameInfo.uisMeUserInfo;
	}

	for (int i=0;i<uActiveCount;i++)
	{
		UserItemStruct * pTmp=(UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
		if ((pTmp!=NULL)&&(pTmp->GameUserInfo.bDeskStation==nDeskStation)) 
		{
			pFindUserItem = pTmp;
			break; 
		}
	}
	if (pFindUserItem!=NULL)
	{
		Info.i64Money = pFindUserItem->GameUserInfo.i64Money;
		_tcscpy(Info.szNickName, pFindUserItem->GameUserInfo.nickName);

		Info.bBoy = pFindUserItem->GameUserInfo.bBoy;
		Info.bDeskNO = pFindUserItem->GameUserInfo.bDeskNO;
		Info.bDeskStation = pFindUserItem->GameUserInfo.bDeskStation;            
		Info.bGameMaster = pFindUserItem->GameUserInfo.bGameMaster;
		Info.bLogoID = pFindUserItem->GameUserInfo.bLogoID;
		Info.dwUserID = pFindUserItem->GameUserInfo.dwUserID;
		Info.dwUserIP = pFindUserItem->GameUserInfo.dwUserIP;
		Info.bIsRobot = pFindUserItem->GameUserInfo.isVirtual;
		_tcscpy(Info.szCity, pFindUserItem->GameUserInfo.szCity);     

		if (m_GameInfo.dwRoomRule & GRR_CONTEST)
		{
			Info.i64Money = pFindUserItem->GameUserInfo.i64ContestScore;
		}
		return true;
	}
	return false;
}
/** @brief ���Ŵ�ͷ���ޱ���
*	@param[in]  nViewStation    ��ͼλ��
*	@param[in]  nIndex          ��������ţ�ĿǰΪ1~7������μ�����������.xls��
*	@return ������
*/
bool XLogic::PlayBigPortrait(int nViewStation, int nIndex)
{
    return true;
}
/** @brief ʹ��Сͷ��
 */
bool XLogic::SetUseSmallLogo(int nViewStation, bool bUseSmall)
{
    DebugPrintfA("%d ʹ��Сͷ�� bUseSmall %d", nViewStation, bUseSmall);

    m_IsUserSmall[nViewStation] = bUseSmall;
    UserItemStruct *puser = FindOnLineUser((BYTE)ViewStation2DeskStation(nViewStation));
    if (puser)
    {
        ShowOneUserInfo(nViewStation, puser, true);
    }
    return true;
}
/// ��������
bool XLogic::PlaySound(const char *szFileName)
{
    return true;
}
/// ĳ��λ�Ƿ�ʹ����Ƶ
bool XLogic::SetUseVideo(int nViewStation, bool bUse)
{
    return true;
}
/// ��ȡ�Ƿ�Ϊ�Թ�״̬
bool XLogic::GetIsWatching()
{
    return m_bWatchMode;
}
/// ��ȡ�Ƿ������Թ�
bool XLogic::GetIsEnableWatch()
{
    return m_bWatchOther;
}
/// ����Ϸ�����д����н���
void XLogic::OnVisitBank()
{
	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�����й���
	if (m_GameInfo.dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ�����й���", "��ܰ��ʾ");
		return ;
	}
	// end duanxiaohui

    SendGameData(MDM_GR_MONEY,ASS_GR_OPEN_WALLET_INGAME,0);
}
/// ��ȡȫ����Ϣ
CGlobal* XLogic::GetGlb()
{
    return &Glb();
}
/** @brief ��ȡ���ڴ�С
 */
RECT XLogic::GetGameWindowRect()
{
    RECT rt={0,0,0,0};
    m_pEngine->GetCWnd()->GetClientRect(&rt);
    return rt;
}




////////////////////////////////////////////////////////////////////////////////
// ��Ϣ����
bool XLogic::OnSystemMessage(NetMessageHead * pNetHead, void *pNetData, WORD wSendSize)
{
    if (pNetHead->bMainID!=MDM_GM_MESSAGE);
    {
        return false;
    }
    switch (pNetHead->bAssistantID)
    {
    case ASS_GM_SYSTEM_MESSAGE:		//ϵͳ��Ϣ
        {            
            //Ч������
            MSG_GA_S_Message * pMessage=(MSG_GA_S_Message *)pNetData;
            AFCPlaySound(AfxGetInstanceHandle(),TEXT("SYSTEM_MSG"));

            //��������
            if ((pMessage->bShowStation&SHS_TALK_MESSAGE)!=0)
            {
                //m_MessageHandle.InsertSystemMessage(pMessage->szMessage,pMessage->bFontSize);
                InsertSystemMessage(pMessage->szMessage);
            }
            if ((pMessage->bShowStation&SHS_MESSAGE_BOX)!=0)
            {
                //m_bSaveView=true;
                SetStationParameter(20);
                //AFCMessageBox("�����������",m_pGameInfo.szGameName);//,MB_ICONQUESTION,this);
                ////AFCMessageBox(pMessage->szMessage,m_GameInfo.szGameName);//,MB_ICONQUESTION,this);
                //m_bSaveView=false;
                if (pMessage->bCloseFace==TRUE)
                {
                    DebugPrintfA("ASS_GM_SYSTEM_MESSAGE ϵͳ��Ϣ���˳���Ϸ");

                    QuitGame();
                }
            }            
            return true;
        }
    }
    return false;
}
bool XLogic::OnFrameMessage(NetMessageHead * pNetHead, void *pNetData, WORD wSendSize)
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
            m_bWatchMode=(m_GameInfo.uisMeUserInfo.GameUserInfo.bUserState==USER_WATCH_GAME);
            SetStationParameter(pGameInfo->bGameStation);
            //��ʾ��Ϣ
            if (wSendSize>(sizeof(MSG_GM_S_GameInfo)-sizeof(pGameInfo->szMessage)))
            {
                InsertSystemMessage(pGameInfo->szMessage);
            }    

            return true;
        }
    case ASS_GM_GAME_STATION:	//��Ϸ״̬
        {	
// 			CString s;
// 			s.Format("DClient-: ASS_GM_GAME_STATION %d %I64d",m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID, m_GameInfo.uisMeUserInfo.GameUserInfo.i64Money);
// 			OutputDebugString(s);

            int nRet = m_pGameImpl->HandleGameMessage(MDM_GM_GAME_NOTIFY,2,(BYTE*)pNetData, wSendSize);            
            return 0==nRet;
            //return SetGameStation(pNetData,uDataSize);//����DLL�еĺ��������齫��uDataSizeΪ51
        }
    case ASS_GM_NORMAL_TALK:	//��ͨ����
        {
            //Ч������
            MSG_GR_RS_NormalTalk * pNormalTalk=(MSG_GR_RS_NormalTalk *)pNetData;

            //��������
            UserItemStruct * pSendUserItem=FindOnLineUser(pNormalTalk->dwSendID);
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
                UserItemStruct * pTargerUserItem=FindOnLineUser(pNormalTalk->dwTargetID);
                if (pTargerUserItem!=NULL) 
                {
                    //m_MessageHandle.InsertUserTalk(GetComType()!=TY_MATCH_GAME?pSendUserItem->GameUserInfo.nickName:"���",GetComType()!=TY_MATCH_GAME?pTargerUserItem->GameUserInfo.nickName:"���",
                    //    pNormalTalk->szMessage,pNormalTalk->crColor,false);
                    //lstrcpy(pNormalTalk->szMessage,msg);
                    //ShowUserTalk(pNormalTalk);

					char szGBuffer[512] = {0};
					char szTempBuffer[256] = {0};

					::strcat(szGBuffer,"<p 4>");

					TCHAR* _pUser = (m_GameInfo.dwRoomRule & GRR_NOTCHEAT) ? TEXT("���") : pSendUserItem->GameUserInfo.nickName;
					::strcat(szGBuffer,_pUser);

					::strcat(szGBuffer,"˵��");

					szTempBuffer[0] = '\0';
					_stprintf(szTempBuffer,"<c %08x>%s</c>",0x000000FF,pNormalTalk->szMessage);
					::strcat(szGBuffer,szTempBuffer);

					::strcat(szGBuffer,"</p>");

					if (m_pRightFrame)
					{
						m_pRightFrame->InsertTalkMessage(szGBuffer);
					}


//                     TalkMessage_t msg;
//                     localtime(&msg.sTime);
//                     TCHAR* _pUser = TY_MATCH_GAME?pSendUserItem->GameUserInfo.nickName:TEXT("���");
//                     _tcscpy(msg.szSourceNickName, _pUser);
//                     msg.iSourceID = pNormalTalk->dwSendID;
//                     msg.iTargetID = pNormalTalk->dwTargetID;
//                     _tcscpy(msg.szMsgString, CA2T(pNormalTalk->szMessage));
//                     InsertTalk(msg);
   //                 RefreshTalk();
                    //char2Tchar(_P->szMessage, msg.szMsgString);

                    return true;
                }
            }
            //m_MessageHandle.InsertUserTalk(GetComType()!=TY_MATCH_GAME?pSendUserItem->GameUserInfo.nickName:"���",NULL,pNormalTalk->szMessage,pNormalTalk->crColor,false);
            //lstrcpy(pNormalTalk->szMessage,msg);
            //if(GetComType()!=TY_MATCH_GAME)
            //    ShowUserTalk(pNormalTalk);
            //if(pNormalTalk->nDefaultIndex>0)
            //{
            //    CString soundFile;
            //    soundFile.Format("..\\Music\\Talk\\%d.wav",pNormalTalk->nDefaultIndex);
            //    BZSoundPlay(this,soundFile.GetBuffer(),0,1);
            //}            


//             TalkMessage_t msgt;            
//             localtime(&msgt.sTime);
//             TCHAR* _pUser = TY_MATCH_GAME?pSendUserItem->GameUserInfo.nickName:TEXT("���");
//             _tcscpy(msgt.szSourceNickName, _pUser);
//             msgt.iSourceID = pNormalTalk->dwSendID;
//             msgt.iTargetID = pNormalTalk->dwTargetID;
//             _tcscpy(msgt.szMsgString, CA2T(pNormalTalk->szMessage));
//             InsertTalk(msgt);
//             RefreshTalk();

			//������������
			CBcfFile fMsg(CBcfFile::GetAppPath() + "TalkInGame.bcf");
			CString str = fMsg.GetKeyVal("BZW_TALK","path","..\\Music\\Talk");

			//������������
			TCHAR szSpeakPath[MAX_PATH];
			if (pSendUserItem->GameUserInfo.bBoy)
			{
				_stprintf(szSpeakPath,"%s\\male\\%s.wav",str,pNormalTalk->szMessage);	
			}
			else
			{
				_stprintf(szSpeakPath,"%s\\female\\%s.wav",str,pNormalTalk->szMessage);
			}
			BZSoundPlay(m_pEngine->GetCWnd(),szSpeakPath,0);
			

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
                    InsertSystemMessage(TEXT("�㱻�����Թ�"));
                else InsertSystemMessage(TEXT("�㱻��ֹ�Թ�"));
                //OnWatchSetChange();
            }
            return true;
        }
    case ASS_GM_USE_KICK_PROP:      // PengJiLin, 2010-9-10, ʹ�����˿�
        {
            return OnUseKickPropResult(pNetHead, pNetData, wSendSize, TRUE);
        }
    case ASS_GM_USE_ANTI_KICK_PROP: // PengJiLin, 2010-9-10, ʹ�÷��߿�
        {
            return OnUseKickPropResult(pNetHead, pNetData, wSendSize, FALSE);;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////
// �û���Ϣ
void XLogic::GetOnLineUser( BYTE bMeDeskStation )
{
    CWnd * winApp=AfxGetApp()->m_pMainWnd;
    if(winApp && winApp->m_hWnd)
        winApp->PostMessage(WM_REQUEST_PLAYERLIST, (WPARAM)bMeDeskStation, 0);
}
UserInfoStruct * XLogic::GetMeUserInfo()
{
    return &m_GameInfo.uisMeUserInfo.GameUserInfo;
}
// �����û�ID�ұ���������Ƿ���
UserItemStruct *XLogic::FindOnLineUser(long int dwUserID)
{
    INT_PTR uActiveCount=m_ThisDeskPtrArray.GetCount();
    for (int i=0;i<uActiveCount;i++)
    {
        UserItemStruct * pFindUserItem=(UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
        if ((pFindUserItem!=NULL)&&(pFindUserItem->GameUserInfo.dwUserID==dwUserID)) 
            return pFindUserItem;
    }
    return NULL;
}

INT_PTR XLogic::GetOnlineUserCount()
{
	return m_ThisDeskPtrArray.GetCount();
}

UserItemStruct *XLogic::FindOnLineUserByIdx(long int dwIdx)
{
	INT_PTR uActiveCount=m_ThisDeskPtrArray.GetCount();
	if (dwIdx<uActiveCount)
	{
		return (UserItemStruct *)m_ThisDeskPtrArray.GetAt(dwIdx);
	}
	return NULL;
}

UserItemStruct *XLogic::FindWatchUser(long int dwUserID)
{
    INT_PTR uActiveCount=m_WatchPtrArray.GetCount();
    for (int i=0;i<uActiveCount;i++)
    {
        UserItemStruct * pFindUserItem=(UserItemStruct *)m_WatchPtrArray.GetAt(i);
        if ((pFindUserItem!=NULL)&&(pFindUserItem->GameUserInfo.dwUserID==dwUserID)) 
            return pFindUserItem;
    }
    return NULL;
}

UserItemStruct *XLogic::FindOnLineUser(BYTE bDeskStation)
{
    INT_PTR uActiveCount=m_ThisDeskPtrArray.GetCount();
    for (int i=0;i<uActiveCount;i++)
    {
        UserItemStruct * pFindUserItem=(UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
        if ((pFindUserItem!=NULL)&&(pFindUserItem->GameUserInfo.bDeskStation==bDeskStation)) 
            return pFindUserItem;
    }
    return NULL;
}

// �û��뿪
bool XLogic::GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
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
                if (NULL == pFindUserItem)
                {
                    continue;
                }
                if (pUserItem->GameUserInfo.dwUserID==pFindUserItem->GameUserInfo.dwUserID)
                {
                    delete pFindUserItem;
                    pFindUserItem = NULL;
                    m_WatchPtrArray.RemoveAt(i);
                    break;
                }
            }
        }
        else
        {
            
        }
        // �ӱ�������б����������
        UserItemStruct * pFindUserItem=NULL;
        INT_PTR uCount=m_ThisDeskPtrArray.GetCount();
        for (INT_PTR i=0;i<uCount;++i)
        {
            pFindUserItem=(UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
            if (pUserItem->GameUserInfo.dwUserID
                ==pFindUserItem->GameUserInfo.dwUserID)
            {
				if (m_pUserListDu)
				{
					m_pUserListDu->DeleteGameUser(pFindUserItem);
				}
                delete pFindUserItem;
                pFindUserItem = NULL;
                m_ThisDeskPtrArray.RemoveAt(i);
                if (bWatchUser==false)
                {
                    
                }
                break;
            }
        }
        RefreshPlayList();

        return true;
    }
    return false;
}

void XLogic::OnUserListAtDesk(UserItemStruct *pUserItemVoid)
{
    UserItemStruct *pUserItemToAdd = (UserItemStruct *)pUserItemVoid;

	bool bIsMySelf = false;

    if (pUserItemToAdd->GameUserInfo.dwUserID == m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)
    {
        memcpy(&m_GameInfo.uisMeUserInfo, pUserItemToAdd, sizeof(UserItemStruct));
		bIsMySelf = true;
    }

    if (pUserItemToAdd->GameUserInfo.bUserState!=USER_WATCH_GAME)
    {
        //���������û�
        bool bNewItem=false;        
        UserItemStruct * pUserItem=FindOnLineUser(pUserItemToAdd->GameUserInfo.dwUserID);
        if (pUserItem==NULL)
        {
            //�����ڴ�
            bNewItem = true;
            pUserItem =new UserItemStruct;
            ASSERT(pUserItem!=NULL);
        }
        // ����Ѿ����˵ģ�ҲҪ�������ݣ���Ϊ�п��������ͬһ���ڻ�����ͬλ��
        memcpy( pUserItem, pUserItemToAdd, sizeof(UserItemStruct) );
        //�������
        if (bNewItem==true) m_ThisDeskPtrArray.Add(pUserItem);

		if (m_pUserListDu)
		{
			m_pUserListDu->AddGameUser(pUserItem,bIsMySelf);
		}

		if (m_pRightFrame)
		{
			if (bIsMySelf)
			{
				m_pRightFrame->ShowUserInfo(pUserItem);
			}
		}
    }

    m_pGameImpl->GameUserCome();
    if (pUserItemToAdd->GameUserInfo.bUserState!=USER_WATCH_GAME)
    {
        ShowOneUserInfo(DeskStation2View(pUserItemVoid->GameUserInfo.bDeskStation), pUserItemVoid, true);
    }    
    else
    {
        //���������û�
        bool bNewItem=false;        
        UserItemStruct * pUserItem=FindWatchUser(pUserItemToAdd->GameUserInfo.dwUserID);
        if (pUserItem==NULL)
        {
            //�����ڴ�
            bNewItem = true;
            pUserItem =new UserItemStruct;
            ASSERT(pUserItem!=NULL);
        }
        // ����Ѿ����˵ģ�ҲҪ�������ݣ���Ϊ�п��������ͬһ���ڻ�����ͬλ��
        memcpy( pUserItem, pUserItemToAdd, sizeof(UserItemStruct) );
        //�������
        if (bNewItem==true) m_WatchPtrArray.Add(pUserItem);//����Ӧ�ü����Թ۶���

		if (m_pUserListDu)
		{
			m_pUserListDu->AddGameUser(pUserItem,bIsMySelf);
		}

		if (m_pRightFrame)
		{
			if (bIsMySelf)
			{
				m_pRightFrame->ShowUserInfo(pUserItem);
			}
		}


		m_pUserListDu->SetWatchUser(pUserItem, pUserItem->GameUserInfo.nickName);

    }
    RefreshPlayList();

	

}



////////////////////////////////////////////////////////////////////////////////
// ���
//������Ϣ 
UINT XLogic::OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize)
{
    //��������
    switch (uControlCode)
    {
    case CM_USER_SEND_TIMES_MONEY:
        {
            if (uDataSize!=sizeof(CM_UserState_Send_Times_Money_For_Exe)) return 1;
            CM_UserState_Send_Times_Money_For_Exe *pMoneyChange = (CM_UserState_Send_Times_Money_For_Exe*)pControlData;

            if (pMoneyChange->dwUserID != m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)
                return 0;

            TCHAR str[100]={0};
            //TCHAR szNum[32]; GlbGetNumString(szNum, pMoneyChange->iSendMoney, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
            //if(pMoneyChange->iLastTimes<=0)
            //{
            //    wsprintf(str,"/:54����%s��ң��ѻ���%d�Σ����޻�������/:54",szNum,
            //        pMoneyChange->iSendTimes,pMoneyChange->iLastTimes);
            //}
            //else
            //{
            //    wsprintf(str,"/:54����%s��ң��ѻ���%d�Σ�����%d�λ�������/:54",szNum,
            //        pMoneyChange->iSendTimes,pMoneyChange->iLastTimes);
            //}

            InsertNormalMessage(str);
            return 0;

        }
        break;
    case CM_USER_STATE:		//�û�״̬�ı�
        {
            //Ч������
            if (uDataSize!=sizeof(CM_UserState_Change_ForExe))
            {
                DebugPrintfA("���ݴ�С����uDataSize =[%d], Ӧ��Ϊ[%d]",sizeof(CM_UserState_Change_ForExe));
                return 1;
            }
            CM_UserState_Change_ForExe * pStateChange=(CM_UserState_Change_ForExe *)pControlData;

			if (pStateChange->bActionCode == ACT_USER_BANKMONEY)
			{
				INT_PTR nUserCountInThisDesk = m_ThisDeskPtrArray.GetCount();
				for (INT_PTR i=0; i<nUserCountInThisDesk; ++i)
				{
					__int64 i64BankMoney = pStateChange->uisUserItem.GameUserInfo.i64Bank;
					UserItemStruct *pFindItem = (UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
					if (pFindItem->GameUserInfo.dwUserID == pStateChange->uisUserItem.GameUserInfo.dwUserID)
					{
						pFindItem->GameUserInfo.i64Bank = i64BankMoney;
						if (m_pBank2Wnd)
						{
							m_pBank2Wnd->SetWealthInfor(i64BankMoney,GetMeUserInfo()->i64Money);
						}
						break;
					}
				}
				return 1;
			}


            INT_PTR nUserCountInThisDesk = m_ThisDeskPtrArray.GetCount();
            for (INT_PTR i=0; i<nUserCountInThisDesk; ++i)
            {
                UserItemStruct *pFindItem = (UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
                if (pFindItem->GameUserInfo.dwUserID == pStateChange->uisUserItem.GameUserInfo.dwUserID)
                {
                    memcpy(pFindItem, &pStateChange->uisUserItem, sizeof(UserItemStruct));
                }
            }            
            //��������
            switch (pStateChange->bActionCode)
            {
            case ACT_USER_UP:		//�û�����
            case ACT_WATCH_UP:		//�Թ�����
                {
                    //��������
                    if (pStateChange->uisUserItem.GameUserInfo.dwUserID
                        != m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)//ͬ�����������뿪
                    {
                        GameUserLeft(pStateChange->bDeskStation, &pStateChange->uisUserItem, pStateChange->bActionCode==ACT_WATCH_UP);

						if (pStateChange->bActionCode != ACT_WATCH_UP)
							m_pGameImpl->GameUserLeft(pStateChange->bDeskStation);

						CString s;
						s.Format("xxxxxx uComType=%d, bShowUserInfo=%d", m_GameInfo.uComType, m_GameInfo.bShowUserInfo);
						OutputDebugString(s);


                        if ((m_GameInfo.uComType!=TY_MATCH_GAME)
                            &&m_GameInfo.bShowUserInfo==true
                            /*&&0 == pStateChange->uisUserItem.GameUserInfo.isVirtual*/)//.������ҲӦ����ʾzht2011-10-25
                        {	

                                char szTalk[255];
                                sprintf(szTalk, "%s �뿪��", pStateChange->uisUserItem.GameUserInfo.nickName);
                                InsertNormalMessage(szTalk);

                        }

						if (m_GameInfo.uComType != TY_MATCH_GAME)
						{
							if (pStateChange->bActionCode != ACT_WATCH_UP)
							{
								ShowOneUserInfo(DeskStation2View(pStateChange->bDeskStation), &pStateChange->uisUserItem, false);
							}
						}
						
                    }
                    else//�Լ��뿪
                    {
                        DebugPrintfA("�Լ��뿪��");
                    }
                    return 0;
                }
            case ACT_WATCH_SIT:		//�û�����
            case ACT_USER_SIT:		//�Թ�����
                {
                    //��������
                    // ���Ǳ������£���ֱ��֪ͨ
                    if (pStateChange->uisUserItem.GameUserInfo.dwUserID
                        != m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)//ͬ����������
                    {
                        OnUserListAtDesk(&pStateChange->uisUserItem);
                        if (m_GameInfo.bShowUserInfo==true&&(m_GameInfo.uComType!=TY_MATCH_GAME))
                        {
                            char szTalk[255];
                            sprintf(szTalk, "%s ������", pStateChange->uisUserItem.GameUserInfo.nickName);
                            InsertNormalMessage(szTalk);                            
                            //m_MessageHandle.InsertUserName(pStateChange->uisUserItem.GameUserInfo.nickName);
                            //if (pStateChange->bActionCode==ACT_WATCH_SIT)
                            //    m_MessageHandle.InsertNormalMessage(TEXT("������"));
                            //else 
                            //    m_MessageHandle.InsertNormalMessage(TEXT("������"));
                        }
                    }
                    else	// �����Լ�����ĳ�����ӣ�����Ҫ�ҵ�ͬ��������ҵ���Ϣ����������Ϣ�����ڻ���
                    {
                        //Ч��״̬
                        BYTE bMeDeskStation=m_GameInfo.uisMeUserInfo.GameUserInfo.bDeskNO;
                        //���ͻ�ȡ��Ϣ
                        MSG_GM_S_ClientInfo ClientInfo;
                        ClientInfo.bEnableWatch=m_GameInfo.bEnableWatch;
                        SendGameData(&ClientInfo,sizeof(ClientInfo),MDM_GM_GAME_FRAME,ASS_GM_GAME_INFO,0);
                        //UpdateGameTitle();
                    }

                    return 0;
                }
            case ACT_USER_CUT:		//��Ҷ���orǿ����
                {

                    //��������
                    //m_UserListDlg.UpdateGameUser(&pStateChange->uisUserItem);
                    //m_pGameView->UpdateUserInfo(pStateChange->bDeskStation);
                    //�����˶��߲�����ʾ��Ϣ201103-16
					if(0 == pStateChange->uisUserItem.GameUserInfo.isVirtual)
					{
						if (m_GameInfo.bShowUserInfo==true)
						{
							//��ʾ��Ϣ
							char szTalk[255];

							sprintf(szTalk, "%s ������", pStateChange->uisUserItem.GameUserInfo.nickName);
							InsertNormalMessage(szTalk);
						}

						UserItemStruct* pFindUserItem = FindOnLineUser(pStateChange->uisUserItem.GameUserInfo.dwUserID);

						if (m_pUserListDu)
						{
							m_pUserListDu->UpdateGameUser(pFindUserItem);
						}

						if (m_pRightFrame)
						{
							m_pRightFrame->ShowUserInfo();
						}
                        //if(m_GameInfo.uComType!=TY_MATCH_GAME)
                        //    m_MessageHandle.InsertUserName(pStateChange->uisUserItem.GameUserInfo.nickName);
                        //m_MessageHandle.InsertNormalMessage(TEXT("������"));
                    }
                    return 0;
                }
            case ACT_USER_AGREE:	//�û�ͬ��
                {
                    //UserAgreeGame(pStateChange->bDeskStation);

                    MSG_GR_R_UserAgree agree;
                    agree.bDeskStation = pStateChange->bDeskStation;
                    agree.bAgreeGame = true;
                    m_pGameImpl->HandleGameMessage(MDM_GM_GAME_NOTIFY, ASS_GM_AGREE_GAME, (BYTE*)&agree, sizeof(agree));

                    /////�û�ͬ��ṹ
                    //struct MSG_GR_R_UserAgree
                    //{
                    //    BYTE								bDeskNO;							///��Ϸ����
                    //    BYTE								bDeskStation;						///λ�ú���
                    //    BYTE								bAgreeGame;							///ͬ���־
                    //};

					UserItemStruct* pFindUserItem = FindOnLineUser(pStateChange->uisUserItem.GameUserInfo.dwUserID);

					if (m_pUserListDu)
					{
						m_pUserListDu->UpdateGameUser(pFindUserItem);
					}

					if (m_pRightFrame)
					{
						m_pRightFrame->ShowUserInfo();
					}

                    return 0;
                }
            case ACT_GAME_BEGIN:	//��Ϸ��ʼ
            case ACT_GAME_END:		//��Ϸ����
                {

                    INT_PTR uActiveCount=m_ThisDeskPtrArray.GetCount();
                    for (int i=0;i<uActiveCount;i++)
                    {
                        UserItemStruct * pFindUserItem=(UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);

						if (pStateChange->bActionCode == ACT_GAME_BEGIN)
						{
							pFindUserItem->GameUserInfo.bUserState = USER_PLAY_GAME;
						}
						else if (pStateChange->bActionCode == ACT_GAME_END)
						{
							pFindUserItem->GameUserInfo.bUserState = USER_SITTING;
						}
						

                        //������Ϣ
                        ShowOneUserInfo(DeskStation2View(pFindUserItem->GameUserInfo.bDeskStation), pFindUserItem, true);

						if (m_pUserListDu)
						{
							m_pUserListDu->UpdateGameUser(pFindUserItem);
						}
                        
                    }

					//������λ���û�
					RefreshPlayList();
                    //for (BYTE i=0;i<m_GameInfo.uDeskPeople;i++)
                    //{
                    //    if (m_pUserInfo[i]!=NULL) 
                    //    {
                    //        m_pUserInfo[i]->GameUserInfo.bUserState = USER_PLAY_GAME;
                    //        m_UserListDlg.UpdateGameUser(m_pUserInfo[i]);
                    //    }
                    //}
                    //m_pGameView->UpdateUserInfo(ViewStation(m_GameInfo.uDeskPeople));
                    //if (m_pGameView->m_bShowRight)	///< ��ֹ�Ŷӻ��ͷ����׷�����˸
                    //{
                    //    ::SendMessage(m_TalkMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
                    //}
                    return 0;
                }

            case ACT_USER_POINT:	//�û�����ֵ
                {
                    UserItemStruct * pUserItem=FindOnLineUser(pStateChange->uisUserItem.GameUserInfo.dwUserID);
                    if (pUserItem == NULL)
                    {
                        return 0;
                    }
                    memcpy( pUserItem, &pStateChange->uisUserItem, sizeof(UserItemStruct) );
                    
                    //������Ϣ
                    ShowOneUserInfo(DeskStation2View(pUserItem->GameUserInfo.bDeskStation), pUserItem, true);
                    //������λ���û�
                    RefreshPlayList();

					if (m_pUserListDu)
					{
						m_pUserListDu->UpdateGameUser(pUserItem);
					}

                    if (GetMeUserInfo()->dwUserID == pStateChange->uisUserItem.GameUserInfo.dwUserID)// add by ramon 09.05.31 �޸�������Ϣ����ˢ��
                    {
                        memcpy(&m_GameInfo.uisMeUserInfo, &pStateChange->uisUserItem, sizeof(UserItemStruct));
                    }
					//����Ѵ���Ϸ���У��������е���ֵ
					//Add by JianGuankun 2012.1.2
					if (GetMeUserInfo()->dwUserID == pStateChange->uisUserItem.GameUserInfo.dwUserID) //�Լ���������
					{
						if (m_pBank2Wnd)
						{
							m_pBank2Wnd->SetWealthInfor(GetMeUserInfo()->i64Bank,GetMeUserInfo()->i64Money);
						}
					}
					//End Add

					if (m_pRightFrame)
					{
						m_pRightFrame->ShowUserInfo();
					}

                    //if(pUserItem->GameUserInfo.dwUserID==m_CurpUserInfoClicked->dwUserID)
                    //{
                    //    m_CurpUserInfoClicked = &pUserItem->GameUserInfo;
                    //    NavigateVirFace();
                    //}

                    //if (pUserItem->GameUserInfo.bUserState!=USER_WATCH_GAME)
                    //{
                    //    m_pGameView->UpdateUserInfo(pStateChange->bDeskStation);
                    //}
                    ////���˻��ָ����ж��Ƿ���Ҫ�뿪
                    //if(GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID)//�Ƿ���Ҫ�뿪��Ϸ��
                    //{
                    //    if(GetMeUserInfo()->bUserState!= USER_PLAY_GAME && GetMeUserInfo()->dwMoney < m_GameInfo.uLessPoint)
                    //    {
                    //        OnBnClickedQuitGame();
                    //    }	
                    //}
                    //if (m_pGameView->m_bShowRight)
                    //{
                    //    InvalidateRect(&m_UserInfoRect);
                    //}
                    return 0;
                }
			case ACT_USER_QUEUE_DISMISS:
				{
					CString debug;
					debug.Format("xxxxxx �û�״̬�ı�ΪACT_USER_QUEUE_DISMISSACT_USER_QUEUE_DISMISSACT_USER_QUEUE_DISMISS");
					OutputDebugString(debug);
					m_bIsQueueDisMiss = true;
					return 0;
				}
            case ACT_USER_CHARM://��������ֵ
                {
                    UserItemStruct * pUserItem=FindOnLineUser(pStateChange->uisUserItem.GameUserInfo.dwUserID);
                    if (pUserItem == NULL)
                    {
                        return 0;
                    }
                    memcpy( pUserItem, &pStateChange->uisUserItem, sizeof(UserItemStruct) );
                    
                    //������Ϣ
                    ShowOneUserInfo(DeskStation2View(pUserItem->GameUserInfo.bDeskStation), pUserItem, true);
                    //������λ���û�
                    RefreshPlayList();

					if (m_pUserListDu)
					{
						m_pUserListDu->UpdateGameUser(pUserItem);
					}

                    if (GetMeUserInfo()->dwUserID == pStateChange->uisUserItem.GameUserInfo.dwUserID)// add by ramon 09.05.31 �޸�������Ϣ����ˢ��
                    {
                        memcpy(&m_GameInfo.uisMeUserInfo, &pStateChange->uisUserItem, sizeof(UserItemStruct));
                    }

					if (m_pRightFrame)
					{
						m_pRightFrame->ShowUserInfo();
					}
                    //if(pUserItem->GameUserInfo.dwUserID==m_CurpUserInfoClicked->dwUserID)
                    //{
                    //    NavigateVirFace();
                    //}
                    //if (pUserItem->GameUserInfo.bUserState!=USER_WATCH_GAME)
                    //{
                    //    m_pGameView->UpdateUserInfo(pStateChange->bDeskStation);
                    //}
                    return 0;
                }
            }
        }
    }

    return 0;
}



////////////////////////////////////////////////////////////////////////////////
// ����
int XLogic::InsertSystemMessage(TCHAR* szMessage)
{

	char szGBuffer[512] = {0};
	char szTempBuffer[256] = {0};

	::strcat(szGBuffer,"<p 6>");

	szTempBuffer[0] = '\0';
	_stprintf(szTempBuffer,"<i dialog\\xtxx.png 0> <c %08x>%s</c>",0x00FF0000,szMessage);
	::strcat(szGBuffer,szTempBuffer);

	::strcat(szGBuffer,"</p>");

	if (m_pRightFrame)
	{
		m_pRightFrame->InsertTalkMessage(szGBuffer);
	}

//     TalkMessage_t _Msg;
//     _Msg.eChatType = CT_SYSTEM;
//     _tcscpy(_Msg.szMsgString, szMessage);
//     InsertTalk(_Msg);
//     RefreshTalk();

    return 0;
}


int XLogic::InsertNormalMessage(TCHAR* szMessage)
{

	char szGBuffer[512] = {0};
	char szTempBuffer[256] = {0};

	::strcat(szGBuffer,"<p 4>");

	szTempBuffer[0] = '\0';
	_stprintf(szTempBuffer,"<c %08x>%s</c>",0x00000000,szMessage);
	::strcat(szGBuffer,szTempBuffer);

	::strcat(szGBuffer,"</p>");

	if (m_pRightFrame)
	{
		m_pRightFrame->InsertTalkMessage(szGBuffer);
	}

//     TalkMessage_t _Msg;
//     _Msg.eChatType = CT_NORMAL;
//     _tcscpy(_Msg.szMsgString, szMessage);
//     InsertTalk(_Msg);
//     RefreshTalk();

    return 0;    
}
int XLogic::InsertDuduMsg(TCHAR * szSendName, TCHAR * szTargetName,TCHAR * szCharString, 
                          COLORREF crTextColor, bool bShowTime, UINT uSize, 
                          TCHAR * szFontName,int iDuduType)
{
	char szGBuffer[512] = {0};
	char szTempBuffer[256] = {0};

	::strcat(szGBuffer,"<p 4>");

	//����ʱ��
	if (bShowTime)
	{
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

		szTempBuffer[0] = '\0';
		_stprintf(szTempBuffer,"<c #%06f>(%02d:%02d:%02d)\n</c>",0x00000000,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
		::strcat(szGBuffer,szTempBuffer);
	}

	::strcat(szGBuffer,szSendName);

	if (szTargetName!=NULL)
	{
		if(iDuduType==BRD_MSG_BIG)
		{
			_stprintf(szTempBuffer,"<c %08x>���Ŵ����Ⱥ�</c>",0x000000FF);
			::strcat(szGBuffer,szTempBuffer);
		}			
		else
		{
			_stprintf(szTempBuffer,"<c %08x>����С���Ⱥ�</c>",0x000000FF);
			::strcat(szGBuffer,szTempBuffer);
		}

		::strcat(szGBuffer,szTargetName);
	}

	//������Ϣ

	szTempBuffer[0] = '\0';
	_stprintf(szTempBuffer,"<c %08x>%s</c>",0x00FF0000,szCharString);
	::strcat(szGBuffer,szTempBuffer);

	::strcat(szGBuffer,"</p>");

	if (m_pRightFrame)
	{
		m_pRightFrame->InsertTalkMessage(szGBuffer);
	}



//     TalkMessage_t msg;
//     if(iDuduType==BRD_MSG_BIG)
//         msg.eChatType = CT_DALABA;
//     else
//         msg.eChatType = CT_XIAOLABA;
// 	msg.crTextColor = crTextColor;
//     _tcscpy(msg.szMsgString, szCharString);
//     _tcscpy(msg.szSourceNickName, szSendName);
//     InsertTalk(msg);

    return 0;
}

int XLogic::InsertTalk(const TalkMessage_t& sMsg)
{
	return 0;
	if (!m_bIsAcceptChatMsg)	//������������Ϣ
	{
		return 0;
	}

    m_TalkMessages.push_back(sMsg);
    int iCount(0);
    for (list<TalkMessage_t>::iterator it = m_TalkMessages.begin(); it != m_TalkMessages.end(); it++)
    {
        if (sMsg.eChatType == it->eChatType) iCount++; 
    }
    if (iCount > 50)
    {
        for (list<TalkMessage_t>::iterator it = m_TalkMessages.begin(); it != m_TalkMessages.end(); it++)
        {
            if (sMsg.eChatType == it->eChatType) 
            {
                m_TalkMessages.erase(it);
                break;
            }
        }
    }
    RefreshTalk();
    return 0;
}

int XLogic::RefreshTalk()
{
    ITalk* pTalk;
    GETCTRL(ITalk, pTalk, m_pIUI, TALK_SHOW);
    if (pTalk != NULL)
    {
        pTalk->ClearText();
        TCHAR* pTalkString;
        for (list<TalkMessage_t>::iterator it = m_TalkMessages.begin(); it != m_TalkMessages.end(); it++)
        {
            pTalkString = FormatTalk(*it);
            pTalk->SetText(CT2W(pTalkString));
            delete pTalkString;
        }
    }
    return 0;
}

TCHAR* XLogic::FormatTalk(const TalkMessage_t& sMsg)
{
    TCHAR* pResult = new TCHAR[600];
    memset(pResult, 0, 600 * sizeof(TCHAR));

    CHATTYPE eChatTarget = sMsg.eChatType;
    if (eChatTarget == CT_PRIVATE)
    {
		static CString clr1;//zht2011-11-9 �޸�ϵͳ��Ϣ��������ɫҲ����
		static bool bGetColor1 = false;
		if (!bGetColor1)
		{        
			TCHAR szPath[MAX_PATH];
			wsprintf(szPath,"GameOption.bcf");
			CBcfFile f(szPath);
			clr1=f.GetKeyVal("Chat","PrivateColor","0xffffff00");
			bGetColor1 = true;
		}

		//wsprintf(pResult, TEXT("<font><color=%s,size=14,style=����>%s</font>"), clr.GetBuffer(), sMsg.szMsgString);
        wsprintf(pResult, TEXT("<font><color=%s,size=12,style=����>%s��</font><font><color=%s,size=12,style=����>%s</font>"), clr1.GetBuffer(),sMsg.szSourceNickName, clr1.GetBuffer(),sMsg.szMsgString);
    }
    else if (eChatTarget == CT_SYSTEM)
    {
		static CString clr2;
		static bool bGetColor2 = false;
		if (!bGetColor2)
		{        
			TCHAR szPath[MAX_PATH];
			wsprintf(szPath,"GameOption.bcf");
			CBcfFile f(szPath);
			clr2=f.GetKeyVal("Chat","SystemColor","0xffffff00");
			bGetColor2 = true;
		}
		

        wsprintf(pResult, TEXT("<font><color=0xffff0000,size=12,style=����>ϵͳ��Ϣ��</font><font><color=%s,size=12,style=����>%s</font>"),clr2.GetBuffer(), sMsg.szMsgString);
    }
    else if (CT_PUBLIC == eChatTarget)
    {
		static CString clr3;
		static bool bGetColor3 = false;
		if (!bGetColor3)
		{        
			TCHAR szPath[MAX_PATH];
			wsprintf(szPath,"GameOption.bcf");
			CBcfFile f(szPath);
			clr3=f.GetKeyVal("Chat","PublicColor","0xffffff00");
			bGetColor3 = true;
		}
        wsprintf(pResult, TEXT("<font><color=0xffff0000,size=12,style=����>%s��</font><font><color=%s,size=12,style=����>%s</font>"),clr3.GetBuffer(), sMsg.szSourceNickName, sMsg.szMsgString);
    }
    else if (CT_NORMAL == eChatTarget)
    {
        static CString clr;
        static bool bGetColor = false;
        if (!bGetColor)
        {        
            TCHAR szPath[MAX_PATH];
            wsprintf(szPath,"GameOption.bcf");
            CBcfFile f(szPath);
            clr=f.GetKeyVal("Chat","NotifyColor","0xffffff00");
            bGetColor = true;
        }

        wsprintf(pResult, TEXT("<font><color=%s,size=12,style=����>%s</font>"), clr.GetBuffer(), sMsg.szMsgString);
    }
    else if (CT_DALABA == eChatTarget)
    {
		static CString clr4;
		static bool bGetColor4 = false;
		if (!bGetColor4)
		{        
			TCHAR szPath[MAX_PATH];
			wsprintf(szPath,"GameOption.bcf");
			CBcfFile f(szPath);
			clr4=f.GetKeyVal("Chat","DaLaBaColor","0xFFFF0000");
			bGetColor4 = true;
			
		}

		///����������ɫֵ�����Ƿ��ģ����������ת��
		int rColor = GetBValue(sMsg.crTextColor);
		int GColor = GetGValue(sMsg.crTextColor);
		int BColor = GetRValue(sMsg.crTextColor);
		COLORREF temp = RGB(rColor,GColor,BColor);

        wsprintf(pResult, TEXT("<font><color=%s,size=12,style=����>%s</font><font><color=0xFF0000FF,size=12,style=����>���Ŵ����Ⱥ�:</font>\
                               <font><color=0xFF%06X,size=12,style=����>%s</font>"),clr4.GetBuffer(),sMsg.szSourceNickName, temp /*sMsg.crTextColor*/,sMsg.szMsgString);
		
    }
    else if (CT_XIAOLABA == eChatTarget)
    {
		static CString clr5;
		static bool bGetColor5 = false;
		if (!bGetColor5)
		{        
			TCHAR szPath[MAX_PATH];
			wsprintf(szPath,"GameOption.bcf");
			CBcfFile f(szPath);
			clr5=f.GetKeyVal("Chat","XiaoLaBaColor","0xFFFF0000");
			bGetColor5 = true;
			//GetRValue();
		}
		///����������ɫֵ�����Ƿ��ģ����������ת��
		int rColor = GetBValue(sMsg.crTextColor);
		int GColor = GetGValue(sMsg.crTextColor);
		int BColor = GetRValue(sMsg.crTextColor);
		COLORREF temp = RGB(rColor,GColor,BColor);

        wsprintf(pResult, TEXT("<font><color=%s,size=12,style=����>%s</font><font><color=0xFF0000FF,size=12,style=����>����С���Ⱥ�:</font>\
                               <font><color=0xFF%06X,size=12,style=����>%s</font>"),clr5.GetBuffer(), sMsg.szSourceNickName, temp/*sMsg.crTextColor*/,sMsg.szMsgString);
	
    }
    return pResult;
}

int XLogic::InitPlayerListCtrl()
{
    IListControl *pList = NULL;
    GETCTRL(IListControl, pList, m_pIUI, RIGHTBOARD_USERLIST);
    if (NULL==pList)
    {
        return -1;
    }


	CString strColumnName;
	CString strColumnIndex;
	CString strColumnIndexAll;
	CString strConfig;
	BOOL    bConfigExist;

	CString strDefaultColumnIndexAll = "Nickname,Coin,WealthLevel,Score,ScoreLevel,Charm,CharmLevel,Sex,GameCount,WinGame,LoseGame,Peace,Escape,WatchUser,";
	bConfigExist = TRUE;

	CBcfFile fUserList(CBcfFile::GetAppPath () + "..\\GameUserInforList.bcf");
	strColumnIndexAll = fUserList.GetKeyVal("InforList","_ColumnOrder","");
	if (strColumnIndexAll.IsEmpty())
	{
		bConfigExist = FALSE;
		strColumnIndexAll = strDefaultColumnIndexAll;
	}

	int iIndex = 0;
	int iWidth = 0;
	int nColCount = 0;
	int nHeadPos = 0;
	int nTailPos = strColumnIndexAll.Find(',', nHeadPos);
	while (nTailPos > nHeadPos)
	{
		

		strColumnIndex = strColumnIndexAll.Mid(nHeadPos, nTailPos-nHeadPos);
		if (bConfigExist)
			strConfig = fUserList.GetKeyVal("InforList",strColumnIndex,"");
		else
			strConfig = COLOMN_NAME[iIndex];

		if (!strConfig.IsEmpty())
		{
			DecodeString(strConfig,strColumnName,iWidth);

			wchar_t szStr[10];
			char2wchar(strColumnName.GetBuffer(strColumnName.GetLength()), szStr);
			pList->InsertColumn(iIndex,szStr,iWidth,20);
			//strncpy(ColStr.ColumnName,strColumnName,MAX_PATH/2);
			//m_MapColumn.insert(pair<string,ColumnStruct>(strColumnIndex.GetBuffer(),ColStr));
			m_MapColumn.insert(pair<string,int>(strColumnIndex.GetBuffer(),iIndex));
			iIndex++;
		}

		nHeadPos = nTailPos + 1;
		nTailPos = strColumnIndexAll.Find(',', nHeadPos);
	}
	//
    /*if (pList->GetColNum()<12)
    {
		CString str;
		wchar_t szStr[10];
		CBcfFile fUserList(CBcfFile::GetAppPath () + "..\\GameUserInforList.bcf");

		str = fUserList.GetKeyVal("InforList","NickName","�ǳ�");
		str.Replace(',', '\0');
		char2wchar(str.GetBuffer(str.GetLength()), szStr);
        pList->InsertColumn(0,szStr,80,20);

		str = fUserList.GetKeyVal("InforList","Coin","�ֶ�");
		str.Replace(',', '\0');
		char2wchar(str.GetBuffer(str.GetLength()), szStr);
        pList->InsertColumn(1,szStr,80,20);

		str = fUserList.GetKeyVal("InforList","WealthLevel","�Ƹ�����");
		str.Replace(',', '\0');
		char2wchar(str.GetBuffer(str.GetLength()), szStr);
        pList->InsertColumn(2,szStr,60,20);

		str = fUserList.GetKeyVal("InforList","Score","����");
		str.Replace(',', '\0');
		char2wchar(str.GetBuffer(str.GetLength()), szStr);
        pList->InsertColumn(3,szStr,60,20);

		str = fUserList.GetKeyVal("InforList","ScoreLevel","���ּ���");
		str.Replace(',', '\0');
		char2wchar(str.GetBuffer(str.GetLength()), szStr);
        pList->InsertColumn(4,szStr,60,20);

		str = fUserList.GetKeyVal("InforList","GameCount","�ܾ���");
		str.Replace(',', '\0');
		char2wchar(str.GetBuffer(str.GetLength()), szStr);
        pList->InsertColumn(5,szStr,60,20);

		str = fUserList.GetKeyVal("InforList","WinGame","ʤ����");
		str.Replace(',', '\0');
		char2wchar(str.GetBuffer(str.GetLength()), szStr);
        pList->InsertColumn(6,szStr,60,20);

		str = fUserList.GetKeyVal("InforList","LoseGame","�����");
		str.Replace(',', '\0');
		char2wchar(str.GetBuffer(str.GetLength()), szStr);
        pList->InsertColumn(7,szStr,60,20);

		str = fUserList.GetKeyVal("InforList","Peace","�;���");
		str.Replace(',', '\0');
		char2wchar(str.GetBuffer(str.GetLength()), szStr);
        pList->InsertColumn(8,szStr,60,20);

		str = fUserList.GetKeyVal("InforList","WatchUser","�Թ۶���");
		str.Replace(',', '\0');
		char2wchar(str.GetBuffer(str.GetLength()), szStr);
        pList->InsertColumn(9,szStr,60,20);

		str = fUserList.GetKeyVal("InforList","Charm","����");
		str.Replace(',', '\0');
		char2wchar(str.GetBuffer(str.GetLength()), szStr);
        pList->InsertColumn(10,szStr,60,20);

		str = fUserList.GetKeyVal("InforList","CharmLevel","��������");
		str.Replace(',', '\0');
		char2wchar(str.GetBuffer(str.GetLength()), szStr);
        pList->InsertColumn(11,szStr,60,20);
    }
    else
    {

        
    }*/




	CBcfFile f("GameOption.bcf");
	
	int r=f.GetKeyVal("GameList","GameListSelectTextR",255);
	int g=f.GetKeyVal("GameList","GameListSelectTextG",0);
	int b=f.GetKeyVal("GameList","GameListSelectTextB",0);
		

    int nCount = pList->GetColNum();
    for (int i=0; i<nCount; ++i)
    {
        pList->SetTextShowStyle(i, AlignmentCenter);
    }

	
    pList->SetSelectRectColor(RGB(r,g,b)+0xff000000);   
    
	

	//pList->SetSelectRectColor(0xffffffff);

	if (m_pUserListDu)
	{
		m_pUserListDu->InitListCtrl(m_GameInfo.pOrderName,m_GameInfo.uComType);
	}

    return 0;
}

int XLogic::RefreshPlayList()
{

    IListControl *pList = NULL;
    GETCTRL(IListControl, pList, m_pIUI, RIGHTBOARD_USERLIST);
    if (NULL==pList)
    {
        return -1;
    }
   /* if (pList->GetColNum()<12)
    {
        return -2;
    }*/

    pList->Clear();
    int nUserCountInThisDesk = m_ThisDeskPtrArray.GetCount();
    for (int i=0; i<nUserCountInThisDesk; ++i)
    {
		
        UserItemStruct *pFindItem = (UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);

		UserItemStruct *pUserInfo = pFindItem;

		if (!pUserInfo)
		{
			continue;
		}

		char szContent[300]={0};
		map<string, int>::iterator ite; 
		
		// �ǳ�
		ite = m_MapColumn.find("Nickname"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%s",pFindItem->GameUserInfo.nickName);
			if ((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID)
			{			
				sprintf(szContent,"���");
			}
			 pList->InsertInItem(i, ite->second, CA2W(szContent));
		}

		// �ֶ�
		ite = m_MapColumn.find("Coin"); 
		if (m_MapColumn.end() != ite)
		{
			// �ֶ�
			GlbGetNumString(szContent, pFindItem->GameUserInfo.i64Money, Glb().m_nPowerOfGold, Glb().m_bUseSpace, TEXT(","));
			if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
			{			
				sprintf(szContent,"???");
			}
			if (m_GameInfo.dwRoomRule & GRR_CONTEST)
			{
				sprintf(szContent,"???");
			}
			pList->InsertInItem(i, ite->second, CA2W(szContent));
		}
           // �Ƹ�����
		ite = m_MapColumn.find("WealthLevel"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%s",GetMoneyOrderName(pFindItem->GameUserInfo.i64Money));
			if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
			{			
				sprintf(szContent,"???");
			}
			if (m_GameInfo.dwRoomRule & GRR_CONTEST)
			{
				sprintf(szContent,"???");
			}
			pList->InsertInItem(i, ite->second, CA2W(szContent));
		}
  
        // ����
		ite = m_MapColumn.find("Score"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%d",pFindItem->GameUserInfo.dwPoint-GetNormalOrderBasePoint(pFindItem->GameUserInfo.dwPoint));
			if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
			{			
				sprintf(szContent,"???");
			}
			pList->InsertInItem(i, ite->second, CA2W(szContent));
		}

        // ���ּ���
		ite = m_MapColumn.find("ScoreLevel"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%s", m_GameInfo.pOrderName(pFindItem->GameUserInfo.dwPoint));
			if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
			{			
				sprintf(szContent,"???");
			}
			pList->InsertInItem(i, ite->second, CA2W(szContent));
		}

    	// �ܾ���
		ite = m_MapColumn.find("GameCount"); 
		if (m_MapColumn.end() != ite)
		{
			UINT uAllCount1=pFindItem->GameUserInfo.uWinCount+pFindItem->GameUserInfo.uLostCount+pFindItem->GameUserInfo.uMidCount;
			sprintf(szContent,"%d",uAllCount1);
			if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
			{			
				sprintf(szContent,"???");
			}
			pList->InsertInItem(i, ite->second, CA2W(szContent));
		}
         
		// ʤ����
		ite = m_MapColumn.find("WinGame"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%d",pFindItem->GameUserInfo.uWinCount);
			if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
			{			
				sprintf(szContent,"???");
			}
			pList->InsertInItem(i, ite->second, CA2W(szContent));
		}
        // �����
		ite = m_MapColumn.find("LoseGame"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%d",pFindItem->GameUserInfo.uLostCount);
			if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
			{			
				sprintf(szContent,"???");
			}
			pList->InsertInItem(i, ite->second, CA2W(szContent));
		}
        // �;���
		ite = m_MapColumn.find("Peace"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%d",pFindItem->GameUserInfo.uMidCount);
			if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
			{			
				sprintf(szContent,"???");
			}
			pList->InsertInItem(i, ite->second, CA2W(szContent));
		}

        //// �Թ۶���
		ite = m_MapColumn.find("WatchUser"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"");
			pList->InsertInItem(i, ite->second, CA2W(szContent));
		}
        // ����
		ite = m_MapColumn.find("Charm"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%d", pFindItem->GameUserInfo.dwFascination);
			if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
			{			
				sprintf(szContent,"???");
			}
			pList->InsertInItem(i, ite->second, CA2W(szContent));
		}
        // ��������
		ite = m_MapColumn.find("CharmLevel"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%s", GetCharmGrade(pFindItem->GameUserInfo.dwFascination));
			if (((m_GameInfo.dwRoomRule & GRR_NOTCHEAT) && pUserInfo->GameUserInfo.dwUserID != GetMeUserInfo()->dwUserID) || m_GameInfo.dwRoomRule & GRR_CONTEST)
			{			
				sprintf(szContent,"???");
			}
			pList->InsertInItem(i, ite->second, CA2W(szContent));
		}
		CString stime;
		stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
		int curtime=atoi(stime);                        //ת��Ϊint

		///vip����ú�ɫ��������ð�ɫ
		if(pFindItem->GameUserInfo.iVipTime>=curtime)
		{
			pList->SetRowFontColor(i, 0xff0000);
		}
		else
		{
			TCHAR szPath[MAX_PATH];
			wsprintf(szPath,"GameOption.bcf");
			CBcfFile f(szPath);
			int r,g,b;
			r=f.GetKeyVal("GameList","GameListTextR",0);
			g=f.GetKeyVal("GameList","GameListTextG",255);
			b=f.GetKeyVal("GameList","GameListTextB",255);
			COLORREF GameListTextColor=RGB(r,g,b);

			pList->SetRowFontColor(i, GameListTextColor);
		}
    }

    int nWatchCount = m_WatchPtrArray.GetCount();
    for (int i=0; i<nWatchCount; ++i)
    {
        UserItemStruct *pFindItem = (UserItemStruct *)m_WatchPtrArray.GetAt(i);
        if (NULL==pFindItem)
        {
            continue;
        }
        // �ǳ�
        //char szContent[300]={0};
        //sprintf(szContent,"%s",pFindItem->GameUserInfo.szName);
        //pList->InsertInItem(nUserCountInThisDesk+i, 0, CA2W(szContent));
        //// �ֶ�
        //GlbGetNumString(szContent, pFindItem->GameUserInfo.i64Money, Glb().m_nPowerOfGold, false, TEXT(","));
        //pList->InsertInItem(nUserCountInThisDesk+i, 1, CA2W(szContent));
        //// �Ƹ�����
        //sprintf(szContent,"%s",GetMoneyOrderName(pFindItem->GameUserInfo.i64Money));
        //pList->InsertInItem(nUserCountInThisDesk+i, 2, CA2W(szContent));
        //// ����
        //sprintf(szContent,"%d",pFindItem->GameUserInfo.dwPoint-GetNormalOrderBasePoint(pFindItem->GameUserInfo.dwPoint));
        //pList->InsertInItem(nUserCountInThisDesk+i, 3, CA2W(szContent));
        //// ���ּ���
        //sprintf(szContent,"%s", m_GameInfo.pOrderName(pFindItem->GameUserInfo.dwPoint));
        //pList->InsertInItem(nUserCountInThisDesk+i, 4, CA2W(szContent));
        //// �ܾ���
        //UINT uAllCount1=pFindItem->GameUserInfo.uWinCount+pFindItem->GameUserInfo.uLostCount+pFindItem->GameUserInfo.uMidCount;
        //sprintf(szContent,"%d",uAllCount1);
        //pList->InsertInItem(nUserCountInThisDesk+i, 5, CA2W(szContent));
        //// ʤ����
        //sprintf(szContent,"%d",pFindItem->GameUserInfo.uWinCount);
        //pList->InsertInItem(nUserCountInThisDesk+i, 6, CA2W(szContent));
        //// �����
        //sprintf(szContent,"%d",pFindItem->GameUserInfo.uLostCount);
        //pList->InsertInItem(nUserCountInThisDesk+i, 7, CA2W(szContent));
        //// �;���
        //sprintf(szContent,"%d",pFindItem->GameUserInfo.uMidCount);
        //pList->InsertInItem(nUserCountInThisDesk+i, 8, CA2W(szContent));
        ////// �Թ۶���
        //UserItemStruct *pWatchUser = FindOnLineUser(pFindItem->GameUserInfo.bDeskStation);
        //if (pWatchUser)
        //{
        //    sprintf(szContent,"%s", pWatchUser->GameUserInfo.nickName);
        //}
        //pList->InsertInItem(nUserCountInThisDesk+i, 9, CA2W(szContent));
        //// ����
        //sprintf(szContent,"%d", pFindItem->GameUserInfo.dwFascination);
        //pList->InsertInItem(nUserCountInThisDesk+i, 10, CA2W(szContent));
        //// ��������
        //sprintf(szContent,"%s", GetCharmGrade(pFindItem->GameUserInfo.dwFascination));
        //pList->InsertInItem(nUserCountInThisDesk+i, 11, CA2W(szContent));


		char szContent[300]={0};
		map<string, int>::iterator ite; 

		// �ǳ�
		ite = m_MapColumn.find("Nickname"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%s",pFindItem->GameUserInfo.nickName);
			pList->InsertInItem(nUserCountInThisDesk+i, ite->second, CA2W(szContent));
		}

		// �ֶ�
		ite = m_MapColumn.find("Coin"); 
		if (m_MapColumn.end() != ite)
		{
			// �ֶ�
			GlbGetNumString(szContent, pFindItem->GameUserInfo.i64Money, Glb().m_nPowerOfGold, Glb().m_bUseSpace, TEXT(","));
			pList->InsertInItem(nUserCountInThisDesk+i, ite->second, CA2W(szContent));
		}
		// �Ƹ�����
		ite = m_MapColumn.find("WealthLevel"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%s",GetMoneyOrderName(pFindItem->GameUserInfo.i64Money));
			pList->InsertInItem(nUserCountInThisDesk+i, ite->second, CA2W(szContent));
		}

		// ����
		ite = m_MapColumn.find("Score"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%d",pFindItem->GameUserInfo.dwPoint-GetNormalOrderBasePoint(pFindItem->GameUserInfo.dwPoint));
			pList->InsertInItem(nUserCountInThisDesk+i, ite->second, CA2W(szContent));
		}

		// ���ּ���
		ite = m_MapColumn.find("ScoreLevel"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%s", m_GameInfo.pOrderName(pFindItem->GameUserInfo.dwPoint));
			pList->InsertInItem(nUserCountInThisDesk+i, ite->second, CA2W(szContent));
		}

		// �ܾ���
		ite = m_MapColumn.find("GameCount"); 
		if (m_MapColumn.end() != ite)
		{
			UINT uAllCount1=pFindItem->GameUserInfo.uWinCount+pFindItem->GameUserInfo.uLostCount+pFindItem->GameUserInfo.uMidCount;
			sprintf(szContent,"%d",uAllCount1);
			pList->InsertInItem(nUserCountInThisDesk+i, ite->second, CA2W(szContent));
		}

		// ʤ����
		ite = m_MapColumn.find("WinGame"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%d",pFindItem->GameUserInfo.uWinCount);
			pList->InsertInItem(nUserCountInThisDesk+i, ite->second, CA2W(szContent));
		}
		// �����
		ite = m_MapColumn.find("LoseGame"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%d",pFindItem->GameUserInfo.uLostCount);
			pList->InsertInItem(nUserCountInThisDesk+i, ite->second, CA2W(szContent));
		}
		// �;���
		ite = m_MapColumn.find("Peace"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%d",pFindItem->GameUserInfo.uMidCount);
			pList->InsertInItem(nUserCountInThisDesk+i, ite->second, CA2W(szContent));
		}

		//// �Թ۶���
		ite = m_MapColumn.find("WatchUser"); 
		
		if (m_MapColumn.end() != ite)
		{
			UserItemStruct *pWatchUser = FindOnLineUser(pFindItem->GameUserInfo.bDeskStation);
			
			if (pWatchUser)
			{
				sprintf(szContent,"%s", pWatchUser->GameUserInfo.nickName);
				
			}
			//sprintf(szContent,"");
			pList->InsertInItem(nUserCountInThisDesk+i, ite->second, CA2W(szContent));
		}
		// ����
		ite = m_MapColumn.find("Charm"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%d", pFindItem->GameUserInfo.dwFascination);
			pList->InsertInItem(nUserCountInThisDesk+i, ite->second, CA2W(szContent));
		}
		// ��������
		ite = m_MapColumn.find("CharmLevel"); 
		if (m_MapColumn.end() != ite)
		{
			sprintf(szContent,"%s", GetCharmGrade(pFindItem->GameUserInfo.dwFascination));
			pList->InsertInItem(nUserCountInThisDesk+i, ite->second, CA2W(szContent));
		}

		CString stime;
		stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
		int curtime=atoi(stime);                        //ת��Ϊint

		
		if(pFindItem->GameUserInfo.iVipTime>=curtime)
		{
			pList->SetRowFontColor(i, 0xff0000);
		}
		else
		{
			TCHAR szPath[MAX_PATH];
			wsprintf(szPath,"GameOption.bcf");
			CBcfFile f(szPath);
			int r,g,b;
			r=f.GetKeyVal("GameList","GameListTextR",0);
			g=f.GetKeyVal("GameList","GameListTextG",255);
			b=f.GetKeyVal("GameList","GameListTextB",255);
			COLORREF GameListTextColor=RGB(r,g,b);
			pList->SetRowFontColor(i, GameListTextColor);
		}
       

    }

	
    return 0;
}



/// ��ʼ�������б�
int XLogic::InitPropPannel()
{
    IContain *pSingleProp = NULL;
    GETCTRL(IContain, pSingleProp, m_pIUI, RIGHTBOARD_PROP_PROP_1);
    if (NULL==pSingleProp)
    {
        return -1;
    }
    pSingleProp->SetControlVisible(false);
    IContain *pPropBoard = NULL;
    GETCTRL(IContain, pPropBoard, m_pIUI, RIGHTBOARD_PROP_CTN);
    if (NULL==pPropBoard)
    {
        return -2;
    }
    pPropBoard->GetContainPaginator()->SetNumPerPage(PROP_PEER_PAGE+1);
    pPropBoard->GetContainPaginator()->SetSpaces(15, 5, 15, 5, 2, 5);

    IBCInterFace *pBtnBuyVip = m_pIUI->GetIControlbyID(BN_BUY_VIP);
    if (NULL!=pBtnBuyVip)
    {
        pBtnBuyVip->SetControlVisible(false);
        pPropBoard->GetContainPaginator()->InsertPagecontent(pBtnBuyVip);
    }    
    /// ��¡���߰�ť
    for (int i=0; i<PROP_PEER_PAGE; ++i)
    {
        m_pPropContain[i] = dynamic_cast<IContain *>(pSingleProp->Clone(PROP_BTN_BEGIN+PROP_IDS*i));
        if (NULL!=m_pPropContain[i])
        {
            pPropBoard->GetContainPaginator()->InsertPagecontent(m_pPropContain[i]);
        }        
    }
    CString s=CBcfFile::GetAppPath();
    CBcfFile fini(s+"..\\bzgame.bcf");
    m_strPropUrl=fini.GetKeyVal("BZW","PropPannelUrl","http://game.bzw.cn/app/Prop/PropInfo.asp");

    CBcfFile f( s + "..\\skin.bcf");
    CString key=TEXT("PropPannel");
    int r,g,b;
    m_bkcolor_R = f.GetKeyVal(key,"PropPannelR",255);
    m_bkcolor_G = f.GetKeyVal(key,"PropPannelG",255);
    m_bkcolor_B = f.GetKeyVal(key,"PropPannelB",255);

    if(NULL==m_pPropPannel)
    {
        m_pPropPannel=new CPropPannel();
        m_pPropPannel->m_pGameInfo = &m_GameInfo;
        m_pPropPannel->m_pParent = m_pEngine->GetCWnd();
		m_pPropPannel->m_bUsingNewUI = true;
		m_pPropPannel->m_pLogic = this;
        AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
        m_pPropPannel->Create(IDD_PROPPANNEL,m_pPropPannel->m_pParent);
        AfxSetResourceHandle(GetModuleHandle(NULL));
        m_pPropPannel->CenterWindow();
        m_pPropPannel->ResetData();
    }
    if(m_pPropSelUser==NULL)
    {
        m_pPropSelUser=new CPropSelUser();
        AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
        m_pPropSelUser->Create(IDD_PROP_SEL_USER,m_pEngine->GetCWnd());
        AfxSetResourceHandle(GetModuleHandle(NULL));
    }
    if(m_dlgBroadcast==NULL)
    {
        m_dlgBroadcast=new CBoardCast();
        AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
        m_dlgBroadcast->Create(IDD_BOARDCAST,m_pEngine->GetCWnd());
        AfxSetResourceHandle(GetModuleHandle(NULL));
        m_dlgBroadcast->pParentWnd=m_pEngine->GetCWnd();
    }
    return 0;
}

int XLogic::RefreshPropPannel()
{
    const int nMaxPage = Glb().userPropLibrary.GetCount()/PROP_PEER_PAGE; 
    if (m_nPropPage>nMaxPage)
    {
        m_nPropPage = nMaxPage;
    }
    else if (m_nPropPage<0)
    {
        m_nPropPage = 0;

    }

    if (nMaxPage==m_nPropPage)
    {
        UIEnableButton(RIGHTBOARD_PROP_BTN_NEXT1, false);
        UIEnableButton(RIGHTBOARD_PROP_BTN_NEXT2, false);
        UIEnableButton(RIGHTBOARD_PROP_BTN_PRE1, true);
        UIEnableButton(RIGHTBOARD_PROP_BTN_PRE2, true);
    }   
    else if (0==m_nPropPage)
    {
        UIEnableButton(RIGHTBOARD_PROP_BTN_NEXT1, true);
        UIEnableButton(RIGHTBOARD_PROP_BTN_NEXT2, true);
        UIEnableButton(RIGHTBOARD_PROP_BTN_PRE1, false);
        UIEnableButton(RIGHTBOARD_PROP_BTN_PRE2, false);
    }
    else
    {
        UIEnableButton(RIGHTBOARD_PROP_BTN_NEXT1, true);
        UIEnableButton(RIGHTBOARD_PROP_BTN_NEXT2, true);
        UIEnableButton(RIGHTBOARD_PROP_BTN_PRE1, true);
        UIEnableButton(RIGHTBOARD_PROP_BTN_PRE2, true);
    }
    


    wchar_t wszPropImagePath[MAX_PATH];
    wsprintfW(wszPropImagePath, L"gamecomm\\right\\prop\\");
    wchar_t wszPropImage[MAX_PATH]={0};

    /// ��ʾ����
    for (int i=m_nPropPage*PROP_PEER_PAGE; i<m_nPropPage*PROP_PEER_PAGE+PROP_PEER_PAGE; ++i)
    {
        IButton *pButton = NULL;
        IText   *pTextNum = NULL;
        GETCTRL(IButton, pButton, m_pIUI, PROP_BTN_BEGIN+PROP_IDS*(i%PROP_PEER_PAGE)+1);
        GETCTRL(IText, pTextNum, m_pIUI, PROP_BTN_BEGIN+PROP_IDS*(i%PROP_PEER_PAGE)+2);

		if (!pButton || !pTextNum)
		{
			continue;
		}

        if (i<Glb().userPropLibrary.GetCount())
        {
            _TAG_USERPROP *userProp=Glb().userPropLibrary.GetAt(i);
            if (NULL!=pButton && NULL!=pTextNum && userProp!=NULL)
            {
                wsprintfW(wszPropImage, L"%sProp%d.bmp", wszPropImagePath, i+1);
                pButton->LoadPic(wszPropImage);
                wchar_t wszNum[100]={0};            
                wsprintfW(wszNum, L"%d", userProp->nHoldCount);
                pTextNum->SetText(wszNum);
            }
        }
        else
        {
            wsprintfW(wszPropImage, L"%sPropEmpty.bmp", wszPropImagePath);
            pButton->LoadPic(wszPropImage);
            pTextNum->SetText(L"0");      
        }

    }    


    if (m_pPropPannel!=NULL)
    {
        m_pPropPannel->ResetData();
    }

	if (m_pRightFrame)
	{
		m_pRightFrame->OnSetPropImages();
	}

    return 0;
}

bool XLogic::UIEnableButton(int nID, bool bEnable)
{
    IButton *pbtn=NULL;
    GETCTRL(IButton, pbtn, m_pIUI, nID);
    if (NULL!=pbtn)
    {
        pbtn->SetEnable(bEnable);
        return bEnable;
    }

    return false;
}

void XLogic::OnHitPropBtn(int nIndex)
{
    m_pPropPannel->setSkinIndex(0);
    m_pPropPannel->openPannel(0,"", nIndex);

    //int nIndexInList = nIndex+m_nPropPage*PROP_PEER_PAGE;
    //if (nIndexInList>=Glb().userPropLibrary.GetCount())
    //{
    //    return;
    //}
    //_TAG_USERPROP *userProp=Glb().userPropLibrary.GetAt(nIndexInList);

    ///// �򿪵���ҳ��
    //IContain *pContain = dynamic_cast<IContain *>(m_pIUI->GetIControlbyID(BUYPROP_CTN));
    //IWeb *pWeb = dynamic_cast<IWeb *>(m_pIUI->GetIControlbyID(BUYPROP_WEB_PROPINFO));
    //IEditorText *pOwnNum = dynamic_cast<IEditorText *>(m_pIUI->GetIControlbyID(BUYPROP_TEXT_OWNNUM));
    //IEditorText *pBuyNum = dynamic_cast<IEditorText *>(m_pIUI->GetIControlbyID(BUYPROP_TEXT_BUYNUM));
    //IButton *pBtnUse = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BUYPROP_BTN_USE));
    //IButton *pBtnBuy = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BUYPROP_BTN_BUY));
    //IButton *pBtnClose = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BUYPROP_BTN_CLOSE));
    //IText *pNeedGold = dynamic_cast<IText *>(m_pIUI->GetIControlbyID(BUYPROP_TEXT_NEEDGOLD));
    //IText *pHaveGold = dynamic_cast<IText *>(m_pIUI->GetIControlbyID(BUYPROP_TEXT_HAVEGOLD));

    //if (pContain!=NULL)
    //{
    //    pContain->SetControlVisible(true);
    //}

    //// ����ҳ��
    //if (pWeb!=NULL)
    //{
    //    pWeb->SetUseWindow(true);
    //    CString surl;
    //    surl.Format("%s?propid=%d&bgcolor=%X%X%X",m_strPropUrl,userProp->nPropID,m_bkcolor_R,m_bkcolor_G,m_bkcolor_B);
    //    pWeb->SetShowWebAddress(CA2W(surl.GetBuffer()));
    //}    
    //// ӵ������
    //if (pOwnNum!=NULL)
    //{
    //    wchar_t wszOwnNum[200]={0};
    //    wsprintfW(wszOwnNum, L"%d", userProp->nHoldCount);
    //    pOwnNum->SetText(wszOwnNum);
    //    pOwnNum->SetEnable(false);
    //}  
    //// ��������
    //if (pBuyNum!=NULL)
    //{
    //    pBuyNum->SetText(L"1");
    //    pBuyNum->SetEnableASCII(1, '0', '9');
    //    pBuyNum->SetMaxTypeIn(3);
    //}
    //// ������
    //if (pNeedGold!=NULL)
    //{
    //    pNeedGold->SetText(L"");
    //}   
    //// ���н��
    //if (pHaveGold!=NULL)
    //{
    //    char szNum[32]; 
    //    GlbGetNumString(szNum, m_GameInfo.uisMeUserInfo.GameUserInfo.dwBank, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
    //    wchar_t wszMoneyText[300];
    //    wsprintfW(wszMoneyText, L"�������ʹ�õ����������еĽ�ң��������������� %s ��ҡ�", CA2W(szNum));
    //    pHaveGold->SetText(wszMoneyText);
    //}    
}

// ���ÿ�
void XLogic::OnSetUIMessage(TUIMessage *pMessage)
{
    if (pMessage->_uMessageType==UI_LBUTTONUP)
    {
        IContain *pCtn = dynamic_cast<IContain *>(m_pIUI->GetIControlbyID(CTN_SET_DLG));
        ISlider *pSliderVol = dynamic_cast<ISlider *>(m_pIUI->GetIControlbyID(SLIDER_SET_VOL));
        IRadioButton *pEnableWatch = dynamic_cast<IRadioButton *>(m_pIUI->GetIControlbyID(CHECK_ENABLEWATCH));

        if (NULL==pCtn || NULL==pSliderVol || NULL==pEnableWatch)
        {
            return;
        }

        switch (pMessage->_uControlID)
        {
        case BTN_SET:
            {
                // ��ʾ���ÿ�
                pCtn->SetControlVisible(true);
            }
            break;
        case BTN_SET_CLOSE:
            {
                // ��ʾ���ÿ�
                pCtn->SetControlVisible(false);
            }
            break;
        case BTN_SET_OK:
            {
                // ��ʾ���ÿ�
                pCtn->SetControlVisible(false);
            }
            break;
        case BTN_SET_CANCEL:
            {
                // ��ʾ���ÿ�
                pCtn->SetControlVisible(false);
            }
            break;
        case CHECK_ENABLEWATCH:
            {
                // �����Թ�
            }
            break;
        default:
            break;
        }
    }
}
void XLogic::OnBuyPropUIMessage(TUIMessage *pMessage)
{
    //switch (pMessage->_uControlID)
    //{    
    //case BUYPROP_TEXT_BUYNUM:   // �������� 
    //    {

    //    }
    //    break;
    //case BUYPROP_BTN_USE:       // ʹ��
    //    {

    //    }
    //    break;
    //case BUYPROP_BTN_BUY:       // ����
    //    {

    //    }
    //    break;
    //case BUYPROP_BTN_CLOSE:     // �ر�
    //    {
    //        if (pMessage->_uMessageType==UI_LBUTTONUP)
    //        {
    //            IContain *pContain = dynamic_cast<IContain *>(m_pIUI->GetIControlbyID(BUYPROP_CTN));
    //            if (NULL!=pContain)
    //            {
    //                pContain->SetControlVisible(false);
    //            }
    //        }            
    //    }
    //    break;
    //default:
    //    break;
    //}
}


// ʹ�õ���     // PengJiLin, 2010-10-14, ����һ����
void XLogic::UsePropItem(int propIndex, BOOL bCheckNum)
{
    //////////////////////////////////////////////////////////////////////////
    //Modify Fred Huang,2008-04-23
    if(GetMeUserInfo()->bUserState==USER_WATCH_GAME)
    {
        AFCMessageBox("�Բ����Թ�ʱ����ʹ�õ��ߣ�","��ʾ");
        return;
    }
	if (m_GameInfo.dwRoomRule & GRR_CONTEST)
	{
		AFCMessageBox("����������ʹ�õ���!");
		return;
	}
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //2008-08-11
    int propCount=(int)Glb().userPropLibrary.GetCount();
    if(propIndex<0 || propIndex>=propCount)
    {
        //û�е��ߣ��������
        //OnGetBuyItemData(1);
        return;
    }
    _TAG_USERPROP * userProp=Glb().userPropLibrary.GetAt(propIndex);
    if(userProp==NULL) return;

    // PengJiLin, 2010-10-14, ���ߵ���������Ϊ0���봦��
    if(userProp->nHoldCount <= 0 && TRUE == bCheckNum)
    {
		TCHAR chInfo[256] = {0};
		wsprintf(chInfo, "��Ǹ��[%s]�Ѿ�ʹ����ϣ����ȹ�����ʹ�ã�", userProp->szPropName);
		if(IDOK == AFCMessageBox(chInfo))
		{
			return ;
		}
		return ; 
        //TCHAR chInfo[256] = {0};
        //wsprintf(chInfo, "��Ǹ��[%s]�Ѿ�ʹ����ϣ��Ƿ����ڹ���ʹ��?", userProp->szPropName);
        //if(IDOK == AFCMessageBox(chInfo, TEXT("��ʾ"), AFC_YESNO, this))  // ȷ������
        //{
        //    BuyAndUseProp(propIndex);
        //    return;
        //}
        //else        // ȡ������
        //{
        //    return;
        //}
    }

    if(userProp->attribAction & 0x80 || userProp->attribAction &0x100)
    {
        //�����Ⱥ�С����
        UseBoardCast( userProp );
        return;
    }

    // PengJiLin, 2010-6-1, ������ ����
    if(userProp->attribAction & 0x200)
    {
        //this->UseLookCardProp(userProp);
        return;
    }

    // PengJiLin, 2010-9-9, ���˿�����
    if(userProp->attribAction & 0x400)
    {
        UseKickUserProp(userProp);
        return;
    }

    // PengJiLin, 2010-9-9, ���߿�����
    if(userProp->attribAction & 0x800)
    {
        UseAntiKickProp(userProp);
        return;
    }

    //���ֶ��Լ�ʹ�ã����Ƕ�������ʹ��
    if(userProp->attribAction & 0x01)
    {
        //��������ʹ��
        if(m_pPropSelUser==NULL)
        {
            m_pPropSelUser=new CPropSelUser;
            AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
            m_pPropSelUser->Create(IDD_PROP_SEL_USER,m_pEngine->GetCWnd());
            AfxSetResourceHandle(GetModuleHandle(NULL));
        }
        INT_PTR uActiveCount=m_ThisDeskPtrArray.GetCount();
        for (int i=0;i<uActiveCount;i++)
        {
            UserItemStruct * pFindUserItem=(UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
            m_pPropSelUser->m_pUserItem[i] = pFindUserItem;
            /*if ((pFindUserItem!=NULL)&&(pFindUserItem->GameUserInfo.dwUserID==dwUserID)) 
                return pFindUserItem;*/
        }
        //for(int i=0;i<MAX_PEOPLE;i++)
        //{
        //    m_pPropSelUser->m_pUserItem[i]=m_pUserInfo[i];
        //}
        m_pPropSelUser->dwUsedPropID=propIndex;
        m_pPropSelUser->dwLocalUserID=GetMeUserInfo()->dwUserID;
        m_pPropSelUser->propItemName=userProp->szPropName;
        m_pPropSelUser->resetInformation();
        m_pPropSelUser->CenterWindow();
        m_pPropSelUser->ShowWindow(SW_SHOW);		
    }
    else
    {
        //���Լ�ʹ��
        /*
        CString stip;
        stip.Format("��ȷ��Ҫʹ�õ��� %s ��?",userProp->szPropName);
        if(userProp->attribAction & 0x2)
        stip+="\n�Ὣ���ĸ���������!";

        */
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

        UINT uRet = AFCMessageBox(stip,"��ʾ",AFC_YESNO,m_pEngine->GetCWnd());
        if(uRet==IDOK)
        {
            OnUseProp(propIndex,GetMeUserInfo()->dwUserID);
        }
        ////////////////////////////////////////////////////
    }

    return;
}

BOOL XLogic::OnUseProp(int nPropIndex, int nDeskUserID)
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

    // ���ؿ۳���Ӧ��������
    //if(userProp->nHoldCount <=0)
    //{
    //	Glb().userPropLibrary.RemoveAt(nPropIndex);
    //	delete userProp;
    //	userProp=NULL;
    //}
    return TRUE;
}

//�û�ʹ�ù㲥���Ի�����ȷ������������û�ID���û�����ֵ
void XLogic::OnUseBoardcast(WPARAM wparam, LPARAM lparam)
{
    _TAG_BOARDCAST *pMsg = (_TAG_BOARDCAST *)lparam;
    pMsg->dwUserID = this->GetMeUserInfo()->dwUserID;
    //	int nType = *((int*)((BYTE*)lparam + sizeof(_TAG_BOARDCAST)));
    _tcscpy_s(pMsg->szUserName, 32, this->GetMeUserInfo()->nickName);

    //���ٵ�������
    int propCount=Glb().userPropLibrary.GetCount();
    _TAG_USERPROP *userProp=NULL;
    for(int i=0;i<propCount;i++)
    {
        userProp=Glb().userPropLibrary.GetAt(i);
        if(userProp->nPropID==pMsg->nPropID)
        {
            userProp->nHoldCount-=1;
            if(userProp->nHoldCount < 0)userProp->nHoldCount = 0;
            //if(userProp->nHoldCount<1)    // PengJiLin, 2010-10-14, �µĵ���ϵͳ��������Ϊ0������
            //{
            //	Glb().userPropLibrary.RemoveAt(i);
            //	delete userProp;
            //	userProp=NULL;
            //}
            //��Ҫ�޸ı��ؽ��
            break;
        }
    }

    // �����lParam��һ��_TAG_BOARDCAST���渽��һ��int
    BYTE *buffer = (BYTE *)lparam;
    if (NULL != m_pIPC)
    {
        m_pIPC->SendData(IPC_MAIN_PROP, IPC_SUB_BROADCAST_BIG, buffer, wparam);
    }
    delete buffer;
    buffer = NULL;

    /*this->m_pGameRoom->PostMessage(WM_USE_BOARDCAST, wparam, lparam);*/
}

//���н��淢������Ϣ
void XLogic::OnBankMessage(UINT message,WPARAM wParam,LPARAM lParam)
{
    /// ���н��淢�����޸�����
    if (message == WM_CHANGE_PASSWD)
    {
        OnChangePasswd( (ChangePasswd_t *)(lParam) );
        return;
    }

    /// ���н��淢����ת�ʼ�¼����
    if (message == WM_TRANSFER_RECORD)
    {
        OnTransferRecord();
        return;
    }	/// ���н��淢����ת����Ϣ
    if (message == WM_TRANSFER_MONEY)
    {
        OnTransferMoney( (TransferMoney_t *)(lParam) );
        return;
    }

    /// ���н��淢���Ĵ�ȡǮ��Ϣ
    if (message == WM_CHECK_MONEY)
    {
        OnCheckMoney( (CheckMoney_t *)(lParam) );
        return;
    }

    // lxl, 2010-11-17, ���н��淢���Ļ�ȡ�û��ǳƵ���Ϣ
    if(WM_BANK_GET_NICKNAME_ONID == message)
    {
        OnGetNickNameOnID((GetNickNameOnID_t*)(lParam));
        return;
    }

	//�°����д�ȡ��
	if (WM_CHECK_MONEY2 == message)
	{
		OnCheckMoney2((bank_op_normal*)(lParam));
		return;
	}

	

	//JianGuankun 2012-1-2,�ر����д���ʱʹ��
	if (WM_BANK2_CLOSE == message)
	{
		OnCloseBank2();
		return;
	}

	//JianGuankun 2012.9.28 �յ����д�ȡ�ɹ�����Ϣ֪ͨ��Ϸ
	if (WM_BANK2_CHECK_SUC == message)
	{
		TUIMessage tagUIMsg;
		tagUIMsg._uMessageType = UI_BANK4GAME_CHECK;
		tagUIMsg._ch = (char)lParam;
		OnUIMessage(&tagUIMsg);
	}

}

//�ر�����
void XLogic::OnCloseBank2()
{
	if (m_pBank2Wnd)
	{	
		delete m_pBank2Wnd;
		m_pBank2Wnd = NULL;
	}

	return;
}


/// ����ȫ�ֱ����еļ��ܷ�ʽ��ȡ�ü��ܺ�������ַ���������sha������MD5
/// �����ﲻ��ָ��ĺϷ������ж�
/// @param szMD5Pass ���ܺ���ַ���
/// @param szSrcPass Դ�ַ���
/// @return ���ܺ���ַ���ָ��
TCHAR *XLogic::GetCryptedPasswd(TCHAR *szMD5Pass, TCHAR *szSrcPass)
{
    CenterServerMsg& par=Glb().m_CenterServerPara;
    //md5��ʽ
    if( par.m_nEncryptType == 1)
    {
        unsigned char szMDTemp[16];
        MD5_CTX Md5;
        Md5.MD5Update((unsigned char *)szSrcPass,lstrlen(szSrcPass));
        Md5.MD5Final(szMDTemp);
        for (int i = 0; i < 16; i ++) 
            wsprintf(&szMD5Pass[i * 2], "%02x", szMDTemp[i] );
    }
    //sha���ܷ�ʽ
    if(par.m_nEncryptType == 2)
    {
        unsigned szMDTemp[5]={0};
        CString strSha=szSrcPass;
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
        wsprintf(szMD5Pass,"%s",strSha);
    }
    return szMD5Pass;
}
/// ���н��淢��ָ�����ת�ʼ�¼
/// @return ��
void XLogic::OnTransferRecord()
{
    MSG_GR_S_TransferRecord_t transferRecord;
    transferRecord.dwUserID = GetMeUserInfo()->dwUserID;
    /// ֻ���û�ID�Ĵ���������Ϣ
    SendGameData(&transferRecord, sizeof(transferRecord), MDM_GR_MONEY, ASS_GR_TRANSFER_RECORD_INGAME, 0);
}
/// ���н��淢��ָ��޸�����
/// @param pChangePasswd �޸�����ṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
/// @return ��
void XLogic::OnChangePasswd( ChangePasswd_t *pChangePasswd )
{
    MSG_GR_S_ChangePasswd_t changePasswd;
    changePasswd.UserID = GetMeUserInfo()->dwUserID;
    GetCryptedPasswd(changePasswd.szMD5PassOld, pChangePasswd->szPwdOld);
    GetCryptedPasswd(changePasswd.szMD5PassNew, pChangePasswd->szPwdNew);

    SendGameData(&changePasswd, sizeof(changePasswd), MDM_GR_MONEY, ASS_GR_CHANGE_PASSWD_INGAME, 0);
    delete pChangePasswd;
    pChangePasswd = NULL;
}
/// ���н��淢��ָ�ת��
/// @param pTransferMoney ��ȡǮ�ṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
/// @return ��
void XLogic::OnTransferMoney( TransferMoney_t *pTransferMoney )
{
    CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");
    MSG_GR_S_TransferMoney transferMoney;
    transferMoney.i64Money = pTransferMoney->i64Money;
    transferMoney.destUserID = pTransferMoney->uDestUserID;
    transferMoney.UserID = GetMeUserInfo()->dwUserID;
    strcpy(transferMoney.szNickName,GetMeUserInfo()->nickName);		//add by lxl ת����ʾ��Ϣ��ʾ�ǳ�
    strcpy(transferMoney.szDestNickName, pTransferMoney->szDestNickName);
	transferMoney.bUseDestID = pTransferMoney->bUseDestID;
    if (transferMoney.UserID == transferMoney.destUserID)
    {
        delete pTransferMoney;
        pTransferMoney = NULL;
        CString str = fMsg.GetKeyVal("BankDlg","TransferToSelf","������ת�ʸ��Լ����Բ���ϵͳ�ݲ��ṩ�������");
        TCHAR szMsg[MAX_PATH];
        wsprintf(szMsg, str.GetBuffer());
        AFCMessageBox(szMsg);
        return;
    }
    GetCryptedPasswd(transferMoney.szMD5Pass, pTransferMoney->szPWD);

    SendGameData(&transferMoney,sizeof(transferMoney),MDM_GR_MONEY,ASS_GR_TRANSFER_MONEY_INGAME,0);
    delete pTransferMoney;
    pTransferMoney = NULL;
}
/// @param pCheckMoney ��ȡǮ�ṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
/// @return ��
void XLogic::OnCheckMoney( CheckMoney_t *pCheckMoney )
{
    CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");
    /// �ж��Ǵ滹��ȡ
    if (pCheckMoney->uType==0) /// ��Ǯ������Ҫ����
    {
        __int64 i64Left = GetMeUserInfo()->i64Money - pCheckMoney->i64Money;
        if (i64Left < 0 || i64Left < m_GameInfo.uLessPoint)
        {
            CString strBuffer;
            TCHAR szNum[128];
            GlbGetNumString(szNum, m_GameInfo.uLessPoint, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
            CString str = fMsg.GetKeyVal("GameRoom","NeedCoins","�˷�����Ҫ�������� %s �Ľ��");
            strBuffer.Format(str, szNum);
            AFCMessageBox(strBuffer,m_GameInfo.szGameName);
            /// �ǵ�ɾ��
            delete pCheckMoney;
            pCheckMoney = NULL;
            return;
        }
        MSG_GR_S_CheckMoney cm;
        cm.i64Count = pCheckMoney->i64Money;
        cm.UserID = GetMeUserInfo()->dwUserID;
        SendGameData(&cm,sizeof(cm),MDM_GR_MONEY,ASS_GR_CHECK_IN_INGAME,0);
    }
    else ///< ȡǮ����Ҫ����
    {
        MSG_GR_S_CheckMoneyWithPwd_t cm;
        cm.i64Count = pCheckMoney->i64Money;
        cm.UserID = GetMeUserInfo()->dwUserID;
        GetCryptedPasswd(cm.szMD5Pass, pCheckMoney->szPWD);
        SendGameData(&cm,sizeof(cm),MDM_GR_MONEY,ASS_GR_CHECK_OUT_INGAME,0);
    }
    /// �ǵ�ɾ��
    delete pCheckMoney;
    pCheckMoney = NULL;
}
/// ��2�����еĴ�ȡ��
/// @param pCheckMoney ��ȡǮ�ṹָ
/// @return ��
void XLogic::OnCheckMoney2(bank_op_normal* pCheckMoney )
{
	SendGameData(pCheckMoney,sizeof(bank_op_normal),MDM_BANK,ASS_BANK_NORMAL,0);
	delete pCheckMoney;
	pCheckMoney = NULL;
	return;
}

// lxl, 2010-11-17, �����û�ID��ȡ�ǳ�
void XLogic::OnGetNickNameOnID(GetNickNameOnID_t* pGetNickNameOnID)
{
    //OutputDebugString("������Ϣ��ExeForGame lxl");
    MSG_GR_S_GetNickNameOnID_t stGetNickNameOnID;
    stGetNickNameOnID.iUserID = pGetNickNameOnID->uUserID;

    SendGameData(&stGetNickNameOnID, sizeof(stGetNickNameOnID), MDM_GR_ROOM, ASS_GR_GET_NICKNAME_ONID_INGAME, 0);
    delete pGetNickNameOnID;
    pGetNickNameOnID = NULL;
}

void XLogic::OnBnClickedRightFrameHide()
{
	if(m_pRightFrame)
	{
		m_pRightFrame->ShowWindow(SW_HIDE);
		RECT rcShrink = {0,0,-m_pRightFrame->m_uOldWidth,0};
		m_pIUI->SetControlsShrinkArea(rcShrink);
	}
	return;
}

void XLogic::OnBnClickedRightFrameShow()
{
	if(m_pRightFrame)
	{
		int iWindowsWidth = GetSystemMetrics(SM_CXSCREEN);
		int iWindowsHeight = GetSystemMetrics(SM_CYSCREEN);
		int iRightFrameWidth = 0;

		WINDOWPLACEMENT place;
		GetWindowPlacement(m_pEngine->GetCWnd()->GetSafeHwnd(),&place);

		if (SW_SHOWMAXIMIZED == place.showCmd)
		{
			if (iWindowsWidth >= 1280)
			{
				iRightFrameWidth = m_pRightFrame->m_uMaxWidth - m_pRightFrame->m_uOldWidth;
			}
			else
			{
				iRightFrameWidth = m_pRightFrame->m_uMinWidth - m_pRightFrame->m_uOldWidth;
			}			
		}
		else
		{
			iRightFrameWidth = m_pRightFrame->m_uMinWidth - m_pRightFrame->m_uOldWidth;
		}

		RECT rcShrink = {0,0,iRightFrameWidth,0};
		m_pIUI->SetControlsShrinkArea(rcShrink);
		m_pRightFrame->ShowWindow(SW_SHOW);

	}
	return;
}

//������Ϣ
bool XLogic::OnPropMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
    switch(pNetHead->bAssistantID)
    {
        //wushuqun 2008.9.28
        //�޸�С����ʹ�ú�û�и��¿ͻ��˵��������ʾ 
    case ASS_PROP_SMALL_BOARDCASE:
        {
            // PengJiLin, 2010-10-13, �ڷ���ʹ��С���ȣ���Ϸ�˵�����û�иı䣬���������´���
            // boardCast->iLength ����Ϊ���䴫�ݹ�����������ֱ��ʹ��
            _TAG_BOARDCAST *boardCast=(_TAG_BOARDCAST*)pNetData;
            int propCount=Glb().userPropLibrary.GetCount();
            _TAG_USERPROP *userProp=NULL;
            for(int i=0;i<propCount;i++)
            {
                userProp=Glb().userPropLibrary.GetAt(i);
                if(userProp->nPropID==boardCast->nPropID)
                {
                    userProp->nHoldCount = boardCast->iLength;                        
                }
            }

            RefreshPropPannel();
            break;
        }
    case ASS_PROP_USEPROP:
        return OnUsePropResult(pNetHead,pNetData,uDataSize);
        break;
    case ASS_PROP_VIP_CHECKTIME:
        return OnPropVipCheckTime(pNetHead,pNetData,uDataSize);
        break;
    case ASS_PROP_NEW_KICKUSER:    // PengJiLin, 2010-9-14, ʹ�����˿����˽����Ϣ
        return OnNewKickUserResult(pNetHead,pNetData,uDataSize);
        break;
    }
    return true;
}


//���������Ϣ
bool XLogic::OnUsePropResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
    if(pNetHead->bHandleCode!=DTK_GR_PROP_USE_SUCCEED)
    {
        switch(pNetHead->bHandleCode)
        {
        case DTK_GP_PROP_GIVE_NOTARGETUSER:
            //ShowMsgInGame("Ŀ����Ҳ���������״̬��");
            break;
        default:
            break;
        }
        return true;
    }
    if(sizeof(_TAG_USINGPROP)!= uDataSize)
        return false;
    _TAG_USINGPROP * usingProp = (_TAG_USINGPROP *)pNetData;
    UserItemStruct *pUserInfo =FindOnLineUser(usingProp->dwTargetUserID);
    UserItemStruct *pUserInfoUsed =FindOnLineUser(usingProp->dwUserID);
    if(pUserInfo == NULL || pUserInfoUsed == NULL)
        return true;
    // ������Լ�ʹ�õĵ��ߣ�Ҫ�۳���������
    if (usingProp->dwUserID == GetMeUserInfo()->dwUserID)
    {
        _TAG_USERPROP *userProp = NULL;
        for (int i=0; i<Glb().userPropLibrary.GetCount(); ++i)
        {
            userProp=Glb().userPropLibrary.GetAt(i);
            if ((userProp!=NULL) && (userProp->nPropID == usingProp->nPropID))
            {
                break;
            }
        }
        if((userProp!=NULL) && (userProp->nHoldCount<=0))
        {
            userProp->nHoldCount-=1;
            if(userProp->nHoldCount < 0)userProp->nHoldCount = 0;
        }
    }

    //ʹ���ߵ��ǳ�
    CString name1=pUserInfoUsed->GameUserInfo.nickName;// rewrite by ramon
    //��ʹ���ߵ��ǳ�
    CString name2=pUserInfo->GameUserInfo.nickName;	// rewrite by ramon

    if(usingProp->dwUserID==m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)
        name1="��";
    if(usingProp->dwTargetUserID==m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)
        name2="��";
    CString name3=name2;
    if(usingProp->dwUserID==usingProp->dwTargetUserID)
        name2="�Լ�";

    int attribAction=usingProp->nPropActionAttrib;
    int attribValue=usingProp->nPropValueAttrib;

    CString stip;
    stip.Format("%s �� %s ʹ���� %s ����",name1,name2,usingProp->szPropName);
    InsertSystemMessage(stip.GetBuffer());


	CString _s = "B";
	if (usingProp->dwTargetUserID==m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)
	{
		_s = "B";
	}
	if (usingProp->dwUserID==m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)
	{
		_s = "A";
	}
	if (_s != "" && pUserInfo->GameUserInfo.bDeskNO == m_GameInfo.uisMeUserInfo.GameUserInfo.bDeskNO)
	{
		CString soundFile;
		soundFile.Format("..\\image\\prop\\Prop%d%s.mp3", usingProp->nPropID, _s);
		PlaySound(soundFile.GetBuffer());
	}


    //˫������
    if(attribAction & 0x4)
    {
        pUserInfo->GameUserInfo.iDoublePointTime=usingProp->iDoubleTime;
        if(usingProp->dwTargetUserID==m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)
        {
            time_t t=(time_t)usingProp->iDoubleTime;
            CTime expiresTime(t);
            stip.Format(TEXT("����˫�����ֹ��ܿ���ʹ�õ� %s��"),expiresTime.Format("%Y-%m-%d %H:%M:%S"));
            InsertSystemMessage(stip.GetBuffer());
        }
    }
    if(attribAction & 0x8)
    {
        pUserInfo->GameUserInfo.iProtectTime=usingProp->iProtectTime;
        if(usingProp->dwUserID == m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)
        {
            time_t t=(time_t)usingProp->iProtectTime;
            CTime expiresTime(t);
            stip.Format(TEXT("���Ļ��������(���˲��ۻ���)����ʹ�õ� %s��"),expiresTime.Format("%Y-%m-%d %H:%M:%S"));
            InsertSystemMessage(stip.GetBuffer());
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
        InsertSystemMessage(stip.GetBuffer());
    }
    if(attribAction & 0x2)
    {
        CString str;
        str.Format("������0���߽��:%d ������0",usingProp->dwTargetUserID);
        //OutputDebugString(str);
        if(usingProp->dwTargetUserID==m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)
        {
            if(m_GameInfo.uisMeUserInfo.GameUserInfo.dwPoint<0)
                m_GameInfo.uisMeUserInfo.GameUserInfo.dwPoint=0;
        }
        if (pUserInfoUsed->GameUserInfo.dwPoint<0)
        {
            pUserInfoUsed->GameUserInfo.dwPoint = 0;
        }
    }

	if (m_pUserListDu)
	{
		m_pUserListDu->UpdateGameUser(pUserInfoUsed);
	}

	if (m_pRightFrame)
	{
		m_pRightFrame->ShowUserInfo();
	}

    //��������б�
    RefreshPlayList();

    return true;
}

//vipʱ����
bool XLogic::OnPropVipCheckTime(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
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
        InsertSystemMessage(strMessage.GetBuffer());
    }
    else
    {
        if(GetMeUserInfo()->iVipTime <=curtime)
        {
            //��������
            CString strMessage;
            strMessage.Format(TEXT("����Vip����Ѿ��������������¹���"));
            InsertSystemMessage(strMessage.GetBuffer());
        }
    }
    //OutputDebugString("OnPropVipCheckTime");
    RefreshPropPannel();
    return true;
}

// PengJiLin, 2010-9-14, ʹ�����˿����˽����Ϣ
bool XLogic::OnNewKickUserResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	if(uDataSize!=sizeof(_TAG_NEW_KICKUSER)) return false;

	_TAG_NEW_KICKUSER* pKickResult = (_TAG_NEW_KICKUSER*)pNetData;
     if(pKickResult != NULL && pKickResult->dwUserID == m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)   // �Լ��߱���
    {
        switch(pNetHead->bHandleCode)
        {
        case ERR_GR_NEW_KICKUSER_SUCCESS:       // �ɹ�
            {
                UserItemStruct* pUserInfo = FindOnLineUser(pKickResult->dwDestID);                
                CString strInfo = "";
                if(NULL != pUserInfo) strInfo.Format("���%s�ѱ��߳���Ϸ��", pUserInfo->GameUserInfo.nickName);
                else strInfo.Format("���%d�ѱ��߳���Ϸ��", pKickResult->dwDestID);

				InsertSystemMessage(strInfo.GetBuffer());
                //ShowMessageDialog(strInfo.GetBuffer(), 0);
				//if(IDOK ==AFCMessageBox(strInfo.GetBuffer()))
				//{
				//	return true;
				//}
                //ShowNewKickPropDlg(TRUE, FALSE, TRUE, strInfo);
            }
            break;
        case ERR_GR_NEW_KICKUSER_NOTIME:        // �Լ��ĵ����ѹ��ڣ�����û�е���
            {
                CString strInfo = "";
                strInfo.Format("��û��\"���˿�\"���ߣ������Ѿ�����");

				InsertSystemMessage(strInfo.GetBuffer());
                //ShowMessageDialog(strInfo.GetBuffer(), 0);
				//if(IDOK ==AFCMessageBox(strInfo.GetBuffer()))
				//{
				//	return true;
				//}
                //ShowNewKickPropDlg(TRUE, TRUE, TRUE, strInfo);
            }
            break;
        case ERR_GR_NEW_KICKUSER_HAD_ANTI:      // �Է��з��߿�ʱ��
            {
                CString strInfo = "�����ʹ����\"���߿�\"�������ܽ����߳���Ϸ��";

				InsertSystemMessage(strInfo.GetBuffer());
                //strInfo.Format();
                //ShowMessageDialog(strInfo.GetBuffer(), 0);
				//if(IDOK ==AFCMessageBox(strInfo.GetBuffer()))
				//{
				//	return true;
				//}
                //ShowNewKickPropDlg(TRUE, FALSE, TRUE, strInfo);
            }
            break;
        case ERR_GR_NEW_KICKUSER_HAD_VIP:       // �Է���VIPʱ��
            {
                CString strInfo = "";
                strInfo.Format("�������VIP��Ա�������ܽ����߳���Ϸ��");

				InsertSystemMessage(strInfo.GetBuffer());
                //ShowMessageDialog(strInfo.GetBuffer(), 0);
				//if(IDOK ==AFCMessageBox(strInfo.GetBuffer()))
				//{
				//	return true;
				//}
                //ShowNewKickPropDlg(TRUE, FALSE, TRUE, strInfo);
            }
            break;
        case ERR_GR_NEW_KICKUSER_PLAYING:       // ��Ϸ�в�����
            InsertSystemMessage("����ʧ�ܣ��Է�������Ϸ");
            break;
        default:
            break;
        }
    }

    return true;
}

void XLogic::UseAntiKickProp(_TAG_USERPROP* userProp)
{
    InsertSystemMessage(TEXT("������ʹ�á����߿������ߡ�"));    // test

    MSG_GR_RS_KickProp KickPro;
    ZeroMemory(&KickPro,sizeof(MSG_GR_RS_KickProp));
    KickPro.dwUserID = GetMeUserInfo()->dwUserID;
    KickPro.iPropID = userProp->nPropID;

    SendGameData(&KickPro, sizeof(KickPro), MDM_GM_GAME_FRAME, ASS_GM_USE_ANTI_KICK_PROP, 0);

    return;
}

void XLogic::UseKickUserProp(_TAG_USERPROP* userProp)
{
    InsertSystemMessage(TEXT("������ʹ�á����˿������ߡ�"));    // test

    MSG_GR_RS_KickProp KickPro;
    ZeroMemory(&KickPro,sizeof(MSG_GR_RS_KickProp));
    KickPro.dwUserID = GetMeUserInfo()->dwUserID;
    KickPro.iPropID = userProp->nPropID;

    SendGameData(&KickPro, sizeof(KickPro), MDM_GM_GAME_FRAME, ASS_GM_USE_KICK_PROP, 0);

    return;
}

//�û�ʹ�ù㲥���Ի�����ȷ������������û�ID���û�����ֵ
void XLogic::UseBoardCast(_TAG_USERPROP * userProp)
{
    //if(userProp->attribAction & 0x80)   // ������
    //    ShowInputDlg(1);
    //else                                // С����
    //    ShowInputDlg(2);

    m_dlgBroadcast->m_nPropID=userProp->nPropID;

    if(userProp->attribAction & 0x80)
        m_dlgBroadcast->SetValue(BRD_MSG_BIG);
    else
        m_dlgBroadcast->SetValue(BRD_MSG_SMALL);
    AfxSetResourceHandle(GetModuleHandle(NULL));
    m_dlgBroadcast->ShowWindow(SW_SHOW);
}

///������С���������
int XLogic::ShowInputDlg(int type)
{
    IContain* _InputDlg;
    GETCTRL(IContain, _InputDlg, m_pIUI, CTN_MsgInput);
    if (NULL==_InputDlg)
    {
        return 0;
    }

    IText* _Tip;
    GETCTRL(IText, _Tip, m_pIUI, CTN_MsgInput_Tip);
    if (NULL==_Tip)
    {
        return 0;
    }

    IEditorText* _Edit;
    GETCTRL(IEditorText, _Edit, m_pIUI, CTN_MsgInput_Edit);
    if (NULL==_Edit)
    {
        return 0;
    }

    if (1==type)
    {
        _Tip->SetText(L"���������Ʒ���127������,254��Ӣ��");
        _Edit->SetMaxTypeIn(254);
    }
    else if (2==type)
    {
        _Tip->SetText(L"С�������Ʒ���60������,120��Ӣ��");
        _Edit->SetMaxTypeIn(120);
    }
    else if (0==type)
    {
        _InputDlg->SetControlVisible(false);
        return 0;
    }
    _InputDlg->SetControlVisible(true);

    return 0;
}


///�����Ի��� funΪ�ص���������Ϊһ��boolֵ��ȷʵ��ȡ����ֵ
void XLogic::ShowMessageDialog(TCHAR* szMsg, int itype)
{
    IContain* _Contain = dynamic_cast<IContain*>(m_pIUI->GetIControlbyID(CTN_DIALOG));
    if (_Contain != NULL)
    {
        IText *_Text;
        _Text = dynamic_cast<IText*>(m_pIUI->GetIControlbyID(TX_DLALOG_MSG));
        if (_Text != NULL)
        {
            _Text->SetText(CT2W(szMsg));
        }

        IButton* _b1 = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_DLALOG_OK));
        IButton* _b2 = dynamic_cast<IButton*>(m_pIUI->GetIControlbyID(BN_DLALOG_CANCEL));

        if (_b1 != NULL && _b2 != NULL)
        {
            if (itype == 0)             // OK
            {
                _b1->SetControlVisible(true);
                _b2->SetControlVisible(false);
                _b1->SetGameXY(_Contain->GetGameX() + (_Contain->GetGameW() - _b1->GetGameW()) / 2, _b1->GetGameY());
            }
            else if (itype == 1)        // ȷ�� ȡ��
            {
                _b1->SetControlVisible(true);
                _b2->SetControlVisible(true);
                int _w = (_Contain->GetGameW() - _b1->GetGameW() - _b2->GetGameW()) / 3;
                _b1->SetGameXY(_Contain->GetGameX() + _w, _b1->GetGameY());
                _b2->SetGameXY(_Contain->GetGameX() + 2 * _w + _b1->GetGameW(), _b2->GetGameY());
            }
            else if (itype == 2)        // �ް�ť
            {
                _b1->SetControlVisible(false);
                _b2->SetControlVisible(false);
            }
            else if (itype == 3)        // ����ʾ
            {
                _Contain->SetControlVisible(false);
                return;
            }
        }

        _Contain->SetControlVisible(true);
    }
}


// PengJiLin, 2010-9-10, ���˿������߿�ʹ�ý��
bool XLogic::OnUseKickPropResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, BOOL bIsKick)
{
    MSG_GR_RS_KickProp* pKickProp = (MSG_GR_RS_KickProp*)pNetData;

    // ���ҵ��ߣ�ͬʱ���ٵ�������
    int propCount=Glb().userPropLibrary.GetCount();
    _TAG_USERPROP * userProp = NULL;
    int iPropValue = 0;
    for(int i = 0; i < propCount; ++i)
    {
        userProp = Glb().userPropLibrary.GetAt(i);
        if(userProp->nPropID == pKickProp->iPropID)
        {
            iPropValue = userProp->attribValue & 0xFFFFFF;
            userProp->nHoldCount -= 1;
            if(userProp->nHoldCount < 0)userProp->nHoldCount = 0;
            //if(userProp->nHoldCount < 1)  // PengJiLin, 2010-10-14, �µĵ���ϵͳ��������Ϊ0������
            //{
            //    Glb().userPropLibrary.RemoveAt(i);
            //    delete userProp;
            //    userProp=NULL;
            //}
            break;
        }
        userProp = NULL;
    }

    if(TRUE == bIsKick)
    {
        CString strValue = "";
        SecondFormat(iPropValue, strValue);

        CString strTotalValue = "";
        SecondFormat(pKickProp->iTotalTime, strTotalValue);

        CString strInfo = ""; 
        strInfo.Format("���ѳɹ�ʹ�á����˿������ߣ�����������Ϸ�Ҳ��б����Ҽ����Ŀ����ҽ����߳���Ϸ�����ߵ�����Чʱ��%s����ʱ��%s", 
            strValue, strTotalValue);

        ShowMessageDialog(strInfo.GetBuffer(), 0);
        //ShowNewKickPropDlg(TRUE, FALSE, TRUE, strInfo);
    }
    else
    {
        CString strValue = "";
        SecondFormat(iPropValue, strValue);

        CString strTotalValue = "";
        SecondFormat(pKickProp->iTotalTime, strTotalValue);

        CString strInfo = ""; 
        strInfo.Format("���ѳɹ�ʹ�á����߿������ߣ��������\n�޷������߳���Ϸ�����ߵ�����Ч\nʱ��%s����ʱ��%s", 
            strValue, strTotalValue);

        ShowMessageDialog(strInfo.GetBuffer(), 0);
        //ShowNewKickPropDlg(TRUE, FALSE, FALSE, strInfo);
    }

    // ˢ��
    RefreshPropPannel();

    return true;
}

// PengJiLin, 2010-9-10, ��������ʽ��Ϊʱ����ĸ�ʽ
void XLogic::SecondFormat(int iSecond, CString& strOutput)
{
    if(iSecond < 0) iSecond = 0;

    int iSec = 0;
    int iMin = 0;
    int iHour = 0;

    iSec = iSecond%60;
    iMin = iSecond/60;

    iHour = iMin/60;
    iMin = iMin%60;

    if(iHour > 0)
    {
        strOutput.Format("%dСʱ%d��%d��", iHour, iMin, iSec);
    }
    else if(iMin > 0)
    {
        strOutput.Format("%d��%d��", iMin, iSec);
    }
    else
    {
        strOutput.Format("%d��", iSec);
    }
}


/// �������е���Ϣ
/// ��ͻ��˵�GameRoomEx�д�ͬС�죬ֻ�������ؽ����������������ݵı仯
/// @param[in] pNetHead Э��ͷ
/// @param[in] pNetData ���ݰ�
/// @param[in] uDataSize ���ݰ���С
/// @return ����ɹ������򷵻�true������false��ʾ���������Ϣʧ�ܣ�����һ�ֿ�������ΪЭ�鲻ƥ�䣬�����޸���ͷ�ļ�ȴû�ж����±���
/// 
bool XLogic::OnAboutMoney(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
    CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");

    /// ������Э��ID�Ĳ�ͬ�����ݳ���Ҳ������ͬ�������ÿ��case�������ж�
    /// ��Ҫ�������Ϣ�У������ڷ����������������к�Ǯ�����仯����Ϣ
    switch (pNetHead->bAssistantID)
    {
    case ASS_GR_TRANSFER_RECORD_INGAME:	///< ת�ʼ�¼
        {
            //Ч������
            if(uDataSize!=sizeof(MSG_GR_R_TransferRecord_t))
            {
                return false;
            }
            MSG_GR_R_TransferRecord_t *pRecord = (MSG_GR_R_TransferRecord_t *)pNetData;
            m_pPersonBank->OnReceivedTransferRecord( pRecord );
            break;
        }
    case ASS_GR_CHANGE_PASSWD_INGAME:	///< �޸�����
        {
            /// ֻ��ҪbHandelCode�Ϳ�����
            CString strBuffer;
            CString str;
            if (pNetHead->bHandleCode == ASS_GR_OPEN_WALLET_SUC)
            {
                str = fMsg.GetKeyVal("GameRoom","ChangePasswdOK","�޸���������ɹ��������Ʊ��������롣");
            }
            else
            {
                str = fMsg.GetKeyVal("GameRoom","ChangePasswdFailed","�޸���������ʧ�ܣ�");
            }
            strBuffer.Format(str);
            AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
            break;
        }
    case ASS_GR_PAY_MONEY:
        {
            switch(pNetHead->bHandleCode)
            {
            case 1://�ɹ�
                {
                    break; // �Ѿ��ɴ����������������һ��Э�鴫���˸ı����ݣ�������ﲻ���ظ�����
                }
            case 0: //���ɹ�
                {
                    CString strBuffer;
                    strBuffer.Format(TEXT("���Ľ�Ҷ���˴�֧��!"));
                    AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
                    break;
                }
            }
            break;
        }
    case ASS_GR_OPEN_WALLET_INGAME:
	case ASS_GR_OPEN_WALLET:
        {
            switch (pNetHead->bHandleCode)
            {
            case ASS_GR_OPEN_WALLET_ERROR:
                {
                    CString strBuffer;
                    strBuffer.Format(TEXT("��Ǯ��ʧ��,���Ժ�����!"));
                    AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
                    break;
                }
            case ASS_GR_OPEN_WALLET_ERROR_PSW:
                {
                    CString strBuffer;
                    strBuffer.Format(TEXT("�����������,������!"));
                    AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
                    break;
                }
            case ASS_GR_OPEN_WALLET_SUC:
                {
                    DebugPrintf("������");
                    //Ч������
                    MSG_GR_UserBank * pBank=(MSG_GR_UserBank *)pNetData;
                    CString strBuffer;
                    if(uDataSize!=sizeof(MSG_GR_UserBank)) 
                    {
                        CString str;
                        str = fMsg.GetKeyVal("GameRoom","ErrorInGettingMoney","��Ǯ��ʧ��,���Ժ�����!");
                        strBuffer.Format(str);
                        AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
                    }

					/*if (pBank->nVer == 1) //�ϰ�����
					{
						AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
						//��������
						if (m_pPersonBank == NULL) 
						{
							try
							{
								m_pPersonBank=new CPBankDlg(m_pEngine->GetCWnd());
								if (m_pPersonBank==NULL)
									return false;
								m_pPersonBank->m_pLogic = this;
							}
							catch (...) { return false; }
						}
						if (m_pPersonBank!=NULL)
						{
							DebugPrintf("��Ϸ�и����г�Ա��ֵdwBank=[%d]dwMoney=[%d]---%d��", pBank->i64Bank, m_GameInfo.uisMeUserInfo.GameUserInfo.i64Money
								, __LINE__);
							/// �ȸ�ֵ
							m_pPersonBank->SetWealthInfor(pBank->i64Bank, m_GameInfo.uisMeUserInfo.GameUserInfo.i64Money);
							if (m_pPersonBank->GetSafeHwnd()==NULL)
							{
								AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
								/// �����еĳ�Ա��ֵ
								DebugPrintf("��Ϸ��׼�������н���[%d]---%d��", m_pPersonBank->GetSafeHwnd()
									, __LINE__);
								m_pPersonBank->DoModal();//Create(IDD_DIALOGBANK,this);
								DebugPrintf("��Ϸ�����н����˳�[%d]---%d��", m_pPersonBank->GetSafeHwnd()
									, __LINE__);
							}
						}
						AfxSetResourceHandle(GetModuleHandle(NULL));
					}
					else if (pBank->nVer == 2) //�°�����*/
					{
						if (!m_pBank2Wnd)
						{
							//�ǰټ�����Ϸ���ô���Ϸ����
// 							TCHAR szKey[10];
// 							CString sPath=CBcfFile::GetAppPath();
// 							CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
// 							wsprintf(szKey, "%d", m_GameInfo.uNameID);
// 							int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);
// 
// 							if (iResult == 0)
// 							{
// 								return false;
// 							}

							m_pBank2Wnd = new CPBank4GameWnd(m_pEngine->GetCWnd(),&m_GameInfo);
							m_pBank2Wnd->Create(m_pEngine->GetCWnd()->m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
							m_pBank2Wnd->SetBankVersion(pBank->nVer);
							m_pBank2Wnd->SetWealthInfor(pBank->i64Bank,GetMeUserInfo()->i64Money);
							m_pBank2Wnd->CenterWindow();
							strcpy(m_pBank2Wnd->m_szTwoPassword,pBank->szTwoPassword);
							m_pBank2Wnd->ShowWindow(SW_SHOW);
						}
					}
                    
                    break;
                }
            }
            break;
        }
    case ASS_GR_CHECK_OUT://ȡǮ
    case ASS_GR_CHECK_OUT_INGAME://��Ϸ��ȡǮ
        {
            if(pNetHead->bHandleCode==ASS_GR_CHECKOUT_ERROR)
            {
                if (ASS_GR_CHECK_OUT_INGAME == pNetHead->bAssistantID)
                {
                    CString strBuffer;
                    CString str;
                    str = fMsg.GetKeyVal("GameRoom","NetWorkBusy","ȡ��ʧ�ܣ�����ԭ�������벻��ȷ�����㣡");
                    strBuffer.Format(str);
					DUIMessageBox(NULL,MB_OK|MB_ICONERROR,m_GameInfo.szGameName,false,strBuffer.GetBuffer());
                }
            }

            if(pNetHead->bHandleCode==ASS_GR_CHECKOUT_ERROR_PLAYING)
            {
                if (ASS_GR_CHECK_OUT_INGAME == pNetHead->bAssistantID)
                {
                    CString strBuffer;
                    CString str;
                    str = fMsg.GetKeyVal("GameRoom","NoDrawForPlaying","��������Ϸ�У�����ȡ�");
                    strBuffer.Format(str);
                   DUIMessageBox(NULL,MB_OK|MB_ICONINFORMATION,m_GameInfo.szGameName,false,strBuffer.GetBuffer());
                }
            }

            if(pNetHead->bHandleCode==ASS_GR_CHECKOUT_SUC)
            {
                //Ч������
                if (uDataSize!=sizeof(MSG_GR_CheckMoney)) return false;
                MSG_GR_CheckMoney * pCheckOut=(MSG_GR_CheckMoney *)pNetData;
                if(pCheckOut->dwUserID==m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)//�Լ�
                {
                    //���¶Ի���
                    if (m_pPersonBank!=NULL) 
                    {
                        if (m_pPersonBank->GetSafeHwnd()!=NULL) 
                        {
                            m_pPersonBank->UpdateUserMoney(0, pCheckOut->i64CheckOut);
                        }
                    }
                }
                //���½����Ϣ
                FlushUserMoney(pCheckOut->dwUserID,pCheckOut->i64CheckOut);

                m_pEngine->GetCWnd()->PostMessage(WM_BANK2_CHECK_SUC,0,1);
            }
            break;
        }
    case ASS_GR_CHECK_IN://��Ǯ
    case ASS_GR_CHECK_IN_INGAME://��Ϸ�д�Ǯ
        {
            if(pNetHead->bHandleCode==ASS_GR_CHECKIN_ERROR)
            {
                if (ASS_GR_CHECK_IN_INGAME == pNetHead->bAssistantID)
                {
                    CString strBuffer;
                    CString str;
                    str = fMsg.GetKeyVal("GameRoom","NetWorkBusySave","����ӵ��,����ʧ��!���Ժ�����!");
                    strBuffer.Format(str);
                    DUIMessageBox(NULL,MB_OK|MB_ICONINFORMATION,m_GameInfo.szGameName,false,strBuffer.GetBuffer());
                }
            }
            if(pNetHead->bHandleCode==ASS_GR_CHECKIN_ERROR_PLAYING)
            {
                if (ASS_GR_CHECK_IN_INGAME == pNetHead->bAssistantID)
                {
                    CString strBuffer;
                    CString str;
                    str = fMsg.GetKeyVal("GameRoom","NoSaveForPlaying","��������Ϸ�У����ܴ�");
                    strBuffer.Format(str);
					DUIMessageBox(NULL,MB_OK|MB_ICONINFORMATION,m_GameInfo.szGameName,false,strBuffer.GetBuffer());
                }
            }

            if(pNetHead->bHandleCode==ASS_GR_CHECKIN_ZHUANGJIA)  //�ټ����� ��ׯ�в��ܴ�� add by lxl 2010-11-5
            {
                if (ASS_GR_CHECK_IN_INGAME == pNetHead->bAssistantID)
                {
                    CString strBuffer;
                    CString str;
                    str = fMsg.GetKeyVal("GameRoom","NoSaveForZhuangjia","��������ׯ�У����ܴ�");
                    strBuffer.Format(str);
                    DUIMessageBox(NULL,MB_OK|MB_ICONINFORMATION,m_GameInfo.szGameName,false,strBuffer.GetBuffer());
                }
            }

            if(pNetHead->bHandleCode==ASS_GR_CHECKIN_SUC)
            {
                //Ч������
                if (uDataSize!=sizeof(MSG_GR_CheckMoney)) return false;
                MSG_GR_CheckMoney * pCheckIn=(MSG_GR_CheckMoney *)pNetData;

                if(pCheckIn->dwUserID==m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)//�Լ�
                {
                    //���¶Ի���
                    if (m_pPersonBank!=NULL) 
                    {
                        m_pPersonBank->UpdateUserMoney(1, pCheckIn->i64CheckOut);
                    }
                }
                FlushUserMoney(pCheckIn->dwUserID,0-pCheckIn->i64CheckOut);

                m_pEngine->GetCWnd()->PostMessage(WM_BANK2_CHECK_SUC,0,0);
            }

            break;
        }
    case ASS_GR_TRANSFER_MONEY:		   //ת��
    case ASS_GR_TRANSFER_MONEY_INGAME: //��Ϸ��ת��
        {
            CString strBuffer;
            CString str;
            TCHAR szNum[128], szBank[128];
            //Ч������
            if (uDataSize!=sizeof(MSG_GR_S_TransferMoney)) return false;
            MSG_GR_S_TransferMoney * pTransfer=(MSG_GR_S_TransferMoney *)pNetData;
            /// ���ת�ʲ��ɹ���������ƽ̨������ת��ָ��򵯳��Ի���������Ϸ�з���ָ���ƽֻ̨��Ӧ�Ի����¼�
            if (ASS_GR_TRANSFER_MONEY_INGAME == pNetHead->bAssistantID)
            {
                switch(pNetHead->bHandleCode)
                {
                case ASS_GR_TRANSFER_SUC:	///< ת�ʳɹ�					
                    {
                        /////Added by xqm 20101109
                        /////��Z����������ת�ʳɹ�����Ϣ,Ȼ����Z��������ת��Ŀ��ͻ��˷���һ��ת�ʹ�������Ϣ
                        //OutputDebugString("��Ϸ����Z������");
                        //SendGameData(pNetData, uDataSize, MDM_GR_MONEY, ASS_GR_TRANSFER_MONEY,ASS_GR_TRANSFER_SUC);	
                        //SendGameData(pNetData, uDataSize, MDM_GR_MONEY, ASS_GR_TRANSFER_MONEY,ASS_GR_TRANSFER_SUC);	
                        ///SendGameData((void *)pNetData, uDataSize, pNetHead->bMainID, ASS_GR_TRANSFER_SUC, 0);
                        //SendGameData(pNetData, uDataSize, MDM_GR_MONEY, ASS_GR_TRANSFER_MONEY,ASS_GR_TRANSFER_SUC);
                        break;
                    }
                case ASS_GR_PASSWORD_ERROR:												///< ת���������
                    {
                        str = fMsg.GetKeyVal("GameRoom","TransferPassWdError","�����������û��ͨ����֤���������������룡");
                        strBuffer.Format(str, szNum);
                        AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
                        return true;
                    }
                case ASS_GR_TRANSFER_TOTAL_LESS:										///< �����ܶ�̫С�������ʸ�
                    {
                        GlbGetNumString(szNum, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
                        str = fMsg.GetKeyVal("GameRoom","TransferTotalLess","����ʱ��������ת�ʣ����д��ﵽ�򳬹�%s��Ҳſ���ת�ʣ�");
                        strBuffer.Format(str, szNum);
                        AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
                        return true;
                    }
                    break;
                case ASS_GR_TRANSFER_TOO_LESS:											///< ����ת����Ŀ̫��
                    {
                        GlbGetNumString(szNum, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
                        str = fMsg.GetKeyVal("GameRoom","TransferTooLess","ת�ʲ��ɹ�������ת�ʽ��������%s��ҡ�");
                        strBuffer.Format(str, szNum);
                        AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
                        return true;
                    }
                    break;
                case ASS_GR_TRANSFER_MULTIPLE:											///< ����ת����Ŀ������ĳ���ı���
                    {
                        GlbGetNumString(szNum, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
                        str = fMsg.GetKeyVal("GameRoom","TransferMultiple","ת�ʲ��ɹ���ת�ʽ�������%s����������");
                        strBuffer.Format(str, szNum);
                        AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
                        return true;
                    }
                case ASS_GR_TRANSFER_NOT_ENOUGH:										///< ���н�������ת��
                    {
                        GlbGetNumString(szNum, pTransfer->i64Money, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
                        GlbGetNumString(szBank, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
                        str = fMsg.GetKeyVal("GameRoom","TransferNotEnoughMoney","ת�ʲ��ɹ�����������ֻ��%s��ң�����֧������ת��%s��ҡ�");
                        strBuffer.Format(str, szBank, szNum);
                        AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
                        return true;
                    }
                case ASS_GR_TRANSFER_NO_DEST:											///< ת��Ŀ�겻����
                    {
                        str = fMsg.GetKeyVal("GameRoom","AccountNotExist","ת��Ŀ�겻����,���ʵ�ʺ�����!");
                        strBuffer.Format(str);
                        AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
                        return true;
                    }
                    break;
                default:
                    {
                        CString strBuffer;
                        CString str;
                        str = fMsg.GetKeyVal("GameRoom","NetWorkBusySave","����ӵ��,����ʧ��!���Ժ�����!");
                        strBuffer.Format(str);
                        AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
                        return true;
                    }
                }
            }
            /// ת�ʳɹ������ô�����Һ����н�����
            if(pNetHead->bHandleCode==ASS_GR_TRANSFER_SUC)
            {
                TCHAR szAct[128];
                CString strBuffer;
                CString str;
                GlbGetNumString(szNum, pTransfer->i64Money, Glb().m_nPowerOfGold/*, false*/, Glb().m_bUseSpace, Glb().m_strSpaceChar);
                GlbGetNumString(szAct, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold/*, false*/, Glb().m_bUseSpace, Glb().m_strSpaceChar);
                /// ���Լ�ת������
                if(pTransfer->UserID == m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)//�Լ�
                {
                    if (ASS_GR_TRANSFER_MONEY_INGAME == pNetHead->bAssistantID)
                    {
                        //��ʾ�û�
						if(pTransfer->bUseDestID)
						{
							str = fMsg.GetKeyVal("GameRoom","TransferCoinsId","��ת��ID[%d]����� %s ��ң��۳������Ѻ�ʵ�ʵ��� %s ���!");
							strBuffer.Format(str, pTransfer->destUserID, szNum, szAct);
						}
						else
						{
							str = fMsg.GetKeyVal("GameRoom","TransferCoinsNn","��ת���ǳ�[%s]����� %s ��ң��۳������Ѻ�ʵ�ʵ��� %s ���!");
							strBuffer.Format(str, pTransfer->szDestNickName, szNum, szAct);
						}
                        AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
                    }
                    //���¶Ի���
                    if (m_pPersonBank!=NULL) 
                    {
                        m_pPersonBank->UpdateUserMoneyByTransfer(0, pTransfer->i64Money);
                    }
                }
                /// ת��Ŀ�����Լ�
                else if (pTransfer->destUserID==m_GameInfo.uisMeUserInfo.GameUserInfo.dwUserID)
                {
                    if (ASS_GR_TRANSFER_MONEY_INGAME == pNetHead->bAssistantID)
                    {
                        str = fMsg.GetKeyVal("GameRoom","BeTransfered","���[%d]ת���� %s ��ң��۳������Ѻ�ʵ�ʵ��� %s ���!");
                        strBuffer.Format(str, pTransfer->UserID, szNum, szAct);
                        AFCMessageBox(strBuffer,m_GameInfo.szGameName);//,MB_OK,this);
                    }
                    //���¶Ի���
                    if (m_pPersonBank!=NULL) 
                    {
                        m_pPersonBank->UpdateUserMoneyByTransfer(1, pTransfer->i64ActualTransfer);
                    }
                }
            }
            break;
        }
    default:
        break;
    }
    return true;
}

/// ˢ���û��б��еĽ��
/// @param dwUserID �û�ID��
/// @param uCount ��Ϊ��������ֵ����˼Ϊ�ӻ��
bool XLogic::FlushUserMoney(long dwUserID,__int64 i64Count)
{
    UserItemStruct * pUserItem=FindOnLineUser(dwUserID);
    if(pUserItem==NULL)
        return false;

    //��������
    pUserItem->GameUserInfo.i64Money += i64Count;
    pUserItem->GameUserInfo.i64Bank -= i64Count;//���н��

    //������Ϣ
    ShowOneUserInfo(DeskStation2View(pUserItem->GameUserInfo.bDeskStation), pUserItem, true);

	if (m_pUserListDu)
	{
		m_pUserListDu->UpdateGameUser(pUserItem);
	}

	if (m_pRightFrame)
	{
		m_pRightFrame->ShowUserInfo();
	}

    //������λ���û�
    RefreshPlayList();
    return true;
}



// ���������б�
void XLogic::LoadBkSoundList(void)
{
    if (!m_pBackAudio)
    {
        return;
    }
    TCHAR sz[61],szPath[MAX_PATH];
    lstrcpy(sz,m_GameInfo.szProcessName);
    sz[lstrlen(sz)-lstrlen(".ico")]='\0';
    wsprintf(szPath,"GameOption.bcf");
    //AfxMessageBox(szPath);
    //DebugPrintf("szPath = [%s]", szPath);
    CBcfFile f(szPath);
    int		soundCount=f.GetKeyVal("BGSOUND","SoundCount",0);
    //DebugPrintf("SoundCount = [%d]", soundCount);
    if(soundCount==0)
        return;

    CString soundFile;
    for(int i=0;i<soundCount;i++)
    {
        wsprintf(sz,"Sound%d",i);
        soundFile=f.GetKeyVal("BGSOUND",sz,"");
        wsprintf(szPath,"%s",soundFile);
        //DebugPrintf("szPath = [%s]", szPath);
        if (m_pBackAudio)
        m_pBackAudio->Add(szPath);
    }
    int nVol=f.GetKeyVal("BGSOUND","SoundVolume",300);
    if (m_pBackAudio)
          m_pBackAudio->SetVolume(nVol*1.0f/1000.0f*100.0f);    
    m_bSndPause=f.GetKeyVal("BGSOUND","PauseSound",0);
    if (m_pBackAudio)
    {
        m_pBackAudio->Play();
        m_pBackAudio->SetLoopPlay(true);
    }

    if(m_bSndPause)
    {
        if (m_pBackAudio)
            m_pBackAudio->Pause();
        IButton *pSoundOn = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_VOLUM_ON));
        IButton *pSoundOFF = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_VOLUM_OFF));
        if (pSoundOn && pSoundOFF)
        {
            pSoundOn->SetControlVisible(true);
            pSoundOFF->SetControlVisible(false);
        }
    }
    else
    {
        if (m_pBackAudio)
        m_pBackAudio->Continue();
        IButton *pSoundOn = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_VOLUM_ON));
        IButton *pSoundOFF = dynamic_cast<IButton *>(m_pIUI->GetIControlbyID(BN_VOLUM_OFF));
        if (pSoundOn && pSoundOFF)
        {
            pSoundOn->SetControlVisible(false);
            pSoundOFF->SetControlVisible(true);
        }
    }
}


void XLogic::OnPropBuyVIP(UINT message,WPARAM wParam,LPARAM lParam)
{
	BYTE *buffer = (BYTE *)lParam;
	if (NULL != m_pIPC)
	{
		m_pIPC->SendData(IPC_PROP_BUY_VIP, IPC_ASS_BUY_VIP, buffer, wParam);
		_TAG_PROP_BUY_VIP *pPropBuy = (_TAG_PROP_BUY_VIP *)lParam;
	}
	// �Ӵ��ڷ�������new������ָ�룬��ʱɾ��
	delete buffer;
	buffer = NULL;
}
bool XLogic::UserProp(_TAG_PROP_MOIVE_USE *propMoive)
{
	int attribAction=propMoive->nPropActionAttrib;
	int attribValue=propMoive->nPropValueAttrib;


	if(attribAction & 0x40)
	{
		int iValue=(attribValue & 0x7F000000)>>24;
		m_stProp.iValue=iValue;
		m_stProp.sendDeskStation=propMoive->nUserStation;
		m_stProp.recvDeskStation=propMoive->nTargetUserStation;

		CString strFilePathName; 
		strFilePathName.Format("..\\image\\prop\\PropGif%dA.bani",iValue%1000); //modify by lxl  ͼƬ��ʽ�޸�gif->spr

		PlayProp(true,strFilePathName);
		}
		//
		//SetTimer(ID_GIF_TIMER,150,NULL);	
		////{��Ӷ����������� zxj 2009-9-17
		//CString soundFile;
		//soundFile.Format("..\\image\\prop\\PropGif%dA.wav",m_stProp.iValue%1000);
		//PlaySound(soundFile.GetBuffer());
		//��ӽ���}
	
	return true;
}

void XLogic::DecodeString(CString str, CString &strColumn, int &width)
{
	int pos;
	CString strTemp;

	strTemp = str;
	pos = strTemp.Find(',');
	if (pos >= 0)
	{
		strColumn = strTemp.Left(pos);

		strTemp = strTemp.Right(strTemp.GetLength()-pos-1);
		if (!strTemp.IsEmpty())
		{
			width = atoi(strTemp);
		}
	}	
}
int XLogic::GetMoneyGrade(__int64 dwPoint)
{
	CString s = CBcfFile::GetAppPath();
	CBcfFile f("..\\Order.bcf");
	CString m_key;
	m_key.Format("PlatMoneyOrderName%d", m_GameInfo.uNameID);
	static int szOrderNum[50];
	CString strKey,strTmp;
	int tmpNum = 0;
	int nCount = 0;
	for (int i = 1;i <=50;i++)
	{
		strKey.Format("platOrder_%d",i);
		tmpNum=f.GetKeyVal(m_key,strKey,0);
		szOrderNum[i-1] = tmpNum;
		strTmp = f.GetKeyMemo(m_key,strKey,"");
		if (strTmp == "")
		{
			nCount = i-1;
			break;
		}
	}
	for (int i=0;i<nCount;i++)	
		if (dwPoint<szOrderNum[i]) 
			return i;
	return nCount;
}

void XLogic::OnMessageProc(UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message) {
		case WM_MOVE:
			{
				if (m_pPmdWnd)
				{
					m_pPmdWnd->AdjustPos((HWND)m_pIUI->GetWindowsHwnd());
					//m_pPmdWnd->ShowCtrl();
				}
			}
			break;
		case UM_FIXRIGHTFRAME:
			{
				if (m_pRightFrame)
				{
					CRect rcRight;
					CRect rcMain;
					::GetWindowRect(m_pRightFrame->GetHWND(),&rcRight);
					::GetWindowRect(m_pEngine->GetCWnd()->GetSafeHwnd(),rcMain);

					ISudoku* pSoduku = dynamic_cast<ISudoku*>(m_pIUI->GetIControlbyID(1));
					if(pSoduku)
					{
						RECT rcTop = pSoduku->GetSrcRect(2);
						RECT rcRight = pSoduku->GetSrcRect(1);
						RECT rcBottom = pSoduku->GetSrcRect(7);
						
						int iWindowsWidth = GetSystemMetrics(SM_CXSCREEN);
						int iWindowsHeight = GetSystemMetrics(SM_CYSCREEN);
						if (wParam == SW_SHOWMAXIMIZED)
						{
							if (iWindowsWidth >= 1280)
							{
								::MoveWindow(m_pRightFrame->GetHWND(),rcMain.right - m_pRightFrame->m_uMaxWidth - (rcRight.right - rcRight.left),
									rcMain.top + (rcTop.bottom - rcTop.top),m_pRightFrame->m_uMaxWidth,
									rcMain.Height() - (rcTop.bottom - rcTop.top) - (rcBottom.bottom - rcBottom.top),
									true);
							}
							else
							{
								::MoveWindow(m_pRightFrame->GetHWND(),rcMain.right - m_pRightFrame->m_uMinWidth - (rcRight.right - rcRight.left),
									rcMain.top + (rcTop.bottom - rcTop.top),m_pRightFrame->m_uMinWidth,
									rcMain.Height() - (rcTop.bottom - rcTop.top) - (rcBottom.bottom - rcBottom.top),
									true);
							}
						}
						else if (wParam == SW_HIDE || wParam == SW_SHOWNORMAL)
						{
							::MoveWindow(m_pRightFrame->GetHWND(),rcMain.right - m_pRightFrame->m_uMinWidth - (rcRight.right - rcRight.left),
								rcMain.top + (rcTop.bottom - rcTop.top),m_pRightFrame->m_uMinWidth,
								rcMain.Height() - (rcTop.bottom - rcTop.top) - (rcBottom.bottom - rcBottom.top),
								true);
						}
					}
				}
			}
			break;
	}
}

/// �����Ҽ��˵���Ӧ
int XLogic::OnTalkMenuMessage(TUIMessage *pMessage)
{	
	IMenu *pMenu;
	GETCTRL(IMenu,pMenu,m_pIUI,TALK_MENU);	

	int iSel = pMenu->GetSelectIndex();
	if (iSel >= 0)
	{
		if (!wcscmp(pMenu->GetItemText(iSel),L"  �����Ļ"))
		{		
			ITalk* pTalk;
			GETCTRL(ITalk, pTalk, m_pIUI, TALK_SHOW);
			pTalk->ClearText();
			m_TalkMessages.clear();

		}
		else if (!wcscmp(pMenu->GetItemText(iSel),L"  ����"))
		{
			ITalk* pTalk;
			GETCTRL(ITalk, pTalk, m_pIUI, TALK_SHOW);
			pTalk->CopyAllText();
		}
		else if (!wcscmp(pMenu->GetItemText(iSel),L"  �������"))
		{
			ITalk* pTalk;
			GETCTRL(ITalk, pTalk, m_pIUI, TALK_SHOW);
			pTalk->SetIsAutoScroll(true);
			m_bIsAutoScroll = true;
		}
		else if (!wcscmp(pMenu->GetItemText(iSel),L"  ����������Ϣ"))
		{
			m_bIsAcceptChatMsg = true;
		}
		else if (!wcscmp(pMenu->GetItemText(iSel),L"�� �������"))
		{
			ITalk* pTalk;
			GETCTRL(ITalk, pTalk, m_pIUI, TALK_SHOW);
			pTalk->SetIsAutoScroll(false);
			m_bIsAutoScroll = false;
		}
		else if (!wcscmp(pMenu->GetItemText(iSel),L"�� ����������Ϣ"))
		{
			m_bIsAcceptChatMsg = false;
		}
	}

	return 0;
}


//�鿴�û���Ϣ
BOOL XLogic::OnCmdShowUserInfo()
{
	if (m_dwCurrentUserID != 0L)
	{
		UserItemStruct* pUserInfo = FindOnLineUser(m_dwCurrentUserID);

		m_pUserDetailsDlg->SetUserItem(pUserInfo);

		POINT point;
		::GetCursorPos(&point);

		CRect rect;
		m_pUserDetailsDlg->GetClientRect(&rect);

		if (m_pRightFrame)
		{
			RECT rcRightWnd;
			::GetWindowRect(m_pRightFrame->GetHWND(),&rcRightWnd);
			m_pUserDetailsDlg->MoveWindow(rcRightWnd.left - rect.Width(), point.y - 20, rect.Width(), rect.Height());

			::SetWindowPos(m_pUserDetailsDlg->GetSafeHwnd(),CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			
		}
// 		else
// 		{
// 			CRect listRect;
// 			m_UserListDlg.GetWindowRect(&listRect);
// 			m_pUserDetailsDlg->MoveWindow(listRect.left - rect.Width() + 15, point.y - 10, rect.Width(), rect.Height());
// 		}

		m_pUserDetailsDlg->ShowWindow(SW_SHOW);
		
	}

	return TRUE;
}

//�����û���Ϣ
BOOL XLogic::OnCmdCopyUserInfo()
{
	if (m_dwCurrentUserID != 0)
	{
		UserItemStruct* pUserItem = FindOnLineUser(m_dwCurrentUserID);

		HWND hwnd = m_pEngine->GetCWnd()->GetSafeHwnd();

		//��������
		if ((pUserItem==NULL)||(::OpenClipboard(hwnd)==FALSE))
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

		SetClipboardData(CF_UNICODETEXT,hData);
		GlobalUnlock(hData); 
		GlobalFree(hData);
		CloseClipboard();
	}

	return TRUE; 
}

//����
BOOL XLogic::OnCmdKickUser()
{
	if (m_dwCurrentUserID!=0L)
	{
		//wushuqun 2009.5.19
		//�Թ�ʱ����T��
		if (GetMeUserInfo()->bUserState == USER_WATCH_GAME)
		{
			AFCMessageBox("�Բ����������Թ���Ϸ�����ܽ��д˲���", "��ʾ");
			return true;
		}

		if(GetMeUserInfo()->dwUserID == m_dwCurrentUserID)
		{
			AFCMessageBox("�Բ���,�޷����Լ����д������","��ʾ");
			return true;
		}

		CString stime;
		stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
		int curtime=atoi(stime);                        //ת��Ϊint
		if(GetMeUserInfo()->iVipTime < curtime)
		{
			AFCMessageBox("�Բ���,ֻ��vip���д����","��ʾ");
			return true;
		}
		///����Ƿ�Ϊ�Թ� add by rmaon 09.04.27
		if( m_bWatchMode )
		{
			AFCMessageBox("�Բ���,�Թ�״̬��������","��ʾ");
			return true;
		}
		//��������
		UserItemStruct * pUserItem=FindOnLineUser(m_dwCurrentUserID);
		if (pUserItem==NULL) 
			return TRUE;
		if(pUserItem->GameUserInfo.iVipTime>curtime)
		{
			AFCMessageBox("�Բ���,�Է���Vip,����ʧ��!","��ʾ");
			return TRUE;
		}

		// PengJiLin, 2010-9-10, �������Ƿ������˿������˹���
		CString sPath=CBcfFile::GetAppPath();
		CBcfFile bcfFile( sPath + "..\\Order.bcf");
		int iValue = bcfFile.GetKeyVal(_T("ClassInExe"), _T("UseKickUserProp"), 0);
		if(iValue > 0)      // ��������˿������߿����ܣ�ʹ���µ����˹���
		{
			_TAG_NEW_KICKUSER stKick;
			stKick.dwUserID = GetMeUserInfo()->dwUserID;
			stKick.dwDestID = m_dwCurrentUserID;
			SendGameData(&stKick,sizeof(stKick),MDM_GR_PROP,ASS_PROP_NEW_KICKUSER,0);
		}
		else
		{
			_TAG_KICKUSER stKick;
			stKick.dwUserID = GetMeUserInfo()->dwUserID;
			stKick.dwDestID = m_dwCurrentUserID;
			SendGameData(&stKick,sizeof(stKick),MDM_GR_PROP,ASS_PROP_KICKUSER,0);
		}
	}

	return TRUE;
}

//����
BOOL XLogic::OnCmdKickUserProp()
{
	if (m_dwCurrentUserID!=0L)
	{
		// �Թ�ʱ����T��
		if (GetMeUserInfo()->bUserState == USER_WATCH_GAME)
		{
			AFCMessageBox("�Բ����������Թ���Ϸ�����ܽ��д˲���", "��ʾ");
			return true;
		}
		// ����Ƿ�Ϊ�Թ�
		if( m_bWatchMode )
		{
			AFCMessageBox("�Բ���,�Թ�״̬��������","��ʾ");
			return true;
		}
		if(GetMeUserInfo()->dwUserID == m_dwCurrentUserID)
		{
			AFCMessageBox("�Բ���,�޷����Լ����д������","��ʾ");
			return true;
		}

		CString stime;
		stime.Format("%d",CTime::GetCurrentTime());         // ��ǰ��ʱ�䣬String��ʽ
		int curtime=atoi(stime);                            // ת��Ϊint
		UserItemStruct * pUserItem=FindOnLineUser(m_dwCurrentUserID);
		if (pUserItem==NULL) 
		{
			AFCMessageBox("�Բ���,�Է�״̬�쳣,����ʧ��!","��ʾ");
			return TRUE;
		}
		if(pUserItem->GameUserInfo.iVipTime>curtime)
		{
			AFCMessageBox("�Բ���,�Է�Ҳ��Vip,����ʧ��!","��ʾ");
			return TRUE;
		}

		// �������ݷ������ȽϷŵ��洢������ʵ��
		_TAG_NEW_KICKUSER stKick;
		stKick.dwUserID = GetMeUserInfo()->dwUserID;
		stKick.dwDestID = m_dwCurrentUserID;
		SendGameData(&stKick,sizeof(stKick),MDM_GR_PROP,ASS_PROP_NEW_KICKUSER,0);
	}

	return TRUE;
}


//��Ӻ���
BOOL XLogic::OnAddFriend()
{
	return m_pIPC->SendData(IPC_FRIEND, IPC_ASS_CHECKFRIEND, &m_dwCurrentUserID, sizeof(int));
}

//��Ӻ�����
BOOL XLogic::OnAddEmeny()
{
	return m_pIPC->SendData(IPC_EMENY, IPC_ASS_CHECKEMENY, &m_dwCurrentUserID, sizeof(int));
}