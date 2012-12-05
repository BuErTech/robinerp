
// ***************************************************************
//  UserData ServerObject   version:  1.0   ��  date: 03/29/2011
//  -------------------------------------------------------------

//	�¿���˹�˿� ����ˣ������Ϸ����\
	NameID��51300280

//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

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

#define UD_VF_BEGIN_GAME			0x11  //��ʼ��Ϸ
#define UD_VF_ALTER_STAKE			0x12  //�޸Ĵ����

class  CUserDataCliObject
{

public:

	///��ʼ������
	CUserDataCliObject(void);

	///�ͷ�����
	virtual~CUserDataCliObject(void);

	/// ��ʼ��
	void Init();

public:

	///���������ϵ�������
	void GetCards(BYTE* pCards);

public:

	//�û����ϵ��˿�
	BYTE m_byMyCards[MAX_HANDCARDS_NUM];

	//�û������˿���Ŀ
	int m_iCardsNum;

	//�û���ǰ�����Ķ������
	//BYTE m_byVerbFlag;
};