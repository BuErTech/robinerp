#include "StdAfx.h"
#include "ServerManage.h"
#include "../../../../sdk/include/Common/writelog.h"
#include <math.h>

//��Ϸ��չ����
BYTE CServerGameDesk:: m_iAllCardCount = 0; 

BYTE CServerGameDesk:: m_iSendCardTime = 0;
BYTE CServerGameDesk:: m_iThinkTime = 0;
BYTE CServerGameDesk:: m_iBeginTime = 0;

/*
* ��ӡ��־�ļ�
*/
void DebugPrintf(int iRoomID, const char *p, ...)
{
	return;
	CTime time = CTime::GetCurrentTime();
	char szFilename[256];
	sprintf(szFilename, "%d_%d-%d%d%d-log.txt", NAME_ID, iRoomID, time.GetYear(), time.GetMonth(), time.GetDay());
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
//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(ALL_ARGEE)
{
	m_iLeaveArgee=0;								//ͬ���뿪
	m_bAddNoteTime=0;								//��ע����(����Ĵ�)
	m_iFirstOutPeople=-1;							//��һ����ע��
	m_iOutCardPeople=-1;
	m_iLastOutPeople=-1;
	m_iNtPeople=-1;
	m_iUpGradePeople=0;						
	m_iBeenPlayGame=0;								//�Ѿ�����ľ���
	m_bGameStation=GS_WAIT_ARGEE;//GS_WAIT_SETGAME;		//��Ϸ״̬
	memset(m_bSuperUser,0,sizeof(m_bSuperUser));
	memset(m_iTotalCard,0,sizeof(m_iTotalCard));
	memset(m_Ming,0,sizeof(m_Ming));
	memset(m_iAddressIndex,0,sizeof(m_iAddressIndex));

	memset(m_bOpenLoser,0,sizeof(m_bOpenLoser));
	memset(m_bIsGiveUp,0,sizeof(m_bIsGiveUp));
	//lc090106��ʼ������
	memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));
	memset(m_iUserCard,0,sizeof(m_iUserCard));
	memset(m_iUserStation,0,sizeof(m_iUserStation));
	memset(m_iUserNote,0,sizeof(m_iUserNote));
	memset(m_iPerJuTotalNote,0,sizeof(m_iPerJuTotalNote));
	memset(m_iNowNote,0,sizeof(m_iNowNote));
	m_iTotalNote=0;
	m_iFrontNote=0;
	memset(m_iThisGameNote,0,sizeof(m_iThisGameNote));
	memset(m_iTotalGameNote,0,sizeof(m_iTotalGameNote));
	m_iMaxNote=0;
	memset(m_ihelp,0,sizeof(m_ihelp));
	m_iNowBigPeople=0;
	m_iLimitNote=0;
	m_iBaseNote=0;
	m_iLimitePerNote = 0;
	m_iGuoDi = 0;

	m_bFirstNote = false;

	m_iTimerCount = 0;

	InitAIData();  //��ʼ���й�����

	//m_bCanOpen = false;

	//memset(m_iUserLeftNote,0,sizeof(m_iUserLeftNote));

	//���������ļ�
	LoadIni();
	InitializeCriticalSection(&m_csForIPCMessage);
	m_bCutLastPeoPle = 255;

	memset(m_byWin,0,sizeof(m_byWin));

	m_iMaxSuperPeople = 0;

	//����ͨ����ע
	//LoadExtIni();
#ifdef SUPER_VERSION
	for(int i = 0; i < 8; i++)
	{
		m_iSuperCardCount[i] = 0;
		::memset(m_bSuperCardList[i],0,sizeof(m_bSuperCardList[i]));
	}
	m_bSuperStation = -1;
#endif

	m_bIsShowIdOrPlay = FALSE;
}
//����ini�����ļ�
BOOL CServerGameDesk::LoadIni()
{
	if ( m_iAllCardCount > 0 
		&& m_iSendCardTime > 0 
		&& m_iThinkTime > 0 
		)
	{
		return true;
	}
	TCHAR skin[MAX_NAME_INFO];
	CString s = CINIFile::GetAppPath ();/////����·��
	CString temp = _T("");
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
	if (!CINIFile::IsFileExist(temp))
	{
		m_iAllCardCount = 52;
		m_iSendCardTime = 500;
		m_iBeginTime = 20;
		m_iThinkTime = 20;
		return true;
	}
	CINIFile f(temp);
	CString key = TEXT("game");
	//	m_iLimitNote = f.GetKeyVal(key,"limitnote",1000);//
	//	m_iBaseNote = f.GetKeyVal(key,"basenote",10);
	m_iAllCardCount = f.GetKeyVal(key,"cardcount",52);

	key=TEXT("other");
	m_iSendCardTime = f.GetKeyVal(key,"sendcardtime",500);
	m_iBeginTime = f.GetKeyVal(key,"begintime",20);
	m_iThinkTime = f.GetKeyVal(key,"thinktime",20);

	return true;
}

bool CServerGameDesk::InitDeskGameStation()
{
	//LoadExtIni();
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);

	return true;
}
//������ע
BOOL CServerGameDesk::LoadExtIni()
{
	TCHAR skin[MAX_NAME_INFO];
	CString s = CINIFile::GetAppPath ();/////����·��
	CString temp = _T("");
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
	if (!CINIFile::IsFileExist(temp))
	{
		m_iLimitNote = 1000;
		m_iBaseNote = 10;
		return true;
	}
	CINIFile f( temp);
	CString key = TEXT("game");
	m_iLimitNote = f.GetKeyVal(key,"limitnote",1000);
	m_iBaseNote = f.GetKeyVal(key,"basenote",10);
	//f.CloseFile();

	return TRUE;
}
//���ݷ���ID���������ļ�
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{

	CString s = CINIFile::GetAppPath ();/////����·��

	TCHAR skin[MAX_NAME_INFO];
	CString temp = _T("");
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));

	/*AfxMessageBox(temp);*/
	CINIFile f(temp);
	CString szSec("game");
	m_bIsShowIdOrPlay = f.GetKeyVal(szSec,"ShowIdOrPlay",TRUE);

	m_dwAIBalanceMoney = f.GetKeyVal(szSec,"AIBalanceMoney",-1);
	m_iAIWinProb[0] = f.GetKeyVal(szSec,"AIWinProb0",20);
	m_iAIWinProb[1] = f.GetKeyVal(szSec,"AIWinProb1",80);

	szSec.Format("%d_%d",NAME_ID,iRoomID);
	m_bIsShowIdOrPlay = f.GetKeyVal(szSec,"ShowIdOrPlay",m_bIsShowIdOrPlay);

	m_iBaseNote = 10;//m_pDataManage->m_InitData.uDiZhu;
	m_iLimitNote = 100000;//m_pDataManage->m_InitData.uMaxLimit2;/*m_InitData.uMaxLimite2*/ 
	m_iGuoDi = 10;//m_pDataManage->m_InitData.uGuoDi;
	m_iLimitePerNote = 1000;//m_pDataManage->m_InitData.uMaxLimit1;

	m_iBaseNote = f.GetKeyVal(szSec,"BaseNote",m_iBaseNote); //��С��ע��ע

	m_iLimitNote = f.GetKeyVal(szSec,"LimitNote",m_iLimitNote); //�����ע
	m_iGuoDi = f.GetKeyVal(szSec,"GuoDi",m_iGuoDi); //��ע
	m_iLimitePerNote = f.GetKeyVal(szSec,"LimitePerNote",m_iLimitePerNote); //һ�������ע


	m_iMaxSuperPeople = f.GetKeyVal(szSec,"MaxSuperPeople",20);
	//m_Logic.Msg("zhtlogServer:m_iLimitNote=%I64d",m_iLimitNote);

	if (m_iMaxSuperPeople<1||m_iMaxSuperPeople>=50)
	{
		return 20;
	}
	CString strCityText = "";
	CString strUserIDText = "";
	for (int i = 0;i<m_iMaxSuperPeople;i++)
	{
		strCityText.Format("CityTextInDex[%d]",i);
		strUserIDText.Format("UserIDTextInDex[%d]",i);
		m_strCityTextInDex[i] = f.GetKeyVal(szSec,strCityText,"");
		m_strUserIDTextInDex[i] = f.GetKeyVal(szSec,strUserIDText,"");
	}

	/*if (m_iBaseNote > m_iLimitePerNote || m_iBaseNote > m_iLimitNote)
	{
	AfxMessageBox("ϵͳ�������ô����������������ݿ��������������");
	}
	else if (m_iGuoDi > m_iLimitePerNote || m_iGuoDi > m_iLimitNote)
	{
	AfxMessageBox("ϵͳ�������ô����������������ݿ��������������");
	}*/
	return TRUE;
}
//��������
CServerGameDesk::~CServerGameDesk(void)
{
	DeleteCriticalSection(&m_csForIPCMessage);
}

bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	if (uTimerID != TIME_MY_TIMER)
	{
		return __super::OnTimer(uTimerID);
	}
	for (int i=0; i<TIMER_COUNT; ++i)
	{
		if (m_arTimer[i].nId == 0)
		{
			continue;
		}
		++m_arTimer[i].nTickCount;
		if (m_arTimer[i].nTickCount >= m_arTimer[i].nTriggerCount)
		{
			m_arTimer[i].nTickCount = 0;
			OnMyTimer(m_arTimer[i].nId);
		}
	}

	return true;
}
//��ʱ����Ϣ
bool CServerGameDesk::OnMyTimer(UINT uTimerID)
{
	if(TIME_BEGIN_ANIMAL==uTimerID)
	{
		if (m_bGameStation==GS_SEND_CARD) 
		{	
			KillMyTimer(TIME_BEGIN_ANIMAL);
			SetMyTimer(TIME_SEND_CARD,m_iSendCardTime);
		}else
			KillMyTimer(TIME_BEGIN_ANIMAL);

		return true;
	}
	else if (uTimerID==TIME_SEND_CARD)	//���ƶ�ʱ��
	{
		if (m_bGameStation==GS_SEND_CARD) 
			SendCard();
		else KillMyTimer(TIME_SEND_CARD);
		return TRUE;
	}

	else if(uTimerID==TIME_COLLECT_JETON)
	{
		if (m_bGameStation==GS_PLAY_GAME)
			CollectJeton();
		else KillMyTimer(TIME_COLLECT_JETON);
		return TRUE;
	}
	else if(uTimerID==TIME_SEND_A_CARD)
	{
		if (m_bGameStation==GS_PLAY_GAME)
			SendACard();
		else KillMyTimer(TIME_SEND_A_CARD);
		return TRUE;
	}
	else if (uTimerID==TIME_GAME_FINISH)	//������ʱ��
	{
		KillMyTimer(TIME_GAME_FINISH);
		if (m_bGameStation==GS_PLAY_GAME) 
			GameFinish(0,GF_NORMAL);
	}
	else if (TIMER_SEND_END == uTimerID)
	{
		KillMyTimer(TIMER_SEND_END);
		if (m_bGameStation==GS_SEND_CARD) 
			SendCardFinish();

	}
	else if (uTimerID==TIME_BIPAI)	//1013
	{
		KillMyTimer(TIME_BIPAI);
		BiPaiActiveResult();
		return TRUE;
	}
	else if (uTimerID == TIME_CUT_AI)
	{
		KillMyTimer(TIME_CUT_AI);
		ExcutNetCutAI();
		return true;
	}
	else if (uTimerID == TIMER_WAIT_NOTE)
	{
		if (--m_iTimerCount>0)
		{
			return true;
		}
		KillMyTimer(TIMER_WAIT_NOTE);
		// ���ʱ�䵽�˻������ƣ�����Ϊ����
		UserActiveResult(m_iOutCardPeople,TYPE_GIVE_UP,0);
		//UserCallScore(m_bCurrentCallScore,0);
	}else if(uTimerID ==  TIMER_FINISH_COMPARE)
	{
		if (--m_iTimerCount>0)
		{
			return true;
		}
		KillMyTimer(TIMER_FINISH_COMPARE);

		SetMyTimer(TIME_GAME_FINISH, 2000);
		FinishCompare();
	}

	return __super::OnTimer(uTimerID);
}
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	switch(pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:		//ǿ���˳�//��ȫ�˳�
		{

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

	switch (pNetHead->bAssistantID)
	{
	case 79://�����ͻ���
		{
#ifdef SUPER_VERSION
			if(bWatchUser)
				return FALSE;
			if(m_bGameStation == GS_PLAY_GAME)
				return true;
			SetCardStruct * pCard = (SetCardStruct*)pData;
			//Ĭ�Ϸ�ʽ(��ǰ�������)
			m_bSuperStation = bDeskStation ;
			if(pCard->paicount[0] > 0)//�������Լ�
			{
				m_iSuperCardCount[m_bSuperStation] = (pCard->paicount[0] > 3) ? 3 : pCard->paicount[0];
				::CopyMemory(m_bSuperCardList[m_bSuperStation],pCard->pai[0],m_iSuperCardCount[m_bSuperStation]);
				/*char buf[100];
				wsprintf(buf,"m_iSuperCardCount[m_bSuperStation]=%d,m_bSuperStation=%d",m_iSuperCardCount[m_bSuperStation],m_bSuperStation);
				OutputDebugStringA(buf);*/

			}else//���ö��0λ�ø�Ĭ��
			{
				for(int i = 1; i < 8; i ++)
				{
					m_iSuperCardCount[i-1] = (pCard->paicount[i] > 3) ? 3 : pCard->paicount[i];
					::CopyMemory(m_bSuperCardList[i-1],pCard->pai[i],sizeof(BYTE)*m_iSuperCardCount[i-1]);
				}
			}
#endif
			return true;
		}
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
				return FALSE;
	
			if (m_bGameStation>=GS_SEND_CARD&&m_bGameStation<GS_WAIT_NEXT)
			{
				return TRUE;
			}
			if(m_bGameStation!=GS_WAIT_NEXT)
				m_bGameStation=GS_WAIT_ARGEE;
			return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}
	case ASS_VREB_CHECK:		//�û�����
		{
			if(bWatchUser)
			{
				return false;
			}

			if(m_bGameStation==GS_WAIT_NEXT || m_bGameStation==GS_WAIT_ARGEE || m_bGameStation==GS_WAIT_SETGAME)
			{
				return true;
			}

			tagUserProcess *pVerb = (tagUserProcess *) pData;

			m_Ming[bDeskStation]=pVerb->bMing;
			if (pVerb->iVrebType == TYPE_COMPARE_CARD)
			{
				tagCompareCard *pVerbComp = (tagCompareCard *) pData;
				UserActiveResult(bDeskStation,pVerbComp->iVrebType,pVerbComp->iNote,pVerbComp->byComparedStation);
			}
			else
			{
				UserActiveResult(bDeskStation,pVerb->iVrebType,pVerb->iNote);
			}

			return true;

		}
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
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
			GameStation.iVersion=DEV_HEIGHT_VERSION;						//��Ϸ�汾��
			GameStation.iVersion2=DEV_LOW_VERSION;
			GameStation.iRoomBasePoint = GetRoomBasePoint();
			GameStation.iRunPublish = GetRunPublish();

			//��Ϸ������Ϣ����
			GameStation.iLimitNote=m_iLimitNote;						//���ע
			GameStation.iBaseNote=m_iBaseNote;							//��ע
			GameStation.iGuoDi = m_iGuoDi;                              //����
			GameStation.iLimtePerNote = m_iLimitePerNote;               //��ע����

			GameStation.iBeginTime=m_iBeginTime;
			GameStation.iThinkTime=m_iThinkTime;
			GameStation.iAllCardCount=m_iAllCardCount;
			GameStation.bShowIdOrPlay = m_bIsShowIdOrPlay;
			GameStation.bGameStation   = m_bGameStation;

			//			GameStation.iBeginStation=m_iBeginStation;
			//			GameStation.iEndStation=m_iEndStation;

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

			GameStation.bGameStation   = m_bGameStation;
			//��Ϸ������������
			//��Ϸ������Ϣ����
			GameStation.iLimitNote=m_iLimitNote;						//���ע
			GameStation.iBaseNote=m_iBaseNote;							//��ע
			GameStation.iGuoDi = m_iGuoDi;                              //����
			GameStation.iLimtePerNote = m_iLimitePerNote;               //��ע����


			GameStation.iBeginTime=m_iBeginTime;
			GameStation.iThinkTime=m_iThinkTime;
			GameStation.iAllCardCount=m_iAllCardCount;

			GameStation.iUpGradePeople=m_iUpGradePeople;		

			GameStation.iBeenPlayGame=m_iBeenPlayGame;	
			for (int i = 0;i<PLAY_COUNT;i++)
			{
				sprintf(GameStation.strCityText[i],"%s",m_strCityTextInDex[m_iAddressIndex[i]]);
				sprintf(GameStation.strUserIDText[i],"%s",m_strUserIDTextInDex[m_iAddressIndex[i]]);
				GameStation.bSuperUser[i] = m_bSuperUser[i];             //������Ƿ��ǳ������
				GameStation.bPlayer[i] = m_iUserStation[i] == -1 ? FALSE:TRUE;

			}

			//�û�������
			::CopyMemory(GameStation.iUserCardCount,m_iUserCardCount,sizeof(m_iUserCardCount));
			/// ֻ�ܰ�����Լ����ϵ��Ʒ���ȥ 20100314 zxd�޸�
			//20100601zht����ʱ��������
			//::CopyMemory(GameStation.iUserCard[bDeskStation],m_iUserCard[bDeskStation],sizeof(m_iUserCard[bDeskStation]));

			//ϵͳ����
			GameStation.iSysCardCount=m_iAllCardCount-m_iSendCardPos;
			//��ǰ���û���ע����
			::CopyMemory(GameStation.iTotalNote,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));

			//			GameStation.iUserCardCount=m_iUserCardCount[bDeskStation];		
			//			::CopyMemory(GameStation.iUserCard,m_iUserCard[bDeskStation],sizeof(BYTE)*m_iUserCardCount[bDeskStation]);

			GameStation.bShowIdOrPlay = m_bIsShowIdOrPlay;
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

			GameStation.bGameStation   = m_bGameStation;
			GameStation.iLimitNote=m_iLimitNote;						//���ע
			GameStation.iBaseNote=m_iBaseNote;							//��ע
			GameStation.iGuoDi = m_iGuoDi;                              //����
			GameStation.iLimtePerNote = m_iLimitePerNote;               //��ע����

			GameStation.iBeginTime=m_iBeginTime;
			GameStation.iThinkTime=m_iThinkTime;

			GameStation.iAllCardCount=m_iAllCardCount;

			GameStation.iUpGradePeople=m_iUpGradePeople;		

			GameStation.iBeenPlayGame=m_iBeenPlayGame;		
			//	GameStation.iGamePoint=m_iGamePoint;
			GameStation.iOutCardPeople=m_iOutCardPeople;

			GameStation.iFirstOutPeople=m_iFirstOutPeople;

			GameStation.bNtPeople = m_iNtPeople;   //ׯ��λ��
			GameStation.bIsFirstNote = m_bFirstNote;


			GameStation.iCurNote = m_iFrontNote;
			GameStation.iTimeRest = m_iTimerCount - 3;
			for (int i = 0;i<PLAY_COUNT;i++)
			{
				sprintf(GameStation.strCityText[i],"%s",m_strCityTextInDex[m_iAddressIndex[i]]);
				sprintf(GameStation.strUserIDText[i],"%s",m_strUserIDTextInDex[m_iAddressIndex[i]]);
				GameStation.bSuperUser[i] = m_bSuperUser[i];
				GameStation.bPlayer[i] = m_iUserStation[i] == -1 ? FALSE:TRUE;
			}	


			//			GameStation.iIngNote = m_iIngPlayNote;
			//			GameStation.iIngPlayStatus = m_iCurPlayStatus;
			//			GameStation.iTotalNote = m_iTotalNote;
			//			GameStation.ButtonPara = m_ButtonParaStruct;
			//Ѻע��ز���
			GameStation.tag.bAddTime=m_bAddNoteTime;		//Ѻע����
			if(m_iNowBigPeople==-1)
				GameStation.tag.iNowBigNote=0;
			else
				GameStation.tag.iNowBigNote=m_iUserNote[m_iNowBigPeople];//��ǰ���Ѻע��

			GameStation.tag.bCountNotePeople=CountNotePeople();
			GameStation.tag.iNowBigNoteStyle=m_iUserStation[m_iNowBigPeople];//Ѻע����
			GameStation.tag.iOutPeople=m_iOutCardPeople;					//��ǰѺע��
			GameStation.tag.iTotalNote=m_iTotalNote;						//��ǰ��ע
			GameStation.tag.iNowBigPeople = m_iNowBigPeople;
			GameStation.tag.iCutOutLastpeople = m_bCutLastPeoPle;
			if ( m_bCutLastPeoPle>=0 && m_bCutLastPeoPle<PLAY_COUNT )
			{
				GameStation.tag.iCutOutLastType = m_iUserStation[m_bCutLastPeoPle];
			}


			//�û�������
			::CopyMemory(GameStation.iUserCardCount,m_iUserCardCount,sizeof(m_iUserCardCount));

			/// ֻ�ܰ�����Լ����ϵ��Ʒ���ȥ 20100314 zxd�޸�
			//���ѿ����Ʋŷ����ݷ��򲻷�20100601zht
			if (m_Ming[bDeskStation])
			{
				::CopyMemory(GameStation.iUserCard[bDeskStation],m_iUserCard[bDeskStation],sizeof(m_iUserCard[bDeskStation]));
			}
			if (SUPER_PLAYER == m_pUserInfo[bDeskStation]->m_UserData.bGameMaster||true == m_bSuperUser[bDeskStation])
			{
				for (int i = 0;i<PLAY_COUNT;i++)
				{
					if (NULL!=m_pUserInfo[i])
					{
						::CopyMemory(GameStation.iUserCard[i],m_iUserCard[i],sizeof(m_iUserCard[i]));
					}
				}				
			}
			::CopyMemory(GameStation.iMing,m_Ming,sizeof(m_Ming));

			::CopyMemory(GameStation.bOpenLose,m_bOpenLoser,sizeof(m_bOpenLoser));
			::CopyMemory(GameStation.bIsGiveUp,m_bIsGiveUp,sizeof(m_bIsGiveUp));

			::CopyMemory(GameStation.iThisGameNote,m_iThisGameNote,sizeof(m_iThisGameNote));
			::CopyMemory(GameStation.iTotalGameNote,m_iTotalGameNote,sizeof(m_iTotalGameNote));

			::CopyMemory(GameStation.byWin,m_byWin,sizeof(m_byWin));
			//ϵͳ����
			//GameStation.iSysCardCount=m_iAllCardCount-m_iSendCardPos;
			//��ǰ���û���ע����
			::CopyMemory(GameStation.iTotalNote,m_iPerJuTotalNote,sizeof(m_iPerJuTotalNote));
			//			//�����û���ע��
			//			::CopyMemory(GameStation.iUserNote,m_iUserNote,sizeof(m_iUserNote));						
			//��������
			::CopyMemory(GameStation.bOpenLose,m_bOpenLoser,sizeof(m_bOpenLoser));

			int iSendSize=sizeof(GameStation);//-sizeof(GameStation.iUserCardList)+sizeof(BYTE)*iPos;

			GameStation.bShowIdOrPlay = m_bIsShowIdOrPlay;
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

			GameStation.bGameStation   = m_bGameStation;
			GameStation.iLimitNote=m_iLimitNote;						//���ע
			GameStation.iBaseNote=m_iBaseNote;							//��ע
			GameStation.iGuoDi = m_iGuoDi;                              //����
			GameStation.iLimtePerNote = m_iLimitePerNote;               //��ע����

			GameStation.iUpGradePeople=m_iUpGradePeople;	

			GameStation.iBeenPlayGame=m_iBeenPlayGame;		

			GameStation.bShowIdOrPlay = m_bIsShowIdOrPlay;
			for (int i = 0;i<PLAY_COUNT;i++)
			{
				sprintf(GameStation.strCityText[i],"%s",m_strCityTextInDex[m_iAddressIndex[i]]);
				sprintf(GameStation.strUserIDText[i],"%s",m_strUserIDTextInDex[m_iAddressIndex[i]]);
				GameStation.bSuperUser[i] = m_bSuperUser[i];
			}	
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
	KillMyTimer(TIME_SEND_CARD);
	//	KillMyTimer(TIME_WAIT_NT);
	KillMyTimer(TIME_GAME_FINISH);
	KillMyTimer(TIME_BEGIN_ANIMAL);
	KillMyTimer(TIME_SEND_A_CARD);
	KillMyTimer(TIME_COLLECT_JETON);

	KillMyTimer(TIMER_WAIT_NOTE);
	m_iTimerCount = 0;

	//if ((bLastStation==GFF_FORCE_FINISH)||(bLastStation==GF_SALE))
	{
		m_iBeenPlayGame=0;
		

	}
	for (int i = 0;i < m_bMaxPeople;i++) 
	{
		m_iUserStation[i] = -1;
		m_iThisGameNote[i]=0;
		m_ihelp[i]=0;
	}
	m_iUpGradePeople=-1;

	//	m_bQueryQuit=false;
	m_iLeaveArgee=0;
	//	m_iGamePoint=0;
	//m_iBaseOutCount=0;
	m_iFirstOutPeople=-1;
	m_iOutCardPeople=-1;
	m_iLastOutPeople=-1;
	m_iSendCardPos=0;
	m_iFrontNote=0;
	m_iMaxNote=0;

	m_bAddNoteTime = 0;
	memset(m_Ming,0,sizeof(m_Ming));
	memset(m_bIsGiveUp,0,sizeof(m_bIsGiveUp));

	memset(m_bOpenLoser,0,sizeof(m_bOpenLoser));
	//m_Logic.SetNTHuaKind(UG_ERROR_HUA);
	//memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));

	InitAIData();
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
		return false;

	KillTimer(TIME_MY_TIMER);
	KillMyTimer(TIME_GAME_FINISH);

	//���¼��������ļ������
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);

	memset(m_byWin,0,sizeof(m_byWin));
	/*for (int i = 0;i<PLAY_COUNT;i++)
	{
		srand((unsigned)time(NULL)); 
		m_iAddressIndex[i] = (rand()%50+PLAY_COUNT-i)%m_iMaxSuperPeople;	
		CString str;
		str.Format("wlogsuper::m_iAddressIndex[i]===%d,%s",m_iAddressIndex[i],m_strCityTextInDex[m_iAddressIndex[i]]);
		OutputDebugString(str);

	}	*/	
	//����״̬
	//	m_bQueryQuit=false;							//����������뿪
	m_iSendCardPos=0;							//����λ��
	m_iBeenPlayGame++;							//�Ѿ���ľ���
	m_iTotalNote = 0;	
	m_iMaxNote=0;
	//������ע��
	//	m_iCurPlayStatus = -1;
	m_iIngPlayNote = -1;
	m_iLastShuttle = -1;
	m_bAddNoteTime=0;							//��ע����
	//	m_iMastNote=0;								//�������ע
	m_bGameStation=GS_SEND_CARD;				//��Ϸ״̬
	//::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_bSuperUser,0,sizeof(m_bSuperUser));

	::memset(m_iUserCardCount,0,sizeof(m_iUserCardCount));		//�û���������

	::memset(m_iUserNote,0,sizeof(m_iUserNote));				//������ע��
	::memset(m_iNowNote,0,sizeof(m_iNowNote));					//���ҵ�ǰ��ע��

	::memset(m_iPerJuTotalNote,0,sizeof(m_iPerJuTotalNote));	//���ָ�����ע

	memset(m_bIsGiveUp,0,sizeof(m_bIsGiveUp));


	for (int i = 0;i < m_bMaxPeople;i++) 
	{
		m_iUserStation[i] = -1;
		m_iThisGameNote[i]=0;
		m_ihelp[i]=0;
		if (m_pUserInfo[i])
		{
			m_iUserStation[i] = TYPE_NORMAL;
		}
	}



	__int64 Temp= GetMinMoney();
	if (Temp < m_iLimitNote)
	{
		m_iLimitNote = Temp;
	}
	if (Temp < m_iBaseNote)
	{
		m_iBaseNote = Temp/10;
	}
	if (Temp < m_iGuoDi)
	{
		m_iGuoDi = Temp/10;
	}
	if (Temp < m_iLimitePerNote)
	{
		m_iLimitePerNote = Temp/5;
	}

	//�ж�����Ƿ��ǳ����ͻ���
	SuperUserIDJudge();
	//�ַ��˿�
	m_Logic.RandCard(m_iTotalCard,m_iAllCardCount,m_bDeskIndex);

	//add by ljl 
	m_iLimitNote = GetMinMoney();
