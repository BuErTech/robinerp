#include "Stdafx.h"
//#include "CardWnd.h"
#include "ClientGameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CCardWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL				CCardWnd::m_bInitList=FALSE;			//�Ƿ������־
CBitmap				CCardWnd::m_CardBmp[3];					//�˿�ͼ
HCURSOR				CCardWnd::m_Cursor;

//���캯��
CCardWnd::CCardWnd()
{
	pgg=NULL;
	m_bShowCard=TRUE;
	m_ShowMode=SW_NO_MOVE;
	m_iMaxCardCount=0;
	m_iCardCount=0;
	m_bUseMouse=FALSE;
	m_pbUp=NULL;
	m_NowDownHitCard=0;
	m_piCardList=NULL;
	
	if (!m_bInitList)
	{
		AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
		//������Ϸ�˿�
		m_bInitList=TRUE;
		m_CardBmp[0].LoadBitmap(IDB_KIESS_CARD);
		m_CardBmp[1].LoadBitmap(IDB_KIESS_CARD_EX);
		m_CardBmp[2].LoadBitmap(IDB_CARD_BACK);
		m_Cursor=::LoadCursor(GetModuleHandle(CLIENT_DLL_NAME),MAKEINTRESOURCE(IDC_CURSOR));
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}
}

//��������
CCardWnd::~CCardWnd()
{
	if (m_piCardList!=NULL) delete [] m_piCardList;
	if (m_pbUp!=NULL) delete [] m_pbUp;
}

//�ػ�����
void CCardWnd::OnPaint() 
{
	CPaintDC dc(this); 

	//�滭����
	if (GetSystemMetrics(SM_CXSCREEN)>=1024)
	{
		for (int i=0;i<m_iCardCount;i++) DrawCard(&dc,CPoint(i*CARD_SPACE,*(m_pbUp+i)?0:20),*(m_piCardList+i),i);
	}
	else
	{
		for (int i=0;i<m_iCardCount;i++) DrawCard(&dc,CPoint(i*(CARD_SPACE-1),*(m_pbUp+i)?0:20),*(m_piCardList+i),i);
	}
}

//�滭�˿˺���
BOOL CCardWnd::DrawCard(CDC * pDC, POINT &Point, BYTE iCard, int iStation)
{
	//���� DC
	CDC CardDC;
	CardDC.CreateCompatibleDC(pDC);
	int cxStation=0,cyStation=0,iBmpStation=0;

	if ((iCard==0)||(m_bShowCard==FALSE)) iBmpStation=2; //�Ʊ�
	else if (iCard<=0x3D)	//����
	{
		iBmpStation=0;
		cxStation=iCard&0x0F;
		if (cxStation>=13) cxStation=0;
		cyStation=(iCard&0xF0)>>4;
	}
	else //����
	{
		iBmpStation=1;
		cxStation=iCard-0x4E;
	}

	//��ʼ�滭
	CardDC.SelectObject(&m_CardBmp[iBmpStation]);
	if (iStation!=(m_iCardCount-1))
	{
		int iWidth=0;
		pDC->BitBlt(Point.x,Point.y,CARD_SPACE,CARD_HIDE,&CardDC,cxStation*CARD_WIDTH,cyStation*CARD_HIDE,SRCCOPY);
		for (int i=1;i<=CARD_WIDTH/CARD_SPACE;i++)
		{
			//����ˢ�¿��
			if ((iStation+i)>=m_iCardCount)
			{
				iWidth=CARD_WIDTH;
				break;
			}
			if (*(m_pbUp+iStation)!=*(m_pbUp+iStation+i)) iWidth+=CARD_SPACE;
			else break;
		}
		if (iWidth!=0)
		{
			iWidth=__min(iWidth,CARD_WIDTH-CARD_SPACE);
			if (*(m_pbUp+iStation))
			{
				//�滭�ϲ�
				pDC->BitBlt(Point.x+CARD_SPACE,Point.y,iWidth,20,&CardDC,cxStation*CARD_WIDTH+CARD_SPACE,cyStation*CARD_HIDE,SRCCOPY);
			}
			else
			{
				//�滭�²�
				pDC->BitBlt(Point.x+CARD_SPACE,Point.y+CARD_HIDE-20,iWidth,20,&CardDC,cxStation*CARD_WIDTH+CARD_SPACE,(cyStation+1)*CARD_HIDE-20,SRCCOPY);
			}
		}
	}
	else pDC->BitBlt(Point.x,Point.y,CARD_WIDTH,CARD_HIDE,&CardDC,cxStation*CARD_WIDTH,cyStation*CARD_HIDE,SRCCOPY);

	return TRUE;
}

//���û�׼�����ʾ����
BOOL CCardWnd::SetBasePoint(POINT &BasePoint, int iShowMode)
{
	m_ShowMode=iShowMode;
	m_BasePoint=BasePoint;
	FixCardControl(FALSE);
	Invalidate(FALSE);
	return TRUE;
}

