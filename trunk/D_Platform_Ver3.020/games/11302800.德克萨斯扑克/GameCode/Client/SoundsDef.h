// ***************************************************************
//  SoundsDef   version:  1.0   ��  date: 03/29/2011
//  -------------------------------------------------------------

//	�¿���˹�˿� ����ˣ������Ϸ���������\
	NameID��51300280

//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#pragma once

#ifndef DZPKSOUND_DEF_HEAD_FILE_51300280

namespace DZPKSound
{
	//��Ϸ��ЧID����
	enum BaseType
	{
		BT_GAME_MUSIC1		= 0,		// ��������
		BT_SEND_BACK_CARD	= 1,		// ����������
		BT_FLOP_ROUND		= 2,		// ���ƹ�������
		BT_TURN_ROUND		= 3,		// ת�ƹ�������
		BT_RIVER_ROUND		= 4,		// ���ƹ�������
		BT_FLOP_PUBLIC_CARD	= 5,		// ������������
		BT_FLOP_BACK_CARD	= 6,		// ����������
		BT_WIN				= 7,		// ʤ������
		BT_LOSE				= 8,		// ʧ������
		BT_FOLD_MUSIC		= 9,		// ��������
		BT_CHIP_MUSIC		= 10,		// ��������
		BT_TRANSFER_TOKEN	= 11,		// �ƽ���������
		BT_OK				= 12,		// ȷ����ť����
		BT_MONEY_BUTTON		= 13,		// ���ѡ������
		BT_ALARM_CLOCK		= 14,		// ��������
		BT_CHIP_MONEY_MUSIC	= 15,		// ������Ч
		BT_NUMS				= 16,		// ����
	};

	//��Ϸ��Ч����
	const static TCHAR* g_szBaseFiles[BT_NUMS] = 
	{
		_TEXT("��������1.ogg"),	// ��������
		_TEXT("����.ogg"),		// ����������
		_TEXT("flop.ogg"),		// ���ƹ�������
		_TEXT("turn.ogg"),		// ת�ƹ�������
		_TEXT("river.ogg"),		// ���ƹ�������
		_TEXT("�м䷭��.ogg"),	// ������������
		_TEXT("����.ogg"),		// ����������
		_TEXT("ʤ��.ogg"),		// ʤ������
		_TEXT("ʧ��.ogg"),		// ʧ������
		_TEXT("����.ogg"),		// ��������
		_TEXT("����.ogg"),		// ��������
		_TEXT("��ʾ��˭.ogg"),	// �ƽ���������
		_TEXT("ȷ����ť.ogg"),	// ȷ����ť����
		_TEXT("���ѡ��.ogg"),	// ���ѡ������
		_TEXT("����.ogg"),		// ��������
		_TEXT("������Ч.ogg"),		// ������Ч
	};

	//�ط�����ID����
	enum LanguageType
	{
		LT_COMMON	= 0,	//��׼����
		LT_DIFANG01	= 1,	//�ط�����
		LT_NUMS		= 2,    //����
	};

	//���԰�·�������
	const static TCHAR* g_szLangPath[LT_NUMS] = 
	{
		_TEXT("common"),		//��׼����
		_TEXT("difang_01"),		//�ط�����
	};

	//��Ϸ��ЧID����
	enum SpeakType
	{
		ST_CALL				= 0,		// ��ע����
		ST_FOLD				= 1,		// ��������
		ST_CHECK			= 2,		// ��������
		ST_RAISE			= 3,		// ��ע����
		ST_ALL_IN			= 4,		// ȫ������
		ST_NUMS				= 5,		// ����
	};

	//��Ϸ��������
	const static TCHAR* g_szSpeakFiles[ST_NUMS][4] = 
	{
		_TEXT("Call_1.ogg"), _TEXT("Call_2.ogg"), _TEXT("Call_3.ogg"), _TEXT("Call_4.ogg"),     	// ��ע����
		_TEXT("Fold_1.ogg"), _TEXT("Fold_2.ogg"),_TEXT("Fold_3.ogg"), _TEXT("Fold_4.ogg"),          // ��������
		_TEXT("Check_1.ogg"),_TEXT("Check_2.ogg"),_TEXT("Check_3.ogg"),_TEXT("Check_4.ogg"),	    // ��������
		_TEXT("Raise_1.ogg"),_TEXT("Raise_2.ogg"),_TEXT("Raise_3.ogg"),_TEXT("Raise_4.ogg"),	    // ��ע����
		_TEXT("All_in_1.ogg"),_TEXT("All_in_2.ogg"),_TEXT("All_in_3.ogg"),_TEXT("All_in_4.ogg"),	// ȫ������
	};
}

#define DZPKSOUND_DEF_HEAD_FILE_51300280
#endif