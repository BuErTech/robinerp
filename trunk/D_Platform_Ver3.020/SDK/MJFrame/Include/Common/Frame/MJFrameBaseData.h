/** @file	MJFrameBaseData.h
 *	@brief	�����齫��صĶ�����Ϣ
 *	@author edward
 *	@date 08.08.04
 */
#ifndef _MJFRAMEBASEDATA_H
#define _MJFRAMEBASEDATA_H

#include "..\commonStructs.h"
#include "mjdef.h"
#include "mjcfgdef.h"
#include "MJ.h"
#include "..\typeDef.h"

namespace MyGame
{

#define MJ_MONEY_TYPE_NORMAL		0
#define MJ_MONEY_TYPE_GAMEMONEY		1
#define MJ_MONEY_TYPE_HAPPYMONEY	2

// Ĭ����ʾ����
// #define MJ_INFO_HAND_NBR	18	// ����
// #define MJ_INFO_GIVE_NBR	36  // ����
#define MJ_INFO_WALL_NBR	36  // ��ǽ

// ���Ʋ�������ʾ��
#define MJ_BLOCK_NBR		        8	// (����������)
#define MJ_BLOCK_COLLECT	        0
#define MJ_BLOCK_TRIPLET	        1
#define MJ_BLOCK_QUADRUPLET	        2
#define MJ_BLOCK_TING		        3
#define MJ_BLOCK_WIN		        4
#define MJ_FLOWER_NBR		        8
#define DICE_NBR	                2

// �Լ�������״̬
#define MJ_STATE_SELECT_NONE	    0	// �����Լ�ѡ�����
#define MJ_STATE_SELECT_GIVE	    1	// ѡ�����
#define MJ_STATE_SELECT_COLLECT	    2	// ����ѡ��
#define MJ_STATE_SELECT_QUADRUPLET	3	// ���Ƶ�ѡ��
#define MJ_STATE_SELECT_TING	    4   // ����ѡ��

// ʱ��IDӳ������
#define MJ_TIMERID_MAP_NBR	        64
	
// ״̬����
#define MJ_STATE_NOSTART	        0	// û��ʼ
#define MJ_STATE_DEAL		        1	// ����
#define MJ_STATE_FLOWER		        2	// ����
#define MJ_STATE_BLOCK		        3	// ����
#define MJ_STATE_GIVE		        4	// ����
#define MJ_STATE_DRAW		        5	// ����
#define MJ_STATE_WAIT		        6	// �ȴ�
#define MJ_STATE_END		        7	// ����
#define MJ_STATE_DRAW_WAIT	        8	// ���Ƶȴ�
#define MJ_STATE_NONE		        9	// ����

// �жϷ���ʱʹ�õĺ�������
#define MJ_WIN_SELFDRAW				0	// ����
#define MJ_WIN_GUN					1	// ����
#define MJ_WIN_GANGDRAW				2	// ���ϻ�
#define MJ_WIN_GANG					3	// ����
#define MJ_WIN_GANGGIVE				4	// ������

// ��������
#define MJ_END_NO					0	// û����  
#define MJ_END_SELF					1	// ����
#define MJ_END_GUN					2	// ����
#define MJ_END_NOTILE				3	// ����
#define MJ_END_ESCAPE				4	// ����
#define MJ_END_REQUEST				5	// Э���Ƴ�
#define MJ_END_SYSTEM				6	// ��ɢ
#define MJ_END_QIANG				7	// ����


#define MJ_GANG_STATE_NONE			0
#define MJ_GANG_STATE_DRAW			1	// �ܻ�
#define MJ_GANG_STATE_GIVE			2	// ��
#define MJ_GANG_STATE_BLOCK			3	// ��

#define MJ_TING_NONE				0	
#define MJ_TING_NORMAL				1
#define MJ_TING_BEGIN				2	// ����

// ʱ������
// �뼶��ʱ��ID��Χ 1 - 50
#define MJ_TIMER_READY				1
#define MJ_TIMER_GIVE				2
#define MJ_TIMER_BLOCK				3
#define MJ_TIMER_WAIT				4
#define MJ_TIMER_FLOWER				5

// ƽ̨���붨ʱ��ID: ��Ϸ����(4) - 50, С�ڵ���4����ڵ���50��Ч
#define MJ_TIMER_DRAW       		10   // ��ʱ����
#define MJ_TIMER_DEAL_FINISH        11   // ��ʱȷ�Ϸ������

// ��ʱ2�μ��й�
#define MJ_TURST_TIMEOUT_MAX	    2

// �ͻ��˵�ʱ�䶨��
#define MJ_GAME_TIMER_LOGIC	        10	// �߼�ʱ�䣬ͬ�������һ�� ��Ϸ���������е�ʱ��

// GM����,����ÿһ���������ظ��֣���Ȼ��������������
#define GMCMD_SWAP "swap"
#define GMCMD_SET  "set"
#define GMCMD_NEXT "next"

#define GMCMD_SET_WALLLEN	"walllen"
#define GMCMD_SET_AUTOSTART "autostart"
#define GMCMD_SET_DEALER	"dealer"
#define GMCMD_SET_DICE		"dice"
#define GMCMD_SET_WALL		"walltile"

// ֪ͨ�ͻ��������Զ���ʼ
#define GMCMD_NOTIFY_AUTOSTART	1

// �齫�������ϢID
#define MJFRAME_MOUDLE_MSG	80
	
#define CS_MJ_REQUSET_REPLACE_ALLTILES	1	//�Զ�������
#define SC_MJ_NOTIFY_REPLACE_ALLTILES	2	

#define CS_MJ_REQUEST_GIVE				3
#define SC_MJ_NOTIFY_GIVE				4

#define SC_MJ_NOTIFY_DRAW				5

#define CS_MJ_REQUEST_BLOCK				6
#define SC_MJ_NOTIFY_BLOCK				7
#define SC_MJ_NOTIFY_BLOCK_RESULT		8

#define CS_MJ_REQUEST_FLOWER			9
#define SC_MJ_NOTIFY_FLOWER				10 
#define SC_MJ_NOTIFY_DEAL				11
#define SC_MJ_NOTIFY_FINISH				12
#define SC_MJ_NOTIFY_DEALER             52
#define CS_MJ_REQUEST_DEAL_FINISH       56  // ���������

// ������������
#define CS_MJ_REQUEST_CHANGE_TILES		13
#define MJ_CHANGE_RANDOM	            0	// �����
#define MJ_CHANGE_TARGET	            1	// ��ָ��Ŀ��
#define SC_MJ_NOTIFY_CHANGE_TILES		14

// �����ȡָ���Ƶ�λ��
#define CS_MJ_REQUEST_GETPOS_TILE		15
#define SC_MJ_NOTIFY_GETPOS_TILE		16

// �����ȡ�����4������Ϣ
#define CS_MJ_REQUEST_GETTILES			17
#define MJ_GETTILES_NBR		            4
#define SC_MJ_NOTIFY_GETTILES			18


// ��������
#define CS_MJ_REQUEST_CONFIGTILE		20
#define SC_MJ_NOTIFY_CONFIGTILE			21

// ǿ��ˢ���������
#define SC_MJ_UPDATEHANDTILES			23

// �ԹۺͶ��������ˢ����Ϣ
#define SC_MJ_NOTIFY_VIEW_INFO			24

// ����޸��Թۺ󷢸��Թ��ߵ���Ϣ
#define SC_MJ_NOTIFY_CHANGE_LOOKON		25

// ��ҽ�������һЩ������Ϣ
#define SC_MJ_NOTIFY_BASE_INFO			26

// ��������״̬����ʽ��ʼ��Ϸ
#define SC_MJ_NOTIFY_FLOWER_OVER		27

// �����Ի�����ʾ��Ϣ
#define SC_MJ_NOTIFY_SHOWDLG			28

// ������Ϣ
#define CS_MJ_REQUEST_CHECK_WIN			31
#define SC_MJ_NOTIFY_CHECK_WIN			32

// ����GM��Ϣ
#define CS_MJ_REQUEST_GMCMD				33
#define SC_MJ_NOTIFY_GMCMD				34

// ���ֵ���Ϣ
#define SC_MJ_NOTIFY_RUN_NBR			35

// SOǿ��ˢ�¿ͻ�������
#define SC_MJ_NOTIFY_RESET_DATA			50


#pragma pack(1)

// ��Ϸ׼����Ϣ(��ID180, ��ID2, 1�ѱ�ƽ̨��ȡ����)
#define ASS_SUB_USERAGREE  50

// ��Ϸ�й���Ϣ(��ID180, ��ID10)
#define ASS_SUB_TRUSTEE  51

//�û�ͬ����Ϣ�ṹ
struct MSG_GR_S_Agree 
{
	int		bDeskNO;						//��Ϸ����
	int		bDeskStation;					//λ�ú���
	int		bAgreeGame;						//ͬ���־
	int		bNotUse;						//�������ڶ���ռλ��
};

// �й���Ϣ�ṹ��
struct MSG_GR_S_UserTruste
{
	int byChair;
	int byFlag;   // 0: ȡ���й�   1: �й� 
};

/**
 * @ingroup MJFrame
 * @brief �������Ƶ����ݽṹ
 *
 * ������óɹ��ڳ�ʼ��һ�ֵ�ʱ��,SO����ִ��ϴ�ƺ�����ׯ�Ҳ���
 */	
struct TCMD_REQUEST_REPLACE_ALLCARDS : public TCMD_HEADER
{
	int nCount;		   // ��ǽ����
	BYTE szCards[144]; // ��
	char cbBankUser;   // ׯ
    BYTE ucDice[2];	   // 2��ɫ��
	TCMD_REQUEST_REPLACE_ALLCARDS()
	{
		memset(this, 0, sizeof(TCMD_REQUEST_REPLACE_ALLCARDS));
		cCmdPara = CS_MJ_REQUSET_REPLACE_ALLTILES;
		cCmdID = MJFRAME_MOUDLE_MSG;
		nDataLen = sizeof(TCMD_REQUEST_REPLACE_ALLCARDS);
	}
};

/**
 * @ingroup MJFrame
 * @brief ��ȡ���ƽ�������ݽṹ
 */	
struct TCMD_NOTIFY_REPLACE_ALLCARDS : public TCMD_HEADER
{
	// ���
	int nResult;
	TCMD_NOTIFY_REPLACE_ALLCARDS()
	{
		memset(this, 0, sizeof(TCMD_NOTIFY_REPLACE_ALLCARDS));
		cCmdPara = SC_MJ_NOTIFY_REPLACE_ALLTILES;
		cCmdID = MJFRAME_MOUDLE_MSG;
		nDataLen = sizeof(TCMD_NOTIFY_REPLACE_ALLCARDS);
	}
};

/**
 * @ingroup MJFrame
 * @brief �������
 */	
struct TCMD_REQUEST_GIVE: public TCMD_HEADER 
{
	int t;		
	int ucTileLength;	// ���ƺ�����Ƴ���
	int ucIndex;		// ������ҳ��������������ĸ�λ�õ���
	int standBlank;		// ռλ��
    TCMD_REQUEST_GIVE()
    {
        memset(this, 0, sizeof(TCMD_REQUEST_GIVE));
        cCmdPara = CS_MJ_REQUEST_GIVE;
 		cCmdID = MJFRAME_MOUDLE_MSG;
		nDataLen = sizeof(TCMD_REQUEST_GIVE);
    }
};

/**
 * @ingroup MJFrame
 * @brief ֪ͨ����
 */	
struct TCMD_NOTIFY_GIVE: public TCMD_HEADER 
{
	int t;	
	int chair;
	int ucHandType; // �ֶ�������
	int ucIndex;    // ������ҳ��������������ĸ�λ�õ���
	int ucInsertPos;// ������ҳ��ƺ�, ���ұ����Ÿ������ưڷ�λ��
    BOOL bTing;      // �Ƿ�Ϊ���Ƴ���

    TCMD_NOTIFY_GIVE()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_GIVE));
        cCmdPara = SC_MJ_NOTIFY_GIVE;
 		cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_GIVE);
    }
};

/**
 * @ingroup MJFrame
 * @brief ��������
 */	
struct TCMD_REQUEST_BLOCK: public TCMD_HEADER 
{
	BYTE ucFlag;		// ���ݳ����ܶ����־ ACTION_EMPTY
	BYTE ucTile[4];		// ������
    TCMD_REQUEST_BLOCK()
    {
        memset(this, 0, sizeof(TCMD_REQUEST_BLOCK));
        cCmdPara = CS_MJ_REQUEST_BLOCK;
 		cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_REQUEST_BLOCK);
    }
};

/**
* @ingroup MJFrame
* @brief ���������
*/	
struct TCMD_REQUEST_DEAL_FINISH : public TCMD_HEADER
{
    TCMD_REQUEST_DEAL_FINISH ()
    {
        memset(this, 0, sizeof(TCMD_REQUEST_DEAL_FINISH));
        cCmdPara = CS_MJ_REQUEST_DEAL_FINISH;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_REQUEST_DEAL_FINISH);
    }
};

/**
* @ingroup MJFrame
* @brief ֪ͨׯ��λ��
*/	
struct TCMD_NOTIFY_DEALER : public TCMD_HEADER
{
    BYTE byDealer;	  // ׯ��λ��

    TCMD_NOTIFY_DEALER()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_DEALER));
        cCmdPara = SC_MJ_NOTIFY_DEALER;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_DEALER);
    }
};

/**
* @ingroup MJFrame
* @brief ֪ͨ����
*/	
struct TCMD_NOTIFY_BLOCK: public TCMD_HEADER 
{
    BYTE ucFlag;		     // û��(0)����(1)����(2)����(4)����(8)����(16)
    BYTE ucTile[5][24];      // ��(0)����(1)����(2)����(3)����(4)������(��������, ���6������, ������������������ŵ�)
    BYTE ucChair;		     // ����λ��
    BYTE ucTingTile[24][15]; // ���ƺ���Ժ�������ucTile��Ӧ

    TCMD_NOTIFY_BLOCK()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_BLOCK));
        cCmdPara = SC_MJ_NOTIFY_BLOCK;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_BLOCK);
    }
};


/**
 * @ingroup MJFrame
 * @brief ֪ͨ���ƽ��
 */	
struct TCMD_NOTIFY_BLOCK_RESULT: public TCMD_HEADER 
{
	BYTE ucFlag;		// �ԡ������ܡ�������
	BYTE ucTile[4];   	// ������
	BYTE ucChair;		// ˭�ԡ�����
	BYTE ucWhich;		// �ԡ���˭��

