// IMMain.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "IMMain.h"
#include "MainRoomEx.h"
#include "IMSetUser.h"


#ifdef  MAINROOM_USE_SHORTMSG
int		CIMMain::m_iMaxShortWnd=10;					//��ര��
#endif

// CIMMain �Ի���

IMPLEMENT_DYNAMIC(CIMMain, CDialog)

CIMMain::CIMMain()
:htiFamilier(NULL)
,htiUnfamilier(NULL)
,htiBlack(NULL)
{
	bInitialed=false;


	InitialIMList();
	
}

CIMMain::~CIMMain()
{
#ifndef  MAINROOM_USE_SHORTMSG
	clearAllSession();
#else
	for (INT_PTR i=0;i<m_ShortMessage.GetCount();i++) 
	{
		delete ((CShortMessage *)m_ShortMessage.GetAt(i));
		m_ShortMessage.SetAt(i,NULL);
	}
#endif
}


BEGIN_MESSAGE_MAP(CIMMain, CTreeCtrl)

	//{{AFX_MSG_MAP(CTreeCtrlEx) 
	ON_WM_PAINT() 
	//}}AFX_MSG_MAP 
	ON_MESSAGE(WM_EXCHANGE_SKIN,OnExchangeSkin)
END_MESSAGE_MAP()


/*
Function		:InitialIMList
Author			:Fred Huang 2008-4-2
Notes			:��ʼIM����
*/

bool CIMMain::InitialIMList(void)
{

	return true;
}

//����
LRESULT CIMMain::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
#ifndef  MAINROOM_USE_SHORTMSG
	CIMSession *imSession=NULL;
	OutputDebugString("*rende*CIMMain::OnExchangeSkin");
	POSITION pos= imSessionList.GetHeadPosition();
	//��Ҫ�Ӷ������ҵ��Ự����
	while(pos)
	{
		imSession=imSessionList.GetNext(pos);
		if(imSession != NULL)
		{
			//����
			imSession->PostMessage(WM_EXCHANGE_SKIN,wpara,0);
		}
	}
#endif
	Invalidate();
	return LRESULT(0);
}

//��ʼ������,ͬʱ���Ӹ��ӵ�
bool CIMMain::Init()
{
	//��ʼ��List Image
	if(m_imgList.m_hImageList)
		m_imgList.DeleteImageList();
	UINT uIconID[]=
	{
		//�̶�ͼ��
		IDI_FRIEND,
		IDI_UNKNOWN,
		IDI_BLACK,
		IDI_PLAYER
	};
	m_imgList.Create(16,16,ILC_COLOR16,sizeof(uIconID)/sizeof(uIconID[0]),sizeof(uIconID)/sizeof(uIconID[0]));
	SetBkColor(RGB(255,255,255));
	for (UINT i=0;i<sizeof(uIconID)/sizeof(uIconID[0]);i++)
	{
		HICON hIcon=AfxGetApp()->LoadIcon(uIconID[i]);
		m_imgList.Add(hIcon);
		DestroyIcon(hIcon);
	}
	SetImageList(&m_imgList, TVSIL_NORMAL);

	htiFamilier=InsertItem("�ҵĺ���",0,0,TVI_ROOT);
	htiUnfamilier=InsertItem("İ����",1,1,TVI_ROOT);
	htiBlack=InsertItem("������",2,2,TVI_ROOT);
	SetItemData(htiFamilier,1);
	SetItemData(htiUnfamilier,2);
	SetItemData(htiBlack,3);

	Expand(htiFamilier,TVE_EXPAND);
	Expand(htiUnfamilier,TVE_EXPAND);
	Expand(htiBlack,TVE_EXPAND);

	return true;
}

//���ؼ���ɫ����
void   CIMMain::SetItemFont(HTREEITEM   hItem,   LOGFONT&   logfont) 
{ 
	Color_Font   cf; 
	if(   !m_mapColorFont.Lookup(   hItem,   cf   )   ) 
		cf.color   =   (COLORREF)-1; 
	cf.logfont   =   logfont; 
	m_mapColorFont[hItem]   =   cf; 
} 

////////////////////////////////////////////////////////////////////// 
void   CIMMain::SetItemBold(HTREEITEM   hItem,   BOOL   bBold) 
{ 
	SetItemState(hItem,   bBold   ?   TVIS_BOLD:   0,   TVIS_BOLD); 
} 

////////////////////////////////////////////////////////////////////// 
void   CIMMain::SetItemColor(HTREEITEM   hItem,   COLORREF   color) 
{ 
	Color_Font   cf; 
	if(!m_mapColorFont.Lookup(hItem,   cf)) 
		cf.logfont.lfFaceName[0]   =   '\0 '; 
	cf.color   =   color; 
	m_mapColorFont[hItem]   =   cf; 
} 

