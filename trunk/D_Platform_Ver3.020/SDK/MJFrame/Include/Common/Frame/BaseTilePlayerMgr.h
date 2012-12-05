/** @file	BaseTilePlayerMgr.h
 *	@brief	���ƹ�����
 *	@author edward
 *	@date 08.08.04
 *
 *	���ƹ����࣬�̳��ڹ����࣬ʵ���˶����ƺͳ��Ƶ����й���
 *	CBaseTilePlayerMgr�ǻ��࣬����C�˺�SO�˷ֱ��в�ͬ��MGR
 */
#if !defined(AFX_BASETILEPLAYERMGR_H__E131B1E3_44E0_4A64_B7AC_BEA3D103C999__INCLUDED_)
#define AFX_BASETILEPLAYERMGR_H__E131B1E3_44E0_4A64_B7AC_BEA3D103C999__INCLUDED_
 
//#include "StdAfx.h"

#include "MJ.h"
#include "mjcfgdef.h"

#include "MJFrameBaseData.h"
#include "IMJTileEvent.h"
#include "IMJStateEvent.h"
#include "MJRule.h"
#include "Clog.h"

#ifndef WINDOWS_LOGIC
#include "SoUserManager_i.h"
#else
#include "gameprocess_i.h"
#include "usermanager_i.h"
#endif
 
namespace MyGame
{

/**
 * @ingroup MJFrame
 * @brief CBaseTilePlayerMgr
 *
 * <p>�� 
 *		CBaseTilePlayerMgr, ���ƿ�����Ļ���
 * </p>
 * @sa
 */	
template<int nPlayerNbr>
class CBaseTilePlayerMgr : public CMJRule
{
public:
	CBaseTilePlayerMgr();
	virtual ~CBaseTilePlayerMgr();

	/**
	 *  @brief Ϊ��һ�������ĳ�ʼ��
	 *
	 *  ����Ϸ����ʱ���ã��������״̬������״̬
	 */	
	virtual void InitForNext();

	/**
	 *  @brief ����״̬�¼��ӿ�
	 *  @param[in]  pStateEvent ״̬�¼��Ľӿ�ָ��
	 *
	 *  Ϊ���������ƻ�ȡ��һ����״̬��Ϣ���ṩ�Ľӿ�
	 */	
	virtual void SetStateEvent( IMJStateEvent* pStateEvent );

	/**
	 *  @brief ���ó�ǽ�¼��ӿ�
	 *  @param[in]  pTileEvent ��ǽ�¼��Ľӿ�ָ��
	 *
	 *  Ϊ���������ƻ�ȡ��һ���ĳ�ǽ��Ϣ���ṩ�Ľӿ�
	 */	
	virtual void SetTileEvent( IMJTileEvent* pTileEvent );

	/**
	 *  @brief ��ȡ״̬�¼��ӿ�
	 */	
	IMJStateEvent* GetStateEvent() { return m_pStateEvent; }

	/**
	 *  @brief ����״̬ʱ�䵽�����¼�
	 *  @param[in]  bAutoWin �Ƿ�ǿ�ƺ���
	 */	
	virtual void OnTimeOutBlock( BOOL bAutoWin = TRUE ){};

	/**
	 *  @brief ����״̬ʱ�䵽�����¼�
	 */	
	virtual void OnTimeOutGive(){};

	/**
	 *  @brief ��������
	 *  @param[in]  nChair ���õ����
	 *  @param[in]  mjHand ���õ�����
	 *  @param[in]  bNew �Ƿ������ǰ����������
	 */	
	virtual void SetHandTile( int nChair, CMJHand &mjHand, BOOL bNew = TRUE );

	/**
	 *  @brief ��������
	 *  @param[in]  nChair ���õ����
	 *  @param[in]  mjSet ���õ�����
	 *  @param[in]  bNew �Ƿ������ǰ����������
	 */	
	virtual void SetSetTile( int nChair, CMJSet &mjSet, BOOL bNew = TRUE );

	/**
	 *  @brief ���һ������
	 *  @param[in]  nChair Ҫ��ӵ����
	 *  @param[in]  ucTile ��ӵ���
	 */	
	virtual void AddHandTile( int nChair, TILE ucTile );

	/**
	 *  @brief ɾ��һ������
	 *  @param[in]  nChair Ҫɾ�������
	 *  @param[in]  ucTile ɾ������
	 *  @return  ����ɾ���Ľ���ɹ�:TRUE,ʧ��:FALSE
	 */	
	virtual BOOL DelHandTile( int nChair, TILE ucTile );

	/**
	 *  @brief �����������
	 *  @param[in]  nChair Ҫ���õ����
	 *  @param[in]  bTing �Ƿ�����
	 */	
	virtual void SetTing( int nChair, BOOL bTing = TRUE );

	/**
	 *  @brief ��ȡ����Ƿ�����
	 *  @param[in]  nChair Ҫ��ȡ�����
	 *  @return  �����Ƿ�����,��:TRUE,��:FALSE
	 */	
	virtual BOOL GetTing( int nChair );

	/**
	 *  @brief �ж��Ƿ���Ҫ����
	 *  @param[in]  nChair �жϵ����
	 *  @return  ���ػ�������
	 */	
	virtual int CheckFlower(  int nChair );


