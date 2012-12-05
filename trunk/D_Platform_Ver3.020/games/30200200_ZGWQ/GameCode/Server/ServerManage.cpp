#include "StdAfx.h"
#include "ServerManage.h"


//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(FULL_BEGIN)
{
	for (int i=0;i<PLAY_COUNT;i++)
	{
		m_bUserNetCut[i]=false;
	}
	m_bAgreeDianmuFinish=-1;
	m_iJuTimeMinute = 10;
	m_iJuTimeSecond = 30;
	m_iBuTimeMinute = 1;
	m_iBuTimeSecond = 30;
	m_iDuMiaoMinute = 1;
	m_iDuMiaoSecond = 30;
	m_Endi=m_Endj=-1;
//	m_bQueryQuit=false;
	m_iEnableWatch=0;
	m_iLeaveArgee=0;
//	m_iNtCardCount=0;
	m_iLessCount=0;
	m_iPlayCardCount=0;
	m_iBeginStation=0;
	m_iEndStation=0;
//	m_iBaseOutCount=0;
//	m_iNtPeople=-1;
//	m_iFirstOutPeople=-1;
	m_iOutCardPeople=-1;
	m_iUpGradePeople=-1;
	m_iBeenPlayGame=0;
//	m_iGamePoint=0;
	m_bGameStation=GS_WAIT_SETGAME;
//	m_CardCount=PLAY_TWO_CARD;
	//::memset(&m_PunishPoint,0,sizeof(m_PunishPoint));
	::memset(m_iPlayNTNum,0,sizeof(m_iPlayNTNum));
}

//��������
CServerGameDesk::~CServerGameDesk(void)
{
}

//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	if (uTimerID==TIME_GAME_FINISH)	//������ʱ��
	{
		KillTimer(TIME_GAME_FINISH);

		if (m_bGameStation==GS_PLAY_GAME) 
			GameFinish(0,GF_NORMAL);
	}

	return __super::OnTimer(uTimerID);
}

//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
		{
			GameStation_1 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;				//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;				//�Ͱ汾
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();		//���ӱ���
			GameStation.iRoomBasePoint = GetRoomBasePoint();		//���䱶��
			GameStation.iRunPublish = GetRunPublish();				//���ܿ۷�
			GameStation.iRunPublishMoney = GetRunPublishMoney();	//���ܽ��
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			GameStation_2 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;				//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;				//�Ͱ汾
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();		//���ӱ���
			GameStation.iRoomBasePoint = GetRoomBasePoint();		//���䱶��
			GameStation.iRunPublish = GetRunPublish();				//���ܿ۷�
			GameStation.iRunPublishMoney = GetRunPublishMoney();	//���ܽ��

			GameStation.iJuTimeMinute = m_iJuTimeMinute;
			GameStation.iJuTimeSecond = m_iJuTimeSecond;
			GameStation.iBuTimeMinute = m_iBuTimeMinute;
			GameStation.iBuTimeSecond = m_iBuTimeSecond;
			GameStation.iDuMiaoMinute = m_iDuMiaoMinute;
			GameStation.iDuMiaoSecond = m_iDuMiaoSecond;

			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	case GS_PLAY_GAME:	//��Ϸ��״̬
		{
			GameStation_4 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;				//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;				//�Ͱ汾
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();		//���ӱ���
			GameStation.iRoomBasePoint = GetRoomBasePoint();		//���䱶��
			GameStation.iRunPublish = GetRunPublish();				//���ܿ۷�
			GameStation.iRunPublishMoney = GetRunPublishMoney();	//���ܽ��

			GameStation.iJuTimeMinute = m_iJuTimeMinute;
			GameStation.iJuTimeSecond = m_iJuTimeSecond;
			GameStation.iBuTimeMinute = m_iBuTimeMinute;
			GameStation.iBuTimeSecond = m_iBuTimeSecond;
			GameStation.iDuMiaoMinute = m_iDuMiaoMinute;
			GameStation.iDuMiaoSecond = m_iDuMiaoSecond;
			GameStation.iEndI=m_Endi;
			GameStation.iEndJ=m_Endj;
			GameStation.m_iLastDeskStation = m_iLastDeskStation;
//			GameStation.iFirstOutPeople = m_iFirstOutPeople;
			GameStation.iOutCardPeople=m_iOutCardPeople;
			GameStation.isHitAgreedianmu=m_bAgreeDianmuFinish;
			GameStation.m_stUsedTimeInfo=m_stUsedTimeInfo;

			memcpy(GameStation.bIsInHuiquStation, m_bIsInHuiquStation, sizeof(m_bIsInHuiquStation));
			memcpy(GameStation.bChessingInHuiQi, m_bChessingInHuiQi, sizeof(m_bChessingInHuiQi));

			//��������
			::CopyMemory(GameStation.iTiZiNum,m_iTiZiNum,sizeof(m_iTiZiNum));
			::CopyMemory(GameStation.iQiZiNum,m_iQiZiNum,sizeof(m_iQiZiNum));

			::CopyMemory(GameStation.iQiZi,m_iBoard,sizeof(m_iBoard));
			int iSendSize=sizeof(GameStation);
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,iSendSize);

			return TRUE;	
		}
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{
			GameStation_5 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;				//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;				//�Ͱ汾
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();		//���ӱ���
			GameStation.iRoomBasePoint = GetRoomBasePoint();		//���䱶��
			GameStation.iRunPublish = GetRunPublish();				//���ܿ۷�
			GameStation.iRunPublishMoney = GetRunPublishMoney();	//���ܽ��

			GameStation.iPlayCount=m_iLessCount;
			GameStation.iCardCount=m_iPlayCardCount;
			GameStation.iBeginStation=m_iBeginStation;
			GameStation.iEndStation=m_iEndStation;
			GameStation.iWinPoint=0;//m_iPlayNTNum[bDeskStation%2]-m_iBeginStation;			
			GameStation.iPunishPoint=0;//m_PunishPoint[bDeskStation];			
			GameStation.iUpGradePeople=m_iUpGradePeople;		
			GameStation.iPlayNTNum[0]=m_iPlayNTNum[0];		
			GameStation.iPlayNTNum[1]=m_iPlayNTNum[1];		
			GameStation.iBeenPlayGame=m_iBeenPlayGame;		

			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	}
	return false;
}

