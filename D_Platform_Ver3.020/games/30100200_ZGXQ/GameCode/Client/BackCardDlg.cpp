#include "Stdafx.h"
#include "Resource.h"
#include "BackCardDlg.h"


BEGIN_MESSAGE_MAP(CBackCardDlg, CGameFaceGo)
END_MESSAGE_MAP()

/**
 * ���캯��
 */
CBackCardDlg::CBackCardDlg(CWnd * pParent) : CGameFaceGo(CBackCardDlg::IDD)
{
	m_iBackCount=0;
}

/**
 * ��������
 */
CBackCardDlg::~CBackCardDlg()
{
}

/**
 * DDX/DDV ֧��
 */
void CBackCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CGameFaceGo::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOk);
}

/**
 * ���õ���
 */
void CBackCardDlg::SetBackCard(BYTE iCardList[], int iCardCount)
{
	m_iBackCount=iCardCount;
	::CopyMemory(m_iBackCard,iCardList,sizeof(BYTE)*iCardCount);
	if (m_BackCard.GetSafeHwnd()) m_BackCard.SetCard(m_iBackCard,NULL,m_iBackCount);
	return;
}

/**
 * ��ʼ������
 */
BOOL CBackCardDlg::OnInitDialog()
{
	CGameFaceGo::OnInitDialog();

	RECT Rect;
	m_BackCard.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,CRect(0,0,0,0),this,20,NULL);
	m_BackCard.SetCard(m_iBackCard,NULL,m_iBackCount);
	m_BackCard.SetShowCard(true);
	GetClientRect(&Rect);
	m_BackCard.SetBasePoint(CPoint(Rect.right/2,-10),CUpGradeGameCard::XShowMode::SWX_MODE_CENTER,CUpGradeGameCard::YShowMode::SWY_MODE_TOP);

	return TRUE;
}

/**
 * ȡ������
 */
void CBackCardDlg::OnCancel()
{
	__super::OnCancel();
//	CGameFaceGo::OnClose();
}

/**
 * ȷ������
 */
void CBackCardDlg::OnOK()
{
	__super::OnOK();
//	CGameFaceGo::OnClose();
}
