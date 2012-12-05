#include "StdAfx.h"	
#include "ClientGameDlg.h"
#include "GameSet.h"
#include "playcardsound.h"
#include ".\clientgamedlg.h"

static void Msg(const char *lpszFormat, ...)
{
	va_list pArg;
	va_start(pArg, lpszFormat);
	char szMessage[1024] = { 0 };
#ifdef WIN32
	_vsnprintf(szMessage, 1023, lpszFormat, pArg);
#else
	vsnprintf(szMessage, 1023, lpszFormat, pArg);
#endif
	va_end(pArg);
	OutputDebugString(szMessage);
}

BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClassInExe)
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(IDM_BEGIN,OnHitBegin)
	//ON_MESSAGE(IDM_USER_NT,OnHitNt)
	ON_MESSAGE(IDM_LEFT_HITCARD,OnHitCard)
	ON_MESSAGE(IDM_MOUSE_MOVE,OnMouseMove)
	ON_MESSAGE(IDM_MOUSE_LEFT,OnMouseLeft)
	//ON_MESSAGE(IDM_GIVE_BACK,OnHitBackCard)
	//ON_MESSAGE(IDM_OUT_CARD,OnHitOutCard)
	//ON_MESSAGE(IDM_LOOK_LAST,OnHitLastTurn)
	//ON_MESSAGE(IDM_BACK_CARD,OnHitLookBack)
	ON_MESSAGE(IDM_HAVE_THING,OnHitHaveThing)
	ON_MESSAGE(IDM_SEND_THING,OnSendHaveThing)
	ON_MESSAGE(IDM_ARGEE_LEAVE,OnArgeeUserLeft)
	ON_MESSAGE(IDM_CALL_SCORE,OnCallScore)				//�з�
	//ON_MESSAGE(IDM_RIGHT_HITCARD,OnRightHitCard)
	ON_MESSAGE(IDM_STOP,OnStop)
	ON_MESSAGE(IDM_STOP_THING,OnStopThing)			//������ǰ�����¼�
	ON_MESSAGE(IDM_ARGEE_STOP,OnAgreeStop)			//ͬ����ǰ�����¼�
	ON_MESSAGE(IDM_AUTO,OnHitAuto)				//�й�
	//ON_MESSAGE(IDM_PASS,OnHitPass)
	//	ON_MESSAGE(IDM_USER_NT,OnHitNt)
	ON_MESSAGE(IDM_ACTIVE_RESULT,OnHitActiveResult)//�û�������
	ON_MESSAGE(IDM_SHOW_NOTE,OnShowNote)			//�û���ע��ע������
	ON_MESSAGE(IDM_SELECT_NOTE_NUM,OnSelectNote)		//�û�ѡ����ע
	ON_MESSAGE(WM_SENDCARD_SHOWCARD,SendCardShowCard)
	//	ON_MESSAGE(WM_SETMESSAGE,OnSetMessage)
	ON_WM_TIMER()
	//	ON_WM_CLOSE()
END_MESSAGE_MAP()

//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClassInExe(&m_PlayView)
{
	m_iVersion=DEV_HEIGHT_VERSION;
	m_iVersion2=DEV_LOW_VERSION;
	m_iThinkTime=0;
	m_iCardCount=0;
	m_iNowOutPeople=0;
	m_iFirstOutPeople = 0;
	m_PlayView.SetNTStation();
	m_bTimeOutCount=0;
	m_bCurrentOperationStation = 255;//��ǰ�з���
#ifdef SUPER_VERSION
	m_SetCard = NULL;
#endif
	
	//ϵͳ������
	//m_iSysCardCount=28;
	m_iNowBigNote=0;				//��ǰ���Ѻע��
	m_iThisTurnLimit=0;				//������ע
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_iUpBullCard,1,sizeof(m_iUpBullCard));
	::memset(m_UserCard,0,sizeof(m_UserCard));
	m_bBgSound=true;
	m_hMciWnd1=NULL;
	m_hMciWnd2=NULL;
	m_iAfterWaitAction = 0;
	//	WriteProfileInt(TEXT("ShowPicture"),GameSetDlg.m_bRightPower?1:0);
	m_iSendAnimaCardPos = 255 ;
	m_iSendCardTotle = 0 ;
	m_bIsSuper       = FALSE;
	m_bPlayer        = FALSE;

}

//��������
CClientGameDlg::~CClientGameDlg()
{
	if(m_hMciWnd1)
	{
		MCIWndStop(m_hMciWnd1);
		MCIWndDestroy(m_hMciWnd1);
		m_hMciWnd1=NULL;
	}

	if(m_hMciWnd2)
	{
		MCIWndStop(m_hMciWnd2);
		MCIWndDestroy(m_hMciWnd2);
		m_hMciWnd2=NULL;
	}
#ifdef SUPER_VERSION
	if (m_SetCard)
	{
		delete m_SetCard;
		m_SetCard = NULL;
	}
#endif

}

//���ݰ󶨺���
void CClientGameDlg::DoDataExchange(CDataExchange * pDX)
{
	CLoveSendClassInExe::DoDataExchange(pDX);
	DoDataExchangeWebVirFace(pDX);
}

//��ʼ������
BOOL CClientGameDlg::OnInitDialog()
{
	CLoveSendClassInExe::OnInitDialog();
	m_PlayView.m_nPowerOfGold = Glb().m_nPowerOfGold;

	//���±���
	SetDialogIcon(LoadIcon(GetModuleHandle(CLIENT_DLL_NAME),MAKEINTRESOURCE(IDI_CLIENT_ROOM)),TRUE);

	m_PlayView.m_bRightPower=TRUE;//GetProfileInt(TEXT("RightPower"),FALSE);
	m_pGameInfo->bEnableSound=true;

	CString s = CBcfFile::GetAppPath();
	CString fpath;
	fpath = s+SKIN_FOLDER+".bcf";
	if (CBcfFile::IsFileExist(fpath))
	{
		CBcfFile f(fpath);
		m_iAfterWaitAction = f.GetKeyVal(_T("config"), _T("AfterWaitAction"), 2);
		m_PlayView.m_strMoneyType.Format("%s", f.GetKeyVal("config", "moneytype", "���"));
		if (m_iAfterWaitAction != 1 && m_iAfterWaitAction != 2)
		{
			m_iAfterWaitAction = 2;
		}
		m_bAutoBull =  f.GetKeyVal(_T("config"), _T("AutoBull"), 1);
	}

	return TRUE;
}

//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	Msg("Test:Step: %d",GetStationParameter());
	//m_PlayView.SetShape(2, 0);

#ifdef SAO_YANG
	m_PlayView.m_btChangeShin.ShowWindow(SW_SHOW);
	m_PlayView.m_btChangeShin.EnableWindow(TRUE);
#endif

	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:	//�ȴ�ͬ��״̬
		{	
			GameStation_2 * pGameStation=(GameStation_2 *)pStationData;
			m_Logic.SetCardShape(pGameStation->iCardShape);
			//��������
			m_iThinkTime=pGameStation->iThinkTime;
			m_iBeginTime=pGameStation->iBeginTime;
			m_iPalyCardCount=pGameStation->iAllCardCount;
			m_iCallScoreTime = pGameStation->iCallScoreTime;
			m_iXiaZhuTime = pGameStation->iXiaZhuTime;
			m_PlayView.m_GameType = pGameStation->bGameType;
			m_PlayView.m_iGameNoteSetType = pGameStation->iGameNoteSetType;
			//��������
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);	
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�
			m_bIsSuper    = pGameStation->bIsSuper;
		
			//��ʽ���а治�����Թ�
			/*	if(m_bWatchMode&&m_pGameInfo->pMeUserInfo->GameUserInfo.bGameMaster!=MAST_PEOPLE)
			{
			PostMessage(WM_CLOSE,0,0);
			return true;
			}*/
			::CopyMemory(m_iLimitNote,&pGameStation->iLimitNote,sizeof(pGameStation->iLimitNote));	//���ע
			m_iBaseNote=pGameStation->iBaseNote;			//��ע
			m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetShowCard(!m_bWatchMode||m_bWatchOther);//20090414�Թۿ�������zht

			if(!m_bWatchMode)
			{
// 				m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
// 				m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);	
// 				m_PlayView.m_btContinue.SetFocus();
// 				SetGameTimer(GetMeUserInfo()->bDeskStation,m_iThinkTime,ID_BEGIN_TIME);
				if (!IsQueueGameRoom()) 
				{ 
					m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
					if(m_PlayView.m_btLeave.m_hWnd)
						m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					//m_PlayView.m_btContinue.SetFocus();
				
					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
				} 
				else
				{
					ResetGameStation(RS_GAME_BEGIN);
				}
			}

			return TRUE;
		}
	case GS_SEND_CARD:	//����״̬
		{
			GameStation_3 * pGameStation=(GameStation_3 *)pStationData;
			//��������
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);
			m_Logic.SetCardShape(pGameStation->iCardShape);
			m_PlayView.m_btLeave.ShowWindow(SW_HIDE);	
			m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
			m_PlayView.m_GameType = pGameStation->bGameType;
			m_PlayView.m_iGameNoteSetType = pGameStation->iGameNoteSetType;

			//��������
			m_iThinkTime=pGameStation->iThinkTime;
			m_iBeginTime=pGameStation->iBeginTime;
			m_iPalyCardCount=pGameStation->iAllCardCount;
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�
			m_iCallScoreTime = pGameStation->iCallScoreTime;
			m_iXiaZhuTime = pGameStation->iXiaZhuTime;
			::CopyMemory(m_iLimitNote,&pGameStation->iLimitNote,sizeof(pGameStation->iLimitNote));	//���ע
			m_iBaseNote=pGameStation->iBaseNote;			//��ע
			m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
			m_iUpGradePeople = pGameStation->iUpGradePeople;
			m_bIsSuper    = pGameStation->bIsSuper;
			m_bPlayer     = pGameStation->bPlayer;

			//Msg("zhtlog:m_bPlayer=%d",m_bPlayer);
			//			::CopyMemory(m_iDeskCardCount,pGameStation->iUserCardCount,sizeof(pGameStation->iUserCardCount));
			BYTE iView = 2;
#ifndef TWO_NN
			iView = 2;
#else
			iView = 1;
#endif
			for (int i=0;i<PLAY_COUNT;i++)
			{	
				//���ø���������
				int desk=ViewStation(i);
				//��������
				m_iDeskCardCount[desk]=pGameStation->iUserCardCount[i];
				::CopyMemory(m_UserCard[desk],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[desk]);
				if (desk == iView)
					::CopyMemory(m_DeskCard[desk],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[desk]);
				else 
					memset(m_DeskCard[desk],0,sizeof(m_DeskCard[desk]));
				m_PlayView.m_UserCard[desk].SetShowCard(!m_bWatchMode||m_bWatchOther);//20090414�Թۿ�������zht
				m_PlayView.m_UserCard[desk].SetCard(m_DeskCard[desk],NULL,m_iDeskCardCount[desk]);

				//���ø�������Ѻע���
				m_PlayView.SetNotePoint(desk,pGameStation->iTotalNote[i]);
				//ϵͳ����Ա��ʾ���ҵ���
				/*if(m_pGameInfo->pMeUserInfo->GameUserInfo.bGameMaster==MAST_PEOPLE)
					m_PlayView.m_UserCard[desk].SetCard(m_UserCard[desk],NULL,m_iDeskCardCount[desk]);*/
#ifdef SUPER_VERSION
				m_PlayView.m_UserCard[desk].SetCard(m_UserCard[desk],NULL,m_iDeskCardCount[desk]);

#endif
			}
			m_iThisTurnLimit=m_iLimitNote[GetMeUserInfo()->bDeskStation];

			//����Ѻע����
			m_PlayView.SetLimitNote(0,m_iThisTurnLimit,m_iBaseNote);

			Msg("jiaofen2:��1��m_iUpGradePeople = %d", m_iUpGradePeople);

			//����ׯ��
// 			if (m_iUpGradePeople != -1)
// 				m_PlayView.SetNTStation(ViewStation(m_iUpGradePeople));
			
			//����Ѻע����
			m_PlayView.SetLimitNote(0,m_iLimitNote[GetMeUserInfo()->bDeskStation],m_iBaseNote);
			//������Ѻע���
//			m_PlayView.SetTotalNote(0, pGameStation->tag.iTotalNote);
			m_iNowOutPeople=pGameStation->tag.iOutPeople;
			//����ʱ��
			//SetGameTimer(GetMeUserInfo()->bDeskStation,m_iThinkTime,ID_OUT_CARD);
			//�Ƿ�Ϊ��ǰѺע��
			if (!m_bWatchMode&&m_iUpGradePeople!=GetMeUserInfo()->bDeskStation && pGameStation->iGameFlag == GS_NOTE &&m_bPlayer)
			{
				if (pGameStation->iTotalNote[ GetMeUserInfo()->bDeskStation ] <= 0)
				{
					m_PlayView.SetDrawNote(true);
					FlashWindowEx(FLASHW_TIMERNOFG|FLASHW_TRAY, 1, 0);
				}
			}
			if (!m_bWatchMode&&m_iNowOutPeople ==GetMeUserInfo()->bDeskStation && pGameStation->iGameFlag ==GS_FLAG_CALL_SCORE && m_bPlayer)
			{
				m_PlayView.m_btCallScore1.ShowWindow(SW_SHOW);
				m_PlayView.m_btCallScorePass.ShowWindow(SW_SHOW);
				m_PlayView.m_btCallScore1.EnableWindow(TRUE);
				m_PlayView.m_btCallScorePass.EnableWindow(TRUE);
				FlashWindowEx(FLASHW_TIMERNOFG|FLASHW_TRAY, 1, 0);
			}

			if (pGameStation->iGameFlag ==GS_FLAG_CALL_SCORE)
			{
				Msg("Test: Client Runing GS_FLAG_CALL_SCORE");
				SetGameTimer(m_iNowOutPeople,m_iCallScoreTime,ID_CALL_SCORE_TIME);
			}
			else if (pGameStation->iGameFlag == GS_NOTE)
			{
				SetGameTimer(GetMeUserInfo()->bDeskStation,m_iXiaZhuTime,ID_NOTE);
				if (GetMeUserInfo()->bDeskStation == m_iUpGradePeople)
				{
					m_PlayView.SetWait(true);//��ȴ���ͼ
				}
			}

			return TRUE;
		}
	case GS_PLAY_GAME:	//��Ϸ������
		{
			GameStation_4 * pGameStation=(GameStation_4 *)pStationData;
			m_PlayView.m_GameType = pGameStation->bGameType;
			m_PlayView.m_iGameNoteSetType = pGameStation->iGameNoteSetType;
			//��������
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);
			m_Logic.SetCardShape(pGameStation->iCardShape);
			m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
			m_PlayView.m_btLeave.ShowWindow(SW_HIDE);	
			m_bIsSuper    = pGameStation->bIsSuper;
			m_bPlayer     = pGameStation->bPlayer;

			BYTE iView = 2;
#ifndef TWO_NN
			iView = 2;
#else
			iView = 1;
