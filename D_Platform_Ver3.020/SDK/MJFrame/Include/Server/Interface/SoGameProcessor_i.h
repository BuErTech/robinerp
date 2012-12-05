#ifndef _GAMEPROCESSOREVENT_H
#define _GAMEPROCESSOREVENT_H

#include "Svrdef.h"
#include "gameevent_i.h"
#include "TimerCall_i.h"


namespace MyGame
{
    //!��Ϸ״̬������
    enum GAME_STATUS
    {
	    gsWait = 0,            /*!< �ȴ�״̬ */
	    gsCountDown,           /*!< ����ʱ״̬ */
	    gsAllPlayerReady,      /*!< ��������Ѿ�׼��״̬ */
	    gsPlaying              /*!< ��Ϸ������״̬ */
    };

	//!��Ϸ����������
	enum GAME_END_TYPE
	{
		enEndByGameOver = 10,  /*!< ��Ϸ��������, ��ֵ���ܴ�0��ʼ����, ƽ̨0Ϊǿ�� */
		enEndByPlayerEscape,   /*!< ������ܵ�����Ϸ���� */
		enEndByNetAdmin,	   /*!< ���ܽ�ɢ��Ϸ������Ϸ���� */
		enEndByArrange,		   /*!< Э���˳������� */
		enNotEndGame,		   /*!< �ͻ��˽�����Ϸ����������SetGameEnd(����ʱʹ��) */
	};

	/**
	 * @ingroup GameSDK
	 * @brief ISoGameProcessor
	 *
	 * <p>�� 
	 *		ISoGameProcessor �������Ϸ���̹���ģ��
	 * </p>
	 * @sa
	 */	
	class ISoGameProcessor : public IGameObject, public IEventProducer
	{
	public:
		/** 
		 *  @brief ������Ϸ��ʼģʽ
		 *  @param [in] byMode 0: ���˲ſ�ʼ   1: ������ͬ��Ϳ�ʼ   2: �Գ�ͬ�⿪ʼ   3: 6����λ��ֻҪ��4����ͬ�⿪ʼ
		 */
		virtual void SetStartMode(BYTE byMode) = 0;

		/**
		 *  @brief �����Ϸ��״̬
		 *	@return ����ֵΪGAME_STATUSö���е�ֵ       
		 *  @sa GAME_STATUS
		 */	
		virtual int GetGameState() = 0;

		//////////////////////////////////////////////////////////////////////////
		/**
		 *  @brief ������Ϸ��ʼ��gamesdk����sogamelogic��Ϸ���Կ�ʼ֮��sogamelogic���������Ϸ���Կ�ʼ�͵��ô˽ӿڣ�gamesdk���ڴ˽ӿ��������mainsvr�Ŀ�ʼ
		 *  @sa
		 */	
		virtual void SetGameStart() = 0;

		//������Ϸ����
		/**
		 *  @brief ������Ϸ����sogamelogic���������Ϸ���Խ����͵��ô˽ӿڣ�gamesdk���ڴ˽ӿ��������mainsvr�Ľ���
		 *  @param nGameEndType GAME_END_TYPEö��ֵ
		 *	@return ����ֵ��������
		 *  @sa GAME_END_TYPE
		 */	
		virtual BOOL SetGameEnd(int nGameEndType) = 0;

		//////////////////////////////////////////////////////////////////////////
		//�����������ҷ�����Ϸ����
		/**
		 *  @brief �����������ҷ�����Ϸ����
		 *	@return ����ֵ��������
		 *  @sa
		 */	
		virtual int SendGameData(char * pcGameData, UINT nDataLen,  UINT uiFlag = 0) = 0;

		//�����nUserID��ҷ�����Ϸ����
		/**
		 *  @brief �����������ҷ�����Ϸ����
		 *	@return ����ֵ��������
		 *  @sa
		 */
		virtual int SendGameDataTo(UINT nPlayerID,char * pcGameData, UINT nDataLen, UINT uiFlag = 0) = 0;

		//�����ָ����λ����Ϸ�ߺ��Թ��߷�����Ϸ����
		/**
		 *  @brief �����ָ����λ����Ϸ�ߺ��Թ��߷�����Ϸ����
		 *	@return ����ֵ��������
		 *  @sa
		 */
		virtual int SendGameDataToSeat(UINT nSeatID, char* pcGameData, UINT nDataLen, UINT uiFlag = 0) = 0;

		//���������ָ����λ������������Ϸ�ߺ��Թ��߷�����Ϸ����
		/**
		 *  @brief ���������ָ����λ������������Ϸ�ߺ��Թ��߷�����Ϸ����
		 *	@return ����ֵ��������
		 *  @sa
		 */
		virtual int SendGameDataExceptSeat(UINT nSeatID, char* pcGameData, UINT nDataLen, UINT uiFlag = 0) = 0;
	
		//���������nUserID��������ҷ�����Ϸ����
		/**
		 *  @brief ���������nUserID��������ҷ�����Ϸ����
		 *	@return ����ֵ��������
		 *  @sa
		 */
		virtual int SendGameDataExcept(UINT nPlayerID, char* pcGameData, UINT nDataLen, UINT uiFlag = 0) = 0;