////////////////////////////////////////////////////////////////////// 
BOOL   CIMMain::GetItemFont(HTREEITEM   hItem,   LOGFONT   *   plogfont) 
{ 
	Color_Font   cf; 
	if(!m_mapColorFont.Lookup(hItem,   cf)) 
		return   FALSE; 
	if(cf.logfont.lfFaceName[0]   ==   '\0 ')   
		return   FALSE; 
	*plogfont   =   cf.logfont; 
	return   TRUE; 

} 

////////////////////////////////////////////////////////////////////// 
BOOL   CIMMain::GetItemBold(HTREEITEM   hItem) 
{ 
	return   GetItemState(hItem,   TVIS_BOLD)   &   TVIS_BOLD; 
} 

////////////////////////////////////////////////////////////////////// 
COLORREF   CIMMain::GetItemColor(HTREEITEM   hItem) 
{ 
	//   Returns   (COLORREF)-1   if   color   was   not   set 
	Color_Font   cf; 
	if(!m_mapColorFont.Lookup(hItem,   cf)) 
		return   (COLORREF)   -   1; 
	return   cf.color; 

}

#ifndef  MAINROOM_USE_SHORTMSG
/*
Function		:openIMSession
Author			:Fred Huang 2008-4-2
Notes			:��һ���Ự���ڣ��ȴ��Ѿ������Ĵ������ң����û���򴴽�һ��
Parameter		:
dwUserID		:�û���ID��
szName			:�û�����
*/
CIMSession* CIMMain::openIMSession(int dwUserID,CString szName)
{
	CIMSession *imSession=NULL;
	try
	{
		POSITION pos=imSessionList.GetHeadPosition();
		//��Ҫ�Ӷ������ҵ��Ự����
		while(pos)
		{
			imSession=imSessionList.GetNext(pos);
			if(imSession->m_dwUserId==dwUserID)
			{
				imSession->FlashWindow(TRUE);
				return imSession;
			}
		}
		//�������û�У��򴴽�һ��
		imSession=new CIMSession(this);
		imSession->Create(IDD_IM_SESSION,GetDesktopWindow());
		imSession->setUser(dwUserID,szName,GetMainRoom(),true);
		//���öԻ������еĸ����ھ��Ϊ�����ڣ����ڷ���Ϣ
		imSession->setWindowOwner(this->m_hWnd);
		imSession->CenterWindow();
		//���뵽������
		imSessionList.AddTail(imSession);
		imSession->FlashWindow(TRUE);	
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}

	return imSession;
}
#endif

/*
Function	:OnNetMessage
Notes		:������Ϣ
Author		:Fred Huang
Date		:2008-04-2
Parameter	:
NetMessageHead	:������Ϣͷ
void *			:����ָ��
UINT			:���ݰ��Ĵ�С
return		:void
*/
void CIMMain::OnNetMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	if(!bInitialed)
		return;
	try
	{
		switch(pNetHead->bAssistantID)
		{
		case ASS_IMS_GETLIST:
			OnNetGetUserList(pNetData);
			break;
		case ASS_IMS_GETUSERINFO:
			OnNetGetUserInfo(pNetData);
			break;
		case ASS_IMS_MSG:
			OnNetGetMsg(pNetData);
			break;
		case ASS_IMS_ADDREQUEST://���������Ϊ����
			OnNetAddRequest(pNetData);
			break;
		case ASS_IMS_ADDREQUESTRESPONSE://�з�Ӧ��
			OnNetAddResponse(pNetData);
			break;
		case ASS_IMC_ADDREQUEST:
			OnNetAddRetrue(pNetHead, pNetData);
			break;
		case ASS_SET_FRIENDTYPE:
			{
				int _t = *((int*)pNetData);
				GetMainRoom()->m_PlaceUserInfo.iAddFriendType = _t;
				break;
			}
		}
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}

	return;
}

void CIMMain::OnNetAddRetrue(NetMessageHead * pNetHead, void * pData)
{
	if (pNetHead->bHandleCode == 1)
	{
		//AFCMessageBox("���û��ܾ��κ��˼���Ϊ���ѻ����û����ڲ�����!");
	     DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"���û��ܾ��κ��˼���Ϊ���ѻ����û����ڲ�����!");
	}
	else if (pNetHead->bHandleCode == 2)
	{
		//AFCMessageBox("���û��ܾ��κ��˼���Ϊ���ѻ����û����ڲ�����!");//��Ϊ���˲���Z��ᵼ�¼ӻ�����Ϊ����ʱ����ʾ�����ߣ����Խ���ʾ�ĳ�ͳһzht2012-10-10
	     DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"���û��ܾ��κ��˼���Ϊ���ѻ����û����ڲ�����!");
	}
}

