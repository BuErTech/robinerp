/** @file	BaseTileDealerMgr.h
 *	@brief	��ǽ������
 *	@date 08.08.04
 *
 *	�̳������ƹ����࣬�Ӷ���������齫�Ŀ���Ȩ
 *	����SO��C�˵Ĳ�ͬ����ͬһ����
 *	CBaseTileDealerMgr��Ϊ
 *	C�˵�CGameTileDealerMgr��
 *	SO��CSoTileDealerMgr
 */
#ifndef ___AFX_BASETILEDEALERMGR_H__02CA6498_356A_4B09_A39F_2F0EFA10916B__INCLUDED___
#define ___AFX_BASETILEDEALERMGR_H__02CA6498_356A_4B09_A39F_2F0EFA10916B__INCLUDED___

#include "IMJTileEvent.h"
#include "BaseTilePlayerMgr.h"
#include "MJTimerMgr.h"
#include "CommonStructs.h"

#ifdef WINDOWS_LOGIC
#include "datastructs.h"
#include <gamechatmanager_i.h> 
#endif

namespace MyGame
{

/**
 * @ingroup MJFrame
 * @brief CBaseTileDealerMgr
 *
 * <p>�� 
 *		CBaseTileDealerMgr, ��ǽ������Ļ���
 * </p>
 * @sa
 */	
template<int nPlayerNbr> 
class CBaseTileDealerMgr : public IMJTileEvent,
						#ifndef WINDOWS_LOGIC
						public CSoTilePlayerMgr<PLAYER_NBR>,
 						public CSoMJTimerMgr
						#else
						public CGameTilePlayerMgr<nPlayerNbr>,
						public CGameMJTimerMgr
						#endif
{
public:
	CBaseTileDealerMgr();
	virtual ~CBaseTileDealerMgr();

	/**
	 *  @brief �������Ʒ��͹����Ĺ����Ƶ��¼�
	 *  @param[in]  nEventID �¼�ID
	 *
	 *  Ŀǰû���κ��¼�
	 */	
	virtual void OnTileEvent( int nEventID );

	/**
	 *  @brief ��ǽ��ʼ��
	 *  @return ����0 
	 *
	 * ����û���κβ���
	 */	
	virtual int InitDealer();

	/**
	 *  @brief ϴ��
	 */	
	virtual void RandomDealer();

	/**
	 *  @brief ���ֵķ��Ƶ��������
	 */	
	virtual void SendTilesToPlayer();

	/**
	 *  @brief ���Ȧ����ŷ�
	 */	
	virtual void ResetWindRound();

	/**
	 *  @brief ����
	 *  @param[in]  nTurn ������Ŀ��
	 */	
	virtual void DrawTile( int nTurn );

	/**
	 *  @brief ��ճ�ǽ
	 */	
	virtual void  OnDealerPostGameEnd();

	/**
	 *  @brief Ϊ��һ�ֳ�ʼ��
	 *
	 *  ������Ȧ����ŷ�ļ���,���������ƹ������س�ʼ��
	 */	
	virtual void InitForNext();

	/**
	 *  @brief ��ȡ��ǰ���������
	 *  @return ���ص�ǰ����� 
	 */	
	virtual int GetTurn() { return m_nNowTurn; }

	/**
	 *  @brief ���õ�ǰ���
	 *  @param[in]  nTurn ��ǰ�����
	 */	
	virtual void SetTurn( int nTurn );

	/**
	 *  @brief ��ȡ��ʾ��ǰ�����λ
	 *  @return ��ǰ��ͷ��ʾָ�����
	 *  @warning ��ʾ����Ҳ�һ�����������ڲ��������
	 */	
	virtual int GetShowTurn() { return m_nShowTurn; }
	
	/**
	 *  @brief ������ʾ��ǰ�����λ
	 *  @param[in]  nTurn Ҫ��ʾָ������
	 */	
	virtual void SetShowTurn( int nTurn );
	
	/**
	 *  @brief ��ȡׯ��
	 *  @return ����ׯ��
	 */	
	virtual int GetDealer() { return m_nDealer; }

	/**
	 *  @brief ҡɫ��
	 *
	 *  Ͷ��DICE_NBR��������ɫ��,����1��6
	 */	
	virtual void RandomDice();

	/**
	 *  @brief �����ŷ�
	 *  @param[in]  nRound ��ǰ�ķ���
	 */	
	virtual void SetWindRound( int nRound );

	/**
	 *  @brief ����Ȧ��
	 *  @param[in]  nNbr ��ǰ��Ȧ��
	 */	
	virtual void SetWindRoundNbr( int nNbr );


	/**
	 *  @brief ��ȡ�ŷ�
	 *  @return �����ŷ��� 
	 */	
	virtual int GetWindRound() { return m_nWindRound; }

	/**
	 *  @brief ��ȡȦ��
	 *  @return ����Ȧ����
	 */	
	virtual int GetWindRoundNbr() { return m_nWindRoundNbr; }

	/**
	 *  @brief ��ȡɫ����
	 *  @param[in]  nIndex ɫ�ӵ����к�
	 *  @return ����ɫ����,������кŲ��ڷ�Χ,����1
	 */	
	virtual int GetDice( int nIndex );
	
	/**
	 *  @brief ͨ�����õķ�ʽ��ȡ��ǽ����
	 *  @param[out]  mjWall ��ǽ�������õ�����
	 *  @return ����0
	 */	
	virtual int GetTilesWall( CMJWall& mjWall );

	/**
	*  @brief �������Ƶ����������ʵ�ʵ���
	*  @param[in]  mjTilesNoGod ȥ�������ƺ������
	*  @param[out]  mjTileoGod ȥ�������������飬�������Ǳ��滻�ص���
	*  @param[in]  mjSet ��ҵ�����
	*  @param[in]  nLaiziCount �����Ƶ�����
	*  @param[in]  bNeedFanCount �Ƿ���Ҫ���Ʒ���,�������Ҫ,ֱ�ӷ���1
	*  @return  �����������,����0,���򷵻������˵ķ���ֵ
	*/	
	virtual int CheckNeedFan( CMJHand mjTilesNoGod, CMJHand& mjTilesGod, CMJSet mjSet, int nLaiziCount = 0, int bNeedFanCount = FALSE );

	/**
	 *  @brief �����㷬��������
	 *  @param[in]  env ���������ṹ
	 *  @param[in]  nChair Ҫ���㷬����
	 */	
	virtual void SetFanEvn( ENVIRONMENT &env, int nChair,  int bSetChairTile = TRUE  );

	/**
	 *  @brief ��ȡ���Ƶ�����
	 *  @param[in]  nChair Ҫ���㻨����
	 *  @return ���ػ��Ƶ�����
	 */	
	virtual int GetFlowerNbr( int nChair );

	/**
	 *  @brief ��ջ�����Ϣ
	 */	
	virtual void ClearFlower();

	/**
	 *  @brief �ж��Ƿ�ķ�
	 *  @return �����ǽû���˾ͷ���TRUE,���򷵻�FALSE
	 */	
	virtual BOOL CheckNoTile();

	/**
	 *  @brief ������ʱ�����
	 *
	 *  ����û����
	 */	
	virtual void OnTimeOutFlower(){};

	/**
	 *  @brief ��ȡ��ǽ
	 *
	 *  ����û����
	 */	
	virtual CMJWall GetTilesDealer(){ return m_TilesDealer; }

	/**
	 *  @brief �����������
	 */	
	virtual void ClearRunNbr();

protected:
	/************************************************************************/
	/* ��ǽ������                                                           */
	/************************************************************************/
	CMJWall m_TilesDealer;
	/************************************************************************/
	/* ɫ��                                                                 */
	/************************************************************************/
	int m_nDice[DICE_NBR];

	/************************************************************************/
	/* ׯ��                                                                 */
	/************************************************************************/
	int m_nDealer;

	/************************************************************************/
	/* ����ͷ�Ȧ                                                           */
	/************************************************************************/
	int m_nWindRound;
	int m_nWindRoundNbr;

	/************************************************************************/
	/* ��ǰ��������                                                         */
	/************************************************************************/
	int m_nNowTurn;

	// ��ͼ��ʾ����
	int m_nShowTurn;
	
	// ��������
	int m_nRunNbr;
	// ��������
	int m_nRunWinNbr;
	// ��������
	int m_nRunNotileNbr;

	/************************************************************************/
	/* ������������������(����������)                                       */
	/************************************************************************/
	CMJHand m_TilesDraw;

	/************************************************************************/
	/* ���иܺ�����������                                                   */
	/************************************************************************/
	CMJHand m_TilesGang;
};

#ifndef WINDOWS_LOGIC


/**
 * @ingroup MJFrame
 * @brief CSoTileDealerMgr
 *
 * <p>�� 
 *		CSoTileDealerMgr, SO�µĳ�ǽ������
 * </p>
 * @sa CBaseTileDealerMgr
 *
 * �̳���CBaseTileDealerMgr
 */	
template<int nPlayerNbr> 
class CSoTileDealerMgr : public CBaseTileDealerMgr<PLAYER_NBR>
{
public:
	CSoTileDealerMgr();
	virtual ~CSoTileDealerMgr();

