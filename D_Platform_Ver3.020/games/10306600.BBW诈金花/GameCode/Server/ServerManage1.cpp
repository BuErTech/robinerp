#include "StdAfx.h"
#include "ServerManage.h"
//#include "..\..\��Ϸ����������\include\���������\common\commonuse.h"
//��Ϸ����
BOOL CServerGameDesk:: m_bHaveKing;
UINT CServerGameDesk:: m_iPlayCard;
UINT CServerGameDesk:: m_iPlayCount;
UINT CServerGameDesk:: m_iSendCount;
UINT CServerGameDesk:: m_iBackCount;
UINT CServerGameDesk:: m_iUserCount;
BYTE CServerGameDesk:: m_iLimitUpGrade;
BOOL CServerGameDesk:: m_bKingCanReplace;
DWORD CServerGameDesk::m_iCardShape1;
DWORD CServerGameDesk::m_iCardShape2;
//��Ϸ��չ����
BYTE CServerGameDesk:: m_iThinkTime;
BYTE CServerGameDesk:: m_iBeginTime;
BYTE CServerGameDesk:: m_EndWaitTime;
BYTE CServerGameDesk:: m_ContributeTime;

void DebugPrintf( const char *p, ...)
{
	CTime time = CTime::GetCurrentTime();
	char szFilename[256];
	sprintf(szFilename, "%d-%d%d%d-log.txt", NAME_ID,time.GetYear(), time.GetMonth(), time.GetDay());
	FILE *fp = fopen(szFilename, "a");
	if (NULL == fp)
	{
		return;
	}
	va_list arg;
	va_start(arg, p);
	vfprintf(fp, p, arg);
	fprintf(fp,"\n");

	fclose(fp);
}
//���ݷ���ID����ini�����ļ�
BOOL CServerGameDesk::LoadExtIni(UINT uRoomID)
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");

	CString str;
	str.Format("51100400_%d",uRoomID);
	CString key =str;
	m_iBeginNt = f.GetKeyVal(key,"beginnt",m_iBeginNt);
	m_iEndNt = f.GetKeyVal(key,"endnt",m_iEndNt);
	m_bCanSetCard = f.GetKeyVal(key,"TestCard",0); //����������

	::memset(m_iPlayNtNum,m_iBeginNt,sizeof(m_iPlayNtNum));

	return TRUE;
}

bool CServerGameDesk::InitDeskGameStation()
{
	
	// ���¼��������ļ������
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	return true;
}

//����ini�����ļ�
BOOL CServerGameDesk::LoadIni()
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +SKIN_FOLDER +"_s.ini");

	
	CString key = TEXT("game");
	m_bHaveKing = f.GetKeyVal(key,"haveking",1);
	m_iPlayCard = f.GetKeyVal(key,"card",2);
	m_iPlayCount = f.GetKeyVal(key,"cardcount",108);
	m_iSendCount = f.GetKeyVal(key,"sendcard",100);
	m_iBackCount = f.GetKeyVal(key,"backcard",8);
	m_iUserCount = f.GetKeyVal(key,"usercount",27);
	m_iLimitUpGrade = f.GetKeyVal(key,"limitupgrade",5);
	m_bKingCanReplace = f.GetKeyVal(key,"kingcanreplace",0);
	key=TEXT("other");
	m_iBeginTime = f.GetKeyVal(key,"begintime",15);
	m_iThinkTime = f.GetKeyVal(key,"thinktime",30);
	m_EndWaitTime = f.GetKeyVal(key,"endwaittime",15);
	m_ContributeTime = f.GetKeyVal(key,"contributetime",30);
	
	key = TEXT("cardshape");
	m_iCardShape1 &=0x00000000;
	m_iCardShape2 &=0x00000000;
	m_iCardShape1 |= (f.GetKeyVal(key,"one",1)<<1&0xFFFFFFFF);						//����
	m_iCardShape1 |= ((f.GetKeyVal(key,"two",1)<<2)&0xFFFFFFFF);					//��
	
	m_iCardShape1 |= ((f.GetKeyVal(key,"varstraight",1)<<3)&0xFFFFFFFF);			//���ֵ�˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"traight",1)<<4)&0xFFFFFFFF);				//��˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"flush",0)<<5)&0xFFFFFFFF);				//ͬ��
	m_iCardShape2 |= ((f.GetKeyVal(key,"straightflush",1)<<16)&0xFFFFFFFF);		//ͬ��˳(��45λ����45-30=15

	m_iCardShape2 |= ((f.GetKeyVal(key,"varstraightflush",1)<<17)&0xFFFFFFFF);	//����ͬ��˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"three",1)<<8)&0xFFFFFFFF);				//����
	m_iCardShape1 |= ((f.GetKeyVal(key,"threeandone",0)<<9)&0xFFFFFFFF);			//������
	m_iCardShape1 |= ((f.GetKeyVal(key,"threeandtwo",0)<<10)&0xFFFFFFFF);			//������
	//����1-10(ͬ��˳�ĳ�46,����ͬ��˳�ĳ�47)

	m_iCardShape1 |= ((f.GetKeyVal(key,"threeanddouble",1)<<11)&0xFFFFFFFF);		//��������
	m_iCardShape1 |= ((f.GetKeyVal(key,"vardoubleseq",1)<<12)&0xFFFFFFFF);	//����˫˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"doubleseq",1)<<13)&0xFFFFFFFF);		//˫˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"varthreeseq",1)<<14)&0xFFFFFFFF);	//������˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"threeseq",1)<<15)&0xFFFFFFFF);		//��˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"varthreeoneseq",0)<<16)&0xFFFFFFFF);//����������˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"threeoneseq",0)<<17)&0xFFFFFFFF);	//������˳

	m_iCardShape1 |= ((f.GetKeyVal(key,"varthreetwoseq",0)<<18)&0xFFFFFFFF);	//����������˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"threetwoseq",0)<<19)&0xFFFFFFFF);		//������˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"varthreedoubleseq",0)<<20)&0xFFFFFFFF);	//����������˳
	//����11-20λ���

	m_iCardShape1 |= ((f.GetKeyVal(key,"threedoubleseq",0)<<21)&0xFFFFFFFF);				//������˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"varthreeseqdoubleseq",0)<<22)&0xFFFFFFFF);		//������˳����˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"threeseqdoubleseq",0)<<23)&0xFFFFFFFF);		//��˳����˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"fourandone",0)<<24)&0xFFFFFFFF);							//�Ĵ���
	m_iCardShape1 |= ((f.GetKeyVal(key,"fourandtwo",0)<<25)&0xFFFFFFFF);							//�Ĵ�����
	m_iCardShape1 |= ((f.GetKeyVal(key,"fourandonedouble",0)<<26)&0xFFFFFFFF);						//�Ĵ���
	m_iCardShape1 |= ((f.GetKeyVal(key,"fourandtwodouble",0)<<27)&0xFFFFFFFF);						//�Ĵ�����
	m_iCardShape1 |= ((f.GetKeyVal(key,"varfourseq",0)<<28)&0xFFFFFFFF);					//������˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"fourseq",0)<<29)&0xFFFFFFFF);						//��˳
	m_iCardShape1 |= ((f.GetKeyVal(key,"varfouroneseq",0)<<30)&0xFFFFFFFF);				//�����Ĵ���˳
	//����21-30λ���
	m_iCardShape2 |= ((f.GetKeyVal(key,"fouroneseq",0)<<0)&0xFFFFFFFF);					//�Ĵ���˳(30-30=0λ
	m_iCardShape2 |= ((f.GetKeyVal(key,"varfourtwoseq",0)<<1)&0xFFFFFFFF);					//�����Ĵ���˳
	m_iCardShape2 |= ((f.GetKeyVal(key,"fourtwoseq",0)<<2)&0xFFFFFFFF);					//�Ĵ���˳
	m_iCardShape2 |= ((f.GetKeyVal(key,"varfouronedoubleseq",0)<<3)&0xFFFFFFFF);			//�����Ĵ���˳
	m_iCardShape2 |= ((f.GetKeyVal(key,"fouronedoubleseq",0)<<4)&0xFFFFFFFF);				//����˳
	m_iCardShape2 |= ((f.GetKeyVal(key,"varfourtwodoubleseq",0)<<5)&0xFFFFFFFF);			//�����Ĵ�����˳
	m_iCardShape2 |= ((f.GetKeyVal(key,"fourtwodoubleseq",0)<<6)&0xFFFFFFFF);				//�Ĵ�����˳
	m_iCardShape2 |= ((f.GetKeyVal(key,"slave510k",0)<<7)&0xFFFFFFFF);							//��ͬ��510k
	m_iCardShape2 |= ((f.GetKeyVal(key,"master510k",0)<<8)&0xFFFFFFFF);							//ͬ��510k
	//����31-39λ���
	m_iCardShape2 |= ((f.GetKeyVal(key,"bomb",1)<<15)&0xFFFFFFFF);								//ը��
	m_iCardShape2 |= ((f.GetKeyVal(key,"samehuabomb",0)<<19)&0xFFFFFFFF);						//ͬ��ը��
	m_iCardShape2 |= ((f.GetKeyVal(key,"kingbomb",1)<<20)&0xFFFFFFFF);							//��ը

	return true;
}
///���ؿ����Ϣ
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	//switch(pNetHead->bAssistantID)
	//{
	//case ASS_GM_FORCE_QUIT:		//ǿ���˳�//��ȫ�˳�
	//	{
	//		//UserFourceQuit(bDeskStation);
	//		return true;
	//	}
	//}
	return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}

//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	

	switch (pNetHead->bAssistantID)
	{
	case ASS_SET_TEST_CARD://�����ͻ���
		{
            SetCardStruct *pSetCard = (SetCardStruct *)pData;

			m_bInSetCard = TRUE;
			for (int i=0; i<PLAY_COUNT&&i<pSetCard->bPlayCount; i++)
			{
				m_SetCardCountTemp[i] = pSetCard->nPlayCardCount[i];
				memcpy(&m_SetCardTemp[i],pSetCard->bPlayCard[i],sizeof(BYTE)*m_SetCardCountTemp[i]);
			}

			return true;
		}
	case ASS_UG_USER_SET:	//������Ϸ
		{
			return TRUE;
		}
	case ASS_GM_AGREE_GAME:		//ͬ����Ϸ
		{
		
			if (bWatchUser)
				return FALSE;
			if(m_bGameStation != GS_WAIT_NEXT)
				m_bGameStation = GS_WAIT_ARGEE;
		
			return   __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}	
	case ASS_CONTRIBUTE://�ֹ�����
		{
			if ((bWatchUser)||(uSize!=sizeof(MoveBackContributeStruct))) 
				return FALSE;

			if (m_bGameStation != GS_CONTRIBUTE/*GS_WAIT_BACK*/) 
				return TRUE;

			MoveBackContributeStruct * pMoveBack=(MoveBackContributeStruct *)pData;

			return UserContribute(bDeskStation,pMoveBack->bCard);
		}
	case ASS_CHOICE_CARD://���ѡ��
		{
			if ((bWatchUser)||(uSize!=sizeof(ChoiceCardStruct)))
			{
				return FALSE;
			}
			if (m_bGameStation != GS_CONTRIBUTE) 
			{
				return FALSE;
			}
			ChoiceCardStruct * Choice=(ChoiceCardStruct *)pData;

			return UserChoiceCard(bDeskStation,Choice->bDest,Choice->bCard);
		}
	case ASS_MOVE_BACK_CONTRIBUTE:	//����˹�
		{
			if ((bWatchUser)||(uSize!=sizeof(MoveBackContributeStruct)))
			{
				return FALSE;
			}
			if (m_bGameStation != GS_BACKCONTRIBUT/* GS_WAIT_BACK*/) 
			{
				return FALSE;
			}
			MoveBackContributeStruct * pMoveBack=(MoveBackContributeStruct *)pData;

			return UserMoveBackContribute(bDeskStation,pMoveBack->bCard);
		}
	case ASS_OUT_CARD:		//�û�����
		{
			if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
			{
				return true;
			}
			OutCardStruct * pOutCard=(OutCardStruct *)pData;
			
			//ȷ����Ч���ݵĴ�С
			if(bWatchUser || uSize != sizeof(OutCardStruct))
			{
				return true;
			}
			return UserOutCard(bDeskStation,pOutCard->iCardList,pOutCard->iCardCount);	
		}
		//////////////////////////////////////////////////////////////////////
		/////////////////////////////������Ϣ/////////////////////////////////
		//////////////////////////////////////////////////////////////////////
	case ASS_AUTO://�й�
		{
			if ((bWatchUser)
				||(uSize!=sizeof(AutoCardStruct))) 
				return FALSE;
			AutoCardStruct * pAuto=(AutoCardStruct *)pData;
			return UserAuto(bDeskStation,pAuto->bAutoCard);
		}
	case ASS_LOSE://Ͷ��
		{
			if (bWatchUser)
				return FALSE;
			return UserLose(bDeskStation);
		}
	case ASS_LOSE_RESULT://Ͷ�����(Ŀǰ����ֻҪ��һ��ͬ����Ϸ������)
		{
			if(bWatchUser||(uSize!=sizeof(LoseResultStruct)))
				return FALSE;
			//if(m_iTouxiang==-1)//��ʾ������Ҳ�ͬ��
			//	return true;
			LoseResultStruct * pLose=(LoseResultStruct *)pData;
			return AgreeUserLose(bDeskStation,pLose->bArgee);
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
			return AgreeUserLeft(bDeskStation,pLeave->bArgeeLeave);
		}

	case ASS_STOP_THING:	//��ǰ����(Ŀǰֻʵ�ֶԼ�ͬ��)
		{
			if(bWatchUser)
				return false;
			return UserStop(bDeskStation);
		}
	case ASS_AGREE_STOP:	//ͬ����ǰ����
		{
			if(bWatchUser)
				return false;
			StopResultStruct * pStop=(StopResultStruct *)pData;
			return AgreeUserStop(bDeskStation,pStop->bArgee);
		}
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}



