#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "ClientView.h"
#include "UpGradeLogic.h"
#include "GameSet.h"
#include "LoveSendClassInExe.h"

#ifdef _USE_VIDEO
#include "BRGameVideoSDK.h"
/**
* ��Ϸ��Ƶ�ṹ
*/
struct MyGameVideo
{
	MyGameVideo()
		: VideoServerPort(0)
	{
	}
	MyGameVideo(const char *serverip,int port)
		: VideoServerPort(port)
	{
		if(serverip) strcpy(VideoServerIP,serverip);
	}
	char VideoServerIP[256];                 // ��Ƶ������IP
	int VideoServerPort;                     // ��Ƶ�������˿�
};
#endif


/// ��Ϣ����
#define IDM_AGREE_INFO		WM_USER+118					///< ���ͬ��Լ����õ���Ϣ
#define IDM_DAOSHI_MONEY	WM_USER+119					///< ���ͬ��۳���ʦ����Ϣ
#define IDM_BEGIN			WM_USER+120					///< ��ʼ��ť��Ϣ
#define IDM_USER_NT			WM_USER+121					///< �û�����
#define IDM_GIVE_BACK		WM_USER+123					///< �����ư�ť
#define IDM_OUT_CARD		WM_USER+124					///< �û�����
#define IDM_LOOK_LAST		WM_USER+125					///< �������˿�
#define IDM_BACK_CARD		WM_USER+126					///< ������Ϣ
#define IDM_HAVE_THING		WM_USER+127					///< ����Ҫ��
#define IDM_SEND_THING		WM_USER+128					///< �����뿪����
#define IDM_ARGEE_LEAVE		WM_USER+129					///< ͬ���뿪��Ϣ
#define IDM_STOP			WM_USER+130					///< ��ǰ������ť
#define IDM_STOP_THING		WM_USER+131					///< ������ǰ����
#define IDM_ARGEE_STOP		WM_USER+132					///< ͬ����ǰ����
#define IDM_ADMIT_DEFEAT	WM_USER+133					///< ����
#define IDM_REGRET_CHESS	WM_USER+134					///< ����
#define IDM_CHESS_MANUAL	WM_USER+135					///< ����
#define IDM_SEND_REN_SHU	WM_USER+136					///< ��������
#define IDM_AGREE_REGRET	WM_USER+137					///< ͬ�����

/// ��ʱ�� ID
#define ID_BEGIN_TIME				100					///< ��ʼ��ʱ��
#define ID_WAIT_ROB					101					///< �ȴ�������
#define ID_GIVE_BACK				102					///< �ȴ�������
#define ID_OUT_CARD					103					///< ���Ƶȴ�ʱ��
#define ID_SHOW_LAST				104					///< �鿴�����˿�
#define ID_SHOW_CARD				105					///< ˦�ƶ�ʱ��
#define ID_LOOK_BACK				106					///< �鿴����

#define ID_CANT_MONEY				199					///< ����Ǯ���㵼ʦ��
#define ID_DAOSHI_QUIT				200					///< ����Ǯ���㵼ʦ��

/// ���ú�������
#define RS_ALL_VIEW					1					///< �������в���
#define RS_GAME_CUT					2					///< �û��뿪����
#define RS_GAME_END					3					///< ��Ϸ��������
#define RS_GAME_BEGIN				4					///< ��Ϸ��ʼ����

/// ��������������
#define SOUND_THREE					0x01				///< ����
#define SOUND_FOUR					0x02				///< ����
#define SOUND_WIN					0x03				///< ʤ��
#define SOUND_ERROR					0x04				///< ����
#define SOUND_GOOD_BYE				0x05				///< �ټ�
#define SOUND_GAME_BEGIN			0x06				///< ����Ϸ
#define SOUND_PUT_STONE				0x07				///< ����
#define SOUND_UNDO					0x08				///< ����
#define	SOUND_WELCOME				0x09				///< ��ӭ
#define SOUND_GAME_END				0x10				///< ��Ϸ����
#define SOUND_WARNING				0x11				///< ����

/// ��ͬ���ķ����ظ����������������ô������Զ�����ˣ��,��Ϊ5��
#define GAMER_SHUALAI_NUM           5

/// ��Ϸ����� 
class CClientGameDlg : public CLoveSendClassInExe//CGameFrameDlg
{
	/// ��������
public:
	/// ��¼�Է����ˣ���ṹ��Ϣ
	GamerShuaLaiJiang       m_gamerShua;     
	int                     m_iGamerShuaLai;            ///< �Է���ͬ���ķ������ظ�����

	/// �̶���Ϣ
	BYTE                    m_iAfterWaitAction;			///< Colin090311��ʼ����ʱ������Ķ��� 1 �Զ���ʼ 2 �Զ��뿪 

	BYTE					m_iLessCount;				///< ������Ϸ����
	BYTE					m_iBeginStation;			///< ��ʼλ��
	BYTE					m_iEndStation;				///< ����λ��
	BYTE					m_iCardCount;				///< ������Ϸ���˿���Ŀ
	BYTE					m_iPalyCardCount;			///< ��Ϸ�˿���Ŀ
	int						m_iRoomBasePoint;			///< ���䱶��
	int						m_iRunPublish;				///< ���ܿ۷�
	int                     m_BgePeaceCount;            ///< ��ʹ���
	/// �Լ��˿�����
	BYTE					m_iMeCardCount;				///< �Լ��˿˵���Ŀ
	BYTE					m_iMeCardList[45];			///< �Լ��˿��б�

	/// ������Ϣ
	BYTE					m_iNtCount;					///< �������˿���Ŀ
	BYTE					m_iNtPeopole;				///< ��������Ϸ��
	BYTE					m_iBackCard[8];				///< �����˿�

	/// ������Ϣ
	BYTE					m_iDeskCardCount[4];		///< ÿ�������˿˵���Ŀ
	BYTE					m_DeskCard[4][39];			///< ÿ��������˿�

	/// ������Ϣ
	BYTE					m_iNowOutPeople;			///< ���ڳ�����
	BYTE					m_iBaseCount;				///< ��һ������������Ŀ
	BYTE					m_iFirstOutPeople;			///< ��һ��������
	BYTE					m_iBaseCard[39];			///< ��һ�������ߵ��˿�

	/// ������ʾ����
	BYTE					m_iLastCardCount;			///< �����˿˵���Ŀ
	BYTE					m_bShowLast;				///< �Ƿ������ʾ�����˿�
	BYTE					m_bShowIngLast;				///< �Ƿ�����ʾ�����˿�
	BYTE					m_iLastOutCard[4][39];		///< ���ֵ��˿�

	/// ��ʱ������
	//int						m_iTimerStation;		///< ��ʱ��λ��
	//int						m_iTimerCount;			///< ������ʣʱ��
	BYTE						m_bTimeOutCount;		///< ��ʱ
	int							m_nJiaoZu;

	int							m_iStartI,				///< �������
								m_iStartJ,
								m_iEndI,				///< �����յ�
								m_iEndJ;

	int							m_iMeBoard[10][9];		///< �������
	int							m_iMeChessFlag;			///< ������־
	int							m_iCurPlayFlag;			///< ��ǰ������

	int							m_iJuTimeMinute,		///< ��ʱ��
								m_iJuTimeSecond,		///< ��ʱ��
								m_iBuTimeMinute,		///< ��ʱ��
								m_iBuTimeSecond,		///< ��ʱ��
								m_iDuMiaoMinute,		///< �����
								m_iDuMiaoSecond,		///< ������
								m_iGameMoney;			///< ָ����
	bool						m_havething;
	TCHAR						m_tPlayName[PLAY_COUNT][62];///< �������
	bool                        m_bNoHaveAgreeRegret;  //�Ƿ�����ͬ������ͬ�����

	BYTE						m_daoshi;
	/// �ؼ�����
public:
	CUpGradeGameLogic		m_Logic;					///< �����߼�
	CClientPlayView			m_PlayView;					///< ��Ϸ��ͼ
	CGameSet				*m_GameSet;
	int						m_iVersion;
	int						m_iVersion2;

#ifdef _USE_VIDEO
	CBRGameVideoSDK         *m_pBRGameVideoSDK;             /// ��Ƶ�ؼ�
	MyGameVideo             m_GameVideoSet;                 /// ��Ϸ��Ƶ����
	std::map<std::string,int>   m_VideoUserInfo;             /// 3����Ƶ�û�
#endif

	/// ��������
public:
	/// ���캯��
	CClientGameDlg();
	/// ��������
	virtual ~CClientGameDlg();
    int GetGameStatus();
protected:
	/// ��ʼ������
	virtual BOOL OnInitDialog();
	/// ���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	virtual void OnWatchSetChange(void);
	virtual void OnGameSetting();
	bool	AFCCloseFrame(bool bNotifyServer = true);

