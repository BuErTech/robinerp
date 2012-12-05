#include "IGameImpl.h"
#include "..\GameMessage\UpGradeLogic.h"
// #include "AFCMessageBox.h"
#include <irrKlang.h>

///��ʱ��
#define		TIME_GAME_BEGIN                10    ///<��ʼʱ��

//��ʱ�� ID
#define ID_BEGIN_TIME				    100					//��ʼ��ʱ��
#define ID_OUT_CARD_TIME				110					//���Ƶȴ�ʱ��
#define ID_TIME_CLOCK                   115                 //�������Ӽ�ʱ��
#define ID_PLAY_MOVE_VSCARD             116                 //��ʼ�ƻر��Ƶ��Ƽ�ʱ��
#define ID_TIME_LOOKCARD                117                 //��ʼ�ƻر��Ƶ��Ƽ�ʱ��


#define  SOUND_OUT_CARD_PASS            0x01               //��������
#define  SOUND_OUT_CARD_BOMB            0x02               //ը������
#define  SOUND_OUT_CARD                 0x03               //��������
#define  SOUND_GAME_OVER_WIN            0x04               //ʤ������
#define  SOUND_GAME_OVER_LOSE           0x05               //ʧ������
#define  SOUND_START_DEF                0x06               //��ʼ����
#define  SOUND_WARNING                  0x07               //�澯
#define  SOUND_BG_1					    0x10				//����1
#define  SOUND_BG_2					    0x11				//����2
#define  SOUND_BG_3					    0x12				//����3
#define  SOUND_CHOUMAMOVE               0x13               //�����ƶ�
#define  SOUND_CLICKBUTTON              0x14               //���°�ť��
#define  SOUND_MOVEBUTTON               0x15               //�ƶ�����ť
#define  SOUND_SENDCARD                 0x16               //����
#define  SOUND_VSCARD                   0x17               //���ƶ���

using namespace irrklang;

#define GETCTRL(CTRL_TYPE,PRE,pUI,CTRL_ID)\
	PRE = dynamic_cast<CTRL_TYPE *>(pUI->GetIControlbyID(CTRL_ID));\
interface IGameFrame;
interface IUserInterface;
class CNewGameClient : public IGameImpl
{
public:
    CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI);
    virtual ~CNewGameClient();
private:
    CNewGameClient(const CNewGameClient &other);
    CNewGameClient &operator = (const CNewGameClient &other);


public:
	/// ��ʼ��
	virtual int Initial();
	/// ��ʼ��UI
	virtual int InitUI();
	/// ��Ϸ��Ϣ
	virtual int HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen);
	/// ������Ϣ
	virtual int OnUIMessage(TUIMessage* pMessage);
	/// 
	virtual int ResetGameFrame(void);
	/// ��ҽ���
	virtual int GameUserCome(void);
	/// ����뿪
	virtual int GameUserLeft(int nIndex); 
	/// ��Ҷ���
	virtual int GameUserOffLine(void);
	// �Ƿ�������Ϸ������Ϸʵ��
	// �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
	virtual bool IsPlayingGame();          
	/// ��ʱ����Ϣ
	virtual void OnTimer(int nTimerID);
	/// ��״̬
	virtual void SetStationParameter(BYTE bGameStation);
	///������Ϸ״̬����Ҫ�Ƕ����������������Ϣ
	virtual 	void SetGameStation(void * pBuffer,int nLen);

	virtual bool OnGameQuiting();


private:                       ///��Ϸ�Լ��Ĳ���
//	CAFCMessageBox *m_pMsgDlg;

	// �����Ի���
