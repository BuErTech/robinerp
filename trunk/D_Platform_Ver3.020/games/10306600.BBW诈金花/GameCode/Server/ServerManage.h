#pragma once

#include "../../../../sdk/include/ServerCommon/GameDesk.h"
#include "../GameMessage/UpGradeLogic.h"

// ����������̫��Ķ�ʱ������Ӱ�����ܣ��ʴ������Ż���ֻ��һ����ʱ��������ʹ�õ��Ķ�ʱ�����������ʱ�����������ڴ����м����ж�

// ��ʱ�����ýṹ
struct TimerStruct 
{
	UINT nId;			//��ʱ����־��
	int nTickCount;		//�Ѿ������˴�
	int nTriggerCount;	// �������ٴκ󴥷��¼�
	TimerStruct(UINT nTimerID, int uElapse) : nId(nTimerID),nTickCount(0), nTriggerCount(uElapse)
	{
	}
	TimerStruct(): nId(0),nTickCount(0), nTriggerCount(0){}
};


//��ʱ�� ID
#define TIME_MY_TIMER				20				//���ö�ʱ����ֻ�������ʱ����ʵ�ʶ�ʱ�������඼�Ǵ������ʱ�������ж�����
#define MY_ELAPSE					250				// ��ʱ�����
#define TIMER_COUNT					11				// �õ��Ķ�ʱ����Ŀ

// ����Ϊ��Ϸ��ʹ�õ��Ķ�ʱ�����������TIME_MY_TIMER��������ͳһ����

#define TIME_SEND_CARD				30				//���ƶ�ʱ��
#define TIME_WAIT_NT				31				//�ȴ�����
#define TIME_GAME_FINISH			32				//��Ϸ������ʱ��
#define TIME_SEND_A_CARD			33				//����һ����
#define TIME_BEGIN_ANIMAL			34				//��Ϸ��ʼ����
#define TIME_COLLECT_JETON			35				//�ռ�����
#define TIMER_SEND_END              36
#define TIME_BIPAI					37				//���ƶ�����ʱ��
#define TIME_CUT_AI                 38              //�йܶ�ʱ��

#define TIMER_WAIT_NOTE             39				// 
#define TIMER_FINISH_COMPARE        40              // ��Ϸ�����ճ����ʱ��

#define DEFAULT_WAIT_FREQUENCE      1000            //һ���м��һ��

/*
//��Ϸ������Ϣ
#define PLAY_MAX_NOTE				500			//�����ע
#define PLAY_BASE_NOTE				5				//��ע
#define THINK_TIME					30				//��Ϸ˼��ʱ��
#define PLAY_CARD_COUNT				1				//�˿˸���

*/#define ALL_CARD_COUNT				52				//һ��ʹ�õ�����
//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SALE						11				//��Ϸ��ȫ����

//#define SEND_CARD_TIME				1000			//����ʱ�����ò�����10����,������1000
#define  TIMER_CUT_AI_LONG                  1000


// ��������ؼ����ͽ�������
class BZWLockForCriticalSection
{
	CRITICAL_SECTION *m_pSection;
	BZWLockForCriticalSection();
public:
	BZWLockForCriticalSection(CRITICAL_SECTION *pSection)
	{
		m_pSection = pSection;
		EnterCriticalSection(m_pSection);
	}
	~BZWLockForCriticalSection()
	{
		LeaveCriticalSection(m_pSection);
	}
};


//��Ϸ����
class CServerGameDesk : public CGameDesk
{

	//std::list<TimerStruct>	m_lstTimer;
	TimerStruct		m_arTimer[TIMER_COUNT];
	// ɱ����ʱ����־
	void KillMyTimer(UINT nTimeID);
	int SetMyTimer(UINT nTimeID, int uElapse);
	bool OnMyTimer(UINT uTimerID);
	CRITICAL_SECTION m_csForIPCMessage;

protected:
	enum 
	{
		FINISH_TYPE_UNFINISH = 0,     //û�н���		
		FINISH_TYPE_FISH = 1,         //��������      
		FINISH_TYPE_REACH_LIMITE_SELF = 2,  //�ﵽ���ޣ�����
		FINISH_TYPE_REACH_LIMITE_NEXT = 3   //��һ�Ҹ�ע�ͻᳬ������
	};
	//����״̬����
	//	int						m_PunishPoint[4];					//�û�����
	BYTE					m_iNtPeople;                          //Ӯ��

	int						m_iUpGradePeople;					//ׯ��λ��
	BYTE					m_iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	BYTE					m_iLeaveArgee;						//�뿪ͬ���־
	BYTE					m_iHaveThingPeople;					//����Ҫ�����

	//״̬��Ϣ
	BYTE					m_iUserCardCount[PLAY_COUNT];				//�û������˿���Ŀ
	BYTE					m_iUserCard[PLAY_COUNT][5];					//�û����ϵ��˿�
	BOOL                    m_Ming[PLAY_COUNT];                         //����

	bool                    m_bOpenLoser[PLAY_COUNT];                    //����ʧ��
	bool                    m_bIsGiveUp[PLAY_COUNT];                     //�Ƿ����

    int                     m_iNowBigPeople;
	//������Ϣ
	//	int						m_iGamePoint;						//�û���Ϸ����
	int						m_iOutCardPeople;					//���ڳ����û�
	int						m_iFirstOutPeople;					//�ȳ��Ƶ��û�
	bool                    m_bFirstNote;                       //�Ƿ�Ϊ��һ����ע

	BYTE					m_bReachLimitPlayer;				// ��һ���ﵽ���޵������λ��
	
	//��������
	BYTE					m_iTotalCard[52];					//�ܵ���
	BYTE					m_iSendCardPos;						//����λ��

	CUpGradeGameLogic		m_Logic;     						//��Ϸ�߼�
	//	bool					m_bQueryQuit;						//����������˳�

	int						m_iLastOutPeople;
	int						m_iUserStation[PLAY_COUNT];				//��¼�û�״̬
	
	__int64						m_iUserNote[PLAY_COUNT];	       //�û�ÿ��Ѻע�����Ǳ�����ע��	
	
	
	__int64						m_iPerJuTotalNote[PLAY_COUNT];      //�û�ÿ��ѹ��ע	
	__int64						m_iNowNote[PLAY_COUNT];			    //��ǰѺע��
	__int64						m_iTotalNote;					    //��ע
	__int64                     m_iFrontNote;                      //ǰ��Ѻע
	
	__int64                     m_iThisGameNote[PLAY_COUNT];	    //������ע(�ж��Ƿ�ﵽ����)
	__int64                     m_iTotalGameNote[PLAY_COUNT];
	
	__int64                     m_iMaxNote;                     //���ע(ÿ��)����ע��
	int                     m_ihelp[PLAY_COUNT];
	BYTE					m_iIngPlayNote;					//��ǰ��ע�ߣ����Թۣ�
	BYTE					m_iLastShuttle;					//��һ������
	int					    m_bAddNoteTime;					//��ע�������������ͻ����ж��Ƿ���Ա���

	BYTE					m_iSendPos;				//��һ�ַ��ƿ�ʼλ��

	//Add By JianGK.20100720
	int						m_dwAIBalanceMoney;						//�����˳��н�����ȶ�ֵ
	BYTE					m_iAIWinProb[2];						//������ʤ�ʱ�

#ifdef SUPER_VERSION
	BYTE					m_bSuperCardList[8][54];				//�����ͻ���
	int						m_iSuperCardCount[8];					//
	int						m_bSuperStation;						//�����ͻ���λ��
	struct SetCardStruct	//�ͻ��˵�������
	{
		BYTE pai[8][54];
		int	paicount[8];
	};
#endif

private:
	static BYTE m_iAllCardCount;	//��Ϸ���õ�����
	__int64 m_iLimitNote;	            //����            
	int m_iBaseNote;		//��ע       
	int m_iGuoDi;           //����ֵ
	__int64 m_iLimitePerNote;   //��ע����  ��ע�ģ�����ʾΪ��ע��

	//bool m_bCanOpen;         //�ж��Ƿ���Կ���

	int      m_iTimerCount;   //��¼���ʱ��
	int      m_iAddressIndex[PLAY_COUNT]; 

	//��ҵ�����
	int     m_iMaxSuperPeople;
	CString  m_strCityTextInDex[50];
	CString  m_strUserIDTextInDex[50];

	bool     m_bSuperUser[PLAY_COUNT];

	//=============��չ
	static 	BYTE m_iSendCardTime;//��Ϸ˼��ʱ��
	static 	BYTE m_iThinkTime;//��Ϸ˼��ʱ��
	static 	BYTE m_iBeginTime;//��Ϸ��ʼʱ��	//��������

	//------------------------------���й���صĹ���------------------
	bool m_arybCutUser[PLAY_COUNT];     //����Ƿ����
	void InitAIData();
	virtual bool OnBeginUserNetCutAI(BYTE bDeskStation);  //���ش����йܺ���
    void ExcutNetCutAI();  //ִ���йܲ���
	void SuperUserIDJudge();
public:
	//���캯��
	CServerGameDesk(); 
	//��������
	virtual ~CServerGameDesk();

	//���غ���
public:
	//��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	//��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//�����Ϣ������
	virtual bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	//�û�����
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pUserInfo);
virtual bool InitDeskGameStation();
	//���ش��麯��
public:
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);

public:
	inline int GetLimit() {return (m_iLimitePerNote < m_iLimitNote ? m_iLimitePerNote : m_iLimitNote);}
	//������
public:
	//��ȡ��̬����
	static BOOL LoadIni();	//׼�����ƹ���
	//������ע
	BOOL LoadExtIni();
	//���ݷ���ID���ص�ע������
	BOOL LoadExtIni(int iRoomID);
	//�û�������Ϸ
	BOOL UserSetGame(UserArgeeGame * pGameSet);
	//�����˿˸��û�
	BOOL SendCard();
	//���ƽ���
	BOOL SendCardFinish();
	//��Ϸ��ʼ
	BOOL BeginPlayUpGrade();

	//��һ�ֿ�ʼ
	BOOL NewPlayTurn(BYTE bDeskStation);
	//��һ������
	BOOL SendACard ();
	//�����û���ע
	BOOL SendUserNote();
	//�û������뿪
	BOOL UserHaveThing(BYTE bDeskStation, char * szMessage);
	//ͬ���û��뿪
	BOOL ArgeeUserLeft(BYTE bDeskStation, BOOL bArgee);
	//�����û����
	BOOL UserActiveResult(BYTE bDeskStation, BYTE iVerbType,int iNoteVal=0,BYTE bComparedStation=255);
    //���ƽ��
	BOOL BiPaiActiveResult();
	//����Ƿ����
	UINT CheckFinish();   //�޸ļ���������ֵ
	//�����û�����ע
	void ModifyPlayNote(BYTE bDeskStation);
	//��ȡ������λ��(bExtVal��ʾ�Ƚ����ƻ��ǱȽ�ȫ����)
	int GetCardGreater(BOOL bExtVal=FALSE);
	//����λ��
	BOOL CanSendCard(BYTE bDeskStation);
	//ͳ����Ϸ����
	BYTE CountPlayer();
	//�ռ�����
	BOOL CollectJeton(BOOL bOnly=false);
	//������
	BOOL SuperSetCard(BYTE iTotalCard[],int iCardCount);
	//�Q��
	BOOL ChangeBackCard(BYTE bDeskStation,BYTE bSourceCard,BYTE bResultCard);
	//������Ƿ���ĳ���Ƿ���Q�ɹ�
	BOOL ReplaceCard(BYTE iTotalCard[],int iSendCardPos,int iEndPos,BYTE bSourceCard,BYTE bResultCard);
	//ͳ�Ƶ�ǰδ�������
	BYTE CountNotePeople();
	//ͳ�Ʊ���(��ͷ)
	int GetHumanBasePoint();
	//���汶��
	int GetDeskBasePoint();
	//���䱶��
	int GetRoomBasePoint();
	//���ܿ۷�
	int GetRunPublish();
	//�õ���һ��������ҵ�λ��
	BYTE GetNextNoteStation(BYTE bCurDeskStation);

	void FinishCompare();                          //��Ϸ����ǰ�ı��ƹ��� 

	//------------------------------------------
	//----������Ӯ--------
	BOOL JudgeWiner();															//�ж�ʤ��	
	BOOL JudgeLoser();															//�ж����
	BOOL ChangeCard(BYTE bDestStation,BYTE bWinStation);						//����	
	BYTE GetWiner();														//�õ�Ӯ��
	BYTE GetLoser();                                                         //�õ����
	bool SetUserCard();			// ��ȡ�����ļ������ڵ��Է���	Add by zxd 20090803
	BYTE m_bCutLastPeoPle;


	//add by ljl 
	/**
	*
	*
	* @return ������С�����ҵĽ��
	*/
	__int64 GetMinMoney();


	BYTE m_byWin[PLAY_COUNT];

	BOOL m_bIsShowIdOrPlay;
};

/******************************************************************************************************/
