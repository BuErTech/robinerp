#include "StdAfx.h"
#include "AFCTabCtrl.h"
#include "AFCFunction.h"
#include "Resource.h" 
#include "GameFrameDlg.h"
#include "..\\MainFrame\\GamePlaceDlg.h"

BEGIN_MESSAGE_MAP(CNewGameClass, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_LOADSKIN,OnLoadSkin)
	ON_MESSAGE(IDM_SEND_ROOMMSG,OnSendRoomMsg)
	ON_MESSAGE(IDM_HIDE_RIGHT,OnHideRight)
	ON_MESSAGE(IDM_OPEN_BANK,OpenBank)
	ON_MESSAGE(IDM_OPEN_IE,OpenIE)
	ON_MESSAGE(IDM_MESSAGE_APPEAL,OAppeal)
	ON_MESSAGE(IDM_CALL_GM,CallGM)
	ON_MESSAGE(IDM_LOCK_ACCOUNT, OnLockAccount)	//�������� zxj 2009-11-12
END_MESSAGE_MAP()

//���ֿռ�
//using namespace AFC;
//using namespace AFC::AFCResource;

//�궨��

#define TAB_HEIGHT				32													//TAB ���
#define TAB_POS_X				5													//TAB��ʼ��

/******************************************************************************************************/

//���캯��
CNewGoGamePower::CNewGoGamePower()
{
	m_pWnd=NULL;
	m_pRightWnd=NULL;
	m_hIcon=NULL;
	m_bAutoDelete=true;
	m_rcTitleRect.SetRectEmpty();

}

//��������
CNewGoGamePower::~CNewGoGamePower()
{
	DeletePage();
	return;
}

//ȡ�ÿ��
UINT CNewGoGamePower::GetAreaWidth(CDC * pDC)
{
	UINT uWidth=pDC->GetTextExtent(m_strTitle).cx;
	if (m_hIcon) uWidth+=25L;
	return uWidth+18L;
}

//ѡ����º�ͻ����л��滭����
void CNewGoGamePower::Draw(CDC * pDC, UINT nViewStyle, bool bActive)
{
	CRect Rect=m_rcTitleRect;
	//�滭������
	CGameImage tab;
	switch (nViewStyle)
	{
	case TS_UP:		//����
		{
			break;
		}
	case TS_DOWN:	//����
		{
			break;
		}
	case TS_LEFT:	//����
		{
			break;
		}
	case TS_RIGHT:	//����
		{
			break;
		}
	}

	//��������
	if (bActive) 
	{
		CGameImageHelper ImageHandle(&m_a);//������ͼƬ
		//ImageHandle.StretchBlt(pDC->GetSafeHdc(),Rect);
		ImageHandle.BitBlt(pDC->GetSafeHdc(),Rect,CPoint(0,0));
		pDC->SetTextColor(m_ca);
	}
	else
	{
		CGameImageHelper ImageHandle(&m_n);//������ͼƬ
		//ImageHandle.StretchBlt(pDC->GetSafeHdc(),Rect);
		ImageHandle.BitBlt(pDC->GetSafeHdc(),Rect,CPoint(0,0));
		pDC->SetTextColor(m_cn);
	}

	/*	//�滭ͼ��
	if ((Rect.Width()>ICON_SIZE)&&(m_hIcon!=NULL))
	{
	DrawIconEx(pDC->GetSafeHdc(),Rect.left,Rect.top+(Rect.Height()-ICON_SIZE)/2,m_hIcon,ICON_SIZE,ICON_SIZE,0,NULL,DI_NORMAL);
	Rect.left+=ICON_SIZE+2;
	}*/

	//�滭����
	///////////////////////////////
	///kylin 20090205 ���־�����ʾ
	//Rect.right = Rect.right - Rect.Width()/5;
	///////////////////////////////
	if (m_strTitle.GetLength()>0)
	{
		Rect.top+=8;
		pDC->DrawText(m_strTitle,Rect,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE);
	}

	return;
}

