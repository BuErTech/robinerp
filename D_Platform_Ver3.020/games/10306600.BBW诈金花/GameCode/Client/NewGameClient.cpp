#include "stdafx.h"
#include "NewGameClient.h"
#include "IGameFrame.h"
//#include "UI8_ControlsDefine.h"
#include "UI_ForLogic/IUserInterface.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "Common/UIMessageDefines.h"
#include "LoveSendClassInExe.h"
#include "define.h"
//#include "LoveSendClassInExe.h"


IGameImpl *CreateNewGameImpl(IGameFrame* pIBase, IUserInterface *pIUI)
{
	return dynamic_cast<IGameImpl*>(new CNewGameClient(pIBase, pIUI));
}
void DestroyNewGameImpl(IGameImpl **ppImp)
{
	delete *ppImp;
	*ppImp = NULL;
}

/**
* ����ǰ��ұ��ʵõ��ַ���
* @param [int] int nNum Ҫ��ʾ������
* @param [out] CString &Str ���ص��ַ���
* @param [int] int nPower ��ұ���
* @param [in]  BOOL bIsSpace TRUE: �ÿո�ָ� FALSE: ���ָ�
* @return void
*/
void GlbGetFloatString( char  Str[256], __int64 nNum, int nPower=0, BOOL bIsSpace=TRUE)
{
	if (0 == nNum)
	{
		wsprintf(Str,"0");
		return;
	}

	// �ݴ���
	TCHAR strTmp[32];
	TCHAR str[50];

	// Ŀǰֻ�������0��������������ַ�������Ҫ������һ��0
	wsprintf(strTmp, TEXT("%I64d"), nNum);

	for (int i = 0; i < nPower; ++i)
	{
		_tcscat(strTmp, TEXT("0"));
	}

	// ��λ��ÿ4λ�ö��Ÿ���ʾ
	int nLen = _tcslen(strTmp);	// �ַ�������
	int nTimes = nLen % 4;	    // ����ַ������ԷֳɵĽ�����������1��

	int nSrc = 0;
	int nDes = 0;

	for (int i = 0; i < nLen; ++i)
	{
		if ((i > 0) && (i % 4 == nTimes) && bIsSpace && (str[i-1] != '-'))
		{
			str[nDes++] = TEXT(',');
		}

		str[nDes++] = strTmp[nSrc++];
	}

	str[nDes] = TEXT('\0');
	wsprintf(Str,"%s",str);
}

CNewGameClient::CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI)
{
	m_pGameFrame = pGameFrame;
	m_pUI = pUI;
	m_bGameStation = GS_WAIT_SETGAME;           //��Ϸ״̬
	m_iBeginTime = 0;			//��Ϸ��ʼʱ��
//	m_iWaitTime = 0;			//�˹���ȴ�ʱ��
	m_iThinkTime = 0;			//��Ϸ˼��ʱ��
	m_iEndWaitTime = 0;			//��Ϸ������ȴ�ʱ��
	m_iPlayCardCount = 0;			//ʹ���˿���
	m_iCurrentTimeId = 0;       //��ǰ��ʱ��ID
	m_bTimeOutCount = 0;
//	m_pMsgDlg = NULL;
	m_bSortCard = 0;

	m_bSendTestCard = FALSE;
	m_bSound = TRUE;
	m_SoundEngine = NULL;
	m_bInLisureRoom = FALSE;

	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));			//��������ϵ��˿���
	::memset(m_iUserCard,0,sizeof(m_iUserCard));			//��������ϵ��˿�
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));			//�����˿˵���Ŀ
	::memset(m_iDeskCard,0,sizeof(m_iDeskCard));			//������˿�
	m_SoundEngine = createIrrKlangDevice();
	//AfxMessageBox("zhtlog:CNewGameClient");

}

CNewGameClient::~CNewGameClient()
{

}

//��Ϸ�˳�
bool CNewGameClient::OnGameQuiting()
{
	
	//��ѯ״̬
	if (GetStationParameter() >= GS_SEND_CARD&&GetStationParameter() < GS_WAIT_NEXT && m_bPlayer[m_pGameFrame->GetMyDeskStation()]
	&& !m_pGameFrame->GetIsWatching())
	{
		CString strInfo = "";
		strInfo += "��������Ϸ�У��Ƿ�Ҫ�˳���Ϸ?\n";

		char cKey[10];
		CString sPath=CBcfFile::GetAppPath();
		int iResult = 1;
		if (CBcfFile::IsFileExist(sPath + "..\\SpecialRule.bcff"))
		{
			CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
			sprintf(cKey, "%d", NAME_ID);
			iResult = fsr.GetKeyVal (_T("ForceQuitAsAuto"), cKey, 0);
		}


		if (iResult)
		{
			strInfo.Format("�����˳����ᱻ��������ע�������Ҫ�˳���?");
		}
		else
		{
			strInfo.Format("��ע���˳����ᱻ�й�,�����Ҫ�˳���");
		}

		if(IDOK != AFCMessageBox(strInfo, "��ʾ",AFC_YESNO))
		{
			return false;
		}	
	}

	m_pGameFrame->SendGameData(NULL,0,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	memset(m_bPlayer,0,sizeof(m_bPlayer));
	return true;
}

//���������ļ�
void CNewGameClient::LoadData(void)
{
	CString s = CBcfFile::GetAppPath ();/////����·��
	CBcfFile f(s + GET_SKIN_FOLDER(szTempStr) +".bcf");
	CString key = TEXT("Operate");
	m_bSound = f.GetKeyVal(key,"Sound",TRUE);
	m_bSendTestCard = f.GetKeyVal("Test","TestCard",FALSE);
}

/// ��ʼ��
int CNewGameClient::Initial()
{
	// ����UI
	LoadData();
	m_pUI->LoadData(L"UI.dat"); 
	const GameInfoStructInExe* gameinfo =  m_pGameFrame->GetGameInfo();
	m_bInLisureRoom = FALSE; //�����ã���ʽ�ƽ�ɾ��
	
	memset(m_bPlayer,0,sizeof(m_bPlayer));

	return 0;
}
/// ��ʼ��UI
int CNewGameClient::InitUI()
{
	BYTE Sequence[PLAY_COUNT][2] = {0};
	
	InitGameData();

	if (m_bSound)
	{
		ShowControl(IDD_SOUND_OPEN_BT,true);
		ShowControl(IDD_SOUND_CLOSE_BT,false);
	}
	else
	{
		ShowControl(IDD_SOUND_OPEN_BT,false);
		ShowControl(IDD_SOUND_CLOSE_BT,true);
	}
	InitGameUI();
	
	ShowControl(IDD_START_BT,false);

	ShowControl(IDD_SET_THING_DLG,false);
	/*if (m_bInLisureRoom)
	{
		ShowControl(IDD_START_BT, true);
		SetGameTimer(m_pGameFrame->GetMyDeskStation(),30,ID_BEGIN_TIME);
	}*/

	return 0;
}

///������Ϸ��ͬ״̬ʱ���������ݣ���Ҫ���ڶ�������
void CNewGameClient::SetGameStation(void * pBuffer,int nLen)
{
		
	InitGameUI();
	InitGameData();
	ShowLogo();
	/*IImage * pImage = NULL;
	GETCTRL(IImage,pImage,m_pUI,1004140);
	pImage->SetIsMove(true);
	pImage->SetMouseThough(true);*/
	const GameInfoStructInExe* gameinfo =  m_pGameFrame->GetGameInfo();
	m_bInLisureRoom = FALSE; //�����ã���ʽ�ƽ�ɾ��
//	m_Logic.Msg("zhtlog:m_bInLisureRoom=%d",m_bInLisureRoom);
	int iMyDesk = m_pGameFrame->GetMyDeskStation();
	GameStationBase *pGameBase = (GameStationBase *) pBuffer;
	SetStationParameter(pGameBase->bGameStation);
	if(m_SoundEngine)
	{
		m_SoundEngine->setAllSoundsPaused(true);
		m_SoundEngine->stopAllSounds();
	}
	PlayRandSound(SOUND_BG_1, SOUND_BG_3, true);

	UpDateMyUserInfo(); //�����Լ����û���Ϣ	

	UpdateRoomInfo();
	//�������ʣ��Ľ��
	UpdateMyLeftMoney();
	UpdateAddChipShow(0);
	SetUserInfo();
	ShowControl(IDD_START_BT,false);

	switch(GetStationParameter())
	{
	case GS_WAIT_SETGAME:
	case GS_WAIT_ARGEE:
		{
	
			GameStation_2 * pGameStation=(GameStation_2 *)pBuffer;
		
			if (m_bSendTestCard)
			{
				IButton *pbut = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(10));
				if (NULL != pbut)
				{
					IBCInterFace  * pClone  = pbut->Clone(99999);
					if (NULL != pClone)
					{
						IButton * pSetCard = dynamic_cast<IButton *>(pClone);
						m_pUI->IInsertControl(pSetCard);
						pSetCard->SetGameXY(10,10);
						pSetCard->SetControlVisible(true);
						pSetCard->SetEnable(true);
						pSetCard->SetZorder(1);
						//pSendCard->SetEnable(false);
					}
				}
			}

			
			m_iThinkTime=pGameStation->iThinkTime;
			m_iBeginTime=pGameStation->iBeginTime;
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�

			m_iLimitNote=pGameStation->iLimitNote;			//���ע
			m_iBaseNote=pGameStation->iBaseNote;			//��ע
			m_iLimitPerNote =pGameStation->iLimtePerNote;   //��ע���ֵ
			m_iGuoDi = pGameStation->iGuoDi;     //����ֵ

			m_CbIsShowIdOrPlay = pGameStation->bShowIdOrPlay;

			m_iFrontNote = 0;

			m_pGameFrame->GetIsWatching();
			if (m_pGameFrame->GetGameInfo()->dwRoomRule & GRR_QUEUE_GAME)
			{
				
				ShowControl(IDD_START_BT,false);
			}
			else
			{
						
				
				ShowControl(IDD_START_BT,true);
				SetGameTimer(m_pGameFrame->GetMyDeskStation(),m_iThinkTime,ID_BEGIN_TIME);
			}
			
			break;
		}
	case GS_SEND_CARD:
		{
			GameStation_3 * pGameStation=(GameStation_3 *)pBuffer;

			//��������
			m_iThinkTime=pGameStation->iThinkTime;
			m_iBeginTime=pGameStation->iBeginTime;
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�

			m_iLimitNote=pGameStation->iLimitNote;			//���ע
			m_iBaseNote=pGameStation->iBaseNote;			//��ע
			m_iLimitPerNote =pGameStation->iLimtePerNote;   //��ע���ֵ
			m_iGuoDi = pGameStation->iGuoDi;                //����ֵ
			
			m_iFrontNote = 0;
			for (int i =0;i<PLAY_COUNT;i++)
			{
				m_strCityText[i].Format("%s",pGameStation->strCityText[i]);
				m_strUserIDText[i].Format("%s",pGameStation->strUserIDText[i]);
				m_bSuperUser[i] = pGameStation->bSuperUser[i];
			}	

			::CopyMemory(m_bPlayer,pGameStation->bPlayer,sizeof(m_bPlayer));
			UpdateRoomInfo();
			//m_PlayView.m_btContinue.ShowWindow(SW_HIDE);

			//			::CopyMemory(m_iDeskCardCount,pGameStation->iUserCardCount,sizeof(pGameStation->iUserCardCount));
			for (int i=0;i<PLAY_COUNT;i++)
			{	
				//���ø���������
				//int desk=ViewStation(i);
				//��������
				m_iDeskCardCount[i]=pGameStation->iUserCardCount[i];
				::CopyMemory(m_iUserCard[i],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);
				::CopyMemory(m_iDeskCard[i],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);
				memset(m_iDeskCard[i],0x00,sizeof(m_iDeskCard[i]));
				//m_PlayView.m_UserCard[desk].SetCard(m_DeskCard[desk],NULL,m_iDeskCardCount[desk]);
				SetHandCard(i,m_iDeskCard[i],NULL,m_iDeskCardCount[i],0);

				//���ø�������Ѻע���
				//SetNotePoint(i,pGameStation->iTotalNote[i]);
				//ϵͳ����Ա��ʾ���ҵ���
				/*if(GetMeUserInfo()->bGameMaster==SUPER_PLAYER||true == m_bSuperUser[GetMeUserInfo()->bDeskStation])
					SetHandCard(i,m_iUserCard[i],NULL,m_iDeskCardCount[i],0);*/
				//m_PlayView.m_UserCard[desk].SetCard(m_UserCard[desk],NULL,m_iDeskCardCount[desk]);

			}

			//������Ѻע���
			SetTotalNote(0, m_iBaseNote*PLAY_COUNT);

			m_CbIsShowIdOrPlay = pGameStation->bShowIdOrPlay;

			break;
		}
	
		
	case GS_PLAY_GAME:
		{
			GameStation_4 * pGameStation=(GameStation_4 *)pBuffer;
			
			DisVisibleAllTip(true);
			//��������
			m_iThinkTime=pGameStation->iThinkTime;
			m_iBeginTime=pGameStation->iBeginTime;
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�
			m_iLimitNote=pGameStation->iLimitNote;			//���ע
			m_iBaseNote=pGameStation->iBaseNote;			//��ע
			m_iLimitPerNote =pGameStation->iLimtePerNote;   //��ע���ֵ
			m_iGuoDi = pGameStation->iGuoDi;                //����ֵ

			m_bFirstGame = pGameStation->bIsFirstNote;     //�Ƿ�Ϊ��һ����ע
			m_bNtPeople = pGameStation->bNtPeople;
			for (int i =0;i<PLAY_COUNT;i++)
			{
				m_strCityText[i].Format("%s",pGameStation->strCityText[i]);
				m_strUserIDText[i].Format("%s",pGameStation->strUserIDText[i]);
				m_bSuperUser[i] = pGameStation->bSuperUser[i];
			
			}	

			SetNT(m_bNtPeople);
			UpdateRoomInfo();


			m_iNowOutPeople=pGameStation->iOutCardPeople;


			m_iFrontNote = pGameStation->iCurNote;

			int iRestTime = pGameStation->iTimeRest;
			if (iRestTime <= 0)
			{
				iRestTime = 1;
			}
			m_iFirstOutPeople=pGameStation->iFirstOutPeople;
			//���浱ǰ�û���ע�ܶ�
			::CopyMemory(m_iThisGameNote,pGameStation->iTotalNote,sizeof(pGameStation->iTotalNote));
			::CopyMemory(m_bMing,pGameStation->iMing,sizeof(pGameStation->iMing));
			::CopyMemory(m_iThisGameNote,pGameStation->iThisGameNote,sizeof(pGameStation->iThisGameNote));
			::CopyMemory(m_iTotalGameNote,pGameStation->iTotalGameNote,sizeof(pGameStation->iTotalGameNote));
			::CopyMemory(m_bOpenLose,pGameStation->bOpenLose,sizeof(pGameStation->bOpenLose));
			::CopyMemory(m_bIsGiveUp,pGameStation->bIsGiveUp,sizeof(pGameStation->bIsGiveUp));
			::CopyMemory(m_byWin,pGameStation->byWin,sizeof(m_byWin));
			::CopyMemory(m_bPlayer,pGameStation->bPlayer,sizeof(m_bPlayer));
			//lym2010-03-29��¼ʤ�����ݣ��������ʾ
			BYTE m_byWinNer[PLAY_COUNT] ;
			::CopyMemory(m_byWinNer,pGameStation->byWin,sizeof(m_byWinNer));

			m_iThisTurnLimit=m_iLimitNote;

			m_iNowBigNote=pGameStation->tag.iNowBigNote;

			SetTotalNote(0, pGameStation->tag.iTotalNote);

			//����ʱ��
			if (m_iNowOutPeople != -1) 
				SetGameTimer(m_iNowOutPeople,iRestTime,ID_OUT_CARD_TIME);



			BOOL bCut = FALSE;
			int  iCountPeople = 0;
			m_iThisNoteTimes = pGameStation->tag.bAddTime;

			for (int i=0;i<PLAY_COUNT;i++)
			{
				/*if (m_pUserInfo[i] == NULL)
				{
					continue;
				}*/
				iCountPeople++;
				//���ø���������
				//int desk=ViewStation(i);
				//��������
				m_iDeskCardCount[i]=pGameStation->iUserCardCount[i];
				::CopyMemory(m_iUserCard[i],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);

				if( i== m_pGameFrame->GetMyDeskStation() && pGameStation->iMing[i])
				{
					memcpy(m_iDeskCard[i] , pGameStation->iUserCard[i] ,sizeof(BYTE)*(m_iDeskCardCount[i])) ; 
					
				}
				else
				{
					memset(m_iDeskCard[i],0x00,sizeof(m_iDeskCard[i]));
				}
				
				SetMing(i,pGameStation->iMing[i]);			

	
				//���ø�������Ѻע���
				//				SetNotePoint(i,pGameStation->iTotalNote[i]);

				SetHandCard(i,m_iDeskCard[i],NULL,m_iDeskCardCount[i],0);

			
				UserInfoForGame UserInfo;

				if (m_pGameFrame->GetUserInfo(m_pGameFrame->GetMyDeskStation(),UserInfo) == FALSE)
				{
					return;
				}


				//ϵͳ����Ա��ʾ���ҵ���
				if(UserInfo.bGameMaster==SUPER_PLAYER||true == m_bSuperUser[m_pGameFrame->GetMyDeskStation()])
				{
					//m_PlayView.m_UserCard[desk].SetCard(m_UserCard[desk],NULL,m_iDeskCardCount[desk]);
					SetHandCard(i,m_iUserCard[i],NULL,m_iDeskCardCount[i],0);
				}

				if (m_bIsGiveUp[i])
				{
					SetTipInfo(i,TYPE_GIVE_UP); //����
				}
				m_iComWinStation = PLAY_COUNT;

				
				if (m_bOpenLose[i])
				{
					m_iComLoseStation = i; 
		//			OnFinishOpenCard();
					bCut = TRUE;
				}

			}

			

			//��ʱ��ʾ��ע��Ϣ
			//Added by JianGK.20100715
//			SetTimer(ID_DELAY_NOTE_INFO,300,NULL);

//			OnFinshLookCard();

			//add by ljl 
			if ( pGameStation->tag.iCutOutLastpeople >=0 && pGameStation->tag.iCutOutLastpeople < PLAY_COUNT )
			{
				if ( TYPE_COMPARE_CARD  != pGameStation->tag.iCutOutLastType && !bCut )
				{

					SetTipInfo(pGameStation->tag.iCutOutLastpeople,pGameStation->tag.iCutOutLastType);
				}	
			}

			UpdateMyLeftMoney();
		
			SetCtrlBtn(m_bFirstGame);

			PlayXianShiChouMa(m_pGameFrame->GetMyDeskStation() ,m_iNoteTotalPoint , true);

			//�������ʣ��Ľ��
			UpdateMyLeftMoney();
			//���Ŷ�ʱ������������ׯ����
			if ( iCountPeople > 2 || !bCut )
			{
				SetGameTimer(m_iNowOutPeople,iRestTime, ID_OUT_CARD_TIME);
			}

			m_CbIsShowIdOrPlay = pGameStation->bShowIdOrPlay;

			// �����û���ʾ��Ϣ
			SetUserInfo();
	
			//lym2010-03-29���ʤ����־
			for (int i=0;i<PLAY_COUNT;i++)
			{/*
				if (m_pUserInfo[i] == NULL)
				{
					continue;
				}*/
				if (m_byWinNer[i] == 2)
				{
					SetTipInfo(i, TYPE_OPEN_WIN);
				}
				
			}


			break;
		}
	case GS_WAIT_NEXT:
		{
			GameStation_5 * pGameStation=(GameStation_5 *)pBuffer;
			//��������
			//��������
			m_iThinkTime=pGameStation->iThinkTime;
			m_iBeginTime=pGameStation->iBeginTime;
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�

			m_iLimitNote=pGameStation->iLimitNote;			//���ע
			m_iBaseNote=pGameStation->iBaseNote;			//��ע
			m_iLimitPerNote =pGameStation->iLimtePerNote;   //��ע���ֵ
			m_iGuoDi = pGameStation->iGuoDi;   //����ֵ

			m_CbIsShowIdOrPlay = pGameStation->bShowIdOrPlay;

			for (int i =0;i<PLAY_COUNT;i++)
			{
				m_strCityText[i].Format("%s",pGameStation->strCityText[i]);
				m_strUserIDText[i].Format("%s",pGameStation->strUserIDText[i]);
				m_bSuperUser[i] = pGameStation->bSuperUser[i];
			}	

			// �����û���ʾ��Ϣ
			SetUserInfo();

			UpdateRoomInfo();
			if (m_pGameFrame->GetGameInfo()->dwRoomRule & GRR_QUEUE_GAME)
			{

				ShowControl(IDD_START_BT,false);
			}
			else
			{


				ShowControl(IDD_START_BT,true);
				SetGameTimer(m_pGameFrame->GetMyDeskStation(),m_iThinkTime,ID_BEGIN_TIME);
			}
			break;
		}
	}
}

