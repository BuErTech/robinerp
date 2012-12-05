#include "stdafx.h"
#include <afxwin.h>
#include <afxext.h>
#include <math.h>
#include "SoDeskLogicImpl.h"
using namespace MyGame;

CDeskLogicImpl::CDeskLogicImpl(void)
{
    m_pGameProcessor   = NULL;  
    m_pUserNanager     = NULL; 
    m_pGameLogerImpl   = NULL;
    m_pGameFactory     = NULL;
    m_pGameSettingImpl = NULL;
    m_pBaseLogic       = NULL;

    for (int i = 0; i < PLAY_COUNT; i++)
    {
        m_SoPlayer[i].Initialize(NULL);
		m_bAgree[i] = FALSE;
		m_bNetCut[i] = FALSE;
    }

	m_bStartMode = FULL_BEGIN;
}

CDeskLogicImpl::~CDeskLogicImpl(void)
{
    if (m_pGameProcessor != NULL)
    {
        delete m_pGameProcessor;
        m_pGameProcessor = NULL;
    }

    if (m_pUserNanager != NULL)
    {
        delete m_pUserNanager; 
        m_pUserNanager = NULL;
    }

    if (m_pGameLogerImpl != NULL)
    {
        delete m_pGameLogerImpl; 
        m_pGameLogerImpl = NULL;
    }

    if (m_pBaseLogic != NULL)
    {
        delete m_pBaseLogic;
        m_pBaseLogic = NULL;
    }
}

// ��ʼ��Ϸ �½����ӵ�ʱ��װ�������ļ�
bool CDeskLogicImpl::InitDeskGameStation()
{
	// �ж���Ϸ������
	if (m_bMaxPeople > PLAY_COUNT) 
	{
		return false;
	}
	
	m_pGameFactory		= new SoGameFactory;
	m_pGameProcessor	= new GameProcessor(this);
	m_pUserNanager		= new SoUserManager(this);
	m_pGameSettingImpl	= new SoGameSettingImpl(this);
	m_pGameLogerImpl	= new SoGameLogerImpl(this);
	m_pBaseLogic		= new CXSoGameLogic();
	m_pBaseLogic->Initialize(m_pGameFactory, 0, m_bDeskIndex);
	m_bPlaying = false;

	// ֪ͨ���������ļ�
	m_pGameSettingImpl->FireEvent(&ISoGameSettingEvent::OnLoadCfg);

	return true;
}

// ��Ϸ��ʼ
bool CDeskLogicImpl::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag) == false)
	{
		GameFinish(0, GFF_SAFE_FINISH);
		return false;
	}

	//m_pGameProcessor->FireEvent(&ISoGameProcessorEvent::OnAllPlayerReady);

	m_nGameStation = gsPlaying;
	m_bPlaying = true;
	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		m_bNetCut[i] = FALSE;
	}

	// ֪ͨ��Ϸ��ʼ
	m_pGameProcessor->FireEvent(&ISoGameProcessorGameEvent::OnGameStart);

	return true;
}

/** 
* @brief ��Ϸ��ʼ
*/
void CDeskLogicImpl::GameBegin()
{

}

// ��Ϸ����
bool CDeskLogicImpl::GameFinish(BYTE bDeskStation,BYTE bCloseFlag)
{
	return __super::GameFinish(bDeskStation, bCloseFlag);
}

// �ж��Ƿ�������Ϸ��
bool CDeskLogicImpl::IsPlayGame(BYTE bDeskStation)
{
	return m_bPlaying;
}

// �ж��Ƿ�������Ϸ��
BOOL CDeskLogicImpl::IsPlayingByGameStation()
{
	return m_bPlaying;
}

// �����Ϣ������
bool CDeskLogicImpl::HandleFrameMessage(BYTE bDeskStation,NetMessageHead* pNetHead,void* pData,UINT uSize,UINT uSocketID,bool bWatchUser) 
{
	switch (pNetHead->bAssistantID) 
	{
	case ASS_GM_GAME_INFO:
		{
			return OnGetGameStation(bDeskStation, uSocketID, bWatchUser);
		}
	case ASS_GM_FORCE_QUIT:
		{
			return true;   // ��������ǿ����Ϣ����
		}
	}
	return __super::HandleFrameMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}

