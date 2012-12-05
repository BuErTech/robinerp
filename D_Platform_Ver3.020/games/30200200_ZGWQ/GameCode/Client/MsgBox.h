#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <afxwin.h>


#define IDR_HIDE		1
// CMsgBox �Ի���

class CMsgBox : public CDialog
{
	DECLARE_DYNAMIC(CMsgBox)

public:
	CMsgBox(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMsgBox();

// �Ի�������
	enum { IDD = IDD_MSGBOX };
	CString m_msg;
	int		m_Time;
	CGameImage		m_bkImg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	CNormalBitmapButtonEx m_btcancel;
	CNormalBitmapButtonEx m_btleave;
public:
	void ShowMsg(TCHAR  szMsg[], int time,bool Showleave=false);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnPaint();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
};
