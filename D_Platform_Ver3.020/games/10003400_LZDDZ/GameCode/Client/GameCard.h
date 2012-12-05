#pragma once

#include "StdAfx.h"

//�������ֿռ�
//using namespace AFC::AFCImage;

//��Ϣ����
#define	IDM_LEFT_HITCARD			WM_USER+110						//����˿�
#define	IDM_RIGHT_HITCARD			WM_USER+111						//�һ��˿�
//#define IDM_MOUSE_OUT				WM_USER+112
#define IDM_MOUSE_UP				WM_USER+113

//��������
#define	UG_HUA_MASK					0xF0							//��ɫ����
#define	UG_VALUE_MASK				0x0F							//��������

//����
#define IMAGE_1024_768				0x00							//��
#define IMAGE_800_600				0x00							//��
#define IMAGE_SMALL					0x01							//��
#define IMAGE_VERY_SMALL			0x03							//��

//�˿˿�����
class CUpGradeGameCard : public CWnd
{
	//ö�ٶ���
public:
	enum XShowMode { SWX_MODE_LEFT, SWX_MODE_CENTER, SWX_MODE_RIGHT };
	enum YShowMode { SWY_MODE_TOP,  SWY_MODE_CENTER, SWY_MODE_BOTTOM }; 
	//CByteArray						m_MoveSelectCardArray;					//�ƶ�ѡ���˿�����


	//��������
protected:
	int								m_iCardHight;					//�˿˸߶�
	int								m_iCardWidth;					//�˿˿��
	int								m_iCardHSpace;					//�˿˼��
	int								m_iCardVSpace;					//�˿˼��
	int								m_iCardUpSpace;					//����߶�
	CRgn							m_CardViewRgn;					//�˿�����
	CWnd							* m_pHandleWnd;					//������

//{{ qy �ƿؼ��޸�20091109
	void DisplayInfo();  // ���ƿ�ѡ
	CRect m_RectDis;     // ��ѡ��Χ
	CPoint m_stPoint;    // ��ʼ����
	BOOL m_bSelectOK;    // �Ƿ�ѡ��״̬
//}} qy �ƿؼ��޸�20091109

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
	BYTE							m_bShowNum;						//�Ƿ���ʾ����	
	BYTE							m_bCardStyle;					//ʹ�����Ƿ�ΪС��true��ʾʹ��С��,false��ʾʹ��Ĭ����
	BOOL							m_bSelect;						//�Ƿ���ĳ������
	CByteArray						m_CardFriendArray;				//�˿�����(����������ֶ�����ʾ)
	BYTE							m_bFriendCardCount;				//��������
//	BOOL							m_bLButtonDown;					//���������
	BYTE							m_iFirstCard;					//
	CPoint							m_ptLeftHit;					//��������µ�����
	CPoint							m_ptLeftHitMove;				//��������ƶ�����������
	CPoint							m_ptMovePoint;					//�ƶ���굱ǰ������
	BOOL							m_bCanMove;						//�Ƿ�Ϊ�ƶ�
	BYTE							m_bMoveFirstCard;				//�ƶ��ĵ�һ���Ƽ�������������
	bool							m_bMoveSel;					
	CPoint                          m_pMovePos;                     //�������λ��
	BYTE                            m_bCanUp;                       //�Ƿ����𱸷�
	bool							m_bMove;
	int								m_CurCardIndex;					// ��굱ǰ����card index
	BYTE							m_StartIndex;					// ��ʼ�ƶ���λ��
	BYTE							m_EndIndex;						// ��ʼ�ƶ���λ��
	CPoint							m_pStartPos;					// ��ʼ���λ��
	bool							m_bMultiSel;
	CPoint							m_pEndPos;						// ����λ��
	///�����ƴ�����
	BYTE                            m_ReplaceCardList[4];  
	///���Ӵ�������
	BYTE                        	m_iReplaceCount;

	//�������÷���
	int								m_iSourcePoint;						//ԭ������
	int								m_iBackPoint;						//�����̵׷���
	int								m_iBackMultiple;					//���õױ���
	//ͼƬ����
protected:
//	CAFCImage						m_CardPic[2];					//�˿˿ؼ�
	static		CGameImage			m_CardPic[7];//[4];					//�˿˿ؼ�
	CGameImage						m_Num;								//����
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
	void SetCardStyle(BYTE bCardStyle=0){m_bCardStyle=bCardStyle;}
	//������ģʽ
	BYTE GetCardStyle(){return m_bCardStyle;}
	//���õ�����ģʽ
	void SetCardShowNum(BYTE bShowNum = 0){m_bShowNum = bShowNum;}
	//������ģʽ
	BYTE GetCardShowNum(){return m_bShowNum;}
	///ĳ�����Ƿ������ƶ�����
	BOOL IsInside(BYTE bCardList[], int iCount, BYTE bCard, BYTE TempCard[], int TempCount);
	//���ܺ���
public:
	//�����˿�
	bool SetCard(BYTE bCardList[], BYTE bUpList[], BYTE bCardCount, BYTE ReplaceCardList[]=NULL, BYTE ReplaceCount=0);
	//ɾ��������˿�
	BYTE RemoveUpCard();
	//��ȡ������˿�
	BYTE GetUpCard(BYTE bCardList[]);
	//��ȡ�˿���Ϣ
	BYTE GetCard(BYTE bCardList[], BYTE bUpList[]);
	//��������
	int GetPoint();
	//�������÷���
	BOOL SetBackPointAndMultiple(int iSourcePoint = 0,int iBackPoint = 0,int iBackMultiple = 1);
	//��������

	//lym1111ʹ�����������
	void DownCard();

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
	//��������
	BYTE HitCardTestAbove(CPoint &Point);
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
	
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	//������������
	BOOL SetFriendArray(BYTE bCardList[], BYTE bUpList[], BYTE bCardCount);

	//{{ qy �ƿؼ��޸�20091109
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int GetCardsIndex(CRect &rect);	               // ��ȡ�������
	void GetCardRect(BYTE byIndex, CRgn &rgnCard); // ��ȡ�����Ƶ�����
	CByteArray  m_aSelIndex;                       // ѡ���Ƶ���������
	COLORREF m_SelecColor;                         // ��ѡ��ɫ
	//}} qy �ƿؼ��޸�20091109

protected:
	//��������������ĳһ���Ƶĸ���
	BYTE FindACardNumInFriendList(BYTE iCardNum);
	//����ͼƬ
	void SetCardImage();
	//����������λ��
	BOOL ChangeIndex(BYTE DestIndex ,BYTE SrcIndex);

	BYTE GetCardIndex(CPoint &Point);

	//��������
	BOOL ChangeIndexArray(BYTE DestIndex,BYTE SrcIndex);
	//���Ʒ�ת
	BOOL Convert(BYTE CardList[],int iCardCount);
	DECLARE_MESSAGE_MAP()


	CByteArray						m_MoveSelectCardArray;					//�ƶ�ѡ���˿�����
};

