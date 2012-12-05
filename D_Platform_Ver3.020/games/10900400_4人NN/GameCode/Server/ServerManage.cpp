#include "StdAfx.h"
#include "ServerManage.h"

//��Ϸ��չ����
BYTE CServerGameDesk:: m_iAllCardCount;
int CServerGameDesk:: m_iSendCardTime;
BYTE CServerGameDesk:: m_iThinkTime;
BYTE CServerGameDesk:: m_iBeginTime;
BYTE CServerGameDesk:: m_iCallScoreTime;
BYTE CServerGameDesk:: m_iXiaZhuTime;
BOOL CServerGameDesk:: m_bTurnRule;		//˳ʱ�뻹����ʱ��
//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	m_iLeaveArgee     = 0;								//ͬ���뿪
	m_iFirstOutPeople = -1;							//��һ����ע��
	m_iOutCardPeople  = -1;
	m_iLastOutPeople  = -1;
	m_iNtPeople       = -1;
	m_iUpGradePeople  = -1;						
	m_iBeenPlayGame   = 0;								//�Ѿ�����ľ���
	m_iPrepareNT      = 255;
	m_bSuperSet       = FALSE;
	memset(m_bIsSuper,0,sizeof(m_bIsSuper));
	::memset(m_iCallScore,-1,sizeof(m_iCallScore));		//�з�
	m_bGameStation=GS_WAIT_ARGEE;//GS_WAIT_SETGAME;		//��Ϸ״̬
	memset(m_iTotalCard,0,sizeof(m_iTotalCard));
	
	for(int i=0; i<PLAY_COUNT; i++)
		memset(m_bBullCard[i],0,sizeof(m_bBullCard[i]));
	
	memset(m_bAdd,0,sizeof(m_bAdd));
	memset(m_bIsCut,0,sizeof(m_bIsCut));
#ifdef SUPER_VERSION
	for(int i = 0; i < 8; i++)
	{
		m_iSuperCardCount[i] = 0;
		::memset(m_bSuperCardList[i],0,sizeof(m_bSuperCardList[i]));
	}
	m_bSuperStation = -1;
#endif
	m_bControlWin = 255;
	m_bControlLost = 255;
	m_iFirstJiao  = 255;

}
//����ini�����ļ�
BOOL CServerGameDesk::LoadIni()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = TEXT("game");
	m_iAllCardCount = f.GetKeyVal(key,"cardcount",54);

	m_iCardShape &=0x00000000;
	m_iCardShape |= (f.GetKeyVal(key,"Yn",0)&0xFFFFFFFF);//��ţ
	m_iCardShape |= ((f.GetKeyVal(key,"Jn",0)<<1)&0xFFFFFFFF);//��ţ
	m_iCardShape |= ((f.GetKeyVal(key,"Bomb",0)<<2)&0xFFFFFFFF);//ը��
	m_iCardShape |= ((f.GetKeyVal(key,"Five",0)<<3)&0xFFFFFFFF);//��С

	m_GameType =  f.GetKeyVal(key,"GameType",0);
	m_iGameNoteSetType = f.GetKeyVal(key,"GameNoteSetType",0);

	m_iGoodCard        =  f.GetKeyVal(key,"iGoodCard",80);
	m_bHaveKing        = f.GetKeyVal(key,"HaveKing",1);//�Ƿ�����
	
	m_Logic.SetCardShape(m_iCardShape);

	CString keyName;
	int base = 0;
	for (int i=0; i<MAX_SHAPE_COUNT; i++)
	{
		keyName.Format("CardShape%d",i);
		if (i < 6)
		{
			base = 1;
		}
		else if(i < 10)
		{
			base = i - 5;
		}
		else if (i == 10)
		{
			base = 5;
		}
		else if(i == 11)
		{
			base = 6;
		}
		else if(i == 12)
		{
			base = 7;
		}
		else if(i > 12)
		{
			base = 8;
		}
		m_bCardShapeBase[i] = f.GetKeyVal(key,keyName,base);
	}


	key=TEXT("other");
	m_iSendCardTime = f.GetKeyVal(key,"sendcardtime",120);
	m_iBeginTime = f.GetKeyVal(key,"begintime",30);
	m_iThinkTime = f.GetKeyVal(key,"thinktime",30);
	m_iCallScoreTime = f.GetKeyVal(key,"CallScoretime",15);
	m_iXiaZhuTime = f.GetKeyVal(key,"XiaZhutime",15);
	m_bTurnRule = f.GetKeyVal(key,"TurnRule",0);
	
	
	//	m_iGiveBackTime = f.GetKeyVal(key,"givebacktime",30);
	return true;
}
//������ע
BOOL CServerGameDesk::LoadExtIni()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = TEXT("game");
	m_iBaseNote = f.GetKeyVal(key,"basenote",10);
	return TRUE;
}
//���ݷ���ID���������ļ�
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{
	TCHAR szKey[20];
	wsprintf(szKey,"%s_%d",SKIN_FOLDER,iRoomID);

	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = szKey;
	//	m_iLimitNote = f.GetKeyVal(key,"limitnote",m_iLimitNote);
	m_iBaseNote = f.GetKeyVal(key,"basenote",m_iBaseNote);
	m_iGameNoteSetType = f.GetKeyVal(key,"GameNoteSetType",m_iGameNoteSetType);
	m_iGoodCard        =  f.GetKeyVal(key,"iGoodCard",m_iGoodCard);
	m_bHaveKing        = f.GetKeyVal(key,"HaveKing",m_bHaveKing);//�Ƿ�����
	return TRUE;
}
//��������
CServerGameDesk::~CServerGameDesk(void)
{
}

//��ʼ��
bool CServerGameDesk::InitDeskGameStation()
{

	/*if(m_pDataManage->m_InitData.uMaxPeople>50)
	{
		CString sfile;
		sfile.Format("ErrRoom_%d.log",m_pDataManage->m_InitData.uRoomID);
		CFile f;
		f.Open(sfile,CFile::modeCreate|CFile::modeNoInherit|CFile::modeReadWrite);
		sfile.Format("[%s] ���� %d �������ã�%d������50��",CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),
			m_pDataManage->m_InitData.uRoomID,m_pDataManage->m_InitData.uMaxPeople);
		f.Write(sfile,sfile.GetLength());
		f.Close();
		typedef struct{int a;}_tag;
		_tag *ta=NULL;
		ta->a=0;
		return false;
	}
	*/
	//���������ļ�
	LoadIni();
	//����ͨ����ע
	LoadExtIni();

	//���¼��������ļ������
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	return true;
}
//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	if(TIME_BEGIN_ANIMAL==uTimerID)
	{
		if (m_bGameStation==GS_SEND_CARD) 
		{	
			KillTimer(TIME_BEGIN_ANIMAL);
			SetTimer(TIME_SEND_CARD,m_iSendCardTime);
		}else
			KillTimer(TIME_BEGIN_ANIMAL);

		return true;
	}
	if (uTimerID==TIME_SEND_CARD)	//���ƶ�ʱ��
	{
		if (m_bGameStation==GS_SEND_CARD) 
			SendCard();
		else KillTimer(TIME_SEND_CARD);
		return TRUE;
	}
	if(uTimerID==TIME_COLLECT_JETON)
	{
		if (m_bGameStation==GS_PLAY_GAME)
			CollectJeton();
		else KillTimer(TIME_COLLECT_JETON);
		return TRUE;
	}
	if(uTimerID==TIME_SEND_A_CARD)
	{
		if (m_bGameStation==GS_PLAY_GAME)
			SendACard();
		else KillTimer(TIME_SEND_A_CARD);
		return TRUE;
	}
	if (uTimerID==TIME_GAME_FINISH)	//������ʱ��
	{
		KillTimer(TIME_GAME_FINISH);
		if (m_bGameStation==GS_PLAY_GAME || m_bGameStation == GS_SEND_CARD) 
			GameFinish(0,GF_NORMAL);
		return TRUE;
	}
	if (uTimerID==TIME_NOTE)
	{
		KillTimer(TIME_NOTE);
		if (m_iGameFlag == GS_NOTE && m_bGameStation == GS_SEND_CARD)
		{
			for (int i=0; i<PLAY_COUNT; i++)
			{
				if (m_pUserInfo[i] != NULL && m_iUserStation[i] == TYPE_PLAY_GAME && i!= m_iUpGradePeople)
				{
					m_iUserStation[i] = TYPE_GIVE_UP;
				}
			}
			if (CountNotePeople() == 1)
			{
				GameFinish(0,GF_SALE);
				return TRUE;
			}
			m_iSendCardPos = 0;
			SetTimer(TIME_SEND_CARD,m_iSendCardTime);
		}
		return TRUE;

	}
	if (uTimerID==TIME_CALL_NT)
	{
		KillTimer(TIME_CALL_NT);
		if (m_iGameFlag == GS_FLAG_CALL_SCORE && m_bGameStation == GS_SEND_CARD)
		{
			UserCallScore(m_bCurrentCallScore, 1);
		}
		return TRUE;
	}
	if (uTimerID==TIME_CUTLINE_AUTO)	//�����Զ���ţ
	{
		KillTimer(TIME_CUTLINE_AUTO);

		// ��ţǰ���ߣ���ׯ�����������û��עǰ���ߣ���ϵͳ�Զ������ţ��
		for (BYTE i=0; i<PLAY_COUNT; i++)
		{
			if (NULL!=m_pUserInfo[i] && m_iUserStation[i] != -1 && m_iUserStation[i] != TYPE_OPEN/*&& m_bIsCut[i]*/)
			{
				BYTE bResult[3];
				memset(bResult, 0, sizeof(bResult));
				tagUserProcess Verb;
				Verb.iVrebType = TYPE_OPEN;

				if (m_Logic.GetBull(m_iUserCard[i], 5, bResult))
				{
					::CopyMemory(m_bBullCard[i], bResult, sizeof(bResult));
				}
				else
				{
					memset(m_bBullCard[i], 0, sizeof(m_bBullCard[i]));
					m_bBullCard[i][0] = m_iUserCard[i][0];
					m_bBullCard[i][1] = m_iUserCard[i][1];
					m_bBullCard[i][2] = m_iUserCard[i][2];
				}

				UserActiveResult(i, Verb.iVrebType, 0);
			}
		}

		CheckFinish();
	}
	return __super::OnTimer(uTimerID);
}

//�����Ϣ������
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch(pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:		//ǿ���˳�//��ȫ�˳�
		{
			
			UserFourceQuit(bDeskStation);

			char cKey[10];
			CString sPath=CBcfFile::GetAppPath();
			int iResult = 1;
			if (CBcfFile::IsFileExist(sPath + "SpecialRule.bcf"))
			{
				CBcfFile fsr( sPath + "SpecialRule.bcf");
				sprintf(cKey, "%d", NAME_ID);
				iResult = fsr.GetKeyVal (_T("ForceQuitAsAuto"), cKey, 0);
			}

			if (iResult == 0)
			{
				return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser); 
			}
			return true;
		}
	}
	return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}

