#pragma once

#ifndef PBANKWND_HEAD_H
#define PBANKWND_HEAD_H

#include "StdAfx.h"
#include <map>
using namespace std;

#include "PBankWndEx.h"

class CPBankWnd : 
	public BzDui::CWindowWnd,
	public BzDui::INotifyUI,
	public BzDui::IMessageFilterUI,
	public BzDui::IListCallbackUI
{

public:

	CPBankWnd(CWnd* pParent,bool bInMainRoom = false);
	virtual ~CPBankWnd(void);

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

	virtual LPCTSTR GetItemText(BzDui::CControlUI* pControl, int iIndex, int iSubItem);

	bool OnNetMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

public:

	//���س���ؼ�Ƥ��
	void LoadNormalCtrlSkin();

	//��ʾ����ҳ��
	void ShowPage(int nIndex);

	//��ʼ���Ƹ���Ϣ
	void InitMoneyInfo();

	//��ʼ���Ͳ���UI
	void InitGiftInfo();

	//��ʼ��ȡ�����UI
	void InitAccessInfo();

	//��ʼ����־����UI
	void InitRecordInfo();

	//��ʼ���޸���������UI
	void InitModifyPwdInfo();

	//ת�˽��Сдת��Ϊ���Ĵ�д
	void OnEnChangeEtbconnoin();

	//��ȡ����Сдת��Ϊ���Ĵ�д
	void OnEnChangeMoney();

	//�л�Ŀ���û�����������
	void OnBnClickedRTransId();
	void OnBnClickedRTransNn();

	//�л���ȡ����
	void OnBnClickedAccessIn();
	void OnBnClickedAccessOut();

	///
	/// �ʺ�ת�� ��ȷ����ť��Ӧ����
	/// @param void
	/// @return void
	void OnBnClickedAccountOK();

	//�����û�ID��ȡ�ǳ�,������û��ǳƻ�ȡ�û�ID
	// @param pEdtCtrl ��ID/�ǳ�����ؼ�
	// @param nType �������ͣ���ID��ȡ�ǳ�,�����ǳ��û�ID
	// @param nUserful ��;����ָ������������г��ϣ���ת�ˣ���ֵ�ȡ�
	// @return bool �Ƿ�����ɹ�
	// JianGuankun, 2012-7-30
	bool OnIDKillFocus(BzDui::CEditUI* pEdtCtrl,BYTE nType,BYTE nUserful);

	//�����û�ID/�ǳ�
	void OnKillfocusTargetUser();

	///
	/// ���� ��ȷ����ť��Ӧ����
	/// @param void
	/// @return void
	///
	void OnBnClickedBankOK();

	void OnBnClickedCoinAll();

	void OnBnClickedRecord();

	void RefreshRecordList();

	///
	/// �յ�һ��ת�˼�¼
	/// @param void
	/// @return void
	///
	void OnReceivedTransferRecord(MSG_GR_R_TransferRecord_t *pRecord);

	//��¼������ҳ
	void OnBnClickedRecordFirstPage();

	//��¼����βҳ
	void OnBnClickedRecordRearPage();

	//��¼����һҳ
	void OnBnClickedRecordPreviousPage();

	//��¼����һҳ
	void OnBnClickedRecordNextPage();

	///
	/// �޸����� ��ȷ����ť��Ӧ����
	/// @param void
	/// @return void
	///
	void OnBnClickedChPW();

private:

	//����Combol���б���
	void LoadComboItems(BzDui::CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect = 0);

	//�����ı�����ѡ���б���
	bool SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText);

	//���ת���Ĵ�д
	void ChangeDigitToChinese(__int64 i64Num, CString &strResult);