/// ��Ϸ��Ϣ
int CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{
	switch(nSubId)
	{
	case ASS_GM_GAME_STATION:           ///<����״̬
		{			    
			SetGameStation(buffer,nLen);
			break;
		}
	case ASS_BEGIN_UPGRADE:
		{
			if (nLen!=sizeof(BeginUpgradeStruct)) 
			{
				return FALSE;
			}
			SetStationParameter(GS_SEND_CARD);
					
//			m_pGameFrame->CloseWaitGroup();
	
			BeginUpgradeStruct * pBegin=(BeginUpgradeStruct *)buffer;

			int iMyDesk = m_pGameFrame->GetMyDeskStation();
		
			// ��������ͷ��
			ShowLogo();
			//SetUserInfo(GetMeUserInfo()->bDeskStation,m_pUserInfo[GetMeUserInfo()->bDeskStation],true);

			ShowControl(IDD_ZJH_BTN_ADD_3, SW_SHOW);
			ShowControl(IDD_ZJH_BTN_ADD, SW_SHOW);
			ShowControl(IDD_ZJH_BTN_FOLLOW, SW_SHOW);
			ShowControl(IDD_ZJH_BTN_GIVEUP, SW_SHOW);
			ShowControl(IDD_ZJH_BTN_LOOK, SW_SHOW);
			ShowControl(IDD_ZJH_BTN_OPEN, SW_SHOW);
			ShowControl(IDD_ZJH_BTN_ADD_1, SW_SHOW);
			ShowControl(IDD_ZJH_BTN_ADD_2, SW_SHOW);
			ShowControl(IDD_ZJH_BTN_ADD_CANCLE, SW_SHOW);
			//ShowControl(IDD_ZJH_SHOW_ADD_COUNT, SW_SHOW);
			ShowControl(IDD_ZJH_GUODI_VALUE, SW_SHOW);
			ShowControl(IDD_ZJH_MAXLIMIT_VALUE, SW_SHOW);
			ShowControl(IDD_ZJH_DIZHU_VALUE, SW_SHOW);
			ShowControl(IDD_ZJH_MAX_ZHU_VALUE, SW_SHOW);
			//ShowControl(IDD_ZJH_USER_NAME, SW_SHOW);
			//ShowControl(IDD_ZJH_USER_POINT, SW_SHOW);
			//ShowControl(IDD_ZJH_USER_LEFT_MONEY, SW_SHOW);

			memset(m_byWin,0,sizeof(m_byWin));
			SetStationParameter(GS_SEND_CARD);
			m_bTimeOutCount=0;												//��ʱ����
			m_iLimitNote=pBegin->iLimitNote;								//���ע
			m_iBaseNote=pBegin->iBaseNote;					//��ע
			m_iLimitPerNote = pBegin->iLimtePerNote;
			m_iGuoDi = pBegin->iGuoDi;
			m_bNtPeople = pBegin->bNtStation;

			m_CbIsShowIdOrPlay = pBegin->bIsShowIdOrPlay;
			for (int i =0;i<PLAY_COUNT;i++)
			{
				m_strCityText[i].Format("%s",pBegin->strCityText[i]);
				m_strUserIDText[i].Format("%s",pBegin->strUserIDText[i]);
				m_bSuperUser[i] = pBegin->bSuperUser[i];
			}	

			//m_Logic.Msg("zhtlog:%I64d pBegin->iLimitNote=%I64d",m_iLimitNote,pBegin->iLimitNote);

			m_iFrontNote = 0;
			m_iMyAddNote = 0;
			
			// �����û���ʾ��Ϣ
			SetUserInfo();

			for(int i=0;i<PLAY_COUNT;i++)
			{
				UserInfoForGame UserInfo;

				if (m_pGameFrame->GetUserInfo(i,UserInfo))
				{

					m_iThisGameNote[i]+=m_iGuoDi;
					m_iTotalGameNote[i]+=m_iGuoDi;

					AddTotalNote(i,m_iGuoDi);	
					m_bPlayer[i] = TRUE;
				}
				else
				{
					m_bPlayer[i] = FALSE;
				}
			}

			for (int i=0; i<PLAY_COUNT; i++)
			{

				UserInfoForGame UserInfo;

				if (m_pGameFrame->GetUserInfo(i,UserInfo))
				{

					UpdateNoteInfo(i);

					PlayXianShiChouMa(i,m_iGuoDi,true);   //�����³��붯��
				}
			//	m_Logic.Msg("zhtlog:m_iGuoDi=%d",m_iGuoDi);

			}

			UpDateMyUserInfo(); //�����Լ����û���Ϣ	

			UpdateRoomInfo();
			//�������ʣ��Ľ��
			UpdateMyLeftMoney();
			UpdateAddChipShow(0);

			KillGameTimer(0);
			//m_Logic.Msg("zhtlog2:m_bSendCardPos=%d",m_bSendCardPos);
			//���Ƿ���
			m_bLookedPai = false;

			return true;

			
			break;
		}
	case ASS_SEND_CARD:			//����������
		{
			if (nLen!=sizeof(SendCardStruct)) 
				return FALSE;
			SendCardStruct * pSendCard=(SendCardStruct *)buffer;
			//�û�������
			//StopAllAnimateNodes();  //ֹͣ���еĶ�������
			for (int i=0; i<PLAY_COUNT; i++)
			{
				if (pSendCard->bCardCount[i]>0)
				{
					for (int j=0; j<MAX_CARD_COUNT; j++)
					{
						m_iUserCard[i][m_iDeskCardCount[i]]=pSendCard->bCard[i][j];
						m_iDeskCard[i][m_iDeskCardCount[i]++]=pSendCard->bCard[i][j];			//�û������ϵ���
					}
				}
				
				//��ⷢ��״��
				memset(m_iDeskCard[i],0x00,sizeof(m_iDeskCard[i]));					
			}

			//���ŷ��ƶ���
			StartPlaySendCard();
			return true;

		}
	case ASS_COLLECT_JETON://�ռ�����
		{
			return true;
		}
	case ASS_SEND_FINISH:		//�������
		{	

			return true;
		}

	case ASS_GAME_PLAY:		//��ʼ��Ϸ
		{
			//������ɣ���ʼ��ע
			if (nLen!=sizeof(BeginPlayStruct)) 
				return FALSE;
			//������Ϸ״̬
			SetStationParameter(GS_PLAY_GAME);

			BeginPlayStruct * pBeginInfo=(BeginPlayStruct *)buffer;
			//������Ϸ״̬
			SetStationParameter(GS_PLAY_GAME);

			m_bTimeOutCount=0;		//��ʱ��������			

			m_iNowOutPeople=m_iFirstOutPeople=pBeginInfo->iOutDeskStation;
			m_iNowBigNote=0;							//��ǰ���Ѻע��
			m_iThisTurnLimit=m_iLimitNote;//m_iLimitNote/32*(1<<1)/m_iBaseNote*m_iBaseNote;
			//��һ����ע
			m_iThisNoteTimes=0;
			m_bNtPeople = pBeginInfo->bNtPeople;   //ׯ��λ��

			//����ʱ�� 
			SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD_TIME);
			if ((m_pGameFrame->GetMyDeskStation()==m_iNowOutPeople) && !m_pGameFrame->GetIsWatching())
			{
				m_bFirstGame = true;
				FlashWindow();
				SetCtrlBtn(true);   //��һ����ע	
			}

			SetNT(m_bNtPeople);
			return true;

		}
	case ASS_NOTE:								//���Ѻע���
		{

			UserInfoForGame UserInfo;

			if (m_pGameFrame->GetUserInfo(m_pGameFrame->GetMyDeskStation(),UserInfo) == FALSE)
			{
				return true;
			}
			if(nLen==sizeof(lookCardResult))
			{
				lookCardResult *pResult = (lookCardResult *) buffer;
				m_bMing[pResult->bDeskStation]=true;
				m_bLookedPai = true;
				SetMing(pResult->bDeskStation,1);
				//������Ч

				UserInfoForGame info;
				bool bBoy = true;
				if (m_pGameFrame->GetUserInfo(pResult->bDeskStation,info))
				{
					bBoy = info.bBoy;
				}
				SetNoteSound(TYPE_LOOKCARD,bBoy);//��ע����

				if (SUPER_PLAYER!=UserInfo.bGameMaster&&false == m_bSuperUser[UserInfo.bDeskStation])
				{
					memcpy(m_iUserCard[pResult->bDeskStation],pResult->iUserCard,sizeof(BYTE)*pResult->iUserCardCount);
				}		
				m_bLookCardW = 30;
				m_pGameFrame->SetTimer(ID_TIME_LOOKCARD,50);
				
				return true;
			}	
			if (nLen != sizeof(tagUserResult)) 
				return FALSE;

			tagUserResult *pResult = (tagUserResult *) buffer;
			//�û���ע������ʾ��ע����,�ݲ���ֵ
			m_iNowOutPeople = pResult->iOutPeople;		//��ǰ��ע��				
			m_iThisTurnLimit=m_iLimitNote;				
			//ϵͳ������ע����
			m_iThisNoteTimes=pResult->bAddTime;
			//��ǰ���Ѻע
			m_iNowBigNote=pResult->iNowBigNote;
			m_bFirstGame = pResult->bIsFirstNote;  //�ж��Ƿ�Ϊ��һ����ע

			if (m_iNowOutPeople == UserInfo.bDeskStation  && !m_pGameFrame->GetIsWatching())
			{					
				SetCtrlBtn(m_bFirstGame);
				FlashWindow();
			}
			//����ʱ��
			if (m_iNowOutPeople!=-1 || m_iNowOutPeople >= PLAY_COUNT) 
			{
				SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD_TIME);
			}

			return true;

		}
	case ASS_BIPAI_RESULT:
		{
			if (nLen!=sizeof(tagCompareCardResult)) 
				return FALSE;

			tagCompareCardResult *result =(tagCompareCardResult*)buffer;

			KillGameTimer();

			TCHAR szMessage[100];

			AddTotalNote(result->iNoNt,result->iCurNote);

			//������Ч

			UserInfoForGame info;
			bool bBoy = true;
			if (m_pGameFrame->GetUserInfo(result->iNt,info))
			{
				bBoy = info.bBoy;
			}
			SetNoteSound(TYPE_OPENCARD,bBoy);//��ע����

			//����ĳһ������ע�ܶ�
			m_iThisGameNote[result->iNt]+=result->iCurNote;
			m_iTotalGameNote[result->iNt]+=result->iCurNote;

			m_bOpenLose[result->iLoster] = true;

			m_iComLoseStation = result->iLoster;

			if (result->bAddStyle == TYPE_REACH_LIMITE_SELF)  //�ﵽ����
			{
				m_bReachLimitStation = result->iNt;
			}
			else if (result->bAddStyle == TYPE_REACH_LIMITE_NEXT)  //��һ�Ҵﵽ����
			{
				m_bReachLimitStation = GetNextNoteStation(result->iNt);
			}
			else
			{
				m_bReachLimitStation = 255;
			}

			int iComRet = 0;
			if (result->iNoNt == result->iLoster)
			{
				iComRet = 1;
				m_iComLoseStation = result->iNoNt;
				m_iComWinStation = result->iNt;
			}
			else
			{
				m_iComLoseStation = result->iNt;
				m_iComWinStation = result->iNoNt;
			}

			// ���ƽ������Ƿ���Ϸ����
			m_bGameFinishAndComp = result->bGameFinish;
			// ���ƽ����������ʤ��
			m_iComFinishWinner = result->bWinner;

			PlayXianShiChouMa(result->iNt,result->iCurNote,true);
			UpdateNoteInfo(result->iNt,result->iCurNote);
			DisableAllOperaBtn();
			PlayOpenCard(result->iNt,result->iNoNt,iComRet);

			//�������ʣ��Ľ��
			UpdateMyLeftMoney();
			UpdateAddChipShow(0);
			return true;
		}
	case ASS_NOTE_RESULT:											//Ѻע���
		{
			if (nLen != sizeof(NoteResult)) 
				return FALSE;

			NoteResult *noteresult=(NoteResult*)buffer;

			BYTE bDeskStation = noteresult->iOutPeople;

			if (bDeskStation >= PLAY_COUNT)
			{
				DebugPrintf("receive wrong station %d",bDeskStation);
				return true;
			}
			m_iLimitNote = noteresult->iLimiteNote;


			//������Ч

			UserInfoForGame info;
			bool bBoy = true;
			if (m_pGameFrame->GetUserInfo(noteresult->iOutPeople,info))
			{
				bBoy = info.bBoy;
			}
			SetNoteSound(noteresult->bAddStyle,bBoy);//��ע����

			//����Ƿ���������ʾ����
			if(noteresult->bAddStyle==TYPE_GIVE_UP)
			{
				SetMing(bDeskStation,false);
				//memset(m_UserCard[bDeskStation],0,m_iDeskCardCount[bDeskStation]);
				SetHandCard(bDeskStation,m_iDeskCard[bDeskStation],NULL,m_iDeskCardCount[bDeskStation],0);
				
				BYTE bViewStation = ViewStation(bDeskStation);

				INoMoveCard * pCard1=NULL;
				GETCTRL(INoMoveCard,pCard1,m_pUI,CARD_HAND_0+bViewStation);

				IImage * pImage = NULL;
				GETCTRL(IImage,pImage,m_pUI,IDD_LOSE_CARD_0+bViewStation);
				if (pImage && pCard1)
				{
					pImage->SetGameXY(pCard1->GetGameX(),pCard1->GetGameY());
					pImage->SetControlVisible(true);
					pCard1->SetControlVisible(false);
				}

				m_bIsGiveUp[bDeskStation] = true;  //������

				
			}
			else			//������ע
			{
				//AddThisTurnNotePoint(bDeskStation,noteresult->iCurNote);
				//��Ϊ�����û���ע��,�����Ǽ�ע��,Ҳ�����Ǹ�ע,��ע
				//					AddNotePoint(bDeskStation,noteresult->iCurNote);
				AddTotalNote(bDeskStation,noteresult->iCurNote);
				//����ĳһ������ע�ܶ�
				m_iThisGameNote[noteresult->iOutPeople]+=noteresult->iCurNote;
				m_iTotalGameNote[noteresult->iOutPeople]+=noteresult->iCurNote;


				m_iFrontNote = noteresult->iNote;

				PlayXianShiChouMa(noteresult->iOutPeople,noteresult->iCurNote,true);   //�����³��붯��

				UpdateNoteInfo(noteresult->iOutPeople, noteresult->iCurNote);

			}

			//DebugPrintf(GetMeUserInfo()->bDeskStation,"note_ret peopleseat %d,total%d,last %d,usertotal %d",
			//	noteresult->iOutPeople,m_iNoteTotalPoint,m_iFrontNote,m_iThisGameNote[noteresult->iOutPeople]);


			if (noteresult->bAddStyle == TYPE_REACH_LIMITE_NEXT)
			{
				if (noteresult->bNextDeskStation >= PLAY_COUNT)
				{
					return true;
				}

				SetTipInfo(noteresult->bNextDeskStation,noteresult->bAddStyle);
			}

			else
			{
				SetTipInfo(bDeskStation,noteresult->bAddStyle);
			}

			//�������ʣ��Ľ��
			UpdateMyLeftMoney();
			UpdateAddChipShow(0);
			UpdateRoomInfo();



			//if(m_pGameInfo->bEnableSound)
			//	SetNoteSound(noteresult->bAddStyle,false,this);					//Ѻע����
			KillGameTimer();

			return true;

		}
	case ASS_SALE_END:	//��Ϸ��ȫ����
		{
			if (nLen!=sizeof(GameCutStruct)) 
				return FALSE;
			GameCutStruct * pGameEnd=(GameCutStruct *)buffer;
			KillGameTimer(0);

			return true;
		}
	case ASS_CUT_END:	//�û�ǿ���뿪
		{
			if (nLen!=sizeof(GameCutStruct))
			{
				return FALSE;
			}

			GameCutStruct * pGameEnd=(GameCutStruct *)buffer;

			UserInfoForGame UserInfo;
			
			if (m_pGameFrame->GetUserInfo(pGameEnd->bDeskStation,UserInfo))
			{
				CString str;
				str.Format(TEXT("%s ǿ���ˣ���Ϸ��ǰ������"),UserInfo.szNickName);
				m_pGameFrame->InsertNormalMessage(str.GetBuffer());
			}
			
			KillGameTimer(0);
			ShowControl(IDD_START_BT,true);
			SetStationParameter(GS_WAIT_ARGEE);	

			InitGameUI();
			InitGameData();
			return true;

		}
	case ASS_CONTINUE_END:	//��Ϸ����
	case ASS_NO_CONTINUE_END:
		{
			if (nLen!=sizeof(GameEndStruct)) 
				return FALSE;
			GameEndStruct * pGameEnd=(GameEndStruct *)buffer;
			KillGameTimer(0);
		
			
			// ��ʾ������ҵ���
			for (int i=0; i<PLAY_COUNT; ++i)
			{
				if (pGameEnd->iCardShape[i] > 0 || i == m_pGameFrame->GetMyDeskStation())
					memcpy(m_iUserCard[i] ,pGameEnd->bCard[i], sizeof(pGameEnd->bCard[i])); // Added by zxd 20090928 ��ʱ���·�������ҵ���
				/*ShowControl(IDD_ZJH_LOSE_LOOK_CARD_1 +i, SW_HIDE);
				ShowControl(IDD_ZJH_CARD_CTRL_1 +i, SW_SHOW);*/
			}
			//this->RefreshAllControls();

			//// ֹͣ���ƶ���
		
			CString stime;
			stime.Format("%d",CTime::GetCurrentTime());
			int curtime=atoi(stime);
			CString str;

			GameFinishNotify finishnotify;
			::memset(&finishnotify,0,sizeof(finishnotify));

			BYTE bWinStation = 255;

			for (int i = 0; i < PLAY_COUNT; i ++) 
			{ 
				memset(finishnotify.name[i],0,sizeof(finishnotify.name[i]));//�����������ʼ��lc090122
			}


			
			m_pGameFrame->InsertNormalMessage(TEXT("������Ϸ�����"));
			m_pGameFrame->InsertNormalMessage(TEXT("������������"));
			for (int i=0;i<PLAY_COUNT;i++)
			{
				//m_Logic.Msg("zhtlog:m_iDeskCardCount[%d]=%d",i,m_iDeskCardCount[i]);
				if(m_iDeskCardCount[i] <= 0)
				{
					
					continue;
				}

				//wchar_t	m_wszNiCheng[256];//��Ϸ�б�����ҵ��ǳ� ,lym 2012-5-11��
				//UserInfoForGame UserInfo;
				//if(m_pGameFrame->GetUserInfo(i,UserInfo))
				//{
				//	MultiByteToWideChar(CP_ACP,0,UserInfo.szNickName,-1,m_wszNiCheng,256);
				//}


				ShowControl(IDD_LOSE_CARD_0+ViewStation(i),false);
				ShowControl(CARD_HAND_0+ViewStation(i),true);
				UserInfoForGame UserInfo;

				if (m_pGameFrame->GetUserInfo(i,UserInfo) == FALSE)
				{
					//m_Logic.Msg("zhtlog:GetUserInfo=%d =false",i);
					continue;
				}
				if (pGameEnd->iChangeMoney[i] > 0)
				{
					bWinStation = i;
				}
				SetHandCard(i,m_iUserCard[i],NULL,m_iDeskCardCount[i],0);


				TCHAR szMessage[256];
				CString strUserID;
				if (/*IsUserIDValid(i)||*/(m_pGameFrame->GetMyDeskStation() == i) || m_CbIsShowIdOrPlay )///20091222
				{
					GlbGetFloatString(szMessage, pGameEnd->iChangeMoney[i]);
					//if (SUPER_PLAYER == UserInfo.bGameMaster)
					//{
					//	//GetRandUserID(strUserID,i);
					//	str.Format(TEXT("%s  �÷� %s"), strUserID, szMessage);
					//	wsprintf(finishnotify.name[i], "%s",UserInfo.szNickName /*strUserID*/);
					//}
					//else
					//{
						str.Format(TEXT("%s  �÷� %s"), UserInfo.szNickName, szMessage);
						wsprintf(finishnotify.name[i], "%s", UserInfo.szNickName/*UserInfo.dwUserID*/);
					//}


				}
				else
				{
					GlbGetFloatString(szMessage, pGameEnd->iChangeMoney[i]);
					str.Format(TEXT("���  �÷� %s"), szMessage);
					wsprintf(finishnotify.name[i], "���");
				}

				m_pGameFrame->InsertNormalMessage(str.GetBuffer(MAX_PATH));

				finishnotify.iWardPoint[i]=pGameEnd->iTurePoint[i];

				finishnotify.iMoney[i]=pGameEnd->iChangeMoney[i];//m_pUserInfo[i]->GameUserInfo.dwMoney;
				finishnotify.iCardShape[i]=pGameEnd->iCardShape[i];	
				//finishnotify.dwUserID[i] = m_pUserInfo[i]->GameUserInfo.dwUserID;
			}
		

			//PlayGetWinChouMa(bWinStation);
			if (m_pGameFrame->GetMyDeskStation() == bWinStation)
			{
				SetPlaySound(SOUND_GAME_OVER_WIN);
			}
			else
			{
				SetPlaySound(SOUND_GAME_OVER_LOSE);
			}


			finishnotify.iBasePoint=m_iBaseNote;
			finishnotify.iStyle=1;

			/*if (!m_pGameFrame->GetIsWatching())
			{
				ShowControl(IDD_START_BT,true);
			}*/
	
			ShowResult(finishnotify);
		
			SetStationParameter(GS_WAIT_ARGEE);

			return true;

		}
	case ASS_FINISH_COMPARE://��Ϸ����ǰ�ı��ƹ��� 
		{				
			if (nLen!=sizeof(GameEndStruct)) 
				return FALSE;
			GameEndStruct * pGameEnd=(GameEndStruct *)buffer;
			BYTE bWinStation = 255;				
			for (int i=0;i<PLAY_COUNT;i++)
			{
				if(m_iDeskCardCount[i] != 3)
					continue;
				
					if (pGameEnd->iTurePoint[i] > 0)
					{
						bWinStation = i;
					}
					if(pGameEnd->iUserState[m_pGameFrame->GetMyDeskStation()] && pGameEnd->iUserState[i])
					{
						SetHandCard(i,m_iUserCard[i],NULL,m_iDeskCardCount[i],0);
					}
				
			}
			
			if (bWinStation < PLAY_COUNT)
			{
				PlayGetWinChouMa(bWinStation);

			}		
			KillGameTimer(0);

			return true;
		}
	}
	return TRUE;
}
/// ������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	//������ϢFF
	if(pMessage->_uMessageType == UI_CATOONFINISH)
	{
		switch(pMessage->_uControlID)
		{

		case IDD_MOV_CENTER:
			{

				
				IMoveAction* pMoveAction = NULL;
				GETCTRL(IMoveAction,pMoveAction,m_pUI,IDD_MOV_CENTER);
				pMoveAction->SetControlingCtrlID(0);

				IShCtrl * pShCtrl = NULL;
				GETCTRL(IShCtrl,pShCtrl,m_pUI,IDD_CM_SHCTRL_CENTER);
				if (NULL != pShCtrl)
				{
					pShCtrl->SetShNote(0);
					pShCtrl->SetControlVisible(false);
					
					IExAnimate* pExAnimate=NULL;
					POINT Point1;
					GETCTRL(IExAnimate,pExAnimate,m_pUI,IDD_PLAY_VS_ANIMAL);
					if (NULL != pExAnimate)
					{
						Point1.x  = pExAnimate->GetGameX()+150;
						Point1.y  = pExAnimate->GetGameY()-50;
					}

					pShCtrl->SetGameXY(Point1.x ,Point1.y);

				}

				break;
			}
		case IDD_MOV_CONTROL:   //�ƶ��Ƴ���ؼ�
		case IDD_MOV_CONTROL_1: //�ƶ��Ƴ���ؼ�
		case IDD_MOV_CONTROL_2: //�ƶ��Ƴ���ؼ�
		case IDD_MOV_CONTROL_3: //�ƶ��Ƴ���ؼ�
		case IDD_MOV_CONTROL_4: //�ƶ��Ƴ���ؼ�
		case IDD_MOV_CONTROL_5: //�ƶ��Ƴ���ؼ�
			{

				
				BYTE iView = pMessage->_uControlID-IDD_MOV_CONTROL;

				int iCurNote = 0;
				IShCtrl * pShCtrl = NULL;
				GETCTRL(IShCtrl,pShCtrl,m_pUI,IDD_CM_SHCTRL_PLAY_NOTE+iView);
				if (NULL != pShCtrl)
				{
					iCurNote = pShCtrl->GetShNote();
					pShCtrl->SetShNote(0);
					pShCtrl->SetControlVisible(false);
					
				}
				GETCTRL(IShCtrl,pShCtrl,m_pUI,IDD_CM_SHCTRL_CENTER);
				if (NULL != pShCtrl)
				{
					pShCtrl->SetShNote(iCurNote);
					pShCtrl->SetControlVisible(true);

				}
							
			}
			break;
		case IDD_MOV_VS_CARD_0:
			{
				if (!m_bMovCardStart)//���Ƕ����׶β�����
				{
					IMoveAction* pMoveAction1 = NULL;
					GETCTRL(IMoveAction,pMoveAction1,m_pUI,IDD_MOV_VS_CARD_0);
					IMoveAction* pMoveAction2 = NULL;
					GETCTRL(IMoveAction,pMoveAction2,m_pUI,IDD_MOV_VS_CARD_1);
					pMoveAction1->SetControlingCtrlID(0);
					pMoveAction2->SetControlingCtrlID(0);

					BYTE bViewStation1 = ViewStation(m_bComCardStation[0]);
					BYTE bViewStation2 = ViewStation(m_bComCardStation[1]);

					IImage * pImage = NULL;
					GETCTRL(IImage,pImage,m_pUI,IDD_LOSE_CARD_0+bViewStation1);
					if (pImage)
						pImage->SetGameXY(m_poComCardXY[0].x,m_poComCardXY[0].y);
					pImage = NULL;
					GETCTRL(IImage,pImage,m_pUI,IDD_LOSE_CARD_0+bViewStation2);
					if (pImage)
						pImage->SetGameXY(m_poComCardXY[1].x,m_poComCardXY[1].y);

					OnFinishOpenCard();
					return 0;
				}
				IExAnimate* pExAnimate=NULL;
				GETCTRL(IExAnimate,pExAnimate,m_pUI,IDD_PLAY_VS_ANIMAL);
				if(NULL != pExAnimate)
				{
					pExAnimate->SetExtFilePath(L"resources\\vsAnimal\\vs.bani");
					pExAnimate->LoadFromFile();
					pExAnimate->SetControlVisible(true);
					pExAnimate->SetPlayState(true);
					pExAnimate->SetIsSymmetrical(false);
					pExAnimate->SetIsLoop(false) ; 
					SetPlaySound(SOUND_VSCARD);
				}

				
			}
			break;
		case IDD_MOV_SEND_CARD:
			{
			
				ShowControl(IDD_SEND_CARD,false);

				//m_Logic.Msg("zhtlog:m_bSendCardCount[%d]=%d",m_bSendCardPos,m_bSendCardCount[m_bSendCardPos]);
				if (m_bSendCardCount[m_bSendCardPos] < 3)
				{
					m_bSendCardCount[m_bSendCardPos]++;
					SetHandCard(m_bSendCardPos,m_iDeskCard[m_bSendCardPos],NULL,m_bSendCardCount[m_bSendCardPos],0);

					m_bSendCardPos = GetNextNoteStation(m_bSendCardPos);

				
					
					if (m_bSendCardCount[m_bSendCardPos] < 3)
					{
						PlaySendCard(m_bSendCardPos);
					}
					else//���ƽ���
					{

						for (int i=0; i<PLAY_COUNT; i++)
						{
							SetHandCard(i,m_iDeskCard[i],NULL,m_iDeskCardCount[i],0);
						}

						UserInfoForGame UserInfo;

						ShowControl(IDD_SEND_CARD,false);

						//OutputDebugString("zhtlog:���ƽ���");
						if (m_pGameFrame->GetUserInfo(m_pGameFrame->GetMyDeskStation(),UserInfo))
					 {
						 //m_Logic.Msg("zhtlog:Me=%d = %d %d",m_pGameFrame->GetMyDeskStation(),UserInfo.bGameMaster,UserInfo.dwUserID);
						 //ϵͳ����Ա��ʾ���ҵ���
						 if(UserInfo.bGameMaster==SUPER_PLAYER||true == m_bSuperUser[m_pGameFrame->GetMyDeskStation()])
						 {
							 for (int i=0; i<PLAY_COUNT; i++)
							 {
								 SetHandCard(i,m_iUserCard[i],NULL,m_iDeskCardCount[i],0);
							 }
						 }
					 }

					}
				}
								
			}
			break;
		case IDD_PLAY_VS_ANIMAL:
			{
				//AfxMessageBox("��������");
				ShowControl(IDD_PLAY_VS_ANIMAL,false);
				m_pGameFrame->SetTimer(ID_PLAY_MOVE_VSCARD,500);
				BYTE bViewStation1 = ViewStation(m_bComCardStation[0]);
				BYTE bViewStation2 = ViewStation(m_bComCardStation[1]);
				INoMoveCard * pCard1=NULL;
				GETCTRL(INoMoveCard,pCard1,m_pUI,CARD_HAND_0+bViewStation1);
				INoMoveCard *  pCard2=NULL;
				GETCTRL(INoMoveCard,pCard2,m_pUI,CARD_HAND_0+bViewStation2);

				IExAnimate* pExAnimate=NULL;
				POINT Point1,Point2;
				GETCTRL(IExAnimate,pExAnimate,m_pUI,IDD_PLAY_VS_ANIMAL);
				if (NULL != pExAnimate)
				{
					Point1.x  = pExAnimate->GetGameX();
					Point1.y  = pExAnimate->GetGameY();

					Point2.x  = pExAnimate->GetGameX() + 300;
					Point2.y  = pExAnimate->GetGameY();
				}
				//ǰ����Ǹ���ʤ
				if (m_bComCardStation[0] == m_iComWinStation)
				{
					pCard2->SetControlVisible(false);
					//pCard2->SetGameXY(m_poComCardXY[1].x,m_poComCardXY[1].y);

					IImage * pImage = NULL;
					GETCTRL(IImage,pImage,m_pUI,IDD_LOSE_CARD_0+bViewStation2);
					pImage->SetGameXY(Point2.x,Point2.y);
					ShowControl(IDD_LOSE_CARD_0 + bViewStation2, true);
				}
				else
				{
					pCard1->SetControlVisible(false);
					//pCard1->SetGameXY(m_poComCardXY[0].x,m_poComCardXY[0].y);

					IImage * pImage = NULL;
					GETCTRL(IImage,pImage,m_pUI,IDD_LOSE_CARD_0+bViewStation1);
					pImage->SetGameXY(Point1.x,Point1.y);
					ShowControl(IDD_LOSE_CARD_0 + bViewStation1, true);
					

				}
			}
			break;
		case IDD_CTR_PLAY_CLOCK_0:
		case IDD_CTR_PLAY_CLOCK_1:
		case IDD_CTR_PLAY_CLOCK_2:
		case IDD_CTR_PLAY_CLOCK_3:
		case IDD_CTR_PLAY_CLOCK_4:
		case IDD_CTR_PLAY_CLOCK_5:
			{
				switch(m_iCurrentTimeId)
				{
				case ID_BEGIN_TIME://��ʼ��ʱ��
					{

						if (GetStationParameter() == GS_WAIT_NEXT)
						{
							OnControlHitBegin();
							KillGameTimer(0);
							InitGameData();
							InitGameUI();
						}
						else if (GetStationParameter() <= GS_WAIT_ARGEE)
						{

							KillGameTimer(0);
							InitGameData();
							InitGameUI();
							m_pGameFrame->CloseClient();
						}
						
						
						break;
					}
				case ID_OUT_CARD_TIME://���Ƶȴ�ʱ��
					{

						if (m_iNowOutPeople==m_pGameFrame->GetMyDeskStation())		
						{
							CString str;
							str.Format("��ע�⣬���Ѿ���ʱ��ϵͳ�Զ�Ϊ�������");
							m_pGameFrame->InsertNormalMessage(str.GetBuffer());

							OnGiveUp();
						}
						break;
					}
								
				}
				KillGameTimer(0);
			}
		}
	}


	//����������
	if(pMessage->_uMessageType==UI_LBUTTONUP)
	{

		switch(pMessage->_uControlID)
		{	
		
		case     IDD_START_BT://��ʼ��ť
			
			ShowControl(IDD_START_BT,false);
			OnControlHitBegin();

			break;
		case    CARD_HAND_2:
			{

				///�����϶�����
				IOperateCard * pCard=NULL;
				GETCTRL(IOperateCard,pCard,m_pUI,CARD_HAND_2);
				if (NULL != pCard)
				{
					int iMyDesk = m_pGameFrame->GetMyDeskStation();
					m_iUserCardCount[iMyDesk] = pCard->GetCardList(m_iUserCard[iMyDesk]);
				}

				break;
			}

		case RESULT_CONTINUE:
			{
				OnControlHitBegin();
				InitGameData();
				InitGameUI();
			}
			break;
		case RESULT_LEAVE:
			{
				InitGameData();
				InitGameUI();
				m_pGameFrame->CloseClient();

			}
			break;
		case IDD_SOUND_OPEN_BT:
			{
                m_bSound = FALSE;
				CString s = CBcfFile::GetAppPath ();/////����·��
				DWORD dwCfgFile = ::cfgOpenFile(s + GET_SKIN_FOLDER(szTempStr) +".bcf");

				if(dwCfgFile >=0x10) //�ļ��򿪳ɹ�
				{
					//������������
					::cfgSetValue(dwCfgFile,"Operate","Sound",m_bSound);

					::cfgClose(dwCfgFile);						
				}

				if(m_SoundEngine)
				{
					m_SoundEngine->setAllSoundsPaused(true);
					m_SoundEngine->stopAllSounds();
				}

				if (m_bSound)
				{
					ShowControl(IDD_SOUND_OPEN_BT,true);
					ShowControl(IDD_SOUND_CLOSE_BT,false);
				}
				else
				{
					ShowControl(IDD_SOUND_OPEN_BT,false);
					ShowControl(IDD_SOUND_CLOSE_BT,true);
				}
				break;
			}
		case IDD_SOUND_CLOSE_BT:
			{
				m_bSound = TRUE;
				CString s = CBcfFile::GetAppPath ();/////����·��
							
				DWORD dwCfgFile = ::cfgOpenFile(s + GET_SKIN_FOLDER(szTempStr) +".bcf");

				if(dwCfgFile >=0x10) //�ļ��򿪳ɹ�
				{
					//������������
					::cfgSetValue(dwCfgFile,"Operate","Sound",m_bSound);

					::cfgClose(dwCfgFile);						
				}

				if(m_SoundEngine)
					m_SoundEngine->setAllSoundsPaused(false);
				PlayRandSound(SOUND_BG_1, SOUND_BG_3, true);

				if (m_bSound)
				{
					ShowControl(IDD_SOUND_OPEN_BT,true);
					ShowControl(IDD_SOUND_CLOSE_BT,false);
				}
				else
				{
					ShowControl(IDD_SOUND_OPEN_BT,false);
					ShowControl(IDD_SOUND_CLOSE_BT,true);
				}
				break;
			}
		case IDD_ZJH_BTN_CHIPIN:    //��ע
			return OnChipIn();
		case IDD_ZJH_BTN_FOLLOW:    //��ע
			return OnFllow(); 
		case IDD_ZJH_BTN_ADD:       //��ע
			return OnAdd();
		case IDD_ZJH_BTN_GIVEUP:    //����
			return OnGiveUp();
		case IDD_ZJH_BTN_OPEN:      //����
			return OnOpen();
		case IDD_ZJH_BTN_LOOK:      //����
			return OnLook();
		case IDD_ZJH_BTN_ADD_1:     //��1
			return OnAddCount(1);
		case IDD_ZJH_BTN_ADD_2:     //��2
			return OnAddCount(2);  
		case IDD_ZJH_BTN_ADD_3:     //��3
			return OnAddCount(3);   
		case IDD_ZJH_BTN_ADD_CANCLE: //����ȡ��
			return OnAddCancle();
		case IDD_ZJH_VS_0:	// ��λ��1����ұ���
		case IDD_ZJH_VS_1:	// ��λ��2����ұ���
		case IDD_ZJH_VS_2:	// ȡ������
		case IDD_ZJH_VS_3:	// ��λ��4����ұ���
		case IDD_ZJH_VS_4:	// ��λ��5����ұ���
		case IDD_ZJH_VS_5:	// ��λ��6����ұ���
			return OnCompareCard(pMessage->_uControlID-IDD_ZJH_VS_0);
			////���Է�����
			break;
		case IDD_SET_THING_BTN:
			{
				IRadioButton* pSoundRadioButton = NULL;

				GETCTRL(IRadioButton,pSoundRadioButton,m_pUI,IDD_SET_THING_SOUND);

				if (pSoundRadioButton != NULL)
				{
					pSoundRadioButton->SetIsSelect(m_bSound);
				}
				ShowControl(IDD_SET_THING_DLG,true);
				EnableControl(IDD_SET_THING_OK_BTN,true);
				EnableControl(IDD_SET_THING_CANCEL_BTN,true);
				break;
			}
		case IDD_SET_THING_SOUND:

			

			break;

		case IDD_SET_THING_OK_BTN:
			{

				IRadioButton * pSoundRadioButton = NULL;

				GETCTRL(IRadioButton,pSoundRadioButton,m_pUI,IDD_SET_THING_SOUND);

				if (pSoundRadioButton != NULL)
				{
					m_bSound = pSoundRadioButton->GetIsSelect();
				}

				ShowControl(IDD_SET_THING_DLG,false);
				break;
			}
		case IDD_SET_THING_CANCEL_BTN:
			{

				IRadioButton* pSoundRadioButton = NULL;

				GETCTRL(IRadioButton,pSoundRadioButton,m_pUI,IDD_SET_THING_SOUND);

				if (pSoundRadioButton != NULL)
				{
					pSoundRadioButton->SetIsSelect(m_bSound);
				}

				ShowControl(IDD_SET_THING_DLG,false);
			}
			break;
		case 99999:
			SetCard();
			break;
			////////
		case 12:
			{
				AutoSetBackWH();
				break;
			}
		default:
			break;
		}
	}
	else 
		if(UI_MOUSEENTER == pMessage->_uMessageType)
		{
			//m_pUI->GetIControlbyID(pMessage->_uControlID);
			IButton *pbut = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(pMessage->_uControlID));

			if (NULL !=  pbut)
			{
				SetPlaySound(SOUND_MOVEBUTTON);
			}
			
		}


	return 0;

}
/// 
int CNewGameClient::ResetGameFrame(void)
{
	InitGameUI();
	return 0;
}
/// ��ҽ���
int CNewGameClient::GameUserCome()
{
	/*if (nIndex == m_pGameFrame->GetMyDeskStation())
	{
		InitGameUI();
		InitGameData();
	}*/
	SetUserInfo();
	ShowLogo();

	for(int i = 0 ; i<PLAY_COUNT ; i++)
	{
		UpdateNoteInfo( i);
	}
	return 0;
}
/// ����뿪
int CNewGameClient::GameUserLeft(int nIndex)
{
	OutputDebugString("zhtlog:����뿪");
	SetUserInfo();
	ShowLogo();

	return 0;
}
/// ��Ҷ���
int CNewGameClient::GameUserOffLine(void)
{
	return 0;
}
// �Ƿ�������Ϸ������Ϸʵ��
// �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
bool CNewGameClient::IsPlayingGame()
{
	return true;
}
/// ��ʱ����Ϣ
void CNewGameClient::OnTimer(int nTimerID)
{

	switch(nTimerID)
	{
	
	case ID_TIME_CLOCK:
		{
			m_TimeClockCount--;
		
			if (m_TimeClockCount < 5)
			{
				SetPlaySound(SOUND_WARNING);
			}
			if (m_TimeClockCount<=0)
			{
				m_pGameFrame->KillTimer(ID_TIME_CLOCK);
			}

			break;
		}
	case ID_PLAY_MOVE_VSCARD:
		{
			m_pGameFrame->KillTimer(ID_PLAY_MOVE_VSCARD);
			MoveVSCard();
		}
		break;
	case ID_TIME_LOOKCARD:
		{
	
			if (GetStationParameter() != GS_PLAY_GAME)
			{
				m_pGameFrame->KillTimer(ID_TIME_LOOKCARD);
				break;
			}
            m_bLookCardW+=1;
	

			ICardBase* pCard1 = NULL;
			pCard1 = dynamic_cast<ICardBase *>(m_pUI->GetIControlbyID(CARD_HAND_0+ViewStation(m_iNowOutPeople)));
			if(NULL != pCard1)
			{			
				pCard1->SetCardHSpace(m_bLookCardW);
				pCard1->SetControlVisible(true);
				SetHandCard(m_iNowOutPeople,m_iDeskCard[m_iNowOutPeople],NULL,3,0);
			}

			if (m_bLookCardW >= 40)
			{
				
				SetHandCard(m_iNowOutPeople,m_iUserCard[m_iNowOutPeople],NULL,3,0);
				SetCtrlBtn(m_bFirstGame);
				m_pGameFrame->KillTimer(ID_TIME_LOOKCARD);
				m_bLookCardW = 30;
				pCard1->SetCardHSpace(40);
			}
		}
		break;
	}
}