//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	CString aa;
	aa.Format("Test: Server Msg %d", pNetHead->bAssistantID);
	OutputDebugString(aa);

	switch (pNetHead->bAssistantID)
	{
#ifdef SUPER_VERSION
		case 79://�����ͻ���
			{
				if(bWatchUser)
					return FALSE;
				if(m_bGameStation == GS_PLAY_GAME)
					return true;
				SetCardStruct * pCard = (SetCardStruct*)pData;

				m_bSuperSet = TRUE;
				//Ĭ�Ϸ�ʽ(��ǰ�������)
				m_bSuperStation = bDeskStation ;
				if(pCard->paicount[0] > 0)//�������Լ�
				{
					m_iSuperCardCount[m_bSuperStation] = pCard->paicount[0];
					::CopyMemory(m_bSuperCardList[m_bSuperStation],pCard->pai[0],m_iSuperCardCount[m_bSuperStation]);

				}else//���ö��0λ�ø�Ĭ��
				{
					for(int i = 1; i < 8; i ++)
					{
						m_iSuperCardCount[i-1] = pCard->paicount[i];
						::CopyMemory(m_bSuperCardList[i-1],pCard->pai[i],sizeof(BYTE)*m_iSuperCardCount[i-1]);
					}
				}
				return TRUE;
			}
#endif
			//�Q����
		case ASS_GM_SUPERCLIENT:
			{
				if (bWatchUser)
					return FALSE;
				if(uSize!=sizeof(ChangeBackStruct))
					return FALSE;
				ChangeBackStruct * pChangeCard=(ChangeBackStruct *)pData;
				return ChangeBackCard(pChangeCard->bDeskStation,pChangeCard->bSourceCard,pChangeCard->bResultCard);
			}
		case ASS_GM_AGREE_GAME:		//�û�ͬ����Ϸ
			{
				if (bWatchUser)
					return TRUE;
				/*		if ((bDeskStation==0)&&(m_bGameStation==GS_WAIT_SETGAME))	//��������
				{
				if (uSize!=sizeof(UserArgeeGame)) return FALSE;
				UserArgeeGame * pGameSet=(UserArgeeGame *)pData;
				if (!UserSetGame(pGameSet)) return FALSE;
				__super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
				return TRUE;
				}
				*/	
				//OutputDebugString("zhtlog:�û�ͬ����Ϸ");
				//�������
				if ((m_bGameStation!=GS_WAIT_ARGEE)&&(m_bGameStation!=GS_WAIT_NEXT)) 
					return TRUE;
					
				if(m_bGameStation!=GS_WAIT_NEXT)
				{
					m_bGameStation=GS_WAIT_ARGEE;
					//GameFinish(0,GF_SALE);
				}
				return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
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
				SendGameData((bDeskStation+2)%PLAY_COUNT,MDM_GM_GAME_NOTIFY,ASS_STOP_THING,0);
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
					SendGameData((bDeskStation+2)%PLAY_COUNT,&stop,sizeof(stop),MDM_GM_GAME_NOTIFY,ASS_AGREE_STOP,0);

					MessageStruct Mess;
					memset(&Mess,0,sizeof(Mess));
					lstrcpy(Mess.Message,"�мҾ�����������ǰ�����˱��֣�");
					SendGameData((bDeskStation+1)%PLAY_COUNT,&Mess,sizeof(Mess),MDM_GM_GAME_NOTIFY,ASS_MESSAGE,0);
					SendGameData((bDeskStation+3)%PLAY_COUNT,&Mess,sizeof(Mess),MDM_GM_GAME_NOTIFY,ASS_MESSAGE,0);

					memset(&Mess,0,sizeof(Mess));
					lstrcpy(Mess.Message,"�мҾ�����������ǰ�����˱��֣�");
					SendWatchData(m_bMaxPeople,&Mess,sizeof(Mess),MDM_GM_GAME_NOTIFY,ASS_MESSAGE,0);
					//������Ϸ
					if (m_bGameStation==GS_PLAY_GAME) 
						GameFinish(0,GF_NORMAL);
				}
				else
				{
					StopResultStruct stop;
					stop.bArgee=false;
					SendGameData((bDeskStation+2)%PLAY_COUNT,&stop,sizeof(stop),MDM_GM_GAME_NOTIFY,ASS_AGREE_STOP,0);
				}
				return true;
			}

		case ASS_BEGIN_UPGRADE:
			{
				//OutputDebugString("zhtlog:��ע11��");
				if(bWatchUser)
					return true;
				if (m_iPerJuTotalNote[bDeskStation]!=0 || m_iUserStation[bDeskStation] == TYPE_GIVE_UP)
				{
					//OutputDebugString("zhtlog:��ע22��");
					return true;
				}
				if (m_iUserStation[bDeskStation] == -1)
				{
					//OutputDebugString("zhtlog:��עʧ�ܣ�");
					return true;
				}

				tagUserProcess *pVerb = (tagUserProcess *) pData;
				return UserNoteResult(bDeskStation,pVerb->iVrebType,pVerb->iNote);

				return TRUE;
			}

		case ASS_VREB_CHECK:		//�û�����
			{
				if(bWatchUser)
					return true;
				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
					return true;
				if (m_iUserStation[bDeskStation] == -1)
				{
					return true;
				}

				tagUserProcess *pVerb = (tagUserProcess *) pData;
				if (pVerb->iVrebType == TYPE_OPEN)
				{
					::CopyMemory(m_bBullCard[bDeskStation],pVerb->bUpCard,sizeof(pVerb->bUpCard));
					for(int i = 0;i < PLAY_COUNT; ++i)
					{
						if(i != bDeskStation)
						{
							SendGameData(i,NULL,0,MDM_GM_GAME_NOTIFY,ASS_TAN_PAI,0);
						}
					}
				}
				//else
				return UserActiveResult(bDeskStation,pVerb->iVrebType,pVerb->iNote);

				if (CheckFinish())
					return true;
				return true;
			}
		case ASS_CALL_SCORE:
			{
				if ((bWatchUser)
					||(uSize!=sizeof(CallScoreStruct))) 
					return true;
				if (m_iUserStation[bDeskStation] == -1)
				{
					return true;
				}
				CallScoreStruct * pCallScore=(CallScoreStruct *)pData;
				UserCallScore(bDeskStation,pCallScore->iValue);
				return true;
			}
		case ASS_REPLACE_CALL_SCORE:
			{
				/*	if (uSize!=sizeof(ReplaceCallScoreStruct)) 
				return FALSE;
				ReplaceCallScoreStruct * pReplace=(ReplaceCallScoreStruct *)pData;
				UserCallScore(pReplace->bDeskStation,pReplace->CallScore);
				return true;*/
			}
		case ASS_SUPERSEDE_VREB_CHECK://�����û�����
			{
				return true;
				if(bWatchUser)
					return false;
				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
					return true;
				SupersedeUserNote *pUserNote = (SupersedeUserNote *) pData;
				if (m_bGameStation == GS_PLAY_GAME)
					return UserActiveResult(pUserNote->bDeskStation ,pUserNote->iVrebType,pUserNote->iNote);
				else
					if (m_iGameFlag == GS_FLAG_CALL_SCORE)
						return UserCallScore(pUserNote->bDeskStation,pUserNote->iVrebType);
				return UserNoteResult(pUserNote->bDeskStation ,pUserNote->iVrebType,pUserNote->iNote);
			}

		case ASS_CONTROL_WIN:
			{

				//m_bSuperSet = TRUE;
				ControlWinStruct * pControl = (ControlWinStruct *) pData;

				if (m_bIsSuper[bDeskStation])
				{
					m_bControlWin = pControl->bWinStation;
					m_bControlLost = pControl->bLoseStation;
				}
				
				return TRUE;
			}
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	if (!bWatchUser)
		SuperSetCard();
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬

	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			GameStation_2 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion=DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2=DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();
			GameStation.iCallScoreTime = m_iCallScoreTime;			//�з�ʱ��
			GameStation.iXiaZhuTime = m_iXiaZhuTime;
			//��Ϸ������Ϣ����
			::CopyMemory(GameStation.iLimitNote,&m_iLimitNote,sizeof(m_iLimitNote));
			GameStation.iBaseNote=m_iBaseNote;							//��ע
			GameStation.iBeginTime=m_iBeginTime;
			GameStation.iThinkTime=m_iThinkTime;
			GameStation.iAllCardCount=m_iAllCardCount;
			GameStation.iCardShape = m_iCardShape;
			GameStation.bGameType = m_GameType;
			GameStation.iGameNoteSetType = m_iGameNoteSetType;
			GameStation.bIsSuper = m_bIsSuper[bDeskStation];

			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	case GS_SEND_CARD:		//����״̬
		{
			GameStation_3 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion=DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2=DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();
			GameStation.iCallScoreTime = m_iCallScoreTime;			//�з�ʱ��
			GameStation.iXiaZhuTime = m_iXiaZhuTime;
			GameStation.iGameFlag = m_iGameFlag;
			//��Ϸ������������
			GameStation.iBeginTime=m_iBeginTime;								//���ע
			GameStation.iBaseNote=m_iBaseNote;//PLAY_BASE_NOTE;							//��ע
			::CopyMemory(GameStation.iLimitNote,&m_iLimitNote,sizeof(m_iLimitNote));
			GameStation.iThinkTime=m_iThinkTime;
			GameStation.iAllCardCount=m_iAllCardCount;
			GameStation.iUpGradePeople=m_iUpGradePeople;		
			GameStation.iBeenPlayGame=m_iBeenPlayGame;	

			GameStation.tag.bCountNotePeople=CountNotePeople();
			GameStation.tag.iOutPeople=m_bCurrentCallScore;					//��ǰ�з�
//			GameStation.tag.iTotalNote=m_iTotalNote;						//��ǰ��ע
			GameStation.iCardShape = m_iCardShape;
			GameStation.bGameType = m_GameType;              //��Ϸ���ࡣ0Ϊ��ͨ��1Ϊͬϲ��
			GameStation.iGameNoteSetType = m_iGameNoteSetType;
			GameStation.bIsSuper = m_bIsSuper[bDeskStation];
			GameStation.bPlayer  = m_iUserStation[bDeskStation]!=-1?TRUE:FALSE;                         //�Ƿ����
			//�û�������
			::CopyMemory(GameStation.iUserCardCount,m_iUserCardCount,sizeof(m_iUserCardCount));
			::CopyMemory(GameStation.iUserCard,m_iUserCard,sizeof(m_iUserCard));
			//ϵͳ����
			GameStation.iSysCardCount=m_iAllCardCount-m_iSendCardPos;
			::CopyMemory(GameStation.iTotalNote,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));
			//��������
			int iSendSize=sizeof(GameStation);//-sizeof(GameStation.iUserCard)+sizeof(BYTE)*m_iUserCardCount[bDeskStation];
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,iSendSize);
			return TRUE;
		}
	case GS_PLAY_GAME:	//��Ϸ��״̬
		{
			GameStation_4 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion=DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2=DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();
			GameStation.iCallScoreTime = m_iCallScoreTime;			//�з�ʱ��
			GameStation.iXiaZhuTime = m_iXiaZhuTime;

			::CopyMemory(GameStation.iLimitNote,&m_iLimitNote,sizeof(m_iLimitNote));
			GameStation.iBaseNote=m_iBaseNote;//PLAY_BASE_NOTE;							//��ע
			GameStation.iBeginTime=m_iBeginTime;
			GameStation.iThinkTime=m_iThinkTime;
			GameStation.iAllCardCount=m_iAllCardCount;
			GameStation.iUpGradePeople=m_iUpGradePeople;	
			GameStation.iBeenPlayGame=m_iBeenPlayGame;		
			GameStation.iOutCardPeople=m_iOutCardPeople;
			GameStation.iFirstOutPeople=m_iFirstOutPeople;

			GameStation.tag.bCountNotePeople=CountNotePeople();
			GameStation.tag.iOutPeople=m_iOutCardPeople;					//��ǰѺע��
//			GameStation.tag.iTotalNote=m_iTotalNote;						//��ǰ��ע
			GameStation.iCardShape = m_iCardShape;
			GameStation.bGameType = m_GameType;              //��Ϸ���ࡣ0Ϊ��ͨ��1Ϊͬϲ��
			GameStation.iGameNoteSetType = m_iGameNoteSetType;
			GameStation.bSplit=false;
			GameStation.bIsSuper = m_bIsSuper[bDeskStation];
			GameStation.bPlayer  = m_iUserStation[bDeskStation]!=-1?TRUE:FALSE;                         //�Ƿ����
			memcpy(GameStation.iUserStation, m_iUserStation, sizeof(GameStation.iUserStation));


			if (m_iUserStation[bDeskStation]==TYPE_OPEN)
			{
				GameStation.bSplit=true;
				memcpy(GameStation.bBullCard, m_bBullCard[bDeskStation], sizeof(GameStation.bBullCard));
			}

			//�û�������
			::CopyMemory(GameStation.iUserCardCount,m_iUserCardCount,sizeof(m_iUserCardCount));
			::CopyMemory(GameStation.iUserCard,m_iUserCard,sizeof(m_iUserCard));
			//��ǰ���û���ע����
			::CopyMemory(GameStation.iTotalNote,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));
			//��������
			int iSendSize=sizeof(GameStation);//-sizeof(GameStation.iUserCardList)+sizeof(BYTE)*iPos;
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,iSendSize);
			return TRUE;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{
			GameStation_5 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion=DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2=DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();
			GameStation.iBeginTime=m_iBeginTime;
			GameStation.iThinkTime=m_iThinkTime;
			GameStation.iAllCardCount=m_iAllCardCount;
			GameStation.iUpGradePeople=m_iUpGradePeople;		
			GameStation.iBeenPlayGame=m_iBeenPlayGame;		
			GameStation.iCallScoreTime = m_iCallScoreTime;			//�з�ʱ��
			GameStation.iXiaZhuTime = m_iXiaZhuTime;
			GameStation.iCardShape = m_iCardShape;
			GameStation.bGameType = m_GameType;              //��Ϸ���ࡣ0Ϊ��ͨ��1Ϊͬϲ��
			GameStation.iGameNoteSetType = m_iGameNoteSetType;
			GameStation.bIsSuper = m_bIsSuper[bDeskStation];
			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	}
	return false;
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	for(int i=0; i<PLAY_COUNT; i++)
		memset(m_bBullCard[i],0,sizeof(m_bBullCard[i]));
	KillTimer(TIME_SEND_CARD);
	//	KillTimer(TIME_WAIT_NT);
	KillTimer(TIME_GAME_FINISH);
	KillTimer(TIME_BEGIN_ANIMAL);
	KillTimer(TIME_SEND_A_CARD);
	KillTimer(TIME_COLLECT_JETON);
	KillTimer(TIME_CUTLINE_AUTO);

	if ((bLastStation==GFF_FORCE_FINISH)||(bLastStation==GF_SALE))
	{
		m_iBeenPlayGame=0;
		m_iNtPeople=-1;	

	}
	m_bControlWin = 255;
	m_bControlLost = 255;
	//	m_iUpGradePeople=-1;
	m_iLeaveArgee=0;
	m_iFirstOutPeople=-1;
	m_iOutCardPeople=-1;
	m_iLastOutPeople=-1;
	m_iSendCardPos=0;
	//m_iUpGradePeople=-1;
	m_iPrepareNT = 255;
	m_bSuperSet       = FALSE;
	::memset(m_iCallScore,-1,sizeof(m_iCallScore));		//�з�
	//m_Logic.SetNTHuaKind(UG_ERROR_HUA);
	//memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	memset(m_iUserCardCount, 0, sizeof(m_iUserCardCount));
	memset(m_bAdd, 0, sizeof(m_bAdd));
	memset(m_bIsCut, 0, sizeof(m_bIsCut));
