#pragma once

#include "StdAfx.h"
#include "Resource.h"
//#include "ResultListCtrl.h"
#include "afxcmn.h"

//#include <gdiplus.h>
//using namespace Gdiplus;
//#pragma comment(lib, "gdiplus.lib")

//#include "gameframedlg.h"
//��Ϸ��Ϣ��
class CGameFinishDlg : public CWnd
{
	//��������
protected:
	GameFinishNotify	m_iFinish;
	CGameImage			m_bk;
	CGameImage          m_win;
	CGameImage          m_lost;
    CGameImage          m_equal;
	int                 m_iCx;
	int                 m_iCy;
	BYTE				m_iViewStation;				//�Ƿ�Ϊ�����
	UserItemStruct					* m_pUserItem[MAX_PEOPLE];
	CString             m_ShapeName[15];
	//enum					{ IDD = IDD_GAME_END };			//�Ի�������
	int                 m_nPowerOfGold;   ///��ұ���

	//��������
public:
	CWnd *m_pParnet;
	//���캯��
	CGameFinishDlg();
	//��������
	virtual ~CGameFinishDlg();

	void SetMessage(GameFinishNotify finishnotify,BYTE iViewStation=0,int nPowerOfGold =0);
	
protected:
public:
	//�ػ�����
	afx_msg void OnPaint();

	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	/// ����ǰ��ұ��ʵõ���ʾ�ַ���
	void GetNumString(__int64 nNum, CString &Str, int nPower);
};
