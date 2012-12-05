#include "StdAfx.h"
#include "ClientView.h"
#include "ClientGameDlg.h"

/// �궨��
#define BT_WIDTH							32						///< ��ť���
#define BT_HEIGHT							32						///< ��ť�߶�

CBitmap				CClientPlayView::m_Bitmap_Num;


BEGIN_MESSAGE_MAP(CClientPlayView, CUsingSkinClass)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BEGIN,OnHitBegin)
	ON_BN_CLICKED(IDC_GIVE_BACK,OnHitBackCard)
	ON_BN_CLICKED(IDC_OUT_CARD,OnHitOutCard)
	ON_BN_CLICKED(IDC_CONTINUE, OnHitContinue)
	ON_BN_CLICKED(IDC_LAST_CARD, OnHitLastTurn)
	ON_BN_CLICKED(IDC_BACK_CARD, OnHitLookBack)
	ON_BN_CLICKED(IDC_HAVE_THING, OnHitThing)
	ON_BN_CLICKED(IDC_STOP, OnHitStop)
	ON_BN_CLICKED(IDC_ADMIT_DEFEAT,OnHitAdmitDefeat)
	ON_BN_CLICKED(IDC_REGRET_CHESS,OnHitRegretChess)
	ON_BN_CLICKED(IDC_CHESS_MANUAL,OnHitChessManual)
	ON_BN_CLICKED(IDC_LEAVE,OnHitLeave)

END_MESSAGE_MAP()

/*******************************************************************************************************/

/**
 * ���캯��
 */
CClientPlayView::CClientPlayView(void) //:m_GameSet(this),m_GameInfo(this)//,m_GameEndView(this)
{
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));

	TCHAR path[MAX_PATH];
	wsprintf(path,".\\image\\zgxq.bmp");
	m_PicGameView.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	m_iGamePoint=0;
	m_bMeSide=-1;
	m_iHuaKind=0;
	m_iShowStation=-1;
	m_iNtStation=-1;

	m_bUpdateScoreboard=true;
	//AfxSetResourceHandle(GetModuleHandle(NULL));
	return;
}

/**
 * ��������
 */
CClientPlayView::~CClientPlayView(void)
{
	if (NULL != m_GameFinishView.GetSafeHwnd())	
		m_GameFinishView.DestroyWindow();	
	if (m_GameInfo.GetSafeHwnd() != NULL) 
		m_GameInfo.DestroyWindow();
	if (m_GameSet.GetSafeHwnd() != NULL) 
		m_GameSet.DestroyWindow();
}

/**
 * �����û�����
 */
void CClientPlayView::UpdateUserInfo(BYTE bViewStation)
{
	byte medesk=((CClientGameDlg*)GetParent())->GetMeUserInfo()->bDeskStation;
	for(int i=0;i<2;i++)
	{
		if(m_pUserItem[i]==NULL)
		{
			m_MeCard.SetUserInfo(NULL,0);
			continue;
		}
		if(m_pUserItem[i]->GameUserInfo.bDeskStation==medesk)
		{
			m_MeCard.SetUserInfo(m_pUserItem[i],1);
		}
		else
			m_MeCard.SetUserInfo(m_pUserItem[i],0);
	}
	UpdateViewFace(NULL);
	m_MeCard.InvalidateRect(NULL);	
	return;
}

/**
 * �滭���溯��
 */
void CClientPlayView::DrawViewFace(CDC * pDC, int iWidth, int iHeight)
{
	//�滭����
	/*CGameImageHelper LogoHandle(&m_PicGameView);
	pDC->FillSolidRect(0,0,iWidth,iHeight,LogoHandle.GetPixel(0,0));*/
	CGameImageHelper LogoHandle(&m_imgBk);
	if (NULL != pDC)
	{
		// �Ȼ�СͼƬ����
		int row = (iWidth + LogoHandle.GetWidth())/LogoHandle.GetWidth();
		int line = (iHeight + LogoHandle.GetHeight())/LogoHandle.GetHeight();
		for (int i=0; i<row; ++i)
		{
			for (int j=0; j<line; ++j)
			{
				LogoHandle.BitBlt(pDC->GetSafeHdc(), i*LogoHandle.GetWidth(), j*LogoHandle.GetHeight());
			}
		}
	}
	//GIF����
	CDC *dc=m_MeCard.GetDC();

	DrawGif(dc,m_PtGif[m_iGifDeskStation].x,m_PtGif[m_iGifDeskStation].y);

	if (m_iFrameCount %3 == 0)//JMod-20090211
	{
		m_MeCard.Invalidate(false);
#ifdef VIDEO
		((CClientGameDlg *)GetParent())->RefreshAllVideoPanel();
#endif
		
		
	}
	return;
}

