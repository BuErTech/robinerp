#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "IMSession.h"
#include "afxwin.h"
#include "explorer1.h"
#define  MAINROOM_USE_SHORTMSG

extern CPtrArray m_ShortMessage;						//��Ϣ����

class CIMMain : public CTreeCtrl , public ISortMessageUserInterface
{
protected: 
	struct   Color_Font
	{
		COLORREF	color; 
		LOGFONT		logfont;
	};
	CMap<void*,void*,Color_Font,Color_Font&>	m_mapColorFont; 

public:
	void SetItemFont(HTREEITEM,   LOGFONT&); 
	void SetItemBold(HTREEITEM,   BOOL); 
	void SetItemColor(HTREEITEM,   COLORREF); 
	BOOL GetItemFont(HTREEITEM,   LOGFONT*); 
	BOOL GetItemBold(HTREEITEM); 
	COLORREF GetItemColor(HTREEITEM); 

protected: 
	//{{AFX_MSG(CTreeCtrlEx) 
	afx_msg   void   OnPaint(); 
	//}}AFX_MSG 

	DECLARE_MESSAGE_MAP()


	//���ܺ���
public:
	//��ʼ������
	bool Init();

public:
	void UpdateAllIcon(void);

private:
	DECLARE_DYNAMIC(CIMMain)
	bool			bInitialed;
	HTREEITEM		htiFamilier;		//������
	HTREEITEM		htiUnfamilier;		//İ����
	HTREEITEM		htiBlack;			//������

	CImageList		m_imgList;			//ͼ���б�

public:
#ifndef  MAINROOM_USE_SHORTMSG
	CList<CIMSession *,CIMSession*>imSessionList;
#else
	//�ؼ�����
public:
	static int			m_iMaxShortWnd;						//��ര��
	SendMsgUserStruct	m_SortMsgUserInfo;
	COLORREF			m_SendFontColor;
	COLORREF			m_ReceiveFontColor;
#endif

public:
	CIMMain(void);
public:
	~CIMMain(void);

public:
	bool InitialIMList(void);
#ifndef  MAINROOM_USE_SHORTMSG
public:
	CIMSession* openIMSession(int dwUserID,CString szName);
#endif
public:
	void checkUserCount(void);
	void OnNetGetUserList(void* pData);
	void addUserToList(int dwUserID, CString szUsername, int sex, int GroupID, int nOnline);
	HTREEITEM FindUserITEM(int dwUserID);

	HTREEITEM FindUserITEM(CString szName);

	void OnNetGetUserInfo(void * pData);
	void OnNetGetMsg(void * pData);
	void SetUser(int dwUser,CString szName="",bool isBlacklist=false);


	void OnNetAddRequest(void *pData);
	void OnNetAddResponse(void * pData);
	void OnNetMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);

	void OnNetAddRetrue(NetMessageHead * pNetHead, void * pData);

public:
	void SetUserGroup(int dwUserId, char* pNick, int nGroupID);
	int GetUserGroup(int dwUserId);
public:
	void clearAllSession(void);
#ifdef  MAINROOM_USE_SHORTMSG
public:
	CShortMessage * ActiveShortWnd(long int dwUserID, char* pNickName, bool bCreate);
	bool GetSendMsgUserStruct(long int lUserID, SendMsgUserStruct * user);
	CShortMessage * openShortMsg(long dwUserID, char* pNickName);
	virtual SendMsgUserStruct * GetMeUserInfo();
	virtual int SendData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
#endif

private:
	void LocalListChange(int dwUserId, char* pNick, int nGroupID);

public:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);
};
