#include "StdAfx.h"
#include "ServerManage.h"
#include "math.h"
//#include "Common\writelog.h"
//��Ϸ����

bool CServerGameDesk:: m_bHaveKing;
bool CServerGameDesk:: m_bRobnt;
bool CServerGameDesk:: m_bAdddouble;
bool CServerGameDesk:: m_bShowcard;
UINT CServerGameDesk:: m_iPlayCard;
UINT CServerGameDesk:: m_iPlayCount;
UINT CServerGameDesk:: m_iSendCount;
UINT CServerGameDesk:: m_iBackCount;
UINT CServerGameDesk:: m_iUserCount;

BYTE CServerGameDesk:: m_iLimitUpGrade;
BOOL CServerGameDesk:: m_bKingCanReplace;
DWORD CServerGameDesk::m_iCardShape;
//��Ϸ��չ����
BYTE CServerGameDesk:: m_iThinkTime;
BYTE CServerGameDesk:: m_iBeginTime;
BYTE CServerGameDesk:: m_iGiveBackTime;
BYTE CServerGameDesk:: m_iCallScoreTime;
BYTE CServerGameDesk:: m_iAddDoubleTime;
BOOL CServerGameDesk:: m_bTurnRule;		//˳ʱ�뻹����ʱ��
int  CServerGameDesk::m_iBaseMult;	//��Ϸ�����ⶥ


//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(FULL_BEGIN)
{
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		m_iAwardPoint[i] = 0;
		m_iWinPoint[i] = 0;
	}
	m_iRecvMsg = 0;
	m_iBase = 0;
	m_iRobMul = 0;
	m_iBombMul = 0;
	m_iGameFlag = GS_FLAG_NORMAL;
	m_iUpGradePeople = -1;
	m_iLeaveArgee = 0;
	m_iNtFirstCount = 0;		//��һ��������
	//	m_iNowCallScorePeople = -1;		//
	m_iBaseOutCount = 0;
	m_iNtPeople = -1;
	m_iDealPeople = -1;
	m_iUpGradePeople = -1;
	m_iFirstOutPeople = -1;
	m_iNowBigPeople = -1;
	m_iOutCardPeople = -1;
	m_iBeenPlayGame = 0;
	m_bGameStation = GS_WAIT_ARGEE;
	m_bThrowoutCard = 0x00;
	m_bFirstCallScore = 255;
	m_iPrepareNT = 255;		//Ԥ��ׯ��
	m_iTurnFirstOut = FALSE;

	m_icountleave = 0;

	::memset(m_iBombCard,0,sizeof(m_iBombCard));	     //�ӱ�����
	m_iBombCount = 0;	                              //Ӳը��
	m_iShamBomCount = 0;                              //��ը��

	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));	//�û�����������
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));	//����������
	::memset(m_iRobNT,0,sizeof(m_iRobNT));				//���������Ʊ���
	::memset(m_iPeopleMul,1,sizeof(m_iPeopleMul));		//�Ӱ�����
	::memset(m_iAIStation,0,sizeof(m_iAIStation));		//�����й�
	::memset(m_bAuto,0,sizeof(m_bAuto));				//�й�
	::memset(m_iCallScore,-1,sizeof(m_iCallScore));		//�з�
	memset(m_iArrIsStartDisplay, 0, sizeof(m_iArrIsStartDisplay));  //���ƿ�ʼ״̬

	LoadIni();
	//���ؾ�������
	//LoadExtIni();

	InitThisGame();
#ifdef SUPER_VERSION
	for(int i = 0; i < 8; i++)
	{
		m_iSuperCardCount[i] = 0;
		::memset(m_bSuperCardList[i],0,sizeof(m_bSuperCardList[i]));
	}
	m_bSuperStation = -1;
#endif
}




//��������
CServerGameDesk::~CServerGameDesk(void)
{
}

//����ini�����ļ�
BOOL CServerGameDesk::LoadIni()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CString strTemp;
	strTemp.Format("%d", NAME_ID);
	CINIFile f( s +"\\" + strTemp +"_s.ini");
	CString key = TEXT("game");
	m_bHaveKing = f.GetKeyVal(key,"haveking",1);
	m_iPlayCard = f.GetKeyVal(key,"card",1);
	m_iPlayCount = f.GetKeyVal(key,"cardcount",54);
#ifdef MY_TWO
	m_iSendCount = f.GetKeyVal(key,"sendcard",34);
#else
	m_iSendCount = f.GetKeyVal(key,"sendcard",51);
#endif
	m_iBackCount = f.GetKeyVal(key,"backcard",3);
	m_iUserCount = f.GetKeyVal(key,"usercount",17);

	m_iLimitUpGrade = f.GetKeyVal(key,"limitupgrade",5);
	m_bKingCanReplace = f.GetKeyVal(key,"kingcanreplace",0);
	key=TEXT("other");
	m_iBeginTime = f.GetKeyVal(key,"begintime",30);
	m_iThinkTime = f.GetKeyVal(key,"thinktime",30);
	m_iGiveBackTime = f.GetKeyVal(key,"givebacktime",30);
	m_iCallScoreTime = f.GetKeyVal(key,"CallScoretime",15);
	m_iAddDoubleTime = f.GetKeyVal(key,"adddoubletime",15);
	key = TEXT("cardshape");
	m_iCardShape &=0x00000000;
	m_iCardShape |= (f.GetKeyVal(key,"one",1)&0xFFFFFFFF);//����
	m_iCardShape |= ((f.GetKeyVal(key,"two",1)<<1)&0xFFFFFFFF);//��
	m_iCardShape |= ((f.GetKeyVal(key,"three",1)<<2)&0xFFFFFFFF);//����
	m_iCardShape |= ((f.GetKeyVal(key,"threeandone",1)<<3)&0xFFFFFFFF);//������
	m_iCardShape |= ((f.GetKeyVal(key,"threeandtwo",0)<<4)&0xFFFFFFFF);//��������
	m_iCardShape |= ((f.GetKeyVal(key,"threeanddouble",1)<<5)&0xFFFFFFFF);//������

	m_iCardShape |= ((f.GetKeyVal(key,"traight",1)<<6)&0xFFFFFFFF);//��˳
	m_iCardShape |= ((f.GetKeyVal(key,"traightflush",0)<<7)&0xFFFFFFFF);//ͬ��˳
	m_iCardShape |= ((f.GetKeyVal(key,"twosequence",1)<<8)&0xFFFFFFFF);//˫˳
	m_iCardShape |= ((f.GetKeyVal(key,"threesequence",1)<<9)&0xFFFFFFFF);//��˳
	m_iCardShape |= ((f.GetKeyVal(key,"threeandonesequence",1)<<10)&0xFFFFFFFF);//��������
	m_iCardShape |= ((f.GetKeyVal(key,"threeandtwosequence",0)<<11)&0xFFFFFFFF);//��������˳
	m_iCardShape |= ((f.GetKeyVal(key,"threeanddublesequence",1)<<12)&0xFFFFFFFF);//������˳

	m_iCardShape |= ((f.GetKeyVal(key,"fourandone",0)<<13)&0xFFFFFFFF);//�Ĵ�һ
	m_iCardShape |= ((f.GetKeyVal(key,"fourandtwo",1)<<14)&0xFFFFFFFF);//�Ĵ�����
	m_iCardShape |= ((f.GetKeyVal(key,"fourandonedouble",0)<<15)&0xFFFFFFFF);//�Ĵ�һ��
	m_iCardShape |= ((f.GetKeyVal(key,"fourandtwodouble",1)<<16)&0xFFFFFFFF);//�Ĵ�����

	m_iCardShape |= ((f.GetKeyVal(key,"510k",0)<<17)&0xFFFFFFFF);			//510k
	m_iCardShape |= ((f.GetKeyVal(key,"master510k",0)<<18)&0xFFFFFFFF);		//ͬ��510k
	m_iCardShape |= ((f.GetKeyVal(key,"bomb",1)<<19)&0xFFFFFFFF);			//ը��
	m_iCardShape |= ((f.GetKeyVal(key,"kingbomb",0)<<20)&0xFFFFFFFF);		//��ը
	return true;
}

//���ؾ�������
BOOL CServerGameDesk::LoadExtIni()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CString strTemp;
	strTemp.Format("%d", NAME_ID);
	CINIFile f( s +"\\" + strTemp +"_s.ini");

	TCHAR szKey[20];
	wsprintf(szKey,"%d",NAME_ID);
	CString key = szKey;
	m_bTurnRule = f.GetKeyVal(key,"TurnRule",0);
	m_iLimitPlayGame = f.GetKeyVal(key,"limitplay",1);
	m_bRobnt = f.GetKeyVal(key,"robnt",0);
	m_bAdddouble=f.GetKeyVal(key,"adddouble",0);
	m_bShowcard=f.GetKeyVal(key,"showcard",0);
	return TRUE;
}

//���ݷ���ID���������ļ�
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{
	TCHAR szKey[20];
	wsprintf(szKey,"%d_%d",NAME_ID,iRoomID);
	
	CString key = szKey;
	CString s = CINIFile::GetAppPath ();/////����·��
	CString strTemp;
	strTemp.Format("%d", NAME_ID);
	CINIFile f( s +"\\" + strTemp +"_s.ini");
	m_bTurnRule = f.GetKeyVal(key,"TurnRule",m_bTurnRule);
	m_iLimitPlayGame = f.GetKeyVal(key,"limitplay",m_iLimitPlayGame);
	m_bRobnt = f.GetKeyVal(key,"robnt",m_bRobnt);
	m_bAdddouble=f.GetKeyVal(key,"adddouble",m_bAdddouble);
	m_bShowcard=f.GetKeyVal(key,"showcard",m_bShowcard);
	m_iBaseMult = f.GetKeyVal(key,"BaseMult",-1);
	return TRUE;
}



//�������ǿ����Ϣ
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	ASSERT(pNetHead->bMainID==MDM_GM_GAME_FRAME);
	//switch (pNetHead->bAssistantID)
	//{
	//case ASS_GM_FORCE_QUIT:
	//	//������
	//	if(m_pDataManage->m_InitData.uComType == TY_MATCH_GAME)
	//	{	
	//		UseAI(bDeskStation);
	//		return 0;
	//	}
	//	break;
	//default:
	//	break;
	//}
	return __super::HandleFrameMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

