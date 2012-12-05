#pragma once
#ifndef FINDNULLDESK_HEAD_H
#define FINDNULLDESK_HEAD_H

#include "DeskFrameView.h"
#include "afxwin.h"
#include "SkinMgr.h"
#include "..\UILibrary2003\CconfigDlg.h" 
// CFindNullDesk �Ի���

class CFindNullDesk : public CDialog
{
	DECLARE_DYNAMIC(CFindNullDesk)

public:
	CFindNullDesk(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFindNullDesk();

// �Ի�������
	//enum { IDD = IDD_FINDNULLDESK };
public:
	CGameImage			m_bkimage;
	CDeskFrameView		*m_pDeskFrame;
	CWnd				*m_pGameRoomEx;
	byte				m_bLastFindDesk;
	HBRUSH m_bkBrush;
	CconfigDlg  m_configDlg; //���ص�ͼ��. 2012.10.17 yyf
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	virtual BOOL OnInitDialog();
	
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedSit();
protected:
	virtual void OnCancel();
public:
	CNormalBitmapButtonEx m_btcancel;
	CNormalBitmapButtonEx m_btok;
	CNormalBitmapButtonEx m_btsit;
protected:
	virtual void OnOK();

private:
	CSkinMgr m_skinmgr;

	void Init();
public:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara); 
	  
	// ���ô���Բ�� 2012.10.17 yyf
	 void SetWndRgn(void);
};

#endif