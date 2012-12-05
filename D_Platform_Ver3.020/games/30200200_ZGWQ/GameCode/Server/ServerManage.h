#pragma once

#include "StdAfx.h"
#include "logic.h"
//��ʱ�� ID
#define TIME_SEND_CARD				30				//���ƶ�ʱ��
#define TIME_WAIT_NT				31				//�ȴ�����
#define TIME_GAME_FINISH			32				//��Ϸ������ʱ��


//�˿���Ŀ����
#define PLAY_TWO_CARD				1				//�����˿�
#define PLAY_THREE_CARD				2				//�����˿�

//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SALE						11				//��Ϸ��ȫ����
#define GF_SUPER_TIMER		12				//��ʱ����
#define GF_LOSER					13				//����

#define BASE_SCORE				10				//�׷�(����)
//*****************************************************************************************

//��Ϸ����
class CServerGameDesk : public CGameDesk
{
protected:
	//�����������
	BYTE					m_iLessCount;						//������Ϸ����
	BYTE					m_iPlayCardCount;					//��Ϸ�˿���Ŀ
	BYTE					m_iBeginStation;					//��Ϸ��ʼλ��
	BYTE					m_iEndStation;						//��Ϸ����λ��
	BYTE					m_iEnableWatch;						//�Ƿ������Թ�

	BYTE					m_iHaveThingPeople;						//����Ҫ�����

	int						m_iJuTimeMinute,//��ʱ
							m_iJuTimeSecond,
							m_iBuTimeMinute,//��ʱ
							m_iBuTimeSecond,
							m_iDuMiaoMinute,//����
							m_iDuMiaoSecond;

	//����״̬����
	int						m_iUpGradePeople;					//ׯ��λ��
	BYTE					m_iPlayNTNum[PLAY_COUNT];					//����ҵ�������Ŀ
	BYTE					m_iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	BYTE					m_iLeaveArgee;						//�뿪ͬ���־
	int						m_iOutCardPeople;					//���ڳ����û�
	CUpGradeGameLogic		m_Logic;							//��Ϸ�߼�

	int						m_iWinFlag;//��ʤ��
	int						m_iTurePoint[PLAY_COUNT];//��ʵ�÷�

	bool					m_bFirstRed;
	bool					m_bQiuHe;
	
	//wq
	int						m_iBoard[21][21];
	int						m_idianmu[21][21];
	int						m_iQiZiNum[PLAY_COUNT];//ͳ�ƺڰ��������µ����������������ӣ�
	int						m_iQiZiLastNum[PLAY_COUNT];//����˫��������
	int						m_iTiZiNum[PLAY_COUNT];

	bool                    m_bUserNetCut[PLAY_COUNT];  //�û�����
	int                     m_bAgreeDianmuFinish;             //-1��ʾ���˵����0��ʾ�ѻ�Ӧ�����Ŀ��1��ʾû�л�Ӧ���

	//��������
	int						m_iLastDeskStation;//�ϲ������ߵ�λ��
	int						m_iLastBoard[21][21];//�ϲ����
	int						m_iLastTiZiNum[PLAY_COUNT];//�ϲ�������
	int						m_Endi;
	int						m_Endj;

	int						m_iFinishNumCnt;//��ɵ�Ŀ
	int						m_iDianMuResult;//��Ŀ�������
	CString					m_dianmumsg;
	int						m_Qiflag[PLAY_COUNT];

	int						m_iBlackStation;			//����λ��
	int						m_iWhiteStation;			//����λ��

	UsedTimeInfo			m_stUsedTimeInfo;//�Ѿ�ʹ�õ�ʱ����Ϣ

	bool					m_bIsInHuiquStation[2];				///< �Ƿ��ڻ���״̬
	bool					m_bChessingInHuiQi[2];
	
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
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);

	//���ش��麯��
public:
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	//������
	//��Ҷ��ߴ���
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
   //�û����½�����Ϸ
	virtual  bool UserReCome(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
public:
	//�û�������Ϸ
	BOOL UserSetGame(UserArgeeGame * pGameSet,BYTE desk);
	//��Ϸ��ʼ
	BOOL BeginPlay();
	//�û�����
	BOOL UserOutCard(BYTE bDeskStation, int iPlayQi[][21], int iPlayFlag, int iEndI, int iEndJ);
	//�û������뿪
	BOOL UserHaveThing(BYTE bDeskStation, char * szMessage);
	//ͬ���û��뿪
	BOOL ArgeeUserLeft(BYTE bDeskStation, BOOL bArgee);
	//��������
	int	GetUpGrade(int iGamePoint, int iPlayCardCount);

	//��Ϸ׼������
	BOOL GameReady();

	//ǿ������
	void ForceNumberZi(int &iWinFlag, CString &strMessage);
public:
	//�������м�ʱ��
	void KillAllTimer();
	//���汶��
	int GetDeskBasePoint();
	//���䱶��
	int GetRoomBasePoint();
	//���ܿ۷�
	int GetRunPublish();
	//���ܿ۷�
	int GetRunPublishMoney();
};
