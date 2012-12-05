// SoCardLogic.h: interface for the CGameLogic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SOBASELOGIC_H)
#define SOBASELOGIC_H

#include "afxwin.h"
#include "metainfo.h"
#include "SoIddeFine.h"
#include "SoModuleimpl_t.h"
#include "gameeventimpl_t.h"

#include "SoGameProcessor_i.h"
#include "SoUserManager_i.h"
#include "SoGameLogger_i.h"
#include "SoGameFactory_i.h"
#include "SoGameSetting_i.h"


using namespace MyGame;

namespace MyGame
{


class CSoBaseLogic:
		public TEventImpl<ISoGameProcessorSendBaseInfoEvent>,
		public TEventImpl<ISoGameProcessorGameEvent>,
		public TEventImpl<ISoGameProcessorEvent>,
        public TEventImpl<ISoGameProcessBaseMsgEvent>,
		public TEventImpl<ISoUserManagerTrusteeEvent>,
		public TEventImpl<ISoGameProcessTimerEvent>,
		public TEventImpl<ISoGameSettingEvent>,
        public TEventImpl<ISoUserManagerEvent>
{
	
public:

	CSoBaseLogic();
	virtual ~CSoBaseLogic();

	virtual STATUS Initialize(ISoGameFactory *pGameFactory, int nGameID, int nTableID);
	virtual int OnBeforeGameUserChangeChair(UINT nPlayerID, BYTE bSrcChair, BYTE bDstChair, BOOL* pbResult);
	// ISoGameProcessorSendBaseInfoEvent
	virtual int  OnSendBaseInfo(UINT uPlayerID);
	virtual int  OnNotityClientVer(int nVer);

	// ISoGameProcessorGameEvent
	virtual int  OnGameStart();
	//   ���ܽ�ɢ��Ϸ��Э���˳�ʱ��ѯ��logic�Ƿ�Ҫ������Ϸ 
	virtual int  OnGameEnd(int nGameEndType);
	virtual int  OnBeforeGameEnd(int nGameEndType);
	virtual int  OnPostGameEnd(int nGameEndType);

	// ISoGameProcessorEvent
	virtual int  OnGameMessage(UINT nMsgAssistantID, UINT nPlayerID, UINT nChair, void * pcGameData, UINT nDataLen);
	virtual int  OnViewerMessage(UINT nMsgAssistantID, UINT nPlayerID, UINT nChair, void * pcGameData, UINT nDataLen);
	virtual int  OnAllPlayerReady();
	virtual int  OnGameTimer();

	//ISoUserManagerEvent
	virtual int  OnGameUserEnter(UINT uPlayerID);
	virtual int  OnGameUserExit(UINT uPlayerID);

	// ĳ����Ҷ��ߺ�SDK���ɷ��¼�
	// TODO:
	//		�ж��Ƿ�Ϊ��Ҷ��ߣ���Ҷ��ߺ��Ƿ���Ҫ������״̬����Ϊ�й�״̬��
	//		�����й�״̬�󣬶��ڵ�ǰ����Ƕ��ߵ���ң���ô��Ҫ��ʾ����һ�µ�ǰ״̬�ĳ�ʱ
	//		(�����������ֵ���ǰ��ҳ���,��ǰ��Ҷ����ˣ���ô���������йܵ�ͬʱ����ǿ�Ƴ���)
	virtual int  OnGameUserOffLine(UINT uPlayerID);
	virtual int  OnGameUserReplay(UINT uPlayerID);
	virtual int  OnGameUserReady (UINT uPlayerID);
	virtual int  OnBeforeKickUser(UINT nPlayerID, ENM_BEFORE_KICKTYPE enmKickType, BOOL *pbResult);
	// ĳ�����(�������Թ�)�޸����Ƿ������Թ����ú��ɷ����¼�
	// TODO:
	//		�������Թ�ʱ����������ϵ�����Ϊ�Ʊ����͸���λ�õ��Թ����
	//		�����Թ�ʱ��������������е��Ʒ��͸���λ�õ��Թ����
	virtual int  OnGameIsPlayerCanEnter(ISoPlayer *pUserInfo, int *pnResult, char *lpszMsg);
	virtual int  OnGameIsViewerCanEnter(ISoPlayer *pUserInfo, int *pnResult, char *lpszMsg);
	virtual int  OnGameNotifyUserExitReason(UINT nReason, UINT nPlayerID, UINT  nszBufSize, char* szBuf);
	virtual int  OnChangeLookOnSetting(UINT nPlayerID, BOOL bLookOn);

	//ISoUserManagerTrusteeEvent
	virtual int  OnChangeTrustee(UINT cChair, BOOL bIsTrustee);

	virtual int  OnTimer(int nTimerID);

    //ISoGameSettingEvent
	virtual int  OnLoadCfg() ;
	virtual int  OnReloadCfg() ;

    //ISoGameProcessBaseMsg
    //�ռ���Ϸ�������ݣ����緿���������
	virtual int OnCollectBaseMsg(UINT uPlayerID);
	//�ռ��ԹۺͶ������������
	virtual int OnCollectMsg(UINT uPlayerID);


public:
	int				  GetTableID();
	ISoGameProcessor* GetSoGameProcessEx();
	ISoUserManager*   GetSoUserManagerEx();
	ISoGameLogger*    GetSoGameLogger();
	ISoGameFactory*   GetGameFactory();
	ISoGameSetting*   GetSoGameSetting();
	
protected:

	ISoGameProcessor*   m_pSoGameProcessor;
	ISoUserManager*     m_pSoUserMgr;
	ISoGameLogger*      m_pSoLogger;
	ISoGameFactory*     m_pGameFactory;
	ISoGameSetting*     m_pSoGameSetting;
	int					m_nTableID;

};

}
#endif 

