#include "StdAfx.h"
#include "UpGradeLogic.h"

//���캯��
CUpGradeGameLogic::CUpGradeGameLogic(void)
{
	m_bSortCardStyle = 0; //0 �ư���С����;1 ����������
	m_iCondition=0;			//�޳�����������
	m_bKingCanReplace=false;
///	m_ioutcard=0;
	//�����ƶ���
	::memset(m_iReplaceCardArray,0,sizeof(m_iReplaceCardArray));
	//	m_iStation[4] = 500;
	//	for (int i=0; i<4; i++)
	//		m_iStation[i] = 100*i;
}
//���캯��
CUpGradeGameLogic::~CUpGradeGameLogic()
{
}
//��ȡ�˿˻�ɫ
BYTE CUpGradeGameLogic::GetCardHuaKind(BYTE iCard, BOOL bTrueHua)
{ 
	int iHuaKind=(iCard&UG_HUA_MASK);
	if (!bTrueHua)
	{
		return iHuaKind=UG_NT_CARD;
	}
	return iHuaKind; 
}

//��ȡ�˿˴�С ��2 - 18 �� 15 ���������� �� 2 - 21 �� 15 ����������
int CUpGradeGameLogic::GetCardBulk(BYTE iCard, bool bExtVal)
{
	if ((iCard == 0x4E) || (iCard == 0x4F))
	{
		return bExtVal ? (iCard-14) : (iCard-62); //��С��64+14-62=16	ֻ���ش�Сè��ֵ
	}

	int iCardNum = GetCardNum(iCard);
	int iHuaKind = GetCardHuaKind(iCard, TRUE);

	if (iCardNum == 2) //2��
	{
		if(bExtVal) //�й�
		{
			return ((iHuaKind>>4)+(15*4));
		}
		else //û�й�������2��
		{
			return 15;
		}
	}

	return ((bExtVal) ? ((iHuaKind>>4)+(iCardNum*4)) : (iCardNum));
}

//��ֵ�õ���
BYTE CUpGradeGameLogic::GetCardByValue(int Value)
{
	BYTE CardArray[55]={
		0x00,
		0x01,0x11, 0x21,0x31,
		0x02, 0x12 ,0x22 , 0x32 ,
		0x03,0x13, 0x23,0x33,
		0x04,0x14, 0x24, 0x34,
		0x05, 0x15,0x25, 0x35,
		0x06, 0x16, 0x26,0x36,
		0x07, 0x17,0x27, 0x37,
		0x08, 0x18, 0x28,  0x38,
		0x09, 0x19, 0x29,0x39,
		0x0A,  0x1A, 0x2A, 0x3A,
		0x0B,0x1B,  0x2B,0x3B,
		0x0C, 0x1C,0x2C, 0x3C,
		0x0D,  0x1D, 0x2D, 0x3D,
		0x4E, 0x4F};

		return CardArray[Value];
}


//���������ִӴ�С�����˿�
BOOL CUpGradeGameLogic::SortCard(BYTE iCardList[], BYTE bUp[], int iCardCount,BOOL bSysSort)
{

	if( iCardList == NULL || iCardList[0]==NULL)
		return FALSE; 
	BOOL bSorted=TRUE,bTempUp;
	int iTemp,iLast=iCardCount-1,iStationVol[45];

	//��ȡλ����ֵ
	for (int i=0;i<iCardCount;i++)
	{
		iStationVol[i]=GetCardBulk(iCardList[i], true);
		if (m_iReplaceCardArray[0] != 0)
			if (GetReplaceCardCount(&iCardList[i],1))
			{
				iStationVol[i]+=400;
			}
		//if (iStationVol[i]>=15) iStationVol[i]+=m_iStation[4];
		//else iStationVol[i]+=m_iStation[GetCardHuaKind(iCardList[i],FALSE)>>4];
	}

	//�������(���Ӵ�С����)
	do
	{
		bSorted=TRUE;
		for (int i=0;i<iLast;i++)
		{
			if (iStationVol[i]<iStationVol[i+1])
			{	
				//����λ��				//==ð������
				iTemp=iCardList[i];
				iCardList[i]=iCardList[i+1];
				iCardList[i+1]=iTemp;

				iTemp=iStationVol[i];
				iStationVol[i]=iStationVol[i+1];
				iStationVol[i+1]=iTemp;

				if (bUp!=NULL)
				{
					bTempUp=bUp[i];
					bUp[i]=bUp[i+1];
					bUp[i+1]=bTempUp;
				}
				bSorted=FALSE;
			}	
		}
		iLast--;
	} while(!bSorted);

	//ϵͳ���в����ǻ�ɫ��������
	if(bSysSort)
	{
		ReverseCard(iCardList,bUp,iCardCount);
		return TRUE;
	}
	if(GetSortCardStyle() == 1) //����������
		SortCardByStyle(iCardList,iCardCount);

	if(GetSortCardStyle() == 2)
		SortCardByKind(iCardList,iCardCount);


	return TRUE;
}

BOOL CUpGradeGameLogic::ReverseCard(BYTE iCardList[], BYTE bUp[], int iCardCount)
{
	BYTE iTemp;
	for(int i=0;i< iCardCount /2 ;i++)
	{
		iTemp = iCardList[i];
		iCardList[i] = iCardList[iCardCount - 1 -i];
		iCardList[iCardCount - 1 -i] = iTemp;
	}
	return TRUE;
}
//����������
BOOL CUpGradeGameLogic::SortCardByStyle(BYTE iCardList[], int iCardCount)
{
	//�������������Ҫ�󰴴�С����
	if(m_bSortCardStyle == 0)
	{
		SortCard(iCardList, NULL, iCardCount);

		return TRUE;
	}

	//����Ĵ���==�������Ŵ�С
	int iStationVol[45];
	for (int i=0;i<iCardCount;i++)
	{
		iStationVol[i]=GetCardBulk(iCardList[i],false);
	}

	int Start=0;
	int j,step;
	BYTE CardTemp[8];					//��������Ҫ��λ������
	int CardTempVal[8];					//����������λ������ֵ
	for(int i=8;i>1;i--)				//����������һ������i����ͬ��ֵ
	{
		for(j=Start;j<iCardCount;j++)
		{
			CardTemp[0]=iCardList[j];			//���浱ǰi��������ͬ��ֵ
			CardTempVal[0]=iStationVol[j];
			for(step=1;step<i&&j+step<iCardCount;)			//��һ������i��ֵ��ȵ�����(����������ʱ������)
			{
				if(iStationVol[j]==iStationVol[j+step])
				{
					CardTemp[step]=iCardList[j+step];			//������������
					CardTempVal[step]=iStationVol[j+step];		//��ֵ
					step++;
				}
				else
					break;
			}

			if(step>=i)	//�ҵ�һ������i����ȵ����鴮��ʼλ��Ϊj,����λ��Ϊj+setp-1
			{			//����Start��ʼ��j���������setp��
				if(j!=Start) //�ų���ʼ��������
				{
					for(;j>=Start;j--) //��Start����j�ź��ƶ�i��
					{
						iCardList[j+i-1]=iCardList[j-1];
						iStationVol[j+i-1]=iStationVol[j-1];
					}
					for(int k=0;k<i;k++)				
					{
						iCardList[Start+k]=CardTemp[k];	//��Start��ʼ���ó�CardSave
						iStationVol[Start+k]=CardTempVal[k];
					}
				}
				Start=Start+i;
			}
			j=j+step-1;
		}
	}
	return TRUE;
}

//����ɫ����
BOOL CUpGradeGameLogic::SortCardByKind(BYTE iCardList[],int iCardCount)
{
	return TRUE;
}

//�����˿�,������ʹ��
BYTE CUpGradeGameLogic::RandCard(BYTE iCard[], int iCardCount,bool bHaveKing)
{
	static const BYTE m_CardArray[54] =
	{
		0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //���� 2 - A
		0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //÷�� 2 - A
		0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //���� 2 - A
		0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //���� 2 - A
		0x4E, 0x4F //С�������
	};

	BYTE iSend=0,iStation=0,iCardList[216],step=(bHaveKing?54:52);
	srand((unsigned)time(NULL));

	for (int i=0;i<iCardCount;i+=step)
		::CopyMemory(&iCardList[i],m_CardArray,sizeof(m_CardArray));

	do
	{
		iStation=rand()%(iCardCount-iSend);
		iCard[iSend]=iCardList[iStation];
		iSend++;
		iCardList[iStation]=iCardList[iCardCount-iSend];
	} while (iSend<iCardCount);

	return iCardCount;
}

///�Ƚ�������,��¼�ڶ��������һ������ͬ���ƣ����ڶ������ڵ�һ���Ʋ�ͬ����
///@param BYTE bCard1[] Ҫ�Ƚϵ�����1
///@param int iCardCount1 Ҫ�Ƚϵ�����1����
///@param BYTE bCard2[] Ҫ�Ƚϵ�����2
///@param int iCardCount2 Ҫ�Ƚϵ�����2����
///@param BYTE bSameCard[] ������ͬ����
///@param int  &iSameCount ��ͬ������
///@param BYTE bDifferCard[] ���ز���ͬ����
///@param int  &iDifferCount ��ͬ��������
void CUpGradeGameLogic::CompareCard(BYTE bCard1[],  
								  int iCardCount1,		
								  BYTE bCard2[],		
								  int iCardCount2,		
								  BYTE bSameCard[],     
								  int  &iSameCount,
								  BYTE bDifferCard[],
								  int  &iDifferCount)
{
	BYTE bCardBuf1[20] = {0};
	memcpy(bCardBuf1, bCard1, sizeof(BYTE)*iCardCount1);
	BYTE bCardBuf2[20] = {0};
	iSameCount=0;
	iDifferCount=0;
	memcpy(bCardBuf2, bCard2, sizeof(BYTE)*iCardCount2);
//	CString str;
	for (int i=0; i< iCardCount2; i++)
	{
		for (int j=0; j<iCardCount1; j++)
		{
			if (bCardBuf1[j] == bCardBuf2[i])
			{
				bSameCard[iSameCount++] = bCardBuf2[i];
				bCardBuf1[j] = 255;//�Ѿ�ƥ����Ͳ���ƥ����
				break;
			}
			else if(j == iCardCount1-1)
			{
	
				bDifferCard[iDifferCount++] = bCardBuf2[i];
				

			}
		}
	}
	


	

}
//ɾ���˿�
int CUpGradeGameLogic::RemoveCard(BYTE iRemoveCard[],   //Ҫɾ��������
								  int iRemoveCount,		//Ҫɾ����������
								  BYTE iCardList[],		//Ҫ����������
								  int iCardCount)		//�������������
{
	//��������
	if(iRemoveCount > iCardCount) return 0;

	int iRecount;
	int iDeleteCount = 0; //��Ҫɾ����������

	for (int i = 0; i < iRemoveCount; i++)
	{
		for (int j = 0; j < iCardCount; j++)
		{
			if (iRemoveCard[i] == iCardList[j])
			{
				iDeleteCount++;
				iCardList[j] = 0;
				break;
			}
		}
	}
	iRecount = RemoveNummCard(iCardList, iCardCount); //ɾ�����˱�ǵ���

	if (iDeleteCount!=iRecount)
		return 0;

	return iDeleteCount;
}


//��� 0 λ�˿�
int CUpGradeGameLogic::RemoveNummCard(BYTE iCardList[], int iCardCount)
{
	int iRemoveCount=0;
	for (int i=0;i<iCardCount;i++)
	{
		if (iCardList[i]!=0)
			iCardList[i-iRemoveCount]=iCardList[i];
		else 
			iRemoveCount++;
	}

	return iRemoveCount;
}


//��������

//�Ƚϵ���
BOOL CUpGradeGameLogic::CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard)
{
	//��һ����ʾ�����������, �ڶ�����ʾҪ������
	return GetCardBulk(iFirstCard) < GetCardBulk(iNextCard);
}

//��������ķ���(5,10,K)
int CUpGradeGameLogic::FindPoint(BYTE iCardList[], int iCardCount)
{
	int iPoint = 0; //����
	for (int i=0; i<iCardCount; i++)
	{
		int iNum = GetCardNum(iCardList[i]); //�������
		switch(iNum)
		{
		case 5:
			iPoint += 5;
			break;
		case 10:
		case 13:
			iPoint += 10;
			break;
		}
	}
	return iPoint;
}

//�������Ƿ�����ͬ����
BOOL CUpGradeGameLogic::IsSameNumCard(BYTE iCardList[],int iCardCount,bool bExtVal)
{
	int i, temp[18] = {0};
	for(i = 0; i < iCardCount; i++)
	{
		temp[GetCardBulk(iCardList[i],false)]++;
	}

	for(i = 0; i < 18; i++)
	{
		if(temp[i]!=0)
			break;
	}
	if(m_bKingCanReplace)
	{
		if(i<16)//����������
			return (temp[i]+temp[16]+temp[17]==iCardCount);
		//else//ֻ����
			if(i < 17)
				return (temp[16]+temp[17]==iCardCount);
	}
	else
		return (temp[i]==iCardCount);
	return 0;
}

//�Ƿ�Ϊͬ��
BOOL CUpGradeGameLogic::IsSameHuaKind(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if (iCardCount <= 0) return FALSE;

	BYTE iFirstHua = GetCardHuaKind(iCardList[0], TRUE); //ȡ�õ�һ���ƵĻ�ɫ

	for (int i = 1; i < iCardCount; i++) //����Ķ��͵�һ�ŵĻ�ɫ��
	{
		if(GetCardHuaKind(iCardList[i], TRUE) != iFirstHua) 
		{
			return FALSE;
		}
	}

	return TRUE;
	/*
	BYTE Kind[5] = {0};

	for(int i = 0; i < iCardCount; i++)
	{
	Kind[GetCardHuaKind(iCardList[i], TRUE)/16] = 1; //��Ӧ��ɫ��ֵ��1
	}

	BYTE iPos = 0;
	for(int i = 0; i < 4; i++)
	{
	if(Kind[i] == 1) iPos++; //�����м�����ɫ
	}

	if(iPos != 1) return FALSE;

	return TRUE;
	//*/
}

//�����û�����ը����
BYTE CUpGradeGameLogic::GetBombCount(BYTE iCardList[],int iCardCount,int iNumCount,bool bExtVal)
{
	int iCount = 0,
		temp[18] = {0};
	for(int i=0; i<iCardCount; i++)
	{
		temp[ GetCardBulk(iCardList[i])]++;
	}
	for(int i=0;i<16;i++)
	{
		if(temp[i] >= iNumCount)
			iCount++;
	}
	return iCount;	
}

//��ȡָ�������Ƹ���
BYTE CUpGradeGameLogic::GetCountBySpecifyNumCount(BYTE iCardList[],int iCardCount,int Num)
{
	BYTE temp[18] = {0};
	int count = 0;
	for(int i = 0;i < iCardCount;i ++)
		temp[GetCardBulk(iCardList[i])]++;

	for(int i = 0;i< 18;i++)
		if(temp[i] == Num)
			count++;

	return count;
}

//��ȡָ���Ƹ���
BYTE CUpGradeGameLogic::GetCountBySpecifyCard(BYTE iCardList[],int iCardCount,BYTE bCard)
{
	int count = 0;
	for(int i = 0;i < iCardCount;i ++)
		if(iCardList[i] == bCard) 
			count++;

	return count;
}
//��ȡָ���������ƴ�С
BYTE CUpGradeGameLogic::GetBulkBySpecifyCardCount(BYTE iCardList[],int iCardCount,int iCount)
{
	BYTE temp[18] = {0};
	for(int i = 0;i < iCardCount;i ++)
		temp[GetCardBulk(iCardList[i])]++;

	for(int i = 17;i> 0;i--)
		if(temp[i] == iCount)
			return i;

	return 0;
}