    TCMD_NOTIFY_BLOCK_RESULT()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_BLOCK_RESULT));
        cCmdPara = SC_MJ_NOTIFY_BLOCK_RESULT;
 		cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_BLOCK_RESULT);
    }
};


/**
* @ingroup MJFrame
* @brief ֪ͨ����
*/	
struct TCMD_NOTIFY_DRAW : public TCMD_NOTIFY_BLOCK 
{
    // �������߼�ȫ���ڷ������ж�, ���Լ̳�������Ϣ, ������Ϣ��������Ϣ�ϲ���һ����ȥ
    BYTE t;	                // ������
    BYTE chair;             // ������
    BYTE t_target;          // ���Ʒ�λ(0: ����ץ��,  1:����λ��ץ��(����)) 
	BYTE byDice[DICE_NBR];  // ����(ĳЩ��������, ���Ʊ��������������Ӿ���)

    TCMD_NOTIFY_DRAW()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_DRAW));
        cCmdPara = SC_MJ_NOTIFY_DRAW;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_DRAW);
    }
};


/**
 * @ingroup MJFrame
 * @brief ���󲹻�
 */	
struct TCMD_REQUEST_FLOWER: public TCMD_HEADER 
{
	BYTE ucChair;		//	˭��
	BYTE ucNumber;		//	���˼���

    TCMD_REQUEST_FLOWER()
    {
        memset(this, 0, sizeof(TCMD_REQUEST_FLOWER));
        cCmdPara = CS_MJ_REQUEST_FLOWER;
 		cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_REQUEST_FLOWER);
    }
};

/**
 * @ingroup MJFrame
 * @brief ֪ͨ����
 */	
struct TCMD_NOTIFY_FLOWER: public TCMD_HEADER 
{
	BYTE ucChair;					//	˭��
	BYTE ucNumber;					//	���˼���
	BYTE ucTile[MJ_FLOWER_NBR];	    //	�ļ��Ż���
	BYTE ucOldTile[MJ_FLOWER_NBR];	//	Ҫ�������ļ���
	BYTE ucFlower[MJ_FLOWER_NBR];	//  8��������

    TCMD_NOTIFY_FLOWER()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_FLOWER));
        cCmdPara = SC_MJ_NOTIFY_FLOWER;
		cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_FLOWER);
    }
};

/**
 * @ingroup MJFrame
 * @brief ֪ͨ���ַ���
 */	
struct TCMD_NOTIFY_DEAL: public TCMD_HEADER 
{
	int tiles[17];		    // �Լ�������
	int ucDealer;		    // ׯ��
	int ucRoundWind;	    // Ȧ��
	int ucRoundWindNbr;	    // ��Ȧ�ĵڼ���
	int ucDice[2];		    // �������ӵ�����
	int ucHandType;         // ��������ʱ, ʹ�õ���ָ����: 0: ʳָ 1: �м�
	int ucGodLength;	    // �����Ƶĳ���
	int ucGodTiles[8];	    // ������
	int ucOther[8];	        // ������Ϣ

    TCMD_NOTIFY_DEAL()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_DEAL));
        cCmdPara = SC_MJ_NOTIFY_DEAL;
		cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_DEAL);
    }
};

/**
 * @ingroup MJFrame
 * @brief ֪ͨ����
 */	
struct TCMD_NOTIFY_FINISH: public TCMD_HEADER 
{
	BYTE byFlag;			     // ״̬�����ơ����ڡ�����
	BYTE byFan[4][128];			 // ������Щ��
	BYTE byFanScore[4][128];     // ÿ�ַ���Ӯ�ķ���
    int  nFanPoint[4];           // �ļ���Ӯ����
	int  nGangFen[4];            // �ļ���Ӯ�ܷ�
	BYTE byWhoWin[4];		     // ˭�͵�(Ҫ֧��һ�ڶ���)
	BYTE byWhoGun;			     // �����ڵ����
	BYTE tLast;			         // �͵���һ��
	int	 nScore[4];              // ��Ӯ����
	int	 nMoney[4];              // ��Ӯ���
                                
	BYTE tile[4][18];            // �ĸ�������ϵ���
    BYTE byBlockTiles[4][20];	 // ��ҵ���������(�����������)(������ܶ��ǰ���4��4���Ĵ��)
    BYTE byBlockTilesFlag[4][5]; // ������Ƶ�����(�����ܱ�־)
    BYTE byBlockTilesWhich[4][5];// �������˭���ƣ�����������һ��

	int  nTax[4];                // ̨��

	BYTE byOtherData[4];		 // ��������
	BYTE byOtherData2[4]; 		 // ��������

    TCMD_NOTIFY_FINISH()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_FINISH));
		cCmdPara = SC_MJ_NOTIFY_FINISH;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_FINISH);
    }
};

/**
 * @ingroup MJFrame
 * @brief ������
 */	