//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(FULL_BEGIN)
{
	m_iBeenPlayGame=0;									//�Ѿ����е���Ϸ����
	m_iNtPeople=-1;										//�Ϲ�������
	m_iUpGradePeople=-1;								//ׯ��
	m_iGrade2=-1;										//����
	m_iLeaveAgree=0;									//ͬ���뿪������(λ�ƿ���)
	m_iBaseOutCount=0;									//���������ĵ�һ��������(����)���һ��������(������)
	m_iFirstOutPeople=-1;								//�����е�һ��������(�����������һ��������)
	m_iOutCardPeople=-1;								//��ǰ�ֵ�˭����
	m_iTempPoint=0;										//�����(������)
	m_bGameStation=GS_WAIT_ARGEE;						//��Ϸ״̬
	m_iOverContribute = 0;                              //�ѽ�������
	m_ChoiceNumber = 0;                                 //��ѡ������
	m_iBeginNt = 2;                                     //��Ϸ��ʼ
	m_iEndNt = 14;                                      //��Ϸ�յ�
	m_iTempTime = 32;                                   //��ʱʱ��
	//��չ����
	
	m_bAdvanceFinish = false;
	/////��������
	m_bCanSetCard     = FALSE;
	m_bInSetCard      = FALSE;
	::memset(m_SetCardCountTemp,0,sizeof(m_SetCardCountTemp));			
	::memset(m_SetCardTemp,0,sizeof(m_SetCardTemp));
	///////////////////////
	//����
	::memset(m_PunishPoint,0,sizeof(m_PunishPoint));	
	//��������
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));	
	//������������ʼ��
	::memset(m_iSequence,0,sizeof(m_iSequence));
	//��ʼ���й�
	::memset(m_bAutoOutCard,0,sizeof(m_bAutoOutCard));
	//��ʼ���з�
	::memset(m_iJiaofen,-1,sizeof(m_iJiaofen));
	//��ʼ���÷�
	::memset(m_iUserGetPoint,0,sizeof(m_iUserGetPoint));
	//��������
	::memset(m_iPointCardCount,0,sizeof(m_iPointCardCount));
	//����������
	memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));
	///////////////����
	//��ʼ������
	::memset(m_iAwardPoint,0,sizeof(m_iAwardPoint));
	//ͷ����
	::memset(m_iHeads,0,sizeof(m_iHeads));
	//�����ʷ��¼
	::memset(m_iSequenceHistory,0,sizeof(m_iSequenceHistory));
	::memset(m_ChangeCard,0,sizeof(m_ChangeCard));
	memset(m_iWinSequence,0,sizeof(m_iWinSequence));
	memset(m_bIsCut,0,sizeof(m_bIsCut));
	//��A������߼��Ĵ���
	memset(m_PlayANumber,0,sizeof(m_PlayANumber));
	
	//��ʷ��¼��
	m_iHistory = 0;
	
	//������Ϸ����	
	LoadIni();

	//InitThisGame();
}

//�O���Α��B
//BOOL CServerGameDesk::InitThisGame()
//{
//	CString str;
//	str.Format("xyh:InitThisGame uRoomID = %d,m_iBeginNt = %d,m_iEndNt = %d",m_pDataManage->m_InitData.uRoomID,m_iBeginNt,m_iEndNt);
//	OutputDebugString(str);
//
//	::memset(m_iPlayNtNum,m_iBeginNt,sizeof(m_iPlayNtNum));
//	return true;
//}

//��������
CServerGameDesk::~CServerGameDesk(void)
{
}

//��ʱ����Ϣ
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	switch(uTimerID)
	{
	case TIME_SEND_ALL_CARD:
		{
			if (m_bGameStation==GS_SEND_CARD)
			{
				SendAllCard(0);

			}
			else KillTimer(TIME_SEND_ALL_CARD);

			
			break;
		}
	///�����Զ�������Ƽ�ʱ��
	case TIMER_OUT_CARD:
	case TIMER_OUT_CARD+1:
	case TIMER_OUT_CARD+2:
	case TIMER_OUT_CARD+3:

		{
			KillTimer(TIMER_OUT_CARD);
			if (m_bGameStation!=GS_PLAY_GAME)
				return true;

			UserAutoOutCard(/*m_iOutCardPeople*/uTimerID-TIMER_OUT_CARD);
			break;
		}
	case TIME_WAIT_NEWTURN:				         //��һ݆
		{
			KillTimer(TIME_WAIT_NEWTURN);
			if (m_bGameStation==GS_PLAY_GAME)
				NewPlayTurn(m_iFirstOutPeople);
			break;
		}	

	case TIMER_WAIT_JIAOFEN:			         //�з�
		{
			if(m_bGameStation!=GS_WAIT_BACK)
				return TRUE;
			KillTimer(TIMER_WAIT_JIAOFEN);
			BeginPlayUpGrade();				     //��ʼ��Ϸ
			break;
		}
	case TIME_WAIT_CONTRIBUTE:			         //�ֹ�����
		{
			if(m_bGameStation!=GS_CONTRIBUTE)
				return TRUE;
			m_iTempTime--;
			if(m_iTempTime <= 0)
			{
				KillTimer(TIME_WAIT_CONTRIBUTE);
				SystemContribute();		             //ϵͳ�����˹�
			}
			break;
		}
	case TIME_CHOICE_CARD:			             //ϵͳ����ѡ�����
		{
			if(m_bGameStation!=GS_CONTRIBUTE)
				return TRUE;
			KillTimer(TIME_CHOICE_CARD);
			SystemChoiceCard();	                		
			return true;
		}
	case TIME_WAIT_MOVE_BACK_CONTRIBUTE:		 //�˹�
		{
			if(m_bGameStation!=GS_BACKCONTRIBUT )
				return TRUE;
			m_iTempTime--;
			if(m_iTempTime <= 0)
			{
				KillTimer(TIME_WAIT_MOVE_BACK_CONTRIBUTE);
				SystemMoveBackContribute();       //ϵͳ�����˹�
			}
			break;
		}	
	case TIME_WAIT_NT:					         //����(����)
		{
			break;
		}	

	case TIME_GAME_FINISH:				         //�Y��
		{
			KillTimer(TIME_GAME_FINISH);
			if (m_bGameStation==GS_PLAY_GAME) 
				GameFinish(0,GF_NORMAL);
			break;
		}
	case TIME_BACK_CONTRIBUTE_WAIT:              ///�˹���ȴ�3���ٿ�ʼ��Ϸ
		{
			KillTimer(TIME_BACK_CONTRIBUTE_WAIT);
			if(m_bGameStation!=GS_BACKCONTRIBUT)
				return TRUE;
			BeginPlayUpGrade();
			
			break;
		}
	}	

	return __super::OnTimer(uTimerID);
}

//��ȡ��Ϸ״̬��Ϣ
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	m_Logic.Msg("zhtlog:m_bGameStation=%d bDeskStation=%d",m_bGameStation,bDeskStation);
	switch (m_bGameStation)
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
		{
			GameStation_2 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;			  //��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;			  //�Ͱ汾
			//��Ϸ��������
			GameStation.iPlayCard = m_iPlayCard;				  //�˿˸���
				GameStation.bKingCanReplace = m_bKingCanReplace;	  //�Ƿ�����
			GameStation.iCardShape1 = m_iCardShape1;			  //��������
			GameStation.iCardShape2 = m_iCardShape2;			  //��������
			//����ʱ��
			GameStation.iBeginTime = m_iBeginTime;				//��Ϸ��ʼʱ������
			GameStation.iThinkTime = m_iThinkTime;				//��Ϸ˼��ʱ��
			GameStation.iContributeTime = m_ContributeTime;      //��Ϸ����ʱ��

						//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();	//���ӱ���
			GameStation.iRoomBasePoint = GetRoomBasePoint();	//���䱶��
			GameStation.iRunPublish = GetRunPublish();			//���ܿ۷�
			GameStation.bGameStation = m_bGameStation;

			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	case GS_SEND_CARD:		//����״̬
	case GS_WAIT_BACK:		//�ȴ������(������Ϊ�����,��������Ϊ�зֻ�������)
		{
			GameStation_3 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;			//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;			//�Ͱ汾
			//��Ϸ��������
			GameStation.iPlayCard = m_iPlayCard;				//�˿˸���
			GameStation.iPlayCount = m_iPlayCount;				//�˿�����
			GameStation.bKingCanReplace = m_bKingCanReplace;				//�Ƿ�����
			GameStation.iCardShape1 = m_iCardShape1;						//��������
			GameStation.iCardShape2 = m_iCardShape2;						//��������
			//����ʱ��
			GameStation.iBeginTime = m_iBeginTime;				//��Ϸ��ʼʱ������
			GameStation.iThinkTime = m_iThinkTime;				//��Ϸ˼��ʱ��
			GameStation.iEndWaitTime = m_EndWaitTime;			//�з�ʱ��
			GameStation.iContributeTime = m_ContributeTime;      //��Ϸ����ʱ��
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();	//���ӱ���
			GameStation.iRoomBasePoint = GetRoomBasePoint();	//���䱶��
			GameStation.iRunPublish = GetRunPublish();			//���ܿ۷�
			GameStation.iBeenPlayGame=m_iBeenPlayGame;
			GameStation.iPlayNtNum[0]=m_iPlayNtNum[0];		
			GameStation.iPlayNtNum[1]=m_iPlayNtNum[1];
			GameStation.iBeginNt = m_iBeginNt;
			GameStation.iEndNt = m_iEndNt;
			GameStation.bGameStation = m_bGameStation;
			//�R�r�f��
			GameStation.iUpGradePeople=m_iUpGradePeople;
			memcpy(GameStation.iWinSequence,m_iWinSequence,sizeof(m_iWinSequence));

			//�й�
			for(int i=0;i<4;i++)
				GameStation.bAuto[i]=m_bAutoOutCard[i];
			//��������
			int iPos=0;
			for(int i=0;i<4;i++)
			{
				GameStation.iUserCardCount[i]=m_iUserCardCount[i];
				::CopyMemory(&GameStation.iUserCardList[iPos],m_iUserCard[i],sizeof(BYTE)*m_iUserCardCount[i]);
				iPos+=m_iUserCardCount[i];
			}
			//���Ʒ���(�����ƵĶ��ӵ��˶�����,�������ӱ���)
			::CopyMemory(&GameStation.iUserCardList[iPos],m_bCardOfFriend,sizeof(BYTE)*4);
			memcpy(GameStation.iWinSequence,m_iWinSequence,sizeof(m_iWinSequence));
		
			iPos+=4;
			//��������
			int iSendSize=sizeof(GameStation)-sizeof(GameStation.iUserCardList)+sizeof(BYTE)+iPos;
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	case GS_CONTRIBUTE:
		{
			GameConrtibute GameStation;
			memset(&GameStation,0,sizeof(GameStation));
			GameStation.iVersion = DEV_HEIGHT_VERSION;			//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;			//�Ͱ汾
			//��Ϸ��������
			GameStation.iPlayCard = m_iPlayCard;				//�˿˸���
			GameStation.iPlayCount = m_iPlayCount;				//�˿�����
			//����ʱ��
			GameStation.iBeginTime = m_iBeginTime;				//��Ϸ��ʼʱ������
			GameStation.iThinkTime = m_iThinkTime;				//��Ϸ˼��ʱ��
			GameStation.iContributeTime = m_iTempTime; //��Ϸ����ʱ��

			GameStation.iBeenPlayGame=m_iBeenPlayGame;
			GameStation.iPlayNtNum[0]=m_iPlayNtNum[0];		
			GameStation.iPlayNtNum[1]=m_iPlayNtNum[1];
			GameStation.iBeginNt = m_iBeginNt;
			GameStation.iEndNt = m_iEndNt;
			GameStation.iUpGradePeople=m_iUpGradePeople;
			GameStation.bGameStation = m_bGameStation;

			///��һ����ҳ���״̬
			GameStation.bWinerDesk1 = GetPostionBySequence(1);
			GameStation.bWinerDesk1 = GetPostionBySequence(2);
			GameStation.bLoseDesk2 = GetPostionBySequence(3);
			GameStation.bLoseDesk1 = GetPostionBySequence(4);

			///���ƺ����κ�δ�εĴ��������ͻ���
			for(int i=0;i<4;i++)
			{

				//
				GameStation.bAuto[i]=m_bAutoOutCard[i];

				GameStation.iUserCardCount[i]=m_iUserCardCount[i];
				if(i == bDeskStation)
					::CopyMemory(GameStation.iUserCardList[i],m_iUserCard[i],sizeof(BYTE)*m_iUserCardCount[i]);
				::CopyMemory(GameStation.iWinSequence[i],m_iWinSequence[i],sizeof(m_iWinSequence[i]));

			}
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			//SendContribute();
			return true;
		}
	case GS_BACKCONTRIBUT: 
		{
			GameConrtibute GameStation;
			memset(&GameStation,0,sizeof(GameStation));
			GameStation.iVersion = DEV_HEIGHT_VERSION;			//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;			//�Ͱ汾
			//��Ϸ��������
			GameStation.iPlayCard = m_iPlayCard;				//�˿˸���
			GameStation.iPlayCount = m_iPlayCount;				//�˿�����
			//����ʱ��
			GameStation.iBeginTime = m_iBeginTime;				//��Ϸ��ʼʱ������
			GameStation.iThinkTime = m_iThinkTime;				//��Ϸ˼��ʱ��
			GameStation.iContributeTime = m_iTempTime;         //��Ϸ����ʱ��
			GameStation.iBeenPlayGame=m_iBeenPlayGame;
			GameStation.iPlayNtNum[0]=m_iPlayNtNum[0];		
			GameStation.iPlayNtNum[1]=m_iPlayNtNum[1];
			GameStation.iBeginNt = m_iBeginNt;
			GameStation.iEndNt = m_iEndNt;
			GameStation.iUpGradePeople=m_iUpGradePeople;
			GameStation.bGameStation = m_bGameStation;
			
			///��һ����ҳ���״̬
			GameStation.bWinerDesk1 = GetPostionBySequence(1);
			GameStation.bWinerDesk1 = GetPostionBySequence(2);
			GameStation.bLoseDesk2 = GetPostionBySequence(3);
			GameStation.bLoseDesk1 = GetPostionBySequence(4);

			///���ƺ����κ�δ�εĴ��������ͻ���
			for(int i=0;i<4;i++)
			{
				//�й�

				GameStation.bAuto[i]=m_bAutoOutCard[i];
				GameStation.iUserCardCount[i]=m_iUserCardCount[i];
				if(i == bDeskStation)
					::CopyMemory(GameStation.iUserCardList[i],m_iUserCard[i],sizeof(BYTE)*m_iUserCardCount[i]);
				::CopyMemory(GameStation.iWinSequence[i],m_iWinSequence[i],sizeof(m_iWinSequence[i]));

			}
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			//SendMoveBackContribute();
			return true;
		}
	case GS_PLAY_GAME:	//��Ϸ��״̬
		{
			GameStation_4 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;			//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;			//�Ͱ汾
			//��Ϸ��������
			GameStation.iPlayCard = m_iPlayCard;				//�˿˸���
			GameStation.iPlayCount = m_iPlayCount;				//�˿�����
			GameStation.bKingCanReplace = m_bKingCanReplace;	//�Ƿ�����
			GameStation.iCardShape1 = m_iCardShape1;			//��������
			GameStation.iCardShape2 = m_iCardShape2;			//��������
			//����ʱ��
			GameStation.iBeginTime = m_iBeginTime;				//��Ϸ��ʼʱ������
			GameStation.iThinkTime = m_iThinkTime;				//��Ϸ˼��ʱ��
			GameStation.iEndWaitTime = m_EndWaitTime;			//�з�ʱ��
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();	//���ӱ���
			GameStation.iRoomBasePoint = GetRoomBasePoint();	//���䱶��
			GameStation.iRunPublish = GetRunPublish();			//���ܿ۷�
			GameStation.bGameStation = m_bGameStation;


			
			//ȷ��ׯ��ׯ������
			GameStation.iNtPeople=m_iNtPeople;			
			GameStation.iUpGradePeople=m_iUpGradePeople;		
			memcpy(GameStation.iWinSequence,m_iWinSequence,sizeof(m_iWinSequence));
			memcpy(GameStation.iSequence,m_iSequence,sizeof(m_iSequence));

			for(int i=0;i<4;i++)
			{
				GameStation.bAuto[i]=m_bAutoOutCard[i];
				GameStation.iAwardPoint[i]=m_iAwardPoint[i];
			}
		
			GameStation.iPlayNtNum[0]=m_iPlayNtNum[0];		
			GameStation.iPlayNtNum[1]=m_iPlayNtNum[1];		
			GameStation.iBeenPlayGame=m_iBeenPlayGame;
			GameStation.iBeginNt = m_iBeginNt;
			GameStation.iEndNt = m_iEndNt;

			GameStation.iOutCardPeople=m_iOutCardPeople;	
			GameStation.iFirstOutPeople=m_iFirstOutPeople;	

			int iPos=0;
			for(int i=0;i<PLAY_COUNT;i++)
			{	//�û���������
				GameStation.iUserCardCount[i]=m_iUserCardCount[i];
				if (i == bDeskStation)
					::CopyMemory(&GameStation.iUserCardList[i],m_iUserCard[i],sizeof(BYTE)*m_iUserCardCount[i]);
				//�û��������˿���Ŀ
				GameStation.iDeskCardCount[i]=m_iDeskCardCount[i];
				::CopyMemory(&GameStation.iDeskCard[i],m_iDeskCard[i],sizeof(BYTE)*m_iDeskCardCount[i]);
			}

			//��ǰ�����
			GameStation.iBaseOutCount=m_iBaseOutCount;
			::CopyMemory(&GameStation.iBigCard,m_iDeskCard[m_iFirstOutPeople],sizeof(BYTE)*m_iBaseOutCount);
			
			//��������
			//int iSendSize=sizeof(GameStation)-sizeof(GameStation.iUserCardList)+sizeof(BYTE)+iPos;
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}

	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{
			GameStation_5 GameStation;
			::memset(&GameStation,0,sizeof(GameStation));
			//��Ϸ�汾�˶�
			GameStation.iVersion = DEV_HEIGHT_VERSION;			//��Ϸ�߰汾
			GameStation.iVersion2 = DEV_LOW_VERSION;			//�Ͱ汾
			//��Ϸ��������
			GameStation.iPlayCard = m_iPlayCard;				//�˿˸���
			GameStation.iPlayCount = m_iPlayCount;				//�˿�����
			GameStation.bKingCanReplace = m_bKingCanReplace;				//�Ƿ�����
			GameStation.iCardShape1 = m_iCardShape1;						//��������
			GameStation.iCardShape2 = m_iCardShape2;						//��������
			//����ʱ��
			GameStation.iBeginTime = m_iBeginTime;				//��Ϸ��ʼʱ������
			GameStation.iThinkTime = m_iThinkTime;				//��Ϸ˼��ʱ��
			GameStation.iEndWaitTime = m_EndWaitTime;			//�з�ʱ��
			//���䱶��
			GameStation.iDeskBasePoint = GetDeskBasePoint();	//���ӱ���
			GameStation.iRoomBasePoint = GetRoomBasePoint();	//���䱶��
			GameStation.iRunPublish = GetRunPublish();			//���ܿ۷�
			GameStation.bGameStation = m_bGameStation;
			//����ʱ̬��Ϣ
			GameStation.iUpGradePeople=m_iUpGradePeople;		
			GameStation.iBeenPlayGame=m_iBeenPlayGame;		
			GameStation.iPlayNtNum[0]=m_iPlayNtNum[0];		
			GameStation.iPlayNtNum[1]=m_iPlayNtNum[1];
			GameStation.iBeginNt = m_iBeginNt;
			GameStation.iEndNt = m_iEndNt;
			memcpy(&GameStation.iWinSequence,m_iWinSequence,sizeof(m_iWinSequence));

			//��������
			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	}
	return false;
}
//������ļ�ʱ��
void CServerGameDesk::KillAllTimer()
{
	//KillTimer(TIME_SEND_CARD);

	//KillTimer(TIMER_OUT_CARD);

	KillTimer(TIME_SEND_ALL_CARD);

	KillTimer(TIMER_WAIT_JIAOFEN);

	KillTimer(TIME_WAIT_NT);

	KillTimer(TIME_GAME_FINISH);

	KillTimer(TIME_WAIT_NEWTURN);

	KillTimer(TIME_WAIT_CONTRIBUTE);//�ֹ�����
	
	KillTimer(TIME_CHOICE_CARD);	//ѡ��

	KillTimer(TIME_WAIT_MOVE_BACK_CONTRIBUTE);//�˹�

	for(int i = 0 ; i<PLAY_COUNT ;i++)
	{
		KillTimer(TIMER_OUT_CARD +i);
	}

	
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	KillAllTimer();

	if ((bLastStation==GFF_FORCE_FINISH)||(bLastStation == GF_SALE) || bLastStation == GF_NO_CONTINUE)
	{
		m_iBeenPlayGame = 0;					//��ʼ����
		m_iUpGradePeople = -1;					//Ӯ��,�мұ����Ϲ�
		m_iDealtPeople = -1;					//����λ��
		
		//InitThisGame();
		memset(m_iPlayNtNum,m_iBeginNt,sizeof(m_iPlayNtNum));
		m_iHistory = 0;							//��ʷ��¼��
		memset(m_PlayANumber,0,sizeof(m_PlayANumber));              //�����Ҵ�m_iEndNt�Ĵ���
		::memset(m_iSequenceHistory,0,sizeof(m_iSequenceHistory));	//�����ʷ��¼
	}
	//һ�ֽ���������ͷ��Ϸ��δ�μ�¼
	if(bLastStation == GF_NO_CONTINUE)
	{
		::memset( m_iWinSequence,0,sizeof( m_iWinSequence));
	}
	/////��������
	m_bInSetCard      = FALSE;
	::memset(m_SetCardCountTemp,0,sizeof(m_SetCardCountTemp));			
	::memset(m_SetCardTemp,0,sizeof(m_SetCardTemp));
	///////////////////////

	m_iGrade2=-1;
	m_iNtPeople = -1;							//�Ϲ�������
	m_iLeaveAgree=0;
	m_iTempPoint=0;
	m_iBaseOutCount=0;
	m_iFirstOutPeople = -1;
	m_iOutCardPeople = -1;
	m_iSendCardPos=0;
	m_iOverContribute = 0;
	//��չ����
	m_bAdvanceFinish = false;
	//��չ��������

	//�й�(ϵͳ�й�,������Զ�й�)
	memset(m_bAutoOutCard,0,sizeof(m_bAutoOutCard));
	//��������
	memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	//���������
	memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));
	//�����������
	::memset(m_bCardOfFriend,0,sizeof(m_bCardOfFriend));
	memset(m_bIsCut,0,sizeof(m_bIsCut));
	//������������ʼ��
	::memset(m_iSequence,0,sizeof(m_iSequence));
	//�зֳ�ʼ��
	::memset(m_iJiaofen,-1,sizeof(m_iJiaofen));
	//�÷�
	::memset(m_iUserGetPoint,0,sizeof(m_iUserGetPoint));
	//��������
	::memset(m_iPointCardCount,0,sizeof(m_iPointCardCount));
	/////////////����
	//��ʼ������
	::memset(m_iAwardPoint,0,sizeof(m_iAwardPoint));
	//ͷ����
	::memset(m_iHeads,0,sizeof(m_iHeads));
	::memset(m_ChangeCard,0,sizeof(m_ChangeCard));
	return TRUE;
}