//�Ƿ�Ϊ����˳��
BOOL CUpGradeGameLogic::IsVariationSequence(BYTE iCardList[], int iCardCount, int iCount)
{
	int iValue = iCardCount / iCount;
	if (iCardCount != iCount *iValue)						 //����������
		return FALSE;

	int iFirstMax = 0, iSecondMax = 0,iThirdMax = 0,iMin = 18;//�ҳ���һ��,�ڶ���,���������,����С��
	BYTE temp[18]={0};
	for(int i = 0;i < iCardCount;i ++)						//�ƶ���
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for(int i=0;i<18;i++)	
	{
		if(temp[i] !=0 && temp[i] != iCount)	//���Ҷ�˳��
			return false;
	}

	for(int i = 0;i < 18;i ++)						//��С�������ܵ�A
	{
		if(temp[i] != 0)
			iMin = i;
	}

	for(int i=17;i>0;i++)
	{
		if(temp[i] !=0 )
		{
			iFirstMax=i;						//������2Ҳ������A
			for(int j=i-1;j>0;j--)
			{
				if(temp[j] !=0)//�ҵ��ڶ�����˳�ѭ��(�޵������)//������AҲ���Է�A
				{
					iSecondMax = j;
					for(int k=j-1;j>0;j--)
					{
						if(temp[k] != 0)//��ڵ�������˳�ѭ��	//���Ǵ���Ҳ���Բ�����
						{
							iThirdMax =k;
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}

	if(iFirstMax < 15)	//������2�����,���������
	{
		return (iFirstMax -iMin + 1 == iValue);
	}

	if(iFirstMax == 15)	//����2,�ٿ��Ƿ����A
	{
		if(iSecondMax == 14)		//����A
		{
			if(iThirdMax == 0)		//�����ڵ�����,Ҳֻ��A2������
				return true;

			return (iThirdMax - iMin +1 == iValue - 2);		//���� A2�����������AA2233
		}
		return (iSecondMax -iMin+1 == iValue-1);
	}

	return false;
}

//�Ƿ�����
BOOL CUpGradeGameLogic::IsSequence(BYTE iCardList[], int iCardCount, int iCount)
{
	BYTE temp[18]={0};
	for(int i= 0;i < iCardCount;i++)
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for(int i = 0; i < 15 ; i ++)
	{
		if(temp [i]!= 0 &&temp[i] !=iCount)	//��ָ��˳
			return false;
	}

	int len = iCardCount / iCount;

	for(int i=0;i<15;i++)
	{
		if(temp[i] != 0)//��ֵ
		{
			//if(temp[i] == iCount )
			//{	
			for(int j = i;j < i + len  ;j ++)
			{
				if(temp[j] != iCount || j >=15 )
					return false;
			}
			return true;
			//}else 
			//	return false;
		}
	}
	return false;
}

//��ȡָ���Ʒ����ҵ��Ƃ���
BYTE  CUpGradeGameLogic::TackOutBySpecifyCard(BYTE iCardList[], int iCardCount,BYTE bCardBuffer[],int &iResultCardCount,BYTE bCard)
{
	iResultCardCount = 0;
	for(int i = 0;i < iCardCount;i++)
	{
		if(iCardList[i] == bCard)
			bCardBuffer[iResultCardCount ++] = iCardList[i];
	}
	return iResultCardCount;
}


//��ȡ1,2,3 or 4����ͬ���ֵ���
int CUpGradeGameLogic::TackOutBySepcifyCardNumCount(BYTE iCardList[], int iCardCount, 
													BYTE iDoubleBuffer[], BYTE bCardNum, 
													bool bExtVal)
{
	int iCount = 0, temp[18] = {0};
	for(int i = 0; i < iCardCount; i++)
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for(int i=0; i<18; i++)
	{
		if(temp[i] == bCardNum) //����Ҫ���ҵ�����:one?double?three?four_bomb?
		{
			for(int j = 0; j < iCardCount; j++)
			{
				if(i == GetCardBulk(iCardList[j]))
					iDoubleBuffer[iCount++] = iCardList[j];
			}
		}
	}
	return iCount;
}

//��ȡָ����ɫ��
int CUpGradeGameLogic::TackOutByCardKind(BYTE iCardList[],int iCardCount,BYTE iDoubleBuffer[],BYTE iCardKind)
{
	int count =0;

	for(int i = 0;i < iCardCount; i++)
	{
		/*TCHAR sz[200];
		wsprintf(sz,"i=%d,iCardKind = %d %d",i,iCardKind,GetCardHuaKind(iCardList[i]));
		WriteStr(sz,2,2);*/
		if( GetCardHuaKind(iCardList[i]) == iCardKind)
		{
			iDoubleBuffer[count++] = iCardList[i];
		}
	}
	return count;
}

//���(�������ƶ�Ĳ���ٵ�)
int CUpGradeGameLogic::TackOutMuchToFew(BYTE iCardList[],int iCardCount,BYTE iDoubleBuffer[],int &iBufferCardCount,BYTE iCardMuch,BYTE iCardFew)
{
	iBufferCardCount=0;
	int count =0;
	BYTE iBuffer[54];
	int iCount = TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,iCardMuch);
	if(iCount <=0)
		return count;
	for(int i = 0;i < iCount; i += iCardMuch)
	{
		::CopyMemory(&iDoubleBuffer[iBufferCardCount],&iBuffer[i],sizeof(BYTE)*iCardFew);
		iBufferCardCount += iCardFew;
		count++;
	}
	return count;
}

//��ȡĳ��ָ����С����
BOOL CUpGradeGameLogic::TackOutCardBySpecifyCardNum(BYTE iCardList[],int iCardCount,BYTE iBuffer[],int &iBufferCardCount,BYTE iCard,BOOL bExtVal)
{
	iBufferCardCount = 0;
	BYTE iCardNum = GetCardBulk(iCard); //�õ��������
	for(int i = 0; i < iCardCount; i++)
	{
		if(GetCardBulk(iCardList[i]) == iCardNum) //����Ҫ���ҵ��Ƶ�����
		{
			iBuffer[iBufferCardCount++] = iCardList[i];
		}
	}

	return iBufferCardCount;
}

//���Ҵ���iCard�ĵ�������iCardList�е����
BYTE  CUpGradeGameLogic::GetSerialByMoreThanSpecifyCard(BYTE iCardList[], int iCardCount,
														BYTE iCard, BYTE iBaseCardCount,
														bool bExtValue)
{
	BYTE MaxCard=0;
	BYTE Serial=0;
	BYTE MaxCardNum=255;

	int BaseCardNum = GetCardBulk(iCard);	//��ǰ�Ƚ�ֵ

	for(BYTE i=0; i<iCardCount; i+=iBaseCardCount)	
	{
		int temp = GetCardBulk(iCardList[i]);

		if(temp<MaxCardNum && temp>BaseCardNum)
		{
			MaxCardNum = temp;
			Serial = i; //�õ����
			break;
		}
	}

	return Serial;
}


//����==iCard�ĵ�������iCardList�е����
int  CUpGradeGameLogic::GetSerialBySpecifyCard(BYTE iCardList[],int iStart,int iCardCount,BYTE iCard)
{
	for(int i = iStart;i < iCardCount;i ++)
	{
		if(iCardList[i] == iCard)
			return i;
	}
	return -1;
}

//����˳��������
BYTE CUpGradeGameLogic::GetBulkBySpecifyVariationSequence(BYTE iCardList[],int iCardCount ,int iSequence )
{
	int iFirstMax = 0, iSecondMax = 0,iThirdMax = 0;//�ҳ���һ��,�ڶ���,���������,����С��
	BYTE temp[18]={0};
	for(int i = 0;i < iCardCount;i ++)						//�ƶ���
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for(int i=17;i>0;i++)
	{
		if(temp[i] == iSequence)
		{
			iFirstMax=i;						//������2Ҳ������A
			for(int j=i-1;j>0;j--)
			{
				if(temp[j] == iSequence)//�ҵ��ڶ�����˳�ѭ��(�޵������)//������AҲ���Է�A
				{
					iSecondMax = j;
					for(int k=j-1;j>0;j--)
					{
						if(temp[k] == iSequence)//��ڵ�������˳�ѭ��	//���Ǵ���Ҳ���Բ�����
						{
							iThirdMax =k;
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}

	if(iFirstMax == 15)	//����2,�ٿ��Ƿ����A
	{
		if(iSecondMax == 14)		//����A
		{
			if(iThirdMax == 0)		//�����ڵ�����,Ҳֻ��A2������
				return 2;

			return iThirdMax;		//���� A2�����������AA2233
		}
		return iSecondMax;
	}
	return 0;
}

//��ȡָ��˳�����Ƶ���Сֵ(iSequence ����˳�ӵ��������Ϊ
BYTE  CUpGradeGameLogic::GetBulkBySpecifySequence(BYTE iCardList[],int iCardCount ,int iSequence )
{
	int temp[18] = {0};
	for(int i = 0; i < iCardCount; i++)
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for(int k = 0; k < 15; k++)
	{
		if(temp[k] == iSequence)
		{
			return k;
		}
	}
	return 0;
}

//�ҳ�һ����С��������
int  CUpGradeGameLogic::GetBulkBySepcifyMinOrMax(BYTE iCardList[], int iCardCount, int MinOrMax/*1 or 255*/, bool bExtVal)
{
	int CardNum = GetCardBulk(iCardList[0], false);

	if(MinOrMax == 1) //����С��
	{
		for(int i = 1; i < iCardCount; i++)
		{
			if(GetCardBulk(iCardList[i], false) < CardNum)
				CardNum = GetCardBulk(iCardList[i], false);
		}
	}
	else if(MinOrMax == 255)
	{
		for(int i = 1; i < iCardCount; i++)
		{
			if(GetCardBulk(iCardList[i], false) > CardNum)
				CardNum = GetCardBulk(iCardList[i], false);
		}
	}

	//���ص��� GetCardBulk() �õ���ֵ
	return CardNum;
}

/////////////////////////////////////////////////////////////////////////
/**
* @info ��ȡ����
* @param iCardList[] ��
* @param iCardCount �Ƶ�����
* @param bExlVol ?
*/
BYTE CUpGradeGameLogic::GetCardShape(BYTE iCardList[], int iCardCount, bool bExlVol)
{
	
	//if (GetReplaceCardCount(iCardList,iCardCount) == iCardCount) return UG_RASCAL;//;//���Ƶ�ȫ�����,12.11.2008;
	if (IsOnlyOne(iCardList,iCardCount)/*&&(GetReplaceCardCount(iCardList,iCardCount)!=iCardCount)*/) return UG_ONLY_ONE; //����
	if (IsDouble(iCardList,iCardCount)/*&&(GetReplaceCardCount(iCardList,iCardCount)!=iCardCount)*/) return UG_DOUBLE;	 //����
	if (IsThreeX(iCardList,iCardCount, 0)/*&&(GetReplaceCardCount(iCardList,iCardCount)!=iCardCount)*/) return UG_THREE;	 //����

	if	(IsKingBomb(iCardList,iCardCount)) return UG_KING_BOMB;//��ը
//	if (IsBombSameHua(iCardList,iCardCount)) return UG_BOMB_SAME_HUA; //ͬ��ը��
	if (IsBomb(iCardList, iCardCount)/*&&(GetReplaceCardCount(iCardList,iCardCount)!=iCardCount)*/) return UG_BOMB; //4������ͬ���ƣ�ը��
	if (IsShamBomb(iCardList, iCardCount)) return UG_SHAM_BOMB; //4������ͬ���ƣ���ը��������
	if (IsThreeX(iCardList,iCardCount,3))	return UG_THREE_DOUBLE;	//������
//	if (IsThreeX(iCardList, iCardCount, 2)) return UG_THREE_TWO; //������
	if (IsThreeX(iCardList, iCardCount, 1)) return UG_THREE_ONE; //����һ

	if  (IsFourX(iCardList,iCardCount,4)) return UG_FOUR_TWO_DOUBLE;		//�Ĵ�����(Ҫ���Ƕ���)
//	if  (IsFourX(iCardList,iCardCount,3)) return UG_FOUR_ONE_DOUBLE;		//�Ĵ�һ��(Ҫ��ɶ�)
	if	(IsFourX(iCardList,iCardCount,2)) return UG_FOUR_TWO;			//�Ĵ���(��Ҫ��ɶ�)
//	if	(IsFourX(iCardList,iCardCount,1)) return UG_FOUR_ONE;			//�Ĵ�һ

	if (IsFourXSequence(iCardList,iCardCount,4)) return UG_FOUR_TWO_DOUBLE_SEQUENCE;	//��˳������
//	if (IsFourXSequence(iCardList,iCardCount,3)) return UG_FOUR_ONE_DOUBLE_SEQUENCE;	//��˳��һ��
	if (IsFourXSequence(iCardList,iCardCount,2)) return UG_FOUR_TWO_SEQUENCE;	//��˳��������
//	if (IsFourXSequence(iCardList,iCardCount,1)) return UG_FOUR_ONE_SEQUENCE;	//��˳������
	if (IsFourXSequence(iCardList,iCardCount,0)) return UG_FOUR_SEQUENCE;	//��˳

//	if (IsVariationFourXSequence(iCardList,iCardCount,0)) return UG_VARIATION_FOUR_SEQUENCE;	//������˳
//	if (IsVariationFourXSequence(iCardList,iCardCount,1)) return UG_VARIATION_FOUR_ONE_SEQUENCE;	//������˳������
//	if (IsVariationFourXSequence(iCardList,iCardCount,2)) return UG_VARIATION_FOUR_TWO_SEQUENCE;	//������˳��������
//	if (IsVariationFourXSequence(iCardList,iCardCount,3)) return UG_VARIATION_FOUR_ONE_DOUBLE_SEQUENCE;	//������˳��һ��
//	if (IsVariationFourXSequence(iCardList,iCardCount,4)) return UG_VARIATION_FOUR_TWO_DOUBLE_SEQUENCE;	//������˳������

	/* ˳���а��� ͬ��˳,�������ж��Ƿ�ͬ��˳,������ǣ����ж��Ƿ���˳�ӣ������˳�ӣ�����һ���˳����*/
//	if (IsStraightFlush(iCardList, iCardCount)) return UG_STRAIGHT_FLUSH; //ͬ��˳
	//if	(IsFlush(iCardList,iCardCount)) return UG_FLUSH;					//ͬ��(��˳��)
	if  (IsStraight(iCardList, iCardCount)) return UG_STRAIGHT;            //˳��	
//	if	(IsVariationStraight(iCardList,iCardCount,1)) return UG_VARIATION_STRAIGHT;				//���ֵ�˳

//	if	(IsVariationDoubleSequence(iCardList,iCardCount,2)) return UG_VARIATION_STRAIGHT;		//����˫˳
	if (IsDoubleSequence(iCardList, iCardCount)) return UG_DOUBLE_SEQUENCE;  //����

//	if (IsThreeSequenceDoubleSequence(iCardList,iCardCount)) return UG_THREE_SEQUENCE_DOUBLE_SEQUENCE;//����
//	if (IsVariationThreeSequenceDoubleSequence(iCardList,iCardCount)) return UG_VARIATION_THREE_SEQUENCE_DOUBLE_SEQUENCE;//���ֺ���

	if (IsThreeXSequence(iCardList, iCardCount, 3)) return UG_THREE_DOUBLE_SEQUENCE; //����������
//	if (IsThreeXSequence(iCardList, iCardCount, 2)) return UG_THREE_TWO_SEQUENCE; //����������
	if (IsThreeXSequence(iCardList, iCardCount, 1)) return UG_THREE_ONE_SEQUENCE; //��������һ
	if (IsThreeXSequence(iCardList, iCardCount, 0)) return UG_THREE_SEQUENCE; //����

//	if (IsVariationThreeXSequence(iCardList, iCardCount, 3)) return UG_THREE_DOUBLE_SEQUENCE; //��������������
//	if (IsVariationThreeXSequence(iCardList, iCardCount, 2)) return UG_THREE_TWO_SEQUENCE; //��������������
//	if (IsVariationThreeXSequence(iCardList, iCardCount, 1)) return UG_THREE_ONE_SEQUENCE; //������������һ
//	if (IsVariationThreeXSequence(iCardList, iCardCount, 0)) return UG_THREE_SEQUENCE; //��������

//	if (IsMaster510K(iCardList, iCardCount)) return UG_MASTER_510K; //510Kͬ��ը��
//	if (IsSlave510K(iCardList, iCardCount)) return UG_SLAVE_510K;            //510Kը��

	return UG_ERROR_KIND;
}

//����
BOOL CUpGradeGameLogic::IsDouble(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if (iCardCount != 2) 
		return FALSE;
	return IsSameNumCard(iCardList, iCardCount, bExtVal);
}

//3 �� 0,1or2,or3
BOOL CUpGradeGameLogic::IsThreeX(BYTE iCardList[], int iCardCount, int iX, bool bExtVal)
{
	if(iCardCount > 5 || iCardCount < 3) 
		return FALSE;

	if(GetCountBySpecifyNumCount(iCardList, iCardCount,3) != 1)//�Ƿ��������
		return false;
	switch(iX)
	{
	case 0:	
		return iCardCount == 3;//IsSameNumCard(iCardList, iCardCount, bExtVal);//����
		break;
	case 1:
		return iCardCount == 4;//������
		break;
	case 2:
		return iCardCount == 5;//�����ţ����ԷǶ��ӣ�
		break;
	case 3:					//��һ��
		return GetCountBySpecifyNumCount(iCardList,iCardCount,2)==1;//�Ƿ���ڶ���
		break;
	default:
		break;
	}
	return false;
}

//�Ĵ�1or2
BOOL CUpGradeGameLogic::IsFourX(BYTE iCardList[],int iCardCount,int iX)
{
	if(iCardCount >8 || iCardCount < 4)
		return false;

	if(GetCountBySpecifyNumCount(iCardList,iCardCount,4) != 1)//�Ƿ����ĸ�����
		return false;

	switch(iX)
	{
	case 0:
		return iCardCount == 4;//����
	case 1:						
		return iCardCount == 5;//�Ĵ�1��
	case 2:
		return iCardCount ==6;//�Ĵ�2(��Ҫ��ɶ�)
	case 3:
		return (iCardCount == 6 && 1 == GetCountBySpecifyNumCount(iCardList,iCardCount,2));//Ҫ��ɶ�
	case 4:
		return (iCardCount == 8 && 2 == GetCountBySpecifyNumCount(iCardList,iCardCount,2));	//�Ĵ�2��
	}

	return FALSE;
}

//��ը
BOOL CUpGradeGameLogic:: IsKingBomb(BYTE iCardList[],int iCardCount)			//�Ƿ�Ϊ��ը(ץ��������)
{
	if(iCardCount != KING_COUNT)
		return false;

	for(int i=0;i<iCardCount;i++)
		if(iCardList[i]!=0x4e&&iCardList[i]!=0x4f)
			return false;
	return true;
}

//4+���� ��ը��
BOOL CUpGradeGameLogic::IsShamBomb(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if(iCardCount != 4)
		return FALSE;
	if (GetReplaceCardCount(iCardList, iCardCount)==0)
		return FALSE;

	BYTE iBuffer[54];
	//����һ��
	BYTE iTmp[54];
	int ReplaceCount = 0;
	int iTmpCount = iCardCount;
	::CopyMemory(iTmp,iCardList,sizeof(BYTE)*iCardCount);
	TackOutByCondition(iCardList,iCardCount,iBuffer,ReplaceCount,m_iReplaceCardArray,m_iReplaceCardCount);
	int iRev = RemoveCard(iBuffer,ReplaceCount,iTmp,iCardCount);
	iTmpCount -= iRev;


	return IsSameNumCard(iTmp, iTmpCount, bExtVal); //�Ƿ�����ͬ����
}
//4+���� ը��
BOOL CUpGradeGameLogic::IsBomb(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if(iCardCount != 4)
		return FALSE;

	return IsSameNumCard(iCardList, iCardCount, bExtVal); //�Ƿ�����ͬ����
}
//ͬ��ը��
BOOL CUpGradeGameLogic::IsBombSameHua(BYTE iCardList[],int iCardCount)
{
	if(!IsBomb(iCardList,iCardCount)) return false;
	if(!IsSameHuaKind(iCardList,iCardCount)) return false;
	return TRUE;
}

//ͬ��(��ͬ��)
BOOL CUpGradeGameLogic::IsFlush(BYTE iCardList[],int iCardCount)
{
	return IsSameHuaKind(iCardList, iCardCount);
}

//ͬ��˳ 5��ͬ��������
BOOL CUpGradeGameLogic::IsStraightFlush(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if(!IsSameHuaKind(iCardList, iCardCount, bExtVal)) return FALSE; //ͬ����

	if( !IsStraight(iCardList, iCardCount, bExtVal) ) return FALSE; //˳�ӣ�
	return TRUE;
}

//�Ƿ��Ǳ���˳��(A2345)��23456
BOOL CUpGradeGameLogic::IsVariationStraight(BYTE iCardList[],int iCardCount,bool bExtVal)
{
	if(iCardCount < 5)
		return false;
	return IsVariationSequence(iCardList,iCardCount,1);
}

//�Ƿ���˳��ָ������
BOOL CUpGradeGameLogic::IsStraight(BYTE iCardList[],int iCardCount,bool bExtVal)
{
	if(iCardCount < 5)
		return false;
	return IsSequence(iCardList,iCardCount,1);
}

//�Ƿ��Ǳ�������AA22��2233��
BOOL CUpGradeGameLogic::IsVariationDoubleSequence(BYTE iCardList[],int iCardCount,bool bExtVal)
{
	if(iCardCount%2 != 0 || iCardCount < 4)
		return false;

	return IsVariationSequence(iCardList,iCardCount,2);
}

//�Ƿ�������
BOOL CUpGradeGameLogic::IsDoubleSequence(BYTE iCardList[],int iCardCount,bool bExtVal)
{
	if(iCardCount%2 != 0 || iCardCount < 6)
		return false;

	return IsSequence(iCardList,iCardCount,2);
}

//������˳����˳
BOOL CUpGradeGameLogic::IsVariationThreeSequenceDoubleSequence(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if(iCardCount < 10)		//��˳����2��˳Ҳ���ٶ�
		return false;

	BYTE iBuffer3[54],iBuffer2[54];
	BOOL bValue3 = false,bValue2 = false;	//��˳,��˳�Ƿ�Ϊ˳,
	int TackOutCount3 = 0,TackOutCount2 = 0;

	TackOutCount3=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer3,3);//����
	TackOutCount2=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer2,2);//����

	if(TackOutCount3 <=0 || TackOutCount2 <=0 || TackOutCount3 + TackOutCount2 !=iCardCount || TackOutCount3/3 != TackOutCount2/2 )
		return FALSE;

	bValue3 =IsVariationSequence(iBuffer3,TackOutCount3,3);
	bValue2 =(IsVariationSequence(iBuffer2,TackOutCount2,2)||IsSequence(iBuffer2,TackOutCount2,2));
	return bValue3&&bValue2;
}

//��˳����˳
BOOL CUpGradeGameLogic::IsThreeSequenceDoubleSequence(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if(iCardCount < 10)		//��˳����2��˳Ҳ���ٶ�
		return false;

	BYTE iBuffer3[54],iBuffer2[54];
	BOOL bValue3 = false,bValue2 = false;	//��˳,��˳�Ƿ�Ϊ˳,
	int TackOutCount3 = 0,TackOutCount2 = 0;

	TackOutCount3=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer3,3);//����
	TackOutCount2=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer2,2);//����
	if(TackOutCount3 <=0 || TackOutCount2 <=0 || TackOutCount3 + TackOutCount2 !=iCardCount || TackOutCount3/3 != TackOutCount2/2 )
		return FALSE;
	bValue3 =IsSequence(iBuffer3,TackOutCount3,3);
	//	TCHAR sz[200];
	//wsprintf(sz,"%d",bValue3);
	//	WriteStr(sz);
	bValue2 =(IsVariationSequence(iBuffer2,TackOutCount2,2)||IsSequence(iBuffer2,TackOutCount2,2));
	//	TCHAR sz[200];
	//	wsprintf(sz,"bValue3=%d,bValue2=%d==����2˳%d,��׼��˳%d",bValue3,bValue2,IsVariationSequence(iBuffer2,TackOutCount2,2),IsSequence(iBuffer2,TackOutCount2,2));
	//	WriteStr(sz);
	return bValue3&&bValue2;
}


//����������x
BOOL CUpGradeGameLogic::IsVariationThreeXSequence(BYTE iCardList[], int iCardCount, int iSeqX, bool bExtVal)
{
	if(iCardCount < 6)		//��˳����2
		return false;

	BYTE iBuffer[54];
	int TackOutCount=0;
	switch(iSeqX)
	{
	case 0:
		if( iCardCount%3 != 0)		
			return false;
		return IsVariationSequence(iCardList,iCardCount,3);
		break;
	case 1://����
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,3);
		if(TackOutCount>0 && TackOutCount/3*4 == iCardCount)
			return IsVariationSequence(iBuffer,TackOutCount,3);
		break;
	case 2://������
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,3);
		if(TackOutCount>0 &&TackOutCount/3*5 == iCardCount)
			return IsVariationSequence(iBuffer,TackOutCount,3);
	case 3://��һ��
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,3);
		if(TackOutCount>0 &&TackOutCount/3*5 == iCardCount
			&&GetCountBySpecifyNumCount(iCardList,iCardCount,2))
			return IsVariationSequence(iBuffer,TackOutCount,3);

		break;
	}
	return false;
}

//�������� 0,1 or 2
BOOL CUpGradeGameLogic::IsThreeXSequence(BYTE iCardList[], int iCardCount, int iSeqX, bool bExtVal)
{
	if(iCardCount < 6)		//��˳����2
		return false;

	BYTE iBuffer[54];
	int TackOutCount=0;
	switch(iSeqX)
	{
	case 0:
		if( iCardCount%3 != 0)		
			return false;
		return IsSequence(iCardList,iCardCount,3);
		break;
	case 1://����
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,3);
		if(TackOutCount>0 && TackOutCount/3*4 == iCardCount)
			return IsSequence(iBuffer,TackOutCount,3)
			&& (TackOutCount/3==GetCountBySpecifyNumCount(iCardList,iCardCount,1));//����Ҫ��333444���ܴ�55;
		break;
	case 2://������
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,3);
		if(TackOutCount>0 &&TackOutCount/3*5 == iCardCount)
			return IsSequence(iBuffer,TackOutCount,3);
	case 3://����
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,3);
		if(TackOutCount>0 &&TackOutCount/3*5 == iCardCount
			&&GetCountBySpecifyNumCount(iCardList,iCardCount,2) == TackOutCount/3)
			return IsSequence(iBuffer,TackOutCount,3);

		break;
	}
	return false;
}

