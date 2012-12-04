#include "../stdafx.h"
#include "AFCWindow.h"
#include "AFCResource.h"
#include "AFCFunction.h"

//���ֿռ�
//using namespace AFC;
//using namespace AFC::AFCWindow;
//using namespace AFC::AFCResource;

//�궨��
#define	TOOLTIP_ID					1								//��ʾ���� ID

/*******************************************************************************************************/

BEGIN_MESSAGE_MAP(CWorldEdit, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CAFCSplitWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CNewMapEx, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CBitmapRgnWindow, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CControlMessage, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CHyperLink, CStatic)
    ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_SETCURSOR()
    ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/*******************************************************************************************************/

//���캯��
CWorldEdit::CWorldEdit(void)
{
	m_hIcon=NULL;
}

//��������
CWorldEdit::~CWorldEdit(void)
{
	if (m_hIcon!=NULL) DestroyIcon(m_hIcon);
}

//����ͼ��
bool CWorldEdit::SetTitleIcon(HICON hIcon)
{
	if (m_hIcon!=NULL) DestroyIcon(m_hIcon);
	m_hIcon=hIcon;
	Invalidate(FALSE);
	return true;
}

//�ػ�����
void CWorldEdit::OnPaint()
{
	CPaintDC dc(this);

	//���õ�ͼ
	CGameImage * pBackImage=NULL;
	if (m_TitlePic.IsNull()==false) pBackImage=&m_TitlePic;
	else pBackImage=&CGameImageLink::m_TitleResource.m_BackPic;

	//�滭����
	CRect ClientRect;
	GetClientRect(&ClientRect);
	CGameImageHelper ImageHandle(pBackImage);
	ImageHandle.StretchBlt(dc.GetSafeHdc(),ClientRect,CRect(0,0,ImageHandle.GetWidth(),ImageHandle.GetHeight()));
	dc.Draw3dRect(&ClientRect,RGB(0,0,200),RGB(0,0,250));

	//�滭ͼ��
	if ((ClientRect.Width()>ICON_SIZE)&&(m_hIcon!=NULL))
	{
		DrawIconEx(dc.GetSafeHdc(),ClientRect.left+5,ClientRect.top+(ClientRect.Height()-ICON_SIZE)/2,m_hIcon,ICON_SIZE,ICON_SIZE,0,NULL,DI_NORMAL);
		ClientRect.left+=ICON_SIZE+5;
	}

	//�滭����
	ClientRect.left+=5;
	CString strTitle;
	GetWindowText(strTitle);
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(&CGameImageLink::m_HFont);
	dc.SetTextColor(CGameImageLink::m_TitleResource.m_crTextColor);
	DWORD dwStyte=GetStyle();
	if (dwStyte&SS_CENTER) dc.DrawText(strTitle,&ClientRect,DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	else if (dwStyte&SS_RIGHT) dc.DrawText(strTitle,&ClientRect,DT_VCENTER|DT_RIGHT|DT_SINGLELINE);
	else dc.DrawText(strTitle,&ClientRect,DT_VCENTER|DT_LEFT|DT_SINGLELINE);
}

/*******************************************************************************************************/

//��������
bool CNewMapEx::CreateTipWnd(CWnd * pParentWnd, UINT uWndID)
{
	//��������
	m_FixSize.SetSize(0,0);
	if (Create(NULL,NULL,WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,0,0),pParentWnd,uWndID,NULL)==FALSE) return false;
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	BringWindowToTop();
	return true;
}

//���ô�����Ϣ
CSize CNewMapEx::SetTipText(const TCHAR * szTipText, bool bFixSize)
{
	//��������
	m_strMessage.Empty();
	if (szTipText!=NULL) m_strMessage=szTipText;

	//��ʾ�ַ�
	if (m_strMessage.GetLength()>0)
	{
		//�����ַ�����
		CDC * pDC=GetDC();
		pDC->SelectObject(&CGameImageLink::m_HFont);
		m_FixSize=pDC->GetTextExtent(m_strMessage);
		m_FixSize.cx+=6;
		m_FixSize.cy+=6;
		ReleaseDC(pDC);

		//�ƶ�����
		if (bFixSize)
		{
			CRect ClientRect;
			GetClientRect(&ClientRect);
			MapWindowPoints(GetParent(),&ClientRect);
			MoveWindow(ClientRect.left,ClientRect.top,m_FixSize.cx,m_FixSize.cy);
			ShowWindow(SW_SHOW);
			Invalidate(FALSE);
		}
		return m_FixSize;
	}
	
	//���ش��� 
	m_FixSize.SetSize(0,0);
	ShowWindow(SW_HIDE);
	return m_FixSize;
}

