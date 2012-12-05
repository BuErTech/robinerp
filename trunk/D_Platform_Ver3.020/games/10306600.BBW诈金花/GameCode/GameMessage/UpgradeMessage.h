#pragma once

//ͬ¥ ��ͨ��/��Ƶ�� ����  release/��Ƶ��  ��������������������

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

//�ļ����ֶ���
#define GAMENAME						TEXT("����")


#define NAME_ID							10306600								//���� ID


//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
//��Ϸ��Ϣ

#define PLAY_COUNT						6								//��Ϸ����
#define MAX_CARD_COUNT                  3


//��Ϸ�ڲ������汾��
#define DEV_HEIGHT_VERSION				3				//�ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
#define DEV_LOW_VERSION					1				//�ڲ������Ͱ汾��

//#define SKIN_FOLDER						TEXT("ddz")	//ͼƬ�ļ���
//#define SKIN							TEXT("cardskin1")	//����ͼƬ����һ��
#define SKIN_CARD						TEXT("cardskin")		//����ͼƬ�ļ���
static TCHAR szTempStr[MAX_PATH] = {0};//�ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ��
#define GET_CLIENT_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))
#define GET_SERVER_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))
#define GET_SKIN_FOLDER(A)              (sprintf(A,"%d",NAME_ID),strcat(A,""))     //��ȡͼƬ�ļ���
#define GET_KEY_NAME(A, B, C, D)			(sprintf(A,"%s%d%s", B, C, D),strcat(A,""))
#define GET_CLIENT_BCF_NAME(A)          (sprintf(A,"%d",NAME_ID),strcat(A,".bcf"))    

#define SUPER_PLAYER                    10                         //�������
#define MAX_NAME_INFO                   256

//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_SEND_CARD					20				//����״̬
#define GS_WAIT_BACK					21				//�ȴ���ѹ����
#define GS_PLAY_GAME					22				//��Ϸ��״̬
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 

//��ע����

#define TYPE_GIVE_UP					0x00			//����
//#define TYPE_NOTE_100					0x01			//100
//#define TYPE_NOTE_200					0x02			//200
//#define TYPE_NOTE_400					0x03			//400ע
#define TYPE_NOTE						0x01			//��ע
#define TYPE_ADD						0x02			//��ע
#define TYPE_FOLLOW						0x03			//��ע
//#define TYPE_SHUTTLE					0x04			//����
#define TYPE_OPENCARD					0x04			//����
#define TYPE_LOOKCARD					0x05			//����
#define TYPE_NORMAL                     0x06            //����״̬



#define TYPE_OPEN_LOSE                  0x06            //����ʧ��
#define TYPE_OPEN_WIN                   0x07            //����ʧ��
#define TYPE_REACH_LIMITE_NEXT          0x08            //��һ����ע��ﵽ����
#define TYPE_REACH_LIMITE_SELF          0x09            //�Լ���ע��ﵽ����
#define TYPE_COMPARE_CARD               0x09           //���Ʋ���

/********************************************************************************/

//��Ϸ���ݰ�

/********************************************************************************/

//�����û����
struct tagUserResult
{
	//	BOOL bShowOnly;				//����ʾ
	//BOOL bAddFlag;				//�ӱ����
	BYTE bCountNotePeople;		//��ǰδ���������
	int bAddTime;				//Ѻע����
	BYTE iNowBigNoteStyle;		//��ע����
	BYTE iNowBigPeople;
	BYTE iCutOutLastpeople;
	BYTE iCutOutLastType;
	BYTE iOutPeople;			//��ע��
	int iNowBigNote;			//��ǰ��ע
	int iTotalNote;				//����ע
    int iUserStation[PLAY_COUNT] ;//�����ע����
	int iFirstOutPeople;         //��һλ�����
	__int64 iUserNote[PLAY_COUNT];   //�û�������ע

	bool bIsFirstNote;
	
	//	int iCurNote;		//��ǰ��ע��
};
//��������
struct lookCardResult
{
   BYTE             bDeskStation;
   BYTE				iUserCardCount;				//�û����ϵ�����
   BYTE				iUserCard[5];				//�û����ϵ���
 //  bool  ming[PLAY_COUNT];
};