//������˳����
BOOL CUpGradeGameLogic::IsVariationFourXSequence(BYTE iCardList[],int iCardCount,int iSeqX)
{
	if(iCardCount < 8)		//��˳����2
		return false;

	BYTE iBuffer[54];
	int TackOutCount=0;
	switch(iSeqX)
	{
	case 0:
		if( iCardCount%4 != 0)		
			return false;
		return IsVariationSequence(iCardList,iCardCount,4);
		break;

	case 1://������
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 && TackOutCount/4*5 == iCardCount)
			return IsVariationSequence(iBuffer,TackOutCount,4);
		break;

	case 2://������(���ԷǶ��ӣ�
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 &&TackOutCount/4*6 == iCardCount)
			return IsVariationSequence(iBuffer,TackOutCount,4);

	case 3://��һ��
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 &&TackOutCount/4*6 == iCardCount 
			&&TackOutCount/4 == GetBulkBySpecifyCardCount(iCardList,iCardCount,2))
			return IsVariationSequence(iBuffer,TackOutCount,4);

	case 4://(�����ԣ�
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 &&TackOutCount/4*6 == iCardCount
			&&TackOutCount/2 == GetBulkBySpecifyCardCount(iCardList,iCardCount,2))
			return IsVariationSequence(iBuffer,TackOutCount,4);
		break;
	}	
	return FALSE;
}

//��˳����
BOOL CUpGradeGameLogic::IsFourXSequence(BYTE iCardList[],int iCardCount,int iSeqX)
{
	if(iCardCount < 8)		//��˳����2
		return false;

	BYTE iBuffer[54];
	int TackOutCount=0;
	switch(iSeqX)
	{
	case 0:
		if( iCardCount%4 != 0)		
			return false;
		return IsSequence(iCardList,iCardCount,4);
		break;

	case 1://������
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 && TackOutCount/4*5 == iCardCount)
			return IsSequence(iBuffer,TackOutCount,4);
		break;

	case 2://������(���ԷǶ��ӣ�
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 &&TackOutCount/4*6 == iCardCount)
			return IsSequence(iBuffer,TackOutCount,4);

	case 3://��һ��
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 &&TackOutCount/4*6 == iCardCount 
			&&TackOutCount/4 == GetBulkBySpecifyCardCount(iCardList,iCardCount,2))
			return IsSequence(iBuffer,TackOutCount,4);

	case 4://(�����ԣ�
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 &&TackOutCount/4*6 == iCardCount
			&&TackOutCount/2 == GetBulkBySpecifyCardCount(iCardList,iCardCount,2))
			return IsSequence(iBuffer,TackOutCount,4);
		break;
	}	
	return FALSE;
}

//�ж��Ƿ���510K ը��
BOOL CUpGradeGameLogic::IsSlave510K(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if(iCardCount != 3) return false;
	BYTE Test[18]={0};
	for(int i = 0; i < iCardCount; i++)
	{
		Test[GetCardNum(i)] ++;
	}

	return (Test[5]==Test[10]==Test[13]==1);
}

//�ж��Ƿ���510K ͬ��
BOOL CUpGradeGameLogic::IsMaster510K(BYTE iCardList[], int iCardCount, bool bExtVal)
{
	if(iCardCount != 3) return false; //��������

	if( !IsSameHuaKind(iCardList, iCardCount, bExtVal) ) return false; //ͬ�� ��

	if( !IsSlave510K(iCardList, iCardCount, bExtVal) ) return false; //510K ��	

	return true;
}
////////////////////////////////////////////////////////////////////////////////////


//�Զ��ҳ����Գ�����
BOOL CUpGradeGameLogic::AutoOutCard(BYTE iHandCard[], int iHandCardCount, //��ǰ����������е�������
									BYTE iBaseCard[], int iBaseCardCount, //ǰһ�����Ƶ��˳���������
									BYTE iResultCard[], int & iResultCardCount, //�ҵ��Ľ��
									BOOL bFirstOut, BOOL bIsCue) //��ǰ����Ƿ�����
{
	iResultCardCount=0;
	if(bFirstOut) //���ֳ����ұ�һ����
	{
		TackOutCardBySpecifyCardNum(iHandCard, iHandCardCount, iResultCard, iResultCardCount, iHandCard[iHandCardCount-1]);
	}
	else //����
	{
		//�����е������ҳ��������ϴ����
		TackOutCardMoreThanLast(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount, false, bIsCue);
		if (!bIsCue)
		{
			if(!CanOutCard(iResultCard, iResultCardCount, iBaseCard, iBaseCardCount, iHandCard, iHandCardCount))
			{
				iResultCardCount = 0;
			}
		}
		//���Ӳ��ܵ����� 12.11.2008
	/*
		if (GetReplaceCardCount(iResultCard, iResultCardCount) == iResultCardCount) //20081110
		{
			iResultCardCount = 0;
		}
		*/
	
		
	}

	return TRUE;
}