//ɾ��ҳ��
bool CNewGoGamePower::DeletePage()
{
	//ɾ������
	if ((m_pWnd!=NULL))
	{
		if (m_pWnd->GetSafeHwnd())
			m_pWnd->DestroyWindow();
		//if (m_bAutoDelete)		//by ZXD �˳�Ա����ʶ���Ƿ�����׷��䣬�������Ƿ�Ϊ�棬��Ҫɾ������
			delete m_pWnd;
		m_pWnd=NULL;
	}
	//�ұߴ���
	if ((m_pRightWnd!=NULL))
	{
		if (m_pRightWnd->GetSafeHwnd())
			m_pRightWnd->DestroyWindow();
		//if (m_bAutoDelete)		//by ZXD �˳�Ա����ʶ���Ƿ�����׷��䣬�������Ƿ�Ϊ�棬��Ҫɾ������
			delete m_pRightWnd;
		m_pRightWnd=NULL;
	}
	//������Դ 
	if (m_hIcon!=NULL)
	{
		DestroyIcon(m_hIcon);
		m_hIcon=NULL;
	}

	//�������
	m_strTitle.Empty();
	m_rcTitleRect.SetRectEmpty();

	m_n.Detach();
	m_a.Detach();

	return true;
}
//��ʾ���д���
void CNewGoGamePower::ShowGameWindow(int nShow)
{
	m_pWnd->SendMessage(WM_SHOWGAMEWINDOW,nShow,0);
	if(m_pRightWnd!=NULL)
		m_pRightWnd->SendMessage(WM_SHOWGAMEWINDOW,nShow,0);
}
void CNewGoGamePower::SetSkin(TCHAR  pathn[], TCHAR  patha[], COLORREF cn, COLORREF ca)
{
	m_n.SetLoadInfo(pathn,CGameImageLink::m_bAutoLock);
	m_a.SetLoadInfo(patha,CGameImageLink::m_bAutoLock);
	m_cn=cn;
	m_ca=ca;
}
/******************************************************************************************************/

//���캯��
CNewGameClass::CNewGameClass()
{
	m_CurWidth = 0;
	m_bInit = false;
	m_cursor=LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_HAND));
	m_bCopyRight=false;
	lstrcpy(m_szURL,"http://www.bzw.cn");
	m_uActivePage=MAX_PAGE;
	m_uViewStyte=TS_DOWN;
	LoadSkin();
	for (int i=0;i<MAX_PAGE;i++) m_uIndex[i]=MAX_PAGE;
	m_bIsLBOK = true;
	m_Rgn.CreateRectRgn(0,0,0,0);
}

//��������
CNewGameClass::~CNewGameClass()
{
	INT_PTR iCount=m_PageArray.GetSize();
	for (INT_PTR i=0;i<iCount;i++)
	{
		CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(i);
		if (pPageItem)
		{	
			pPageItem->DeletePage();
			delete pPageItem;
			pPageItem=NULL;
		}
	}
	m_PageArray.RemoveAll();
	m_Rgn.DeleteObject();
	if(this->m_hWnd)
		KillTimer(TIMER_AD);
}

void CNewGameClass::ShowAllWindow(int nShow)
{
	INT_PTR iCount=m_PageArray.GetSize();
	for (INT_PTR i=0;i<iCount;i++)
	{
		CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(i);
		if (pPageItem)
		{	
			pPageItem->ShowGameWindow(nShow);
		}
	}
}
//��������
bool CNewGameClass::CreateTabWnd(CWnd * pParentWnd, UINT uWndID)
{
	//��������
	if (Create(AfxRegisterWndClass(CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW,
		AfxGetApp()->LoadStandardCursor(IDC_ARROW),
		(HBRUSH)GetStockObject(NULL_BRUSH),NULL),
		NULL,WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
		CRect(0,0,0,0),pParentWnd,uWndID,NULL)==FALSE)
		return false;
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	//	m_copyright.Create(TEXT("test"),SS_SIMPLE|WS_CHILD|WS_VISIBLE|SS_NOTIFY,CRect(0,0,0,0),this,300);
	//	m_copyright.SetLinkCursor(LoadCursor(GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_HAND)));
	//	m_copyright.ShowWindow(0);

	LoadSkin();
	m_bInit = true;
	return true;
}
//���봰��
UINT CNewGameClass::AddTabPage(CWnd * pWnd,TCHAR * szTitle, HICON hIcon, bool bAutoDelete, bool bActive)
{
	//��������
	try
	{
		CNewGoGamePower * pPageItem=new CNewGoGamePower;
		if (pPageItem==NULL) return MAX_PAGE;
		//��������
		if (szTitle!=NULL) 
			pPageItem->m_strTitle=szTitle;
		pPageItem->m_pWnd=pWnd;
		
		pPageItem->m_hIcon=hIcon;
		pPageItem->m_bAutoDelete=bAutoDelete;
		pPageItem->SetSkin(m_pathn,m_patha,m_cn,m_ca);
		//�ƶ�����
		CRect ClientRect;
		GetClientRect(&ClientRect);
		pWnd->MoveWindow(&ClientRect);

		//��������
		for (UINT i=0;i<MAX_PAGE;i++)
		{
			if (m_uIndex[i]==MAX_PAGE) 
			{
				m_uIndex[i]=m_PageArray.Add(pPageItem);
				FixTabPageSize();
				if ((bActive==true)||(m_uActivePage==MAX_PAGE)) 
					SetActivePage(i);
				return i;
			}
		}

		//��������
		pPageItem->DeletePage();
		delete pPageItem;
		pPageItem=NULL;
	}
	catch (...) {}

	return MAX_PAGE;
}
//���봰��
UINT CNewGameClass::AddTabPage(CWnd * pWnd, CWnd *pRightWnd,TCHAR * szTitle, HICON hIcon, bool bAutoDelete, bool bActive)
{
	//��������
	try
	{
		CNewGoGamePower * pPageItem=new CNewGoGamePower;
		if (pPageItem==NULL)
			return MAX_PAGE;
		//��������
		if (szTitle!=NULL)
			pPageItem->m_strTitle=szTitle;
		pPageItem->m_pWnd=pWnd;
		//�ұߴ���ָ��
		pPageItem->m_pRightWnd=pRightWnd;

		pPageItem->m_hIcon=hIcon;
		pPageItem->m_bAutoDelete=bAutoDelete;
		pPageItem->SetSkin(m_pathn,m_patha,m_cn,m_ca);
		//�ƶ�����
		CRect ClientRect;
		GetClientRect(&ClientRect);
		pWnd->MoveWindow(&ClientRect);

		//��������
		for (UINT i=0;i<MAX_PAGE;i++)
		{
			if (m_uIndex[i]==MAX_PAGE) 
			{
				m_uIndex[i]=m_PageArray.Add(pPageItem);
				FixTabPageSize();
				if ((bActive==true)||(m_uActivePage==MAX_PAGE))
					SetActivePage(i);
				return i;
			}
		}

		//��������
		pPageItem->DeletePage();
		delete pPageItem;
		pPageItem=NULL;
	}
	catch (...) {}

	return MAX_PAGE;
}

