#pragma once
#include "afxwin.h"
#include "SkinListCtrl.h"
#include <map>
#include "SkinMgr.h"

/// ��ȡǮ�õ���Ϣ����
const int WM_CHECK_MONEY	= WM_USER + 520;
const int WM_TRANSFER_MONEY	= WM_USER + 521;
const int WM_CHANGE_PASSWD	= WM_USER + 522;
const int WM_TRANSFER_RECORD= WM_USER + 523;
const int WM_CHECK_MONEY2   = WM_USER + 524; //�°����д�ȡ��

// �����û�ID��ȡ�ǳ� lxl, 2010-11-17
const int WM_BANK_GET_NICKNAME_ONID = WM_USER + 525;

// lxl 2010-11-19  ���ת���Ĵ�д����
const int DIGITSIZE = 10;	//�������Ĵ�д
const int UNITSIZE = 19;	//������λ��

static char* szDigit[DIGITSIZE] = {"��", "Ҽ", "��", "��", "��", "��", "½", "��", "��", "��"}; //���Ĵ�д����
static char* szUnit[UNITSIZE] = {"", "ʰ", "��", "Ǫ", "��", "ʰ", "��", "Ǫ", "��", "ʰ","��","Ǫ","��","ʰ","��","Ǫ","��","ʰ","��"};	  //���Ĵ�д��λ


/// ��ȡǮ
/// �����븸���ڽ���ͨѶ�Ľṹ
struct CheckMoney_t
{
	UINT uType;			///< ���ȡ����Ϊ0��ȡΪ1
	__int64 i64Money;	///< Ҫ��ȡ�Ľ��
	TCHAR szPWD[32];	///< ���룬δ����
	CheckMoney_t()
	{
		ZeroMemory(this, sizeof(CheckMoney_t));
	}
};
/// ת��
/// �����븸���ڽ���ͨѶ�Ľṹ
struct TransferMoney_t
{
	UINT uDestUserID;	///< Ҫת����Ŀ���û�ID
	__int64 i64Money;			///< Ҫ��ȡ�Ľ��
	TCHAR szPWD[32];	///< ���룬δ����
	TCHAR szDestNickName[50]; ///Ŀ���ǳ�
	bool bUseDestID;
	TransferMoney_t()
	{
		ZeroMemory(this, sizeof(TransferMoney_t));
	}
};

// PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ�
struct GetNickNameOnID_t
{
    UINT    uUserID;        // �û�ID
    TCHAR   chNickName[61]; // �ǳ�

    GetNickNameOnID_t()
    {
        ZeroMemory(this, sizeof(GetNickNameOnID_t));
    }
};

/// �޸�����
/// �����븸���ڽ���ͨѶ�Ľṹ
struct ChangePasswd_t
{
	TCHAR szPwdOld[32];
	TCHAR szPwdNew[32];
	ChangePasswd_t()
	{
		ZeroMemory(this, sizeof(ChangePasswd_t));
	}
};

// CPBankDlg �Ի���

class CPBankDlg : public CDialog
{
	DECLARE_DYNAMIC(CPBankDlg)

	HBRUSH m_bkBrush;
public:
	CPBankDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPBankDlg();
	
// �Ի�������
	//enum { IDD = IDD_BANK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	virtual BOOL OnInitDialog();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedInfor();
	afx_msg void OnBnClickedTrans();
	afx_msg void OnBnClickedRecord();
	afx_msg void OnBnClickedSafe();
	afx_msg void OnBnClickedPassWord();

	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedGet();

	afx_msg void OnBnClickedRTransId();
	afx_msg void OnBnClickedRTransNn();

	///
	/// �ʺ�ת�� ��ȷ����ť��Ӧ����
	/// @param void
	/// @return void
	afx_msg void OnBnClickedAccountOK();

	///
	/// �޸����� ��ȷ����ť��Ӧ����
	/// @param void
	/// @return void
	///
	afx_msg void OnBnClickedChPW();
	
	///
	/// ���� ��ȷ����ť��Ӧ����
	/// @param void
	/// @return void
	///
	afx_msg void OnBnClickedBankOK();

	///
	/// ���� �ļ��ť��Ӧ����
	/// @param void
	/// @return void
	///
	afx_msg void OnBnClickedBankActive();

