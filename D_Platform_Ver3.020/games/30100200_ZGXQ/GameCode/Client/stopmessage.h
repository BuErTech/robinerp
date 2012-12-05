#pragma once

#include "stdafx.h"
#include "Resource.h"

//��������뿪��Ϣ
class CStopMessage : public CGameFaceGo
{

	//��������
protected:
	CNormalBitmapButtonEx				m_btOk;							//ȷ����ť
	CNormalBitmapButtonEx				m_btCancel;						//ȡ����ť
	enum					{ IDD = IDD_STOP_MESSAGE };
	CGameImage							m_bk;

	//��������
public:
	//���캯��
	CStopMessage();
	//��������
	virtual ~CStopMessage();
	//��ʼ������
	virtual BOOL OnInitDialog();
	CWnd *m_pParent;

protected:
	//ȡ������
	virtual void OnCancel();
	//ȷ������
	virtual void OnOK();
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL ShowWindow(int nCmdShow);
};
