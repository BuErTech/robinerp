#pragma once
#include "../stdafx.h"
#include "AFCButton.h"
#include "AFCFunction.h"
#include "AFCResource.h"
//////////����ť�Ľ���//////////////
//using namespace AFC;
//using namespace AFC::AFCButton;
//using namespace AFC::AFCResource;
/*******************************************************************************************************/
BEGIN_MESSAGE_MAP(CTrueBitmapButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CNormalBitmapButton, CTrueBitmapButton)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CNormalBitmapButtonEx, CTrueBitmapButton)
END_MESSAGE_MAP()

/*******************************************************************************************************/

//���캯��
CTrueBitmapButton::CTrueBitmapButton()
{
	m_bIsMouseMove=false;
}
//��������
CTrueBitmapButton::~CTrueBitmapButton()
{
	m_bIsMouseMove=false;
}
//���󸽼ӵ����д���
void CTrueBitmapButton::PreSubclassWindow()
{
	SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE) | WS_CLIPSIBLINGS);
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW);
	SetFont(&CGameImageLink::m_HFont,FALSE);
	CButton::PreSubclassWindow();
}
//��������
bool CTrueBitmapButton::EnableSound(bool bEnable)
{
	CGameImageLink::m_ButtonResource.m_bSound=bEnable;
	return CGameImageLink::m_ButtonResource.m_bSound;
}
//���������
bool CTrueBitmapButton::LoadButtonCursor(HCURSOR hCursor)
{
	if (CGameImageLink::m_ButtonResource.m_hMouneCursor!=NULL)
	{
		DestroyCursor(CGameImageLink::m_ButtonResource.m_hMouneCursor);
		CGameImageLink::m_ButtonResource.m_hMouneCursor=NULL;
	}
	CGameImageLink::m_ButtonResource.m_hMouneCursor=hCursor;
	return (CGameImageLink::m_ButtonResource.m_hMouneCursor!=NULL);
}

//���������
bool CTrueBitmapButton::LoadButtonCursor(HINSTANCE hInstance, UINT uCursorID)
{
	if (CGameImageLink::m_ButtonResource.m_hMouneCursor!=NULL)
	{
		DestroyCursor(CGameImageLink::m_ButtonResource.m_hMouneCursor);
		CGameImageLink::m_ButtonResource.m_hMouneCursor=NULL;
	}
	CGameImageLink::m_ButtonResource.m_hMouneCursor=LoadCursor(hInstance,MAKEINTRESOURCE(uCursorID));
	return (CGameImageLink::m_ButtonResource.m_hMouneCursor!=NULL);
}
//����ƶ���Ϣ
void CTrueBitmapButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bIsMouseMove==false)
	{
		//ע����Ϣ
		m_bIsMouseMove=true;
		Invalidate(FALSE);
		TRACKMOUSEEVENT TrackMouseEvent;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.hwndTrack=GetSafeHwnd();
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		_TrackMouseEvent(&TrackMouseEvent);

		//��������
		if (CGameImageLink::m_ButtonResource.m_bSound==true)
			AFCPlaySound(GetModuleHandle(FACE_DLL_NAME),TEXT("BUTTON_MOVE"));
	}
	CButton::OnMouseMove(nFlags, point);
}

//����뿪��Ϣ
LRESULT CTrueBitmapButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bIsMouseMove=false;
	Invalidate(FALSE);
	return 0;
}

//�������Ϣ
BOOL CTrueBitmapButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (CGameImageLink::m_ButtonResource.m_hMouneCursor!=NULL) 
	{
		SetCursor(CGameImageLink::m_ButtonResource.m_hMouneCursor);
		return TRUE;
	}
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

/*******************************************************************************************************/

//���캯��
CNormalBitmapButton::CNormalBitmapButton()
{
}
//��������
CNormalBitmapButton::~CNormalBitmapButton()
{
}

