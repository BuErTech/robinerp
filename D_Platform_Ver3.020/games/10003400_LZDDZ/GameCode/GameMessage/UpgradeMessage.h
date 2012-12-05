#pragma once

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

//�ļ����ֶ���
#define GAMENAME						TEXT("��Ӷ�����")

#ifdef VIDEO
#define NAME_ID							10003405								//���� ID
//#define CLIENT_DLL_NAME					TEXT("lzddz_video.ico")
//#define SERVER_DLL_NAME					TEXT("lzddzSrv_video.DLL")
//#define GAME_TABLE_NAME					"MatchTable_ddz"
#else
#define NAME_ID							10003400								//���� ID
//#define CLIENT_DLL_NAME					TEXT("lzddz.ico")
//#define SERVER_DLL_NAME					TEXT("lzddzSrv.DLL")
//#define GAME_TABLE_NAME					"MatchTable_ddz"
#endif


//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
//��Ϸ��Ϣ

#define PLAY_COUNT						3								//��Ϸ����


//��Ϸ�ڲ������汾��
#define DEV_HEIGHT_VERSION				8				//�ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
#define DEV_LOW_VERSION					4				//�ڲ������Ͱ汾��

//#define SKIN_FOLDER						TEXT("lzddz")	//ͼƬ�ļ���
//#define SKIN							TEXT("cardskin1")	//����ͼƬ����һ��
//#define SKIN_CARD						TEXT("card")		//����ͼƬ�ļ���
#define GET_STRING(NUM)                 #NUM
#define INT_TO_STR(NUM)                 GET_STRING(NUM)

#define SKIN_FOLDER                     TEXT(INT_TO_STR(NAME_ID))                // �ͻ���Ŀ¼
#define SKIN_CARD						TEXT("cardskin")		//����ͼƬ�ļ���
static TCHAR szTempStr[MAX_PATH] = {0};//�ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ��
#define GET_CLIENT_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))
#define GET_SERVER_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))
//#define GET_SKIN_FOLDER(A)				(sprintf(A,"%d",NAME_ID),strcat(A,""))//GET_SKIN_FOLDER(szTempStr)
#define GET_SKIN_FOLDER(A)				""

//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_SEND_CARD					20				//����״̬
#define GS_WAIT_BACK					21				//�ȴ���ѹ����
#define GS_PLAY_GAME					22				//��Ϸ��״̬
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 

#define GS_FLAG_NORMAL					0				//�������
#define GS_FLAG_CALL_SCORE				1				//�з�
#define GS_FLAG_ROB_NT					2				//������
#define GS_FLAG_ADD_DOUBLE				3				//�Ӱ�
#define GS_FLAG_SHOW_CARD				4				//����
/********************************************************************************/
//��Ϸ���ݰ�
//#define SUPER_VERSION
/********************************************************************************/
//�û��������ݰ� �������������
struct OutCardStruct
{
	int					iCardCount;						//�˿���Ŀ
	bool                bIsReplace;                     //�Ƿ�ʹ�ô�����
	BYTE				iCardList[45];					//�˿���Ϣ
	BYTE                iReplaceCardList[45];           //���������Ϣ
	//int                 iReplaceCount;                  //����������
};

//������ҳ���
struct ReplaceOutCardStruct
{
	BYTE				bDeskStation;					//������Ƶ�λ��
	bool                bIsReplace;                     //�Ƿ�ʹ�ô�����
	int					iCardCount;						//�˿���Ŀ
	BYTE				iCardList[45];					//�˿���Ϣ
	BYTE                iReplaceCardList[45];           //���������Ϣ
};

//�û��������ݰ� ������ͻ��ˣ�
struct OutCardMsg
{
	int					iNextDeskStation;				//��һ������
	bool                bIsReplace;                     //�Ƿ�ʹ�ô�����
	BYTE				iCardCount;						//�˿���Ŀ
	BYTE				bDeskStation;					//��ǰ������
	BYTE				iCardList[45];					//�˿���Ϣ
	BYTE                iReplaceCardList[45];           //���������Ϣ
};
//��һ��
struct NewTurnStruct
{
	BYTE				bDeskStation;					//����
	BYTE				bReserve;						//����
};
//��Ϸ״̬���ݰ�	�� �ȴ���������״̬ ��
struct GameStation_1
{
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	//��Ϸ��Ϣ
};

//��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
struct GameStation_2
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	//��Ϸ��Ϣ
	BYTE				iPlayCard;						//�˿���Ŀ
	UINT				iPlayCount;						//ʹ�õ�����(54,108,162,216,324)
	BYTE				iBackCount;						//������
	BYTE				iBeginNt;						//��ʼλ��
	BYTE				iEndNt;							//����λ��
	BOOL				bTwoIsNt;						//2�Ƿ�Ϊ����
	BOOL				bHaveKing;						//�Ƿ�����
	BOOL				bKingCanReplace;				//���Ƿ�ɵ�
	DWORD				iCardShape;						//��������
	//����
	BYTE				iBeginTime;						//��ʼ׼��ʱ��
	BYTE				iThinkTime;						//����˼��ʱ��
	BYTE				iGiveBackTime;					//����ʱ��
	BYTE				iCallScoreTime;					//�зּ�ʱ
	BYTE				iAddDoubleTime;					//�Ӱ�ʱ��
	BYTE				iPlayLimit;						//��Ϸ����
	//��Ϸ����
	UINT				iDeskBasePoint;					//���������
	UINT				iRoomBasePoint;					//���䱶��
	int					iRunPublish;					//���ܿ۷�
};


//��Ϸ״̬���ݰ�	�� �ȴ���Ѻ����״̬ ��
struct GameStation_3
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	//��Ϸ��Ϣ
	BYTE				iPlayCard;						//�˿���Ŀ
	UINT				iPlayCount;						//ʹ�õ�����(54,108,162,216,324)
	BYTE				iBackCount;						//������
	BYTE				iBeginNt;						//��ʼλ��
	BYTE				iEndNt;							//����λ��
	BOOL				bTwoIsNt;						//2�Ƿ�Ϊ����
	BOOL				bHaveKing;						//�Ƿ�����
	BOOL				bKingCanReplace;				//���Ƿ�ɵ�
	DWORD				iCardShape;						//��������
	//����
	BYTE				iBeginTime;						//��ʼ׼��ʱ��
	BYTE				iThinkTime;						//����˼��ʱ��
	BYTE				iGiveBackTime;					//����ʱ��
	BYTE				iCallScoreTime;					//�зּ�ʱ
	BYTE				iAddDoubleTime;					//�Ӱ�ʱ��
	//��Ϸ����
	UINT				iDeskBasePoint;					//���������
	UINT				iRoomBasePoint;					//���䱶��
	int					iRunPublish;					//���ܿ۷�
	BYTE				iPlayLimit;						//��Ϸ����


	//����״̬����
	//int					iWinPoint;						//��Ϸ����
	//int					iPunishPoint;					//�û�����
	int					iUpGradePeople;						//ׯ��λ��
	BYTE				iBeenPlayGame;						//�Ѿ���Ϸ�ľ���
	bool				bAuto[PLAY_COUNT];					//�й����

	BYTE				iCallScorePeople;					//��ǰ�з���
	BYTE				iCallScore[PLAY_COUNT];				//���ҽз����
	BYTE				iRobNT[PLAY_COUNT];					//������
	BYTE				iGameFlag;							//�зֱ��
	BYTE				iCallScoreResult;					//���еķ�

	//״̬��Ϣ
	BYTE				iUserCardCount[PLAY_COUNT];				   //�û������˿���Ŀ
	BYTE				iUserCardList[195];				   //�û����ϵ��˿�
};

//��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
struct GameStation_4	
{
	BYTE				iVersion;							//��Ϸ�汾��
	BYTE				iVersion2;							//��Ϸ�汾��
	//��Ϸ��Ϣ
	BYTE				iPlayCard;							//�˿���Ŀ
	UINT				iPlayCount;							//ʹ�õ�����(54,108,162,216,324)
	BYTE				iBeginNt;							//��ʼλ��
	BYTE				iEndNt;								//����λ��
	BOOL				bTwoIsNt;							//2�Ƿ�Ϊ����
	BOOL				bHaveKing;							//�Ƿ�����
	BOOL				bKingCanReplace;					//���Ƿ�ɵ�
	DWORD				iCardShape;							//��������
	//����
	BYTE				iBeginTime;							//��ʼ׼��ʱ��
	BYTE				iThinkTime;							//����˼��ʱ��
	BYTE				iGiveBackTime;						//����ʱ��
	BYTE				iCallScoreTime;						//�зּ�ʱ
	BYTE				iAddDoubleTime;						//�Ӱ�ʱ��
	//��Ϸ����
	UINT				iDeskBasePoint;						//���������
	UINT				iRoomBasePoint;						//���䱶��
	int					iRunPublish;						//���ܿ۷�
	BYTE				iPlayLimit;							//��Ϸ����
	int					iBase;								//��ǰը������
	//����״̬����
	//int					iWinPoint;						//��Ϸ����
	//int					iPunishPoint;					//�û�����
	int					iUpGradePeople;						//ׯ��λ��
	BYTE				iBeenPlayGame;						//�Ѿ���Ϸ�ľ���
//	BYTE				iBackCard[12];
	int					iBackCardCount;						//��������
	//������Ϣ
	int 				iCallScoreResult;					//�зֽ��
	bool				bAuto[PLAY_COUNT];					//�й����
	BYTE				iAwardPoint[PLAY_COUNT];			//����
	BYTE				iPeopleBase[PLAY_COUNT];			//�Ӱ�
	BYTE				iRobNT[PLAY_COUNT];					//������
	int                 iArrIsDisplayCard[PLAY_COUNT];      //�Ƿ�����

