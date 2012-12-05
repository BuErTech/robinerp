#pragma once

#include "StdAfx.h"
#include "Resource.h"

//�˿˻�ɫ����
#define SH_FANG_KUAI	1
#define SH_MEI_HUA		2
#define SH_HONG_TAO		3
#define SH_HEI_TAO		4

//�˿���
class CShowHandCard : public CWnd
{
	//ö�ٶ���
public:
	enum XShowMode { SWX_MODE_LEFT, SWX_MODE_CENTER, SWX_MODE_RIGHT };
	enum YShowMode { SWY_MODE_TOP, SWY_MODE_CENTER, SWY_MODE_BOTTOM }; 

	//��������
public:
	int								m_iCardHight;					//�˿˸߶�
	int								m_iCardWidth;					//�˿˿��
	int								m_iCardHSpace;					//�˿˼��

	//��������
protected:
	bool							m_bShowFirst;					//��ʾ�˿�
	bool							m_bUseMouse;					//��Ӧ���
	BYTE							m_bCardCount;					//�˿���Ŀ
	CPoint							m_PtBasePoint;					//��׼����
	XShowMode						m_XShowMode;					//��ʾģʽ
	YShowMode						m_YShowMode;					//��ʾģʽ
	CByteArray						m_CardArray;					//�˿�����

	//��̬����
protected:
	static bool						m_bInit;						//��ʼ��־
	static HCURSOR					m_hCursor;						//�����
	static CAFCImage				m_CardPic;						//�˿˱���
	static CAFCImage				m_BackPic;						//�˿˱���

	//��������
public:
	//���캯��
	CShowHandCard();
	//��������
	virtual ~CShowHandCard();

	//���ܺ���
public:
	//��ȡ��С����
	bool GetCardSize(SIZE & Size);
	//�����Ƿ�ʹ�����
	void SetUseMouse(bool bUserMouse);
	//�����Ƿ���ʾ��һ���˿�
	void SetShowFirst(bool bShowFirst);
	//�����˿�
	void SortCard();
	//�����˿�
	bool SetCard(BYTE bCardList[], BYTE bCardCount);
	//�����˿�
	BYTE GetCard(BYTE bCardBuffer[], BYTE bBufferLength);
	//���û�׼�㺯��
	void SetBasePoint(POINT & BasePoint, XShowMode XShowMode, YShowMode YShowMode);

	//�ڲ�����
private:
	//��ȡ�˿���ֵ
	BYTE GetCardValoe(BYTE bCard);
	//��ȡ�˿˻�ɫ
	BYTE GetHuaKind(BYTE bCard) { return (bCard-1)/13; };
	//��ȡ�˿�λ��
	BYTE GetCardXPos(BYTE bCard) { return (bCard-1)%13; };
	//��ȡ�˿�λ��
	BYTE GetCardYPos(BYTE bCard) { return (bCard-1)/13; };
	//��������λ��
	void FixCardControl();
	//�Աȵ�ֻ�˿�
	bool CompareOnlyOne(BYTE bFirstCard, BYTE bLastCard);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�Ҽ���Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�Ҽ���Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//���ù��
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};
