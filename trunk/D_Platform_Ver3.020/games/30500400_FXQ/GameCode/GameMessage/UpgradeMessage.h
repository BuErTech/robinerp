#pragma once

//ͬ¥ ��ͨ��/��Ƶ�� ����  release/��Ƶ��  ��������������������

#ifndef AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE
#define AFC_SERVER_SHOWHAND_MESSAGE_HEAD_FILE

//�ļ����ֶ���
#define GAMENAME						TEXT("������")
//���� ID
#define NAME_ID							30500400			
//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
//��Ϸ����
#define PLAY_COUNT						4								
//�汾����
#define GAME_MAX_VER					1								//������߰汾
#define GAME_LESS_VER					1								//������Ͱ汾
#define GAME_CHANGE_VER					0								//�޸İ汾
//��Ϸ�ڲ������汾��
#define DEV_HEIGHT_VERSION				1				//�ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
#define DEV_LOW_VERSION					0				//�ڲ������Ͱ汾��

static TCHAR szTempStr[MAX_PATH] = {0};//�ݴ��ַ�����������ȡ dll_name �ȵ�ʱ��ʹ��
#define GET_CLIENT_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".ico"))
#define GET_SERVER_DLL_NAME(A)			(sprintf(A,"%d",NAME_ID),strcat(A,".dll"))
#define GET_CLIENT_BCF_NAME(A)          (sprintf(A,"%d",NAME_ID),strcat(A,".bcf"))    

//��Ϸ״̬����
#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_SEND_CARD					20				//����״̬
#define GS_PLAY_GAME					22				//��Ϸ��״̬
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 


///�ͻ��˺ͷ���˵ļ���״̬
#define     WAIT_PLAYER_DICE           50             ///�ȴ���Ҵ�ɫ��                  
#define     WAIT_DICE_ANI              51             ///�ȴ����ɫ�Ӷ���
#define     WAIT_PLAYER_CHOISE         52             ///�ȴ����ѡ������
#define     WAIT_PLANE_FLY_ANI         53             ///�ȴ����ӷ��ж���

const  int  PLAYER_PLANE_NUM  =  4 ;    /// ÿ�������ļܷɻ�
const  int  MAX_MAGIC_NUM     =  1 ;    ///��ҵ��ߵ�������ࣨ���Ժ���չ��
const  int  PUBLIC_PATH_STEP  =  52;    ///����·��
const  int  PRIVATE_PATH_STEP =  6 ;    ///˽�е�λ��
const  int  FINISH_PATH_STEP  =  56 ;   ///�������·����Ҫ�Ĳ���
const  int  STEP_INTEVAL      =  13 ;   ///���ҷɻ�֮��ļ��
const  int  SHORTCUT_STEP_VALUE = 17 ;  ///����߽ݾ�ʱ��·��ֵ 
const  int  SHORTCUT_OTHER_STEP  = 29 ; //���������һ�ߵ�λ��
const  int  STEP_TAKEOFF        = 254 ; ///���λ�ö���
const  int  STEP_HOME           = 255 ; ///�ҵ�λ��   
///�������ӵļ�����λ
enum  enumSide
{
	emGreen = 0 ,       ///�̷�
	emRed  = 1 ,        ///�췽
	emYellow = 2 ,      ///�Ʒ�
	emBlue =  3 ,       ///����
	emInvalidate = 255  ///�������κ�һ��
};
///�ɻ��ļ���״̬
enum enumPlaneState
{
	emStandby = 0,	     //�ڷɻ���
	emTakeoff = 1,	     //��ɣ�����ɵ��Ǹ�Ȧ��
	emFlying = 2,	     //�ڷ�
	emLanding = 3,	     //��½����ָ���������岽����
	emStop = 4	,	     //�ɻ���ͣ������
	emError = 255        //�����״̬
};
///ħ�����ࣨ��ʱֻ���ɽ趫�磩
enum  enumMagicType
{
	emWindMagic   = 1 ,    ///�ɽ趫��
	emErrorMagic  = 255    ///����ĵ���
};
///��Ϸ�зɻ��ķ���
enum enumDirection
{
	emLeft     = 0,    
	emLeftUp   = 1 ,
	emUp       = 2 , 
	emUpRight  = 3 ,
	emRight    = 4 ,
	emRightDown= 5 , 
	emDown     = 6,
	emDownLeft = 7 ,
	emErrorDir = 255   
};
///��ҷɻ�����
enum  ActiveType
{
	emTakeOfFActive = 10 , // ���
	emFlyingActive  = 11 , //����
	emShortCutActive= 12 , //���˽ݾ�
	emBackActive    = 13 , //����
	emBackTakeOff   = 14 , //�ص����״̬
	emHomeActive    = 15 , //����
	emArriveHome    = 16 , //������� 
	emMutiPlane     = 16 , //����˵ķɻ�����
	emErrorActive   = 255 
};
///�ɻ�������
struct PLANE
{
	BYTE            emSide;		    //�ɻ�������һ����
	BYTE            emPlaneState;	//�ɻ�״̬
	BYTE            cbStep;		    //�ɻ�������λ�� (��������½����λ��,�Ƿ��ڣ�									��½����cbPlaneState����,��½��ֻ���ĸ�ֵ										01234)
	BYTE            cbAirPortPos;	//�ɻ��ڻ�����λ��(��λ�ó�ʼ���󲻻��)
	BYTE            cbLandPos;		//��½��λ��
	PLANE()
	{
		memset(this , 0, sizeof(PLANE)) ; 
	}
	void Reset()
	{
		memset(this , 0 , sizeof(PLANE)) ;  
	}
};
///ħ��������
struct MAGIC
{
	BYTE cbMagic;  ///ħ��
	BYTE cbCount;  ///ħ���ĸ���
};
// ÿ������
struct STEP 
{
	enumDirection enPlaneDt;	        // �ɻ��ڴ˸��ƶ�ʱ�ķ���
	BYTE          cbSide;				// ������һ���ĸ���	`
	BYTE          cbMagic;				// ����ӵ��ʲô����ħ��
	CPoint        ptPathPos ;          //���ӵ�λ��

	STEP()
	{
		cbSide = emInvalidate;
		cbMagic = emErrorMagic;
		enPlaneDt =	emErrorDir;
		ptPathPos.SetPoint(0 , 0 ) ; 
	}
};

//��Ϸ״̬���ݰ�	�� �ȴ���������״̬ ��
struct GameBaseStation
{
	BYTE				iVersion1;						//��Ϸ�汾��
	BYTE				iVersion2;						//��Ϸ�汾��

	bool                bUserReady[PLAY_COUNT] ;    ///����Ƿ���׼��

	bool                bUserAuto[PLAY_COUNT] ; 

	BYTE                cbBeginTime ;                  //��ʼʱ��
	BYTE                cbDiceTime  ;                  //��ɫ��ʱ��
	BYTE                cbThinkTime ;                 //��ɫ��ʱ��
};
///��Ϸ���������ݰ�
struct PlayingGameStation: GameBaseStation
{
	BYTE   cbOperater ;  ///��ǰ��������
	BYTE   cbPropCount[PLAY_COUNT] ;  ///��ҵĵ���
	int    iPlayerfight[2] ;  ///��Ҵ�����˷ɻ�������
	PLANE  PlaneData[PLAY_COUNT][PLAYER_PLANE_NUM] ;  ///��ҵķɻ�״̬
};
///�ȴ���һ�����ݰ�
struct WaitNextGameStation: GameBaseStation
{

};
//��Ϸ��ʼ
struct BeginUpgradeStruct
{
	BYTE				iBeenPlayGame;			//�Ѿ����˶��پ�
	BYTE				iPlayLimit;				//��Ϸ�ܾ���
};
///֪ͨ��Ҹ��´�ɫ�ӽ���
struct WaitDiceStruct
{
	BYTE  cbDeskStation ; 
};
///���Ҫ��������
struct NotifyDiceStruct
{
	BYTE    cbDeskStation ;             ///����
	BYTE    cbDiceValue ;              ///����ֵ
	BYTE    cbPropCount ; 
};
///֪ͨ���ѡ��ɻ�����
struct NotifyChoisePlane
{
	BYTE  cbDeskStation ; 
};
///���ѡ��ɻ�
struct PlayerChoiseStruct
{
	BYTE  cbDeskStation ; 
    int   inDexPlane ; 
};
///��Ҷ������й����еĶ����ṹ��
struct PlaneFlyStruct
{
	BYTE     cbSide ;  ///˭�ҵķɻ�
    BYTE     cbIndex  ;       ///�ɻ�������
	BYTE     cbOperateType ;   ///�ɻ��Ķ���
	BYTE     cbComeByStepIndex[20] ; ///����߹��Ĺ켣
	int      cbStepCount ;          ///�ɻ��߹��������

	PlaneFlyStruct()
	{
		memset(this , 255 ,sizeof(PlaneFlyStruct)) ; 
		cbStepCount = 0 ;
	}
};
///��ҷɻ���������
struct PlayerActiveStruct
{
	BYTE            cbDeskStation ;          ///��ǰ���������
	BYTE            cbDiceValue ;           ///��ǰ���ɫ�ӵ���
	int             iPlaneCount ;            ///�ж����ķɻ��ĸ���
	int             iPlayerfight[2];         ///��¼˫����ս��
	PlaneFlyStruct  PlaneActive[8] ;        ///��Ҫ�ж����ķɻ���Ӧ�ò��ᳬ��8�ܰɣ�  
};
///������ҵķɻ�״̬��ͬ����˱���һ�£�
struct  ScenceChangeStruct
{
	BYTE   cbDeskStation ; 
	PLANE  PlaneData[PLAY_COUNT][PLAYER_PLANE_NUM] ; 
};
///��ҵ���֪ͨ����˴�ɫ��
struct PropDiceStruct
{
	BYTE    cbDeskStaion ;       //ʹ�õ��ߵ����
	BYTE    cbIndex ;            //��������
};

//�й����ݽṹ
struct AutoStruct
{
	BYTE bDeskStation;	// λ��
	bool bAuto;			// �Ƿ��й�
	BYTE byAutoCount;	// �Ѿ��йܴ���
};

//��Ϸ����ͳ�����ݰ�
struct GameEndStruct
{
	__int64				   iTurePoint[PLAY_COUNT];	  	  ///��ҵ÷�
    __int64				   iChangeMoney[8];				  ///��ҽ��
};

//��Ϸ����ͳ�����ݰ�
struct GameCutStruct
{
	int					bDeskStation;					//�˳�λ��
	__int64				iTurePoint[PLAY_COUNT];					//ׯ�ҵ÷�
	__int64				iChangeMoney[8];				//��ҽ��

};


/********************************************************************************/

// ���ݰ���������ʶ
//warning:79��IDԤ���������ͻ���,����ʹ��
/********************************************************************************/

#define  ASS_UG_USER_SET				50				//������Ϸ
#define  ASS_GAME_BEGIN					51				//��Ϸ��ʼ
#define  ASS_APPLY_DICE                 52               //�ͻ��������ɫ��
#define  ASS_NOTIFY_DICE                53              //֪ͨ��Ҵ�����
#define  ASS_NOTIFY_DICE_FINISH         54              //��Ҵ�ɫ�ӽ���
#define  ASS_NOTIFY_CHOISE_PLANE        55             ///֪ͨ���ѡ��ɻ�����
#define  ASS_PLAYER_CHOISE_PLANE        56              ///���ѡ������
#define  ASS_PLANE_FLY_ANI              57              ///֪ͨ�ͻ��˽��з��ж���
#define  ASS_PLANE_FLY_FINISH           58             ///����˸��¿ͻ��˷��ж�����
#define  ASS_DICE_PROP                  59            ///��Ҵ�ɫ�ӵ���
#define  ASS_AUTO                       60 

#define  ASS_SUPER_MSG                  79              ///��ҳ�������

#define  ASS_SAFE_END                   83
#define  ASS_CONTINUE_END				84				//��Ϸ����
#define  ASS_NO_CONTINUE_END            85              //��Ϸֱ���˳�
#define  ASS_CUT_END					86				//�û�ǿ���뿪


/********************************************************************************/
#endif
