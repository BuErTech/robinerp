#include "StdAfx.h"
#include "ServerManage.h"
#include "writelog.h"
#include <math.h>
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
BYTE CServerGameDesk:: m_iSendCardTime ; 
BYTE CServerGameDesk:: m_iCallScoreTime;
BYTE CServerGameDesk:: m_iRobNTTime ; 
BYTE CServerGameDesk:: m_iAddDoubleTime;
BOOL CServerGameDesk:: m_bTurnRule;		//˳ʱ�뻹����ʱ��


void DebugPrintf(const char *p, ...)
{
	char szFilename[256];
	sprintf( szFilename, "E:\\bzw\\DebugInfo\\%dServer.txt", NAME_ID);
	FILE *fp = fopen( szFilename, "a" );
	if (NULL == fp)
	{
		return;
	}
	va_list arg;
	va_start( arg, p );
	vfprintf(fp, p, arg );
	fprintf(fp,"\n");
	fclose(fp);
}
//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(FULL_BEGIN)
{
	//c++ test
	::ZeroMemory(m_iUserCardCount,sizeof(m_iUserCardCount));
	::ZeroMemory(m_iUserCard,sizeof(m_iUserCard));
	::ZeroMemory(m_iBackCard,sizeof(m_iBackCard));
	::ZeroMemory(m_iDeskCard,sizeof(m_iDeskCard));

	//�ѳ�ʼ��
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		m_iLastCardCount[i] = 0;
		m_iAwardPoint[i] = 0;
		m_iWinPoint[i] = 0;
	}
	::ZeroMemory(m_iLastOutCard, 0, PLAY_COUNT*45);
	m_iRecvMsg = 0;
	m_iGameFlag = GS_FLAG_NORMAL;
	m_iUpGradePeople = -1;
	m_iLeaveArgee = 0;
	m_iNtFirstCount = 0;		//��һ��������
	//	m_iNowCallScorePeople = -1;		//
	m_iBaseOutCount = 0;
	m_iDealPeople = -1;
	m_iFirstOutPeople = -1;
	m_iNowBigPeople = -1;
	m_iOutCardPeople = -1;
	m_iBeenPlayGame = 0;
	m_bGameStation = GS_WAIT_ARGEE;
	m_bThrowoutCard = 0x00;
	m_bFirstCallScore = 255;
	m_iPrepareNT = 255;		//Ԥ��ׯ��
	m_iFirstShow = 255 ; 
	m_iCurrentRobPeople = 255 ;
	m_iFirstRobNt = 255 ; 
	m_bHaveSendBack = false;

	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));	
	::memset(m_iAIStation,0,sizeof(m_iAIStation));		//�����й�
	::memset(m_bAuto,0,sizeof(m_bAuto));				//�й�
	::memset(m_iCallScore,-1,sizeof(m_iCallScore));		//�з�
	::memset(m_bCanleave,1,sizeof(m_bCanleave));
	::memset(m_bUserReady , 0 , sizeof(m_bUserReady)) ; 
	::memset(m_iRobStation , 0  ,sizeof(m_iRobStation)) ; 
	::memset(m_iAddStation , 0 , sizeof(m_iAddStation)) ; 

	::memset(m_byPass , 0 ,sizeof(m_byPass)) ; 
	::memset(m_byLastTurnPass , 0 , sizeof(m_byLastTurnPass)) ; 

	m_bySendFinishCount = 0 ; 

	m_iFirstCutPeople = 255;
	m_icountleave = 0;

	m_iBaseMult = 1 ;
	m_iAddDoubleLimit = 0 ;
	m_iGameMaxLimit = 0 ; 
	m_iLimitPoint = 0 ; 

	LoadIni();

	m_GameMutiple.IniData(m_iBaseMult) ; 

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
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +nid +"_s.ini");
	CString key = TEXT("game");
	m_bHaveKing = f.GetKeyVal(key,"haveking",1);
	m_iPlayCard = f.GetKeyVal(key,"card",1);
	m_iPlayCount = f.GetKeyVal(key,"cardcount",54);

	m_iSendCount = f.GetKeyVal(key,"sendcard",51);

	m_iBackCount = f.GetKeyVal(key,"backcard",3);
	m_iUserCount = f.GetKeyVal(key,"usercount",17);

	m_iLimitUpGrade = f.GetKeyVal(key,"limitupgrade",5);
	m_bKingCanReplace = f.GetKeyVal(key,"kingcanreplace",0);
	key=TEXT("other");
	m_iBeginTime = f.GetKeyVal(key,"begintime",15);
	m_iThinkTime = f.GetKeyVal(key,"thinktime",20);

	m_iSendCardTime = f.GetKeyVal(key,"SendCardTime",6);
	m_iRobNTTime    = f.GetKeyVal(key,"RobNTTime",10);
	m_iCallScoreTime = f.GetKeyVal(key,"CallScoretime",15);
	m_iAddDoubleTime = f.GetKeyVal(key,"adddoubletime",5);

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
	m_iCardShape |= ((f.GetKeyVal(key,"kingbomb",1)<<20)&0xFFFFFFFF);		//��ը
	return true;
}

//���ؾ�������
BOOL CServerGameDesk::LoadExtIni()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CString nid;
	nid.Format("%d",NAME_ID);
	CINIFile f( s +nid +"_s.ini");

	CString key = "game";

	m_bTurnRule = f.GetKeyVal(key,"TurnRule",1);
	m_iLimitPlayGame = f.GetKeyVal(key,"limitplay",1);
	m_bRobnt = f.GetKeyVal(key,"robnt",1);
	m_bAdddouble=f.GetKeyVal(key,"adddouble",1);
	m_bShowcard=f.GetKeyVal(key,"showcard",1);
	m_iBaseMult = f.GetKeyVal(key , "BaseMutiple" , 15) ; 

	if(m_iBaseMult <=0)
	{
		m_iBaseMult = 1 ; 
	}

	m_iAddDoubleLimit = f.GetKeyVal(key , "AddDoubleLimit" , 0) ; 
	m_iGameMaxLimit  = f.GetKeyVal(key , "GameMaxLimit" , 0) ; 
	m_iLimitPoint  = f.GetKeyVal(key , "LimitPoint" , 0) ; 

	

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
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +nid +"_s.ini");
 

	m_iLimitPlayGame = f.GetKeyVal(key,"limitplay",m_iLimitPlayGame);
	m_bRobnt = f.GetKeyVal(key,"robnt",m_bRobnt);
	m_bAdddouble=f.GetKeyVal(key,"adddouble",m_bAdddouble);
	m_bShowcard=f.GetKeyVal(key,"showcard",m_bShowcard);

	m_bTurnRule = f.GetKeyVal(key,"TurnRule",m_bTurnRule);
	m_iBaseMult = f.GetKeyVal(key , "BaseMutiple" , m_iBaseMult) ; 

	m_iAddDoubleLimit = f.GetKeyVal(key , "AddDoubleLimit" , m_iAddDoubleLimit) ; 
	m_iGameMaxLimit  = f.GetKeyVal(key , "GameMaxLimit" , m_iGameMaxLimit) ;
	m_iLimitPoint  = f.GetKeyVal(key , "LimitPoint" , m_iLimitPoint) ; 



	return TRUE;
}
//�O���Α��B
BOOL CServerGameDesk::InitThisGame()
{
	m_Logic.SetKingCanReplace(m_bKingCanReplace);
	m_Logic.SetCardShape(m_iCardShape);
	return true;
}
bool CServerGameDesk::InitDeskGameStation()
{
	LoadExtIni();
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);

	m_GameMutiple.IniData(m_iBaseMult) ; 

	return true;
}
//�������ǿ����Ϣ
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	if (bDeskStation>=PLAY_COUNT)
	{
		return true;
	}

	ASSERT(pNetHead->bMainID==MDM_GM_GAME_FRAME);

	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:
		{
			bool  bForceQuitAsAuto = true ; 
			///�����ϲ�·��
			CString szAppPath = CINIFile::GetAppPath();

			CString strBCFFile  ; 
			CString strKeyName  ; 
			strKeyName.Format("%d" ,NAME_ID) ; 
			strBCFFile.Format("%s\\SpecialRule.bcf",szAppPath) ; 

			CBcfFile File(strBCFFile) ;

			if (File.IsFileExist(strBCFFile))
			{
				bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto",strKeyName ,0) ; 
			}

			CString strInfo ; 
			strInfo.Format("wysoutlog::bForceQuitAsAuto===%d " ,bForceQuitAsAuto) ; 
			OutputDebugString(strInfo) ; 

			if(bForceQuitAsAuto == true)
			{
				return true ; 
			}
		}
		break;
	default:
		break;
	}
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
		GameFinish(255,/*GFF_SAFE_FINISH*/GFF_FORCE_FINISH);
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
				{
					return true;
				}
				if(m_bGameStation != GS_WAIT_NEXT)
				{
					m_bGameStation = GS_WAIT_ARGEE;
				}

				if(m_bUserReady[bDeskStation])
				{
					return true; 
				}

				m_bUserReady[bDeskStation] = true ; 

				if(uSize == sizeof(UserMingStruct))
				{
					UserMingStruct *pMingPai = (UserMingStruct *)pData ; 
				
					m_GameMutiple.sMingPaiMutiple[bDeskStation] = 5 ;  ///����5��

					if(pMingPai->bMing  == true && 255 == m_iFirstShow)
					{
						m_iFirstShow = bDeskStation ; 
					}
				}
		
				return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);

			}
		case ASS_SEND_FINISH:
			{
				m_bySendFinishCount ++ ; 

				if(m_bySendFinishCount >= PLAY_COUNT)
				{
					KillTimer(TIME_SEND_CARD_ANI) ; 
					KillTimer(TIME_SEND_CARD_FINISH) ;
					SetTimer(TIME_SEND_CARD_FINISH , 300) ; 
				}
				return true ; 
			}
		case ASS_CALL_SCORE:
			{
				if (bWatchUser)
				{
					return true;
				}
				if(uSize != sizeof(CallScoreStruct))
				{
					return true ; 
				}

				CallScoreStruct * pCallScore=(CallScoreStruct *)pData;
				UserCallScore(bDeskStation,pCallScore->iValue);
				return true;
			}
		case ASS_ROB_NT:						//������
			{

				if(bWatchUser)
				{
					return true;
				}
				if(uSize != sizeof(RobNTStruct))
				{
					return true ; 
				}

				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
				{
					return true;
				}

				RobNTStruct * pRobNT=(RobNTStruct *)pData;
				UserRobNT(pRobNT->bDeskStation,pRobNT->iValue);
				return TRUE;
			}
		case ASS_ADD_DOUBLE:					//�Ӱ�
			{
				if(bWatchUser)
				{
					return true;
				}
				if(uSize != sizeof(AddDoubleStruct))
				{
					return true ; 
				}

				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
				{
					return true;
				}

				AddDoubleStruct * pAddDouble=(AddDoubleStruct *)pData;
				UserAddDouble(pAddDouble->bDeskStation,pAddDouble->iValue);
				return TRUE;
			}
		case ASS_SHOW_CARD:	//����
			{
				if(bWatchUser)
				{
					return true;
				}
				if(uSize != sizeof(ShowCardStruct))
				{
					return true ; 
				}

				if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
				{
					return true;
				}

				ShowCardStruct * pShowCard=(ShowCardStruct *)pData;
				UserShowCard(pShowCard->bDeskStation,pShowCard->iValue);
				return true;
			}
		case ASS_OUT_CARD:		//�û�����
			{
				if(bWatchUser)
				{
					return true;
				}
				if(uSize != sizeof(OutCardStruct))
				{
					return true ; 
				}

				if(m_bGameStation != GS_PLAY_GAME)
				{
					return true;
				}

				OutputDebugString("wysoutdebugSever::ASS_Out_card") ; 

				OutCardStruct * pOutCard=(OutCardStruct *)pData;
				UserOutCard(bDeskStation,pOutCard->iCardList,pOutCard->iCardCount);
				return true;
			}
		case ASS_AUTO:			//�й�
			{
				if (bWatchUser)
				{
					return true ; 
				}
				if(uSize!=sizeof(AutoStruct))
				{
					return true ; 
				}

				AutoStruct * pAuto = (AutoStruct*)pData;
				UserSetAuto(bDeskStation,pAuto->bAuto);
				return true;
			}
		case ASS_HAVE_THING:	//���������뿪
			{
				if(bWatchUser)
				{
					return true;
				}
				if(uSize !=sizeof(HaveThingStruct))
				{
					return true ; 
				}

				HaveThingStruct * pThing=(HaveThingStruct *)pData;
				UserHaveThing(bDeskStation,pThing->szMessage);
				return true;
			}
		case ASS_LEFT_RESULT: //ͬ���û��뿪
			{
				if(bWatchUser)
				{
					return true;
				}
				if(uSize != sizeof(LeaveResultStruct))
				{
					return true ; 
				}

				LeaveResultStruct * pLeave=(LeaveResultStruct *)pData;
				ArgeeUserLeft(bDeskStation,pLeave->bArgeeLeave);
				return true;
			}
		default: 
			{
				break; 
			}
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}
//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	//JAdd-20090113-//ȡ��1M����ʱ�˳�
	if ((uTimerID>=IDT_USER_CUT)&&(uTimerID<(UINT)(IDT_USER_CUT+m_bMaxPeople)))
	{
		if (m_bGameStation>=20&&m_bGameStation<23)
		{
			return true;
		}
	}
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
	case TIME_SEND_CARD_ANI:					///�ͻ��˷��ƶ�ʱ��
		{
			KillTimer(TIME_SEND_CARD_ANI) ; 

			SetTimer(TIME_SEND_CARD_FINISH , 300) ; 
			
			break;
		}
	case  TIME_SEND_CARD_FINISH:
		{
			KillTimer(TIME_SEND_CARD_FINISH) ; 
			SendCardFinish();
			break;
		}
	case TIME_JIAO_FEN:                // �зֶ�ʱ��
		{
			KillTimer(TIME_JIAO_FEN);

			if (GS_WAIT_BACK == m_bGameStation && GS_FLAG_CALL_SCORE == m_iGameFlag)
			{
				// �Զ����з�
				UserCallScore(m_bCurrentCallScore, 0);
			}

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
	case TIME_ROB_NT:
		{
			KillTimer(TIME_ROB_NT);

			if (m_bGameStation == GS_WAIT_BACK)
			{
				UserRobNT(m_iCurrentRobPeople,0);
			}
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
	case TIME_SHOW_CARD:
		{
			KillTimer(TIME_SHOW_CARD);
			if (m_bGameStation == GS_WAIT_BACK)
			{
				UserShowCard(m_iUpGradePeople,0);
			}
		}
	}
	return __super::OnTimer(uTimerID);
}

