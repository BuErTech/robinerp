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
	CGameImage			                m_bk;			                //����ͼ

	//��������
public:
	//���캯��
	CStopMessage(CWnd * pParent=NULL);
	//��������
	virtual ~CStopMessage();
	CWnd					 *m_pParent;

protected:
	//ȡ������
	virtual void OnCancel();
	//ȷ������
	virtual void OnOK();
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    
	//��ʼ������
	virtual BOOL OnInitDialog();
	//void AutoSize();

	//�Զ�������С
	void AutoSize(int x,int y);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
};
