#pragma once
#ifndef GETPOINT_HEAD_H
#define GETPOINT_HEAd_H

#include "PublicDefine.h"
#include "AFCImage.h"
#include "AFCButton.h"

// CGetPoint �Ի���

class EXT_CLASS  CGetPoint : public CDialog
{
	DECLARE_DYNAMIC(CGetPoint)

public:
	CGetPoint(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGetPoint();
	int		m_SerPoint;
	CWnd			* m_pSend;
	int		m_iInWhere;					//0����,1����

	HBRUSH m_bkBrush;

// �Ի�������

#ifndef IDD_GETPOINT
	#define IDD_GETPOINT 2068
#endif
	//enum { IDD = IDD_GETPOINT };

protected:

	CGameImage	m_bkimage;	

	CNormalBitmapButtonEx	m_btcancel;
	CNormalBitmapButtonEx	m_btok;



	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnPaint();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnBnClickedOk();
public:
	void SetPoint(DWORD dwPoint);
};
#endif