//ɾ��ҳ��
bool CNewGameClass::RemoveTabPage(UINT uExtIndex)
{
	//Ч�����
	if ((uExtIndex>=MAX_PAGE)||(m_uIndex[uExtIndex]==MAX_PAGE)) return false;
	INT_PTR uCount=m_PageArray.GetCount();
	if (m_uIndex[uExtIndex]>=uCount) return false;

	//��ȡҳ��
	CNewGoGamePower * pPageItem=GetPageItem(uExtIndex);
	if (pPageItem==NULL) 
		return false;

	//ɾ��ҳ��
	bool bDeleteActivePage=((m_uActivePage<MAX_PAGE)&&(m_uActivePage==m_uIndex[uExtIndex]));
	m_PageArray.RemoveAt(m_uIndex[uExtIndex]);
	pPageItem->DeletePage();
	delete pPageItem;
	pPageItem=NULL;
	uCount--;
	//��������
	for (INT_PTR i=0;i<MAX_PAGE;i++)
	{
		if (i==uExtIndex) continue;
		if ((m_uIndex[i]!=MAX_PAGE)&&(m_uIndex[i]>m_uIndex[uExtIndex])) m_uIndex[i]--;
	}
	if (m_uActivePage>(UINT)m_uIndex[uExtIndex]) m_uActivePage--;
	m_uIndex[uExtIndex]=MAX_PAGE;

	//���½���
	FixTabPageSize();
	if (bDeleteActivePage==true) 
	{
		m_uActivePage=MAX_PAGE;
		if (uCount>0) SetActivePage(0);
	}
	//Invalidate(FALSE);

	return true;
}

void CNewGameClass::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_rect.PtInRect(point))
	{
		//ShowCursor(false);
		//SetCursor(m_cursor);
		//ShowCursor(true);
	}
	CWnd::OnMouseMove(nFlags, point);
}