	//״̬��Ϣ
	int					iOutCardPeople;						//���ڳ����û�
	int					iFirstOutPeople;					//�ȳ��Ƶ��û�
	int					iBigOutPeople;					//�ȳ��Ƶ��û�
	BYTE				iBaseOutCount;						//���Ƶ���Ŀ
	BYTE				iUserCardCount[PLAY_COUNT];			//�û������˿���Ŀ
	BYTE				iDeskCardCount[PLAY_COUNT];			//�����˿˵���Ŀ
	BYTE				iUserCardList[195];					//�û����ϵ��˿�

};

//��Ϸ״̬���ݰ�	�� �ȴ����̿�ʼ״̬  ��
struct GameStation_5
{
	BYTE				iVersion;							//��Ϸ�汾��
	BYTE				iVersion2;							//��Ϸ�汾��
	//��Ϸ��Ϣ
	BYTE				iPlayCard;							//�˿���Ŀ
	UINT				iPlayCount;							//ʹ�õ�����(54,108,162,216,324)
	BYTE				iBackCount;							//������
	BYTE				iBeginNt;							//��ʼλ��
	BYTE				iEndNt;								//����λ��
	bool				bTwoIsNt;							//2�Ƿ�Ϊ����
	bool				bHaveKing;							//�Ƿ�����
	DWORD				iCardShape;							//��������
	//����
	BYTE				iBeginTime;							//��ʼ׼��ʱ��
	BYTE				iThinkTime;							//����˼��ʱ��
	BYTE				iGiveBackTime;						//����ʱ��
	BYTE				iCallScoreTime;						//�зּ�ʱ
	BYTE				iAddDoubleTime;						//�Ӱ�ʱ��
	BYTE				iPlayLimit;							//��Ϸ����
	//��Ϸ����
	UINT				iDeskBasePoint;						//���������
	UINT				iRoomBasePoint;						//���䱶��
	int					iRunPublish;						//���ܿ۷�
};

//�û�ͬ����Ϸ
struct UserArgeeGame
{
	BYTE				iPlayCount;							//��Ϸ����
	BYTE				iCardCount;							//�˿���Ŀ
};

//��Ϸ��ʼ
struct BeginUpgradeStruct
{
	BYTE				iUpgradeStation;					//ׯ��λ��
	BYTE				iPlayLimit;							//��Ϸ�ܾ���
	BYTE				iBeenPlayGame;						//�Ѿ����˶��پ�

};
//��Ϸ׼�����ݰ�
struct PrepareStruct
{
	BYTE				bCard;									//����
};
//�������ݰ�
struct SendCardStruct
{
	BYTE				bDeskStation;							//���
	BYTE				bCard;									//�Ʊ��
};

//�l�������Ɣ���
struct SendAllStruct
{
	BYTE iUserCardCount[PLAY_COUNT];										//��������
	BYTE iUserCardList[108];									//���ƶ���
};
//���ƽ���
struct SendCardFinishStruct
{
	BYTE		bReserve;									
};


//�������ݰ�
struct BackCardStruct
{
	BYTE				bDeskStation;					//ׯ��λ��
	BYTE				iBackCardCount;					//�˿���Ŀ
	BYTE				iBackCard[12];					//��������
};

//�������ݰ�
struct BackCardExStruct
{
	BYTE				iGiveBackPeople;				//�������
	BYTE				iBackCardCount;					//�˿���Ŀ
	BYTE				iBackCard[12];					//��������
};


//��Ϸ��ʼ���ݰ�
struct BeginPlayStruct
{
	BYTE				iOutDeskStation;				//���Ƶ�λ��
};


//�û�˦��ʧ�����ݰ� ������ͻ��ˣ�
struct OutShowCardStruct
{
	BYTE				iCardCount;						//�˿���Ŀ
	BYTE				bDeskStation;					//��ǰ������
	BYTE				iResultCount;					//����˿���Ŀ
	BYTE				iCardList[78];					//�˿���Ϣ
};

