#ifndef _MJ_FRAME_LOGIC_H_
#define _MJ_FRAME_LOGIC_H_

// �齫�ͻ��˿���߼���
#include "IGameImpl.h"
#include "Common/UIMessageDefines.h"
#include "UI_ForLogic/IUserInterface.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "MJControlsDefine.h"
#include "MJSoundDefines.h"
#include "IGameFrame.h"
#include "Gameroommessage.h"
#include "commonuse.h"
#include "basemessage.h"
#include "XFanCounter.h"
#include "irrKlang/irrKlang.h"
using namespace irrklang;

#define GETCTRL(CTRL_TYPE, PRE, pUI, CTRL_ID) \
	(( PRE = dynamic_cast<CTRL_TYPE *>(pUI->GetIControlbyID(CTRL_ID)) ) != NULL) \

#define SAFE_CTRL_OPERATE(CTRL_TYPE, CTRL_ID, OPERATE) \
{\
	CTRL_TYPE* PRE = NULL; \
	PRE = dynamic_cast<CTRL_TYPE *>(m_pUI->GetIControlbyID(CTRL_ID));\
	if (PRE != NULL) PRE->OPERATE; \
}

/// char����תwchar_t����
static void C2W(char *pchar, wchar_t * wchar)
{
	int nwLen = MultiByteToWideChar(CP_ACP, 0, pchar, -1, NULL, 0); 
	MultiByteToWideChar(CP_ACP,0,pchar,-1,wchar,nwLen);
}

/// wchar_t����תchar����
static void W2C(wchar_t * wchar, char *pchar)
{
	int len = WideCharToMultiByte(CP_ACP, 0, wchar, (int)wcslen(wchar), NULL, 0,NULL,NULL);

	// ���ֽڱ���ת���ɶ��ֽڱ���   
	WideCharToMultiByte(CP_ACP, 0, wchar, (int)wcslen(wchar), pchar, len, NULL, NULL);
}

// ƽ̨UI��Ϣ�¼�(�����޸Ĵ�ID)
#define  UI_TIME_RUN     316     /// ʱ��ؼ�������Ϣ���������ŵ���ʱ����

// ƽ̨��ʱ��ID���巶Χ10-40
#define TIME_FRAME_MAX_VALUE 40  /// ƽ̨��ʱ��ID���ֵ
#define TIME_DEAL            10  /// ���ƶ�ʱ��
#define TIME_DICE_FINISH     11  /// ������ɶ�ʱ��
#define TIME_FIND_POS        12  /// ����λ�ö�ʱ��
#define TIME_HAND_MOVE       13  /// С��ָ�ƶ���ʱ��
#define TIME_SHOW_FINISH     14  /// ��ʾ����

/** 
 * @brief ������Ϣ���ݽṹ 
 */
struct ST_BLOCKINFO
{
	BYTE    byFlag;          /**< ���Ʊ�ʶ: ��(0x01), ��(0x02) ��(0x04), ��(0x08), ��(0x10) */
	CMJSet  tilesSet;        /**< ��������: �ԡ������� */
	CMJHand tingGive;        /**< ���ƿ��Ժ��ȥ���� */
	CMJHand tingWin[20];     /**< ���ƴ����Ӧ�����ƺ���Ժ��ļ�����, ÿ���±��� mjTilesTingGive����ÿ����ֵ��Ӧ*/
	
	void ReleaseAll()
	{
		memset(this, 0, sizeof(ST_BLOCKINFO));
	}   
};

/** 
 * @brief ����״̬����
 */
enum ENM_DICE_STATE
{
	En_Dice_None = 0,        /**< û��״̬ */
	En_Dice_1,               /**< ��1������, ȷ����һλ�����ӵ���� */
	En_Dice_2,               /**< ��2������, ȷ��ץ��λ�� */
	En_Dice_3,               /**< ��3������, ȷ�����Ƶ�λ�� */
	En_Dice_4,               /**< ��4������, �������� */
};

