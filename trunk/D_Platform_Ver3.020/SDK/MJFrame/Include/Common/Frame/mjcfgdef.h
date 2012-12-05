/** @file	mjcfgdef.h
 *	@brief	�齫��ܵ�������Ϣ
 *	@author edward
 *	@date 08.08.04
 */
#ifndef _MJCFGDEF_H
#define _MJCFGDEF_H

#include "..\typeDef.h"

namespace MyGame
{

#define MAX_FAN_NUMBER		128
#define MAX_FAN_NAME		18
#define MAX_FAN_CHECK_NBR	4
#define MAX_FAN_NOCHECK_NBR	12	

/**
 * @ingroup MJFrame
 * @brief һ���������ݽṹ
 */	
typedef struct tagFAN_NODE
{
	char		byCount;					     // ����
	BOOL		bFan;						     // ÿ�ַ��Ƿ�����
	BOOL		bCheck;						     // �Ƿ������
	char		nCheckNbr;					     // �жϹ��������
	char		nNoCheckNbr;				     // ȡ���жϵķ�������
	char        nCheckFunID[MAX_FAN_CHECK_NBR];	 // �жϵĹ���
	char		nNoCheckFan[MAX_FAN_NOCHECK_NBR];// ����������ʱ��ȡ���жϵ�������ͻ��
	int		    byFanPoint;					     // ÿ�ַ��Ƕ��ٷ�
	char		szFanName[MAX_FAN_NAME];	     // ÿ�ַ�������
}FAN_NODE; 

/**
 * @ingroup MJFrame
 * @brief ���з������ݽṹ
 */	
typedef	struct tagFAN_COUNT
{
	FAN_NODE	m_FanNode[MAX_FAN_NUMBER];
}FAN_COUNT;

/**
 * @ingroup MJFrame
 * @brief �齫�õ���������Ϣ
 * 10.01.21 ����˳�򱻴��ң������ֽڶ���
 */	
typedef struct tagMJSettingData
{
	// ֧��һ�ڶ��� 
	char nMoreWin;	

	// ���Ƶĵȼ� 0Ϊ������
	// nBlockLevelTing���ж�ʱ����Ȼֻ�����Լ����Ƶ�ʱ��Ż�ʹ��
	char nBlockLevelCollect;
	char nBlockLevelTriplet;
	char nBlockLevelQuadruplet;
	char nBlockLevelTing;
	char nBlockLevelWin;

	// ����
	char nQiangGang;
	// ©��
	char nLouHu;
    // �޷���
    char nWuFanHu;
	// ���ƵĶ�����ʾ���Ӻ��Ǵ�ǰ������Ӱ���߼�
	char nDrawGangBack;
	// ����Ҳ�㵽˳������ȥ
	char nCollateWind;
	// ��ͷ��Ҳ�㵽˳������ȥ
	char nCollateArrow;
	// �����Գ���
	char nAllCanCollate;
	// �Ƿ�������ʾ
	char nShowTing;
	// �Ƿ������ƺ���Ȼ��ȫ��
	char nTingCanGang;
    // �Ƿ񰵸�����(0: ֻ�а�������Ҳ��ܿ���,  1: ������Ҷ��ܿ�����������)
    char nAnGangShow;
    // �Ƿ��˶���������
    char nDongNanXiBeiGang;
    // �Ƿ����з��׸�
    char nZhongFaBaiGang;
	// �Ƿ�������Ϊ��ͨ�����Ժ�
	char nCanWinLaizi;
	// ����Ƿ���Ա�������
	char nCanBlockLaizi;
	// �Ƿ���˲���ͨ��
	char nOpenGM;
	// ��ǽ���
	char nTilesChar;
	char nTilesBamboo;
	char nTilesBall;
	char nTilesWind;
	char nTilesArrow;
	char nTilesFlower;
	
	// ���������ж�
	// ��������齫�����µľͲ�����д������
    char nCheck7dui;		// 7��
    char nCheckQuanbukao;	// ȫ����
    char nCheck131;		    // 131
    char nCheckZuhelong;	// �����
    char nCheckLualv;		// ȫ��
    char nCHeckLuafeng; 	// ȫ��

	char nTimerGive;
	char nTimerBlock;
	char nTimerReady;
	char nTimerServer;
	char nTimerFlower;

    char nTimerCutCard;     // ���Ʋ���ʱ��
    char nTimerDice;        // ���Ӳ���ʱ��
    char nTimerDeal;        // ���Ʋ���ʱ��
    char nTimerJP;          // ���Ʋ���ʱ��

    char nOpenLookTile;     // �Ƿ���(��Ϸ������, �Ƿ���ʾδ���Ƶ���ҵ���)
    char nTimerLookTile;    // �����ƺ���೤ʱ�����ϴ��

    char nGiveBlockShow;    // �Ƿ���ʾ���������б������ܵ���
	char nGiveOrder;        // ����˳��(0: ����˳ʱ��, 1: ������ʱ��, ��ȡ�������л�ת��Ϊ1��3, ������Ϸ������һ�����λ�ü���)

	// ��Ʊ�����㣬���ܵĿ۷ѷ�ʽ
	char nMoneyType;
	// ���ܿ۷ֳַ�
	char nEscapeShareScore;
	// 1��ʹ�õ��ߵ��������ޣ���������
	char nItemLimitTimes;

	// �Ǻ��Ƶ���������
	char nTilesHandNbr;	
	// �ܵ�������
	int nTilesNumber;
	int nMaxFan;
	int nBaseScore;
	int nMinFan;
    
	// ��Ʊ
	int nTicket;
	// ���ƻ���(Ϊ0����ʾ����)
	int nGangScore;
	// ��תǮ
	int nScoreToMoney;
	// �����Ǯ
	int nMaxMoney;

	// �������ߵļ۸������Ϊ0��ʹ��ʱ���п۷���ʾ
	int nVoicePrice;
	// 1��ʹ�õ��ߵļ۸����ޣ���������
	int nItemLimitMoney;

	// ������ߵļ۸�����
	int nBuyItemMoney;
	FAN_COUNT tagFanCount;
}MJ_SETTING_DATA;

// ������������������洢����
#define PRESENTGIFT_URL_MAX		6
// ���������������·��		
#define PRESENTGIFT_URL_PATH	128		
// ��������������ߵȼ���
#define PRESENTGIFT_RANK_MAX	32
// ���������������͵�������
#define PRESENTGIFT_LEVEL_GIFT	6
// ��������ÿ�����͵�������
#define PRESENTGIFT_TODAY_GIFT  6
// ����ȼ�����������Ϊ7
#define PERSENTGIFT_LEVEL_MAX	7

// ���������������
typedef struct tagMJPresentGiftSetting
{
	// ����ÿ�����ͼ��ʱ�䣬���Ϊ0�򲻻�����
	int nPresentColdDown;

	// ����ð����ʾ���ʱ��
	int  nTipsColdDown;

    // ����������ʾ��ť�ĳ��ּ��ʣ����100
	char  nShowPresentGiftBtn;

	// ����ÿ�����ͼ�ֵ
	int nTodayGiftWorth[PERSENTGIFT_LEVEL_MAX];
	// ����ÿ�����͵���ID����Ӧ����ID
	char nTodayGiftID[PERSENTGIFT_LEVEL_MAX][PRESENTGIFT_TODAY_GIFT];
	// ����ÿ�����͵���������ʱ�����ȫ���趨Ϊ�͵����죬������
	char nTodayGiftValue[PERSENTGIFT_LEVEL_MAX][PRESENTGIFT_TODAY_GIFT];

	// �����������ͼ�ֵ
	int nLVGiftWorth[PERSENTGIFT_LEVEL_MAX];
	// �����������͵���ID����Ӧ����ID
	char nLVGiftID[PERSENTGIFT_LEVEL_MAX][PRESENTGIFT_LEVEL_GIFT];
	// �����������͵���������ʱ�����ȫ���趨Ϊ�͵����죬������
	char nLVGiftValue[PERSENTGIFT_LEVEL_MAX][PRESENTGIFT_LEVEL_GIFT];

	// �ȼ�����
	char ucRankCount;
	// �ȼ���Ҫ�Ļ���
	int  nRankScore[PRESENTGIFT_RANK_MAX];

	// ˢ�µ�ʱ��㣬�Զ�8��Ϊ��׼����ȷ��ΪСʱ��24Сʱ��
	char ucUpdateTime;

    // ����������
	char ucUrlCount; 
	// �����Ӱ汾������汾��ͬ���ǲ�����¿ͻ��˵ĳ��������õ�
	char ucUrlVersion;
	// �����ӵ�ַ
	char szUrlPath[PRESENTGIFT_URL_MAX][PRESENTGIFT_URL_PATH];

}MJ_PRESENT_GIFT_SETTING;

}

#endif