//������ɫ
void CNormalBitmapButton::SetColor(COLORREF ActiveColor, COLORREF ActiveTextColor, COLORREF NormalTextColor, COLORREF SelectTextColor,
			  COLORREF FrameColor, COLORREF FrameHeight, COLORREF FrameShadow,COLORREF NormalColorTop, COLORREF NormalColorBottom)
{
	CGameImageLink::m_ButtonResource.m_ActiveColor=ActiveColor;		
	CGameImageLink::m_ButtonResource.m_ActiveTextColor=ActiveTextColor;	
	CGameImageLink::m_ButtonResource.m_NormalTextColor=NormalTextColor;	
	CGameImageLink::m_ButtonResource.m_SelectTextColor=SelectTextColor;	
	CGameImageLink::m_ButtonResource.m_FrameColor=FrameColor;		
	CGameImageLink::m_ButtonResource.m_FrameHeight=FrameHeight;		
	CGameImageLink::m_ButtonResource.m_FrameShadow=FrameShadow;		
	CGameImageLink::m_ButtonResource.m_NormalColorTop=NormalColorTop;	
	CGameImageLink::m_ButtonResource.m_NormalColorBottom=NormalColorBottom;
}

//����滭����
void CNormalBitmapButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC * pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	DWORD nAction=lpDrawItemStruct->itemAction; 
	CRect DrawRect=lpDrawItemStruct->rcItem;
	UINT uStyle=DFCS_BUTTONPUSH;
	CString strText;

	GetWindowText(strText);
	pDC->SetBkMode(TRANSPARENT);

	if (lpDrawItemStruct->itemState&ODS_SELECTED)
	{
		m_bIsMouseMove=0;
		DrawFace(pDC,CGameImageLink::m_ButtonResource.m_NormalColorBottom,CGameImageLink::m_ButtonResource.m_NormalColorTop,DrawRect,DrawRect);
		DrawFrame(pDC,CGameImageLink::m_ButtonResource.m_FrameShadow,CGameImageLink::m_ButtonResource.m_FrameShadow,CGameImageLink::m_ButtonResource.m_FrameColor,DrawRect);
		pDC->SetTextColor(CGameImageLink::m_ButtonResource.m_SelectTextColor);
	}
	else
	{
		DrawFace(pDC,CGameImageLink::m_ButtonResource.m_NormalColorTop,CGameImageLink::m_ButtonResource.m_NormalColorBottom,DrawRect,DrawRect);
		DrawFrame(pDC,CGameImageLink::m_ButtonResource.m_FrameHeight,CGameImageLink::m_ButtonResource.m_FrameShadow,CGameImageLink::m_ButtonResource.m_FrameColor,DrawRect);
		pDC->SetTextColor(CGameImageLink::m_ButtonResource.m_NormalTextColor);
	}

	if (m_bIsMouseMove)
	{
		CRect Rect(DrawRect);
		Rect.DeflateRect(2,2,2,2);
		COLORREF rcActiveColor=CGameImageLink::m_ButtonResource.m_ActiveColor;
		DrawFace(pDC,RGB(GetRValue(rcActiveColor),GetGValue(rcActiveColor)+100,GetBValue(rcActiveColor)+100),
				rcActiveColor, DrawRect, DrawRect);
		DrawFace(pDC,CGameImageLink::m_ButtonResource.m_NormalColorTop,CGameImageLink::m_ButtonResource.m_NormalColorBottom,Rect,DrawRect);

		CBrush NullBrush;
		NullBrush.CreateStockObject(NULL_BRUSH);
		CBrush * pOldBrush=pDC->SelectObject(&NullBrush);
		
		CPen Pen;
		Pen.CreatePen(PS_SOLID,1,CGameImageLink::m_ButtonResource.m_FrameColor);
		CPen * pOldPen=pDC->SelectObject(&Pen);
		DrawRect.InflateRect(1,1,1,1);
		pDC->RoundRect(DrawRect,CPoint(3, 3));
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
		pDC->SetTextColor(CGameImageLink::m_ButtonResource.m_ActiveTextColor);
		Pen.DeleteObject();
	}
	pDC->DrawText(strText,strText.GetLength(),&lpDrawItemStruct->rcItem,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
}