/// ������Ϸ��״̬
void CNewGameClient::SetStationParameter(BYTE bGameStation)
{
	m_bGameStation = bGameStation;
	if (m_pGameFrame)
		m_pGameFrame->SetStationParameter(bGameStation);
}

/// ��ȡ��Ϸ״̬
BYTE CNewGameClient::GetStationParameter(void)
{
	return m_bGameStation;
}

void CNewGameClient::ShowLogo()
{
	IExAnimate *pExAnimate = NULL ;
	
	for(int i = 0 ; i<PLAY_COUNT ; i++)
	{
		UserInfoForGame UserInfo ; 
		memset(&UserInfo,0,sizeof(UserInfo));
		bool bRet = m_pGameFrame->GetUserInfo(i,UserInfo) ;

		if (bRet)
		{
			BYTE iView = m_pGameFrame->DeskStation2View(i) ; 
			CString strUserPath = "..\\Image\\LogoAnimal\\" ; 
			CString strDir = (iView == 1||iView == 2) ?"right":"left";
			CString strSex = (UserInfo.bBoy)?"boy":"girl";

			CString strName ;
			strName.Format("%s_%d_%s.bani",strSex,UserInfo.bLogoID%4,strDir) ; 
			strUserPath += strName;

			pExAnimate =  dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(PLAYER_LOGO0 + iView));
			if(NULL != pExAnimate)
			{
				pExAnimate->SetExtFilePath(CA2W(strUserPath)) ; 
				pExAnimate->LoadFromFile();
				pExAnimate->SetControlVisible(1);
				pExAnimate->SetPlayState(1);
				pExAnimate->SetIsSymmetrical(false);
				pExAnimate->SetIsLoop(true) ; 
			}
		}
		
	}

	return ; 
}

