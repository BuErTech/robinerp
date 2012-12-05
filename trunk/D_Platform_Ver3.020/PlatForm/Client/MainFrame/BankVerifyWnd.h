#pragma once

#ifndef BankVerifyWnd_HEAD_H
#define BankVerifyWnd_HEAD_H

#include "StdAfx.h"
#include "SkinMgr.h"
#include "..\uilibrary2003\VerifyCodeCreater.h"

class CBankVerifyWnd : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CBankVerifyWnd();
	virtual ~CBankVerifyWnd(void);

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
	BzDui::CEditUI* m_pPwdEdt;

	//��֤�������
	BzDui::CEditUI* m_pVCodeEdt;

	//��֤��������
	CVerifyCodeCreater* m_pVCCreater;

public:

	//��֤��Ϣ
	MSG_GR_UserBank* m_pVerifyInfo;

	//�û���Ϣ
	SendMsgUserStruct* m_pMyInfo;

	//��֤��ı�����ɫ
	COLORREF m_crVCodeBk;

	//1��ʾȷ����0��ʾȡ��
	int m_nRetFlag;

private:
	//����
	CSkinMgr m_skinmgr;
};





#endif
/*******************************************************************************************************/