	/**
	 *  @brief �ж�����
	 *  @param[in]  nChair �жϵ����
	 *  @param[in]  bSelf �Ƿ�ֻ�ж��Լ�
	 *  @param[in]  ucLastTile ���һ�ű��˳�����,�����ֻ�ж��Լ�,���������û������
	 *  @param[in]  shLevel �趨���Ƶȼ������ڵȼ���Ҳ����Ϊ��������
	 *  @return  �������Ƶȼ�
	 */	
	virtual short CheckBlock(  int nChair, BOOL bSelf = TRUE, TILE ucLastTile = TILE_CHAR_1, short shLevel = 1 );
	
	/**
	 *  @brief �ж���������
	 *  @param[in]  nChair �жϵ����
	 *  @param[out] shBlockLevel ���������������ߵȼ����᷵�ش˵ȼ�
	 *  @param[in]  ucLastTile ���һ�ű��˳�����,�����ֻ�ж��Լ�,���������û������
	 *  @return  �������Ƶȼ�
	 */	
	virtual short CheckBlockSelfWin(  int nChair, short &shBlockLevel, TILE ucLastTile = TILE_CHAR_1 );

	/**
	 *  @brief �ж������Լ���
	 *  @param[in]  nChair �жϵ����
	 *  @param[out] shBlockLevel ���������������ߵȼ����᷵�ش˵ȼ�
	 *  @param[in]  ucLastTile ���һ�ű��˳�����,�����ֻ�ж��Լ�,���������û������
	 *  @return  �������Ƶȼ�
	 */	
	virtual short CheckBlockSelfQuadruplet(  int nChair, short &shBlockLevel, TILE ucLastTile = TILE_CHAR_1 );
	
	/**
	 *  @brief �ж������Լ���
	 *  @param[in]  nChair �жϵ����
	 *  @param[out] shBlockLevel ���������������ߵȼ����᷵�ش˵ȼ�
	 *  @param[in]  ucLastTile ���һ�ű��˳�����,�����ֻ�ж��Լ�,���������û������
	 *  @return  �������Ƶȼ�
	 */	
	virtual short CheckBlockTing(  int nChair, short &shBlockLevel, TILE ucLastTile = TILE_CHAR_1 );

	/**
	 *  @brief �ж����Ƴ���
	 *  @param[in]  nChair �жϵ����
	 *  @param[out] shBlockLevel ���������������ߵȼ����᷵�ش˵ȼ�
	 *  @param[in]  ucLastTile ���һ�ű��˳�����,�����ֻ�ж��Լ�,���������û������
	 *  @return  �������Ƶȼ�
	 */	
	virtual short CheckBlockCollect(  int nChair, short &shBlockLevel, TILE ucLastTile = TILE_CHAR_1, int nGiveChair = 0 );
	
	/**
	 *  @brief �ж�������
	 *  @param[in]  nChair �жϵ����
	 *  @param[out] shBlockLevel ���������������ߵȼ����᷵�ش˵ȼ�
	 *  @param[in]  ucLastTile ���һ�ű��˳�����,�����ֻ�ж��Լ�,���������û������
	 *  @return  �������Ƶȼ�
	 */	
	virtual short CheckBlockTriplet(  int nChair, short &shBlockLevel, TILE ucLastTile = TILE_CHAR_1 );

	/**
	 *  @brief �ж����Ƹܱ���
	 *  @param[in]  nChair �жϵ����
	 *  @param[out] shBlockLevel ���������������ߵȼ����᷵�ش˵ȼ�
	 *  @param[in]  ucLastTile ���һ�ű��˳�����,�����ֻ�ж��Լ�,���������û������
	 *  @return  �������Ƶȼ�
	 */	
	virtual short CheckBlockQuadruplet(  int nChair, short &shBlockLevel, TILE ucLastTile = TILE_CHAR_1 );

	/**
	 *  @brief �ж����ƺ�����
	 *  @param[in]  nChair �жϵ����
	 *  @param[out] shBlockLevel ���������������ߵȼ����᷵�ش˵ȼ�
	 *  @param[in]  ucLastTile ���һ�ű��˳�����,�����ֻ�ж��Լ�,���������û������
	 *  @return  �������Ƶȼ�
	 */	
	virtual short CheckBlockWinOther(  int nChair, short &shBlockLevel, TILE ucLastTile = TILE_CHAR_1 );

	/**
	 *  @brief �ж�����
	 *  @param[in]  nChair �жϵ����
	 *  @return  �����Ƿ��������,��:TRUE,��:FALSE
	 */	
	virtual BOOL CheckTing( int nChair );

	/**
	 *  @brief ��ȡ����
	 *  @param[in]  nChair ����ȡ�����
	 *  @param[out]  mjHand �������õ�������ò�������
	 *  @return  ���Ƶĳ���
	 */	
	virtual int GetTilesHand( int nChair, CMJHand& mjHand );

	/**
	 *  @brief ��ȡ����
	 *  @param[in]  nChair ����ȡ�����
	 *  @param[out]  mjHand �������õ�������ò�������
	 *  @return  ���Ƶĳ���
	 */	
	virtual int GetTilesSet( int nChair, CMJSet& mjSet );

	/**
	 *  @brief ��ȡ����
	 *  @param[in]  nChair ����ȡ�����
	 *  @param[out]  mjHand �������õ�������ò�������
	 *  @return  ���Ƶĳ���
	 */	
	virtual int GetTilesGive( int nChair, CMJGive& mjGive );

