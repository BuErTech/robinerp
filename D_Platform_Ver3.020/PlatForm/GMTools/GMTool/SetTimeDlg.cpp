// SetTimeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TCP.h"
#include "SetTimeDlg.h"


// CSetTimeDlg �Ի���

IMPLEMENT_DYNAMIC(CSetTimeDlg, CDialog)

CSetTimeDlg::CSetTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTimeDlg::IDD, pParent)
	, m_szReason(_T(""))
{
	
}

CSetTimeDlg::~CSetTimeDlg()
{
}

void CSetTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_DayEdit);
	DDX_Control(pDX, IDC_EDIT2, m_HourEdit);
	DDX_Control(pDX, IDC_EDIT3, m_MinuteEdit);
	DDX_Control(pDX, IDC_CHECK1, m_ForeverChk);
	DDX_Text(pDX, IDC_RICHEDIT21, m_szReason);
}


BEGIN_MESSAGE_MAP(CSetTimeDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CSetTimeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CSetTimeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetTimeDlg ��Ϣ�������

void CSetTimeDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnCancel();
}

void CSetTimeDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_Minute = 0;
	CString s;
	m_DayEdit.GetWindowText(s);
	m_Minute += atoi(s.GetBuffer(s.GetLength())) * 24 * 60; 
	m_HourEdit.GetWindowText(s);
	m_Minute += atoi(s.GetBuffer(s.GetLength())) * 60;
	m_MinuteEdit.GetWindowText(s);
	m_Minute += atoi(s.GetBuffer(s.GetLength()));

	if (m_ForeverChk.GetCheck())
	{
		m_Minute = 0xFFFFFFFF;
	}

	OnOK();
}

void CSetTimeDlg::Init()
{
	m_DayEdit.SetWindowText("");
	m_HourEdit.SetWindowText("");
	m_MinuteEdit.SetWindowText("30");
	m_ForeverChk.SetCheck(false);
}
INT_PTR CSetTimeDlg::DoModal()
{
	// TODO: �ڴ����ר�ô����/����û���
	
	INT_PTR r = CDialog::DoModal();
	//Init();
	return r;
}
