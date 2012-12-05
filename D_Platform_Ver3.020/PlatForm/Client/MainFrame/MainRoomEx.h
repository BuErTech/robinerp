#pragma once
#ifndef MAINROOMEX_HEAD_H
#define MAINROOMEX_HEAD_H

#include "apipe.h"//�����ܵ� 2012.08.20 yyf
#include "Socket32.h"


#include "Stdafx.h"
#include "GamePlace.h"
#include "BaseRoom.h"
#include "AutoUpdate.h"
#include "GameListCtrl.h"
#include "GameRoomEx.h"
#include "NavigationDlg.h"
#include "UserListCtrl.h"
#include "HttpDownloadDlg.h"

#include "AFCActWnd.h"					//����֪ͨ
#include "AfcTopWnd.h"					//������ť
#include "DUITopWnd.h"
#include  "afcgamelistwnd.h"			//��Ϸ������
#include "afCRoomRightWnd.h"
#include "Bulletin.h"

#include "PBankWndEx.h"
#include "BankVerifyWnd.h"
#include "MobileVerifyWnd.h"

//ͷ���ϴ�
#include "LogoUpload.h"
//IM
#include "IMMain.h"
//�������
#include "PropPannel.h"
#include "AppealDlg.h"

#include "GMListDlg.h"
#include "..\\UILibrary2003\\ShortMessage.h"
#include "gameplacemessage.h"
#include "GameListTip.h"

#include "CharmExchange.h"
#include "SafeCenterWnd.h"

#include "SkinMgr.h"
#include "PassDesk.h"
#include <vector>
#include <queue>
using namespace std;

//�궨��
#define MS_MAX						10									//�����������
#define MAX_GAME_ROOM				3									//��󷿼���Ŀ
#define WM_CLOSE_ROOM				WM_USER+216							//�ر���Ϸ����

//wushuqun 2008.10.14
#define WM_COLSE_ROOM_WITH_MSG      WM_USER+219                      //�رշ�����ô�������һ����Ϣ��

#define IDM_SKIN					WM_USER+3
#define WM_REFLASNEWS				WM_USER+302							//�����ȡ����
#define WM_REFLAS_SYS_MSG			WM_USER+303							//�����ȡϵͳ��Ϣ

#define WM_GAME_LAUNCHED					WM_USER+439					///< ��Ϸ�������̽���
#define WM_GM_MSG							WM_USER+477
#define WM_GM_KICK							WM_USER+478

#define WM_GP_PROP							WM_USER+479					//��Ϸ�˵�����Ϣ

// PengJiLin, 2010-6-29, ��ս��ʱ�䵽, �ر���Ϸ��
#define WM_GAME_TIMEOUT                     WM_USER+442


#define WM_QUEUEROOM_EXIT                     WM_USER+443			//�Ŷӻ������ɢʱ���Ѿ�ͬ����û�����ȥ�Ŷӣ���û�������û����ʲô������ȥ�Ŷ�
#define WM_USERCLIKE_AGREE                    WM_USER+444			//�û����ͬ��
#define WM_CLOSEGAME		                   WM_USER+445			//�û����ͬ��




//����ṹ
struct MissionStruct
{
	bool							bAccess;							//�Ƿ���Է���
	UINT							uMissionID;							//���� ID
	UINT							uKindID;							//���� ID
	UINT							uNameID;							//���� ID
};
//��Ϸ������Ϣ
struct RoomInfoStruct
{
	//�ṹ����
	bool							bAccess;							//���ʱ�־
	UINT							uTabIndex;							//��������
	DWORD							dwGameMSVer;						//���汾��
	DWORD							dwGameLSVer;						//���汾��
	CBaseRoom						* pGameRoomWnd;						//����ָ��
	CRoomRightWnd					*pRoomInfoWnd;						//��Ϸ���
	TCHAR							szProcessName[31];					//��������
	ComRoomInfo						pComRoomInfo;						//��������
	CAFCRoomItem*					pRoomItem;					
};
//���½ṹ
typedef struct update
{
	int				uNameID;	//��ϷID
	CAFCRoomItem	*pGameRoomItem;	//
	HANDLE			hProcess;	//���½���
}UPDATE,*LPUPDATE;

//add by wyl  11-5-16


