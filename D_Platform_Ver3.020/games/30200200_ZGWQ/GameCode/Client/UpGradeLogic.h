#pragma once

#include "StdAfx.h"

//��������
#define	UG_HUA_MASK					0xF0			//1111 0000
#define	UG_VALUE_MASK				0x0F			//0000 1111

//�˿˻�ɫ
#define UG_FANG_KUAI				0x00			//����	0000 0000
#define UG_MEI_HUA					0x10			//÷��	0001 0000
#define UG_HONG_TAO					0x20			//����	0010 0000
#define UG_HEI_TAO					0x30			//����	0011 0000
#define UG_NT_CARD					0x40			//����	0100 0000
#define UG_ERROR_HUA				0xF0			//����  1111 0000

//�˿˳�������
#define UG_ERROR_KIND				0				//����
#define UG_ONLY_ONE					1				//����
#define UG_DOUBLE					2				//����
#define UG_THREE					3				//����
#define UG_DOUBLE_TRACKOR			4				//����������
#define UG_THREE_TARCKOR			5				//����������
#define	UG_SHOW_CARD				6				//˦��





///////////////////////////////////////////////////////////////

//�û��ǳ�//���18
#define XQ_UPPEOPLE_X				(40)
#define XQ_UPPEOPLE_Y				(185)
#define XQ_DOWNPEOPLE_X				(40)
#define XQ_DOWNPEOPLE_Y				(500)

//�û�ͷ��
#define XQ_UPPEOPLE_LOG_X			(25)
#define XQ_UPPEOPLE_LOG_Y			(67)
#define XQ_DOWNPEOPLE_LOG_X			(25)
#define XQ_DOWNPEOPLE_LOG_Y			(381)

#define XQ_UP_RANGZI_Y				(XQ_UPPEOPLE_Y+18)//����
#define XQ_UP_TIZI_Y				(XQ_UPPEOPLE_Y+18*2)//����
#define XQ_UP_MUSU_Y				(XQ_UPPEOPLE_Y+18*3)//Ŀ��
#define XQ_SET_TOP_JU_TIME_Y		(XQ_UPPEOPLE_Y+18*4)//��ʱ
#define XQ_SET_TOP_BU_TIME_Y		(XQ_UPPEOPLE_Y+18*5)//����ʱ

#define XQ_DOWN_RANGZI_Y			(XQ_DOWNPEOPLE_Y+18)//����
#define XQ_DOWN_TIZI_Y				(XQ_DOWNPEOPLE_Y+18*2)//����
#define XQ_DOWN_MUSU_Y				(XQ_DOWNPEOPLE_Y+18*3)//Ŀ��
#define XQ_SET_BOT_JU_TIME_Y		(XQ_DOWNPEOPLE_Y+18*4)//��ʱ
#define XQ_SET_BOT_BU_TIME_Y		(XQ_DOWNPEOPLE_Y+18*5)//����ʱ

//��ʾʱ����������
//�м���ʾ
#define XQ_INIT_SET_JU_TIME_X		(XQ_UPPEOPLE_X)
#define XQ_INIT_SET_JU_TIME_Y		(325)
#define XQ_INIT_SET_BU_TIME_X		(XQ_UPPEOPLE_X)
#define XQ_INIT_SET_BU_TIME_Y		(350)
#define XQ_INIT_SET_DU_TIME_X		(85)
#define XQ_INIT_SET_DU_TIME_Y		(418)

//�Ϸ���ʾ��ʱ
#define XQ_SET_TOP_JU_TIME_X		(XQ_UPPEOPLE_X)
#define XQ_SET_TOP_BU_TIME_X		(XQ_UPPEOPLE_X)

#define XQ_SET_TOP_DU_TIME_X		(XQ_UPPEOPLE_X)
#define XQ_SET_TOP_DU_TIME_Y		(341)

//�·���ʾ��ʱ
#define XQ_SET_BOT_JU_TIME_X		(XQ_DOWNPEOPLE_X)
#define XQ_SET_BOT_BU_TIME_X		(XQ_DOWNPEOPLE_X)

#define XQ_SET_BOT_DU_TIME_X		(XQ_DOWNPEOPLE_X)//
#define XQ_SET_BOT_DU_TIME_Y		((XQ_UPPEOPLE_Y+18))//