void CClientPlayView::TransparentBlt2( HDC hdcDest,      // Ŀ��DC
					int nXOriginDest,   // Ŀ��Xƫ��
					int nYOriginDest,   // Ŀ��Yƫ��
					int nWidthDest,     // Ŀ����
					int nHeightDest,    // Ŀ��߶�
					HDC hdcSrc,         // ԴDC
					int nXOriginSrc,    // ԴX���
					int nYOriginSrc,    // ԴY���
					int nWidthSrc,      // Դ���
					int nHeightSrc,     // Դ�߶�
					UINT crTransparent  // ͸��ɫ,COLORREF����
					)
{
	HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	// ��������λͼ
	HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);			// ������ɫ����λͼ
	HDC		hImageDC = CreateCompatibleDC(hdcDest);
	HDC		hMaskDC = CreateCompatibleDC(hdcDest);
	hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
	hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);

	// ��ԴDC�е�λͼ��������ʱDC��
//	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
//	else
//		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, 
//					hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);

	// ����͸��ɫ
	SetBkColor(hImageDC, crTransparent);

	// ����͸������Ϊ��ɫ����������Ϊ��ɫ������λͼ
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);

	// ����͸������Ϊ��ɫ���������򱣳ֲ����λͼ
	SetBkColor(hImageDC, RGB(0,0,0));
	SetTextColor(hImageDC, RGB(255,255,255));
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// ͸�����ֱ�����Ļ���䣬�������ֱ�ɺ�ɫ
	SetBkColor(hdcDest,RGB(255,255,255));
	SetTextColor(hdcDest,RGB(0,0,0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// "��"����,��������Ч��
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);


	// �����ָ�	
	SelectObject(hImageDC, hOldImageBMP);
	DeleteDC(hImageDC);
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteDC(hMaskDC);
	DeleteObject(hImageBMP);
	DeleteObject(hMaskBMP);
}

/**
 * ������Ϣ
 */
int CClientPlayView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CUsingSkinClass::OnCreate(lpCreateStruct) == -1)	return -1;

	//�����ؼ�
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
	TCHAR filename[MAX_PATH];

#ifdef	VIDEO
	wsprintf(filename,".\\image\\VideoFrame.bmp");
	m_pVideoFrame.SetLoadInfo(filename,false);
#endif

	m_GameNoteView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,0,0),this,50,NULL);
	m_GameNoteView.AutoSize();
	//SetWindowLong(m_GameNoteView.GetSafeHwnd(), GWL_EXSTYLE, ::GetWindowLong(m_GameNoteView.GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_TRANSPARENT);

	m_showcoredlg.m_count = PLAY_COUNT;
	m_StationView.Create(this,100);
	m_btOutCard.Create(TEXT("����"),WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,80,25),this,IDC_OUT_CARD);
	m_btContinue.Create(TEXT("����"),WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,90,34),this,IDC_CONTINUE);
	m_btStart.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,90,34),this,IDC_CONTINUE);
	
	wsprintf(filename,".\\image\\start_bt.bmp");
	m_btStart.LoadButtonBitmap(filename,false);
	m_btThing.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,90,34),this,IDC_HAVE_THING);
	
	wsprintf(filename,".\\image\\havething_bt.bmp");
	m_btThing.LoadButtonBitmap(filename,false);
	m_btStop.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,90,34),this,IDC_STOP);
	
	wsprintf(filename,".\\image\\qiuhe_bt.bmp");
	m_btStop.LoadButtonBitmap(filename,false);
	m_btAdmitDefeat.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,90,34),this,IDC_ADMIT_DEFEAT);
	m_btLeave.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,90,34),this,IDC_LEAVE);
	
	wsprintf(filename,".\\image\\leave_bt.bmp");
	m_btLeave.LoadButtonBitmap(filename,false);

	wsprintf(filename,".\\image\\renshu_bt.bmp");
	m_btAdmitDefeat.LoadButtonBitmap(filename,false);
	m_btRegretChess.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,90,34),this,IDC_REGRET_CHESS);
	
	wsprintf(filename,".\\image\\huiqi_bt.bmp");
	m_btRegretChess.LoadButtonBitmap(filename,false);
	
	//�ƶ�λ��
//	m_btLastTurn.SetFont(&CGameImageLink::m_HFont);
//	m_btBackCard.SetFont(&CGameImageLink::m_HFont);
	m_btThing.SetFont(&CGameImageLink::m_HFont);

	m_MeCard.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,CRect(0,0,0,0),this,25,NULL);
	m_MeCard.InitGameCard(GetParent());
	//������Դ
	m_btOutCard.SetFont(&CGameImageLink::m_HFont);
