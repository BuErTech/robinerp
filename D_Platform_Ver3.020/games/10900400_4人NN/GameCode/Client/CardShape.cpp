// CardShape.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CardShape.h"


// CCardShape

IMPLEMENT_DYNAMIC(CCardShape, CWnd)

CCardShape::CCardShape()
{
	m_byShape = -1;
	m_pt.x = 0;
	m_pt.y = 0;
}

CCardShape::~CCardShape()
{

}


BEGIN_MESSAGE_MAP(CCardShape, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CCardShape ��Ϣ�������

int CCardShape::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	return 0;
}

BOOL CCardShape::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

void CCardShape::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
#ifdef	SAO_YANG
	Graphics g(GetDC()->m_hDC);
	g.DrawImage(m_pImage, Rect(8,5,m_pImage->GetWidth(),m_pImage->GetHeight()));
#else
	CGameImageHelper help(&m_imgShape);
	help.BitBlt(dc, 0, 0);
#endif

}

/// ������ʾλ��
void CCardShape::SetPoint(int x, int y)
{	
	m_pt.x = x;
	m_pt.y = y;
	
	ShowCardShape(m_byShape);
}

/// ��ʾ����
void CCardShape::ShowCardShape(BYTE byShape)
{	
	m_byShape = byShape;

	if(m_byShape > 10)
	{
		return;
	}

#ifdef	SAO_YANG
	CString strTemp;
	strTemp.Format(".\\image\\%dNum.png",m_byShape);

	m_pImage = Image::FromFile(CA2W(strTemp));
	CreateRgn();

	MoveWindow(m_pt.x+10, m_pt.y, m_pImage->GetWidth(), m_pImage->GetHeight());
#else
	TCHAR szPath[MAX_PATH];
	wsprintf(szPath, ".\\image\\%dNum.bmp", m_byShape);
	m_imgShape.SetLoadInfo(szPath, CGameImageLink::m_bAutoLock);

	CGameImageHelper help(&m_imgShape);
	HRGN hRgn = AFCBmpToRgn(help, RGB(255, 0, 255), RGB(1, 1, 1));     // ���˵�RGB(255,0,255)����ɫ����
	if (hRgn != NULL)
	{
		SetWindowRgn(hRgn,TRUE);        // ����Ϊû�й��˵�������
	}

	MoveWindow(m_pt.x, m_pt.y, help.GetWidth(), help.GetHeight());
#endif
	
	Invalidate(FALSE);

	ShowWindow(SW_SHOW);
}

//<��������
void CCardShape::CreateRgn()
{
	HRGN hRgn = NULL;

	Image * pImage = m_pImage;
	if (pImage == NULL)
	{
		return ;
	}

	// ����͸��ɫ����
	Region* _rgn = NULL;
	int nW = pImage->GetWidth();
	int nH = pImage->GetHeight();

	///////////////////////  ����͸��ɫ���� /////////////////////
	Bitmap* bmp = new Bitmap(nW, nH);   
	Graphics* g = Graphics::FromImage(bmp);  

	RectF rect;
	rect.X = 8;
	rect.Y = 5;
	rect.Width = nW;
	rect.Height = nH;
	g->DrawImage(pImage, rect, 0, 0, nW, nH, UnitPixel, NULL, NULL, NULL);

	GraphicsPath gp;   
	for (int i = 0; i < nW; i ++)   
	{   
		for (int j = 0; j < nH; j++)   
		{   
			Color cl;   
			bmp->GetPixel(i, j, &cl);   
			if (cl.GetA() != 0)   
			{
				gp.AddRectangle(Rect(i, j, 1, 1));   
			}
		}  
	}   

	delete g;
	delete bmp;   
	_rgn = new Region(&gp);  
	///////////////////////////////////////////////////////////////

	if (_rgn != NULL)
	{
		Graphics g(GetDC()->m_hDC);   
		hRgn = ::CreateRectRgn(0, 0, 0, 0);    
		CombineRgn(hRgn, _rgn->GetHRGN(&g), NULL, RGN_COPY);    
		delete _rgn;

		SetWindowRgn(hRgn, FALSE);
	}
}