#ifdef SUPER_VERSION
	for(int i = 0; i < 8; i++)
	{
		m_iSuperCardCount[i] = 0;
		::memset(m_bSuperCardList[i],0,sizeof(m_bSuperCardList[i]));
	}
	m_bSuperStation = -1;
#endif
	return TRUE;
}

//��Ϸ��ʼ
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag)==false) 
	{
		GameFinish(0,GF_SALE);
		return false;
	}
	//����״̬
	//	m_bQueryQuit=false;							//����������뿪
	m_iSendCardPos=0;							//����λ��
	m_iBeenPlayGame++;							//�Ѿ���ľ���
//	m_iTotalNote = 0;	
	//������ע��
	//	m_iCurPlayStatus = -1;
	m_iGameStation = -1;
	m_iIngPlayNote = -1;
	m_iLastShuttle = -1;
	m_bGameStation = GS_SEND_CARD;				//��Ϸ״̬
	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));		//�û���������
	for (int i = 0; i < m_bMaxPeople; i++) 
	{
		m_iUserStation[i] = -1;
		m_iPerJuTotalNote[i] = 0;
		if (m_pUserInfo[i])
		{
			m_iUserStation[i] = TYPE_PLAY_GAME;
		}
//		m_iInsuranceNote[i] = 0;
	}
	//�ַ��˿�
	m_Logic.RandCard(m_iTotalCard,m_iAllCardCount,m_bHaveKing);
	int temp = rand()%100;
	CString str;
	str.Format("zhtlog: temp=%d m_iGoodCard=%d",temp,m_iGoodCard);
	OutputDebugString(str);
	//ϴ������
	if (temp < m_iGoodCard)
	{
		for (int i=0; i<20; i++)
		{

			if (GetMaxCardShape(m_iTotalCard,m_iAllCardCount) > UG_BULL_SEVEN)
			{
				OutputDebugString("zhtlog:ϴ������");
				break;
			}
			else
			{
				OutputDebugString("zhtlog:ûϴ������");
			}
			m_Logic.RandCard(m_iTotalCard,m_iAllCardCount,m_bHaveKing);
		}
	}
	//����˷��Ʋ���
	//	SuperSetCard(m_iTotalCard,ALL_CARD_COUNT);
	//���Ϳ�ʼ��Ϣ
	//if (!FindNt())
	//{
	//	GameFinish(0,GF_SALE);
	//	for (int i=0;i<m_bMaxPeople;i++) 
	//	{
	//		
	//		if(m_pUserInfo[i])
	//			SendGameData(i,MDM_GM_GAME_NOTIFY,ASS_NOMONEY,0);
	//		else
	//			continue;
	//	}
	//	SendWatchData(m_bMaxPeople,MDM_GM_GAME_NOTIFY,ASS_NOMONEY,0);
	//	return TRUE;
	//}
	srand(GetTickCount());
	//m_iFirstJiao= rand()%PLAY_COUNT;
#ifdef SUPER_VERSION
	for(int j = 0; j < PLAY_COUNT; j++)
	{
		for(int i = 0; i < m_iSuperCardCount[j]; i ++)
		{
			//����Ӧһ����
			int temp = m_Logic.GetSerialBySpecifyCard(m_iTotalCard,0,m_iAllCardCount,m_bSuperCardList[j][i]);
			if(temp != -1)
			{	
				BYTE t = m_iTotalCard[i + j * 5];
				m_iTotalCard[i + j * 5] = m_iTotalCard[temp];
				m_iTotalCard[temp] = t;

			}
		}
	}
#endif
	m_bGameStation = GS_SEND_CARD;

	if (m_iFirstJiao != 255)
	{
		m_iFirstJiao = GetNextDeskStation(m_iFirstJiao);
		//OutputDebugString("zhtlog:");
	}
	else
	{
		m_iFirstJiao = GetNextDeskStation(rand()%PLAY_COUNT);
		//OutputDebugString("zhtlogelse:");
	}
	

	/*CString str;
	str.Format("zhtlog:m_iFirstJiao=%d",m_iFirstJiao);
	OutputDebugString(str);*/
	SendCallScore(m_iFirstJiao);
	//�����һ����ׯ����
	/*for (int i = 0; i<PLAY_COUNT; i++)
	{
		m_iFirstJiao = m_iFirstJiao%PLAY_COUNT;
		if(!m_pUserInfo[m_iFirstJiao])
		{
			m_iFirstJiao++;
		}
		else
		{
			SendCallScore(m_iFirstJiao);

			return true;
		}
	}*/

	return true;
}
//�û���ע
BOOL CServerGameDesk::UserNote()
{
	m_iGameFlag = GS_NOTE;
	m_iUpGradePeople = m_iPrepareNT;
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if (i==m_iUpGradePeople || !m_pUserInfo[i])
			continue;
		m_iLimitNote[i] = GetLimit(i);
	}

	BeginUpgradeStruct BeginMessage;
	memset(&BeginMessage,0,sizeof(BeginMessage));
	BeginMessage.AllCardCount=m_iAllCardCount;
	::CopyMemory(BeginMessage.iLimitNote,m_iLimitNote,sizeof(m_iLimitNote));
	BeginMessage.iBaseNote=m_iBaseNote;//PLAY_BASE_NOTE;			//��ע���ע�İٷ�֮һ
	::CopyMemory (BeginMessage.SysCard,m_iTotalCard,sizeof(BYTE)*m_iAllCardCount);
	m_MaxPlayer = CountNotePeople() - 1;//�мҵ�����Ϊ����δ������ҵ�������ȥһ��ׯ��
	BeginMessage.iNt = m_iUpGradePeople;	 

	for (int i=0;i<m_bMaxPeople;i++) 
	{
		if(m_pUserInfo[i]&&m_iUserStation[i]!=TYPE_GIVE_UP)
			SendGameData(i,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_BEGIN_UPGRADE,0);
		else
			continue;
	}
	SendWatchData(m_bMaxPeople,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_BEGIN_UPGRADE,0);

	SetTimer(TIME_NOTE,(m_iXiaZhuTime+2)*1000);
	m_iSendPos=0;
	return TRUE;
}