#ifdef SUPER_VERSION
	for(int j = 0; j < PLAY_COUNT; j ++)
	{
		for(int i = 0; i < m_iSuperCardCount[j]; i ++)
		{
			int temp=m_Logic.GetSerialBySpecifyCard(m_iTotalCard,0,m_iAllCardCount,m_bSuperCardList[j][i]);
			// AfxMessageBox("1111");
			if(temp != -1)
			{	
				BYTE t = m_iTotalCard[i + j * 3];
				m_iTotalCard[i + j * 3] = m_iTotalCard[temp];
				m_iTotalCard[temp] = t;
			}
			/*char buf[100];
			wsprintf(buf,"temp=%d,m_iTotalCard[temp]=%d",temp,m_iTotalCard[temp]);
			OutputDebugStringA(buf);*/

		}


	}
#endif

	//���Ϳ�ʼ��Ϣ

	for(int i=0;i<m_bMaxPeople;i++)									//����ĿǰѺע���
	{
		if(m_pUserInfo[i] != NULL)
		{
			m_iPerJuTotalNote[i] = m_iGuoDi;//m_iLimitNote/100;;//PLAY_BASE_NOTE;
			m_iTotalNote += m_iGuoDi;
			m_iThisGameNote[i] = m_iGuoDi;
			m_iTotalGameNote[i] = m_iGuoDi;
		}
	}

	//m_iTotalNote=(m_iBaseNote)*CountPlayer();//;*PLAY_COUNT;		//��ע


	//�������ׯ��
	if (m_iNtPeople == 255)
	{
		m_iNtPeople = rand() % PLAY_COUNT;
	}

	if(	m_pUserInfo[m_iNtPeople] == NULL)
	{
		for(int i=0; i<PLAY_COUNT; ++i)
		{
			if(m_pUserInfo[i] == NULL)
				continue;
			m_iNtPeople = i;
			break;
		}
	}


	m_iUpGradePeople = GetNextNoteStation(m_iNtPeople);

	BeginUpgradeStruct BeginMessage;
	memset(&BeginMessage,0,sizeof(BeginMessage));
	BeginMessage.AllCardCount=m_iAllCardCount;
	BeginMessage.iLimitNote=m_iLimitNote;							//���ע
	BeginMessage.iBaseNote=m_iBaseNote;
	BeginMessage.iLimtePerNote = m_iLimitePerNote;
	BeginMessage.iGuoDi = m_iGuoDi;
	BeginMessage.bIsShowIdOrPlay = m_bIsShowIdOrPlay;
	BeginMessage.bNtStation      = m_iUpGradePeople;
	//m_Logic.Msg("zhtlog:Server m_iLimitNote=%I64d BeginMessage.iLimitNote=%I64d",m_iLimitNote,BeginMessage.iLimitNote);
	for (int i = 0;i<PLAY_COUNT;i++)
	{
		sprintf(BeginMessage.strCityText[i],"%s",m_strCityTextInDex[m_iAddressIndex[i]]);
		sprintf(BeginMessage.strUserIDText[i],"%s",m_strUserIDTextInDex[m_iAddressIndex[i]]);
		BeginMessage.bSuperUser[i] = m_bSuperUser[i];
	}

	for (int i=0;i<m_bMaxPeople;i++) 
	{
		if(m_pUserInfo[i])
			SendGameData(i,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_BEGIN_UPGRADE,0);
		else
			continue;
	}
	SendWatchData(m_bMaxPeople,&BeginMessage,sizeof(BeginMessage),MDM_GM_GAME_NOTIFY,ASS_BEGIN_UPGRADE,0);




	m_iSendPos=0;
	//���ÿ�ʼ����
	//	SetMyTimer(TIME_BEGIN_ANIMAL,3000);
	//������Ϣ

	//���ж�˭��Ӯ�����ж�˭����
	if (!JudgeWiner())
		JudgeLoser();

	//����˷��Ʋ���
	//SetUserCard();

	SetTimer(TIME_MY_TIMER, MY_ELAPSE);
	SetMyTimer(TIME_SEND_CARD,m_iSendCardTime);
	m_bCutLastPeoPle = 255;
	/*
	m_ButtonParaStruct.ShuttleEnable = TRUE;
	m_ButtonParaStruct.FollowEnable = FALSE;
	m_ButtonParaStruct.NotAddEnable = FALSE;
	m_ButtonParaStruct.b100NoteEnable = TRUE;
	m_ButtonParaStruct.b200NoteEnable = TRUE;
	m_ButtonParaStruct.b400NoteEnable = TRUE;
	m_ButtonParaStruct.GiveUpEnable = TRUE;
	*/
	return TRUE;
}
/// �������ļ��ж�ȡ�����ͻ����û�����֤�����û�Ȩ��
/// @param void
/// return ��֤�ɹ�������ͻ��˷��ͳ���Ȩ�ޣ����򲻷�����Ϣ���޷���ֵ
void CServerGameDesk::SuperUserIDJudge()
{
	CString s = CINIFile::GetAppPath ();	//����·��
	CINIFile f (s + GET_SKIN_FOLDER(szTempStr) +"_s.ini");
	CString key = TEXT("superUserConfig");

	CString strText;
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (NULL == m_pUserInfo[i])
		{
			continue ;
		}
		// �жϳ����û��ڷ��������Ƿ�����
		int iCount = f.GetKeyVal(key, "SuperUserCount", 5);
		for (int j = 0; j <iCount; j++)
		{
			strText.Format("SuperUserID[%d]", j);
			long  lUserID = f.GetKeyVal(key, strText, 0);

			if (m_pUserInfo[i]->m_UserData.dwUserID == lUserID)
			{
				// ���û�Ϊ�����û�
				m_bSuperUser[i] = true;				
			}
		}

	}
}
BOOL CServerGameDesk::SuperSetCard(BYTE iTotalCard[],int iCardCount)
{
	TCHAR skin[MAX_NAME_INFO];
	CString s = CINIFile::GetAppPath ();/////����·��
	CString temp = _T("");
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));
	CINIFile f( temp);
	CString key = TEXT("SuperCard");
	iTotalCard[0] = f.GetKeyVal(key,"card0",0x09);
	iTotalCard[1] = f.GetKeyVal(key,"card1",0x08);
	iTotalCard[2] = f.GetKeyVal(key,"card2",0x19);
	iTotalCard[3] = f.GetKeyVal(key,"card3",0x18);
	iTotalCard[4] = f.GetKeyVal(key,"card4",0x06);
	iTotalCard[5] = f.GetKeyVal(key,"card5",0x0B);
	iTotalCard[6] = f.GetKeyVal(key,"card6",0x1B);
	iTotalCard[7] = f.GetKeyVal(key,"card7",0x0B);
	iTotalCard[8] = f.GetKeyVal(key,"card8",0x3B);
	iTotalCard[9] = f.GetKeyVal(key,"card9",0x1a);
	iTotalCard[10] = f.GetKeyVal(key,"card10",0x0b);
	//f.CloseFile();
	return true;
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
	//if (m_iSendCardPos == CountPlayer()*3)//����λ�õ������һ����ҷ�������ʱ��������
	//{
	//	KillMyTimer(TIME_SEND_CARD);

	//	SendCardFinish();
	//	return TRUE;
	//}


	for(int i=0;i<m_bMaxPeople;i++)
	{
		if(m_pUserInfo[i]&&CanSendCard(i)&&m_iUserStation[i] != -1)//����û��ǿ�,��������������
		{
			m_iSendPos=i;
			break;
		}
	}
	SendCardStruct SendCard;

	memset(&SendCard,0,sizeof(SendCard));
	//SendCard.bDeskStatin = m_iSendPos;
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] == NULL)
		{
			continue;
		}
		m_iSendPos = i;
		for (int j=0; j<MAX_CARD_COUNT;j++)
		{
			BYTE bCard = m_iTotalCard[m_iSendPos * 3 + m_iUserCardCount[ m_iSendPos ]];
			m_iUserCard[ m_iSendPos ][m_iUserCardCount[m_iSendPos] ++ ] = bCard;
			//SendCard.bCard[i][j] = bCard;
		}
		SendCard.bCardCount[i] = m_iUserCardCount[ m_iSendPos ];
	}
	////�ĳ������ƶ��Ѿ�ȷ����
	//BYTE bCard = m_iTotalCard[m_iSendPos * 3 + m_iUserCardCount[ m_iSendPos ]];

	//m_iUserCard[ m_iSendPos ][m_iUserCardCount[m_iSendPos] ++ ] = bCard;
	////SendCard.iCard = bCard;



	for(int j=0;j<m_bMaxPeople;j++)
	{
		if(m_pUserInfo[j])
		{
 
			// ֻ����������ϵ���
			if (m_pUserInfo[j]->m_UserData.bGameMaster == SUPER_PLAYER||true ==m_bSuperUser[j])	// ����ǳ�����ң��·�������ҵ���
			{
				for (int cd=0; cd<PLAY_COUNT; ++cd)
				{
					memcpy(SendCard.bCard[cd], m_iUserCard[cd], sizeof(SendCard.bCard[0]));
				}
			}
			else	// ������ǳ�����ң����������������ݺ��ٸ����Լ���
			{
				memset(SendCard.bCard,0,sizeof(SendCard.bCard));
				//memcpy(SendCard.bCard[j], m_iUserCard[j], sizeof(SendCard.bCard[0]));//20100601����ʱ������ֵ��ȥ�����˿��ƺ��ٷ�
			}

			//����ǻ����ˣ����·��Լ�����
			if(m_pUserInfo[j]->m_UserData.isVirtual)
			{
				memcpy(SendCard.bCard[j], m_iUserCard[j], sizeof(SendCard.bCard[j]));
				//lym�����˷������е��ƣ�������������ܲ���
				for (int cd=0; cd<PLAY_COUNT; ++cd)
				{
					memcpy(SendCard.bCard[cd], m_iUserCard[cd], sizeof(SendCard.bCard[0]));
				}
			}

			SendGameData(j,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);
		}
	}
	SendWatchData(m_bMaxPeople,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_SEND_CARD,0);
	//		m_iUserCardCount[m_iSendCardPos%PLAY_COUNT]++;
	//m_iSendCardPos++;


	KillMyTimer(TIME_SEND_CARD);
	SetMyTimer(TIMER_SEND_END,CountPlayer() * 1000);
	//SendCardFinish();


	return TRUE;
}

