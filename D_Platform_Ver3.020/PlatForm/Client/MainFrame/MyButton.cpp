#include "stdafx.h"

#include "MyButton.h"

BEGIN_MESSAGE_MAP(BzButton, CButton)
  ON_WM_MOUSEMOVE()
  ON_WM_TIMER()
  ON_WM_LBUTTONDOWN()
  ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/**
 * ���캯��
 */
BzButton::BzButton()
	: m_Mousex(0),m_Mousey(0),m_Width(0),m_Height(0),
	  m_isRgn(true)
{
	m_state = NORMAL;
	m_bMouseOver = false;
	m_bDCStored = false;
}

/**
 * ��������
 */
BzButton::~BzButton()
{
	m_memDC.DeleteDC();
}

/**
 * ��������Ҫ��ʾ��ͼƬ
 *
 * @param path ����Ҫ��ʾ��ͼƬ·��
 */
void BzButton::SetImage(CString path)
{
	// ��ձ���
	ClearBackground();

	if(!m_image.IsNull())
		m_image.Destroy();

	m_image.Load(path);

	if(m_image.IsNull())
	{
		return;
	}

	m_Width = m_image.GetWidth() / 4;
	m_Height = m_image.GetHeight();
	m_isRgn = true;
}

/**
 * ����Դ�ļ��е���ͼƬ
 */
void BzButton::LoadImageFromeResource(HINSTANCE hInstance,UINT nIDResource )
{
	// ��ձ���
	ClearBackground();

	if(!m_image.IsNull())
		m_image.Destroy();

	m_image.LoadFromResource(hInstance,nIDResource);

	if(m_image.IsNull())
	{
		return;
	}

	m_Width = m_image.GetWidth() / 4;
	m_Height = m_image.GetHeight();
	m_isRgn = true;
	MoveWindow(0,0,m_Width,m_Height);
}

/**
 * �ı䰴ť����ʾλ��
 *
 * @param pos �µİ�ť��ʾλ��
 */
void BzButton::SetPosition(CPoint pos)
{
	MoveWindow(pos.x,pos.y,m_Width,m_Height);
}

void BzButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	UINT state = lpDIS->itemState; 

	m_state = NORMAL;

	if ((state & ODS_FOCUS)  &&
			PointInRect(CPoint(m_Mousex,m_Mousey)))
	{
		if ((state & ODS_SELECTED))
		{ 
			m_state = DOWN;
			m_isRgn = true;
		}
		else
		{
			if(m_bMouseOver)
			{
			   m_state = HOVER;
			}
		}
	}
	else
	{
		m_state = NORMAL;
	}
	if (state & ODS_DISABLED)
	{
		m_state = DISABLE;
	}

	DrawButton();
}

void BzButton::DrawButton(void)
{
	CClientDC  dc(this);

	CRect	rect;
	GetClientRect(&rect);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap Screen;
	Screen.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	memDC.SelectObject(&Screen);
	Screen.DeleteObject();

	//������ 
	//memDC.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &m_memDC, 0, 0, SRCCOPY);

	memDC.SetBkMode(TRANSPARENT);
	memDC.FillSolidRect(&rect,RGB(255,0,255));
//	memDC.SelectObject(BBaseContainer::m_Font);

	switch(m_state)
	{
	case NORMAL:
		
		if(!m_image.IsNull())
			m_image.Draw(memDC,rect.left,rect.top,rect.right,rect.bottom,
			                      0,0,m_Width,m_Height);
		break;
	case HOVER:

		if(!m_image.IsNull())
			m_image.Draw(memDC,rect.left,rect.top,rect.right,rect.bottom,
			                  m_Width,0,m_Width,m_Height);
		break;
	case DOWN:

		if(!m_image.IsNull())
			m_image.Draw(memDC,rect.left,rect.top,rect.right,rect.bottom,
			                  m_Width*2,0,m_Width,m_Height);
		break;
	case DISABLE:

		if(!m_image.IsNull())
			m_image.Draw(memDC,rect.left,rect.top,rect.right,rect.bottom,
			                  m_Width*3,0,m_Width,m_Height);
		break;
	default:
		break;
	}

	// д��ť����
	if(!m_Text.empty())
	{
		UINT uFormat=DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS;

		memDC.SetTextColor(m_TextColor);
		memDC.DrawText(m_Text.c_str(),lstrlen(m_Text.c_str()),&rect,uFormat);
	}

	//������������
	//if(m_isRgn) 
	//{
	//	HRGN hRgn = BBaseContainer::CreateRgnFromFile(*(memDC.GetCurrentBitmap()),RGB(255,0,255));
	//	SetWindowRgn(hRgn, TRUE );

	//	m_isRgn = false;
	//}

	dc.TransparentBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, rect.left,rect.top,rect.Width(),rect.Height(),RGB(255,0,255));
	//dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.DeleteDC();
}

