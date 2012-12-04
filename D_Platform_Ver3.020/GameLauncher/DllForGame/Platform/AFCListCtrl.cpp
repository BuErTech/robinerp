//�����û��б�
#include "../stdafx.h"
//#include "Resource.h"
#include "AFCResource.h"
#include "AFCListCtrl.h"
#include "AFCFunction.h"
//#include "GameFrameDlg.h"
//using namespace AFC;
//using namespace AFC::AFCListCtrl;
//using namespace AFC::AFCResource;

//�û�״̬ͼ
//CImageList				CHappyGoDlg::m_UserStateList;				//�û�״̬�б�
//UINT					CHappyGoDlg::m_uSortPos[MAX_SORT_INDEX];	//��������
//bool					CHappyGoDlg::m_bInitStatic=false;			//�Ƿ��ʼ��

BEGIN_MESSAGE_MAP(CComBodyClass, CHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CHappyGoDlg, CListCtrl)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
END_MESSAGE_MAP()

/****************************************************************************************************/

//������Ϣ�ṹ
struct ListSortInfoStruct
{
	bool							bAsc;								//����˳��
	UINT							uSubItem;							//��������
	UINT							uItemKind;							//��������
	CHappyGoDlg				* pListCtrl;						//�б�ָ��
};

/****************************************************************************************************/

//���캯��
CComBodyClass::CComBodyClass()
{
	m_Type = 0;
}

//��������
CComBodyClass::~CComBodyClass()
{
}
void CComBodyClass::SetBkImage(TCHAR path[],TCHAR path2[],COLORREF color,COLORREF tcolor, int type)
{
	m_bkimg.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_bkimg2.SetLoadInfo(path2,CGameImageLink::m_bAutoLock);
	m_color=color;
	m_textcolor=tcolor;
	m_Type = type;
}
//�ػ�����
void CComBodyClass::OnPaint()
{
	CPaintDC dc(this);

	//��������ͼ
	CRect ClientRect;
	GetClientRect(&ClientRect);
	CBitmap BufferBmp;
	BufferBmp.CreateCompatibleBitmap(&dc,ClientRect.Width(),ClientRect.Height());
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(&dc);
	BufferDC.SelectObject(&BufferBmp);
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SelectObject(CGameImageLink::m_HFont);
	//BufferDC.SetTextColor(CGameImageLink::m_ListResource.m_crListColor);
	BufferDC.SetTextColor(RGB(0,0,0));
	BufferDC.FillSolidRect(&ClientRect,m_color);

	//{{ Added By ZXD�޸�����
	CFont font,*pOldFont;
	font.CreateFont(-12,0,0,0,400,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
	pOldFont=BufferDC.SelectObject(&font);	
	// Added By ZXD�޸�����}}

	HDITEM HDItem;
	CRect rcItemRect;
	TCHAR szBuffer[30];
	memset(szBuffer,0,sizeof(szBuffer));

	
	for (int i=0;i<GetItemCount();i++)
	{
		//��ȡ��Ϣ
		HDItem.pszText=szBuffer;
		HDItem.mask=HDI_TEXT|HDI_FORMAT;
		HDItem.cchTextMax=sizeof(szBuffer)/sizeof(szBuffer[0])-1;
		GetItemRect(i,&rcItemRect);
		GetItem(i,&HDItem);
		//�滭����
		if(m_Type == 0)
		{//��һ�ֽ�������
			if(i%2==0)
				m_bkimg.StretchBlt(BufferDC.GetSafeHdc(),rcItemRect);
			else
				m_bkimg2.StretchBlt(BufferDC.GetSafeHdc(),rcItemRect);
			CPen pen,*oldpen;
			pen.CreatePen(PS_SOLID,3,m_color);
			oldpen=BufferDC.SelectObject(&pen);
			BufferDC.MoveTo(rcItemRect.left,rcItemRect.bottom);
			BufferDC.LineTo(rcItemRect.right,rcItemRect.bottom);
			if(i!=(GetItemCount()-1))
				BufferDC.LineTo(rcItemRect.right,rcItemRect.top);
			::DeleteObject(BufferDC.SelectObject(*oldpen));
			pen.DeleteObject();
		}
		else if(m_Type == 1)
		{//�ڶ��ֽ�������
			m_bkimg.StretchBlt(BufferDC.GetSafeHdc(),rcItemRect);
			if(i!=(GetItemCount()-1))
				m_bkimg2.BitBlt(BufferDC.GetSafeHdc(),rcItemRect.right-m_bkimg2.GetWidth(),rcItemRect.top);
		}
		rcItemRect.DeflateRect(6,2,6,2);

		//�滭����ͷ
		UINT uFormat=DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS;
		if (HDItem.fmt&HDF_CENTER) uFormat|=DT_CENTER;
		else if (HDItem.fmt&HDF_LEFT) uFormat|=DT_LEFT;
		else if (HDItem.fmt&HDF_RIGHT) uFormat|=DT_RIGHT;
		BufferDC.SetTextColor(m_textcolor);
		BufferDC.DrawText(HDItem.pszText,lstrlen(HDItem.pszText),&rcItemRect,uFormat);
	}

	//�滭����
	dc.BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),&BufferDC,0,0,SRCCOPY);
	font.DeleteObject();
	BufferDC.DeleteDC();
	BufferBmp.DeleteObject();
	BufferDC.SelectObject(pOldFont);
	return;
}

//��������
BOOL CComBodyClass::OnEraseBkgnd(CDC * pDC)
{
	return FALSE;
}

/****************************************************************************************************/

//	�����б���

/****************************************************************************************************/

//���캯��
CHappyGoDlg::CHappyGoDlg(UINT * pItemKind)
{
	m_bAsc=true;
	m_pPointOrderFun=NULL;
	m_pItemKind=pItemKind;
	if (m_bInitStatic==false)
	{
		m_bInitStatic=true;
		for (UINT i=0;i<sizeof(m_uSortPos)/sizeof(m_uSortPos[0]);i++) m_uSortPos[i]=i;
	}
	m_FocusbkColor=RGB(255,255,255);//������ɫ
	m_FocusTextColor=RGB(0,0,0);
	m_bkColor=RGB(255,255,255);//������ɫ
	m_TextColor=RGB(0,0,0);
}

//��������
CHappyGoDlg::~CHappyGoDlg()
{
	int i=0;
	for (i=0; i<m_UserStateList.GetImageCount(); ++i)
	{
		m_UserStateList.Remove(i);
	}
	m_NewUserStateBmp.DeleteObject();
	m_UserStateList.DeleteImageList();
}

//��ʼ��
void CHappyGoDlg::InitListCtrl(GetOrderName * pOrderFunction, UINT uComType)
{
	m_pPointOrderFun=pOrderFunction;
	LoadSkin();
	return;
}

//��ȡ�û�״̬ ID
UINT CHappyGoDlg::GetStateImageIndex(UserItemStruct * pUserItem)
{
	//����״̬
	static BYTE bUserState[USER_STATE_COUNT]={USER_LOOK_STATE,USER_SITTING,USER_ARGEE,
		USER_WATCH_GAME,USER_CUT_GAME,USER_PLAY_GAME};

	//Ѱ��״̬
	UserInfoStruct * pUserInfo=&pUserItem->GameUserInfo;
	for (int i=0;i<USER_STATE_COUNT;i++)
	{
		if (pUserInfo->bUserState==bUserState[i])
		{
			if (pUserInfo->bTrader == 1) return USER_STATE_COUNT*7 + i; 
			if (pUserItem->bMatchUser==true) return USER_STATE_COUNT*6+i;
			if (pUserItem->uConnection==CM_FRIEND) return USER_STATE_COUNT*2+i;
			if (pUserInfo->bGameMaster>=2) return USER_STATE_COUNT*4+i;
			if (pUserInfo->bGameMaster==1) return USER_STATE_COUNT*5+i;
			if (pUserInfo->bMember>=2) return USER_STATE_COUNT*3+i;
			if (pUserInfo->bMember==1) return USER_STATE_COUNT*1+i;
			return i;
		}
	}

	return 0;
}

