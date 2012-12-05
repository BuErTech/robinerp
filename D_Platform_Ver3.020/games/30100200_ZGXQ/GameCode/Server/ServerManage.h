#pragma once

#include "StdAfx.h"

/// ��ʱ�� ID
#define TIME_SEND_CARD				30				///< ���ƶ�ʱ��
#define TIME_WAIT_NT				31				///< �ȴ�����
#define TIME_GAME_FINISH			32				///< ��Ϸ������ʱ��

/// ��������
#define	UG_HUA_MASK					0xF0			///< 1111 0000
#define	UG_VALUE_MASK				0x0F			///< 0000 1111

/// �˿˻�ɫ
#define UG_FANG_KUAI				0x00			///< ����	0000 0000
#define UG_MEI_HUA					0x10			///< ÷��	0001 0000
#define UG_HONG_TAO					0x20			///< ����	0010 0000
#define UG_HEI_TAO					0x30			///< ����	0011 0000
#define UG_NT_CARD					0x40			///< ����	0100 0000
#define UG_ERROR_HUA				0xF0			///< ����  1111 0000

/// �˿˳�������
#define UG_ERROR_KIND				0				///< ����
#define UG_ONLY_ONE					1				///< ����
#define UG_DOUBLE					2				///< ����
#define UG_THREE					3				///< ����
#define UG_DOUBLE_TRACKOR			4				///< ����������
#define UG_THREE_TARCKOR			5				///< ����������
#define	UG_SHOW_CARD				6				///< ˦��

/// �˿���Ŀ����
#define PLAY_TWO_CARD				1				///< �����˿�
#define PLAY_THREE_CARD				2				///< �����˿�

/// ��Ϸ������־����
#define GF_NORMAL					10				///< ��Ϸ��������
#define GF_SALE						11				///< ��Ϸ��ȫ����


/******************************************************************************************************/

enum XQ_PLAY_FLAG
{
	XQ_PLAY_FLAG_RED = 0,
	XQ_PLAY_FLAG_BLACK = 1
};


/// ����
#define XQ_FLAG_NONE				0
#define XQ_FLAG_RED					1
#define XQ_FLAG_BLACK				-1

#define XQ_QIZI_NONE				0
#define XQ_CROSS					8

/// �췽
#define XQ_RED_JU					1
#define XQ_RED_MA					2
#define XQ_RED_PAO					3
#define XQ_RED_SHI					4
#define XQ_RED_XIANG				5
#define XQ_RED_JIANG				6
#define XQ_RED_BIN					7

/// �ڷ�
#define XQ_BLACK_JU					-1
#define XQ_BLACK_MA					-2
#define XQ_BLACK_PAO				-3
#define XQ_BLACK_SHI				-4
#define XQ_BLACK_XIANG				-5
#define XQ_BLACK_JIANG				-6
#define XQ_BLACK_BIN				-7

#define XQ_MAX_I					(10)
#define XQ_MAX_J					(9)

/******************************************************************************************************/
/******************************************************************************************************/

/// �����߼��� ��֧�� 2 ���� 3 ���˿ˣ�
class CUpGradeGameLogic
{
	/// ��������
private:
	
	/// ��������
public:
	/// ���캯��
	CUpGradeGameLogic(void);	

	/// ���ܺ���������������
public:

	/// ��ʼ�����
	void InitBoard(int iMeFlag, int qi[][9]);
	/// check win or lose
	BOOL CheckWinOrLose(int iMeFlag, int qi[][9], BOOL &bIfIWin);
	
private:
	/// win
	BOOL Win(int iMeFlag, int qi[][9]);
	/// lose
	BOOL Lose(int iMeFlag, int qi[][9]);
	
};

//*****************************************************************************************

/// ��Ϸ����
class CServerGameDesk : public CGameDesk
{
protected:
	/// ��Ϸ��������
	BYTE					m_CardCount;						///< �˿���Ŀ��λ������
	/// �����������
	BYTE					m_iLessCount;						///< ������Ϸ����
	BYTE					m_iPlayCardCount;					///< ��Ϸ�˿���Ŀ
	BYTE					m_iBeginStation;					///< ��Ϸ��ʼλ��
	BYTE					m_iEndStation;						///< ��Ϸ����λ��
	BYTE					m_iEnableWatch;						///< �Ƿ������Թ�

	int						m_iJuTimeMinute,					///< ��ʱ
							m_iJuTimeSecond,
							m_iBuTimeMinute,					///< ��ʱ
							m_iBuTimeSecond,
							m_iDuMiaoMinute,					///< ����
							m_iDuMiaoSecond,
							m_iGameMoney;						///��Ϸָ����

	int						m_iBotJuUseTime[PLAY_COUNT];		///< ��Ϸ��ʱ
	int						m_iBotBuUseTime[PLAY_COUNT];
	int						m_iBotDuUseTime[PLAY_COUNT];

	int m_iStartI,m_iStartJ,m_iEndI,m_iEndJ ;

	/// ����״̬����
	int						m_PunishPoint[4];					///< �û�����
	int						m_iUpGradePeople;					///< ׯ��λ��
	int                     m_iBgePeaceCount[2];                ///< ��ʹ���
	BYTE					m_iPlayNTNum[2];					///< ����ҵ�������Ŀ
	BYTE					m_iBeenPlayGame;					///< �Ѿ���Ϸ�ľ���
	BYTE					m_iLeaveArgee;						///< �뿪ͬ���־

