#pragma once

//////////////////////////////////////////////////////////////////////////
///��¼�Ի��򣬸����û������ʺ������Լ���¼Z������
///2010.8.12 rongqiufen
//////////////////////////////////////////////////////////////////////////

#include "afxwin.h"
#include "CommandMessage.h"


// CLongoDlg �Ի���

class CLongoDlg : public CDialog
{
	DECLARE_DYNAMIC(CLongoDlg)

public:
	CLongoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLongoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox	m_UeserNameCmb;      ///�û��������
	CEdit		m_PassWordEdt;		 ///���������
	CString		m_UeserName;		 ///�û���	
	CString		m_PassWord;			 ///����	
	
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
protected:
	virtual void OnCancel();
public:
	CString m_name;
};
