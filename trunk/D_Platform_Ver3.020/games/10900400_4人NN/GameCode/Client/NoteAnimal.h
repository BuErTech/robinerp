#pragma once

#include "StdAfx.h"

//�������ֿռ�
//using namespace AFC::AFCImage;

//�˿˿�����
class CNoteAnimal : public CWnd
{
	//ö�ٶ���
public:
CPoint							m_PtBasePoint;					//��׼����
	//��������
protected:
	CRgn							m_CardViewRgn;					//�˿�����
	CWnd							* m_pHandleWnd;					//������

	int								m_iTotalJeton;					//��ֵ
	int								m_iArrayJeton[16];		//�����������

	BYTE							m_bJetonCount;					//�����������ó��������Ŀ
	//��������ϵ
	int								m_iWidth;				//��
	int								m_iHeight;				//��
	int								m_iXoffset;				//x��ƫ����
	int								m_iYoffset;				//y��ƫ����
	
	//�����ص���ʽ
	BOOL							m_bOverlap;				//Ĭ��Ϊ�ټ�ȫ����ʾ
	//��Ϣ����
protected:
	bool							m_bHorzLook;					//�Ƿ���
	
	BOOL							m_bCardStyle;					//ʹ�����Ƿ�ΪС��true��ʾʹ��С��,false��ʾʹ��Ĭ����
	
	//ͼƬ����
protected:
	CGameImage						m_CardPic;					//�˿˿ؼ�
	CGameImageHelper*				m_pMoveCard;					//���ƶ�

	BOOL							m_bSelect;						//�Ƿ�ѡ����
	//��������
public:
	//���캯��
	CNoteAnimal(BOOL bOverlap=false);//�Ƿ����һ��
	//��������
	virtual ~CNoteAnimal();

	//���ú���
public:
	//��ʼ������
	bool InitGameCard(CWnd * pHandleWnd);
	//�����˿˲���
	void SetCardParameter(int iWidth,int iHeight,int iXoffset,int iYoffset);
	//���û�׼�㺯��
	void SetBasePoint(CPoint & BasePoint);
	//���ó�����ֵ
	void SetTotalJeton(int iTotal);
	//��ȡ������ֵ
	int  GetJeton(){return m_iTotalJeton;}
	//�����û�����ע���
	void AddJeton(int iTotal);
	//��������
	int AnalyseJeton(int iTotalJeton,int iArrayJeton[],BYTE iJetonCount,BYTE bJeton[]);
	//������ʾ��ʽ
	void SetLookMode(bool bHorzLook=true);
	//��ȡ��ʾ��ʽ
	BOOL GetLookMode();
	//�����ص���ʽ
	void SetOverlap(BOOL bOverlap){m_bOverlap=bOverlap;}
	//��ȡ�ص���ʽ
	BOOL GetOverlap(){return m_bOverlap;}
public:
	//��������
public:
	//��ȡ�˿���Ŀ
	BYTE GetCardCount() { return m_bJetonCount; }
	//��ȡ�˿�λ��
	BYTE GetCardXPos(BYTE bCard) { return bCard; };
	//��ȡ�˿�λ��
	BYTE GetCardYPos(BYTE bCard) { return bCard; };

	//�ڲ�����
protected:
	//��������λ��
	void FixCardControl();
	//��ȡ��С����
	bool GetCardSize(CSize & Size);

	//��Ϣ����
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�ػ�����
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
};

