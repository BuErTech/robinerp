#pragma once
#ifndef CHECKPSW_HEAD_H
#define CHECKPSW_HEAD_H

#include "Stdafx.h"

//��½�Ի�����
class CCheckPSW: public CLoadFileBmp
{
public:
	static char							m_szScrPass[61];					//�û�ԭ����
	static char							m_szMD5Pass[50];					//���ܺ������
	CGameImage							m_bk;				//����
	CNormalBitmapButtonEx				m_BtCancel;				//��ԭ
	CNormalBitmapButtonEx				m_BtOk;

public:
	//����
	CCheckPSW();
	//����
	~CCheckPSW();
	
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
};

#endif