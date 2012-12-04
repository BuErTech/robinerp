// IMSetUser.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DllForGame.h"
#include "IMSetUser.h"


// CIMSetUser �Ի���

IMPLEMENT_DYNAMIC(CIMSetUser, CDialog)

CIMSetUser::CIMSetUser(CWnd* pParent /*=NULL*/)
	: CDialog(CIMSetUser::IDD, pParent)
{

}

CIMSetUser::~CIMSetUser()
{
}

void CIMSetUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ctlWords);
	DDX_Control(pDX, IDC_STATE, ctlTip);
}


BEGIN_MESSAGE_MAP(CIMSetUser, CDialog)
	ON_BN_CLICKED(IDOK, &CIMSetUser::OnBnClickedOk)
END_MESSAGE_MAP()


// CIMSetUser ��Ϣ�������

void CIMSetUser::OnBnClickedOk()
{
	m_ctlWords.GetWindowText(imWords);
	imWords.Trim();
	if(imWords.GetLength()>100)
		imWords=imWords.Left(100);//���ܳ���
	OnOK();
}

BOOL CIMSetUser::OnInitDialog()
{
	CDialog::OnInitDialog();

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str = fMsg.GetKeyVal("MainRoom","MakeFriendLeaveMsg","�����û�[%s] �����Ϊ���ѣ��������������ԣ�");
	CString s;
	s.Format(str,szName);
	ctlTip.SetWindowText(s);
	imWords="";


	SetWindowPos(&wndTopMost,   0,   0,   0,   0,   SWP_NOMOVE|SWP_NOSIZE|WS_EX_TOPMOST); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



// CIMSetUser ��Ϣ�������
