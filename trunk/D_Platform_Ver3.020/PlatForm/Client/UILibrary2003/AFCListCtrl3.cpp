//�����û��б�
#include "Stdafx.h"
#include "Resource.h"
#include "AFCResource.h"
#include "AFCListCtrl.h"
#include "AFCFunction.h"
#include "GameFrameDlg.h"
//using namespace AFC;
//using namespace AFC::AFCListCtrl;
//using namespace AFC::AFCResource;

BEGIN_MESSAGE_MAP(CComBodyClass, CHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CHappyGoDlg, CListCtrl)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
END_MESSAGE_MAP()

//�û�״̬ͼ
CImageList				CHappyGoDlg::m_UserStateList;				//�û�״̬�б�
UINT					CHappyGoDlg::m_uSortPos[MAX_SORT_INDEX];	//��������
bool					CHappyGoDlg::m_bInitStatic=false;			//�Ƿ��ʼ��

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
void CComBodyClass::SetBkImage(TCHAR path[],TCHAR path2[],COLORREF color,COLORREF tcolor)
{
	m_bkimg.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_bkimg2.SetLoadInfo(path2,CGameImageLink::m_bAutoLock);
	m_color=color;
	m_textcolor=tcolor;
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
	BufferDC.FillSolidRect(&ClientRect,RGB(255,255,255));

	HDITEM HDItem;
	CRect rcItemRect;
	TCHAR szBuffer[30];

	
	for (int i=0;i<GetItemCount();i++)
	{
		//��ȡ��Ϣ
		HDItem.pszText=szBuffer;
		HDItem.mask=HDI_TEXT|HDI_FORMAT;
		HDItem.cchTextMax=sizeof(szBuffer)/sizeof(szBuffer[0])-1;
		GetItemRect(i,&rcItemRect);
		GetItem(i,&HDItem);
		//�滭����
		if(i%2==0)
			m_bkimg.StretchBlt(BufferDC.GetSafeHdc(),rcItemRect);
		else
			m_bkimg2.StretchBlt(BufferDC.GetSafeHdc(),rcItemRect);
		//ImageHandle.StretchBlt(BufferDC.GetSafeHdc(),rcItemRect);
		//TCHAR sz[100];
		//wsprintf(sz,"%d",rcItemRect.Height());
		//AfxMessageBox(sz);

		//BufferDC.Draw3dRect(&rcItemRect,DFC_BUTTON,DFCS_BUTTONPUSH);
		CPen pen,*oldpen;
		pen.CreatePen(PS_SOLID,3,m_color);
		oldpen=BufferDC.SelectObject(&pen);
		BufferDC.MoveTo(rcItemRect.left,rcItemRect.bottom);
		BufferDC.LineTo(rcItemRect.right,rcItemRect.bottom);
		if(i!=(GetItemCount()-1))
			BufferDC.LineTo(rcItemRect.right,rcItemRect.top);
		::DeleteObject(BufferDC.SelectObject(*oldpen));

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
	BufferDC.DeleteDC();
	BufferBmp.DeleteObject();
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
	m_pGetOrderFun=NULL;
	m_pItemKind=pItemKind;
	if (m_bInitStatic==false)
	{
		m_bInitStatic=true;
		for (UINT i=0;i<sizeof(m_uSortPos)/sizeof(m_uSortPos[0]);i++) m_uSortPos[i]=i;
	}
}

//��������
CHappyGoDlg::~CHappyGoDlg()
{
}

//��ʼ��
void CHappyGoDlg::InitListCtrl(GetOrderName * pOrderFunction, UINT uComType)
{
	m_pGetOrderFun=pOrderFunction;
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
	case IK_NAME:			//�û�����
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
//	case IK_USERID:			//ID ����
//		{
//			bool bBiger=pUserItem1->GameUserInfo.dwUserID>pUserItem2->GameUserInfo.dwUserID;
//			return (pSortInfo->bAsc)?bBiger:!bBiger;
//		}
//	case IK_ACCID:			//ACC ����
//		{
//			bool bBiger=pUserItem1->GameUserInfo.dwAccID>pUserItem2->GameUserInfo.dwAccID;
//			return (pSortInfo->bAsc)?bBiger:!bBiger;
//		}
	case IK_POINT:			//����
		{
			bool bBiger=pUserItem1->GameUserInfo.dwPoint>pUserItem2->GameUserInfo.dwPoint;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
//	case IK_MOBILENUM:		//�ֻ�����
//		{
//			bool bBiger=pUserItem1->GameUserInfo.dwMobileNum>pUserItem2->GameUserInfo.dwMobileNum;
//			return (pSortInfo->bAsc)?bBiger:!bBiger;
//		}
	case IK_LOGOID:			//ͷ�� ID ����
		{
			bool bBiger=pUserItem1->GameUserInfo.bLogoID>pUserItem2->GameUserInfo.bLogoID;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_WINCOUNT:		//ʤ����Ŀ
		{
			bool bBiger=pUserItem1->GameUserInfo.uWinCount>pUserItem2->GameUserInfo.uWinCount;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_LOSTCOUNT:		//����Ŀ
		{
			bool bBiger=pUserItem1->GameUserInfo.uLostCount>pUserItem2->GameUserInfo.uLostCount;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_CUTCOUNT:		//ǿ����Ŀ
		{
			bool bBiger=pUserItem1->GameUserInfo.uCutCount>pUserItem2->GameUserInfo.uCutCount;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_MIDCOUNT:		//�;���Ŀ
		{
			bool bBiger=pUserItem1->GameUserInfo.uMidCount>pUserItem2->GameUserInfo.uMidCount;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
//	case IK_CLASSNAME:		//��Ϸ����
//		{
//			int iCompRes=strcmp(pUserItem1->GameUserInfo.szClassName,pUserItem2->GameUserInfo.szClassName);
//			return (pSortInfo->bAsc)?iCompRes:-iCompRes;
//		}
	case IK_DESKNO:			//��Ϸ����
		{
			bool bBiger=pUserItem1->GameUserInfo.bDeskNO>pUserItem2->GameUserInfo.bDeskNO;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_DESKSTATION:	//����λ��
		{
			bool bBiger=pUserItem1->GameUserInfo.bDeskStation>pUserItem2->GameUserInfo.bDeskStation;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_USERSTATE:		//�û�״̬
		{
			bool bBiger=pUserItem1->GameUserInfo.bUserState>pUserItem2->GameUserInfo.bUserState;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_MEMBER:			//��Ա�ȼ�
		{
			bool bBiger=pUserItem1->GameUserInfo.bMember>pUserItem2->GameUserInfo.bMember;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_GAMEMASTER:		//����ȼ�
		{
			bool bBiger=pUserItem1->GameUserInfo.bGameMaster>pUserItem2->GameUserInfo.bGameMaster;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_USERIP:			//��¼IP��ַ
		{
			bool bBiger=pUserItem1->GameUserInfo.dwUserIP>pUserItem2->GameUserInfo.dwUserIP;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_ALL_COUNT:		//�ܾ���
		{
			UINT uAllCount1=pUserItem1->GameUserInfo.uWinCount+pUserItem1->GameUserInfo.uLostCount+pUserItem1->GameUserInfo.uCutCount+pUserItem1->GameUserInfo.uMidCount;
			UINT uAllCount2=pUserItem2->GameUserInfo.uWinCount+pUserItem2->GameUserInfo.uLostCount+pUserItem2->GameUserInfo.uCutCount+pUserItem2->GameUserInfo.uMidCount;
			bool bBiger=uAllCount1>uAllCount2;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
		}
	case IK_CUT_PERCENT:	//������
		{
			UINT uAllCount1=pUserItem1->GameUserInfo.uWinCount+pUserItem1->GameUserInfo.uLostCount+pUserItem1->GameUserInfo.uCutCount+pUserItem1->GameUserInfo.uMidCount;
			UINT uAllCount2=pUserItem2->GameUserInfo.uWinCount+pUserItem2->GameUserInfo.uLostCount+pUserItem2->GameUserInfo.uCutCount+pUserItem2->GameUserInfo.uMidCount;
			double dCutPencent1=0.0,dCutPencent2=0.0;
			if (uAllCount1!=0) dCutPencent1=(double)pUserItem1->GameUserInfo.uCutCount/(double)uAllCount1;
			if (uAllCount2!=0) dCutPencent2=(double)pUserItem2->GameUserInfo.uCutCount/(double)uAllCount2;
			bool bBiger=dCutPencent1>dCutPencent2;
			return (pSortInfo->bAsc)?bBiger:!bBiger;
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
		CDC * pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
		UserItemStruct * pUserItem=(UserItemStruct *)GetItemData(lpDrawItemStruct->itemID);

		//������ɫ
		COLORREF crTextColor,crBackColor;
		bool bSelect=(lpDrawItemStruct->itemState&ODS_SELECTED)||(lpDrawItemStruct->itemState&ODS_FOCUS);
		GetDrawColor(crTextColor,crBackColor,pUserItem,0,bSelect);
		pDC->SetBkColor(crBackColor);
		pDC->SetTextColor(crTextColor);
		//�滭��Ϣ
		CSize TextSize;
		for (int i=0;i<m_ListHeader.GetItemCount();i++)
		{
			int iTemp=GetColumnWidth(i);
			TextSize=pDC->GetTextExtent(szBuffer,lstrlen(szBuffer));
			GetItemText(lpDrawItemStruct->itemID,i,szBuffer,sizeof(szBuffer));
			CRect TextRect(lpDrawItemStruct->rcItem.left+iWidth,lpDrawItemStruct->rcItem.top,lpDrawItemStruct->rcItem.left+iWidth+iTemp,lpDrawItemStruct->rcItem.bottom);
/*			CGameImage image;
			image.SetLoadInfo(".\\image\\client\\userlist.bmp",CGameImageLink::m_bAutoLock);
			CGameImageHelper help(&image);
			help.BitBlt(pDC->GetSafeHdc(),TextRect.left,TextRect.top);
			if(bSelect)
				PatBlt(pDC->GetSafeHdc(),TextRect.left,TextRect.top,TextRect.Width(),TextRect.Height(),DSTINVERT);
*/
			pDC->DrawText(szBuffer,lstrlen(szBuffer),&TextRect,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
			if (TextSize.cx<iTemp) 
			{
				pDC->FillSolidRect(lpDrawItemStruct->rcItem.left+iWidth+TextSize.cx,lpDrawItemStruct->rcItem.top,
					iTemp-TextSize.cx,lpDrawItemStruct->rcItem.bottom-lpDrawItemStruct->rcItem.top,crBackColor);
			}
			iWidth+=iTemp;
		}
	}
	return;
}

//��ȡ��ɫ
bool CHappyGoDlg::GetDrawColor(COLORREF & crTextColor, COLORREF & crBackColor, UserItemStruct * pUserItem, UINT uRow, bool bSelect)
{
	if (pUserItem!=NULL)
	{
		if (bSelect)
		{
			crTextColor=CGameImageLink::m_ListResource.m_crFousTxColor;
			crBackColor=CGameImageLink::m_ListResource.m_crFousBkColor;
		}
		else
		{
			crTextColor=CGameImageLink::m_ListResource.m_crNormalTxColor;
			crBackColor=CGameImageLink::m_ListResource.m_crBkColor;
		}
	}
	else
	{
		if (bSelect)
		{
			crTextColor=CGameImageLink::m_ListResource.m_crFousTxColor;
			crBackColor=CGameImageLink::m_ListResource.m_crFousBkColor;
		}
		else
		{
			crTextColor=CGameImageLink::m_ListResource.m_crNormalTxColor;
			crBackColor=CGameImageLink::m_ListResource.m_crBkColor;
		}
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
	pDC->FillSolidRect(&DrawRect,CGameImageLink::m_ListResource.m_crBkColor);
	return TRUE;
}

/****************************************************************************************************/

void CHappyGoDlg::LoadSkin(void)
{
		//������Ϣ
	CString s=CINIFile::GetAppPath ();/////����·��
	CINIFile f( s + "skin.ini");
	CString key=Glb().m_skin;
	CString skinfolder;
	skinfolder=f.GetKeyVal(key,"skinfolder","image\\client\\skin0\\");
	int r,g,b;
	r=f.GetKeyVal(key,"listtextclr",255);
	g=f.GetKeyVal(key,"listtextclg",255);
	b=f.GetKeyVal(key,"listtextclb",255);
	CGameImageLink::m_ListResource.m_crNormalTxColor=RGB(r,g,b);

	r=f.GetKeyVal(key,"listbkclr",233);
	g=f.GetKeyVal(key,"listbkclg",233);
	b=f.GetKeyVal(key,"listbkclb",233);
	CGameImageLink::m_ListResource.m_crBkColor=RGB(r,g,b);

	r=f.GetKeyVal(key,"listheadr",0);
	g=f.GetKeyVal(key,"listheadg",59);
	b=f.GetKeyVal(key,"listheadb",108);
	COLORREF color=RGB(r,g,b);

	TCHAR path[MAX_PATH],path1[MAX_PATH];
	wsprintf(path,"%sgame_list_head1.bmp",skinfolder);
	wsprintf(path1,"%sgame_list_head2.bmp",skinfolder);
	m_ListHeader.SetBkImage(path,path1,color,CGameImageLink::m_ListResource.m_crNormalTxColor);

	SetBkColor(CGameImageLink::m_ListResource.m_crBkColor);
	SetTextBkColor(CGameImageLink::m_ListResource.m_crBkColor);
	SetTextColor(CGameImageLink::m_ListResource.m_crNormalTxColor);
	SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_INFOTIP);
	SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);

	//����ͼƬ
	if (m_UserStateList.GetSafeHandle()==NULL)
	{
		//װ��λͼ
		BITMAP BmpInfo;
		CGameImage	UserStateBmp;
		CBitmap NewUserStateBmp;
		AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
		UserStateBmp.SetLoadInfo(".\\image\\client\\alluse\\user_state.bmp",true);;
		AfxSetResourceHandle(GetModuleHandle(NULL));
		//UserStateBmp.GetBitmap(&BmpInfo);
		BmpInfo.bmWidth=UserStateBmp.GetWidth();
		BmpInfo.bmHeight=UserStateBmp.GetHeight();

		//����λͼ
		CDC TargetDC, * pDC=GetDC();
		TargetDC.CreateCompatibleDC(pDC);
		NewUserStateBmp.CreateCompatibleBitmap(pDC,BmpInfo.bmWidth,BmpInfo.bmHeight);
		CBitmap * pOldBmp=TargetDC.SelectObject(&NewUserStateBmp);
		TargetDC.FillSolidRect(0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,CGameImageLink::m_ListResource.m_crBkColor);
		AFCStretchImage(&TargetDC,0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,UserStateBmp,0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,RGB(255,255,255));
		TargetDC.SelectObject(pOldBmp);
		TargetDC.DeleteDC();

		//������Դ��
		m_UserStateList.Create(USER_STATE_WIDTH,USER_STATE_HEIGHT,ILC_COLOR16,0,0);
		m_UserStateList.Add(&NewUserStateBmp,RGB(0,0,255));

		//������Դ
		UserStateBmp.Detach();
		NewUserStateBmp.Detach();
		ReleaseDC(pDC);
	}
	SetImageList(&m_UserStateList,LVSIL_SMALL);
}
//
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
//
void CComBodyClass::SetBkImage(TCHAR path[],TCHAR path2[],COLORREF color,COLORREF tcolor, int type)
{
	m_bkimg.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_bkimg2.SetLoadInfo(path2,CGameImageLink::m_bAutoLock);
	m_color=color;
	m_textcolor=tcolor;
	m_Type = type;
}