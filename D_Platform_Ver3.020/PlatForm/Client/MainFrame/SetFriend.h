#pragma once

#include "resource.h"
#include "SkinMgr.h"
#include "..\UILibrary2003\CconfigDlg.h" //2012.10.15 yyf
// CSetFriend �Ի���

class CSetFriend : public CDialog
{
	DECLARE_DYNAMIC(CSetFriend)

public:
	CSetFriend(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetFriend();

	int m_iType;

// �Ի�������
	enum { IDD = IDD_DIALOG11 };

	CGameImage		m_bkimage;
	HBRUSH			m_bkBrush;
	
	CconfigDlg  m_configDlg; //���ص�ͼ��. 2012.10.15 yyf
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	virtual void OnOK();

	void SetType(int Type);

	CNormalBitmapButtonEx  m_Ok;
	CNormalBitmapButtonEx  m_Cancel;
	CNormalBitmapButtonEx  m_Close;


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClose();

private:
	CSkinMgr m_skinmgr;
private:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);

	void Init();

	void LoadSkin();
public:
	// ���ô���Բ�� 2012.10.15 yyf
	void SetWndRgn(void);
};