/*
Function	:checkUserCount
Notes		:��鱾�غ����������������������жԱ�
Author		:Fred Huang
Date		:2008-04-2
Parameter	:void
return		:void
*/
void CIMMain::checkUserCount(void)
{
	if(!bInitialed)
	{
		if(InitialIMList())
			bInitialed=true;
	}
	else
		return;

	//CMainRoomEx *pWnd=(CMainRoomEx*)pParentWnd;
	try
	{
		//���ṹ��
		MSG_IM_C_COUNT msg;
		ZeroMemory(&msg,sizeof(MSG_IM_C_COUNT));
		msg.dwUserID=GetMainRoom()->m_PlaceUserInfo.dwUserID;
		msg.dwUserCount=0;//atoi((char*)_bstr_t(vt));
		msg.dwUserCountReturn=0;

		//����ѯ�ʺ���
		GetMainRoom()->SendData(&msg,sizeof(msg),MDM_GP_IM,ASS_IMC_GETCOUNT,0);
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}

	return;	
}


/*
Function	:OnNetGetUserList
Notes		:�յ����������صĺ����б�
Author		:Fred Huang
Date		:2008-04-2
Parameter	:pData-���������ص�����
return		:void
*/
void CIMMain::OnNetGetUserList(void* pData)
{
	try
	{
		MSG_IM_S_USERLIST *msg=(MSG_IM_S_USERLIST*)pData;
		MSG_IM_S_GETUSERINFO *userInfo=(MSG_IM_S_GETUSERINFO*)msg->sBody;
		for(int i=0;i<msg->nListCount;i++)
		{
			//ȡÿһ���˵�����
			addUserToList(userInfo->dwUserID,userInfo->sUserName,userInfo->nSex,userInfo->GroupID,userInfo->nOnline);
			userInfo++;//sizeof(MSG_IM_S_GETUSERINFO);//(MSG_IM_S_GETUSERINFO*)msg->sBody+p;
		}
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}
}
/*
Function	:addUserToList
Notes		:����û�������
Author		:Fred Huang
Date		:2008-04-2
Parameter	:
dwUserID	:�û�ID
szUserName	:�û����������������ݿ�����Nickname
sex			:�Ա���-1��Ů-0��
GroupID		:��ID��1-���ѣ�2-İ���ˣ�3-������
onLine		:�Ƿ�����
return		:void
*/
void CIMMain::addUserToList(int dwUserID, CString szUsername, int sex, int GroupID, int nOnline)
{
	try
	{

		//���Sex������Χ��ȡ1
		sex=(sex==0)?0:1;
		//���online������Χ��ȡ1
		nOnline=(nOnline==0)?0:1;

		//�����ĺ��볬����Χ��ȡİ����
		HTREEITEM hRoot=htiUnfamilier;
		if(GroupID==1)
			hRoot=htiFamilier;
		if(GroupID==3)
			hRoot=htiBlack;

		//�ҳ��û����������е�λ��
		HTREEITEM htiUser=FindUserITEM(dwUserID+1000);
		if(htiUser)
		{
			HTREEITEM htiParent = GetParentItem(htiUser);
			DeleteItem(htiUser);
			if(ItemHasChildren(htiParent) == FALSE)
			{
				SetItemImage(htiParent, 0, 0);
			}
		}

		//ȡ����ʾ��ͼ������
		int nImage = 3;
// 		if(sex==0)
// 		{
// 			if(nOnline==0)
// 				nImage=6;
// 			else
// 				nImage=5;
// 		}
// 		else
// 		{
// 			if(nOnline==0)
// 				nImage=4;
// 			else
// 				nImage=3;
// 		}
		//��������ߣ�����ʾ�����棬������ʾ������
		if(nOnline)
			htiUser=InsertItem(szUsername,nImage,nImage,hRoot,TVI_FIRST);
		else
			htiUser=InsertItem(szUsername,nImage,nImage,hRoot,TVI_LAST);
		SetItemData(htiUser,dwUserID+1000);
		Expand(GetParentItem(htiUser),TVE_EXPAND);
		//SetItemImage(GetParentItem(htiUser),1,1);

#ifndef  MAINROOM_USE_SHORTMSG
		POSITION pos=imSessionList.GetHeadPosition();
		CIMSession * session=NULL;
		while(pos)
		{
			session=imSessionList.GetNext(pos);
			if(session->m_dwUserId==dwUserID)
			{
				session->setUserOnline(nOnline==0?false:true);
				break;
			}
		}
#endif

	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}
}

