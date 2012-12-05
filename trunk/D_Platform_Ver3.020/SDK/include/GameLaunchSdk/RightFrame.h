#pragma once

#ifndef RightFrame_HEAD_H
#define RightFrame_HEAD_H

#include "StdAfx.h"
#include <deque>

class CGameUserListCtrlDu;
class CUseBoardCastWnd;

#define MAX_PROP_NUMS	20

class CRightFrame : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI,
		public BzDui::IListCallbackUI
{

public:

	CRightFrame(CWnd* pParent);
	virtual ~CRightFrame(void);

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

	LRESULT OnCmdUserListMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	bool OnNetMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);

	virtual LPCTSTR GetItemText(BzDui::CControlUI* pControl, int iIndex, int iSubItem);

	//��������
	void ShowWindow(bool bShow = true, bool bTakeFocus = true);

	//������߰�ť
	void OnBnProp(BzDui::CButtonUI* pButton);

	//���ѡ�����
	void OnBnExpression();

	//���û��б�Ĳ˵�
	void OnPopupUserListMenu(POINT pt);

	//�������Ϣ��׼����
	void OnTalkBufferReady(BzDui::CStdString strBuf,int iTextHeight);

public:

	//���س���ؼ�Ƥ��
	void LoadNormalCtrlSkin();

	//�б�ѡ�е��û���Ϣ����ʾ���Ϸ����
	void ShowUserInfo(UserItemStruct* pUser);
	void ShowUserInfo();

	//����б�ѡ�е��û�
	UserItemStruct* GetShowUser();

	//��ȡ���ͷ���logo�ļ�·��

	void GetUserLogoFile(UserItemStruct* pUser,char* sz_out_file);

	//���õ��߰�ťͼƬ
	void OnSetPropImages();	

	//�ض�talkIngame.ini
	void OnReLoadTalkINI();

	//�����������������
	int GetTalkInputMessage(char* szMsg);

	//���ý��������������
	void SetFocusToTalkInput();

	//������������
	void ClearTalkInput();

	//��ӱ��鵽���������
	void AddExpressionToTalkInput(char* szExpression);

	//������Box������Ϣ����
	void InsertTalkMessage(char* szMsgBuf);


private:

	//����Combol���б���
	void LoadComboItems(BzDui::CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect = 0);

	//�����ı�����ѡ���б���
	bool SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText);

	//��ʼ��������
	void InitProp();

	//������UIλ��ƥ��
	void FixPropUI(int cx,int cy);

	//��ʾ����ҳ
	void ShowPropPage(UINT uPage);


public:

	BzDui::CPaintManagerUI m_pm;

//�ؼ�����
public:

	//����б�ؼ�
	CGameUserListCtrlDu* m_pUsersList;

	//�����
	BzDui::CListUI* m_pTalkBoxList;

	//������򻺳��ı�
	BzDui::CTextUI* m_pTalkTempText;
		

private:

	//������ָ��
	CWnd* m_pRoom;

	//�Ƿ�ټ�����Ϸ
	bool m_bBJLGame;

	//�Ƿ������ظô���
	bool m_bCanHide;

	//����Ƿ������˵���
	bool m_bUseKickUserProp;

	//��ǰ��������ʾ��Ϣ���û�
	UserItemStruct* m_pShowUser;

	//���߰�ťȺ
	BzDui::CButtonUI* m_pPropBtn[MAX_PROP_NUMS];

	//����ÿҳ������ť������
	UINT m_uPropPageSize;

	//��ǰ��ʾ���ߵ�ҳ
	UINT m_uCurrPropPage;

	//����ҳ��
	UINT m_uPropPageNums;

	//����͸����
	int m_iTransparent;

public:
	//���ȴ���
	CUseBoardCastWnd* m_pBoardCastWnd;

	//�����������Ϣ
	std::deque<CString> m_dqTalkBuff;

public:

	//���ڵ������
	int m_uMaxWidth;

	//���ڵ���С���
	int m_uMinWidth;

};





#endif
/*******************************************************************************************************/