//�ػ�����
void CNewGameClass::OnPaint()
{
	//--------------------------------------------
	/// ˫�����沿�ִ���
	CPaintDC realdc(this);
	//realdc.SelectClipRgn(&m_Rgn, RGN_AND);

	CRect ClientRect, clipRect;
	GetClipBox( realdc.GetSafeHdc(), &clipRect );
	GetClientRect(&ClientRect);
	int iWidth = ClientRect.Width();
	int iHeight = ClientRect.Height();
	CRgn rgn;
	rgn.CreateRectRgn(0, 0, ClientRect.right, m_tabl.GetHeight());
	realdc.SelectClipRgn(&rgn, RGN_AND);

	CBitmap BufBmp;
	CDC dc;
	CRect rc;
	GetClientRect(&rc);
	BufBmp.CreateCompatibleBitmap(&realdc, rc.Width(), rc.Height());
	dc.CreateCompatibleDC(&realdc);
	CBitmap *pOldBmp=dc.SelectObject(&BufBmp);
	CFont *currentFont = realdc.GetCurrentFont();
	CFont *oldFont = dc.SelectObject(currentFont);
	//--------------------------------------------
	//CPaintDC dc(this);
	//CRect ClientRect;
	//GetClientRect(&ClientRect);
	//����
	CGameImageHelper helpc(&m_tabc);
	if(helpc.GetWidth() > 0)
	{
		helpc.StretchBlt(dc.GetSafeHdc(), 0,0,ClientRect.Width(), helpc.GetHeight(),
			0,0,helpc.GetWidth(),helpc.GetHeight());
	}
	////���Ͻ�
	CGameImageHelper helpl(&m_tabl);
	helpl.BitBlt(dc.GetSafeHdc(),0,0);
	////���Ͻ�
	CGameImageHelper helpr(&m_tabr);
	helpr.BitBlt(dc.GetSafeHdc(),ClientRect.right-helpr.GetWidth(),0);
	if(m_uActivePage != 0)
	{
		helpr.BitBlt(dc.GetSafeHdc(),ClientRect.right-helpr.GetWidth() - m_CurWidth ,0);
	}
	//	CPaintDC dc(this);
	//	CDC BufferDC;
	//	CRect ClientRect;
	//	CBitmap bitmap;	

	//	GetClientRect(&ClientRect);
	//	BufferDC.CreateCompatibleDC(&dc);
	//	bitmap.CreateCompatibleBitmap(&dc,ClientRect.Width(),ClientRect.Height());
	//	BufferDC.SelectObject(&bitmap);

	//	BufferDC.SetBkMode(TRANSPARENT);
	//	BufferDC.SelectObject(&CGameImageLink::m_HFont);//��ʾ����
	//����(�����)
	//	CGameImageHelper tab(&m_c);
	/*	if(tab.GetWidth()>0)
	for(int i = 300; i < ClientRect.right; i+=tab.GetWidth())
	{
	tab.BitBlt(BufferDC.GetSafeHdc(),i,ClientRect.bottom-tab.GetHeight());
	}
	BufferDC.SetTextColor(m_adcolor);
	BufferDC.DrawText(m_ad,&m_addrawrect,DT_VCENTER|DT_RIGHT|DT_END_ELLIPSIS|DT_SINGLELINE);	
	*/	//��ǩ
	//����
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(&CGameImageLink::m_HFont);//��ʾ����
	INT_PTR uCount=m_PageArray.GetSize();
	for (INT_PTR i=0;i<uCount;i++)
	{
		CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(i);
		if (pPageItem!=NULL) 
		{
			pPageItem->Draw(&dc,m_uViewStyte,m_uActivePage==i);
		}
	}

	//--------------------------------------------
	/// ˫������沿�ִ���
	realdc.BitBlt(
		0,
		0,
		ClientRect.Width(), 
		ClientRect.Height(),
		&dc,
		0,
		0,
		SRCCOPY);
	BufBmp.DeleteObject();
	oldFont->DeleteObject();
	dc.DeleteDC();
	rgn.DeleteObject();
	/// ˫������沿�ִ���
	//--------------------------------------------
}

//��ʾ��Ȩ��Ϣ
bool CNewGameClass::ShowCopyRight(bool bShow)
{
	m_bCopyRight=false;
	//Invalidate(FALSE);
	return true;
}

//�滭���
void CNewGameClass::DrawTabFrame(CDC * pDC)
{
	CRect ClientRect;
	GetClientRect(&ClientRect);
	ClientRect.bottom-=TAB_HEIGHT+5;
	pDC->Draw3dRect(ClientRect,CGameImageLink::m_TabResource.m_crFrameHeight,CGameImageLink::m_TabResource.m_crFrameShadow);
	ClientRect.DeflateRect(1,1);
	pDC->Draw3dRect(ClientRect,CGameImageLink::m_TabResource.m_crFrameHeight,CGameImageLink::m_TabResource.m_crFrameShadow);
	return;
}