//��Ϸ��ʼ
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	
	if (__super::GameBegin(bBeginFlag)==false) 
		return false;
	//����״̬
	m_iSendCardPos=0;
	m_iBeenPlayGame++;
	m_bGameStation=GS_SEND_CARD;
	m_Logic.SetNTCardNum(m_iPlayNtNum[((m_iUpGradePeople == -1)?0:m_iUpGradePeople)%2]);  //������������
	BYTE RepCardList[18]={0};
	RepCardList[0] = m_Logic.GetCardFromHua(UG_HONG_TAO,m_Logic.GetNTCardNum());
	m_Logic.SetReplaceCard(RepCardList,1);

	//���Ϳ�ʼ��Ϣ
	BeginUpgradeStruct BeginMessage;

	if(m_iUpGradePeople == -1)
	{
		
		m_iUpGradePeople = rand()%4;

	}
	BeginMessage.iUpgradeStation = m_iUpGradePeople; 
	BeginMessage.iBeenPlayGame = m_iBeenPlayGame;
	BeginMessage.iStation[0] = m_iPlayNtNum[0];//rand()%13 + 2;
	BeginMessage.iStation[1] = m_iPlayNtNum[1];//rand()%13 + 2;
	
	//��ʱׯ��
	for (int i=0;i < 4;i++) 
		SendGameData(i,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_BEGIN_UPGRADE,0);

	SendWatchData(m_bMaxPeople,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_BEGIN_UPGRADE,0);

	//�ַ��˿�
	BYTE iCardArray[162];
	//׼���˿�
	m_Logic.RandCard(iCardArray,m_iPlayCount,m_bHaveKing);
	
	const int  USER_HAND_CARD = 27;

	if (m_bInSetCard && m_bCanSetCard)
	{
		for (int i=0;i<4;i++)//����������
		{

			::CopyMemory(m_iUserCard[i],m_SetCardTemp[i],sizeof(BYTE)*(m_SetCardCountTemp[i]));		
			m_iUserCardCount[i] = m_SetCardCountTemp[i];	
		}
	}
	else
	{
		for (int i=0;i<4;i++)//����������
		{
			::CopyMemory(m_iUserCard[i],&iCardArray[m_iUserCount*i],sizeof(BYTE)*(m_iUserCount));
			m_iUserCardCount[i] = m_iUserCount;	
		}
	}
	
	//m_iUserCard[1][0] =   0x4F;
	//m_iUserCard[3][0] =   0x4F;
	Ready();
	SetTimer(TIME_SEND_ALL_CARD,1000);
	return TRUE;
}
//׼������
BOOL CServerGameDesk::Ready()
{
	//��ʱׯ��(��һ��ץ����)
	if(m_iUpGradePeople == -1)
	{
	//	srand(time(0));
		m_iUpGradePeople = 0 ;//rand()%4;
	}
	ReadyStruct ready;
	::memset(&ready,0,sizeof(ready));
	ready.bDeskStation = m_iUpGradePeople ;
	for(int i = 0;i < 4;i ++)
		SendGameData(i,&ready,sizeof(ready),MDM_GM_GAME_NOTIFY,ASS_READY,0);
	SendWatchData(m_bMaxPeople,&ready,sizeof(ready),MDM_GM_GAME_NOTIFY,ASS_READY,0);

	return TRUE;
}
//һ�η���������
BOOL CServerGameDesk::SendAllCard( BYTE iStyle)
{

	KillTimer(TIME_SEND_ALL_CARD);


	SendAllStruct SendAll;
	::memset(&SendAll,0,sizeof(SendAll));
	int iPos=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
			
		SendAll.iUserCardCount[i] = m_iUserCardCount[i];
		memset(SendAll.iUserCardCount,m_iUserCount,sizeof(SendAll.iUserCardCount));
		memset(SendAll.iUserCardList,0,sizeof(SendAll.iUserCardList));
		::CopyMemory(&SendAll.iUserCardList[i],m_iUserCard[i],sizeof(m_iUserCard[i]));
		SendGameData(i,&SendAll,sizeof(SendAll),MDM_GM_GAME_NOTIFY,ASS_SEND_ALL_CARD,0);
	}

	m_iSendCardPos++;

	SendCardFinish();

	return 0;
}

