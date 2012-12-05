#pragma once

#ifndef UserTabDuWnd_HEAD_H
#define UserTabDuWnd_HEAD_H

#include "StdAfx.h"
using namespace std;

#include "SkinMgr.h"
class CGameListWnd;

class CUserTabDuWnd : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CUserTabDuWnd();
	virtual ~CUserTabDuWnd(void);

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

	LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	void OnOK();

private:
public:

	//���س���ؼ�Ƥ��
	void LoadNormalCtrlSkin();

	//�����رա���ԭ����С����ťλ��
	void FixCtrlPosition(UINT uType,bool bMaxSize);

public:

	BzDui::CPaintManagerUI m_pm;

	//�ϰ汾��TopWnd
	CGameListWnd* m_pGameListWnd;

	CSkinMgr m_skinmgr;
};





#endif
/*******************************************************************************************************/