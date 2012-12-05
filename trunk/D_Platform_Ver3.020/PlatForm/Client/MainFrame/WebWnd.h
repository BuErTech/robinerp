#pragma once
#include "explorer1.h"
#include "resource.h"

// CWebWnd �Ի���

class CWebWnd : public CDialog
{
	DECLARE_DYNAMIC(CWebWnd)

public:
	CWebWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWebWnd();

// �Ի�������
	enum { IDD = IDD_WEB_WND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CExplorer1 m_ie;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	void Browse(CString strURL);
};
