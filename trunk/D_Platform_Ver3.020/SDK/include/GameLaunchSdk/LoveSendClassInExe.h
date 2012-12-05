#pragma once

#include "Platform/AFCDevHead.h"
#include "Platform/explorer1.h"  //IE �������Fred Huang 2008-06-02
#include "Platform/UserListDlg.h"
#include "Platform/DlgLeftShow.h"
#include "Platform/PlaySound.h"
#include "Platform/PropSelUser.h"
#include "Platform/AFCDialog.h"
#include "../common/centerserverhead.h"
#include "Platform/PlayView.h"
#include "BoardCast.h"			///< ������С���ȶԻ���
#include "PropPannel.h"			///< ������߶Ի���
#include "BankDlg.h"			///< ���жԻ���
#include "checkPSW.h"			///< ��������Ի���
#include "PBankDlg.h"
#include "UserDetailsDlg.h"   ///< �û���ϸ�������
#include "SkinMgr.h"

class CPBank4GameWnd;
class CRightFrame;				///��Ϸ�ұ߿�
class CGameTrottingWnd;

/// CLoveSendClassInExe �Ի���

/// ��Ϸ��ܽӿ�
interface IFrameInterface
{
	//��������
public:
	//��ʼ�����״�����ʹ�ã�
	virtual bool InitFrame(GameInfoStructInExe * pGameInfo, CWnd * pGameRoom)=0;
	//��������
	virtual bool CreateFrame()=0;
	//�رմ���
	virtual bool AFCCloseFrame(bool bNotifyServer=true)=0;
	//ɾ������
	virtual void DeleteFrame()=0;
	////��ʾ����
	//virtual void AFCShowFrame(int nShow)=0;
	////����windows��Ϣ
	//virtual LRESULT SendWindowMessage(UINT msg,WPARAM wParam,LPARAM lParam)=0;
	//������Ϣ 
	virtual UINT OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize)=0;
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)=0;
	//������Ϸ������Ϣ
	virtual void ShowMsgInGame(TCHAR * szMsg, BYTE bNewOrSys=0,UINT uSize=0, TCHAR * szFontName=NULL,TCHAR *Title=NULL)=0;
	//��������Ϣ
	virtual void ShowMsgDudu(TCHAR * szSendName, TCHAR * szTargetName,TCHAR * szCharString, COLORREF crTextColor, bool bShowTime, UINT uSize=0, TCHAR * szFontName=NULL,int iDuduType=0)=0;
	//ͬ���û��嵥
	virtual void OnUserListAtDesk(void *pIPCBuffer)=0;
	//���������û�����
	virtual void ResetUserInfo(void *pIPCBuffer)=0;
	//��ʼ����������
	virtual void OnInitPropList(int nRefleshType = 0)=0;
	//¼Ӱ�ӿں���lc090226
	virtual void SetRecordView()=0;
	//��Ϊ����
	virtual void OnAddFriend(bool bIsHave) = 0;
	
	// duanxiaohui 2011-11-11 ���������
	virtual void OnAddEmeny(bool bIsHave) = 0;
	// end duanxiaohui

	virtual void OnGetFriendList(void* pBuff, int nSize) = 0;
	////����������Ϣ
	//virtual void SendShortMessage(MSG_GR_RS_NormalTalk *pShortMessage)=0;
};



/// CLoveSendClassInExe
/// ��������Ϸ�����˼̳еĻ���
/// �ṩ��Ϣ���պͷ��ͽӿ�
/// �ṩ��Ϸ�����н���
/// �ṩ���߲�������
/// �ṩȷ�ϻ���ʾ�Ի������
/// 
class EXT_CLASS CLoveSendClassInExe : public CLoadFileBmp, public IFrameInterface
{
	friend class CRightFrame;

	enum{PEER_PAGE=9/*4*/}; // PengJiLin, 2010-10-12, ֻ��ʾ3��
	DECLARE_DYNAMIC(CLoveSendClassInExe)


	//-------------------------------------------------------------------------
	//ԭCLoveSendClass�ĳ�Ա
	CWnd								* m_pGameRoom;					//���䴰��
	CUsingSkinClass						* const m_pGameView;			//��Ϸ��ͼ