/*
Function	:FindUserITEM
Notes		:�ҵ��û������е�λ�ã���Ҫ�������нڵ�
Author		:Fred Huang
Date		:2008-04-2
Parameter	:
dwUserID	:�û�ID
return		:
HTREEITEM	:�ڵ�
*/
HTREEITEM CIMMain::FindUserITEM(int dwUserID)
{
	try
	{
		HTREEITEM rootHti,hti;
		for(rootHti=GetChildItem(TVI_ROOT);rootHti!=NULL;rootHti=GetNextSiblingItem(rootHti))
		{
			for(hti=GetChildItem(rootHti);hti!=NULL;hti=GetNextSiblingItem(hti))
			{
				if(GetItemData(hti)==dwUserID)
					return hti;
			}
		}
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}
	return NULL;
}


HTREEITEM CIMMain::FindUserITEM(CString szName)
{
	try
	{
		HTREEITEM rootHti,hti;
		for(rootHti=GetChildItem(TVI_ROOT);rootHti!=NULL;rootHti=GetNextSiblingItem(rootHti))
		{
			for(hti=GetChildItem(rootHti);hti!=NULL;hti=GetNextSiblingItem(hti))
			{
				if(GetItemText(hti)== szName)
					return hti;
			}
		}
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}
	return NULL;
}

/*
Function	:OnNetGetUserInfo
Notes		:�յ����������صĺ�����Ϣ
Author		:Fred Huang
Date		:2008-04-7
Parameter	:pData-���������ص�����
return		:void
*/
void CIMMain::OnNetGetUserInfo(void * pData)
{
	MSG_IM_S_GETUSERINFO *userInfo=(MSG_IM_S_GETUSERINFO*)pData;
	addUserToList(userInfo->dwUserID,userInfo->sUserName,userInfo->nSex,userInfo->GroupID,userInfo->nOnline);
}
/*
Function	:OnNetGetMsg
Notes		:�յ�������ת���ĺ�����Ϣ
Author		:Fred Huang
Date		:2008-04-7
Parameter	:pData-���������ص�����
return		:void
*/
void CIMMain::OnNetGetMsg(void * pData)
{

	try
	{
		MSG_IM_S_MSG *msg=(MSG_IM_S_MSG*)pData;
		//�ҵ��û������е�λ��
		HTREEITEM hti=FindUserITEM(msg->dwUserID+1000);

		int charType = GetMainRoom()->m_PlaceUserInfo.iAddFriendType / 65535;
		
		if (charType == 1)
		{
			return;
		}
		if (charType == 2 && GetParentItem(hti) != htiFamilier)
		{
			return;
		}

		if(!hti)
		{//���û�У���ӵ�İ������
			hti=InsertItem(msg->szUserName,2,2,htiUnfamilier);
			SetItemData(hti,msg->dwUserID+1000);
		}
		else
		{//����ں������У�ֱ�ӷ��أ�����Ӧ
			if(GetParentItem(hti)==htiBlack)
				return;
		}
		//�򿪶Ի���
		//������Ϣ����
#ifndef  MAINROOM_USE_SHORTMSG
		CIMSession * session=openIMSession(msg->dwUserID,msg->szUserName);		if(!session)return;
		if(!session->m_hWnd)return;
		session->ShowWindow(SW_SHOW);
		session->setUserOnline(true);
		//�����Ϣ
		session->OnGetMessage(msg);
#else
		CShortMessage * pShortMessageWnd=NULL;
		if (msg->dwUserID == GetMainRoom()->m_PlaceUserInfo.dwUserID)
		{
			//�Լ����͵���Ϣ
			pShortMessageWnd=openShortMsg(msg->dwRemoteUserID,msg->szUserName);
			if (pShortMessageWnd!=NULL) 
			{
				pShortMessageWnd->OnRecvMessage(msg->szUserName,msg->szMsg,RGB(0, 0, 255));
			}
		}
		else	//������Ϸ�߷�����Ϣ	
		{
			pShortMessageWnd = openShortMsg(msg->dwUserID,msg->szUserName);
			if (pShortMessageWnd!=NULL)
			{
				pShortMessageWnd->OnRecvMessage(msg->szUserName,msg->szMsg,RGB(255, 0, 0));
			}

		}
#endif
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}
}