//���ƽ���
BOOL CServerGameDesk::SendCardFinish()
{
	//���ͷ��ƽ���
	for(int j=0;j<m_bMaxPeople;j++)
	{
		SendGameData(j,MDM_GM_GAME_NOTIFY,ASS_SEND_FINISH,0);
	}
	SendWatchData(m_bMaxPeople,MDM_GM_GAME_NOTIFY,ASS_SEND_FINISH,0);

	//m_bGameStation=GS_WAIT_BACK;			//ת����Ϸ״̬

	SendContribute();		//������Ϣ
	return true;
}
//���ͽ�����Ϣ
BOOL CServerGameDesk::SendContribute()
{
/***********************����ʹ��**************************/
	//m_iHistory = 1;
	//m_iBeenPlayGame = 2;
/*********************************************************/

	m_bGameStation = GS_CONTRIBUTE;                         //������ϷΪ����״̬

	if(m_iBeenPlayGame == 1)								//��һ����Ϸ
	{
		BeginPlayUpGrade();									//ת����Ϸ״̬
		return TRUE;
	}
	
	m_iStateFlag = 1;										//��ǰ��Ϸ״̬�ֽ�

	BYTE iWiner1 = GetPostionBySequence(1);	    			///ͷ��
	BYTE iWiner2 = GetPostionBySequence(2);					///����
	BYTE iLose3 = GetPostionBySequence(3);                  ///����  
	BYTE iLose4 = GetPostionBySequence(4);                  ///����

	//BYTE iLose3,iLose4;
	//if (iWiner2 != (iWiner1+2)%PLAY_COUNT)
	//{
	//	BYTE iLose3 = GetPostionBySequence(3);//���
	//	if (iWiner2 != (iLose3+2)%PLAY_COUNT)
	//		 iLose4 = (iWiner2+2)%PLAY_COUNT;
	//	else
	//		 iLose4 = (iWiner1+2)%PLAY_COUNT;
	//}
	//else
	//{
	//	 iLose3 = (iWiner1+1)%PLAY_COUNT;
	//	 iLose4 = (iWiner2+1)%PLAY_COUNT;
	//}

	int count = 2;//�Ϲ�����
	if (iWiner2 != (iWiner1+2)%PLAY_COUNT)                      //�ڶ�Ӯ�Ҳ��ǶԼ�
          count = 1;
	if(GainstContribute(iLose4,count))							//�����ɹ�
	{
		GainstContributeStruct gainst;

		for(int i = 0;i < 4;i ++)
			SendGameData(i,&gainst,sizeof(gainst),MDM_GM_GAME_NOTIFY,ASS_GAINST_CONTRIBUTE,0);
		SendWatchData(m_bMaxPeople,&gainst,sizeof(gainst),MDM_GM_GAME_NOTIFY,ASS_GAINST_CONTRIBUTE,0);
		
		BeginPlayUpGrade();									//ת����Ϸ״̬
		return TRUE;
	}

	int rev = 0;
	//������Ϣ
	ContributeStruct contribute;
	::memset(&contribute,0,sizeof(contribute));
	if (iWiner2 == (iWiner1+2)%PLAY_COUNT)                 //˫�� ��ǰ������һ��
	{
		contribute.bLoseDesk1 = iLose3;						//��һ���
		contribute.bLoseDesk2 = iLose4;
		contribute.bWinerDesk1 = iWiner1;
		contribute.bWinerDesk2 = iWiner2;
		contribute.PlayCount = 2;
		m_ContributeCount = 2;
	}
	else
	{
		contribute.bLoseDesk1 = iLose4;
		contribute.bWinerDesk1 = iWiner1;
		contribute.bLoseDesk2 = 255;
		contribute.bWinerDesk2 = 255;
		m_ContributeCount = 1;
		contribute.PlayCount = 1;
	}
	for(int i = 0;i < 4;i ++)
		SendGameData(i,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_CONTRIBUTE,0);
	SendWatchData(m_bMaxPeople,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_CONTRIBUTE,0);
	//���ý���
	m_iTempTime = 32;              ///����ʱ���Ŀռ���Ϊ32
	SetTimer(TIME_WAIT_CONTRIBUTE,1000);
	return TRUE;
}

///����˴����Զ�����
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
		m_Logic.AutoOutCard(m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation],
			m_iDeskCard[m_iFirstOutPeople],m_iDeskCardCount[m_iFirstOutPeople],
			bCardList,iCardCount,FALSE);
	}
	
	
	return UserOutCard(bDeskStation,bCardList,iCardCount);
}

//����(���λ��)
BOOL CServerGameDesk::GainstContribute(BYTE bLose, int count)
{
	if (count == 2)
	{
		BYTE bTwo = ( bLose + 2 )%4;
		//����ж���è�ɿ���
		if(m_Logic.GetCountBySpecifyCard(m_iUserCard[bLose],m_iUserCardCount[bLose],0x4F)
			+m_Logic.GetCountBySpecifyCard(m_iUserCard[bTwo],m_iUserCardCount[bTwo],0x4F) == 2)
			return TRUE;
	}
	if (count ==1)
	{
		if(m_Logic.GetCountBySpecifyCard(m_iUserCard[bLose],m_iUserCardCount[bLose],0x4F) == 2)
			return TRUE;
	}
	return FALSE;
}
//�ֹ�����
BOOL CServerGameDesk::UserContribute(BYTE bDeskStation,BYTE bCard)
{
	if (m_iUserCardCount[bDeskStation] != m_iUserCount||
		(bDeskStation != GetPostionBySequence(3) && bDeskStation != GetPostionBySequence(4)))
	{
		return TRUE;
	}
	//�����
	BYTE bMaxCard = m_Logic.GetMaxCard(m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation]);			//��һ�����������ׯ��
	
	//������δ�����
	if(m_Logic.GetCardBulk(bMaxCard) != m_Logic.GetCardBulk(bCard))
	{
		return FALSE;
	}
	int rev = 0;
	if(!(rev = m_Logic.RemoveCard(&(bCard),1,m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation])))	//ɾ����
		return FALSE;		
	m_iUserCardCount[bDeskStation] -= 1;
	m_iOverContribute++;
	//���ͽ������
	MoveBackContributeStruct contribute;
	::memset(&contribute,0,sizeof(contribute));
	contribute.bDeskStation = bDeskStation;
	contribute.bCard  = bCard;
	m_ChangeCard [bDeskStation] = bCard;        //����ĳһ��λ���Ͻ�������
	for(int i = 0;i < 4;i ++)
	{
		SendGameData(i,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_CONTRIBUTE_RESLULT,0);
	}
	SendWatchData(m_bMaxPeople,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_CONTRIBUTE_RESLULT,0);

	if (m_iOverContribute == m_ContributeCount)
	{
		KillTimer(TIME_WAIT_CONTRIBUTE);
		ContributeFinish();
		//SystemContribute();		
	}
	return TRUE;
}
//Ӯ���˹�
BOOL CServerGameDesk::UserMoveBackContribute(BYTE bDeskStation,BYTE bCard)
{
	if (m_iUserCardCount[bDeskStation] == m_iUserCount||
		(bDeskStation != GetPostionBySequence(1) && bDeskStation != GetPostionBySequence(2)))
	{
		return TRUE;
	}
	int rev = 0;
	if(!(rev = m_Logic.RemoveCard(&(bCard),1,m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation])))	//ɾ����
		return FALSE;
	m_iUserCardCount[bDeskStation] -= 1;
	m_iOverContribute --;
	//���ͽ������
	MoveBackContributeStruct contribute;
	::memset(&contribute,0,sizeof(contribute));
	contribute.bDeskStation = bDeskStation;
	contribute.bCard  = bCard;
	m_ChangeCard[bDeskStation] = bCard;   //����ĳһ��λ���Ͻ�������
	
	for(int i = 0;i < 4;i ++)
		SendGameData(i,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_MOVE_BACK_RESULT,0);
	SendWatchData(m_bMaxPeople,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_MOVE_BACK_RESULT,0);

	if (m_iOverContribute == 0)
	{
		KillTimer(TIME_WAIT_MOVE_BACK_CONTRIBUTE);
		MoveBackContributeFinish();
	}
	return TRUE;
}

//ϵͳ����ʱδ������
BOOL CServerGameDesk::SystemContribute()
{
	/// AfxMessageBox("ϵͳ����ʱδ������");

	BYTE iWiner1 = GetPostionBySequence(1);	    			///ͷ��
	BYTE iWiner2 = GetPostionBySequence(2);					///����
	BYTE iLose2 = GetPostionBySequence(3);                  ///����
	BYTE iLose1 = GetPostionBySequence(4);                  ///����  
	


	//BYTE iLose1; 						                    //���
	//BYTE iLose2;						                    //�ڶ����

	//if (iWiner2 != (iWiner1+2)%PLAY_COUNT)
	//{
	//	BYTE iLose2 = GetPostionBySequence(3);              //���
	//	if (iWiner2 != (iLose2+2)%PLAY_COUNT)
	//		iLose1 = (iWiner2+2)%PLAY_COUNT;
	//	else
    //       iLose1 = (iWiner1+2)%PLAY_COUNT;
	//}
	//else
	//{
	//	iLose1 = (iWiner1+1)%PLAY_COUNT;
	//	iLose2 = (iWiner2+1)%PLAY_COUNT;
	//}

	//�����һ���δ����
	if(m_iUserCardCount[iLose1] == m_iUserCount)			
	{
		//�����
		BYTE bMaxCard = m_Logic.GetMaxCard(m_iUserCard[iLose1],m_iUserCardCount[iLose1]);			//��һ�����������ׯ��
		int rev = 0;
		if(!(rev = m_Logic.RemoveCard(&(bMaxCard),1,m_iUserCard[iLose1],m_iUserCardCount[iLose1])))	//ɾ����
			return FALSE;		
		m_iUserCardCount[iLose1] -= 1;

		//���ͽ������
		MoveBackContributeStruct contribute;
		::memset(&contribute,0,sizeof(contribute));
		contribute.bDeskStation = iLose1;
		contribute.bCard  = bMaxCard;
		//����ĳһ��λ���Ͻ�������
		m_ChangeCard[iLose1] = bMaxCard;
		for(int i = 0;i < 4;i ++)
			SendGameData(i,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_CONTRIBUTE_RESLULT,0);
		SendWatchData(m_bMaxPeople,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_CONTRIBUTE_RESLULT,0);

	}

	//����ڶ����δ����
	if(m_ContributeCount==2 && m_iUserCardCount[iLose2] == m_iUserCount)		
	{
		//�����
		BYTE bMaxCard = m_Logic.GetMaxCard(m_iUserCard[iLose2],m_iUserCardCount[iLose2]);			//��һ�����������ׯ��
		int rev = 0;
		if(!(rev = m_Logic.RemoveCard(&(bMaxCard),1,m_iUserCard[iLose2],m_iUserCardCount[iLose2])))	//ɾ����
			return FALSE;		
		m_iUserCardCount[iLose2] -= 1;

		//���ͽ������
		MoveBackContributeStruct contribute;
		::memset(&contribute,0,sizeof(contribute));
		contribute.bDeskStation = iLose2;
		contribute.bCard  = bMaxCard;
		//����ĳһ��λ���Ͻ�������
		m_ChangeCard [iLose2] = bMaxCard;

		for(int i = 0;i < 4;i ++)
			SendGameData(i,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_CONTRIBUTE_RESLULT,0);
		SendWatchData(m_bMaxPeople,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_CONTRIBUTE_RESLULT,0);
	}
	
	m_iOverContribute = m_ContributeCount;      ///��ֹϵͳ����ʱû�� ��������û������
	//�������
	ContributeFinish();
	return TRUE;
}
//�������
BOOL CServerGameDesk::ContributeFinish()
{

	//���ͽ�������
	for(int i=0;i<4;i++)
	{
		SendGameData(i,MDM_GM_GAME_NOTIFY,ASS_CONTRIBUTE_FINISH,0);
	}
	SendWatchData(m_bMaxPeople,MDM_GM_GAME_NOTIFY,ASS_CONTRIBUTE_FINISH,0);
	
	//���Ϲ����ƽ��зַ���
	BYTE iWiner1 = GetPostionBySequence(1);					//��Ӯ���Ƴ����
	BYTE iWiner2 = GetPostionBySequence(2);					//�ڶ�Ӯ��

	BYTE iLose1;        //���
	BYTE iLose2;		//�ڶ����

	if (iWiner2 != (iWiner1+2)%PLAY_COUNT)                  //�ڶ�Ӯ�Ҳ��ǶԼ�
	{
		BYTE iLose2 = GetPostionBySequence(3);             //���
		if (iWiner2 != (iLose2+2)%PLAY_COUNT)
			iLose1 = (iWiner2+2)%PLAY_COUNT;
		else
			iLose1 = (iWiner1+2)%PLAY_COUNT;
	}
	else
	{
		iLose1 = (iWiner1+1)%PLAY_COUNT;
		iLose2 = (iWiner2+1)%PLAY_COUNT;
	}

	if (m_ContributeCount == 1)
	{
		//��һ���ȳ���
		m_iNtPeople = iLose1;
		//����һ���ƽ���������
		m_iUserCard[iWiner1][m_iUserCardCount[iWiner1]++] = m_ChangeCard[iLose1];
		ChangeCardStruct change;
		::memset(&change,0,sizeof(change));
		change.bDeskFrom = iLose1;
		change.bDeskTo =iWiner1;
		change.bCard = m_ChangeCard[iLose1];
		for(int i=0;i<4;i++)
			SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
		SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
	}
	else//���������Ҫ����
	{
		//��һ���Ϲ��ƴ�
		if(m_Logic.GetCardBulk(m_ChangeCard[iLose1]) > m_Logic.GetCardBulk(m_ChangeCard[iLose2]))
		{
			//��һ���ȳ���
			m_iNtPeople = iLose1;
			//����һ���ƽ���������
			m_iUserCard[iWiner1][m_iUserCardCount[iWiner1]++] = m_ChangeCard[iLose1];
			ChangeCardStruct change;
			::memset(&change,0,sizeof(change));
			change.bDeskFrom = iLose1;
			change.bDeskTo =iWiner1;
			change.bCard = m_ChangeCard[iLose1];
			for(int i=0;i<4;i++)
				SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
			SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);

			//���ڶ����ƽ���������
			if (m_ContributeCount == 2)
			{
				m_iUserCard[iWiner2][m_iUserCardCount[iWiner2]++] = m_ChangeCard[iLose2];
				//ChangeCardStruct change;
				change.bDeskFrom = iLose2;
				change.bDeskTo =iWiner2;
				change.bCard = m_ChangeCard[iLose2];
				for(int i=0;i<4;i++)
					SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
				SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
			}
		}
		else  //��һ���Ϲ���С
			//if(m_Logic.GetCardBulk(m_ChangeCard[iLose1]) < m_Logic.GetCardBulk(m_ChangeCard[iLose2]))
			{
				//��һ���ȳ���
				m_iNtPeople = iLose2;
				//����һ���ƽ���������
				m_iUserCard[iWiner1][m_iUserCardCount[iWiner1]++] = m_ChangeCard[iLose2];
				ChangeCardStruct change;
				::memset(&change,0,sizeof(change));
				change.bDeskFrom = iLose2;
				change.bDeskTo =iWiner1;
				change.bCard = m_ChangeCard[iLose2];
				for(int i=0;i<4;i++)
					SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
				SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);

				if (m_ContributeCount == 2)
				{
					//���ڶ����ƽ���������
					m_iUserCard[iWiner2][m_iUserCardCount[iWiner2]++] = m_ChangeCard[iLose1];
					//ChangeCardStruct change;
					change.bDeskFrom = iLose1;
					change.bDeskTo =iWiner2;
					change.bCard = m_ChangeCard[iLose1];
					for(int i=0;i<4;i++)
						SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
					SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);

				}
			}//201-6-28 �޸����Ź�����ȵĻ���ϵͳ���ƣ����������ѡ
		//	else     //�����Ϲ������
		//	{
		//		//δ����ͬ����С
		//		ContributeStruct contribute;
		//		::memset(&contribute,0,sizeof(contribute));
		//		if (m_ContributeCount == 2)
		//		{
		//			contribute.bLoseDesk1 = iLose1;											//��һ���
		//			contribute.bLoseCard1 = m_ChangeCard[iLose1];
		//			contribute.bLoseDesk2 = iLose2;
		//			contribute.bLoseCard2 = m_ChangeCard[iLose2];
		//			contribute.bWinerDesk1 = iWiner1;
		//			contribute.bWinerDesk2 = iWiner2;
		//		}
		//		else
		//		{
		//			contribute.bLoseDesk1 = iLose1;											//��һ���
		//			contribute.bLoseCard1 = m_ChangeCard[iLose1];
		//			contribute.bWinerDesk1 = iWiner1;
		//		}
		//		for(int i = 0;i < 4;i ++)
		//			SendGameData(i,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_CHOICE_CARD,0);
		//	SendWatchData(m_bMaxPeople,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_CHOICE_CARD,0);
		//	//����ѡ��ʱ��
		//	SetTimer(TIME_CHOICE_CARD,m_EndWaitTime*1000);		
		//	return true;
		//}
	}

	SendMoveBackContribute();
	return TRUE;
}
//���ѡ��(˭ѡ��,ѡ��˭����,ѡ�Ƶ�ֵ
BOOL CServerGameDesk::UserChoiceCard(BYTE bDeskStation,BYTE bDest,BYTE bCard)
{	
	if (m_iUserCardCount[bDeskStation] != m_iUserCount)
	{
		return TRUE;
	}
	BYTE iWiner1 = GetPostionBySequence(1);					//��Ӯ���Ƴ����
	BYTE iWiner2 = GetPostionBySequence(2);					//�ڶ�Ӯ��
	BYTE iLose1 = GetPostionBySequence(4); 						//���
	BYTE iLose2 = GetPostionBySequence(3);						//�ڶ����

	//BYTE iLose1; 						//���
	//BYTE iLose2;						//�ڶ����

	//if (iWiner2 != (iWiner1+2)%PLAY_COUNT)
	//{
	//	BYTE iLose2 = GetPostionBySequence(3);              //���
	//	if (iWiner2 != (iLose2+2)%PLAY_COUNT)
	//		iLose1 = (iWiner2+2)%PLAY_COUNT;
	//	else
	//		iLose1 = (iWiner1+2)%PLAY_COUNT;
	//}
	//else
	//{
	//	iLose1 = (iWiner1+1)%PLAY_COUNT;
	//	iLose2 = (iWiner2+1)%PLAY_COUNT;

	//}

	BYTE bDestPeople = (bDest == 1)?iLose1:iLose2;			//��ѡ���Ƶ�λ�û�����
	//ѡ����
	if(m_ChangeCard[bDestPeople] != bCard)
	{
		/***********************************************��ӡ��־********************************************/
		CString str;
		str.Format("bDeskStation=%d,bDestPeople=%d,m_ChangeCard[%d]=%d,bCard=%d",bDeskStation,bDestPeople,bDestPeople,m_ChangeCard[bDestPeople],bCard);
		DebugPrintf(str);

		/***********************************************��ӡ��־********************************************/
		return FALSE;
	}

	if(bDeskStation == iLose1 && bDeskStation == iLose2 )
	{
		return FALSE;
	}
	//��һ���ȳ���
	m_iNtPeople = bDestPeople;
	//�ڶ�Ӯ��
	//BYTE lose2 = (bDestPeople+2)%4;

	//����һ���ƽ���������
	//m_iUserCard[iWiner1][m_iUserCardCount[iWiner1]++] = m_ChangeCard[bDestPeople];
	
	///���Ƹ�ѡ�Ƶ���

	m_iUserCard[bDeskStation][m_iUserCardCount[bDeskStation]++] = m_ChangeCard[bDestPeople];


	ChangeCardStruct change;
	::memset(&change,0,sizeof(change));
	change.bDeskFrom = bDestPeople;
	change.bDeskTo = bDeskStation;
	change.bCard = m_ChangeCard[bDestPeople];
	change.bDest = bDest;
	
	for(int i=0;i<4;i++)
	{
		SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
	}
	SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
	
	m_ChoiceNumber++;

	/////һ�����ѡ�ƺ󣬽�ʣ�µ���ֱ�ӷ�����һ�����
	//m_iUserCard[iWiner2][m_iUserCardCount[iWiner2]++] = m_ChangeCard[(bDestPeople+2)%4];
	//memset(&change,0,sizeof(change));              ///��սṹ��
	//change.bDeskFrom = (bDestPeople+2)%4;
	//change.bDeskTo = (bDeskStation+2)%4;
	//change.bCard = m_ChangeCard[(bDestPeople+2)%4];

	//for(int i=0;i<4;i++)
	//{
	//	SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
	//}
	//SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);

	
	if(m_ChoiceNumber == m_ContributeCount)   ///������Ҷ���ѡ��
	{
		///����ѡ�ƽ�����Ϣ����Ϊ�����ѡ�Ƶ�ֵ��֮ǰ���ѷ��ͣ���������ֻ֪ͨ
		ChoiceCardStruct choice;
		::memset(&choice,0,sizeof(choice));
		//���ͽ��
		for(int i=0;i<4;i++)
		{
			SendGameData(i,&choice,sizeof(choice),MDM_GM_GAME_NOTIFY,ASS_CHOICE_CARD_RESULT,0);
		}
		SendWatchData(m_bMaxPeople,&choice,sizeof(choice),MDM_GM_GAME_NOTIFY,ASS_CHOICE_CARD_RESULT,0);

		KillTimer(TIME_CHOICE_CARD);
		SendMoveBackContribute();
	}
	return TRUE;
}
	//ϵͳѡ��
BOOL CServerGameDesk::SystemChoiceCard()
{
	//���Ϲ����ƽ��зַ���
	BYTE iWiner1 = GetPostionBySequence(1);					//��Ӯ���Ƴ����
	BYTE iWiner2 = GetPostionBySequence(2);							//�ڶ�Ӯ��
	BYTE iLose1 = GetPostionBySequence(4); 						//���
	BYTE iLose2 = GetPostionBySequence(3);						//�ڶ����

	//BYTE iLose1; 						//���
	//BYTE iLose2;						//�ڶ����

	//if (iWiner2 != (iWiner1+2)%PLAY_COUNT)
	//{
	//	BYTE iLose2 = GetPostionBySequence(3);//���
	//	if (iWiner2 != (iLose2+2)%PLAY_COUNT)
	//		iLose1 = (iWiner2+2)%PLAY_COUNT;
	//	else
	//		iLose1 = (iWiner1+2)%PLAY_COUNT;
	//}
	//else
	//{
	//	iLose1 = (iWiner1+1)%PLAY_COUNT;
	//	iLose2 = (iWiner2+1)%PLAY_COUNT;
	//}						//�ڶ����


	//��һ���ȳ���
	m_iNtPeople = iLose1;
	
	///����һ���ƽ���������
	if(m_iUserCardCount[iWiner1] == m_iUserCount)    ///��ֹ�����ѡ�ƺ�ϵͳ�ٴθ����ѡ��     
	{		
		m_iUserCard[iWiner1][m_iUserCardCount[iWiner1]++] = m_ChangeCard[iLose1];
		ChangeCardStruct change;
		::memset(&change,0,sizeof(change));
		change.bDeskFrom = iLose1;
		change.bDeskTo =iWiner1;
		change.bCard = m_ChangeCard[iLose1];
		for(int i=0;i<4;i++)
			SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
		SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
	}

	///���ڶ����ƽ���������
	if (m_iUserCardCount[iWiner2] == m_iUserCount)
	{		
		m_iUserCard[iWiner2][m_iUserCardCount[iWiner2]++] = m_ChangeCard[iLose2];
		ChangeCardStruct change;
		change.bDeskFrom = iLose2;
		change.bDeskTo =iWiner2;
		change.bCard = m_ChangeCard[iLose2];
		for(int i=0;i<4;i++)
			SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
		SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
	}

	///����ѡ�ƽ�����Ϣ����Ϊ�����ѡ�Ƶ�ֵ��֮ǰ���ѷ��ͣ���������ֻ֪ͨ
	ChoiceCardStruct choice;
	::memset(&choice,0,sizeof(choice));
	//���ͽ��
	for(int i=0;i<4;i++)
	{
		SendGameData(i,&choice,sizeof(choice),MDM_GM_GAME_NOTIFY,ASS_CHOICE_CARD_RESULT,0);
	}
	SendWatchData(m_bMaxPeople,&choice,sizeof(choice),MDM_GM_GAME_NOTIFY,ASS_CHOICE_CARD_RESULT,0);

	///���ͽ�����Ϣ
	SendMoveBackContribute();
	return TRUE;
}

//�����˹���Ϣ
BOOL CServerGameDesk::SendMoveBackContribute()
{
	m_bGameStation = GS_BACKCONTRIBUT;     ///�˹�״̬

	m_ChoiceNumber = 0;				//ѡ����ɺ�ͳ������
	BYTE iWiner1 = GetPostionBySequence(1);					//��Ӯ���Ƴ����
	BYTE iWiner2 = GetPostionBySequence(2);							//�ڶ�Ӯ��
	BYTE iLose1 = GetPostionBySequence(4); 						//���
	BYTE iLose2 = GetPostionBySequence(3);						//�ڶ����

	//if (iWiner2 != (iWiner1+2)%PLAY_COUNT)
	//{
	//	BYTE iLose2 = GetPostionBySequence(3);//���
	//	if (iWiner2 != (iLose2+2)%PLAY_COUNT)
	//		iLose1 = (iWiner2+2)%PLAY_COUNT;
	//	else
	//		iLose1 = (iWiner1+2)%PLAY_COUNT;
	//}
	//else
	//{
	//	iLose1 = (iWiner1+1)%PLAY_COUNT;
	//	iLose2 = (iWiner2+1)%PLAY_COUNT;
	//}
	
	//������Ϣ
	ContributeStruct contribute;
	::memset(&contribute,0,sizeof(contribute));
	if (m_ContributeCount ==2)
	{
		contribute.bLoseDesk1 = iLose1;											//��һ���
		contribute.bLoseDesk2 = iLose2;
		contribute.bWinerDesk1 = iWiner1;
		contribute.bWinerDesk2 = iWiner2;
	}
	else
	{
		contribute.bLoseDesk1 = iLose1;	
		contribute.bWinerDesk1 = iWiner1;
		contribute.bWinerDesk2 = 255;
		contribute.bLoseDesk2 = 255;
	}
	for(int i = 0;i < 4;i ++)
		SendGameData(i,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_MOVE_BACK_CONTRIBUTE,0);
	SendWatchData(m_bMaxPeople,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_MOVE_BACK_CONTRIBUTE,0);
	//�����˹�
	m_iTempTime = 32;              ///����ʱ���Ŀռ���Ϊ32

	SetTimer(TIME_WAIT_MOVE_BACK_CONTRIBUTE,1000);

	return TRUE;
}


//ϵͳ����ʱδ�˹���
BOOL CServerGameDesk::SystemMoveBackContribute()
{
	BYTE iWiner1 = GetPostionBySequence(1);					//��Ӯ���Ƴ����
	BYTE iWiner2 = GetPostionBySequence(2);							//�ڶ�Ӯ��
	BYTE iLose1 = GetPostionBySequence(4); 						//���
	BYTE iLose2 = GetPostionBySequence(3);						//�ڶ����

	//if (iWiner2 != (iWiner1+2)%PLAY_COUNT)
	//{
	//	 iLose2 = GetPostionBySequence(3);//���
	//	if (iWiner2 != (iLose2+2)%PLAY_COUNT)
	//		iLose1 = (iWiner2+2)%PLAY_COUNT;
	//	else
	//		iLose1 = (iWiner1+2)%PLAY_COUNT;
	//}
	//else
	//{
	//	iLose1 = (iWiner1+1)%PLAY_COUNT;
	//	iLose2 = (iWiner2+1)%PLAY_COUNT;
	//}

	//�����һ���δ�˹�
	if(m_iUserCardCount[iWiner1] > m_iUserCount)			
	{
		//��С��
		BYTE bMinCard = m_Logic.GetMinCard(m_iUserCard[iWiner1],m_iUserCardCount[iWiner1]);//������������С��
		int rev = 0;
		if(!(rev = m_Logic.RemoveCard(&(bMinCard),1,m_iUserCard[iWiner1],m_iUserCardCount[iWiner1])))	//ɾ����
			return FALSE;		
		m_iUserCardCount[iWiner1] -= 1;

		//���ͽ������
		MoveBackContributeStruct contribute;
		::memset(&contribute,0,sizeof(contribute));
		contribute.bDeskStation = iWiner1;
		contribute.bCard  = bMinCard;
		//����ĳһ��λ���Ͻ�������
		m_ChangeCard [iWiner1] = bMinCard;

		for(int i = 0;i < 4;i ++)
			SendGameData(i,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_MOVE_BACK_RESULT,0);
		SendWatchData(m_bMaxPeople,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_MOVE_BACK_RESULT,0);
	}
	if (m_ContributeCount==2)
	{
		//����ڶ�Ӯ��δ����
		if(m_iUserCardCount[iWiner2] > m_iUserCount)		
		{
			//��С��
			BYTE bMinCard = m_Logic.GetMinCard(m_iUserCard[iWiner2],m_iUserCardCount[iWiner2]);//���ζԼ�������С��
			int rev = 0;
			if(!(rev = m_Logic.RemoveCard(&(bMinCard),1,m_iUserCard[iWiner2],m_iUserCardCount[iWiner2])))	//ɾ����
				return FALSE;		
			m_iUserCardCount[iWiner2] -= 1;

			//���ͽ������
			MoveBackContributeStruct contribute;
			::memset(&contribute,0,sizeof(contribute));
			contribute.bDeskStation = iWiner2;
			contribute.bCard  = bMinCard;
			//����ĳһ��λ���Ͻ�������
			m_ChangeCard [iWiner2] = bMinCard;
			for(int i = 0;i < 4;i ++)
				SendGameData(i,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_MOVE_BACK_RESULT,0);
			SendWatchData(m_bMaxPeople,&contribute,sizeof(contribute),MDM_GM_GAME_NOTIFY,ASS_MOVE_BACK_RESULT,0);
		}
	}

	MoveBackContributeFinish();
	return TRUE;
}
//�˹����
BOOL CServerGameDesk::MoveBackContributeFinish()
{
	
	//���˹����Ƽӵ��������
	BYTE iWiner1 = GetPostionBySequence(1);					//��Ӯ���Ƴ����
	BYTE iWiner2 = GetPostionBySequence(2);							//�ڶ�Ӯ��
	BYTE iLose1 = GetPostionBySequence(4); 						//���
	BYTE iLose2 = GetPostionBySequence(3);						//�ڶ����

	//��һ���Ϲ��ƴ�
	if(m_Logic.GetCardBulk(m_ChangeCard[iLose1]) > m_Logic.GetCardBulk(m_ChangeCard[iLose2]))
	{
		//��һ��ҵõ���һʤ�ߵ���
		m_iUserCard[iLose1][m_iUserCardCount[iLose1]++] = m_ChangeCard[iWiner1];
		ChangeCardStruct change;
		::memset(&change,0,sizeof(change));
		change.bDeskFrom = iWiner1;
		change.bDeskTo =iLose1;
		change.bCard = m_ChangeCard[iWiner1];
		for(int i=0;i<4;i++)
			SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
		SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);

		//�ڶ���ҵõ��ڶ�ʤ�ߵ���
		if (m_ContributeCount==2)
		{
			m_iUserCard[iLose2][m_iUserCardCount[iLose2]++] = m_ChangeCard[iWiner2];
			memset(&change,0,sizeof(change));
			change.bDeskFrom = iWiner2;
			change.bDeskTo =iLose2;
			change.bCard = m_ChangeCard[iWiner2];
			for(int i=0;i<4;i++)
				SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
			SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);

		}
	}else
		if(m_Logic.GetCardBulk(m_ChangeCard[iLose1]) < m_Logic.GetCardBulk(m_ChangeCard[iLose2]))
		{
			//����һ���ƽ���������
			m_iUserCard[iLose2][m_iUserCardCount[iLose2]++] = m_ChangeCard[iWiner1];
			ChangeCardStruct change;
			::memset(&change,0,sizeof(change));
			change.bDeskFrom = iWiner1;
			change.bDeskTo =iLose2;
			change.bCard = m_ChangeCard[iWiner1];
			for(int i=0;i<4;i++)
				SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
			SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);

			//���ڶ����ƽ���������
			if (m_ContributeCount==2)
			{
				m_iUserCard[iLose1][m_iUserCardCount[iLose1]++] = m_ChangeCard[iWiner2];
				memset(&change,0,sizeof(change));
				change.bDeskFrom = iWiner2;
				change.bDeskTo =iLose1;
				change.bCard = m_ChangeCard[iWiner2];
				for(int i=0;i<4;i++)
					SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
				SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
			}
		}
		else
		{
			//δ����ͬ����С
			//��һ��ҵõ���һʤ�ߵ���
			if(m_iNtPeople != iLose1 && m_iNtPeople != iLose2)
				return FALSE;

			m_iUserCard[m_iNtPeople][m_iUserCardCount[m_iNtPeople]++] = m_ChangeCard[iWiner1];
			ChangeCardStruct change;
			::memset(&change,0,sizeof(change));
			change.bDeskFrom = iWiner1;
			change.bDeskTo =m_iNtPeople;
			change.bCard = m_ChangeCard[iWiner1];
			for(int i=0;i<4;i++)
				SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
			SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
			//�ڶ���ҵõ��ڶ�ʤ�ߵ���
			BYTE lose2 = (iLose1 == m_iNtPeople)?iLose2:iLose1;
			
			m_iUserCard[lose2][m_iUserCardCount[lose2]++] = m_ChangeCard[iWiner2];
			memset(&change,0,sizeof(change));
			change.bDeskFrom = iWiner2;
			change.bDeskTo =lose2;
			change.bCard = m_ChangeCard[iWiner2];
			for(int i=0;i<4;i++)
				SendGameData(i,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
			SendWatchData(m_bMaxPeople,&change,sizeof(change),MDM_GM_GAME_NOTIFY,ASS_CHANGE_CARD_RESULT,0);
		}

	///�����˹�������Ϣ
	for(int i=0;i<4;i++)
		SendGameData(i,MDM_GM_GAME_NOTIFY,ASS_MOVE_BACK_FINISH,0);
	SendWatchData(m_bMaxPeople,MDM_GM_GAME_NOTIFY,ASS_MOVE_BACK_FINISH,0);


	///�˹���ͣ������
	SetTimer(TIME_BACK_CONTRIBUTE_WAIT,3*1000);
	return TRUE;
}