//2012.08017 yyf
//�����ܵ���Ϣ�ṹ
typedef struct UpdateStructMsg
{  
	char command;//1: ������->�ӽ��� ; 0:�ӽ���->������
	int nRoomID;//����id�� 
	TCHAR g_GameName[200];//������ (��Ϸ��
	TCHAR g_url[1000];//����URL��ַ
	TCHAR g_ver[20];//�汾�� 
} sUpdateStructMsg;//ÿ������������Ҫ����Ϣ������һ���ṹ��.

class CMainRoomEx ;
typedef struct DownLoadUpdateMsg
{
	UpdateStructMsg *pUpdateStructMsg;
	UPDATE *pUPDATE;
	CMainRoomEx *pCMainRoomEx;
	CBcfFile *pCBcfFile;
} sDownLoadUpdateMsg;//�������������߳��� 2012.08.24 yyf

//��Ϸ�������Ի���
class CMainRoomEx : public CBaseRoom,public IAnonymousPipe,public ISimpleSocket
{
	CGameListTip *m_pListTip;
	static bool bTipInit;
	bool m_bListmessage;
	bool m_bInit;
	void OnConnectToCenterServerSucceed();
	bool m_bConnectingToCenterServer;
	bool m_bGetPara;
	//��������
public:
	static bool						m_bLockQuit;
	bool							m_bLogon;							//��½�ɹ�
	CGameListWnd					*m_pGameListWnd;					//��Ϸ�б�
	CActWnd							*m_pActWnd;							//���洰��
	CTopWndClass					*m_TopWnd;							//�Ϸ�����
	CTopDuWndClass					*m_TopDuWnd;						//�Ϸ�����Direct UI��
	CBulletin						*hBulletin;
	//add by wyl    11-5-13
	CAFCRoomItem					*m_pRoomItemInfo;					//��Ϸ������Ϣ
	MSG_GP_R_SellGameListResult		m_SellListInfo;						//�����б���Ϣ
	static char						m_szMobileVCode[36];				//�ӷ����̷��������ֻ���֤��MD5����\
																			Adde by JianGuankun 2012.5.16


private:
	bool							m_bCheckVer;						//���汾
	bool							m_bHideList;						//�����б�
	bool							m_bHideNews;						//���ع���
	CAutoUpdate						m_Update;							//�Զ�����
	MissionStruct					m_Mission[MS_MAX];					//��������
	vector<LPUPDATE>					m_UpdateList;						//���ڸ����б�
	CRITICAL_SECTION				m_Section;							//�����б���µ�ͬ��

	//CAnonymousPipe m_AnonymousPipe;//�����ܵ� 2012.08.17 yyf
	InstanceLockBase m_InstanceLockBase;//ͬ�������� 2012.08.22 yyf
	CSocket32      m_CSocket32;//���ؽ��̼� �������  2012.08.24 yyf

	static UINT							m_onlineUserCount;					//��������(������Ϸ����ͳ������������)
public:
	RoomInfoStruct					m_RoomInfo[MAX_GAME_ROOM];			//������Ϣ
	MSG_GP_R_LogonResult			m_PlaceUserInfo;					//�û�����

	//����ͼ
private:
	CGameImage						m_mainlc,m_mainrc,m_maintc,m_mainbc;	//���ܿ�
	CGameImage						m_mainlt,m_mainrt,m_mainlb,m_mainrb;//�Ľ�


	int								m_skx,m_sky;
	int								m_adx,m_ady,m_adheight,m_picx,m_picy,m_infox,m_infoy;
//	int								m_minx,m_miny,m_clx,m_cly,m_bx,m_by;
	int								m_wb;	//��ť֮��ļ��
	CString							m_utitle;
	COLORREF						m_titlecolor;
	BOOL							m_bLockMathine;						//��ʾ�������������Ƿ����� zxj 2009-11-12
	BOOL							m_bCanClick;						//��ʾ��ǰ����������ť�Ƿ����
public:
	BOOL							m_HaveSellList;						//�Ƿ��������б�

	//�ؼ�����
public:

	CIEClass							m_WebView;							//��ҳ��ͼ

	CNavigationDlg						m_NavigationDlg;					//��������

	CNormalBitmapButtonEx					m_Bt1;						//
	CNormalBitmapButtonEx					m_Bt2;						//
	CNormalBitmapButtonEx					m_Bt3;						//
	CNormalBitmapButtonEx					m_Bt4;						//
	CNormalBitmapButtonEx					m_Bt5;						//
	CNormalBitmapButtonEx					m_Bt6;
	CNormalBitmapButtonEx					m_Bt7;
	CNormalBitmapButtonEx					m_Bt8;
	CNormalBitmapButtonEx					m_Bt9;
	CNormalBitmapButtonEx					m_Bt10;

