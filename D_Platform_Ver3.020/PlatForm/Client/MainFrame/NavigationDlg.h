#pragma once

#ifndef NAVIGATIONDLG_HEAD_H
#define NAVIGATIONDLG_HEAD_H

#include "StdAfx.h"
#include "GamePlace.h"

//�궨��
#define NAVIGATE_DLG_HIGH							52

//��˵��
class CNavigationDlg;

/*******************************************************************************************************/

//��ť����
class CNavButtonInfo
{
	//��Ԫ����
	friend class CNavigationDlg;

	//��������
protected:
	UINT								m_uButtonHigh;						//��ť�߶�
	UINT								m_uButtonWide;						//��ť���
	CNormalBitmapButtonEx						* m_pPicButton;						//��ťָ��

	//��������
protected:
	//���캯��
	CNavButtonInfo() { m_uButtonHigh=0; m_uButtonWide=0; m_pPicButton=NULL; };
	//��������
	~CNavButtonInfo() { delete m_pPicButton; };
};

/*******************************************************************************************************/

//����������
class CNavigationDlg : public CGameFaceGo
{
	//��������
public:
	CWnd								* m_pParentWnd;						//������
	CGameImage							m_BackImage;						//����ͼ
	CPtrArray							m_ButtonPtrArray;					//��ťָ��

	//��������
public:
	//���캯��
	CNavigationDlg(CWnd * pParentWnd);
	//��������
	virtual ~CNavigationDlg();

	DECLARE_MESSAGE_MAP()

	//���غ���
public:
	//���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK() { return; }
	//ȡ������
	virtual void OnCancel() { return; }

	//���ܺ���
public:
	//��ȡ���ڸ߶�
	inline UINT GetWindowHigh() { return NAVIGATE_DLG_HIGH; };
	//���밴ť
	bool AddButton(UINT uButtonWide, UINT uButtonHigh, HINSTANCE hInstance, UINT uButtonID);
	//����λ��
	void FixControlSize();

	//��Ϣ����
public:
	//�ػ�����
	afx_msg void OnPaint();
};

#endif
/*******************************************************************************************************/