	///
	/// ���� �����Ѱ�ť��Ӧ����
	/// @param void
	/// @return void
	///
	afx_msg void OnBnClickedBankRecharge();
	
	// �����û�ID��ȡ�ǳ� lxl, 2010-11-17
    afx_msg void OnIDKillFocus();

	// lxl 2010-11-19  ת�˽��Сдת��Ϊ���Ĵ�д
	afx_msg void OnEnChangeEtbconnoin();
	// lxl 2010-11-19  ��ȡ����Сдת��Ϊ���Ĵ�д
	afx_msg void OnEnChangeMoney();

	afx_msg void OnBnClickedCoinAll();

protected:
	void ShowInterface();
	DWORD GetBackGroundColor();
	virtual void OnOK();

private:
	CWnd *m_pGameRoom;		/// ������ָ��
	Image *m_pbgImg;
	CGameImage		m_bkimage;
	CNormalBitmapButtonEx m_BtClose;
	CNormalBitmapButtonEx m_btInfor;
	CNormalBitmapButtonEx m_bnTrans;
	CNormalBitmapButtonEx m_bnRecord;
	CNormalBitmapButtonEx m_bnSafe;
	CNormalBitmapButtonEx m_bnPW;
	CNormalBitmapButtonEx m_btnRefresh;
	CNormalBitmapButtonEx m_btnRcoClose;
	
	CNormalBitmapButtonEx m_btnTranOK;
	
	CNormalBitmapButtonEx m_btnBok;
	CNormalBitmapButtonEx m_btnBactive;
	CNormalBitmapButtonEx m_btnBcharge;

	CNormalBitmapButtonEx m_btnChOK;
	CNormalBitmapButtonEx m_btnChClose;

	CNormalBitmapButtonEx m_btnCoinAll;

	CSkinListCtrl	m_SkinList;

	DECLARE_MESSAGE_MAP()

public:	///< �ⲿ���õĽӿ�
	///
	/// ���òƸ���Ϣ
	/// @param nBank ���д��
	/// @param nWallet �����ֽ�
	/// @return void
	///
	void SetWealthInfor(__int64 i64Bank, __int64 i64Wallet);

	/// ��ȡǮ����½���
	/// @param Opt 0-��ʾȡ����1-��ʾ����
	/// @param nMoney ��ʾ�����������Ľ����
	void UpdateUserMoney(int OptType, __int64 i64Money);

	/// ת�ʳɹ��������������
	/// @param Opt 0-��ʾת�������ˣ�1-��ʾ�ɱ���ת����Լ�
	/// @param nMoney ��ʾ�����������Ľ����
	void UpdateUserMoneyByTransfer(int OptType, __int64 i64Money);
	///
	/// ���� �ʺ�ת�� �µĽ����
	/// @param void
	/// @return void
	///
	void SetAccountInfor();

	///
	/// ���� ����ת������
	/// @param void
	/// @return void
	///
	void InsertRecordList();

	///
	/// ˢ�� ת������
	/// @param void
	/// @return void
	///
	void RefreshRecordList();
	///
	/// �յ�һ��ת�˼�¼
	/// @param void
	/// @return void
	///
	void OnReceivedTransferRecord(MSG_GR_R_TransferRecord_t *pRecord);

   
    // ���û�ȡ���ǳ� lxl  2010-11-17
    void SetNickName(TCHAR* szNickName);
    CString m_strNickName;   
	CString m_strSaveID;
    int m_iUseNickName;				// �Ƿ�ʹ���ǳ���ʾ

	XLogic	*m_pLogic;

private:
	void ShowMoneyBank();

	int		m_nCurPage;
	UINT	m_uCheckInOrOut;	///< ��Ǯ����ȡǮ����ǮΪ0��ȡǮΪ1
	bool	m_bUseDestID;
	__int64	m_i64WalletMoney;		///< ���е��ֽ�
	__int64	m_i64BankMoney;		///< ������Ĵ��
	std::map<int, MSG_GR_R_TransferRecord_t> m_mapTransferRecord;

	void ChangeDigitToChinese(__int64 i64Num, CString &result); //add by lxl 2010-11-19 ���ת���Ĵ�д
	vector<int> digitArray;
	bool bFlag;
	CSkinMgr m_skinmgr;

public:
	void LoadSkin();
};