//��Ϸ��ʼ
BOOL CServerGameDesk::BeginPlayUpGrade()
{
	//��������
	m_bGameStation = GS_PLAY_GAME;       ///��Ϸ����״̬

	m_iBaseOutCount=0;				 //���Ƶ���Ŀ

	if(m_iUpGradePeople ==-1)
		m_iUpGradePeople=(m_iNtPeople == -1? 0 : m_iNtPeople);

	m_iGrade2 = (m_iUpGradePeople + 2)%4;

	//�Ϲ�����ߵ�һ������(�򿹹������,�Ͼֵ�һӮ�ҳ���)
	m_iFirstOutPeople = (m_iNtPeople == -1? m_iUpGradePeople : m_iNtPeople);
	m_iOutCardPeople = m_iFirstOutPeople;
	m_iTempPoint=0;
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));

	//�����˿�
	for (int i=0; i<PLAY_COUNT; i++) 
	{
		m_Logic.SortCard(m_iUserCard[i],NULL,m_iUserCardCount[i]);
	}
	//������Ϸ��ʼ��Ϣ
	BeginPlayStruct Begin;
	Begin.iOutDeskStation=m_iOutCardPeople;

	//�����˿� ͬʱ����ҵ��Ƹ��Ƶ���ʼ��Ϣ��
	for (int i=0; i<PLAY_COUNT; i++) 
	{
		m_Logic.SortCard(m_iUserCard[i],NULL,m_iUserCardCount[i]);
		Begin.iUserCardCount[i] = m_iUserCardCount[i];
		CopyMemory(Begin.iUserCard[i],m_iUserCard[i],sizeof(BYTE)*m_iUserCardCount[i]);

	}

	for (int i=0;i<m_bMaxPeople;i++) 
	{
		SendGameData(i,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
	}
	SendWatchData(m_bMaxPeople,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);

	if(true == m_bIsCut[m_iOutCardPeople])
	{
		SetTimer(TIMER_OUT_CARD + m_iOutCardPeople ,1000);
	}
	else
	{
		SetTimer(TIMER_OUT_CARD + m_iOutCardPeople,m_iThinkTime*1000 + 3000);
	}
	

	return TRUE;
}

//δ���Ʋ���������
BOOL  CServerGameDesk::FindKingCount()
{
	for(int i=0;i<4;i++)
	{
		//���������������ĸ���
		int king=m_Logic.GetCountBySpecifyCard(m_iUserCard[i],m_iUserCardCount[i],0x4e)
			+m_Logic.GetCountBySpecifyCard(m_iUserCard[i],m_iUserCardCount[i],0x4f);

		if(3==king)			//��������һ��
		{
			m_iHeads[i][9]=3;
		}

		if(4==king)
		{
			m_iHeads[i][9]=4;
		}
	}
	return TRUE;
}

