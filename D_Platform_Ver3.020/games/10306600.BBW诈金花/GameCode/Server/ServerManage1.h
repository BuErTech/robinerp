#pragma once

#include "StdAfx.h"
#include"..\\GameMessage\\UpGradeLogic.h"
//��ʱ�� ID
#define TIME_SEND_CARD				30				//���ƶ�ʱ��
#define TIME_SEND_ALL_CARD			31				//��������
#define TIME_WAIT_NT				32				//�ȴ�����
#define TIME_GAME_FINISH			33				//��Ϸ������ʱ��
#define TIME_WAIT_NEWTURN			35				//��һ��ʱ������
#define TIMER_WAIT_JIAOFEN			36				//�ȴ��з�


#define TIME_WAIT_CONTRIBUTE		38				//�ֹ�����
#define TIME_WAIT_MOVE_BACK_CONTRIBUTE		39		//�ȴ��˹�
#define TIME_CHOICE_CARD			40				//ѡ��
#define TIME_BACK_CONTRIBUTE_WAIT   41              //�˹���ȴ�3��
#define TIMER_OUT_CARD				42				//ÿ�ֳ��Ƽ�ʱ�� //�˺�42-45Ϊ���ƶ�ʱ��

//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SALE						11				//��Ϸ��ȫ����
#define	GF_TUOXIANG					12				//��Ϸ��Ͷ��������
#define GF_LOSE						13				//Ͷ��
#define GF_NO_CONTINUE				14				//������Ϸ
/******************************************************************************************************/

//��Ϸ����
class CServerGameDesk : public CGameDesk
{
protected:
	//��Ϸ��������
	BYTE					m_iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	//����״̬����
	int						m_iDealtPeople;						//����λ��
	int						m_iNtPeople;						//�Ϲ�������
	int						m_iUpGradePeople;					//ׯ��λ��
	int						m_iGrade2;							//ׯ��ͬ��
	int						m_iOutCardPeople;					//���ڳ����û�
	int						m_iFirstOutPeople;					//�ȳ��Ƶ��û�
	BYTE					m_iPlayNtNum[2];					//����ҵ���������
	//״̬��Ϣ
	BYTE					m_iUserCardCount[4];				//�û������˿���Ŀ
	BYTE					m_iUserCard[4][54];					//�û����ϵ��˿�
	BYTE					m_iDeskCardCount[4];				//�����˿˵���Ŀ
	BYTE					m_iDeskCard[4][54];					//������˿�

	//������Ϣ(���ƺͽ���)
	BYTE					m_iPointCardList[4][54];			//�û�ץ�ַ���
	int						m_iPointCardCount[4];				//��������
	int						m_iUserGetPoint[4];					//�û���õķ�
	int						m_iTempPoint;						//�û���Ϸ����
	BYTE					m_iBaseOutCount;					//���Ƶ���Ŀ
	BOOL					m_iStateFlag;						//�ȴ����״̬�ֽ�(�з֣��л�,���)
	bool                    m_bIsCut[PLAY_COUNT];              //�Ƿ��������ҪΪ����������
	BYTE					m_iSequence[4];						//����ҵĳ��Ƶ��Ⱥ�˳��
	int						m_iJiaofen[4];						//���ҽз�
	int						m_PunishPoint[4];					//�û�����
	int						m_iAwardPoint[4];					//���μӷ�
	BYTE					m_iHeads[4][10];					//ͳ��4-8ը����
	BYTE					m_iBackCard[12];					//����
	BYTE					m_ChangeCard[4];					//���Ž�������
	//��������
	BYTE					m_iSendCardPos;						//����λ��
	BYTE					m_bCardOfFriend[4];					//����

	BYTE                    m_SetCardTemp[PLAY_COUNT][54];          //���ñ�������
	BYTE                    m_SetCardCountTemp[PLAY_COUNT];          //���ñ�������

	CUpGradeGameLogic		m_Logic;							//��Ϸ�߼�
	///������չ����
	BOOL					m_bAdvanceFinish;					//��ǰ����
//////////////////////////////////��������///////////////////////////////////
	BOOL					m_iLastCard[4];						//���һ����(�����������)
	BYTE					m_iHaveThingPeople;					//����Ҫ�����
	BOOL					m_bAutoOutCard[4];					//�й�����
	BYTE					m_iLeaveAgree;						//�뿪ͬ���־
	BYTE					m_iLoseAgree;						//Ͷ��ͬ���־
	//����10����Ϸ�и�����������Σ��������
	BYTE					m_iSequenceHistory[255][4];			//����˳����ʷ��¼
	BYTE					m_iHistory;							//������ʷ��¼��
	BOOL					m_bWhoIsBig;						//�Ϲ���˭�Ϲ��Ĵ�(��һ���(winer+1)%4,�ڶ������(winer+3)%4)
	BYTE                    m_iOverContribute;                  //����Ϲ�������
	BYTE                    m_ContributeCount;                  //Ҫ�Ϲ�������
	BYTE                    m_iWinSequence[PLAY_COUNT][2];      //���κ�δ�εĴ���
	BYTE                    m_PlayANumber[2];                   //��A���������һ���Ĵ���
	BYTE                    m_ChoiceNumber;                     //ͳ��ѡ��ʱ��ѡ������
    BYTE					m_iBeginNt;		                    //��Ϸ��ʼ
    BYTE					m_iEndNt;		                    //��Ϸ�յ�
	BYTE                    m_iTempTime;                       //��ʱʱ��

	BOOL                    m_bInSetCard;                      //��ǰ����������
	BOOL                    m_bCanSetCard;                     //��ǰ���Ƿ��������
	


	//��ȡ����,����,����,����λ��
	BYTE GetPostionBySequence(BYTE iSequence);
	//�Ϲ�
	BOOL SendContribute();//ShangGong();
	//����
	BOOL GainstContribute(BYTE bLose,int count);//CanKangGong();
	//�ֹ�����
	BOOL UserContribute(BYTE bDeskStation,BYTE bCard);
	//��ʱδ������
	BOOL SystemContribute();									//ϵͳ����δ�˹���
	//�������
	BOOL ContributeFinish();			
	//���ѡ��(˭ѡ��,ѡ��˭����,ѡ�Ƶ�ֵ
	BOOL UserChoiceCard(BYTE bDeskStation,BYTE bDest,BYTE bCard);
	//ϵͳѡ��
	BOOL SystemChoiceCard();
	//�����˹�
	BOOL SendMoveBackContribute();
	//�˹�
	BOOL UserMoveBackContribute(BYTE bDeskStation,BYTE bCard);//TuiGong();
	//��ʱδ�˹���
	BOOL SystemMoveBackContribute();							//ϵͳ����δ������
	//�˹����
	BOOL MoveBackContributeFinish();			
	//��̬����
private:
	static BOOL m_bHaveKing;//�Ƿ�����(DEF=1,0)
	static BOOL m_bKingCanReplace;//���ɷ������(DEF=1,0)
	static UINT m_iPlayCard;//�����˿˸���(1,DEF=2,3
	static UINT m_iPlayCount;//ʹ���˿���(52,54,104,DEF=108)
	static UINT m_iSendCount;//������(48,51,DEF=100,108)
	static UINT m_iBackCount;//������(3,6,DEF=8,12)
	static UINT m_iUserCount;//�����������(12,13,DEF=25,27)
	static BYTE m_iLimitUpGrade;//������������ټ�
	static DWORD m_iCardShape1;	//����
	static DWORD m_iCardShape2;	//����
	//=============��չ
	static 	BYTE m_iThinkTime;//��Ϸ˼��ʱ��
	static 	BYTE m_iBeginTime;//��Ϸ��ʼʱ��
	static  BYTE m_EndWaitTime;//��Ϸ������ȴ�ʱ��
	static  BYTE m_ContributeTime;//��Ϸ����ʱ��
	static  UINT m_iBaseMult;	//��Ϸ����

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
	/// �ж��Ƿ�������Ϸ
	/// 
	/// @param[in]		BYTE bDeskStation
	/// @return			bool
	virtual bool IsPlayGame(BYTE bDeskStation);
	/// �ж��Ƿ�������Ϸ
	virtual bool IsPlayGameEx(BYTE bDeskStation);
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//���ؿ����Ϣ
	bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);;
	//�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);

	//
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo);

	///�û����½���
	virtual  bool UserReCome(BYTE bDeskStation, CGameUserInfo *pLostUserInfo);


	//���ش��麯��
public:
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	//������
public:
	//��ʼ��һϵ�й���(���¿�ʼ��Ϸ�Ž��д��ֳ�ʼ��)
	//BOOL InitThisGame();
	//��ȡ��̬����
	static BOOL LoadIni();	//׼�����ƹ���
	BOOL LoadExtIni(UINT uRoomID);   //���ݷ���ID���������ļ�

	BOOL Ready();
	//ȫ������
	BOOL SendAllCard( BYTE iStyle);
	//�������
	BOOL SendCardFinish();
	//�ܷ���ǰ����(����ץ��4��������7��ͷ��8��ͷ)
	//BOOL CanAdvanceFinish();
	//��Ϸ��ʼ
	BOOL BeginPlayUpGrade();
	//�û�����
	BOOL UserOutCard(BYTE bDeskStation, BYTE iOutCard[], int iCardCount);
	////�����ģ�����
	BOOL UserAutoOutCard(BYTE bDeskStation);
	//////////////////////////���ƹ��ܺ���///////////////////////////////
	//�����Ƿ�Ϊ�ӷ���
	int IsAwardPointCard(BYTE iOutCard[],int iCardCount,BYTE bDeskStation);
	//�ӷ�
	BOOL IsGamePoint(BYTE iOutCard[],int iCardCount,BYTE bDeskStation);
	//�Ƿ�Ϊ���һ����
	BOOL IsLastCardRemind(BYTE bDeskStation,BYTE iCardList[],int iCardCount);
	//ĳλ���Ѿ�����
	BOOL OutCardFinish(BYTE bDeskStation);
	//����Ƿ���ƽ���
	BOOL CheckFinish();
	//�Ƿ�Ϊ��һ��
	BOOL IsNewTurn();
	//��һ��������(��Aλ���Ѿ�����,�����¼ҽӷ�,���¼�Ҳ�Ѿ�����,�������¼ҽӷ�)
	BYTE GetNextOutCardPeople(BYTE bDeskStation);

	////////////////////////////���ƹ��ܺ������///////////////////////////////////
	//��һ�ֿ�ʼ
	BOOL NewPlayTurn(int iWiner);

	//�����м�ʱ��
	void KillAllTimer();
public:
	//δ����ʱ����������
	BOOL FindKingCount();
	//�ӷ��ж�
	BOOL AwardPoint_OutCard(BYTE iOutCard[],int iCardCount,BYTE bDeskStation);
	//�����Ƿ�Ϊͬһ�ֻ�ɫ
	BOOL TestSameColor(BYTE iCardList[],int iCardCount,int iCardNum);
	//�Ƿ�ͬһ�ֻ�ɫ
	BOOL IsSameColor(BYTE iCardList[],int iCardCount);

	///////////////////////////������غ���/////////////////////////////
	//�p��߀�ǆοۻ�ƽ��
	int StatWin();
	//ͳ���������ξ���
	BYTE CountSequence(BYTE bDeskStation ,BYTE FirstOrLast);
	//ͳ�Ƶ�ǰһ����������
	BYTE CountSequence(BYTE iFirstOrLast);
	//���㽱��
	int StatAwardPoint(BYTE bDeskStation);
	//////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////��Ϸ��������/////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//�й�
	bool UserAuto(BYTE bDeskStation,BOOL bAuto);
	//�û������뿪
	BOOL UserHaveThing(BYTE bDeskStation, char * szMessage);
	//ͬ���û��뿪
	BOOL AgreeUserLeft(BYTE bDeskStation, BOOL bArgee);
	//������ǰ����
	BOOL UserStop(BYTE bDeskStation,BOOL bAgree= false);
	//ͬ����ǰ����
	BOOL AgreeUserStop(BYTE bDeskStation,BOOL bAgree= false);
	//Ͷ��
	BOOL UserLose(BYTE bDeskStation,BOOL bAgree= false);
	//ͬ��Ͷ��
	BOOL AgreeUserLose(BYTE bDeskStation,BOOL bAgree= false);
	//�������
	BOOL UserBuyProp(BYTE bDeskStation,BYTE iLevel = 0);
	//ͳ�Ʊ���(��ͷ)
	int GetHumanBasePoint();
	//���汶��
	int GetDeskBasePoint();
	//���䱶��
	int GetRoomBasePoint();
	//���ܿ۷�
	int GetRunPublish();
	//��ǿ����ҵ÷�
	int GetRunAwayOtherGetPoint(BYTE bDeskStation,BYTE bRunDeskStation);
	//�����¾���������
	BOOL SetNextNTNum(int iNextNtNum);
	//�����¾�ׯ��λ�� 
	BOOL SetNextNTStation(int iTurePoint);
	//����ĩ������
	//void LastSequence();
		//ͳ����Ϸ����
	BYTE CountPlayer();
		//ͳ�Ƶ������
	BYTE CountCutPlayer();
	//���AI����
	BOOL UserAI(BYTE bDeskStation,BYTE bCutType);
	virtual bool InitDeskGameStation();
};

/******************************************************************************************************/
