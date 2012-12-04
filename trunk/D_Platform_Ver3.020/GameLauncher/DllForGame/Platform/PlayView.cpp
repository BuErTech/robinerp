#include "../stdafx.h"
#include "PlayView.h"
#include "AFCFunction.h"
#include "AFCLogoResource.h"
//#include "GameFrameDlg.h"
//#include "..\\��Դ.2003\\Resource.h"

//using namespace AFC;
//using namespace AFC::AFCResource;

//Avatarϵͳ���
#define BzGetAvatarIndex(obj) \
	GetAvatarIndex((obj)->GameUserInfo.bLogoID,(obj)->GameUserInfo.i64Bank,(obj)->GameUserInfo.i64Money,\
	(obj)->GameUserInfo.dwPoint,m_pGameInfo->uNameID,m_pGameInfo->uComType)
 
BEGIN_MESSAGE_MAP(CUsingSkinClass, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	//ON_WM_MOUSEMOVE()
	//ON_MESSAGE(PROP_SELECT_CURSOR, OnPropSelectCursorChange)
//	ON_WM_RBUTTONDOWN()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//��̬����
CGameImage				CUsingSkinClass::m_PicAgree;							//ͬ��ͼ��
CGameImage				CUsingSkinClass::m_PicTimer;							//ʱ������
CGameImage              CUsingSkinClass::m_PicTimerFrame;                       //ʱ���
CGameImage				CUsingSkinClass::m_PicTop;								//���ͼƬ
CGameImage				CUsingSkinClass::m_PicBottom;							//���ͼƬ
CGameImage				CUsingSkinClass::m_PicLeft;								//���ͼƬ
CGameImage				CUsingSkinClass::m_PicRight;							//���ͼƬ
CGameImage				CUsingSkinClass::m_PicTopRight;							//���ͼƬ
CGameImage				CUsingSkinClass::m_PicTopLeft;							//���ͼƬ
CGameImage				CUsingSkinClass::m_PicBottomRight;						//���ͼƬ
CGameImage				CUsingSkinClass::m_PicBottomLeft;						//���ͼƬ
//CGameImage				CUsingSkinClass::m_iUserFrame;						//���ͼƬ

/*******************************************************************************************************/

//���캯��
CUsingSkinClass::CUsingSkinClass(bool bDoubleBuf, int iBufferColor, CNewCDC * pDrawInfoPtr) 
	: m_bDoubleBuf((pDrawInfoPtr==NULL)?bDoubleBuf:false), m_iBufferColor(iBufferColor), m_pDrawInfoPtr(pDrawInfoPtr)
{
	//���ñ���
	m_bReDraw=true;
	memset(m_uTime,0,sizeof(m_uTime));
	memset(m_PtLogo,0,sizeof(m_PtLogo));
	memset(m_PtName,0,sizeof(m_PtName));
	memset(m_PtTime,0,sizeof(m_PtTime));
	memset(m_PtAgree,0,sizeof(m_PtAgree));
	memset(&m_PicInfo,0,sizeof(m_PicInfo));
	memset(m_pUserItem,0,sizeof(m_pUserItem));

	//����λͼ
	HINSTANCE hDllHandle=GetModuleHandle(FACE_DLL_NAME);

	m_pLoadGif= NULL;
	m_iGifDeskStation = 0;

	m_bkColor=RGB(4,56,106);

	m_bkBrush=NULL;
//	m_hCursor = NULL;	//add by zxj 2008/04/09

}
//����frameͼƬ
void CUsingSkinClass::SetFrameImage(TCHAR *path)
{
	m_iUserFrame.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
}

//��������
CUsingSkinClass::~CUsingSkinClass()
{
	m_iGifDeskStation = 0;
	DeleteObject(m_bkBrush);
}

//������Ϣ
int CUsingSkinClass::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct) == -1) return -1;

	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	m_PicAgree.SetLoadInfo(".\\image\\agree.bmp",CGameImageLink::m_bAutoLock);
	m_PicTimer.SetLoadInfo(".\\image\\game_time.bmp",CGameImageLink::m_bAutoLock);
	m_PicTimerFrame.SetLoadInfo(".\\image\\clock.bmp",CGameImageLink::m_bAutoLock);
	m_iUserFrame.SetLoadInfo(".\\image\\logoframe.bmp",CGameImageLink::m_bAutoLock);

	//�����߳�
	if (m_pDrawInfoPtr!=NULL) m_pDrawInfoPtr->CreateDrawThread();

	AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
	m_showcoredlg.Create(IDD_SHOWSCORE,this);

	m_ContestInfoWnd.Create(IDD_CONTESTINFO, this);
	AfxSetResourceHandle(GetModuleHandle(NULL));
	m_showcoredlg.ShowWindow(SW_SHOW);
	//m_ContestInfoWnd.MoveWindow(0, 0, 100, 100, true);
	
	if (m_pGameInfo->dwRoomRule & GRR_CONTEST)
	{
		m_ContestInfoWnd.ShowWindow(SW_SHOW);
	}



	InitGif();

	m_bkBrush=CreateSolidBrush(m_bkColor);


	//OutputDebugString("CUsingSkinClass::OnCreate 7");
	return 0;
}