#endif
			//��������
			m_iThinkTime=pGameStation->iThinkTime;
			m_iBeginTime=pGameStation->iBeginTime;
			m_iPalyCardCount=pGameStation->iAllCardCount;
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�
			::CopyMemory(m_iLimitNote,&pGameStation->iLimitNote,sizeof(pGameStation->iLimitNote));	//���ע
			m_iBaseNote=pGameStation->iBaseNote;			//��ע
			m_PlayView.m_btContinue.ShowWindow(SW_HIDE);

			m_iXiaZhuTime = pGameStation->iXiaZhuTime;
			m_iCallScoreTime = pGameStation->iCallScoreTime;
			m_iNowOutPeople=pGameStation->iOutCardPeople;
			m_iFirstOutPeople=pGameStation->iFirstOutPeople;
			m_iUpGradePeople = pGameStation->iUpGradePeople;
			//���浱ǰ�û���ע�ܶ�
			::CopyMemory(m_iThisGameNote,pGameStation->iTotalNote,sizeof(pGameStation->iTotalNote));
			::CopyMemory(m_iThisGameNote,pGameStation->iThisGameNote,sizeof(pGameStation->iThisGameNote));
			::CopyMemory(m_iTotalGameNote,pGameStation->iTotalGameNote,sizeof(pGameStation->iTotalGameNote));
			m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetShowCard(!m_bWatchMode||m_bWatchOther);//20090414�Թۿ�������zht
			
			for (int i=0;i<PLAY_COUNT;i++)
			{	
				//���ø���������
				int desk=ViewStation(i);
				//��������
				m_iDeskCardCount[desk]=pGameStation->iUserCardCount[i];
				::CopyMemory(m_UserCard[desk],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[desk]);
				if (desk == iView)
					::CopyMemory(m_DeskCard[desk],pGameStation->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[desk]);
				else 
					memset(m_DeskCard[desk],0,sizeof(m_DeskCard[desk]));
				m_PlayView.m_UserCard[desk].SetCard(m_DeskCard[desk],NULL,m_iDeskCardCount[desk]);
				//���ø�������Ѻע���
				m_PlayView.SetNotePoint(desk,pGameStation->iTotalNote[i]);
				m_PlayView.m_bOpenCard[desk] = pGameStation->iUserStation[i];

				//ϵͳ����Ա��ʾ���ҵ���
				/*if(m_pGameInfo->pMeUserInfo->GameUserInfo.bGameMaster==MAST_PEOPLE)
					m_PlayView.m_UserCard[desk].SetCard(m_UserCard[desk],NULL,m_iDeskCardCount[desk]);*/
#ifdef SUPER_VERSION
				m_PlayView.m_UserCard[desk].SetCard(m_UserCard[desk],NULL,m_iDeskCardCount[desk]);

#endif
			}

			m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetUseMouse(!m_bWatchMode);

			m_iThisTurnLimit=m_iLimitNote[GetMeUserInfo()->bDeskStation];

			//����ׯ��
			if (m_iUpGradePeople != -1)
				m_PlayView.SetNTStation(ViewStation(m_iUpGradePeople));

			//����Ѻע����
			m_PlayView.SetLimitNote(0,m_iLimitNote[GetMeUserInfo()->bDeskStation],m_iBaseNote);
			m_iNowBigNote=pGameStation->tag.iNowBigNote;
			//������Ѻע���
//			m_PlayView.SetTotalNote(0, pGameStation->tag.iTotalNote);
				
			//����ʱ��
			if (m_iNowOutPeople != -1) 
				SetGameTimer(GetMeUserInfo()->bDeskStation,m_iThinkTime,ID_OUT_CARD);
			//�Ƿ�Ϊ��ǰѺע��
			if (!m_bWatchMode && pGameStation->iUserStation[GetMeUserInfo()->bDeskStation] != TYPE_GIVE_UP&&m_bPlayer)
			{	
				//if (!m_PlayView.m_bRightPower)
				m_PlayView.m_btBull.ShowWindow(SW_SHOW);
				m_PlayView.m_btOpen.ShowWindow(SW_SHOW);
				m_PlayView.m_btReset.ShowWindow(SW_SHOW);
				m_PlayView.m_btBull.EnableWindow(TRUE);
				m_PlayView.m_btOpen.EnableWindow(TRUE);
				m_PlayView.m_btReset.EnableWindow(TRUE);//С��21�����Ҫ��

				FlashWindowEx(FLASHW_TIMERNOFG|FLASHW_TRAY, 1, 0);

			}

			// �Ѿ�������
			if (pGameStation->bSplit)
			{
				KillGameTimer(ID_OUT_CARD);
				m_PlayView.m_btBull.ShowWindow(SW_HIDE);
				m_PlayView.m_btOpen.ShowWindow(SW_HIDE);

				BYTE card[10];
				::CopyMemory(m_iUpBullCard[iView], pGameStation->bBullCard, sizeof(m_iUpBullCard[iView]));
				::CopyMemory(card,&m_UserCard[iView],sizeof(m_UserCard[iView]));
				m_Logic.RemoveCard(m_iUpBullCard[iView], 3, card, 5);
				if (m_Logic.GetShape(m_UserCard[iView], 5, m_iUpBullCard[iView]) > UG_NO_POINT)
				{
				m_PlayView.m_UserCard[iView].SetCard(m_iUpBullCard[iView], NULL, 3);
				m_PlayView.m_UserBull[iView].SetCard(card, NULL, 2);
				}
				else
				{
					m_PlayView.m_UserCard[iView].SetCard(m_UserCard[iView], NULL,5);
					m_PlayView.m_UserBull[iView].SetCard(NULL, NULL, 0);
				}
				m_PlayView.m_UserCard[iView].SetUseMouse();
				m_PlayView.m_UserBull[iView].SetShowCard(!m_bWatchMode||m_bWatchOther);
				m_PlayView.m_UserCard[iView].SetShowCard(!m_bWatchMode||m_bWatchOther);


				//FlashWindowEx(FLASHW_TIMERNOFG|FLASHW_TRAY, 1, 0);
			}

			return TRUE;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
		{
			GameStation_5 * pGameStation=(GameStation_5 *)pStationData;
			//��������
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);
			m_Logic.SetCardShape(pGameStation->iCardShape);
			m_PlayView.m_GameType = pGameStation->bGameType;
			m_PlayView.m_iGameNoteSetType = pGameStation->iGameNoteSetType;
			//��������
			m_iThinkTime=pGameStation->iThinkTime;
			m_iBeginTime=pGameStation->iBeginTime;
			m_iPalyCardCount=pGameStation->iAllCardCount;
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;		//���ܿ۷�
			m_iCallScoreTime = pGameStation->iCallScoreTime;
			m_iXiaZhuTime = pGameStation->iXiaZhuTime;
			m_iUpGradePeople = pGameStation->iUpGradePeople;
			m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetShowCard(!m_bWatchMode||m_bWatchOther);//20090414�Թۿ�������zht
			
			m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
			m_PlayView.m_btLeave.ShowWindow(SW_HIDE);
			m_bIsSuper    = pGameStation->bIsSuper;

			//��ʾ��ʼ��ť 
			if (GetMeUserInfo()->bUserState != USER_ARGEE)
			{
				if (m_bWatchMode == false)
				{
// 					m_PlayView.m_btContinue.ShowWindow(SW_SHOW);
// 					m_PlayView.m_btLeave.ShowWindow(SW_SHOW);
// 					m_PlayView.m_btContinue.SetFocus();
					if (!IsQueueGameRoom()) 
					{ 
						m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
						if(m_PlayView.m_btLeave.m_hWnd)
							m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
						m_PlayView.m_btContinue.SetFocus();
						SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
					} 
					else
					{
						ResetGameStation(RS_GAME_BEGIN);
					}
				}

				 
// 				SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
			}
			return TRUE;
		}
	}
	return false;
}