//����һ���ȵ�ǰ���
BOOL CUpGradeGameLogic::TackOutCardMoreThanLast(BYTE iHandCard[], int iHandCardCount,
												BYTE iBaseCard[], int iBaseCardCount,
												BYTE iResultCard[], int &iResultCardCount, 
												bool bExtVal, BOOL bIsCue)
{
	BYTE iTempCard[54];
	iResultCardCount = 0;
	int iBaseShape = GetCardShape(iBaseCard, iBaseCardCount); //�������Ƶ�����
	//	TCHAR sz[200];
	//	wsprintf(sz,"��������iBaseShape=%d",iBaseShape);
	//	WriteStr(sz);
	switch(iBaseShape)
	{
	case UG_ONLY_ONE: //����
	case UG_DOUBLE:   //����
	case UG_THREE:    //����
	case UG_SHAM_BOMB: //��ը
		{
			//����1,2,3,or4����
			BYTE iCount = TackOutBySepcifyCardNumCount(iHandCard, iHandCardCount, iTempCard, iBaseCardCount);
			
			if(iCount > 0)
			{
				BYTE Step = GetSerialByMoreThanSpecifyCard(iTempCard, iCount, iBaseCard[0], iBaseCardCount, false);
				CopyMemory(iResultCard, &iTempCard[Step], sizeof(BYTE)*iBaseCardCount);	

				if(CompareOnlyOne(iBaseCard[0], iResultCard[0]))
				{
					iResultCardCount = iBaseCardCount;
					return TRUE;
				}
			}
			 if (bIsCue)
			{
				int kingcount = 0;
				int iNumCount = iBaseCardCount;
				BYTE iBuffer[54];
				if(GetReplaceCardCount(iHandCard,iHandCardCount))
				{
					TackOutByCondition(iHandCard,iHandCardCount,iBuffer,kingcount,m_iReplaceCardArray,m_iReplaceCardCount);
				}else
					break;
				int BaseCard = iBaseCard[0];
				if(GetReplaceCardCount(iBaseCard,iBaseCardCount))
				{
                      for (int i=1; i<iBaseCardCount; i++)
						  if (BaseCard!=iBaseCard[i])
						  {
							  BaseCard = iBaseCard[i];
							  break;
						  }
				}
				int ReplaceCount=1;
				for ( ;ReplaceCount<=kingcount; ReplaceCount++)
				{
					
					iNumCount = iBaseCardCount - ReplaceCount;
					iCount = TackOutBySepcifyCardNumCount(iHandCard, iHandCardCount, iTempCard, iNumCount);
					if(iCount > 0)
					{
						BYTE Step = GetSerialByMoreThanSpecifyCard(iTempCard, iCount, BaseCard, iNumCount, false);
						if (GetCardNum(iTempCard[Step]) > 14)
							break;
						CopyMemory(iResultCard, &iTempCard[Step], sizeof(BYTE)*iNumCount);	

						
						if(CompareOnlyOne(BaseCard, iResultCard[iNumCount-1]))
						{
							iResultCardCount = iNumCount;
							if (kingcount > 0)
							{
								for (int i=0; i<ReplaceCount; i++)
								{
									iResultCard[iNumCount+i] = iBuffer[i];
								}
								iResultCardCount = iBaseCardCount;
							}
							return TRUE;
						}

					}

					iResultCardCount = 0;
				}

			}

			break;
		}
		break;
	case UG_BOMB:	//���� ը��
		{
			//����1,2,3,or4����
			BYTE iCount = TackOutBySepcifyCardNumCount(iHandCard, iHandCardCount, iTempCard, iBaseCardCount);

			if(iCount > 0)
			{
				BYTE Step = GetSerialByMoreThanSpecifyCard(iTempCard, iCount, iBaseCard[0], iBaseCardCount, false);
				CopyMemory(iResultCard, &iTempCard[Step], sizeof(BYTE)*iBaseCardCount);	

				if(CompareOnlyOne(iBaseCard[0], iResultCard[0]))
					iResultCardCount = iBaseCardCount;
			}				
			break;			
		}
		//case UG_THREE:    //����Ҳ���������������ȡ
	case UG_THREE_ONE: //����һ
		{
			if(TackOutThreeX(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,1))
				return TRUE;
			if (bIsCue)
				if (TackOuttReplaceThreeX(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,1))
					return TRUE;
			break;
		}
	case UG_THREE_TWO: //��������
		{
			if(TackOutThreeX(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,2))
				return TRUE;
			if (bIsCue)
				if (TackOuttReplaceThreeX(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,2))
					return TRUE;
			break;
		}
	case UG_THREE_DOUBLE:	//����һ��
		{
			if(TackOutThreeX(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,3))
				return TRUE;
			if (bIsCue)
				if (TackOuttReplaceThreeX(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,3))
					return TRUE;
			break;
		}
	case UG_FLUSH:		//ͬ��
		{
			//if(TackOutStraightFlush(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount))
			//	return true;
			break;
		}

	case UG_STRAIGHT: //˳��
		/*if(TackOutStraightFlush(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount))
		{
		return TRUE; //������ͬ�Ƶ��ͬ��˳
		}*/
		if(TackOutSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,1))
		{
			return TRUE; //�����Ƶ���˳��
		}
		if (bIsCue)
			if (TackOutReplaceSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,1))
				return TRUE;
		break;
	case UG_STRAIGHT_FLUSH: //ͬ��˳
		{
			if(TackOutStraightFlush(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount))
				return true;
		}
		break;
	case UG_DOUBLE_SEQUENCE: //����
		{
			if(TackOutSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,2))
			{
				return TRUE; //�����Ƶ���˳��
			}
			if (bIsCue)
				if (TackOutReplaceSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,2))
					return TRUE;
					break;
		}
	case UG_THREE_SEQUENCE: //����
		{
			if(TackOutSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,3))
			{
				return TRUE;
			}
			if (bIsCue)
				if (TackOutReplaceSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,3))
					return TRUE;
					break;
		}

	case UG_THREE_ONE_SEQUENCE: //����һ������
		{
			if(TrackOut3XSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,1))
				break;
		}
	case UG_THREE_TWO_SEQUENCE: //������������
		if(TrackOut3XSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,2))
			//if(TrackOut3XSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount))
		{
			return TRUE;
		}	
	case UG_THREE_DOUBLE_SEQUENCE://����������
		{
			if(TrackOut3XSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,3))
				return TRUE;
			break;
		}	
	case UG_THREE_SEQUENCE_DOUBLE_SEQUENCE:
		{
			if(TrackOut3Sequence2Sequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount))
				return true;
			break;
		}
		//case UG_FOUR_ONE_SEQUENCE:

	case UG_SLAVE_510K: //ֻ����ͬ����ѹ,�����Ҵ������,�������������
		//	break;
	case UG_MASTER_510K:
		TrackOut510K(iHandCard, iHandCardCount, iResultCard, iResultCardCount, true); //�ҳ�ͬ�� 510K
		if(GetCardHuaKind(iBaseCard[0], TRUE) >= GetCardHuaKind(iResultCard[0], TRUE)) //�Ƚϻ�ɫ
		{
			iResultCardCount = 0;
		}
		break;
	default:
		iResultCardCount = 0;
	}

	if(iResultCardCount == 0) //û�ҵ�ͬ���͵Ĵ���,���Ҵ�һ�������
	{
		switch(iBaseShape)
		{
		case UG_ONLY_ONE: //���Բ����,��������ѹ���ƻ��߶���
		case UG_DOUBLE:
			{
				if(TackOutCardByNoSameShape(iHandCard, iHandCardCount, iResultCard, iResultCardCount, iBaseCard, iBaseCardCount))
					return TRUE;
				break;
			}
		case UG_BOMB:
			{
				//����û�ҵ���ͬ�����Ĵ�ը��,���������������
				if(TackOutBomb(iHandCard, iHandCardCount, iResultCard, iResultCardCount, iBaseCardCount+1))
					return TRUE;
			}
			break;
		default: //����Ҳ�����Ķ��ӵ��ƾ��Ҵ������,warning�˴�����break;
			break;
		}
	}

	if(iResultCardCount == 0)
		TackOutMoreThanLastShape(iHandCard, iHandCardCount, iResultCard, iResultCardCount, iBaseCard, iBaseCardCount);
	//				if(TackOutMoreThanLastShape(iHandCard, iHandCardCount, iResultCard, iResultCardCount, iBaseCard, iBaseCardCount))
	//				{
	//					return TRUE;
	//				}
	return TRUE;
}

//ֱ����ȡ�������ϴ������
BOOL  CUpGradeGameLogic::TackOutMoreThanLastShape(BYTE iCardList[], int iCardCount, 
												  BYTE iResultCard[], int &iResultCardCount, 
												  BYTE iBaseCard[], int iBaseCardCount)
{
	//AfxMessageBox("");
	iResultCardCount = 0;
	int ishape = GetCardShape(iBaseCard, iBaseCardCount);
	switch(ishape)
	{
	case UG_ONLY_ONE: //����
	case UG_DOUBLE:   //����
	case UG_THREE:    //����

	case UG_DOUBLE_SEQUENCE: //����
	case UG_THREE_SEQUENCE:  //������
	case UG_THREE_ONE:				//����1
	case UG_THREE_ONE_SEQUENCE:		//����˳
	case UG_THREE_TWO:				//����2
	case UG_THREE_TWO_SEQUENCE:		//����2˳
	case UG_THREE_DOUBLE:			//������
	case UG_THREE_DOUBLE_SEQUENCE:	//������˳


	case UG_FOUR_ONE:						//�Ĵ�һ
	case UG_FOUR_TWO:						//�Ĵ���
	case UG_FOUR_ONE_DOUBLE:				//�Ĵ�һ��
	case UG_FOUR_TWO_DOUBLE:				//�Ĵ�����

	case UG_STRAIGHT:				 //˳��
	case UG_STRAIGHT_FLUSH:			//ͬ��˳
		
		if (TackOutShamBomb(iCardList,iCardCount,iResultCard,iResultCardCount))
		{
		//	AfxMessageBox("ShamBomb");
			break;
		}
		
	case UG_SHAM_BOMB:
		if (TackOutBomb(iCardList,iCardCount,iResultCard,iResultCardCount)) //��ը��
		{
		//	AfxMessageBox("Bomb");
			break;
		}
	case UG_BOMB:
		if (TackOutKingBomb(iCardList,iCardCount,iResultCard,iResultCardCount)) //��ը��
		{
		//	AfxMessageBox("KingBomb");
			break;
		}
	//	AfxMessageBox("default");
	case UG_KING_BOMB:
		break;
	default:
		break;
	}
	return true;
}

//��ȡ����������0, 1 or 2 ���״����Ǽ�,�� iBaseCount-3 ������
BYTE CUpGradeGameLogic::TackOutThreeX(BYTE iCardList[], int iCardCount, 
									  BYTE iBaseCard[], int iBaseCount, 
									  BYTE iResultCard[], int &iResultCount, int iValue)
{
	iResultCount = 0;
	if(iCardCount<iBaseCount)
		return FALSE;
	BYTE iTempCard[54];
	int threecard = GetBulkBySpecifyCardCount(iBaseCard,iBaseCount,3);//���������ŵĵ���
	//3�����ܸ���
	BYTE iCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iTempCard, 3);

	if(iCount > 0)//��ȡ�������������
	{
		BYTE byCardTemp = 0x00;
		for (int i=0; i<iBaseCount; ++i)
		{
			if (threecard == GetCardBulk(iBaseCard[i]))
			{
				byCardTemp = iBaseCard[i];
				break;
			}
		}
		if (0x00 == byCardTemp)
		{
			return FALSE;
		}

		BYTE Step = GetSerialByMoreThanSpecifyCard(iTempCard, iCount, byCardTemp, 3, true);//����ֵ��ȥ

		CopyMemory(iResultCard, &iTempCard[Step], sizeof(BYTE)*3);	

		if(threecard >= GetBulkBySpecifyCardCount(iResultCard,3,3))
			return FALSE;

	}else 
		return FALSE;
	//��ԭֵ����
	BYTE Tmp[54];
	int iTempCount=iCardCount;
	::CopyMemory(Tmp,iCardList,sizeof(BYTE)*iCardCount);
	RemoveCard(iResultCard,3,Tmp,iTempCount);
	iTempCount-=3;
	int destCount = iBaseCount - 3;

	switch(iValue)
	{
	case 1:
	case 2:
		{	
			iCount=TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,1);
			if(iCount >= destCount)//���ҵ�����
			{
				CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
				iResultCount = iBaseCount;
				break;
			}
			//�����������
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,2);
			if(iCount >= destCount)
			{
				CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
				iResultCount = iBaseCount;
				break;
			}
			//��������������
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,3);
			if(iCount < 3)//��һ�����޷���
				break;
			CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
			iResultCount=iBaseCount;
			break;
		}
	case 3:
		{
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,2);
			if(iCount > 0)
			{
				CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
				iResultCount = iBaseCount;
				break;
			}
			//��������������
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,3);
			if(iCount < 3)//��һ�����޷���
				break;
			CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
			iResultCount=iBaseCount;
			break;

		}
	default:
		iResultCount = 0;
		break;
	}
	//		wsprintf(sz,"iResultCount=%d,iBaseCount=%d",iResultCount,iBaseCount);
	//	WriteStr(sz,8,8);
	if(iResultCount == iBaseCount )
		return  TRUE;
	iResultCount =0;
	return FALSE;
	//return (iResultCount == iBaseCount);
}

//��ȡ����������0, 1 or 2 ���״����Ǽ�,�� iBaseCount-3 ������
BYTE CUpGradeGameLogic::TackOuttReplaceThreeX(BYTE iCardList[], int iCardCount, 
									  BYTE iBaseCard[], int iBaseCount, 
									  BYTE iResultCard[], int &iResultCount, int iValue)
{
	iResultCount = 0;
	if(iCardCount<iBaseCount)
		return FALSE;
	int kingcount = 0;
	BYTE iBuffer[54];
	if(GetReplaceCardCount(iCardList,iCardCount))
	{
		TackOutByCondition(iCardList,iCardCount,iBuffer,kingcount,m_iReplaceCardArray,m_iReplaceCardCount);
	}else
		return FALSE;
	int count = 3 - kingcount;
	if (count <= 0)
		return FALSE;

	BYTE iTempCard[54];
	int threecard = GetBulkBySpecifyCardCount(iBaseCard,iBaseCount,3);//���������ŵĵ���
	//3�����ܸ���
	BYTE iCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iTempCard, count);

	if(iCount > 0)//��ȡ�������������
	{
		BYTE Step = GetSerialByMoreThanSpecifyCard(iTempCard, iCount, threecard, count, true);//����ֵ��ȥ

		if (iTempCard[Step] == 0x4e || iTempCard[Step] == 0x4f)
			return FALSE;
		CopyMemory(iResultCard, &iTempCard[Step], sizeof(BYTE)*count);	

		if(threecard >= GetBulkBySpecifyCardCount(iResultCard,count,count))
			return FALSE;
		CopyMemory(&iResultCard[count], iBuffer, sizeof(BYTE)*kingcount);

	}else 
		return FALSE;
	//��ԭֵ����
	BYTE Tmp[54];
	int iTempCount=iCardCount;
	::CopyMemory(Tmp,iCardList,sizeof(BYTE)*iCardCount);
	RemoveCard(iResultCard,3,Tmp,iTempCount);
	iTempCount-=3;
	int destCount = iBaseCount - 3;

	switch(iValue)
	{
	case 1:
	case 2:
		{	
			iCount=TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,1);
			if(iCount >= destCount)//���ҵ�����
			{
				CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
				iResultCount = iBaseCount;
				break;
			}
			//�����������
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,2);
			if(iCount >= destCount)
			{
				CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
				iResultCount = iBaseCount;
				break;
			}
			//��������������
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,3);
			if(iCount < 3)//��һ�����޷���
				break;
			CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
			iResultCount=iBaseCount;
			break;
		}
	case 3:
		{
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,2);
			if(iCount > 0)
			{
				CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
				iResultCount = iBaseCount;
				break;
			}
			//��������������
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,3);
			if(iCount < 3)//��һ�����޷���
				break;
			CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
			iResultCount=iBaseCount;
			break;

		}
	default:
		iResultCount = 0;
		break;
	}
	//		wsprintf(sz,"iResultCount=%d,iBaseCount=%d",iResultCount,iBaseCount);
	//	WriteStr(sz,8,8);
	if(iResultCount == iBaseCount )
		return  TRUE;
	iResultCount =0;
	return FALSE;
	//return (iResultCount == iBaseCount);
}

