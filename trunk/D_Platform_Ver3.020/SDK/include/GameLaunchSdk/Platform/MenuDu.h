#pragma once

#ifndef MenuDu_HEAD_H
#define MenuDu_HEAD_H

#include "StdAfx.h"
using namespace std;

class CMenuDu : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CMenuDu(HWND hParent,UINT uID);

	virtual ~CMenuDu(void);

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

	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

public:

	//������ʾλ��
	void SetShowPoint(POINT pt);

	//��Ӳ˵���
	void AddMenuItem(char* szName,UINT uMsgID);

	//��ӷָ���
	void AddMenuSeparator();

protected:

	//�����˵����ڵ�λ��
	void FixPosition();

public:

	//���س���ؼ�Ƥ��
	void LoadNormalCtrlSkin();

public:

	BzDui::CPaintManagerUI m_pm;

private:

	//�����ڵľ��
	HWND m_hParent;

	//�˵�ID
	UINT m_uID;

	//�˵���ʾ����ʼ������
	POINT m_ptPos;

	//�б�ؼ�
	BzDui::CListUI* m_pListCtrl;

	struct TMenuNode
	{
		char szName[32];
		UINT uMsgID;		//��Ϣ
		TMenuNode()
		{
			::memset(szName,0,sizeof(szName));
			uMsgID = 0;
		}
	};

	//�˵���
	std::vector<TMenuNode> m_vtMenu;

	


};





#endif
/*******************************************************************************************************/