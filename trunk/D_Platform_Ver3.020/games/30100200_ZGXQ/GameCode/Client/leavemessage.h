#pragma once

#include "stdafx.h"
#include "Resource.h"
#include "TransparentStatic.h"

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
	CWnd *m_pParent;
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	//ȡ������
	virtual void OnCancel();
	//ȷ������
	virtual void OnOK();
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	CTransparentStc m_LeaveNameC;
};