	/**
	 *  @brief ��ȡ�Ƿ�����ʾ���ƣ����ƣ�
	 *  @param[in]  nChair ����ȡ�����
	 *  @return  ����:TRUE,����:FALSE
	 */	
	virtual BOOL GetShowTile( int nChair );

	/**
	 *  @brief ���õ�ǰ�����һ����
	 *  @param[in]  ucTile ���һ����
	 *
	 *  �����ƿ�����������Ļ��������ȥ��
	 */	
	void SetLastTile( TILE ucTile );

	/**
	 *  @brief ��ȡ��ǰ�����һ����
	 *  @return  ���һ����
	 *
	 *  �����ƿ�����������Ļ��������ȥ��
	 */	
	TILE GetLastTile() { return m_ucLastTile; }

	/**
	 *  @brief ���©������
	 *  @param[in]  nChair ����յ����
	 */	
	virtual void ClearLoseHu( int nChair );

	/**
	 *  @brief ���ó�Ϊ©������
	 *  @param[in]  nChair ���õ����
	 *  @param[in]  ucTile ��¼©������
	 *  @warning  ©���洢��������ֻ��17��,����17�Ž����ܱ���¼
	 */	
	virtual void SetLoseHu( int nChair, TILE ucTile );

	/**
	 *  @brief �ж��Ƿ�Ϊ©����¼����
	 *  @param[in]  nChair �������
	 *  @param[in]  ucTile ��������
	 *  @return  ����©��������:TRUE,����:FALSE
	 */	
	virtual BOOL CheckLoseHu( int nChair, TILE ucTile );

	/**
	 *  @brief ��ȡ��ǰ���и��Ƶ�����
	 *  @return  ���и��Ƶ�����
	 */	
	virtual int GetAllGangNbr() { return m_nAllGangNbr; }

	/**
	 *  @brief ����
	 *  @param[in]  mjHand ��Ҫ���������
	 *  @param[in]  nMod 0:����,1:����
	 *  @param[in]  nGod 0:�����Ʋ�������������,1:�����Ʒ������е�ͷ��,����:�����Ʒ������е�β��
	 *  @param[in]  ptilesGod ��������
	 */	
	virtual void SortHandTiles( CMJHand &mjHand, int nMod = 0, int nGod = 0, CMJHand* ptilesGod = NULL );
	
	/**
	 *  @brief �ж��Ƿ�������Զ�������
	 *  @return ������Զ�������TRUE������FALSE
	 */	
	virtual BOOL CheckIsAutoWinTile();

	/**
	 *  @brief ��ȡ�ӳ�ǽ���滭������
	 *  @return  ��Ҫ�Ӻ��滭������
	 */	
	virtual int GetDrawWallBackNbr();

	/**
	 *  @brief ���������Ƶ���
	 *  @param[in]  byChair ���Ƶ���
	 */	
	virtual void SetLastGiveChair(BYTE byChair);
	
	/**
	 *  @brief ��ȡ�����Ƶ���
	 *  @return ���������Ƶ���
	 */	
	virtual BYTE GetLastGiveChair();

protected:
	// 2�����·��ʵĽӿ�
	IMJStateEvent*  m_pStateEvent;
	IMJTileEvent*	m_pTileEvent;


	// ���ƺͳ�������
	CMJHand m_TilesHand[nPlayerNbr];
	CMJSet  m_TilesSet[nPlayerNbr];
	CMJGive m_TilesGive[nPlayerNbr];

	// ���Ƶ�����
	int		m_nTingType[nPlayerNbr];
	// �����ƣ��ܴ��ȥ��
	CMJHand	m_TilesTingGive;
	// ���ƴ��ȥ��������
	CMJHand m_TilesTingGiveWin[MAX_HAND_TILES];

	// ����������������
	// �Ƿ�������
	BOOL	m_bShowTile[nPlayerNbr];

	// ���Ƶ�״̬��ÿ���ж�ǰ�������
	BOOL	m_bBlockEnable[MJ_BLOCK_NBR];

	// ÿ����ҵ����Ƶȼ�����CHECKBLOCK�����
	int		m_nBlockLevel[nPlayerNbr];

	// ���Ƶ�״̬
	short   m_shGangState;

	// ���Ƶ�����
	int m_nAllGangNbr;

	// ������
	TILE m_ucLastTile;

	// ���˵���
	BOOL m_bWiner[nPlayerNbr];

	// ©��
	TILE m_ucLoseHu[nPlayerNbr][MAX_HAND_TILES];

	// �����Ƶ����
	CMJHand m_TilesPlayerGod[nPlayerNbr];

	// ���һ�����Ƶ���
	BYTE m_byLastChair;

	/**
	 *  @brief ��ǰ���������жϵ���ң���CHECKBLOCK��SWITCHBLOCK��ʱ�����ã��Ա�֪����˭���ڱ��ж�
	 */	
	BYTE m_ucCurCheckChair;

	// ��¼�����õ�
	int  m_nOldGangNbr;

