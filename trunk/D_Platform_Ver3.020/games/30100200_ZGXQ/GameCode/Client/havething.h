#pragma once

#include "Stdafx.h"
#include "Resource.h"

//�����뿪�Ի�����
class CHaveThing : public CGameFaceGo
{
//	DECLARE_DYNAMIC(CHaveThing)

	//��������
public:
	CNormalBitmapButtonEx				m_btOk;							//ȷ����ť
	CNormalBitmapButtonEx				m_btCancel;						//ȡ����ť
	//CButton		m_btOk;
	//CButton		m_btCancel;
	enum					{ IDD = IDD_THING };			//�Ի�������
	CWnd *m_pParnet;
	CGameImage			m_bk;
	bool	isclick;
	//��������
public:
	//���캯��
	CHaveThing();   
	//��������
	virtual ~CHaveThing();
	//��ʼ������
	virtual BOOL OnInitDialog();
	virtual BOOL ShowWindow(int nCmdShow);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnSetfocusEdit();
protected:
	//ȡ������
	virtual void OnCancel();
	//ȷ������
	virtual void OnOK();
	// DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