//�滭����
void CUsingSkinClass::OnPaint()
{
	if(!this->m_hWnd)
		return;
	CPaintDC dc(this);

	if (m_pDrawInfoPtr==NULL)
	{
		CRect ClientRect;
		GetClientRect(&ClientRect);
		if ((m_bDoubleBuf==true)&&(m_PicBuffer.GetSafeHandle()!=NULL))
		{
			//�������� DC
			CDC DCBack;
			DCBack.CreateCompatibleDC(&dc);
			DCBack.SelectObject(&m_PicBuffer);
			//�ػ�������
			if (m_bReDraw==true)
			{
				m_bReDraw=false;
				DCBack.SetBkMode(TRANSPARENT);
				DCBack.SelectObject(&CGameImageLink::m_HFont);
				if(m_bShowRight)
				{
					CGameImageHelper	handle(&m_bkimgs);
					CRect rect;
					GetClientRect(&rect);
					MapWindowPoints(GetParent(),(LPPOINT)&rect,2);
					handle.StretchBlt(DCBack.GetSafeHdc(),0, 0,rect.Width(),rect.Height(),rect.left,rect.top,rect.Width(),rect.Height());
				}
				else
				{
					CGameImageHelper handle(&m_bkimgh);
					CRect rect;
					GetClientRect(&rect);
					MapWindowPoints(GetParent(),(LPPOINT)&rect,2);
					handle.StretchBlt(DCBack.GetSafeHdc(),0, 0,rect.Width(),rect.Height(),rect.left,rect.top,rect.Width(),rect.Height());
				}

				DrawViewFace(&DCBack,ClientRect.Width(),ClientRect.Height());
				//if(m_ShowType==1 && m_bShowRight)
				//	((CLoveSendClassInExe *)GetParent())->DrawCurUserInfo(&DCBack);
			}			
			//�滭����Ļ
			CRect ClipRect;
			dc.GetClipBox(&ClipRect);
			dc.BitBlt(ClipRect.left,ClipRect.top,ClipRect.right,ClipRect.bottom,&DCBack,ClipRect.left,ClipRect.top,SRCCOPY);
			DCBack.DeleteDC();
		}
		else
		{
			dc.SetBkMode(TRANSPARENT);
			dc.SelectObject(&CGameImageLink::m_VFont);
			DrawViewFace(&dc,ClientRect.Width(),ClientRect.Height());
		}
	}
	return;
}

//�滭���溯��
void CUsingSkinClass::DrawViewFace(CDC * pDC, int iWidth, int iHeight)
{
	pDC->FillSolidRect(0,0,iWidth,iHeight,RGB(255,255,0));
	return;
}

//�滭����
void CUsingSkinClass::DrawBackPic(CDC * pDC, DrawMode Mode, int iWidth, int iHeight)
{
	switch (Mode)
	{
	case DM_ELONGATE:	//����
		{
			CGameImageHelper BackHandle(&m_PicGameView);
			BackHandle.StretchBlt(pDC->GetSafeHdc(),0,0,iWidth,iHeight,0,0,BackHandle.GetWidth(),BackHandle.GetHeight());
			return;
		}
	case DM_CENTENT:	//����
		{
			CGameImageHelper BackHandle(&m_PicGameView);
			BackHandle.BitBlt(pDC->GetSafeHdc(),(iWidth-BackHandle.GetWidth())/2,(iHeight-BackHandle.GetHeight())/2);
			return;
		}
	case DM_SPREAD:	//ƽ��
		{
			CGameImageHelper BackHandle(&m_PicGameView);
			for (int i=0;i<iWidth;i+=BackHandle.GetWidth())
			{
				for (int j=0;j<iHeight;j+=BackHandle.GetHeight())
				{
					BackHandle.BitBlt(pDC->GetSafeHdc(),i,j);
				}
			}
			return;
		}
	}
	return;
}

//�滭ͬ���־
void CUsingSkinClass::DrawArgeeFlag(CDC * pDC, int iXPos, int iYPos)
{
	CGameImageHelper ImageHandle(&m_PicAgree);
	CDC destdc;
	destdc.CreateCompatibleDC(pDC);
	destdc.SelectObject(ImageHandle);
	::TransparentBlt(pDC->GetSafeHdc(),iXPos,iYPos,ImageHandle.GetWidth(),ImageHandle.GetHeight(),destdc.GetSafeHdc(),0,0,ImageHandle.GetWidth(),ImageHandle.GetHeight(),destdc.GetPixel(0,0));

	destdc.DeleteDC();

	//AFCStretchImage(pDC,iXPos,iYPos,AGREE_WIDTH,AGREE_HEIGHT,ImageHandle,0,0,AGREE_WIDTH,AGREE_HEIGHT,RGB(0,128,128));
	return;
}

