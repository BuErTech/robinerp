#pragma once

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE



#ifdef	VIDEO
		/// �ļ����ֶ���
		#define GAMENAME						TEXT("������Ƶ��")
		#define CLIENT_DLL_NAME					TEXT("XQ_Video.ico")
		#define SERVER_DLL_NAME					TEXT("XQSrv_Video.DLL")

		#define NAME_ID							30100205									///< ���� ID
#else
		/// �ļ����ֶ���
		#define GAMENAME						TEXT("����")
		#define CLIENT_DLL_NAME					TEXT("XQ.ico")
		//#define SERVER_DLL_NAME					TEXT("XQSrv.DLL")

		#define NAME_ID							30100200									///< ���� ID
#endif

/// �ļ����ֶ���
//#define SKIN_FOLDER							TEXT("xq")									///< ͼƬ�ļ���
//#define	SKIN								TEXT("cardskin1")
#define	SKIN_CARD								TEXT("cardskin")

//JMod-�ļ�������׼��-20090209
#define SOUND_MJ_FOLDER			TEXT("mj")//��ȡ������·��ʹ��
static TCHAR szTempStr[MAX_PATH] = {0};//�ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ��
#define GET_CLIENT_DLL_NAME(A)(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))					///< ���ԭ�ȵ� CLIENT_DLL_NAME 
#define GET_SERVER_DLL_NAME(A)(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))					///< ���ԭ�ȵ� SERVER_DLL_NAME 
#define GET_SKIN_FOLDER(A)(sprintf(A,"%d",NAME_ID),strcat(A,""))							///< ���ԭ�ȵ� SKIN_FOLDER 
#define MAX_NAME_INFO					MAX_PATH


/// �汾����
#define GAME_MAX_VER					1								///< ������߰汾
#define GAME_LESS_VER					1								///< ������Ͱ汾
#define GAME_CHANGE_VER					0								///< �޸İ汾

/// ֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
#define GAME_TABLE_NAME					"MatchTable_XQ20001"

/// ��Ϸ��Ϣ
#define PLAY_COUNT						2								///< ��Ϸ����
#define MAIN_VERSION					7								///< ���汾��,?��?��
#define ASS_VERSION						0								///< �����汾��,?��

/// ��Ϸ�����汾
#define DEV_HEIGHT_VERSION				9								///< �ڲ������߰汾��
#define DEV_LOW_VERSION					2								///< �ڲ������Ͱ汾��

/// ��Ϸ״̬����
#define GS_WAIT_SETGAME					0								///< �ȴ���������״̬
#define GS_WAIT_ARGEE					1								///< �ȴ�ͬ������
#define GS_SEND_CARD					20								///< ����״̬
#define GS_WAIT_BACK					21								///< �ȴ���ѹ����
#define GS_PLAY_GAME					22								///< ��Ϸ��״̬
#define GS_WAIT_NEXT					23								///< �ȴ���һ�̿�ʼ 

/********************************************************************************/

/// ��Ϸ���ݰ�

/********************************************************************************/

/**
 * ��Ϸ״̬���ݰ�	�� �ȴ���������״̬ ��
 */
struct GameStation_1
{
	/// ��Ϸ��Ϣ
	bool        bRequireCoachMoeny;                                     ///�Ƿ���Ҫָ����
	BYTE		iCardCount;												///< �˿���Ŀ
	int			iMainVersion;
	int			iLessVersion;
	/// ��Ϸ�汾
	BYTE				iVersion;										///< ��Ϸ�汾��
	BYTE				iVersion2;										///< ��Ϸ�汾��
	int					iRoomBasePoint;									///< �����������
	int					iRunPublish;									///< ���ܿ۷�
};

/**
 * ��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
 */
