#pragma once

#include "resource.h"
#include "afxwin.h"
#include "SkinMgr.h"
#include "..\UILibrary2003\CconfigDlg.h" //2012.10.15 yyf

// CAddFriend �Ի���

class CAddFriend : public CDialog
{
	DECLARE_DYNAMIC(CAddFriend)

public:
	CAddFriend(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddFriend();

// �Ի�������
	enum { IDD = IDD_DIALOG10 };

	CGameImage		m_bkimage;
	HBRUSH			m_bkBrush;
	
	CconfigDlg  m_configDlg; //���ص�ͼ��. 2012.10.15 yyf
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();

	afx_msg void OnPaint();
	LRESULT OnExchangeSkin(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
private:
	void Init();

	void LoadSkin();
public:
	CString m_Flag;

	CNormalBitmapButtonEx  m_Ok;
	CNormalBitmapButtonEx  m_Cancel;
	CNormalBitmapButtonEx  m_Close;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClose();
public:
	CComboBox m_Combox;


	afx_msg void OnCbnSelchangeCombo1();
public:
	CString m_flagtype;
private:
	CSkinMgr m_skinmgr;
public:
	// ���ô���Բ�� 2012.10.15 yyf
	void SetWndRgn(void);
};
