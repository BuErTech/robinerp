#pragma once

#ifndef PBANKENDEX_HEAD_H
#define PBANKENDEX_HEAD_H

#include "StdAfx.h"
#include <list>
#include <vector>
using namespace std;

/*------------------------------------------------------------------------------------
	����Լ����
	1.��������Tab��Ĵ�ť��Ϊ"���Ͱ�ť";
	2.������Ͱ�ť���г�����item,ÿ��item��Ϊ"������"��
	3.ÿ�����Ͱ�ť����Ӧ����item���ϳ�Ϊ"�����͵Ĺ����б�";
	4.ÿ��������ԭ�����ڴ����ҷ�������һ������������ʹ�ù���������Ҫ�Ŀؼ�,
	  ��Ϊ�ù��ܵ�"��װ����"��

  ----------------------------------------------------------------------------------*/

const int WM_BANK = WM_USER + 600;

// JianGuankun,2012-1-4,�����û�IDȡ�ǳƣ�������ǳ�ȡ�û�ID
const int WM_BANK_GET_NICKNAMEID = WM_USER + 601;

// JianGuankun, 2012-1-4, ����ID��ȡ�ǳ�,���ǳƻ�ȡID
struct GetNickNameID_t
{
	long int _user_id;       //�û�ID
	TCHAR	 _nickname[61];  //�û��ǳ�
	BYTE	 _nType;	     //0����ͨ���û�IDȡ�û��ǳ�\
						       1����ͨ���û��ǳ�ȡ�û�ID
	BYTE	 _nUseful;		 //��;��ʶ				
	GetNickNameID_t()
	{
		ZeroMemory(this, sizeof(GetNickNameID_t));
	}
};


class CPBankWndEx;

typedef void (CPBankWndEx::*lpNotifyFun)(BzDui::TNotifyUI& msg);	//������UI����������Ϣ��Ӧ����

class CPBankWndEx : 
	public BzDui::CWindowWnd,
	public BzDui::INotifyUI,
	public BzDui::IMessageFilterUI,
	public BzDui::IListCallbackUI
{

public:

	CPBankWndEx(CWnd* pParent,bool bInMainRoom = false);
	virtual ~CPBankWndEx(void);

	LPCTSTR GetWindowClassName() const;

	UINT GetClassStyle() const;

	void OnFinalMessage(HWND hWnd);

	void Init();

	//����ȡ�����
	void Init_Ctn1_1();

	//���ڴ�����
	void Init_Ctn1_2();

	//����ת��
	void Init_Ctn1_3();

	//���ڴ浥����
	void Init_Ctn1_5();

	//���ڳ�ֵ
	void Init_Ctn1_7();

	//�����޸�����
	void Init_Ctn2_1();

	//���ڰ�ȫ����
	void Init_Ctn3_1();

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

	virtual LPCTSTR GetItemText(BzDui::CControlUI* pControl, int iIndex, int iSubItem);

public:

	bool OnNetMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

	//������1���еġ�����1����Ϣ������
	void Notify_1_1(BzDui::TNotifyUI& msg);

	//������1���еġ�����2����Ϣ������
	void Notify_1_2(BzDui::TNotifyUI& msg);

	//������1���еġ�����3����Ϣ������
	void Notify_1_3(BzDui::TNotifyUI& msg);

	//������1���еġ�����4����Ϣ������
	void Notify_1_4(BzDui::TNotifyUI& msg);

	//������1���еġ�����5����Ϣ������
	void Notify_1_5(BzDui::TNotifyUI& msg);

	//������1���еġ�����6����Ϣ������
	void Notify_1_6(BzDui::TNotifyUI& msg);

	//������1���еġ�����7����Ϣ������
	void Notify_1_7(BzDui::TNotifyUI& msg);

	//������2���еġ�����1����Ϣ������
	void Notify_2_1(BzDui::TNotifyUI& msg);

	//������3���еġ�����1����Ϣ������
	void Notify_3_1(BzDui::TNotifyUI& msg);

public:	///< �ⲿ���õĽӿ�
	
	///
	/// ���òƸ���Ϣ
	/// @param nBank ���д��
	/// @param nWallet �����ֽ�
	/// @return void
	///
	void SetWealthInfor(__int64 i64Bank, __int64 i64Wallet);

	///
	/// ���²Ƹ���Ϣ
	/// @return void
	///
	void UpdateWealthInfor();

	///
	/// ��ʾ
	/// @param pToCtrl ��ĳһ�ؼ�����ʾ
	/// @param lpSecName ClientMessage.bcf�еĶ���
	/// @param lpKeyName ClientMessage.bcf�еļ�ֵ
	/// @param lpDefault ClientMessage.bcfû���ҵ���ֵʱ��ʹ�õ�Ĭ��ֵ
	/// @return void
	///
	static void ShowTipMessage(BzDui::CControlUI* pToCtrl,LPCTSTR lpSecName,LPCTSTR lpKeyName,LPCTSTR lpDefault,...);

	///�����û�ID��ȡ�ǳ�
	bool OnIDKillFocus(BzDui::CEditUI* pEdtCtrl,BYTE nType,BYTE nUserful);

	/// ת�ʳɹ��������������
	/// @param Opt 0-��ʾת�������ˣ�1-��ʾ�ɱ���ת����Լ�
	/// @param nMoney ��ʾ�����������Ľ����
	void UpdateUserMoneyByTransfer(int OptType,__int64 i64Money);

	///
	/// �յ�һ��ת�˼�¼
	/// @param void
	/// @return void
	///
	void OnReceivedTransferRecord(MSG_GR_R_TransferRecord_t *pRecord);

	///ˢ�¼�¼��ҳ��
	void UpdateRecordPageState();

	///�����û�ID�ǳƵĲ�ѯ���
	void UpdateNicknameIDResult(GetNickNameID_t* pNID);

public:

	//���س���ؼ�Ƥ��
	void LoadNormalCtrlSkin();

	//Ϊĳһ���UI��������Ϣ��Ӧ����
	void BindNotify(UINT uSortIdx,BzDui::CStdString strCtnName,lpNotifyFun lpFunPtr,bool bNeedSubmit = true,BzDui::CStdString strBntName = "�ύ");

	//���ظ�����UI��������Ϣ����
	void LoadNotifyFunctions();

	//��ʼ����ϷǮ������
	void InitGameWalletTable();


private:

	//����Combol���б���
	void LoadComboItems(BzDui::CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect = 0);

	//�����ı�����ѡ���б���
	bool SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText);

	//չ��ĳ����������б�
	void ShowFuns(int nIdx);

	//���ת���Ĵ�д
	void ChangeDigitToChinese(__int64 i64Num, CString &strResult);

	//����������������������ͬ��ת��Ϊ���Ĵ�д
	void ChangeEtToChinese(BzDui::CEditUI* pEditCtrl,BzDui::CTextUI* pTextCtrl);

