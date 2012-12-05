#pragma once


#include "gameplacemessage.h"
#include "resource.h"
#include "MyListCtrl.h"
#include "SkinMgr.h"
#include <vector>
#include <algorithm>
#include <functional>
#include "afxcmn.h"
 
#include "..\UILibrary2003\CconfigDlg.h" 

using namespace std;
// CCharmExchange �Ի���


class CCharmExchange : public CDialog
{
	DECLARE_DYNAMIC(CCharmExchange)

public:
	CCharmExchange(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCharmExchange();

	HBRUSH	m_bkBrush;

// �Ի�������
	enum { IDD = IDD_DIALOG8 };

	vector<TCharmExchange_Item> m_ChangeList;

public:
	void OnNetMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CHappyGoDlgT m_list;

	CNormalBitmapButtonEx  m_Change;
	CNormalBitmapButtonEx  m_Cancel;
	CNormalBitmapButtonEx  m_Close;

	CGameImage		m_bkimage;
	//CDUIWnd m_duiWnd;
	CconfigDlg  m_configDlg; //���ص�ͼ��. 2012.10.15 yyf

	afx_msg void OnBnClickedChange();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClose();

	
	void Clear();
public:
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnPaint();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:

	//��������ֵ
	void UpdateFascination();
	 
	// ���ô���Բ�� 2012.10.15 yyf
	void SetWndRgn(void);
private:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);

	void LoadSkin();

	void Init();
private:
	CSkinMgr m_skinmgr;
	
};
