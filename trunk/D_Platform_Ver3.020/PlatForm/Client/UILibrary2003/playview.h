#pragma once

#ifndef CPLAYVIEW_HEAD_FILE
#define CPLAYVIEW_HEAD_FILE

#include "PublicDefine.h"
#include "AFCResource.h"
#include "ClientComStruct.h"
#include "ShowScore.h"
#include "objectfactory.h"
/*******************************************************************************************************/
 
//��ܴ�С 
#define VIEW_FRAME_TOP					0								//����ϸ�
#define VIEW_FRAME_LEFT					18								//������
#define VIEW_FRAME_RIGHT				18								//����ҿ�
#define VIEW_FRAME_BOTTOM				18								//����¸�

//��ʱ����С
#define TIME_WIDTH						30								//ʱ����
#define TIME_HEIGHT						21								//ʱ��߶�								 

//ͬ���־��С
#define AGREE_WIDTH						13								//ͬ�����
#define AGREE_HEIGHT					13								//ͬ���߶�

//��ʱ����Ϣ
#define THREAD_TIME_ID					1								//�̶߳�ʱ�� ID

#define GIF_COUNT						4								//gif��������

//#define PLAYVIEW_LBUTTON_DOWN			WM_USER + 3421					//add by zxj 2008/04/09
//#define PROP_SELECT_CURSOR				WM_USER + 3562					//add by zxj 2008/04/09	
/*******************************************************************************************************/

//�滭��Ϣ��
class EXT_CLASS CNewCDC//C-ThreadDraw
{
	//��������
private:
	bool								m_bQuit;						//�Ƿ��˳�
	int									m_iSpeed;						//��ʾ֡��
	int									m_iTimeSpace;					//˯��ʱ��
	int									m_iGDISleepTime;				//˯��ʱ��
	HANDLE								m_hGDIThreadHandle;				//�߳̾��
	HANDLE								m_hTimeThreadHandle;			//�߳̾��

	//λ����Ϣ�����߳����ã�
protected:
	int									m_iXPos;						//X ��
	int									m_iYPos;						//Y ��
	int									m_iDrawWidth;					//��ͼ���
	int									m_iDrawHeight;					//��ͼ�߶�
	CWnd								* const m_pGameView;			//��Ϸ��ͼ

	//��������
public:
	//���캯��
	CNewCDC(CWnd * pGameView);
	//��������
	virtual ~CNewCDC();

	//���غ���
public:
	//��ʼ����ͼ����
	virtual bool InitDraw(CDC * pDrawDC);
	//ȡ����ʼ������
	virtual bool UnInitDraw();
	//�滭���溯��
	virtual void ThreadDrawView(CDC * pDC, int iDrawWith, int iDrawHeight)=0;

	//���ܺ���
public:
	//��ȡ֡��
	int GetDisplaySpeed() { return m_iSpeed; }
	//���ö�ʱ�����
	void SetTimeSpace(int iTimeSpace) { m_iTimeSpace=iTimeSpace; };
	//���� GDI �̼߳��
	void SetGDISleepTime(int iGDISleepTime) { m_iGDISleepTime=iGDISleepTime; };
	//���÷�Χ
	void SetDrawRange(int iXPos, int iYPos, int iDrawWidth, int iDrawHeight);

	//�̺߳���
public:
	//��ʼ�߳�
	bool CreateDrawThread();
	//�����߳�
	bool EndDrawThread();
	//ֹͣ�߳�
	void SuspendThread();
	//�����߳�
	void ResumeThread();
	//��ʱ���߳�
	static unsigned __stdcall TimerThread(LPVOID pThreadData);
	//��ͼ�߳�
	static unsigned __stdcall GDIModeThread(LPVOID pThreadData);
};

/*******************************************************************************************************/

//��ͼ������
class EXT_CLASS CUsingSkinClass/*C-GDIPlayView*/ : public CWnd
{
	//��Ԫ����
	friend class CNewCDC;
	//�滭��Ϣ
private:
	bool							m_bReDraw;							//�Ƿ��ػ�
	BITMAP							m_PicInfo;							//λͼ��Ϣ
	CBitmap							m_PicBuffer;						//����λͼ
	const bool						m_bDoubleBuf;						//�����־
	const int						m_iBufferColor;						//����ɫ��
	CNewCDC						* const m_pDrawInfoPtr;				//��ͼ��Ϣ
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	COLORREF						m_bkColor;

	HBRUSH m_bkBrush;
protected:
	CString							m_strTitle;							//���ڱ���
	CGameImage						m_PicGameView;						//��Ϸ��ͼ
	CGameImage						m_PicGameViewLogo;					//��Ϸ��ͼLOGO

	

	//�滭���������죬���У�ƽ�̣�
	enum DrawMode { DM_ELONGATE, DM_CENTENT, DM_SPREAD };

	//����ͼ
private:
	static CGameImage				m_PicAgree;							//ͬ��ͼ��
	static CGameImage				m_PicTimer;							//ʱ������
	static CGameImage				m_PicTop;							//���ͼƬ
	static CGameImage				m_PicBottom;						//���ͼƬ
	static CGameImage				m_PicLeft;							//���ͼƬ
	static CGameImage				m_PicRight;							//���ͼƬ
	static CGameImage				m_PicTopRight;						//���ͼƬ
	static CGameImage				m_PicTopLeft;						//���ͼƬ
	static CGameImage				m_PicBottomRight;					//���ͼƬ
	static CGameImage				m_PicBottomLeft;					//���ͼƬ
public:
	CGameImage						m_iUserFrame;						//����ͷ��ͼƬ
	//{add by huangyin
	//int								nLogoPositionInformation[MAX_PEOPLE][5];
	//[][0]:ID
	//[][1]:Left
	//[][2]:Top
	//[][3]:Width
	//[][4]:Height
//	LRESULT	OnPropSelectCursorChange(WPARAM wParam, LPARAM lParam);
//	HCURSOR	m_hCursor;
	//end add}
	//������Ϣ
protected:	
	UserItemStruct					* m_pUserItem[MAX_PEOPLE];			//�û���Ϣ

