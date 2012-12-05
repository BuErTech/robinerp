#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "GameNoteWnd.h"
#include "HaveThing.h"
#include "LeaveMessage.h"
#include "GameCard.h"
#include "GameFinishDlg.h"
#include "DisplayTalkDialog.h"	//�����
#include "FlashModule\shockwaveflash1.h"
#include "Counter.h"
#include "Dialect.h"

class CUpGradeDlg;
class CUpGradeView;

extern DWORD      g_bucFileHandle;  //buc�ļ����
extern CGameImage g_gameImage;      //��ϷͼƬ�ļ�


//�ؼ� ID
#define IDC_OUT_CARD				121				///< ���ư�ť
#define IDC_CONTINUE				124				///< ������ť
#define IDC_LAST_CARD				125				///< ���ְ�ť
#define IDC_HAVE_THING				127				///< ����Ҫ��
#define IDC_AUTOCARD				130				///< �й�
#define IDC_PASS					132				///< ����
#define IDC_SORT_CARD				133				///< ����
#define IDC_CUE						134				///< ��ʾ
#define IDC_COUNTER                 135             ///<������
#define IDC_DIALECT                 136             ///<����
#define IDC_CALL_SCORE_PASS			140				///< �з�����
#define IDC_CALL_SCORE_1			141				///< �з�����
#define IDC_CALL_SCORE_2			142				///< �з�����
#define IDC_CALL_SCORE_3			143				///< �з�����
#define IDC_LEAVE					144				///< �뿪����
#define IDC_ROB_NT					145				///< ������
#define IDC_ROB_NT_2				146				///< ����
#define IDC_ADD_DOUBLE				147				///< �Ӱ�
#define IDC_ADD_DOUBLE_2			148				///< ���Ӱ�
#define IDC_SHOW_CARD				149				///< ����
#define	IDC_SHOW_CARD_2				150				///< ����
#define IDC_CHANGE_SHIN				151				///<����
#define IDC_CHUI_YES				152				///<��
#define IDC_CHUI_NO					153				///<����

const  int VIDEO_WIDTH  = 180;
const  int VIDEO_HEIGHT = 150;
const  int VIDEO_WIDTH_MYSELF = 215;
const  int VIDEO_HEIGHT_MYSELF = 150;

//�����ͼ��
class CClientPlayView : public CUsingSkinClass//CGDIPlayView
{
	bool							m_bInit;
	//�������
protected:
	CPoint							m_PtNt[PLAY_COUNT];				///< ׯ�Һ�����λ��
	CPoint							m_PtAuto[PLAY_COUNT];			///< �й�����
	CPoint							m_PtAwardPoint[PLAY_COUNT];		///< �������
	CPoint							m_PtSequence[PLAY_COUNT];		///< �����Ⱥ�˳��
	CPoint							m_PtPass[PLAY_COUNT];			///< ����
	CPoint							m_PtLastCardFlag[PLAY_COUNT];	///< ���һ���Ʊ��
	CPoint							m_PtGetPoint[PLAY_COUNT];		///< ץ����ʾ����
	CPoint							m_PtDeskTempPoint;				///< ������ʱ��
	CPoint							m_PtDeskMultiple;				///< ���ӱ���
	CPoint							m_PtRoomMultiple;				///< ���䱶��
	CPoint							m_PtGameMultiple;				///< ���䱶��
	CPoint							m_PtCallScore[PLAY_COUNT];		///< �з�����
	CPoint							m_PtCallScoreResult;			///< �зֽ��(����������Ϸ�л����)
	CPoint							m_PtCallScoreNT;				///< �з�ׯ��
	CPoint							m_PtGameTitle;					///< ��Ϸ����logo����
	CPoint							m_PtBottomCue;					///< ��Ϸ��ʾ
	CPoint							m_PtPeopleMul[PLAY_COUNT];		///< ��ҼӰ���ʾλ��
	CPoint							m_PtShowMsg;					///< 
	CPoint                          m_PtGif[PLAY_COUNT];
	CPoint							m_PtResultTime;					
	CPoint							m_PtCurOrientation[PLAY_COUNT];	///< ��ǰ�����߷�������
	CPoint							m_PtDeskCue;					///< ������ʾ����	
	//�˿�����
	POINT							ptCardPoint[PLAY_COUNT];
	POINT							ptHandCard[PLAY_COUNT];
	POINT							ptBackCard;						///< ����
	int								m_Bt1WidthSpace;				///< �ұ߰�ť����
	int								m_Bt1HeightSpace;
	int								m_Bt2WidthSpace;				///< �зְ�ť
	int								m_Bt2HeightSpace;
public:
	CPoint                          m_ptVideoPos[PLAY_COUNT];        ///��ҵ���Ƶλ��
	int								m_iDeskTempPoint;				///< �����ϵķ���(��ʱ��)
	__int64							m_iDeskMultiple;				///< ���ӱ���
	__int64							m_iRoomMultiple;				///< ���䱶��
	__int64							m_iGameMultiple;				///< ��Ϸ����
	
