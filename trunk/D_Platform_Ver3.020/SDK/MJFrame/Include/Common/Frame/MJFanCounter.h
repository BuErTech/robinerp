/** @file	MJFanCounter.h
 *	@brief	���Ĺ�����
 *	@author duanxiaohui
 *	@date 2010.09.28
 *
 *  �㷬�õĺ����������˴�������з��ͣ�������ͨ��������϶��γ��µķ���
 */
#ifndef ___MJFANCOUNTER_H___
#define ___MJFANCOUNTER_H___

#include "mjcfgdef.h"
#include "MJFrameBaseData.h"

namespace MyGame
{
/* ��Ϸ�ܹ�����128�ַ���
*  ���ʵ�ֵķ���ռ��IDλ�� 0-103��, 105-127��IDλ��������Ϸ��������Ա������ɫ����ʹ��
*/

// ������IDӳ��
#define FAN_ID_QINGYISE       0     // ��һɫ
#define FAN_ID_HUNYISE		  1	    // ��һɫ
#define FAN_ID_QIDUI		  2   	// 7��
#define FAN_ID_DUAN19		  3	    // ��19
#define FAN_ID_BUQIUREN		  4	    // ������
#define FAN_ID_QUAN19		  5	    // ��19
#define FAN_ID_QUANQIUREN	  6	    // ȫ����
#define FAN_ID_PINGHU		  7	    // ƽ��
#define FAN_ID_ZIMO			  8	    // ����
#define FAN_ID_KANZHANG		  9	    // ����
#define FAN_ID_BIANZHANG	  10	// ����
#define FAN_ID_WUZI			  11	// ����
#define FAN_ID_QUEYIMEN		  12	// ȱһ��
#define FAN_ID_HUAPAI		  13	// ���㻨��
#define FAN_ID_DANDIAO		  14	// ����
#define FAN_ID_MINGGANG		  15	// ����
#define FAN_ID_ANGANG		  16	// ����
#define FAN_ID_LIANLIU		  17	// ����
#define FAN_ID_LAOSHAOFU	  18	// ���ٸ�
#define FAN_ID_1SE2TONGSHUN	  19	// 1ɫ2ͬ˳(һ���)
#define FAN_ID_1SE3TONGSHUN	  20	// 1ɫ3ͬ˳
#define FAN_ID_1SE4TONGSHUN	  21	// 1ɫ4ͬ˳
#define FAN_ID_2SE2TONGSHUN	  22	// 2ɫ2ͬ˳
#define FAN_ID_3SE3TONGSHUN	  23	// 3ɫ3ͬ˳
#define FAN_ID_19KE			  24	// 19��(������)
#define FAN_ID_QUANSHUN		  25	// ȫ˳
#define FAN_ID_2ANKE		  26	// ˫����
#define FAN_ID_3ANKE		  27	// ������
#define FAN_ID_3FENGKE		  28	// 3���
#define FAN_ID_2TONGKE		  29	// 2ͬ��
#define FAN_ID_3TONGKE		  30	// 3ͬ��
#define FAN_ID_ALLKE		  31	// ������
#define FAN_ID_4GUI1		  32	// 4��1
#define FAN_ID_MENFENGKE	  33	// �ŷ��
#define FAN_ID_QUANFENGKE	  34	// Ȧ���
#define FAN_ID_JIANKE		  35	// ����
#define FAN_ID_JUEZHANG		  36	// ����
#define FAN_ID_2MINGGANG	  37	// ˫����
#define FAN_ID_3GANG		  38	// 3��
#define FAN_ID_4GANG		  39	// 4��
#define FAN_ID_5MENQI		  40	// 5����
#define FAN_ID_1SE3BU		  41	// 1ɫ3����
#define FAN_ID_1SE4BU		  42	// 1ɫ4����
#define FAN_ID_3SE3BU		  43	// 3ɫ3����
#define FAN_ID_2ANGANG		  44	// ˫����
#define FAN_ID_QIANGGANG	  45	// ����
#define FAN_ID_GANGSHANGHUA	  46	// ���ϻ�
#define FAN_ID_GANGSHANGPAO	  47	// ������
#define FAN_ID_HAIDIPAO		  48	// �����ں�
#define FAN_ID_HAIDIZIMO	  49	// ��������
#define FAN_ID_TUIDAOHU		  50	// �Ƶ���
#define FAN_ID_TUIBUDAO		  51	// �Ʋ���
#define FAN_ID_3SELONG		  52	// ����
#define FAN_ID_1SELONG		  53	// ����
#define FAN_ID_QUANXIAO		  54	// С
#define FAN_ID_QUANZHONG	  55	// ��
#define FAN_ID_QUANDA		  56	// ��
#define FAN_ID_2SE1TONGSHUN	  57	// 2ɫ1ͬ˳
#define FAN_ID_QUANSHUANG	  58	// ȫ˫
#define FAN_ID_QUANDAN		  59	// ȫ��
#define FAN_ID_QUANBUKAO	  60	// ȫ����
#define FAN_ID_1SE3JIEGAO	  61	// һɫ3�ڸ�
#define FAN_ID_1SE4JIEGAO	  62	// һɫ4�ڸ�
#define FAN_ID_3SE3JIEGAO	  63	// 3ɫ3�ڸ�
#define FAN_ID_QUANZI		  64	// ��һɫ
#define FAN_ID_XIAOSHANYUAN	  65	// С��Ԫ
#define FAN_ID_DASHANYUAN	  66	// ����Ԫ
#define FAN_ID_XIAOSIXI		  67	// С��ϲ
#define FAN_ID_DASIXI		  68	// ����ϲ
#define FAN_ID_131			  69	// 131
#define FAN_ID_9LIAN		  70	// 9������
#define FAN_ID_LVYISE		  71	// ��һɫ
#define FAN_ID_XIAOYU5		  72	// С��5
#define FAN_ID_DAYU5		  73	// ����5
#define FAN_ID_ZUHELONG		  74	// �����
#define FAN_ID_7XING		  75	// 7�ǲ���
#define FAN_ID_QUANDAI5		  76	// ȫ��5
#define FAN_ID_SHUANGLONG	  77	// ˫��
#define FAN_ID_TIANHU		  78	// ���
#define FAN_ID_DIHU			  79	// �غ�
#define FAN_ID_RENHU		  80	// �˺�
#define FAN_ID_BAIWANSHI	  81	// ����ʯ
#define FAN_ID_HONGKONGQUE	  82	// ���ȸ
#define FAN_ID_DONGBEI		  83	// �����¸���
#define FAN_ID_KONGQUEDNF     84	// ��ȸ���Ϸ�
#define FAN_ID_SHUANGJIANKE	  85	// ˫����
#define FAN_ID_YISHUANGLONG	  86	// һɫ˫����
#define FAN_ID_LIANQIDUI	  87	// ��7��
#define FAN_ID_TING			  88	// ����
#define FAN_ID_TIANTING		  89	// ����
#define FAN_ID_LONGQIDUI	  90	// ��7��
#define FAN_ID_YOUZI		  91	// ����
#define FAN_ID_PAOHU		  92	// �ں�
#define FAN_ID_DUTING		  93	// ����
#define FAN_ID_DUYIDUI		  94	// ��һ��
#define FAN_ID_258JIANG		  95	// 258��
#define FAN_ID_ZHUANGJIA	  96	// ׯ��
#define FAN_ID_DADIAOCHE	  97	// �����
#define FAN_ID_QING19		  98	// ���۾�
#define FAN_ID_4ANKE          99    // �İ���
#define FAN_ID_XIXIANGFENG   100    // ϲ���
#define FAN_ID_MQQ           101    // ��ǰ��
#define FAN_ID_QUANDAI1		 102    // ȫ����
#define FAN_ID_SANSHUANGLONG 103    // ��ɫ˫����
#define FAN_ID_FENG19KE		 104	// 19��(����)
#define FAN_ID_PIHU          105    // ƨ��

/**                          
 * @ingroup MJFrame          
 * @brief �㷬�õĻ������ݽ� ��
 */	                         
typedef	struct tagENVIRONMENT
{                            
	BYTE	byChair;		 				// ���˭��
	BYTE	byTurn;			 				// �ֵ�˭������ǵ��ڣ����ǵ��ڵ��Ǹ���
	BYTE	tHand[4][MAX_HAND_TILES];		// �ļ����ϵ���
	BYTE	byHandCount[4];	 				// �����м�����
                             
	BYTE	tSet[4][MAX_SET_TILES][3];		// �ļң�4���ƣ�flag��tile��chair
	BYTE	bySetCount[4];	 				// set�м�����
                             
	BYTE	tGive[4][40];	 				// �ļҳ�������
	BYTE	byGiveCount[4];	 				// ÿ�˳��˼�����
                             
	BYTE	tLast;			 				// ���͵�������
	BYTE	tGodTile;		 				// ������
	BYTE	byFlag;			 				// 0������1���ڡ�2���ϻ���3����
	BYTE    ucDealer;		 				// ׯ��
	BYTE    ucDice[DICE_NBR];				// ɫ��
	BYTE	byRoundWind;	 				// Ȧ��
	BYTE	byPlayerWind;	 				// �ŷ�
	BYTE	byTilesLeft;	 				// ��ʣ�������ƣ��������㺣�׵�
	BYTE    byCollectWind;	 				// ����Ҳ���Գ�(��������)
    BYTE    byCollectArrow;                 // ��ͷ��Ҳ���Գ�(�з���)

	BYTE	byFlowerCount[4];				// 4�Ҹ��ж����Ż�	
	BYTE	byTing[4];						// ���Ƶ����
	BYTE	byOtherInfo[4];					// ������Ϣ
	BYTE    byOtherInfo2[4];				// ������Ϣ
}ENVIRONMENT;


/**
 * @ingroup MJFrame
 * @brief CMJFanCounter
 *
 * <p>�� 
 *		CMJFanCounter, ���Ĺ�����
 * </p>
 * @sa
 */	
class /*SERAPI*/ CMJFanCounter  
{
public:
	CMJFanCounter();
	virtual ~CMJFanCounter();

