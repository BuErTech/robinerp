#pragma once

//ͬ¥ ��ͨ��/��Ƶ�� ����  release/��Ƶ��  ��������������������

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

//�ļ����ֶ���
#define GAMENAME						TEXT("���ֶ�����")

#ifdef VIDEO
#define NAME_ID							10001305								//���� ID
#else
#define NAME_ID							10001300								//���� ID
#endif


//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME


//��Ϸ�ڲ������汾��
#define DEV_HEIGHT_VERSION				2				//�ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
#define DEV_LOW_VERSION					0				//�ڲ������Ͱ汾��



//��Ϸ��Ϣ
#define PLAY_COUNT						3								//��Ϸ����
#define SKIN_CARD						TEXT(".\\image\\cardskin")		//����ͼƬ�ļ���
static TCHAR szTempStr[MAX_PATH] = {0};//�ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ��
#define GET_CLIENT_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))
#define GET_SERVER_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))

#define GET_CLIENT_BCF_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".bcf"))

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
#define GS_FLAG_PLAY_GAME               5 
 
const  int  MAX_TASK_TYPE   = 4 ;  ///����������� 
const  int  MAX_CARD_SHAPE  = 8 ;  ///�����������
const  int  MAX_CARD_TYPE   = 15 ; ///������

///��������
enum BackCardType
{
	TYPE_NONE        = 0 ,//ʲô���Ͷ�����
	TYPE_DOUBLE_CARD = 10 , 
	TYPE_SAME_HUA    = 11,
	TYPE_SMALL_KING  = 12 ,
	TYPE_BIG_KING    = 13 , //����
	TYPE_TRIPLE_CARD = 14 ,
	TYPE_STRAIT      = 15,
	TYPE_ROCKET      = 16
};
///��������
enum LastTaskType
{
	TYPE_LAST_NONE         = 0 ,    ///���κ����� 
	TYPE_HAVE_A_CARD       = 100 ,  ///��ĳ����
	TYPE_SOME_SHAPE        = 101 ,  ///��ĳ������
	TYPE_SINGLE_SOME_CARD  = 102 ,  ///���ĳ����
	TYPE_DOUBLE_SOME_CARD  = 103   ///���һ��ĳ����
};

///��Ϸ����ṹ
struct GameTaskStruct
{
	BYTE     byBackCardType ; 
	BYTE     byTaskType ; 
	BYTE     bySpecifyShape ; 
	BYTE     bySpecifyCard ;
	BYTE     byBackCardMutiple ;  ///���Ʊ���

	GameTaskStruct()
	{
		memset(this , 0 , sizeof(GameTaskStruct)) ; 
	}
	void IniData(void)
	{
		memset(this , 0 ,sizeof(GameTaskStruct)) ;
	}

};
///��Ϸ�еı���
struct  GameMutipleStruct
{
	int       sBaseMutiple ;                 ///��Ϸ�еĻ�����ע
	int       sBackCardMutiple ;             ///���Ʊ���
	int       sBombCount ;                   ///ը������
	int       sSprintMutiple ;               ///����
	int       sCardShapeMutiple  ;           ///���ͱ�����һ��ָ�����еı�����

	BYTE      sAddGameMutiple[PLAY_COUNT] ;  /// ��Ϸ�мӱ�
	BYTE      sRobNtMutiple[PLAY_COUNT] ;    ///����������
	BYTE      sMingPaiMutiple[PLAY_COUNT] ;  ///ÿ���˵����Ʊ���

	GameMutipleStruct()
	{
		sBaseMutiple = 1 ; 
		sBackCardMutiple = 1 ; 
		sBombCount = 0 ; 
		sSprintMutiple = 1;
		sCardShapeMutiple = 100 ; 

		memset(sAddGameMutiple , 0 , sizeof(sAddGameMutiple)) ; 
		memset(sRobNtMutiple , 0 , sizeof(sRobNtMutiple)) ; 
		memset(sMingPaiMutiple , 0 , sizeof(sMingPaiMutiple)) ; 
	};
	void  IniData(int  iBaseMutiple)
	{
		sBaseMutiple = iBaseMutiple ; 
		sBombCount = 0 ; 
		sBackCardMutiple = 1 ; 
		sSprintMutiple = 1;
		sCardShapeMutiple = 100; 

		memset(sAddGameMutiple , 0 , sizeof(sAddGameMutiple)) ; 
		memset(sRobNtMutiple , 0 , sizeof(sRobNtMutiple)) ; 
		memset(sMingPaiMutiple , 0 , sizeof(sMingPaiMutiple)) ; 
	}
	///��ȡ���������
	int  GetMingMaxMutiple(void)
	{
		int iMingMutiple = max(max(sMingPaiMutiple[0] , sMingPaiMutiple[1]) ,sMingPaiMutiple[2]) ; 

		return (iMingMutiple >0?iMingMutiple :1) ; 
	}
	///��ȡ������������
	int  GetRobNtMutiple(void)
	{
		int  iRobMutiple = 0 ; 
		int  iRobCount   = 0 ; 

		for(int  i = 0  ; i <PLAY_COUNT ; i++)
		{
			iRobCount += sRobNtMutiple[i] ; 
		}

		iRobMutiple = pow(2.0, iRobCount) ; 

		return iRobMutiple ; 
	}
    ///��ȡ��������
    int  GetPublicMutiple(void)
	{
		int  iBombMutiple = pow(2.0 , sBombCount) ; 

		int  iGameMutiple  = sBaseMutiple*sBackCardMutiple*iBombMutiple*sSprintMutiple*sCardShapeMutiple*GetRobNtMutiple()*GetMingMaxMutiple()/100 ;
			
		return iGameMutiple ; 
		
	}
};


/********************************************************************************/
//��Ϸ���ݰ�

/********************************************************************************/
//�û��������ݰ� �������������
struct OutCardStruct
{
	int					iCardCount;						//�˿���Ŀ
	BYTE				iCardList[45];					//�˿���Ϣ
};

//������ҳ���
struct ReplaceOutCardStruct
{
	BYTE				bDeskStation;					//������Ƶ�λ��
	int					iCardCount;						//�˿���Ŀ
	BYTE				iCardList[45];					//�˿���Ϣ
};

//�û��������ݰ� ������ͻ��ˣ�
struct OutCardMsg
{
	int					iNextDeskStation;				//��һ������
	BYTE				iCardCount;						//�˿���Ŀ
	BYTE				bDeskStation;					//��ǰ������	
	BYTE				iCardList[45];					//�˿���Ϣ
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
	bool                bUserReady[PLAY_COUNT] ;        ///����Ƿ���׼��

	BYTE				iBeginTime;						//��ʼ׼��ʱ��
	BYTE				iThinkTime;						//����˼��ʱ��
	BYTE				iCallScoreTime;					//�зּ�ʱ
	BYTE                iRobNTTime ;                    //������ʱ��
	BYTE				iAddDoubleTime;					//�Ӱ�ʱ��

	int                 iGameMutiple   ;             ///��Ϸ�еı���
	int                 iAddDoubleLimit;             //�ӱ�����
	int                 iGameMaxLimit ;              ///��Ϸ�����Ӯ

	DWORD				iCardShape;						//��������
	//��Ϸ����
	UINT				iDeskBasePoint;					//���������
	UINT				iRoomBasePoint;					//���䱶��
	__int64				iRunPublish;					//���ܿ۷�
	
};


//��Ϸ״̬���ݰ�	�� �ȴ���Ѻ����״̬ ��
struct GameStation_3
{
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��

	BYTE				iBackCount;						//������
	BYTE				iBeginTime;						//��ʼ׼��ʱ��
	BYTE				iThinkTime;						//����˼��ʱ��
	BYTE                iRobNTTime ;                    //������ʱ��
	BYTE				iCallScoreTime;					//�зּ�ʱ
	BYTE				iAddDoubleTime;					//�Ӱ�ʱ��

	BYTE				iCallScorePeople;				//��ǰ�з���
	BYTE				iGameFlag;						//�зֱ��
	BYTE				iCallScoreResult;				//���еķ�
	
	int					iUpGradePeople;					//ׯ��λ��
	int                 iCurOperator ;                 ///��ǰ��������

	UINT				iDeskBasePoint;					//���������
	UINT				iRoomBasePoint;					//���䱶��

	DWORD				iCardShape;						//��������
	int                 iGameMutiple   ;             ///��Ϸ�еı���
	int                 iAddDoubleLimit;             //�ӱ�����
	int                 iGameMaxLimit ;              ///��Ϸ�����Ӯ

