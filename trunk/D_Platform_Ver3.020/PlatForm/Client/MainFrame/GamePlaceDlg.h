#pragma once
#ifndef GAMEPLACEDLG_HEAD_H
#define GAMEPLACEDLG_HEAD_H

#include "StdAfx.h"
#include "Resource.h"
#include "MainRoomEx.h"
#include "GameRoomEx.h"
//#include "LongonDialog.h"
#include "PlaceResource.h"

#include "DUITopWnd.h"

#include "AFCActwnd.h"						//����֪ͨ
#include "AfcTopWnd.h"						//������ť
#include "chatting.h"
#include  "afcgamelistwnd.h"				//��Ϸ������
#include "SkinMgr.h"

#define LEFT_WND_WIDTH				296		//��ߴ��ڴ�С
#define LEFT_WND_TOP				86		//����
#define QUIT_GAMEROOM               WM_USER + 101  ///< ���«2�����޸�������û�������Ϣ

#define IDT_GP_SHOW_WINDOW			1		//ShowWindow����ʱ

/*******************************************************************************************************/

//���ڶԻ���
class CAboutDlg : public CDialog
{
	//��������
public:
	//���캯��
	CAboutDlg();
	virtual BOOL OnInitDialog();
	void OnFeedBack();
	void OnServer();
	afx_msg void OnPaint();
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);   
	CNormalBitmapButtonEx				m_Bt1;

	CGameImage	m_bk;

	DECLARE_MESSAGE_MAP()

private:
	CSkinMgr m_skinmgr;

	void Init();

};

/*******************************************************************************************************/

//���Ի���
class CGamePlaceDlg : public CDialog
{
	CRgn							m_Rgn;								///< �������򣬲��ػ��Ӵ��ڲ���
	POINT							m_SavedScreen;
public:
	bool							m_bMax;								//�Ƿ����
	bool							m_bShowMax;
	//��������
public:
//	BOOL					//m_bRegisterHot;							//�Ƿ�ע���ȼ�
//	CChattingWnd			m_ChattingWnd;								//���䶥����
	CTopWndClass			m_topWnd;									//���䶥����AFC��
	CTopDuWndClass			m_topDuWnd;									//���䶥����DirectUI��
	CString					m_strTopWndRander;							//���䶥��������Ⱦ��ʽ\
																			��ʹ��AFC����DirectUI\
																			Added By JianGuankun 2012.3.20

	CActWnd					m_ActInformWnd;								//����֪ͨ
//	CBottomWndClass			m_BottomWnd;								//���䶥����
	CGameListWnd			m_GameListWnd;								//��Ϸ������
	CNewGameClass			m_RoomTabWnd;								//���� TAB �ؼ�
	CMainRoomEx				* m_pMainRoom;								//��������
	//////////////////////////////////////////////////////////////////////////
	//�����Ǻ����ֲ����йصĿ��ƣ�Fred Huang 2008-07-24
	int								m_bgHallSoundID;
	int								m_bgRoomSoundID;
	int								m_bgCurrentID;

	CNormalBitmapButtonEx	m_btSndPlay;
	CNormalBitmapButtonEx	m_btSndPause;
	CNormalBitmapButtonEx   m_btVolDown;
	CNormalBitmapButtonEx	m_btVolUp;
#ifndef DEV_NOFREE
	CNormalBitmapButtonEx   m_btTechSupport; //����֧��	add by lxl 2011-1-23
#endif

	CToolTipCtrl            m_toolTip;

	int						m_sndBtnCx,m_sndBtnCy;

	int sndPlayX,sndPlayY;
	int sndPauseX,sndPauseY;
	int sndVolUpX,sndVolUpY;
	int sndVolDownX,sndVolDownY;
	int techSupportX, techSupportY;

	void OnSndPlay();
	void OnSndPause();
	void OnSndVolDown();
	void OnSndVolUp();
	void OnTechSupport();
	
	bool m_bSndPause;
	int	 m_sndVolHall,m_sndVolRoom;
	//////////////////////////////////////////////////////////////////////////

	CGameImage						m_lc,m_rc,m_tc,m_bc;	//���ͼƬ	
	CGameImage						m_lt,m_rt,m_lb,m_rb,m_rbext;	//�߽�	
public:
	//����ͼƬ
	void LoadSkin(void);
	//HICON hIcon; 
	//��������
public:
	//���캯��
	CGamePlaceDlg();
	~CGamePlaceDlg();
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ػ�����
	afx_msg void OnPaint();
	afx_msg void OnMove(int x,int y);
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);
	//ȷ������
	virtual void OnOK() { return; }
	//ȡ������
	virtual void OnCancel();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	BOOL ShowWindow(int nCmdShow);

	// add xqm 2011-3-22
	//����Ӳ������
	void SetAcceleration(int iLevel);

	//�������ԭ����Ӳ����������
	int		m_iLevel;
	DECLARE_MESSAGE_MAP()

	//��Ϣӳ��
public:
	//�Ƚ�����
	LRESULT OnHotKey(WPARAM wparam, LPARAM lparam);	
	////����֪ͨ����
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//ϵͳ�˵�
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//λ�ñ仯 
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//�õ�����
	afx_msg void OnSetFocus(CWnd * pOldWnd);

    // PengJiLin, 2011-6-23, ����
    afx_msg void OnActivate(UINT uState, CWnd* pOldWnd, BOOL bMinisted);

	/// 
	/// ��ʱ����Ӧ��Ϣ
	/// @param[in]		UINT_PTR nIDEvent		��ʱ��ID
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	CAboutDlg dlgAbout;
private:
	void DrawViewFrame(CDC * pDC, int iWidth, int iHeight);

//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	///< �˳�����GameRoomʱ�����Ի���
	///< wParam �˳���������
	///< return LRESULT
	LRESULT OnQuitGameRoom(WPARAM wParam, LPARAM lParam);
public:
	void SetDisplays(void);
	//������ʾģʽ
	LRESULT  SetLeftHideOrShow(WPARAM wparam,LPARAM lparam);
	//������ʾģʽ
	LRESULT  SetRightHideOrShow(WPARAM wparam,LPARAM lparam);
	//������
	LRESULT OpenBank(WPARAM wparam,LPARAM lparam);
	//������
	LRESULT OpenIE(WPARAM wparam,LPARAM lparam);

	///����GM rongqiufen 2010-8-10
	LRESULT CallGM(WPARAM wparam,LPARAM lparam);

    //Ͷ��
	LRESULT OpenAppeal(WPARAM wparam,LPARAM lparam);
	LRESULT OnLockAccount(WPARAM wParam, LPARAM lParam);	//�������� zxj 2009-11-12
	
	//�汾�˶�
	BOOL CheckVersion();
	bool SafeMoveWindow(UINT uID, int x, int y, int iWidth, int iHeight);
	//�ƶ�����
	bool SafeMoveWindow(CWnd * pControlWnd, int x, int y, int iWidth, int iHeight);
public:
	afx_msg void OnNcPaint();

	//wushuqun 2009.7.13
	//�޸���Ϸ�������˳���Ϸʱ��������
	afx_msg BOOL OnEraseBkgnd(CDC* pDC)
	{
		return true;
		//
		return CWnd::OnEraseBkgnd(pDC);
	}
private:
	void EnCode_Key(CString &strdest, char *strsource, char key);

public:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);
private:
	CSkinMgr m_skinmgr;
};
#endif
/*******************************************************************************************************/