	// ����ʹ�õĺ���ָ��
	typedef BOOL (*CHECKFUNC)(CMJFanCounter*);

	/**
	 *  @brief ��ȡ����
	 *  @return  ����������ܷ���
	 */	
	virtual int GetFanCount();

	/**
	 *  @brief �����㷬�Ļ���
	 *  @param[in]  env ����������
	 */	
	virtual void SetEnv( ENVIRONMENT& env );

	/**
	 *  @brief �������н����Ϣ
	 *  @param[in]  tagFanCount ����������
	 */	
	virtual void SetFanCount( FAN_COUNT& tagFanCount );

	/**
	 *  @brief ��������CHECK���
	 *
	 *  �������ý���CHECK����,��ȷ���´��㷬��ʱ���ܱ�����
	 */	
	virtual void ResetFanCount();

	/**
	 *  @brief ���ָ����ҵ�����
	 *  @param[out]  mjHand �����÷�ʽ�������ø���
	 *  @param[in]  nChair �����ȡ�����
	 * 
	 *  ��ȡ���ݵ���Դ�����õĻ������ݽṹ
	 */	
	virtual void GetHandTiles( CMJHand& mjHand, int nChair );

	/**
	 *  @brief ���ָ����ҵ�ȫ������
	 *  @param[out]  mjAll �����÷�ʽ�������ø���
	 *  @param[in]  nChair �����ȡ�����
	 *
	 *  ��ȡ���ݵ���Դ�����õĻ������ݽṹ,�˺��������ҵ��������ϵ�������һ�𷵻�,�ܱ����ϻ����
	 */	
	virtual void GetAllTiles( CMJHand& mjAll, int nChair );

