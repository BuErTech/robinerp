#pragma once
#ifndef CGAMEFRAMEDLG_HEAD_FILE
#define CGAMEFRAMEDLG_HEAD_FILE

#include "AfxTempl.h"
#include "AFCButton.h"
#include "AFCWindow.h"
#include "AFCDialog.h"
#include "MessageHandle.h"
#include "UserListDlg.h"
#include "TCPClientSocket.h"
#include "PlayView.h"
#include "FlashViewDlg.h"
#include "AFCWeb.h"
#include "AFCTabCtrl.h"
#include "DlgLeftShow.h"
#include "PlaySound.h"
#include "GetPoint.h"
#include "GameRoomMessage.h"
#include "PersonalInfo.h"
//#include "userprop.h"
#include "shortmessage.h"
#include "CommonUse.h"
#include "GamePlaceMessage.h"
//#include "BuyItem.h"
//#include "HoldItemDlg.h"
//#include "BroadcastMsgInputDlg.h"
//#include "BuyPropDialog.h"	//2008/04/02
#include "PropSelUser.h"
#include "explorer1.h"  //IE �������Fred Huang 2008-06-02
//#include "MjSet.h"
//#include "CardSet.h"
/*******************************************************************************************************/

//������Ϣ����
#define CM_USER_STATE					1						//״̬�ı�
#define CM_USER_SEND_TIMES_MONEY       2                       //�û���ð������͵Ľ��

#define CM_USER_PROP					12							//������Ϣ
#define CM_USER_TROTTING				13						//�����������Ϣ

//Unity3d��Ϣ����
#define CM_U3D_GAMEBASEINFO				15	
#define CM_U3D_APPLICATION_QUIT			16

#define CM_EX_SKIN						17

#define IDM_GETPOINT						WM_USER + 430				//����
#define IDM_GET_ROOMNAME					WM_USER+435					//�õ���������
#define IDM_CLOSE_GAME						WM_USER+436					//���߸������Ѿ��ر�

#define IDM_BIG_BROADCAST_MSG				WM_USER + 437				//��㲥
#define IDM_QUIT_ROOM_MSG					WM_USER + 438				//JMod-�˳���Ϸ�����������йܴ���-20090513

//��������
#define ACT_USER_UP						1						//�û�����
#define ACT_WATCH_UP					2						//�Թ�����
#define ACT_USER_SIT					3						//�û�����
#define ACT_WATCH_SIT					4						//�Թ�����
#define ACT_USER_CUT					5						//�û�����
#define ACT_USER_AGREE					6						//�û�ͬ��
#define ACT_GAME_BEGIN					7						//��Ϸ��ʼ
#define ACT_GAME_END					8						//��Ϸ����
#define ACT_USER_POINT					9						//�û�����ֵ
#define ACT_USER_CHARM					10						//�û�����ֵ
#define ACT_USER_SEND_TIMES_MONEY		11                      //�û���ð����͵Ľ��

#define ACT_USER_BANKMONEY				14						//�����ܽ�����ĸ���
#define ACT_USER_CONTESTPOINT			15						//�û�����ֵ
#define ACT_USER_QUEUE_DISMISS			18						//�Ŷӻ���ɢ
#define ACT_USER_AVATAR_CHANGE			19						//�������ı�

//wushuqun 2009.9.5
//�޸ģ�1.���������ȫ����ʼ��Ŵ���Ϸ���ڽ���
//      2.������Ϸ����δ��ʱ���޷����մ�������Ϣ�������Ƚ���Ϸ���������������أ���ȷ���ܽ�����Ϣ
//      3.����ҵ���������ϵĿ�ʼ��ťʱ������Ϸ���ģ��һ����Ϣ����������Ϸ�е��˿�ʼ��ť����Ϣ
//      4.��ҵ���������ϵĿ�ʼ��ť
#define ACT_USER_HITDESKBEGIN           14

#define ID_GIF_TIMER					50						//gif����
#define ID_GIF_TIMER2					51						//gif����

//�û�״̬�ı�
struct CM_UserState_Change
{
	BYTE								bActionCode;			//��������
	BYTE								bDeskStation;			//����λ��
	UserItemStruct						* pUserItem;			//�û�ָ��
};
//send to game talking
struct CM_UserState_Send_Times_Money : public CM_UserState_Change
{
	int dwUserID;
    int                                 dwGetMoney;             // �����Ľ����
    int                                 dwMoneyOnTimes;         // ���ö���ʱ�����͵Ľ��
    int                                 dwMoneyOnCounts;        // ���ö��پ����͵Ľ��
    int                                 dwTimesNeed;            // ��һ�β����ʱ��
    int                                 dwCountsNeed;           // ��һ�β���پ�
};
/*******************************************************************************************************/

