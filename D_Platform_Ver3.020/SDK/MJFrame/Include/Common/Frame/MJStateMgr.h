/** @file	MJStateMgr.h
 *	@brief	�齫״̬������
 *	@author edward
 *	@date 08.08.04
 */
#ifndef ___AFX_MJSTATEMGR_H__52351F89_A913_425E_89A1_53CFDA134643__INCLUDED___
#define ___AFX_MJSTATEMGR_H__52351F89_A913_425E_89A1_53CFDA134643__INCLUDED___

#include "IMJStateEvent.h"
#include "BaseTileDealerMgr.h"

namespace MyGame
{ 
/**
 * @ingroup MJFrame
 * @brief CMJStateMgr
 *
 * <p>�� 
 *		CMJStateMgr, �齫״̬�������
 * </p>
 * @sa
 */	
template<int nPlayerNbr>
class CMJStateMgr : public IMJStateEvent,
					#ifndef WINDOWS_LOGIC
					public CSoTileDealerMgr<PLAYER_NBR>
					#else
					public CGameTileDealerMgr<nPlayerNbr>
					#endif
{
public:
	CMJStateMgr();
	virtual ~CMJStateMgr();

    /**
    *  @brief ����״̬
    *  @param[in]  nState    ���õ�״̬
    *  @param[in]  bTimelag  TRUE: ��ʱ����  FALSE: ����ʱ
    *
    *  ��������״̬������״ִ̬����صĲ���
    */	
    virtual void SetMJState( int nState, BOOL bTimelag = FALSE );

	/**
	 *  @brief ��ȡ��ǰ���齫״̬
	 *  @return  �齫״̬
	 */	
	virtual int GetMJState() { return m_nGameState; }

protected:
	int m_nGameState;
};

/**
 * @ingroup MJFrame
 * @brief CMJSoStateMgr
 *
 * <p>�� 
 *		CMJSoStateMgr, SO�齫״̬�������
 * </p>
 * @sa
 */	
#ifndef WINDOWS_LOGIC
template<int nPlayerNbr>
class CMJSoStateMgr : public CMJStateMgr<PLAYER_NBR>
{
public:
	CMJSoStateMgr();
	virtual ~CMJSoStateMgr();

    /**
    *  @brief ����״̬
    *  @param[in]  nState    ���õ�״̬
    *  @param[in]  bTimelag  TRUE: ��ʱ����  FALSE: ����ʱ
    *
    *  ��������״̬������״ִ̬����صĲ���
    */	
    virtual void SetMJState( int nState, BOOL bTimelag = FALSE );

	virtual int OnGameMessage(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen);

	/**
	 *  @brief ���뿪ʼʱ�ķ���״̬
	 *  @param[in]  nOldState ��һ��״̬
	 */	
	virtual void OnSetStateDeal( int nOldState );

	/**
	 *  @brief ��������״̬
	 *  @param[in]  nOldState ��һ��״̬
	 */	
	virtual void OnSetStateDraw( int nOldState );

	/**
	 *  @brief ��������״̬
	 *  @param[in]  nOldState ��һ��״̬
	 */	
	virtual void OnSetStateBlock( int nOldState );
	
	/**
	 *  @brief �������״̬
	 *  @param[in]  nOldState ��һ��״̬
	 */	
	virtual void OnSetStateEnd( int nOldState );

	/**
	 *  @brief �������״̬
	 *  @param[in]  nOldState ��һ��״̬
	 */	
	virtual void OnSetStateGive( int nOldState );

	/**
	 *  @brief ���뿪ʼǰ����״̬
	 *  @param[in]  nOldState ��һ��״̬
	 *
	 *  ��״̬���Բ�һ���������������״̬���ص���Ҫ�ȴ�������ҷ�����Ϣ�ٽ�����Ϸ��״̬
	 */	
	virtual void OnSetStateFlower( int nOldState );

	/**
	 *  @brief ������Ϸ����������
	 *  @param[in]  eGameEndType ����������
	 */	
	virtual void SetGameLogicEnd(GAME_END_TYPE eGameEndType) {};

    // ���뼶��ʱ��
    virtual void OnMSTimer(int nTimerID);

protected:
	/**
	 *  @brief ��ʱʱ�䵽������ʱ��
	 */	
	virtual void OnTimeOut( int nTimerID );

};

/**
 * @ingroup MJFrame
 * @brief CMJGameStateMgr
 *
 * <p>�� 
 *		CMJGameStateMgr, C�齫״̬�������
 * </p>
 * @sa
 */	
#else

template<int nPlayerNbr>
class CMJGameStateMgr : public CMJStateMgr<nPlayerNbr>
{
public:
	CMJGameStateMgr();
	virtual ~CMJGameStateMgr();

	/**
	 *  @brief ����״̬
	 *  @param[in]  nState    ���õ�״̬
     *  @param[in]  bTimelag  TRUE: ��ʱ����  FALSE: ����ʱ
	 *
	 *  ��������״̬������״ִ̬����صĲ���
	 */	
	virtual void SetMJState( int nState, BOOL bTimelag = FALSE );

	/**
	 *  @brief �������״̬
	 *  @param[in]  nOldState ��һ��״̬
	 */	
	virtual void OnSetStateGive( int nOldState );
	
	/**
	 *  @brief ����ȴ�״̬
	 *  @param[in]  nOldState ��һ��״̬
	 *
	 *  ��״̬����ҵ�ʱ��һֱѭ��
	 */	
	virtual void OnSetStateWait( int nOldState );

	/**
	 *  @brief �������״̬
	 *  @param[in]  nOldState ��һ��״̬
	 */	
	virtual void OnSetStateEnd( int nOldState );

	/**
	 *  @brief ���뿪ʼǰ����״̬
	 *  @param[in]  nOldState ��һ��״̬
	 *  
	 *  ��״̬���Բ�һ���������������״̬���ص���Ҫ�ȴ�������ҷ�����Ϣ�ٽ�����Ϸ��״̬
	 */	
	virtual void OnSetStateFlower( int nOldState );

	/**
	 *  @brief ��������״̬
	 *  @param[in]  nOldState ��һ��״̬
	 */	
	virtual void OnSetStateBlock( int nOldState );

	virtual void OnGameMessage(UINT dwLen,  LPBYTE  pGameMsg);

	/**
	 *  @brief ����ʱ���������¼�����
	 *  @param[in]  dwGameTimerID ʱ���ID
	 */	
	virtual void OnGameTimer( UINT dwGameTimerID );

	/************************************************************************/
	/* 
	�߼�ʱ�䵽
	*/
	/************************************************************************/

	/**
	 *  @brief �߼�ʱ����¼�����
	 *  @param[in]  nLogicTimerID �߼�ʱ���ID
	 */	
	virtual void OnLogicTimeOut( int nLogicTimerID );

	
	/**
	 *  @brief �ȴ�ʱ�䵽���¼�����
	 *  @param[in]  nLogicTimerID �߼�ʱ���ID
	 */	
	virtual void OnTimeOutWait();

	/**
	 *  @brief ׼��ʱ�䵽���¼�����
	 *  @param[in]  nLogicTimerID �߼�ʱ���ID
	 */	
	virtual void OnTimeOutReady();

};

#endif

}

#include "MJStateMgr.inl"

#endif // !defined(AFX_MJSTATEMGR_H__52351F89_A913_425E_89A1_53CFDA134643__INCLUDED_)