		//�����nChairλ���ϵ����йۿ��߷�����Ϸ����
		/**
		 *  @brief �����nChairλ���ϵ����йۿ��߷�����Ϸ����
		 *	@return ����ֵ��������
		 *  @sa
		 */
		virtual int SendGameDataToLookOnUsers(UINT nChair, char* pcGameData, UINT nDataLen, UINT uiFlag = 0) = 0;
		

		//��ó�����Ϣ
		/**
		 *  @brief ��ȡ������Ϣ
		 *	@return ������
		 *  @sa
		 */
		virtual int GetGameContinueRounds() = 0;

		/**
		 *  @brief ���ó�����������Ϣ������֮��gamesdk���Զ��Ϳͻ��˵�ͬ��
		 *	@return ����ֵ��������
		 *  @sa
		 */
		virtual int SetGameContinueRounds(UINT nRound) = 0;

        /**
        *  @brief ����һ����ʱ��,��ʱ����ID��nTimerID, ��ʱ����ʱ��ΪnTime
        *  @param[in] nTimerID ��ʱ����ID
        *  @param[in] nTime	��ʱ����ʱ��(ms��Ϊ��λ)
        */	
        virtual void  SetTimer(UINT nTimerID, int nTime) = 0;

        /**
        *  @brief ֹͣ��ʱ��
        *  @param[in] nTimerID ��ʱ����ID
        */	
        virtual void KillTimer(UINT nTimerID) = 0;

		/**
		 *  @brief ����һ����ʱ���ص������齫
		 */
		virtual int   AddTimerCall(ISoTimerCall* pTimerCall) = 0;

        /**
	 	*  @brief ��ȡ����ID
	 	*/
		virtual UINT  GetRoomID() = 0;

		/**
		*  @brief ��ȡ����˰�ձ���
		*/
		virtual UINT GetRoomTax() = 0;

        /**
        *  @brief ��ȡ�����ע
        */
        virtual UINT  GetBaseMoney() = 0;

        /**
        *  @brief ��ȡ�������ٽ������
        */
        virtual UINT  GetMinLimitMoney() = 0;
	};
	

	/**
	 * @ingroup GameSDK
	 * @brief ISoGameProcessorEvent
	 *
	 * <p>�� 
	 *		ISoGameProcessorEvent ��Ϸ�����¼�
	 * </p>
	 * @sa
	 */	
	class ISoGameProcessorEvent : public IEvent
	{
	public:
		/**
		 *  @brief �յ���ҷ���GameMsg�����Լ������¼�������GameMsg
		 *  @param nMsgAssistantID Msg�ĸ���ϢID���������ڰ�����Ϣ�������ˣ����ô�����
		 *  @param nPlayerID ���ʹ�GameMsg�����PlayerId
		 *  @param nChair ���ʹ�GameMsg����ҵ�ChairId
		 *  @param pcGameData ��Ϣ��ṹ��ָ��
		 *  @param nDataLen ��Ϣ�ĳ���
		 *	@return ����ֵ��������
		 *  @sa
		 */
		virtual int OnGameMessage(UINT  nMsgAssistantID, UINT nPlayerID, UINT nChair,  void * pcGameData,  UINT nDataLen) = 0;

		/**
		 *  @brief �յ��Թ۷���GameMsg�����Լ������¼�����Թ�GameMsg
		 *  @param cMsgID Msg��ID
		 *  @param nPlayerID ���ʹ�GameMsg���Թ�PlayerId
		 *  @param nChair ���ʹ�GameMsg���Թ۵�ChairId
		 *  @param pcGameData ��Ϣ��ṹ��ָ��
		 *  @param nDataLen ��Ϣ�ĳ���
		 *	@return ����ֵ��������
		 *  @sa
		 */
		virtual int OnViewerMessage(UINT  nMsgAssistantID, UINT nPlayerID, UINT nChair,  void * pcGameData,  UINT nDataLen) = 0;
		//��Ϸ��ʼ����� ����Logicȥ���ã�����ֻ��֪ͨ������

		/**
		 *  @brief ���е���Ҷ��Ѿ�׼���õ��¼�
		 *	@return ����ֵ��������
		 *  @sa
		 */
		virtual int OnAllPlayerReady() = 0;
		
		/**
		 *  @brief ���¼�ÿ�봥��һ�Σ�����ֻ����Ϸ���й����д���
		 *	@return ����ֵ��������
		 *  @sa
		 */
		virtual int OnGameTimer() = 0;
		//
	/*	//lightchen ֱ�Ӽ���UserManager
		virtual int OnUserEnter(short shPlayerID) = 0;
		virtual int OnUserExit(short shPlayerID) = 0;
		virtual int OnUserOffline(short shPlayerID) = 0;
		virtual int OnUserReplay(short shPlayerID) = 0;*/
		
