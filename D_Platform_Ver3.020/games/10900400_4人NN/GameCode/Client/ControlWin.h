#pragma once
#include "StdAfx.h"
#include "Resource.h"

// CControlWin �Ի���

class CControlWin : public CDialog
{
	DECLARE_DYNAMIC(CControlWin)

public:
	CControlWin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CControlWin();

// �Ի�������IDD_CONTROLWIN
	enum { IDD = IDD_CONTROLWIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	void SetParentWnd(CWnd* pWnd);
};