	virtual int  OnGameMessage(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen);

	/**
	 *  @brief ��ǽ��ʼ��
	 *  @return ����0 
	 *
	 *  ִ��ϴ��,�����ŷ����Ȧ����,��ջ���,���������ƵȲ���
	 */	
	virtual int InitDealer();

	/**
	 *  @brief ���ֵķ��Ƶ��������
	 *
	 *  �����ҵ����������,���ڴ˺�������ִ�������Ʋ�����
	 */	
	virtual void SendTilesToPlayer();

	/**
	 *  @brief ���ͽ�����Ϣ���������
	 *
	 *  ����Ϸ״̬�л���MJ_STATE_END��ʱ��ͻ�ִ�д˺���,������صĽ�����Ϣ���������(�����Թ�)
	 */	
	virtual void SendFinishMsg();

	/**
	 *  @brief ��Ϸ��ʼ
	 *
	 *  �������ƹ���������غ���,����û����������,��SDK��OnGameStartִ��ʱ��MJFrame����
	 */	
	virtual int  OnGameStart();

	/**
	 *  @brief ��Ϸ����
	 *
	 */	
	virtual int  OnGameFinish();

	/**
	 *  @brief ����
	 *  @param[in]  nTurn Ҫ��������
	 *
	 *  ��һ���Ƹ�ָ�������,��ͨ�����ƵĲ����ж���һ���л�����״̬
	 */	
	virtual void DrawTile( int nTurn );