//�ػ�����
void CNewMapEx::OnPaint()
{
	CPaintDC dc(this);

	//�滭����
	CRect ClientRect;
	GetClientRect(&ClientRect);
	dc.FillSolidRect(&ClientRect,RGB(255,255,225));
	dc.Draw3dRect(&ClientRect,RGB(0,0,0),RGB(0,0,0));

	//�滭����
	ClientRect.top+=2;
	ClientRect.left+=2;
	ClientRect.right-=2;
	ClientRect.bottom-=2;
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,0));
	dc.SelectObject(&CGameImageLink::m_HFont);
	dc.DrawText(m_strMessage,&ClientRect,DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER|DT_SINGLELINE);

	return;
}

/*******************************************************************************************************/

//�ػ�����
void CAFCSplitWnd::OnPaint()
{
	CPaintDC dc(this);

	CRect ClipRect;
	dc.GetClipBox(&ClipRect);
	dc.FillSolidRect(&ClipRect,RGB(125,125,125));
}

/*******************************************************************************************************/

//���캯��
CControlMessage::CControlMessage(void)
{
	m_iMaxPos=0;
	m_iLessPos=0;
	m_bHorSplit=true;
	m_Color=RGB(0,0,0);		

}

//��������
CControlMessage::~CControlMessage(void)
{
	if (m_SplitWnd.GetSafeHwnd()) m_SplitWnd.DestroyWindow();
}

//λ����Ϣ
void CControlMessage::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
}

//��ʼ�������
bool CControlMessage::InitSplitBar(UINT uLessPos, UINT uMaxPos, bool bHorSplit)
{
	m_iMaxPos=uMaxPos;
	m_iLessPos=uLessPos;
	m_bHorSplit=bHorSplit;
	return true;
}

//�ػ�����
void CControlMessage::OnPaint()
{
	CPaintDC dc(this);

	//�滭�߿�
	CRect ClientRect;
	GetClientRect(&ClientRect);
	CGameImageHelper imagehelper1(&m_bkimagel);//��
	CGameImageHelper imagehelper2(&m_bkimager);//��
	CGameImageHelper imagehelper(&m_bkimage);
	for(int i=0;i<ClientRect.Height()-(m_bHorSplit?0:imagehelper2.GetHeight());i+=imagehelper.GetHeight())
	{
		for(int j=0;j<ClientRect.Width()-(m_bHorSplit?imagehelper1.GetWidth():0);j+=imagehelper.GetWidth())
			imagehelper.BitBlt(dc.GetSafeHdc(),j,i);
	}
	imagehelper1.BitBlt(dc.GetSafeHdc(),0,0);	
	imagehelper2.BitBlt(dc.GetSafeHdc(),ClientRect.right-imagehelper2.GetWidth(),ClientRect.bottom-imagehelper2.GetHeight());
}

void CControlMessage::SetbkImage(TCHAR imagepath1[],TCHAR imagepath2[],TCHAR imagepath3[] )
{
	m_bkimagel.SetLoadInfo(imagepath1,CGameImageLink::m_bAutoLock);
	m_bkimage.SetLoadInfo(imagepath2,CGameImageLink::m_bAutoLock);
	m_bkimager.SetLoadInfo(imagepath3,CGameImageLink::m_bAutoLock);
}