//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_SYNC_TIME:
		{
			SyncTimeStruct *pMes = (SyncTimeStruct *)pData;
			m_stUsedTimeInfo.m_iJuUseTime[bDeskStation] = pMes->iBotJuUseTime;
			m_stUsedTimeInfo.m_iBuUseTime[bDeskStation] = pMes->iBotBuUseTime;
			m_stUsedTimeInfo.m_iDuUseTime[bDeskStation] = pMes->iBotDuUseTime;
		}
		return true;
	case ASS_GM_AGREE_GAME:		//�û�ͬ����Ϸ
		{
			if (bWatchUser)
				return FALSE;

			if (m_bGameStation == GS_WAIT_SETGAME)	//��һ�������������
			{
				if (uSize!=sizeof(UserArgeeGame))
					return FALSE;
				UserArgeeGame * pGameSet=(UserArgeeGame *)pData;
				if (!UserSetGame(pGameSet,bDeskStation))
					return FALSE;
				__super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
				return TRUE;
			}
			//�������
			if ((m_bGameStation != GS_WAIT_ARGEE)&&(m_bGameStation != GS_WAIT_NEXT)) 
				return TRUE;
			return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}
	case ASS_OUT_CARD:		//�û�����
		{
			if(bWatchUser)
				return false;
			if(m_bGameStation == GS_WAIT_NEXT 
				|| m_bGameStation == GS_WAIT_ARGEE 
				|| m_bGameStation == GS_WAIT_SETGAME)
				return true;
			if (uSize != sizeof(PlayChessMsg))
				return false;
			PlayChessMsg * pPlayChessInfo =(PlayChessMsg *)pData;

			UserOutCard(bDeskStation,pPlayChessInfo->iChessArray,pPlayChessInfo->iPlayFlag,pPlayChessInfo->iEndI,pPlayChessInfo->iEndJ);
			return true;
		}

	case ASS_REN_SHU://����
		{
			if (bWatchUser)
				return false;
			if(m_bGameStation == GS_WAIT_NEXT 
				|| m_bGameStation == GS_WAIT_ARGEE 
				|| m_bGameStation == GS_WAIT_SETGAME)
				return true;
			if (uSize != sizeof(LoserStruct))
				return false;
			LoserStruct * pLoser = (LoserStruct*)pData;
			m_iWinFlag = (pLoser->bDeskStation + 1) % PLAY_COUNT;
			GameFinish(bDeskStation,GF_LOSER);
			return true;			
		}
	case ASS_SUPER_TIMER://��ʱ��������
		{
			if (bWatchUser)
				return false;
			if(m_bGameStation == GS_WAIT_NEXT 
				|| m_bGameStation == GS_WAIT_ARGEE 
				|| m_bGameStation == GS_WAIT_SETGAME)
				return true;
			if (uSize != sizeof(SuperTimerStruct))
				return false;
			SuperTimerStruct * pSuper = (SuperTimerStruct*)pData;
			m_iWinFlag = (pSuper->bDeskStation + 1) % PLAY_COUNT;
			GameFinish(bDeskStation,GF_SUPER_TIMER);
			return true;
		}
	case ASS_HUI_QI://����
		{
			SendGameData((bDeskStation+1)%2,MDM_GM_GAME_NOTIFY,ASS_HUI_QI,0);
			m_bIsInHuiquStation[bDeskStation] = true;
			return true;
		}
	case ASS_AGREE_HUI_QI://����
		{
			int iflag = (int)pNetHead->bHandleCode;
			m_bIsInHuiquStation[(bDeskStation+1)%2] = false;
			m_bChessingInHuiQi[(bDeskStation+1)%2] = true;

			if (iflag == 0)	//��ͬ�����
			{
				SendGameData((bDeskStation+1)%2,MDM_GM_GAME_NOTIFY,ASS_AGREE_HUI_QI,iflag);
				return true;
			}

			RegretChessMsg regretInfo;

			m_iOutCardPeople = m_iLastDeskStation;
			regretInfo.iLastDeskStation = m_iLastDeskStation;
			::CopyMemory(regretInfo.iLastTiZiNum,m_iLastTiZiNum,sizeof(m_iLastTiZiNum));
			::CopyMemory(regretInfo.iLastChessArray,m_iLastBoard,sizeof(m_iLastBoard));

			for (int i = 0;i < 2;i++)
				SendGameData(i,&regretInfo,sizeof(regretInfo),MDM_GM_GAME_NOTIFY,ASS_AGREE_HUI_QI,1);

			SendWatchData(m_bMaxPeople,&regretInfo,sizeof(regretInfo),MDM_GM_GAME_NOTIFY,ASS_AGREE_HUI_QI,1);

			::CopyMemory(m_iBoard,m_iLastBoard,sizeof(m_iLastBoard));
			::CopyMemory(m_iTiZiNum,m_iLastTiZiNum,sizeof(m_iLastTiZiNum));

			return true;
		}


	case ASS_DIAN_MU://��Ŀ
		{   
			m_bAgreeDianmuFinish=1;//��Ҫ�����Ŀ
			CString CST;
			CST.Format("wyshdebug m_iOutCardPeople===%d,m_iLastDeskStation===%d",m_iOutCardPeople,m_iLastDeskStation);
			OutputDebugString(CST);
			SendGameData((bDeskStation+1)%2,MDM_GM_GAME_NOTIFY,ASS_DIAN_MU,0);

			return true;
		}

	case ASS_AGREE_DIAN_MU://ͬ���Ŀ
		{
			if (uSize != sizeof(DianMuResultStruct)) return false;
			DianMuResultStruct *dianMuResult = (DianMuResultStruct *) pData;

			m_bAgreeDianmuFinish=0;
			CString CST;
			CST.Format("wyshdebug ��ǰͬ���Ŀ����%d��Ӧ�����ӵ���%d",bDeskStation,(bDeskStation+1)%2);
			OutputDebugString(CST);
		      
			if (dianMuResult->bAgree&&m_bUserNetCut[(bDeskStation+1)%2]==false)
			{	
				DianMuResultStruct dianMuInfo;
				dianMuInfo.bAgree = true;
				for (int i = 0;i < 2;i++)
					SendGameData(i,&dianMuInfo,sizeof(dianMuInfo),MDM_GM_GAME_NOTIFY,ASS_AGREE_DIAN_MU,0);
				SendWatchData(m_bMaxPeople,&dianMuInfo,sizeof(dianMuInfo),MDM_GM_GAME_NOTIFY,ASS_AGREE_DIAN_MU,0);
			}
			else
			{
				DianMuResultStruct dianMuInfo;
				dianMuInfo.bAgree = false;
				SendGameData((bDeskStation+1)%2,&dianMuInfo,sizeof(dianMuInfo),MDM_GM_GAME_NOTIFY,ASS_AGREE_DIAN_MU,0);
			}
			return true;
		}

	case ASS_NUMBER_ZI://ǿ������
		{
			struct DianMu *p=(struct DianMu *)pData;
			if(m_iFinishNumCnt==0)
				::memcpy(m_idianmu,m_iBoard,sizeof(m_iBoard));
			for(int i=1;i<20;i++)
			{
				for(int j=1;j<20;j++)
				{
					if((p->qi[i][j])==3)
						m_idianmu[i][j]=0;
				}
			}
			m_iFinishNumCnt++;
			if (m_iFinishNumCnt == 2)
			{
				m_iFinishNumCnt=0;
				int j;
				ForceNumberZi(j,m_dianmumsg);
				if (j == -1) 
						m_bQiuHe = true;
					else 
						m_iWinFlag = j;

				FinishDianMu finishdm;
				lstrcpy(finishdm.msg,(LPTSTR)(LPCTSTR) m_dianmumsg);
				::memcpy(finishdm.qi,m_idianmu,sizeof(m_idianmu));
				for (int i = 0; i < 2;i++) SendGameData(i,&finishdm,sizeof(finishdm),MDM_GM_GAME_NOTIFY,ASS_NUMBER_ZI,0);
				SendWatchData(m_bMaxPeople,&finishdm,sizeof(finishdm),MDM_GM_GAME_NOTIFY,ASS_NUMBER_ZI,0);
			}
			return true;
		}

	case ASS_DIAN_MU_RESULT://�Ե�Ŀ�������ͬ�⣬������Ŀ���������壩
		{
			m_bAgreeDianmuFinish=-1;  //ͬ���Ŀ�Ѿ�����
			int flag = (int) pNetHead->bHandleCode;
			if (flag == 1)//ͬ���Ŀ���
			{
				m_iDianMuResult++;

				if (m_iDianMuResult == 2)//��ͬ����
				{
				//	int i;
				//	CString s;
				//	ForceNumberZi(i,s);
					MessageStruct mess;
					lstrcpy(mess.Message,(LPTSTR)(LPCTSTR) m_dianmumsg);
					for (int j = 0; j < PLAY_COUNT;j++) 
						SendGameData(j,&mess,sizeof(mess),MDM_GM_GAME_NOTIFY,ASS_DIAN_MU_RESULT,flag);
					SendWatchData(m_bMaxPeople,&mess,sizeof(mess),MDM_GM_GAME_NOTIFY,ASS_DIAN_MU_RESULT,flag);

				//	if (i == -1) 
				//		m_bQiuHe = true;
				//	else 
				//		m_iWinFlag = i;
					GameFinish(0,GF_NORMAL);
				}
				//AfxMessageBox("ͬ���Ŀ���");
			}
			else if (flag == 2)//������Ŀ
			{
				m_iFinishNumCnt = 0;
				m_iDianMuResult = 0;

				for (int i = 0; i < 2;i++) SendGameData(i,MDM_GM_GAME_NOTIFY,ASS_DIAN_MU_RESULT,flag);
				SendWatchData(m_bMaxPeople,MDM_GM_GAME_NOTIFY,ASS_DIAN_MU_RESULT,flag);
				//AfxMessageBox("������Ŀ");
			}
			else if (flag == 3)//��������
			{
				m_iFinishNumCnt = 0;
				m_iDianMuResult = 0;

				for (int i = 0; i < PLAY_COUNT;i++) 
					SendGameData(i,MDM_GM_GAME_NOTIFY,ASS_DIAN_MU_RESULT,flag);
				SendWatchData(m_bMaxPeople,MDM_GM_GAME_NOTIFY,ASS_DIAN_MU_RESULT,flag);
				//AfxMessageBox("��������");
			}

			return true;
		}
	case ASS_HAVE_THING:	//���������뿪
		{
			if(bWatchUser)
				return false;
			HaveThingStruct * pThing =( HaveThingStruct *)pData;
			 UserHaveThing(bDeskStation,pThing->szMessage);
			return true;
		}
	case ASS_LEFT_RESULT: //ͬ���û��뿪
		{
			if(bWatchUser)
				return false;
			LeaveResultStruct * pLeave=(LeaveResultStruct *)pData;
			ArgeeUserLeft(bDeskStation,pLeave->bArgeeLeave);
			return true;
		}
	case ASS_STOP_THING:
		{
			if(bWatchUser)
				return false;
			SendGameData((bDeskStation + 1) % PLAY_COUNT,MDM_GM_GAME_NOTIFY,ASS_STOP_THING,0);
			return true;
		}
	case ASS_GET_GAME_USED_TIME:
		{
			m_stUsedTimeInfo=*((UsedTimeInfo*)pData);
			return true;
		}
	case ASS_AGREE_STOP:
		{
			if(bWatchUser)
				return false;
			StopResultStruct * pStop=(StopResultStruct *)pData;
			if(pStop->bArgee==1)
			{
				//ͬ��
				StopResultStruct stop;
				stop.bArgee=true;
				SendGameData((bDeskStation+1)%2,&stop,sizeof(stop),MDM_GM_GAME_NOTIFY,ASS_AGREE_STOP,0);

				MessageStruct Mess;

				memset(&Mess,0,sizeof(Mess));
				lstrcpy(Mess.Message,"���ĶԼ�������ǰ�������֣�");
				SendWatchData(m_bMaxPeople,&Mess,sizeof(Mess),MDM_GM_GAME_NOTIFY,ASS_MESSAGE,0);
				//������Ϸ
				if (m_bGameStation==GS_PLAY_GAME) 
				{
					m_bQiuHe = true;
					GameFinish(0,GF_NORMAL);
				}
			}
			else
			{
				StopResultStruct stop;
				stop.bArgee=false;
				SendGameData((bDeskStation+1)%2,&stop,sizeof(stop),MDM_GM_GAME_NOTIFY,ASS_AGREE_STOP,0);
			}
			return true;
		}
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

