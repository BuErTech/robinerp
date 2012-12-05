
#include "StdAfx.h"
#include "GameDataObject.h"
#include "../GameMessage/GameAlgorithm.h"
#include "../GameMessage/UpGradeLogic.h"

///���ƣ�CGameDataObject
///��������ʼ���������ã��������͵�
///@param 
///@return 
CGameDataObject::CGameDataObject(void)
{	
	memset(m_UserMoney, 0, sizeof(m_UserMoney));
	m_iDealPeople = 255;
}

///���ƣ���������
///������~CGameDataObject
///@param 
///@return 
CGameDataObject::~CGameDataObject(void)
{

}

///��������ʼ��
void CGameDataObject::Init()
{
	//HalfInit();
	//�����û�����
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		m_UserData[i].Init();
	}

	//����ĳɼ���
	//m_tagResult.Clear();

	//ׯ��
	m_byNTUser = 255;

	memset(m_bNoBet, 0, sizeof(m_bNoBet));
	memset(m_nBetMoney, 0, sizeof(m_nBetMoney));
	memset(m_bGiveUp, true, sizeof(m_bGiveUp));
    memset(m_bResultCards, 0, sizeof(m_bResultCards));
	memset(m_byAllPools, 0, sizeof(m_byAllPools));	
	memset(m_bAllBet, 0, sizeof(m_bAllBet));

	memset(m_iCards, 0, sizeof(m_iCards));
	m_iCardsCount = 0;

	m_nMaxBetMoney = 0;
	m_nBetPool = 0;
	m_iCheckCount = 0;
    m_bRaiseStation = -1;
	m_bBetStation = 255;
    m_bAllBetStation = -1;
	m_bFirstStation = -1;
    m_iBetCounts = 0;
	m_bGiveUpOver = false;
    m_bNewTurn = false;
	m_bNewTurns = false;

    //�û���ǰ�����Ķ������
    m_byVerbFlag = 0x00;
}

///���������ʼ�������ڻغ�֮��������ȫ�Ĳ���������Ϊ�е�������Ҫ������
void CGameDataObject::HalfInit()
{
	//�������Ϸ�����е�����///////////////////////////////////

	//����˿�Դ����
	::memset(m_byCards,255,sizeof(m_byCards));

	//������Ƶ�����
	::memset(m_byBackCards,255,sizeof(m_byBackCards));

	//����λ��
	m_iGetCardPos = 0;

	//�����û�����
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		m_UserData[i].HalfInit();
	}

	//��ǰ��Ϸ���Ȧ��
	m_nTurnNums = 0;

	//�¿���˹�˿���Ϸ���е�����//////////////////////////////////

	//��ǰ����û�
	m_byTokenUser	= 255;
	m_bSmallBlind	= 255;
	m_bBigBlind		= 255;
	// �Ƿ�����Ҽ�ע
	m_bRaise = false;

	// ��¼��ע���
	m_iRaiseCounts = 0;

	// ��¼��ע���
	m_iAllinUser = 0;

	// ��¼��ע���
	m_iCallCounts = 0;

	// ��¼�������
	m_iFoldCount = 0;

	m_bAutoSendCard = false;

	return;
}
															
///�������������Ϊδ֪�Ƿ�ͬ���ע״̬
///@param bDeskStation���λ��
void CGameDataObject::SetUnknowStateTo(BYTE bDeskStation)
{
	m_UserData[bDeskStation].m_nState = CUserDataSvrObject::UNKNOWN;
	return;
}

///�������������Ϊͬ���ע״̬
///@param bDeskStation���λ��
void CGameDataObject::SetAgreeStateTo(BYTE bDeskStation)
{
	m_UserData[bDeskStation].m_nState = CUserDataSvrObject::AGREE_DESKCFG;
	return;
}

///�������������Ϊ��ͬ���ע״̬
///@param bDeskStation���λ��
void CGameDataObject::SetDisAgreeStateTo(BYTE bDeskStation)
{
	m_UserData[bDeskStation].m_nState = CUserDataSvrObject::DISAGREE_DESKCFG;
	return;
}