//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	/// �����������ʾ�����й�״̬���������������Ϊ���й�
	if (!bWatchUser)
	{
		m_bAuto[bDeskStation] = false;
	}
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
			GameStation.iCardShape = m_iCardShape;						//��������
			//����ʱ��
			GameStation.iBeginTime = m_iBeginTime;				//��Ϸ��ʼʱ������
			GameStation.iThinkTime = m_iThinkTime;				//��Ϸ˼��ʱ��
			GameStation.iCallScoreTime = m_iCallScoreTime;			//�з�ʱ��
			GameStation.iRobNTTime = m_iRobNTTime ; 
			GameStation.iAddDoubleTime = m_iAddDoubleTime;			//�з�ʱ��
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();	//���ӱ���
			GameStation.iRoomBasePoint = GetRoomMul();	//���䱶��
			GameStation.iRunPublish = GetRunPublish();			//���ܿ۷�

			GameStation.iGameMutiple = m_iBaseMult ; 
			GameStation.iAddDoubleLimit = m_iAddDoubleLimit ; 
			GameStation.iGameMaxLimit = m_iGameMaxLimit ; 

			memcpy(GameStation.bUserReady ,m_bUserReady , sizeof(GameStation.bUserReady )) ;
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	case GS_SEND_CARD:		//����״̬
	case GS_WAIT_BACK:		//�ȴ������״̬
		{
			//����ȡ���������й�
			GameStation_3 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion  = DEV_HEIGHT_VERSION;			//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;			//�Ͱ汾
	    	//����ʱ��
			GameStation.iBeginTime = m_iBeginTime;				//��Ϸ��ʼʱ������
			GameStation.iThinkTime = m_iThinkTime;				//��Ϸ˼��ʱ��
			GameStation.iCallScoreTime = m_iCallScoreTime;			//�з�ʱ��
			GameStation.iRobNTTime = m_iRobNTTime ; 
			GameStation.iAddDoubleTime = m_iAddDoubleTime;			//�з�ʱ��
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();	//���ӱ���
			GameStation.iRoomBasePoint = GetRoomMul();	//���䱶��
			GameStation.iRunPublish    = GetRunPublish();			//���ܿ۷�

			GameStation.iCardShape = m_iCardShape;						//��������
			GameStation.iGameMutiple = m_iBaseMult ;
			GameStation.iAddDoubleLimit = m_iAddDoubleLimit ; 
			GameStation.iGameMaxLimit = m_iGameMaxLimit ; 
		
			GameStation.iUpGradePeople = m_iUpGradePeople;		
			GameStation.iGameFlag   = m_iGameFlag;	
		
			memcpy(GameStation.bCanleave, m_bCanleave, sizeof(m_bCanleave));
			memcpy(GameStation.bAuto , m_bAuto , sizeof(m_bAuto)) ;
			
			int iPos = 0;
			ZeroMemory(&GameStation.iUserCardList, sizeof(GameStation.iUserCardList));
			for(int i = 0; i < PLAY_COUNT; i ++)
			{
				GameStation.iUserCardCount[i] = m_iUserCardCount[i];
				/// ֻ���Լ������ƣ�Modified by zxd 20100314
				if (bDeskStation == i||m_GameMutiple.sMingPaiMutiple[i] >0)
				{
					::CopyMemory(&GameStation.iUserCardList[iPos],m_iUserCard[i],sizeof(BYTE)*m_iUserCardCount[i]);
				}
				iPos += m_iUserCardCount[i];
			}

			GameStation.iBackCardCount = m_iBackCount;
			memset(&GameStation.iGameBackCard , 0 , sizeof(GameStation.iGameBackCard)) ; 

			switch(m_iGameFlag)
			{
			case  GS_FLAG_ROB_NT:
				{
					for(int  i = 0 ; i<PLAY_COUNT ; i++)
					{
						if(m_iFirstRobNt!= 255) 
						{
							if(m_iRobStation[i] == 255) ///����
							{
								GameStation.iRobNT[i] = 2 ; 
							}
							else if(m_iRobStation[i] >0 &&m_GameMutiple.sRobNtMutiple[bDeskStation] >0)
							{
								GameStation.iRobNT[i] = 3 ; 
							}
							else
							{
								GameStation.iRobNT[i] = -1 ; 
							}

						}
						else
						{
							if(m_iRobStation[i] == 255) ///����
							{
								GameStation.iRobNT[i] = 0 ; 
							}
							else if(m_iRobStation[i] >0) ///�е���
							{
								GameStation.iRobNT[i] = 3 ; 
							}
							else ///û�н�
							{
								GameStation.iRobNT[i] = -1 ; 
							}
						}
					}

					GameStation.iCurOperator = m_iCurrentRobPeople ;  ///��ǰ�е�������

					break;
				}
			case  GS_FLAG_ADD_DOUBLE:
				{
					for(int  i = 0 ; i<PLAY_COUNT ; i++)
					{
						if(m_iAddStation[i] == 255)  //���ӱ�
						{
							GameStation.iUserDoubleValue[i] = -1 ; 
						}
						else
						{
							GameStation.iUserDoubleValue[i] = m_iAddStation[i] ;
						}
					}
					::CopyMemory(&GameStation.iGameBackCard,m_iBackCard,sizeof(BYTE)*m_iBackCount);
					GameStation.iUpGradePeople = m_iUpGradePeople ; 
					break ; 
				}
			case  GS_FLAG_SHOW_CARD:
				{
					::CopyMemory(&GameStation.iGameBackCard,m_iBackCard,sizeof(BYTE)*m_iBackCount);

					GameStation.iUpGradePeople = m_iUpGradePeople ;
					break;
				}
			default: 
				{
					break;
				}
			}
		
			GameStation.gameMutiple = m_GameMutiple ; 
			m_Logic.SetGameTask(GameStation.gameTask);

			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			
			return TRUE;
		}

	case GS_PLAY_GAME:	//��Ϸ��״̬
		{
			GameStation_4 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;			//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;			//�Ͱ汾
			//��Ϸ��������
			GameStation.iCardShape = m_iCardShape;						//��������
			//����ʱ��
			GameStation.iBeginTime = m_iBeginTime;				//��Ϸ��ʼʱ������
			GameStation.iThinkTime = m_iThinkTime;				//��Ϸ˼��ʱ��
			GameStation.iCallScoreTime = m_iCallScoreTime;			//�з�ʱ��
			GameStation.iRobNTTime = m_iRobNTTime ; 
			GameStation.iAddDoubleTime = m_iAddDoubleTime;			//�з�ʱ��
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();	//���ӱ���
			GameStation.iRoomBasePoint = GetRoomMul();	//���䱶��
			GameStation.iRunPublish = GetRunPublish();			//���ܿ۷�

			GameStation.iGameMutiple = m_iBaseMult ; 
			GameStation.iAddDoubleLimit = m_iAddDoubleLimit ; 
			GameStation.iGameMaxLimit = m_iGameMaxLimit ; 

			GameStation.iUpGradePeople = m_iUpGradePeople;		
			
			GameStation.iCallScoreResult = m_iCallScore[m_iUpGradePeople];	//��ǰ�з�
			GameStation.iOutCardPeople = m_iOutCardPeople;	
			GameStation.iFirstOutPeople = m_iFirstOutPeople;	
			GameStation.iBigOutPeople = m_iNowBigPeople;	
			//��Ϸ����	
			memcpy(GameStation.bCanleave, m_bCanleave, sizeof(m_bCanleave));
			memcpy(GameStation.bAuto , m_bAuto , sizeof(m_bAuto)) ;

			memcpy(GameStation.bPass , m_byPass , sizeof(GameStation.bPass)) ; 
			memcpy(GameStation.bLastTurnPass ,m_byLastTurnPass , sizeof(GameStation.bLastTurnPass)) ;
		
			//���ø���������
			int iPos = 0;
			ZeroMemory(&GameStation.iUserCardList, sizeof(GameStation.iUserCardList));
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				//�����û�������
				GameStation.iUserCardCount[i] = m_iUserCardCount[i];

				if (i == bDeskStation ||m_GameMutiple.sMingPaiMutiple[i] >0)
				{
					::CopyMemory(&GameStation.iUserCardList[iPos],m_iUserCard[i],sizeof(BYTE)*m_iUserCardCount[i]);
				}

				iPos += m_iUserCardCount[i];
			}

			//�����û���������
			GameStation.bIsLastCard = m_bIsLastCard;

			GameStation.iBaseOutCount = m_iBaseOutCount;	

			::CopyMemory(&GameStation.iBaseCardList , m_iDeskCard[m_iNowBigPeople] , sizeof(BYTE)*m_iBaseOutCount ) ; 
			::CopyMemory(&GameStation.iDeskCardCount, m_iDeskCardCount, sizeof(GameStation.iDeskCardCount));
			::CopyMemory(GameStation.iDeskCardList, m_iDeskCard, sizeof(GameStation.iDeskCardList));

			::CopyMemory(&GameStation.iLastCardCount, m_iLastCardCount, sizeof(GameStation.iLastCardCount));
			::CopyMemory(GameStation.iLastOutCard, m_iLastOutCard, sizeof(GameStation.iLastOutCard));
	
			//�Ƿ񲻳�
			GameStation.bIsPass = m_byteHitPass;

			GameStation.iBackCardCount = m_iBackCount;

			::CopyMemory(&GameStation.iGameBackCard,m_iBackCard,sizeof(BYTE)*m_iBackCount);
					
			GameStation.gameMutiple = m_GameMutiple ; 		

			m_Logic.SetGameTask(GameStation.gameTask);
			
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));

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
			//����ʱ��
			GameStation.iBeginTime = m_iBeginTime;				//��Ϸ��ʼʱ������
			GameStation.iThinkTime = m_iThinkTime;				//��Ϸ˼��ʱ��
			GameStation.iCallScoreTime = m_iCallScoreTime;			//�з�ʱ��
			GameStation.iRobNTTime = m_iRobNTTime ; 
			GameStation.iAddDoubleTime = m_iAddDoubleTime;			//�з�ʱ��
			GameStation.iCardShape = m_iCardShape;						//��������

			GameStation.iGameMutiple = m_iBaseMult ; 
			GameStation.iAddDoubleLimit = m_iAddDoubleLimit ; 
			GameStation.iGameMaxLimit = m_iGameMaxLimit ; 
		
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();	//���ӱ���
			GameStation.iRoomBasePoint = GetRoomMul();	//���䱶��
			GameStation.iRunPublish = GetRunPublish();			//���ܿ۷�

			memcpy(GameStation.bUserReady ,m_bUserReady , sizeof(GameStation.bUserReady )) ;

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
	KillTimer(TIME_SEND_CARD_ANI);
	KillTimer(TIME_GAME_FINISH);
	KillTimer(TIME_WAIT_NEWTURN);
	KillTimer(TIME_OUT_CARD);
	KillTimer(TIME_START_GAME);
	KillTimer(TIME_ADD_DOUBLE);
	KillTimer(TIME_ROB_NT);
	KillTimer(TIME_SHOW_CARD);
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
		m_iDealPeople=-1;
	}

	m_iPrepareNT = 255;		//Ԥ��ׯ��
	m_iCurrentRobPeople = 255 ;
	m_iFirstRobNt = 255 ; 
	m_iFirstShow = 255 ; 
	m_iUpGradePeople = -1;

	m_iFirstCutPeople = 255;

	for(int i = 0;i < PLAY_COUNT; i ++)
	{
		m_iAwardPoint[i]=0;
	}
	m_iGameFlag = GS_FLAG_NORMAL;
	m_bCurrentCallScore = 255;
	m_iNtFirstCount=0;		//ׯ�ҳ��ĵ�һ��������
	m_iLeaveArgee=0;
	m_iBaseOutCount=0;
	m_iNowBigPeople=-1;
	m_iOutCardPeople=-1;
	m_iSendCardPos=0;
	m_bFirstCallScore = 255;
	m_bHaveSendBack = false;

	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));
	::memset(m_iAIStation,0,sizeof(m_iAIStation));		//�����й�
	::memset(m_bAuto,0,sizeof(m_bAuto));				//�й�
	::memset(m_iCallScore,-1,sizeof(m_iCallScore));		//�з�
	::memset(m_iRobStation , 0  ,sizeof(m_iRobStation)) ; 
	::memset(m_iAddStation , 0 , sizeof(m_iAddStation)) ; 

	m_GameMutiple.IniData(m_iBaseMult) ; 

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
	{
		GameFinish(0,GF_SAFE);
		return false;
	}

	::ZeroMemory(m_iAIStation,sizeof(m_iAIStation));//��մ��������20090218 By Shao

	m_icountleave = 0;
	m_iSendCardPos = 0;
	m_bySendFinishCount = 0 ;  
	m_iBeenPlayGame ++;
	m_bGameStation = GS_SEND_CARD;

	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));
	::memset(m_bCanleave,1,sizeof(m_bCanleave));
	::memset(m_bUserReady , 0 , sizeof(m_bUserReady)) ; 

	//���Ϳ�ʼ��Ϣ
	BeginUpgradeStruct BeginMessage;
	memset(&BeginMessage , 0 , sizeof(BeginMessage)) ; 
	BeginMessage.iBeenPlayGame = m_iBeenPlayGame;
	BeginMessage.iPlayLimit = m_iLimitPlayGame;
	memcpy(BeginMessage.bUserMing , m_GameMutiple.sMingPaiMutiple , sizeof(BeginMessage.bUserMing)) ; 

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		m_iLastCardCount[i] = 0;
	}
	::ZeroMemory(m_iLastOutCard, 0, PLAY_COUNT*45);
	m_bIsLastCard = false;
	m_byteHitPass = 0;

	for (int i = 0; i < PLAY_COUNT; i ++)
		SendGameData(i,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_GAME_BEGIN,0);
	SendWatchData(m_bMaxPeople,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_GAME_BEGIN,0);

	//�ַ��˿�
	BYTE iCardArray[162];
	m_Logic.RandCard(iCardArray,m_iPlayCount,m_bHaveKing);

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
		::CopyMemory(m_iUserCard[i],&iCardArray[m_iUserCount*i],sizeof(BYTE)*m_iUserCount);
	}
	::CopyMemory(m_iBackCard,&iCardArray[m_iSendCount],sizeof(BYTE)*m_iBackCount);

	m_Logic.InitGameTask() ; 

	if(!m_Logic.GetBackCardType(m_iBackCard , m_iBackCount))
	{
		m_Logic.GetRandTask(m_bDeskIndex) ; 
	}
	GamePrepare();

	return TRUE;
}

