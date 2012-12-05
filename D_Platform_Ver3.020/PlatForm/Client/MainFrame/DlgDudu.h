#pragma once

#ifndef AFC_CDLGDUDU_HEAD_FILE
#define AFC_CDLGDUDU_HEAD_FILE


#include "..\�����.2003\AFCFunction.h"

// CDlgDudu �Ի���

class CDlgDudu : public CDialog
{
	DECLARE_DYNAMIC(CDlgDudu)

public:
	CDlgDudu(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDudu();
	
	CWnd* m_pGameRoom;
// �Ի�������
	//enum { IDD = IDD_DLG_DUDU };

protected:

	bool		m_bCanSend;

	CGameImage	m_bkimage;
	CNormalBitmapButtonEx	m_btcancel;
	CNormalBitmapButtonEx	m_btok;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	afx_msg void OnEnChangeEdit1();

	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	// �����ַ�
	CString m_InputStr;
public:
	afx_msg void OnBnClickedCancel();
};
#endif