//����ƶ���Ϣ
void CControlMessage::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_SplitWnd.GetSafeHwnd()!=NULL)
	{
		CRect ClientRect;
		GetClientRect(&ClientRect);
		ClientRect.right=__max(ClientRect.right,3);
		MapWindowPoints(GetParent(),&ClientRect);
		MapWindowPoints(GetParent(),&point,1);
		CPoint PointCheck(point);
		if (m_bHorSplit==false)
		{
			PointCheck.x=__min(m_iMaxPos,__max(m_iLessPos,PointCheck.x));
			ClientRect.OffsetRect((PointCheck.x-ClientRect.Width()/2-ClientRect.left),0);
			if (PointCheck.x!=point.x) SetCursor(CGameImageLink::m_SplitResource.m_hDisableCur);
			else SetCursor(CGameImageLink::m_SplitResource.m_hHorCur);
		}
		else
		{
			PointCheck.y=__min(m_iMaxPos,__max(m_iLessPos,PointCheck.y));
			ClientRect.OffsetRect(0,(PointCheck.y-ClientRect.Height()/2-ClientRect.top));
			if (PointCheck.y!=point.y) SetCursor(CGameImageLink::m_SplitResource.m_hDisableCur);
			else SetCursor(CGameImageLink::m_SplitResource.m_hVorCur);
		}
		m_SplitWnd.MoveWindow(&ClientRect);
		m_SplitWnd.Invalidate(FALSE);
	}

	return;
}

//��갴����Ϣ
void CControlMessage::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_SplitWnd.GetSafeHwnd()==NULL)
	{
		CRect ClientRect;
		GetClientRect(&ClientRect);
		ClientRect.right=__max(ClientRect.right,3);
		MapWindowPoints(GetParent(),&ClientRect);
		HINSTANCE hLastHandle=AfxGetResourceHandle();
		AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
		if (m_SplitWnd.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,ClientRect,GetParent(),10))	
		{
			m_SplitWnd.BringWindowToTop();
			SetFocus();
			SetCapture();
		}
		AfxSetResourceHandle(hLastHandle);
	}

	return;
}

//��갴����Ϣ
void CControlMessage::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_SplitWnd.GetSafeHwnd()!=NULL)
	{
		//��ʾ����
		CRect ViewRect,SplitRect;
		GetWindowRect(&ViewRect);
		m_SplitWnd.GetWindowRect(&SplitRect);
		m_SplitWnd.DestroyWindow();
		ShowWindow(SW_SHOW);
		ReleaseCapture();

		//������Ϣ
		if (ViewRect!=SplitRect)
		{
			MapWindowPoints(GetParent(),&point,1);
			if (m_bHorSplit==false) point.x=__min(m_iMaxPos,__max(m_iLessPos,point.x));
			else point.y=__min(m_iMaxPos,__max(m_iLessPos,point.y));
			GetParent()->SendMessage(WM_SPLIT_WND,GetWindowLong(GetSafeHwnd(),GWL_ID),MAKELONG(point.x,point.y));
		}
	}
	return;
}

//���ù����Ϣ
BOOL CControlMessage::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	//��ȡ��ǰ��
	POINT Point;
	::GetCursorPos(&Point);
	ScreenToClient(&Point);
	MapWindowPoints(GetParent(),&Point,1);

	//������
	if (IsWindowEnabled())
	{
		if (m_bHorSplit==false) SetCursor(CGameImageLink::m_SplitResource.m_hHorCur);
		else SetCursor(CGameImageLink::m_SplitResource.m_hVorCur);
		return TRUE;
	}
	SetCursor(CGameImageLink::m_SplitResource.m_hDisableCur);
	return TRUE;
}

//ʧȥ����
void CControlMessage::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	//������
	if (m_SplitWnd.GetSafeHwnd()!=NULL)
	{
		m_SplitWnd.DestroyWindow();
		ShowWindow(SW_SHOW);
		ReleaseCapture();
	}
}

/*******************************************************************************************************/

//���캯��
CHyperLink::CHyperLink(void)
{
    m_hLinkCursor=NULL;
    m_bVisited=false;
    m_bUnderline =true;
    m_bAdjustToFit=true;
    m_bOverControl=false;
    m_crLinkColour=RGB(0,0,238);
    m_crVisitedColour=RGB(85,26,139);
    m_crHoverColour=::GetSysColor(COLOR_HIGHLIGHT);
	m_strURL="http://www.bzw.cn";
	m_ToolTip.Create(this);
}

//��������
CHyperLink::~CHyperLink(void)
{
    m_Font.DeleteObject();
}

//���ó�������
void CHyperLink::SetURL(CString strURL)
{
    m_strURL=strURL;
    if (::IsWindow(GetSafeHwnd())) 
	{
        PositionWindow();
		m_ToolTip.AddTool(this,m_strURL);
    }
	return;
}

