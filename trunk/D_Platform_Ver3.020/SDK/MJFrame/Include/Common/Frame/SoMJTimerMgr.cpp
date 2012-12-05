#include "MJTimerMgr.h"

using namespace MyGame;

CSoMJTimerMgr::CSoMJTimerMgr()
{
    m_nTimerID = 0;
    m_cuTimerRemain = 0;
    m_SetRemain = 0;
}

CSoMJTimerMgr::~CSoMJTimerMgr()
{
    
}

int CSoMJTimerMgr::OnSoTimer(int cuBeginTime)
{
    if (m_nTimerID !=0)
    {
        if (m_cuTimerRemain <= 0)
        {
            UINT cuTimerID = m_nTimerID;
            
            //����ڲ�û���ٴ����ã���������Ҫ�Ѷ�ʱ�������
            //����ڲ����ٴ������˶�ʱ�������ʱ��ֵ����Ϊ0
            //ʹ���������kill��
            KillTimer();    

            OnTimeOut(cuTimerID);
        }
        else
        {
            m_cuTimerRemain -- ;
        }
    }

    return 0;
}

void CSoMJTimerMgr::SetTimer(int nTimerID, int nTime)
{
    if ((nTimerID == 0) || (nTime < 0))
    {
        return;
    }

    m_nTimerID = nTimerID;
    m_cuTimerRemain = nTime;
    m_SetRemain = nTime;
}

void CSoMJTimerMgr::KillTimer()
{
    // ���
    m_cuTimerRemain = 0;
    m_SetRemain = 0;
    m_nTimerID = 0;
}

void CSoMJTimerMgr::OnTimeOut( int nTimerID )
{
    // ����ķ����õ�
}

/**
*  @brief ��õ�ǰ�������еĶ�ʱ����ID
*/
int CSoMJTimerMgr::GetTimerID()
{
    return m_nTimerID;
}

/**
*  @brief ��õ�ǰ�������еĶ�ʱ����ʣ��ʱ�䣬 �˹�����ʱû��ʵ��
*/
int CSoMJTimerMgr::GetRemainTime()
{
    return m_cuTimerRemain;
}




