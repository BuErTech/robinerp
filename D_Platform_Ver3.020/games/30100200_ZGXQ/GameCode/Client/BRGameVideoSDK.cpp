// BRGameVideoSDK.cpp : implementation file
//
#include "StdAfx.h"
#include <afxwin.h>
#include "BRGameVideoSDK.h"



/////////////////////////////////////////////////////////////////////////////
// CBRGameVideoSDK

CBRGameVideoSDK::CBRGameVideoSDK()
{
	m_pBRGameVideoClient = NULL;
}

CBRGameVideoSDK::~CBRGameVideoSDK()
{
}

/**
 *	��ʼ��SDK���÷�����������������������֮ǰ����
 *	@param pParent ������ָ�룬����Ϊ��
 *	@param hMsgWnd ��Ϣ�ӿڴ��ھ����Ĭ��Ϊ�գ���ʾ��Ϣ���͵�������
 *	@return ��ʼ���ɹ�����TRUE�����򷵻�FALSE�����ܵ�ԭ����SDK���û��ע��
 */
BOOL CBRGameVideoSDK::InitGameVideoSDK(CWnd* pParent, HWND hMsgWnd)
{
	if(!pParent)
		return FALSE;
	if(m_pBRGameVideoClient)		///< �Ѿ���ʼ��
		return TRUE;

	BOOL rc = FALSE;

	m_pBRGameVideoClient = new CBRGameVideoClient;
	if(m_pBRGameVideoClient->Create(NULL,0,CRect(0,0,0,0),pParent,0))
	{
		m_pBRGameVideoClient->ShowWindow(SW_HIDE);
		// ����־��¼ 
		m_pBRGameVideoClient->ActiveCallLog(TRUE);
		// ������Ϣ֪ͨ���վ��
		long* phWnd = hMsgWnd ? (long*)&hMsgWnd : (long*)&pParent->m_hWnd;
		m_pBRGameVideoClient->SetMessageNotifyHandle(phWnd);
		// ���òƸ�ͼ���ļ�
		m_pBRGameVideoClient->SetFortuneImage(NULL);
		
		// ����¼����ʱ�ļ�����·��
		CHAR szRecordDirectory[MAX_PATH] = {0};
		::GetModuleFileName(NULL,szRecordDirectory,MAX_PATH);
		(strrchr(szRecordDirectory,'\\'))[1] = 0; 
		strcat(szRecordDirectory,"Record");
		m_pBRGameVideoClient->SetRecordFileSavePath(szRecordDirectory);
		
		// ���ÿ�����ʱ�ļ�����·��
		CHAR szSnapShotDirectory[MAX_PATH] = {0};
		::GetModuleFileName(NULL,szSnapShotDirectory,MAX_PATH);
		(strrchr(szSnapShotDirectory,'\\'))[1] = 0; 
		strcat(szSnapShotDirectory,"SnapShot");
		m_pBRGameVideoClient->SetSnapShotSavePath(szSnapShotDirectory);

		SetVideoFrameStyle();
		rc = TRUE;
	}
	else
	{
		delete m_pBRGameVideoClient;
		m_pBRGameVideoClient = NULL;
		rc = FALSE;
	}
	return rc;
}

/**
 *	������Ƶ������
 */
