// ExtRobot.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ExtRobot.h"


// CExtRobot �Ի���

IMPLEMENT_DYNAMIC(CExtRobot, CDialog)

CExtRobot::CExtRobot(CWnd* pParent /*=NULL*/)
	: CDialog(CExtRobot::IDD, pParent)
	, m_extRobot(0)
{

}

CExtRobot::~CExtRobot()
{
}

void CExtRobot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_extRobot);
}


BEGIN_MESSAGE_MAP(CExtRobot, CDialog)
	ON_BN_CLICKED(IDOK, &CExtRobot::OnBnClickedOk)
END_MESSAGE_MAP()


// CExtRobot ��Ϣ�������

void CExtRobot::OnBnClickedOk()
{
	UpdateData();
	OnOK();
}
