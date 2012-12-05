
//////////////////////////////////////////////////////////////////////
#ifndef __SOMJ_FRAMELOGIC_H___
#define __SOMJ_FRAMELOGIC_H___

#include "SoBaseLogic.h"
#include "MJStateMgr.h"
#include "SoMJFrameSetting.h"

using namespace MyGame;

namespace MyGame
{

class CSoMJFrameLogic;

class CSoMJFrameLogic :  
	public MyGame::CSoBaseLogic,
	//public TEventImpl<ISoItemManagerEvent>,  //�������ߵ��¼�  , ��ʱ��Ҫ���ߵ��¼�
	public CMJSoStateMgr<PLAYER_NBR>
{
public:
	CSoMJFrameLogic();
	virtual ~CSoMJFrameLogic();
	virtual STATUS Initialize(ISoGameFactory *pGameFactory, int nGameID, int nTableID);

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	// ISoGameProcessorSendBaseInfoEvent
	// ֪ͨ����ģ�����������Ϸ
    virtual int  OnSendBaseInfo(UINT uPlayerID);

	// ֪ͨ����ģ����ҵĿͻ��˰汾��
	virtual int  OnNotityClientVer(int nVer);

	// ISoGameProcessorGameEvent
	// ��Ϸ��ʼ�¼�
	virtual int  OnGameStart();

	// ������Ϸ���������� GAME_END_TYPE
	virtual void SetGameLogicEnd(GAME_END_TYPE eGameEndType);

    // ���ܽ�ɢ��Ϸ��Э���˳�ʱ��ѯ��logic�Ƿ�Ҫ������Ϸ GAME_END_TYPE
	virtual int OnGameEnd(int nGameEndType);
	virtual int OnBeforeGameEnd(int nGameEndType);
	virtual int OnPostGameEnd(int nGameEndType);

	// ISoGameProcessorEvent
	virtual int  OnGameMessage(UINT nMsgAssistantID, UINT nPlayerID, UINT nChair, void * pcGameData, UINT nDataLen);
	virtual int  OnViewerMessage(UINT nMsgAssistantID, UINT nPlayerID, UINT nChair, void * pcGameData, UINT nDataLen);
	virtual int  OnAllPlayerReady();
	virtual int  OnGameTimer();

	//ISoUserManagerEvent
	// ĳ����ҽ����¼�
    virtual int  OnGameUserEnter(UINT uPlayerID);

	// ĳ����ҳ����¼�
	virtual int  OnGameUserExit(UINT uPlayerID);

	// ĳ����Ҷ��ߺ�SDK���ɷ��¼�
	// TODO:
	//		�ж��Ƿ�Ϊ��Ҷ��ߣ���Ҷ��ߺ��Ƿ���Ҫ������״̬����Ϊ�й�״̬��
	//		�����й�״̬�󣬶��ڵ�ǰ����Ƕ��ߵ���ң���ô��Ҫ��ʾ����һ�µ�ǰ״̬�ĳ�ʱ
    //		(�����������ֵ���ǰ��ҳ���,��ǰ��Ҷ����ˣ���ô���������йܵ�ͬʱ����ǿ�Ƴ���)
	virtual int  OnGameUserOffLine(UINT uPlayerID);

    // ĳ������ؽ�
    virtual int  OnGameUserReplay(UINT uPlayerID);

	// ĳ�����׼����
	virtual int  OnGameUserReady(UINT uPlayerID);

	// ĳ�����(�������Թ�)�޸����Ƿ������Թ����ú��ɷ����¼�
	// TODO:
	//		�������Թ�ʱ����������ϵ�����Ϊ�Ʊ����͸���λ�õ��Թ����
	//		�����Թ�ʱ��������������е��Ʒ��͸���λ�õ��Թ����
	virtual int  OnChangeLookOnSetting(UINT nPlayerID, BOOL bLookOn);

	// *pbResult ��ʼ��ΪTRUE, ���ΪFALSE�� FRAME�ǲ����û����иı���λ��
	virtual int  OnBeforeGameUserChangeChair(UINT nPlayerID, BYTE bSrcChair, BYTE bDstChair, BOOL* pbResult);

	//ISoUserManagerTrusteeEvent
	// ����й��¼�֪ͨ
	virtual int  OnChangeTrustee(UINT cChair, BOOL bIsTrustee);

	// ��ʱʱ�䵽������ʱ��
	virtual void OnTimeOut(int nTimerID);

    // ���뼶��ʱ��
    virtual int  OnTimer(int nTimerID);

    //ISoGameSettingEvent
	// ���������ļ�
	virtual int  OnLoadCfg() ;

	// ���¼ӽ������ļ�
	virtual int  OnReloadCfg() ;

    //ISoGameProcessBaseMsg
    //�ռ���Ϸ�������ݣ����緿���������
	virtual int OnCollectBaseMsg(UINT uPlayerID);

	//�ռ��ԹۺͶ������������
	virtual int OnCollectMsg(UINT uPlayerID);

	// �����Ϸ�����ܵĴ���
	virtual void OnPlayerEscapeInGame(UINT shPlayerID );

	// ���ݽ�����Ϣ������ҵ�����
	virtual int SetScoreByFinishMsg(TCMD_NOTIFY_FINISH *pCmd);

    //���ظ�����������࣬ʹ�ø����л���ȡ�����������ָ��
    virtual ISoUserManager* GetSoUserManager();
	virtual ISoGameProcessor* GetSoGameProcess();


	/************************************************************************/
	/* ���淽����ԭ����û�еģ������¼���*/
	/************************************************************************/
	// ��ȡ���ܿ۷���
	virtual int  GetEscapeScore( BYTE ucChair );

	// �������ܿ۷ֻ�ȡ�������
	virtual int  SetEscapeShareScore( TCMD_NOTIFY_FINISH* pCmd, int nEscapeScore, BYTE ucEscapeChair );
	virtual int  SetEscapeShareMoney( TCMD_NOTIFY_FINISH* pCmd, int nEscapeScore, BYTE ucEscapeChair );
	
	// ��ȡ����������Ϣ
	virtual void GetViewInfoMsg( UINT shPlayerID, TCMD_NOTIFY_VIEW_INFO* pCmd );
	virtual void GetBaseInfoMsg( UINT shPlayerID, TCMD_NOTIFY_MJ_BASE_INFO* pCmd );

	// ���Ͳ��ָ�����Ϸ��Ϣ
	virtual void SendOtherGameInfo( UINT shPlayerID );
	virtual void SendItemLimitInfo( UINT shPlayerID );

    // ����
    virtual void DrawTile( int nTurn );

	// ��ȡ�齫����ָ��
	virtual CSoMJFrameSetting * GetMJSoGameSettingPtr() {return m_pSoFrameSetting;}


    // ����GM��Ϣ
    virtual int ProcessRequestGMCmd(UINT nPlayerID, int nChair, void * pcGameData, UINT nDataLen);
    virtual BYTE TransChatToTile(char* pStr);
	virtual BOOL TransGMCmdSetDice(int nChair, char* pStr);   // ����������
	virtual BOOL TransGMCmdSetDealer(int nChair, char* pStr); // ����ׯ��
    virtual BOOL TransGMCmdSwap(int nChair, char* pStr);      // ���ƹ���
    virtual BOOL TransGMCmdSet(int nChair, char* pStr);       // ����ĳ����ҽ�����Ҫ������
    virtual BOOL TransGMCmdNext(int nChair, char* pStr);      // ���ý�����������
    virtual BYTE TransChatToChair(int nChair, char str);  

#if 0
    virtual int ProcessRequestUseVoice(char cMsgID, UINT nPlayerID, int nChair, void * pcGameData, UINT nDataLen);
    virtual int ProcessRequestBuyItem(char cMsgID, UINT nPlayerID, int nChair, void * pcGameData, UINT nDataLen);
	

	virtual BOOL CutUserMoney( int nChair, int nMoneyType, int nMoney );
	virtual void SetFanEvn( ENVIRONMENT &env, int nChair,  int bSetChairTile = TRUE );
	virtual int ProcessRequestCheckWin(char cMsgID, UINT nPlayerID, int nChair, void * pcGameData, UINT nDataLen);

	virtual BOOL CheckIsAutoWinTile();



	void SetGameID( int nGameID ) { m_nGameID = nGameID; }
	int GetGameID() { return m_nGameID; }

	virtual int SetScoreByFinishMsg( TCMD_NOTIFY_FINISH *pCmd );

	// ˢ�¿ͻ��˸���
	virtual void ResetPlayerGangNbr( UINT shPlayerID );

	// ��¼����
	virtual int SwitchBlockCollect( int nNextChair, BYTE ucRequestTile, TCMD_NOTIFY_BLOCK_RESULT &cmd );

	virtual void SendCollectWhich( UINT shPlayerID );
#endif

protected:
	// ���͵ļ�¼
    // ENVIRONMENT	    m_tagGMEnv;
	BOOL			    m_bResetGMEnv;
	CSoMJFrameSetting * m_pSoFrameSetting;

	int  m_nItemLimitMoney[PLAYER_NBR];
	int  m_nItemLimitTimes[PLAYER_NBR];
	char m_ucCollectWhich[PLAYER_NBR][5];

    // ��¼��ʱ����
    int m_nTimeOutCount[PLAYER_NBR];

protected:
	// �������͵������
	//CSoMJPresentGiftMgr m_SoMJPresentGiftMgr;
	//SoMJPresentGiftMgr* m_pSoMJPresentGiftMgr;

	int m_nGameID;  // ��������

 	//CSoMJFrameSetting      m_xSoGameSetting;
 	//CXGameRule           m_GameRule;

//	ISoApplication     *m_pApplication;
//	ISoGameProcessor   *m_pSoGameProcessor;
//	ISoUserManager     *m_pSoUserMgr;
//	ISoGameLogger      *m_pLogger;
//	ENM_GAME_LOGIC_STATUS m_eGameStatus;
};

}

#endif