	/**
	 *  @brief ���һ�����Ľ��
	 *  @param[in/out]  tagFan �������ݽṹ
	 *
	 *  ����������Զ����ý��ΪTRUE�����ر��������õĳ�����͹رյķ���
	 */	
	virtual void CheckFanNode( FAN_NODE &tagFan );

	/**
	 *  @brief ������ID��Ӱ�亯���Ƿ����
	 *  @param[in]  nID ��Ӱ��ķ�ID
	 *  @return �����������TRUE,���򷵻�FALSE
	 */	
	virtual BOOL CheckFanByID( int nID );

	/**
	 *  @brief �����ͨ����
	 *  @param[in]  tilesHand ��������
	 *  @param[in]  bNoJiang �Ƿ�Ҫ�жϽ���,���ΪTRUE,�������н���
	 *  @return �����������TRUE,���򷵻�FALSE]
	 */	
	BOOL CheckWinNormal(CMJHand tilesHand, BOOL bNoJiang = FALSE );

	/**
	 *  @brief ���ȫ˳�Ӻ���
	 *  @param[in]  tilesHand ��������
	 *  @return �����������TRUE,���򷵻�FALSE
	 */	
	BOOL CheckWinCollect(CMJHand tilesHand);

	/**
	 *  @brief ���ȫ���Ӻ���
	 *  @param[in]  tilesHand ��������
	 *  @return �����������TRUE,���򷵻�FALSE
	 */	
	static BOOL CheckWinTriplet(CMJHand tilesHand);

