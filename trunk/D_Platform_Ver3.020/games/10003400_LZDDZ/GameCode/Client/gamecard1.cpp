#include "Stdafx.h"
#include "Resource.h"
#include "GameCard.h"
#include "ClientGameDlg.h"
//��̬����
CGameImage				CUpGradeGameCard::m_CardPic[5];								//������

BEGIN_MESSAGE_MAP(CUpGradeGameCard, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

//���캯��
CUpGradeGameCard::CUpGradeGameCard(BYTE bCardStyle)
{
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
	m_bCardCount=0;
	SetCardStyle(bCardStyle);
	SetCardParameter(16,13,20);
	m_pHandleWnd=NULL;
	m_bHorzLook=true;
	m_bShowCard=false;
	m_bUseMouse=false;
	m_bHotHitCard=255;
	m_bSelect=false;
	m_PtBasePoint.SetPoint(0,0);
	m_bFriendCardCount=0;			//��������
	m_bShowNum = 0;
	m_XShowMode=XShowMode::SWX_MODE_CENTER;
	m_YShowMode=YShowMode::SWY_MODE_CENTER;
	m_iSourcePoint = 0 ;
	m_iBackPoint = 0;
	m_iBackMultiple = 1;
//	m_bLButtonDown = FALSE;
	m_ptLeftHit = CPoint(0,0);
	m_bCanMove = FALSE;
	//AfxSetResourceHandle(GetModuleHandle(NULL));
}

//��������
CUpGradeGameCard::~CUpGradeGameCard()
{
}

//��ʼ������
bool CUpGradeGameCard::InitGameCard(CWnd * pHandleWnd)
{
	m_pHandleWnd=pHandleWnd;
	return true;
}

//�����˿˲���
void CUpGradeGameCard::SetCardParameter(int iHSpace, int iVSpace, int iUpSpace)
{
	m_iCardHSpace=iHSpace;
	m_iCardVSpace=iVSpace;
	m_iCardUpSpace=iUpSpace;
	FixCardControl(false);
	return;
}

//��ȡ��С����
bool CUpGradeGameCard::GetCardSize(CSize & Size)
{
	if (m_bCardCount>0)
	{
		if (m_bHorzLook==true)
		{
			Size.cx=m_iCardWidth+(m_bCardCount-1)*m_iCardHSpace;
			Size.cy=m_iCardHight+m_iCardUpSpace;
		}
		else
		{
			Size.cx=m_iCardWidth;
			Size.cy=m_iCardHight+(m_bCardCount-1)*m_iCardVSpace;
		}
	}
	else
	{
		Size.cx=0;
		Size.cy=0;
	}
	return true;
}

//���û�׼�㺯��
void CUpGradeGameCard::SetBasePoint(CPoint & BasePoint, XShowMode XShowMode, YShowMode YShowMode)
{
	m_XShowMode=XShowMode;
	m_YShowMode=YShowMode;
	m_PtBasePoint=BasePoint;
	FixCardControl();
	return;
}

//������ʾ�˿�
void CUpGradeGameCard::SetShowCard(bool bShow)
{
	m_bShowCard=bShow;
	Invalidate(FALSE);
	return;
}

//����ʹ�����
void CUpGradeGameCard::SetUseMouse(bool bUserMouse)
{
	m_bUseMouse=bUserMouse;
	return;
}

//������ʾ��ʽ
void CUpGradeGameCard::SetLookMode(bool bHorzLook)
{
	m_bHorzLook=bHorzLook;
	return;
}

//�����˿�
bool CUpGradeGameCard::SetCard(BYTE bCardList[], BYTE bUpList[], BYTE bCardCount)
{
	if(bCardList == NULL)
	{
		m_iSourcePoint = 0;
		m_iBackPoint = 0; 
		m_iBackMultiple = 1;
	}
	if (bCardCount>m_CardArray.GetSize()) m_CardArray.SetSize(bCardCount);
	if (bCardCount>m_CardUpArray.GetSize()) m_CardUpArray.SetSize(bCardCount);
	if (bCardList!=NULL) CopyMemory(m_CardArray.GetData(),bCardList,sizeof(BYTE)*bCardCount);
	if (bUpList!=NULL) CopyMemory(m_CardUpArray.GetData(),bUpList,sizeof(BYTE)*bCardCount);
	else memset(m_CardUpArray.GetData(),0,sizeof(BYTE)*bCardCount);
	m_bCardCount=bCardCount;
	if(((CClientGameDlg*)m_pHandleWnd)->GetGameStatus()==0)
		FixCardControl(false);
	else
		FixCardControl();
	
	return true;
}

//��ȡ������˿�
BYTE CUpGradeGameCard::GetUpCard(BYTE bCardList[])
{
	BYTE bUpCount=0;
	for (BYTE i=0;i<m_bCardCount;i++)
	{
		if (m_CardUpArray.GetAt(i)==TRUE)
		{
			if (bCardList!=NULL)
				bCardList[bUpCount]=m_CardArray.GetAt(i);
			bUpCount++;
		}
	}
	return bUpCount;
}

//��ȡ�˿���Ϣ
BYTE CUpGradeGameCard::GetCard(BYTE bCardList[], BYTE bUpList[])
{
	if (bCardList!=NULL)
		CopyMemory(bCardList,m_CardArray.GetData(),m_bCardCount*sizeof(BYTE));
	if (bUpList!=NULL)
		CopyMemory(bUpList,m_CardUpArray.GetData(),m_bCardCount*sizeof(BYTE));
	return m_bCardCount;
}

//��������
BYTE CUpGradeGameCard::HitCardTest(CPoint & Point)
{
	if (m_bHorzLook==true)
	{
		//Ѱ�Ұ����˿�
		BYTE bCardIndex=(BYTE)(Point.x/m_iCardHSpace);
		if (bCardIndex>=m_bCardCount)
			bCardIndex=m_bCardCount-1;

		//�жϰ���λ��
		bool bHitThisCard=true;
		BYTE bCardUp=m_CardUpArray.GetAt(bCardIndex);
		if ((bCardUp==TRUE)&&(Point.y>m_iCardHight)) 
			bHitThisCard=false;
		if ((bCardUp==FALSE)&&(Point.y<m_iCardUpSpace))
			bHitThisCard=false;

		//Ѱ��ǰ������˿�
		if (bHitThisCard==false)
		{
			BYTE bFindCount=m_iCardWidth/m_iCardHSpace+1;
			for (BYTE i=1;i<bFindCount;i++)
			{
				if (bCardIndex==0)
					break;
				if(bCardIndex-i < 0)
					break;
				bCardUp=m_CardUpArray.GetAt(bCardIndex-i);
				if ((bCardUp==FALSE)&&(Point.y>m_iCardWidth)) 
					return bCardIndex-i;
				if ((bCardUp==TRUE)&&(Point.y<m_iCardUpSpace))
					return bCardIndex-i;
			}
			return 255;
		}
		return bCardIndex;
	}
	return 255;
}

//��������(�Ƿ���ĳ�����Ϸ�)
BYTE CUpGradeGameCard::HitCardTestAbove(CPoint &Point)
{
	if (m_bHorzLook==true)
	{
		//Ѱ�Ұ����˿�
		BYTE bCardIndex=(BYTE)(Point.x/m_iCardHSpace);
		if (bCardIndex>=m_bCardCount)
			bCardIndex=m_bCardCount-1;
		
		//�жϰ���λ��
	/*	bool bHitThisCard=true;
		BYTE bCardUp=m_CardUpArray.GetAt(bCardIndex);
		if ((bCardUp==TRUE)&&(Point.y>m_iCardHight)) 
			bHitThisCard=false;
		if ((bCardUp==FALSE)&&(Point.y>m_iCardHight))
			bHitThisCard=false;

		//Ѱ��ǰ������˿�
		if (bHitThisCard==false)
		{
			BYTE bFindCount=m_iCardWidth/m_iCardHSpace+1;
			for (BYTE i=1;i<bFindCount;i++)
			{
				if (bCardIndex==0) break;
				if(bCardIndex-i < 0)
					break;
				bCardUp=m_CardUpArray.GetAt(bCardIndex-i);
				if ((bCardUp==FALSE)&&(Point.y>m_iCardWidth)) return bCardIndex-i;
				if ((bCardUp==TRUE)&&(Point.y<m_iCardUpSpace)) return bCardIndex-i;
			}
			return 255;
		}*/
		return bCardIndex;
	}
	return 255;
}

//ɾ��������˿�
BYTE CUpGradeGameCard::RemoveUpCard()
{
	for (BYTE i=0;i<m_bCardCount;i++)
	{
		if (m_CardUpArray.GetAt(i)==TRUE)
		{
			m_bCardCount--;
			m_CardArray.RemoveAt(i);
			m_CardUpArray.RemoveAt(i);
			i--;
		}
	}
	FixCardControl();
	return m_bCardCount;
}

//��������λ��
void CUpGradeGameCard::FixCardControl(bool bInvalide)
{
	if(!this->m_hWnd)
		return;
	//��ȡλ��
	CSize Size;
	GetCardSize(Size);

	//����λ��
	CRect CardRect;
	switch (m_XShowMode)
	{
	case SWX_MODE_LEFT: { CardRect.left=m_PtBasePoint.x; break; }
	case SWX_MODE_CENTER: {	CardRect.left=m_PtBasePoint.x-Size.cx/2; break; }
	case SWX_MODE_RIGHT: { CardRect.left=m_PtBasePoint.x-Size.cx; break; }
	}

	switch (m_YShowMode)
	{
	case SWY_MODE_TOP: { CardRect.top=m_PtBasePoint.y; break; }
	case SWY_MODE_CENTER: {	CardRect.top=m_PtBasePoint.y-Size.cy/2;	break; }
	case SWY_MODE_BOTTOM: { CardRect.top=m_PtBasePoint.y-Size.cy; break; }
	}

	//�ƶ�λ��
	CardRect.right=CardRect.left+Size.cx;
	CardRect.bottom=CardRect.top+Size.cy;
	MoveWindow(&CardRect);
//	if (bInvalide)
//		Invalidate(TRUE);
	//��������
	CRgn AllRgn,SignedRgn;
	AllRgn.CreateRectRgn(0,0,0,0);
	for (BYTE i=0;i<m_bCardCount;i++)
	{
		if (m_bHorzLook==true)
		{
			BYTE bUpCard=m_CardUpArray.GetAt(i);
			SignedRgn.CreateRoundRectRgn(i*m_iCardHSpace,(bUpCard==TRUE)?0:m_iCardUpSpace,
				i*m_iCardHSpace+m_iCardWidth+1,(bUpCard==TRUE)?m_iCardHight+1:m_iCardUpSpace+m_iCardHight+1,2,2);
			AllRgn.CombineRgn(&AllRgn,&SignedRgn,RGN_OR);
			SignedRgn.DeleteObject();
		}
		else 
		{
			SignedRgn.CreateRoundRectRgn(0,i*m_iCardVSpace,m_iCardWidth+1,i*m_iCardVSpace+m_iCardHight+1,2,2);
			AllRgn.CombineRgn(&AllRgn,&SignedRgn,RGN_OR);
			SignedRgn.DeleteObject();
		}
	}

	//��������
//	m_CardViewRgn.CopyRgn(&AllRgn);
	SetWindowRgn(AllRgn,TRUE);
	AllRgn.DeleteObject();
//	m_CardViewRgn.DeleteObject();
//	m_CardViewRgn.Attach(AllRgn.Detach());
	//�ػ�����
//	if (bInvalide)
	Invalidate(FALSE);
	return;
}

//������Ϣ
int CUpGradeGameCard::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct)==-1) return -1;
//	m_ToolTip.Create(this);
//	m_ToolTip.SetDelayTime(10);
	SetCardImage();
	SetCardWidthHeight(0,0);

	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	return 0;
}

