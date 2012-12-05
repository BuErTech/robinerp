#pragma once

#include "StdAfx.h"

//�������ֿռ�
//using namespace AFC::AFCImage;

//��Ϣ����
#define	IDM_LEFT_HITCARD			WM_USER+110						//����˿�
#define	IDM_RIGHT_HITCARD			WM_USER+111						//�һ��˿�
#define IDM_SUPER_TIMER				WM_USER+113						//��ʱ
#define IDM_MOVE_ONCARD				WM_USER+115						

//��������
#define	UG_HUA_MASK					0xF0							//��ɫ����
#define	UG_VALUE_MASK				0x0F							//��������
void  Testlog(TCHAR *pMessage);
//�˿˿�����
class CUpGradeGameCard : public CWnd
{
	//ö�ٶ���
public:
	enum XShowMode { SWX_MODE_LEFT, SWX_MODE_CENTER, SWX_MODE_RIGHT };
	enum YShowMode { SWY_MODE_TOP,  SWY_MODE_CENTER, SWY_MODE_BOTTOM }; 
	
	CDC userDC;
	//��������
protected:
//	int								m_iCardHight;					//�˿˸߶�
//	int								m_iCardWidth;					//�˿˿��
//	int								m_iCardHSpace;					//�˿˼��
//	int								m_iCardVSpace;					//�˿˼��
//	int								m_iCardUpSpace;					//����߶�
//	CRgn							m_CardViewRgn;					//�˿�����
	CWnd							* m_pHandleWnd;					//������
	
	int								initFlag;			//��ʼ������
	int								m_iQZiWidth;
	int								m_iBoard[21][21];	//���
	int								m_iMeFlag;			//�ҷ���־
	int								m_iCurPlay;			//��ǰ���ӷ�;
	BOOL							m_bInitBoard;		//��ʼ������
	int								m_iBoardWidth;		//���̿��
	int								m_iBoardHeight;		//���̸߶�
	
	int								m_iStartI,			//������ʼ��
									m_iStartJ,
									m_iEndI,			//�����յ�
									m_iEndJ;
	BOOL							m_bIsPlay;			//�Ƿ�����
public:
	int								m_iJuMin;			//��ʱ
	int								m_iJuSec;
	int								m_iBuMin;
	int								m_iBuSec;
	int								m_iDuMin;
	int								m_iDuSec;

	int								m_iTopJuUseTime;//�Ϸ���ʱ
	int								m_iTopBuUseTime;
	int								m_iTopDuUseTime;
	int								m_iBotJuUseTime;//�·���ʱ
	int								m_iBotBuUseTime;
	int								m_iBotDuUseTime;
	
	UserItemStruct					*m_pUserInfo[2];//�û���Ϣ


	int								m_iFocusX;
	int								m_iFocusY;

	int								m_iTiZiNum[2];		//������
	bool							m_bUseMouseFlag;	//ʹ������־

	CString							m_strFinishMessage;	//������Ϣ
	bool							m_bIsFinish;		//�Ƿ����
public:
	int								m_iQiZiNum[2];		//Ŀ������������

#ifdef	VIDEO
	public:
		CPoint							m_PtVideo[2];					//�û���Ƶ����λ��
	public:
		CGameImage						m_pVideoFrame;					//��Ƶ����
#endif

	//��Ϣ����
protected:
	//bool							m_bShowCard;					//��ʾ�˿�
//	bool							m_bHorzLook;					//�Ƿ���
	bool							m_bUseMouse;					//��Ӧ���
	//BYTE							m_bHotHitCard;					//�����˿�
	//BYTE							m_bCardCount;					//�˿���Ŀ
	CPoint							m_PtBasePoint;					//��׼����
	XShowMode						m_XShowMode;					//��ʾģʽ
	YShowMode						m_YShowMode;					//��ʾģʽ
	//CByteArray						m_CardArray;					//�˿�����
//	CByteArray						m_CardUpArray;					//�˿�����

	//ͼƬ����
protected:
//	CAFCImage						m_CardPic[2];					//�˿˿ؼ�
	//CGameImage						m_CardPic[2];					//�˿˿ؼ�
	