void CBRGameVideoSDK::SetVideoFrameStyle(void)
{
	GV_VW_STYLE					m_VideoWindowStyle;			///< ��Ƶ������

	memset(&m_VideoWindowStyle,0,sizeof(GV_VW_STYLE));
	m_VideoWindowStyle.cbSize = sizeof(GV_VW_STYLE);
	m_VideoWindowStyle.bkColor = RGB(4,111,193);			///< ���ô��屳��ɫ
	m_VideoWindowStyle.selfTitleColor = RGB(254,114,141);	///< �Լ��ı�����ɫ
	m_VideoWindowStyle.otherTitleColor = RGB(255,255,255);	///< ���˵ı�����ɫ
	m_VideoWindowStyle.vfStyle = GV_VF_STYLE_SMALLFRAME;	///< ���ô�����ʽ
	m_VideoWindowStyle.bMoveWindow = FALSE;					///< ��֧�ִ����ƶ�
	m_VideoWindowStyle.bShowTitle = TRUE;					///< ��ʾ����
	m_VideoWindowStyle.bShowVolume = TRUE;					///< ��ʾ����
	m_VideoWindowStyle.vfSkin = GV_VF_SKIN_DEFAULT;			///< ����Ĭ�ϴ���Ƥ�����

	GV_VFT_STYLE				m_VideoToolbarStyle;		///< ��Ƶ���������
	memset(&m_VideoToolbarStyle,0,sizeof(GV_VFT_STYLE));
	m_VideoToolbarStyle.cbSize = sizeof(GV_VFT_STYLE);
	m_VideoToolbarStyle.bkColor = RGB(241,254,150);			///< ���ù���������ɫ
	m_VideoToolbarStyle.maxTransPercent = 70;				///< ���ù��������͸����
	m_VideoToolbarStyle.animateDuration = 500;				///< ���ù������仯����(����)
	m_VideoToolbarStyle.dwBtnMark = 0xFFFFFFFFL;			///< ��ʾ���а�ť
	m_VideoToolbarStyle.dwBtnSpacePix = 5;					///< ��ť֮��ļ��
	m_VideoToolbarStyle.vftDispMode = GV_VFTDM_AUTOSHOW;	///< ��������ʾģʽ���Զ�
	m_VideoToolbarStyle.vftBtnAlign = GV_VFTBA_LEFT;		///< ��ť����ģʽ
	strcpy((char*)m_VideoToolbarStyle.szExtendBtnTooltip,"����...");	///< ��������չ��ť��ʾ��Ϣ
	m_VideoToolbarStyle.dwEBtnTooltipSize =					///< ��������չ��ť��ʾ��Ϣ�ַ�������
		strlen((char*)m_VideoToolbarStyle.szExtendBtnTooltip);

	if(m_pBRGameVideoClient)
	{
		m_pBRGameVideoClient->SetVideoFrameStyle((long*)&m_VideoWindowStyle);
		m_pBRGameVideoClient->SetVideoToolBarStyle((long*)&m_VideoToolbarStyle);
	}
}
/**
 *	�ر�SDK���˳�ϵͳ֮ǰ���ã������ͷ���Դ�������ڴ�й¶
 */
void CBRGameVideoSDK::CloseGameVideoSDK(void)
{
	if(m_pBRGameVideoClient)
	{
		m_pBRGameVideoClient->Release();
		m_pBRGameVideoClient->DestroyWindow();
		delete m_pBRGameVideoClient;
		m_pBRGameVideoClient = NULL;
	}
}
/**
 *	���ӵ�������
 *	�÷����������أ����ӷ����Ƿ�ɹ�����ͨ����ص���Ϣ���
 *	@param ipaddr ������IP��ַ������ΪIP��ַ��������������
 *	@param port �������˿ڣ�Ĭ��Ϊ8902
 */
void CBRGameVideoSDK::ConnectToServer(LPCSTR ipaddr, DWORD port)
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->StartVideoClient(ipaddr,(SHORT)port);
}
/**
 *	��¼������
 *	�÷����������أ���¼�Ƿ�ɹ�����ͨ����ص���Ϣ���
 *	@param username ��¼�û�����Ĭ��Ϊ�գ���ʾ���οͷ�ʽ��¼ϵͳ
 *	@param password ��¼���룬Ĭ�Ͽգ���ʾ���οͷ�ʽ��¼ϵͳ
 */
void CBRGameVideoSDK::LoginSystem(LPCSTR username, LPCSTR password)
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->RegisterUser(0,0,username,password,0);
}
/**
 *	�ӷ�����ע��
 */
void CBRGameVideoSDK::LogoutSystem(void)
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->UnRegisterUser(-1);
}
/**
 *	���뷿��
 *	@param roomid ������
 *	@param roompass �������룬Ĭ��Ϊ�գ���ʾ�÷���û������
 */
void CBRGameVideoSDK::EnterRoom(INT roomid, LPCSTR roompass)
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->EnterRoom(roomid,roompass,0);	
}
/**
 *	�뿪����
 *	@param roomid �����ţ�Ĭ��Ϊ-1����ʾ�˳���ǰ����
 */
void CBRGameVideoSDK::LeaveRoom(INT roomid)
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->LeaveRoom(roomid);	
}
/**
 *	�򿪱���Mic
 */
void CBRGameVideoSDK::OpenLocalMic()
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->UserSpeakControl(-1,TRUE);
}
/**
 *	�رձ���Mic
 */
void CBRGameVideoSDK::CloseLocalMic()
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->UserSpeakControl(-1,FALSE);
}
/**
 *	�򿪱�������ͷ
 */
void CBRGameVideoSDK::OpenLocalCamera()
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->UserCameraControl(-1,TRUE,0,0,0,0,0);
}
/**
 *	�رձ�������ͷ
 */