//��Ϸ��ܽӿ�
interface IFrameInterface
{
	//��������
public:
	//��ʼ�����״�����ʹ�ã�
	virtual bool InitFrame(GameInfoStruct * pGameInfo, CWnd * pGameRoom)=0;
	//��������
	virtual bool AFCCreateFrame()=0;
	//�رմ���
	virtual bool AFCCloseFrame()=0;
	//��ʾ����
	virtual void AFCShowFrame(int nShow)=0;
	//����windows��Ϣ
	virtual LRESULT SendWindowMessage(UINT msg,WPARAM wParam,LPARAM lParam)=0;
	//ɾ������
	virtual void DeleteFrame()=0;
	//������Ϣ 
	virtual UINT OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize)=0;
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)=0;
	//������Ϸ������Ϣ
	virtual void ShowMsgInGame(TCHAR * szMsg, BYTE bNewOrSys=0,UINT uSize=0, TCHAR * szFontName=NULL,TCHAR *Title=NULL)=0;
	//��������Ϣ
	virtual void ShowMsgDudu(TCHAR * szSendName, TCHAR * szTargetName,TCHAR * szCharString, COLORREF crTextColor, bool bShowTime, UINT uSize=0, TCHAR * szFontName=NULL,int iDuduType=0)=0;
	//����������Ϣ
	virtual void SendShortMessage(MSG_GR_RS_NormalTalk *pShortMessage)=0;
    //���ε����е�����Ϸ��صİ�ť����ʾ������ҵ���,¼��ר�ýӿ� add by yjj 090224
	virtual void SetRecordView() = 0;
	//��ȡ��Ϸ�Ƿ�����
	virtual bool GetGameIsStarting() = 0;
};

/*******************************************************************************************************/

//������Ϣ���ͽӿ�
interface ISocketSendInterface
{
	//��������
public:
	//����������Ϣ����
	virtual int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)=0;
	//����������Ϣ����
	virtual int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)=0;
};

/*******************************************************************************************************/

//�����û��ӿ�
interface IFindUserInterface
{
	//��������
public:
	//�����û�
	virtual UserItemStruct * FindOnLineUser(long int dwUserID)=0;
	//�����û�
	virtual UserItemStruct * FindOnLineUser(int dwAccID)=0;
	//�����û�
	virtual INT_PTR FindOnLineUser(const TCHAR * szUserName, CPtrArray & ResultPtrArray)=0;
	//�����û�
	virtual INT_PTR FindOnLineUser(BYTE bDeskIndex, CPtrArray & PlayPtrArray, CPtrArray & WatchPtrArray)=0;
};

interface ISortMessageUserInterface
{
public:
	virtual bool GetSendMsgUserStruct(long int lUserID, SendMsgUserStruct * user)=0;
	virtual SendMsgUserStruct * GetMeUserInfo()=0;
	virtual int SendData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)=0;
};
/*******************************************************************************************************/

//��Ϸ������� 
class EXT_CLASS CLoveSendClass/*C-GameFrameDlg*/ : public CLoadFileBmp, public IFrameInterface
{
	POINT							m_SavedScreen;
	bool							m_bMax;								//�Ƿ����
	bool							m_bShowMax;							//�Ƿ���ʾ���

	void NavigateVirFace();
	UserInfoStruct * m_CurpUserInfoClicked;
	bool	m_bInit;

	int								m_nTalkDefaultCount;

	//////////////////////////////////////////////////////////////////////////
	//���������ֿ��ƣ�Fred Huang 2008-7-24
	int						m_sndListID;
	CNormalBitmapButtonEx	m_btSndPlay;
	CNormalBitmapButtonEx	m_btSndPause;
	CNormalBitmapButtonEx   m_btVolDown;
	CNormalBitmapButtonEx	m_btVolUp;

	int						m_sndBtnCx,m_sndBtnCy;

	int sndPlayX,sndPlayY;
	int sndPauseX,sndPauseY;
	int sndVolUpX,sndVolUpY;
	int sndVolDownX,sndVolDownY;

	void OnSndPlay();
	void OnSndPause();
	void OnSndVolDown();
	void OnSndVolUp();

	bool m_bSndPause;
	//////////////////////////////////////////////////////////////////////////


	//��������
protected:
	void DoDataExchangeWebVirFace(CDataExchange * pDX);
	CWnd								* m_pGameRoom;					//���䴰��
	CUsingSkinClass						* const m_pGameView;			//��Ϸ��ͼ

	HBRUSH m_bkBrush;

	//��Ϸ����
protected:
	bool								m_bSaveView;					//�������
	bool								m_bWatchMode;					//�Թ�ģʽ
	bool								m_bWatchOther;					//�����Թ�
	BYTE								m_bWaitTime;					//�ȴ�ʱ��
	CPtrArray							m_WatchPtrArray;				//�Թ�ָ������
public:
	GameInfoStruct						* m_pGameInfo;					//��Ϸ��Ϣָ��
	CExplorer2							m_ie;							//IE�����
protected:
	UserItemStruct						* m_pUserInfo[MAX_PEOPLE];		//��Ϸ�����Ϣ
	TCHAR								m_BackMusicIni[MAX_PATH+1];		//�������������ļ�
	TCHAR								m_BMusicKey[100];				//��������key

	CString								szAdvUrl;
	//���и�����Ϣ
private:
	long int							m_dwCurrentUserID;				//��ǰ�û�

	//���츨����Ϣ
private:
	CString								m_strLastTalk;					//�������
	long int							m_dwLastTalkTime;				//˵��ʱ��
	static CArray<LONG,LONG>			m_ShieldArray;					//�����û�

	//��ʱ��
private:
	UINT								m_uTimeID;						//��ʱ�� ID
	UINT								m_uTimeCount;					//��ʱ������
	BYTE								m_bTimeStation;					//������λ��
	COLORREF							m_FrameColor;					//�������ɫ
	int									m_titlex,m_titley;				//����λ��
	int									m_u1x,m_u1y,m_u1h, m_u1w;
	int									m_u2x,m_u2y,m_u2h,m_u2w;
	int									m_vlistx,m_vlisty;
	int									m_logx,m_logy;					//�û�ͷ��λ��
	COLORREF							m_titlecr,m_usercr;				//������û���Ϣ��ɫ
	/////////////////////////////
	//Kylin 20090205 �����û���Ϣ����
	COLORREF	m_userbkcr;
	/////////////////////////////
	CGameImage							m_backImage;
	CAFCColorResource					m_colorresource;
	CString								m_iniFile;						//�����ļ�·��
	CString								m_HidebtPath;
	CString								m_ShowbtPath;
	//�ؼ�����
public:
	CDlgLeftShow						m_DlgShowLeft;					//�����뿪��ʾ��
	CToolTipCtrl						m_ToolTip;						//������ʾ
	//	CControlMessage						m_SplitBar;						//�������
	CControlMessage						m_HorSplitBar;					//������
	//CIEHelper							m_ADDialog;						//��洰��
	CNewMiniDlg							m_TalkMessage;					//������Ϣ
	CPlaySound							m_PlaySound;					//��������
	CGetPoint							m_DlgGetPoint;					//�����
	CComboBox							m_InputMessage;					//������Ϣ
	//	CComboBox							m_TargetPeople;					//Ŀ���û�

	CNormalBitmapButtonEx				m_BtSend;						//���Ͱ�ť
	CNormalBitmapButtonEx				m_BtExpression;					//���鰴ť
	CNormalBitmapButtonEx				m_BtGameQuit;					//�˳���ť
	CNormalBitmapButtonEx				m_BtSetColor;					//��������������ɫ

	CGameTalkText						m_ExpressWnd;					//���鴰��
	CPictureHelper						m_UserListDlg;					//�û��б�
	CLookingClass						m_MessageHandle;				//��Ϣ����

	CNormalBitmapButtonEx				m_Btbt1;						//
	CNormalBitmapButtonEx				m_Btbt2;						//
	CNormalBitmapButtonEx				m_Btbt3;						//
	CNormalBitmapButtonEx				m_Btbt4;						//
	CNormalBitmapButtonEx				m_Btbt5;						//
	CNormalBitmapButtonEx				m_Btbt6;						//
	CNormalBitmapButtonEx				m_Btbt7;						//
	CNormalBitmapButtonEx				m_Btbt8;						//
	CNormalBitmapButtonEx				m_Btbt9;						//
	CNormalBitmapButtonEx				m_Btbt10;						//

	//{add by zxj 2008/04/07
	CNormalBitmapButtonEx				m_BtPropGoAhead;	
	CNormalBitmapButtonEx				m_BtPropGoNext;	
	CNormalBitmapButtonEx				m_BtProp1;
	CNormalBitmapButtonEx				m_BtProp2;
	CNormalBitmapButtonEx				m_BtProp3;
	CNormalBitmapButtonEx				m_BtProp4;
	UINT								m_BtPropArray[4];			//Ϊ�˷���,���Խ��ĸ���ť�ŵ�������

	//int		m_nIndex1, m_nIndex2, m_nIndex3, m_nIndex4;				//���߰�ť��Ӧ�ڵ������������
	//TCHAR	m_szPropImage[PLATFORM_ITEM_KIND][MAX_PATH];			//����ÿ�����͵��ߵ�ͼƬ
	//TCHAR	m_szPropToolTip[PLATFORM_ITEM_KIND][MAX_PATH];			//������ʾ��Ϣ
	TCHAR	m_szPropEmptyImage[MAX_PATH];							//�յ��߻���߼���Ϊ0ʱ��ͼƬ·��
	CRect	m_UserFrameRectArray[MAX_PEOPLE];						//��¼�û�λ�õĿ�����
	//	int		m_itemIndex[PLATFORM_ITEM_KIND];						//����λ������
	//	int		m_useMaxItem;											//�û���ʹ�õ����ĵ���
	int		m_curPropPage;												//��ǰ����ҳ