//��ȡ���ڴ�С
BOOL CCardWnd::GetWndSize(SIZE &Size)
{
	if (m_iCardCount==0)
	{
		Size.cx=0;
		Size.cy=0;
	}
	else
	{
		Size.cx=CARD_WIDTH+(m_iCardCount-1)*CARD_SPACE;
		Size.cy=CARD_HIDE+20;
	}
	return TRUE;
}

//��������λ��
BOOL CCardWnd::FixCardControl(BOOL bReDraw)
{
	CRect Rect;
	SIZE Size;
	GetWndSize(Size);
	GetClientRect(&Rect);

	switch (m_ShowMode)
	{
	case SW_NO_MOVE:
		{
			POINT Point;
			Point.x=Rect.left;
			Point.y=Rect.top;
			MapWindowPoints(GetParent(),&Point,1);
			Rect.SetRect(Point.x,Point.y,Point.x+Size.cx,Point.y+Size.cy);
			break;
		}
	case SW_MODE_LEFT:
		{
			POINT PointTurn[2];
			PointTurn[0].x=m_BasePoint.x;
			PointTurn[0].y=m_BasePoint.y;
			PointTurn[1].x=m_BasePoint.x+Size.cx;
			PointTurn[1].y=m_BasePoint.y+Size.cy;
			Rect.SetRect(PointTurn[0],PointTurn[1]);
			break;
		}
	case SW_MODE_CENTER:
		{
			POINT PointTurn[2];
			PointTurn[0].x=m_BasePoint.x-Size.cx/2;
			PointTurn[0].y=m_BasePoint.y;
			PointTurn[1].x=m_BasePoint.x+Size.cx/2;
			PointTurn[1].y=m_BasePoint.y+Size.cy;
			Rect.SetRect(PointTurn[0],PointTurn[1]);
			break;
		}
	case SW_MODE_RIGHT:
		{
			POINT PointTurn[2];
			PointTurn[0].x=m_BasePoint.x-Size.cx;
			PointTurn[0].y=m_BasePoint.y;
			PointTurn[1].x=m_BasePoint.x;
			PointTurn[1].y=m_BasePoint.y+Size.cy;
			Rect.SetRect(PointTurn[0],PointTurn[1]);
			break;
		}
	}
	MoveWindow(&Rect,TRUE);
	
	//��������
	CRgn AllRgn,SignedRgn;
	AllRgn.CreateRectRgn(0,0,0,0);
	GetClientRect(Rect);
	for (int i=0;i<m_iCardCount;i++)
	{
		SignedRgn.CreateRoundRectRgn(i*CARD_SPACE,*(m_pbUp+i)?0:20,i*CARD_SPACE+CARD_WIDTH,*(m_pbUp+i)?CARD_HIDE:116,2,2);
		AllRgn.CombineRgn(&AllRgn,&SignedRgn,RGN_OR);
		SignedRgn.DeleteObject();
	}
	SetWindowRgn(AllRgn,TRUE);
	
	return TRUE;
}

//��ʼ���˿�
BOOL CCardWnd::InitCard(int iCardCount)
{
	if (iCardCount>0)
	{
		BYTE * pTempCard=new BYTE [iCardCount];
		BOOL * pTempUp=new BOOL [iCardCount];
		if ((pTempCard==NULL)||(pTempUp==NULL))
		{
			delete [] pTempCard;
			delete [] pTempUp;
			return FALSE;
		}
		delete m_piCardList;
		delete m_pbUp;
		m_pbUp=pTempUp;
		m_piCardList=pTempCard;
		m_iMaxCardCount=iCardCount;
		m_iCardCount=0;
	}
	return TRUE;
}

//�����˿�
BOOL CCardWnd::SetCard(BYTE iCardList[], BOOL bUp[], int iCardCount)
{
	if (iCardCount>m_iMaxCardCount)
	{
		BYTE * pTempCard=new BYTE [iCardCount];
		BOOL * pTempUp=new BOOL [iCardCount];
		if ((pTempCard==NULL)||(pTempUp==NULL))
		{
			delete [] pTempCard;
			delete [] pTempUp;
			return FALSE;
		}
		delete m_piCardList;
		delete m_pbUp;
		m_pbUp=pTempUp;
		m_piCardList=pTempCard;
		m_iMaxCardCount=iCardCount;
	}

	if (iCardList!=NULL):: CopyMemory(m_piCardList,iCardList,sizeof(BYTE)*iCardCount);
	if (bUp!=NULL) ::CopyMemory(m_pbUp,bUp,sizeof(BOOL)*iCardCount);
	else memset(m_pbUp,FALSE,sizeof(int)*iCardCount);
	m_iCardCount=iCardCount;

	FixCardControl(TRUE);
	Invalidate();

	return TRUE;
}