//���ƽ���
BOOL CServerGameDesk::SendCardFinish()
{
	//����Ϣ�������ͻ��ˣ���Ϊû�д���
	//for (int i=0;i<m_bMaxPeople;i++) 
	//{
	//	if(m_pUserInfo[i])
	//		SendGameData(i,MDM_GM_GAME_NOTIFY,ASS_SEND_FINISH,0);
	//}
	//SendWatchData(m_bMaxPeople,MDM_GM_GAME_NOTIFY,ASS_SEND_FINISH,0);

	BeginPlayUpGrade();
	return true;
}

//�Ƿ����ӷ���
BOOL CServerGameDesk::CanSendCard(BYTE bDeskStation)
{
	int min=99;
	for(int i=0;i<m_bMaxPeople;i++)
	{
		if(m_iUserCardCount[i]<min&&m_pUserInfo[i])
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
	//m_iBaseOutCount=0;
	//m_iUpGradePeople = (m_iNtPeople==-1?0:m_iNtPeople);//GetCardGreater(false);						//��ȡ���������



	m_iOutCardPeople=m_iUpGradePeople;
	m_iFirstOutPeople=m_iUpGradePeople;

	m_iNowBigPeople=-1;						//��ǰ�����
	m_iFrontNote=0;

	BeginPlayStruct Begin;
	Begin.iOutDeskStation=m_iOutCardPeople;
	Begin.bNtPeople = m_iNtPeople;  //ׯ��λ��

	CString str="";

	memset(Begin.byUserData,255,sizeof(Begin.byUserData));//ȷ������Ƿ�����˺���ҵ��ƴ�С������ֻ���������ˣ�lym
	int stdata[PLAY_COUNT],index=0,temp=0;
	BYTE bRobot[PLAY_COUNT];
	memset(stdata,-1,sizeof(stdata));//λ������
	memset(bRobot,255,sizeof(bRobot));//�Ƿ������
	for (int i=0;i<m_bMaxPeople;i++)
	{
		if (m_pUserInfo[i] == NULL)
		{
			continue;
		}
		if(m_pUserInfo[i]->m_UserData.isVirtual)
		{
			bRobot[i] = 1;
		}
		else
		{
			bRobot[i] = 0;
		}
		stdata[index] = i;//��¼���λ��
		index++;
	}
	//��ȡ�ƴ�С����
	for(int i=0;i<index-1;i++)
	{//���ƴӴ�С������ҵ�λ��
		for(int j=i+1;j<index;j++)
		{
			if(m_Logic.CompareCard(m_iUserCard[stdata[j]],m_iUserCardCount[stdata[j]],m_iUserCard[stdata[i]],m_iUserCardCount[stdata[i]])==1)//stdata[i] λ�õ���û��stdata[j]���ƴ�
			{
				temp = stdata[i];
				stdata[i] = stdata[j];
				stdata[j] = temp;
			}
		}
	}

	for (int i=0;i<m_bMaxPeople;i++)
	{
		if (m_pUserInfo[i] == NULL)
		{
			continue;
		}
		memset(Begin.byUserData,255,sizeof(Begin.byUserData));//ȷ������Ƿ�����˺���ҵ��ƴ�С������ֻ���������ˣ�lym
		if(m_pUserInfo[i]->m_UserData.isVirtual)//�����˷����ƴ�С����
		{
			for(int j=0;j<PLAY_COUNT;j++)
			{
				Begin.byUserData[j][0] = bRobot[j];
				for(int k=0;k<PLAY_COUNT;k++)
				{
					if(stdata[k] == j)
					{
						Begin.byUserData[j][1] = k;
					}
				}
			}
		}
		SendGameData(i,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);
	}	
	SendWatchData(m_bMaxPeople,&Begin,sizeof(Begin),MDM_GM_GAME_NOTIFY,ASS_GAME_PLAY,0);

	m_bFirstNote = true;

	m_iTimerCount = m_iThinkTime + 3;
	SetMyTimer(TIMER_WAIT_NOTE, DEFAULT_WAIT_FREQUENCE);

	return TRUE;
}


//�����û����
BOOL CServerGameDesk::UserActiveResult(BYTE bDeskStation, BYTE iVerbType,int iNoteVal,BYTE bComparedStation)
{
	// ��⵱ǰ��������Ƿ�Ϸ�
	if (bDeskStation != m_iOutCardPeople || m_iUserStation[bDeskStation] == -1)
	{
		return TRUE;
	}
	//m_Logic.Msg("zhtlog:iNoteVal=%d",iNoteVal);
	switch (iVerbType)
	{
	case TYPE_ADD:
	case TYPE_NOTE:
		{
			// ��ֹ��ע���עΪ0����
			if (0 == iNoteVal)
			{
				return TRUE;
			}

			// ��ע������ע��Χ
			int iMoney = m_Ming[bDeskStation]?iNoteVal/2:iNoteVal;
			if (m_iFrontNote + iMoney > GetLimit())
			{
				return TRUE;
			}
		}
		break;
	case TYPE_FOLLOW:
		{
			// ��ֹ��һ�Ѹ�����, ǰ��Ѻע����С�ڵ���0
			if (m_iFrontNote <= 0)
			{
				return TRUE;
			}
		}
		break;
	case TYPE_COMPARE_CARD:
		{
			// һȦ��עδ���������ܱ���
			if (m_bAddNoteTime < CountPlayer())
			{
				return TRUE;
			}
		}
		break;
	}
	//add by ljl,�������Զ��ұ��Լ�С����ұ��ƣ�������Ȼ����˴��ҵ�һ����ҡ�
	if ( m_pUserInfo[bDeskStation]->m_UserData.isVirtual &&  TYPE_COMPARE_CARD == iVerbType)
	{
		BYTE byComparePeople = 255;
		//���������ѡ����Ƶ����
		int iRand = rand()%PLAY_COUNT;
		for ( int i=0; i<PLAY_COUNT; i++ )
		{
			iRand += 1;
			iRand %=PLAY_COUNT;
			if ( NULL != m_pUserInfo[iRand] && iRand != bDeskStation && m_iUserStation[iRand] != TYPE_GIVE_UP && m_iUserStation[iRand] != -1)
			{
				byComparePeople = iRand;
				break;
			}
		}
		bComparedStation = byComparePeople;
	}
	BZWLockForCriticalSection cs(&m_csForIPCMessage);
	if (m_iLastOutPeople == m_iOutCardPeople)
	{
		return true;
	}
	if (iVerbType != TYPE_LOOKCARD)
	{
		m_iLastOutPeople = m_iOutCardPeople;
		m_iUserStation[bDeskStation] = iVerbType;									//Ѻע����
	}
	int frontplay;//��һλ��ע��
	for(int i=1;i<PLAY_COUNT;i++)
	{
		frontplay=(bDeskStation-i<0)?PLAY_COUNT+(bDeskStation-i):bDeskStation-i;//���Ƴ���һλ��ע��(�����һ����ע��)
		if(!m_pUserInfo[frontplay])		
			continue;

		if(m_iUserStation[frontplay]!=TYPE_GIVE_UP)
			break;
	}
	m_bCutLastPeoPle = bDeskStation;
	switch (iVerbType)
	{
	case TYPE_OPENCARD:
		{		
			if(m_iNowBigPeople!=-1)
			{	
				if(m_Ming[bDeskStation]==m_Ming[m_iNowBigPeople])
					m_iNowNote[bDeskStation]=m_iPerJuTotalNote[m_iNowBigPeople]-m_iPerJuTotalNote[bDeskStation];//m_iFrontNote;//m_iPerJuTotalNote[m_iNowBigPeople]-m_iPerJuTotalNote[bDeskStation];	//��ǰѺע��
				else
				{
					if(m_Ming[bDeskStation])
						m_iNowNote[bDeskStation]=m_iPerJuTotalNote[m_iNowBigPeople]*2-m_iPerJuTotalNote[bDeskStation];//m_iFrontNote*2;
					else
						m_iNowNote[bDeskStation]=m_iPerJuTotalNote[m_iNowBigPeople]/2-m_iPerJuTotalNote[bDeskStation];//m_iFrontNote/2;

				}
				if(m_Ming[bDeskStation])
					m_iNowNote[bDeskStation]+=m_iLimitNote/20*5;
				else
					m_iNowNote[bDeskStation]+=m_iLimitNote/20*3;
			}
			else 
			{
				if(m_Ming[bDeskStation])
					m_iNowNote[bDeskStation]=m_iLimitNote/20*5;
				else
					m_iNowNote[bDeskStation]=m_iLimitNote/20*3;
			}

			m_iUserNote[bDeskStation]=m_iUserNote[bDeskStation]+m_iNowNote[bDeskStation];
			m_iPerJuTotalNote[bDeskStation]=m_iPerJuTotalNote[bDeskStation]+m_iNowNote[bDeskStation];


			break;
		}
	case TYPE_COMPARE_CARD: //����
		{  

			if (m_Ming[bDeskStation])
			{
				m_iNowNote[bDeskStation] = m_iFrontNote * 4;
			}
			else 
			{
				m_iNowNote[bDeskStation] = m_iFrontNote * 2;
			}

			m_iPerJuTotalNote[bDeskStation] += m_iNowNote[bDeskStation];
			m_iUserNote[bDeskStation] = m_iNowNote[bDeskStation];/*m_iUserNote[bDeskStation]+*/	

			m_bAddNoteTime++;

			//m_iNowBigPeople=bDeskStation;
			break;
		}

	case TYPE_FOLLOW:  // Ѻע��=��ǰ���ע��-�Ѿ���ע��
		{	
			if (m_iNowBigPeople == -1)
			{
				return FALSE;
			}

			if (m_Ming[bDeskStation])
			{
				m_iNowNote[bDeskStation] = (m_iFrontNote) * 2;
			}
			else
			{
				m_iNowNote[bDeskStation] = m_iFrontNote;
			}


			CString str="";
			str.Format("��ע���ԣ������� ���� Note=%I64d  λ��%d ��ǰ��ע%I64d",m_iNowNote[bDeskStation],bDeskStation,m_iFrontNote);
			OutputDebugString(str);


			m_iUserNote[bDeskStation] = m_iNowNote[bDeskStation];	//�������Ѻע��=���Ѻע��+��ע��
			m_iPerJuTotalNote[bDeskStation] += m_iNowNote[bDeskStation];		//�û�һ��Ѻע��			

			m_bFirstNote = false;


			m_bAddNoteTime++;

			break;
		}
	case TYPE_ADD://��ע��=��ǰ���ע��-�û��Ѿ���ע��+��ע��
		{
			if(m_iNowBigPeople==-1)
				return false;			
		
			if (m_Ming[bDeskStation])
			{
				m_iNowNote[bDeskStation] = (m_iFrontNote + iNoteVal) * 2;
			}
			else
			{
				m_iNowNote[bDeskStation] = m_iFrontNote + iNoteVal;
			}
			//m_iNowNote[bDeskStation]=m_iFrontNote+iNoteVal;//m_iPerJuTotalNote[m_iNowBigPeople]-m_iPerJuTotalNote[bDeskStation]+iNoteVal;	//��ǰѺע��


			CString str="";
			str.Format("��ע���ԣ������� ��ע Note=%I64d  λ��%d ��ǰ��ע%I64d",m_iNowNote[bDeskStation],bDeskStation,m_iFrontNote);
			OutputDebugString(str);

			//����Ѻע����	
			m_iNowBigPeople=bDeskStation;
			int temp=m_iPerJuTotalNote[bDeskStation]+m_iNowNote[bDeskStation] ;

			m_iPerJuTotalNote[bDeskStation] += m_iNowNote[bDeskStation];
			m_iUserNote[bDeskStation] = m_iNowNote[bDeskStation];/*m_iUserNote[bDeskStation]+*/	
			m_iFrontNote += iNoteVal;
			m_bFirstNote = false;

			m_bAddNoteTime++;

			break;
		}
		break;
	case TYPE_NOTE:
		{
			//int iChipIn = iNoteVal * m_iBaseNote;
			if(m_Ming[bDeskStation])
			{  
				m_iNowNote[bDeskStation] = iNoteVal*2;
	
			}
			else
				m_iNowNote[bDeskStation] = iNoteVal;

			m_iUserNote[bDeskStation] = m_iNowNote[bDeskStation];	//�������Ѻע��=���Ѻע��+��ע��
			m_iPerJuTotalNote[bDeskStation] += m_iNowNote[bDeskStation];		//�û�һ��Ѻע��			
			m_iNowBigPeople = bDeskStation;
			m_iFrontNote = iNoteVal;

			m_bFirstNote = false;

			m_bAddNoteTime++;

		}
		break;

	case TYPE_GIVE_UP:														//����
		{
			if(	m_iNowBigPeople==-1)										//��һ���󱣴���Ϊ���ֵ
				m_iNowBigPeople=bDeskStation;									
			m_iNowNote[bDeskStation]=0;											//��ǰѺע��Ϊ0

			//memset(m_iUserCard[bDeskStation],0,sizeof(m_iUserCard[bDeskStation]));//�������

			m_bIsGiveUp[bDeskStation] = true;
			++m_bAddNoteTime;

			//add by ljl 
			m_iLimitNote = GetMinMoney();
			break;
		}
	case TYPE_LOOKCARD:
		{
			lookCardResult result;
			memset(&result,0,sizeof(result));
			result.bDeskStation=bDeskStation;
			m_ihelp[bDeskStation]=1;

			//������ƺ��ﵽϵͳ���ޣ�����
			if ((m_iPerJuTotalNote[bDeskStation] + m_iFrontNote * 2) > m_iLimitNote)  
			{
				m_bReachLimitPlayer = bDeskStation;		// Added by zxd 20090827 ���ȴﵽ���޵���ң�������ͬ�ƣ��Դ�Ϊ����ʤ���ĸ���
				//����Ѻע���
				NoteResult noteresult;					
				noteresult.bAddStyle = TYPE_REACH_LIMITE_SELF;										//��ע����
				noteresult.iOutPeople = m_iOutCardPeople;								//������ע��
				noteresult.iCurNote = 0;							//�����û���ע��
				noteresult.iNote = m_iFrontNote;

				KillMyTimer(TIMER_WAIT_NOTE);
				m_iTimerCount = 0;

				noteresult.bNextDeskStation = bDeskStation;

				for (int i = 0;i < m_bMaxPeople;i++)
				{
					if(m_pUserInfo[i])
						SendGameData(i,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_NOTE_RESULT,0);
				}
				SendWatchData(m_bMaxPeople,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_NOTE_RESULT,0);	


				FinishCompare();
				SetMyTimer(TIME_GAME_FINISH,2000);
				return true;


			}

			result.iUserCardCount = m_iUserCardCount[bDeskStation];
			
			//ת��������Ϣ
			for (int i = 0;i < PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i] == NULL)
				{
					continue;
				}
				if (i == bDeskStation)
				{
					memcpy(result.iUserCard, m_iUserCard[i], sizeof(result.iUserCard));
				}
				else
				{
					memset(result.iUserCard, 0, sizeof(result.iUserCard));
				}
				SendGameData(i,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);
			}

			SendWatchData(m_bMaxPeople,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);

			return true;
			break;
		}
	default:
		break;
	}
	//���ۼ���ע����
	m_iTotalNote += m_iNowNote[bDeskStation];									//������ע��


	m_iThisGameNote[bDeskStation] += m_iNowNote[bDeskStation];
	m_iTotalGameNote[bDeskStation] += m_iNowNote[bDeskStation];

	if (iVerbType == TYPE_COMPARE_CARD)
	{
		tagCompareCardResult result;

		int IsWin = m_Logic.CompareCard(m_iUserCard[bDeskStation],m_iUserCardCount[bDeskStation],m_iUserCard[bComparedStation],m_iUserCardCount[bComparedStation]);
		if (IsWin == -1)  //���Ƚ��ŵ��ƴ�
		{
			m_iUserStation[bDeskStation] = TYPE_GIVE_UP;
			result.iLoster = bDeskStation;
			result.bWinner = bComparedStation;
			
		}
		else   //�����߱Ƚϴ�
		{
			m_iUserStation[bComparedStation] = TYPE_GIVE_UP;
			result.iLoster = bComparedStation;
			result.bWinner = bDeskStation;

		}
		m_byWin[result.iLoster]  = 1;
		m_byWin[result.bWinner]  = 2;

		UINT uFinishType = CheckFinish();

		m_bOpenLoser[result.iLoster] = true;
		result.iNt = bDeskStation;
		result.iNoNt = bComparedStation;

		result.iCurNote = m_iNowNote[bDeskStation];							//�����û���ע��
		result.iNote = m_iFrontNote;

		if (uFinishType == FINISH_TYPE_REACH_LIMITE_SELF)
		{
			result.bAddStyle = TYPE_REACH_LIMITE_SELF; //�ﵽ����
		}
		else if (uFinishType == FINISH_TYPE_REACH_LIMITE_NEXT)
		{
			result.bAddStyle = TYPE_REACH_LIMITE_NEXT; //�¼һ�ﵽ����
		}
		else
		{
			result.bAddStyle = 0;
		}
		if(uFinishType > FINISH_TYPE_UNFINISH)
		{
			result.bGameFinish = true;
			result.bWinner = GetCardGreater(true); // Add by zxd ���ƽ�����ֱ�Ӿ���Ϸ��������Ϊ��һ���ٸ�ע�ͻ�ﵽ��ע�����ˣ���ʱ����Ҫ�����ж�˭����ʤ����
		}
		else
		{
			result.bGameFinish = false;
		}



		for (int i = 0;i < m_bMaxPeople;i++)
		{
			if (m_pUserInfo[i] != NULL)
			{
				SendGameData(i,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_BIPAI_RESULT,0);
			}
		}

		//SendWatchData(m_bMaxPeople,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_BIPAI_RESULT,0);
		SendWatchData(m_bMaxPeople,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_BIPAI_RESULT,0);

		////////////////����������һ����ʱ��,��ʱ����ͣ6��֮��,�ټ���������һ���˵Ĳ���.//////////////////1013
		if (uFinishType > FINISH_TYPE_UNFINISH)
		{
			SetMyTimer(TIME_GAME_FINISH, 5000);
			return true;
		}
		else
		{
			SetMyTimer(TIME_BIPAI, 5000);
			return TRUE;
		}		
	}

	UINT uFinishType = CheckFinish();



	BYTE bNextStation = GetNextNoteStation(m_iOutCardPeople);


	//����Ѻע���
	NoteResult noteresult;					
	noteresult.bAddStyle = iVerbType;										//��ע����
	noteresult.iOutPeople = m_iOutCardPeople;								//������ע��
	noteresult.iCurNote = m_iNowNote[bDeskStation];							//�����û���ע��
	noteresult.iNote = m_iFrontNote;
	noteresult.iLimiteNote = m_iLimitNote;

	KillMyTimer(TIMER_WAIT_NOTE);
	m_iTimerCount = 0;

	////��һѺע��


	//�ﵽ����
	if ( bNextStation != 255)
	{
		if (uFinishType == FINISH_TYPE_REACH_LIMITE_NEXT)
		{
			noteresult.bAddStyle = TYPE_REACH_LIMITE_NEXT; //�ﵽ����
		}
		else if (uFinishType == FINISH_TYPE_REACH_LIMITE_SELF)
		{
			noteresult.bAddStyle = TYPE_REACH_LIMITE_SELF; //�ﵽ����
		}

	}

	noteresult.bNextDeskStation = bNextStation;
	/*noteresult.iLimiteNote = m_iLimitNote;*/
	for (int i = 0;i < m_bMaxPeople;i++)
	{
		if(m_pUserInfo[i])
			SendGameData(i,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_NOTE_RESULT,0);
	}
	SendWatchData(m_bMaxPeople,&noteresult,sizeof(noteresult),MDM_GM_GAME_NOTIFY,ASS_NOTE_RESULT,0);	

	if(iVerbType == TYPE_OPENCARD)
	{

		FinishCompare();
		SetMyTimer(TIME_GAME_FINISH, 2000);
		return true;
	}
	if (uFinishType > FINISH_TYPE_UNFINISH)
	{
		//��������ʱ�����ÿͻ��˽������޵ĳ��붯������֮�����ռ�zht209100511
		SetMyTimer(TIMER_FINISH_COMPARE, 2000);

		return true;
	}


	m_iOutCardPeople = bNextStation;
	if(m_iUserStation[m_iFirstOutPeople]==TYPE_GIVE_UP)//�����ǰׯ�ѷ���,����������һ�������һ��ע��
		m_iFirstOutPeople=m_iOutCardPeople;




	//��һѺע��Ϊ�������Ѻע��,һ���Ѿ�������Ҫ�·�һ����
	//ֱ���µ���ע�����棬����Ҫ
	//if (m_iOutCardPeople==m_iNowBigPeople)												//�Ƿ���һ�ֿ�ʼ
	//{	
	//	m_iFirstOutPeople=m_iOutCardPeople;
	//	SetMyTimer(TIME_COLLECT_JETON,1000);
	//	//NewPlayTurn(m_iOutCardPeople);
	//}
	//else
	{
		tagUserResult result;
		result.bCountNotePeople=CountNotePeople();
		result.bAddTime=m_bAddNoteTime;								//��ע����
		result.iOutPeople = m_iOutCardPeople;						//��ע��
		result.iNowBigNote=m_iUserNote[m_iNowBigPeople];			//���������ע��
		result.iNowBigNoteStyle=m_iUserStation[m_iNowBigPeople];	//��ǰ�����ע����
		result.iTotalNote=m_iTotalNote;//��ע	
		result.iNowBigPeople=m_iNowBigPeople;
		result.iFirstOutPeople=m_iFirstOutPeople;
		result.bIsFirstNote = m_bFirstNote;

		::CopyMemory(result.iUserNote,&m_iUserNote,sizeof(m_iUserNote));
		for (int i = 0;i < PLAY_COUNT;i++)
		{
			if(m_pUserInfo[i])		
				SendGameData(i,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);
		}	
		SendWatchData(m_bMaxPeople,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);
	}


	
	m_iTimerCount = m_iThinkTime + 3;
	SetMyTimer(TIMER_WAIT_NOTE,DEFAULT_WAIT_FREQUENCE);

	//if (m_arybCutUser[m_iOutCardPeople])
	//{
	//	SetMyTimer(TIME_CUT_AI,TIMER_CUT_AI_LONG);   //���ö�ʱ���������й�

	//}
	
	return TRUE;
}

