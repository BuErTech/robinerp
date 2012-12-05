#include "StdAfx.h"
#include "GamePlace.h"
#include "NavigationDlg.h"

BEGIN_MESSAGE_MAP(CNavigationDlg, CGameFaceGo)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//�궨��
#define MAX_SPACE								25						//�����

//���캯��
CNavigationDlg::CNavigationDlg(CWnd * pParentWnd) : CGameFaceGo(IDD_NVAIGATION)
{
	m_pParentWnd=pParentWnd;
//	m_BackImage.SetLoadInfo(AfxGetInstanceHandle(),IDB_NAV_BACK,CGameImageLink::m_bAutoLock);
}

//��������
CNavigationDlg::~CNavigationDlg()
{
}

//���ݰ󶨺���
void CNavigationDlg::DoDataExchange(CDataExchange* pDX)
{
	CGameFaceGo::DoDataExchange(pDX);
}

//��ʼ������
BOOL CNavigationDlg::OnInitDialog()
{
	CGameFaceGo::OnInitDialog();

	return TRUE;
}

//�ػ�����
void CNavigationDlg::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	CRect DrawRect,ClientRect;
	dc.GetClipBox(&DrawRect);
	GetClientRect(&ClientRect);
/*	CGameImageHelper BackImageHandle(&m_BackImage);
	for (int i=DrawRect.left;i<DrawRect.right;i+=BackImageHandle.GetWidth())
	{
		BackImageHandle.BitBlt(dc.GetSafeHdc(),i,0);
	}*/
	dc.Draw3dRect(&ClientRect,RGB(255,255,255),RGB(0,0,0));

	return;
}

//����λ��
void CNavigationDlg::FixControlSize()
{
	//��ȡλ��
	UINT uAllWide=0;
	for (INT_PTR i=0;i<m_ButtonPtrArray.GetCount();i++)
	{
		uAllWide+=((CNavButtonInfo *)m_ButtonPtrArray.GetAt(i))->m_uButtonWide;
	}

	//�ƶ�λ��
	CRect ClientRect;
	GetClientRect(&ClientRect);
	UINT uSpaceWide=ClientRect.Width()/((UINT)m_ButtonPtrArray.GetCount()+1);
	if (uSpaceWide>MAX_SPACE) uSpaceWide=MAX_SPACE;
	UINT uBasePos=uSpaceWide;
	for (INT_PTR i=0;i<m_ButtonPtrArray.GetCount();i++)
	{
		CNavButtonInfo * pButtonInfo=(CNavButtonInfo *)m_ButtonPtrArray.GetAt(i);
		SafeMoveWindow(pButtonInfo->m_pPicButton,uBasePos,(ClientRect.Height()-pButtonInfo->m_uButtonHigh)/2,
			pButtonInfo->m_uButtonWide,pButtonInfo->m_uButtonHigh);
		uBasePos+=(uSpaceWide+pButtonInfo->m_uButtonWide);
	}

	return;
}

//���밴ť
bool CNavigationDlg::AddButton(UINT uButtonWide, UINT uButtonHigh, HINSTANCE hInstance, UINT uButtonID)
{
	return true;
}
