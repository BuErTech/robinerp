#pragma once

#include "resource.h"
// CExtRobot �Ի���

class CExtRobot : public CDialog
{
	DECLARE_DYNAMIC(CExtRobot)

public:
	CExtRobot(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExtRobot();

// �Ի�������
	enum { IDD = IDD_EXTROBOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_extRobot;
public:
	afx_msg void OnBnClickedOk();
};
