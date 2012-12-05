#pragma once
#include "StdAfx.h"
#include "Animal.h"
#include "CardAnimal.h"

//�Ӷ��������������ýṹ��
struct AnimalStruct 
{
	CGameImage *pImage;
	int	nFrame;
};
//�Ӷ������󲥷����Խṹ��
struct AnimalShowAttriStruct
{
	int nTime;
	int nElapse;
	int xPos;
	int yPos;
};

//��϶�����(���֧��3�����,����չ��n��汾 090329a1 JJ)
class CCombineAnimal
{
public:
	CCombineAnimal(void);

	~CCombineAnimal(void);
public:
	LRESULT Create(IN AnimalStruct * pAniStruct, IN int count);				//����count���Ӷ���(�Ӷ�������, ����)

	LRESULT Begin(IN AnimalShowAttriStruct * pAniShowAttri, IN int *nBegin, IN int *nEnd, IN int count);	//��ʼ���Ŷ���(��������, ��ʼʱ��, ����ʱ��, ����)
		
	LRESULT BeginCard(IN int beginCardTime, IN int endCardTime, IN CPoint ptBegin, IN CPoint ptEnd, IN int zPos, BYTE card[], int count);	//�����ƶ�������,��ʼ����ʱ��,z��λ��, ������, ����Ŀ 

	void End();

	LRESULT DrawAnimal(IN CDC *pDC, IN int xPos, IN int yPos);				//������,����ָ������OnPaint������

	LRESULT OnTimer();														//����ָ������OnTime��,����ֵ���ڿ����Ƿ񶯻�����

	bool IsValid();															//�Ƿ��ǿ�ʹ�õĶ���

	bool IsOnShow();														//�Ƿ����ڲ���
private:
	void Init();															//��ʼ��,�ڲ�ʹ��

	bool CanShow();															//��ǰ���ж����Ƿ񶼿���
public:
	CCardAnimal	m_CardAnimal;	//�ƶ�������(������)
private:
	CAnimal m_childAnimal[3];	//3���Ӷ�������

	int	m_nCardBeginTime;		//�ƶ�����ʼʱ��

	int m_nCardEndTime;			//�ƶ�������ʱ��
	
	int m_nCardZpos;			//�ƶ���Z��λ��

	int m_nTime;				//��ʱ��(100ms�ı���,  = 3���Ӷ������������֡��*100ms)

	int m_nNowTime;				//�Ѿ����Ź���ʱ��(100ms�ı���)

	int m_nElapse[3];			//�Ӷ����Լ��ļ��ʱ��(100ms�ı���)

	int m_nChildOnUse;			//�˶����м����Ӷ�����������ʹ��

	int m_nBeginTime[3];		//�Ӷ�������ʼʱ��(100ms�ı���)

	int m_nEndTime[3];			//�Ӷ����������ʱ��(100ms�ı���)
};
