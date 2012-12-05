#include "Stdafx.h"
#include "StationView.h"

IMPLEMENT_DYNAMIC(CStationView, CWnd)

BEGIN_MESSAGE_MAP(CStationView, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()
#define VIEW_FRAME_TOP					20								//����ϸ�

/**
 * ���캯��
 */
CStationView::CStationView()
{
	m_iUpGrade=-1;
	m_iBeginStation=-1;
	m_iEndStation=-1;
	m_iMeStation=-1;
	m_iOtherStation=-1;
	m_iLessPlay=-1;	
	m_iPlayCount=0;	
	m_iNTStation=-1;
	m_iNTCount=0;
	m_iTurePoint=0;
	m_iPunishPoint=0;
	m_iPlayCardCount=2;
	m_bShowMessage=FALSE;
}

/**
 * ��������
 */
CStationView::~CStationView()
{
}

/**
 * ��������
 */
int CStationView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)	return -1;

	//�����ؼ�
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	m_PointCard.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,50,NULL);
	m_PointCard.SetBasePoint(CPoint(100,-13),CUpGradeGameCard::XShowMode::SWX_MODE_LEFT,CUpGradeGameCard::YShowMode::SWY_MODE_TOP);
	m_PointCard.SetShowCard(true);

//	m_PicBottomBar.SetLoadInfo(GetModuleHandle(CLIENT_DLL_NAME),IDB_BOTTOM_BAR,CGameImageLink::m_bAutoLock);
//	m_PicBottomBarLeft.SetLoadInfo(GetModuleHandle(CLIENT_DLL_NAME),IDB_BOTTOM_BAR_LEFT,CGameImageLink::m_bAutoLock);
//	m_PicBottomBarRight.SetLoadInfo(GetModuleHandle(CLIENT_DLL_NAME),IDB_BOTTOM_BAR_RIGHT,CGameImageLink::m_bAutoLock);
//	m_NTLogo.LoadBitmap(IDB_NTLOGO);
	//AfxSetResourceHandle(GetModuleHandle(NULL));

	return 0;
}

/**
 * �滭����
 */
