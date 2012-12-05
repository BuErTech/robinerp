#include "Stdafx.h"
#include "Resource.h"
#include "GameCard.h"
#include "ClientGameDlg.h"
//��̬����
CGameImage CUpGradeGameCard::m_CardPic[7]; //������

//{{ qy �ƿؼ��޸�20091109
#define  ALL_CARDS_NORMAL  0   // ����
#define  ALL_CARDS_DOWN    1   // ���е���
#define  ALL_CARDS_UP      2   // ���е��Ʊ�����
//}} qy �ƿؼ��޸�20091109

BEGIN_MESSAGE_MAP(CUpGradeGameCard, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//���캯��
CUpGradeGameCard::CUpGradeGameCard(BYTE bCardStyle)
{
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
	m_bCardCount=0;
	SetCardStyle(bCardStyle);
	SetCardParameter(15,13,20);
	m_pHandleWnd=NULL;
	m_bHorzLook=true;
	m_bShowCard=false;
	m_bUseMouse=false;
	m_bHotHitCard=255;
	m_bSelect=false;
	m_PtBasePoint.SetPoint(0,0);
	m_bFriendCardCount=0; //��������
	m_bShowNum = 0;
	m_XShowMode=XShowMode::SWX_MODE_CENTER;
	m_YShowMode=YShowMode::SWY_MODE_CENTER;
	m_iSourcePoint = 0 ;
	m_iBackPoint = 0;
	m_iBackMultiple = 1;
	// m_bLButtonDown = FALSE;
	m_ptLeftHit = CPoint(0,0);
	m_bCanMove = FALSE;
	memset(m_ReplaceCardList,0,sizeof(m_ReplaceCardList));
	m_iReplaceCount = 0;
	//AfxSetResourceHandle(GetModuleHandle(NULL));

//{{ qy �ƿؼ��޸�20091109
	m_stPoint.SetPoint(0,0);
	m_SelecColor= RGB(170,170,170);
	m_bSelectOK = FALSE;
	m_aSelIndex.RemoveAll();
//}} qy �ƿؼ��޸�20091109

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
	//if(bUserMouse)
	//{
	//	OutputDebugString("�ƿؼ� SetUseMouse = true");
	//}
	//else
	//{
	//	OutputDebugString("�ƿؼ� SetUseMouse = false");
	//}
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
bool CUpGradeGameCard::SetCard(BYTE bCardList[], BYTE bUpList[], BYTE bCardCount, BYTE ReplaceCardList[], BYTE ReplaceCount)
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
	if (bUpList!=NULL) 
	{
		CopyMemory(m_CardUpArray.GetData(),bUpList,sizeof(BYTE)*bCardCount);
	}
	else
	{
		memset(m_CardUpArray.GetData(),0,sizeof(BYTE)*bCardCount);
	}
	if (ReplaceCardList!=NULL) 
	{
		CopyMemory(m_ReplaceCardList,ReplaceCardList,sizeof(BYTE)*ReplaceCount);//������
		m_iReplaceCount = ReplaceCount;
	}
	else
	{
		memset(m_ReplaceCardList,0,sizeof(m_ReplaceCardList));
		m_iReplaceCount = 0;
	}
	if(m_MoveSelectCardArray.GetSize()!=bCardCount)
	{
		m_MoveSelectCardArray.SetSize(bCardCount);
		memset(m_MoveSelectCardArray.GetData(),0,sizeof(BYTE)*bCardCount);
	}
	m_bCardCount=bCardCount;
	if(((CClientGameDlg*)m_pHandleWnd)->GetGameStatus()==0)
		FixCardControl(false);
	else
		FixCardControl();

	return true;
}

///ĳ�����Ƿ������ƶ�����
///@param BYTE bCardList[] �ƶ���
///@param int  iCount      ��������
///@param BYTE bCard       Ҫ���������
///@param BYTE TempCard[]      �Ѵ�����
///@param int TempCount       �Ѵ���������
///@return �Ƿ�������
BOOL CUpGradeGameCard::IsInside(BYTE bCardList[], int iCount, BYTE bCard, BYTE TempCard[], int TempCount)
{
	if (TempCount >= iCount)
	{
		return FALSE;
	}
	/*for (int j=0; j<TempCount; j++)
	{
		if (TempCard[j] == bCard)
		{
			return FALSE;
		}
	}*/
	for (int i=0; i<iCount; i++)
	{
		if (bCardList[i] == bCard)
		{

			return TRUE;
		}
	}
   return FALSE;
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
//{{ qy �ƿؼ��޸�20091109  �޸�ѡ�еķ�Χ�Ƿ���Ч 
	// ��ȡ������
	CRect rect;
	GetClientRect(&rect);
	if (!rect.PtInRect(Point))
	{
		return 255;
	}

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
				{
					break;
				}
				if(bCardIndex-i < 0)
				{
					break;
				}

				// ����x�����Ƿ񳬹����һ����
				int iX = (bCardIndex-i) * m_iCardHSpace + m_iCardWidth;
				if (Point.x > iX)
				{
					continue ;
				}

				bCardUp=m_CardUpArray.GetAt(bCardIndex-i);
				if ((bCardUp==0)&&(Point.y>m_iCardWidth))
				{
					return bCardIndex-i;
				}
				if ((bCardUp>0)&&(Point.y<m_iCardUpSpace))
				{
					return bCardIndex-i;
				}
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
		/* bool bHitThisCard=true;
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
	{
		return;
	}

	//��ȡλ��
	CSize Size;
	GetCardSize(Size);

	//����λ��
	CRect CardRect;
	switch (m_XShowMode)
	{
	case SWX_MODE_LEFT: { CardRect.left=m_PtBasePoint.x; break; }
	case SWX_MODE_CENTER: { CardRect.left=m_PtBasePoint.x-Size.cx/2; break; }
	case SWX_MODE_RIGHT: { CardRect.left=m_PtBasePoint.x-Size.cx; break; }
	}

	switch (m_YShowMode)
	{
	case SWY_MODE_TOP: { CardRect.top=m_PtBasePoint.y; break; }
	case SWY_MODE_CENTER: { CardRect.top=m_PtBasePoint.y-Size.cy/2; break; }
	case SWY_MODE_BOTTOM: { CardRect.top=m_PtBasePoint.y-Size.cy; break; }
	}

	//�ƶ�λ��
	CardRect.right=CardRect.left+Size.cx;
	CardRect.bottom=CardRect.top+Size.cy;
	MoveWindow(&CardRect);
	Invalidate(FALSE);

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
//{{ qy �ƿؼ��޸�20091109 ȥ����ǰ�ĸ�������ֱ��ɾ������
	//��������
	SetWindowRgn(AllRgn,TRUE);
	AllRgn.DeleteObject();
//{{ qy �ƿؼ��޸�20091109
	return;
}

//������Ϣ
int CUpGradeGameCard::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct)==-1) return -1;
	// m_ToolTip.Create(this);
	// m_ToolTip.SetDelayTime(10);
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
	//����λͼ
	CGameImageHelper CardHandle(&m_CardPic[0]);//������
	CGameImageHelper BackHandle(&m_CardPic[1]);//�����Ʊ�
	CGameImageHelper SmallHandle(&m_CardPic[2]); //С��
	CGameImageHelper SmallBackHandle(&m_CardPic[3]); //С�ƴ�Сè
	CGameImageHelper LaiZiCardsHandle(&m_CardPic[4]); //�����
	CGameImageHelper MoveSelectCardHandle(&m_CardPic[5]);
	CGameImageHelper MoveSelectBackHandle(&m_CardPic[6]);

	BYTE CardTemp[4] ={0};//��ʱ�Ѵ������б�
	BYTE iTempCount = 0;  //��ʱ�Ѵ���������
	//�滭�˿�
	int iXPos=0,iYPos=0;
	CGameImageHelper * pActiveHandle=NULL;
	for (BYTE i=0;i<m_bCardCount;i++)
	{
		BYTE bCard=m_CardArray.GetAt(i);
		BYTE bUpCard=m_CardUpArray.GetAt(i);
        BYTE bMoveSelect =m_MoveSelectCardArray.GetAt(i);
		if ( i == 0 && m_bCardCount>= 2 &&  m_MoveSelectCardArray.GetAt(1) != 1 )
		{
			bMoveSelect = 0;
		}
		if ((m_bShowCard==true)&&bCard != 0)//(m_CardArray.GetAt(i)!=0))
		{
			CString str;
			str.Format("zhtlogg:bCard=%02d",bCard);
			OutputDebugString(str);
			if ((bCard==0x4E)||(bCard==0x4F))
			{
				if(bMoveSelect==1)
				{
					iXPos=(bCard-0x4D)*m_iCardWidth;
					iYPos=0;
					pActiveHandle=&MoveSelectBackHandle;	

				}
				else if(IMAGE_SMALL == GetCardStyle() )
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
				if(bMoveSelect==1)
				{
					pActiveHandle=&MoveSelectCardHandle;
					iXPos=GetCardXPos(bCard)*m_iCardWidth;
					iYPos=GetCardYPos(bCard)*m_iCardHight;

				}
				else
				if(IMAGE_SMALL == GetCardStyle() )
				{
					pActiveHandle=&SmallHandle;
					iXPos=GetCardXPos(bCard)*m_iCardWidth;
					iYPos=GetCardYPos(bCard)*m_iCardHight;
				}
				else
				{
					pActiveHandle=&CardHandle;
					iXPos=GetCardXPos(bCard)*m_iCardWidth;
					iYPos=GetCardYPos(bCard)*m_iCardHight;
				}
				if (GetCardNum(bCard) == GetCardNum(((CClientPlayView*)GetParent())->m_bLaiCardList[0])
					|| (IsInside(m_ReplaceCardList,m_iReplaceCount,bCard,CardTemp,iTempCount)) && !m_bUseMouse )//���������
					//|| (0 == i && (((CClientPlayView*)GetParent())->m_bIsFakeBomb)) && !m_bUseMouse )//���������
				{
					CardTemp[iTempCount++] = bCard;
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
					|| (IsInside(m_ReplaceCardList,m_iReplaceCount,bCard,CardTemp,iTempCount)) && !m_bUseMouse )//���������
					//|| (0 == i && (((CClientPlayView*)GetParent())->m_bIsFakeBomb)) && !m_bUseMouse )//���������
				{
					CardTemp[iTempCount++] = bCard;
					pActiveHandle = &LaiZiCardsHandle;
				}
			}
			else
				pActiveHandle=&BackHandle;
		}
		if (m_bHorzLook==true)
		{
			pActiveHandle->BitBlt(BufferDC,i*m_iCardHSpace,bUpCard?0:m_iCardUpSpace,m_iCardWidth,m_iCardHight,iXPos,iYPos);
		}
		else
		{
			pActiveHandle->BitBlt(BufferDC,0,i*m_iCardVSpace,m_iCardWidth,m_iCardHight,iXPos,iYPos);
		}
	}

	//ˢ�½���
	dc.BitBlt(0,0,CardSize.cx,CardSize.cy,&BufferDC,0,0,SRCCOPY);
	//͸������
	CRect rect,rect1;
	//���������
	rect.left=0;
	rect.right=CardSize.cx;
	if(m_bHorzLook)
	{
		rect.top = CardSize.cy - m_iCardHight;
	}
	else
	{
		rect.top =0;
	}
	rect.bottom=CardSize.cy;
	switch(m_bShowNum)
	{
	case 1:
		if (m_bHorzLook==true)//С����
		{
			rect1.left = rect.right - m_iCardWidth;
			rect1.top = rect.top;
			rect1.bottom = rect.bottom;
			rect1.right = rect.right;
		}else
		{
			rect1.left = rect.left ;
			rect1.top = rect.bottom - m_iCardHight;
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
	//////lym1112����ʣ��������///////////////////////////
	CDC NumDC;//,SrcNumDC;
	NumDC.CreateCompatibleDC(&dc);
	CGameImageHelper Num(&m_Num);
	NumDC.SelectObject(Num);
	int xoffset =(rect1.Width()-( m_bCardCount >=10?2:1)*iNumWidth)/2;
	int yoffset = (rect1.Height()-iNumHeight)/2;
	int num1 =m_bCardCount/10 ,num2 = m_bCardCount%10;
	if (m_bShowNum)
	{
		if(m_bCardCount >= 10)
		{
			dc.TransparentBlt(rect1.left+xoffset,rect1.top+yoffset,iNumWidth,iNumHeight,&NumDC,num1*iNumWidth,0,iNumWidth,iNumHeight,RGB(255,0,255));
			dc.TransparentBlt(rect1.left+xoffset+iNumWidth,rect1.top+yoffset,iNumWidth,iNumHeight,&NumDC,num2*iNumWidth,0,iNumWidth,iNumHeight,RGB(255,0,255));
		}
		else
		{
			dc.TransparentBlt(rect1.left+xoffset,rect1.top+yoffset,iNumWidth,iNumHeight,&NumDC,num2*iNumWidth,0,iNumWidth,iNumHeight,RGB(255,0,255));
		}
	}
	//////lym1112����ʣ��������///////////////////////
	//ˢ�½���
	//dc.BitBlt(0,0,CardSize.cx,CardSize.cy,&NumDC,0,0,SRCCOPY);
	//dc.TransparentBlt(rect1.left+xoffset,rect1.top+yoffset,iBmpWidth,iBmpHeight,&NumDC,0,0,iBmpWidth,iBmpHeight,RGB(255,0,255));
	// DeleteObject(NumBmp);
	 NumDC.DeleteDC();
	// SrcNumDC.DeleteDC();
	//������Դ
	BufferDC.SelectObject(pOldBmp);
	BufferDC.DeleteDC();
	BufferBmp.DeleteObject();

//{{ qy �ƿؼ��޸�20091109 ��ѡ��ʱ���ƿ�ѡ
	// ������Ӱ
	/*if (m_bSelectOK)
	{
		DisplayInfo();
	}*/
//}} qy �ƿؼ��޸�20091109

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
//lym1111ʹ�����������
void CUpGradeGameCard::DownCard()
{
	OutputDebugString("�ƿؼ� : DownCard");
	for (int i=0; i<m_bCardCount; ++i)
	{
		m_CardUpArray.SetAt(i,0);					
	}
	FixCardControl();

}
//�����Ϣ
void CUpGradeGameCard::OnLButtonDown(UINT nFlags, CPoint Point)
{
//{{ qy �ƿؼ��޸�20091109   ��ǰ�Ķ����Բ��ã�����ֻ��¼ѡ�������
	if (!m_bSelectOK)
	{
		OutputDebugString("�ƿؼ� OnLButtonDown");
		m_bSelectOK = TRUE;
		m_stPoint = Point;
		if (m_bUseMouse)
		{
			if (255 == HitCardTest(Point))
			{
				m_bSelectOK = FALSE;
			}
		}
		SetCapture();
	}
	return CWnd::OnLButtonDown(nFlags, Point);
//}} qy �ƿؼ��޸�20091109
	//if (m_bUseMouse==true)
	//{
	//	if (m_bHorzLook==true)
	//	{
	//		m_bHotHitCard=HitCardTest(Point);
	//		if(m_bHotHitCard == 255)
	//			return ;
	//		//Ѱ�Ұ����˿�
	//		BYTE bCardUp=m_CardUpArray.GetAt(m_bHotHitCard);
	//		bool AllUp = true;
	//		for(int i=0; i<m_CardUpArray.GetCount(); i++)
	//		{
	//			if(!m_CardUpArray.GetAt(i))
	//			{
	//				AllUp = false;
	//				break;
	//			}
	//		}
	//		if(!AllUp && bCardUp && Point.y<m_iCardUpSpace+m_iCardHight)
	//		{
	//			m_bMoveSel = true;
	//			m_pMovePos = Point;

	//			m_bCanUp = bCardUp;
	//			if (!bCardUp)
	//			{
	//				m_bMoveFirstCard = m_bHotHitCard;
	//				m_CardUpArray.SetAt(m_bHotHitCard,1);
	//			}
	//			m_bMove = false;
	//			m_CurCardIndex = -1;
	//			m_StartIndex = m_bHotHitCard;
	//			for(int i=m_bHotHitCard; i>=0; i--)
	//			{
	//				if(m_CardUpArray.GetAt(i))
	//				{
	//					m_StartIndex = i;
	//					continue;
	//				}
	//			}
	//			m_EndIndex = m_bHotHitCard;
	//			for(int i=m_bHotHitCard+1; i<m_bCardCount; i++)
	//			{
	//				if(m_CardUpArray.GetAt(i))
	//				{
	//					m_EndIndex = i;
	//					continue;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			m_pStartPos = Point;
	//			m_bMultiSel = true;
	//		}
	//	}
	//}
	return;
}
//paramenter iSourcePoint Դ����
//paramenter iBackPoint �׷���
//paramenter iBackMultiple �̵ױ���
//�������÷�����
BOOL CUpGradeGameCard::SetBackPointAndMultiple(int iSourcePoint,int iBackPoint,int iBackMultiple)
{
	m_iSourcePoint = iSourcePoint;
	m_iBackPoint = iBackPoint;
	m_iBackMultiple = iBackMultiple;
	return true;
}
BYTE CUpGradeGameCard::GetCardIndex(CPoint &Point)
{
	BYTE index = HitCardTest(Point);
	if(index == 255)
	{
		index=(BYTE)(Point.x/m_iCardHSpace);
		if (index!=255 && index>=m_bCardCount)
			index=m_bCardCount-1;
	}
	return index;
}

//�����Ϣ
void CUpGradeGameCard::OnLButtonUp(UINT nFlags, CPoint Point)
{
//{{ qy �ƿؼ��޸�20091109
	::ReleaseCapture();

	if (m_bSelectOK && m_bUseMouse)
	{
		// ��ȡѡ�е���
		BOOL bFixCtrl = FALSE;
		for(int i=0; i<m_bCardCount; i++)
		{
			m_MoveSelectCardArray.SetAt(i,false);
		}
		if (m_RectDis.IsRectEmpty())
		{
			// �����һ������ѡ�е����ƣ���������λ��ȡ��
			BYTE byIndex = HitCardTest(m_stPoint);
			if (byIndex < m_bCardCount)
			{
				m_CardUpArray.GetAt(byIndex) > 0?m_CardUpArray.SetAt(byIndex, 0):m_CardUpArray.SetAt(byIndex, 1);
				bFixCtrl = TRUE;
			}
		}
		else
		{
			for (int i=0; i<m_aSelIndex.GetSize(); ++i)
			{
				bFixCtrl = TRUE;
				BYTE byIndex = m_aSelIndex.GetAt(i);
				m_CardUpArray.GetAt(byIndex) > 0?m_CardUpArray.SetAt(byIndex, 0):m_CardUpArray.SetAt(byIndex, 1);					
			}
		}
		OutputDebugString("�ƿؼ� OnLButtonUp");

		// �ָ��ƿؼ�
		m_RectDis.SetRectEmpty();
		m_bSelectOK = FALSE;
		GetParent()->Invalidate();   // ˢ����ͼʹ��ʱ��������������
		if (bFixCtrl)
		{
			FixCardControl();
			if (m_pHandleWnd!=NULL)
				m_pHandleWnd->SendMessage(IDM_LEFT_HITCARD,0,0);   // ����Ϸ��Ӧ���������Ϣ�����ݲ�ͬ��Ϸ���Ը���

		}
		else
		{
			Invalidate();
		}
	}
	return CWnd::OnLButtonUp(nFlags, Point);
}

//�����Ϣ
void CUpGradeGameCard::OnRButtonUp(UINT nFlags, CPoint point)
{
//{{ qy �ƿؼ��޸�20091109
	::ReleaseCapture();
	// �ָ��ƿؼ�
	m_RectDis.SetRectEmpty();
	m_bSelectOK = FALSE;
	GetParent()->Invalidate();   // ˢ����ͼʹ��ʱ��������������
	FixCardControl();

	if ((m_bUseMouse==true)&&(m_pHandleWnd!=NULL))
		m_pHandleWnd->SendMessage(IDM_RIGHT_HITCARD,0,0);  // ����Ϸ��Ӧ�Ҽ�������Ϣ�����磺����,���ݲ�ͬ��Ϸ���Ը���
	return CWnd::OnRButtonUp(nFlags, point);
//}} qy �ƿؼ��޸�20091109 ��ǰ����Ϣ���ݿ��Բ�����
}

//�����Ϣ
BOOL CUpGradeGameCard::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	//qy �ƿؼ��޸�20091109 ȥ��������ã�����ʹ�������Ĺ��
	return __super::OnSetCursor(pWnd,nHitTest,message);
}

/*
BOOL CUpGradeGameCard::PreTranslateMessage(MSG* pMsg)
{
// m_ToolTip.RelayEvent(pMsg);
return CWnd::PreTranslateMessage(pMsg);
}*/
//�����Ϣ(������϶�,���Խ������������ƶ����ұ�)
void CUpGradeGameCard::OnMouseMove(UINT nFlags, CPoint Point)
{
//{{ qy �ƿؼ��޸�20091109 ��¼��굱ǰλ�ã���ѡʱ��ȡ��Χ���л���
	if (m_bUseMouse && m_bSelectOK)
	{
		CRect rect;
		rect.left = m_stPoint.x;
		rect.top = m_stPoint.y;
		rect.right = Point.x;
		rect.bottom = Point.y;
		int sindex = GetCardIndex(m_RectDis.BottomRight());
		int eindex = GetCardIndex(m_RectDis.TopLeft());

		if(sindex > eindex)
		{
			int temp = sindex;
			sindex = eindex;
			eindex = temp;
		}	
		for(int i=0; i<m_bCardCount; i++)
		{
			m_MoveSelectCardArray.SetAt(i,false);
		}

		for(int i=sindex; i<=eindex; i++)
		{
			m_MoveSelectCardArray.SetAt(i,true);
		}
	//	OutputDebugMsg("qlog: �������꣨%d��%d��  end (%d   %d)",m_stPoint.x,m_stPoint.y, Point.x, Point.y);
		GetCardsIndex(rect);
		m_RectDis = rect;
		GetParent()->Invalidate();  // ˢ����ͼʹ��ʱ����������
		Invalidate();
		//DisplayInfo();		
	}
//}} qy �ƿؼ��޸�20091109  ��ǰ���ݿ���ɾ��

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
	int index = -1;//��һ�ŵ�λ��
	//������������
	for(int i = SrcIndex - 1;i > 0; i --)//Դ��߼��������Ƽ�����ǰ��
	{ //������������ȫ������
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
	{ //������������ȫ������
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
		for(int i = index ; i < index + k; i ++)//�ճ�λ��
		{
			if(i + k >= m_bCardCount)
				break;
			m_CardArray.SetAt(i,m_CardArray.GetAt(i + k ));
			m_CardUpArray.SetAt(i,m_CardUpArray.GetAt(i + k ));
		}
		for(int i = DestIndex - k + 1; i <= DestIndex; i ++)//�����������Ƽ���
		{
			if(i - (DestIndex - k + 1) < 0 )
				break;
			m_CardArray.SetAt(i,MoveBlock[i - (DestIndex - k + 1)]);
			m_CardUpArray.SetAt(i,1);
		}
	}
	else if (DestIndex < SrcIndex)//����
	{
		for(int i = index + k - 1 ; i > index - 1 ;i--)//�ճ�λ��
		{
			if( i - k < 0)
				break;
			m_CardArray.SetAt(i,m_CardArray.GetAt(i - k));
			m_CardUpArray.SetAt(i,m_CardUpArray.GetAt(i - k));
		}
		for(int i = DestIndex; i < DestIndex + k ; i ++)//�����������Ƽ���
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
	if(DestIndex > SrcIndex)//����
	{
		for(int i = SrcIndex ; i < DestIndex ;i++)
		{
			if(i + 1 >= m_bCardCount )
				break;
			m_CardArray.SetAt(i,m_CardArray.GetAt(i + 1));
			m_CardUpArray.SetAt(i,m_CardUpArray.GetAt(i + 1));
		}
	}else if (DestIndex < SrcIndex)//����
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
	/* CRect Rect;
	if(m_bHotHitCard > m_bMoveFirstCard)//����
	Rect.SetRect(m_bMoveFirstCard*m_iCardHSpace,0,m_bHotHitCard*m_iCardHSpace+m_iCardWidth,m_iCardHight+m_iCardUpSpace);
	else//����
	Rect.SetRect(m_bHotHitCard*m_iCardHSpace,0,m_bMoveFirstCard*m_iCardHSpace+m_iCardWidth,m_iCardHight+m_iCardUpSpace);
	InvalidateRect(Rect,FALSE);
	*/
	return TRUE;
}
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
		wsprintf(path,TEXT(".\\image\\CardSkin\\game_CARD_SMALL.bmp"));
		m_CardPic[2].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		if(m_CardPic[2].GetWidth() <= 0)
		{
			wsprintf(path,TEXT(".\\image\\CardSkin\\game_CARD_SMALL.bmp"));
			m_CardPic[2].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		}
		wsprintf(path,TEXT(".\\image\\CardSkin\\game_CARD_SMALL_back.bmp"));
		m_CardPic[3].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		if(m_CardPic[3].GetWidth() <= 0)
		{
			wsprintf(path,TEXT(".\\image\\CardSkin\\game_CARD_SMALL_back.bmp"));
			m_CardPic[3].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		}
		wsprintf(path,TEXT(".\\image\\CardSkin\\game_card_small_laizi.bmp"));//�����
		m_CardPic[4].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		break;
	case IMAGE_VERY_SMALL:
		wsprintf(path,TEXT(".\\image\\CardSkin\\very_small.bmp"));
		m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		if(m_CardPic[0].GetWidth() <= 0)
		{
			wsprintf(path,TEXT(".\\image\\CardSkin\\very_small.bmp"));
			m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		}
		break;
	default:
		if(GetSystemMetrics(SM_CXSCREEN)>=1024)
		{
			wsprintf(path,TEXT(".\\image\\CardSkin\\game_CARD.bmp"));//������
			m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			if(m_CardPic[0].GetWidth() <= 0)
			{
				wsprintf(path,TEXT(".\\image\\CardSkin\\game_CARD.bmp"));
				m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			}
			wsprintf(path,TEXT(".\\image\\CardSkin\\game_CARD_back.bmp"));//�Ʊ�����
			m_CardPic[1].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			if(m_CardPic[1].GetWidth() <= 0 )
			{
				wsprintf(path,TEXT(".\\image\\CardSkin\\game_CARD_back.bmp"));
				m_CardPic[1].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			}
			wsprintf(path,TEXT(".\\image\\CardSkin\\game_card_laizi.bmp"));//�����
			m_CardPic[4].SetLoadInfo(path,CGameImageLink::m_bAutoLock);

			m_CardPic[5].SetLoadInfo(".\\image\\CardSkin\\game_Card_SELECT.bmp",CGameImageLink::m_bAutoLock);

			//wsprintf(path,TEXT("%s\\game_Card_BACK_SELECT.bmp"), SKIN_CARD);
			m_CardPic[6].SetLoadInfo(".\\image\\CardSkin\\game_Card_BACK_SELECT.bmp",CGameImageLink::m_bAutoLock);
		}
		else
		{
			wsprintf(path,TEXT(".\\image\\CardSkin\\game_CARD_800.bmp"));//������
			m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			if(m_CardPic[0].GetWidth() <= 0 )
			{
				wsprintf(path,TEXT(".\\image\\CardSkin\\game_CARD_800.bmp"));
				m_CardPic[0].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			}

			wsprintf(path,TEXT(".\\image\\CardSkin\\game_CARD_800_back.bmp"));//�Ʊ�����
			m_CardPic[1].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			if(m_CardPic[1].GetWidth() <= 0 )
			{
				wsprintf(path,TEXT(".\\image\\CardSkin\\game_CARD_800_back.bmp"));
				m_CardPic[1].SetLoadInfo(path,CGameImageLink::m_bAutoLock);
			}


			wsprintf(path,TEXT(".\\image\\CardSkin\\game_card_800_laizi.bmp"),SKIN_CARD);//�����
			m_CardPic[4].SetLoadInfo(path,CGameImageLink::m_bAutoLock);

			//ѡ�����ͼ
			//wsprintf(path,TEXT("%s\\game_Card_800_SELECT.bmp"), SKIN_CARD);
			m_CardPic[5].SetLoadInfo(".\\image\\CardSkin\\game_Card_800_SELECT.bmp",CGameImageLink::m_bAutoLock);

			//wsprintf(path,TEXT("%s\\game_Card_800_back_SELECT.bmp"), SKIN_CARD);
			m_CardPic[6].SetLoadInfo(".\\image\\CardSkin\\game_Card_800_back_SELECT.bmp",CGameImageLink::m_bAutoLock);
		}
		break;
	}
	wsprintf(path,TEXT(".\\image\\CardSkin\\cardnum.bmp"));
	m_Num.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	if(m_Num.GetWidth() <= 0)
	{
		wsprintf(path,TEXT(".\\image\\CardSkin\\cardnum.bmp"));
		m_Num.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	}
	return ;
}
//�����˿˸ߺͿ�
void CUpGradeGameCard::SetCardWidthHeight(int iWidth,int iHeight)
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

//{{ qy �ƿؼ��޸�20091109
// ���ƿ�ѡ��Χ
void CUpGradeGameCard::DisplayInfo()
{
	CDC *pDeskDc=GetDesktopWindow()->GetWindowDC();//GetWindowDC();//
	if (NULL == pDeskDc)
	{
		return ;
	}

	CRect rc = m_RectDis;
	ClientToScreen(rc);
	CPen penLine(PS_SOLID, 1,m_SelecColor);
	CBrush brTmp;
	brTmp.CreateSolidBrush(m_SelecColor);      //��͸������ɫ 
	int iSet =	pDeskDc->SetROP2( R2_MASKPEN );//and����	
	CBrush *pOldB = pDeskDc->SelectObject( &brTmp ); 
	CPen *pOldP = pDeskDc->SelectObject( &penLine ); 
	pDeskDc->Rectangle( &rc );//ʵ�Ļ�ˢ��� 
	brTmp.DeleteObject(); 
	pDeskDc->SetROP2(iSet);
	pDeskDc->SelectObject(pOldP);
	pDeskDc->SelectObject(pOldB);
	pDeskDc->DeleteDC();
	
}

// ����λ�û�ȡ��ѡ���Ƽ���Χ
int CUpGradeGameCard::GetCardsIndex(CRect &rect)
{
	m_aSelIndex.RemoveAll();
	CRect rectOld = rect;
	rect.SetRect(0,0,0,0);
	if (m_bHorzLook == true)
	{
		int iMinIndex = 0;
		int iMaxIndex = 0;
		// ��ʼ����
		CPoint stPoint(rectOld.left,rectOld.top);
		CPoint endPoint(rectOld.right, rectOld.bottom);
		//OutputDebugMsg("qlog: ��������ֵstart(%d  %d),   end (%d   %d)",stPoint.x, stPoint.y,endPoint.x, endPoint.y);

		// ����������Ƿ�����
		BYTE byStart = HitCardTest(stPoint);
		if ((255 == byStart) || (byStart >= m_bCardCount))
		{
			return -1;
		}

		// ��ȡ���һ����
		int byMinStart = 0;
		if ((stPoint.x-m_iCardWidth) >  m_iCardHSpace)
		{
			byMinStart = ((stPoint.x-m_iCardWidth)/m_iCardHSpace);
			if (byMinStart >= m_bCardCount)
			{
				byMinStart = m_bCardCount-1;
			}
		}

		int byMaxStart = 0;
		if ((stPoint.x) >  m_iCardHSpace)
		{
			byMaxStart = (stPoint.x/m_iCardHSpace);
			if (byMaxStart >= m_bCardCount)
			{
				byMaxStart = m_bCardCount-1;
			}
		}

		CRect rct;
		GetClientRect(rct);
		CRect rectEnd ;

		// ��ȡ���һ����
		int byMin = 0;
		if ((endPoint.x-m_iCardWidth) >  m_iCardHSpace)
		{
			byMin = ((endPoint.x-m_iCardWidth)/m_iCardHSpace);
			if (byMin >= m_bCardCount)
			{
				byMin = m_bCardCount-1;
			}
		}

		int byMax = 0;
		if ((endPoint.x) >  m_iCardHSpace)
		{
			byMax = (endPoint.x/m_iCardHSpace);
			if (byMax >= m_bCardCount)
			{
				byMax = m_bCardCount-1;
			}
		}
		// ��������Ľ���	
		BYTE byStartIndex = min(byMinStart, byMaxStart);
		byStartIndex = min(byStartIndex, byMin);
		byStartIndex = min(byStartIndex, byMax);

		// ����λ��
		BYTE byEndIndex = max(byMinStart, byMaxStart);
		byEndIndex = max(byEndIndex, byMin);
		byEndIndex = max(byEndIndex, byMax);
		//OutputDebugMsg("qlog: ���ҷ�Χ  %d ==== > %d    byMinStart=%d, byMaxStart=%d, byMin=%d byMax=%d",
		//	byStartIndex, byEndIndex, byMinStart, byMaxStart,byMin, byMax);

		iMinIndex = -1;
		CRgn rgnCard;
		rgnCard.CreateRectRgn(0,0,0,0);
		for (BYTE byIndex=byStartIndex; byIndex<(byEndIndex+1); ++byIndex)
		{
			rgnCard.SetRectRgn(0,0,0,0);
			GetCardRect(byIndex, rgnCard);
			if (rgnCard.RectInRegion(rectOld))  // �ж��Ƿ��������򽻴�
			{
				m_aSelIndex.Add(byIndex);
				if (-1==iMinIndex)
				{
					iMinIndex=byIndex;
				}
				iMaxIndex = byIndex;
			}
		}
		rgnCard.DeleteObject();
		if (m_aSelIndex.GetSize() <= 0)
		{
			iMinIndex = 0;
			return -1;
		}
	//	OutputDebugMsg("qlog: ���ҵ���������iMinIndex=%d  iMaxIndex=%d", iMinIndex, iMaxIndex);

		// ����ת��
		// �����յ��������ƿؼ���Χ��
		if (endPoint.x <= rct.left)
		{
			endPoint.x = rct.left+1;
		}
		else if (endPoint.x>=rct.right)
		{
			endPoint.x = rct.right-1;
		}
		if (endPoint.y<=rct.top)
		{
			endPoint.y = rct.top+1;
		}
		else if (endPoint.y>=rct.bottom)
		{
			endPoint.y = rct.bottom-1;
		}

		// ��ʼ����
		if (endPoint.x<stPoint.x)
		{
			rect.left  = endPoint.x;
			rect.right = stPoint.x;
		}
		else
		{
			rect.left = stPoint.x;
			rect.right = endPoint.x;
		}

		// �յ�����
		if (endPoint.y<stPoint.y)
		{
			rect.top = endPoint.y;
			rect.bottom = stPoint.y;
		}
		else
		{
			rect.top = stPoint.y;
			rect.bottom = endPoint.y;
		}

		// ����x����
		stPoint.x = iMinIndex*m_iCardHSpace;               // ��С��x����
		endPoint.x = iMaxIndex*m_iCardHSpace+m_iCardWidth; // ����x����
		if (rect.left < stPoint.x)
		{
			rect.left = stPoint.x;
		}
		if (rect.left > endPoint.x)
		{
			rect.left = endPoint.x;
		}
		if (rect.right < stPoint.x)
		{
			rect.right = stPoint.x;
		}
		if (rect.right > endPoint.x)
		{
			rect.right = endPoint.x;
		}
		

		// �ж��Ƿ����еĶ�����
		int iCount = 0;   // ����û�б������������
		int iCountUp = 0;
		for (int i=0; i<m_aSelIndex.GetSize(); ++i)
		{
			// û�б�����
			if (0 == m_CardUpArray.GetAt(m_aSelIndex.GetAt(i)))
			{
				iCount ++;
			}
			else
			{
				iCountUp ++;
			}
			if (iCount > 0 && iCountUp>0) // �е�����ƣ�����������
			{
				if (rect.top == rect.bottom)
				{
					rect.bottom = rect.top+1;
				}
				return ALL_CARDS_NORMAL;
			}
		}

		if (0 == iCount)
		{
			if (rect.bottom >= (rct.bottom-m_iCardUpSpace))
			{
				rect.bottom = rct.bottom-m_iCardUpSpace-1;
			}
			if (rect.top == rect.bottom)
			{
				rect.bottom = rect.top+1;
			}
			return ALL_CARDS_UP;
		}
		else
		{
			if (rect.top <= m_iCardUpSpace)
			{
				rect.top = m_iCardUpSpace+1;
			}
			if (rect.top == rect.bottom)
			{
				rect.bottom = rect.top+1;
			}
			return ALL_CARDS_DOWN;
		}
	}
	else
	{
		return -1;
	}
}

// ����������ȡ�����ƵĿ�������
void CUpGradeGameCard::GetCardRect(BYTE byIndex, CRgn &rgnCard)
{
	rgnCard.SetRectRgn(0,0,0,0);
	if (byIndex >= m_bCardCount)   // ������Χ
	{
		return ;
	}
	// ����
	CRect rect;
	if (m_CardUpArray.GetAt(byIndex) > 0)  // �Ǳ��������
	{
		rect.left = byIndex * m_iCardHSpace;
		rect.top = 0;
		rect.right = rect.left + m_iCardWidth;
		rect.bottom = rect.top + m_iCardHight;
	}
	else
	{
		rect.left = byIndex * m_iCardHSpace; 
		rect.top = m_iCardUpSpace;
		rect.right = rect.left + m_iCardWidth;
		rect.bottom = rect.top + m_iCardHight;
	}
	rgnCard.SetRectRgn(rect);
	// ����������һ�� ��Ҫ�����������
	if (byIndex<(m_bCardCount-1))
	{
		CRect rectTemp;                        // ʣ�ಿ��
		if (m_CardUpArray.GetAt(byIndex) > 0)  // �Ǳ��������
		{
			rect.left = byIndex * m_iCardHSpace;
			rect.top = 0;
			rect.right = rect.left + m_iCardHSpace;
			rect.bottom = rect.top + m_iCardHight;
			rectTemp.left = rect.right;
			rectTemp.top = rect.top;
			rectTemp.right = rect.right;
			rectTemp.bottom = m_iCardUpSpace;
		}
		else
		{
			rect.left = byIndex * m_iCardHSpace; 
			rect.top = m_iCardUpSpace;
			rect.right = rect.left + m_iCardHSpace;
			rect.bottom = rect.top + m_iCardHight;
			rectTemp.left = rect.right;
			rectTemp.right = rect.right;
			rectTemp.top = rect.bottom-m_iCardUpSpace;
			rectTemp.bottom = rect.bottom;			
		}
		rgnCard.SetRectRgn(rect);

		// �ܹ���Ҫ����������
		BYTE bFindCount= m_iCardWidth/m_iCardHSpace+1;
		for (BYTE i=1;i<bFindCount; ++i)
		{
			BYTE byIndexTemp = byIndex + i;
			if (byIndexTemp > (m_bCardCount-1))
			{
				break;
			}
			rectTemp.right = byIndexTemp * m_iCardHSpace;
			if (((m_CardUpArray.GetAt(byIndexTemp)>0)&& (m_CardUpArray.GetAt(byIndex)>0))
				|| ((0 == m_CardUpArray.GetAt(byIndexTemp)) && (0 == m_CardUpArray.GetAt(byIndex))))
			{
				break;
			}
		}
		CRgn rgnTemp;
		rgnTemp.CreateRectRgnIndirect(rectTemp);
		rgnCard.CombineRgn(&rgnCard, &rgnTemp, RGN_OR);
		rgnTemp.DeleteObject();
	}
}

BOOL CUpGradeGameCard::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;//CWnd::OnEraseBkgnd(pDC);
}
//}} qy �ƿؼ��޸�20091109
