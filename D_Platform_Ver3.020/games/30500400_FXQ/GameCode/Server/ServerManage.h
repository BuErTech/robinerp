#pragma once

#include "GameDesk.h"
#include "AsaveAndReadfile.h"
#include "..\GameMessage\UpGradeLogic.h"

// ��ʱ�����ýṹ
struct TimerStruct 
{
	UINT nId;			//��ʱ����־��
	int nTickCount;		//�Ѿ������˴�
	int nTriggerCount;	// �������ٴκ󴥷��¼�
	TimerStruct(UINT nTimerID, int uElapse) : nId(nTimerID),nTickCount(0), nTriggerCount(uElapse)
	{
	}
	TimerStruct(): nId(0),nTickCount(0), nTriggerCount(0){}
};
//��ʱ�� ID
#define TIME_MY_TIMER				20				//���ö�ʱ����ֻ�������ʱ����ʵ�ʶ�ʱ�������඼�Ǵ������ʱ�������ж�����
#define MY_ELAPSE					500				// ��ʱ�����
#define TIMER_COUNT					10		// �õ��Ķ�ʱ����Ŀ

// ����Ϊ��Ϸ��ʹ�õ��Ķ�ʱ�����������TIME_MY_TIMER��������ͳһ����
#define TIME_SEND_CARD				30				//���ƶ�ʱ��
#define TIME_APPLY_DICE             31             ///��������ɫ��
#define TIME_PLAYER_DICE            32             ///��Ҵ�ɫ��
#define TIME_PLANE_FLY              33             ///�ɻ���ɶ�ʱ��
#define TIME_PLANE_FLY_INTEVAL      34             ///�ɻ����ʱ����


//��Ϸ������־����
#define GF_NORMAL					10				//��Ϸ��������
#define GF_SAFE						11				//��Ϸ��ȫ����
#define GF_TERMINATE				14				//����ֹͣ��Ϸ
#define GF_AHEAD_END				15				//��ǰ����


///��Ϸ����
class CServerGameDesk : public CGameDesk
{
private:
	///��ʱ������������
	TimerStruct		       m_arTimer[TIMER_COUNT];
	void                   KillMyTimer(UINT nTimeID);
	int                    SetMyTimer(UINT nTimeID, int uElapse);
	bool                   OnMyTimer(UINT uTimerID);
	void                   KillAllMyTimer() ; 
	int				       m_iTimerCount;	/// ��ʱ������

	//��̬����
private:

	static                 BOOL m_bTurnRule;	       ///��Ϸ˳��	
	static                 BOOL m_bIskinescope;        /// �Ƿ�¼��Ĭ�ϲ�¼
	//=============��չ
	BYTE                   m_iBeginTime;               ///��Ϸ��ʼʱ��
	BYTE                   m_iDiceTime ;               ///��ɫ��ʱ��
	BYTE                   m_iThinkTime;               ///��Ϸ˼��ʱ��
	int                    m_iGameBasePoint ;          ///������ע

protected:
	///��Ϸ�߼�
	CUpGradeGameLogic		m_Logic;
	///¼��洢
	CAsaveAndReadfile       m_saveFile;
	///���ٴ�����پ�
	UINT	                m_iLimitPlayGame;	
	///�Ѿ���Ϸ�ľ���
	BYTE					m_iBeenPlayGame;
 
public:

	BYTE                    m_cbOperater ;  ///��ǰ��������    
	BYTE                    m_cbDiceValue ;  ///��ǰɫ�ӵĵ���
	BYTE                    m_cbWinPlayer ;  //ʤ�������


	BYTE                    m_cbPropDiceValue ;  ///���Ҫ����ɫ��

	BYTE                    m_cbPropDiceCount[PLAY_COUNT] ; 

	bool                    m_bPlayerReady[PLAY_COUNT] ; 

	///�й�����
	bool					m_bAuto[PLAY_COUNT];	

	bool                    m_bUserNetCut[PLAY_COUNT] ; 

	bool                    m_bDiceAniFinish[PLAY_COUNT] ; 


	PlayerActiveStruct      m_ActiveData ;  ///�������Ҫ��������У�Կͻ��˵ķɻ�����

	bool                    m_bFlying  ;       ///�ɻ����ڷ��й�����

	bool                    m_bUserProp ;    //�Ƿ�ʹ�õ���

	//��������
public:
	//���캯��
	CServerGameDesk(); 
	//��������
	virtual ~CServerGameDesk();
	//���غ���
public:
	//��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	//��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	//�ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);
	//��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	virtual bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	//�û�ͬ�⿪ʼ
	virtual bool UserAgreeGame(BYTE bDeskStation);
	//��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	//������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	//��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	//����INI�ļ�
	virtual bool InitDeskGameStation();	
	//��Ҷ��ߴ���
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
	//�û���������
	virtual bool UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo);
	//�û�������Ϸ��
	virtual BYTE UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo);

public:
	///���ؾ�̬�������
	static BOOL LoadStaticIni();
    ///��ʼ����ʼ����
	bool IniBeginData() ; 
	///����һ���������
	BOOL LoadExtIni();
	//���ݷ���ID���ص�ע������
	BOOL LoadExtIni(int iRoomID);
	//��Ϸ��ʼ
	BOOL BeginPlay();
	//������Ӌ�r��
	void KillAllTimer();
	//���ܿ۷�
	int  GetRunPublish();
	//���ܿ۷�
	int  GetRunPublish(BYTE bDeskStation);
	//��ǿ����ҵ÷�
	int  GetRunAwayOtherGetPoint(BYTE bDeskStation,BYTE bRunDeskStation);
	//��ȡ��һ�����λ��
	BYTE GetNextDeskStation(BYTE bDeskStation);
	///���ɻ����״��
	bool CheckPlaneCanFly(BYTE bDeskStation) ;  
	///֪ͨ�ͻ��˸��´�ɫ�ӽ���
	bool OnNotifyClentApplyDice(BYTE bDeskStation ) ;  
	///����ͻ��˷�������Ϣ
	bool OnCSNotifyDiceMessage(BYTE bDeskStation) ;
	///����ͻ��˷��������ѡ����ɻ������Ϣ
	bool  OnCSNotifyFlyMessage(BYTE bDeskStation ,int inDex) ; 
	///����ͻ��˷����ĵ�����Ϣ
	bool  OnCSPropDiceMessage(BYTE  bDeskStation , BYTE cbDicevalue) ; 
	///����ͻ��˷�������Ҷ����������
	bool  OnUserAniFinish(BYTE bDeskStation ) ; 
	///�û������й�
	bool  UserSetAuto(BYTE bDeskStation, bool bAutoCard) ;

public:

	///�������ݺ��� ��������Ϣ����Ϸ�ߣ����Ҵ���
	bool SendGameDataSaveFile(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode,bool bSingle = true );
	/// �������ݺ��� ��������Ϣ����Ϸ�ߣ����Ҵ���
	bool SendGameDataSaveFile(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode,bool bSingle = true);
	///��־��¼�ӿ�
	void  SrvTestlog(TCHAR *pMessage);	
};

/******************************************************************************************************/
