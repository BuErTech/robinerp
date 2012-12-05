#ifndef __CDESK_LOGIC_IMPL__
#define __CDESK_LOGIC_IMPL__

#include "afxwin.h"
#include "SoGameProcessor.h"
#include "SoUserManager.h"
#include "SoGameLogerImpl.h"
#include "SoGameFactory.h"
#include "SoGameSettingImpl.h"
#include "SoFireEvent_t.h"
#include "SoBaseLogic.h"
#include "SoUserManager.h"

// ��Ϸ��ʱ��ID��ID����С����������, ������ܻ���ƽ̨�е�ID��ͻ��֮ǰ�����ڶ�ʱ��Ϊ1, ��ƽ̨�еĶ��߶�ʱ�������ǵ�, �����齫�����߼���ʱ��ֹͣ����)
#define TIMER_GAME_ID  (PLAY_COUNT + 1)

namespace MyGame
{

class CGameDeskX : public CGameDesk
{
public:
	CGameDeskX():CGameDesk(FULL_BEGIN) {};
	~CGameDeskX() {};
};

class CDeskLogicImpl : virtual public TSoEventProducerImpl<CGameDeskX>
{
public:
    MyGame::SoPlayer    m_SoPlayer[PLAY_COUNT];  
    SoGameFactory       *m_pGameFactory;
    GameProcessor       *m_pGameProcessor;
    SoUserManager       *m_pUserNanager;
    SoGameLogerImpl     *m_pGameLogerImpl;
    SoGameSettingImpl   *m_pGameSettingImpl;
    CSoBaseLogic        *m_pBaseLogic;
	BYTE				m_nGameStation;   // ƽ̨GameDesk������һ�� m_bGameStation ����, �����в�Ҫ������
	bool				m_bPlaying;
	BOOL				m_bAgree[PLAY_COUNT];
	BOOL				m_bNetCut[PLAY_COUNT];
	BYTE                m_bStartMode;     // ��ʼģʽ(0: ���˲ſ�ʼ  1: ������ͬ��Ϳ�ʼ  2: �Գ�ͬ�⿪ʼ  3: 6����λ��ֻҪ��4����ͬ�⿪ʼ)
 
public:
	CDeskLogicImpl(void);
	~CDeskLogicImpl(void);

    //-------------------------------------------------------------------------
    // ��IGameDesk����
    //-------------------------------------------------------------------------

	//��ʼ��Ϸ
	virtual bool InitDeskGameStation();

	//��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);

	//��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation,BYTE bCloseFlag);

	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);

	//������Ϸ�ж��Ƿ�����Ϸ��
	virtual BOOL IsPlayingByGameStation();

	//�����Ϣ������
	virtual bool HandleFrameMessage(BYTE bDeskStation,NetMessageHead* pNetHead,void* pData,UINT uSize,UINT uSocketID,bool bWatchUser);

	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation,NetMessageHead* pNetHead,void* pData,UINT uSize,UINT uSocketID,bool bWatchUser);

	//�������
	virtual BYTE UserSitDesk(MSG_GR_S_UserSit* pUserSit, CGameUserInfo* pUserInfo);

	//�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation,CGameUserInfo* pUserInfo);

	//������ȡ��Ҷ��߻�����Ϣ
	virtual bool UserReCome(BYTE bDeskStation,CGameUserInfo* pNewUserInfo);

	//������ȡ��Ҷ�����Ϣ(��ƽ̨�ӿ�)
	virtual bool UserNetCut(BYTE bDeskStation,CGameUserInfo* pLostUserInfo);

	///�Ƿ���Կ�ʼ��Ϸ
	virtual bool CanBeginGame();

	//������ȡ��Ҷ�����Ϣ(������ƽ̨�Ľӿ�)
	//virtual bool UserNetCut(BYTE bDeskStation,CGameUserInfo* pLostUserInfo,bool bIsForceQuit);

	//�����ı��û����������ĺ���
	//virtual bool CanNetCut(BYTE bDeskStation);

	/*-���ش��麯��-*/

	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);

	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);

	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);

	//������Ϸ�¼�
	virtual bool LoadEvents();

	//���ͬ����Ϸ
	virtual bool UserAgreeGame(BYTE bDeskStation);

	//���ö�ʱ��
	bool SetGameTimer(UINT nTimerID, int nTime);

	//������Ϸ��ʼģʽ:  0: ���˲ſ�ʼ   1: ������ͬ��Ϳ�ʼ   2: �Գ�ͬ�⿪ʼ   3: 6����λ��ֻҪ��4����ͬ�⿪ʼ
	void SetStartMode(BYTE byMode);

    // ���ش��麯��
public:

    /** 
    * @brief ��ȡ�û���Ϣ
    * @param bDeskStation ���λ��
    * @return �����û���Ϣ
    */
    UserInfoStruct * GetUserInfo(BYTE bDeskStation);

    /** 
    * @brief ��ȡ�û���Ϣ
    * @param bDeskStation ���λ��
    * @return �����û���Ϣ
    */
    SoPlayer * GetSoPlayer(BYTE bDeskStation);


    /** 
    * @brief ��Ϸ��ʼ
    */
    void GameBegin();

    /** 
    * @brief ��Ϸ����
    * @param nGameEndType ��Ϸ��������
    */
    void GameFinish(int nGameEndType);

    /**
    *  @brief ��ȡ����ID
    */
    UINT GetRoomID();

	/**
    *  @brief ��ȡ����˰�ձ���
    */
    UINT GetRoomTax();

    /**
    *  @brief ��ȡ�����ע
    */
    UINT GetBaseMoney();

    /**
    *  @brief ��ȡ�������ٽ������
    */
    UINT GetMinLimitMoney();

    /// ����й�
    int UserTrustee(int nDeskStation, BYTE* pBuffer, int nLen);

	/*
	*	@brief ������Ϣ��װ
	*/
	int SendDeskGameData(UINT bAssID, BYTE *pBuffer, int nLen);

	/*
	*	@brief ������Ϣ��װ
	*/
	int SendDeskGameData(BYTE bDeskStation, UINT bAssID, BYTE *pBuffer, int nLen);

	/*
	*	@brief �����Թ���Ϣ��װ
	*/
	int SendDeskWatchGameData(UINT bAssID, BYTE *pBuffer, int nLen);

	/*
	*	@brief �����Թ���Ϣ��װ
	*/
	int SendDeskWatchGameData(BYTE bDeskStation, UINT bAssID, BYTE *pBuffer, int nLen);

	/*
	*	@brief ��ҽ�Ҽ���
	*/
	void ChangeUserPoint(int nDealer, BYTE *byWin, int *arPoint, int *rePoint);
};
};

#endif