bool CDeskLogicImpl::HandleNotifyMessage(BYTE bDeskStation,NetMessageHead* pNetHead,void* pData,UINT uSize,UINT uSocketID,bool bWatchUser)
{
	if (pNetHead->bAssistantID == ASS_SUB_TRUSTEE) 
	{
		UserTrustee(bDeskStation, (BYTE *)pData, uSize);
	} 
	else if (pNetHead->bAssistantID == ASS_GM_AGREE_GAME/* || pNetHead->bAssistantID == ASS_SUB_USERAGREE*/) 
	{
		UserAgreeGame(bDeskStation);
	}
	else 
	{
		m_pGameProcessor->FireEvent(&ISoGameProcessorEvent::OnGameMessage,
			uSocketID,
			(UINT)m_pUserInfo[bDeskStation]->m_UserData.dwUserID,
			(UINT)bDeskStation,
			(void*)pData,
			(UINT)uSize);
	}

	return true;
}

///�Ƿ���Կ�ʼ��Ϸ
bool CDeskLogicImpl::CanBeginGame()
{
	if (m_bStartMode == FULL_BEGIN)
	{
		return __super::CanBeginGame();
	}
	else
	{
		//�ж��������Ƿ�ͬ��
		BYTE bPeopoleCount = 0;
		for (BYTE i = 0 ; i < m_bMaxPeople; i ++) 
		{
			if (m_pUserInfo[i] != NULL)
			{
				bPeopoleCount ++;
				if (m_pUserInfo[i]->m_UserData.bUserState != USER_ARGEE)
				{
					return false;
				}
			}
		}

		// �ж��ʺϿ�ʼ����
		if (bPeopoleCount > 1L)
		{
			if (m_bStartMode == SYMMETRY_ARGEE)
			{
				if ((bPeopoleCount % 2) != 0) 
				{
					return false;
				}

				if (bPeopoleCount == m_bMaxPeople)
				{
					return true;
				}

				BYTE bHalfCount = m_bMaxPeople/2;

				for (BYTE i = 0; i < bHalfCount; i ++)
				{
					if ((m_pUserInfo[i] == NULL)&&(m_pUserInfo[i + bHalfCount] != NULL)) 
					{
						return false;
					}

					if ((m_pUserInfo[i] != NULL)&&(m_pUserInfo[i + bHalfCount] == NULL)) 
					{
						return false;
					}
				}

				return true;
			}
			else 
			{
				if (m_bStartMode == FULL_BEGIN) 
				{
					return (bPeopoleCount == m_bMaxPeople);
				}
				else if (m_bStartMode == ALL_ARGEE) 
				{
					return true;
				}
				else if (m_bStartMode == FOUR_SIX_ARGEE)
				{
					// 6��������4���˿�ʼ
					if(bPeopoleCount >= 4)
					{
						return true;
					}

					if (bPeopoleCount == m_bMaxPeople)
					{
						return true;
					}

					return false;
				}
			}
		}
	}

	return false;
}

// �������
BYTE CDeskLogicImpl::UserSitDesk(MSG_GR_S_UserSit* pUserSit, CGameUserInfo* pUserInfo)
{
	if (pUserSit != NULL && pUserInfo != NULL) 
	{
		BYTE bDeskStation = pUserSit->bDeskStation;

		m_SoPlayer[bDeskStation].Initialize(pUserInfo);
		
		// ��Ϸ����
		int nPlayCount = 0;
		for (int i = 0; i < m_bMaxPeople; i ++) 
		{
			if (m_pUserInfo[i] != NULL) 
			{
				nPlayCount++;
			}
		}

		if (nPlayCount == 0) 
		{
			// ������Ϸ��ʱ��
			SetTimer(TIMER_GAME_ID, 1000);
		}

		m_nGameStation = gsCountDown;


		// ֪ͨ��ҽ����¼�
		m_pUserNanager->FireEvent(&ISoUserManagerEvent::OnGameUserEnter, (UINT)pUserInfo->m_UserData.dwUserID);
	}

	return __super::UserSitDesk(pUserSit, pUserInfo);
}

