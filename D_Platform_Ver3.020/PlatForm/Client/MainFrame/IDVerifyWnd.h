#pragma once

#ifndef IDVerifyWnd_HEAD_H
#define IDVerifyWnd_HEAD_H

#include "StdAfx.h"
#include "IDKeyBoardWnd.h"
#include "..\uilibrary2003\VerifyCodeCreater.h"
#include "SkinMgr.h"

class CIDVerifyWnd : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CIDVerifyWnd();
	virtual ~CIDVerifyWnd(void);

	LPCTSTR GetWindowClassName() const;

	UINT GetClassStyle() const;

	void OnFinalMessage(HWND hWnd);

	void Init();

	void Notify(BzDui::TNotifyUI& msg);

	void CenterWindow();

public:

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	//������̰���
	LRESULT OnVirtualKey(UCHAR uc,UINT nType);


	void OnOK();

public:

	//���س���ؼ�Ƥ��
	void LoadNormalCtrlSkin();

private:

	//����Combol���б���
	void LoadComboItems(BzDui::CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect = 0);

	//�����ı�����ѡ���б���
	bool SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText);

public:

	BzDui::CPaintManagerUI m_pm;

//�ؼ�����
protected:

	//���֤�����
	BzDui::CEditUI* m_pIDCardEdt;

	//��֤�������
	BzDui::CEditUI* m_pVCodeEdt;

	//���ݼ��̴���
	CIDKeyBoardWnd* m_pIDKeyBoardWnd;

	//��֤��������
	CVerifyCodeCreater* m_pVCCreater;

public:

	//��֤��Ϣ
	MSG_GP_R_IDVerify* m_pVerifyInfo;

	//1��ʾȷ����0��ʾȡ��
	int m_nRetFlag;							

private:
	CSkinMgr m_skinmgr;
};





#endif
/*******************************************************************************************************/