/** 
 * @brief �����ṹ����
 */
#define MAX_SOUND_RAND 5    // �м��ֲ�һ��������

struct SOUND_INFO
{
	char szFileName[MAX_SOUND_RAND][MAX_PATH];
	int nCount;       

	SOUND_INFO()
	{
		memset(szFileName, 0, sizeof(szFileName));
		nCount = 0;
	}
	SOUND_INFO(const SOUND_INFO &other)
	{
		memcpy(szFileName, other.szFileName, sizeof(SOUND_INFO));
		nCount = other.nCount;
	}
	SOUND_INFO &operator = (const SOUND_INFO &other)
	{
		if (&other==this)
		{
			return *this;
		}
		memcpy(szFileName, other.szFileName, sizeof(SOUND_INFO));
		nCount = other.nCount;
		return *this;
	}
};

interface IGameFrame;
interface IUserInterface;
class CMJFrameLogic : public IGameImpl
{
public:
    CMJFrameLogic(IGameFrame *pGameFrame, IUserInterface *pUI);
    virtual ~CMJFrameLogic();

private:
    CMJFrameLogic(const CMJFrameLogic &other);
    CMJFrameLogic &operator = (const CMJFrameLogic &other);

public:
    /** 
	 * @brief ��ʼ��
	 */
    virtual int Initial();

	/** 
	* @brief ��ʼ��UI
	*/
    virtual int InitUI();

	/** 
	 * @brief �Զ����ʹ������
	 */
	virtual void InitData();


	/** 
	 * @brief ��Ϸ��Ϣ
	 * @param [in] nMainID ��Ϸ��ID
	 * @param [in] nSubId  ��Ϸ��ID
	 * @param [in] buffer  ��Ϸ���ݰ�
	 * @param [in] nLen    ��Ϸ���ݰ��ĳ���
	 * @return 
	 */
    virtual int HandleGameMessage(WORD nMainID, WORD nSubId, BYTE * buffer, int nLen);

	/** 
	 * @brief ������Ϣ
	 * @param [in] pMessage ��Ӧ��������Ŀؼ���Ϣ
	 */
    virtual int OnUIMessage(TUIMessage* pMessage);
    
	/** 
	 * @brief ������Ϸ
	 */
    virtual int ResetGameFrame(void);

	/** 
	 * @brief ��ҽ���
	 */
    virtual int GameUserCome(void);

	/** 
	 * @brief ��ҽ���
	 * @param [in] nIndex ��Ϸ����λ��
	 */
	virtual int GameUserCome(int nIndex);

	/** 
	 * @brief ����뿪
	 */
    virtual int GameUserLeft(int nIndex);

	/** 
	 * @brief ��Ҷ���
	 */
    virtual int GameUserOffLine(void);

	/** 
	 * @brief �Ƿ�������Ϸ������Ϸʵ��
	 * @return �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
	 */
    virtual bool IsPlayingGame();   

	/** 
	 * @brief ��ʱ����Ϣ
	 * @param [in] nTimerID ��ʱ��ID
	 */
    virtual void OnTimer(int nTimerID);

	/** 
	 * @brief ����״̬
	 * @param [in] bGameStation ��Ϸ״̬
	 */
    virtual void SetStationParameter(BYTE bGameStation);

	/** 
	 * @brief 
	 */
	virtual bool OnGameQuiting();

protected:
    IGameFrame     * m_pGameFrame;            /**< ��Ϸ��ܶ��� */
    IUserInterface * m_pUI;                   /**< ��ϷUI���� */
	ISoundEngine   * m_pSoundEngine;	      /**< ��Ϸ�������� */
	
