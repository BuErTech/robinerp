#pragma once
#include "resource.h"


// CPersonalInfo �Ի���
#define			WINDOW_APP			1
#define			WINDOW_DISAPP		2

class CPersonalInfo : public CDialog
{
	DECLARE_DYNAMIC(CPersonalInfo)
private:
	UserInfoStruct m_userInfoItem;
public:
	CPersonalInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPersonalInfo();

	virtual BOOL OnInitDialog();
	bool UpdateUserItem(UserInfoStruct user);
	afx_msg void OnPaint();
	public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void close();
	CGameImage						m_bkimage;
	GetOrderName					* pOrderName;						//�ȼ�����
	CWnd*							m_roomWnd;
	CNormalBitmapButtonEx			m_btChat;
	CNormalBitmapButtonEx			m_btInvite;		
	CNormalBitmapButtonEx	        m_btWatch;

public:
	void getRoomPtr(CWnd* room);
// �Ի�������
	enum { IDD = IDD_PERSONINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


	DECLARE_MESSAGE_MAP()
private:
	int m_time;
	CString m_szNiCheng;
	long m_lCaiFu;
	long m_lJiFeng;
	CString m_szJiBie;

	afx_msg void OnBnClickedChat();
	afx_msg void OnBnClickedCall();
	afx_msg void OnBnClickedWatchGame();
};
