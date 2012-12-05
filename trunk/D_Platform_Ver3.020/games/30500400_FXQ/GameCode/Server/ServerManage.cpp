#include "StdAfx.h"
#include "ServerManage.h"
#include "../../../../sdk/include/Common/writelog.h"
#include <math.h>

//��Ϸ��չ����
BOOL CServerGameDesk:: m_bTurnRule = TRUE;		//˳ʱ�뻹����ʱ��
BOOL CServerGameDesk:: m_bIskinescope = FALSE;


void DebugPrintf(int uRoomID,const char *p, ...)
{
	//return;	// ����ʱ�����κ���	By Zxd 20090727
	char szFilename[256];
	CString strPath = CINIFile::GetAppPath() +"\\GameLog\\";

	SHCreateDirectoryEx(NULL, strPath, NULL);

	wsprintf( szFilename, "%s%d_%d_Server.txt",strPath ,  NAME_ID,uRoomID);
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
void MsgPrint(const char *lpszFormat, ...)
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

//���캯��
CServerGameDesk::CServerGameDesk(void):CGameDesk(FULL_BEGIN)
{
	m_iTimerCount = 0; 
	m_iGameBasePoint =  1 ; 
	m_iLimitPlayGame = 0; 
	m_iBeenPlayGame = 0 ; 
	m_cbOperater = 255 ;

	m_cbDiceValue = 0 ;

	m_cbWinPlayer = 255 ; 

	m_cbPropDiceValue = 0 ;
	
	m_bFlying = false ;

	m_bUserProp = false ; 

	memset(m_bPlayerReady , 0 , sizeof(m_bPlayerReady)) ; 

	memset(m_bAuto , 0 , sizeof(m_bAuto)) ; 

	memset(m_bUserNetCut , 0 , sizeof(m_bUserNetCut)) ; 

	memset(m_bDiceAniFinish , false , sizeof(m_bDiceAniFinish)) ; 

	memset(m_cbPropDiceCount , 0 , sizeof(m_cbPropDiceCount)) ; 

	memset(&m_ActiveData , 0 ,sizeof(m_ActiveData)) ; 

	m_bGameStation = GS_WAIT_SETGAME;

	LoadStaticIni();
}
//��������
CServerGameDesk::~CServerGameDesk(void)
{
	KillTimer(TIME_MY_TIMER);
}
///���ؾ�̬����������
BOOL CServerGameDesk::LoadStaticIni()
{
	CString strName;
	CString fileName ; 
	CString s = CINIFile::GetAppPath();/////����·��
	strName.Format("%d",NAME_ID);	
	fileName.Format("%s%s_s.ini",s,strName) ; 

	CINIFile f( fileName );
	CString key ="game" ; 

	m_bTurnRule      = f.GetKeyVal(key,"TurnRule", 0 );
	m_bIskinescope   = f.GetKeyVal(key, "Kinesscope" , 0) ; 

	return  true  ;
}

//���ؾ�������
BOOL CServerGameDesk::LoadExtIni()
{
	CString strName;
	CString fileName ; 
	strName.Format("%d",NAME_ID);

	CString s = CINIFile::GetAppPath();/////����·��
	fileName.Format("%s%s_s.ini",s,strName) ; 

	CINIFile f( fileName );
	CString key ="game" ; 
	m_iDiceTime  = f.GetKeyVal(key , "DiceTime" , 10) ; 
	m_iBeginTime     = f.GetKeyVal(key,"BeginTime", 10);
	m_iThinkTime     = f.GetKeyVal(key,"ThinkTime", 15);

	m_iGameBasePoint = f.GetKeyVal(key , "GameBasePoint" , 1) ; 

	return TRUE;
}
//���ݷ���ID���������ļ�
BOOL CServerGameDesk::LoadExtIni(int iRoomID)
{
	CString strName;
	CString fileName ; 
	strName.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath();/////����·��

	fileName.Format("%s%s_s.ini",s,strName) ; 

	CINIFile f( fileName );
	CString key ;
	key.Format("%d_%d",NAME_ID,iRoomID) ; 

	m_iBeginTime     = f.GetKeyVal(key,"BeginTime", m_iBeginTime);
	m_iDiceTime      = f.GetKeyVal(key , "DiceTime" , m_iDiceTime) ;      
	m_iThinkTime     = f.GetKeyVal(key,"ThinkTime", m_iThinkTime);

	m_iGameBasePoint = f.GetKeyVal(key , "GameBasePoint" , m_iGameBasePoint) ; 

	return TRUE;
}
///���������ļ�����
bool CServerGameDesk::InitDeskGameStation()
{
	LoadExtIni();
	LoadExtIni(m_pDataManage->m_InitData.uRoomID);
	return true;
}
//�����м�ʱ��
void CServerGameDesk::KillAllTimer()
{
	memset(m_arTimer, 0, sizeof(m_arTimer));
}
///��Ϸ��ʼǰ��ʼһЩ����
bool CServerGameDesk::IniBeginData()
{
	m_bFlying = false ;  

	m_bUserProp = false ; 

	m_cbDiceValue = 0 ;

	m_cbPropDiceValue = 0 ;

	m_cbWinPlayer = 255 ;  

	for(int i = 0 ; i<PLAY_COUNT ; i++)
	{
		m_cbPropDiceCount[i] = 1 ;
	}

	m_cbPropDiceValue = 0 ;

	memset(m_bPlayerReady , 0 , sizeof(m_bPlayerReady)) ; 

	memset(m_bUserNetCut , 0 , sizeof(m_bUserNetCut)) ; 

	memset(m_bAuto,0,sizeof(m_bAuto)) ; 

	memset(m_bDiceAniFinish , false , sizeof(m_bDiceAniFinish)) ; 

	memset(&m_ActiveData , 0 ,sizeof(m_ActiveData)) ; 

	m_Logic.InitServerData() ; 

	return true ; 
}

//������Ϸ״̬
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	KillAllTimer();

	m_bFlying = false ; 

	return true;
}

