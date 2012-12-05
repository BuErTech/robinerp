
// ***************************************************************
//  GameDataObject   version:  1.0   ��  date: 03/29/2011
//  -------------------------------------------------------------

//	�¿���˹�˿ˣ��������Ϸ����\
	NameID���¿���˹�˿�

//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

#include "UserDataSvrObject.h"
#include "../GameMessage/UpgradeMessage.h"
#include "../GameMessage/UpGradeLogic.h"

class  CGameDataObject
{

public:

	//��ʼ������
	CGameDataObject(void);

	//�ͷ�����
	virtual~CGameDataObject(void);

	//��ʼ��
	void Init();

	//���ʼ��
	void HalfInit();

/*-------------------------------------------------------------------------------------
    �������
  -----------------------------------------------------------------------------------*/
public:

	//�������Ϊδ֪�Ƿ�ͬ���ע״̬
	void SetUnknowStateTo(BYTE bDeskStation);

	//�������Ϊͬ���ע״̬
	void SetAgreeStateTo(BYTE bDeskStation);

	//�������Ϊ��ͬ���ע״̬
	void SetDisAgreeStateTo(BYTE bDeskStation);
	
	//Ϊĳ����ҷ���
	bool DealCardsTo(BYTE bDeskStation,int iSendNum);

	//��ձ��ֵ�������ҳ�������
	void ClearAllUserOutCard();

/*-----------------------------------------------------------------
    ��ȡ��Ϣ���
  ---------------------------------------------------------------*/
public:

	//�õ��ж��ٸ����ͬ����Ϸ��ͬ���ע��
	int GetAgreeNums();

	//�õ��ж��ٸ�����ѷ�����
	bool IsAllPlayerSendCardFinish();

	//��ȡ���ҵ�����
	int GetActivePlayerNums();

	//��ȡ��ע���
	__int64 GetBetMoney(BYTE bDeskStation = 255, bool bAll = true);

    //������ҵĶ���
    void MakeVerbs(BYTE byFlag);

	//��һ�ú���
	void GetGoodCard(BYTE bDeskStation);

/*-----------------------------------------------------------------
    �������Ϸ�����е�����
  ---------------------------------------------------------------*/
public:

	//��Դ���飬���֧���ĸ���
	BYTE m_byCards[MAX_CARDS_NUM];

	//��ŵ��Ƶ�����
	BYTE m_byBackCards[PLAY_COUNT][MAX_BACKCARDS_NUM];

	//���Ƶ�����
	int m_iBackCardNum;

	//����λ��
	int m_iGetCardPos;

	//�û�����
	CUserDataSvrObject m_UserData[PLAY_COUNT];

	//�õ����Ƶ��û������ɻ���û�
	BYTE m_byTokenUser;

	//ׯ��
	BYTE m_byNTUser;

	//��ǰ��Ϸ���Ȧ��
	int m_nTurnNums;


/*-----------------------------------------------------------------
    �¿���˹�˿���Ϸ���е�����
  ---------------------------------------------------------------*/
	
	//��äע
	BYTE m_bBigBlind;

	// Сäע
	BYTE m_bSmallBlind;
	
	// ��ע���
	__int64 m_nBetMoney[PLAY_COUNT][8];

	// ��ǰ��ע�������
	__int64 m_nMaxBetMoney;

	// ��ǰ��ע��
	__int64 m_nBetPool;

    // ��¼���һ��ȫ�µ���
    BYTE m_bAllBetStation;

	// ��¼������ע
	bool m_bNoBet[PLAY_COUNT];

	// ��¼������ĸ�����ȫ�µ� 2011-10-28 duanxiaohui
	bool m_bAllBet[PLAY_COUNT][8];

	// ��¼����
	bool m_bGiveUp[PLAY_COUNT];

	// ÿ����ҿ�����ע���
	__int64 m_UserMoney[PLAY_COUNT];

	// ���ƴ���
	int m_iCheckCount;

	// ��һ����ע���
	int m_iBetUser;

	// �����Ϲ�����
	BYTE m_iCards[5];

	// ���湫������
	int m_iCardsCount;

	// �Ƿ�����Ҽ�ע
	bool m_bRaise;
	
	// ��¼��ע����
	int m_iRaiseCounts;

	// ��¼ȫ���������
	int m_iAllinUser;

	// ��¼ÿ���߳��Ƿ���ȫ�´���
	BYTE m_byAllPools[8];
	
	//��¼��ע���
	int m_iCallCounts;

	// ��¼�������
	int m_iFoldCount;

	// һ���е�һ���������
	BYTE m_bFirstStation;

	// ��¼��һ����ע���
	BYTE m_bBetStation;

    // ��¼���һ����ע���
    BYTE m_bRaiseStation;

    // ��¼��ע�������
    int m_iBetCounts;

    // �û���ǰ�����Ķ������
    BYTE m_byVerbFlag;

    // ÿ�����������������
    BYTE m_bResultCards[PLAY_COUNT][5];

	// ����������ƽ���
	bool m_bGiveUpOver;

	// �Զ�����
	bool m_bAutoSendCard;

    // �µ�һ��
    bool m_bNewTurn;

	// �µ�һ��
    bool m_bNewTurns;

	// ��һ��������
	BYTE m_iDealPeople;	

public:

	//��������
	TDeskCfg m_tagDeskCfg;

	//����ĳɼ���
	TResult m_tagResult;

private:
	CUpGradeGameLogic m_GamgeLogic;	//��Ϸ�߼���

};


