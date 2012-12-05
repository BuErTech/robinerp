
#if !defined(SO_USER_MANAGER_I_H)
#define SO_USER_MANAGER_I_H

#include "Svrdef.h"
#include "gameobject_i.h"
#include "gameevent_i.h"
#include "playerManagerCommon.h"
#include "commonStructs.h"

namespace MyGame
{
#ifndef IPC_VERSION
//////////////////////////////////////////////////////////////////////////////
//���岿�֣� begin

	struct stRegionOption 
	{
		char m_langCode[3];
		char m_areaCode[3];
		int m_timeZone;
	};

	#define		MAX_NAME_LEN		20

	
	typedef struct
	{
		unsigned int m_iUin;				// ��ҵ�ID����
		short m_nPlayerID;  				// �����server�û��б�������
		short m_nChair;     				// ������ڵ����Ӻ�
		short m_nState;     				// ��ҵ�״̬
		short m_nFaceID;					// ��ҵ�ͼ����
		unsigned int m_uiRightLevel;		// �û�Ȩ��
		char  m_szUserName[MAX_NAME_LEN];	// �����
	} TABLE_USER_INFO;

//���岿��: end.
//////////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////////
//IPC ���� : begin

	#define		MAX_NAME_LEN		30

	struct stRegionOption 
	{
		char m_langCode[3];
		char m_areaCode[3];
		int m_timeZone;
	};
	
//	#ifndef __REGION_OPT__
//	#define __REGION_OPT__
//	#endif

	typedef struct
	{
		unsigned int m_iUin;				// ��ҵ�ID����
		short m_nPlayerID;  				// �����server�û��б�������
		short m_nChair;     				// ������ڵ����Ӻ�
		short m_nState;     				// ��ҵ�״̬
		short m_nFaceID;					// ��ҵ�ͼ����
		unsigned int m_uiRightLevel;		// �û�Ȩ��
		char  m_szUserName[MAX_NAME_LEN + 1];	// �����
// 	#ifdef __REGION_OPT__
		stRegionOption m_region;
// 	#endif

	} TABLE_USER_INFO;

//IPC ���� end.
//////////////////////////////////////////////////////////////////////////////
#endif

	//BEGIN modified by nekeyzhong for ������ϵ2006-1-18 sequence:mainsvr_111_20060109
	enum enMutexInfoType
	{
		TMUTEX_MONEY	= 0,				//��Ϸ��
		TMUTEX_HAPPY_ERNERGY =  1,			//��������ֵ
		TMUTEX_HAPPY_CURRECY = 2			//���ֱ�
	};
	//END modified by nekeyzhong 

	typedef struct tagReplacingAvatar
	{
		int m_iAvatarID;
		short m_nAvatarType;
		int m_iValidTime;
		int m_iAvatarEquiping;
	}TReplacingAvatar;

	//! �ʵ�����
	typedef enum enmBillType 
	{
		EBT_HAPPY_TICKET		= 1001,	/*!<  ������Ʊ����*/
		EBT_HAPPY_ESCAPE		= 1002,	/*!<  ������Ϸ����������*/
		EBT_HAPPY_ITEM			= 1003,	/*!<  ����ʹ��*/
		EBT_MONEY_TICKET		= 2001,	/*!<  ��Ϸ����Ʊ����,��������Ϸ��ƽ���õ����ģ���ʼֵ*/
		EBT_MONEY_ESCAPE		= 2002,	/*!<  ��Ϸ����Ϸ����������*/
		EBT_MONEY_ITEM			= 2003,	/*!<  ��Ϸ��ʹ��*/
	}ENM_BILL_TYPE;

	enum enPayType
	{
		pay_bySelf		= 0,	// �Լ�����
		pay_byOther		= 1,	// ���˸���
		pay_forOther	= 2		// Ϊ���˸���
	};

	enum enItemType
	{
		public_item = 0,		// ���õ���
		unit_item = 1,			// ��������
		time_item = 2			// ʱЧ����
	};

	// ��ҵ÷�
	typedef struct	tagUserScore
	{ 
		int  lScore;			// ��ҷ���         �������� ��+����-
		int  lMoney;			// ��Ϸ����         �������� ��+����-

        int  lExperience;     // �����ľ���ֵ��   �������� ��+����-
        int  lGameExperience;   //���������ڵ���Ϸ�ľ���ֵ�� ������������+ ����-

		UINT lWinCount;		// Ӯ������         �������� ��+������-
		UINT lLostCount;		// �������         �������� ��+������-
		UINT lDogfall;		// ƽ������         �������� ��+������-
		UINT lEscapeCount;	// ���ܴ���         �������� ��+������-

		UINT lCostTime;      // ������Ϸ��ʱ�䳤
		UINT lSharedMoney;   // ��ˮ��Ϸ����    ƽ̨�ڲ��ķ��س�ˮ��Ϸ����
        UINT lShareScore;	// ��ҷ���        ƽ̨�ڲ��ķ��س�ˮ����
		int  lBillType;		// �˵�����        //��ң�����
	}USER_SCORE;
	

