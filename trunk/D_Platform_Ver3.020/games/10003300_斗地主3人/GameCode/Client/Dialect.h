#pragma once
#include"Resource.h"


// CDialect �Ի���

class CDialect : public CDialog
{
	DECLARE_DYNAMIC(CDialect)

public:
	CDialect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialect();

// �Ի�������
	enum { IDD = IDD_DIALECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	//��ʼ���Ի���
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	

public:

	CGameImage				  m_bk;			//����ͼ
	CNormalBitmapButtonEx     m_btOk ;          //ȷ����ť

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
};
