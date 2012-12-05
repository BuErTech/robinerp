#pragma once

#ifndef MiniPopWnd_HEAD_H
#define MiniPopWnd_HEAD_H

#include "StdAfx.h"

class CMiniPopWnd : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CMiniPopWnd(CWnd* pParent);
	virtual ~CMiniPopWnd(void);

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

	bool OnNetMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);

public:

	//���س���ؼ�Ƥ��
	void LoadNormalCtrlSkin();

private:

	//����Combol���б���
	void LoadComboItems(BzDui::CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect = 0);

	//�����ı�����ѡ���б���
	bool SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText);

public:

	//���õ�����Ϣ
	void SetMessage(const TMailItem* pPopWndMsg);

	//�ƶ����ڵ����½�
	void MoveWindowToRightBottom();

public:

	BzDui::CPaintManagerUI m_pm;

//�ؼ�����
protected:

public:

	//1��ʾȷ����0��ʾȡ��
	int m_nRetFlag;			

private:

	//������ָ��
	CWnd* m_pRoom;

	//������Ϣ
	TMailItem m_tagMailMsg;

	//͸���ȱ仯ֵ
	int m_iTransparent;

};





#endif
/*******************************************************************************************************/