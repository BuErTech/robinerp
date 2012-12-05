#pragma once
#ifndef AFC_GAMEROOMEX_HEAD_FILE
#define AFC_GAMEROOMEX_HEAD_FILE

#include "Stdafx.h"
#include "GamePlace.h"
#include "BaseRoom.h"
#include "ChatControl.h"
#include "UserListCtrl.h"
#include "ManageDlg.h"
#include "DeskFrameView.h"
#include "UserInfoView.h"
#include "GameUserManage.h"
#include "GamePlaceMessage.h"
#include "BitmapListCtrl.h"
#include "DialogCallGM.h"
#include "FindNullDesk.h"
#include "BankDlg.h"
#include "PBankWnd.h"
#include "PBankWndEx.h"
#include "BankVerifyWnd.h"
#include "BoardCast.h"
#include "PBankDlg.h"
#include "DlgRoomPW.h"
#include "UserDetailsDlg.h"
//#include "DlgDudu.h"

#include "VipTipDlg.h"

#include "GetHelpMoneyDlg.h"

#include "..\..\Record\include\interface.h"
#include "..\..\Record\include\struct.h"
#include "..\..\Record\PRStructExchange\AddRecordModule.h"

#include "AfCRoomRightWnd.h"
#include "LoveSendClassForEXE.h"	// By ZXD 20090316
#include "queueInRoomDlg.h"

#include "BzLog.h"

#define IDM_BIG_BROADCAST_MSG				WM_USER + 437				//��㲥
#define WM_BRING_GAMEWND_TO_TOP				WM_USER + 438				//������Ϸ����
#define WM_VIPROOM_MSG                      WM_USER + 440               ///< VIP������Ϣ
//�ṹ˵��
struct RoomInfoStruct;

extern CPtrArray m_ShortMessage;						//��Ϣ����

//�������ṹ
struct RecentlyTalkUser
{
	bool							bAccess;							//�Ƿ�ɷ�
	long int						dwUserID;							//�û� ID
	TCHAR							szUserName[61];						//�û�����
};

///
///CGameRoomEx ������Ϣ����
///
///@Modify yc.yan
///@version 1.0
///@date 2010.3.10
///

///������Ϣ����
class CGameRoomEx : public CBaseRoom , public ISortMessageUserInterface
{

	//��Ԫ����
	friend class CFindDesk;
	friend class CFindUserDlg;
	friend class CShortMessage;
	bool							m_bIsSitting;					///< �Ƿ���������
	bool							m_bInit;

	DWORD							dwRandWords;						//�������BCF��� 20081211
	static BYTE						m_deskIndex;						//���������ͽ��� 20081211
	static BYTE						m_deskStation;						//���������ͽ��� 20081211

	//��Ϣ����
protected:
	HINSTANCE						m_hGameInstance;					//��Ϸ����
public:
	GameInfoStruct					m_GameInfo;							//��Ϸ��Ϣ
	static bool						m_bIsAsk;							//�Ƿ���ʾ����
	IFrameInterface					* m_IGameFrame;						//��Ϸ���
protected:
	