    /** 
     * @brief ���뼶��ʱ��
     * @param[in] nTimerID ��ʱ��ID
     */
    virtual void OnMSTimer(int nTimerID);

	/**
	 *  @brief ֧���Զ������Ե���Ϸǰ������Ϣ����
	 *  @warning �˹����ڷ�����ʱ�����ر�
	 */	
    virtual int ProcessRequestReplaceAllCards(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen);

	/**
	 *  @brief �������󲹻�����Ϣ
	 *  
	 *  �ڽ��ܵ����ִ�в�������,����Ϸǰ����״̬��ʱ��,���ж��Ƿ������˶����󲹻���������Ϸ�е�״̬
	 */	
    virtual int ProcessRequestFlower(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen);

	/**
	 *  @brief ������
	 *  
	 *  �ƻ���ľ��͵�컻����������ִ�е���Ϣ
	 */	
	virtual int ProcessRequestChangeTiles(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen);
	
	/**
	 *  @brief �����ȡ��λ��
	 *  
	 *  ǧ���۵���ִ�е���Ϣ
	 */	
	virtual int ProcessRequestGetPosTile(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen);

	/**
	 *  @brief �����ȡ��ǽ��Ϣ
	 *  
	 *  ˳�������ִ�е���Ϣ
	 */	
	virtual int ProcessRequestGetTiles(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen);
		