// ����뿪����
BYTE CDeskLogicImpl::UserLeftDesk(BYTE bDeskStation, CGameUserInfo* pUserInfo)
{
	if (m_nGameStation == gsPlaying)
	{
		return 0;	// ����Ϸ�в����˳�
	}

	// ��Ҳ�����Ϸ�У�������������
	if (pUserInfo != NULL && m_pUserInfo[bDeskStation] != NULL) 
	{
		if (m_pUserInfo[bDeskStation]->m_UserData.dwUserID == pUserInfo->m_UserData.dwUserID) 
		{
			int userID = m_pUserInfo[bDeskStation]->m_UserData.dwUserID;

			// ֪ͨ����˳��¼�
			m_pUserNanager->FireEvent(&ISoUserManagerEvent::OnGameUserExit, (UINT)pUserInfo->m_UserData.dwUserID);
			m_SoPlayer[bDeskStation].Initialize(NULL);
			m_bAgree[bDeskStation] = FALSE;
		}
	}

	// ��Ϸ����
	int nPlayCount = 0;
	for (int i = 0; i < m_bMaxPeople; i++) 
	{
		if (m_pUserInfo[i] != NULL) 
		{
			nPlayCount ++;
		}
	}

	if (nPlayCount == 0) 
	{
		KillTimer(TIMER_GAME_ID);
	}

	return __super::UserLeftDesk(bDeskStation, pUserInfo);
}

// ������ȡ��Ҷ��߻�����Ϣ
bool CDeskLogicImpl::UserReCome(BYTE bDeskStation,CGameUserInfo* pNewUserInfo)
{
	if (bDeskStation < 0 || bDeskStation >= m_bMaxPeople)
	{
		return false;
	}

	if (m_pUserInfo[bDeskStation] != NULL && m_pUserInfo[bDeskStation]->m_UserData.dwUserID == pNewUserInfo->m_UserData.dwUserID) 
	{
		m_SoPlayer[bDeskStation].Initialize(pNewUserInfo);

		// ��Ҷ���
		m_bNetCut[bDeskStation] = FALSE;
	}

	// ֪ͨ��������¼�
	m_pUserNanager->FireEvent(&ISoUserManagerEvent::OnGameUserReplay, (UINT)pNewUserInfo->m_UserData.dwUserID);

	return __super::UserReCome(bDeskStation, pNewUserInfo);
}

// ������ȡ��Ҷ�����Ϣ(��ƽ̨�ӿ�)
bool CDeskLogicImpl::UserNetCut(BYTE bDeskStation,CGameUserInfo* pLostUserInfo) 
{
	if (bDeskStation < 0 || bDeskStation >= m_bMaxPeople) 
	{
		return false;
	}

	if (m_pUserInfo[bDeskStation] != NULL && m_pUserInfo[bDeskStation]->m_UserData.dwUserID == pLostUserInfo->m_UserData.dwUserID) 
	{
		m_pUserInfo[bDeskStation] = pLostUserInfo;

		m_SoPlayer[bDeskStation].Initialize(pLostUserInfo);

		// ��Ҷ���
		m_bNetCut[bDeskStation] = TRUE;
	}

	// ֪ͨ��Ҷ����¼�
	m_pUserNanager->FireEvent(&ISoUserManagerEvent::OnGameUserOffLine, (UINT)pLostUserInfo->m_UserData.dwUserID);
	

	// ������Ϣ����ƽ̨����, ��OnGameUserOffLine()�Լ�����
	////bool bRet = __super::UserNetCut(bDeskStation, pLostUserInfo);

	return true;
}

// ���ͬ����
bool CDeskLogicImpl::UserAgreeGame(BYTE bDeskStation) 
{
	if (bDeskStation < 0 || bDeskStation >= m_bMaxPeople) 
	{
		return false;
	}

	if (m_pUserInfo[bDeskStation] == NULL) 
	{
		return true;
	}

	// �������Ϸ״̬���򷵻�ʧ��
	if (m_nGameStation == gsPlaying) 
	{
		return true;
	}
	
	if (m_bAgree[bDeskStation]) 
	{
		return true;
	}
	
	m_pUserNanager->FireEvent(&ISoUserManagerEvent::OnGameUserReady, (UINT)m_pUserInfo[bDeskStation]->m_UserData.dwUserID);

	// �������ͬ����Ϣ
	MSG_GR_S_Agree msg;
	msg.bDeskNO				= m_pUserInfo[bDeskStation]->m_UserData.bDeskNO;
	msg.bDeskStation		= m_pUserInfo[bDeskStation]->m_UserData.bDeskStation;
	msg.bAgreeGame			= TRUE;
	m_bAgree[bDeskStation]	= TRUE;
	m_pUserInfo[bDeskStation]->m_UserData.bUserState = USER_ARGEE;

	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		if (m_pUserInfo[i] != NULL) 
		{
			SendGameData(i, (void *)&msg, sizeof(msg), MDM_GM_GAME_NOTIFY, ASS_GM_AGREE_GAME, 0);
		}
	}

	return __super::UserAgreeGame(bDeskStation);
}