//��Ϸ��ʼ
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag)==false) 
	{
		return false;
	}

	IniBeginData() ; 

	SetTimer(TIME_MY_TIMER, MY_ELAPSE);

	//��������
	m_bGameStation = GS_PLAY_GAME;

	///���һ����Ͷɫ��
	srand( (unsigned)time(NULL) + 3333) ; 
	m_cbOperater = rand()%PLAY_COUNT ;

	BeginUpgradeStruct BeginData  ;
	memset(&BeginData , 0 , sizeof(BeginData)) ;

	BeginData.iBeenPlayGame = m_iBeenPlayGame ; 
	BeginData.iPlayLimit  = m_iLimitPlayGame ;

	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		SendGameDataSaveFile(i,&BeginData,sizeof(BeginData),MDM_GM_GAME_NOTIFY,ASS_GAME_BEGIN,0);
	}
	SendWatchData(m_bMaxPeople,&BeginData,sizeof(BeginData),MDM_GM_GAME_NOTIFY,ASS_GAME_BEGIN,0);

	SetMyTimer(TIME_APPLY_DICE , m_iDiceTime *1000) ;

	OnNotifyClentApplyDice(m_cbOperater) ; 

	return TRUE;
}

//�������ǿ����Ϣf
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	if (bDeskStation>=PLAY_COUNT)
	{
		return true;
	}

	bool  bForceQuitAsAuto = true ; 
	
	ASSERT(pNetHead->bMainID==MDM_GM_GAME_FRAME);
	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_FORCE_QUIT:
		{
			///�����ϲ�·��
			CString szAppPath = CINIFile::GetAppPath();

			CString strBCFFile  ; 
			strBCFFile.Format("%s\\SpecialRule.bcf",szAppPath) ; 

			CBcfFile File(strBCFFile) ;

			if (File.IsFileExist(strBCFFile))
			{
				bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto","30500400" ,1) ; 
			}

			if(bForceQuitAsAuto == true)
			{
				return true ; 
			}

			break;
		}
		break;
	default:
		break;
	}
	return __super::HandleFrameMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}