//������С
bool CNewGameClass::FixTabPageSize()
{
	if(!m_bInit)
		return true;

	//��ȡ���ڴ�С
	CRect ClientRect;
	GetClientRect(&ClientRect);
	//MapWindowPoints(GetParent(),&ClientRect);

	m_Rgn.SetRectRgn(&ClientRect);
	CRgn rgn;
	rgn.CreateRectRgn(0,0,0,0);

	//���ñ���
	UINT uBaseXPos=0,uBaseYPos=0,uStepX=0,uStepY=0,iWidth=0,iHeight=0;
	switch (m_uViewStyte)
	{
	case TS_DOWN:	//����
		{//Ĭ��
			uBaseXPos=m_x;
			uBaseYPos=m_y;
			uStepX=m_tabn.GetWidth();
			uStepY= 0;
			iWidth=uStepX;
			iHeight=m_tabn.GetHeight();
			break;
		}
	case TS_UP:	//����
		{
			uBaseXPos=m_x+400;
			uBaseYPos=ClientRect.top+m_y+m_tabc.GetHeight();
			uStepX=m_tabn.GetWidth();
			uStepY= 0;
			iWidth=uStepX;
			iHeight=m_tabn.GetHeight();
			break;
		}
	}
	//����λ��
	INT_PTR uPageCount=m_PageArray.GetCount();
	INT_PTR i;
	for (i=0;i<uPageCount;i++)
	{
		CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(i);
		if (pPageItem!=NULL)
		{
			//��ǩλ��
			pPageItem->m_rcTitleRect.SetRect(uBaseXPos+i*uStepX,uBaseYPos+i*uStepY,uBaseXPos+i*uStepX+iWidth,uBaseYPos+i*uStepY+iHeight);//ѡ��Ĵ�С

			//�����������ִ���
			if(i == 0)
			{
				pPageItem->m_pWnd->MoveWindow(0,m_tabc.GetHeight(), ClientRect.Width(),ClientRect.Height()-m_tabc.GetHeight());

			}
			else
			{
				if(pPageItem->m_pRightWnd != NULL)
				{
					CGamePlaceDlg *pDlg=(CGamePlaceDlg *)GetParent();
					m_CurWidth = pDlg->m_bMax?RIGHT_WND_WIDTH :RIGHT_WND_WIDTH -100;

					pPageItem->m_pWnd->MoveWindow(
						0,
						m_tabc.GetHeight(),
						ClientRect.Width() - m_CurWidth,
						ClientRect.Height()-m_tabc.GetHeight());

					pPageItem->m_pRightWnd->MoveWindow(
						ClientRect.Width() - m_CurWidth,
						0,
						m_CurWidth,
						ClientRect.Height()-0);
				}
				else
				{
					pPageItem->m_pWnd->MoveWindow(0,m_tabc.GetHeight(),ClientRect.Width(),ClientRect.Height()-m_tabc.GetHeight());
				}
			}
		}		
	}
	rgn.SetRectRgn(0, m_tabc.GetHeight(), ClientRect.Width(),ClientRect.Height());
	rgn.CombineRgn(&rgn, &m_Rgn, RGN_AND);
	m_Rgn.CombineRgn(&m_Rgn, &rgn, RGN_XOR);
	if (m_uActivePage>0)
	{
		rgn.SetRectRgn(
			ClientRect.Width() - m_CurWidth,
			0,
			m_CurWidth,
			ClientRect.Height()
			);
		rgn.CombineRgn(&rgn, &m_Rgn, RGN_AND);
		m_Rgn.CombineRgn(&m_Rgn, &rgn, RGN_XOR);
	}
	rgn.DeleteObject();

	return true;
}
//���ô������ڴ�С
void CNewGameClass::SetMainRoomWnd(CRect rect)
{
	//����λ��
	INT_PTR uPageCount=m_PageArray.GetCount();
	INT_PTR i;
	for (i=0;i<uPageCount;i++)
	{
		CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(i);
		if (pPageItem!=NULL)
		{
			pPageItem->m_pWnd->MoveWindow(&rect);
		}		
	}
	InvalidateRect(rect);
	return  ;
}
//λ����Ϣ
void CNewGameClass::SetViewStyle(UINT uStyle)
{
	m_uViewStyte=uStyle;
}

//λ����Ϣ
void CNewGameClass::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	//	if(m_copyright.GetSafeHwnd()!=NULL)
	//		m_copyright.MoveWindow(cx-200,cy-30,180,25);
	//�������°�Ȩ��Ϣ
	/*	m_rect.top=cy-30;
	m_rect.left=cx-280;
	m_rect.bottom=cy;
	m_rect.right=cx-15;*/
	FixTabPageSize();
}

