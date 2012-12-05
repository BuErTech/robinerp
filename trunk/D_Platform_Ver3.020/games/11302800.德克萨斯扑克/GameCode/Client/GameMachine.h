
// ***************************************************************
//  GameEngine   version:  1.0   ��  date: 02/11/2011
//  -------------------------------------------------------------

//	�¿���˹�˿ˣ���״̬������Ϸ�ͻ���״̬��\
	NameID��51300280

//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

#include "StdAfx.h"
#include "UserDataCliObject.h"
#include "../GameMessage/UpGradeLogic.h"
#include "GameLanchSdk\bzwUiInclude\bbaseparameter.h"
#include "SoundsDef.h"
#include "CardSender.h"
#include<stack>
using namespace std;


#define IDT_TIMEOUT_NO_USE			 100    //ֻ���ڰ���Ķ�ʱ��
#define IDT_TIMEOUT_CFG_DESK		 101	//���õ�ע�Ķ�ʱ��ID
#define IDT_TIMEOUT_WAIT_AGREE		 102	//�ȴ�����ͬ���ע��ʱ��ID1
#define IDT_TIMEOUT_VERB			 103    //�ȴ���ע��ʱ��ID
#define IDT_TIMEOUT_NO_PLAYER		 104	//û����ҵȴ���Ϸ���˳�
#define IDT_TIMEOUT_WAIT_CONFIG_NOTE 105	//�ȴ����õ�ע��ʱ
#define IDT_TIMEOUT_NOT_ENOUGH_MONEY 107	//���Ǯ������ʾ��ʱ
#define IDT_TIMEOUT_SHOW_BASE_CARD	 109	//�ȴ���ʾ����
#define IDT_TIMEOUT_NEXT_ROUND_BEGIN 110	//�ȴ���һ�غϿ�ʼ

#define IDT_COLLECT_JETONS			 120	//�ռ����붯����ʱ��
#define IDT_WAIT_DISPATCH_JETONS	 121	//�ȴ��䷢����
#define IDT_DISPATCH_JETONS			 122	//�䷢���붯����ʱ��

#define IDT_TIME_SOUND               123    //��ʱ���Ŷ�ʱ������

#define IDSM_CARD_SENDER			 1		//������״̬��ID (IDSM = ID of Sub Machine)

/************************************************************************************
*									�¿���˹�˿�									*
*************************************************************************************/
#define IDT_TIMEOUT_USER_STAKE		120		// ע�ʱ��״̬


/************************************************************************************
*																					*
*************************************************************************************/

class CMiniGameEngine;

class CGameMachine
{

public:

	//����������
	CGameMachine();

	virtual ~CGameMachine();

public:

	//������Ϸ��ǰ��״̬��������Ϸ��״̬���ӿ�
	virtual bool StartUp(CMiniGameEngine* p,BYTE byGameStation,void* pGSData);

	//�ر�
	virtual bool ShutDown();

	//��Ϸ��Ч���ü���
	virtual bool LoadSoundCfg();

	//����ID��ȡ��״̬��
	virtual void* GetSubMachine(UINT uiMacID);

/*--------------------------------------------------------------------
	����״̬��״̬�任�ӿ�
  ------------------------------------------------------------------*/
public:

	//��ʼ��Ϸ�Ľӿ�
	virtual bool OnHandleGameBegin(void* pData,UINT uiSize);

	//Ϊ����ṩѡ��ע��Ľӿ�
	virtual bool OnHandleCfgDesk(void* pData,UINT uiSize);

	//ϵͳ�ṩѡ��ע������Է�������ע����Ϣ�Ĵ����տ�
	virtual bool OnHandleGetSysDeskCfg(void* pData,UINT uiSize);

	//����ṩѡ��ע������Է�������ע����Ϣ�Ĵ����տ�
	virtual bool OnHandleGetDeskCfg(void* pData,UINT uiSize);

	//����ṩ����������տ�
	virtual bool OnHandleInMoney(void* pData,UINT uiSize);

	//���ѡ���עд�ɵĴ���ӿ�
	virtual bool OnHandleCfgDeskFinish(BYTE byIndex);

