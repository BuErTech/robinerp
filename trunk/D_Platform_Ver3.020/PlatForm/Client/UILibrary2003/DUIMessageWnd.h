#pragma once

#ifndef MessageWndDu_HEAD_H
#define MessageWndDu_HEAD_H

#include "StdAfx.h"

class CMessageWndDu : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CMessageWndDu();
	virtual ~CMessageWndDu(void);

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

	//���ô��ڱ���
	void SetTitle(const char* szText);

	//�����ı�
	void SetMainText(const char* szText,bool bFormat);

	//���ð�ť
	void SetStyle(UINT uStyle);

	//��ʾ����
	void ShowWindow(bool bShow = true, bool bTakeFocus = true);

public:

	//����ֵ
	UINT m_uRetFlag;


//�ؼ�����
protected:

private:

	BzDui::CPaintManagerUI m_pm;

	//����͸��ֵ
	int m_iBkTransparent;

	//�Ի�����ʽ
	UINT m_uStyle;

					
};





#endif
/*******************************************************************************************************/