	// dxh add ��¼�����ܱ�־  
	BOOL m_bIsSet;
};


#ifndef WINDOWS_LOGIC

/**
 * @ingroup MJFrame
 * @brief CSoTilePlayerMgr
 *
 * <p>�� 
 *		CSoTilePlayerMgr, SO���ƿ�����Ļ���
 * </p>
 * @sa
 */	
template<int nPlayerNbr>
class CSoTilePlayerMgr : public CBaseTilePlayerMgr<PLAYER_NBR>
{
public:
	CSoTilePlayerMgr();
	virtual ~CSoTilePlayerMgr();

	virtual int  OnGameMessage(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen);

	/**
	 *  @brief ��ȡISoGameProcessor�ӿ�
	 *  @return  ISoGameProcessor�ӿ�ָ��
	 *  @warning �˺�������ֱ�ӷ���NULL,����ҪLogic���������
	 */	
	virtual ISoGameProcessor* GetSoGameProcess() { return NULL; }

	/**
	 *  @brief ��ȡISoUserManager�ӿ�
	 *  @return  ISoUserManager�ӿ�ָ��
	 *  @warning �˺�������ֱ�ӷ���NULL,����ҪLogic���������
	 */	
	virtual ISoUserManager* GetSoUserManager() { return NULL; }
	
    
    /**
    *  @brief �ж�����
    *  @param[in]  nChair �жϵ����
    *  @param[in]  bSelf �Ƿ�ֻ�ж��Լ�
    *  @param[in]  ucLastTile ���һ�ű��˳�����,�����ֻ�ж��Լ�,���������û������
    *  @param[in]  shLevel �趨���Ƶȼ������ڵȼ���Ҳ����Ϊ��������
    *  @return  �������Ƶȼ�
    */	
    virtual short CheckBlock(  int nChair, BOOL bSelf = TRUE, TILE ucLastTile = TILE_CHAR_1, short shLevel = 1 );

  	/**
	 *  @brief ���Ƶ�ִ��
	 *  @param[in]  nChair ���Ƶ���
	 *  @param[in]  ucTile ������
	 *
	 *  ������һ������bTimeOutΪTRUE,ִ��ǿ�Ʋ���,ǰ��Ĳ���û�м�ֵ
	 */	
	virtual int CheckGiveBlock( int nChair, BYTE ucTile );

 	/**
	 *  @brief ��������������Ϣ
	 *  @warning �����ʱ��,��һ�����ڴ�������Ϣ���˵�����
	 */	
	virtual int ProcessRequestBlock(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen);
	
  	/**
	 *  @brief ���󲹻�
	 */	
	virtual int ProcessRequestFlower(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen);

    /**
    *  @brief �������������Ϣ
    */	
    virtual int ProcessRequestGive(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen);

  	/**
	 *  @brief ����ʱ�䵽����¼�����
	 *  @param[in]  bAutoWin �Ƿ�ǿ�ƺ���
	 */	
	virtual void OnTimeOutBlock( BOOL bAutoWin = TRUE );

  	/**
	 *  @brief ����ʱ�䵽����¼�����
	 *
	 *  SOִ�д��¼���ʱ��,��ǿ��ˢ�³�����ҵ���������
	 */	
	virtual void OnTimeOutGive();

  	/**
	 *  @brief ��Ϸ��ʼ���¼�����
	 */	
	virtual int OnGameStart();

  	/**
	 *  @brief ���Ƶ�ִ��
	 *  @param[in]  nChair �������Ƶ����
	 *  @param[in]  ucFlag �������Ƶı��(��,��,��,��,��,����)
	 *  @param[in]  ucTile �������Ƶ�������
	 *  @param[in]  bTimeOut �Ƿ�ǿ��ȫ������
	 *
	 *  ������һ������bTimeOutΪTRUE,ִ��ǿ�Ʋ���,ǰ��Ĳ���û�м�ֵ
	 */	
	virtual void SwitchBlock( int nChair, BYTE ucFlag, BYTE ucTile[4], BOOL bTimeOut = FALSE );

  	/**
	 *  @brief ���Ƶ�ִ�г�
	 *  @param[in]  nNextChair �������Ƶ����
	 *  @param[in]  nNextChair �������Ƶ���(��һ�������һ��)
	 *  @param[in]  cmd ���Ƶ���Ϣ
	 */	
	virtual int SwitchBlockCollect( int nNextChair, BYTE ucRequestTile, TCMD_NOTIFY_BLOCK_RESULT &cmd );

   	/**
	 *  @brief ���Ƶ�ִ����
	 *  @param[in]  nNextChair �������Ƶ����
	 *  @param[in]  nNextChair �������Ƶ���(��һ�������һ��)
	 *  @param[in]  cmd ���Ƶ���Ϣ
	 */	
	virtual int SwitchBlockTriplet( int nNextChair, BYTE ucRequestTile, TCMD_NOTIFY_BLOCK_RESULT &cmd );

   	/**
	 *  @brief ���Ƶ�ִ�а���
	 *  @param[in]  nNextChair �������Ƶ����
	 *  @param[in]  nNextChair �������Ƶ���(��һ�������һ��)
	 *  @param[in]  cmd ���Ƶ���Ϣ
	 */	
	virtual int SwitchBlockQuadrupletConcealed( int nNextChair, BYTE ucRequestTile, TCMD_NOTIFY_BLOCK_RESULT &cmd );

