#pragma once

//////////////////////////////////////////////////////////////////////////
///�����û��Ի���
///2010.8.12 rongqiufen
//////////////////////////////////////////////////////////////////////////

#include "GM_MessageDefine.h"
#include "CommandMessage.h"
// CFindUserDlg �Ի���

class CFindUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindUserDlg)

public:
	CFindUserDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFindUserDlg();


	
// �Ի�������
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	GM_GetOperationByPlayer_t m_GMGetPlay;			///�����û����ݰ�
	CString m_UserNickName;							///������û�ID���ǳƻ��ߵ�¼��

	DECLARE_MESSAGE_MAP()
public:
	
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
};