	int										m_mainbtnX;				//�Ϸ���ťλ����ʼ����
	int										m_mainbtnSpace;			//�Ϸ���ť֮���϶
	bool									m_bShowSysmsg;			//�Ƿ���������ϵͳ��Ϣ
	CLogoUpload								*logoUpload;			//ͷ���ϴ�����
	CPropPannel								*propPannel;			//����������

	static bool								m_bBankLogoned;			//�ѵ�½����?

	CCharmExchange							*CharmPannel;

	//Ͷ�߶Ի���
	CAppealDlg                              *m_pAppealDlg;            //Ͷ�߶Ի���

	//Ͷ�߶Ի���
	CGMListDlg                              *m_pGMListDlg;            //Ͷ�߶Ի���

	CSafeCenterWnd							*m_pSafeCenterWnd;		  //��ȫ���ĶԻ���

	CPBankWnd								* m_pBankWnd;			  //�ɰ��������
	CBankVerifyWnd							* m_pBankVerifyWnd;		  //�°�������У���½��
	CPBankWndEx								* m_pBankWndEx;			  //�°��������

	CMobileVerifyWnd						* m_pMobileVerifyWnd;	  //�ֻ���֤����


	map<int, CShortMessage*>				m_pGMMsgDlgs;

#ifdef AI_MACHINE
	int										m_Airoom;
#endif

	void checkNetSignal(void);
    void getNetSignal(void * pNetData);
    void drawSignal(void);
    int m_NetSignalIndex;                //����������Ϣ������
    int m_SavedSingalIndex;     //��һ�η��͵�������Ϣ�����ţ��յ���Ϣ����m_NetSingalIndexͬ��
    int m_NetSignalLevel;                //�����źż���
    Image  *img;

    // PengJiLin, 2010-9-13, ���˿������߿�ʹ��ʱ��
    int     m_iKickPropTotalTime;       // ���˿�����ʱ��
    int     m_iAntiKickPropTotalTime;   // ���߿�����ʱ��

	int		m_nKeepAliveCount;			//����	
#ifdef MY_QUICK_GAME_ON

	// JianGK 2011/09/20,�����ܵ�,�����ⲿͨѶ,����ƽ̨����һЩ���ڲ��ԵĹ���
	HANDLE m_hQGNamedPipe; //�ⲿͨѶ�Ĺܵ�
	HANDLE m_hQGThread;  //m_hNamedPipeͨѶ�����߳̾��
	UINT m_uQGRoomID; //Ҫ���������ķ���ID

#endif
	

	struct TQGInfo 
	{
		UINT uNameID;
		UINT uRoomID;

		TQGInfo()
		{
			uNameID = 0;
			uRoomID = 0;
		}
	};


	//��������
public:
	//���캯��
	CMainRoomEx();
	//������������
	CMainRoomEx(CGameListWnd *pGameListWnd, CActWnd *pActWnd, CTopWndClass *pTopWnd,CTopDuWndClass *pTopDuWnd);
	//��������
	virtual ~CMainRoomEx();

	DECLARE_MESSAGE_MAP()

	//���غ���
public:
	//���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//�ӿں���
public:
	//ѯ�ʹر�
	virtual bool CanCloseNow() { return true; };
	//�رմ���
	virtual bool CloseGameRoom() { return true; };
	//λ�ñ仯u
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
	//�˵�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	
	//���ܰ�ť
	void OnBnClickedBt1();////
	
	void OnBnClickedBt2();/////////
	
	void OnBnClickedBt3();/////////
	
	void OnBnClickedBt4();/////////
	
	void OnBnClickedBt5();/////////

	void OnBnClickedBt6();////
	
	void OnBnClickedBt7();/////////
	
	void OnBnClickedBt8();/////////
	
	void OnBnClickedBt9();/////////
	
	void OnBnClickedBt10();/////////

