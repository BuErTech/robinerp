#pragma once

#include "StdAfx.h"
#include "Resource.h"
//��Ϸ��Ϣ��
class CGameFinishDlg : public CWnd
{
	//��������
protected:
	//CFont				m_Font;						//��ʾ����
//	CString				m_strMessage;				//�ַ���Ϣ
//	CBitmap				m_BackBmp;					//����
	GameFinishNotify	m_iFinish;
	CGameImage			m_bk;
	CGameImage			m_Flag;						//���
	BYTE				m_iViewStation;				//�Ƿ�Ϊ�����
//	enum					{ IDD = IDD_GAME_END };			//�Ի�������
	//��������
public:
	CWnd *m_pParnet;
	//���캯��
	CGameFinishDlg();
	//��������
	virtual ~CGameFinishDlg();
	//�Զ�������С
	void AutoSize(int x,int y);
	//������Ϣ
//	void SetMessage(TCHAR * szMessage);
	void SetMessage(GameFinishNotify finishnotify,BYTE iViewStation=0);
	//void SetShow(BOOL bShow = TRUE);
	//��Ϣ����
public:
	//�ػ�����
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	void SetIntToCharLjl(CString  &str, __int64 iMoney, int iSet);

	int m_iPower; 
	void SetPower(int iPower=0)
	{
		m_iPower = iPower;
	}
};
