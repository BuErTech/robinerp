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
	int                 m_iGameEndX;
	int                 m_iGameEndY;
	int                 m_iGameEndLineSpace;
	int                 m_iGameEndWinLostX;
	int                 m_iGameEndWinLostY;
//	enum					{ IDD = IDD_GAME_END };			//�Ի�������
	//��������
public:
	CWnd *m_pParnet;
	int m_nPowerOfGold;
	//���캯��
	CGameFinishDlg();
	//��������
	virtual ~CGameFinishDlg();
	//�Զ�������С
	void AutoSize(int x,int y);
	//������Ϣ
//	void SetMessage(TCHAR * szMessage);
	void SetMessage(GameFinishNotify finishnotify,BYTE iViewStation,bool bInVideoRoom);
	//void SetShow(BOOL bShow = TRUE);
	void GetNumString(int nNum, CString &Str, int nPower);
	//��Ϣ����
public:
	//�ػ�����
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnMove(int x, int y);
};