//��ȡ����
BOOL  CUpGradeGameLogic::TrackOut3Sequence2Sequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount)
{
	iResultCardCount = 0;
	if(iCardCount < iBaseCount)	//��������
		return false;
	BYTE tmpBaseCard[54];//,destCard[54];
	int tmpbaseCardCount =0,destCardCount =0;
	//�������Ƶ������������
	tmpbaseCardCount =TackOutBySepcifyCardNumCount(iBaseCard,iBaseCount,tmpBaseCard,3);
	if(tmpbaseCardCount < 6)	//������������
		return FALSE;
	//����ȡ����������˳
	if(!TackOutSequence(iCardList,iCardCount,tmpBaseCard,tmpbaseCardCount,iResultCard,iResultCardCount,3))
		return FALSE;
	//�����Ƹ���һ��(�Ƴ���˳��)
	BYTE TMP[54];
	int TmpCount = iCardCount ;
	::CopyMemory(TMP,iCardList,sizeof(BYTE)*iCardCount);
	RemoveCard(iResultCard,iResultCardCount,TMP,TmpCount);
	TmpCount -= iResultCardCount;
	destCardCount = iBaseCount - iResultCardCount;	//��������

	BYTE twoList[54];
	int twoCount;
	//�������ƵĶ�˳�������
	tmpbaseCardCount =TackOutBySepcifyCardNumCount(iBaseCard,iBaseCount,tmpBaseCard,2);
	if(!TackOutSequence(TMP,TmpCount,tmpBaseCard,tmpbaseCardCount,twoList,twoCount,2,true))
		return false;
	//	int TwoSequenceLen = (iBaseCount- tmpbaseCardCount)/2;
	//	tmpbaseCardCount =TackOutBySepcifyCardNumCount(TMP,TmpCount,tmpBaseCard,3);
	::CopyMemory(&iResultCard[iResultCardCount],twoList,sizeof(BYTE)*twoCount);
	iResultCardCount +=twoCount;
	return true;
}
//��ȡָ��������˳
BOOL  CUpGradeGameLogic::TrackOut3XSequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount, int xValue)
{
	iResultCardCount=0;
	if(iCardCount < iBaseCount)	//��������
		return false;
	BYTE tmpBaseCard[54];//,destCard[54];
	int tmpbaseCardCount =0,destCardCount =0;
	//�������Ƶ������������
	tmpbaseCardCount =TackOutBySepcifyCardNumCount(iBaseCard,iBaseCount,tmpBaseCard,3);
	if(tmpbaseCardCount < 6)	//������������
		return FALSE;
	//TCHAR sz[200];
	//wsprintf(sz,"��˳����ȡ֮ǰ%d",iResultCardCount);
	//WriteStr(sz);	
	//����ȡ����������˳
	if(!TackOutSequence(iCardList,iCardCount,tmpBaseCard,tmpbaseCardCount,iResultCard,iResultCardCount,3))
		return FALSE;
	//TCHAR sz[200];
	//wsprintf(sz,"��˳����ȡ�ɹ�%d",iResultCardCount);
	//WriteStr(sz);
	//�����Ƹ���һ��
	BYTE TMP[54];
	int TmpCount = iCardCount ;
	::CopyMemory(TMP,iCardList,sizeof(BYTE)*iCardCount);
	RemoveCard(iResultCard,iResultCardCount,TMP,TmpCount);
	TmpCount -= iResultCardCount;
	destCardCount = iBaseCount - iResultCardCount;	//��������

	switch(xValue)
	{
	case 1:
	case 2:
		{
			tmpbaseCardCount =TackOutBySepcifyCardNumCount(TMP,TmpCount,tmpBaseCard,1);//�յ���
			if(tmpbaseCardCount >= destCardCount)
			{
				::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*destCardCount);//����
				iResultCardCount += destCardCount;
			}
			else
			{
				::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*tmpbaseCardCount);
				iResultCardCount += tmpbaseCardCount;
				destCardCount -= tmpbaseCardCount;
				tmpbaseCardCount =TackOutBySepcifyCardNumCount(TMP,TmpCount,tmpBaseCard,2);//�ö��Ʋ�
				if(tmpbaseCardCount>=destCardCount)
				{
					::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*destCardCount);
					iResultCardCount += destCardCount;
				}
				else
				{
					::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*tmpbaseCardCount);
					iResultCardCount += tmpbaseCardCount;
					destCardCount -= tmpbaseCardCount;
					tmpbaseCardCount =TackOutBySepcifyCardNumCount(TMP,TmpCount,tmpBaseCard,3);//��������
					//
					if(tmpbaseCardCount>=destCardCount)
					{
						::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*destCardCount);
						iResultCardCount += destCardCount;
					}
				}
			}
			break;
		}
	case 3:
		{
			tmpbaseCardCount =TackOutBySepcifyCardNumCount(TMP,TmpCount,tmpBaseCard,2);//�ն���
			if(tmpbaseCardCount>=destCardCount)
			{
				::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*destCardCount);
				iResultCardCount += destCardCount;
			}
			else
			{
				::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*tmpbaseCardCount);
				iResultCardCount += tmpbaseCardCount;
				destCardCount -= tmpbaseCardCount;
				//tmpbaseCardCount =TackOutBySepcifyCardNumCount(TMP,TmpCount,tmpBaseCard,3);//����������
				TackOutMuchToFew(TMP,TmpCount,tmpBaseCard,tmpbaseCardCount,3,2);	//������������ɶ�����
				if(tmpbaseCardCount >=destCardCount)//������Թ���
				{
					::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*destCardCount);
					iResultCardCount += destCardCount;
				}
			}
			break;
		}
	default:
		break;
	}
	//wsprintf(sz,"iResultCardCount=%d,iBaseCount=%d",iResultCardCount,iBaseCount);
	//WriteStr(sz);
	if(iResultCardCount == iBaseCount )
		return true;
	iResultCardCount =0;
	return FALSE;
	return (iResultCardCount == iBaseCount);
	//	return FALSE;
}

/*
//��ȡ2����������������1,2
BYTE CUpGradeGameLogic::TrackOut3XSequence(BYTE iCardList[], int iCardCount, 
BYTE iBaseCard[], int iBaseCount, 
BYTE iResultCard[], int &iResultCardCount, bool bExtVal)
{
//��������������м�������,�Լ���С�����ŵĵ���
int BaseBulkCount[17] = {0};
for(int num = 0; num < iBaseCount; num++)
{
BYTE tmp = GetCardBulk(iBaseCard[num]);
BaseBulkCount[tmp]++;
}

int BaseCountThree = 0; //base�������Ƶĸ���
int BaseMinCard = 0;    //��С�������Ƶ��Ƶ�
for(int count = 0; count < 17; count++)
{
if(BaseBulkCount[count] == 3)
{
if(BaseMinCard == 0)
{
BaseMinCard = count;
}
BaseCountThree++;
}
}

int BulkCount[17] = {0}; //ע��,������ʼ��,ֻ�ܱ�֤��һ����0,����Ķ��Ǳ������Զ���

for(int k = 0; k < iCardCount; k++)
{
BYTE CardBulk = GetCardBulk(iCardList[k]);
BulkCount[CardBulk]++;
}

int MyThreeCount = 0; //���ŵ�����
int MinCardNum = 0; //�ҵ�����������С���Ƶ�
for(int k = 0; k < 17; k++)
{
if( BulkCount[k] == 3 && //����
k > BaseMinCard && //�Ƶ�Ҫ����������С���Ǹ���
k < 15 ) //2���߿�
{
MyThreeCount++;
if(MyThreeCount >= BaseCountThree) //��������
{
MinCardNum = k - MyThreeCount + 1;
break;
}
}
else if(BulkCount[k] != 3 && MyThreeCount > 0 && MyThreeCount < BaseCountThree)
{
//ǰ���Ѿ�������,����ʱ�Ѿ�������
MyThreeCount = 0;
}
}

if(MyThreeCount < BaseCountThree) return FALSE;

//Ѱ�Ҵ��ĵ���
int CountOfX = iBaseCount - 3*BaseCountThree; //�����Ƶĸ���

int iCountX = 0; //����
int iCardX[30] = {0}; //����������м���? 13*2?
for(int one = 0; one < 17; one++)
{
if(BulkCount[one] == 1 && one < 14)
{
iCardX[iCountX++] = one;
}
}

//����û�ҹ�,�Ҷ���
if(iCountX < CountOfX)
{
for(int two = 0; two < 17; two++)
{
if(BulkCount[two] == 2 && two < 14)
{
iCardX[iCountX++] = two;
iCardX[iCountX++] = two; //Ҫд2��
}
}
}

if(iCountX < CountOfX) return FALSE;

BYTE tempCardList[26] = {0};
CopyMemory(tempCardList, iCardList, iCardCount);

iResultCardCount = 0;
//�� ���� �ȸ㵽���������
for(int j = 0; j < iCardCount; j++)
{
BYTE tmpbulk = GetCardBulk(tempCardList[j]);
if( BulkCount[tmpbulk] == 3 && //����
MinCardNum <= tmpbulk &&   //������С�����ŵĵ���
(tmpbulk - MinCardNum) < MyThreeCount ) //���ĸ���
{
iResultCard[iResultCardCount++] = tempCardList[j];
tempCardList[j] = 0;
}
}

for(int x = 0; x < CountOfX; x++)
{
if(iCardX[x] == 0) break;

for(int j = 0; j < iCardCount; j++)
{
if( tempCardList[j] != 0 &&
GetCardBulk(tempCardList[j]) == iCardX[x] )
{
iResultCard[iResultCardCount++] = tempCardList[j];
tempCardList[j] = 0;
break;
}
}

if(iResultCardCount >= iBaseCount)
{
return TRUE;
}
}

return FALSE;
}
*/
//��д��ȡ���ŵ�˳��,���� or ����
BOOL CUpGradeGameLogic::TackOutSequence(BYTE iCardList[], int iCardCount, //���е���
										BYTE iBaseCard[], int iBaseCount,   //������������, �Ƶĸ���
										BYTE iResultCard[], int &iResultCount, //�ҵ�����
										int xSequence,BOOL bNoComp)							//˳�ӵĸ���
{
	iResultCount=0;
	BYTE iTack[54];
	int iTackCount=iCardCount;
	//����һ��
	::CopyMemory(iTack,iCardList,sizeof(BYTE)*iCardCount);
	BYTE iBuffer[54];
	int iBufferCount=0;
	int iBaseStart,iDestStart=0,iDestEnd=0;
	int iSequenceLen=iBaseCount;
	int temp[18]={0};
	int num=0;
	//��ȡ����ը��(������ɾ������ը��)
	TackOutAllBomb(iTack,iTackCount,iBuffer,iBufferCount);
	RemoveCard(iBuffer,iBufferCount,iTack,iTackCount);
	iTackCount-=iBufferCount;
	//����һ��ϵͳ����������(����������С->�����
	SortCard(iTack,NULL,iTackCount,TRUE);
	//�û����������
	for(int i=0;i<iTackCount;i++)
	{
		temp[GetCardBulk(iTack[i])]++;
	}

	switch(xSequence)
	{
		//��˳
	case 1:
		iSequenceLen = iBaseCount;
		if(!bNoComp)
			iBaseStart = GetSequenceStartPostion(iBaseCard,iBaseCount,1);
		else
			iBaseStart = 3;
		for(int i=iBaseStart+1;i<15;i++)
		{
			if(temp[i]>=1)
			{
				if(iDestStart == 0)
					iDestStart = i;
				iDestEnd++;
				if(iDestEnd == iSequenceLen)
					break;
			}else
			{
				iDestStart = 0;
				iDestEnd = 0;
			}
		}
		if(iDestEnd != iSequenceLen)
			return false;
		//��ȡ����
		for(int j=0;j<iTackCount;j++)
		{
			if(GetCardBulk(iTack[j]) == iDestStart)//�ҵ�һ����
			{
				iResultCard[iResultCount++] = iTack[j];
				iDestStart++;
				iDestEnd--;
				//break;
			}
			//�Ѿ���ȫ
			if(iDestEnd == 0)
			{
				return true;
			}
		}
		break;
	case 2:
		iSequenceLen = iBaseCount/2;
		if(!bNoComp)
			iBaseStart=GetSequenceStartPostion(iBaseCard,iBaseCount,2);
		else
			iBaseStart =3;
		for(int i=iBaseStart+1;i<15;i++)
		{
			if(temp[i] >= 2)
			{
				if(iDestStart == 0)
					iDestStart = i;
				iDestEnd++;
				if(iDestEnd == iSequenceLen)
					break;
			}else
			{
				iDestStart = 0;
				iDestEnd = 0;
			}
		}
		if(iDestEnd != iSequenceLen)
			return false;
		num=0;
		//��ȡ����
		for(int j=0;j<iTackCount;j++)
		{
			if(GetCardBulk(iTack[j]) == iDestStart)
			{
				iResultCard[iResultCount++] = iTack[j];
				num++;
			}

			if(num ==2)//һ���Ѿ��ҵ�
			{
				num=0;
				iDestStart++;
				iDestEnd--;
				//�Ѿ���ȫ
				if(iDestEnd == 0)
					return true;
				//break;
				//i = 0;
				//continue;
			}
		}
		break;
	case 3:
		iSequenceLen = iBaseCount/3;
		if(!bNoComp)
			iBaseStart=GetSequenceStartPostion(iBaseCard,iBaseCount,3);
		else
			iBaseStart = 3;
		for(int i=iBaseStart+1;i<15;i++)
		{
			if(temp[i] >= 3)
			{
				if(iDestStart == 0)
					iDestStart = i;
				iDestEnd++;
				if(iDestEnd == iSequenceLen)
					break;
			}else
			{
				iDestStart = 0;
				iDestEnd = 0;
			}
		}
		if(iDestEnd != iSequenceLen)
			return false;
		num=0;
		//��ȡ����
		for(int j=0;j<iTackCount;j++)
		{
			if(GetCardBulk(iTack[j]) == iDestStart)
			{
				iResultCard[iResultCount++] = iTack[j];
				num++;

				if(num == 3)//�ҵ�����
				{
					num=0;
					iDestStart++;
					iDestEnd--;
					//�Ѿ���ȫ
					if(iDestEnd == 0)
						return true;
				}
			}
		}

		break;
	}
	return FALSE;
}

