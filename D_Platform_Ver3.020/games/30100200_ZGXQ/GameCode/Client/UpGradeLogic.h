#pragma once

#include "StdAfx.h"

/// ��������
#define	UG_HUA_MASK					0xF0			///< 1111 0000
#define	UG_VALUE_MASK				0x0F			///< 0000 1111

/// �˿˻�ɫ
#define UG_FANG_KUAI				0x00			///< ����	0000 0000
#define UG_MEI_HUA					0x10			///< ÷��	0001 0000
#define UG_HONG_TAO					0x20			///< ����	0010 0000
#define UG_HEI_TAO					0x30			///< ����	0011 0000
#define UG_NT_CARD					0x40			///< ����	0100 0000
#define UG_ERROR_HUA				0xF0			///< ����   1111 0000

/// �˿˳�������
#define UG_ERROR_KIND				0				///< ����
#define UG_ONLY_ONE					1				///< ����
#define UG_DOUBLE					2				///< ����
#define UG_THREE					3				///< ����
#define UG_DOUBLE_TRACKOR			4				///< ����������
#define UG_THREE_TARCKOR			5				///< ����������
#define	UG_SHOW_CARD				6				///< ˦��

///////////////////////////////////////////////////////////////

enum XQ_PLAY_FLAG
{
	XQ_PLAY_FLAG_RED = 0,
	XQ_PLAY_FLAG_BLACK = 1
};


/// ����
#define XQ_FLAG_NONE				0
#define XQ_FLAG_RED					1
#define XQ_FLAG_BLACK				-1

#define XQ_QIZI_NONE				0
#define XQ_CROSS					8

/// �췽
#define XQ_RED_JU					1
#define XQ_RED_MA					2
#define XQ_RED_PAO					3
#define XQ_RED_JIANG				6
#define XQ_RED_SHI					4
#define XQ_RED_XIANG				5
#define XQ_RED_BIN					7

/// �ڷ�
#define XQ_BLACK_JU					-1
#define XQ_BLACK_MA					-2
#define XQ_BLACK_PAO				-3
#define XQ_BLACK_JIANG				-6
#define XQ_BLACK_SHI				-4
#define XQ_BLACK_XIANG				-5
#define XQ_BLACK_BIN				-7

#define XQ_MAX_I					(10)
#define XQ_MAX_J					(9)


/// ����
#define XQ_BOARD_ORG_X				(266)			///< 302)	//�������Ͻ��ڿͻ����е�����
#define XQ_BOARD_ORG_Y				(81)			///< 114)	//�������Ͻ��ڿͻ����е�����
#define XQ_BOARD_X					(248)			///< (223)	//�������Ͻ�����������ͼ������
#define XQ_BOARD_Y					(74)			///< (64)//(78)	//�������Ͻ�����������ͼ������
#define XQ_QIZI_RECT_A				(57)			///< ������ÿ�����ӵĿ��
#define XQ_QIZI_RECT_R				(50)			///< ����

/// ��ʾʱ����������
/// �м���ʾ
#define XQ_INIT_SET_JU_TIME_X		(85)			///< (63)
#define XQ_INIT_SET_JU_TIME_Y		(327)			///< (337)
#define XQ_INIT_SET_BU_TIME_X		(85)			///< (63)
#define XQ_INIT_SET_BU_TIME_Y		(349)			///< (359)
#define XQ_INIT_SET_DU_TIME_X		(85)			///< (63)
#define XQ_INIT_SET_DU_TIME_Y		(371)			///< (381)

/// �Ϸ���ʾ��ʱ
#define XQ_SET_TOP_JU_TIME_X		(85)			///< (63)
#define XQ_SET_TOP_JU_TIME_Y		(248)			///< (258)
#define XQ_SET_TOP_BU_TIME_X		(85)			///< (63)
#define XQ_SET_TOP_BU_TIME_Y		(267)			///< (277)
#define XQ_SET_TOP_DU_TIME_X		(85)			///< (63)
#define XQ_SET_TOP_DU_TIME_Y		(285)			///< (295)

/// �·���ʾ��ʱ
#define XQ_SET_BOT_JU_TIME_X		(85)			///< (63)
#define XQ_SET_BOT_JU_TIME_Y		(438)			///< (448)
#define XQ_SET_BOT_BU_TIME_X		(85)			///< (63)
#define XQ_SET_BOT_BU_TIME_Y		(455)			///< (465)
#define XQ_SET_BOT_DU_TIME_X		(85)			///< (63)
#define XQ_SET_BOT_DU_TIME_Y		(471)			///< (481)

