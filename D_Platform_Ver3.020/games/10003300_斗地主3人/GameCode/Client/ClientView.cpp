#include "StdAfx.h"
#include "ClientView.h"
#include "ClientGameDlg.h"
//#include "FlashModule\shockwaveflash1.cpp"
//CBitmap				CClientPlayView::m_Bitmap_Num;


BEGIN_MESSAGE_MAP(CClientPlayView, CUsingSkinClass)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CONTINUE, OnHitContinue)
	ON_BN_CLICKED(IDC_LEAVE, OnHitLeave)
	ON_BN_CLICKED(IDC_HAVE_THING, OnHitThing)
	ON_BN_CLICKED(IDC_AUTOCARD, OnHitAuto)
	ON_BN_CLICKED(IDC_SORT_CARD,OnHitSort)
	ON_BN_CLICKED(IDC_LAST_CARD,OnHitLastTurn)	
	ON_BN_CLICKED(IDC_OUT_CARD,OnHitOutCard)
	ON_BN_CLICKED(IDC_CUE,OnCue)
	ON_BN_CLICKED(IDC_PASS,OnHitPass)
	ON_BN_CLICKED(IDC_COUNTER , OnHitCounter)
	ON_BN_CLICKED(IDC_DIALECT , OnHitDialect)
	ON_COMMAND_RANGE(IDC_CALL_SCORE_PASS,IDC_CALL_SCORE_3,OnHitCallScore)
	//�Ӱ�������
	ON_BN_CLICKED(IDC_ROB_NT,OnHitRobNT)	
	ON_BN_CLICKED(IDC_ROB_NT_2,OnHitRobNT2)	
	ON_BN_CLICKED(IDC_ADD_DOUBLE,OnHitAddDouble)	
	ON_BN_CLICKED(IDC_ADD_DOUBLE_2,OnHitAddDouble2)	
	ON_BN_CLICKED(IDC_SHOW_CARD,OnHitShowCard)	
	ON_BN_CLICKED(IDC_SHOW_CARD_2,OnHitShowCard2)
	ON_BN_CLICKED (IDC_CHANGE_SHIN,OnChangeShin)  //����
	ON_BN_CLICKED(IDC_CHUI_YES,OnHitChuiYes)	//��	
	ON_BN_CLICKED(IDC_CHUI_NO,OnHitChuiNo)		//����

	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

/*******************************************************************************************************/

//���캯��
CClientPlayView::CClientPlayView(void) //:m_GameSet(this),m_GameInfo(this)//,m_GameEndView(this)
{
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
	//c++ test
	for(int i=0; i<PLAY_COUNT; i++)
	{
		m_PtNt[i].SetPoint(0,0);
		m_PtAuto[i].SetPoint(0,0);
		m_PtAwardPoint[i].SetPoint(0,0);
		m_PtSequence[i].SetPoint(0,0);
		m_PtPass[i].SetPoint(0,0);
		m_PtLastCardFlag[i].SetPoint(0,0);
		m_PtGetPoint[i].SetPoint(0,0);
		m_PtCallScore[i].SetPoint(0,0);
		m_PtPeopleMul[i].SetPoint(0,0);
		m_PtGif[i].SetPoint(0,0);
		m_PtCurOrientation[i].SetPoint(0,0);
		ptCardPoint[i].x = 0;
		ptCardPoint[i].y = 0;
		ptHandCard[i].x = 0;
		ptHandCard[i].y = 0;
		m_PtStart[i].SetPoint(0,0);
		m_PtSendCardEnd[i].SetPoint(0,0);
		m_PtSendCardStart[i].SetPoint(0,0);

		m_iPass[i]= 0;
		m_iCallScore[i] = 0;
		m_iAwardPoint[i] = 0;
		m_bAuto[i] = false;
		m_iPeopleMul[i] =0;
		m_bHaveSendCardAnimal[i] = false;

		m_HandCard[i].SetShowCard(FALSE);//�����ò���ʾ��״̬
		m_HandCard[i].SetCard(NULL,NULL,0);
		m_MeBackCard.SetCard(NULL,NULL,0);
		m_MeCard.SetCard(NULL,NULL,0);
	}
	//m_dlgDisTalk m_UserCard m_HandCard m_SendCard
	m_Bt1WidthSpace = 0;
	m_Bt1HeightSpace = 0;
	m_Bt2WidthSpace= 0; 
	m_Bt2HeightSpace= 0;
	m_iDeskTempPoint= 0;
	m_iDeskMultiple= 0;
	//x = 0;
	SetNameColor(RGB(255,255,0));			//����Ĭ���û�����ɫ
	memset(m_iPass,0,sizeof(m_iPass));
	m_iCurOrientation=-1;	//���Ʒ���
	m_iRoomMultiple=0;
	m_iGameMultiple=0;
	m_iNtStation=-1;
	m_iCallScoreResult = -1;
	m_iCueType = 3;
	::memset(m_iCallScore,-1,sizeof(m_iCallScore));
	::memset(m_iPeopleMul,1,sizeof(m_iPeopleMul));
	m_bCanLeave = true;
	
	m_bInit = false;

	m_bAIChooseCard = false ; 

	m_bFirstGame = true;		//090310a1 JJ 
	m_bUpdateScoreboard=true;
	//AfxSetResourceHandle(GetModuleHandle(NULL));

	CString fpath = CBcfFile::GetAppPath();
	//fpath += "GameOption.bcf";
	fpath +=  GET_CLIENT_BCF_NAME(szTempStr);
	CBcfFile file(fpath);
	CString firstKey(TEXT("UISet"));

	m_NameFont_Height	= file.GetKeyVal(firstKey,TEXT("NameFontHeight"),18);
	m_NameFont_Width	= file.GetKeyVal(firstKey,TEXT("NameFontWidth"),16);
	m_NameTextOffset_Height	= file.GetKeyVal(firstKey,TEXT("NameTextOffset_Height"),0);
	m_NameTextOffset_Width	= file.GetKeyVal(firstKey,TEXT("NameTextOffset_Width"),0);
	m_JinBiFont_Height	= file.GetKeyVal(firstKey,TEXT("JinBiFontHeight"),14);
	m_JinBiFont_Width	= file.GetKeyVal(firstKey,TEXT("JinBiFontWidth"),7);
	m_JinBiTextOffset_Height = file.GetKeyVal(firstKey,TEXT("JinBiTextOffset_Height"),0);
	m_JinBiTextOffset_Width	 = file.GetKeyVal(firstKey,TEXT("JinBiTextOffset_Width"),0);

	m_bShowLogo	  = file.GetKeyVal("config" , "ShowLogo", 0 );
	m_bShowMoney  = file.GetKeyVal("config" , "ShowMoney" ,0) ; 
	m_strCoinName = file.GetKeyVal("config" , "coinName" ,"���") ;

	CString secKey;
	for (int i=0; i<PLAY_COUNT; i++)
	{
		secKey.Format("UserNameOffset%d_x",i);
		m_UserNameOffset_x[i] = file.GetKeyVal(firstKey,secKey,0);
		secKey.Format("UserNameOffset%d_y",i);
		m_UserNameOffset_y[i] = file.GetKeyVal(firstKey,secKey,0);
		secKey.Format("JinBiOffset%d_x",i);
		m_JinBiOffset_x[i] = file.GetKeyVal(firstKey,secKey,0);
		secKey.Format("JinBiOffset%d_y",i);
		m_JinBiOffset_y[i] = file.GetKeyVal(firstKey,secKey,0);
	}
	m_iCurrentBG = 0;		//��ǰ����ͼƬ
	m_iCurrentStitch = 5;	//������ǰ����
	::memset(m_iChui,1,sizeof(m_iChui));	//�Ƿ�һ�� -1:δ֪״̬��0:������1:��
	return;
}

//��������
CClientPlayView::~CClientPlayView(void)
{
}

//�����û�����
void CClientPlayView::UpdateUserInfo(BYTE bViewStation)
{
	UpdateViewFace(NULL);
	return;
}