	MatchUserInfo					* m_pMatchUserInfo;					//������Ϣ
	CFindNullDesk					m_finddesk;
	bool                            m_bDeAloneAll;                        ///< �Ƿ�����������
	bool OnAloneDesk(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	
public:
	CBoardCast						*boardCast;
	//������Ϣ
private:
	CRgn							m_Rgn;								///< �������򣬲��ػ��Ӵ��ڲ���
	BYTE							m_bPass;							//��������
	BYTE							m_bLimitCut;						//���ƶ���
	BYTE							m_bLimitPoint;						//���ƾ���ֵ
	BYTE							m_bCutPercent;						//������
	TCHAR							m_szPass[61];						//��������
	long int						m_dwLowPoint;						//��;���ֵ(����)
	long int						m_dwHighPoint;						//��߾���ֵ(����) 
	long int						m_dwLowMoney;						//��;���ֵ(���)
	long int						m_dwHighMoney;						//��߾���ֵ(���) 
	static BYTE						m_bSameIP;							//��ͬ IP
	static BYTE						m_bSameIPNumber;					//����ǰ��λIP��ͬ�������Ϸ
	static BYTE						m_bInvaliMode;						//����ģʽ
	static BYTE						m_bSaveTalk;						//�����¼
	static BYTE						m_bShowUser;						//��ʾ�û�
	static BYTE						m_bShowMessage;						//��ʾ��Ϣ
	int								m_iNewsNO;							//��������ʾ���ƣ��ڼ�����
	int								m_iSysMsgNO;						//ϵͳ��Ϣ��ʾ���ƣ��ڼ�����
	int								m_iNewCurCircle;					//����ѭ������
	int								m_iSysCurCircle;					//ϵͳ��Ϣѭ������
	//������Ϣ
private:
	static bool						m_bAutoSit;							//�Ƿ��Զ�
	static int						m_iMaxShortWnd;						//��ര��

	//���и�����Ϣ
protected:
	bool							m_bRoomSystem;						//������Ϣ
	bool							m_bGameSystem;						//��Ϸ��Ϣ
	bool							m_bWaitSit;							//�ȴ�����
	bool							m_bHideRight;						//�Ƿ�����
	bool							m_bHideBottom;						//�Ƿ�����
	bool							m_bLogonFinish;						//��½���
	BYTE							m_bMatchDeskIndex;					//��������
	BYTE							m_bMatchDeskStation;				//����λ��
	BYTE							m_bPreSitDeskIndex;					//Ԥ������
	BYTE							m_bPreSitDeskStation;				//Ԥ��λ��
	BYTE							m_bLastFindDesk;					//����λ��
	long int						m_dwCurrentUserID;					//��ǰ�û�
	int								m_itime;							//30��󲻿�ʼ��Ҫ�Զ��뿪
	//int								m_iNewsNO;							//��������ʾ���ƣ��ڼ�����
	//int								m_iSysMsgNO;						//ϵͳ��Ϣ��ʾ���ƣ��ڼ�����
	int                             m_RoomOwner;                         ///< ���«2 �Ƿ���Ȩ���޸�����,1 ��Ȩ�ޣ�2Ϊ�ڶ����Ժ��޸�

	//wushuqun 2009.7.10 �޸��û����С�ڷ�����ʱ����ʾ��ʽ
	int                             m_lessPointType;
	CString							m_strLessPointTipURL;

	//���츨����Ϣ
protected:
	CString							m_strLastTalk;						//��������
	CString							m_strTalkName;						//��������
	long int						m_dwLastTalkTime;					//˵��ʱ��
	long int						m_dwTalkTarget;						//���� ID
	RecentlyTalkUser				m_RecentlyUser[5];					//����û�
	static CArray<LONG,LONG>		m_ShieldArray;						//�����û�

	//�ⲿ��Ϣ
protected:
	RoomInfoStruct					* m_pRoomInfo;						//������Ϣ
	MSG_GP_R_LogonResult			* m_pLogonInfo;						//��½��Ϣ

protected:
	//CBzLog							m_BzLog;			                //������־

	//�ؼ�ָ��
public:
	CComboBox						* m_pInputMessage;					//������Ϣ
	CUserListCtrl					* m_pUserList;						//�û��б�
//	CUserListCtrl					* m_pFriendList;					//�û��б�
	CFindUserDlg					* m_pFindUser;						//�����û�
	CManageDlg						* m_pManageDlg;						//����ָ��
	//CDialogCallGM					* m_pCallGMDlg;						//����GM
	//CBankDlg						* m_pBankDlg;						//Ǯ��
	CPBankDlg						* m_pPersonBank;					//��������
	CPBankWnd						* m_pBankWnd;						//��������
	CBankVerifyWnd					* m_pBankVerifyWnd;					//�°�������У���½��
	CPBankWndEx						* m_pBankWndEx;						//�°��������
	CQueueInRoomDlg					* m_pQueueInRoomDlg;				///<�Ŷӻ��Ի���
	CUserDetailsDlg					* m_pUserDetailsDlg;				///<�û���ϸ��Ϣ���
	//CGoAnimationWnd						m_Flash;
	CIEClass							m_WebView;


	//����ͼ
private:
	CGameImage						m_roomlc,m_roomrc,m_roomtc,m_roombc;	//���ͼƬ	
	CGameImage						m_roomlt,m_roomrt,m_roomlb,m_roomrb;	//�߽�	
	CGameImage						m_mainlt,m_mainrt;

	//CGameImage						m_l,m_r,m_t,m_b,m_lt,m_lb,m_rt,m_rb;//���ͼƬ
	//CGameImage						m_vl,m_hl,m_lcl;
	//CGameImage						m_vlist_r,m_vlist_rt,m_vlist_rb;//vlist�ұ�
	//CGameImage						m_ulist_t,m_richedit_b;
	//COLORREF						m_titlec;
	//int								m_minx,m_miny,m_clx,m_cly,m_bx,m_by,m_ulisty,m_listy;
	//int								m_btw;	//��ť֮��ļ��
	//	int								m_colory,m_adx,m_ady,m_adheight,m_titlex,m_titley;
	//CGameImage						m_button,m_min,m_cl;
	//CGameImage						m_lcll,m_lclr,m_sk;
	//int								m_desktopbty,m_skx,m_sky;
	//	CGameImage						m_bi1,m_bi11;
	//=============================================================================
	//CGameImage						m_PicRoomBlud;						//���ͼƬ
	//CGameImage						m_PicRoomRed;						//���ͼƬ
	//CControlMessage                 m_HorSplitBar;                       //��ֱ�ָ���
public:
	CDialogCallGM						m_pCallGMDlg;

	//�ؼ�����
public:
	SendMsgUserStruct					m_SortMsgUserInfo;
	//CControlMessage					m_HorSplitBar;						//������
	//CControlMessage					m_HorSplitBar2;						//�������
	//CControlMessage					m_VorSplitBar;						//�������
	//CNewMiniDlg						m_TalkMessage;						//������Ϣ
	//CNormalBitmapButtonEx				m_Btsk;
	//CNormalBitmapButtonEx					m_BtGameFace;						//���水ť
	//CNormalBitmapButtonEx					m_BtFind;							//���Ұ�ť
	//CNormalBitmapButtonEx					m_BtRoomSet;						//���ð�ť
	//CNormalBitmapButtonEx					m_BtHideRight;						//���ذ�ť
	////CNormalBitmapButtonEx					m_BtMin;
	//CNormalBitmapButtonEx					m_BtClose;
	//CNormalBitmapButtonEx					m_restore;
	//ֻ����10�����ܰ�ť,��������
	CNormalBitmapButtonEx					m_bt1;
	CNormalBitmapButtonEx					m_bt2;
	CNormalBitmapButtonEx					m_bt3;
	CNormalBitmapButtonEx					m_bt4;
	CNormalBitmapButtonEx					m_bt5;
	CNormalBitmapButtonEx					m_bt6;
	CNormalBitmapButtonEx					m_bt7;
	CNormalBitmapButtonEx					m_bt8;
	CNormalBitmapButtonEx					m_bt9;
	CNormalBitmapButtonEx					m_bt10;
	int										m_iBtnX,m_iBtnY,m_iBtnSpace;		//��ťλ�úͼ�϶
	//CNormalBitmapButtonEx					m_rBt1;					//��̳
	//CNormalBitmapButtonEx					m_rBt2;					//�������
	//CNormalBitmapButtonEx					m_rBt3;					//�������
	//CNormalBitmapButtonEx					m_rBt4;					//�������
	//CNormalBitmapButtonEx					m_rBt5;					//�������
	//CNormalBitmapButtonEx					m_rBt6;
	//CNormalBitmapButtonEx					m_rBt7;

	//CNormalBitmapButtonEx					m_showmenu;
	//CNormalBitmapButtonEx					m_deskup;
	//CNormalBitmapButtonEx					m_deskdown;
	//CNormalBitmapButtonEx					m_listup;
	//CNormalBitmapButtonEx					m_listdown;

	CDeskFrameView					m_DeskFrame;						//��Ϸ����
	CGameUserManage					m_UserManage;						//�û�����
	//CLookingClass					m_MessageHandle;					//��Ϣ����
	//CUserListCtrl					m_UserList;							//�û��б�
	//CUserListCtrl					m_UserListFriend;					//�û��б�
	//CChatControl					m_ChatControl;						//�������
	CToolTipCtrl					m_ToolTip;							//������ʾ
	CGameTalkText					m_ExpressWnd;						//���鴰��
	TCHAR							m_title[300];						//����
	CAFCColorResource				m_colorresource;
	//CDlgDudu						m_dudu;								//С����

	//�����
//	CGetPoint						m_DlgGetPoint;					//�����
	CRoomRightWnd *					m_pRightWnd;					//�ұߴ���ָ��
	bool							m_bAddQueue;						//�Ѿ��������

	int								m_nKeepAliveCount;				//�������ݰ�

    // PengJiLin, 2010-5-18, ��Ϊ0������¼����, ���е�¼���ܺ������������ô�������
    BYTE    m_byVideoUse;

    // PengJiLin, 2010-6-11, �Ҳ���û����б����ʽ�����ã�0Ϊԭ�ȵģ�1ΪŸ�ӵģ�2...
    BYTE    m_byUserListDefineType;

    // PengJiLin, 2010-6-29, ��ս���Ƿ�ʱ�䵽
    bool    m_bGameTimeOut;

    // PengJiLin, 2010-9-15, ׼����ɺ�����������λ
    BOOL    m_bAllowJoin;

	//���¼������ģ��            yjj 090223
	CAddRecordModule                m_AddRecordModule;
	void checkNetSignal(void);
    void getNetSignal(void * pNetData);
    void drawSignal(CDC *pDC);
    int m_NetSignalIndex;                //����������Ϣ������
    int m_SavedSingalIndex;     //��һ�η��͵�������Ϣ�����ţ��յ���Ϣ����m_NetSingalIndexͬ��
    int m_NetSignalLevel;                //�����źż���
    Image  *img;

	//�����źŵ���ʾλ��
	int  m_netSignX;
	int  m_netSignY;
	int	 m_GameTax;
	int  m_RoomRule;

	//����������ʾλ��
	int  m_roomNameX;
	int  m_roomNameY;
	//����������ɫ
	COLORREF   m_roomNameClr;
	
	//������λ��
	int  m_toolBarX;
	int  m_toolBarW;

	CFont   *  m_roomNameFont;

    // PengJiLin, 2010-9-13, ���˿������߿�ʹ��ʱ���Ƿ��ڼ��
    BOOL    m_bCheckKickPropTime;       // FALSE = �����
    BOOL    m_bCheckAntiKickPropTime;   // FALSE = �����
    int     m_iKickPropTotalTime;       // ���˿�����ʱ��, ���ܹ���ʱ��ʼ��
    int     m_iAntiKickPropTotalTime;   // ���߿�����ʱ�䣬���ܹ���ʱ��ʼ��

	int nFlagMax;

    // PengJiLin, 2011-4-14, ��ȡ��� ����
    CGetHelpMoneyDlg*               m_pGetHelpMoneyDlg;

	// JianGuankun 2012-2-15, ����Ƿ��½���䣬����û�е�½����ʱ��
	// �յ�һЩ��Ϣ���ܻ�ʹ�������������ʹ����һ��־����
	bool   m_bLoginRoom;


	bool   m_bQueueRoomDisMiss;		//�ų��������Ƿ��ɢ������Ѿ���ɢ���û��㿪ʼҲ���˳���Ϸ�����Ŷ�
	int		m_iQueueNo;
	int     m_iQueueStation;

	bool	m_bCloseIsAutoQueue;	//��Ϸ�������뿪�Ƿ��Զ��Ŷ�
	int		m_iQueueTime;			//�Ŷӷ�����ʱֵ
	bool	m_bEnterAutoQueue;		//�����Ƿ��Զ��Ŷ�
	bool	m_bContinueExit;		//��Ϸ����������Ƿ��˳���Ϸ
	bool    m_bQueueing;			//�Ƿ��Ŷ�



	//�ͽ����ʾ
	vector<CString> m_TipSendMoney;
	int		m_iDeskPeopleMe;

private:
	CSkinMgr m_skinmgr;

public:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);
public:

	/// ����ȫ�ֱ����еļ��ܷ�ʽ��ȡ�ü��ܺ�������ַ���������sha������MD5
	/// �����ﲻ��ָ��ĺϷ������ж�
	/// @param szMD5Pass ���ܺ���ַ���
	/// @param szSrcPass Դ�ַ���
	/// @return ���ܺ���ַ���ָ��
	static TCHAR * GetCryptedPasswd(TCHAR *szMD5Pass, TCHAR *szSrcPass);

	/// ���н��淢��ָ�ת��
	/// @param pTransferMoney ��ȡǮ�ṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
	/// @return ��
	static void OnTransferMoney(CTCPClientSocket* pClientSocket,TransferMoney_t* pTransferMoney,bool bInMainRoom,UINT uHandleCode = 0);

	/// ת���û�ID�Ƚ�
	/// @param UserID ����Լ���ID��UserIDDestĿ��ID,ɢ��ID NomalIDFrom��ʼ��ɢ��ID NomalIDEnd����
	/// @return  0����ת�ˣ�1 ����ת��,��˰��2 ����ת��,����˰.3 ����ת�˶�Ϊ���ر�ID�����ת�˽��
	static int TransferIDCompare(long UserID,long UserIDDest,long NomalIDFrom,long NomalIDEnd);