//���濪ʼ
struct ReplaceAgreeStruct
{
	BYTE bDeskStation;									
};

//�з����ݰ�
struct CallScoreStruct
{	
	BYTE				bDeskStation;							//��ǰ�з���
	int					iValue;									//�з�����
	bool 				bCallScoreflag;								//�зֱ��								
};
//������
struct RobNTStruct
{
	BYTE bDeskStation;										//����������
	int iValue;												//���������
};

//����
struct ShowCardStruct
{
	BYTE bDeskStation;										//����
	int iValue;											    //����
};

//��������
struct ReplaceShowCardStruct
{
	BYTE bDeskStation;										//����
	BYTE bRepDeskStation;									//����λ��
	int iValue;												//����
};
//�Ӱ�
struct AddDoubleStruct
{
	BYTE bDeskStation;										//�Ӱ�λ��
	int iValue;												//�Ӱ����
};

//����з����ݰ�
struct ReplaceCallScoreStruct
{	
	BYTE		bDeskStation;								//����з���
	int		    NowCallScorePeople;							//��ǰ�з���
	int			CallScore;									//�з�����
	bool 		CallScoreFlag;								//�зֱ��								
};

//����
struct AwardPointStruct
{
	BYTE	iAwardPoint;									//����
	BYTE	bDeskStation;									//����
	int		iBase;											//����
};

//�й����ݽṹ
struct AutoStruct
{
	BYTE bDeskStation;
	bool bAuto;
};

//�������й�
struct UseAIStation
{
	BYTE bDeskStation;					//����
	BOOL bState;						//״̬
};

//�ھ����
struct ChampionStruct
{
	BYTE bDeskStation ;
};

//�û��������ݰ� ������ͻ��ˣ�
struct BatchOutCardMsg
{
	BYTE				iCardCount;						//�˿���Ŀ
	BYTE				iCardList[156];					//�˿���Ϣ
};

////����
//struct PropStruct
//{
//	BYTE				sendDeskStation;				//����λ��
//	BYTE				recvDeskStation;				//����λ��
//	int					iValue;
//};
//��Ϸ����ͳ�����ݰ�
struct GameEndStruct
{
	UINT                iTax;                           //˰��
	int					iBasePoint;						//��Ϸ����
	__int64					iTurePoint[PLAY_COUNT];					//��ҵ÷�
	__int64					iInitScore[PLAY_COUNT];					//δ����ϵͳ�ӷֺ�ĵ÷�
	__int64					iChangeMoney[8];				//��ҽ��
	BYTE				iUpGradeStation;				//ׯ��λ��
	BYTE				iCallScore;						//ׯ�ҽз�
	BYTE				iRobMul;						//���������Ʊ���
	BYTE				iOutCardMul;					//���Ƽӱ�
	BYTE				iBombMul;						//ը���ӱ�
	int 				iTotalMul;						//�ܱ���
	BYTE                iBombCard[4][10];                         //0,����,1.���,2.Ӳը,3.��ը
};

//��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	int					iRoomBasePoint;						//����
	int					iDeskBasePoint;					//���汶��
	int					iHumanBasePoint;				//��ͷ����

	int					bDeskStation;					//�˳�λ��
	__int64					iChangeMoney[PLAY_COUNT];				//��ҽ��
	__int64				iTurePoint[PLAY_COUNT];					//ׯ�ҵ÷�

};
//��Ϸ����ͳ�����ݰ�
struct ReplaceStruct
{
	BYTE SrcCardList[45]; //ԭ��
	BYTE ReplaceList[100][45]; //�������ƶ���
	BYTE ReplaceListShape[100]; //�������ƶ�������
	BYTE ReplaceCard[10];   //������
	int  CardCount;       //������
	int  ReplaceCount;    //�����Ƶ�����
	int  ListCount;        //���γɵĴ�����


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

//��Ϸ����
struct GameFinishNotify
{
	TCHAR name[PLAY_COUNT][21];
	BYTE iStyle;			//��Ϸ�����Ƿ�Ϊǿ��,������������
	int	iBasePoint;			//ϵͳ����
	int iCallScore;         //�з�
	int	iDeskBasePoint;		//���������
	int iUpGradePeople;		//�f��
	int iGrade2;			//�f������
	UINT iTax;               //˰��
	__int64	iWardPoint[PLAY_COUNT];
	__int64 iAwardPoint[PLAY_COUNT];		//����
	__int64 iTotalScore[PLAY_COUNT];			//�ܻ���
	__int64 iMoney[PLAY_COUNT];				//��������Ǯ��
	BYTE                    iBombCard[4][10];                         //0,����,1.���,2.Ӳը,3.��ը