//�滭���溯��
void CClientPlayView::DrawViewFace(CDC * pDC, int iWidth, int iHeight)
{

	DrawBk(pDC,iWidth,iHeight);

	CRect rect;
	GetClientRect(&rect);

#ifndef	POINT5_D 
	CGameImageHelper yichunxs_bg(&m_Background);				//����ͼ��LOGO		
	int x=(rect.right-yichunxs_bg.GetWidth())/2;
	int y=((rect.bottom)-yichunxs_bg.GetHeight())/2+30;
	yichunxs_bg.BitBlt(pDC->GetSafeHdc(),x,y);
	//��logo
	DrawGameTitle(pDC);
#endif


	TCHAR sz[100];
	POINT offsetLogo={0,0};
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	
	if((p->m_pGameInfo->dwRoomRule & GRR_VIDEO_ROOM)!=0)
	{
		for (int i=0;i<PLAY_COUNT;i++)
		{
			if (m_pUserItem[i]!=NULL)
			{
				UserInfoStruct * pUserInfo=&m_pUserItem[i]->GameUserInfo;
				pDC->SetTextColor(RGB(255,251,4));		//�û��ǳ���ɫ
				//pDC->SetTextColor(GetNameColor(&(m_pUserItem[i]->GameUserInfo)));		//�û��ǳ���ɫ
				CGameImageHelper bg(&m_pVideoFrame);

				POINT offsetAgree={0,0};
				RECT  rectName,rectcars;  ///�ǳƺ�ʣ�������Ŀ�	

				int iMyNameMsgPos = 0;
				/*if (1==i)
				{
					CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
					if (p != NULL && p->IsQueueGameRoom())
						iMyNameMsgPos = -10;
				}*/

				offsetLogo.x	= m_rectVideoFrame[i].left + 40;
				offsetLogo.y	= m_rectVideoFrame[i].top +30 + iMyNameMsgPos;

 				rectName.left	= m_rectVideoFrame[i].left + 20;
 				rectName.top	= m_rectVideoFrame[i].top + 2 + iMyNameMsgPos;
 				rectName.right	= rectName.left + bg.GetWidth()- 1;
 				rectName.bottom	= rectName.top + 100;
				
				rectcars.left	= m_rectVideoFrame[i].left + 25;
				rectcars.top	= m_rectVideoFrame[i].bottom + 25 ;
				rectcars.right	= rectcars.left +47;
				rectcars.bottom	= rectcars.top + 20;

				offsetAgree.x	= m_rectVideoFrame[i].left;
				offsetAgree.y	= m_rectVideoFrame[i].top -12;	
				
				///��Ƶ��ͼ
				CDC dc;
				dc.CreateCompatibleDC(pDC);
				dc.SelectObject(bg);
				::TransparentBlt(pDC->GetSafeHdc(),m_rectVideoFrame[i].left,m_rectVideoFrame[i].top,m_rectVideoFrame[i].Width(),m_rectVideoFrame[i].Height(),dc.GetSafeHdc(),0,0,m_pVideoFrame.GetWidth(),m_pVideoFrame.GetHeight(),dc.GetPixel(0,0));
				dc.DeleteDC();

				DrawLogo(pDC,m_pUserItem[i]->GameUserInfo.bLogoID,offsetLogo.x, offsetLogo.y,m_pUserItem[i]->GameUserInfo.bUserState==USER_CUT_GAME,
					m_pUserItem[i]->GameUserInfo.dwUserID,false,false);

				//�ǳ�
				wsprintf(sz,"%s",m_pUserItem[i]->GameUserInfo.nickName); 
				UINT uFlag = (-10==iMyNameMsgPos) ? DT_CENTER : DT_LEFT;
					CFont newFont,*OldFont; 
					if (i == 1)
					{
						newFont.CreateFont(12,0,0,0,0,0,0,0,134,3,2,1,2,TEXT("����"));
						OldFont = pDC->SelectObject(&newFont);
					}
					else if (i == 2)
					{
						newFont.CreateFont(12,0,2700,0,0,0,0,0,134,3,2,1,2,TEXT("����"));
						OldFont = pDC->SelectObject(&newFont);
					}
					else
					{
						newFont.CreateFont(12,0,900,0,0,0,0,0,134,3,2,1,2,TEXT("����"));
						OldFont = pDC->SelectObject(&newFont);
					}

				pDC->DrawText(sz,lstrlen(sz),&rectName,uFlag|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS|DT_CENTER);

				/*if (i == 1)
				{
					CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
					if (p != NULL && p->IsQueueGameRoom())
					{
						CString strNum;
						p->GetNumString(m_pUserItem[i]->GameUserInfo.i64Money,strNum,Glb().m_nPowerOfGold);
						wsprintf(sz,TEXT("%s:%s"),m_strCoinName, strNum);
						rect.left=rectName.left;
						rect.right=rect.left+145;
						rect.top=m_PtName[i].y+15+iMyNameMsgPos;
						rect.bottom=rect.top +20;
						pDC->DrawText(sz,lstrlen(sz),&rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
					}
				}*/
				
				//��ʱ��
				if (m_uTime[i]!=0)
				{
					if (!m_Result.IsWindowVisible())//��ֹ���Y����					
						DrawTimer(pDC,m_PtTime[i].x,m_PtTime[i].y,m_uTime[i]);					
					else					
						DrawTimer(pDC, iWidth/2-15, iHeight/2-11, m_uTime[i]);					
				}
				//ͬ���ʶ
				if (m_pUserItem[i]->GameUserInfo.bUserState==USER_ARGEE)
					DrawArgeeFlag(pDC,offsetAgree.x, offsetAgree.y+iMyNameMsgPos);
			}
		}
	}
	else
	{
		CGameImageHelper hlpLogoFrame(&m_imgLogoFrame);
		for (int i = 0; i < PLAY_COUNT; i ++)
		{			
			if (m_pUserItem[i] == NULL)
				continue;
			pDC->SetTextColor(RGB(255,251,4));		//�û��ǳ���ɫ			
			int iMyNameMsgPos = 0;

			UserInfoStruct * pUserInfo = &m_pUserItem[i]->GameUserInfo;
			wsprintf(sz,TEXT("�ǳ�:%s"),pUserInfo->nickName);

			CFont newFont,*OldFont; 

			CPoint offsetLogo;
			offsetLogo.x = m_PtLogo[i].x;
			offsetLogo.y = m_PtLogo[i].y +  iMyNameMsgPos;
			if (m_iUserFrame.GetWidth() > 0)
			{
				offsetLogo.x += (m_iUserFrame.GetWidth() - LOGO_WIDTH)/2;
				offsetLogo.y += (m_iUserFrame.GetHeight() - LOGO_HEIGHT)/2;
			}

			//ͷ��׿�
			CDC dc;
			dc.CreateCompatibleDC(pDC);
			CBitmap *pOldBmp = dc.SelectObject(hlpLogoFrame);
			::TransparentBlt(pDC->GetSafeHdc(), offsetLogo.x-15, offsetLogo.y-8, \
				hlpLogoFrame.GetWidth(), hlpLogoFrame.GetHeight(), dc.GetSafeHdc(), 0, 0, \
				hlpLogoFrame.GetWidth(), hlpLogoFrame.GetHeight(), RGB(255,0,255));
			dc.SelectObject(pOldBmp);

			if(m_bShowLogo)
			{
            #ifdef SHAO_YANG
                int OffsetX = -20;
                int OffsetY = 0;

                if (0 == i)         //��
                {
                    OffsetX = 0;
                    OffsetY = -60;
                }
                else if (1 == i)    //��
                {
                    OffsetX = -45;
                    OffsetY = -35;
                }
                else if (2 == i)    //��
                {
                    OffsetX = 25;
                    OffsetY = -60;
                }

                DrawLogo(pDC,m_pUserItem[i]->GameUserInfo.bLogoID,offsetLogo.x + OffsetX,offsetLogo.y + OffsetY,m_pUserItem[i]->GameUserInfo.bUserState==USER_CUT_GAME,
                    m_pUserItem[i]->GameUserInfo.dwUserID,false,false);

            #else
                DrawLogo(pDC,m_pUserItem[i]->GameUserInfo.bLogoID,offsetLogo.x,offsetLogo.y,m_pUserItem[i]->GameUserInfo.bUserState==USER_CUT_GAME,
                    m_pUserItem[i]->GameUserInfo.dwUserID,false,false);

            #endif

			}

			if(i == 1 )
			{
				rect.left = m_PtLogo[i].x +80 ;
				rect.right = rect.left + 120;

				rect.top = m_PtLogo[i].y+iMyNameMsgPos + 41;
				rect.bottom = rect.top+20;
			}
			else
			{
				rect.left = m_PtLogo[i].x +35  ;
				rect.right = rect.left + 120;

				rect.top = m_PtLogo[i].y+iMyNameMsgPos + 80;
				rect.bottom = rect.top+20;
			}

			newFont.CreateFont(12,0,0,0,12,0,0,0,134,3,2,1,2,TEXT("����"));
			OldFont = pDC->SelectObject(&newFont);		

			pDC->DrawText(sz,lstrlen(sz),&rect,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
			newFont.DeleteObject();	

			CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
			if (p != NULL && m_bShowMoney)
			{
				CRect iMoneyRect ; 
				iMoneyRect.left = rect.left ; 
				iMoneyRect.right = iMoneyRect.left + 120 ;
				iMoneyRect.top = rect.top + 20 ; 
				iMoneyRect.bottom = iMoneyRect.top + 20 ;

				//CString strNum1;
				char strNum1[MAX_PATH] = {0};
				//p->GetNumString(m_pUserItem[i]->GameUserInfo.i64Money,strNum1,Glb().m_nPowerOfGold);
				GlbGetNumString(strNum1,m_pUserItem[i]->GameUserInfo.i64Money,Glb().m_nPowerOfGold,FALSE,"");
				wsprintf(sz,TEXT("%s:%s"),m_strCoinName ,strNum1);

				pDC->SetTextColor(RGB(255,255,0));
				newFont.CreateFont(12,0,0,0,12,0,0,0,134,3,2,1,2,TEXT("����"));
				OldFont = pDC->SelectObject(&newFont);

				pDC->DrawText(sz,lstrlen(sz),&iMoneyRect,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
				pDC->SelectObject(OldFont);
				newFont.DeleteObject();
			}

			if (i == 1)
			{
				CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
				/*if (p != NULL && p->IsQueueGameRoom())
				{
					CString strNum;
					p->GetNumString(m_pUserItem[i]->GameUserInfo.i64Money,strNum,Glb().m_nPowerOfGold);
#ifdef TWENTY
					wsprintf(sz,TEXT("ͭ��:%s"),strNum);
#else
					wsprintf(sz,TEXT("%s:%s"),m_strCoinName,strNum);
#endif
					rect.left=offsetLogo.x-30;
					rect.right=rect.left+hlpLogoFrame.GetWidth()+30;
					rect.top=m_PtName[i].y+15+iMyNameMsgPos;
					rect.bottom=rect.top+20;
					pDC->DrawText(sz,lstrlen(sz),&rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

					if(m_bShowLogo)
					{
						rect.left = m_PtName[i].x;
						rect.right = rect.left+120;
						rect.top=m_PtName[i].y+5;
						rect.bottom=rect.top+20;								
						pDC->DrawText(sz,lstrlen(sz),&rect,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
					}
				}*/
		
#ifdef TWENTY
				wsprintf(sz,TEXT("�챦ʯ����%d/20  ����ʯ��: %d/20"),m_pUserItem[i]->GameUserInfo.iRedDiamondGameCnt,m_pUserItem[i]->GameUserInfo.iBlueDiamondGameCnt);
				rect.left=offsetLogo.x-30;
				rect.right=rect.left+200;
				rect.top=rect.top+20;
				rect.bottom=rect.top+20;
				pDC->DrawText(sz,lstrlen(sz),&rect,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
#endif
			}
			if (m_uTime[i] != 0) 
			{
				if (!m_Result.IsWindowVisible())//�Y��򲻳��F�r				
					DrawTimer(pDC,m_PtTime[i].x,m_PtTime[i].y,m_uTime[i]);				
				else				
					DrawTimer(pDC, iWidth/2-15+240, iHeight/2+150+8, m_uTime[i]);				
			}
			if (pUserInfo->bUserState == USER_ARGEE)
				DrawArgeeFlag(pDC,m_PtAgree[i].x,m_PtAgree[i].y+iMyNameMsgPos);
		}
	}

	//�滭ׯ��λ��
	if (m_iNtStation != -1) 
	{
		int iMyNameMsgPos=0;
		if (1==m_iNtStation)
		{
			CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
			if (p != NULL && p->IsQueueGameRoom())
				iMyNameMsgPos = -10;
		}

		DrawNt(pDC,m_PtNt[m_iNtStation].x,m_PtNt[m_iNtStation].y +  iMyNameMsgPos);		
	}
	//GIF����
	DrawGif(pDC,m_PtGif[m_iGifDeskStation].x,m_PtGif[m_iGifDeskStation].y);
	//�з���ʾ��Ϣ
	DrawShowMsg(pDC);
	//�зֽ��	
	DrawCallScoreResult(pDC);
	//����
	DrawRoomMultiple(pDC);
	//�з�
	DrawFen(pDC);
	//����
	DrawPass(pDC);
	//�й�
	DrawAuto(pDC);
	//����
	DrawAwardPoint(pDC);
	//���Ƴ���ָʾ����
	DrawOrientation(pDC);
	//�Ӱ�
	DrawPeopleMultiple(pDC);

#ifdef POINT5_D
		DarwCardImage(pDC);	//��2.5D�Ʊ�

		DarwChuiAnim(pDC,iWidth,iHeight);	//������ͼƬ

	#ifdef SHAO_YANG
		DarwChuiSign(pDC);	//���ƴ�һ����־
	#endif
#endif

	return;
}
//������Ϸ����Сͼ
void CClientPlayView::DrawBk(CDC *pDC, int iWidth, int iHeight)
{

#ifdef POINT5_D
	if(NULL == m_ImgBackground[m_iCurrentBG])
	{
		return;
	}

	Graphics GDIDC(pDC->GetSafeHdc());	

	GDIDC.DrawImage(m_ImgBackground[m_iCurrentBG],Rect(0, 0, iWidth, iHeight));
#else
	CGameImageHelper bg(&m_bit_background);
	if(m_bit_background.GetHeight()>0)
	{
		for(int i = 0; i < iHeight; i += m_bit_background.GetHeight())
		{
			for(int j = 0; j < iWidth; j += m_bit_background.GetWidth())
			{	
				bg.BitBlt(pDC->GetSafeHdc(), j ,i);	
			}	
		}
	}
#endif

	return;
}

//������Ϸ����logo
void CClientPlayView::DrawGameTitle(CDC *pDC)
{
	CGameImageHelper bg(&m_GameTitleLogo);

	bg.BitBlt(pDC->GetSafeHdc(),m_PtGameTitle.x,m_PtGameTitle.y);	
	return ;
}

//͸������
void CClientPlayView::TransparentBlt2( HDC hdcDest,       // Ŀ��DC
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
	//	SetTextColor(hImageDC,RGB(255,255,255));
	// ����͸������Ϊ��ɫ����������Ϊ��ɫ������λͼ(s|~b)
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);

	// ����͸������Ϊ��ɫ���������򱣳ֲ����λͼ
	SetBkColor(hImageDC, RGB(0,0,0));
	SetTextColor(hImageDC, RGB(255,255,255));//(s&b)
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);

	// ͸�����ֱ�����Ļ���䣬�������ֱ�ɺ�ɫ
	SetBkColor(hdcDest,RGB(255,255,255));
	SetTextColor(hdcDest,RGB(0,0,0));//(s&b)
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

//������Ϣ
int CClientPlayView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CUsingSkinClass::OnCreate(lpCreateStruct) == -1)	return -1;
	//�����ؼ�
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));

	//��������ͼƬ
	TCHAR path[MAX_PATH];

	wsprintf(path,TEXT(".\\image\\cardnum.bmp"));
	m_Num.SetLoadInfo(path,CGameImageLink::m_bAutoLock);


	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	if((p->m_pGameInfo->dwRoomRule & GRR_VIDEO_ROOM)!=0)
	{
		wsprintf(path, (".\\image\\VideoFrame.bmp"));
		m_pVideoFrame.SetLoadInfo(path,false);

	}
	else
	{
		if(m_iUserFrame.GetWidth() <= 0)
		{
			wsprintf(path,TEXT(".\\image\\logoframe.bmp"));
			SetFrameImage(path);
		}
	}

	//wsprintf(path,TEXT(".\\image\\logoframe.bmp"));
	//m_imgLogoFrame.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	wsprintf(path,TEXT(".\\image\\BackColorOnly.bmp"));
	m_bit_background.SetLoadInfo(path,CGameImageLink::m_bAutoLock);				//С��ͼ����

	wsprintf(path,TEXT(".\\image\\centerlogo.bmp"));
	m_Background.SetLoadInfo(path,CGameImageLink::m_bAutoLock);		//����ͼƬ

#ifdef POINT5_D
	CString strTemp = "";

	for(int i = 0; i < 5; ++i)
	{
		if(i < 4)
		{
			strTemp.Format(".\\image\\Background%d.png",i);
			m_ImgBackground[i] = Image::FromFile(CA2W(strTemp));	//����ͼƬ
		}

		strTemp.Format(".\\image\\Anim\\chui%d.png",i);
		m_ImageAnim[i] = Image::FromFile(CA2W(strTemp));
	}

	strTemp=".\\image\\CBUC.png";
	m_pChuiSign = Image::FromFile(CA2W(strTemp));
#endif

	wsprintf(path,TEXT(".\\image\\Nt.bmp"));
	m_NT.SetLoadInfo(path,false);						

	wsprintf(path,TEXT(".\\image\\NTFriend.bmp"));
	m_NT2.SetLoadInfo(path,false);						

	wsprintf(path,TEXT(".\\image\\AwardPoint1.bmp"));
	m_AwardPint.SetLoadInfo(path,false);						

	wsprintf(path,TEXT(".\\image\\CallScore2_pass.bmp"));
	m_CallScore.SetLoadInfo(path,false);					

	wsprintf(path,TEXT(".\\image\\tuoguan.bmp"));
	m_AutoLogo.SetLoadInfo(path,false);	

	wsprintf(path,TEXT(".\\image\\pass.bmp"));
	m_Pass.SetLoadInfo(path,false);							

	wsprintf(path,TEXT(".\\image\\lastcardflag.bmp"));
	m_LastCardFlag.SetLoadInfo(path,false);						

	//����
	wsprintf(path,TEXT(".\\image\\CurOutCard.bmp"));
	m_ImgFangXiang.SetLoadInfo(path,false);						

	//��ұ���
	wsprintf(path,TEXT(".\\image\\ban.bmp"));
	m_PeopleMul.SetLoadInfo(path,true);						

	//�зֱ���
	wsprintf(path,TEXT(".\\image\\showmsg.bmp"));
	m_ShowMsg.SetLoadInfo(path,false);						

	m_btOutCard.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,56,26),this,IDC_OUT_CARD);
	m_btPass.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,56,26),this,IDC_PASS);
	m_btCue.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,56,26),this,IDC_CUE);

	m_btContinue.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,56,26),this,IDC_CONTINUE);
	m_btLeave.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|BS_OWNERDRAW,CRect(0,0,0,0),this,IDC_LEAVE);

	m_btLastTurn.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,56,26),this,IDC_LAST_CARD);
	m_btThing.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,56,26),this,IDC_HAVE_THING);
	m_btAuto.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,56,26),this,IDC_AUTOCARD);
	m_btSortCard.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,56,26),this,IDC_SORT_CARD);
	m_btCounter.Create(TEXT("") , WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,0,0),this , IDC_COUNTER) ;
	m_btdialect.Create(TEXT("") , WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,0,0),this , IDC_DIALECT) ;

	m_GameNoteView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,0,0),this,50,NULL);
	m_GameNoteView.AutoSize();
	m_Result.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,0,0),this,51,NULL);
	m_CounterDlg.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,0,0),this, 51, NULL);
	m_DialectDlg.Create(IDD_DIALECT,this) ;

    // PengJiLin, 2011-4-19, ����ʱ���������ͽ��ͼ����ʾ
    m_GetMoneyImageView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,0,0),this,52,NULL);
    m_GetMoneyImageView.AutoSize();

	m_btCallScorePass.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,25,25),this,IDC_CALL_SCORE_PASS);
	m_btCallScore1.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,25,25),this,IDC_CALL_SCORE_1);
	m_btCallScore2.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,25,25),this,IDC_CALL_SCORE_2);
	m_btCallScore3.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,25,25),this,IDC_CALL_SCORE_3);
	//�Ӱ�������
	m_btRobNT.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,25,25),this,IDC_ROB_NT);
	m_btRobNT2.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,25,25),this,IDC_ROB_NT_2);

	m_btAddDouble.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,25,25),this,IDC_ADD_DOUBLE);
	m_btAddDouble2.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,25,25),this,IDC_ADD_DOUBLE_2);

	m_btShowCard.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,25,25),this,IDC_SHOW_CARD);
	m_btShowCard2.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,25,25),this,IDC_SHOW_CARD_2);

	//����
	m_btChangeShin.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,80,25),this,IDC_CHANGE_SHIN);
	m_btChangeShin.ShowWindow(SW_HIDE);

	//��
	m_btChuiYes.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,80,25),this,IDC_CHUI_YES);
	m_btChuiYes.ShowWindow(SW_HIDE);

	//����
	m_btChuiNo.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS,CRect(0,0,80,25),this,IDC_CHUI_NO);
	m_btChuiNo.ShowWindow(SW_HIDE);

	//��Ҫ�������....
	//for(int i = 0;i< PLAY_COUNT;i++)
	//{
	//	m_dlgDisTalk[i].m_viewid = i;
	//	m_dlgDisTalk[i].Create(IDD_DISPLAY_TALK,this);  //����������Ϣ��
	//	m_dlgDisTalk[i].ShowWindow(SW_HIDE);             //����������
	//}
	m_ToolTip.Create(this);

	m_ToolTip.AddTool((CWnd*)&m_btThing,TEXT("����Ҫ��"));
	m_ToolTip.AddTool((CWnd*)&m_btAuto,TEXT("�й�"));
	m_ToolTip.AddTool((CWnd*)&m_btSortCard,TEXT("����"));

	m_ToolTip.AddTool((CWnd*)&m_btLeave,TEXT("�뿪"));

	CGameImage m_bk;
	HRGN hRgn;
	TCHAR szName[MAX_PATH];
	wsprintf(szName,".\\image\\leave_bt.bmp");	//�뿪��ť
	m_btLeave.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btLeave.SetWindowRgn(hRgn,true);

	wsprintf(szName,TEXT(".\\image\\start_bt.bmp"));	//��ʼ��ť
	m_btContinue.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btContinue.SetWindowRgn(hRgn,true);

	wsprintf(szName,TEXT(".\\image\\OutCard_bt.bmp"));			//���ư�ť
	m_btOutCard.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btOutCard.SetWindowRgn(hRgn,true);

	wsprintf(szName,TEXT(".\\image\\Pass_bt.bmp"));			//Pass��ť
	m_btPass.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btPass.SetWindowRgn(hRgn,true);

	wsprintf(szName,TEXT(".\\image\\Cue_bt.bmp"));			//��ʾ��ť
	m_btCue.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btCue.SetWindowRgn(hRgn,true);

	wsprintf(szName,TEXT(".\\image\\auto_bt.bmp"));			//�йܰ�ť
	m_btAuto.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btAuto.SetWindowRgn(hRgn,true);

	wsprintf(szName,TEXT(".\\image\\havething_bt.bmp"));			//����Ҫ�߰�ť
	m_btThing.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btThing.SetWindowRgn(hRgn,true);

	wsprintf(szName,TEXT(".\\image\\sort_bt.bmp"));			//����ť
	m_btSortCard.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btSortCard.SetWindowRgn(hRgn,true);

	wsprintf(szName,TEXT(".\\image\\leave_bt.bmp"));	//�뿪��ť		
	m_btLeave.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btLeave.SetWindowRgn(hRgn,true);

	//��һ��
	wsprintf(szName,TEXT(".\\image\\Lastturn_bt.bmp"));			
	m_btLastTurn.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btLastTurn.SetWindowRgn(hRgn,true);	

	//������
	wsprintf(szName,TEXT(".\\image\\showhistory.bmp"));			
	m_btCounter.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btCounter.SetWindowRgn(hRgn,true) ; 

	//������ť
	wsprintf(szName,TEXT(".\\image\\dialect_bt.bmp"));			
	m_btdialect.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btdialect.SetWindowRgn(hRgn,true) ; 

	//������
	wsprintf(szName,TEXT(".\\image\\RobNt_bt.bmp"));			
	m_btRobNT.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btRobNT.SetWindowRgn(hRgn,true);

	//��������
	wsprintf(szName,TEXT(".\\image\\RobNt2_bt.bmp"));		
	m_btRobNT2.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btRobNT2.SetWindowRgn(hRgn,true);	
	//�Ӱ�
	wsprintf(szName,TEXT(".\\image\\AddDouble_bt.bmp"));		
	m_btAddDouble.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btAddDouble.SetWindowRgn(hRgn,true);
	//���Ӱ�
	wsprintf(szName,TEXT(".\\image\\AddDouble2_bt.bmp"));			
	m_btAddDouble2.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btAddDouble2.SetWindowRgn(hRgn,true);	

	//����
	wsprintf(szName,TEXT(".\\image\\ShowCard_bt.bmp"));			
	m_btShowCard.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btShowCard.SetWindowRgn(hRgn,true);
	//����
	wsprintf(szName,TEXT(".\\image\\ShowCard2_bt.bmp"));			
	m_btShowCard2.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btShowCard2.SetWindowRgn(hRgn,true);	

	//�з�1��ť
	wsprintf(szName,TEXT(".\\image\\jiaofen1_bt.bmp"));			
	m_btCallScore1.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btCallScore1.SetWindowRgn(hRgn,true);

	//�з�2��ť
	wsprintf(szName,TEXT(".\\image\\jiaofen2_bt.bmp"));			
	m_btCallScore2.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btCallScore2.SetWindowRgn(hRgn,true);

	//�з�3��ť
	wsprintf(szName,TEXT(".\\image\\jiaofen3_bt.bmp"));			
	m_btCallScore3.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btCallScore3.SetWindowRgn(hRgn,true);

	//���а�ť
	wsprintf(szName,TEXT(".\\image\\jiaofen_pass.bmp"));			
	m_btCallScorePass.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn=AFCBmpToRgn(m_bk,RGB(255,0,255),RGB(1,1,1));
	if(hRgn)
		m_btCallScorePass.SetWindowRgn(hRgn,true);

	//������ť
	wsprintf(szName, ".\\image\\changeshin_bt.bmp");//����
	m_btChangeShin.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn = AFCBmpToRgn(m_bk, RGB(255, 0, 255),RGB(1, 1, 1));
	if(hRgn)
		m_btChangeShin.SetWindowRgn(hRgn,true);

	//��
	wsprintf(szName, ".\\image\\chuiyes_bt.bmp");//����
	m_btChuiYes.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn = AFCBmpToRgn(m_bk, RGB(255, 0, 255),RGB(1, 1, 1));
	if(hRgn)
		m_btChuiYes.SetWindowRgn(hRgn,true);


	//����
	wsprintf(szName, ".\\image\\chuino_bt.bmp");//����
	m_btChuiNo.LoadButtonBitmap(szName,false);	
	m_bk.SetLoadInfo(szName,true);
	hRgn = AFCBmpToRgn(m_bk, RGB(255, 0, 255),RGB(1, 1, 1));
	if(hRgn)
		m_btChuiNo.SetWindowRgn(hRgn,true);


	
	//BLENDFUNCTION blend;
	//blend.AlphaFormat = 1;
	//blend.BlendFlags = 0;
	//blend.BlendOp = 0;
	//blend.SourceConstantAlpha = 255;