//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/)
{
	if (pNetHead->bMainID==MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{
			//�Q����
		case ASS_GM_SUPERCLIENT:
			{
				if (uDataSize!=sizeof(ChangeBackStruct))
					return FALSE;
				ChangeBackStruct * pBack=(ChangeBackStruct *)pNetData;

				return ChangeBackCardResult(ViewStation(pBack->bDeskStation ),pBack->bSourceCard ,pBack->bResultCard,pBack->bExtVal);				
			}
		case ASS_UG_USER_SET:		//�û������ƾ�
			{
				/*			if (uDataSize!=sizeof(UserArgeeGame))
				return FALSE;
				SetStationParameter(GS_WAIT_ARGEE);
				UserArgeeGame * pGameSet=(UserArgeeGame *)pNetData;

				//��������
				m_iThinkTime=pGameSet->iPlayCount=30;
				m_iBeginStation=pGameSet->iBeginStation;
				m_iEndStation=pGameSet->iEndStation;
				m_iPalyCardCount=pGameSet->iCardCount;

				//��ʾ����
				m_PlayView.m_StationView.SetShowMessage(TRUE);

				m_PlayView.m_StationView.SetPlayInfo(1,m_iThinkTime);
				if ((m_bWatchMode==FALSE)&&(GetMeUserInfo()->bDeskStation!=0))
				{
				m_PlayView.m_GameNoteView.SetMessage(NULL);
				m_PlayView.m_GameInfo.SetGameSetInfo(m_iPalyCardCount,m_iThinkTime,m_iBeginStation,m_iEndStation);
				}
				else m_PlayView.m_GameNoteView.SetMessage(TEXT("���ڵȴ�������ҿ�ʼ��Ϸ"));
				*/					
				return TRUE;
			}
		case ASS_BEGIN_UPGRADE:		//��Ϸ��ʼ
			{
				if (uDataSize!=sizeof(BeginUpgradeStruct))
					return FALSE;

				m_PlayView.m_GameNoteView.SetMessage(NULL);
				//���ÿ�ʼ������ʼλ��
				//������Ϸ״̬
				SetStationParameter(GS_SEND_CARD);
				m_bTimeOutCount=0;	
				
				BeginUpgradeStruct * pBegin=(BeginUpgradeStruct *)pNetData;
				::CopyMemory(m_iLimitNote,&pBegin->iLimitNote,sizeof(pBegin->iLimitNote));	//���ע
				m_iBaseNote = pBegin->iBaseNote;
				m_iUpGradePeople = pBegin->iNt;                    //ׯ��
				m_iThisTurnLimit=m_iLimitNote[GetMeUserInfo()->bDeskStation];

				//m_bPlayer        = TRUE;
				m_PlayView.SetNTStation(ViewStation(m_iUpGradePeople));
				memset(m_PlayView.m_bOpenCard,false, sizeof(m_PlayView.m_bOpenCard));

				if (GetMeUserInfo()->bDeskStation == m_iUpGradePeople || m_bWatchMode || !m_bPlayer)
				{
					m_PlayView.SetWait(true);//��ȴ���ͼ
					if (m_bPlayer)
						SetGameTimer(GetMeUserInfo()->bDeskStation,m_iXiaZhuTime,ID_NOTE );
				}
				else 
				{
					m_PlayView.m_btBull.EnableWindow(FALSE);
					m_PlayView.m_btOpen.EnableWindow(FALSE);
					m_PlayView.m_btReset.EnableWindow(FALSE);
//					m_PlayView.m_btSplit.EnableWindow(FALSE);

					m_PlayView.SetDrawNote(true);
					//�м�30����עʱ��
					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iXiaZhuTime,ID_NOTE);
					FlashWindowEx(FLASHW_TIMERNOFG|FLASHW_TRAY, 1, 0);
				}
				if (m_bWatchMode==TRUE) 
				{
					KillAllTimer();
					
					ResetGameStation(RS_GAME_BEGIN);
				}
				m_PlayView.SetLimitNote(-1,m_iLimitNote[GetMeUserInfo()->bDeskStation],m_iBaseNote);

				return TRUE;
			}
		case ASS_SEND_CARD:			//����������
			{
				if(uDataSize != sizeof(SendAllCardStruct))
				{
					return FALSE;
				}
				SendAllCardStruct *pAllCard = (SendAllCardStruct *)pNetData ;

				for(int i = 0 ; i < PLAY_COUNT; i++)
				{
					BYTE bDesk = ViewStation(i);

					m_iDeskCardCount[bDesk] = pAllCard->iUserCardCount[i];	
			
					memcpy(&m_UserCard[bDesk],&pAllCard->iUserCard[i],sizeof(BYTE)*m_iDeskCardCount[bDesk]);

				}

			
				m_iSendCardTotle = 0;
				m_iSendAnimaCardPos = pAllCard->iStartPos;
				
				m_PlayView.SendCard(ViewStation(m_iSendAnimaCardPos),150);		

				SetTimer(ID_SEND_CARD_ANIMAL+ViewStation(m_iSendAnimaCardPos),150,NULL);

				if(m_pGameInfo->bEnableSound)
				{
					SetPlaySound(SOUND_SEND_CARD);
				}

				return TRUE;


			}
		case ASS_SEND_FINISH:		//�������
			{				
				KillAllTimer();

				m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetUseMouse(!m_bWatchMode);
				BYTE bDesk = ViewStation(GetMeUserInfo()->bDeskStation);

				memcpy(&m_DeskCard,&m_UserCard,sizeof(m_UserCard));

				m_Logic.SortCard(m_DeskCard[bDesk],NULL,m_iDeskCardCount[bDesk]);
				m_PlayView.m_UserCard[bDesk].SetCard(m_DeskCard[bDesk],NULL,m_iDeskCardCount[bDesk]);
#ifdef SUPER_VERSION
				m_PlayView.m_UserCard[bDesk].SetCard(m_UserCard[bDesk],NULL,m_iDeskCardCount[bDesk]);
#endif
				

				if (m_bWatchMode)
					m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetShowCard(m_bWatchOther);				
		
				for(int i = 0; i < PLAY_COUNT; i++)
				{
					m_PlayView.m_SendCardAnimal[i].ShowWindow(SW_HIDE);
				}
				return TRUE;
			}
		case ASS_COLLECT_JETON://�ռ�����
			{
				if (uDataSize != sizeof(CollectJetonStruct)) 
					return FALSE;	

				CollectJetonStruct * pCollectJeton=(CollectJetonStruct *)pNetData;

				BYTE desk=ViewStation(pCollectJeton->bDeskStation);
				//Ŀǰ��ʾ����
				m_PlayView.SetCollectJetonStart();//�����ռ�������ʼλ��
				m_bShowAnimalStation=desk;						//���õ�ǰ����λ��
				m_iNowBigNote=0;

				////��݆��ע����
				//for(int i=0;i<PLAY_COUNT;i++)
				//	m_PlayView.SetThisTurnNotePoint(i,0);
				//������ʼ����
				m_PlayView.SetCollectJetonStart();
				m_PlayView.CollectJetonAnimal();					//��ʼ���ж���	
				SetTimer(ID_COLLECT_JETON_ANIMAL,50,0);

				return TRUE;
			}
		case ASS_SEND_A_CARD:
			{
				if (uDataSize != sizeof(SendCardStruct)) 
					return FALSE;	
				SendCardStruct * pSendCard=(SendCardStruct *)pNetData;
				int desk=ViewStation(pSendCard->bDeskStatin);
				m_DeskCard[desk][m_iDeskCardCount[desk]]=pSendCard->iCard;			//�û������ϵ���
				if (desk == 2) //�����ƾ���ʾ
					m_UserCard[desk][m_iDeskCardCount[desk]++]=pSendCard->iCard;		//�û�������
				else
					m_UserCard[desk][m_iDeskCardCount[desk]++]=0x00;		//�û�������
				m_PlayView.m_UserCard[desk].SetCard(m_DeskCard[desk],NULL,m_iDeskCardCount[desk]);//��ʾ����������
				
			
				BYTE Rev[1];
				Rev[0]=pSendCard->iCard;
				/*for(int i=0;i<PLAY_COUNT;i++)
				m_PlayView.SetThisTurnNotePoint(i,0);*/
				//��Ϸ��ʼ��Ч����
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_SEND_CARD);
				return true;
			}

		case ASS_GAME_PLAY:		//��ʼ��Ϸ
			{
				if (uDataSize!=sizeof(BeginPlayStruct)) 
					return FALSE;

				//������Ϸ״̬
				SetStationParameter(GS_PLAY_GAME);
				m_PlayView.m_GameNoteView.SetMessage(NULL);

				BeginPlayStruct * pBeginInfo=(BeginPlayStruct *)pNetData;

				m_bTimeOutCount=0;					//��ʱ��������

				m_iNowOutPeople=m_iFirstOutPeople=pBeginInfo->iOutDeskStation;
				m_iNowBigNote=0;							//��ǰ���Ѻע��
				m_iThisTurnLimit=m_iLimitNote[GetMeUserInfo()->bDeskStation];//m_iLimitNote/32*(1<<1)/m_iBaseNote*m_iBaseNote;

				//����ʱ��
				if (m_bPlayer)
					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iThinkTime,ID_OUT_CARD);
#ifndef TWO_NN
				if ( !m_bWatchMode && m_iDeskCardCount[2]==5)
#else
				if( !m_bWatchMode && m_iDeskCardCount[1] == 5 )
#endif
				{
					m_PlayView.m_btBull.ShowWindow(SW_SHOW);

                    //yl add 20120817/
					///m_PlayView.m_btOpen.ShowWindow(SW_SHOW);
					///m_PlayView.m_btReset.ShowWindow(TRUE);//
					m_PlayView.m_btBull.EnableWindow(TRUE);
					///m_PlayView.m_btOpen.EnableWindow(TRUE);
					///m_PlayView.m_btReset.EnableWindow(TRUE);//
					//����Ҫ�ư�ť
					FlashWindowEx(FLASHW_TIMERNOFG|FLASHW_TRAY, 1, 0);
				}
				return TRUE;
			}
		case ASS_NOTE:								//���Ѻע���
			{
				if (uDataSize != sizeof(tagUserResult)) 
					return FALSE;
				tagUserResult *pResult = (tagUserResult *) pNetData;
				//�û���ע������ʾ��ע����,�ݲ���ֵ
				m_iNowOutPeople = pResult->iOutPeople;		//��ǰ��ע��
				m_iThisTurnLimit=m_iLimitNote[GetMeUserInfo()->bDeskStation];
				//ϵͳ������ע����

				if (m_iNowOutPeople == GetMeUserInfo()->bDeskStation && !m_bWatchMode && m_bPlayer)
				{
					m_PlayView.m_btBull.EnableWindow(TRUE);
					m_PlayView.m_btOpen.EnableWindow(TRUE);
					//����Ҫ�Ƶȵİ�ť            
					FlashWindowEx(FLASHW_TIMERNOFG|FLASHW_TRAY, 1, 0);
				}
				//����ʱ��
				if (m_iNowOutPeople!=-1 && (m_bPlayer)) 
					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iThinkTime,ID_OUT_CARD);
				return true;
			}
		case ASS_NOTE_RESULT:											//Ѻע���
			{
				if (uDataSize != sizeof(NoteResult)) 
					return FALSE;


				NoteResult *noteresult=(NoteResult*)pNetData;
				
				m_iNowOutPeople=noteresult->iOutPeople;
				int iViewStation = ViewStation(noteresult->iOutPeople);
				m_PlayView.SetWait(false);//ȡ����ȴ���ͼ

				if (GetMeUserInfo()->bDeskStation == noteresult->iOutPeople)
				{
					m_PlayView.SetDrawNote();
				}


				//����Ƿ���������ʾ����
				switch(noteresult->bAddStyle)
				{
				case TYPE_GIVE_UP://����
					{
						if (GetStationParameter()!=GS_PLAY_GAME)
							m_PlayView.SetPass(iViewStation,1);
						memset(m_UserCard[iViewStation],0,m_iDeskCardCount[iViewStation]);
						memset(m_DeskCard[iViewStation],0,m_iDeskCardCount[iViewStation]);
						m_PlayView.m_UserCard[iViewStation].SetCard(m_DeskCard[iViewStation],NULL,m_iDeskCardCount[iViewStation]);//��ʾ����������
						if(GetMeUserInfo()->bDeskStation==noteresult->iOutPeople)
						{
							m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetUseMouse(false);
						}
						break;
					}

				case TYPE_OPEN://ͣ��(��Ҫ��)
					{
						
		
						break;
					}
				/*case TYPE_RESET:
					{

						break;
					}*/
				case TYPE_NOTE:
					//������ע
					{
						
						m_PlayView.SetNotePoint(iViewStation,noteresult->iCurNote);
						//����ĳһ������ע�ܶ�
						m_iThisGameNote[noteresult->iOutPeople]+=noteresult->iCurNote;
						//������ʾ�û���������ע���
						//m_PlayView.SetAddNote(noteresult->iCurNote);

					//	Msg("zhtlog:noteresult->iCurNote=%I64d",noteresult->iCurNote);

						m_PlayView.SetNotePointStart(ViewStation(noteresult->iOutPeople));
						//Ŀǰ��ʾ����

						if(m_pGameInfo->bEnableSound)
						{
							SetPlaySound(SOUND_USER_CHIP);	
						}
	
					}
					break;
				}
				return true;
			}
		case ASS_CALL_SCORE:			//�з�
			{	
				CallScoreStruct *score=(CallScoreStruct *)pNetData;	

				if (m_bWatchMode) 
				{
 					//int iViewStation = ViewStation(score->bDeskStation);
 					//m_PlayView.SetNTStation(iViewStation);

					KillAllTimer();
					ResetGameStation(RS_GAME_BEGIN);
				}

				if(m_pGameInfo->bEnableSound)
				{
					SetPlaySound(SOUND_START_DEF);
				}

				SetStationParameter(GS_SEND_CARD);
				
				m_bPlayer = score->bPlayer;
				m_bCurrentOperationStation = score->bDeskStation;
				m_PlayView.m_GameNoteView.SetMessage(NULL);
				if(m_bCurrentOperationStation != 255)
				{
					SetGameTimer(m_bCurrentOperationStation,m_iCallScoreTime,ID_CALL_SCORE_TIME);
				}

				//Msg("zhtlog11:m_bPlayer=%d",m_bPlayer);

				if(!m_bWatchMode && GetMeUserInfo()->bDeskStation == score->bDeskStation && m_bPlayer)
				{
					ShowCallScoreBt(score->iValue);
#ifdef AI_MACHINE
					int result = rand()%4;
					if(result > score->iCallScore && result > 0 && result <= 3)
						;
					else 
						result = 0;
					OnCallScore(result,result);
#endif 
				}
				return true;
			}
		case ASS_CALL_SCORE_RESULT:			//�зֽ��
			{
				if (uDataSize != sizeof(CallScoreStruct)) 
					return FALSE;
				KillTimer(ID_CALL_SCORE_TIME);
				HideButton();

				return true;
			}
		case ASS_CALL_SCORE_FINISH:
			{
				if (uDataSize != sizeof(CallScoreStruct)) 
					return FALSE;

				KillTimer(ID_CALL_SCORE_TIME);
				HideButton();
				m_bCurrentOperationStation = 255;
				CallScoreStruct *score=(CallScoreStruct*)pNetData;

				int iViewStation = ViewStation(score->bDeskStation);
				m_PlayView.SetNTStation(iViewStation);

				return true;
			}

		case ASS_SALE_END:	//��Ϸ��ȫ����
			{
				if (uDataSize!=sizeof(GameCutStruct)) 
					return FALSE;
				GameCutStruct * pGameEnd=(GameCutStruct *)pNetData;
				KillGameTimer(0);
				
				SetStationParameter(GS_WAIT_ARGEE);	
				KillAllTimer();
				ResetGameStation(RS_GAME_CUT);
				//�Ƿ�Ϊ������
				if(!m_bWatchMode)
				{
					m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btContinue.SetFocus();

					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
					
				}
				return TRUE;
			}
		case ASS_CUT_END:	//�û�ǿ���뿪
			{
				if (uDataSize!=sizeof(GameCutStruct))
					return FALSE;
				GameCutStruct * pGameEnd=(GameCutStruct *)pNetData;
				KillGameTimer(0);
				//����ǿ�
				GameFinishNotify CutNotify;
				::memset(&CutNotify,0,sizeof(CutNotify));
				if(pGameEnd->bDeskStation != GetMeUserInfo()->bDeskStation&&m_pUserInfo[pGameEnd->bDeskStation]!=NULL)
				{
					//��ʾ��Ϣ
					TCHAR szMessage[100];
					if (pNetHead->bAssistantID==ASS_CUT_END)
						wsprintf(szMessage,TEXT("���ڡ� %s ��ǿ���˳�����Ϸ������"),m_pUserInfo[pGameEnd->bDeskStation]->GameUserInfo.nickName);
					else 
						wsprintf(szMessage,TEXT("���ڡ� %s ���뿪����Ϸ������"),m_pUserInfo[pGameEnd->bDeskStation]->GameUserInfo.nickName);
					//					m_PlayView.m_StationView.CleanAllInfo(TRUE);
					//���ý�����ʾ��Ϣ
					for(int i=0;i<PLAY_COUNT;i++)
					{
						if(m_pUserInfo[i]!=NULL)
						{
							wsprintf(CutNotify.name[i],"%s",m_pUserInfo[i]->GameUserInfo.nickName);
							CutNotify.iWardPoint[i]=pGameEnd->iTurePoint[i];
							//	CutNotify.iTotalPoint[i]=m_pUserInfo[i]->GameUserInfo.dwPoint+pGameEnd->iTurePoint[i];
							CutNotify.iMoney[i]=pGameEnd->iChangeMoney[i];//m_pUserInfo[i]->GameUserInfo.dwMoney;
						}
						else
						{
							memset(CutNotify.name[i],0,sizeof(CutNotify.name[i]));
							CutNotify.iWardPoint[i]=0;
							//CutNotify.iTotalPoint[i]=0;
							CutNotify.iMoney[i]=0;
						}
					}

					m_MessageHandle.InsertNormalMessage(szMessage);
					//if(m_pGameInfo->bEnableSound)
					//	SetSenceSound(SOUND_USER_RUNAWARY,m_pUserInfo[GetMeUserInfo()->bDeskStation]->GameUserInfo.bBoy);

				}
				SetStationParameter(GS_WAIT_ARGEE);
				KillAllTimer();

				//m_bShowLast=FALSE;
				//m_iLastCardCount=0;
				ResetGameStation(RS_GAME_CUT);
				//				m_PlayView.m_Result.ShowWindow(SW_SHOW);

				//�����ƾ�
				if (m_bWatchMode==FALSE)
				{
					m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					///m_PlayView.m_btContinue.SetFocus();	//yl add 20121015/
					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
				}
				return TRUE;
			}
		case ASS_CONTINUE_END:	//��Ϸ����
		case ASS_NO_CONTINUE_END:
			{
				if (uDataSize!=sizeof(GameEndStruct)) 
					return FALSE;
				GameEndStruct * pGameEnd=(GameEndStruct *)pNetData;
				KillGameTimer(0);
				//���÷��� 
				//m_PlayView.m_StationView.SetTurePoint(pGameEnd->iTurePoint[GetMeUserInfo()->bDeskStation]);

				//���÷��� 
				CString stime;
				stime.Format("%d",CTime::GetCurrentTime());
				int curtime=atoi(stime);
				CString str;
				m_MessageHandle.InsertNormalMessage(TEXT("������Ϸ�����"));
				m_MessageHandle.InsertNormalMessage(TEXT("������������������������"));
				GameFinishNotify finishnotify;
				::memset(&finishnotify,0,sizeof(finishnotify));
				int iUserInfoNull = 0;
				
				for (int i=0;i<PLAY_COUNT;i++)
				{
					if(m_pUserInfo[i] == NULL||pGameEnd->iUserState[i]==-1)
					{
						finishnotify.iCardShape[i] = 100;
						iUserInfoNull++;
						continue;
					}
					::CopyMemory(m_UserCard[ViewStation(i)],&pGameEnd->iCardList[i],sizeof(m_UserCard[i]));
					if(pGameEnd->iUserState[GetMeUserInfo()->bDeskStation] && pGameEnd->iUserState[i])
					{
						//OutputDebugString("zhtout:11");
						if (pGameEnd->iCardShape[i] != UG_NO_POINT && pGameEnd->iCardShape[i] < UG_BULL_SILVER)
						{
								
							
							m_Logic.RemoveCard(pGameEnd->iUpBullCard[i],3,m_UserCard[ViewStation(i)],5);
							m_iDeskCardCount[ViewStation(i)]-=3;

							m_PlayView.m_UserBull[ViewStation(i)].SetShowCard(true);
							m_PlayView.m_UserBull[ViewStation(i)].SetCard(m_UserCard[ViewStation(i)],NULL,2);

							m_PlayView.m_UserCard[ViewStation(i)].SetShowCard(true);
							m_PlayView.m_UserCard[ViewStation(i)].SetCard(pGameEnd->iUpBullCard[i],NULL,3);
						
						}
						else //�������ŷ���2
						{
							m_Logic.SortCard(m_UserCard[ViewStation(i)],NULL,m_iDeskCardCount[ViewStation(i)]);

							m_PlayView.m_UserBull[ViewStation(i)].SetUseMouse(false);
							m_PlayView.m_UserBull[ViewStation(i)].SetCard(NULL,NULL,0);

							m_PlayView.m_UserCard[ViewStation(i)].SetShowCard(true);
							m_PlayView.m_UserCard[ViewStation(i)].SetCard(m_UserCard[ViewStation(i)],NULL,5);

						}

						//m_PlayView.m_CardShape[i].ShowCardShape(pGameEnd->iCardShape[i]);
						//m_PlayView.SetNotePoint(ViewStation(i),pGameEnd->iChangeMoney[i]);

						m_PlayView.m_UserCard[ViewStation(i)].Invalidate();
						m_PlayView.m_UserCard[ViewStation(i)].UpdateWindow();
						m_PlayView.m_UserBull[ViewStation(i)].Invalidate();
						m_PlayView.m_UserBull[ViewStation(i)].UpdateWindow();

						m_PlayView.SetShape(ViewStation(i),pGameEnd->iCardShape[i]);
					}
					CString strMoney;
					m_PlayView.GetNumString(pGameEnd->iChangeMoney[i],strMoney,Glb().m_nPowerOfGold);
					//���ý�����ʾ��Ϣ
					str.Format(TEXT("%s �÷� %I64d %s %I64d"),m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i],m_PlayView.m_strMoneyType,pGameEnd->iChangeMoney[i]);
					//˫��
					if(m_pUserInfo[i]->GameUserInfo.iDoublePointTime>curtime && pGameEnd->iTurePoint[i]>0)
					{
						pGameEnd->iTurePoint[i]*=2;
						str.Format(TEXT("%s : �÷�%I64d(˫�����ֿ�Ч����%s %s"),
							m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i],m_PlayView.m_strMoneyType,strMoney/*pGameEnd->iChangeMoney[i]*/);							
					}
					else
					{
						str.Format(TEXT("%s : �÷� %I64d %s %s"),
							m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i],m_PlayView.m_strMoneyType,strMoney/*pGameEnd->iChangeMoney[i]*/);								
					}

					m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));
					wsprintf(finishnotify.name[i],"%s",m_pUserInfo[i]->GameUserInfo.nickName);
					finishnotify.iWardPoint[i]=pGameEnd->iTurePoint[i];
					finishnotify.iMoney[i]=pGameEnd->iChangeMoney[i];
					finishnotify.iCardShape[i]=pGameEnd->iCardShape[i];
				}
				//һ�ֽ���ʱ,���ݷ������Ի���
				for(int i=0;i<PLAY_COUNT;i++)
					m_PlayView.m_showcoredlg.SetUserScore(ViewStation(i)/*viewstation*/,pGameEnd->iTurePoint[i]/*�ı�ķ���*/,pGameEnd->iChangeMoney[i]);

				finishnotify.iBasePoint=m_iBaseNote;
				finishnotify.iStyle=1;


				m_PlayView.m_Result.SetMessage(finishnotify,GetMeUserInfo()->bDeskStation, Glb().m_nPowerOfGold);
				//��Ҹջ�����ָ��Ϊ�գ�����ɽ������ʲô������ʾ������������Ͳ���ʾ�������
				if (iUserInfoNull != PLAY_COUNT)
				{
					m_PlayView.m_Result.ShowWindow(SW_SHOW);
				}
	
				SetStationParameter((pNetHead->bAssistantID==ASS_CONTINUE_END)?GS_WAIT_NEXT:GS_WAIT_ARGEE);

				ResetGameStation(RS_GAME_END);
				if (!m_bWatchMode)
				{
					m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					//m_PlayView.m_btContinue.SetFocus();
					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
				}

				if(m_pGameInfo->bEnableSound)
				{
					if (pGameEnd->iTurePoint[GetMeUserInfo()->bDeskStation] > 0||pGameEnd->iChangeMoney[GetMeUserInfo()->bDeskStation]>0)
						SetPlaySound(SOUND_WIN);
					else
						SetPlaySound(SOUND_LOSE);
				}
				return TRUE;
			}
		case ASS_HAVE_THING:	//�û������뿪
			{
				//HaveThingStruct * pThing=(HaveThingStruct *)pNetData;
				//if (m_PlayView.m_ThingMessage.GetSafeHwnd()==NULL) 
				//{
				//m_PlayView.m_ThingMessage.Create(IDD_THING_MESSAGE,this);
				//m_PlayView.m_ThingMessage.m_pParent=this;
				//}
				//m_PlayView.m_ThingMessage.SetLeaveMessage(pThing->szMessage,m_pUserInfo[pThing->pos]->GameUserInfo.nickName);
				//m_PlayView.m_ThingMessage.ShowWindow(SW_SHOW);

				return TRUE;
			}
		case ASS_LEFT_RESULT:	//�����뿪��� 
			{
				//if (uDataSize!=sizeof(LeaveResultStruct)) 
				//	return FALSE;
				//LeaveResultStruct * pLeftResult=(LeaveResultStruct *)pNetData;
				//CString strBuffer;
				//if (!pLeftResult->bArgeeLeave) 
				//{
				//	strBuffer.Format(TEXT("��� %s ��ͬ����Ϸ�����������ֽ����������뿪�߿����뿪��"),m_pUserInfo[pLeftResult->bDeskStation]->GameUserInfo.nickName);
				//	m_MessageHandle.InsertNormalMessage(strBuffer.GetBuffer(MAX_PATH));
				//}
				//else
				//{
				//	if(pLeftResult->bArgeeLeave&&GetMeUserInfo()->bDeskStation==pLeftResult->bDeskStation&&!m_bWatchMode)
				//		OnCancel();
				//}
				return TRUE;
			}
		case ASS_MESSAGE:
			{
				if (uDataSize!=sizeof(MessageStruct))
				{
					return FALSE;
				}
				MessageStruct *pMessage=(MessageStruct*)pNetData;
				m_MessageHandle.InsertSystemMessage(pMessage->Message);
				return true;
			}
		case ASS_STOP_THING://�Լ�������ǰ����
			{
				//if (m_PlayView.m_StopMessage.GetSafeHwnd()==NULL) 
				//{
				//m_PlayView.m_StopMessage.Create(IDD_STOP_MESSAGE,this);
				//m_PlayView.m_StopMessage.m_pParent=this;
				//}
				//m_PlayView.m_StopMessage.ShowWindow(SW_SHOW);
				//				
				//
				return true;
			}
		case ASS_AGREE_STOP://������ǰ�������
			{
				//if (uDataSize!=sizeof(StopResultStruct))return FALSE;
				//StopResultStruct *pStop=(StopResultStruct*)pNetData;
				//if (pStop->bArgee) 
				//{
				//	m_MessageHandle.InsertNormalMessage("�Լ�ͬ����ǰ������");
				//}
				//else 
				//{
				//	m_MessageHandle.InsertNormalMessage("�ԼҲ�ͬ����ǰ������");
				//}
				return true;
			}
		case ASS_NOMONEY:
			{
				ResetGameStation(RS_ALL_VIEW);
				OnCancel();
				AfxMessageBox("���������û�дﵽׯ��Ҫ��,��Ϸ�����¿�ʼ");
				//m_MessageHandle.InsertNormalMessage("��ϵͳ��Ϣ��:��������ж�û�дﵽ��ׯ��Ҫ��,��Ϸ�����¿�ʼ");
				return true;



			}
		case ASS_TAN_PAI:		//̯��
			{
				if (m_pGameInfo->bEnableSound)
				{
					SetPlaySound(SOUND_TAN_PAI);
				}
				return true;
			}

		}
	}

	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize/*,pClientSocket*/);
}

