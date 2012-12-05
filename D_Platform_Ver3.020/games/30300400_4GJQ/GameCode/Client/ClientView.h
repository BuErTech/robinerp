#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "GameNoteWnd.h"
#include "PlaySetDlg.h"
#include "GameInfoDlg.h"
#include "HaveThing.h"
#include "LeaveMessage.h"
#include "Stop.h"
#include "StopMessage.h"
#include "jqboard.h"
#include "MsgBox.h"
#include "Result.h"

class CUpGradeDlg;
class CUpGradeView;

/// �ؼ� ID
#define IDC_GAME_BEGIN				120				///< ��ʼ��ť
#define IDC_CONTINUE				124				///< ������ť
#define IDC_HAVE_THING				127				///< ����Ҫ��
#define IDC_STOP					128				///< ֹͣ����
#define IDC_FINISH_DIAODU			130				///< ���ư�ť
#define IDC_SAVE_BUZHEN				131				///< ����Ҫ��
#define IDC_LOAD_BUZHEN				132				///< ֹͣ����
#define IDC_LEAVE					133				///< ֹͣ����
/// �����ͼ��
class CClientPlayView : public CUsingSkinClass//CGDIPlayView
{
	///CRect m_rcMjDesk; ////�齫��ʾ������
	void DrawUserMj(CDC * pDC, int iClientWidth, int iClientHeight);
	void DrawNt(CDC * pDC, int x, int y);
	/// �������
protected:
	UINT m_imaxelaspse;
	
	CPoint				m_PtNt[PLAY_COUNT];					///< �û�����λ��
	CPoint              m_PtGif[PLAY_COUNT];        ///< ���߶���
	/// �ؼ�����
public:
	int			m_widthVideo;
	int			m_heightVideo;
	void SetCheckTimer(int maxelaspse);

	void KillCheckTimer();
	CJqBoard m_Logic;
	void Init();
	
	CBitmap				m_NT;						///< NT ͼ��
	CGameInfoDlg		m_GameInfo;					///< ��Ϸ��Ϣ
	CPlaySetDlg			m_GameSet;					///< ��Ϸ����
	CGameNoteWnd		m_GameNoteView;				///< ��Ϸ��Ϣ
	CNormalBitmapButtonEx		m_btContinue;		///< ������ť
	//CNormalBitmapButtonEx		m_btleave;			///< �뿪
	CNormalBitmapButtonEx		m_btThing;			///< ����Ҫ��
	CNormalBitmapButtonEx		m_btStop;			///< ֹͣ����

	CNormalBitmapButtonEx m_btFinishDiaoDu;//.Create(TEXT("��ɵ���"),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,25,25),this,IDC_FINISH_DIAODU);
	CNormalBitmapButtonEx m_btLoadBuZhen;//.Create(TEXT("���벼��"),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|WS_DISABLED|BS_OWNERDRAW,CRect(0,0,25,25),this,IDC_LOAD_BUZHEN);
	CNormalBitmapButtonEx m_btSaveBuZhen;

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

	bool		m_bFirstGame;						///��һ�ν�������

	CLeaveMessage		m_ThingMessage;				///< �뿪��Ϣ
	CHaveThing			m_HaveThing;				///< �����뿪
	CStop				m_StopThing;
	CStopMessage		m_StopMessage;
	CMsgBox				m_msgbox;					///< ��ʾ��Ϣ��
	CMsgBoxResult       m_Result;                   ///< ��Ϸ�����
	CGameImage			m_PicTopBar;				///< ������ͼ
	CGameImage			m_PicTopBarLeft;			///< ������ͼ
	CGameImage			m_PicTopBarRight;			///< ������ͼ


	bool							m_bUpdateScoreboard;			///< ���»��ְ�
	/// ��������
public:
	/// ���캯��
	CClientPlayView();
	/// ��������
	virtual ~CClientPlayView();
public:
	/// /�����ǳƵ������Ƶ���ھ���д�����rect,������TRUE;û�ҵ�����falst
	BOOL	GetVideoRect(CString  strNickname, RECT& rect, CString& pic);

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
	/// ��ʾ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	/// ��Ϣ����
protected:
	/// ��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	/// ���ñ���ɫ
	void SetBackBmp(UINT uResourceID, TCHAR * szFileName=NULL, BOOL bResource=TRUE);
	/// ����ׯ��λ��
	void SetNTStation(int iViewStation=-1);
	/// ������Ϸ����
	void SetGamePoint(int iGamePoint=0, int bMeSide=-1);
	
	DECLARE_MESSAGE_MAP()
	/// ��ť��Ϣ����
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	/// ��ʼ��ť
	void OnHitBegin();
	void OnHitFinishDiaoDu();
	void OnHitSaveBuZhen();
	void OnHitLoadBuZhen();	
	/// ������ť
	void OnHitContinue();
	void OnHitLeave();
	/// ����Ҫ��
	void OnHitThing();
	/// ��ǰֹͣ
	void OnHitStop();
public:
	static CBitmap				m_Bitmap_Num;
	BOOL						m_bRightPower;
	afx_msg void OnTimer(UINT nIDEvent);

public :

#ifdef	VIDEO
	BOOL					m_bWatch;						///< �Լ��Ƿ�Ϊ��ս
	CGameImage				m_pVideoFrame;					///< ��Ƶ����
	CRect					m_rectVideoFrame[MAX_PEOPLE];   ///< ��Ƶ��������
	BOOL					GetVideoRect(CString  strNickname, CRect& rect);
	CPlayVideo				m_pPlayVideo;
#endif
};