BOOL CServerGameDesk::BiPaiActiveResult()
{

	//if (CheckFinish())	
	//{//�Ƿ����
	//	//if (uFinishType > FINISH_TYPE_UNFINISH)
	//	//{
	//		SetMyTimer(TIME_GAME_FINISH,2000);
	//	//	return true;
	//	//}
	//	return TRUE;
	//}

	//}
	BYTE bNext = GetNextNoteStation(m_iOutCardPeople);

	m_iOutCardPeople = bNext;

	if(m_iUserStation[m_iFirstOutPeople]==TYPE_GIVE_UP)//�����ǰׯ�ѷ���,����������һ�������һ��ע��
		m_iFirstOutPeople=m_iOutCardPeople;


	tagUserResult result;
	result.bCountNotePeople=CountNotePeople();
	//		result.bAddTime=m_bAddNoteTime;								//��ע����
	result.iOutPeople = m_iOutCardPeople;						//��ע��
	result.iNowBigNote=m_iUserNote[m_iNowBigPeople];			//���������ע��
	result.iNowBigNoteStyle=m_iUserStation[m_iNowBigPeople];	//��ǰ�����ע����
	result.iTotalNote=m_iTotalNote;//��ע	
	result.iNowBigPeople=m_iNowBigPeople;
	result.iFirstOutPeople=m_iFirstOutPeople;
	result.bAddTime = m_bAddNoteTime;	
	result.bIsFirstNote = m_bFirstNote;
	::CopyMemory(result.iUserNote,&m_iUserNote,sizeof(m_iUserNote));
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if(m_pUserInfo[i])		
			SendGameData(i,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);
	}	
	SendWatchData(m_bMaxPeople,&result,sizeof(result),MDM_GM_GAME_NOTIFY,ASS_NOTE,0);



	m_iTimerCount = m_iThinkTime + 3;
	SetMyTimer(TIMER_WAIT_NOTE,DEFAULT_WAIT_FREQUENCE);

	//if (m_arybCutUser[m_iOutCardPeople])
	//{
	//	SetMyTimer(TIME_CUT_AI,TIMER_CUT_AI_LONG);   //���ö�ʱ���������й�

	//}

	return TRUE;
}


