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
	enum					{ IDD = IDD_THING_MESSAGE };
	CGameImage					m_bk;
	//��������
public:
	//���캯��
	CLeaveMessage();
	//��������
	virtual ~CLeaveMessage();
	//������Ϣ
	void SetLeaveMessage(TCHAR * szMessage, TCHAR * szUserName);
	CWnd *m_pParent;

protected:
	//ȡ������
	virtual void OnCancel();
	//ȷ������
	virtual void OnOK();
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	virtual BOOL OnInitDialog();
};
