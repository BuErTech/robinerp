#include "IGameImpl.h"
#include "..\GameMessage\UpGradeLogic.h"
#include "PlaneMoveAni.h"

#include <irrKlang.h>

using namespace irrklang;


const  int  TIME_GAME_BEGIN    = 200 ;    ///��Ϸ��ʼ��ʱ��
const  int  TIME_PLAYER_DICE   = 201 ;   ///��Ҵ�����
const  int  TIME_DICE_FINISH   = 202 ;  ///��Ҵ�ɫ�Ӷ�������
const  int  TIME_PLAYER_CHOISE = 203 ; ///���ѡ��ɻ�
const  int  TIME_AIM_AT_PLANE  = 204 ; //ָ��ɻ��Ķ���
const  int  TIME_PLANE_FLY     = 205 ;  ///�ɻ���ɶ���
const  int  TIME_CURSOR_ANI    = 300 ;    //��ҹ����ת��ʱ��

const  int   MAX_ANI_PLANE     = 8 ;  ///���8�ܷɻ�



///��Ϸ�е���ЧID
const   int   PLANE_ARRIVE   = 0x10 ;  //�ɻ�����
const   int   BOMB_SOUND     = 0x11 ;  //�ɻ���ը
const   int   DICE_ROLL      = 0x12 ;  //ɫ�ӹ���
const   int   PLANE_FLY      = 0x13 ; //�ɻ�����
const   int   PLANE_TAKEOFF  = 0x14 ;  //�ɻ����
const   int   TIME_OUT       = 0x15 ; ///ʱ�䵽
const   int   WIND_SOUND     = 0x16 ;  //��
const   int   AIM_AT_PLANE   = 0x17 ; //��׼ 
const   int   ARRIVE_HOME    = 0x18 ; //�ɹ��ؼ�

const   int   SOUND_WIN      = 0x20 ; // ʤ��
const   int   SOUND_LOST     = 0x21 ; ///ʧ��

const   int   GAME_BG_SOUND  = 0x30 ; //��Ϸ������Ч


#define GetControlNode(A ,IControl , IControlID ) \
	IControl *A=  dynamic_cast<IControl *>(m_pUI->GetIControlbyID(IControlID)); \
	if (NULL != A) \
	A


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
	IGameFrame            *m_pGameFrame;
	///������Ϸָ��
	IUserInterface        *m_pUI;

	ISoundEngine           *m_SoundEngine;               ///��Ϸ��Ч

	ISoundEngine           *m_SoundEnginBg ;              ///������Ч
