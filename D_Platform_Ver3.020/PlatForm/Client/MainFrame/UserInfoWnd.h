#pragma once

#ifndef USERINFOWND_HEAD_H
#define USERINFOWND_HEAD_H

#include "StdAfx.h"
#include <map>
#include <vector>
using namespace std;

class CUserInfoWnd : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CUserInfoWnd();
	virtual ~CUserInfoWnd(void);

	LPCTSTR GetWindowClassName() const;

	UINT GetClassStyle() const;

	void OnFinalMessage(HWND hWnd);

	void Init();

	void Notify(BzDui::TNotifyUI& msg);

	//�����û�����
	void SetPlaceUserInfo(MSG_GP_UserInfo* pUserInfo);

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

public:

	//���س���ؼ�Ƥ��
	void LoadNormalCtrlSkin();

	//��ʾ����ҳ��
	void ShowPage(int nIndex,int nSubIndex = 0);

	//��ʼ��������Ϣҳ��
	void InitBaseInfo();

	//��ʼ����ϸ��Ϣҳ��
	void InitDetail();

	//��ʼ��������Ϣҳ��
	void InitLockPCInfo();

	//��ʼ���ֻ���ҳ��
	void InitBindMobile();

	//��ʼ��ʡ�ݵ�����ϵ
	static void InitAreaMap();

	//��ȫ���������ϵ
	static void SafeClearAreaMap();

	///
	/// ��ʾ
	/// @param pToCtrl ��ĳһ�ؼ�����ʾ
	/// @param lpSecName ClientMessage.bcf�еĶ���
	/// @param lpKeyName ClientMessage.bcf�еļ�ֵ
	/// @param lpDefault ClientMessage.bcfû���ҵ���ֵʱ��ʹ�õ�Ĭ��ֵ
	/// @return void
	///
	static void ShowTipMessage(BzDui::CControlUI* pToCtrl,LPCTSTR lpSecName,LPCTSTR lpKeyName,LPCTSTR lpDefault,...);


private:

	//����Combol���б���
	void LoadComboItems(BzDui::CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect = 0);

	//�����ı�����ѡ���б���
	bool SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText);

	//�ѳ������ַ�����ʼ��vector��
	static vector<LPSTR>* PutInVector(vector<LPSTR>* vec,LPSTR pszCitys);

	//���³����б�
	void UpdateCitys(int nProvIdx);

public:

	//�����û��Ļ�����Ϣ
	void OnUpdateBaseInfo();

	//�����û�����ϸ��Ϣ
	void OnUpdateDetailInfo();

	//��������ֻ���
	void OnModifyMobileOpen();

	//ȡ�������ֻ���
	void OnModifyMobileCancel();

	//ȷ�������ֻ���
	void OnModifyMobileOK();

	//��������/��������
	void OnLockOperatePage(bool bLock);

	//�����ֻ����󶨵���
	void OnBindMobileOperatePage(bool bLock);

	//����/������Ť��Ӧ
	void OnLockPC(bool bLock);

	//����/������Ϣ
	void OnLockResult(bool bLock);

	//���ֻ����밴Ť��Ӧ
	void OnBindMobile(bool bLock);

	//���ֻ�������Ϣ
	void OnBindMobileResult(bool bLock);

	//�����û�����
	void OnUpdatePassword();

	//�����ȡ��֤��İ�ť
	void OnGetMobileVCode(UINT uType,BzDui::CButtonUI* pBtn,BzDui::CEditUI* pMobileNoEdt,BzDui::CEditUI* pVCodeEdt,UINT uTimerID);

	//����������ȡ��֤��Ķ�ʱ��
	void ResetReVCodeUI();


public:

	BzDui::CPaintManagerUI m_pm;

	//��ǰʵ�����ھ��
	static HWND m_hInstanceWnd;

private:

	//�û���Ϣ
	MSG_GP_UserInfo* m_pUserInfo;

	//ʡ�ݵ�����ϵ
	static map<char*,vector<char*>> m_mapArea;

public:

	//Ĭ����ʾ��ҳ����
	int m_nDefaultPage;

	//Ĭ����ʾ����ҳ����
	int m_nDefaultSubPage;

//����ؼ�
public:

	//ҳ������
	BzDui::CContainerUI* m_pPageContainer[5];

	//��������ҳ�洰��
	BzDui::CContainerUI* m_pLockMachineCtn[2];

	//�ֻ���ҳ��
	BzDui::CContainerUI* m_pBindMobileCtn[2];

	//��ȡ�ֻ���֤�밴ť1(�޸��û���ϸ��Ϣtabҳ��)
	BzDui::CButtonUI* m_pVCodeBtn1;

	//��ȡ�ֻ���֤�밴ť2(�޸��������õ�tabҳ��)
	BzDui::CButtonUI* m_pVCodeBtn2;

	//��ȡ�ֻ���֤�밴ť3(�ֻ��󶨵�tabҳ��)
	BzDui::CButtonUI* m_pVCodeBtn3;

private:

	//��ȡ�ֻ���֤���ʣ��ʱ��
	int m_iReVCodeTime;	

};


#endif
/*******************************************************************************************************/