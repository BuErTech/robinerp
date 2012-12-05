#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "ClientView.h"
#include "GameSet.h"
//#include "PlaySound.h"
#include "LoveSendClassInExe.h"

/// ��Ϣ����
#define IDM_BEGIN			WM_USER+120					///< ��ʼ��ť��Ϣ
#define IDM_USER_NT			WM_USER+121					///< �û�����
//#define IDM_GIVE_BACK		WM_USER+123					///< �����ư�ť

#define IDM_OUT_CARD_PASS		WM_USER+123				///< �û�����
#define IDM_OUT_CARD		WM_USER+124					///< �û�����
#define IDM_LOOK_LAST		WM_USER+125					///< �������˿�
///#define IDM_BACK_CARD		WM_USER+126				///< ������Ϣ
#define IDM_HAVE_THING		WM_USER+127					///< ����Ҫ��
#define IDM_SEND_THING		WM_USER+128					///< �����뿪����
#define IDM_ARGEE_LEAVE		WM_USER+129					///< ͬ���뿪��Ϣ
#define IDM_STOP			WM_USER+130					///< ��ǰ������ť
#define IDM_STOP_THING		WM_USER+131					///< ������ǰ����
#define IDM_ARGEE_STOP		WM_USER+132					///< ͬ����ǰ����
#define IDM_SET_FLASH_CHESS WM_USER+122					///< ����������˸

/// ��ʱ�� ID
#define ID_BEGIN_TIME				100					///< ��ʼ��ʱ��
///#define ID_WAIT_ROB					101				///< �ȴ�������
///#define ID_GIVE_BACK				102					///< �ȴ�������
#define ID_OUT_CARD					103					///< ���Ƶȴ�ʱ��
#define ID_SHOW_LAST				104					///< �鿴�����˿�
///#define ID_SHOW_CARD				105					///< ˦�ƶ�ʱ��
///#define ID_LOOK_BACK				106					///< �鿴����
#define ID_SHOW_PATH				107					///< ��ʾ�켣
#define ID_FLASH_CHESS				108					///< ��˸����

/// ���ú�������
#define RS_ALL_VIEW					1					///< �������в���
#define RS_GAME_CUT					2					///< �û��뿪����
#define RS_GAME_END					3					///< ��Ϸ��������
#define RS_GAME_BEGIN				4					///< ��Ϸ��ʼ����

/// ��Ϸ�����    AFC::�����::IDD_GAME_FRAME 
class CClientGameDlg : public CLoveSendClassInExe//CGameFrameDlg
{	
bool H041_UserLose(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);
	bool H0011_UserBuZhen(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);
	bool H085_DuiJiaHaveThingResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);
	bool H084_DuiJiaHaveThing(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);
	bool H083_UserHaveThingResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);
	bool H082_UserHaveThing(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);
	bool H081_UserContinueEnd(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);
	bool H080_UserLeftOrGameAnQuanFinish(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);
	bool H040_UserOutCard(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);

	bool H030_NextStep(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);
	bool H010_GameBegin(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);
	bool H000_UserSetGameJu(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize/*, CTCPClientSocket * pClientSocket*/);

protected:
	/// �û�����
	virtual bool GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);
	
public:
	bool  IsWatcher( ){return m_bWatchMode;}
	/// �̶���Ϣ
	BYTE					m_iLessCount;				///< ������Ϸ����
	BYTE						m_bTimeOutCount;		///< ��ʱ

	/// �ؼ�����
public:
	CClientPlayView			m_PlayView;					///< ��Ϸ��ͼ
	CGameSet				* m_GameSet;
	_PathMsg				m_MResultPath;				///< �켣����

	int						m_iVersion;
	int						m_iVersion2;
	int						uRunPublish;				///< �۷�
	int						basepoint;					///< ����
	BYTE					m_iAfterWaitAction;  		///< ��ʼ����ʱ������Ķ��� 1 �Զ���ʼ 2 �Զ��뿪
	/// ��������
public:
	/// ���캯��
	CClientGameDlg();
	/// ��������
	virtual ~CClientGameDlg();

protected:
	/// ��ʼ������
	virtual BOOL OnInitDialog();
	/// ���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	virtual void OnWatchSetChange(void);
	virtual void OnGameSetting();	
	bool	AFCCloseFrame();

	CPlaySound				m_playsound;
	CPlaySound				m_playsound2;

	/// ���غ���
public:
	int GetPlayerCount();
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
	/// view
	virtual BYTE ViewStation(BYTE bDeskStation);
	/// ��Ϣ����
public:
	/// ��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);
	/// �����˿˰�ť����
	/// /LRESULT	OnHitCard(WPARAM wparam, LPARAM lparam);

	/// �������ְ�ť����
	/// LRESULT	OnHitLastTurn(WPARAM wparam, LPARAM lparam);
	/// �������°�ť����
	LRESULT	OnHitHaveThing(WPARAM wparam, LPARAM lparam);
	/// �����뿪������
	LRESULT	OnSendHaveThing(WPARAM wparam, LPARAM lparam);
	/// �û������뿪
	LRESULT	OnArgeeUserLeft(WPARAM wparam, LPARAM lparam);
	/// �Ҽ�����
	/// /LRESULT	OnRightHitCard(WPARAM wparam, LPARAM lparam);
	/// ��ǰ������ť
	LRESULT	OnStop(WPARAM wparam, LPARAM lparam);
	/// ������ǰ����
	LRESULT	OnStopThing(WPARAM wparam, LPARAM lparam);
	/// ͬ����ǰ����
	LRESULT	OnAgreeStop(WPARAM wparam, LPARAM lparam);
	/// ������˸����
	LRESULT	OnFlashChess(WPARAM wparam, LPARAM lparam);

	void ResetGameStation(int iGameStation);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
protected:
	virtual void OnCancel();
};

extern int  GetStation( int viewStation);
extern CClientGameDlg * g_pCClientGameDlg;
extern  bool  IsWatcher( );
extern int GetStationParameter();