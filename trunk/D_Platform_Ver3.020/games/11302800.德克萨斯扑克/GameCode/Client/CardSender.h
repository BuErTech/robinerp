
// ***************************************************************
//  GameEngine   version:  1.0   ��  date: 02/11/2011
//  -------------------------------------------------------------

//	��������״̬������Ϸ�ͻ��˷��ƻ�\
	NameID��50100440

//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

#include "StdAfx.h"

#include <afxmt.h>

class CMiniGameEngine;
class CGameMachine;

#define IDT_SEND_CARD_GAP			 220	//����ʱ������ʾ��ʱ����
#define IDT_SEND_MOVING_GAP			 221	//�����˶�ʱ��ļ��

class CCardSender
{

public:

	//����������
	CCardSender();

	virtual ~CCardSender();

public:

	//������Ϸ��ǰ��״̬��������Ϸ��״̬���ӿ�\
	  Ϊʲô��ҪCGameMachineָ�룿\
	  ����Ϊ�ڷ��ƻ���ת������Ҫ��������ݣ�һЩ��Ϸ�������ã�����Ҫ�õ���״̬��ָ�룻\
	  ��Ҫ����UI����Ч���õ����棬��CGameMachineָ���ȡ��Ϸ���棻\
	  ͬʱҲ������״̬����һ��������
	virtual bool StartUp(CGameMachine* pRootMachine,BYTE byGameStation,void* pGSData);

	//�ر�״̬��
	virtual bool ShutDown();

public:

	//��ÿ���Ƶ�ÿ��ʱ��������
	bool OnTimerGap();

	//��ÿ���Ƶ��˶�ʱ��������
	bool OnTimerMovingGap();

public:

	//��ȡ����ʾ����
	void GetShowedCards(BYTE byCard[]);

public:

	//���ƹ�������
	bool OnRecurSending(void* pGSBase);

protected:

	//������ҷ��ƶ�������
	bool LoadSendCardQueue();

private:

	//��Ϸ�������ָ�룬�䵣��������ɫ����һ��UI�����ڶ���ͨѶ����\
	  ���������������ʱ��ƺ���Ϊ�����ࡱ����ʱ��ƺ���Ϊ��ͨѶ�ࡱ
	CMiniGameEngine* m_pGameEngine;

	//��Ϸ��״̬��
	CGameMachine* m_pRootMachine;

/*--------------------------------------------------------------------
	����
  ------------------------------------------------------------------*/
public:

	//����ʾ����
	BYTE m_byShowed[MAX_HANDCARDS_NUM];

	//������Ҷ���
	BYTE m_SendCardQueue[PLAY_COUNT + 1];

	//���ƽ��ȣ����ڷ���һ���ŷ�����Ҫ��¼��ǰ���ѷ����ڼ�����
	int m_iSendingPos;

	//��¼��ǰ�Ƿ����ϵĵڼ�����
	int m_iSendingIndex;

};