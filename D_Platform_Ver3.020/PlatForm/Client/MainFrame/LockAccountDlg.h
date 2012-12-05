#pragma once

#include "SkinMgr.h"
#include "..\UILibrary2003\CconfigDlg.h" 
// CLockAccountDlg �Ի���

class CLockAccountDlg : public CLoadFileBmp
{
	DECLARE_DYNAMIC(CLockAccountDlg)
	HBRUSH	m_bkBrush;
public:
	CLockAccountDlg(CWnd* pParent = NULL, int nType=0);   // ��׼���캯��
	virtual ~CLockAccountDlg();

	static char							m_szScrPass[61];					//�û�ԭ����
	static char							m_szMD5Pass[50];					//���ܺ������
	CGameImage							m_bk;				//����
	CNormalBitmapButtonEx				m_BtCancel;			//ȡ��
	CNormalBitmapButtonEx				m_BtOk;				//ȷ��
	CNormalBitmapButtonEx				m_BtClose;			//�ر�
	CconfigDlg  m_configDlg; //���ص�ͼ��. 2012.10.17 yyf
// �Ի�������
	enum { IDD = IDD_LOCK_ACCOUNT };
private:
	int m_nShowType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void drawSkin();
public:
	afx_msg void OnPaint(); 
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedClose();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedCancel();

private:
	CSkinMgr m_skinmgr;

public:
	  // ���ô���Բ�� 2012.10.17 yyf
	  void SetWndRgn(void);
};
