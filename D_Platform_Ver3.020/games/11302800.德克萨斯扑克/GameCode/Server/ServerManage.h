#pragma once
#include "../../../../sdk/include/ServerCommon/GameDesk.h"
#include "../GameMessage/UpGradeLogic.h"
#include "GameDataObject.h"


//��ʱ�� ID
#define IDT_USER_CUT				1L				//���߶�ʱ�� ID

//*****************************************************************************************
//CServerGameDeskΪ������ܲ�������ļ�ʱ��ʽMyTimer����ص�ID���������£�

#define IDT_MYTIMER					0x21			//���ö�ʱ����ֻ�������ʱ����ʵ�ʶ�ʱ�������඼�Ǵ������ʱ�������ж�����

#define MY_ELAPSE					500				//��ʱ�����
#define TIMER_COUNT					25				//�õ��Ķ�ʱ����Ŀ
#define TIMER_ALLTIMERFINSH         0x24            //�ܵĶ�ʱ��

#define IDMYT_BASE					30				//MyTimer��ID����������ʱ
#define IDMYT_EVENT_PROTECT			31				//�¼���������ʱ��

#define IDEV_SYS_CFG_DESK			35				//ϵͳ�趨�������ö�ʱ��/�¼�
#define IDEV_USER_CFG_DESK			36				//����趨�������ö�ʱ��/�¼�
#define IDEV_RAND_CARD				37				//ϴ�ƶ�ʱ��/�¼�

#define IDEV_SENDACARD				38				//�·��ƶ�ʱ��/�¼�
#define IDEV_SEND_3_CARD			39				//��   3 ���ƶ�ʱ��/�¼�
#define IDEV_SEND_4_5_CARD			40				//���� 4, 5 ���ƶ�ʱ��/�¼�
#define IDEV_BURN_CARD				42				//���ƶ�ʱ��/�¼�

#define IDEV_BET_START				43				//�״���ע��ʱ��/�¼�
#define IDEV_BET_BLINDS_AUTO		44				//��Сäעͬʱ��ע��ʱ��/�¼�(ϵͳǿ��)
#define IDEV_BET_NEXT				45				//��λ�����ע��ʱ��/�¼�

#define IDEV_BIGBLIND               46              //�´���ע��ʱ��
#define IDEV_SMALL_BLIND	        47              //��С��ע��ʱ��

#define IDEV_ROUND_FINISH			48				//һ�غ����/�¼�
#define IDEV_RESULT					49				//��Ϸ���㶨ʱ��/�¼�
#define IDEV_NEXT_ROUND_BEGIN		50				//��һ�غϿ�ʼ�Ķ�ʱ��/�¼�		
#define IDEV_COMPARE_CARD           51              //����
#define IDEV_RANDOM					52				//���һ��ׯ���С/�¼�

#define IDEV_AUTO_OPERATE           53              //�Զ�������ʱ��, ����ͻ������δ��������������������

#define ONE_SECOND                  1000            // 1 ��ʱ��

//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SALE						11				//��Ϸ��ȫ����

//��Ϸ����

//��Ϸ����
class CServerGameDesk : public CGameDesk
{

public:
	//���캯��
	CServerGameDesk(); 

	//��������
	virtual ~CServerGameDesk();

/*-----------------------------------------------------------------------------------------------------------
    һϵ��ƽ̨�ṩ����Ҫ������������Ϸ����ĺ����������ô������Ϸ�Ŀ�������ÿ��������ʵ������Ϸ���졣
  ----------------------------------------------------------------------------------------------------------*/
public:

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
	virtual BYTE UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo);

	//�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation,CGameUserInfo* pUserInfo);

	//������ȡ��Ҷ��߻�����Ϣ
	virtual bool UserReCome(BYTE bDeskStation,CGameUserInfo* pNewUserInfo);

	//������ȡ��Ҷ�����Ϣ
	virtual bool UserNetCut(BYTE bDeskStation,CGameUserInfo* pLostUserInfo);

	//�����ı��û����������ĺ���
	virtual bool CanNetCut(BYTE bDeskStation);

  /*-���ش��麯��-*/

	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);

	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);

	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);


