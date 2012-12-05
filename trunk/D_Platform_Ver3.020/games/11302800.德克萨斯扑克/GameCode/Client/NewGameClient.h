#include "IGameImpl.h"
#include "UserDataCliObject.h"
#include "UI_ForLogic/IUserInterface.h"
#include "../GameMessage/UpGradeLogic.h"
#include <irrKlang.h>
#include <stack>
#include <queue>

#define GetControlNode(A ,IControl , IControlID ) \
	IControl *A=  dynamic_cast<IControl *>(m_pUI->GetIControlbyID(IControlID)); \
	if (NULL != A) \
	A


using namespace irrklang;

const BYTE MY_VIEWSTATION = 4;				// �Լ�����ͼλ������6

#define IDT_MODIFY_MONEY			 1		// �ȴ�����޸Ĵ�����
#define IDT_COLLECT_JETONS			 3		// �ռ����붯����ʱ��
#define IDT_WAIT_DISPATCH_JETONS	 4		// �ȴ��䷢����
#define IDT_DISPATCH_JETONS			 5		// �䷢���붯����ʱ��
#define IDT_HIDE_BALANCE_TIMER		 6      // ���ؽ����ʱ��
#define IDT_IN_MONEY_BOX			 7      // �����������
#define IDT_MONEY_LACK				 8		// ��ұ�����㶨ʱ��
#define IDT_WIN_DONG_HUA_TIMER		 9		// ʤ��������ʱ��
#define IDT_OK_LOAD					 10		// ȷ�����붨ʱ��
#define IDT_AOUT_IN					 11		// �Զ�������
#define IDT_LOOK_CARD				 12		// ����

/// ������������
#define BT_SEND_BACK_CARD			100		// ����������
#define BT_FLOP_ROUND				101		// ���ƹ�������
#define BT_TURN_ROUND				102		// ת�ƹ�������
#define BT_RIVER_ROUND				103		// ���ƹ�������
#define BT_FLOP_PUBLIC_CARD			104		// ������������
#define BT_FLOP_BACK_CARD			105		// ����������
#define BT_WIN						106		// ʤ������
#define BT_LOSE						107		// ʧ������
#define BT_FOLD_MUSIC				108		// ��������
#define BT_CHIP_MUSIC				109		// ��������
#define BT_TRANSFER_TOKEN			110		// �ƽ���������
#define BT_OK						111		// ȷ����ť����
#define BT_MONEY_BUTTON				112		// ���ѡ������
#define BT_ALARM_CLOCK				113		// ��������
#define BT_CHIP_MONEY_MUSIC			114		// ������Ч

#define ST_BET						115		// ��ע����
#define ST_CALL						116		// ��ע����
#define ST_FOLD						117		// ��������
#define ST_CHECK					118		// ��������
#define ST_RAISE					119		// ��ע����
#define ST_ALL_IN					120		// ȫ������

interface IGameFrame;
interface IUserInterface;
class CNewGameClient : public IGameImpl
{
#define SAFE_CTRL_OPERATE(CTRL_TYPE,CTRL_ID, OPERATE)\
	{\
	CTRL_TYPE* PRE=NULL; \
	PRE = dynamic_cast<CTRL_TYPE *>(m_pUI->GetIControlbyID(CTRL_ID));\
	if (PRE!=NULL) PRE->OPERATE; \
}
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
	/// ����
	virtual int ResetGameFrame(void);
	/// ��ҽ���
	virtual int GameUserCome();
	/// ����뿪
	virtual int GameUserLeft(int nIndex);
	/// ��Ҷ���
	virtual int GameUserOffLine(void);
	/// �Ƿ�������Ϸ������Ϸʵ��
	/// �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
	virtual bool IsPlayingGame();          
	/// ��ʱ����Ϣ
	virtual void OnTimer(int nTimerID);
	/// ����״̬
	virtual void SetStationParameter(BYTE bGameStation);
	/// ����˳�
	virtual bool OnGameQuiting();
	/// ������Ϸ״̬,��Ҫ�Ƕ����������������Ϣ
	virtual bool SetGameStation(void * pStationData, UINT uDataSize);

private:
    IGameFrame                  *m_pGameFrame;
    IUserInterface              *m_pUI;
	BYTE									m_bGameStation;
protected:
	// ��ȡ��Ϸ״̬
	BYTE  GetStationParameter();