public:

	//ͨ����ϷNameID,�õ���Ϸ������
	char* GetGameNameById(UINT uNameID);

	//ͨ����ϷName,�õ���Ϸ��NameID
	UINT GetGameIDByName(LPCTSTR pGameName);

	//ͨ����ϷNameID,�õ���Ϸ������
	int GetWalletIndexById(UINT uNameID);

	//ͨ����ϷNameID,������Ǯ����Ǯ
	bool SetGameWalletMoney(UINT uNameID,__int64 NewMoney);

	//ͨ����ϷNameID,������Ǯ�����Ǯ
	bool AddGameWalletMoney(UINT uNameID,__int64 AddMoney);

	//ͨ����ϷName,��ȡǮ����Ǯ
	__int64 GetGameWalletMoney(LPCTSTR pGameName);

	//ͨ����ϷNameID,��ȡǮ����Ǯ
	__int64 GetGameWalletMoney(UINT uNameID);

public:

	BzDui::CPaintManagerUI m_pm;

	//��ǰʵ�����ھ��
	static HWND m_hInstanceWnd;

private:

	// ������ָ��
	CWnd* m_pRoom;

	//�����ڵ�����
	enum
	{
		IsMainRoom = 1, //��������MianRoomEx
		ISGameRoom = 2, //��������GameRoomEx
	}m_emParWndType;

	bank_op_normal m_bank_op_normal;
	bank_op_deposit m_bank_op_deposit;

	//���Ͱ�ť
	BzDui::CButtonUI* m_pSortBnt[3];

	//ÿ�����Ͷ�Ӧ�����б������
	BzDui::CContainerUI* m_pFunsCtn[3];

	//�ύ��ť
	BzDui::CButtonUI* m_pSubmitBnt;

	//ת�˼�¼��ؼ�ָ��
	BzDui::CListUI* m_pRecordList;

	//�浥��¼��ؼ�ָ��
	BzDui::CListUI* m_pDepList;

	struct TFunsNode
	{
		BzDui::CButtonUI* pFunsBnt;
		BzDui::CContainerUI* pFunsCtn;
		lpNotifyFun pNotifyFunPtr;
		bool bNeedSubmit;	//�Ƿ���Ҫ�ύ��ť
		BzDui::CStdString sBntName; //��ť������
		TFunsNode()
		{
			::memset(this,NULL,sizeof(TFunsNode));
		}
	};

	//ÿ�����͵Ĺ������Ա�
	list<TFunsNode> m_lstFuns[3];

//��������
private:

	//��ǰѡ��Ĺ���
	TFunsNode* m_pSelFunNode;

	//������Ĵ��
	__int64 m_i64BankMoney;

	//ת����д�����õĻ���
	vector<int> digitArray;

	bool bFlag;

	//����Ϸ��Ǯ�����
	struct TGmWalletNode
	{
		UINT uNameID;			//��Ϸ���� ID ����
		char szGameName[61];	//��Ϸ����
		__int64 i64Wallet;		//Ǯ
		TGmWalletNode()
		{
			::memset(this,NULL,sizeof(TGmWalletNode));
		}
	};
	vector<TGmWalletNode> m_vecGmWallets;

	//�û�ID
	DWORD m_dwOpUserID;

// 	//�ϴ�����ת�˵�Ŀ���û�ID
// 	CString m_strTransTargetID;
// 
// 	//�ϴ�����ת�˵�Ŀ���û��ǳ�
 	CString m_strTransTargetNickName;

	//ת�˳ɹ�
	bool m_bTransferSuccess; 

	//ת����־��¼��
	std::map<int,MSG_GR_R_TransferRecord_t> m_mapRecord;

	//ת����־��¼����ʾλ��
	int m_iRecDispPos;

	//�����浥�ɹ�
	bool m_bDepositSuccess;

	//�浥��¼��
	std::map<int,bank_op_deposit_node> m_mapDeposits;

	//��ʱ��
	DWORD m_dwTickCount;

	//�Ƿ�ʱ���
	bool m_bCheckFrequence;
	int  m_iMinutes;

	//��¼�༭��ý���ǰ���ַ�������killfocusʱ�����
	BzDui::CStdString m_strFocusTemp;

};


#endif
/*******************************************************************************************************/