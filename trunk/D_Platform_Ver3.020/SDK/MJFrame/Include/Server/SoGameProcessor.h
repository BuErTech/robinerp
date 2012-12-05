#ifndef __GAME_PROCESSOR__
#define __GAME_PROCESSOR__

#include "afxwin.h"
#include "typeDef.h"
#include "SoGameProcessor_i.h"
#include "SoFireEvent_t.h"

namespace MyGame
{
class CDeskLogicImpl;
class GameProcessor : public TSoEventProducerImpl<ISoGameProcessor, 5>
{
public:
	GameProcessor(CDeskLogicImpl* pDeskLogicImpl);

	~GameProcessor();

	/** 
	 *  @brief ������Ϸ��ʼģʽ
	 *  @param [in] byMode 0: ���˲ſ�ʼ   1: ������ͬ��Ϳ�ʼ   2: �Գ�ͬ�⿪ʼ   3: 6����λ��ֻҪ��4����ͬ�⿪ʼ
	 */
	virtual void SetStartMode(BYTE byMode);

	/**
	 *  @brief �����Ϸ��״̬
	 *	@return ����ֵΪGAME_STATUSö���е�ֵ
	 *  @sa GAME_STATUS
	 */	
	virtual int GetGameState();

	//////////////////////////////////////////////////////////////////////////
	/**
	 *  @brief ������Ϸ��ʼ��gamesdk����sogamelogic��Ϸ���Կ�ʼ֮��sogamelogic���������Ϸ���Կ�ʼ�͵��ô˽ӿڣ�gamesdk���ڴ˽ӿ��������mainsvr�Ŀ�ʼ
	 *  @sa
	 */	
	virtual void SetGameStart();

	//������Ϸ����
	/**
	 *  @brief ������Ϸ����sogamelogic���������Ϸ���Խ����͵��ô˽ӿڣ�gamesdk���ڴ˽ӿ��������mainsvr�Ľ���
	 *  @param nGameEndType GAME_END_TYPEö��ֵ
	 *	@return ����ֵ��������
	 *  @sa GAME_END_TYPE
	 */	
	virtual BOOL SetGameEnd(int nGameEndType);

	//////////////////////////////////////////////////////////////////////////
	/**
	 *  @brief �����������ҷ�����Ϸ����
	 *	@return ����ֵ��������
	 *  @sa
	 */	
	virtual int SendGameData(char * pcGameData, UINT nDataLen,  UINT uiFlag );

	/**
	 *  @brief  �����nUserID��ҷ�����Ϸ����
	 *	@return ����ֵ��������
	 *  @sa
	 */
	virtual int SendGameDataTo(UINT nPlayerID, char * pcGameData, UINT nDataLen, UINT uiFlag );

    /**
    *  @brief  �����nSeatID��ҷ�����Ϸ����
    *  @return ����ֵ��������
    *  @sa
    */
    virtual int SendGameDataTo(int nSeatID, char * pcGameData, UINT nDataLen, UINT uiFlag );

	/**
	 *  @brief �����ָ����λ����Ϸ�ߺ��Թ��߷�����Ϸ����
	 *	@return ����ֵ��������
	 *  @sa
	 */
	virtual int SendGameDataToSeat(UINT nSeatID, char* pcGameData, UINT nDataLen, UINT uiFlag);

	/**
	 *  @brief ���������ָ����λ������������Ϸ�ߺ��Թ��߷�����Ϸ����
	 *	@return ����ֵ��������
	 *  @sa
	 */
	virtual int SendGameDataExceptSeat(UINT nSeatID, char* pcGameData, UINT nDataLen, UINT uiFlag);
	
	/**
	 *  @brief ���������nUserID��������ҷ�����Ϸ����
	 *	@return ����ֵ��������
	 *  @sa
	 */
	virtual int SendGameDataExcept(UINT nPlayerID, char* pcGameData, UINT nDataLen, UINT uiFlag);

	/**
	 *  @brief �����nChairλ���ϵ����йۿ��߷�����Ϸ����
	 *	@return ����ֵ��������
	 *  @sa
	 */
	virtual int SendGameDataToLookOnUsers(UINT nChair, char* pcGameData, UINT nDataLen, UINT uiFlag);

	/**
	 *  @brief ��ȡ������Ϣ
	 *	@return ������
	 *  @sa
	 */
	virtual int GetGameContinueRounds();

	/**
	 *  @brief ���ó�����������Ϣ������֮��gamesdk���Զ��Ϳͻ��˵�ͬ��
	 *	@return ����ֵ��������
	 *  @sa
	 */
	virtual int SetGameContinueRounds(UINT nRound);
	
	/**
	 *  @brief ����һ����ʱ��,��ʱ����ID��nTimerID, ��ʱ����ʱ��ΪnTime
	 *  @param[in] nTimerID ��ʱ����ID
	 *  @param[in] nTime	��ʱ����ʱ��(ms��Ϊ��λ)
	 */	
	virtual void  SetTimer(UINT nTimerID, int nTime);

	/**
	 *  @brief ֹͣ��ʱ��
     *  @param[in] nTimerID ��ʱ����ID
	 */	
	virtual void KillTimer(UINT nTimerID);

    /**
	 *  @brief ����һ����ʱ���ص������齫
	 */
	virtual int AddTimerCall(ISoTimerCall* pTimerCall);

    /**
    *  @brief ��ȡ����ID
    */
    virtual UINT GetRoomID();

	/**
    *  @brief ��ȡ����˰�ձ���
    */
    virtual UINT GetRoomTax();

    /**
    *  @brief ��ȡ�����ע
    */
    virtual UINT GetBaseMoney();

    /**
    *  @brief ��ȡ�������ٽ������
    */
    virtual UINT GetMinLimitMoney();
public:
	/**
	 *  @brief �붨ʱ���ĵ��÷������̶�����������ֻ��һ���붨ʱ��
	 */
    void OnMillTimer();

private:
    CDeskLogicImpl*  m_pDeskLogicImpl;
    ISoTimerCall*    m_pAddTimerCall;

public:
    UINT             m_GameBeginTimes; // ��Ϸ�ѽ����˶೤ʱ��

};

}

#endif