    /**
    *  @brief ���ȫ��X����(XΪĳ���Ƶ�)
    *  @param[in]  tilesHand ��������
    *  @param[in]  byNum     �ж�ÿ�鼰����Ҫ�������Ƶ�
    *  @return �����������TRUE,���򷵻�FALSE
    */	
    static BOOL CheckWinWithX(CMJHand tilesHand, BYTE byNum);
	
	/**
	 *  @brief ��鴿ȫ19����
	 *  @param[in]  tilesHand ��������
	 *  @return �����������TRUE,���򷵻�FALSE
	 */	
	static BOOL CheckWinWith19(CMJHand tilesHand);

	// ���ַ�
	static BOOL Check000(CMJFanCounter* pCounter);
	static BOOL Check001(CMJFanCounter* pCounter);
	static BOOL Check002(CMJFanCounter* pCounter);
	static BOOL Check003(CMJFanCounter* pCounter);
	static BOOL Check004(CMJFanCounter* pCounter);
	static BOOL Check005(CMJFanCounter* pCounter);
	static BOOL Check006(CMJFanCounter* pCounter);
	static BOOL Check007(CMJFanCounter* pCounter);
	static BOOL Check008(CMJFanCounter* pCounter);
	static BOOL Check009(CMJFanCounter* pCounter);

	static BOOL Check010(CMJFanCounter* pCounter);
	static BOOL Check011(CMJFanCounter* pCounter);
	static BOOL Check012(CMJFanCounter* pCounter);
	static BOOL Check013(CMJFanCounter* pCounter);
	static BOOL Check014(CMJFanCounter* pCounter);
	static BOOL Check015(CMJFanCounter* pCounter);
	static BOOL Check016(CMJFanCounter* pCounter);
	static BOOL Check017(CMJFanCounter* pCounter);
	static BOOL Check018(CMJFanCounter* pCounter);
	static BOOL Check019(CMJFanCounter* pCounter);

	static BOOL Check020(CMJFanCounter* pCounter);
	static BOOL Check021(CMJFanCounter* pCounter);
	static BOOL Check022(CMJFanCounter* pCounter);
	static BOOL Check023(CMJFanCounter* pCounter);
	static BOOL Check024(CMJFanCounter* pCounter);
	static BOOL Check025(CMJFanCounter* pCounter);
	static BOOL Check026(CMJFanCounter* pCounter);
	static BOOL Check027(CMJFanCounter* pCounter);
	static BOOL Check028(CMJFanCounter* pCounter);
	static BOOL Check029(CMJFanCounter* pCounter);

	static BOOL Check030(CMJFanCounter* pCounter);
	static BOOL Check031(CMJFanCounter* pCounter);
	static BOOL Check032(CMJFanCounter* pCounter);
	static BOOL Check033(CMJFanCounter* pCounter);
	static BOOL Check034(CMJFanCounter* pCounter);
	static BOOL Check035(CMJFanCounter* pCounter);
	static BOOL Check036(CMJFanCounter* pCounter);
	static BOOL Check037(CMJFanCounter* pCounter);
	static BOOL Check038(CMJFanCounter* pCounter);
	static BOOL Check039(CMJFanCounter* pCounter);

	static BOOL Check040(CMJFanCounter* pCounter);
	static BOOL Check041(CMJFanCounter* pCounter);
	static BOOL Check042(CMJFanCounter* pCounter);
	static BOOL Check043(CMJFanCounter* pCounter);
	static BOOL Check044(CMJFanCounter* pCounter);
	static BOOL Check045(CMJFanCounter* pCounter);
	static BOOL Check046(CMJFanCounter* pCounter);
	static BOOL Check047(CMJFanCounter* pCounter);
	static BOOL Check048(CMJFanCounter* pCounter);
	static BOOL Check049(CMJFanCounter* pCounter);

	static BOOL Check050(CMJFanCounter* pCounter);
	static BOOL Check051(CMJFanCounter* pCounter);
	static BOOL Check052(CMJFanCounter* pCounter);
	static BOOL Check053(CMJFanCounter* pCounter);
	static BOOL Check054(CMJFanCounter* pCounter);
	static BOOL Check055(CMJFanCounter* pCounter);
	static BOOL Check056(CMJFanCounter* pCounter);
	static BOOL Check057(CMJFanCounter* pCounter);
	static BOOL Check058(CMJFanCounter* pCounter);
	static BOOL Check059(CMJFanCounter* pCounter);