/*-----------------------------------------------------------------------------------------------------------
        ����һϵ���������л��ƺ����������ô������Ϸ�Ŀ�������ÿ��������ʵ������Ϸ���졣
    �������û��ơ���ʱ���ơ����������ơ���Ϸ״̬�Ĺ���
  ----------------------------------------------------------------------------------------------------------*/
public:

	//���ع�����������
	BOOL LoadDeskCfg();

	//���ݷ���ID������������
	BOOL LoadDeskCfg(int iRoomID);

	//��������
	BOOL LoadChangCard(void);

	//��ʼ�����
	void InitGroupTable(void);

	//��Ӷ�ʱ����־
	int SetMyTimer(UINT nTimeID,int uElapse);

	//�����ʱ����־
	void KillMyTimer(UINT nTimeID);

	//��ʱ��ĳ��־����Ӧ����
	bool OnMyTimer(UINT uTimerID);

	//����������
	bool ActionRedirect(UINT uiActID,int iDelay);

	//����������������
	bool OpenEventProtection(UINT uiActID,int iActLife);

	//�¼�����ʱ����
	bool EventGoing();

	//�ͷ��¼���Դ
	bool CloseEventProtection();

	//�����Ϸ�Ļ�����״̬
	void GetGSBase(TGSBase* pGSBase,BYTE byDeskstation);

	//�����Ϸ�Ļ�����״̬
	void CopyGSBase(TGSBase* pTarget,const TGSBase* pSource);


/*-----------------------------------------------------------------------------------------------------------
    ������Ϸ�¼���Ӧ���������ܺ�������ͬ��Ϸ����ϴ�
  ----------------------------------------------------------------------------------------------------------*/
public:

  /*-�����¼���Ӧ����������Ϸ���Է�����---*/

	//ϵͳ�趨������¼�������Ҫ�趨ʱ�䡢��Ϸ����ȣ�
	bool OnEventSysCfgDesk();

	//����趨������¼�������Ҫ�趨��ע��
	bool OnEventUserCfgDesk();

	//ϴ���¼�
	bool OnEventRandCard();

	//���һ��ׯ
	bool OnEventRandom();

	//Ϊÿ����ҷ��Ƶ��¼�
	bool OnEventSendACard();

	//Ϊ��3�Ź��Ƶ��¼�
	bool OnEventSend3Card();

	//Ϊ��1�Ź������Ƶ��¼�
	bool OnEventSend1Card();

	//�״���ע�¼�
	bool OnEventBetStart();

	//��Сäעǿ��ͬʱ��ע�¼�
	bool OnEventBetBlindsAuto();

	//��һλ�����ע�¼�
	bool OnEventBetNext();

	//��ʼ��һ�غ���Ϸ�¼�����
	bool OnEventNextRoundBegin();

    //�Զ�������Ϸ�¼�����
    bool OnEventAutoOperate();

    // ����
    bool OnCompareCard();
  /*-�����¼���Ӧ��ͨ���ͻ�������---*/

	//�������ͬ����Ϸ
	bool OnUserAgreeGame(BYTE bDeskStation,
		NetMessageHead* pNetHead,void* pData,UINT uSize,UINT uSocketID,bool bWatchUser);

	//����������õ�ע
	bool OnUserCfgDesk(BYTE bDeskStation,
		NetMessageHead* pNetHead,void* pData,UINT uSize,UINT uSocketID,bool bWatchUser);

	//����������õ�ע��ʱ
	bool OnUserCfgDeskTimeout(BYTE bDeskStation,
		NetMessageHead* pNetHead,void* pData,UINT uSize,UINT uSocketID,bool bWatchUser);

	//�������ͬ���ע
	bool OnUserAgreeDeskCfg(BYTE bDeskStation,
		NetMessageHead* pNetHead,void* pData,UINT uSize,UINT uSocketID,bool bWatchUser);

    //��Ҵ���������
    bool OnUserSubMoney(BYTE bDeskStation, void* pData);

	//�����ע��Ϣ
	bool OnUserBet(BYTE bDeskStation, void* pData , bool bUserforceExit = false);

	//������ҷ������
	bool OnUserSendCardFinish(BYTE bDeskStation,
		NetMessageHead* pNetHead,void* pData,UINT uSize,UINT uSocketID,bool bWatchUser);

	//������Ҷ���һ�غ�׼�����ź�
	bool OnUserNextRoundReq(BYTE bDeskStation,
		NetMessageHead* pNetHead,void* pData,UINT uSize,UINT uSocketID,bool bWatchUser);

	//������ұ����Ҳ��������ٴ�����
	bool OnUserMoneyLack(BYTE bDeskStation);

	////�����������������Ϸ������
	bool OnTestFinishRoundRequest(BYTE bDeskStation,
		NetMessageHead* pNetHead,void* pData,UINT uSize,UINT uSocketID,bool bWatchUser);


  /*-��Ϸ�¼���������ĳ�¼�����������û���յ��κ��ֶ����ٵ���Ϣ������Ȼ�����ڻ���ǰӦ�ý��д��ܴ�����ȷ����Ϸ���̲���Ӱ��--------*/

	//������ҷ������
	bool OnAgentSendCardsFinish();

	//�����¼�������
	bool OnAgentOutCard();

	//��һ�ֿ�ʼ�¼�������
	bool OnAgentNextRoundBegin();


  /*-��Ϸ״̬�ָ���Ӧ�����ڿͻ������κ������������Ϸ���õ���������ȷ������-----*/
	//����Ҫ�õ���Ϸ�ȴ���ʼ������
	bool OnGetGSWaitAgree(BYTE bDeskStation,
		UINT uSocketID,bool bWatchUser,TGSBase* pGSBase);

	//����Ҫ�õ���Ϸ��״̬������
	bool OnGetGSPlaying(BYTE bDeskStation,
		UINT uSocketID,bool bWatchUser,TGSBase* pGSBase);

	//����Ҫ�õ���Ϸ�غϽ���������
	bool OnGetGSWaitNextRound(BYTE bDeskStation,
		UINT uSocketID,bool bWatchUser,TGSBase* pGSBase);


  /*-���ܺ���--*/

	//��ʱ��˳��õ��¼�λ��
	BYTE GetNextUserStation(BYTE byStation, BOOL bIsAll = TRUE);

	//��ʱ��˳��õ��ϼ�λ��
	BYTE GetLastUserStation(BYTE byStation, BOOL bIsAll = TRUE);

	//��ȡ��Ϸ�������
	int GetPlayerCount(bool bFold = false);

    // ����ÿ��ʤ�����
    void CalculateWin(BYTE nSectionCount, CByteArray &arrayResult);

	// �ж�����Ƿ�������������
	bool CanGetGoodCard(BYTE byStation);
