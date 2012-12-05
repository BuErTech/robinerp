#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "ClientView.h"
#include "UpGradeLogic.h"

//��Ϣ����
#define IDM_BEGIN			WM_USER+120					//��ʼ��ť��Ϣ
#define IDM_USER_NT			WM_USER+121					//�û�����
#define IDM_GIVE_BACK		WM_USER+123					//�����ư�ť
#define IDM_OUT_CARD		WM_USER+124					//�û�����
#define IDM_LOOK_LAST		WM_USER+125					//�������˿�
#define IDM_BACK_CARD		WM_USER+126					//������Ϣ
#define IDM_HAVE_THING		WM_USER+127					//����Ҫ��
#define IDM_SEND_THING		WM_USER+128					//�����뿪����
#define IDM_ARGEE_LEAVE		WM_USER+129					//ͬ���뿪��Ϣ

//��ʱ�� ID
#define ID_BEGIN_TIME				100					//��ʼ��ʱ��
#define ID_WAIT_ROB					101					//�ȴ�������
#define ID_GIVE_BACK				102					//�ȴ�������
#define ID_OUT_CARD					103					//���Ƶȴ�ʱ��
#define ID_SHOW_LAST				104					//�鿴�����˿�
#define ID_SHOW_CARD				105					//˦�ƶ�ʱ��
#define ID_LOOK_BACK				106					//�鿴����

//���ú�������
#define RS_ALL_VIEW					1					//�������в���
#define RS_GAME_CUT					2					//�û��뿪����
#define RS_GAME_END					3					//��Ϸ��������
#define RS_GAME_BEGIN				4					//��Ϸ��ʼ����

//��Ϸ����� 
class CClientGameDlg : public CGameFrameDlg
{
	//��������
public:
	//�̶���Ϣ
	BYTE					m_iLessCount;				//������Ϸ����
	BYTE					m_iBeginStation;			//��ʼλ��
	BYTE					m_iEndStation;				//����λ��
	BYTE					m_iCardCount;				//������Ϸ���˿���Ŀ
	BYTE					m_iPalyCardCount;			//��Ϸ�˿���Ŀ

	//�Լ��˿�����
	BYTE					m_iMeCardCount;				//�Լ��˿˵���Ŀ
	BYTE					m_iMeCardList[45];			//�Լ��˿��б�

	//������Ϣ
	BYTE					m_iNtCount;					//�������˿���Ŀ
	BYTE					m_iNtPeopole;				//��������Ϸ��
	BYTE					m_iBackCard[8];				//�����˿�

	//������Ϣ
	BYTE					m_iDeskCardCount[4];		//ÿ�������˿˵���Ŀ
	BYTE					m_DeskCard[4][39];			//ÿ��������˿�

	//������Ϣ
	BYTE					m_iNowOutPeople;			//���ڳ�����
	BYTE					m_iBaseCount;				//��һ������������Ŀ
	BYTE					m_iFirstOutPeople;			//��һ��������
	BYTE					m_iBaseCard[39];			//��һ�������ߵ��˿�

	//������ʾ����
	BYTE					m_iLastCardCount;			//�����˿˵���Ŀ
	BYTE					m_bShowLast;				//�Ƿ������ʾ�����˿�
	BYTE					m_bShowIngLast;				//�Ƿ�����ʾ�����˿�
	BYTE					m_iLastOutCard[4][39];		//���ֵ��˿�

	////��ʱ������
	//int						m_iTimerStation;			//��ʱ��λ��
	//int						m_iTimerCount;				//������ʣʱ��

	//�ؼ�����
public:
	CUpGradeGameLogic		m_Logic;					//�����߼�
	CClientPlayView			m_PlayView;					//��Ϸ��ͼ

	//��������
public:
	//���캯��
	CClientGameDlg();
	//��������
	virtual ~CClientGameDlg();

protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//���ݰ󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	virtual void OnWatchSetChange(void);
	virtual void OnGameSetting();

	//���غ���
public:
	//������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize);
	//��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�������ý���
	virtual void ResetGameFrame();
	//��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount);
	//������ж�ʱ��
	void KillAllTimer();
	//ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin();
	//��ȫ������Ϸ
	virtual bool OnControlSafeFinish(){return false;};

	//��Ϣ����
public:
	//��ʼ��ť����
	LRESULT	OnHitBegin(WPARAM wparam, LPARAM lparam);
	//NT ��ť����
	LRESULT	OnHitNt(WPARAM wparam, LPARAM lparam);
	//�����˿˰�ť����
	LRESULT	OnHitCard(WPARAM wparam, LPARAM lparam);
	//�����ư�ť����
	LRESULT	OnHitBackCard(WPARAM wparam, LPARAM lparam);
	//�������ư�ť����
	LRESULT	OnHitOutCard(WPARAM wparam, LPARAM lparam);
	//�������ְ�ť����
	LRESULT	OnHitLastTurn(WPARAM wparam, LPARAM lparam);
	//���������ư�ť����
	LRESULT	OnHitLookBack(WPARAM wparam, LPARAM lparam);
	//�������°�ť����
	LRESULT	OnHitHaveThing(WPARAM wparam, LPARAM lparam);
	//�����뿪������
	LRESULT	OnSendHaveThing(WPARAM wparam, LPARAM lparam);
	//�û������뿪
	LRESULT	OnArgeeUserLeft(WPARAM wparam, LPARAM lparam);
	//�Ҽ�����
	LRESULT	OnRightHitCard(WPARAM wparam, LPARAM lparam);

	BOOL UpdateNtList();
	void ResetGameStation(int iGameStation);

	DECLARE_MESSAGE_MAP()
};