//��갴����Ϣ
void CNewGameClass::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bIsLBOK)
	{
		return;
	}

	INT_PTR uCount=m_PageArray.GetSize();
	for (int i=0;i<uCount;i++)
	{
		CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(i);
		if ((pPageItem!=NULL)&&(pPageItem->m_rcTitleRect.PtInRect(point)))
		{
			SetActivePage(i);
			break;
		}
	}
	if(m_rect.PtInRect(point))
	{
		//ShellExecute(NULL,"open",m_szURL,NULL,NULL,SW_MAXIMIZE);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

//��ȡҳ��
CNewGoGamePower * CNewGameClass::GetPageItem(UINT uExtIndex)
{
	if ((uExtIndex<MAX_PAGE)&&(m_uIndex[uExtIndex]<m_PageArray.GetCount())) 
	{
		return (CNewGoGamePower *)m_PageArray.GetAt(m_uIndex[uExtIndex]);
	}
	return NULL;
}

//�����
bool CNewGameClass::SetActivePageByExtIndex(UINT uExtIndex)
{
	if (uExtIndex<MAX_PAGE) 
		return SetActivePage((UINT)m_uIndex[uExtIndex]);
	return false;
}

//�����
bool CNewGameClass::SetActivePage(UINT uIndex)
{
	//Ч������
	if ((uIndex==m_uActivePage)||(uIndex>=(UINT)m_PageArray.GetCount())) return false;

	//Ѱ����ҳ��
	CNewGoGamePower * pNewPageItem=(CNewGoGamePower *)m_PageArray.GetAt(uIndex);
	if (pNewPageItem==NULL) return false;

	// by ZXD��Ϊ�����ط����׷���ķ�������ǩҳ
	if (!pNewPageItem->m_bAutoDelete) return false;

	//����ҳ��
	if ((m_uActivePage!=MAX_PAGE) && (m_uActivePage!=uIndex))
	{
		CNewGoGamePower * pOldPageItem=(CNewGoGamePower *)m_PageArray.GetAt(m_uActivePage);

		if (pOldPageItem != NULL)
		{
			if (pOldPageItem->m_pWnd != NULL)
			{
				pOldPageItem->m_pWnd->PostMessageA(IDM_TAB_SELECT_LOOST,0,0);
				pOldPageItem->m_pWnd->ShowWindow(SW_HIDE);
			}

			if (pOldPageItem->m_pRightWnd != NULL)
			{
				pOldPageItem->m_pRightWnd->ShowWindow(SW_HIDE);
			}
			
		}	
	}

	//��ʾҳ��
	m_uActivePage=uIndex;
	pNewPageItem->m_pWnd->ShowWindow(SW_SHOW);
	if(m_uActivePage == 0)//����Ǵ���������ʾ�������
	{
		pNewPageItem->m_pWnd->PostMessageA(IDM_SHOW_LEFT,1,1);
	}else
	{
		pNewPageItem->m_pWnd->PostMessageA(IDM_SHOW_LEFT,0,0);
	}

	pNewPageItem->m_pWnd->PostMessageA(IDM_TAB_SELECT,0,0);

	GetParent()->PostMessage(WM_USER+100,m_uActivePage);

	if(pNewPageItem->m_pRightWnd !=NULL)
	{
		pNewPageItem->m_pRightWnd->ShowWindow(SW_SHOW);
	}
		
	pNewPageItem->m_pWnd->SetFocus();
	FixTabPageSize();
	Invalidate(FALSE);

	return true;
}

//�õ�����
void CNewGameClass::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	if (m_uActivePage!=m_PageArray.GetCount()) 
	{
		CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(m_uActivePage);
		if (pPageItem!=NULL) pPageItem->m_pWnd->SetFocus();
	}
}


void CNewGameClass::LoadSkin(void)
{
	int r,g,b;
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "skin0.bcf");
	CString key="NewGameClass";//Glb().m_skin;
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal("skin0","skinfolder","image\\client\\skin0\\");

	wsprintf(path,"%stab\\tab_c.bmp",skinfolder);
	m_tabc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%stab\\tab_l.bmp",skinfolder);
	m_tabl.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%stab\\tab_r.bmp",skinfolder);
	m_tabr.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	m_x=f.GetKeyVal(key,"tabx",7);
	m_y=f.GetKeyVal(key,"taby",8);

	wsprintf(m_pathn,"%stab\\tab_n.bmp",skinfolder);
	m_tabn.SetLoadInfo(m_pathn,CGameImageLink::m_bAutoLock);
	wsprintf(m_patha,"%stab\\tab_a.bmp",skinfolder);
	//���߾���
	//	wsprintf(m_pathn,"%shall\\t.bmp",skinfolder);
	//	m_top.SetLoadInfo(m_pathn,CGameImageLink::m_bAutoLock);

	r=f.GetKeyVal(key,"tabnr",0);
	g=f.GetKeyVal(key,"tabng",0);
	b=f.GetKeyVal(key,"tabnb",255);
	m_cn=RGB(r,g,b);

	r=f.GetKeyVal(key,"tabar",0);
	g=f.GetKeyVal(key,"tabag",255);
	b=f.GetKeyVal(key,"tabab",0);
	m_ca=RGB(r,g,b);


	INT_PTR uPageCount=m_PageArray.GetCount();
	for (INT_PTR i=0;i<uPageCount;i++)
	{
		CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(i);
		if (pPageItem!=NULL)
		{
			pPageItem->SetSkin(m_pathn,m_patha,m_cn,m_ca);
		}
	}
}

