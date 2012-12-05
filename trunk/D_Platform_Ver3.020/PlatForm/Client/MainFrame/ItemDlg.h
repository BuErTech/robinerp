#pragma once

#include "SkinMgr.h"
#include "..\UILibrary2003\CconfigDlg.h"
// CItemDlg �Ի���

class CItemDlg : public CDialog
{
	DECLARE_DYNAMIC(CItemDlg)

public:
	CItemDlg();   // ��׼���캯��
	virtual ~CItemDlg();

// �Ի�������
	//enum { IDD = IDD_DIG_ITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	//CString ReadFile(CString filename);
	afx_msg void OnPaint();
	void OnClickedClose();

	CGameImage	m_bk;
	CString m_strText;
	CFile m_File;
	CNormalBitmapButtonEx				m_Bt1;	
	CNormalBitmapButtonEx				m_Bt2;	
	DECLARE_MESSAGE_MAP()

private:
	CSkinMgr m_skinmgr;
	CconfigDlg  m_configDlg; //���ص�ͼ��. 2012.10.17 yyf
public:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);
 
	  // ���ô���Բ�� 2012.10.15 yyf
	  void SetWndRgn(void);
};