	void OnInitPropList();											//���յ������б���Ϣʱ��ʼ������
	void OnSetPropImages();											//���õ��߰�ťͼƬ
	//	void OnSetPropItemImage(int index);										//���õ������߰�ť
	//	void OnSetPropToolTips();										//���õ�����ʾ��Ϣ
	BOOL OnToolTipNotify(UINT id, NMHDR * pTTTStruct, LRESULT * pResult );	//��ӦToolTip��Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//	LRESULT OnHandlePlayViewMsg(WPARAM wParam, LPARAM lParam);
	void OnUpdateUserPropOnLine(int nPropIndex, int nUpdateNum);	//���߸����û��ĵ���nPropIndex��ʾ����ID��nUpdateNum��ʾ����������ʾ��������ʾ��
	BOOL OnUseProp(int nPropIndex, int nDeskUserID);				//ʹ�õ���
	//	void OnFlashPropToolTip(int nPropIndex);
	enum{PEER_PAGE=4};
	//typedef struct PropData											//����ֱ�Ӷ���tgItemData������֪ԭ��ı���
	//{																//����ֻ���ڴ˶���һ����tgItemData��ͬ�Ľṹ��
	//	int		aItemCount[PLATFORM_ITEM_KIND];				//���ߵ�����				
	//	TCHAR	aItemName[PLATFORM_ITEM_KIND][61];			//��������
	//	TCHAR	aItemURL[PLATFORM_ITEM_KIND][128];			//��ʾ��WEB��ַ
	//	TCHAR	aItemUnit[PLATFORM_ITEM_KIND][16];			//��ʾ���ߵĵ�λ
	//}_PropItem, *LPPropItem;
	//_PropItem	m_PropItems;								//�û�ӵ�еĵ���
	//	int			m_nCurSelPropID;							//��ǰѡ�еĵ��ߣ���ʼ��Ϊ-1����ʾû�е��߱�ѡ��
	CPropSelUser *hPropSelUser;
	//end add}


	//CNormalBitmapButtonEx				m_BtGameSet;					//���ð�ť
	CNormalBitmapButtonEx				m_BtHide;						//����
	CNormalBitmapButtonEx				m_BtExit;						//��ť(��)
	CNormalBitmapButtonEx				m_BtMin;						//��ť
	CNormalBitmapButtonEx				m_btMax;						//���
	CNormalBitmapButtonEx				m_btRest;						//��ԭ

	int									m_iStatic1X,m_iStatic1Y;
	CRect								m_UserInfoRect,m_UserListRect,m_TalkRect;
	//����ͼ
	CGameImage							m_rc,m_lc,m_tc,m_bc;									//�ĸ���
	CGameImage							m_lt,m_rt,m_lb,m_rb;									//�ĸ���
	//�ұ�
	CGameImage							m_right_rc,m_right_lc,m_right_tc,m_right_bc;			//�ĸ���
	CGameImage							m_right_lt,m_right_rt,m_right_lb,m_right_rb;			//�ĸ���
	//�м�
	//CGameImage							m_iMid;													//�м�
	int									m_userlistW,m_userlistH,m_talkW,m_talkH;
	int									m_talkX,m_talkY;										//������XY
	TCHAR								m_szGameRoomName[61];

	PropStruct							m_stProp ;						//��ǰ���Ŷ������;

	static bool						m_bAutoSit;							//�Ƿ��Զ�
	static int						m_iMaxShortWnd;						//��ര��
	CPtrArray						m_ShortMessage;						//��Ϣ����
	//CBuyItem						*m_pBuyItemdlg;						//�������
	//	CHoldItemDlg					*m_pHoldItemdlg;					//���е��ߴ���
	//	CBroadcastMsgInputDlg			*m_pBroadcastDlg;					//С�԰ȴ���

	//	CBuyPropDialog					*m_pBuyPropDlg;						//��������б���ͼ2008/04/02 zxj

	//��Ϸ�м����źŵƹ���

	void checkNetSignal(void);
	void getNetSignal(void * pNetData);
	void drawSignal(void);
	int m_NetSignalIndex;                //����������Ϣ������
	int m_SavedSingalIndex;     //��һ�η��͵�������Ϣ�����ţ��յ���Ϣ����m_NetSingalIndexͬ��
	int m_NetSignalLevel;                //�����źż���
	Image  *pSingnalimg;

public:
	CAFCColorResource colorresource;		//����,ϵͳ��ɫ�ṹ
	void SetFillColor(COLORREF rgb);		//�������ɫ

	void UserInfoMenu(UserInfoStruct * pUserInfo);		//�Ҽ���ʾ��Ϣ

	void DrawCurUserInfo(CDC *pDC);

	//��������
protected:
	//���캯��
	CLoveSendClass(CUsingSkinClass * pGameView);
	//��������
	virtual ~CLoveSendClass();