//�������й�
BOOL CServerGameDesk::UseAI(BYTE bDeskStation)
{
	m_iAIStation[bDeskStation] = 1;

	int count = 0;
	for(int i = 0; i < PLAY_COUNT ; i ++)
		if(m_iAIStation[i])
			count ++;

	if(count >= PLAY_COUNT)
	{
		GameFinish(bDeskStation,GFF_SAFE_FINISH);
		return true;
	}
	SetAIMachine(bDeskStation,TRUE);
	return true;
}

//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
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
		case ASS_GM_AGREE_GAME:		//�û�ͬ����Ϸ
			{
				if (bWatchUser) 
					return FALSE;
				if(m_bGameStation != GS_WAIT_NEXT)
					m_bGameStation = GS_WAIT_ARGEE;

				if (NULL != pData)
				{
					ShowCardStruct* pShowCard = (ShowCardStruct*)pData; //�洢����Ƿ�����,�Դ���������                                    
					m_iArrIsStartDisplay[pShowCard->bDeskStation] =  pShowCard->iValue;
				}

				////��һ���ͬ���,ÿ��һ��ʱ�俪ʼɨ���Ƿ���Կ�ʼ��Ϸ 
				//if(TY_MATCH_GAME == m_pDataManage->m_InitData.uComType)
				//	SetTimer(TIME_START_GAME,m_iBeginTime*1000);


				return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
			}
		case ASS_REPLACE_GM_AGREE_GAME:		//�������ͬ����Ϸ 
			{
				if ((bWatchUser)
					||(uSize!=sizeof(ReplaceAgreeStruct))) 
					return FALSE;
				ReplaceAgreeStruct *agree = (ReplaceAgreeStruct*)pData;
				//�����û����濪ʼ
				if(m_pUserInfo[agree->bDeskStation] != NULL
					&&m_pUserInfo[agree->bDeskStation]->m_UserData.bUserState == USER_ARGEE)
					return true;
				m_pUserInfo[agree->bDeskStation]->m_UserData.bUserState = USER_ARGEE;

				return true;
			}
		case ASS_CALL_SCORE:
			{
				if ((bWatchUser)
					||(uSize!=sizeof(CallScoreStruct))) 
					return FALSE;
				CallScoreStruct * pCallScore=(CallScoreStruct *)pData;
				UserCallScore(bDeskStation,pCallScore->iValue);
				return true;
			}
		case ASS_REPLACE_CALL_SCORE:
			{
				if (uSize!=sizeof(ReplaceCallScoreStruct)) 
					return FALSE;
				ReplaceCallScoreStruct * pReplace=(ReplaceCallScoreStruct *)pData;
				UserCallScore(pReplace->bDeskStation,pReplace->CallScore);
				return true;
			}
		case ASS_ROB_NT:						//������
			{
				if(bWatchUser)
					return false;
				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
					return true;
				RobNTStruct * pRobNT=(RobNTStruct *)pData;
				UserRobNT(pRobNT->bDeskStation,pRobNT->iValue);
				return TRUE;
			}
		case ASS_REPLACE_ROB_NT:			//����������
			{
				if(bWatchUser)
					return false;
				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
					return true;
				RobNTStruct * pRobNT=(RobNTStruct *)pData;
				UserRobNT(pRobNT->bDeskStation,pRobNT->iValue);
				return true;
			}
		case ASS_SHOW_CARD:	//����
			{
				if(bWatchUser)
					return false;
				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
					return true;
				ShowCardStruct * pShowCard=(ShowCardStruct *)pData;
				UserShowCard(pShowCard->bDeskStation,pShowCard->iValue);
				return true;
			}
		case ASS_REPLACE_SHOW_CARD://��������
			{
				if(bWatchUser)
					return false;
				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
					return true;
				ReplaceShowCardStruct * pShowCard=(ReplaceShowCardStruct *)pData;
				UserShowCard(pShowCard->bRepDeskStation,pShowCard->iValue);
				return true;
			}
		case ASS_ADD_DOUBLE:					//�Ӱ�
			{
				if(bWatchUser)
					return false;
				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
					return true;
				AddDoubleStruct * pAddDouble=(AddDoubleStruct *)pData;
				UserAddDouble(pAddDouble->bDeskStation,pAddDouble->iValue);
				return TRUE;
			}
		case ASS_REPLACE_ADD_DOUBLE:			//����Ӱ�
			{
				if(bWatchUser)
					return false;
				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
					return true;
				AddDoubleStruct * pAddDouble=(AddDoubleStruct *)pData;
				UserAddDouble(pAddDouble->bDeskStation,pAddDouble->iValue);
				return true;
			}	
		case ASS_OUT_CARD:		//�û�����
			{
				if(bWatchUser)
					return false;
				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
					return true;
				OutCardStruct * pOutCard=(OutCardStruct *)pData;
				/*if ((uSize<(sizeof(OutCardStruct)-sizeof(pOutCard->iCardList)))
					||(uSize>sizeof(OutCardStruct))||bWatchUser) return FALSE;
				if (uSize!=(sizeof(OutCardStruct)-sizeof(pOutCard->iCardList)+sizeof(BYTE)*pOutCard->iCardCount))
					return FALSE;*/
				UserOutCard(bDeskStation,pOutCard->iCardList,pOutCard->iCardCount,pOutCard->iReplaceCardList,pOutCard->bIsReplace);
				
				return true;
			}
		case ASS_REPLACE_OUT_CARD://�������
			{
				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
					return true;
				ReplaceOutCardStruct * pOutCard=(ReplaceOutCardStruct *)pData;
				/*if ((uSize<(sizeof(ReplaceOutCardStruct)-sizeof(pOutCard->iCardList)))
					||(uSize>sizeof(ReplaceOutCardStruct))||bWatchUser) return FALSE;
				if (uSize!=(sizeof(ReplaceOutCardStruct)-sizeof(pOutCard->iCardList)+sizeof(BYTE)*pOutCard->iCardCount))
					return FALSE;*/
				UserOutCard(pOutCard->bDeskStation,pOutCard->iCardList,pOutCard->iCardCount,pOutCard->iReplaceCardList,pOutCard->bIsReplace);
				return true;
			}
			//������Ϣ
		case ASS_HAVE_THING:	//���������뿪
			{
				if(bWatchUser)
					return false;
				HaveThingStruct * pThing=(HaveThingStruct *)pData;
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
		case ASS_AUTO:			//�й�
			{
				if ((bWatchUser)
					||(uSize!=sizeof(AutoStruct))) 
					return FALSE;
				AutoStruct * pAuto = (AutoStruct*)pData;
				UserSetAuto(bDeskStation,pAuto->bAuto);
				return true;
			}
		//case ASS_PROP:			//�й�
		//	{
		//		if ((bWatchUser)
		//			||(uSize!=sizeof(PropStruct))) 
		//			return FALSE;
		//		PropStruct * pProp = (PropStruct*)pData;
		//		UserProp(bDeskStation,pProp->recvDeskStation,pProp->iValue);
		//		return true;
		//	}
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}


//�O���Α��B
BOOL CServerGameDesk::InitThisGame()
{
	m_Logic.SetKingCanReplace(m_bKingCanReplace);
	return true;
}
bool CServerGameDesk::InitDeskGameStation()
{

	//   if(m_pDataManage->m_InitData.uMaxPeople>50)
	//{
	//	CString sfile;
	//	sfile.Format("ErrRoom_%d.log",m_pDataManage->m_InitData.uRoomID);
	//	CFile f;
	//	f.Open(sfile,CFile::modeCreate|CFile::modeNoInherit|CFile::modeReadWrite);
	//	sfile.Format("[%s] ���� %d �������ã�%d������50��",CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"),
	//		m_pDataManage->m_InitData.uRoomID,m_pDataManage->m_InitData.uMaxPeople);
	//	f.Write(sfile,sfile.GetLength());
	//	f.Close();
	//	typedef struct{int a;}_tag;
	//	_tag *ta=NULL;
	//	ta->a=0;
	//	return false;
	//}
	LoadExtIni();
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	return true;
}


//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	switch(uTimerID)
	{
	case TIME_START_GAME:
		{
			if(m_bGameStation == GS_WAIT_NEXT ||m_bGameStation == GS_WAIT_ARGEE)
			{
				StartGame();
			}else
			{
				KillTimer(TIME_START_GAME);
			}
			break;
		}
	case TIME_SEND_CARD:				//�l��
		{
			if (m_bGameStation == GS_SEND_CARD)
				SendCard();
			else KillTimer(TIME_SEND_CARD);
			break;
		}
	case TIME_SEND_ALL_CARD:
		{
			if (m_bGameStation == GS_SEND_CARD)
				SendAllCard();
			else KillTimer(TIME_SEND_ALL_CARD);
			break;
		}
	case TIME_WAIT_NEWTURN:				//��һ݆
		{
			KillTimer(TIME_WAIT_NEWTURN);
			if (m_bGameStation == GS_PLAY_GAME)
				NewPlayTurn(m_iNowBigPeople);
			break;
		}	
	case TIME_ADD_DOUBLE:
		{
			if (m_bGameStation == GS_WAIT_BACK)
				AddDoubleResult();
			else KillTimer(TIME_ADD_DOUBLE);
			break;
		}
	case TIME_WAIT_CALL:					//����(����)
		{
			break;
		}	
	case TIME_OUT_CARD:
		{
			KillTimer(TIME_OUT_CARD);
			if (m_bGameStation == GS_PLAY_GAME) 
				UserAutoOutCard(m_iOutCardPeople);
			break;
		}
	case TIME_GAME_FINISH:				//�Y��
		{
			KillTimer(TIME_GAME_FINISH);
			if (m_bGameStation == GS_PLAY_GAME) 
				GameFinish(0,GF_NORMAL);
			break;
		}	
	}
	return __super::OnTimer(uTimerID);
}