	/// ��ȡ���ID��Ϣ���Ƿ�ɢ�����ر��˺ţ�ָ���˺�
	/// @param UserID ���ID,NomalIDFromɢ��ID��ʼ��NomalIDEndɢ��ID����
	/// @return 0��8��ָ���˺ź����0�ĸ�����-1Ϊɢ����-2Ϊ��Ч�˺ţ�-5Ϊ5���ţ�-6Ϊ6����
	static int GetIDInformation(long UserID,long NomalIDFrom,long NomalIDEnd);

	/// ���н��淢��ָ�ת�ʼ�¼
	/// @return ��
	static void OnTransferRecord(CTCPClientSocket* pClientSocket,bool bInMainRoom);

private:
	/// ������������Ƿ���Ǯ���жϿɲ����Լ�����Ϸ
	/// @return ��Ǯ����true, ���򷵻�false
	bool IsEnoughMoneyToPlay();

	bool IsEnoughMoney();

	/// ���н��淢��ָ���Ǯ��ȡǮ
	/// @param pCheckMoney ��ȡǮ�ṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
	/// @return ��
	void OnCheckMoney( CheckMoney_t *pCheckMoney );
	/// ���н��淢��ָ��޸�����
	/// @param pChangePasswd �޸�����ṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
	/// @return ��
	void OnChangePasswd( ChangePasswd_t *pChangePasswd );
	/// �������뷿����Ϣ
	/// @param HandleCode ��Ϣ����
	/// @return BOOL ��Ϣ����������TRUE
	BOOL OnPassWordRoomMsg(DWORD HandleCode);

public:
    // PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ�
    void OnGetNickNameOnID(GetNickNameOnID_t* pGetNickNameOnID);

    // PengJiLin, 2010-8-23, ����ȫת����
    void OnTransferAllMoney( TransferMoney_t *pTransferMoney );

public:
    // PengJiLin, 2010-9-13, ���˿������߿�ʹ��ʱ������
    void SetKickPropTime(int iKickTime);
    void SetAntiKickPropTime(int iAntiKickTime);