struct GameStationBase
{
	BYTE bGameStation;
};
//��Ϸ״̬���ݰ�	�� �ȴ���������״̬ ��

struct GameStation_1 : GameStationBase
{

	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	//	char				pUpDateHttp[50];
	//��Ϸ��Ϣ
	//	BYTE				iCardCount;						//�˿���Ŀ
};

//��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
struct GameStation_2 : GameStationBase
{
	BYTE                bEXFlag[12];                    //zht 2011-4-12, ��ʹ�õ��ֶΣ�ռλ��
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��
	//��Ϸ��Ϣ
	//	BYTE				iBeginStation;					//��ʼλ��
	//	BYTE				iEndStation;					//����λ��
	BYTE				iThinkTime;						//��Ϸ����
	BYTE				iBeginTime;						//��Ϸ����
	BYTE				iAllCardCount;						//�˿���Ŀ
	int					iRoomBasePoint;					//���䱶��
	int					iRunPublish;					//���ܿ۷�
	BYTE                bEXFlag1[12];                    //zht 2011-4-12, ��ʹ�õ��ֶΣ�ռλ��

	
	//   ���������Ϣ
	__int64					iLimitNote;						//��ע�ⶥֵ   ��ע / ��ע
	int					iBaseNote;						//��ע	       ��ע
	int                 iGuoDi;                         //����
	__int64                 iLimtePerNote;                   //            ��ע���ޣ���ע��Ҫ * 2

	BOOL                bShowIdOrPlay;

};

//��Ϸ״̬���ݰ�	�� ��Ϸ���ڷ���״̬ ��
struct GameStation_3 : GameStationBase
{
	BYTE                bEXFlag[12];                    //zht 2011-4-12, ��ʹ�õ��ֶΣ�ռλ��
	//��Ϸ�汾
	BYTE				iVersion;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��

	//��Ϸ��Ϣ
	//	BYTE				iBeginStation;					//��ʼλ��
	//	BYTE				iEndStation;					//����λ��

	BYTE				iThinkTime;						//��Ϸ����
	BYTE				iBeginTime;						//��Ϸ����
	BYTE				iAllCardCount;						//�˿���Ŀ
    BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���

	BYTE				iSysCardCount;					//��ʣ��������
	BYTE                bEXFlag1[12];                    //zht 2011-4-12, ��ʹ�õ��ֶΣ�ռλ��
		//״̬��Ϣ
	//	BYTE				iUserCardCount;					//�û������˿���Ŀ
	//	BYTE				iUserCard[45];					//�û����ϵ��˿�
	BYTE				iUserCardCount[PLAY_COUNT];				//�û����ϵ�����
	BYTE				iUserCard[PLAY_COUNT][5];				//�û����ϵ���	
	//����״̬����

	int					iUpGradePeople;					//ׯ��λ��
	int					iRoomBasePoint;					//���䱶��
	int					iRunPublish;					//���ܿ۷�
	
	__int64			    iLimitNote;						//��ע�ⶥֵ   ��ע / ��ע
	int					iBaseNote;						//��ע	       ��ע
	int                 iGuoDi;                         //����
	__int64             iLimtePerNote;                  //��ע���ޣ���ע��Ҫ * 2

	__int64				iTotalNote[PLAY_COUNT];					//�û���ǰ��ע
	BOOL                bShowIdOrPlay;
	char                strCityText[PLAY_COUNT][50];       //������������ַ
	char                strUserIDText[PLAY_COUNT][50];     //����������ID
	bool                bSuperUser[PLAY_COUNT];             //������Ƿ��ǳ������
	BOOL                bPlayer[PLAY_COUNT];

	
	
};

//��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
struct GameStation_4 : GameStationBase
{
	BYTE                bEXFlag[12];                    //zht 2011-4-12, ��ʹ�õ��ֶΣ�ռλ��
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
	//BYTE                bNtPeople;

	BYTE                bEXFlag2[12];                    //zht 2011-4-12, ��ʹ�õ��ֶΣ�ռλ��
	__int64				iLimitNote;						//��ע�ⶥֵ   ��ע / ��ע
	int					iBaseNote;						//��ע	       ��ע
	int                 iGuoDi;                         //����
	__int64             iLimtePerNote;                   //            ��ע���ޣ���ע��Ҫ * 2