BYTE CServerGameDesk::CountNotePeople()
{
	int iCount=0;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(!m_pUserInfo[i]||m_iUserStation[i] == -1)		
			continue;
		if(m_iUserStation[i]!=TYPE_GIVE_UP)
			iCount++;
	}
	return iCount;
}
//����Ƿ����
UINT CServerGameDesk::CheckFinish()
{
	//����Ƿ����
	/*******************
	����������
	*******************/
	//	bool bFinish = false;
	int iShuttle = 0,iGiveUp = 0;

	int note=m_iFrontNote*2;

	BYTE backpaly = GetNextNoteStation(m_iOutCardPeople);//��һλ��ע��
	//for (int i = 1; i < PLAY_COUNT; i++)
	//{
	//	backpaly=(m_iOutCardPeople+i)%PLAY_COUNT;
	//	if(!m_pUserInfo[backpaly])		
	//		continue;
	//	if(m_iUserStation[backpaly]!=TYPE_GIVE_UP)
	//		break;
	//}

	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if(!m_pUserInfo[i])
			continue;

		if (m_iUserStation[i] == TYPE_GIVE_UP)
		{
			iGiveUp++;
			continue;
		}
		if(i==m_iOutCardPeople)
			continue;


		//if(m_Ming[m_iOutCardPeople]!=m_Ming[i])
		//{
		//	if(m_Ming[i])

		//		note=note*2;
		//	else
		//		note=note/2;
		//}

		////if(!m_pUserInfo[i])		
		////	continue;
		//int temp=(m_pUserInfo[i]->m_UserData.dwMoney)-m_iPerJuTotalNote[i];
		//if(temp<note&&GetDeskBasePoint())

		//{
		//	//	if(m_iOutCardPeople==m_iFirstOutPeople&&m_iOutCardPeople==i)
		//	{
		//		//����
		//		SetMyTimer(TIME_GAME_FINISH,2000);
		//		return true;
		//	}
		//}

	}	

	//	if (((iShuttle + iGiveUp) == CountPlayer())&&!bFinish || (iGiveUp ==  CountPlayer()-1))			//�����ҷ���,�������Ѿ������ҷ����ĺ����𻨵����Ϊ��Ϸ����
	if (iGiveUp ==  CountPlayer()-1)			//�����ҷ���,�������Ѿ������ҷ����ĺ����𻨵����Ϊ��Ϸ����
	{
		//����
		//SetMyTimer(TIME_GAME_FINISH,2000);
		return FINISH_TYPE_FISH;
	}

	if (backpaly == 255)
	{
		return FINISH_TYPE_FISH;  //��Ϸ����
	}

	if (m_iPerJuTotalNote[m_iOutCardPeople] > m_iLimitNote)
	{
		m_bReachLimitPlayer = m_iOutCardPeople; // Added by zxd 20090827 ���ȴﵽ���޵���ң�������ͬ�ƣ��Դ�Ϊ����ʤ���ĸ���
		return FINISH_TYPE_REACH_LIMITE_SELF;
	}


	//int temp = m_iPerJuTotalNote[backpaly];

	//if(!m_Ming[backpaly])
	//	temp = m_iPerJuTotalNote[backpaly]*2;
	int iFrontNote = m_iFrontNote;
	if (m_Ming[backpaly])
	{
		iFrontNote *=2;
	}

	//��ǰ���Ƶĺ�һ��Ҵﵽ���޾��Զ�����
	if(m_iPerJuTotalNote[backpaly] + iFrontNote > m_iLimitNote) //������ע�ﵽ���ޣ�����Ϸ����
	{
		//����
		//SetMyTimer(TIME_GAME_FINISH,2000);
		m_bReachLimitPlayer = backpaly;	// Added by zxd 20090827 ���ȴﵽ���޵���ң�������ͬ�ƣ��Դ�Ϊ����ʤ���ĸ���
		return FINISH_TYPE_REACH_LIMITE_NEXT;   //�ﵽ���ޣ�����
	}


	return FINISH_TYPE_UNFINISH;  //û�н���

}
BOOL CServerGameDesk::CollectJeton(BOOL bOnly)
{
	KillMyTimer(TIME_COLLECT_JETON);
	//	Sleep(1000);
	CollectJetonStruct jeton;
	for (int i = 0;i < PLAY_COUNT;i++) 
	{
		if (m_pUserInfo[i] == NULL)
		{
			continue;
		}
		SendGameData(i,&jeton,sizeof(jeton),MDM_GM_GAME_NOTIFY,ASS_COLLECT_JETON,0);
	}

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
 SetMyTimer(TIME_GAME_FINISH,2000);
 return TRUE;
 }
 m_iOutCardPeople = GetCardGreater(false);//iGreater;
 //��ʼ����λ��
 m_iSendPos=0;
 SetMyTimer(TIME_SEND_A_CARD,SEND_CARD_TIME);

 //		SendACard(m_iOutCardPeople);	
 //�����־�ȫ�����,�����һ���ƾͱȽϴ�С
 //	}*/
	if(m_Ming[m_iNowBigPeople])
		m_iMaxNote+=m_iUserNote[m_iNowBigPeople];
	else
		m_iMaxNote+=m_iUserNote[m_iNowBigPeople]*2;
	m_iNowBigPeople=-1;
	m_iOutCardPeople=-1;
	for(int i=0;i<PLAY_COUNT;i++)
		m_iUserNote[i]=0;
	SendUserNote();
	return TRUE;
}

BOOL CServerGameDesk::SendACard()
{
	//if(m_iSendPos>=PLAY_COUNT)//δ������ÿ�˷���һ����
	//{
	//	KillMyTimer(TIME_SEND_A_CARD);
	//	SendUserNote();
	//	return true;
	//}
	////��һ����
	//SendCardStruct SendCard;
	//int pos=(m_iOutCardPeople+m_iSendPos)%PLAY_COUNT;		//��ʼ����λ��
	////���ĳλ���Ƿ���,������һ�����
	//if (m_iUserStation[pos]!= TYPE_GIVE_UP&&m_pUserInfo[pos]) 
	//{
	//	m_iUserCard[pos][m_iUserCardCount[pos]] = m_iTotalCard[m_iSendCardPos];
	//	SendCard.iCard = m_iUserCard[pos][m_iUserCardCount[pos]];				//������ֵ
	//	SendCard.bDeskStatin=pos;												//������λ��
	//	m_iUserCardCount[pos]++;												//�û�����������
	//	m_iSendCardPos++;													//����λ��
	//	m_iUserNote[pos] = 0;													//Ѻע��ʼ
	//	m_iNowNote[pos]=0;													//��һ��Ѻע��
	//	m_iSendPos++;

	//	for (int i = 0;i < PLAY_COUNT;i++) 
	//		SendGameData(i,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_SEND_A_CARD,0);
	//	SendWatchData(m_bMaxPeople,&SendCard,sizeof(SendCard),MDM_GM_GAME_NOTIFY,ASS_SEND_A_CARD,0);
	//	//		}
	//}
	//else
	//	m_iSendPos++;

	return TRUE;
}