   	/**
	 *  @brief ���Ƶ�ִ������
	 *  @param[in]  nNextChair �������Ƶ����
	 *  @param[in]  nNextChair �������Ƶ���(��һ�������һ��)
	 *  @param[in]  cmd ���Ƶ���Ϣ
	 */	
	virtual int SwitchBlockQuadrupletRevealed( int nNextChair, BYTE ucRequestTile, TCMD_NOTIFY_BLOCK_RESULT &cmd );

   	/**
	 *  @brief ���Ƶ�ִ�к���
	 *  @param[in]  nNextChair �������Ƶ����
	 *  @param[in]  nNextChair �������Ƶ���(��һ�������һ��)
	 *  @param[in]  cmd ���Ƶ���Ϣ
	 */	
	virtual int SwitchBlockWin( int nNextChair, BYTE ucRequestTile, TCMD_NOTIFY_BLOCK_RESULT &cmd );

   	/**
	 *  @brief ���Ƶ�ִ�к��״̬�޸�
	 *  @param[in]  nChair �������Ƶ����
	 *  @param[in]  cmd ���Ƶ���Ϣ
	 */	
	virtual int SwitchBlockOver( int nChair, TCMD_NOTIFY_BLOCK_RESULT &cmd );
	
	/**
	 *  @brief ˢ���������
	 *  @param[in]  nChair ��ˢ�µ����
	 *
	 *  ��SO������ҿͻ������ݳ����첽,ִ�д˺�������ˢ����Ϣˢ����ҵ�����
	 */	
	virtual void UpdateHandTiles( int nChair );

  	/**
	 *  @brief ��ȡ��ҵ���Ϸ��(���ֶ�)
	 *  @param[in]  nChair �����ȡ�����
	 *  @return  ��ҵ���Ϸ��(���ֶ�)
	 *
	 *  �������þ����Ƿ��ػ��ֶ�������Ϸ��
	 */	
	virtual int GetMJUserMoney( int nChair );

    /**
    *  @brief ��ȡ������Ϣ
    *  @param[in]  nChair ����ȡ�����
    *  @return TCMD_NOTIFY_BLOCK �ṹ
    */	
    virtual TCMD_NOTIFY_BLOCK GetBlockInfo( int nChair );

  	/**
	 *  @brief Ҫ��C��ʾ��ʾ�Ի���
	 *  @param[in]  pMsg ��ʾ������
	 *  @param[in]  nLen ���ݵĳ���
	 *  @param[in]  nMsgID ��Ϣ��ID(ĿǰĬ��Ϊ0)
	 *  @param[in]  nPlayerID �Ƿ���Ҫ��ָ����ҷ�����Ϣ(���Ϊ-1��ȫ������)
	 *  @param[in]  byFlag YESNO����OK����
	 */	
	virtual void ShowDlgMsg(char* pMsg, int nLen, int nMsgID = 0, short nPlayerID = -1, BYTE byFlag = 0);
	
  	/**
	 *  @brief ��ȡ�Ƿ����
	 *  @param[in]  ucChair �����ȡ�����
	 *  @return  ������߷���TRUE,���򷵻�FALSE
	 */	
	BOOL IsOffline(BYTE ucChair);

	
  	/**
	 *  @brief SO�����й��������
	 *  @param[in]  ucChair �����йܵ����
	 *  @param[in]  shPlayerID ��ҵ�ID
	 */	
	virtual void SoTrustOffline(BYTE ucChair, short shPlayerID);

	/**
	 *  @brief SO�����й�����������
	 *  @param[in]  ucChair �����йܵ����
	 *  @param[in]  shPlayerID ��ҵ�ID
	 */	
	virtual void SoTrustOfflineGive(BYTE ucChair, short shPlayerID);

	/**
	 *  @brief SO�����й������������
	 *  @param[in]  ucChair �����йܵ����
	 *  @param[in]  shPlayerID ��ҵ�ID
	 */	
	virtual void SoTrustOfflineBlock(BYTE ucChair, short shPlayerID);

	/**
	 *  @brief SO�����й�������鲹��
	 *  @param[in]  ucChair �����йܵ����
	 *  @param[in]  shPlayerID ��ҵ�ID
	 */	
	virtual void SoTrustOfflineFlower(BYTE ucChair, short shPlayerID);

	/**
	 *  @brief �����������
	 *  @param[in]  nChair Ҫ���õ����
	 *  @param[in]  bTing �Ƿ�����
	 */	
	virtual void SetTing( int nChair, BOOL bTing = TRUE );

   	/**
	 *  @brief ���Ƶ�ִ�л�ȡ��ҵ�����ȼ�
	 *  @param[in]  nChair �������Ƶ����
	 *  @param[in]  ucFlag ���������
	 *  @param[in]  ucTile �������
	 */	
	virtual int GetRequestLevel( int nChair, BYTE ucFlag, BYTE ucTile );

   	/**
	 *  @brief ���Ƶ��ж��Ƿ���Ҫ�ȴ�����������
	 *  @param[in]  nChair �������Ƶ����
	 *  @param[in]  ucFlag ���������
	 *  @param[in]  ucTile �������
	 *  @param[out]  nMaxLevel ����ĵȼ�
	 */	
	virtual int IsWaitOtherRequest( int nChair, BYTE ucFlag, BYTE ucTile, int &nMaxLevel );

   	/**
	 *  @brief ���Ƶ�ִ�������������
	 *  @param[in]  nMaxLevel ����ĵȼ�
	 *  @param[out]  cmd ִ�к�Ľ��
	 */	
	virtual int SwitchBlockProcess( int nMaxLevel, TCMD_NOTIFY_BLOCK_RESULT &cmd );
	