//��Ϸ���ݰ�������
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	if (bDeskStation>=PLAY_COUNT)
	{
		return true;
	}	

	switch (pNetHead->bAssistantID)
	{
	case ASS_SUPER_MSG://�����ͻ���
		{		
			return true;
		}
	case ASS_GM_AGREE_GAME:		//�û�ͬ����Ϸ
		{
			if (bWatchUser) 
			{
				return true;
			}
			if(m_bGameStation != GS_WAIT_ARGEE)
			{
				m_bGameStation = GS_WAIT_ARGEE;	
			}

			m_bPlayerReady[bDeskStation] = true ; 

			return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
		}	
	case  ASS_APPLY_DICE:
		{
			if(bWatchUser)
			{
				return true ; 
			}

			OnCSNotifyDiceMessage(bDeskStation) ; 
			return true ; 
		}
	case  ASS_NOTIFY_DICE_FINISH:
		{
			if(bWatchUser)
			{
				return true ; 
			}

			if(bDeskStation != m_cbOperater)
			{
				return true ; 
			}

			KillMyTimer(TIME_PLAYER_DICE) ; 

			if(!CheckPlaneCanFly(m_cbOperater))
			{
				m_cbOperater = GetNextDeskStation(m_cbOperater) ; 
				OnNotifyClentApplyDice(m_cbOperater) ; 

				if(m_bUserNetCut[m_cbOperater] == true)
				{
					SetMyTimer(TIME_APPLY_DICE , 1000) ; 
				}
				else
				{
					SetMyTimer(TIME_APPLY_DICE , m_iDiceTime*1000) ; 
				}

			}
			return true ; 
		}
	case ASS_PLAYER_CHOISE_PLANE:
		{
			if(bWatchUser)
			{
				return true ; 
			}
			if(uSize != sizeof(PlayerChoiseStruct))
			{
				return false ; 
			}
			PlayerChoiseStruct *pChoiseData = (PlayerChoiseStruct *)pData ; 

			OnCSNotifyFlyMessage(bDeskStation ,pChoiseData->inDexPlane ) ; 

			return true ; 
		}
	case  ASS_DICE_PROP:
		{
			if(bWatchUser)
			{
				return true ; 
			}
			if(uSize != sizeof(PropDiceStruct))
			{
				return false ; 
			}

			PropDiceStruct *pDiceData = (PropDiceStruct *)pData ; 

			OnCSPropDiceMessage(bDeskStation , pDiceData->cbIndex) ;

			return true ; 
		}
	case  ASS_PLANE_FLY_FINISH:
		{
			if(bWatchUser)
			{
				return true ; 
			}

			OnUserAniFinish(bDeskStation) ; 
			return true ; 
		}
	case ASS_AUTO:			//�й�
		{
			if ((bWatchUser)||(uSize!=sizeof(AutoStruct))) 
				return FALSE;
			AutoStruct * pAuto = (AutoStruct*)pData;
			UserSetAuto(bDeskStation,pAuto->bAuto);
			return true;
		}
	default: 
		{
			break;
		}
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}
///֪ͨ��Ҵ�ɫ�ӽ���
bool CServerGameDesk::OnNotifyClentApplyDice(BYTE bDeskStation )
{
	m_bFlying = false ; 

	m_bUserProp = false ; 

	for(int i = 0 ; i<PLAY_COUNT ; i++) 
	{
		if(true == m_Logic.CheckGameFinish(i))  ///���������жϵ�(�˴���ҵĶ�������)
		{
			m_cbWinPlayer = i ;
			GameFinish(i ,GF_NORMAL) ; 
			return true ; 
		}
	}

	memset(m_bDiceAniFinish , false ,sizeof(m_bDiceAniFinish)) ; 

	WaitDiceStruct uData ; 
	uData.cbDeskStation = bDeskStation ; 

	for(int  i = 0 ; i<PLAY_COUNT ; i++)
	{
		if(NULL != m_pUserInfo[i])
		{
			SendGameDataSaveFile(i , &uData , sizeof(uData) , MDM_GM_GAME_NOTIFY , ASS_APPLY_DICE ,0 ) ; 
		}
	}
	SendWatchData(m_bMaxPeople, &uData , sizeof(uData) , MDM_GM_GAME_NOTIFY , ASS_APPLY_DICE ,0 ) ; 
	return true ; 
}
///��ҵ���ɫ��
bool CServerGameDesk::OnCSPropDiceMessage(BYTE  bDeskStation , BYTE cbDicevalue) 
{
	if(bDeskStation != m_cbOperater)
	{
		return true ; 
	}

	if(cbDicevalue <1 ||cbDicevalue >6)
	{
		return true ; 
	}
#ifdef  TEST_VERSION
	m_cbPropDiceCount[bDeskStation] = 2; ///���԰���Զ�е���
#else 
	if(m_cbPropDiceCount[bDeskStation]<1)
	{
		return true ; 
	}
	
#endif

	m_bUserProp = true ; 

	m_cbPropDiceValue = cbDicevalue ; 

	m_cbPropDiceCount[bDeskStation] --  ; 

	OnCSNotifyDiceMessage(bDeskStation) ;

	return true ; 

}
///����ͻ��˷���������Ϣ,֪ͨ��Ҵ�ɫ��
bool  CServerGameDesk::OnCSNotifyDiceMessage(BYTE bDeskStation) 
{
	KillMyTimer(TIME_APPLY_DICE) ; 

	if(m_bGameStation != GS_PLAY_GAME)
	{
		return true ; 
	}
	if(bDeskStation !=  m_cbOperater)
	{
		return true ; 
	}

	srand((unsigned)time(NULL) + 1111) ;
	m_cbDiceValue = rand()%6 + 1 ; 

	if(m_cbPropDiceValue >0 &&m_cbPropDiceValue <7 )
	{
		m_cbDiceValue = m_cbPropDiceValue ; 

		m_cbPropDiceValue = 0 ; 
	}

	NotifyDiceStruct NotifyData ;
	NotifyData.cbDeskStation = bDeskStation ; 
	NotifyData.cbDiceValue = m_cbDiceValue ; 
	NotifyData.cbPropCount = m_cbPropDiceCount[bDeskStation] ; 

	for(int i = 0 ; i<PLAY_COUNT ; i++)
	{
		if(NULL != m_pUserInfo[i])
		{
			SendGameDataSaveFile( i ,&NotifyData,sizeof(NotifyData),MDM_GM_GAME_NOTIFY,ASS_NOTIFY_DICE ,0) ; 
		}
	}

	SetMyTimer(TIME_PLAYER_DICE,3000) ; 

	return true ; 
}
///����ͻ��˷����ķɻ������Ϣ
bool CServerGameDesk::OnCSNotifyFlyMessage(BYTE bDeskStation,int inDex)
{
	if(bDeskStation != m_cbOperater)
	{
		DebugPrintf(m_pDataManage->m_InitData.uRoomID , "������ȷ���˴�������Ϣ") ;

		return true ; 
	}
	if(0 >inDex ||inDex>=PLAYER_PLANE_NUM )
	{
		DebugPrintf(m_pDataManage->m_InitData.uRoomID , "û���ҵ���ȷ�ķɻ���") ;

		return true  ; 
	}

	if( !m_Logic.IsCanMove(bDeskStation , inDex , m_cbDiceValue))  //�����Ϸ����߼�
	{
		return true ; 
	}

	if(m_bFlying)   ///��ֹ�ͻ��˶�ε��
	{
		return true ; 
	}

	m_bFlying = true ; 

	KillMyTimer(TIME_PLANE_FLY) ;

	memset(&m_ActiveData , 0 ,sizeof(m_ActiveData)) ; 

	///����˸��ݵ�ǰ�ĵ�������
	int  bActivePlane = m_Logic.PlayerPlaneFly(bDeskStation ,m_cbDiceValue ,inDex ,m_ActiveData.PlaneActive) ;

	MsgPrint("wysoutfxqinfo::��ҵķɻ���ʼ���� ���еĵ���==%d " ,m_ActiveData.PlaneActive[0].cbStepCount); 

	PlayerActiveStruct uActiveData ; 

	uActiveData = m_ActiveData ; 

	uActiveData.iPlaneCount = bActivePlane ;  

	uActiveData.cbDeskStation = bDeskStation ; 

	uActiveData.cbDiceValue = m_cbDiceValue ; 

	memcpy(uActiveData.iPlayerfight , m_Logic.m_iPlayerfight , sizeof(m_Logic.m_iPlayerfight)) ; 
	

	if(0 == bActivePlane)
	{
		DebugPrintf(m_pDataManage->m_InitData.uRoomID ,"��Ϸ���������� ,һ���ɻ���û��");
	}

	for(int i = 0 ; i<PLAY_COUNT ; i++)
	{
		if(NULL != m_pUserInfo[i])
		{
			continue; 
		}
		SendGameDataSaveFile(i, &uActiveData ,sizeof(uActiveData) , MDM_GM_GAME_NOTIFY ,ASS_PLANE_FLY_ANI ,0) ; 
	}

	SendGameDataSaveFile(m_bMaxPeople , &uActiveData ,sizeof(uActiveData) , MDM_GM_GAME_NOTIFY ,ASS_PLANE_FLY_ANI ,0) ;


	SetMyTimer(TIME_PLANE_FLY_INTEVAL ,15*1000) ;
	

	return true ; 
}