#ifdef  VIDEO
	for (int i = 0;i<PLAY_COUNT;i++)
	{		
		CRect iRect;
		iRect.left = m_PtLogo[i].x;
		iRect.top = m_PtLogo[i].y;
		iRect.right = iRect.left +200;
		iRect.bottom = iRect.top +130;
		m_FlashPlayer[i].Create(NULL,WS_POPUP,iRect,this,400+i,NULL,NULL,NULL);	
		m_FlashPlayer[i].put_WMode("Transparent");
	}	
#endif
	


	///��ƵͼƬ
// 	wsprintf(szName,TEXT(".\\image\\myInfo.bmp"));
// 
// 	m_bk.SetLoadInfo(szName,true);
// 	hRgn=AFCBmpToRgn(m_bk,RGB(56,34,2),RGB(1,1,1));
// 	if(hRgn)
// 		m_FlashPlayer[1].SetWindowRgn(hRgn,true);

	//�����ͻ��˿�
	
	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		m_UserCard[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,CRect(0,0,0,0),this,20+i,NULL);
		m_UserCard[i].SetShowCard(true);

		m_HandCard[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,CRect(0,0,0,0),this,24+i,NULL);
		m_HandCard[i].SetShowCard(false);
		int wt=0, ht = 0;
		m_HandCard[i].GetCardWindthAndHeight(wt,ht);
		m_HandCard[i].SetCardParameter(wt/5,15,20);
		m_HandCard[i].SetCardShowNum(1);
	}
	m_MeCard.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,CRect(0,0,0,0),this,30,NULL);
	m_MeCard.InitGameCard(GetParent());
	m_MeBackCard.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS,CRect(0,0,0,0),this,31,NULL);
	m_MeBackCard.InitGameCard(GetParent());
	int wt=0, ht = 0;
	m_MeBackCard.GetCardWindthAndHeight(wt,ht);
	m_MeBackCard.SetCardParameter(wt+3,0,0);
	m_MeBackCard.SetShowCard(false);
	m_MeBackCard.SetUseMouse(false);
	m_MeBackCard.ShowWindow(SW_HIDE);

	m_btCallScorePass.ShowWindow(SW_HIDE);
	m_btCallScore1.ShowWindow(SW_HIDE);
	m_btCallScore2.ShowWindow(SW_HIDE);
	m_btCallScore3.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPass.ShowWindow(SW_HIDE);
	m_btCue.ShowWindow(SW_HIDE);


	//AfxSetResourceHandle(GetModuleHandle(NULL));

	m_bInit = true;
	return 0;
}

void CClientPlayView::FixControlSize(int cx,int cy)
{
	if(!m_bInit || !m_hWnd)
		return ;
	if ((cx==0)||(cy==0))
		return;

	//��������
	m_PtDeskTempPoint.x = 30;
	m_PtDeskTempPoint.y = 60;

	m_PtDeskMultiple.x = 30;
	m_PtDeskMultiple.y = 30;

	m_PtGameMultiple.x = 60;
	m_PtGameMultiple.y = 25;

	m_PtRoomMultiple.x = 60;
	m_PtRoomMultiple.y = 55;

	m_PtCallScoreResult.x = 30;
	m_PtCallScoreResult.y = 30;

	m_PtCallScoreNT.x = 30;
	m_PtCallScoreNT.y = 50;

	m_PtGameTitle.x = 0;
	m_PtGameTitle.y = 0;

	//�ƶ�����
	int iMidLine = cx/2;			//x�м���
	int iBaseLine = cy/2;		    //y�м���
	m_PtDeskCue.x = iMidLine;
	m_PtDeskCue.y = iBaseLine;	

	//ʹ������
	LoadSkinIni(cx,cy);
	//��ʱ������
	m_PtTime[0].x=iMidLine-TIME_WIDTH/2;
	m_PtTime[0].y=iBaseLine-TIME_HEIGHT/2;
	m_PtTime[2] = m_PtTime[1] = m_PtTime[0];

	POINT	spt;
	CGameImage img;
	TCHAR name[MAX_PATH];
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		int viewid = i;
		wsprintf(name,TEXT(".\\image\\usertalk%d.BMP"),viewid + 1);
		img.SetLoadInfo(name,true);
		switch(viewid)
		{
		case 0://��ȥͷ�����Ϣ����
			//	spt.x = m_PtLogo[0].x - img.GetWidth();
			//	spt.y = m_PtLogo[0].y + LOGO_WIDTH+2;
			//	break;
			//case 1:
			spt.x = m_PtLogo[0].x - img.GetWidth();
			spt.y = m_PtLogo[0].y;
			break;
		case 1:
			{
				spt.x = m_PtLogo[1].x + LOGO_WIDTH;
				spt.y = m_PtLogo[1].y - img.GetHeight() + 30;
			}
			break;
		case 2:
			{
				spt.x = m_PtLogo[2].x + LOGO_WIDTH;
				spt.y = m_PtLogo[2].y;
			}
			break;
		default:
			break ;
		}
		CRect rectDisplay(spt.x,spt.y,spt.x+img.GetWidth(),spt.y+img.GetHeight());
		if(m_dlgDisTalk[i].m_hWnd)
			m_dlgDisTalk[i].MoveWindow(&rectDisplay);
	}

	m_ptVideoPos[0].x = m_PtLogo[0].x-75;
	m_ptVideoPos[0].y = m_PtLogo[0].y-7;

	m_ptVideoPos[1].x = m_PtLogo[1].x-81;
	m_ptVideoPos[1].y = m_PtLogo[1].y-5;

	m_ptVideoPos[2].x = m_PtLogo[2].x-12;
	m_ptVideoPos[2].y = m_PtLogo[2].y-7;



	//�ƶ�һЩ�̶�����ֵ
	MoveScreen(cx,cy);
	m_bUpdateScoreboard=true;
	for (int i = 0;i<PLAY_COUNT;i++)
	{		
		if (i == 1)
		{
			   m_FlashPlayer[i].MoveWindow(m_ptVideoPos[i].x,
				m_ptVideoPos[i].y,
				VIDEO_WIDTH_MYSELF,
				VIDEO_HEIGHT_MYSELF);	
		}
		else
		{
			m_FlashPlayer[i].MoveWindow(m_ptVideoPos[i].x,
				m_ptVideoPos[i].y,
				VIDEO_WIDTH,
				VIDEO_HEIGHT);	
		}	
		//if(i != 1)
		{		
			SetVideoRgn(i);
		}
	}
	return;
}

//���ݼ��������ƶ�
void CClientPlayView::MoveScreen(int cx,int cy)
{
	if(!m_hWnd)
		return;
	int iMidLine = cx/2;
	int iBaseLine = cy/2;
	int y_Card = 95;
	//�ƶ��˿�
	m_HandCard[0].SetBasePoint(CPoint(ptHandCard[0].x -60,ptHandCard[0].y-5),
		CUpGradeGameCard::XShowMode::SWX_MODE_RIGHT,CUpGradeGameCard::YShowMode::SWY_MODE_CENTER);
	m_HandCard[0].SetLookMode(false);


	m_HandCard[1].SetBasePoint(CPoint(ptHandCard[1].x,ptHandCard[1].y),
		CUpGradeGameCard::XShowMode::SWX_MODE_CENTER,CUpGradeGameCard::YShowMode::SWY_MODE_TOP);
	m_HandCard[1].ShowWindow(SW_HIDE);

	m_HandCard[2].SetBasePoint(CPoint(ptHandCard[2].x +50 ,ptHandCard[2].y-5),
		CUpGradeGameCard::XShowMode::SWX_MODE_LEFT,CUpGradeGameCard::YShowMode::SWY_MODE_CENTER);
	m_HandCard[2].SetLookMode(false);

	//�ƶ��˿�
	m_UserCard[0].SetBasePoint(CPoint(ptCardPoint[0].x,ptCardPoint[0].y),
		CUpGradeGameCard::XShowMode::SWX_MODE_CENTER,CUpGradeGameCard::YShowMode::SWY_MODE_TOP);
	m_UserCard[1].SetBasePoint(CPoint(ptCardPoint[1].x,ptCardPoint[1].y),
		CUpGradeGameCard::XShowMode::SWX_MODE_CENTER,CUpGradeGameCard::YShowMode::SWY_MODE_TOP);
	m_UserCard[2].SetBasePoint(CPoint(ptCardPoint[2].x,ptCardPoint[2].y),
		CUpGradeGameCard::XShowMode::SWX_MODE_CENTER,CUpGradeGameCard::YShowMode::SWY_MODE_TOP);

	m_MeCard.SetBasePoint(CPoint(ptHandCard[1].x,ptHandCard[1].y ),
		CUpGradeGameCard::XShowMode::SWX_MODE_CENTER,CUpGradeGameCard::YShowMode::SWY_MODE_TOP);

#ifdef	POINT5_D
	//����λ��
	m_MeBackCard.SetBasePoint(CPoint(ptBackCard.x - 255,ptBackCard.y - 25),								//����
		CUpGradeGameCard::XShowMode::SWX_MODE_CENTER,CUpGradeGameCard::YShowMode::SWY_MODE_TOP);
#else
	//����λ��
	m_MeBackCard.SetBasePoint(CPoint(ptBackCard.x,ptBackCard.y),								//����
		CUpGradeGameCard::XShowMode::SWX_MODE_CENTER,CUpGradeGameCard::YShowMode::SWY_MODE_TOP);
#endif


	//�ƶ�������ͼ
	CRect ContronRect;
	//�ƶ���Ϣ��ͼ
	m_GameNoteView.GetClientRect(&ContronRect);

	m_GameNoteView.MoveWindow(iMidLine-ContronRect.Width() / 2,iBaseLine-ContronRect.Height() / 2,
		ContronRect.Width(),ContronRect.Height(),FALSE);
	//m_GameNoteView.AutoSize(iMidLine-ContronRect.Width() / 2,iBaseLine-ContronRect.Height() / 2);

	//�ƶ���Ϣ��ͼ
	m_GameNoteView.GetClientRect(&ContronRect);
	m_GameNoteView.MoveWindow(iMidLine-ContronRect.Width()/2,iBaseLine-ContronRect.Height()/2,
		ContronRect.Width(),ContronRect.Height(),FALSE);

	//�ƶ��ؼ�
	m_btOutCard.GetClientRect(&ContronRect);
	int btWidth = ContronRect.Width(),btHeight = ContronRect.Height();

	//����
	m_btOutCard.MoveWindow(iMidLine-2*ContronRect.Width(),m_Bt2HeightSpace+5,btWidth,btHeight);
	m_btPass.MoveWindow(iMidLine-ContronRect.Width()/2,m_Bt2HeightSpace+5,btWidth,btHeight);
	m_btCue.MoveWindow(iMidLine+ContronRect.Width(),m_Bt2HeightSpace+5,btWidth,btHeight);

	//�з�
	m_btCallScore1.GetClientRect(&ContronRect);
	btWidth = ContronRect.Width(),btHeight = ContronRect.Height();
	m_btCallScore1.MoveWindow(iMidLine-ContronRect.right*2 - 30,m_Bt2HeightSpace,btWidth,btHeight);
	m_btCallScore2.MoveWindow(iMidLine-ContronRect.right - 10,m_Bt2HeightSpace,btWidth,btHeight);
	m_btCallScore3.MoveWindow(iMidLine+10,m_Bt2HeightSpace,btWidth,btHeight);		
	m_btCallScorePass.MoveWindow(iMidLine+ContronRect.right + 30,m_Bt2HeightSpace,btWidth,btHeight);		

	//��������������
	m_btRobNT.GetClientRect(&ContronRect);
	btWidth = ContronRect.Width(),btHeight = ContronRect.Height();
	m_btRobNT.MoveWindow(iMidLine-2*ContronRect.Width(),m_Bt2HeightSpace,btWidth,btHeight);
	m_btRobNT2.MoveWindow(iMidLine+ContronRect.Width(),m_Bt2HeightSpace,btWidth,btHeight);
	//�Ӱ����Ӱ�
	m_btAddDouble.GetClientRect(&ContronRect);
	btWidth = ContronRect.Width(),btHeight = ContronRect.Height();
	m_btAddDouble.MoveWindow(iMidLine-2*ContronRect.Width(),m_Bt2HeightSpace,btWidth,btHeight);
	m_btAddDouble2.MoveWindow(iMidLine+ContronRect.Width(),m_Bt2HeightSpace,btWidth,btHeight);
	//����,����
	m_btShowCard.GetClientRect(&ContronRect);
	btWidth = ContronRect.Width(),btHeight = ContronRect.Height();
	m_btShowCard.MoveWindow(iMidLine-2*ContronRect.Width(),m_Bt2HeightSpace,btWidth,btHeight);
	m_btShowCard2.MoveWindow(iMidLine+ContronRect.Width(),m_Bt2HeightSpace,btWidth,btHeight);
	//��ʼ�뿪
	m_btContinue.GetClientRect(&ContronRect);
	btWidth = ContronRect.Width(),btHeight = ContronRect.Height();

	m_btContinue.GetClientRect(&ContronRect);
	//m_btContinue.MoveWindow(iMidLine +20,m_Bt2HeightSpace,ContronRect.Width(),ContronRect.Height());
	///m_btContinue.MoveWindow(iMidLine+20, iBaseLine+150, ContronRect.Width(), ContronRect.Height());
#ifdef SHAO_YANG    //20120809 yl add/������ʼ�뿪��λ��
    m_btContinue.MoveWindow(iMidLine-20-ContronRect.Width(), iBaseLine+150, ContronRect.Width(), ContronRect.Height());
#else
    m_btContinue.MoveWindow(iMidLine+20, iBaseLine+150, ContronRect.Width(), ContronRect.Height());
#endif

	m_btContinue.GetClientRect(&ContronRect);
	//m_btLeave.MoveWindow(iMidLine - ContronRect.Width() - 20 - 8,m_Bt2HeightSpace,ContronRect.Width(),ContronRect.Height());
	///m_btLeave.MoveWindow(iMidLine-20-ContronRect.Width(), iBaseLine+150, ContronRect.Width(), ContronRect.Height());
#ifdef SHAO_YANG    //20120809 yl add/������ʼ�뿪��λ��
    m_btLeave.MoveWindow(iMidLine+20, iBaseLine+150, ContronRect.Width(), ContronRect.Height());
#else
    m_btLeave.MoveWindow(iMidLine-20-ContronRect.Width(), iBaseLine+150, ContronRect.Width(), ContronRect.Height());
#endif

	/*m_PtResultTime.x = iMidLine-20-ContronRect.Width();
	m_PtResultTime.y = iBaseLine+150;*/
	//�й�
	m_btAuto.GetClientRect(&ContronRect);
	//m_btThing.MoveWindow(cx-ContronRect.Width() -m_Bt1WidthSpace,cy-(ContronRect.Height()+m_Bt1HeightSpace),ContronRect.Width(),ContronRect.Height());
	m_btThing.MoveWindow(cx-10-ContronRect.Width(), cy - (3+ContronRect.Height()), ContronRect.Width(),ContronRect.Height());
	//m_btSortCard.MoveWindow(cx-ContronRect.Width() -m_Bt1WidthSpace,cy-(ContronRect.Height()+m_Bt1HeightSpace)*2,ContronRect.Width(),ContronRect.Height());
	m_btSortCard.MoveWindow(cx-10-ContronRect.Width(), cy - (3+ContronRect.Height())*3, ContronRect.Width(), ContronRect.Height());
	//m_btAuto.MoveWindow(cx-ContronRect.Width() -m_Bt1WidthSpace,cy-(ContronRect.Height()+m_Bt1HeightSpace)*3,ContronRect.Width(),ContronRect.Height());
	m_btAuto.MoveWindow(cx-10-ContronRect.Width(), cy - (3+ContronRect.Height())*2, ContronRect.Width(), ContronRect.Height());
	//m_btLastTurn.MoveWindow(cx-ContronRect.Width() -m_Bt1WidthSpace,cy-(ContronRect.Height()+m_Bt1HeightSpace)*4,ContronRect.Width(),ContronRect.Height());
	m_btLastTurn.MoveWindow(cx-10-ContronRect.Width(), cy - (3+ContronRect.Height())*4, ContronRect.Width(), ContronRect.Height());
	///
	m_btCounter.GetClientRect(&ContronRect) ; 
	m_btCounter.MoveWindow(cx-10-ContronRect.Width(), cy - (3+ContronRect.Height())*5, ContronRect.Width(), ContronRect.Height());
	///
	m_btdialect.GetClientRect(&ContronRect) ; 
	m_btdialect.MoveWindow(cx-10-ContronRect.Width(), cy - (3+ContronRect.Height())*6, ContronRect.Width(), ContronRect.Height());

#ifdef POINT5_D
	m_btChangeShin.GetClientRect(&ContronRect);	//����
	m_btChangeShin.MoveWindow(cx-ContronRect.Width()-10, cy-(3+ContronRect.Height())*7, ContronRect.right,ContronRect.bottom); 
	
	m_btChuiYes.GetClientRect(&ContronRect); //��
	m_btChuiYes.MoveWindow(iMidLine-2*ContronRect.Width(),m_Bt2HeightSpace, ContronRect.right,ContronRect.bottom); 

	m_btChuiNo.GetClientRect(&ContronRect);	//����
	m_btChuiNo.MoveWindow(iMidLine+ContronRect.Width(),m_Bt2HeightSpace, ContronRect.right,ContronRect.bottom); 

#endif

	//�ı���ְ�,x����
	CRect rect;
	m_showcoredlg.GetClientRect(&rect);
	m_showcoredlg.SetWindowPos(NULL,cx-rect.Width(),-m_showcoredlg.m_showh,0,0,SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_NOSIZE);

	m_Result.CenterWindow();
	return ;
}

