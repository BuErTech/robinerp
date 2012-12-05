#include "StdAfx.h"
#include "GameNoteWnd.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CGameNoteWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/**
 * ���캯��
 */
CGameNoteWnd::CGameNoteWnd(void)
{
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
	TCHAR path[MAX_PATH];
	wsprintf(path,TEXT(".\\image\\waiting.bmp"));
	m_BackBmp.SetLoadInfo(path,true);
	m_Font.CreateFont(-12,0,0,0,400,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
	//AfxSetResourceHandle(GetModuleHandle(NULL));
}

/**
 * ��������
 */
CGameNoteWnd::~CGameNoteWnd(void)
{
}

/**
 * �ػ�����
 */
void CGameNoteWnd::OnPaint() 
{
	CPaintDC dc(this); 

	//�滭����
	CDC BackDC;	
	BackDC.CreateCompatibleDC(&dc);
	BackDC.SelectObject(m_BackBmp);
	//dc.BitBlt(0,0,m_BackBmp.GetWidth(),m_BackBmp.GetHeight(),&BackDC,0,0,SRCCOPY);
	::TransparentBlt(dc.GetSafeHdc(),0,0,m_BackBmp.GetWidth(),m_BackBmp.GetHeight(),BackDC.GetSafeHdc(),\
	0,0,m_BackBmp.GetWidth(),m_BackBmp.GetHeight(),RGB(255,0,255));
	ReleaseDC(&BackDC);

	
}

/**
 * �Զ�������С
 */
void CGameNoteWnd::AutoSize()
{
	BITMAP BmpInfo;	
	MoveWindow(0,0,m_BackBmp.GetWidth(),m_BackBmp.GetHeight());
}

/**
 * ������Ϣ
 */
void CGameNoteWnd::SetMessage(TCHAR * szMessage)
{
	m_strMessage.Empty();
	if (szMessage!=NULL)
	{
		ShowWindow(SW_SHOW);
	}
	else ShowWindow(SW_HIDE);
	return;
}
