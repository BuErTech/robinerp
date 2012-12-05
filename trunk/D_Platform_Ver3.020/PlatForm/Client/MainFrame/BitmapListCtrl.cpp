#include "StdAfx.h"
#include "BitmapListCtrl.h"


CBitmapListCtrl::CBitmapListCtrl(void)
{
	NCOverride=FALSE; //False as default...
	Who=SB_BOTH;
	//m_itembk.SetLoadInfo(".\\image\\userlist.bmp",CGameImageLink::m_bAutoLock);
}

CBitmapListCtrl::~CBitmapListCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CBitmapListCtrl, CUserListCtrl)
	//{{AFX_MSG_MAP(CListCtrlHiddenSB)
	ON_WM_NCCALCSIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CBitmapListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType==ODT_LISTVIEW)
	{
		//TCHAR sz[100];
		//wsprintf(sz,"%d",(lpDrawItemStruct->rcItem.bottom-lpDrawItemStruct->rcItem.top));
		//MessageBox(sz);
		//��ȡ����
		UINT iWidth=0;
		TCHAR szBuffer[30];
		memset(szBuffer,0,sizeof(szBuffer));
		CDC * pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
		UserItemStruct * pUserItem=(UserItemStruct *)GetItemData(lpDrawItemStruct->itemID);
		CFont font;
		font.CreateFont(-12,0,0,0,700,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
		
		CFont *pOldFont = pDC->SelectObject(&font);	///< Modified by zxd 20100709 �����屣��������

		//������ɫ
//		COLORREF crTextColor,crBackColor;
		bool bSelect=(lpDrawItemStruct->itemState&ODS_SELECTED)||(lpDrawItemStruct->itemState&ODS_FOCUS);
		//GetDrawColor(crTextColor,crBackColor,pUserItem,0,bSelect);
		//pDC->SetBkColor(crBackColor);
		//pDC->SetTextColor(crTextColor);
		pDC->SetBkMode(TRANSPARENT);
		//�滭��Ϣ
		CSize TextSize;
		CRect rect1;
		TCHAR szname[300];
		for (int i=0;i<m_ListHeader.GetItemCount();i++)
		{
			int iTemp=GetColumnWidth(i);
			TextSize=pDC->GetTextExtent(szBuffer,lstrlen(szBuffer));
			GetItemText(lpDrawItemStruct->itemID,i,szBuffer,sizeof(szBuffer));
			CRect TextRect(lpDrawItemStruct->rcItem.left+iWidth,lpDrawItemStruct->rcItem.top,lpDrawItemStruct->rcItem.left+iWidth+iTemp,lpDrawItemStruct->rcItem.bottom);
			CGameImageHelper help(&m_itembk);
			help.BitBlt(pDC->GetSafeHdc(),TextRect.left,TextRect.top);
			//ͷ��
			rect1=m_rlog;
			rect1.OffsetRect(TextRect.left,TextRect.top);
			CGameImage log;
			wsprintf(szname,".\\image\\log\\roomlistlog%d.bmp",pUserItem->GameUserInfo.bLogoID);
			//lstrcpy(szname,".\\image\\log\\log1.bmp");
			log.SetLoadInfo(szname,CGameImageLink::m_bAutoLock);
			CGameImageHelper helper(&log);
			//AFCStretchImage(pDC,rect1.left,rect1.top,rect1.Width(),rect1.Height(),helper,0,0,log.GetWidth(),log.GetHeight(),log.GetPixel(1,1));
			helper.BitBlt(pDC->GetSafeHdc(),rect1.left,rect1.top);
			helper.CloseGDIHandle();
			//����
			rect1=m_rname;
			rect1.OffsetRect(TextRect.left,TextRect.top);
			lstrcpy(szname,pUserItem->GameUserInfo.nickName);
			DrawText(pDC->GetSafeHdc(),szname,lstrlen(szname),rect1,DT_CENTER|DT_VCENTER);
			//�ȼ�
			lstrcpy(szname,m_pGetOrderFun(pUserItem->GameUserInfo.dwPoint,0));
			rect1=m_rorder;
			rect1.OffsetRect(TextRect.left,TextRect.top);
			DrawText(pDC->GetSafeHdc(),szname,lstrlen(szname),rect1,DT_CENTER|DT_VCENTER);
			//���
			lstrcpy(szname,m_sztitle);
			rect1=m_rcoin;
			rect1.OffsetRect(TextRect.left,TextRect.top);
			DrawText(pDC->GetSafeHdc(),szname,lstrlen(szname),rect1,DT_CENTER|DT_VCENTER);
			//����ֵ
			wsprintf(szname,"%d",pUserItem->GameUserInfo.dwPoint-GetNormalOrderBasePoint(pUserItem->GameUserInfo.dwPoint));
			rect1=m_rcoinc;
			rect1.OffsetRect(TextRect.left,TextRect.top);
			DrawText(pDC->GetSafeHdc(),szname,lstrlen(szname),rect1,DT_CENTER|DT_VCENTER);
			//Ӯ
			wsprintf(szname,"Ӯ %d",pUserItem->GameUserInfo.uWinCount);
			rect1=m_rwin;
			rect1.OffsetRect(TextRect.left,TextRect.top);
			DrawText(pDC->GetSafeHdc(),szname,lstrlen(szname),rect1,DT_CENTER|DT_VCENTER);
			//��
			wsprintf(szname,"�� %d",pUserItem->GameUserInfo.uLostCount);
			rect1=m_rloss;
			rect1.OffsetRect(TextRect.left,TextRect.top);
			DrawText(pDC->GetSafeHdc(),szname,lstrlen(szname),rect1,DT_CENTER|DT_VCENTER);
			//����
			if(pUserItem->GameUserInfo.bDeskNO!=255&&m_uComType!=TY_MATCH_GAME)
			{
				wsprintf(szname,"%d\n��\n��",pUserItem->GameUserInfo.bDeskNO+1);
				rect1=m_rdesk;
				rect1.OffsetRect(TextRect.left,TextRect.top);
				DrawText(pDC->GetSafeHdc(),szname,lstrlen(szname),rect1,DT_CENTER|DT_VCENTER);
			}

			
			if(bSelect)
				PatBlt(pDC->GetSafeHdc(),TextRect.left,TextRect.top,TextRect.Width(),TextRect.Height(),DSTINVERT);
/*
			pDC->DrawText(szBuffer,lstrlen(szBuffer),&TextRect,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
			if (TextSize.cx<iTemp) 
			{
				pDC->FillSolidRect(lpDrawItemStruct->rcItem.left+iWidth+TextSize.cx,lpDrawItemStruct->rcItem.top,
					iTemp-TextSize.cx,lpDrawItemStruct->rcItem.bottom-lpDrawItemStruct->rcItem.top,crBackColor);
			}
			iWidth+=iTemp;*/
		}

		/// {{Added by zxd 20100709 �ͷ�GDI��Դ
		pDC->SelectObject(pOldFont);
		font.DeleteObject();
		/// Added by zxd 20100709 �ͷ�GDI��Դ}}
	}
	return;	
}

