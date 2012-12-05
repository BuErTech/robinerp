#include "Stdafx.h"
#include "CardSender.h"

#include "GameMachine.h"
#include "MiniGameEngine.h"


///���ƣ�CCardSender
///���������캯��
CCardSender::CCardSender()
{
	//��Ϸ��״̬��ָ��
	m_pRootMachine = NULL;

	//��Ϸ����ָ��
	m_pGameEngine = NULL;

	//��ʼ����ʾ����
	::memset(m_byShowed,0,MAX_HANDCARDS_NUM);

	//���ƽ���
	m_iSendingPos = 0;

	//��¼��ǰ�Ƿ����ϵĵڼ�����
	m_iSendingIndex = 0;

	//������Ҷ���
	::memset(m_SendCardQueue,255,sizeof(m_SendCardQueue));

}

///���ƣ�~CCardSender
///��������������
CCardSender::~CCardSender()
{

}


//��������ʼ������
bool CCardSender::StartUp(CGameMachine* pRootMachine,BYTE byGameStation,void* pGSData)
{
	//TGSSendingCard* pGSSendingCard = (TGSSendingCard*)pGSData;

	//��¼��Ϸ��״̬��ָ��
	m_pRootMachine = pRootMachine;

	//��¼��Ϸ����ָ��
	m_pGameEngine = pRootMachine->m_pGameEngine;

	//������ҷ��ƶ�������
	LoadSendCardQueue();

	//��ǰ����λ�����0
	m_iSendingPos = 0;

	//��ǰ��������Ϊ0
	m_iSendingIndex = 0;

// 	if (GS_SENDIND_CARD == byGameStation)
// 	{
// 		//���ַ��ƹ���
// 		OnRecurSending(pGSData);
// 	}

	//֪ͨ������л滭���������ڷ��ƵĹ��������ŷ�����Ҫ��ʱ���� \
	  ����һ��ʱ�����ڽ�������GameEngine�ص���Machine�Ĵ�����OnTimerGap()
	if (m_pGameEngine)
	{
		if (0 == byGameStation)
		{
			m_pGameEngine->SetTimer(IDT_SEND_CARD_GAP,500,NULL);
		}
// 		else if (GS_SENDIND_CARD == byGameStation)
// 		{
// 			m_pGameEngine->OnRecurShowSendingCard(pGSSendingCard);
// 			m_pGameEngine->SetTimer(IDT_SEND_CARD_GAP,500,NULL);
// 		}
	}

	return true;
}

//�ر�
bool CCardSender::ShutDown()
{
	//�ͷŷ��ƻ�
	if (m_pGameEngine)
	{
		m_pGameEngine->KillTimer(IDT_SEND_CARD_GAP);
	}

	m_pRootMachine = NULL;
	m_pGameEngine = NULL;
	
	return true;
}

//��ÿ���Ƶ�ʱ�����ѵ�
bool CCardSender::OnTimerGap()
{
	if (!m_pRootMachine || !m_pGameEngine)
	{
		return false;
	}

	if (m_pRootMachine->m_byMeStation == m_SendCardQueue[m_iSendingPos])
	{
		//m_pGameEngine->SetTimer(IDT_SEND_MOVING_GAP,200,NULL);
	}

	//֪ͨ������ʾ///////////////
	m_pGameEngine->OnPlayACardAnimate(m_SendCardQueue[m_iSendingPos],m_iSendingIndex);

	//֪ͨ��Ч�ಥ�ŷ�������///////////////
	m_pGameEngine->PlayBaseSound(DZPKSound::BT_SEND_BACK_CARD);

	m_iSendingPos++;
	if (m_SendCardQueue[m_iSendingPos] == 255)
	{
		m_iSendingPos = 0;
		m_iSendingIndex++;

		if (m_iSendingIndex == 2)
		{
			m_pGameEngine->KillTimer(IDT_SEND_CARD_GAP);

			//֪ͨͨ������й���
			m_pGameEngine->SendGameData(MDM_GM_GAME_NOTIFY,ASS_SEND_CARD_FINISH,0);

			return true;
		}
	}

	//֪ͨ��Ч����в��Ź���
	//m_pGameEngine->PlayBaseSound(DZPKSound::BT_SENDACARD);

	return true;
}

//��ÿ���Ƶ��˶�ʱ��������
bool CCardSender::OnTimerMovingGap()
{
	if (!m_pRootMachine || !m_pGameEngine)
	{
		return false;
	}

	//֪ͨ������ʾ///////////////
	m_pGameEngine->KillTimer(IDT_SEND_MOVING_GAP);

	CString str;
	str.Format("dxy:m_iSendingIndex = %d",m_iSendingIndex);
	OutputDebugString(str);

	if (m_iSendingIndex < 2)
	{
		//m_pGameEngine->OnEndSendMeACard(m_iSendingIndex);
	}
	else
	{
		//m_pGameEngine->OnEndSendMeACard(1);
	}
	
	return true;
}

//��ȡ����ʾ����
void CCardSender::GetShowedCards(BYTE byCard[])
{
	::memcpy(byCard,m_byShowed,m_iSendingPos);
	return;
}

//��Ϸ������״̬����
bool CCardSender::OnRecurSending(void* pGSBase)
{
	//TGSSendingCard* pGSSendingCard = (TGSSendingCard*)pGSBase;

	if (!m_pRootMachine || !m_pGameEngine)
	{
		return false;
	}

// 	if (pGSSendingCard == NULL)
// 	{
// 		return false;
// 	}

	//��������////


	//֪ͨ������ʾ///////////////

	return true;
}

//������ҷ��ƶ�������
bool CCardSender::LoadSendCardQueue()
{
	if (!m_pRootMachine || !m_pGameEngine)
	{
		return false;
	}

	if (m_pRootMachine->m_byNTUser == 255)
	{
		return false;
	}

	int i = 0;
	BYTE byNextUser = 255;

	//��ׯ�ҿ�ʼ
	m_SendCardQueue[i++] = m_pRootMachine->m_byNTUser;
	byNextUser = m_pRootMachine->GetNextUserStation(m_pRootMachine->m_byNTUser);

	while(byNextUser != m_pRootMachine->m_byNTUser)
	{
		m_SendCardQueue[i++] = byNextUser;
		byNextUser = m_pRootMachine->GetNextUserStation(byNextUser);
	}

	return true;
}