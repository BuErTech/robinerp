// LockDeskDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TCP.h"
#include "LockDeskDlg.h"


// CLockDeskDlg �Ի���

IMPLEMENT_DYNAMIC(CLockDeskDlg, CDialog)

CLockDeskDlg::CLockDeskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLockDeskDlg::IDD, pParent)
	, m_Lower(0)
	, m_Upper(0)
	, m_Infostatic(_T(""))
{

}

CLockDeskDlg::~CLockDeskDlg()
{
}

void CLockDeskDlg::SetInfo(int itype, int imin, int imax)
{
	m_type = itype;
	m_Min = imin;
	m_Max = imax;
	m_Lower = imin;
	m_Upper = imax;
}

void CLockDeskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Lower);
	DDX_Text(pDX, IDC_EDIT2, m_Upper);
	DDX_Text(pDX, IDC_STATIC1, m_Infostatic);
}


BEGIN_MESSAGE_MAP(CLockDeskDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLockDeskDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLockDeskDlg ��Ϣ�������

void CLockDeskDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (m_Lower < m_Min || m_Upper > m_Max)
	{
		AfxMessageBox("��������ȷ�����䷶Χ");
		return;
	}
	if (m_type == 1)
	{
		::PostMessage(m_hWnd, MSG_CMD_GM_ALLLOCKOK, WPARAM(&m_Lower), LPARAM(&m_Upper));
	}
	else
	{
		::PostMessage(m_hWnd, MSG_CMD_GM_ALLUNLOCKOK, WPARAM(&m_Lower), LPARAM(&m_Upper));
	}
	OnOK();
}

BOOL CLockDeskDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (m_type == 1)
	{
		SetWindowText("��������");
		m_Infostatic = "������������������:";
	}
	else 
	{
		SetWindowText("�������"); 
		m_Infostatic = "����������������:";
	}

	((CEdit*)GetDlgItem(IDC_EDIT1))->SetLimitText(3);	
	((CEdit*)GetDlgItem(IDC_EDIT2))->SetLimitText(3);	

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
