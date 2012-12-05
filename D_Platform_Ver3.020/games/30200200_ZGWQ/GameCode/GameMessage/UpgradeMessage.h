#pragma once

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

//#define	VIDEO

#ifdef	VIDEO
		//�ļ����ֶ���
		#define GAMENAME						TEXT("Χ����Ƶ��")
		#define NAME_ID							30200205							//���� ID
#else
		//�ļ����ֶ���
		#define GAMENAME						TEXT("Χ��")
		#define NAME_ID							30200200							//���� ID
#endif

//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
#define GAME_TABLE_NAME					"WQ"
//��Ϸ��Ϣ

#define PLAY_COUNT						2								//��Ϸ����

#define MAIN_VERSION					704						 //���汾��,?��?��
#define ASS_VERSION						21				//�����汾��,?��

//��Ϸ�ڲ������汾��
#define DEV_HEIGHT_VERSION				1				//�ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
#define DEV_LOW_VERSION					1				//�ڲ������Ͱ汾��


//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_PLAY_GAME					22				//��Ϸ��״̬
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 

#define ASS_GAME_READY					63				//��Ϸ׼������

//#define SKIN_FOLDER						TEXT("wq")	//ͼƬ�ļ���
//#define SKIN							TEXT("cardskin1")	//����ͼƬ����һ��
//#define SKIN_CARD						TEXT("wq")		//����ͼƬ�ļ���

#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)
#define CLIENT_DLL_NAME                 TEXT(INT_TO_STR(NAME_ID)##".ico")        // �ͻ���ico����
#define SKIN_FOLDER                     TEXT("") //TEXT(INT_TO_STR(NAME_ID))     // �ͻ���Ŀ¼
#define GET_NAME_ID                     TEXT(INT_TO_STR(NAME_ID))                // ��ϷID
#define MUSIC_FOLDER					TEXT("wq")	//ͼƬ�ļ���


/********************************************************************************/

//��Ϸ���ݰ�

/********************************************************************************/
//��Ϸ��ʹ���˵�ʱ����Ϣ jph100127
struct UsedTimeInfo
{
	int m_iJuUseTime[2];
	int	m_iBuUseTime[2];
	int	m_iDuUseTime[2];
	UsedTimeInfo()
	{
		memset(m_iJuUseTime,0,sizeof(m_iJuUseTime));
		memset(m_iBuUseTime,0,sizeof(m_iBuUseTime));
		memset(m_iDuUseTime,0,sizeof(m_iDuUseTime));
	}
};

//��Ϸ״̬���ݰ�	�� �ȴ���������״̬ ��
struct GameStation_1
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	//��Ϸ����
	UINT				iDeskBasePoint;					//���������
	UINT				iRoomBasePoint;					//���䱶��
	int					iRunPublish;					//���ܿ۷�
	int					iRunPublishMoney;				//���ܽ��
};

//��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
struct GameStation_2
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	//��Ϸ����
	UINT				iDeskBasePoint;					//���������
	UINT				iRoomBasePoint;					//���䱶��
	int					iRunPublish;					//���ܿ۷�
	int					iRunPublishMoney;				//���ܽ��

	int					iJuTimeMinute;					//��ʱ��
	int					iJuTimeSecond;					//��ʱ��
	int					iBuTimeMinute;					//��ʱ��
	int					iBuTimeSecond;					//��ʱ��
	int					iDuMiaoMinute;					//�����
	int					iDuMiaoSecond;					//������
};


//��Ϸ״̬���ݰ�(��Ϸ��)	


//��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
struct GameStation_4	
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	//��Ϸ����
	UINT				iDeskBasePoint;					//���������
	UINT				iRoomBasePoint;					//���䱶��
	int					iRunPublish;					//���ܿ۷�
	int					iRunPublishMoney;				//���ܽ��
	//��Ϸ��Ϣ
	int					iJuTimeMinute;					//��ʱ��
	int					iJuTimeSecond;					//��ʱ��
	int					iBuTimeMinute;					//��ʱ��
	int					iBuTimeSecond;					//��ʱ��
	int					iDuMiaoMinute;					//�����
	int					iDuMiaoSecond;					//������

	BYTE				iPlayCount;						//��Ϸ����
	BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	int					iOutCardPeople;					//���ڳ����û�
	int					iFirstOutPeople;				//�ȳ��Ƶ��û�
	int					m_iLastDeskStation;				//�����Ƶ��û�
	int					iQiZi[21][21];
	int					iTiZiNum[2];
	int					iQiZiNum[2];						//����������
	int					iEndI;
	int					iEndJ;
	int                isHitAgreedianmu;                  //�Ƿ�����ͬ���Ŀ
	UsedTimeInfo		m_stUsedTimeInfo;					//ʹ�ù���ʱ��

	bool				bIsInHuiquStation[2];			//�Ƿ������״̬
	bool				bChessingInHuiQi[2];
};

//��Ϸ״̬���ݰ�	�� �ȴ����̿�ʼ״̬  ��
struct GameStation_5
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	//��Ϸ����
	UINT				iDeskBasePoint;					//���������
	UINT				iRoomBasePoint;					//���䱶��
	int					iRunPublish;					//���ܿ۷�
	int					iRunPublishMoney;				//���ܽ��
	//��Ϸ��Ϣ
	BYTE				iBeginStation;					//��ʼλ��
	BYTE				iEndStation;					//����λ��
	BYTE				iPlayCount;						//��Ϸ����
	BYTE				iCardCount;						//�˿���Ŀ

	//����״̬����
	int					iWinPoint;						//��Ϸ����
	int					iPunishPoint;					//�û�����
	int					iUpGradePeople;					//ׯ��λ��
	BYTE				iPlayNTNum[2];					//����ҵ�������Ŀ
	BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
};

//�û�ͬ����Ϸ
struct UserArgeeGame
{
	
	int			iJuTimeMinute;//��ʱ��
	int			iJuTimeSecond;//��ʱ��
	int			iBuTimeMinute;//��ʱ��
	int			iBuTimeSecond;//��ʱ��
	int			iDuMiaoMinute;//�����
	int			iDuMiaoSecond;//������
};

struct PrepareStruct
{
	BYTE			bDeskStation;					//ѡ��λ��
};

//��Ϸ��ʼ
struct BeginUpgradeStruct
{
	BYTE				bDeskStation;				//ׯ��λ��
	BYTE				iPlayCount;						//��Ϸ�غ�
};


//��Ϸ��ʼ���ݰ�
struct BeginPlayStruct
{
	BYTE				bDeskStation;				//���Ƶ�λ��
};

//��Ϸ׼��,����λ��,����λ��
struct GameReadyStruct
{
	BYTE				iBlackStation;					//����λ��
	BYTE				iWhiteStation;					//����λ��
};

//��Ϸ����ͳ�����ݰ�
struct GameEndStruct
{
	//BYTE				bDeskStation;						//ׯ��λ��
	BYTE				bDeskStation;						//���λ��
	BYTE				iDoublePointEffect;					//˫�����ֿ���Ч�� 20081125
	__int64				iTurePoint[PLAY_COUNT];					//ׯ�ҵ÷�
	__int64				iChangeMoney[PLAY_COUNT];				//���
	char				szNickName[PLAY_COUNT][61];
};

////��Ϸ����
//struct GameFinishNotify
//{
//	TCHAR name[PLAY_COUNT][21];
//	BYTE iStyle;								//��Ϸ�����Ƿ�Ϊǿ��,������������
//	int	iScore1;								//������
//	int	iScore2;								//��������
//	int	iTurePoint[PLAY_COUNT];					//�������
//	int iChangeMoney[PLAY_COUNT];				//��������Ǯ��
//};