	// ��Ϸ�м����źŵƹ���
	void checkNetSignal(void);
	void getNetSignal(void * pNetData);
	void drawSignal(CDC * pDc);
	int m_NetSignalIndex;                //����������Ϣ������
	int m_SavedSingalIndex;     //��һ�η��͵�������Ϣ�����ţ��յ���Ϣ����m_NetSingalIndexͬ��
	int m_NetSignalLevel;                //�����źż���
	Image								*pSingnalimg;
	GameInfoStructInExe					* m_pGameInfo;					//��Ϸ��Ϣָ��
	vector<MSG_IM_C_GETFRIENDLIST>		m_FriendList;

	UserItemStruct						* m_pUserInfo[MAX_PEOPLE];		//��Ϸ�����Ϣ

protected:

//	CBankDlg						* m_pBankDlg;						///< ���н���Ի���
//	CCheckPSW						* m_pCheckPwd;						///< ��������Ի���
	CPBankDlg						* m_pPersonBank;					///< �������н���Ի���
	CPBank4GameWnd					* m_pBank2Wnd;						///< �ڶ�����Ϸ���жԻ���
	CUserDetailsDlg					* m_pUserDetailsDlg;				///< �û���ϸ�������
	CGameTrottingWnd				* m_pPmdWnd;						///< �����

	bool								m_bSaveView;					//�������
	POINT								m_SavedScreen;
	bool								m_bMax;							//�Ƿ����
	bool								m_bShowMax;						//�Ƿ���ʾ���

	bool								m_bWatchMode;					//�Թ�ģʽ
	bool								m_bWatchOther;					//�����Թ�
	BYTE								m_bWaitTime;					//�ȴ�ʱ��
	CPtrArray							m_WatchPtrArray;				//�����Թ����ָ������
	CPtrArray							m_ThisDeskPtrArray;				//���ڱ��������ָ������

	CString								m_iniFile;						//�����ļ�·��
	CString								m_HidebtPath;
	CString								m_ShowbtPath;

	bool								m_bInit;						// ָʾ��Ա�Ƿ��ʼ��
	UserInfoStruct						* m_CurpUserInfoClicked;		// ��ǰѡ�е������Ϣָ��

	TCHAR								m_BackMusicIni[MAX_PATH+1];		//�������������ļ�
	TCHAR								m_BMusicKey[100];				//��������key

	CString								szAdvUrl;						//����ַ����

	//���и�����Ϣ
	long int							m_dwCurrentUserID;				//��ǰ�û�
	CPropSelUser						*m_pPropSelUser;				//��ǰ����ʹ�õ��ߵ����
	int									m_curPropPage;					//��ǰ����ҳ
	TCHAR								m_szPropEmptyImage[MAX_PATH];	//�յ��߻���߼���Ϊ0ʱ��ͼƬ·��
	CRect								m_UserFrameRectArray[MAX_PEOPLE];						//��¼�û�λ�õĿ�����

	//���츨����Ϣ
	CString								m_strLastTalk;					//�������
	long int							m_dwLastTalkTime;				//˵��ʱ��
	static CArray<LONG,LONG>			m_ShieldArray;					//�����û�
	int									m_nTalkDefaultCount;			//Ĭ���������

	//-----------------------------------------------------------------\
	//�е�����ʾ�������
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
	CGameImage							m_backImage;					//����ͼƬ

	int									m_iStatic1X,m_iStatic1Y;
	CRect								m_UserInfoRect,m_UserListRect,m_TalkRect;	// �û���Ϣ������б�������
	//����ͼ
	CGameImage							m_rc,m_lc,m_tc,m_bc;							//�ĸ���
	CGameImage							m_lt,m_rt,m_lb,m_rb;							//�ĸ���
	//�ұ�
	CGameImage							m_right_rc,m_right_lc,m_right_tc,m_right_bc;	//�ĸ���
	CGameImage							m_right_lt,m_right_rt,m_right_lb,m_right_rb;	//�ĸ���
	CGameImage							m_UserInfo_bk;
	int									m_userlistW,m_userlistH,m_talkW,m_talkH;
	int									m_talkX,m_talkY;										//������XY