//�滭ʱ��
void CUsingSkinClass::DrawTimer(CDC * pDC, int iXPos, int iYPos, UINT uTime)
{
	if ((uTime!=0L)&&(uTime<100L))
	{
		CGameImageHelper ImageFrameHandle(&m_PicTimerFrame);
		CDC destdc;
		destdc.CreateCompatibleDC(pDC);
		destdc.SelectObject(ImageFrameHandle);
		::TransparentBlt(pDC->GetSafeHdc(),iXPos-12,iYPos-15,ImageFrameHandle.GetWidth(),ImageFrameHandle.GetHeight(),destdc.GetSafeHdc(),0,0,ImageFrameHandle.GetWidth(),ImageFrameHandle.GetHeight(),RGB(255,0,255));

		UINT uFirst=uTime/10L,uLast=uTime%10;
		CGameImageHelper ImageHandle(&m_PicTimer);
		//CDC destdc;
		//destdc.CreateCompatibleDC(pDC);
		destdc.SelectObject(ImageHandle);
		::TransparentBlt(pDC->GetSafeHdc(),iXPos,iYPos,TIME_WIDTH/2,TIME_HEIGHT,destdc.GetSafeHdc(),uFirst*TIME_WIDTH/2,0,TIME_WIDTH/2,TIME_HEIGHT,RGB(0,0,0));
		::TransparentBlt(pDC->GetSafeHdc(),iXPos+15,iYPos,TIME_WIDTH/2,TIME_HEIGHT,destdc.GetSafeHdc(),uLast*TIME_WIDTH/2,0,TIME_WIDTH/2,TIME_HEIGHT,RGB(0,0,0));

		destdc.DeleteDC();
	}
	return;
}
//������ͷ���
void CUsingSkinClass::DrawLogoFrame(CDC * pDC, int iXPos, int iYPos)
{
	return;	// ������ͷ���
	CGameImageHelper help(&m_iUserFrame);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	dc.SelectObject(help);
	::BitBlt(pDC->GetSafeHdc(),iXPos,iYPos,help.GetWidth(),help.GetHeight(),dc.GetSafeHdc(),0,0,SRCCOPY);
	::TransparentBlt(pDC->GetSafeHdc(),iXPos,iYPos,help.GetWidth(),help.GetHeight(),dc.GetSafeHdc(),0,0,help.GetWidth(),help.GetHeight(),dc.GetPixel(0,0));
	dc.DeleteDC();
	return ;
}
//�滭ͷ��
void CUsingSkinClass::DrawLogo(CDC * pDC, UINT uLogoID, int iXPos, int iYPos, bool bLost,long int dwUserID,BOOL bIsSmall,bool bRightTop)
{
	//	return;	// ������ͷ��
	//�滭����ͷ��
	WCHAR wszBuffer[MAX_PATH];	
	TCHAR szBuffer[MAX_PATH];
	memset(wszBuffer,0,sizeof(wszBuffer));
	memset(szBuffer,0,sizeof(szBuffer));

	UserItemStruct *pUserItem = NULL;
	for (int i=0; i<MAX_PEOPLE; i++)
	{

		if (NULL!=m_pUserItem[i] && m_pUserItem[i]->GameUserInfo.dwUserID == dwUserID)
			pUserItem = m_pUserItem[i];
	}


	if (NULL == pUserItem)
		return;

	int iSitGrade = BzGetAvatarIndex(pUserItem);

	static bool bGetDeskClass = false ; 
    static CString strDeskClass  = ""; 
	CString strSex ; 

	if(false == bGetDeskClass)
	{
		CBcfFile  f("GameOption.bcf") ; 

		strDeskClass = f.GetKeyVal("DESK" , "Class" , "3d") ; 

		bGetDeskClass = true ;
	}
	
	if(strDeskClass == "2d" || strDeskClass == "3d")
	{
		Graphics gra(pDC->GetSafeHdc());

		//uLogoID = iLevel ; 

		if (pUserItem->GameUserInfo.bBoy)
		{
			strSex = "boy"  ;   ///< uLogoID ����ǰ����Ϸ�а汾��ȷ���ģ����ڵ�����ͷ���ǰ���ҵȼ�����ʾ������޸���uLogoID��
		}
		else
		{
			strSex = "girl" ; 
		}

		if(bLost)
		{
			Image img(L"..\\image\\log\\LOST_LOGO.png");
			gra.DrawImage(&img,Rect(iXPos+78/2-img.GetWidth()/2,iYPos+108/2-img.GetHeight()/2,img.GetWidth(),img.GetHeight()),0,0,img.GetWidth(),img.GetHeight(),UnitPixel);
		}
		else
		{
			CString szAppPath = CINIFile::GetAppPath();
			szAppPath.SetAt(szAppPath.GetLength()-1, TCHAR('\0'));
			int nPosOfSlash = szAppPath.ReverseFind(TCHAR('\\'));
			CString szAppPathParent = szAppPath.Left(nPosOfSlash);
			
			wsprintf(szBuffer,_T("%s\\image\\log\\%s\\gamelog%d_big.png"), szAppPathParent,strSex,iSitGrade);

			wsprintfW(wszBuffer,L"%S",szBuffer);
			Image img(wszBuffer);

			if(img.GetWidth() <= 0)
			{
				return ; 
				//wsprintf(szBuffer,_T("..\\image\\log\\stand\\gamelog%d.bmp"),uLogoID%0x100);
				//if (!CBcfFile::IsFileExist(szBuffer))
				//{
				//	wsprintf(szBuffer,_T("..\\image\\log\\stand\\gamelog%d.bmp"),uLogoID%12);
				//}
				//wsprintfW(wszBuffer,L"%S",szBuffer);
				//img.FromFile(wszBuffer);
			}
			gra.DrawImage(&img,Rect(iXPos+78/2-img.GetWidth()/2,iYPos+108/2-img.GetHeight()/2,img.GetWidth(),img.GetHeight()),0,0,img.GetWidth(),img.GetHeight(),UnitPixel);

		}
		return;

	}

	//{{add by zxd 2009-03-26������һ��Ŀ¼����ͷ��ͼƬ����szAppPathParent����ԭ����CINIFile::GetAppPath()
	CString szAppPath = CINIFile::GetAppPath();
	szAppPath.SetAt(szAppPath.GetLength()-1, TCHAR('\0'));
	int nPosOfSlash = szAppPath.ReverseFind(TCHAR('\\'));
	CString szAppPathParent = szAppPath.Left(nPosOfSlash);
	//add by zxd}}

	if(bIsSmall)
	{
		wsprintf(szBuffer,TEXT("%s/image\\log\\%s\\gamelog%ld.png"), szAppPathParent,pUserItem->GameUserInfo.bBoy?"Boy":"Girl",iSitGrade);
		if(uLogoID>=0xFF && uLogoID<0x200)
		{
			wsprintf(szBuffer,"%s/CustomFace\\%d.png", szAppPathParent, dwUserID);
			CFileFind ff;
			BOOL bFoundFile=ff.FindFile(szBuffer);
			ff.Close();
			if(!bFoundFile)
				wsprintf(szBuffer,TEXT("%s/image\\log\\%s\\gamelog%ld.png"), szAppPathParent,pUserItem->GameUserInfo.bBoy?"Boy":"Girl",iSitGrade);
		}
#ifndef BZ_ZLIB
		if (!CBcfFile::IsFileExist(szBuffer))
		{
			wsprintf(szBuffer,TEXT("%s/image\\log\\%s\\gamelog%ld.png"), szAppPathParent,pUserItem->GameUserInfo.bBoy?"Boy":"Girl",0 /*uLogoID%12*/);
		}
#endif

		CGameImage img;	
		img.SetLoadInfo(szBuffer,CGameImageLink::m_bAutoLock);
		//������ϷlogoĿ¼
		if(img.GetWidth() <= 0)
		{
			wsprintf(szBuffer,"..\\image\\log\\stand\\gamelog%d.bmp",iSitGrade);
#ifndef BZ_ZLIB
			if (!CBcfFile::IsFileExist(szBuffer))
			{
				wsprintf(szBuffer,"..\\image\\log\\stand\\gamelog%d.bmp",iSitGrade);
			}

#endif
			img.SetLoadInfo(szBuffer,CGameImageLink::m_bAutoLock);
		}
		CGameImageHelper help(&img);
		CDC dc;
		dc.CreateCompatibleDC(pDC);
		dc.SelectObject(help);
		::TransparentBlt(pDC->GetSafeHdc(),iXPos+78/2-help.GetWidth()/2,iYPos+108/2-help.GetHeight()/2,help.GetWidth(),help.GetHeight(),dc.GetSafeHdc(),0,0,help.GetWidth(),help.GetHeight(),dc.GetPixel(0,0));
		dc.DeleteDC();
	}
	else
	{
		wsprintf(szBuffer,TEXT("%s/image\\log\\log2\\%s\\gamelog%ld.png"), szAppPathParent,pUserItem->GameUserInfo.bBoy?"Boy":"Girl", iSitGrade);
		bool bUserCustom=false;
		COLORREF cr=RGB(255,0,255);
		if(uLogoID>=0xFF && uLogoID<0x200)
		{
			bUserCustom=true;
			wsprintf(szBuffer,"%s/CustomFace\\%d.png", szAppPathParent, dwUserID);
			CFileFind ff;
			BOOL bFoundFile=ff.FindFile(szBuffer);
			ff.Close();
			if(!bFoundFile)
			{
				wsprintf(szBuffer,TEXT("%s/image\\log\\log2\\%s\\gamelog%ld.png"), szAppPathParent,pUserItem->GameUserInfo.bBoy?"Boy":"Girl", iSitGrade);
				bUserCustom=false;
			}
		}
#ifndef BZ_ZLIB
		if (!CBcfFile::IsFileExist(szBuffer))
		{
			wsprintf(szBuffer,TEXT("%s/image\\log\\log2\\%s\\gamelog%ld.png"), szAppPathParent,pUserItem->GameUserInfo.bBoy?"Boy":"Girl", 0/*uLogoID%12*/);
		}
#endif

		CGameImage img;	
		img.SetLoadInfo(szBuffer,CGameImageLink::m_bAutoLock);

		CGameImageHelper help(&img);
		CDC dc;
		dc.CreateCompatibleDC(pDC);
		CBitmap *Oldbmp = (CBitmap*)dc.SelectObject(help);
		if(!bUserCustom)
			cr=dc.GetPixel(0,0);

		::TransparentBlt(pDC->GetSafeHdc(),iXPos+LOGO_WIDTH/2-help.GetWidth()/2,iYPos+LOGO_HEIGHT/2-help.GetHeight()/2,help.GetWidth(),help.GetHeight(),dc.GetSafeHdc(),0,0,help.GetWidth(),help.GetHeight(),cr);//,dc.GetPixel(0,0));
		dc.SelectObject(Oldbmp);
		dc.DeleteDC();
	}


	return;
}

