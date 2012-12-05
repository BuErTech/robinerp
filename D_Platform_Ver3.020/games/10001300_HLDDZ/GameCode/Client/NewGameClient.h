#include "IGameImpl.h"
#include "../GameMessage/UpGradeLogic.h"
#include <irrKlang.h>

using namespace irrklang;

#define GetControlNode(A ,IControl , IControlID ) \
	IControl *A=  dynamic_cast<IControl *>(m_pUI->GetIControlbyID(IControlID)); \
	if (NULL != A) \
	A

const   int  GAME_WAIT_STATION    = 1 ;
const   int  GAME_BEGIN_STATION   = 2 ; 
const   int  GAME_FINISH_STATION  = 3 ; 

const  int  TIME_GAME_BEGIN     = 200 ;    ///��Ϸ��ʼ��ʱ��
const  int  TIME_SEND_CARD      = 201 ;   ///��ҷ��ƶ�ʱ��
const  int  TIME_CALL_SCORE     = 202 ;    ///��ҽз�
const  int  TIME_USER_ROB_NT    = 203 ;   ///���������
const  int  TIME_GAME_DOUBLE    = 204 ;   ///��Ҽӱ�
const  int  TIME_SHOW_CARD      = 205 ;   ///�������
const  int  TIME_USER_OUT_CARD  = 208 ; ///��ҳ��ƶ�ʱ��

const  int  TIME_LAST_TURN      = 210 ;  ///��һ��


//������������
#define SOUND_OUT_CARD				0x10				//��������
#define SOUND_WARNING				0x20				//����
#define SOUND_SEND_CARD				0x30				//��������
#define SOUND_OUT_CARD_DEF			0x40				//Ĭ�ϳ�������
#define SOUND_OUT_CARD_BOMB			0x50				//��ը��
#define SOUND_OUT_CARD_PASS			0x60				//����
#define SOUND_INVALIDATE			0x70				//��ʾʧ��
#define SOUND_START_DEF				0x80				//�ĸ��˿�ʼ��Ϸ��
#define SOUND_GAME_OVER				0x90				//��Ϸ����
#define SOUND_ADD_DOUBLE			0xA0				//�Ӱ�
#define SOUND_NOT_ADD_DOUBLE        0xA1                 //���ӱ�
#define SOUND_CALL_NT               0xA2                 //�е���
#define SOUND_NOT_CALL_NT           0xA3                 //����
#define SOUND_ROB_NT                0xB0                 //������
#define SOUND_NO_ROB_NT				0xC0				//��������
#define SOUND_SHOW_CARD				0xD0				//����

#define SOUND_WIN					0x01				//ʤ��
#define SOUND_LOSE					0x02				//��
#define SOUND_CUT_END               0x03                ///����
#define SOUND_ERROR					0x04				//���Ƴ�����
#define SOUND_READY					0x05				//��Ϸ��ʼ
#define SOUND_START					0x06				//׼������һ����
#define SOUND_START_OUT_CARD		0x07				//����һ����
#define SOUND_CALL_SCORE			0x08				//�з�
#define SOUND_OUT_CARD_FINISH		0x09				//ĳ��ҳ��ƽY��
#define SOUND_GAME_PASS				0x0A				//����
#define SOUND_FRIEND_APPEAR			0x0B				//���ѳ��F
#define SOUND_USER_RUNAWARY			0x0C				//��������
#define SOUND_USER_LEFT				0x0D				//�����뿪
#define SOUND_GIVE_BACK				0x0E				//���
#define SOUND_SHUNZHI				0x13				//˳��
#define SOUND_FEIJI					0x14				//�ɻ�
#define SOUND_BAOJING               0x15
#define SOUND_LAST_CARD             0x16                // �������
#define SOUND_BIG_SHAPE             0x17                //�������

#define SOUND_GAME_BG               0xff                 //������Ч



interface IGameFrame;
interface IUserInterface;
class CNewGameClient : public IGameImpl
{
public:
    CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI);
    virtual ~CNewGameClient();
private:
    CNewGameClient(const CNewGameClient &other);
    CNewGameClient &operator = (const CNewGameClient &other);


private:
	///�����Ϸָ��
	IGameFrame         *m_pGameFrame;
	///������Ϸָ��
	IUserInterface     *m_pUI;

	ISoundEngine       *m_SoundEngine;               ///��Ϸ��Ч

	ISoundEngine       *m_SoundEnginBg ;              ///������Ч