	/// ������Ϣ
	int						m_iNtPeople;						///< �������
	BYTE					m_iNtCardCount;						///< ��������

	/// ״̬��Ϣ
	BYTE					m_iUserCardCount[4];				///< �û������˿���Ŀ
	BYTE					m_iUserCard[4][45];					///< �û����ϵ��˿�
	BYTE					m_iBackCard[8];						///< �����б�

	/// ������Ϣ
	int						m_iGamePoint;						///< �û���Ϸ����
	int						m_iOutCardPeople;					///< ���ڳ����û�
	int						m_iFirstOutPeople;					///< �ȳ��Ƶ��û�
	BYTE					m_iBaseOutCount;					///< ���Ƶ���Ŀ
	BYTE					m_iDeskCardCount[4];				///< �����˿˵���Ŀ
	BYTE					m_iDeskCard[4][39];					///< ������˿�

	/// ��������
	BYTE					m_iSendCardPos;						///< ����λ��

	CUpGradeGameLogic		m_Logic;							///< ��Ϸ�߼�
	bool					m_bQueryQuit;						///< ����������˳�


	int						m_iRedBoard[10][9];					///< �췽���
	int						m_iBlackBoard[10][9];				///< �ڷ����
	
	int						m_iWinFlag;							///< ��ʤ��
	int						m_iTurePoint[2];					///< ��ʵ�÷�

	bool					m_bFirstRed;
	bool					m_bQiuHe;
	bool                    m_bRequireCoachMoeny;                ///�Ƿ���Ҫָ����

	int						m_iRedLastQiZi[10][9];				///< �췽�ϲ����
	int						m_iBlackLastQiZi[10][9];			///< �ڷ��ϲ����
	int						m_iLastPlay;						///< �ϲ�������
	int						m_iLastDeskStation;					///< �ϲ������ߵ�λ��
	int						m_iWatchStation;					///< �Թ۵�
	int 					m_iCurPlayFlag;						///< ��ǰ�����
	char                    m_bStationRegret;                  ////<�Ƿ��ڻ���״̬

	/// ����Ѿ�����Ĳ���        
	/// �涨�췽����Ϊm_iPutChess[1],�ڷ�Ϊ m_iPutChess[0]
	int                     m_iPutChess[2];

	int						m_firstPlay;						///<�Ƿ�Ϊ��1��ͬ�����
	int                     m_daoshi_money;						///<��ʦ��
	int						m_daoshi;							///<˭�ǵ�ʦ
	/// ��������
public:
	/// ���캯��
	CServerGameDesk(); 
	/// ��������
	virtual ~CServerGameDesk();
	//����INI�ļ�
	virtual bool InitDeskGameStation();
	//������ע
	BOOL LoadExtIni();
	//���ݷ���ID���ص�ע������
	BOOL LoadExtIni(int iRoomID);

	/// ���غ���
public:
	/// ��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	/// ��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	/// �ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);
	/// ��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	/// �û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);

	/// ���ش��麯��
public:
	/// ��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	/// ������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	/// ��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	/// ������
public:
	/// �û�������Ϸ
	BOOL UserSetGame(UserArgeeGame * pGameSet,byte desk);
	/// �����˿˸��û�
	BOOL SendCard();
	/// �û�����
	BOOL UserNtCard(BYTE bDeskStation, int iHuaKind, int iCardCount);
	/// ׼������
	BOOL GameReady();
	/// ��Ϸ��ʼ
	BOOL BeginPlayUpGrade();
	/// �¼Ӻ��������������û����Ӻ�Ĳ�������������
	BOOL UserOutCard(BYTE bDeskStation,PlayChessStruct * playChessInfo);
	/// �û�����
// 	BOOL UserOutCard(BYTE bDeskStation,int iStartI,int iStartJ,int iEndI, int iEndJ,int iCurPlay, int iBoard[][9]);
	/// �ܷ����(�������Լ����)
	BOOL CanFinish(BYTE PutStoneDeskStation,BOOL bCanFinish = false);
	/// ���նˁ�yӋ�Ƿ�Y��
	BOOL IsFinish(int iEndI,int iEndJ,int Value);
	/// ���͵���
	BOOL SendBackCard();
	/// �û��Ͻ�����
	BOOL UserGiveBackCard(BYTE bDeskStation, BYTE iBackCard[], int iCardCount);
	/// ��һ�ֿ�ʼ
	BOOL NewPlayTurn(int iWiner);
	/// �û������뿪
	BOOL UserHaveThing(BYTE bDeskStation, char * szMessage);
	/// ͬ���û��뿪
	BOOL ArgeeUserLeft(BYTE bDeskStation, BOOL bArgee);
	/// ��������
	int	GetUpGrade(int iGamePoint, int iPlayCardCount);
	/// ���䱶��
	int GetRoomBasePoint();
	/// ���ӱ���
	int GetDeskBasePoint();
	/// ��ͷ����
	int GetHumanBasePoint();
	/// ���ܿ۷�
	int GetRunPublish();
	/// ��ȡ���ַ���
	int GetTruePoint(BYTE bDeskStation);
};

/******************************************************************************************************/
