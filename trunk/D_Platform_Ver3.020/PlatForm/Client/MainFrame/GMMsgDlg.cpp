#include "StdAfx.h"
#include "GMMsgDlg.h"

CGMMsgDlg::CGMMsgDlg(CString strInfo, int sec):
ConfirmDlg(strInfo)
{
	m_sec = sec;
}

CGMMsgDlg::~CGMMsgDlg(void)
{
}

BOOL CGMMsgDlg::OnInitDialog()
{
	ConfirmDlg::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	if (m_sec != 0)
		SetTimer(10000, m_sec * 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
BEGIN_MESSAGE_MAP(CGMMsgDlg, ConfirmDlg)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CGMMsgDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 10000) OnOK();
	ConfirmDlg::OnTimer(nIDEvent);
}

void CGMMsgDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	ConfirmDlg::OnOK();
}
