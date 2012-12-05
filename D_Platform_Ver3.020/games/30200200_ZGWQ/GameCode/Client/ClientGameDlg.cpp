#include "StdAfx.h"
#include "ClientGameDlg.h"
#include "GameSet.h"
//#include ".\clientgamedlg.h"
#include "playcardsound.h"

BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClassInExe)
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(IDM_BEGIN,OnHitBegin)						//��ʼ
	ON_MESSAGE(IDM_LEFT_HITCARD,OnHitCard)					//
	ON_MESSAGE(IDM_HAVE_THING,OnHitHaveThing)				//����Ҫ��
	ON_MESSAGE(IDM_SEND_THING,OnSendHaveThing)				//��������Ҫ��
	ON_MESSAGE(IDM_ARGEE_LEAVE,OnArgeeUserLeft)				//ͬ���뿪
	ON_MESSAGE(IDM_STOP,OnStop)								//���
	ON_MESSAGE(IDM_STOP_THING,OnStopThing)					//������ǰ�����¼�
	ON_MESSAGE(IDM_ARGEE_STOP,OnAgreeStop)					//ͬ����ǰ�����¼�
	ON_MESSAGE(IDM_ADMIT_DEFEAT,OnAdmitDefeat)				//����
	ON_MESSAGE(IDM_REGRET_CHESS,OnRegretChess)				//����
	ON_MESSAGE(IDM_CHESS_MANUAL,OnChessManual)				//����
	ON_MESSAGE(IDM_SUPER_TIMER,OnSuperTimer)				//��ʱ����
	ON_MESSAGE(IDM_SEND_REN_SHU,OnSendAdmitDefeat)			//ȷ������
	ON_MESSAGE(IDM_AGREE_REGRET,OnAgreeRegret)				//ͬ�����
	ON_MESSAGE(IDM_FINISH_DIAN_MU,OnFinishDianMu)			//������Ŀ
	ON_MESSAGE(IDM_AGREE_DIAN_MU,OnAgreeDianMu)				//ͬ���Ŀ
	ON_MESSAGE(IDM_NUMBER_ZI,OnNumberZi)					//ǿ������
	ON_MESSAGE(IDM_ASSURANCE_NUM_ZI,OnAssuranceNumZi)		//ȷ��ǿ������
	ON_MESSAGE(IDM_MOVE_ONCARD,OnMouseMoveCard)				//����ƶ���������
	ON_MESSAGE(IDM_ASSURANCE_DIAN_MU,OnAssuranceDianMu)		//ȷ����Ŀ
	ON_WM_TIMER()
#ifdef _USE_VIDEO
	ON_MESSAGE(WM_GV_ENTERROOM,&CClientGameDlg::OnGVClientEnterRoom)
	ON_MESSAGE(WM_GV_USERATROOM,&CClientGameDlg::OnGVClientUserAtRoom)
	ON_MESSAGE(WM_GV_ONLINEUSER,&CClientGameDlg::OnGVClientOnlineUser)
	ON_MESSAGE(WM_GV_LOGINSYSTEM,&CClientGameDlg::OnGVClientLogin)
#endif
	ON_MESSAGE(IDM_GAMESETTING_OK,OnGameSettingOK)
END_MESSAGE_MAP()


__int64 GetPowerofGold()
{
	__int64 nMoneyTemp = 1;
	for(int i=0; i<Glb().m_nPowerOfGold; i++)
	{
		nMoneyTemp*=10;
	}

	return nMoneyTemp;
}

//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClassInExe(&m_PlayView)
{
	m_bNoHaveAgreeRegret = false;
	m_iVersion = DEV_HEIGHT_VERSION;
	m_iVersion2 = DEV_LOW_VERSION;
	m_GameSet = NULL;
	m_iNowOutPeople = -1;
	m_iEndI = m_iEndJ = -1;
	m_iStartI = m_iStartJ = -1;
	m_iMeChessFlag = 0;
	m_iCurPlayFlag = 1;
	m_bIsInDianZi = false;
	m_bCanPlay = true;

	memset(m_iDianMuQiZi,0,sizeof(m_iDianMuQiZi));

	for (int i = 0; i < WQ_MAX_I; i ++)
		for(int j = 0; j < WQ_MAX_J; j ++)
		{
			if (i == 0 || i == WQ_MAX_I - 1 || j == 0 || j == WQ_MAX_J - 1)
				m_iMeBoard[i][j] = WQ_FLAG_SIDE;
			else m_iMeBoard[i][j] = WQ_FLAG_NONE;
		}

		//	m_bTimeOutCount = 0;
		//��ʼʱ��
		m_iBeginTime = 15;

}

//��������
CClientGameDlg::~CClientGameDlg()
{
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
	//���±���
	SetDialogIcon(LoadIcon(GetModuleHandle(CLIENT_DLL_NAME),MAKEINTRESOURCE(IDI_CLIENT_ROOM)),TRUE);
	m_PlayView.m_Result.m_nPowerOfGold=Glb().m_nPowerOfGold;

	//	m_PlayView.m_bRightPower=GetProfileInt(TEXT("RightPower"),FALSE);
#ifdef _USE_VIDEO
	CString  strVideoCfg=CINIFile::GetAppPath()+TEXT("VideoCfg.ini");
	if(!CINIFile::IsFileExist(strVideoCfg))
	{
		return 0;
	}
	CINIFile iniFile(strVideoCfg);
	CString serverip = iniFile.GetKeyVal(TEXT("VideoCfg"),TEXT("VideoServerIP"), "127.0.0.1");
	int port = iniFile.GetKeyVal(TEXT("VideoCfg"),TEXT("VideoServerPort"), 8906);

	m_GameVideoSet = MyGameVideo(serverip,port);

	if(m_pBRGameVideoSDK || 
		m_PlayView.GetSafeHwnd() == NULL)
		return 0;

	m_pBRGameVideoSDK = new CBRGameVideoSDK;
	if(m_pBRGameVideoSDK->InitGameVideoSDK(this))
	{
		return 0;
	}
	else
	{
		m_pBRGameVideoSDK->CloseGameVideoSDK();
		delete m_pBRGameVideoSDK;
		m_pBRGameVideoSDK = NULL;
	}
#endif	
	return TRUE;
}

//������Ϸ״̬
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
		{
			GameStation_1 * pGameStation=(GameStation_1 *)pStationData;
			//�汾�˶�
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);			
			//
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;		//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;				//���ܿ۷�
			m_iRunPublishMoney = pGameStation->iRunPublishMoney ;	//���ܿ۽��

			if(!m_bWatchMode)
			{
				m_PlayView.m_btLeave.EnableWindow(!m_bWatchMode);
				m_PlayView.m_GameSet.SetGameSetInfo(0);
				m_PlayView.m_GameSet.CenterWindow(&m_PlayView);
			}

#ifdef _USE_VIDEO
			RefreshAllVideoPanel();
#endif
			return TRUE;
		}
	case GS_WAIT_ARGEE:	//�ȴ�ͬ��״̬
		{
			GameStation_2 * pGameStation=(GameStation_2 *)pStationData;
			//�汾�˶�
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);			

			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;		//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;				//���ܿ۷�
			m_iRunPublishMoney = pGameStation->iRunPublishMoney ;	//���ܿ۽��

			m_iJuTimeMinute = pGameStation->iJuTimeMinute;
			m_iJuTimeSecond = pGameStation->iJuTimeSecond;
			m_iBuTimeMinute = pGameStation->iBuTimeMinute;
			m_iBuTimeSecond = pGameStation->iBuTimeSecond;
			m_iDuMiaoMinute = pGameStation->iDuMiaoMinute;
			m_iDuMiaoSecond = pGameStation->iDuMiaoSecond;
			if(!m_bWatchMode)
			{
				m_PlayView.m_GameInfo.SetGameSetInfo(m_iJuTimeMinute,m_iJuTimeSecond,
					m_iBuTimeMinute,m_iBuTimeSecond,m_iDuMiaoMinute,m_iDuMiaoSecond);
				m_PlayView.m_GameInfo.CenterWindow(&m_PlayView);
			}


#ifdef _USE_VIDEO
			RefreshAllVideoPanel();