	//���غ���
protected:
	// ֹͣ��������
	void StopBackMusic();
	// ������������
	void ContinueMusic();
	// ���ñ�������
	void SetBackMusic(LPCTSTR filepath,LPCTSTR key=TEXT("BackSound"));
	//���ھ�����ʾ
	virtual void ShowMaxSize();
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ż��󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��Ϣ���ͺ���
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�˵���Ϣ
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//�س���Ϣ
	virtual void OnOK() { return; }
	//�ܷ��뿪��λ
	//virtual BOOL CanLeftDesk(){return true;};
	//�˳�����
	virtual void OnCancel();
	//������Ƶ���øı�
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//�������غ���
protected:
	//��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�������Ϣ������
	virtual bool HandlePointMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	////ʹ�õ���
	//virtual bool OnUserUseItemMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϣ
	virtual bool OnPropMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//virtual bool OnItemDataMsg(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϣ
	virtual bool OnPropVipCheckTime(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//vip
	//virtual bool OnPropVipCheckTime(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�㲥
	//	virtual bool OnBroadcastMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//���е���
	//virtual bool /OnUpdateHoldItemResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//ʹ�õ���
	virtual bool OnUsePropResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//���Թ�������б�
	//	virtual bool OnGetBuyItemResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������߽��
	//	virtual bool OnBuyItemResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

	//������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize)=0;
	//��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)=0;
	//�Թ۸ı�
	virtual void OnWatchSetChange()=0;
	//�������ý���
	virtual void ResetGameFrame();
	//����������Ϸ����
	virtual void ResetGameStation(int iGameStation=1){};
	//���ú���
	virtual void OnGameSetting() {};
	//����
	virtual bool UserProp(_TAG_PROP_MOIVE_USE *propMoive);
	//virtual bool UserProp(BYTE bsendDeskStation,BYTE brecvDeskStation,int iItemID);
	//����
	virtual bool PlayNextGif();
    //¼��ϵͳ�������á�
	virtual void SetRecordView() {};

	virtual long int GetIDByNickName(TCHAR *pszNickName);
	//�õ���ǰѡ���λ��
	BYTE GetDestStation(TCHAR szUserName[]);
	//��Ϸ���ƺ���
protected:
	//ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin()=0;
	//��ȫ������Ϸ
	virtual bool OnControlSafeFinish()=0;

	//���ܺ���
public:
	void SetInifile(LPCTSTR path);
	//�����
	void ActiveClient();

	virtual void ShowUserTalk(MSG_GR_RS_NormalTalk * pNormalTalk){return ;}
	//���Ż��㵽��ͼλ��
	virtual BYTE ViewStation(BYTE bDeskStation);
	//����û�����ֵ
	bool CheckUserPoint(int dwChangePoint);

	void LoadSkin(const CString& strFileName = "",const CString& strSkinKey = "",const CString& strSkinPath = "");
	//��ȡ�Լ���Ϣ
	UserInfoStruct * GetMeUserInfo() { return &m_pGameInfo->pMeUserInfo->GameUserInfo; };
	//��������
	void PlayGameSound(TCHAR * szFileName);
	//��������
	void PlayGameSound(HINSTANCE hInstance, TCHAR * szResourceName);
	//���ö�ʱ��
	void SetGameTimer(BYTE bDeskStation, UINT uMaxTime, UINT uTimeID);
	//ɾ����ʱ��
	void KillGameTimer(UINT uTimeID);
	//��ȡ��ʱ��λ��
	BYTE GetGameTimeStation() { return m_bTimeStation; };
	//���Ҽ��˵�
	void OpenUserMenu(UserItemStruct * pUserItem);
	//����״̬����
	void SetStationParameter(BYTE bGameStation) { m_pGameInfo->bGameStation=bGameStation; };
	//��ȡ״̬����
	BYTE GetStationParameter() { return m_pGameInfo->bGameStation; };
	//����������Ϣ����
	int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//����������Ϣ����
	int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);

	//ע�����
public:
	void OnLBClickUserList(UserItemStruct * pUserItem);
	void OnLBDBClickUserList(UserItemStruct * pUserIte);
	//д����ֵ
	BOOL WriteProfileInt(TCHAR * szEntry, int iDefault);
	//��ȡ�ַ���
	BOOL WriteProfileString(TCHAR * szEntry, TCHAR * szValue);
	//��ȡ��ֵ
	UINT GetProfileInt(TCHAR * szEntry, int iDefault);
	//��ȡ�ַ���
	CString GetProfileString(TCHAR * szEntry, TCHAR * szDefault);

	//�ڲ����⺯��������Ҫ���أ�
protected:
	//���±���
	virtual void UpdateGameTitle();
	//�û�����
	virtual bool GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);
	//�û��뿪
	virtual bool GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);
	//�û�ͬ��
	virtual bool UserAgreeGame(BYTE bDeskStation);
	//����λ��
	virtual void FixControlStation(int iWidth, int iHeight);
	//��ܺ���
	virtual bool OnFrameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//��Ϣ����
	virtual bool OnSystemMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�ӿں���������Ҫ���أ�
private:
	//��ʼ��
	virtual bool InitFrame(GameInfoStruct * pGameInfo, CWnd * pGameRoom);

	//��������
	virtual bool AFCCreateFrame();
public:
	//�رմ���
	virtual bool AFCCloseFrame();
	//��ʾ����
	virtual void AFCShowFrame(int nShow);
private:
	//������Ϣ 
	virtual UINT OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize);
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//ɾ������
	virtual void DeleteFrame();
	//��Ϣ����
	virtual LRESULT SendWindowMessage(UINT msg,WPARAM wParam,LPARAM lParam);