#define XQ_UP_FLAG_X				(75)//�����־
#define XQ_UP_FLAG_Y				(262)
#define XQ_DOWN_FLAG_X				(75)
#define XQ_DOWN_FLAG_Y				(580)

//////////////////////////////////////////////////wq//////////////
#define WQ_USE_TIME_LEFT_X				(70)//�����ʱ
#define WQ_USE_TIME_LEFT_Y				(392)
#define WQ_DAO_TIME_LEFT_X				(70)//��ߵ���ʱ
#define	WQ_DAO_TIME_LEFT_Y				(412)

#define WQ_USE_TIME_RIGHT_X				(901)//�ұ�
#define WQ_USE_TIME_RIGHT_Y				(390)
#define WQ_DAO_TIME_RIGHT_X				(901)
#define WQ_DAO_TIME_RIGHT_Y				(413)

#define WQ_BOARD_ORG_X					(150)//�������Ͻ��������ͻ����е�����
#define WQ_BOARD_ORG_Y					(29)//�������Ͻ��������ͻ����е�����
#define WQ_BOARD_X						(150)//��������������ͼ�е�����
#define WQ_BOARD_Y						(29)//��������������ͼ�е�����
#define WQ_RECT_W						(32)//������ÿ�����ӵĿ��
#define WQ_QIZI_R						(29)//����ֱ��

#define WQ_MAX_I						(21)//19 �м�2�߽�
#define WQ_MAX_J						(21)//19 �м�2�߽�

#define WQ_FLAG_NONE					0
#define WQ_FLAG_BLACK					1
#define WQ_FLAG_WHITE					2
#define WQ_FLAG_SIDE					3	//2�߽�
#define WQ_FLAG_CUR_BLACK				(WQ_FLAG_BLACK + 10)
#define WQ_FLAG_CUR_WHITE				(WQ_FLAG_WHITE + 10)

#define WQ_FLAG_BLACK_CROSS				4
#define WQ_FLAG_WHITE_CROSS				5


#define WQ_FLAG_HASCHECKED_QIZI			(-10)

#define WQ_BLACK_TIE_MU					(3.75)//����Ŀ3��3/4��


enum WQ_PLAY_FLAG
{
	WQ_PLAY_FLAG_BLACK = 0,
	WQ_PLAY_FLAG_WHITE = 1
};


//��������
struct tagPointNode
{
	int i;
	int j;

	tagPointNode()
	{
		i = -1;
		j = -1;
	}

	tagPointNode(tagPointNode &pn)
	{
		i = pn.i;
		j = pn.j;
	}

	tagPointNode(int ii, int jj)
	{
		i = ii;
		j = jj;
	}

};

/////////////////////////////////////////////////////

//Χ���߼��� 
class CUpGradeGameLogic
{
	//��������
private:

	int							m_iQiZi[21][21];	//����
	//���
	int							m_iDJCurPutI,
								m_iDJCurPutJ;

	int							m_iDJLastKilledI,
								m_iDJLastKilledJ;

	//��������
public:
	int								m_shizi[21][21];
	//���캯��
	CUpGradeGameLogic(void);
public:
	//�ܷ�������
	BOOL  CanAddQiZi(int curI, int curJ,int myFlag, int qi[][21]);
	//��Ŀ
	BOOL IsIllegalDianMu(int curI, int curJ, int myFlag, int qi[][21]);

	void NumQiZi(float *totalBlack, float *totalWhite, int qi[][21]);
	//�ڲ�����
private:
	
	int KillAllQiZi(int *killedI, int *killedJ, int myFlag, int qi[][21]);
	int AddNewQiZi(int pi, int pj, int iFlag, int qi[][21]);
	int GetEmptyFlag(int pi,int pj,int qi[][21]);
	//��ջ��
protected:
	class CStack
	{
	public:
		///CTList <tagPointNode *> m_list;
		CPtrList	m_list;

		bool bPop(int *piRet, int *pjRet);
		bool bGetTop(int *piRet, int *pjRet);

		void Push(int iIn, int jIn);
		bool bIsInQ(int iIn, int jIn);
		bool bIsEmpty();

		CStack & operator =(CStack &s);
		CStack();
		CStack(CStack &s);
		virtual ~CStack();
	};
};