	class ISoUser  
	{
	public:
		//������ID
		virtual UINT GetUserID() = 0;
		virtual UINT GetUserAge() = 0;
		virtual UINT GetUserGender() = 0;        /// GetUserGender ���� 0 ��ʾŮ�ԣ� ���� 1����ʾ����
		virtual UINT GetChairID() = 0;
		virtual UINT GetUserStatus() = 0;
		virtual UINT GetFaceID() = 0;
        virtual int  GetRightLevel() = 0;
        virtual int  GetScore() = 0;              /// ��ȡ��Ϸ����
        virtual int  GetMoney() = 0;              /// ��ȡ��Ϸ���

		//nSizeΪ��lpszNickName����Ŀռ�
		virtual void  GetNickName(char *lpszNickName, int nSize) = 0;
		virtual char * GetNickName() = 0;
        virtual BOOL  IsReady() = 0;
		virtual BOOL  IsVIP() = 0;                 /// ��Ա���еȼ������� bMember �Ǵ����Ա�ĵȼ���Ϣ

		virtual BOOL  IsPlayer()   = 0;
		virtual BOOL  IsOffline()  = 0;

        virtual void   ClearState() = 0;
	};

	class ISoPlayer :public ISoUser
	{
	public:
		virtual void SetTeam(ENM_PLAYER_TEAM enmTeam) = 0;
		virtual ENM_PLAYER_TEAM GetTeam() = 0;

		//�����Թ�
		virtual void SetBCanBeLookedOn(BOOL bCanBeLookedOn) = 0;
		virtual BOOL GetBCanBeLookedOn() = 0;
		virtual BOOL IsLockedMoney() = 0;

		//������ҵ�Ǯ
		virtual void SetPlayerLockMoneyFlag(BOOL bLocked) = 0;

		//�����й�
		virtual BOOL GetTrusteeStatusVal() = 0;

		virtual void SetTrusteeStatusVal(BOOL bTrue) = 0;

		virtual void SetPlayerLockHappyBeanFlag(BOOL bLocked) = 0;
		virtual BOOL IsLockedHappyBean() = 0;

        virtual BOOL  IsForceExit() = 0;
        virtual void   SetForceExit(BOOL bForceExit) = 0;

	};

	class ISoWatcher : public ISoPlayer
	{
		
	};

	//ISoUserManager �� ISoUserManagerInter
	/**
	 * @ingroup GameSDK
	 * @brief ISoUserManager
	 *
	 * <p>�� 
	 *		ISoUserManager �������ҹ���ģ��
	 * </p>
	 * @sa
	 */	
	class ISoUserManager : public IGameObject, public IEventProducer
	{
	public:
 		//��ȡ���������
		/**
		 *  @brief ��ȡ���������
		 *  @param bExitAtPlay TRUE��Ϸ���˳������Ҳ���㣬�����෴
		 *  @sa 
		 *	@return ���������
		 */	
 		virtual UINT GetTeamCount(BOOL bExitAtPlay /*= FALSE*/) = 0;

		/**
		 *  @brief ����PlayerID��ȡ�û�ISoUserָ��
		 *  @param shPlayerID �û���PlayerId
		 *  @sa ISoUser
		 *	@return �û���ISoUser�ӿ�ָ�룬���п�
		 */	
		virtual ISoUser* GetUserByID(UINT shPlayerID) = 0;

		/**
		 *  @brief ��õ�ǰ��ҵ�����
		 *	@return ��ǰ��ҵ�����
		 */	
		virtual UINT	GetPlayerCount() = 0;

		/**
		 *  @brief ����PlayerID��ȡ���ISoPlayerָ��
		 *  @param shPlayerID ��ҵ�PlayerId
		 *  @sa ISoPlayer
		 *	@return ��ҵ�ISoPlayer�ӿ�ָ�룬���п�
		 */	
		virtual ISoPlayer*	GetPlayerByID(UINT shPlayerID) = 0;

		/**
		 *  @brief �������ChairId��ȡ���ISoPlayerָ��
		 *  @param unUin ���ChairId
		 *  @sa ISoPlayer
		 *	@return ��ҵ�ISoPlayer�ӿ�ָ�룬���п�
		 */
		virtual ISoPlayer*	GetPlayerByChair(UINT uChair) = 0;

		/**
		 *  @brief �ж�һ���û��Ƿ������
		 *  @sa 
		 *	@return TRUE��ʾ�ǣ�FALSE��ʾ����
		 */
		virtual BOOL IsPlayer(UINT uPlayerID) = 0;

		/**
		 *  @brief ����һ������Ƿ����й�״̬
		 *  @param cChair ��ҵ�ChairID
		 *  @param bTrustee �Ƿ����й�״̬
		 *  @sa 
		 */
		virtual void SetPlayerTrusteeStatus(UINT uChair, BOOL bTrustee) = 0;

		/**
		 *  @brief �ж�һ���û��Ƿ����й�״̬
		 *  @param cChair ��ҵ�ChairID
		 *  @sa 
		 *	@return TRUE��ʾ�ǣ�FALSE��ʾ����
		 */
		virtual BOOL GetPlayerTrusteeStatus(UINT cChair) = 0;

