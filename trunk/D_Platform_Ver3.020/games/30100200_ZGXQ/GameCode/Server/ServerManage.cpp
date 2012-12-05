#include "StdAfx.h"
#include "ServerManage.h"

/**
 * ���캯��
 */
CServerGameDesk::CServerGameDesk(void):CGameDesk(FULL_BEGIN)
{
	m_daoshi = -1;
	m_bRequireCoachMoeny = false;
	m_bQueryQuit=false;//����������˳�
	m_bFirstRed = true;
	m_iEnableWatch=0;//�Ƿ������Թ�
	m_iLeaveArgee=0;//�뿪ͬ���־
	m_iNtCardCount=0;//��������
	m_iLessCount=0;//������Ϸ����
	m_iPlayCardCount=0;//��Ϸ�˿���Ŀ
	m_iBeginStation=0;//��Ϸ��ʼλ��
	m_iEndStation=0;//��Ϸ����λ��
	m_iBaseOutCount=0;//���Ƶ���Ŀ
	m_iNtPeople=-1;//�������
	m_iFirstOutPeople=0;//�ȳ��Ƶ��û�
	m_iOutCardPeople=-1;//���ڳ����û�
	m_iUpGradePeople=-1;//ׯ��λ��
	m_iStartI=m_iStartJ=m_iEndI=m_iEndJ-1;
	m_iBeenPlayGame=0;//�Ѿ���Ϸ�ľ���
	m_iGamePoint=0;//�û���Ϸ����
	for (int i=0; i<PLAY_COUNT; i++)
		m_iBgePeaceCount[i] = 3;         //��ʹ���
	m_bGameStation=GS_WAIT_SETGAME;//��Ϸ״̬
	m_CardCount=PLAY_TWO_CARD;//�˿���Ŀ
	::memset(&m_PunishPoint,0,sizeof(m_PunishPoint));//�û�����
	::memset(m_iPlayNTNum,0,sizeof(m_iPlayNTNum));//����ҵ�������Ŀ
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));	//�����˿˵���Ŀ

	m_firstPlay = 0;
	//�������Ĳ���
	::memset(m_iPutChess,0,sizeof(m_iPutChess));
}

/**
 * ��������
 */
CServerGameDesk::~CServerGameDesk(void)
{
}
bool CServerGameDesk::InitDeskGameStation()
{
	LoadExtIni();
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	return true;
}
//���ؾ�������
BOOL CServerGameDesk::LoadExtIni()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CString nid;
	nid.Format("%d",NAME_ID);
	CINIFile f( s +nid +"_s.ini");
	TCHAR szKey[20];
	wsprintf(szKey,"game");
	CString key = szKey;
	m_bRequireCoachMoeny = f.GetKeyVal(key,"RequireCoachMoeny",0);
	return TRUE;
}

//���ݷ���ID���������ļ�
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{
	TCHAR szKey[20];
	CString nid;
	nid.Format("%d",NAME_ID);
	wsprintf(szKey,"%s_%d",nid,iRoomID);
	CString key = szKey;
	CString s = CINIFile::GetAppPath ();
	CINIFile f( s +nid +"_s.ini");
	m_bRequireCoachMoeny = f.GetKeyVal(key,"RequireCoachMoeny",m_bRequireCoachMoeny);
	return TRUE;
}

/**
 * ��ʱ����Ϣ
 */
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	if (uTimerID==TIME_SEND_CARD)	//���ƶ�ʱ��
	{
		if (m_bGameStation==GS_SEND_CARD) SendCard();
		else KillTimer(TIME_SEND_CARD);
		return TRUE;
	}
	if (uTimerID==TIME_WAIT_NT)		//���ƶ�ʱ��
	{
		if (m_bGameStation==GS_SEND_CARD) SendBackCard();
		KillTimer(TIME_WAIT_NT);
		return TRUE;
	}
	if (uTimerID==TIME_GAME_FINISH)	//������ʱ��
	{
		KillTimer(TIME_GAME_FINISH);
		if (m_bGameStation==GS_PLAY_GAME)
			GameFinish(0,GF_NORMAL);
	}
	return __super::OnTimer(uTimerID);
}

/**
 * ��ȡ��Ϸ״̬��Ϣ
 */
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
		{
			GameStation_1 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			GameStation.iCardCount=m_CardCount;
			GameStation.iMainVersion=MAIN_VERSION;
			GameStation.iLessVersion=ASS_VERSION;
				//��Ϸ�汾�˶�
			GameStation.iVersion=DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2=DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();
			GameStation.bRequireCoachMoeny = m_bRequireCoachMoeny;
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));

			MessageStruct Mess;
			ZeroMemory(Mess.Message,200);
			lstrcpy(Mess.Message,"��ӭ���μ�������Ϸ��ף����ÿ��ģ�");
			SendGameData(bDeskStation,&Mess,sizeof(Mess),MDM_GM_GAME_NOTIFY,ASS_MESSAGE,0);

			return TRUE;
		}
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			GameStation_2 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));

			GameStation.iJuTimeMinute = m_iJuTimeMinute;
			GameStation.iJuTimeSecond = m_iJuTimeSecond;
			GameStation.iBuTimeMinute = m_iBuTimeMinute;
			GameStation.iBuTimeSecond = m_iBuTimeSecond;
			GameStation.iDuMiaoMinute = m_iDuMiaoMinute;
			GameStation.iDuMiaoSecond = m_iDuMiaoSecond;
			GameStation.iGameMoney = m_iGameMoney;
			GameStation.iMainVersion=MAIN_VERSION;
			GameStation.iLessVersion=ASS_VERSION;
			//��Ϸ�汾�˶�
			GameStation.iVersion=DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2=DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();
			GameStation.bRequireCoachMoeny = m_bRequireCoachMoeny;

			GameStation.bDaoShi = m_daoshi;

			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	case GS_SEND_CARD:		//����״̬
	case GS_WAIT_BACK:		//�ȴ������״̬
		{
			GameStation_3 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			GameStation.iPlayCount=m_iLessCount;
			GameStation.iCardCount=m_iPlayCardCount;
			//��Ϸ�汾�˶�
			GameStation.iVersion=DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2=DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();

			GameStation.iBeginStation=m_iBeginStation;
			GameStation.iEndStation=m_iEndStation;
			GameStation.iMeStation=m_iPlayNTNum[(bDeskStation)%2];
			GameStation.iOtherStation=m_iPlayNTNum[(bDeskStation+1)%2];

			GameStation.iWinPoint=m_iPlayNTNum[bDeskStation%2]-m_iBeginStation;			
			GameStation.iPunishPoint=m_PunishPoint[bDeskStation];			
			GameStation.iUpGradePeople=m_iUpGradePeople;		
			GameStation.iPlayNTNum[0]=m_iPlayNTNum[0];		
			GameStation.iPlayNTNum[1]=m_iPlayNTNum[1];		
			GameStation.iBeenPlayGame=m_iBeenPlayGame;		
			GameStation.iNtPeople=m_iNtPeople;			
			GameStation.iNtCardCount=m_iNtCardCount;			
			GameStation.iNtHua=0;//m_Logic.GetNTHuaKind();
			GameStation.bRequireCoachMoeny = m_bRequireCoachMoeny;
			GameStation.bDaoShi = m_daoshi;

			GameStation.iUserCardCount=m_iUserCardCount[bDeskStation];		
			::CopyMemory(GameStation.iUserCard,m_iUserCard[bDeskStation],sizeof(BYTE)*m_iUserCardCount[bDeskStation]);

			//��������
			int iSendSize=sizeof(GameStation)-sizeof(GameStation.iUserCard)+sizeof(BYTE)*m_iUserCardCount[bDeskStation];
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,iSendSize);
			return TRUE;
		}
	case GS_PLAY_GAME:	//��Ϸ��״̬
		{
			GameStation_4 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			GameStation.iPlayCount=m_iLessCount;
			GameStation.iBeenPlayGame=m_iBeenPlayGame;		
			//��Ϸ�汾�˶�
			GameStation.iVersion=DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2=DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();

			GameStation.iJuTimeMinute = m_iJuTimeMinute;
			GameStation.iJuTimeSecond = m_iJuTimeSecond;
			GameStation.iBuTimeMinute = m_iBuTimeMinute;
			GameStation.iBuTimeSecond = m_iBuTimeSecond;
			GameStation.iDuMiaoMinute = m_iDuMiaoMinute;
			GameStation.iDuMiaoSecond = m_iDuMiaoSecond;
			GameStation.iRegretStation = m_bStationRegret;

			GameStation.bRequireCoachMoeny = m_bRequireCoachMoeny;
			GameStation.bDaoShi = m_daoshi;

			memcpy(GameStation.iJuUseTime, m_iBotJuUseTime, sizeof(m_iBotJuUseTime));
			memcpy(GameStation.iBuUseTime, m_iBotBuUseTime, sizeof(m_iBotBuUseTime));
			memcpy(GameStation.iDuUseTime, m_iBotDuUseTime, sizeof(m_iBotDuUseTime));

			GameStation.iStartI=m_iStartI;
			GameStation.iStartJ=m_iStartJ;
			GameStation.iEndI=m_iEndI;
			GameStation.iEndJ=m_iEndJ;
			::CopyMemory(GameStation.iBgePeaceCount,m_iBgePeaceCount,sizeof(m_iBgePeaceCount));
			switch(m_iOutCardPeople)
			{	
			case 0:
				m_iCurPlayFlag = m_bFirstRed?-1:1;break;
			case 1:
				m_iCurPlayFlag = m_bFirstRed?1:-1;break;
			}
			GameStation.iOutCardPeople = m_iCurPlayFlag;
			GameStation.iLastOutPeople=m_iFirstOutPeople;
			//��������

			m_iWatchStation = bDeskStation;

			if (!m_bFirstRed)
			{	
				if (bDeskStation == 0)
				{
					GameStation.iFirstOutPeople = XQ_FLAG_RED;
					::CopyMemory(GameStation.iQiZi,m_iRedBoard,sizeof(m_iRedBoard));
					int iSendSize=sizeof(GameStation);//-sizeof(GameStation.iUserCardList)+sizeof(BYTE)+iPos;
					SendGameStation(0,uSocketID,bWatchUser,&GameStation,iSendSize);
				}
				if(bDeskStation == 1)
				{
					GameStation.iFirstOutPeople = XQ_FLAG_BLACK;
					::CopyMemory(GameStation.iQiZi,m_iBlackBoard,sizeof(m_iBlackBoard));
					int iSendSize=sizeof(GameStation);//-sizeof(GameStation.iUserCardList)+sizeof(BYTE)+iPos;
					SendGameStation(1,uSocketID,bWatchUser,&GameStation,iSendSize);
				}


			}
			else
			{
				if (bDeskStation == 1)
				{
					GameStation.iFirstOutPeople = XQ_FLAG_RED;
					::CopyMemory(GameStation.iQiZi,m_iRedBoard,sizeof(m_iRedBoard));
					int iSendSize=sizeof(GameStation);//-sizeof(GameStation.iUserCardList)+sizeof(BYTE)+iPos;
					SendGameStation(1,uSocketID,bWatchUser,&GameStation,iSendSize);
				}
				if(bDeskStation == 0)
				{
					GameStation.iFirstOutPeople = XQ_FLAG_BLACK;
					::CopyMemory(GameStation.iQiZi,m_iBlackBoard,sizeof(m_iBlackBoard));
					int iSendSize=sizeof(GameStation);//-sizeof(GameStation.iUserCardList)+sizeof(BYTE)+iPos;
					SendGameStation(0,uSocketID,bWatchUser,&GameStation,iSendSize);
				}
			}
			return TRUE;

		}
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{
			GameStation_5 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();
			GameStation.iJuTimeMinute = m_iJuTimeMinute;
			GameStation.iJuTimeSecond = m_iJuTimeSecond;
			GameStation.iBuTimeMinute = m_iBuTimeMinute;
			GameStation.iBuTimeSecond = m_iBuTimeSecond;
			GameStation.iDuMiaoMinute = m_iDuMiaoMinute;
			GameStation.iDuMiaoSecond = m_iDuMiaoSecond;
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
			//��Ϸ�汾�˶�
			GameStation.iVersion=DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2=DEV_LOW_VERSION;
			GameStation.bRequireCoachMoeny = m_bRequireCoachMoeny;

			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	}
	return false;
}