struct GameStation_2
{
	bool        bRequireCoachMoeny;                                     ///�Ƿ���Ҫָ����
	int			iJuTimeMinute;											///< ��ʱ��
	int			iJuTimeSecond;											///< ��ʱ��
	int			iBuTimeMinute;											///< ��ʱ��
	int			iBuTimeSecond;											///< ��ʱ��
	int			iDuMiaoMinute;											///< �����
	int			iDuMiaoSecond;											///< ������
	int			iGameMoney;												///��Ϸָ����
	int			iMainVersion;
	int			iLessVersion;
	/// ��Ϸ�汾
	BYTE				iVersion;										///< ��Ϸ�汾��
	BYTE				iVersion2;										///< ��Ϸ�汾��
	int					iRoomBasePoint;									///< �����������
	int					iRunPublish;									///< ���ܿ۷�

	BYTE				bDaoShi;
};

/**
 * ��Ϸ״̬���ݰ�	�� ��Ϸ���ڷ���״̬ ��
 * ��Ϸ״̬���ݰ�	�� �ȴ���Ѻ����״̬ ��
 */
struct GameStation_3
{
	/// ��Ϸ�汾
	bool                bRequireCoachMoeny;                                     ///�Ƿ���Ҫָ����
	BYTE				iVersion;						///< ��Ϸ�汾��
	BYTE				iVersion2;						///< ��Ϸ�汾��
	/// ��Ϸ��Ϣ
	BYTE				iBeginStation;					///< ��ʼλ��
	BYTE				iEndStation;					///< ����λ��
	BYTE				iMeStation;
	BYTE				iOtherStation;
	BYTE				iPlayCount;						///< ��Ϸ����
	BYTE				iCardCount;						///< �˿���Ŀ
	int					iRoomBasePoint;					///< �����������
	int					iRunPublish;					///< ���ܿ۷�
	/// ����״̬����
	int					iWinPoint;						///< ��Ϸ����
	int					iPunishPoint;					///< �û�����
	int					iUpGradePeople;					///< ׯ��λ��
	BYTE				iPlayNTNum[2];					///< ����ҵ�������Ŀ
	BYTE				iBeenPlayGame;					///< �Ѿ���Ϸ�ľ���
	/// ������Ϣ
	int					iNtPeople;						///< �������
	BYTE				iNtHua;							///< ���ƻ�ɫ
	BYTE				iNtCardCount;					///< ��������
	/// ̬��Ϣ
	BYTE				iUserCardCount;					///< �û������˿���Ŀ
	BYTE				iUserCard[45];					///< �û����ϵ��˿�

	BYTE				bDaoShi;

};

/**
 * ��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
 */
struct GameStation_4	
{
	/// ��Ϸ�汾
	bool                bRequireCoachMoeny;                                     ///�Ƿ���Ҫָ����
	BYTE				iVersion;						///< ��Ϸ�汾��
	BYTE				iVersion2;						///< ��Ϸ�汾��
	int                 iBgePeaceCount[PLAY_COUNT];
	int					iRoomBasePoint;					///< �����������
	int					iRunPublish;					///< ���ܿ۷�
	
	/// ��Ϸ��Ϣ
	int					iJuTimeMinute;					///< ��ʱ��
	int					iJuTimeSecond;					///< ��ʱ��
	int					iBuTimeMinute;					///< ��ʱ��
	int					iBuTimeSecond;					///< ��ʱ��
	int					iDuMiaoMinute;					///< �����
	int					iDuMiaoSecond;					///< ������
	char                iRegretStation;                 //< ����״̬
	int					iJuUseTime[PLAY_COUNT];			///< ��ʱ
	int					iBuUseTime[PLAY_COUNT];	
	int					iDuUseTime[PLAY_COUNT];	

	BYTE				iPlayCount;						///< ��Ϸ����
	BYTE				iBeenPlayGame;					///< �Ѿ���Ϸ�ľ���
	int					iOutCardPeople;					///< ���ڳ����û�
	int					iFirstOutPeople;				///< �ȳ��Ƶ��û�
	int					iLastOutPeople;					///< ��һ�ֳ��Ƶ��û�
	int					iStartI;
	int					iStartJ;
	int					iEndI;
	int					iEndJ ;
	int					iQiZi[10][9];

