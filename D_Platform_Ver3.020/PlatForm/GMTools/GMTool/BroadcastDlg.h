#pragma once

//////////////////////////////////////////////////////////////////////////
///���浯���Ի���
///2010.8.12 rongqiufen
//////////////////////////////////////////////////////////////////////////

#include "GM_MessageDefine.h"
#include "CommandMessage.h"

// CBroadcastDlg �Ի���

class CBroadcastDlg : public CDialog
{
	DECLARE_DYNAMIC(CBroadcastDlg)

public:
	CBroadcastDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBroadcastDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	GM_Broadcast_t	m_Broadcast_t;			///��������	
	CString m_Info;						    ///��������						

public:
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedOk();
};