//׼������
BOOL CServerGameDesk::GamePrepare()
{
	PrepareStruct Prepare;
	Prepare.bCard = m_bThrowoutCard;

	for(int i = 0; i < PLAY_COUNT; i ++)
		SendGameData(i,&Prepare,sizeof(Prepare),MDM_GM_GAME_NOTIFY,ASS_GAME_PREPARE,0);

	SendWatchData(m_bMaxPeople,&Prepare,sizeof(Prepare),MDM_GM_GAME_NOTIFY,ASS_GAME_PREPARE,0);

	SetTimer(TIME_SEND_ALL_CARD,200);

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

///һ�ΰl������
/// Ϊ��ֹ��ҿ��������ڣ�ֻ���Լ������ƣ����˵�������ա�
/// �Թ���Ҳ����κ��ƣ�������������
/// ��ZXD�޸���20100314

BOOL CServerGameDesk::SendAllCard()
{
	SendAllStruct SendAll;
	::memset(&SendAll,0,sizeof(SendAll));

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		m_iUserCardCount[i] = m_iUserCount;		
		SendAll.iUserCardCount[i] = m_iUserCardCount[i];
	}

	//��������
	int iPos = 0;
	int iTempPos = 0 ;
	for(int i = 0;i < PLAY_COUNT; i ++)
	{
		int iTempPos = 0 ;

		for(int  j  = 0  ; j < PLAY_COUNT  ;j++)
		{
			if(i == j ||m_GameMutiple.sMingPaiMutiple[j] > 0)
			{
				::CopyMemory(&SendAll.iUserCardList[iTempPos],m_iUserCard[j],sizeof(BYTE)*m_iUserCardCount[j]);
			}

			iTempPos += m_iUserCardCount[j] ; 
		}
	
		iPos += m_iUserCardCount[i];
		SendGameData(i,&SendAll,sizeof(SendAll),MDM_GM_GAME_NOTIFY,ASS_SEND_ALL_CARD,0);
		SendWatchData(i,&SendAll,sizeof(SendAll),MDM_GM_GAME_NOTIFY,ASS_SEND_ALL_CARD,0);
		::ZeroMemory(&SendAll.iUserCardList, sizeof(SendAll.iUserCardList));
	}


	m_iSendCardPos ++;
	if(m_iSendCardPos == 1)
	{
		KillTimer(TIME_SEND_ALL_CARD);

		SetTimer(TIME_SEND_CARD_ANI , m_iSendCardTime *1000) ; ///����ҷ��ƶ�ʱ��
		
		return TRUE;
	}

	return FALSE;
}