//�û�����
BOOL CServerGameDesk::UserOutCard(BYTE bDeskStation, BYTE iOutCard[], int iCardCount)
{	


	if(bDeskStation!=m_iOutCardPeople)
	{
		return true;
	}

	//�����������
	if(iCardCount>0)					//ԭ�����߼��е�һ��������,��Ϊ��ǰ��������
	{	
		if (!m_Logic.CanOutCard(iOutCard,iCardCount,m_iDeskCard[m_iFirstOutPeople],m_iBaseOutCount,m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation],bDeskStation==m_iFirstOutPeople))
		{
			OutCardErrorStruct ErrorCard;


			ErrorCard.iCardCount = m_iUserCardCount[bDeskStation];

			memcpy(ErrorCard.bCard,m_iUserCard[bDeskStation],sizeof(ErrorCard.bCard));

			SendGameData(bDeskStation,&ErrorCard,sizeof(ErrorCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_ERROR,0);

			return TRUE;
		}
		//ɾ���˿�
		if(m_Logic.RemoveCard(iOutCard,iCardCount,m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation])==0)
		{		

/***********************************************��ӡ��־********************************************/
			CString Number,str;
			Number.Format("%s������������:m_iUserCardCount[%d] = %d",m_pUserInfo[bDeskStation]->m_UserData.nickName,bDeskStation,m_iUserCardCount[bDeskStation]);

			for(int index = 0;index < m_iUserCardCount[bDeskStation];index++)
			{
				str.Format("%d,",m_Logic.GetCardNum(m_iUserCard[bDeskStation][index]));
				Number += str;
			}
			DebugPrintf(Number);

			Number = "";str = "";
			Number.Format("%sҪɾ��������:iCardCount = %d",m_pUserInfo[bDeskStation]->m_UserData.nickName,iCardCount);

			for(int index = 0;index < iCardCount;index++)
			{
				str.Format("%d,",m_Logic.GetCardNum(iOutCard[index]));
				Number += str;
			}
			DebugPrintf(Number);
/***********************************************��ӡ��־********************************************/

            
			OutCardErrorStruct ErrorCard;


			ErrorCard.iCardCount = m_iUserCardCount[bDeskStation];

			memcpy(ErrorCard.bCard,m_iUserCard[bDeskStation],sizeof(ErrorCard.bCard));

			SendGameData(bDeskStation,&ErrorCard,sizeof(ErrorCard),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_ERROR,0);

			return TRUE;
		}

		m_iFirstOutPeople = bDeskStation;
		m_iBaseOutCount = iCardCount;
		m_iUserCardCount[bDeskStation] -= iCardCount;
		//��¼������Ϣ
		m_iDeskCardCount[bDeskStation]=iCardCount;
		::CopyMemory(m_iDeskCard[bDeskStation],iOutCard,sizeof(BYTE)*iCardCount);
	}
	else
	{	//ĳһλ��ʲ�N��Ҳ����
		m_iDeskCardCount[bDeskStation] = iCardCount;
	}
	KillAllTimer();	
	//������ҳ��ƽ��
	OutCardMsg UserOutResult;
	UserOutResult.bDeskStation=bDeskStation;
	UserOutResult.iCardCount=iCardCount;
	::CopyMemory(UserOutResult.iCardList,iOutCard,sizeof(BYTE)*iCardCount);

	for (int i=0;i<m_bMaxPeople;i++)
	{
		SendGameData(i,&UserOutResult,sizeof(OutCardMsg),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);
	}

	SendWatchData(m_bMaxPeople,&UserOutResult,sizeof(OutCardMsg),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_RESULT,0);

	////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////ֻ�г�������Ϊ0�Ž����ж�//////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////
	if(iCardCount>0)
	{
		//�ж��������Ƿ������
		if (m_iUserCardCount[bDeskStation] <= 0)
		{
			//�����Ƹ������O��
			OutCardFinish(bDeskStation);
			
			//����ܷ������Ϸ
			if(CheckFinish())
			{
				SetTimer(TIME_GAME_FINISH,2000);
				return true;
			}
		}
		//�����������,���¿�ʼ��һ��
		if(m_Logic.IsKingBomb(iOutCard, iCardCount))
		{	
			m_iFirstOutPeople=m_iOutCardPeople=bDeskStation;
			IsNewTurn();
			return true;
		}
	}
	//������һ������
	m_iOutCardPeople=(bDeskStation+3)%4;

	for(int i= m_iOutCardPeople;;i = (i+3)%4)
	{
		m_iOutCardPeople=i;				//��ǰ������
		if(IsNewTurn())
		{
			return true;
		}
		//��ǰ����������δ��
		if(m_iUserCardCount[i]>0)
		{
			break;
		}
	}
	OutCardMsg UserOut;

	UserOut.iNextDeskStation=m_iOutCardPeople;
	UserOut.iCardCount=0;

	for (int i=0;i<m_bMaxPeople;i++) 
	{
		SendGameData(i,&UserOut,sizeof(OutCardMsg),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
	}

	SendWatchData(m_bMaxPeople,&UserOut,sizeof(OutCardMsg),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
	

	if(true == m_bIsCut[m_iOutCardPeople])
	{
		SetTimer(TIMER_OUT_CARD + m_iOutCardPeople ,1000);
	}
	else
	{
		SetTimer(TIMER_OUT_CARD + m_iOutCardPeople,m_iThinkTime*1000 + 3000);
	}
	return TRUE;
}

BOOL CServerGameDesk::IsNewTurn()
{
	if (m_iOutCardPeople==m_iFirstOutPeople)			//���ȳ�����
	{
		//CollectPoint(m_iOutCardPeople);            xiang

		m_iOutCardPeople = -1;
		if(m_iUserCardCount[m_iFirstOutPeople] == 0)
		{
			//�����ǰ���������Ѿ�û���Ƴ�,�������ĶԼҽӷ�
			m_iOutCardPeople=m_iFirstOutPeople = GetNextOutCardPeople(m_iFirstOutPeople);
		}
		SetTimer(TIME_WAIT_NEWTURN,1000);
		//NewPlayTurn(m_iFirstOutPeople);	//������һ����Ϸ
		return true;
	}
	return false;
}
//ĳ��ҳ������һ����,���˴�,�ɶԼ����ӷ�
BYTE CServerGameDesk::GetNextOutCardPeople(BYTE bDeskStation)
{

	if(m_iUserCardCount[(bDeskStation + 2)%4] > 0)
		return (bDeskStation + 2)%4;
	else 
		return 0;
}

//���������
BOOL CServerGameDesk::OutCardFinish(BYTE bDeskStation)
{
	int count=0;
	BYTE bFriendStation=0;
	for(int i=0;i<4;i++)
	{
		if(m_iSequence[i])
			count++;
	}
	//��һ�������ƞ���һ���f��   //��ͳ���Ժ���ҳ����Ƶ�˳��
	m_iSequence[bDeskStation]=++count;	

	if(count == 1)
	{
		m_iNtPeople = bDeskStation;
	}

	//����ǰ�������ߵĶԼ���˭(ֻ����Ƕ������)
	if(m_iJiaofen[m_iUpGradePeople]==-1)
	{
		//�����ׯ,��������
		if( bDeskStation == m_iUpGradePeople)
		{
			bFriendStation = m_iGrade2;
		}
		else if( bDeskStation == m_iGrade2 )
			{
				bFriendStation = m_iUpGradePeople;
			}
			else
			{
				for(int i=0;i<4;i++)
				{
					if( i != m_iGrade2 && i != m_iUpGradePeople && i!=bDeskStation)
					{
						bFriendStation = i;
						break;
					}
				}

			}
	}
	else
	{
		bFriendStation = bDeskStation;
	}

	OutCardFinishStruct finish;
	finish.bFriendDesk =bFriendStation;
	finish.bDeskStation=bDeskStation;
	finish.bSequence=m_iSequence[bDeskStation];

	for (int i=0;i<m_bMaxPeople;i++)
	{
		SendGameData(i,&finish,sizeof(finish),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_FINISH,0);
	}

	SendWatchData(m_bMaxPeople,&finish,sizeof(finish),MDM_GM_GAME_NOTIFY,ASS_OUT_CARD_FINISH,0);

	return true;
}


//����Ƿ���Խ���
BOOL CServerGameDesk::CheckFinish()
{
	int ntcount=0,notntcount=0;
	for(int i=0;i<4;i++)//ͳ�Ƴ����Ƶ����
	{
		if(m_iUserCardCount[i]==0)
		{
			if(i==m_iUpGradePeople||i==m_iGrade2)
				ntcount++;
			else
				notntcount++;
		}
	}
	switch(m_iJiaofen[m_iUpGradePeople])
	{
	case -1:
		{
			return (notntcount==2||ntcount==2);
			break;
		}
	case 1://���˶���
		{
			return (ntcount+notntcount);
			break;
		}
	}
	return false;
}

//ͳ��ʤ��
int CServerGameDesk::StatWin()
{
	int step = 0;
	//δ��������ȫ��������
	for(int i=0; i < PLAY_COUNT;i++)
	{
		if(m_iSequence[i] == 0)
		{
			m_iSequence[i] = 4;
		}
	}

	//������һ�ֳ��Ƽ�¼

	for(int i=0; i<PLAY_COUNT; i++)
	{
		m_iSequenceHistory[m_iHistory][i] = m_iSequence[i];
	}

	m_iHistory ++;

	////ֻ����9��(�����ǰ3�֣�����ǰ��)
	//if (m_iHistory > 9)
	//{
	//	for(int i = 1; i<m_iHistory -1; i++)
	//		::CopyMemory(m_iSequenceHistory[i - 3],m_iSequenceHistory[ i ],sizeof(BYTE) * 4);
	//	m_iHistory -= 3;
	//  CString str;
	//	str.Format("xyh:m_iBeenPlayGame=%d,m_iHistory=%d",m_iHistory,m_iBeenPlayGame);
	//	OutputDebugString(str);
	//}

	CString str;
	str.Format("xyh:m_iBeenPlayGame=%d,m_iHistory=%d",m_iHistory,m_iBeenPlayGame);
	OutputDebugString(str);
	
	//�������
	switch(m_iSequence[m_iUpGradePeople] + m_iSequence[m_iGrade2])//ͬ�˷ֱ�Ϊ���κͶ���
	{
	case 1:
	case 2:
	case 3: 
		{
			step = 3;
			break;
		}
	case 4:
		{
			step =  2;
			break;
		}
	case 5://��1.4��ʤ,��2.3�θ�
		{
			if(m_iSequence[m_iUpGradePeople] == 1 || m_iSequence[m_iGrade2] == 1)
				step = 1;
			else
				step = -1;
			break;
		}
	case 6:
		{
			step = -2;
			break;
		}
	case 7:
	case 8:
		{
			step = -3;
			break;
		}
	}

	for (int i=0; i<PLAY_COUNT; i++)
	{
		m_iWinSequence[i][0] = CountSequence(i,1);
		m_iWinSequence[i][1] = CountSequence(i,4);

	}

	return step; /*+ stepFirst*/;
}
//ͳ�����κ�δ�δ���
BYTE CServerGameDesk::CountSequence(BYTE FirstOrLast)
{
	BYTE bDeskStation = GetPostionBySequence(FirstOrLast);
	BYTE Count = 0;
	for(int i = m_iHistory - 1;i >= 0;i --)
	{
		if(m_iSequenceHistory[i][bDeskStation] == FirstOrLast)
			Count ++;
		else
			break;
	}
	return Count;
}
//ͳ�����κ�δ�δ���
BYTE CServerGameDesk::CountSequence(BYTE bDeskStation ,BYTE FirstOrLast)
{
//	BYTE bDesk = GetPostionBySequence(FirstOrLast);
	BYTE Count = 0;
	for(int i = m_iHistory - 1;i >= 0;i --)
	{
		if(m_iSequenceHistory[i][bDeskStation] == FirstOrLast)
			Count ++;
		else
			break;
	}
	return Count;
}

//��ȡ��ǰ���Σ����Σ����Σ�����(δ����������������
BYTE CServerGameDesk::GetPostionBySequence(BYTE iSequence)
{

	//for(int i = 0;i < 4;i++)
	//{
	//	if(m_iSequenceHistory[m_iHistory - 1][i] == iSequence)
	//	{
	//		return i;
	//	}
	//}
	//retrun 0;

	BYTE iWiner1 = 255;				    //��Ӯ���Ƴ����
	BYTE iWiner2 = 255;					//�ڶ�Ӯ��
	BYTE iLose1 = 255; 					//���
	BYTE iLose2 = 255;					//�ڶ����

	///������Ρ�����
	for(int i = 0;i < 4;i++)
	{
		if(m_iSequenceHistory[m_iHistory - 1][i] == 1)
		{
			iWiner1 = i;
		}
		if(m_iSequenceHistory[m_iHistory - 1][i] == 2)
		{
			iWiner2 = i;
		}
	}
	
	///û���������ε����
	if(iWiner2 != (iWiner1+2)%PLAY_COUNT)
	{
		for(int i = 0;i < 4;i++)
		{
			if(m_iSequenceHistory[m_iHistory - 1][i] == 3)
			{
				iLose1 = i;
			}
			if(m_iSequenceHistory[m_iHistory - 1][i] == 4)
			{
				iLose2 = i;
			}
		}
	}
	///���������ε����
	else
	{
		iLose1 = (iWiner1+1)%PLAY_COUNT;
		iLose2 = (iWiner2+1)%PLAY_COUNT;
	}

	///����Ҫ��õ�״̬
	switch(iSequence)
	{
	case 1:
		{
			return iWiner1;
		}
	case 2:
		{
			return iWiner2;
		}
	case 3:
		{
			return iLose1;
		}
	case 4:
		{
			return iLose2;
		}
	default:
		return 0;
	}
	return 0;

}
	//ͳ�ƽ���
int CServerGameDesk::StatAwardPoint(BYTE bDeskStation)
{
	int award=0,temp=0;
	for(int i=9;i>=0;i--)
	{
		if(m_iHeads[bDeskStation][i]!=0)
		{
			switch(i)
			{	
			case 9:
				{
					switch(m_iHeads[bDeskStation][i])
					{
					case 8:award+=3;break;//4��һ�����3��
					case 4:award+=2;break;//�����2��
					case 6://������һ�����1��
					case 3:award+=1;break;//������һ�����1��
					default:break;
					}
					break;
				}
				//award+=(m_iHeads[bDeskStation][i]==2)?3:1;	break;		,3����1��
			case 8:
				award+=m_iHeads[bDeskStation][i]*10;break;						//ÿһ��8ը��5��
			case 7:
				award+=m_iHeads[bDeskStation][i]*3;break;						//ÿһ��7ը��3��
			case 6:
				temp=m_iHeads[bDeskStation][i]*1+m_iHeads[bDeskStation][7];		//7��ͷҲ��������6��ͷ
				award+=temp;break;						//ÿһ��6ը��1�ֻ�Ҫ����7ը�ĸ�����
			case 5:
				temp=m_iHeads[bDeskStation][i]*1+m_iHeads[bDeskStation][7]+m_iHeads[bDeskStation][6];		//7��ͷҲ��������6��ͷ
				award+=(temp>=2)?(temp-1)*1:0;break;						//ÿ2��5ը��1��
			case 4:
				temp=m_iHeads[bDeskStation][i]*1+m_iHeads[bDeskStation][7]+m_iHeads[bDeskStation][6]+m_iHeads[bDeskStation][5];		//7��ͷҲ��������6��ͷ
				award+=(temp>=4)?(temp-3)*1:0;break;						//ÿ4��4ը��1��
			case 0:
				award+=m_iHeads[bDeskStation][i]*1;break;						//ÿһ��ͬ����1��
			}
		}
	}
	return award;
}

//��һ�ֿ�ʼ
BOOL CServerGameDesk::NewPlayTurn(int iWiner)
{
	m_iTempPoint=0;									//��ʱ������
	m_iBaseOutCount=0;
	m_iOutCardPeople=iWiner;
	m_iFirstOutPeople=iWiner;
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	NewTurnStruct newturn;
	newturn.bDeskStation = iWiner;
	for (int i=0;i<m_bMaxPeople;i++) 
		SendGameData(i,&newturn,sizeof(newturn),MDM_GM_GAME_NOTIFY,ASS_NEW_TURN,0);
	SendWatchData(m_bMaxPeople,&newturn,sizeof(newturn),MDM_GM_GAME_NOTIFY,ASS_NEW_TURN,0);

	if(true == m_bIsCut[m_iOutCardPeople])
	{
		SetTimer(TIMER_OUT_CARD + m_iOutCardPeople ,1000);
	}
	else
	{
		SetTimer(TIMER_OUT_CARD + m_iOutCardPeople,m_iThinkTime*1000 + 3000);
	}

	return TRUE;
}

//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	//�õ���С���Ľ��
	long m_dwMaxPoint=0;
	bool bSetMaxMoney=false;
	

	//��д����
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			m_bGameStation = GS_WAIT_NEXT;
			for (int i=0;i<m_bMaxPeople;i++)
			{
				if (m_pUserInfo[i]!=NULL) 
					m_pUserInfo[i]->m_UserData.bUserState=USER_SITTING;
			}

			//��Ϸ����(�����Ϸ��������)
			GameEndStruct GameEnd;
			::memset(&GameEnd,0,sizeof(GameEnd));
			GameEnd.iUpGradeStation=m_iUpGradePeople;
			GameEnd.iGrade2 = m_iGrade2;

			//����(������Ҽ��𶨳�����Ϸ����)
			int iHumanBasePoint = GetHumanBasePoint();
			GameEnd.iHumanBasePoint = iHumanBasePoint;
			//���汶��
			int iDeskBasePoint=GetDeskBasePoint();
			GameEnd.iDeskBasePoint=iDeskBasePoint;
			//���䱶��
			int iRoomBasePoint = GetRoomBasePoint();
			GameEnd.iRoomBasePoint = iRoomBasePoint;

			//����÷�
			int iTurePoint = StatWin();//(int)GetUpGrade(m_iTempPoint,m_fen);

			::CopyMemory(GameEnd.iSequence,m_iSequence,sizeof(m_iSequence));
			

			//::CopyMemory(GameEnd.iHeads,m_iSequenceHistory,sizeof(GameEnd.iHeads));
			
			for(int i = 0 ;i < 4 ; i ++)
			{
				if (m_pUserInfo[i])
				{
					
					GameEnd.dwUserID[i] = m_pUserInfo[i]->m_UserData.dwUserID;
					
					for (int j=0; j<2; j++)
					{
						GameEnd.iWinSequence[i][j]=m_iWinSequence[i][j];
					}
					if(i == m_iUpGradePeople || i == m_iGrade2)
					{
						GameEnd.iTurePoint[i] = iTurePoint;
					}
					else
					{
						GameEnd.iTurePoint[i] = - iTurePoint;
					}

					GameEnd.iTurePoint[i] *= m_pDataManage->m_InitData.uDiZhu;
					GameEnd.iContinueWin[i] =  CountSequence(i,1);
					GameEnd.iContinueLose[i] = CountSequence(i,4);
				}
			}
			//д�����ݿ�				
			int temp_point[PLAY_COUNT];
			bool temp_cut[PLAY_COUNT];
			memset(temp_point,0,sizeof(temp_point));
			memset(temp_cut,0,sizeof(temp_cut));

			for (int i=0;i<PLAY_COUNT;i++)
			{
				temp_point[i] = GameEnd.iTurePoint[i];
			}

			if(!SetNextNTNum(iTurePoint))
			{
				//�����Ҵ�A�������λ��������ϣ���ׯ�ҿ�8��
				if(m_PlayANumber[m_iUpGradePeople%2] >= 3)
				{
					m_PlayANumber[m_iUpGradePeople%2] = 0;
					GameEnd.iTurePoint[m_iUpGradePeople] -= 8;
					GameEnd.iTurePoint[m_iGrade2] -= 8;
					temp_point[m_iUpGradePeople] = GameEnd.iTurePoint[m_iUpGradePeople];
					temp_point[m_iGrade2] = GameEnd.iTurePoint[m_iGrade2];
				}

				ChangeUserPoint(temp_point, temp_cut);
				__super::RecoderGameInfo(GameEnd.iChangeMoney);
				//��������
				for (int i=0;i<m_bMaxPeople;i++)
				{
					SendGameData(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
				}

				SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
				ReSetGameState(bCloseFlag);	
				//���Ŵ���ĩ�ν���
				//LastSequence();
			}
			else//���뿪��������
			{
				CString str;
				for(int i = 0 ;i < 4 ; i ++)
				{
					if(i == m_iUpGradePeople || i == m_iGrade2)
					{
						GameEnd.iTurePoint[i] += 15;//���⽱15��
					}
					else
					{
						GameEnd.iTurePoint[i] -= 15;    //ԭ��Ϸ�����ǲ��۷�
					}

					str.Format("xyh:%d�ķ���:%d",i,GameEnd.iTurePoint[i]);
					OutputDebugString(str);

					temp_point[i] = GameEnd.iTurePoint[i];
					ChangeUserPoint(temp_point, temp_cut);
					__super::RecoderGameInfo(GameEnd.iChangeMoney);
				}
				m_bGameStation = GS_WAIT_ARGEE;
				for (int i=0;i<m_bMaxPeople;i++)
				{
					SendGameData(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_NO_CONTINUE_END,0);
				}

				SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_NO_CONTINUE_END,0);
				ReSetGameState(GF_NO_CONTINUE);		
			}
			//bCloseFlag = GFF_FORCE_FINISH;
		    __super::GameFinish(bDeskStation,GFF_FORCE_FINISH);
			return true;
		}
	case GF_TUOXIANG:
	case GF_LOSE:
		{
			return true;
		}
	case GF_SALE:			//��Ϸ��ȫ����
		{
			//��������
			m_bGameStation = GS_WAIT_ARGEE;

			for (int i=0;i<m_bMaxPeople;i++)
			{
				if (m_pUserInfo[i]!=NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}
			GameCutStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));

			for (int i=0;i<m_bMaxPeople;i++) 
			{
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);
			}
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);

			for (int i=0;i<m_bMaxPeople;i++) 
			{
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
			}
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);

			bCloseFlag = GFF_FORCE_FINISH;

			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);
			return true;
		}

	case GFF_FORCE_FINISH:		//�û������뿪
		{
			m_bGameStation=GS_WAIT_ARGEE;

			for (int i=0;i<m_bMaxPeople;i++)
			{
				if (m_pUserInfo[i]!=NULL) 
					m_pUserInfo[i]->m_UserData.bUserState=USER_SITTING;
			}
			GameCutStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));
			CutEnd.bDeskStation=bDeskStation;
			//����(������Ҽ��𶨳�����Ϸ����)
			int iHumanBasePoint = GetHumanBasePoint();
			CutEnd.iHumanBasePoint = iHumanBasePoint;
			//���汶��
			int iDeskBasePoint=GetDeskBasePoint();
			CutEnd.iDeskBasePoint=iDeskBasePoint;
			//���䱶��
			int iRoomBasePoint = GetRoomBasePoint();
			CutEnd.iRoomBasePoint = iRoomBasePoint;

			CutEnd.iTurePoint[bDeskStation] = - GetRunPublish();//*iBase*iDeskBase;;
			if (m_iPlayNtNum[(bDeskStation+1)%2] >= 5)
			{
				CutEnd.iTurePoint[(bDeskStation+1)%4] = GetRunPublish()/2;
				CutEnd.iTurePoint[(bDeskStation+3)%4] = GetRunPublish()/2;
			}

			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			for (int i=0;i<PLAY_COUNT;i++)
			{
				temp_cut[i]=(bDeskStation== i && CutEnd.iTurePoint[i]<0);
			}


			ChangeUserPoint(CutEnd.iTurePoint, temp_cut);

			__super::RecoderGameInfo(CutEnd.iChangeMoney);

			for (int i=0;i<m_bMaxPeople;i++)
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);

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

