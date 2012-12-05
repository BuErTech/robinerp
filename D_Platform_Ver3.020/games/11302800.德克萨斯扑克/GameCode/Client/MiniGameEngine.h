#pragma once

#include "defines.h"
#include "../../../../sdk/include/GameLanchSdk/LoveSendClassInExe.h"
#include "../../../../sdk/include/GameLanchSdk/BZAnima.h"
#include "AFCMessageBox.h"
#include "FinishDlg.h"
#include "LoadSelection.h"

#include "GameMachine.h"

#include <irrKlang.h>
using namespace irrklang;

#include "SoundCfgDlg.h"

#define MAST_PEOPLE			-1							//�Ƿ�Ϊ����Ա
//��Ϣ����
#define IDM_BEGIN			WM_USER+120					//��ʼ��ť��Ϣ
#define IDM_HAVE_THING		WM_USER+127					//����Ҫ��
#define IDM_SEND_THING		WM_USER+128					//�����뿪����
#define IDM_ARGEE_LEAVE		WM_USER+129					//ͬ���뿪��Ϣ
#define IDM_STOP			WM_USER+130					//��ǰ������ť
#define IDM_STOP_THING		WM_USER+131					//������ǰ����
#define IDM_ARGEE_STOP		WM_USER+132					//ͬ����ǰ����
#define IDM_PASS			WM_USER+133					//������ť��Ϣ
#define IDM_AUTO			WM_USER+134					//�й���Ϣ
#define	IDM_ACTIVE_RESULT	WM_USER+136					//�û�������
#define IDM_SHOW_NOTE		WM_USER+137					//�û�ѡ����ע,��ע
#define IDM_SELECT_NOTE_NUM	WM_USER+138					//�û���ע
#define	IDM_SOUND			WM_USER+139					//�û���������
#define IDM_BGSOUND			WM_USER+140					//��������

//���ú�������
#define RS_ALL_VIEW					1					//�������в���
#define RS_GAME_CUT					2					//�û��뿪����
#define RS_GAME_END					3					//��Ϸ��������
#define RS_GAME_BEGIN				4					//��Ϸ��ʼ����
#define GAME_RAND_TIME				5						//ϵͳʱ��

const BYTE MY_VIEWSTATION = 5;				// �Լ�����ͼλ������6

#define TT_NONE			1	//��ʾ���ͣ�û����ʾ
#define TT_READY		2	//��ʾ���ͣ�׼����Ϸ
#define TT_GENG_FAILD	3	//��ʾ���ͣ�����ʧ��
#define TT_THROW_FAILD	4	//��ʾ���ͣ�˦��ʧ��

#define IDT_CLOCK					204 //�ӣ���������ʱ�䲻��ʱ����ʾ
#define IDT_GENG_FAILD				205	//����ʧ����ʾȫ�Ƶ�ʱ��
#define IDT_THROW_FAILD				206	//˦��ʧ����ʾȫ�Ƶ�ʱ��
#define IDT_THROW_SHOW_BASE_CARD	207	//ׯ����ʾ���Ƶ�ʱ��
#define IDT_LAST_ROUND_OUTCARD		208	//�����ʾ��һ���Ƶ�ʱ��
#define	IDT_OFF_BALANCE				209	//���ؽ����ʱ��
#define	IDT_AUTO_READY			    210	//�Զ�׼����ʱ��

//��Ϸ����� 
class CMiniGameEngine : public CLoveSendClassInExe
{

public:

	//���캯��
	CMiniGameEngine();

	//��������
	virtual ~CMiniGameEngine();

DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

/*-----------------------------------------------------------------------------------------------------------
	һϵ��ƽ̨�ṩ����Ҫ������������Ϸ�ĺ����������ô������Ϸ�Ŀ�������ÿ��������ʵ������Ϸ���졣
  ----------------------------------------------------------------------------------------------------------*/
public:

	//��ʼ���ڣ�UI�ؼ�
	virtual BOOL OnInitDialog();

	// ������Ϸ״̬
	virtual void ResetGameStation(int iGameStation=1);

	//������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize);

	//�������ý���
	virtual void ResetGameFrame();

	//��ͼת��
	virtual BYTE ViewStation(BYTE bDeskStation);

	//��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize);

	//��BzMainFrame�̳������ĺ���
	virtual bool OnPostMessage(UINT pId,BBaseParameter* pParam);

	//������Ϣ
	virtual void ShowUserTalk(MSG_GR_RS_NormalTalk *pNormalTalk);