void CBRGameVideoSDK::CloseLocalCamera()
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->UserCameraControl(-1,FALSE,0,0,0,0,0);
}
/**
 *	��ȡ��������ͷ״̬
 *	���ݷ��ز����Ĳ�ͬ�������б��û���ǰ����ͷ��״̬
 *	@return û������ͷ����0��������ͷ��û�д򿪷���1������ͷ�Ѵ򿪷���2
 */
INT CBRGameVideoSDK::GetLocalCameraState()
{
	INT rc = 0;
	if(m_pBRGameVideoClient)
		rc = m_pBRGameVideoClient->GetCameraState(-1);
	return rc;
}

/**
 *	��ʾ������Ƶ�������
 */
void CBRGameVideoSDK::ShowSettingPanel(void)
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->ShowSettingPanel();
}

/**
 *	��ʾ�����أ��û���Ƶ����
 *	@param userid		�û���ţ�-1��ʾ��ʾ�Լ������
 *	@param bShow		�Ƿ���ʾ��BOOL�ͣ�TRUE��ʾ�������ʾ����
 *	@param rc			��ʾλ��
 *	@param style		��ʾ��ʽ��0 �������ڣ�1 �Ӵ���
 *	@param pParentWnd	�����ڶ���ָ��
 */
void CBRGameVideoSDK::ShowUserVideo(INT userid, short bShow, CRect rc, CWnd* pParentWnd)
{
	if(m_pBRGameVideoClient)
	{
//		m_pBRGameVideoClient->ShowUserPanelEx(userid,bShow,
//		(short)rc.left,(short)rc.top,(short)rc.right,(short)rc.bottom,1,(long*)pParentWnd);

/*
 ע��������ķ���Ҳ���ԣ�ShowUserPanelEx2��������V4.1�汾�������ģ��������ڣ�
	ShowUserPanelEx(...,pParentWnd)�����Ĳ����Ǵ��� CWnd*��ַ
	ShowUserPanelEx2(...,parentHandle)�����Ĳ����Ǵ��� CWnd����ľ����m_hWnd����ַ���÷���������C#�����Ե���	
*/

		HWND parentHandle = pParentWnd->GetSafeHwnd();
		m_pBRGameVideoClient->ShowUserPanelEx2(userid,bShow,
		(short)rc.left,(short)rc.top,(short)rc.right,(short)rc.bottom,1,(long*)&parentHandle);
	}
}
/**
 *	�����û�����������
 *	ͨ���÷������������û���nickname��������Ϣ�ȣ�Ĭ������£������ø÷�������ʾ�û��ĵ�¼�û���
 *	�÷��������û������ʾ֮����ã�����ShowUserVideo����֮�����
 *	@param userid		�û���ţ�-1��ʾ�����Լ���������
 *	@param lpTitleStr	����������
 */
void CBRGameVideoSDK::SetUserTitle(INT userid, LPCTSTR lpTitleStr) 
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->SetUserPanelTitle(userid,lpTitleStr);
}
/**
 *	�����û��ĸ���ͼƬ����ʾ�ڱ�����
 *	@param userid �û���ţ�-1��ʾ�Լ�
 *	@param pathname ͼƬ·��
 */
void CBRGameVideoSDK::SetUserPicture(INT userid, LPCSTR pathname)
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->SetUserPicture(userid,pathname);
}
/**
 *	��ȡ��ǰ���������û��б�
 *	@param �����û��б���Ϣ��list
 *	@return ���������û���
 */
INT CBRGameVideoSDK::GetRoomUserList(std::list<INT>& userlist)
{
	INT num = 0;
	if(m_pBRGameVideoClient)
	{
		m_pBRGameVideoClient->PrepaGetUserList();
		INT userid = -1;
		while( (userid = m_pBRGameVideoClient->FetchNextUserID()) != -1)
		{
			userlist.push_back(userid);
			num ++;
		}
	}
	return num;
}

/**
 * �õ�ָ��ID���û���
 *
 * @param id Ҫ�õ��û���ID
 *
 * @return ��������û�������
 */
CString CBRGameVideoSDK::GetUserNameById(int id)
{
	if(m_pBRGameVideoClient) 
		return m_pBRGameVideoClient->GetUserNameByUserID(id);

	return "";
}

/**
 * ˢ��ָ�������
 *
 * @param id Ҫˢ�µ����
 */
void CBRGameVideoSDK::RefreshUserPanel(int id)
{
	if(m_pBRGameVideoClient) 
		m_pBRGameVideoClient->RefreshUserPanel(id);
}

