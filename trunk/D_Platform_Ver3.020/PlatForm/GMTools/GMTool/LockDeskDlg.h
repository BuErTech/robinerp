#pragma once

//////////////////////////////////////////////////////////////////////////
///��������
///2010.8.12 rongqiufen
//////////////////////////////////////////////////////////////////////////

#include "CommandMessage.h"

// CLockDeskDlg �Ի���

class CLockDeskDlg : public CDialog
{
	DECLARE_DYNAMIC(CLockDeskDlg)

public:
	CLockDeskDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLockDeskDlg();

	void SetInfo(int itype, int imin, int imax);

// �Ի�������
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_Lower;		///�����½�
	int m_Upper;		///�����Ͻ�

	int m_Min;			///�����½���Сֵ
	int m_Max;			///�����Ͻ���Сֵ	
	int m_type;			///��������(1��ʾ������������ʾ����)

	HWND m_hWnd;		///������Ϣ���ھ��
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
public:
	CString m_Infostatic;
};
