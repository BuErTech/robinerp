#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "ClientView.h"
#include "LoveSendClassInExe.h"
#include "../GameMessage/UpGradeLogic.h"
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

//��Ϣ����
#define IDM_BEGIN			WM_USER+120					//��ʼ��ť��Ϣ
#define IDM_OUT_CARD		WM_USER+124					//�û�����
#define IDM_HAVE_THING		WM_USER+127					//����Ҫ��
#define IDM_SEND_THING		WM_USER+128					//�����뿪����
#define IDM_ARGEE_LEAVE		WM_USER+129					//ͬ���뿪��Ϣ
#define IDM_STOP			WM_USER+130					//��ǰ������ť
#define IDM_STOP_THING		WM_USER+131					//������ǰ����
#define IDM_ARGEE_STOP		WM_USER+132					//ͬ����ǰ����
#define IDM_AUTOCARD		WM_USER+134					//�й�
#define IDM_CALL_SCORE		WM_USER+135					//�з�

#define IDM_SORT			WM_USER+137					//����
#define IDM_PASS			WM_USER+138					//��
#define IDM_CUE				WM_USER+139					//��ʾ

#define IDM_ADD_DOUBLE		WM_USER+140					//�Ӱ�
#define IDM_ROB_NT			WM_USER+141					//������
#define IDM_LOOK_LAST		WM_USER+142					//�������˿�
#define IDM_SHOW_CARD		WM_USER+143				    //����
#define IDM_COUNTER         WM_USER+144                 //������
#define IDM_DIALECT         WM_USER+145                 ///����
#define IDM_CHUIYES         WM_USER+146                 ///��
#define IDM_CHUINO          WM_USER+147                 ///����
//��ʱ�� ID
#define ID_BEGIN_TIME				100					//��ʼ��ʱ��
#define ID_LEAVE_TIME				101					//�뿪��ʱ��
#define ID_OUT_CARD					103					//���Ƶȴ�ʱ��
#define ID_CALL_SCORE_TIME			107					//�зֶ�ʱ��
#define ID_ROB_NT_TIME				108					//��������ʱ��
#define ID_ADD_DOUBLE_TIME			109					//�Ӱ�
#define ID_SHOW_LAST				110					//�鿴�����˿�
#define ID_SHOW_CARD				111					//����


#define ID_CONNECT_FLASH_TIME       112                 //������Ƶ������
#define ID_REMOVE_VIDEO_COLOR       113                  //ȥ����Ƶ��ɫ��ʱ��
#define ID_ANIM_TIME				114					//������ʱ��
#define TIME_CHUI					115					//��
//���ú�������
#define RS_ALL_VIEW					1					//�������в���
#define RS_GAME_CUT					2					//�û��뿪����
#define RS_GAME_END					3					//��Ϸ��������
#define RS_GAME_BEGIN				4					//��Ϸ��ʼ����

#define GAME_AUTO_TIME				1					//�Ԅӳ��ƕr�g�O��
#define GAME_RAND_TIME				2					//����ʱ��

//��Ϸ����� 
class CClientGameDlg : public CLoveSendClassInExe//CGameFrameDlg
{
	int						m_iVersion;					//�ڲ������汾��
	int						m_iVersion2;				//�ڲ������汾��
	BYTE					m_iPalyCard;				//��Ϸ�˿˸���
	int						m_iPlayCount;				//��Ϸ�˿�����
	__int64					m_iRoomBasePoint;			//���䱶��
	__int64					m_iRunPublish;				//���ܿ۷�
	BYTE					m_iAfterWaitAction;  	//��ʼ����ʱ������Ķ��� 1 �Զ���ʼ 2 �Զ��뿪 0900311a1 JJ

//	BYTE					m_iBeginNt;					//��ʼλ��
//	BYTE					m_iEndNt;					//����λ�� 
	//=====================ʱ�����Ϣ===================
	BYTE					m_iThinkTime;				//������Ϸ����
	BYTE					m_iGiveBackTime;			//����ʱ��
	BYTE					m_iBeginTime;				//��Ϸ��ʼʱ��
	BYTE					m_iCallScoreTime;				//��Ϸ��ʼʱ��
	BYTE					m_iAddDoubleTime;				//��Ϸ��ʼʱ��

	BYTE					m_iBeenPlayGame;				//����Ϸ����
	//�Լ��˿�����
	BYTE					m_iMeCardCount;				//�Լ��˿˵���Ŀ
	BYTE					m_iMeCardList[45];			//�Լ��˿��б�
	BYTE					m_Card[PLAY_COUNT][45];				//�������е��˿�����
	BYTE					m_CardCount[PLAY_COUNT];				//�������е�����
	bool                    m_bIsFirstOne[PLAY_COUNT];
	bool                    m_bIsFirstTow[PLAY_COUNT];

	//������Ϣ
	BYTE					m_iNtPeople;				//��������Ϸ��
	BYTE					m_iBackCard[12];				//�����˿�
	BYTE					m_iBackCount;				//������

	//������Ϣ
	BYTE					m_iDeskCardCount[PLAY_COUNT];		//ÿ�������˿˵���Ŀ
	BYTE					m_DeskCard[PLAY_COUNT][45];			//ÿ��������˿�

	//������Ϣ
	BYTE					m_bCurrentOperationStation;		//�F�ڽз���
	BYTE					m_iNowOutPeople;			//���ڳ�����
	BYTE					m_iBaseCount;				//��һ������������Ŀ
	BYTE					m_iFirstOutPeople;			//��һ��������
	BYTE					m_iBigOutPeople;			//��ǰ��������

	BYTE					m_iBaseCard[45];			//��һ�������ߵ��˿�

	//������ʾ����
	BYTE					m_bShowLast;				//�Ƿ������ʾ�����˿�
	BYTE					m_bShowIngLast;				//�Ƿ�����ʾ�����˿�
	BYTE					m_iLastCardCount[PLAY_COUNT];		//�����˿˵���Ŀ
	BYTE					m_iLastOutCard[PLAY_COUNT][45];		//���ֵ��˿�
	BYTE					m_byteHitPass;						//�Ƿ񲻳�

	BYTE					m_iAIStation[PLAY_COUNT];			//�������й�
	////��ʱ������
	BYTE						m_bTimeOutCount;		//��ʱ
	int							m_btAutoTime;
	BYTE						m_bSortCard;				//����ʽ

	void SetColor();
	CAFCColorResource			colorresource;		//����,ϵͳ��ɫ�ṹ
	void ShowCallScoreBt(int CallScore,BOOL bExtVol=false);
	//�ؼ�����
public:
	CUpGradeGameLogic		m_Logic;					//�����߼�
	CClientPlayView			m_PlayView;					//��Ϸ��ͼ

	//	bool					m_iToux;					//�Ƿ���������Ͷ��
//	bool					m_bisexit;
	bool					m_bAutoCard;				//�Ƿ��й�
	int						m_ResultCallScore;			//�зֽ��
//	int						m_TotalScore;				//�ܷ�

	HWND					m_hMciWnd1;						//��������(����)
	HWND					m_hMciWnd2;						//��������(����)
	HWND					m_hMciWnd3;						//��������(����)
	BOOL					m_bPlay;						//����

	TCHAR					m_CueMessage[MAX_PATH];			//��ʾ
	BOOL					m_bRightPower;					//˳ʱ�����

	BOOL                    bInitFinish;
	bool                    m_bPlayFlashVideo;                       //�Ƿ�������Ƶ

	bool                    m_bConnectFlash[PLAY_COUNT];
	char                    m_strFlash[PLAY_COUNT][256];	    //��¼��ҵ���Ƶ��Ϣ

	bool                    m_LastOneCard;               

    int                     m_iDialectType ;                 ///������ʽ

#ifdef _USE_VIDEO
	CBRGameVideoSDK         *m_pBRGameVideoSDK;             /// ��Ƶ�ؼ�
	MyGameVideo             m_GameVideoSet;                 /// ��Ϸ��Ƶ����
	std::map<std::string,int>   m_VideoUserInfo;             /// 3����Ƶ�û�
#endif

	//��������
public:
	//���캯��
	CClientGameDlg();
	//��������
	virtual ~CClientGameDlg();

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
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);
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
	//������Ϣ
	virtual void ShowUserTalk(MSG_GR_RS_NormalTalk *pNormalTalk);
	//ȡ����Ϸ״̬

	/// Ϊ��Ƶģ�����ش˺���
	virtual bool GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);
	
	virtual bool AFCCloseFrame();

		///����ֵת����String
	void GetNumString(__int64 nNum, CString &Str, int nPower);

    // PengJiLin, 2011-4-19, ����ʱ���������ͽ��ͼ����ʾ
    virtual void ShowGetMoneyImage();

	//��Ϣ����
public:
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);

	//�����˿˰�ť����
	LRESULT	OnHitCard(WPARAM wparam, LPARAM lparam);

	//�������ư�ť����
	LRESULT	OnHitOutCard(WPARAM wparam, LPARAM lparam);

	//�������°�ť����
	LRESULT	OnHitHaveThing(WPARAM wparam, LPARAM lparam);
	//�����뿪������
	LRESULT	OnSendHaveThing(WPARAM wparam, LPARAM lparam);
	//�û������뿪
	LRESULT	OnArgeeUserLeft(WPARAM wparam, LPARAM lparam);
	//�Ҽ�����
	LRESULT	OnRightHitCard(WPARAM wparam, LPARAM lparam);
	//�й�
	LRESULT	OnAuto(WPARAM wparam, LPARAM lparam);
		//�й�
	LRESULT	OnCounter(WPARAM wparam, LPARAM lparam);
	///����
	LRESULT	OnDialect(WPARAM wparam, LPARAM lparam);
	//�з�
	LRESULT	OnCallScore(WPARAM wparam, LPARAM lparam);
	//����
	LRESULT OnHitPassCard(WPARAM wparam,LPARAM lparam);
	//��ʾ
	LRESULT OnCue(WPARAM wparam,LPARAM lparam);
	//����
	LRESULT OnHitSortCard(WPARAM wparam,LPARAM lparam);
	//������
	LRESULT OnRobNT(WPARAM wparam,LPARAM lparam);
	//�Ӱ�
	LRESULT OnAddDouble(WPARAM wparam,LPARAM lparam);
	//�Ӱ�
	LRESULT OnShowCard(WPARAM wparam,LPARAM lparam);
	//��
	LRESULT OnChuiYes(WPARAM wparam,LPARAM lparam);
	//����
	LRESULT OnChuiNo(WPARAM wparam,LPARAM lparam);
	//����������Ϸ����
	virtual void ResetGameStation(int iGameStation);
	//¼Ӱ�ӿں���lc090226
	virtual void SetRecordView();

	virtual void FixControlStation(int iWidth, int iHeight);
	//ȡ����Ϸ״̬
	int	GetGameStatus()
	{
		if(this)
			return GetStationParameter();
		else
			return 0;
	};
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
	//��ͼת��
	virtual BYTE ViewStation(BYTE bDeskStation);
	//�汾�˶�
	BOOL CheckVersion(BYTE iVersion,BYTE iVersion2);
	//������ʾ�Ƶ�����
	void SetCueCardList(BYTE CueCardList[],BYTE iMeCardList[],int iMeCardCount,BYTE ResultCard[],int iResultCardCount);
	//���ű�������
	BOOL PlayBackgroundMusic(){return TRUE;};
	//�Ñ�����
	BOOL UserOutCard();
	//������ʾ
	BOOL SetNoteFlag(int iFlag = 0);
	//��Ҷ���,���ж��Ƿ�Ϊ��ǰ������
	virtual BOOL UserCut(BYTE bDeskStation);
	//������ҿ�ʼ
	BOOL ReplaceBegin(BYTE bDeskStation);
	//�����Ñ��з�
	BOOL ReplaceCallScore(BYTE bDeskStation);
	//�����Ñ�����
	BOOL ReplaceOutCard(BYTE bDeskStation);
	//����Ӱ�
	BOOL ReplaceAddDouble(BYTE bDeskStation);
	//��������
	BOOL ReplaceRobNT(BYTE bDeskStation);
	//�������ְ�ť����
	LRESULT	OnLastTurn(WPARAM wparam, LPARAM lparam);
	//���ذ�ť
	BOOL HideButton();

	//add by ljl
	//��������
	BOOL VoiceOut(BYTE bDeskStation, LPCSTR pszSound);
public:
	/// ������Ƶ 
	void PlayFlashVideo(BYTE bDeskStation,UserItemStruct * pUserItem,bool byMySelf);
    ///ֹͣ��Ƶ
	void StopFlashVideo(BYTE iView);
	///
	void CreateTrustTxt(void);
};
