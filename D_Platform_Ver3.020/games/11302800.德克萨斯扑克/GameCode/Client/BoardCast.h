#pragma once

#include "resource.h"
#include "Platform/AFCImage.h"
#include "Platform/AFCButton.h"
// CBoardCast �Ի���

class CBoardCast : public CDialog
{
	DECLARE_DYNAMIC(CBoardCast)

public:
	CBoardCast(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBoardCast();
public:
	CGameImage				m_BackgroundImage3;	//С���Ƚ���
	CGameImage				m_BackgroundImage2;	//�����Ƚ���
	CNormalBitmapButtonEx	m_btCancel;
	CNormalBitmapButtonEx	m_btOk;
	CNormalBitmapButtonEx	m_btChangeColor;
	CWnd					*pParentWnd;
	int						m_nPropID;

	CString					m_strInput;
	UINT					m_uiInputLen;
	int						m_iBroadcastStyle;						//��������
	COLORREF				m_RGB;

// �Ի�������
	//enum { IDD = IDD_BOARDCAST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


	DECLARE_MESSAGE_MAP()
	afx_msg void OnChangeColor();

	virtual void OnOK();
public:
	afx_msg void OnPaint();

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnChangeEditInput();
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	void SetValue(int iBroadcastMsg);
	//����ɫ
	COLORREF GetDuduRGB();
};