//��ʼ������
void CBitmapListCtrl::InitListCtrl(GetOrderName * pOrderFunction, UINT uComType)
{
	CHappyGoDlg::InitListCtrl(pOrderFunction,uComType);
	
	//��������
	m_uComType=uComType;
	lstrcpy(m_sztitle,TEXT("����"));
	if (uComType==TY_MONEY_GAME)
		lstrcpy(m_sztitle,TEXT("���"));
	else if (uComType==TY_MATCH_GAME) 
		lstrcpy(m_sztitle,TEXT("����"));

	this->SetBkColor(RGB(203, 211, 227));

	CRect rect1(0,0,62,48);//ͷ��
	CRect rect2(62,6,192,26);//����
	CRect rect3(62,30,192,48);//�ȼ�

	CRect rect4(192,6,283,26);//���
	CRect rect5(192,30,283,48);//�����

	CRect rect6(283,6,376,26);//Ӯ
	CRect rect7(283,30,376,48);//��

	CRect rect8(376,6,408,48);//����
	//CRect rect9(62,24,192,48);
	
	m_rlog=rect1;
	m_rname=rect2;
	m_rorder=rect3;
	m_rcoin=rect4;
	m_rcoinc=rect5;
	m_rwin=rect6;
	m_rloss=rect7;
	m_rdesk=rect8;

	//��������
	SetRedraw(FALSE);
	//InsertColumn(0,"�û���",LVCFMT_LEFT,120);
	InsertColumn(0,"�ǳ�",LVCFMT_LEFT,120);
//	InsertColumn(1,"ID ����",LVCFMT_LEFT,60);
/*	InsertColumn(1,"����",LVCFMT_LEFT,40);
	InsertColumn(2,szBuffer,LVCFMT_LEFT,60);
	InsertColumn(3,"����",LVCFMT_LEFT,90);
	InsertColumn(4,"�ܾ���",LVCFMT_LEFT,50);
	InsertColumn(5,"ʤ��",LVCFMT_LEFT,50);
	InsertColumn(6,"���",LVCFMT_LEFT,50);
	InsertColumn(7,"�;�",LVCFMT_LEFT,50);
	InsertColumn(8,"����",LVCFMT_LEFT,50);*/
//	InsertColumn(12,"��Ϸ����",LVCFMT_CENTER,100);
	SetRedraw(TRUE);
	return;
}