//���ƽ���
BOOL CServerGameDesk::SendCardFinish()
{
	SendCardFinishStruct sendcardfinish;

	//���ͷ��ƽ���
	for(int i = 0; i < PLAY_COUNT; i ++)
		SendGameData(i,&sendcardfinish,sizeof(sendcardfinish),MDM_GM_GAME_NOTIFY,ASS_SEND_FINISH,0);
	SendWatchData(m_bMaxPeople,&sendcardfinish,sizeof(sendcardfinish),MDM_GM_GAME_NOTIFY,ASS_SEND_FINISH,0);

	//if (m_bFirstCallScore == 255)
	//{
	//	srand(GetTickCount());
	//	m_bFirstCallScore = rand()%PLAY_COUNT;
	//}
	//SendCallScore(m_bFirstCallScore);

	///��Ҫ�з����� ,ֱ�ӿ�ʼ�������ͽе������� 

	m_bGameStation = GS_WAIT_BACK;

	if(m_iFirstShow != 255 )  ///�����ƿ�ʼ����
	{
		m_iCurrentRobPeople = m_iFirstShow ; 
	}
	else
	{
		m_iCurrentRobPeople = rand()%PLAY_COUNT ; 
	}
	
	SendRobNT(m_iCurrentRobPeople);

	return TRUE;
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

	// ���ö�ʱ�� 20���ڣ��з�����Ӧ����������Զ�����з� 20100319 duanxiaohui ���
	SetTimer(TIME_JIAO_FEN, (m_iCallScoreTime+3) * 1000); 

	return TRUE;
}

//�з�
BOOL CServerGameDesk::UserCallScore(BYTE bDeskStation, int iVal)
{
	if (bDeskStation != m_bCurrentCallScore)
	{
		return true;
	}

	// �����зֶ�ʱ��
	KillTimer(TIME_JIAO_FEN);

	if(iVal == 0)
	{	
		m_iCallScore[bDeskStation] = 0;		//====ĳλ�ò��з�
	}
	else
	{
		m_iCallScore[bDeskStation] = iVal;	//�з�����

		m_iPrepareNT = bDeskStation;				//���з���

		if( iVal == 3)						//����ֱ�ӽ�3��
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
		GameFinish(0,GF_NO_CALL_SCORE);
		return true;
	}
	if (m_iCallScore[m_iPrepareNT] == 0 )	//û���˽з�.���·���
	{
		GameFinish(0,GF_NO_CALL_SCORE);
		return true;
	}
	//����������λ�ý�������
	m_iUpGradePeople = m_iPrepareNT;

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
	{
		RobNTFinish();
	}
	else
	{
		SendRobNT(bDeskStation);
	}
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
	robnt.iValue = ((m_iFirstRobNt == 255) ?0:1) ;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&robnt,sizeof(robnt),MDM_GM_GAME_NOTIFY,ASS_ROB_NT,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&robnt,sizeof(robnt),MDM_GM_GAME_NOTIFY,ASS_ROB_NT,0);

	SetTimer(TIME_ROB_NT, (m_iRobNTTime+3) * 1000); 
	return TRUE;
}

//�����������Ϣ
BOOL CServerGameDesk::UserRobNT(BYTE bDeskStation, int iValue)
{
	if(bDeskStation != m_iCurrentRobPeople )
	{
		return TRUE ; 
	}

	if(m_iRobStation[bDeskStation]>1)
	{
		return TRUE ; 
	}

	KillTimer(TIME_ROB_NT);

	//������������
	if(iValue >0)
	{
		m_iPrepareNT = bDeskStation;	
		//��һԤ��ׯ��
		m_iRobStation[bDeskStation] += 1 ; 
	}
	else
	{
		m_iRobStation[bDeskStation] = 255;  
	}


	int iTempValue = 0 ; 
	if(m_iFirstRobNt != 255)
	{
		if(iValue >0 )
		{
			iTempValue = 3 ; 
			m_GameMutiple.sRobNtMutiple[bDeskStation] += 1 ; 
		}
		else
		{
			iTempValue = 2 ; 
		}
	}
	else
	{
		iTempValue = iValue ; 
	}

	RobNTStruct  robnt;
	robnt.bDeskStation = bDeskStation;
	robnt.iValue = iTempValue ;
	robnt.byRobCount = m_GameMutiple.sRobNtMutiple[bDeskStation] ; 

	for(int i = 0;i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&robnt,sizeof(robnt),MDM_GM_GAME_NOTIFY,ASS_ROB_NT_RESULT,0);		  //���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&robnt,sizeof(robnt),MDM_GM_GAME_NOTIFY,ASS_ROB_NT_RESULT,0);


	if(m_iRobStation[(bDeskStation+1)%PLAY_COUNT] == 255 &&m_iRobStation[(bDeskStation + 2)%PLAY_COUNT] == 255) 
	{
		RobNTFinish() ;
		return  TRUE ; 
	}

	if(m_iPrepareNT != 255 
		&& m_iRobStation[(m_iPrepareNT + 1)%PLAY_COUNT] == 255 
		&&m_iRobStation[(m_iPrepareNT + 2)%PLAY_COUNT] == 255) 
	{
		RobNTFinish() ;
		return  TRUE ; 
	}

	if(bDeskStation == m_iFirstRobNt)
	{
		RobNTFinish() ; 
		return TRUE ; 
	}


	if(iValue>0 && m_iFirstRobNt == 255) ///��һ�� ѡ������������
	{
		m_iFirstRobNt = bDeskStation ; 
	}

	BYTE bDesk = GetRobNtDeskStation(bDeskStation);

	if(bDesk == bDeskStation)
	{
		RobNTFinish() ; 
		return TRUE ; 
	}

	CString  strInfo ; 
	strInfo.Format("wysoutlog:: %d" , m_bRobnt) ; 
	OutputDebugString(strInfo) ; 

	if(iValue >0 &&false == m_bRobnt)
	{
		RobNTFinish() ; 
		return TRUE ; 
	}

	m_iCurrentRobPeople = bDesk ; 

	SendRobNT(bDesk);

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
		if(m_iRobStation[i] != 255)
			return i;
	}
	return bDeskStation;
		 
}

//����������
BOOL CServerGameDesk::RobNTFinish()
{
	if(m_iPrepareNT == 255)
	{
		if(m_iFirstShow != 255)
		{
			 m_iPrepareNT = m_iFirstShow ; 
		}
		else
		{
			GameFinish(0 , GF_NO_CALL_SCORE) ;

			return TRUE ; 
		}
	}

	m_iUpGradePeople = m_iPrepareNT ; 

	//�������Ƿ�ɹ�
	RobNTStruct  robresult;
	robresult.bDeskStation = m_iPrepareNT;
	robresult.iValue = -1;
	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		SendGameData(i,&robresult,sizeof(robresult),MDM_GM_GAME_NOTIFY,ASS_ROB_NT_FINISH,0);
	}
	SendWatchData(m_bMaxPeople,&robresult,sizeof(robresult),MDM_GM_GAME_NOTIFY,ASS_ROB_NT_FINISH,0);
	
	//����������������
	SendBackCard();
	return TRUE;
}

//���͵���
BOOL CServerGameDesk::SendBackCard()
{
	if (m_bHaveSendBack)
	{
		return TRUE;
	}
	m_bHaveSendBack = true;

	m_iUpGradePeople = m_iPrepareNT;

	BackCardExStruct BackCard;
	memset(&BackCard , 0 , sizeof(BackCard)) ; 

	BackCard.iGiveBackPeople = m_iUpGradePeople;
	BackCard.iBackCardCount  = m_iBackCount;

	m_Logic.SetGameTask(BackCard.gameTask) ;

	BackCard.gameTask.byBackCardMutiple = m_Logic.GetBackCardMytiple() ; 
	m_GameMutiple.sBackCardMutiple = m_Logic.GetBackCardMytiple()  ; 

	::CopyMemory(&m_iUserCard[m_iUpGradePeople][m_iUserCardCount[m_iUpGradePeople]],m_iBackCard,sizeof(BYTE)*BackCard.iBackCardCount);
	m_iUserCardCount[m_iUpGradePeople] += BackCard.iBackCardCount;

	::CopyMemory(BackCard.iBackCard,m_iBackCard,sizeof(BYTE)*BackCard.iBackCardCount);

	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		SendGameData(i,&BackCard,sizeof(BackCard),MDM_GM_GAME_NOTIFY,ASS_BACK_CARD_EX,0);
	}
	SendWatchData(m_bMaxPeople,&BackCard,sizeof(BackCard),MDM_GM_GAME_NOTIFY,ASS_BACK_CARD_EX,0);

	if(m_bAdddouble)
	{
		SendAddDouble();
	}
	else
	{
		AddDoubleFinish();
	}

    return TRUE;
}