//��д��ȡ���ŵ�˳��,���� or ����
BOOL CUpGradeGameLogic::TackOutReplaceSequence(BYTE iCardList[], int iCardCount, //���е���
										BYTE iBaseCard[], int iBaseCount,   //������������, �Ƶĸ���
										BYTE iResultCard[], int &iResultCount, //�ҵ�����
										int xSequence,BOOL bNoComp)							//˳�ӵĸ���
{
	int ReplaceCount = GetReplaceCardCount(iCardList,iCardCount);
	//if (ReplaceCount==0 || ReplaceCount >= 3)
	 if (ReplaceCount==0)
		return FALSE;
	int TempCount = ReplaceCount;
	int kingcount = 0;
	BYTE iReplaceBuffer[54];
	TackOutByCondition(iCardList,iCardCount,iReplaceBuffer,kingcount,m_iReplaceCardArray,m_iReplaceCardCount);
	iResultCount=0;
	BYTE iTack[54];
	int iTackCount=iCardCount;
	//����һ��
	::CopyMemory(iTack,iCardList,sizeof(BYTE)*iCardCount);
	BYTE iBuffer[54];
	int iBufferCount=0;
	int iBaseStart,iDestStart=0,iDestEnd=0;
	int iSequenceLen=iBaseCount;
	int temp[18]={0};
	int num=0;
	//��ȡ����ը��(������ɾ������ը��)
	TackOutAllBomb(iTack,iTackCount,iBuffer,iBufferCount);
	RemoveCard(iBuffer,iBufferCount,iTack,iTackCount);
	iTackCount-=iBufferCount;
	RemoveCard(iReplaceBuffer,kingcount,iTack,iTackCount);
	iTackCount-=kingcount;
	//����һ��ϵͳ����������(����������С->�����
	SortCard(iTack,NULL,iTackCount,TRUE);
	//�û����������
	for(int i=0;i<iTackCount;i++)
	{
		temp[GetCardBulk(iTack[i])]++;
	}

	switch(xSequence)
	{
		//��˳
	case 1:
		iSequenceLen = iBaseCount;
		if(!bNoComp)
			iBaseStart = GetSequenceStartPostion(iBaseCard,iBaseCount,1);
		else
			iBaseStart = 3;
		for(int i=iBaseStart+1;i<15;i++)
		{
			if(temp[i]>=1)
			{
				if(iDestStart == 0)
					iDestStart = i;
				iDestEnd++;
				if(iDestEnd == iSequenceLen)
					break;
			}else
			{
				if (TempCount > 1)
				{
					TempCount = TempCount - 1;
					if(iDestStart == 0)
						iDestStart = i;
					iDestEnd++;
					if(iDestEnd == iSequenceLen)
						break;
				}
				else
				{

					i = i - iDestEnd;
					iDestStart = 0;
					iDestEnd = 0;
					TempCount = ReplaceCount;
				}
			}
		}
		if(iDestEnd != iSequenceLen)
			return false;
		//��ȡ����
		for(int j=0;j<iTackCount;j++)
		{
			
			if(GetCardBulk(iTack[j]) == iDestStart)//�ҵ�һ����
			{
				iResultCard[iResultCount++] = iTack[j];
				iDestStart++;
				iDestEnd--;
				//break;
			}
			else
			{
				if (GetCardBulk(iTack[j]) > iDestStart)
				{
					iResultCard[iResultCount++] = iReplaceBuffer[TempCount-1];
					TempCount++;
					iDestStart++;
					iDestEnd--;
					j--;
				}
			}
			//�Ѿ���ȫ
			if(iDestEnd == 0)
			{
				return true;
			}
		}
		break;
	case 2:
		iSequenceLen = iBaseCount/2;
		if(!bNoComp)
			iBaseStart=GetSequenceStartPostion(iBaseCard,iBaseCount,2);
		else
			iBaseStart =3;
		for(int i=iBaseStart+1;i<15;i++)
		{
			if(temp[i] >= 2)
			{
				if(iDestStart == 0)
					iDestStart = i;
				iDestEnd++;
				if(iDestEnd == iSequenceLen)
					break;
			}
			else
			{
				if (TempCount >= (2 - temp[i]))
				{
					TempCount = (TempCount - (2 - temp[i]));
					if(iDestStart == 0)
						iDestStart = i;
					iDestEnd++;
					if(iDestEnd == iSequenceLen)
						break;
				}
				else
				{
					i = i - iDestEnd;
					iDestStart = 0;
					iDestEnd = 0;
					TempCount = ReplaceCount;
				}
			}
		}
		if(iDestEnd != iSequenceLen)
			return false;
		num=0;
		//��ȡ����
		for(int j=0;j<iTackCount;j++)
		{
			
			if(GetCardBulk(iTack[j]) == iDestStart)
			{
				iResultCard[iResultCount++] = iTack[j];
				num++;
			}
			else
			{
				if (GetCardBulk(iTack[j]) > iDestStart)
				{
					TempCount++;
					iResultCard[iResultCount++] = iReplaceBuffer[TempCount-1];
					
					num++;
					j--;
				}
			}

			if(num ==2)//һ���Ѿ��ҵ�
			{
				num=0;
				iDestStart++;
				iDestEnd--;
				//�Ѿ���ȫ
				if(iDestEnd == 0)
					return true;
			}
		}
		break;
	case 3:
		iSequenceLen = iBaseCount/3;
		if(!bNoComp)
			iBaseStart=GetSequenceStartPostion(iBaseCard,iBaseCount,3);
		else
			iBaseStart = 3;
		for(int i=iBaseStart+1;i<15;i++)
		{
			if(temp[i] >= 3)
			{
				if(iDestStart == 0)
					iDestStart = i;
				iDestEnd++;
				if(iDestEnd == iSequenceLen)
					break;
			}else
			{
				if (TempCount >= (3 - temp[i]))
				{
					TempCount = (TempCount - (3 - temp[i]));
					if(iDestStart == 0)
						iDestStart = i;
					iDestEnd++;
					if(iDestEnd == iSequenceLen)
						break;
				}
				else
				{
					i = i - iDestEnd;
					iDestStart = 0;
					iDestEnd = 0;
					TempCount = ReplaceCount;
				}
			}
		}
		if(iDestEnd != iSequenceLen)
			return false;
		num=0;
		//��ȡ����
		for(int j=0;j<iTackCount;j++)
		{
			if(GetCardBulk(iTack[j]) == iDestStart)
			{
				iResultCard[iResultCount++] = iTack[j];
				num++;
			}else
			{
				if (GetCardBulk(iTack[j]) > iDestStart)
				{
					TempCount++;
					iResultCard[iResultCount++] = iReplaceBuffer[TempCount-1];
					
					num++;
					j--;
				}
			}
			if(num == 3)//�ҵ�����
			{
				num=0;
				iDestStart++;
				iDestEnd--;
				//�Ѿ���ȫ
				if(iDestEnd == 0)
					return true;
			}

		}

		break;
	}
	return FALSE;
}
//��ȡͬ���
BOOL CUpGradeGameLogic::TackOutStraightFlush(BYTE iCardList[],int iCardCount,BYTE iBaseCard[],int iBaseCount,BYTE iResultCard[], int &iResultCardCount)
{
	iResultCardCount=0;
	if(iCardCount < iBaseCount)
		return false;
	BYTE iBaseMinCard = GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 1);//�����˳������С����
	BYTE iTack[54];
	int iTackCount=iCardCount;
	//����һ��
	::CopyMemory(iTack,iCardList,sizeof(BYTE)*iCardCount);
	BYTE iBuffer[54];
	int iBufferCount=0;
	int iDestStart=0,iDestEnd=0;
	int iSequenceLen=iBaseCount;
	int temp[18]={0};
	int num=0;
	//��ȡ����ը��(������ɾ������ը��)
	TackOutAllBomb(iTack,iTackCount,iBuffer,iBufferCount);
	RemoveCard(iBuffer,iBufferCount,iTack,iTackCount);
	iTackCount-=iBufferCount;

	SortCard(iTack,NULL,iTackCount,TRUE);

	BYTE iTempKind[54];
	int iTempKindCount =0;
	//TCHAR sz[200];
	//wsprintf(sz,"iTackCount=%d,iBaseCount=%d",iTackCount,iBaseCount);
	//WriteStr(sz);
	//�û����������
	for(int kind = 0;kind<= 48 ;kind+=16)
	{	//��ȡ����
		iResultCardCount=0;
		iTempKindCount = TackOutByCardKind(iTack,iTackCount,iTempKind,kind);
		//wsprintf(sz,"kind=%d,iTempKindCount=%d",kind,iTempKindCount);
		//WriteStr(sz);
		if(iTempKindCount >=iBaseCount )					//��������
		{
			for(int i = 0 ;i < iTempKindCount;i++)
			{
				temp[GetCardBulk(iTempKind[i])]++;
			}

			for(int i = iBaseMinCard+1;i<15;i++)//�Զ������б���
			{
				if(temp[i]>=1)		//ĳ��ɫ����
				{
					if(iDestStart == 0)
						iDestStart = i;
					iDestEnd++;
					if(iDestEnd == iSequenceLen)
						break;
				}else
				{
					iDestStart = 0;
					iDestEnd = 0;
				}
			}
			//wsprintf(sz,"iDestEnd=%d,iCardCount=%d",iDestEnd,iCardCount);
			//WriteStr(sz);

			if(iDestEnd != iBaseCount)	//ĳ�ֻ�ɫ������,������һ�ֻ�ɫ
				continue;
			//��ȡ����
			for(int j=0;j<iTempKindCount;j++)
			{
				if(GetCardBulk(iTempKind[j]) == iDestStart)
				{
					iResultCard[iResultCardCount++] = iTempKind[j];
					iDestStart++;
					iDestEnd--;
				}
				//�Ѿ���ȫ
				if(iDestEnd == 0)
					return true;
			}
		}

	}

	return FALSE;
}

//�õ�˳�ӵ���ʼλ��
int CUpGradeGameLogic::GetSequenceStartPostion(BYTE iCardList[],int iCardCount,int xSequence)
{
	BYTE temp[18]={0};
	int Postion=0;
	for(int i=0;i<iCardCount;i++)
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for(int i=0;i<18;i++)
	{
		if(temp[i] == xSequence)
			return i;
	}
	return Postion;
}
/*
//��ȡ���ŵ�˳��,���� or ����
BYTE CUpGradeGameLogic::TackOutSequence(BYTE iCardList[], int iCardCount, //���е���
BYTE iBaseCard[], int iBaseCount,   //������������, �Ƶĸ���
BYTE iResultCard[], int &iResultCount //�ҵ�����
)
{
iResultCount = 0;

int BaseBulkCount[17] = {0};
for(int base = 0; base < iBaseCount; base++)
{
BaseBulkCount[GetCardBulk(iBaseCard[base])]++;
}

int iXSeq = 0; //1,2,3����?
BYTE iBaseMinCard = 0; //��С���Ƶ�
for(int k = 0; k < 17; k++)
{
if(BaseBulkCount[k] != 0)
{
iXSeq = BaseBulkCount[k];
iBaseMinCard = k;
break;
}
}

int BulkCount[17] = {0};
//����ÿ���Ƶ������
for(int num = 0; num < iCardCount; num++)
{
BYTE iCardBulk = GetCardBulk( iCardList[num] );
BulkCount[iCardBulk]++;
}

int NumOfSeq = 0; //���ӻ����ŵ�����
int MinCardNum = 0; //�ҵ�����������С���Ƶ�
for(int k = 0; k < 17; k++)
{
if( BulkCount[k] >= iXSeq && //�Ƶĸ���Ҫ��
k > iBaseMinCard && //�Ƶ�Ҫ����������С���Ǹ���
k < 15 ) //2���߿�
{
NumOfSeq++;
if(NumOfSeq >= iBaseCount/iXSeq) //��������
{
MinCardNum = k - NumOfSeq + 1;
break;
}
}
else if(BulkCount[k] < iXSeq && NumOfSeq > 0 && NumOfSeq < iBaseCount/iXSeq)
{
//ǰ���Ѿ��ж��ӻ�����,����ʱ�Ѿ�������,����
NumOfSeq = 0;
}
}

if(NumOfSeq < iBaseCount/iXSeq) return FALSE; //��������

int ResCard[17] = {0}; //����ÿ���Ƶ��Ѿ��м�����
for(int j = 0; j < iCardCount; j++)
{
BYTE tmpCardBulk = GetCardBulk( iCardList[j] );
if( (tmpCardBulk - MinCardNum) >= 0 && (tmpCardBulk - MinCardNum) < NumOfSeq &&
BulkCount[tmpCardBulk] >= iXSeq && //�Ƕ���or����
ResCard[tmpCardBulk] < iXSeq )
{
ResCard[tmpCardBulk]++;

iResultCard[iResultCount++] = iCardList[j]; //����copy�������

if(iResultCount >= iBaseCount) 
{
return TRUE; //�ҹ���
}
}
}

return FALSE;
}
*/
//��ȡͬ��˳,ֻ����ͬ�Ƶ��ͬ��˳,�Ƶ���,����һ��˳�ӵĺ�������
/*BYTE CUpGradeGameLogic::TackOutStraightFlush(BYTE iCardList[], int iCardCount, 
BYTE iBaseCard[], int iBaseCount, 
BYTE iResultCard[], int &iResultCardCount, 
int bExtVal)
{
BYTE iBaseMinCard = GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 1);//�����˳������С����

using namespace std;
vector<BYTE> MyCardList;

for(int i = 0; i < iCardCount; i++) //����һ��
{
MyCardList.push_back(iCardList[i]);
}

vector<BYTE> MinCard; //�洢��С����
//�Ѳ�����˳�ӵ�����ȫ���޳�
for(vector<BYTE>::iterator it = MyCardList.begin(); it != MyCardList.end(); it++)
{
if( GetCardBulk(*it) < iBaseMinCard || 
GetCardBulk(*it) >= (iBaseMinCard+iBaseCount) )
{
MyCardList.erase(it);
}
else if(GetCardBulk(*it) == iBaseMinCard) //��С�Ǹ�
{
MinCard.push_back(*it);
}
}

if(MyCardList.size() < (unsigned int)iBaseCount) return FALSE; //ʣ�µ�����˳�ӵĸ������ղ���

for(vector<BYTE>::iterator min = MinCard.begin(); min != MinCard.end(); min++) //��С����,������ɫ��Ҫ�Ƚ�һ��
{
iResultCardCount = 0;
for(int i = 0; i < iBaseCount; i++) //��û������,һ��ֻ����һ������
{
for(vector<BYTE>::iterator it = MyCardList.begin(); it != MyCardList.end(); it++) //��ʣ�µ�������
{
if( GetCardHuaKind((*min), TRUE) == GetCardHuaKind((*it), TRUE) && //��ɫ��ͬ
GetCardBulk(*it) - GetCardBulk(*min) == iResultCardCount) //���������
{
//�ҵ���,
iResultCard[iResultCardCount++] = (*it);
MyCardList.erase(it); //������һ��ѭ����������,�ɵ�

if(iResultCardCount >= iBaseCount) return TRUE;

break;
}
}
}
}

return FALSE;
}
*/

//��ȡ510K
BOOL CUpGradeGameLogic::TrackOut510K(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount, bool bExtVal)
{
	iResultCardCount=0;
	BYTE temp[48]={0};
	BYTE huasei[4][16]={0};
	int k = 0, num[4] = {0};
	//�õ�510K����
	for(int i=0; i<iCardCount; i++)
	{
		int n = GetCardNum(iCardList[i]);
		if(n==5 || n==10 || n==13)
		{
			temp[k++] = iCardList[i];
			int kind = GetCardHuaKind(iCardList[i],true) >> 4;

			huasei[kind][num[kind]++] = iCardList[i];
		}
	}
	//5,10,k��Ŀ����3��
	if(num[0]+num[1]+num[2]+num[3] < 3)
		return false;
	//Ҫ����510K��������3��
	if(bExtVal && num[0]<3 && num[1]<3 && num[2]<3 && num[3]<3)
		return false;
	for(int i=0;i<4;i++)
	{
		if(Test510K(huasei[i],num[i]))//ĳһ��ɫ�Ƿ�Ϊ��510K
		{
			Copy510K(huasei[i],num[i],iResultCard,iResultCardCount);	

			if(bExtVal) //�Ƿ���Ҫ��ȡ��510K
				return true;
			else
			{
				RemoveCard(iResultCard,iResultCardCount,huasei[i],num[i]);//����510K�Ƴ�ѡ����ɫ����
				RemoveCard(iResultCard,iResultCardCount,temp,k);	//����510K�Ƴ�510K����
				num[i]-=iResultCardCount;
				k-=iResultCardCount;	
			}
			//			return true;
		}
	}

	if(bExtVal) return false; //��Ҫͬ��510K

	if(Test510K(temp,k))
	{
		Copy510K(temp,k,iResultCard,iResultCardCount);
		return true;
	}
	return FALSE;
}