	//��������
private:
#ifdef AI_MACHINE
	bool EnterRoom(int roomid);
#endif
public:
	DECLARE_EVENTSINK_MAP()
	//��������
public:
	static DWORD WINAPI DownLoadNewIni(LPVOID lparam);
	//��д����
	void WriteNews();
	//

	
	void IE(LPCTSTR URL);
	//�汾���
	bool GamePlaceVerCheck(MSG_S_ConnectSuccess * pVerInfo, bool & bNewVer);
	//��������
	bool AddMission(UINT uMissionType, UINT uKindID, UINT uNameID);
	//ɾ������
	bool DeleteMission();
	//��������
	bool HandleMission();
	//���͵�½��Ϣ
	bool SendUserInfo();
	//��ȡ��Ϸ�б�
	bool GetGameRoomList();
	//��ȡ�����¼����Ϸ
	bool GetLastLoginGame();
	///��ȡ�һ��б�
	bool GetCharmExchangeList();
	//������Ϣ����
	bool OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//��½ ��Ϣ����
	bool OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//���� ��Ϣ����
	bool OnBankMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�б���Ϣ
	bool OnListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//ϵͳ��Ϣ
	bool OnSystemMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϸ����
	bool OnCentranceGameRoom(CAFCRoomItem * pGameRoomItem);