//���ͼӰ���Ϣ
BOOL CServerGameDesk::SendAddDouble()
{
	m_iGameFlag = GS_FLAG_ADD_DOUBLE;				//�Ӱ�																
	m_iRecvMsg = 0;

	SetTimer(TIME_ADD_DOUBLE,(m_iAddDoubleTime+3) * 1000);
	//���������
	AddDoubleStruct  adddouble;
	memset(&adddouble , 0 , sizeof(adddouble)) ; 
	adddouble.bDeskStation = m_iUpGradePeople;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if(NULL == m_pUserInfo[i])
		{
			continue;
		}

		if(m_pUserInfo[m_iUpGradePeople]->m_UserData.i64Money<m_iAddDoubleLimit && m_iAddDoubleLimit >0)
		{
			adddouble.iValue = 2;
		}
		else if(m_pUserInfo[i]->m_UserData.i64Money<m_iAddDoubleLimit && m_iAddDoubleLimit >0)
		{
			adddouble.iValue = 1 ; 
		}
		else
		{
			adddouble.iValue = 0 ; 
		}
		
		SendGameData(i,&adddouble,sizeof(adddouble),MDM_GM_GAME_NOTIFY,ASS_ADD_DOUBLE,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&adddouble,sizeof(adddouble),MDM_GM_GAME_NOTIFY,ASS_ADD_DOUBLE,0);
	return  TRUE;
}

//��ҼӰ�
BOOL CServerGameDesk::UserAddDouble(BYTE bDeskStation, int iVal)
{
	if(m_iAddStation[bDeskStation] >0 )
	{
		return TRUE;
	}

	m_iRecvMsg ++ ;

	if(iVal >0 )
	{
		m_GameMutiple.sAddGameMutiple[bDeskStation] =  1 ;
		m_iAddStation[bDeskStation] = iVal ; 
	}
	else
	{
		m_iAddStation[bDeskStation] = 255 ;  
	}

	//������������
	AddDoubleStruct  adddouble;
	adddouble.bDeskStation = bDeskStation;
	adddouble.iValue = iVal;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&adddouble,sizeof(adddouble),MDM_GM_GAME_NOTIFY,ASS_ADD_DOUBLE_RESULT,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&adddouble,sizeof(adddouble),MDM_GM_GAME_NOTIFY,ASS_ADD_DOUBLE_RESULT,0);

	if(m_iRecvMsg >= 3)
	{
		AddDoubleResult();
	}

	return TRUE;
}

//�Ӱ����
BOOL CServerGameDesk::AddDoubleResult()
{
	KillTimer(TIME_ADD_DOUBLE);

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

	if(m_bShowcard && !m_GameMutiple.sMingPaiMutiple[m_iUpGradePeople])
	{
		SendShowCard();
	}
	else  
	{
		ShowCardFinish();
	}
		
	return true;
}

//����
BOOL CServerGameDesk::SendShowCard()
{
	m_iGameFlag = GS_FLAG_SHOW_CARD;				//����																
	ShowCardStruct show;
	show.bDeskStation = m_iUpGradePeople;

	SetTimer(TIME_SHOW_CARD, m_iAddDoubleTime * 1000); 

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
	KillTimer(TIME_SHOW_CARD);
	if(iValue > 0)
	{
		m_GameMutiple.sMingPaiMutiple[bDeskStation] = iValue ; 

		if(255 == m_iFirstShow &&GS_FLAG_SHOW_CARD != m_iGameFlag)
		{
			m_iFirstShow = bDeskStation ; 
		}
	}

	ShowCardStruct showresult;
	::ZeroMemory(&showresult, sizeof(showresult));
	showresult.bDeskStation = bDeskStation;
	showresult.iValue = iValue;

	showresult.iCardCount = m_iUserCardCount[bDeskStation] ; 
	memcpy(showresult.iCardList , m_iUserCard[bDeskStation] , sizeof(BYTE) *m_iUserCardCount[bDeskStation]) ; 

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		SendGameData(i,&showresult,sizeof(showresult),MDM_GM_GAME_NOTIFY,ASS_SHOW_CARD_RESULT,0);		//���з�������������û�
	}
	SendWatchData(m_bMaxPeople,&showresult,sizeof(showresult),MDM_GM_GAME_NOTIFY,ASS_SHOW_CARD_RESULT,0);

	if(m_iGameFlag == GS_FLAG_SHOW_CARD)   ///ׯ������
	{
		ShowCardFinish();
	}
	return TRUE;
}

//���ƽ���
BOOL CServerGameDesk::ShowCardFinish()
{
	m_iGameFlag = GS_FLAG_PLAY_GAME ; 

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
	KillAllTimer() ; 
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

	if (m_pUserInfo[m_iOutCardPeople] != NULL)
	{
		// �жϸö�������Ƿ����
		if (USER_CUT_GAME == m_pUserInfo[m_iOutCardPeople]->m_UserData.bUserState)
		{
			// �Զ�����
			SetTimer(TIME_OUT_CARD, 1000);
		}
		else
		{
			SetTimer(TIME_OUT_CARD,(m_iThinkTime + 5)*1000);
		}
	}

	return TRUE;
}

//�û�����
BOOL CServerGameDesk::UserOutCard(BYTE bDeskStation,  BYTE iOutCard[],  int iCardCount)
{
	CSingleLock singleLock(&m_Mutex);
	singleLock.Lock();

	if (bDeskStation != m_iOutCardPeople) 
		return true;
	KillTimer(TIME_OUT_CARD);

	//�����������
	if(iCardCount > 0)					
	{
		if(0 == m_iNtFirstCount)		//�������ĵ�һ����
			m_iNtFirstCount = iCardCount;

		m_iNowBigPeople = bDeskStation;
		m_iBaseOutCount = iCardCount;
		//���ӳ����߼��Ϸ����ж�zht 2010-03-23
		if (!m_Logic.CanOutCard(iOutCard,iCardCount,m_iDeskCard[m_iNowBigPeople],m_iDeskCardCount[m_iNowBigPeople],\
			m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation],bDeskStation == m_iNowBigPeople))
		{
			return TRUE;
		}
		if (m_Logic.RemoveCard(iOutCard,iCardCount,m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation]) == 0)
			return FALSE;
		m_iUserCardCount[bDeskStation] -= iCardCount;
		//��¼������Ϣ
		m_iDeskCardCount[bDeskStation] = iCardCount;
		::CopyMemory(m_iDeskCard[bDeskStation],iOutCard,sizeof(BYTE)*iCardCount);

		m_byPass[bDeskStation] = false ; 
	}
	else
	{	//ĳһλ��ʲ�N��Ҳ����
		m_iDeskCardCount[bDeskStation] = iCardCount;

		m_byteHitPass |= (1<<bDeskStation);	//��¼PASS
		m_byPass[bDeskStation] = true ; 
	}
	//������ҳ��ƽ��
	OutCardMsg UserOutResult;
	UserOutResult.bDeskStation = bDeskStation;
	UserOutResult.iCardCount = iCardCount;
	::CopyMemory(UserOutResult.iCardList,iOutCard,sizeof(BYTE)*iCardCount);

	for (int i = 0;i < PLAY_COUNT; i ++) 
	{
		SendGameData(i,&UserOutResult,sizeof(OutCardMsg),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);
	}
	SendWatchData(m_bMaxPeople,&UserOutResult,sizeof(OutCardMsg),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);

	//�Ƿ�Ϊ�ӷ�����
	IsAwardPoin(iOutCard,iCardCount,bDeskStation);

	//====�ж��Ƿ��������Ƿ������
	if (m_iUserCardCount[bDeskStation] <= 0)
	{
		m_Logic.SetLastCardData(iOutCard , iCardCount) ; 

		m_iDealPeople = bDeskStation;
		//�����Ƹ������O��
		SetTimer(TIME_GAME_FINISH,1000);
		return true;

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
		SendGameData(i,&UserOut,sizeof(OutCardMsg),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);

	SendWatchData(m_bMaxPeople,&UserOut,sizeof(OutCardMsg),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);

	// duanxiaohui ���� 20100319
	if (m_pUserInfo[m_iOutCardPeople] != NULL)
	{
		// �жϸö�������Ƿ����
		if (USER_CUT_GAME == m_pUserInfo[m_iOutCardPeople]->m_UserData.bUserState)
		{
			// �Զ�����
			SetTimer(TIME_OUT_CARD, 1000);
		}
		else
		{
			SetTimer(TIME_OUT_CARD,(m_iThinkTime + 8)*1000);
		}
	}
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
	m_iOutCardPeople = m_iFirstOutPeople = m_iNowBigPeople = bDeskStation;

	//������һ�ֳ������
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		m_bIsLastCard = true;
		m_byteHitPass = 0;
		m_iLastCardCount[i] = m_iDeskCardCount[i];
		::CopyMemory(m_iLastOutCard[i],m_iDeskCard[i],sizeof(BYTE)*m_iLastCardCount[i]);
		::CopyMemory(m_byLastTurnPass , m_byPass , sizeof(m_byLastTurnPass)) ; 
	}

	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_byPass , 0 ,sizeof(m_byPass)); 

	NewTurnStruct turn;
	turn.bDeskStation = bDeskStation ;

	for (int i = 0; i < PLAY_COUNT; i ++) 
		SendGameData(i,&turn,sizeof(turn),MDM_GM_GAME_NOTIFY,ASS_NEW_TURN,0);
	SendWatchData(m_bMaxPeople,&turn,sizeof(turn),MDM_GM_GAME_NOTIFY,ASS_NEW_TURN,0);

	//SetTimer(TIME_OUT_CARD,(m_iThinkTime + 8)*1000);

	// duanxiaohui ���� 20100319
	if (m_pUserInfo[m_iOutCardPeople] != NULL)
	{
		// �жϸö�������Ƿ����
		if (USER_CUT_GAME == m_pUserInfo[m_iOutCardPeople]->m_UserData.bUserState)
		{
			// �Զ�����
			SetTimer(TIME_OUT_CARD, 1000);
		}
		else
		{
			SetTimer(TIME_OUT_CARD,(m_iThinkTime + 8)*1000);
		}
	}
	return TRUE;
}