void CStationView::OnPaint()
{
	CFont Font;
	CPaintDC dc(this);
	Font.CreateFont(-12,0,0,0,400,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
	dc.SelectObject(Font);
	dc.SetTextColor(RGB(0,0,0));
	dc.SetBkMode(TRANSPARENT);

	//�滭����
	/*
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(135,162,124));
	
	CGameImageHelper BottomBarHandle(&m_PicBottomBar);
	for(int i = 0; i < rect.Width(); i+=BottomBarHandle.GetWidth())
	{
		BottomBarHandle.BitBlt(dc.GetSafeHdc(),i,rect.Height() - BottomBarHandle.GetHeight());
	}

	CGameImageHelper BottomBarLeftHandle(&m_PicBottomBarLeft);
	BottomBarLeftHandle.BitBlt(dc.GetSafeHdc(),0,rect.Height() - BottomBarHandle.GetHeight());

	CGameImageHelper BottomBarRightHandle(&m_PicBottomBarRight);
	BottomBarRightHandle.BitBlt(dc.GetSafeHdc(),rect.Width() - BottomBarRightHandle.GetWidth(),rect.Height() - BottomBarHandle.GetHeight());
*/
//	dc.TextOut(1,1,"�汾��1.0.0.1");
	if (!m_bShowMessage) return;

	//�滭��Ϣ
	TCHAR szBuffer[50];
	TCHAR * szUpGrade[]={TEXT("���� -- �Լ�"),TEXT("�з� -- �Ҽ�"),TEXT("���� -- �Լ�"),TEXT("�з� -- ���")};
	TCHAR * szHua[]={TEXT("����"),TEXT("÷��"),TEXT("����"),TEXT("����")};
	TCHAR * szCard[13]={TEXT("2"),TEXT("3"),TEXT("4"),TEXT("5"),TEXT("6"),TEXT("7"),
						TEXT("8"),TEXT("9"),TEXT("10"),TEXT("J"),TEXT("Q"),TEXT("K"),
						TEXT("A")};
/*
	dc.TextOut(10, 5,TEXT("ׯ�ң�"));
	dc.TextOut(10,5,TEXT("������"));	
	dc.TextOut(10,22,TEXT("�з���"));
	dc.TextOut(10,44,TEXT("������"));
	dc.TextOut(10,39,TEXT("�Ѵ������"));
	dc.TextOut(10,56,TEXT("���֣�"));
	dc.TextOut(10,73,TEXT("���֣�"));

	if (m_iUpGrade!=-1) 
		dc.TextOut(50,5,szUpGrade[m_iUpGrade],lstrlen(szUpGrade[m_iUpGrade]));
	else 
		dc.TextOut(50,5,TEXT("��ʱ���˱�������ׯ��"));

	if (m_iMeStation!=-1)
	{
		sprintf(szBuffer,TEXT("��%s��%s�����ڴ�%s"),szCard[m_iBeginStation-2],szCard[m_iEndStation-2],szCard[m_iMeStation-2]);
		dc.TextOut(50,5,szBuffer,lstrlen(szBuffer));
	}
	if (m_iOtherStation!=-1)
	{
		sprintf(szBuffer,TEXT("��%s��%s�����ڴ�%s"),szCard[m_iBeginStation-2],szCard[m_iEndStation-2],szCard[m_iOtherStation-2]);
		dc.TextOut(50,22,szBuffer,lstrlen(szBuffer));
	}
	if (m_iNTStation!=-1)
	{
		sprintf(szBuffer,TEXT("%s [%s] ��%s��"),szHua[m_iNTHua>>4],szUpGrade[m_iNTStation]+8,
				(m_iPlayCardCount==2)?TEXT("������"):TEXT("������"));
		dc.TextOut(50,44,szBuffer,lstrlen(szBuffer));
	}
	if (m_iLessPlay!=-1)
	{
		sprintf(szBuffer,TEXT("%d �����ٻغϣ�%d��"),m_iPlayCount,m_iLessPlay);
		sprintf(szBuffer,TEXT("%d"),m_iPlayCount);
		dc.TextOut(80,39,szBuffer,lstrlen(szBuffer));
	}
	sprintf(szBuffer,TEXT("%d"),m_iTurePoint);
	dc.TextOut(50,56,szBuffer,lstrlen(szBuffer));
	if (m_iPunishPoint!=0)
	{
		sprintf(szBuffer,TEXT("%d"),m_iPunishPoint);
		dc.TextOut(50,73,szBuffer,lstrlen(szBuffer));
	}
	else 
		dc.TextOut(50,73,TEXT("��"),2);
*/
	dc.TextOut(10+VIEW_FRAME_LEFT,5+VIEW_FRAME_TOP,TEXT("������"));	
	dc.TextOut(10+VIEW_FRAME_LEFT,20+VIEW_FRAME_TOP,TEXT("�з���"));
	dc.TextOut(10+VIEW_FRAME_LEFT,35+VIEW_FRAME_TOP,TEXT("�Ѵ������"));
//	dc.TextOut(10+VIEW_FRAME_LEFT,50+VIEW_FRAME_TOP,TEXT("������"));
//	dc.TextOut(10+VIEW_FRAME_LEFT,65+VIEW_FRAME_TOP,TEXT("���֣�"));
	dc.TextOut(10+VIEW_FRAME_LEFT,50+VIEW_FRAME_TOP,TEXT("���ƣ�"));


	dc.TextOut(10+VIEW_FRAME_LEFT,750,TEXT("������"));	
	dc.TextOut(10+VIEW_FRAME_LEFT,20+VIEW_FRAME_TOP,TEXT("�з���"));
	dc.TextOut(10+VIEW_FRAME_LEFT,35+VIEW_FRAME_TOP,TEXT("�Ѵ������"));
//	dc.TextOut(10+VIEW_FRAME_LEFT,50+VIEW_FRAME_TOP,TEXT("������"));
//	dc.TextOut(10+VIEW_FRAME_LEFT,65+VIEW_FRAME_TOP,TEXT("���֣�"));
	dc.TextOut(10+VIEW_FRAME_LEFT,50+VIEW_FRAME_TOP,TEXT("���ƣ�"));
	if (m_iMeStation!=-1)
	{
		//sprintf(szBuffer,TEXT("%s��%s�����ڴ�%s"),szCard[m_iBeginStation-2],szCard[m_iEndStation-2],szCard[m_iMeStation-2]);
		///dc.TextOut(50+VIEW_FRAME_LEFT,5+VIEW_FRAME_TOP,szBuffer,lstrlen(szBuffer));
	}
	if (m_iOtherStation!=-1)
	{
		//sprintf(szBuffer,TEXT("%s��%s�����ڴ�%s"),szCard[m_iBeginStation-2],szCard[m_iEndStation-2],szCard[m_iOtherStation-2]);
		///dc.TextOut(50+VIEW_FRAME_LEFT,20+VIEW_FRAME_TOP,szBuffer,lstrlen(szBuffer));
	}

	if (m_iLessPlay!=-1)
	{
//		sprintf(szBuffer,TEXT("%d �����ٻغϣ�%d��"),m_StationView.m_iPlayCount,m_StationView.m_iLessPlay);
		//sprintf(szBuffer,TEXT("%d"),m_iPlayCount);
		//dc.TextOut(80+VIEW_FRAME_LEFT,35+VIEW_FRAME_TOP,szBuffer,lstrlen(szBuffer));
	}

//	sprintf(szBuffer,TEXT("%d"),m_StationView.m_iTurePoint);
//	dc.TextOut(50+VIEW_FRAME_LEFT,50+VIEW_FRAME_TOP,szBuffer,lstrlen(szBuffer));

	//if (m_StationView.m_iPunishPoint!=0)
	//{
	//	sprintf(szBuffer,TEXT("%d"),m_StationView.m_iPunishPoint);
	//	dc.TextOut(50+VIEW_FRAME_LEFT,65+VIEW_FRAME_TOP,szBuffer,lstrlen(szBuffer));
	//}
	//else 
	//	dc.TextOut(50+VIEW_FRAME_LEFT,65+VIEW_FRAME_TOP,TEXT("��"),2);

	if (m_iNTStation!=-1)
	{
		DrawNtLogo(&dc,50+VIEW_FRAME_LEFT,50+VIEW_FRAME_TOP,m_iNTHua);
		wsprintf(szBuffer,TEXT("%s %s"),szHua[m_iNTHua>>4],(m_iUpGrade==0 || m_iUpGrade==2)?szCard[m_iMeStation-2]:szCard[m_iOtherStation-2]);
		dc.TextOut(50+VIEW_FRAME_LEFT+17,50+VIEW_FRAME_TOP,szBuffer,lstrlen(szBuffer));
	}
	return;
}

/**
 * �滭 NT ͼ��
 */
void CStationView::DrawNtLogo(CDC * pDC, int x, int y, int iHuaKind)
{
	CDC NtDC;
	COLORREF crColor=0;
	NtDC.CreateCompatibleDC(pDC);
	NtDC.SelectObject(&m_NTLogo);
	int iBeginStation=(iHuaKind>>4)*15;
	for (int i=0;i<15;i++)
	{
		for (int j=0;j<15;j++)
		{
			crColor=NtDC.GetPixel(i+iBeginStation,j);
			if (crColor!=RGB(255,255,255)) pDC->SetPixel(x+i,y+j,crColor);
		}
	}
	return;
}

/**
 * ����ׯ��
 */
void CStationView::SetUpGrade(int iUpGradeStation)
{
	m_iUpGrade=iUpGradeStation;
	Invalidate(FALSE);
	return;
}

/**
 * ���ñ����ƾ���Ϣ
 */
void CStationView::SetMeSideInfo(int iBeginStation, int iEndStation, int iPlayStation)
{
	m_iBeginStation=iBeginStation;
	m_iEndStation=iEndStation;
	m_iMeStation=iPlayStation;
	Invalidate(FALSE);
	return;
}

/**
 * ���õз��ƾ���Ϣ
 */
void CStationView::SetOtherSideInfo(int iBeginStation, int iEndStation, int iPlayStation)
{
	m_iBeginStation=iBeginStation;
	m_iEndStation=iEndStation;
	m_iOtherStation=iPlayStation;
	Invalidate(FALSE);
	return;
}

/**
 * ����������Ϣ
 */
void CStationView::SetNTInfo(int iNTStation, int iNTHua, int iPlayCardCount)
{
	m_iNTStation=iNTStation;
	m_iNTHua=iNTHua;
	m_iPlayCardCount=iPlayCardCount;
	Invalidate(FALSE);
	return;
}

/**
 * ���ûغ���Ϣ
 */
void CStationView::SetPlayInfo(int iPlayCount, int iLessCount)
{
	m_iPlayCount=iPlayCount;
	m_iLessPlay=iLessCount;
	Invalidate(FALSE);
	return;
}

/**
 * �������
 */
void CStationView::AddPointCard(BYTE iCardList[], int iCardCount)
{
	BYTE iTempCard[36];
	int iCount=m_PointCard.GetCard(iTempCard,NULL);
	::CopyMemory(&iTempCard[iCount],iCardList,sizeof(BYTE)*iCardCount);
	m_PointCard.SetCard(iTempCard,NULL,iCardCount+iCount);
	return;
}

/**
 * �������
 */
void CStationView::SetTurePoint(int iTurePoint)
{
	m_iTurePoint=iTurePoint;
	Invalidate(FALSE);
	return;
}

/**
 * ���뷣��
 */
void CStationView::AddPunishPoint(int iPunishPoint)
{
	m_iPunishPoint+=iPunishPoint;
	Invalidate(FALSE);
	return;
}

/**
 * ������Ϣ
 */
void CStationView::CleanAllInfo(BOOL bAll)
{
	m_iNTStation=-1;
	m_iNTCount=0;
	if (bAll) 
	{
		m_iUpGrade=-1;
		m_iMeStation=-1;
		m_iOtherStation=-1;
		m_iPlayCount=0;	
		m_iBeginStation=-1;	
		m_iEndStation=-1;	
		m_iLessPlay=-1;	
		m_iTurePoint=0;
		m_iPunishPoint=0;
		m_iPlayCardCount=2;
	}
	m_PointCard.SetCard(NULL,NULL,0);
	Invalidate(FALSE);
	return;
}

/**
 * �Ƿ���ʾ��Ϣ
 */
void CStationView::SetShowMessage(BOOL bShow)
{
	m_bShowMessage=bShow;
	Invalidate(FALSE);
	return;
}

/**
 * ��������
 */
BOOL CStationView::Create(CWnd * pParentWnd, UINT nID)
{
	return CWnd::Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,CRect(0,0,0,0),pParentWnd,nID,NULL);
}