public: 


	CUpGradeGameLogic      m_ClientLogic ;                 ///�ͻ����߼���
	BYTE                   m_bGameStation ;                ///��ǰ����Ϸ״̬

	int                    m_iMyDeskStation ;              ///����Լ�����Ϸλ��
	BYTE                   m_cbOperater ;                 ///��ǰ��������
	BYTE                   m_cbCurrentStation ;             ///��ǰ����Ϸ״̬
	BYTE                   m_cbDiceFrame ;               ///��ǰ���ӹ���������һ֡
  
	///ʱ���б�
	BYTE					m_iBeginTime;		           ///��Ϸ��ʼʱ��
	BYTE                    m_iDiceTime ;                 ///��Ϸ�д�ɫ��ʱ��
	BYTE					m_iThinkTime;		           ///������Ϸ����

	PATH                    m_BasePath ;                  ///��Ϸ����·��

	CPoint                  m_ptBasePoint ;               ///��Ϸ��׼��

	CPlaneMoveAni           m_planeAni[8] ;               ///�ɻ�����(���˼ܷɻ�һ���ж���)

	CPlaneMoveAni           m_bombAni ;                   ///��׼����

	BYTE                    m_cbDiceValue ;                ///��ǰɫ�ӵĵ���

	GameScence              m_GameScene ;                  ///��Ϸ����

	BYTE                    m_iPropCount[PLAY_COUNT] ;    ///����ɽ趫������

	int                     m_iPlayerfight[2] ;  ///���ս��

	int                     m_iAniPlaneCount ;        ///�����ɻ��ĸ���

	bool                    m_bReady[PLAY_COUNT] ;   ///��ʾ���׼��ͼ��

	bool                    m_bAutoOutCard[PLAY_COUNT];              ///�Ƿ�Ϊ�йܳ��� ,ֻ�ڿͻ������й�   

	BYTE                    m_bAutoOutTime ;           ///��ҳ�ʱ����

	bool                    m_bSoundPlay ;  //�Ƿ���Բ�������

	bool                    m_bSoundBgPlay ;  //������Ч

	bool                    m_bAllowWatch ; // �Ƿ������Թ�

	CString                 m_strCoinName ;   ///��Ϸ�еĽ��

	bool                    m_bWatchStation;  ///�Ƿ����Թ�״̬


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
	///��ʼ������
	void  ResetScenceData(void) ; 
	///��ҽ�����Ϸ
	void  SetGameStation(void * pBuffer,int nLen);
	///ɱ�����ж�ʱ��
	void  KillAllGameTime(void) ; 
	///��ʾ�����ؿ�ʼ����ʱʱ��
	void  ShowOrHideUserTime(BYTE bDeskStation,int nShowMaxNum,int nShowMinNum,bool bFlag) ;

	///�������ж�ʱ��
	void  HideAllTime(void) ;
	///������Ϸ������ͼ����
	void  SetBaseImage(void) ; 
	///���Ƶ�ǰ�ķɻ����
	void  DrawPlayerPlane(BYTE bDeskStation , bool bShow) ; 
	///������׼
	void  DrawAimBombAni(bool bShow) ; 
	///����ը������
	void  DrawBmobAni(bool bPlay) ; 
	///���Ʒɻ�����
	void  DrawPlaneAni(void) ; 
    ///���õ�ǰ�������״
	void  SetCurrentCursor(int iSrcPos);
	///�������˷������Ŀ�ʼ��Ϣ
	bool  OnSCBeginMessage(BeginUpgradeStruct *pBeginData); 
	///�������˷������ĸ������Ҫ���ɫ�ӽ���
	bool  OnSCWaitDiceMessage(WaitDiceStruct *pWaiData) ;
	///�������˷������Ĵ�ɫ����Ϣ
	bool  OnSCDiceMessage(NotifyDiceStruct *pDiceDate) ; 
	///�������˷����Ĵ���ѡ��ɻ�����Ϣ
	bool  OnSCChoisePlaneMessage(NotifyChoisePlane *pChoiseData) ; 
	///�������˷�����
	bool  OnSCPlaneFlyMessage(PlayerActiveStruct *pActiveData) ; 
	///��ɷ�������ʵ���Ǻͷ����У�����ݣ�
	bool  OnSCPlaneFlyFinish(ScenceChangeStruct *pScenceChange) ; 
	///��Ϸ����
	bool  OnSCGameFinish(GameEndStruct *pGameEnd) ; 
    ///��Ϸǿ�н���
	bool  OnSCGameCutFinish(GameCutStruct *pCutEnd) ;
	///����ɫ�Ӷ���
	void  PlayDiceAni(BYTE bDeskStation , bool bPlay , bool  bShow) ; 
    ///�����������״̬
	void  PlayHihtLightAni(bool bPlay ,bool bShow) ;
	///��ʾ���׼��ͼ��
	void  ShowReadyLogo(bool bShow) ; 
	///��ʾ��ҵ���ͼ��
	void  ShowPlayerPropLogo(bool bshow) ; 
	///��ʾĿ�ĵ�ͼƬ
	void  ShowDestineImg(int  cbStep  ,BYTE inDex , bool bShow) ; 
	///��ʾ��ҵ�ս��
	void  ShowFightScore(bool bShow) ; 
	///��ʾ��Ϸ����
	void  ShowGameFinish(bool  bShow) ;


	///��ʾ�йܱ�ʶ
	void  ShowAutoOutCard(bool bShow) ;
   ///�����û��й�״̬
	void  SetAutoStation(BYTE bDeskStation,bool bAtuoOrCancel)  ;
	///�û��йܴ�ɫ��
	void  UserAutoDice(void) ; 
	///�û��й�ѡ������
	void  UserAutoChoisePlane(void) ;
	///���������Ϸ��Ϣ
	int   UserSetGameInfo(bool bSoundEnable,bool bSoundBgEnable ,bool bWatchEnable,bool bShow) ;
	///��Ϸ�е���Ч
	void  SetPlaySound(BYTE iDeskStation,int SoundID, bool bISLoop) ;
   ///��Ϸ�еı�����Ч
	void  SetGameBgSound(int SoundID, bool bISLoop) ;

		 
};