	int GetGameStatus()
	{
		if(this)
			return GetStationParameter();
		else
			return 0;
	};

public:
	// ȫ��ʼ��
	void OnInitData();

	//��Ϸ��ʼ
	void OnHandleGameBegin(BYTE * buffer,int nLen);

	//�յ�ϵͳ�������ӵ���Ϣ����������
	void OnHandleGetSysDeskCfg(BYTE * buffer,int nLen);

	//���õ�ע
	void OnHandleCfgDesk(BYTE * buffer,int nLen);

	//�������õ�ע��ʱ            
	void OnHandleWaitCfgDeskTimeout();

	//�յ�ע����Ϣ����������
	void OnHandleGetDeskCfg(BYTE * buffer,int nLen);

	//�յ���������Ϣ����������
	void OnHandleInMoney(BYTE * buffer,int nLen);

	//�յ���������һ���ư���������
	void OnHandleSendACards(BYTE * buffer,int nLen);

	//�յ��������߳ظ��°���������
	void OnHandleBetPoolUp(BYTE * buffer,int nLen);

	//�յ���������һ���ư���������
	void OnHandleSend3Cards(BYTE * buffer,int nLen);

	//�յ���������һ���ư���������
	void OnHandleSend1Cards(BYTE * buffer,int nLen);

	//�յ�����������������ҷ�����ɵ���Ϣ��������
	void OnHandleSendCardFinish();

	//�յ������ע����Ϣ
	void OnHandleGetBetInfo(BYTE * buffer,int nLen);

	//�յ�������Ϣ�������û�
	void OnHandleGetToken(BYTE * buffer,int nLen);

	//���ƴ���ӿ�
	void OnHandleCompareCard(BYTE * buffer,int nLen);

	//�յ�������Ϣ����������
	void OnHandleRoundResult(BYTE * buffer,int nLen);

	//�յ��µ�һ�غϿ�ʼ��Ϣ����������
	void OnHandleNewRoundBegin(BYTE * buffer,int nLen);

	//û����ҽ�����Ϸ���˳�����
	void OnHandleNoPlayer(BYTE * buffer,int nLen);

	//������ϵ�Ǯ���㹻ʱ�Ĵ���ӿ�
	void OnHandleNotEnoughMoney(BYTE * buffer,int nLen);

public:
	// ��ID�ű�ʶ��ʱ�������ã�����Ϊλ�ã����ʱ������;
	void ShowTimer(BYTE bDeskStation, UINT uTimeID, UINT uMaxTime);

	// �����ʱ��
	void ShowTimers(UINT uTimeID, UINT uMaxTime);

	//������ҵõ����Ƶ�UI����
	void OnShowOtherPlayerGetToken(const TToken * pToken = NULL);

	//ϵͳ�Զ�����Ƭ
	void OnUserSortOut();

	//�Լ��õ����Ƶ�UI����
	void OnShowMeGetToken(const TToken * pToken);

	//�Զ��йܴ���
	bool AutoTrusteeship(const TToken * pToken);

	//�йܰ�ť��ʾ/����
	void OnAutoTrusteeship(int nCmdShow);

	//�Լ��¼�UI����
	void OnShowPdg(int nCmdShow);

	//ϵͳ�����������ݺ��UI������Ҫ��ʾ��Ϸ����
	void OnShowSysDeskCfg(const TDeskCfg* pDeskCfg);

	//��������������ݺ��UI������Ҫ��ʾ��/��ע
	void OnShowSubMoney(const TSubMoney * pSubMoney);

	//������Ϣ����
	void OnShowGetToken(const TToken* pToken,BYTE byTime = 0);

	//��ʾ����������ݴ���
	void OnShowPots(const TBetPool* pBetPool);

	//��ʱ��˳��õ��¼�λ��
	BYTE GetNextUserStation(BYTE byStation, BOOL bIsAll = TRUE);

	//��ʱ��˳��õ��¼�λ��
	BYTE GetNextUserStations(BYTE byStation, BOOL bIsAll = TRUE);

	//��ʱ��˳��õ��ϼ�λ��
	BYTE GetLastUserStation(BYTE byStation, BOOL bIsAll = TRUE);