//	m_btGiveBack.SetFont(&CGameImageLink::m_HFont);
	m_btContinue.SetFont(&CGameImageLink::m_HFont);
	//AfxSetResourceHandle(GetModuleHandle(NULL));

	//���ر���ͼƬ
	wsprintf(filename,".\\image\\BackColorOnly.bmp");	//����
	m_imgBk.SetLoadInfo(filename,CGameImageLink::m_bAutoLock);

	return 0;
}

void CClientPlayView::FixControlSize(int cx,int cy)
{

#ifdef	VIDEO
	/////������Ƶ����λ�� 
	//if(NULL!=CPlayVideo::CreatePlayVideo())
	//	CPlayVideo::CreatePlayVideo()->SetUserPos();  

	///������ʱ���£��Ժ��Ƶ��ű��л��߶�̬������
	CString  strVideoCfg=CINIFile::GetAppPath()+TEXT("VideoCfg.ini");
	
	CINIFile iniFile(strVideoCfg);
	int	 m_heightVideo=0,m_widthVideo=0;
	m_widthVideo
		=iniFile.GetKeyVal(TEXT("VedioCfg"),TEXT("WidthVideoXQ"), 158);
	m_heightVideo
		=iniFile.GetKeyVal(TEXT("VedioCfg"),TEXT("HeightVideoXQ"), 159);
#endif

	m_MeCard.SetBasePoint(CPoint(0,-5),
		CUpGradeGameCard::XShowMode::SWX_MODE_CENTER,CUpGradeGameCard::YShowMode::SWY_MODE_TOP);
	
	m_MeCard.MoveWindow((cx-751)/2-23,(cy-712)/2+9,798,693,true);
	
	m_PtLogo[0].x=(cx-751)/2+XQ_UPPEOPLE_LOG_X;
	m_PtLogo[0].y=(cy-712)/2+XQ_UPPEOPLE_LOG_Y;

#ifdef	VIDEO
	m_RtVideo[0].left = XQ_UPPEOPLE_LOG_X - 30;
	m_RtVideo[0].top =  XQ_UPPEOPLE_LOG_Y - 30;
	m_RtVideo[0].right = m_RtVideo[0].left + m_widthVideo;
	m_RtVideo[0].bottom = m_RtVideo[0].top + m_heightVideo;
#endif
	m_PtLogo[1].x=(cx-751)/2+XQ_DOWNPEOPLE_LOG_X;
	m_PtLogo[1].y=(cy-712)/2+XQ_DOWNPEOPLE_LOG_Y;

#ifdef	VIDEO
	m_RtVideo[1].left = XQ_DOWNPEOPLE_LOG_X - 30;
	m_RtVideo[1].top = XQ_DOWNPEOPLE_LOG_Y - 30;
	m_RtVideo[1].right = m_RtVideo[1].left + m_widthVideo;
	m_RtVideo[1].bottom = m_RtVideo[1].top + m_heightVideo;

	///������Ƶ����λ�� 
	//if(NULL!=CPlayVideo::CreatePlayVideo())
	//	CPlayVideo::CreatePlayVideo()->SetUserPos();  
#endif

	m_showcoredlg.ShowWindow(SW_HIDE);//ȡ��������ʾ
	//�ƶ���Ϣ��ͼ
	CRect ContronRect;
	m_GameNoteView.GetClientRect(&ContronRect);
	m_GameNoteView.MoveWindow((cx-ContronRect.Width())/2+80,(cy-ContronRect.Height())/2-22,
							ContronRect.Width(),ContronRect.Height(),FALSE);

	int w=75,h=34;
	TCHAR filePath[MAX_PATH];

	wsprintf(filePath,".\\image\\start_bt.bmp");
	CGameImage btImage;
	btImage.SetLoadInfo(filePath,true);
	w = btImage.GetWidth() / 4;
	h = btImage.GetHeight();

	int xw=95/*100*/;
	int x,y;
	x=130+(cx-751)/2+15 + 37-23;
	y=660+(cy-712)/2+9;
	m_btStart.MoveWindow(x+xw*0 + 10,y - 12,w,h);
	m_btRegretChess.MoveWindow(x+xw*1 + 10,y -12,w,h);
	m_btAdmitDefeat.MoveWindow(x+xw*2 + 10,y - 12,w,h);
	m_btStop.MoveWindow(x+xw*3 + 10,y - 12,w,h);
	m_btThing.MoveWindow(x+xw*4 + 10,y - 12,w,h);
	m_btLeave.MoveWindow(x+xw*5 + 10,y - 12,w,h);

//	m_btUserFace[0].ShowWindow(0);
//	m_btUserFace[1].ShowWindow(0);
	CRect rect;
	m_showcoredlg.GetClientRect(&rect);
	m_showcoredlg.SetWindowPos(NULL,cx/2-100,-m_showcoredlg.m_showh,0,0,SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOSIZE);
	m_showcoredlg.ShowWindow(true);

	m_GameFinishView.GetClientRect(&ContronRect);
	//m_Result.AutoSize(cx/2 - ContronRect.Width()/2,cy/2 - ContronRect.Height()/2 - 30);
	m_GameFinishView.CenterWindow(GetParent());

	m_bUpdateScoreboard=true;

	CString s = CINIFile::GetAppPath ();/////����·��
//#ifdef VIDEO
//	CINIFile f( s + strClientName +"video.ini");
//#else
//	CINIFile f( s + strClientName +".ini");
//#endif

	//JMod-תBCF��ʽ����-20090217
	//DWORD cfgHandle=cfgOpenFile(s +"GameOption.bcf");
	DWORD cfgHandle=cfgOpenFile(s + GET_SKIN_FOLDER(szTempStr)+".bcf");
	if(cfgHandle<0x10)
	{
		return;
	}
	CString key = TEXT("UISet");
#ifdef VIDEO
	m_PtGif[0].x = XQ_UPPEOPLE_LOG_X+cfgGetValue(cfgHandle,key,TEXT("flower0x"),0)+40;
	m_PtGif[0].y = XQ_UPPEOPLE_LOG_Y+cfgGetValue(cfgHandle,key,TEXT("flower0y"),0);
	m_PtGif[1].x = XQ_DOWNPEOPLE_LOG_X+cfgGetValue(cfgHandle,key,TEXT("flower1x"),0)+40;
	m_PtGif[1].y = XQ_DOWNPEOPLE_LOG_Y+cfgGetValue(cfgHandle,key,TEXT("flower1y"),0);
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->RefreshAllVideoPanel();
#else
	m_PtGif[0].x = XQ_UPPEOPLE_LOG_X +cfgGetValue(cfgHandle,key,TEXT("flower0x"),0);
	m_PtGif[0].y = XQ_UPPEOPLE_LOG_Y+40+cfgGetValue(cfgHandle,key,TEXT("flower0y"),0);
	m_PtGif[1].x = XQ_DOWNPEOPLE_LOG_X+cfgGetValue(cfgHandle,key,TEXT("flower1x"),0);
	m_PtGif[1].y = XQ_DOWNPEOPLE_LOG_Y+40+cfgGetValue(cfgHandle,key,TEXT("flower1y"),0);
#endif
	cfgClose(cfgHandle);
	
	

	return;
}