BOOL CServerGameDesk::SuperSetCard()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");
	CString key = TEXT("Super");
	int count = f.GetKeyVal(key,"count",0);
	if(count <= 0 )					//��ʤ��
		return FALSE;
	TCHAR szName[21];
	BYTE  bFind = 255;				//ʤ������
	int probability = 0;
	CString szNickName;
	memset(m_bIsSuper,0,sizeof(m_bIsSuper));
	for (int j = 0; j < PLAY_COUNT; j++)
	{
		if(m_pUserInfo[j] == NULL)
			continue;

		for(int i = 0; i < count; i ++)
		{
			wsprintf(szName,"nickname%d",i);
			szNickName = f.GetKeyVal(key,szName,"wing");

			if(strcmp(m_pUserInfo[j]->m_UserData.nickName,szNickName) != 0)
				continue;
		
			m_bIsSuper[j] = TRUE;
			break;
		}

	}
	return true;
}

//����÷�
int	CServerGameDesk::GetUpGrade(int iGamePoint, int iPlayCardCount)
{
	/**********************************************
	Ӯ�ҵľ����֣�������������µ���ע�����������
	������
	**********************************************/

	/*	int fen[4] = {0,0,0,0},total = 0;

	int iWiner = GetCardGreater();
	if (iWiner >= 0) 
	fen[iWiner] = PLAY_MAX_NOTE;

	for (int i = 0;i < 4;i++)
	{
	total += m_iPerJuTotalNote[i];
	if (i == iWiner) continue;
	if (m_iUserStation[i] == BUTTON_TYPE_GIVEUP)
	{
	if (m_iPerJuTotalNote[i] > PLAY_MAX_NOTE) fen[i] = -(PLAY_MAX_NOTE);
	else fen[i] = -(m_iPerJuTotalNote[i]);
	continue;
	}
	fen[i] = -m_iPerJuTotalNote[i];//-(PLAY_MAX_NOTE);
	}
	fen[iWiner] = total;

	for (int i = 0;i < 4;i++)
	{
	int flag = 1;
	long ul = fen[i];
	if (ul < 0)
	{
	ul = -ul;
	flag = -1;
	}
	ul = ul / 100;
	m_iTurePoint[i] = flag * ul;
	}*/
	return 0;
}

//�û�������Ϸ
BOOL CServerGameDesk::UserSetGame(UserArgeeGame * pGameSet)
{
	/*	//Ч������
	if (m_bGameStation!=GS_WAIT_SETGAME) return FALSE;

	//Ч������
	if (((pGameSet->iPlayCount<=0)||(pGameSet->iPlayCount>99))||
	(pGameSet->iBeginStation<2)||(pGameSet->iEndStation>14)||
	(pGameSet->iBeginStation>pGameSet->iEndStation)) 
	return FALSE;
	if ((m_CardCount & pGameSet->iCardCount)==0) return FALSE;

	//��������
	m_iThinkTime=pGameSet->iPlayCount;
	m_iPlayCardCount=(pGameSet->iCardCount&1)?2:3;
	m_iBeginStation=pGameSet->iBeginStation;
	m_iEndStation=pGameSet->iEndStation;
	m_iBeenPlayGame=0;


	m_bGameStation=GS_WAIT_ARGEE;

	//������Ϣ
	UserArgeeGame Agree;
	Agree.iBeginStation=m_iBeginStation;
	Agree.iEndStation=m_iEndStation;
	Agree.iPlayCount=m_iThinkTime;
	Agree.iCardCount=m_iPlayCardCount;
	for (int i=0;i<4;i++) SendGameData(i,&Agree,sizeof(Agree),MDM_GM_GAME_NOTIFY,ASS_UG_USER_SET,0);
	SendWatchData(m_bMaxPeople,&Agree,sizeof(Agree),MDM_GM_GAME_NOTIFY,ASS_UG_USER_SET,0);
	*/
	return TRUE;
}

//�����˿˸��û�
BOOL CServerGameDesk::SendCard()
{
	KillTimer(TIME_SEND_CARD);

	int QuitCardCount = 0;
	for (int i=0; i < PLAY_COUNT; i++)
	{
		if(!m_pUserInfo[i] && m_iUserCardCount[i] > 0)
			QuitCardCount += m_iUserCardCount[i];
	}
	if (/*m_iSendCardPos == CountNotePeople()*5 + QuitCardCount*/m_iSendCardPos>0)//����λ�õ������һ����ҷ���2��ʱ��������
	{
		KillTimer(TIME_SEND_CARD);

		SendCardFinish();
		return TRUE;
	}
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if( m_iUserStation[i] == TYPE_GIVE_UP)
			continue;
		if(m_pUserInfo[i] && CanSendCard(i) )//����û��ǿ�,��������������
		{
			m_iSendPos = i;
			break;
		}
	}
	//
	SendAllCardStruct  pAllCard;
	pAllCard.iStartPos = m_iUpGradePeople;

	for(int j = 0 ; j<5 ;j++)
	{
		for(int i = 0 ;i<PLAY_COUNT;i++)
		{
			if( NULL == m_pUserInfo[i] || m_iUserStation[i] == -1)
			{
				m_iUserCardCount[i] = 0;
				continue;
			}
			else
			{
				m_iUserCard[i][j] =  m_iTotalCard[i*5+j];	
				
			}			
		}
	}
	for(int i = 0 ;i<PLAY_COUNT;i++)
	{
		if(NULL == m_pUserInfo[i]|| m_iUserStation[i] == -1)
		{
			m_iUserCardCount[i] = 0;
			pAllCard.iUserCardCount[i] = 0;
			continue;
		}
		else
		{
			m_iUserCardCount[i] = 5;
			memcpy(&pAllCard.iUserCard[i],&m_iUserCard[i],sizeof(BYTE)*m_iUserCardCount[i]);			
			pAllCard.iUserCardCount[i] = m_iUserCardCount[i] ;
		}
	}	
	for(int j=0;j<m_bMaxPeople;j++)
	{
		if(m_pUserInfo[j])
			SendGameData(j,&pAllCard,sizeof(pAllCard),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);
		
	}	
	SendWatchData(m_bMaxPeople,&pAllCard,sizeof(pAllCard),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);

	///����ʱ��д������ͻ���һ��150*5
	SetTimer(TIME_SEND_CARD,/*6*m_iSendCardTime*/CountNotePeople()*750+500);

	m_iSendCardPos++;



	for(int i = 0 ; i<PLAY_COUNT;i++)
	{
		for(int j = 0 ;j<5 ;j++)
		{
			CString str;
			str.Format("wys423sever::m_iUserCard[%d][%d]=%d",i,j,m_iUserCard[i][j]);
			OutputDebugString(str);

		}
	}

	return TRUE;

 //   SendCardStruct SendCard;
	//SendCard.bDeskStatin=m_iSendPos;//m_iSendCardPos%m_bMaxPeople; 
	////�ĳ������ƶ����úõ�
	//BYTE bCard = m_iTotalCard[m_iSendPos * 5 + m_iUserCardCount[ m_iSendPos ]];
	//m_iUserCard[m_iSendPos][m_iUserCardCount[m_iSendPos]++] = bCard;
	//SendCard.iCard = bCard;//m_iUserCard[m_iSendCardPos%4][m_iSendCardPos/m_bMaxPeople];

	//for(int j=0;j<m_bMaxPeople;j++)
	//{
	//	if(m_pUserInfo[j])
	//		SendGameData(j,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);
	//}
	//SendWatchData(m_bMaxPeople,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);
	//m_iSendCardPos++;
	//return TRUE;
}

//���ƽ���
BOOL CServerGameDesk::SendCardFinish()
{
	for (int i=0;i<m_bMaxPeople;i++) 
	{
		if(m_pUserInfo[i])
			SendGameData(i,MDM_GM_GAME_NOTIFY,ASS_SEND_FINISH,0);
	}
	SendWatchData(m_bMaxPeople,MDM_GM_GAME_NOTIFY,ASS_SEND_FINISH,0);

	//	SetTimer(TIME_GAME_FINISH,2000);

	BeginPlayUpGrade();
	return true;
}

//�Ƿ����ӷ���
BOOL CServerGameDesk::CanSendCard(BYTE bDeskStation)
{
	int min=99;
	for(int i=0;i<m_bMaxPeople;i++)
	{
		if(m_iUserCardCount[i]<min&&m_pUserInfo[i]&&m_iUserStation[i] != TYPE_GIVE_UP&&m_iUserStation[i] != -1)
			min=m_iUserCardCount[i];
	}
	if(m_iUserCardCount[bDeskStation]==min)
		return true;
	return false;
}

//ͳ������Ϸ���
BYTE CServerGameDesk::CountPlayer()
{
	BYTE count=0;
	for(int i=0;i<m_bMaxPeople;i++)
	{
		if(m_pUserInfo[i] && m_iUserStation[i] != -1)
			count++;
	}
	return count;
}

