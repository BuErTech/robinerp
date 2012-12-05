#pragma once

#ifndef BANKDLG_HEAD_H
#define BANKDLG_HEAD_H

//#include "BaseRoom.h"
//#include "DlgInMoney.h"
//#include "DlgOutMoney.h"
#include "resource.h"
#include "../common/GameRoomMessage.h"
#include "Platform/AFCImage.h"
#include "Platform/AFCButton.h"
#include "SkinMgr.h"

#define WM_BANK_CHECK_OUT	WM_USER + 228		///< ���в����Ի����з��ʹ�Ǯ��Ϣ��CLoveSendClassInExe����
/// CBankDlg dialog
/// ���в�������Ի���
/// �ɸ�����ʵʱnew���������򵱸������յ�������Ϣ�ر�ʱ��ʧ�ص��ӶԻ���ѭ�����������
class CBankDlg : public CDialog
{
	DECLARE_DYNAMIC(CBankDlg)

	HWND	m_hWndParent;			///< �����ھ��
	HBRUSH m_bkBrush;
public:
	CBankDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBankDlg();
	void SetBankMoney(int count);
	void SetRoomMoney(int count);
	int GetBankMoney();
	//void SetSocketPoint(CTCPClientSocket* m_TCPSocket);
	void GetUserID(long UserID);
	void GetRoomName(TCHAR * roomName);
	//void GetConnectPoint(CConnectDlg* connection);
	void getUserItem(UserItemStruct * pMeUserInf);
	bool GetRoomType(int uComType);

	/// ���½���
	/// @param[in] Opt =0,��ʾȡ����Opt=1����ʾ����
	/// @param[in] nMoney ��ʾ�����������Ľ����
	/// @return void
	void UpdateUserMoney(int OptType, long nMoney);
	//��ʼ������
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	long int m_UserID;

	MSG_GR_S_CheckMoney checkMoney;

	// Dialog Data
	//enum { IDD = IDD_DIALOGBANK };
	CNormalBitmapButtonEx	m_BtCancel;		///< ����ȡ��
	CNormalBitmapButtonEx	m_btCheckOut;	///< ����ȡ��
	CNormalBitmapButtonEx	m_btTransfer;	///< ����ת��
	CNormalBitmapButtonEx	m_btQuery;		///< ���в�ѯ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	// ʣ����
	int m_BankMoney;
	int m_RoomMoney;
	afx_msg void OnBnClickedCheckout();
	afx_msg void OnBnClickedTransfer();
	afx_msg void OnBnClickedQuery();
	CString m_RoomName;
	CGameImage		m_bkimage;
	int m_uComType;
	UserItemStruct					* m_pMeUserInfo;						//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnChangeCheckNum();

private:
	CSkinMgr m_skinmgr;
};
#endif