	//��ռ�¼��һ���������ջ�б�
	void ClearAllStack();

	//�ռ���ҳ������Ҵ���ӿ�
	void OnShowCollectJotens(BYTE bDeskStation);

	//ȷ��������
	void OnOKLOAD();

	//��ʾ���Logo
	void OnShowUserLogo();

	//�ر��������ͷ��
	void CloseAllLogo(BYTE bDesk = 255);

	// ��ʾ�û���Ϣ
	void OnShowUserInfo();

	// ��ʼ������UI
	void OnInitYuanSu(bool bShow);

	// ��ʾׯ����Сäע�ı�Ƕ���
	void OnPlayMoveFlagAnim(BYTE bDeskStation,UINT type,bool bHandout);

	// ���ƶ���
	void OnFaPaiAnim();

	// ֹͣ��ʱ��
	void OnEndTimer(BYTE bDeskStation);

	//����䷢����ӿ�
	void OnHandleDispatchJetons();

	//�غϽ�����ʾ��UI������������Ի���
	void OnShowRoundResult(const TResult* pResult);

	//�䷢����UI����ӿ�
	void OnShowDispatchJotens(BYTE bDeskStation,__int64 iMoney);

	//�������ӻ���
	void OnShowDeskTop();

	//������ж�ʱ��
	void OnClearTimer();

	// �Զ�������
	void AutoInBox();

	// ���������Ϸ��Ϣ
	int UserSetGameInfo(bool bSoundEnable,bool bWatchEnable,bool bShow);

	// ���ű�������
	void PlayBeiJingSound();

	// ������������
	void PlayTimerSound();

	// ������Ϸ��ע����
	void PlayBetSound(BYTE bDeskStation,int SoundID);

	// ������Ϸ��������
	void PlayPublicSound(int SoundID);

	// ��Ϸ����
	void FlashWindow();

public:
	// �޸�ע��
	bool OnLoadSelectionDlg(bool bAuto = false);

	// �����ע����ť��ӿ�
	void OnHandleBet();

	// ��Ҹ�ע��ť����ӿ�
	void OnHandleCall();

	// ��Ҽ�ע��ť����ӿ�
	void OnHandleRaise();

	// ��ҹ��ư�ť����ӿ�
	void OnHandleCheck();

	// ������ư�ť����ӿ�
	void  OnHandleFolo();

	// ���ȫ�°�ť����ӿ�
	void OnHandleAllin();

	// ��ҽ�ť����ӿ�
	void OnHandleButtonMoney(int nID);

	// ��һ�����ע�ᴦ��ӿ�
	void OnHandleSlidingAxle();

	// ��Ҽ�עť����ӿ�
	void OnHandleAddMinus();

	// ��Ҽ�עť����ӿ�
	void OnHandleSubMinus();

	// ��Ҹ�ע�йܴ���ӿ�
	void OnHandleCallMandate(bool bFlag);

	// ��ҹ����йܴ���ӿ�
	void OnHandleCheckMandate(bool bFlag);

	// ��Ҹ��κ�ע�йܴ���ӿ�
	void OnHandleCallAnyMandate(bool bFlag);

	// ��ҹ��ƻ������йܴ���ӿ�
	void OnHandlePassAbandonMandate(bool bFlag);

#ifdef CLOSE_CARD
	// ����Լ��ĵ���
	void OnHitMySelfBackCard();

	//�������ҵ���ʱ �ƶ���ҵ���
	void OnHandleMovePlayerCard(BYTE * buffer,int nLen);

	//���չʾ�ư�ť
	void OnHitShowCard();
#endif
public:
	// �ָ���������Ϸ����
	void RecurSaveBase(const LPCVOID lpData);

public:
	//ע����Ϣ
	TDeskCfg m_tagDeskCfg;

	// ��������Ϣ
	TSubMoney m_tagSubMoney;

	// ������Ϣ
	TResult m_tagResult;

	//ÿλ����ֵ�����
	CUserDataCliObject m_UserData[PLAY_COUNT];

	//ׯ��λ��
	BYTE m_byNTUser;

	//Сäעλ��
	BYTE m_bySmallBlind;

	//��äעλ��
	BYTE m_byBigBlind;

	//��������
	BYTE m_byBackCard[MAX_BACKCARDS_NUM];