//�������ý���
void CClientGameDlg::ResetGameFrame()
{
	m_PlayView.m_Result.ShowWindow(SW_HIDE);
	m_bTimeOutCount=0;

	m_iNowOutPeople=-1;

	m_iFirstOutPeople=-1;

	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));

	//��������
	for (int i=0;i<PLAY_COUNT;i++)
	{
		m_PlayView.m_UserCard[i].SetUseMouse(false);
		m_PlayView.m_UserCard[i].SetCard(NULL,NULL,0);
		m_PlayView.m_UserBull[i].SetUseMouse(false);
		m_PlayView.m_UserBull[i].SetCard(NULL,NULL,0);
	}
	m_PlayView.m_GameNoteView.SetMessage(NULL);
	//	if (m_PlayView.m_GameInfo.GetSafeHwnd()) m_PlayView.m_GameInfo.DestroyWindow();
	//	if (m_PlayView.m_BackCardDlg.GetSafeHwnd()) m_PlayView.m_BackCardDlg.DestroyWindow();
	//	if (m_PlayView.m_HaveThing.GetSafeHwnd()) m_PlayView.m_HaveThing.DestroyWindow();
	//	if (m_PlayView.m_ThingMessage.GetSafeHwnd()) m_PlayView.m_ThingMessage.DestroyWindow();
	//	if (m_PlayView.m_GameSet.GetSafeHwnd()) m_PlayView.m_GameSet.DestroyWindow();
	//	if (m_PlayView.m_GameEndView.GetSafeHwnd()) m_PlayView.m_GameEndView.DestroyWindow();
	return __super::ResetGameFrame();
}

//��ʼ��ť����
LRESULT	CClientGameDlg::OnHitBegin(WPARAM wparam, LPARAM lparam)
{
	if (m_pGameInfo->bEnableSound)
	{
		SetPlaySound(SOUND_HIT_BEGIN);
	}
	OnControlHitBegin();
	return 0;
}

bool CClientGameDlg::OnControlHitBegin()
{
	m_PlayView.m_btContinue.ShowWindow(SW_HIDE);		//����
	m_PlayView.m_btLeave.ShowWindow(SW_HIDE);		//����
	//SetSenceSound(SOUND_READY,m_pUserInfo[GetMeUserInfo()->bDeskStation]->GameUserInfo.bBoy);
	if (NULL != m_PlayView.m_GameNoteView.GetSafeHwnd())
	{
		m_PlayView.m_GameNoteView.SetMessage(TEXT("���ڵȴ�������ҿ�ʼ��Ϸ!"));		//�Դ�������Waitingͼ��
	}

	if ((GetStationParameter()==GS_WAIT_SETGAME||GetStationParameter()==GS_WAIT_NEXT||GetStationParameter()==GS_WAIT_ARGEE)&&!m_bWatchMode)
	{
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}
	KillAllTimer();
	//	m_PlayView.m_StationView.CleanAllInfo(FALSE);
	//	m_PlayView.m_StationView.m_iTurePoint=0;
	ResetGameStation(RS_GAME_BEGIN);

	//�Թ���ҹرս���Ԫ��		//yl add 20121015/
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		m_PlayView.m_CardShape[i].ShowWindow(SW_HIDE);
	}

	return true;
}

//������ж�ʱ��
void CClientGameDlg::KillAllTimer()
{
	KillTimer(ID_BEGIN_TIME);
	KillTimer(ID_OUT_CARD);
	KillTimer(ID_LOOK_BACK);
	KillTimer(ID_NOTE);
	KillTimer(ID_CALL_SCORE_TIME);
	KillTimer(ID_BEGIN_ANIMAL);
	//	KillTimer(ID_END_ANIMAL);
	KillTimer(ID_NOTE_ANIMAL_ZERO);
	KillTimer(ID_NOTE_ANIMAL_ONE);
	KillTimer(ID_NOTE_ANIMAL_TWO);
	KillTimer(ID_NOTE_ANIMAL_THREE);	
	KillTimer(ID_COLLECT_JETON_ANIMAL);
	for(int i = 0 ;i<PLAY_COUNT ;i++)
	{
		KillTimer(ID_SEND_CARD_ANIMAL+i);
	}
	memset(m_PlayView.m_uTime,0,sizeof(m_PlayView.m_uTime));
	m_PlayView.m_uTime[2]=0;
	return;
}

//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case ID_BEGIN_TIME:	//������Ϸ��ʱ��
		{
			if (uTimeCount<=0)
			{
				//ʱ�䵽��
				if (!m_bWatchMode)
				{
					if (1 == m_iAfterWaitAction)
					{
						OnHitBegin(0,0);
					}
					else
					{
						OnCancel();
					}
					
				}
				return true;
			}
			if (uTimeCount<=5) 
			{
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_WARNING_0 - uTimeCount);	
			}

			return true;
		}
	case ID_NOTE:
		{
			if (uTimeCount<=0)
			{
				KillAllTimer();
				if (GetMeUserInfo()->bDeskStation != m_iUpGradePeople)
				{
					m_PlayView.SetDrawNote();
					if (!m_bWatchMode)
					{
						m_PlayView.OnNote(4);
					}
				}

				OnHitActiveResult(0,0);
			}
			if (uTimeCount<=5) 
			{
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_WARNING_0 - uTimeCount);	
					//PlaySound(TEXT("WARNING"),GetModuleHandle(CLIENT_DLL_NAME),SND_RESOURCE|SND_NOSTOP|SND_ASYNC|SND_NODEFAULT|SND_NOWAIT);
			}
			return true;

		}
	case ID_CALL_SCORE_TIME:
		{
			if (uTimeCount<=0)	//ʱ�䵽��
			{
				KillAllTimer();
				//����
				/*if(USER_CUT_GAME == m_pUserInfo[m_bCurrentOperationStation]->GameUserInfo.bUserState )
				{
					UserSupersedeNote(m_bCurrentOperationStation);
				
					return true;
					}*/
				if (!m_bWatchMode)
					OnCallScore(0,0);

			}
			if (uTimeCount<=5) 
			{
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_WARNING_0 - uTimeCount);	
					//PlaySound(TEXT("WARNING"),GetModuleHandle(CLIENT_DLL_NAME),SND_RESOURCE|SND_NOSTOP|SND_ASYNC|SND_NODEFAULT|SND_NOWAIT);
			}
			return true;

		}

	case ID_OUT_CARD:	//����
		{
			int iViewStation=ViewStation(bDeskStation);
			if (uTimeCount<=0)	//ʱ�䵽��
			{

				//����
				//for (int i = 0; i < PLAY_COUNT; i++)
				//{
				//	if (!m_pUserInfo[i])
				//		continue;
				//	if(m_pUserInfo[i]->GameUserInfo.bUserState==USER_CUT_GAME)

				//		UserSupersedeNote(i);
				////	return true;
				//}
				
				//OnHitActiveResult(TYPE_OPEN,0);
				//�жϳ�ʱ
				if(/*(m_iNowOutPeople==GetMeUserInfo()->bDeskStation) && */(m_bWatchMode==FALSE))
					m_bTimeOutCount++;
				if (m_bTimeOutCount>3 && (m_iNowOutPeople==GetMeUserInfo()->bDeskStation) && (m_bWatchMode==FALSE))
				{
					KillGameTimer(0);
					SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
					SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
					AFCMessageBox(TEXT("��ʱ����̫�࣬��Ϸ������"),m_pGameInfo->szGameName,AFC_CONFIRM);//,this);
					PostMessage(WM_CLOSE,0,0);
					return false;
				}
				else if(/*m_iNowOutPeople==GetMeUserInfo()->bDeskStation && */!m_bWatchMode)
				{
					CString str;
					//					str.Format("��ע�⣬���Ѿ���ʱ %d ���ˣ���ʱ���λᵱ�����ܴ���",m_bTimeOutCount);
					//str.Format("��ע�⣬���Ѿ���ʱ��ϵͳ�Զ�Ϊ��̯�ơ�");
					m_MessageHandle.InsertNormalMessage("��ע�⣬���Ѿ���ʱ��ϵͳ�Զ�Ϊ��̯�ơ�");
					OnHitActiveResult(TYPE_OPEN,0);
				}

				
				return true;
			}
			if ((uTimeCount<=5)&&(m_bWatchMode==FALSE)&&(iViewStation==2)) 
			{
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_WARNING_0 - uTimeCount);	
				//					PlaySound(TEXT("WARNING"),GetModuleHandle(CLIENT_DLL_NAME),SND_RESOURCE|SND_NOSTOP|SND_ASYNC|SND_NODEFAULT|SND_NOWAIT);
			}
			return true;
		}
	}
	return true;
}

//�����˿�
LRESULT CClientGameDlg::OnHitCard(WPARAM wparam, LPARAM lparam)
{
	if (GetStationParameter()==GS_PLAY_GAME && !m_bWatchMode)
	{//��֤������Ϣ
		BYTE iUpCard[39];
		BYTE bView=1;
#ifndef TWO_NN
		bView = 2;
		int iCardCount=m_PlayView.m_UserCard[2].GetUpCard(iUpCard);
#else 
		bView = 1;
		int iCardCount = m_PlayView.m_UserCard[1].GetUpCard(iUpCard);
#endif
		if (m_pGameInfo->bEnableSound)
		{
			SetPlaySound(SOUND_HIT_CARD);
		}

		//������Ŀ�ж�
		if (iCardCount!=3 && iCardCount!=2)
		{
			//m_PlayView.m_btBull.EnableWindow(FALSE);
			return 0;
		}
		//if (!m_PlayView.m_bRightPower && iCardCount!=3)
		//{
		//	//m_PlayView.m_btBull.EnableWindow(FALSE);
		//	return 0;
		//}

		//��֤�Ƿ���Գ��������˿�
		//if (!m_PlayView.m_bRightPower)
			//m_PlayView.m_btBull.EnableWindow(m_Logic.IsBull(iUpCard,iCardCount));

		if (m_PlayView.m_bRightPower)
		{
			Msg("Test: �� If �� Into");

			BYTE card[10];	
			if (iCardCount == 2)
			{
							
				::CopyMemory(card,&m_UserCard[bView],sizeof(m_UserCard[bView]));
				m_Logic.RemoveCard(iUpCard,iCardCount,card,5);

				if (m_Logic.IsBull(card,3))
				{	
					for (int i=0; i<3; i++)
					{
						m_iUpBullCard[bView][i]=card[i];
					}

					OnHitActiveResult(TYPE_BULL, 1);
				}

				m_PlayView.SetShape(bView, m_Logic.GetShape(m_UserCard[bView],5,card));
				m_PlayView.m_btBull.ShowWindow(SW_SHOW);
				m_PlayView.m_btOpen.ShowWindow(SW_SHOW);
				m_PlayView.m_btReset.ShowWindow(SW_SHOW);
				m_PlayView.m_btBull.EnableWindow(TRUE);
				m_PlayView.m_btOpen.EnableWindow(TRUE);
				m_PlayView.m_btReset.EnableWindow(TRUE);

				//if(m_pGameInfo->bEnableSound && m_Logic.GetShape(m_UserCard[bView],5,card) == UG_BULL_BULL)
					//SetNoteSound(TYPE_NN,m_pUserInfo[ViewStation(GetMeUserInfo()->bDeskStation)]->GameUserInfo.bBoy);					//Ѻע����
			}

			if (iCardCount == 3)
			{
				m_PlayView.SetShape(bView,m_Logic.GetShape(m_UserCard[bView],5,iUpCard));
				if (m_Logic.IsBull(iUpCard,iCardCount))
					OnHitActiveResult(TYPE_BULL, 1);
				m_PlayView.m_btBull.ShowWindow(SW_SHOW);
				m_PlayView.m_btOpen.ShowWindow(SW_SHOW);
				m_PlayView.m_btReset.ShowWindow(SW_SHOW);
				m_PlayView.m_btBull.EnableWindow(TRUE);
				m_PlayView.m_btOpen.EnableWindow(TRUE);
				m_PlayView.m_btReset.EnableWindow(TRUE);

				//if(m_pGameInfo->bEnableSound && NULL!=m_pUserInfo[bView] && m_Logic.GetShape(m_UserCard[bView],5,iUpCard)==UG_BULL_BULL)
					//SetNoteSound(TYPE_NN,m_pUserInfo[bView]->GameUserInfo.bBoy);					//Ѻע����

			}
		}
		else
		{
			Msg("Test: �� else �� Into");
			//m_PlayView.SetShape(bView, m_Logic.GetShape(m_UserCard[bView], 5, iUpCard));
			//if(m_pGameInfo->bEnableSound && NULL!=m_pUserInfo[bView] && m_Logic.GetShape(m_UserCard[bView], 5, iUpCard)==UG_BULL_BULL)
				//SetNoteSound(TYPE_NN, m_pUserInfo[bView]->GameUserInfo.bBoy);					//Ѻע����

		}

	}
	
	return 0L;
}

