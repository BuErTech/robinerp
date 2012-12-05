#pragma once
#ifndef CHECKPSW_HEAD_H
#define CHECKPSW_HEAD_H

#include "resource.h"
#include "Platform/AFCImage.h"
#include "Platform/AFCButton.h"
#include "MD5.h"
#include "sha1.h"
#include "SkinMgr.h"

//��½�Ի�����
class CCheckPSW: public CDialog
{
	DECLARE_DYNAMIC(CCheckPSW)
public:
	//����
	CCheckPSW(CWnd *pWndParent);
	//����
	~CCheckPSW();
public:
	char							m_szScrPass[61];					//�û�ԭ����
	char							m_szMD5Pass[50];					//���ܺ������
	CGameImage							m_bk;				//����
	CNormalBitmapButtonEx				m_BtCancel;				//��ԭ
	CNormalBitmapButtonEx				m_BtOk;

	
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);

private:
	CSkinMgr m_skinmgr;
};

#endif