	TCHAR								m_szGameRoomName[61];
	CAFCColorResource					m_colorresource;				//����,ϵͳ��ɫ�ṹ
	PropStruct							m_stProp ;						//��ǰ���߲��Ŷ������;

	/////////////////////////////
	//Kylin 20090205 �����û���Ϣ����
	COLORREF	m_userbkcr;
	/////////////////////////////

	CRightFrame*						m_pRightFrame;					//��Ϸ�ұ߿�\
																		//  JianGuankun 2012.4.11

	//ԭ���ڳߴ�
	int									m_nWiondwMetricsX;
	int									m_nWiondwMetricsY;

	//-----------------------------------------------------------------\
	//�ؼ�

	CBoardCast							*m_dlgBroadcast;
	CExplorer2							m_ie;							//IE�����
	CDlgLeftShow						m_DlgShowLeft;					//�����뿪��ʾ��
	CToolTipCtrl						m_ToolTip;						//������ʾ
	//CControlMessage						m_HorSplitBar;					//������
	CNewMiniDlg							m_TalkMessage;					//������Ϣ
	CPlaySound							m_PlaySound;					//��������
	CGetPoint							m_DlgGetPoint;					//�����
	CComboBox							m_InputMessage;					//������Ϣ

	//CPtrArray							m_ShortMessage;					//��Ϣ����
	CPictureHelper						m_UserListDlg;					//�û��б�
	CGameTalkText						m_ExpressWnd;					//���鴰��
	CLookingClass						m_MessageHandle;				//��Ϣ����
	// �������
	int									m_sndListID;
	//CNormalBitmapButtonEx				m_btSndPlay;					//����
	//CNormalBitmapButtonEx				m_btSndPause;					//��ͣ
	//CNormalBitmapButtonEx				m_btVolDown;					//��������
	//CNormalBitmapButtonEx				m_btVolUp;						//��������
	int									m_sndBtnCx,m_sndBtnCy;			//��ť��С
	int									sndPlayX,sndPlayY;				//���Ű�ť����
	int									sndPauseX,sndPauseY;			//��ͣ��ť����
	int									sndVolUpX,sndVolUpY;			//������С��ť����
	int									sndVolDownX,sndVolDownY;		//��������ť����
	bool								m_bSndPause;					//�Ƿ���ͣ

	// ��Ҽ佻��
	CNormalBitmapButtonEx				m_BtSend;						//���Ͱ�ť
	CNormalBitmapButtonEx				m_BtExpression;					//���鰴ť
	CNormalBitmapButtonEx				m_BtGameQuit;					//�˳���ť
	CNormalBitmapButtonEx				m_BtSetColor;					//��������������ɫ

	CNormalBitmapButtonEx				m_Btbt1;						//
	CNormalBitmapButtonEx				m_Btbt2;						//
	CNormalBitmapButtonEx				m_Btbt3;						//
	CNormalBitmapButtonEx				m_Btbt4;						//
	CNormalBitmapButtonEx				m_Btbt5;						//
	CNormalBitmapButtonEx				m_Btbt6;						//
	CNormalBitmapButtonEx				m_Btbt7;						//
	//CNormalBitmapButtonEx				m_Btbt8;						//
	//CNormalBitmapButtonEx				m_Btbt9;						//
	CNormalBitmapButtonEx				m_Btbt10;						//

	// ����
	CNormalBitmapButtonEx				m_BtPropGoAhead;	
	CNormalBitmapButtonEx				m_BtPropGoNext;	
	CNormalBitmapButtonEx				m_BtProp1;
	CNormalBitmapButtonEx				m_BtProp2;
	CNormalBitmapButtonEx				m_BtProp3;
	CNormalBitmapButtonEx				m_BtProp4;
	CNormalBitmapButtonEx				m_BtProp5;
	CNormalBitmapButtonEx				m_BtProp6;
	CNormalBitmapButtonEx				m_BtProp7;
	CNormalBitmapButtonEx				m_BtProp8;
	CNormalBitmapButtonEx				m_BtProp9;
	CNormalBitmapButtonEx				m_BtProp10;
	UINT								m_BtPropArray[PEER_PAGE];			//Ϊ�˷���,���Խ��ĸ���ť�ŵ�������
	CPropPannel							*m_pPropPannel;				//����������

	CNormalBitmapButtonEx				m_BtHide;						//����
	CNormalBitmapButtonEx				m_BtExit;						//��ť(��)
	CNormalBitmapButtonEx				m_BtMin;						//��ť
	CNormalBitmapButtonEx				m_btMax;						//���
	CNormalBitmapButtonEx				m_btRest;						//��ԭ
	int									nNothing;						//���ó�Ա��
	//------------------------------------------------------------------------//

    // PengJiLin, 2010-9-9, ���˿������߿��Ի���
    CBoardCast*                         m_dlgKickProp;

    // PengJiLin, 2010-9-28
    CGameImage  m_propBackImage;
    CRect       m_propBackRect;
    CGameImage  m_propBackImage2;
    CRect       m_propBackRect2;

    int         m_curPropPage_S;


	bool		m_bIsQueueDisMiss;

private:
	CSkinMgr	m_skinmgr;
public:
    // PengJiLin, 2011-4-18, ����ʱ���������ͽ�ҹ���
    CM_UserState_Send_Times_Money_For_Exe m_SendMoneyData;

    // PengJiLin, 2011-4-18, ����ʱ���������ͽ�ҹ��ܣ���ȡ��ʽ����ʾ��Ϣ
    void GetSendMoneyFormatInfo(CString& strInfo);

    // PengJiLin, 2011-5-10, ����ʱ���������ͽ�ҹ��ܣ���ȡ��ʽ����ʾ��Ϣ
    void GetSendMoneyFormatInfo(CString& strTime, CString& strCount);

    // PengJiLin, 2011-5-10, ����ʱ���������ͽ�ҹ��ܣ������ʹ�õĸ�ʽ����Ϣ
    void GetSendMoneyFormatInfo(CString& strGet, CString& strTime, CString& strCount);

    // PengJiLin, 2011-4-19, ����ʱ���������ͽ��ͼ����ʾ
    virtual void ShowGetMoneyImage();

	UserItemStruct* GetUserItemInfo(int iIdx);

public:
	CLoveSendClassInExe(CUsingSkinClass * pGameView);   // ��׼���캯��
	virtual ~CLoveSendClassInExe();
	//������Ƶ���øı�
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//��ʼ��
	bool InitFrame(GameInfoStructInExe * pGameInfo, CWnd *pGameRoom);
	bool AFCCloseFrame(bool bNotifyServer=true);

	bool CreateFrame();
	void DeleteFrame();
	//��ʼ������
	virtual BOOL OnInitDialog();

	// �Ի�������
	//enum { IDD = IDD_EXE_DIALOG };

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void DoDataExchangeWebVirFace(CDataExchange * pDX){}
	//��Ϣ���ͺ���
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�ػ�����
	afx_msg void OnPaint();
	//λ�ñ仯
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	public:
	///< ��ȡ�ȼ���Ϣ��GetGameOrderNameʹ��
	static void ReadLevelData();

    // PengJiLin, 2011-6-23, ����
    afx_msg void OnActivate(UINT uState, CWnd* pOldWnd, BOOL bMinisted);

