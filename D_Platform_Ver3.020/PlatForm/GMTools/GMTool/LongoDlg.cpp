// LongoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TCP.h"
#include "LongoDlg.h"

#include "cfgEngine.h"
#include "wCore2.h"


// CLongoDlg �Ի���

IMPLEMENT_DYNAMIC(CLongoDlg, CDialog)

CLongoDlg::CLongoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLongoDlg::IDD, pParent)
	, m_name(_T(""))
{

}

CLongoDlg::~CLongoDlg()
{
}

void CLongoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_UeserNameCmb);
	DDX_Control(pDX, IDC_EDIT1, m_PassWordEdt);
	DDX_CBString(pDX, IDC_COMBO1, m_name);
	DDV_MaxChars(pDX, m_name, 16);
}


BEGIN_MESSAGE_MAP(CLongoDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLongoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLongoDlg ��Ϣ�������

BOOL CLongoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��Ȩ���
	long timeStamp=coreGetTimeStamp();

	long licExpires=coreGetLicenseExpires();

	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW); 
	CenterWindow();
	m_UeserNameCmb.AddString("bozong1001");
	m_UeserNameCmb.AddString("bozong1002");
	m_UeserNameCmb.AddString("bozong1003");
	m_UeserNameCmb.SetCurSel(0);

	m_PassWordEdt.SetWindowText("112233");
	((CEdit*)GetDlgItem(IDC_EDIT1))->SetLimitText(16);
//	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetLimitText(16);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	if(timeStamp>licExpires)
	{
		CString s,code=coreGetCode();
		s.Format("���ķ�����δע����ѹ��ڣ������������ϵ��\n\n�뽫���»����뷢�͸������̣���ȡע�����ļ���\n\n%s\n\n",code);
		MessageBox(s,"��ʾ",MB_ICONERROR);
		s="�������Ѹ��Ƶ����ļ������У�ֱ��Ctrl+Vճ�������룡";
		MessageBox(s,"��ʾ",MB_ICONINFORMATION);
		OpenClipboard();
		EmptyClipboard();
		HANDLE hData=GlobalAlloc(GMEM_MOVEABLE,code.GetLength()+5); 
		if (hData==NULL)  
		{ 
			CloseClipboard(); 
			return TRUE; 
		}
		LPTSTR szMemName=(LPTSTR)GlobalLock(hData); 
		lstrcpy(szMemName,code); 
		SetClipboardData(CF_TEXT,hData); 
		GlobalUnlock(hData);  
		GlobalFree(hData); 
		CloseClipboard(); 
		PostQuitMessage(0);
		DestroyWindow();
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLongoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_UeserNameCmb.GetWindowText(m_UeserName);
	m_PassWordEdt.GetWindowText(m_PassWord);
	AfxGetMainWnd()->PostMessage(MSG_CMD_LONGIN, WPARAM(&m_UeserName), LPARAM(&m_PassWord));
	OnOK();
}

void CLongoDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	PostQuitMessage(0);
	CDialog::OnCancel();
	
}