//�����ʱ��
void CServerGameDesk::KillAllTimer()
{
	KillTimer(TIME_SEND_CARD);
	KillTimer(TIME_WAIT_NT);
	KillTimer(TIME_GAME_FINISH);
}
//��Ҷ��ߴ���
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	CString CST;
	CST.Format("wyshdebug UserNetCut bDeskStation===%d",bDeskStation);
    OutputDebugString(CST);

	m_bUserNetCut[bDeskStation]=true;

	return __super::UserNetCut(bDeskStation,  pLostUserInfo);
}
//�û����½�����Ϸ
bool CServerGameDesk::UserReCome(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(i==bDeskStation)
			continue;
		SendGameData(i,MDM_GM_GAME_NOTIFY,ASS_GET_GAME_USED_TIME,0);
	}
	m_bUserNetCut[bDeskStation]=false;

	return __super::UserReCome(bDeskStation,pLostUserInfo);

}
//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	KillAllTimer();

	m_Endi = m_Endj = -1;
	if ((bLastStation == GFF_FORCE_FINISH)||(bLastStation == GF_SALE))
	{
		m_iLessCount = 0;
		m_iPlayCardCount = 0;
		m_iBeginStation = 0;
		m_iEndStation = 0;
		m_iBeenPlayGame = 0;
		m_iUpGradePeople = -1;
		::memset(m_iPlayNTNum,0,sizeof(m_iPlayNTNum));
	}