//��Ϸ��ʼ
BOOL CServerGameDesk::BeginPlayUpGrade()
{
	//��������
	m_bGameStation=GS_PLAY_GAME;

	//	m_iOutCardPeople=(m_iUpGradePeople + 1)%PLAY_COUNT;
	for (int i = m_iOutCardPeople;;i=(i+1) % PLAY_COUNT)
	{
		if(!m_pUserInfo[i] || m_iUserStation[i] == TYPE_GIVE_UP || i == m_iUpGradePeople)
			continue;
		else
		{
			m_iOutCardPeople = i;
			break;
		}
	}
	//������Ϸ��ʼ��Ϣ
	BeginPlayStruct Begin;
	Begin.iOutDeskStation=m_iOutCardPeople;
	for (int i=0;i<m_bMaxPeople;i++)
		SendGameData(i,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
	SendWatchData(m_bMaxPeople,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);

	// ����ʱ��ţ
	SetTimer(TIME_CUTLINE_AUTO, (m_iThinkTime+2)*1000);
	return TRUE;
}
//�û���ע
BOOL CServerGameDesk::UserNoteResult(BYTE bDeskStation, BYTE iVerbType,__int64 iNoteVal)
{
	if (m_iPerJuTotalNote[bDeskStation]!=0 || m_iUserStation[bDeskStation] == TYPE_GIVE_UP)
		return true;
	if (m_iLimitNote[bDeskStation] < iNoteVal || 0 > iNoteVal)//2011-03-03 ���iNoteVal < 0�жϣ���ֹ����޸����ݰ��¸���
	{
		return true;
	}
	m_iPerJuTotalNote[bDeskStation] = iNoteVal;
	m_iUserStation[bDeskStation] = iVerbType;
	//m_iGameStation = iVerbType;

	CString aa;
	aa.Format("Test: Server �û�״̬ %d-��%d", bDeskStation, m_iUserStation[bDeskStation]);
	OutputDebugString(aa);

	m_MaxPlayer--;
	if (CountNotePeople() == 1)
	{
		GameFinish(0,GF_SALE);
		return TRUE;
	}
	NoteResult noteresult;					
	noteresult.bAddStyle=iVerbType;										//��ע����
	noteresult.iOutPeople = bDeskStation;								//������ע��
	noteresult.iCurNote =  iNoteVal;
	for (int i = 0; i < m_bMaxPeople; i++)
	{
		if(m_pUserInfo[i])
		{
			SendGameData(i,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_NOTE_RESULT,0);
		}		
	}
	SendWatchData(m_bMaxPeople,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_NOTE_RESULT,0);

	if (m_MaxPlayer <= 0)
		//������Ϣ
	{
		m_iGameFlag = GS_SEND_CARD;
		KillTimer(TIME_NOTE);
		if (!m_bSuperSet)//û�г������õ�����²Ŵ��������Ӯ
		{
			JudgeLoser();
			JudgeWiner();
		}
		

		if (m_bControlWin != 255)
		{
			BYTE bDest = GetIndex(1);
			
			if (bDest != m_bControlWin)
				ChangeCard(m_bControlWin,bDest);

		}
		if (m_bControlLost != 255)
		{

			BYTE bDest = GetIndex(0);
			if (bDest != m_bControlLost)
				ChangeCard(m_bControlLost,bDest);
		}
		m_iSendCardPos = 0;
		SetTimer(TIME_SEND_CARD,m_iSendCardTime);
		return TRUE;

	}
	if (CheckFinish())
	{
		return TRUE;
	}
	return TRUE;

}

//�����û����
BOOL CServerGameDesk::UserActiveResult(BYTE bDeskStation, BYTE iVerbType,int iNoteVal)
{
	CString aa;
	aa.Format("Test: Server UserActiveResult %d %d %d", bDeskStation, iVerbType, iNoteVal);
	OutputDebugString(aa);

	//if (bDeskStation != m_iOutCardPeople)
	//	return true;
	//BYTE TempStation;//ͣ�Ƶ���ʱλ��
	switch (iVerbType)
	{
		//����
	case TYPE_GIVE_UP:
		{
			break;
		}
		//�ӱ�
	case TYPE_BULL:
		{

			break;
		}
		//����
	case TYPE_OPEN:
		{
			m_iGameStation = TYPE_OPEN;
			m_iUserStation[bDeskStation] = TYPE_OPEN;
			if (CheckFinish())
			{
				return TRUE;
			}

		}
	}
	NoteResult noteresult;					
	noteresult.bAddStyle=iVerbType;										//��ע����
	noteresult.iOutPeople = bDeskStation;								//������ע��
	//	noteresult.NoHitStation = TempStation;
	noteresult.iCurNote = m_iPerJuTotalNote[bDeskStation] / 2;
	for (int i = 0; i < m_bMaxPeople; i++)
	{
		if(m_pUserInfo[i])
			SendGameData(i,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_NOTE_RESULT,0);
	}
	SendWatchData(m_bMaxPeople,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_NOTE_RESULT,0);
	if (CheckFinish())															//�Ƿ����
		return TRUE;
	////��һλҪ����//��ǰ��Ҳ�Ҫ�Ƶ�����²ż�����һҪ����
	//if ((!m_bIsHit[m_iOutCardPeople][0] && !m_bIsHit[m_iOutCardPeople][1]) || (!m_bIsHit[m_iOutCardPeople][0] && !m_bSplit[bDeskStation]))
	//{
	//	m_iOutCardPeople=(m_iOutCardPeople + 1)%PLAY_COUNT;
	//	for (int i = m_iOutCardPeople;;i=(i+1) % PLAY_COUNT)
	//	{
	//		if(!m_pUserInfo[i] || m_iUserStation[i] == TYPE_GIVE_UP /*|| i == m_iUpGradePeople*/)
	//			continue;
	//		else
	//		{
	//			m_iOutCardPeople = i;
	//			break;
	//		}
	//	}
	//}
	//tagUserResult result;
	//result.iOutPeople = m_iOutCardPeople;
	//result.bCountNotePeople = CountNotePeople();
	//result.bSplit = m_bSplit[m_iOutCardPeople];
	//result.iSplitStation = (m_bIsHit[m_iOutCardPeople][0]?0:1);

	//for (int i = 0;i < PLAY_COUNT;i++)
	//{
	//	if(m_pUserInfo[i])
	//	{
	//		SendGameData(i,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);
	//	}
	//}
	//SendWatchData(m_bMaxPeople,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);

	return TRUE;
}


BYTE CServerGameDesk::CountNotePeople()
{
	int iCount=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(!m_pUserInfo[i])		
			continue;
		if(m_iUserStation[i]!=TYPE_GIVE_UP && m_iUserStation[i]!=-1)
			iCount++;
	}
	return iCount;
}
//����Ƿ����
BOOL CServerGameDesk::CheckFinish()
{
	//����Ƿ����
	/*******************
	����������
	*******************/
	//	bool bFinish = false;

	int iShuttle = 0,iGiveUp = 0;
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if(!m_pUserInfo[i])
			continue;

		if (m_iUserStation[i] == TYPE_GIVE_UP)
		{
			iGiveUp++;
			continue;
		}
		if (m_iUserStation[i] == TYPE_OPEN)
		{
			iShuttle++;
			continue;
		}
	}
	//	//	if (((iShuttle + iGiveUp) == CountPlayer())&&!bFinish || (iGiveUp ==  CountPlayer()-1))			//�����ҷ���,�������Ѿ������ҷ����ĺ����𻨵����Ϊ��Ϸ����
	if((iGiveUp  ==  CountPlayer()-1)||(iShuttle + iGiveUp) == CountPlayer())			//�����ҷ���,�������Ѿ������ҷ����ĺ����𻨵����Ϊ��Ϸ����
	{
		//����
		SetTimer(TIME_GAME_FINISH,2000);
		return true;
	}
	if (iShuttle == CountNotePeople())
	{
		//����
		SetTimer(TIME_GAME_FINISH,2000);
		return true;
	}

	return FALSE;
}
BOOL CServerGameDesk::CollectJeton(BOOL bOnly)
{
	KillTimer(TIME_COLLECT_JETON);
	CollectJetonStruct jeton;
	for (int i = 0;i < PLAY_COUNT;i++) 
		SendGameData(i,&jeton,sizeof(jeton),MDM_GM_GAME_NOTIFY,ASS_COLLECT_JETON,0);
	SendWatchData(m_bMaxPeople,&jeton,sizeof(jeton),MDM_GM_GAME_NOTIFY,ASS_COLLECT_JETON,0);
	if(!bOnly)
		NewPlayTurn(m_iOutCardPeople);
	return TRUE;
}

//��һ�ֿ�ʼ
BOOL CServerGameDesk::NewPlayTurn(BYTE bDeskStation)
{/*
 //	if (m_iNowBigPeople < 0)//new turn
 //	{
 m_bAddNoteTime++;
 m_iNowBigPeople=-1;

 //ÿ�˷�һ����
 bool bIs5Card = false;
 for (int i = 0;i < PLAY_COUNT;i++)
 {
 if(!m_pUserInfo[i])
 continue;
 if (m_iUserCardCount[i] != 5)
 continue;
 bIs5Card = true;
 }
 if (bIs5Card) 
 {
 SetTimer(TIME_GAME_FINISH,2000);
 return TRUE;
 }
 m_iOutCardPeople = GetCardGreater(false);//iGreater;
 //��ʼ����λ��
 m_iSendPos=0;
 SetTimer(TIME_SEND_A_CARD,SEND_CARD_TIME);

 //		SendACard(m_iOutCardPeople);	
 //�����־�ȫ�����,�����һ���ƾͱȽϴ�С
 //	}*/
	//if(m_Ming[m_iNowBigPeople])
	//	m_iMaxNote+=m_iUserNote[m_iNowBigPeople];
	//else
	//	m_iMaxNote+=m_iUserNote[m_iNowBigPeople]*2;
	//m_iNowBigPeople=-1;
	//m_iOutCardPeople=-1;
	//for(int i=0;i<PLAY_COUNT;i++)
	//	m_iUserNote[i]=0;
	SendUserNote();
	return TRUE;
}

BOOL CServerGameDesk::SendACard()
{
	if(m_iSendPos>=PLAY_COUNT)//δ������ÿ�˷���һ����
	{
		KillTimer(TIME_SEND_A_CARD);
		SendUserNote();
		return true;
	}
	//��һ����
	SendCardStruct SendCard;
	int pos=m_iOutCardPeople;		//��ʼ����λ��

	m_iUserCard[pos][m_iUserCardCount[pos]] = m_iTotalCard[m_iSendCardPos];
	SendCard.iCard = m_iUserCard[pos][m_iUserCardCount[pos]];
	m_iUserCardCount[pos]++;
	SendCard.bIsSplit = false;

	SendCard.bDeskStatin=pos;												//������λ��
	for (int i = 0;i < PLAY_COUNT;i++) 
		SendGameData(i,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_SEND_A_CARD,0);
	SendWatchData(m_bMaxPeople,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_SEND_A_CARD,0);

	m_iSendCardPos++;
	KillTimer(TIME_SEND_A_CARD);
	//SendUserNote();

	return TRUE;
}

BOOL CServerGameDesk::SendUserNote()
{
	//����Ƿ���Խ���(����һ�ֶ������������½��з���)����Ϸ�������ʡ
	//		if(CheckFinish())
	//		{
	//			return true;
	//		}
	//��ע��
	tagUserResult result;
	result.iOutPeople = m_iOutCardPeople;
	result.bCountNotePeople=CountNotePeople();
	::CopyMemory(result.iUserStation,&m_iUserStation,sizeof(m_iUserStation));
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i])
			SendGameData(i,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);
	}
	SendWatchData(m_bMaxPeople,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);

	return TRUE;
}

