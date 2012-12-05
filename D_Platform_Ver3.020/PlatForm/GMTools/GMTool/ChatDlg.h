#pragma once

//////////////////////////////////////////////////////////////////////////
///˽�ĵ����Ի���
///2010.8.12 rongqiufen
//////////////////////////////////////////////////////////////////////////


#include "afxcmn.h"
#include "GM_MessageDefine.h"
#include "CommandMessage.h"
#include "comstruct.h"

// CChatDlg �Ի���

class CChatDlg : public CDialog
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChatDlg();

	void SetUserInfo(UserInfoStruct *userinfo, int iRoomID);
	void SetUserInfo(GM_OperatedPlayerInfo_t *userinfo);
	void AddMsg(CString msg, bool issystem = false);

// �Ի�������
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��



	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
public:
	GM_Broadcast_t	m_Broadcast_t;				///˽�����ݰ�	
	CString m_SendInfo;							///Ҫ���͵�˽������	
	int m_iUserID;								///�û�ID
	CString m_szNickName;						///�û��ǳ�
	CString m_szUserName;						///�û���¼��		
	int m_RoomID;								///����ID	
	HBRUSH m_hBrush;							///ֻ��EDIT�ı���ˢ		
public:
	afx_msg void OnClose();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	CString m_ChatInof;
};