//�����˿�
LRESULT CClientGameDlg::OnMouseMove(WPARAM wparam, LPARAM lparam)
{
	//if (GetStationParameter() == GS_PLAY_GAME/*&&m_Ming[GetMeUserInfo()->bDeskStation]*/)
	//{
	//	/*BYTE iCard[5];
	//	int iCount = m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].GetUpCard(iCard);
	//	if (iCount == 0) 
	//	return 0L;
	//	for (int i = 0;i < iCount;i++)
	//	if (m_Logic.GetCardNum(iCard[i]) != 1) 
	//	return 0L;*/
	//	//	SetGameTimer(NULL,3000,ID_LOOK_BACK);
	//	SetTimer(ID_LOOK_BACK,3000,NULL);

	//	m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetCard(m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)],NULL,m_iDeskCardCount[ViewStation(GetMeUserInfo()->bDeskStation)]);
	//}
	return TRUE;;
}
//�����˿�
LRESULT CClientGameDlg::OnMouseLeft(WPARAM wparam, LPARAM lparam)
{
	//if (GetStationParameter() == GS_PLAY_GAME /*&&m_Ming[GetMeUserInfo()->bDeskStation]*/)
	//{
	//	/*BYTE iCard[5];
	//	int iCount = m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].GetUpCard(iCard);
	//	if (iCount == 0) 
	//	return 0L;
	//	for (int i = 0;i < iCount;i++)
	//	if (m_Logic.GetCardNum(iCard[i]) != 1) 
	//	return 0L;*/
	//	//	SetGameTimer(NULL,3000,ID_LOOK_BACK);
	//	SetTimer(ID_LOOK_BACK,3000,NULL);

	//	m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetCard(m_DeskCard[ViewStation(GetMeUserInfo()->bDeskStation)],NULL,m_iDeskCardCount[ViewStation(GetMeUserInfo()->bDeskStation)]);
	//}
	return TRUE;	
}

//�û�������
LRESULT CClientGameDlg::OnHitActiveResult(WPARAM wparam, LPARAM lparam)
{
	if (GetStationParameter() == GS_PLAY_GAME)
	{
		int ret = (int) wparam;
		m_PlayView.SetDrawNote();
        
		switch(wparam)
		{
		case TYPE_RESET:	//����
			if (m_pGameInfo->bEnableSound)
			{
				SetPlaySound(SOUND_CHONG_PAI);
			}
			//m_PlayView.m_btBull.ShowWindow(SW_SHOW&&!m_PlayView.m_bRightPower);
			m_PlayView.m_btBull.ShowWindow(SW_SHOW);
#ifndef TWO_NN
			memset(m_iUpBullCard[2],1,sizeof(m_iUpBullCard));
			m_PlayView.SetShape(2,-1);
			m_PlayView.m_btOpen.ShowWindow(SW_HIDE);    //yl alter/
			m_PlayView.m_btReset.ShowWindow(SW_HIDE);   //yl alter/
			m_PlayView.m_btBull.EnableWindow(TRUE);
			m_PlayView.m_btOpen.EnableWindow(FALSE);    //yl alter/
			m_PlayView.m_btReset.EnableWindow(FALSE);   //yl alter/
			m_PlayView.m_UserCard[2].SetCard(m_DeskCard[2],NULL,m_iDeskCardCount[2]);
			m_PlayView.m_UserBull[2].SetCard(NULL,NULL,0);
			m_PlayView.m_UserCard[2].SetUseMouse(true);
#else
			memset(m_iUpBullCard[1],1,sizeof(m_iUpBullCard));
			m_PlayView.SetShape(1,-1);
			m_PlayView.m_btOpen.ShowWindow(SW_SHOW);
			m_PlayView.m_btBull.EnableWindow(TRUE);
			m_PlayView.m_btOpen.EnableWindow(TRUE);
			m_PlayView.m_btReset.ShowWindow(SW_SHOW);
			m_PlayView.m_btReset.EnableWindow(TRUE);
			m_PlayView.m_UserCard[1].SetCard(m_DeskCard[1],NULL,m_iDeskCardCount[1]);
			m_PlayView.m_UserCard[1].SetUseMouse(true);
			m_PlayView.m_UserBull[1].SetCard(NULL,NULL,0);
			m_PlayView.m_UserCard[1].SetUseMouse(true);
#endif
			break;
		case TYPE_OPEN:	//̯��
			{
				KillAllTimer();
				m_PlayView.m_btOpen.ShowWindow(SW_HIDE);
				m_PlayView.m_btOpen.EnableWindow(FALSE);
				m_PlayView.m_btBull.ShowWindow(SW_HIDE);
				m_PlayView.m_btBull.EnableWindow(FALSE);
				m_PlayView.m_btReset.ShowWindow(SW_HIDE);
                m_PlayView.m_btReset.EnableWindow(FALSE);///
				

				tagUserProcess process;
				process.iVrebType = TYPE_OPEN;				//����
				if (m_bAutoBull && m_PlayView.m_UserBull[ViewStation(GetMeUserInfo()->bDeskStation)].GetCardCount() == 0)
				{
					OnHitActiveResult(TYPE_BULL,0);
				}
#ifndef TWO_NN
				::CopyMemory(process.bUpCard,m_iUpBullCard[2],sizeof(m_iUpBullCard[2]));		
#else
				::CopyMemory(process.bUpCard,m_iUpBullCard[1],sizeof(m_iUpBullCard[1]));
#endif
				int itemp = m_Logic.GetShape(m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)],5,m_iUpBullCard[ViewStation(GetMeUserInfo()->bDeskStation)]);

				m_PlayView.SetShape(ViewStation(GetMeUserInfo()->bDeskStation),itemp);
				if (m_pGameInfo->bEnableSound&&itemp>=0)
				{
					SetNoteSound(itemp+SOUND_NO_POINT,GetMeUserInfo()->bBoy);

                    if (itemp == 0)
                    {
                        m_PlayView.m_btOpen.ShowWindow(SW_HIDE);
                        m_PlayView.m_btOpen.EnableWindow(FALSE);
                        m_PlayView.m_btReset.ShowWindow(SW_HIDE);
                        m_PlayView.m_btReset.EnableWindow(FALSE);
                    }
				}
				SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
			}
			break;
		case TYPE_BULL:		//��ʾ
			{
				if (m_pGameInfo->bEnableSound)
				{
					SetPlaySound(SOUND_USER_CUE);
				}
				int iView=2;

#ifndef TWO_NN
				iView = 2;
#else
				iView = 1;
#endif

			   // ����ţ��
				BYTE card[10],cardBuf[5];
				if (lparam != 1)
				{
					
					if ( !m_Logic.GetBull(m_UserCard[iView], 5, m_iUpBullCard[iView]) )
					{
					
						m_PlayView.SetShape(iView,0);
						m_PlayView.m_btBull.EnableWindow(FALSE);
                        m_PlayView.m_btBull.ShowWindow(SW_HIDE);

                        //yl add 20120817/
                        m_PlayView.m_btOpen.ShowWindow(SW_SHOW);
                        m_PlayView.m_btOpen.EnableWindow(TRUE);
                        m_PlayView.m_btReset.ShowWindow(SW_SHOW);
                        m_PlayView.m_btReset.EnableWindow(TRUE);

						return 0;
					}
					
				}
				else
				{
					int iUpCount = m_PlayView.m_UserCard[iView].GetUpCard(cardBuf);
					::CopyMemory(m_iUpBullCard[iView], cardBuf,sizeof(m_iUpBullCard[iView]));

					if (3==iUpCount)
					{
						;
					}
					else if (2==iUpCount)
					{
						::CopyMemory(card, &m_UserCard[iView], sizeof(m_UserCard[iView]));
						m_Logic.RemoveCard(m_iUpBullCard[iView], 2, card, 5);
						::CopyMemory(m_iUpBullCard[iView], card, sizeof(m_iUpBullCard[iView]));
					}
					else
						return 0;
				}

				if ( !m_Logic.IsBull(m_iUpBullCard[iView], 3) )
					return 0;

				if(1 != lparam)
				{
					OutputDebugString("zhtlog:555");
					m_PlayView.SetShape(iView, m_Logic.GetShape(m_UserCard[iView],5,m_iUpBullCard[iView]));
					//if (m_pGameInfo->bEnableSound && NULL!=m_pUserInfo[iView] && m_Logic.GetShape(m_UserCard[iView],5,m_iUpBullCard[iView])==UG_BULL_BULL)
						//SetNoteSound(TYPE_NN,m_pUserInfo[iView]->GameUserInfo.bBoy);
				}

				::CopyMemory(card,&m_UserCard[iView],sizeof(m_UserCard[iView]));				
				m_PlayView.m_UserBull[iView].SetShowCard(true);
				m_Logic.RemoveCard(m_iUpBullCard[iView], 3, card, 5);
				m_PlayView.m_UserCard[iView].SetCard(m_iUpBullCard[iView], NULL, 3);
				m_PlayView.m_UserBull[iView].SetCard(card, NULL, 2);
				m_PlayView.m_UserCard[iView].SetUseMouse();

				m_PlayView.m_btBull.ShowWindow(SW_HIDE);
				m_PlayView.m_btBull.EnableWindow(FALSE);

                //yl add 20120817/
                m_PlayView.m_btOpen.ShowWindow(SW_SHOW);
                m_PlayView.m_btOpen.EnableWindow(TRUE);
                m_PlayView.m_btReset.ShowWindow(SW_SHOW);
                m_PlayView.m_btReset.EnableWindow(TRUE);
	
				
			}
			break;
		}

	}
	return 0L;
}

//�й�
LRESULT CClientGameDlg::OnHitAuto(WPARAM wparam,LPARAM lparam)
{
	return 0;
}
//�������°�ť����
LRESULT	CClientGameDlg::OnHitHaveThing(WPARAM wparam, LPARAM lparam)
{
	/*	if (m_PlayView.m_HaveThing.GetSafeHwnd()==NULL) 
	{
	m_PlayView.m_HaveThing.Create(IDD_THING,this);
	m_PlayView.m_HaveThing.m_pParnet=this;
	}
	m_PlayView.m_HaveThing.ShowWindow(SW_SHOW);
	m_PlayView.m_HaveThing.SetFocus();*/
	return 0;
}

//��ǰ����
LRESULT	CClientGameDlg::OnStop(WPARAM wparam, LPARAM lparam)
{
	/*	if(m_PlayView.m_bMeSide && m_PlayView.m_iGamePoint>=80 && !m_bWatchMode)
	{
	if (m_PlayView.m_StopThing.GetSafeHwnd()==NULL) 
	{
	m_PlayView.m_StopThing.Create(IDD_STOP,this);
	m_PlayView.m_StopThing.m_pParnet=this;
	}
	m_PlayView.m_StopThing.ShowWindow(SW_SHOW);
	m_PlayView.m_StopThing.SetFocus();
	}*/
	return 0;
}

//������ǰ����
LRESULT	CClientGameDlg::OnStopThing(WPARAM wparam, LPARAM lparam)
{
	/*	if(!m_bWatchMode && GetStationParameter()!=GS_WAIT_SETGAME && m_PlayView.m_bMeSide && m_PlayView.m_iGamePoint>=80)
	{
	SendGameData(MDM_GM_GAME_NOTIFY,ASS_STOP_THING,0);
	}*/
	return 0;
}

//ͬ����ǰ����
LRESULT	CClientGameDlg::OnAgreeStop(WPARAM wparam, LPARAM lparam)
{
	/*	if(!m_bWatchMode && GetStationParameter()!=GS_WAIT_SETGAME)
	{
	StopResultStruct Stop;
	Stop.bArgee=(BYTE)wparam;
	SendGameData(&Stop,sizeof(Stop),MDM_GM_GAME_NOTIFY,ASS_AGREE_STOP,0);
	}
	*/	return 0;
}


//�����뿪������
LRESULT	CClientGameDlg::OnSendHaveThing(WPARAM wparam, LPARAM lparam)
{
	/*	if(!m_bWatchMode && GetStationParameter()!=GS_WAIT_SETGAME)
	{
	m_PlayView.m_btThing.EnableWindow(FALSE);
	HaveThingStruct HaveThing;
	HaveThing.pos=0;
	::strcpy(HaveThing.szMessage,(char *)wparam);
	SendGameData(&HaveThing,sizeof(HaveThing),MDM_GM_GAME_NOTIFY,ASS_HAVE_THING,0);
	}*/
	return 0;
}

//�û������뿪
LRESULT	CClientGameDlg::OnArgeeUserLeft(WPARAM wparam, LPARAM lparam)
{
	/*	LeaveResultStruct Leave;
	Leave.bDeskStation=GetMeUserInfo()->bDeskStation;
	Leave.bArgeeLeave=(BYTE)wparam;
	SendGameData(&Leave,sizeof(Leave),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	*/	return 0;
}



