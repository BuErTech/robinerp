#pragma once

#include "StdAfx.h"
#include "Resource.h"
//#include "StationView.h"
//#include "NtList.h"
#include "GameNoteWnd.h"
#include "PlaySetDlg.h"
#include "GameInfoDlg.h"
//#include "GameEndInfoDlg.h"
//#include "BackCardDlg.h"
#include "HaveThing.h"
#include "LeaveMessage.h"
#include "GameCard.h"
#include "Stop.h"
#include "StopMessage.h"
#include "ChessManual.h"
#include "AdmitDefeat.h"
#include "RegretChess.h"
#include "DianMu.h"
#include "MsgBox.h"
#include "GameFinishDlg.h"
#include "GameSet.h"

class CUpGradeDlg;
class CUpGradeView;

//�����ͼ��
class CClientPlayView : public CUsingSkinClass
{

#ifdef	VIDEO
	public:
		CRect				m_RtVideo[2];				//�û�����Ƶ����λ��
#endif

		//���ڶ�����ʾ
	CPoint                          m_PtGif[PLAY_COUNT];


	//�ؼ�����
public:
	CUpGradeGameCard			m_MeCard;					//����

	CGameSet					m_GameSetDlg;
	CPlaySetDlg					m_GameSet;					//��Ϸ����
	CGameInfoDlg				m_GameInfo;					//��Ϸ��Ϣ
	CGameNoteWnd				m_GameNoteView;				//��Ϸ��Ϣ
//	CStationView				m_StationView;				//״̬����
	CMsgBox						m_msgbox;					//��ʾ��Ϣ��
	CLeaveMessage				m_ThingMessage;				//�뿪��Ϣ
	CHaveThing					m_HaveThing;				//�����뿪
	CStop						m_StopThing;				//���
	CStopMessage				m_StopMessage;				//��Ͷ�Ӧ
	CAdmitDefeat				m_AdmitDefeat;				//����
	CChessManual				m_ChessManual;				//����
	CRegretChess				m_RegretChess;				//����
	CDianMu						m_DianMu;					//��Ŀ
	CGameFinishDlg				m_Result;					//��Ϸ������Ϣ

	CNormalBitmapButtonEx		m_btContinue;				//������ť
	CNormalBitmapButtonEx		m_btLeave;					//�뿪
	CNormalBitmapButtonEx		m_btStop;					//���
	CNormalBitmapButtonEx		m_btAdmitDefeat;			//����
	CNormalBitmapButtonEx		m_btRegretChess;			//����
	CNormalBitmapButtonEx		m_btChessManual;			//��Ŀ
	CNormalBitmapButtonEx		m_btFinishDianMu;			//��ɵ�Ŀ
	CNormalBitmapButtonEx		m_btThing;					//����Ҫ��
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
	
	virtual void FixControlSize(int,int);
	//���ܺ���
public:
	void TransparentBlt2( HDC hdcDest,      // Ŀ��DC
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
	DECLARE_MESSAGE_MAP()
	//��ť��Ϣ����
public:
	//��ť��Ϣ
	void OnHitButton(UINT uID);

public:
#ifdef	VIDEO
	BOOL						m_bWatch;    ///�Լ��Ƿ�Ϊ��ս
	CGameImage					m_pVideoFrame;	//��Ƶ����
	///�����ǳƵ������Ƶ���ھ���д�����rect,������TRUE;û�ҵ�����falst
	BOOL						GetVideoRect(CString  strNickname, RECT& rect, CString& pic);
#endif

};
