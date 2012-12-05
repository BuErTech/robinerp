#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "StationView.h"
#include "NtList.h"
#include "GameNoteWnd.h"
#include "PlaySetDlg.h"
#include "GameInfoDlg.h"
//#include "GameEndInfoDlg.h"
#include "BackCardDlg.h"
#include "HaveThing.h"
#include "LeaveMessage.h"
#include "GameCard.h"
#include "Stop.h"
#include "StopMessage.h"
#include "ChessManual.h"
#include "AdmitDefeat.h"
#include "RegretChess.h"
#include "GameFinishDlg.h"
#include "GameDaoSHiDlg.h"
class CUpGradeDlg;
class CUpGradeView;

/// �ؼ� ID
#define IDC_GAME_BEGIN				120				///< ��ʼ��ť
#define IDC_OUT_CARD				121				///< ���ư�ť
#define IDC_GIVE_BACK				122				///< ���װ�ť
#define IDC_CONTINUE				124				///< ������ť
#define IDC_LAST_CARD				125				///< ���ְ�ť
#define IDC_BACK_CARD				126				///< ���ư�ť
#define IDC_HAVE_THING				127				///< ����Ҫ��
#define IDC_STOP					128				///< ֹͣ����
#define IDC_ADMIT_DEFEAT			130				///< ����
#define IDC_REGRET_CHESS			131				///< ����
#define IDC_CHESS_MANUAL			132				///< ����
#define IDC_USER_FACE				133				///< �û�����
#define IDC_USER_FACE2				134				///< �û�����
#define IDC_LEAVE					135				///< ���_

/// �����ͼ��
class CClientPlayView : public CUsingSkinClass//CGDIPlayView
{
	/// �������
protected:
	CPoint				m_PtNt[4];					///< �û�����λ��
	CPoint				m_PtNtLogo[4];				///< �û�����λ��
	CPoint              m_PtGif[PLAY_COUNT];
	
	/// �û���Ϣ
	int					m_iHuaKind;					///< ���ƻ�ɫ
	int					m_iShowStation;				///< ����λ��
	int					m_iNtStation;				///< ׯ��λ��

#ifdef	VIDEO
	public:
		CRect				m_RtVideo[2];			///< �û�����Ƶ����λ��
#endif

	/// �ؼ�����
public:
	CBitmap				m_NT;									///< NT ͼ��
	CBitmap				m_NTLogo;								///< ��־ͼ��
	//CNtList				m_NtList;							///< ���ư�ť
	CUpGradeGameCard			m_MeCard;						///< �Լ����˿�
//	CUpGradeGameCard			m_MeBackCard;					///< �Լ��ĵ���
	int						m_iCardNum;
//	CUpGradeGameCard			m_UserCard[4];					///< �û��˿�		
	CGameInfoDlg		m_GameInfo;								///< ��Ϸ��Ϣ(�������õ���Ϸʱ����Ϣ)
	CGameDaoSHiDlg		m_GameDaoShi;							///< ��Ϸ��Ϣ(�������õ���Ϸʱ����Ϣ)
	CPlaySetDlg			m_GameSet;								///< ��Ϸ����(��һ�����뷿�䵯���Ĵ����)
	CGameNoteWnd		m_GameNoteView;							///< ��Ϸ��Ϣ
	CStationView		m_StationView;							///< ״̬����
//	CNormalBitmapButtonEx			m_btUserFace[0];			///< �û�ͷ��
//	CNormalBitmapButtonEx			m_btUserFace[2];			///< �û�ͷ��
	CNormalBitmapButtonEx			m_btOutCard;				///< ���ư�ť
	CNormalBitmapButtonEx			m_btGiveBack;				///< ���װ�ť
	CNormalBitmapButtonEx			m_btContinue;				///< ������ť
//	CAFCPicButton			m_btLastTurn;						///< ��һ��
	CNormalBitmapButtonEx			m_btLastTurn;				///< ��һ��
	CNormalBitmapButtonEx			m_btBackCard;				///< ����
	CNormalBitmapButtonEx			m_btThing;					///< ����Ҫ��
	CNormalBitmapButtonEx			m_btStop;					///< ֹͣ����  ;���
	CNormalBitmapButtonEx			m_btAdmitDefeat;			///< ����
	CNormalBitmapButtonEx			m_btRegretChess;			///< ����
	CNormalBitmapButtonEx			m_btChessManual;			///< ����
	CNormalBitmapButtonEx			m_btStart;					///< ����
	CNormalBitmapButtonEx			m_btLeave;					///< ����


	
	CLeaveMessage		m_ThingMessage;							///< �뿪��Ϣ
	CBackCardDlg		m_BackCardDlg;							///< ������ͼ
	CHaveThing			m_HaveThing;							///< �����뿪
	CStop				m_StopThing;
	CStopMessage		m_StopMessage;
	CAdmitDefeat		m_AdmitDefeat;							///< ����
	CChessManual		m_ChessManual;							///< /����
	CRegretChess		m_RegretChess;							///< ����
	CGameFinishDlg		m_GameFinishView;						///< ��Ϸ����
//	GameEndInfoDlg		m_GameEndView;							///< ������ͼ
//	CGameImage			m_PicTopBar;							///< ������ͼ
//	CGameImage			m_PicTopBarLeft;						///< ������ͼ
//	CGameImage			m_PicTopBarRight;						///< ������ͼ