//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)
{
	KillAllTimer();
	//��������
	m_iNowOutPeople=-1;
	m_iFirstOutPeople=-1;
	m_PlayView.SetWait(false);//ȡ����ȴ���ͼ
	//��������
//	m_PlayView.m_btAuto.EnableWindow(FALSE);
//	m_PlayView.m_btThing.EnableWindow(FALSE);
	m_PlayView.SetDrawNote();
//	m_PlayView.m_btNote.ShowWindow(SW_HIDE);
//	m_PlayView.m_btOver.ShowWindow(SW_HIDE);
	m_PlayView.m_btBull.ShowWindow(SW_HIDE);
	m_PlayView.m_btOpen.ShowWindow(SW_HIDE);
	m_PlayView.m_btReset.ShowWindow(SW_HIDE);
	m_PlayView.m_NtList.ShowWindow(SW_HIDE);

//	m_PlayView.m_btNote.EnableWindow(FALSE);
//	m_PlayView.m_btOver.EnableWindow(FALSE);
	m_PlayView.m_btBull.EnableWindow(FALSE);
	m_PlayView.m_btOpen.EnableWindow(FALSE);
//	m_PlayView.m_btReset.EnableWindow(FALSE);
//	m_PlayView.m_btSplit.EnableWindow(FALSE);
	
	HideButton();
	DelShowThis();
	::memset(m_iUpBullCard,1,sizeof(m_iUpBullCard));
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));	
	::memset(m_UserCard,0,sizeof(m_UserCard));
	//	m_PlayView.m_ShowNote.ShowWindow(SW_HIDE);

	
	for(int i=0;i<PLAY_COUNT;i++)
	{
		m_PlayView.m_SendCardAnimal[i].SetCard(NULL,0,NULL);
	}

	//for (int i = 0; i < PLAY_COUNT; i++)
	//{
	//	m_PlayView.m_CardShape[i].ShowWindow(SW_HIDE);
	//}

	switch (iGameStation)
	{
	case RS_ALL_VIEW:
		//			KillAllTimer();

		for (int i=0;i<PLAY_COUNT;i++) 
		{

			m_PlayView.SetPass(i,0);
			m_PlayView.SetMing(i,0);
		}

		::memset(m_iTotalGameNote,0,sizeof(m_iTotalGameNote));
		::memset(m_iThisGameNote,0,sizeof(m_iThisGameNote));
		//		m_iThisNoteTimes=0;
		m_PlayView.m_Result.ShowWindow(SW_HIDE);
//		m_PlayView.SetAddNote(0);
		//��ע�������õĳ�����
//		m_PlayView.m_NoteAnimal.SetTotalJeton(0);
		//�������õĳ�����
//		for(int i=0;i<50;i++)
//			m_PlayView.m_NoteAll[i].SetTotalJeton(0);
		//���ý����г������
		m_PlayView.SetAddTimes(0);
		//���ø�����ע������ʼֵ
//		for(int i=0;i<PLAY_COUNT;i++)
//			m_PlayView.m_NoteTotal[i].SetTotalJeton(0);
	case RS_GAME_CUT:
		{
			m_PlayView.SetNTStation();
			m_PlayView.m_GameNoteView.SetMessage(NULL);
			/*for (int i=0;i<PLAY_COUNT;i++)
			{
				m_PlayView.m_UserCard[i].SetUseMouse(false);
				m_PlayView.m_UserCard[i].SetCard(NULL,NULL,0);
			}*/
			//ϵͳ������
			//m_iSysCardCount=28;
//			m_PlayView.m_SysBackCard.SetCard(NULL,0,NULL);
			//for(int i=0;i<PLAY_COUNT;i++)
			//	m_PlayView.m_SendCardAnimal[i].SetCard(NULL,0,NULL);
			//���������
			::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));



			//��ʼ�������Ѻע���
			for(int i=0;i<PLAY_COUNT;i++)
			{
				m_PlayView.SetNotePoint(i,0);
				//m_PlayView.SetThisTurnNotePoint(i,0);
				m_PlayView.SetGetPoint(i, 0);
				m_PlayView.SetShape(i, -1);
				m_PlayView.SetGetPointSplit(i, 0);
			}
			//��ʼ����ע
//			m_PlayView.SetTotalNote(0,0);
			//��ʼ����������
			m_PlayView.SetLimitNote(0,0,0);
			m_PlayView.SetMingLImit(0);
			break;
		}
	case RS_GAME_END:
		{
			m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetUseMouse(false);
			memset(m_PlayView.m_bOpenCard,0,sizeof(m_PlayView.m_bOpenCard));
			//for (int i = 0;i < PLAY_COUNT;i++)		
			//{					
			//					m_PlayView.m_EndAnimal[i].ShowWindow(SW_HIDE);
			//					int iViewStation = ViewStation(i);
			//m_PlayView.m_UserCard[i].SetShowCard(true);
			//	m_PlayView.m_UserCard[i].SetCard(m_UserCard[i],NULL,m_iDeskCardCount[i]);
			//}
			//��ʾϵͳ��
			//				m_PlayView.m_SysBackCard.SetShowCard(true);
			//				m_PlayView.m_SysBackCard.SetCard(m_SysCard,0,m_iSysCardCount);
			//���ø�����ע������ʼֵ
//			for(int i=0;i<PLAY_COUNT;i++)
//				m_PlayView.m_NoteTotal[i].SetTotalJeton(0);
			m_PlayView.m_GameNoteView.SetMessage(NULL);
			break;
		}
	case RS_GAME_BEGIN:
		{
			Msg("jiaofen3: ��4��");

			//m_PlayView.SetNTStation();	//yl alter/�޸��Թ������ׯͼ��
			m_PlayView.SetDrawNote();
			for (int i=0;i<PLAY_COUNT;i++) 
			{
				
				m_PlayView.m_UserCard[i].SetUseMouse(false);
				m_PlayView.m_UserCard[i].SetCard(NULL,NULL,0);
				m_PlayView.m_UserBull[i].SetUseMouse(false);
				m_PlayView.m_UserBull[i].SetCard(NULL,NULL,0);
				m_PlayView.SetPass(i,0);
				m_PlayView.SetGetPoint(i, 0);
				m_PlayView.SetShape(i, -1);

				if (m_bWatchMode) 
				{
					m_PlayView.m_CardShape[i].ShowWindow(SW_HIDE);
				}
			}
			//������Ʊ��
			memset(m_PlayView.m_bIsSendCard,0,sizeof(m_PlayView.m_bIsSendCard));
			memset(m_PlayView.m_bIsNote,0,sizeof(m_PlayView.m_bIsNote));
			memset(m_PlayView.m_bOpenCard,0,sizeof(m_PlayView.m_bOpenCard));
			
			//���������
			::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
			::memset(m_iThisGameNote,0,sizeof(m_iThisGameNote));
			::memset(m_iTotalGameNote,0,sizeof(m_iTotalGameNote));
			m_PlayView.m_Result.ShowWindow(SW_HIDE);
//			m_PlayView.SetAddNote(0);
			//��ע�������õĳ�����
//			m_PlayView.m_NoteAnimal.SetTotalJeton(0);
			//�������õĳ�����
//			for(int i=0;i<50;i++)
//				m_PlayView.m_NoteAll[i].SetTotalJeton(0);
			//���ý����г������
			m_PlayView.SetAddTimes(0);
			//���ø�����ע������ʼֵ
//			for(int i=0;i<PLAY_COUNT;i++)
//				m_PlayView.m_NoteTotal[i].SetTotalJeton(0);
			//��ʼ�������Ѻע���
			for(int i=0;i<PLAY_COUNT;i++)
			{
				m_PlayView.SetNotePoint(i,0);
				//m_PlayView.SetThisTurnNotePoint(i,0);
			}
			//��ʼ����ע
//			m_PlayView.SetTotalNote(0,0);
			//��ʼ����������
			m_PlayView.SetLimitNote(0,0,0);
			m_PlayView.SetMingLImit(0);

			//			if (m_PlayView.m_GameSet.GetSafeHwnd()) m_PlayView.m_GameSet.DestroyWindow();
			//			if (m_PlayView.m_GameEndView.GetSafeHwnd()) m_PlayView.m_GameEndView.DestroyWindow();
			break;
		}
	}
	return;
}

void CClientGameDlg::OnGameSetting()
{
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
#ifdef SUPER_VERSION
	AfxSetResourceHandle(GetModuleHandle("DllForGame.dll"));
	if (m_bIsSuper)
	{
		if (m_Control.GetSafeHwnd() == NULL)
		{
			m_Control.Create(IDD_CONTROLWIN,this);
			m_Control.SetParentWnd(this);
		}
		m_Control.ShowWindow(SW_SHOW);
		m_Control.CenterWindow();
		AfxSetResourceHandle(NULL);
		return;
	}
#endif
#ifdef SENDCARD_VERSION
	AfxSetResourceHandle(GetModuleHandle("DllForGame.dll"));
	{

		if (m_SetCard == NULL)
		{
			m_SetCard = new CCardSet();
			m_SetCard->Create(IDD_CARD_SET,this);
			m_SetCard->CenterWindow();
			m_SetCard->ShowWindow(SW_SHOW);
		}
		else
		{
			m_SetCard->UpdateComboBox();
			m_SetCard->ShowWindow(SW_SHOW);
		}

		//dlg->DoModal();
	}
	

	AfxSetResourceHandle(NULL);
	return ;
#endif
	
	CGameSet GameSetDlg(this);
	GameSetDlg.m_bSound=m_pGameInfo->bEnableSound;
	GameSetDlg.m_bShowUser=m_pGameInfo->bShowUserInfo;	
	GameSetDlg.m_bEnableWatch=m_pGameInfo->bEnableWatch;
	GameSetDlg.m_bRightPower=TRUE;//GetProfileInt(TEXT("RightPower"),FALSE);
	
	//if (GameSetDlg.DoModal()==IDOK)
	int reval =  GameSetDlg.DoModal();

	if (reval == IDOK)
	{
		//�������
		bool bSendWatch=((m_bWatchMode==false)&&(m_pGameInfo->bEnableWatch!=GameSetDlg.m_bEnableWatch));

		//��������
		m_pGameInfo->bShowUserInfo=GameSetDlg.m_bShowUser;
		m_pGameInfo->bEnableWatch=GameSetDlg.m_bEnableWatch;
		m_pGameInfo->bEnableSound=GameSetDlg.m_bSound;
		WriteProfileInt(TEXT("RightPower"),GameSetDlg.m_bRightPower?1:0);
		m_PlayView.m_bRightPower=GameSetDlg.m_bRightPower;

		//��������
		if (bSendWatch==true)
		{
			MSG_GM_WatchSet WatchSet;			
			memset(&WatchSet,0,sizeof(WatchSet));
			WatchSet.dwUserID=0;
			SendGameData(&WatchSet,sizeof(WatchSet),MDM_GM_GAME_FRAME,ASS_GM_WATCH_SET,GameSetDlg.m_bEnableWatch?TRUE:FALSE);
		}
	}
	//AfxSetResourceHandle(GetModuleHandle(NULL));
	return;
}

void CClientGameDlg::OnWatchSetChange(void)
{
	if (m_bWatchMode==true)
	{
		m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetShowCard(m_bWatchOther?TRUE:FALSE);//20090414�Թۿ�������zht
		if (m_bWatchOther)	m_MessageHandle.InsertNormalMessage(TEXT("����������Թ�����Ϸ"));
		else m_MessageHandle.InsertNormalMessage(TEXT("��Ҳ��������Թ�����Ϸ"));
	}
}

//�û��뿪
bool CClientGameDlg::GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	/*	if ((bWatchUser==false)&&(pUserItem!=NULL)&&(bDeskStation==0))
	{
	if (GetStationParameter()==GS_WAIT_ARGEE)
	{
	for (int i=0;i<4;i++)
	{
	if (m_pUserInfo[i]!=NULL) 
	{
	m_pUserInfo[i]->GameUserInfo.bUserState=USER_SITTING;
	m_UserListDlg.UpdateGameUser(m_pUserInfo[i]);
	}
	}
	if (m_PlayView.m_GameInfo.GetSafeHwnd()) m_PlayView.m_GameInfo.DestroyWindow();
	m_PlayView.m_GameNoteView.SetMessage(TEXT("���ڵȴ����������ƾ�"));
	m_PlayView.UpdateViewFace(NULL);
	}
	}*/
	//	if ((bWatchUser==false)&&(pUserItem!=NULL)&&(bDeskStation==0))		//����Ƕ����뿪
	//	{
	/*if (GetStationParameter()>GS_WAIT_ARGEE)
	{
		for (int i=0;i<PLAY_COUNT;i++)
		{
			if (m_pUserInfo[i]!=NULL) 
			{
				m_pUserInfo[i]->GameUserInfo.bUserState=USER_SITTING;
				m_UserListDlg.UpdateGameUser(m_pUserInfo[i]);
			}
		}
		m_PlayView.UpdateViewFace(NULL);
	}*/
	//m_PlayView.SetNTStation();

	//m_PlayView.m_UserCard[ViewStation(bDeskStation)].ShowWindow(SW_HIDE);
	//	}
	//��ǰ����뿪��������Ƶ����Ҳ��֮�뿪����
	if(GetMeUserInfo()->bDeskStation==bDeskStation&&pUserItem!=NULL&&!bWatchUser==TRUE)
	{
		//	MessageBox("���ۿ������,�Ѿ��˳�.","ϵͳ��ʾ",MB_ICONQUESTION);
		OnCancel();
		//PostMessage(WM_CLOSE,0,0);
	}

	if(m_pGameInfo->bEnableSound)	//�뿪��Ч
	{
		SetPlaySound(SOUND_USER_LEAVE);
	}
	return __super::GameUserLeft(bDeskStation,pUserItem,bWatchUser);
}

void CClientGameDlg::OnTimer(UINT nIDEvent)
{
	int iAllCount = 0 ;
	for(int i = 0 ; i<PLAY_COUNT ;i++)
	{
		if(NULL != m_pUserInfo[i] && m_iDeskCardCount[ViewStation(i)] == 5)
		{
			iAllCount ++ ; 
		}
		CString str;
		str.Format("wysm_iDeskCardCount[%d]=%d,i=%d",ViewStation(i),m_iDeskCardCount[ViewStation(i)],i);
		OutputDebugString(str);
	}
	CString strr;
	strr.Format("wys423��ʱ������::��ǰ��ʱ��===%d",nIDEvent);
	OutputDebugString(strr);
	
	if ((nIDEvent>=ID_SEND_CARD_ANIMAL) && (nIDEvent<(ID_SEND_CARD_ANIMAL+PLAY_COUNT)))
	{
		CString str;
		str.Format("wys423::Ontimer::iAllCount===%d,m_iSendAnimaCardPos==%d��m_iSendCardTotle==%d",iAllCount,m_iSendAnimaCardPos,m_iSendCardTotle);
		OutputDebugString(str);
		
		
		KillTimer(nIDEvent);		
		int desk = nIDEvent-ID_SEND_CARD_ANIMAL;
		
		m_DeskCard[desk][(m_iSendCardTotle/iAllCount)%5] = (ViewStation(GetMeUserInfo()->bDeskStation)==desk)?m_UserCard[desk][(m_iSendCardTotle/iAllCount)%5]:0;
		m_PlayView.m_SendCardAnimal[desk].StopMove();
		m_PlayView.m_SendCardAnimal[desk].ShowWindow(SW_HIDE);
		Msg("qlog%d: view %d", GetMeUserInfo()->bDeskStation, desk);
		m_PlayView.m_UserCard[desk].SetCard(m_DeskCard[desk],NULL,(m_iSendCardTotle/iAllCount)%5+1);

		if(m_pGameInfo->bEnableSound)
		{
			SetPlaySound(SOUND_SEND_CARD);
		}

		m_iSendCardTotle ++;

		if(m_iSendCardTotle<iAllCount *5)
		{			
			//m_iSendAnimaCardPos = GetNetUserPos(m_iSendAnimaCardPos);
			m_iSendAnimaCardPos = (m_iSendAnimaCardPos+1)%PLAY_COUNT;
			while(NULL == m_pUserInfo[m_iSendAnimaCardPos]||m_iDeskCardCount[ViewStation(m_iSendAnimaCardPos)]==0)
			{
				m_iSendAnimaCardPos = (m_iSendAnimaCardPos+1)%PLAY_COUNT;
			}

			m_PlayView.SendCard(ViewStation(m_iSendAnimaCardPos),150);		

			SetTimer(ID_SEND_CARD_ANIMAL+ViewStation(m_iSendAnimaCardPos),150,NULL);
		}

		return ;

	}
	switch(nIDEvent)
	{
	case ID_LOOK_BACK:
		m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetCard(m_DeskCard[ViewStation(GetMeUserInfo()->bDeskStation)],NULL,m_iDeskCardCount[ViewStation(GetMeUserInfo()->bDeskStation)]);
		KillTimer(ID_LOOK_BACK);
		break;
	case ID_BEGIN_ANIMAL:
		m_PlayView.BeginAnimal();
		break;
	case  ID_NOTE_ANIMAL_ZERO:							//��ע����
		{
			m_PlayView.NoteAnimal(0);
			break;
		}
	case ID_NOTE_ANIMAL_ONE:							//��ע����
		{
			m_PlayView.NoteAnimal(1);
			break;
		}
	case ID_NOTE_ANIMAL_TWO:								//��ע����
		{
			m_PlayView.NoteAnimal(2);
			break;
		}
	case ID_NOTE_ANIMAL_THREE://��ע����
		{
			//			m_PlayView.m_NoteAnimal.ShowWindow(SW_SHOW);
			m_PlayView.NoteAnimal(3);
			break;
		}
	case ID_COLLECT_JETON_ANIMAL://�ռ����붯��
		{
			//			m_PlayView.m_NoteAnimal.ShowWindow(SW_SHOW);
			m_PlayView.CollectJetonAnimal();

			break;
		}
	//case ID_SEND_CARD_ANIMAL:
	//	{
	//		m_PlayView.SendCardAnimal(m_bShowAnimalStation);
	//		break;
	//	}

	default:
		break;
	}
	CLoveSendClassInExe::OnTimer(nIDEvent);
}