//�������������Ϣ
void CCardWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if ((m_bUseMouse)&&(m_NowDownHitCard!=0))
	{
		int iHitCard=HitCardTest(point);
		if ((m_NowDownHitCard!=iHitCard)||(iHitCard==0))
		{
			m_NowDownHitCard=0;
			return;
		}
		*(m_pbUp+iHitCard-1)=*(m_pbUp+iHitCard-1)?FALSE:TRUE;
		FixCardControl();

		//�ػ�ʧЧ����
		CRect Rect;
		Rect.SetRect((iHitCard-1)*CARD_SPACE,0,(iHitCard-1)*CARD_SPACE+CARD_WIDTH,116);
		InvalidateRect(Rect,FALSE);

		//������Ϣ
		CClientGameDlg *p=CONTAINING_RECORD(pgg,CClientGameDlg,m_PlayView);
		p->SendMessage(IDM_MOUSE_HIT,0,0);
	}
	return;
}

//�������������Ϣ
void CCardWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bUseMouse) m_NowDownHitCard=HitCardTest(point);
	return;
}

//���ù��
BOOL CCardWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_bUseMouse) 
	{
		SetCursor(CGameImageLink::m_hHandCursor);
		return TRUE;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

//�����Ƿ�ʹ�����
BOOL CCardWnd::SetUseMouse(BOOL bUserMouse)
{
	m_bUseMouse=bUserMouse;
	return TRUE;
}

//��ȡ�˿���Ϣ
int CCardWnd::GetCard(BYTE iCard[], BOOL bUp[])
{
	if (iCard!=NULL) CopyMemory(iCard,m_piCardList,m_iCardCount*sizeof(BYTE));
	if (bUp!=NULL) CopyMemory(bUp,m_pbUp,m_iCardCount*sizeof(BOOL));
	return m_iCardCount;
}

//��������
int CCardWnd::HitCardTest(CPoint & point)
{
	//Ѱ�Ұ����˿�
	int iCardIndex=(point.x/CARD_SPACE+(point.x%CARD_SPACE==0?0:1));
	if (iCardIndex==0) iCardIndex=1;
	if (iCardIndex>m_iCardCount) iCardIndex=m_iCardCount;

	//�жϰ���λ��
	BOOL bHitThisCard=TRUE;
	BOOL bCardUp=*(m_pbUp+iCardIndex-1);
	if ((point.y<20)||(point.y>CARD_HIDE))
	{
		if (((bCardUp==TRUE)&&(point.y>CARD_HIDE))||((bCardUp==FALSE)&&(point.y<20))) bHitThisCard=FALSE;
	}

	//Ѱ��ǰ������˿�
	if (bHitThisCard==FALSE)
	{
		int iFindCount=CARD_WIDTH/CARD_SPACE+1;
		for (int i=1;i<iFindCount;i++)
		{
			if ((iCardIndex-i)==0) break;
			bCardUp=*(m_pbUp+iCardIndex-i-1);
			if (((bCardUp==FALSE)&&(point.y>CARD_HIDE))||((bCardUp==TRUE)&&(point.y<20)))
				return iCardIndex-i;
		}
		return 0;
	}

	return iCardIndex;
}

//��ȡ������˿�
int	CCardWnd::GetUpCard(BYTE iCard[])
{
	int iUpCount=0;
	for (int i=0;i<m_iCardCount;i++)
	{
		if (*(m_pbUp+i)==TRUE) 
		{
			if (iCard!=NULL) iCard[iUpCount]=*(m_piCardList+i);
			iUpCount++;
		}
	}
	return iUpCount;
}

//ɾ��������˿�
int CCardWnd::RemoveUpCard()
{
	for (int i=0;i<m_iCardCount;i++)
	{
		if (*(m_pbUp+i)==TRUE) 
		{
			m_iCardCount--;
			::MoveMemory(m_pbUp+i,m_pbUp+i+1,sizeof(BOOL)*(m_iCardCount-i));
			::MoveMemory(m_piCardList+i,m_piCardList+i+1,sizeof(BYTE)*(m_iCardCount-i));
			i--;
		}
	}
	FixCardControl(TRUE);
	return m_iCardCount;
}

//������Ϣ
int CCardWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)	return -1;

	//���ñ���ˢ
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return 0;
}

//�����Ϣ
void CCardWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_bUseMouse) 
	{
		CClientGameDlg *p=CONTAINING_RECORD(pgg,CClientGameDlg,m_PlayView);
		p->SendMessage(IDM_OUT_CARD,0,0);
	}
	return;
}

//�����Ƿ���ʾ�˿�
BOOL CCardWnd::ShowCard(BOOL bShow)
{
	m_bShowCard=bShow;
	Invalidate(FALSE);
	return TRUE;
}