void CNewGameClient::CloseAllLogo(BYTE bDesk)
{
	if (bDesk >= PLAY_COUNT)
	{
		for (int i=0; i<PLAY_COUNT; i++)
		{
			IExAnimate* pAnimal=dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(PLAYER_LOGO0+i));
			if (pAnimal)
			{
				pAnimal->SetIsLoop(false);
				pAnimal->SetControlVisible(false);

			}
		}
	}
	else
	{
		IExAnimate * pAnimal = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(PLAYER_LOGO0+ViewStation(bDesk)));
		if (pAnimal)
		{
			pAnimal->SetIsLoop(false);
			pAnimal->SetControlVisible(false);
		}
	}

}

int CNewGameClient::ViewStation(int desk)
{
	if (NULL != m_pGameFrame)
	{
		return m_pGameFrame->DeskStation2View(desk);
	}

	return 0;


}



///��ʼ��Ϸ,��ʼ����
bool CNewGameClient::OnControlHitBegin()
{
	if (m_bInLisureRoom)
	{
		MSG_GM_S_ClientInfo ClientInfo;
		ClientInfo.bEnableWatch=false;
		m_pGameFrame->SendGameData(&ClientInfo,sizeof(ClientInfo),MDM_GM_GAME_FRAME,ASS_GM_GAME_INFO,0);
	}
	else
	{
		m_pGameFrame->SendGameData(ASS_GM_AGREE_GAME,NULL,0);

//		m_pGameFrame->PlayWaitGroup();
	}
	
	KillGameTimer(0);

	ShowControl(RESULT_WND, false);
			
	return true;
}
//�й�ֱ�ӵ��ô˺���
BOOL CNewGameClient::UserOutCard()
{

	return true;
}
//��Ϸ�߼���������
void CNewGameClient::InitGameData()
{
//	m_iUpGradePeople = 255;			//ׯ��λ��


	m_iCurrentTimeId = 0;       //��ǰ��ʱ��ID
	m_bAutoCard = FALSE;
	m_bTimeOutCount = 0;
	m_bReachLimitStation = 255;
	m_bMovCardStart = false;
	m_bLookCardW = 36;
	m_bSendCardPos = 255;
	m_bNtPeople = 255;
	m_iNowBigNote = 0;

	m_iThisNoteTimes = 0;			//Ѻע����
	m_iFrontNote     = 0;                //��ǰ��Ч��ע����ǰλ�����Ч��ע��;
	m_iMyAddNote     = 0;                //�Ҽ�ע��ֵ 
	//������Ϣ
	m_iLimitNote     = 0;				//����޶�ע
	 m_iBaseNote     = 0;				//��ע
	m_iGuoDi         = 0;                   //����
	m_iLimitPerNote  = 0;            //ÿע���ƣ�Ϊ��ע�����ƣ�����ʾΪ��ע������

	m_iThisTurnLimit = 0;			//������ע
	SetTotalNote(0,0);
	memset(m_bSendCardCount,0,sizeof(m_bSendCardCount));

	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));			//��������ϵ��˿���
	::memset(m_iUserCard,0,sizeof(m_iUserCard));			//��������ϵ��˿�
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));			//�����˿˵���Ŀ
	::memset(m_iDeskCard,0,sizeof(m_iDeskCard));			//������˿�
	//m_bAddNoteTime = 0;
	memset(m_bMing,0,sizeof(m_bMing));
	memset(m_bIsGiveUp,0,sizeof(m_bIsGiveUp));
	memset(m_bOpenLose,0,sizeof(m_bOpenLose));
	memset(m_bComCardStation,0,sizeof(m_bComCardStation));
	memset(m_poComCardXY,0,sizeof(m_poComCardXY));
	memset(m_iThisGameNote,0,sizeof(m_iThisGameNote));
	memset(m_iTotalGameNote,0,sizeof(m_iTotalGameNote));

	//m_Logic.Msg("zhtlog1:m_bSendCardPos=%d",m_bSendCardPos);



}
//��ϷUI����
void CNewGameClient::InitGameUI()
{

	//����������
	ICardBase *pCard1 = NULL;
	for (int i=0; i<PLAY_COUNT; i++)
	{

		pCard1 = dynamic_cast<ICardBase *>(m_pUI->GetIControlbyID(CARD_HAND_0+i));
		if(NULL != pCard1)
		{			
			pCard1->SetControlVisible(false);

			pCard1->SetCardList(NULL,0);
			pCard1->SetCardHSpace(30);
		}
	
		ShowControl(IDD_PLAY_BG_0+i,false);
		ShowControl(IDD_CTR_PLAY_NT_0+i,false);
		ShowControl(IDD_TIPS_PLAYER_TOTAL_NOTE_0+i,false);
		ShowControl(IDD_TIPS_PLAYER_OPEN_0+i,false);
		ShowControl(IDD_CTR_CLOCK_BG_0+i,false);
		ShowControl(IDD_CTR_PLAY_CLOCK_0+i,false);

		ShowControl(IDD_TIPS_PLAYER_GIVEUP_0+i,false);
		ShowControl(IDD_TIPS_PLAYER_LOOK_0+i,false);
		ShowControl(IDD_TIPS_PLAYER_OPEN_WIN_0+i,false);
		ShowControl(IDD_TIPS_PLAYER_OPEN_LOSE_0+i,false);
		ShowControl(IDD_TIPS_PLAYER_MAX_LIMIT_0+i,false);
		ShowControl(IDD_ZJH_VS_0+i,false);
		ShowControl(IDD_LOSE_CARD_0+i,false);
		ShowControl(CARD_HAND_0+i,true);
	}

	ShowControl(IDD_ZJH_TOTAL_MONEY,false);
	ShowControl(IDD_SEND_CARD,false);
	
	//���������
	IContain* pResult=dynamic_cast<IContain *>(m_pUI->GetIControlbyID(RESULT_WND));

	if (NULL != pResult)
	{
		pResult->SetControlVisible(false);
	}

	DisableAllOperaBtn();
	DisVisibleAllTip();
	SetUserInfo();
	IShCtrl* pShCtrl = NULL;
	GETCTRL(IShCtrl,pShCtrl,m_pUI,IDD_MOV_CONTROL);
	if (NULL != pShCtrl)
	{
		pShCtrl->SetShNote(0);
		pShCtrl->SetControlVisible(false);
	}

	GETCTRL(IShCtrl,pShCtrl,m_pUI,IDD_CM_SHCTRL_CENTER);
	if (NULL != pShCtrl)
	{
		pShCtrl->SetShNote(0);
		pShCtrl->SetControlVisible(false);
	}
	CloseAllLogo();
}

//��������
void CNewGameClient::SetHandCard(BYTE bDesk, BYTE iCardList[], BYTE iUpCard[], int iCardCount, int iUpCardCount)
{
	BYTE bView = ViewStation(bDesk);

	INoMoveCard * pCard=NULL;
	pCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(CARD_HAND_0+bView));
	if (pCard != NULL)
	{
		pCard->SetCardList(iCardList,iCardCount);
		pCard->SetControlVisible(true);
	}

}