void CClientGameDlg::OnCancel()
{

	/*if(GetComType() == TY_MONEY_GAME)
		wsprintf(sz,"�����˳����ᱻ�й�,�����Ҫ�˳���?",m_iRunPublish,m_iRunPublish * m_iRoomBasePoint);
	else
		wsprintf(sz,"�����˳����ᱻ��������עX5��ׯ����۳������м���עX5,�����Ҫ�˳���?",m_iRunPublish);*/

	//char cKey[10];
	//CString sPath=CBcfFile::GetAppPath();
	//int iResult = 1;
	//if (CBcfFile::IsFileExist(/*sPath + */"..\\SpecialRule.bcf"))
	//{
	//	CBcfFile fsr( /*sPath + */"..\\SpecialRule.bcf");
	//	wsprintf(cKey, "%d", NAME_ID);
	//	iResult = fsr.GetKeyVal (_T("ForceQuitAsAuto"), cKey, 0);
	//}

	//if (iResult)
	//{

	//	wsprintf(sz,"�����˳����ᱻ��������עX5��ׯ����۳������м���עX5,�����Ҫ�˳���?");
	//}
	//else
	//{
	//	wsprintf(sz,"��ע���˳����ᱻ�й�,�����Ҫ�˳���");
	//}
	

	//��ѯ״̬
	if (!m_bWatchMode && GetStationParameter()>=GS_SEND_CARD&&GetStationParameter() < GS_WAIT_NEXT && m_bPlayer)
	{
		TCHAR sz[200];
		char cKey[10];
		//CString sPath=CBcfFile::GetAppPath();
		int iResult = 1;
		if (CBcfFile::IsFileExist( "..\\SpecialRule.bcf"))
		{
			CBcfFile fsr( "..\\SpecialRule.bcf");
			sprintf(cKey, "%d", NAME_ID);
			iResult = fsr.GetKeyVal (_T("ForceQuitAsAuto"), cKey, 0);
		}

		if (iResult)
			wsprintf(sz,TEXT("��������Ϸ�У������˳���Ϸ�Զ��й�.�����Ҫ�˳���?"));

		else
		{
			if (m_pGameInfo->uComType == 1)
			{
				wsprintf(sz,TEXT("��������Ϸ�У������˳����ᱻ�ۻ��֣��Ƿ�Ҫ�˳���Ϸ��"));
			}
			else
			{
				wsprintf(sz,TEXT("��������Ϸ�У������˳����ᱻ�۽�ң��Ƿ�Ҫ�˳���Ϸ��"));
			}
		}


		if (IDOK!=AFCMessageBox(sz,m_pGameInfo->szGameName,AFC_YESNO)) //|MB_DEFBUTTON2|MB_ICONINFORMATION,this)) 
			return;
		SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	}

	m_bPlayer        = FALSE;
	KillAllTimer();
	AFCCloseFrame();
	//	if(hmciwnd)
	//		MCIWndDestroy(hmciwnd);
	__super::OnCancel();
}

//�޸��û�Ѻע���
BOOL CClientGameDlg::ModifyNote(BYTE iViewStation,int iCurNote,BOOL bExtVal)
{
	//׷�ӱ���ĳ��Ѻע���
	m_PlayView.AddNotePoint(iViewStation,iCurNote);
	//��ע�ۼ�
//	m_PlayView.AddTotalNote(0,iCurNote);
	return true;
}

////�û�Ѻע����
//BOOL CClientGameDlg::SetNoteSound(BYTE bNoteStyle,BOOL bIsBoy)
//{
//	TCHAR str[MAX_PATH+1]="";
//	TCHAR filename[MAX_PATH];//path[MAX_PATH];
//	CString s=CINIFile::GetAppPath ();/////����·��
//	CINIFile f( s + "cardsound.ini");
//	CString key = TEXT("wznnnote");
//	//	TCHAR path[MAX_PATH];
//	CString folder,extendname,name;
//	folder=f.GetKeyVal(key,"folder","music\\cardwav\\wznn\\");
//	extendname=f.GetKeyVal(key,"extendname",".wav");
//	TCHAR Val[50];
//	srand(GetCurrentTime());
//	int count =f.GetKeyVal(key,"count",1);
//	switch(bNoteStyle)
//	{
//		/*case TYPE_OPENCARD:
//		{	
//		wsprintf(Val,"%c(open%d)",bIsBoy?'b':'g',rand()%count);
//		name = f.GetKeyVal(key,Val,Val);
//		break;
//		}
//		case TYPE_FOLLOW:
//		{
//		wsprintf(Val,"%c(follow%d)",bIsBoy?'b':'g',rand()%count);
//		name = f.GetKeyVal(key,Val,Val);
//		break;
//		}
//		case TYPE_BULL:
//		{
//		wsprintf(Val,"%c(add%d)",bIsBoy?'b':'g',rand()%count);
//		name = f.GetKeyVal(key,Val,Val);
//		break;
//		}*/
//	case TYPE_NOTE:
//		{
//			wsprintf(Val,"%c(note%d)",bIsBoy?'b':'g',rand()%count);
//			name = f.GetKeyVal(key,Val,Val);
//			break;
//		}
//	case TYPE_GIVE_UP:
//		{
//			wsprintf(Val,"%c(pass%d)",bIsBoy?'b':'g',rand()%count);
//			name = f.GetKeyVal(key,Val,Val);
//			break;
//		}	
//	default:
//		{
//			wsprintf(str,"%c(def)",bIsBoy?'b':'g');
//			name = f.GetKeyVal(key,Val,Val);
//			break;
//		}
//
//	}
//	wsprintf(filename,"%s%s%s",folder,name,extendname);
//	PlayOutCardType(filename,m_hMciWnd1);
//	return true;
//}
//
//BOOL CClientGameDlg::SetPlaySound(BYTE bSoundStyle,BOOL bExtVal)
//{
//	CString s=CINIFile::GetAppPath ();/////����·��
//	CINIFile f( s + "cardsound.ini");
//	CString key = TEXT("system");
//	CString folder,filename;
//	folder=f.GetKeyVal(key,"folder","music\\cardwav\\");
//	TCHAR fi[MAX_PATH+1];
//	switch(bSoundStyle)
//	{
//	case SOUND_WARNING:
//		{
//			filename = f.GetKeyVal(key,"warning","warning");
//			break;
//		}
//	case SOUND_INVALIDATE:
//		{
//			filename = f.GetKeyVal(key,"invalidate","invalidate");
//			break;
//		}
//	case SOUND_SEND_CARD:
//		{
//			filename = f.GetKeyVal(key,"sendcard","sendcard");
//			break;
//		}
//	}
//	wsprintf(fi,"%s%s%s",folder,filename,".wav");
//	::PlaySound(fi,NULL,SND_FILENAME|SND_ASYNC|SND_NOSTOP|SND_NOWAIT|SND_NODEFAULT);
//	return true;
//}
//
////���ó�����������
//BOOL CClientGameDlg::SetOutCardTypeSound(BYTE iCardList[],int iCardCount,BOOL IsBoy)
//{
//	int cardshape = m_Logic.GetCardShape(iCardList,iCardCount);
//	//	int cardstart,cardend;
//	//	TCHAR filename[MAX_PATH],
//	TCHAR str[100]="";
//	TCHAR filename[MAX_PATH];//path[MAX_PATH];
//	CString s=CINIFile::GetAppPath ();/////����·��
//	CINIFile f( s + "cardsound.ini");
//	CString key = TEXT("type0");
//	//	TCHAR path[MAX_PATH];
//	CString folder,extendname,name;
//	folder=f.GetKeyVal(key,"folder","music\\cardwav\\type0\\");
//	extendname=f.GetKeyVal(key,"extendname",".mp3");
//
//
//	if(IsBoy)
//	{
//		wsprintf(str,"b(def)");
//	}
//	else
//		wsprintf(str,"g(def)");
//
//	name=f.GetKeyVal(key,str,str);
//	wsprintf(filename,"%s%s%s",folder,name,extendname);
//
//	PlayOutCardType(filename,m_hMciWnd1);
//
//	//::PlaySound(filename,NULL,SND_FILENAME|SND_ASYNC|SND_NOWAIT|SND_NODEFAULT);
//
//	return TRUE;
//}
////���ų�������
//BOOL CClientGameDlg::SetSenceSound(BYTE iSendSound,BOOL IsBoy,int iVal)
//{
//	TCHAR str[100]="";
//	TCHAR filename[MAX_PATH];//path[MAX_PATH];
//	CString s=CINIFile::GetAppPath ();/////����·��
//	CINIFile f( s + "cardsound.ini");
//	CString key = TEXT("cardsence");
//	//	TCHAR path[MAX_PATH];
//	CString folder,extendname,name;
//	folder=f.GetKeyVal(key,"folder","music\\cardwav\\sence\\");
//	extendname=f.GetKeyVal(key,"extendname",".mp3");
//
//	//	TCHAR filename[MAX_PATH],str[100]="";
//	srand(GetCurrentTime());
//	int count;
//	switch(iSendSound)
//	{
//	case SOUND_WIN://ʤ��
//		if(IsBoy)
//		{	
//			count = f.GetKeyVal(key,"bwincount",10);
//			wsprintf(str,"b(win%d)",rand()%count);
//		}
//		else
//		{
//			count = f.GetKeyVal(key,"gwincount",10);
//			wsprintf(str,"g(win%d)",rand()%count);
//		}
//		break;
//	case SOUND_LOSE://ʧ��
//		if(IsBoy)
//		{
//			count = f.GetKeyVal(key,"blosecount",10);
//			wsprintf(str,"b(lose%d)",rand()%count);
//		}
//		else
//		{
//			count = f.GetKeyVal(key,"glosecount",10);
//			wsprintf(str,"g(lose%d)",rand()%count);
//		}
//		break;
//	case SOUND_WAITING://�ȴ�
//		if(IsBoy)
//		{
//			count = f.GetKeyVal(key,"bwaitcount",10);
//			wsprintf(str,"b(wait%d)",rand()%count);
//		}
//		else
//		{
//			count = f.GetKeyVal(key,"gwaitcount",10);
//			wsprintf(str,"g(wait%d)",rand()%count);
//		}
//		break;
//	case SOUND_ERROR://���Ƴ�����
//		if(IsBoy)
//		{
//			count = f.GetKeyVal(key,"berrorcount",10);
//			wsprintf(str,"b(error%d)",rand()%count);
//		}
//		else
//		{
//			count = f.GetKeyVal(key,"gerrorcount",10);
//			wsprintf(str,"g(error%d)",rand()%count);
//		}
//		break;
//	case SOUND_READY://׼��
//		if(IsBoy)
//		{
//			count = f.GetKeyVal(key,"breadycount",10);
//			wsprintf(str,"b(ready%d)",rand()%count);
//		}
//		else
//		{
//			count = f.GetKeyVal(key,"greadycount",10);
//			wsprintf(str,"g(ready%d)",rand()%count);
//		}
//		break;
//	case SOUND_START://��ʼ��Ϸ(���)
//		if(IsBoy)
//		{
//			count = f.GetKeyVal(key,"bstartcount",10);
//			wsprintf(str,"b(start%d)",rand()%count);
//		}
//		else
//		{
//			count = f.GetKeyVal(key,"gstartcount",10);
//			wsprintf(str,"g(start%d)",rand()%count);
//		}
//		break;
//	case SOUND_JIAO_FEN://�з�
//		{
//			if(IsBoy)
//				wsprintf(str,"b(jiaofen%d)",iVal);
//			else
//				wsprintf(str,"g(jiaofen%d)",iVal);
//		}
//	case SOUND_OUT_CARD_FINISH://������
//		{
//			if(IsBoy)
//			{
//				count = f.GetKeyVal(key,"boutcardfinishcount",10);
//				wsprintf(str,"b(outcardfinish%d)",rand()%count);
//			}
//			else
//			{
//				count = f.GetKeyVal(key,"goutcardfinishcount",10);
//				wsprintf(str,"g(outcardfinish%d)",rand()%count);
//			}
//			break;
//		}
//	case SOUND_FRIEND_APPEAR://���ѳ��F
//		{
//			if(IsBoy)
//			{
//				count = f.GetKeyVal(key,"bfriendcount",10);
//				wsprintf(str,"b(friend%d)",rand()%count);
//			}
//			else
//			{
//				count = f.GetKeyVal(key,"gfriendcount",10);
//				wsprintf(str,"g(friend%d)",rand()%count);
//			}
//			break;
//		}
//	case SOUND_USER_RUNAWARY:	//����
//		{
//			if(IsBoy)
//			{
//				count = f.GetKeyVal(key,"brunawarycount",10);
//				wsprintf(str,"b(runawary%d)",rand()%count);
//			}
//			else
//			{
//				count = f.GetKeyVal(key,"grunawarycount",10);
//				wsprintf(str,"g(runawary%d)",rand()%count);
//			}
//			break;
//		}
//	case SOUND_USER_LEFT:	//�����뿪
//		{
//			if(IsBoy)
//			{
//				count = f.GetKeyVal(key,"bleftcount",10);
//				wsprintf(str,"b(left%d)",rand()%count);
//			}
//			else
//			{
//				count = f.GetKeyVal(key,"gleftcount",10);
//				wsprintf(str,"g(left%d)",rand()%count);
//			}
//			break;
//		}
//	case SOUND_GIVE_BACK://���
//		{
//			if(IsBoy)
//			{
//				count = f.GetKeyVal(key,"bgivebackcount",10);
//				wsprintf(str,"b(giveback%d)",rand()%count);
//			}
//			else
//			{
//				count = f.GetKeyVal(key,"ggivebackcount",10);
//				wsprintf(str,"g(giveback%d)",rand()%count);
//			}
//			break;
//		}
//	default:
//		if(IsBoy)
//			wsprintf(str,"b(sence_0)");
//		else
//			wsprintf(str,"g(sence_0)");
//		break;
//	}
//	name=f.GetKeyVal(key,str,str);
//	wsprintf(filename,"%s%s%s",folder,name,extendname);
//	PlayOutCardType(filename,m_hMciWnd2);
//	return TRUE;
//}
////����Mp3����
//BOOL CClientGameDlg::PlayOutCardType(TCHAR FileName[],HWND hmciwnd)
//{
//	if(hmciwnd)
//	{
//		MCIWndStop(hmciwnd);
//		MCIWndDestroy(hmciwnd);
//		hmciwnd=NULL;
//	}
//
//	//hmciwnd = MCIWndCreate(this->m_hWnd,AfxGetInstanceHandle(),WS_CHILD,FileName);
//
//	hmciwnd=MCIWndCreate(this->m_hWnd,AfxGetInstanceHandle(),WS_CHILD|MCIWNDF_NOMENU|MCIWNDF_NOOPEN|MCIWNDF_NOPLAYBAR|MCIWNDF_NOERRORDLG,FileName);
//	if(hmciwnd)
//		MCIWndPlay(hmciwnd);	
//	return true;
//}
////��������
BOOL CClientGameDlg::CheckVersion(BYTE iVersion,BYTE iVersion2)
{	
	TCHAR GameName[200];
	CString strBuffer;
	wsprintf(GameName,"���� %s ��Ϸ�汾����,��Ҫ�������������°汾��?",m_pGameInfo->szGameName);
	if(iVersion!=m_iVersion || iVersion2!=m_iVersion2)
	{
		TCHAR sz[200];
		wsprintf(sz,"�汾��ͻ:��ǰ�汾%d-%d,����˰汾%d-%d",m_iVersion,m_iVersion2,iVersion,iVersion2);
	}
	if(iVersion!=m_iVersion)
	{

		if (IDOK==AFCMessageBox(GameName,m_pGameInfo->szGameName,AFC_YESNO)) //|MB_DEFBUTTON1|MB_ICONINFORMATION,this)) 
		{
			strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strDownLoadSetupADDR),1,1);
			ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		}
		PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}

	if(iVersion2!=m_iVersion2)
	{
		if (IDOK==AFCMessageBox(GameName,m_pGameInfo->szGameName,AFC_YESNO)) //|MB_DEFBUTTON1|MB_ICONINFORMATION,this)) 
		{
			strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strDownLoadSetupADDR),1,1);
			ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		}
		PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}

	return TRUE;
}
//�û���ע
LRESULT CClientGameDlg::OnShowNote(WPARAM wparam,LPARAM lparam)
{
	m_PlayView.m_NtList.ShowWindow(TRUE);
	m_PlayView.m_NtList.SetNumList(m_iThisTurnLimit, m_iBaseNote);				//������ע��Ŀ
	return TRUE;
}
//�û�ѡ����ע��
LRESULT CClientGameDlg::OnSelectNote(WPARAM wparam,LPARAM lparam)
{
	int ret = (int)wparam;
	if ((int)lparam == 15 )
	{
		m_PlayView.m_NtList.ShowWindow(SW_HIDE);

		return true;
	}
	if(ret<=0)
		ret=0;
	m_PlayView.SetDrawNote();
	m_PlayView.m_NtList.ShowWindow(SW_HIDE);

	KillAllTimer();

	__int64 Num = 0;
	switch(ret)
	{
	case 1:
		if (m_PlayView.m_GameType == 1)              //��Ϸ���ࡣ0Ϊ��ͨ��1Ϊͬϲ��
		{
			Num = m_PlayView.m_iNoteLimit/100*100;
		}
		else
		{
			Num = m_PlayView.m_iNoteLimit;
		}
		break;
	case 2:
		if (m_PlayView.m_GameType == 1)              //��Ϸ���ࡣ0Ϊ��ͨ��1Ϊͬϲ��
		{
			Num = m_PlayView.m_iNoteLimit/125*100;
		}
		else
		{
			if (m_PlayView.m_iGameNoteSetType>0)
			{
				Num = m_PlayView.m_iNoteLimit/2;
			}
			else
			{
				Num = m_PlayView.m_iNoteLimit/1.25;
			}

		}
		break;
	case 3:

		if (m_PlayView.m_GameType == 1)              //��Ϸ���ࡣ0Ϊ��ͨ��1Ϊͬϲ��
		{
			Num = m_PlayView.m_iNoteLimit/250*100;
		}
		else
		{
			if (m_PlayView.m_iGameNoteSetType>0)
			{
				Num = m_PlayView.m_iNoteLimit/4;
			}
			else
			{
				Num = m_PlayView.m_iNoteLimit/2.5;
			}

		}
		break;
	case 4:

		if (m_PlayView.m_GameType == 1)              //��Ϸ���ࡣ0Ϊ��ͨ��1Ϊͬϲ��
		{
			Num = m_PlayView.m_iNoteLimit/500*100;
		}
		else
		{
			if (m_PlayView.m_iGameNoteSetType>0)
			{
				Num = m_PlayView.m_iNoteLimit/8;
			}
			else
			{
				Num =m_PlayView.m_iNoteLimit/5;
			}

		}
		break;
	}
	//Msg("zhtlog:Num=%I64d ret=%d m_PlayView.m_GameType =%d m_PlayView.m_iGameNoteSetType=%d",Num,ret,m_PlayView.m_GameType,m_PlayView.m_iGameNoteSetType );
	tagUserProcess process;
	process.iVrebType =(int)lparam;//ret;				//��ע����
	process.iNote=Num;						//��ע��(ֻ����ע�ͼ�ע�����ô�ֵ)
	if (GetStationParameter() ==GS_PLAY_GAME)
		SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_VREB_CHECK,0);
	SendGameData(&process,sizeof(process),MDM_GM_GAME_NOTIFY,ASS_BEGIN_UPGRADE,0);
	return TRUE;
}