	virtual void FixControlStation(int iWidth, int iHeight);

	/// ���غ���
public:
	/// ������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize);
	/// ��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);
	/// �������ý���
	virtual void ResetGameFrame();
	/// ��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount);
	/// ������ж�ʱ��
	void KillAllTimer();
	/// ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin();
	/// ��ȫ������Ϸ
	virtual bool OnControlSafeFinish(){return false;};
	/// �û��뿪
	virtual bool GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);
	/// �û�����
	virtual bool GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);

	/// ��Ϣ����
public:
	// ���ͬ��Լ����õ���Ϣ
	LRESULT	OnHitAgreeInfo(WPARAM wparam, LPARAM lparam);
	//���ͬ��Լ����õ���Ϣ
	LRESULT	CClientGameDlg::OnAgreeDaoShiMoney(WPARAM wparam, LPARAM lparam);
	/// ��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);
	/// NT ��ť����
	LRESULT	OnHitNt(WPARAM wparam, LPARAM lparam);
	/// �����˿˰�ť����
	LRESULT	OnHitCard(WPARAM wparam, LPARAM lparam);
	/// �����ư�ť����
	LRESULT	OnHitBackCard(WPARAM wparam, LPARAM lparam);
	/// �������ư�ť����
	LRESULT	OnHitOutCard(WPARAM wparam, LPARAM lparam);
	/// �������ְ�ť����
	LRESULT	OnHitLastTurn(WPARAM wparam, LPARAM lparam);
	/// ���������ư�ť����
	LRESULT	OnHitLookBack(WPARAM wparam, LPARAM lparam);
	/// �������°�ť����
	LRESULT	OnHitHaveThing(WPARAM wparam, LPARAM lparam);
	/// �����뿪������
	LRESULT	OnSendHaveThing(WPARAM wparam, LPARAM lparam);
	/// �û������뿪
	LRESULT	OnArgeeUserLeft(WPARAM wparam, LPARAM lparam);
	/// �Ҽ�����
	LRESULT	OnRightHitCard(WPARAM wparam, LPARAM lparam);
	/// ��ǰ������ť
	LRESULT	OnStop(WPARAM wparam, LPARAM lparam);
	/// ������ǰ����
	LRESULT	OnStopThing(WPARAM wparam, LPARAM lparam);
	/// ͬ����ǰ����
	LRESULT	OnAgreeStop(WPARAM wparam, LPARAM lparam);	
	/// ����
	LRESULT OnAdmitDefeat(WPARAM wparam, LPARAM lparam);
	/// ����
	LRESULT OnRegretChess(WPARAM wparam, LPARAM lparam);
	/// ����
	LRESULT OnChessManual(WPARAM wparam, LPARAM lparam);
	/// ��ʱ����
	LRESULT OnSuperTimer(WPARAM wparam, LPARAM lparam);
	/// ȷ������
	LRESULT OnSendAdmitDefeat(WPARAM wparam, LPARAM lparam);
	/// ͬ�����
	LRESULT OnAgreeRegret(WPARAM wparam, LPARAM lparam);
	BOOL UpdateNtList();
	void ResetGameStation(int iGameStation);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);

#ifdef _USE_VIDEO
	afx_msg LRESULT OnGVClientEnterRoom(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGVClientLogin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGVClientUserAtRoom(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGVClientOnlineUser(WPARAM wParam, LPARAM lParam);
	/// ˢ�����е���Ƶ���
public:
	void RefreshAllVideoPanel(void);
#endif

//	afx_msg void OnClose();
protected:
	virtual void OnCancel();
public:
	bool IsWatcher(void);
	/// ��������
	BOOL SetPlaySound(BYTE bDeskStation,BYTE bSoundType,BOOL bExtVal=false);
	/// ��������(�汾�˶�)
	BOOL CheckVersion(BYTE iVersion,BYTE iVersion2,TCHAR pUpDateHttp[100]);
	/// �û�����
	BOOL UserPutStone();
	/// λ��ת��(���ڰ���λ�û�����)
	int StationSwap(BYTE iViewStation,BYTE iMeChessFlag);
	/// λ������ת��
	int SwapIx(int iX);
	/// λ������ת��
	int SwapIy(int iY);
	/// ¼Ӱ�ӿں���lc090227
	virtual void SetRecordView();

};