///��Ҷ����������
bool CServerGameDesk::OnUserAniFinish(BYTE bDeskStation )
{
	m_bDiceAniFinish[bDeskStation] = true ; 
	int  iAniFinishCount = 0 ; 
	for(int i = 0 ; i <PLAY_COUNT ; i++)
	{
		if(m_bDiceAniFinish[i] == true||m_bUserNetCut[i])
		{
			iAniFinishCount ++ ; 
		}
	}

	if(iAniFinishCount  == PLAY_COUNT)
	{
		KillMyTimer(TIME_PLANE_FLY_INTEVAL) ;


		ScenceChangeStruct pScenceData ;

		for(int i = 0 ; i<PLAY_COUNT ; i++)
		{
			for(int j = 0 ;  j <PLAYER_PLANE_NUM ; j++)
			{
				pScenceData.PlaneData[i][j] = m_Logic.m_PlayerSerInfo[i].Plane[j] ; 
			}
		}

		for(int i = 0 ; i<PLAY_COUNT ; i++)
		{
			SendGameDataSaveFile(i , &pScenceData ,sizeof(pScenceData) ,MDM_GM_GAME_NOTIFY ,ASS_PLANE_FLY_FINISH ,0) ; 
		}

		SendWatchData(m_bMaxPeople ,&pScenceData ,sizeof(pScenceData) ,MDM_GM_GAME_NOTIFY ,ASS_PLANE_FLY_FINISH ,0) ; 

		if(m_cbDiceValue != 6 ||true == m_bUserProp)
		{
			m_cbOperater = GetNextDeskStation(m_cbOperater) ; 
		}

		OnNotifyClentApplyDice(m_cbOperater) ; 

		if(m_bUserNetCut[m_cbOperater] == true)
		{
			SetMyTimer(TIME_APPLY_DICE , 1000) ; 
		}
		else
		{
			SetMyTimer(TIME_APPLY_DICE , m_iDiceTime*1000) ; 
		}
	}

	return true ; 
}
//�й�����
bool CServerGameDesk::UserSetAuto(BYTE bDeskStation, bool bAutoCard)
{
	if (bDeskStation>=PLAY_COUNT)
	{
		return false;
	}
	m_bAuto[bDeskStation] = bAutoCard;

	AutoStruct autoset;
	autoset.bAuto = bAutoCard;
	autoset.bDeskStation = bDeskStation;

	for(int i = 0; i < PLAY_COUNT; i ++)
		SendGameDataSaveFile(i,&autoset,sizeof(autoset),MDM_GM_GAME_NOTIFY,ASS_AUTO,0);
	SendWatchData(m_bMaxPeople,&autoset,sizeof(autoset),MDM_GM_GAME_NOTIFY,ASS_AUTO,0);
	return true;
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
///ɾ���������Ķ�ʱ��
void CServerGameDesk::KillAllMyTimer()
{
	for(int i = 0 ; i<TIMER_COUNT ; i++)
	{
		KillMyTimer(i +TIME_SEND_CARD) ;
	}

	return ; 
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
		DebugPrintf(m_pDataManage->m_InitData.uRoomID,"��ʱ��[%d]�������ɹ�", nTimeID);
		return -1;
	}

	char mess[200];
	wsprintf (mess, "SetMyTimer  m_arTimer ����=%d ʵ��ֵ%d nTimeID=%d",17,nIdx,nTimeID);
	SrvTestlog(mess);

	m_arTimer[nIdx].nId = nTimeID;
	m_arTimer[nIdx].nTriggerCount = nElapse;
	return 0;
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
	switch(uTimerID)
	{
	case TIME_APPLY_DICE:
		{
			KillMyTimer(TIME_APPLY_DICE) ; 
			OnCSNotifyDiceMessage(m_cbOperater) ;  
			break;
		}
	case  TIME_PLAYER_DICE:
		{
			KillMyTimer(TIME_PLAYER_DICE) ; 

			if(!CheckPlaneCanFly(m_cbOperater))
			{
				m_cbOperater = GetNextDeskStation(m_cbOperater) ; 
				OnNotifyClentApplyDice(m_cbOperater) ; 

				if(m_bUserNetCut[m_cbOperater] == true)
				{
					SetMyTimer(TIME_APPLY_DICE , 1000) ; 
				}
				else
				{
					SetMyTimer(TIME_APPLY_DICE , m_iDiceTime*1000) ; 
				}

			}
			break;
		}
	case  TIME_PLANE_FLY:
		{
			KillMyTimer(TIME_PLANE_FLY) ; 

			int iPlaneIndex = m_Logic.FindOnePlaneToFly(m_cbOperater ,m_cbDiceValue) ; 

			MsgPrint("wysoutfxqinfo::iPlaneIndex==%d ,m_cbDiceValue==%d ,m_cbOperater==%d" ,iPlaneIndex , m_cbDiceValue ,m_cbOperater) ;

			OnCSNotifyFlyMessage(m_cbOperater ,iPlaneIndex ) ; 

			break;
		}
	case TIME_PLANE_FLY_INTEVAL:
		{
			KillMyTimer(TIME_PLANE_FLY_INTEVAL) ;


			ScenceChangeStruct pScenceData ;

			for(int i = 0 ; i<PLAY_COUNT ; i++)
			{
				for(int j = 0 ;  j <PLAYER_PLANE_NUM ; j++)
				{
					pScenceData.PlaneData[i][j] = m_Logic.m_PlayerSerInfo[i].Plane[j] ; 
				}
			}

			for(int i = 0 ; i<PLAY_COUNT ; i++)
			{
				SendGameDataSaveFile(i , &pScenceData ,sizeof(pScenceData) ,MDM_GM_GAME_NOTIFY ,ASS_PLANE_FLY_FINISH ,0) ; 
			}

			SendWatchData(m_bMaxPeople ,&pScenceData ,sizeof(pScenceData) ,MDM_GM_GAME_NOTIFY ,ASS_PLANE_FLY_FINISH ,0) ; 

			if(m_cbDiceValue != 6 ||true == m_bUserProp)
			{
				m_cbOperater = GetNextDeskStation(m_cbOperater) ; 
			}

			OnNotifyClentApplyDice(m_cbOperater) ;
			if(m_bUserNetCut[m_cbOperater] == true)
			{
				SetMyTimer(TIME_APPLY_DICE , 1000) ; 
			}
			else
			{
				SetMyTimer(TIME_APPLY_DICE , m_iDiceTime*1000) ; 
			}
			break;
		}
	default:
		{
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
			GameBaseStation GameStation ;
			::memset(&GameStation,0,sizeof(GameStation)) ; 

			GameStation.iVersion1 = DEV_HEIGHT_VERSION ; 
			GameStation.iVersion2 = DEV_LOW_VERSION ;

			memcpy(GameStation.bUserReady ,m_bPlayerReady , sizeof(m_bPlayerReady)) ;
			memcpy(GameStation.bUserAuto , m_bAuto , sizeof(m_bAuto)) ; 

			GameStation.cbBeginTime = m_iBeginTime ; 
			GameStation.cbDiceTime  = m_iDiceTime ; 
			GameStation.cbThinkTime = m_iThinkTime ; 

			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));

			return TRUE;
		}
	case GS_SEND_CARD:		//����״̬
		{		
			return TRUE;
		}
	case GS_PLAY_GAME:	//��Ϸ��״̬
		{
			PlayingGameStation GameStation;		
			::memset(&GameStation,0,sizeof(GameStation));

			GameStation.cbBeginTime = m_iBeginTime ; 
			GameStation.cbDiceTime  = m_iDiceTime ; 
			GameStation.cbThinkTime = m_iThinkTime ; 

			GameStation.iVersion1 = DEV_HEIGHT_VERSION ; 
			GameStation.iVersion2 = DEV_LOW_VERSION ; 

			GameStation.cbOperater = m_cbOperater ; 

			memcpy(GameStation.bUserAuto , m_bAuto , sizeof(m_bAuto)) ; 

			memcpy(GameStation.cbPropCount , m_cbPropDiceCount , sizeof(GameStation.cbPropCount)) ; 
			memcpy(GameStation.iPlayerfight , m_Logic.m_iPlayerfight , sizeof(GameStation.iPlayerfight)) ; 

			for(int i = 0 ; i<PLAY_COUNT ; i++)
			{
				for(int j = 0 ;  j <PLAYER_PLANE_NUM ; j++)
				{
					GameStation.PlaneData[i][j] = m_Logic.m_PlayerSerInfo[i].Plane[j] ; 
				}
			}

			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{		
			WaitNextGameStation GameStation;
			::memset(&GameStation,0,sizeof(GameStation));

			GameStation.iVersion1 = DEV_HEIGHT_VERSION ; 
			GameStation.iVersion2 = DEV_LOW_VERSION ; 

			GameStation.cbBeginTime = m_iBeginTime ; 
			GameStation.cbDiceTime  = m_iDiceTime ; 
			GameStation.cbThinkTime = m_iThinkTime ; 

			SendGameStation(bDeskStation,uSocketID,bWatchUser,&GameStation,sizeof(GameStation));
			return TRUE;
		}

	default:
		break;
	}
	return false;
}