//	m_bQueryQuit=false;
	m_iLeaveArgee = 0;
//	m_iGamePoint = 0;
//	m_iBaseOutCount = 0;
//	m_iFirstOutPeople = -1;
	m_iOutCardPeople = -1;
//	m_iNtPeople = -1;
//	m_iNtCardCount = 0;
//	m_iSendCardPos = 0;
	return TRUE;
}
//�û�������Ϸ
BOOL CServerGameDesk::UserSetGame(UserArgeeGame * pGameSet,BYTE desk)
{
	//Ч������
	if (m_bGameStation != GS_WAIT_SETGAME)
		return FALSE;

	m_bFirstRed = true;
	m_iBeenPlayGame = 0;
	m_bGameStation = GS_WAIT_ARGEE;

	m_iJuTimeMinute = pGameSet->iJuTimeMinute;
	m_iJuTimeSecond = pGameSet->iJuTimeSecond;
	m_iBuTimeMinute = pGameSet->iBuTimeMinute;
	m_iBuTimeSecond = pGameSet->iBuTimeSecond;
	m_iDuMiaoMinute = pGameSet->iDuMiaoMinute;
	m_iDuMiaoSecond = pGameSet->iDuMiaoSecond;

	//������Ϣ
	UserArgeeGame Agree;
	Agree.iJuTimeMinute = pGameSet->iJuTimeMinute;
	Agree.iJuTimeSecond = pGameSet->iJuTimeSecond;
	Agree.iBuTimeMinute = pGameSet->iBuTimeMinute;
	Agree.iBuTimeSecond = pGameSet->iBuTimeSecond;
	Agree.iDuMiaoMinute = pGameSet->iDuMiaoMinute;
	Agree.iDuMiaoSecond = pGameSet->iDuMiaoSecond;
	for (int i = 0; i < PLAY_COUNT; i ++)
		if(i != desk)//���������������
			SendGameData(i,&Agree,sizeof(Agree),MDM_GM_GAME_NOTIFY,ASS_UG_USER_SET,0);
	SendWatchData(m_bMaxPeople,&Agree,sizeof(Agree),MDM_GM_GAME_NOTIFY,ASS_UG_USER_SET,0);

	return TRUE;
}
//��Ϸ��ʼ
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag)==false) 
		return false;

	memset(m_bIsInHuiquStation, false, sizeof(m_bIsInHuiquStation));
	memset(m_bChessingInHuiQi, false, sizeof(m_bChessingInHuiQi));
	
	for (int i=0;i<PLAY_COUNT;i++)
	{
		m_bUserNetCut[i]=false;
	}
	//����״̬
	m_bQiuHe = false;