	//�ȴ�ׯ��ѡ��ע��ĳ�ʱ����
	virtual bool OnHandleWaitCfgDeskTimeout();

/***************************************************************************************************/
	// ��ʼ��ť
	virtual bool OnHandleBeginGame(BBaseParameter* pParam);

	// �޸Ĵ����ť����ӿ�
	virtual bool OnHandleModifyMoney();
	
	// �����ע����ť��ӿ�
	virtual bool OnHandleBet();

	// ��Ҹ�ע��ť����ӿ�
	virtual bool OnHandleCall();

	// ��Ҽ�ע��ť����ӿ�
	virtual bool OnHandleRaise();
	
	// ��ҹ��ư�ť����ӿ�
	virtual bool OnHandleCheck();

	// ������ư�ť����ӿ�
	virtual bool OnHandleFolo();

	// ���ȫ�°�ť����ӿ�
	virtual bool OnHandleAllin();
	
	// ��ҽ�ť����ӿ�
	virtual bool OnHandleButtonMoney(int nID);

	// ��һ�����ע�ᴦ��ӿ�
	virtual bool OnHandleSlidingAxle();

	// ��Ҽ�עť����ӿ�
	virtual bool OnHandleAddMinus();

	// ��Ҽ�עť����ӿ�
	virtual bool OnHandleSubMinus();

	// ��Ҹ�ע�йܴ���ӿ�
	virtual bool OnHandleCallMandate(bool bFlag);

	// ��ҹ����йܴ���ӿ�
	virtual bool OnHandleCheckMandate(bool bFlag);

	// ��Ҹ��κ�ע�йܴ���ӿ�
	virtual bool OnHandleCallAnyMandate(bool bFlag);

	// ��ҹ��ƻ������йܴ���ӿ�
	virtual bool OnHandlePassAbandonMandate(bool bFlag);

	// ��ʼ������
	virtual bool Init();
/***************************************************************************************************/

	//�������·��ƴ���ӿ�
	virtual bool OnHandleSendACards(void* pData,UINT uiSize);

	//�������·�3�Ź������ƴ���ӿ�
	virtual bool OnHandleSend3Cards(void* pData,UINT uiSize);

	//�������·�1�ŵ��ƴ���ӿ�
	virtual bool OnHandleSend1Cards(void* pData,UINT uiSize);

	//�������·��ƴ���ӿ�
	virtual bool OnHandleBetPoolUp(void* pData,UINT uiSize);

	//��������ע���´���ӿ�
	virtual bool OnHandleBetInfo(void* pData,UINT uiSize);

	//����������������ҷ�����ɵ���Ϣ����ӿ�
	virtual bool OnHandleSendCardFinish();

	//�������·����ƴ���ӿ�
	virtual bool OnHandleGetToken(void* pData,UINT uiSize);

	//�յ������ע����Ϣ�ӿ�
	virtual bool OnHandleGetBetInfo(void* pData,UINT uiSize);

	//����������ƴ���ӿ�
	virtual bool OnHandleHitHandCards(BYTE byCard[],int iUpCount);

	//��Ұ���ȫ���´���ӿ�
	virtual bool OnHandleUnUpCard();

	//�����ʾ����ӿ�
	virtual bool OnHandleCue();

	//���ƴ���ӿ�
	virtual bool OnHandleCompareCard(void* pData,UINT uiSize);

	//���㴦��ӿ�
	virtual bool OnHandleRoundResult(void* pData,UINT uiSize);

	//����䷢����ӿ�
	virtual bool OnHandleDispatchJetons();

	//��һ�غ�׼����������ӿ�
	virtual bool OnHandleNextRoundReq(BBaseParameter* pParam);

	//�»غϿ�ʼ����ӿ�
	virtual bool OnHandleNewRoundBegin(void* pData,UINT uiSize);

	//û�����ͬ���ע����û����ҵ�ʱ����ӿ�
	virtual bool OnHandleNoPlayer(void* pData,UINT uiSize);

	//����Ƿ�ͬ����Ϸ�Ĵ���ӿ�
	virtual bool OnHandleAgree(void* pData,UINT uiSize);