//���ƶ�����ʾ��Ϻ�,��ʾ������
LRESULT CClientGameDlg::SendCardShowCard(WPARAM wparam,LPARAM lparam)
{
	BYTE iViewStation=(BYTE)wparam;
	//	m_PlayView.m_UserCard[m_PlayView.m_flag].SetCard(m_DeskCard[m_PlayView.m_flag],NULL,m_iDeskCardCount[m_PlayView.m_flag]);

	//ϵͳ����Ա��ʾ���ҵ���
	/*if(m_pGameInfo->pMeUserInfo->GameUserInfo.bGameMaster==MAST_PEOPLE)
		m_PlayView.m_UserCard[iViewStation].SetCard(m_UserCard[iViewStation],NULL,m_iDeskCardCount[iViewStation]);*/
	return 0;
#ifdef SUPER_VERSION
	m_PlayView.m_UserCard[iViewStation].SetCard(m_UserCard[iViewStation],NULL,m_iDeskCardCount[iViewStation]);

#endif
}

//�Ñ��Q����
LRESULT CClientGameDlg::ChangeBackCard()
{
	if (GetStationParameter()==GS_PLAY_GAME&&!m_bWatchMode)
	{
		ChangeBackStruct change;
		change.bDeskStation=GetMeUserInfo()->bDeskStation ;
		change.bSourceCard=m_UserCard[GetMeUserInfo()->bDeskStation][0];
		change.bResultCard=0x3d;
		SendGameData(&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_GM_SUPERCLIENT,0);
	}
	return 0;
}

//���Ż��㵽��ͼλ�� zxj 2009-11-21 ��ע�Ͳ��޸�����
BYTE CClientGameDlg::ViewStation(BYTE bDeskStation)
{
	return __super::ViewStation(bDeskStation);	//����ԭ��ע�͵��Ĳ��֣���Ӵ˾䰴ť
//	BYTE bViewStation=m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation;
//	int temp=((PLAY_COUNT/2)-bViewStation+bDeskStation);
//	if(temp>=0)
//		return temp%PLAY_COUNT;
//	else
//		return PLAY_COUNT-1;
//switch(bViewStation)
//	{
//	case 0: { 
//		if(bDeskStation==0)	
//			return 3;
//		if(bDeskStation==1)
//			return 4;
//		if(bDeskStation==2)
//			return 5;
//		if(bDeskStation==3)
//			return 0;
//		if(bDeskStation==4)
//			return 1;
//		if(bDeskStation==5)
//			return 2;
//		break;
//			}
//	case 1: {
//		if(bDeskStation==0)	
//			return 2;
//		if(bDeskStation==1)
//			return 3;
//		if(bDeskStation==2)
//			return 4;
//		if(bDeskStation==3)
//			return 5;
//		if(bDeskStation==4)
//			return 0;
//        if(bDeskStation==5)
//			return 1;
//		break;
//			}
//	case 2: { 
//		if(bDeskStation==0)	
//			return 1;
//		if(bDeskStation==1)
//			return 2;
//		if(bDeskStation==2)
//			return 3;
//		if(bDeskStation==3)
//			return 4;
//		if(bDeskStation==4)
//			return 5;
//		if(bDeskStation==5)
//			return 0;
//		break;
//			}
//	case 3: { 
//		if(bDeskStation==0)	
//			return 0;
//		if(bDeskStation==1)
//			return 1;
//		if(bDeskStation==2)
//			return 2;
//		if(bDeskStation==3)
//			return 3;
//		if(bDeskStation==4)
//			return 4;
//		if(bDeskStation==5)
//			return 5;
//		break;
//			}	
//	case 4: { 
//		if(bDeskStation==0)	
//			return 5;
//		if(bDeskStation==1)
//			return 0;
//		if(bDeskStation==2)
//			return 1;
//		if(bDeskStation==3)
//			return 2;
//		if(bDeskStation==4)
//			return 3;
//		if(bDeskStation==5)
//			return 4;
//		break;
//			}
//	case 5:
//		{
//		if(bDeskStation==0)	
//			return 4;
//		if(bDeskStation==1)
//			return 5;
//		if(bDeskStation==2)
//			return 0;
//		if(bDeskStation==3)
//			return 1;
//		if(bDeskStation==4)
//			return 2;
//		if(bDeskStation==5)
//			return 3;
//		break;
//		}
//	default:
//		break;
//    }
//	return 0;
}
//�Q���ƽY��
BOOL CClientGameDlg::ChangeBackCardResult(BYTE bDeskStation,BYTE bSourceBackCard,BYTE bResultCard,BOOL bExtVal)
{
	if(!bExtVal)
		return true;
	m_UserCard[bDeskStation][0]=bResultCard;
	/*m_PlayView.m_UserCard[bDeskStation].SetCard(m_UserCard[bDeskStation],NULL,m_iDeskCardCount[bDeskStation]);
	ReplaceCard(m_SysCard,0,m_iSysCardCount,bSourceBackCard,bResultCard);

	m_PlayView.m_SysBackCard.SetCard(m_SysCard,NULL,m_iSysCardCount);*/
	return true;
}
//ϵ�y�������Ƿ���ĳ����
BOOL CClientGameDlg::ReplaceCard(BYTE iTotalCard[],int iSendCardPos,int iEndPos,BYTE bSourceCard,BYTE bResultCard)
{
	//�Q����
	for(int i=iSendCardPos;i<iEndPos;i++)
	{
		if(iTotalCard[i]==bResultCard)
		{
			iTotalCard[i]=bSourceCard;
			return true;
		}
	}
	return false;
}

//������������ע
BOOL CClientGameDlg::UserSupersedeNote(BYTE bDeskStation)
{
	KillAllTimer();
	if (GetStationParameter() == GS_PLAY_GAME || GetStationParameter() == GS_SEND_CARD)
	{
		if (bDeskStation==GetMeUserInfo()->bDeskStation)
			return 0L;
		SupersedeUserNote supNote;
		supNote.bDeskStation = bDeskStation;	//������עλ��
		supNote.iVrebType = 0;					//��ע����
		supNote.iNote = 0;						//��ע��(ֻ����ע�ͼ�ע�����ô�ֵ)
		if (GetStationParameter() == GS_PLAY_GAME)
			supNote.iVrebType = TYPE_OPEN;
		SendGameData(&supNote,sizeof(supNote),MDM_GM_GAME_NOTIFY,ASS_SUPERSEDE_VREB_CHECK,0);
	}
	return TRUE;
}

void CClientGameDlg::DelShowThis()
{
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		m_PlayView.m_UserCard[i].SetCardShowNum(0);
	}
}
//Ϊ��Ƶģ�����ش˺���
#ifdef VIDEO
bool CClientGameDlg::GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	if (! __super::GameUserCome(bDeskStation, pUserItem, bWatchUser))
	{
		return FALSE;
	}

	if( GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID && bWatchUser )
	{
		m_PlayView.m_bWatch=bWatchUser;
	}
	if( GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID && (!bWatchUser) )
	{    
		CPlayVideo *pPlayVideo=CPlayVideo::CreatePlayVideo();


		if(NULL!=pPlayVideo)
		{ 

			CString NickName=TEXT("");
			int    byteDeskNO=0;
			UserInfoStruct &UInfo=	pUserItem->GameUserInfo;
			NickName+=UInfo.nickName;
			byteDeskNO=UInfo.bDeskNO;
			UINT	uiRoomID = m_pGameInfo->uRoomID;
			UINT	uiBRRoomID =   uiRoomID * 1000 + byteDeskNO; 

			ST_INITVIDEO stInitVideo;
			stInitVideo.iRoomPassType = 0;
			stInitVideo.nRoomID		  = uiBRRoomID;
			stInitVideo.pParentWnd    = &m_PlayView;
			stInitVideo.strRoomPass   = _T("");
			stInitVideo.strUserName	  = NickName;
			stInitVideo.strUserPass   = _T("");
			if( pPlayVideo->Video_Initialize(stInitVideo) )
			{
				//			
				//sprintf(sz,"��Ƶ��ʼ���ɹ�!");
				//Writelog(sz,1);
				////������Ƶ������

				pPlayVideo->Connect();

			}
			else
			{
				pPlayVideo->Video_Release();
				/*	sprintf(sz,"��Ƶ��ʼ��ʧ��!");
				Writelog(sz,1);*/
				return 0;
			}


		} 
	}
	return TRUE;
}
bool CClientGameDlg::AFCCloseFrame()
{
	///�ͷ���Ƶ��Դ 
	if( NULL != CPlayVideo::GetPlayVideo())
		CPlayVideo::GetPlayVideo()->Video_Release(); 
	return __super::AFCCloseFrame();
}
#endif
//���ؽ�ׯ��ť
BOOL CClientGameDlg::HideButton()
{
	m_PlayView.m_btCallScore1.ShowWindow(SW_HIDE);
	m_PlayView.m_btCallScorePass.ShowWindow(SW_HIDE);
	return true;
}
//�з�
void CClientGameDlg::ShowCallScoreBt(int CallScore,BOOL bExtVol)
{
	m_PlayView.m_btCallScore1.ShowWindow(SW_SHOW);
	m_PlayView.m_btCallScorePass.ShowWindow(SW_SHOW);
	m_PlayView.m_btCallScorePass.EnableWindow(1);
	m_PlayView.m_btCallScore1.EnableWindow(TRUE);
	return;
}
//�з�
LRESULT	CClientGameDlg::OnCallScore(WPARAM wparam, LPARAM lparam)
{

	KillTimer(ID_CALL_SCORE_TIME);
	HideButton();
	CallScoreStruct score;
	memset(&score,0,sizeof(score));
	score.iValue = (int)wparam;	
	if(m_pGameInfo->bEnableSound)
	{
		if(score.iValue == 0)	//������ׯ
		{
			SetPlaySound(SOUND_GIVEUP_NT);
		}
		else
		{
			SetPlaySound(SOUND_CALL_NT);
		}
	}
	score.bDeskStation = GetMeUserInfo()->bDeskStation;			
	score.bCallScoreflag = false;
	SendGameData(&score,sizeof(score),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE,0);

	return 0;
}

void CClientGameDlg::SetRecordView() 
{ 
	m_PlayView.m_btContinue.ShowWindow(SW_HIDE); 
	m_PlayView.m_btLeave.ShowWindow(SW_HIDE); 
	m_PlayView.m_btReset.ShowWindow(SW_HIDE); 
	m_PlayView.m_btOpen.ShowWindow(SW_HIDE);
	m_PlayView.m_btBull.ShowWindow(SW_HIDE);
	m_PlayView.m_btCallScorePass.ShowWindow(SW_HIDE); 
	m_PlayView.m_btCallScore1.ShowWindow(SW_HIDE); 

	m_PlayView.m_btContinue.EnableWindow(FALSE); 
	m_PlayView.m_btLeave.EnableWindow(FALSE); 
	m_PlayView.m_btReset.EnableWindow(FALSE); 
	m_PlayView.m_btOpen.EnableWindow(FALSE);
	m_PlayView.m_btBull.EnableWindow(FALSE);
	m_PlayView.m_btCallScorePass.EnableWindow(FALSE); 
	m_PlayView.m_btCallScore1.EnableWindow(FALSE); 
} 
BYTE CClientGameDlg::GetNetUserPos(BYTE iDesk)
{	
	for(int i = 0 ;i <PLAY_COUNT ;i++)
	{		
		BYTE iStation = (iDesk + 1);

		if(NULL != m_pUserInfo[iStation] &&m_iDeskCardCount[ViewStation(iStation)]>0)
		{
			return iStation;
		}
	}
	return 255;
}