void CBitmapListCtrl::HideScrollBars(int Type, int Which)
{
	if(Type==LCSB_CLIENTDATA) //This is the clientrect function
	{
		RECT ierect;
		int cxvs, cyvs;
		GetClientRect(&ierect); //Get client width and height

		cxvs = GetSystemMetrics (SM_CXVSCROLL); //Get the system metrics - VERT
		cyvs = GetSystemMetrics (SM_CYVSCROLL); //Get the system metrics - HORZ
		
		if(Which==SB_HORZ) cxvs=0; //Set VERT to zero when choosen HORZ
		if(Which==SB_VERT) cyvs=0; //Set HORZ to zero when choosen VERT

		//Here we set the position of the window to the clientrect + the size of the scrollbars
		SetWindowPos(NULL, ierect.left, ierect.top, ierect.right+cxvs, ierect.bottom+cyvs, SWP_NOMOVE | SWP_NOZORDER);

		//Her we modify the rect so the right part is subbed from the rect.
		if(Which==SB_BOTH||Which==SB_HORZ) ierect.bottom -= ierect.top;
		if(Which==SB_BOTH||Which==SB_VERT) ierect.right -= ierect.left;

		//Just to be safe that the left/top corner is 0...
		ierect.top = 0;
		ierect.left = 0;
		
		HRGN iehrgn = NULL; //This range is created base on which scrollbar that is going to be removed!

		//The -2 is probably a border of some kind that we also need to remove. I could not find any good
		//metrics that gave me an 2 as an answer. So insted we makes it static with -2.
		if(Which==SB_BOTH) iehrgn=CreateRectRgn (ierect.left, ierect.top, ierect.right-2, ierect.bottom-2);
		if(Which==SB_HORZ) iehrgn=CreateRectRgn (ierect.left, ierect.top, ierect.right, ierect.bottom-2);
		if(Which==SB_VERT) iehrgn=CreateRectRgn (ierect.left, ierect.top, ierect.right-2, ierect.bottom);
		
		//After the range has been made we add it...
		SetWindowRgn (iehrgn, TRUE);

		//Reset of NCOverride
		NCOverride=FALSE;

		/// {{Added by zxd 20100709 �ͷ�GDI��Դ
		DeleteObject(iehrgn);
		/// Added by zxd 20100709 �ͷ�GDI��Դ}}
	}

	if(Type==LCSB_NCOVERRIDE) //This is the NcCalcSize override
	{
		NCOverride=TRUE; //Set to true, so we run the code on each OnNcCalcSize.
		Who=Which; //Selects which scrollbars to get hidden.
	}
}

void CBitmapListCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	// TODO: Add your message handler code here and/or call default

	if(NCOverride==TRUE) //If the NCOverride is true we remove each time the CListCtrl calc its rect.
	{
		  if(Who==SB_HORZ) ModifyStyle(WS_HSCROLL ,0,0); //Just by modifing the style we remove the scrollbar
		  if(Who==SB_VERT) ModifyStyle(WS_VSCROLL,0,0);
		  if(Who==SB_BOTH) ModifyStyle(WS_HSCROLL | WS_VSCROLL,0,0);
	}

	CListCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
}

//��������
BOOL CBitmapListCtrl::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
	CRect DrawRect,HeadRect;
	pDC->GetClipBox(&DrawRect);
	if (m_ListHeader.GetSafeHwnd()!=NULL)
	{
		m_ListHeader.GetWindowRect(&HeadRect);
		if (DrawRect.top<HeadRect.Height()) DrawRect.top=HeadRect.Height();
	}
	pDC->FillSolidRect(&DrawRect,RGB(203,211,227));
	return TRUE;
}