//�ػ�����
void CUpGradeGameCard::OnPaint()
{
	CPaintDC dc(this);
	if (m_bCardCount==0) return;

	//��������
	CDC BufferDC;
	CSize CardSize;
	CBitmap BufferBmp;
	GetCardSize(CardSize);
	BufferDC.CreateCompatibleDC(&dc);
	BufferBmp.CreateCompatibleBitmap(&dc,CardSize.cx,CardSize.cy);
	CBitmap * pOldBmp=BufferDC.SelectObject(&BufferBmp);
	//���
	//BufferDC.SelectObject(GetStockObject(NULL_BRUSH));
	//::Rectangle(BufferDC.GetSafeHdc(),0,0,CardSize.cx,CardSize.cy);
	//::FillRect(BufferDC.GetSafeHdc(),CRect(0,0,CardSize.cx,CardSize.cy),(HBRUSH)GetStockObject(NULL_BRUSH));
	//����λͼ
	//	CAFCImageHandle CardHandle(&m_CardPic[0]);
	//	CGameImageHelper CardHandle(&m_CardPic[0]);
	CGameImageHelper CardHandle(&m_CardPic[0]);
	CGameImageHelper BackHandle(&m_CardPic[1]);
	CGameImageHelper SmallHandle(&m_CardPic[2]);		//С��
	CGameImageHelper SmallBackHandle(&m_CardPic[3]);	//С�ƴ�Сè
	CGameImageHelper LaiZiCardsHandle(&m_CardPic[4]);   //�����

	//�滭�˿�
	int iXPos=0,iYPos=0;
	CGameImageHelper * pActiveHandle=NULL;
	for (BYTE i=0;i<m_bCardCount;i++)
	{
		BYTE bCard=m_CardArray.GetAt(i);
		BYTE bUpCard=m_CardUpArray.GetAt(i);

		if ((m_bShowCard==true)&&bCard != 0)//(m_CardArray.GetAt(i)!=0))
		{
			if ((bCard==0x4E)||(bCard==0x4F))
			{
				if(IMAGE_SMALL == GetCardStyle() )
				{
					iXPos=(bCard-0x4D)*m_iCardWidth;
					iYPos=0;
					pActiveHandle=&SmallBackHandle;		
				}
				else
				{
					iXPos=(bCard-0x4D)*m_iCardWidth;
					iYPos=0;
					pActiveHandle=&BackHandle;		
				}
			}
			else
			{
				if(IMAGE_SMALL == GetCardStyle() )
				{
					pActiveHandle=&SmallHandle;
					iXPos=GetCardXPos(bCard)*m_iCardWidth;
					iYPos=GetCardYPos(bCard)*m_iCardHight;
				}else
				{
					pActiveHandle=&CardHandle;
					iXPos=GetCardXPos(bCard)*m_iCardWidth;
					iYPos=GetCardYPos(bCard)*m_iCardHight;
				}
				if (GetCardNum(bCard) == GetCardNum(((CClientPlayView*)GetParent())->m_bLaiCardList[0]) 
					|| (0 == i && (((CClientPlayView*)GetParent())->m_bIsFakeBomb)) && !m_bUseMouse   )//���������
				{
					pActiveHandle = &LaiZiCardsHandle;
				}
			}
		}
		else
		{
			iXPos=0;
			iYPos=0;
			if(IMAGE_SMALL == GetCardStyle() )
			{
				pActiveHandle=&SmallBackHandle;
				if (GetCardNum(bCard) == GetCardNum(((CClientPlayView*)GetParent())->m_bLaiCardList[0]) 
					|| (0 == i && (((CClientPlayView*)GetParent())->m_bIsFakeBomb)) && !m_bUseMouse )//���������
				{
					pActiveHandle = &LaiZiCardsHandle;
				}
			}
			else
				pActiveHandle=&BackHandle;
		}


		if (m_bHorzLook==true) 
			pActiveHandle->BitBlt(BufferDC,i*m_iCardHSpace,bUpCard?0:m_iCardUpSpace,m_iCardWidth,m_iCardHight,iXPos,iYPos);
		else 
			pActiveHandle->BitBlt(BufferDC,0,i*m_iCardVSpace,m_iCardWidth,m_iCardHight,iXPos,iYPos);
	}

	//ˢ�½���
	dc.BitBlt(0,0,CardSize.cx,CardSize.cy,&BufferDC,0,0,SRCCOPY);
	//͸������
//	dc.TransparentBlt(0,0,CardSize.cx,CardSize.cy,&BufferDC,0,0,CardSize.cx,CardSize.cy,RGB(255,255,255));
	
	CRect rect,rect1;
	//���������
	rect.left=0;
	rect.right=CardSize.cx;
	if(m_bHorzLook)
		rect.top = CardSize.cy - m_iCardHight;
	else 
		rect.top =0;
	rect.bottom=CardSize.cy;

	switch(m_bShowNum)
	{
	case 1:
		if (m_bHorzLook==true) 		//С����
		{
			rect1.left =  rect.right - m_iCardWidth;
			rect1.top =  rect.top;
			rect1.bottom = rect.bottom;
			rect1.right = rect.right;
		}else
		{
			rect1.left =  rect.left ;
			rect1.top =  rect.bottom - m_iCardHight;
			rect1.bottom = rect.bottom ;
			rect1.right = rect.right;;
		}
		break;	
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
	int iNumWidth = 25,iNumHeight = 28;
	////int iBmpWidth=rect1.Width(),iBmpHeight=rect1.Height();
	//int iBmpWidth=iNumWidth*2;
	//int iBmpHeight=iNumHeight;

	CDC NumDC;//,SrcNumDC;
	//CBitmap NumBmp;
	NumDC.CreateCompatibleDC(&dc);
	//SrcNumDC.CreateCompatibleDC(&dc);

	CGameImageHelper Num(&m_Num);

	//NumBmp.CreateCompatibleBitmap(&dc,iBmpWidth,iBmpHeight);
	NumDC.SelectObject(Num);
	//CBrush cBrush;
	//cBrush.CreateSolidBrush(RGB(255,0,255));
	//NumDC.SelectObject(cBrush);
	//NumDC.FillRect(rect,&cBrush);
	//NumDC.FillRect(rect1,&cBrush);
	//SrcNumDC.SelectObject(Num);
	int xoffset =(rect1.Width()-( m_bCardCount >=10?2:1)*iNumWidth)/2;
	int yoffset = (rect1.Height()-iNumHeight)/2;
	int num1 =m_bCardCount/10 ,num2 = m_bCardCount%10;
	if (m_bShowNum)
	{
		if(m_bCardCount >= 10)
		{
			//NumDC.BitBlt(0,0,iNumWidth,iNumHeight,&SrcNumDC,num1*iNumWidth,0,SRCCOPY);
			dc.TransparentBlt(rect1.left+xoffset,rect1.top+yoffset,iNumWidth,iNumHeight,&NumDC,num1*iNumWidth,0,iNumWidth,iNumHeight,RGB(255,0,255));
			//NumDC.BitBlt(iNumWidth,0,iNumWidth,iNumHeight,&SrcNumDC,num2*iNumWidth,0,SRCCOPY);
			dc.TransparentBlt(rect1.left+xoffset+iNumWidth,rect1.top+yoffset,iNumWidth,iNumHeight,&NumDC,num2*iNumWidth,0,iNumWidth,iNumHeight,RGB(255,0,255));
		}else
		{
			//NumDC.BitBlt(0,0,iNumWidth,iNumHeight,&SrcNumDC,m_bCardCount*iNumWidth,0,SRCCOPY);
			dc.TransparentBlt(rect1.left+xoffset,rect1.top+yoffset,iNumWidth,iNumHeight,&NumDC,num2*iNumWidth,0,iNumWidth,iNumHeight,RGB(255,0,255));
		}
	}
	//ˢ�½���
	//dc.BitBlt(0,0,CardSize.cx,CardSize.cy,&NumDC,0,0,SRCCOPY);
	//dc.TransparentBlt(rect1.left+xoffset,rect1.top+yoffset,iBmpWidth,iBmpHeight,&NumDC,0,0,iBmpWidth,iBmpHeight,RGB(255,0,255));

//	DeleteObject(NumBmp);
//	NumDC.DeleteDC();
//	SrcNumDC.DeleteDC();

	//������Դ
	BufferDC.SelectObject(pOldBmp);
	BufferDC.DeleteDC();
	BufferBmp.DeleteObject();
	
//	if(!m_bUseMouse)
//		return ;
	
//	CRgn rgn ;
//	SetRectRgn(rgn,0,0,CardSize.cx,CardSize.cy);
//	SetWindowRgn(rgn,TRUE);
	return;
}

//����
int CUpGradeGameCard::GetPoint()
{
	if(m_iSourcePoint && m_iBackPoint && m_iBackMultiple)
		return m_iSourcePoint +m_iBackPoint * m_iBackMultiple;
	int score = 0;
	for (BYTE i=0;i<m_bCardCount;i++)
	{
		BYTE bCard=m_CardArray.GetAt(i);
		if(GetCardValue(bCard) == 5 )
		score +=5;
		if(GetCardValue(bCard) == 10||GetCardValue( bCard ) == 13 )
			score +=10;
	}
	return score;
}
//�����Ϣ
void CUpGradeGameCard::OnLButtonDown(UINT nFlags, CPoint Point)
{
	if ((m_bHorzLook==true)&&(m_bUseMouse==true))
	{
		m_bHotHitCard=HitCardTest(Point);
		if(m_bHotHitCard == 255)
			return ;
		BYTE bUpCard=m_CardUpArray.GetAt(m_bHotHitCard);

		//m_CardUpArray.SetAt(m_bHotHitCard,(bUpCard==TRUE)?FALSE:TRUE);

		//FixCardControl();

		//�ػ�ʧЧ����
	//	CRect Rect;
	//	Rect.SetRect(m_bHotHitCard*m_iCardHSpace,0,m_bHotHitCard*m_iCardHSpace+m_iCardWidth,m_iCardHight+m_iCardUpSpace);
	//	InvalidateRect(Rect,FALSE);
		m_iFirstCard = m_bHotHitCard;						//��һ����
	}
	return;
}
//paramenter iSourcePoint Դ����
//paramenter iBackPoint		�׷���
//paramenter iBackMultiple	�̵ױ���
//�������÷�����
BOOL CUpGradeGameCard::SetBackPointAndMultiple(int iSourcePoint,int iBackPoint,int iBackMultiple)
{
	m_iSourcePoint = iSourcePoint;
	m_iBackPoint = iBackPoint;
	m_iBackMultiple = iBackMultiple;
	return true;
}
//�����Ϣ
void CUpGradeGameCard::OnLButtonUp(UINT nFlags, CPoint Point)
{
	m_iFirstCard = 255;
	m_bMoveFirstCard = 255;
	if ((m_bUseMouse==true)&&(m_bHotHitCard!=255))
	{
		BYTE bHitCard=HitCardTest(Point);
		if ((bHitCard==255)||(m_bHotHitCard!=bHitCard))
		{
			m_bHotHitCard=255;
			return;
		}
		BYTE bUpCard=m_CardUpArray.GetAt(bHitCard);
		m_CardUpArray.SetAt(bHitCard,(bUpCard==TRUE)?FALSE:TRUE);
		FixCardControl();

		//�ػ�ʧЧ����
		//CRect Rect;
		//Rect.SetRect(bHitCard*m_iCardHSpace,0,bHitCard*m_iCardHSpace+m_iCardWidth,m_iCardHight+m_iCardUpSpace);
		//InvalidateRect(Rect,FALSE);
		if (m_pHandleWnd!=NULL) 
			m_pHandleWnd->SendMessage(IDM_LEFT_HITCARD,m_CardArray.GetAt(bHitCard)/*��ֵ*/,bUpCard/*�Ƿ�Ϊ��������*/);
	}
	return;
}

//�����Ϣ
void CUpGradeGameCard::OnRButtonUp(UINT nFlags, CPoint point)
{
	//	if(!((CClientGameDlg*)m_pHandleWnd)->m_PlayView.m_bRightPower)
//	{
		//�һ�����
		if ((m_bUseMouse==true)&&(m_pHandleWnd!=NULL)) 
			m_pHandleWnd->SendMessage(IDM_RIGHT_HITCARD,0,0);
//	}
/*	else
	{
	if ((m_bUseMouse==true)&&(m_pHandleWnd!=NULL))
	{
	BYTE bHitCard=HitCardTest(point);
	if ((bHitCard==255))
	{
	m_bHotHitCard=255;
	return;
	}
	BYTE bUpCard=m_CardUpArray.GetAt(bHitCard);
	//�����˵�����˿�
	if(bHitCard)
	{
	BYTE HuaSe=GetCardNum(m_CardArray.GetAt(bHitCard));
	for(int i=0;i<m_bCardCount;i++)
	{
	if ((m_bShowCard==true)&&(m_CardArray.GetAt(i)!=0))
	{
	BYTE ttt=GetCardNum(m_CardArray.GetAt(i));
	if(HuaSe==ttt)
	{
	m_CardUpArray.SetAt(i,(bUpCard==TRUE)?FALSE:TRUE);
	}
	}
	}
	FixCardControl();
	CRect Rect;
	Rect.SetRect(0,0,33*m_iCardHSpace+m_iCardWidth,m_iCardHight+m_iCardUpSpace);
	InvalidateRect(Rect,FALSE);
	m_pHandleWnd->SendMessage(IDM_RIGHT_HITCARD,0,0);
	}
	}
	}*/
	return;
}

//�����Ϣ
BOOL CUpGradeGameCard::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	if (m_bUseMouse==true)
	{
		//SetCursor(CGameImageLink::m_hHandCursor);
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		return TRUE;
	}
	return __super::OnSetCursor(pWnd,nHitTest,message);
}

