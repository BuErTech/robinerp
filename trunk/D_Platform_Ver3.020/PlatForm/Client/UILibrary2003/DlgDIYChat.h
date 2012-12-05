#pragma once

#ifndef DLGDIYCHAT_HEAD_H
#define DLGDIYCHAT_HEAD_H

#include "afxwin.h"
#include "AFCImage.h"
#include "AFCButton.h"
#include "CconfigDlg.h" 

// CDlgDIYChat �Ի���

class EXT_CLASS CDlgDIYChat : public CDialog
{
	DECLARE_DYNAMIC(CDlgDIYChat)

public:
	CDlgDIYChat(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDIYChat();
	CListBox *m_listBox;
	//��ʼ������
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	int m_GameOrHall;
	CGameImage		m_bkimage;
	// �Ի�������
	//enum { IDD = IDD_DIY_CHAT };
	CNormalBitmapButtonEx	m_btcancel;
	CNormalBitmapButtonEx	m_btOk;
	CNormalBitmapButtonEx	m_btDelete;
	CNormalBitmapButtonEx	m_btModify;
	CNormalBitmapButtonEx	m_btAdd;

	HBRUSH m_bkBrush;
	CconfigDlg  m_configDlg; //���ص�ͼ��. 2012.10.17 yyf
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
public:
	//���ܺ���
	void ReadTalkINI();
	void WriteTalkINI();

	//��Ϣ��Ӧ����
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedAdd();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnDblclkListChat();
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	CString m_EditSzBuffer;
private:
	void ChangeSkin(const CString& strFileName,const CString& strSkinKey,const CString& strSkinPath);

public:
	void OnExchangeSkin(const CString& strFileName,const CString& strSkinKey,const CString& strSkinPath);
   
	  // ���ô���Բ�� 2012.10.17 yyf
	  void SetWndRgn(void);
};

#endif