	//�û��뿪
	virtual bool GameUserLeft(BYTE bDeskStation, UserItemStructInExe * pUserItem, bool bWatchUser);

	//�û�����Ϊ��Ƶģ�����ش˺���
	virtual bool GameUserCome(BYTE bDeskStation, UserItemStructInExe * pUserItem, bool bWatchUser);

	//��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount);

	//�رմ���
	virtual bool AFCCloseFrame();

	//�˳�
	virtual void OnCancel();

	//��ȫ������Ϸ
	virtual bool OnControlSafeFinish(){return false;};


/*-----------------------------------------------------------------------------------------------------------
	    ����һϵ���������к����������ô������Ϸ�Ŀ�������ÿ��������ʵ������Ϸ���졣
    ����ͨ����ʾ��״̬����ʱ����Ч��ͨѶ����Դ���յȷ��档
  ----------------------------------------------------------------------------------------------------------*/
public:	

/*-ͨ����Դ�������-*/
public:

	//��������LOGO
	void ShowLogo();

	//�������������ʾ
	void ShowUserInfo(void);

	//�ر��������ͷ��
	void CloseAllLogo(BYTE bDesk = 255);

	//�ؼ���ʾ������
	void DispalyAllControl(int iShow);


/*-��ʱ���-*/
public:

	//��ID�ű�ʶ��ʱ�������ã�����Ϊλ�ã����ʱ������;
	void ShowFlashTimer(BYTE bDeskStation,UINT uMaxTime,UINT uTimeID);

	//ֹͣ�����ض�ʱ��
	void StopAndHideFlashTimer(BYTE bDeskStation);

	//ʱ�䶯����������ñ��������������жϸ���ʲô��������
	void OnFlashTimer(BYTE bDeskStation, UINT uEventID);

/*-�������-*/
public:

	//����Flash����������Ϣʱ�ĺ���
	LRESULT FlashEndMessage(WPARAM wParam, LPARAM lParam);

	// ��ȡFlash�е��ƺ�
	BYTE GetFlashCardValue(BYTE bCardValue);

/*-ͨ����ʾ���-*/
public:

	//�ж��Ƿ�����ʾ�����
	bool IsResultDlgShowing();

	//ͨ����ʾ��
	UINT AFCMessageBox(CString strInfo,CString strTitel, UINT style);

	//���������Ч���UI����
	void OnShowCfgSound();

/*-��Ч���-*/
public:

	//����������Ч
	void Speak(BYTE bDeskStation,DZPKSound::SpeakType st_type,DZPKSound::LanguageType lt_type = DZPKSound::LanguageType::LT_COMMON);

	//������Ϸ��Ч
	ISound* PlayBaseSound(DZPKSound::BaseType type, bool bMusic = false);


/*-ͨѶ���-*/
public:

	//����������Ϣ����
	int SendGameData(BYTE bMainID,BYTE bAssistantID, BYTE bHandleCode);

	//����������Ϣ����
	int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);


/*-��Դ�������-*/
public:

	//�˳���Ϸʱ�ж�4
	bool OnCloseGame();

	//��ȫ�رս����
	bool SafeCloseFinishDlg();

	//��ȫ�ر��޸�ע��Ի���
	bool SafeCloseLoadSelectDlg();

	//�޸�ע��Ի���
	bool OnLoadSelectionDlg(bool bAuto = false);

	//��ȫ�ر��������ÿ�
	bool SafeCloseSoundCfgDlg();

	//�ر����ж���
	void SafeCloseFlash(UINT &ruFlashId);

/*-����-*/
public:
	//��ȡ�����Ϣ
	UserItemStructInExe * GetUserInfo(BYTE bDeskStation) { return m_pUserInfo[bDeskStation]; }

	//ȡ����Ϸ״̬
	int	GetGameStatus();

	//�޸���Ϸ״̬
	void SetGameStatus(BYTE bGameStation);

/*----------------------------------------------------------------------------------------------------------------
	һϵ���������Žӿڣ����ڣ�������ʵ��������Ⱦ�����⣨��GameEngine����¶�����ӿڣ�ÿ����Ϸ����ͬ
  ---------------------------------------------------------------------------------------------------------------*/
public:

/*----------------------------------------------------------------------------------------------------------------
	һϵ�н�����µĽӿڣ����ڣ�������ʵ��UI��Ⱦ�����⣨��GameEngine����¶�����ӿڣ�ÿ����Ϸ����ͬ
  ---------------------------------------------------------------------------------------------------------------*/
