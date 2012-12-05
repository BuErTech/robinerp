#pragma once

#ifndef TopDuWndClass_HEAD_H
#define TopDuWndClass_HEAD_H

#include "StdAfx.h"
#include "QuitAsk.h"
#include "SkinMgr.h"
#include <map>
#include <list>
using namespace std;

class CTopWndClass;
class CGameListWnd;

class CTopDuWndClass : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CTopDuWndClass();
	virtual ~CTopDuWndClass(void);

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

	LRESULT OnExchangeSkin(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnOK();

public:

	void OnHitMin();

	void OnHitMax();

	void OnHitRestore();

	void OnHitClose();

	void OnHitSkin();

	void OnHitYellow();

	void OnHitBlue();
private:

	//���ܰ�ť
	void OnBnClickedBt1();

	void OnBnClickedBt2();

	void OnBnClickedBt3();

	void OnBnClickedBt4();

	void OnBnClickedBt5();

	void OnBnClickedBt6();

	void OnBnClickedBt7();

	void OnBnClickedBt8();

	void OnBnClickedBt9();

	void OnBnClickedBt10();

	void OnBnClickedBt11();

	void OnBnClickedBt21();

	void OnBnClickedBt22();

	void OnBnClickedBt23();

	void OnBnClickedBt24();

	void OnBnClickedBt25();


public:
	void MakeSkinSelUnVisible();

	//���س���ؼ�Ƥ��
	void LoadNormalCtrlSkin();

	//�����رա���ԭ����С����ťλ��
	void FixCtrlBtnPosition(UINT uType,bool bMaxSize);

	//�õ�֪ͨ�Ѿ��յ�flash��ַ
	void SetFlashAd();

	//��ȡbcf�ļ��ж�����ť�Ĵ������������
	int GetTopBtnMaxWidth();

	//��ȡbcf�ļ��ж�����ť�Ĵ�����С�������
	int GetTopBtnMinWidth();
private:
	void ChangeDUISkin();

public:

	BzDui::CPaintManagerUI m_pm;

	//�ϰ汾��TopWnd
	CTopWndClass* m_pOldTopWndClass;

	//��Ϸ�б�
	CGameListWnd* m_pGameListWnd;

public:
	CQuitAsk	m_quitAskdlg;

	static HHOOK m_hMHook;

	static CTopDuWndClass* m_pInstance;

	BzDui::CContainerUI* m_pExSkinCtn;

private:

	CSkinMgr	m_skinmgr;

	BzDui::CButtonUI* m_pYellowBtn;

	BzDui::CButtonUI* m_pBlueBtn;
};





#endif
/*******************************************************************************************************/