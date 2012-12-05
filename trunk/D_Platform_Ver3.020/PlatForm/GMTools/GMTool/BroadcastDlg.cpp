// BroadcastDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TCP.h"
#include "BroadcastDlg.h"



// CBroadcastDlg �Ի���

IMPLEMENT_DYNAMIC(CBroadcastDlg, CDialog)

CBroadcastDlg::CBroadcastDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBroadcastDlg::IDD, pParent)
	, m_Info(_T(""))
{

}

CBroadcastDlg::~CBroadcastDlg()
{
}

void CBroadcastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Info);
	DDV_MaxChars(pDX, m_Info, 255);
}


BEGIN_MESSAGE_MAP(CBroadcastDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBroadcastDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBroadcastDlg ��Ϣ�������

BOOL CBroadcastDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CBroadcastDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	UpdateData();
	if (m_Info != "")
	{
		m_Broadcast_t.iUserID = 0;
		strcpy(m_Broadcast_t.szMsg, m_Info.GetBuffer());
		AfxGetMainWnd()->PostMessage(MSG_CMD_BROADCAST, WPARAM(&m_Broadcast_t), 0);
		m_Info = "";
		UpdateData(false);
		CDialog::OnOK();
	}
	else 
	{
		AfxMessageBox("�������ݲ���Ϊ�գ�");
	}
}

void CBroadcastDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