// ��ȡ��Ϸ״̬��Ϣ
bool CDeskLogicImpl::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	if (bDeskStation < 0 || bDeskStation >= m_bMaxPeople)
	{
		return false;
	}

	// �����ȷ�״̬��Ϣ2�� �����180��Ϣ���ܷ��� ����ͻ����ղ�����Ϣ
	SendGameData(bDeskStation, MDM_GM_GAME_FRAME, ASS_GM_GAME_STATION, 0);

	// ֪ͨ�ռ���Ϸ�������� ���緿���������
	m_pGameProcessor->FireEvent(&ISoGameProcessBaseMsgEvent::OnCollectBaseMsg, (UINT)m_pUserInfo[bDeskStation]->m_UserData.dwUserID);
	
	// ֪ͨ�ռ��ԹۺͶ����������
	m_pGameProcessor->FireEvent(&ISoGameProcessBaseMsgEvent::OnCollectMsg, (UINT)m_pUserInfo[bDeskStation]->m_UserData.dwUserID);
	
	return true;
}

// ������Ϸ��ʱ��
bool CDeskLogicImpl::SetGameTimer(UINT nTimerID, int nTime)
{
	// ƽ̨���붨ʱ��ID���ܳ���50, ����50��Ч
	// ƽ̨���߶�ʱ��ռ��ID 1~(��������), �����齫��ܹ����߼�ID��Ϊ��������, �������齫��ʱ�����������������(�����߼�ID)
	if (nTimerID <= TIMER_GAME_ID || nTimerID >= 50 || nTime < 0)
	{
		return false;
	}

	return SetTimer(nTimerID, nTime);
}

//������Ϸ��ʼģʽ:  0: ���˲ſ�ʼ   1: ������ͬ��Ϳ�ʼ   2: �Գ�ͬ�⿪ʼ   3: 6����λ��ֻҪ��4����ͬ�⿪ʼ
void CDeskLogicImpl::SetStartMode(BYTE byMode)
{
	m_bStartMode = byMode;
}

// ������Ϸ״̬
bool CDeskLogicImpl::ReSetGameState(BYTE bLastStation)
{
	return true;
}

// ��ʱ����Ϣ
bool CDeskLogicImpl::OnTimer(UINT uTimerID)
{
	if (uTimerID == TIMER_GAME_ID) 
	{
		// �붨ʱ��
		m_pGameProcessor->OnMillTimer();

		if (m_nGameStation == gsPlaying) 
		{ 
			// ��Ϸʱ�Żᷢ�ͳ���Ϸ�Ķ�ʱ��Ϣ
			m_pGameProcessor->FireEvent(&ISoGameProcessorEvent::OnGameTimer);
		}
	} 
	else 
	{
		KillTimer(uTimerID);

		// ���붨ʱ��
		// ��Ϸʱ�Żᷢ�ͳ���Ϸ�Ķ�ʱ��Ϣ
		m_pGameProcessor->FireEvent(&ISoGameProcessTimerEvent::OnTimer, (int)uTimerID);
	}

	return true;
}

// ������Ϸ�¼�
bool CDeskLogicImpl::LoadEvents() 
{
	return true;
}

/** 
* @brief ��ȡ�û���Ϣ
* @param bDeskStation ���λ��
* @return �����û���Ϣ
*/
UserInfoStruct * CDeskLogicImpl::GetUserInfo(BYTE bDeskStation)
{
	if (bDeskStation >= 0 && bDeskStation < m_bMaxPeople)
	{
		if (m_pUserInfo[bDeskStation] != NULL) 
		{
			if (m_pUserInfo[bDeskStation]->m_UserData.dwUserID != 0)
			{
				return &m_pUserInfo[bDeskStation]->m_UserData;
			}
		}
	}

	return NULL; 
}

/** 
* @brief ��ȡ�û���Ϣ
* @param bDeskStation ���λ��
* @return �����û���Ϣ
*/
SoPlayer * CDeskLogicImpl::GetSoPlayer(BYTE bDeskStation)
{
	if (bDeskStation >= 0 && bDeskStation < m_bMaxPeople)
	{
		if (m_SoPlayer[bDeskStation].GetUserID() != 0)
		{
			return &m_SoPlayer[bDeskStation];
		}
	}

	return NULL; 
}