	/**
	 *  @brief ��������
	 *  
	 *  Ǭ����Ų�Ƶ���ִ�е���Ϣ
	 */	
	virtual int ProcessRequestConfigTiles(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen);
	
 	/**
	 *  @brief �������������Ϣ
	 *
	 *  ÿ�η��ƺ󣬵���Ҫ����ʾλ�����õ������ߵ��¼�
	 */	
	virtual int ProcessRequestGive(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen);

    /*
    *  @brief ��������������Ϣ
    */
    virtual int ProcessRequestDealFinish(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen);

	/**
	 *  @brief ����״̬����
	 *  
	 *  ִ�в���״̬������,�жϵ�ǰ���������������������¸�״̬
	 */	
	virtual void OnTimeOutFlower();

	/**
	 *  @brief ��������ж�
	 *  @param[in]  nChair ���󲹻�����
	 *  
	 *  ������󲹻���,����ڲ���״̬,�ͻ��ж��Ƿ�ȫ�����󲹻��������¸�״̬
	 */	
	virtual void CheckOverFlower( int nChair );

	/**
	 *  @brief ��Ϸ��ʼʱ����ׯ��
	 */	
	virtual void InitSetDealer();

	/**
	 *  @brief ��Ϸ��ʼʱ������������ǽ
	 */	
	virtual void FillWallTiles();

	/**
	 *  @brief �Խ�����Ϣ�����
	 *  @param[in/out]  pCmd ������Ϣ
	 *  
	 *  ��������Ϣ�����������ƣ����ƣ�������Ϣ��
	 */	
    virtual int FillFinishMsg(  TCMD_NOTIFY_FINISH *pCmd  );

	/**
	 *  @brief ���ͽ�����Ϣ
	 *  @param[in]  pCmd ������Ϣ
	 *  
	 *  �ѻ��ֵ���Ϣд��DB��Ȼ���͸��ͻ���
	 */	
    virtual int SendSaveFinishMsg(  TCMD_NOTIFY_FINISH *pCmd, int nCmdLen  );


protected:
	/**
	 *  @brief ���Ϳ�����Ϣ��C��
	 *  
	 *  ��ÿ����ҵ����Ƶ������Ϣ�ֱ��͸�ÿһ�����
	 */	
	virtual void SendStartInfoToClient();

	/**
	 *  @brief ���ݽ�����Ϣ������ҵ�����
	 *  @param[in]  cmd ��������Ϣ
	 *  @return  1��ʾ�ɹ���-1��ʾʧ�ܣ�0��ʾ���ܷ�ȡ����ʧ��
	 *  
	 *  ��ÿ����ҵ����Ƶ������Ϣ�ֱ��͸�ÿһ�����
	 */	
	virtual int SetScoreByFinishMsg( TCMD_NOTIFY_FINISH *pCmd );

	/**
	 *  @brief �۳���Ʊ
	 *  
	 *  ִ�п۳���Ʊ����
	 */	
	virtual void PayTicket();

	/**
	 *  @brief ������ʱ������
	 *  @param[in/out]  cmd ��������Ϣ
	 *  
	 *  ������,���Ƶ���Ϣȫ����䵽������Ϣ����ȥ
	 */	
	virtual void FillTilesForFinishMsg( TCMD_NOTIFY_FINISH *pCmd );

