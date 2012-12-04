#pragma once
#include "XGameLogicBase.h"
#include "IGameFrame.h"
#include "IAudioPlay.h"
#include "UserDetailsDlg.h"   ///< �û���ϸ�������
#include <list>
#include <map>
#include "afxWin.h"
#include "afxext.h"
#include "afxdisp.h"
#include "PBank4GameWnd.h"		///< �ڶ�����Ϸ����
#include "GameTrottingWnd.h"
#include "SkinMgr.h"

#include "UI_ForLogic/iuserinterface.h"
using std::list;

class XRightFrame;				///��Ϸ�ұ߿�

const int HEART_BEAT_SECOND		= 3;	// �������
const int HEART_BEAT_DELAY		= 10;	// ���������ʱ

const int OLEDRAW_TIMER_ID      = 1;    // ��OLE���Ϳؼ�
const int OLEDRAW_INTERVAL      = 66;

#define UM_FIXRIGHTFRAME		(WM_USER + 100) //�����ұ߿�λ����Ϣ

///��������
enum CHATTYPE
{
    CT_PRIVATE	= 0,	///<˽��
    CT_SYSTEM	= 1,	///<ϵͳ
    CT_NORMAL   = 2,    ///<һ����Ϣ
    CT_PUBLIC	= 3,	///<����
    CT_SOCIETY  = 4,	///<����
    CT_DALABA   = 5,    ///<��������Ϣ
    CT_XIAOLABA = 6     ///<С������Ϣ
};
///������Ϣ
struct TalkMessage_t
{
    TCHAR           szSourceNickName[32]; /// ������Ϣ���ǳ�
    int				iSourceID;
    int				iTargetID;
    CHATTYPE		eChatType;
    time_t			sTime;
    TCHAR			szMsgString[MAX_TALK_LEN + 1];
	COLORREF        crTextColor;
    TalkMessage_t()
    {
        ZeroMemory(this, sizeof(TalkMessage_t));
    }
};

