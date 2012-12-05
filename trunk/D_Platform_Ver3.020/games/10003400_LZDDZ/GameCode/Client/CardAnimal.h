#pragma once
#include "StdAfx.h"

#define CARD_WIDTH	80		//�ƿ�
#define CARD_HEIGHT	105		//�Ƹ�
#define CARD_SPACE	15		//�Ƽ��

//�ƶ�����(090330a1 JJ)
class CCardAnimal 
{
public:
	CCardAnimal(void);

	~CCardAnimal(void);
private:
	CPoint	m_ptBegin;						//���λ��

	CPoint	m_ptEnd;						//�յ�λ��

	BYTE	m_nCard[10];					//������,���֧��10λ

	int		m_nCardCount;					//��ʱ�ж�������

	int		m_nTime;						//��ʱ��(ms)

	int		m_nNowTime;					//ʣ��ʱ��(ms)

	int		m_nElapse;						//ʱ����

	CGameImage	*m_pImgNormalCard;			//��ͨ��42����

	CGameImage	*m_pImgBackCard;			//�Ʊ��ʹ�С��
public:
	//����
	LRESULT Create(IN CGameImage * pNormalCard, IN CGameImage * pBackCard);	//���ö��� (����ͼƬָ��)

	LRESULT Begin(IN int nTime, IN int nElapse, IN CPoint ptBegin, IN CPoint ptEnd, IN BYTE card[], IN int count);	//��ʼ���Ŷ���(���Ŷ��(ms) ʱ���� ����λ����ʼ�ͽ�������) �� ����

	void End();																//�������Ŷ���

	LRESULT DrawAnimal(IN CDC *pDC, IN int xPos, IN int yPos);				//������,����ָ������OnPaint������

	LRESULT OnTimer();														//����ָ������OnTime��,����ֵ���ڿ����Ƿ񶯻�����

	bool IsValid();															//�Ƿ��ǿ�ʹ�õĶ���

	bool IsOnShow();														//�Ƿ����ڲ���

	bool CanShow();															//��ǰ�����Ƿ����
	
	static BYTE GetCardXPos(BYTE bCard)										//��ȡ������ͼ��Xλ��
	{ 
		BYTE bkk=(bCard&0x0F)+1;
		if (bkk==14) bkk=1;
		return bkk-1; 
	}

	static BYTE GetCardYPos(BYTE bCard) { return (bCard&0xF0)>>4; }			//��ȡ������ͼ��Yλ��
protected:
	void Init();															//��ʼ��,�ڲ�ʹ��
};
