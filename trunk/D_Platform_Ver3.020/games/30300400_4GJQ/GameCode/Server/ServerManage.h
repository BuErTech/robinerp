#pragma once

#include "StdAfx.h"

#include "jqlogic.h"
/// ��ʱ�� ID
///#define TIME_SEND_CARD			30							///< ���ƶ�ʱ��
#define TIME_GAME_FINISH			32							///< ��Ϸ������ʱ��

#define TIME_USER_OUT_CARD          33                          //������嶨ʱ��
/// �˿���Ŀ����
#define PLAY_TWO_CARD				1							///< �����˿�
#define PLAY_THREE_CARD				2							///< �����˿�

/// ��Ϸ������־����
#define GF_NORMAL					10							///< ��Ϸ��������
#define GF_SALE						11							///< ��Ϸ��ȫ����
#define GF_PREEND                   12							///< ��ǰ����

/// ��Ϸ���� H021_UserPass
class CServerGameDesk : public CGameDesk
{
	bool H021_UserPass(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	CJqLogic m_Logic;
	bool H0011_UserBuZhen(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	
	int m_Counter;
	///void H004_SendCard();
	bool H001_GameBegin(BYTE bBeginFlag);
    bool H048_ArgeeStopThing(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
    bool H046_StopThing(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
    bool H043_ArgeeUserLeft(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	bool H040_UserHaveThing(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	bool H020_UserOutCard(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	bool H000_UserAgreeGame(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);

protected:
	/// �����������
	BYTE					m_iLessCount;						///< ������Ϸ����
	BYTE					m_iEnableWatch;						///< �Ƿ������Թ�

	//����״̬����
	int						m_PunishPoint[4];					///< �û�����
	BYTE					m_iBeenPlayGame;					///< �Ѿ���Ϸ�ľ���
	BYTE					m_iLeaveArgee;						///< �뿪ͬ���־
	int                     m_iTimerCount ;                     ///��ǰ�Ķ�ʱ��
	
	bool					m_bQueryQuit;						///< ����������˳�
	int						m_bdesk;							///< �����˳������
	bool					m_bBuzhen[4];						///< ����Ƿ��Ѳ���
	/// ��������
public:
	/// ���캯��
	CServerGameDesk(); 
	/// ��������
	virtual ~CServerGameDesk();

	/// ���غ���
public:
	/// ��Ϸ��ʼ
	int GetPlayerCount();
	/// �õ���һ������ҵ�λ��
	int GetPlayerStation();
	virtual bool GameBegin(BYTE bBeginFlag)
	{
		m_Logic.m_maxPeople=m_bMaxPeople;
		m_Logic.m_iPlayerCount=GetPlayerCount();
		m_Logic.m_iStation=GetPlayerStation();
		m_Counter = 0;
		return H001_GameBegin(bBeginFlag);
	}
	/// ��Ϸ����
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);
	/// �ж��Ƿ�������Ϸ
	virtual bool IsPlayGame(BYTE bDeskStation);
	/// ��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	/// �û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);

	/// ���ش��麯��
public:
	/// ��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser);
	/// ������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation);
	/// ��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	/// ������
public:
	/// �û�������Ϸ
	BOOL UserSetGame(UserArgeeGame * pGameSet);
	/// �����˿˸��û�
	/// /BOOL SendCard();
	/// ��Ϸ��ʼ
	BOOL BeginPlayUpGrade();	
	/// ��һ�ֿ�ʼ
	BOOL NewPlayTurn(int iWiner);
	/// �û������뿪
	BOOL H041_UserHaveThing(BYTE bDeskStation, char * szMessage);
	/// ͬ���û��뿪
	BOOL H044_ArgeeUserLeft(BYTE bDeskStation, BOOL bArgee);
};
