	std::map<BYTE, SOUND_INFO> m_SoundMap;    /**< ������Դ���� */

protected:
	wchar_t m_wNickName[PLAYER_NBR][MAX_PATH];/**< ��Ϸ�ǳ� */
	int     m_nTempMoney[PLAYER_NBR];         /**< ��ʱ��ұ仯��Ϣ */
	int     m_nTimeOutCount;                  /**< ��ʱ���� */
	bool    m_bSound;                         /**< �������� */

protected:
	CMJHand m_TilesHand[PLAYER_NBR];          /**< ������� */
	CMJGive m_TilesGive[PLAYER_NBR];          /**< ��ҳ��� */
	CMJSet  m_TilesSet[PLAYER_NBR];           /**< ������� */
	CMJHand m_TilesFlower[PLAYER_NBR];        /**< �������� */
	CMJWall m_TilesWall;                      /**< ��ǽ */
	CMJHand m_TilesGod;                       /**< ���� */
	BYTE    m_byDice[DICE_NBR];               /**< ���� */
	BYTE    m_byCurDice[2];                   /**< ��ǰ���� */
	BYTE    m_byDealer;                       /**< ׯ��λ�� */
	BYTE    m_byMyStation;                    /**< �Լ���λ�� */
	BYTE    m_byTurnStation;                  /**< ��ǰ������ */
	TILE    m_LastTile;                       /**< ��һ�δ���� */
	BYTE    m_byGameState;                    /**< ��Ϸ״̬ */
	BYTE    m_byStateSelect;                  /**< ����ѡ��״̬ */
	MJ_SETTING_DATA m_tagSetting;             /**< �������� */
	UserInfoForGame m_tmpUserInfo;            /**< ��ʱ�û����� */
	ST_BLOCKINFO    m_stBlockInfo;            /**< ������Ϣ */
	bool    m_bReady[PLAYER_NBR];             /**< ׼�� */
	bool    m_bTuoGuan[PLAYER_NBR];           /**< �Ϲ� */
	
	BYTE    m_byDiceFinishCount;              /**< ���Ӳ�������� */
	BYTE    m_byGangCount;                    /**< �������� */
	ENM_DICE_STATE  m_enDiceState;            /**< ����״̬ */

	TCMD_NOTIFY_DEAL m_stDealInfo;            /**< ������Ϣ */
	TCMD_NOTIFY_DRAW m_stDrawInfo;            /**< ץ����Ϣ */
	TCMD_NOTIFY_FINISH  m_stFinishInfo;		  /**< ������Ϣ */

	int     m_nSmallHandCount;                /**< С��ָ�ƶ����� */

	/// ������Ϣ���պ���
public:
	/////////////////////////////////////////////////////////////////////////
	/// ��������յ���Ϣ����
	/**
	 *  @brief ���������Ϣ
	 */	
	virtual void OnNotifyBaseInfo(BYTE * buffer, int nLen);

	/**
	 *  @brief ���ܵ����롢�ػء����ݻָ���Ϣ
	 */	
	virtual void OnNotifyGameInfo(BYTE * buffer, int nLen);

	/**
	 *  @brief ��������������Ϣ(��Ϸ��ʼ���͵ĵĵ�һ����Ϸ��Ϣ)
	 */	
	virtual void OnNotifyRunNbr(BYTE * buffer, int nLen);

	/**
	 *  @brief ���ܵ�֪ͨׯ��λ����Ϣ
	 */	
	virtual void OnNotifyDealer(BYTE * buffer, int nLen);

	/**
	 *  @brief ���ܵ����ַ�����Ϣ��
	 */	
	virtual void OnNotifyDeal(BYTE * buffer, int nLen);

	/**
	 *  @brief ������������Ϣ
	 */	
	virtual void OnNotifyFlowerOver(BYTE * buffer, int nLen);

	/**
	 *  @brief ���ܵ�������Ϣ
	 */	
	virtual void OnNotifyDraw(BYTE * buffer, int nLen);

    /**
	 *  @brief ���ܵ�������ƽ����Ϣ
	 */	
	virtual void OnNotifyGive(BYTE * buffer, int nLen);

	/**
	 *  @brief ���ܵ�������Ϣ
	 */	
	virtual void OnNotifyBlock(BYTE * buffer, int nLen);

