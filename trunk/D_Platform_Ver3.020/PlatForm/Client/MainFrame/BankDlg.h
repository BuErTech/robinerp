#pragma once

#ifndef BANKDLG_HEAD_H
#define BANKDLG_HEAD_H

#include "BaseRoom.h"
#include "..\UILibrary2003\CconfigDlg.h"
//#include "DlgInMoney.h"
//#include "DlgOutMoney.h"
//#include "DlgTransferMoney.h"
// CBankDlg dialog

class CBankDlg : public CDialog
{
	DECLARE_DYNAMIC(CBankDlg)

public:
	CBankDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBankDlg();
	void SetBankMoney(int count);
	void SetRoomMoney(int count);
	int GetBankMoney();
	void SetSocketPoint(CTCPClientSocket* m_TCPSocket);
	void GetUserID(long UserID);
	void GetRoomName(TCHAR * roomName);
	void GetConnectPoint(CConnectDlg* connection);
	void getUserItem(UserItemStruct * pMeUserInf);
	bool GetRoomType(int uComType);

	void UpdateUserMoney(int OptType, long nMoney);	//���½��棬Opt=0,��ʾȡ����Opt=1����ʾ���룬nMoney��ʾ�����������Ľ����
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
	CNormalBitmapButtonEx	m_btCheckIn;	///< ���д���
	CNormalBitmapButtonEx	m_btTransfer;	///< ����ת��
	CNormalBitmapButtonEx	m_btQuery;		///< ���в�ѯ
	CconfigDlg  m_configDlg; //���ص�ͼ��. 2012.10.17 yyf
	HBRUSH m_bkBrush;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	// ʣ����
	int m_BankMoney;
	int m_RoomMoney;
	afx_msg void OnBnClickedCheckout();
	afx_msg void OnBnClickedCheckin();
	afx_msg void OnBnClickedTransfer();
	afx_msg void OnBnClickedQuery();
	CTCPClientSocket* m_TCPSocket;
	CString m_RoomName;
	CConnectDlg * m_Connection;
	CGameImage		m_bkimage;
	int m_uComType;
	UserItemStruct					* m_pMeUserInfo;						//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    
public:
	  // ���ô���Բ�� 2012.10.17 yyf
	  void SetWndRgn(void);
};
#endif