struct TCMD_REQUEST_CHANGE_TILES: public TCMD_HEADER
{
	BYTE ucRequestTile; // �������
	BYTE ucChangeType;	// ��������
	BYTE ucTargetTile;	// Ŀ����

    TCMD_REQUEST_CHANGE_TILES()
    {
        memset(this, 0, sizeof(TCMD_REQUEST_CHANGE_TILES));
		cCmdPara = CS_MJ_REQUEST_CHANGE_TILES;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_REQUEST_CHANGE_TILES);
    }
};

/**
 * @ingroup MJFrame
 * @brief ֪ͨ����
 */	
struct TCMD_NOTIFY_CHANGE_TILES: public TCMD_HEADER
{
	BYTE ucRequestTile; // �������
	BYTE ucChangeType;	// ��������
	BYTE ucTargetTile;	// Ŀ����
	BYTE ucResult;		// ���ƽ��
    TCMD_NOTIFY_CHANGE_TILES()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_CHANGE_TILES));
		cCmdPara = SC_MJ_NOTIFY_CHANGE_TILES;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_CHANGE_TILES);
    }
};

/**
 * @ingroup MJFrame
 * @brief �����ȡĿ����λ��
 */	
struct TCMD_REQUEST_GETPOS_TILE: public TCMD_HEADER
{
	BYTE ucRequestTile;	// �������

    TCMD_REQUEST_GETPOS_TILE()
    {
        memset(this, 0, sizeof(TCMD_REQUEST_GETPOS_TILE));
		cCmdPara = CS_MJ_REQUEST_GETPOS_TILE;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_REQUEST_GETPOS_TILE);
    }
};

/**
 * @ingroup MJFrame
 * @brief ֪ͨ��ȡ��Ŀ����λ��
 */	
struct TCMD_NOTIFY_GETPOS_TILE: public TCMD_HEADER
{
	BYTE ucRequestTile;	// �������
	BYTE ucTilePos;		// �Ƶ�λ��,0Ϊû��

    TCMD_NOTIFY_GETPOS_TILE()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_GETPOS_TILE));
		cCmdPara = SC_MJ_NOTIFY_GETPOS_TILE;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_GETPOS_TILE);
    }
};

/**
 * @ingroup MJFrame
 * @brief �����ȡһ�������ĳ�ǽ������
 */	
struct TCMD_REQUEST_GETTILES: public TCMD_HEADER
{
	BYTE ucGetNbr;	// �����ȡ������
    TCMD_REQUEST_GETTILES()
    {
        memset(this, 0, sizeof(TCMD_REQUEST_GETTILES));
		cCmdPara = CS_MJ_REQUEST_GETTILES;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_REQUEST_GETTILES);
    }
};

/**
 * @ingroup MJFrame
 * @brief ֪ͨ��ȡ�ĳ�ǽ������
 */	
struct TCMD_NOTIFY_GETTILES: public TCMD_HEADER
{
	BYTE ucGetTile[MJ_GETTILES_NBR];	// ��ѯ���ƣ���಻����4��
	BYTE ucGetNbr;						// ��ѯ������

    TCMD_NOTIFY_GETTILES()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_GETTILES));
		cCmdPara = SC_MJ_NOTIFY_GETTILES;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_GETTILES);
    }
};


/**
 * @ingroup MJFrame
 * @brief ����ʹ������
 */	
struct TCMD_REQUSET_CONFIGTILE: public TCMD_HEADER
{
	BYTE ucChair;	// ʹ�õ���
	
    TCMD_REQUSET_CONFIGTILE()
    {
        memset(this, 0, sizeof(TCMD_REQUSET_CONFIGTILE));
		cCmdPara = CS_MJ_REQUEST_CONFIGTILE;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_REQUSET_CONFIGTILE);
    }
};

/**
 * @ingroup MJFrame
 * @brief ֪ͨʹ������
 */	
struct TCMD_NOTIFY_CONFIGTILE: public TCMD_HEADER
{
	BYTE ucChair;	// ʹ�õ���
	BYTE bResult;
    TCMD_NOTIFY_CONFIGTILE()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_CONFIGTILE));
		cCmdPara = SC_MJ_NOTIFY_CONFIGTILE;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_CONFIGTILE);
    }
};

/**
 * @ingroup MJFrame
 * @brief ֪ͨˢ������
 */	
struct TCMD_NOTIFY_UPDATEHANDTILES: public TCMD_HEADER
{
	BYTE ucChair;				// ���µ���
	BYTE ucHandCount;			// ��������
	BYTE ucHandTiles[18];		// ��������
	BYTE ucGiveCount[4];		// ��������
	BYTE ucGiveTiles[4][36];	// ���Ƶ�����
	TCMD_NOTIFY_UPDATEHANDTILES()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_UPDATEHANDTILES));
		cCmdPara = SC_MJ_UPDATEHANDTILES;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_UPDATEHANDTILES);
    }
};

/**
 * @ingroup MJFrame
 * @brief ֪ͨ��Ϸ���������
 */	