	CGameImage			m_imgBk;  								///< ��Ϸ����ͼ


	bool				m_bUpdateScoreboard;					///< ���»��ְ�

	/// ��������
public:
	/// ���캯��
	CClientPlayView();
	/// ��������
	virtual ~CClientPlayView();

	/// ���غ���
public:
	/// ���ý��溯��
	virtual void ResetGameView();
	/// �滭���溯��
	virtual void DrawViewFace(CDC * pDC, int iWidth, int iHeight);
	/// �����û�����
	virtual void UpdateUserInfo(BYTE bViewStation);
	virtual void FixControlSize(int,int);
	/// ���ܺ���
public:
	void DrawInfo(CDC *pDC);
	void TransparentBlt2( HDC hdcDest,  // Ŀ��DC
					int nXOriginDest,   // Ŀ��Xƫ��
					int nYOriginDest,   // Ŀ��Yƫ��
					int nWidthDest,     // Ŀ����
					int nHeightDest,    // Ŀ��߶�
					HDC hdcSrc,         // ԴDC
					int nXOriginSrc,    // ԴX���
					int nYOriginSrc,    // ԴY���
					int nWidthSrc,      // Դ���
					int nHeightSrc,     // Դ�߶�
					UINT crTransparent  // ͸��ɫ,COLORREF����
					);

	/// ��Ϣ����
protected:
	/// ��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	/// ��ʾ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	/// ���ñ���ɫ
	void SetBackBmp(UINT uResourceID, TCHAR * szFileName=NULL, BOOL bResource=TRUE);
	/// ����ׯ��λ��
	void SetNTStation(int iViewStation=-1);
	/// ���ñ���λ��
	void SetShowNtStation(int iViewStation=-1, int iHuaKind=0);
	/// ������Ϸ����
	void SetGamePoint(int iGamePoint=0, int bMeSide=-1);
	/// �滭 NT ��־
	void DrawNt(CDC * pDC, int x, int y);
	/// �滭 NT ͼ��
	void DrawNtLogo(CDC * pDC, int x, int y, int iHuaKind);
	DECLARE_MESSAGE_MAP()
	/// ��ť��Ϣ����
protected:
	/// ��ʼ��ť
	void OnHitBegin();
	/// �����ư�ť
	void OnHitBackCard();
	/// ���ư�ť
	void OnHitOutCard();
	/// ������ť
	void OnHitContinue();
	/// �����˿�
	void OnHitLastTurn();
	/// ���ư�ť
	void OnHitLookBack();
	/// ����Ҫ��
	void OnHitThing();
	/// ��ǰֹͣ
	void OnHitStop();
	/// ����
	void OnHitAdmitDefeat();
	/// ����
	void OnHitRegretChess();
	/// ����
	void OnHitChessManual();
	/// ����
	void OnHitLeave();

public:
#ifdef	VIDEO
	BOOL						m_bWatch;		///< �Լ��Ƿ�Ϊ��ս
	CGameImage					m_pVideoFrame;	///< ��Ƶ����
	/// �����ǳƵ������Ƶ���ھ���д�����rect,������TRUE;û�ҵ�����falst
	BOOL	GetVideoRect(CString  strNickname, RECT& rect, CString& pic);
#endif

	static CBitmap				m_Bitmap_Num;
	BOOL						m_bRightPower;
	BOOL						m_bMeSide;					///< �Ƿ񱾷��÷�
	int							m_iGamePoint;				///< ��Ϸ����
};