#endif

			return TRUE;
		}
	case GS_PLAY_GAME:	//��Ϸ������
		{
			GameStation_4 * pGameStation = (GameStation_4 *)pStationData;
			//�汾�˶�
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);			
			//
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;		//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;				//���ܿ۷�
			m_iRunPublishMoney = pGameStation->iRunPublishMoney ;	//���ܿ۽��

			KillAllTimer();
			::CopyMemory(m_PlayView.m_MeCard.m_iQiZiNum,pGameStation->iQiZiNum,sizeof(pGameStation->iQiZiNum));
			::CopyMemory(m_iMeBoard,pGameStation->iQiZi,sizeof(pGameStation->iQiZi));
			if(pGameStation->iEndI!=-1)
				//m_Logic.CanAddQiZi(pGameStation->iEndI,pGameStation->iEndJ,pGameStation->m_iLastDeskStation,m_iMeBoard);
				m_Logic.CanAddQiZi(pGameStation->iEndI,pGameStation->iEndJ,m_iNowOutPeople,m_iMeBoard);

			::CopyMemory(m_PlayView.m_MeCard.m_iQiZiNum,pGameStation->iQiZiNum,sizeof(pGameStation->iQiZiNum));
			::CopyMemory(m_iMeBoard,pGameStation->iQiZi,sizeof(pGameStation->iQiZi));

			m_iJuTimeMinute = pGameStation->iJuTimeMinute;
			m_iJuTimeSecond = pGameStation->iJuTimeSecond;
			m_iBuTimeMinute = pGameStation->iBuTimeMinute;
			m_iBuTimeSecond = pGameStation->iBuTimeSecond;
			m_iDuMiaoMinute = pGameStation->iDuMiaoMinute;
			m_iDuMiaoSecond = pGameStation->iDuMiaoSecond;
			//jph 100127
			m_PlayView.m_MeCard.m_iBotBuUseTime=pGameStation->m_stUsedTimeInfo.m_iBuUseTime[GetMeUserInfo()->bDeskStation];
			m_PlayView.m_MeCard.m_iBotJuUseTime=pGameStation->m_stUsedTimeInfo.m_iJuUseTime[GetMeUserInfo()->bDeskStation];
			m_PlayView.m_MeCard.m_iBotDuUseTime=pGameStation->m_stUsedTimeInfo.m_iDuUseTime[GetMeUserInfo()->bDeskStation];
			m_PlayView.m_MeCard.m_iTopBuUseTime=pGameStation->m_stUsedTimeInfo.m_iBuUseTime[(GetMeUserInfo()->bDeskStation+1)%2];
			m_PlayView.m_MeCard.m_iTopJuUseTime=pGameStation->m_stUsedTimeInfo.m_iJuUseTime[(GetMeUserInfo()->bDeskStation+1)%2];
			m_PlayView.m_MeCard.m_iTopDuUseTime=pGameStation->m_stUsedTimeInfo.m_iDuUseTime[(GetMeUserInfo()->bDeskStation+1)%2];

			m_iNowOutPeople=pGameStation->iOutCardPeople;
			m_iMeChessFlag=GetMeUserInfo()->bDeskStation==pGameStation->iFirstOutPeople?WQ_PLAY_FLAG_BLACK:WQ_PLAY_FLAG_WHITE;
			m_Qiflag[GetMeUserInfo()->bDeskStation]=m_iMeChessFlag;
			m_Qiflag[(GetMeUserInfo()->bDeskStation+1)%2]=(m_iMeChessFlag+1)%2;
			m_PlayView.m_MeCard.SetPlayTime(m_iJuTimeMinute,m_iJuTimeSecond,m_iBuTimeMinute,m_iBuTimeSecond,m_iDuMiaoMinute,m_iDuMiaoSecond);
			m_PlayView.m_MeCard.SetChess(m_iMeChessFlag,m_Qiflag[m_iNowOutPeople],m_iMeBoard);
			m_PlayView.m_MeCard.SetTiZiNum(pGameStation->iTiZiNum);
			m_PlayView.m_MeCard.SetTimerCount();
			//m_bIsInDianZi = FALSE;
			//m_bCanPlay = true;
			//�˽׶�Ϊͬ���Ŀ�׶�
			if (pGameStation->isHitAgreedianmu>0&&!m_bWatchMode)
			{
				if (m_PlayView.m_DianMu.GetSafeHwnd() == NULL)
				{
					m_PlayView.m_DianMu.Create(IDD_DIAN_MU,this);
					m_PlayView.m_DianMu.m_pParent = this;
				}
				m_PlayView.m_DianMu.SetWindowText(TEXT("��Ŀ"));
				TCHAR path[MAX_PATH] = {0};
				wsprintf(path,TEXT("image\\%s\\dianmu.bmp"),SKIN_FOLDER);
				m_PlayView.m_DianMu.m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
				wsprintf(path,TEXT("image\\%s\\agree_bt.bmp"),SKIN_FOLDER);
				m_PlayView.m_DianMu.m_btOk.LoadButtonBitmap(path,false);
				m_PlayView.m_DianMu.m_btOk.MoveWindow(50,100,46,20);
				m_PlayView.m_DianMu.m_btOk.ShowWindow(SW_SHOW);

				wsprintf(path,TEXT("image\\%s\\oppose.bmp"),SKIN_FOLDER);
				m_PlayView.m_DianMu.m_btCancel.LoadButtonBitmap(path,false);
				m_PlayView.m_DianMu.m_btCancel.MoveWindow(150,100,46,20);
				m_PlayView.m_DianMu.m_btContinueChess.ShowWindow(SW_HIDE);

				m_PlayView.m_DianMu.SetMessage(TEXT("�Լ������Ŀ����ͬ����?"),50,60);
				m_PlayView.m_DianMu.CenterWindow(&m_PlayView);
				m_PlayView.m_DianMu.ShowWindow(SW_SHOW);
				m_PlayView.m_DianMu.SetFocus();
				return true;

			}
			if ((m_iNowOutPeople == GetMeUserInfo()->bDeskStation) && (m_bWatchMode == FALSE))
			{	
				m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
				m_PlayView.m_btChessManual.EnableWindow(!m_bWatchMode);
				m_PlayView.m_btAdmitDefeat.EnableWindow(!m_bWatchMode);
				m_PlayView.m_btStop.EnableWindow(!m_bWatchMode);
				m_PlayView.m_btRegretChess.EnableWindow(FALSE);
				m_PlayView.m_btThing.EnableWindow(!m_bWatchMode);
			}
			else
			{
				m_PlayView.m_MeCard.SetUseMouse(false);
				m_PlayView.m_btChessManual.EnableWindow(FALSE);
				m_PlayView.m_btAdmitDefeat.EnableWindow(FALSE);
				m_PlayView.m_btStop.EnableWindow(FALSE);
				m_PlayView.m_btRegretChess.EnableWindow(!m_bWatchMode&&pGameStation->iEndI!=-1);
				m_PlayView.m_btThing.EnableWindow(FALSE);
			}
			CString CST;
			CST.Format("wyshdebug m_iOutCardPeople===%d,m_iLastDeskStation===%d,�Լ�===%d",m_iNowOutPeople,pGameStation->m_iLastDeskStation,GetMeUserInfo()->bDeskStation);
			OutputDebugString(CST);

			//////////////////////////////////////////////////////////////////////////
			// ����״̬
			if (pGameStation->bIsInHuiquStation[GetMeUserInfo()->bDeskStation] )
			{
				m_PlayView.m_btRegretChess.EnableWindow(false);
				m_PlayView.m_GameNoteView.SetMessage(TEXT ("���ڵȴ��Լ�ͬ��..."));
				m_PlayView.m_GameNoteView.CenterWindow(&m_PlayView);
				m_PlayView.m_GameNoteView.ShowWindow(SW_SHOW);
			}
			if (pGameStation->bChessingInHuiQi[GetMeUserInfo()->bDeskStation])
			{
				m_PlayView.m_btAdmitDefeat.EnableWindow(FALSE);
				m_PlayView.m_btStop.EnableWindow(FALSE);
				m_PlayView.m_btChessManual.EnableWindow(FALSE);
			}

			if (pGameStation->bIsInHuiquStation[ (GetMeUserInfo()->bDeskStation+1)%2 ])
			{
				m_bNoHaveAgreeRegret = true;
				if (m_PlayView.m_RegretChess.GetSafeHwnd()==NULL) 
				{
					m_PlayView.m_RegretChess.Create(IDD_DLG_HUI_QI,this);
					m_PlayView.m_RegretChess.m_pParnet=this;
				}
				m_PlayView.m_RegretChess.CenterWindow(&m_PlayView);
				m_PlayView.m_RegretChess.ShowWindow(SW_SHOW);
			}

#ifdef _USE_VIDEO
			RefreshAllVideoPanel();
#endif
			return TRUE;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ�̿�ʼ
		{
			GameStation_5 * pGameStation=(GameStation_5 *)pStationData;
			//�汾�˶�
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);			
			//
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;		//���䱶��
			m_iRunPublish = pGameStation->iRunPublish ;				//���ܿ۷�
			m_iRunPublishMoney = pGameStation->iRunPublishMoney ;	//���ܿ۽��

			//	m_PlayView.m_StationView.SetShowMessage(TRUE);
			//			m_PlayView.m_MeCard.SetShowCard(!m_bWatchMode||m_bWatchOther);

			//��ʾ��ʼ��ť 
			if (GetMeUserInfo()->bUserState!=USER_ARGEE)
			{
				if (!m_bWatchMode)
				{
					m_PlayView.m_btContinue.EnableWindow(!m_bWatchMode);
					m_PlayView.m_btLeave.EnableWindow(!m_bWatchMode);
					m_PlayView.m_btContinue.SetFocus();
				}
				SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
			}

#ifdef _USE_VIDEO
			RefreshAllVideoPanel();
#endif

			return TRUE;
		}
	}
	return false;
}

