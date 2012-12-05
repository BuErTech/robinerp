#pragma once

#ifndef PBank4GameWnd_HEAD_H
#define PBank4GameWnd_HEAD_H

#include "StdAfx.h"

class CPBank4GameWnd : 
		public BzDui::CWindowWnd,
		public BzDui::INotifyUI,
		public BzDui::IMessageFilterUI
{

public:

	CPBank4GameWnd(CWnd* pParent,GameInfoStructInExe* pGameInfo);
	virtual ~CPBank4GameWnd(void);

	LPCTSTR GetWindowClassName() const;

	UINT GetClassStyle() const;

	void OnFinalMessage(HWND hWnd);

	void Init();

	void Notify(BzDui::TNotifyUI& msg);

	void CenterWindow();

	void ShowWindow(bool bShow = true, bool bTakeFocus = true);

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

	//����OK
	void OnInOK();

	//ȡ��OK
	void OnOutOK();

public:

	//���س���ؼ�Ƥ��
	void LoadNormalCtrlSkin();

private:

	//����Combol���б���
	void LoadComboItems(BzDui::CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect = 0);

	//�����ı�����ѡ���б���
	bool SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText);

public:

	/// �������а汾
	void SetBankVersion(int nVer);

	/// ����ȫ�ֱ����еļ��ܷ�ʽ��ȡ�ü��ܺ�������ַ���������sha������MD5
	/// �����ﲻ��ָ��ĺϷ������ж�
	/// @param szMD5Pass ���ܺ���ַ���
	/// @param szSrcPass Դ�ַ���
	/// @return ���ܺ���ַ���ָ��
	static TCHAR* GetCryptedPasswd(TCHAR *szMD5Pass, TCHAR *szSrcPass);

	///
	/// ���òƸ���Ϣ
	/// @param nBank ���д��
	/// @param nWallet �����ֽ�
	/// @return void
	///
	void SetWealthInfor(__int64 i64Bank, __int64 i64Wallet);

	///
	/// ���òƸ���Ϣ
	/// @param nWallet �����ֽ�
	/// @return void
	///
	void SetWealthInfor(__int64 i64Wallet);

	///
	/// ���²Ƹ���Ϣ
	/// @return void
	///
	void UpdateWealthInfor();

	//����������������������ͬ��ת��Ϊ���Ĵ�д
	void ChangeEtToChinese(BzDui::CEditUI* pEditCtrl,BzDui::CTextUI* pTextCtrl);

	///
	/// ��ʾ
	/// @param pToCtrl ��ĳһ�ؼ�����ʾ
	/// @param lpSecName ClientMessage.bcf�еĶ���
	/// @param lpKeyName ClientMessage.bcf�еļ�ֵ
	/// @param lpDefault ClientMessage.bcfû���ҵ���ֵʱ��ʹ�õ�Ĭ��ֵ
	/// @return void
	///
	static void ShowTipMessage(BzDui::CControlUI* pToCtrl,LPCTSTR lpSecName,LPCTSTR lpKeyName,LPCTSTR lpDefault,...);

	//�л���ȡ����
	void OnBnClickedAccessIn();
	void OnBnClickedAccessOut();

	//�������İ�ť��ӽ�Ҵ������
	void AddMoneyToEdtCtrl(BzDui::CEditUI* pEdtUI,__int64 i64Money);

public:

	BzDui::CPaintManagerUI m_pm;

//�ؼ�����
protected:

	//������ñ༭��
	BzDui::CEditUI* m_pCointEdt;

public:

	//1��ʾȷ����0��ʾȡ��
	int m_nRetFlag;

	//��������
	char m_szTwoPassword[50];					

private:

	//������ָ��
	CWnd* m_pRoom;

	//���а汾
	int m_nBankVer;

	//��Ϸ��Ϣ
	GameInfoStructInExe* m_pGameInfo;

	bool bFlag;

	//������Ĵ��
	__int64 m_i64BankMoney;

	//��Ϸ��Ǯ����Ǯ
	__int64 m_i64GameWallet;

};





#endif
/*******************************************************************************************************/