    // PengJiLin, 2010-10-14, ģ�����������Ϸ�˷�����Ϣ
    void SendDataToGame(void * pNetData, UINT uDataSize, 
                        DWORD dwMainID, DWORD dwAssistantID, DWORD dwHandleCode);
    void SendDataToGame(DWORD dwMainID, DWORD dwAssistantID, DWORD dwHandleCode);

	//JianGuankun, 2012-2-16, �����˳����䰴ť����
	void SetEnableExitRoomBtn();

    // PengJiLin, 2011-6-23, ������Ϣ
    void SetActiveToGameRoom(UINT uState);
    BOOL    m_bActive;

public:
	void OnParentMove(int x,int y );
	//��������
public:
	//���캯��
	CGameRoomEx(TCHAR * szGameName, RoomInfoStruct * pRoomInfo, MSG_GP_R_LogonResult * pLogonInfo); 
	//���캯��
	CGameRoomEx(TCHAR * szGameName, RoomInfoStruct * pRoomInfo, MSG_GP_R_LogonResult * pLogonInfo,CRoomRightWnd *pRightWnd); 
	//��������
	virtual ~CGameRoomEx();

///////////////////����ĺ��������ֻΪ���˷�����/////////////////////////////////

	//Ϊ�����˳��ܹ��ڽ��治���������л����䣬�����ù����������ֱ�Ӽ����·���
	void RelaodRoom(int iIndex);
	//���ӽ��뷿��
	void EnterRoom(int iIndex);
	//���˳���ʱ����Ҫ�л����䣬-1Ϊ����������Ϊ�л����������
	int GetChangeRoomIndex();
	//��������
	bool ChangeRoom();
	//�ҵ���ǰ�������ٵ���С��iDownLimit�ķ���
	int FindMinPeopleRoom(int iDownLimit = -1);
	//�ҵ���ǰ������൫������iUpLimit�ķ���
	int	FindMaxPeopleRoom(int iUpLimit = 9999);
	//�Ƿ����еķǿշ��䶼����iLimit
	bool IsAllRoomPass(int iUpLimit);
	//���ǿշ��䶼�Ѿ������˿����޿����·���
	int FindNewRoom();
	//��ǰ���˷�������
	int m_iContestRoomIndex;
	//�Ƿ���Ҫ�л�����
	bool m_bChangeRoom;
	//�����˿����ޣ�m_iPeopleLimit1Ĭ��Ϊdeskpeople*deskcount)/2,m_iPeopleLimit2=m_iPeopleLimit1 * 5/4)
	int	m_iPeopleLimit1, m_iPeopleLimit2;
	int m_iQueueTimeOut;
	//���˳��л������־Ӌ����
	int m_iChangeRooming;
	//��⵽socket�Ͽ��Ĵ���
	int m_iSocketClose;

////////////////////////////////////////////////////////////////////////////////


	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

	//���غ���
public:
	//���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�˵�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//�ӿں��� 
public:
	//ѯ�ʹر�
	virtual bool CanCloseNow();
	//�رմ���
	virtual bool CloseGameRoom();
	//λ�ñ仯
	virtual bool OnSizeEvent(UINT nSizeType, int cx, int cy);
	//�������Ϣ
	virtual bool OnSplitEvent(UINT uSplitID, UINT uXPos, UINT uYPos);
	//����λ��
	virtual void FixControlStation(int iWidth, int iHeight);
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//����������Ϣ
	virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket);
	//����ر���Ϣ
	virtual bool OnSocketCloseEvent();
	//ȡ��������Ϣ
	virtual bool OnCancelConnectEvent();

	//��Ϸ�ͻ��˿��ƺ���
protected:
	//������Ϸ�ͻ���
	inline bool StartGameClient(bool bShow = 1);
	//�ر���Ϸ�ͻ���
	inline bool CloseGameClient();
	//���Ϳ�����Ϣ 
	inline UINT ControlGameClient(UINT uControlCode, void * pControlData, UINT uDataSize);

	//��������
public:

	 //ADD xqm 20101109
	//��ȡ��Ϸ��ܵ�ָ��
	IFrameInterface* GetGameFrame(){return m_IGameFrame;};

	//�汾���
	bool GameRoomVerCheck(MSG_S_ConnectSuccess * pVerInfo, bool & bNewVer);
	//���Ҽ��˵�
	bool OpenUserMenu(UserItemStruct * pUserItem);
	//���Ҽ��˵�
	bool OpenUserMenuEx(UserItemStruct * pUserItem,bool bInDesk);
	//��ʾ���Ҵ���
	bool ShowFindUserWnd();
	//��ȡ��������
	UINT GetUserSortIndex(UserInfoStruct * pUserItem);
	//��ȡ��ҹ�ϵ
	UINT GetUserConnection(long int dwUserID);
	//������ʾ����
	void UpdateRoomTitle();
	//����������Ϣ
	void SendUserSetInfo();
	/// ����������ͷ�������������Ϣ
	/// @param ��
	/// @return �ޣ�ֻ��VIP��Ҳ�����������
	void SendVIPRoomSetInfo(char * PassWord,int len);
	///��GDI+˫���淽ʽʵ�ֵĻ滭���
	void DrawViewFramePlus(CDC * pDC, int iWidth, int iHeight);
	//�滭���
	void DrawViewFrame(CDC * pDC, int iWidth, int iHeight);
	//���Ͷ���Ϣ
	void SendShortMessage(UserItemStruct * pUserItem);
	//������Ϣ����
	CShortMessage * ActiveShortWnd(long int dwUserID, UserItemStruct * pUserItem, bool bCreate);
	//ͬ����Һ;���ֵ
	void SyncMoneyAndPoint();
	//ѡ�񷿼��ұ���
	void ChoiseDeskBasePoint();
	//��ʾ���Ż�ϵͳ��Ϣ
	void ShowNewsOrSysMsg(BYTE newsOrSysMsg);
	//����ĳ�����Ӻ�λ�Ӻŵ��û�Ϊ��ǰ�û�
	void SetCurrentUser(long dwUserID);
	//��Ǯ����
	bool PayMoney(int iCount,int iPayID,long dwUserID=-1);
	/// ˢ���û��б��еĽ��
	/// @param dwUserID �û�ID��
	/// @param uCount ��Ϊ��������ֵ����˼Ϊ�ӻ��
	bool FlushUserMoney(long dwUserID,__int64 i64Count);
	///ˢ���û��Լ������н��
	/// @param i64Count ���º�����н��
	bool FlushMyBankMoney(__int64 i64Count);
	//С����
	bool Dudu(_TAG_BOARDCAST* pDudu,int iDuduType = 0);

	/*ˢ����ҵ�����
		@Author:JianGuankun 2012.6.27
		@param dwUserID ����������û�ID,
		@param uNewLogoID �������logo ID
	*/
	bool OnAvatarChange(long dwUserID,UINT uNewLogoID);

	///< ��ȡ��Ϸ����Ļ��ּ���
	///< @dwPoint��һ���
	///< ��һ�������
	static TCHAR * GetGameOrderName(__int64 dwPoint,UINT uGameNameID);
	///< ��ȡ�ȼ���Ϣ��GetGameOrderNameʹ��
	void ReadLevelData();

	//����Ƿ�ټ��ַ���
	//JianGuankun 2012.3.29
	bool IsBJLRoom();


	//���뵽�Ŷӻ�
	void JoinQueue();
	//������Ϣ����
protected:
	//�����û���Ϣ
	bool SendUserInfo();
	//Ǯ����
	bool OnAboutMoney(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϣ����
	bool OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�����½����
	bool OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�û��б���
	bool OnUserListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�û���������
	bool OnUserActionMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	////////////////////////////////////////////////////////////////////
	/// �������
	bool OnUserActionSitDown(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	/// ������´���
	bool OnUserActionSitDownError(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	/// �Ŷӻ��������������
	bool OnUserActionQueueSitDown(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	/// ���վ��
	bool OnUserActionStandUp(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	/// ��Ҷ���
	bool OnUserActionOffLine(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	/// ����뿪
	bool OnUserActionCome(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	/// ����뿪
	bool OnUserActionLeave(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	/// ��ҵ����ʼ
	bool OnUserActionHitBegin(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	/// ������ñ���
	bool OnUserActionSetBasePoint(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	////////////////////////////////////////////////////////////////////

	//������Ϣ����
	bool OnRoomMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϣ����
	bool OnMatchMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϣ����
	bool OnManageMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//ϵͳ��Ϣ����
	bool OnSystemMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//ģ�����������ϵͳ��Ϣ
	bool SendMessageToGame(CString szMessage);	
	//ģ�����������ϵͳ��Ϣ
	bool SendMessageToGame(TCHAR * szMessage);	
	//�������Ϣ������
	virtual bool HandlePointMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϣ������
//	virtual bool OnItemDataMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//��ȡĳһ����������
	BOOL UpdateHoldItem(int ItemID);
	//�ܹ�����ĵ����б�
	virtual bool OnPropMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	virtual bool OnUserUsePropResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	virtual bool OnUseSmallBoardcast(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�������
//	virtual bool OnBuyItemResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//���³��е���
//	virtual bool OnUpdateHoldItemResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//ʹ�õ���
//	virtual bool OnUserUseItemResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�㲥��Ϣ
	//virtual bool OnBroadCastManage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//���ѹ���
	virtual bool OnFriendManage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϣ
	virtual bool OnQueueMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//vip���
	virtual bool OnVipCheckMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//vip��Ϣ
//	virtual bool OnVipOperationMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�㲥��Ϣ
	//virtual bool OnBroadcastMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϣ
	virtual bool OnKickUserMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

    // PengJiLin, 2010-9-13, ʹ�����˿����˹���
    virtual bool OnNewKickUserMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);


	virtual bool OnNeBankMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

//�û��˵���Ϣ
public:
	//�鿴�û�����
	void OnUserDetails(bool bInDesk);
	//�����û�����
	void OnCopyUserName();
	//���Ͷ���Ϣ
	void OnSendShortMessage();
	//����������Ϣ
	void OnSendRoomTalk();
	//��������
	void OnShieldUser();
	//�������
	void OnUnShieldUser();
	//���ҿ�����
	void OnFindNullDesk();

	//�������£���Ҫ�ж�IP�ǲ�������֮ͬ��
	bool IsCanSit(UserItemStruct* pUserInfo, int index);

	//������λ
	void OnFindDesk(bool bsit,bool bOneNull, bool bTwoNull, bool bNoAllNull, bool bAllNull);
	//�������
	void OnInvitePlay();
	//����Ϊ�������
	void OnSetTalkObject();
	//ȡ���������
	void OnCancelTalkObject();

	///�ж��Ƿ����Ŷӻ�����
	///@return true:���Ŷӻ����䣻false�������Ŷӻ�����
	bool IsQueueGameRoom();

	//��ť��Ϣ����
public:
	////���ذ�ť
	//void OnBnClickedHideRight();
	////���水ť
	//void OnBnClickedGameFace();
	//���Ұ�ť
	//void OnBnClickedFindMore();
	//���ð�ť
	void OnBnClickedRoomSet();
	//���鰴ť
	void OnBnClickedExpression();
	////������ť
	//void OnBnClickedAction();
	////���߰�ť
	//void OnBnClickedOnLine();
	//���Ѱ�ť
	void OnBnClickedFriend();
	////��С����ť
	//void OnBnClickedMin();
	//�رհ�ť
	void OnBnClickedClose();
	//void OnBnClickedr1();
	//void OnBnClickedr2();
	//void OnBnClickedr3();
	//void OnBnClickedr4();
	//void OnBnClickedr5();
	//���书�ܰ�ť
	void OnBnClickedBt1();
	//
	void OnBnClickedBt2();
	//
	void OnBnClickedBt3();
	//
	void OnBnClickedBt4();
	/// �����������ð�����Ӧ
	/// @param ��
	/// @return ��
	void OnBnClickedBt5();
	//
	void OnBnClickedBt6();
	//
	void OnBnClickedBt7();
	//
	void OnBnClickedBt8();
	//
	void OnBnClickedBt9();
	//
	void OnBnClickedBt10();

public:
		
	CDlgRoomPW dlg;
	//��Ϣ����
public:
	//�ػ�����
	afx_msg void OnPaint();
	//��ʾ����
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	////����Ҽ��û��б���Ϣ
	//afx_msg void OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	////���˫���û��б���Ϣ
	//afx_msg void OnNMDblclkUserList(NMHDR * pNMHDR, LRESULT * pResult);
	////����Ҽ��û��б���Ϣ
	//afx_msg void OnNMRclickUserListFriend(NMHDR * pNMHDR, LRESULT * pResult);
	////���˫���û��б���Ϣ
	//afx_msg void OnNMDblclkUserListFriend(NMHDR * pNMHDR, LRESULT * pResult);
	//�����������
	LRESULT OnLeftHitDesk(WPARAM wparam, LPARAM lparam);
	//����ʱ�����������
	LRESULT OnHitDesk(WPARAM wparam, LPARAM lparam);
	//�Ƿ���ʾ��Ϸ,�����ȼ�
	LRESULT OnShowGameWindow(WPARAM wparam, LPARAM lparam);
	//ˢ�¾���ֵ
	LRESULT OnReflashMoney(WPARAM wparam, LPARAM lparam);
	//�Ҽ���������
	LRESULT OnRightHitDesk(WPARAM wparam, LPARAM lparam);
	//������Ϣ
	LRESULT OnExpression(WPARAM wparam, LPARAM lparam);
	//��ʼ��ť
	LRESULT OnHitDeskBegin(WPARAM wparam, LPARAM lparam);
	//
	LRESULT OnLoadSkin(WPARAM wparam, LPARAM lparam);

	//�ټ��ֿ�ʼ��ť add by wlr 20090713
	LRESULT OnHitBJLDeskBegin(WPARAM wparam,LPARAM lparam);
	//end of add by wlr 20090713


	//�Ŷӻ��뿪��ť
	LRESULT OnHitQueueStop(WPARAM wparam,LPARAM lparam);


	//�ض�talk.ini
	//LRESULT OnReLoadTalkINI(WPARAM wparam, LPARAM lparam);
	//��������
	LRESULT  OnReflashNews(WPARAM wparam, LPARAM lparam);
	//���ͻ���
	LRESULT OnGetPoint(WPARAM wparam,LPARAM lparam);	
	//����ϵͳ��Ϣ
	LRESULT OnReflashSys(WPARAM wparam, LPARAM lparam);

	LRESULT OnSendRoomMsg(WPARAM wparam, LPARAM lparam);

	/// ��ӦVIP�������Ϣ
	/// @param wparam �Ƿ�VIP����
	/// @return ��
	LRESULT OnVIPRoomMsg(WPARAM wparam,LPARAM lparam);

	afx_msg void OnTimer(UINT nIDEvent);
	//afx_msg void OnBnClickedListpagedown();
	//afx_msg void OnBnClickedListpageup();
	//afx_msg void OnBnClickedShowmenu();
	//afx_msg void OnBnClickedSkin();
	void LoadSkin(void);
	//afx_msg void OnBnClickedBt9();
	//afx_msg void OnBnClickedBt8();
	//afx_msg void OnBnClickedBt6();
	//afx_msg void OnBnClickedBt7();
	//��¼��������û���Ϣ    yjj 090224
	void  AddUserInfoToRecord();
	//---------------------------

	LRESULT OnShowLeft(WPARAM wparam,LPARAM lparam);

	LRESULT OnTopTabSelected(WPARAM wparam,LPARAM lparam);

	LRESULT OnTopTabLostSelected(WPARAM wparam,LPARAM lparam);

	LRESULT OnGetRoomName(WPARAM wparam,LPARAM lparam);

	LRESULT OnCloseGameClient(WPARAM wparam,LPARAM lparam);

	LRESULT OnCutNetQuitRoom(WPARAM wparam,LPARAM lparam);//JMod-��Ҷ����йܺ��˵���Ϸ����-20090513

	//��ҳURL�仯��⣬JianGuankun 2012.7.10
	void BeforeNavigate2Explorer(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
	void NavigateErrorExplorer(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Frame, VARIANT* StatusCode, BOOL* Cancel);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//�Թ�
	LRESULT OnWatch(WPARAM wparam, LPARAM lparam);
	//����
	LRESULT OnChating(WPARAM wparam, LPARAM lparam);
	//����
	LRESULT OnInvite(WPARAM wparam, LPARAM lparam);
	//��IE
	LRESULT OnOpenIE(WPARAM wparam,LPARAM lparam);
	//����ɫ
	COLORREF GetDuduRGB();
	//����Ƿ�Ҫ�뿪��Ϸ��
	bool CheckLeaveDesk();//��Ҹ���ʱ�����ж�
	//�Ŷ�
	bool AddQueue();
	//�Ŷӽ��
	bool  AddQueueResult();
	//����������
	bool OnSitDesk();
	//ͳ��ĳ������
	BYTE CountAmount(BYTE bDeskIndex);	
	//��������������(�����λ)
	bool OnFindRandDesk(BYTE &bDeskIndex,BYTE &bDeskStation);
	//���ʹ�㲥��Ϣ
	//LRESULT SendBigBoardcastMsg(WPARAM,LPARAM);
public:

	void IE(LPCTSTR URL);

public:
	void UseBoardCast(_TAG_USERPROP * userProp);
public:
	void SendBoardCast(_TAG_BOARDCAST * boardCastMsg, int BRD_MSG_TYPE);
public:
	void OnGiveProp(long dwUserID,int openFrom);
public:
	void InsertSystemMessageWithGame(CString strMsg);
public:
	void RandVirtualMessage(UserItemStruct * userItem);
public:
	//����Ϸ�����������Ϣ
	void BoardcastTrottingMsg(TMailItem* pTrottingMsg);


	//��Ҳ����������л��߳�ֵ����
	void NoEnoughMoneyEvent();

	
//////////////////////////////////////////////////////////////////////////
// YangJie 2012-05-14 9:21 AM
// Unity3d��Ϣ���ݺ���
public:
	int u3dSendGameBaseInfo();

public:
	bool GetSendMsgUserStruct(long int lUserID, SendMsgUserStruct * user);
	SendMsgUserStruct * GetMeUserInfo();
	//int SendData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//int SendData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);


	//����
	virtual int SendData(void * pData, UINT uSize, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//����
	virtual int SendData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);

	//wushuqun 2009.7.13
	//�޸���Ϸ�������˳���Ϸʱ��������
	afx_msg BOOL OnEraseBkgnd(CDC* pDC)
	{
		return true;
		//
		return CWnd::OnEraseBkgnd(pDC);
	}
public:
	CVipTipDlg m_vipDlg;
};
// �������������Ϣ
extern void DebugPrintf(const char *p, ...);

static CGameRoomEx * gpGameRoomEx = NULL;
//gpGameRoomEx����Ϸ���䴴��ʱ���޸ģ�
//				����Ϸ����Tabҳ��ѡ��ʱ���޸ģ�
//				��MainRoom����Tabҳ��ѡ��ʱҲ���޸ġ�

///��ȡ��Ϸ����ָ��
///@return ������Ϸ�����ָ��
extern CGameRoomEx * GetGameRoom(void);
extern CGameRoomEx * GetGameRoom(UINT uRoomID);

void u3dchar2Wchar(char* pchar,wchar_t* wchar);

#endif