	static BOOL Check060(CMJFanCounter* pCounter);
	static BOOL Check061(CMJFanCounter* pCounter);
	static BOOL Check062(CMJFanCounter* pCounter);
	static BOOL Check063(CMJFanCounter* pCounter);
	static BOOL Check064(CMJFanCounter* pCounter);
	static BOOL Check065(CMJFanCounter* pCounter);
	static BOOL Check066(CMJFanCounter* pCounter);
	static BOOL Check067(CMJFanCounter* pCounter);
	static BOOL Check068(CMJFanCounter* pCounter);
	static BOOL Check069(CMJFanCounter* pCounter);

	static BOOL Check070(CMJFanCounter* pCounter);
	static BOOL Check071(CMJFanCounter* pCounter);
	static BOOL Check072(CMJFanCounter* pCounter);
	static BOOL Check073(CMJFanCounter* pCounter);
	static BOOL Check074(CMJFanCounter* pCounter);
	static BOOL Check075(CMJFanCounter* pCounter);
	static BOOL Check076(CMJFanCounter* pCounter);
	static BOOL Check077(CMJFanCounter* pCounter);
	static BOOL Check078(CMJFanCounter* pCounter);
	static BOOL Check079(CMJFanCounter* pCounter);
	
	static BOOL Check080(CMJFanCounter* pCounter);
	static BOOL Check081(CMJFanCounter* pCounter);
	static BOOL Check082(CMJFanCounter* pCounter);
	static BOOL Check083(CMJFanCounter* pCounter);
	static BOOL Check084(CMJFanCounter* pCounter);
	static BOOL Check085(CMJFanCounter* pCounter);
	static BOOL Check086(CMJFanCounter* pCounter);
	static BOOL Check087(CMJFanCounter* pCounter);
	static BOOL Check088(CMJFanCounter* pCounter);
	static BOOL Check089(CMJFanCounter* pCounter);
	
	static BOOL Check090(CMJFanCounter* pCounter);
	static BOOL Check091(CMJFanCounter* pCounter);
	static BOOL Check092(CMJFanCounter* pCounter);
	static BOOL Check093(CMJFanCounter* pCounter);
	static BOOL Check094(CMJFanCounter* pCounter);
	static BOOL Check095(CMJFanCounter* pCounter);
	static BOOL Check096(CMJFanCounter* pCounter);
	static BOOL Check097(CMJFanCounter* pCounter);
	static BOOL Check098(CMJFanCounter* pCounter);
	static BOOL Check099(CMJFanCounter* pCounter);

	static BOOL Check100(CMJFanCounter* pCounter);
	static BOOL Check101(CMJFanCounter* pCounter);
	static BOOL Check102(CMJFanCounter* pCounter);
	static BOOL Check103(CMJFanCounter* pCounter);
	static BOOL Check104(CMJFanCounter* pCounter);
	static BOOL Check105(CMJFanCounter* pCounter);
	static BOOL Check106(CMJFanCounter* pCounter);
	static BOOL Check107(CMJFanCounter* pCounter);
	static BOOL Check108(CMJFanCounter* pCounter);
	static BOOL Check109(CMJFanCounter* pCounter);

	static BOOL Check110(CMJFanCounter* pCounter);
	static BOOL Check111(CMJFanCounter* pCounter);
	static BOOL Check112(CMJFanCounter* pCounter);
	static BOOL Check113(CMJFanCounter* pCounter);
	static BOOL Check114(CMJFanCounter* pCounter);
	static BOOL Check115(CMJFanCounter* pCounter);
	static BOOL Check116(CMJFanCounter* pCounter);
	static BOOL Check117(CMJFanCounter* pCounter);
	static BOOL Check118(CMJFanCounter* pCounter);
	static BOOL Check119(CMJFanCounter* pCounter);

	static BOOL Check120(CMJFanCounter* pCounter);
	static BOOL Check121(CMJFanCounter* pCounter);
	static BOOL Check122(CMJFanCounter* pCounter);
	static BOOL Check123(CMJFanCounter* pCounter);
	static BOOL Check124(CMJFanCounter* pCounter);
	static BOOL Check125(CMJFanCounter* pCounter);
	static BOOL Check126(CMJFanCounter* pCounter);
	static BOOL Check127(CMJFanCounter* pCounter);

	// �㷬�Ļ���
	ENVIRONMENT m_tagEnv;
	FAN_COUNT m_tagFanCount;

    // ��ǰ�жϵķ���ID
    BYTE m_byFanID;

	// ��¼�ĺ���ָ��
	CHECKFUNC m_fpCheckFan[MAX_FAN_NUMBER];

};

}

#endif