///������Ϊĳ����ҷ���
///@param bDeskStation���λ�ã�����籣֤��Խ�磻iSendNum����������
///@return 
bool CGameDataObject::DealCardsTo(BYTE bDeskStation,int iSendNum)
{
	BYTE byGetPai = 0;
	CUserDataSvrObject* pUserData = &m_UserData[bDeskStation];
	
	for (int i = 0; i < iSendNum; ++i)
	{
		if (m_iGetCardPos < MAX_CARDS_NUM)
		{
			byGetPai = m_byCards[m_iGetCardPos++];
		}

		pUserData->m_byMyCards[pUserData->m_iCardsNum++] = byGetPai;
	}
	
	return true;
}

///���ƣ�GetAgreeNums
///�������õ��ж��ٸ����ͬ����Ϸ��ͬ���ע��
int CGameDataObject::GetAgreeNums()
{
	int iAgreeConuter = 0;
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_UserData[i].m_nState == CUserDataSvrObject::AGREE_DESKCFG)
		{
			iAgreeConuter++;
		}
	}

	return iAgreeConuter;
}

//�õ��ж��ٸ�����ѷ�����
bool CGameDataObject::IsAllPlayerSendCardFinish()
{
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_UserData[i].m_iCardsNum > 0)
		{
			if (!m_UserData[i].m_bSendCardFinish)
			{
				return false;
			}
		}
	}
	return true;
}

//��ȡ���ҵ�����
int CGameDataObject::GetActivePlayerNums()
{
	int iNums = 0;

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_UserData[i].m_iCardsNum > 0)
		{
			iNums++;
		}
	}

	return iNums;
}

//��ȡ��ע���
__int64 CGameDataObject::GetBetMoney(BYTE bDeskStation, bool bAll)
{
	int i, j = 0;
	__int64 nCount = 0;

	if (bAll)
	{
		if (bDeskStation == 255)
		{
			// ��ȡ���������ע���
			for (i = 0; i < PLAY_COUNT; i++)
			{
				for (j = 0; j < 8; j++)
				{
					nCount += m_nBetMoney[i][j];
				}
			}
		}
		else 
		{
			// ��ȡ��ҽ��
			for (i = 0; i < 8; i++)
			{
				nCount += m_nBetMoney[bDeskStation][i];
			}
		}
	}
	else
	{
		// ��ȡָ���ı߳����н��
		for (i = 0; i < PLAY_COUNT; i++)
		{
			nCount += m_nBetMoney[i][bDeskStation];
		}
	}

	return nCount;
}

///���ƣ�SetVerbs
///������������ҵĶ���
///@param pCardsȡ�õ���ֵ���ڴ�
///@return
void CGameDataObject::MakeVerbs(BYTE byFlag)
{
    m_byVerbFlag = byFlag;
}

//��һ�ú���
void CGameDataObject::GetGoodCard(BYTE bDeskStation)
{
	//��һ�û�л������ ֱ�ӷ���
	if(m_iGetCardPos <= 0)
	{
		return;
	}

	BYTE byDeskCard[5] = {0};	//������

	::CopyMemory(byDeskCard,&m_byCards[m_iGetCardPos],sizeof(BYTE)*5);	//��ȡ����

	int iCardKind[PLAY_COUNT] = {0};

	BYTE byResultCard[7] = {0};
	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		//��λ�����û�в�����Ϸ
		if(m_UserData[i].m_iCardsNum <= 0)
		{
			continue;
		}

		iCardKind[i] = m_GamgeLogic.AnalysisCard(m_UserData[i].m_byMyCards,2,byDeskCard,5,byResultCard);
	}


	int iMaxPeople = 255;
	int iTempCardKind = 0;
	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(iCardKind[i] > iTempCardKind)
		{
			iTempCardKind = iCardKind[i];
			iMaxPeople = i;
		}
	}

	//������߾����Լ� ���� �Լ������� �� �����������ͬ ֱ�ӷ���
	if(bDeskStation == iMaxPeople || iCardKind[bDeskStation] == iCardKind[iMaxPeople])
	{
		return;
	}

	//�����Լ� �� ����ߵ� ��
	BYTE byTempCard[2] = {0};
	::CopyMemory(byTempCard,m_UserData[bDeskStation].m_byMyCards,sizeof(BYTE)*2);
	::CopyMemory(m_UserData[bDeskStation].m_byMyCards,m_UserData[iMaxPeople].m_byMyCards,sizeof(BYTE)*2);
	::CopyMemory(m_UserData[iMaxPeople].m_byMyCards,byTempCard,sizeof(BYTE)*2);
}