struct TCMD_NOTIFY_VIEW_INFO: public TCMD_HEADER
{
	BYTE ucDealer;					        // ׯ��
	BYTE ucWindRound;				        // ��
	BYTE ucWindRoundNbr;		        	// Ȧ
	BYTE ucCurTurn;				            // ��ǰ������
	BYTE ucWallLength;				        // ��ǽʣ�µĳ���
	BYTE ucNowTime;				            // ��ǰ���е�ʱ��
	BYTE ucGameState;				        // ��ǰ����Ϸ״̬
	BYTE ucBLookOn;				            // �Թ�
	BYTE ucLastTile;				        // �������
	BYTE ucLastGiveChair;			        // �����Ƶ���
	BYTE ucDice[2];			                // ɫ�ӵ���
	BYTE ucHandTilesLength[4];		        // ������Ƶĳ���
	BYTE ucGiveTilesLength[4];		        // ��ҳ��Ƶĳ���
	BYTE ucSetTilesLength[4];		        // ������Ƶĳ���
	BYTE ucFlower[8];	                    // ����
	BYTE ucHandTiles[18];		            // ��ҵ����ƣ�ֻ���Լ���ߵ��˵�
    BYTE ucBlockTiles[4][20];	            // ��ҵ���������(�����������)
	BYTE ucBlockTilesFlag[4][5];	        // ������Ƶ�����(�����ܱ�־)
	BYTE ucBlockTilesWhich[4][5];           // �������˭���ƣ�����������һ��
	BYTE ucGiveTiles[4][36];	            // ��ҵĳ���
	BYTE ucGodTilesLength;			        // �����Ƶĳ���
	BYTE ucGodTiles[8];			            // ������
	BYTE ucFlowerNbr[4];			        // ��ҵĻ�������
	BYTE ucTingType[4];			            // ������Ƶ�����
    BYTE ucReady[4];                        // ׼��״̬

    BOOL bGiveBlockTiles[4][36];            // ��ҳ��Ʊ������˵���
    BOOL bGiveTingTiles[4][36];             // ��ҳ���������������

    BYTE ucTingTiles[20];                   // ���������Ժ�����
	BYTE ucTrusteeFlag[4];                  // �Ϲܱ�־

	BYTE ucDarwTileLength;                  // ����������
	BYTE ucGangTileLength;                  // �Ѹ�������

	
	//int ucTicket;				    // ��Ʊ
	//int nVoicePrice;				// �������ߵ�
	//int nBuyItemMoney;			// ������ߵĵ���

	//BYTE ucGangScore;				// ���ƵĻ���
	//BYTE ucMinFan;				// ��С��
	//BYTE ucBaseScore;				// �׷�
	//BYTE ucMoneyType;				// �۷�����

	//BYTE ucTilesNumber;			// �Ƶ��ܳ�
	//BYTE ucTilesHandNbr;			// ���Ƴ�

	//BYTE ucBlockLevelCollect;		// ���Ƶȼ�
	//BYTE ucBlockLevelTriplet;		// ���Ƶȼ�
	//BYTE ucBlockLevelQuadruplet;	// ���Ƶȼ�
	//BYTE ucBlockLevelTing;        // ���Ƶȼ�
	//BYTE ucBlockLevelWin;			// ���Ƶȼ�

	//BYTE ucTimerGive;				// ����ʱ��
	//BYTE ucTimerFlower;			// ����ʱ��
	//BYTE ucTimerBlock;			// ����ʱ��
	//BYTE ucTimerReady;			// ׼������ʱ

	//BYTE ucLouHu;					// ©��
	//BYTE ucCollateWind;			// ��˳
	//BYTE ucAllCanCollate;			// ȫ��
	//BYTE ucShowTing;				// ������ʾ
	//BYTE ucTingCanGang;			// ����
	//BYTE ucQiangGang;	
	//BYTE ucMoreWin;	
	//BYTE ucDrawGangBack;
	//BYTE ucCanWinLaizi;
	//BYTE ucCanBlockLaizi;

	//BYTE ucCheck7dui;
	//BYTE ucCheckQuanbukao;
	//BYTE ucCheck131;
	//BYTE ucCheckZuhelong;
	//BYTE ucCheckLualv;
	//BYTE ucCHeckLuafeng;

	TCMD_NOTIFY_VIEW_INFO()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_VIEW_INFO));
		cCmdPara = SC_MJ_NOTIFY_VIEW_INFO;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_VIEW_INFO);
    }
	
};

/**
 * @ingroup MJFrame
 * @brief ֪ͨ�޸��Թ�״̬
 */	
struct TCMD_NOTIFY_CHANGE_LOOKON: public TCMD_HEADER
{
	BYTE ucLookOn;						// �Ƿ������Թ�
	BYTE ucHandTilesLength;			    // ���Ƶĳ���
	BYTE ucHandTiles[MAX_HAND_TILES];	// ���Ƶ�����
	BYTE ucQuadrupletConcealedNbr;		// ���ܵ�����
	BYTE ucQuadrupletConcealedTiles[MAX_SET_TILES];	// ���ܵ�����

    TCMD_NOTIFY_CHANGE_LOOKON()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_CHANGE_LOOKON));
		cCmdPara = SC_MJ_NOTIFY_CHANGE_LOOKON;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_CHANGE_LOOKON);
    }
};

/**
 * @ingroup MJFrame
 * @brief ֪ͨ��Ϸ��ػ�����Ϣ
 */	
struct TCMD_NOTIFY_MJ_BASE_INFO: public TCMD_HEADER
{
    char ucTimerGive;			   // ����ʱ��
    char ucTimerFlower;		       // ����ʱ��
    char ucTimerBlock;		       // ����ʱ��
    char ucTimerReady;		       // ׼������ʱ

    char ucTimerCutCard;           // ���Ʋ���ʱ��
    char ucTimerDice;              // ���Ӳ���ʱ��
    char ucTimerDeal;              // ���Ʋ���ʱ��
    char ucTimerJP;                // ���Ʋ���ʱ��

    int  nOpenGM;                  // �Ƿ���˲���ͨ��

    char ucOpenLookTile;           // �Ƿ���(��Ϸ������, �Ƿ���ʾδ���Ƶ���ҵ���)
    char ucTimerLookTile;          // �����ƺ���೤ʱ�����ϴ��

    char ucGiveBlockShow;          // �Ƿ���ʾ���������б������ܵ���

	int  ucTilesNumber;		       // �Ƶ��ܳ�
	char ucTilesHandNbr;		   // ���Ƴ�

	BYTE ucGangScore;			   // ���ƵĻ���
	BYTE ucMinFan;			       // ��С��
	BYTE ucBaseScore;			   // �׷�
	BYTE ucMoneyType;			   // �۷�����

    /*
    int  ucTicket;			       // ��Ʊ
    int  nVoicePrice;			   // �������ߵ�
    int  nBuyItemMoney;

	BYTE ucBlockLevelCollect;	   // ���Ƶȼ�
	BYTE ucBlockLevelTriplet;	   // ���Ƶȼ�
	BYTE ucBlockLevelQuadruplet;   // ���Ƶȼ�
	BYTE ucBlockLevelTing;         // ���Ƶȼ�
	BYTE ucBlockLevelWin;		   // ���Ƶȼ�

	BYTE ucTimerGive;			   // ����ʱ��
	BYTE ucTimerFlower;		       // ����ʱ��
	BYTE ucTimerBlock;		       // ����ʱ��
	BYTE ucTimerReady;		       // ׼������ʱ

	BYTE ucLouHu;				   // ©��
	BYTE ucCollateWind;		       // ���ƿɳ�
    BYTE ucCollateArrow;           // ��ͷ�ƿɳ�
	BYTE ucAllCanCollate;		   // ȫ��
	BYTE ucShowTing;			   // ������ʾ
	BYTE ucTingCanGang;		       // ����
    BYTE ucDongNanXiBeiGang;       // ����������
    BYTE ucZhongFaBaiGang;         // �з���X��
	BYTE ucQiangGang;	
	BYTE ucMoreWin;
	BYTE ucDrawGangBack;
	BYTE ucCanWinLaizi;
	BYTE ucCanBlockLaizi;

	BYTE ucCheck7dui;
	BYTE ucCheckQuanbukao;
	BYTE ucCheck131;
	BYTE ucCheckZuhelong;
	BYTE ucCheckLualv;
	BYTE ucCHeckLuafeng;
    */

	TCMD_NOTIFY_MJ_BASE_INFO()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_MJ_BASE_INFO));
		cCmdPara = SC_MJ_NOTIFY_BASE_INFO;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_MJ_BASE_INFO);
    }
	
};

/**
 * @ingroup MJFrame
 * @brief ֪ͨ��������,�����¸�״̬
 */	
struct TCMD_NOTIFY_FLOWER_OVER: public TCMD_NOTIFY_BLOCK/*TCMD_HEADER*/
{
	BYTE ucInfo[4];
	TCMD_NOTIFY_FLOWER_OVER()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_FLOWER_OVER));
		cCmdPara = SC_MJ_NOTIFY_FLOWER_OVER;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_FLOWER_OVER);
    }
};

/**
 * @ingroup MJFrame
 * @brief ֪ͨ��ʾ��Ϣ�Ի���
 */	
struct TCMD_MJ_NOTIFY_SHOWDLG: public TCMD_HEADER 
{
	int  nMsgID;		// ��ϢID
	BYTE ucFlag;		// ��ʾ�Ի��������
	char szMsg[128];	// ������е���

    TCMD_MJ_NOTIFY_SHOWDLG()
    {
        memset(this, 0, sizeof(TCMD_MJ_NOTIFY_SHOWDLG));
		cCmdPara = SC_MJ_NOTIFY_SHOWDLG;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_MJ_NOTIFY_SHOWDLG);
    }
};