/// ��ʾ/���� �ؼ�
/// @param int iControlID �ؼ�ID
/// @param bool bShow  �Ƿ���ʾ
/// @return BOOL �Ƿ����óɹ�
BOOL CNewGameClient::ShowControl(int iControlID, bool bShow)
{
	IBCInterFace *pControl = NULL;

	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(iControlID));

	if (NULL != pControl)
	{
		pControl->SetControlVisible(bShow);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// ���ÿ���/���� �ؼ���ֻ֧�ְ�ť��
/// @param int iControlID �ؼ�ID
/// @param bool bShow  �Ƿ���ʾ
/// @return BOOL �Ƿ����óɹ�
BOOL CNewGameClient::EnableControl(int iControlID, bool bShow)
{
	IButton *pControl = NULL;

	pControl = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iControlID));

	if (NULL != pControl)
	{
		pControl->SetEnable(bShow);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/// ��ѯ��ť����/���� �ؼ���ֻ֧�ְ�ť��
/// @param int iControlID �ؼ�ID
/// @return BOOL 
BOOL CNewGameClient::GetControlEnable(int iControlID)
{
	IButton *pControl = NULL;

	pControl = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iControlID));

	if (NULL != pControl)
	{
		return pControl->GetEnable();
	}
	else
	{
		return FALSE;
	}
}

/// ��ѯ��ť��ʾ/���� �ؼ���ֻ֧�ְ�ť��
/// @param int iControlID �ؼ�ID
/// @return BOOL 
BOOL CNewGameClient::GetControlShow(int iControlID)
{
	IBCInterFace *pControl = NULL;

	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(iControlID));

	if (NULL != pControl)
	{
		return pControl->GetVisible();
	}
	else
	{
		return FALSE;
	}
}

/// FlashWindowEx��գ�
void CNewGameClient::FlashWindow()
{
	HWND hwnd = (m_pUI->GetWindowsHwnd());
	FLASHWINFO info;
	info.cbSize = sizeof(FLASHWINFO);
	info.dwFlags = FLASHW_TRAY|FLASHW_TIMERNOFG;
	info.hwnd = hwnd;
	info.uCount = 100;
	info.dwTimeout = 500;
	FlashWindowEx(&info);
}
/// �趨��Ϸ��ʱ��
/// @param BYTE bDesk ��ǰ��ʾ��ʱ����λ��
/// @param int TimeLen ʱ�䳤��
/// @param int TimerID ��ʱ��ID
BOOL CNewGameClient::SetGameTimer(BYTE bDesk, int TimeLen, int TimerID)
{
	BYTE bView = ViewStation(bDesk);

	//��������ָʾ
	for (int i=0; i<PLAY_COUNT; i++)
	{
		ShowControl(IDD_CTR_PLAY_CLOCK_0+i,false);
		ShowControl(IDD_CTR_CLOCK_BG_0+i,false);

	}
	
	ShowControl(IDD_CTR_CLOCK_BG_0+ViewStation(bDesk),true);
	ITime *pTime = NULL ;
	pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(IDD_CTR_PLAY_CLOCK_0+ViewStation(bDesk)));
	
	if(NULL != pTime)
	{
		pTime->SetShowMaxNum(TimeLen);
		pTime->SetShowMinNum(0);
		pTime->SetPLay(true);
		pTime->SetControlVisible(true);
		
		//��������ѵĻ��������µ�ǰTIMEID���Ա�֤��ʱ����Ӧ
		if (m_pGameFrame->GetMyDeskStation() == bDesk)
		{
			m_iCurrentTimeId = TimerID;
			m_pGameFrame->SetTimer(ID_TIME_CLOCK,1000);
			m_TimeClockCount = TimeLen;
		}

		return TRUE;
	}

	return FALSE;

}

/// ������ʱ��
BOOL CNewGameClient::KillGameTimer(int TimerID)
{

	//��������ָʾ
	for (int i=0; i<PLAY_COUNT; i++)
	{
		ShowControl(IDD_CTR_PLAY_CLOCK_0+i,false);
		ShowControl(IDD_CTR_CLOCK_BG_0+i,false);
	}

	ITime *pTime = NULL ;
	pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(IDD_CTR_PLAY_CLOCK_0));

	m_iCurrentTimeId = 0;
	m_pGameFrame->KillTimer(ID_TIME_CLOCK);
	if(NULL != pTime)
	{
		pTime->SetPLay(false);
		pTime->SetControlVisible(false);

		return TRUE;
	}
	return FALSE;
}

/// ��ʾ �����
void CNewGameClient::ShowResult(GameFinishNotify szResultInfo)
{
  
	wchar_t wszTemp[64];
	IText* pText = NULL;
	IImage* pImage = NULL;

	BYTE nMeStation = m_pGameFrame->GetMyDeskStation();
	UserInfoForGame UserInfo;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_pGameFrame->GetUserInfo(i,UserInfo) )
		{
			bool bMyStation = i==m_pGameFrame->GetMyDeskStation()?true:false;
			GETCTRL(IText,pText,m_pUI,RESULT_NAME + i);
			if (pText)
			{
				pText->SetFontShowStyle(AlignmentCenter);
				if (bMyStation)
				{
					pText->SetFontColor(RGB(0,255,255));
				}
				else
				{
					///pText->SetFontColor(RGB(0,0,0));
					//yl alter 20121024 �޸Ľ����������ʾ��ɫ
					pText->SetFontColor(RGB(0,255,0));
				}

				wchar_t	m_wszNiCheng[256];//��Ϸ�б�����ҵ��ǳ� ,lym 2012-5-11��
				MultiByteToWideChar(CP_ACP,0,UserInfo.szNickName,-1,m_wszNiCheng,256);

				//::swprintf_s(wszTemp,L"%ld",UserInfo.dwUserID);
				::swprintf_s(wszTemp,L"%s ",m_wszNiCheng);

				pText->SetText(wszTemp);
				pText->SetControlVisible(true);
			}
			GETCTRL(IText,pText,m_pUI,RESULT_FEN + i);
			if (pText)
			{
				pText->SetFontShowStyle(AlignmentCenter);
				char szText[32];
				if (bMyStation)
				{
					pText->SetFontColor(RGB(0,255,255));
				}
				else
				{
					///pText->SetFontColor(RGB(0,0,0));
					//yl alter 20121024 �޸Ľ����������ʾ��ɫ
					pText->SetFontColor(RGB(0,255,0));
				}
				static TCHAR * szOrderName[9]=
				{
#ifdef  ZJH_CONTINUE_MORE
					TEXT(""),TEXT("����"),TEXT("����"),TEXT("����"),
					TEXT("��"),TEXT("˳��"),	TEXT("˳��"),TEXT("����"),TEXT("")
#else
					TEXT(""),TEXT("����"),TEXT("����"),TEXT("����"),
					TEXT("˳��"),TEXT("��"),	TEXT("˳��"),TEXT("����"),TEXT("")
#endif
				};
				//::swprintf_s(wszTemp,L"%s",szOrderName[szResultInfo.iCardShape[i]]);
				MultiByteToWideChar(CP_ACP,0,szOrderName[szResultInfo.iCardShape[i]],-1,wszTemp,256);
				pText->SetText(wszTemp);
				pText->SetControlVisible(true);
			}
			GETCTRL(IText,pText,m_pUI,RESULT_MONEY + i);
			if (pText)
			{
				pText->SetFontShowStyle(AlignmentCenter);
				if (bMyStation)
				{
					pText->SetFontColor(RGB(0,255,255));
				}
				else
				{
					///pText->SetFontColor(RGB(0,0,0));
					//yl alter 20121024 �޸Ľ����������ʾ��ɫ
					pText->SetFontColor(RGB(0,255,0));
				}
				char szText[32];
				GlbGetFloatString(szText,szResultInfo.iMoney[i]);
				MultiByteToWideChar(CP_ACP,0,szText,-1,wszTemp,256);
				pText->SetText(wszTemp);
				pText->SetControlVisible(true);
			}

		}
		else
		{
			ShowControl(RESULT_NAME+i,false);
			ShowControl(RESULT_FEN+i,false);
			ShowControl(RESULT_MONEY+i,false);
		}
	}
	if (m_pGameFrame->GetIsWatching())
	{
		EnableControl(RESULT_CONTINUE,false);
	}
	else
	{
		EnableControl(RESULT_CONTINUE,true);
	}
	ShowControl(RESULT_WND,true);
	SetGameTimer(m_pGameFrame->GetMyDeskStation(),m_iThinkTime,ID_BEGIN_TIME);
  
}

//����ׯ��
void CNewGameClient::SetNT(int iNTDesk)
{
    int iViewDesk = ViewStation(iNTDesk);

	for (int i=0; i<PLAY_COUNT; i++)
	{
       if (i == iViewDesk)
	   {
		   ShowControl(IDD_CTR_PLAY_NT_0+i ,true);
	   }
	   else
	   {
		   ShowControl(IDD_CTR_PLAY_NT_0+i ,false);
	   }
	}
}



/// ���������Ϣ
void CNewGameClient::SetUserInfo(int iDesk)
{
	UserInfoForGame UserInfo;
	if (iDesk == -1)
	{
		
		for (int i=0; i<PLAY_COUNT; i++)
		{
			if (m_pGameFrame->GetUserInfo(i,UserInfo))
			{

				wchar_t wszTemp[64];
				IText* pText = NULL;
				GETCTRL(IText,pText,m_pUI,IDD_PLAY_USERID_0+ViewStation(i));
				if (pText)
				{
					//MultiByteToWideChar(CP_ACP,0,UserInfo.szNickName,-1,wszTemp,256);
					if ((m_pGameFrame->GetMyDeskStation() == i) || m_CbIsShowIdOrPlay )
					{
						if (SUPER_PLAYER == UserInfo.bGameMaster)
						{
							CString strUserID;
							GetRandUserID(strUserID,i);
							
							//o::swprintf_s(wszTemp,L"%s",strUserID);

							MultiByteToWideChar(CP_ACP,0,strUserID,-1,wszTemp,256);
						}
						else
						{
							wchar_t	m_wszNiCheng[256];//��Ϸ�б�����ҵ��ǳ� ,lym 2012-5-11��
							MultiByteToWideChar(CP_ACP,0,UserInfo.szNickName,-1,m_wszNiCheng,256);
							::swprintf_s(wszTemp,L"%s ",m_wszNiCheng);

							//::swprintf_s(wszTemp,L"%ld",UserInfo.dwUserID);
						}
					}
					else
					{
						::swprintf_s(wszTemp,L"���");
					}
					
					pText->SetText(wszTemp);
					pText->SetControlVisible(true);
				}
				GETCTRL(IText,pText,m_pUI,IDD_PLAY_MONEY_0+ViewStation(i));
				if (pText)
				{
					CString strLocation = "";//m_pGameFrame->GetIPLocation(UserInfo.dwUserIP);

					// ����ǻ����ˣ�����ʾ���ݿ���ĵ�ַAdd by zxd 20090630
					if (UserInfo.bIsRobot)
					{
//						strLocation = UserInfo.szCity;
						
					}
					else
					{
						//CString szCityName;
						if (SUPER_PLAYER == UserInfo.bGameMaster)
						{		
							//GetRandAddress(strLocation,i);					
							
						}	
						
					}

					
					char szText[32];
					GlbGetFloatString(szText,UserInfo.i64Money);

					//::swprintf_s(wszTemp,L"%s",strLocation);

					MultiByteToWideChar(CP_ACP,0,strLocation,-1,wszTemp,256);
					pText->SetText(wszTemp);
					pText->SetControlVisible(true);
				}
				ShowControl(IDD_PLAY_BG_0+ViewStation(i),true);
			}
			else
			{
				ShowControl(IDD_PLAY_USERID_0+ViewStation(i),false);
				ShowControl(IDD_PLAY_MONEY_0+ViewStation(i),false);
				ShowControl(IDD_PLAY_BG_0+ViewStation(i),false);
			}

		}
		
	}
	else
	{
		int iView = ViewStation(iDesk);
		if (m_pGameFrame->GetUserInfo(iDesk,UserInfo))
		{

			wchar_t wszTemp[64];
			IText* pText = NULL;
			GETCTRL(IText,pText,m_pUI,IDD_PLAY_USERID_0+iView);
			if (pText)
			{
				if ((m_pGameFrame->GetMyDeskStation() == iDesk) || m_CbIsShowIdOrPlay )
				{
					if (SUPER_PLAYER == UserInfo.bGameMaster)
					{
						CString strUserID;
						GetRandUserID(strUserID,iDesk);

						//o::swprintf_s(wszTemp,L"%s",strUserID);

						MultiByteToWideChar(CP_ACP,0,strUserID,-1,wszTemp,256);
					}
					else
					{
						wchar_t	m_wszNiCheng[256];//��Ϸ�б�����ҵ��ǳ� ,lym 2012-5-11��
						MultiByteToWideChar(CP_ACP,0,UserInfo.szNickName,-1,m_wszNiCheng,256);
						::swprintf_s(wszTemp,L"%s ",m_wszNiCheng);

						//::swprintf_s(wszTemp,L"%ld",UserInfo.dwUserID);
					}
				}
				else
				{
					::swprintf_s(wszTemp,L"���");
				}

				
				pText->SetText(wszTemp);
				pText->SetControlVisible(true);
			}
			GETCTRL(IText,pText,m_pUI,IDD_PLAY_MONEY_0+iView);
			if (pText)
			{
				CString strLocation = "";//m_pGameFrame->GetIPLocation(UserInfo.dwUserIP);

				// ����ǻ����ˣ�����ʾ���ݿ���ĵ�ַAdd by zxd 20090630
				if (UserInfo.bIsRobot)
				{
					strLocation = UserInfo.szCity;

				}
				else
				{
					//CString szCityName;
					if (SUPER_PLAYER == UserInfo.bGameMaster)
					{		
						GetRandAddress(strLocation,iDesk);					

					}	

				}


				char szText[32];
				GlbGetFloatString(szText,UserInfo.i64Money);

				//::swprintf_s(wszTemp,L"%s",strLocation);

				MultiByteToWideChar(CP_ACP,0,strLocation,-1,wszTemp,256);
						
				pText->SetText(wszTemp);
				pText->SetControlVisible(true);
			}
			ShowControl(IDD_PLAY_BG_0+iView,true);
		}
	}
}
///  ������ȡ�����ļ�������Ϣ�������
void CNewGameClient::SetCard()
{
	CFileDialog dlg(TRUE, "pai", "*.pai", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"*.pai");
	CString strfilename;
	if ( dlg.DoModal() == IDOK )
	{
		strfilename = dlg.GetPathName();
		CFile  file; 
		if(file.Open(strfilename,CFile::modeRead))  
		{  
			TCHAR sRead[1000]={0};
			file.SeekToBegin();
			file.Read(sRead,sizeof(sRead));

			CString str;
			TCHAR seps[] = _T("*;"); 
			TCHAR* token = _tcstok( (LPTSTR)(LPCTSTR)sRead, seps ); 
			SetCardStruct Msg;
			int iIndex = 0;
			int iNowStation = 0;
			int iCardCount = 0;
			while( token != NULL ) 
			{ 
				iIndex ++;
				if (_tstoi(token) == -1)
				{
					token = _tcstok( NULL, seps );
					continue;
				}
				switch(iIndex)
				{
				case 1:
					{
						Msg.bPlayCount = _tstoi(token);
						token = _tcstok( NULL, seps );
						continue;
					}
					break;
				case 3:
					{
						Msg.bCardCount = _tstoi(token);
						token = _tcstok( NULL, seps );
						continue;
					}
					break;
				case 2://2��˭�ȳ��ƣ�������Ч��ֱ���Թ�
					{
						token = _tcstok( NULL, seps );
						continue;
					}
				}

				if ((iIndex - 4 - iNowStation - (iNowStation*60)) == 0)
				{
					Msg.nPlayCardCount[iNowStation++] = _tstoi(token);
					iCardCount = 0;
					token = _tcstok( NULL, seps );
					continue;
				}

				int Temp = _tstoi(token);
				BYTE bCard = 0;
				
				if (Temp == 53)
				{
					bCard = 0x4F;
				}
				else if (Temp == 54)
				{
					bCard = 0x4E;
				}
				else
				{
					int Shape = (Temp-1) / 13;
					int Point = (Temp%13)-1;
					if (Point == 0)
					{
						Point = 13;
					}
					if (Point == -1)
					{
						Point = 12;
					}
					Shape = abs(Shape-3);
				
					bCard = Shape*16+Point;

				}

				Msg.bPlayCard[iNowStation-1][iCardCount++] = bCard;
				token = _tcstok( NULL, seps );
			}

			m_pGameFrame->SendGameData(&Msg,sizeof(SetCardStruct),MDM_GM_GAME_NOTIFY,ASS_SET_TEST_CARD,0);
					
		}
		file.Close();
	}
}


//�û�Ѻע����(רΪ����׼��)
bool CNewGameClient::SetNoteSound(BYTE bNoteStyle,BOOL bIsBoy)
{

	if (!m_bSound || m_SoundEngine==NULL)
	{
		return false;
	}

	char szFileName[MAX_PATH];
	
	switch(bNoteStyle)
	{
	case TYPE_FOLLOW:
		{
			wsprintf(szFileName, "Sound\\%s\\follow.ogg",bIsBoy?"MAN":"WOM");
			break;
		}
	case TYPE_ADD:
		{
			wsprintf(szFileName, "Sound\\%s\\add.ogg",bIsBoy?"MAN":"WOM");
			break;
		}
	case TYPE_NOTE:
		{
			wsprintf(szFileName, "Sound\\%s\\note.ogg",bIsBoy?"MAN":"WOM");
			break;
		}
	case TYPE_GIVE_UP:
		{
			wsprintf(szFileName, "Sound\\%s\\pass.ogg",bIsBoy?"MAN":"WOM");
			break;
		}	
	case TYPE_OPENCARD: //����
		{
			wsprintf(szFileName, "Sound\\%s\\open.ogg",bIsBoy?"MAN":"WOM");;
			break;
		}
	case TYPE_LOOKCARD: //����
		{
			wsprintf(szFileName, "Sound\\%s\\look.ogg",bIsBoy?"MAN":"WOM");
			break;

		}
	default:
		return false;
		break;

	
	}
 
	m_SoundEngine->play2D(szFileName,false);
	return true;
}