	/**
	 *  @brief ���ܵ��������ƽ����Ϣ
	 */	
	virtual void OnNotifyBlockResult(BYTE * buffer, int nLen);

	/**
	 *  @brief ���ܵ�������Ϣ
     */	
	virtual void OnNotifyFinish(BYTE * buffer, int nLen);

	/**
	 *  @brief ���ܵ��޸��Թ�״̬��Ϣ
	 */	
	virtual void OnNotifyChangeLookOn(BYTE * buffer, int nLen);

	/**
	 *  @brief ���ܵ�ˢ��������Ϣ
	 */	
	virtual void OnNotifyUpdateHandTiles(BYTE * buffer, int nLen);
	
	/**
     *  @brief ���ܵ�GM������Ϣ��ִ�е��¼�
     */
    virtual void OnNotifyGMCmd(BYTE * buffer, int nLen);
	/////////////////////////////////////////////////////////////////////////


	/// ������ƺ���
public:
	/////////////////////////////////////////////////////////////////////////
	/// UI������ػ��ƺ���
	/** 
	 * @brief ���������Ϣ
	 * @param[in] byChair    ���λ��
	 * @param[in] stUserInfo �����Ϣ
	 * @param[in] bShow      �Ƿ���ʾ
	 */
	virtual void DrawUserInfo(BYTE byChair, UserInfoForGame stUserInfo, bool bShow = true);

	/** 
	 * @brief ����׼��
	 * @param[in] byChair ���λ��
	 * @param[in] bShow   �Ƿ���ʾ
	 */
	virtual void DrawReady(BYTE byChair, bool bShow = true);

	/** 
	 * @brief ��������
	 * @param[in] byChair ���λ��
	 * @param[in] byDice  ����
	 * @param[in] bShow   �Ƿ���ʾ
	 */
	virtual void DrawDice(BYTE byChair, BYTE byDice[], bool bShow = true);

	/** 
	 * @brief �����й�
	 * @param[in] byChair ���λ��
	 * @param[in] bShow   �Ƿ���ʾ
	 */
	virtual void DrawTuoGuan(BYTE byChair, bool bShow = true);

	/** 
	 * @brief ����ׯ��
	 * @param[in] byChair ���λ��
	 * @param[in] bShow   �Ƿ���ʾ
	 */
	virtual void DrawDealer(BYTE byChair, bool bShow = true);

	/** 
	 * @brief �������ƿ�
	 * @param[in] byFlag  ���Ʊ�ʶ: ��(0x01), ��(0x02) ��(0x04), ��(0x08), ��(0x10)
	 */
	virtual void DrawBlock(BYTE byFlag);

	/** 
	 * @brief ��������
	 * @param[in] byChair ���λ��
	 * @param[in] mjHand  ��������
	 * @param[in] bIsLaydown �ؼ��Ƿ�����(̯��)
	 * @param[in] bIsGiveTileMode ���ұ�һ�����Ƿ�ֿ�
	 */
	virtual void DrawHand(BYTE byChair, CMJHand mjHand, bool bIsGiveTileMode = false, bool bIsLaydown = false);

	/** 
	 * @brief ���Ƴ���
	 * @param[in] byChair ���λ��
	 * @param[in] mjGive  ��������
	 */
	virtual void DrawGive(BYTE byChair, CMJGive mjGive);

	/** 
	 * @brief ��������
	 * @param[in] byChair ���λ��
	 * @param[in] mjSet   ��������
	 */
	virtual void DrawSet(BYTE byChair, CMJSet mjSet);

	/** 
	 * @brief ������ǽ
	 * @param[in] bShow   �Ƿ���ʾ
	 */
	virtual void DrawWall(bool bShow = true);

	/** 
	 * @brief ����С��
	 * @param[in] nOffsetPos С��ָ���ڼ��յ���
	 * @param[in] bShow      �Ƿ���ʾ
	 */
	virtual void DrawSmallHand(int nOffsetPos, bool bShow = true);