   	/**
	 *  @brief ���Ƶ�ִ��ÿһ���������
	 *  @param[in]  nNextChair ��������
	 *  @param[in]  ucRequestTile �������
	 *  @param[out]  cmd ִ�к�Ľ��
	 */	
	virtual int SwitchBlockProcessOne( int nNextChair, BYTE ucRequestTile[4], TCMD_NOTIFY_BLOCK_RESULT &cmd );

   	/**
	 *  @brief ���Ƶ�ִ������
	 *  @param[in]  nNextChair �������Ƶ����
	 *  @param[in]  nNextChair �������Ƶ���(��һ�������һ��)
	 *  @param[in]  cmd ���Ƶ���Ϣ
	 */	
	virtual int SwitchBlockTing( int nChair, BYTE ucFlag, BYTE ucTile );

  	/**
	 *  @brief ���ƺ����Ƶ��ж�
	 *  @param[in]  nChair ���Ƶ���
	 */	
	virtual int CheckDrawBlock( int nChair );

	virtual int GetDrawWallBackNbr() { return GetAllGangNbr(); };

protected:
	// ����������Ƶ�
	int m_nBlockRequest[nPlayerNbr];
	// ��������
	TILE m_ucRequestTile[nPlayerNbr][4];
    // ���Ƽ�¼
    BOOL m_bTing[nPlayerNbr];
	// �������������Ƶļ�¼��ʽ�ǣ���¼ÿ�Ż���
	BYTE m_nFlower[MJ_FLOWER_NBR];
	// ����������
	BYTE m_ucEndType;
};
#else

/**
 * @ingroup MJFrame
 * @brief CGameTilePlayerMgr
 *
 * <p>�� 
 *		CGameTilePlayerMgr, �ͻ������ƿ�����Ļ���
 * </p>
 * @sa
 */	
template<int nPlayerNbr>
class CGameTilePlayerMgr : public CBaseTilePlayerMgr<nPlayerNbr>
{
public:
	CGameTilePlayerMgr();
	virtual ~CGameTilePlayerMgr();

  	/**
	 *  @brief ��ȡ��ɫ����ӿ�
	 *  @return  ��ɫ����ӿ�
	 *  @warning �˺����������ⲿ�̳в����أ�����ֻ����NULL
	 */	
	virtual IUserManager* GetSoUserManager() { return NULL; }

  	/**
	 *  @brief ��ȡUI����ӿ�
	 *  @return  UI����ӿ�
	 *  @warning �˺����������ⲿ�̳в����أ�����ֻ����NULL
	 */	
	//virtual IUIManager* GetUIManager() { return NULL; }


  	/**
	 *  @brief ˢ�������齫����
     *  @param[in]  bEnforce ǿ��ˢ��
	 *  ���߼����֣����ڸ����齫�ؼ����ݺ󣬵��ô˺�����ˢ��UI���֣���Ҫ����
	 */	
	virtual void RefreshMJCtrl(BOOL bEnforce = FALSE, BOOL bAllRefres = FALSE) = 0;

	
  	/**
	 *  @brief ��ȡ�Ƿ�����ʱ�����
	 *  @return  �������ʱ�룬����TRUE�����򷵻�FALSE
	 */	
	virtual BOOL GetAntiClockWise() { return TRUE; }

  	/**
	 *  @brief ��ȡIGameProcess�ӿ�
	 *  @return  IGameProcess�ӿ�
	 *  @warning �˺����������ⲿ�̳в����أ�����ֻ����NULL
	 */	
	virtual IGameProcess* GetGameProcess() { return NULL; }

	virtual void  OnGameMessage(UINT dwLen,  LPBYTE  pGameMsg);

  	/**
	 *  @brief ��Ϸ��ʼʱִ�е��¼�
	 */	
	virtual void  OnGameStart();

  	/**
	 *  @brief ���ܵ�������Ϣ��ִ�е��¼�
	 */	
	virtual void OnNotifyFlower(UINT nLen, BYTE *pBuf);

	/**
	 *  @brief ���ܵ�ȫ�����������Ϣ��ִ�е��¼�
	 *
	 *  ����Ϣ��־��SO�뿪����Ϸǰ�����׶ζ�������Ϸ�еĳ��ƻ����ƽ׶�
	 */	
	virtual void OnNotifyFlowerOver(UINT nLen, BYTE *pBuf);


  	/**
	 *  @brief ���ܵ��������ƽ����Ϣ��ִ�е��¼�
	 */	
	virtual void OnNotifyBlock(UINT nLen, BYTE *pBuf);

   	/**
	 *  @brief ���ܵ��������ƽ����Ϣ��ִ�еĳ����¼�
	 *  @param[in]  pCmd ������Ϣ
	 *  @param[in]  nOldTurn ����ʱ�Ĳ������
	 */	
	virtual void OnNotifyBlockCollect( TCMD_NOTIFY_BLOCK_RESULT *pCmd, int nOldTurn );

   	/**
	 *  @brief ���ܵ��������ƽ����Ϣ��ִ�е������¼�
	 *  @param[in]  pCmd ������Ϣ
	 *  @param[in]  nOldTurn ����ʱ�Ĳ������
	 */	
	virtual void OnNotifyBlockTriplet( TCMD_NOTIFY_BLOCK_RESULT *pCmd, int nOldTurn );