public:
	//������Ϸ������Ϣ
	virtual void ShowMsgInGame(TCHAR * szMsg,BYTE bNewOrSys=0, UINT uSize=0, TCHAR * szFontName=NULL, TCHAR *Title=NULL);
	//��������Ϣ
	virtual void ShowMsgDudu(TCHAR * szSendName, TCHAR * szTargetName,TCHAR * szCharString, COLORREF crTextColor, bool bShowTime, UINT uSize=0, TCHAR * szFontName=NULL,int iDuduType = 0);
	//����������Ϣ
	virtual void SendShortMessage(MSG_GR_RS_NormalTalk *pShortMessage);
	//������Ϣ����
	CShortMessage * ActiveShortWnd(long int dwUserID, UserItemStruct * pUserItem, bool bCreate);
protected:
	//����������Ϣ
	void OnBnClickedSendTalk();
	//���鰴ť
	void OnBnClickedExpression();
	void OnBnClickedSetcolor();
	//������ť
	void OnBnClickedGameRoom();
	//�˳���ť
	void OnBnClickedQuitGame();
	//��С����ť
	void OnBnClickedMin();
	//��󻯰�ť
	void OnBnClickedMax();
	//��ԭ��ť
	void OnBnClickedRest();

	void OnBnClickedUp();

	void OnBnClickedDown();

	void OnBnClickedEnd();	
	//====����,����,δβ��ť

	void OnBnClkGameBt01();

	void OnBnClkGameBt02();

	void OnBnClkGameBt03();

	void OnBnClkGameBt04();

	void OnBnClkGameBt05();

	void OnBnClkGameBt06();

	void OnBnClkGameBt07();

	void OnBnClkGameBt08();

	void OnBnClkGameBt09();

	void OnBnClkGameBt10();

	//{add by zxj 2008/04/07
	void OnBnClkPropGoAhead();
	void OnBnClkPropGoNext();
	void OnBnClkProp1();
	void OnBnClkProp2();
	void OnBnClkProp3();
	void OnBnClkProp4();
	//end add}

	//���ð�ť
	//void OnBnClickedSetting();


	//�����б�ť
	void OnBnClickedHideList();
	//������Ϣ
	LRESULT OnExpression(WPARAM wparam, LPARAM lparam);
	//������Ϣ
	LRESULT OnRichEditHit(WPARAM wparam, LPARAM lparam);
	//�ر���Ϸ
	LRESULT OnCloseGame(WPARAM wparam, LPARAM lparam);
	//�������Ϣ
	LRESULT OnSplitMessage(WPARAM wparam, LPARAM lparam);

	LRESULT OnLoadSkin(WPARAM wparam, LPARAM lparam);
	//�ض�talkIngame.ini
	LRESULT OnReLoadTalkINI(WPARAM wparam, LPARAM lparam);
	//���ͻ���
	LRESULT OnGetPoint(WPARAM wparam,LPARAM lparam);
	////////////////////////////
	///Kylin 20090122 ʹ�õ���
	LRESULT OnUserItem(WPARAM wparam,LPARAM lparam);
	////////////////////////////
	//��ȡ��ǰ���е���
	//LRESULT OnGetHoldItemData(WPARAM wparam,LPARAM lparam);
	//ʹ�õ���
	//	LRESULT OnUserUseItemData(WPARAM wparam,LPARAM lparam);
	//��ȡ�ܹ���������б��۸�
	//	LRESULT OnGetBuyItemData(WPARAM wparam,LPARAM lparam);
	//�������
	//	LRESULT OnBuyItemData(WPARAM wparam,LPARAM lparam);
	//ʹ������
	//LRESULT //OnUseBroadcastMsg(WPARAM,LPARAM);
	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//λ�ñ仯
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//�����Ҽ���Ϣ������
	afx_msg void OnRButtonDown(UINT   nFlags,   CPoint   point);
	//������������Ϣ	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//����ѡ��仯
	afx_msg void OnCbnSelInput();
	DECLARE_MESSAGE_MAP()
public:
	int GetComType(void);
public:
	void LoadImage(void);
	//����㲥��Ϣ
	//virtual bool OnBroadCastMsg(int iItemID);

	//���ƿ��
	void DrawViewFrame(CDC * pDC, int iWidth, int iHeight);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	void UsePropItem(int propIndex);
public:
	//	afx_msg void OnNcPaint();
public:
	void LoadSoundList(void);
public:
	void SendTalkMessage(int nDefaultIndex=0);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//����
	void OnExchangeSkin(const CString& strFileName,const CString& strSkinKey,const CString& strSkinPath);
};