//	m_bQueryQuit = false;
//	m_iNtPeople = -1;
//	m_iNtCardCount = 0;
//	m_iSendCardPos = 0;
	m_bAgreeDianmuFinish=-1; //ͬ���Ŀ��ɱ�ʾ
	m_iBeenPlayGame ++;

	m_iFinishNumCnt = 0;
	m_iDianMuResult = 0;
	//����
	for (int i = 0; i < WQ_MAX_I; i ++)
		for (int j = 0;j < WQ_MAX_J;j++)
		{
			if (i == 0 || j == 0 || i == WQ_MAX_I - 1 || j == WQ_MAX_J - 1)
			{
				m_iBoard[i][j] = WQ_FLAG_SIDE;
				continue;
			}
			else 
				m_iBoard[i][j] = WQ_FLAG_NONE;

		}

		::memset(m_iQiZiNum,0,sizeof(m_iQiZiNum));
		::memset(m_iQiZiLastNum,0,sizeof(m_iQiZiLastNum));
		::memset(m_iTiZiNum,0,sizeof(m_iTiZiNum));

		::memset(m_iLastBoard,0,sizeof(m_iLastBoard));
		::memset(m_iLastTiZiNum,0,sizeof(m_iLastTiZiNum));
	
		BeginPlay();
		return TRUE;
}