void CNewGameClass::ChangeSkin(
	const CString& skinBcfFileName,
	const CString& strSkinKey, 
	const CString& skinPath)
{
	int r,g,b;
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + skinBcfFileName);
	CString key="NewGameClass";//Glb().m_skin;
	TCHAR path[MAX_PATH];
	CString skinfolder;
	skinfolder=f.GetKeyVal(strSkinKey,"skinfolder",skinPath);

	wsprintf(path,"%stab\\tab_c.bmp",skinfolder);
	m_tabc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%stab\\tab_l.bmp",skinfolder);
	m_tabl.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	wsprintf(path,"%stab\\tab_r.bmp",skinfolder);
	m_tabr.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	m_x=f.GetKeyVal(key,"tabx",7);
	m_y=f.GetKeyVal(key,"taby",8);

	wsprintf(m_pathn,"%stab\\tab_n.bmp",skinfolder);
	m_tabn.SetLoadInfo(m_pathn,CGameImageLink::m_bAutoLock);
	wsprintf(m_patha,"%stab\\tab_a.bmp",skinfolder);
	//���߾���
	//	wsprintf(m_pathn,"%shall\\t.bmp",skinfolder);
	//	m_top.SetLoadInfo(m_pathn,CGameImageLink::m_bAutoLock);

	r=f.GetKeyVal(key,"tabnr",0);
	g=f.GetKeyVal(key,"tabng",0);
	b=f.GetKeyVal(key,"tabnb",255);
	m_cn=RGB(r,g,b);

	r=f.GetKeyVal(key,"tabar",0);
	g=f.GetKeyVal(key,"tabag",255);
	b=f.GetKeyVal(key,"tabab",0);
	m_ca=RGB(r,g,b);


	INT_PTR uPageCount=m_PageArray.GetCount();
	for (INT_PTR i=0;i<uPageCount;i++)
	{
		CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(i);
		if (pPageItem!=NULL)
		{
			pPageItem->SetSkin(m_pathn,m_patha,m_cn,m_ca);
		}
	}
}

void CNewGameClass::SetAd(TCHAR ad[])
{
	/*	m_ad=ad;
	CDC *dc=GetDC();
	GetTextExtentPoint32(dc->GetSafeHdc(), ad, lstrlen(ad), &m_adsize);
	m_addrawrect=m_adRect;
	m_addrawrect.left=m_adRect.right;
	m_addrawrect.right=m_addrawrect.left+m_adsize.cx;
	SetTimer(TIMER_AD,100,NULL);*/
}

void CNewGameClass::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==TIMER_AD)
	{
		m_addrawrect.left-=3;
		m_addrawrect.right=m_addrawrect.left+m_adsize.cx;
		if(m_addrawrect.right<=m_adRect.left)
		{
			m_addrawrect=m_adRect;
			m_addrawrect.left=m_adRect.right;
			m_addrawrect.right=m_addrawrect.left+m_adsize.cx;
		}
	}
	InvalidateRect(m_adRect);
}

LRESULT CNewGameClass::OnLoadSkin(WPARAM wparam, LPARAM lparam)
{
	int sk=(int)wparam;
	//	TCHAR sz[100];
	CString str;
	int cursk;
	if(sk>4||sk<0)
		return false;

	cursk=Glb().m_skin.GetAt(4);
	cursk -='0';
	if(cursk==sk)
		return false;

	str.Format("skin%d",sk);

	Glb().m_skin=str;

	CString s=CBcfFile::GetAppPath ();
	CBcfFile f( s + "bzgame.bcf");
	CString m_key="BZW";
	f.SetKeyValString(m_key,"skin",str);

	LoadSkin();
	FixTabPageSize();

	INT_PTR uPageCount=m_PageArray.GetCount();
	INT_PTR i;
	for (i=0;i<uPageCount;i++)
	{
		CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(i);
		if (pPageItem!=NULL)
		{
			pPageItem->m_pWnd->PostMessage(WM_LOADSKIN,wparam,lparam);
		}		
	}
	return true;
}