/*
Function	:SetUser
Notes		:�������û�Ҫ����Ӻ���
Author		:Fred Huang
Date		:2008-04-7
Parameter	:
dwUser		:�û�ID
szName		:�û���
return		:void
*/
void CIMMain::SetUser(int dwUser,CString szName,bool isBlacklist)
{
	try
	{
		HTREEITEM hti;
		if (dwUser == -1 && szName != "")
			hti = FindUserITEM(szName);
		else 
			hti = FindUserITEM(dwUser+1000);

		//�Ѿ��Ǻ�����
		if(GetParentItem(hti) == htiFamilier)
		{
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

			AFCMessageBox(fMsg.GetKeyVal("IMDlg", \
				"SorryThisGuyIsYourFriend",\
				"�Բ��𣬸��û��Ѿ������ĺ��ѣ�����Ҫ����ӣ�"),\
				fMsg.GetKeyVal("IMDlg","Tip","��ʾ"));

			return;
		}

		if(szName.GetLength()==0)
		{
			szName = GetItemText(hti);
		}

		//�����������
		CIMSetUser imsu;
		imsu.dwUser=dwUser;
		imsu.szName=szName;
		if(isBlacklist == false && imsu.DoModal()==IDCANCEL)
			return;
		MSG_IM_C_ADDREQUEST msg;
		ZeroMemory(&msg,sizeof(MSG_IM_C_ADDREQUEST));

		msg.dwUserID=GetMainRoom()->m_PlaceUserInfo.dwUserID;
		msg.dwRequestedUserID=dwUser;
		memcpy(&msg.sUserName,GetMainRoom()->m_PlaceUserInfo.nickName,sizeof(GetMainRoom()->m_PlaceUserInfo.nickName));
		memcpy(&msg.sRequestedUserName,szName.GetBuffer(), szName.GetLength());
		
		memcpy(&msg.sRequestNotes,imsu.imWords,imsu.imWords.GetLength());
		msg.nMsgLength=imsu.imWords.GetLength();
		msg.cbSize=sizeof(MSG_IM_C_ADDREQUEST)-sizeof(msg.sRequestNotes)+msg.nMsgLength+1;//ʵ�ʳ���

		if (dwUser > -1)
			GetMainRoom()->SendData(&msg,msg.cbSize,MDM_GP_IM,ASS_IMC_ADDREQUEST,0);
		else
			GetMainRoom()->SendData(&msg,msg.cbSize,MDM_GP_IM,ASS_IMC_ADDREQUEST,1);
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}
}

/*
Function	:OnNetAddRequest
Notes		:���������Ϊ���ѣ���Ҫ����
Author		:Fred Huang
Date		:2008-04-7
Parameter	:pData-���������ص�����
return		:void
*/
void CIMMain::OnNetAddRequest(void * pData)
{
	try
	{
		MSG_IM_S_ADDREQUEST * msg=(MSG_IM_S_ADDREQUEST*)pData;
		CString stip;
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		CString str = fMsg.GetKeyVal("IMDlg","MakeFriend","�û� %s �������Ϊ���ѣ�\n%s\n���Ƿ�ͬ�⣿");
		stip.Format(str,msg->sUserName,msg->sUserName,msg->sRequestNotes);
		int ret=GetDesktopWindow()->MessageBox(stip,"��ʾ",MB_ICONQUESTION|MB_YESNOCANCEL);//2008-04-28

		//��Ӧ
		MSG_IM_C_ADDRESPONSE resp;
		ZeroMemory(&resp,sizeof(MSG_IM_C_ADDRESPONSE));
		resp.dwRequestedUserID=msg->dwRequestedUserID;	//��������ID���Լ���ID
		if (ret != IDYES)
			resp.dwRefusedUserID = msg->dwUserID;
		resp.dwUserID=msg->dwUserID;					//������ID���Է�ID

		memcpy(&resp.sRequestedUserName,GetMainRoom()->m_PlaceUserInfo.nickName,sizeof(GetMainRoom()->m_PlaceUserInfo.nickName));
		memcpy(resp.sRequirUsrName,msg->sUserName,sizeof(resp.sRequirUsrName));
		resp.nResponse=ret;
		resp.nSex=0;
		GetMainRoom()->SendData(&resp,sizeof(MSG_IM_C_ADDRESPONSE),MDM_GP_IM,ASS_IMC_ADDREQUESTRESPONSE,0);
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}

}