	BYTE iGameStyle;					//��Ϸ����
};

//���嶯������
#define ANIMAL_DZWIN	1	//����Ӯ
#define ANIMAL_DZLOSE	2	//������ 
#define ANIMAL_SBOMB0	3	//0�����Сը��
#define ANIMAL_SBOMB1	4	//1�����Сը��
#define ANIMAL_SBOMB2	5	//2�����Сը��
#define ANIMAL_BBOMB0	6	//0����Ҵ�ը��
#define ANIMAL_BBOMB1	7	//1����Ҵ�ը��
#define ANIMAL_BBOMB2	8	//2����Ҵ�ը��

//���ش������
#define ER_OK		0		//����
#define ER_ERROR	1		//����	
#define ER_END		2		//CAnimal��ʹ��,��ʾ�����ѽ���
#define ER_UPDATE	3		//CCombineAnimal��ʹ��,��ʾ��Ҫˢ�½���
/********************************************************************************/

// ���ݰ���������ʶ
//warning:79��IDԤ���������ͻ���,����ʹ��
/********************************************************************************/
#define ASS_UG_USER_SET					50				//������Ϸ
#define ASS_GAME_BEGIN					51				//��Ϸ��ʼ
#define ASS_GAME_PREPARE				52				//׼������

#define ASS_SEND_CARD					53				//������Ϣ
#define ASS_SEND_CARD_MSG				54				//���ƹ����д�����Ϣ
#define ASS_SEND_ALL_CARD				55				//����������(һ�·���ȫ��)
#define ASS_SEND_FINISH					56				//�������

#define ASS_CALL_SCORE					57				//�з�
#define ASS_CALL_SCORE_RESULT			58				//�зֽ��
#define ASS_CALL_SCORE_FINISH			59				//�зֽ���
#define ASS_REPLACE_CALL_SCORE			60				//����з�

#define ASS_ROB_NT						61				//������
#define ASS_ROB_NT_RESULT				62				//���������
#define ASS_REPLACE_ROB_NT				63				//����������
#define ASS_GAME_MULTIPLE				64				//��Ϸ����(���������ӱ�)
#define ASS_ROB_NT_FINISH				65				//���������


#define ASS_BACK_CARD					66				//��������
#define ASS_BACK_CARD_EX				67				//��չ��������(������ҿɼ�)

#define ASS_ADD_DOUBLE					68				//�Ӱ�
#define ASS_ADD_DOUBLE_RESULT			69				//�Ӱ����
#define ASS_REPLACE_ADD_DOUBLE			70				//����Ӱ�
#define ASS_ADD_DOUBLE_FINISH			71				//�Ӱ�����

#define ASS_SHOW_CARD					72				//����
#define ASS_SHOW_CARD_RESULT			73				//���ƽ��
#define ASS_REPLACE_SHOW_CARD			74				//��������
#define ASS_SHOW_CARD_FINISH			75				//���ƽ���

#define ASS_GAME_PLAY					76				//��ʼ��Ϸ
#define ASS_OUT_CARD					77				//�û�����
#define ASS_OUT_CARD_RESULT				78				//���ƽY��
#define ASS_REPLACE_OUT_CARD			80				//�������(79���������ͻ��˷�����)

#define ASS_ONE_TURN_OVER				81				//һ�����(ʹ�ͻ�����һ�ֿ���)
#define ASS_NEW_TURN					82				//��һ�ֿ�ʼ
#define ASS_AWARD_POINT					83				//����(ը�����)


#define ASS_CONTINUE_END				84				//��Ϸ����
#define ASS_NO_CONTINUE_END				85				//��Ϸ����
#define ASS_NO_CALL_SCORE_END			86				//���˽з�
#define ASS_CUT_END						87				//�û�ǿ���뿪
#define ASS_SAFE_END					88				//��Ϸ��ȫ����
#define ASS_TERMINATE_END				89				//�������
#define ASS_AHEAD_END					90				//��ǰ����

#define ASS_AUTO						91				//�û��й�
#define ASS_HAVE_THING					92				//����
#define ASS_LEFT_RESULT					93				//�����뿪���
#define ASS_MESSAGE						94				//������Ϣ
#define ASS_AI_STATION					95				//�������й�(���߻���)
#define ASS_REPLACE_GM_AGREE_GAME		96				//������ҿ�ʼ

//#define ASS_PROP						200				//����

/********************************************************************************/
#endif
