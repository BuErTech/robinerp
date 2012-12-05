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


//��Ϣ����
#define IDM_BEGIN					WM_USER+120					//��ʼ��ť��Ϣ
#define IDM_HAVE_THING				WM_USER+127					//����Ҫ��
#define IDM_SEND_THING				WM_USER+128					//�����뿪����
#define IDM_ARGEE_LEAVE				WM_USER+129					//ͬ���뿪��Ϣ
#define IDM_STOP					WM_USER+130					//��ǰ������ť
#define IDM_STOP_THING				WM_USER+131					//������ǰ����
#define IDM_ARGEE_STOP				WM_USER+132					//ͬ����ǰ����
#define IDM_ADMIT_DEFEAT			WM_USER+133					//����
#define IDM_REGRET_CHESS			WM_USER+134					//����
#define IDM_CHESS_MANUAL			WM_USER+135					//����
#define IDM_SEND_REN_SHU			WM_USER+136					//��������
#define IDM_AGREE_REGRET			WM_USER+137					//ͬ�����
#define IDM_FINISH_DIAN_MU			WM_USER+138
#define IDM_ASSURANCE_DIAN_MU		WM_USER+139					//ȷ����Ŀ
#define IDM_AGREE_DIAN_MU			WM_USER+140					//ͬ���Ŀ
#define	IDM_NUMBER_ZI				WM_USER+141					//ǿ������
#define IDM_ASSURANCE_NUM_ZI		WM_USER+142					//ȷ��ǿ������

#define IDM_GAMESETTING_OK				WM_USER+143					//ȷ��ǿ������

//��ʱ�� ID
#define ID_BEGIN_TIME				100							//��ʼ��ʱ��
#define ID_OUT_CARD					103							//���Ƶȴ�ʱ��

//���ú�������
#define RS_ALL_VIEW					1							//�������в���
#define RS_GAME_CUT					2							//�û��뿪����
#define RS_GAME_END					3							//��Ϸ��������
#define RS_GAME_BEGIN				4							//��Ϸ��ʼ����

//��Ϸ����� 
class CClientGameDlg : public CLoveSendClassInExe//CGameFrameDlg
{
	//��������
public:
	int							m_iVersion;							//�߰汾��
	int							m_iVersion2;						//�Ͱ汾��

	//��������	
	int							m_iRoomBasePoint;					//���䱶��
	int							m_iRunPublish;						//���ܿ۷�
	int							m_iRunPublishMoney;					//���ܿ۷�

	BYTE						m_iBeginTime;						//��ʼʱ��
	//������Ϣ
	BYTE						m_iNowOutPeople;					//���ڳ�����
	int							m_Qiflag[2];						//˫���ĺڰ�
	CString						m_finishMsg;						//��Ŀ��Ϣ
//	BYTE						m_bTimeOutCount;					//��ʱ
	//int							m_nJiaoZu;
	
	int							m_iStartI,							//�������
								m_iStartJ,
								m_iEndI,							//�����յ�
								m_iEndJ;


	int							m_iMeBoard[21][21];					//�������
	int							m_iMeChessFlag;						//������־
	int							m_iCurPlayFlag;						//��ǰ������

	int							m_iJuTimeMinute,					//��ʱ��
								m_iJuTimeSecond,					//��ʱ��
								m_iBuTimeMinute,					//��ʱ��
								m_iBuTimeSecond,					//��ʱ��
								m_iDuMiaoMinute,					//�����
								m_iDuMiaoSecond;					//������

	bool						m_bIsInDianZi;
	bool						m_bCanPlay;
	int							m_iDianMuQiZi[21][21];
	TCHAR						m_szMSG[MAX_PATH];					//��Ϣ�ַ�����
	bool                        m_bNoHaveAgreeRegret;                //�Ƿ�����ͬ�����᲻ͬ�����
	//�ؼ�����
public:
	CUpGradeGameLogic			m_Logic;						//�����߼�
	CClientPlayView				m_PlayView;						//��Ϸ��ͼ
	CGameSet					* m_GameSet;
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

	//��Ϣ����
public:
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);
	//�����˿˰�ť����
	LRESULT	OnHitCard(WPARAM wparam, LPARAM lparam);
	//�������°�ť����
	LRESULT	OnHitHaveThing(WPARAM wparam, LPARAM lparam);
	//�����뿪������
	LRESULT	OnSendHaveThing(WPARAM wparam, LPARAM lparam);
	//�û������뿪
	LRESULT	OnArgeeUserLeft(WPARAM wparam, LPARAM lparam);
	//�Ҽ�����
	LRESULT	OnRightHitCard(WPARAM wparam, LPARAM lparam);
	//��ǰ������ť
	LRESULT	OnStop(WPARAM wparam, LPARAM lparam);
	//������ǰ����
	LRESULT	OnStopThing(WPARAM wparam, LPARAM lparam);
	//ͬ����ǰ����
	LRESULT	OnAgreeStop(WPARAM wparam, LPARAM lparam);
	//����
	LRESULT OnAdmitDefeat(WPARAM wparam, LPARAM lparam);
	//����
	LRESULT OnRegretChess(WPARAM wparam, LPARAM lparam);
	//����
	LRESULT OnChessManual(WPARAM wparam, LPARAM lparam);
	//��ʱ����
	LRESULT OnSuperTimer(WPARAM wparam, LPARAM lparam);
	//ȷ������
	LRESULT OnSendAdmitDefeat(WPARAM wparam, LPARAM lparam);
	//ͬ�����
	LRESULT OnAgreeRegret(WPARAM wparam, LPARAM lparam);
	//��ɵ�Ŀ
	LRESULT OnFinishDianMu(WPARAM wparam, LPARAM lparam);

	//ͬ���Ŀ
	LRESULT OnAgreeDianMu(WPARAM wParam, LPARAM lParam);
	
	//ǿ������
	LRESULT	OnNumberZi(WPARAM wParam, LPARAM lParam);
	
	//ȷ��ǿ������
	LRESULT OnAssuranceNumZi(WPARAM wParam, LPARAM lParam);

	//����ƶ��������ϣ���Ŀ�ã�
	LRESULT OnMouseMoveCard(WPARAM wparam, LPARAM lParam);

	//ȷ����Ŀ
	LRESULT OnAssuranceDianMu(WPARAM wParam, LPARAM lParam);

	LRESULT OnGameSettingOK(WPARAM wParam, LPARAM lParam);


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

protected:
	virtual void OnCancel();
	virtual bool	AFCCloseFrame(bool bNotifyServer = true);
	virtual void FixControlStation(int iWidth, int iHeight);
public:
	//�汾�˶�
	BOOL CheckVersion(BYTE iVersion,BYTE iVersion2);
	//�Ƿ������;�뿪
	virtual BOOL CanLeftDesk(BYTE bDeskStation);
	//�û�����
	virtual bool GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);

};