//����510K
BOOL CUpGradeGameLogic::Copy510K(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount)
{
	iResultCardCount=0;
	BYTE five,ten,k;
	for(int i=0;i<iCardCount;i++)
	{
		if(GetCardNum(iCardList[i])==5)
			five=iCardList[i];
		else if(GetCardNum(iCardList[i])==10)
			ten=iCardList[i];
		else k=iCardList[i];
	}
	iResultCard[0]=five;
	iResultCard[1]=ten;
	iResultCard[2]=k;
	iResultCardCount=3;
	return  TRUE;
}

//�����Ƿ�Ϊ5,10k
BOOL CUpGradeGameLogic::Test510K(BYTE iCardList[],int iCardCount, bool bExtVal)
{
	BOOL five=false,ten=false,k=false;
	for(int i=0;i<iCardCount;i++)
	{
		if(GetCardNum(iCardList[i])==5)
			five=true;
		else if (GetCardNum(iCardList[i])==10)
			ten=true;
		else 
			k=true;
	}
	//��5,10,k
	if(five&&ten&&k)
		return true;
	return false;
}


//���
BOOL CUpGradeGameLogic::TackOutCardByNoSameShape(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount,BYTE iBaseCard[],int iBaseCardCount)
{
	iResultCardCount = 0;
	BYTE temp[18] = {0};
	int t = GetCardBulk(iBaseCard[0], false); //�õ��������Ǹ��Ƶ�ֵ
	for(int i = 0; i < iCardCount; i++)
	{
		temp[GetCardBulk(iCardList[i],false)]++;
	}

	//��(ը�Ʋ���)
	for(int i = 0; i < 18; i++)
	{
		if( temp[i] < 4 &&               //��ը����
			temp[i] > iBaseCardCount &&  //�����������ƶ�
			i > t )                      //�����ִ�
		{
			for(int j=0; j < iCardCount; j++)
			{
				if(GetCardBulk(iCardList[j],false) == i)
				{
					iResultCard[iResultCardCount++] = iCardList[j];
					if(iResultCardCount == iBaseCardCount)
						return true ;			
				}
			}
		}
	}
	return false;
}


//�Ƿ���Գ���
BOOL CUpGradeGameLogic::CanOutCard(BYTE iOutCard[], int iOutCount,  //Ҫ������
								   BYTE iBaseCard[], int iBaseCount,//Ҫѹ����
								   BYTE iHandCard[], int iHandCount,//���е���
								   bool bFirstOut)
{
	BYTE iOutCardShape = GetCardShape(iOutCard, iOutCount);

	if(iOutCardShape == UG_ERROR_KIND) //���Ͳ���
		return FALSE;

	if(bFirstOut)
		return TRUE;
	BYTE iBaseCardShape = GetCardShape(iBaseCard, iBaseCount); //�����ϵ�����

	if(iBaseCardShape > iOutCardShape)						//����<
	{
		return FALSE;
	}

	if( iBaseCardShape < iOutCardShape)						//����>
	{
		if(UG_SLAVE_510K <= iOutCardShape) //ը��
		{
			return TRUE;
		}	
		//������һ��������Ҳ���Դ����˳�Ӻ�˳�Ӵ�С�Ƚ�
		if(iBaseCount != iOutCount) //��������
			return FALSE;

		switch(iBaseCardShape)
		{
		case UG_STRAIGHT:									//ͬ��혴������
			{
				if(iOutCardShape == UG_STRAIGHT_FLUSH)
					return true;
			}
		case UG_VARIATION_STRAIGHT:							//��С����˳��
			{
				if(iOutCardShape == UG_STRAIGHT)			//����˳������Ч���ֵС������˳���������
					return GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,1) <  GetBulkBySpecifySequence(iOutCard, iOutCount,1) ;
				return false;
			}

		case UG_VARIATION_DOUBLE_SEQUENCE://��С����˳��
			{
				if(iOutCardShape == UG_DOUBLE_SEQUENCE)	//����˳������Ч���ֵС������˳���������
					return GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,2) <  GetBulkBySpecifySequence(iOutCard, iOutCount,2) ;
				return false;
			}

		case UG_VARIATION_THREE_SEQUENCE:		//������˳
		case UG_VARIATION_THREE_ONE_SEQUENCE://������˳
		case UG_VARIATION_THREE_TWO_SEQUENCE://����������˳
		case UG_VARIATION_THREE_DOUBLE_SEQUENCE://����������˳
		case UG_VARIATION_THREE_SEQUENCE_DOUBLE_SEQUENCE://������˳����˳
			{
				if(iOutCardShape == iBaseCardShape+1)
					return GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,3) <  GetBulkBySpecifySequence(iOutCard, iOutCount,3) ;
				return false;
			}
		case UG_VARIATION_FOUR_SEQUENCE:		//������˳
		case UG_VARIATION_FOUR_ONE_SEQUENCE:	//�����Ĵ�һ˳
		case UG_VARIATION_FOUR_TWO_SEQUENCE:	//�����Ĵ���˳
		case UG_VARIATION_FOUR_ONE_DOUBLE_SEQUENCE://�����Ĵ�һ��˳
		case UG_VARIATION_FOUR_TWO_DOUBLE_SEQUENCE://�����Ĵ�����˳
			{
				if(iOutCardShape == iBaseCardShape+1)
					return GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,4) <  GetBulkBySpecifySequence(iOutCard, iOutCount,4) ;
				return false;
			}
		case UG_THREE_TWO://����һ������������
			{
				if(iOutCardShape == UG_THREE_DOUBLE)
					return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount,3)<GetBulkBySpecifyCardCount(iOutCard, iOutCount,3);
				return false;
			}
		case UG_THREE_TWO_SEQUENCE://����һ���(�����)>���������
			{
				if(iOutCardShape == UG_THREE_DOUBLE_SEQUENCE || iOutCardShape == UG_THREE_SEQUENCE_DOUBLE_SEQUENCE)
					return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount,3)<GetBulkBySpecifyCardCount(iOutCard, iOutCount,3);
				return false;
			}
		case UG_FOUR_TWO://�Ď�һ�����Ď�����
			{
				if(iOutCardShape == UG_FOUR_ONE_DOUBLE)
					return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount,4)<GetBulkBySpecifyCardCount(iOutCard, iOutCount,4);
				return false;
			}
		case UG_FOUR_TWO_SEQUENCE://�Ď�һ��혣��Ď������
			{
				if(iOutCardShape == UG_FOUR_ONE_DOUBLE_SEQUENCE)
					return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount,4)<GetBulkBySpecifyCardCount(iOutCard, iOutCount,4);
				return false;
			}
		case UG_THREE_DOUBLE_SEQUENCE:	//����������혎���
			{
				if(iOutCardShape == UG_THREE_SEQUENCE_DOUBLE_SEQUENCE)
					return GetBulkBySpecifySequence(iBaseCard, iBaseCount,3) < GetBulkBySpecifySequence(iOutCard, iOutCount,3);
				return false;
			}
		}
		return false;
	}	

	switch(iBaseCardShape)			//��������һ��
	{
	case UG_ONLY_ONE:  //����
	case UG_DOUBLE:    //����
	case UG_THREE:     //����
		{
			return GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 1) < GetBulkBySepcifyMinOrMax(iOutCard, iOutCount, 1);
		}		
	case UG_BOMB: //4+�� ը��
		{
			if(iBaseCount > iOutCount) //�������ը����
				return FALSE;

			if(iBaseCount == iOutCount) //������ͬ,�ȵ���
				return GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 1) < GetBulkBySepcifyMinOrMax(iOutCard, iOutCount, 1);
			return TRUE;
		}
	case UG_SHAM_BOMB:
		{
			//���浱ǰ�����пɵ��ƶ���
			BYTE iBaseBuffer[54];
			BYTE iOutBuffer[54];
			BYTE TempBase[10];
			BYTE TempOut[10];
			::CopyMemory(iBaseBuffer,iBaseCard,sizeof(BYTE)*iBaseCount);
			::CopyMemory(iOutBuffer,iOutCard,sizeof(BYTE)*iOutCount);
	
			int iBaseReplace = 0;
			int iOutReplace  = 0;
			TackOutByCondition(iBaseBuffer,iBaseCount,TempBase,iBaseReplace,m_iReplaceCardArray,m_iReplaceCardCount);
			TackOutByCondition(iOutBuffer,iOutCount,TempOut,iOutReplace,m_iReplaceCardArray,m_iReplaceCardCount);

			RemoveCard(TempBase,iBaseReplace,iBaseBuffer,iBaseCount);
			RemoveCard(TempOut,iOutReplace,iOutBuffer,iOutCount);
			return (GetCardBulk(iOutBuffer[0]) > GetCardBulk(iBaseBuffer[0]));
		}

	case UG_FLUSH:			//ͬ��(��˳�ӣ��Ƚ�ͬ����������
		{
			return GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 255) < GetBulkBySepcifyMinOrMax(iOutCard, iOutCount, 255);
		}
	case UG_STRAIGHT_FLUSH: //ͬ��˳
	case UG_STRAIGHT:		//˳��
	case UG_DOUBLE_SEQUENCE: //����
	case UG_THREE_SEQUENCE:  //����  
	case UG_FOUR_SEQUENCE:	//��˳
		if(iOutCount != iBaseCount)
			return FALSE;
		{
			return GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 1) < GetBulkBySepcifyMinOrMax(iOutCard, iOutCount, 1);
		}

	case UG_THREE_ONE:		//����һ
	case UG_THREE_TWO:		//������
	case UG_THREE_DOUBLE:	//������
		//��һ�������Ƶ��Ƶ��С������
		//return (SearchThreeCard(iBaseCard, iBaseCount) < SearchThreeCard(iOutCard, iOutCount));
		{
			return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount,3)<GetBulkBySpecifyCardCount(iOutCard, iOutCount,3);
		}
	case UG_FOUR_ONE:						//�Ĵ�һ
	case UG_FOUR_TWO:						//�Ĵ���
	case UG_FOUR_ONE_DOUBLE:				//�Ĵ�һ��
	case UG_FOUR_TWO_DOUBLE:				//�Ĵ�����
		{
			return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount,4)<GetBulkBySpecifyCardCount(iOutCard, iOutCount,4);
		}

	case UG_THREE_ONE_SEQUENCE: //2+������һ
	case UG_THREE_TWO_SEQUENCE: //2+��������
	case UG_THREE_DOUBLE_SEQUENCE://������˳
	case UG_THREE_SEQUENCE_DOUBLE_SEQUENCE:		//��˳����˳(����)
		{
			if(iOutCount != iBaseCount)
				return FALSE;
			return(GetBulkBySpecifySequence(iBaseCard, iBaseCount,3) < GetBulkBySpecifyCardCount(iOutCard, iOutCount,3));
		}
	case UG_FOUR_ONE_SEQUENCE:					//��˳
	case UG_FOUR_TWO_SEQUENCE:
	case UG_FOUR_ONE_DOUBLE_SEQUENCE:
	case UG_FOUR_TWO_DOUBLE_SEQUENCE:
		{
			return(GetBulkBySpecifySequence(iBaseCard, iBaseCount,4) < GetBulkBySpecifyCardCount(iOutCard, iOutCount,4));
		}
	case UG_MASTER_510K: //ͬ��510K����ɫ:���� > ���� > ÷�� > ��Ƭ
		{
			return (GetCardHuaKind(iBaseCard[0],true) < GetCardHuaKind(iOutCard[0],true)); //�Ȼ�ɫ
		}
	case UG_SLAVE_510K: //��510K��һ����
		{
			return FALSE;
		}
		//�������δ���
	case UG_VARIATION_STRAIGHT://��˳
		{
			if(iOutCount != iBaseCount)
				return FALSE;
			return(GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,1) < GetBulkBySpecifyVariationSequence(iOutCard, iOutCount,1));

			break;
		}
	case UG_VARIATION_DOUBLE_SEQUENCE://��˳
		{
			if(iOutCount != iBaseCount)
				return FALSE;
			return(GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,2) < GetBulkBySpecifyVariationSequence(iOutCard, iOutCount,2));
			break;
		}
	case UG_VARIATION_THREE_SEQUENCE://��˳
	case UG_VARIATION_THREE_ONE_SEQUENCE://����һ˳
	case UG_VARIATION_THREE_TWO_SEQUENCE://������˳
	case UG_VARIATION_THREE_DOUBLE_SEQUENCE://������˳
	case UG_VARIATION_THREE_SEQUENCE_DOUBLE_SEQUENCE://��˳����˳
		{
			if(iOutCount != iBaseCount)
				return FALSE;
			return(GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,3) < GetBulkBySpecifyVariationSequence(iOutCard, iOutCount,3));
			break;
		}
	case UG_VARIATION_FOUR_SEQUENCE:		//������˳
	case UG_VARIATION_FOUR_ONE_SEQUENCE:	//�����Ĵ�һ˳
	case UG_VARIATION_FOUR_TWO_SEQUENCE:	//�����Ĵ���˳
	case UG_VARIATION_FOUR_ONE_DOUBLE_SEQUENCE://�����Ĵ�һ��˳
	case UG_VARIATION_FOUR_TWO_DOUBLE_SEQUENCE://�����Ĵ�����˳
		{
			if(iOutCount != iBaseCount)
				return FALSE;
			return(GetBulkBySpecifySequence(iBaseCard, iBaseCount,4) < GetBulkBySpecifyCardCount(iOutCard, iOutCount,4));

			return false;
		}
	}

	return FALSE;
}

//��ȡ����ը��Ϊ�ᷴ��˳,˫˳,��˳��׼��
BOOL CUpGradeGameLogic::TackOutAllBomb(BYTE iCardList[],int iCardCount,
									   BYTE iResultCard[],int &iResultCardCount,int iNumCount)
{
	iResultCardCount=0;
	BYTE bCardBuffer[54];
	BYTE bombcount=GetBombCount(iCardList,iCardCount,iNumCount);
	if(bombcount<0)
		return false;
	for(int i=iNumCount;i < 9;i++)
	{
		int count=TackOutBySepcifyCardNumCount(iCardList,iCardCount,bCardBuffer,i);
		if(count > 0)
		{
			::CopyMemory(&iResultCard[iResultCardCount],bCardBuffer,sizeof(BYTE)*count);
			iResultCardCount+=count;
			break;
		}
	}
	return true;
}

//��ȡը��
BOOL CUpGradeGameLogic::TackOutBomb(BYTE iCardList[], int iCardCount,
									BYTE iResultCard[], int &iResultCardCount,int iNumCount)
{
	iResultCardCount=0;
	BYTE bCardBuffer[54];
	BYTE bombcount=GetBombCount(iCardList,iCardCount,iNumCount);
	if(bombcount<0)
		return false;
	for(int i=iNumCount;i<9;i++)
	{
		int count=TackOutBySepcifyCardNumCount(iCardList,iCardCount,bCardBuffer,i);
		if(count > 0)
		{
			::CopyMemory(iResultCard,bCardBuffer,sizeof(BYTE)*i);
			iResultCardCount=i;
			break;
		}
	}
	if(iResultCardCount == 0)
		TackOutKingBomb(iCardList,iCardCount,iResultCard,iResultCardCount);
	return true;
}