	///< ��ȡ��Ϸ����Ļ��ּ���
	///< @dwPoint��һ���
	///< ��һ�������
	static TCHAR * GetGameOrderName(__int64 dwPoint);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedExit();
	virtual afx_msg void OnBnClickedRest();//�ټ��� �ĳ��麯�� by wlr 20090716
	//������Ϣ
	LRESULT OnExpression(WPARAM wparam, LPARAM lparam);
	//������Ϣ
	LRESULT OnRichEditHit(WPARAM wparam, LPARAM lparam);
	//�ر���Ϸ
	LRESULT OnCloseGame(WPARAM wparam, LPARAM lparam);
	//�������Ϣ
	LRESULT OnSplitMessage(WPARAM wparam, LPARAM lparam);
	//�û�ʹ�ù㲥ȷ����
	LRESULT OnUseBoardcast(WPARAM wparam, LPARAM lparam);
	// �ӵ�������д���ʹ�õ�����Ϣ
	LRESULT OnPannelUseProp(WPARAM wparam, LPARAM lparam);
	// �ӵ�������д��ع��������Ϣ
	LRESULT OnPannelBuyProp(WPARAM wparam, LPARAM lparam);
	// �ӵ�������д������͵�����Ϣ
	LRESULT OnPannelGiveProp(WPARAM wparam, LPARAM lparam);
	//����Ƥ��
	LRESULT OnLoadSkin(WPARAM wparam, LPARAM lparam);
	//�ض�talkIngame.ini
	LRESULT OnReLoadTalkINI(WPARAM wparam, LPARAM lparam);
	//���ͻ���
	LRESULT OnGetPoint(WPARAM wparam,LPARAM lparam);
	//����ȡǮ����
	LRESULT OnBankCheckOut(WPARAM wparam,LPARAM lparam);
	//���ߴ��صĹ���VIP��Ϣ     add by wyl   11-5-21
	LRESULT OnPannelBuyVIP(WPARAM wparam, LPARAM lparam);
	//��Ӻ���
	LRESULT OnSendGetFriendList(WPARAM wParam, LPARAM lParam);   
	//��Ϸ���д�ȡǮ
	LRESULT OnGameBankMoneyCheck(WPARAM wParam, LPARAM lParam);   
	LRESULT OnGameBankMoneyCheck2(WPARAM wParam, LPARAM lParam);   
	//�ر���Ϸ���д�����Ϣ
	LRESULT OnGameBankClose(WPARAM wParam, LPARAM lParam);   

	//����ʹ��
	LRESULT OnBuyAndUse(WPARAM wParam, LPARAM lParam);  


protected:
	DECLARE_MESSAGE_MAP()

	//���ƿ��
	void DrawViewFrame(CDC * pDC, int iWidth, int iHeight);
	void DrawCurUserInfo(CDC *pDC);

	// ----------------------\\
	//ע�����
	//ע�����
public:
	///д����ֵ
	///@param szEntry ����ַ���
	///@param iDefault Ĭ��ֵ
	BOOL WriteProfileInt(TCHAR * szEntry, int iDefault);
	//��ȡ�ַ���
	BOOL WriteProfileString(TCHAR * szEntry, TCHAR * szValue);
	//��ȡ��ֵ
	UINT GetProfileInt(TCHAR * szEntry, int iDefault);
	//��ȡ�ַ���
	CString GetProfileString(TCHAR * szEntry, TCHAR * szDefault);

    // PengJiLin, 2010-6-1, ���� ������ �ӿ�, ��Ϸ������
    virtual void UseLookCardProp(_TAG_USERPROP* userProp);

    // PengJiLin, 2010-9-9, ���˿�����߿����ܽӿ�
    virtual void UseKickUserProp(_TAG_USERPROP* userProp);
    virtual void UseAntiKickProp(_TAG_USERPROP* userProp);

    // PengJiLin, 2010-9-10, ��������ʽ��Ϊʱ����ĸ�ʽ
    void SecondFormat(int iSecond, CString& strOutput);

    // PengJiLin, 2010-9-10, ���˿������߿�ʹ�ý��
    bool OnUseKickPropResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, BOOL bIsKick);

    // PengJiLin, 2010-9-14, ���˿������߿���ʾ����ʾ
    void ShowNewKickPropDlg(BOOL bUseForOther, BOOL bUseLink, BOOL bIsTOut, CString& strInfo);

