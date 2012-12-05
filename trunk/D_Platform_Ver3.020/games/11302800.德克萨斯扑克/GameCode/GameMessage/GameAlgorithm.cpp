#include "StdAfx.h"
#include <time.h>

#include "GameAlgorithm.h"
#include "UpgradeMessage.h"

//���캯��
CGameAlgorithm::CGameAlgorithm(void)
{

}

//���캯��
CGameAlgorithm::~CGameAlgorithm(void)
{

}

/*-------------------------------------------------------------------------------------------------------
	���������Ķ���
  -----------------------------------------------------------------------------------------------------*/

//��ȡ�˿�����
BYTE CGameAlgorithm::GetCardNum(BYTE byCard)
{
	return CARD_VAL(byCard);
}

//��ȡ�˿˻�ɫ
BYTE CGameAlgorithm::GetCardHuaKind(BYTE byCard)
{
	return CARD_HUA(byCard); 
}

//�����ֵ
USHORT CGameAlgorithm::GetCardBulk(BYTE byCrd,LPCVOID pData /*=NULL*/)
{
	return 0;
}

//���ƱȽ�
bool CGameAlgorithm::CompareCard(BYTE byCard1,BYTE byCard2,LPCVOID pData /*=NULL*/)
{
	return (GetCardBulk(byCard1,pData) > GetCardBulk(byCard2,pData));
}


//������RandCard
/*�����˿�
	@param byCard     [OUT]  ���ڴ�Ž�ϴ���Ƶ�������
	       nNum		  [IN]   ʹ�ö��ٷ��˿�
		   nDeskIndex [IN]   ���ţ������Ż������������
*/
BYTE CGameAlgorithm::RandCard(BYTE byCard[],int nNum,int nDeskIndex)
{
	const BYTE m_byBaseCards[52] =
	{
		0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,		//���� 2 - A
		0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,		//÷�� 2 - A
		0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,		//���� 2 - A
		0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,		//���� 2 - A
		/*0x4E, 0x4F	*/																		//С�����
	};

	::memset(byCard,0,sizeof(byCard));

	for (int i = 0; i < nNum; ++i)
	{
		::memcpy(byCard + i * sizeof(m_byBaseCards),m_byBaseCards,sizeof(m_byBaseCards));
	}

	int iCardsCount = nNum * sizeof(m_byBaseCards);
	int iHalfCount = iCardsCount / 2;

	BYTE* pLSide = byCard;
	BYTE* pRSide = byCard + iHalfCount;

	srand((unsigned)time(NULL));

	int iRandPos = 0;
	BYTE temp = 0;
	for (int i = 0; i < iHalfCount; ++i)
	{
		iRandPos = rand() % iHalfCount;
		temp = pLSide[iRandPos];
		pLSide[iRandPos] = pRSide[i];
		pRSide[i] = temp;
	}


	return iCardsCount;
}


//�����ź���������ټ���һ���ƣ���Ӻ󱣳�����
void CGameAlgorithm::InsertSorted(BYTE byCard[],int iCount,BYTE byNewCard,LPCVOID pData /*=NULL*/)
{
	if (iCount <= 0)
	{
		byCard[0] = byNewCard;
		return;
	}

	if (iCount >= MAX_HANDCARDS_NUM )
	{
		return;
	}

	int iLstPos = iCount - 1;

	if (iCount > 0)
	{
		for (int i = iLstPos; i >= 0; --i)
		{
			if (CompareCard(byNewCard,byCard[i],pData))
			{
				byCard[i + 1] = byCard[i];
				byCard[i] = byNewCard;
			}
			else
			{
				if (i == iLstPos)
				{
					byCard[i + 1] = byNewCard;
				}
				break;
			}
		}
	}
	return;
}


//������SortCard
/*�����˿�
  @param byCard	    [OUT] ����ԭʼ�����ݣ�����ź������
         bUp	    [IN]  ����ԭʼ���������λ�ã�����ź�����Ʊ��������λ��
		 iCardCount	[IN]  ������
		 pData      [IN]  ��������
*/
void CGameAlgorithm::SortCard(BYTE byCard[],BYTE bUp[],int iCardCount,LPCVOID pData /*= NULL*/)
{
	//�����˿�
	bool bSorted = true;
	bool bTempUp;
	int iLast = iCardCount-1;
	USHORT usBulks[45];
	USHORT usTemp;

	//��ȡλ����ֵ
	for (int i = 0; i < iCardCount; ++i)
	{
		usBulks[i] = GetCardBulk(byCard[i],pData);
	}

	//�������
	do
	{
		bSorted = true;
		for (int i = 0; i < iLast; ++i)
		{
			if (usBulks[i] < usBulks[i+1])
			{	
				//����λ��
				
				usTemp = byCard[i];
				byCard[i] = byCard[i+1];
				byCard[i+1] = usTemp;
				usTemp = usBulks[i];
				usBulks[i] = usBulks[i+1];
				usBulks[i+1] = usTemp;
				if (bUp != NULL)
				{
					bTempUp = bUp[i];
					bUp[i] = bUp[i+1];
					bUp[i+1] = bTempUp;
				}
				bSorted = false;
			}	
		}
		iLast--;
	} while(!bSorted);

	return;
}

//���0λ�˿�
int CGameAlgorithm::RemoveNummCard(BYTE iCardList[], int iCardCount)
{
	int iRemoveCount=0;
	for (int i=0;i<iCardCount;i++)
	{
		if (iCardList[i]!=0) 
			iCardList[i-iRemoveCount]=iCardList[i];
		else iRemoveCount++;
	}
	return iRemoveCount;
}

///ɾ���˿�
int CGameAlgorithm::RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount)
{
	//��������
	if ((iRemoveCount>iCardCount)) 
		return 0;

	//��Ҫɾ����������
	int iDeleteCount=0;
	for (int i=0;i<iRemoveCount;i++)
	{
		for (int j=0;j<iCardCount;j++)
		{
			if (iRemoveCard[i]==iCardList[j])
			{
				iDeleteCount++;
				iCardList[j]=0;
				break;
			}
		}
	}
	RemoveNummCard(iCardList,iCardCount);
	if (iDeleteCount!=iRemoveCount)
		return 0;

	return iDeleteCount;
}