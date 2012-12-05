#pragma once

#include "StdAfx.h"
#include "..\\GameMessage\\UpGradeLogic.h"
//��ʱ�� ID
#define TIME_SEND_CARD				30				//���ƶ�ʱ��
#define TIME_SEND_ALL_CARD			31				//��������
#define TIME_WAIT_CALL				32				//�ȴ�����
#define TIME_ROB_NT					33				//������
#define TIME_ADD_DOUBLE				34				//�Ӱ�
#define TIME_OUT_CARD				35				//����
#define TIME_WAIT_NEWTURN			36				//��һ��ʱ������
#define TIME_GAME_FINISH			37				//��Ϸ������ʱ��

#define TIME_START_GAME				40				//������ʼʱ������
//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SAFE						11				//��Ϸ��ȫ����
#define GF_NO_CALL_SCORE			13				//���˽з�
#define GF_TERMINATE				14				//����ֹͣ��Ϸ
#define GF_AHEAD_END				15				//��ǰ����
////��Ϸ����
//#ifdef FKDDZ
//#define m_bRobnt			    	true			//�Ƿ������
//#define m_bAdddouble				true			//���ԼӰ�
//#define m_bShowcard				true			//�Ƿ��������
//#else
//#define m_bRobnt				    false			//�Ƿ������
//#define m_bAdddouble				false			//���ԼӰ�
//#define m_bShowcard				false			//�Ƿ��������
//#endif
//*****************************************************************************************
//��Ϸ����

class CServerGameDesk : public CGameDesk
{
protected:
	//����״̬����
	BYTE					m_iBeenPlayGame;						//�Ѿ���Ϸ�ľ���
	BYTE					m_iLeaveArgee;							//�뿪ͬ���־
	//״̬��Ϣ
	BYTE					m_iUserCardCount[PLAY_COUNT];			//�û������˿���Ŀ
	BYTE					m_iUserCard[PLAY_COUNT][45];			//�û����ϵ��˿�
	BYTE					m_iBackCard[12];						//�����б�
	BYTE					m_iBaseOutCount;						//���Ƶ���Ŀ
	BYTE					m_iDeskCardCount[PLAY_COUNT];			//�����˿˵���Ŀ
	BYTE					m_iDeskCard[PLAY_COUNT][45];			//������˿�
	BYTE					m_iBaseCard[PLAY_COUNT][45];			//���һ���˿�
	BOOL					m_iTurnFirstOut;						//�Ƿ��Ǳ��ֵ�һ�γ���(090402b2 JJ)
	//��������
	BYTE					m_iSendCardPos;							//����λ��
	BYTE					m_iBase;								//����
	BYTE					m_iRobMul;								//��������������
	BYTE					m_iBombMul;								//ը������
	BYTE					m_iRobNT[PLAY_COUNT];					//�����������Ʊ���
	BYTE					m_iPeopleMul[PLAY_COUNT];				//��ҼӰ����
	BYTE					m_iAIStation[PLAY_COUNT];				//�ļһ����й�״̬
	BYTE					m_iHaveThingPeople;						//����Ҫ�����
	BYTE					m_iGameFlag;							//��Ϸ״̬С�ֽ�
	int                     m_iArrIsStartDisplay[PLAY_COUNT];          //���ƿ�ʼ״̬
	
	BYTE					m_bThrowoutCard;						//����
	BYTE					m_bFirstCallScore;						//��һ���е�����
	BYTE					m_bCurrentCallScore;					//��ǰ�з���
	BYTE					m_iPrepareNT;							//Ԥ��ׯ��
	BYTE                    m_iBombCard[4][10];                         //0,����,1.���,2.Ӳը,3.��ը
	int                     m_iBombCount;                           //Ӳը��
	int                     m_iShamBomCount;                        //��ը��

	BYTE					m_byteHitPass ;							//��¼����

	int						m_iUpGradePeople;						//ׯ��λ��
	int						m_iNtFirstCount;						//�������ĵ�һ������
	//������Ϣ
	int						m_iOutCardPeople;						//���ڳ����û�
	int						m_iFirstOutPeople;						//�ȳ��Ƶ��û�
	int						m_iNowBigPeople;						//��ǰ��������������
	int						m_iRecvMsg;								//�յ���Ϣ����
	
	CUpGradeGameLogic		m_Logic;								//��Ϸ�߼�

	int						m_iCallScore[PLAY_COUNT];
	int						m_iAwardPoint[PLAY_COUNT];				//���μӷ�
	bool					m_bAuto[PLAY_COUNT];					//�й�����
	//������ʹ�ñ���
	int						m_iWinPoint[PLAY_COUNT];				//ʤ��
	int						m_iNtPeople;							//�������
	int						m_iDealPeople;							//�������
	int                     m_icountleave;                           //���뿪����ͳ��

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
	//��̬����
private:
	static bool m_bHaveKing;//�Ƿ�����(DEF=1,0)
	static BOOL m_bKingCanReplace;//���ɷ������(DEF=1,0)
	static bool m_bRobnt;//�Ƿ����������
	static bool m_bAdddouble;//�Ƿ���ԼӰ�
	static bool m_bShowcard;//�Ƿ����������
	static UINT m_iPlayCard;//�����˿˸���(1,DEF=2,3
	static UINT m_iPlayCount;//ʹ���˿���(52,54,104,DEF=108)
	static UINT m_iSendCount;//������(48,51,DEF=100,108)
	static UINT m_iBackCount;//������(3,6,DEF=8,12)
	static UINT m_iUserCount;//�����������(12,13,DEF=25,27)
	static BYTE m_iLimitUpGrade;//������������ټ�
	static DWORD m_iCardShape;	//����
	//=============��չ
	static 	BYTE m_iThinkTime;//��Ϸ˼��ʱ��
	static 	BYTE m_iBeginTime;//��Ϸ��ʼʱ��
	static  BYTE m_iGiveBackTime;//����ʱ��
	static  BYTE m_iCallScoreTime;//����ʱ��
	static  BYTE m_iAddDoubleTime;//�Ӱ�ʱ��
	static  int  m_iBaseMult;	//��Ϸ�����ⶥ
	static  BOOL m_bTurnRule;	//��Ϸ˳��
	int  	m_iLimitPlayGame;	//���ٴ�����پ�

	//��������
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
	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGameEx(BYTE bDeskStation);
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	virtual bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);

	//�û���������
	virtual bool UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo);
	//��ʼ��
	//virtual bool OnInit();

	//���ش��麯��
public:
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	//����INI�ļ�
	virtual bool InitDeskGameStation();
	//������
public:
	//�û�������Ϸ
	BOOL UserSetGame(UserArgeeGame * pGameSet);
	//������Զ���ʼ���ɷ���˿���
	BOOL StartGame();
	//��Ϸ׼������
	BOOL GamePrepare();
	//���ƹ�����
	BOOL SendCardMsg(BYTE bDeskStation,BYTE bCard);
	//��ʼ��һϵ�й���(���¿�ʼ��Ϸ�Ž��д��ֳ�ʼ��)
	BOOL InitThisGame();
	//��ȡ��̬����
	static BOOL LoadIni();
	//������ע
	BOOL LoadExtIni();
	//���ݷ���ID���ص�ע������
	BOOL LoadExtIni(int iRoomID);
	//�����˿˸��û�
	BOOL SendCard();
	//һ�ν�������ȫ������
	BOOL SendAllCard();
	//�l�ͽY��
	BOOL SendCardFinish();
	//���ͽз�
	BOOL SendCallScore(BYTE bDeskStation);
	//�û��Ͻ��з�
	BOOL UserCallScore(BYTE bDeskStation, int iVal);
	//�зֽ���
	BOOL CallScoreFinish();
	//������������Ϣ
	BOOL SendRobNT(BYTE bDeskStation);
	//�����������Ϣ
	BOOL UserRobNT(BYTE bDeskStation, int iVal);
	//����������
	BOOL RobNTFinish();
	//���͵���
	BOOL SendBackCard();
	//���ͼӰ���Ϣ
	BOOL SendAddDouble();
	//��ҼӰ�
	BOOL UserAddDouble(BYTE bDeskStation, int iVal);
	//�Ӱ����
	BOOL AddDoubleResult();
	//�Ӱ�����
	BOOL AddDoubleFinish();
	//����
	BOOL SendShowCard();
	//����
	BOOL UserShowCard(BYTE bDeskStation,int iValue);
	//���ƽ���
	BOOL ShowCardFinish();
	//��Ϸ��ʼ
	BOOL BeginPlay();
	//�û�����
	BOOL UserOutCard(BYTE bDeskStation, BYTE iOutCard[],  int iCardCount, BYTE iReplaceCardList[],bool bIsReplace);
	//��һ�ֿ�ʼ
	BOOL NewPlayTurn(BYTE bDeskStation);
	//�û������뿪
	BOOL UserHaveThing(BYTE bDeskStation, char * szMessage);
	//ͬ���û��뿪
	BOOL ArgeeUserLeft(BYTE bDeskStation, BOOL bArgee);
	//�û��й�
	bool UserSetAuto(BYTE bDeskStation,bool bAutoCard);
	//�����־��
	//	bool FindAllInfo(BYTE iCardList[],int iCardCount,GamePlayInfo  &iWriteInfo);
	//�Ƿ����һ݆
	BOOL IsNewTurn();
	//������Ӌ�r��
	void KillAllTimer();
	//�ӷ��ж�
	BOOL IsAwardPoin(BYTE iOutCard[],int iCardCount,BYTE bDeskStation);
	//ͳ�Ʊ���(��ͷ)
	int GetHumanBasePoint();
	//���汶��
	int GetDeskBasePoint();
	//���䱶��
	int GetRoomMul();
	//���ܿ۷�
	int GetRunPublish();
	//���ܿ۷�
	int GetRunPublish(BYTE bDeskStation);
	//��ǿ����ҵ÷�
	int GetRunAwayOtherGetPoint(BYTE bDeskStation,BYTE bRunDeskStation);
	//������ҵ÷�
	int GetTruePoint(BYTE bDeskStation);
	//������һ��ׯ��
	BOOL SetNextBanker(BYTE bGameFinishState = 0);
	//��ȡ��һ��ׯ��
	BYTE GetNextBanker();
	//��ȡ�����������
	BYTE GetGameWiner(BOOL &bCut);
	//�������й�
	BOOL UseAI(BYTE bDeskStation );
	//��ҳ�ʱ����(ȫ���ɷ���˽ӹ�)
	BOOL UserAutoOutCard(BYTE bDeskStation);
	//����ȡ���������й�
	virtual BOOL SetAIMachine(BYTE bDeskStation,BOOL bState = FALSE);
	//��Ҷ��ߴ���
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
	//��ȡ��һ�����λ��
	BYTE GetNextDeskStation(BYTE bDeskStation);
	//��ȡһ��������λ��
	BYTE GetRobNtDeskStation(BYTE bDeskStation);
	//�Ƿ�Ϊδ������
	BYTE GetNoOutCard();
////����
//bool UserProp(BYTE bsendDeskStation,BYTE brecvDeskStation,int iValue);
};

/******************************************************************************************************/