//��ȡ��������
CString CHyperLink::GetURL() const
{
	return m_strURL;
}

//������ɫ
void CHyperLink::SetColours(COLORREF crLinkColour, COLORREF crVisitedColour, COLORREF crHoverColour)
{
    m_crLinkColour=crLinkColour; 
    m_crVisitedColour=crVisitedColour;
	if (crHoverColour==-1) m_crHoverColour=::GetSysColor(COLOR_HIGHLIGHT);
	else crHoverColour=crHoverColour;
    if (::IsWindow(m_hWnd)) Invalidate();
	return;
}

//��ȡ��ɫ
COLORREF CHyperLink::GetLinkColour() const
{
    return m_crLinkColour; 
}

//��ȡ��ɫ
COLORREF CHyperLink::GetVisitedColour() const
{
    return m_crVisitedColour; 
}

//��ȡ��ɫ
COLORREF CHyperLink::GetHoverColour() const
{
    return m_crHoverColour;
}

//�����Ƿ����		
void CHyperLink::SetVisited(bool bVisited)
{
    m_bVisited=bVisited; 
    if (::IsWindow(GetSafeHwnd())) Invalidate(); 
	return;
}

//��ȡ�Ƿ����		
bool CHyperLink::GetVisited() const
{
    return m_bVisited; 
}

//���ù��
void CHyperLink::SetLinkCursor(HCURSOR hCursor)
{
    m_hLinkCursor=hCursor;
    if (m_hLinkCursor==NULL) SetDefaultCursor();
	return;
}

//��ȡ���
HCURSOR CHyperLink::GetLinkCursor() const
{
    return m_hLinkCursor;
}

//����Ĭ�Ϲ��
void CHyperLink::SetDefaultCursor()
{
	return;
}

//�����Ƿ����»���
void CHyperLink::SetUnderline(bool bUnderline)
{
    m_bUnderline=bUnderline;
    if (::IsWindow(GetSafeHwnd()))
    {
        LOGFONT lf;
        GetFont()->GetLogFont(&lf);
        lf.lfUnderline=m_bUnderline;
        m_Font.DeleteObject();
        m_Font.CreateFontIndirect(&lf);
        SetFont(&m_Font);
        Invalidate(); 
    }
	return;
}

//��ȡ�Ƿ����»���
bool CHyperLink::GetUnderline() const
{
    return m_bUnderline; 
}

//�����Ƿ��Զ����ڴ�С
void CHyperLink::SetAutoSize(bool bAutoSize)
{
    m_bAdjustToFit=bAutoSize;
    if (::IsWindow(GetSafeHwnd())) PositionWindow();
	return;
}

//��ȡ�Ƿ��Զ����ڴ�С
bool CHyperLink::GetAutoSize() const
{
	return m_bAdjustToFit;
}

//ת����ҳ
HINSTANCE CHyperLink::GotoURL(const TCHAR * szURL, int iShowCmd)
{
	return ShellExecute(NULL,"open",szURL,NULL,NULL,SW_MAXIMIZE);
}

/*
 void CHyperLink::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
 {
	 SetBkMode(lpDrawItemStruct->hDC,TRANSPARENT);
	 TCHAR szCaption[100];
	 GetWindowText(szCaption,99);
	 DrawText(lpDrawItemStruct->hDC,szCaption,99,&(lpDrawItemStruct->rcItem),DT_VCENTER|DT_CENTER);
 }*/

 void CHyperLink::OnPaint()
 {
	 CPaintDC dc(this);
	 dc.SetBkMode(TRANSPARENT);
	 TCHAR szCaption[100];
	 GetWindowText(szCaption,99);
	 CRect rect;
	 GetWindowRect(&rect);
	 dc.DrawText(szCaption,99,&rect,DT_VCENTER|DT_CENTER);
 }