//������Ϸ��������
void CClientPlayView::LoadSkinIni(int cx,int cy)
{
	int	 m_heightVideo=0,m_widthVideo=0;
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	if((p->m_pGameInfo->dwRoomRule & GRR_VIDEO_ROOM)!=0)
	{
		//////////////////////////////////��Ƶ///////////////////////////
		///������ʱ���£��Ժ��Ƶ��ű��л��߶�̬������
		CString  strVideoCfg=CINIFile::GetAppPath()+TEXT("VideoCfg.ini");
		if(!CINIFile::IsFileExist(strVideoCfg))
		{

			AFCMessageBox("δ�ҵ���Ƶ�����ļ� ");
			return  ;
		}
		CINIFile iniFile(strVideoCfg);
		m_widthVideo		=iniFile.GetKeyVal(TEXT("VedioCfg"),TEXT("WidthVideo"), 0);
		m_heightVideo		=iniFile.GetKeyVal(TEXT("VedioCfg"),TEXT("HeightVideo"), 0);
		//////////////////////////////////////////////////////////////////
	}

	int iMidLine = cx/2;			//x�м���
	int iBaseLine = cy/2;		    //y�м���
	int x_Card = 73;				//�ƿ�
	int y_Card = 95;				//�Ƹ�

	CString s = CBcfFile::GetAppPath ();/////����·��


	//CBcfFile f( "GameOption.bcf");
	CBcfFile f(GET_CLIENT_BCF_NAME(szTempStr));

	CString key = TEXT("UISet");
	if((p->m_pGameInfo->dwRoomRule & GRR_VIDEO_ROOM)!=0)
		key="VideoUISet";

	//1��λ��
	//������
	ptHandCard[0].x = cx + f.GetKeyVal(key,TEXT("handcard0x"),-50);
	ptHandCard[0].y =iBaseLine + f.GetKeyVal(key,TEXT("handcard0y"), -30);
	//��������
	ptCardPoint[0].x =3*iMidLine/2+f.GetKeyVal(key,TEXT("deskcard0x"),-x_Card-10);
	ptCardPoint[0].y =iBaseLine + f.GetKeyVal(key,TEXT("deskcard0y"),-47-45);

	if((p->m_pGameInfo->dwRoomRule & GRR_VIDEO_ROOM)!=0)
	{
		////ͷ��
		m_PtLogo[0].x =cx-m_widthVideo + f.GetKeyVal(key,TEXT("logo0x"),-0);
		m_PtLogo[0].y =iBaseLine-m_heightVideo/2+ f.GetKeyVal(key,TEXT("logo0y"),-10);

		// �ǳ�
		m_PtName[0].x =cx + f.GetKeyVal(key,TEXT("name0x"),-55);
		m_PtName[0].y =cy / 2 + f.GetKeyVal(key,TEXT("name0y"),-80);
		//ͷ����ʾ��
		//	m_UserRect[0].SetRect(m_PtLogo[0].x,m_PtLogo[0].y,m_PtLogo[0].x+m_widthVideo,m_PtLogo[0].y+m_heightVideo);

		//��ʱ��
		m_PtTime[0].x =m_PtName[0].x+ f.GetKeyVal(key,TEXT("time0x"),-50);
		m_PtTime[0].y =m_PtName[0].y+ f.GetKeyVal(key,TEXT("time0y"),0);
		//ׯͼ��
		m_PtNt[0].x = m_PtName[0].x+f.GetKeyVal(key,TEXT("nt0x"),-190);
		m_PtNt[0].y = m_PtName[0].y+f.GetKeyVal(key,TEXT("nt0y"),-300);
		//�й�
		m_PtAuto[0].x =m_PtName[0].x+ f.GetKeyVal(key,TEXT("auto0x"),-50);
		m_PtAuto[0].y =m_PtName[0].y+ f.GetKeyVal(key,TEXT("auto0y"),300);

		ptHandCard[0].x = m_PtName[0].x +f.GetKeyVal(key,TEXT("handcard0x"),-50);
		ptHandCard[0].y = m_PtName[0].y + f.GetKeyVal(key,TEXT("handcard0y"), -y_Card/2);

		//�Ӱ�λ��
		m_PtPeopleMul[0].x = m_PtLogo[0].x+f.GetKeyVal(key,TEXT("peopleMul0x"),100);
		m_PtPeopleMul[0].y = m_PtLogo[0].y+f.GetKeyVal(key,TEXT("peopleMul0y"),-40);

		//����
		m_PtAwardPoint[0].x =m_PtLogo[0].x+ f.GetKeyVal(key,TEXT("awardpoint0x"),5);
		m_PtAwardPoint[0].y = m_PtLogo[0].y+m_heightVideo+f.GetKeyVal(key,TEXT("awardpoint0y"),0);

	}
	else
	{
		//ͷ��
		m_PtLogo[0].x =cx-LOGO_WIDTH + f.GetKeyVal(key,TEXT("logo0x"),-33);
		m_PtLogo[0].y =iBaseLine-LOGO_HEIGHT/2+ f.GetKeyVal(key,TEXT("logo0y"),-28);

		m_PtName[0].x =cx + f.GetKeyVal(key,TEXT("name0x"),-55);
		m_PtName[0].y =cy / 2 + f.GetKeyVal(key,TEXT("name0y"),-80);

		if (m_iUserFrame.GetWidth() > 0)
		{
			//m_PtLogo[0].x -= (m_iUserFrame.GetWidth() - LOGO_WIDTH)/2;
			//m_PtLogo[0].y -= (m_iUserFrame.GetHeight() - LOGO_HEIGHT)/2;
			m_PtLogo[0].x -= 10;
		}

		//ͷ����ʾ��
		//m_UserRect[0].SetRect(m_PtLogo[0].x,m_PtLogo[0].y,m_PtLogo[0].x+LOGO_WIDTH,m_PtLogo[0].y+LOGO_HEIGHT);

		//��ʱ��
		m_PtTime[0].x =m_PtName[0].x+ f.GetKeyVal(key,TEXT("time0x"),-70);
		m_PtTime[0].y =m_PtName[0].y+ f.GetKeyVal(key,TEXT("time0y"),0);
		//ׯͼ��
#ifdef SHAO_YANG
        m_PtNt[0].x = m_PtName[0].x +f.GetKeyVal(key,TEXT("nt0x"),8);//yl alter/5
        m_PtNt[0].y =m_PtName[0].y +f.GetKeyVal(key,TEXT("nt0y"),-90);//yl alter/-80
#else
        m_PtNt[0].x = m_PtName[0].x +f.GetKeyVal(key,TEXT("nt0x"),5);
        m_PtNt[0].y =m_PtName[0].y +f.GetKeyVal(key,TEXT("nt0y"),-80);
#endif
		//�й�
		m_PtAuto[0].x =m_PtName[0].x+ f.GetKeyVal(key,TEXT("auto0x"),-50);
		m_PtAuto[0].y =m_PtName[0].y+ f.GetKeyVal(key,TEXT("auto0y"),100);

		//�Ӱ�λ��
		m_PtPeopleMul[0].x = m_PtLogo[0].x+f.GetKeyVal(key,TEXT("peopleMul0x"),40);
		m_PtPeopleMul[0].y = m_PtLogo[0].y+LOGO_HEIGHT+f.GetKeyVal(key,TEXT("peopleMul0y"),35);

		//����
		m_PtAwardPoint[0].x =m_PtLogo[0].x+f.GetKeyVal(key,TEXT("awardpoint0x"),39);
		m_PtAwardPoint[0].y = m_PtLogo[0].y+f.GetKeyVal(key,TEXT("awardpoint0y"),-35);

	}

	m_PtGif[0].x = m_PtLogo[0].x+f.GetKeyVal(key,TEXT("flower0x"),-50);
	m_PtGif[0].y = m_PtLogo[0].y+f.GetKeyVal(key,TEXT("flower0y"),123);

	//��һ����־
	m_PtChui[0].x = m_PtLogo[0].x+f.GetKeyVal(key,TEXT("chui0x"),70);
	m_PtChui[0].y = m_PtLogo[0].y+f.GetKeyVal(key,TEXT("chui0y"),93);

	//�ǳ�
// 	m_PtName[0].x =m_PtLogo[0].x+f.GetKeyVal(key,TEXT("name0x"),0);
// 	m_PtName[0].y =m_PtLogo[0].y+LOGO_HEIGHT+f.GetKeyVal(key,TEXT("name0y"),15);

	m_PtName[0].x =cx + f.GetKeyVal(key,TEXT("name0x"),-80);
	m_PtName[0].y =cy / 2 + f.GetKeyVal(key,TEXT("name0y"),-80);

	//����
	m_PtAgree[0].x =m_PtName[0].x+ f.GetKeyVal(key,TEXT("Agree0x"),-30);
	m_PtAgree[0].y =m_PtName[0].y+ f.GetKeyVal(key,TEXT("Agree0y"),20);
	//��
	m_PtPass[0].x =ptCardPoint[0].x+ f.GetKeyVal(key,TEXT("pass0x"),-50);
	m_PtPass[0].y = ptCardPoint[0].y+f.GetKeyVal(key,TEXT("pass0y"),10);
	//�з�
	m_PtCallScore[0].x = ptCardPoint[0].x+f.GetKeyVal(key,TEXT("CallScore0x"),-50);
	m_PtCallScore[0].y = ptCardPoint[0].y+f.GetKeyVal(key,TEXT("CallScore0y"),0);
	//ָʾ������
	m_PtCurOrientation[0].x = iMidLine+f.GetKeyVal(key,TEXT("Orientation0x"),15);
	m_PtCurOrientation[0].y = iBaseLine+f.GetKeyVal(key,TEXT("Orientation0y"),-12);
	//2��λ��
	//������
	ptHandCard[1].x =iMidLine+f.GetKeyVal(key,TEXT("handcard1x"),12);
	ptHandCard[1].y =cy+f.GetKeyVal(key,TEXT("handcard1y"),-y_Card-y_Card + 30);
	//��������
	ptCardPoint[1].x =iMidLine+f.GetKeyVal(key,TEXT("deskcard1x"),0);
	ptCardPoint[1].y =cy+f.GetKeyVal(key,TEXT("deskcard1y"),-y_Card-y_Card-y_Card-y_Card/2+30);

	if((p->m_pGameInfo->dwRoomRule & GRR_VIDEO_ROOM)!=0)
	{
		//ͷ��
		m_PtLogo[1].x = cx  - f.GetKeyVal(key,TEXT("logo1x"),cx );
		m_PtLogo[1].y = cy-m_heightVideo+f.GetKeyVal(key,TEXT("logo1y"),-30);

		m_PtName[1].x =cx / 2 + f.GetKeyVal(key,TEXT("name0x"),-80);
		m_PtName[1].y =cy + f.GetKeyVal(key,TEXT("name0y"),-30);

		//ͷ����ʾ��
		//	m_UserRect[1].SetRect(m_PtLogo[1].x,m_PtLogo[1].y,m_PtLogo[1].x+m_widthVideo,m_PtLogo[1].y+m_heightVideo);
		//��ʱ��
		m_PtTime[1].x =m_PtName[1].x+m_widthVideo+ f.GetKeyVal(key,TEXT("time1x"),0);
		m_PtTime[1].y = m_PtName[1].y+f.GetKeyVal(key,TEXT("time1y"),-30);
		//ׯͼ��
		m_PtNt[1].x =m_PtName[1].x+ f.GetKeyVal(key,TEXT("nt1x"),-180);
		m_PtNt[1].y =m_PtName[1].y+ f.GetKeyVal(key,TEXT("nt1y"),-120);
		//�й�
		m_PtAuto[1].x = m_PtName[1].x + f.GetKeyVal(key,TEXT("auto1x"),-220);
		m_PtAuto[1].y = m_PtName[1].y + f.GetKeyVal(key,TEXT("auto1y"),-100);

		//�Ӱ�λ��
		m_PtPeopleMul[1].x = m_PtLogo[1].x+f.GetKeyVal(key,TEXT("peopleMul1x"),100);
		m_PtPeopleMul[1].y = m_PtLogo[1].y+f.GetKeyVal(key,TEXT("peopleMul1y"),-40);

		//����
		m_PtAwardPoint[1].x =m_PtLogo[1].x+ f.GetKeyVal(key,TEXT("awardpoint1x"),-50);
		m_PtAwardPoint[1].y = m_PtLogo[1].y+m_heightVideo+f.GetKeyVal(key,TEXT("awardpoint1y"),0);

	}
	else
	{
		//ͷ��
		m_PtLogo[1].x = iMidLine  - f.GetKeyVal(key,TEXT("logo1x"),320);
		m_PtLogo[1].y = cy-LOGO_HEIGHT+f.GetKeyVal(key,TEXT("logo1y"),-41);

		m_PtName[1].x =cx / 2 + f.GetKeyVal(key,TEXT("name0x"),-85);
		m_PtName[1].y =cy + f.GetKeyVal(key,TEXT("name0y"),-30);

		/*if (m_iUserFrame.GetWidth() > 0)
		{
		m_PtLogo[1].x -= (m_iUserFrame.GetWidth() - LOGO_WIDTH)/2;
		m_PtLogo[1].y -= (m_iUserFrame.GetHeight() - LOGO_HEIGHT)/2;
		}*/
		//ͷ����ʾ��
		//m_UserRect[1].SetRect(m_PtLogo[1].x,m_PtLogo[1].y,m_PtLogo[1].x+LOGO_WIDTH,m_PtLogo[1].y+LOGO_HEIGHT);
		//��ʱ��
		m_PtTime[1].x =m_PtName[1].x + f.GetKeyVal(key,TEXT("time1x"),0);
		m_PtTime[1].y = m_PtName[1].y+f.GetKeyVal(key,TEXT("time1y"),-30);
		//ׯͼ��
#ifdef SHAO_YANG
        m_PtNt[1].x =m_PtName[1].x+ f.GetKeyVal(key,TEXT("nt1x"),-210);//yl alter/-180
        m_PtNt[1].y =m_PtName[1].y+ f.GetKeyVal(key,TEXT("nt1y"),-90);//yl alter/-120
#else
        m_PtNt[1].x =m_PtName[1].x+ f.GetKeyVal(key,TEXT("nt1x"),-180);
        m_PtNt[1].y =m_PtName[1].y+ f.GetKeyVal(key,TEXT("nt1y"),-120);
#endif

		//�й�
		m_PtAuto[1].x = m_PtName[1].x + f.GetKeyVal(key,TEXT("auto1x"),-220);
		m_PtAuto[1].y = m_PtName[1].y + f.GetKeyVal(key,TEXT("auto1y"),-100);

		//�Ӱ�λ��
		m_PtPeopleMul[1].x = m_PtLogo[1].x+f.GetKeyVal(key,TEXT("peopleMul1x"),-60);;
		m_PtPeopleMul[1].y = m_PtLogo[1].y+f.GetKeyVal(key,TEXT("peopleMul1y"),30);;

		//����
		m_PtAwardPoint[1].x =m_PtLogo[1].x+ f.GetKeyVal(key,TEXT("awardpoint1x"),39);
		m_PtAwardPoint[1].y = m_PtLogo[1].y+f.GetKeyVal(key,TEXT("awardpoint1y"),-35);

	}

	m_PtGif[1].x = m_PtLogo[1].x+f.GetKeyVal(key,TEXT("flower1x"),-121);
	m_PtGif[1].y = m_PtLogo[1].y+f.GetKeyVal(key,TEXT("flower1y"),0);

	//��һ����־
#ifdef SHAO_YANG
    m_PtChui[1].x = m_PtLogo[1].x+f.GetKeyVal(key,TEXT("chui1x"),-110);//yl alter/-90
    m_PtChui[1].y = m_PtLogo[1].y+f.GetKeyVal(key,TEXT("chui1y"),10);
#else
    m_PtChui[1].x = m_PtLogo[1].x+f.GetKeyVal(key,TEXT("chui1x"),-90);
    m_PtChui[1].y = m_PtLogo[1].y+f.GetKeyVal(key,TEXT("chui1y"),10);

#endif

	//�ǳ�
	/*m_PtName[1].x = m_PtLogo[1].x+f.GetKeyVal(key,TEXT("name1x"),0);
	m_PtName[1].y = m_PtLogo[1].y+LOGO_HEIGHT+f.GetKeyVal(key,TEXT("name1y"),15);*/
	//����
	m_PtAgree[1].x = m_PtName[1].x+ f.GetKeyVal(key,TEXT("Agree1x"),0);
	m_PtAgree[1].y = m_PtName[1].y+ f.GetKeyVal(key,TEXT("Agree1y"),-16);
	//��
	m_PtPass[1].x = ptCardPoint[1].x+ f.GetKeyVal(key,TEXT("pass1x"),-30);
	m_PtPass[1].y = ptCardPoint[1].y+ f.GetKeyVal(key,TEXT("pass1y"),y_Card/2+50);
	//�з�
	m_PtCallScore[1].x = ptCardPoint[1].x+f.GetKeyVal(key,TEXT("CallScore1x"),-30);
	m_PtCallScore[1].y = ptCardPoint[1].y+ f.GetKeyVal(key,TEXT("CallScore1y"),y_Card/2+50);
	//ָʾ������
	m_PtCurOrientation[1].x =iMidLine+ f.GetKeyVal(key,TEXT("Orientation1x"),-15);
	m_PtCurOrientation[1].y =iBaseLine + f.GetKeyVal(key,TEXT("Orientation1y"),10);


	//3��λ��
	//������
	ptHandCard[2].x =LOGO_WIDTH+f.GetKeyVal(key,TEXT("handcard2x"),0);
	ptHandCard[2].y =iBaseLine+f.GetKeyVal(key,TEXT("handcard2y"),-30);
	//��������
	ptCardPoint[2].x =iMidLine/2+f.GetKeyVal(key,TEXT("deskcard2x"),x_Card+10);
	ptCardPoint[2].y =iBaseLine+f.GetKeyVal(key,TEXT("deskcard2y"),-47-45);

	if((p->m_pGameInfo->dwRoomRule & GRR_VIDEO_ROOM)!=0)
	{
		////ͷ��
		m_PtLogo[2].x = f.GetKeyVal(key,TEXT("logo2x"),0);
		m_PtLogo[2].y =iBaseLine-m_heightVideo/2+ f.GetKeyVal(key,TEXT("logo2y"),-10);

		m_PtName[2].x =f.GetKeyVal(key,TEXT("name0x"),10);
		m_PtName[2].y =cy / 2 + f.GetKeyVal(key,TEXT("name0y"),-160);
		//ͷ����ʾ��
		//	m_UserRect[2].SetRect(m_PtLogo[2].x,m_PtLogo[2].y,m_PtLogo[2].x+m_widthVideo,m_PtLogo[2].y+m_heightVideo);
		//��ʱ��
		m_PtTime[2].x =m_PtName[2].x+m_widthVideo+ f.GetKeyVal(key,TEXT("time2x"),-680);
		m_PtTime[2].y =m_PtName[2].y+ f.GetKeyVal(key,TEXT("time2y"),0);
		//ׯͼ��
		m_PtNt[2].x = m_PtName[2].x+f.GetKeyVal(key,TEXT("nt2x"),-400);
		m_PtNt[2].y = m_PtName[2].y+f.GetKeyVal(key,TEXT("nt2y"),-20);
		//�й�
		m_PtAuto[2].x = m_PtName[2].x+f.GetKeyVal(key,TEXT("auto2x"),-600);
		m_PtAuto[2].y = m_PtName[2].y+f.GetKeyVal(key,TEXT("auto2y"),100);

		//������
		ptHandCard[2].x =m_widthVideo+f.GetKeyVal(key,TEXT("handcard2x"),5);
		ptHandCard[2].y =iBaseLine+f.GetKeyVal(key,TEXT("handcard2y"),-y_Card/2);
		//��������
		ptCardPoint[2].x =iMidLine/2+f.GetKeyVal(key,TEXT("deskcard2x"),x_Card+10);
		ptCardPoint[2].y =iBaseLine+f.GetKeyVal(key,TEXT("deskcard2y"),-47-45);

		//�Ӱ�λ��
		m_PtPeopleMul[2].x = m_PtLogo[2].x+f.GetKeyVal(key,TEXT("peopleMul2x"),100);
		m_PtPeopleMul[2].y = m_PtLogo[2].y+f.GetKeyVal(key,TEXT("peopleMul2y"),-40);

		//����
		m_PtAwardPoint[2].x = m_PtLogo[2].x+f.GetKeyVal(key,TEXT("awardpoint2x"),2);
		m_PtAwardPoint[2].y = m_PtLogo[2].y+m_heightVideo-f.GetKeyVal(key,TEXT("awardpoint2y"),0);

	}
	else
	{
		////ͷ��
		m_PtLogo[2].x = f.GetKeyVal(key,TEXT("logo2x"),28);
		m_PtLogo[2].y =iBaseLine-LOGO_WIDTH/2+ f.GetKeyVal(key,TEXT("logo2y"),-48);

		m_PtName[2].x = f.GetKeyVal(key,TEXT("name0x"),10);
		m_PtName[2].y =cy / 2 + f.GetKeyVal(key,TEXT("name0y"),-160);

		if (m_iUserFrame.GetWidth() > 0)
		{
			//m_PtLogo[2].x -= (m_iUserFrame.GetWidth() - LOGO_WIDTH)/2;
			//m_PtLogo[2].y -= (m_iUserFrame.GetHeight() - LOGO_HEIGHT)/2;
			m_PtLogo[2].x -= 17;
		}
		//ͷ����ʾ��
		//m_UserRect[2].SetRect(m_PtLogo[2].x,m_PtLogo[2].y,m_PtLogo[2].x+LOGO_WIDTH,m_PtLogo[2].y+LOGO_HEIGHT);
		//��ʱ��
		m_PtTime[2].x =m_PtName[2].x + f.GetKeyVal(key,TEXT("time2x"),-680);
		m_PtTime[2].y =m_PtName[2].y + f.GetKeyVal(key,TEXT("time2y"),0);
		//ׯͼ��
		m_PtNt[2].x = m_PtName[2].x+f.GetKeyVal(key,TEXT("nt2x"),-620);
		m_PtNt[2].y = m_PtName[2].y +f.GetKeyVal(key,TEXT("nt2y"),0);
		//�й�
		m_PtAuto[2].x = m_PtName[2].x+f.GetKeyVal(key,TEXT("auto2x"),-650);
		m_PtAuto[2].y = m_PtName[2].y+f.GetKeyVal(key,TEXT("auto2y"),100);

		//�Ӱ�λ��
		m_PtPeopleMul[2].x = m_PtLogo[2].x+f.GetKeyVal(key,TEXT("peopleMul2x"),40);
		m_PtPeopleMul[2].y = m_PtLogo[2].y+LOGO_HEIGHT+f.GetKeyVal(key,TEXT("peopleMul2y"),35);

		//����
		m_PtAwardPoint[2].x = m_PtLogo[2].x+f.GetKeyVal(key,TEXT("awardpoint2x"),39);
		m_PtAwardPoint[2].y = m_PtLogo[2].y+f.GetKeyVal(key,TEXT("awardpoint2y"),-35);
	}

	m_PtGif[2].x = m_PtLogo[2].x+f.GetKeyVal(key,TEXT("flower2x"),0);
	m_PtGif[2].y = m_PtLogo[2].y+f.GetKeyVal(key,TEXT("flower2y"),123);

	//��һ����־
	m_PtChui[2].x = m_PtLogo[2].x+f.GetKeyVal(key,TEXT("chui2x"),22);
	m_PtChui[2].y = m_PtLogo[2].y+f.GetKeyVal(key,TEXT("chui2y"),93);

	////�ǳ�
	m_PtName[2].x =f.GetKeyVal(key,TEXT("name2x"),0);
	m_PtName[2].y =cy / 2+f.GetKeyVal(key,TEXT("name2y"),-180);
	//����
	m_PtAgree[2].x =m_PtName[2].x + f.GetKeyVal(key,TEXT("Agree2x"),10);
	m_PtAgree[2].y = m_PtName[2].y+f.GetKeyVal(key,TEXT("Agree2y"),-85);
	//��
	m_PtPass[2].x = ptCardPoint[2].x+f.GetKeyVal(key,TEXT("pass2x"),-20);
	m_PtPass[2].y =ptCardPoint[2].y+ f.GetKeyVal(key,TEXT("pass2y"),0);
	//�з�
	m_PtCallScore[2].x =  ptCardPoint[2].x+f.GetKeyVal(key,TEXT("CallScore2x"),-20);
	m_PtCallScore[2].y =  ptCardPoint[2].y+f.GetKeyVal(key,TEXT("CallScore2y"),0);
	//ָʾ������
	m_PtCurOrientation[2].x =iMidLine+ f.GetKeyVal(key,TEXT("Orientation2x"),-45);
	m_PtCurOrientation[2].y =iBaseLine+ f.GetKeyVal(key,TEXT("Orientation2y"),-12);

	//����
	ptBackCard.x=iMidLine+ f.GetKeyVal(key,TEXT("backcardx"),0);
	ptBackCard.y=f.GetKeyVal(key,TEXT("backcardy"),30);
	//�׷���ʾ
	m_PtBottomCue.x = iMidLine+ f.GetKeyVal(key,TEXT("bottonx"),0);
	m_PtBottomCue.y = cy+f.GetKeyVal(key,TEXT("bottony"),-10);
	//�ұ߰�ť��
	m_Bt1WidthSpace=f.GetKeyVal(key,TEXT("WidthSpace1"),10);
	m_Bt1HeightSpace=f.GetKeyVal(key,TEXT("HeightSpace1"),10);
	//�зְ�ť��
	m_Bt2WidthSpace=f.GetKeyVal(key,TEXT("WidthSpace2"),10);
	m_Bt2HeightSpace=iBaseLine+f.GetKeyVal(key,TEXT("HeightSpace2"),3*y_Card/2-20+30);
	//
	m_PtShowMsg.x = f.GetKeyVal(key,TEXT("showmsgx"),26);
	m_PtShowMsg.y = f.GetKeyVal(key,TEXT("showmsgy"),30);


	if((p->m_pGameInfo->dwRoomRule & GRR_VIDEO_ROOM)!=0)
	{
		//��Ƶ�������
		m_rectVideoFrame[2].top=m_PtLogo[2].y;// cy-m_heightVideo;
		m_rectVideoFrame[2].left=m_PtLogo[2].x;//10;
		m_rectVideoFrame[2].bottom=m_rectVideoFrame[2].top+m_heightVideo;
		m_rectVideoFrame[2].right=m_rectVideoFrame[2].left+m_widthVideo;
		m_rectVideoFrame[3].top=m_PtLogo[3].y;//249;
		m_rectVideoFrame[3].left=m_PtLogo[3].x;//10;
		m_rectVideoFrame[3].bottom=m_rectVideoFrame[3].top+m_heightVideo;
		m_rectVideoFrame[3].right=m_rectVideoFrame[3].left+m_widthVideo;

		m_rectVideoFrame[0].top=m_PtLogo[0].y;//0;
		m_rectVideoFrame[0].left=m_PtLogo[0].x;//220;
		m_rectVideoFrame[0].bottom=m_rectVideoFrame[0].top+m_heightVideo;
		m_rectVideoFrame[0].right=m_rectVideoFrame[0].left+m_widthVideo;
		m_rectVideoFrame[1].top=m_PtLogo[1].y;//242;
		m_rectVideoFrame[1].left=m_PtLogo[1].x;//cx-m_widthVideo+5;
		m_rectVideoFrame[1].bottom=m_rectVideoFrame[1].top+m_heightVideo;
		m_rectVideoFrame[1].right=m_rectVideoFrame[1].left+m_widthVideo; 

		//�˴��������λ�� �й�λ�� 
		m_PtAgree[0].x = m_rectVideoFrame[0].left  + f.GetKeyVal(key,_T("agree0x"),35);
		m_PtAgree[0].y = m_rectVideoFrame[0].top + f.GetKeyVal(key,_T("agree0y"),-13);
		m_PtAgree[1].x = m_rectVideoFrame[1].left  + f.GetKeyVal(key,_T("agree1x"),0);
		m_PtAgree[1].y = m_rectVideoFrame[1].top + f.GetKeyVal(key,_T("agree1y"),-13);
		m_PtAgree[2].x = m_rectVideoFrame[2].left  + f.GetKeyVal(key,_T("agree2x"),10);
		m_PtAgree[2].y = m_rectVideoFrame[2].top + f.GetKeyVal(key,_T("agree2y"),-13);

		/*for (int i = 0; i < PLAY_COUNT; i++)
		{
		CString strkey = _T("");
		strkey.Format(_T("auto%dx"), i);
		m_PtAuto[i].x = m_PtAgree[i].x + f.GetKeyVal(key,strkey,0);
		strkey.Format(_T("auto%dy"), i);

		m_PtAuto[i].y = m_PtAgree[i].y + f.GetKeyVal(key,strkey,-22);
		if (i == 2)
		{
		m_PtAuto[i].x -= 32;
		}
		}*/

		m_PtGif[0].x = m_PtLogo[0].x+f.GetKeyVal(key,TEXT("flower0x"),-163);
		m_PtGif[0].y = m_PtLogo[0].y+f.GetKeyVal(key,TEXT("flower0y"),-50);
		m_PtGif[1].x = m_PtLogo[1].x+f.GetKeyVal(key,TEXT("flower1x"),160);
		m_PtGif[1].y = m_PtLogo[1].y+f.GetKeyVal(key,TEXT("flower1y"),30);
		m_PtGif[2].x = m_PtLogo[2].x+f.GetKeyVal(key,TEXT("flower2x"),223);
		m_PtGif[2].y = m_PtLogo[2].y+f.GetKeyVal(key,TEXT("flower2y"),-50);

		///������Ƶ����λ��

		//if(NULL!=CPlayVideo::CreatePlayVideo())
		//{
		//	CPlayVideo::CreatePlayVideo()->SetUserPos();  
		//}
	}
	else
	{
		//20�ѿͻ�����
#ifdef TWENTY
		for (int i=0; i<PLAY_COUNT; i++)
		{
			m_PtLogo[i].y -= 20;
		}
		//�ǳ�
	m_PtName[0].y = m_PtLogo[0].y+LOGO_HEIGHT+f.GetKeyVal(key,TEXT("name0y"),15);
	m_PtName[1].y = m_PtLogo[1].y+LOGO_HEIGHT+f.GetKeyVal(key,TEXT("name1y"),15);
	m_PtName[2].y = m_PtLogo[2].y+LOGO_HEIGHT+f.GetKeyVal(key,TEXT("name2y"),15);
#endif
        #ifdef SHAO_YANG
        m_PtAgree[0].x = m_PtName[0].x + f.GetKeyVal(key,_T("agree0x"),5) - 70;///�Ҽ�
        m_PtAgree[0].y = m_PtName[0].y + f.GetKeyVal(key,_T("agree0y"),-65) + 110;
        m_PtAgree[1].x = m_PtName[1].x + f.GetKeyVal(key,_T("agree1x"),40) + 0;///����
        m_PtAgree[1].y = m_PtName[1].y + f.GetKeyVal(key,_T("agree1y"),-18) + 0;
        m_PtAgree[2].x = m_PtName[2].x + f.GetKeyVal(key,_T("agree2x"),-20) + 105;///���
        m_PtAgree[2].y = m_PtName[2].y + f.GetKeyVal(key,_T("agree2y"),20) + 120;
        #else
        m_PtAgree[0].x = m_PtName[0].x + f.GetKeyVal(key,_T("agree0x"),5);
		m_PtAgree[0].y = m_PtName[0].y + f.GetKeyVal(key,_T("agree0y"),-65);
		m_PtAgree[1].x = m_PtName[1].x + f.GetKeyVal(key,_T("agree1x"),40);
		m_PtAgree[1].y = m_PtName[1].y + f.GetKeyVal(key,_T("agree1y"),-18);
		m_PtAgree[2].x = m_PtName[2].x + f.GetKeyVal(key,_T("agree2x"),-20);
		m_PtAgree[2].y = m_PtName[2].y + f.GetKeyVal(key,_T("agree2y"),20);
        #endif

		if (!m_bShowLogo)
		{
			m_PtAgree[0].x += f.GetKeyVal(key,_T("agreeoffset0x"),35);
			m_PtAgree[0].y += f.GetKeyVal(key,_T("agreeoffset0y"),0)-18;
			m_PtAgree[1].x += f.GetKeyVal(key,_T("agreeoffset1x"),0);
			m_PtAgree[1].y += f.GetKeyVal(key,_T("agreeoffset1y"),0)-18;
			m_PtAgree[2].x += f.GetKeyVal(key,_T("agreeoffset2x"),50);
			m_PtAgree[2].y += f.GetKeyVal(key,_T("agreeoffset2y"),0);
		}

		///< �й�����
		m_PtAuto[0].x = m_PtName[0].x + f.GetKeyVal(key,_T("auto0x"),-50);
		m_PtAuto[0].y = m_PtName[0].y + f.GetKeyVal(key,_T("auto0y"),0);
		m_PtAuto[1].x = m_PtName[1].x  + f.GetKeyVal(key,_T("auto1x"),-180);
		m_PtAuto[1].y = m_PtName[1].y  + f.GetKeyVal(key,_T("auto1y"),-80);
		if (m_iUserFrame.GetWidth() > 0)
			m_PtAuto[2].x = m_PtName[2].x+ f.GetKeyVal(key,_T("auto2x"),m_iUserFrame.GetWidth());
		else
			m_PtAuto[2].x = m_PtName[2].x  + f.GetKeyVal(key,_T("auto2x"),LOGO_WIDTH);

		m_PtAuto[2].y = m_PtName[2].y  + f.GetKeyVal(key,_T("auto2y"),0);

		///< ׯ�Һ�����λ��
	/*	m_PtNt[0].x = m_PtName[0].x  + f.GetKeyVal(key,_T("nt0x"),1);
		m_PtNt[0].y = m_PtName[0].y  + f.GetKeyVal(key,_T("nt0y"),-43);
		m_PtNt[1].x = m_PtName[1].x  + f.GetKeyVal(key,_T("nt1x"),1);
		m_PtNt[1].y = m_PtName[1].y  + f.GetKeyVal(key,_T("nt1y"),-43);*/
		if (m_iUserFrame.GetWidth() > 0)
			m_PtNt[2].x = m_PtName[2].x  + f.GetKeyVal(key,_T("nt2x"),/*m_iUserFrame.GetWidth()*/10);
		else
			m_PtNt[2].x = m_PtName[2].x  + f.GetKeyVal(key,_T("nt2x"),/*LOGO_WIDTH*/10);

#ifdef SHAO_YANG
        m_PtNt[2].y = m_PtName[2].y  + f.GetKeyVal(key,_T("nt2y"),-10) - 20;///yl alter
#else
        m_PtNt[2].y = m_PtName[2].y  + f.GetKeyVal(key,_T("nt2y"),-10);
#endif

		///< �������
		m_PtAwardPoint[0].x = m_PtName[0].x  + f.GetKeyVal(key,_T("awardpoint0x"),41);
		m_PtAwardPoint[0].y = m_PtName[0].y  + f.GetKeyVal(key,_T("awardpoint0y"),-30);
		m_PtAwardPoint[1].x = m_PtName[1].x  + f.GetKeyVal(key,_T("awardpoint1x"),-160);
		m_PtAwardPoint[1].y = m_PtName[1].y  + f.GetKeyVal(key,_T("awardpoint1y"),-30);
		m_PtAwardPoint[2].x = m_PtName[2].x  + f.GetKeyVal(key,_T("awardpoint2x"),20);
		m_PtAwardPoint[2].y = m_PtName[2].y  + f.GetKeyVal(key,_T("awardpoint2y"),40);

		///< ��ҼӰ���ʾλ��
		m_PtPeopleMul[0].x = m_PtName[0].x + f.GetKeyVal(key,_T("mul0x"),40);
		m_PtPeopleMul[0].y = m_PtName[0].y + f.GetKeyVal(key,_T("mul0y"),LOGO_HEIGHT+20);
		m_PtPeopleMul[1].x = m_PtName[1].x + f.GetKeyVal(key,_T("mul1x"),-130);
		m_PtPeopleMul[1].y = m_PtName[1].y + f.GetKeyVal(key,_T("mul1y"),-40);
		m_PtPeopleMul[2].x = m_PtName[2].x + f.GetKeyVal(key,_T("mul2x"),5);
		m_PtPeopleMul[2].y = m_PtName[2].y + f.GetKeyVal(key,_T("mul2y"),230);

		///< ����
		m_PtPass[0].x = ptCardPoint[0].x + f.GetKeyVal(key,_T("pass0x"),-50);
		m_PtPass[0].y = ptCardPoint[0].y + f.GetKeyVal(key,_T("pass0y"),50);
		m_PtPass[1].x = ptCardPoint[1].x + f.GetKeyVal(key,_T("pass1x"),-30);
		m_PtPass[1].y = ptCardPoint[1].y + f.GetKeyVal(key,_T("pass1y"),30);
		m_PtPass[2].x = ptCardPoint[2].x + f.GetKeyVal(key,_T("pass2x"),-20);
		m_PtPass[2].y = ptCardPoint[2].y + f.GetKeyVal(key,_T("pass2y"),50);
		
		m_PtGif[0].x = m_PtName[0].x+f.GetKeyVal(key,TEXT("flower0x"),-15);
		m_PtGif[0].y = m_PtName[0].y+f.GetKeyVal(key,TEXT("flower0y"),70);
		m_PtGif[1].x = m_PtName[1].x+f.GetKeyVal(key,TEXT("flower1x"),35);
		m_PtGif[1].y = m_PtName[1].y+f.GetKeyVal(key,TEXT("flower1y"),40);
		m_PtGif[2].x = m_PtName[2].x+f.GetKeyVal(key,TEXT("flower2x"),0);
		m_PtGif[2].y = m_PtName[2].y+f.GetKeyVal(key,TEXT("flower2y"),70);
	}
}


