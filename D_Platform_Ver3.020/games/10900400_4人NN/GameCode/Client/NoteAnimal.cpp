#include "Stdafx.h"
#include "NoteAnimal.h"

BEGIN_MESSAGE_MAP(CNoteAnimal, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//���캯��
CNoteAnimal::CNoteAnimal(BOOL bOverlap)
{
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
	SetOverlap(bOverlap);
	m_pHandleWnd=NULL;
	m_bHorzLook=true;
	//��ע������ֵ
	m_iTotalJeton=0;
	//����ʹ�ó�������
	m_iArrayJeton[0]=1;//{1,2,5,10,20,50,100,200,500,1000,2000,5000,10000};
	m_iArrayJeton[1]=2;
	m_iArrayJeton[2]=5;
	m_iArrayJeton[3]=10;
	m_iArrayJeton[4]=20;
	m_iArrayJeton[5]=50;
	m_iArrayJeton[6]=100;
	m_iArrayJeton[7]=200;
	m_iArrayJeton[8]=500;
	m_iArrayJeton[9]=1000;
	m_iArrayJeton[10]=2000;
	m_iArrayJeton[11]=5000;
	m_iArrayJeton[12]=10000;
	m_iArrayJeton[13]=20000;
	m_iArrayJeton[14]=50000;
	m_iArrayJeton[15]=100000;
	//AfxSetResourceHandle(GetModuleHandle(NULL));
}

//��������
CNoteAnimal::~CNoteAnimal()
{
	    if(m_pMoveCard)delete m_pMoveCard;
}

//��ʼ������
bool CNoteAnimal::InitGameCard(CWnd * pHandleWnd)
{
	m_pHandleWnd=pHandleWnd;
	return true;
}

//�����˿˲���
void CNoteAnimal::SetCardParameter(int iWidth,int iHeight,int iXoffset,int iYoffset)
{
	m_iWidth=iWidth;
	m_iHeight=iHeight;
	m_iXoffset=iXoffset;
	m_iYoffset=iYoffset;
	FixCardControl();
	return;
}

//��ȡ��С����
bool CNoteAnimal::GetCardSize(CSize & Size)
{
	if (m_bJetonCount>0)
	{
		if(GetLookMode())
		{
			Size.cx=m_iWidth*m_bJetonCount;
			Size.cy=m_iHeight;
		}else
		{
			Size.cx=m_iWidth;
			Size.cy=m_iHeight*m_bJetonCount;
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
void CNoteAnimal::SetBasePoint(CPoint & BasePoint)
{
	if(m_iTotalJeton<0||m_iTotalJeton>100000)
		m_iTotalJeton=0;
	m_PtBasePoint=BasePoint;
	FixCardControl();
	return;
}

void CNoteAnimal::AddJeton(int iTotal)
{
	if(iTotal<0||iTotal>100000)
		iTotal=0;
	m_iTotalJeton+=iTotal;
	BYTE temp[16]={0};
	m_bJetonCount=AnalyseJeton(m_iTotalJeton,m_iArrayJeton,15,temp);
	FixCardControl();
}

void CNoteAnimal::SetTotalJeton(int iTotal)
{
	if(iTotal<0||iTotal>100000)
		iTotal=0;
	m_iTotalJeton=iTotal;
	BYTE temp[16]={0};
	m_bJetonCount=AnalyseJeton(iTotal,m_iArrayJeton,15,temp);
	FixCardControl();
}

//������������
int CNoteAnimal::AnalyseJeton(int iTotalJeton,int iArrayJeton[],BYTE iJetonCount,BYTE bJeton[])
{
	int total=0;
	::memset(bJeton,0,sizeof(bJeton));

	while(iTotalJeton>0)
		if(iTotalJeton>=iArrayJeton[iJetonCount])
		{
			iTotalJeton-=iArrayJeton[iJetonCount];
			bJeton[iJetonCount]++;
			total++;
		}
		else
		{
			iJetonCount--;
		}
		
	return total;
}
//������ʾ��ʽ
void CNoteAnimal::SetLookMode(bool bHorzLook)
{
	m_bHorzLook=bHorzLook;
	return;
}
//������ʾ��ʽ
BOOL CNoteAnimal::GetLookMode()
{
	return m_bHorzLook;
}
//��������λ��
void CNoteAnimal::FixCardControl()
{

	//��ȡλ��
	CSize Size;
	GetCardSize(Size);

	//����λ��
	CRect CardRect;
	CardRect.left=m_PtBasePoint.x-Size.cx/2;
	CardRect.top=m_PtBasePoint.y-Size.cy/2;

	//�ƶ�λ��
	CardRect.right=CardRect.left+Size.cx;
	CardRect.bottom=CardRect.top+Size.cy;
	MoveWindow(&CardRect);
	Invalidate(FALSE);

	//��������
	CRgn AllRgn,SignedRgn;
	AllRgn.CreateRectRgn(0,0,0,0);
	BYTE tem[16]={0};
	int count=AnalyseJeton(m_iTotalJeton,m_iArrayJeton,15,tem);
	int setX=0,setY=0;
	if(GetLookMode())//�����ص���ʽ
	{
		for(int i=15;i>=0;i--)
		{
			if(tem[i])
			{
				for(int j=0;j<tem[i];j++)
				{
					SignedRgn.CreateEllipticRgn(setX*m_iWidth+setY*m_iXoffset,0+setY*m_iYoffset,(setX+1)*m_iWidth+setY*m_iXoffset,m_iHeight+setY*m_iYoffset);
					AllRgn.CombineRgn(&AllRgn,&SignedRgn,RGN_OR);
					SignedRgn.DeleteObject();
					//				setY++;
					setX++;
				}
			}
		}
	}
	else
	{
		for(int i=15;i>=0;i--)
		{
			if(tem[i])
			{
				for(int j=0;j<tem[i];j++)
				{
					SignedRgn.CreateEllipticRgn(0,setX*m_iHeight,m_iWidth,m_iHeight*(setX+1));
					AllRgn.CombineRgn(&AllRgn,&SignedRgn,RGN_OR);
					SignedRgn.DeleteObject();
					//				setY++;
					setX++;
				}
			}
		}

	}
/*	for (BYTE i=0;i<m_bJetonCount;i++)
	{
		SignedRgn.CreateEllipticRgn(i*m_iWidth,0,(i+1)*m_iWidth,m_iHeight);
		AllRgn.CombineRgn(&AllRgn,&SignedRgn,RGN_OR);
		SignedRgn.DeleteObject();
	}*/

	//��������
	SetWindowRgn(AllRgn,TRUE);
	m_CardViewRgn.DeleteObject();
	m_CardViewRgn.Attach(AllRgn.Detach());

	return;
}

//������Ϣ
int CNoteAnimal::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct)==-1) return -1;

	m_iHeight=30;
	m_iWidth=31;	
	m_iXoffset=2;
	m_iYoffset=2;

	TCHAR szName[MAX_PATH];
	wsprintf(szName,".\\image\\jeton.bmp");
	m_CardPic.SetLoadInfo(szName,CGameImageLink::m_bAutoLock);
	m_pMoveCard=new CGameImageHelper(&m_CardPic);

	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	return 0;
}

//�ػ�����
void CNoteAnimal::OnPaint()
{
	CPaintDC dc(this);
	if (m_bJetonCount==0) return;

	//��������
	CDC BufferDC;
	CSize CardSize;
	CBitmap BufferBmp;
	GetCardSize(CardSize);
	BufferDC.CreateCompatibleDC(&dc);
	BufferBmp.CreateCompatibleBitmap(&dc,CardSize.cx,CardSize.cy);
	CBitmap * pOldBmp=BufferDC.SelectObject(&BufferBmp);
	
	//����λͼ
	CGameImageHelper CardHandle(&m_CardPic);
	 
	//�滭�˿�
	int iXPos=0,iYPos=0;
	CGameImageHelper * pActiveHandle=NULL;
	BYTE tem[16]={0};
	int count=AnalyseJeton(m_iTotalJeton,m_iArrayJeton,15,tem);
	int setX=0,setY=0;
	if(GetLookMode())//�����ص���ʽ
	{
		for (int i=15;i>=0;i--)
		{
			if(tem[i])
			{
				iXPos=GetCardXPos(i)*m_iWidth;
				iYPos=GetCardYPos(0)*m_iHeight;
				pActiveHandle=&CardHandle;

				for(int j=0;j<tem[i];j++)
				{
					pActiveHandle->BitBlt(BufferDC,setX*m_iWidth+setY*m_iXoffset,0+setY*m_iYoffset,(setX+1)*m_iWidth+setY*m_iXoffset,m_iHeight+setY*m_iYoffset,iXPos,iYPos);
					//				setY++;
					setX++;
				}
			}
		}

	}
	else
	{
		for (int i=15;i>=0;i--)
		{
			if(tem[i])
			{
				iXPos=GetCardXPos(i)*m_iWidth;
				iYPos=GetCardYPos(0)*m_iHeight;
				pActiveHandle=&CardHandle;

				for(int j=0;j<tem[i];j++)
				{
					pActiveHandle->BitBlt(BufferDC,0,setX*m_iHeight,m_iWidth,m_iHeight*(setX+1),iXPos,iYPos);
					//				setY++;
					setX++;
				}
			}
		}
	}

	//ˢ�½���
	dc.BitBlt(0,0,CardSize.cx,CardSize.cy,&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.SelectObject(pOldBmp);
	BufferDC.DeleteDC();
	BufferBmp.DeleteObject();

	return;
}