///  ������Ч
void CNewGameClient::SetPlaySound(int iSoundID, BOOL bIsBoy, BOOL bISLoop)
{
  if (!m_bSound || m_SoundEngine==NULL)
  {
	  return;
  }

  char szFileName[MAX_PATH];
  strcpy(szFileName, "Sound\\clock.wav");
  switch(iSoundID)
  {

  case SOUND_WARNING://����
	  {
		  strcpy(szFileName, "Sound\\clock.ogg");
		  break;
	  }

  case  SOUND_GAME_OVER_WIN:             //��Ϸ����ʤ����
	  {

		  strcpy(szFileName, "Sound\\win.ogg");
		  break;
		
	  }
  case  SOUND_GAME_OVER_LOSE:             //��Ϸ����ʤ����
	  {

		  strcpy(szFileName, "Sound\\lose.ogg");
		  break;
	  }
  case  SOUND_START_DEF:             //��ʼ����
	  {

		  strcpy(szFileName, "Sound\\start.ogg");
		  break;
	  }
  case  SOUND_BG_1:				//����1
	  {

		  strcpy(szFileName, "sound\\bg1.ogg");
		  break;
	  }
  case  SOUND_BG_2:				//����2
	  {

		  strcpy(szFileName, "sound\\bg2.ogg");
		  break;
	  }
  case  SOUND_BG_3:				//����3
	  {
		  strcpy(szFileName, "sound\\bg3.ogg");
		  break;
	  }
  case   SOUND_CHOUMAMOVE:              //�����ƶ�

	   strcpy(szFileName, "sound\\choumamove.ogg");
	  break;
  case  SOUND_CLICKBUTTON:              //���°�ť��

	   strcpy(szFileName, "sound\\clickbutton.ogg");
	  break;
  case  SOUND_MOVEBUTTON:               //�ƶ�����ť 

	   strcpy(szFileName, "sound\\movebutton.ogg");
	  break;

  case SOUND_SENDCARD:
 
	  strcpy(szFileName, "sound\\sendcard.ogg");
	  break;


  case SOUND_VSCARD:

	  strcpy(szFileName, "sound\\VS.ogg");
	  break;
  }

  m_SoundEngine->play2D(szFileName,bISLoop);
//  if (m_)
}

//�����������
BOOL CNewGameClient::PlayRandSound(BYTE bStartType, BYTE bEndType, bool isLoop)
{
	
	srand((unsigned)time(NULL));
	BYTE bSoundType = rand()%(bEndType-bStartType+1)+bStartType;
	SetPlaySound(bSoundType, false, isLoop);

	return true;
}


//���÷�����Ϣ
void CNewGameClient::UpdateRoomInfo()
{
	IText* pStatic = NULL;
	GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_GUODI_VALUE);
	TCHAR szMessage[MAX_PATH];

	if (pStatic != NULL)
	{
		wchar_t wszTemp[64];
		GlbGetFloatString(szMessage, m_iGuoDi); 
		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
	}

	GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_MAXLIMIT_VALUE);
	if (pStatic != NULL)
	{
		wchar_t wszTemp[64];
		GlbGetFloatString(szMessage, m_iLimitNote); 
		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
	}
	GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_DIZHU_VALUE);
	if (pStatic != NULL)
	{
		wchar_t wszTemp[64];
		GlbGetFloatString(szMessage, m_iBaseNote); 
		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
	}

	GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_MAX_ZHU_VALUE);
	if (pStatic != NULL)
	{
	
		wchar_t wszTemp[64];
		GlbGetFloatString(szMessage, m_iLimitPerNote * 2); 
		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
			;
	}
}
//��ʼ����ע
void CNewGameClient::SetTotalNote(BYTE iViewStation,int iNoteTotal)
{
	m_iNoteTotalPoint=iNoteTotal;	
	return ;
}
//��ע
void CNewGameClient::AddTotalNote(BYTE iViewStation,int iNoteTotal)
{
	m_iNoteTotalPoint += iNoteTotal;

	return ;
}

void CNewGameClient::SetMing(BYTE bDeskStation,BOOL iGiveup)
{
	m_bMing[bDeskStation]=iGiveup;
	if (iGiveup)
		SetTipInfo(bDeskStation,TYPE_LOOKCARD);
	return ;
}

//�����Լ�����Ϣ
void CNewGameClient::UpDateMyUserInfo()
{
	IText* pStatic = NULL;
	
	TCHAR szMessage[MAX_PATH];	

	UserInfoForGame UserInfo;

	if (m_pGameFrame->GetUserInfo(m_pGameFrame->GetMyDeskStation(),UserInfo) == FALSE)

		return;

	GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_USER_NAME);
	if (NULL != pStatic)
	{
		//wsprintf(szMessage,"%d",UserInfo.dwUserID);
		wchar_t wszTemp[64];

		//MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		MultiByteToWideChar(CP_ACP,0,UserInfo.szNickName,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
	}
	
		

	GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_USER_POINT);
	if (NULL != pStatic)
	{
		GlbGetFloatString(szMessage,UserInfo.i64Money);
		wchar_t wszTemp[64];

		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
	}

	GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_USER_LEFT_MONEY);
	if (NULL != pStatic)
	{
		GlbGetFloatString(szMessage,UserInfo.i64Money);
		wchar_t wszTemp[64];

		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
	}


}
//�����Լ��Ŀ������
void CNewGameClient::UpdateMyLeftMoney()
{
	IText* pStatic = NULL;

	TCHAR szMessage[MAX_PATH];	

	UserInfoForGame UserInfo;

	if (m_pGameFrame->GetUserInfo(m_pGameFrame->GetMyDeskStation(),UserInfo) == FALSE)
	{
		return;
	}

	GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_USER_POINT);
	if (NULL != pStatic)
	{
		GlbGetFloatString(szMessage,UserInfo.i64Money);
		wchar_t wszTemp[64];

		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
	}

	GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_USER_LEFT_MONEY);
	if (NULL != pStatic)
	{
		int temp = m_iLimitNote - m_iTotalGameNote[m_pGameFrame->GetMyDeskStation()];
		if (temp < 0)
			temp = 0;
		GlbGetFloatString(szMessage,temp);
		wchar_t wszTemp[64];
		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);
	}
}
//-----------------------------------��ʾ��ز���--------------------------------
//������ʾ��Ϣ
void CNewGameClient::SetTipInfo(BYTE bDeskStation,int iOperateType)
{
	if (bDeskStation>=PLAY_COUNT)
	{
		return;
	}
	BYTE bViewStation = ViewStation(bDeskStation);

	switch(iOperateType)
	{
	case TYPE_NOTE:
		{
			DisVisibleAllTip(true);
			ShowControl(IDD_TIPS_PLAYER_CHIPIN_0+bViewStation,SW_SHOW);

		}
		break;
	case TYPE_ADD:
		{
			DisVisibleAllTip(true);
			ShowControl(IDD_TIPS_PLAYER_ADD_0+bViewStation,SW_SHOW);

		}
		break;
	case TYPE_FOLLOW:
		{
			DisVisibleAllTip(true);
			ShowControl(IDD_TIPS_PLAYER_FOLLOW_0+bViewStation,SW_SHOW);

		}
		break;
	case TYPE_GIVE_UP:
		{
			DisVisibleAllTip(true);
			ShowControl(IDD_TIPS_PLAYER_LOOK_0+bViewStation,SW_HIDE);  //����ԭ���Ŀ��Ʊ�ʶ
			ShowControl(IDD_TIPS_PLAYER_GIVEUP_0+bViewStation,SW_SHOW);

		}
		break;
	case TYPE_OPEN_LOSE:
		{
			DisVisibleAllTip(true);
			ShowControl(IDD_TIPS_PLAYER_LOOK_0+bViewStation,SW_HIDE);  //����ԭ���Ŀ��Ʊ�ʶ
			ShowControl(IDD_TIPS_PLAYER_GIVEUP_0+bViewStation,SW_HIDE); //����ԭ���ķ�����ʶ
			ShowControl(IDD_TIPS_PLAYER_OPEN_LOSE_0+bViewStation,SW_SHOW);
			ShowControl(IDD_TIPS_PLAYER_CHIPIN_0+bViewStation,SW_HIDE);
			ShowControl(IDD_TIPS_PLAYER_ADD_0+bViewStation,SW_HIDE);
			ShowControl(IDD_TIPS_PLAYER_FOLLOW_0+bViewStation,SW_HIDE);

		}
		break;
	case TYPE_REACH_LIMITE_NEXT:
	case TYPE_REACH_LIMITE_SELF:
		{
			DisVisibleAllTip(true);
			ShowControl(IDD_TIPS_PLAYER_MAX_LIMIT_0+bViewStation,SW_SHOW);
		}
		break;
	case TYPE_LOOKCARD:
		{			
			if (m_bIsGiveUp[bDeskStation] || m_bOpenLose[bDeskStation])
			{
				break;
			}			
			DisVisibleAllTip(true);
			ShowControl(IDD_TIPS_PLAYER_LOOK_0+bViewStation,SW_SHOW);
		}
		break;
	case TYPE_OPEN_WIN:
		{

			DisVisibleAllTip(true);
			ShowControl(IDD_TIPS_PLAYER_OPEN_WIN_0+bViewStation,SW_SHOW);
			ShowControl(IDD_TIPS_PLAYER_LOOK_0+bViewStation,SW_HIDE);  //����ԭ���Ŀ��Ʊ�ʶ
			ShowControl(IDD_TIPS_PLAYER_GIVEUP_0+bViewStation,SW_HIDE); //����ԭ���ķ�����ʶ
			ShowControl(IDD_TIPS_PLAYER_OPEN_LOSE_0+bViewStation,SW_HIDE);

		}
		break;
	default:
		break;
	}

}
void CNewGameClient::DisVisibleAllTip(bool bIsShowGaveUpAndLose)  //�������е���ʾ��Ϣ
{
	if (bIsShowGaveUpAndLose)
	{
		DisVisibleAllAddTip();
		//�������е���ע��ʾ
		DisVisibleAllChipinTip();
		//�������еĸ�ע��ť
		DisVisibleAllFollowTip();
		//�������еķ�����ʾ

		DisVisibleAllLookTip();      //������ʾ
		DisVisibleReachLimiteTip();  //�ﵽ������ʾ

		DisVisibleAllOpenWinTip();

	}
	else
	{
		DisVisibleAllGiveUpTip();    //������ʾ
		DisVisibleAllOpenLoseTip();  //����ʧ����ʾ
		DisVisibleAllAddTip();
		//�������е���ע��ʾ
		DisVisibleAllChipinTip();
		//�������еĸ�ע��ť
		DisVisibleAllFollowTip();
		//�������еķ�����ʾ

		DisVisibleAllLookTip();      //������ʾ
		DisVisibleReachLimiteTip();  //�ﵽ������ʾ

		DisVisibleAllOpenWinTip();
	}


}
//�������еļ�ע��ʾ
void CNewGameClient::DisVisibleAllAddTip()
{
	for (int i=0; i<PLAY_COUNT; i++)
	{
		ShowControl(IDD_TIPS_PLAYER_ADD_0+i,SW_HIDE);
	}

}
//�������е���ע��ʾ
void CNewGameClient::DisVisibleAllChipinTip()
{
	for (int i=0; i<PLAY_COUNT; i++)
	{
		ShowControl(IDD_TIPS_PLAYER_CHIPIN_0+i,SW_HIDE);
	}

}
//�������еĸ�ע��ť
void CNewGameClient::DisVisibleAllFollowTip()
{
	for (int i=0; i<PLAY_COUNT; i++)
	{
		ShowControl(IDD_TIPS_PLAYER_FOLLOW_0+i,SW_HIDE);
	}

}
//�������еķ�����ʾ
void CNewGameClient::DisVisibleAllGiveUpTip()    //������ʾ
{
	for (int i=0; i<PLAY_COUNT; i++)
	{
		ShowControl(IDD_TIPS_PLAYER_GIVEUP_0+i,SW_HIDE);
	}

}
void CNewGameClient::DisVisibleAllOpenLoseTip()  //����ʧ����ʾ
{
	for (int i=0; i<PLAY_COUNT; i++)
	{
		ShowControl(IDD_TIPS_PLAYER_OPEN_LOSE_0+i,SW_HIDE);
	}

}
void CNewGameClient::DisVisibleAllLookTip()      //������ʾ
{
	for (int i=0; i<PLAY_COUNT; i++)
	{
		ShowControl(IDD_TIPS_PLAYER_LOOK_0+i,SW_HIDE);
	}

}
void CNewGameClient::DisVisibleReachLimiteTip()  //�ﵽ������ʾ
{
	for (int i=0; i<PLAY_COUNT; i++)
	{
		ShowControl(IDD_TIPS_PLAYER_MAX_LIMIT_0+i,SW_HIDE);
	}

}
void CNewGameClient::DisVisibleAllOpenWinTip()
{
	for (int i=0; i<PLAY_COUNT; i++)
	{
		ShowControl(IDD_TIPS_PLAYER_OPEN_WIN_0+i,SW_HIDE);
	}

}

//��ʾ�������ע״��
void CNewGameClient::UpdateNoteInfo(BYTE bDeskStation, int nNoteValue)
{
	if (bDeskStation >= PLAY_COUNT)
	{
		return;
	}

	BYTE bViewStation = ViewStation(bDeskStation);//(ViewStation(bDeskStation) + PLAY_COUNT) % (PLAY_COUNT + 1);
	
	IText* pStatic = NULL;

	TCHAR szMessage[MAX_PATH];	

	UserInfoForGame UserInfo;

	CString strInfo ; 
	strInfo.Format("wysoutinfo:: 111���Ӻ� %d" ,bDeskStation); 
	OutputDebugString(strInfo) ; 

	if (!m_pGameFrame->GetUserInfo(bDeskStation,UserInfo))
	{
		return;
	}

	strInfo.Format("wysoutinfo:: 222���Ӻ� %d" ,bDeskStation); 
	OutputDebugString(strInfo) ; 

	GETCTRL(IText,pStatic,m_pUI,IDD_TIPS_PLAYER_TOTAL_NOTE_0+bViewStation);
	if (pStatic != NULL)
	{
		TCHAR szMessage[MAX_PATH];
		TCHAR szTempMessage[256];
		TCHAR szTempMessage2[256];

		int iTempNote = m_iFrontNote;

		// ����Լ���������������עΪ0
		if (m_bIsGiveUp[bDeskStation])
		{
			iTempNote = 0;
		}


		if (m_bMing[bDeskStation])
		{
			iTempNote *= 2;
		}

		if (m_iTotalGameNote[bDeskStation] == 0 && iTempNote == 0)
		{
			memset(szMessage,0,sizeof(szMessage));
		}
		else
		{
			// �������������������������Ϊ׼
			if (nNoteValue>0)
			{
				iTempNote = nNoteValue;
			}
			if (bDeskStation == m_pGameFrame->GetMyDeskStation())
			{
				GlbGetFloatString(szTempMessage, iTempNote);
				GlbGetFloatString(szTempMessage2, m_iTotalGameNote[bDeskStation]); 

				wsprintf(szMessage,"����ע: %s/����ע: %s",szTempMessage,
					szTempMessage2);

			}
			else
			{
				GlbGetFloatString(szTempMessage, iTempNote);
				GlbGetFloatString(szTempMessage2, m_iTotalGameNote[bDeskStation]); 

				wsprintf(szMessage," %s/%s",szTempMessage,
					szTempMessage2);
				OutputDebugString("wysoutinfo:: ffaffffff") ;
			}
		}

		wchar_t wszTemp[64];
		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		pStatic->SetText(wszTemp);
		pStatic->SetControlVisible(true);

		GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_TOTAL_MONEY);
		if (pStatic != NULL)
		{

			//*����*/BzImage* pBk = dynamic_cast<BzImage *>(GetControlById(IDD_GET_POINT_SHOW_BAK));
			//pBk->Invalidate();

			GlbGetFloatString(szMessage, m_iNoteTotalPoint);

			wchar_t wszTemp[64];
			MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
			pStatic->SetText(wszTemp);
			pStatic->SetControlVisible(true);
			//AfxMessageBox("11");
		}
	}
}
//���ŷ��ƶ���
void CNewGameClient::PlaySendCard(BYTE Station)
{
 
	
	INoMoveCard * pCard1=NULL;
	GETCTRL(INoMoveCard,pCard1,m_pUI,IDD_SEND_CARD);
	IMoveAction* pMoveAction = NULL;
	GETCTRL(IMoveAction,pMoveAction,m_pUI,IDD_MOV_SEND_CARD);

	BYTE bViewStation = ViewStation(Station);
	
	POINT poStart, poEnd;
	poStart.x = 300;
	poStart.y = 300;

	INoMoveCard * pCard2=NULL;
	GETCTRL(INoMoveCard,pCard2,m_pUI,CARD_HAND_0+bViewStation);
	if (pCard2 != NULL)
	{
		poEnd.x = pCard2->GetGameX();
		poEnd.y = pCard2->GetGameY();
	}
	//m_Logic.Msg("zhtlog: poEnd.x=%d poEnd.y=%d",poEnd.x,poEnd.y);
	BYTE TempCard = 0;;
	if(pMoveAction!=NULL && pCard1!=NULL)
	{
		pCard1->SetCardList(&TempCard,1);
		pCard1->SetControlVisible(true);
		pMoveAction->SetControlingCtrlID(IDD_SEND_CARD);
		pMoveAction->SetStartP(poStart);
		pMoveAction->SetEndP(poEnd);
		pMoveAction->SetTotalMoveTime(200);
		pMoveAction->SetControlVisible(true);
		pMoveAction->SetPlayState(true);
		SetPlaySound(SOUND_SENDCARD);
		//m_Logic.Msg("zhtlog: ��ʼ����");
	}


	//for(int i=0;i<PLAY_COUNT;i++)
	//{
	//	UserInfoForGame UserInfo;

	//	if (m_pGameFrame->GetUserInfo(i,UserInfo))
	//	{

	//	}
	//}
}
void CNewGameClient::StartPlaySendCard()
{

	for (int i=0; i<PLAY_COUNT; i++)
	{
		SetHandCard(i,m_iDeskCard[i],NULL,0,0);
	}
	if (m_bSendCardPos == 255)
	{
		m_bSendCardPos = m_bNtPeople;
		memset(m_bSendCardCount,0,sizeof(m_bSendCardCount));
	}

	//m_Logic.Msg("zhtlog:m_bSendCardPos=%d m_bNtPeople=%d",m_bSendCardPos,m_bNtPeople);


   PlaySendCard(m_bSendCardPos);
}
int CNewGameClient::GetTotalPlayNum()
{
	UserInfoForGame UserInfo;

	int iCount = 0;
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if (m_pGameFrame->GetUserInfo(i,UserInfo))
		{
			iCount++;
		}
	}
	return iCount;

}
//--------------------------------------------------------------------------------