/*
Function	:OnNetAddResponse
Notes		:�����Ϊ���ѣ��Է����˷�Ӧ
Author		:Fred Huang
Date		:2008-04-7
Parameter	:pData-���������ص�����
return		:void
*/
void CIMMain::OnNetAddResponse(void * pData)
{
	try
	{
		MSG_IM_S_ADDRESPONSE * msg=(MSG_IM_S_ADDRESPONSE*)pData;
		if(msg->nResponse!=IDYES)
		{
			if (msg->dwRefusedUserID == GetMainRoom()->m_PlaceUserInfo.dwUserID)
			{
				CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
				
				//AFCMessageBox(fMsg.GetKeyVal("IMDlg","RefuseMakeFriend","�Բ��𣬶Է��ܾ�����Ϊ���ѣ�"),fMsg.GetKeyVal("IMDlg","Tip","��ʾ"));
	            DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,fMsg.GetKeyVal("IMDlg","Tip","��ʾ"),false,fMsg.GetKeyVal("IMDlg","RefuseMakeFriend","�Բ��𣬶Է��ܾ�����Ϊ���ѣ�"));
			}
			return;
		}
		HTREEITEM hti=FindUserITEM(msg->dwRequestedUserID);
		if(!hti)
		{
			addUserToList(msg->dwRequestedUserID,msg->sRequestedUserName,1,1,1);

			if (GetMainRoom()->m_PlaceUserInfo.dwUserID == msg->dwUserID)
			{
				CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
				CString szMsg;
				szMsg.Format(fMsg.GetKeyVal("IMDlg","AcceptMakeFriend","���[%s]�Ѿ���ӵ���ĺ����б��С�"), msg->sRequestedUserName);
				AFCTimerMsgBox(5, szMsg,fMsg.GetKeyVal("IMDlg","Tip","��ʾ"));
			}
			else
			{
				CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
				CString szMsg;
				szMsg.Format(fMsg.GetKeyVal("IMDlg","AcceptMakeFriend","���[%s]�Ѿ���ӵ���ĺ����б��С�"), msg->sRequirUsrName);
				AFCTimerMsgBox(5, szMsg,fMsg.GetKeyVal("IMDlg","Tip","��ʾ"));
			}
		}
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}
}

/*
Function	:SetUserGroup
Notes		:�������ú����飬����Ҫ�Է�ȷ��
Author		:Fred Huang
Date		:2008-04-7
Parameter	:
dwUserID	:�Է���ID��
nGroupID	:��ID��1-���ѣ�3-������,0-ɾ��
return		:void
*/
void CIMMain::SetUserGroup(int dwUserId, char* pNick, int nGroupID)
{
	try
	{
		//�����������Ϣ
		MSG_IM_C_SETGROUP msg;
		msg.dwUserID=GetMainRoom()->m_PlaceUserInfo.dwUserID;
		msg.dwRemoteUserID=dwUserId;
		msg.groupID=nGroupID;
		GetMainRoom()->SendData(&msg,sizeof(MSG_IM_C_SETGROUP),MDM_GP_IM,ASS_IMC_SETGROUP,0);
		
		LocalListChange(dwUserId, pNick, nGroupID);
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}

	return;

}

int CIMMain::GetUserGroup(int dwUserId)
{
	HTREEITEM hti=FindUserITEM(dwUserId+1000);
	if(GetParentItem(hti) == htiFamilier)
	{
		return 1;
	}
	else if(GetParentItem(hti) == htiBlack)
	{
		return 3;
	}
	else
	{
		return 2;
	}
	return 0;
}

void CIMMain::LocalListChange(int dwUserId, char* pNick, int nGroupID)
{
	//���ش���
	int nImg = 3;
	CString sname;
	HTREEITEM hti=FindUserITEM(dwUserId + 1000);

	if(pNick == NULL && hti)
	{
		sname=GetItemText(hti);
	}

	if(hti)
	{
		if(nGroupID==0)
		{
			DeleteItem(hti);
			return;
		}
		GetItemImage(hti,nImg,nImg);
		DeleteItem(hti);
	}

	
	else if(pNick == NULL && !hti)
	{
		sname = CString("İ����");
	}
	else
	{
		sname = CString(pNick);
	}


	HTREEITEM insertAfter=TVI_LAST;
// 	if(nImg==3 || nImg==5)
// 		insertAfter=TVI_FIRST;

	if(nGroupID==1)
	{
		hti=InsertItem(sname,nImg,nImg,htiFamilier,insertAfter);
		Expand(htiFamilier,TVE_EXPAND);
		//SetItemImage(htiFamilier,1,1);
	}
	else if(nGroupID==2)
	{
		hti=InsertItem(sname,nImg,nImg,htiUnfamilier,insertAfter);
		Expand(htiUnfamilier,TVE_EXPAND);
		//SetItemImage(htiBlack,1,1);
	}
	else
	{
		hti=InsertItem(sname,nImg,nImg,htiBlack,insertAfter);
		Expand(htiBlack,TVE_EXPAND);
		//SetItemImage(htiBlack,1,1);
	}
	SetItemData(hti,dwUserId + 1000);
}