	//Ԫ��λ��
public:
	CShowScore						m_showcoredlg;
	UINT							m_uTime[MAX_PEOPLE];				//�û�ʱ��
	CPoint							m_PtLogo[MAX_PEOPLE];				//ͼ��λ��
	CPoint							m_PtName[MAX_PEOPLE];				//����λ��
	CPoint							m_PtTime[MAX_PEOPLE];				//ʱ��λ��
	CPoint							m_PtAgree[MAX_PEOPLE];				//ͬ��λ��
	int								m_ShowType;							//��ʾ��ʽ
	bool							m_bShowRight;						//�Ƿ���ʾ�ұ߿ؼ�
	int								m_vlistx;							//���ذ�ť��λ��
	CGameImage						m_bkimgs;							//��ʾ�ұ�ʱ��ͼƬ
	CGameImage						m_bkimgh;							//�����ұ�ʱ��ͼƬ
	//��������
protected:
	//���캯��
	CUsingSkinClass(bool bDoubleBuf=true, int iBufferColor=16, CNewCDC * pDrawInfoPtr=NULL);
	//��������
	virtual ~CUsingSkinClass();

	//���غ���
public:
	//�������ý���
	virtual void ResetGameView();
	//�滭���溯��
	virtual void DrawViewFace(CDC * pDC, int iWidth, int iHeight);
	//�����û�����
	virtual void UpdateUserInfo(BYTE bViewStation)=0;
	//����λ��
	virtual void FixControlSize(int iWidth, int iHeight)=0;

	//�滭���� 
public:
	//�滭����
	void DrawBackPic(CDC * pDC, DrawMode Mode, int iWidth, int iHeight);
	//�滭ͬ���־
	void DrawArgeeFlag(CDC * pDC, int iXPos, int iYPos);
	//�滭ʱ��
	void DrawTimer(CDC * pDC, int iXPos, int iYPos, UINT uTime);
	//�滭ͷ��
	void DrawLogo(CDC * pDC, UINT uLogoID, int iXPos, int iYPos, bool bLost,long dwUserID =0,BOOL bIsSmall = false,bool bRightTop=false);
	//����ͷ���
	virtual void DrawLogoFrame(CDC *pDC,int iXPos,int iYPos);
	//�滭���
	void DrawViewFrame(CDC * pDC, int iWidth, int iHeight);
	//͸���滭
	bool StretchBlt(CDC * pDesDC, int iXDes, int iYDes, int iDesWidth, int iDesHeight, CDC * pScrDC, int iXScr, int iYScr, int iScrWidth, int iScrHeight, COLORREF crColor);
	//�滭͸��ͼ
	bool StretchImage(CDC * pDesDC, int iXDes, int iYDes, int iDesWidth, int iDesHeight, HBITMAP hBitBmp, int iXScr, int iYScr, int iScrWidth, int iScrHeight, COLORREF crColor);

	//���ܺ��� 
public:
	//���½���
	void UpdateViewFace(const RECT * pRect);
	//�ػ�����
	void ReDrawViewFace(const RECT * pRect);
	//���ô��ڱ���
	void SetViewTitle(const TCHAR * szTitle);
	//������ʾʱ��
	virtual bool SetUserTimer(BYTE bViewStation, UINT uTime);   //yjj 
	//�����û���Ϣ
	bool SetUserInfo(BYTE bViewStation, UserItemStruct * pUserItem);
	//�ƶ�����
	bool SafeMoveWindow(UINT uID, int x, int y, int iWidth, int iHeight);
	//�ƶ�����
	bool SafeMoveWindow(CWnd * pControlWnd, int x, int y, int iWidth, int iHeight);

	//��Ϣ����
public:
	//�滭����
	afx_msg void OnPaint();
	//λ�ñ仯
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//����֪ͨ����
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��������
	afx_msg void OnDestroy();
	//����Ҽ�����
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//����frameͼƬ
	void SetFrameImage(TCHAR *path) ;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);	
	int						m_iGifDeskStation;					//��ǰ���Ŷ���λ��
	IGif					*m_pLoadGif;						//����
	IGif					*m_pGif;							//��ǰ����ָ��
	int						m_iFrameCount;						//����֡��
	int						m_iGifLifeTime;						//����������
	//��ʼ��gif	
	virtual void InitGif();
	//����gif
	virtual void SetGif(BYTE bDeskView,int iValue,int iTimer);
	//����gif
	virtual void DrawGif(CDC *pDC,int ix,int iy);

	//��ȡ����ǳ���ɫ
	virtual COLORREF GetNameColor(UserInfoStruct *pUserInfo);
	DECLARE_MESSAGE_MAP()
//public:
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//public:
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	////////////////////////////////
	/////Kylin 20090120 ������
	//����Gif��������
	void ClearGif();
	////////////////////////////////

};

/*******************************************************************************************************/

#endif