//	UINT AFCMessageBox(CString strInfo,CString strTitel, UINT style)
//	{
//		if (NULL == m_pMsgDlg)
//		{
//			m_pMsgDlg = new CAFCMessageBox();
//		}
//		else
//		{
//			//m_pMsgDlg->EndModalLoop(IDD_DLG_BTN_CANCLE);
//			m_pMsgDlg = new CAFCMessageBox();
//		}
////		m_pMsgDlg->SetParent(this);
//
//		if (style == DLG_MSG_MB_OK)
//		{
//			m_pMsgDlg->LoadConfig(IDD_MSG_DLG_RES_CONFIRM);		
//		}
//		else if(style == DLG_MSG_MB_YESNO)
//		{
//			m_pMsgDlg->LoadConfig(IDD_MSG_DLG_RES_SELECT);
//		}
//		else if(style == DLG_MSG_MB_YESNO_NEW)
//		{
//			m_pMsgDlg->LoadConfig(IDD_MSG_DLG_RES_SELECT_NEW);
//		}
//		else
//		{
//			m_pMsgDlg->LoadConfig(IDD_MSG_DLG_RES_CONFIRM_NEW);
//		}
//
//		m_pMsgDlg->ShowAllControl(SW_SHOW);
//
//		if(style == DLG_MSG_MB_OK || style == DLG_MSG_MB_YESNO)
//		{
//			m_pMsgDlg->SetMessage(strInfo);	
//		}
//		else
//		{
//			m_pMsgDlg->SetMessage_New(strInfo);	
//		}
//
//		UINT uRet = m_pMsgDlg->DoModal();
//		// ����Ҫ�ǵ�ɾ��
//		delete m_pMsgDlg;
//		m_pMsgDlg = NULL;
//		return uRet;
//	};

	//{{Added by zxd 20090820 ȡ����һ����ң����ƽ�������Ҫ��ȡ�����ĸ���Ҵﵽ��ע����
	BYTE GetNextNoteStation(BYTE bCurDeskStation)
	{
		BYTE bNextStation = bCurDeskStation;

		int kkk = PLAY_COUNT - 1;   //��ʱ��
		for (int i = 0;i < PLAY_COUNT;i++)
		{
			bNextStation = (bNextStation + kkk) % PLAY_COUNT;
			if(m_iDeskCardCount[bNextStation] == 0)
				continue;	

			if (!m_bIsGiveUp[bNextStation])
				break;
		}
		if (bNextStation == bCurDeskStation)
		{
			return 255;
		}
		return bNextStation;

	}

	///��������LOGO
	void ShowLogo();
	///�ر��������ͷ��
	void CloseAllLogo(BYTE bDesk = 255);

	int ViewStation(int desk);

	//��ϷUI����
	void InitGameUI();

	//��������
	void SetHandCard(BYTE bDesk, BYTE iCardList[], BYTE iUpCard[], int iCardCount, int iUpCardCount);

	// ��ʾ/���� �ؼ�
	BOOL ShowControl(int iControlID, bool bShow);

	/// ��ѯ��ť����/���� �ؼ���ֻ֧�ְ�ť��
	BOOL GetControlEnable(int iControlID);

	/// ���ÿ���/���� �ؼ���ֻ֧�ְ�ť��
	BOOL EnableControl(int iControlID, bool bShow);

	/// �趨��Ϸ��ʱ��
	BOOL SetGameTimer(BYTE bDesk, int TimeLen, int TimerID);

	/// ������ʱ��
	BOOL KillGameTimer(int TimerID = 0);

	/// FlashWindowEx��գ�
	void FlashWindow();

	/// ��ѯ��ť��ʾ/���� �ؼ���ֻ֧�ְ�ť��
	BOOL GetControlShow(int iControlID);

	//�й�ֱ�ӵ��ô˺���
	BOOL UserOutCard();

	/// ��ʾ �����
	void ShowResult(GameFinishNotify szResultInfo);

	/// 
	void SetCard();

	//
	void SetNT(int iNTDesk);

	//====��ʼ��Ϸ,��ʼ����
	bool OnControlHitBegin();


	//���������Ϣ
	void SetUserInfo(int iDesk = -1);

	//��Ϸ�߼���������
	void InitGameData();

	//����
	void OnSort();

	void OnLast();

	///���������ļ�
	void LoadData(void);

	///  ������Ч
	void SetPlaySound(int iSoundID, BOOL bIsBoy = TRUE, BOOL bISLoop = FALSE);

	//�����������
	BOOL PlayRandSound(BYTE bStartType, BYTE bEndType, bool isLoop);

	/// �������������
	void SetCardNum(int iDesk,int iNum);

	//���÷�����Ϣ
	void UpdateRoomInfo();

	//��ʼ����ע
	void SetTotalNote(BYTE iViewStation,int iNoteTotal);
	
	//��ע
	void AddTotalNote(BYTE iViewStation,int iNoteTotal);


	void SetMing(BYTE bDeskStation,BOOL iGiveup);

	//�����Լ��Ŀ������
	void UpdateMyLeftMoney();

	//�����Լ�����Ϣ
	void UpDateMyUserInfo();

	//�������е���ʾ��Ϣ
	void DisVisibleAllTip(bool bIsShowGaveUpAndLose=false);  


	//�������еļ�ע��ʾ
	void DisVisibleAllAddTip();
	//�������е���ע��ʾ
	void DisVisibleAllChipinTip();
	//�������еĸ�ע��ť
	void DisVisibleAllFollowTip();
	//�������еķ�����ʾ
	void DisVisibleAllGiveUpTip();    //������ʾ
	void DisVisibleAllOpenLoseTip();  //����ʧ����ʾ
	void DisVisibleAllLookTip();      //������ʾ
	void DisVisibleReachLimiteTip();  //�ﵽ������ʾ
	void DisVisibleAllOpenWinTip();     //�������еı���ʤ����ʾ
	//������ʾ��Ϣ
	void SetTipInfo(BYTE bDeskStation,int iOperateType);

	//��ʾ�������ע״��
	void UpdateNoteInfo(BYTE bDeskStation, int nNoteValue = 0);

	//���ŷ��ƶ���
	void PlaySendCard(BYTE Station);

	void StartPlaySendCard();

	int GetTotalPlayNum();
	//--------------------------���潻����������ť���ƺ���---------------------

	void UpdateShowNoteBtn(bool bOpen=false,bool bAdd=false,bool bGiveUp=false,bool bFollow=false,bool bLook=false,bool bChipIn=false);

	void SetCtrlBtn(bool bIsFirstNote);
	//���ü�ע���ٵİ�ť״̬
	bool UpdatAddNumBtn(bool bFirst);

	//void DrawUserInfo();
	//�������е���Ϸ������ť
	void DisableAllOperaBtn();
	//
	void EnableAddOrNote();
	
	//���üӳ�����ʾ�Ĳ���
	void UpdateAddChipShow(int bMutBase);
	//---------------------------------------------
	inline int GetLimit() {return (m_iLimitPerNote < m_iLimitNote ? m_iLimitPerNote : m_iLimitNote);}

	//��Ϸ��ť����
	bool OnChipIn();   //��ע����
	bool OnFllow();    //��ע����
	bool OnAdd();      //��ע����
	bool OnGiveUp();   //��������
	bool OnLook();     //���Ʋ���  ��Ϊ�������  
	bool OnOpen();     //���Ʋ���


	//��ע���
	bool OnAddCount(int bMutBase);  //���x1,x2,x3��ť
	bool OnAddCancle();              //�����ע�Աߵ�ȡ����ť
	bool OnCompareCard(BYTE bViewStation);	// ������ĳ��ҵı��ư�ť


	//��ע���붯��
	void PlayXianShiChouMa(int nDeskStation, int iNum,bool bFlag);;
	UINT PlayOpenCard(BYTE bDeskStation1,BYTE bDeskStation2,int iCmpRet);
	////�ƻرȽϵĶ���
	UINT MoveVSCard();

	//�ռ�Ӯ�ĳ��붯��
	void PlayGetWinChouMa(int nDeskStation);

	// ���ƶ�������
	void OnFinishOpenCard();

	//�û�Ѻע����(רΪ����׼��)
	bool SetNoteSound(BYTE bNoteStyle,BOOL bIsBoy);

	//��ȡ�����ַ
	void GetRandAddress(CString &Str,BYTE bDeskStation);
	//��ȡ����û�ID
	void GetRandUserID(CString &Str,BYTE bDeskStation);

	/// ��ȡ��Ϸ״̬
	BYTE GetStationParameter(void);

	///�Զ��趨���Ʊ�����С
	void AutoSetBackWH();


