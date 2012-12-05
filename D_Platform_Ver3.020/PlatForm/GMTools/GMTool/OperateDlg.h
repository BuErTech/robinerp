#pragma once

//////////////////////////////////////////////////////////////////////////
///�����Ի����û���д��������ʱ�䣬ԭ�����ѡ���������ֹ���ԣ��߳����䣬��ֹ��¼
///2010.8.12 rongqiufen
//////////////////////////////////////////////////////////////////////////


#include "comstruct.h"
#include "GM_MessageDefine.h"
#include "CommandMessage.h"
#include "afxwin.h"

// COperateDlg �Ի���

class COperateDlg : public CDialog
{
	DECLARE_DYNAMIC(COperateDlg)

public:
	COperateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COperateDlg();

	void Init();
	void SetUserInfo(UserInfoStruct *userinfo, int iRoomID);
	void SetUserInfo(GM_OperatedPlayerInfo_t *userinfo);
	void SetOperateType(int operatetype);

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
public:
	int			m_day;			///����
	int			m_hour;			///ʱ��
	int			m_min;			///����
public:
	CString		m_Reason;		///ԭ��
public:
	UINT		m_UserID;		///�û�ID
	CString		m_UserNickName;	///�û��ǳ�
	CString		m_UserLoginName;///�û���¼��

	int			m_OperateType;	///��������
	int			m_RoomID;		///�����
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();
public:
	BOOL m_Forever;						///�Ƿ�����
	GM_OperateByID_t	m_GMOperate;	///�������ݰ�
public:
	CEdit m_dayEdit;					///���������
	CEdit m_hourEdit;					///ʱ�������
	CEdit m_MinEdit;					///���������
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnEnChangeEdit5();
};