		//
//		virtual int OnAllPlayerAgreeEndGame() = 0;	//��������Ϸ������һ������

// 		virtual int LoadCfg(const char * pConfigFile)  = 0;
// 		virtual int ReloadCfg(const char* pConfigFile) = 0;
	};

	
	/**
	 * @ingroup GameSDK
	 * @brief ISoGameProcessorGameEvent
	 *
	 * <p>�� 
	 *		ISoGameProcessorGameEvent ��Ϸ������ʼ���¼�
	 * </p>
	 * @sa
	 */	
	class ISoGameProcessorGameEvent: public IEvent
	{
	public:
		/**
		 *  @brief ��Ϸ��ʼ�¼�
		 *	@return ����ֵ��������
		 *  @sa
		 */
		virtual int OnGameStart() = 0;

		//���ܽ�ɢ��Ϸ��Э���˳���
		/**
		 *  @brief ���ܽ�ɢ��Ϸ��Э���˳��¼���֪ͨ��gamesdk�������֪֮ͨǰ�Լ�֮�󲢲���������Ϸ��������Ҫgamelogic������֮�����gamesdk����Ϸ�����ӿ�
		 *  @param nGameEndType GAME_END_TYPEö��ֵ
		 *	@return ����ֵ��������
		 *  @sa nGameEndType
         */
	    /*            
        *enum GAME_END_TYPE{
		*enEndByGameOver = 0,	 ��Ϸ�������� 
		*enEndByPlayerEscape,	 ������ܵ�����Ϸ���� 
		*enEndByNetAdmin,		 ���ܽ�ɢ��Ϸ������Ϸ���� 
		*enEndByArrange,		Э���˳�����
		*enNotEndGame,			 �ͻ��˽�����Ϸ����������SetGameEnd(����ʱʹ��) 
	    };
        */

		virtual int OnGameEnd(int nGameEndType) = 0;

		/**
		 *  @brief ������Ϸ����ǰ�������������¼�֮�󣬻��뿪������Ϸ״̬�͵���mainsvr����Ϸ����
		 *  @param nGameEndType GAME_END_TYPEö��ֵ
		 *	@return ����ֵ��������
		 *  @sa nGameEndType
		 */
		virtual int OnBeforeGameEnd(int nGameEndType) = 0;

		/**
		 *  @brief ������Ϸ״̬�͵���mainsvr����Ϸ����֮�󴥷�
		 *  @param nGameEndType GAME_END_TYPEö��ֵ
		 *	@return ����ֵ��������
		 *  @sa nGameEndType
		 */
		virtual int OnPostGameEnd(int nGameEndType) = 0;
	};

	/**
	 * @ingroup GameSDK
	 * @brief ISoGameProcessorSendBaseInfoEvent
	 *
	 * <p>�� 
	 *		ISoGameProcessorSendBaseInfoEvent ��Ϸ�����¼��� ����ҽ���ʱ����֮
	 * </p>
	 * @sa
	 */	
	class ISoGameProcessorSendBaseInfoEvent : public IEvent
	{
	public:
//		virtual int OnSendBaseInfo(int nVer) = 0;
		/**
		 *  @brief ֪ͨ����ģ�����������Ϸ��baseinfo
		 *  @param shPlayerID ��ҵ�PlayerID
		 *	@return ����ֵ��������
		 *  @sa
		 */
		virtual int OnSendBaseInfo(UINT shPlayerID) = 0;

		/**
		 *  @brief ֪ͨ����ģ����ҵĿͻ��˰汾��
		 *  @param shPlayerID ��ҵ�PlayerID
		 *	@return ����ֵ��������
		 *  @sa
		 */
		virtual int OnNotityClientVer(int nVer) = 0;
	};

	/**
	 * @ingroup GameSDK
	 * @brief ISoGameProcessTimerEvent
	 *
	 * <p>�� 
	 *		ISoGameProcessTimerEvent SO��Timer������(Ŀǰֻ֧��һ��Timer)
	 * </p>
	 * @sa
	 */	
	class ISoGameProcessTimerEvent : public IEvent
	{
	public:
		/**OnCollectBaseMsg
		 *  @brief ��ʱ������ʱִ�еĺ���
		 *  @param[in] nTimerID          ��ʱ����ID  
		 */
		virtual int  OnTimer(int nTimerID) = 0;
	};

      /**
	 * @ingroup GameSDK
	 * @brief ISoGameProcess
	 *
	 * <p>�� 
	 *		ISoGameProcessBaseMsg SO�˴�����ҽ�������ߵ���Ϣ
	 * </p>
	 * @sa
	 */	
    class ISoGameProcessBaseMsgEvent : public IEvent
	{
	public:
		/**OnCollectBaseMsg
		 *  @brief �û��������ӣ��������ӵȵ���
		 *  @param[in] shPlayerID          �û��� ID  
		 */
		virtual int OnCollectBaseMsg(UINT uPlayerID) = 0;

    	/**OnCollectMsg
		 *  @brief ��ʱ������ʱִ�еĺ���
		 *  @param[in] nTimerID          ��ʱ����ID  
		 */
		virtual int OnCollectMsg(UINT uPlayerID) = 0;

	};

}

#endif //_GAMEPROCESSOREVENT_H