	int								m_iNtStation;					///< ׯ��λ��
	int								m_iPass[PLAY_COUNT];			///< �������Ѻע����

	int								m_iCallScore[PLAY_COUNT];		///< ���ҽз�
	int								m_iCueType;						///< ��ʾ��ʽ

	bool							m_bFirstGame;					///< �Ƿ��½�������
	bool							m_bCanLeave;					///< �ܷ����˳�

	CToolTipCtrl					m_ToolTip;						///< ��ʾ����
	CDisplayTalkDialog				m_dlgDisTalk[PLAY_COUNT];		///< �����
	BYTE							m_iCurOrientation;				///< ��ǰ�����߷���
	//ͼƬ
	CGameImage						m_bit_background;				///< �������Сͼ
	CGameImage						m_Background;					///< ������ͼ(����ͼ)
	CGameImage						m_GameTitleLogo;				///< ��Ϸ����logo
	CGameImage						m_NT;							///< ׯ�ұ�ʶ	
	CGameImage						m_NT2;							///< ׯ�ұ�ʶ	
	CGameImage						m_AwardPint;					///< �������
	CGameImage						m_CallScore;					///< �з�ͼƬ
	CGameImage						m_AutoLogo;						///< �йܱ�ʶ
	CGameImage						m_Pass;							///< passͼʾ
	CGameImage						m_LastCardFlag;					///< ���һ������ʾͼ��
	CGameImage						m_PicSequence;					///< �����Ⱥ�˳����ʾͼ��
	CGameImage						m_PicBottomCue;					///< �����Ⱥ�˳����ʾͼ��
	CGameImage						m_PeopleMul;					///< �����Ⱥ�˳����ʾͼ��
	CGameImage						m_ShowMsg;						///< �����Ⱥ�˳����ʾͼ��
	CGameImage						m_ImgFangXiang;					///< ����ͼƬ	
	CGameImage						m_imgLogoFrame;					///< ͷ���ͼ 20090516
    CGameImage                      m_Num;
	Image*							m_ImgBackground[4];				//����ͼƬ����
	int								m_iCurrentBG;					//��ǰ����ͼƬ
	Image							*m_ImageAnim[5];				//����ͼƬ
	int								m_iCurrentStitch;				//������ǰ����
	Image*							m_pChuiSign;					//��һ����־
	CPoint							m_PtChui[PLAY_COUNT];			//��һ����־����
	BYTE 							m_iChui[PLAY_COUNT];			//�Ƿ�һ�� 1:δ֪״̬��0:������2:��

	CUpGradeGameCard				m_MeCard;						///< �Լ����˿�
	CUpGradeGameCard				m_MeBackCard;					///< �Լ��ĵ���
	CUpGradeGameCard				m_UserCard[PLAY_COUNT];			///< �û��˿�
	CUpGradeGameCard				m_HandCard[3];					///< �û������˿�
	CUpGradeGameCard                m_ThreeCard;            	    ///< ���˶������в����˵��˿� 

	CShockwaveflash1 m_FlashPlayer[PLAY_COUNT];	

	/// ���������ļ����ã��Ƿ���ʾ����ͷ��
	bool 		m_bShowLogo;
	bool        m_bShowMoney ;  ///�Ƿ���ʾ��ҽ��

	int			m_NameFont_Height;
	int			m_NameFont_Width;
	int			m_NameTextOffset_Height;
	int			m_NameTextOffset_Width;
	int			m_JinBiFont_Height;
	int			m_JinBiFont_Width;
	int			m_JinBiTextOffset_Height;
	int			m_JinBiTextOffset_Width;
	int			m_UserNameOffset_x[PLAY_COUNT];
	int			m_UserNameOffset_y[PLAY_COUNT];
	int			m_JinBiOffset_x[PLAY_COUNT];
	int			m_JinBiOffset_y[PLAY_COUNT];

	CString     m_strCoinName ;    //��������

	CLeaveMessage					m_ThingMessage;					///< �뿪��Ϣ
	CGameNoteWnd					m_GameNoteView;					///< ��Ϸ��Ϣ
	CGameFinishDlg					m_Result;						///< ��Ϸ������Ϣ
	CCounter                        m_CounterDlg ;                  ///�����������ť
	CDialect                        m_DialectDlg ; 