//����������������
BOOL CServerGameDesk::IsAwardPoin(BYTE iOutCard[],int iCardCount,BYTE bDeskStation)
{
	if(m_Logic.IsKingBomb(iOutCard,iCardCount) || m_Logic.IsBomb(iOutCard,iCardCount))
	{
		m_GameMutiple.sBombCount += 1 ; 
		
		m_iAwardPoint[bDeskStation] += 1;							

		AwardPointStruct award;
		award.iAwardPoint = m_iAwardPoint[bDeskStation];
		award.bDeskStation = bDeskStation;
		
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
	CString strInfo ; 
	strInfo.Format("wysouthlddz:: bDeskStation ==%d , bCloseFlag==%d" , bDeskStation , bCloseFlag) ; 
	OutputDebugString(strInfo) ; 
		//��д����
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			//�������� 
			m_bGameStation = GS_WAIT_ARGEE;
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}
			///��Ϸ�������������ȡ���й�
			for(int i = 0 ; i <PLAY_COUNT ; i++)
			{
				if(NULL != m_pUserInfo[i])
				{
					UserSetAuto(i , false) ; 
				}
			}
			//��Ϸ����
			GameEndStruct GameEnd;
			::memset(&GameEnd,0,sizeof(GameEnd));
			::memset(GameEnd.iTurePoint,0,sizeof(GameEnd.iTurePoint));

			for(int i = 0 ; i <PLAY_COUNT ; i ++)
			{
				GameEnd.iUserCardCount[i] = m_iUserCardCount[i] ; 
				memcpy(GameEnd.iUserCard[i] , m_iUserCard[i] , sizeof(BYTE) *m_iUserCardCount[i]) ; 
			}

			///����������
			GameEnd.bFinishTask = m_Logic.IsFinishTask() ; 

			if(GetNoOutCard())
			{
				m_GameMutiple.sSprintMutiple = 2 ;
			}

			m_GameMutiple.sCardShapeMutiple = m_Logic.GetTaskMutiple(GameEnd.bFinishTask) ;

			GameEnd.gameMutiple = m_GameMutiple ;  ///�����е��¼�ȫ�����

			int  iTurePoint = m_GameMutiple.GetPublicMutiple() ;
			//��˭ʤ
			if(m_iUserCardCount[m_iUpGradePeople] == 0)	
			{
				iTurePoint = iTurePoint;
			}
			else 
			{
				iTurePoint = -iTurePoint;
			}

			if(m_iLimitPoint >0 && abs(iTurePoint) <m_iLimitPoint)  //������������
			{				
				iTurePoint = iTurePoint >0 ?(m_iLimitPoint):(-m_iLimitPoint); 				
			}
														
			GameEnd.iUpGradeStation = m_iUpGradePeople;
		
			memcpy(&GameEnd.iUserCard,&m_iUserCard,sizeof(GameEnd.iUserCard));

			memcpy(&GameEnd.iUserCardCount,&m_iUserCardCount,sizeof(m_iUserCardCount));

			int  iNtMutiple = m_GameMutiple.sAddGameMutiple[m_iUpGradePeople] >0 ?2:1 ; 

			for(int i = 0; i < PLAY_COUNT; i++)
			{
				int iMyMutiple = m_GameMutiple.sAddGameMutiple[i] >0 ?2:1 ; 

				if(i == m_iUpGradePeople)//ׯ��
				{
					continue; 
				}
				else
				{
					GameEnd.iTurePoint[i] =-iTurePoint*iNtMutiple*iMyMutiple;//�Ӱ�
				}

				GameEnd.iTurePoint[m_iUpGradePeople]-= GameEnd.iTurePoint[i];
			}

			CString strInfo ; 
			///һ�¼�����ҵĵ÷����������ǽ�ҳ��Ļ���Ҫ�ж��Ƿ񳬹�������ϵ�Ǯ������ǻ��ֳ������ж�
			if(m_pDataManage->m_InitData.uComType != 1 ) 
			{
				UINT iBasePoint = m_pDataManage->m_InitData.uBasePoint ; 

				if(iBasePoint < 1)
				{
					iBasePoint = 1 ; 
				}
				///ͳ�ƴ�Сʱ������Ϸ�еı���
				for(int i = 0 ; i< PLAY_COUNT ; i++)
				{
					GameEnd.iTurePoint[i] *= iBasePoint ; 
				}

				__int64 iLimitMoney  = m_pUserInfo[m_iUpGradePeople]->m_UserData.i64Money; 

				if(m_iGameMaxLimit >0)
				{
					iLimitMoney = min(iLimitMoney , m_iGameMaxLimit) ; 
				}

				///��ӷ������ƺ����ӮǮ���ܴ����Լ���Ǯ
				if(iLimitMoney <_abs64(GameEnd.iTurePoint[m_iUpGradePeople]) && 0 != GameEnd.iTurePoint[m_iUpGradePeople])
				{
					float fPercent = abs(iLimitMoney/(float)GameEnd.iTurePoint[m_iUpGradePeople]);

					strInfo.Format("wyslog::ׯ��%d ,GameEnd.iTurePoint[0]=%I64d , GameEnd.iTurePoint[1]=%I64d ,GameEnd.iTurePoint[2]=%I64d ,iLimitMoney==%I64d ,fPercent=%f ,%d" , m_iUpGradePeople ,GameEnd.iTurePoint[0] , GameEnd.iTurePoint[1] ,GameEnd.iTurePoint[2],iLimitMoney ,fPercent ,iBasePoint) ; 
					OutputDebugString(strInfo) ; 

					//GameEnd.iTurePoint[m_iUpGradePeople] *= fPercent ;

					//GameEnd.iTurePoint[(m_iUpGradePeople + 2)%PLAY_COUNT] = - GameEnd.iTurePoint[m_iUpGradePeople] - GameEnd.iTurePoint[(m_iUpGradePeople + 1)%PLAY_COUNT] ; 

					GameEnd.iTurePoint[(m_iUpGradePeople + 1)%PLAY_COUNT] *=fPercent ; 

					GameEnd.iTurePoint[(m_iUpGradePeople + 2)%PLAY_COUNT] *= fPercent ; 
				}

				for(int  i = 0 ; i<PLAY_COUNT ; i++)
				{
					if(i == m_iUpGradePeople)
					{
						continue;
					}

					if(_abs64(GameEnd.iTurePoint[i]) >m_pUserInfo[i]->m_UserData.i64Money )
					{
						GameEnd.iTurePoint[i] = m_pUserInfo[i]->m_UserData.i64Money *(iTurePoint>0 ?-1 :1) ; 
					}
				}

				//GameEnd.iTurePoint[m_iUpGradePeople] = -GameEnd.iTurePoint[(m_iUpGradePeople + 1)%PLAY_COUNT] -GameEnd.iTurePoint[(m_iUpGradePeople + 2)%PLAY_COUNT] ; 
				
				///ͳ�ƴ�Сʱ������Ϸ�еı���
				for(int i = 0 ; i< PLAY_COUNT ; i++)
				{
					if(i == m_iUpGradePeople)
					{
						continue ;
					}

					GameEnd.iTurePoint[i] /= iBasePoint ; 
				}

				GameEnd.iTurePoint[m_iUpGradePeople] = -GameEnd.iTurePoint[(m_iUpGradePeople + 1)%PLAY_COUNT] - GameEnd.iTurePoint[(m_iUpGradePeople + 2)%PLAY_COUNT] ; 
			}

			strInfo.Format("wyslog::���ս���ׯ��%d ,GameEnd.iTurePoint[0]=%I64d , GameEnd.iTurePoint[1]=%I64d ,GameEnd.iTurePoint[2]=%I64d " , m_iUpGradePeople ,GameEnd.iTurePoint[0] , GameEnd.iTurePoint[1] ,GameEnd.iTurePoint[2]) ; 
			OutputDebugString(strInfo) ; 

			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));

			ChangeUserPointint64(GameEnd.iTurePoint,temp_cut);
			__super::RecoderGameInfo(GameEnd.iChangeMoney);

			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0; /// ��ҳ����ۻ���    add by wys 2010-11-1
			
			if (true == bNotCostPoint)
			{
				memset(GameEnd.iTurePoint,0,sizeof(GameEnd.iTurePoint));
			}
			//����
			if(m_iBeenPlayGame >= m_iLimitPlayGame || TY_MATCH_GAME != m_pDataManage->m_InitData.uComType)//���ֽ���
			{
				m_bGameStation = GS_WAIT_ARGEE;

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
			m_bGameStation = GS_WAIT_ARGEE;

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
				GameFinish(255,GF_SAFE);
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
			CutEnd.bDeskStation = -1 ; 

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
			CutEnd.iTurePoint[bDeskStation] = - GetRunPublish(bDeskStation);
			CutEnd.iTurePoint[(bDeskStation + 1)%PLAY_COUNT] = GetRunAwayOtherGetPoint((bDeskStation + 1)%PLAY_COUNT,bDeskStation);
			CutEnd.iTurePoint[(bDeskStation + 2)%PLAY_COUNT] = GetRunAwayOtherGetPoint((bDeskStation + 2)%PLAY_COUNT,bDeskStation);

			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				temp_cut[i] = (bDeskStation == i && CutEnd.iTurePoint[i] < 0);
			}

			ChangeUserPointint64(CutEnd.iTurePoint, temp_cut);
			__super::RecoderGameInfo(CutEnd.iChangeMoney);

			bool bNotCostPoint = (m_pDataManage->m_InitData.dwRoomRule & GRR_NOT_COST_POINT) > 0; /// ��ҳ����ۻ���  add by wys 2010-11-1
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
	BYTE iResultCardList[45] ; 
	memset(iResultCardList , 0 , sizeof(iResultCardList)) ; 

	int  iResultCardcount = 0 ; 
	int  ibombCount = 0; 
	for(int i = 0 ; i < PLAY_COUNT ; i++)
	{
		if(NULL != m_pUserInfo[i])
		{
			ibombCount +=m_Logic.GetBombCount(m_iUserCard[i] ,m_iUserCardCount[i],4) ; 
		}
		if(m_Logic.TackOutKingBomb(m_iUserCard[i] ,m_iUserCardCount[i] ,iResultCardList ,iResultCardcount))
		{
			ibombCount ++ ; 
		}
	}

	m_GameMutiple.sBombCount += ibombCount ;

	if(m_GameMutiple.sBaseMutiple <=0)
	{
		return m_pDataManage->m_InitData.uRunPublish;
	}

	int  iMutiple =  m_GameMutiple.GetPublicMutiple()/ m_GameMutiple.sBaseMutiple;


	if(bDeskStation == m_iUpGradePeople)
	{
		if(iMutiple <10)
		{
			iMutiple = 20 ; 
		}
		else
		{
			iMutiple *= 4; 
		}
	}
	else
	{
		if(iMutiple <10)
		{
			iMutiple = 10 ; 
		}
		else
		{
			iMutiple *= 2 ; 
		}
	}

	return  (iMutiple *m_GameMutiple.sBaseMutiple) ;
	
}