        /**
        *  @brief  ������Ϸ��������
        *  @param nDealer ׯ��λ��
        *  @param byWin   Ӯ��λ��(֧��һ�ڶ���)
        *  @param arPoint ÿ�������Ҫ����ĵ���
        *  @param rePoint �����ĵ���
        *  @return ����ֵ������
        */
        virtual STATUS SetGameScore(int nDealer, BYTE *byWin, int *arPoint, int *rePoint) = 0;

		/**
		 *  @brief ��ȡĳ���û�����Ϸ������
		 *  @param nPlayerID �û���PlayerID
		 *  @param piMoney[out]      �����Ϸ������int�͵ĵ�ַ
		 *  @param bMinusTicket      ��ȡ����Ϸ���Ƿ��Ѽ�ȥ��Ʊ
		 *	@return ����ֵ������
		 */
		virtual int GetMoney(UINT nPlayerID, int* piMoney, BOOL bMinusTicket = TRUE) = 0;
	
		/**
		 *  @brief ��ȡ��������ͣ���Ϸ�ҷ��䡢���ֶ����仹����ͨ����
		 *	@return ���ط��������
		 *  @sa ENM_ROOM_TYPE
		 */
		virtual int GetRoomType() = 0; 
	};


/*	//��Ϸ��ʽ��ʼ �� ��Ϸ����ʱ��CSoUserManager�еĴ����������gameprocess�ڲ�����
	class ISoUserManagerInter
	{
	public:
		//��Ϸ��ʼ
		virtual int  OnGameStart() = 0;
		//��Ϸ����
		virtual void OnGameEnd() = 0;
	};*/
	
	class ISoUserManagerTrusteeEvent : public IEvent
	{
	public:
		virtual int OnChangeTrustee(UINT nChair, BOOL bIsTrustee) = 0;
	};

	
	typedef enum enmBeforeKickType
	{
		EBKT_OFFLINE_ON_NOTALLOW_OFFLINE = 0,					// ��������������ʱ�� ���ߺ�����
		EBKT_OFFLINE_ON_WATCHER_OFFLINE_GAMEONTSTART,			// �Թۻ���Ϸ��û�п�ʼ ���ߺ�����
		EBKT_OFFLINE_ON_GAME_END,								// ��Ϸ������ �Զ�����ҵĴ���
		EBKT_FAIL_LOCK_MONEY,									// ��Ϸ�ҳ�������Ϸ��ʧ��
		EBKT_FAIL_LOCK_HAPPYBEAN,								// ���ֶ����������ֶ�ʧ��
		EBKT_MORE_THAN_UPPER,									// ��������
		EBKT_LESS_THAN_TICKET,									// ������Ʊ
		EBKT_LESS_THAN_LOWER,									// ��������
		EBKT_GAME_HAS_START,									// ��Ϸ�Ѿ���ʼ
        EBKT_VIP_KICK,                                          // VIP����              
        EBKT_GM_KICK                                            // GM ����
	}ENM_BEFORE_KICKTYPE;

	class ISoUserManagerEvent : public IEvent
	{
	public:
		virtual int OnGameUserEnter(UINT nPlayerID) = 0;
		virtual int OnGameUserExit(UINT nPlayerID) = 0;
		virtual int OnGameUserOffLine(UINT nPlayerID) = 0;
		virtual int OnGameUserReplay(UINT nPlayerID) = 0;
		virtual int OnGameUserReady(UINT nPlayerID) = 0;
        	//��ʱû��ʵ�ִ�Щ����
        	//�����Ӧ��bResult == 1 �����������ˣ���Ȼ�Ͳ�׼�������
        	// enmKickType  ��������ENM_BEFORE_KICKTYPE
		virtual int OnBeforeKickUser(UINT nPlayerID, ENM_BEFORE_KICKTYPE enmKickType, BOOL *pbResult) = 0;
		//��TransHandle������ 
		//nResultΪ1ʱ�ſ������£�����������
		virtual int OnGameIsPlayerCanEnter(ISoPlayer *pUserInfo, int *pnResult, char*lpszMsg) = 0;
		virtual int OnGameIsViewerCanEnter(ISoPlayer *pUserInfo, int *pnResult, char*lpszMsg) = 0;
		virtual int OnGameNotifyUserExitReason(UINT nReason, UINT nPlayerID, UINT nszBufSize, char* szBuf) = 0;
		
		virtual int OnChangeLookOnSetting(UINT nPlayerID, BOOL bLookOn) = 0;

        //*pbResult ��ʼ��ΪTRUE, ���ΪFALSE�� FRAME�ǲ����û����иı���λ��
        virtual int OnBeforeGameUserChangeChair(UINT nPlayerID, BYTE bSrcChair, BYTE bDstChair, BOOL* pbResult)= 0;
	};

	class ISoUserMgrToGameProcessEvent : public IEvent
	{
	public:
		//��ʼ����ʱ, ����ֵû������
		virtual int OnBeginCountDown() = 0;
		//ȡ������ʱ, ����ֵû������
		virtual int OnCancleCountDown() = 0;
	};
}
#endif

