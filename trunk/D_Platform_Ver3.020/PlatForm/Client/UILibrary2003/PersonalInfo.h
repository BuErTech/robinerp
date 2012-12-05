#pragma once
#ifndef PERSONALINFO_HEAD_H
#define PERSONALINFO_HEAD_H
#include "afcdevhead.h"

// CPersonalInfo �Ի���
#define			WINDOW_APP			1
#define			WINDOW_DISAPP		2

class EXT_CLASS CPersonalInfo : public CDialog
{
	DECLARE_DYNAMIC(CPersonalInfo)
private:
	UserInfoStruct m_userInfoItem;

	HBRUSH m_bkBrush;
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
	CNormalBitmapButtonEx			m_btInvite;							///<�����Ѿ��������������һ����Ϸ��ť	
	CNormalBitmapButtonEx	        m_btWatch;
	CNormalBitmapButtonEx	        m_btGift;

	COLORREF						m_FontClr;							//JMod-������ɫ 20090506
public:
	void getRoomPtr(CWnd* room);
	int GetWidth(){return m_bkimage.GetWidth();}
	int GetHeight(){return m_bkimage.GetHeight();}
// �Ի�������
	//enum { IDD = IDD_PERSONINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


	DECLARE_MESSAGE_MAP()
private:
	int m_time;
	CString m_szNiCheng;
	__int64 m_i64CaiFu;
	long m_lJiFeng;
	CString m_szJiBie;

	afx_msg void OnBnClickedChat();
	afx_msg void OnBnClickedCall();
	afx_msg void OnBnClickedWatchGame();
	afx_msg void OnBnClickedGift();
};

#endif