//��ʼ��ť
void CClientPlayView::OnHitBegin()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_BEGIN,0,0);

    // PengJiLin, 2011-4-18, ����ʱ���������ͽ��
    m_GetMoneyImageView.SetShow(FALSE);

	return;
}
//�뿪��ť
void CClientPlayView::OnHitLeave()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);

    // PengJiLin, 2011-4-18, ����ʱ���������ͽ�� ѯ��
    CString strInfo = "";
    p->GetSendMoneyFormatInfo(strInfo);
    strInfo += "��ȷ��Ҫ�뿪��?";

    if(p->m_SendMoneyData.dwMoneyOnTimes > 0 || p->m_SendMoneyData.dwMoneyOnCounts > 0)
    {
        if(IDOK != AFCMessageBox(strInfo, "��ʾ", AFC_YESNO))
        {
            return;
        }
    }
	
	p->SendMessage(WM_CLOSE,0,0);
	return;
}
//���ư�ť
void CClientPlayView::OnHitOutCard()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_OUT_CARD,1,1);
	return;
}

//������ť
void CClientPlayView::OnHitContinue()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_BEGIN,0,0);

    // PengJiLin, 2011-4-18, ����ʱ���������ͽ��
    m_GetMoneyImageView.SetShow(FALSE);

	return;
}