/*-���ڳ�ʱ��UI����-*/
public:

	//��ʾ���ӻ���
	void OnShowTableEnv();

	//�������ӻ���
	void OnShowDeskTop();

	//���������ҵ��ƿؼ�
	void OnClearCard();

	//���õ�עUI����
	bool OnShowCfgDesk(const TCfgDesk* pNoteSelect);

	//�ȴ����õ�עUI����
	bool OnShowWaitCfgDesk();

	//ϵͳ�����������ݺ��UI������Ҫ��ʾ��Ϸ����
	bool OnShowSysDeskCfg(const TDeskCfg* pDeskCfg);

	//���ѡ���ע��ť���UI�仯
	void OnShowClickedBottomCasting();

	//�ȴ����õ�ע��ʱUI����
	void OnShowWaitCfgDeskTimeout();

	//����Ƿ�ͬ���ע���UI�仯
	void OnShowClickedAgreeDeskCfg();

	//��������������ݺ��UI������Ҫ��ʾ��/��ע
	bool OnShowDeskCfg(const TDeskCfg* pDeskCfg);

	//��������������ݺ��UI������Ҫ��ʾ��/��ע
	bool OnShowSubMoney(const TSubMoney * pSubMoney);

	//��ʾׯ�ҵı�Ƕ���
	void OnPlayMoveFlagAnim(BYTE bDeskStation,UINT type,bool bHandout);

	//��ʾׯ�ҵı��
	void OnShowNTFlag(const TNextRoundBeginInfo* pNextRoundBeginInfo);

	//�����Ч���ð�ť��UI�ı仯����
	bool OnShowClickedSoundCfg();

	//�Լ��õ����Ƶ�UI����
	bool OnShowMeGetToken(const TToken * pToken);
	
    //�Զ��йܴ���
    bool AutoTrusteeship(const TToken * pToken);

	//��ʾ��һ��ҽ�ҵ���ע��ťUI����
	bool OnShowCall();

	//�յ���ע��Ϣ���UI�仯
	bool OnShowGetBetInfo(const TBet* pBet);

	//������ҵõ����Ƶ�UI����
	bool OnShowOtherPlayerGetToken(const TToken * pToken = NULL);

	//�йܰ�ť��ʾ/����
	void OnAutoTrusteeship(int nCmdShow);

	//�Լ��¼�UI����
	bool OnShowPdg(int nCmdShow);

	//�ռ�����UI����ӿ�
	bool OnShowCollectJotens(BYTE bDeskStation);

	//��ճ���UI����ӿ�
	bool OnRemoveCollectJotens();

	//�䷢����UI����ӿ�
	bool OnShowDispatchJotens(BYTE bDeskStation,int iMoney);

	//�غϽ�����ʾ��UI������������Ի���
	void OnShowRoundResult(const TResult* pResult);

	//��һ�غ�׼�������������ȴ���
	void OnShowNextRoundReady();

	//�»غϿ�ʼ��UI����
	void OnShowNewRoundBegin(const TNextRoundBeginInfo* pNextRoundBeginInfo);

	//��1���Ƶ�UI����
	void OnShowACard(const TCards* pCards);

	//��1���ƵĶ�������
	void OnPlayACardAnimate(BYTE bDeskStation,int nIndex);

	//��1���ƣ����˶������󣬵�UI����
	void OnEndSendMeACard(int nIndex);

	//��ʾ�߳����ݴ���
	void OnShowPot(const TBetPool* pBetPool);

	//��ʾ����������ݴ���
	void OnShowPots(const TBetPool* pBetPool);

	//��3���Ƶ�UI����
	void OnShow3Card(const TCards * pCards);

	//��1���Ƶ�UI����
	void OnShow1Card(const const TCards * pCards);

	//û������ڳ���UI����������ע�����ʾ
	void OnShowNoPlayer(const TNoPlayer* pNoPlayer, bool bHaveResultDlg = false);

	//���ͬ����Ϸ��UI����
	void OnShowAgree(BYTE bDeskStation);

	//�������ͬ����Ϸ��UI����
	void OnShowAllAgree();

	//��Ҳ�ͬ����Ϸ��UI����
	void OnShowDisAgree(BYTE bDeskStation);

	//�������Ǯ������UI����
	void OnShowNotEnoughMoney(const TMoneyLack * pMoneyLack);


/*-���ڳ�ʱ��UI����-*/
public:
	
	//�ȴ����õ�עUI����
	void OnTimeoutCfgDesk();

	//�ȴ����õ�עUI����
	void OnTimeoutAgreeDeskCfg();

	//�ȴ���һ�غϵĳ�ʱ���UI����
	void OnTimeoutNextRoundBegin();

	//���Ƴ�ʱ���UI����
	void OnTimeoutOutCard();

