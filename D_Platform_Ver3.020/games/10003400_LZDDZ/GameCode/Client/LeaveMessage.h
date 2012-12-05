#pragma once

#include "stdafx.h"
#include "Resource.h"

//��������뿪��Ϣ
class CLeaveMessage : public CGameFaceGo
{

	//��������
protected:
	CNormalBitmapButtonEx				m_btOk;							//ȷ����ť
	CNormalBitmapButtonEx				m_btCancel;						//ȡ����ť
//	CTransparentStatic					m_Name;
	enum					{ IDD = IDD_THING_MESSAGE };
	CGameImage				m_bk;										//����ͼ

	//��������
public:
	//���캯��
	CLeaveMessage();
	//��������
	virtual ~CLeaveMessage();
	//������Ϣ
	void SetLeaveMessage(TCHAR * szMessage, TCHAR * szUserName);
	//��ʼ������
	virtual BOOL OnInitDialog();
	BOOL ShowWindow(int nCmdShow);

public:
	CWnd *m_pParent;
	
protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//ȡ������
	virtual void OnCancel();
	//ȷ������
	virtual void OnOK();
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
};