//��ȡ��һ�����λ��
BYTE CServerGameDesk::GetNextDeskStation(BYTE bDeskStation)
{
	if (bDeskStation>=PLAY_COUNT)
	{
		return 0;
	}
	if(FALSE == m_bTurnRule )//˳ʱ��
	{
		return (bDeskStation + 1 ) % PLAY_COUNT;
	}

	return 	(bDeskStation + (PLAY_COUNT - 1)) % PLAY_COUNT;
}
///���ɻ����
bool  CServerGameDesk::CheckPlaneCanFly(BYTE bDeskStation)
{
	int  iActiveCount  = m_Logic.FindActivePlaneCount(bDeskStation) ;

	int  iFinishCount = m_Logic.FindPlaneFinishCount(bDeskStation) ; 

	MsgPrint("wysoutfxqinfo::iActiveCount ===%d , iFinishCount==%d ,m_cbDiceValue==%d" , iActiveCount , iFinishCount ,m_cbDiceValue) ;

	if(m_cbDiceValue == 6)  ///��ǰ�����ӵ���Ϊ6�Ļ�
	{
		if(iFinishCount == PLAYER_PLANE_NUM - 1)  ///�Ѿ������ܷɻ���ɷ�������
		{
			SetMyTimer(TIME_PLANE_FLY ,20 ) ;
		}
		else
		{
			if(true == m_bUserNetCut[bDeskStation])
			{
				SetMyTimer(TIME_PLANE_FLY , 20);
			}
			else
			{
				SetMyTimer(TIME_PLANE_FLY , (m_iThinkTime + 3)*1000);
			}
			

			NotifyChoisePlane   pChoiseData ; 
			pChoiseData.cbDeskStation = bDeskStation ; 

			SendGameDataSaveFile(bDeskStation ,&pChoiseData , sizeof(pChoiseData),MDM_GM_GAME_NOTIFY ,ASS_NOTIFY_CHOISE_PLANE ,0) ;
		}
		return true ; 
	}

	if(iActiveCount >0)
	{
		if(iActiveCount  == 1 )  ///ֻ��һ�ܵ�ʱ��ֱ�����
		{
			SetMyTimer(TIME_PLANE_FLY , 20) ;
		}
		else ///��Ҫ��ҵ����һ���ɻ���ɲ���
		{
			if(true == m_bUserNetCut[bDeskStation])
			{
				SetMyTimer(TIME_PLANE_FLY , 20);
			}
			else
			{
				SetMyTimer(TIME_PLANE_FLY , (m_iThinkTime + 3)*1000);
			}

			NotifyChoisePlane   pChoiseData ; 
			pChoiseData.cbDeskStation = bDeskStation ; 

			for(int i = 0 ; i<PLAY_COUNT ; i++)
			{
				SendGameDataSaveFile(bDeskStation ,&pChoiseData , sizeof(pChoiseData),MDM_GM_GAME_NOTIFY ,ASS_NOTIFY_CHOISE_PLANE ,0) ;
			}
			SendWatchData(m_bMaxPeople ,&pChoiseData , sizeof(pChoiseData),MDM_GM_GAME_NOTIFY ,ASS_NOTIFY_CHOISE_PLANE ,0) ;
		}
		return true ; 
	}

	return false ;  //�¸���Ҽ�����ɫ��
}
//��Ϸ����
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	if (bDeskStation>=PLAY_COUNT)
	{
		return TRUE;
	}	
	KillAllMyTimer() ; 
	KillTimer(TIME_MY_TIMER);
	//��д����
	switch (bCloseFlag)
	{
	case GF_NORMAL:		//��Ϸ��������
		{
			//DebugPrintf(m_pDataManage->m_InitData.uRoomID,"��Ϸ����");
			//�������� 
			m_bGameStation = GS_WAIT_ARGEE;
			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				if (m_pUserInfo[i] != NULL) 
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING;
			}

			//��Ϸ����
			GameEndStruct GameEnd;
			::memset(&GameEnd,0,sizeof(GameEnd));

			///��ȡ��Ϸ�еı���
			int  iPoint = 0; 

			if(m_Logic.m_iPlayerfight[m_cbWinPlayer%2] >m_Logic.m_iPlayerfight[(m_cbWinPlayer+1)%2])
			{
				iPoint = 3 ; 
			}
			else if(m_Logic.m_iPlayerfight[m_cbWinPlayer%2] == m_Logic.m_iPlayerfight[(m_cbWinPlayer+1)%2])
			{
				iPoint = 2 ; 
			}
			else
			{
				iPoint = 1 ; 
			}

			for(int i = 0 ; i<PLAY_COUNT ; i ++ )
			{
				if(i == m_cbWinPlayer || i == (m_cbWinPlayer + 2 )%PLAY_COUNT)
				{
					GameEnd.iTurePoint[i] = m_iGameBasePoint *iPoint  ; 
				}
				else
				{
					GameEnd.iTurePoint[i] =-m_iGameBasePoint *iPoint ; 
				}
			}

			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));

			ChangeUserPointint64(GameEnd.iTurePoint,temp_cut);
			__super::RecoderGameInfo(GameEnd.iChangeMoney);

			m_bGameStation = GS_WAIT_ARGEE;

			for (int i = 0; i < PLAY_COUNT; i ++) 
			{
				SendGameDataSaveFile(i,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_NO_CONTINUE_END,0);
			}

			SendWatchData(m_bMaxPeople,&GameEnd,sizeof(GameEnd),MDM_GM_GAME_NOTIFY,ASS_NO_CONTINUE_END,0);

			bCloseFlag = GFF_FORCE_FINISH;
			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);

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
				SendGameDataSaveFile(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_SAFE_END,0);
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
					m_pUserInfo[i]->m_UserData.bUserState = USER_SITTING ;
			}
			GameCutStruct CutEnd;
			::memset(&CutEnd,0,sizeof(CutEnd));
			CutEnd.bDeskStation = bDeskStation;

			int  iArriveCount[PLAY_COUNT] ; 
			memset(iArriveCount , 0 , sizeof(iArriveCount)) ; 

			for(int  i = 0 ; i<PLAY_COUNT ; i++)
			{
				iArriveCount[i] = m_Logic.FindPlaneFinishCount(i) ;
			}

			if(iArriveCount[bDeskStation]+iArriveCount[(bDeskStation + 2)%PLAY_COUNT]>iArriveCount[(bDeskStation + 1)%PLAY_COUNT]+iArriveCount[(bDeskStation + 3)%PLAY_COUNT])
			{
				CutEnd.iTurePoint[bDeskStation] = - 4*(8 -iArriveCount[bDeskStation]-iArriveCount[(bDeskStation + 2)%PLAY_COUNT])*m_iGameBasePoint ;

				CutEnd.iTurePoint[(bDeskStation+2)%PLAY_COUNT] = 2*(8 -iArriveCount[bDeskStation]-iArriveCount[(bDeskStation + 2)%PLAY_COUNT])*m_iGameBasePoint ;
			}
			else
			{
				int iPoint1 = 0 ;  ///���ܱ���
				int iPoint2 = 0 ;  /// δ���ܱ���
				if(m_Logic.m_iPlayerfight[bDeskStation] +m_Logic.m_iPlayerfight[(bDeskStation+2)%PLAY_COUNT]< m_Logic.m_iPlayerfight[(bDeskStation+1)%PLAY_COUNT] +m_Logic.m_iPlayerfight[(bDeskStation+3)%PLAY_COUNT])
				{
					iPoint1 = 6 ; 
					iPoint2 = 3 ;
				}
				else if(m_Logic.m_iPlayerfight[bDeskStation] +m_Logic.m_iPlayerfight[(bDeskStation+2)%PLAY_COUNT]==m_Logic.m_iPlayerfight[(bDeskStation+1)%PLAY_COUNT] +m_Logic.m_iPlayerfight[(bDeskStation+3)%PLAY_COUNT])
				{
					iPoint1 = 4 ; 
					iPoint2 = 2; 
				}
				else
				{
					iPoint1 = 3 ; 
					iPoint2 = 1 ;
				}
				CutEnd.iTurePoint[bDeskStation] = -iPoint1 *8*m_iGameBasePoint ; 
				CutEnd.iTurePoint[(bDeskStation + 1)%PLAY_COUNT] = iPoint2 *m_iGameBasePoint *(8 - iArriveCount[bDeskStation] - iArriveCount[(bDeskStation + 2)%PLAY_COUNT])/2 ;
				CutEnd.iTurePoint[(bDeskStation + 3)%PLAY_COUNT] = iPoint2 *m_iGameBasePoint *(8 - iArriveCount[bDeskStation] - iArriveCount[(bDeskStation + 2)%PLAY_COUNT])/2 ;
			}


			bool temp_cut[PLAY_COUNT];
			memset(temp_cut,0,sizeof(temp_cut));

			for (int i = 0; i < PLAY_COUNT; i ++)
			{
				temp_cut[i] = (bDeskStation == i && CutEnd.iTurePoint[i] < 0);
			}

			ChangeUserPointint64(CutEnd.iTurePoint, temp_cut);
			__super::RecoderGameInfo(CutEnd.iChangeMoney);

			for (int i = 0; i < PLAY_COUNT; i ++)
				SendGameDataSaveFile(i,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);
			SendWatchData(m_bMaxPeople,&CutEnd,sizeof(CutEnd),MDM_GM_GAME_NOTIFY,ASS_CUT_END,0);

			bCloseFlag = GFF_FORCE_FINISH;

			ReSetGameState(bCloseFlag);
			__super::GameFinish(bDeskStation,bCloseFlag);

			return TRUE;
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
//�û�ͬ�⿪ʼ
bool CServerGameDesk::UserAgreeGame(BYTE bDeskStation)
{
	return  __super::UserAgreeGame(bDeskStation);	
}
//�ж��Ƿ�������Ϸ
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if((m_bGameStation >= GS_SEND_CARD && m_bGameStation < GS_WAIT_NEXT))
		return true;
	return false;
}
//�û��뿪��Ϸ��
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	if (bDeskStation>=PLAY_COUNT)
	{
		return 0;
	}	

	m_bPlayerReady[bDeskStation] = false ; 

	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}