//��Ϸ��ʼ
BOOL CServerGameDesk::BeginPlay()
{
	//��������
	//��������
/*	if(m_iUpGradePeople == -1)
		m_iUpGradePeople = rand() % PLAY_COUNT;
	else
		m_iUpGradePeople = (m_iUpGradePeople + 1) % PLAY_COUNT;//�����Ⱥ���
*/	
	if(-1 == m_iUpGradePeople)
		m_iUpGradePeople= WQ_PLAY_FLAG_BLACK;
	else
	{
		if(m_iUpGradePeople == WQ_PLAY_FLAG_BLACK)		//�����Ⱥ���
			m_iUpGradePeople = WQ_PLAY_FLAG_WHITE;
		else if(m_iUpGradePeople == WQ_PLAY_FLAG_WHITE)
			m_iUpGradePeople = WQ_PLAY_FLAG_BLACK;
	}
	
	m_Qiflag[m_iUpGradePeople] = WQ_PLAY_FLAG_BLACK;
	m_Qiflag[(m_iUpGradePeople + 1) % PLAY_COUNT] = WQ_PLAY_FLAG_WHITE;
	m_bGameStation = GS_PLAY_GAME;
    //����˭���֣������裬�ͻ��˻������Ϣ���е��û���Ϊ��BUG
	m_iOutCardPeople = m_iUpGradePeople;
	//������Ϣ
	PrepareStruct stPrepare;
	stPrepare.bDeskStation = m_iOutCardPeople;

	for (int i = 0; i < m_bMaxPeople; i ++) 
	{
		SendGameData(i,&stPrepare,sizeof(stPrepare),MDM_GM_GAME_NOTIFY,ASS_GAME_PREPARE,0);
	}
	SendWatchData(m_bMaxPeople,&stPrepare,sizeof(stPrepare),MDM_GM_GAME_NOTIFY,ASS_GAME_PREPARE,0);


	m_iOutCardPeople = m_iUpGradePeople;
	//������Ϸ��ʼ��Ϣ
	BeginPlayStruct Begin;
	Begin.bDeskStation = m_iOutCardPeople;
	for (int i = 0; i < m_bMaxPeople; i ++) 
	{
		SendGameData(i,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
	}
	SendWatchData(m_bMaxPeople,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);

	GameReady();

	return TRUE;
}

//�û�����
BOOL CServerGameDesk::UserOutCard(BYTE bDeskStation, int iPlayQi[][21], int iPlayFlag, int iEndI, int iEndJ)
{
	if (bDeskStation != m_iOutCardPeople)
		return FALSE;

	m_bChessingInHuiQi[bDeskStation] = false;
	//���浱ǰ���
	m_iLastDeskStation = bDeskStation;
	::CopyMemory(m_iLastBoard,m_iBoard,sizeof(m_iBoard));
	::CopyMemory(m_iLastTiZiNum,m_iTiZiNum,sizeof(m_iTiZiNum));
	m_Endi = iEndI;
	m_Endj = iEndJ;
	m_iLastDeskStation = bDeskStation;

	::CopyMemory(m_iBoard,iPlayQi,sizeof(m_iBoard));

	::memset(m_iQiZiNum,0,sizeof(m_iQiZiNum));

	//ͳ����������
	for (int i = 1; i < WQ_MAX_I - 1; i ++)
		for (int j = 1; j < WQ_MAX_J - 1; j ++)
		{
			if (m_iBoard[i][j] == WQ_FLAG_BLACK || m_iBoard[i][j] == WQ_FLAG_CUR_BLACK)
			{
				m_iQiZiNum[WQ_PLAY_FLAG_BLACK] ++;
				continue;
			}
			if (m_iBoard[i][j] == WQ_FLAG_WHITE || m_iBoard[i][j] == WQ_FLAG_CUR_WHITE)
			{
				m_iQiZiNum[WQ_PLAY_FLAG_WHITE]++;
				continue;
			}
		}

		//ͳ��������
		PlayChessMsg playChessInfo;
		::memset(&playChessInfo,0,sizeof(playChessInfo));
		playChessInfo.bDeskStation = bDeskStation;

		///::memset(m_iTiZiNum,0,sizeof(m_iTiZiNum));
		int iCnt;
		if (m_Qiflag[bDeskStation] == WQ_PLAY_FLAG_BLACK)//�ڷ�����ͳ������׷�����
		{
			iCnt = m_iQiZiLastNum[WQ_PLAY_FLAG_WHITE] - m_iQiZiNum[WQ_PLAY_FLAG_WHITE];
			if (iCnt < 0) iCnt = 0;
			m_iTiZiNum[WQ_PLAY_FLAG_BLACK] += iCnt;
			playChessInfo.iTiZiNum[WQ_PLAY_FLAG_BLACK] = m_iTiZiNum[WQ_PLAY_FLAG_BLACK];
			playChessInfo.iTiZiNum[WQ_PLAY_FLAG_WHITE] = m_iTiZiNum[WQ_PLAY_FLAG_WHITE];
		}
		else if (m_Qiflag[bDeskStation] == WQ_PLAY_FLAG_WHITE)//�׷�����ͳ������ڷ�����
		{
			iCnt = m_iQiZiLastNum[WQ_PLAY_FLAG_BLACK] - m_iQiZiNum[WQ_PLAY_FLAG_BLACK];
			if (iCnt < 0) iCnt = 0;
			m_iTiZiNum[WQ_PLAY_FLAG_WHITE] += iCnt;
			playChessInfo.iTiZiNum[WQ_PLAY_FLAG_WHITE] = m_iTiZiNum[WQ_PLAY_FLAG_WHITE];
			playChessInfo.iTiZiNum[WQ_PLAY_FLAG_BLACK] = m_iTiZiNum[WQ_PLAY_FLAG_BLACK];
		}
		playChessInfo.iEndI = iEndI;
		playChessInfo.iEndJ = iEndJ;
		//playChessInfo.iNextPlayStation = m_iOutCardPeople;
		playChessInfo.iPlayFlag = bDeskStation;
		::CopyMemory(playChessInfo.iQiZiNum,m_iQiZiNum,sizeof(m_iQiZiNum));
		::CopyMemory(playChessInfo.iChessArray,iPlayQi,sizeof(playChessInfo.iChessArray));

		for(int i = 0; i < PLAY_COUNT; i ++) 
			SendGameData(i,&playChessInfo,sizeof(playChessInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);

		SendWatchData(m_bMaxPeople,&playChessInfo,sizeof(playChessInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);
		//int iNextPlayFlag = iPlayFlag;
		::CopyMemory(m_iQiZiLastNum,m_iQiZiNum,sizeof(m_iQiZiNum));

		///��һ������
		m_iOutCardPeople = (bDeskStation + 1) % PLAY_COUNT;

		PlayChessStruct play;
		play.bDeskStation = m_iOutCardPeople;
		for(int i = 0; i < PLAY_COUNT; i ++)
		{
			SendGameData(i,&play,sizeof(play),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
			SendWatchData(m_bMaxPeople,&play,sizeof(play),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
		}
		return TRUE;
}

//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	// ƽ̨�����벽ʱ���ܻ��ͻ���½�������, ��������ж�
	if (m_bGameStation != GS_PLAY_GAME)
		return true;

	//��д����
	switch (bCloseFlag)
	{
	case GF_LOSER://����
		{
			//������һ��
			m_bGameStation = GS_WAIT_ARGEE;
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}
			GameEndStruct loser;
			::memset(&loser,0,sizeof(loser));
			loser.bDeskStation = bDeskStation;

			strcpy(loser.szNickName[bDeskStation],m_pUserInfo[bDeskStation]->m_UserData.nickName);
			strcpy(loser.szNickName[(bDeskStation+1)%2],m_pUserInfo[(bDeskStation+1)%2]->m_UserData.nickName);
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if(i == bDeskStation)
					 loser.iTurePoint[i] = - BASE_SCORE;
					else
					 loser.iTurePoint[i] = BASE_SCORE;
			}
			//д�����ݿ�				
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			ChangeUserPointint64(loser.iTurePoint, temp_cut);
			__super::RecoderGameInfo(loser.iChangeMoney);
			loser.iDoublePointEffect = GetDoublePointEffect();//20081125

			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0;//��ҳ����ۻ���add by xqm 2010-12-14
			if (true == bNotCostPoint)
			{
				memset(loser.iTurePoint,0,sizeof(loser.iTurePoint));
			}
			//��������
			for (int i = 0; i < m_bMaxPeople; i ++)
				SendGameData(i, &loser, sizeof(loser), MDM_GM_GAME_NOTIFY, ASS_LOSER_END, 0);
			SendWatchData(m_bMaxPeople, &loser, sizeof(loser), MDM_GM_GAME_NOTIFY, ASS_LOSER_END, 0);
			__super::GameFinish(bDeskStation,GF_SALE);
			ReSetGameState(bCloseFlag); 
			return true;
		}
	case GF_SUPER_TIMER://��ʱ����
		{
			//������һ��
			m_bGameStation = GS_WAIT_ARGEE;
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}
			GameEndStruct supertimer;
			::memset(&supertimer,0,sizeof(supertimer));
			supertimer.bDeskStation = bDeskStation;
			strcpy(supertimer.szNickName[bDeskStation],m_pUserInfo[bDeskStation]->m_UserData.nickName);
			strcpy(supertimer.szNickName[(bDeskStation+1)%2],m_pUserInfo[(bDeskStation+1)%2]->m_UserData.nickName);

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if(i == bDeskStation)
					 supertimer.iTurePoint[i] = -BASE_SCORE;
					else
					 supertimer.iTurePoint[i] = BASE_SCORE;
			}
			//д�����ݿ�				
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			ChangeUserPointint64(supertimer.iTurePoint, temp_cut);
			__super::RecoderGameInfo(supertimer.iChangeMoney);
			supertimer.iDoublePointEffect = GetDoublePointEffect();//20081125

			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0;//��ҳ����ۻ���add by xqm 2010-12-14
			if (true == bNotCostPoint)
			{
				memset(supertimer.iTurePoint,0,sizeof(supertimer.iTurePoint));
			}
			//��������
			for (int i = 0; i < m_bMaxPeople; i ++)
				SendGameData(i,&supertimer,sizeof(supertimer),MDM_GM_GAME_NOTIFY,ASS_SUPER_TIMER_END,0);
			SendWatchData(m_bMaxPeople,&supertimer,sizeof(supertimer),MDM_GM_GAME_NOTIFY,ASS_SUPER_TIMER_END,0);
			__super::GameFinish(bDeskStation,GF_SALE);
			ReSetGameState(bCloseFlag);
			return true;
		}
	case GF_NORMAL:		//��Ϸ��������
		{
			//������һ��
			m_bGameStation = GS_WAIT_NEXT;
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;				
			}
			int iBasePoint = GetRoomBasePoint();
			//��Ϸ����
		//	GetUpGrade(0,m_iPlayCardCount);
			GameEndStruct GameEnd;
			::memset(&GameEnd,0,sizeof(GameEnd));
			GameEnd.bDeskStation = m_iUpGradePeople; 
			///����÷�
			GetUpGrade(0,m_iPlayCardCount);

			for (int i=0; i<PLAY_COUNT; ++i)
			{
				if (NULL != m_pUserInfo[i])
				{
					sprintf(GameEnd.szNickName[i], "%s", m_pUserInfo[i]->m_UserData.nickName );
				}				
			}
			if(!m_bQiuHe)//add 20081127  ;��Ͷ����۷�,������ͼ������
			{
				if (m_iWinFlag == 0)
				{
					//�ڷ�ʤ
					GameEnd.iTurePoint[m_iBlackStation] = BASE_SCORE;
					GameEnd.iTurePoint[m_iWhiteStation] = - BASE_SCORE;
				}
				else if (m_iWinFlag == 1)
				{
					//�׷�ʤ

					GameEnd.iTurePoint[m_iWhiteStation] = BASE_SCORE;
					GameEnd.iTurePoint[m_iBlackStation] = - BASE_SCORE;
				}
				else
				{
					GameEnd.iTurePoint[0] = 0; 
					GameEnd.iTurePoint[1] = 0;
				}
			}
			//д�����ݿ�				
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			ChangeUserPointint64(GameEnd.iTurePoint, temp_cut);
			__super::RecoderGameInfo(GameEnd.iChangeMoney);
			GameEnd.iDoublePointEffect = GetDoublePointEffect();//20081125

			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0;//��ҳ����ۻ���add by xqm 2010-12-14
			if (true == bNotCostPoint)
			{
				memset(GameEnd.iTurePoint,0,sizeof(GameEnd.iTurePoint));
			}
			//��������
			for (int i = 0; i < m_bMaxPeople; i ++)
				SendGameData(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
			SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);

			__super::GameFinish(bDeskStation,GF_SALE);
			ReSetGameState(bCloseFlag);
			return true;
		}
	case GF_SALE:			//��Ϸ��ȫ����
		{
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}	
			int iBasePoint = GetRoomBasePoint();//����
			m_bGameStation = GS_WAIT_ARGEE;
			GameEndStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));
			CutEnd.bDeskStation = bDeskStation;
			
			for (int i = 0; i < PLAY_COUNT; i ++) 
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);
				SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);

			bCloseFlag = GFF_FORCE_FINISH;

			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GFF_FORCE_FINISH:		//�û������뿪----------(�û�ǿ��)
		{
			int iBasePoint = GetRoomBasePoint();//����

			m_bGameStation = GS_WAIT_ARGEE;

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}

			GameEndStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));

			CutEnd.bDeskStation = bDeskStation;
			strcpy(CutEnd.szNickName[bDeskStation],m_pUserInfo[bDeskStation]->m_UserData.nickName);
			strcpy(CutEnd.szNickName[(bDeskStation+1)%2],m_pUserInfo[(bDeskStation+1)%2]->m_UserData.nickName);

			CutEnd.iTurePoint[bDeskStation] =  - GetRunPublish();
			CutEnd.iTurePoint[(bDeskStation + 1) % PLAY_COUNT] = GetRunPublish();

			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			
			/*for (int i = 0; i < PLAY_COUNT; i ++)
			{
				temp_cut[i] = (bDeskStation == i && CutEnd.iTurePoint[i] < 0);
			}			
			ChangeUserPoint(CutEnd.iTurePoint, temp_cut);*/

			for (int i=0; i<PLAY_COUNT; ++i) 
			{ 
				temp_cut[i] = (bDeskStation == i)?true:false; 
			}

			ChangeUserPointint64(CutEnd.iTurePoint, temp_cut);//,CutEnd.iChangeMoney,false);
			__super::RecoderGameInfo(CutEnd.iChangeMoney);
			CutEnd.iDoublePointEffect = GetDoublePointEffect();//20081125

			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0;//��ҳ����ۻ���add by xqm 2010-12-14
			if (true == bNotCostPoint)
			{
				memset(CutEnd.iTurePoint,0,sizeof(CutEnd.iTurePoint));
			}
			
			for (int i = 0; i < PLAY_COUNT; i ++) 
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
			bCloseFlag = GFF_FORCE_FINISH;

			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	}
	//��������
	ReSetGameState(bCloseFlag);
	__super::GameFinish(bDeskStation,bCloseFlag);

	return true;
}