//�滭����
void CNormalBitmapButton::DrawFace(CDC * pDC, COLORREF crTop, COLORREF crBottom, CRect & rc, CRect & ButtonRect)
{
	int ColR=GetRValue(crTop);
	int ColG = GetGValue(crTop);
	int ColB =GetBValue(crTop);
	int R=(GetRValue(crTop)-GetRValue(crBottom))/(ButtonRect.bottom-ButtonRect.top);
	int G=(GetGValue(crTop)-GetGValue(crBottom))/(ButtonRect.bottom-ButtonRect.top);
	int B=(GetBValue(crTop)-GetBValue(crBottom))/(ButtonRect.bottom-ButtonRect.top);

	CPen Pen, * pOldPen=NULL;
	for (int i=0;i<rc.Height();i++)
	{
		ColR-=R;
		ColG-=G;
		ColB-=B;
		Pen.CreatePen(PS_SOLID,1,RGB(ColR,ColG,ColB));
		pOldPen=pDC->SelectObject(&Pen);
		pDC->MoveTo(rc.left,rc.top+i);
		pDC->LineTo(rc.right,rc.top+i);
		pDC->SelectObject(pOldPen);
		Pen.DeleteObject();
	}
	return;
}

//�滭���
void CNormalBitmapButton::DrawFrame(CDC * pDC, COLORREF HeightLight, COLORREF ShadowLight, COLORREF FrameColor, CRect & rc)
{
	CBrush NullBrush;
	NullBrush.CreateStockObject(NULL_BRUSH);
	CBrush * pOldBrush=pDC->SelectObject(&NullBrush);

	CPen Pen;
	Pen.CreatePen(PS_SOLID,1,FrameColor);
	CPen * pOldPen=pDC->SelectObject(&Pen);
	
	pDC->RoundRect(rc,CPoint(3, 3));
	rc.DeflateRect(1,1,1,1); 
	pDC->Draw3dRect(rc,HeightLight,ShadowLight);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	Pen.DeleteObject();
}

/*******************************************************************************************************/