void BzButton::PreSubclassWindow()
{
	SetOwnerDraw(true);

	CButton::PreSubclassWindow();
}

void BzButton::SetOwnerDraw(bool IsDraw)
{
	if(IsDraw)
	{
		ModifyStyle(NULL, BS_OWNERDRAW); 
		Invalidate();
	}
	else
	{
		ModifyStyle(BS_OWNERDRAW, NULL); 
		Invalidate();
	}
}

/**
 * ��ձ���ͼ
 */
void BzButton::ClearBackground()
{
	m_memDC.DeleteDC();
    m_bDCStored = false;
	m_isRgn = true;

	CRect m_Rect;
	GetClientRect(&m_Rect);
    
	InvalidateRect(m_Rect);
}

void BzButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bMouseOver &&
		PointInRect(point))
	{
		m_bMouseOver=true;
		m_state = HOVER;

		CPoint	point;
		CRect	rect;
		GetWindowRect(&rect);	
		GetCursorPos(&point);
		if (!rect.PtInRect(point) && m_bMouseOver)
		{
			SetTimer(1,10,NULL);
			return;
		}

		DrawButton();
	
		SetTimer(1,10,NULL);
	}

	m_Mousex = point.x; 
	m_Mousey = point.y;

	CButton::OnMouseMove(nFlags, point);
}

void BzButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	//if(PointInRect(point))
	//{
	//	BBzParameter *myParam = new BBzParameter();

	//	myParam->SetValue(EVENT_LPARAM,point.x);
	//	myParam->SetValue(EVENT_RPARAM,point.y);

	//	::PostMessage(GetParent()->m_hWnd,EVENT_BZ_GUI,GetDlgCtrlID(),(LPARAM)myParam);
	//}

	CButton::OnLButtonDown(nFlags,point);
}

/**
 * ��⵱ǰ�����Ƿ��ڰ�ť�Ŀɼ���Χ��
 *
 * @param pPoint Ҫ���İ���
 *
 * @return �����������Ļ��ť�ɼ���Χ�ڷ����棬���򷵻ؼ�
 */
bool BzButton::PointInRect(CPoint pPoint)
{
	if(!m_image.IsNull())
	{
		if((pPoint.x >= 0 && pPoint.x < m_image.GetWidth()) &&
			(pPoint.y >= 0 && pPoint.y < m_image.GetHeight()))
		{
			//if(m_image.GetPixel(pPoint.x,pPoint.y) == RGB(255,0,255))
				return true;
		}
	}

	return false;
}

void BzButton::OnTimer(UINT_PTR nIDEvent)
{
	CPoint	point;
	CRect	rect;
	GetWindowRect(&rect);	
	GetCursorPos(&point);

	if (!rect.PtInRect(point) && m_bMouseOver)
	{
		KillTimer (1);
		m_bMouseOver=false;
		if(m_state != DISABLE) m_state = NORMAL;
		DrawButton();
	}

	CButton::OnTimer(nIDEvent);
}

BOOL BzButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(!m_bDCStored)
	{
    	CRect  rect;
	    GetClientRect(&rect);

		m_memDC.DeleteDC();
		m_memDC.CreateCompatibleDC (pDC);
		CBitmap	btScreen;
		btScreen.CreateCompatibleBitmap (pDC,rect.Width(),rect.Height());
		m_memDC.SelectObject (&btScreen);

		//m_memDC.FillSolidRect(&rect,RGB(255,0,255));
		m_memDC.BitBlt (0,0,rect.Width(),rect.Height(),pDC,0,0,SRCCOPY);

		m_bDCStored=true;

		btScreen.DeleteObject();
	}
	return TRUE;// CButton::OnEraseBkgnd(pDC);//
}