//����Ҫ��
void CClientPlayView::OnHitThing()
{
	// ����Լ��˳���ť�Ѿ�������������ٵ��
	if (!m_bCanLeave)
	{
		m_btThing.EnableWindow(FALSE);
		return ;
	}

	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_HAVE_THING,0,0);
	return;
}



//���ý��溯��
void CClientPlayView::ResetGameView()
{
	CUsingSkinClass::ResetGameView();

	return;
}

//�滭 NT ��־
void CClientPlayView::DrawNt(CDC * pDC, int x, int y)
{
	CGameImageHelper	nt(&m_NT);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	dc.SelectObject(nt);
	::TransparentBlt(pDC->GetSafeHdc(),x,y,nt.GetWidth(),nt.GetHeight(),dc.GetSafeHdc(),0,0,nt.GetWidth(),nt.GetHeight(),dc.GetPixel(0,0));
	return;
}


//����ׯ�ҵ�λ��
void CClientPlayView::SetNTStation(int iViewStation)
{
	m_iNtStation = iViewStation;
	UpdateViewFace(NULL);
	return;
}



//���ñ���ɫ
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

/*******************************************************************************************************/

void CClientPlayView::DrawFen(CDC * pDC)
{
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	TCHAR path[MAX_PATH];
	for(int i = 0 ; i < PLAY_COUNT; i ++)
	{
		if(m_iCallScore[i]==-1||m_iCallScore[i]==255 || m_iCallScore[i] == 3)
			continue;
		else
		{
			switch(m_iCallScore[i])
			{
			case 0:
				wsprintf(path,TEXT(".\\image\\jiaofen2_pass.bmp"));
				break;
			case 1:
				wsprintf(path,TEXT(".\\image\\jiaofen1.bmp"));
				break;
			case 2:
				wsprintf(path,TEXT(".\\image\\jiaofen2.bmp"));
				break;
			default:
				break;
			}
		}
		m_CallScore.SetLoadInfo(path,false);						//��Ϸlogo��־
		CGameImageHelper	help(&m_CallScore);
		dc.SelectObject(help);
		/*	if (m_iCallScore[i]!=0)
		::TransparentBlt(pDC->GetSafeHdc(),m_PtCallScore[i].x,m_PtCallScore[i].y,help.GetWidth()/4,help.GetHeight(),dc.GetSafeHdc(),0,0,m_CallScore.GetWidth()/4,m_CallScore.GetHeight(),dc.GetPixel(0,0));
		else*/
		::TransparentBlt(pDC->GetSafeHdc(),m_PtCallScore[i].x,m_PtCallScore[i].y,help.GetWidth(),help.GetHeight(),dc.GetSafeHdc(),0,0,m_CallScore.GetWidth(),m_CallScore.GetHeight(),dc.GetPixel(0,0));
	}
	return ;
}

void CClientPlayView::OnCue(void)		//��ʾ
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_CUE,0,0);
	return;
}

void CClientPlayView::OnHitAuto(void)		//�й�
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_AUTOCARD,0,0);
	return;
}
//���ø��ҽз�
void CClientPlayView::SetCallScoreValue(BYTE bViewStation, int iCallScore)
{
	m_iCallScore[bViewStation] = iCallScore;

	UpdateViewFace(NULL);
}

//�O�ýзֽY��
void CClientPlayView::SetCallScoreResult(int iCardCount)	//�зֽ��
{
	m_iCallScoreResult=iCardCount;

	UpdateViewFace(NULL);
}

//����
void CClientPlayView::OnHitPass()		//200
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_PASS,1,1);
	return;
}
///������
void CClientPlayView::OnHitCounter()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_COUNTER,1,1);
	return ; 
}
///�������
void CClientPlayView::OnHitDialect()
{
	if(m_DialectDlg.IsWindowVisible())
	{
		m_DialectDlg.ShowWindow(SW_HIDE) ;
	}
	else
	{
		m_DialectDlg.CenterWindow(this) ;
		m_DialectDlg.ShowWindow(SW_SHOW) ;
	}
	
	return ; 
}
void CClientPlayView::OnHitSort()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_SORT,1,1);
	return;
}

