#include "Stdafx.h"
#include "FindDesk.h"
#include "GameRoomEx.h"
#include "DeskFrameView.h"

BEGIN_MESSAGE_MAP(CFindDesk, CAFCBaseDialog)
	ON_BN_CLICKED(IDC_SITDOWN, OnBnClickedSitdown)
END_MESSAGE_MAP()

//���캯��
CFindDesk::CFindDesk(CWnd * pParent) : CAFCBaseDialog(IDD_FIND_DESK,pParent)
{
	m_bFindOne=false;			
	m_bFindTwo=false;			
	m_bFindAllNull=false;		
	m_bFindNoAllNull=false;		
	m_bLastFindDesk=255;
	m_pParent=pParent;
}

//��������
CFindDesk::~CFindDesk()
{
}

//�ؼ��󶨺���
void CFindDesk::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_BtFind);
	DDX_Control(pDX, IDCANCEL, m_BtCancel);
	DDX_Control(pDX, IDC_SITDOWN, m_BtSitDown);
}

//��ʼ������
BOOL CFindDesk::OnInitDialog() 
{
	__super::OnInitDialog();

	//���ñ���
	if (m_bFindOne) ((CButton *)GetDlgItem(IDC_FIND_ONE_NULL))->SetCheck(BST_CHECKED);
	if (m_bFindTwo) ((CButton *)GetDlgItem(IDC_FIND_TWO_NULL))->SetCheck(BST_CHECKED);
	if (m_bFindNoAllNull) ((CButton *)GetDlgItem(IDC_NOT_ALL_NULL))->SetCheck(BST_CHECKED);
	if (m_bFindAllNull) ((CButton *)GetDlgItem(IDC_ALL_NULL))->SetCheck(BST_CHECKED);

	return TRUE; 
}

//ȡ��
void CFindDesk::OnCancel() 
{
	m_bLastFindDesk=255;
	DestroyWindow();
}

//����λ
void CFindDesk::OnOK() 
{
	//�������
	CGameRoomEx * pGameRoom=(CGameRoomEx *)m_pParent;
	CDeskFrameView * pDeskFrame=&pGameRoom->m_DeskFrame;
	bool bFindOne=(((CButton *)GetDlgItem(IDC_FIND_ONE_NULL))->GetCheck()!=0)?true:false;
	bool bFindTwo=(((CButton *)GetDlgItem(IDC_FIND_TWO_NULL))->GetCheck()!=0)?true:false;
	bool bFindNoAllNull=(((CButton *)GetDlgItem(IDC_NOT_ALL_NULL))->GetCheck()!=0)?true:false;
	bool bFindAllNull=(((CButton *)GetDlgItem(IDC_ALL_NULL))->GetCheck()!=0)?true:false;

	//Ч�����
	if ((bFindOne==false)&&(bFindTwo==false)&&(bFindNoAllNull==false)&&(bFindAllNull==false))
	{
		//AfxMessageBox(TEXT("��Ҫ����λ����Ҫ���������ģ�ѡ����Ҫ�������ɣ�"),MB_ICONQUESTION);
		AfxMessageBox(TEXT("����ѡ��������"),MB_ICONQUESTION);
		return;
	}

	//�����������
	if (bFindOne!=m_bFindOne)				{ m_bFindOne=bFindOne;m_bLastFindDesk=255; }
	if (bFindTwo!=m_bFindTwo)				{ m_bFindTwo=bFindTwo;m_bLastFindDesk=255; }
	if (bFindNoAllNull!=m_bFindNoAllNull)	{ m_bFindNoAllNull=bFindNoAllNull;m_bLastFindDesk=255; }
	if (bFindAllNull!=m_bFindAllNull)		{ m_bFindAllNull=bFindAllNull;m_bLastFindDesk=255; }

	//��ʼ����λ��
	m_bLastFindDesk=pDeskFrame->FindGameDesk(m_bLastFindDesk,bFindOne,bFindTwo,bFindNoAllNull,bFindAllNull);
	if (m_bLastFindDesk!=255)
	{
		CString strBuffer;
		strBuffer.Format(TEXT("�ҵ� %2d ����Ϸ����������"),m_bLastFindDesk+1);
		GetDlgItem(IDC_RESULT_MESSAGE)->SetWindowText(strBuffer);
	}
	else GetDlgItem(IDC_RESULT_MESSAGE)->SetWindowText(TEXT("û�з������������ӣ����޸��������ң�"));

	return;	
}

//����
void CFindDesk::OnBnClickedSitdown()
{
	if (m_bLastFindDesk!=255)
	{
		CGameRoomEx * pGameRoom=(CGameRoomEx *)m_pParent;
		pGameRoom->m_dwCurrentUserID=0L;
		BYTE bDeskStation=pGameRoom->m_DeskFrame.GetNullStation(m_bLastFindDesk);
		if (bDeskStation!=255)
		{
			pGameRoom->PostMessage(WM_LEFT_MOUSE_HIT_DESK,m_bLastFindDesk,bDeskStation);
			OnCancel();
		}
		else 
			//GetDlgItem(IDC_RESULT_MESSAGE)->SetWindowText("���λ���Ѿ����˽����ȵ��ˣ�");
			GetDlgItem(IDC_RESULT_MESSAGE)->SetWindowText("���λ���Ѿ����˽����ȵ��ˣ�");
		return;
	}

	//AfxMessageBox(TEXT("û�з���Ҫ���λ�ã����޸��������ң�"),MB_ICONQUESTION);
	AfxMessageBox(TEXT("û���ҵ�����Ҫ���λ�ã����޸��������ң�"),MB_ICONQUESTION);

	return;
}