//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬

	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			GameStation_2 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;			//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;			//�Ͱ汾
			//��Ϸ��������
			GameStation.iPlayCard = m_iPlayCard;				//�˿˸���
			GameStation.iPlayCount = m_iPlayCount;				//�˿�����
			GameStation.bKingCanReplace = m_bKingCanReplace;				//�Ƿ�����
			GameStation.iCardShape = m_iCardShape;						//��������
			//����ʱ��
			GameStation.iBeginTime = m_iBeginTime;				//��Ϸ��ʼʱ������
			GameStation.iThinkTime = m_iThinkTime;				//��Ϸ˼��ʱ��
			GameStation.iCallScoreTime = m_iCallScoreTime;			//�з�ʱ��
			GameStation.iAddDoubleTime = m_iAddDoubleTime;			//�з�ʱ��
			//��Ϸ����
			GameStation.iPlayLimit = m_iLimitPlayGame;
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();	//���ӱ���
			GameStation.iRoomBasePoint = GetRoomMul();	//���䱶��
			GameStation.iRunPublish = GetRunPublish();			//���ܿ۷�
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	case GS_SEND_CARD:		//����״̬
	case GS_WAIT_BACK:		//�ȴ������״̬
		{
			//����ȡ���������й�
			if(!bWatchUser)
				SetAIMachine(bDeskStation,false);

			GameStation_3 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;			//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;			//�Ͱ汾
			//��Ϸ����
			GameStation.iPlayLimit = m_iLimitPlayGame;
			//��Ϸ��������
			GameStation.iPlayCard = m_iPlayCard;				//�˿˸���
			GameStation.iPlayCount = m_iPlayCount;				//�˿�����
			GameStation.bKingCanReplace = m_bKingCanReplace;				//�Ƿ�����
			GameStation.iCardShape = m_iCardShape;						//��������
			//����ʱ��
			GameStation.iBeginTime = m_iBeginTime;				//��Ϸ��ʼʱ������
			GameStation.iThinkTime = m_iThinkTime;				//��Ϸ˼��ʱ��
			GameStation.iCallScoreTime = m_iCallScoreTime;			//�з�ʱ��
			GameStation.iAddDoubleTime = m_iAddDoubleTime;			//�з�ʱ��
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();	//���ӱ���
			GameStation.iRoomBasePoint = GetRoomMul();	//���䱶��
			GameStation.iRunPublish = GetRunPublish();			//���ܿ۷�

			GameStation.iUpGradePeople = m_iUpGradePeople;		
			GameStation.iBeenPlayGame = m_iBeenPlayGame;
			GameStation.iGameFlag = m_iGameFlag;				//m_iGameFlag��Ϊ�з�״̬

			GameStation.iCallScoreResult = (m_iNtPeople == -1)?-1:m_iCallScore[m_iNtPeople];	//��ǰ���еķ�
			GameStation.iCallScorePeople = m_bCurrentCallScore;

			//�����й�״̬����
			for(int i = 0; i < PLAY_COUNT; i ++)
			{
				GameStation.bAuto[i] = m_bAuto[i];
				GameStation.iCallScore[i] = m_iCallScore[i];	//��ǰ���������з�
				GameStation.iRobNT[i] = m_iRobNT[i];
			}

			int iPos = 0;
			for(int i = 0; i < PLAY_COUNT; i ++)
			{
				GameStation.iUserCardCount[i] = m_iUserCardCount[i];
				::CopyMemory(&GameStation.iUserCardList[iPos],m_iUserCard[i],sizeof(BYTE)*m_iUserCardCount[i]);
				iPos += m_iUserCardCount[i];
			}
			//��������
			int iSendSize = sizeof(GameStation) - sizeof(GameStation.iUserCardList) + sizeof(BYTE) + iPos;
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,iSendSize);
			return TRUE;
		}

	case GS_PLAY_GAME:	//��Ϸ��״̬
		{
			//����ȡ���������й�
			if(!bWatchUser)
				SetAIMachine(bDeskStation,false);

			GameStation_4 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;			//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;			//�Ͱ汾
			//��Ϸ��������
			GameStation.iPlayCard = m_iPlayCard;				//�˿˸���
			GameStation.iPlayCount = m_iPlayCount;				//�˿�����
			GameStation.bKingCanReplace = m_bKingCanReplace;				//�Ƿ�����
			GameStation.iCardShape = m_iCardShape;						//��������
			//����ʱ��
			GameStation.iBeginTime = m_iBeginTime;				//��Ϸ��ʼʱ������
			GameStation.iThinkTime = m_iThinkTime;				//��Ϸ˼��ʱ��
			GameStation.iCallScoreTime = m_iCallScoreTime;			//�з�ʱ��
			GameStation.iAddDoubleTime = m_iAddDoubleTime;			//�з�ʱ��
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();	//���ӱ���
			GameStation.iRoomBasePoint = GetRoomMul();	//���䱶��
			GameStation.iRunPublish = GetRunPublish();			//���ܿ۷�
			//��Ϸ����
			GameStation.iPlayLimit = m_iLimitPlayGame;

			//����
			//::memcpy(GameStation.iBackCard,m_iBackCard,sizeof(BYTE)*m_iBackCount);
			GameStation.iUpGradePeople = m_iUpGradePeople;		
			GameStation.iBeenPlayGame = m_iBeenPlayGame;		
			GameStation.iCallScoreResult = m_iCallScore[m_iUpGradePeople];	//��ǰ�з�
			GameStation.iOutCardPeople = m_iOutCardPeople;	
			GameStation.iFirstOutPeople = m_iFirstOutPeople;	
			GameStation.iBigOutPeople = m_iNowBigPeople;	
			GameStation.iBaseOutCount = m_iBaseOutCount;	
			GameStation.iBase = m_iBase;								//��Ϸ����	
			//�й�״̬
			for(int i = 0; i < PLAY_COUNT; i ++)
			{
				GameStation.bAuto[i] = m_bAuto[i];
				GameStation.iAwardPoint[i] = m_iAwardPoint[i];	//���ҽ���
				GameStation.iPeopleBase[i] = m_iPeopleMul[i];	//�Ӱ�
                GameStation.iArrIsDisplayCard[i] = m_iArrIsStartDisplay[i]; //�Ƿ����ƿ�ʼ		
				GameStation.iRobNT[i] = m_iRobNT[i];
			}	
			//���ø���������
			int iPos = 0;
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				//�����û�������
				GameStation.iUserCardCount[i] = m_iUserCardCount[i];
				::CopyMemory(&GameStation.iUserCardList[iPos],m_iUserCard[i],sizeof(BYTE)*m_iUserCardCount[i]);
				iPos += m_iUserCardCount[i];
				//�����û���������
				GameStation.iDeskCardCount[i] = m_iDeskCardCount[i];
				::CopyMemory(&GameStation.iUserCardList[iPos],m_iDeskCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);
				iPos += m_iDeskCardCount[i];
			}
			//��ǰ�����
			GameStation.iBaseOutCount = m_iBaseOutCount;
			::CopyMemory(&GameStation.iUserCardList[iPos],m_iBaseCard[m_iNowBigPeople],sizeof(BYTE)*m_iBaseOutCount);
			iPos += m_iBaseOutCount;
			//����
			GameStation.iBackCardCount = m_iBackCount;
			::CopyMemory(&GameStation.iUserCardList[iPos],m_iBackCard,sizeof(BYTE)*m_iBackCount);
			iPos += m_iBackCount;

			//��������
			int iSendSize = sizeof(GameStation) - sizeof(GameStation.iUserCardList) + sizeof(BYTE) + iPos;
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,iSendSize);
			return TRUE;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{
			//����ȡ���������й�
			//SetAIMachine(bDeskStation,false);
			GameStation_5 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;			//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;			//�Ͱ汾
			//��Ϸ��������
			GameStation.iPlayCard = m_iPlayCard;				//�˿˸���
			GameStation.iPlayCount = m_iPlayCount;				//�˿�����
			GameStation.iCardShape = m_iCardShape;						//��������
			//����ʱ��
			GameStation.iBeginTime = m_iBeginTime;				//��Ϸ��ʼʱ������
			GameStation.iThinkTime = m_iThinkTime;				//��Ϸ˼��ʱ��
			GameStation.iCallScoreTime = m_iCallScoreTime;			//�з�ʱ��
			GameStation.iAddDoubleTime = m_iAddDoubleTime;			//�з�ʱ��
			//��Ϸ����
			GameStation.iPlayLimit = m_iLimitPlayGame;
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();	//���ӱ���
			GameStation.iRoomBasePoint = GetRoomMul();	//���䱶��
			GameStation.iRunPublish = GetRunPublish();			//���ܿ۷�
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	}
	return false;
}

//�����м�ʱ��
void CServerGameDesk::KillAllTimer()
{
	KillTimer(TIME_SEND_CARD);
	KillTimer(TIME_SEND_ALL_CARD);
	KillTimer(TIME_WAIT_CALL);
	KillTimer(TIME_GAME_FINISH);
	KillTimer(TIME_WAIT_NEWTURN);
	KillTimer(TIME_OUT_CARD);
	KillTimer(TIME_START_GAME);
	KillTimer(TIME_ADD_DOUBLE);
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	KillAllTimer();
	if ((bLastStation == GFF_FORCE_FINISH)||( bLastStation== GF_SAFE))
	{
		::memset(m_iWinPoint,0,sizeof(m_iWinPoint));
		::memset(m_iAIStation,0,sizeof(m_iAIStation));
		m_iBeenPlayGame=0;
		m_iNtPeople=-1;
		m_iDealPeople=-1;
	}
	m_iPrepareNT = 255;		//Ԥ��ׯ��
	m_iUpGradePeople = -1;
	m_iBase = 0;
	m_iRobMul = 0;
	m_iBombMul = 0;
	m_iTurnFirstOut = FALSE;

	for(int i = 0;i < PLAY_COUNT; i ++)
	{
		m_iAwardPoint[i]=0;
	}
	m_Logic.SetLai(255);
	m_iGameFlag = GS_FLAG_NORMAL;
	m_bCurrentCallScore = 255;
	m_iNtFirstCount=0;		//ׯ�ҳ��ĵ�һ��������
	m_iLeaveArgee=0;
	m_iBaseOutCount=0;
	m_iNowBigPeople=-1;
	m_iOutCardPeople=-1;
	m_iSendCardPos=0;
	m_bFirstCallScore = 255;
	memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));

	::memset(m_iBombCard,0,sizeof(m_iBombCard));	     //�ӱ�����
	m_iBombCount = 0;	                              //Ӳը��
	m_iShamBomCount = 0;                              //��ը��

	::memset(m_iRobNT,0,sizeof(m_iRobNT));				//���������Ʊ���
	::memset(m_iPeopleMul,1,sizeof(m_iPeopleMul));		//�Ӱ�����
	::memset(m_iAIStation,0,sizeof(m_iAIStation));		//�����й�
	::memset(m_bAuto,0,sizeof(m_bAuto));				//�й�
	::memset(m_iCallScore,-1,sizeof(m_iCallScore));		//�з�
	memset(m_iArrIsStartDisplay, 0, sizeof(m_iArrIsStartDisplay));//�Ƿ�����

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

