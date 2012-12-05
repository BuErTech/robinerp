#pragma once
#ifndef USERINFOVIEW_HEAD_H
#define USERINFOVIEW_HEAD_H

#include "StdAfx.h"
#include "Afxcmn.h"

//�û���Ϣ��
class CUserInfoView : public CGameFaceGo
{
	//��������
protected:
	GetOrderName						* m_pOrderName;						//�ȼ�����

	//�ؼ�����
public:
	CIEHelper							m_FlashView;						//FLASH ����
	CNormalBitmapButtonEx						m_BtCancel;							//�رհ�ť
	CNormalBitmapButtonEx						m_BtHighInfo;						//��ϸ��Ϣ
	CNormalBitmapButtonEx						m_BtUpdateInfo;						//��������

	//��������
public:
	//���캯��
	CUserInfoView(GetOrderName * pOrderName);
	//��������
	virtual ~CUserInfoView();

	//���غ���
public:
	//���ݰ�
	virtual void DoDataExchange(CDataExchange * pDX);    
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ��
	virtual void OnOK() { DestroyWindow(); };
	//ȡ��
	virtual void OnCancel() { DestroyWindow(); };

	//���ܺ���
public:
	//�����û���Ϣ
	void SetUserInfo(UserItemStruct * pUserItem);
	//����λ��
	void FixControlSize(int iWidth, int iHeight);

	DECLARE_MESSAGE_MAP()

	//��ť��Ϣ
public:
	//���°�ť
	void OnBnClickedUpdate();
	
	//��Ϣ����
public:
	//�滭��Ϣ
	afx_msg void OnPaint();
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#endif