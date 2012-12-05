#pragma once

#include "StdAfx.h"
#include "Resource.h"
class CShowNote : public CDialog
{
	//�ؼ�����
public:	
	CGameImage			m_bk;								//����ͼ
//	CBitmapButton		m_BmShow;							//��ť
//	CBitmapButton		m_BmNote;							//��ť
//	CBitmapButton		m_BmAdd;							//��ť
//	CBitmapButton		m_BmFollow;							//��ť
//	CBitmapButton		m_BmGiveup;							//��ť
	CNormalBitmapButtonEx       m_BmShow;							//����ť
	CNormalBitmapButtonEx		m_BmNote;							//�°�ť
	CNormalBitmapButtonEx		m_BmAdd;							//�Ӱ�ť
	CNormalBitmapButtonEx		m_BmFollow;							//����ť
	CNormalBitmapButtonEx		m_BmGiveup;	                        //��
	CNormalBitmapButtonEx	    m_BmLook;                           //��
	CWnd				*m_pParent;
	
	enum				{ IDD = IDD_SHOW_NOTE };			//ID ����

	//��������
public:
	//���캯��
	CShowNote(CWnd * pParent = NULL);
	//�����б�
	void UpdateShowNoteBtn(BOOL show=false,BOOL note=false,BOOL add=false,BOOL follow=false,BOOL giveup=false,BOOL look=true);
	//������ע
	void SetShowNoteBtn(BOOL show=false,BOOL note=false,BOOL add=false,BOOL follow=false,BOOL giveup=false,BOOL look=true);
protected:
	//DDX ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//��Ϣ����
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//��ť��Ϣ
	afx_msg void OnNote();
	//��ť��Ϣ
	afx_msg void OnShow();
	//��ť��Ϣ
	afx_msg void OnFollow();
	//��ť��Ϣ
	afx_msg void OnGiveup();
	//��ť��Ϣ
	afx_msg void OnAdd();
	afx_msg void OnLook();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void DrawBk(CDC * pDC, int x, int y);
	DECLARE_MESSAGE_MAP()
};