class CChannelService;
class IGameImpl;
interface IUserInterface;
class IGameEngine;
class IContain;
class XLogic : public XGameLogicBase,
    public IGameFrame
{

	friend class XRightFrame;
	friend class CUseBoardCastWnd;

public:
    XLogic();
    virtual ~XLogic();

private:
    XLogic(const XLogic &other);
    XLogic &operator = (const XLogic &other);

public:
    /// ��ʼ��
    virtual int Initial(LogicInitParam_t &lip);
    /// IPC��Ϣ
    virtual bool OnIPCMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    /// IPC�������
    virtual bool OnIPCHeartBeat();
    /// UI��Ϣ
    virtual int OnUIMessage(TUIMessage *p);
    /// ʱ��
    virtual void OnTimer(int nEventID);
    //�뿪��Ϸ
    virtual void QuitGame(bool bNotify = true,bool bNeedCheck = false/*�Ƿ�Ҫ������ʾ��*/);
    /// ���ڴ�С�ı�
    virtual void OnSize(RECT &rect);
    /// ʹ�õ���
    virtual void OnPannelUseProp(WPARAM wParam, LPARAM lParam);
    /// �������
    virtual void OnPannelBuyProp(WPARAM wParam, LPARAM lParam);
    /// ���͵���
    virtual void OnPannelGiveProp(WPARAM wParam, LPARAM lParam);
    /// ������ʹ�õ���
    virtual void OnSelOtherUseProp(WPARAM wParam, LPARAM lParam);
    //�û�ʹ�ù㲥���Ի�����ȷ������������û�ID���û�����ֵ
    virtual void OnUseBoardcast(WPARAM wparam, LPARAM lparam);   
    //���н��淢������Ϣ
    virtual void OnBankMessage(UINT message,WPARAM wParam,LPARAM lParam);
	//�ر�����
	virtual void OnCloseBank2();


public:
    int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
    /// ����Ϣ
    virtual bool SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
    ///��Ϸ�ӿ���л�ȡ����Ϣ
    virtual int GetMyDeskStation();
    ///����Լ������Ӻ�
    virtual int GetMyDeskNum();
    ///���ﴫ������ͼ����
    virtual int GetUserStation(int nIndex);  
    ///����Ϣ���ݷ��͵���Ϸ������
    virtual int SendGameData(WORD wSubId,void *buffer, int nLen);
    ///ֻ����Ϣ���͵��ͻ��˴���
    virtual int SendIpcData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);
    ///��ͼת��������λ ��Ϊ������Ϸ��Ҫ�õ��������ڿ���д���
    virtual int ViewStation2DeskStation(int nViewIndex);
    ///��λ��ת��ͼ��
    virtual int DeskStation2View(int nDeskStation);
    ///��ȡ����ǳ�
    virtual TCHAR * GetUserNickName(int nDeskStation);
    ///�ر���Ϸ�ͻ���
    virtual void CloseClient();
    ///�������
    virtual int ClickContinue();
    ///ǿ�ƹر���Ϸ
    virtual void CloseClientforce();
    ///����ʱ��
    virtual void SetTimer(int nEventID, DWORD dwTime);
    ///�ر�ʱ��
    virtual void KillTimer(int nEventID);
    ///��ȡ��Ϸ����
    virtual int GetVolume(); 
    ///��ȡ��Ϸ����
    virtual int GetTalkKind();
    ///����һ��ϵͳ��Ϣ
    virtual int InsertSystemMessage(TCHAR* szMessage);
    ///����һ����ͨ��Ϣ
    virtual int InsertNormalMessage(TCHAR* szMessage);

    ///��ȡ����·��
    /// param _path[in, out] ������׼�����ڴ�ռ�
    /// param nSize[in] _path���ڵ�ַӵ�еĿռ��С����λΪsizeof(TCHAR)
    /// return ��_path���ڿռ丳ֵ�󣬷���_path
    virtual char* GetAudioPath(int nDeskStation,char *_path, int nSize = MAX_PATH);
    ///��ȡ��������
    virtual bool GetAwardType(AwardTypeForGame &Type);	
    ///���Ž�������
    virtual bool PlayAwardAnimation(const AwardTypeForGame &type);
    ///��ȡ����
    virtual bool GetGameBaseInfo(GameInfoForGame &Info);
    ///��ȡ�û���Ϣ
    virtual bool GetUserInfo(int nDeskStation, UserInfoForGame &Info);
    /** @brief ���Ŵ�ͷ���ޱ���
    *	@param[in]  nViewStation    ��ͼλ��
    *	@param[in]  nIndex          ��������ţ�ĿǰΪ1~7������μ�����������.xls��
    *	@return ������
    */
    virtual bool PlayBigPortrait(int nViewStation, int nIndex);
    /** @brief ʹ��Сͷ��
     */
    virtual bool SetUseSmallLogo(int nViewStation, bool bUseSmall=true);
    /// ��������
    virtual bool PlaySound(const char *szFileName);
    /// ĳ��λ�Ƿ�ʹ����Ƶ
    virtual bool SetUseVideo(int nViewStation, bool bUse);
    /// ��ȡ�Ƿ�Ϊ�Թ�״̬
    virtual bool GetIsWatching();
    /// ��ȡ�Ƿ������Թ�
    virtual bool GetIsEnableWatch();
    /// ����Ϸ�����д����н���
    virtual void OnVisitBank();
    /// ��ȡȫ����Ϣ
    virtual CGlobal* GetGlb();
    virtual RECT GetGameWindowRect();

    /// ��ȡ��Ϸ��Ϣ2011-7-7
	virtual const GameInfoStructInExe* GetGameInfo(){return &m_GameInfo;};

	 //״̬�ı�
    virtual void SetStationParameter(BYTE bGameStation);

