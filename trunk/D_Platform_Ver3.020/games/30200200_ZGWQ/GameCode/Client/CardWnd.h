#pragma once

#include "StdAfx.h"
#include "Resource.h"

//���뷽ʽ
#define	SW_NO_MOVE			0						//���ƶ�
#define SW_MODE_LEFT		1						//�����
#define SW_MODE_CENTER		2						//����
#define SW_MODE_RIGHT		3						//�Ҷ���

//�˿�����
#define CARD_HIDE			95						//�˿˸߶�
#define CARD_WIDTH			70						//�˿˿��
#define CARD_SPACE			15						//�˿˼��

//�˿˿�����
class CCardWnd : public CWnd
{
	//��������
protected:
	int						m_ShowMode;				//��ʾģʽ
	int						m_iMaxCardCount;		//�������Ŀ
	int						m_iCardCount;			//��ǰ����Ŀ
	int						m_NowDownHitCard;		//��ǰ������
	BYTE					m_bShowCard;			//�Ƿ���ʾ
	BYTE					* m_piCardList;			//��������
	BOOL					* m_pbUp;				//�Ƿ���
	BYTE					m_bUseMouse;			//�Ƿ���Ӧ�����Ϣ
	CPoint					m_BasePoint;			//��׼λ�õ�
	static	HCURSOR			m_Cursor;				//���
	static 	CBitmap			m_CardBmp[3];			//�˿�ͼ
	static  BOOL			m_bInitList;			//�Ƿ������־

public:
	CWnd * pgg;

	//��������
public:
	//���캯��
	CCardWnd();
	//��������
	virtual ~CCardWnd();
	//��ȡ��С����
	BOOL GetWndSize(SIZE &Size);
	//���û�׼�㺯��
	BOOL SetBasePoint(POINT & BasePoint, int iShowMode);
	//��ʼ���˿�
	BOOL InitCard(int iCardCount);
	//��ȡ�˿���Ϣ
	int GetCard(BYTE iCard[],BOOL bUp[]);
	//�����Ƿ�ʹ�����
	BOOL SetUseMouse(BOOL bUserMouse=TRUE);
	//�����˿�
	BOOL SetCard(BYTE iCardList[], BOOL bUp[], int iCardCount);
	//��ȡ������˿�
	int	GetUpCard(BYTE iCard[]);
	//ɾ��������˿�
	int RemoveUpCard();
	//�����Ƿ���ʾ�˿�
	BOOL ShowCard(BOOL bShow);

protected:
	//��������
	int HitCardTest(CPoint & point);
	//�滭�˿˺���
	BOOL DrawCard(CDC * pDC, POINT &Point, BYTE iCard, int iStation);
	//��������λ��
	BOOL FixCardControl(BOOL bReDraw = FALSE);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