/*
BOOL CUpGradeGameCard::PreTranslateMessage(MSG* pMsg)
{
//	m_ToolTip.RelayEvent(pMsg);
	return CWnd::PreTranslateMessage(pMsg);
}*/
//�����Ϣ(������϶�,���Խ������������ƶ����ұ�)
void CUpGradeGameCard::OnMouseMove(UINT nFlags, CPoint Point)
{
	m_bHotHitCard=HitCardTest(Point);

	if ((m_bUseMouse==true)&&(m_bHotHitCard!=255))
	{
		//m_ptMovePoint = Point;
		//������������϶�ѡ����
		if(MK_LBUTTON == nFlags && m_iFirstCard != m_bHotHitCard && !m_bCanMove)//�����ظ�����
		{
			BYTE bUpCard=m_CardUpArray.GetAt(m_bHotHitCard);
			m_CardUpArray.SetAt(m_bHotHitCard,(bUpCard==TRUE)?FALSE:TRUE);
			FixCardControl();

			//�ػ�ʧЧ����
			CRect Rect;
			Rect.SetRect(m_bHotHitCard*m_iCardHSpace,0,m_bHotHitCard*m_iCardHSpace+m_iCardWidth,m_iCardHight+m_iCardUpSpace);
			InvalidateRect(Rect,FALSE);

			m_iFirstCard = m_bHotHitCard;

			if (m_pHandleWnd!=NULL)
				m_pHandleWnd->SendMessage(IDM_LEFT_HITCARD,m_CardArray.GetAt(m_bHotHitCard),bUpCard);
		}
		//������������϶�����������,������ƶ�λ��
//		if(MK_LBUTTON == nFlags && m_bCanMove &&m_bMoveFirstCard != 255 )
//		{
			//��λ��(����)
			//ChangeIndexArray(m_bHotHitCard,m_bMoveFirstCard);
//			ChangeIndex(m_bHotHitCard,m_bMoveFirstCard);
			
//			m_bMoveFirstCard = m_bHotHitCard;
			//	InvalidateRect(NULL,FALSE);
//		}
			//	if (m_pHandleWnd!=NULL)
			//		m_pHandleWnd->SendMessage(IDM_LEFT_HITCARD,m_CardArray.GetAt(m_bHotHitCard),1);
	}
		//SetCapture();
		/*if(MK_LBUTTON == nFlags )
		{
			BYTE DestMoveCard = HitCardTestAbove(Point);
			//��λ��(����)
			if(DestMoveCard != 255)
			{
				if(m_bMoveFirstCard != 255)
				{
					//ChangeIndex(DestMoveCard,m_bMoveFirstCard);
					ChangeIndexArray(DestMoveCard,m_bMoveFirstCard);
				}
				if(m_CardUpArray.GetAt(DestMoveCard) == TRUE)
					m_bMoveFirstCard = DestMoveCard;
			}
			//	InvalidateRect(NULL,FALSE);
		}*/

	CWnd::OnMouseMove(nFlags, Point);
}

