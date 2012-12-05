#pragma once
#include "StdAfx.h"

//���Ŷ�����(������Ͷ���)(090329a1 JJ)
class CAnimal
{
public:
	CAnimal(void);

	~CAnimal(void);
//����
private:
	CPoint	m_ptBasepoint;			//����λ��

	int		m_nFrame;				//��֡��

	int		m_nNowFrame;			//��ǰ���ڲ��ŵ�֡

	int		m_nTime;				//��ʱ��(ms)

	int		m_nLeftTime;			//ʣ��ʱ��(ms)

	int		m_nElapse;				//ʱ����

	CGameImage * m_pImage;			//ʹ�õ�ͼ��
//����
public:
	LRESULT Create(IN CGameImage *pImage, IN int nFrame);					//���ö��� (����ͼƬָ�� ͼƬ֡��)

	LRESULT Begin(IN int nTime, IN int nElapse, IN int xPos, IN int yPos);	//��ʼ���Ŷ���(���Ŷ��(ms)  ����λ��x,y����)
	
	void End();																//�������Ŷ���

	LRESULT DrawAnimal(IN CDC *pDC, IN int iMidx, IN int iMidy);			//������,����ָ������OnPaint������

	LRESULT OnTimer();														//����ָ������OnTime��,����ֵ���ڿ����Ƿ񶯻�����

	bool IsValid();															//�Ƿ��ǿ�ʹ�õĶ���

	bool IsOnShow();														//�Ƿ����ڲ���

	bool CanShow();															//��ǰ�����Ƿ����
protected:
	void Init();															//��ʼ��,�ڲ�ʹ��
};