	BYTE				bDaoShi;
};

/**
 * ��Ϸ״̬���ݰ�	�� �ȴ����̿�ʼ״̬  ��
 */
struct GameStation_5
{
	/// ��Ϸ�汾
	bool                bRequireCoachMoeny;                                     ///�Ƿ���Ҫָ����
	BYTE				iVersion;						///< ��Ϸ�汾��
	BYTE				iVersion2;						///< ��Ϸ�汾��
	int					iRoomBasePoint;					///< �����������
	int					iRunPublish;					///< ���ܿ۷�
	/// ��Ϸ��Ϣ
	int					iJuTimeMinute;					///< ��ʱ��
	int					iJuTimeSecond;					///< ��ʱ��
	int					iBuTimeMinute;					///< ��ʱ��
	int					iBuTimeSecond;					///< ��ʱ��
	int					iDuMiaoMinute;					///< �����
	int					iDuMiaoSecond;					///< ������
	BYTE				iBeginStation;					///< ��ʼλ��
	BYTE				iEndStation;					///< ����λ��
	BYTE				iPlayCount;						///< ��Ϸ����
	BYTE				iCardCount;						///< �˿���Ŀ
	/// ����״̬����
	int					iWinPoint;						///< ��Ϸ����
	int					iPunishPoint;					///< �û�����
	int					iUpGradePeople;					///< ׯ��λ��
	BYTE				iPlayNTNum[2];					///< ����ҵ�������Ŀ
	BYTE				iBeenPlayGame;					///< �Ѿ���Ϸ�ľ���
};

/**
 * �û�ͬ����Ϸ
 */
struct UserArgeeGame
{
	
	int					iJuTimeMinute;					///< ��ʱ��
	int					iJuTimeSecond;					///< ��ʱ��
	int					iBuTimeMinute;					///< ��ʱ��
	int					iBuTimeSecond;					///< ��ʱ��
	int					iDuMiaoMinute;					///< �����
	int					iDuMiaoSecond;					///< ������
	int					iGameMoney;						///��Ϸָ����

	//BYTE				iBeginStation;					//��ʼλ��
	//BYTE				iEndStation;					//����λ��
	//BYTE				iPlayCount;						//��Ϸ����
	//BYTE				iCardCount;						//�˿���Ŀ
};

/**
 * ��Ϸ��ʼ
 */
struct BeginUpgradeStruct
{
	BYTE				iUpgradeStation;				///< ׯ��λ��
	BYTE				iPlayCount;						///< ��Ϸ�غ�
	BYTE				iNtStation;						///< ���ڵ�����
	BYTE				iStation[2];					///< ���Ƶ�λ��
	int                 BgePeaceCount[2];               ///< ��ʹ���
	int                 idaoshi;                         ///<��ʦ
};

/**
 * ��Ϸ׼��,����λ��,����λ��
 */
struct GameReadyStruct
{
	BYTE				iBlackStation;					///< ����λ��
	BYTE				iFirstStation;					///< ����λ��
};

/**
 * �������ݰ�
 */
struct SendCardStruct
{
	BYTE				iCard;							///< �Ʊ��
};

/**
 * ���������ݰ�  �������������
 */
struct ClientNtStruct
{
	BYTE				iHuaKind;						///< �Ʊ��
	BYTE				iCardCount;						///< ������Ŀ
};

/**
 * ���������ݰ�  ������ͻ��ˣ�
 */
struct ServerNtStruct
{
	BYTE				bDeskStation;					///< ����λ��
	BYTE				iHuaKind;						///< ��ɫ
};

/**
 * �������ݰ�
 */
struct BackCardStruct
{
	BYTE				iBackCardCount;					///< �˿���Ŀ
	BYTE				iBackCard[8];					///< ��������
};

