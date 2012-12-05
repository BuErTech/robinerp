#pragma once

#include "resource.h"

#include "IMMain.h"
#include "AddFriend.h"
#include "SetFriend.h" 
#include "..\UILibrary2003\CconfigDlg.h" 

// CIMWnd �Ի���

class CIMWnd : public CDialog
{
	DECLARE_DYNAMIC(CIMWnd)

public:
	CIMWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIMWnd();

// �Ի�������
	enum { IDD = IDD_DIALOG9 };

	CIMMain*		m_IMList;
	CGameImage		m_bkimage;
	HBRUSH			m_bkBrush;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedSet();
	afx_msg void OnBnClickedClose();

	//���IM�б�
	afx_msg void OnNMClickIMList(NMHDR *pNMHDR, LRESULT *pResult);
	//�һ�IM�б�
	afx_msg void OnNMRclickIMList(NMHDR * pNMHDR, LRESULT * pResult);
	//˫��IM�б�
	afx_msg void OnNMDblclkIMList(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()

	CNormalBitmapButtonEx  m_Close;
	CNormalBitmapButtonEx  m_Add;
	CNormalBitmapButtonEx  m_Set;
	CconfigDlg  m_configDlg; //���ص�ͼ��.
public:
	virtual BOOL OnInitDialog();

	void SetIM(CIMMain* pIM);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	CAddFriend addFriendDlg;
	CSetFriend setFriendDlg;

private:
	CSkinMgr m_skinmgr;

	void Init();
private:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);

	void LoadSkin();
public:
	// ���ô���Բ��2012.10.15 yyf
	void SetWndRgn(void);
};