class CGlobal//ȫ����
{
public :
	CGlobal()
	{
		m_CenterServerPort=CENTER_SERVER_PORT;//���ķ���˿�
		CString s=CBcfFile::GetAppPath (false);/////����·��
		CBcfFile forder( s + "Order.bcf");
		m_nPowerOfGold = forder.GetKeyVal("VirtualMoney", "PowerOfGold", 0);

        // PengJiLin, 2010-8-3, �����ʾ�ָ�������
        m_bUseSpace = forder.GetKeyVal("VirtualMoney", "UseSpace", 0);
        CString strSpace = forder.GetKeyVal("VirtualMoney", "SpaceChar", " ");
        memcpy(m_strSpaceChar, strSpace, sizeof(m_strSpaceChar)*sizeof(TCHAR));

		int _bit = forder.GetKeyVal("VirtualMoney", "SpaceBits", 3);
		if (_bit == 0) m_iMaxBit = 18;
		else m_iMaxBit = 18 + 18 / _bit - 1;

		CBcfFile f( s + "bzgame.bcf");
		m_key="BZW";
		m_CenterServerIPAddr=f.GetKeyVal (m_key,"LoginIP","127.0.0.1");//������IP		

		m_title=f.GetKeyVal (m_key,"title","");//����
		m_TabName=f.GetKeyVal (m_key,"DeskName","");//��һ��ѡ�		
		m_LoginTit=f.GetKeyVal (m_key,"LoginTitle","");//���õ�¼����ı���
		m_TreeCount=f.GetKeyVal (m_key,"TreeCount",1);//��������,Ĭ����1Ϊ�˲��ԣ���������INI��ָ��Ϊ0
		if(m_TreeCount>10)
			m_TreeCount=10;
		for (int i=0;i<m_TreeCount;i++)
		{
			s.Format("%d",i); 
			m_Tree[i]=f.GetKeyVal (m_key,"Tree"+s,"%d");//��һ����������
			m_Tree[i+10]=f.GetKeyVal(m_key,"TreeURLid"+s,"");//��һ�����ĵ�ַ
			m_Tree[i+20]=f.GetKeyVal(m_key,"TreeIcon"+s,"");//��һ������ICON
		}
		m_TreeCount2=f.GetKeyVal (m_key,"TreeCount2",1);//��������,Ĭ����1Ϊ�˲��ԣ���������INI��ָ��Ϊ0
		if(m_TreeCount2>10)
			m_TreeCount2=10;
		for (int i=0;i<m_TreeCount2;i++)
		{
			s.Format("%d",i); 
			m_Tree2[i]=f.GetKeyVal (m_key,"Tree2"+s,"%d");//��һ����������
			m_Tree2[i+10]=f.GetKeyVal (m_key,"TreeURLid2"+s,"");//��һ�����ĵ�ַ
			m_Tree2[i+20]=f.GetKeyVal(m_key,"Tree2Icon"+s,"");//��һ������ICON
		}
		//m_manager=f.GetKeyVal (m_key,"manager","");
		//m_develop=f.GetKeyVal (m_key,"develop","");
		m_UpdateUrl=f.GetKeyVal(m_key,"update","");
		m_skin=f.GetKeyVal(m_key,"skin","skin0");
		//m_pastimeurl=f.GetKeyVal(m_key,"pastime","");
		m_urlpostfix=f.GetKeyVal(m_key,"urlpostfix","asp");
		m_NewUrl=f.GetKeyVal(m_key,"newurl","");
		m_release=true;
		m_autoreg=false;
		m_weblogon=false;
		m_szSPToken="sn";
		///////////////////////////////////////////////////////////
		///Kylin 20090107  ��������Ϸ�б�
		CString temp;
		CUIntArray  baTemp;
		int nLimit;

		temp=f.GetKeyVal(m_key,"Favor","");
		nLimit=f.GetKeyVal(m_key,"FavorLimit",10);
		if(temp!="")
		{
#define DELIMITERS ";"
			int curPos= 0;
			CString resToken;

			resToken= temp.Tokenize(_T(DELIMITERS),curPos);
			while (resToken != "")
			{
				UINT bValue;

				bValue=atoi(resToken.GetBuffer());					
				if(bValue)
					baTemp.Add(bValue);
				resToken= temp.Tokenize(_T(DELIMITERS),curPos);
			};
			if(baTemp.GetCount()>nLimit)
			{
				for(int i=0;i<nLimit;i++)
					m_baFavor.Add(baTemp[baTemp.GetCount()-nLimit+i]);
			}
			else
			{
				m_baFavor.Copy(baTemp);
			}
		}
		m_baFavorSave.Copy(m_baFavor);
		///////////////////////////////////////////////////////////

		//wushuqun 2009.6.26
		//�����Ƿ����������ͼ��
		bEnableUserType = f.GetKeyVal(m_key,"EnableUserType",0);

		bEnableDiamondUserType = f.GetKeyVal(m_key,"EnableDiamondUserType",0);//������ʯ��ݱ�ʶ add by huangYuanSong 09.07.14

		//wushuqun 2009.6.30
		//����֧�ֶ����йܵ���Ϸ,���ø�ʽΪ��
		//����������Ϸ���ߺ��Ƿ�֧���Զ��йܹ���,����Ϸ��ID��+ ';'�ָ�,���������Ϸ��֧�ֶ����йܣ�ֱ������ΪAll,��������Ϊ���¸�ʽ,��
		//  10003300;10000305;11200405
		m_bAllAbet = false;

		CString strAuto;
		strAuto = f.GetKeyVal("GameSet","AfterCutAutoInstead","");
		if (strAuto.IsEmpty())
		{
			m_cutAutoInSteadAry.RemoveAll();
		}
		else
		{
			//�����ַ���
			if (strAuto.MakeLower() == "all")
			{
				//������Ϸ��֧�ֶ��ߺ��Զ��й�
				m_bAllAbet = true;
			}
			else
			{
				//����ʽ�����ַ���
				for(int i = 0;i < strAuto.GetLength();i++)
				{
					if (strAuto.GetAt(i) == ';')
					{
						CString strTmp = strAuto.Left(i);
						int nId = atoi(strTmp);
						m_cutAutoInSteadAry.Add(nId);
						strAuto = strAuto.Right(strAuto.GetLength() - i - 1);
						i = 0;
					}
				}
			}
		}

	}
	virtual ~CGlobal()
	{
		//�ͷ�
		m_cutAutoInSteadAry.RemoveAll();
		//wushuqun 2009.7.7
		//�޸�bzwcore.bcf���ط�ʽ
		if (m_hRandVirUser)
			::cfgClose(m_hRandVirUser);
	}
	CenterServerMsg m_CenterServerPara;
	UINT m_CenterServerPort,m_TreeCount,m_TreeCount2,m_regFace;//,m_CS;
	CString m_key,m_URL,m_CenterServerIPAddr,m_title,m_TabName,m_LoginTit;
	CString m_Tree[30],m_Tree2[30];//���10������0-9�����������ƣ�10-19��������ַ��20-29��������ICO�ļ���ַ
	CString m_Btn[10];//���20��
	//	CString m_manager,m_develop;
	CString	m_szSPToken;//CookieSPToken,������֤�û���¼
	CString m_Path;
	CString m_UpdateUrl;
	bool	m_release;
	bool	m_autoreg;
	bool	m_weblogon;	
	CString	m_skin;
	CString m_sktext[5];
	//	CString	m_pastimeurl;
	CString m_urlpostfix;
	CString m_NewUrl;
	CString m_TML_SN;												//���ڴ��ֶ�û���õ����ʽ������ݻ����� zxj 2009-11-12 ��������
	CArray<_TAG_USERPROP*,_TAG_USERPROP*> userPropLibrary;							//�û�ӵ�еĵ���
	///////////////////////////////////////////////////////////
	///Kylin 20090107  ��������Ϸ�б�
	CUIntArray  m_baFavor;//��ʾ�������Ϸ�б�
	CUIntArray  m_baFavorSave;//��Ҫ����������Ϸ�б�
	///////////////////////////////////////////////////////////