	/**
	 *  @brief ������ʱ����䷬��
	 *  @param[in/out]  cmd ��������Ϣ
	 *  @warning cmd��score�������ÿ���˻�õķ���,����ʵ�ʵĻ���
	 *  
	 *  ��Ϸ����ʱ���㷬����
	 */	
	virtual void FillFanForFinishMsg( TCMD_NOTIFY_FINISH *pCmd );

	/**
	 *  @brief �ѽ���ʱ�Ľ�����ֻ�����Ϸ��(���ֶ�)
	 *  @param[in/out]  cmd ��������Ϣ
	 *  
	 *  ����cmd�ṩ��ÿ����ҵ�scoreֵ����������õĻ���ֵ.�������Ӯ������
	 */	
	virtual void ScoreToMoneyForFinishMsg( TCMD_NOTIFY_FINISH *pCmd );
	
	/**
	 *  @brief ��Ϸ����ʱ�����
	 *  @param[in/out]  cmd ��������Ϣ
	 *  @warning ����ǻ���cmd�����score�������,�����Ḳ�Ǳ����ֵ
	 *  
	 *  ����cmd�ṩ��ÿ����ҵ�scoreֵ������ɻ���
	 */	
	virtual void CountScoreForFinishMsg( TCMD_NOTIFY_FINISH *pCmd );

	/**
	 *  @brief ����
	 *  @param[in]  nChair Ҫ���Ƶ���
	 *  @return �������ƵĽ��,�ɹ�TRUE,ʧ��:FALSE
	 *
	 *  ������ò����������,��һɫ:20%,��һɫ:30%,�ԶԺ�:%50
	 */	
	virtual int ConfigureTiles( int nChair );

	/**
	 *  @brief ������һɫ
	 *  @param[in]  nChair Ҫ���Ƶ���
	 *  @return �������ƵĽ��,�ɹ�TRUE,ʧ��:FALSE
	 *
	 *  ��8��ͬһ��ɫ���Ƶ�����
	 */	
	BOOL ConfigureQingyise( int nChair );

	/**
	 *  @brief �����İ���
	 *  @param[in]  nChair Ҫ���Ƶ���
	 *  @return �������ƵĽ��,�ɹ�TRUE,ʧ��:FALSE
	 *
	 *  ����2����,һ�����ӵ�����
	 */	
	BOOL ConfigureSianke( int nChair );

	/**
	 *  @brief ������һɫ
	 *  @param[in]  nChair Ҫ���Ƶ���
	 *  @return �������ƵĽ��,�ɹ�TRUE,ʧ��:FALSE
	 *
	 *  ��8����������Ƶ�����
	 */	
	BOOL ConfigureZiyise( int nChair );

	/**
	 *  @brief ����������Ϣ
	 */	
	virtual void SendRunNbr();

	/**
	 *  @brief ������һɫ
	 *  @param[out]  pCmd ��䲿����Ϣ
	 *
	 */	
	virtual int GetDealInfo( int nChair, TCMD_NOTIFY_DEAL* pCmd );

	/**
	 *  @brief ��Ϸ����ʱ����֣����㷬�ͷ�֮ǰ������
	 *  @param[in]  pCmd ��������Ϣ
	 */	
	virtual void CountRunNbr( TCMD_NOTIFY_FINISH *pCmd );

	/**
	 *  @brief ��Ϸ����ʱ�ļ���ܷ֣��ڽ�����Ӯ��֮�󱻵���
	 *  @param[in]  pCmd ��������Ϣ
	 */	
    virtual void CountGangScoreForFinishMsg( TCMD_NOTIFY_FINISH *pCmd );

protected:
     // �Ƿ������ˣ�������������Զ�ȡ����Ϸ��ʼʱ�����ݳ�ʼ����ׯ��ָ��
     BOOL m_bReplaceWall;
     BOOL m_bReplaceDice;
     BOOL m_bReplaceDealer;