//�滭���
void CUsingSkinClass::DrawViewFrame(CDC * pDC, int iWidth, int iHeight)
{

	//�滭���¿��
	CGameImageHelper TopHandle(&m_PicTop);
	CGameImageHelper BottomHandle(&m_PicBottom);
	for (int i=0;i<iWidth;i+=TopHandle.GetWidth()) 
	{
		TopHandle.BitBlt(pDC->GetSafeHdc(),i,0);
		BottomHandle.BitBlt(pDC->GetSafeHdc(),i,iHeight-BottomHandle.GetHeight());
	}
	
	//�滭���ҿ��
	CGameImageHelper LeftHandle(&m_PicLeft);
	CGameImageHelper RightHandle(&m_PicRight);
	for (int i=0;i<iHeight;i+=LeftHandle.GetHeight())
	{
		LeftHandle.BitBlt(pDC->GetSafeHdc(),0,i);
		RightHandle.BitBlt(pDC->GetSafeHdc(),iWidth-LeftHandle.GetWidth(),i);
	}

	//�滭�Ľ�
	CGameImageHelper TopLeftHandle(&m_PicTopLeft);
	AFCStretchImage(pDC,0,0,TopLeftHandle.GetWidth(),TopLeftHandle.GetHeight(),
		TopLeftHandle,0,0,TopLeftHandle.GetWidth(),TopLeftHandle.GetHeight(),RGB(0,0,0));

	CGameImageHelper TopRightHandle(&m_PicTopRight);
	AFCStretchImage(pDC,iWidth-m_PicTopRight.GetWidth(),0,m_PicTopRight.GetWidth(),m_PicTopRight.GetHeight(),
		m_PicTopRight,0,0,m_PicTopRight.GetWidth(),m_PicTopRight.GetHeight(),RGB(0,0,0));

	CGameImageHelper BottomLeftHandle(&m_PicBottomLeft);
	AFCStretchImage(pDC,0,iHeight-BottomLeftHandle.GetHeight(),BottomLeftHandle.GetWidth(),BottomLeftHandle.GetHeight(),
		BottomLeftHandle,0,0,BottomLeftHandle.GetWidth(),BottomLeftHandle.GetHeight(),RGB(0,0,0));

	CGameImageHelper BottomRightHandle(&m_PicBottomRight);
	AFCStretchImage(pDC,iWidth-BottomRightHandle.GetWidth(),iHeight-BottomRightHandle.GetHeight(),BottomRightHandle.GetWidth(),BottomRightHandle.GetHeight(),
		BottomRightHandle,0,0,BottomRightHandle.GetWidth(),BottomRightHandle.GetHeight(),RGB(0,0,0));

	pDC->SetTextAlign(TA_LEFT);
	pDC->SetTextColor(RGB(0,0,0));
	pDC->TextOut(9,10,TMLcopyright);
	pDC->TextOut(11,10,TMLcopyright);
	pDC->TextOut(10,9,TMLcopyright);
	pDC->TextOut(10,11,TMLcopyright);
	pDC->SetTextColor(RGB(255,255,255));
	pDC->TextOut(10,10,TMLcopyright);//"��Ϸ���ġ�������֣������ҿ��֣�");

	return;
}