//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	if (pNetHead->bMainID==MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{
		case ASS_UG_USER_SET:		//�û������ƾ�
			{
				if (uDataSize!=sizeof(UserArgeeGame))
					return FALSE;
				SetStationParameter(GS_WAIT_ARGEE);
				UserArgeeGame * pGameSet=(UserArgeeGame *)pNetData;
				//��������
				m_iJuTimeMinute = pGameSet->iJuTimeMinute;
				m_iJuTimeSecond = pGameSet->iJuTimeSecond;
				m_iBuTimeMinute = pGameSet->iBuTimeMinute;
				m_iBuTimeSecond = pGameSet->iBuTimeSecond;
				m_iDuMiaoMinute = pGameSet->iDuMiaoMinute;
				m_iDuMiaoSecond = pGameSet->iDuMiaoSecond;
				//��ʾ����
				if(!m_bWatchMode)
				{
					if(m_PlayView.m_GameSet.GetSafeHwnd() != NULL)
						m_PlayView.m_GameSet.DestroyWindow();					
					m_PlayView.m_GameNoteView.SetMessage(NULL);
					m_PlayView.m_GameInfo.SetGameSetInfo(m_iJuTimeMinute,m_iJuTimeSecond,m_iBuTimeMinute,m_iBuTimeSecond,m_iDuMiaoMinute,m_iDuMiaoSecond);
					m_PlayView.m_GameInfo.CenterWindow(&m_PlayView);
				}
				return TRUE;
			}
		case ASS_GAME_PREPARE://˭����
			{
				if (uDataSize!=sizeof(PrepareStruct)) return FALSE;
				PrepareStruct * pPrepare=(PrepareStruct *)pNetData;
				if(m_pUserInfo[pPrepare->bDeskStation] != NULL)
				{
					wsprintf(m_szMSG,TEXT("[��ʾ]��� %s ִ����!"),m_pUserInfo[pPrepare->bDeskStation]->GameUserInfo.nickName);
					m_MessageHandle.InsertNormalMessage(m_szMSG);
				}
				return TRUE;
			}
		case ASS_GAME_PLAY:		//��ʼ������Ϸ
			{
				if (uDataSize!=sizeof(BeginPlayStruct)) return FALSE;
				BeginPlayStruct * pBeginInfo=(BeginPlayStruct *)pNetData;

				if (m_bWatchMode) 
				{
					KillAllTimer();
					ResetGameStation(RS_GAME_BEGIN);
				}

				m_PlayView.m_btThing.EnableWindow(!m_bWatchMode);

				m_PlayView.m_GameNoteView.SetMessage(NULL);
				m_PlayView.m_btStop.EnableWindow(FALSE);
				m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
				//				m_PlayView.m_MeCard.SetShowCard(!m_bWatchMode||m_bWatchOther);

				//����ִ�Ӻ�or��
				m_iMeChessFlag = (GetMeUserInfo()->bDeskStation == pBeginInfo->bDeskStation ? WQ_PLAY_FLAG_BLACK:WQ_PLAY_FLAG_WHITE);
				m_Qiflag[GetMeUserInfo()->bDeskStation] = m_iMeChessFlag;
				m_Qiflag[(GetMeUserInfo()->bDeskStation + 1) % 2]= (m_iMeChessFlag+1) % 2;
				//m_bTimeOutCount=0;
				SetStationParameter(GS_PLAY_GAME);
				m_iNowOutPeople = pBeginInfo->bDeskStation;

				if (m_iNowOutPeople == GetMeUserInfo()->bDeskStation) 
				{
					m_iMeChessFlag = WQ_PLAY_FLAG_BLACK;
				}
				else
				{
					m_iMeChessFlag = WQ_PLAY_FLAG_WHITE;
				}
				m_iCurPlayFlag = WQ_PLAY_FLAG_BLACK;

				m_PlayView.m_MeCard.SetPlayTime(m_iJuTimeMinute,m_iJuTimeSecond,m_iBuTimeMinute,m_iBuTimeSecond,m_iDuMiaoMinute,m_iDuMiaoSecond);
				m_PlayView.m_MeCard.SetChess(m_iMeChessFlag,m_Qiflag[m_iNowOutPeople],m_iMeBoard);
				m_PlayView.m_MeCard.SetTimerCount();
				if ((m_iNowOutPeople == GetMeUserInfo()->bDeskStation) && (m_bWatchMode == FALSE))
				{	
					m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
					m_PlayView.m_btChessManual.EnableWindow(FALSE);
					m_PlayView.m_btAdmitDefeat.EnableWindow(TRUE);
					m_PlayView.m_btStop.EnableWindow(TRUE);
					m_PlayView.m_btRegretChess.EnableWindow(FALSE);
					m_PlayView.m_btThing.EnableWindow(TRUE);
				}
				else
				{
					m_PlayView.m_MeCard.SetUseMouse(FALSE);
					m_PlayView.m_btChessManual.EnableWindow(FALSE);
					m_PlayView.m_btAdmitDefeat.EnableWindow(FALSE);
					m_PlayView.m_btStop.EnableWindow(FALSE);
					m_PlayView.m_btRegretChess.EnableWindow(FALSE);
					m_PlayView.m_btThing.EnableWindow(FALSE);
				}

				m_PlayView.m_GameNoteView.SetMessage(NULL);
				//	SetGameTimer(m_iNowOutPeople,m_iLessCount,ID_OUT_CARD);
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_START_DEF);
				//		PlaySound(TEXT("BEGIN_GAME"),GetModuleHandle(CLIENT_DLL_NAME),SND_RESOURCE|SND_NOSTOP|SND_ASYNC|SND_NODEFAULT|SND_NOWAIT);

				return TRUE;
			}
			//case ASS_SUPER_TIMER://��ʱ����
			//	{
			//		KillAllTimer();
			//		m_iNowOutPeople = pNetHead->bHandleCode;			
			//		m_PlayView.m_MeCard.SetChess(m_iMeChessFlag,m_Qiflag[m_iNowOutPeople],m_iMeBoard);
			//		m_PlayView.m_MeCard.SetTimerCount();
			//		if ((m_iNowOutPeople == GetMeUserInfo()->bDeskStation) && (m_bWatchMode == FALSE))
			//		{	
			//			m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
			//			m_PlayView.m_btChessManual.EnableWindow(TRUE);
			//			m_PlayView.m_btAdmitDefeat.EnableWindow(TRUE);
			//			m_PlayView.m_btStop.EnableWindow(TRUE);
			//			m_PlayView.m_btRegretChess.EnableWindow(FALSE);
			//			m_PlayView.m_btThing.EnableWindow(TRUE);
			//		}
			//		else
			//		{
			//			m_PlayView.m_MeCard.SetUseMouse(false);
			//			m_PlayView.m_btChessManual.EnableWindow(FALSE);
			//			m_PlayView.m_btAdmitDefeat.EnableWindow(FALSE);
			//			m_PlayView.m_btStop.EnableWindow(FALSE);
			//			m_PlayView.m_btRegretChess.EnableWindow(TRUE&&!m_bWatchMode);
			//			m_PlayView.m_btThing.EnableWindow(FALSE);
			//		}
			//		return true;
			//	}
		case ASS_OUT_CARD_RESULT:		//���ӽ��
			{
				PlayChessMsg * pPlayChessInfo=(PlayChessMsg *)pNetData;
				KillAllTimer();
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_OUT_CARD);
				//PlaySound(TEXT("OUT_CARD"),GetModuleHandle(CLIENT_DLL_NAME),SND_RESOURCE|SND_NOSTOP|SND_ASYNC|SND_NODEFAULT|SND_NOWAIT);
				int CurDesk = pPlayChessInfo->iPlayFlag;
				m_Logic.CanAddQiZi(pPlayChessInfo->iEndI,pPlayChessInfo->iEndJ,pPlayChessInfo->bDeskStation,m_iMeBoard);
				::CopyMemory(m_PlayView.m_MeCard.m_iQiZiNum,pPlayChessInfo->iQiZiNum,sizeof(pPlayChessInfo->iQiZiNum));
				::CopyMemory(m_iMeBoard,pPlayChessInfo->iChessArray,sizeof(pPlayChessInfo->iChessArray));
				m_PlayView.m_MeCard.SetTiZiNum(pPlayChessInfo->iTiZiNum);
				m_PlayView.m_MeCard.SetUseMouse(false);
				m_PlayView.m_btChessManual.EnableWindow(FALSE);
				m_PlayView.m_btAdmitDefeat.EnableWindow(FALSE);
				m_PlayView.m_btStop.EnableWindow(FALSE);
				m_PlayView.m_btRegretChess.EnableWindow(TRUE&&!m_bWatchMode);
				m_PlayView.m_btThing.EnableWindow(FALSE);
				return true;
			}
		case ASS_OUT_CARD://����
			{
				PlayChessStruct * pPlay=(PlayChessStruct *)pNetData;
				KillAllTimer();

				m_iNowOutPeople = pPlay->bDeskStation;
				m_PlayView.m_MeCard.SetChess(m_iMeChessFlag,m_Qiflag[m_iNowOutPeople],m_iMeBoard);
				m_PlayView.m_MeCard.SetTimerCount();
				if ((m_iNowOutPeople == GetMeUserInfo()->bDeskStation) && (m_bWatchMode == FALSE))
				{	
					m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
					m_PlayView.m_btChessManual.EnableWindow(TRUE);
					m_PlayView.m_btAdmitDefeat.EnableWindow(TRUE);
					m_PlayView.m_btStop.EnableWindow(TRUE);
					m_PlayView.m_btRegretChess.EnableWindow(FALSE);
					m_PlayView.m_btThing.EnableWindow(TRUE);
				}

				return TRUE;
			}
		case ASS_HUI_QI://����
			{
				m_bNoHaveAgreeRegret =true;
				if (m_PlayView.m_RegretChess.GetSafeHwnd()==NULL) 
				{
					m_PlayView.m_RegretChess.Create(IDD_DLG_HUI_QI,this);
					m_PlayView.m_RegretChess.m_pParnet=this;
				}
				m_PlayView.m_RegretChess.CenterWindow(&m_PlayView);
				m_PlayView.m_RegretChess.ShowWindow(SW_SHOW);				
				return true;
			}

		case ASS_AGREE_HUI_QI://ͬ�����
			{
				m_bNoHaveAgreeRegret =false;
				int flag = (int)pNetHead->bHandleCode;
				if (flag == 0)	//��ͬ��
				{
					m_PlayView.m_GameNoteView.SetMessage(NULL);
					m_MessageHandle.InsertNormalMessage("�ԼҲ�ͬ����塣");
					return true;
				}

				if (uDataSize != sizeof(RegretChessMsg)) return false;

				m_PlayView.m_GameNoteView.SetMessage(NULL);
				RegretChessMsg *regretInfo = (RegretChessMsg *) pNetData;

				m_iNowOutPeople=regretInfo->iLastDeskStation;

				::CopyMemory(m_iMeBoard,regretInfo->iLastChessArray,sizeof(m_iMeBoard));

				m_PlayView.m_MeCard.SetChess(m_iMeChessFlag,m_Qiflag[m_iNowOutPeople],m_iMeBoard);
				m_PlayView.m_MeCard.SetTiZiNum(regretInfo->iLastTiZiNum);
				m_PlayView.m_btRegretChess.EnableWindow(false);			
				if ((m_iNowOutPeople == GetMeUserInfo()->bDeskStation) && (m_bWatchMode == FALSE))
				{
					m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);

				}
				else
				{
					m_PlayView.m_MeCard.SetUseMouse(m_bWatchMode);
				}

				if (m_iNowOutPeople != GetMeUserInfo()->bDeskStation)
				{
					m_PlayView.m_btChessManual.EnableWindow(FALSE);
					m_PlayView.m_btAdmitDefeat.EnableWindow(FALSE);
					m_PlayView.m_btStop.EnableWindow(FALSE);
				}

				return true;
			}
		case ASS_DIAN_MU://��Ŀ
			{
				if (m_bWatchMode)
				{
					return true;
				}
				if (m_PlayView.m_DianMu.GetSafeHwnd() == NULL)
				{
					m_PlayView.m_DianMu.Create(IDD_DIAN_MU,this);
					m_PlayView.m_DianMu.m_pParent = this;
				}
				m_PlayView.m_DianMu.SetWindowText(TEXT("��Ŀ"));
				TCHAR path[MAX_PATH] = {0};
				wsprintf(path,TEXT("image\\%s\\dianmu.bmp"),SKIN_FOLDER);
				m_PlayView.m_DianMu.m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
				wsprintf(path,TEXT("image\\%s\\agree_bt.bmp"),SKIN_FOLDER);
				m_PlayView.m_DianMu.m_btOk.LoadButtonBitmap(path,false);
				m_PlayView.m_DianMu.m_btOk.MoveWindow(50,100,46,20);
				m_PlayView.m_DianMu.m_btOk.ShowWindow(SW_SHOW);

				wsprintf(path,TEXT("image\\%s\\oppose.bmp"),SKIN_FOLDER);
				m_PlayView.m_DianMu.m_btCancel.LoadButtonBitmap(path,false);
				m_PlayView.m_DianMu.m_btCancel.MoveWindow(150,100,46,20);
				m_PlayView.m_DianMu.m_btContinueChess.ShowWindow(SW_HIDE);

				m_PlayView.m_DianMu.SetMessage(TEXT("�Լ������Ŀ����ͬ����?"),50,60);
				m_PlayView.m_DianMu.CenterWindow(&m_PlayView);

				m_PlayView.m_DianMu.ShowWindow(SW_SHOW);

				m_PlayView.m_DianMu.SetFocus();
				return true;
			}

		case ASS_AGREE_DIAN_MU://ͬ���Ŀ
			{
				if (uDataSize != sizeof(DianMuResultStruct)) return false;
				if (m_bWatchMode)
				{
					return true;
				}
				DianMuResultStruct *dianMuResult = (DianMuResultStruct *) pNetData;

				if (dianMuResult->bAgree)
				{
					m_bIsInDianZi = true;
					m_bCanPlay = false;

					::CopyMemory(m_iDianMuQiZi,m_iMeBoard,sizeof(m_iMeBoard));
					float tatalblack,totalwhite;
					m_Logic.NumQiZi(&tatalblack,&totalwhite,m_iDianMuQiZi);
					m_PlayView.m_MeCard.SetChess(m_iMeChessFlag,m_Qiflag[m_iNowOutPeople],m_iDianMuQiZi);	
					::memset(m_Logic.m_shizi,0,sizeof(m_Logic.m_shizi));
					m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
					m_MessageHandle.InsertNormalMessage(TEXT("��ʼ��Ŀ�������������ӣ�Ȼ����<��ɵ�Ŀ>��ť��"));
					m_PlayView.m_btFinishDianMu.EnableWindow(true);

				}
				else
				{
					if (m_PlayView.m_DianMu.GetSafeHwnd() == NULL)
					{
						m_PlayView.m_DianMu.Create(IDD_DIAN_MU,this);
						m_PlayView.m_DianMu.m_pParent = this;
					}
					CRect rect;
					m_PlayView.m_DianMu.SetWindowText(TEXT("��Ŀ"));
					m_PlayView.m_DianMu.GetClientRect(&rect);
					m_PlayView.m_DianMu.m_btOk.ShowWindow(SW_HIDE);

					TCHAR path[MAX_PATH] = {0};
					wsprintf(path,TEXT("image\\%s\\ok.bmp"),SKIN_FOLDER);
					m_PlayView.m_DianMu.m_btCancel.LoadButtonBitmap(path,false);
					m_PlayView.m_DianMu.m_btCancel.MoveWindow(95,100,46,20);
					m_PlayView.m_DianMu.SetMessage(TEXT("�ԼҲ�ͬ����������!"),50,60);
					m_PlayView.m_DianMu.CenterWindow(&m_PlayView);
					m_PlayView.m_DianMu.ShowWindow(SW_SHOW);
					m_PlayView.m_DianMu.SetFocus();
					if(m_Qiflag[m_iNowOutPeople] == m_iMeChessFlag)
					{
						m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
						m_PlayView.m_btChessManual.EnableWindow(!m_bWatchMode);
					}

				}
				return true;
			}

		case ASS_NUMBER_ZI://ǿ������
			{                
				if (m_bWatchMode)
				{
					return true;
				}
				FinishDianMu *mess = (FinishDianMu *) pNetData;
				CString s,s1;
				s.Format("%s",mess->msg);
				s1 =s + TEXT(" ��ͬ����������");
				m_PlayView.m_MeCard.SetChess(m_iMeChessFlag,m_Qiflag[m_iNowOutPeople],mess->qi);
				if (m_PlayView.m_DianMu.GetSafeHwnd() == NULL)
				{
					m_PlayView.m_DianMu.Create(IDD_DIAN_MU,this);
					m_PlayView.m_DianMu.m_pParent = this;
				}
				m_PlayView.m_DianMu.SetWindowText(TEXT("��Ŀ���"));

				TCHAR path[MAX_PATH] = {0};
				wsprintf(path,TEXT("image\\%s\\dianmuresult.bmp"),SKIN_FOLDER);
				m_PlayView.m_DianMu.m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
				wsprintf(path,TEXT("image\\%s\\agree_bt.bmp"),SKIN_FOLDER);
				m_PlayView.m_DianMu.m_btOk.LoadButtonBitmap(path,false);
				wsprintf(path,TEXT("image\\%s\\condianmu.bmp"),SKIN_FOLDER);
				m_PlayView.m_DianMu.m_btCancel.LoadButtonBitmap(path,false);
				wsprintf(path,TEXT("image\\%s\\xiaqi.bmp"),SKIN_FOLDER);
				m_PlayView.m_DianMu.m_btContinueChess.LoadButtonBitmap(path,false);
				m_PlayView.m_DianMu.SetMessage((LPTSTR)(LPCTSTR) s1,20,50);

				m_PlayView.m_DianMu.m_btOk.MoveWindow(30,100,46,20);
				m_PlayView.m_DianMu.m_btCancel.MoveWindow(90,100,46,20);
				m_PlayView.m_DianMu.m_btContinueChess.MoveWindow(160,100,46,20);

				m_PlayView.m_DianMu.m_btOk.ShowWindow(SW_SHOW);
				m_PlayView.m_DianMu.m_btCancel.ShowWindow(SW_SHOW);
				m_PlayView.m_DianMu.m_btContinueChess.ShowWindow(SW_SHOW);
				m_PlayView.m_DianMu.CenterWindow(&m_PlayView);
				m_PlayView.m_DianMu.ShowWindow(SW_SHOW);
				m_PlayView.m_DianMu.SetFocus();

				//m_PlayView.m_MeCard.SetFinishMessage(s,true);
				///AfxMessageBox("ǿ������");
				return true;
			}

		case ASS_DIAN_MU_RESULT://��Ŀ�������
			{
				if (m_bWatchMode)
				{
					return true;
				}
				int flag = (int) pNetHead->bHandleCode;
				if (flag == 1)//˫����ͬ����
				{
					MessageStruct *mess = (MessageStruct *) pNetData;
					CString s;
					s.Format("%s",mess->Message);
					m_PlayView.m_MeCard.SetFinishMessage(s,true);
					m_finishMsg=s;
				}
				else if (flag == 2)//������Ŀ
				{
					m_bIsInDianZi = true;
					m_bCanPlay = false;
					::memset(m_Logic.m_shizi,0,sizeof(m_Logic.m_shizi));
					::memcpy(m_iDianMuQiZi,m_iMeBoard,sizeof(m_iMeBoard));
					float tatalblack,totalwhite;
					m_Logic.NumQiZi(&tatalblack,&totalwhite,m_iDianMuQiZi);
					m_PlayView.m_MeCard.SetChess(m_iMeChessFlag,m_Qiflag[m_iNowOutPeople],m_iDianMuQiZi);					
					m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
					m_PlayView.m_btFinishDianMu.EnableWindow(!m_bWatchMode);
					m_MessageHandle.InsertNormalMessage(TEXT("�ԼҲ�ͬ���Ŀ�����������Ŀ��"));
				}
				else if (flag == 3)//��������
				{
					m_bCanPlay = true;
					m_bIsInDianZi = false;
					m_MessageHandle.InsertNormalMessage(TEXT("�ԼҲ�ͬ���Ŀ���,��������."));
					m_PlayView.m_MeCard.SetChess(m_iMeChessFlag,m_Qiflag[m_iNowOutPeople],m_iMeBoard);	
					m_PlayView.m_btFinishDianMu.EnableWindow(false);
					if(m_Qiflag[m_iNowOutPeople]==m_iMeChessFlag)
					{
						m_PlayView.m_btChessManual.EnableWindow(!m_bWatchMode);
						m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
					}				
					else
					{
						m_PlayView.m_btChessManual.EnableWindow(false);
						m_PlayView.m_MeCard.SetUseMouse(false);
					}					
				}

				return true;
			}

		case ASS_SALE_END:						//��Ϸ��ȫ����
		case ASS_LOSER_END:						//����
		case ASS_SUPER_TIMER_END:				//��ʱ����
		case ASS_CUT_END:						//�û�ǿ���뿪
		case ASS_CONTINUE_END:					//��Ϸ�����н���
		case ASS_NO_CONTINUE_END:				//��������Ϸ
			{
				if (uDataSize!=sizeof(GameEndStruct)) return FALSE;
				GameEndStruct * pGameEnd=(GameEndStruct *)pNetData;
				GameFinishNotify finish;
				memset(&finish,0,sizeof(finish));
				KillGameTimer(0);
				//��겻����
				m_PlayView.m_MeCard.SetChess(m_iMeChessFlag,0,m_iMeBoard);
				m_PlayView.m_MeCard.SetUseMouse(FALSE);

				CString stime;
				stime.Format("%d",CTime::GetCurrentTime());
				int curtime=atoi(stime);
				CString str;

				BYTE gameWiner = pGameEnd->bDeskStation; //ʤ����
				if(m_pUserInfo[gameWiner]!=0)
				{
					//��ʾ��Ϣ
					TCHAR szMessage[100];
					memset(szMessage,0,sizeof(szMessage));
					switch(pNetHead->bAssistantID)//���н������ݶ�һ��
					{
					case ASS_SALE_END:
						wsprintf(szMessage,TEXT("��Ϸ������"));
						break;
					case ASS_CUT_END:
						wsprintf(szMessage,TEXT("���ڡ� %s ��ǿ���˳�����Ϸ������"),pGameEnd->szNickName[ pGameEnd->bDeskStation ]);
						for(int i = 0; i < PLAY_COUNT; i ++)
							m_PlayView.m_showcoredlg.SetUserScore(ViewStation(i),pGameEnd->iTurePoint[i],pGameEnd->iChangeMoney[i]);
						break;
					case ASS_LOSER_END:			//------------------���������ʾ��Ϣ����-------------------------
						wsprintf(szMessage,"���ڡ� %s �����䣬��Ϸ����",pGameEnd->szNickName[ pGameEnd->bDeskStation ]);
						for(int i = 0; i < PLAY_COUNT; i ++)
							m_PlayView.m_showcoredlg.SetUserScore(ViewStation(i),pGameEnd->iTurePoint[i],pGameEnd->iChangeMoney[i]);
						break;
					case ASS_SUPER_TIMER_END:	//----------��ҵ�Ŀ����Ϸ��������һ��ҳ�ʱʾ��ʼ�������½�����Ϸ����---------						
						wsprintf(szMessage,TEXT("���ڡ� %s ����ʱ�и�����Ϸ������"),pGameEnd->szNickName[ pGameEnd->bDeskStation ]);
						break;
					case ASS_CONTINUE_END:
					case ASS_NO_CONTINUE_END:
						{
							//���ְ��Ϸ���
							for(int i = 0; i < PLAY_COUNT; i ++)
								m_PlayView.m_showcoredlg.SetUserScore(ViewStation(i),pGameEnd->iTurePoint[i],pGameEnd->iChangeMoney[i]);
							break;
						}
					default:
						wsprintf(szMessage,TEXT("δ֪ԭ��,��Ϸ������"));
						break;
					}
					m_MessageHandle.InsertNormalMessage(szMessage);
				}
				//�����
				BYTE iDoublePointEffect = 0;
				for(int i = 0; i < PLAY_COUNT; i ++)
				{
					if(m_pUserInfo[i] == NULL)
					{
						memset(finish.name[i],0,sizeof(finish.name[i]));
						finish.iTurePoint[i]=0;
						finish.iMoney[i]=0;
						continue;
					}
					else
					{
						//if(m_pUserInfo[i]->GameUserInfo.iDoublePointTime>curtime && pGameEnd->iTurePoint[i]>0)
						//20081125 ˫�����ֿ�Ч��
						CString CST;
						iDoublePointEffect = (1<<i);
						if(m_pGameInfo->uComType == TY_MONEY_GAME)
						{
							if((iDoublePointEffect & pGameEnd->iDoublePointEffect)
								&& pGameEnd->iTurePoint[i] >0)
							{
								//m_PlayView.m_Result.GetNumString(pGameEnd->iChangeMoney[i],CST,Glb().m_nPowerOfGold,false);
								pGameEnd->iTurePoint[i]*=2;
								str.Format("%s �÷�: %I64d ��˫�����ֿ�Ч���� ���: %I64d",pGameEnd->szNickName[i],pGameEnd->iTurePoint[i],pGameEnd->iChangeMoney[i]*GetPowerofGold());
							}
							else
							{
								//m_PlayView.m_Result.GetNumString(pGameEnd->iChangeMoney[i],CST,Glb().m_nPowerOfGold,false);
								str.Format("%s �÷�: %I64d ���: %I64d",pGameEnd->szNickName[i],pGameEnd->iTurePoint[i],pGameEnd->iChangeMoney[i]*GetPowerofGold());
							}
						}
						else
						{
							if((iDoublePointEffect & pGameEnd->iDoublePointEffect)
								&& pGameEnd->iTurePoint[i] >0)
							{
								pGameEnd->iTurePoint[i]*=2;
								str.Format("%s �÷�: %I64d ��˫�����ֿ�Ч����",pGameEnd->szNickName[i],pGameEnd->iTurePoint[i]);
							}
							else
							{
								str.Format("%s �÷�: %I64d",pGameEnd->szNickName[i],pGameEnd->iTurePoint[i]);		
							}
						}

						m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));

						lstrcpy(finish.name[i],m_pUserInfo[i]->GameUserInfo.nickName);
						finish.iTurePoint[i] = pGameEnd->iTurePoint[i];
						finish.iMoney[i] = pGameEnd->iChangeMoney[i];	
					}
				}
				m_PlayView.m_Result.SetMessage(finish,GetMeUserInfo()->bDeskStation);
				m_PlayView.m_Result.ShowWindow(SW_SHOW);
				//�������ʾ�������
				if(m_pGameInfo->bEnableSound
					&& GetStationParameter() == GS_PLAY_GAME) //JMod-��Ϸ������Ч-20090531
				{
					SetPlaySound(SOUND_GAME_OVER);
				}

				SetStationParameter(GS_WAIT_ARGEE);
				KillAllTimer();
				ResetGameStation(RS_GAME_END);
				if(!m_bWatchMode)
				{
					m_PlayView.m_btContinue.EnableWindow(true);
					m_PlayView.m_btLeave.EnableWindow(true);
					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
				}
				//if(GetMeUserInfo()->bDeskStation==pGameEnd->bDeskStation)
				//	OnCancel();
				return TRUE;
			}
			//		case ASS_CONTINUE_END:	//��Ϸ����
			//		case ASS_NO_CONTINUE_END:
			//			{
			//				if (uDataSize!=sizeof(GameEndStruct)) return FALSE;
			//				GameEndStruct * pGameEnd=(GameEndStruct *)pNetData;
			//				KillGameTimer(0);
			//
			//				m_PlayView.m_MeCard.SetChess(m_iMeChessFlag,0,m_iMeBoard);
			//				//���÷��� 
			//
			//				m_PlayView.m_MeCard.SetUseMouse(FALSE);
			////				m_PlayView.m_MeCard.SetShowCard(TRUE);
			//				//�����������ʾ�ɼ�
			//
			//				CString msg;
			//				msg=TEXT("\n\n������Ϸ�����\n������������\n");
			//				if(!m_finishMsg.IsEmpty())
			//				{
			//					msg=msg+m_finishMsg+TEXT("\n");
			//				}
			//				CString str;
			//				for (int i=0;i<2;i++)
			//				{
			//					if (m_pUserInfo[i]!=NULL)
			//					{
			//						if(3!=GetComType())
			//							str.Format("%s �÷� %d",m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i]);
			//						if(3==GetComType())
			//							str.Format("%s �÷� %d ��� %d ",m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i],pGameEnd->iChangeMoney[i]);
			//						msg+=str;
			//						msg+=TEXT("\n");
			//						m_PlayView.m_showcoredlg.SetUserScore(ViewStation(i),pGameEnd->iTurePoint[i],pGameEnd->iChangeMoney[i]);
			//					}
			//				}
			//				msg+=TEXT("������������");
			//				m_PlayView.m_msgbox.ShowMsg(msg.GetBuffer(),20,true);
			//
			//				//��������
			//				if(m_pGameInfo->bEnableSound)
			//					PlaySound(TEXT("GAME_OVER"),GetModuleHandle(CLIENT_DLL_NAME),SND_RESOURCE|SND_NOSTOP|SND_ASYNC|SND_NODEFAULT|SND_NOWAIT);
			//				SetStationParameter(GS_WAIT_NEXT);
			//				ResetGameStation(RS_GAME_END);
			//				if(!m_bWatchMode)
			//				{
			//					m_PlayView.m_btContinue.EnableWindow(true);
			//					m_PlayView.m_btLeave.EnableWindow(true);
			//				}
			//				if (pNetHead->bAssistantID == ASS_CONTINUE_END)
			//					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
			//				return TRUE;
			//			}
		case ASS_HAVE_THING:	//�û������뿪
			{
				AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));	
				HaveThingStruct * pThing=(HaveThingStruct *)pNetData;
				if (m_PlayView.m_ThingMessage.GetSafeHwnd()==NULL) 
				{
					m_PlayView.m_ThingMessage.Create(IDD_THING_MESSAGE, &m_PlayView);
					m_PlayView.m_ThingMessage.m_pParent = &m_PlayView;
				}
				m_PlayView.m_ThingMessage.SetLeaveMessage(pThing->szMessage,m_pUserInfo[pThing->bDeskStation ]->GameUserInfo.szName);
				m_PlayView.m_ThingMessage.CenterWindow(&m_PlayView);
				m_PlayView.m_ThingMessage.ShowWindow(SW_SHOW);
				AfxSetResourceHandle(GetModuleHandle(NULL));
				return TRUE;
			}
		case ASS_GAME_READY:				//��Ϸ׼��
			{
				if (uDataSize!=sizeof(GameReadyStruct)) return FALSE;
				GameReadyStruct* pReady = (GameReadyStruct *)pNetData;
				if(GetMeUserInfo()->bDeskStation == pReady->iBlackStation)		//�Լ�Ϊ���ֻ��Ǻ���
				{
					m_iMeChessFlag = WQ_PLAY_FLAG_BLACK;
				}
				else
				{
					m_iMeChessFlag = WQ_PLAY_FLAG_WHITE; //pReady->iWhiteStation;
				}
				return TRUE;
			}
		case ASS_LEFT_RESULT:	//�����뿪��� 
			{
				if (uDataSize!=sizeof(LeaveResultStruct)) return FALSE;
				LeaveResultStruct * pLeftResult=(LeaveResultStruct *)pNetData;
				CString strBuffer;
				if (!pLeftResult->bArgeeLeave) 
				{
					strBuffer.Format(TEXT("��� %s ��ͬ����Ϸ������"),m_pUserInfo[pLeftResult->bDeskStation]->GameUserInfo.szName);
					m_MessageHandle.InsertNormalMessage(strBuffer.GetBuffer(MAX_PATH));
				}
				else
				{
					if(pLeftResult->bArgeeLeave&&GetMeUserInfo()->bDeskStation==pLeftResult->bDeskStation&&!m_bWatchMode)
					{
						OnCancel(); // JMod-ͬ�������뿪���Զ��뿪����-20090531
					}
				}
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
				if (m_PlayView.m_StopMessage.GetSafeHwnd()==NULL) 
				{
					m_PlayView.m_StopMessage.Create(IDD_STOP_MESSAGE,&m_PlayView);
					m_PlayView.m_StopMessage.m_pParent = &m_PlayView;
				}
				m_PlayView.m_StopMessage.CenterWindow(&m_PlayView);
				m_PlayView.m_StopMessage.ShowWindow(SW_SHOW);
				return true;
			}
		case ASS_AGREE_STOP://������ǰ�������
			{
				if (uDataSize!=sizeof(StopResultStruct))return FALSE;
				StopResultStruct *pStop=(StopResultStruct*)pNetData;
				if (pStop->bArgee) 
				{
					m_MessageHandle.InsertNormalMessage("�Լ�ͬ�������ǰ������");
				}
				else 
				{
					m_MessageHandle.InsertNormalMessage("�Լ���Ϊ���л�ʤ�Ļ��ᡣ");
				}
				return true;
			}

		case ASS_REN_SHU://����
			{
				int iStation = (int)pNetHead->bHandleCode;
				CString strBuffer;
				strBuffer.Format(TEXT("��� %s �Ѿ����䡣"),m_pUserInfo[iStation]->GameUserInfo.szName);
				m_MessageHandle.InsertNormalMessage(strBuffer.GetBuffer(MAX_PATH));
				return true;
			}
		case ASS_GET_GAME_USED_TIME:
			{

				UsedTimeInfo timeInfo;		

				timeInfo.m_iBuUseTime[GetMeUserInfo()->bDeskStation]=m_PlayView.m_MeCard.m_iBotBuUseTime;
				timeInfo.m_iDuUseTime[GetMeUserInfo()->bDeskStation]=m_PlayView.m_MeCard.m_iBotDuUseTime;
				timeInfo.m_iJuUseTime[GetMeUserInfo()->bDeskStation]=m_PlayView.m_MeCard.m_iBotJuUseTime;
				timeInfo.m_iBuUseTime[(GetMeUserInfo()->bDeskStation+1)%2]=m_PlayView.m_MeCard.m_iTopBuUseTime;
				timeInfo.m_iDuUseTime[(GetMeUserInfo()->bDeskStation+1)%2]=m_PlayView.m_MeCard.m_iTopDuUseTime;
				timeInfo.m_iJuUseTime[(GetMeUserInfo()->bDeskStation+1)%2]=m_PlayView.m_MeCard.m_iTopJuUseTime;
				SendGameData(&timeInfo,sizeof(timeInfo),MDM_GM_GAME_NOTIFY,ASS_GET_GAME_USED_TIME,0);
				return true;
			}

		}
	}

	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize);
}

