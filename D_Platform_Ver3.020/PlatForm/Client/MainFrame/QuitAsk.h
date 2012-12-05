#pragma once
#ifndef  QUITASK_HEAD_H
#define QUITASK_HEAD_H

#include "Stdafx.h"
#include "SkinMgr.h"

//��½�Ի�����
class CQuitAsk: public CLoadFileBmp
{
public:
	CGameImage							m_bk;				//����
	CNormalBitmapButtonEx				m_Bt1;				//��ԭ
	CNormalBitmapButtonEx				m_Bt2;
	CNormalBitmapButtonEx				m_Bt3;
	CNormalBitmapButtonEx				m_Bt4;
public:
	//����
	CQuitAsk();
	//����
	~CQuitAsk();
	
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
protected:
	virtual void OnOK();

	virtual void DoDataExchange(CDataExchange* pDX);

	void OnClicked1();

	void OnClicked2();

	void OnClicked3();

	void OnClicked4();
public:
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CSkinMgr m_skinmgr;

	void LoadSkin();
public:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);
};
#endif