/**
 * ������Ϸ״̬
 */
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	KillTimer(TIME_SEND_CARD);
	KillTimer(TIME_WAIT_NT);
	KillTimer(TIME_GAME_FINISH);
	m_iStartI=m_iStartJ=m_iEndI=m_iEndJ-1;
	if ((bLastStation==GFF_FORCE_FINISH)||(bLastStation==GF_SALE))
	{
		m_iLessCount=0;
		m_iPlayCardCount=0;
		m_iBeginStation=0;
		m_iEndStation=0;
		m_iBeenPlayGame=0;
		m_iUpGradePeople=-1;
		//m_Logic.SetNTCardNum(0);
		::memset(m_iPlayNTNum,0,sizeof(m_iPlayNTNum));
		::memset(m_PunishPoint,0,sizeof(m_PunishPoint));
	}

	for (int i=0; i<PLAY_COUNT; i++)
		m_iBgePeaceCount[i] = 3;
	m_bQueryQuit=false;
	m_iLeaveArgee=0;
	m_iGamePoint=0;
	m_iBaseOutCount=0;
	m_iFirstOutPeople=0;
	m_iOutCardPeople=-1;
	m_iNtPeople=-1;
	m_iNtCardCount=0;
	m_iSendCardPos=0;
	m_bStationRegret = -1;
	//	m_Logic.SetNTHuaKind(UG_ERROR_HUA);
	memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));

	return TRUE;
}

/**
 * ��Ϸ��ʼ
 */
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag)==false) 
		return false;

	//����״̬
	m_bQiuHe = false;
	m_bStationRegret = -1;
	m_bQueryQuit=false;
	m_iNtPeople=-1;
	m_iNtCardCount=0;
	m_iSendCardPos=0;
	m_iBeenPlayGame++;
	//	m_Logic.SetNTHuaKind(UG_ERROR_HUA);
	//m_bGameStation=GS_SEND_CARD;
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));

	::memset(m_iRedBoard,0,sizeof(m_iRedBoard));
	::memset(m_iBlackBoard,0,sizeof(m_iBlackBoard));


	::memset(m_iPutChess,0,sizeof(m_iPutChess));
	//���Ϳ�ʼ��Ϣ
	BeginUpgradeStruct BeginMessage;
	BeginMessage.iNtStation=0;//m_Logic.GetNTCardNum();
	BeginMessage.iStation[0]=0;//m_iPlayNTNum[0];
	BeginMessage.iStation[1]=0;//m_iPlayNTNum[1];
	BeginMessage.iPlayCount=m_iBeenPlayGame;
	BeginMessage.idaoshi = m_daoshi;
	::CopyMemory(BeginMessage.BgePeaceCount,m_iBgePeaceCount,sizeof(m_iBgePeaceCount));
	BeginMessage.iUpgradeStation=m_iUpGradePeople;
	for (int i=0;i<2;i++) 
		SendGameData(i,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_BEGIN_UPGRADE,0);
	SendWatchData(m_bMaxPeople,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_BEGIN_UPGRADE,0);

	//�ַ��˿�
	//BYTE iCardArray[162];
	//m_Logic.RandCard(iCardArray,(m_iPlayCardCount==2)?108:162);
	//for (int i=0;i<4;i++)
	///::CopyMemory(m_iUserCard[i],&iCardArray[((m_iPlayCardCount==2)?25:39)*i],sizeof(BYTE)*((m_iPlayCardCount==2)?25:39));
	//::CopyMemory(m_iBackCard,&iCardArray[(m_iPlayCardCount==2)?100:156],sizeof(BYTE)*((m_iPlayCardCount==2)?8:6));

	//������Ϣ
	///SetTimer(TIME_SEND_CARD,200);

	//��ʼ��Ϸ


	GameReady();

	BeginPlayUpGrade();

	return TRUE;
}

/**
 * ��Ϸ׼��
 */
BOOL CServerGameDesk::GameReady()
{
	if (m_bFirstRed) 
		m_iUpGradePeople = 0;
	else 
		m_iUpGradePeople = 1;

	m_Logic.InitBoard(XQ_FLAG_RED,m_iRedBoard);
	m_Logic.InitBoard(XQ_FLAG_BLACK,m_iBlackBoard);

	GameReadyStruct GameReady;
	::memset(&GameReady,0,sizeof(GameReady));
	//GameReady.iBlackStation=m_iBlackStation;
	//GameReady.iFirstStation=m_iWhiteStation;
	for (int i = 0; i < 2;i++) 
		SendGameData(i,&GameReady,sizeof(GameReady),MDM_GM_GAME_NOTIFY,ASS_GAME_READY,0);
	SendWatchData(m_bMaxPeople,&GameReady,sizeof(GameReady),MDM_GM_GAME_NOTIFY,ASS_GAME_READY,0);

	return TRUE;
}

/**
 * ��Ϸ��ʼ
 */