void CIMMain::OnPaint()
{
	CPaintDC   dc(this); 

	//   Create   a   memory   DC   compatible   with   the   paint   DC 
	CDC   memDC; 
	memDC.CreateCompatibleDC(&dc); 

	CRect   rcClip,   rcClient; 
	dc.GetClipBox(   &rcClip   ); 
	GetClientRect(&rcClient); 

	//   Select   a   compatible   bitmap   into   the   memory   DC 
	CBitmap   bitmap; 
	bitmap.CreateCompatibleBitmap(   &dc,   rcClient.Width(),   rcClient.Height()   ); 
	memDC.SelectObject(   &bitmap   ); 

	//dc.SetBkMode(TRANSPARENT);

	//   Set   clip   region   to   be   same   as   that   in   paint   DC 
	CRgn   rgn; 
	rgn.CreateRectRgnIndirect(   &rcClip   ); 
	memDC.SelectClipRgn(&rgn); 
	rgn.DeleteObject(); 

	//   First   let   the   control   do   its   default   drawing. 
	CWnd::DefWindowProc(WM_PAINT,   (WPARAM)memDC.m_hDC,   0); 

	HTREEITEM   hItem   =   GetFirstVisibleItem(); 

	int   iItemCount   =   GetVisibleCount()   +   1; 
	while(hItem   &&   iItemCount--) 
	{ 
		CRect   rect; 

		//   Do   not   meddle   with   selected   items   or   drop   highlighted   items 
		UINT   selflag   =   TVIS_DROPHILITED   |   TVIS_SELECTED; 
		Color_Font   cf; 


		if   ((GetItemState(hItem,   selflag)   &   selflag)   
			&&   ::GetFocus()   ==   m_hWnd) 
			; 
		else   if   (m_mapColorFont.Lookup(hItem,   cf)) 
		{ 
			CFont   *pFontDC; 
			CFont   fontDC; 
			LOGFONT   logfont; 

			if(cf.logfont.lfFaceName[0]   !=   '\0 ')   
				logfont   =   cf.logfont; 
			else   { 
				//   No   font   specified,   so   use   window   font 
				CFont   *pFont   =   GetFont();
				pFont-> GetLogFont(   &logfont   ); 
			} 

			if(GetItemBold(hItem)) 
				logfont.lfWeight   =   700; 

			fontDC.CreateFontIndirect(&logfont); 
			pFontDC   =   memDC.SelectObject(&fontDC   ); 

			if(cf.color   !=   (COLORREF)   -   1) 
				memDC.SetTextColor(cf.color); 
			else 
				memDC.SetTextColor(GetSysColor(COLOR_WINDOWTEXT)); 


			CString   sItem   =   GetItemText(hItem); 

			GetItemRect(hItem,   &rect,   TRUE); 
			//memDC.SetBkColor( GetSysColor(COLOR_WINDOW )); 
			memDC.SetBkMode(TRANSPARENT);
			memDC.TextOut(rect.left   +   2,   rect.top   +   5,   sItem); 

			memDC.SelectObject(pFontDC); 
		} 

		hItem   =   GetNextVisibleItem(hItem); 
	} 


	dc.BitBlt(rcClip.left,   rcClip.top,   rcClip.Width(),   rcClip.Height(),   &memDC,   
		rcClip.left,   rcClip.top,   SRCCOPY); 

	memDC.DeleteDC(); 
}

void CIMMain::clearAllSession(void)
{
#ifndef  MAINROOM_USE_SHORTMSG
	try
	{
		CIMSession *imSession=NULL;
		while(imSessionList.GetCount()>0)
		{
			imSession=imSessionList.GetHead();
			imSessionList.RemoveHead();
			delete imSession;
		}
		DeleteAllItems();
		bInitialed=false;
	}
	catch (...)
	{
		TCHAR smsg[1000]={0};
		wsprintf(smsg,"IM_TRY_CATCH : [%s]",__FUNCTION__);
	}
#endif
}

void CIMMain::UpdateAllIcon(void)
{
}

#ifndef  MAINROOM_USE_SHORTMSG
#else
CShortMessage * CIMMain::openShortMsg(long dwUserID, char* pNickName)
{
	CShortMessage * pShortMessageWnd=ActiveShortWnd(dwUserID,pNickName,true);
	if (pShortMessageWnd==NULL)	
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		
		//AFCMessageBox(fMsg.GetKeyVal("GameRoom","ClosePartWindows","����̫��,��رղ��ִ���!"));//,MB_ICONQUESTION,this);
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("GameRoom","ClosePartWindows","����̫��,��رղ��ִ���!"));
	}

	return pShortMessageWnd;
}

