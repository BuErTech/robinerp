/** @file	MJTimerMgr.h
 *	@brief	ʱ�������
 *	@date 08.08.04
 */
#ifndef ___AFX_MJTIMERMGR_H__E6EAD12F_9702_428F_8B64_0D7CE606EC71__INCLUDED___
#define ___AFX_MJTIMERMGR_H__E6EAD12F_9702_428F_8B64_0D7CE606EC71__INCLUDED___

#include "MJFrameBaseData.h"
#include "gameeventimpl_t.h"

#ifdef WINDOWS_LOGIC
#include "gameprocess_i.h"
#endif

#include "TimerCall_i.h"

namespace MyGame
{

#ifndef WINDOWS_LOGIC
/**
 * @ingroup MJFrame
 * @brief CSoMJTimerMgr
 *
 * <p>�� 
 *		CSoMJTimerMgr, SOʱ�������
 * </p>
 * @sa
 */	
class CSoMJTimerMgr :public ISoTimerCall
{
public:
	CSoMJTimerMgr();
	virtual ~CSoMJTimerMgr();

    virtual int OnSoTimer(int cuBeginTime);

	/**
	 *  @brief ����ʱ��
	 *  @param[in]  nTimerID ʱ���ID
	 *  @param[in]  nTime ʱ���ʱ��
	 *  
	 *  SOʼ��ֻ����һ��ʱ��ID
	 */	
	void SetTimer(int nTimerID, int nTime);

	/**
	 *  @brief ֹͣʱ��
	 *
	 *  ʱ�䲢û��������ֹͣ��ֻ��ID������Ϊ0
	 */	
	void KillTimer();

    /**
    *  @brief ��õ�ǰ�������еĶ�ʱ����ID
    */
    int GetTimerID();

    /**
    *  @brief ��õ�ǰ�������еĶ�ʱ����ʣ��ʱ�䣬 �˹�����ʱû��ʵ��
    */
    int GetRemainTime();

protected:
	/**
	 *  @brief ʱ�䵽�˺���¼�����
	 */	
	virtual void OnTimeOut( int nTimerID );

    UINT  m_nTimerID;
    int   m_cuTimerRemain;
    int   m_SetRemain;
};

#else
/**
 * @ingroup MJFrame
 * @brief ʱ��ID��ӳ��
 *
 *  ��ΪҪ֧�ֶ�������֮���ʱ�䣬���Բ���SO��ô��һʱ�䣬��Ҳ�и�ID����ӳ���߼�TIMER��SOһ��
 */	
typedef struct tagMJTIMER_ID_MAP
{
	int dwTimerID;	// �����ID
	int nTime;	// �ڲ�ID
}MJTIMER_ID_MAP;

/**
 * @ingroup MJFrame
 * @brief CGameMJTimerMgr
 *
 * <p>�� 
 *		CGameMJTimerMgr, �ͻ���ʱ�������
 * </p>
 * @sa
 */	
class SERAPI CGameMJTimerMgr
{   
public:
	CGameMJTimerMgr();
	virtual ~CGameMJTimerMgr();

    void Initialize(IGameProcess* pProcesss);
	/**
	 *  @brief ������Ϸʱ��
	 *  @param[in]  dwTimerID ʱ���ID���ڲ���ʱ��ID��
	 *  @param[in]  nElapse ʱ��ļ��,��λΪ����
	 */	
	void SetGameTimer(UINT dwTimerID,UINT nElapse); // ��λΪ����

	/**
	 *  @brief ֹͣʱ��
	 *  @param[in]  dwTimerID ʱ���ID
	 */	
	void KillGameTimer(UINT dwTimerID);  

	/**
	 *  @brief ʱ����Ӧ���¼�����,�ⲿʹ�ã� �ڲ������˶�ʱ�����ڲ�ʱ�䵽�ˣ�����������ʹ�ⲿ�õ���Ӧ�����²�ʱ��Ҫ����֮
	 *  @param[in]  dwGameTimerID ʱ���ID
	 *
	 *  �ⲿֱ�����������
	 */	
	virtual void OnGameTimer(UINT dwGameTimerID);

	/**
	 *  @brief ����ID��ʱ��
	 *  @param[in]  nTime ��ʾ��ʱ��
	 *  @param[in]  bVisible �Ƿ���ʾ
	 *  @param[in]  bStart �Ƿ�ʼ��ʱ
	 *  @param[in]  bInCrease �Ƿ���������ʾʱ��
	 *
	 *  �˺�����Ҫ�ⲿ����ʵ��, ������������ڲ����õģ��ⲿʵ�֣������ڲ���ʼ��ʾ���Ƶȴ�ʱ�䣬
     *  �ڽ����Ͼ�Ҫ��ʾ����
	 */	
	virtual void StartClock(int nTime, BOOL bVisible = TRUE, BOOL bStart = TRUE, BOOL bInCrease = FALSE) {};

	/**
	 *  @brief �����߼�ʱ��
	 *  @param[in]  nLogicTimerID �߼�ʱ��ID
	 *  @param[in]  nTimer �߼�ʱ��ʱ��
	 */	
	void SetLogicTimer( int nLogicTimerID, int nTimer );

	/**
	 *  @brief ֹͣ�߼�ʱ��
	 *  @param[in]  bVisible �Ƿ�Ҫ��ʾʱ��
	 */	
	void KillLogicTImer( BOOL bVisible = TRUE );

	/**
	 *  @brief ˢ���߼�ʱ��
	 * 
	 *  ���ʱ�����,��ִ��TimeOut
	 */	
	virtual void RefreshLogicTimer();

	/**
	 *  @brief �߼�ʱ��ʱ�䵽��ִ�е��¼�
	 *  @param[in]  nLogicTimerID ������ʱ��ID��
     *  �˷�����Ҳ�Ǹ��ⲿʵ�ֵģ��緢�Ƶȴ�ʱ���ѵ�������������ⲿʵ���߼ӵ�ת��
	 */	
	virtual void OnLogicTimeOut( int nLogicTimerID ){};

	/**
	 *  @brief ���ȫ����ʱ��
	 */	
	void ClearAllTimer();

protected:
	MJTIMER_ID_MAP m_tagIDMap[MJ_TIMERID_MAP_NBR];
	int  m_nLogicTimerID;	// �߼�ʱ��ID
	int  m_nLogicGameTimer;	// �߼�ʱ�� TimeOutʹ��
	IGameProcess*  m_TimerMgrProcesss;                     //�˱���һ��Ҫ�ڳ�ʼʱ������ֵ��
};
#endif

}

// #include "MJTimerMgr.inl"

#endif // !defined(AFX_MJTIMERMGR_H__E6EAD12F_9702_428F_8B64_0D7CE606EC71__INCLUDED_)