//������Զ���ʼ��Ϸ
BOOL CServerGameDesk::StartGame()
{
	for(int i = 0; i < PLAY_COUNT ;i++)
	{
		if(m_pUserInfo[i] == NULL)
			UseAI(i);
	}
	if(GameBegin(ALL_ARGEE))
		KillTimer(TIME_START_GAME);
	return TRUE;
}

//��Ϸ��ʼ
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag)==false) 
		return false;

	m_byteHitPass = 0;
	m_icountleave =0;
	m_iSendCardPos = 0;
	m_iBeenPlayGame ++;
	m_bGameStation = GS_SEND_CARD;
	m_iTurnFirstOut = TRUE;
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));
	::memset(m_iBaseCard,0,sizeof(m_iBaseCard));
	//���Ϳ�ʼ��Ϣ
	BeginUpgradeStruct BeginMessage;
	BeginMessage.iUpgradeStation = m_iUpGradePeople;
	BeginMessage.iBeenPlayGame = m_iBeenPlayGame;
	BeginMessage.iPlayLimit = m_iLimitPlayGame;



	for (int i = 0; i < PLAY_COUNT; i ++)
		SendGameData(i,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_GAME_BEGIN,0);
	SendWatchData(m_bMaxPeople,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_GAME_BEGIN,0);

	//�ַ��˿�
	BYTE iCardArray[162];
	m_Logic.RandCard(iCardArray,m_iPlayCount,m_bHaveKing);
	m_Logic.SetLai(255);
#ifdef SUPER_VERSION
	for(int j = 0; j < PLAY_COUNT; j++)
	{
		for(int i = 0; i < m_iSuperCardCount[j]; i ++)
		{
			//����Ӧһ����
			int temp = m_Logic.GetSerialBySpecifyCard(iCardArray,0,m_iPlayCount,m_bSuperCardList[j][i]);

			if(temp != -1)
			{	
				BYTE t = iCardArray[i + j * m_iUserCount];
				iCardArray[i + j * m_iUserCount] = iCardArray[temp];
				iCardArray[temp] = t;
			}
		}
	}
#endif
	//����
	srand(GetTickCount());
	m_bThrowoutCard = iCardArray[rand() % (m_iSendCount/*m_iPlayCount - 3*/)];

	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		::memset(m_iBaseCard[i],0,sizeof(m_iBaseCard[i]));
		::CopyMemory(m_iUserCard[i],&iCardArray[m_iUserCount*i],sizeof(BYTE)*m_iUserCount);
	}
	::CopyMemory(m_iBackCard,&iCardArray[m_iSendCount],sizeof(BYTE)*m_iBackCount);

	GamePrepare();

	return TRUE;
}

//׼������
BOOL CServerGameDesk::GamePrepare()
{
	srand((unsigned int )time(0));

	m_iDealPeople = rand() % PLAY_COUNT;

	PrepareStruct Prepare;
	Prepare.bCard = m_bThrowoutCard;

	for(int i = 0; i < PLAY_COUNT; i ++)
		SendGameData(i,&Prepare,sizeof(Prepare),MDM_GM_GAME_NOTIFY,ASS_GAME_PREPARE,0);

	SendWatchData(m_bMaxPeople,&Prepare,sizeof(Prepare),MDM_GM_GAME_NOTIFY,ASS_GAME_PREPARE,0);

	//������Ϣ
	//SetTimer(TIME_SEND_CARD,50);
	SetTimer(TIME_SEND_ALL_CARD,1000);

	return TRUE;
}

//�����˿˸��û�
BOOL CServerGameDesk::SendCard()
{
	if (m_iSendCardPos == m_iSendCount)
	{
		
		KillTimer(TIME_SEND_CARD);
		SendCardFinish();
		return TRUE;
	}
	//���������˿�(1�η�����)
	for(int i = 0; i < 2; i ++)
	{
		BYTE bDeskStation = (m_iDealPeople + m_iSendCardPos) % PLAY_COUNT;
		SendCardStruct SendCard;
		SendCard.bDeskStation = bDeskStation;
		SendCard.bCard = m_iUserCard[bDeskStation][m_iSendCardPos/PLAY_COUNT];

		for(int i = 0; i < PLAY_COUNT; i ++)
			SendGameData(i,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);
		SendWatchData(m_bMaxPeople,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);

		SendCardMsg(bDeskStation,SendCard.bCard);

		m_iUserCardCount[bDeskStation] ++;
		m_iSendCardPos ++;
		if(m_iSendCardPos == m_iSendCount)
			break;
	}
	return TRUE;
}

//������Ϣ
BOOL CServerGameDesk::SendCardMsg(BYTE bDeskStation,BYTE bCard)
{
	if(m_bFirstCallScore != 255)
		return TRUE;
	if(bCard == m_bThrowoutCard)
	{
		m_bFirstCallScore = bDeskStation;
		SendCardStruct sendcard;
		sendcard.bDeskStation = bDeskStation;
		sendcard.bCard = m_bThrowoutCard;
		for(int i = 0; i < PLAY_COUNT; i ++)
			SendGameData(i,&sendcard,sizeof(sendcard),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD_MSG,0);

		SendWatchData(m_bMaxPeople,&sendcard,sizeof(sendcard),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD_MSG,0);
	}
	return TRUE;
}

//һ�ΰl������
BOOL CServerGameDesk::SendAllCard()
{
	
	SendAllStruct SendAll;
	::memset(&SendAll,0,sizeof(SendAll));

	int iPos = 0;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		m_iUserCardCount[i] = m_iUserCount;		
		SendAll.iUserCardCount[i] = m_iUserCardCount[i];
		::CopyMemory(&SendAll.iUserCardList[iPos],m_iUserCard[i],sizeof(BYTE)*m_iUserCardCount[i]);
		iPos += m_iUserCardCount[i];
	}

	//��������
	for(int i = 0;i < PLAY_COUNT; i ++)
		SendGameData(i,&SendAll,sizeof(SendAll),MDM_GM_GAME_NOTIFY,ASS_SEND_ALL_CARD,0);
	SendWatchData(m_bMaxPeople,&SendAll,sizeof(SendAll),MDM_GM_GAME_NOTIFY,ASS_SEND_ALL_CARD,0);

	m_iSendCardPos ++;
	if(m_iSendCardPos == 1)
	{
		KillTimer(TIME_SEND_ALL_CARD);
		SendCardFinish();
		return FALSE;
	}

	return 0;
}

//���ƽ���
BOOL CServerGameDesk::SendCardFinish()
{
	SendCardFinishStruct sendcardfinish;

	//���ͷ��ƽ���
	for(int i = 0; i < PLAY_COUNT; i ++)
		SendGameData(i,&sendcardfinish,sizeof(sendcardfinish),MDM_GM_GAME_NOTIFY,ASS_SEND_FINISH,0);
	SendWatchData(m_bMaxPeople,&sendcardfinish,sizeof(sendcardfinish),MDM_GM_GAME_NOTIFY,ASS_SEND_FINISH,0);

	if (m_bFirstCallScore == 255)
	{
		srand(GetTickCount());
		m_bFirstCallScore = rand()%PLAY_COUNT;
	}
	SendCallScore(m_bFirstCallScore);
	return true;
}