//������Ϣ����
CShortMessage * CIMMain::ActiveShortWnd(long int dwUserID, char* pNickName, bool bCreate)
{
	//��������
	INT_PTR iCloseIndex=-1,iHideIndex=-1;
	UserItemStruct stUserItem;

	ZeroMemory(&stUserItem, sizeof(stUserItem));
	stUserItem.GameUserInfo.dwUserID = dwUserID;
	stUserItem.GameUserInfo.dwAccID = 0;
	strcpy(stUserItem.GameUserInfo.nickName, pNickName);

	//Ѱ�����ϴ���
	for (INT_PTR i=0;i<m_ShortMessage.GetCount();i++)
	{
		CShortMessage * pShortMessageWnd=(CShortMessage *)m_ShortMessage.GetAt(i);
		ASSERT(pShortMessageWnd!=NULL);
		if (pShortMessageWnd!= NULL)
		{
			if (dwUserID==pShortMessageWnd->m_dwTargetUserID)
			{
				if (pShortMessageWnd->GetSafeHwnd()==NULL)
				{
					AfxSetResourceHandle(GetModuleHandle(NULL));
					pShortMessageWnd->Create(IDD_SHORT_MESSAGE,GetDesktopWindow());
					pShortMessageWnd->CenterWindow();
				}
				pShortMessageWnd->ShowWindow(SW_SHOWNOACTIVATE);
				return pShortMessageWnd;
			}
		}
		else
		{
			m_ShortMessage.RemoveAt(i);
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
			pShortMessageWnd->Create(IDD_SHORT_MESSAGE,GetDesktopWindow());
			pShortMessageWnd->CenterWindow();
		}
		SendMsgUserStruct user;
		pShortMessageWnd->TransUserInfo(&stUserItem, &user);
		pShortMessageWnd->SetTalkInfo(&user,GetMainRoom()->m_PlaceUserInfo.dwGamePower,"����");
		pShortMessageWnd->ShowWindow(SW_SHOWNOACTIVATE);
		return pShortMessageWnd;
	}

	//ʹ�����ش���
	if ((bCreate==true)&&(iHideIndex>=0))
	{
		CShortMessage * pShortMessageWnd=(CShortMessage *)m_ShortMessage.GetAt(iHideIndex);
		ASSERT(pShortMessageWnd!=NULL);
		SendMsgUserStruct user;
		pShortMessageWnd->TransUserInfo(&stUserItem, &user);
		pShortMessageWnd->SetTalkInfo(&user,GetMainRoom()->m_PlaceUserInfo.dwGamePower,"����");
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
			pShortMessageWnd->TransUserInfo(&stUserItem, &user);
			pShortMessageWnd->SetTalkInfo(&user,GetMainRoom()->m_PlaceUserInfo.dwGamePower,"����");
			pShortMessageWnd->Create(IDD_SHORT_MESSAGE,GetDesktopWindow());
			pShortMessageWnd->CenterWindow();
			pShortMessageWnd->ShowWindow(SW_SHOWNOACTIVATE);
			m_ShortMessage.Add(pShortMessageWnd);
			return pShortMessageWnd;
		}
		catch (...) { return NULL; }
	}

	return NULL;
}

bool CIMMain::GetSendMsgUserStruct(long int lUserID, SendMsgUserStruct * user)
{
	user->bDeskNO = 255;
	user->bGameMaster = 0;
	user->dwUserID = GetMainRoom()->m_PlaceUserInfo.dwUserID;
	user->dwAccID = GetMainRoom()->m_PlaceUserInfo.dwAccID;
	strcpy(user->nickName, GetMainRoom()->m_PlaceUserInfo.nickName);
	return true;
}

SendMsgUserStruct * CIMMain::GetMeUserInfo()
{
	m_SortMsgUserInfo.bDeskNO = 255;
	m_SortMsgUserInfo.bGameMaster = 0;
	m_SortMsgUserInfo.dwUserID = GetMainRoom()->m_PlaceUserInfo.dwUserID;
	m_SortMsgUserInfo.dwAccID = GetMainRoom()->m_PlaceUserInfo.dwAccID;
	strcpy(m_SortMsgUserInfo.nickName, GetMainRoom()->m_PlaceUserInfo.nickName);

	return &m_SortMsgUserInfo;
}

int CIMMain::SendData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return GetMainRoom()->SendData(pData, uBufLen, bMainID, bAssistantID, bHandleCode);
}


#endif