/**
 * ��ʼ��ť
 */
void CClientPlayView::OnHitBegin()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_BEGIN,0,0);
	return;
}

/**
 * ���ư�ť
 */
void CClientPlayView::OnHitOutCard()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_OUT_CARD,0,0);
	return;
}

/**
 * ������ť
 */
void CClientPlayView::OnHitContinue()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_BEGIN,0,0);
	return;
}

/**
 * �����˿�
 */
void CClientPlayView::OnHitLastTurn()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_LOOK_LAST,0,0);
	return;
}

/**
 * ���ư�ť
 */
void CClientPlayView::OnHitLookBack()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_BACK_CARD,0,0);
	return;
}

/**
 * ����Ҫ��
 */
void CClientPlayView::OnHitThing()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_HAVE_THING,0,0);
	return;
}

/**
 * �����ư�ť
 */
void CClientPlayView::OnHitBackCard()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_GIVE_BACK,0,0);
	return;
}

/**
 * ��ǰֹͣ
 */
void CClientPlayView::OnHitStop()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_STOP,0,0);
	return;
}

/**
 * ����
 */
void CClientPlayView::OnHitAdmitDefeat()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_ADMIT_DEFEAT,0,0);
	return;
}

/**
 * ����
 */
void CClientPlayView::OnHitRegretChess()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_REGRET_CHESS,0,0);
	return;
}

/**
 * ����
 */
void CClientPlayView::OnHitChessManual()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_CHESS_MANUAL,0,0);
	return;
}

/**
 * ����
 */
void CClientPlayView::OnHitLeave()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(WM_CLOSE,0,0);
	return;
}

/**
 * ���ý��溯��
 */
void CClientPlayView::ResetGameView()
{
	CUsingSkinClass::ResetGameView();

	return;
}

/**
 * �滭 NT ��־
 */
