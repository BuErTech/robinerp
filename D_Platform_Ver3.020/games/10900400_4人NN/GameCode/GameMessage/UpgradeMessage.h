#pragma once

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

//�ļ����ֶ���
#ifdef VIDEO
#define GAMENAME						TEXT("ţţ")
#define NAME_ID							10900405							//���� ID
#define PLAY_COUNT						4								//��Ϸ����
#else
#ifdef TWO_NN
#define GAMENAME						TEXT("ţţ(VIP)")
#define NAME_ID							10900200							//���� ID
#define PLAY_COUNT						2								//��Ϸ����
#else
#define GAMENAME						TEXT("ţţ")
#define NAME_ID							10900400							//���� ID
#define PLAY_COUNT						4								//��Ϸ����
#endif
#endif
//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
#define GAME_TABLE_NAME					"MatchTable"
//��Ϸ��Ϣ



//#define SKIN_FOLDER						TEXT("nn")	//ͼƬ�ļ���
//#define SKIN							TEXT("cardskin1")	//����ͼƬ����һ��
//#define SKIN_CARD						TEXT("card")		//����ͼƬ�ļ���

#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)
#define CLIENT_DLL_NAME                 TEXT(INT_TO_STR(NAME_ID)##".ico")        // �ͻ���ico����
#define SKIN_FOLDER                     TEXT(INT_TO_STR(NAME_ID))                // �ͻ���Ŀ¼
#define SKIN_CARD						TEXT("CardSkin")		                 // ����ͼƬ�ļ���


//��Ϸ�����汾
#define DEV_HEIGHT_VERSION				3								//�ڲ������߰汾��
#define DEV_LOW_VERSION					10 								//�ڲ������Ͱ汾��

//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_SEND_CARD					20				//����״̬
#define GS_WAIT_BACK					21				//�ȴ���ѹ����
#define GS_PLAY_GAME					22				//��Ϸ��״̬
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 
#define GS_FLAG_CALL_SCORE              24              //�з�
#define GS_NOTE                         25              //�µ�ע

//��ע����
#define TYPE_GIVE_UP					0x00			//����
#define TYPE_RESET                      0x01            //Ҫ��
#define TYPE_OPEN                       0x02            //����
#define TYPE_BULL						0x03			//ţ
#define TYPE_PLAY_GAME                  0x04            //�����
#define TYPE_NOTE						0x06			//��ע

#define MAX_SHAPE_COUNT                 15              //������������
/********************************************************************************/

//��Ϸ���ݰ�
//#define SUPER_VERSION
/********************************************************************************/

//�����û����
struct tagUserResult
{
	//	BOOL bShowOnly;				//����ʾ
	//BOOL bAddFlag;				//�ӱ����
	BYTE bCountNotePeople;		//��ǰδ���������
	BYTE bAddTime;				//Ѻע����
	BYTE iNowBigNoteStyle;		//��ע����
	BYTE iNowBigPeople;
	BYTE iOutPeople;			//��ע��
	BYTE iSplitStation;         //����λ��
	int iNowBigNote;			//��ǰ��ע
	int iTotalNote;				//����ע
    int iUserStation[PLAY_COUNT] ;//�����ע����
	int iFirstOutPeople;         //��һλ�����
	int iUserNote[PLAY_COUNT];   //�û�������ע
	bool bSplit;                 //�Ƿ����

	
	//	int iCurNote;		//��ǰ��ע��
};
//��������
struct ControlWinStruct
{
  BYTE     bWinStation;
  BYTE     bLoseStation;
 //  bool  ming[PLAY_COUNT];
};
//��Ϸ״̬���ݰ�	�� �ȴ���������״̬ ��

struct GameStation_1
{

	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	
};

//��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
struct GameStation_2
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	//��Ϸ��Ϣ
	//	BYTE				iBeginStation;					//��ʼλ��
	//	BYTE				iEndStation;					//����λ��
	BYTE				iThinkTime;						//��Ϸ����
	BYTE				iBeginTime;						//��Ϸ����
	BYTE				iCallScoreTime;						//�зּ�ʱ
	BYTE				iXiaZhuTime;						//��עʱ��
	BYTE				iAllCardCount;						//�˿���Ŀ
	int					iRoomBasePoint;					//���䱶��
	int					iRunPublish;					//���ܿ۷�
	__int64					iLimitNote[PLAY_COUNT];						//���ע
	int					iBaseNote;						//��ע
	DWORD				iCardShape;						//��������
	BYTE                bGameType;                      //��Ϸ���ࡣ1Ϊͬϲ�ֵģ�ͬϲ����עΪ100����
	BYTE                iGameNoteSetType;                //��Ϸ������������
	BOOL                bIsSuper;                        //�Ƿ����ʹ��������
};

//��Ϸ״̬���ݰ�	�� ��Ϸ���ڷ���״̬ ��
struct GameStation_3
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��

	//��Ϸ��Ϣ
	BYTE				iThinkTime;						//��Ϸ����
	BYTE				iBeginTime;						//��Ϸ����
	BYTE				iAllCardCount;						//�˿���Ŀ
	BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	BYTE				iCallScoreTime;						//�зּ�ʱ
	BYTE				iXiaZhuTime;						//��עʱ��
	BYTE				iSysCardCount;					//��ʣ��������
	//״̬��Ϣ
	BYTE				iUserCardCount[PLAY_COUNT];				//�û����ϵ�����
	BYTE				iUserCard[PLAY_COUNT][10];				//�û����ϵ���
	BYTE				iUserCardCountSplit[PLAY_COUNT];				//�û����ϵ�����
	BYTE				iUserCardSplit[PLAY_COUNT][10];				//�û����ϵ���
	BYTE                iGameFlag;
	tagUserResult		tag;
	
	//����״̬����

	int					iUpGradePeople;					//ׯ��λ��
	int					iRoomBasePoint;					//���䱶��
	int					iRunPublish;					//���ܿ۷�
	__int64					iLimitNote[PLAY_COUNT];						//���ע
	int					iBaseNote;						//��ע

	__int64					iTotalNote[PLAY_COUNT];					//�û���ǰ��ע
	DWORD				iCardShape;						//��������
	BYTE                bGameType;                      //��Ϸ���ࡣ1Ϊͬϲ�ֵģ�ͬϲ����עΪ100����
	BYTE                iGameNoteSetType;                //��Ϸ������������
	BOOL                bIsSuper;                        //�Ƿ����ʹ��������
	BOOL                bPlayer;                         //�Ƿ����
	
};

//��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
struct GameStation_4	
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��

	//��Ϸ��Ϣ
	//	BYTE				iBeginStation;					//��ʼλ��

	BYTE				iThinkTime;						//��Ϸ����
	BYTE				iBeginTime;						//��Ϸ����
	BYTE				iAllCardCount;						//�˿���Ŀ
	BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	BYTE				SysCard[28];					//��ʣ�µ���
	BYTE				iSysCardCount;					//��ʣ��������
	BYTE				iCallScoreTime;						//�зּ�ʱ
	BYTE				iXiaZhuTime;						//��עʱ��
	__int64					iLimitNote[PLAY_COUNT];						//���ע
	int					iBaseNote;						//��ע
	int					iRoomBasePoint;					//���䱶��
	int					iRunPublish;					//���ܿ۷�
	//����״̬����
	int					iUpGradePeople;					//��עλ��
	int					iOutCardPeople;					//���ڳ����û�
	int					iFirstOutPeople;				//�ȳ��Ƶ��û�
	__int64					iTotalNote[PLAY_COUNT];					//�û���ǰ��ע
	int                 iThisGameNote[PLAY_COUNT];			//������Ϸ��ע��(��ע)
	int					iTotalGameNote[PLAY_COUNT];
	int					iUserStation[PLAY_COUNT];				//��¼�û�״̬
	bool                bSplit;                      //�Ƿ��з���
	BYTE                iSplitStation;               //�����Ƶ�λ��(����)
   //״̬��Ϣ

	//	int					iIngNote;						//��ǰ��ע�û�
	//	int					iIngPlayStatus;					//������Ϸ�Ķ���
	//	int					iUserNote[4];					//�û�������ע����
	//	int					iTotalNote;						//������ע
	tagUserResult		tag;
	BYTE				iUserCardCount[PLAY_COUNT];				//�û����ϵ�����
	BYTE				iUserCard[PLAY_COUNT][10];				//�û����ϵ���
	BYTE				bBullCard[3];							//�û�ţ��
	BYTE				iUserCardCountSplit[PLAY_COUNT];				//�û����ϵ�����
	BYTE				iUserCardSplit[PLAY_COUNT][10];				//�û����ϵ���
	DWORD				iCardShape;						//��������
	BYTE                bGameType;                      //��Ϸ���ࡣ1Ϊͬϲ�ֵģ�ͬϲ����עΪ100����
	BYTE                iGameNoteSetType;                //��Ϸ������������
	BOOL                bIsSuper;                        //�Ƿ����ʹ��������
	BOOL                bPlayer;                         //�Ƿ����
	//	tagButtonStatus		ButtonPara;						//��ť״̬
};

//��Ϸ״̬���ݰ�	�� �ȴ����̿�ʼ״̬  ��
struct GameStation_5
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	int					iRoomBasePoint;					//���䱶��
	int					iRunPublish;					//���ܿ۷�

	//��Ϸ��Ϣ
	//	BYTE				iBeginStation;					//��ʼλ��
	//	BYTE				iEndStation;					//����λ��
	BYTE				iThinkTime;						//��Ϸ����
	BYTE				iBeginTime;						//��Ϸ����
	BYTE				iAllCardCount;						//�˿���Ŀ
	BYTE				iCallScoreTime;						//�зּ�ʱ
	BYTE				iXiaZhuTime;						//��עʱ��
	//����״̬����
	int					iUpGradePeople;					//ׯ��λ��

	BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
	DWORD				iCardShape;						//��������
	BYTE                bGameType;                      //��Ϸ���ࡣ1Ϊͬϲ�ֵģ�ͬϲ����עΪ100����
	BYTE                iGameNoteSetType;                //��Ϸ������������
	BOOL                bIsSuper;                        //�Ƿ����ʹ��������
};

//�û�ͬ����Ϸ
struct UserArgeeGame
{
	BYTE				iBeginStation;					//��ʼλ��
	BYTE				iEndStation;					//����λ��
	BYTE				iPlayCount;						//��Ϸ����
	BYTE				iCardCount;						//�˿���Ŀ
};

//��Ϸ��ʼ
struct BeginUpgradeStruct
{
	BYTE				AllCardCount;					//һ��ʹ�õ�����
	int					iBaseNote;						//��ע	
	__int64				iLimitNote[PLAY_COUNT];						//���ע��
	BYTE				SysCard[52];					//��ǰ��
	int                 iNt;                            //ׯ��

	//BYTE				iNtStation;						//���ڵ�����
	//BYTE				iStation[2];					//���Ƶ�λ��
};
//�ռ����붯��
struct CollectJetonStruct
{
	int JetonTotal;
	BYTE bDeskStation;
};
//�������ݰ�
struct SendCardStruct
{
	BYTE				iCard;									//�Ʊ��
	BYTE				bDeskStatin;							//��һ�ֳ�����
	bool                bIsSplit;                               //�Ƿ��ڷ���λ���Ϸ���
	//	BYTE				bAddTime;								//Ѻע����
	//	BOOL				bEndFlag;								//������־
	//	SendCardStruct()
	//	{
	//		iOutPeople = 0;
	//		memset(iCard,0,sizeof(iCard));
	//		bAddTime=0;
	//		bEndFlag=FALSE;
	//	}
};
/// �������ݰ���һ�ν��˿�ȫ�������ͻ���
struct SendAllCardStruct
{
	BYTE      iStartPos;                  //������ʼλ��
	BYTE      iUserCard[PLAY_COUNT][5];   //�û��˿�
	BYTE      iUserCardCount[PLAY_COUNT];