public:
	//-------------------------------------------------------------------------\
	//���Ӵ��ڵ��õĽӿ�
	int GetComType(void){return m_pGameInfo->uComType;}

	void OpenUserMenu(UserItemStruct * pUserItem);			// �һ��û��б�ʱ�����Ҽ��˵�
	void OpenUserMenuEx(UserItemStruct * pUserItem);		// �һ��û��б�ʱ�����Ҽ��˵�
	void OnLBClickUserList(UserItemStruct * pUserItem);		// ����û��б�
	void OnLBDBClickUserList(UserItemStruct * pUserItem);	// ˫���û��б�

	bool IsEnableSound(){ return m_pGameInfo->bEnableSound; }		// �Ƿ񲥷�����
	//��ȡ�Լ���Ϣ
	UserInfoStruct * GetMeUserInfo() { if(NULL == m_pGameInfo)return NULL; return &m_pGameInfo->uisMeUserInfo.GameUserInfo; };
	//������Ϣ 
	virtual UINT OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize);
	// �յ��������˴�����ͬ������嵥
	void OnUserListAtDesk(void *pUserItem);
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);
	//������Ϸ������Ϣ
	virtual void ShowMsgInGame(TCHAR * szMsg,BYTE bNewOrSys=0, UINT uSize=0, TCHAR * szFontName=NULL, TCHAR *Title=NULL);
	//��������Ϣ
	virtual void ShowMsgDudu(TCHAR * szSendName, TCHAR * szTargetName,TCHAR * szCharString, COLORREF crTextColor, bool bShowTime, UINT uSize=0, TCHAR * szFontName=NULL,int iDuduType = 0);
	void OnInitPropList(int nRefleshType = 0);											//���յ������б���Ϣʱ��ʼ������
	void ResetUserInfo(void *pUserInfo);					//����ͬ���������

	void OnAddFriend(bool bIsHave);
	void OnGetFriendList(void* pBuff, int nSize);

	// duanxiaohui 2011-11-11 ���������
	void OnAddEmeny(bool bIsHave);

	//�˵�����///////////////////////////////////////////////

	//�鿴�û���Ϣ
	BOOL OnCmdShowUserInfo();

	//�����û���Ϣ
	BOOL OnCmdCopyUserInfo();

	//����
	BOOL OnCmdKickUser();

	//����
	BOOL OnCmdKickUserProp();

	/////////////////////////////////////////////////////////

	//¼Ӱ�ӿں���lc090226
	virtual void SetRecordView()
	{
	};
	//-------------------------------------------------------------------------\
	//���ܺ���
	//����������Ϣ����
	int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//����������Ϣ����
	int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//���Ż��㵽��ͼλ��
	virtual BYTE ViewStation(BYTE bDeskStation);
	//��ȡ״̬����
	BYTE GetStationParameter() { return m_pGameInfo->bGameStation; };
	//���ö�ʱ��
	void SetGameTimer(BYTE bDeskStation, UINT uMaxTime, UINT uTimeID);
	//��ȡ��Ϸ��� add by wlr 20090716
	virtual void GetGameCode(char * szGameCode);
	///�ж��Ƿ����Ŷӻ�����
	///@return true:���Ŷӻ����䣻false�������Ŷӻ�����
	inline bool IsQueueGameRoom() const;


protected:
	void LoadSkin();///< ����Ƥ��
	void LoadSoundList(void);

	void SendTalkMessage(int nDefaultIndex=0);
	void OnSetPropImages();											//���õ��߰�ťͼƬ
	BOOL OnToolTipNotify(UINT id, NMHDR * pTTTStruct, LRESULT * pResult );	//��ӦToolTip��Ϣ
	BOOL OnUseProp(int nPropIndex, int nDeskUserID);				//ʹ�õ���
	//����״̬����
	void SetStationParameter(BYTE bGameStation);
	//�������ý���
	virtual void ResetGameFrame();
	virtual void ShowUserTalk(MSG_GR_RS_NormalTalk * pNormalTalk){return ;}
	//��������
	void PlayGameSound(TCHAR * szFileName);
	//��������
	void PlayGameSound(HINSTANCE hInstance, TCHAR * szResourceName);
	//ɾ����ʱ��
	void KillGameTimer(UINT uTimeID);

	/// ����Ϸ�����д����н���
	void OnVisitBank();
	/// ˢ���û��б��еĽ��
	/// @param dwUserID �û�ID��
	/// @param uCount ��Ϊ��������ֵ����˼Ϊ�ӻ��
	bool FlushUserMoney(long dwUserID,__int64 i64Count);

    // PengJiLin, 2010-10-12, �̳����߹���
    void OnSetPropImagesS();

	//������Ӧ��Ϣ����
	afx_msg void OnBnClickedGoAhead();//��һ��
	afx_msg void OnBnClickedGoNext();
    afx_msg void OnBnClickedGoAheadS();//�ڶ���
    afx_msg void OnBnClickedGoNextS();

	afx_msg void OnBnClickedButtonProp1();
	afx_msg void OnBnClickedButtonProp2();
	afx_msg void OnBnClickedButtonProp3();
    afx_msg void OnBnClickedButtonProp4();
    afx_msg void OnBnClickedButtonProp5();
    afx_msg void OnBnClickedButtonProp6();
	afx_msg void OnBnClickedButtonProp7();
	afx_msg void OnBnClickedButtonProp8();
	afx_msg void OnBnClickedButtonProp9();
	afx_msg void OnBnClickedButtonProp10();

    void BuyAndUseProp(int propIndex);

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
	////��ܺ���
	virtual bool OnFrameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);
	////��Ϣ����
	virtual bool OnSystemMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);

	// ��Ҫ���ص����⺯��
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);
	//��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)=0;