//���Ʒ�ת
BOOL CUpGradeGameCard::Convert(BYTE CardList[],int iCardCount)
{
	for(int i = 0; i< iCardCount/2 ;i++)
	{
		BYTE Card = CardList[i];
		CardList[i] = CardList[iCardCount-1];
		CardList[iCardCount-1] = Card;
	}
	return TRUE;
}

//�ƶ���λ��,��Դλ���ƶ���Ŀ��λ��
BOOL CUpGradeGameCard::ChangeIndexArray(BYTE DestIndex ,BYTE SrcIndex)
{
	BYTE MoveBlock[100]={0};
	int k = 0;
	int index = -1;						 //��һ�ŵ�λ��
	//������������
	for(int i = SrcIndex - 1;i > 0; i --)//Դ��߼��������Ƽ�����ǰ��
	{	//������������ȫ������
		if(m_CardUpArray.GetAt(i) == TRUE)
		{
			if(index == -1)
				index = i;
			MoveBlock[k++] = m_CardArray.GetAt(i);
		}
		else 
			break;
	}
	//ǰ����
	if(index == -1)
		index = SrcIndex;
	else
	{
		//��ת
		Convert(MoveBlock,k);
	}

	MoveBlock[k++] = m_CardArray.GetAt(SrcIndex);

	for(int i = SrcIndex+1;i < m_bCardCount; i ++)//Դ�ұ߼��������Ƽ����ں���
	{	//������������ȫ������
		if(m_CardUpArray.GetAt(i) == TRUE)
		{
			MoveBlock[k++] = m_CardArray.GetAt(i);
		}
		else 
			break;
	}

	//Դ��Ŀ��֮���ֵ���������ƶ�
	if(DestIndex > SrcIndex)//����
	{
		for(int i = index ; i < index + k; i ++)					//�ճ�λ��
		{
			if(i + k  >= m_bCardCount) 
				break;
			m_CardArray.SetAt(i,m_CardArray.GetAt(i + k ));
			m_CardUpArray.SetAt(i,m_CardUpArray.GetAt(i + k ));
		}
		for(int i = DestIndex - k + 1; i <= DestIndex; i ++)				//�����������Ƽ���
		{
			if(i - (DestIndex - k + 1) < 0 ) 
				break;
			m_CardArray.SetAt(i,MoveBlock[i - (DestIndex - k + 1)]);
			m_CardUpArray.SetAt(i,1);
		}
	}
	else if (DestIndex < SrcIndex)//����
	{
		for(int i = index + k - 1 ; i > index - 1 ;i--)				//�ճ�λ��
		{
			if( i - k < 0)
				break;
			m_CardArray.SetAt(i,m_CardArray.GetAt(i - k));
			m_CardUpArray.SetAt(i,m_CardUpArray.GetAt(i - k));
		}
		for(int i = DestIndex; i < DestIndex + k ; i ++)			//�����������Ƽ���
		{
			if(i - DestIndex < 0)
				break;
			m_CardArray.SetAt(i,MoveBlock[i - DestIndex]);
			m_CardUpArray.SetAt(i,1);
		}
	}
	//�ػ�
	FixCardControl();
	return TRUE;
}