private:

	//��talkIngame.ini
	void OnReLoadTalkINI();
    //����֪ͨ
    bool OnIPCControl(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //����ָ��
    bool OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //����ָ��
    bool OnIPCConfig(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //�û�����
    bool OnIPCUserAct(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //���߳�ʼ��
    bool OnIPCProp(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    //�׽�����Ϣ
    bool OnSocketReadEvent(NetMessageHead * pNetHead, void *pNetData, WORD wSendSize);
    //ϵͳ��Ϣ
    bool OnSystemMessage(NetMessageHead * pNetHead, void *pNetData, WORD wSendSize);
    //������Ϣ
    bool OnPropMessage(NetMessageHead * pNetHead, void *pNetData, UINT wSendSize);
    bool OnUsePropResult(NetMessageHead * pNetHead, void *pNetData, UINT wSendSize);
    //vipʱ����
    bool OnPropVipCheckTime(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);
    // PengJiLin, 2010-9-14, ʹ�����˿����˽����Ϣ
    bool OnNewKickUserResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);
    //�����Ϣ
    bool OnFrameMessage(NetMessageHead * pNetHead, void *pNetData, WORD wSendSize);    
    // PengJiLin, 2010-9-10, ���˿������߿�ʹ�ý��
    bool OnUseKickPropResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, BOOL bIsKick);
    // PengJiLin, 2010-9-10, ��������ʽ��Ϊʱ����ĸ�ʽ
    void SecondFormat(int iSecond, CString& strOutput);
   
	//���߲������
	bool OnIPCUserPropResult(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
	//�������Ϣ����
	bool OnIPCTrottingMsg(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
	//������Ϣ
	bool OnExchageSkin(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);


	bool UserProp(_TAG_PROP_MOIVE_USE *propMoive);

	/// �����Ҽ��˵���Ӧ
	int OnTalkMenuMessage(TUIMessage *pMessage);
private:
    ////////////////////////////////////////////////////////////////////////////////
    // �û���Ϣ
    // �����û�ID�ұ���������Ƿ���
    // ����ƽ̨����ͬ����ҵ�����
    void GetOnLineUser( BYTE bMeDeskStation );
    UserItemStruct *FindOnLineUser(long int dwUserID);
    UserItemStruct *FindWatchUser(long int dwUserID);
    UserItemStruct *FindOnLineUser(BYTE bDeskStation);
	void OnUserListAtDesk(UserItemStruct *puserinfo);
    bool GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser);
    UserInfoStruct * GetMeUserInfo();

private:
    ////////////////////////////////////////////////////////////////////////////////
    // ���
    UINT OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize);

    ////////////////////////////////////////////////////////////////////////////////
    // ����    
    int InsertTalk(const TalkMessage_t& sMsg);
    int InsertDuduMsg(TCHAR * szSendName, TCHAR * szTargetName,TCHAR * szCharString, 
        COLORREF crTextColor, bool bShowTime, UINT uSize=0, 
        TCHAR * szFontName=NULL,int iDuduType=0);

    int RefreshTalk();
    TCHAR* FormatTalk(const TalkMessage_t& sMsg);

    // ��ʼ�������
    void UIInitBiaoQingKuang();
    // ���������
    void UIShowBiaoQingKuang(TUIMessage *pMessage);
    // ��������
    void UIHitBiaoQing(TUIMessage *pMessage);
    void SendTalkMsg(TCHAR* pMsg, int iUserID);
    /// ���� nControlIndex��ʾ���Լ����������ڼ�����������0������������
    int ShowOneUserInfo(int nControlIndex,UserItemStruct * pUserInfo,bool bVisible);

	int ShowCurrentUserInfo(UserItemStruct * pUserInfo,bool bVisible);
    /// ��ʼ������б�ؼ�
    int InitPlayerListCtrl();
    int RefreshPlayList();

    ////////////////////////////////////////////////////////////////////////////////
    // ���� 
    /// ��ʼ�������б�
    int InitPropPannel();
    int RefreshPropPannel();
    void OnHitPropBtn(int nIndex);
    bool UIEnableButton(int nID, bool bEnable);
    void OnBuyPropUIMessage(TUIMessage *pMessage);
    // ʹ�õ���
    void UsePropItem(int propIndex, BOOL bCheckNum = TRUE);
    BOOL OnUseProp(int nPropIndex, int nDeskUserID);    
    void UseAntiKickProp(_TAG_USERPROP* userProp);  // ʹ�÷��߿�
    void UseKickUserProp(_TAG_USERPROP* userProp);  // ʹ�����˿�    
    void UseBoardCast(_TAG_USERPROP * userProp);    // �û�ʹ�ù㲥���Ի�����ȷ������������û�ID���û�����ֵ
    int ShowInputDlg(int type);                   // ������С���������    

    void OnSetUIMessage(TUIMessage *pMessage);
    ///�����Ի��� funΪ�ص���������Ϊһ��boolֵ��ȷʵ��ȡ����ֵ
    void ShowMessageDialog(TCHAR* szMsg, int itype);

    void OpenUserMenu(UserItemStruct * pUserItem);			// �һ��û��б�ʱ�����Ҽ��˵�

    ////////////////////////////////////////////////////////////////////////////////
    // ����
    /// �������е���Ϣ
    /// ��ͻ��˵�GameRoomEx�д�ͬС�죬ֻ�������ؽ����������������ݵı仯
    /// @param[in] pNetHead Э��ͷ
    /// @param[in] pNetData ���ݰ�
    /// @param[in] uDataSize ���ݰ���С
    /// @return ����ɹ������򷵻�true������false��ʾ���������Ϣʧ�ܣ�����һ�ֿ�������ΪЭ�鲻ƥ�䣬�����޸���ͷ�ļ�ȴû�ж����±���
    /// 
    bool OnAboutMoney(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);
    /// ˢ���û��б��еĽ��
    /// @param dwUserID �û�ID��
    /// @param uCount ��Ϊ��������ֵ����˼Ϊ�ӻ��
    bool FlushUserMoney(long dwUserID,__int64 i64Count);

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

	void OnBnClickedRightFrameHide();
	void OnBnClickedRightFrameShow();

    /************************************************************************/
    //Function:checkNetSignal()
    //Parameter:
    //                pNetData        :���������ص���Ϣ��
    //Author:Fred Huang 2008-06-27
    //Note:�������������ص���Ϣ
    /************************************************************************/
    void getNetSignal(void * pNetData);
    void drawSignal();
	//�����ȡ�ȼ�
	int GetMoneyGrade(__int64 dwPoint);
	void DecodeString(CString str, CString &strColumn, int &width);

public:
	//��Ӧ������Ϣ
	void OnMessageProc(UINT message,WPARAM wParam,LPARAM lParam);

	//�˵�����///////////////////////////////////////////////

	//�鿴�û���Ϣ
	BOOL OnCmdShowUserInfo();

	//�����û���Ϣ
	BOOL OnCmdCopyUserInfo();

	//����
	BOOL OnCmdKickUser();

	//����
	BOOL OnCmdKickUserProp();

	//��Ӻ���
	BOOL OnAddFriend();

	//��Ӻ�����
	BOOL OnAddEmeny();

	//���ŵ��߶���
	void PlayProp(const bool& isSend,const CString& strFilePathName);

private:
    IGameEngine             *m_pEngine;

    IGameImpl               *m_pGameImpl;
    HINSTANCE               m_hGameInstance;
    CChannelService         *m_pIPC;
    IUserInterface          *m_pIUI;                ///< UI����
    HWND                    m_hWnd;

    DWORD                   m_dwIPCHeartBeatTick;

	CUserDetailsDlg *       m_pUserDetailsDlg;
    GameInfoStructInExe		m_GameInfo;
    list<TalkMessage_t>	    m_TalkMessages;

    bool                    m_bWatchOther;
    bool                    m_bWatchMode;
    BYTE                    m_bWaitTime;
    CArray<LONG,LONG>                       m_ShieldArray;                  //�����û�
    CArray<_TAG_USERPROP*,_TAG_USERPROP*>   m_userPropLibrary;              //�û�ӵ�еĵ���
    CPtrArray							    m_WatchPtrArray;				//�����Թ����ָ������
    CPtrArray							    m_ThisDeskPtrArray;				//���ڱ��������ָ������
	map<string, int>                        m_MapColumn;                    //�û��б������

	int						m_iSelectIndex;

	PropStruct							m_stProp ;						//��ǰ���߲��Ŷ������;


    enum
    {
        PROP_PEER_PAGE=9,   ///<һҳ���ٸ�����
        PROP_IDS=5          ///<��������ռ����ID����ť+1 ����+2
    };
    IContain                *m_pPropContain[PROP_PEER_PAGE];
    int                     m_nPropPage;

    CString                 m_strPropUrl;
    int			            m_bkcolor_R;
    int			            m_bkcolor_G;
    int			            m_bkcolor_B;

    std::map<int, bool>     m_IsUserSmall;

    int                     m_NetSignalIndex;
    int                     m_NetSignalLevel;                               //�����źż���
    int                     m_SavedSingalIndex;                             //��һ�η��͵�������Ϣ�����ţ��յ���Ϣ����m_NetSingalIndexͬ��

	bool m_bIsAutoScroll;	//��¼�Ƿ����
	bool m_bIsAcceptChatMsg;	//�Ƿ����������Ϣ
public:
	CBoardCast              *m_dlgBroadcast;                                //���㲥�Ի���
	CPropPannel             *m_pPropPannel;                                 //����������
	CPropSelUser            *m_pPropSelUser;                                //��ǰ����ʹ�õ��ߵ����
	CPBankDlg				*m_pPersonBank;					                //�������н���Ի���
	CPBank4GameWnd			*m_pBank2Wnd;									//�ڶ�����Ϸ���жԻ���

private:
    HMODULE                 m_hAudioMod;
    CREATEAUDIOPLAYC        pCreateAudioPlay;
    DELETEAUDIOPLAYC        pDeleteAudioPlay;
    IAudioPlay              *m_pBackAudio;
    bool                    m_bSndPause;
    // ���������б�
    void LoadBkSoundList(void);
	void OnPropBuyVIP(UINT message,WPARAM wParam,LPARAM lParam);

public:
	vector<MSG_IM_C_GETFRIENDLIST>		m_FriendList;
	INT_PTR GetOnlineUserCount();
	UserItemStruct *FindOnLineUserByIdx(long int dwIdx);
	CChannelService *GetChannelServer() { return m_pIPC;};


	bool		m_bIsQueueDisMiss;
public:
	
	CGameTrottingWnd* m_pPmdWnd; //�����

	XRightFrame* m_pRightFrame;	//��Ϸ�ұ߿�\
									JianGuankun 2012.4.11

	CGameUserListCtrlDu* m_pUserListDu;	//�û��б�

	IButton* m_pShowRightFrameBtn; //��ʾ�ұ߿�İ�ť

	//ԭ���ڳߴ�
	int					m_nWiondwMetricsX;
	int					m_nWiondwMetricsY;

	long int			m_dwCurrentUserID;				//��ǰ�û�
private:
	CSkinMgr			m_skinmgr;
};