//���͸���һ���з�
BOOL CServerGameDesk::SendCallScore(BYTE bDeskStation)
{
	m_bGameStation = GS_WAIT_BACK;
	m_iGameFlag = GS_FLAG_CALL_SCORE;																	

	CallScoreStruct CallScore;
	CallScore.iValue = (m_iPrepareNT == 255) ? -1 : m_iCallScore[m_iPrepareNT];
	CallScore.bDeskStation = bDeskStation;						
	CallScore.bCallScoreflag = true;						

	m_bCurrentCallScore = bDeskStation ;

	for(int i = 0; i < PLAY_COUNT; i ++)
		SendGameData(i,&CallScore,sizeof(CallScore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE,0);

	SendWatchData(m_bMaxPeople,&CallScore,sizeof(CallScore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE,0);
	return TRUE;
}

//�з�
BOOL CServerGameDesk::UserCallScore(BYTE bDeskStation, int iVal)
{
	if(bDeskStation != m_bCurrentCallScore)
		return true;

	if(iVal == 0)
	{	
		m_iCallScore[bDeskStation] = 0;		//====ĳλ�ò��з�
	}
	else
	{
		m_iCallScore[bDeskStation] = iVal;	//�з�����

		m_iPrepareNT = bDeskStation;				//���з���

		if( iVal == 30)						//����ֱ�ӽ�3��
		{
			CallScoreStruct callscore;
			callscore.bDeskStation = bDeskStation;
			callscore.bCallScoreflag = FALSE;					
			callscore.iValue = m_iCallScore[bDeskStation];		//��ǰ�з����ͱ���

			for(int i = 0; i < PLAY_COUNT; i ++)
				SendGameData(i,&callscore,sizeof(callscore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_RESULT,0);		//���з�������������û�

			goto NT;
		}
	}
	CallScoreStruct callscore;
	callscore.bDeskStation = bDeskStation;
	callscore.bCallScoreflag = FALSE;					
	callscore.iValue = m_iCallScore[bDeskStation];		//��ǰ�з����ͱ���

	for(int i = 0; i < PLAY_COUNT; i ++)
		SendGameData(i,&callscore,sizeof(callscore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_RESULT,0);		//���з�������������û�

	SendWatchData(m_bMaxPeople,&callscore,sizeof(callscore),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE_RESULT,0);

	int iNextPeople = GetNextDeskStation(bDeskStation);

	if(m_iCallScore[iNextPeople] == 0)
		goto NT;

	if(m_iCallScore[iNextPeople] == -1)
	{
		SendCallScore(iNextPeople);
		return TRUE;
	}

NT:		
	CallScoreFinish();
	return true;
}

//�зֽ���
BOOL CServerGameDesk::CallScoreFinish()
{
	if (m_iPrepareNT == 255 )	//û���˽з�.���·���
	{
		m_iPrepareNT = m_bFirstCallScore;
		m_iCallScore[m_bFirstCallScore] = 10;
		/*GameFinish(0,GF_NO_CALL_SCORE);
		return true;*/
	}
	if (m_iCallScore[m_iPrepareNT] == 0 )	//û���˽з�.���·���
	{
		m_iPrepareNT = m_bFirstCallScore;
		m_iCallScore[m_bFirstCallScore] = 10;
		/*GameFinish(0,GF_NO_CALL_SCORE);
		return true;*/
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

	BYTE bDeskStation = GetRobNtDeskStation(m_iPrepareNT);
	if(bDeskStation == m_iPrepareNT || !m_bRobnt)
		RobNTFinish();
	else
		SendRobNT(bDeskStation);

	//ֱ�ӽ�����Ϸ
	//SendBackCard();
	return TRUE;
}

//������������Ϣ
BOOL CServerGameDesk::SendRobNT(BYTE bDeskStation)
{
	m_iGameFlag = GS_FLAG_ROB_NT;				//������																
	//���������
	RobNTStruct  robnt;
	robnt.bDeskStation = bDeskStation;
	robnt.iValue = -1;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&robnt,sizeof(robnt),MDM_GM_GAME_NOTIFY,ASS_ROB_NT,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&robnt,sizeof(robnt),MDM_GM_GAME_NOTIFY,ASS_ROB_NT,0);
	return TRUE;
}

//�����������Ϣ
BOOL CServerGameDesk::UserRobNT(BYTE bDeskStation, int iValue)
{
	//������������
	if(iValue)
	{
		m_iRobNT[bDeskStation] += 1;				//������
		m_iBase += 1;
		m_iRobMul += 1;
		AwardPointStruct awardpoint;
		::memset(&awardpoint,0,sizeof(awardpoint));
		awardpoint.iBase = m_iBase;
		//��Ϸͳһ����
		for(int i = 0 ; i < PLAY_COUNT;i++)
		{
			SendGameData(i,&awardpoint,sizeof(awardpoint),MDM_GM_GAME_NOTIFY,ASS_GAME_MULTIPLE,0);		//���з�������������û�
		}
		m_iPrepareNT = bDeskStation;																	//��һԤ��ׯ��
	}
	RobNTStruct  robnt;
	robnt.bDeskStation = bDeskStation;
	robnt.iValue = iValue;
	for(int i = 0;i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&robnt,sizeof(robnt),MDM_GM_GAME_NOTIFY,ASS_ROB_NT_RESULT,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&robnt,sizeof(robnt),MDM_GM_GAME_NOTIFY,ASS_ROB_NT_RESULT,0);
	//���������
	if(bDeskStation == m_iNtPeople)
	{
		RobNTFinish();
		return TRUE;
	}

	BYTE bDesk = GetRobNtDeskStation(bDeskStation);
	//��ũ��δ������
	if(bDesk == m_iNtPeople 
		&&m_iRobNT[(m_iNtPeople + 1 )%PLAY_COUNT] == 0
		 &&m_iRobNT[(m_iNtPeople + 2 )%PLAY_COUNT] == 0)
	{
		RobNTFinish();
	}else
	{
		SendRobNT(bDesk);
	}
	return TRUE;
}

//��ȡ��һ�����λ��
BYTE CServerGameDesk::GetNextDeskStation(BYTE bDeskStation)
{
	if(!m_bTurnRule)//˳ʱ��
		return (bDeskStation + 1 ) % PLAY_COUNT;
	//��ʱ��
	return 	(bDeskStation + (PLAY_COUNT - 1)) % PLAY_COUNT;
}

//��ȡһ�����������
BYTE CServerGameDesk::GetRobNtDeskStation(BYTE bDeskStation)
{
	for(int i = GetNextDeskStation(bDeskStation); ; i = GetNextDeskStation(i))
	{
		if(m_iCallScore[i] != 0)
			return i;
	}
	return bDeskStation;
}

//����������
BOOL CServerGameDesk::RobNTFinish()
{
	//�������Ƿ�ɹ�
	if(m_iPrepareNT != m_iNtPeople)
	{
		RobNTStruct  robresult;
		robresult.bDeskStation = m_iPrepareNT;
		robresult.iValue = -1;
		for (int i = 0; i < PLAY_COUNT; i ++) 
			SendGameData(i,&robresult,sizeof(robresult),MDM_GM_GAME_NOTIFY,ASS_ROB_NT_FINISH,0);
		SendWatchData(m_bMaxPeople,&robresult,sizeof(robresult),MDM_GM_GAME_NOTIFY,ASS_ROB_NT_FINISH,0);
	}

	//����������������
	SendBackCard();
	return TRUE;
}

//���͵���
BOOL CServerGameDesk::SendBackCard()
{
	//תׯ��
//	m_iNtPeople = m_iPrepareNT;//��������,

	m_iUpGradePeople = m_iPrepareNT;

	BackCardExStruct BackCard;
	BackCard.iGiveBackPeople = m_iUpGradePeople;
	BackCard.iBackCardCount = m_iBackCount;

	::CopyMemory(&m_iUserCard[m_iUpGradePeople][m_iUserCardCount[m_iUpGradePeople]],m_iBackCard,sizeof(BYTE)*BackCard.iBackCardCount);
	m_iUserCardCount[m_iUpGradePeople] += BackCard.iBackCardCount;

	::CopyMemory(BackCard.iBackCard,m_iBackCard,sizeof(BYTE)*BackCard.iBackCardCount);

	m_Logic.SetLai(BackCard.iBackCard[1]);
	for (int i = 0; i < PLAY_COUNT; i ++) 
		SendGameData(i,&BackCard,sizeof(BackCard),MDM_GM_GAME_NOTIFY,ASS_BACK_CARD_EX,0);
	SendWatchData(m_bMaxPeople,&BackCard,sizeof(BackCard),MDM_GM_GAME_NOTIFY,ASS_BACK_CARD_EX,0);

	if(m_bAdddouble)
		SendAddDouble();
	else
		AddDoubleFinish();
	//ֱ�ӽ�����Ϸ
	//BeginPlay();
	return TRUE;
}

//���ͼӰ���Ϣ
BOOL CServerGameDesk::SendAddDouble()
{
	m_iGameFlag = GS_FLAG_ADD_DOUBLE;				//�Ӱ�																
	m_iRecvMsg = 0;

	SetTimer(TIME_ADD_DOUBLE,m_iAddDoubleTime * 1000);
	//���������
	AddDoubleStruct  adddouble;
	adddouble.bDeskStation = m_iUpGradePeople;
	adddouble.iValue = -1;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&adddouble,sizeof(adddouble),MDM_GM_GAME_NOTIFY,ASS_ADD_DOUBLE,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&adddouble,sizeof(adddouble),MDM_GM_GAME_NOTIFY,ASS_ADD_DOUBLE,0);
	return  TRUE;
}

//��ҼӰ�
BOOL CServerGameDesk::UserAddDouble(BYTE bDeskStation, int iVal)
{
	m_iRecvMsg ++ ;

	if(iVal)
	{//��ҼӰ�
		m_iPeopleMul[bDeskStation] = 2;
	}

	if(m_iRecvMsg >= 2)
		AddDoubleResult();
	return TRUE;
}

//�Ӱ����
BOOL CServerGameDesk::AddDoubleResult()
{
	KillTimer(TIME_ADD_DOUBLE);

	for(int j = 0; j < PLAY_COUNT ;j++)
	{
		//������������
		AddDoubleStruct  adddouble;
		adddouble.bDeskStation = j;
		adddouble.iValue = m_iPeopleMul[j];

		for(int i = 0; i < PLAY_COUNT; i ++)
		{
			SendGameData(i,&adddouble,sizeof(adddouble),MDM_GM_GAME_NOTIFY,ASS_ADD_DOUBLE_RESULT,0);		//���з�������������û�
		}
		SendWatchData(m_bMaxPeople,&adddouble,sizeof(adddouble),MDM_GM_GAME_NOTIFY,ASS_ADD_DOUBLE_RESULT,0);
	}
	AddDoubleFinish();

	return TRUE;
}

//�Ӱ�����
BOOL CServerGameDesk::AddDoubleFinish()
{
	//������������
	AddDoubleStruct  adddouble;
	::memset(&adddouble,0,sizeof(adddouble));
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&adddouble,sizeof(adddouble),MDM_GM_GAME_NOTIFY,ASS_ADD_DOUBLE_FINISH,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&adddouble,sizeof(adddouble),MDM_GM_GAME_NOTIFY,ASS_ADD_DOUBLE_FINISH,0);

	if(m_bShowcard)
		SendShowCard();
	else
		ShowCardFinish();
	return true;
}

//����
BOOL CServerGameDesk::SendShowCard()
{
	m_iGameFlag = GS_FLAG_SHOW_CARD;				//����																
	ShowCardStruct show;
	show.bDeskStation = m_iUpGradePeople;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&show,sizeof(show),MDM_GM_GAME_NOTIFY,ASS_SHOW_CARD,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&show,sizeof(show),MDM_GM_GAME_NOTIFY,ASS_SHOW_CARD,0);
	return TRUE;
}

//������Ϣ
BOOL CServerGameDesk::UserShowCard(BYTE bDeskStation,int iValue)
{
	if(iValue)
	{
		m_iBase += 1;
		m_iRobMul += 1;

		m_iRobNT[bDeskStation] += 2;				//����
	}

	ShowCardStruct showresult;
	showresult.bDeskStation = bDeskStation;
	showresult.iValue = iValue;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&showresult,sizeof(showresult),MDM_GM_GAME_NOTIFY,ASS_SHOW_CARD_RESULT,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&showresult,sizeof(showresult),MDM_GM_GAME_NOTIFY,ASS_SHOW_CARD_RESULT,0);

	ShowCardFinish();
	return TRUE;
}