void CNewGameClient::SetCtrlBtn(bool bIsFirstNote)
{
	
	if (GetStationParameter() != GS_PLAY_GAME)  //û����Ϸ״̬
	{
		DisableAllOperaBtn();
		return;
	}
	else if (m_pGameFrame->GetMyDeskStation() != m_iNowOutPeople)  //��������ע
	{
		DisableAllOperaBtn();
		return;

	}
	bool bCanOpen = false;
	BYTE bMeStation = m_pGameFrame->GetMyDeskStation() ;
	int iPlayNum = GetTotalPlayNum();
	if ( bMeStation < PLAY_COUNT && m_iThisNoteTimes >= iPlayNum)
	{
		int iTempFront = (m_iFrontNote * 2);
		if (m_bMing[bMeStation])
		{
			iTempFront *= 2;
		}
		//int iLimet = GetLimit();
		if (m_iTotalGameNote[bMeStation] + iTempFront <= m_iLimitNote)
		{
			bCanOpen = true;
		}
		else
		{
			bCanOpen = false;
		}		
	}

	//TCHAR szMessage[MAX_PATH];
	//wsprintf(szMessage,"%d,%d,%d",bMeStation,m_iThisNoteTimes,bCanOpen);
	//MessageBox(szMessage);

	if (bIsFirstNote) //�տ�ʼ��Ϸ
	{
		bool bLookCard = true;
		if (m_bMing[bMeStation])
		{
			bLookCard = false;
		}
		UpdateShowNoteBtn(bCanOpen,0,1,0,bLookCard,0); 
		UpdatAddNumBtn(true);
	}
	else  //֮�����ע
	{
		bool bLookCard = true;
		if (m_bMing[bMeStation])
		{
			bLookCard = false;
		}

		UpdateShowNoteBtn(bCanOpen,0,1,1,bLookCard,0);
		UpdatAddNumBtn(false);
	}
}
//���ü�ע������ť����ʾ״̬
bool CNewGameClient::UpdatAddNumBtn(bool bFirst)
{
	if (bFirst)  //��һ�Σ�������X1 X2 X3
	{
		EnableControl(IDD_ZJH_BTN_ADD_1,true);
		EnableControl(IDD_ZJH_BTN_ADD_2,true);
		EnableControl(IDD_ZJH_BTN_ADD_3,true);
		EnableControl(IDD_ZJH_BTN_ADD_CANCLE,true);
// 		m_AiOperateStation.SetNmuBtnEnableCount(3);
		return true;
	}
	else
	{
		int iLimet = GetLimit();
		if (m_iFrontNote + m_iBaseNote * 3 <= iLimet)
		{
			EnableControl(IDD_ZJH_BTN_ADD_1,true);
			EnableControl(IDD_ZJH_BTN_ADD_2,true);
			EnableControl(IDD_ZJH_BTN_ADD_3,true);
			EnableControl(IDD_ZJH_BTN_ADD_CANCLE,true);

		//	m_AiOperateStation.SetNmuBtnEnableCount(3);
		}
		else if (m_iFrontNote + m_iBaseNote * 2 <= iLimet)
		{
			EnableControl(IDD_ZJH_BTN_ADD_1,true);
			EnableControl(IDD_ZJH_BTN_ADD_2,true);
			EnableControl(IDD_ZJH_BTN_ADD_3,false);
			EnableControl(IDD_ZJH_BTN_ADD_CANCLE,true);

		//	m_AiOperateStation.SetNmuBtnEnableCount(2);
		}
		else if (m_iFrontNote + m_iBaseNote <= iLimet)
		{
			EnableControl(IDD_ZJH_BTN_ADD_1,true);
			EnableControl(IDD_ZJH_BTN_ADD_2,false);
			EnableControl(IDD_ZJH_BTN_ADD_3,false);
			EnableControl(IDD_ZJH_BTN_ADD_CANCLE,true);

		//	m_AiOperateStation.SetNmuBtnEnableCount(1);

		}
		else
		{
			EnableControl(IDD_ZJH_BTN_ADD_1,false);
			EnableControl(IDD_ZJH_BTN_ADD_2,false);
			EnableControl(IDD_ZJH_BTN_ADD_3,false);
			EnableControl(IDD_ZJH_BTN_ADD_CANCLE,false);

		//	m_AiOperateStation.SetNmuBtnEnableCount(0);
		}
		return true;

	}

}
//���ð�ť����״̬
void CNewGameClient::UpdateShowNoteBtn(bool bOpen,bool bAdd,bool bGiveUp,bool bFollow,bool bLook,bool bChipIn)
{

	EnableControl(IDD_ZJH_BTN_ADD,bAdd);
	EnableControl(IDD_ZJH_BTN_FOLLOW,bFollow);
	EnableControl(IDD_ZJH_BTN_GIVEUP,bGiveUp);
	EnableControl(IDD_ZJH_BTN_LOOK,bLook);
	EnableControl(IDD_ZJH_BTN_CHIPIN,bChipIn);
	EnableControl(IDD_ZJH_BTN_OPEN,bOpen);

// 	m_AiOperateStation.SetAIStation(bOpen,bAdd,bGiveUp,bFollow,bLook,bChipIn);


	return ;
}
//���а�ť��������
void CNewGameClient::DisableAllOperaBtn()
{
	EnableControl(IDD_ZJH_BTN_ADD,false);
	EnableControl(IDD_ZJH_BTN_FOLLOW,false);
	EnableControl(IDD_ZJH_BTN_GIVEUP,false);
	EnableControl(IDD_ZJH_BTN_LOOK,false);
	EnableControl(IDD_ZJH_BTN_CHIPIN,false);
	EnableControl(IDD_ZJH_BTN_OPEN,false);
	EnableControl(IDD_ZJH_BTN_ADD_1,false);
	EnableControl(IDD_ZJH_BTN_ADD_2,false);
	EnableControl(IDD_ZJH_BTN_ADD_3,false);
	EnableControl(IDD_ZJH_BTN_ADD_CANCLE,false);
	UpdateAddChipShow(0);

	// ���ر��ư�ť
	for (BYTE i=0; i<PLAY_COUNT; ++i)
	{
		ShowControl(IDD_ZJH_VS_0+i, SW_HIDE);
	}
	ShowControl(IDD_ZJH_BIPAI_BACKGROUND, SW_HIDE);
	//m_AiOperateStation.Init();
}

void CNewGameClient::EnableAddOrNote()
{
	if (m_iMyAddNote <= 0)
	{
		EnableControl(IDD_ZJH_BTN_CHIPIN,false);
		EnableControl(IDD_ZJH_BTN_ADD,false);
		return;

	}
	//if (m_bFirstGame )
	//{
	//	EnableControl(IDD_ZJH_BTN_CHIPIN,true);
	//}
	//else
	{
		EnableControl(IDD_ZJH_BTN_ADD,true);
	}

}
//���üӳ�����ʾ�Ĳ���
void CNewGameClient::UpdateAddChipShow(int bMutBase)
{

	IText* pStatic = NULL;

	UserInfoForGame UserInfo;
	GETCTRL(IText,pStatic,m_pUI,IDD_ZJH_TEXT_MONEY);
	if (pStatic != NULL)
	{

		TCHAR szMessage[MAX_PATH];
		GlbGetFloatString(szMessage, bMutBase); 
		wchar_t wszTemp[64];
		MultiByteToWideChar(CP_ACP,0,szMessage,-1,wszTemp,256);
		   ///<�������ģʽ
		pStatic->SetFontShowStyle(FONTSHOWSTYLE::AlignmentFar);
		pStatic->SetText(wszTemp);
		//pStatic->SetMyText(CClientCommonUse::GetIntToFloatStr(szMessage,sizeof(szMessage),bMutBase));
	}
}
//---------------��Ϸ����----------------------------------
bool CNewGameClient::OnChipIn()   //��ע����
{
	if (m_pGameFrame->GetMyDeskStation() != m_iNowOutPeople)
	{
		// ������Ҳ��ǵ�ǰ������ң���ִ��
		return false;
	}
	// {{Add by zxd 20090905 һ��ע��ɱ����ʱ���������η�����ע��Ϣ 
	
	// Add by zxd 20090905 һ��ע��ɱ����ʱ���������η�����ע��Ϣ }}
	if (m_iMyAddNote + m_iFrontNote > GetLimit())
	{
		///m_MsgBox.OpenMessageBox("��ע�������ƣ������²���","",this);
		SetCtrlBtn(true);
		return true;
	}
	KillGameTimer(0);
	tagUserProcess process;
	process.iVrebType = TYPE_NOTE;//ret;				//��ע����
	process.iNote=m_iMyAddNote;						//��ע��(ֻ����ע�ͼ�ע�����ô�ֵ)
	process.bMing=m_bMing[m_iNowOutPeople];
	m_pGameFrame->SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);


	DisableAllOperaBtn();   //�������еĲ�����ť
	return true;
}
bool CNewGameClient::OnAdd()      //��ע����
{
	if (m_pGameFrame->GetMyDeskStation() != m_iNowOutPeople)
	{
		// ������Ҳ��ǵ�ǰ������ң���ִ��
		return false;
	}

	if (m_iMyAddNote <= 0)
	{
		// ��ע���Ϊ0�����ܼ�ע
		return false;
	}

	KillGameTimer(0);

	//�����ע�Ƿ񳬹�����	 
	tagUserProcess process;
	process.iVrebType = TYPE_ADD;//ret;				//��ע����
	if (m_iFrontNote <= 0)
	{
		process.iVrebType = TYPE_NOTE;//ret;				//��ע����
	}
	process.iNote=m_iMyAddNote;						//��ע��(ֻ����ע�ͼ�ע�����ô�ֵ)

	CString str="";
	str.Format("��ע���ԣ����� m_iMyAddNote=%d",m_iMyAddNote);
	OutputDebugString(str);

	process.bMing=m_bMing[m_iNowOutPeople];
	m_pGameFrame->SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);

	DisableAllOperaBtn();    //�������еĲ�����ť
	//UpdateAddChipShow(0);

	// ����
	m_iMyAddNote = 0;
	return true;

}
bool CNewGameClient::OnFllow()    //��ע����
{
	if (m_pGameFrame->GetMyDeskStation() != m_iNowOutPeople)
	{
		// ������Ҳ��ǵ�ǰ������ң���ִ��
		return false;
	}
	if (m_iThisNoteTimes <= 0 || m_iFrontNote <= 0)
	{
		// ��һ�Ѳ��ܸ���ֻ�ܼ�ע�����
		return false;
	}

	KillGameTimer(0);

	tagUserProcess process;
	process.iVrebType = TYPE_FOLLOW;				//��ע����
	process.bMing=m_bMing[m_iNowOutPeople];
	process.iNote = 0;  //����ע��Ϊ0
	//		process.iNote=100;						//��ע��(ֻ����ע�ͼ�ע�����ô�ֵ)
	m_pGameFrame->SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);

	CString str="";
	str.Format("��ע���ԣ����˸��� m_iMyAddNote=%d",m_iMyAddNote);
	OutputDebugString(str);

	DisableAllOperaBtn();     //�������Ĳ�����ť
	return true;
}

bool CNewGameClient::OnGiveUp()  //��������
{
	if (m_pGameFrame->GetMyDeskStation() != m_iNowOutPeople)
	{
		// ������Ҳ��ǵ�ǰ������ң���ִ��
		return false;
	}
	
	KillGameTimer();

	tagUserProcess process;
	process.iVrebType = TYPE_GIVE_UP;				//��ע����
	process.bMing=m_bMing[m_iNowOutPeople];
	process.iNote = 0;  //����ע��Ϊ0	
	m_pGameFrame->SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
	DisableAllOperaBtn();     //�������Ĳ�����ť


	return true;
}
bool CNewGameClient::OnLook()     //���Ʋ���  ��Ϊ������� 
{
	if (m_pGameFrame->GetMyDeskStation() != m_iNowOutPeople)
	{
		// ������Ҳ��ǵ�ǰ������ң���ִ��
		return false;
	}

	//KillGameTimer();
	if(m_bMing[m_iNowOutPeople])
	{ 
		return true;  //���ƹ��ˣ��Ͳ����ٷ�������Ϣ��
	}
	m_bMing[m_iNowOutPeople]=true;
	tagUserProcess process;
	process.iVrebType = TYPE_LOOKCARD;				//��ע����
	process.bMing=m_bMing[m_iNowOutPeople];
	m_pGameFrame->SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);	
	DisableAllOperaBtn();   //����


	return true;
}
bool CNewGameClient::OnOpen()     //���Ʋ���
{
	if (m_pGameFrame->GetMyDeskStation() != m_iNowOutPeople)
	{
		// ������Ҳ��ǵ�ǰ������ң���ִ��
		return false;
	}

	if (m_iThisNoteTimes < GetTotalPlayNum())
	{
		// һȦ��עδ���������ܱ���
		return false;
	}
	KillGameTimer();
	DisableAllOperaBtn();
	AutoSetBackWH();
	ShowControl(IDD_ZJH_BIPAI_BACKGROUND, SW_SHOW);

	for (int i=0; i<PLAY_COUNT; i++)
	{

		// �޴���ң����Ѿ���������֮ǰ�Ѿ�����ʧ�ܣ������id�����Լ�
		if (m_iDeskCardCount[i]!=3 || m_bIsGiveUp[i] || m_bOpenLose[i] || i == m_iNowOutPeople || !m_bPlayer[i])
		{
			ShowControl(IDD_ZJH_VS_0+ViewStation(i), SW_HIDE);
		}
		else
		{
			// ��������λ�����δ�ȹ��Ƶ����洦��ʾVS��ť���ȴ����
			ShowControl(IDD_ZJH_VS_0+ViewStation(i), SW_SHOW);
		
		}
	}
	// ��ʾ���Ȱ�ť
	ShowControl(IDD_ZJH_VS_0+ViewStation(m_iNowOutPeople), SW_SHOW);

	return true;
}
//��עΪ����
bool CNewGameClient::OnAddCount(int bMutBase)
{
	if (m_pGameFrame->GetMyDeskStation() != m_iNowOutPeople)
	{
		// ������Ҳ��ǵ�ǰ������ң���ִ��
		return false;
	}


	// ��ֹ��ע������ע
	if (m_iFrontNote + m_iBaseNote * bMutBase > GetLimit())
	{
		// �����Ϲ���
		return false;
	}

	m_iMyAddNote = bMutBase * m_iBaseNote;	

	if (m_bMing[m_iNowOutPeople])
	{
		UpdateAddChipShow(m_iMyAddNote * 2);
	}
	else
	{
		UpdateAddChipShow(m_iMyAddNote);
	}
	EnableAddOrNote(); //��ע����ʾ��ע����ע��ť
	return true;
}
bool CNewGameClient::OnAddCancle()
{
	if (m_pGameFrame->GetMyDeskStation() != m_iNowOutPeople)
	{
		// ������Ҳ��ǵ�ǰ������ң���ִ��
		return false;
	}

	m_iMyAddNote = 0;
	EnableAddOrNote();
	UpdateAddChipShow(m_iMyAddNote);
	return true;

}