/**
 * �������ݰ�
 */
struct BackCardExStruct
{
	BYTE				iNtHua;							///< ���ƻ�ɫ
	BYTE				iGiveBackPeople;				///< �������
	BYTE				iBackCardCount;					///< �˿���Ŀ
	BYTE				iBackCard[8];					///< ��������
};

/**
 * ������״̬���ݰ�
 */
struct BackStationStruct
{
	BYTE				iNtPeople;						///< ����λ��	
	BYTE				iGiveBackStation;				///< ����λ��
};

/**
 * ��Ϸ��ʼ���ݰ�
 */
struct BeginPlayStruct
{
	BYTE				iOutDeskStation;				///< ���Ƶ�λ��
	int					m_iBoard[10][9];				///< ���
};

/**
 * �z���Ƿ�Y��
 */
struct PlayCheckFinish
{
	BOOL bCanFinish;
};

/**
 * �û��������ݰ� �������������
 */
struct OutCardStruct
{
	int					iCardCount;						///< �˿���Ŀ
	BYTE				iCardList[39];					///< �˿���Ϣ
};

struct SuperTimer
{
	int                 bDeskStation;					///< ��ʱ��
};

/**
 * ��������
 */
struct LoseStruct
{
	BYTE bDeskStation;
	int iFlag;
};

/**
 * �û�˦��ʧ�����ݰ� ������ͻ��ˣ�
 */
struct OutShowCardStruct
{
	BYTE				iCardCount;						///< �˿���Ŀ
	BYTE				bDeskStation;					///< ��ǰ������
	BYTE				iResultCount;					///< ����˿���Ŀ
	BYTE				iCardList[78];					///< �˿���Ϣ
};

/**
 * �û��������ݰ� ������ͻ��ˣ�
 */
struct OutCardMsg
{
	int					iNextDeskStation;				///< ��һ������
	BYTE				iCardCount;						///< �˿���Ŀ
	BYTE				bDeskStation;					///< ��ǰ������
	BYTE				iCardList[39];					///< �˿���Ϣ
};

/**
 * �û��������ݰ� ������ͻ��ˣ�
 */
struct BatchOutCardMsg
{
	BYTE				iCardCount;						///< �˿���Ŀ
	BYTE				iCardList[156];					///< �˿���Ϣ
};

/**
 * ��ҵķ����ݰ�
 */
struct GamePointStruct
{
	bool				bAdd;
	int					iPoint;							///< �÷���Ŀ
};

/**
 * ��Ϸ����ͳ�����ݰ�
 */
struct GameEndStruct
{
	int					iGamePoint;						///< ��Ϸ�÷�
	__int64					iTurePoint[4];					///< ׯ�ҵ÷�
	__int64					iChangeMoney[8];				///< ���
	BYTE				iUpGradeStation;				///< ׯ��λ��
	BYTE				iBackList[8];					///< �����˿�

	bool                bShuaLai;						///< �Ƿ��������ˣ��
	BYTE                bDeskStation;					///< ˣ�����
};

/**
 * ��Ϸ����ͳ�����ݰ�
 */
struct GameCutStruct
{
	int					bDeskStation;					///< �˳�λ��
	__int64					iTurePoint[4];					///< ׯ�ҵ÷�
	__int64					iChangeMoney[8];				///< ���
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

struct HaveThingStruct
{
	BYTE pos;
	char szMessage[101];
};

struct LeaveResultStruct
{
	BYTE bDeskStation;
	int bArgeeLeave;
};

/**
 * �ͻ��˵�������
 */
struct MessageStruct
{
	char Message[200];
};

struct StopResultStruct
{
	bool bArgee;
};

struct PlayChessStruct
{
	POINT	ptStart;									///< ������ʼ��
	POINT   ptEnd;										///< �����յ�
	int		iCurPlay;									///< ��ǰ���ӵ�
	int		iValue;										///< ���ӵ�ֵ
	int		iNextPlay;									///< �¸�������
	int		iBoard[10][9];								///< ���

	/// �¼ӳ�Ա��Ϊ��ֹ���ˣ�����ظ�ͬ���ķ�������
	int     iPutChess;      							///< ���岽��
	int     xChess,yChess;  							///< ���ӵ�����ֵ
	int     idChess;        							///< ��ʲô����

	
};

/**
 * ��һ��������
 */
struct NextPlayInfo
{
	int iNextPlay;										///< ��һ��������
	int iCurPlay;										///< �t����,߀�Ǻ�����
};

/**
 * �������ݰ�(����ͻ���)
 */
struct RegretChessMsg
{
	int					iLastChessArray[10][9];			///< ��¼�ϲ�����
	int					iLastPlay;						///< ��¼�ϲ�������
	int					iLastDeskStation;				///< �����ߵ�λ��
};

struct GamerShuaLaiJiang
{
	int     iPutChess;     							     ///< ���岽��
	int     xChess,yChess; 							     ///< ���ӵ�����ֵ
	int     idChess;       							     ///< ��ʲô����
};

/**
 * ��Ϸ����
 */
struct GameFinishNotify
{
	bool bExist[4];									   ///< ����Ƿ����
	int	 iUpGradePeople;
	int iGrade2;
	TCHAR name[4][61];
	//	int	iBasePoint;
	BYTE iStyle;										///< ��Ϸ�����Ƿ�Ϊǿ��,������������
	int	iWardPoint[4];
	//__int64 iTotalPoint[4];
	//int iAwardPoint[4];								//����
	//	int iGetPoint[4];								//����ץ��
	//	int nt;											//���s
	__int64 iMoney[4];
};

/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/

#define ASS_UG_USER_SET					50				///< �û�������Ϸ
#define ASS_BEGIN_UPGRADE				51				///< ������Ϸ��ʼ
#define ASS_SEND_CARD					52				///< ������Ϣ
#define ASS_NT_CARD						53				///< �û�����
#define ASS_SEND_FINISH					54				///< �������
#define ASS_BACK_CARD					55				///< ��������
#define ASS_BACK_CARD_EX				56				///< ��������
#define ASS_BACK_CARD_RES				57				///< �������ݽ��
#define ASS_WAIT_BACK					58				///< �û�������
#define ASS_GAME_PLAY					59				///< ��ʼ��Ϸ
#define ASS_OUT_CARD					60				///< �û�����
#define ASS_OUT_CARD_RESULT				61				///< ���ӽ��
#define ASS_CAN_FINISH					62				///< �ͻ��˲���Ƿ���Խ���
#define ASS_GAME_READY					63				///< ��Ϸ׼������
//#define ASS_SHOW_CARD					61				///< ˦��ʧ��
//#define ASS_BATCH_OUT					62				///< ��������
//#define ASS_GAME_POINT					63			///< �û�����
#define ASS_SYNC_TIME					63				///< ͬ����ʱ
#define ASS_NEW_TURN					64				///< ��һ�ֿ�ʼ
#define ASS_CONTINUE_END				65				///< ��Ϸ����
#define ASS_NO_CONTINUE_END				66				///< ��Ϸ����
#define ASS_CUT_END						67				///< �û�ǿ���뿪
#define ASS_SALE_END					68				///< ��Ϸ��ȫ����
#define ASS_HAVE_THING					69
#define ASS_LEFT_RESULT					70
#define ASS_MESSAGE						71
#define ASS_STOP_THING					72
#define ASS_AGREE_STOP					73
#define ASS_SUPER_TIMER					74				///< ��ʱ����
#define ASS_REN_SHU						75				///< ����
#define ASS_HUI_QI						76				///< ����
#define ASS_AGREE_HUI_QI				77				///< ͬ�����

/// ���ˣ���������ظ�5�����Զ��������
#define ASS_GAMER_SHUALAI_JIANG         80
/********************************************************************************/
#endif