public: 

	CUpGradeGameLogic   m_cLogic ;             ///�ͻ����߼�

	BYTE                m_byGameStation ;         ///��ǰ����Ϸ״̬
	int                 m_iMyDeskStation ;       ///����Լ�����Ϸλ��
  
	///ʱ���б�
	BYTE				m_byThinkTime;		   ///������Ϸ����
	BYTE                m_byRobNTTime ;        ///���������
	BYTE				m_byBeginTime;		   ///��Ϸ��ʼʱ��
	BYTE				m_byCallScoreTime;	   ///��Ϸ��ʼʱ��
	BYTE				m_byAddDoubleTime;	   ///��Ϸ��ʼʱ��

	BYTE                m_byNtPeople ;         ///��ǰ�ĵ��� 
	BYTE                m_byOperatePerson ;            ///��ǰ���������(�зֻ�������ʱ�õ���
	BYTE                m_byOutCardPeople ;    /// ��ǰ���Ƶ���

	BYTE                m_byStation ;  //���ƹ����е�״̬

	bool                m_bUserReady[PLAY_COUNT] ;  ///��Ϸ���׼��״̬

	///��ҵ�������
	BYTE                m_byHandCardList[PLAY_COUNT][45] ;    ///������е�������
	BYTE                m_byHandCardCount[PLAY_COUNT] ;      ///��������Ƶ�����

	BYTE                m_byBackCardList[12] ;               ///��Ϸ�еĵ�������
	BYTE                m_byBackCardCount ;                  /// ��Ϸ�еĵ�������

	BYTE                m_byDeskCardList[PLAY_COUNT][45] ;   ///��Ϸ�������������
	BYTE                m_byDeskCardCount[PLAY_COUNT] ;      ///��Ϸ�����������

	BYTE                m_byBaseCardList[45] ;               ///�����ϱȽϵ���
	BYTE                m_byBaseCardCount ;                  ///�����ϱȽ��Ƶ�����

	BYTE                m_byLastTurnCardList[PLAY_COUNT][45] ; 
	BYTE                m_byLastTurnCardCount[PLAY_COUNT] ; 

	bool                m_bUserPass[PLAY_COUNT] ;               ///��ҵ�ǰһ��pass
	bool                m_bLastTurnPass[PLAY_COUNT] ;           //�����һ��pass

	bool                m_bHaveLastTurnData ;    ///�Ƿ�����һ�ֵ�����

	BYTE                m_iSendAllCardCount ;             ///��ǰ���͵��Ƶ�����  
	BYTE                m_SendCardList[PLAY_COUNT][45] ;  ///��ҷ��ƵĽ��    
	BYTE                m_iSendCardCount[PLAY_COUNT] ;    ///��ҷ��Ƶ�����

	BYTE                m_bySortCard ;              ///����ʽ

	bool			    m_bAutoCard;				//�Ƿ��йܴ���
	bool                m_bAutoFlag[PLAY_COUNT] ;   ///����йܱ�ʶ

	int                 m_bAutoOutTime ; 
	int                 m_iUserCallScore[PLAY_COUNT] ;
	int                 m_iUserRobNtValue[PLAY_COUNT] ;
	int                 m_iUserDoubleValue[PLAY_COUNT] ;
	
	GameTaskStruct      m_gameTask ; 
	GameEndStruct       m_gameResult ; ///��Ϸ����

	int                 m_iGameBaseMutiple ;   ///��Ϸ������ע

	int                 m_iAddDoubleLimit ;  

	int                 m_iGameMaxLimit ; 

	GameMutipleStruct   m_MutipleGather ;  ///��Ϸ�еı�������

	bool                m_bSoundPlay ;  //�Ƿ���Բ�������
	bool                m_bSoundBgPlay ;  //������Ч
	bool                m_bAllowWatch ; // �Ƿ������Թ�
	bool                m_bStartMode ;  //��ʼģʽ�����˳�ģʽ
	bool                m_bWatchMode ;  //�Ƿ����Թ�ģʽ
	CString             m_strCoinName ;   ///��Ϸ�еĽ��


 public:
    /// ��ʼ��
    virtual int Initial();
    /// ��ʼ��UI
    virtual int InitUI();
    /// ��Ϸ��Ϣ
    virtual int HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen);
    /// ������Ϣ
    virtual int OnUIMessage(TUIMessage* pMessage);
    /// ��������״̬
    virtual int ResetGameFrame(void);
    /// ��ҽ���
    virtual int GameUserCome(void);
    /// ����뿪
    virtual int GameUserLeft(int nIndex);
    /// ��Ҷ���
    virtual int GameUserOffLine(void);
    // �Ƿ�������Ϸ������Ϸʵ��
    // �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
    virtual bool IsPlayingGame();          
    /// ��ʱ����Ϣ
    virtual void OnTimer(int nTimerID);
    /// ��״̬
    virtual void SetStationParameter(BYTE bGameStation);
    ///������Ϸ״̬
	virtual BYTE GetStationParameter();
	///��Ϸ����
	virtual bool OnGameQuiting();

