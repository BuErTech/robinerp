#pragma once

//#include "StdAfx.h"

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
#define UG_ERROR_KIND				    0				//����
#define SH_OTHER					    1				//ɢ��
#define SH_DOUBLE					    2				//����
#define SH_TWO_DOUBLE				    3				//����
#define SH_THREE					    4				//����
#define SH_SMALL_CONTINUE			    5				//��С˳��
#define SH_CONTINUE					    6				//˳��
#define SH_SAME_HUA					    7				//ͬ��
#define SH_HU_LU					    8				//��«
#define SH_TIE_ZHI					    9				//4��
#define SH_SMALL_SAME_HUA_CONTINUE	    11              //��Сͬ��˳
#define SH_SAME_HUA_CONTINUE		    10				//ͬ��˳
#define SH_REGIUS_SAME_HUA_CONTINUE		12				//�ʼ�ͬ��˳


//�����߼��� 
class CUpGradeGameLogic
{
	//��������
private:
	int						m_iStation[5];						//���λ�ã����飬÷�������ң����ң����ƣ�	
	int						SevenCard[4][7];					//���Ʒֻ�ɫ��������
	CPtrList				m_ptrPSList;//���б�	

	struct PSTypeNode
	{
		int mps,mcount;

		PSTypeNode()
		{
			mps = -1;
			mcount = 0;
		}
	};


	//��������
public:
	//���캯��
	CUpGradeGameLogic(void);

	//���ܺ���������������
public:

	//��ȡ�˿�����
	inline int GetCardNum(BYTE iCard) { return (iCard & UG_VALUE_MASK) + 1;}

	//��ȡ�˿˻�ɫ
	BYTE GetCardHuaKind(BYTE iCard);

	//�����˿�
	BOOL SortCard(BYTE iCardList[], BYTE bUp[], int iCardCount);

	//ɾ���˿�
	int RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount);

	//��������
	BOOL ReSortCard(BYTE iCardList[], int iCardCount);

	//��������
public:
	//�Ƿ���
	inline BOOL IsOnlyOne(BYTE iCardList[], int iCardCount) { return iCardCount==1; };

	//�Ƿ����
	BOOL IsDouble(BYTE iCardList[], int iCardCount);

	//�Ƿ�����
	BOOL IsThree(BYTE iCardList[], int iCardCount);

	//�õ����л�ɫ
	BYTE GetCardListHua(BYTE iCardList[], int iCardCount);

	//��ȡ����
	BYTE GetCardShape(BYTE iCardList[], int iCardCount);

	//��ȡ�˿���Դ�С
	int GetCardBulk(BYTE iCard, BOOL bExtVol);

	//�Աȵ���
	BOOL CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard);

	//�Ƿ�˳��
	BOOL IsSingleContinue(BYTE iCardList[], int iCardCount);

	//�Ƿ���С˳��
	BOOL IsSmallSingleContinue(BYTE iCardList[], int iCardCount);

	//�Ƿ�����
	BOOL IsCompleDouble(BYTE iCardList[], int iCardCount);

	//�Ƿ�ͬ��
	BOOL IsSameHua(BYTE iCardList[], int iCardCount);

	//�Ƿ��«
	BOOL IsHuLu(BYTE iCardList[], int iCardCount);

	//�Ƿ�4��
	BOOL IsTieZhi(BYTE iCardList[], int iCardCount);

	//�Ƿ�ͬ��˳
	BOOL IsSameHuaContinue(BYTE iCardList[], int iCardCount);

	//�Ƿ�ʼ�ͬ��˳
	BOOL IsRegiusContinue(BYTE iCardList[], int iCardCount);

	//�Ƿ�����Сͬ��˳
	BOOL IsSmallRegiusContinue(BYTE iCardList[], int iCardCount);

	//��������
public:
	//��ȡ�˿�
	BYTE GetCardFromHua(int iHuaKind, int iNum);

	bool m_bCompareHuaSe;

	//������ר�ú���
public:
	//�����˿�
	BYTE RandCard(BYTE iCard[], int iCardCount);

	//�ͻ��˺���
public:
	//�Զ����ƺ���
	BOOL AutoOutCard(BYTE iHandCard[], int iHandCardCount, BYTE iBaseCard[], int iBaseCardCount, BYTE iResultCard[], int & iResultCardCount, BOOL bFirstOut);

	//�Ƿ���Գ���
	BOOL CanOutCard(BYTE iOutCard[], int iOutCount, BYTE iBaseCard[], int iBaseCount, BYTE iHandCard[], int iHandCount);

	//�Ƚ������ƴ�С
	int CompareCard(BYTE iFirstCardList[], int iFirstCardCount, BYTE iSecondCardList[], int iSecondCardCount);

	//�ڲ�����
private:

	//��� 0 λ�˿�
	int RemoveNummCard(BYTE iCardList[], int iCardCount);

public:
	//����==iCard�ĵ�������iCardList�е����(��ʼλ��,���K�cλ��)
	int GetSerialBySpecifyCard(BYTE iCardList[], int iStart, int iCardCount,BYTE iCard);

    //��7�����з�����5���������
    int AnalysisCard(BYTE bHandCards[], int nHandCount, BYTE bPublicCards[], int nPublicCount, BYTE bResultCard[]);
};
