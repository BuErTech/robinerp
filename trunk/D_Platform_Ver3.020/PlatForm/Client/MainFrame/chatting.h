#pragma once
#ifndef CHATTING_HEAD_H
#define CHATTING_HEAD_H

#include "StdAfx.h"
#include "Resource.h"
//#include "afcdialog.h"
//#include "afcWeb.h"
//��Ϸ��Ϣ��
class CChattingWnd : public CDialog
{
	//��������
protected:
	CGameImage				m_bk;
	CIEClass				m_WebView;						// ���
	int						m_iWidth,m_iHeight;				//�߿�
	CString					m_URL;							//����
	enum					{ IDD = IDD_CHATTING };			//�Ի�������
	//��������
public:
	CWnd *m_pParnet;
	//���캯��
	CChattingWnd();
	//��������
	virtual ~CChattingWnd();

	virtual BOOL OnInitDialog();

	//��Ϣ����
public:
	//�ػ�����
	afx_msg void OnPaint();
	
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);

	DECLARE_MESSAGE_MAP()
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//������ҳ����
	void UpdateIE();
public:
	afx_msg void OnClose();
};

#endif