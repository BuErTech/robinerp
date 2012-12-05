#pragma once

///#include "..\..\��Ϣ�ļ�\mj.h"

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

#ifdef	VIDEO
		//�ļ����ֶ���
		#define GAMENAME						TEXT("�Ĺ�������Ƶ��")
		#define NAME_ID							30300405								///< ���� ID
#else
		#define GAMENAME						TEXT("�Ĺ�����")
		#define NAME_ID							30300400								///< ���� ID
#endif

/// �汾����
#define GAME_MAX_VER					1												///< ������߰汾
#define GAME_LESS_VER					1												///< ������Ͱ汾
#define GAME_CHANGE_VER					0												///< �޸İ汾

/// ֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
#define GAME_TABLE_NAME					"MatchTable_JQ"
/// ��Ϸ��Ϣ

#define PLAY_COUNT						4												///< ��Ϸ����

#define	VERSION							3
#define MAIN_VERSION					707												///< ���汾��,?��?��
#define ASS_VERSION						VERSION											///< �����汾��,?��

static TCHAR szTempStr[MAX_PATH] = {0};//�ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ��
#define GET_CLIENT_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))
#define GET_SERVER_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))       

#ifdef TWO_PEOPLE
#define CLIENT_DLL_NAME					TEXT("JQ2.ico")
#define SERVER_DLL_NAME					TEXT("JQ2Srv.DLL")
#define NAME_ID							51												///< ���� ID
#define PLAY_COUNT						2												///< ��Ϸ����
#define MAIN_VERSION					707												///< ���汾��,?��?��
#define ASS_VERSION						(VERSION*10+1)									///< �����汾��,?��
#endif

//#define SKIN_FOLDER					TEXT("jq")										///< ͼƬ�ļ���
//#define SKIN							TEXT("cardskin1")								///< ����ͼƬ����һ��
//#define SKIN_CARD						TEXT("card")									///< ����ͼƬ�ļ���

#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)
#define CLIENT_DLL_NAME                 TEXT(INT_TO_STR(NAME_ID)##".ico")				///<  �ͻ���ico����
#define SKIN_FOLDER                     TEXT("")//TEXT(INT_TO_STR(NAME_ID))             ///<  �ͻ���Ŀ¼
#define SKIN_FOLDER1                    TEXT(INT_TO_STR(NAME_ID))						///<  �ͻ���Ŀ¼

/// ��Ϸ״̬����
#define GS_WAIT_SETGAME					0												///< �ȴ���������״̬
#define GS_WAIT_ARGEE					1												///< �ȴ�ͬ������
#define GS_BUZHEN						20												///< ����״̬
//wushuqun  2009.6.16
#define GS_PLAY_GAME					21												///< ��Ϸ��״̬
#define GS_WAIT_NEXT					23												///< �ȴ���һ�̿�ʼ 

/********************************************************************************/

struct tagBuZhen
{
	int station;
	int Count;										///< ���ҵ��˿���Ŀ
	int CardArr[40];								///< �Լ����˿���Ϣ

	tagBuZhen()
	{
		station=-1;
		Count=0;
		memset(CardArr,0,40);
	}
};

/// ��Ϸ����ͳ�����ݰ�
struct tagNextStep
{
	int					station;					///< �˳�λ��
	int					station2;					///< ʧ�ܵĵڶ������
};

struct tagMove
{
	int					m_iStartI,m_iStartJ,m_iEndI,m_iEndJ;
	int					iStep;
	BYTE				cPath[17][17];
};

struct tagMoveResult
{
	int	m_iStartI,m_iStartJ,m_iEndI,m_iEndJ;	
	int station,nextstation;
	int ToJq,FromJq;
	int loseColor;
	int loseColor2;									///< �ڶ���ʧ�����
	int iStep;
	BYTE cPath[17][17];						///< ����·��
};

typedef struct tagPATHMSG
{
	bool bContinueInsertPos[17][17];
	int iCurStep;
	POINT FrontPath;
	POINT BackPath;
	POINT CurPath;
	tagMoveResult PathMsg;

	void Init()
	{
		iCurStep = 0;
		FrontPath.x = FrontPath.y = 255;
		BackPath.x = BackPath.y = 255;
		CurPath.x = CurPath.y = 255;		
		memset(&PathMsg, 0, sizeof(PathMsg));
		memset(bContinueInsertPos, 0, sizeof(bContinueInsertPos));
	}

	void tagMoveResult()
	{
		Init();
	}
}_PathMsg;

#define JJ_TEST
#ifdef JJ_TEST
//#define JJ_TEST_DEAD
#endif





















/*





struct tagMjPai
{
	int					 Count;						//���ҵ��˿���Ŀ
	int				 CardArr[40];					//�Լ����˿���Ϣ

	tagMjPai()
	{
		Count=0;
		memset(CardArr,0,40);
	}
};



struct tagUserMjPai
{
	int station;
	tagMjPai   MjPai	;	
	tagUserMjPai(){station=-1;}
};


struct tagUserZhuaPai
{
	int station;
	int iStartIndex;
	int iEndIndex;
	tagMjPai   MjPai	;	
	tagUserZhuaPai(){station=iStartIndex=iEndIndex=-1;}
};



//�û��������ݰ� �������������
struct ChuPaiStruct
{
	int ChuPaiType;
	int station;
	int					iCardCount;						//�˿���Ŀ
//	ARRUSERMJ				iCardList;					//�˿���Ϣ

	int m_iF1;

	ChuPaiStruct &operator =(ChuPaiStruct& from)
	{
		ChuPaiType=from.ChuPaiType;
		station=from.station;
		iCardCount=from.iCardCount;
		//for(int i=0;i<iCardCount;i++)
//			iCardList[i]=from.iCardList[i];
		return * this;
	}
	void Clear(){ChuPaiType = station =m_iF1=-1;iCardCount = 0;}
};


*/