/*-----------------------------------------------------------------------------------------------------------------
	����ͨ�ý�����������������ڲ�ʹ�ã�ÿ����Ϸ����ͬ
  ----------------------------------------------------------------------------------------------------------------*/
private:

	//��ʼ���ӿؼ�
	void OnInitClocks();

	//��ʼ�����ʾ�ؼ�
	void OnInitMoneyStatic();

	//��ʼ�����ؼ�
	void OnInitHanHuaControl();

	//��ʼ�����ƿؼ�
	void OnInitCardsControl();

	//��ʼ������ʾ�ؼ�
	void OnInit510KPoints();

	//����������ʾ���͵�UI�ؼ�
	void HideAllNoticeControl();

	//��ʾ���Ƹ�ĳ��ҵĶ���
	void ShowCardFlash(BYTE bDeskStation, BYTE bCard,int nIndex = 0);

	//������ʼ���ƶ���
	void ShowCardFlashAction(void);

	//��ʾ�շ������ƣ��������δ��ʾ�������ƣ��򷵻ط�0���޴�����������0
	int ShowNextCardFlashAction(void);

	//����������ϵ��˿���
	void SetOutCardsTo(BYTE bDeskStation,BYTE* pCard,BYTE* pUpList,int nCount,int nUpCount);

	//�õ���������˿���
	void GetHandCards(BYTE bDeskStation,BYTE* pCard,BYTE* pUpList);

	//��ʾ������ϵ��˿���
	void ShowHandCards(BYTE bDeskStation,bool bShow = true);

	//���ĳ����ѳ�����
	void ClearOutCardTo(BYTE bDeskStation);

	//���ĳ������ϵ���
	void ClearHandCardTo(BYTE bDeskStation);

	//ȡ���붯��λ��
	bool GetJetonAnimRect(BYTE bDeskStation,CRect &rcStart,CRect &rcEnd);


/*-----------------------------------------------------------------------------------------------------------------
	������������ݣ�������Щ���������ڴ��������Ϸ����ʱʹ��
  ----------------------------------------------------------------------------------------------------------------*/
public:

	// �����Ի���
	CAFCMessageBox *m_pMsgDlg;

	//��Ϸ״̬��
	CGameMachine m_GameMachine;

	//��Ч����
	ISoundEngine* m_pSEBase;
	ISoundEngine* m_pSEHurryUp;
	ISoundEngine* m_pSESpeak;
	ISoundEngine* m_pSEBkMusic;

	//�����
	CFinishDlg* m_pFinishDlg;	

	//�޸�ע���
	CLoadSelectionDlg* m_pLoadSelectionDlg;

	//��Ч���ÿ�
	CSoundCfgDlg* m_pSoundCfgDlg;

	//��ʾID�������
	bool m_bShowPlayerID;

	//�������淢�ƶ������ݵĽṹ
	struct tagSendCardFlash
	{
		//�ĸ����
		BYTE byUser; 

		//ʲô��
		BYTE byCard;

		//�ڼ�����
		int  nIndex; 

		tagSendCardFlash():\
			byUser(255),byCard(0),nIndex(0){}
	};

	//����״̬��������
	std::list<tagSendCardFlash>	m_lstSendCardFlash;

	//ʱ�Ӽ���
	int m_iClockCounter;

/*-----------------------------------------------------------------------------------------------------------
	������������ݣ�������Щ����ֻ������������Ϸ����ʱʹ��
  ----------------------------------------------------------------------------------------------------------*/
private:

	//��ʱ��������� 
	UINT m_uTimerFlashID[PLAY_COUNT];

/*-�ؼ�ָ��-*/
private:

	//��������ͼƬָ��
	BzNumberPanel *m_pClocks[PLAY_COUNT];

	//�ƿؼ�ָ��
	CUpGradeGameCard* m_pOutCards[PLAY_COUNT];

	//�Լ����ϵ��ƿؼ�ָ��
	CUpGradeGameCard* m_pMyHandCards;

	//��������ϵ����˶��ؼ�
	BzAnimateNode* m_pBackCardAnim[PLAY_COUNT*2];

	//��������ϵĳ����˶��ؼ�
	BzAnimateNode* m_pJetonAnim[PLAY_COUNT];

	//����־���˶��ؼ�
	BzAnimateNode* m_pFlagAnim[3];

	//���λ��
	int m_iUser;

public:
	// ��¼ÿ����Ҵ�����
	int m_iMoneys[PLAY_COUNT];

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
};