//���ƽ���
BOOL CServerGameDesk::ShowCardFinish()
{
	ShowCardStruct showresult;
	::memset(&showresult,0,sizeof(showresult));

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&showresult,sizeof(showresult),MDM_GM_GAME_NOTIFY,ASS_SHOW_CARD_FINISH,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&showresult,sizeof(showresult),MDM_GM_GAME_NOTIFY,ASS_SHOW_CARD_FINISH,0);

	BeginPlay();

	return TRUE;
}

//��Ϸ��ʼ
BOOL CServerGameDesk::BeginPlay()
{
	//��������
	m_bGameStation = GS_PLAY_GAME;
	m_iBaseOutCount = 0;
	m_iNowBigPeople = m_iOutCardPeople = m_iFirstOutPeople = m_iUpGradePeople;

	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));

	//�����˿�
	for (int i = 0; i < PLAY_COUNT; i ++) 
		m_Logic.SortCard(m_iUserCard[i],NULL,m_iUserCardCount[i]);

	//������Ϸ��ʼ��Ϣ
	BeginPlayStruct Begin;
	Begin.iOutDeskStation = m_iOutCardPeople;
	for (int i = 0;i < PLAY_COUNT; i ++)
		SendGameData(i,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
	SendWatchData(m_bMaxPeople,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
	SetTimer(TIME_OUT_CARD,(m_iThinkTime + 2)*1000);

	return TRUE;
}

//�û�����
BOOL CServerGameDesk::UserOutCard(BYTE bDeskStation,  BYTE iOutCard[],  int iCardCount,  BYTE iReplaceCardList[],bool bIsReplace)
{
	//GameFinish(bDeskStation,GF_NORMAL);
	//return true;

	if (bDeskStation != m_iOutCardPeople) 
		return true;

	//�����������
	if(iCardCount > 0)					
	{
		if(0 == m_iNtFirstCount)		//�������ĵ�һ����
			m_iNtFirstCount = iCardCount;


		//CString str;
		//str.Format("zhtout:bDeskStation=%d,m_iFirstOutPeople=%d",bDeskStation,m_iFirstOutPeople);
		//OutputDebugString(str);
		if (bIsReplace)
		{
			if (!m_Logic.CanOutCard(iReplaceCardList,iCardCount,m_iBaseCard[m_iFirstOutPeople],m_iBaseOutCount,
				m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation],bDeskStation == m_iFirstOutPeople))
			{
				return TRUE;
			}
		}
		else
		{
		if (!m_Logic.CanOutCard(iOutCard,iCardCount,m_iBaseCard[m_iFirstOutPeople],m_iBaseOutCount,
			m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation],bDeskStation == m_iFirstOutPeople))
		{
			return TRUE;
		}
		}
		m_iNowBigPeople = bDeskStation;
		m_iBaseOutCount = iCardCount;

		//ɾ���˿�
		if (m_Logic.RemoveCard(iOutCard,iCardCount,m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation]) == 0)
			return TRUE;
		
		m_iUserCardCount[bDeskStation] -= iCardCount;
		//��¼������Ϣ
		m_iDeskCardCount[bDeskStation] = iCardCount;
		::CopyMemory(m_iDeskCard[bDeskStation],iOutCard,sizeof(BYTE)*iCardCount);
		if (bIsReplace)
			::CopyMemory(m_iBaseCard[bDeskStation],iReplaceCardList,sizeof(BYTE)*iCardCount);
		else
			::CopyMemory(m_iBaseCard[bDeskStation],iOutCard,sizeof(BYTE)*iCardCount);

	}
	else
	{	//ĳһλ��ʲ�N��Ҳ����
		m_iDeskCardCount[bDeskStation] = iCardCount;

		m_byteHitPass |= (1<<bDeskStation);	//��¼PASS
	}
	KillTimer(TIME_OUT_CARD);
	//������ҳ��ƽ��
	OutCardMsg UserOutResult;
	UserOutResult.bDeskStation = bDeskStation;
	UserOutResult.iCardCount = iCardCount;
	//iNextDeskStation��ʱ��ʾ�����Ƿ�����һ�ֳ��� (090402b2 JJ)
	UserOutResult.iNextDeskStation=m_iTurnFirstOut;
	m_iTurnFirstOut = FALSE;
	///////////////////////////////////////////end
	UserOutResult.bIsReplace = bIsReplace;
	::CopyMemory(UserOutResult.iReplaceCardList,iReplaceCardList,sizeof(BYTE)*iCardCount);
	::CopyMemory(UserOutResult.iCardList,iOutCard,sizeof(BYTE)*iCardCount);

	int iSendSizeReslut = sizeof(UserOutResult);

	for (int i = 0;i < PLAY_COUNT; i ++) 
		SendGameData(i,&UserOutResult,iSendSizeReslut,MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);

	SendWatchData(m_bMaxPeople,&UserOutResult,iSendSizeReslut,MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);

	//�Ƿ�Ϊ�ӷ�����
	if (bIsReplace)
		IsAwardPoin(iReplaceCardList,iCardCount,bDeskStation);
	else
		IsAwardPoin(iOutCard,iCardCount,bDeskStation);

	//====�ж��Ƿ��������Ƿ������
	if (m_iUserCardCount[bDeskStation] <= 0)
	{
		m_iDealPeople = bDeskStation;
		//�����Ƹ������O��
		//OutCardFinish(bDeskStation);

		//����Ƿ����
		//if(CheckFinish())
		{
			SetTimer(TIME_GAME_FINISH,1000);
			return true;
		}
	}
	//�����������,���¿�ʼ��һ��
	if(m_Logic.IsKingBomb(iOutCard,iCardCount))
	{	
		m_iNowBigPeople = m_iFirstOutPeople = m_iOutCardPeople = bDeskStation;
		IsNewTurn();
		return true;
	}

	//������һ�ֳ�����
	m_iOutCardPeople = GetNextDeskStation(bDeskStation);//(bDeskStation+1)%PLAY_COUNT;

	for(int i = m_iOutCardPeople ;;i = GetNextDeskStation(i))
	{
		m_iOutCardPeople = i;				//��ǰ������
		if(IsNewTurn())
			return true;
		//��ǰ����������δ��
		if(m_iUserCardCount[i]>0)
			break;
	}

	OutCardMsg UserOut;
	UserOut.iNextDeskStation = m_iOutCardPeople;
	UserOut.iCardCount = 0;
	
	for (int i=0;i<m_bMaxPeople;i++) 
		SendGameData(i,&UserOut,sizeof(UserOut),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);

	SendWatchData(m_bMaxPeople,&UserOut,sizeof(UserOut),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);

	SetTimer(TIME_OUT_CARD,(m_iThinkTime + 2)*1000);
	return TRUE;
}

//�Ƿ����һ݆
BOOL CServerGameDesk::IsNewTurn()
{
	if(m_iOutCardPeople == m_iFirstOutPeople)
	{
		for(int i = 0; i < PLAY_COUNT; i ++)
			SendGameData(i,MDM_GM_GAME_NOTIFY,ASS_ONE_TURN_OVER,0);

		SendWatchData(m_bMaxPeople,MDM_GM_GAME_NOTIFY,ASS_ONE_TURN_OVER,0);
	}

	if (m_iOutCardPeople == m_iNowBigPeople)			//���ȳ�����
	{
		//m_iOutCardPeople = -1;

		SetTimer(TIME_WAIT_NEWTURN,1000);
		return true;
	}
	return false;
}
//��һ�ֿ�ʼ
BOOL CServerGameDesk::NewPlayTurn(BYTE bDeskStation)
{
	m_iBaseOutCount = 0;
	m_byteHitPass = 0;
	m_iOutCardPeople = m_iFirstOutPeople = m_iNowBigPeople = bDeskStation;
	m_iTurnFirstOut = TRUE;

	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));

	NewTurnStruct turn;
	turn.bDeskStation = bDeskStation ;

	for (int i = 0; i < PLAY_COUNT; i ++) 
		SendGameData(i,&turn,sizeof(turn),MDM_GM_GAME_NOTIFY,ASS_NEW_TURN,0);
	SendWatchData(m_bMaxPeople,&turn,sizeof(turn),MDM_GM_GAME_NOTIFY,ASS_NEW_TURN,0);
	return TRUE;
}

