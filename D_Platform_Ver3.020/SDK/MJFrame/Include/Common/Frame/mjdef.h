/** @file	mjdef.h
 *	@brief	�齫�ṹ�Ļ�������
 *	@author dxh
 *	@date 2010.10.08
 */
#ifndef __MJDEF_H___
#define __MJDEF_H___

#include "..\typeDef.h"

namespace MyGame
{

typedef unsigned char TILE;

#define TILE_BEGIN		0		// ����
#define TILE_CHAR_1		1		// ��
#define TILE_CHAR_2		2
#define TILE_CHAR_3		3
#define TILE_CHAR_4		4
#define TILE_CHAR_5		5
#define TILE_CHAR_6		6
#define TILE_CHAR_7		7
#define TILE_CHAR_8		8
#define TILE_CHAR_9		9
#define TILE_BAMBOO_1	11		// ��
#define TILE_BAMBOO_2	12
#define TILE_BAMBOO_3	13
#define TILE_BAMBOO_4	14
#define TILE_BAMBOO_5	15
#define TILE_BAMBOO_6	16
#define TILE_BAMBOO_7	17
#define TILE_BAMBOO_8	18
#define TILE_BAMBOO_9	19
#define TILE_BALL_1		21		// Ͳ
#define TILE_BALL_2		22
#define TILE_BALL_3		23
#define TILE_BALL_4		24
#define TILE_BALL_5		25
#define TILE_BALL_6		26
#define TILE_BALL_7		27
#define TILE_BALL_8		28
#define TILE_BALL_9		29
#define TILE_EAST		31		// ��
#define TILE_SOUTH		32
#define TILE_WEST		33
#define TILE_NORTH		34
#define TILE_ZHONG		35
#define TILE_FA			36
#define TILE_BAI		37
#define TILE_FLOWER_CHUN	41	// ��	// ��
#define TILE_FLOWER_XIA		42			// ��
#define TILE_FLOWER_QIU		43			// ��
#define TILE_FLOWER_DONG	44			// ��
#define TILE_FLOWER_MEI		45			// ÷
#define TILE_FLOWER_LAN		46			// ��
#define TILE_FLOWER_ZHU		47			// ��
#define TILE_FLOWER_JU		48			// ��

#define TILE_END			49	// ����

#define TILE_ALL_NBR		34

#define TILE_GENRE_CHAR		0	// ���ͣ���
#define TILE_GENRE_BAMBOO	1	// ��
#define TILE_GENRE_BALL		2	// Ͳ
#define TILE_GENRE_WIND		3	// ��
#define TILE_GENRE_FLOWER	4	// ��
#define TILE_GENRE_OTHER	5	// û��

#define MAX_TOTAL_TILES		144	// �ܵ�������
#define MAX_HAND_TILES		18	// ���Ƶ�����
#define MAX_GIVE_TILES		36	// ���Ƶ�����
#define MAX_SET_TILES		8	// �������Ƶ�����
#define MAX_INCREASE_TILES	9	// ���������

//��λ����

//////////////////////////////////////////////////////////////////////////
//  ��Ϸ��������λ��λ�ķֲ� (4��)		2��	
// 				 2						 1	
// 	             |						 |
// 				 |						 |
// 		  3 --�������� 1				 |
// 				 |                       |
// 				 |                       |      
// 				 0	                     0
// ����Ϸ���߼���˳��ȴ��˳ʱ���
//////////////////////////////////////////////////////////////////////////
#define SEAT_SELF			0	// �Լ�
#define SEAT_FOLLOW			1	// �¼�
#define SEAT_OPPOSITE		2	// �Լ�
#define SEAT_AHEAD			3	// �ϼ�

//����ԡ�������
#define ACTION_EMPTY				0x00	// û����Ϊ
#define ACTION_COLLECT				0x10	// ����
#define ACTION_TRIPLET				0x11	// ��
#define ACTION_QUADRUPLET_PATCH		0x12	// ����
#define ACTION_QUADRUPLET_CONCEALED	0x13	// ����
#define ACTION_QUADRUPLET_REVEALED	0x14	// ����
#define ACTION_QUADRUPLET_DNXB      0x15    // ����������
#define ACTION_QUADRUPLET_ZFB       0x16    // �з��׸�
#define ACTION_WIN					0x17	// ����
#define ACTION_TING					0x18	// ����
#define ACTION_FLOWER				0x19	// ����
#define ACTION_GIVE					0x1A	// Ҫ�����ƣ����ұߵ�һ�ţ�


// ���ƵĽ��λ�ñ��
#define COLLECT_RES_OOX	1
#define COLLECT_RES_OXO	10
#define COLLECT_RES_XOO	100

/**
 * @ingroup MJFrame
 * @brief �齫�ؼ�֧�ֵ����ݽṹ
 */	
struct TILES_INFO
{
	TILE	tile;		// ����
	BOOL	bSelect;	// �Ƿ񱻵�����Ƿ���
	BYTE	ucFlag;		// ��ǣ���¼�����ܵ����ã�
	BYTE	ucWitch;	// �Ե���һ��	(0, 1, 2)
	BYTE	ucChair;	// ������˭��
    BYTE    ucType;     // ���ֺ������ͣ�0��������1�����ڣ� 2������
	UINT	dwReserve1;	// Ԥ��BUFFλ	1λ���ڳ��Ƶ�����£�0������ѡ���ƣ�1������ѡ����
						//              2λ��������ͼƬ��־��0������ʾͼƬ��1����ʾͼƬ
						//				3λ���ж��Ƿ񲥷Ű������嶯����0�����ţ�1��������
						//              4λ������ֵĿ�ͷ��0����������ܣ�2������ܵĿ�ͷһ��
	UINT	dwReserve2;	// ����ʱ, ���Ʋ��뵽���������������е�λ��

	TILES_INFO()
    {
        memset(this, 0, sizeof(TILES_INFO));
    }
};

/**
* @ingroup MJFrame
* @brief �����ػ�, �齫�����������ݻָ�
*/	
struct TILES_GIVE
{
    TILE	tile;	   // ��ֵ
    BOOL	bCPG;	   // �Ƿ񱻳�����
    BOOL    bTing;     // �Ƿ�Ϊ���ƺ�����������
    
    TILES_GIVE()
    {
        memset(this, 0, sizeof(TILES_GIVE));
    }
};

}
#endif

