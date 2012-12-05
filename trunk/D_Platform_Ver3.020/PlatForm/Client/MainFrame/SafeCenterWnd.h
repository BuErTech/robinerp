#pragma once

#ifndef SafeCenterWnd_HEAD_H
#define SafeCenterWnd_HEAD_H

#include "StdAfx.h"
#include <map>
#include <list>
using namespace std;

#include "SkinMgr.h"
class CSafeCenterWnd;

typedef bool (CSafeCenterWnd::*lpOperateFun)(LPCVOID);	//�����Ŀ������׼����ָ������

class CSafeCenterWnd : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CSafeCenterWnd();
	virtual ~CSafeCenterWnd(void);

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

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	void OnOK();

public:

	//���س���ؼ�Ƥ��
	void LoadNormalCtrlSkin();

	//����û���Ϣ�Ƿ�Ϊ��ȫ������falseΪ����ȫ������trueΪ��ȫ
	static bool CheckSafe(const MSG_GP_UserInfo* pUserInfo);

//��ȫ�����Ŀ�Ĳ�������
public:

	//�������ò���
	bool OnOperateLockMachine(LPCVOID lpData);

	//�����֤����
	bool OnOperateSetIDVerify(LPCVOID lpData);

	//�ֻ�����֤����
	bool OnOperateBindMobileVerify(LPCVOID lpData);

private:

	//����Combol���б���
	void LoadComboItems(BzDui::CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect = 0);

	//�����ı�����ѡ���б���
	bool SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText);

public:

	BzDui::CPaintManagerUI m_pm;

	//�û���Ϣ
	MSG_GP_UserInfo* m_pUserInfo;

	//һ�������Ŀ
	struct TCheckNode
	{
		std::string name;    //��Ŀ��
		std::string opText;	 //�����ı���������ť�ϵ��ı�
		std::string norText; //��̬�ı���������ť�ϵ��ı������ܵ����
		std::string bntName; //�󶨵Ĳ�����ť��
		lpOperateFun OpFun;	 //����ִ�к���
		bool bOperated;		 //�û��Ƿ�����ɸ���Ŀ������
	};

	enum CheckItemType
	{
		CIT_LockMachine = 0, //�������
		CIT_IDVerify	= 1, //���֤���
		CIT_BindMobile	= 2, //�ֻ��󶨼��
	};

	map<CheckItemType,TCheckNode> m_mapSCTable;

public:

	//����ĳһ��ȫ��״̬
	void SetItemSafeState(CheckItemType emType,bool bSafeType);

//�ؼ�����
protected:

public:

	//1��ʾȷ����0��ʾȡ��
	int m_nRetFlag;							

private:
	CSkinMgr m_skinmgr;
};





#endif
/*******************************************************************************************************/