//��ȡը��
BOOL CUpGradeGameLogic::TackOutShamBomb(BYTE iCardList[], int iCardCount,
									BYTE iResultCard[], int &iResultCardCount,int iNumCount)
{

	int kingcount = 0;
	int ReplaceCount = 1;
	iResultCardCount = 0;
	int iTempNumcount = iNumCount;
	BYTE iBuffer[54];
	BYTE iTempCard[54];
	if(GetReplaceCardCount(iCardList,iCardCount))
	{
		TackOutByCondition(iCardList,iCardCount,iBuffer,kingcount,m_iReplaceCardArray,m_iReplaceCardCount);
	}else
		return FALSE;

	BYTE iTmp[54]={0};//ȥ�������ƺ��������
	int iTmpCount = iCardCount;
	::CopyMemory(iTmp,iCardList,sizeof(BYTE)*iCardCount);
	int iRev = RemoveCard(iBuffer,kingcount,iTmp,iCardCount);
	iTmpCount = iCardCount - iRev;


	iNumCount = iTempNumcount - ReplaceCount;
	BYTE iCount = TackOutBySepcifyCardNumCount(iTmp, iTmpCount, iTempCard, iNumCount);
	for (int i=1; i<kingcount; i++)
	{
		if (iCount > 0)
			break;
		
		//����1,2,3,or4����
		if (iCount == 0 && ReplaceCount < kingcount)
		{
	    	if (iTempNumcount - ReplaceCount < 1)
				break;
			ReplaceCount++;
			iNumCount = iTempNumcount - ReplaceCount;
			iCount = TackOutBySepcifyCardNumCount(iTmp, iTmpCount, iTempCard, iNumCount);
		}
		
	}

	if(iCount > 0)
	{
		CopyMemory(iResultCard, iTempCard, sizeof(BYTE)*iNumCount);	
		iResultCardCount = iNumCount;
		if (kingcount > 0)
		{

			for (int i=0; i<ReplaceCount; i++)
			{
				iResultCard[iNumCount+i] = iBuffer[i];
			}
			iResultCardCount = iResultCardCount+ReplaceCount;
			if (iResultCardCount != iTempNumcount)
			{
				iResultCardCount = 0;
				return FALSE;
			}
			return TRUE;
		}
		
	}
	iResultCardCount = 0;
	return FALSE;
}
//��ȡ��ը
BOOL CUpGradeGameLogic::TackOutKingBomb(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount)
{
	iResultCardCount=0;

	BYTE bCardBuf[8];
	int kingcount=0;
	int SingKing= KING_COUNT/2;
	int count=TackOutBySpecifyCard(iCardList,iCardCount,bCardBuf,kingcount,0x4e);
	if(count != SingKing)
		return false;

	::CopyMemory(iResultCard,bCardBuf,sizeof(BYTE)*count);

	count=TackOutBySpecifyCard(iCardList,iCardCount,bCardBuf,kingcount,0x4f);
	if(count != SingKing)
	{
		return false;
	}	
	::CopyMemory(&(iResultCard[SingKing]),bCardBuf,sizeof(BYTE)*count);
	return iResultCardCount = KING_COUNT;
}
//�O�ô��������
BOOL CUpGradeGameLogic::SetReplaceCard(BYTE iCardList[],int iCardCount)
{
	::CopyMemory(m_iReplaceCardArray,iCardList,iCardCount);
	m_iReplaceCardCount = iCardCount;
	return TRUE;
}

//��ȡ�����пɴ���������
int CUpGradeGameLogic::GetReplaceCardCount(BYTE iCardList[],int iCardCount)
{

	int iResultCount = 0;
	for(int i = 0;i < m_iReplaceCardCount;i ++)
	{	
		iResultCount += GetCountBySpecifyCard(iCardList,iCardCount,m_iReplaceCardArray[i]);
	}
	return iResultCount;
}
//�趨����
int CUpGradeGameLogic::SetLai(BYTE iCard)
{
	
	int LaiCard = 0;
	if (iCard == 0x4e || iCard ==0x4f)
	{
		LaiCard = 3;

	}
	else if (GetCardNum(iCard) == 14)
	{
		LaiCard = 2;
	}else
	{
		LaiCard = GetCardNum(iCard) + 1;
	}
	BYTE LaiCardList[4] = {0};
	if (iCard == 255)
	{
		SetReplaceCard(LaiCardList, 4);
		return 0;
	}
	LaiCardList[0] = (LaiCard - 1) | UG_FANG_KUAI;
	LaiCardList[1] = (LaiCard - 1) | UG_MEI_HUA;
	LaiCardList[2] = (LaiCard - 1) | UG_HONG_TAO;
	LaiCardList[3] = (LaiCard - 1) | UG_HEI_TAO;

	SetReplaceCard(LaiCardList, 4);
	return LaiCard;
}
//��������ĳ��������
BOOL CUpGradeGameLogic::TackOutByCondition(BYTE iCardList[],int iCardCount, 
										   BYTE iResultCard[], int & iResultCardCount,
										   BYTE iConditionCard[],int iConditionCardCount)
{
	iResultCardCount = 0;
	for(int i = 0 ;i < iConditionCardCount ; i++)
	{
		for(int j = 0 ;j<iCardCount ;j ++)
		{
			if(iConditionCard[i] == iCardList[j])
				iResultCard[iResultCardCount++] = iCardList[j]; 
		}
	}
	return iResultCardCount;
}
int CUpGradeGameLogic::GetCardReplaceShape(BYTE iCardList[],int iCardCount, ReplaceStruct & buf)
{
	//���浱ǰ�����пɵ��ƶ���
	BYTE iBuffer[54];
	//����һ��
	BYTE iTmp[54];

	int iTmpCount = iCardCount;
	::CopyMemory(iTmp,iCardList,sizeof(BYTE)*iCardCount);
    ::CopyMemory(buf.SrcCardList,iCardList,sizeof(BYTE)*iCardCount);
	int ReplaceCount = 0;// = GetReplaceCardCount(iCardList,iCardCount);
	TackOutByCondition(iCardList,iCardCount,iBuffer,ReplaceCount,m_iReplaceCardArray,m_iReplaceCardCount);
	int iRev = RemoveCard(iBuffer,ReplaceCount,iTmp,iCardCount);
	iTmpCount -= iRev;
    buf.CardCount = iCardCount;
	buf.ReplaceCount = ReplaceCount;
	buf.ListCount = 0;


	///20100108 zht
	if (ReplaceCount == iCardCount)
	{
        ::CopyMemory(buf.ReplaceList[0],iTmp,sizeof(BYTE)*iCardCount);
		buf.ReplaceListShape[0] = GetCardShape(iTmp,iCardCount);
		buf.ListCount = 1;
		return 1;
	}

	if(ReplaceCount <= 0 /*|| ReplaceCount == iCardCount*/)  //commented by Longan  12.12 2008
		return 0;
	switch(ReplaceCount) 
	{
	case 1:
		{
			for (BYTE j=0x01; j<=0x0D; j++)
				{
					
					iTmp[iTmpCount] = j;
					if (GetCardShape(iTmp,iCardCount) !=UG_ERROR_KIND)
					{
						
                   		::CopyMemory(buf.ReplaceList[buf.ListCount],iTmp,sizeof(BYTE)*iCardCount);
						if (IsThreeX(iTmp,iCardCount,1))
						{
							BYTE NowTemp[10];
							TackOutBySepcifyCardNumCount(buf.ReplaceList[buf.ListCount], iCardCount, NowTemp,1);
							if (NowTemp[0] == j)
							{
								if (GetReplaceCardCount(&j,1)==0)
									continue;
							}
							buf.ReplaceList[buf.ListCount][iTmpCount] += 0x40;
						}
						buf.ReplaceListShape[buf.ListCount] = GetCardShape(iTmp,iCardCount);
						buf.ListCount++;
					}
				}
		}
		break;
	case 2:
		{
			for (BYTE i=0x1; i<=0x0D; i++)
			{
				for (BYTE j=i; j<=0x0D; j++)
				{
					iTmp[iTmpCount] = i;
					iTmp[iTmpCount+1] = j;
					
					if (GetCardShape(iTmp,iCardCount) !=UG_ERROR_KIND)
					{
						
						::CopyMemory(buf.ReplaceList[buf.ListCount],iTmp,sizeof(BYTE)*iCardCount);
						if (IsThreeX(iTmp,iCardCount,1))
						{
							BYTE NowTemp[10];
							TackOutBySepcifyCardNumCount(buf.ReplaceList[buf.ListCount], iCardCount, NowTemp,1);
							if (NowTemp[0] == j || NowTemp[0] == i)
							{
						
								if (GetReplaceCardCount(&iTmp[iTmpCount],2)==0)
									continue;
								if (NowTemp[0] == j)
									buf.ReplaceList[buf.ListCount][iTmpCount+1] +=0x40;
								if (NowTemp[0] == i)
									buf.ReplaceList[buf.ListCount][iTmpCount] +=0x40;
						
							}
						}
						else if (GetCardShape(iTmp,iCardCount) == UG_SHAM_BOMB)
								continue;
						buf.ReplaceListShape[buf.ListCount] = GetCardShape(iTmp,iCardCount);
						buf.ListCount++;
					}
				}
			}
		}

		break;
	case 3:
		{
			for (BYTE i=0x1; i<=0x0D; i++)
			{
				
				for (BYTE j=i; j<=0x0D; j++)
				{
					for (BYTE k = j; k<=0x0D; k++)
					{
						iTmp[iTmpCount] = i;
						iTmp[iTmpCount+1] = j;
						iTmp[iTmpCount+2] = k;
						if (GetCardShape(iTmp,iCardCount) !=UG_ERROR_KIND)
						{
							
							::CopyMemory(buf.ReplaceList[buf.ListCount],iTmp,sizeof(BYTE)*iCardCount);
							
							if (IsThreeX(iTmp,iCardCount,1))
							{
								BYTE NowTemp[10];
								TackOutBySepcifyCardNumCount(buf.ReplaceList[buf.ListCount], iCardCount, NowTemp,1);
								if (NowTemp[0] == j || NowTemp[0] == i || NowTemp[0] == k)
								{
									if (GetReplaceCardCount(&iTmp[iTmpCount],3)==0)
										continue;
									if (NowTemp[0] == j)
										buf.ReplaceList[buf.ListCount][iTmpCount+1] +=0x40;
									if (NowTemp[0] == i)
										buf.ReplaceList[buf.ListCount][iTmpCount] +=0x40;
									if (NowTemp[0] == k)
										buf.ReplaceList[buf.ListCount][iTmpCount+2] += 0x40;
								}
								else
								{
									if (GetReplaceCardCount(&j,1)!=0)
									{
										buf.ReplaceList[buf.ListCount][iTmpCount] += 0x40;
									 }
								}
							}
							else if (GetCardShape(iTmp,iCardCount) == UG_SHAM_BOMB)
								continue;
							buf.ReplaceListShape[buf.ListCount] = GetCardShape(iTmp,iCardCount);
							buf.ListCount++;
							
						}
					}
				}
			}
		}
		break;
	case 4:
		{
			for (BYTE i=0x1; i<=0x0D; i++)
			{
				for (BYTE j=i; j<0x0D; j++)
				{
					for (BYTE k = j; k<0x0D; k++)
					{
						for (BYTE l = k; l<0x0D; l++)
						{
							iTmp[iTmpCount] = i;
							iTmp[iTmpCount+1] = j;
							iTmp[iTmpCount+2] = k;
							iTmp[iTmpCount+3] = l;
							if (GetCardShape(iTmp,iCardCount) !=UG_ERROR_KIND)
							{
							
								::CopyMemory(buf.ReplaceList[buf.ListCount],iTmp,sizeof(BYTE)*iCardCount);

								if (IsThreeX(iTmp,iCardCount,1))
								{
									BYTE NowTemp[10];
									TackOutBySepcifyCardNumCount(buf.ReplaceList[buf.ListCount], iCardCount, NowTemp,1);
									if (NowTemp[0] == j || NowTemp[0] == i || NowTemp[0] == k || NowTemp[0] == l)
									{
										if (GetReplaceCardCount(&iTmp[iTmpCount],4)==0)
											continue;
										if (NowTemp[0] == j)
											buf.ReplaceList[buf.ListCount][iTmpCount+1] += 0x40;
										if (NowTemp[0] == i)
											buf.ReplaceList[buf.ListCount][iTmpCount] += 0x40;
										if (NowTemp[0] == k)
											buf.ReplaceList[buf.ListCount][iTmpCount+2] += 0x40;
										if (NowTemp[0] == l)
											buf.ReplaceList[buf.ListCount][iTmpCount+3] += 0x40;
									}
								}
								else if (GetCardShape(iTmp,iCardCount) == UG_SHAM_BOMB)
									continue;
								buf.ReplaceListShape[buf.ListCount] = GetCardShape(iTmp,iCardCount);
								buf.ListCount++;
							}
						}
					}
				}
			}
		}
		break;
	}
	if (buf.ListCount == 0)
		memset(&buf,0,sizeof(buf));
	return buf.ListCount;
}
//�Ƿ���Գ���
BOOL CUpGradeGameLogic::ReplaceCanOutCard(BYTE iOutCard[], int iOutCount,  //Ҫ������
								   BYTE iBaseCard[], int iBaseCount,//Ҫѹ����
								   BYTE iHandCard[], int iHandCount,//���е���
								   ReplaceStruct & ReplaceCardList, 
								   bool bFirstOut)
{

//  m_ioutcard=iOutCount;
	ReplaceStruct replace;
	memset(&replace,0,sizeof(replace));
	if (GetCardReplaceShape(iOutCard,iOutCount,replace) <= 0)
		return FALSE;
	int BaseShape = GetCardShape(iBaseCard, iBaseCount);
	int srcShape = 0;//ԭ�Ƶ�����
	if (0 != replace.SrcCardList[0])
	{
		int srcShape = GetCardShape(replace.SrcCardList, iOutCount);
	}
	if (BaseShape == UG_BOMB && srcShape != UG_BOMB)
		return FALSE; //�󲻹�Ӳը��

	memset(&ReplaceCardList,0,sizeof(ReplaceCardList));
	BYTE CompareCard[45];//������Ҫѹ���ƱȽϵ���,��Ҫ����������ը����ը
	for (int i=0; i<replace.ListCount; i++)
	{
		if (replace.ReplaceListShape[i] == UG_BOMB)
		{
			::CopyMemory(CompareCard, iOutCard, sizeof(BYTE)*iOutCount);
		}
		else
		{
			::CopyMemory(CompareCard, replace.ReplaceList[i], sizeof(BYTE)*iOutCount);
		}
		if (CanOutCard(CompareCard, iOutCount, iBaseCard, iBaseCount, iHandCard, iHandCount, bFirstOut))
		{
	    	::CopyMemory(ReplaceCardList.ReplaceList[ReplaceCardList.ListCount], replace.ReplaceList[i], sizeof(BYTE)*iOutCount);
			
			ReplaceCardList.ReplaceListShape[ReplaceCardList.ListCount] = replace.ReplaceListShape[i];
			ReplaceCardList.CardCount = replace.CardCount;
			
			::CopyMemory(ReplaceCardList.SrcCardList, replace.SrcCardList, sizeof(BYTE)*iOutCount);
			ReplaceCardList.ReplaceCount = replace.ReplaceCount;
			
			::CopyMemory(ReplaceCardList.ReplaceCard, replace.ReplaceCard, sizeof(BYTE)*replace.ReplaceCount);
			ReplaceCardList.ListCount++;

		}

	}
	if (ReplaceCardList.ListCount > 0)
		return TRUE;
	memset(&ReplaceCardList,0,sizeof(ReplaceCardList));
	return FALSE;
}
///��ȡ�����Ƶ�����
int CUpGradeGameLogic::GetMaxCard(BYTE iBaseCard[] , int iBaseCardCount)
{
	for(int i = 18;i>0 ;i--)						//�ƶ���
	{
		for(int  j = 0 ; j< iBaseCardCount ; j ++)
		{
			if(i == GetCardBulk(iBaseCard[j]))
			{
				return i; 
			}
		}
	}
	return -1 ; 
}