//��Ϸ׼��
BOOL CServerGameDesk::GameReady()
{
	/*
	if(-1 == m_iUpGradePeople)
		m_iUpGradePeople= WQ_PLAY_FLAG_BLACK;
	else
	{
		if(m_iUpGradePeople == WQ_PLAY_FLAG_BLACK)		//�����Ⱥ���
			m_iUpGradePeople = WQ_PLAY_FLAG_WHITE;
		else if(m_iUpGradePeople == WQ_PLAY_FLAG_WHITE)
			m_iUpGradePeople = WQ_PLAY_FLAG_BLACK;
	}
*/
	//����,����λ��
	m_iBlackStation = m_iUpGradePeople;
	m_iWhiteStation =(m_iUpGradePeople+1) % 2;
	GameReadyStruct GameReady;
	::memset(&GameReady,0,sizeof(GameReady));
	GameReady.iBlackStation=m_iBlackStation;
	GameReady.iWhiteStation=m_iWhiteStation;
	for (int i = 0; i < 2;i++) 
		SendGameData(i,&GameReady,sizeof(GameReady),MDM_GM_GAME_NOTIFY,ASS_GAME_READY,0);
	SendWatchData(m_bMaxPeople,&GameReady,sizeof(GameReady),MDM_GM_GAME_NOTIFY,ASS_GAME_READY,0);

	return TRUE;
}