    // PengJiLin, 2011-4-19, ����ʱ���������ͽ��ͼ����ʾ
    CGetMoneyWnd                    m_GetMoneyImageView;

//	CStationView					m_StationView;					///< ״̬����
	CNormalBitmapButtonEx			m_btOutCard;					///< ���ư�ť
	CNormalBitmapButtonEx			m_btPass;						///< ����
	CNormalBitmapButtonEx			m_btCue;						///< ��ʾ

	CNormalBitmapButtonEx			m_btContinue;					///< ������ť
	CNormalBitmapButtonEx			m_btLeave;						///< ������ť
	CNormalBitmapButtonEx			m_btAuto;						///< �йܰ�ť
	CNormalBitmapButtonEx			m_btThing;						///< ����Ҫ��
	CNormalBitmapButtonEx			m_btSortCard;					///< ����
	CNormalBitmapButtonEx			m_btRobNT;						///< ������
	CNormalBitmapButtonEx			m_btRobNT2;						///< ����
	CNormalBitmapButtonEx			m_btAddDouble;					///< �Ӱ�
	CNormalBitmapButtonEx			m_btAddDouble2;					///< ���Ӱ�
	CNormalBitmapButtonEx			m_btShowCard;					///< ����
	CNormalBitmapButtonEx			m_btShowCard2;					///< ����
	CNormalBitmapButtonEx			m_btLastTurn;					///< ��һ��
	CNormalBitmapButtonEx			m_btCounter;					///< ������
	CNormalBitmapButtonEx           m_btdialect ;                   ///������ť
	CNormalBitmapButtonEx           m_btChangeShin;					///<����
	CNormalBitmapButtonEx           m_btChuiYes;					///<��
	CNormalBitmapButtonEx           m_btChuiNo;						///<����

	CHaveThing						m_HaveThing;					///< �����뿪
	BYTE							m_iAwardPoint[PLAY_COUNT];		///< �ļҽ������
	bool							m_bAuto[PLAY_COUNT];			///< �йܱ��
	bool							m_bUpdateScoreboard;			///< ���»��ְ�
	int								m_iPeopleMul[PLAY_COUNT];		///< ����Ƿ�Ӱ�

	bool                            m_bAIChooseCard ; 
public:
		//====�з����
	CNormalBitmapButtonEx			m_btCallScorePass;				///< ����
	CNormalBitmapButtonEx			m_btCallScore1;					///< ����
	CNormalBitmapButtonEx			m_btCallScore2;					///< ����
	CNormalBitmapButtonEx			m_btCallScore3;					///< ����

private:
	COLORREF			m_NameColor;								///< ��ʾ�û���ɫ

public :
	BOOL					m_bWatch;								///< �Լ��Ƿ�Ϊ��ս
	CGameImage				m_pVideoFrame;							///< ��Ƶ����
	CRect					m_rectVideoFrame[MAX_PEOPLE];			///< ��Ƶ��������
	BOOL					GetVideoRect(CString  strNickname, RECT& rect, CString& pic);

public:
	void SetNameColor(COLORREF rgb)	{m_NameColor=rgb;	}
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

	//��Ϣ����
protected:
	/// ��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

public:
	/// ���ñ���ɫ
	void SetBackBmp(UINT uResourceID, TCHAR * szFileName=NULL, BOOL bResource=TRUE);
	/// ���Ʊ������Сͼ
	void DrawBk(CDC *pDC, int iWidth, int iHeight);
	/// ������Ϸlogo
	void DrawGameTitle(CDC *pDC);
	/// ����ׯ��λ��
	void SetNTStation(int iViewStation=-1);
	/// �滭 NT ��־
	void DrawNt(CDC * pDC, int x, int y);
	/// �滭 NT ͼ��
	void DrawNtLogo(CDC * pDC, int x, int y, int iHuaKind);
	DECLARE_MESSAGE_MAP()
	/// ��ť��Ϣ����
protected:
	/// ��ʼ��ť
	void OnHitBegin();
	/// �뿪
	void OnHitLeave();	
	/// ���ư�ť
	void OnHitOutCard();
	/// ������ť
	void OnHitContinue();
	/// ����Ҫ��
	void OnHitThing();
	/// Ͷ��
	void OnHitTuoxiang();
public:
	/// ����������
	void DrawFen(CDC * pDC);
public:
	void OnHitAuto(void);

	void SetCallScoreValue(BYTE bViewStation,int iCallScore);
	