   	/**
	 *  @brief ���ܵ��������ƽ����Ϣ��ִ�е������¼�
	 *  @param[in]  pCmd ������Ϣ
	 *  @param[in]  nOldTurn ����ʱ�Ĳ������
	 */	
	virtual void OnNotifyBlockQuadrupletRevealed( TCMD_NOTIFY_BLOCK_RESULT *pCmd, int nOldTurn );

   	/**
	 *  @brief ���ܵ��������ƽ����Ϣ��ִ�еİ����¼�
	 *  @param[in]  pCmd ������Ϣ
	 *  @param[in]  nOldTurn ����ʱ�Ĳ������
	 */	
	virtual void OnNotifyBlockQuadrupletConcealed( TCMD_NOTIFY_BLOCK_RESULT *pCmd, int nOldTurn );

   	/**
	 *  @brief ���ܵ��������ƽ����Ϣ��ִ�е�ͣ���¼�
	 *  @param[in]  pCmd ������Ϣ
	 *  @param[in]  nOldTurn ����ʱ�Ĳ������
	 */	
	virtual void OnNotifyBlockTing( TCMD_NOTIFY_BLOCK_RESULT *pCmd, int nOldTurn );

	/**
	 *  @brief ���ܵ�������ƽ����Ϣ��ִ�е��¼�
	 */	
	virtual void OnNotifyGive(UINT nLen, BYTE *pBuf);

  	/**
	 *  @brief ���ܵ�������Ϣ��ִ�е��¼�
	 */	
	virtual void OnNotifyDraw(UINT nLen, BYTE *pBuf);

  	/**
	 *  @brief ���ܵ����ַ�����Ϣ��ִ�е��¼�
	 */	
	virtual void OnNotifyDeal(UINT nLen, BYTE *pBuf);

  	/**
	 *  @brief ���ܵ�ˢ��������Ϣ��ִ�е��¼�
	 */	
	virtual void OnNotifyUpdateHandTiles( UINT nLen, BYTE *pBuf );

  	/**
	 *  @brief ���ܵ��޸��Թ�״̬��Ϣ��ִ�е��¼�
	 */	
	virtual void OnNotifyChangeLookOn( UINT nLen, BYTE *pBuf );

  	/**
	 *  @brief ���ܳ���
	 *  @param[in]  ucTile ����Ҫ������
	 *  @param[in]  nSel   ���ĵڼ���λ�õ���
	 *
	 *  ���ػ��ڷ���������Ϣǰ�������Լ����������
	 */	
	virtual void RequestGive( TILE ucTile, int nSel );

  	/**
	 *  @brief ��������
	 *  @param[in]  ucFlag �������Ƶı��
	 *  @param[in]  ucTile ����Ҫ������
	 */	
	virtual void RequestBlock( BYTE ucFlag, TILE ucTile );

  	/**
	 *  @brief ���󲹻�
	 *  @param[in]  nChair ��������
	 */	
	virtual void RequestFlower( int nChair );

  	/**
	 *  @brief �����������Ƶ�ѡ��״̬
	 *  @param[in]  bCan �Ƿ��ѡ
	 *
	 *  �������ѡ�����ƻ�һ�
	 */	
	virtual void SetAllCanSelect( BOOL bCan = TRUE );

  	/**
	 *  @brief ����һ�����Ƶ�ѡ��״̬
	 *  @param[in]  pTilesSelect ���Ƶ�����
	 *  @param[in]  nLength ���Ƶ�����ĳ���
	 */	
	virtual void SetTilesSelect( int* pTilesSelect, int nLength );

  	/**
	 *  @brief ����һ�����Ƶ�ѡ��״̬
	 *  @param[in]  nIndex ���Ƶ����к�
	 *  @param[in]  nSelect ���Ƶ�ѡ��״̬0��ѡ��1����ѡ��2�Ѿ���ѡ��
	 */	
	virtual void SetTileSelect( int nIndex, int nSelect );

  	/**
	 *  @brief ���ÿ�ѡ����
	 *  @return ����б�ѡ���Ʒ��£�����TRUE�����򷵻�FALSE
	 *
	 *  ���ÿ�ѡ���ƣ�ȫ�����ڿ�ѡ״̬����ѡ����Ʒ���
	 */	
	virtual BOOL SetCanBeSelect();

  	/**
	 *  @brief ��ȡ�����Ʊ�ѡ����
	 *  @return ѡ���Ƶ����к�
	 */	
	virtual int  GetCanBeSelect();

  	/**
	 *  @brief ��ȡ�Ƶ�ѡ����Ϣ
	 *  @param[in]  nIndex ���Ƶ����к�
	 *  @return ѡ���Ƶ�״̬
	 */	
	virtual int  GetTileSelect( int nIndex );

  	/**
	 *  @brief ��ȡ��ǰ���Ƶ�ѡ��״̬
	 *  @return ���Ƶ�ѡ��״̬
	 *  @as MJ_STATE_SELECT_NONE
	 */	
	virtual int  GetStateSelect() { return m_nStateSelect; }

