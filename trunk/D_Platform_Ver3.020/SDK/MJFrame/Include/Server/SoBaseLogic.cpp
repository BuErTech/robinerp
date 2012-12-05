#include "stdafx.h"
#include "SoBaseLogic.h"

#include "SoHelperFun.h"

using namespace MyGame;

CSoBaseLogic::CSoBaseLogic()
{
    m_pSoGameProcessor = NULL;
    m_pSoGameProcessor = NULL;
    m_pSoUserMgr       = NULL;
    m_pSoLogger        = NULL;
    m_pSoGameSetting   = NULL;
}

CSoBaseLogic::~CSoBaseLogic()
{
    //MyGame_RemoveSoEventHandler(ISoGameProcessor, ISoGameProcessorEvent, this);
    //MyGame_RemoveSoEventHandler(ISoGameProcessor, ISoGameProcessorGameEvent, this);
    //MyGame_RemoveSoEventHandler(ISoGameProcessor, ISoGameProcessorSendBaseInfoEvent, this);
    //MyGame_RemoveSoEventHandler(ISoGameProcessor, ISoGameProcessTimerEvent, this);

    //MyGame_RemoveSoEventHandler(ISoGameProcessor, ISoGameProcessBaseMsgEvent, this);
    //MyGame_RemoveSoEventHandler(ISoUserManager, ISoUserManagerEvent, this);
    //MyGame_RemoveSoEventHandler(ISoUserManager, ISoUserManagerTrusteeEvent, this);
    //MyGame_RemoveSoEventHandler(ISoGameSetting, ISoGameSettingEvent, this);
}

STATUS CSoBaseLogic::Initialize(ISoGameFactory *pGameFactory, int nGameID, int nTableID)
{
    m_nTableID = nTableID;
    m_pGameFactory = pGameFactory;
    m_pSoGameProcessor = (ISoGameProcessor*)m_pGameFactory->QueryInterface(MODULE_GAMEPROCESSOR);
	m_pSoUserMgr = (ISoUserManager*)m_pGameFactory->QueryInterface(MODULE_USERMAGER);
	m_pSoLogger = (ISoGameLogger*)m_pGameFactory->QueryInterface(MODULE_SOGAMELOGGER);
	m_pSoGameSetting = (ISoGameSetting*)m_pGameFactory->QueryInterface(MODULE_GAMESETTING);

    MyGame_AddSoEventHandler(ISoGameProcessor, ISoGameProcessorEvent, this);
    MyGame_AddSoEventHandler(ISoGameProcessor, ISoGameProcessorGameEvent, this);
    MyGame_AddSoEventHandler(ISoGameProcessor, ISoGameProcessorSendBaseInfoEvent, this);
    MyGame_AddSoEventHandler(ISoGameProcessor, ISoGameProcessTimerEvent, this);

    MyGame_AddSoEventHandler(ISoGameProcessor, ISoGameProcessBaseMsgEvent, this);
    MyGame_AddSoEventHandler(ISoUserManager, ISoUserManagerEvent, this);
    MyGame_AddSoEventHandler(ISoUserManager, ISoUserManagerTrusteeEvent, this);
    MyGame_AddSoEventHandler(ISoGameSetting, ISoGameSettingEvent, this);
    
    return RE_SUCCEED;
}

int CSoBaseLogic::OnBeforeGameUserChangeChair(UINT nPlayerID, BYTE bSrcChair, BYTE bDstChair, BOOL* pbResult)
{
    return 0;
}
// ISoGameProcessorSendBaseInfoEvent
int  CSoBaseLogic::OnSendBaseInfo(UINT shPlayerID)
{
    return 0;
}

int  CSoBaseLogic::OnNotityClientVer(int nVer)
{
    return 0;
}

// ISoGameProcessorGameEvent
int  CSoBaseLogic::OnGameStart()
{
    return 0;
}
//   ���ܽ�ɢ��Ϸ��Э���˳�ʱ��ѯ��logic�Ƿ�Ҫ������Ϸ 
int  CSoBaseLogic::OnGameEnd(int nGameEndType)
{
    return 0;
}

int  CSoBaseLogic::OnBeforeGameEnd(int nGameEndType)
{
    return 0;
}

int  CSoBaseLogic::OnPostGameEnd(int nGameEndType)
{
    return 0;
}

