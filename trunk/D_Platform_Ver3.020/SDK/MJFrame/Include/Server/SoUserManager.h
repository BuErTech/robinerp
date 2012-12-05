#ifndef __SO_USERMANAGER__
#define __SO_USERMANAGER__

#include "afxwin.h"
#include "SoUserManager_i.h"
#include "SoFireEvent_t.h"
#include "../../Include/ServerCommon/UserInfo.h"

namespace MyGame
{
	class SoPlayer : public ISoPlayer
	{
    public:
        SoPlayer();
        ~SoPlayer();
        void Initialize(CGameUserInfo* pUserInfo);

        // ������ID
		UINT GetUserID();
		UINT GetUserAge();
		UINT GetUserGender();
		UINT GetChairID();
		UINT GetUserStatus();
		UINT GetFaceID();                   //��ʱû��ʵ��FACE ID��
		int  GetRightLevel();
        int  GetScore(); 
        int  GetMoney(); 
     
		// nSizeΪ��lpszNickName����Ŀռ�
		void  GetNickName(char *lpszNickName, int nSize);
		char * GetNickName();
        BOOL  IsReady();
		BOOL  IsVIP() ;
		BOOL  IsPlayer();
		BOOL  IsOffline()  ;
        BOOL  IsForceExit();
        void  SetForceExit(BOOL bForceExit);
        
        // ISoPlayer 
		void SetTeam(ENM_PLAYER_TEAM enmTeam);
		ENM_PLAYER_TEAM GetTeam();

		// �����Թ�
		void SetBCanBeLookedOn(BOOL bCanBeLookedOn);
		BOOL GetBCanBeLookedOn();
		BOOL IsLockedMoney();

		// ������ҵ�Ǯ
		void SetPlayerLockMoneyFlag(BOOL bLocked);

		//�����й�
		BOOL GetTrusteeStatusVal();

		void SetTrusteeStatusVal(BOOL bTrue);

		void SetPlayerLockHappyBeanFlag(BOOL bLocked);

		BOOL IsLockedHappyBean();

        void ClearState();

    private:
        CGameUserInfo*  m_pUserInfo;
        char           m_bIfCanLook;
        char           m_bLockedMoney;
        char           m_bLockedHappyBean;
        char           m_bTrustee;             // �Ϲ�״̬
        char           m_bForceExit;
	};

    //Ϊ�˽ӿ�һ����ʹ��SoWatcher ������ SoPlayer ˵��һ����ֻ����ԭ���ķ������屣��һ��
	class SoWatcher : public ISoWatcher
	{
    public:
        SoWatcher();
        ~SoWatcher();
	};

    class CDeskLogicImpl;
	class SoUserManager : public TSoEventProducerImpl<ISoUserManager, 5>
	{
    public:
		SoUserManager(CDeskLogicImpl* pDeskLogicImpl);
		~SoUserManager(void);
	public:
		//��ȡ���������
		/**
		*  @brief ��ȡ���������
		*  @param bExitAtPlay TRUE��Ϸ���˳������Ҳ���㣬�����෴
		*  @sa 
		*	@return ���������
		*/	
		UINT GetTeamCount(BOOL bExitAtPlay /*= FALSE*/);   //���ǲ�ʵ��

		/**
		*  @brief ����PlayerID��ȡ�û�ISoUserָ��
		*  @param shPlayerID �û���PlayerId
		*  @sa ISoUser
		*	@return �û���ISoUser�ӿ�ָ�룬���п�
		*/	
		ISoUser* GetUserByID(UINT shPlayerID);

		/**
		*  @brief ��õ�ǰ��ҵ�����
		*	@return ��ǰ��ҵ�����
		*/	
		UINT GetPlayerCount();

		/**
		*  @brief ����PlayerID��ȡ���ISoPlayerָ��
		*  @param shPlayerID ��ҵ�PlayerId
		*  @sa ISoPlayer
		*	@return ��ҵ�ISoPlayer�ӿ�ָ�룬���п�
		*/	
		ISoPlayer* GetPlayerByID(UINT shPlayerID);

		/**
		*  @brief �������ChairId��ȡ���ISoPlayerָ��
		*  @param unUin ���ChairId
		*  @sa ISoPlayer
		*	@return ��ҵ�ISoPlayer�ӿ�ָ�룬���п�
		*/
		ISoPlayer* GetPlayerByChair(UINT uChair);

		/**
		*  @brief �ж�һ���û��Ƿ������
		*  @sa 
		*  @return TRUE��ʾ�ǣ�FALSE��ʾ����
		*/
		BOOL IsPlayer(UINT uPlayerID);

		/**
		*  @brief ����һ������Ƿ����й�״̬
		*  @param cChair ��ҵ�ChairID
		*  @param bTrustee �Ƿ����й�״̬
		*  @sa 
		*/
		void SetPlayerTrusteeStatus(UINT uChair, BOOL bTrustee);

		/**
		*  @brief �ж�һ���û��Ƿ����й�״̬
		*  @param cChair ��ҵ�ChairID
		*  @sa 
		*  @return TRUE��ʾ�ǣ�FALSE��ʾ����
		*/
		BOOL GetPlayerTrusteeStatus(UINT cChair);

        /**
        *  @brief  ������Ϸ��������
        *  @param nDealer ׯ��λ��
        *  @param byWin   Ӯ��λ��(֧��һ�ڶ���)
        *  @param arPoint ÿ�������Ҫ����ĵ���
        *  @param rePoint �����ĵ���
        *  @return ����ֵ������
        */
        virtual STATUS SetGameScore(int nDealer, BYTE *byWin, int *arPoint, int *rePoint);

		/**
		*  @brief ��ȡĳ���û�����Ϸ������
		*  @param nPlayerID �û���PlayerID
		*  @param piMoney[out]      �����Ϸ������int�͵ĵ�ַ
		*  @param bMinusTicket      ��ȡ����Ϸ���Ƿ��Ѽ�ȥ��Ʊ
		*	@return ����ֵ������
		*/
		int GetMoney(UINT nPlayerID, int* piMoney, BOOL bMinusTicket = TRUE);   // �����Ʊ������û��ʵ��

		/**
		*  @brief ��ȡ��������ͣ���Ϸ�ҷ��䡢���ֶ����仹����ͨ����
		*	@return ���ط��������
		*  @sa ENM_ROOM_TYPE
		*/
		int GetRoomType();                              //�ӿ������ţ�û��ʵ��

    private:
        CDeskLogicImpl* m_pDeskLogicImpl;
        UINT            m_cuChairIndex;
        UINT            m_cuWatcherIndex;
	};

};

#endif