//�������ý���
void CClientGameDlg::ResetGameFrame()
{
	m_bNoHaveAgreeRegret = false;
	m_iNowOutPeople=-1;

	m_PlayView.m_btAdmitDefeat.EnableWindow(FALSE);
	m_PlayView.m_btChessManual.EnableWindow(FALSE);
	m_PlayView.m_btRegretChess.EnableWindow(FALSE);
	m_PlayView.m_btStop.EnableWindow(FALSE);
	m_PlayView.m_btThing.EnableWindow(FALSE);
	m_PlayView.m_btFinishDianMu.EnableWindow(FALSE);

	m_PlayView.m_GameNoteView.SetMessage(NULL);

	if (m_PlayView.m_GameInfo.GetSafeHwnd()) m_PlayView.m_GameInfo.DestroyWindow();
	if (m_PlayView.m_HaveThing.GetSafeHwnd()) m_PlayView.m_HaveThing.DestroyWindow();
	if (m_PlayView.m_ThingMessage.GetSafeHwnd()) m_PlayView.m_ThingMessage.DestroyWindow();
	if (m_PlayView.m_GameSet.GetSafeHwnd()) m_PlayView.m_GameSet.DestroyWindow();
	if (m_PlayView.m_AdmitDefeat.GetSafeHwnd()) m_PlayView.m_AdmitDefeat.DestroyWindow();
	if (m_PlayView.m_ChessManual.GetSafeHwnd()) m_PlayView.m_ChessManual.DestroyWindow();
	if (m_PlayView.m_RegretChess.GetSafeHwnd()) m_PlayView.m_RegretChess.DestroyWindow();
	if (m_PlayView.m_DianMu.GetSafeHwnd()) m_PlayView.m_DianMu.DestroyWindow();
	
	//if (m_GameSet != NULL)
	//{
	//	if(m_GameSet->GetSafeHwnd())	
	//		m_GameSet->EndDialog(0);
	//}
	m_PlayView.m_Result.ShowWindow(SW_HIDE);
	return __super::ResetGameFrame();
}
//������Ϸ
void CClientGameDlg::ResetGameStation(int iGameStation)
{
	OutputDebugString("��Ϸ���¿�ʼ��");
	//��������
	m_bNoHaveAgreeRegret = false;
	m_iNowOutPeople=-1;
	::memset(m_iMeBoard,0,sizeof(m_iMeBoard));
	memset(m_iDianMuQiZi,0,sizeof(m_iDianMuQiZi));
	for (int i = 0;i < WQ_MAX_I;i++)
		for(int j = 0;j < WQ_MAX_J;j++)
		{
			if (i == 0 || i == WQ_MAX_I - 1 || j == 0 || j == WQ_MAX_J - 1)
				m_iMeBoard[i][j] = WQ_FLAG_SIDE;
			else m_iMeBoard[i][j] = WQ_FLAG_NONE;
		}
		m_PlayView.m_btRegretChess.EnableWindow(false);
		m_PlayView.m_MeCard.Init();
		m_bIsInDianZi = false;
		m_bCanPlay = true;
		m_finishMsg="";
		m_iEndI = m_iEndJ = -1;
		m_iStartI = m_iStartJ = -1;
		m_iCurPlayFlag = 1;	

		m_PlayView.m_btAdmitDefeat.EnableWindow(FALSE);
		m_PlayView.m_btChessManual.EnableWindow(FALSE);
		m_PlayView.m_btRegretChess.EnableWindow(FALSE);
		m_PlayView.m_btStop.EnableWindow(FALSE);
		m_PlayView.m_btThing.EnableWindow(FALSE);
		m_PlayView.m_btFinishDianMu.EnableWindow(FALSE);
		m_PlayView.m_btContinue.EnableWindow(false);
		m_PlayView.m_btLeave.EnableWindow(false);
		if (m_PlayView.m_GameInfo.GetSafeHwnd()) 
			m_PlayView.m_GameInfo.DestroyWindow();
		if (m_PlayView.m_HaveThing.GetSafeHwnd()) 
			m_PlayView.m_HaveThing.DestroyWindow();
		if (m_PlayView.m_ThingMessage.GetSafeHwnd()) 
			m_PlayView.m_ThingMessage.DestroyWindow();
		if (m_PlayView.m_AdmitDefeat.GetSafeHwnd()) 
			m_PlayView.m_AdmitDefeat.DestroyWindow();
		if (m_PlayView.m_ChessManual.GetSafeHwnd())
			m_PlayView.m_ChessManual.DestroyWindow();
		if (m_PlayView.m_RegretChess.GetSafeHwnd())
			m_PlayView.m_RegretChess.DestroyWindow();
		if (m_PlayView.m_DianMu.GetSafeHwnd())
			m_PlayView.m_DianMu.DestroyWindow();
		if (m_PlayView.m_StopMessage.GetSafeHwnd())
			m_PlayView.m_StopMessage.DestroyWindow();
		if (m_PlayView.m_StopThing.GetSafeHwnd())
			m_PlayView.m_StopThing.DestroyWindow();

		switch (iGameStation)
		{
		case RS_ALL_VIEW:
		case RS_GAME_CUT:
			{
				if (m_PlayView.m_GameSet.GetSafeHwnd())
				{
					m_PlayView.m_GameSet.DestroyWindow();
				}
				break;
			}
		case RS_GAME_BEGIN:
			{
				m_PlayView.m_Result.ShowWindow(SW_HIDE);
				m_PlayView.m_msgbox.ShowWindow(false);
				if (m_PlayView.m_GameSet.GetSafeHwnd())
					m_PlayView.m_GameSet.DestroyWindow();
				m_PlayView.m_MeCard.SetFinishMessage("",false);
				break;
			}
		}
		return;
}
//��ʼ��ť����
LRESULT	CClientGameDlg::OnHitBegin(WPARAM wparam, LPARAM lparam)
{	
	OnControlHitBegin();
	return 0;
}