	 /********************/
	 /* GM��Ҫ�����Ƽ� */
	 /********************/
	 CMJHand m_TilesSetConfig[nPlayerNbr]; // ָ����ҽ�����Ҫ����������
	 CMJHand m_TilesNextConfig;            // ������Ҫ����������
	 CMJHand m_TielsDiceConfig;            // ��������
	 CMJHand m_TilesDealerConfig;          // ׯ������

     // �Ƿ���16�ֻ�ׯ���趨��Ĭ��û��
     BOOL m_bWindReroll;

     // ��¼������Ϣ
     TCMD_NOTIFY_FINISH m_cmdFinish;

     // ��¼�Ƿ�ʹ�������Ƶ��ߣ�����Ϸ��ʼ���Ƶ�ʱ��ʹ��
     BOOL m_bConfigTiles[nPlayerNbr];

     // ��¼�Ƿ񲹻���
     BOOL m_bCheckFlower[nPlayerNbr];

     // ������ɱ�־ 
     BOOL m_bDealFinish[nPlayerNbr];  
};
		
#else

/**
 * @ingroup MJFrame
 * @brief CGameTileDealerMgr
 *
 * <p>�� 
 *		CGameTileDealerMgr, �ͻ����µĳ�ǽ������
 * </p>
 * @sa CBaseTileDealerMgr
 *
 * �̳���CBaseTileDealerMgr
 */	
template<int nPlayerNbr> 
class CGameTileDealerMgr : public CBaseTileDealerMgr<nPlayerNbr>
{
public:
	CGameTileDealerMgr();
	virtual ~CGameTileDealerMgr();

	virtual void OnGameMessage(UINT dwLen,  LPBYTE  pGameMsg);

	/**
	 *  @brief ����ʼ�ķ�����Ϣ�Ͷ���ׯ�ҵ�
	 */	
	virtual void OnNotifyDeal(UINT nLen, BYTE *pBuf);

	/**
	 *  @brief ����������Ϣ
	 */	
	virtual void OnNotifyDraw(UINT nLen, BYTE *pBuf);

	/**
	 *  @brief ���������Ϣ���߼�����
	 */	
	virtual void OnNotifyFinish(UINT nLen, BYTE *pBuf);

	/**
	 *  @brief ��������Ϣ
	 */	
	virtual void OnNotifyFlower(UINT nLen, BYTE *pBuf);

	/**
	 *  @brief �����Ƶ�����Ϣ
	 */	
	virtual void OnNotifyChangeTiles( UINT nLen, BYTE *pBuf );

	/**
	 *  @brief �������Ƶ�����Ϣ
	 */	
	virtual void OnNotifyConfigTiles( UINT nLen, BYTE *pBuf );

	/**
	 *  @brief ��������������Ϣ
	 */	
	virtual void OnNotifyRunNbr( UINT nLen, BYTE *pBuf );

	/**
	 *  @brief ��Ϸ��ʼʱ��ִ�е���Ϣ
	 */	
	virtual void  OnGameStart() {};
		
	/**
	 *  @brief ���õ�ǰ�����λ
	 *  @param[in]  nTurn ��ǰ���ڲ��������
	 */	
	virtual void SetTurn( int nTurn );

	/**
	 *  @brief ��ȡSDK�ĵ��߹���ӿ�
	 *  @return ���߹�����ӿ�
	 *  @warning ��ǽ�����౾�����ᴴ����ץȡ�ӿ�,��Ҫ����
	 */	
	//virtual IItemManager* GetItemManager() { return NULL; }

	/**
	 *  @brief ��Ϸǰ����ʱ�䵽��ִ�еĲ���
	 */	
	virtual void OnTimeOutFlower() {};

	virtual BOOL IsLastDrawTileBack() { return m_bLastDrawBack; }
protected:
	// �Ƿ��ǴӺ���������
	BOOL m_bLastDrawBack;

};

#endif

}

#include "BaseTileDealerMgr.inl"

#endif // !defined(AFX_BASETILEDEALERMGR_H__02CA6498_356A_4B09_A39F_2F0EFA10916B__INCLUDED_)