	SendAllCardStruct()
	{
		memset(iUserCard,0,sizeof(iUserCard));
		memset(iUserCardCount,0,sizeof(iUserCardCount));
		iStartPos = 255;
	}
};
/*
struct SendCardStruct
{
BYTE				iCard[4][2];							//�Ʊ��
BYTE				iOutPeople;								//��һ�ֳ�����
BYTE				bAddTime;								//Ѻע����
BOOL				bEndFlag;								//������־
SendCardStruct()
{
iOutPeople = 0;
memset(iCard,0,sizeof(iCard));
bAddTime=0;
bEndFlag=FALSE;
}
};
*/

//��Ϸ��ʼ���ݰ�
struct BeginPlayStruct
{
	BYTE				iOutDeskStation;				//���Ƶ�λ��
};

//�û��������ݰ� �������������
struct OutCardStruct
{
	int					iCardCount;						//�˿���Ŀ
	BYTE				iCardList[39];					//�˿���Ϣ
};



//�û��������ݰ� ������ͻ��ˣ�
struct OutCardMsg
{
	int					iNextDeskStation;				//��һ������
	BYTE				iCardCount;						//�˿���Ŀ
	BYTE				bDeskStation;					//��ǰ������
	BYTE				iCardList[39];					//�˿���Ϣ
};



//��Ϸ����ͳ�����ݰ�
struct GameEndStruct
{
	int					iUserState[PLAY_COUNT];					//�ļ�״̬(��ǰ����,������)

	BYTE				iCardShape[PLAY_COUNT];
	__int64				iTurePoint[PLAY_COUNT];					//ׯ�ҵ÷�
	__int64				iChangeMoney[8];
	BYTE				iUpGradeStation;				//ׯ��λ��
	BYTE                iUpBullCard[PLAY_COUNT][3];      //�������
	BYTE				iCardList[PLAY_COUNT][10];					//�˿���Ϣ
//	BYTE				iBackList[8];					//�����˿�
};

//��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	int					bDeskStation;					//�˳�λ��
	__int64					iTurePoint[PLAY_COUNT];					//ׯ�ҵ÷�
	__int64				iChangeMoney[8];
	BYTE                iQuitType;                     //�˳�����
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
//�з����ݰ�
struct CallScoreStruct
{	
	BYTE				bDeskStation;							//��ǰ�з���
	int					iValue;									//�з�����
	bool 				bCallScoreflag;								//�зֱ��		
	bool               bPlayer;
};
/*
//��ť����
enum BUTTON_TYPE
{
BUTTON_TYPE_NONE = -1,
BUTTON_TYPE_GIVEUP,//����
BUTTON_TYPE_100NOTE,//100ע
BUTTON_TYPE_200NOTE,//200ע
BUTTON_TYPE_400NOTE,//400ע
BUTTON_TYPE_BULL,	//��ע
BUTTON_TYPE_FOLLOW,//����
BUTTON_TYPE_SHUTTLE,//����
};*/

//�û�������Ϣ
struct tagUserProcess
{
	BYTE iVrebType;//������İ�ť
	__int64  iNote;		//��ע��
	BYTE bUpCard[3];
};