// ISoGameProcessorEvent
int  CSoBaseLogic::OnGameMessage(UINT nMsgAssistantID, UINT nPlayerID, UINT nChair, void * pcGameData, UINT nDataLen)
{
    return 0;
}

int  CSoBaseLogic::OnViewerMessage(UINT nMsgAssistantID,  UINT nPlayerID, UINT nChair, void * pcGameData, UINT nDataLen)
{
    return 0;
}

int  CSoBaseLogic::OnAllPlayerReady()
{
    return 0;
}

int  CSoBaseLogic::OnGameTimer()
{
    return 0;
}

//ISoUserManagerEvent
int  CSoBaseLogic::OnGameUserEnter(UINT shPlayerID)
{

    return 0;
}

int  CSoBaseLogic::OnGameUserExit(UINT shPlayerID)
{
    return 0;
}

// ĳ����Ҷ��ߺ�SDK���ɷ��¼�
// TODO:
//		�ж��Ƿ�Ϊ��Ҷ��ߣ���Ҷ��ߺ��Ƿ���Ҫ������״̬����Ϊ�й�״̬��
//		�����й�״̬�󣬶��ڵ�ǰ����Ƕ��ߵ���ң���ô��Ҫ��ʾ����һ�µ�ǰ״̬�ĳ�ʱ
//		(�����������ֵ���ǰ��ҳ���,��ǰ��Ҷ����ˣ���ô���������йܵ�ͬʱ����ǿ�Ƴ���)
int  CSoBaseLogic::OnGameUserOffLine(UINT shPlayerID)
{
    return 0;
}

int  CSoBaseLogic::OnGameUserReplay(UINT shPlayerID)
{
    return 0;
}

int  CSoBaseLogic::OnGameUserReady (UINT shPlayerID)
{
    return 0;
}

int  CSoBaseLogic::OnBeforeKickUser(UINT nPlayerID, ENM_BEFORE_KICKTYPE enmKickType, BOOL *pbResult)
{
    //��ʼ����Ϊ��������
    *pbResult = 1;
    return 0;
}

// ĳ�����(�������Թ�)�޸����Ƿ������Թ����ú��ɷ����¼�
// TODO:
//		�������Թ�ʱ����������ϵ�����Ϊ�Ʊ����͸���λ�õ��Թ����
//		�����Թ�ʱ��������������е��Ʒ��͸���λ�õ��Թ����
int  CSoBaseLogic::OnChangeLookOnSetting(UINT nPlayerID, BOOL bLookOn)
{
   return 0;
}

int  CSoBaseLogic::OnGameIsPlayerCanEnter(ISoPlayer *pUserInfo, int *pnResult, char *lpszMsg)
{
    if (pnResult)
    {
        *pnResult = 1;
    }
    return 0;
}

int  CSoBaseLogic::OnGameIsViewerCanEnter(ISoPlayer *pUserInfo, int *pnResult, char *lpszMsg)
{
    if (pnResult)
    {
        *pnResult = 1;
    }
	return 0;
}

int  CSoBaseLogic::OnGameNotifyUserExitReason(UINT  nReason, UINT nPlayerID, UINT  nszBufSize, char* szBuf)
{

	return 0;
}

//ISoUserManagerTrusteeEvent
int  CSoBaseLogic::OnChangeTrustee(UINT cChair, BOOL bIsTrustee)
{
    return 0;
}

int  CSoBaseLogic::OnTimer(int nTimerID)
{
    return 0;
}

int  CSoBaseLogic::OnLoadCfg()
{
    return 0;
}

int  CSoBaseLogic::OnReloadCfg()
{
	return 0;
}


int CSoBaseLogic::OnCollectBaseMsg(UINT uPlayerID)
{
    return 0;
}

int CSoBaseLogic::OnCollectMsg(UINT uPlayerID)
{
    return 0;
}

int	CSoBaseLogic::GetTableID()
{
	return m_nTableID;
}

ISoGameProcessor* CSoBaseLogic::GetSoGameProcessEx()
{
	return m_pSoGameProcessor;
}

ISoUserManager*   CSoBaseLogic::GetSoUserManagerEx()
{
	return m_pSoUserMgr;
}
ISoGameLogger*    CSoBaseLogic::GetSoGameLogger()
{
	return m_pSoLogger;
}
ISoGameFactory*   CSoBaseLogic::GetGameFactory()
{
	return m_pGameFactory;
}
ISoGameSetting*   CSoBaseLogic::GetSoGameSetting()
{
	return m_pSoGameSetting;
}