void CClientPlayView::OnHitCallScore(UINT uID)		
{
	int iTmpScore=0;
	for(int i = 0 ; i < PLAY_COUNT; i ++)
	{
		if(m_iCallScore[i]==-1||m_iCallScore[i]==255)
			continue;

		if (iTmpScore<m_iCallScore[i])
			iTmpScore = m_iCallScore[i];
	}

	if (uID-IDC_CALL_SCORE_PASS<=iTmpScore && uID!=IDC_CALL_SCORE_PASS)
	{
		return;
	}

	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_CALL_SCORE,uID - IDC_CALL_SCORE_PASS,uID -IDC_CALL_SCORE_PASS);
	return;
}

//���������Ӱ�
void CClientPlayView::OnHitRobNT()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_ROB_NT,1,1);

	return;
}

//���������Ӱ�
void CClientPlayView::OnHitRobNT2()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_ROB_NT,0,0);
	return;
}

//���������Ӱ�
void CClientPlayView::OnHitAddDouble()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_ADD_DOUBLE,1,1);
	return;
}
//���������Ӱ�
void CClientPlayView::OnHitAddDouble2()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_ADD_DOUBLE,0,0);
	return;
}

//���������Ӱ�
void CClientPlayView::OnHitShowCard()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_SHOW_CARD,1,1);
	return;
}

//���������Ӱ�
void CClientPlayView::OnHitShowCard2()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_SHOW_CARD,0,0);
	return;
}

//��һ��
void CClientPlayView::OnHitLastTurn()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_LOOK_LAST,0,0);
	return;
}

//����
void CClientPlayView::OnChangeShin()
{
	m_iCurrentBG++;
	if(m_iCurrentBG >= 4)
	{
		m_iCurrentBG = 0;
	}

	//���ͼƬ�ļ��Ƿ����
	if(m_ImgBackground[m_iCurrentBG]->GetWidth() > 0)
	{
		UpdateViewFace(NULL);
	}
	else if(m_iCurrentBG != 0)	//ͼƬ��������Ĭ��ͼƬ
	{
		m_iCurrentBG = 0;
	}
}

/// ��
void CClientPlayView::OnHitChuiYes()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_CHUIYES,0,0);
}

/// ����
void CClientPlayView::OnHitChuiNo()
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_CHUINO,0,0);
}

//�O���й�
void CClientPlayView::SetAuto(int bDeskStation,bool Trusteeship)
{
	m_bAuto[bDeskStation]=Trusteeship;
	CRect rect;
	rect.SetRect(m_PtAuto[bDeskStation].x,m_PtAuto[bDeskStation].y,m_PtAuto[bDeskStation].x+33,m_PtAuto[bDeskStation].y+33);
	UpdateViewFace(NULL);	

	TCHAR szName[200];
	if(1 == bDeskStation)
	{
		if(m_bAuto[bDeskStation] )
			wsprintf(szName,TEXT(".\\image\\auto_less.bmp"));
		else
			wsprintf(szName,TEXT(".\\image\\auto_bt.bmp"));			
		m_btAuto.LoadButtonBitmap(szName,false);	
	}
}

//�����й�
BOOL CClientPlayView::DrawAuto(CDC * pDC)
{
	CGameImageHelper	AutoLogo(&m_AutoLogo);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	dc.SelectObject(AutoLogo);
	for(int i = 0;i < PLAY_COUNT;i++)
	{
		if(m_bAuto[i] /*&& i == 1*/)
		{
			int iMyNameMsgPos=0;
			CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
			if (p != NULL && p->IsQueueGameRoom())
				iMyNameMsgPos = -10;

			::TransparentBlt(pDC->GetSafeHdc(),m_PtAuto[i].x,m_PtAuto[i].y+iMyNameMsgPos,AutoLogo.GetWidth(),AutoLogo.GetHeight(),dc.GetSafeHdc(),0,0,AutoLogo.GetWidth(),AutoLogo.GetHeight(),dc.GetPixel(0,0));
		}
		/*else if (m_bAuto[i])
		{
			int iMyNameMsgPos = 0;
			CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
			if (p != NULL && p->IsQueueGameRoom())
				iMyNameMsgPos = -10;
			::TransparentBlt(pDC->GetSafeHdc(),m_PtAuto[i].x,m_PtAuto[i].y+iMyNameMsgPos,AutoLogo.GetWidth(),AutoLogo.GetHeight(),dc.GetSafeHdc(),0,0,AutoLogo.GetWidth(),AutoLogo.GetHeight(),dc.GetPixel(0,0));
		}*/
	}
	return true;
}

//�����Ϣ
void CClientPlayView::OnLButtonUp(UINT nFlags, CPoint Point)
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	//	p->SendMessage(IDM_OUT_CARD,0,0);
}

//�����Ϣ
void CClientPlayView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
	p->SendMessage(IDM_OUT_CARD,1,2);

	//�Ҽ�����
}

//���ý������
void CClientPlayView::SetAwardPoint(BYTE bDeskStation,BYTE iAwardPoint)
{
	m_iAwardPoint[bDeskStation] = iAwardPoint;
	UpdateViewFace(NULL);
}

//���ƽ���
void CClientPlayView::DrawAwardPoint(CDC * pDC)
{
	TCHAR szName[MAX_PATH];

	CDC dc;
	dc.CreateCompatibleDC(pDC);
	for(int k= 0;k < PLAY_COUNT;k ++)
	{
		if(m_iAwardPoint[k] == 0 )
			continue;
		BYTE temp = m_iAwardPoint[k];
		int iMyNameMsgPos=0;
		if (1==k)
		{
			CClientGameDlg *p=CONTAINING_RECORD(this,CClientGameDlg,m_PlayView);
			if (p != NULL && p->IsQueueGameRoom())
				iMyNameMsgPos = -10;
		}

		for(int i = 0; i < temp; i ++)
		{
			if(temp >= 10)
			{
				temp -= 9;
				wsprintf(szName,TEXT(".\\image\\AwardPoint3.bmp"));
				m_AwardPint.SetLoadInfo(szName,true);
			}
			else if(temp >= 5)
			{
				temp -= 4;
				wsprintf(szName,TEXT(".\\image\\AwardPoint2.bmp"));
				m_AwardPint.SetLoadInfo(szName,true);
			}
			else
			{	
				wsprintf(szName,TEXT(".\\image\\AwardPoint.bmp"));
				m_AwardPint.SetLoadInfo(szName,true);
			}

			CGameImageHelper	AwardPint(&m_AwardPint);
			dc.SelectObject(AwardPint);

#ifdef SHAO_YANG
            int OffsetX = 0;
            int OffsetY = 0;
            if (0 == k)         //��
            {
                OffsetX = 0;
                OffsetY = 0;
            }
            else if (1 == k)    //��
            {
                OffsetX = -60;
                OffsetY = -10;
            }
            else if (2 == k)    //��
            {
                OffsetX = 55;
                OffsetY = 120;
            }
            ::TransparentBlt(pDC->GetSafeHdc(),m_PtAwardPoint[k].x+i*30 + OffsetX,m_PtAwardPoint[k].y+iMyNameMsgPos + OffsetY,AwardPint.GetWidth(),AwardPint.GetHeight(),dc.GetSafeHdc(),0,0,AwardPint.GetWidth(),AwardPint.GetHeight(),dc.GetPixel(0,0));
#else
            ::TransparentBlt(pDC->GetSafeHdc(),m_PtAwardPoint[k].x+i*30,m_PtAwardPoint[k].y+iMyNameMsgPos,AwardPint.GetWidth(),AwardPint.GetHeight(),dc.GetSafeHdc(),0,0,AwardPint.GetWidth(),AwardPint.GetHeight(),dc.GetPixel(0,0));
#endif
		}
	}
	return;
}

//���ò���
void CClientPlayView::SetPass(BYTE iViewStation,int iGiveup)
{
	m_iPass[iViewStation]=iGiveup;
	UpdateViewFace(NULL);
	return ;
}

//���ƹ�
void CClientPlayView::DrawPass(CDC *pDC)
{
	CGameImageHelper	AutoLogo(&m_Pass);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	dc.SelectObject(AutoLogo);
	for(int i=0;i < PLAY_COUNT;i++)
	{
		if(m_iPass[i] != 0)
			::TransparentBlt(pDC->GetSafeHdc(),m_PtPass[i].x ,m_PtPass[i].y,AutoLogo.GetWidth(),AutoLogo.GetHeight(),dc.GetSafeHdc(),0,0,m_Pass.GetWidth(),m_Pass.GetHeight(),dc.GetPixel(0,0));
	}
	return ;
}


//�����
void CClientPlayView::DisplayTalkMsg(int Viewstation,TCHAR * szCharString)
{ 
	if (m_dlgDisTalk[Viewstation].m_hWnd)
	{
		m_dlgDisTalk[Viewstation].DisplayTalkText(szCharString);
	}
}

//��ʾ����
BOOL CClientPlayView::PreTranslateMessage(MSG * pMsg)
{
	if (pMsg->hwnd==m_showcoredlg.m_hWnd && m_bUpdateScoreboard)
	{
		// ���û��ְ��ı���ɫ
		//CBcfFile f( "GameOption.bcf");
		CBcfFile f( GET_CLIENT_BCF_NAME(szTempStr));
		CString key = TEXT("config");
		int uColorR = f.GetKeyVal(key,TEXT("ScorBarR"), 0);
		int uColorG = f.GetKeyVal(key,TEXT("ScorBarG"), 0);
		int uColorB = f.GetKeyVal(key,TEXT("ScorBarB"),0);
		m_showcoredlg.SetTextColor(RGB(uColorR, uColorG, uColorB));
		m_bUpdateScoreboard = false;
	}
	
	for(int i = 0 ;i<PLAY_COUNT;i++)
	{
		if(pMsg->hwnd == m_FlashPlayer[i].m_hWnd &&/*i != 1 &&*/ (pMsg->message == WM_PAINT||pMsg->message == WM_LBUTTONDOWN||pMsg->message == WM_RBUTTONDOWN))
		{			
			SetVideoRgn(i);			
		}
	}	

	m_ToolTip.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//���÷���
void CClientPlayView::SetOrientation(BYTE bDeskStation)
{
	m_iCurOrientation = bDeskStation ;

	UpdateViewFace(NULL);
	return ;
}

//���Ʒ���
BOOL CClientPlayView::DrawOrientation(CDC *pDC)
{
	if(m_iCurOrientation >= 0 && m_iCurOrientation < PLAY_COUNT)	//ָʾ���ڳ�����
	{
		CDC dc;
		dc.CreateCompatibleDC(pDC);
		CGameImageHelper img(&m_ImgFangXiang);
		dc.SelectObject(img);

		TransparentBlt(pDC->GetSafeHdc(),m_PtCurOrientation[m_iCurOrientation].x,m_PtCurOrientation[m_iCurOrientation].y,
			img.GetWidth()/8,img.GetHeight(),dc.GetSafeHdc(),(m_iCurOrientation + 1)*img.GetWidth()/8,0,
			img.GetWidth()/8,img.GetHeight(),RGB(255,0,255));

	}
	return TRUE;
}

//������ʼ��
void CClientPlayView::Init()
{
	//���ش���
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		m_dlgDisTalk[i].ShowWindow(SW_HIDE);             //����������
	}
}

//�O�û��A���@ʾ
BOOL CClientPlayView::SetRoomMultiple(int iRoomMultiple)
{
	m_iRoomMultiple=iRoomMultiple;

	UpdateViewFace(NULL);
	return true;
}

//�L�u�����
BOOL CClientPlayView::DrawRoomMultiple(CDC *pDC)
{
	CFont Font,*OldFont;
	TCHAR szMsg[50];
	Font.CreateFont(16,9,0,0,0,0,0,0,134,3,2,1,2,TEXT("����"));
	pDC->SetTextColor(RGB(255,255,0));
	pDC->SetBkMode(TRANSPARENT);
	OldFont=pDC->SelectObject(&Font);


	CRect rect;
#ifdef POINT5_D
	SetRect(&rect,m_PtShowMsg.x + 160,m_PtShowMsg.y+85,m_PtShowMsg.x+220,m_PtShowMsg.y+99);
#else
	SetRect(&rect,m_PtShowMsg.x + 50,m_PtShowMsg.y+40,m_PtShowMsg.x+110,m_PtShowMsg.y+54);
#endif
	wsprintf(szMsg,TEXT(" %I64d "),m_iGameMultiple);
	pDC->DrawText(szMsg,lstrlen(szMsg),rect,DT_CENTER);
	DeleteObject(pDC->SelectObject(OldFont));
	return true;
}

//������Ϸ����
BOOL CClientPlayView::SetGameMultiple(int iGameMultiple)
{
	m_iGameMultiple = iGameMultiple;
	UpdateViewFace(NULL);
	return true;
}

//�L�u�����
BOOL CClientPlayView::DrawGameMultiple(CDC *pDC)
{
	if(m_iGameMultiple == 0)
		return true;
	CFont Font,*OldFont;
	TCHAR szMsg[50];
	Font.CreateFont(12,9,0,0,0,0,0,0,134,3,2,1,2,TEXT("����"));
	pDC->SetTextColor(RGB(255,255,0));
	pDC->SetBkMode(TRANSPARENT);
	OldFont=pDC->SelectObject(&Font);

	CRect rect;
	SetRect(&rect,m_PtGameMultiple.x,m_PtGameMultiple.y,m_PtGameMultiple.x+60,m_PtGameMultiple.y+16);

	wsprintf(szMsg,TEXT(" %I64d "),m_iGameMultiple);
	pDC->DrawText(szMsg,lstrlen(szMsg),rect,DT_CENTER);
	DeleteObject(pDC->SelectObject(OldFont));
	return true;
}
//�L�u�����
BOOL CClientPlayView::DrawCallScoreResult(CDC *pDC)
{
	if(m_iCallScoreResult <= 0 || m_iCallScoreResult >3)
		return true;
	CFont Font,*OldFont;
	TCHAR szMsg[50];
	Font.CreateFont(16,9,0,0,0,0,0,0,134,3,2,1,2,TEXT("����"));
	pDC->SetTextColor(RGB(255,255,0));
	pDC->SetBkMode(TRANSPARENT);
	OldFont=pDC->SelectObject(&Font);

	CRect rect;
#ifdef POINT5_D
	SetRect(&rect,m_PtShowMsg.x+45,m_PtShowMsg.y+85,m_PtShowMsg.x+105,m_PtShowMsg.y+99);
#else
	SetRect(&rect,m_PtShowMsg.x+50,m_PtShowMsg.y+9,m_PtShowMsg.x+110,m_PtShowMsg.y+25);
#endif

	wsprintf(szMsg,TEXT(" %d "),m_iCallScoreResult);
	pDC->DrawText(szMsg,lstrlen(szMsg),rect,DT_CENTER);
	DeleteObject(pDC->SelectObject(OldFont));
	return true;
}

//�O�����汶��
BOOL CClientPlayView::SetDeskMultiple(int iDeskMultiple)
{
	m_iDeskMultiple = iDeskMultiple;
	CRect rect;
	rect.SetRect(m_PtDeskMultiple.x,m_PtDeskMultiple.y,m_PtDeskMultiple.x+120,m_PtDeskMultiple.y+20);
	UpdateViewFace(rect);	

	return true;
}

//�L�u������A��
BOOL CClientPlayView::DrawDeskMultiple(CDC *pDC)
{
	if(m_iDeskMultiple == 0)
		return true;
	CFont Font,*OldFont;
	TCHAR szMsg[50];
	Font.CreateFont(12,9,0,0,0,0,0,0,134,3,2,1,2,TEXT("����"));
	pDC->SetTextColor(RGB(255,255,0));
	pDC->SetBkMode(TRANSPARENT);
	OldFont=pDC->SelectObject(&Font);

	wsprintf(szMsg,TEXT("̨������: %I64d ��"),m_iDeskMultiple);

	pDC->TextOutA(m_PtDeskMultiple.x,m_PtDeskMultiple.y,szMsg,lstrlen(szMsg));
	DeleteObject(pDC->SelectObject(OldFont));
	return true;
}

//������ұ���
void CClientPlayView::SetPeopleMultiple(BYTE bViewStation,int iVal)
{
	m_iPeopleMul[bViewStation] = iVal;
	UpdateViewFace(NULL);	
	return ;
}