	//wushuqun 2009.7.7
	//�޸�bzwcore.bcf���ط�ʽ
	//--------------------------------
	DWORD       m_hRandVirUser;

	static DWORD WINAPI InitVirUserThread(LPVOID lparam)
	{
		DWORD * hRandVir = (DWORD *)lparam;
		* hRandVir = cfgOpenFile("bzwcore.bcf");
		return 0;
	}
	BOOL  InitVirUserName()
	{
		//wushuqun 2009.6.19
		//�������еĳ�ʼ������������ַŵ�������
		::CreateThread(NULL,0,InitVirUserThread,&m_hRandVirUser,0,NULL);
		return TRUE;
	}
	//-------------------------------------

	//wushuqun 2009.6.26
	//�����Ƿ����������ͼ��
	bool        bEnableUserType;

	bool        bEnableDiamondUserType;//������ʯ��ݱ�ʶ add by huangYuanSong 09.07.14


	//wushuqun 2009.6.30
	//����֧�ֶ����йܵ���Ϸ
	CArray<UINT>   m_cutAutoInSteadAry;  
	//�Ƿ���������Ϸ��֧�ֶ��ߺ��Զ��й�
	bool           m_bAllAbet;
	/// ��ʾ��ҽ�ң����н�ҵȱ�ʾ������ֵʱ��������ϼ�����
	/// 123�������ʾΪ123000�����ֵΪ3
	int			m_nPowerOfGold;		

    // PengJiLin, 2010-8-3, �����ʾ���������Ƿ��зָ������Լ�ʲô���ķָ���
    bool        m_bUseSpace;        // true = �зָ���
    TCHAR       m_strSpaceChar[10]; // ������ĸ�ķָ���

	int			m_iMaxBit;
public:
	CString PropIDtoName(int ipropid)
	{
		if (ipropid == 0) return "���";
		_TAG_USERPROP *userProp=NULL;
		for (int i = 0; i < userPropLibrary.GetCount(); i++)
		{
			userProp = userPropLibrary.GetAt(i);
			if (userProp != NULL && userProp->nPropID == ipropid)
			{
				return userProp->szPropName;
			}
		}
		return "";
	}

};



void char2Wchar(char* pchar,wchar_t* wchar);



static CGlobal g_global;

__declspec( dllexport ) CGlobal& Glb();//{return g_global;}
//static CGlobal g_global;

//static CGlobal& Glb(){return g_global;}


/*******************************************************************************************************/

#endif