//��Ϸ���ݰ�

/********************************************************************************/

//��Ϸ״̬���ݰ�	�� �ȴ���������״̬ ��
struct GameStation_1
{

	//��Ϸ��Ϣ
	BYTE				iCardCount;						//�˿���Ŀ
	int			        iMainVersion;
	int			        iLessVersion;
	int					uRunPublish;	//�۷�
	int					basepoint;		//����
};

//��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
struct GameStation_2
{
	//��Ϸ��Ϣ
	BYTE				iPlayCount;						//��Ϸ����
	BYTE				iCardCount;						//�˿���Ŀ
	int			iMainVersion;
	int			iLessVersion;
	int					uRunPublish;	//�۷�
	int					basepoint;		//����
	int mBoard[17][17];
};

//��Ϸ״̬���ݰ�	�� ��Ϸ���ڷ���״̬ ��
//��Ϸ״̬���ݰ�	�� �ȴ���Ѻ����״̬ ��
struct GameStation_3
{
	//��Ϸ��Ϣ
	
	BYTE				iPlayCount;						//��Ϸ����
	int                 iTimerCount ; 
	int					iWinPoint;						//��Ϸ����
	int					iPunishPoint;					//�û�����
	BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	int					uRunPublish;	//�۷�
	int					basepoint;		//����
	int					m_iNowOutPeople;	
	int mBoard[17][17];
	int m_iStartI,m_iStartJ,m_iEndI,m_iEndJ;
};

/*

//��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
struct GameStation_4	:public GameStation_3
{
	int					iOutCardPeople;					//���ڳ����û�
};
*/
//��Ϸ״̬���ݰ�	�� �ȴ����̿�ʼ״̬  ��
struct GameStation_5
{
	
	BYTE				iPlayCount;						//��Ϸ����
	BYTE				iCardCount;						//�˿���Ŀ

	//����״̬����
	int					iWinPoint;						//��Ϸ����
	int					iPunishPoint;					//�û�����
	int					iUpGradePeople;					//ׯ��λ��
	BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	int					uRunPublish;	//�۷�
	int					basepoint;		//����
};

//�û�ͬ����Ϸ
struct UserArgeeGame
{
	BYTE				iPlayCount;						//��Ϸ����
	BYTE				iCardCount;						//�˿���Ŀ
};

//��Ϸ��ʼ
struct BeginGameStruct
{
	BYTE				iPlayCount;						//��Ϸ�غ�
};

//�������ݰ�
struct SendCardStruct
{
	int					iCardCount[4];						//���ҵ��˿���Ŀ
	int iStartIndex;
	int iEndIndex;
};

//��Ϸ��ʼ���ݰ�
struct BeginPlayStruct
{
	BYTE				iOutDeskStation;				//���Ƶ�λ��
};

/*
struct PassOutCkeckStruct
{
	int bDeskStation;
};



//��ҵķ����ݰ�
struct GamePointStruct
{
	bool				bAdd;
	int					iPoint;							//�÷���Ŀ
	int station;
};
*/
//��Ϸ����ͳ�����ݰ�
struct GameEndStruct
{
	int					iTurePoint[4];					//ׯ�ҵ÷�
	__int64					iChangeMoney[4];				//���
	BYTE				iDoublePointEffect;				//˫�����ֿ�Ч����20081125	
};

//��Ϸ����ͳ�����ݰ�
typedef struct tagGameResult
{
	int					iWardPoint[PLAY_COUNT];					//ׯ�ҵ÷�
	int					iMoney[PLAY_COUNT];				//���
	TCHAR               name[PLAY_COUNT][60];
	tagGameResult()
	{
        memset(this, 0, sizeof(tagGameResult));
	}
}GameResult;

//��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	int					bDeskStation;					//�˳�λ��
	__int64					iChangeMoney[4];				//���
	int					iTurePoint[4];					//ׯ�ҵ÷�
};

struct HaveThingStruct
{
	BYTE pos;
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

struct StopResultStruct
{
	bool bArgee;
};

/*
struct CastSeZiStruct
{
	int step;////��ɫ�Ӵ���
	int sezi1Dian,
		sezi2Dian;

	CastSeZiStruct(){step =sezi1Dian =sezi2Dian=0;}
};

*/
/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/
#define ASS_UG_USER_SET					50				//�û�������Ϸ
#define ASS_BEGIN_UPGRADE				51				//������Ϸ��ʼ
#define ASS_BUZHEN					52				//������Ϣ

///#define ASS_SEND_FINISH					54				//�������
#define ASS_GAME_PLAY					59				//��ʼ��Ϸ
#define ASS_OUT_CARD					60				//�û�����
#define ASS_GAME_POINT					63				//�û�����
///#define ASS_NEW_TURN					64				//��һ�ֿ�ʼ
#define ASS_CONTINUE_END				65				//��Ϸ����
#define ASS_NO_CONTINUE_END				66				//��Ϸ����
#define ASS_CUT_END						67				//�û�ǿ���뿪
#define ASS_SALE_END					68				//��Ϸ��ȫ����
#define ASS_HAVE_THING					69
#define ASS_LEFT_RESULT					70
#define ASS_MESSAGE						71
#define ASS_STOP_THING					72
#define ASS_AGREE_STOP					73

#define ASS_NEXT_STEP					74				//��һ�ֿ�ʼ

#define ASS_LOSE                        75
#define ASS_PASS                        76


/********************************************************************************/
#endif
