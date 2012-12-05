#pragma once

#ifndef MobileVerifyWnd_HEAD_H
#define MobileVerifyWnd_HEAD_H

#include "StdAfx.h"
#include "SkinMgr.h"

class CMobileVerifyWnd : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CMobileVerifyWnd();
	virtual ~CMobileVerifyWnd(void);

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

	//����������ȡ��֤��Ķ�ʱ��
	void ResetReVCodeUI();

	//��ȡ��֤�����Чʱ��
	static int GetVCodeValidTime();

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
	BzDui::CEditUI* m_pMobileEdt;

	//��֤�������
	BzDui::CEditUI* m_pVCodeEdt;

	//��ȡ�ֻ���֤��ť
	BzDui::CButtonUI* m_pReVCodeBtn;

public:

	//��֤��Ϣ
	MSG_GP_R_MobileVerify* m_pVerifyInfo;

	//1��ʾȷ����0��ʾȡ��
	int m_nRetFlag;		

private:

	//��ȡ�ֻ���֤���ʣ��ʱ��
	int m_iReVCodeTime;	

	CSkinMgr m_skinmgr;
};





#endif
/*******************************************************************************************************/