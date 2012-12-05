#include "stdafx.h"
#include "ClientGameDlg.h"
#include "PlayVideo.h"
#include "ClientView.h"
 
CPlayVideo*  CPlayVideo::m_pPlayVideo=NULL;

CPlayVideo::CPlayVideo()
{
	m_pPlayVideo=this;

}
CPlayVideo::~CPlayVideo()
{
	Video_Release();
}

CPlayVideo*  CPlayVideo::CreatePlayVideo()
{
 
	if(NULL != m_pPlayVideo)
		return m_pPlayVideo;
	else
	{
		return NULL;
	}
 }

bool  CPlayVideo::Video_Initialize( ST_INITVIDEO& stInitVideo)
{
	return __super::Video_Initialize( stInitVideo);
}

void	CPlayVideo::Video_Release()
{
	__super::Video_Release();
	//delete this;
	//m_pPlayVideo=NULL; //!!!
}

/*****************************************
	(IN)  name, ����ǳ�;
	(OUT) rect, �������Ƶ��������;
	���ɹ������rectʱ������true,����false;
******************************************/
#ifdef VIDEO
bool	CPlayVideo::GetVideoRectByName( CString name, RECT& rect, CString& pic )
{
	CClientPlayView *pPlayView =(CClientPlayView*)m_pParentWnd;
	 
	ASSERT(pPlayeView);
	if(pPlayView)
	{
		return pPlayView->GetVideoRect(name, rect, pic);
	}
	else
		return false;
	 
}
#endif