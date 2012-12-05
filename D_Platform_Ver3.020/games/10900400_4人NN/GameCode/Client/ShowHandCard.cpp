#include "Stdafx.h"
#include "ShowHandCard.h"

BEGIN_MESSAGE_MAP(CShowHandCard, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
END_MESSAGE_MAP()

bool					CShowHandCard::m_bInit=false;						//��ʼ��־
HCURSOR					CShowHandCard::m_hCursor=NULL;						//�����
CAFCImage				CShowHandCard::m_CardPic;							//�˿˱���
CAFCImage				CShowHandCard::m_BackPic;							//�˿˱���

//���캯��
CShowHandCard::CShowHandCard()
{
	m_iCardWidth=70;
	m_iCardHight=95;
	m_iCardHSpace=16;
	m_bCardCount=0;
	m_bUseMouse=false;
	m_bShowFirst=false;
	m_XShowMode=SWX_MODE_LEFT;
	m_YShowMode=SWY_MODE_TOP;
	if (m_bInit==false)
	{
		m_bInit=true;
		m_hCursor=LoadCursor(GetModuleHandle(CLIENT_DLL_NAME),MAKEINTRESOURCE(IDC_CARD_CUR));
		m_CardPic.SetLoadInfo(GetModuleHandle(CLIENT_DLL_NAME),IDB_GAME_CARD,CAFCResourceManage::m_bAutoLock);
		m_BackPic.SetLoadInfo(GetModuleHandle(CLIENT_DLL_NAME),IDB_CARD_BACK,CAFCResourceManage::m_bAutoLock);
	}
	return;
}

//��������
CShowHandCard::~CShowHandCard()
{
	return;
}

//��ȡ��С����
bool CShowHandCard::GetCardSize(SIZE & Size)
{
	if (m_bCardCount==0)
	{
		Size.cx=0;
		Size.cy=0;
	}
	else
	{
		Size.cx=m_iCardWidth+(m_bCardCount-1)*m_iCardHSpace;
		Size.cy=m_iCardHight;
	}
	return true;
}

//�����Ƿ�ʹ�����
void CShowHandCard::SetUseMouse(bool bUserMouse)
{
	m_bUseMouse=bUserMouse;
	return;
}

//�����Ƿ���ʾ��һ���˿�
void CShowHandCard::SetShowFirst(bool bShowFirst)
{
	m_bShowFirst=bShowFirst;
	Invalidate(FALSE);
	return;
}

//�����˿�
void CShowHandCard::SortCard()
{
	if (m_bCardCount>0)
	{
		BYTE bTemp,bLast=m_bCardCount-1;
		BOOL bSorted=TRUE;
		do
		{
			bSorted=TRUE;
			for (BYTE i=0;i<bLast;i++) 
			{
				if (CompareOnlyOne(m_CardArray[i],m_CardArray[i+1])==false)
				{	
					bTemp=m_CardArray[i];
					m_CardArray[i]=m_CardArray[i+1];
					m_CardArray[i+1]=bTemp;
					bSorted=FALSE;
				}	
			}
			bLast--;
		} while(!bSorted);
	}
	
	return;
}

//�����˿�
bool CShowHandCard::SetCard(BYTE bCardList[], BYTE bCardCount)
{
	if (bCardCount>m_CardArray.GetSize()) m_CardArray.SetSize(bCardCount);
	CopyMemory(m_CardArray.GetData(),bCardList,sizeof(BYTE)*bCardCount);
	m_bCardCount=bCardCount;
	FixCardControl();
	return true;
}

//�����˿�
BYTE CShowHandCard::GetCard(BYTE bCardBuffer[], BYTE bBufferLength)
{
	if (bCardBuffer!=NULL) CopyMemory(bCardBuffer,m_CardArray.GetData(),__min(m_bCardCount,bBufferLength)*sizeof(BYTE));
	return m_bCardCount;
}

//���û�׼�㺯��
void CShowHandCard::SetBasePoint(POINT & BasePoint, XShowMode XShowMode, YShowMode YShowMode)
{
	m_XShowMode=XShowMode;
	m_YShowMode=YShowMode;
	m_PtBasePoint=BasePoint;
	FixCardControl();
	return;
}

//��ȡ�˿���ֵ
BYTE CShowHandCard::GetCardValoe(BYTE bCard)
{
	BYTE bVolea=(bCard-1)%13;
	if (bVolea==0) bVolea=14;
	return bVolea;
}

//��������λ��
void CShowHandCard::FixCardControl()
{
	//��ȡλ��
	SIZE Size;
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
	Invalidate(FALSE);

	return;
}

//�Աȵ�ֻ�˿�
bool CShowHandCard::CompareOnlyOne(BYTE bFirstCard, BYTE bLastCard)
{
	BYTE bFirstVolae=GetCardValoe(bFirstCard);
	BYTE bLastVolae=GetCardValoe(bLastCard);
	if (bFirstVolae==bLastVolae) return bFirstCard>bLastCard;
	return bFirstVolae>bLastVolae;
}

//�ػ�����
void CShowHandCard::OnPaint()
{
	CPaintDC dc(this);
	if (m_bCardCount==0) return;

	//����λͼ
	CAFCImageHandle CardHandle(&m_CardPic);
	CAFCImageHandle BackHandle(&m_BackPic);

	//�滭�˿�
	if ((m_bShowFirst==true)&&(m_CardArray[0]!=0))
	{
		//�滭�˿�
		CardHandle.BitBlt(dc,0,0,(m_bCardCount==1)?m_iCardWidth:m_iCardHSpace,m_iCardHight,
			GetCardXPos(m_CardArray[0])*m_iCardWidth,GetCardYPos(m_CardArray[0])*m_iCardHight);
	}
	else 
	{
		//�滭����
		BackHandle.BitBlt(dc,0,0,(m_bCardCount==1)?m_iCardWidth:m_iCardHSpace,m_iCardHight,0,0);
	}

	for (BYTE i=1;i<m_bCardCount;i++)
	{
		if (m_CardArray[i]==0)
		{
			//�滭����
			BackHandle.BitBlt(dc,i*m_iCardHSpace,0,(i!=(m_bCardCount-1))?m_iCardHSpace:m_iCardWidth,m_iCardHight,0,0);
		}
		else
		{
			//�滭�˿�
			CardHandle.BitBlt(dc,i*m_iCardHSpace,0,(i!=(m_bCardCount-1))?m_iCardHSpace:m_iCardWidth,m_iCardHight,
				GetCardXPos(m_CardArray[i])*m_iCardWidth,GetCardYPos(m_CardArray[i])*m_iCardHight);
		}
	}
	return;
}

//�Ҽ���Ϣ
void CShowHandCard::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags, point);

	if ((m_bUseMouse==true)&&(m_bShowFirst==false)&&(point.x<(int)m_iCardHSpace)&&(m_bCardCount>0))
	{
		CDC * pWindowDC=GetDC();
		CAFCImageHandle CardHandle(&m_CardPic);
		CardHandle.BitBlt(pWindowDC->GetSafeHdc(),0,0,(m_bCardCount==1)?m_iCardWidth:m_iCardHSpace,m_iCardHight,
			GetCardXPos(m_CardArray[0])*m_iCardWidth,GetCardYPos(m_CardArray[0])*m_iCardHight);
		ReleaseDC(pWindowDC);
		SetCapture();
	}

	return;
}

//�Ҽ���Ϣ
void CShowHandCard::OnLButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags, point);

	ReleaseCapture();
	if ((m_bUseMouse==true)&&(m_bShowFirst==false))
	{
		CDC * pWindowDC=GetDC();
		CAFCImageHandle BackHandle(&m_BackPic);
		BackHandle.BitBlt(pWindowDC->GetSafeHdc(),0,0,(m_bCardCount==1)?m_iCardWidth:m_iCardHSpace,m_iCardHight,0,0);
		ReleaseDC(pWindowDC);
	}

	return;
}

//���ù��
BOOL CShowHandCard::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	POINT PtMousePoint;
	GetCursorPos(&PtMousePoint);
	ScreenToClient(&PtMousePoint);
	
	if ((m_bUseMouse==true)&&(m_bShowFirst==false)&&(PtMousePoint.x<m_iCardHSpace))
	{
		SetCursor(m_hCursor);
		return TRUE;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

//������Ϣ
int CShowHandCard::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct)==-1) return -1;

	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return 0;
}