/**
 *	���òƸ��˵���ͼ��
 *	ͼ���ļ��е�ÿ��ͼ���16�����أ���15�����أ�RGB24λɫ������ɫΪRGB(255,0,255)ʱ����͸��
 *	@param lpImageFile ͼ���б��ļ������Ϊ�գ���ʹ��ϵͳĬ�ϵ�ͼ��
 *	@return �ɹ�����TRUE�����򷵻�FALSE��ʾ����ʧ��
 */
BOOL CBRGameVideoSDK::SetFortuneImage(LPCTSTR lpImageFile) 
{
	BOOL rc = FALSE;
	if(m_pBRGameVideoClient)
		rc = m_pBRGameVideoClient->SetFortuneImage(lpImageFile)==0 ? TRUE : FALSE;
	return rc;
}
/**
 *	���ã���գ�ָ���û��ĲƸ��˵�
 *	@param userid �û���ţ�-1��ʾ�����Լ��ĲƸ��˵�
 */
void CBRGameVideoSDK::ResetFortuneMenu(long userid) 
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->ResetFortuneMenu(userid);
}
/**
 *	��ָ���û���ӲƸ��˵���÷���ͨ������ʾ�û�������ã������û��ĲƸ���Ϣ�����ı�ʱ����
 *	@param userid �û���ţ�-1��ʾ�Լ�
 *	@param menuText �Ƹ��˵���ʾ����
 *	@param toolTip  �Ƹ��˵�����ʾ��Ϣ
 *	@param menuTag  �˵���ǣ�ֵҪ�����0�����û�ѡ��һ���˵��󣬻�ͨ����Ϣ������ֵ���͸����ó��򣬱��ڵ��ó�����Ӧ�˵��¼�
 *	@param imageIndex �Ƹ��˵�ͼ����������0��ʼ����ʾ�ò˵���ʾimageIndex��ָ��ͼ�꣬ͼ��ΪSetFortuneImage��������
 */
void CBRGameVideoSDK::AppendFortuneMenuItem(long userid, LPCTSTR menuText, LPCTSTR toolTip, short menuTag, short imageIndex) 
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->AppendFortuneMenuItem(userid,menuText,toolTip,menuTag,imageIndex);
}

/**
 *	���ĵ�ǰ����ģʽ
 *	@param chatmode ����ģʽ��0 ����ģʽ�� 1 ˽��ģʽ
 */
void CBRGameVideoSDK::ChangeChatMode(DWORD chatmode)
{
	if(m_pBRGameVideoClient)
		m_pBRGameVideoClient->ChangeChatMode(chatmode);
}

/**
 *	��ȡָ���û���ǰ������ģʽ
 *	@param userid ��Ҫ��ȡ����ģʽ���û�ID�ţ�-1��ʾ��ȡ�Լ�������ģʽ
 *	@return ���ظ��û�������ģʽ
 */
long CBRGameVideoSDK::GetUserChatMode(long userid) 
{
	long rc = 0;
	if(m_pBRGameVideoClient)
		rc = m_pBRGameVideoClient->GetUserChatMode(userid);
	return 0;
}
/**
 *	������Է�˽�ģ���Է�����˽������
 *	@param userid �Է����û�ID��
 */
long CBRGameVideoSDK::PrivateChatRequest(long userid) 
{
	long rc = 0;
	if(m_pBRGameVideoClient)
		rc = m_pBRGameVideoClient->PrivateChatRequest(userid);
	return rc;
}
/**
 *	�ظ��Է���˽������
 *	@param userid �Է����û�ID��
 *	@param requestid ����ID�ţ���������Ϣ�д���������ʾ����һ������
 *	@param bAccept �Ƿ�ͬ��Է���˽������
 */
long CBRGameVideoSDK::PrivateChatEcho(long userid, long requestid, long bAccept) 
{
	long rc = 0;
	if(m_pBRGameVideoClient)
		rc = m_pBRGameVideoClient->PrivateChatEcho(userid,requestid,bAccept);
	return rc;
}
/**
 *	�˳���ĳ�û���˽�ģ����߽�ĳ�û����Լ���˽���б������
 *	�Ͽ�����û���˽�����Ӻ󣬲�Ӱ���������û���˽��
 *	@param userid �Է����û�ID��
 */
long CBRGameVideoSDK::PrivateChatExit(long userid) 
{
	long rc = 0;
	if(m_pBRGameVideoClient)
		rc = m_pBRGameVideoClient->PrivateChatExit(userid);
	return rc;
}