public:
	///��ʼ����Ϸ�еı���
	int   InitialGameData(void) ; 
	///��ʼ����Ϸ�Ľ���
	int   InitialGameUI(void) ; 
	///ɾ�����ж�ʱ��
	void  KillAllGameTime(void) ;
	///����������Ϸ�н���
	void  ResetGameByStation(BYTE  iGameStation) ; 

	///��ҽ�����Ϸ
	void  SetGameStation(void * pBuffer,int nLen);
	///��ʾ�����ؿ�ʼ����ʱʱ��
	void  ShowOrHideUserTime(BYTE bDeskStation,int nShowMaxNum,int nShowMinNum,bool bFlag) ;
	///�������ж�ʱ��
	void  HideAllTime(void) ;

    ///��ʾ���׼��״̬
	void  ShowGameReady(bool bShow) ; 
	///��ʾ����Ԫ�غ���
	void  ShowGameBeginBtn(bool bShow) ; 
	///��ʾ��������
	void  ShowBackCard(bool bShow) ; 
	///�ͻ�����������
	void  ShowFirstMyCard(BYTE  iDeleteCard[] ,BYTE iDeleteCount ,BOOL bShow) ; 
    ///��ʾ��ҵ�����
	void  ShowUserHandCard(BYTE bDeskStation , bool bShow ,bool bShowAll = false) ;
	///��ʾ��ҵķ���
	void  ShowUserSendCard(BYTE bDeskStation ,bool bShow) ;
	///��ʾ�����Ƶ�����
	void  ShowUserCardCount(BYTE bDeskStation , bool  bShow , bool bSend) ; 
	///��ʾ��������ϵ���
	void  ShowUserDeskCard(BYTE bDeskStation , bool bShow) ;
	///��ʾ��һ���˿�
	void  ShowLastTurnCard(bool bShow) ; 
	///��ʾ����ͼƬ
	void  ShowUserPass(BYTE bDeskStation , bool bShow)  ;
	///�������е��ƿؼ�
	void  DownUserAllCard(bool bShow); 
	///��ʾ�йܰ�ť��ȡ����ť
	void  ShowAutoBtnStation(bool bShow , bool bEnable); 
	///��ʾׯ��ͼ��
	void  ShowNtFlat(bool bShow) ; 
	///��ʾ��Ϸ�еı���
	void  ShowGameMutiple(bool bShow) ;
	///��ʾ������Ϣ
    void  ShowGameMutipleContent(bool  bShow) ; 
	///��ʾ�й�ͼ��
	void  ShowUserAutoFlag(bool bShow) ; 
	///��ʾ��ҽз�״̬
	void  ShowUserCallScoreBtn(int iCallValue , bool bShow) ;
	///��ʾ��ҽзֽ����ʶ
	void  ShowUserCallScoreFlag(bool bShow) ; 
	///��ʾ�����������ť
	void  ShowUserRobNtBtn(bool bCall , bool bShow) ; 
	///��ʾ�����������ʶ
	void  ShowUserRobNtFlag(bool bShow) ; 
	///��ʾ��Ҽӱ�
	void  ShowUserDoubleBtn(bool bShow ,bool bEnable  = true) ; 
	///��ʾ��Ҽӱ���ʶ
	void  ShowUserDoubleFlag(bool bShow) ;
	///��ʾ������ư�ť
	void  ShowUserShowCardBtn(bool bShow) ; 
	///��ʾ������Ʊ�ʶ
	void  ShowUserShowCardFlag(bool bShow) ;  
	///��ʾ��ҳ��ƽ���İ�ť
	void  ShowOutCardStationBtn(bool bEnaleOutCard ,bool bShowPass ,bool bShow) ; 
	///��ʾ��Ϸ�����
	void  ShowGameResult(bool bShow) ; 
	///��ʾ��Ϸ�е�����
	void  ShowGameTask(bool  bShow) ; 
	///��ʾ�������ͼƬ
	void  ShowGameTaskFinish(bool bShow) ; 
	///�û��з�
	int   OnUserCallScore(int iCallValue); 
	///��ҵ���������Ȳ���
	int   OnRobNtButton(int iRobvalue) ; 
	///��ҵ���ӱ��Ͳ��ӱ�
	int   OnAddDoubleButton(int iAddValue) ; 
	///��ҵ�����ƺͲ���
	int   OnShowCardButton(int iShowValue) ; 
	///�û�������ز���
	int   OnUserOutCardOperate(int  iControlID);	
	///�û�����˿�
	int   OnCheckOutCard(void) ; 
	///��ҵ���й�
	int   OnAutoButton(void) ; 
	///��ҵ������
	int   OnSortButton(void) ; 
	////��Ϸ��Ϣ����

	///�������񶯻�
	void  OnPlayTaskAni(bool bPlay ,bool bShow) ; 
    ///���ű�������
	void  OnPlayWarningAni(BYTE bDeskStation , bool bPlay ,BYTE bType , bool bShow) ;
	///���ű������Ӷ���
	void  OnPlayGameAni(int iAniID , bool bPlay , bool bShow ,bool bIsloop = false) ; 
    ///��ʾ������ũ����Ӯ����
	void  OnPlayWinLostAni(bool bWin , bool bPlay , bool bShow) ; 
	///������Ϸ��ʼ��Ϣ
	void  OnHandleGameBegin(BeginUpgradeStruct *pBeginData); 
	///������Ϸ������Ϣ
	void  OnHandleSendCardMsg(SendAllStruct *pSendCardData) ; 
	///������Ϸ���ƽ�����Ϣ
	void  OnHandleSendCardFinish(void) ;
	///����з���Ϣ
	void  OnHandleCallScoreMsg(CallScoreStruct *pCallScoreData); 
	///����зֽ��
	void  OnHandleCallScoreResult(CallScoreStruct *pCallScoreData); 
	///����зֽ���
	void  OnHandleCallScoreFinish(CallScoreStruct *pCallScoreData) ; 
	///������������Ϣ
	void  OnHandleRobNtMsg(RobNTStruct *pRobNtData) ; 
	///���������������Ϣ
	void  OnHandleRobNtResult(RobNTStruct *pRobNtData);
	///����������������Ϣ
	void  OnHandleRobNtFinish(RobNTStruct *pRobNtData) ; 
	///����������Ϣ
	void  OnHandleSendBackCard(BackCardExStruct *pBackCard) ; 
	///����ӱ���Ϣ
	void  OnHandleAddDoubleMsg(AddDoubleStruct *pAddDoubleData) ;
	///����ӱ������Ϣ
	void  OnHandleAddDoubleResult(AddDoubleStruct *pAddDoubleData) ; 
	///����ӱ�������Ϣ
	void  OnHandleAddDoubleFinish(void) ;
	///����������Ϣ
	void  OnHandleShowCardMsg(ShowCardStruct *pShowCardData) ; 
	///�������ƽ����Ϣ
	void  OnHandleShowCardResult(ShowCardStruct *pShowCardData) ;
	///�������ƽ�����Ϣ
	void  OnHandleShowCardFinish(void) ; 
	///������ҿ�ʼ������Ϣ
	void  OnHandleBeginPlayGame(BeginPlayStruct *pBeginPlayData) ;   
	///֪ͨ��ҳ�����Ϣ
	void  OnHandleNotifyUserOutCard(OutCardMsg *pOutCardData) ;
	///��ҳ��ƽ����Ϣ
	void  OnHandleUserOutCardResult(OutCardMsg *pOutCardResult);
	///һ�ֽ���
	void  OnHandleOneTurnOver(void) ; 
	///��һ�ֳ���
	void  OnHandleNewTurnBegin(NewTurnStruct *pNewTurnData);  
	///��Ϸ����
	void  OnHandleGameFinish(GameEndStruct *pGameEndData) ; 
	///���ǿ���˳�
	void  OnHandGameCutEnd(GameCutStruct *pCutEnd) ; 
 
	///�û���ʱ�����йܲ���
	bool  OnAutoOutCard(void) ; 
    ///��ȡ�����������
	void  GetTaskName(CString &strName) ; 

	///���������Ϸ��Ϣ
	int   UserSetGameInfo(bool bSoundEnable,bool bSoundBgEnable ,bool bWatchEnable,bool bShow) ;
	///��Ϸ�е���Ч
	void  SetPlaySound(BYTE iDeskStation,int SoundID, bool bHaveSex , bool bISLoop) ;
	///������������
	void  SetOutCardPlaySound(BYTE iDeskStation,int SoundID,BYTE bCardType , bool bISLoop) ;
	///��Ϸ�еı�����Ч
	void  SetGameBgSound(int SoundID, bool bISLoop) ;


};