//׼����ʼ
bool CClientGameDlg::OnControlHitBegin()
{
	if ((GetStationParameter() == GS_WAIT_SETGAME))//��Ϸ����
	{
		tagGameSetInfo * pGameSet = m_PlayView.m_GameSet.GetGameSetInfo();
		if (pGameSet != NULL)
		{
			m_iJuTimeMinute = pGameSet->iJuTimeMinute;
			m_iJuTimeSecond = pGameSet->iJuTimeSecond;
			m_iBuTimeMinute = pGameSet->iBuTimeMinute;
			m_iBuTimeSecond = pGameSet->iBuTimeSecond;
			m_iDuMiaoMinute = pGameSet->iDuMiaoMinute;
			m_iDuMiaoSecond = pGameSet->iDuMiaoSecond;

			UserArgeeGame Agree;
			Agree.iJuTimeMinute = pGameSet->iJuTimeMinute;
			Agree.iJuTimeSecond = pGameSet->iJuTimeSecond;
			Agree.iBuTimeMinute = pGameSet->iBuTimeMinute;
			Agree.iBuTimeSecond = pGameSet->iBuTimeSecond;
			Agree.iDuMiaoMinute = pGameSet->iDuMiaoMinute;
			Agree.iDuMiaoSecond = pGameSet->iDuMiaoSecond;
			SendGameData(&Agree,sizeof(Agree),MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
		}
		m_PlayView.m_btContinue.EnableWindow(false);
		m_PlayView.m_btLeave.EnableWindow(false);		
	}
	else if ((GetStationParameter() == GS_WAIT_NEXT)||(GetStationParameter() == GS_WAIT_ARGEE))
	{
		m_PlayView.m_btContinue.EnableWindow(false);
		m_PlayView.m_btLeave.EnableWindow(false);
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);		
	}
	//----------------------��ʾ�ȴ��жԻ���--------------------
	m_PlayView.m_GameNoteView.CenterWindow(&m_PlayView);
	m_PlayView.m_GameNoteView.ShowWindow(SW_SHOW);
	KillAllTimer();
	ResetGameStation(RS_GAME_BEGIN);

#ifdef _USE_VIDEO
	RefreshAllVideoPanel();
#endif

	return true;
}


//������ж�ʱ��
void CClientGameDlg::KillAllTimer()
{
	KillTimer(ID_BEGIN_TIME);
	KillTimer(ID_OUT_CARD);
	return;
}