//��������λ��
void CNewGameClass::AutoSize(int cx,int cy)
{
	CRect clientrect;
	GetClientRect(&clientrect);
	this->MoveWindow(cx,cy,clientrect.Width(),clientrect.Height());
}
//��Ϣ
LRESULT CNewGameClass::OnSendRoomMsg(WPARAM wparam, LPARAM lparam)
{
		CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(m_uActivePage);
		if (pPageItem!=NULL)
		{
			pPageItem->m_pWnd->PostMessage(IDM_SEND_ROOMMSG,wparam,lparam);
		}		
	return 0;
}
//��Ϣ
LRESULT CNewGameClass::OnHideRight(WPARAM wparam, LPARAM lparam)
{

	this->GetParent()->PostMessage(IDM_HIDE_RIGHT,wparam,lparam);
		
	return 0;
}

//������Ϣ
LRESULT CNewGameClass::OpenBank(WPARAM wparam, LPARAM lparam)
{
	if(m_uActivePage == 0 &&(int)wparam == 0)
	{
		//AFCMessageBox("�������Ϸ������ʹ�����й��ܡ�","��ܰ��ʾ");
		CNewGoGamePower * pPageItem = (CNewGoGamePower*)m_PageArray.GetAt(m_uActivePage);
		if( pPageItem != NULL && pPageItem->m_pWnd != NULL )
		{
			pPageItem->m_pWnd->PostMessage(IDM_OPEN_BANK, 0, 0);
		}  //����������Ϣ��Ӧ��ť�ڴ�ʵ��
		return 0;
	}
	if(m_uActivePage == 0 &&(int)wparam == 1)
	{
		//AFCMessageBox("�������Ϸ������ʹ�ú������ܹ��ܡ�","��ܰ��ʾ");
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�������Ϸ������ʹ�ú������ܹ��ܡ�");
		return 0;
	}
	if((int)wparam == 0)
	{
		CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(m_uActivePage);
		if (pPageItem!=NULL)
		{
			pPageItem->m_pWnd->PostMessage(WM_COMMAND,MAKEWPARAM(2028,BN_CLICKED),(LPARAM)GetSafeHwnd());
		}
	}
	else if((int)wparam == 1)
	{
		CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(m_uActivePage);
		if (pPageItem!=NULL)
		{
			pPageItem->m_pWnd->PostMessage(WM_COMMAND,MAKEWPARAM(2027,BN_CLICKED),(LPARAM)GetSafeHwnd());
		}
	}

	return 0;
}

//IE��Ϣ
LRESULT CNewGameClass::OpenIE(WPARAM wparam, LPARAM lparam)
{
	CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(0);
	pPageItem->m_pWnd->PostMessage(IDM_OPEN_IE,wparam,lparam);
	return 0;
}

//IE��Ϣ
LRESULT CNewGameClass::CallGM(WPARAM wparam, LPARAM lparam)
{
	CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(0);
	pPageItem->m_pWnd->PostMessage(IDM_CALL_GM,wparam,lparam);
	return 0;
}

LRESULT CNewGameClass::OAppeal(WPARAM wparam, LPARAM lparam)
{
	CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(0);
	pPageItem->m_pWnd->PostMessage(IDM_MESSAGE_APPEAL,wparam,lparam);
	return 0;
}

//�������� zxj 2009-11-12
LRESULT CNewGameClass::OnLockAccount(WPARAM wParam, LPARAM lParam)
{
	CNewGoGamePower * pPageItem = (CNewGoGamePower*)m_PageArray.GetAt(0);
	if( pPageItem != NULL && pPageItem->m_pWnd !=NULL )
	{
		pPageItem->m_pWnd->PostMessage(IDM_LOCK_ACCOUNT, wParam, lParam);
	}
	return 0;
}

LRESULT CNewGameClass::OnOpenBank(WPARAM wParam, LPARAM lParam)
{
	MessageBox("CNewGameClass Bank");
	CNewGoGamePower * pPageItem = (CNewGoGamePower*)m_PageArray.GetAt(0);
	if( pPageItem != NULL && pPageItem->m_pWnd !=NULL )
	{
		pPageItem->m_pWnd->PostMessage(IDM_OPEN_BANK, wParam, lParam);
	}
	return 0;
}

//˫���ر���Ϸ
void CNewGameClass::OnLButtonDblClk(UINT nFlags,CPoint point)
{
	if(!m_uActivePage)
		return;

	CNewGoGamePower * pPageItem=(CNewGoGamePower *)m_PageArray.GetAt(m_uActivePage);
	pPageItem->m_pWnd->PostMessage(WM_USER+1000,0,0);

	CWnd::OnLButtonDblClk(nFlags, point);

}
//����
void CNewGameClass::OnExchangeSkin(const CString& skinBcffile,const CString& strKey,const CString& skinPath)
{
	ChangeSkin(skinBcffile,strKey,skinPath);
	if ((CGamePlaceDlg*)AfxGetMainWnd() != NULL)
		((CGamePlaceDlg*)AfxGetMainWnd())->Invalidate();
	return;
}
