#pragma once


// CIMSetUser �Ի���

class CIMSetUser : public CDialog
{
	DECLARE_DYNAMIC(CIMSetUser)

public:
	CIMSetUser(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIMSetUser();

	int		dwUser;
	CString szName;
	CString imWords;
// �Ի�������
	enum { IDD = IDD_IM_SETUSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctlWords;
public:
	CStatic ctlTip;
protected:
	//	virtual void PreInitDialog();
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
};