//���½���
void CUsingSkinClass::UpdateViewFace(const RECT * pRect)
{
	if(!this->m_hWnd)
		return;
	m_bReDraw=true;
	if (pRect!=NULL) 
		InvalidateRect(pRect,FALSE);
	else
	{
		if(this->m_hWnd)
			Invalidate(FALSE);
	}
	return;
}

//�ػ�����
void CUsingSkinClass::ReDrawViewFace(const RECT * pRect)
{
	if (m_PicBuffer.GetSafeHandle()!=NULL)
	{
		//��ȡλ��
		CRect ClientRect;
		GetClientRect(&ClientRect);

		//�������� DC
		CDC DCBack;
		DCBack.CreateCompatibleDC(NULL);
		DCBack.SelectObject(&m_PicBuffer);

		//�ػ�������
		if (m_bReDraw==true)
		{
			m_bReDraw=false;
			DCBack.SetBkMode(TRANSPARENT);
			DCBack.SelectObject(&CGameImageLink::m_HFont);
			DrawViewFace(&DCBack,ClientRect.Width(),ClientRect.Height());
		}
			
		//�滭����Ļ
		CDC * pDC=GetDC();
		if (pRect!=NULL) pDC->BitBlt(pRect->left,pRect->top,pRect->right,pRect->bottom,&DCBack,pRect->left,pRect->top,SRCCOPY);
		else pDC->BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),&DCBack,0,0,SRCCOPY);
		DCBack.DeleteDC();
		ReleaseDC(pDC);
	}
	else
	{
		if (pRect==NULL) 
			Invalidate(FALSE);
		else 
		{
			if(this->m_hWnd)
				InvalidateRect(pRect,FALSE);
		}
	}
	return;
}

//���ô��ڱ���
void CUsingSkinClass::SetViewTitle(const TCHAR * szTitle)
{
	m_strTitle=szTitle;
	UpdateViewFace(NULL);
	return;
}

//������ʾʱ��
bool CUsingSkinClass::SetUserTimer(BYTE bViewStation, UINT uTime)
{
	if (bViewStation<MAX_PEOPLE)
	{
		m_uTime[bViewStation]=uTime;
 
		CRect TimerRect(m_PtTime[bViewStation].x-20,m_PtTime[bViewStation].y-20,m_PtTime[bViewStation].x+TIME_WIDTH+20,m_PtTime[bViewStation].y+TIME_HEIGHT+20);
		UpdateViewFace(&TimerRect);
		return true;
	}
	return false;
}

//�����û���Ϣ
bool CUsingSkinClass::SetUserInfo(BYTE bViewStation, UserItemStruct * pUserItem)
{
	if (bViewStation < MAX_PEOPLE)
	{
		m_showcoredlg.UpdateUser(bViewStation,pUserItem);
		m_pUserItem[bViewStation]=pUserItem;
		m_uTime[bViewStation]=0;
		
		if (pUserItem != NULL && m_pGameInfo != NULL && m_pGameInfo->dwRoomRule & GRR_CONTEST)
		{
			if (m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID == pUserItem->GameUserInfo.dwUserID)
			{
				m_ContestInfoWnd.UpdateUser(pUserItem);
				pUserItem->GameUserInfo.i64Money = pUserItem->GameUserInfo.i64ContestScore;
			}
		}
	}
	return false;
}

bool CUsingSkinClass::SetGameInfo(GameInfoStructInExe* pGameInfo)
{
	m_pGameInfo = pGameInfo;
	//if (m_pGameInfo->dwRoomRule & GRR_CONTEST)
	{
		m_ContestInfoWnd.SetGameInfo(m_pGameInfo);
	}
	return true;
}