//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case ID_BEGIN_TIME:	//������Ϸ��ʱ��
		{
			if (uTimeCount <= 1)
			{
				//ʱ�䵽��
				//OnHitBegin(0,0);
				//SetStationParameter(GS_WAIT_ARGEE);
				OnCancel(); // ʱ�䵽�Զ��˳�
				return true;
			}
			if (uTimeCount <= 5) 
			{
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_WAITING);
				//PlaySound(TEXT("WARNING"),GetModuleHandle(CLIENT_DLL_NAME),SND_RESOURCE|SND_NOSTOP|SND_ASYNC|SND_NODEFAULT|SND_NOWAIT);
			}

			return true;
		}

	case ID_OUT_CARD:	//����
		{
			int iViewStation=ViewStation(bDeskStation);
			//if (uTimeCount <= 1)	//ʱ�䵽��
			//{
			//	//�жϳ�ʱ
			//	if((m_iNowOutPeople==GetMeUserInfo()->bDeskStation) && (m_bWatchMode==FALSE))
			//		m_bTimeOutCount++;
			//	if (m_bTimeOutCount > 5 && (m_iNowOutPeople==GetMeUserInfo()->bDeskStation) && (m_bWatchMode==FALSE))
			//	{
			//		KillGameTimer(0);
			//		SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
			//		SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
			//		PostMessage(WM_CLOSE,0,0);
			//		AFCMessage(TEXT("��ʱ����̫�࣬��Ϸ������"),m_pGameInfo->szGameName,MB_ICONQUESTION,this);
			//		return false;
			//	}
			//	else if(m_iNowOutPeople == GetMeUserInfo()->bDeskStation && !m_bWatchMode)
			//	{
			//		CString str;
			//		str.Format("��ע�⣬���Ѿ���ʱ %d ���ˣ���ʱ��λᵱ�����ܴ���",m_bTimeOutCount);
			//		m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));
			//	}

			//	return true;
			//}
			if ((uTimeCount <= 6)&&(!m_bWatchMode)&&(iViewStation == 2)) 
			{
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_WAITING);
				//PlaySound(TEXT("WARNING"),GetModuleHandle(CLIENT_DLL_NAME),SND_RESOURCE|SND_NOSTOP|SND_ASYNC|SND_NODEFAULT|SND_NOWAIT);
			}
			return true;
		}
	}
	return true;
}

//�����˿˰�ť����
LRESULT	CClientGameDlg::OnHitCard(WPARAM wparam, LPARAM lparam)
{
	if(m_bWatchMode)
		return true;
	if (GetStationParameter() == GS_PLAY_GAME)	//����״̬
	{
		int iCurI = (int) wparam,
			iCurJ = (int) lparam;
		//����
		if (m_bIsInDianZi)
		{			
			BOOL bDianZi = m_Logic.IsIllegalDianMu(iCurI,iCurJ,GetMeUserInfo()->bDeskStation,m_iDianMuQiZi);
			if (!bDianZi) 
				return 0;
			m_PlayView.m_MeCard.SetChess(m_iMeChessFlag,m_Qiflag[m_iNowOutPeople],m_iDianMuQiZi);	
			return 0;
		}
		//��������
		if (!m_bCanPlay) 
			return 0;
		if (m_iNowOutPeople != GetMeUserInfo()->bDeskStation) 
			return 0;
		//
		BOOL bAdd = m_Logic.CanAddQiZi(iCurI,iCurJ,m_Qiflag[m_iNowOutPeople],m_iMeBoard);

		if (!bAdd)
			return 0;

		m_PlayView.m_MeCard.SetChess(m_iMeChessFlag,0,m_iMeBoard);

		PlayChessMsg playChessInfo;
		playChessInfo.iEndI = iCurI;
		playChessInfo.iEndJ = iCurJ;
		playChessInfo.iPlayFlag = 0;
		::CopyMemory(playChessInfo.iChessArray,m_iMeBoard,sizeof(m_iMeBoard));
		SendGameData(&playChessInfo,sizeof(playChessInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);

		/// ����ͬ��ʱ����Ϣ
		SyncTimeStruct	sMess;
		sMess.iBotJuUseTime = m_PlayView.m_MeCard.m_iBotJuUseTime;
		sMess.iBotBuUseTime = m_PlayView.m_MeCard.m_iBotBuUseTime;
		sMess.iBotDuUseTime = m_PlayView.m_MeCard.m_iBotDuUseTime;

		SendGameData(&sMess, sizeof(sMess), MDM_GM_GAME_NOTIFY, ASS_SYNC_TIME, 0);

	}

	m_PlayView.m_btRegretChess.EnableWindow(TRUE);
	if (m_PlayView.m_RegretChess.GetSafeHwnd()) 
	{
		m_PlayView.m_RegretChess.DestroyWindow();
		if (m_bNoHaveAgreeRegret)
		{
			SendMessage(IDM_AGREE_REGRET,0,0);
		}		
	}
	if (m_PlayView.m_StopMessage.GetSafeHwnd())
	{
		m_PlayView.m_StopMessage.DestroyWindow();
		SendMessage(IDM_ARGEE_STOP,0,0);
	}
	if (m_PlayView.m_DianMu.GetSafeHwnd())
	{
       m_PlayView.m_DianMu.DestroyWindow();
	  // SendMessage(IDM_AGREE_DIAN_MU,0,0);
	}

	return 0;
}


//�������°�ť����
LRESULT	CClientGameDlg::OnHitHaveThing(WPARAM wparam, LPARAM lparam)
{
	if (m_PlayView.m_HaveThing.GetSafeHwnd()==NULL) 
	{
		m_PlayView.m_HaveThing.Create(IDD_THING, &m_PlayView);
		m_PlayView.m_HaveThing.m_pParnet = &m_PlayView;
	}
	m_PlayView.m_HaveThing.CenterWindow(&m_PlayView);
	m_PlayView.m_HaveThing.ShowWindow(SW_SHOW);
	m_PlayView.m_HaveThing.SetFocus();
	return 0;
}

//��ǰ����
LRESULT	CClientGameDlg::OnStop(WPARAM wparam, LPARAM lparam)
{
	if(!m_bWatchMode)
	{
		if (m_PlayView.m_StopThing.GetSafeHwnd()==NULL) 
		{
			m_PlayView.m_StopThing.Create(IDD_STOP, &m_PlayView);
			m_PlayView.m_StopThing.m_pParnet = &m_PlayView;
		}
		m_PlayView.m_StopThing.CenterWindow(&m_PlayView);
		m_PlayView.m_StopThing.ShowWindow(SW_SHOW);
		m_PlayView.m_StopThing.SetFocus();

	}
	return 0;
}

//������ǰ����
LRESULT	CClientGameDlg::OnStopThing(WPARAM wparam, LPARAM lparam)
{
	if(!m_bWatchMode && GetStationParameter()!=GS_WAIT_SETGAME)
	{
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_STOP_THING,0);
	}
	return 0;
}

//ͬ����ǰ����
LRESULT	CClientGameDlg::OnAgreeStop(WPARAM wparam, LPARAM lparam)
{
	if(!m_bWatchMode && GetStationParameter()!=GS_WAIT_SETGAME)
	{
		StopResultStruct Stop;
		Stop.bArgee = (BYTE)wparam;
		SendGameData(&Stop,sizeof(Stop),MDM_GM_GAME_NOTIFY,ASS_AGREE_STOP,0);
	}
	return 0;
}


//�����뿪������
LRESULT	CClientGameDlg::OnSendHaveThing(WPARAM wparam, LPARAM lparam)
{
	if(!m_bWatchMode && GetStationParameter()!=GS_WAIT_SETGAME)
	{
		m_PlayView.m_btThing.EnableWindow(FALSE);
		HaveThingStruct HaveThing;
		HaveThing.bDeskStation = 0;
		::lstrcpy(HaveThing.szMessage,(char *)wparam);
		SendGameData(&HaveThing,sizeof(HaveThing),MDM_GM_GAME_NOTIFY,ASS_HAVE_THING,0);
	}
	return 0;
}