//����������������
BOOL CServerGameDesk::IsAwardPoin(BYTE iOutCard[],int iCardCount,BYTE bDeskStation)
{
	if(m_Logic.IsKingBomb(iOutCard,iCardCount) || m_Logic.IsBomb(iOutCard,iCardCount))
	{
		m_iBase += 2;
		m_iBombMul += 2;
		m_iAwardPoint[bDeskStation] += 1;	
		if (m_Logic.IsKingBomb(iOutCard,iCardCount))
			m_iBombCard[1][0] = 1;
		else
			m_iBombCard[2][m_iBombCount] = m_Logic.GetCardNum(iOutCard[0]);

		m_iBombCount++;
		AwardPointStruct award;
		award.iAwardPoint = m_iAwardPoint[bDeskStation];
		award.bDeskStation = bDeskStation;
		award.iBase = m_iBase;
		//���ͽ������
		for (int i = 0;i < PLAY_COUNT; i++) 
			SendGameData(i,&award,sizeof(award),MDM_GM_GAME_NOTIFY,ASS_AWARD_POINT,0);
		SendWatchData(m_bMaxPeople,&award,sizeof(award),MDM_GM_GAME_NOTIFY,ASS_AWARD_POINT,0);
	}
	if (m_Logic.IsShamBomb(iOutCard,iCardCount))
	{
		m_iBase += 1;
		m_iBombMul += 1;
		m_iAwardPoint[bDeskStation] += 1;							

		BYTE card;
		for (int i=0; i<4; i++)
		{
			if (m_Logic.GetReplaceCardCount(&iOutCard[i], 1)==0)
			{
				card = iOutCard[i];
				break;
			}
		}
		m_iBombCard[3][m_iShamBomCount] = m_Logic.GetCardNum(card);
		m_iShamBomCount++;
		AwardPointStruct award;
		award.iAwardPoint = m_iAwardPoint[bDeskStation];
		award.bDeskStation = bDeskStation;
		award.iBase = m_iBase;
		//���ͽ������
		for (int i = 0;i < PLAY_COUNT; i++) 
			SendGameData(i,&award,sizeof(award),MDM_GM_GAME_NOTIFY,ASS_AWARD_POINT,0);
		SendWatchData(m_bMaxPeople,&award,sizeof(award),MDM_GM_GAME_NOTIFY,ASS_AWARD_POINT,0);
	}
	return true;
}

//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	//��д����
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			//�������� 
			m_bGameStation = GS_WAIT_NEXT;
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}

			BOOL bOutCard = GetNoOutCard();
			if (bOutCard)
			{
				m_iBombCard[0][0]=1;
				m_iBase += 1;
			}

			double t = pow((double)2.0,(int)m_iBase);
			int iTurePoint = m_iCallScore[m_iNtPeople];			
			if (m_iArrIsStartDisplay[m_iUpGradePeople])  //�������ƿ�ʼʱ
			{
				iTurePoint *= 2;
			}
            
			//��˭ʤ
			if(m_iUserCardCount[m_iUpGradePeople] == 0)		
				iTurePoint = iTurePoint;
			else 
				iTurePoint = - iTurePoint;
			//��Ϸ����
			GameEndStruct GameEnd;
			::memset(&GameEnd,0,sizeof(GameEnd));
			GameEnd.iCallScore = m_iCallScore[m_iNtPeople];							//�׷�
			GameEnd.iRobMul = m_iRobMul;											//������;
			GameEnd.iBombMul = m_iBombMul;												//ը��;
			GameEnd.iOutCardMul = (bOutCard?1:0);
			GameEnd.iTotalMul = t* GetRoomMul();											//��
			GameEnd.iUpGradeStation = m_iUpGradePeople;
			GameEnd.iBasePoint = GetRoomMul();
			GameEnd.iTax = m_pDataManage->m_InitData.uTax;
			if (m_iBaseMult != -1)
			{
				if (t > m_iBaseMult)
					t = m_iBaseMult;//�ⶥ����
			}

			for (int i=0; i<4; i++)
			{
				::CopyMemory(GameEnd.iBombCard[i], m_iBombCard[i], sizeof(m_iBombCard[i]));

			}
			memset(GameEnd.iTurePoint, 0, sizeof(GameEnd.iTurePoint));
			memset(GameEnd.iInitScore, 0, sizeof(GameEnd.iInitScore));
		
			//���ҵ÷�
			int iBaseCount = 0;						//�Ӱ���
			for(int i = 0; i < PLAY_COUNT ; i ++)
			{
				if(m_iPeopleMul[i] == 2)
					iBaseCount ++;
				if (i != m_iUpGradePeople && m_iArrIsStartDisplay[i]) //���ƿ�ʼ
				{
					iBaseCount ++;
					m_iPeopleMul[i] *= 2;
				}
			}

			for(int i = 0; i < PLAY_COUNT; i ++)
			{			
				if(i == m_iUpGradePeople)//����
				{
#ifdef MY_TWO
					GameEnd.iInitScore[i] = (1 + iBaseCount) * iTurePoint * t ;
#else
					GameEnd.iInitScore[i] = (2 + iBaseCount) * iTurePoint * t ;
#endif				

					GameEnd.iTurePoint[i] = GameEnd.iInitScore[i];

				}
				else//ũ��
				{
					//��ʵ�÷�
					GameEnd.iInitScore[i] = - iTurePoint*(int)t/**(bOutCard?2:1)*/*m_iPeopleMul[i];//�Ӱ�
					bool add =((m_iUserCardCount[i] == 0)&&m_pDataManage->m_InitData.uComType == TY_MATCH_GAME);         
					GameEnd.iTurePoint[i] = GameEnd.iInitScore[i] + (add ? 1: 0) ;//�ȳ����Ƶ�ũ���һ��
				}
			}
		    
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			ChangeUserPointint64(GameEnd.iTurePoint,temp_cut);
			__super::RecoderGameInfo(GameEnd.iChangeMoney);

			//����
			if(m_iBeenPlayGame >= m_iLimitPlayGame || TY_MATCH_GAME != m_pDataManage->m_InitData.uComType)//���ֽ���
			{
				m_bGameStation = GS_WAIT_ARGEE;

				bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0;//��ҳ����ۻ���add by xqm 2010-11-12
				if (true == bNotCostPoint)
				{
					memset(GameEnd.iTurePoint,0,sizeof(GameEnd.iTurePoint));
				}
				for (int i = 0; i < PLAY_COUNT; i ++) 
					SendGameData(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_NO_CONTINUE_END,0);
				SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_NO_CONTINUE_END,0);

				bCloseFlag = GFF_FORCE_FINISH;
				ReSetGameState(bCloseFlag);
				__super::GameFinish(bDeskStation,bCloseFlag);
				return TRUE;
			}
			else//��һ�ֿ�ʼ
			{
				bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0;//��ҳ����ۻ���add by xqm 2010-11-12
				if (true == bNotCostPoint)
				{
					memset(GameEnd.iTurePoint,0,sizeof(GameEnd.iTurePoint));
				}
				for (int i = 0; i < PLAY_COUNT; i ++) 
					SendGameData(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
				SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
				//�������� 
				m_bGameStation = GS_WAIT_NEXT;
			}

			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GF_NO_CALL_SCORE:
		{
			//�������� 
			m_bGameStation = GS_WAIT_NEXT;

			BOOL bhavecut = false;
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
				{
					if(m_pUserInfo[i]->m_UserData.bUserState == USER_CUT_GAME)
					{
						bhavecut = true;
						break;
					}
					m_pUserInfo[i]->m_UserData.bUserState = USER_ARGEE;
				}
			}
			if(bhavecut)
			{
				GameFinish(0,GF_SAFE);
				return true;
			}
			//��Ϸ����
			GameEndStruct GameEnd;
			::memset(&GameEnd,0,sizeof(GameEnd));
			//��������
			for (int i = 0; i < PLAY_COUNT; i ++) 
				SendGameData(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_NO_CALL_SCORE_END,0);
			SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_NO_CALL_SCORE_END,0);

			ReSetGameState(GF_NORMAL);
			GameBegin(ALL_ARGEE);
			return true;
		}
	case GFF_SAFE_FINISH:
	case GF_SAFE:			//��Ϸ��ȫ����
		{
			//��������
			m_bGameStation = GS_WAIT_ARGEE;//GS_WAIT_SETGAME;

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}
			GameCutStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));

			for (int i = 0; i < PLAY_COUNT; i ++) 
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SAFE_END,0);
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SAFE_END,0);

			bCloseFlag = GFF_FORCE_FINISH;
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}
	case GFF_FORCE_FINISH:		//�û������뿪
		{
			m_bGameStation = GS_WAIT_ARGEE;

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}
			GameCutStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));
			CutEnd.bDeskStation = bDeskStation;

			CutEnd.iTurePoint[bDeskStation] = - GetRunPublish();
			CutEnd.iTurePoint[(bDeskStation + 1)%PLAY_COUNT] = GetRunAwayOtherGetPoint((bDeskStation + 1)%PLAY_COUNT,bDeskStation);
			CutEnd.iTurePoint[(bDeskStation + 2)%PLAY_COUNT] = GetRunAwayOtherGetPoint((bDeskStation + 2)%PLAY_COUNT,bDeskStation);
			//CutEnd.iTurePoint[(bDeskStation+3)%4] = GetRunAwayOtherGetPoint((bDeskStation+3)%4,bDeskStation);

			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				temp_cut[i] = (bDeskStation == i && CutEnd.iTurePoint[i] < 0);
			}
			ChangeUserPointint64(CutEnd.iTurePoint, temp_cut/*,CutEnd.iChangeMoney,false*/);
			__super::RecoderGameInfo(CutEnd.iChangeMoney);

			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0;//��ҳ����ۻ���add by xqm 2010-11-12
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
			return TRUE;
		}
	case GF_TERMINATE://����ֹͣ��Ϸ
		{
			//�������� 
			m_bGameStation = GS_WAIT_ARGEE;
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}
			GameCutStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));
			for (int i = 0; i < PLAY_COUNT; i ++)
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_TERMINATE_END,0);
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_TERMINATE_END,0);

			bCloseFlag = GFF_FORCE_FINISH;
			ReSetGameState(bCloseFlag);
			__super::GameFinish(0,bCloseFlag);
			return true;
		}
	case GF_AHEAD_END://��ǰ����
		{
			//�������� 
			m_bGameStation = GS_WAIT_ARGEE;

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}
			GameCutStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));
			for (int i = 0; i < PLAY_COUNT; i ++)
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_AHEAD_END,0);
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_AHEAD_END,0);

			bCloseFlag = GFF_FORCE_FINISH;
			ReSetGameState(bCloseFlag);
			__super::GameFinish(0,bCloseFlag);
			return true;
		}
	}

	//��������
	ReSetGameState(bCloseFlag);
	__super::GameFinish(bDeskStation,bCloseFlag);
	return true;
}

//�������������ҵ÷�
int CServerGameDesk::GetRunAwayOtherGetPoint(BYTE bDeskStation,BYTE bRunDeskStation )
{
	return 0;
}

//���ܿ۷�
int CServerGameDesk::GetRunPublish()
{
	return m_pDataManage->m_InitData.uRunPublish;
	
}
//���ܿ۷�
int CServerGameDesk::GetRunPublish(BYTE bDeskStation)
{
	return m_pDataManage->m_InitData.uRunPublish;
}