	int                 iCurNote;                       //��һλ�����עֵ���Ѱ�ע����

	int					iRoomBasePoint;					//���䱶��
	int					iRunPublish;					//���ܿ۷�
	//����״̬����
	BYTE                bNtPeople;                      //ׯ��λ��
	BYTE				iUpGradePeople;					//��עλ��
	BYTE				iOutCardPeople;					//���ڳ����û�
	BYTE				iFirstOutPeople;				//�ȳ��Ƶ��û�
	__int64				iTotalNote[PLAY_COUNT];			//�û���ǰ��ע
	__int64             iThisGameNote[PLAY_COUNT];		//������Ϸ��ע��(��ע)
	__int64				iTotalGameNote[PLAY_COUNT];
	BOOL                iMing[PLAY_COUNT];              //����״̬

	bool                bOpenLose[PLAY_COUNT];          //��ұ����Ƿ�����
	bool                bIsGiveUp[PLAY_COUNT];                      //����Ƿ����

	bool               bIsFirstNote;                     //�Ƿ�Ϊ��һ����ע

	int				   iTimeRest;						// ��ʱ��ʵ��ʣ�µ�ʱ�䣬���ڱ�ʾ��������ʱʣ��ʱ��ֵ
	char                strCityText[PLAY_COUNT][50];       //������������ַ
	char                strUserIDText[PLAY_COUNT][50];     //����������ID
	bool                bSuperUser[PLAY_COUNT];             //������Ƿ��ǳ������

	//״̬��Ϣ
	//	int					iIngNote;						//��ǰ��ע�û�
	//	int					iIngPlayStatus;					//������Ϸ�Ķ���
	//	int					iUserNote[4];					//�û�������ע����
	//	int					iTotalNote;						//������ע
	tagUserResult		tag;
	BYTE				iUserCardCount[PLAY_COUNT];				//�û����ϵ�����
	BYTE				iUserCard[PLAY_COUNT][5];				//�û����ϵ���

	bool                bOpenLoser[PLAY_COUNT];
	BOOL                bPlayer[PLAY_COUNT];

	BYTE                byWin[PLAY_COUNT];
		BOOL                bShowIdOrPlay;
	
};

//��Ϸ״̬���ݰ�	�� �ȴ����̿�ʼ״̬  ��
struct GameStation_5 : GameStationBase
{
	BYTE                bEXFlag[12];                    //zht 2011-4-12, ��ʹ�õ��ֶΣ�ռλ��
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

	BYTE                bEXFlag2[12];                    //zht 2011-4-12, ��ʹ�õ��ֶΣ�ռλ��

	__int64					iLimitNote;						//��ע�ⶥֵ   ��ע / ��ע
	int					iBaseNote;						//��ע	       ��ע
	int                 iGuoDi;                         //����
	__int64                 iLimtePerNote;                   //            ��ע���ޣ���ע��Ҫ * 2
	char                strCityText[PLAY_COUNT][50];       //������������ַ
	char                strUserIDText[PLAY_COUNT][50];     //����������ID
	bool                bSuperUser[PLAY_COUNT];             //������Ƿ��ǳ������

	//����״̬����
	int					iUpGradePeople;					//ׯ��λ��

	BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
		BOOL                bShowIdOrPlay;
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
	__int64					iLimitNote;						//���ע��
	int                 iGuoDi;                         //����
	__int64                 iLimtePerNote;                   //            ��ע���ޣ���ע��Ҫ * 2

	BYTE				SysCard[52];					//��ǰ��

	BOOL                bIsShowIdOrPlay;                  //��ʾid�������
	char                strCityText[PLAY_COUNT][50];
	char                strUserIDText[PLAY_COUNT][50];
	bool                bSuperUser[PLAY_COUNT];             //������Ƿ��ǳ������
	BYTE                bNtStation;                      //ׯ��      
 

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
	BYTE				bCard[PLAY_COUNT][MAX_CARD_COUNT];					//�Ʊ��
	BYTE                bCardCount[PLAY_COUNT];                             //������
	//BYTE				bDeskStatin;							//��һ�ֳ�����
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
	BYTE                bNtPeople;                      //ׯ��λ��
	BYTE				byUserData[PLAY_COUNT][2];					//ȷ������Ƿ�����˺���ҵ��ƴ�С������ֻ���������ˣ�lym
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
	__int64					iTurePoint[PLAY_COUNT];					//ׯ�ҵ÷�
	__int64					iChangeMoney[8];
	BYTE				iUpGradeStation;						//ׯ��λ��
	BYTE				bCard[PLAY_COUNT][MAX_CARD_COUNT];		//�Ʊ��	Added by zxd 20090928 Ϊ��ֹ���ף������·�������ҵ��ƣ������Ϸ����ʱҪ��
};

//��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	int					bDeskStation;					//�˳�λ��
	__int64					iTurePoint[PLAY_COUNT];					//ׯ�ҵ÷�
	__int64					iChangeMoney[8];
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
//�������ݰ�
struct SetCardStruct
{
	BYTE bCardCount;//������
	BYTE bPlayCount;//�����
	int  nPlayCardCount[8]; //ÿ�����������
	BYTE bPlayCard[8][60]; // ÿ����ҵ���
	SetCardStruct()
	{
		memset(this,0,sizeof(SetCardStruct));
	}
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
BUTTON_TYPE_ADD,	//��ע
BUTTON_TYPE_FOLLOW,//����
BUTTON_TYPE_SHUTTLE,//����
};*/

//�û�������Ϣ
struct tagUserProcess
{
	BYTE iVrebType;//������İ�ť
	int  iNote;		//��ע��
	BOOL bMing;     //���Ʒ�?
};

struct tagCompareCard :  public tagUserProcess 
{
	BYTE byComparedStation ;  //���Ա��Ƶ����   
};

struct tagCompareCardResult
{
	BYTE iNt;		//������
	BYTE iNoNt;     //������
	BYTE iLoster;	//����
	BYTE bWinner;		// Add By ZXD 20090820 ʤ���ߣ������ƽ�������һ�����ϴﵽ��ע���ޣ����ʱ�ͻ�����Ҫ֪��ʤ������˭
	int iCurNote;		//��ǰ�����ע��
	int iNote;          //��ǰ��Чע��
	BYTE bAddStyle;   //����
	bool bGameFinish;
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
	int iCurNote;		//��ǰ�����ע��
	int iNote;          //��ǰ��Чע��
	BYTE bNextDeskStation;
	__int64  iLimiteNote;             //�����ע
};
struct GameFinishNotify
{
	TCHAR name[PLAY_COUNT][61];
	//int dwUserID[PLAY_COUNT];         //��ӡuserid
	int	iBasePoint;
	BYTE iStyle;			//��Ϸ�����Ƿ�Ϊǿ��,������������
	int	iWardPoint[PLAY_COUNT];
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
#define ASS_GAME_PLAY					59				//��ʼ��Ϸ
#define ASS_NEW_TURN					64				//��һ�ֿ�ʼ
#define ASS_CONTINUE_END				65				//��Ϸ����
#define ASS_NO_CONTINUE_END				66				//��Ϸ����
#define ASS_CUT_END						67				//�û�ǿ���뿪
#define ASS_SALE_END					68				//��Ϸ��ȫ����


#define ASS_VREB_CHECK					179				//�û�����

#define ASS_SHUTTLE						80				//����
#define ASS_ADD_NOTE					82				//��ע
#define ASS_GIVE_UP						86				//����
#define ASS_MODIFY_PLAY_NOTE			90				//�����û�����ע
#define ASS_NOTE						91				//��ע
#define ASS_NOTE_RESULT					92				//��ע���
#define ASS_COLLECT_JETON				94				//�ռ�����
#define ASS_BIPAI_RESULT                95              //���ƽ��
#define ASS_GM_SUPERCLIENT				100				//�����͑���
#define ASS_FINISH_COMPARE              101             
#define ASS_AI_WIN                      102             //�����˱�Ӯ
#define ASS_SET_TEST_CARD               120             //�������趨��
/********************************************************************************/
#endif