BOOL CServerGameDesk::BeginPlayUpGrade()
{
	//��������
	m_bGameStation=GS_PLAY_GAME;
	m_iBaseOutCount=0;
	m_iOutCardPeople=m_iUpGradePeople;
	m_iGamePoint=0;
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));

	//�����˿�
	//	for (int i=0;i<4;i++) m_Logic.SortCard(m_iUserCard[i],NULL,m_iUserCardCount[i]);

	//������Ϸ��ʼ��Ϣ
	if (m_bFirstRed) 
	{
		m_iFirstOutPeople=XQ_FLAG_NONE;
		m_iCurPlayFlag=XQ_FLAG_RED;
		BeginPlayStruct Begin;
		Begin.iOutDeskStation=m_iOutCardPeople;
		::CopyMemory(Begin.m_iBoard,m_iRedBoard,sizeof(m_iRedBoard));
		SendGameData(0,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
		SendWatchData(0,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);

		Begin.iOutDeskStation = m_iOutCardPeople;
		::CopyMemory(Begin.m_iBoard,m_iBlackBoard,sizeof(m_iBlackBoard));
		SendGameData(1,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
		SendWatchData(1,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
		m_bFirstRed = false;
	}
	else
	{
		m_iFirstOutPeople=XQ_FLAG_NONE;
		m_iCurPlayFlag=XQ_FLAG_BLACK;
		BeginPlayStruct Begin;
		Begin.iOutDeskStation=m_iOutCardPeople;
		::CopyMemory(Begin.m_iBoard,m_iRedBoard,sizeof(m_iRedBoard));
		SendGameData(1,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
		SendWatchData(1,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);

		Begin.iOutDeskStation = m_iOutCardPeople;
		::CopyMemory(Begin.m_iBoard,m_iBlackBoard,sizeof(m_iBlackBoard));
		SendGameData(0,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
		SendWatchData(0,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);

		m_bFirstRed = true;
	}

	//for (int i=0;i<m_bMaxPeople;i++) SendGameData(i,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
	//SendWatchData(m_bMaxPeople,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);

	return TRUE;
}

/**
 * �¼Ӻ��������������û����Ӻ�Ĳ�������������
 */
BOOL CServerGameDesk::UserOutCard(BYTE bDeskStation,PlayChessStruct * pPlayChessInfo)
{
	//�������,д���������Ĳ���
	if (bDeskStation < 2)
	{
		m_iPutChess[bDeskStation]++;
	}

	int iStartI,iStartJ,iEndI,iEndJ;
	iStartI = m_iStartI = pPlayChessInfo->ptStart.x,
	iStartJ = m_iStartJ = pPlayChessInfo->ptStart.y,
	iEndI = m_iEndI = pPlayChessInfo->ptEnd.x,
	iEndJ = m_iEndJ = pPlayChessInfo->ptEnd.y;

	if (bDeskStation!=m_iOutCardPeople) 
		return FALSE;

	PlayChessStruct playRedInfo;
	PlayChessStruct playBlackInfo;
	::memset(&playRedInfo,0,sizeof(playRedInfo));
	::memset(&playBlackInfo,0,sizeof(playBlackInfo));
	playBlackInfo.iCurPlay = pPlayChessInfo->iCurPlay;
	playRedInfo.iCurPlay = pPlayChessInfo->iCurPlay;
	
	//�洢�ϲ����
	m_iLastPlay = pPlayChessInfo->iCurPlay;
	m_iLastDeskStation = (int)bDeskStation;
	::CopyMemory(m_iRedLastQiZi,m_iRedBoard,sizeof(m_iRedBoard));
	::CopyMemory(m_iBlackLastQiZi,m_iBlackBoard,sizeof(m_iBlackBoard));

	int iNextPlayFlag = pPlayChessInfo->iCurPlay;//��ǰ������
	if (iNextPlayFlag == XQ_FLAG_RED)
	{	
		m_iFirstOutPeople=XQ_FLAG_BLACK;
		int xqqz = m_iRedBoard[iStartI][iStartJ];
		m_iBlackBoard[10-iStartI-1][9-iStartJ-1] = XQ_QIZI_NONE;
		m_iBlackBoard[10-iEndI-1][9-iEndJ-1] = xqqz;
		m_iRedBoard[iStartI][iStartJ] = XQ_QIZI_NONE;
		m_iRedBoard[iEndI][iEndJ] = xqqz;
		iNextPlayFlag = XQ_FLAG_BLACK;

		//red
		playRedInfo.iPutChess = m_iPutChess[1];   //�췽����Ĳ���
		playRedInfo.xChess = iEndI;               //���������
		playRedInfo.yChess = iEndJ;
		playRedInfo.idChess = pPlayChessInfo->idChess;   //�µ�ʲô����

		playRedInfo.ptStart.x = iStartI;
		playRedInfo.ptStart.y = iStartJ;
		playRedInfo.ptEnd.x = iEndI;
		playRedInfo.ptEnd.y = iEndJ;
		playRedInfo.iNextPlay = m_iOutCardPeople;
		::CopyMemory(playRedInfo.iBoard,m_iRedBoard,sizeof(playRedInfo.iBoard));


		playBlackInfo.iPutChess = m_iPutChess[0];   //�ڷ�����Ĳ���
		playBlackInfo.xChess = 10-iEndI-1;               //���������
		playBlackInfo.yChess = 9-iEndJ-1;
		playBlackInfo.idChess = 0;                       //�ڷ��µ��岻��������
		//black
		playBlackInfo.ptStart.x = 10-iStartI-1;
		playBlackInfo.ptStart.y = 9-iStartJ-1;
		playBlackInfo.ptEnd.x = 10-iEndI-1;
		playBlackInfo.ptEnd.y = 9-iEndJ-1;
		::CopyMemory(playBlackInfo.iBoard,m_iBlackBoard,sizeof(playBlackInfo.iBoard));
	}
	else if(iNextPlayFlag == XQ_FLAG_BLACK)
	{	
		m_iFirstOutPeople=XQ_FLAG_RED;
		int xqqz = m_iBlackBoard[iStartI][iStartJ];
		m_iRedBoard[10-iStartI-1][9-iStartJ-1] = XQ_QIZI_NONE;
		m_iRedBoard[10-iEndI-1][9-iEndJ-1] = xqqz;
		m_iBlackBoard[iStartI][iStartJ] = XQ_QIZI_NONE;
		m_iBlackBoard[iEndI][iEndJ] = xqqz;
		iNextPlayFlag = XQ_FLAG_RED;

		playRedInfo.iPutChess = m_iPutChess[1];   //�췽����Ĳ���
		playRedInfo.xChess = 10-iEndI-1;               //���������
		playRedInfo.yChess = 9-iEndJ-1;
		playRedInfo.idChess = 0;                  //��ǰ�Ǻڷ����壬����������
		//red
		playRedInfo.ptStart.x = 10-iStartI-1;
		playRedInfo.ptStart.y = 9-iStartJ-1;
		playRedInfo.ptEnd.x = 10-iEndI-1;
		playRedInfo.ptEnd.y = 9-iEndJ-1;
		::CopyMemory(playRedInfo.iBoard,m_iRedBoard,sizeof(playRedInfo.iBoard));

		playBlackInfo.iPutChess = m_iPutChess[0];   //�췽����Ĳ���
		playBlackInfo.xChess = iEndI;               //���������
		playBlackInfo.yChess = iEndJ;
		playBlackInfo.idChess = pPlayChessInfo->idChess; //��ǰ�Ǻڷ�����
		//black
		playBlackInfo.ptStart.x = iStartI;
		playBlackInfo.ptStart.y = iStartJ;
		playBlackInfo.ptEnd.x = iEndI;
		playBlackInfo.ptEnd.y = iEndJ;
		::CopyMemory(playBlackInfo.iBoard,m_iBlackBoard,sizeof(playBlackInfo.iBoard));
	}

	if (!m_bFirstRed)
	{
		SendGameData(0, &playRedInfo, sizeof(playRedInfo), MDM_GM_GAME_NOTIFY, ASS_OUT_CARD_RESULT, 0);
		SendGameData(1, &playBlackInfo, sizeof(playBlackInfo), MDM_GM_GAME_NOTIFY, ASS_OUT_CARD_RESULT, 0);
		if (m_iWatchStation == 0)
			SendWatchData(m_bMaxPeople,&playRedInfo,sizeof(playRedInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);
		if (m_iWatchStation == 1) 
			SendWatchData(m_bMaxPeople,&playBlackInfo,sizeof(playBlackInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);
	}
	else
	{
		SendGameData(1, &playRedInfo, sizeof(playRedInfo), MDM_GM_GAME_NOTIFY, ASS_OUT_CARD_RESULT, 0);
		SendGameData(0, &playBlackInfo, sizeof(playBlackInfo), MDM_GM_GAME_NOTIFY, ASS_OUT_CARD_RESULT, 0);
		if (m_iWatchStation == 1)
			SendWatchData(m_bMaxPeople,&playRedInfo,sizeof(playRedInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);
		if (m_iWatchStation == 0) 
			SendWatchData(m_bMaxPeople,&playBlackInfo,sizeof(playBlackInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);
	}	
	CanFinish(bDeskStation,false);
	return TRUE;
}

/*
//�û�����
BOOL CServerGameDesk::UserOutCard(BYTE bDeskStation,int iStartI,int iStartJ,int iEndI,int iEndJ, int iCurPlay, int iBoard[][9])
{
	if (bDeskStation!=m_iOutCardPeople) 
		return FALSE;

	PlayChessStruct playRedInfo;
	PlayChessStruct playBlackInfo;
	::memset(&playRedInfo,0,sizeof(playRedInfo));
	::memset(&playBlackInfo,0,sizeof(playBlackInfo));
	playBlackInfo.iCurPlay = bDeskStation;
	playRedInfo.iCurPlay = bDeskStation;
	
//	TCHAR sz[200];
//	wsprintf(sz,"bDeskStation=%d,iCurPlay=%d",bDeskStation,iCurPlay);
//	WriteStr(sz);
	//�洢�ϲ����
	m_iLastPlay = iCurPlay;
	m_iLastDeskStation = (int)bDeskStation;
	::CopyMemory(m_iRedLastQiZi,m_iRedBoard,sizeof(m_iRedBoard));
	::CopyMemory(m_iBlackLastQiZi,m_iBlackBoard,sizeof(m_iBlackBoard));



	int iNextPlayFlag = iCurPlay;//��ǰ������
	if (iNextPlayFlag == XQ_FLAG_RED)
	{	
		m_iFirstOutPeople=XQ_FLAG_BLACK;
		int xqqz = m_iRedBoard[iStartI][iStartJ];
		m_iBlackBoard[10-iStartI-1][9-iStartJ-1] = XQ_QIZI_NONE;
		m_iBlackBoard[10-iEndI-1][9-iEndJ-1] = xqqz;
		m_iRedBoard[iStartI][iStartJ] = XQ_QIZI_NONE;
		m_iRedBoard[iEndI][iEndJ] = xqqz;
		iNextPlayFlag = XQ_FLAG_BLACK;

		//red

		playRedInfo.ptStart.x = iStartI;
		playRedInfo.ptStart.y = iStartJ;
		playRedInfo.ptEnd.x = iEndI;
		playRedInfo.ptEnd.y = iEndJ;
		playRedInfo.iNextPlay = m_iOutCardPeople;
		//playRedInfo.iCurPlay = iNextPlayFlag;
		::CopyMemory(playRedInfo.iBoard,m_iRedBoard,sizeof(playRedInfo.iBoard));


		//black
		playBlackInfo.ptStart.x = 10-iStartI-1;
		playBlackInfo.ptStart.y = 9-iStartJ-1;
		playBlackInfo.ptEnd.x = 10-iEndI-1;
		playBlackInfo.ptEnd.y = 9-iEndJ-1;
		//playBlackInfo.iNextPlay = m_iOutCardPeople;
		//playBlackInfo.iCurPlay = iNextPlayFlag;
		::CopyMemory(playBlackInfo.iBoard,m_iBlackBoard,sizeof(playBlackInfo.iBoard));

		////check win or lose
		//		BOOL bb = FALSE;
		//		if (m_Logic.CheckWinOrLose(XQ_FLAG_RED,m_iRedBoard,bb))
		//		{
		//			if (bb) m_iWinFlag = XQ_FLAG_RED;
		//			else m_iWinFlag = XQ_FLAG_BLACK;
		//			SetTimer(TIME_GAME_FINISH,2000);
		//			return TRUE;
		//
		//		}
	}
	else if(iNextPlayFlag == XQ_FLAG_BLACK)
	{	
		m_iFirstOutPeople=XQ_FLAG_RED;
		int xqqz = m_iBlackBoard[iStartI][iStartJ];
		m_iRedBoard[10-iStartI-1][9-iStartJ-1] = XQ_QIZI_NONE;
		m_iRedBoard[10-iEndI-1][9-iEndJ-1] = xqqz;
		m_iBlackBoard[iStartI][iStartJ] = XQ_QIZI_NONE;
		m_iBlackBoard[iEndI][iEndJ] = xqqz;
		iNextPlayFlag = XQ_FLAG_RED;

		//red
		playRedInfo.ptStart.x = 10-iStartI-1;
		playRedInfo.ptStart.y = 9-iStartJ-1;
		playRedInfo.ptEnd.x = 10-iEndI-1;
		playRedInfo.ptEnd.y = 9-iEndJ-1;
		//playRedInfo.iNextPlay = m_iOutCardPeople;
		//playRedInfo.iCurPlay = iNextPlayFlag;
		::CopyMemory(playRedInfo.iBoard,m_iRedBoard,sizeof(playRedInfo.iBoard));

		//black
		playBlackInfo.ptStart.x = iStartI;
		playBlackInfo.ptStart.y = iStartJ;
		playBlackInfo.ptEnd.x = iEndI;
		playBlackInfo.ptEnd.y = iEndJ;
		//playBlackInfo.iNextPlay = m_iOutCardPeople;
		//playBlackInfo.iCurPlay = iNextPlayFlag;
		::CopyMemory(playBlackInfo.iBoard,m_iBlackBoard,sizeof(playBlackInfo.iBoard));


		////check win or lose
		//	BOOL bb = FALSE;
		//	if (m_Logic.CheckWinOrLose(XQ_FLAG_BLACK,m_iRedBoard,bb))
		//	{
		//		if (bb) m_iWinFlag = XQ_FLAG_BLACK;
		//		else m_iWinFlag = XQ_FLAG_RED;
		//
		//			SetTimer(TIME_GAME_FINISH,2000);
		//			return TRUE;
		//
		//		}

	}
	//�������ӽ��
	//	for(int i = 0; i < 2; i++)
	//		SendGameData(i, &playChessInfo, sizeof(playChessInfo), MDM_GM_GAME_NOTIFY, ASS_OUT_CARD_RESULT, 0);
	//	SendWatchData(m_bMaxPeople,&playChessInfo,sizeof(playChessInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);


	if (!m_bFirstRed)
	{
		SendGameData(0, &playRedInfo, sizeof(playRedInfo), MDM_GM_GAME_NOTIFY, ASS_OUT_CARD_RESULT, 0);
		//SendGameData(0,&playRedInfo,sizeof(playRedInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
		//SendWatchData(1,&playRedInfo,sizeof(playRedInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
		SendGameData(1, &playBlackInfo, sizeof(playBlackInfo), MDM_GM_GAME_NOTIFY, ASS_OUT_CARD_RESULT, 0);
		//SendGameData(1,&playBlackInfo,sizeof(playBlackInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
		//SendWatchData(0,&playBlackInfo,sizeof(playBlackInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);

		if (m_iWatchStation == 0)
			SendWatchData(m_bMaxPeople,&playRedInfo,sizeof(playRedInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);
		if (m_iWatchStation == 1) 
			SendWatchData(m_bMaxPeople,&playBlackInfo,sizeof(playBlackInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);
	}
	else
	{
		SendGameData(1, &playRedInfo, sizeof(playRedInfo), MDM_GM_GAME_NOTIFY, ASS_OUT_CARD_RESULT, 0);
		//SendGameData(0,&playRedInfo,sizeof(playRedInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
		//SendWatchData(1,&playRedInfo,sizeof(playRedInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
		SendGameData(0, &playBlackInfo, sizeof(playBlackInfo), MDM_GM_GAME_NOTIFY, ASS_OUT_CARD_RESULT, 0);
		//SendGameData(1,&playBlackInfo,sizeof(playBlackInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
		//SendWatchData(0,&playBlackInfo,sizeof(playBlackInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);

		if (m_iWatchStation == 1)
			SendWatchData(m_bMaxPeople,&playRedInfo,sizeof(playRedInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);
		if (m_iWatchStation == 0) 
			SendWatchData(m_bMaxPeople,&playBlackInfo,sizeof(playBlackInfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);
	}

	/*	///��һ������
	m_iOutCardPeople = (bDeskStation + 1) % 2;
	NextPlayInfo nextplayinfo;
	nextplayinfo.iNextPlay = m_iOutCardPeople;
	if (!m_bFirstRed)
	{
	SendGameData(0,&nextplayinfo,sizeof(nextplayinfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
	if (m_iWatchStation == 0)
	SendWatchData(m_bMaxPeople,&nextplayinfo,sizeof(nextplayinfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
	if (m_iWatchStation == 1) 
	SendWatchData(m_bMaxPeople,&nextplayinfo,sizeof(nextplayinfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
	}
	else
	{
	SendGameData(1,&nextplayinfo,sizeof(nextplayinfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
	if (m_iWatchStation == 1)
	SendWatchData(m_bMaxPeople,&nextplayinfo,sizeof(nextplayinfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
	if (m_iWatchStation == 0) 
	SendWatchData(m_bMaxPeople,&nextplayinfo,sizeof(nextplayinfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
	}

	m_iCurPlayFlag = iNextPlayFlag;*/
/*	
	CanFinish(bDeskStation,false);
	return TRUE;
}
*/

/**
 * �ܷ����(��������ȫ���ɿͻ���ʵ��)
 */
BOOL CServerGameDesk::CanFinish(BYTE PutStoneDeskStation,BOOL bCanFinish/*,int iWiner*/)
{
	if(PutStoneDeskStation!=m_iOutCardPeople)
		return true;
	m_iLastDeskStation = PutStoneDeskStation;
	if(bCanFinish)
	switch(PutStoneDeskStation)//ݔ��λ��
	{
	case 0:m_iWinFlag = m_bFirstRed?1:-1;break;
	case 1:m_iWinFlag = m_bFirstRed?-1:1;break;
	default:
		break;
	}
	
//	TCHAR sz[200];
//	wsprintf(sz,"%d,bCanFinish=%d,%s",PutStoneDeskStation,bCanFinish,m_pUserInfo[PutStoneDeskStation]->m_UserData.nickName);
//	WriteStr(sz);
	if(bCanFinish)
	{
		//if(iWiner == -1)
		SetTimer(TIME_GAME_FINISH, 2000);
		return true;
	}
	///��һ������
	m_iOutCardPeople = (PutStoneDeskStation + 1) % 2;
	NextPlayInfo nextplayinfo;
	nextplayinfo.iNextPlay = m_iOutCardPeople;
	switch(m_iOutCardPeople)
	{	
	case 0:
		nextplayinfo.iCurPlay = m_bFirstRed?-1:1;break;
	case 1:
		nextplayinfo.iCurPlay = m_bFirstRed?1:-1;break;
	}

	for(int i = 0;i < PLAY_COUNT;i ++)
	{
		SendGameData(i,&nextplayinfo,sizeof(nextplayinfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);

	}
	SendWatchData(m_bMaxPeople,&nextplayinfo,sizeof(nextplayinfo),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);

	m_iCurPlayFlag = nextplayinfo.iCurPlay;
	//m_iCurPlayFlag = m_iOutCardPeople;
	return true;
}

/**
 * ��Ϸ����
 */
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	// ƽ̨�����벽ʱ���ܻ��ͻ���½�������, ��������ж�
	if (m_bGameStation != GS_PLAY_GAME)
		return true;

	//�õ���С���Ľ��
	long m_dwMaxPoint=0;
	bool bSetMaxMoney=false;
	/*for (BYTE i=0;i<m_bMaxPeople;i++)
	{
	if (m_pUserInfo[i]!=NULL)
	{
	if (bSetMaxMoney==false)
	{
	bSetMaxMoney=true;
	m_dwMaxPoint=(long int)m_pUserInfo[i]->m_UserData.dwPoint-1;
	}
	else 
	m_dwMaxPoint=__min(m_dwMaxPoint,(long int)m_pUserInfo[i]->m_UserData.dwPoint-1);
	}
	}
	if(m_pDataManage->m_InitData.uComType!=TY_NORMAL_GAME && m_dwMaxPoint<0)
	{
	TRACE("���ִ��������һ������Ϸ�У���ҵĽ����Ϊ������");
	return false;
	}*/

	//��д����
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			m_bGameStation=GS_WAIT_NEXT;
			for (int i=0;i<m_bMaxPeople;i++)
			{
				if (m_pUserInfo[i]!=NULL) 
					m_pUserInfo[i]->m_UserData.bUserState=USER_SITTING;
			}

			int iBasePoint=m_pDataManage->m_InitData.uBasePoint;//����
			//����÷�
			//��Ϸ����
			GameEndStruct GameEnd;
			::memset(&GameEnd,0,sizeof(GameEnd));
			GameEnd.iUpGradeStation=m_iUpGradePeople;
			GameEnd.iGamePoint=m_iGamePoint;

			///����÷�
			GetUpGrade(0,m_iPlayCardCount);

			//CString s;

			//int iTruePoint =GetTruePoint();		//����÷�,˭ʤ˭��
			////������һ�� GetTruePoint(i);//
			for (int i = 0;i < 2;i++)
			{
				GameEnd.iTurePoint[i] =m_iTurePoint[i];
				//if(i == m_iUpGradePeople)
				//	GameEnd.iTurePoint[i] = iTruePoint;
				//else 
				//	GameEnd.iTurePoint [i] = -iTruePoint;
				///s.AppendFormat("%d: %d |",i,m_iUserTruePoint[i]);
			}		
			/*if (m_pDataManage->m_InitData.uComType!=TY_NORMAL_GAME)
			{
			GameEnd.iTurePoint[0]*=iBasePoint;
			GameEnd.iTurePoint[1]*=iBasePoint;

			for (BYTE i=0;i<m_bMaxPeople;i++)
			{
			if(GameEnd.iTurePoint[i]>0)
			GameEnd.iTurePoint[i]=__min(m_dwMaxPoint,GameEnd.iTurePoint[i]);
			if(GameEnd.iTurePoint[i]<0)
			GameEnd.iTurePoint[i]=__max(-m_dwMaxPoint,GameEnd.iTurePoint[i]);
			if (GameEnd.iTurePoint[i]>=100)
			{
			if(m_pDataManage->m_InitData.uComType!=SUP_MATCH_GAME)
			GameEnd.iTurePoint[i]=GameEnd.iTurePoint[i]*49L/50L;
			}
			}
			}*/


			//д�����ݿ�				
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			/*for (int i=0; i<PLAY_COUNT; ++i) 
			{ 
				temp_cut[i] = (bDeskStation == i)?true:false; 
			}*/
			ChangeUserPointint64(GameEnd.iTurePoint, temp_cut);
			__super::RecoderGameInfo(GameEnd.iChangeMoney);

			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0; /// ��ҳ����ۻ���    add by wys 2010-11-1
			if (true == bNotCostPoint)
			{
				memset(GameEnd.iTurePoint,0,sizeof(GameEnd.iTurePoint));
			}
			//��������
			for (int i=0;i<m_bMaxPeople;i++) SendGameData(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
			SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);


			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,GF_SALE);

			return true;
		}
	case GF_SALE:			//��Ϸ��ȫ����
	case GFF_FORCE_FINISH:		//�û������뿪
		{

			m_firstPlay =0;
			int iBasePoint=m_pDataManage->m_InitData.uBasePoint;//����
			//����÷�
			int iTurePoint=GetUpGrade(m_iGamePoint,m_iPlayCardCount);
			//��������
			m_bGameStation=GS_WAIT_ARGEE;

			for (int i=0;i<m_bMaxPeople;i++)
			{
				if (m_pUserInfo[i]!=NULL) 
					m_pUserInfo[i]->m_UserData.bUserState=USER_SITTING;
			}
			GameCutStruct CutEnd;
			memset(& CutEnd,0,sizeof(CutEnd));
			CutEnd.bDeskStation=bDeskStation;
			if(bCloseFlag==GF_SALE)
			{
				CutEnd.iTurePoint[bDeskStation]=0;
				CutEnd.iTurePoint[(bDeskStation+1)%4]=0;
				CutEnd.iTurePoint[(bDeskStation+2)%4]=0;
				CutEnd.iTurePoint[(bDeskStation+3)%4]=0;
			}
			else
			{
				//���÷���
				if (iTurePoint>0) m_iUpGradePeople=(m_iUpGradePeople+2)%4;
				else m_iUpGradePeople=(m_iUpGradePeople+1)%4;

				if(iTurePoint>-1)
				{
					CutEnd.iTurePoint[bDeskStation]=-GetRunPublish();
					CutEnd.iTurePoint[(bDeskStation+1)%PLAY_COUNT]=GetRunPublish();
				}
				else
				{
					if(bDeskStation==m_iUpGradePeople || bDeskStation==(m_iUpGradePeople+2)%4)//������������������߲��÷֣����Լҵ÷�
					{
						CutEnd.iTurePoint[m_iUpGradePeople%4]=CutEnd.iTurePoint[(m_iUpGradePeople+2)%4]=abs(iTurePoint*3);
						CutEnd.iTurePoint[bDeskStation]=0;
						CutEnd.iTurePoint[(m_iUpGradePeople+1)%4]=CutEnd.iTurePoint[(m_iUpGradePeople+3)%4]=-abs(iTurePoint*3);
					}
					else
					{
						CutEnd.iTurePoint[bDeskStation]=-GetRunPublish();
						CutEnd.iTurePoint[(bDeskStation+1)%PLAY_COUNT]+= GetRunPublish();
					
					}
				}
			}			
			//������Ϣ
			if (bCloseFlag==GF_SALE)
			{
				for (int i=0;i<m_bMaxPeople;i++) SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);
				SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);
			}
			else
			{
				//��������ͱ������ʵ�÷�
				/*if (m_pDataManage->m_InitData.uComType!=TY_NORMAL_GAME)
				{
				CutEnd.iTurePoint[0]*=iBasePoint;
				CutEnd.iTurePoint[1]*=iBasePoint;
				CutEnd.iTurePoint[2]*=iBasePoint;
				CutEnd.iTurePoint[3]*=iBasePoint;
				for (BYTE i=0;i<m_bMaxPeople;i++)
				{
				if(CutEnd.iTurePoint[i]>0)
				CutEnd.iTurePoint[i]=__min(m_dwMaxPoint,CutEnd.iTurePoint[i]);
				if(CutEnd.iTurePoint[i]<0)
				CutEnd.iTurePoint[i]=__max(-m_dwMaxPoint,CutEnd.iTurePoint[i]);
				if (CutEnd.iTurePoint[i]>=100)
				{
				if(m_pDataManage->m_InitData.uComType!=SUP_MATCH_GAME)
				CutEnd.iTurePoint[i]=CutEnd.iTurePoint[i]*49L/50L;
				}
				}
				}*/

				bool temp_cut[PLAY_COUNT];
				memset(temp_cut,0,sizeof(temp_cut));
				/*for (int i=0;i<PLAY_COUNT;i++)
				{
					temp_cut[i]=(bDeskStation==i && CutEnd.iTurePoint[i]<0);
				}*/
				for (int i=0; i<PLAY_COUNT; ++i) 
				{ 
					temp_cut[i] = (bDeskStation == i)?true:false; 
				}
				ChangeUserPointint64(CutEnd.iTurePoint, temp_cut);
				__super::RecoderGameInfo(CutEnd.iChangeMoney);

				bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0; /// ��ҳ����ۻ���    add by wys 2010-11-1
				if (true == bNotCostPoint)
				{
					memset(CutEnd.iTurePoint,0,sizeof(CutEnd.iTurePoint));
				}

				for (int i=0;i<m_bMaxPeople;i++) SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
				SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
			}

			bCloseFlag=GFF_FORCE_FINISH;

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

/**
 * �ж��Ƿ�������Ϸ
 */
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if(m_bGameStation>=GS_SEND_CARD&&m_bGameStation<GS_WAIT_NEXT)
		return true;
	else
		return false;
}

/**
 * �û��뿪��Ϸ��
 */
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	if(bDeskStation>254||bDeskStation<0)
		return true;
	m_firstPlay =0;
	if(m_pUserInfo[(bDeskStation+1)%2]==NULL)
	{
		m_bGameStation=GS_WAIT_SETGAME;
	}
	if(m_bGameStation>GS_WAIT_ARGEE)
		GameFinish(bDeskStation,GF_SALE);
	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}

/**
 * ��Ϸ���ݰ�������
 */
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_SYNC_TIME:
		{
			SyncTimeStruct *pMes = (SyncTimeStruct *)pData;
			m_iBotJuUseTime[bDeskStation] = pMes->iBotJuUseTime;
			m_iBotBuUseTime[bDeskStation] = pMes->iBotBuUseTime;
			m_iBotDuUseTime[bDeskStation] = pMes->iBotDuUseTime;
		}
		return true;
	case ASS_GM_AGREE_GAME:		//�û�ͬ����Ϸ
		{
			if (bWatchUser) return FALSE;

			__super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
			m_firstPlay ++;
			if (m_bGameStation==GS_WAIT_SETGAME)	//��������
			{
				if (uSize!=sizeof(UserArgeeGame)) return FALSE;

				if(m_firstPlay ==1)
				{
					m_daoshi = bDeskStation;
				}
				GameEndStruct GameEnd;
				::memset(&GameEnd,0,sizeof(GameEnd));

				UserArgeeGame *Agree=(UserArgeeGame *)pData;
				//д�����ݿ�				
				bool temp_cut[PLAY_COUNT];
				temp_cut[0] = 0;//bDeskStation%2;
				temp_cut[1] = 0;//(bDeskStation+1)%2;

				m_daoshi_money= Agree->iGameMoney;
				GameEnd.iTurePoint[bDeskStation%2] = Agree->iGameMoney;
				GameEnd.iTurePoint[(bDeskStation+1)%2] = -Agree->iGameMoney;


				if(m_firstPlay == 2 /*&& m_daoshi_money > 0*/)
				{
					if(m_pUserInfo[bDeskStation]->m_UserData.i64Money <= Agree->iGameMoney)
					{
						GameFinish(bDeskStation,GF_SALE);
						return true;
					}
					if(bDeskStation == m_daoshi)
					{
						GameEnd.iTurePoint[(bDeskStation+1)%2] = -m_daoshi_money;
						GameEnd.iTurePoint[bDeskStation%2] = m_daoshi_money;
					}
					ChangeUserPointint64(GameEnd.iTurePoint,temp_cut);
				}
				if (!UserSetGame(Agree,bDeskStation)) return FALSE;
				
				return TRUE;
			}

			GameEndStruct GameEnd;
			::memset(&GameEnd,0,sizeof(GameEnd));

			//д�����ݿ�				
			bool temp_cut[PLAY_COUNT];
			temp_cut[0] = 0/*bDeskStation%2*/;
			temp_cut[1] = 0/*(bDeskStation+1)%2*/;

			GameEnd.iTurePoint[bDeskStation%2] = m_daoshi_money;
			GameEnd.iTurePoint[(bDeskStation+1)%2] = -m_daoshi_money;
		

			if(m_firstPlay == 2)
			{
				if(m_pUserInfo[bDeskStation]->m_UserData.i64Money <= m_daoshi_money)
				{
					GameFinish(bDeskStation,GF_SALE);
					return true;
				}
				if(m_daoshi != bDeskStation && bDeskStation == 0)
				{
					GameEnd.iTurePoint[bDeskStation%2] = -m_daoshi_money;
					GameEnd.iTurePoint[(bDeskStation+1)%2] = m_daoshi_money;
				}
				if(m_daoshi == bDeskStation && bDeskStation == 1)
				{
					GameEnd.iTurePoint[bDeskStation%2] = -m_daoshi_money;
					GameEnd.iTurePoint[(bDeskStation+1)%2] = m_daoshi_money;
				}
				ChangeUserPointint64(GameEnd.iTurePoint,temp_cut);
			}
			
			//�������
			if ((m_bGameStation!=GS_WAIT_ARGEE)&&(m_bGameStation!=GS_WAIT_NEXT)) 
				return TRUE;
			return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}

	case ASS_OUT_CARD:		//�û�����
		{
			if(bWatchUser)
				return false;
			if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
				return true;
			if (uSize != sizeof(PlayChessStruct)) 
				return false;

			PlayChessStruct *pPlayChessInfo = (PlayChessStruct *)pData;

			return UserOutCard(bDeskStation,pPlayChessInfo);
		}
	case ASS_CAN_FINISH://�û����Ӻ�,�ɿͶ�ͳ���Ƿ���Խ���
		{
			if(bWatchUser)
				return false;
			if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
				return true;
			if (uSize != sizeof(PlayCheckFinish)) return false;
			PlayCheckFinish* pPlayCheck = (PlayCheckFinish*)pData;
			return CanFinish(bDeskStation,pPlayCheck->bCanFinish);			
		}

	case ASS_HAVE_THING:	//���������뿪
		{
			if(bWatchUser)
				return false;
			HaveThingStruct * pThing=(HaveThingStruct *)pData;
			return UserHaveThing(bDeskStation,pThing->szMessage);
		}
	case ASS_LEFT_RESULT: //ͬ���û��뿪
		{
			if(bWatchUser)
				return false;
			LeaveResultStruct * pLeave=(LeaveResultStruct *)pData;
			return ArgeeUserLeft(bDeskStation,pLeave->bArgeeLeave);
		}
	case ASS_STOP_THING:
		{
			if(bWatchUser)
				return false;
			if (m_iBgePeaceCount[bDeskStation] <= 0)
				return false;
			m_iBgePeaceCount[bDeskStation]--;
			SendGameData((bDeskStation+1)%2,MDM_GM_GAME_NOTIFY,ASS_STOP_THING,0);
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
				//memset(&Mess,0,sizeof(Mess));
				//strcpy(Mess.Message,"�мҾ�����������ǰ�����˱��֣�");
				//SendGameData((bDeskStation+1)%4,&Mess,sizeof(Mess),MDM_GM_GAME_NOTIFY,ASS_MESSAGE,0);
				//SendGameData((bDeskStation+3)%4,&Mess,sizeof(Mess),MDM_GM_GAME_NOTIFY,ASS_MESSAGE,0);

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


	case ASS_SUPER_TIMER://��ʱ����
		{
			if (bWatchUser) return false;

			if (m_bGameStation == GS_PLAY_GAME)
			{	
				SuperTimer  *supertimer = (SuperTimer*)pData;
				int flag = (int)supertimer->bDeskStation;
				if (flag == XQ_FLAG_RED)
				{
					m_iWinFlag = XQ_FLAG_BLACK;
				}
				else if(flag == XQ_FLAG_BLACK)
				{
					m_iWinFlag = XQ_FLAG_RED;
				}
				GameFinish(0,GF_NORMAL);
			}
			return true;
		}

	case ASS_REN_SHU://����
		{
			if (uSize != sizeof(LoseStruct)) 
				return false;
				if (bWatchUser) 
					return false;
			SendGameData((bDeskStation+1)%2,MDM_GM_GAME_NOTIFY,ASS_REN_SHU,(int)bDeskStation);
			LoseStruct  * lose= (LoseStruct*)pData;
			if (m_bGameStation==GS_PLAY_GAME)
			{
				int flag = lose->iFlag;

				if (flag == XQ_FLAG_RED)
				{
					m_iWinFlag = XQ_FLAG_BLACK;
				}
				else if(flag == XQ_FLAG_BLACK)
				{
					m_iWinFlag = XQ_FLAG_RED;
				}
				GameFinish(0,GF_NORMAL);
				m_bGameStation= GS_WAIT_ARGEE;
			}
			return true;

		}


	case ASS_HUI_QI://����
		{
			m_bStationRegret = 1;
			SendGameData((bDeskStation+1)%2,MDM_GM_GAME_NOTIFY,ASS_HUI_QI,0);
			return true;
		}

	case ASS_AGREE_HUI_QI://����
		{
			m_bStationRegret =-1;
			int iflag = (int)pNetHead->bHandleCode;
			if (iflag == 0)	//��ͬ�����
			{

				SendGameData((bDeskStation+1)%2,MDM_GM_GAME_NOTIFY,ASS_AGREE_HUI_QI,0);
				return true;
			}

			//ͬ����һ���
			if (bDeskStation < 2)
			{
				m_iPutChess[(bDeskStation+1)%2]--;
			}

			RegretChessMsg regretRedInfo,regretBlackInfo;

		    m_iOutCardPeople = m_iLastDeskStation;
			regretRedInfo.iLastPlay = m_iLastPlay;
			regretRedInfo.iLastDeskStation = m_iLastDeskStation;
			::CopyMemory(regretRedInfo.iLastChessArray,m_iRedLastQiZi,sizeof(m_iRedLastQiZi));

			regretBlackInfo.iLastPlay = m_iLastPlay;
			regretBlackInfo.iLastDeskStation = m_iLastDeskStation;
			::CopyMemory(regretBlackInfo.iLastChessArray,m_iBlackLastQiZi,sizeof(m_iBlackLastQiZi));

			if (!m_bFirstRed)
			{	
				SendGameData(0,&regretRedInfo,sizeof(regretRedInfo),MDM_GM_GAME_NOTIFY,ASS_AGREE_HUI_QI,1);
				SendWatchData(0,&regretRedInfo,sizeof(regretRedInfo),MDM_GM_GAME_NOTIFY,ASS_AGREE_HUI_QI,1);
				SendGameData(1,&regretBlackInfo,sizeof(regretBlackInfo),MDM_GM_GAME_NOTIFY,ASS_AGREE_HUI_QI,1);
				SendWatchData(1,&regretBlackInfo,sizeof(regretBlackInfo),MDM_GM_GAME_NOTIFY,ASS_AGREE_HUI_QI,1);
			}
			else
			{
				SendGameData(1,&regretRedInfo,sizeof(regretRedInfo),MDM_GM_GAME_NOTIFY,ASS_AGREE_HUI_QI,1);
				SendWatchData(1,&regretRedInfo,sizeof(regretRedInfo),MDM_GM_GAME_NOTIFY,ASS_AGREE_HUI_QI,1);
				SendGameData(0,&regretBlackInfo,sizeof(regretBlackInfo),MDM_GM_GAME_NOTIFY,ASS_AGREE_HUI_QI,1);
				SendWatchData(0,&regretBlackInfo,sizeof(regretBlackInfo),MDM_GM_GAME_NOTIFY,ASS_AGREE_HUI_QI,1);
			}

			::CopyMemory(m_iRedBoard,m_iRedLastQiZi,sizeof(m_iRedLastQiZi));
			::CopyMemory(m_iBlackBoard,m_iBlackLastQiZi,sizeof(m_iBlackLastQiZi));
			return true;
		}
	// �����ˣ����ͬ���Ľ��������ظ�5�飬�Զ��и������
	case ASS_GAMER_SHUALAI_JIANG:
		{
			m_bGameStation=GS_WAIT_NEXT;
			for (int i=0;i<m_bMaxPeople;i++)
			{
				if (m_pUserInfo[i]!=NULL) 
					m_pUserInfo[i]->m_UserData.bUserState=USER_SITTING;
			}
			int iBasePoint=m_pDataManage->m_InitData.uBasePoint;//����
			//��Ϸ����
			GameEndStruct GameEnd;
			::memset(&GameEnd,0,sizeof(GameEnd));
			GameEnd.iUpGradeStation=m_iUpGradePeople;
			GameEnd.iGamePoint=m_iGamePoint;
			GameEnd.bShuaLai = true;
			GameEnd.bDeskStation = (bDeskStation + 1) % 2;

		  switch(GameEnd.bDeskStation)//����λ��
		  {
				case 0:
				{
					m_iWinFlag = m_bFirstRed? XQ_FLAG_RED :XQ_FLAG_BLACK;
					break;
				}
				case 1:
				{
					m_iWinFlag = m_bFirstRed? XQ_FLAG_BLACK:1;
					break;
				}
				default:
					break;
		   }	
			///����÷�
			GetUpGrade(0,m_iPlayCardCount);

			//����÷�,˭ʤ˭��
			for (int i = 0;i < 2;i++)
			{
				GameEnd.iTurePoint[i] =m_iTurePoint[i];
			}
			//д�����ݿ�				
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			ChangeUserPointint64(GameEnd.iTurePoint, temp_cut);
			__super::RecoderGameInfo(GameEnd.iChangeMoney);
			//��������
			for (int i=0;i<m_bMaxPeople;i++) 
			{
				SendGameData(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_GAMER_SHUALAI_JIANG,0);
			}
			SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_GAMER_SHUALAI_JIANG,0);
			ReSetGameState(GF_NORMAL);
			__super::GameFinish(bDeskStation,GF_SALE);
			return true;
		}

	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

/**
 * ����÷� һ��Ҳʱ������ Χ�� ��д����
 */
int	CServerGameDesk::GetUpGrade(int iGamePoint, int iPlayCardCount)
{
	if (m_bQiuHe)
	{
		m_iTurePoint[0] = 0;
		m_iTurePoint[1] = 0;
		return 0;
	}
	if (m_iWinFlag == XQ_FLAG_RED)
	{
		if (!m_bFirstRed)
		{
			m_iTurePoint[0] = 1;
			m_iTurePoint[1] = -1;
		}
		else
		{
			m_iTurePoint[1] = 1;
			m_iTurePoint[0] = -1;
		}
	}
	if (m_iWinFlag == XQ_FLAG_BLACK)
	{	
		if (!m_bFirstRed)
		{
			m_iTurePoint[0] = -1;
			m_iTurePoint[1] = 1;
		}
		else
		{
			m_iTurePoint[1] = -1;
			m_iTurePoint[0] = 1;
		}
	}
	return 0;
}

/**
 * �û�������Ϸ
 */
BOOL CServerGameDesk::UserSetGame(UserArgeeGame * pGameSet,byte desk)
{
	//Ч������
	if (m_bGameStation!=GS_WAIT_SETGAME) return FALSE;

	//Ч������
	///if (((pGameSet->iPlayCount<=0)||(pGameSet->iPlayCount>99))||
	///	(pGameSet->iBeginStation<2)||(pGameSet->iEndStation>14)||
	///	(pGameSet->iBeginStation>pGameSet->iEndStation)) 
	///	return FALSE;
	///if ((m_CardCount & pGameSet->iCardCount)==0) return FALSE;

	//��������
	//m_iLessCount=pGameSet->iPlayCount;
	//m_iPlayCardCount=2;//(pGameSet->iCardCount&1)?2:3;
	//m_iBeginStation=pGameSet->iBeginStation;
	//m_iEndStation=pGameSet->iEndStation;

	m_bFirstRed = true;
	m_iBeenPlayGame=0;
	m_bGameStation=GS_WAIT_ARGEE;
	//	m_Logic.SetNTCardNum(pGameSet->iBeginStation);
	//	m_Logic.SetNTHuaKind(UG_ERROR_HUA);
	//m_iPlayNTNum[0]=pGameSet->iBeginStation;
	//m_iPlayNTNum[1]=pGameSet->iBeginStation;

	m_iJuTimeMinute = pGameSet->iJuTimeMinute;
	m_iJuTimeSecond = pGameSet->iJuTimeSecond;
	m_iBuTimeMinute = pGameSet->iBuTimeMinute;
	m_iBuTimeSecond = pGameSet->iBuTimeSecond;
	m_iDuMiaoMinute = pGameSet->iDuMiaoMinute;
	m_iDuMiaoSecond = pGameSet->iDuMiaoSecond;
	m_iGameMoney = pGameSet->iGameMoney;

	//������Ϣ
	UserArgeeGame Agree;
	Agree.iJuTimeMinute=pGameSet->iJuTimeMinute;
	Agree.iJuTimeSecond=pGameSet->iJuTimeSecond;
	Agree.iBuTimeMinute=pGameSet->iBuTimeMinute;
	Agree.iBuTimeSecond=pGameSet->iBuTimeSecond;
	Agree.iDuMiaoMinute = pGameSet->iDuMiaoMinute;
	Agree.iDuMiaoSecond = pGameSet->iDuMiaoSecond;
	Agree.iGameMoney = pGameSet->iGameMoney;
	for (int i=0;i<2;i++)
		if(i!=desk)
			SendGameData(i,&Agree,sizeof(Agree),MDM_GM_GAME_NOTIFY,ASS_UG_USER_SET,0);
	SendWatchData(m_bMaxPeople,&Agree,sizeof(Agree),MDM_GM_GAME_NOTIFY,ASS_UG_USER_SET,0);

	return TRUE;
}

/**
 * �����˿˸��û�
 */
BOOL CServerGameDesk::SendCard()
{
	return TRUE;
}

/**
 * �û�����
 */
BOOL CServerGameDesk::UserNtCard(BYTE bDeskStation, int iHuaKind, int iCardCount)
{
	//if ((iHuaKind==UG_HEI_TAO)||(iHuaKind==UG_HONG_TAO)||(iHuaKind==UG_MEI_HUA)||(iHuaKind==UG_FANG_KUAI))
	//{
	//	//Ч������
	//	if ((iCardCount<1)||(iCardCount>3)) return FALSE;

	//	//��������
	//	if (iCardCount<=m_iNtCardCount) return TRUE;
	//	if (bDeskStation==m_iNtPeople)
	//	{
	//		if ((iHuaKind!=m_Logic.GetNTHuaKind())&&(iCardCount<=m_iNtCardCount)) 
	//			return TRUE;
	//	}
	//	else
	//	{
	//		if (m_iNtCardCount==0)
	//		{
	//			iCardCount=1;
	//		}
	//		else if ((iCardCount>m_iNtCardCount)||(iHuaKind>m_Logic.GetNTHuaKind()))
	//		{
	//			iCardCount=m_iNtCardCount+(iHuaKind>m_Logic.GetNTHuaKind())?0:1;
	//			iCardCount=__min(iCardCount,3);
	//		}
	//		else return TRUE;
	//	}

	//	//��������
	//	m_iNtPeople=bDeskStation;
	//	m_iNtCardCount=iCardCount;
	//	m_Logic.SetNTHuaKind(iHuaKind);

	//	//������Ϣ
	//	ServerNtStruct ServerNt;
	//	ServerNt.bDeskStation=bDeskStation;
	//	ServerNt.iHuaKind=iHuaKind;
	//	for (int i=0;i<m_bMaxPeople;i++) SendGameData(i,&ServerNt,sizeof(ServerNt),MDM_GM_GAME_NOTIFY,ASS_NT_CARD,0);
	//	SendWatchData(m_bMaxPeople,&ServerNt,sizeof(ServerNt),MDM_GM_GAME_NOTIFY,ASS_NT_CARD,0);

	//	return TRUE;
	//}
	return FALSE;
}

/**
 * ���͵���
 */
BOOL CServerGameDesk::SendBackCard()
{
	//BOOL bExtBack=FALSE;
	//if (m_iUpGradePeople==-1)	//��ׯ״̬
	//{
	//	m_iUpGradePeople=(m_iNtPeople==-1)?0:m_iNtPeople;
	//}
	//
	//if (m_iNtPeople==-1)	//û���˷��ƣ���������
	//{
	//	bExtBack=TRUE;
	//	m_iNtPeople=m_iUpGradePeople;
	//	m_Logic.SetNTHuaKind(UG_HEI_TAO);
	//}

	////��������
	//m_bGameStation=GS_WAIT_BACK;
	//::CopyMemory(&m_iUserCard[m_iUpGradePeople][m_iUserCardCount[m_iUpGradePeople]],
	//			 m_iBackCard,sizeof(BYTE)*((m_iPlayCardCount==2)?8:6));
	//m_iUserCardCount[m_iUpGradePeople]+=((m_iPlayCardCount==2)?8:6);

	//if (bExtBack==FALSE)
	//{
	//	//����״̬��Ϣ
	//	BackStationStruct UserGiveBack;
	//	UserGiveBack.iNtPeople=m_iNtPeople;
	//	UserGiveBack.iGiveBackStation=m_iUpGradePeople;
	//	for (int i=0;i<m_bMaxPeople;i++) SendGameData(i,&UserGiveBack,sizeof(UserGiveBack),MDM_GM_GAME_NOTIFY,ASS_WAIT_BACK,0);
	//	SendWatchData(m_bMaxPeople,&UserGiveBack,sizeof(UserGiveBack),MDM_GM_GAME_NOTIFY,ASS_WAIT_BACK,0);

	//	//���͵�����Ϣ
	//	BackCardStruct BackCard;
	//	BackCard.iBackCardCount=(m_iPlayCardCount==2)?8:6;
	//	::CopyMemory(BackCard.iBackCard,m_iBackCard,sizeof(BYTE)*BackCard.iBackCardCount);
	//	SendGameData(m_iUpGradePeople,&BackCard,sizeof(BackCard),MDM_GM_GAME_NOTIFY,ASS_BACK_CARD,0);
	//	SendWatchData(m_iUpGradePeople,&BackCard,sizeof(BackCard),MDM_GM_GAME_NOTIFY,ASS_BACK_CARD,0);
	//}
	//else	//���͵�����Ϣ
	//{
	//	BackCardExStruct BackCard;
	//	BackCard.iNtHua=0;m_Logic.GetNTHuaKind();
	//	BackCard.iGiveBackPeople=m_iUpGradePeople;
	//	BackCard.iBackCardCount=(m_iPlayCardCount==2)?8:6;
	//	::CopyMemory(BackCard.iBackCard,m_iBackCard,sizeof(BYTE)*BackCard.iBackCardCount);
	//	for (int i=0;i<m_bMaxPeople;i++) SendGameData(i,&BackCard,sizeof(BackCard),MDM_GM_GAME_NOTIFY,ASS_BACK_CARD_EX,0);
	//	SendWatchData(m_bMaxPeople,&BackCard,sizeof(BackCard),MDM_GM_GAME_NOTIFY,ASS_BACK_CARD_EX,0);
	//}

	return TRUE;
}

/**
 * �û��Ͻ�����
 */
BOOL CServerGameDesk::UserGiveBackCard(BYTE bDeskStation, BYTE iBackCard[], int iCardCount)
{
	//if ((iCardCount!=((m_iPlayCardCount==2)?8:6))||(bDeskStation!=m_iUpGradePeople)) return FALSE;
	//
	////��¼����
	//::CopyMemory(m_iBackCard,iBackCard,sizeof(BYTE)*iCardCount);
	//if (m_Logic.RemoveCard(iBackCard,iCardCount,m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation])==0) 
	//	return FALSE;
	//m_iUserCardCount[bDeskStation]-=iCardCount;

	////���ͽ����Ϣ
	//BackCardStruct BackCard;
	//BackCard.iBackCardCount=(m_iPlayCardCount==2)?8:6;
	//::CopyMemory(BackCard.iBackCard,m_iBackCard,sizeof(BYTE)*BackCard.iBackCardCount);
	//SendGameData(bDeskStation,&BackCard,sizeof(BackCardStruct),MDM_GM_GAME_NOTIFY,ASS_BACK_CARD_RES,0);
	//SendWatchData(bDeskStation,&BackCard,sizeof(BackCardStruct),MDM_GM_GAME_NOTIFY,ASS_BACK_CARD_RES,0);

	//��ʼ��Ϸ
	///BeginPlayUpGrade();

	return TRUE;
}

/**
 * ��һ�ֿ�ʼ
 */
BOOL CServerGameDesk::NewPlayTurn(int iWiner)
{
	return TRUE;
}

/**
 * �û������뿪
 */
BOOL CServerGameDesk::UserHaveThing(BYTE bDeskStation, char * szMessage)
{
	if (m_bGameStation<GS_SEND_CARD) return TRUE;

	m_iLeaveArgee|=1<<bDeskStation;
	if (m_iLeaveArgee!=15)
	{
		HaveThingStruct HaveThing;
		HaveThing.pos=bDeskStation;
		::CopyMemory(HaveThing.szMessage,szMessage,60*sizeof(char));
		for (int i=0;i<m_bMaxPeople;i++)
			if (i!=bDeskStation) SendGameData(i,&HaveThing,sizeof(HaveThing),MDM_GM_GAME_NOTIFY,ASS_HAVE_THING,0);
	}
	else GameFinish(bDeskStation,GF_SALE);

	return TRUE;
}

/**
 * ͬ���û��뿪
 */
BOOL CServerGameDesk::ArgeeUserLeft(BYTE bDeskStation, BOOL bArgee)
{
	if (bArgee) m_iLeaveArgee|=1<<bDeskStation;
	else m_iLeaveArgee&=~1<<bDeskStation;
	if (m_iLeaveArgee!=3)
	{
		m_bQueryQuit=true;
		LeaveResultStruct Leave;
		Leave.bDeskStation=bDeskStation;
		Leave.bArgeeLeave=bArgee;
		for (int i=0;i<m_bMaxPeople;i++) 
			if (i!=bDeskStation) SendGameData(i,&Leave,sizeof(Leave),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	}
	else GameFinish((bDeskStation+1)%2,GF_SALE);

	return TRUE;
}

/******************************************************************************************************/
/***********************************************************************************************************/

// �����߼���

/***********************************************************************************************************/

/**
 * ���캯��
 */
CUpGradeGameLogic::CUpGradeGameLogic(void)
{
}

void CUpGradeGameLogic::InitBoard(int iMeFlag, int qi[][9])
{
	for (int i = 0;i < 10;i++)
		for (int j = 0;j < 9;j++)
			qi[i][j] = XQ_QIZI_NONE;


	if (iMeFlag == XQ_FLAG_RED)
	{
		qi[0][0] = qi[0][8] = XQ_BLACK_JU;
		qi[0][1] = qi[0][7] = XQ_BLACK_MA;
		qi[0][2] = qi[0][6] = XQ_BLACK_XIANG;
		qi[0][3] = qi[0][5] = XQ_BLACK_SHI;
		qi[0][4]		    = XQ_BLACK_JIANG;
		qi[2][1] = qi[2][7] = XQ_BLACK_PAO;
		qi[3][0] = qi[3][2] = qi[3][4] = qi[3][6] = qi[3][8] =XQ_BLACK_BIN;


		qi[6][0] = qi[6][2] = qi[6][4] = qi[6][6] = qi[6][8] =XQ_RED_BIN;
		qi[9][0] = qi[9][8] = XQ_RED_JU;
		qi[9][1] = qi[9][7] = XQ_RED_MA;
		qi[9][2] = qi[9][6] = XQ_RED_XIANG;
		qi[9][3] = qi[9][5] = XQ_RED_SHI;
		qi[9][4]		    = XQ_RED_JIANG;
		qi[7][1] = qi[7][7] = XQ_RED_PAO;

	}

	else if (iMeFlag == XQ_FLAG_BLACK)
	{
		qi[0][0] = qi[0][8] = XQ_RED_JU;
		qi[0][1] = qi[0][7] = XQ_RED_MA;
		qi[0][2] = qi[0][6] = XQ_RED_XIANG;
		qi[0][3] = qi[0][5] = XQ_RED_SHI;
		qi[0][4]		    = XQ_RED_JIANG;
		qi[2][1] = qi[2][7] = XQ_RED_PAO;
		qi[3][0] = qi[3][2] = qi[3][4] = qi[3][6] = qi[3][8] =XQ_RED_BIN;


		qi[6][0] = qi[6][2] = qi[6][4] = qi[6][6] = qi[6][8] =XQ_BLACK_BIN;
		qi[9][0] = qi[9][8] = XQ_BLACK_JU;
		qi[9][1] = qi[9][7] = XQ_BLACK_MA;
		qi[9][2] = qi[9][6] = XQ_BLACK_XIANG;
		qi[9][3] = qi[9][5] = XQ_BLACK_SHI;
		qi[9][4]			= XQ_BLACK_JIANG;
		qi[7][1] = qi[7][7] = XQ_BLACK_PAO;
	}

	return;
}

BOOL CUpGradeGameLogic::Win(int iMeFlag, int qi[][9])
{
	for (int i = 0;i < 9;i++)
		for (int j = 0;j < 10;j++)
		{
			int qz = qi[j][i];
			if (iMeFlag == XQ_FLAG_RED)
			{
				if (qz == XQ_BLACK_JIANG) return FALSE;
			}
			if (iMeFlag == XQ_FLAG_BLACK)
			{
				if (qz == XQ_RED_JIANG) return FALSE;
			}

		}
		return TRUE;
}

BOOL CUpGradeGameLogic::Lose(int iMeFlag, int qi[][9])
{
	for (int i = 0;i < 9;i++)
		for (int j = 0;j < 10;j++)
		{
			int qz = qi[j][i];
			if (iMeFlag == XQ_FLAG_RED)
			{
				if (qz == XQ_RED_JIANG) return FALSE;
			}
			if (iMeFlag == XQ_FLAG_BLACK)
			{
				if (qz == XQ_BLACK_JIANG) return FALSE;
			}

		}
		return FALSE;
}

BOOL CUpGradeGameLogic::CheckWinOrLose(int iMeFlag, int qi[][9], BOOL &bIfIWin)
{
	BOOL bb = FALSE;
	if (Win(iMeFlag,qi))
	{
		bIfIWin = TRUE;
		bb = TRUE;
	}
	if (Lose(iMeFlag,qi))
	{
		bIfIWin = FALSE;
		bb = TRUE;
	}
	return bb;
}

/**
 * ��ȡ��ʵ�÷�
 */
int CServerGameDesk::GetTruePoint(BYTE bDeskStation)
{
	if (m_bQiuHe)
	{
		return 0;
	}
	if (m_iWinFlag == XQ_FLAG_RED)
	{
		if (m_bFirstRed)
		{
		return  (bDeskStation == 0)?4:-4;
		//	m_iTurePoint[0] = 4;
		//	m_iTurePoint[1] = -4;
		}
		else
		{
		return  (bDeskStation == 1)?4:-4;
		//	m_iTurePoint[1] = 4;
		//	m_iTurePoint[0] = -4;
		}
	}
	if (m_iWinFlag == XQ_FLAG_BLACK)
	{	
		if (m_bFirstRed)
		{
			return  (bDeskStation == 1)?4:-4;
			//m_iTurePoint[0] = -4;
		//	m_iTurePoint[1] = 4;
		}
		else
		{
			return  (bDeskStation == 0)?4:-4;
			//m_iTurePoint[1] = -4;
			//m_iTurePoint[0] = 4;
		}
	}
	return 0;
}

/**
 * ���ܿ۷�
 */
int CServerGameDesk::GetRunPublish()
{
	return m_pDataManage->m_InitData.uRunPublish;
}

/**
 * �Α���A����
 */
int CServerGameDesk::GetRoomBasePoint()
{
	int iBasePoint=m_pDataManage->m_InitData.uBasePoint;//����
	return (iBasePoint>0?iBasePoint:1);
}

/**
 * ���ӱ���
 */
int CServerGameDesk::GetDeskBasePoint()
{
	return 1;//GetTableBasePoint();
}

/**
 * ͳ�Ʊ���
 */
int CServerGameDesk::GetHumanBasePoint()
{
	//��ҳ������бȷֻ���
	//	if(m_pDataManage->m_InitData.uComType == TY_MONEY_GAME)

	__int64 minval=m_pUserInfo[0]->m_UserData.dwPoint;
	for(int i=1;i<PLAY_COUNT;i++)
	{
		minval=min(minval,m_pUserInfo[i]->m_UserData.dwPoint);
	}

	__int64 uBasePoint[22]=
	{
		0,10,30,60,
		100,200,400,640,
		920,1240,2140,4640,
		7390,10390,13640,20640,
		35640,51640,68640,86640,
		108640,138640
	};
	int BasePoint[22]=
	{
		1,2,3,4,
		5,10,12,14,
		16,18,50,55,
		60,65,70,150,
		160,170,180,190,
		200,210
	};
	for (int i=0;i<22;i++)	
		if (minval<uBasePoint[i]) 
			return BasePoint[i];
	return BasePoint[21];
}