	//��������
	int m_iBackCardNums;

	//��ǰ��ע���
	__int64 m_nCurrentMoney; 

	// ��ע���
	__int64 m_nCallMoney;

	//��¼��ע���
	int m_iBetCounts;

	//��Ϸ�е����ж���ʱ��
	int m_iVerbTime;

	//ÿ���û����ý��
	__int64 m_nMoney[PLAY_COUNT];

	//ÿ�������ע��� 
	__int64 m_nBetMoney[PLAY_COUNT];

	// �������
	bool m_bGiveUp[PLAY_COUNT];

	// ȫ�����
	bool m_bAllBet[PLAY_COUNT];

	// ��ע�߳�
	TBetPool m_tagBetPool;

	// ��¼ÿ����Ҵ�����
	__int64 m_iMoneys[PLAY_COUNT];

	// ���������ʾ
	BYTE m_iCardData[MAX_HANDCARDS_NUM];

	//������3
	int m_iCardDataCounts;

	// ���г������������
	bool m_bInLisureRoom;

	//��������
	BYTE m_iUserDataCard[MAX_HANDCARDS_NUM];

	//������
	int m_iCardCounts;

	// �Լ���λ��
	BYTE m_byMeStation;

	//���λ��
	int m_iUser;

	//�Ƿ���֪ͨ���������׼����Ϸ
	bool m_bAgreeGame;

	//�õ����Ƶ����
	BYTE m_byTokenUser;

	//�йܱ�־ 0: �Զ���ע(ֻ��һ��)  1: �Զ����κ�ע(ÿ�ֶ���ע)  2: �Զ�����  3: �Զ�����(�ϼ����˹��ƾ͹��ƣ�û�˹��ƾ�����)
	bool m_bTrusteeship[4];

	//��¼ÿ����Ҳ�������
	emType m_emLastOperation[PLAY_COUNT];

	//��¼��һ���������
	stack<BYTE> m_stackLastStation;

	//�Ƿ����й�״̬
	bool m_bTuoGuan;

	__int64 m_iMinMoney;     // ��С������
	__int64 m_iMaxMoney;     // ��������
	__int64 m_iCurrentMoney; // ��ǰҪ����Ľ��
	__int64 m_iSelfMoney;    // �Լ�����ӵ�еĽ��
	__int64 m_iMinusMoney ;  // �޸�ע��Ӽ����

	UINT    m_uTime;         /**< ��ʱ������ */

	// ׯ����Сäע�ƶ�����
	IMoveAction * m_pMove[3];

	// ����ط��ƶ�����
	IMoveAction * m_pAction[8];

	// ���ط��ƶ�����
	IMoveAction * m_pMoveAction[8];

	//����ʾ����
	BYTE m_byShowed[MAX_HANDCARDS_NUM];

	//������Ҷ���
	struct TSendCardAnimNode
	{
		BYTE byUser;
		int  iPos;

		TSendCardAnimNode()
		{
			byUser = 255;
			iPos = 0;
		}
	};

	std::queue<TSendCardAnimNode> m_qSendCards;

	//��������
	emTypeCard					m_nTypeCard;

	bool						m_bTransceiving;

	// ���ص�ׯ��Сäע
	bool						m_bNtDaXiao;	

	int							m_nCount;
	// ���ֿ���
	bool						m_bSoundPlay;	
	// �Ƿ������Թ�
	bool						m_bAllowWatch; 
	// ���ű�����������
	ISoundEngine		*		m_pSoundEngine;
	// ��Ϸ��ע��Ч����
	ISoundEngine		*		m_pTimerSMusic;
	// ��Ϸ������Ч����
	ISoundEngine		*		m_pSEBkMusic;
	// ��Ϸ��ע��Ч����
	ISoundEngine		*		m_pBetSMusic;

	// �Թ���
	bool m_bWatch;

	// �Ƿ������
	int  m_iUserStation;

	// �Ƿ�Ϊ�����ػ����
	bool m_bIsOffline;

	// ���Ϊ��
	bool m_bpUserInfo[PLAY_COUNT];

	// �Զ���ʼʱ����
	bool m_bAutoStartTimer;
	
	// ��ʾ�ռ�������ע
	__int64 m_nBetMoneys;

};