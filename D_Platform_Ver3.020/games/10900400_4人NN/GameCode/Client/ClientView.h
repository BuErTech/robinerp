#pragma once

#include "StdAfx.h"
#include "Resource.h"
//#include "StationView.h"
//#include "GameNoteWnd.h"
//#include "PlaySetDlg.h"
//#include "GameInfoDlg.h"
//#include "GameEndInfoDlg.h"
//#include "BackCardDlg.h"
//#include "HaveThing.h"
//#include "LeaveMessage.h"
#include "GameCard.h"
//#include "Stop.h"
//#include "StopMessage.h"
#include "ShowNoteBtn.h"
#include "GameFinishDlg.h"
#include "BeginAnimal.h"
//#include "EndAnimal.h"
#include "NoteAnimal.h"
#include "CeasyNtList.h"
#include "GameNoteWnd.h"
#include "CardShape.h"


class CUpGradeDlg;
class CUpGradeView;

#define ID_BEGIN_ANIMAL					10001			//��ʼ����
#define ID_NOTE_ANIMAL_ZERO				10002			//��ע����
#define ID_NOTE_ANIMAL_ONE				10003			//��ע����
#define ID_NOTE_ANIMAL_TWO				10004			//��ע����
#define ID_NOTE_ANIMAL_THREE			10005			//��ע����
#define ID_SEND_CARD_ANIMAL				10006			//���ƶ���
#define ID_COLLECT_JETON_ANIMAL			10007			//�ռ�����
#define WM_SETMESSAGE					10008			//������Ϣ
#define WM_SENDCARD_SHOWCARD			10009			//���ƺ���ʾ������

//�ؼ� ID
#define IDC_CONTINUE					124				//������ť
#define IDC_LEAVE						125				//�뿪
//#define IDC_LAST_CARD					125				//���ְ�ť
//#define IDC_BACK_CARD					126				//���ư�ť
#define IDC_HAVE_THING					127				//����Ҫ��
#define IDC_STOP						128				//ֹͣ����
#define IDC_AUTO						129				//�й�
#define IDC_SHUTTLE						135				//���˰�ť
#define IDC_NOTADD						137				//���Ӱ�ť
#define IDC_CALL_SCORE_PASS				140				//�з�����
#define IDC_CALL_SCORE_1				141				//�з�����
#define IDC_CANSOUND					142				//�û���������
#define IDC_BGSOUND						143				//��������
#define MAX_JETON						16				//��������
#define MAX_NOTEJEONT					50				//���֧��50��������ʾ����������
#define NOTEVIEW						150				//�ȴ���

//�����ͼ��
class CClientPlayView : public CUsingSkinClass//CGDIPlayView
{
//�������
public:
	BOOL						m_bIsNote[PLAY_COUNT];				//�Ƿ�����ע����
	bool                        m_bDraw;                            //�Ƿ����ע
	bool						m_bIsSendCard[PLAY_COUNT];			//˭�з���
	int						    m_bOpenCard[PLAY_COUNT];			//̯��
	int							m_flag;								//�Ƿ����е����
	int                         m_iGetPoint[PLAY_COUNT];            //������Ƶ÷�
	int                         m_iShape[PLAY_COUNT];
	int                         m_iGetPointSplit[PLAY_COUNT];       //������Ƶ÷� 
	//int							m_iThisTurnAddNote;				//����Ҫ��������������
	int							m_iNoteJeontTimes;					//�û���ע�������
	int							m_NoteStart;						//��עλ��
	BYTE                        m_GameType;							//��Ϸ���ࡣ0Ϊ��ͨ��1Ϊͬϲ��
	BYTE                        m_iGameNoteSetType;                 //��Ϸ������������

	//ȫ��������ͼ
	//CNoteAnimal					m_NoteAll[MAX_NOTEJEONT];		//�û���ע��������ͼ
	//CNoteAnimal					m_NoteAnimal;					//�û���ע����
	//CNoteAnimal					m_NoteTotal[PLAY_COUNT];	
	CPoint						m_NoteAllPoint[MAX_NOTEJEONT];
	//CUpGradeGameCard			m_SendCardAnimal[PLAY_COUNT];		//���ƶ���ʹ�õ���
	//CBeginAnimal				m_BeginAnimal;						//��ʼ����
	//CUpGradeGameCard			m_FlashCard;						//���˿˶���Ч��
	CGameNoteWnd			    m_GameNoteView;						//��Ϸ��Ϣ

	
protected:
	CPoint				m_PtAuto[PLAY_COUNT];							//�û��Ϲ�
	CPoint				m_PtNote[PLAY_COUNT];							//�û�Ѻעλ��
	CPoint				m_PtNoteTotal[PLAY_COUNT];						//�û���ע
	CPoint				m_PtNoteLimit[PLAY_COUNT];						//������ע����
	CPoint				m_PtFace[PLAY_COUNT];							//���������ʾλ��
	CPoint				m_PtScore[PLAY_COUNT];							//�û�������
	CPoint				m_PtPass[PLAY_COUNT];							//����
	CPoint				m_PtJeton[PLAY_COUNT];							//��ע����λ��
	CPoint				m_PtShowLimit;									//��ʾ�û���ע����
	CPoint				m_PtNoteAll;									//�û���עȫ����������λ��
	CPoint				m_PtBeginAnimalStart;							//��ʼ�������
	CPoint				m_PtBeginAnimalEnd;								//��ʼ�����յ�
	CPoint				m_PtGameTitle;									//��Ϸ����logo����
	CPoint              m_PtPeopleBk[PLAY_COUNT];						//���ﱳ��ͼ����
	CPoint              m_PtNt[PLAY_COUNT];								//���Ʊ�־����
	CPoint				m_PtGetPoint[PLAY_COUNT];						//ץ����ʾ����
    CPoint              m_PtShape[PLAY_COUNT];							//���ͱ�־
	CRect				m_BtnRect0,m_BtnRect1,m_BtnRect2,m_BtnRect3;	//���ο�
	CPoint              m_PtGif[PLAY_COUNT];							//����
	CPoint				m_PtOpenCard[PLAY_COUNT];						//̯��
public:
	//��ע������̬����ϵ
	CPoint				m_PtNoteAnimalStart[PLAY_COUNT];				//���ҿ�ʼ��ע������ͼ����
	CPoint				m_PtNoteAnimalEnd[PLAY_COUNT];					//��ע������������
	//�û���Ϣ
//	int					m_iHuaKind;						//���ƻ�ɫ
//	int					m_iShowStation;					//����λ��
	int					m_iNtStation;					//ׯ��λ��
	int                 m_nPowerOfGold;             	//��ұ���

	//�ؼ�����
public:
	CGameImage				m_Jeton;					//��־ͼ��
	//ͼƬ
	CGameImage				m_bit_background;			//�������Сͼ
	CGameImage				m_Background;				//������ͼ(����ͼ)
	CGameImage				m_GameTitleLogo;			//��Ϸ����logo
	CGameImage				m_Pass;						//passͼʾ
	CGameImage              m_PeopleBk1;				//���ﱳ��1
	CGameImage              m_PeopleBk2;				//���ﱳ��2
	CGameImage              m_PeopleBk3;				//���ﱳ��3

	CGameImage              m_Ming;						//���Ʊ�־
	CGameImage				m_HuangGuan;				//�ʹ�
	CGameImage				m_PicGreenNum;				//��ɫ����
	CGameImage              m_NoteKuan;					//�����
	CGameImage				m_PicNum;					//ʱ������
	CGameImage				m_PicSmallNum;				//Сʱ������
	CeasyNtList			    m_NtList;
	CShowNote			    m_ShowNote;					//��ʾ��ť
	
	CUpGradeGameCard		m_UserCard[PLAY_COUNT];		//�û��˿�
    CUpGradeGameCard        m_UserBull[PLAY_COUNT];
	//CUpGradeGameCard		m_SysBackCard;				//ϵͳ����ʾ

	CGameFinishDlg			m_Result;					//��Ϸ������Ϣ

	CCardShape             m_CardShape[PLAY_COUNT]; // �ĸ���ҵ�����

	CNormalBitmapButtonEx			m_btContinue;		//������ť
	CNormalBitmapButtonEx			m_btLeave;			//�뿪
	CNormalBitmapButtonEx           m_btReset;          //Ҫ��
	CNormalBitmapButtonEx           m_btOpen;           //��Ҫ��
	CNormalBitmapButtonEx           m_btBull;           //�ӱ�
	CNormalBitmapButtonEx           m_btChangeShin;     //����

			//====�з����
	CNormalBitmapButtonEx			m_btCallScorePass;					//����
	CNormalBitmapButtonEx			m_btCallScore1;						//����
	CGameImage						m_PicLeftLogo;						//������ͼ
	CGameImage						m_PicLogoface;						//ͷ����ͼ
    CGameImage                      m_Waiting;                          //�ȴ���ͼ
	CGameImage						m_PicScore;							//�û�����ע��
	CGameImage                      m_Nt;                               //ׯ
	CGameImage                      m_WinFen;                           //Ӯ��
	CGameImage                      m_LossFen;                          //���
	CGameImage                      m_Niu;                              //ţ
	CGameImage                      m_Niu_n;                            //��ţ
	CGameImage                      m_Nn;                               //ţţ
	CGameImage                      m_ImgJn;                            //��ţ
	CGameImage                      m_ImgYn;                            //��ţ
	CGameImage                      m_ImgBomb;                          //ը��
	CGameImage                      m_ImgFive;                          //��С
	CGameImage                      m_Num;                              //ţ����
	CGameImage						m_imgOpenCard;						//̯��
	CGameImage                      m_imgScoreRule;                     //���ֹ���ʾ��ͼ
	__int64							m_iNotePoint[PLAY_COUNT];			//�û�Ѻע��
	__int64							m_iNoteTotalPoint;					//��ע
	__int64							m_iNoteLimit;						//��������
	__int64                         m_iMingLImit;                       //��ע���
	int                             m_iAnLImit;
	__int64                         m_iBaseNote;                        //��ע
	int								m_iThisTurnNote[PLAY_COUNT];		//�������Ѻע����
	int								m_iPass[PLAY_COUNT];				
	BOOL                            m_bMing[PLAY_COUNT];                //����
	BOOL                            m_bWait;                            //���ȴ�ͼƬ
	
	/// ���������ļ����ã��Ƿ���ʾ����ͷ��
	BOOL		m_bShowLogo;
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
	bool		m_bClass3DInfo;

	CString     m_strMoneyType;									//�������

	CUpGradeGameCard				m_SendCardAnimal[4];		//���ƶ���ʹ�õ���
	CUpGradeGameCard				m_FlashCard;				//���˿˶���Ч��

	ULONG_PTR						m_gdiplusToken;				/**< GDI+*/
	Image*							m_pImgBackground[4];		//����ͼƬ����
	int								m_iCurrentBG;				//��ǰ����ͼƬ
	GameFinishNotify				m_ViFinish;					//��������
	bool							m_bShowResult;				//�Ƿ���ʾ����;
	CString							m_ShapeName[15];

private:
	COLORREF						m_NameColor;				//��ʾ�û���ɫ
	int								m_iJetonCount;				//��������
#ifdef VIDEO
public :
	BOOL					m_bWatch;    ///�Լ��Ƿ�Ϊ��ս
	CGameImage				m_pVideoFrame;    ///��Ƶ����
	CRect					m_rectVideoFrame[MAX_PEOPLE];  ///��Ƶ��������
	BOOL					GetVideoRect(CString  strNickname, RECT& rect, CString& pic);
	CPlayVideo				m_PlayVideo;
#endif
public:
	void SetNameColor(COLORREF rgb)	{m_NameColor=rgb;}
	//��������
public:
	//���캯��
	CClientPlayView();
	//��������
	virtual ~CClientPlayView();
	//���غ���
public:
	//���ý��溯��
	virtual void ResetGameView();
	//�滭���溯��
	virtual void DrawViewFace(CDC * pDC, int iWidth, int iHeight);
	//�����û�����
	virtual void UpdateUserInfo(BYTE bViewStation);
	//λ�õ���
	virtual void FixControlSize(int,int);
	//���»��û�ͷ��
//	virtual void DrawLogo(CDC *pDC,BYTE iViewStation,int iXPos,int iYPos,BOOL bBoy,BOOL bCut=false);
	//���ܺ���
public:
	//���Ʊ������Сͼ
	void DrawBk(CDC *pDC,Image *pImage,int iWidth, int iHeight);
	//������Ϸlogo
	void DrawGameTitle(CDC *pDC,int iWidth,int iHeight);
	void DrawInfo(CDC *pDC);
	void TransparentBlt2( HDC hdcDest,        // Ŀ��DC
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
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	//���ñ���ɫ
	void SetBackBmp(UINT uResourceID, TCHAR * szFileName=NULL, BOOL bResource=TRUE);
	//����ׯ��λ��
//	void SetNTStation(int iViewStation=-1);
	//���ñ���λ��
	void SetShowNtStation(int iViewStation=-1, int iHuaKind=0);
	//������Ϸ����
	void SetGamePoint(int iGamePoint=0, int bMeSide=-1);
	//�滭 NT ��־
	//void DrawNt(CDC * pDC, int x, int y);
	//�滭 NT ͼ��
	//void DrawNtLogo(CDC * pDC, int x, int y, int iHuaKind);
	//�滭���Ʊ�־
	void SetMing(BYTE iViewStation,BOOL iGiveup);
     //�����Ƿ��ȴ�ͼƬ 
	void SetWait(BOOL wait=0);

	DECLARE_MESSAGE_MAP()
	//��ť��Ϣ����
protected:
	//��ʼ��ť
	void OnHitBegin();
	//������ť
	void OnHitContinue();
	//�й�
	void OnHitAuto();
	//��ǰֹͣ
	void OnHitLeave();
	//�����˿�
	//void OnHitLastTurn();
	//���ư�ť
	//void OnHitLookBack();
	//����Ҫ��
	void OnHitThing();
	//��ǰֹͣ
	void OnHitStop();
	//��ע
	void OnNote();
	////��
    void OnOver();
	//Ҫ��
	void OnReset();
	//��Ҫ
	void OnBull();
	//�ӱ�
	void OnOpen();
	//
	void OnHitCallScore(UINT uID);

	//����
	void OnChangeShin();

	void SetAuto(int bDeskStation,bool bAuto);
	//���й�ͼ��
	void DrawAutoLogo(CDC * pDC, int x, int y);
public:
//	static CBitmap				m_Back_Center;
//	static BITMAP				m_Back_Center_Info;
//	static CBitmap				m_Bitmap_Num;
	BOOL						m_bRightPower;
	BOOL						m_bMeSide;					//�Ƿ񱾷��÷�
	int							m_iGamePoint;				//��Ϸ����
	BOOL						m_bAuto[PLAY_COUNT];		//�й�״̬
	//����Ѻע��
	void SetNotePoint(BYTE iViewStation,__int64 iNotePoint=0);
	//���ñ�����ע��
	//void SetThisTurnNotePoint(BYTE iViewStation,int iThisTurnPoint=0);
	//���ӱ�����ע��
	//void AddThisTurnNotePoint(BYTE iViewStation,int iThisTurnPoint);
	//��ע
	void AddNotePoint(BYTE iViewStation,__int64 iNotePoint);
	//��ע
	//void AddTotalNote(BYTE iViewStation,int iNoteTotal);
	//������ע
	//void SetTotalNote(BYTE iViewStation,int iNoteTotal);
	//�������ע
	void SetMingLImit(int limit,int an=0);
	//����Ѻע��
	//void DrawNote(CDC *pDC);
	//���ñ���Ѻע����
	void SetLimitNote(BYTE iViewStation,__int64 iNoteLimit,__int64 iBaseNote);
	//����
	void DrawNum(CDC * pDC, int iXPos, int iYPos, UINT Num,int offset,BOOL bThousand=false);
	//����
	void DrawSmallNum(CDC * pDC, int iXPos, int iYPos, int Num,int offset,BOOL bIsFourOrSix=FALSE/*��ʾ��λ������λ*/,BOOL bIsPlus=TRUE,/*�Ƿ�Ϊ����*/BOOL bThousand=FALSE/*�Ƿ��õ�K*/);	
	//��ͷ������ע��
	void DrawScore(CDC * pDC, int x, int y);
	//����
	void DrawPass(CDC *pDC);
	//������
	void DrawMing(CDC *pDC);
   //���ȴ�
	void  DrawWait(CDC *pDC, int iWidth,int iHeight);
	//
	//����
	void DrawGreenNum(CDC * pDC, int iXPos, int iYPos, int Num,int offset,BOOL bIsFourOrSix=FALSE/*��ʾ��λ������λ*/,BOOL bIsPlus=TRUE,/*�Ƿ�Ϊ����*/BOOL bThousand=FALSE/*�Ƿ��õ�K*/);
	//����
	BOOL DrawGetPoint(CDC *pDC);

	//���÷���
	void SetPass(BYTE iViewStation,int iGiveup);
	//����ׯ��
	void SetNTStation(int iViewStation = -1);
	//��ׯ��
	void  DrawNt(CDC *pDC);
public:
	//��ʼ����
	void BeginAnimal(void);
	//��ע����,iViewStationλ��
	void NoteAnimal(BYTE iViewStation);
	//���Ʊ���ͼ
	void DrawBackPic(CDC* pDC,int iWidth,int iHeight);
	//��ʼ��ĳλ�ö�����ʼ����
	void SetNoteAnimalPoint(BYTE iViewStation);
	//���ñ��������
	//void SetAddNote(int iAddNote){m_iThisTurnAddNote=iAddNote;}
	//��ʼ��������
	CPoint m_iNoteAnimalPointStart[PLAY_COUNT];
	//�ռ�������ʼ����
	CPoint m_PtCollectJetonStart[PLAY_COUNT];
	//������ʼ���궯��
	void SetNotePointStart(BYTE iViewStation);
	//���ƶ���
	void SendCardAnimal(BYTE iViewStation, bool split = false);
	//���ƶ����ƿ�ʼλ��
	CPoint m_iSendCardAnimalPointStart[PLAY_COUNT];
	//���Ƹ�λ����ʼ����
	CPoint m_PtSendCardAnimalStart[PLAY_COUNT];
	//���ƶ����и�λ���յ�����;		
	CPoint	m_PtSendCardAnimalEnd[PLAY_COUNT];
	//����λ�õķ��ƶ����յ�
	CPoint	m_PtSendCardAnimalEndSplit[PLAY_COUNT];
	//���÷��ƶ�����ʼλ��
	void SetSendCardPointStart(BYTE iViewStation);
	//�����ռ�������ʼ����
	void SetCollectJetonStart();
	//�ռ����붯��
	void CollectJetonAnimal();
	//���üӳ�����
	void SetAddJeton(int iBaseNote);
	//��ʾ�м�һ�ѳ���
	void ShowAllNote();
	//���ó������
	void SetAddTimes(int iTimes);
	//���ÿ�ʼ����λ��
	void SetBeginAnimalPos();
	//��ʼ�����ɱ�����
	CPoint m_PtBeginAnimal;
	//���������
	void SetGetPoint(BYTE iViewStation,int iGetPoint);
	//�����Ʒ������
	void SetGetPointSplit(BYTE iViewStation,int iGetPoint);
	//������
	void  DrawShape(CDC *pDC,int x,int y,int shape);
	//��������
	void SetShape(BYTE iViewStation,int Shape);
	//����ע��ť
	void CClientPlayView::DrawNoteBt(CDC *pDC);
	//��̯�Ʊ�ʶ
	void DrawOpenCard(CDC *pDC);
	void SetDrawNote(bool draw = false);
	void OnNote(int type);
	void DrawMoney(CDC *pDC);
	/// ����ǰ��ұ��ʵõ���ʾ�ַ���
	void GetNumString(__int64 nNum, CString &Str, int nPower);
	// ���ƶ���
	void SendCard(BYTE byViewSeat, int iTime);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	//����ͷ���
	void DrawLogoBJ(int iView,CDC *pDC,int iXPos,int iYPos);
};