//�����û�����ע
void CServerGameDesk::ModifyPlayNote(BYTE bDeskStation)
{
	/*	m_iIngPlayNote = bDeskStation;

	tagUserResult result;
	result.iOutPeople = bDeskStation;
	result.iTotalNote = m_iTotalNote;
	CopyMemory(result.iCurNote,m_iUserNote,sizeof(m_iUserNote));

	for (int i = 0;i < 4;i++)
	SendGameData(i,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_MODIFY_PLAY_NOTE,0);
	SendWatchData(m_bMaxPeople,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_MODIFY_PLAY_NOTE,0);
	*/	return;
}

//��ȡ�������
int CServerGameDesk::GetCardGreater(BOOL bExtVal)
{
	int iGreater =m_iOutCardPeople;// -1;
	//    if(m_iUserStation[iGreater]==TYPE_GIVE_UP)
	//         iGreater=-1;
	//for (int i = 0;i < m_bMaxPeople;i++)
	//{	
	//	if (m_iUserStation[i] == TYPE_GIVE_UP||!m_pUserInfo[i]) 
	//		continue;
	//	if (iGreater < 0)
	//	{
	//		iGreater = i; //i;
	//		continue;
	//	}
	//	if(i == m_iOutCardPeople)
	//		continue;
	//	int ret; 		
	//	if(!bExtVal)		//�Ƚ�����
	//		ret = m_Logic.CompareCard(m_iUserCard[iGreater]+1,m_iUserCardCount[iGreater]-1,m_iUserCard[i]+1,m_iUserCardCount[i]-1);
	//	else				//�Ƚ�������
	//		ret= m_Logic.CompareCard(m_iUserCard[iGreater],m_iUserCardCount[iGreater],m_iUserCard[i],m_iUserCardCount[i]);
	//	if (ret > 0)
	//		continue;
	//	else iGreater = i;
	//}
	return iGreater;
}

//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	//�õ���С���Ľ��
	long m_dwMaxPoint=0;
	bool bSetMaxMoney=false;
	KillTimer(TIME_CUTLINE_AUTO);
	KillAllTimer();


	//��������쳣��ֱ���㰲ȫ����
	if (CountPlayer() <= 1)
	{
		bCloseFlag = GF_SALE;
	}
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			//�������� 
			m_bGameStation=GS_WAIT_NEXT;

			for (int i=0;i<PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i]!=NULL) 
					m_pUserInfo[i]->m_UserData.bUserState=USER_SITTING;
			}	

			//��Ϸ����
			GameEndStruct GameEnd;
			::memset(&GameEnd,0,sizeof(GameEnd));
			GameEnd.iUpGradeStation=m_iUpGradePeople;
			::CopyMemory(GameEnd.iUpBullCard,&m_bBullCard,sizeof(m_bBullCard));
			::CopyMemory(GameEnd.iCardList,&m_iUserCard,sizeof(m_iUserCard));
			//�����ͻ��˵�ǰ����
			for (int i = 0;i < PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i]==NULL) 
					continue;
				//���ʹ���
				if(m_iUserStation[i]!=TYPE_GIVE_UP)
				{
					GameEnd.iUserState[i] = m_iUserStation[i];
					GameEnd.iCardShape[i] = m_Logic.GetShape(m_iUserCard[i],m_iUserCardCount[i],m_bBullCard[i]);
				}
			}
			__int64 iNtPoint = 0; //ׯ�����Ǯ������Ӯ�����мң�
			double  iNtLoseMoney = 0;//ׯ�����Ǯ��������Ӯ�����мҵģ�
			for (int i = 0;i < PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i]==NULL) 
					continue;
				if(i != m_iUpGradePeople)//�õ����мҵĵ÷ֵ�
				{
					
					GameEnd.iTurePoint[i] = ComputePoint(i)*m_iPerJuTotalNote[i];
					
					//20100802 zht ͬϲ�ֿͻ��޸�����Ӯ�����мҲ���Ǯ
					if (GameEnd.iTurePoint[i] + m_pUserInfo[i]->m_UserData.i64Money < 0)
					{
                         GameEnd.iTurePoint[i] = -m_pUserInfo[i]->m_UserData.i64Money;    
					}
					if (GameEnd.iTurePoint[i] > 0)
					{
						iNtLoseMoney+=GameEnd.iTurePoint[i];
					}
					iNtPoint += GameEnd.iTurePoint[i];
					//CString str;
					//str.Format("zhtout:iNtPoint=%d iNtLoseMoney=%lf GameEnd.iTurePoint[%d]=%d",iNtPoint,iNtLoseMoney,i,GameEnd.iTurePoint[i]);
					//OutputDebugString(str);
					//GameEnd.iTurePoint[m_iUpGradePeople] -= 
				}
			}
			//20100802 zht ͬϲ�ֿͻ��޸�����Ӯ����
			if (m_pUserInfo[m_iUpGradePeople] != NULL)
			{

				__int64 dwNtMoney = m_pUserInfo[m_iUpGradePeople]->m_UserData.i64Money;
				if (iNtPoint > dwNtMoney)//����Ǯ��
				{

					for (int i = 0;i < PLAY_COUNT;i++)
					{
						if (m_pUserInfo[i]==NULL) 
							continue;
						if(i != m_iUpGradePeople && GameEnd.iTurePoint[i] > 0)//�õ����мҵĵ÷ֵ�
						{
							GameEnd.iTurePoint[i] = (dwNtMoney+(iNtLoseMoney-iNtPoint)) *(GameEnd.iTurePoint[i]/iNtLoseMoney);
						}
					}
					GameEnd.iTurePoint[m_iUpGradePeople] = -m_pUserInfo[m_iUpGradePeople]->m_UserData.i64Money; 

				}
				else
				{
					GameEnd.iTurePoint[m_iUpGradePeople] = -iNtPoint;
				}
			}

			//д�����ݿ�				
			int temp_point[PLAY_COUNT];
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			for (int i=0;i<PLAY_COUNT;i++)
			{
				temp_point[i]=GameEnd.iTurePoint[i];//+m_iAwardPoint[i];
				GameEnd.iChangeMoney[i]=GameEnd.iTurePoint[i];
				CString str;
			
			}
			
			ChangeUserPointint64(/*temp_point*/GameEnd.iTurePoint, temp_cut);
			__super::RecoderGameInfo(GameEnd.iChangeMoney);

			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0; /// ��ҳ����ۻ���    add by wys 2010-11-1
			if (true == bNotCostPoint)
			{
				memset(GameEnd.iTurePoint,0,sizeof(GameEnd.iTurePoint));
			}
			//��������

			for (int i=0; i < PLAY_COUNT; i++) 
			{
				if (m_pUserInfo[i]!=NULL) 
					SendGameData(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
			}
			SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);

			ClearQuitPlayer();
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GF_SALE:			//��Ϸ��ȫ����
	case GFF_SAFE_FINISH:
		{
			m_bGameStation = GS_WAIT_ARGEE;//GS_WAIT_SETGAME;
			for (int i = 0; i < m_bMaxPeople; i++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}
			GameCutStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));
			CutEnd.bDeskStation=bDeskStation;
			for (int i = 0; i < m_bMaxPeople; i++)
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);
			bCloseFlag = GFF_SAFE_FINISH;

			ClearQuitPlayer();
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;

		}
	case GFF_FORCE_FINISH:		//�û������뿪
		{
			//��������
			m_bGameStation=GS_WAIT_ARGEE;//GS_WAIT_SETGAME;

			for (int i=0;i<m_bMaxPeople;i++)
			{
				if (m_pUserInfo[i]!=NULL) 
					m_pUserInfo[i]->m_UserData.bUserState=USER_SITTING;
			}
			GameCutStruct CutEnd;
			memset(&CutEnd,0,sizeof(CutEnd));
			CutEnd.bDeskStation=bDeskStation;
			int total = 0;
			for (int i=0; i<PLAY_COUNT; i++)
			{
				total+=m_iPerJuTotalNote[i];
			}

			if (bDeskStation == m_iUpGradePeople)
			{
				CutEnd.iTurePoint[bDeskStation]= -total*5;;
			}
			else
			{
				CutEnd.iTurePoint[bDeskStation]= -m_iPerJuTotalNote[bDeskStation]*5;;//m_iTotalNote;			//�۳�ǿ�����ȫ����ע
			}
			//CutEnd.iTurePoint[(bDeskStation+1)%PLAY_COUNT]=(m_iTotalNote-m_iBaseNote)/(CountPlayer()-1);
			//CutEnd.iTurePoint[(bDeskStation+2)%PLAY_COUNT]=(m_iTotalNote-m_iBaseNote)/(CountPlayer()-1);
			//CutEnd.iTurePoint[(bDeskStation+3)%PLAY_COUNT]=(m_iTotalNote-m_iBaseNote)/(CountPlayer()-1);
			////CutEnd.iTurePoint[(bDeskStation+4)%PLAY_COUNT]=(m_iTotalNote-m_iBaseNote)/(CountPlayer()-1);
			//CutEnd.iTurePoint[(bDeskStation+5)%PLAY_COUNT]=(m_iTotalNote-m_iBaseNote)/(CountPlayer()-1);

			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			int i=0;
			for (i=0; i<PLAY_COUNT; ++i) 
			{ 
				temp_cut[i] = (bDeskStation == i)?true:false; 
			} 


			ChangeUserPointint64(CutEnd.iTurePoint, temp_cut);
			//__super::RecoderGameInfo(CutEnd.iChangeMoney);

			__super::RecoderGameInfo(CutEnd.iChangeMoney);
			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0; /// ��ҳ����ۻ���    add by wys 2010-11-1
			if (true == bNotCostPoint)
			{
				memset(CutEnd.iTurePoint,0,sizeof(CutEnd.iTurePoint));
			}

			for (int i=0;i<m_bMaxPeople;i++)
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);

			bCloseFlag=GFF_FORCE_FINISH;

			ClearQuitPlayer();
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

bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo)
{
	UserAI(bDeskStation, 1, true);
	return __super:: UserNetCut(bDeskStation, pLostUserInfo);
}

//�û���������
bool CServerGameDesk::UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo)
{
	UserAI(bDeskStation, 1, false);
	return __super:: UserReCome(bDeskStation, pNewUserInfo);
}

//���AI����
BOOL CServerGameDesk::UserAI(BYTE bDeskStation,BYTE bCutType,bool bNet)
{
	m_bIsCut[bDeskStation] = bNet;

	if(CountCutPlayer() == CountPlayer())	
	{
		GameFinish(0,GF_SALE);

		return FALSE;
	}
	if(1 == CountPlayer())	//��1��
	{
		GameFinish(0,GF_SALE);

		return FALSE;
	}
	return TRUE;
}
//ͳ������Ϸ���
BYTE CServerGameDesk::CountCutPlayer()
{
	BYTE count = 0;
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if(m_pUserInfo[i] == NULL)
			continue;

		if(m_bIsCut[i])
		{	
			count ++;
		}
	}
	return count;
}
//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	/*CString str;
	str.Format("zhtlog:bDeskStation=%d",bDeskStation);
	OutputDebugString(str);*/
	
	if (m_iUserStation[bDeskStation] == -1)
	{
		OutputDebugString("zhtlog:�ж��Ƿ�������Ϸ");
		return false;
	}
	if(m_bGameStation>=GS_SEND_CARD&&m_bGameStation<GS_WAIT_NEXT)
		return true;
	else
		return false;
	//return __super::IsPlayGame(bDeskStation);
}