public:	///< �ⲿ���õĽӿ�
	///
	/// ���òƸ���Ϣ
	/// @param nBank ���д��
	/// @param nWallet �����ֽ�
	/// @return void
	///
	void SetWealthInfor(__int64 i64Bank, __int64 i64Wallet);

	// PengJiLin, 2010-8-23, �����û�ID��ȡ���ǳ�
	// ���û�ȡ���ǳ�
	void SetNickName(TCHAR* szNickName);

	///�����û�ID�ǳƵĲ�ѯ���
	void UpdateNicknameIDResult(GetNickNameID_t* pNID);

	/// ת�ʳɹ��������������
	/// @param Opt 0-��ʾת�������ˣ�1-��ʾ�ɱ���ת����Լ�
	/// @param nMoney ��ʾ�����������Ľ����
	void UpdateUserMoneyByTransfer(int OptType, __int64 i64Money);

	/// ��ȡǮ����½���
	/// @param Opt 0-��ʾȡ����1-��ʾ����
	/// @param nMoney ��ʾ�����������Ľ����
	void UpdateUserMoney(int OptType, __int64 i64Money);

	///ˢ�¼�¼��ҳ��
	void UpdateRecordPageState();

	///
	/// ��ʾ
	/// @param pToCtrl ��ĳһ�ؼ�����ʾ
	/// @param lpSecName ClientMessage.bcf�еĶ���
	/// @param lpKeyName ClientMessage.bcf�еļ�ֵ
	/// @param lpDefault ClientMessage.bcfû���ҵ���ֵʱ��ʹ�õ�Ĭ��ֵ
	/// @return void
	///
	static void ShowTipMessage(BzDui::CControlUI* pToCtrl,LPCTSTR lpSecName,LPCTSTR lpKeyName,LPCTSTR lpDefault,...);

public:

	BzDui::CPaintManagerUI m_pm;

private:

public:

	//Ĭ����ʾ��ҳ����
	int m_nDefaultPage;

	//��ǰʵ�����ھ��
	static HWND m_hInstanceWnd;

//����ؼ�
public:

	//ҳ������
	BzDui::CContainerUI* m_pPageContainer[5];

	// ������ָ��
	CWnd* m_pGameRoom;

	//�����ڵ�����
	enum
	{
		IsMainRoom = 1, //��������MianRoomEx
		ISGameRoom = 2, //��������GameRoomEx
	}m_emParWndType;

	//��¼��ؼ�ָ��
	BzDui::CListUI* m_pRecordList;

	//��ҳ�ؼ���ť
	BzDui::CButtonUI* m_pFirstPageBnt;
	BzDui::CButtonUI* m_pPrePageBnt;
	BzDui::CButtonUI* m_pNextPageBnt;
	BzDui::CButtonUI* m_pRearPageBnt;

private:

	//�û�ID
	DWORD m_dwOpUserID;

	//���е��ֽ�
	__int64 m_i64WalletMoney;	

	//������Ĵ��
	__int64 m_i64BankMoney;

	vector<int> digitArray;
	bool bFlag;

	//��ֵ��Ŀ����ң�ʹ��ID��ʽ����ʹ���ǳƷ�ʽ
	bool m_bUseDestID;

	// PengJiLin, 2010-9-16, �Ƿ�ʹ���ǳ���ʾ
	int m_iUseNickName;
	CButton m_Check_TransAll;

	// PengJiLin, 2010-8-23, �����û�ID��ȡ���ǳ�
	CString m_strNickName;
	bool m_bTransferSuccess; // ת�˳ɹ�

	// PengJiLin, 2010-9-15
	CString m_strSaveID;

	// ��Ǯ����ȡǮ����ǮΪ0��ȡǮΪ1
	UINT m_uCheckInOrOut;
	
	//ת����־��¼��
	std::map<int,MSG_GR_R_TransferRecord_t> m_mapRecord;

	//ת����־��¼����ʾλ��
	int m_iRecDispPos;

	//��¼�༭��ý���ǰ���ַ�������killfocusʱ�����
	BzDui::CStdString m_strFocusTemp;
	

};


#endif
/*******************************************************************************************************/