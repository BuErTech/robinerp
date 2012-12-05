
// ***************************************************************
//  UserData ServerObject   version:  1.0   ��  date: 03/29/2011
//  -------------------------------------------------------------

//	�¿���˹�˿˷���ˣ������Ϸ����\
	NameID��51300280

//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

#include "../GameMessage/UpgradeMessage.h"

#define UD_VF_CALL					0x01  //���Ը�ע
#define UD_VF_ADD					0x02  //���Լ�ע
#define UD_VF_BET					0x04  //������ע
#define UD_VF_FOLD					0x08  //��������
#define UD_VF_CHECK					0x10  //���Թ���
#define UD_VF_ALLIN					0x20  //����ȫ��
#define UD_VF_CHECKMANDATE			0x40  //���Թ����й�
#define UD_VF_CALLMANDATE			0x80  //���Ը�ע�й�
#define UD_VF_CALLANYMANDATE		0x100 //���Ը��κ�ע�й�
#define UD_VF_PASSABANDONMANDATE	0x200 //���Թ���/�����й�

class  CUserDataSvrObject
{

public:

	//��ʼ������
	CUserDataSvrObject(void);

	//�ͷ�����
	virtual~CUserDataSvrObject(void);

	//��ʼ��
	void Init();

	//���ʼ��
	void HalfInit();

public:

	//���������ϵ�������
	void GetHandCards(BYTE* pCards);

	//�����Ҹճ�����
	void GetOutCards(BYTE* pCards);

	////������ҵĶ���
	//void MakeVerbs(BYTE byFlag);

public:

	//���״̬����Ҫ�����ж���Щ���ͬ��/��ͬ����Ϸ 
	enum emState
	{
		UNKNOWN = 0,		 //δ֪״̬
		AGREE_DESKCFG = 1,	 //���ͬ����Ϸ
		DISAGREE_DESKCFG = 2,//��Ҳ�ͬ����Ϸ
	};
	emState m_nState;

	//�û����ϵ��˿�
	BYTE m_byMyCards[MAX_BACKCARDS_NUM];

	//�û������˿���Ŀ
	int m_iCardsNum;

	//��ǰ�û�������˿�
	BYTE m_byOutCards[MAX_HANDCARDS_NUM];

	//��ǰ�û�������˿�����
	int m_iOutCardsNum;

public:

	//����Ƿ��Ѷ���
	bool m_bNetCut;

	//����Ƿ�����
	bool m_bSendCardFinish;

	// ��ȡ��ǰ��������Ƿ�2��
	int m_iNnt[PLAY_COUNT];

	////�û���ǰ�����Ķ������
	//BYTE m_byVerbFlag;

	// ��¼�Ƿ���ȫ��ȷ�ϴ���
	bool m_bIn;
};