//�û������뿪
bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo)
{
	m_bUserNetCut[bDeskStation] = true ; 
	return  __super::UserNetCut(bDeskStation,  pLostUserInfo);
}

//�û���������
bool CServerGameDesk::UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo)
{	
	m_bUserNetCut[bDeskStation] = false ; 
	return __super::UserReCome(bDeskStation,pNewUserInfo);
}

//�û�������Ϸ��
BYTE CServerGameDesk::UserSitDesk(MSG_GR_S_UserSit* pUserSit, CGameUserInfo* pUserInfo)
{
	return __super::UserSitDesk(pUserSit,pUserInfo);
}
/**
* �������ݺ��� ��������Ϣ����Ϸ�ߣ����Ҵ���
*
* @param bDeskStation ���͸��ĸ���ҵ��߼���
*
* @param bMainID���ݵ�����
*
* @param bAssID���ݵ�id
*
* @bSingle����ֵ
*/
bool CServerGameDesk::SendGameDataSaveFile(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode,bool bSingle)
{
	if (m_bIskinescope )  //�Ƿ�¼��
	{
		if ( bMainID == MDM_GM_GAME_NOTIFY ) 
		{
			if ( !bSingle )
			{ 
				NetMessageHead NetHead;
				NetHead.bMainID      = bMainID;
				NetHead.bAssistantID = bAssID;
				m_saveFile.SaveFileMessage(&NetHead,NULL,0);

			}
			else
			{
				if ( bDeskStation == 0  )
				{
					NetMessageHead NetHead;
					NetHead.bMainID      = bMainID;
					NetHead.bAssistantID = bAssID;
					m_saveFile.SaveFileMessage(&NetHead,NULL,0);
				}
			}

		}
	}
	return SendGameData(bDeskStation,bMainID,bAssID,bHandleCode);
}