// ������ĳ��ҵı��ư�ť
bool CNewGameClient::OnCompareCard(BYTE bViewStation)
{
	if (m_pGameFrame->GetMyDeskStation() != m_iNowOutPeople)
	{
		// ������Ҳ��ǵ�ǰ������ң���ִ��
		return false;
	}

	// ���ر��ư�ť
	for (BYTE i=0; i<PLAY_COUNT; ++i)
	{
		ShowControl(IDD_ZJH_VS_0+i, SW_HIDE);
	}
	// ������Լ�����ʾ����
	if (3==bViewStation)
	{
		ShowControl(IDD_ZJH_BIPAI_BACKGROUND, SW_HIDE);
		// �ָ�ԭ�а�ť״̬
		SetCtrlBtn(false);
		return true;
	}
	// {{Add by zxd 20090905 һ��ע��ɱ����ʱ���������η�����ע��Ϣ 
	KillGameTimer();
	// Add by zxd 20090905 һ��ע��ɱ����ʱ���������η�����ע��Ϣ }}

	tagCompareCard process;
	process.iVrebType = TYPE_COMPARE_CARD;
	process.byComparedStation = m_pGameFrame->ViewStation2DeskStation(bViewStation);	// ��ͼλ��ת��Ϊ����λ��
	process.bMing=m_bMing[m_iNowOutPeople];
	process.iNote = 0;

	m_pGameFrame->SendGameData(&process,sizeof(process), MDM_GM_GAME_NOTIFY, ASS_VREB_CHECK, 0);
	return true;
}
//�ռ�Ӯ�ĳ��붯��
void CNewGameClient::PlayGetWinChouMa(int nDeskStation)
{

	int iView = ViewStation(nDeskStation);
	IMoveAction* pMoveAction  = NULL;
	GETCTRL(IMoveAction,pMoveAction,m_pUI,IDD_MOV_CENTER);
	IShCtrl* pShCtrl = NULL;
	POINT pointEnd;
	POINT pStart;
	GETCTRL(IShCtrl,pShCtrl,m_pUI,IDD_CM_SHCTRL_CENTER);
	if (NULL != pShCtrl)
	{
		pStart.x = pShCtrl->GetGameX();
		pStart.y = pShCtrl->GetGameY();
	}
		
	IBCInterFace *pControl = NULL;

	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(PLAYER_LOGO0+iView));

	if (NULL != pControl)
	{
		pointEnd.x = pControl->GetGameX();
		pointEnd.y = pControl->GetGameY();;
	}
	/*switch(iView)
	{
	case 0:
		pointEnd.x = 100;
		pointEnd.y = 100;
		break;
	case 1:
		pointEnd.x = 500;
		pointEnd.y = 150;
		break;
	case 2:
		pointEnd.x = 500;
		pointEnd.y = 350;
		break;
	case 3:
		pointEnd.x = 400;
		pointEnd.y = 400;
		break;
	case 4:
		pointEnd.x = 300;
		pointEnd.y = 200;
		break;
	case 5:
		pointEnd.x = 300;
		pointEnd.y = 100;
		break;
	}*/


	if(pMoveAction!=NULL)
	{

		pMoveAction->SetControlingCtrlID(IDD_CM_SHCTRL_CENTER);
		pMoveAction->SetStartP(pStart);
		pMoveAction->SetEndP(pointEnd);
		pMoveAction->SetTotalMoveTime(500);
		pMoveAction->SetControlVisible(true);
		pMoveAction->SetPlayState(true);
		pShCtrl->SetControlVisible(true);
		SetPlaySound(SOUND_CHOUMAMOVE);

	}
	//PlayMusic(MUSIC_MOVE_CHOUMA);
}

//��ע���붯��
void CNewGameClient::PlayXianShiChouMa(int nDeskStation, int iNum,bool bFlag)
{

	int iView = ViewStation(nDeskStation);
	IMoveAction* pMoveAction  = NULL;
	GETCTRL(IMoveAction,pMoveAction,m_pUI,IDD_MOV_CONTROL+iView);
	IShCtrl* pShCtrl = NULL;
	POINT pointEnd;
	GETCTRL(IShCtrl,pShCtrl,m_pUI,IDD_CM_SHCTRL_CENTER);
	if (NULL != pShCtrl)
	{
		pointEnd.x = pShCtrl->GetGameX();
		pointEnd.y = pShCtrl->GetGameY();
	}
	
	POINT pStart;
	switch(iView)
	{
	case 0:
		pStart.x = 100;
		pStart.y = 100;
		break;
	case 1:
		pStart.x = 500;
		pStart.y = 150;
		break;
	case 2:
		pStart.x = 500;
		pStart.y = 350;
		break;
	case 3:
		pStart.x = 400;
		pStart.y = 400;
		break;
	case 4:
		pStart.x = 300;
		pStart.y = 200;
		break;
	case 5:
		pStart.x = 300;
		pStart.y = 100;
		break;
	}
	IBCInterFace *pControl = NULL;

	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(CARD_HAND_0+iView));

	if (NULL != pControl)
	{
		pStart.x = pControl->GetGameX();
		pStart.y = pControl->GetGameY();
	}
	
	GETCTRL(IShCtrl,pShCtrl,m_pUI,IDD_CM_SHCTRL_PLAY_NOTE+iView);
	if (NULL != pShCtrl)
	{
		pShCtrl->SetShNote(0);
		pShCtrl->SetShNote(iNum);
		pShCtrl->SetControlVisible(bFlag);
		pShCtrl->SetGameXY(pStart.x,pStart.y);
	}

	if(pMoveAction!=NULL)
	{
		
		pMoveAction->SetControlingCtrlID(IDD_CM_SHCTRL_PLAY_NOTE+iView);
		pMoveAction->SetStartP(pStart);
		pMoveAction->SetEndP(pointEnd);
		pMoveAction->SetTotalMoveTime(500);
		pMoveAction->SetControlVisible(true);
		pMoveAction->SetPlayState(true);

		SetPlaySound(SOUND_CHOUMAMOVE);
	}
	//PlayMusic(MUSIC_MOVE_CHOUMA);
}


// ���ƶ�������
void CNewGameClient::OnFinishOpenCard()
{
	BOOL Cut = FALSE;
	for ( int i=0; i<PLAY_COUNT; i++  )
	{
		if ( m_byWin[i] == 2 || m_byWin[i] == 1  )
		{
			Cut  = TRUE;
		}
	}

	BYTE bMyDesk = m_pGameFrame->GetMyDeskStation();

	// �ָ�����ʱ�䰵�Ļ���Ϊ��������
	ShowControl(IDD_ZJH_BIPAI_BACKGROUND, SW_HIDE);


	if ( !Cut )
	{
		//{{ Add by ZXD 20090703 ���ƶ�����������Ҫ��ʾ�ƿؼ�

		if (m_bMing[m_iComWinStation])
		{
			if (m_iComWinStation != bMyDesk)
			{
			
				SetTipInfo(m_iComWinStation,TYPE_LOOKCARD);  //���ÿ��Ʊ�ǩ
			}
		}

		SetTipInfo(m_iComLoseStation, TYPE_OPEN_LOSE);  //���ñ����ǩ

	}


	// Add by ZXD 20090703}}

	for (int i=0; i<PLAY_COUNT; i++)
	{
		if (m_iDeskCardCount[i] != 3)
		{
			continue;
		}
	
		if (m_bOpenLose[i])
		{
			if ( !Cut )
			{
				SetTipInfo(i,TYPE_OPEN_LOSE);  //���ÿ��Ʊ�ǩ

			}

		}		
	}


	if (m_bGameFinishAndComp)
	{
		SetTipInfo(m_iComFinishWinner, TYPE_OPEN_WIN);
		if (m_bReachLimitStation < PLAY_COUNT)
		{
			SetTipInfo(m_bReachLimitStation,TYPE_REACH_LIMITE_SELF);   //���˴ﵽ����
		}
		
		PlayGetWinChouMa(m_iComFinishWinner);
	}
	else
	{
		if ( !Cut )
		{
			SetTipInfo(m_iComWinStation,TYPE_OPEN_WIN);
		}

		if (bMyDesk == m_iComFinishWinner)
		{
			//PalyExtAddSound(SOUND_TYPE_WIN);		
		}
		else if (bMyDesk == m_iComLoseStation)
		{
			//PalyExtAddSound(SOUND_TYPE_LOSE);				
		}
	}

	if ( Cut )
	{
		//{{ Add by ZXD 20090703 ���ƶ�����������Ҫ��ʾ�ƿؼ�
		for ( int i=0; i<PLAY_COUNT; i++ )
		{
			if (m_iDeskCardCount[i] != 3)
			{
				continue;
			}
			if ( 1 ==  m_byWin[i] )
			{

				SetTipInfo(i,TYPE_OPEN_LOSE);

			}
			if ( 2 == m_byWin[i] )
			{
				
				SetTipInfo(i,TYPE_OPEN_WIN);
			}


		}
		memset(m_byWin,0,sizeof(m_byWin));
		
	}
	m_iComFinishWinner = 255;
	m_iComWinStation = 255;
	m_bReachLimitStation = 255;
	m_iComLoseStation = 255;
}

UINT CNewGameClient::PlayOpenCard(BYTE bDeskStation1,BYTE bDeskStation2,int iCmpRet)
{

	BYTE bViewStation1 = ViewStation(bDeskStation1);
	BYTE bViewStation2 = ViewStation(bDeskStation2);


	INoMoveCard * pCard1=NULL;
	GETCTRL(INoMoveCard,pCard1,m_pUI,CARD_HAND_0+bViewStation1);
	INoMoveCard * pCard2=NULL;
	GETCTRL(INoMoveCard,pCard2,m_pUI,CARD_HAND_0+bViewStation2);
	if (pCard1 == NULL || pCard2 == NULL)
	{
		return 0;
	}

	CPoint pt1;
	CPoint pt2;
	CPoint ptTemp;
	pt1.x = pCard1->GetGameX();
	pt1.y = pCard1->GetGameY();

	pt2.x = pCard2->GetGameX();
	pt2.y = pCard2->GetGameY();

	bool bChane = false;

	if (pt1.x > pt2.x)   // 12����Ҫ��������
	{
		bChane = true;	   
	}
	else if (pt1.x == pt2.x)  //���������
	{
		int iZOne = 0;  //���
		if (bViewStation1 == 0 || bViewStation1 == 3)
		{
			iZOne = 1;  //�м�
		}
		else if (bViewStation1 == 1 || bViewStation2 == 2)
		{
			iZOne = 2;  //�ұ�
		}
		if (iZOne == 0 || iZOne == 1)
		{
			if (pt1.y > pt2.y)
			{
				bChane = true;				
			}
		}
		else 
		{
			if (pt1.y < pt2.y)
			{
				bChane = true;				
			}
		}
	}
	if (bChane)
	{
		ptTemp = pt1;
		pt1 = pt2;
		pt2 = ptTemp;

		m_bComCardStation[0] = bDeskStation2;
		m_bComCardStation[1] = bDeskStation1;

		//iCmpRet = (!iCmpRet);
	}
	else
	{
		m_bComCardStation[0] = bDeskStation1;
		m_bComCardStation[1] = bDeskStation2;
	}


	 bViewStation1 = ViewStation(m_bComCardStation[0]);
	 bViewStation2 = ViewStation(m_bComCardStation[1]);


	pCard1=NULL;
	GETCTRL(INoMoveCard,pCard1,m_pUI,CARD_HAND_0+bViewStation1);
	pCard2=NULL;
	GETCTRL(INoMoveCard,pCard2,m_pUI,CARD_HAND_0+bViewStation2);

	IMoveAction* pMoveAction1 = NULL;
	GETCTRL(IMoveAction,pMoveAction1,m_pUI,IDD_MOV_VS_CARD_0);
	IMoveAction* pMoveAction2 = NULL;
	GETCTRL(IMoveAction,pMoveAction2,m_pUI,IDD_MOV_VS_CARD_1);
	POINT pStart1,pStart2,pointEnd1,pointEnd2;

	if (NULL != pMoveAction1 && NULL != pMoveAction2)
	{
		pMoveAction1->SetControlingCtrlID(CARD_HAND_0+bViewStation1);
		pMoveAction2->SetControlingCtrlID(CARD_HAND_0+bViewStation2);

		pStart1.x = pCard1->GetGameX();
		pStart1.y = pCard1->GetGameY();
		pointEnd1.x = 180;
		pointEnd1.y = 320;
		pStart2.x = pCard2->GetGameX();
		pStart2.y = pCard2->GetGameY();
		pointEnd2.x = 450;
		pointEnd2.y = 320;

		IExAnimate* pExAnimate=NULL;
		GETCTRL(IExAnimate,pExAnimate,m_pUI,IDD_PLAY_VS_ANIMAL);
		if (NULL != pExAnimate)
		{
           pointEnd1.x  = pExAnimate->GetGameX();
		   pointEnd1.y  = pExAnimate->GetGameY();

		   pointEnd2.x  = pExAnimate->GetGameX() + 300;
		   pointEnd2.y  = pExAnimate->GetGameY();
		}

		m_poComCardXY[0] = pt1;
		m_poComCardXY[1] = pt2;

		pMoveAction1->SetStartP(pStart1);
		
		pMoveAction1->SetEndP(pointEnd1);
		pMoveAction1->SetTotalMoveTime(500);
		pMoveAction1->SetControlVisible(true);
		pMoveAction1->SetPlayState(true);

		pMoveAction2->SetStartP(pStart2);
		
		pMoveAction2->SetEndP(pointEnd2);
		pMoveAction2->SetTotalMoveTime(500);
		pMoveAction2->SetControlVisible(true);
		pMoveAction2->SetPlayState(true);

	}
	m_bMovCardStart = true; //������ƶ����׶�
	
}

////�ƻرȽϵĶ�����
UINT CNewGameClient::MoveVSCard()
{
	if (m_iComWinStation == 255 || m_bComCardStation[0] == 255 || !m_bMovCardStart)
	{
		return 0;

	}
	
	m_bMovCardStart = false;
	BYTE bViewStation1 = ViewStation(m_bComCardStation[0]);
	BYTE bViewStation2 = ViewStation(m_bComCardStation[1]);
	
	IMoveAction* pMoveAction1 = NULL;
	GETCTRL(IMoveAction,pMoveAction1,m_pUI,IDD_MOV_VS_CARD_0);
	IMoveAction* pMoveAction2 = NULL;
	GETCTRL(IMoveAction,pMoveAction2,m_pUI,IDD_MOV_VS_CARD_1);
	POINT pStart1,pStart2,pointEnd1,pointEnd2;

	if (NULL != pMoveAction1 && NULL != pMoveAction2)
	{

		pStart1.x = 300;
		pStart1.y = 350;
		pointEnd1.x = m_poComCardXY[0].x;
		pointEnd1.y = m_poComCardXY[0].y;
		pStart2.x = 600;
		pStart2.y = 350;
		pointEnd2.x = m_poComCardXY[1].x;
		pointEnd2.y = m_poComCardXY[1].y;


		IExAnimate* pExAnimate=NULL;
		GETCTRL(IExAnimate,pExAnimate,m_pUI,IDD_PLAY_VS_ANIMAL);
		if (NULL != pExAnimate)
		{
			pStart1.x  = pExAnimate->GetGameX();
			pStart1.y  = pExAnimate->GetGameY();

			pStart2.x  = pExAnimate->GetGameX() + 300;
			pStart2.y  = pExAnimate->GetGameY();
		}

		
		INoMoveCard * pCard1=NULL;
		GETCTRL(INoMoveCard,pCard1,m_pUI,CARD_HAND_0+bViewStation1);
		INoMoveCard *  pCard2=NULL;
		GETCTRL(INoMoveCard,pCard2,m_pUI,CARD_HAND_0+bViewStation2);

		//ǰ����Ǹ���ʤ
		if (m_bComCardStation[0] == m_iComWinStation)
		{
			/*pCard2->SetControlVisible(false);
			pCard2->SetGameXY(m_poComCardXY[1].x,m_poComCardXY[1].y);
			pMoveAction1->SetControlingCtrlID(CARD_HAND_0+bViewStation1);

			IImage * pImage = NULL;
			GETCTRL(IImage,pImage,m_pUI,IDD_LOSE_CARD_0+bViewStation2);
			pImage->SetGameXY(pStart2.x,pStart2.y);

			ShowControl(IDD_LOSE_CARD_0 + bViewStation2, true);*/

			//pMoveAction1->SetControlingCtrlID(CARD_HAND_0+bViewStation1);
			pMoveAction2->SetControlingCtrlID(IDD_LOSE_CARD_0+bViewStation2);
			
			

		}
		else
		{
			/*pCard1->SetControlVisible(false);
			pCard1->SetGameXY(m_poComCardXY[0].x,m_poComCardXY[0].y);

			pMoveAction2->SetControlingCtrlID(CARD_HAND_0+bViewStation2);

			IImage * pImage = NULL;
			GETCTRL(IImage,pImage,m_pUI,IDD_LOSE_CARD_0+bViewStation1);
			pImage->SetGameXY(pStart1.x,pStart1.y);
			ShowControl(IDD_LOSE_CARD_0 + bViewStation1, true);*/
			//pMoveAction2->SetControlingCtrlID(CARD_HAND_0+bViewStation2);
			pMoveAction1->SetControlingCtrlID(IDD_LOSE_CARD_0+bViewStation1);
			
		}

		pMoveAction1->SetStartP(pStart1);
		
		pMoveAction1->SetEndP(pointEnd1);
		pMoveAction1->SetTotalMoveTime(1000);
		pMoveAction1->SetControlVisible(true);
		pMoveAction1->SetPlayState(true);

		pMoveAction2->SetStartP(pStart2);
		
		pMoveAction2->SetEndP(pointEnd2);
		pMoveAction2->SetTotalMoveTime(1000);
		pMoveAction2->SetControlVisible(true);
		pMoveAction2->SetPlayState(true);

	}
	

}
//��ȡ�����ַ
void CNewGameClient::GetRandAddress(CString &Str,BYTE bDeskStation)
{	
	UserInfoForGame UserInfo;
	if (!m_pGameFrame->GetUserInfo(bDeskStation,UserInfo))
	{
		Str ="";
		return;
	}
	
	if ("" == m_strCityText[bDeskStation])
	{
//		Str = m_pGameFrame->GetIPLocation(UserInfo.dwUserIP);
	}
	else
	{
		Str = m_strCityText[bDeskStation];
	}
	//m_Logic.Msg("zhtlog:%s",Str);
}
//��ȡ����������ID
void CNewGameClient::GetRandUserID(CString &Str,BYTE bDeskStation)
{
	UserInfoForGame UserInfo;
	if (!m_pGameFrame->GetUserInfo(bDeskStation,UserInfo))
	{
		Str ="";
		return;
	}	
	if ("" == m_strCityText[bDeskStation])
	{
		Str.Format("%d",UserInfo.dwUserID);
	}
	else
	{
		Str = m_strUserIDText[bDeskStation];
	}

}

//
void CNewGameClient::AutoSetBackWH()
{
  
  IBCInterFace *pControl = NULL;

  pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(IDD_ZJH_BIPAI_BACKGROUND));

  IBCInterFace *pContro2 = NULL;

  pContro2 = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(1004136));

  int nW = 800;
  int nH = 800;
  if (NULL != pContro2)
  {
        nW = pContro2->GetGameX();
		nH = pContro2->GetGameY()+30;
  }
  pControl->SetGameW(nW);
  pControl->SetGameH(nH);
}