/** 
* @brief ��Ϸ����
* @param nGameEndType ��Ϸ��������
*/
void CDeskLogicImpl::GameFinish(int nGameEndType)
{
	m_bPlaying = false;
	m_nGameStation = gsWait;

	for (BYTE i = 0; i < PLAY_COUNT; i++)
	{	
		m_bAgree[i] = FALSE;
	}

	m_pGameProcessor->FireEvent(&ISoGameProcessorGameEvent::OnBeforeGameEnd, nGameEndType);
	m_pGameProcessor->FireEvent(&ISoGameProcessorGameEvent::OnGameEnd, nGameEndType);
	m_pGameProcessor->FireEvent(&ISoGameProcessorGameEvent::OnPostGameEnd, nGameEndType);

	__super::GameFinish(0, nGameEndType);
}


/**
*  @brief ��ȡ����ID
*/
UINT CDeskLogicImpl::GetRoomID()
{ 
	return m_pDataManage->m_InitData.uRoomID;
}

/**
*  @brief ��ȡ����˰�ձ���
*/
UINT CDeskLogicImpl::GetRoomTax()
{
	return 0;
}

/**
*  @brief ��ȡ�����ע
*/
UINT CDeskLogicImpl::GetBaseMoney() 
{ 
	return 1;
}

/**
*  @brief ��ȡ�������ٽ������
*/
UINT CDeskLogicImpl::GetMinLimitMoney() 
{ 
	return 0;
}

// ����й�
int CDeskLogicImpl::UserTrustee(int nDeskStation, BYTE* pBuffer, int nLen) 
{
	MSG_GR_S_UserTruste *pCmd = (MSG_GR_S_UserTruste *)pBuffer;
	if (pCmd == NULL) 
	{
		return -1;
	}

	if (m_nGameStation != gsPlaying) 
	{
		return 0;
	}

	m_pUserNanager->SetPlayerTrusteeStatus(nDeskStation, pCmd->byFlag);

	return true;
}

// ���ͷ�װ��Ϣ
int CDeskLogicImpl::SendDeskGameData(UINT bAssID, BYTE *pBuffer, int nLen) 
{
	
	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		if (m_pUserInfo[i] != NULL) 
		{
			SendGameData(i, pBuffer, nLen, MDM_GM_GAME_NOTIFY, bAssID, 0);
		}
	}
	return 0;
}

// ���ͷ�װ��Ϣ
int CDeskLogicImpl::SendDeskGameData(BYTE bDeskStation, UINT bAssID, BYTE *pBuffer, int nLen)
{
	return SendGameData(bDeskStation, pBuffer, nLen, MDM_GM_GAME_NOTIFY, bAssID, 0);
}

// �����Թ���Ϣ
int CDeskLogicImpl::SendDeskWatchGameData(UINT bAssID, BYTE *pBuffer, int nLen) 
{
	
	for (int i = 0; i < m_bMaxPeople; i ++) 
	{
		if (m_pUserInfo[i] != NULL) 
		{
			SendWatchData(i, pBuffer, nLen, MDM_GM_GAME_NOTIFY, bAssID, 0);
		}
	}

	return 0;
}

// �����Թ���Ϣ
int CDeskLogicImpl::SendDeskWatchGameData(BYTE bDeskStation, UINT bAssID, BYTE *pBuffer, int nLen) 
{
	return SendWatchData(bDeskStation, pBuffer, nLen, MDM_GM_GAME_NOTIFY, bAssID, 0);
}

// ������ҽ��
void CDeskLogicImpl::ChangeUserPoint(int nDealer, BYTE *byWin, int *arPoint, int *rePoint) 
{
	::memset(rePoint, 0, sizeof(rePoint));

	__int64 tempPoint[4];
	__int64 tempMoney[4];
	::memset(tempPoint, 0, sizeof(__int64) * 4);
	::memset(tempMoney, 0, sizeof(__int64) * 4);
	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		tempPoint[i] = arPoint[i];
	}
	
	// д�����ݿ�
	bool bNetCutUser[PLAY_COUNT];
	::memset(bNetCutUser, 0, sizeof(bNetCutUser));
	__super::ChangeUserPointint64(tempPoint, bNetCutUser);

	// ��ȡʵ�ʵĿ۷�ֵ
	__super::RecoderGameInfo(tempMoney);

	for (int i = 0; i < PLAY_COUNT; i ++) 
	{
		rePoint[i] = (int)tempMoney[i];
	}
}