//�û������뿪
LRESULT	CClientGameDlg::OnArgeeUserLeft(WPARAM wparam, LPARAM lparam)
{
	LeaveResultStruct Leave;
	Leave.bDeskStation=GetMeUserInfo()->bDeskStation;
	Leave.bArgeeLeave=(BYTE)wparam;
	SendGameData(&Leave,sizeof(Leave),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	return 0;
}


//����
LRESULT CClientGameDlg::OnAdmitDefeat(WPARAM wparam, LPARAM lparam)
{
	if(!m_bWatchMode)
	{
		if (m_PlayView.m_AdmitDefeat.GetSafeHwnd()==NULL) 
		{
			m_PlayView.m_AdmitDefeat.Create(IDD_REN_SHU, &m_PlayView);
			m_PlayView.m_AdmitDefeat.m_pParnet= &m_PlayView;
		}
		m_PlayView.m_AdmitDefeat.CenterWindow(&m_PlayView);
		m_PlayView.m_AdmitDefeat.ShowWindow(SW_SHOW);
		m_PlayView.m_AdmitDefeat.SetFocus();
	}
	return 0;
}


//����
LRESULT CClientGameDlg::OnRegretChess(WPARAM wparam, LPARAM lparam)
{
	m_bNoHaveAgreeRegret = true;
	m_PlayView.m_GameNoteView.SetMessage(TEXT ("���ڵȴ��Լ�ͬ��..."));
	m_PlayView.m_GameNoteView.CenterWindow(&m_PlayView);
	m_PlayView.m_GameNoteView.ShowWindow(SW_SHOW);
	m_PlayView.m_btRegretChess.EnableWindow(false);	
	SendGameData(MDM_GM_GAME_NOTIFY,ASS_HUI_QI,0);
	return 0;
}


//��Ŀ
LRESULT CClientGameDlg::OnChessManual(WPARAM wparam, LPARAM lparam)
{
	if(!m_bWatchMode)
	{
		if (m_PlayView.m_DianMu.GetSafeHwnd() == NULL)
		{
			m_PlayView.m_DianMu.Create(IDD_DIAN_MU,this);
			m_PlayView.m_DianMu.m_pParent = this;
		}
		m_PlayView.m_DianMu.SetWindowText(TEXT("��Ŀ"));
		
		TCHAR path[MAX_PATH] = {0};
		wsprintf(path,TEXT("image\\%s\\dianmu.bmp"),SKIN_FOLDER);
		m_PlayView.m_DianMu.m_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		m_PlayView.m_DianMu.SetMessage(TEXT("��ȷ��Ҫ��ʼ��Ŀ��?"),50,60);

		wsprintf(path,TEXT("image\\%s\\ok.bmp"),SKIN_FOLDER);
		m_PlayView.m_DianMu.m_btOk.LoadButtonBitmap(path,false);
		m_PlayView.m_DianMu.m_btOk.MoveWindow(50,100,46,20);

		wsprintf(path,TEXT("image\\%s\\cancel.bmp"),SKIN_FOLDER);
		m_PlayView.m_DianMu.m_btCancel.LoadButtonBitmap(path,false);
		m_PlayView.m_DianMu.m_btCancel.MoveWindow(150,100,46,20);

		m_PlayView.m_DianMu.m_btOk.ShowWindow(SW_SHOW);
		m_PlayView.m_DianMu.m_btCancel.ShowWindow(SW_SHOW);
		m_PlayView.m_DianMu.m_btContinueChess.ShowWindow(SW_HIDE);
		m_PlayView.m_DianMu.CenterWindow(&m_PlayView);
		m_PlayView.m_DianMu.ShowWindow(SW_SHOW);
		m_PlayView.m_DianMu.SetFocus();
	}
	return 0;
}

//��ɵ�Ŀ
LRESULT CClientGameDlg::OnFinishDianMu(WPARAM wparam, LPARAM lparam)
{
	if(m_bIsInDianZi)
	{
		m_PlayView.m_btFinishDianMu.EnableWindow(FALSE);
		float fw,fb;
		m_Logic.NumQiZi(&fw,&fb,m_iDianMuQiZi);
		m_PlayView.m_MeCard.SetChess(m_iMeChessFlag,m_Qiflag[m_iNowOutPeople],m_iDianMuQiZi);

		m_bIsInDianZi = false;
		struct DianMu dianmu;
		::memcpy(dianmu.qi,m_Logic.m_shizi,sizeof(m_Logic.m_shizi));
		SendGameData(&dianmu,sizeof(dianmu),MDM_GM_GAME_NOTIFY,ASS_NUMBER_ZI,0);

		if (m_iNowOutPeople != GetMeUserInfo()->bDeskStation)
		{
			m_PlayView.m_MeCard.SetUseMouse(false);
		}
	}
	return 0;
}

//ͬ���Ŀ
LRESULT CClientGameDlg::OnAgreeDianMu(WPARAM wParam, LPARAM lParam)
{
	DianMuResultStruct dianMuInfo;
	dianMuInfo.bAgree = (int) wParam;
	SendGameData(&dianMuInfo,sizeof(dianMuInfo),MDM_GM_GAME_NOTIFY,ASS_AGREE_DIAN_MU,0);
	return 0;
}

//ȷ��ǿ������
LRESULT CClientGameDlg::OnAssuranceNumZi(WPARAM wParam, LPARAM lParam)
{

	if (GetStationParameter() == GS_PLAY_GAME)
	{
		int flag = (int) wParam;

		SendGameData(MDM_GM_GAME_NOTIFY,ASS_DIAN_MU_RESULT,flag);

	}
	return 0;
}

//ǿ������
LRESULT CClientGameDlg::OnNumberZi(WPARAM wParam, LPARAM lParam)
{

	return 0;
}


LRESULT CClientGameDlg::OnAssuranceDianMu(WPARAM wParam, LPARAM lParam)
{
	m_PlayView.m_btChessManual.EnableWindow(false);
	m_PlayView.m_MeCard.SetUseMouse(false);
	SendGameData(MDM_GM_GAME_NOTIFY,ASS_DIAN_MU,0);
	return 0;
}

//mouse move qizi
LRESULT CClientGameDlg::OnMouseMoveCard(WPARAM wParam, LPARAM lParam)
{
	if (GetStationParameter() == GS_PLAY_GAME)
	{
		if (m_bIsInDianZi)
		{
			int i = (int) wParam,
				j = (int) lParam;

			if (m_Qiflag[GetMeUserInfo()->bDeskStation] == 0)
			{
				if (m_iMeBoard[i][j] == 2 || m_iMeBoard[i][j] == 12) 
					m_PlayView.m_MeCard.SetUseMouse(false);
				else
					m_PlayView.m_MeCard.SetUseMouse(true);
			}
			else if (m_Qiflag[GetMeUserInfo()->bDeskStation] == 1)
			{
				if (m_iMeBoard[i][j] == 1 || m_iMeBoard[i][j] == 11)
					m_PlayView.m_MeCard.SetUseMouse(false);
				else 
					m_PlayView.m_MeCard.SetUseMouse(true);
			}
		}
	}
	return 0;
}


//��ʱ����
LRESULT CClientGameDlg::OnSuperTimer(WPARAM wparam, LPARAM lparam)
{
	if(m_bIsInDianZi)
	{
		OutputDebugString("��Ŀ״̬�£���ʱ����");
		return true;
	}
	if (m_bWatchMode == FALSE)
	{
		SuperTimerStruct  supertimer;
		::memset(&supertimer,0,sizeof(supertimer));
		supertimer.bDeskStation = GetMeUserInfo()->bDeskStation;
		SendGameData(&supertimer,sizeof(supertimer),MDM_GM_GAME_NOTIFY,ASS_SUPER_TIMER,0);
	}
	return 0;
}


//ȷ������
LRESULT CClientGameDlg::OnSendAdmitDefeat(WPARAM wparam, LPARAM lparam)
{
	if (m_bWatchMode == FALSE)
	{
		LoserStruct  loser;
		::memset(&loser,0,sizeof(loser));
		loser.bDeskStation = GetMeUserInfo()->bDeskStation;
		SendGameData(&loser,sizeof(loser),MDM_GM_GAME_NOTIFY,ASS_REN_SHU,0);
	}
	//	SendGameData(MDM_GM_GAME_NOTIFY,ASS_REN_SHU,(int)m_iMeChessFlag);
	return 0;
}


//ͬ�����
LRESULT CClientGameDlg::OnAgreeRegret(WPARAM wparam, LPARAM lparam)
{

	m_bNoHaveAgreeRegret = false;
	int flag = (int)(wparam);

	SendGameData(MDM_GM_GAME_NOTIFY,ASS_AGREE_HUI_QI,flag);
	return 0;
}

//����
void CClientGameDlg::OnGameSetting()
{
	AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));

	//CGameSet GameSetDlg(this);
	m_GameSet = &m_PlayView.m_GameSetDlg;
	//GameSetDlg.m_bSound = m_pGameInfo->bEnableSound;
	//GameSetDlg.m_bShowUser = m_pGameInfo->bShowUserInfo;	
	//GameSetDlg.m_bEnableWatch = m_pGameInfo->bEnableWatch;
	//GameSetDlg.m_bRightPower = GetProfileInt(TEXT("RightPower"),FALSE);

	m_GameSet->m_bSound = m_pGameInfo->bEnableSound;
	m_GameSet->m_bShowUser = m_pGameInfo->bShowUserInfo;	
	m_GameSet->m_bEnableWatch = m_pGameInfo->bEnableWatch;
	m_GameSet->m_bRightPower = GetProfileInt(TEXT("RightPower"),FALSE);


	m_GameSet->CenterWindow(&m_PlayView);

	//CRect clientviewrect;
	//m_PlayView.GetClientRect(&clientviewrect);
	//int viewCenter_x = clientviewrect.Width()/2;
	//int viewCenter_y = clientviewrect.Height()/2;

	//CRect gamesetrect;
	//m_GameSet->GetClientRect(&gamesetrect);
	//int gamesetCenter_x = gamesetrect.Width()/2;
	//int gamesetCenter_y = gamesetrect.Height()/2;

	//CString path = CINIFile::GetAppPath();
	//path += TEXT("\\GameOption.bcf");
	//CBcfFile f(path);
	//CString key("config");	

	//int gameset_x = viewCenter_x+f.GetKeyVal(key,"GameSetDlg_Offset_x",-gamesetCenter_x);
	//int gameset_y = viewCenter_y+f.GetKeyVal(key,"GameSetDlg_Offset_y",-gamesetCenter_y);

	//m_GameSet->MoveWindow(gameset_x,gameset_y,gamesetrect.Width(),gamesetrect.Height());


	m_GameSet->ShowWindow(SW_SHOW);
	//if (m_GameSet->DoModal() == IDOK)
	//{
	//	m_GameSet = NULL;
	//	//�������
	//	bool bSendWatch=((!m_bWatchMode)&&(m_pGameInfo->bEnableWatch != m_GameSet->m_bEnableWatch));

	//	//��������
	//	m_pGameInfo->bShowUserInfo=m_GameSet->m_bShowUser;
	//	m_pGameInfo->bEnableWatch=m_GameSet->m_bEnableWatch;
	//	m_pGameInfo->bEnableSound=m_GameSet->m_bSound;
	//	WriteProfileInt(TEXT("RightPower"),m_GameSet->m_bRightPower?1:0);
	//	//		m_PlayView.m_bRightPower=GameSetDlg.m_bRightPower;

	//	//��������
	//	if (!bSendWatch)
	//	{
	//		MSG_GM_WatchSet WatchSet;			
	//		memset(&WatchSet,0,sizeof(WatchSet));
	//		WatchSet.dwUserID = 0;
	//		SendGameData(&WatchSet,sizeof(WatchSet),MDM_GM_GAME_FRAME,ASS_GM_WATCH_SET,m_GameSet->m_bEnableWatch?TRUE:FALSE);
	//	}
	//}
	//m_GameSet = NULL;

	//AfxSetResourceHandle(GetModuleHandle(NULL));
	return;
}
LRESULT CClientGameDlg::OnGameSettingOK(WPARAM wParam, LPARAM lParam)
{
	m_GameSet = &m_PlayView.m_GameSetDlg;

	//�������
	bool bSendWatch=((!m_bWatchMode)&&(m_pGameInfo->bEnableWatch != m_GameSet->m_bEnableWatch));

	//��������
	m_pGameInfo->bShowUserInfo=m_GameSet->m_bShowUser;
	m_pGameInfo->bEnableWatch=m_GameSet->m_bEnableWatch;
	m_pGameInfo->bEnableSound=m_GameSet->m_bSound;
	WriteProfileInt(TEXT("RightPower"),m_GameSet->m_bRightPower?1:0);
	//		m_PlayView.m_bRightPower=GameSetDlg.m_bRightPower;

	//��������
	if (!bSendWatch)
	{
		MSG_GM_WatchSet WatchSet;			
		memset(&WatchSet,0,sizeof(WatchSet));
		WatchSet.dwUserID = 0;
		SendGameData(&WatchSet,sizeof(WatchSet),MDM_GM_GAME_FRAME,ASS_GM_WATCH_SET,m_GameSet->m_bEnableWatch?TRUE:FALSE);
	}
	return 0;
}
void CClientGameDlg::OnWatchSetChange(void)
{
	if (m_bWatchMode == true)
	{
		//		m_PlayView.m_MeCard.SetShowCard(m_bWatchOther?TRUE:FALSE);
		if (m_bWatchOther)
			m_MessageHandle.InsertNormalMessage(TEXT("����������Թ�����Ϸ"));
		else m_MessageHandle.InsertNormalMessage(TEXT("��Ҳ��������Թ�����Ϸ"));
	}
}

bool CClientGameDlg::GameUserLeft(BYTE bDeskStation, UserItemStruct *pUserItem, bool bWatchUser)
{
	//��
	if(pUserItem == NULL)
		return false;
	//��������Ϸ�����뿪
	if(GetMeUserInfo()->bDeskStation==bDeskStation&&!bWatchUser && TY_MATCH_GAME == GetComType())
	{
		OnCancel();
		return __super::GameUserLeft(bDeskStation,pUserItem,bWatchUser);
	}

	//�������Ǳ����뿪��Ϸ��
	if(GetMeUserInfo()->bDeskStation != bDeskStation&&!bWatchUser && TY_MATCH_GAME == GetComType())
	{
		return __super::GameUserLeft(bDeskStation,pUserItem,bWatchUser);
	}

	if (bWatchUser==false)//&&(bDeskStation==0))
	{
		if (GetStationParameter() > GS_WAIT_ARGEE)
		{
			this->SetStationParameter(GS_WAIT_ARGEE);
			ResetGameStation(RS_ALL_VIEW);			
			m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode && TY_MATCH_GAME != GetComType());
		}
	}

	//�Թ���Ҳ�뿪��Ϸ��
	if(GetMeUserInfo()->bDeskStation==bDeskStation&&!bWatchUser==TRUE)
	{
		OnCancel();
	}

	return __super::GameUserLeft(bDeskStation, pUserItem, bWatchUser);
}

bool CClientGameDlg::GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	if (! __super::GameUserCome(bDeskStation, pUserItem, bWatchUser))
	{
		return FALSE;
	}
	
#ifdef	VIDEO
	if( GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID && bWatchUser )
	{
		 m_PlayView.m_bWatch=bWatchUser;
	}
	// if( GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID && (!bWatchUser) )
	//{    
	//	CPlayVideo *pPlayVideo=CPlayVideo::CreatePlayVideo();
	//	if(NULL!=pPlayVideo)
	//	{ 
	//		CString NickName=TEXT("");
	//		int    byteDeskNO=0;
	//		UserInfoStruct &UInfo=	pUserItem->GameUserInfo;
	//		NickName+=UInfo.nickName;
	//		
	//		byteDeskNO=UInfo.bDeskNO;
	//		UINT	uiRoomID = m_pGameInfo->uRoomID;
	//		UINT	uiBRRoomID =   uiRoomID * 1000 + byteDeskNO; 

	//		ST_INITVIDEO stInitVideo;
	//		stInitVideo.iRoomPassType = 0;
	//		stInitVideo.nRoomID		  = uiBRRoomID;
	//		stInitVideo.pParentWnd    = &m_PlayView.m_MeCard;
	//		stInitVideo.strRoomPass   = _T("");
	//		stInitVideo.strUserName	  = NickName;
	//		stInitVideo.strUserPass   = _T("");
	//		if( pPlayVideo->Video_Initialize(stInitVideo) )
	//		{
	//			//AppendLog("��Ƶ��ʼ���ɹ�!");
	//			//������Ƶ������
	//			pPlayVideo->Connect();
	//		}
	//		else
	//		{
	//			pPlayVideo->Video_Release();
	//			//AppendLog("��Ƶ��ʼ��ʧ��!");
	//			return 0;
	//		}

	//	} 
	//}
