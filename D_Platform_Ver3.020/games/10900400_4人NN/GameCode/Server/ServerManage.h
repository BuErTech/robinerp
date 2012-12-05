#pragma once

#include "StdAfx.h"
#include"..\GameMessage\UpgradeLogic.h"

//��ʱ�� ID
#define TIME_SEND_CARD				30				//���ƶ�ʱ��
#define TIME_CALL_NT				31				//��ׯ
#define TIME_GAME_FINISH			32				//��Ϸ������ʱ��
#define TIME_SEND_A_CARD			33				//����һ����
#define TIME_BEGIN_ANIMAL			34				//��Ϸ��ʼ����
#define TIME_COLLECT_JETON			35				//�ռ�����
#define TIME_CUTLINE_AUTO			36				//�����Զ���ţ
#define TIME_NOTE                   37              //��ע��ʱ��
/*
//��Ϸ������Ϣ
#define PLAY_MAX_NOTE				500			//�����ע
#define PLAY_BASE_NOTE				5				//��ע
#define THINK_TIME					30				//��Ϸ˼��ʱ��
#define PLAY_CARD_COUNT				1				//�˿˸���

*/

//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SALE						11				//��Ϸ��ȫ����

//
#define SH_USER_CARD                5               //�Ƶ�����


//#define SEND_CARD_TIME				1000			//����ʱ�����ò�����10����,������1000

//��Ϸ����
class CServerGameDesk : public CGameDesk
{
protected:
	//����״̬����
	int						m_iNtPeople;                          //Ӯ��
	int						m_iUpGradePeople;					//ׯ��λ��
	BYTE					m_iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	BYTE					m_iLeaveArgee;						//�뿪ͬ���־
	BYTE					m_iHaveThingPeople;					//����Ҫ�����
	//״̬��Ϣ
	BYTE					m_iUserCardCount[PLAY_COUNT];				//�û������˿���Ŀ
	BYTE					m_iUserCard[PLAY_COUNT][10];					//�û����ϵ��˿�
	int						m_iCallScore[PLAY_COUNT];                     //��ׯ״��
	//������Ϣ
	int						m_iOutCardPeople;					//���ڳ����û�
	int						m_iFirstOutPeople;					//�ȳ��Ƶ��û�
	//��������
	BYTE					m_iTotalCard[52];					//�ܵ���
	BYTE					m_iSendCardPos;						//����λ��
	BYTE                    m_bBullCard[PLAY_COUNT][3];                        //��Ұڵ�ţ��

	CUpGradeGameLogic		m_Logic;							//��Ϸ�߼�
	int						m_iGameStation;						//��Ϸ״̬
	int						m_iLastOutPeople;
    int                     m_MaxPlayer;                        //��ע�мҵ�����;
	int						m_iUserStation[PLAY_COUNT];				//��¼�û�״̬
	__int64						m_iPerJuTotalNote[PLAY_COUNT];			//�û�ÿ��ѹ��ע
	//int						m_iInsuranceNote[PLAY_COUNT];					//��ǰ���ս�
	//__int64					m_iTotalNote;					//��ע
	bool                    m_bAdd[PLAY_COUNT];                //�Ƿ�ӱ�
	bool                    m_bIsCut[PLAY_COUNT];              //�Ƿ��������ҪΪ����������
	BYTE					m_iIngPlayNote;					//��ǰ��ע�ߣ����Թۣ�
	BYTE					m_iLastShuttle;					//��һ������
	BYTE					m_iSendPos;				//��һ�ַ��ƿ�ʼλ��
	BYTE                    m_iPrepareNT;              //Ԥ��ׯ��
	BYTE					m_bCurrentCallScore;					//��ǰ�з���
	BYTE                    m_iFirstJiao;
	BYTE                    m_iGameFlag;             //��ǰ��Ϸ��״̬
	BOOL                    m_bIsSuper[PLAY_COUNT];   //�Ƿ����ʹ�÷���
	BOOL                    m_bSuperSet;              //�Ƿ������˷���
	BYTE                    m_bControlWin;             //������Ӯλ��
	BYTE                    m_bControlLost;             //������Ӯλ��


private:
	static BYTE m_iAllCardCount;	//��Ϸ���õ�����
	__int64 m_iLimitNote[PLAY_COUNT];	//����
	int m_iBaseNote;		//��ע
	//=============��չ
	static 	int m_iSendCardTime;//��Ϸ˼��ʱ��
	static 	BYTE m_iThinkTime;//��Ϸ˼��ʱ��
	static 	BYTE m_iBeginTime;//��Ϸ��ʼʱ��
	static 	BYTE m_iCallScoreTime;
	static 	BYTE m_iXiaZhuTime;
	static  BOOL m_bTurnRule;	//��Ϸ˳��
	DWORD m_iCardShape;	//����
	BYTE  m_GameType;
	/// ���ͱ���2010-08-03
	BYTE m_bCardShapeBase[MAX_SHAPE_COUNT];//0-10������ţ��ţţ��ı���
    BYTE  m_iGameNoteSetType;         //�Ƿ�����ţţ�����е����͵����ֵ 
	int  m_iGoodCard;//�����ƻ���ţ������
	bool m_bHaveKing;//�Ƿ�����
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
	//��������
public:
	//���캯��
	CServerGameDesk(); 
	//��������
	virtual ~CServerGameDesk();

	//���غ���
public:
	//��ʼ����Ϸ�߼�
	virtual bool InitDeskGameStation();
	//��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	//��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);
	//�����Ϣ������
	virtual bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	//
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo);
	//�û���������
	virtual bool UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo);

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
	//��ȡ��̬����
	BOOL LoadIni();	//׼�����ƹ���
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
	//��������
	int	GetUpGrade(int iGamePoint, int iPlayCardCount);
	//�����û����
	BOOL UserActiveResult(BYTE bDeskStation, BYTE iVerbType,int iNoteVal=0);
	//�����û���ע���
	BOOL UserNoteResult(BYTE bDeskStation, BYTE iVerbType,__int64 iNoteVal=0);
	//����Ƿ����
	BOOL CheckFinish();
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
	//����������
	BOOL SuperSetCard();
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
	//��ׯ��
	BOOL FindNt();
	//������ҷ���
	int ComputePoint(BYTE DeskStation);
	//�õ�����ƺ���С��,winֵΪ1ʱ�õ����,����Ϊ��С
	BYTE GetIndex(int win);
	//�ж�ʤ��
	BOOL JudgeWiner();
	//�ж����
	BOOL JudgeLoser();
	//����
	BOOL ChangeCard(BYTE bDestStation,BYTE bWinStation);
	//��ׯ
	BOOL UserCallScore(BYTE bDeskStation, int iVal);
	//��ȡ��һ�����λ��
	BYTE GetNextDeskStation(BYTE bDeskStation);
	//���͸���һ���з�
	BOOL SendCallScore(BYTE bDeskStation);
	//�зֽ���
	BOOL CallScoreFinish();
	//�û���ע
	BOOL UserNote();
	//������Ͷע�������
	__int64 GetLimit(int bDeskStation);
	//ͳ�Ƶ������
	BYTE CountCutPlayer();
	//���AI����
	BOOL UserAI(BYTE bDeskStation,BYTE bCutType,bool bNet);
	//���ǿ��
	BOOL UserFourceQuit(BYTE bDeskStation);
	//����ǿ�����
	void ClearQuitPlayer();
	///�������м�ʱ��
	void KillAllTimer();;

	///��ȡ��ǰ�����������ͣ�
	int GetMaxCardShape(BYTE iCard[], int iCardCount);
};

/******************************************************************************************************/