void CClientPlayView::DrawNt(CDC * pDC, int x, int y)
{
	CDC NtDC;
	BITMAP BmpInfo;
	m_NT.GetBitmap(&BmpInfo);
	NtDC.CreateCompatibleDC(pDC);
	NtDC.SelectObject(&m_NT);
	pDC->BitBlt(x,y,BmpInfo.bmWidth,BmpInfo.bmHeight,&NtDC,0,0,SRCCOPY);
	return;
}

/**
 * �滭 NT ͼ��
 */
void CClientPlayView::DrawNtLogo(CDC * pDC, int x, int y, int iHuaKind)
{
	CDC NtDC;
	COLORREF crColor=0;
	NtDC.CreateCompatibleDC(pDC);
	NtDC.SelectObject(&m_NTLogo);
	int iBeginStation=(iHuaKind>>4)*15;
	for (int i=0;i<15;i++)
	{
		for (int j=0;j<15;j++)
		{
			crColor=NtDC.GetPixel(i+iBeginStation,j);
			if (crColor!=RGB(255,255,255)) pDC->SetPixel(x+i,y+j,crColor);
		}
	}
	return;
}

/**
 * ����ׯ�ҵ�λ��
 */
void CClientPlayView::SetNTStation(int iViewStation)
{
	m_iNtStation=iViewStation;
	UpdateViewFace(NULL);

	return;
}

/**
 * ���ñ���λ��
 */
void CClientPlayView::SetShowNtStation(int iViewStation, int iHuaKind)
{
	m_iHuaKind=iHuaKind;
	m_iShowStation=iViewStation;
	UpdateViewFace(NULL);

	return;
}

/**
 * ������Ϸ����
 */
void CClientPlayView::SetGamePoint(int iGamePoint, int bMeSide)
{
	if ((iGamePoint==0)&&(bMeSide==-1))
	{
		bMeSide=-1;
		m_iGamePoint=0;
	}
	else if (iGamePoint==0) m_bMeSide=bMeSide;
	else m_iGamePoint+=iGamePoint;

	UpdateViewFace(NULL);
	
	return;
}

/**
 * ���ñ���ɫ
 */
void CClientPlayView::SetBackBmp(UINT uResourceID, TCHAR * szFileName, BOOL bResource)
{
	if (bResource)
	{
		CBitmap Temp;
		if (!Temp.LoadBitmap(uResourceID)) return;
		return;
	}
	else
	{
		if (szFileName==NULL) return;
	}
	return;
}

void CClientPlayView::DrawInfo(CDC *pDC)
{
	return;
}
/*******************************************************************************************************/


/**
 * �����ǳƵ������Ƶ���ھ���д�����rect,
 * @return ������TRUE;û�ҵ�����falst
 */
#ifdef	VIDEO
BOOL	CClientPlayView::GetVideoRect(CString  strNickname, RECT& rect, CString& pic)
{
	CString strLocalName;
	for(int iCount=0; iCount<MAX_PEOPLE; iCount++)
	{
		if(m_pUserItem[iCount])
		{    
			strLocalName=m_pUserItem[iCount]->GameUserInfo.nickName;
			
			if(strLocalName == strNickname)
			{
				rect=m_RtVideo[iCount];
				m_MeCard.ClientToScreen(&rect);
				ScreenToClient(&rect);
				CString logopathstr;
				
				logopathstr.Format("image\\log\\gamelog%d.bmp", m_pUserItem[iCount]->GameUserInfo.bLogoID);
				
				CString  tempstr = CINIFile::GetAppPath()+logopathstr;
				
				pic = tempstr;
				return true;
			}
		}
	
	}	 
	return false; //û�ҵ�
}
#endif
//��ʾ����
BOOL CClientPlayView::PreTranslateMessage(MSG * pMsg)
{
	if (pMsg->hwnd==m_showcoredlg.m_hWnd && m_bUpdateScoreboard)
	{
		// ���û��ְ��ı���ɫ
		//CBcfFile f( "GameOption.bcf");
		CString ss = GET_SKIN_FOLDER(szTempStr);
		ss += ".bcf";
		CBcfFile f(ss);
		CString key = TEXT("UISet");
		int uColorR = f.GetKeyVal(key,TEXT("ScorBarR"), 255);
		int uColorG = f.GetKeyVal(key,TEXT("ScorBarG"), 255);
		int uColorB = f.GetKeyVal(key,TEXT("ScorBarB"),255);
		m_showcoredlg.SetTextColor(RGB(uColorR, uColorG, uColorB));
		m_bUpdateScoreboard = false;
	}

//	m_ToolTip.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}