//�Α���A����
int CServerGameDesk::GetRoomMul()
{
	return m_pDataManage->m_InitData.uBasePoint;//����
}

//���ӱ���
int CServerGameDesk::GetDeskBasePoint()
{
	return 1;
	//return GetTableBasePoint();
}
//ͳ�Ʊ���
int CServerGameDesk::GetHumanBasePoint()
{
	return 1;
	int minval = m_pUserInfo[0]->m_UserData.dwPoint;
	for(int i = 1; i < PLAY_COUNT;i ++)
	{
		minval = min(minval,m_pUserInfo[i]->m_UserData.dwPoint);
	}
	return minval;
}

//������һ��ׯ��,����һ������λ��,�з�λ��
BOOL CServerGameDesk::SetNextBanker(BYTE bGameFinishState)
{
	switch(bGameFinishState)
	{
	case GF_NORMAL://������������ʤ����һ�����Ƚз�
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if(m_iUserCardCount[i] == 0)
			{
				m_iDealPeople = m_iUpGradePeople = i;
				break;
			}
		}
		break;
	case GF_NO_CALL_SCORE://���˽з�
#ifdef TWO
		m_iDealPeople = m_iUpGradePeople =  (m_iUpGradePeople +1)%PLAY_COUNT;
#else
		m_iDealPeople = m_iUpGradePeople =  (m_iUpGradePeople +2)%PLAY_COUNT;
#endif
		break;
	default:
		m_iDealPeople = m_iUpGradePeople = -1;
	}
	return TRUE;
}

//����ʤ��
BYTE CServerGameDesk::GetGameWiner(BOOL &bCut)
{
	int winer = 0;
	for(int i = 1;i < PLAY_COUNT ; i ++)
	{
		if(m_pUserInfo[i] == NULL)
			continue;
		if(m_iWinPoint[i] > m_iWinPoint[winer])
		{
			winer = i;
		}
	}
	//�������,��һ���Ƕ�����Ҵ���
	if(m_pUserInfo[winer]!=NULL&&m_pUserInfo[winer]->m_UserData.bUserState  == USER_CUT_GAME || m_iAIStation[winer])
	{
		bCut = TRUE;
		if(m_iWinPoint[(winer+1)%PLAY_COUNT] > m_iWinPoint[(winer+2)%PLAY_COUNT])
			if(m_pUserInfo[(winer+1)%PLAY_COUNT] != NULL&&m_pUserInfo[(winer+1)%PLAY_COUNT]->m_UserData.bUserState  != USER_CUT_GAME  && !m_iAIStation[(winer+1)%PLAY_COUNT])
				return (winer+1)%PLAY_COUNT;

		if(m_pUserInfo[(winer+2)%PLAY_COUNT] != NULL&&m_pUserInfo[(winer+2)%PLAY_COUNT]->m_UserData.bUserState  != USER_CUT_GAME)
			return (winer+2)%PLAY_COUNT;
	}

	return winer;
}

//��ȡ��ʵ��(ÿһ�������ʵ��
int CServerGameDesk::GetTruePoint(BYTE bDeskStation)
{
	return 0;
}


//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if((m_bGameStation >= GS_SEND_CARD && m_bGameStation < GS_WAIT_NEXT))
		return true;
	return false;
}

//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGameEx(BYTE bDeskStation)
{
	return true;
}
//�û���������
bool CServerGameDesk::UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo)
{
	UserSetAuto(bDeskStation,FALSE);
	return __super::UserReCome(bDeskStation, pNewUserInfo);
}
//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	//if(m_bGameStation > GS_WAIT_ARGEE && m_pDataManage->m_InitData.uComType != TY_MATCH_GAME)
	//{
	//	GameFinish(bDeskStation,GF_SAFE);
	//	//		m_bGameStation=GS_WAIT_ARGEE;
	//}
	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}


//�û�������Ϸ
BOOL CServerGameDesk::UserSetGame(UserArgeeGame * pGameSet)
{
	return TRUE;
}


//�û������뿪
BOOL CServerGameDesk::UserHaveThing(BYTE bDeskStation, char * szMessage)
{
	if (m_bGameStation < GS_SEND_CARD) 
		return TRUE;
	if (bDeskStation!=m_iHaveThingPeople)
	{
		m_iLeaveArgee =0;
	}
	m_icountleave=0;
	m_iHaveThingPeople = bDeskStation;
	m_iLeaveArgee|= 1<<bDeskStation;
	if (m_iLeaveArgee != 7)
	{
		HaveThingStruct HaveThing;
		HaveThing.pos = bDeskStation;
		::CopyMemory(HaveThing.szMessage,szMessage,60*sizeof(char));

		for (int i = 0; i < PLAY_COUNT; i ++)
			if (i != bDeskStation)
				SendGameData(i,&HaveThing,sizeof(HaveThing),MDM_GM_GAME_NOTIFY,ASS_HAVE_THING,0);
	}
	else 
		GameFinish(bDeskStation,GF_SAFE);
	return TRUE;
}

//ͬ���û��뿪
BOOL CServerGameDesk::ArgeeUserLeft(BYTE bDeskStation, BOOL bArgee)
{
	m_icountleave++;
	if (bArgee) m_iLeaveArgee|= 1<<bDeskStation;
	else 
	{
		m_iLeaveArgee = 0;//m_iLeaveArgee&=~1<<bDeskStation;
	}
	if (m_iLeaveArgee != 7)				//3������Ϸ
	{
		LeaveResultStruct Leave;
		Leave.bDeskStation = bDeskStation;
		Leave.bArgeeLeave = bArgee;
		for (int i = 0; i < PLAY_COUNT; i ++) 
			if (i != bDeskStation) 
				SendGameData(i,&Leave,sizeof(Leave),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	}
	else
	{
		GameFinish(m_iHaveThingPeople,GF_SAFE);
		LeaveResultStruct Leave1;
		Leave1.bDeskStation = m_iHaveThingPeople;
		Leave1.bArgeeLeave = true;
		SendGameData(m_iHaveThingPeople,&Leave1,sizeof(Leave1),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	}
	if (m_icountleave>=2)
	{
		m_iLeaveArgee=0;
	}

	return TRUE;
}

//�й�����
bool CServerGameDesk::UserSetAuto(BYTE bDeskStation,bool bAutoCard)
{
	m_bAuto[bDeskStation] = bAutoCard;
	AutoStruct autoset;
	autoset.bAuto = bAutoCard;
	autoset.bDeskStation = bDeskStation;

	for(int i = 0; i < PLAY_COUNT; i ++)
		SendGameData(i,&autoset,sizeof(autoset),MDM_GM_GAME_NOTIFY,ASS_AUTO,0);
	SendWatchData(m_bMaxPeople,&autoset,sizeof(autoset),MDM_GM_GAME_NOTIFY,ASS_AUTO,0);
	return true;
}
////����
//bool CServerGameDesk::UserProp(BYTE bsendDeskStation,BYTE brecvDeskStation,int iValue)
//{
//	PropStruct stProp;
//	stProp.sendDeskStation = bsendDeskStation;
//	stProp.recvDeskStation = brecvDeskStation;
//	stProp.iValue = iValue;
//
//	for(int i = 0; i < PLAY_COUNT; i ++)
//		SendGameData(i,&stProp,sizeof(stProp),MDM_GM_GAME_NOTIFY,ASS_PROP,0);
//	SendWatchData(m_bMaxPeople,&stProp,sizeof(stProp),MDM_GM_GAME_NOTIFY,ASS_PROP,0);
//	return true;
//}
//�Զ�����
BOOL CServerGameDesk::UserAutoOutCard(BYTE bDeskStation)
{
	BYTE bCardList[54];
	int iCardCount = 0;
	if(bDeskStation == m_iFirstOutPeople)
	{
		m_Logic.AutoOutCard(m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation],
			m_iDeskCard[m_iFirstOutPeople],m_iDeskCardCount[m_iFirstOutPeople],
			bCardList,iCardCount,bDeskStation == m_iFirstOutPeople);
	}
	else
	{
		iCardCount = 0;
	}
	return UserOutCard(bDeskStation,bCardList,iCardCount,bCardList,false);
	//return 0;//��Ч����,
}


//����ȡ���������й�
BOOL CServerGameDesk::SetAIMachine(BYTE bDeskStation,BOOL bState)
{
	m_iAIStation[bDeskStation] = bState;

	UseAIStation ai;
	ai.bDeskStation = bDeskStation;
	ai.bState = bState;
	for(int i = 0; i < PLAY_COUNT ;i++)
	{
		if(i != bDeskStation &&m_pUserInfo[i] != NULL)		
			SendGameData(i,&ai,sizeof(ai),MDM_GM_GAME_NOTIFY,ASS_AI_STATION,0);	
	}
	SendWatchData(m_bMaxPeople,&ai,sizeof(ai),MDM_GM_GAME_NOTIFY,ASS_AI_STATION,0);

	return TRUE;
}


//�û������뿪
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	__super::UserNetCut(bDeskStation,  pLostUserInfo);
	
	return UseAI(bDeskStation);
}


//�Ƿ�Ϊδ������
BYTE CServerGameDesk::GetNoOutCard()
{
	BOOL bOutCard = FALSE;
	//���м�û�г�����
	if(m_iUserCardCount[(m_iUpGradePeople + 1) % PLAY_COUNT] == m_iUserCardCount[(m_iUpGradePeople + 2) % PLAY_COUNT]
	&&m_iUserCardCount[(m_iUpGradePeople + 2) % PLAY_COUNT] == 17)
	{
		bOutCard = true;
	}
#ifdef TWO
		if(m_iUserCardCount[(m_iUpGradePeople + 1) % PLAY_COUNT] == 17)
	{
		bOutCard = true;
	}
#endif
	//ׯ�ҽ����˵�һ����
	if(m_iUserCardCount[m_iUpGradePeople] == 20 - m_iNtFirstCount)		
	{
		bOutCard = true;
	}
	return bOutCard;
}
