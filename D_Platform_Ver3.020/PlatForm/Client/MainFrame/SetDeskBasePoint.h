#pragma once
#ifndef SETDESKBASEPOINT_HEAD_H
#define SETDESKBASEPOINT_HEAD_H

#include "resource.h"
#include "SkinMgr.h"
// CSetDeskBasePoint �Ի���

class CSetDeskBasePoint : public CDialog
{
	DECLARE_DYNAMIC(CSetDeskBasePoint)

public:
	int										m_iBasePoint;
	CNormalBitmapButtonEx					m_BtOK;								//Ӧ�ð�ť
	CNormalBitmapButtonEx					m_BtCancel;							//ȡ����ť
	CGameImage								m_bk;
	CSetDeskBasePoint(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetDeskBasePoint();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	int GetBasePoint();

// �Ի�������
	enum { IDD = IDD_SETDESKBPDLG };

protected:
	HBRUSH m_bkBrush;
	


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//ȷ������
	virtual void OnOK();
	//��ʼ������
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CSkinMgr m_skinmgr;

public:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);

};

#endif