//������ұ���
void CClientPlayView::DrawPeopleMultiple(CDC *pDC)
{
	CGameImageHelper	help(&m_PeopleMul);
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	dc.SelectObject(help);
	for(int i = 0;i< PLAY_COUNT;i++)
	{
		if(m_iPeopleMul[i] == 2)
			::TransparentBlt(pDC->GetSafeHdc(),m_PtPeopleMul[i].x,m_PtPeopleMul[i].y,help.GetWidth(),help.GetHeight(),dc.GetSafeHdc(),0,0,help.GetWidth(),help.GetHeight(),dc.GetPixel(0,0));
	}
	return ;
}

//������ҽзֽ���ͱ��ʱ���ͼ
void CClientPlayView::DrawShowMsg(CDC *pDC)
{
#ifndef POINT5_D
	CGameImageHelper help(&m_ShowMsg);
	help.BitBlt(pDC->GetSafeHdc(),m_PtShowMsg.x,m_PtShowMsg.y);	
#endif

	return ;
}


///�����ǳƵ������Ƶ���ھ���д�����rect,������TRUE;û�ҵ�����falst
BOOL CClientPlayView::GetVideoRect(CString  strNickname, RECT& rect, CString& pic)
{
	CString strLocalName;
	CString logopathstr;
	for(int iCount=0; iCount<MAX_PEOPLE; iCount++)
	{
		if(m_pUserItem[iCount])
		{    
			strLocalName=m_pUserItem[iCount]->GameUserInfo.nickName;
			if(strLocalName == strNickname)
			{
				rect=m_rectVideoFrame[iCount];

				logopathstr.Format("..\\image\\log\\gamelog%d.bmp", m_pUserItem[iCount]->GameUserInfo.bLogoID);

				CString  tempstr = CBcfFile::GetAppPath()+logopathstr;

				pic = tempstr;
				return true;
			}
		}
	}	 
	return false; //û�ҵ�
}


void CClientPlayView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//˫���ջ����е���
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//˫���ջ����е���
	m_MeCard.UnUpAllCard();

	m_bAIChooseCard = false ; 

	CUsingSkinClass::OnLButtonDblClk(nFlags, point);
}
 HRGN CClientPlayView::CreateRgnFromFile( HBITMAP hBmp, COLORREF color )
{
	// get image properties
	BITMAP bmp = { 0 };
	GetObject( hBmp, sizeof(BITMAP), &bmp );
	// allocate memory for extended image information
	LPBITMAPINFO bi = (LPBITMAPINFO) new BYTE[ sizeof(BITMAPINFO) + 8 ];
	memset( bi, 0, sizeof(BITMAPINFO) + 8 );
	bi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	// set window size
	//m_dwWidth	= bmp.bmWidth;		// bitmap width
	//m_dwHeight	= bmp.bmHeight;		// bitmap height
	// create temporary dc
	HDC dc = CreateIC( "DISPLAY",NULL,NULL,NULL );
	// get extended information about image (length, compression, length of color table if exist, ...)
	DWORD res = GetDIBits( dc, hBmp, 0, bmp.bmHeight, 0, bi, DIB_RGB_COLORS );
	// allocate memory for image data (colors)
	LPBYTE pBits = new BYTE[ bi->bmiHeader.biSizeImage + 4 ];
	// allocate memory for color table
	if ( bi->bmiHeader.biBitCount == 8 )
	{
		// actually color table should be appended to this header(BITMAPINFO),
		// so we have to reallocate and copy it
		LPBITMAPINFO old_bi = bi;
		// 255 - because there is one in BITMAPINFOHEADER
		bi = (LPBITMAPINFO)new char[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD) ];
		memcpy( bi, old_bi, sizeof(BITMAPINFO) );
		// release old header
		delete old_bi;
	}
	// get bitmap info header
	BITMAPINFOHEADER& bih = bi->bmiHeader;
	// get color table (for 256 color mode contains 256 entries of RGBQUAD(=DWORD))
	LPDWORD clr_tbl = (LPDWORD)&bi->bmiColors;
	// fill bits buffer
	res = GetDIBits( dc, hBmp, 0, bih.biHeight, pBits, bi, DIB_RGB_COLORS );
	DeleteDC( dc );

	BITMAP bm;
	GetObject( hBmp, sizeof(BITMAP), &bm );
	// shift bits and byte per pixel (for comparing colors)
	LPBYTE pClr = (LPBYTE)&color;
	// swap red and blue components
	BYTE tmp = pClr[0]; pClr[0] = pClr[2]; pClr[2] = tmp;
	// convert color if curent DC is 16-bit (5:6:5) or 15-bit (5:5:5)
	if ( bih.biBitCount == 16 )
	{
		// for 16 bit
		color = ((DWORD)(pClr[0] & 0xf8) >> 3) |
				((DWORD)(pClr[1] & 0xfc) << 3) |
				((DWORD)(pClr[2] & 0xf8) << 8);
		// for 15 bit
//		color = ((DWORD)(pClr[0] & 0xf8) >> 3) |
//				((DWORD)(pClr[1] & 0xf8) << 2) |
//				((DWORD)(pClr[2] & 0xf8) << 7);
	}

	const DWORD RGNDATAHEADER_SIZE	= sizeof(RGNDATAHEADER);
	const DWORD ADD_RECTS_COUNT		= 40;			// number of rects to be appended
													// to region data buffer

	// BitPerPixel
	BYTE	Bpp = bih.biBitCount >> 3;				// bytes per pixel
	// bytes per line in pBits is DWORD aligned and bmp.bmWidthBytes is WORD aligned
	// so, both of them not
	DWORD m_dwAlignedWidthBytes = (bmp.bmWidthBytes & ~0x3) + (!!(bmp.bmWidthBytes & 0x3) << 2);
	// DIB image is flipped that's why we scan it from the last line
	LPBYTE	pColor = pBits + (bih.biHeight - 1) * m_dwAlignedWidthBytes;
	DWORD	dwLineBackLen = m_dwAlignedWidthBytes + bih.biWidth * Bpp;	// offset of previous scan line
													// (after processing of current)
	DWORD	dwRectsCount = bih.biHeight;			// number of rects in allocated buffer
	INT		i, j;									// current position in mask image
	INT		first = 0;								// left position of current scan line
													// where mask was found
	bool	wasfirst = false;						// set when mask has been found in current scan line
	bool	ismask;									// set when current color is mask color

	// allocate memory for region data
	// region data here is set of regions that are rectangles with height 1 pixel (scan line)
	// that's why first allocation is <bm.biHeight> RECTs - number of scan lines in image
	RGNDATAHEADER* pRgnData = 
		(RGNDATAHEADER*)new BYTE[ RGNDATAHEADER_SIZE + dwRectsCount * sizeof(RECT) ];
	// get pointer to RECT table
	LPRECT pRects = (LPRECT)((LPBYTE)pRgnData + RGNDATAHEADER_SIZE);
	// zero region data header memory (header  part only)
	memset( pRgnData, 0, RGNDATAHEADER_SIZE + dwRectsCount * sizeof(RECT) );
	// fill it by default
	pRgnData->dwSize	= RGNDATAHEADER_SIZE;
	pRgnData->iType		= RDH_RECTANGLES;

	for ( i = 0; i < bih.biHeight; i++ )
	{
		for ( j = 0; j < bih.biWidth; j++ )
		{
			// get color
			switch ( bih.biBitCount )
			{
			case 8:
				ismask = (clr_tbl[ *pColor ] != color);
				break;
			case 16:
				ismask = (*(LPWORD)pColor != (WORD)color);
				break;
			case 24:
				ismask = ((*(LPDWORD)pColor & 0x00ffffff) != color);
				break;
			case 32:
				ismask = (*(LPDWORD)pColor != color);
			}
			// shift pointer to next color
			pColor += Bpp;
			// place part of scan line as RECT region if transparent color found after mask color or
			// mask color found at the end of mask image
			if ( wasfirst )
			{
				if ( !ismask )
				{
					// save current RECT
					pRects[ pRgnData->nCount++ ] = CRect( first, i, j, i + 1 );
					// if buffer full reallocate it with more room
					if ( pRgnData->nCount >= dwRectsCount )
					{
						dwRectsCount += ADD_RECTS_COUNT;
						// allocate new buffer
						LPBYTE pRgnDataNew = new BYTE[ RGNDATAHEADER_SIZE + dwRectsCount * sizeof(RECT) ];
						// copy current region data to it
						memcpy( pRgnDataNew, pRgnData, RGNDATAHEADER_SIZE + pRgnData->nCount * sizeof(RECT) );
						// delte old region data buffer
						delete pRgnData;
						// set pointer to new regiondata buffer to current
						pRgnData = (RGNDATAHEADER*)pRgnDataNew;
						// correct pointer to RECT table
						pRects = (LPRECT)((LPBYTE)pRgnData + RGNDATAHEADER_SIZE);
					}
					wasfirst = false;
				}
			}
			else if ( ismask )		// set wasfirst when mask is found
			{
				first = j;
				wasfirst = true;
			}
		}

		if ( wasfirst && ismask )
		{
			// save current RECT
			pRects[ pRgnData->nCount++ ] = CRect( first, i, j, i + 1 );
			// if buffer full reallocate it with more room
			if ( pRgnData->nCount >= dwRectsCount )
			{
				dwRectsCount += ADD_RECTS_COUNT;
				// allocate new buffer
				LPBYTE pRgnDataNew = new BYTE[ RGNDATAHEADER_SIZE + dwRectsCount * sizeof(RECT) ];
				// copy current region data to it
				memcpy( pRgnDataNew, pRgnData, RGNDATAHEADER_SIZE + pRgnData->nCount * sizeof(RECT) );
				// delte old region data buffer
				delete pRgnData;
				// set pointer to new regiondata buffer to current
				pRgnData = (RGNDATAHEADER*)pRgnDataNew;
				// correct pointer to RECT table
				pRects = (LPRECT)((LPBYTE)pRgnData + RGNDATAHEADER_SIZE);
			}
			wasfirst = false;
		}

		pColor -= dwLineBackLen;
	}
	// release image data
	delete pBits;
	delete bi;

	// create region
	HRGN hRgn = ExtCreateRegion( NULL, RGNDATAHEADER_SIZE + pRgnData->nCount * sizeof(RECT), (LPRGNDATA)pRgnData );
	// release region data
	delete pRgnData;

	return hRgn;
}
void CClientPlayView::RemoveVideoBaseColor(void)
{
	CRect iRect;
	m_FlashPlayer[1].GetClientRect(iRect);
	if(iRect.Height()<=0)
	{
		return;
	}

	CClientDC dcFlash(&m_FlashPlayer[1]);

	CDC memDC;
	memDC.CreateCompatibleDC(&dcFlash);

	CBitmap memBmp;
	memBmp.CreateCompatibleBitmap(&dcFlash, iRect.Width(), iRect.Height());

	memDC.SelectObject(&memBmp);
	memBmp.DeleteObject();

	memDC.SetBkMode(TRANSPARENT);
	memDC.FillSolidRect(&iRect,RGB(255,0,255));		

	memDC.BitBlt(0, 0, iRect.Width(), iRect.Height(), &dcFlash, 0, 0, SRCCOPY);
	HRGN hRgn;
	hRgn = CreateRgnFromFile(*(memDC.GetCurrentBitmap()),0xFFFF00FF);	
	::SetWindowRgn(m_FlashPlayer[1].GetSafeHwnd(), hRgn, TRUE );	

	memDC.DeleteDC();

}
void CClientPlayView::SetVideoRgn(BYTE iView)
{
	 CRgn rgnVideoBox;
	if(1 == iView)
	{
		rgnVideoBox.CreateRectRgn(0,0,215,150);
		m_FlashPlayer[iView].SetWindowRgn((HRGN)rgnVideoBox,TRUE);

	}
	else
	{
		rgnVideoBox.CreateRectRgn(4,0,180,200);
		m_FlashPlayer[iView].SetWindowRgn((HRGN)rgnVideoBox,TRUE);
	}
	
}

//��2.5D�Ʊ�
void CClientPlayView::DarwCardImage(CDC *pDC)
{
	CUpGradeGameCard *pGameCard;
	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		pGameCard = &m_HandCard[i];

		//�����ж�
		if(NULL==pGameCard||pGameCard->m_CardArray.GetCount() <= 0||	//��������Ϊ0
			pGameCard->m_XShowMode== CUpGradeGameCard::XShowMode::SWX_MODE_CENTER)	//�����Լ�����
		{
			continue;
		}

		CByteArray TempCardArray;
		TempCardArray.Copy(pGameCard->m_CardArray);

		//ȥ�������еĵ���
		int iTemp = 0;
		for(int cardIndex = 0; cardIndex < TempCardArray.GetCount(); ++cardIndex)
		{
			//��������αȽ�
			for(int index = 0; index < m_MeBackCard.m_CardArray.GetCount(); ++index)
			{
				if(TempCardArray.GetAt(cardIndex) == m_MeBackCard.m_CardArray.GetAt(index))
				{
					TempCardArray.RemoveAt(cardIndex);
					iTemp++;
					break;
				}
			}

			//ȫ������
			if(iTemp >= 3)
			{
				break;
			}
		}

		//û����
		if(!TempCardArray.GetAt(0))
		{	
			Rect RectXY;
			CString strTemp;
			if(pGameCard->m_XShowMode==CUpGradeGameCard::XShowMode::SWX_MODE_LEFT)
			{
				strTemp.Format(".\\image\\CardLeft\\%d.png",pGameCard->m_CardArray.GetCount());
				RectXY.X = pGameCard->m_PtBasePoint.x-85;
			}
			else if(pGameCard->m_XShowMode==CUpGradeGameCard::XShowMode::SWX_MODE_RIGHT)
			{
				strTemp.Format(".\\image\\CardRight\\%d.png",pGameCard->m_CardArray.GetCount());
				RectXY.X = pGameCard->m_PtBasePoint.x-145;
			}

			RectXY.Y = pGameCard->m_PtBasePoint.y - 40;

			//��ͼ
			Image *pImage = Image::FromFile(CA2W(strTemp));
			RectXY.Width = pImage->GetWidth();
			RectXY.Height = pImage->GetHeight();
			if(NULL != pImage)
			{
				Graphics gDC(pDC->GetSafeHdc());
				gDC.DrawImage(pImage,RectXY);
			}
			
			pGameCard->ShowWindow(SW_HIDE);		//�����ƿؼ�
		}
		else
		{
			pGameCard->ShowWindow(SW_SHOW);		//��ʾ�ƿؼ�
		}
	}
}

//���ö�������
void CClientPlayView::SetAnimStitch(int iStitch)
{
	m_iCurrentStitch = iStitch;
}

//������ͼƬ
void CClientPlayView::DarwChuiAnim(CDC *pDC,int iW,int iH)
{
	if(m_iCurrentStitch >= 5 || NULL == m_ImageAnim[m_iCurrentStitch] || m_ImageAnim[m_iCurrentStitch]->GetHeight() < 0)
	{
		return;
	}
	
	Image *pImage = m_ImageAnim[m_iCurrentStitch];

	Graphics gDC(pDC->GetSafeHdc());
	gDC.DrawImage(pImage,Rect((iW-pImage->GetWidth())/2+50,(iH-pImage->GetHeight())/2,pImage->GetWidth(),pImage->GetHeight()));
}

//���ô�һ������
void CClientPlayView::SetChuiData(BYTE byStation, BYTE iFlag)
{
	m_iChui[byStation] = iFlag;
}

//���ƴ�һ����־
void CClientPlayView::DarwChuiSign(CDC *pDC)
{
	Image *pImage = m_pChuiSign;
	Graphics gDC(pDC->GetSafeHdc());

	if(NULL == pImage)
	{
		return;
	}

	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(1 == m_iChui[i])
			continue;

		if(2 == m_iChui[i])		//�Ӵ�
		{
			gDC.DrawImage(pImage,m_PtChui[i].x,m_PtChui[i].y,0,0,55,40,Unit::UnitPixel);
		}
		else if(0 == m_iChui[i])	//δ�Ӵ�
		{
			gDC.DrawImage(pImage,m_PtChui[i].x,m_PtChui[i].y,55,0,55,40,Unit::UnitPixel);
		}
	}
 }

