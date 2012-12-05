#pragma once
#ifndef LOGOSELECT_HEAD_H
#define LOGOSELECT_HEAD_H
#include "StdAfx.h"
#include "GamePlace.h"

//����ͷ����Ϣ
#define WM_HIT_LOGO_MESSTION		WM_USER+215								//����ͷ����Ϣ

//ͷ��ѡ����
class CLogoSelect : public CDialog
{
	//��������
protected:
	bool							m_bVisible;								//�Ƿ���� 
	int								m_iPos;									//��ǰλ��
	int								m_iWidthCount;							//����Ŀ
	UINT							m_uSelectIndex;							//ѡ������
	HCURSOR							m_hMouseCursor;							//С����״�������

	//��������
public:
	//���캯��
	CLogoSelect();
	//��������
	virtual ~CLogoSelect();
	//ȷ������
	virtual void OnOK() { return; }
	//ȡ������
	virtual void OnCancel();
	//��ʼ������
	virtual BOOL OnInitDialog();
	//��ʾ����
	void SetVisible() { m_bVisible=true; };

	//��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()

	//��������
protected:
	//�滭ͷ��
	void DrawItem(CDC * pDC, UINT uIndex, int iXPos, int iYPos, bool bSelect);
	//��������
	UINT HitItemTest(CPoint MousePoint);

	//��Ϣ����
public:
	//�ػ���Ϣ
	afx_msg void OnPaint();
	//ʧȥ������Ϣ
	afx_msg void OnKillFocus(CWnd * pNewWnd);
	//����ƶ���Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//��갴����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//���ù����Ϣ
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//��������Ϣ
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);
	//��������Ϣ
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//���̰���
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//λ�ñ仯��Ϣ
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};

#endif