//�Α���A����
int CServerGameDesk::GetRoomMul()
{
	return m_pDataManage->m_InitData.uBasePoint; // ����
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

//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	m_bUserReady[bDeskStation] = false ; 
	m_GameMutiple.sMingPaiMutiple[bDeskStation] = 0 ; 
	//JAdd-20090116
	if (m_bGameStation>=20&&m_bGameStation<23)  //����Ϸ�У�
	{
		//return false;
		return __super::UserLeftDesk(bDeskStation,pUserInfo);
	}
	else
	{
		return __super::UserLeftDesk(bDeskStation,pUserInfo);
	}
}

//�û�������Ϸ
BOOL CServerGameDesk::UserSetGame(UserArgeeGame * pGameSet)
{
	return TRUE;
}

//�����ı��û����������ĺ���
bool CServerGameDesk::CanNetCut(BYTE bDeskStation)
{
	return true;
}

//�û������뿪
BOOL CServerGameDesk::UserHaveThing(BYTE bDeskStation, char * szMessage)
{
	// �Ѿ�����˳���ť(һ��ֻ�ܵ���һ��)
	if (!m_bCanleave[bDeskStation])
		return TRUE;

	if (m_bGameStation < GS_SEND_CARD) 
		return TRUE;

	if (m_iHaveThingPeople != bDeskStation)
	{
		m_iLeaveArgee = 0;
	}

	m_icountleave = 0;
	m_iHaveThingPeople = bDeskStation;
	m_iLeaveArgee|= 1<<bDeskStation;
	m_bCanleave[bDeskStation]=false;

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
		GameFinish(255,GF_SAFE);
	return TRUE;
}

//ͬ���û��뿪
BOOL CServerGameDesk::ArgeeUserLeft(BYTE bDeskStation, BOOL bArgee)
{
	m_icountleave++;
	if (bArgee) m_iLeaveArgee|= 1<<bDeskStation;
	else 
		m_iLeaveArgee = 0;
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
		GameFinish(255,GF_SAFE);
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

//�Զ�����
BOOL CServerGameDesk::UserAutoOutCard(BYTE bDeskStation)
{
	if (bDeskStation >= PLAY_COUNT)
	{
		return FALSE;
	}

	BYTE bCardList[54];
	int iCardCount = 0;
	//if(bDeskStation == m_iFirstOutPeople)   // duanxiaohui alter 20100319
	{
		m_Logic.AutoOutCard(m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation],
			m_iDeskCard[m_iNowBigPeople],m_iDeskCardCount[m_iNowBigPeople],
			bCardList,iCardCount,bDeskStation == m_iNowBigPeople);
	}
	//else
	//{
	//	iCardCount = 0;
	//}
	return UserOutCard(bDeskStation,bCardList,iCardCount);
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

	UserleftDesk  bagUserleft;
	bagUserleft.bDeskStation = bDeskStation;
	for (int i = 0;i<PLAY_COUNT;i++)
	{
		if (NULL == m_pUserInfo[i])
		{
			continue;
		}
		SendGameData(i,&bagUserleft,sizeof(bagUserleft),MDM_GM_GAME_NOTIFY,ASS_USER_LEFTDESK,0);
	}

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