/// �û��ǳ�
#define XQ_UPPEOPLE_X				(85)			///< (63)
#define XQ_UPPEOPLE_Y				(228)			///< (238)
#define XQ_DOWNPEOPLE_X				(85)			///< (63)
#define XQ_DOWNPEOPLE_Y				(420)			///< (430)

/// �û�ͷ��
#define XQ_UPPEOPLE_LOG_X			(47)			///< (27)
#define XQ_UPPEOPLE_LOG_Y			(96)
#define XQ_UPPEOPLE_VIDEO_X			(10)
#define XQ_UPPEOPLE_VIDEO_Y			(76)

#define XQ_DOWNPEOPLE_LOG_X			(47)			///< (27)
#define XQ_DOWNPEOPLE_LOG_Y			(522)
#define XQ_DOWNPEOPLE_VIDEO_X		(47)
#define XQ_DOWNPEOPLE_VIDEO_Y		(502)
/////////////////////////////////////////////////////



/// �����߼��� ��֧�� 2 ���� 3 ���˿ˣ�
class CUpGradeGameLogic
{
	/// ��������
private:
	int							m_iQiZi[10][9];		///< ����
	int							m_iSideFlag;		///< ����,���ǵз�
	/// ��������
public:
	/// ���캯��
	CUpGradeGameLogic(void);

public:
	/// ����������ʲô��
	int  GetChess(int xChess,int yChess,int chessArray[][9]);	
	/// �Ƿ���Գ���
	// BOOL CanOutCard(BYTE iOutCard[], int iOutCount, BYTE iBaseCard[], int iBaseCount, BYTE iHandCard[], int iHandCount);
	/// ��ʼ������
	void InitBoard(int iMeFlag, int qi[][9]);
	/// ����,���ǵз�����
	int IsMeSideBin(){return m_iSideFlag;}
	/// �����Ƿ�Ϊ����
	int SetMeSide(int iSide){m_iSideFlag=iSide;}
	/// ��ȡĳһ����������ֵ
	int GetQiZiValue(int fx, int fy);
	/// �ܷ�����(��ֻ����,��������ʵʩ)
	BOOL CanMoveQiZi(int fromi, int fromj, int toi, int toj, int qi[][9],bool check);
	/// ����(ǰ��ֻ�ǲ����Ƿ������)
	BOOL MoveQiZi(int fromi, int fromj, int toi, int toj, int qi[][9]);
	/// ��������(���ڻ���)
	BOOL SetQiZi(int fx,int fy,int value);
	/// �Ƿ񔳷�����������܊
	int CheckOtherOrMeJiang(int Startx, int Starty,int Endx,int Endy,int meflag,int MeBoard[][9]);
	/// �Ƿ񱾷�����
	int CheckMeJiangJun(int Startx, int Starty,int Endx,int Endy,int meflag,int MeBoard[][9]);
	/// �Ƿ�з�����
	int CheckOtherJiangJun(int Startx, int Starty,int Endx,int Endy,int meflag,int MeBoard[][9],BOOL bTest=false);
	/// ��������λ��
	CPoint GetQiZiKing(int meflag,int meboard[][9]);
	/// ����ĳ��������
	int GetMeQiZi(CPoint point[],int meflag,int MeBoard[][9]);
	/// ȷ���Ƿ��������
	BOOL HaveQiZiMove(int meflag,int meboard[][9]);
	/// ȷ���Ƿ�ʤ��
	BOOL IsLose(int iMeFlag, int qi[][9]);
	/// ȷ���Ƿ��л��
	BOOL IsLive(int meflag,int MeBoard[][9]);
	/// �ܹ��Ե��Է���
	BOOL CanEatKing(int meflag,int MeBoard[][9]);
	/// ����
	BOOL IsDieCheck(int meflag,int MeBoard[][9]);	
	void SetBoard(int iMeFlag, int qi[][9]);

	/// �ڲ�����
private:
	
	/// �߳�
	BOOL MoveZhu(int fx, int fy, int tx, int ty);
	/// ����
	BOOL MoveMa(int fx, int fy, int tx, int ty);
	/// ����
	BOOL MovePao(int fx, int fy, int tx, int ty, int flag);
	/// ����
	BOOL MoveXiang(int fx, int fy, int tx, int ty);
	/// ��ʿ
	BOOL MoveShi(int fx, int fy, int tx, int ty);
	/// �߽�
	BOOL MoveShuai(int fx, int fy, int tx, int ty);
	/// �߱�
	BOOL MoveBin(int fx, int fy, int tx, int ty);
};