  	/**
	 *  @brief ���õ�ǰ���Ƶ�ѡ��״̬
	 *  @param[in]  nState ���Ƶ�ѡ��״̬
	 *  @as MJ_STATE_SELECT_NONE
	 */	
	virtual void SetStateSelect( int nState );

  	/**
	 *  @brief ��ȡ�Լ�������
	 *  @param[out]  mjHand ���Ƶ�����
	 */	
	virtual void GetLocalMJHand( CMJHand& mjHand );

  	/**
	 *  @brief ��ȡ�Լ�������
	 *  @param[out]  mjSet ���Ƶ�����
	 */	
	virtual void GetLocalMJSet( CMJSet& mjSet );

  	/**
	 *  @brief ��ȡ�Լ��ĳ���
	 *  @param[out]  mjGive ���Ƶ�����
	 */	
	virtual void GetLocalMJGive( CMJGive& mjGive );

  	/**
	 *  @brief ��ȡ�Լ���λ��ID
	 *  @return �Լ���λ�ú�
	 */	
	virtual int  GetLocalChair();

  	/**
	 *  @brief ��ȡ�Լ����������
	 *  @return �Լ����������
	 */	
	virtual TILE GetLocalGive() { return m_ucLocalGive; }

  	/**
	 *  @brief �����Լ����������
	 *  @param[in]  ucTIle ���������
	 */	
	virtual void SetLocalGive( TILE ucTIle );

  	/**
	 *  @brief �������Ƶ�״̬
	 *  @param[in]  nIndex ���Ƶ����к�(0~4,����������)
	 *  @param[in]  bEnble �Ƿ��ѡ
	 */	
	virtual void SetBlockEnable( int nIndex, BOOL bEnble );

  	/**
	 *  @brief ��ȡ���Ƶ�״̬
	 *  @return ���Ƶ����к�
	 */	
	virtual BOOL GetBlockEnable( int nIndex );

  	/**
	 *  @brief ��ȡ��ʾλ��
	 *  @return ��ʾ�ķ�λ
	 */	
	virtual int GetShowTurn() { return 0; }

  	/**
	 *  @brief ������ʾλ��
	 *  @param[in]  nTurn ��ʾ�ķ�λ
	 */	
	virtual void SetShowTurn( int nTurn ) {};

  	/**
	 *  @brief ����ʱ�䵽�˺�ִ�е��¼�
	 *  @param[in]  bAutoWin �Ƿ��Զ�����
	 */	
	virtual void OnTimeOutBlock( BOOL bAutoWin = TRUE );

  	/**
	 *  @brief ����ʱ�䵽�˺�ִ�е��¼�
	 */	
	virtual void OnTimeOutGive();
	

  	/**
	 *  @brief �������������Ϣ
	 *  @param[in]  ucChair ���ID
	 */	
	void ClearPlayerTiles( BYTE ucChair );

  	/**
	 *  @brief ˢ�����и��ƻ���
	 */	
	virtual void RefreshGangScore();

  	/**
	 *  @brief ��ȡ�ܷ�
	 *  @param[in]  ucChair ���ID
	 *  @return  ���Ƶĵ÷�
	 */	
	int GetGangScore( BYTE ucChair );

  	/**
	 *  @brief �����Ƿ������
	 *  @return  �������ҷ���TRUE���Թ��߷���FALSE
	 */	
	BOOL CheckIsPlayer();

  	/**
	 *  @brief ��ȡ�Ƿ�����Թ�����
	 *  @return  ����Ƿ���TRUE�����򷵻�FALSE
	 */	
	BOOL GetBLookOn() { return m_bAllowLookOn; }
	
	/**
	 *  @brief ����ѡ��״̬Ϊѡ�����
	 */	
	virtual void OnSetSelectStateColloect();

	/**
	 *  @brief ����ѡ��״̬Ϊѡ�����
	 */	
	virtual void OnSetSelectStateQuadruplet();

	/**
	 *  @brief ����ѡ��״̬Ϊѡ������
	 */	
	virtual void OnSetSelectStateTing();

	/**
	 *  @brief ����ѡ��״̬Ϊѡ�����
	 */	
	virtual void OnSetSelectStateGive();

	/**
	 *  @brief ��ȡ����Ƶ�������Ϣ
	 *  @param[in]  ucSelTile �������
	 *  @param[out]  mjTilesTing	������
	 *  @return   �Ƿ������
	 *
	 *  �������ֻ��¼��18�ţ�������Щ������Ժ�34�ŵģ�����û��¼��ô��
	 */	
	virtual BOOL GetTingGiveWinTiles( TILE ucSelTile, CMJHand &mjTilesTing );

    // ���뼶��ʱ��
    virtual void OnMSTimer(int nTimerID);

protected:
	// 0 ������ѡ��1 ����ѡ�� 2 �Ѿ���ѡ
	int m_nTileSelect[MAX_HAND_TILES];

	// ������Ʋ�����״̬
	int m_nStateSelect;

	// ���س�����
	// ���ȱ��ֵ��ƣ�һֱ�����ܵ�������Ϣ�Ż��0
	TILE m_ucLocalGive;

	int m_nGangScore[nPlayerNbr];
	BOOL m_bAllowLookOn;
};

#endif

}

#include "BaseTilePlayerMgr.inl"

#endif // !defined(AFX_BASETILEPLAYERMGR_H__E131B1E3_44E0_4A64_B7AC_BEA3D103C999__INCLUDED_)