//�ƶ���λ��,��Դλ���ƶ���Ŀ��λ��
BOOL CUpGradeGameCard::ChangeIndex(BYTE DestIndex ,BYTE SrcIndex)
{
	BYTE temp = m_CardArray.GetAt(SrcIndex);
	BYTE tempup = m_CardUpArray.GetAt(SrcIndex);

	//Դ��Ŀ��֮���ֵ���������ƶ�
	if(DestIndex > SrcIndex)								//����
	{
		for(int i = SrcIndex ; i < DestIndex ;i++)
		{
			if(i + 1 >= m_bCardCount )
				break;
			m_CardArray.SetAt(i,m_CardArray.GetAt(i + 1));
			m_CardUpArray.SetAt(i,m_CardUpArray.GetAt(i + 1));
		}
	}else if (DestIndex < SrcIndex)							//����
	{
		for(int i = SrcIndex ; i > DestIndex ;i--)
		{
			if(i - 1 < 0)
				break;
			m_CardArray.SetAt(i,m_CardArray.GetAt(i - 1));
			m_CardUpArray.SetAt(i,m_CardUpArray.GetAt(i - 1));
		}
	}
	m_CardArray.SetAt(DestIndex,temp);
	m_CardUpArray.SetAt(DestIndex,tempup);

	//�ػ�
	FixCardControl();
/*	CRect Rect;
	if(m_bHotHitCard > m_bMoveFirstCard)//����
		Rect.SetRect(m_bMoveFirstCard*m_iCardHSpace,0,m_bHotHitCard*m_iCardHSpace+m_iCardWidth,m_iCardHight+m_iCardUpSpace);
	else//����
		Rect.SetRect(m_bHotHitCard*m_iCardHSpace,0,m_bMoveFirstCard*m_iCardHSpace+m_iCardWidth,m_iCardHight+m_iCardUpSpace);
	InvalidateRect(Rect,FALSE);
*/
	return TRUE;
}
/*
//�����Ϣ(����������϶�ѡ��)
void CUpGradeGameCard::OnMouseMove(UINT nFlags, CPoint Point)
{
	m_bHotHitCard=HitCardTest(Point);
	
	if ((m_bUseMouse==true)&&(m_bHotHitCard!=255))
	{
		if(MK_LBUTTON == nFlags && m_iFirstCard != m_bHotHitCard)//�����ظ�����
		{
			BYTE bUpCard=m_CardUpArray.GetAt(m_bHotHitCard);
			m_CardUpArray.SetAt(m_bHotHitCard,(bUpCard==TRUE)?FALSE:TRUE);
			FixCardControl();

			//�ػ�ʧЧ����
			CRect Rect;
			Rect.SetRect(m_bHotHitCard*m_iCardHSpace,0,m_bHotHitCard*m_iCardHSpace+m_iCardWidth,m_iCardHight+m_iCardUpSpace);
			InvalidateRect(Rect,FALSE);

			m_iFirstCard = m_bHotHitCard;

			if (m_pHandleWnd!=NULL)
				m_pHandleWnd->SendMessage(IDM_LEFT_HITCARD,m_CardArray.GetAt(m_bHotHitCard),bUpCard);
		}
	}

	CWnd::OnMouseMove(nFlags, Point);
}*/
//��������
BOOL CUpGradeGameCard::SetFriendArray(BYTE bCardList[], BYTE bUpList[], BYTE bCardCount)
{
	/*if (bCardCount>m_CardFriendArray.GetSize())*/ m_CardFriendArray.SetSize(bCardCount);
	//if (bCardCount>m_CardUpArray.GetSize()) m_CardUpArray.SetSize(bCardCount);
	if (bCardList!=NULL) CopyMemory(m_CardFriendArray.GetData(),bCardList,sizeof(BYTE)*bCardCount);
	//if (bUpList!=NULL) CopyMemory(m_CardUpArray.GetData(),bUpList,sizeof(BYTE)*bCardCount);
	//else memset(m_CardUpArray.GetData(),0,sizeof(BYTE)*bCardCount);
	m_bFriendCardCount=bCardCount;
	return TRUE;
	//m_CardFriendArray
}
//�����������в���ĳ���Ƶ���Ŀ
BYTE CUpGradeGameCard::FindACardNumInFriendList(BYTE iCardNum)
{
	int FriendCount=0;
	if(m_bFriendCardCount>0&&m_bCardCount>0)
	{
		for(int i=0;i<m_bFriendCardCount;i++)
		{
			if(GetCardNum(m_CardFriendArray.GetAt(i))==iCardNum)
				FriendCount++;
		}	
	}
	return FriendCount;
}
//���õ���ͼƬ
void CUpGradeGameCard::SetCardImage()
{
	TCHAR path[MAX_PATH];
	switch(GetCardStyle())
	{
	case 2:
	case IMAGE_SMALL:
			wsprintf(path,TEXT(".\\image\\%s\\game_CARD_SMALL.bmp"),GET_SKIN_FOLDER(szTempStr));
			m_CardPic[2].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			if(m_CardPic[2].GetWidth() <= 0)
			{
				wsprintf(path,TEXT(".\\image\\%s\\game_CARD_SMALL.bmp"),SKIN_CARD);
				m_CardPic[2].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			}
			
			wsprintf(path,TEXT(".\\image\\%s\\game_CARD_SMALL_back.bmp"),GET_SKIN_FOLDER(szTempStr));
			m_CardPic[3].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			if(m_CardPic[3].GetWidth() <= 0)
			{
				wsprintf(path,TEXT(".\\image\\%s\\game_CARD_SMALL_back.bmp"),SKIN_CARD);
				m_CardPic[3].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			}


			wsprintf(path,TEXT(".\\image\\%s\\game_card_small_laizi.bmp"),SKIN_CARD);//�����
			m_CardPic[4].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		break;
	case IMAGE_VERY_SMALL:
			wsprintf(path,TEXT(".\\image\\%s\\very_small.bmp"),GET_SKIN_FOLDER(szTempStr));
			m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			if(m_CardPic[0].GetWidth() <= 0)
			{
				wsprintf(path,TEXT(".\\image\\%s\\very_small.bmp"),SKIN_CARD);
				m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			}
		break;
	default:
		if(GetSystemMetrics(SM_CXSCREEN)>=1024)
		{
			wsprintf(path,TEXT(".\\image\\%s\\game_CARD.bmp"),GET_SKIN_FOLDER(szTempStr));
			m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			if(m_CardPic[0].GetWidth() <= 0)
			{
			wsprintf(path,TEXT(".\\image\\%s\\game_CARD.bmp"),SKIN_CARD);
			m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			}
			wsprintf(path,TEXT(".\\image\\%s\\game_CARD_back.bmp"),GET_SKIN_FOLDER(szTempStr));
			m_CardPic[1].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			if(m_CardPic[1].GetWidth() <= 0 )
			{
			wsprintf(path,TEXT(".\\image\\%s\\game_CARD_back.bmp"),SKIN_CARD);
			m_CardPic[1].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			}


			wsprintf(path,TEXT(".\\image\\%s\\game_card_laizi.bmp"),SKIN_CARD);//�����
			m_CardPic[4].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		}
		else
		{
			wsprintf(path,TEXT(".\\image\\%s\\game_CARD_800.bmp"),GET_SKIN_FOLDER(szTempStr));
			m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			if(m_CardPic[0].GetWidth() <= 0 )
			{
			wsprintf(path,TEXT(".\\image\\%s\\game_CARD_800.bmp"),SKIN_CARD);
			m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			}
			
			wsprintf(path,TEXT(".\\image\\%s\\game_CARD_800_back.bmp"),GET_SKIN_FOLDER(szTempStr));
			m_CardPic[1].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			if(m_CardPic[1].GetWidth() <= 0 )
			{
			wsprintf(path,TEXT(".\\image\\%s\\game_CARD_800_back.bmp"),SKIN_CARD);
			m_CardPic[1].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			}


			wsprintf(path,TEXT(".\\image\\%s\\game_card_800_laizi.bmp"),SKIN_CARD);//�����
			m_CardPic[4].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		}
		break;
	}
	wsprintf(path,TEXT(".\\image\\%s\\cardnum.bmp"),GET_SKIN_FOLDER(szTempStr));
	m_Num.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	if(m_Num.GetWidth() <= 0)
	{
	  wsprintf(path,TEXT(".\\image\\%s\\cardnum.bmp"),GET_SKIN_FOLDER(szTempStr));
	   m_Num.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	}



	return ;
}
//�����˿˸ߺͿ�
void  CUpGradeGameCard::SetCardWidthHeight(int iWidth,int iHeight)
{
	switch(GetCardStyle())
	{
	case IMAGE_SMALL:
		m_iCardHight=57;
		m_iCardWidth=42;
		break;
	case IMAGE_VERY_SMALL:
		m_iCardHight=12;
		m_iCardWidth=25;
		break;
	default:
		if(GetSystemMetrics(SM_CXSCREEN)>=1024)
		{	
			m_iCardHight=95;
			m_iCardWidth=70;	
		}
		else
		{
			m_iCardHight=73;
			m_iCardWidth=54;
		}
	}
	return ;
}