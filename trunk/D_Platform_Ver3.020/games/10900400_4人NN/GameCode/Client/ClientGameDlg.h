#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "ClientView.h"
#include "LoveSendClassInExe.h"
#include "..\GameMessage\UpGradeLogic.h"
#ifdef SUPER_VERSION
#include "platform/CardSet.h"

#endif
#include "ControlWin.h"
//#include "MidiSample.h"

#define MAST_PEOPLE			-1							//�Ƿ�Ϊ����Ա
//��Ϣ����
#define IDM_BEGIN			WM_USER+120					//��ʼ��ť��Ϣ
//#define IDM_USER_NT		WM_USER+121					//�û�����
//#define IDM_GIVE_BACK		WM_USER+123					//�����ư�ť
//#define IDM_OUT_CARD		WM_USER+124					//�û�����
//#define IDM_LOOK_LAST		WM_USER+125					//�������˿�
//#define IDM_BACK_CARD		WM_USER+126					//������Ϣ
#define IDM_HAVE_THING		WM_USER+127					//����Ҫ��
#define IDM_SEND_THING		WM_USER+128					//�����뿪����
#define IDM_ARGEE_LEAVE		WM_USER+129					//ͬ���뿪��Ϣ
#define IDM_STOP			WM_USER+130					//��ǰ������ť
#define IDM_STOP_THING		WM_USER+131					//������ǰ����
#define IDM_ARGEE_STOP		WM_USER+132					//ͬ����ǰ����

#define IDM_PASS			WM_USER+133					//������ť��Ϣ
#define IDM_CALL_SCORE		WM_USER+135					//�з�
#define IDM_AUTO			WM_USER+134					//�й���Ϣ
#define	IDM_ACTIVE_RESULT	WM_USER+136					//�û�������
#define IDM_SHOW_NOTE		WM_USER+137					//�û�ѡ����ע,��ע
#define IDM_SELECT_NOTE_NUM	WM_USER+138					//�û���ע
#define	IDM_SOUND			WM_USER+139					//�û���������
#define IDM_BGSOUND			WM_USER+140					//��������
//��ʱ�� ID
#define ID_BEGIN_TIME				100					//��ʼ��ʱ��
//#define ID_WAIT_ROB				101					//�ȴ�������
//#define ID_GIVE_BACK				102					//�ȴ�������
#define ID_OUT_CARD					103					//���Ƶȴ�ʱ��
#define ID_LOOK_BACK				105					//������
#define ID_CHANGE_MUSIC			    106					//����������
#define ID_NOTE                     110                 //�ȴ��м���ע
#define ID_CALL_SCORE_TIME			107					//�зֶ�ʱ��
//���ú�������
#define RS_ALL_VIEW					1					//�������в���
#define RS_GAME_CUT					2					//�û��뿪����
#define RS_GAME_END					3					//��Ϸ��������
#define RS_GAME_BEGIN				4					//��Ϸ��ʼ����


//��Ϸ����� 
class CClientGameDlg : public CLoveSendClassInExe//CLoveSendClassInExe//CGameFrameDlg
{
	//��������
public:
	//�̶���Ϣ
	//=====================ʱ�����Ϣ===================
	BYTE					m_iThinkTime;				//������Ϸ����
	BYTE					m_iBeginTime;				//��Ϸ��ʼʱ��
	BYTE					m_iCallScoreTime;			//��Ϸ��ʼʱ��
	BYTE					m_iXiaZhuTime;				//��עʱ��
	int						m_iRoomBasePoint;			//���䱶��
	int						m_iRunPublish;				//���ܿ۷�
	int						m_iVersion;
	int						m_iVersion2;
	//	BYTE					m_iBeginStation;			//��ʼλ��
	//	BYTE					m_iEndStation;				//����λ��
	BYTE					m_iCardCount;				//������Ϸ���˿���Ŀ
	BYTE					m_iPalyCardCount;			//��Ϸ�˿���Ŀ
	__int64					m_iLimitNote[PLAY_COUNT];				//����޶�ע
	__int64					m_iBaseNote;				//��ע
	__int64					m_iThisTurnLimit;			//������ע
	//�Լ��˿�����
	//BYTE					m_iMeCardCount;				//�Լ��˿˵���Ŀ
	//BYTE					m_iMeCardList[45];			//�Լ��˿��б�
	//BYTE					m_iBackCard[8];				//�����˿�
	//������Ϣ
	BYTE					m_DeskCard[PLAY_COUNT][10];			//ÿ��������˿�
	BYTE					m_iDeskCardCount[PLAY_COUNT];		//ÿ�������˿˵���Ŀ
	BYTE					m_UserCard[PLAY_COUNT][10];			//�û��˿�
	BYTE                    m_iUpBullCard[PLAY_COUNT][3];//�����ţ��
	//������Ϣ
	BYTE				    	m_bCurrentOperationStation;		//�F�ڽз���
	BYTE						m_bTimeOutCount;			//��ʱ
	BYTE						m_bShowAnimalStation;		//������ʾλ��
	int					    	m_iNowOutPeople;			//���ڳ�����
	int						    m_iFirstOutPeople;			//��һ��������
	int					    	m_iNowBigNote;				//��ǰ���Ѻע��
	int						    m_iThisGameNote[PLAY_COUNT];			//������Ϸ��ע��(��ע)
	int						    m_iTotalGameNote[PLAY_COUNT];			//������Ϸ��ע��(ʵ��)
	int						    m_iUpGradePeople;					//ׯ��λ��
	int                         m_iNote;                    //��ǰ��Ч��ע����ǰλ�����Ч��ע��;
	BOOL						m_bBgSound;					//��������
	int                         m_iAfterWaitAction;

	bool                        m_bAutoBull;                ///�Ƿ��Զ���ţ
	BYTE                        m_iSendAnimaCardPos;     ///���ƶ���λ��
	int                         m_iSendCardTotle;         ///���Ƶ�����
	BOOL                        m_bIsSuper;               ///�Ƿ񳬶�
	CControlWin                 m_Control;
	BOOL                        m_bPlayer;                ///�Ƿ���ʵ��ң�2011-8-17 ��ƽ̨�޸Ĺ�������ҿ��Խ�����Ϸ������;�������ӵĲ�����ʵ���
	//�ؼ�����
public:
	CUpGradeGameLogic		m_Logic;					//�����߼�
	CClientPlayView			m_PlayView;					//��Ϸ��ͼ
	//��������
	HWND					m_hMciWnd1;						//��������(����)
	HWND					m_hMciWnd2;						//��������(����)



#ifdef SUPER_VERSION
	CCardSet	*m_SetCard;
#endif
	//��������
public:
	//���캯��
	CClientGameDlg();
	//��������
	virtual ~CClientGameDlg();

	int GetGameStatus()
	{
		if(this)
			return GetStationParameter();
		else
			return 0;
	};

protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	virtual void OnWatchSetChange(void);
	virtual void OnGameSetting();
	//���غ���
public:
	//������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize);
	//��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);
	//�������ý���
	virtual void ResetGameFrame();
	//��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount);
	//������ж�ʱ��
	void KillAllTimer();
	//ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin();
	//��ȫ������Ϸ
	virtual bool OnControlSafeFinish(){return false;};
	//�û��뿪
	virtual bool GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);
     // ƽ̨¼Ӱ����
	virtual void SetRecordView();

	BYTE GetNetUserPos(BYTE iDesk);

#ifdef VIDEO
	/// Ϊ��Ƶģ�����ش˺���
	virtual bool GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);
	virtual bool AFCCloseFrame();
#endif 

	//��Ϣ����
