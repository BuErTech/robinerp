#pragma once

#include "StdAfx.h"

//�������ֿռ�
//using namespace AFC::AFCImage;

//��Ϣ����
#define	IDM_LEFT_HITCARD			WM_USER+110						//����˿�
#define	IDM_RIGHT_HITCARD			WM_USER+111						//�һ��˿�
#define IDM_MOUSE_MOVE				WM_USER+112						//����ƶ�������
#define	IDM_MOUSE_LEFT				WM_USER+113						//����ƿ�

//��������
#define	UG_HUA_MASK					0xF0							//��ɫ����
#define	UG_VALUE_MASK				0x0F							//��������

//����
#define IMAGE_1024_768				0x00							//��
#define IMAGE_800_600				0x01							//��
#define IMAGE_SMALL					0x02							//��
#define IMAGE_VERY_SMALL			0x03							//��

//�˿˿�����
class CUpGradeGameCard : public CWnd
{
	//ö�ٶ���
public:
	enum XShowMode { SWX_MODE_LEFT, SWX_MODE_CENTER, SWX_MODE_RIGHT };
	enum YShowMode { SWY_MODE_TOP,  SWY_MODE_CENTER, SWY_MODE_BOTTOM }; 

	//��������
protected:
	int								m_iCardHight;					//�˿˸߶�
	int								m_iCardWidth;					//�˿˿��
	int								m_iCardHSpace;					//�˿˼��
	int								m_iCardVSpace;					//�˿˼��
	int								m_iCardUpSpace;					//����߶�
//	CRgn							m_CardViewRgn;					//�˿�����
	CWnd							* m_pHandleWnd;					//������

	//��Ϣ����
protected:
	bool							m_bShowCard;					//��ʾ�˿�
	bool							m_bHorzLook;					//�Ƿ���
	bool							m_bUseMouse;					//��Ӧ���
	BYTE							m_bHotHitCard;					//�����˿�
	BYTE							m_bCardCount;					//�˿���Ŀ
	CPoint							m_PtBasePoint;					//��׼����
	XShowMode						m_XShowMode;					//��ʾģʽ
	YShowMode						m_YShowMode;					//��ʾģʽ
	CByteArray						m_CardArray;					//�˿�����
	CByteArray						m_CardUpArray;					//�˿�����
	BYTE							m_bCardStyle;					//ʹ�����Ƿ�ΪС��true��ʾʹ��С��,false��ʾʹ��Ĭ����
	BOOL							m_bSelect;						//�Ƿ���ĳ������
	CByteArray						m_CardFriendArray;				//�˿�����(����������ֶ�����ʾ)
	BYTE							m_bFriendCardCount;				//��������
	BYTE							m_bShowNum;	
		//ͼƬ����
	CGameImage                      m_NowThis;                             //���ڶ�����
protected:
//	CAFCImage						m_CardPic[2];					//�˿˿ؼ�
	static		CGameImage				m_CardPic[4];//[4];					//�˿˿ؼ�
	CToolTipCtrl					m_ToolTip;						//������ʾ

	//colin09041���ƶ���
	CPoint                     m_ptStart;    // �ƶ��Ŀ�ʼ����
	CPoint                     m_ptEnd;      // �ƶ�Ŀ������
	long                       m_lMoveCount;   // �ƶ�����
	//��������
public:
	//���캯��
	CUpGradeGameCard(BYTE bCardStyle=0);
	//��������
	virtual ~CUpGradeGameCard();

	//���ú���
public:
	//��ʼ������
	bool InitGameCard(CWnd * pHandleWnd);
	//�����˿˲���
	void SetCardParameter(int iHSpace, int iVSpace, int iUpSpace);
	//�����ƿ�͸�
	void SetCardWidthHeight(int iWidth,int iHeight);
	//���û�׼�㺯��
	void SetBasePoint(CPoint & BasePoint, XShowMode XShowMode, YShowMode YShowMode);
	//������ʾ�˿�
	void SetShowCard(bool bShow);
	//����ʹ�����
	void SetUseMouse(bool bUserMouse=false);
	//������ʾ��ʽ
	void SetLookMode(bool bHorzLook);
	//���õ�����ģʽ
	void SetCardShowNum(BYTE bShowNum = 0){m_bShowNum = bShowNum; Invalidate(FALSE);}
	//���õ�����ģʽ
	void SetCardStyle(BYTE bCardStyle=0){m_bCardStyle=bCardStyle;}
	//������ģʽ
	BYTE GetCardStyle(){return m_bCardStyle;}
	//���ܺ���
public:
	//�����˿�
	bool SetCard(BYTE bCardList[], BYTE bUpList[], BYTE bCardCount);
	//ɾ��������˿�
	BYTE RemoveUpCard();
	//��ȡ������˿�
	BYTE GetUpCard(BYTE bCardList[]);
	//��ȡ�˿���Ϣ
	BYTE GetCard(BYTE bCardList[], BYTE bUpList[]);

	//��������
public:
	//��ȡ�˿���Ŀ
	BYTE GetCardCount() { return m_bCardCount; }
	//��ȡ�˿���ֵ
	BYTE GetCardValue(BYTE bCard) 
	{ 
		//return (bCard&UG_VALUE_MASK);
		BYTE bkk=(bCard&UG_VALUE_MASK)+1;
		if (bkk==14) bkk=1;
		return bkk; 
	}
	//��ȡ�˿˻�ɫ
	BYTE GetHuaKind(BYTE bCard) { return bCard&UG_HUA_MASK; }
	//��ȡ�˿˴�С
	BYTE GetCardNum(BYTE bCard) { return bCard&UG_VALUE_MASK; }	
	//��ȡ�˿�λ��
	BYTE GetCardXPos(BYTE bCard) { return GetCardValue(bCard)-1; };
	//��ȡ�˿�λ��
	BYTE GetCardYPos(BYTE bCard) { return GetHuaKind(bCard)>>4; };

	//�ڲ�����
protected:
	//��������
	BYTE HitCardTest(CPoint & Point);
	//��������λ��
	void FixCardControl(bool bInvalide=true);
	//��ȡ��С����
	bool GetCardSize(CSize & Size);

	//��Ϣ����
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�ػ�����
	afx_msg void OnPaint();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags,CPoint Point);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	//������������
	BOOL SetFriendArray(BYTE bCardList[], BYTE bUpList[], BYTE bCardCount);

		// ��ȡ��ǰλ��
	void GetBasePoint(CPoint &ptBasePoint);

	/****************************************************************
	* ������: void MoveAnimal(CPoint &ptStart, CPoint &ptEnd, long lTime)
	* ����: ��ʼ�ƶ����룬�����ƶ���ʼ���յ㡢��ʱ��
	* ����: CPoint &ptStart    [in]  ��ʼλ��
	*       CPoint &ptEnd      [in]  �յ�
	*       long lTime         [in]  �ƶ���ʱ�� (ms)
	* ����: N/A
	****************************************************************/
	void MoveAnimal(CPoint &ptStart, CPoint &ptEnd, long lTime);

	// ֹͣ�ƶ�
	void StopMove(void);
protected:
	//��������������ĳһ���Ƶĸ���
	BYTE FindACardNumInFriendList(BYTE iCardNum);
	//����ͼƬ
	void SetCardImage();
	DECLARE_MESSAGE_MAP()
};