//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	/*	if ((m_pUserInfo[bDeskStation]!=NULL)&&(m_bGameStation==GS_WAIT_ARGEE)&&(bDeskStation==0))
	{
	//��������
	m_bGameStation=GS_WAIT_SETGAME;
	for (int i=0;i<m_bMaxPeople;i++)
	{
	if (m_pUserInfo[i]!=NULL) m_pUserInfo[i]->m_UserData.bUserState=USER_SITTING;
	}
	}
	else if(m_bGameStation==GS_WAIT_SETGAME && (m_pUserInfo[bDeskStation]!=NULL) && m_iBeenPlayGame>0)
	{
	GameFinish(0,GF_SALE);
	}*/
	SuperSetCard();
	if (m_iUserStation[bDeskStation] == -1)
	{
		return __super::UserLeftDesk(bDeskStation,pUserInfo);
	}
	if(m_bGameStation>GS_WAIT_ARGEE)
	{
		GameFinish(0,GF_SALE);
	}
	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}
//�û������뿪
BOOL CServerGameDesk::UserHaveThing(BYTE bDeskStation, char * szMessage)
{
	if (m_bGameStation<GS_SEND_CARD)
		return TRUE;
	m_iHaveThingPeople=bDeskStation;
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

//ͬ���û��뿪
BOOL CServerGameDesk::ArgeeUserLeft(BYTE bDeskStation, BOOL bArgee)
{
	if (bArgee) m_iLeaveArgee|=1<<bDeskStation;
	else m_iLeaveArgee&=~1<<bDeskStation;
	if (m_iLeaveArgee!=15)
	{
		//		m_bQueryQuit=true;
		LeaveResultStruct Leave;
		Leave.bDeskStation=bDeskStation;
		Leave.bArgeeLeave=bArgee;
		for (int i=0;i<m_bMaxPeople;i++) 
			if (i!=bDeskStation) SendGameData(i,&Leave,sizeof(Leave),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	}
	else
	{
		GameFinish(bDeskStation,GF_SALE);
		//		GameFinish(m_iHaveThingPeople/*bDeskStation*/,GF_SALE);
		LeaveResultStruct Leave1;
		Leave1.bDeskStation=m_iHaveThingPeople;
		Leave1.bArgeeLeave=true;
		SendGameData(m_iHaveThingPeople,&Leave1,sizeof(Leave1),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	}
	return TRUE;
}

//�Q����
BOOL CServerGameDesk::ChangeBackCard(BYTE bDeskStation,BYTE bSourceCard,BYTE bResultCard)
{
	ChangeBackStruct back;

	if(ReplaceCard(m_iTotalCard,m_iSendCardPos,28,bSourceCard,bResultCard))
	{
		m_iUserCard[bDeskStation][0]=bResultCard;
		back.bExtVal=true;
	}else
		back.bExtVal=false;

	back.bDeskStation=bDeskStation;
	back.bSourceCard=bSourceCard;
	back.bResultCard=bResultCard;
	for(int i=0;i<PLAY_COUNT;i++)
		SendGameData(i,&back,sizeof(back),MDM_GM_GAME_NOTIFY,ASS_GM_SUPERCLIENT,0);
	return true;
}
//ϵ�y�������Ƿ���ĳ����
BOOL CServerGameDesk::ReplaceCard(BYTE iTotalCard[],int iSendCardPos,int iEndPos,BYTE bSourceCard,BYTE bResultCard)
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

//���ܿ۷�
int CServerGameDesk::GetRunPublish()
{
	return 15;//return m_pDataManage->m_InitData.uRunPublish;
}
//�Α���A����
int CServerGameDesk::GetRoomBasePoint()
{
	int iBasePoint = m_pDataManage->m_InitData.uBasePoint;//����
	return (iBasePoint>0?iBasePoint:1);
}

//���ӱ���
int CServerGameDesk::GetDeskBasePoint()
{
	return 1;//GetTableBasePoint();
}
//ͳ�Ʊ���
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
//��ׯ��
BOOL CServerGameDesk::FindNt()
{
	/*if (m_iUpGradePeople == -1)
	{

	for (int i = 0; i < PLAY_COUNT; i++)
	{
	if (m_pUserInfo[i] != NULL && m_pUserInfo[i]->m_UserData.dwMoney > m_iLimitNote * 20)
	{
	m_iUpGradePeople = i;
	return TRUE;
	}
	}
	return FALSE;


	}
	else 
	{
	m_iUpGradePeople = (m_iUpGradePeople + 1) % PLAY_COUNT;
	for (int i = m_iUpGradePeople;;i=(i+1) % PLAY_COUNT)
	{
	if(!m_pUserInfo[i])
	continue;
	if (m_pUserInfo[i] != NULL && m_pUserInfo[i]->m_UserData.dwMoney > m_iLimitNote * 20)
	{
	m_iUpGradePeople = i;
	return TRUE;
	}
	if (i == (m_iUpGradePeople + (PLAY_COUNT-1)) % PLAY_COUNT)
	return FALSE;
	}

	}*/
	return true;

}
//������ҷ���
int CServerGameDesk::ComputePoint(BYTE DeskStation)
{

	int Shape;
	int Point = 0;
	int winer = -1;
	if( m_Logic.CompareCard(m_iUserCard[DeskStation], m_iUserCardCount[DeskStation], m_bBullCard[DeskStation],
		m_iUserCard[m_iUpGradePeople],m_iUserCardCount[m_iUpGradePeople],m_bBullCard[m_iUpGradePeople]) == 1)
	{
		winer = DeskStation;
		Point = 1;
	}
	else
	{
		winer = m_iUpGradePeople;
		Point = -1;
	}
	Shape = m_Logic.GetShape(m_iUserCard[winer],m_iUserCardCount[winer],m_bBullCard[winer]);
	Point = Point * m_bCardShapeBase[Shape];
	//switch(Shape)
	//{
	//case UG_FIVE_SMALL://��С

	//	Point = Point * 8;
	//	break;
	//case UG_BULL_BOMB://ը��
	//	Point = Point * 8;
	//	break;
	//case UG_BULL_GOLD://��ţ
	//	Point = Point * 7;
	//	break;
	//case UG_BULL_SILVER://��ţ
	//	Point = Point * 6;
	//	break;
	//case UG_BULL_BULL:
	//	Point = Point * 5;
	//	break;
	//case UG_BULL_NINE:
	//	Point = Point * 4;
	//	break;
	//case UG_BULL_EIGHT:
	//	Point = Point * 3;
	//	break;
	//case UG_BULL_SEVEN:
	//	Point = Point * 2;
	//	break;
	//default:
	//	break;



	//}
	return Point;
}
BYTE CServerGameDesk::GetIndex(int win)
{
	BYTE Max = 0, Mix = 0;
	for (int i=1; i<PLAY_COUNT; i++)
	{
		if (m_Logic.CompareCard(&m_iTotalCard[Max*SH_USER_CARD],SH_USER_CARD,
			&m_iTotalCard[i*SH_USER_CARD],SH_USER_CARD)==-1)
			Max = i;
		if (m_Logic.CompareCard(&m_iTotalCard[Mix*SH_USER_CARD],SH_USER_CARD,
			&m_iTotalCard[i*SH_USER_CARD],SH_USER_CARD)==1)
			Mix = i;
	}
	if (win == 1)
		return Max;
	else 
		return Mix;

}
//�жϸ���
BOOL CServerGameDesk::JudgeLoser()
{
	try
	{
		//CString s = CINIFile::GetAppPath ();/////����·��
		//CINIFile f( s +"userlist.ini");
		CString s = CINIFile::GetAppPath ();/////����·��
		CINIFile f( s +SKIN_FOLDER +"_s.ini");
		CString key = TEXT("loser");
		int count = f.GetKeyVal(key,"count",0);
		if(count <= 0 )					//��ʤ��
			return FALSE;
		TCHAR szName[21];
		BYTE  bFind = 255;				//ʤ������
		int probability = 0;
		CString szNickName;
		for (int j = 0; j < PLAY_COUNT; j++)
		{
			if(m_pUserInfo[j] == NULL)
				continue;

			for(int i = 0; i < count; i ++)
			{
				wsprintf(szName,"nickname%d",i);
				szNickName = f.GetKeyVal(key,szName,"wing");

				if(strcmp(m_pUserInfo[j]->m_UserData.nickName,szNickName) != 0)
					continue;
				//����ʤ��(��ֻ����һ��)
				wsprintf(szName,"prob%d",i);
				probability = f.GetKeyVal(key,szName,0);
				bFind = j;
				break;
			}

		}
		//��ǰ������޸���
		if(bFind == 255)
			return FALSE;

		BYTE bDest = GetIndex(0);
		srand(GetTickCount()); 
		if((rand() % 100) + 1 <= probability)	//һ������
		{
			if(bFind != bDest)
				ChangeCard(bFind,bDest);
		}
		//else								//һ����ʤ ��ʤ������ʤ���ܿ���
		//{
		//	if(bFind != bDest)
		//		ChangeCard(bFind,bDest);
		//}
		return TRUE;
	}
	catch(...)
	{
		return TRUE;
	}
	return FALSE;
}

//�ж�ʤ��
BOOL CServerGameDesk::JudgeWiner()
{
	try
	{
		//CString s = CINIFile::GetAppPath ();/////����·��
		//CINIFile f( s +"userlist.ini");
		CString s = CINIFile::GetAppPath ();/////����·��
		CINIFile f( s +SKIN_FOLDER +"_s.ini");
		CString key = TEXT("winer");
		int count = f.GetKeyVal(key,"count",0);

		if(count <= 0 )					//��ʤ��
			return FALSE;

		TCHAR szName[21];
		BYTE  bFind = 255;				//ʤ������
		int probability = 0;
		CString szNickName;
		for (int j = 0; j < PLAY_COUNT; j++)
		{
			if(m_pUserInfo[j] == NULL)
				continue;
			for(int i = 0; i < count; i ++)
			{
				wsprintf(szName,"nickname%d",i);
				szNickName = f.GetKeyVal(key,szName,"wing");

				if(strcmp(m_pUserInfo[j]->m_UserData.nickName,szNickName) != 0)
					continue;
				//����ʤ��(��ֻ����һ��)
				wsprintf(szName,"prob%d",i);
				probability = f.GetKeyVal(key,szName,0);
				bFind = j;
				break;
			}
			if(bFind != 255)
				break;
		}

		//��ǰ�������ʤ��
		if(bFind == 255)
			return FALSE;
		BYTE bDest = GetIndex(1);
		srand(GetTickCount()); 
		if((rand() % 100) + 1 <= probability)//����ʤ��
		{
			if(bFind != bDest)
				ChangeCard(bFind,bDest);
		}
		//else	//һ������
		//{
		//	if(bFind == bDest)
		//		ChangeCard(bFind,(bDest + 1) % PLAY_COUNT);
		//}
		return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}
	return FALSE;
}
//����
BOOL CServerGameDesk::ChangeCard(BYTE bDestStation,BYTE bWinStation)
{
	for(int i = 0; i < SH_USER_CARD; i ++)
	{
		BYTE bTemp = m_iTotalCard[bDestStation * SH_USER_CARD + i];
		m_iTotalCard[bDestStation * SH_USER_CARD + i ] = m_iTotalCard[bWinStation * SH_USER_CARD + i];
		m_iTotalCard[bWinStation * SH_USER_CARD + i] = bTemp;
	}
	return TRUE;
}
//��ׯ
BOOL CServerGameDesk::UserCallScore(BYTE bDeskStation, int iVal)
{
	if(bDeskStation != m_bCurrentCallScore)
		return true;
	KillTimer(TIME_CALL_NT);
	if(iVal == 0)
	{	
		m_iCallScore[bDeskStation] = 0;		//====ĳλ�ò��з�
	}
	else
	{
		m_iPrepareNT = bDeskStation;				//���з���
		goto NT;
	}
	CallScoreStruct callscore;
	callscore.bDeskStation = bDeskStation;
	callscore.bCallScoreflag = FALSE;					
	callscore.iValue = m_iCallScore[bDeskStation];		//��ǰ�з����ͱ���

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&callscore,sizeof(callscore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_RESULT,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&callscore,sizeof(callscore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_RESULT,0);

	int iNextPeople = GetNextDeskStation(bDeskStation);

	if(m_iCallScore[iNextPeople] == 0)
	{
		m_iPrepareNT=m_iFirstJiao;
		m_iCallScore[m_iFirstJiao]=1;
		goto NT;
	}

	if(m_iCallScore[iNextPeople] == -1)
	{
		SendCallScore(iNextPeople);
		return TRUE;
	}

NT:		
	CallScoreFinish();
	return true;
}
//��ȡ��һ�����λ��
BYTE CServerGameDesk::GetNextDeskStation(BYTE bDeskStation)
{
	if(!m_bTurnRule)//˳ʱ��
	{
		int i=1;
		for (;i<PLAY_COUNT; i++)
		{
			if (m_pUserInfo[(bDeskStation + i ) % PLAY_COUNT] && m_iUserStation[(bDeskStation + i ) % PLAY_COUNT] != -1)
				break;
		}

		return (bDeskStation + i ) % PLAY_COUNT;
	}
	//��ʱ��
	int i=3;
	for (;i>=1; i--)
	{
		if (m_pUserInfo[(bDeskStation + i ) % PLAY_COUNT] &&  m_iUserStation[(bDeskStation + i ) % PLAY_COUNT] != -1)
			break;
	}
	return 	(bDeskStation + i) % PLAY_COUNT;
}
//�зֽ���
BOOL CServerGameDesk::CallScoreFinish()
{
	if (m_iPrepareNT == 255 )	//û���˽з�.���·���
	{
		GameFinish(0,GF_SALE);
		return true;
	}
	if (m_iCallScore[m_iPrepareNT] == 0 )	//û���˽з�.���·���
	{
		GameFinish(0,GF_SALE);
		return true;
	}
	//����������λ�ý�������
	m_iNtPeople = m_iPrepareNT;

	CallScoreStruct scoreresult;
	scoreresult.iValue = m_iCallScore[m_iPrepareNT];
	scoreresult.bDeskStation  = m_iPrepareNT;
	scoreresult.bCallScoreflag = false;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&scoreresult,sizeof(scoreresult),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_FINISH,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&scoreresult,sizeof(scoreresult),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_FINISH,0);

	UserNote();
	//ֱ�ӽ�����Ϸ
	//SendBackCard();
	return TRUE;
}
//���͸���һ���з�
BOOL CServerGameDesk::SendCallScore(BYTE bDeskStation)
{
	//m_bGameStation = GS_WAIT_BACK;
	m_iGameFlag = GS_FLAG_CALL_SCORE;																	

	CallScoreStruct CallScore;
	CallScore.iValue = (m_iPrepareNT == 255) ? -1 : m_iCallScore[m_iPrepareNT];
	CallScore.bDeskStation = bDeskStation;						
	CallScore.bCallScoreflag = true;	


	m_bCurrentCallScore = bDeskStation ;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		CallScore.bPlayer = m_iUserStation[i] == -1?false:true;
		SendGameData(i,&CallScore,sizeof(CallScore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE,0);
	}

	SendWatchData(m_bMaxPeople,&CallScore,sizeof(CallScore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE,0);
	SetTimer(TIME_CALL_NT,(m_iCallScoreTime+2)*1000);
	return TRUE;
}
//�õ���ҵ���ע����
__int64 CServerGameDesk::GetLimit(int bDeskStation)
{
	if (m_pUserInfo[m_iUpGradePeople]!= NULL && m_pUserInfo[bDeskStation]!=NULL&&CountPlayer()>1)
	{

		__int64 NtNote = m_pUserInfo[m_iUpGradePeople]->m_UserData.i64Money;
		__int64 StationNote = m_pUserInfo[bDeskStation]->m_UserData.i64Money;

		//2011-7-25 �����趨���ܳ���ʮ�ڣ�����ʮ����ʮ����ʾ
		/*if (NtNote > 1000000000)
		{
		NtNote = 1000000000;
		}*/
		double TempBase = 0;
		double temp = 1;
		int MaxBase = 1; //��Ϸ�����
		int iCardShapeCount = MAX_SHAPE_COUNT;
		if (m_iGameNoteSetType>0)
		{
			iCardShapeCount = 11;
		}
		for (int i=0; i<iCardShapeCount; i++)
		{
			if (m_bCardShapeBase[i] > MaxBase)
			{
				MaxBase = m_bCardShapeBase[i];
			}
		}
		if (m_GameType == 1)
		{
			TempBase = 0.125;
		}
		else
		{
			if (NtNote > StationNote)
			{
				TempBase = temp/MaxBase/(CountPlayer()-1);
			}
			else
			{
				TempBase = temp/MaxBase/(CountPlayer()-1);
			}
		}
		/*CString CST;
		CST.Format("wyslog::MaxBase===%d,m_GameType==%d",MaxBase,m_GameType);
		OutputDebugString(CST);*/
		if (NtNote > StationNote)
		{
			return StationNote*TempBase;//StationNote/5/(CountPlayer()-1); //20100802 zht ͬϲ�ֿͻ��޸�����Ӯ����
		}
		else
		{
			return NtNote*TempBase;//NtNote/5/(CountPlayer()-1); //20100802 zht ͬϲ�ֿͻ��޸�����Ӯ����
		}
	}
	else
	{
		return 0;
	}
}

//���ǿ��
BOOL CServerGameDesk::UserFourceQuit(BYTE bDeskStation)
{
	/*CString aa;
	aa.Format("AAAAA:HandleGameMessage %d, %d, %d",m_iUserStation[bDeskStation],m_iGameStation,m_bGameStation);*/

	// ���԰�ţ�ˣ���ʱ���ǿ�ˡ�ϵͳ�Զ������ţ
	if ((TYPE_OPEN==m_iGameStation || TYPE_NOTE==m_iGameStation || TYPE_BULL==m_iGameStation))
	{
		BYTE bResult[3];
		memset(bResult, 0, sizeof(bResult));
		tagUserProcess Verb;
		Verb.iVrebType = TYPE_OPEN;
		
		if (m_Logic.GetBull(m_iUserCard[bDeskStation], 5, bResult))
		{
			::CopyMemory(m_bBullCard[bDeskStation], bResult, sizeof(bResult));
		}
		else
		{
			memset(m_bBullCard[bDeskStation], 0, sizeof(m_bBullCard[bDeskStation]));
			m_bBullCard[bDeskStation][0] = m_iUserCard[bDeskStation][0];
			m_bBullCard[bDeskStation][1] = m_iUserCard[bDeskStation][1];
			m_bBullCard[bDeskStation][2] = m_iUserCard[bDeskStation][2];
		}
		
		UserActiveResult(bDeskStation, Verb.iVrebType, 0);
		CheckFinish();
	}
	else
	{
		// û�н�ׯ����ׯʱǿ��ֻ�۵�ǿ����ҷָ�ϵͳ����Ϸ����
		//GameFinish(bDeskStation, GFF_FORCE_FINISH);
	}

	//CleanCutGameInfo(bDeskStation);

	//��Ϸ����
	if(IsPlayingByGameStation())
	{	
		if(!UserAI(bDeskStation, 2, true))
			return TRUE;
	}
	else
	{
		return TRUE;
	}

	return TRUE;
}

//����ǿ�����
void CServerGameDesk::ClearQuitPlayer()
{
	return;
	//�㲥�߳����ж������
	for (BYTE i=0; i<PLAY_COUNT; i++)
	{
		if (m_bIsCut[i])
		{
			MakeUserOffLine(i);
		}
	}
}
///�������м�ʱ��
void CServerGameDesk::KillAllTimer()
{
	KillTimer(TIME_SEND_CARD);			//���ƶ�ʱ��
	KillTimer(TIME_CALL_NT);				//��ׯ
	KillTimer(TIME_GAME_FINISH);			//��Ϸ������ʱ��
	KillTimer(TIME_SEND_A_CARD);				//����һ����
	KillTimer(TIME_BEGIN_ANIMAL);				//��Ϸ��ʼ����
	KillTimer(TIME_COLLECT_JETON);				//�ռ�����
	KillTimer(TIME_CUTLINE_AUTO);				//�����Զ���ţ
	KillTimer(TIME_NOTE);             //��ע��ʱ��
}

int CServerGameDesk::GetMaxCardShape(BYTE iCard[], int iCardCount)
{
	if (iCardCount < SH_USER_CARD*PLAY_COUNT)
	{
		return 0;
	}
	int shape = 0;
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if (m_pUserInfo[i] && m_iUserStation[i]!=-1)
		{
			int temp = m_Logic.GetShape(&m_iTotalCard[i*SH_USER_CARD],SH_USER_CARD);
			if (shape < temp)
			{
				shape = temp;
			}
		}
		
	}
	return shape;
}