	bool				bAuto[PLAY_COUNT];				//�й����
	bool				bCanleave[PLAY_COUNT];			//�ܷ���˳�
	BYTE				iCallScore[PLAY_COUNT];			//���ҽз����
	int 				iRobNT[PLAY_COUNT];				//���������
	int                 iUserDoubleValue[PLAY_COUNT] ;  ///��Ҽӱ����
	BYTE				iUserCardCount[PLAY_COUNT];		//�û������˿���Ŀ
	BYTE				iUserCardList[195];				//�û����ϵ��˿�
	BYTE                iGameBackCard[12] ;                 ///����
	BYTE 				iBackCardCount;						//��������
	__int64				iRunPublish;					//���ܿ۷�

	GameMutipleStruct   gameMutiple ;                      ///��Ϸ����  
	GameTaskStruct      gameTask ;                         ///��Ϸ����


};

//��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
struct GameStation_4	
{
	bool				bIsLastCard;						//�Ƿ�����������
	
	BYTE				iVersion;							//��Ϸ�汾��
	BYTE				iVersion2;							//��Ϸ�汾��
	BYTE				iBackCount;							//������

	BYTE				iBeginTime;							//��ʼ׼��ʱ��
	BYTE				iThinkTime;							//����˼��ʱ��
	BYTE                iRobNTTime ;                    //������ʱ��
	BYTE				iCallScoreTime;						//�зּ�ʱ
	BYTE				iAddDoubleTime;						//�Ӱ�ʱ��

	BYTE				bIsPass;							//�Ƿ񲻳�

	int					iRunPublish;						//���ܿ۷�	
	int					iBase;								//��ǰը������
	int					iUpGradePeople;						//ׯ��λ��
	
	int 				iCallScoreResult;					//�зֽ��
	int					iOutCardPeople;						//���ڳ����û�
	int					iFirstOutPeople;					//�ȳ��Ƶ��û�
	int					iBigOutPeople;						//�ȳ��Ƶ��û�

	UINT				iDeskBasePoint;						//���������
	UINT				iRoomBasePoint;						//���䱶��

	int                 iGameMutiple   ;             ///��Ϸ�еı���
	DWORD				iCardShape;							//��������
	int                 iAddDoubleLimit;             //�ӱ�����
	int                 iGameMaxLimit ;              ///��Ϸ�����Ӯ

	BYTE				iAwardPoint[PLAY_COUNT];			//����
	BYTE				iPeopleBase[PLAY_COUNT];			//�Ӱ�
	BYTE				iRobNT[PLAY_COUNT];					//������


	bool				bAuto[PLAY_COUNT];					//�й����
	bool				bCanleave[PLAY_COUNT];				//�ܷ���˳�

	BYTE				iUserCardCount[PLAY_COUNT];			//�û������˿���Ŀ
	BYTE				iUserCardList[195];					//�û����ϵ��˿�

	BYTE				iBaseOutCount;						//���Ƶ���Ŀ
	BYTE                iBaseCardList[45] ;                //�����ϵ���

	BYTE				iDeskCardCount[PLAY_COUNT];			//�����˿˵���Ŀ
	BYTE                iDeskCardList[PLAY_COUNT][45] ;    ///�����ϵ���

	BYTE				iLastCardCount[PLAY_COUNT];			//�����˿˵���Ŀ
	BYTE				iLastOutCard[PLAY_COUNT][45];		//���ֵ��˿�

	bool                bPass[PLAY_COUNT] ;                //����
	bool                bLastTurnPass[PLAY_COUNT] ;         //��һ�ֲ���

	BYTE                iGameBackCard[12] ;                 ///����
	BYTE 				iBackCardCount;						//��������

	GameMutipleStruct   gameMutiple ;                      ///��Ϸ����  
	GameTaskStruct      gameTask ;                         ///��Ϸ����

};

//��Ϸ״̬���ݰ�	�� �ȴ����̿�ʼ״̬  ��
struct GameStation_5
{
	BYTE				iVersion;							//��Ϸ�汾��
	BYTE				iVersion2;							//��Ϸ�汾��
	BYTE				iBeginTime;							//��ʼ׼��ʱ��
	BYTE				iThinkTime;							//����˼��ʱ��
	BYTE                iRobNTTime ;                    //������ʱ��
	BYTE				iCallScoreTime;						//�зּ�ʱ
	BYTE				iAddDoubleTime;						//�Ӱ�ʱ��

	int                 iGameMutiple   ;               ///��Ϸ�еı���
	int                 iAddDoubleLimit;             //�ӱ�����
	int                 iGameMaxLimit ;              ///��Ϸ�����Ӯ

	bool                bUserReady[PLAY_COUNT] ;     ///���׼��
	DWORD				iCardShape;							//��������

	UINT				iDeskBasePoint;						//���������
	UINT				iRoomBasePoint;						//���䱶��
	__int64				iRunPublish;						//���ܿ۷�
};

//�û�ͬ����Ϸ
struct UserArgeeGame
{
	BYTE				iPlayCount;							//��Ϸ����
	BYTE				iCardCount;							//�˿���Ŀ
};
///���ƿ�ʼ
struct  UserMingStruct
{
	bool    bStart  ;       ///�Ƿ�Ϊ��ʼ
	bool    bMing ;         ///����Ƿ�����
	BYTE    bDeskStaion ;   ///��ҵ�λ��
	BYTE    byCardCount ;   ///����ʱ����˿˵�����
};

//��Ϸ��ʼ
struct BeginUpgradeStruct
{
	BYTE				iPlayLimit;							//��Ϸ�ܾ���
	BYTE				iBeenPlayGame;						//�Ѿ����˶��پ�
	BYTE                bUserMing[PLAY_COUNT] ;            ///����������
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
	BYTE             iUserCardCount[PLAY_COUNT];		//��������
	BYTE             iUserCardList[108];				//���ƶ���
};
//���ƽ���
struct SendCardFinishStruct
{
	BYTE		bReserve;									
};

//�������ݰ�
struct BackCardExStruct
{
	BYTE				iGiveBackPeople;				//�������
	BYTE				iBackCardCount;					//�˿���Ŀ
	BYTE				iBackCard[12];					//��������

	GameTaskStruct      gameTask ;                      //��Ϸ����  

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
	bool 				bCallScoreflag;							//�зֱ��								
};
//������
struct RobNTStruct
{
	BYTE   bDeskStation;										//����������
	BYTE   byRobCount ;                                         //�������������
	int    iValue;												//���������
};

//����
struct ShowCardStruct
{
	BYTE bDeskStation;										//����
	BYTE iCardList[54];					                   //�˿���Ϣ
	BYTE iCardCount;						              //�˿���Ŀ
	int iValue;											  //����
	int iBase;											  //���� 20081204
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
	bool                bFinishTask ;                    //�Ƿ����������
	BYTE				iUpGradeStation;				//ׯ��λ��
	BYTE			    iUserCardCount[PLAY_COUNT];	    //�û������˿���Ŀ
	BYTE			    iUserCard[PLAY_COUNT][45];		//�û����ϵ��˿�

	GameMutipleStruct   gameMutiple ; 

    __int64  			iTurePoint[PLAY_COUNT];			//��ҵ÷�
	__int64				iChangeMoney[8];				//��ҽ��

	GameEndStruct()
	{
		memset(this , 0 ,sizeof(GameEndStruct)) ; 
	}

};

//��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	int					iRoomBasePoint;					//����
	int					iDeskBasePoint;					//���汶��
	int					iHumanBasePoint;				//��ͷ����

	int					bDeskStation;					//�˳�λ��
	__int64				iChangeMoney[8];				//��ҽ��
	__int64				iTurePoint[PLAY_COUNT];			//ׯ�ҵ÷�

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
	TCHAR name[PLAY_COUNT][61];
	//	int	iBasePoint;
	BYTE iStyle;			//��Ϸ�����Ƿ�Ϊǿ��,������������
	int	iBasePoint;			//ϵͳ����
	int	iDeskBasePoint;		//���������
	int iUpGradePeople;		//�f��
	int iGrade2;			//�f������
	__int64	iWardPoint[PLAY_COUNT];
	__int64 iAwardPoint[PLAY_COUNT];	//����
	__int64 iTotalScore[PLAY_COUNT];	//�ܻ���
	__int64 iMoney[PLAY_COUNT];			//��������Ǯ��
	BYTE iGameStyle;					//��Ϸ����
};
struct UserleftDesk
{
	BYTE bDeskStation;                //�������
};
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
#define ASS_USER_LEFTDESK               97               //����뿪���ӻ����

/********************************************************************************/
#endif