//��Ϸ����
struct GameFinishNotify
{
	int	 iUpGradePeople;
	int iGrade2;
	TCHAR name[4][21];
	BYTE iStyle;			//��Ϸ�����Ƿ�Ϊǿ��,������������
	int	iWardPoint[4];
	__int64	iTurePoint[PLAY_COUNT];				//�������
	int	iScore1;								//������
	int	iScore2;								//��������
	int iChangeMoney[PLAY_COUNT];				//��������Ǯ��
	__int64 iMoney[4];
};

struct HaveThingStruct
{
	BYTE bDeskStation;
	char szMessage[61];
};

struct LeaveResultStruct
{
	BYTE bDeskStation;
	int bArgeeLeave;
};

struct MessageStruct	//�ͻ��˵�������
{
	char Message[200];
};
//
struct StopResultStruct
{
	bool bArgee;
};
//���ݰ�(����)
struct LoserStruct 
{
	BYTE	bDeskStation;
	BYTE	bReserver;
};
//���ݰ�(��ʱ,����)
struct SuperTimerStruct 
{
	BYTE	bDeskStation;
	BYTE	bReserver;
};
///�������ݰ�������ͻ��ˣ�
struct PlayChessMsg
{	
	BYTE				bDeskStation;		//��ǰ������
	int					iEndI;
	int					iEndJ;
	int					iPlayFlag;
	int					iTiZiNum[2];//������
	int					iChessArray[21][21];
//	int					iNextPlayStation;
	int					iQiZiNum[2];
};
//������
struct PlayChessStruct
{
	BYTE				bDeskStation;
};
//
struct DianMu
{
	int qi[21][21];
};
//
struct FinishDianMu
{
	int qi[21][21];
	char msg[50];
};
//�������ݰ�(����ͻ���)
struct RegretChessMsg
{
	int					iLastChessArray[21][21];//��¼�ϲ����
	int					iLastTiZiNum[2];//��¼�ϲ�������
	int					iLastDeskStation;//�����ߵ�λ��
};

struct DianMuResultStruct	//��Ŀ
{
	bool bAgree;
};

/**
* @brief ͬ��ʱ�����ݰ�
*/
struct SyncTimeStruct
{
	int					iBotJuUseTime;
	int					iBotBuUseTime;
	int					iBotDuUseTime;
};


/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/

#define ASS_UG_USER_SET					50				//�û�������Ϸ
#define ASS_BEGIN_UPGRADE				51				//������Ϸ��ʼ
#define ASS_GAME_PREPARE				52				//����
#define ASS_GAME_PLAY					51				//��ʼ��Ϸ

#define ASS_OUT_CARD_RESULT				60				//�û����ƽ��
#define ASS_OUT_CARD					61				//��һ������
#define ASS_SYNC_TIME					63				///< ͬ����ʱ

#define ASS_SUPER_TIMER					71				//��ʱ����
#define ASS_REN_SHU						72				//����
#define ASS_HUI_QI						73				//����
#define ASS_AGREE_HUI_QI				74				//ͬ�����

#define ASS_DIAN_MU						75			//��Ŀ
#define ASS_AGREE_DIAN_MU				76			//ͬ���Ŀ
#define ASS_NUMBER_ZI					77			//ǿ������
#define ASS_DIAN_MU_RESULT				78			//��Ŀ�������

#define ASS_CONTINUE_END				80				//��Ϸ����
#define ASS_SUPER_TIMER_END				81				//��ʱ����
#define ASS_LOSER_END					82				//�������
#define ASS_NO_CONTINUE_END				83				//��Ϸ����
#define ASS_CUT_END						84				//�û�ǿ���뿪
#define ASS_SALE_END					85				//��Ϸ��ȫ����

#define ASS_HAVE_THING					90
#define ASS_LEFT_RESULT					91
#define ASS_MESSAGE						92
#define ASS_STOP_THING					93
#define ASS_AGREE_STOP					94

#define ASS_GET_GAME_USED_TIME			95				//��ȡʹ��ʱ��	

/********************************************************************************/
#endif