//��������û�������Ϣ
struct SupersedeUserNote
{
	BYTE bDeskStation;//�����λ��
	BYTE iVrebType;//������İ�ť
	int  iNote;		//��ע��
};
//�Q���Ɣ����Y��
struct ChangeBackStruct
{
	BYTE bDeskStation;//λ��
	BYTE bSourceCard;//Դ��
	BYTE bResultCard;//�Y����
	BOOL bExtVal;	//�Ƿ���Q�ɹ�
};

//�����û�Ѻע
struct NoteResult
{
	BYTE bAddStyle;		//��ע����
	BYTE iOutPeople;	//��ע��
	__int64 iCurNote;		//��ǰ�����ע��
	BYTE NoHitStation;          //ͣ��λ��
};
struct GameFinishNotify
{
	TCHAR name[PLAY_COUNT][21];
	int	iBasePoint;
	BYTE iStyle;			//��Ϸ�����Ƿ�Ϊǿ��,������������
	__int64	iWardPoint[PLAY_COUNT];
	//__int64 iTotalPoint[4];
	BYTE iCardShape[PLAY_COUNT];
	__int64 iMoney[PLAY_COUNT];
};
/*
//�����������
struct tagUserGiveUp
{
BOOL bShowOnly;		//����ʾ
BOOL bAddFlag;	//�ӱ����	
BYTE bAddTime;	//Ѻע����
BYTE bAddStyle;	//��ע����	
BYTE iGiveUpPeo;//������
BYTE iOutPeople;//��ע��
//	tagButtonStatus ButtonPara;//��ť����
};
*/
/********************************************************************************/

// ���ݰ���������ʶ

/********************************************************************************/

#define ASS_UG_USER_SET					50				//�û�������Ϸ
#define ASS_BEGIN_UPGRADE				51				//������Ϸ��ʼ
#define ASS_SEND_CARD					52				//������Ϣ
#define ASS_SEND_FINISH					54				//�������

#define ASS_CALL_SCORE					55				//�з�
#define ASS_CALL_SCORE_RESULT			56				//�зֽ��
#define ASS_CALL_SCORE_FINISH			57				//�зֽ���
#define ASS_REPLACE_CALL_SCORE			58				//����з�

#define ASS_GAME_PLAY					59				//��ʼ��Ϸ
#define ASS_SEND_ALLCARD				63				//�������е�����
#define ASS_NEW_TURN					64				//��һ�ֿ�ʼ
#define ASS_CONTINUE_END				65				//��Ϸ����
#define ASS_NO_CONTINUE_END				66				//��Ϸ����
#define ASS_CUT_END						67				//�û�ǿ���뿪
#define ASS_SALE_END					68				//��Ϸ��ȫ����
#define ASS_HAVE_THING					69
#define ASS_LEFT_RESULT					70
#define ASS_MESSAGE						71
#define ASS_STOP_THING					72
#define ASS_AGREE_STOP					73
#define ASS_NOMONEY                     75              //û��ׯ��.�˳�



#define ASS_SUPERSEDE_VREB_CHECK		78				//����(����)�û�����
#define ASS_VREB_CHECK					89				//�û�����

#define ASS_SHUTTLE						80				//����
#define ASS_FOLLOW						81				//���˻򲻼�
#define ASS_ADD_NOTE					82				//��ע
#define ASS_SHOW_100					83				//Ѻע100
#define ASS_SHOW_200					84				//Ѻע200
#define ASS_SHOW_400					85				//Ѻע400
#define ASS_GIVE_UP						86				//����
#define ASS_MODIFY_PLAY_NOTE			90				//�����û�����ע
#define ASS_NOTE						91				//��ע
#define ASS_NOTE_RESULT					92				//��ע���
#define ASS_SEND_A_CARD					93				//�·�һ����
#define ASS_COLLECT_JETON				94				//�ռ�����
#define ASS_CONTROL_WIN                 95              //������Ӯ
#define ASS_GM_SUPERCLIENT				100				//�����͑���
#define ASS_TAN_PAI						101				//̯����Ϣ�����ͻ���

/********************************************************************************/
#endif
