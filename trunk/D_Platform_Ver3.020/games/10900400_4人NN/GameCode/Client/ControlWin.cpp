// ControlWin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ControlWin.h"
#include ".\clientgamedlg.h"


// CControlWin �Ի���

IMPLEMENT_DYNAMIC(CControlWin, CDialog)

CControlWin::CControlWin(CWnd* pParent /*=NULL*/)
	: CDialog(CControlWin::IDD, pParent)
{

}

CControlWin::~CControlWin()
{
}

void CControlWin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlWin, CDialog)
	ON_BN_CLICKED(IDOK, &CControlWin::OnBnClickedOk)
END_MESSAGE_MAP()

void CControlWin::SetParentWnd(CWnd* pWnd)
{
	m_pParentWnd = pWnd;
	if (PLAY_COUNT == 2)
	{
		((CButton *)(GetDlgItem(IDC_RADIO3)))->ShowWindow(SW_HIDE);
		((CButton *)(GetDlgItem(IDC_RADIO5)))->ShowWindow(SW_HIDE);
		((CButton *)(GetDlgItem(IDC_RADIO17)))->ShowWindow(SW_HIDE);
		((CButton *)(GetDlgItem(IDC_RADIO19)))->ShowWindow(SW_HIDE);
		((CButton *)(GetDlgItem(IDC_RADIO16)))->ShowWindow(SW_HIDE);
		((CButton *)(GetDlgItem(IDC_RADIO18)))->ShowWindow(SW_HIDE);

		((GetDlgItem(IDC_STATIC2)))->ShowWindow(SW_HIDE);
		((GetDlgItem(IDC_STATIC3)))->ShowWindow(SW_HIDE);
		((GetDlgItem(IDC_STATIC4)))->ShowWindow(SW_HIDE);
		((GetDlgItem(IDC_STATIC1)))->ShowWindow(SW_HIDE);
		((GetDlgItem(IDC_STATIC5)))->ShowWindow(SW_HIDE);
		((GetDlgItem(IDC_STATIC6)))->ShowWindow(SW_HIDE);
		
	}
}
// CControlWin ��Ϣ�������

void CControlWin::OnBnClickedOk()
{
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	BYTE bMyDesk = ((CClientGameDlg *)m_pParentWnd)->GetMeUserInfo()->bDeskStation;
	BYTE bWinDesk = 255;
	BYTE bLostDesk = 255;

	if (((CButton *)(GetDlgItem(IDC_RADIO4)))->GetCheck())//����Ӯ
	{

		bWinDesk = bMyDesk;
	}
	else if (((CButton *)(GetDlgItem(IDC_RADIO3)))->GetCheck())//��Ӯ
	{

		bWinDesk = (bMyDesk+3)%PLAY_COUNT;
	}
	else if (((CButton *)(GetDlgItem(IDC_RADIO2)))->GetCheck())//�Ϸ�Ӯ
	{

		if(PLAY_COUNT == 2)
		{
			bWinDesk = (bMyDesk+1)%PLAY_COUNT;
		}
		else
		{
			bWinDesk = (bMyDesk+2)%PLAY_COUNT;
		}
	}
	else if (((CButton *)(GetDlgItem(IDC_RADIO5)))->GetCheck())//�ҷ�Ӯ
	{

		bWinDesk = (bMyDesk+1)%PLAY_COUNT;
	}

	if (((CButton *)(GetDlgItem(IDC_RADIO18)))->GetCheck())//
	{

		bLostDesk = bMyDesk;
	}
	else if (((CButton *)(GetDlgItem(IDC_RADIO17)))->GetCheck())//
	{

		bLostDesk = (bMyDesk+3)%PLAY_COUNT;
	}
	else if (((CButton *)(GetDlgItem(IDC_RADIO16)))->GetCheck())//
	{

		if(PLAY_COUNT == 2)
		{
			bLostDesk = (bMyDesk+1)%PLAY_COUNT;
		}
		else
		{
			bLostDesk = (bMyDesk+2)%PLAY_COUNT;
		}

		//bLostDesk = (bMyDesk+2)%PLAY_COUNT
	}
	else if (((CButton *)(GetDlgItem(IDC_RADIO19)))->GetCheck())//
	{

		bLostDesk = (bMyDesk+1)%PLAY_COUNT;
	}


	ControlWinStruct msg;
	msg.bWinStation = bWinDesk;
	msg.bLoseStation = bLostDesk;

	((CClientGameDlg *)m_pParentWnd)->SendGameData((void *)&msg,sizeof(msg),MDM_GM_GAME_NOTIFY,ASS_CONTROL_WIN,0);

	ShowWindow(SW_HIDE);
	((CButton *)(GetDlgItem(IDC_RADIO2)))->SetCheck(0);
	((CButton *)(GetDlgItem(IDC_RADIO3)))->SetCheck(0);
	OnOK();
}