/*-----------------------------------------------------------------------------------------------------------
    ������������ݣ�������Щ���������ڴ��������Ϸ����ʱʹ��
  ----------------------------------------------------------------------------------------------------------*/
protected:

	//��Ϸ����
	CGameDataObject m_GameData;

	//��Ϸ�߼�
	CUpGradeGameLogic m_Logic;

	//����������̫��Ķ�ʱ������Ӱ�����ܣ��ʴ������Ż���ֻ��һ����ʱ����\
	  ����ʹ�õ��Ķ�ʱ�����������ʱ�����������ڴ����м����ж�

	struct TMyTimerNode
	{
		//��ʱ����־��
		UINT uiID;

		//�Ѿ������˴�
		int  nTickCount; 

		//�������ٴκ󴥷��¼�
		int  nTriggerCount; 

		TMyTimerNode(UINT nTimerID, int uElapse):\
			uiID(nTimerID),nTickCount(0),nTriggerCount(uElapse){}

		TMyTimerNode():\
			uiID(0),nTickCount(0), nTriggerCount(0){}
	};

	TMyTimerNode m_arTimer[TIMER_COUNT];

	//��ǰ�¼�������״̬��������
	struct TEvProtectDescr
	{
		//��ǰ���������¼�
		UINT uiEvID;

		//�¼�������
		int  iLiftTime;

		//����ȥ��ʱ��
		int  iPassTime;

		TEvProtectDescr():\
			uiEvID(0),iLiftTime(0),iPassTime(0){}

	};

	TEvProtectDescr m_EventProtecter;


/*-----------------------------------------------------------------------------------------------------------
    ������������ݣ�������Щ���������ڴ��������Ϸ����ʱʹ��
  ----------------------------------------------------------------------------------------------------------*/
private:
	bool m_bGameFinished;
protected:
	int						m_iUserStation[PLAY_COUNT];				//��¼�û�״̬
	
	bool					m_bFaiPaiFinish;						// �������
};