public:
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);

	//�����˿˰�ť����
	LRESULT	OnHitCard(WPARAM wparam, LPARAM lparam);
	//�����˿˰�ť����
	LRESULT	OnMouseMove(WPARAM wparam, LPARAM lparam);
	//�����˿˰�ť����
	LRESULT	OnMouseLeft(WPARAM wparam, LPARAM lparam);

	//�������ư�ť����
	//LRESULT	OnHitOutCard(WPARAM wparam, LPARAM lparam);
	//�������ְ�ť����
	//LRESULT	OnHitLastTurn(WPARAM wparam, LPARAM lparam);
	//���������ư�ť����
	//LRESULT	OnHitLookBack(WPARAM wparam, LPARAM lparam);
	//�������°�ť����
	LRESULT	OnHitHaveThing(WPARAM wparam, LPARAM lparam);
	//�����뿪������
	LRESULT	OnSendHaveThing(WPARAM wparam, LPARAM lparam);
	//�û������뿪
	LRESULT	OnArgeeUserLeft(WPARAM wparam, LPARAM lparam);
	//�Ҽ�����
	//LRESULT	OnRightHitCard(WPARAM wparam, LPARAM lparam);
	//��ǰ������ť
	LRESULT	OnStop(WPARAM wparam, LPARAM lparam);
	//������ǰ����
	LRESULT	OnStopThing(WPARAM wparam, LPARAM lparam);
	//ͬ����ǰ����
	LRESULT	OnAgreeStop(WPARAM wparam, LPARAM lparam);
	//������ť
	//LRESULT OnHitPass(WPARAM wparam, LPARAM lparam);
	//�û�������
	LRESULT OnHitActiveResult(WPARAM wparam, LPARAM lparam);
	//�й�
	LRESULT OnHitAuto(WPARAM wparam,LPARAM lparam);
	//��ʾ�û���ע����
	LRESULT OnShowNote(WPARAM wparam,LPARAM lparam);
	//�û���ע��Ϣ
	LRESULT OnSelectNote(WPARAM wparam,LPARAM lparam);	
	//NT ��ť����
	LRESULT	OnHitNt(WPARAM wparam, LPARAM lparam);
	//�з�
	LRESULT	OnCallScore(WPARAM wparam, LPARAM lparam);

	//���ƶ�����ɺ���ʾ������
	LRESULT SendCardShowCard(WPARAM wparam,LPARAM lparam);
	//������ע�б�
	BOOL UpdateNtList();

	void ResetGameStation(int iGameStation);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
	//	afx_msg void OnClose();
protected:
	virtual void OnCancel();
	virtual BYTE ViewStation(BYTE bDeskStation);	//zxj 2009-11-21 ��ע��
	//��ʾ��ע��ť,iAddNote��ʾ���������ע��.bTimes��ʾ��ע����
	//	BOOL ShowHandAddNote(int iNowBigNote=0,BYTE bTimes=1,BYTE iNowBigNoteStyle=0,BOOL bExtVal=false);
	//��������
	//BOOL SetPlaySound(BYTE bSoundStyle,BOOL bExtVal=false);
	//��������
	BOOL CheckVersion(BYTE iVersion,BYTE iVersion2);
	//�޸��û�Ѻע����
	BOOL ModifyNote(BYTE iViewStation,int iCurNote,BOOL bExtVal=false);
	//����Ѻע����
	//BOOL SetNoteSound(BYTE bNoteStyle,BOOL bIsBoy = true);
	//������������ע
	BOOL UserSupersedeNote(BYTE bDeskStation);
	////�������������O��
	//BOOL SetOutCardTypeSound(BYTE iCardList[],int iCardCount,BOOL IsBoy = true);
	////����mp3����
	//BOOL PlayOutCardType(TCHAR FileName[],HWND hmciwnd);
	////���볡������
	//BOOL SetSenceSound(BYTE iSenceSound = 0,BOOL IsBoy = TRUE,int iVal = -1);
public:
	//�����͑��˓Q����
	LRESULT ChangeBackCard();
	//�Q���ƽY��
	BOOL ChangeBackCardResult(BYTE bDeskStation,BYTE bSourceBackCard,BYTE bResultCard,BOOL bExtVal=false);
	//�Qϵ�y��
	BOOL ReplaceCard(BYTE iTotalCard[],int iSendCardPos,int iEndPos,BYTE bSourceCard,BYTE bResultCard);
public:
	
	//ȥ������ָʾ�Ʊ�ʾ
	void DelShowThis();
	//���ؽ�ׯ��ť
	BOOL HideButton();
	//�з�
	void ShowCallScoreBt(int CallScore,BOOL bExtVol=false);
};
