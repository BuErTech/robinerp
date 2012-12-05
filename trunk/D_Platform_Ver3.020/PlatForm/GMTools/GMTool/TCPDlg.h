// TCPDlg.h : ͷ�ļ�
//

#pragma once
#include "../../../SDK/include/socket/TCPClientSocket.h"
#include "BClientSocket.h"
#include "ZClientSocket.h"
#include "WClientSocket.h"
#include "LongoDlg.h"
#include "GameManageDlg.h"



// CTCPDlg �Ի���
class CTCPDlg : public CDialog
{
// ����
public:
	CTCPDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TCP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	CBClientSocket		m_bSocket;				///B�������ͻ���
	CZClientSocket		m_zSocket;				///Z�������ͻ���

	CLongoDlg			m_LongoDlg;				///��¼�Ի���
	CGameManageDlg		 m_GameManageDlg;		///����

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
public:
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	virtual BOOL Create(UINT lpszTemplateName, CWnd* pParentWnd = NULL);
public:
	virtual BOOL DestroyWindow();
};