BOOL CServerGameDesk::SendUserNote()
{
	//����Ƿ���Խ���(����һ�ֶ������������½��з���)����Ϸ�������ʡ
	//		if(CheckFinish())
	//		{
	//			return true;
	//		}
	//���ҵ�ǰ�����������
	/*		int iGreater = -1;
	for (int i = 0;i < PLAY_COUNT;i++)
	{	
	if (m_iUserStation[i] == TYPE_GIVE_UP)
	continue;
	if (iGreater < 0)
	{
	iGreater = i;
	continue;
	}
	int ret = m_Logic.CompareCard(m_iUserCard[iGreater],m_iUserCardCount[iGreater],m_iUserCard[i],m_iUserCardCount[i]);
	if (ret > 0) 
	continue;
	else iGreater = i;
	}*/

	m_iOutCardPeople =m_iFirstOutPeople;// GetCardGreater(false);//iGreater;

	/*	TCHAR sz[200],temp[100];
	wsprintf(sz,"SendUserNote//m_iUpGradePeople=%d,�������������%s,����=%d",
	m_iUpGradePeople,m_pUserInfo[m_iUpGradePeople]->m_UserData.nickName,m_iUserCard[m_iOutCardPeople],m_iUserCardCount[m_iOutCardPeople]);
	WriteStr(sz);

	for(int i=0;i<PLAY_COUNT;i++)		
	{
	wsprintf(sz,"%s\t,����=%d\t",m_pUserInfo[i]->m_UserData.nickName,m_Logic.GetCardShape(m_iUserCard[i],m_iUserCardCount[i]));
	for(int j=0;j<m_iUserCardCount[i];j++)
	{
	wsprintf(temp,"%d=%x\t",i,m_iUserCard[i][j]);
	lstrcat(sz,temp);
	}
	WriteStr(sz);
	}*/
	//��һ����ע��
	tagUserResult result;
	//	result.bAddTime=m_bAddNoteTime;
	result.iOutPeople = m_iOutCardPeople;
	result.bCountNotePeople=CountNotePeople();
	result.iNowBigNote=0;//m_iUserNote[m_iNowBigPeople];
	result.iNowBigNoteStyle=0;
	result.iFirstOutPeople=m_iFirstOutPeople;

	::CopyMemory(result.iUserStation,&m_iUserStation,sizeof(m_iUserStation));
	::CopyMemory(result.iUserNote,&m_iUserNote,sizeof(m_iUserNote));
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
	int iGreater =m_bReachLimitPlayer;// -1;
	if(m_iUserStation[iGreater]==TYPE_GIVE_UP)
	{
		iGreater=-1;
	}
	// ������ﵽ���޵���ҿ�ʼ��������Ƚ�
	int nToCompare = 0;
	for (int i = 0;i < m_bMaxPeople;i++)
	{
		nToCompare = (m_bMaxPeople - 1 - i +m_bReachLimitPlayer)%m_bMaxPeople;
		if (m_iUserStation[nToCompare] == TYPE_GIVE_UP||(m_pUserInfo[nToCompare]==NULL) || m_iUserStation[nToCompare] == -1)
			continue;
		if (iGreater < 0)	// ���û�г�ֵ�����һ��Ϊ��ֵ��������һ�αȽ�
		{
			iGreater = nToCompare;
			continue;
		}
		if(nToCompare == m_bReachLimitPlayer)// ������ǳ�ֵ��������һ�αȽ�
			continue;
		int ret; 		
		if(!bExtVal)		//�Ƚ�����
			ret = m_Logic.CompareCard(m_iUserCard[iGreater]+1,m_iUserCardCount[iGreater]-1,m_iUserCard[nToCompare]+1,m_iUserCardCount[nToCompare]-1);
		else				//�Ƚ�������
			ret= m_Logic.CompareCard(m_iUserCard[iGreater],m_iUserCardCount[iGreater],m_iUserCard[nToCompare],m_iUserCardCount[nToCompare]);

		/*		if(m_iUserCardCount[0]>4)
		{
		TCHAR sz[200];
		wsprintf(sz,"bExtVal=%d,ret=%d",bExtVal,ret);
		WriteStr(sz,1,1);
		}*/
		if (ret > 0)
			continue;
		else iGreater = nToCompare;
	}
	return iGreater;
}

//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	KillTimer(TIME_MY_TIMER);
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{	
			BYTE  iStation = m_bGameStation ;
			//�������� 
			m_bGameStation=GS_WAIT_ARGEE;

			for (int i=0;i<PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i]!=NULL) 
					m_pUserInfo[i]->m_UserData.bUserState=USER_SITTING;
			}	

			int winer=GetCardGreater(true);//m_iPerJuTotalNote[i];//
			m_iNtPeople=winer;

			//��Ϸ����
			GameEndStruct GameEnd;
			::memset(&GameEnd,0,sizeof(GameEnd));
			GameEnd.iUpGradeStation=m_iUpGradePeople;

			//�����ͻ��˵�ǰ����
			for (int i = 0;i < PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i]==NULL || m_iUserStation[i] == -1) 
					continue;
				//���ʹ���
				if(m_iUserStation[i]!=TYPE_GIVE_UP)//&&m_iUserCardCount[i]==5)
				{
					GameEnd.iUserState[i] = m_iUserStation[i];
					GameEnd.iCardShape[i]=m_Logic.GetCardShape(m_iUserCard[i],m_iUserCardCount[i]);
				}
				else
					if(i!=winer)
						GameEnd.iCardShape[i]=0;
					else 
						GameEnd.iCardShape[i]=10;
			}

			for (int i = 0;i < PLAY_COUNT;i++)
			{
				if (m_pUserInfo[i]==NULL) 
					continue;
				if(i==winer)
				{
					GameEnd.iTurePoint[i]=m_iTotalNote-m_iPerJuTotalNote[i]; 

				}
				else
					GameEnd.iTurePoint[i]=-m_iPerJuTotalNote[i];				//�������Ѻ��ע
			}

			bool  bExceptional  = false;
//			int  iGuoDi = m_pDataManage->m_InitData.uGuoDi ;
			for(int  i = 0 ; i<PLAY_COUNT;i++)
			{
				if(NULL == m_pUserInfo[i])
				{
					continue;
				}
				else
				{
					if(m_iPerJuTotalNote[i]<m_iGuoDi)
					{
						bExceptional = true ; 						
						break ;
					}
				}
			}

			if(true == bExceptional)
			{
				CString strDebug ; 
				DebugPrintf(m_pDataManage->m_InitData.uRoomID,"����˼�⵽�쳣�������������");
				DebugPrintf(m_pDataManage->m_InitData.uRoomID,"----------------------------");				
				for(int i = 0 ; i<PLAY_COUNT ; i++)
				{
					if(NULL == m_pUserInfo[i])
					{
						continue;
					}
					strDebug.Format("��ǰ��ҵ��߼�λ��==%d,�������==%s����ǰ����ʱ��״̬==%d,��ǰ��ҵ���ע==%I64d,��ǰ��ҵĴ������״̬==%d,����==%d",i ,m_pUserInfo[i]->m_UserData.szName,iStation,m_iPerJuTotalNote[i],m_iUserStation[i],m_iGuoDi);
					DebugPrintf(m_pDataManage->m_InitData.uRoomID,strDebug);
				}
				DebugPrintf(m_pDataManage->m_InitData.uRoomID,"---------------------");

			}			


			//д�����ݿ�				
			//д�����ݿ�				
			
			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));
			for (int i=0;i<PLAY_COUNT;i++)
			{
				//temp_point[i]=GameEnd.iTurePoint[i];//+m_iAwardPoint[i];
				GameEnd.iChangeMoney[i]=GameEnd.iTurePoint[i]/**GetRoomBasePoint()*/;
			}
			ChangeUserPointint64(GameEnd.iTurePoint, temp_cut/*,GameEnd.iChangeMoney,true*/);
			//ChangeUserPoint(GameEnd.iTurePoint, temp_point);
			__super::RecoderGameInfo(GameEnd.iChangeMoney);


			//������ҵ����·�
			for (int i=0; i<PLAY_COUNT; ++i)
			{
				memcpy(GameEnd.bCard[i], m_iUserCard[i], sizeof(GameEnd.bCard[0]));
			}
			//��������
			for (int i=0;i<PLAY_COUNT;i++) 
			{
				if (m_pUserInfo[i]!=NULL) 
					SendGameData(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);
			}
			SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_CONTINUE_END,0);

			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);

			return true;
		}
	case GF_SALE:			//��Ϸ��ȫ����
		{
			m_bGameStation=GS_WAIT_ARGEE;//GS_WAIT_SETGAME;

			for (int i=0;i<m_bMaxPeople;i++)
			{
				if (m_pUserInfo[i]!=NULL) 
					m_pUserInfo[i]->m_UserData.bUserState=USER_SITTING;
			}
			GameCutStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));
			CutEnd.bDeskStation=bDeskStation;
			for (int i=0;i<m_bMaxPeople;i++)
			{
				if (m_pUserInfo[i] == NULL)
				{
					continue;
				}
				SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);
			}
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SALE_END,0);
			bCloseFlag=GFF_FORCE_FINISH;

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
			CutEnd.iTurePoint[bDeskStation]=-GetRunPublish()-m_iPerJuTotalNote[bDeskStation];//m_iTotalNote;			//�۳�ǿ�����ȫ����ע
			//CutEnd.iTurePoint[(bDeskStation+1)%PLAY_COUNT]=(m_iTotalNote-m_iBaseNote)/(CountPlayer()-1);
			//CutEnd.iTurePoint[(bDeskStation+2)%PLAY_COUNT]=(m_iTotalNote-m_iBaseNote)/(CountPlayer()-1);
			//CutEnd.iTurePoint[(bDeskStation+3)%PLAY_COUNT]=(m_iTotalNote-m_iBaseNote)/(CountPlayer()-1);
			////CutEnd.iTurePoint[(bDeskStation+4)%PLAY_COUNT]=(m_iTotalNote-m_iBaseNote)/(CountPlayer()-1);
			//CutEnd.iTurePoint[(bDeskStation+5)%PLAY_COUNT]=(m_iTotalNote-m_iBaseNote)/(CountPlayer()-1);

			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));

			ChangeUserPointint64(CutEnd.iTurePoint, temp_cut);
			//__super::RecoderGameInfo(CutEnd.iChangeMoney);

			__super::RecoderGameInfo(CutEnd.iChangeMoney);
			for (int i=0;i<m_bMaxPeople;i++)
			{
				if (m_pUserInfo[i] != NULL)
				{
					SendGameData(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
				}

			}
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

//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if(m_bGameStation>=GS_SEND_CARD&&m_bGameStation<GS_WAIT_NEXT && m_iUserStation[bDeskStation] != -1)
	{
		return true;
	}
	else
	{
		return false;
	}
	//return __super::IsPlayGame(bDeskStation);
}

//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	if (this->IsPlayGame(bDeskStation))
	{
		m_arybCutUser[bDeskStation] = true;
	}	
	else
	{
		m_iNtPeople=255;	
	}

	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}