//���ý��溯��
void CUsingSkinClass::ResetGameView()
{
	memset(m_uTime,0,sizeof(m_uTime));
	memset(m_pUserItem,0,sizeof(m_pUserItem));
	
	m_showcoredlg.ClearUser();
	m_showcoredlg.ClearScore();
	
	UpdateViewFace(NULL);
	return;
}

//͸���滭
bool CUsingSkinClass::StretchBlt(CDC * pDesDC, int iXDes, int iYDes, int iDesWidth, int iDesHeight,
					  CDC * pScrDC, int iXScr, int iYScr, int iScrWidth, int iScrHeight, COLORREF crColor)
{
	return AFCStretchBlt(pDesDC,iXDes,iYDes,iDesWidth,iDesHeight,pScrDC,iXScr,iYScr,iScrWidth,iScrHeight,crColor);
}

//�滭͸��ͼ
bool CUsingSkinClass::StretchImage(CDC * pDesDC, int iXDes, int iYDes, int iDesWidth, int iDesHeight,
					   HBITMAP hBitBmp, int iXScr, int iYScr, int iScrWidth, int iScrHeight, COLORREF crColor)
{
	return AFCStretchImage(pDesDC,iXDes,iYDes,iDesWidth,iDesHeight,hBitBmp,iXScr,iYScr,iScrWidth,iScrHeight,crColor);
}

//�ƶ�����
bool CUsingSkinClass::SafeMoveWindow(UINT uID, int x, int y, int iWidth, int iHeight)
{
	CWnd * pMoveWnd=GetDlgItem(uID);
	if ((pMoveWnd!=NULL)&&(pMoveWnd->GetSafeHwnd())) 
	{
		if ((iWidth<=0)||(iHeight<=0)) iWidth=0,iHeight=0;
		pMoveWnd->MoveWindow(x,y,iWidth,iHeight);
		return true;
	}
	return false;
}

//�ƶ�����
bool CUsingSkinClass::SafeMoveWindow(CWnd * pControlWnd, int x, int y, int iWidth, int iHeight)
{
	if ((pControlWnd!=NULL)&&(pControlWnd->GetSafeHwnd())) 
	{
		if ((iWidth<=0)||(iHeight<=0)) iWidth=0,iHeight=0;
		pControlWnd->MoveWindow(x,y,iWidth,iHeight);
		return true;
	}
	return false;
}

//λ�ñ仯
void CUsingSkinClass::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//���Ļ���ͼ
	if ((m_bDoubleBuf==true)&&(SIZE_MINIMIZED!=nType))
	{
		if ((cx>m_PicInfo.bmWidth)||(cy>m_PicInfo.bmHeight))
		{
			CDC * pDC=GetDC();
			m_PicBuffer.DeleteObject();
			m_PicBuffer.CreateCompatibleBitmap(pDC,__max(cx,m_PicInfo.bmWidth),__max(cy,m_PicInfo.bmHeight));
			m_PicBuffer.GetBitmap(&m_PicInfo);
			ReleaseDC(pDC);
		}
	}

	//����λ��
	if(m_ShowType == 1 && m_bShowRight)
		FixControlSize(cx-m_vlistx,cy);//���ڲ���С,����ʾ�����С
	else
		FixControlSize(cx,cy);

	//���û滭����
	if (m_pDrawInfoPtr!=NULL)
	{
		m_pDrawInfoPtr->SetDrawRange(0,0,cx,cy);
		if (nType==SIZE_MINIMIZED) m_pDrawInfoPtr->SuspendThread();
		else m_pDrawInfoPtr->ResumeThread();
	}
	else UpdateViewFace(NULL);

	return;
}

//����������
void CUsingSkinClass::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	__super::OnLButtonDown(nFlags, point);
}

//��������
void CUsingSkinClass::OnDestroy()
{
	__super::OnDestroy();

	//�����߳�
	if (m_pDrawInfoPtr!=NULL) m_pDrawInfoPtr->EndDrawThread();
	m_showcoredlg.DestroyWindow();
	return;
}
//��ʼ��gif
void CUsingSkinClass::InitGif()
{
	//��ʼ��
	HRESULT hr;
	hr = CObjectFactory::InitRes();
	if (FAILED(hr))
	{
		CString	str("Init Failed!");
		//MessageBox(str);
	}
	m_pGif =NULL; 
	m_iFrameCount = 0;
	m_iGifLifeTime = 0;
}
//����ͼ
void CUsingSkinClass::SetGif(BYTE bDeskView,int iValue,int iTimer)
{
	if(m_pGif != NULL)
		return ;
	if (bDeskView < 0 || bDeskView >= MAX_PEOPLE)
		return;

	if (iValue < 0 || iValue%1000 >= GIF_COUNT)
		return;

	m_iGifDeskStation = bDeskView;
	

	CString strFilePathName; 
	//strFilePathName.Format("..\\image\\prop\\PropGif%d%s.gif",iValue%1000,iValue<1000?"A":"B");
	strFilePathName.Format("..\\image\\prop\\PropGif%d%s.spr",iValue%1000,iValue<1000?"A":"B"); //modify by lxl  ͼƬ��ʽ�޸�gif->spr
	if(m_pLoadGif)
		m_pLoadGif->Release();
	CObjectFactory	Object;
	HRESULT hr = Object.CreateInstance(NULL, CLSID_IGif, (void **)&m_pLoadGif);
	if (FAILED(hr))
	{
		CString	str("Create Object Failed!");
	}
	hr = m_pLoadGif->LoadFromFile(strFilePathName);
	if (FAILED(hr))
	{
		CString	str("Load Gif Failed!");
	}
	m_pGif = m_pLoadGif;
	m_pGif->Reset();
	m_iFrameCount = 0;
	m_iGifLifeTime = iTimer / 100;
//	UpdateViewFace(NULL);
}
//����
void CUsingSkinClass::DrawGif(CDC *pDC,int ix,int iy)
{
	if(m_pGif ==  NULL)
		return ;
	if(m_iGifLifeTime == 0)
		return ;
	if(m_iFrameCount >= m_pGif->GetFrameCount()*2)//m_iGifLifeTime)
	{
		m_pGif =  NULL;
		m_iFrameCount = 0;
	}
	if(m_pGif != NULL)
	{
		this->m_pGif->Bitblt(m_iFrameCount, pDC,ix, iy);
	}
}

