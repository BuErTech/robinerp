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

//���캯��
CGameNoteWnd::CGameNoteWnd(void)
{
	AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
	
	TCHAR path[MAX_PATH] = {0};
	wsprintf(path,TEXT(".\\image\\%s\\waiting.bmp"),SKIN_FOLDER);
	m_BackBmp.SetLoadInfo(path,true);
	m_Font.CreateFont(-12,0,0,0,400,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
	AfxSetResourceHandle(GetModuleHandle(NULL));
}

//��������
CGameNoteWnd::~CGameNoteWnd(void)
{
}

//�ػ�����
void CGameNoteWnd::OnPaint() 
{
	CPaintDC dc(this); 
	CGameImageHelper image(&m_BackBmp);
	image.BitBlt(dc.GetSafeHdc(),0,0,SRCCOPY);

	//�滭����
	RECT ClientRect;
	GetClientRect(&ClientRect);
	dc.SelectObject(m_Font);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,0));
	//dc.DrawText(m_strMessage,&ClientRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

//�Զ�������С
void CGameNoteWnd::AutoSize()
{
	MoveWindow(0,0,m_BackBmp.GetWidth(),m_BackBmp.GetHeight());
}

//������Ϣ
void CGameNoteWnd::SetMessage(TCHAR * szMessage)
{
	m_strMessage.Empty();
	if (szMessage!=NULL)
	{
		m_strMessage=szMessage;
		/*Invalidate();
		UpdateWindow();
		ShowWindow(SW_SHOW);*/
	}
	else ShowWindow(SW_HIDE);
	return;
}
