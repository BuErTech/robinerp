#pragma once

#ifndef FinanceWnd_HEAD_H
#define FinanceWnd_HEAD_H

#include "StdAfx.h"

class CFinanceWnd : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CFinanceWnd();
	virtual ~CFinanceWnd(void);

	LPCTSTR GetWindowClassName() const;

	UINT GetClassStyle() const;

	void OnFinalMessage(HWND hWnd);

	void Init();

	void Notify(BzDui::TNotifyUI& msg);

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

	//��ʾ����
	void ShowWindow(bool bShow = true, bool bTakeFocus = true);

	//��ʾ������������
	void ShowFinanceInfo(TMSG_GP_BankFinanceInfo* pFinanceInfo);


//�ؼ�����
protected:

private:

	BzDui::CPaintManagerUI m_pm;

	//����͸��ֵ
	int m_iBkTransparent;

					
};





#endif
/*******************************************************************************************************/