	CGameImage						m_BoardPic;//����
	
	CGameImage						m_bitFocus;//���ӱ�־
	CGameImage						m_bitBlack,//����
									m_bitWhite,//����
									m_bitCurBlack,//��ǰ����
									m_bitCurWhite,//��ǰ����
									m_bitHeiGuan,//�����
									m_bitBaiGuan,//�����
									m_bitBCross,//
									m_bitWCross;//


	//��������
public:
	//���캯��
	CUpGradeGameCard();
	//��������
	virtual ~CUpGradeGameCard();

	//���ú���
public:
	//��ʼ������
	bool InitGameCard(CWnd * pHandleWnd);
	//�����˿˲���
	void SetCardParameter(int iHSpace, int iVSpace, int iUpSpace);
	//���û�׼�㺯��
	void SetBasePoint(CPoint & BasePoint, XShowMode XShowMode, YShowMode YShowMode);
	//������ʾ�˿�
	//void SetShowCard(bool bShow);
	//����ʹ�����
	void SetUseMouse(bool bUserMouse);
	//������ʾ��ʽ
	//void SetLookMode(bool bHorzLook);

	//���ܺ���
public:

	//�������
	void SetChess(int iMeFlag, int iCurPlay, int qi[][21], BOOL initBoard = FALSE);
	//���������־
	void SetPlayFlag(int iStartI, int iStartJ, int iEndI, int iEndJ, BOOL bIsPlay = FALSE);
	//����ʱ��
	void SetPlayTime(int iJuMin, int iJuSec, int iBuMin, int iBuSec, int iDuMin, int iDuSec);
	//������ʱ��
	void SetTimerCount();
	//���ý�����Ϣ
	void SetFinishMessage(CString strMessage, bool bIsFinish);
	//�����û���Ϣ
	void SetUserInfo(UserItemStruct *m_UserInfo, int iStation);
	//��������
	void SetTiZiNum(int iNum[]);
	//�����˿�
	//bool SetCard(BYTE bCardList[], BYTE bUpList[], BYTE bCardCount);
	//ɾ��������˿�
	//BYTE RemoveUpCard();
	//��ȡ������˿�
	//BYTE GetUpCard(BYTE bCardList[]);
	//��ȡ�˿���Ϣ
	//BYTE GetCard(BYTE bCardList[], BYTE bUpList[]);

	//��������
public:
	//��ȡ�˿���Ŀ
//	BYTE GetCardCount() { return m_bCardCount; }
	
	//�ڲ�����
protected:
	//��������
	BYTE HitCardTest(CPoint & Point);
	//��������λ��
	void FixCardControl();
	//��ȡ��С����
	bool GetCardSize(CSize & Size);
	

	//��������
	void SetBoard(CDC &memDC);
	//��������
	void SetQiZi(int i, int j, int type, CDC *memDC);
	//��ʾ����
	void ShowQiZi(int iDestX, int iDestY,int type, CDC *memDC);
	//��ʾ���ӱ�־
	void ShowPlayFlag(CDC *memDC);
	//get init time string
	void GetInitTimeString(int iMinute, int iSec,CString &str);
	//get time string 
	void GetTimeString(int iSec, CString &str);
	//��ʾ������
	void ShowTiZiNum(CDC *memDC);
	//set display time
	void SetTimeDisplay(int iDestX, int iDestY,CString &str, CDC *memDC, COLORREF crColor);
	//��ʾ��˭�߱�־
	void ShowGeneralFlag(CDC *memDC);
	//��ʼ����Ϣ
	void SetInitInfo(CDC *memDC, bool bFlag = false); 
	//��ʾ�����Ϣ
	void ShowFinishMessage(CDC *memDC);

	//͸����ͼ
	void TransparentBlt(CDC *pDestDC,int iDestX, int iDestY, int iDestWidth, int iDestHeight,
						CDC *pSrcDC, int iSrcX, int iSrcY, int iSrcWidth, int iSrcHeight, COLORREF transparentColor);
	//��Ϣ����
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�ػ�����
	afx_msg void OnPaint();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	void Init(void);
};

