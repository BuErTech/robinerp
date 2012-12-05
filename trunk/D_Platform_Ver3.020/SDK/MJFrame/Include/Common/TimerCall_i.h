#ifndef __TIMER_CALL_BACK_I_H__
#define __TIMER_CALL_BACK_I_H__
#include "typeDef.h"
#include "gameevent_i.h"

namespace MyGame
{
    class IWinTimerEvent;

	class ISoTimerCall
	{
	public:
		virtual int  OnSoTimer(int cuBeginTime) = 0;
	};


	class IWinTimer 
	{
	public:
		/**
		 *  @brief ���ò�����ʱ������ʼ��ʱ
		 *  @param dwMilliSeconds ��ʱ�����ʱ�䣨���룩
		 *  @param dwParam ��ʱ���¼��ص�ʱ���صĲ��������û�����
		 *	@param pEvent ��ʱ��������ָ��
		 *  @return ���ض�ʱ��ID���ڱ���ʱ��������Ψһ��
		 */		
		virtual UINT SetTimer(UINT dwMilliSeconds, UINT dwParam, IWinTimerEvent* pEvent) = 0;

		/**
		 *  @brief ��ĳ��ID�Ķ�ʱ�������µļ��ʱ��,��ʱ��������һ������ڼ����и����µļ��ʱ��
		 *  @param dwTimerID ��ʱ��ID
		 *  @param dwMilliSeconds �µļ��ʱ�䣨���룩
		 *  @return �ɹ��򷵻�TRUE��ʧ�ܷ���FALSE��ʧ�ܵ�ԭ��ͨ���Ǹö�ʱ��ID������
		 */	
		virtual BOOL SetTimer(UINT  dwTimerID, UINT  dwMilliSeconds) = 0;

		/**
		 *  @brief ֹͣ��ʱ��
		 *  @param dwTimerID ��ʱ��ID���� SetTimer() ��������
		 */		
		virtual void KillTimer(UINT  dwTimerID) = 0;

		/**
		 *  @brief ����������е����ж�ʱ��
		 *  @sa 
		 */	
		virtual void ClearAll() = 0;
	};

#ifdef WINDOWS_LOGIC

	/**
	 * @ingroup system
	 * @brief WinTimer�¼��ӿ�
	 *
	 * <p>
	 *���� 
	 * </p>
	 *
	 * @sa 
	 */	
	class IWinTimerEvent : public IEvent
	{
	public:
		/**
		 *  @brief ��ʱ���¼��ص�
		 *  @param dwTimerID,��ʱ��ID���� IWinTimer::SetTimer() ��������
		 *  @param dwParam ��ʱ���������� IWinTimer::SetTimer() ��������
		 */		
		virtual void OnTimer(DWORD dwTimerID, DWORD dwParam) = 0;
	};
#endif
}

#endif