//�������
void CHyperLink::ReportError(int iError)
{
    CString strBuffer;
    switch (iError) {
        case 0: 
			{
				strBuffer = "OS out\nof memory or resources."; 
				break;
			}
        case SE_ERR_PNF:
			{
				strBuffer = "path was not found."; 
				break;
			}
        case SE_ERR_FNF:
			{
				strBuffer = "file was not found."; 
				break;
			}
        case ERROR_BAD_FORMAT:        
			{
				strBuffer = "The .EXE file error"; 
				break;
			}
        case SE_ERR_ACCESSDENIED:     
			{
				strBuffer = "OS denied\naccess to the specified file."; 
				break;
			}
        case SE_ERR_ASSOCINCOMPLETE:  
			{
				strBuffer = "The filename association is\nincomplete or invalid."; 
				break;
			}
        case SE_ERR_DDEBUSY:         
			{
				strBuffer = "The DDE transaction could not\nbe completed because other DDE transactions\nwere being processed."; break;
			}
        case SE_ERR_DDEFAIL:         
			{
				strBuffer = "The DDE transaction failed."; 
				break;
			}
        case SE_ERR_DDETIMEOUT:      
			{
				strBuffer = "The DDE transaction could not\nbe completed because the request timed out."; 
				break;
			}
        case SE_ERR_DLLNOTFOUND:     
			{
				strBuffer = "DLL was not found."; 
				break;
			}
        case SE_ERR_NOASSOC:          
			{
				strBuffer = "There is no application associated\nwith the given filename extension."; 
				break;
			}
        case SE_ERR_OOM:             
			{
				strBuffer = "not enough memory"; 
				break;
			}
        case SE_ERR_SHARE:            
			{
				strBuffer = "A sharing violation occurred. ";
				break;
			}
        default:      
			{
				strBuffer.Format("Unknown Error (%d) occurred.", iError); 
				break;
			}
    }
    strBuffer=TEXT("Unable to open hyperlink:\n\n")+strBuffer;
    AFCMessageBox(strBuffer);

	return;
}

//��ȡע�����ֵ
long int CHyperLink::GetRegKey(HKEY hRegKey, TCHAR * szSubKey, TCHAR * szRetData)
{
	return 0;
}

//�ƶ�����
void CHyperLink::PositionWindow()
{
	return;
}

//�������໯���� 
void CHyperLink::PreSubclassWindow()
{
	return;
}

//��Ϣ���ͺ���
BOOL CHyperLink::PreTranslateMessage(MSG * pMsg)
{
	
    m_ToolTip.RelayEvent(pMsg);
    return CStatic::PreTranslateMessage(pMsg);
}

//������ɫ��Ϣ
HBRUSH CHyperLink::CtlColor(CDC * pDC, UINT nCtlColor)
{
    if (m_bOverControl) pDC->SetTextColor(m_crHoverColour);
    else if (m_bVisited) pDC->SetTextColor(m_crVisitedColour);
    else pDC->SetTextColor(m_crLinkColour);
    pDC->SetBkMode(TRANSPARENT);
    return (HBRUSH)GetStockObject(NULL_BRUSH);
}

//���ù����Ϣ
BOOL CHyperLink::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
    if (m_hLinkCursor)
    {
        ::SetCursor(m_hLinkCursor);
        return TRUE;
    }
    return FALSE;
}

//����ƶ���Ϣ
void CHyperLink::OnMouseMove(UINT nFlags, CPoint point)
{
    CStatic::OnMouseMove(nFlags, point);

	if (m_bOverControl)
    {
        CRect Rect;
        GetClientRect(Rect);
        if (Rect.PtInRect(point)==FALSE)
        {
            m_bOverControl=false;
            ReleaseCapture();
            RedrawWindow();
            return;
        }
    }
    else
    {
        m_bOverControl=true;
        RedrawWindow();
        SetCapture();
    }
	return;
}

//������Ϣ
void CHyperLink::OnClicked()
{
	GotoURL(m_strURL,SW_SHOW);
    //DWORD iResult=(DWORD)GotoURL(m_strURL,SW_SHOW);
    /*m_bVisited=(iResult>HINSTANCE_ERROR);
    if (m_bVisited==false)
	{
        MessageBeep(MB_ICONEXCLAMATION);
        ReportError((int)iResult);
		return;
    } 
	
	SetVisited(true);*/
	return;
}

/*******************************************************************************************************/

//���캯��
CBitmapRgnWindow::CBitmapRgnWindow()
{
	return;
}

//��������
CBitmapRgnWindow::~CBitmapRgnWindow()
{
	m_WindowRgn.DeleteObject();
	return;
}

//������Ϣ
int CBitmapRgnWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) return -1;

	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return 0;
}