/**
* �������ݺ��� ��������Ϣ����Ϸ�ߣ����Ҵ���
*
* @param bDeskStation ���͸��ĸ���ҵ��߼���
*
* @param pData���͵����ݰ�
*
* @param uSize���͵�����
*
* @param bMainID���ݵ�����
*
* @param bAssID���ݵ�id
*
* @bSingle����ֵ
*/
bool CServerGameDesk::SendGameDataSaveFile(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode,bool bSingle)
{
	return SendGameData(bDeskStation,pData,uSize,bMainID,bAssID,bHandleCode);
}
///���ƣ�SrvTestlog
///��������־��¼�ӿ�
///@param pMessage ��־��Ϣ
///@return 
void  CServerGameDesk::SrvTestlog(TCHAR *pMessage)
{
	return ;	// ������ȥ����¼
	try
	{
		CString  tempstr = CINIFile::GetAppPath();
		SYSTEMTIME st;
		GetLocalTime(&st);
		TCHAR FileName[256];
		//wsprintf(FileName, TEXT("e:\\syslog%d.txt\0"),GetCurrentProcessId());
		//wsprintf(FileName, TEXT("ddz-L.txt"),tempstr,st.wYear,st.wMonth,st.wDay,st.wHour);
		wsprintf(FileName, TEXT("%d-%d-%d-ddz-L.txt"),st.wMonth,st.wDay,st.wHour);
		CFile syslog;
		syslog.Open(FileName,CFile::modeNoTruncate| CFile::modeCreate |CFile::modeReadWrite);
		TCHAR szDate[256];
		wsprintf(szDate, TEXT("%d-%d-%d %d-%d-%d.%d\t"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
		syslog.SeekToEnd();
		syslog.Write(szDate,lstrlen(szDate));
		syslog.SeekToEnd();
		syslog.Write(pMessage,lstrlen(pMessage));
		TCHAR nextLine[] = TEXT("\r\n");
		syslog.SeekToEnd();
		syslog.Write(nextLine,lstrlen(nextLine));

		syslog.Close();
	}
	catch(CException *e)
	{
		//AfxMessageBox("some exception!");
		e->Delete();
		return ; 
	}
}