private:
    IGameFrame                  *m_pGameFrame;

    IUserInterface              *m_pUI;

	CUpGradeGameLogic		    m_Logic;					//�߼�����

	ISoundEngine               *m_SoundEngine;

	BYTE                         m_bGameStation;               ///<��Ϸ״̬

	///����״̬����

	BYTE					  m_iUserCardCount[PLAY_COUNT];		///<�û������˿���Ŀ

	BYTE					  m_iUserCard[PLAY_COUNT][54];		///<�û����ϵ��˿�

	BYTE					  m_iDeskCardCount[PLAY_COUNT];		///<�����˿˵���Ŀ

	BYTE					  m_iDeskCard[PLAY_COUNT][54];		///<������˿�

	
	///��Ϸ��������
	BYTE                      m_iBeginTime;                     ///<��Ϸ��ʼʱ��

	BYTE                      m_iThinkTime;                     ///<��Ϸ˼��ʱ��

	BYTE                      m_iEndWaitTime;                   ///<��Ϸ������ȴ�ʱ��

	int                       m_iPlayCardCount;                 ///<ʹ���˿���

	int                       m_iRoomBasePoint;                 ///<���䱶��

	int                       m_iDeskBasePoint;                 ///<���ӱ���

	int                       m_iRunPublish;                    ///<���ܿ۷���

	int                       m_iBeenPlayGame;                  ///��Ϸ����
 
	int                       m_iCurrentTimeId;                 ///��ǰ��ʱ��ID

	BOOL                      m_bAutoCard;                      ///�Ƿ��й�

	BYTE                      m_bSortCard;                      ///��������

	BOOL                      m_bSendTestCard;                  ///�Ƿ������Է���

	BOOL                      m_bSound;                        ///�Ƿ�����Ч

	int                       m_TimeClockCount;                ///���ӵ���ʱʱ��

	BYTE                      m_bTimeOutCount;                 ///��ʱ����

	bool					  m_bInLisureRoom;				//����Ϸ�Ƿ������з�


	__int64						m_iNowBigNote;				//��ǰ���Ѻע��
	__int64						m_iThisGameNote[PLAY_COUNT];			//������Ϸ��ע��(��ע)	
	__int64						m_iTotalGameNote[PLAY_COUNT];			//������Ϸ��ע��(ʵ��)


	BYTE					   m_iThisNoteTimes;			//Ѻע����
	int                        m_iFrontNote;                //��ǰ��Ч��ע����ǰλ�����Ч��ע��;
	int                        m_iMyAddNote;                //�Ҽ�ע��ֵ 
	//������Ϣ
	__int64						   m_iLimitNote;				//����޶�ע
	int						   m_iBaseNote;				//��ע
	int                        m_iGuoDi;                   //����
	__int64                        m_iLimitPerNote;            //ÿע���ƣ�Ϊ��ע�����ƣ�����ʾΪ��ע������

	__int64						   m_iThisTurnLimit;			//������ע
	//��ʾ��һ�����ʾid
	BOOL m_CbIsShowIdOrPlay;

	CString                    m_strText[20]; //�û����������ַ

	CString                    m_strCityText[PLAY_COUNT];
	CString                    m_strUserIDText[PLAY_COUNT];
	bool                       m_bSuperUser[PLAY_COUNT]; 
	bool                       m_bLookedPai;
	int						   m_iNoteTotalPoint;					//��ע
	int						   m_iNoteLimit;						//��������
	BOOL                       m_bMing[PLAY_COUNT];                        //����
	int						   m_iNowOutPeople;			//���ڳ�����

	int						   m_iFirstOutPeople;			//��һ��������
	bool                       m_bOpenLose[PLAY_COUNT];     //��ҵı��ƽ��
	bool                       m_bIsGiveUp[PLAY_COUNT];     //����Ƿ����

	

	BYTE                       m_bNtPeople;   //ׯ��
	bool                       m_bFirstGame;
	BYTE                       m_byWin[PLAY_COUNT];
	BYTE                       m_iComWinStation;			// ����ʤ��
	BYTE                       m_iComLoseStation;			// ���Ƹ���
	BYTE                       m_iComFinishWinner;			// ���ƽ�����������Ϸ
	BYTE                       m_bReachLimitStation;       //�ﵽ�������
	bool                       m_bGameFinishAndComp;        //�Ƿ������Ϸ����
	BYTE                       m_bComCardStation[2];         //���Ƶ�����λ��
	POINT                      m_poComCardXY[2];               //�������Ƶ�����λ��
	bool                       m_bMovCardStart;            //�Ƿ������ƶ����׶�

	BYTE                       m_bLookCardW;               //���Ƶļ�����
	BYTE                       m_bSendCardPos;             //��ǰ���ƶ�������
	BYTE                       m_bSendCardCount[PLAY_COUNT]; //��ǰ���ƶ���������

	BOOL                       m_bPlayer[PLAY_COUNT]; //�Ƿ���һ���;�����ս��

};