protected:
	//�������غ���
	//��Ϸ��Ϣ������
	//�������Ϣ������
	virtual bool HandlePointMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);
	/// �������е���Ϣ
	/// ��ͻ��˵�GameRoomEx�д�ͬС�죬ֻ�������ؽ����������������ݵı仯
	/// @param[in] pNetHead Э��ͷ
	/// @param[in] pNetData ���ݰ�
	/// @param[in] uDataSize ���ݰ���С
	/// @return ����ɹ������򷵻�true������false��ʾ���������Ϣʧ�ܣ�����һ�ֿ�������ΪЭ�鲻ƥ�䣬�����޸���ͷ�ļ�ȴû�ж����±���
	/// 
	virtual bool OnAboutMoney(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);
	//������Ϣ
	virtual bool OnPropMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);
	//������Ϣ
	virtual bool OnPropVipCheckTime(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);
	//ʹ�õ���
	virtual bool OnUsePropResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);
	//����������Ϸ����
	virtual void ResetGameStation(int iGameStation=1){};
	//���ú���
	virtual void OnGameSetting() {};
	//���ú���
	virtual void OnGetBuyItemData(int iPropID);
	//������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize){return false;};
	//�Թ۸ı�
	virtual void OnWatchSetChange(){};
	//����
	virtual bool UserProp(_TAG_PROP_MOIVE_USE *propMoive);
	//���߶���
	virtual bool PlayNextGif();

    // PengJiLin, 2010-9-14, ʹ�����˿����˽����Ϣ
    virtual bool OnNewKickUserResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);

	//�ر�����
	virtual void OnCloseBank2();

private:
	void SetFillColor(COLORREF rgb);		//�������ɫ
	void NavigateVirFace();
	afx_msg void OnBnClickedQuitGame();
	virtual afx_msg void OnBnClickedMax();//�ټ��� �ĳ��麯�� by wlr 20090716
	afx_msg void OnBnClickedHide();
	afx_msg void OnBnClickedMin();
	virtual afx_msg void OnMove(int x, int y);//�ټ��� �ĳ��麯�� by wlr 20090716
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void OnSndPlay();
	void OnSndPause();
	void OnSndVolDown();
	void OnSndVolUp();

	/// ����ȫ�ֱ����еļ��ܷ�ʽ��ȡ�ü��ܺ�������ַ���������sha������MD5
	/// �����ﲻ��ָ��ĺϷ������ж�
	/// @param szMD5Pass ���ܺ���ַ���
	/// @param szSrcPass Դ�ַ���
	/// @return ���ܺ���ַ���ָ��
	TCHAR * GetCryptedPasswd(TCHAR *szMD5Pass, TCHAR *szSrcPass);
	/// ���н��淢��ָ���Ǯ��ȡǮ
	/// @param pCheckMoney ��ȡǮ�ṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
	/// @return ��
	void OnCheckMoney( CheckMoney_t *pCheckMoney );
	/// ��2�����еĴ�ȡ��
	/// @param pCheckMoney ��ȡǮ�ṹָ��
	/// @return ��
	void OnCheckMoney2( bank_op_normal *pCheckMoney );
	/// ���н��淢��ָ�ת��
	/// @param pTransferMoney ��ȡǮ�ṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
	/// @return ��
	void OnTransferMoney( TransferMoney_t *pTransferMoney );
	/// ���н��淢��ָ��޸�����
	/// @param pChangePasswd �޸�����ṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
	/// @return ��
	void OnChangePasswd( ChangePasswd_t *pChangePasswd );
	/// ���н��淢��ָ�ת�ʼ�¼
	/// @return ��
	void OnTransferRecord();

	void OnGetNickNameOnID(GetNickNameOnID_t* pGetNickNameOnID);