//�û�����
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	m_arybCutUser[bDeskStation] = true;

	BYTE count = 0;
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if (m_arybCutUser[i])
		{
			count++;
		}
	}
	if (count >= PLAY_COUNT)
	{
		__super::UserNetCut(bDeskStation,pUserInfo);
		GameFinish(0,GF_SALE);
		return true;
	}

	return __super::UserNetCut(bDeskStation,pUserInfo);
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
	{
		if (m_pUserInfo[i] != NULL)
		{
			SendGameData(i,&back,sizeof(back),MDM_GM_GAME_NOTIFY,ASS_GM_SUPERCLIENT,0);
		}

	}
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
	return m_pDataManage->m_InitData.uRunPublish;
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

BYTE CServerGameDesk::GetNextNoteStation(BYTE bCurDeskStation)
{
	BYTE bNextStation = bCurDeskStation;

	int kkk = PLAY_COUNT - 1;   //��ʱ��
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		bNextStation = (bNextStation + kkk) % PLAY_COUNT;
		if(!m_pUserInfo[bNextStation])
			continue;	
		if (m_iUserStation[bNextStation] != TYPE_GIVE_UP && m_iUserStation[bNextStation] != -1)
			break;
	}
	if (bNextStation == bCurDeskStation)
	{
		return 255;
	}
	return bNextStation;

}
void CServerGameDesk::FinishCompare()                         //��Ϸ����ǰ�ı��ƹ���  
{	
	int winer=GetCardGreater(true);//m_iPerJuTotalNote[i];//
	//��Ϸ����
	GameEndStruct GameEnd;
	::memset(&GameEnd,0,sizeof(GameEnd));
	GameEnd.iUpGradeStation=m_iUpGradePeople;
	//�����ͻ��˵�ǰ����
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if (m_pUserInfo[i]==NULL || m_iUserStation[i] == -1) 
			continue;
		//���ʹ���
		if(m_iUserStation[i]!=TYPE_GIVE_UP)//&&m_iUserCardCount[i]==5)
		{
			GameEnd.iUserState[i] = m_iUserStation[i];
			GameEnd.iCardShape[i]=m_Logic.GetCardShape(m_iUserCard[i],m_iUserCardCount[i]);
		}
		else if(i!=winer)
			GameEnd.iCardShape[i]=0;
		else 
			GameEnd.iCardShape[i]=10;
	}

	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if (m_pUserInfo[i]==NULL) 
			continue;
		if(i==winer)
		{
			GameEnd.iTurePoint[i]=m_iTotalNote-m_iPerJuTotalNote[i]; 

		}
		else
			GameEnd.iTurePoint[i]=-m_iPerJuTotalNote[i];				//�������Ѻ��ע


	}



	//��������

	for (int i=0;i<PLAY_COUNT;i++) 
	{
		if (m_pUserInfo[i]!=NULL) 
			SendGameData(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_FINISH_COMPARE,0);
	}
	SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_FINISH_COMPARE,0);


}
//---------------------------�й���ع���--------------------

void CServerGameDesk::InitAIData()
{
	memset(m_arybCutUser,0,sizeof(m_arybCutUser));

}
bool CServerGameDesk::OnBeginUserNetCutAI(BYTE bDeskStation)  //���ش����йܺ���
{
	if (bDeskStation >= PLAY_COUNT)
	{
		return true;
	}
	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return true;
	}
	if (m_arybCutUser[bDeskStation])
	{
		return true;
	}
	m_arybCutUser[bDeskStation] = true;
	if (m_iOutCardPeople == bDeskStation)  //��ǰ���ߵ�Ϊ������
	{
		ExcutNetCutAI();   //ִ�в���
	}
	return true;

}
void CServerGameDesk::ExcutNetCutAI() //ִ���йܲ���
{
	if (m_iOutCardPeople >= PLAY_COUNT)
	{
		return;
	}
	if (m_arybCutUser[m_iOutCardPeople])
	{
		UserActiveResult(m_iOutCardPeople,TYPE_GIVE_UP,0);
	}

}


//-----------------------------------------------------------------------------
//--
BOOL CServerGameDesk::JudgeLoser()
{
	try
	{
		CString s = CINIFile::GetAppPath ();/////����·��

		TCHAR skin[MAX_NAME_INFO];
		CString temp = _T("");
		temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));

		CINIFile f(temp);
		CString key = TEXT("loser");
		int count = f.GetKeyVal(key,_T("count"),0);
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
				wsprintf(szName,_T("nickname%d"),i);
				szNickName = f.GetKeyVal(key,szName,_T("wing"));
				//�����Ƿ�ʹ��Unicode�Ĳ�ͬ���ò�ͬ�ĺ����ӿ�
				//				if(strcmp(m_pUserInfo[j]->m_UserData.nickName,szNickName) != 0)	continue;
				if(strcmp(m_pUserInfo[j]->m_UserData.szName,szNickName) != 0)	continue;

				//����ʤ��(��ֻ����һ��)
				wsprintf(szName,_T("prob%d"),i);
				probability = f.GetKeyVal(key,szName,0);
				bFind = j;
				break;
			}

		}
		//��ǰ������޸���
		if(bFind == 255)
			return FALSE;

		BYTE bDest = GetLoser();
		if((rand() % 100) + 1 <= probability)	//һ������
		{
			if(bFind != bDest)
				ChangeCard(bFind,bDest);
		}
		//else								//һ����ʤ ��ʤ������ʤ���ܿ���
		//{
		//	bDest = GetIndex(1);
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


//
//�ж�ʤ��
//
BOOL CServerGameDesk::JudgeWiner()
{
	try
	{
		CString s = CINIFile::GetAppPath ();/////����·��

		TCHAR skin[MAX_NAME_INFO];
		CString temp = _T("");
		temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));

		CINIFile f(temp);
		CString key = TEXT("winer");
		int count = f.GetKeyVal(key,_T("count"),0);

		TCHAR szName[21];
		BYTE  bFind = 255;				//ʤ������
		int probability = 0;
		CString szNickName;

		//		OutputDebugString("jgkout:����ʤ��...");
		CString str;
		str.Format("jgk:AIBalanceMoney = %d,AIWinProb[0] = %d,AIWinProb[1] = %d",m_dwAIBalanceMoney,m_iAIWinProb[0],m_iAIWinProb[1]);
		OutputDebugString(str);

		for (int j = 0; j < PLAY_COUNT; j++)
		{
			if(m_pUserInfo[j] == NULL)
				continue;

			//�������ǻ�����
			if (m_dwAIBalanceMoney > 0 && m_pUserInfo[j]->m_UserData.isVirtual)
			{
				//�����趨�Ľ�Ҿ���ֵ�������ʵ�ʵĽ��ֵ���бȽϣ����ݲ���Ĵ�СΪ�������ƶ�ʤ�ʿ���
				if (m_pUserInfo[j]->m_UserData.i64Money >= m_dwAIBalanceMoney)
					probability = m_iAIWinProb[0];
				else
					probability = m_iAIWinProb[1];

				bFind = j;
				break;
			}

			if (count <= 0)
				continue;	//��ʤ��

			//������������
			for(int i = 0; i < count; i ++)
			{
				wsprintf(szName,TEXT("nickname%d"),i);
				szNickName = f.GetKeyVal(key,szName,_T("wing"));
				//�����Ƿ�ʹ��Unicode�Ĳ�ͬ���ò�ͬ�ĺ����ӿ�
				if(strcmp(m_pUserInfo[j]->m_UserData.szName,szNickName) != 0)	continue;

				//����ʤ��(��ֻ����һ��)
				wsprintf(szName,_T("prob%d"),i);
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

		if((rand() % 100) + 1 <= probability)//����ʤ��
		{
			BYTE bDest = GetWiner();
			if(bFind != bDest)
				ChangeCard(bFind,bDest);

			SendGameData(bFind,MDM_GM_GAME_NOTIFY,ASS_AI_WIN,0);
		}
		else   //���ʲ����ϣ�һ��Ҫ��
		{
			BYTE bDest = GetLoser();
			if(bFind != bDest)
				ChangeCard(bFind,bDest);
		}

		return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}
	return FALSE;
}

bool CServerGameDesk::SetUserCard()
{
	// ��ȡ�����ļ�
	CString s = CINIFile::GetAppPath ();/////����·��

	TCHAR skin[MAX_NAME_INFO];
	CString temp = _T("");
	temp.Format("%s%s_s.ini",s,GET_SKIN_FOLDER(skin));

	if (!CINIFile::IsFileExist(temp))
	{
		return false;
	}
	CINIFile f(temp);
	char key[MAX_PATH];
	char name[MAX_PATH];
	// Ϊÿ����Ҷ�ȡ�����ļ�����Ƶ�
	for (int i=0; i<PLAY_COUNT; ++i)
	{
		sprintf(key, "Player%d", i);
		for (int j=0; j<3; ++j)
		{
			sprintf(name, "card%d", j);
			int nTmpCard = f.GetKeyVal(key, name, -1);
			if (nTmpCard>0)
			{
				m_iTotalCard[i*3+j] = nTmpCard;
			}
		}
	}
	return true;
}
//
//����
//
BOOL CServerGameDesk::ChangeCard(BYTE bDestStation,BYTE bWinStation)
{
	for(int i = 0; i < 3; i ++)
	{
		BYTE bTemp = m_iTotalCard[bDestStation * 3 + i];
		m_iTotalCard[bDestStation * 3 + i ] = m_iTotalCard[bWinStation * 3 + i];
		m_iTotalCard[bWinStation * 3 + i] = bTemp;
	}
	return TRUE;
}


BYTE CServerGameDesk::GetWiner()														//�õ�Ӯ��
{
	BYTE Max = 0, Mix = 0;
	for (int i=1; i<PLAY_COUNT; i++)
	{
		if (m_Logic.CompareCard(&m_iTotalCard[Max*3],3,
			&m_iTotalCard[i*3],3)==-1)
			Max = i;

	}

	return Max;

}
BYTE CServerGameDesk::GetLoser()
{
	BYTE Max = 0, Mix = 0;
	for (int i=1; i<PLAY_COUNT; i++)
	{
		if (m_Logic.CompareCard(&m_iTotalCard[Mix*3],3,
			&m_iTotalCard[i*3],3)==1)
			Mix = i;
	}
	return Mix;

}
// ɾ����Ϸ�ڲ���ʱ��
void CServerGameDesk::KillMyTimer(UINT nTimeID)
{
	int nIdx = nTimeID-TIME_SEND_CARD;
	if (nIdx<0 || nIdx>TIMER_COUNT-1)
	{
		return;
	}
	memset(&m_arTimer[nIdx], 0, sizeof(TimerStruct));
}
// ������Ϸ�ڲ���ʱ��
int CServerGameDesk::SetMyTimer(UINT nTimeID, int uElapse)
{
	int nElapse = uElapse/MY_ELAPSE;
	if (nElapse<1)
	{
		nElapse = 1;
	}
	int nIdx = nTimeID-TIME_SEND_CARD;
	if (nIdx<0 || nIdx>TIMER_COUNT-1)
	{
		return -1;
	}
	m_arTimer[nIdx].nId = nTimeID;
	m_arTimer[nIdx].nTriggerCount = nElapse;
	return 0;
}

//add by ljl 
/**
*
*
* @return ������С�����ҵĽ��,������õ���Ӯ����С�����ٽ����ҵĽ�ң����Ƿ�����Ӯ����
*/
__int64 CServerGameDesk::GetMinMoney()
{
	__int64 Min = -1;
	for ( int i=0; i<PLAY_COUNT; i++ )
	{
		
		if ( NULL == m_pUserInfo[i] || m_iUserStation[i] == TYPE_GIVE_UP || m_iUserStation[i] == -1)
		{
			continue;
		}
		if (Min == -1)
		{
			Min = m_pUserInfo[i]->m_UserData.i64Money;
		}
		if ( m_pUserInfo[i]->m_UserData.i64Money < Min )
		{
			Min = m_pUserInfo[i]->m_UserData.i64Money;
		}
	}
	if ( Min > m_iLimitNote )
	{
		Min = m_iLimitNote;
	}
	return Min;
}