//�ػ�����
void CBitmapRgnWindow::OnPaint()
{
	CPaintDC dc(this);

	//�滭��ͼ
	if (m_BackBitmap.IsNull()==false)
	{
		CRect ClientRect;
		GetClientRect(&ClientRect);
		CGameImageHelper ImageHandle(&m_BackBitmap);
		ImageHandle.BitBlt(dc.GetSafeHdc(),0,0);
	}

	return;
}

//��ȡ����
bool CBitmapRgnWindow::SafeGetClientRect(UINT uID, CRect & ClientRect)
{
	CWnd * pControlWnd=GetDlgItem(uID);
	if ((pControlWnd!=NULL)&&(pControlWnd->GetSafeHwnd())) 
	{
		pControlWnd->GetClientRect(&ClientRect);
		return true;
	}
	ClientRect.SetRectEmpty();
	return false;
}

//��ȡ����
bool CBitmapRgnWindow::SafeGetClientRect(CWnd * pControlWnd, CRect & ClientRect)
{
	if ((pControlWnd!=NULL)&&(pControlWnd->GetSafeHwnd())) 
	{
		pControlWnd->GetClientRect(&ClientRect);
		return true;
	}
	ClientRect.SetRectEmpty();
	return false;
}

//�ƶ�����
bool CBitmapRgnWindow::SafeMoveWindow(UINT uID, int x, int y, int iWidth, int iHeight)
{
	CWnd * pControlWnd=GetDlgItem(uID);
	if ((pControlWnd!=NULL)&&(pControlWnd->GetSafeHwnd())) 
	{
		if ((iWidth<=0)||(iHeight<=0)) iWidth=0,iHeight=0;
		pControlWnd->MoveWindow(x,y,iWidth,iHeight);
		return true;
	}
	return false;
}

//�ƶ�����
bool CBitmapRgnWindow::SafeMoveWindow(CWnd * pControlWnd, int x, int y, int iWidth, int iHeight)
{
	if ((pControlWnd!=NULL)&&(pControlWnd->GetSafeHwnd())) 
	{
		if ((iWidth<=0)||(iHeight<=0)) iWidth=0,iHeight=0;
		pControlWnd->MoveWindow(x,y,iWidth,iHeight);
		return true;
	}
	return false;
}

//����λͼ
bool CBitmapRgnWindow::LoadDialogBitmap(HINSTANCE hInstance, UINT uBitmapID, COLORREF crTransparentColor, COLORREF crTolerance)
{
	if (uBitmapID!=0)
	{
		//��������
		m_BackBitmap.SetLoadInfo(hInstance,uBitmapID,CGameImageLink::m_bAutoLock);
		CGameImageHelper ImageHandle(&m_BackBitmap);
		HRGN hRgn=AFCBmpToRgn(ImageHandle,crTransparentColor,crTolerance);
		if (hRgn!=NULL)
		{
			SetWindowPos(NULL,0,0,ImageHandle.GetWidth(),ImageHandle.GetHeight(),SWP_NOMOVE);
			SetWindowRgn(hRgn,TRUE);
			m_WindowRgn.DeleteObject();
			m_WindowRgn.Attach(hRgn);
			return true;
		}
	}
	return false;
}

//����λͼ
bool CBitmapRgnWindow::LoadDialogBitmap(TCHAR * szFileName, COLORREF crTransparentColor, COLORREF crTolerance)
{
	if (szFileName!=NULL)
	{
		//��������
		m_BackBitmap.SetLoadInfo(szFileName,CGameImageLink::m_bAutoLock);
		CGameImageHelper ImageHandle(&m_BackBitmap);
		HRGN hRgn=AFCBmpToRgn(ImageHandle,crTransparentColor,crTolerance);
		if (hRgn!=NULL)
		{
			SetWindowPos(NULL,0,0,ImageHandle.GetWidth(),ImageHandle.GetHeight(),SWP_NOMOVE);
			SetWindowRgn(hRgn,TRUE);
			m_WindowRgn.DeleteObject();
			m_WindowRgn.Attach(hRgn);
			return true;
		}
	}
	return false;
}

//����Ҽ�������Ϣ
void CBitmapRgnWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags, point);

	//ģ�ⰴ����
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
}

/*******************************************************************************************************/