//������һׯ��λ��
BOOL CServerGameDesk::SetNextNTStation(int iTurePoint)
{	
	if(iTurePoint > 0)//ׯʤ���Լ�Ϊ��һׯ��
	{
		m_iDealtPeople = m_iUpGradePeople ;//= m_iGrade2;
	}
	else //ׯ�ܣ�ׯ�¼�
	{
		m_iDealtPeople = m_iUpGradePeople = (m_iUpGradePeople +3)%4;
	}

	return TRUE;
}
//return FALSE Game continue 
//return  TRUE Can Play Next or Exit this game
//������һ��������,ׯ��λ��
BOOL CServerGameDesk::SetNextNTNum(int iTurePoint)
{
	//����ʤ��
	if (iTurePoint > 0)//ׯʤ
	{
		//Ӳ�����һ��
		if(m_iPlayNtNum[m_iUpGradePeople%2] + iTurePoint > m_iEndNt && m_iPlayNtNum[m_iUpGradePeople%2] < m_iEndNt)
		{
			m_iPlayNtNum[m_iUpGradePeople%2] = m_iEndNt;
		}
		else
		{
			m_iPlayNtNum[m_iUpGradePeople%2] += iTurePoint;
		}

		if(m_iPlayNtNum[m_iUpGradePeople%2] > m_iEndNt)		//ͨ�ش���
		{
			//�����A���뱣֤Ӯ��û��һ��������
			if(iTurePoint == 1)							   //���ִ�A
			{
				m_PlayANumber[m_iUpGradePeople%2]++;       //��¼��A�Ĵ���
				m_PlayANumber[(m_iUpGradePeople+1)%2] = 0; //����ׯ��ʱ�������¼����ֹ�ۼ�   
			
				//���δ�m_iEndNt�����Ĵ�m_iBeginNt
				if(m_PlayANumber[m_iUpGradePeople%2] >= 3)
				{
					m_iPlayNtNum[m_iUpGradePeople%2] = m_iBeginNt;
				}
				else
				{
					m_iPlayNtNum[m_iUpGradePeople%2] = m_iEndNt;						//���ܹ�����������Ϸ������A
				}
				m_iDealtPeople = m_iUpGradePeople = GetPostionBySequence(1);		   //Ӯ��
				return FALSE;
			}
			else//���Խ���������Ϸ
			{
				return TRUE;
			}
		}
		else
		{
			m_iDealtPeople = m_iUpGradePeople = GetPostionBySequence(1);//m_iDealtPeople = m_iUpGradePeople = m_iGrade2;	
		}
	}
	else  //ׯ��
	{
		m_iPlayNtNum[(m_iUpGradePeople+1) % 2] += (- iTurePoint);		
	
		if(m_iPlayNtNum[(m_iUpGradePeople+1) % 2 ] > m_iEndNt)			
		{
			m_iPlayNtNum[(m_iUpGradePeople+1) % 2] = m_iEndNt;
		}
		//��������Ϊׯ
		m_iDealtPeople = m_iUpGradePeople = GetPostionBySequence(1);
	}
	return FALSE;
}

/*
Function �����·������ҵ÷�ͳ��
param bDeskstation ���λ��
param bRunDeskStation �������λ��
return value �÷ּ���
*/
//�������������ҵ÷�
int CServerGameDesk::GetRunAwayOtherGetPoint(BYTE bDeskStation,BYTE bRunDeskStation )
{
	//����������
	if( bDeskStation == (bRunDeskStation+2)%4)
	{
		if(m_iUserGetPoint [(bDeskStation+1)%4]+ m_iUserGetPoint[(bDeskStation+3)%4] >100)
			return -5;
		else 
			return 0;
	}
	//�Ƕ���������
	if( m_iUserGetPoint [bDeskStation]+ m_iUserGetPoint[(bDeskStation+2)%4] >100)
		return 5;
	return 0;
}
//���ܿ۷�
int CServerGameDesk::GetRunPublish()
{
	return m_pDataManage->m_InitData.uRunPublish;
	//return 50;
}
//���䱶��
int CServerGameDesk::GetRoomBasePoint()
{
	return m_pDataManage->m_InitData.uBasePoint;;
}

//���汶��
int CServerGameDesk::GetDeskBasePoint()
{
	return 1;
	//return GetTableBasePoint();
}

//ͳ�Ʊ���
int CServerGameDesk::GetHumanBasePoint()
{
	//��ҳ������бȷֻ���
	//	if(m_pDataManage->m_InitData.uComType == TY_MONEY_GAME)

	//int  minval = m_pUserInfo[0]->m_UserData.dwPoint;
	//for(int i=1;i<PLAY_COUNT;i++)
	//{
	//	if(m_pUserInfo[i] != NULL)
	//	{
	//		minval=min(minval,m_pUserInfo[i]->m_UserData.dwPoint);
	//	}
	//}

	int  minval = 2147483647;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i] != NULL)
		{
			minval=min(minval,m_pUserInfo[i]->m_UserData.dwPoint);
		}
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

//ͬһ�ֻ�ɫ
BOOL CServerGameDesk::IsSameColor(BYTE iCardList[],int iCardCount)
{
	if(iCardCount<4)
		return false;
	int temp[5]={0};
	for(int i=0;i<iCardCount;i++)
	{
		temp[m_Logic.GetCardHuaKind(iCardList[i], true)/16]++;
	}

	if(temp[0]+temp[2]==4||temp[1]+temp[3]==4)
		return true;
	else
		return false;
}

////////////////////////////////////////////////////////////////
/////////////////////��Ϸ�������///////////////////////////////
////////////////////////////////////////////////////////////////

//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if(m_bGameStation >= GS_SEND_CARD && m_bGameStation < GS_WAIT_NEXT)
		return true;
	else
		return false;
	//__super::IsPlayGame(bDeskStation);
}
//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGameEx(BYTE bDeskStation)
{
	return 	m_bGameStation >= GS_SEND_CARD && m_bGameStation < GS_WAIT_NEXT;
}

//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	if(m_bGameStation>GS_WAIT_ARGEE)
	{
		GameFinish(0,GF_SALE);
	}
	m_bGameStation=GS_WAIT_ARGEE;
	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}

//�û������뿪
BOOL CServerGameDesk::UserHaveThing(BYTE bDeskStation, char * szMessage)
{
	if (m_bGameStation<GS_SEND_CARD)
		return TRUE;
	m_iHaveThingPeople=bDeskStation;
	m_iLeaveAgree|=1<<bDeskStation;
	if (m_iLeaveAgree!=15)
	{
		HaveThingStruct HaveThing;
		HaveThing.pos=bDeskStation;
		::CopyMemory(HaveThing.szMessage,szMessage,60*sizeof(char));
		for (int i=0;i<m_bMaxPeople;i++)
			if (i!=bDeskStation) 
				SendGameData(i,&HaveThing,sizeof(HaveThing),MDM_GM_GAME_NOTIFY,ASS_HAVE_THING,0);
	}
	else
	{
		GameFinish(bDeskStation,GF_SALE);
	}
	return TRUE;
}

//ͬ���û��뿪
BOOL CServerGameDesk::AgreeUserLeft(BYTE bDeskStation, BOOL bArgee)
{
	if (bArgee) m_iLeaveAgree|=1<<bDeskStation;
	else 
		m_iLeaveAgree&=~1<<bDeskStation;
	if (m_iLeaveAgree!=15)
	{
		//		m_bQueryQuit=true;
		LeaveResultStruct Leave;
		Leave.bDeskStation=bDeskStation;
		Leave.bArgeeLeave=bArgee;
		for (int i=0;i<m_bMaxPeople;i++) 
			if (i!=bDeskStation) 
				SendGameData(i,&Leave,sizeof(Leave),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	}
	else
	{
		GameFinish(m_iHaveThingPeople/*bDeskStation*/,GF_SALE);
		LeaveResultStruct Leave1;
		Leave1.bDeskStation=m_iHaveThingPeople;
		Leave1.bArgeeLeave=true;
		SendGameData(m_iHaveThingPeople,&Leave1,sizeof(Leave1),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	}
	return TRUE;
}

//��ǰ����
BOOL CServerGameDesk::UserStop(BYTE bDeskStation, BOOL bArgee)
{
	SendGameData((bDeskStation+2)%4,MDM_GM_GAME_NOTIFY,ASS_STOP_THING,0);
	return true;
}
//ͬ����ǰ����
BOOL CServerGameDesk::AgreeUserStop(BYTE bDeskStation, BOOL bArgee)
{
	if(bArgee)
	{
		//ͬ��
		StopResultStruct stop;
		stop.bArgee=true;
		SendGameData((bDeskStation+2)%4,&stop,sizeof(stop),MDM_GM_GAME_NOTIFY,ASS_AGREE_STOP,0);
		//������Ϸ
		if (m_bGameStation==GS_PLAY_GAME) 
			GameFinish(0,GF_NORMAL);
	}
	else
	{
		StopResultStruct stop;
		stop.bArgee=false;
		SendGameData((bDeskStation+2)%4,&stop,sizeof(stop),MDM_GM_GAME_NOTIFY,ASS_AGREE_STOP,0);
	}
	return true;
}
//����
BOOL CServerGameDesk::UserLose(BYTE bDeskStation, BOOL bArgee)
{
	for (int i=0;i<4;i++)
		if (i!=bDeskStation)
			SendGameData(i,MDM_GM_GAME_NOTIFY,ASS_LOSE,0);
	return true;
}
//ͬ������(��ʱֻ����һ��ͬ��ͽ�����Ϸ)
BOOL CServerGameDesk::AgreeUserLose(BYTE bDeskStation, BOOL bArgee)
{
	if(bArgee)
	{
		//	m_iAgreeLose++;
		//	if(m_iAgreeLose >= 3)
		GameFinish(bDeskStation,GF_TUOXIANG);
	}
	else
	{
		for(int i=0;i<4;i++)
			SendGameData(i,MDM_GM_GAME_NOTIFY,ASS_LOSE_RESULT,bDeskStation );
		SendWatchData(m_bMaxPeople,MDM_GM_GAME_NOTIFY,ASS_LOSE_RESULT,bDeskStation);
		//	m_iTouxiang=-1;
		//	m_iTxpoint=0;
	}
	return true;
}

//�й�
bool CServerGameDesk::UserAuto(BYTE bDeskStation,BOOL  bAutoCard)
{
	m_bAutoOutCard[bDeskStation]=bAutoCard;

	AutoCardStruct autocard;
	autocard.bAutoCard=bAutoCard;
	autocard.bDeskStation=bDeskStation;
	for(int i=0;i<4;i++)
	{
		SendGameData(i,&autocard,sizeof(autocard),MDM_GM_GAME_NOTIFY,ASS_AUTO,0);
	}
	SendWatchData(m_bMaxPeople,&autocard,sizeof(autocard),MDM_GM_GAME_NOTIFY,ASS_AUTO,0);
	return true;
}
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo)
{
	__super:: UserNetCut(bDeskStation, pLostUserInfo);
	return UserAI(bDeskStation, true);
}
bool CServerGameDesk::UserReCome(BYTE bDeskStation, CGameUserInfo *pLostUserInfo)
{
	m_bIsCut[bDeskStation] = false;
	__super::UserReCome(bDeskStation, pLostUserInfo);
	return true ;
}
//���AI����
BOOL CServerGameDesk::UserAI(BYTE bDeskStation,BYTE bCutType)
{
	m_bIsCut[bDeskStation] = true;

	if(CountCutPlayer() == CountPlayer())	//��1��
	{
		GameFinish(0,GF_SALE);

		return FALSE;
	}
	return TRUE;
}
//ͳ������Ϸ���
BYTE CServerGameDesk::CountPlayer()
{
	BYTE count=0;
	for(int i=0;i<m_bMaxPeople;i++)
	{
		if(m_pUserInfo[i])
			count++;
	}
	return count;
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
BOOL CServerGameDesk::UserBuyProp(BYTE bDeskStation,BYTE iLevel )
{
	SendGameData(bDeskStation,MDM_GM_GAME_NOTIFY,ASS_BUY_PROP,iLevel);
	return true;
}


//����ĩ�ν�������
//void CServerGameDesk::LastSequence()
//{
//	BYTE SetpLast = 0;      //�����������ε�����
//	for(int i=0; i < PLAY_COUNT;i++)
//	{
//		if(m_iSequence[i] == 4)
//		{
//			SetpLast = 0;
//			switch(CountSequence(i,4))
//			{
//			case 0:
//			case 1:
//			case 2:;
//			case 3:SetpLast	+=1;break;
//			case 4:SetpLast += 2;break;
//			case 5:SetpLast += 3;break;
//			case 6:SetpLast += 4;break;
//			case 7:SetpLast += 5;break;
//			case 8:SetpLast += 6;break;
//			case 9:
//			default:SetpLast += 7;break;
//			}
//			if (!(m_iPlayNtNum[i%2] ==3 && (i%2) == m_iUpGradePeople))
//				m_iPlayNtNum[i%2] = m_iPlayNtNum[i%2]-SetpLast;
//			if (m_iPlayNtNum[i%2] < m_iBeginNt)
//				m_iPlayNtNum[i%2] = m_iBeginNt;
//		}
//	}
//}