#ifdef _USE_VIDEO
	if( GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID && (!bWatchUser) )
		if(m_pBRGameVideoSDK != NULL)
		{
			CString NickName=TEXT("");
			int    byteDeskNO=0;
			UserInfoStruct &UInfo=	pUserItem->GameUserInfo;
			NickName+=UInfo.nickName;
			byteDeskNO=UInfo.bDeskNO;
			UINT	uiRoomID = m_pGameInfo->uRoomID;
			UINT	uiBRRoomID =   uiRoomID * 1000 + byteDeskNO; 

			m_pBRGameVideoSDK->ConnectToServer(m_GameVideoSet.VideoServerIP,m_GameVideoSet.VideoServerPort);
			m_pBRGameVideoSDK->LoginSystem(NickName,"");
			m_pBRGameVideoSDK->EnterRoom(uiBRRoomID,"");			
		}
#endif
#endif
	return TRUE;
}

bool	CClientGameDlg::AFCCloseFrame(bool bNotifyServer)
{
#ifdef	VIDEO
		///�ͷ���Ƶ��Դ  
	//if( NULL != CPlayVideo::CreatePlayVideo())
	//	CPlayVideo::CreatePlayVideo()->Video_Release();
#endif
#ifdef _USE_VIDEO
	if(!m_pBRGameVideoSDK)
		return __super::AFCCloseFrame();

	m_pBRGameVideoSDK->LeaveRoom();
	m_pBRGameVideoSDK->CloseGameVideoSDK();

	delete m_pBRGameVideoSDK;
	m_pBRGameVideoSDK = NULL;
#endif
	return __super::AFCCloseFrame();
}


//��ʱ��
void CClientGameDlg::OnTimer(UINT nIDEvent)
{
	CLoveSendClassInExe::OnTimer(nIDEvent);
}

//�رմ���
void CClientGameDlg::OnCancel()
{
	TCHAR strMoney[200];

	CString s = CINIFile::GetAppPath ();/////����·��	
	DWORD cfgHandle;
#ifdef	VIDEO
	cfgHandle=cfgOpenFile(s +"30200205.bcf");
#else
	cfgHandle=cfgOpenFile(s +"30200200.bcf");
#endif

	if(cfgHandle<0x10)
	{
		return;
	}
	CString key = TEXT("config");
	CString sec;

	__int64 iTempMoney = 1;
	for (int i=0; i<Glb().m_nPowerOfGold; i++)
	{
		iTempMoney *=10;
	}
    iTempMoney = iTempMoney*(m_iRunPublish*m_iRoomBasePoint);
	sec.Format("FouceQuitTip%d", m_pGameInfo->uComType);	
	
	//��ѯ״̬
	if (!CanLeftDesk(GetMeUserInfo()->bDeskStation))
	{
		if (GetComType() == TY_MONEY_GAME)
		{
			wsprintf(strMoney,cfgGetValue(cfgHandle,key,sec,""),m_iRunPublish,iTempMoney);
		}
		else
		{
			wsprintf(strMoney,cfgGetValue(cfgHandle,key, sec, ""),m_iRunPublish);
		}
		//if(GetComType() == TY_MONEY_GAME)
		//{
		//	//m_PlayView.m_Result.GetNumString(m_iRunPublish * m_iRoomBasePoint, strMoney, Glb().m_nPowerOfGold, false);
		//	strMoney.Format("�����˳������ %d ��(��ҳ������ %I64d ���).�����Ҫ�˳���?",m_iRunPublish,m_iRunPublish*m_iRoomBasePoint*GetPowerofGold());
		//}
		//else if(GetComType() == TY_MATCH_GAME)
		//{
		//	//m_PlayView.m_Result.GetNumString(m_iRunPublishMoney, strMoney, Glb().m_nPowerOfGold, false);
		//	strMoney.Format("�˱�����,�����˳�,���۵������( %I64d )����( %d )",m_iRunPublish*m_iRoomBasePoint*GetPowerofGold(),m_iRunPublish);
		//}
		//else
		//{
		//	strMoney.Format("�����˳������ %d ��.�����Ҫ�˳���?",m_iRunPublish);
		//}

		if (IDOK!=AFCMessageBox(strMoney,m_pGameInfo->szGameName,AFC_YESNO)) //|MB_DEFBUTTON2|MB_ICONINFORMATION,this)) 
			return;
		SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	}

	cfgClose(cfgHandle);
	AFCCloseFrame();

	__super::OnCancel();
}

//�Ƿ������;�뿪
BOOL CClientGameDlg::CanLeftDesk(BYTE bDeskStation)
{
	if(GetStationParameter() >= GS_PLAY_GAME&& GetStationParameter() < GS_WAIT_NEXT&&!m_bWatchMode)
		return FALSE;
	return TRUE;
}

//�汾�˶�
BOOL CClientGameDlg::CheckVersion(BYTE iVersion,BYTE iVersion2)
{
	TCHAR GameName[100];
	CString strBuffer;
	wsprintf(GameName,TEXT("����%s��Ϸ�汾���ɣ���Ҫ�������������°汾��?"),m_pGameInfo->szGameName);
	if(iVersion!=m_iVersion||iVersion2!=m_iVersion2)
	{
		TCHAR sz[200];
		wsprintf(sz,TEXT("�汾��ͻ,��ǰ�������汾��%d-%d,���İ汾��%d-%d"),iVersion,iVersion2,m_iVersion,m_iVersion2);
		//		WriteStr(sz);
	}	
	if(iVersion!=m_iVersion)
	{
		if (IDOK==AFCMessageBox(GameName,m_pGameInfo->szGameName,AFC_YESNO)) //|MB_DEFBUTTON1|MB_ICONINFORMATION,this)) 
		{
			strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strDownLoadSetupADDR),1,1);
			ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		}
		OnCancel();
		return TRUE;
	}

	if(iVersion2!=m_iVersion2)
	{
		if (IDOK==AFCMessageBox(GameName,m_pGameInfo->szGameName,AFC_YESNO)) //|MB_DEFBUTTON1|MB_ICONINFORMATION,this)) 
		{
			strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strDownLoadSetupADDR),1,1);
			ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		}
		OnCancel();
		return TRUE;
	}
	return TRUE;
}

#ifdef _USE_VIDEO
/**
*	�յ���Ϣ���ͻ��˽��뷿��
*	@param wParam ��INT����ʾ�����뷿���ID��
*	@param lParam ��INT����ʾ�Ƿ���뷿�䣺0�ɹ����룬����Ϊ�������
*/
LRESULT CClientGameDlg::OnGVClientEnterRoom(WPARAM wParam, LPARAM lParam)
{
	if(m_pBRGameVideoSDK)
	{
		CRect pRect;
		CString plogpath;
		if(m_PlayView.GetVideoRect(GetMeUserInfo()->nickName,pRect,plogpath))
			m_pBRGameVideoSDK->ShowUserVideo(-1,TRUE,pRect,FromHandle(m_PlayView.GetSafeHwnd()));
		m_pBRGameVideoSDK->OpenLocalCamera();
	}

	return 0;
}
/**
*	�յ���Ϣ���û����루�뿪������
*	@param wParam ��INT����ʾ�û�ID��
*	@param lParam ��BOOL����ʾ���û��ǽ��루TRUE�����뿪��FALSE������
*/
LRESULT CClientGameDlg::OnGVClientUserAtRoom(WPARAM wParam, LPARAM lParam)
{
	if(!m_pBRGameVideoSDK)
		return 0;

	CString logstr;
	INT onlinenum = (INT)wParam;

	std::list<INT> userlist;
	std::list<INT>::iterator Iterator;
	m_pBRGameVideoSDK->GetRoomUserList(userlist);

	for(Iterator=userlist.begin(); Iterator!=userlist.end(); Iterator++)
	{
		INT userid = *Iterator;

		if(m_pBRGameVideoSDK)
		{
			std::map<std::string,int>::iterator iter = m_VideoUserInfo.find(std::string(m_pBRGameVideoSDK->GetUserNameById(userid)));
			if(iter != m_VideoUserInfo.end())
				(*iter).second = userid;
			else
				m_VideoUserInfo.insert(std::pair<std::string,int>(std::string(m_pBRGameVideoSDK->GetUserNameById(userid)),userid));

			CRect pRect;
			CString plogpath;
			if(m_PlayView.GetVideoRect(m_pBRGameVideoSDK->GetUserNameById(userid),pRect,plogpath))
				m_pBRGameVideoSDK->ShowUserVideo(userid,TRUE,pRect,FromHandle(m_PlayView.GetSafeHwnd()));
		}
		else			
			break;
	}

	return 0;
}

/**
*	�յ���Ϣ���ͻ��˵�¼ϵͳ
*	@param wParam ��INT����ʾ�Լ����û�ID��
*	@param lParam ��INT����ʾ��¼�����0 �ɹ�������Ϊ������룬�ο�������붨��
*/
LRESULT CClientGameDlg::OnGVClientLogin(WPARAM wParam, LPARAM lParam)
{
	if(GetMeUserInfo() == NULL) return 0;

	// �����Լ���userid
	std::map<std::string,int>::iterator iter = m_VideoUserInfo.find(std::string(GetMeUserInfo()->nickName));
	if(iter != m_VideoUserInfo.end())
		(*iter).second = (int)wParam;
	else
		m_VideoUserInfo.insert(std::pair<std::string,int>(std::string(GetMeUserInfo()->nickName),(int)wParam));

	return 0;
}

/**
*	�յ���ǰ����������û���Ϣ
*	���뷿��󴥷�һ��
*	@param wParam ��INT����ʾ�����û������������Լ���
*	@param lParam ��INT����ʾ����ID
*/
LRESULT CClientGameDlg::OnGVClientOnlineUser(WPARAM wParam, LPARAM lParam)
{
	if(!m_pBRGameVideoSDK)
		return 0;

	CString logstr;
	INT onlinenum = (INT)wParam;

	std::list<INT> userlist;
	std::list<INT>::iterator Iterator;
	m_pBRGameVideoSDK->GetRoomUserList(userlist);

	for(Iterator=userlist.begin(); Iterator!=userlist.end(); Iterator++)
	{
		INT userid = *Iterator;

		if(m_pBRGameVideoSDK)
		{
			std::map<std::string,int>::iterator iter = m_VideoUserInfo.find(std::string(m_pBRGameVideoSDK->GetUserNameById(userid)));
			if(iter != m_VideoUserInfo.end())
				(*iter).second = userid;
			else
				m_VideoUserInfo.insert(std::pair<std::string,int>(std::string(m_pBRGameVideoSDK->GetUserNameById(userid)),userid));

			CRect pRect;
			CString plogpath;
			if(m_PlayView.GetVideoRect(m_pBRGameVideoSDK->GetUserNameById(userid),pRect,plogpath))
				m_pBRGameVideoSDK->ShowUserVideo(userid,TRUE,pRect,FromHandle(m_PlayView.GetSafeHwnd()));
		}
		else				
			break;
	}

	return 0;
}
/**
* ˢ�����е���Ƶ���
*/
void CClientGameDlg::RefreshAllVideoPanel(void)
{
	if(m_pBRGameVideoSDK == NULL ||
		m_VideoUserInfo.empty()) return;

	/// Ϊ��Ƶ����rgn(����ר��)
	CRgn rgn;
	CRect rcWnd;
	m_PlayView.m_MeCard.GetClientRect(&rcWnd);
	rgn.CreateRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom);


	std::map<std::string,int>::iterator iter = m_VideoUserInfo.begin();
	for(;iter != m_VideoUserInfo.end();++iter)
	{
		CRect pRect;
		CString plogpath;


		if(m_PlayView.GetVideoRect(m_pBRGameVideoSDK->GetUserNameById((*iter).second),pRect,plogpath))
			m_pBRGameVideoSDK->ShowUserVideo((*iter).second,TRUE,pRect,FromHandle(m_PlayView.GetSafeHwnd()));

		/// Ϊ��Ƶ����rgn(����ר��)
		m_PlayView.ClientToScreen(&pRect);
		m_PlayView.m_MeCard.ScreenToClient(&pRect);
		CRgn rgn2;
		rgn2.CreateRectRgn(pRect.left, pRect.top, 
			pRect.right, pRect.bottom);
		rgn.CombineRgn(&rgn, &rgn2, RGN_XOR);

	}

	/// Ϊ��Ƶ����rgn(����ר��)
	m_PlayView.m_MeCard.SetWindowRgn((HRGN)rgn.GetSafeHandle(), TRUE);
}
#endif

void CClientGameDlg::FixControlStation(int iWidth, int iHeight)
{
	CLoveSendClassInExe::FixControlStation(iWidth,iHeight);

#ifdef _USE_VIDEO
	RefreshAllVideoPanel();
#endif
}