	/** 
	 * @brief ���Ƴ��Ʊ��
	 * @param[in] byChair ���λ��
	 * @param[in] bShow   �Ƿ���ʾ
	*/
	virtual void DrawGiveAnimate(BYTE byChair, bool bShow = true);
	
	/** 
	 * @brief ���ƽ����
	 * @param[in] pCmd    ��������
	 * @param[in] bShow   �Ƿ���ʾ
	 */
	virtual void DrawFinish(TCMD_NOTIFY_FINISH * pCmd, bool bShow = true);
	/////////////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////////////
	/** 
	 * @brief ���ö�ʱ��
	 * @param[in] byChair ���λ��
	 * @param[in] uTime   ʱ������
	 */
	virtual void SetGameTimer(BYTE byChair, UINT uTime);

	/** 
	 * @brief ֹͣ���ж�ʱ��
	 */
	void KillAllTimer();

	/** 
	 * @brief ������Ч��Դ
	 */
	virtual void LoadSound();

	/** 
	 * @brief ��������
	 * @param[in] nSoundID ��������ID
	 * @param[in] byChair  ���λ��
	 */
	void PlaySound(int nSoundID, BYTE byChair = -1);

	/** 
	 * @brief ��������
	 * @param[in] pSoundName ��������
	 */
	void PlaySound(char * pSoundName);

	/** 
	 * @brief ���ű�������
	 * @param[in] bPlay ���ſ���(true: ����,  false: ֹͣ)
	 */
	void PlayBackSound(bool bPlay = true);
	////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////
	/// ͨ��UI���潻���������������غ���
	/** 
	 * @brief ����GM����
	 */
	virtual void OnRequestGMCmd();

	/** 
	 * @brief �������                
	 * @param [in] uControlID �ؼ�ID
	 * @param [in] bAuto      �Ƿ��Զ�����
	 */
	virtual void OnRequestGive(UINT32 uControlID, bool bAuto = false);

	/** 
	 * @brief ��������              
	 * @param [in] uControlID �ؼ�ID
	 */
	virtual void OnRequestBlock(UINT32 uControlID);

	/** 
	 * @brief ��������              
	 * @param [in] uControlID �ؼ�ID
	 */
	virtual void OnRequestBlockSelect(UINT32 uControlID);

	/** 
 	 * @brief ������Ϣ�ƶ���ʾѡ��             
 	 * @param [in] uControlID �ؼ�ID
	 * @param [in] nIndex     ������λ��
	 */
	virtual void OnBlockMoveSelect(UINT32 uControlID, int nIndex = -1);
	///////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////
	/// ������������õ���غ���
	/** 
	 * @brief ��������
	 * @param[in] uControlID �ؼ�ID
	 */
	virtual void OnAnimalFinish(UINT32 uControlID);

	/** 
	 * @brief �������ӽ���
	 * @param[in] uControlID �ؼ�ID
	 */
	virtual void OnAnimalDiceFinish(UINT32 uControlID);

	/** 
	 * @brief ������ʱ������
	 * @param[in] uControlID �ؼ�ID
	 */
	virtual void OnAnimalClockFinish(UINT32 uControlID);

	/** 
	 * @brief ���Ƴ�ʱ
	 * @param[in] uControlID �ؼ�ID
	 */
	virtual void OnTimeOutGive(UINT32 uControlID);

	/** 
	 * @brief ���Ƴ�ʱ
	 * @param[in] uControlID �ؼ�ID
	 */
	virtual void OnTimeOutBlock(UINT32 uControlID);
	/////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////
	/// ��ʱ����������õ���غ���
	/** 
	 * @brief ���Ʋ�������
	 */
	virtual void OnDeal();

	/** 
	 * @brief ���Ӳ�������
	 */
	virtual void OnDice();

	/** 
	 * @brief С���ƶ���������
	 */
	virtual void OnSmallHandMove();

	/** 
	 * @brief ��ʾ�����������
	 */
	virtual void OnShowFinish();
	/////////////////////////////////////////////////////////////////////////////
};

#endif