//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if(m_bGameStation > GS_WAIT_ARGEE&&m_bGameStation < GS_WAIT_NEXT)
		return true;
	else
		return false;
	//return __super::IsPlayGame(bDeskStation);
}

//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{	
	//-------------�û���Ŀ������ҳ�ʱδ�㿪ʼ�����½�����Ϸ������������--------------
	/*if(m_pUserInfo[(bDeskStation + 1) % PLAY_COUNT] == NULL)	
		m_bGameStation = GS_WAIT_SETGAME;*/	
	//-------------------��Ŀ���µ����⣬�޸ĳ����º���Ϸ�ܹ���������--------------------
	if(m_pUserInfo[(bDeskStation+1)%2]==NULL)	
		m_bGameStation = GS_WAIT_SETGAME;	
	else
		m_bGameStation = GS_WAIT_ARGEE;

	if(m_bGameStation > GS_WAIT_ARGEE)
		GameFinish(bDeskStation,GF_SALE);

	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}


//����÷�
int	CServerGameDesk::GetUpGrade(int iGamePoint, int iPlayCardCount)
{
	if (m_bQiuHe)
	{	
		m_iTurePoint[0] = 0;
		m_iTurePoint[1] = 0;
		return 0;
	}
	if(m_iWinFlag < 0)
		return 0;
	return 0;
}

//force number qizi
void CServerGameDesk::ForceNumberZi(int &iWinFlag, CString &strMessage)
{
	float totalWhite = 0.00000,
		totalBlack = 0.00000;

	m_Logic.NumQiZi(&totalBlack,&totalWhite,m_idianmu);
	totalBlack -= WQ_BLACK_TIE_MU;
	totalWhite += WQ_BLACK_TIE_MU;

	float blackWinMu = totalBlack - totalWhite;
	int flag = (blackWinMu > 0.00000) ? 1 : ((blackWinMu == 0.00000) ? 0 : -1);
	blackWinMu = (flag > 0) ? blackWinMu : -blackWinMu;
	int count = (int) (blackWinMu);//��������
	int fraction = (int) ((blackWinMu - count) * 100);//С������
	int winFlag = 0;
	CString s;

	if (flag > 0)//�ڷ�Ӯ
	{
		winFlag = WQ_PLAY_FLAG_BLACK;
		s = TEXT("�ڷ�Ӯ");
		goto ret;
	}
	else if (flag < 0)//�׷�Ӯ
	{
		winFlag = WQ_PLAY_FLAG_WHITE;
		s = TEXT("�׷�Ӯ");
		goto ret;
	}
	else//ƽ��
	{
		winFlag = -1;
		s = TEXT("�˾�Ϊƽ��");
	}
ret:if (fraction != 0)
	{
		CString str;
		str.Format("%d",count);
		s += str;
		switch (fraction)
		{
		case 25:
			s += TEXT("��1/4��");
			break;

		case 50:
			s += TEXT("��1/2��");
			break;

		case 75:
			s += TEXT("��3/4��");
			break;
		}
	}
//�˴��ж������Ӯʱ��������ǻ���ͬһ������
//�޸� : wushuqun 2008.8.5

//	iWinFlag = m_Qiflag[winFlag];
	iWinFlag = winFlag;
	strMessage = s;
	return;
}


//�û������뿪
BOOL CServerGameDesk::UserHaveThing(BYTE bDeskStation, char * szMessage)
{
	if (m_bGameStation < GS_PLAY_GAME)
		return TRUE;

	m_iHaveThingPeople = bDeskStation;
	m_iLeaveArgee|=1<<bDeskStation;
	if (m_iLeaveArgee != 15)
	{
		HaveThingStruct HaveThing;
		HaveThing.bDeskStation = bDeskStation;
		::CopyMemory(HaveThing.szMessage,szMessage,60*sizeof(char));
		for (int i = 0; i < m_bMaxPeople; i ++)
			if (i != bDeskStation)
				SendGameData(i,&HaveThing,sizeof(HaveThing),MDM_GM_GAME_NOTIFY,ASS_HAVE_THING,0);
	}
	else GameFinish(bDeskStation,GF_SALE);

	return TRUE;
}

//ͬ���û��뿪
BOOL CServerGameDesk::ArgeeUserLeft(BYTE bDeskStation, BOOL bArgee)
{
	if (!bArgee)
	{
//		m_bQueryQuit=true;
		LeaveResultStruct Leave;
		Leave.bDeskStation = bDeskStation;
		Leave.bArgeeLeave = bArgee;
		for (int i = 0; i < m_bMaxPeople; i ++) 
			if (i != bDeskStation)
				SendGameData(i,&Leave,sizeof(Leave),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	}
	//else
	//	GameFinish((bDeskStation + 1) % PLAY_COUNT,GF_SALE);
	//
	else //JMod-�Զ��뿪-20090531
	{
		GameFinish(m_iHaveThingPeople,GF_SALE);
		LeaveResultStruct Leave1;
		Leave1.bDeskStation = m_iHaveThingPeople;
		Leave1.bArgeeLeave = true;
		SendGameData(m_iHaveThingPeople,&Leave1,sizeof(Leave1),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	}

	return TRUE;
}


//���ܿ۷�
int CServerGameDesk::GetRunPublishMoney()
{
	return m_pDataManage->m_InitData.uRunPublish;
	//return m_pDataManage->m_InitData.uRunPublishMoney;
}

//���ܿ۷�
int CServerGameDesk::GetRunPublish()
{
	return m_pDataManage->m_InitData.uRunPublish;
}

//�Α���A����
int CServerGameDesk::GetRoomBasePoint()
{
	int iBasePoint = m_pDataManage->m_InitData.uBasePoint;//����
	//	int iBasePoint = m_pDataManage->m_InitData.uScoreRate;//����
	return iBasePoint;
}

//���ӱ���
int CServerGameDesk::GetDeskBasePoint()
{
	return 1;
	//	return GetTableBasePoint();
}

