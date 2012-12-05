// ***************************************************************
//  GameAlgorithm   version:  1.0   ��  date: 03/29/2011
//  -------------------------------------------------------------

//		�¿���˹�˿ˣ���Ϸ��Ҫ�㷨\
	    NameID��51300280

//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

//#include "StdAfx.h"

//��������
#define	CARD_MASK_HUA				0xF0
#define	CARD_MASK_VAL				0x0F

//�˿˻�ɫ
#define CARD_HUA_FANG				0x00			//����	0000 0000
#define CARD_HUA_MEI				0x10			//÷��	0001 0000
#define CARD_HUA_TAO				0x20			//����	0010 0000
#define CARD_HUA_KUI				0x30			//�ڿ�	0011 0000
#define CARD_HUA_JOKE				0x40			//����	0100 0000
#define CARD_HUA_ERR				0xF0			//����  1111 0000

#define CARD_VAL(card)	(((card) & CARD_MASK_VAL) + 1)	//ȡ��һ���Ƶĵ���
#define CARD_HUA(card)	((card) & CARD_MASK_HUA)		//ȡ��һ���ƵĻ�ɫ

class CGameAlgorithm
{

public:

	//����/����
	CGameAlgorithm(void);

	virtual ~CGameAlgorithm(void);

/*--------------------------------------------------------------------
	�����������ʺϴ����������Ϸʹ��
  ------------------------------------------------------------------*/
public:

	//��ȡ�˿�����
	static BYTE GetCardNum(BYTE byCard);

	//��ȡ�˿˻�ɫ
	static BYTE GetCardHuaKind(BYTE byCard);

	//�����ֵ
	static USHORT GetCardBulk(BYTE byCard,LPCVOID pData = NULL);

	//���ƱȽ�
	static bool CompareCard(BYTE byCard1,BYTE byCard2,LPCVOID pData = NULL);

	//�����˿�
	static BYTE RandCard(BYTE byCard[],int nNum,int nDeskIndex);

	//�����ź���������ټ���һ���ƣ���Ӻ󱣳�����
	static void InsertSorted(BYTE byCard[],int iCount,BYTE byNewCard,LPCVOID pData = NULL);

	//�����˿�
	static void SortCard(BYTE byCard[],BYTE bUp[],int iCardCount,LPCVOID pData = NULL);

	//���0λ�˿�
	static int RemoveNummCard(BYTE iCardList[], int iCardCount);

	//ɾ���˿�
	static int RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount);

	//ö������
	static void EnumCardShapes(BYTE byCard[],int iCount,LPCVOID pData = NULL);

	//������Ҫ����������ֵ����õ��������Ƶ����ݣ�����SetCard�Ĳ�����
	static void GetUpCardList(BYTE byUp[],BYTE byCard[],int iCardNum,BYTE byUpSrc[],int iUpSrcNum);


/*--------------------------------------------------------------------
	������Ϸ���к���
  ------------------------------------------------------------------*/
public:


};

#define GAlg	CGameAlgorithm