//��ȡ����ǳ���ɫ
COLORREF  CUsingSkinClass::GetNameColor(UserInfoStruct *pUserInfo)
{
	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
	int curtime=atoi(stime);                        //ת��Ϊint
	if(pUserInfo->iVipTime>curtime)
		return RGB(255,0,0);
	return RGB(255,255,0);;
}

/*******************************************************************************************************/

//���캯��
CNewCDC::CNewCDC(CWnd * pGameView) : m_pGameView(pGameView)
{
	m_iXPos=0;
	m_iYPos=0;
	m_iSpeed=0L;
	m_bQuit=false;
	m_iDrawWidth=0;
	m_iDrawHeight=0;
	m_iTimeSpace=10;
	m_iGDISleepTime=10;
	m_hGDIThreadHandle=NULL;
	m_hTimeThreadHandle=NULL;
}

//��������
CNewCDC::~CNewCDC()
{
	EndDrawThread();
}

//��ʼ����ͼ����
bool CNewCDC::InitDraw(CDC * pDrawDC)
{
	//���ñ���
	m_iSpeed=0L;
	m_iTimeSpace=10;
	m_iGDISleepTime=10;

	return true;
}

//ȡ����ʼ������
bool CNewCDC::UnInitDraw()
{
	m_iSpeed=0L;
	m_iDrawWidth=0L;
	m_iDrawHeight=0L;
	m_iTimeSpace=10L;
	m_iGDISleepTime=10L;
	return true;
}

//���÷�Χ
void CNewCDC::SetDrawRange(int iXPos, int iYPos, int iDrawWidth, int iDrawHeight) 
{ 
	//��������
	m_iXPos=iXPos;
	m_iYPos=iYPos;
	m_iDrawWidth=iDrawWidth;
	m_iDrawHeight=iDrawHeight;

	return;
}

//ֹͣ�߳�
void CNewCDC::SuspendThread()
{
	::SuspendThread(m_hGDIThreadHandle);
}
	
//�����߳�
void CNewCDC::ResumeThread()
{
	::ResumeThread(m_hGDIThreadHandle);
}

//��ʼ�߳�
bool CNewCDC::CreateDrawThread()
{
	try
	{
		//�����߳�
		m_bQuit=false;
		UINT dwThreadID=0;
		m_hGDIThreadHandle=(HANDLE)_beginthreadex(NULL,0,GDIModeThread,this,CREATE_SUSPENDED,&dwThreadID);
		if (m_hGDIThreadHandle==NULL) throw TEXT("Start Thread Error.");
		SetThreadPriority(m_hGDIThreadHandle,THREAD_PRIORITY_IDLE);
		m_hTimeThreadHandle=(HANDLE)_beginthreadex(NULL,0,TimerThread,this,CREATE_SUSPENDED,&dwThreadID);
		if (m_hTimeThreadHandle==NULL) throw TEXT("Start Thread Error.");
		::ResumeThread(m_hGDIThreadHandle);
		::ResumeThread(m_hTimeThreadHandle);
	}
	catch (...)
	{
		EndDrawThread();
		return false;
	}

	return true;
}

//�����߳�
bool CNewCDC::EndDrawThread()
{
	//�����߳�
	m_bQuit=true;
	if (m_hGDIThreadHandle!=NULL)
	{
		if (WaitForSingleObject(m_hGDIThreadHandle,1000)==WAIT_TIMEOUT)	TerminateThread(m_hGDIThreadHandle,1);
		CloseHandle(m_hGDIThreadHandle);
		m_hGDIThreadHandle=NULL;
	}

	//�����߳�
	if (m_hTimeThreadHandle!=NULL)
	{
		if (WaitForSingleObject(m_hTimeThreadHandle,1000)==WAIT_TIMEOUT) TerminateThread(m_hTimeThreadHandle,1);
		CloseHandle(m_hTimeThreadHandle);
		m_hTimeThreadHandle=NULL;
	}

	return true;
}

//��ʱ���߳�
unsigned __stdcall CNewCDC::TimerThread(LPVOID pThreadData)
{
	//�������
	CNewCDC * pDrawInfo=(CNewCDC *)pThreadData;
	CWnd * pHandleWnd=pDrawInfo->m_pGameView->GetParent();

	//ѭ����ʱ��
	try
	{
		while (1)
		{
			//���Ͷ�ʱ����Ϣ
			Sleep(pDrawInfo->m_iTimeSpace);
			pHandleWnd->PostMessage(WM_TIMER,THREAD_TIME_ID,0);

			//���ֹͣ
			if (pDrawInfo->m_bQuit==true) break;
		}
	}
	catch (...) {}

	return 0;
}

