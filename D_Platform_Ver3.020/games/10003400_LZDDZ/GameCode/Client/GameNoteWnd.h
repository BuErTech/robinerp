#pragma once

#include "StdAfx.h"

//��Ϸ��Ϣ��
class CGameNoteWnd : public CWnd
{
	//��������
protected:
//	CFont				m_Font;						//��ʾ����
//	CString				m_strMessage;				//�ַ���Ϣ
//	CBitmap				m_BackBmp;					//����
	CNormalBitmapButtonEx			m_btLogo;		//��½
	CGameImage			m_bk;
	int					m_iFlag;									
	unsigned long m_dwUserID;
	//��������
public:
	//���캯��
	CGameNoteWnd();
	//��������
	virtual ~CGameNoteWnd();
	//�Զ�������С
	void AutoSize(int xValue,int yValue);
	//������Ϣ
	void SetMessage(TCHAR * szMessage);

	//��Ϣ����
public:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�ػ�����
	afx_msg void OnPaint();
	void SetGameStateFlag(int iStateFlag = 0);
	DECLARE_MESSAGE_MAP()
};