	int m_iCallScoreResult;
	/// �зֽ��
	void SetCallScoreResult(int iCardCount = 0);		
	/// �й�
	void SetAuto(int bDeskStation,bool bAuto);
	/// �����й�
	BOOL DrawAuto(CDC * pDC);
	/// ���ý���
	void SetAwardPoint(BYTE iViewStation,BYTE iAwardPoint);
	/// �L�u����
	void DrawAwardPoint(CDC *pDC);
	///����з�
	void OnHitCallScore(UINT uID);
	/// ��ʾ
	void OnCue();	
	/// �������
	void OnHitPass();
	///��Ӽ���������
	void OnHitCounter() ; 
	///����
	void OnHitDialect() ;
	/// ����
	void OnHitSort();
	/// �O�÷��䱶��
	BOOL SetRoomMultiple(int iRoomMultiple = 0);
	/// ���Ʒ��䱶��
	BOOL DrawRoomMultiple(CDC *pDC);
	/// ������Ϸ����
	BOOL SetGameMultiple(int iGameMultiple = 0);
	/// ������Ϸ����
	BOOL DrawGameMultiple(CDC *pDC);
	/// ���ƽзֽ��
	BOOL DrawCallScoreResult(CDC *pDC);
	/// �������汶��
	BOOL SetDeskMultiple(int iDeskMultiple= 0);
	/// �������汶��
	BOOL DrawDeskMultiple(CDC *pDC);
	/// ����������ʱ��
	void SetDeskTempPoint(int iDeskTempPoint = 0);
	/// ����������ʱ��
	void AddDeskTempPoint(int iDeskTempPoint);
	/// ����������ʱ��
	BOOL DrawDeskTempPoint(CDC *pDC);
	/// ���Ƶײ���ʾ
	BOOL DrawBottom(CDC * pDC);
	/// ����
	void DrawPass(CDC *pDC);
	/// ���÷���
	void SetPass(BYTE iViewStation,int iGiveup);
///////////////////////////////////////////////////////////////////////////
	CUpGradeGameCard		m_DeskCard;								///< ��������(������ʾ���ƶ���)
	CUpGradeGameCard		m_SendCard[PLAY_COUNT];					///< ��������(������ʾ���ƶ���)

	BOOL				m_bHaveSendCardAnimal[PLAY_COUNT];			///< ���ƶ������Ʊ���
	CPoint				m_PtStart[PLAY_COUNT];						///< ���ƶ��������
	CPoint				m_PtSendCardEnd[PLAY_COUNT];				///< ���ƶ�������������
	CPoint				m_PtSendCardStart[PLAY_COUNT];				///< ���ƶ�����ʼ������

	void				SetSendCardPointStart(BYTE iViewStation);	///< ���÷��ƶ�����ʼ����
	void				SendCardAnimal();							///< ���ƶ�������

///////////////////////////////////////////////////////////////////////////
	/// �����
	void DisplayTalkMsg(int Viewstation,TCHAR * szCharString);
	/// ��ʾ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	/// ����
	void SetOrientation(BYTE bDeskStation);
	/// ���Ʒ���
	BOOL DrawOrientation(CDC *pDC);
	/// ������ʼ��
	void Init();
	/// ������Ϸ��������
	void LoadSkinIni(int cx,int cy);
	/// �������������ƶ�
	void MoveScreen(int cx,int cy);
	/// ������
	void OnHitRobNT();
	/// ��������
	void OnHitRobNT2();
	/// �Ӱ�
	void OnHitAddDouble();
	/// ���Ӱ�
	void OnHitAddDouble2();
	/// �Ӱ�
	void OnHitShowCard();
	/// ���Ӱ�
	void OnHitShowCard2();
	/// ��һ��
	void OnHitLastTurn();
	//����
	void OnChangeShin();
	/// ��
	void OnHitChuiYes();
	/// ����
	void OnHitChuiNo();
	/// ������ұ���
	void SetPeopleMultiple(BYTE bViewStation,int iVal = 1);
	/// ������ұ���
	void DrawPeopleMultiple(CDC *pDC);
	/// ������ҽзֽ���ͱ��ʱ���ͼ
	void DrawShowMsg(CDC *pDC);

public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
     //��������������
	 HRGN CreateRgnFromFile( HBITMAP hBmp, COLORREF color );
     //ȥ����Ƶ��ɫ
     void RemoveVideoBaseColor(void);

	 void SetVideoRgn(BYTE iView);

	 void DarwCardImage(CDC *pDC);	//��2.5D�Ʊ�

	 void SetAnimStitch(int iStitch);	//���ö�������

	 void DarwChuiAnim(CDC *pDC,int iW,int iH);	//������ͼƬ

	 void SetChuiData(BYTE byStation, BYTE iFlag);	//���ô�һ������

	 void DarwChuiSign(CDC *pDC);			//���ƴ�һ����־
};