//������������
bool CHappyGoDlg::SetSortPos(UINT uSortPos[MAX_SORT_INDEX])
{
	CopyMemory(m_uSortPos,uSortPos,sizeof(m_uSortPos));
	return true;
}

//��ȡ��������
UINT * CHappyGoDlg::GetSortPos(UINT uSortPos[MAX_SORT_INDEX])
{
	if (uSortPos!=NULL) CopyMemory(uSortPos,m_uSortPos,sizeof(uSortPos));
	return m_uSortPos;
}

//�������Ϣ
void CHappyGoDlg::OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult) 
{
	NM_LISTVIEW * pNMListView=(NM_LISTVIEW *)pNMHDR;

	if (m_pItemKind!=NULL)
	{
		//��������
		ListSortInfoStruct SortInfo;
		SortInfo.bAsc=m_bAsc;
		SortInfo.pListCtrl=this;
		SortInfo.uSubItem=pNMListView->iSubItem;
		SortInfo.uItemKind=m_pItemKind[pNMListView->iSubItem];
		SortItems(SortFun,(LPARAM)&SortInfo);

		//��������
		*pResult=0;
		m_bAsc=!m_bAsc;
	}

	return;
}

//���к���
int CALLBACK CHappyGoDlg::SortFun(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList)
{
	ListSortInfoStruct * pSortInfo=(ListSortInfoStruct*)lParamList;

	//��������
	UserItemStruct * pUserItem1=(UserItemStruct *)lParam1;
	UserItemStruct * pUserItem2=(UserItemStruct *)lParam2;

	//�������
	switch (pSortInfo->uItemKind)
	{
	case IK_NAME:			//�û���
		{
			if (pUserItem1->uSortIndex!=pUserItem2->uSortIndex)
			{
				UINT uSortPos1=pSortInfo->pListCtrl->m_uSortPos[pUserItem1->uSortIndex];
				UINT uSortPos2=pSortInfo->pListCtrl->m_uSortPos[pUserItem2->uSortIndex];
				return uSortPos1<uSortPos2;
			}

			int iCompRes=strcmp(pUserItem1->GameUserInfo.nickName,pUserItem2->GameUserInfo.nickName);
			return (pSortInfo->bAsc)?iCompRes:-iCompRes;
		}
	case IK_USERID:			//�û�ID
		{
			bool bBiger=pUserItem1->GameUserInfo.dwUserID>pUserItem2->GameUserInfo.dwUserID;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_DESKNO:			//����
		{
			bool bBiger=pUserItem1->GameUserInfo.bDeskNO>pUserItem2->GameUserInfo.bDeskNO;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_MONEY:			//���
	case IK_WEALTH_LEVEL:	//�Ƹ�����
		{
			bool bBiger=pUserItem1->GameUserInfo.i64Money>pUserItem2->GameUserInfo.i64Money;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_POINT:			//����
	case IK_LEVEL:			//���ֵȼ�
		{
			bool bBiger=pUserItem1->GameUserInfo.dwPoint>pUserItem2->GameUserInfo.dwPoint;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_CHARM:			//����
	case IK_CHARM_LEVEL:	//�����ȼ�
		{
			bool bBiger=pUserItem1->GameUserInfo.dwFascination>pUserItem2->GameUserInfo.dwFascination;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_SEX:			//�Ա�
		{
			bool bBiger=pUserItem1->GameUserInfo.bBoy>pUserItem2->GameUserInfo.bBoy;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_ALL_COUNT:		//�ܾ���
		{
			UINT uAllCount1=pUserItem1->GameUserInfo.uWinCount+pUserItem1->GameUserInfo.uLostCount+pUserItem1->GameUserInfo.uMidCount;
			UINT uAllCount2=pUserItem2->GameUserInfo.uWinCount+pUserItem2->GameUserInfo.uLostCount+pUserItem2->GameUserInfo.uMidCount;
			bool bBiger=uAllCount1>uAllCount2;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_WINCOUNT:		//ʤ��
		{
			bool bBiger=pUserItem1->GameUserInfo.uWinCount>pUserItem2->GameUserInfo.uWinCount;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_LOSTCOUNT:		//���
		{
			bool bBiger=pUserItem1->GameUserInfo.uLostCount>pUserItem2->GameUserInfo.uLostCount;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_MIDCOUNT:		//�;�
		{
			bool bBiger=pUserItem1->GameUserInfo.uMidCount>pUserItem2->GameUserInfo.uMidCount;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_CUT_PERCENT:	//������
		{
			UINT uAllCount1=pUserItem1->GameUserInfo.uWinCount+pUserItem1->GameUserInfo.uLostCount+pUserItem1->GameUserInfo.uMidCount;
			UINT uAllCount2=pUserItem2->GameUserInfo.uWinCount+pUserItem2->GameUserInfo.uLostCount+pUserItem2->GameUserInfo.uMidCount;
			double dCutPencent1=0.0,dCutPencent2=0.0;
			if (uAllCount1!=0) dCutPencent1=(double)pUserItem1->GameUserInfo.uCutCount/(double)uAllCount1;
			if (uAllCount2!=0) dCutPencent2=(double)pUserItem2->GameUserInfo.uCutCount/(double)uAllCount2;
			bool bBiger=dCutPencent1>dCutPencent2;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_WATCH:	//�Թ۶���
		{
			return 0;
		}
	default:	//Ĭ������
		{
			CString strItem1,strItem2;
			LVFINDINFO Info;
			Info.flags=LVFI_PARAM;
			Info.lParam=lParam1;
			int iIndex=-1;
			if ((iIndex=pSortInfo->pListCtrl->FindItem(&Info))!=-1)
				strItem1=pSortInfo->pListCtrl->GetItemText(iIndex,pSortInfo->uSubItem);
			Info.lParam=lParam2;
			if ((iIndex=pSortInfo->pListCtrl->FindItem(&Info))!=-1) 
				strItem2=pSortInfo->pListCtrl->GetItemText(iIndex,pSortInfo->uSubItem);
			int iCompRes=strcmp(strItem1,strItem2);
			return (pSortInfo->bAsc)?iCompRes:-iCompRes;
		}
	}
	return true;
}

//�ؼ���
void CHappyGoDlg::PreSubclassWindow()
{
	m_ListHeader.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());
	__super::PreSubclassWindow();
}

//�滭����
void CHappyGoDlg::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType==ODT_LISTVIEW)
	{
		//��ȡ����
		UINT iWidth=0;
		TCHAR szBuffer[30];
		memset(szBuffer,0,sizeof(szBuffer));
		CDC * pDcItem=CDC::FromHandle(lpDrawItemStruct->hDC);
		/// �����ڴ滺��DC
		CBitmap BufBmp;
		CDC dc;
		CDC *pDC = &dc;
		CRect rc;
		GetClientRect(&rc);
		BufBmp.CreateCompatibleBitmap(pDcItem, rc.Width(), rc.Height());
		dc.CreateCompatibleDC(pDcItem);
		CBitmap * pOldBmp=dc.SelectObject(&BufBmp);

		/// ѡ������
		CFont *pDefaultFont = pDcItem->GetCurrentFont();
		CFont *pOldFont = dc.SelectObject(pDefaultFont);
		UserItemStruct * pUserItem=(UserItemStruct *)GetItemData(lpDrawItemStruct->itemID);
		if (NULL == pUserItem)
		{
			dc.SelectObject(pOldBmp);
			dc.SelectObject(pOldFont); 		
			BufBmp.DeleteObject();
			return ;
		}
		//������ɫ
		COLORREF crTextColor,crBackColor;
		bool bSelect=(lpDrawItemStruct->itemState&ODS_SELECTED);//||(lpDrawItemStruct->itemState&ODS_FOCUS);
		GetDrawColor(crTextColor,crBackColor,pUserItem,0,bSelect);
		pDC->SetBkColor(crBackColor);
		pDC->SetTextColor(crTextColor);
		pDC->SetBkMode(TRANSPARENT);
		//�滭��Ϣ
		CSize TextSize;
		pDC->FillSolidRect(lpDrawItemStruct->rcItem.left,
			lpDrawItemStruct->rcItem.top,
			lpDrawItemStruct->rcItem.right-lpDrawItemStruct->rcItem.left,
			lpDrawItemStruct->rcItem.bottom-lpDrawItemStruct->rcItem.top,
			crBackColor);
		CImageList *imgList=GetImageList(LVSIL_SMALL);
		if(imgList)
		{
			int IconID=GetStateImageIndex(pUserItem);
			POINT pt;
			pt.x=lpDrawItemStruct->rcItem.left;
			pt.y=lpDrawItemStruct->rcItem.top+1;
			imgList->Draw(pDC,IconID,pt,ILD_TRANSPARENT);
		}
		if((lpDrawItemStruct->itemState&ODS_SELECTED)||(lpDrawItemStruct->itemState&ODS_FOCUS))
		{//�����
			int mode=pDC->SetBkMode(TRANSPARENT);
			CPen penBlack,*pOldPen;
			//penBlack.DeleteObject();
			if(lpDrawItemStruct->itemState&ODS_SELECTED)
			{
				if(penBlack.CreatePen(PS_DOT,1, m_bkColor));// RGB(255,255,255)));
					pOldPen=pDC->SelectObject(&penBlack);
			}
			else
			{
				if(penBlack.CreatePen(PS_DOT,1, m_FocusbkColor));// RGB(0,0,0)));
					pOldPen=pDC->SelectObject(&penBlack);
			}
			pDC->SelectStockObject(NULL_BRUSH);
			pDC->Rectangle(lpDrawItemStruct->rcItem.left,lpDrawItemStruct->rcItem.top,
				lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom);	
			pDC->SetBkMode(mode);
			penBlack.DeleteObject();

		}
		for (int i=0;i<m_ListHeader.GetItemCount();i++)
		{
			int iTemp=GetColumnWidth(i);
			TextSize=pDC->GetTextExtent(szBuffer,lstrlen(szBuffer));
			GetItemText(lpDrawItemStruct->itemID,i,szBuffer,sizeof(szBuffer));
			CRect TextRect(lpDrawItemStruct->rcItem.left+iWidth+5,lpDrawItemStruct->rcItem.top,lpDrawItemStruct->rcItem.left+iWidth+iTemp,lpDrawItemStruct->rcItem.bottom);
			if(i==0)
				TextRect.left+=32;

			pDC->DrawText(szBuffer,lstrlen(szBuffer),&TextRect,((i==0)?DT_LEFT:DT_CENTER)|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
			iWidth+=iTemp;
		}

		//�滭��Ļ
		pDcItem->BitBlt(
			lpDrawItemStruct->rcItem.left,
			lpDrawItemStruct->rcItem.top,
			lpDrawItemStruct->rcItem.right-lpDrawItemStruct->rcItem.left, 
			lpDrawItemStruct->rcItem.bottom-lpDrawItemStruct->rcItem.top,
			&dc,
			lpDrawItemStruct->rcItem.left,
			lpDrawItemStruct->rcItem.top,
			SRCCOPY);
		dc.SelectObject(pOldBmp);
		dc.SelectObject(pOldFont); 	
		dc.DeleteDC();
		BufBmp.DeleteObject();

	}
	return;
}

void CHappyGoDlg::SetColor(COLORREF bkcr, COLORREF textcr, COLORREF focusbkcr, COLORREF focustextcr)
{
	m_bkColor = bkcr;
	m_TextColor = textcr;
	m_FocusbkColor = focusbkcr;
	m_FocusTextColor = focustextcr;
	SetBkColor(m_bkColor);
	SetTextBkColor(m_bkColor);
	SetTextColor(m_TextColor);
}

//��ȡ��ɫ
bool CHappyGoDlg::GetDrawColor(COLORREF & crTextColor, COLORREF & crBackColor, UserItemStruct * pUserItem, UINT uRow, bool bSelect)
{
	if (bSelect)
	{
		crTextColor=m_FocusTextColor;
		crBackColor=m_FocusbkColor;
	}
	else
	{
		crTextColor=m_TextColor;
		crBackColor=m_bkColor;
	}
	if (pUserItem!=NULL)
	{
		CString stime;
		stime.Format("%d",CTime::GetCurrentTime());
		if(pUserItem->GameUserInfo.iVipTime>atol(stime))
			crTextColor=RGB(255,0,0);
	}
	return true;
}

//��������
BOOL CHappyGoDlg::OnEraseBkgnd(CDC * pDC)
{
	CRect DrawRect,HeadRect;
	pDC->GetClipBox(&DrawRect);
	if (m_ListHeader.GetSafeHwnd()!=NULL)
	{
		m_ListHeader.GetWindowRect(&HeadRect);
		if (DrawRect.top<HeadRect.Height()) DrawRect.top=HeadRect.Height();
	}
	COLORREF crTextColor,crBackColor;
	GetDrawColor(crTextColor,crBackColor,NULL,0,false);
	pDC->FillSolidRect(&DrawRect,crBackColor);
	return TRUE;
}

/****************************************************************************************************/

void CHappyGoDlg::LoadSkin(void)
{
	SetBkColor(CGameImageLink::m_ListResource.m_crBkColor);
	SetTextBkColor(CGameImageLink::m_ListResource.m_crBkColor);
	SetTextColor(CGameImageLink::m_ListResource.m_crNormalTxColor);
	SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_INFOTIP);
	SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);

	//����ͼƬ
	if (m_UserStateList.GetSafeHandle()==NULL)
	{
		m_UserStateList.DeleteImageList();
		//װ��λͼ
		BITMAP BmpInfo;
		CGameImage	UserStateBmp;
		//CBitmap m_NewUserStateBmp;
		AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
		UserStateBmp.SetLoadInfo(".\\image\\user_state.bmp",true);;
		AfxSetResourceHandle(GetModuleHandle(NULL));
		//UserStateBmp.GetBitmap(&BmpInfo);
		BmpInfo.bmWidth=UserStateBmp.GetWidth();
		BmpInfo.bmHeight=UserStateBmp.GetHeight();

		//����λͼ
		CDC TargetDC, * pDC=GetDC();
		TargetDC.CreateCompatibleDC(pDC);
		/// {{Modified by Zxd 20100715 ������������Ϊ��Ա���������ͷ�
		if (m_NewUserStateBmp.m_hObject != NULL)
		{
			m_NewUserStateBmp.DeleteObject();
		}
		/// Modified by Zxd 20100715 ������������Ϊ��Ա���������ͷ�}}
		m_NewUserStateBmp.CreateCompatibleBitmap(pDC,BmpInfo.bmWidth,BmpInfo.bmHeight);
		CBitmap * pOldBmp=TargetDC.SelectObject(&m_NewUserStateBmp);
		TargetDC.FillSolidRect(0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,CGameImageLink::m_ListResource.m_crBkColor);
		AFCStretchImage(&TargetDC,0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,UserStateBmp,0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,RGB(255,255,255));
		TargetDC.SelectObject(pOldBmp);
		TargetDC.DeleteDC();

		//������Դ��
		m_UserStateList.Create(USER_STATE_WIDTH,USER_STATE_HEIGHT,ILC_COLOR16,0,0);
		m_UserStateList.Add(&m_NewUserStateBmp,RGB(0,0,255));

		//������Դ
		UserStateBmp.Detach();
		ReleaseDC(pDC);
		TargetDC.DeleteDC();
	}
	SetImageList(&m_UserStateList,LVSIL_SMALL);
}