	//������ϵ�Ǯ���㹻ʱ�Ĵ���ӿ�
	virtual bool OnHandleNotEnoughMoney(void* pData,UINT uiSize);

	//ϵͳ�Զ�����Ƭ����ӿ�
	virtual bool OnUserSortOut();
	
	//���������Ч����ӿ�
	virtual bool OnHandleCfgSound(BBaseParameter* pParam);

	/*--���ڶ�ʱ�Ĵ���---------------------------------------------*/

    /*--���ڳ�ʱ�Ĵ���---------------------------------------------*/

	//ѡ��ע��ĳ�ʱ�ӿ�
	virtual bool OnTimeoutCfgDesk(BYTE bDeskStation);

	//ѡ��ע��ĳ�ʱ�ӿ�
	virtual bool OnTimeoutWaitCfgNote();

	//�ȴ���עͬ��ĳ�ʱ�ӿ�
	virtual bool OnTimeoutAgreeNote(BYTE bDeskStation);

	//�ȴ���һ�غϿ�ʼ��ʱ���ù⴦��ӿ�
	virtual bool OnTimeoutNextRoundBegin(BYTE bDeskStation);

	//���û��Ǯ��ʱ����ӿ�
	virtual bool OnTimeoutNotEnoughMoney(BYTE bDeskStation);

	/*����״̬���ֵĴ���------------------------------------------*/

	//��Ϸ������״̬����
	virtual bool OnRecurBase(BYTE byGameStation,TGSBase* pGSBase);

	//��Ϸ�ȴ�ͬ��״̬����
	virtual bool OnRecurWaitAgree(void* pData);

	//��Ϸ�����е�״̬����
	virtual bool OnRecurPlayingGame(void* pData);

	//�ȴ���һ����Ϸ��״̬����
	virtual bool OnRecurWaitNextRound(void* pData);


	//��ʱ��˳��õ��¼�λ��
	BYTE GetNextUserStation(BYTE byStation, BOOL bIsAll = TRUE);

	//��ʱ��˳��õ��ϼ�λ��
	BYTE GetLastUserStation(BYTE byStation, BOOL bIsAll = TRUE);

	//��ռ�¼��һ���������ջ�б�
	void ClearAllStack();
/*--------------------------------------------------------------------
	״̬����������
  ------------------------------------------------------------------*/
public:

	//��Ϸ�������ָ�룬�䵣��������ɫ����һ��UI�����ڶ���ͨѶ����\
	  ���������������ʱ��ƺ���Ϊ�����ࡱ����ʱ��ƺ���Ϊ��ͨѶ�ࡱ
	CMiniGameEngine* m_pGameEngine;

	//�Լ����߼�λ��
	BYTE m_byMeStation;

	//��Ϸ�߼�
	CUpGradeGameLogic m_Logic;

	//�ط��������
	DZPKSound::LanguageType m_ltLangType;

	//��Ϸ����������Ч����
	bool m_bBgMusic;

	//��Ϸ��Ч����
	bool m_bGameSound;

	//�Ƿ����й�״̬
	bool m_bTuoGuan;

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

/*--------------------------------------------------------------------
	��״̬��
  ------------------------------------------------------------------*/
private:

	//���ƻ�ָ�룬���ƻ���Ҫ��̬���������ƺ����ͷţ�������ֵ
	CCardSender* m_pCardSender;

/*--------------------------------------------------------------------
	��Ϸ����
  ------------------------------------------------------------------*/
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
	int m_nCurrentMoney; 

	// ��ע���
	int m_nCallMoney;

	//��¼��ע���
	int m_iBetCounts;

	//��Ϸ�е����ж���ʱ��
	int m_iVerbTime;

	//ÿ���û����ý��
	int m_nMoney[PLAY_COUNT];

	//ÿ�������ע���
	int m_nBetMoney[PLAY_COUNT];

	// �������
	bool m_bGiveUp[PLAY_COUNT];

	// ȫ�����
	bool m_bAllBet[PLAY_COUNT];
	
	// ��ע�߳�
	TBetPool m_tagBetPool;
};