//��ͼ�߳�
unsigned __stdcall CNewCDC::GDIModeThread(LPVOID pThreadData)
{
	//��ʼ������
	CDC	DCBuffer;
	CBitmap	BufferBmp;
	CNewCDC * pDrawInfo=(CNewCDC *)pThreadData;
	CDC * pDrawDC=pDrawInfo->m_pGameView->GetDC();
	CUsingSkinClass * pGDIView=(CUsingSkinClass *)pDrawInfo->m_pGameView;

	try
	{
		//λ����Ϣ
		int iDrawWidth=0;
		int iDrawHeight=0;

		//����˫����
		DCBuffer.CreateCompatibleDC(pDrawDC);
		DCBuffer.SetBkMode(TRANSPARENT);
		if (pDrawInfo->InitDraw(&DCBuffer)==false) throw TEXT("��ʼ������");

		//֡������
		int iSpeed=0;
		long int dwBeginTime=(long int)time(NULL);
		long int dwEndTime=(long int)time(NULL);

		//ѭ���滭
		while (1)
		{
			//Ч��λͼ��С
			if ((pDrawInfo->m_iDrawWidth>iDrawWidth)||(pDrawInfo->m_iDrawHeight>iDrawHeight))
			{
				iDrawWidth=__max(iDrawWidth,pDrawInfo->m_iDrawWidth);
				iDrawHeight=__max(iDrawHeight,pDrawInfo->m_iDrawHeight);
				BufferBmp.Detach();
				BufferBmp.DeleteObject();
				BufferBmp.CreateCompatibleBitmap(pDrawDC,iDrawWidth,iDrawHeight);
				DeleteObject(DCBuffer.SelectObject(&BufferBmp));
			}

			//�滭����
			pDrawInfo->ThreadDrawView(&DCBuffer,iDrawWidth,iDrawHeight);
			pGDIView->DrawViewFrame(&DCBuffer,iDrawWidth,iDrawHeight);
			pDrawDC->BitBlt(pDrawInfo->m_iXPos,pDrawInfo->m_iYPos,iDrawWidth,iDrawHeight,&DCBuffer,0,0,SRCCOPY);

			//����֡��
			dwEndTime=(long int)time(NULL);
			iSpeed++;
			if ((dwEndTime-dwBeginTime)>=1L)
			{
				pDrawInfo->m_iSpeed=iSpeed;
				dwBeginTime=dwEndTime;
				iSpeed=0;
			}

			//���ֹͣ
			if (pDrawInfo->m_bQuit==true) break;
			Sleep(pDrawInfo->m_iGDISleepTime);
		}
	}
	catch (...) {}

	//��������
	pDrawInfo->m_pGameView->ReleaseDC(pDrawDC);
	DCBuffer.DeleteDC();
	BufferBmp.DeleteObject();
	pDrawInfo->UnInitDraw();

	return 0;
}


/*******************************************************************************************************/

//void CUsingSkinClass::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	if( m_hCursor )
//		SetCursor(m_hCursor);
//	else
//	{
//		DestroyCursor(m_hCursor);
//		m_hCursor = NULL;
//	}
//	CWnd::OnMouseMove(nFlags, point);
//}


//LRESULT	CUsingSkinClass::OnPropSelectCursorChange(WPARAM wParam, LPARAM lParam)
//{
//	//CPropSelUser psu;
//	//psu.dwSelectUserID=-1;
//	//for(int i=0;i<MAX_PEOPLE;i++)
//	//	psu.m_pUserItem[i]=m_pUserItem[i];
//
//	//psu.dwLocalUserID=(int)lParam;
//	//if(psu.DoModal()==IDOK)
//	//{
//	//	CWnd * pParent = GetParent();
//	//	if(pParent)
//	//		pParent->SendMessage(PLAYVIEW_LBUTTON_DOWN, (WPARAM)psu.dwSelectUserID, 0);
//	//}
//	return 0;
//}
//void CUsingSkinClass::OnRButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	CWnd * pParent = GetParent();
//	if( pParent )
//	{
//		//��������£�����ѡ����û�ID������û��������ʱ����û�з���
//		//ĳ���û���ͷ���ϣ��򴫵�-1��
//		pParent->SendMessage(PLAYVIEW_LBUTTON_DOWN, (WPARAM)-1, 0);
//	}
//	CWnd::OnRButtonDown(nFlags, point);
//}
LRESULT CUsingSkinClass::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		//case WM_CTLCOLORSCROLLBAR :
		//case WM_CTLCOLORBTN:
		//case WM_CTLCOLORMSGBOX:
		//case WM_CTLCOLOREDIT:
		//case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSTATIC :
		SetTextColor((HDC)wParam, RGB(0,0,0)) ;
		SetBkColor((HDC)wParam, m_bkColor);
		return (LRESULT)m_bkBrush;
	}


	return __super::WindowProc(message, wParam, lParam);
}
BOOL CUsingSkinClass::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//return false;
	//return CWnd::OnEraseBkgnd(pDC);
	return true;
}
	////////////////////////////////
	/////Kylin 20090120 ������
//�������Gif��������
void CUsingSkinClass::ClearGif()
{
	m_pGif =  NULL;
	m_iGifLifeTime = 0;
	m_iFrameCount = 0;
}
////////////////////////////////
//ͳ��֡��
void CUsingSkinClass::CountFrame()
{
	m_iFrameCount++;
}

//��ȡ֡��
int CUsingSkinClass::GetFrameCount() const
{
	return m_iFrameCount;
}