protected:
	//�ڱ������ҵ����û��������û�ID����
	UserItemStruct * FindOnLineUser(long int dwUserID);
	//��ƽ̨��������ͬ����������������Լ���������ʱ
	void GetOnLineUser( BYTE bMeDeskStation );
	// ʹ�õ���
	void UsePropItem(int propIndex, BOOL bCheckNum = TRUE);     // PengJiLin, 2010-10-14, ����һ����
	void UseBoardCast(_TAG_USERPROP * userProp);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedGameBtn01();
public:
	afx_msg void OnBnClickedGameBtn02();  //��Ϸ�̳�
public:
	afx_msg void OnBnClickedGameBtn03();
public:
	afx_msg void OnBnClickedGameBtn04();
public:
	afx_msg void OnCbnSelchangeInput();
public:
	afx_msg void OnBnClickedSend();
public:
	afx_msg void OnBnClickedSetcolor();
public:
	afx_msg void OnBnClickedExpression();
public:
    afx_msg void OnBnClickedBlueDiamond();  // PengJiLin, 2010-9-30, ������� ����vip

public:
	afx_msg void OnClose();
protected:
	virtual void OnCancel();
};

class EXT_CLASS CGlobal//ȫ����
{
public :
	CGlobal()
	{
		CString s=CBcfFile::GetAppPath();/////����·��


		CINIFile f( s + "bzgame.ini");
		m_key="BZW";
		_tcscpy(m_CenterServerPara.m_strHomeADDR, TEXT("http://bbs"));
		//		m_CenterServerIPAddr=f.GetKeyVal (m_key,"LoginIP","127.0.0.1");//������IP		

		m_skin=f.GetKeyVal(m_key,"skin","skin0");
		//m_pastimeurl=f.GetKeyVal(m_key,"pastime","");
		m_urlpostfix=f.GetKeyVal(m_key,"urlpostfix","asp");
		m_NewUrl=f.GetKeyVal(m_key,"newurl","");

		m_szSPToken="sn";
		m_szProcessName = "ddz";
		userPropLibrary.RemoveAll();
	}
	virtual ~CGlobal()
	{
		for (int i=0; i<userPropLibrary.GetCount(); ++i)
		{
			_TAG_USERPROP *userProp = userPropLibrary.GetAt(i);
			delete userProp;
		}
		userPropLibrary.RemoveAll();
	}
	CenterServerMsg m_CenterServerPara;
	CString m_key;
	CString m_Btn[10];//���20��
	CString	m_szSPToken;//CookieSPToken,������֤�û���¼
	CString	m_skin;
	CString m_sktext[5];
	CString m_urlpostfix;
	CString m_NewUrl;
	CString m_TML_SN;
	CArray<_TAG_USERPROP*,_TAG_USERPROP*> userPropLibrary;							//�û�ӵ�еĵ���

	// ֻ������Ϸ���ֵĳ�Ա��ƽ̨���ֵ�����������
	CString m_szProcessName;
	/// ��ʾ��ҽ�ң����н�ҵȱ�ʾ������ֵʱ��������ϼ�����
	/// 123�������ʾΪ123000�����ֵΪ3
	int			m_nPowerOfGold;	

    // PengJiLin, 2010-8-3, �����ʾ���������Ƿ��зָ������Լ�ʲô���ķָ���
    bool        m_bUseSpace;        // true = �зָ���
    TCHAR       m_strSpaceChar[10]; // ������ĸ�ķָ���

	int			m_iMaxBit;
};



extern EXT_CLASS CGlobal& Glb();