	//��ҳURL�仯��⣬JianGuankun 2012.06.26
	void BeforeNavigate2Explorer(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
	void NavigateErrorExplorer(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Frame, VARIANT* StatusCode, BOOL* Cancel);

	//��������仯��JianGuankun 2012.06.26
	void ProcessMyAvatarChange(UINT uNewLogoID);

	///���������Ľ��뷿�亯�� lxl 2010-10-08
	bool EnterRoom(CAFCRoomItem * pGameRoomItem);
	//������������������� zxj 2009-11-16
	void OnLockAccountResult( NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

	void ShowCharmPannel();


	bool GetInGame();

	//��ȡ��������
	UINT GetonlineUserCount();
private:
	//������������
	void SetonlineUserCount(UINT onlineUserNum);
	//���ܺ���
public:
	//�ر����з���
	bool CloseAllGameRoom();
	//�滭���
	void DrawViewFrame(CDC * pDC, int iWidth, int iHeight);

    // PengJiLin, 2010-9-13, ���˿������߿����ܣ�����ʹ��ʱ��
    void SetKickPropTime(int iKickTime);
    void SetAntiKickPropTime(int iAntiKickTime);

	string JoinUrl(string p1, string p2);
	void char2Tchar(char *pchar,TCHAR * tchar);

    // PengJiLin, 2011-6-23, ������Ϣ
    void SetActiveToMainRoom(UINT uState);

	RoomInfoStruct* GetCurrentGameRoom();


#ifdef MY_QUICK_GAME_ON

	//JianGK,����ͨѶ�ܵ�
	bool OpenQuickGameNamedPipe(char* szPipeName);

	//JianGK,�ر�ͨѶ�ܵ�
	bool CloseNamedPipe();

	//JianGK,�ܵ�ͨѶ�߳�
	//static DWORD WINAPI NamedPipeThread(LPVOID param);
	static UINT AFX_CDECL NamedPipeThread(LPVOID param);

#endif // MY_QUICK_GAME_ON

public:
	CPassDesk pdDlg;
	//��Ϣ����
public:
	//��ʼ��
	void Init(const CString& strLogonUserID = _T(""));
	//��ҳ��ť
	void OnWeb();
	//�޸�����
	//void OnChangeInfo();
	//������Ϸ����
	void OnInGame();
	//�˳���Ϸ
	void OnQuitPlace();
	//�رշ�������
	LRESULT OnCloseGameRoom(WPARAM wparam, LPARAM lparam);

	//wushuqun 2008.10.14
	LRESULT OnCloseGameRoomWithMsg(WPARAM wparam, LPARAM lparam);
	
	
	LRESULT OnLoadSkin(WPARAM wparam, LPARAM lparam);
	LRESULT OnExchangeSkin(WPARAM wparam, LPARAM lparam);
//���ع������Ű�ť
	//void OnBnClickedHideNews();  //ON_COMMAND(IDM_CONNECT_CENTER_SERVER, OnConnectToCenterServer)
	//�����б�ť
	//void OnBnClickedHideList();  //ON_COMMAND(IDM_CONNECT_CENTER_SERVER, OnConnectToCenterServer)

	afx_msg void OnConnectToCenterServer();
	//��¼��Ϸ����
	afx_msg void OnLogonPlace();
	//�һ���Ϸ�б�
	//afx_msg void OnNMRclickGameList(NMHDR * pNMHDR, LRESULT * pResult);
	//˫����Ϸ�б�
	//afx_msg void OnNMDblclkGameList(NMHDR * pNMHDR, LRESULT * pResult);
	//��Ϸ�б���ı�
	//afx_msg void OnTvnSelchangedGameList(NMHDR * pNMHDR, LRESULT * pResult);
	//�б�����չ��
	//afx_msg void OnTvnItemexpandingGameList(NMHDR * pNMHDR, LRESULT * pResult);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//�ػ�����
	afx_msg void OnPaint();
	//��ʾ����
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//��С����ť
	//void OnBnClickedMin();
	//�رհ�ť
	//void OnBnClickedClose();
	//afx_msg void OnBnClickedSkin();

//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	//afx_msg void OnBnClickedShowmenu();
public:
	//afx_msg void OnBnClickedBt6();
public:
	//afx_msg void OnBnClickedBt7();
public:
	void LoadSkin(const CString& strLogonUserID = _T(""));
	//��ʾ�����������
	LRESULT OnShowLeft(WPARAM ,LPARAM);

	LRESULT OnTopTabSelected(WPARAM wparam,LPARAM lparam);
	LRESULT OnTopTabLostSelected(WPARAM wparam,LPARAM lparam);

	//��IE
	LRESULT OnOpenIE(WPARAM ,LPARAM);

	LRESULT OnCallGM(WPARAM wParam, LPARAM lParam);

	LRESULT OnOpenBank(WPARAM wParam, LPARAM lParam);

	//Ͷ��  yjj 090310
	LRESULT OnAppeal(WPARAM ,LPARAM);
	LRESULT OnLockAccount(WPARAM wParam, LPARAM lParam);	//�������� zxj 2009-11-12

	
	//�ϴ�ͷ��
	LRESULT OnUpLoadUserLogo(WPARAM,LPARAM);
//������ͷ��
void checkUserLogo(long dwUserID);
public:
	bool StartDownLoad(TCHAR filename[],CAFCRoomItem * pGameRoomItem);

	bool CheckUpdate(TCHAR filename[],CAFCRoomItem * pGameRoomItem);
public:
	//��÷�����Ϣ�ṹָ��
	bool GetRoomInfoStruct(RoomInfoStruct** pRoomInfoStruct);
	//���±�����Ϣ
	BOOL UpdateMeInfo(__int64 i64Money,BOOL IsUpdateMoney,int dwFascination,BOOL IsUpDateFascination);
	//

	//��������GameFrame�����н������Ϣ
	//������Ҵ򿪶����Ϸ���У�����һ�������б仯���ѱ仯ͬ����������������д���
	//JianGuankun 2012.1.5
	void UpdateBankMoneyToAllRoom(__int64 i64Money,CBaseRoom* pLaunchRoom = NULL);

	void ChangeImageList(COLORREF color);
public:
	void OpenBulletin(void);
public:
	void propGetListRequest(void);
public:
	//��ȡ��������غ��� zxj 2009-11-12 ��������
	static CString innerGetMac();			//ȡ������Ӳ����ַ
	static CString coreGetCode();	//ȡ�û�����

public:
	void	OnSwitchLockButton();	//�л�����������ť״̬

	void OnSendMobileVCodeSms(char* szMobileNo,UINT uType);			//�����ֻ���֤����� JianGuankun 2012.5.16
	static bool	ValidMobileVCode(char* szVCode);					//��֤�û�������ֻ���֤���Ƿ���ȷ JianGuankun 2012.5.16
	static void ClearMobileVCode();									//�����ֻ���֤�룬ʹ��֤����� JianGuankun 2012.5.16
	static CString GetUrlValue(CString szUrl, CString szField);		//��ȡURL����

	//�����ܵ��ص� �� �ӿ� 2012.08.20 yyf
	virtual void ReadAnonymousPipe(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead);//2012.08.17 yyf
	virtual  void ReadSimpleSocket(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead);//���ؽ��̼����� �ص��ӿ� 2012.08.24 yyf
	static DWORD WINAPI DownLoadUpdate(LPVOID lparam);//���ظ����߳�  2012.08.24 yyf

private:
	CSkinMgr m_skinmgr;
};

//��ȡ��Ϸ����ָ��
extern CMainRoomEx * GetMainRoom(void);

#endif