//���캯��
CNormalBitmapButtonEx::CNormalBitmapButtonEx()
{
	m_hIcon=NULL;
	m_bExpand=false;
	m_bSetColor=false;
	m_crTextColor=RGB(255,255,255);
	m_wuiHandle=0;
	m_uControlID=-1;

	m_resFont = new CFont();
	m_resFont->CreateFont(-12,0,0,0,400,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
	
	 // PengJiLin, 2010-9-27
    m_bUseTextInfo = false;
    m_strTextInfo = "";
	
}

//��������
CNormalBitmapButtonEx::~CNormalBitmapButtonEx()
{
	if (m_hIcon!=NULL) 
	{
		DestroyIcon(m_hIcon);
		m_hIcon=NULL;
	}
	if (NULL != m_resFont)
	{
		m_resFont->DeleteObject();
		delete m_resFont;
		m_resFont = NULL;
	}
	return;
}

void CNormalBitmapButtonEx::SetWuiParameter(DWORD wuiHandle,UINT uControlID)
{
	m_wuiHandle=wuiHandle;
	m_uControlID=uControlID;
}

//����λͼ
bool CNormalBitmapButtonEx::LoadButtonBitmap(HINSTANCE hInstance, UINT uBitmapID, bool bExpand)
{
	if (uBitmapID!=0)
	{
		m_bExpand=bExpand;
		m_ButtonBitmap.SetLoadInfo(hInstance,uBitmapID,CGameImageLink::m_bAutoLock);
		if (m_bExpand==false) FixButtonSize();
		if (GetSafeHwnd()!=NULL) Invalidate(FALSE);
		return true;
	}
	return false;
}

//����λͼ
bool CNormalBitmapButtonEx::LoadButtonBitmap(TCHAR * szFileName, bool bExpand)
{
	if (szFileName!=NULL)
	{
		m_bExpand=bExpand;
		m_ButtonBitmap.SetLoadInfo(szFileName,CGameImageLink::m_bAutoLock);
		if (m_bExpand==false) FixButtonSize();
		if (GetSafeHwnd()!=NULL) Invalidate(FALSE);
		return true;
	}
	return false;
}

//����λ��
bool CNormalBitmapButtonEx::FixButtonSize()
{
	if ((m_ButtonBitmap.IsNull()==false)&&(GetSafeHwnd()!=NULL))
	{
		HRGN hRgn=AFCBmpToRgn(m_ButtonBitmap,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
		{
			DWORD style = GetWindowLong(m_hWnd, GWL_STYLE);
			SetWindowLong(m_hWnd, GWL_STYLE, style|WS_CLIPSIBLINGS);
			SetWindowRgn(hRgn,true);
		}
		CGameImageHelper ImageHandle(&m_ButtonBitmap);
		SetWindowPos(NULL,0,0,ImageHandle.GetWidth()/4,ImageHandle.GetHeight(),SWP_NOMOVE);
		return true;
	}
	return false;
}

//����滭����
void CNormalBitmapButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//�������
	CRect ClientRect;
	GetClientRect(&ClientRect);
	bool bDisable=false,bButtonDown=false;
	CDC * pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	
	int buttonStatus=0;
	//�滭������
	if(m_wuiHandle && m_uControlID!=-1)
	{
		if (lpDrawItemStruct->itemState&ODS_DISABLED) //ʧЧ��
		{
			bDisable=true;
			buttonStatus=3;
		}
		else if (lpDrawItemStruct->itemState&ODS_SELECTED) ////ѡ���ˣ�������
		{
			bButtonDown=true;
			buttonStatus=2;
		}
		else if (m_bIsMouseMove==true) buttonStatus=1;///////��꾭����
		else if (lpDrawItemStruct->itemState&ODS_FOCUS) buttonStatus=2;///��꼤���ˣ�������
		//wuiSetButtonStatus(m_wuiHandle,m_uControlID,buttonStatus);
		//wuiPaintControl(m_hWnd,pDC,m_wuiHandle,m_uControlID);
	}	
	else if (m_ButtonBitmap.IsNull()==false)//����Ϊ������ǰ�Ĳ���
	{
		CGameImageHelper ImageHandle(&m_ButtonBitmap);
		int iPartWidth=ImageHandle.GetWidth()/4,iDrawPos=0;
		if (lpDrawItemStruct->itemState&ODS_DISABLED) //ʧЧ��
		{
			bDisable=true;
			iDrawPos=iPartWidth*3;
		}
		else if (lpDrawItemStruct->itemState&ODS_SELECTED) ////ѡ���ˣ�������
		{
			bButtonDown=true;
			iDrawPos=iPartWidth*2;
		}
		else if (m_bIsMouseMove==true) iDrawPos=iPartWidth*1;///////��꾭����
		else if (lpDrawItemStruct->itemState&ODS_FOCUS) iDrawPos=iPartWidth*2;///��꼤���ˣ�������
		else iDrawPos=0;
		if (m_bExpand==false) ImageHandle.BitBlt(pDC->GetSafeHdc(),ClientRect,CPoint(iDrawPos,0));
		else ImageHandle.StretchBlt(pDC->GetSafeHdc(),ClientRect,CRect(iDrawPos,0,iDrawPos+iPartWidth,ImageHandle.GetHeight()));
	}
	else if (CGameImageLink::m_ButtonResource.m_BackPic.IsNull()==false)
	{
		CGameImageHelper ImageHandle(&CGameImageLink::m_ButtonResource.m_BackPic);
		int iHeight=ImageHandle.GetHeight();
		int iPartWidth=ImageHandle.GetWidth()/5,iDrawPos=0;
		if (lpDrawItemStruct->itemState&ODS_DISABLED) //ʧЧ��
		{
			bDisable=true;
			iDrawPos=iPartWidth*3;
		}
		else if (lpDrawItemStruct->itemState&ODS_SELECTED) ////ѡ���ˣ�������
		{
			bButtonDown=true;
			iDrawPos=iPartWidth*2;
		}
		else if (m_bIsMouseMove==true) iDrawPos=iPartWidth*1;//////��꾭����
		else if (lpDrawItemStruct->itemState&ODS_FOCUS) iDrawPos=iPartWidth*2;///��꼤���ˣ�������
		else iDrawPos=0;
		ImageHandle.StretchBlt(pDC->GetSafeHdc(),ClientRect,CRect(iDrawPos,0,iPartWidth+iDrawPos,iHeight));
	}
	
	//�滭ͼ��
	if (bDisable==true) pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	else if (m_bSetColor==true) pDC->SetTextColor(m_crTextColor);
	if (bButtonDown==true) ClientRect.top+=2;
	if (m_hIcon!=NULL)
	{
		DrawIconEx(pDC->GetSafeHdc(),ClientRect.left+6,ClientRect.top+(ClientRect.Height()-16)/2,m_hIcon,16,16,0,NULL,DI_NORMAL);
		ClientRect.left+=23;
	}

	//�滭����
	CString strText;
	GetWindowText(strText);
	pDC->SetBkMode(TRANSPARENT);
	CFont * oldFont = pDC->GetCurrentFont();
	pDC->SelectObject(m_resFont);
	//pDC->SelectObject(&CGameImageLink::m_HFont);
	pDC->DrawText(strText,ClientRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

    // PengJiLin, 2010-9-27, ��ʾ�������õ��ַ�
    if(true == m_bUseTextInfo)
    {
        pDC->DrawText(m_strTextInfo, ClientRect, DT_RIGHT | DT_BOTTOM | DT_SINGLELINE);
    }

	pDC->SelectObject(oldFont);
	return;
}

//����ͼ��
bool CNormalBitmapButtonEx::SetButtonIcon(HICON hIcon)
{
	if (m_hIcon!=NULL) 
	{
		DestroyIcon(m_hIcon);
		m_hIcon=NULL;
	}
	m_hIcon=hIcon;
	return true;
}

//������ɫ
bool CNormalBitmapButtonEx::SetTextColor(COLORREF crTextColor, bool bSetColor)
{
	m_bSetColor=bSetColor;
	m_crTextColor=crTextColor;
	Invalidate(FALSE);
	return true;
}

// PengJiLin, 2010-9-27, �����Ƿ���ʾ������ַ�
void CNormalBitmapButtonEx::EnableToShowText(CString& strInfo, bool bUseText)
{
    m_strTextInfo = strInfo;
    m_bUseTextInfo = bUseText;
}
void CNormalBitmapButtonEx::EnableToShowText(TCHAR* strInfo, bool bUseText)
{
    m_strTextInfo = strInfo;
    m_bUseTextInfo = bUseText;
}
void CNormalBitmapButtonEx::EnableToShowText(int iInfo, bool bUseText)
{
    if(iInfo < 0) iInfo = 0;
    m_strTextInfo.Format("%d", iInfo);
    m_bUseTextInfo = bUseText;
}

//����λͼ
bool CNormalBitmapButtonEx::SetStaticBitmap(HINSTANCE hInstance, UINT uBitmapID)
{
	if (uBitmapID!=0)
	{
		CGameImageLink::m_ButtonResource.m_BackPic.SetLoadInfo(hInstance,uBitmapID,CGameImageLink::m_bAutoLock);
		return true;
	}
	return false;
}

//����λͼ
bool CNormalBitmapButtonEx::SetStaticBitmap(TCHAR * szFileName)
{
	if (szFileName!=NULL)
	{
		CGameImageLink::m_ButtonResource.m_BackPic.SetLoadInfo(szFileName,CGameImageLink::m_bAutoLock);
		return true;
	}
	return false;
}

/*******************************************************************************************************/

BOOL CTrueBitmapButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return true;
	return CButton::OnEraseBkgnd(pDC);
}

void CTrueBitmapButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	//��������
	if (CGameImageLink::m_ButtonResource.m_bSound==true)
	{
		AFCPlaySound(GetModuleHandle(FACE_DLL_NAME),TEXT("BUTTON_UP"));
	}
	CButton::OnLButtonUp(nFlags, point);
}