/**
 * @ingroup MJFrame
 * @brief �Զ���������Ϣ
 */	
struct TCMD_REQUEST_CHECK_WIN: public TCMD_HEADER 
{
	BYTE	byChair;			// ���˭��
	BYTE	byTurn;				// �ֵ�˭������ǵ��ڣ����ǵ��ڵ��Ǹ���
	BYTE	tHand[4][MAX_HAND_TILES];		// �ļ����ϵ���
	BYTE	byHandCount[4];		// �����м�����

	BYTE	tSet[4][MAX_SET_TILES][3];		// �ļң�4���ƣ�flag��tile��chair
	BYTE	bySetCount[4];		// set�м�����

	BYTE	tGive[4][40];		// �ļҳ�������
	BYTE	byGiveCount[4];		// ÿ�˳��˼�����

	BYTE	tLast;				// ���͵�������
	BYTE	tGodTile;			// ������
	BYTE	byFlag;				// 0������1���ڡ�2���ϻ���3����
	BYTE    ucDealer;			// ׯ��
	BYTE    ucDice[DICE_NBR];	// ɫ��
	BYTE	byRoundWind;		// Ȧ��
	BYTE	byPlayerWind;		// �ŷ�
	BYTE	byTilesLeft;		// ��ʣ�������ƣ��������㺣�׵�
	BYTE    byCollectWind;		// ����Ҳ���Գ�

	BYTE	byFlowerCount[4];	// 4�Ҹ��ж����Ż�	
	BYTE	byTing[4];			// ���Ƶ����
	BYTE	byOtherInfo[4];		// ������Ϣ
	BYTE    byOtherInfo2[4];	// ������Ϣ

    TCMD_REQUEST_CHECK_WIN()
    {
        memset(this, 0, sizeof(TCMD_REQUEST_CHECK_WIN));
		cCmdPara = CS_MJ_REQUEST_CHECK_WIN;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_REQUEST_CHECK_WIN);
    }
};


struct TCMD_NOTIFY_CHECK_WIN: public TCMD_HEADER 
{
	BYTE    byRes;			    // �жϵĽ��
	BYTE	byFlag;			    // ״̬�����ơ����ڡ�����
	BYTE	byFan[4][128];		// ������Щ��
	BYTE	byFanNumber[4][128];// �м��������粹������
	BYTE	byWhoWin[4];		// ˭�͵�(Ҫ֧��һ�ڶ���)
	BYTE	byWhoGun;			// ˭���ڵ�
	BYTE	tLast;			    // �͵���һ��
	int		nScore[4];
	int		nMoney[4];

	BYTE    tile[4][MAX_HAND_TILES];	// �ĸ�������ϵ���
	BYTE	tSet[4][MAX_SET_TILES][3];	// �ĸ���ҵ�����

	BYTE    byOtherData[4];		// ��������
	BYTE    byOtherData2[4];	// ��������
    TCMD_NOTIFY_CHECK_WIN()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_CHECK_WIN));
		cCmdPara = SC_MJ_NOTIFY_CHECK_WIN;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_CHECK_WIN);
    }
};

struct TCMD_REQUEST_GMCMD: public TCMD_HEADER
{
	wchar_t strGMCmd[128];   // C# char����Ϊ2���ֽ�(Unicode����), ����C++����������Ϊwchar_t
    TCMD_REQUEST_GMCMD()
    {
        memset(this, 0, sizeof(TCMD_REQUEST_GMCMD));
		cCmdPara = CS_MJ_REQUEST_GMCMD;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_REQUEST_GMCMD);
    }
};

struct TCMD_NOTIFY_GMCMD: public TCMD_HEADER
{
	BYTE ucChair;
	BYTE ucGMCmd;	// SO֪ͨ���ͻ��˵Ĳ�������
	char strGMResult[256];
    TCMD_NOTIFY_GMCMD()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_GMCMD));
		cCmdPara = SC_MJ_NOTIFY_GMCMD;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_GMCMD);
    }
};

struct TCMD_NOTIFY_RUN_NBR: public TCMD_HEADER
{
	int nRunNbr;		// ������
	int nRunWinNbr;		// ������
	int nRunNotileNbr;	// ������
	
    TCMD_NOTIFY_RUN_NBR()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_RUN_NBR));
		cCmdPara = SC_MJ_NOTIFY_RUN_NBR;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_RUN_NBR);
    }
};

// ǿ��ˢ�¿ͻ�������
struct TCMD_NOTIFY_RESET_DATA: public TCMD_HEADER
{
	BYTE ucDataIndex;
	int  nResetData;
    TCMD_NOTIFY_RESET_DATA()
    {
        memset(this, 0, sizeof(TCMD_NOTIFY_RESET_DATA));
		cCmdPara = SC_MJ_NOTIFY_RESET_DATA;
        cCmdID = MJFRAME_MOUDLE_MSG;
        nDataLen = sizeof(TCMD_NOTIFY_RESET_DATA);
    }
};

#pragma pack()

}

#endif

