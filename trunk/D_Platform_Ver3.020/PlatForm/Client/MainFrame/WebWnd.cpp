// WebWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WebWnd.h"


// CWebWnd �Ի���

IMPLEMENT_DYNAMIC(CWebWnd, CDialog)

CWebWnd::CWebWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CWebWnd::IDD, pParent)
{

}

CWebWnd::~CWebWnd()
{
}

void CWebWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_ie);
}


BEGIN_MESSAGE_MAP(CWebWnd, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWebWnd ��Ϣ�������

void CWebWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(m_ie.m_hWnd)
		m_ie.MoveWindow(0,0,cx,cy);

}

void CWebWnd::Browse(CString strURL)
{
	m_ie.Navigate(strURL,0,0,0,0);
}
