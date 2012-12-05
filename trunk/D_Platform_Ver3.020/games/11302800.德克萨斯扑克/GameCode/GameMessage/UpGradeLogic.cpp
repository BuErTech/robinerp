#include "StdAfx.h"
#include "UpGradeLogic.h"

// ���캯��
CUpGradeGameLogic::CUpGradeGameLogic(void)
{
	m_bCompareHuaSe = true;
	memset((void*)SevenCard, 0, sizeof(SevenCard));

	m_iStation[4] = 500;
	for (int i = 0; i < 6; i++)
	{
		m_iStation[i] = 100 * i;
	}
}

// ��ȡ�˿˻�ɫ
BYTE CUpGradeGameLogic::GetCardHuaKind(BYTE iCard)
{ 
	int iHuaKind = (iCard & UG_HUA_MASK);	
	return iHuaKind; 
}

// ��ȡ�˿˴�С ��2 - 18 �� 15 ���������� �� 2 - 21 �� 15 ����������
int CUpGradeGameLogic::GetCardBulk(BYTE iCard, BOOL bExtVol)
{
	if ((iCard == 0x4E) || (iCard == 0x4F))
	{
		return bExtVol ? (iCard - 14) : (iCard - 62);	// ��С��64+14-62=16ֻ���ش�Сè��ֵ
	}

	int iCardNum = GetCardNum(iCard);
	int iHuaKind = GetCardHuaKind(iCard);

	return ((bExtVol) ? ((iHuaKind >> 4) + (iCardNum * 4)) : (iCardNum));
}

// �����˿�
BOOL CUpGradeGameLogic::SortCard(BYTE iCardList[], BYTE bUp[], int iCardCount)
{
	BOOL bSorted = TRUE,bTempUp;
	int iTemp, iLast = iCardCount - 1, iStationVol[45];

	// ��ȡλ����ֵ
	for (int i = 0;i < iCardCount; i++)
	{
		iStationVol[i] = GetCardBulk(iCardList[i], TRUE);
	}

	// �������
	do
	{
		bSorted = TRUE;
		for (int i = 0; i < iLast; i++)
		{
			if (iStationVol[i] < iStationVol[i + 1])
			{	
				// ����λ��
				iTemp = iCardList[i];
				iCardList[i] = iCardList[i + 1];
				iCardList[i + 1] = iTemp;
				iTemp = iStationVol[i];
				iStationVol[i] = iStationVol[i + 1];
				iStationVol[i + 1] = iTemp;

				if (bUp != NULL)
				{
					bTempUp = bUp[i];
					bUp[i] = bUp[i + 1];
					bUp[i + 1] = bTempUp;
				}
				bSorted = FALSE;
			}	
		}
		iLast--;
	} while(!bSorted);
		
	return TRUE;
}


// ��������,������ͬ���ֵ�����ǰ�š��������Ƚϵ�ʱ����   
BOOL CUpGradeGameLogic::ReSortCard(BYTE iCardList[], int iCardCount)
{
	SortCard(iCardList,NULL,iCardCount);
	// �������Ŵ�С
	int iStationVol[45];
	for (int i = 0; i < iCardCount; i++)
	{
		iStationVol[i] = GetCardBulk(iCardList[i], false);
	}

	int Start = 0;
	int j, step;
	BYTE CardTemp[8];			// ��������Ҫ��λ������
	int CardTempVal[8];			// ����������λ������ֵ
	for(int i = 8;i > 1; i--)	// ����������һ������i����ͬ��ֵ
	{
		for(j = Start; j < iCardCount; j++)
		{
			CardTemp[0] = iCardList[j];								// ���浱ǰi��������ͬ��ֵ
			CardTempVal[0] = iStationVol[j];
				for(step = 1; step< i && j + step < iCardCount;)	// ��һ������i��ֵ��ȵ�����(����������ʱ������)
				{
					if(iStationVol[j] == iStationVol[j + step])
					{
						CardTemp[step] = iCardList[j + step];		// ������������
						CardTempVal[step] = iStationVol[j + step];	// ��ֵ
						step++;
					}
					else
						break;
				}

			if(step >= i)						// �ҵ�һ������i����ȵ����鴮��ʼλ��Ϊj,����λ��Ϊj+setp-1
			{									// ����Start��ʼ��j���������setp��
				if(j != Start)					// �ų���ʼ��������
				{
					for(;j >= Start; j--)		// ��Start����j�ź��ƶ�i��
						{
							iCardList[j + i - 1] = iCardList[j - 1];
							iStationVol[j + i - 1] = iStationVol[j - 1];
						}
					for(int k = 0; k < i; k++)				
					{
						iCardList[Start + k] = CardTemp[k];	// ��Start��ʼ���ó�CardSave
						iStationVol[Start + k] = CardTempVal[k];
					}
				}
				Start = Start + i;
			}
			j =j + step - 1;
		}
	}
	return true;
}

// ��ȡ�˿�
BYTE CUpGradeGameLogic::GetCardFromHua(int iHuaKind, int iNum)
{
	if (iHuaKind != UG_ERROR_HUA) return (iHuaKind + iNum - 1);
	return 0;
}

// �Ƿ����
BOOL CUpGradeGameLogic::IsDouble(BYTE iCardList[], int iCardCount)
{
	if (iCardCount< 2)
		return FALSE;

	int temp[17] = {0}, itwo = 0;
	for(int i = 0; i < iCardCount; i++)
	{
		temp[GetCardNum(iCardList[i])]++;
	}
 
	for(int i = 0; i < 17; i++)
	{
		if(temp[i] == 2)
			itwo++;
	}
	return (itwo == 1);
}

// �Ƿ�����
BOOL CUpGradeGameLogic::IsThree(BYTE iCardList[], int iCardCount)
{

	if (iCardCount < 3)
		return FALSE;
	int temp[17] = {0},ithree = 0;

	for(int i = 0; i < iCardCount; i++)
		temp[GetCardNum(iCardList[i])]++;

	for(int i = 0; i < 17; i++)
	{
		if(temp[i] == 3)
			ithree++;
	}
	return (ithree == 1);
}

// �Ƿ����ԣ��������ӣ�
BOOL CUpGradeGameLogic::IsCompleDouble(BYTE iCardList[], int iCardCount)
{
	if (iCardCount <5)
		return FALSE;

	int temp[17] = {0}, itwo = 0;

	for(int i = 0;i < iCardCount; i++)
		temp[GetCardNum(iCardList[i])]++;

	for(int i = 0; i < 17; i++)
	{
		if(temp[i] == 2)
			itwo++;
	}
	return (itwo > 1);
}

// �Ƿ�ͬ��(ͬ��Ϊ������Ϊһ�ֻ�ʽ)
BOOL CUpGradeGameLogic::IsSameHua(BYTE iCardList[], int iCardCount)
{
	// �ĸ�Ҳ�����㻨
	if(iCardCount < 4)
		return FALSE;

	int hs = -1;
		SortCard(iCardList, NULL, iCardCount);
	int FourCardColor[4] = {0};
	// SevenCard[4][7]={0}; // ���Ʒֻ�ɫ��������
	::memset(SevenCard, 0, sizeof(SevenCard));

	// ----------------ͬ������------xuqiangming------Begin-------------
	for (int i = 0;i < iCardCount; i++)
	{
		int hua = GetCardHuaKind(iCardList[i]);
		FourCardColor[hua / 16]++;
		SevenCard[hua / 16][i] = iCardList[i];
	}
	
	// -------------------------�޸��ж��Ƿ�Ϊͬ��-----------------------
	for (int j = 0; j < 4; j++)
		if (FourCardColor[j] >= 5)
			return TRUE;
	return FALSE;
	// ----------------ͬ������------xuqiangming------End------------
}

// �Ƿ�Ϊ��«������һ�����ƴ�2��һ�����ƣ�
BOOL CUpGradeGameLogic::IsHuLu(BYTE iCardList[], int iCardCount)
{	
	ReSortCard(iCardList ,iCardCount);
	
	// ��«������д -----------xuqiangming--------begein
	if (iCardCount < 4)
		return FALSE;

	int temp[18] = {0};
	for(int i = 0; i < iCardCount; i++)
	{
		temp[GetCardNum(iCardList[i])]++;
	}	
	bool bThree = false;

	int iTemp = -1;

	for(int i = 0; i < 18; i++)
	{
		if( temp[i] == 3 )	// �ȼ���Ƿ����3��	
		{
			bThree = true;	
			iTemp = i;
		}
	}
	if (!bThree)			// ������3�������ֱ�ӷ���false
		return false;

	for(int i = 0; i < 18; i++)
	{
		if (i == iTemp)
		{
			continue;
		}
		if( temp[i] >= 2)	// ������������Ƿ�Ϊ����
			return true;		
	}

	return false;
}

// �Ƿ�Ϊ4����4��ͬ����С���ƣ�
BOOL CUpGradeGameLogic::IsTieZhi(BYTE iCardList[], int iCardCount)
{
	/*for(int i=0;i<7;i++)
	{
		if(iCardList[i]==0)
			return FALSE;
	}*/
	if (iCardCount < 4) 
		return FALSE;
	//int pd = -1;
	int temp[18] = {0};
	for(int i = 0; i < iCardCount; i++)
	{
		temp[GetCardNum(iCardList[i])]++;
	}

	for(int i = 0;i < 18; i++)
	{
		if(temp[i] == 4)
			return  true;
	}
	return false;
}

// �Ƿ�ͬ��˳��5�ţ�
BOOL CUpGradeGameLogic::IsSameHuaContinue(BYTE iCardList[], int iCardCount)
{
	// -----------ͬ��˳����--------xuqiangming-------Begin-------------
	if(iCardCount < 4)
		return FALSE;
	/*if (!IsSameHua(iCardList,iCardCount)) 
		return FALSE;*/

	int FourCardColor[4] = {0};
	
	BYTE iCard[7] = {0};						 // ��5�ż�����ͬ��ɫ�Ʒ���������
	BYTE iSevenCard[4][18] = {0};				 // ���Ʒֻ�ɫ��������
	::memset(iCard,0,sizeof(iCard));
	::memset(iSevenCard,0,sizeof(iSevenCard));	
	
	for (int i = 0; i < iCardCount; i++)
	{
		int hua = GetCardHuaKind(iCardList[i]);
		FourCardColor[hua / 16]++;
		iSevenCard[hua / 16][i] = iCardList[i];	 // ���Ʒֻ�ɫ��������
	}

	int iHua = -1;
	for (int j = 0; j < 4; j++)			// �ҳ� ͬ�� >=5  ��һά����
	{
		if (FourCardColor[j] >= 5)
			iHua = j; 		
	}
	
	if (iHua == -1)		// û��һ�Ż�ɫ�������ﵽ5�Ż�����
		return false;

	int n = 0;
	for (int s = 0; s < 18; s++)			// ���ҳ��� ͬ���� ����һά������
	{
		if (iSevenCard[iHua][s] > 0)	// ��ֵ������
			iCard[n++] = iSevenCard[iHua][s];		
	}

	// ----------------------���� C++ &&���� ������һ������Ϊfalseʱֱ�ӷ���false------------------------
	// 5���Ƶ�ͬ��
	if (iCard[0] - 1 == iCard[1] && iCard[1] - 1 == iCard[2] && iCard[2] - 1 == iCard[3] && iCard[3] - 1 == iCard[4])
			return true;
	// 6���Ƶ�ͬ��
	if (FourCardColor[iHua] > 5)
	{
		// ��2�ŵ���6����˳��
		if (iCard[1] - 1 == iCard[2] && iCard[2] - 1 == iCard[3] && iCard[3] - 1 == iCard[4] && iCard[4] - 1 == iCard[5])
			return true;
		// 7���Ƶ�ͬ��
		if (FourCardColor[iHua] > 6)
		{ 
			// ��3�ŵ���7����˳��
			if (iCard[2] - 1 == iCard[3] && iCard[3] - 1 == iCard[4] && iCard[4] - 1 == iCard[5] && iCard[5] - 1 == iCard[6])
			return true;
		}
		return false;
	}	
	return false;	

return FALSE;
}
// ��С˳��(a,2,3,4,5)
BOOL CUpGradeGameLogic::IsSmallSingleContinue(BYTE iCardList[],int iCardCount)
{
	//for(int i=0;i<7;i++)
	//{
	//	if(iCardList[i]==0)
	//		return FALSE;
	//}
	if (IsSingleContinue(iCardList,iCardCount))
		return FALSE;
	if(iCardCount < 4)
		return FALSE;

	BYTE Temp[18] = {0};
	for(int i = 0; i < iCardCount; i ++)
	{
		Temp[GetCardNum(iCardList[i])] ++;
	}
	
	if(Temp[14] && Temp[1] && Temp[2] && Temp[3] && Temp[4])
		return TRUE;
	
	return FALSE;
}

// �ʼ�ͬ��˳(A,K,Q,J,10)
BOOL CUpGradeGameLogic::IsRegiusContinue(BYTE iCardList[],int iCardCount)
{

	if(iCardCount < 4)
		return FALSE;
	if (!IsSameHua(iCardList, iCardCount)) 
		return FALSE;
	if (!IsSingleContinue(iCardList, iCardCount))
		return FALSE;
	if (!IsSameHuaContinue(iCardList, iCardCount))
		return FALSE;
	BYTE Temp[61] = {0};
	for(int i = 0; i < iCardCount; i ++)
	{
		Temp[iCardList[i]] ++;
	}
	
	if(Temp[9] && Temp[10] && Temp[11]&& Temp[12] && Temp[13])
		return TRUE;
	if(Temp[25] && Temp[26] && Temp[27]&& Temp[28] && Temp[29])
		return TRUE;
	if(Temp[41] && Temp[42] && Temp[43]&& Temp[44] && Temp[45])
		return TRUE;
	if(Temp[57] && Temp[58] && Temp[59]&& Temp[60] && Temp[61])
		return TRUE;
	
	return FALSE;
}


// �Ƿ�˳,���������˳��
BOOL CUpGradeGameLogic::IsSingleContinue(BYTE iCardList[], int iCardCount)
{
	SortCard(iCardList, NULL, iCardCount);

	// --------��С��ͬ��˳-------
	BYTE Temp[18] = {0};
	for(int i = 0; i < iCardCount; i ++)
	{
		Temp[GetCardNum(iCardList[i])]++;
	}
	if(Temp[14] && Temp[5] && Temp[2] && Temp[3] && Temp[4])
	{
		return TRUE;
	}
	int kk;
	if (iCardCount == 5)
		kk = 1;
	else
		kk = 3;
	for(int k = 0; k < kk; k++)
	{
		int n = 0;
		int l = 1;
		for(int j = 1; j < iCardCount; j++)
		{			
			 if((GetCardNum(iCardList[k]) - GetCardNum(iCardList[j])) == l)
			 {
				l++;
				n++;
			 }
		}

		if(n >= 4)
			return TRUE;
		else
			continue;
	}

	// --------��С��ͬ��˳-------   
		return FALSE;
	// ��д---end--yinyi
}


// �õ����л�ɫ
BYTE CUpGradeGameLogic::GetCardListHua(BYTE iCardList[], int iCardCount)
{
	int iHuaKind = GetCardHuaKind(iCardList[0]);
	if (GetCardHuaKind(iCardList[iCardCount - 1]) != iHuaKind)
	return UG_ERROR_HUA;
	return iHuaKind;
}

// ��ȡ����
BYTE CUpGradeGameLogic::GetCardShape(BYTE iCardList[], int iCardCount)
{
	/***************************************************
	ͬ��˳>4��>��«>ͬ��>˳��>����>����>����>ɢ��
	***************************************************/

//	if (iCardCount <= 0) 	return UG_ERROR_KIND;//�Ƿ���
	if (IsRegiusContinue(iCardList, iCardCount))		return SH_REGIUS_SAME_HUA_CONTINUE;			// �ʼ�ͬ��˳	
	if (IsSameHuaContinue(iCardList, iCardCount))		return SH_SAME_HUA_CONTINUE;				// ͬ��˳
	if (IsSmallRegiusContinue(iCardList, iCardCount))	return SH_SMALL_SAME_HUA_CONTINUE;			// ��Сͬ��˳
	if (IsTieZhi(iCardList, iCardCount))				return SH_TIE_ZHI;							// ����
	if (IsHuLu(iCardList, iCardCount))					return SH_HU_LU;							// ��«
	if (IsSameHua(iCardList, iCardCount))				return SH_SAME_HUA;							// ͬ��
	if (IsSingleContinue(iCardList, iCardCount))		return SH_CONTINUE;							// ˳��
	if (IsSmallSingleContinue(iCardList, iCardCount))	return SH_SMALL_CONTINUE;					// ��С˳��
	if (IsThree(iCardList, iCardCount))					return SH_THREE;							// ����
	if (IsCompleDouble(iCardList, iCardCount))			return SH_TWO_DOUBLE;						// ����
	if (IsDouble(iCardList, iCardCount))				return SH_DOUBLE;							// ����
														return SH_OTHER;							// ɢ��
}			

// �Ƚ��������ƵĴ�С
int CUpGradeGameLogic::CompareCard(BYTE iFirstCard[], int iFirstCount, BYTE iSecondCard[], int iSecondCount)
{
	/***************************************************
	ͬ��˳>4��>��«>ͬ��>˳��>����>����>����>ɢ��
	***************************************************/
	BYTE iFirCard[7], iSecCard[7];
	memset(iFirCard, 0, sizeof(iFirCard));
	memset(iSecCard, 0, sizeof(iSecCard));

	::CopyMemory(iFirCard, iFirstCard, sizeof(BYTE) * iFirstCount);
	::CopyMemory(iSecCard, iSecondCard, sizeof(BYTE) * iSecondCount);


	BYTE iFirstCardKind = GetCardShape(iFirCard, iFirstCount),
		iSecondCardKind = GetCardShape(iSecCard, iSecondCount);

	ReSortCard(iFirCard, iFirstCount);
	ReSortCard(iSecCard, iSecondCount);

	// ���Ͳ�ͬ
	if (iFirstCardKind != iSecondCardKind) 
	{
		return (iFirstCardKind - iSecondCardKind > 0) ? 1 : -1;
	}

	// ������ͬ(�ȱȽ�����ƣ���Ƚϻ�ɫ)
	switch (iFirstCardKind)
	{
	case SH_DOUBLE: // ����
		{
			// �ȶ��Ӵ�С
			int pd1 = GetCardNum(iFirCard[0]);
			int	pd2 = GetCardNum(iSecCard[0]);

			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}

			// �ȵ�3����
			pd1 = GetCardNum(iFirCard[2]);
			pd2 = GetCardNum(iSecCard[2]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}

			// �ȵ�4����
			pd1 = GetCardNum(iFirCard[3]);
			pd2 = GetCardNum(iSecCard[3]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}

			// �ȵ�5����
			pd1 = GetCardNum(iFirCard[4]);
			pd2 = GetCardNum(iSecCard[4]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}

			return 0;

			//int hs1,hs2;   
			//switch(iFirstCount) // ��ǰ������ȣ��Ƚ�������
			//{
			////case 2: // ��Ϊ���ԱȽϴ�С  dxhע��
			////	{
			////		hs1 = GetCardHuaKind(iFirCard[0]);
			////		hs2 = GetCardHuaKind(iSecCard[0]);
			////		if ((!m_bCompareHuaSe) || (hs1 == hs2))
			////		{
			////			return 0;
			////		}
			////		return (hs1 - hs2 > 0 ? 1 : -1);
			////	}
			////case 3: // ��ֻ��һ����
			////	{
			////		pd1 = GetCardNum(iFirCard[2]);
			////		pd2 = GetCardNum(iSecCard[2]);
			////		if (pd1 != pd2)
			////		{
			////			return (pd1 - pd2 > 0 ? 1 : -1);
			////		}
			////		hs1 = GetCardHuaKind(iFirCard[2]);
			////		hs2 = GetCardHuaKind(iSecCard[2]);
			////		if ((!m_bCompareHuaSe) || (hs1 == hs2))
			////		{
			////			return 0;
			////		}
			////		return (hs1 - hs2 > 0 ? 1 : -1);
			////	}
			////case 4: // ������
			////	{
			////		pd1 = GetCardNum(iFirCard[2]);
			////		pd2 = GetCardNum(iSecCard[2]);
			////		if (pd1 != pd2)
			////	 {
			////		 return (pd1 - pd2 > 0 ? 1 : -1);
			////	 }

			////		pd1 = GetCardNum(iFirCard[3]);
			////		pd2 = GetCardNum(iSecCard[3]);
			////		if (pd1 != pd2)
			////	 {
			////		 return (pd1 - pd2 > 0 ? 1 : -1);
			////	 }
			////		hs1 = GetCardHuaKind(iFirCard[3]);
			////		hs2 = GetCardHuaKind(iSecCard[3]);
			////		if ((!m_bCompareHuaSe) || (hs1 == hs2))
			////		{
			////			return 0;
			////		}
			////		return (hs1 - hs2 > 0 ? 1 : -1);
			////	}
			//case 5: // ������  //5
			//	{
			//		pd1 = GetCardNum(iFirCard[2]);
			//		pd2 = GetCardNum(iSecCard[2]);
			//		if (pd1 != pd2)
			//	 {
			//		 return (pd1 - pd2 > 0 ? 1 : -1);
			//	 }

			//		pd1 = GetCardNum(iFirCard[3]);
			//		pd2 = GetCardNum(iSecCard[3]);
			//		if (pd1 != pd2)
			//	 {
			//		 return (pd1 - pd2 > 0 ? 1 : -1);
			//	 }

			//		pd1 = GetCardNum(iFirCard[4]);
			//		pd2 = GetCardNum(iSecCard[4]);
			//		if (pd1 != pd2)
			//	 {
			//		 return (pd1 - pd2 > 0 ? 1 : -1);
			//	 }
			//		// �Ƚ�����ƻ�ɫ
			//		hs1 = GetCardHuaKind(iFirCard[2]);
			//		hs2 = GetCardHuaKind(iSecCard[2]);
			//		if ((!m_bCompareHuaSe) || (hs1 == hs2))
			//		{
			//			return 0;
			//		}
			//		return (hs1 - hs2 > 0 ? 1 : -1);
			//	}
			//}
		}
	case SH_REGIUS_SAME_HUA_CONTINUE:   // �ʼ�ͬ��˳ 
	case SH_SMALL_SAME_HUA_CONTINUE:    // ��Сͬ��˳
	case SH_SAME_HUA_CONTINUE:       	// ͬ��˳
	{
		// ��ɫ�޴�С
		/*int hs1=GetCardHuaKind(iFirCard[0]),
			hs2=GetCardHuaKind(iSecCard[0]);
		if ((!m_bCompareHuaSe) || (hs1 == hs2))
		{
			return 0;
		}
		return (hs1 - hs2 > 0 ? 1 : -1); */

		int pd1 = GetCardNum(iFirCard[0]),
			pd2 = GetCardNum(iSecCard[0]);
		if (pd1 != pd2)
		{
			return (pd1 - pd2 > 0 ? 1 : -1);
		}
		else 
		{
			return 0;
		}
	}
	
	case SH_SAME_HUA:			// ͬ��
		{
			//SortCard(iFirCard,NULL,iFirstCount);
			//SortCard(iSecCard,NULL,iSecondCount);

			// �Ƚ�ÿ���ƴ�С
			int pd1 = GetCardNum(iFirCard[0]);
			int pd2 = GetCardNum(iSecCard[0]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}
			pd1 = GetCardNum(iFirCard[1]);
			pd2 = GetCardNum(iSecCard[1]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}
			pd1 = GetCardNum(iFirCard[2]);
			pd2 = GetCardNum(iSecCard[2]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}
			pd1 = GetCardNum(iFirCard[3]);
			pd2 = GetCardNum(iSecCard[3]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}
			pd1 = GetCardNum(iFirCard[4]);
			pd2 = GetCardNum(iSecCard[4]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}

			return 0;
			//hs1 = GetCardHuaKind(iFirCard[0]);
			//hs2 = GetCardHuaKind(iSecCard[0]);
			//if ((!m_bCompareHuaSe) || (hs1 == hs2))
			//{
			//	return 0;
			//}
			//return (hs1 - hs2 > 0 ? 1 : -1);
		}
	case SH_CONTINUE:			// ˳��
		{
			// ˳��ֻ�����һ������С����, Ϊʲô���ȵ�1��, ��Ϊ(AKQJ10��A2345�ڵ�һ�����г�ͻ��) duanxiaohui
			int pd1 = GetCardNum(iFirCard[4]),
				pd2 = GetCardNum(iSecCard[4]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}

			return 0;

			/*int hs1 = GetCardHuaKind(iFirCard[0]),
				hs2 = GetCardHuaKind(iSecCard[0]);
			if ((!m_bCompareHuaSe) || (hs1 == hs2))
			{
				return 0;
			}
			return (hs1 - hs2 > 0 ? 1 : -1);*/
		}

	case SH_TIE_ZHI:	// 4��
		{
			int pd1 = GetCardNum(iFirCard[0]),
				pd2 = GetCardNum(iSecCard[0]);

			// ������
			if(pd1!=pd2)
				return (pd1 - pd2 > 0 ? 1 : -1);

			// �ȵ�5����
			pd1 = GetCardNum(iFirCard[4]);
			pd2 = GetCardNum(iSecCard[4]);
			if(pd1!=pd2)
				return (pd1 - pd2 > 0 ? 1 : -1);

			return 0;
		}
	case SH_HU_LU:		// ��«
		{
			int pd1 = GetCardNum(iFirCard[0]),
				pd2 = GetCardNum(iSecCard[0]);

			// ������
			if(pd1!=pd2)
				return (pd1 - pd2 > 0 ? 1 : -1);

			// �ȶ���
			pd1 = GetCardNum(iFirCard[3]);
			pd2 = GetCardNum(iSecCard[3]);
			if(pd1!=pd2)
				return (pd1 - pd2 > 0 ? 1 : -1);

			return 0;
		}
		break;
	case SH_THREE:		// ����
		{
			int pd1 = GetCardNum(iFirCard[0]),
				pd2 = GetCardNum(iSecCard[0]);

			// ��ǰ��3����
			if(pd1!=pd2)
				return (pd1 - pd2 > 0 ? 1 : -1);

			// �ȵ�4����
			pd1 = GetCardNum(iFirCard[3]);
			pd2 = GetCardNum(iSecCard[3]);
			if(pd1!=pd2)
				return (pd1 - pd2 > 0 ? 1 : -1);

			// �ȵ�5����
			pd1 = GetCardNum(iFirCard[4]);
			pd2 = GetCardNum(iSecCard[4]);
			if(pd1!=pd2)
				return (pd1 - pd2 > 0 ? 1 : -1);


			return 0;
			//// 5����������ȫ���Ծ�����С����Ӱ��ֻ�Ƚ�5�ŵ�
			//pd1 = GetCardNum(iFirCard[5]);
			//pd2 = GetCardNum(iSecCard[5]);

			//if(pd1!=pd2)
			//	return (pd1 - pd2 > 0 ? 1 : -1);

			//pd1 = GetCardNum(iFirCard[6]);
			//pd2 = GetCardNum(iSecCard[6]);

			//if(pd1!=pd2)
			//	return (pd1 - pd2 > 0 ? 1 : -1);	// �����ж������С

			//if (!m_bCompareHuaSe)
			//{
			//	return 0;
			//}

			//    pd1 = GetCardHuaKind(iFirCard[3]);  // �����ж��ƵĻ�ɫ��С
			//	pd2 = GetCardHuaKind(iSecCard[3]);
			//if(pd1!=pd2)
			//   return (pd1 - pd2 > 0 ? 1 : -1);

			//     pd1 = GetCardHuaKind(iFirCard[4]);   
			//	 pd2 = GetCardHuaKind(iSecCard[4]);
			// if(pd1!=pd2)
			//   return (pd1 - pd2 > 0 ? 1 : -1);

			//    pd1 = GetCardHuaKind(iFirCard[5]);   
			//	pd2 = GetCardHuaKind(iSecCard[5]);
			//if(pd1!=pd2)
			//   return (pd1 - pd2 > 0 ? 1 : -1);
			//pd1 = GetCardHuaKind(iFirCard[6]);   
			//pd2 = GetCardHuaKind(iSecCard[6]);
			//if(pd1!=pd2)
			//	return (pd1 - pd2 > 0 ? 1 : -1);
			//return 0;
		}

	case SH_TWO_DOUBLE:	// �����ȱȽϵ�һ��,�ٱȽϵڶ���
		{
			// ��һ�ԱȽ�
			int pd1 = GetCardNum(iFirCard[0]),
				pd2 = GetCardNum(iSecCard[0]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}

			// �ڶ��ԱȽ�
			pd1 = GetCardNum(iFirCard[2]);
			pd2 = GetCardNum(iSecCard[2]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}

			// �ȵ�5����
			pd1 = GetCardNum(iFirCard[4]);
			pd2 = GetCardNum(iSecCard[4]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}

			return 0;

			//int hs1,hs2;

			//switch(iFirstCount)
			//{
			////case 4:	// ��Ϊ���ԣ��Ƚ�С�Ի�ɫ dxhע��
			////	{
			////		hs1 = GetCardHuaKind(iFirCard[2]);
			////		hs2 = GetCardHuaKind(iSecCard[2]);
			////		if ((!m_bCompareHuaSe) || (hs1 == hs2))
			////		{
			////			return 0;
			////		}
			////		return (hs1 - hs2 > 0 ? 1 : -1);
			////	}

			//case 5:	// ��һ���� //5
			//	{
			//		pd1 = GetCardNum(iFirCard[4]);
			//		pd2 = GetCardNum(iSecCard[4]);
			//		if (pd1 != pd2)
			//	 {
			//		 return (pd1 - pd2 > 0 ? 1 : -1);
			//	 }
			//		hs1 = GetCardHuaKind(iFirCard[4]);
			//		hs2 = GetCardHuaKind(iSecCard[4]);
			//		if ((!m_bCompareHuaSe) || (hs1 == hs2))
			//		{
			//			return 0;
			//		}
			//		return (hs1 - hs2 > 0 ? 1 : -1);
			//	}
			//}
		}
	case SH_OTHER:	// ���ƱȽ�
		{
			SortCard(iFirCard,NULL,iFirstCount);
			SortCard(iSecCard,NULL,iSecondCount);

			// �ȵ�1����
			int pd1 = GetCardNum(iFirCard[0]),
				pd2 = GetCardNum(iSecCard[0]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}

			// �ȵ�2����
			pd1 = GetCardNum(iFirCard[1]);
			pd2 = GetCardNum(iSecCard[1]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}

			// �ȵ�3����
			pd1 = GetCardNum(iFirCard[2]);
			pd2 = GetCardNum(iSecCard[2]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}

			// �ȵ�4����
			pd1 = GetCardNum(iFirCard[3]);
			pd2 = GetCardNum(iSecCard[3]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}

			// �ȵ�5����
			pd1 = GetCardNum(iFirCard[4]);
			pd2 = GetCardNum(iSecCard[4]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}

			return 0;

			//int hs1,hs2;
			//switch(iFirstCount)
			//{
			////case 1:   dxhע��
			////	{
			////		hs1 = GetCardHuaKind(iFirCard[0]);
			////		hs2 = GetCardHuaKind(iSecCard[0]);
			////		if ((!m_bCompareHuaSe) || (hs1 == hs2))
			////		{
			////			return 0;
			////		}
			////		return (hs1 - hs2 > 0 ? 1 : -1);
			////	}
			////case 2:
			////	{
			////		pd1 = GetCardNum(iFirCard[0]);
			////		pd2 = GetCardNum(iSecCard[0]);
			////		if (pd1 != pd2)
			////		{
			////			return (pd1 - pd2 > 0 ? 1 : -1);
			////		}
			////		pd1 = GetCardNum(iFirCard[1]);
			////		pd2 = GetCardNum(iSecCard[1]);
			////		if (pd1 != pd2)
			////		{
			////			return (pd1 - pd2 > 0 ? 1 : -1);
			////		}
			////		hs1 = GetCardHuaKind(iFirCard[1]);
			////		hs2 = GetCardHuaKind(iSecCard[1]);
			////		if ((!m_bCompareHuaSe) || (hs1 == hs2))
			////		{
			////			return 0;
			////		}
			////		return (hs1 - hs2 > 0 ? 1 : -1);
			////	}
			////case 3:
			////	{
			////		pd1 = GetCardNum(iFirCard[0]);
			////		pd2 = GetCardNum(iSecCard[0]);
			////		if (pd1 != pd2)
			////		{
			////			return (pd1 - pd2 > 0 ? 1 : -1);
			////		}
			////		pd1 = GetCardNum(iFirCard[1]);
			////		pd2 = GetCardNum(iSecCard[1]);
			////		if (pd1 != pd2)
			////		{
			////			return (pd1 - pd2 > 0 ? 1 : -1);
			////		}
			////		pd1 = GetCardNum(iFirCard[2]);
			////		pd2 = GetCardNum(iSecCard[2]);
			////		if (pd1 != pd2)
			////		{
			////			return (pd1 - pd2 > 0 ? 1 : -1);
			////		}
			////		hs1 = GetCardHuaKind(iFirCard[2]);
			////		hs2 = GetCardHuaKind(iSecCard[2]);
			////		if ((!m_bCompareHuaSe) || (hs1 == hs2))
			////		{
			////			return 0;
			////		}
			////		return (hs1 - hs2 > 0 ? 1 : -1);
			////	}
			////case 4:
			////	{
			////		pd1 = GetCardNum(iFirCard[0]);
			////		pd2 = GetCardNum(iSecCard[0]);
			////		if (pd1 != pd2)
			////		{
			////			return (pd1 - pd2 > 0 ? 1 : -1);
			////		}
			////		pd1 = GetCardNum(iFirCard[1]);
			////		pd2 = GetCardNum(iSecCard[1]);
			////		if (pd1 != pd2)
			////		{
			////			return (pd1 - pd2 > 0 ? 1 : -1);
			////		}
			////		pd1 = GetCardNum(iFirCard[2]);
			////		pd2 = GetCardNum(iSecCard[2]);
			////		if (pd1 != pd2)
			////		{
			////			return (pd1 - pd2 > 0 ? 1 : -1);
			////		}
			////		pd1 = GetCardNum(iFirCard[3]);
			////		pd2 = GetCardNum(iSecCard[3]);
			////		if (pd1 != pd2)
			////		{
			////			return (pd1 - pd2 > 0 ? 1 : -1);
			////		}
			////		hs1 = GetCardHuaKind(iFirCard[3]);
			////		hs2 = GetCardHuaKind(iSecCard[3]);
			////		if ((!m_bCompareHuaSe) || (hs1 == hs2))
			////		{
			////			return 0;
			////		}
			////		return (hs1 - hs2 > 0 ? 1 : -1);
			////	}
			//case 5:
			//	{
			//		pd1 = GetCardNum(iFirCard[0]);
			//		pd2 = GetCardNum(iSecCard[0]);
			//		if (pd1 != pd2)
			//		{
			//			return (pd1 - pd2 > 0 ? 1 : -1);
			//		}
			//		pd1 = GetCardNum(iFirCard[1]);
			//		pd2 = GetCardNum(iSecCard[1]);
			//		if (pd1 != pd2)
			//		{
			//			return (pd1 - pd2 > 0 ? 1 : -1);
			//		}
			//		pd1 = GetCardNum(iFirCard[2]);
			//		pd2 = GetCardNum(iSecCard[2]);
			//		if (pd1 != pd2)
			//		{
			//			return (pd1 - pd2 > 0 ? 1 : -1);
			//		}
			//		pd1 = GetCardNum(iFirCard[3]);
			//		pd2 = GetCardNum(iSecCard[3]);
			//		if (pd1 != pd2)
			//		{
			//			return (pd1 - pd2 > 0 ? 1 : -1);
			//		}
			//		pd1 = GetCardNum(iFirCard[4]);
			//		pd2 = GetCardNum(iSecCard[4]);
			//		if (pd1 != pd2)
			//		{
			//			return (pd1 - pd2 > 0 ? 1 : -1);
			//		}
			//		hs1 = GetCardHuaKind(iFirCard[0]);
			//		hs2 = GetCardHuaKind(iSecCard[0]);
			//		if ((!m_bCompareHuaSe) || (hs1 == hs2))
			//		{
			//			return 0;
			//		}
			//		return (hs1 - hs2 > 0 ? 1 : -1);
			//	}	
			//	/*case 7:
			//	{
			//		pd1 = GetCardNum(iFirCard[0]);
			//		pd2 = GetCardNum(iSecCard[0]);
			//		if (pd1 != pd2)
			//		{
			//			return (pd1 - pd2 > 0 ? 1 : -1);
			//		}
			//		pd1 = GetCardNum(iFirCard[1]);
			//		pd2 = GetCardNum(iSecCard[1]);
			//		if (pd1 != pd2)
			//		{
			//			return (pd1 - pd2 > 0 ? 1 : -1);
			//		}
			//		pd1 = GetCardNum(iFirCard[2]);
			//		pd2 = GetCardNum(iSecCard[2]);
			//		if (pd1 != pd2)
			//		{
			//			return (pd1 - pd2 > 0 ? 1 : -1);
			//		}
			//		pd1 = GetCardNum(iFirCard[3]);
			//		pd2 = GetCardNum(iSecCard[3]);
			//		if (pd1 != pd2)
			//		{
			//			return (pd1 - pd2 > 0 ? 1 : -1);
			//		}
			//		pd1 = GetCardNum(iFirCard[4]);
			//		pd2 = GetCardNum(iSecCard[4]);
			//		if (pd1 != pd2)
			//		{
			//			return (pd1 - pd2 > 0 ? 1 : -1);
			//		}
			//		pd1 = GetCardNum(iFirCard[5]);
			//		pd2 = GetCardNum(iSecCard[5]);
			//		if (pd1 != pd2)
			//		{
			//			return (pd1 - pd2 > 0 ? 1 : -1);
			//		}
			//		pd1 = GetCardNum(iFirCard[6]);
			//		pd2 = GetCardNum(iSecCard[6]);
			//		if (pd1 != pd2)
			//		{
			//			return (pd1 - pd2 > 0 ? 1 : -1);
			//		}
			//		hs1 = GetCardHuaKind(iFirCard[0]);
			//		hs2 = GetCardHuaKind(iSecCard[0]);
			//		if ((!m_bCompareHuaSe) || (hs1 == hs2))
			//		{
			//			return 0;
			//		}
			//		return (hs1 - hs2 > 0 ? 1 : -1);
			//	}	*/
			//}
		}
	}
	return -1;
}

// �Աȵ���
BOOL CUpGradeGameLogic::CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard)
{
	int iFirstNum = GetCardNum(iFirstCard);				// ������
	int iNextNum  = GetCardNum(iNextCard);				// ������

	if (iFirstCard == 0x4F) return FALSE;				// ����
	if (iNextCard == 0x4F) return TRUE;

	if (iFirstCard == 0x4E) return FALSE;				// С��
	if (iNextCard == 0x4E) return TRUE;

	if (iFirstNum == 2) return FALSE;					// 2
	if (iNextNum == 2) return TRUE;

	return ((iNextNum - iFirstNum) > 0 ? TRUE : FALSE);	// ����
}

// �Զ����ƺ���
BOOL CUpGradeGameLogic::AutoOutCard(BYTE iHandCard[], int iHandCardCount, BYTE iBaseCard[], int iBaseCardCount,
									BYTE iResultCard[], int & iResultCardCount, BOOL bFirstOut)
{
	if (bFirstOut == TRUE)
	{
		iResultCard[0] = iHandCard[iHandCardCount-1];
		iResultCardCount = 1;
	}
	return TRUE;
}


// ��� 0 λ�˿�
int CUpGradeGameLogic::RemoveNummCard(BYTE iCardList[], int iCardCount)
{
	int iRemoveCount=0;
	for (int i=0;i<iCardCount;i++)
	{
		if (iCardList[i]!=0) iCardList[i-iRemoveCount]=iCardList[i];
		else iRemoveCount++;
	}
	return iRemoveCount;
}

// �����˿�
BYTE CUpGradeGameLogic::RandCard(BYTE iCard[], int iCardCount)
{ 
	static const BYTE m_CardArray[52]={
		    /*2     3     4     5     6    7      8     9     10    J    Q      K     A*/
			0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,		//���� 2 - A
			0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,		//÷�� 2 - A
			0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,		//���� 2 - A
			0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D		//���� 2 - A
			/*0x4E, 0x4F*/};
			//С�����

	//static const BYTE m_CardArray[54]={
	//	    /*2     3     4     5     6    7      8     9     10    J    Q      K     A*/
	//		0x0B, 0x0C, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x11, 0x01, 0x02, 0x12, 0x0D,		//���� 2 - A
	//		0x09, 0x0A,	0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,		//÷�� 2 - A
	//		0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,		//���� 2 - A
	//		0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,		//���� 2 - A
	//		0x4E, 0x4F};
	//		//С�����

	//static const BYTE m_CardArray[54]={
	//	    /*2     3     4     5     6    7      8     9     10    J    Q      K     A*/
	//		0x14, 0x08, 0x03, 0x04, 0x0D, 0x06, 0x07, 0x11, 0x12, 0x0A, 0x0B, 0x0C, 0x05,		//���� 2 - A
	//		0x01, 0x02, 0x13, 0x09, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,		//÷�� 2 - A
	//		0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,		//���� 2 - A
	//		0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,		//���� 2 - A
	//		0x4E, 0x4F};


	BYTE iSend=0,iStation=0,iCardList[162];
	srand((unsigned)time(NULL));	

	::CopyMemory(iCard, m_CardArray, sizeof(m_CardArray));	
	
	int j = 0, n = 0;
	if(iCardCount != 28)
	  for (int i = 0; i < iCardCount; i += 12)
	    {
		  j = n * 13 + 1;
		  ::CopyMemory(&iCardList[i], &m_CardArray[j], sizeof(BYTE) * 12);
		   n++;
	    }
	else // ���Ż�ɫֻȡ8-A
	{
		for (int i = 0; i < iCardCount; i += 7)
	    {
		  j = n * 13 + 6;
		  ::CopyMemory(&iCardList[i], &m_CardArray[j], sizeof(BYTE) * 7);
		   n++;
	    }
	}

	for (int i = 0; i < iCardCount; i += 52)
		::CopyMemory(&iCardList[i], m_CardArray, sizeof(m_CardArray));

	do
	{
		iStation = rand() % (iCardCount - iSend);
		iCard[iSend] = iCardList[iStation];
		iSend++;
		iCardList[iStation] = iCardList[iCardCount - iSend];
	} while (iSend < iCardCount);

	return iCardCount;
}

// ɾ���˿�
int CUpGradeGameLogic::RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount)
{
	// ��������
	if ((iRemoveCount > iCardCount)) return 0;

	// ��Ҫɾ����������
	int iDeleteCount = 0;
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
	RemoveNummCard(iCardList,iCardCount);
	if (iDeleteCount != iRemoveCount)
		return 0;

	return iDeleteCount;
}


// ����==iCard�ĵ�������iCardList�е����
int  CUpGradeGameLogic::GetSerialBySpecifyCard(BYTE iCardList[],int iStart,int iCardCount,BYTE iCard)
{
	for(int i = iStart; i < iCardCount; i++)
	{
		if(iCardList[i] == iCard)
			return i;
	}
	return -1;
}

BOOL CUpGradeGameLogic::IsSmallRegiusContinue(BYTE iCardList[],int iCardCount)
{
	if(iCardCount < 4)
		return FALSE;

	for(int i = 0; i < iCardCount; i++)
	{
		if(iCardList[i] == 0)
			return FALSE;
	}

	BYTE Temp[61] = {0};
	for(int i = 0; i < iCardCount; i++)
	{
		Temp[iCardList[i]] ++;
	}
	
	if(Temp[13] && Temp[1] && Temp[2] && Temp[3] && Temp[4])
		return TRUE;
	if(Temp[29] && Temp[17] && Temp[18] && Temp[19] && Temp[20])
		return TRUE;
	if(Temp[45] && Temp[33] && Temp[34] && Temp[35] && Temp[36])
		return TRUE;
	if(Temp[61] && Temp[49] && Temp[50] && Temp[51] && Temp[52])
		return TRUE;
	
	return FALSE;
}

/**
* @brief ��7�����з�����5���������
* @param bHandCards[]     Ҫ����������
* @param nCount           ��������
* @param bPublicCards[]   Ҫ����������
* @param nPublicCount     ����������
* @param bResultCard[]    ���ط����õ���������
* @return ����
*/
int CUpGradeGameLogic::AnalysisCard(BYTE bHandCards[], int nHandCount, BYTE bPublicCards[], int nPublicCount, BYTE bResultCard[])
{
    if ((nHandCount + nPublicCount) != 7)
    {
        return 0;
    }
    
    int i, j;
    CByteArray arrCards;

    for (i = 0; i < nHandCount; i++)
    {
        arrCards.Add(bHandCards[i]);
    }
    
    for (i = 0; i < nPublicCount; i++)
    {
        arrCards.Add(bPublicCards[i]);
    }

    BYTE bCard[5] = {0};
    int nCardKind[21] = {0};

    //// 21����ɷ���
    //BYTE bIndex[21][5] = {0, 1, 2, 3, 4, \
    //                      0, 1, 2, 3, 5, \
    //                      0, 1, 2, 3, 6, \
    //                      0, 1, 2, 4, 5, \
    //                      0, 1, 2, 4, 6, \
    //                      0, 1, 2, 5, 6, \
    //                      0, 1, 3, 4, 5, \
    //                      0, 1, 3, 4, 6, \
    //                      0, 1, 3, 5, 6, \
    //                      0, 1, 4, 5, 6, \
    //                      0, 2, 3, 4, 5, \
    //                      0, 2, 3, 4, 6, \
    //                      0, 2, 3, 5, 6, \
    //                      0, 2, 4, 5, 6, \
    //                      0, 3, 4, 5, 6, \
    //                      1, 2, 3, 4, 5, \
    //                      1, 2, 3, 4, 6, \
    //                      1, 2, 3, 5, 6, \
    //                      1, 2, 4, 5, 6, \
    //                      1, 3, 4, 5, 6, \
    //                      2, 3, 4, 5, 6, \
    //                     };

	// 21����ɷ���
	BYTE bIndex[21][7] = {0, 1, 2, 3, 4, 5, 6,
		                  0, 1, 2, 3, 5, 4, 6,
		                  0, 1, 2, 3, 6, 4, 5,
		                  0, 1, 2, 4, 5, 3, 6,
		                  0, 1, 2, 4, 6, 3, 5,
		                  0, 1, 2, 5, 6, 3, 4,
		                  0, 1, 3, 4, 5, 2, 6,
		                  0, 1, 3, 4, 6, 2, 5,
		                  0, 1, 3, 5, 6, 2, 4, 
		                  0, 1, 4, 5, 6, 2, 3, 
		                  0, 2, 3, 4, 5, 1, 6, 
		                  0, 2, 3, 4, 6, 1, 5, 
		                  0, 2, 3, 5, 6, 1, 4, 
		                  0, 2, 4, 5, 6, 1, 3, 
		                  0, 3, 4, 5, 6, 1, 2, 
		                  1, 2, 3, 4, 5, 0, 6, 
		                  1, 2, 3, 4, 6, 0, 5, 
		                  1, 2, 3, 5, 6, 0, 4, 
		                  1, 2, 4, 5, 6, 0, 3, 
		                  1, 3, 4, 5, 6, 0, 2, 
		                  2, 3, 4, 5, 6, 0, 1
						 };

    for (i = 0; i < 21; i++)
    {	
        for (j = 0; j < 5; j++)
        {
            // �����±�ȡ��5����
            bCard[j] = arrCards.GetAt(bIndex[i][j]);
        }
      
        // ��ȡ����
        nCardKind[i] = GetCardShape(bCard, 5);

		//CString str;
		//str.Format("dxh: ��%d�ַ���, ����: %d",  i, nCardKind[i]);
		//OutputDebugString(str);
    }

    // ȡ�������λ��
    int nMax = 0;
    for (i = 1; i < 21; i++)
    {
        if (nCardKind[i] > nCardKind[nMax])
        {
            nMax = i;
        }
    }

    BYTE bCompareCard1[5], bCompareCard2[5];
    int nCompareMax = nMax;

    // �ҳ�������ͺ�, ���ҳ���ͬ������������(����һ�������, ���ȴ�С, �����������Ҫȡ������ͬ������)
    for (i = 0; i < 21; i++)
    {
        if (i == nMax)
        {
            continue;
        }

        for (j = 0; j < 5; j++)
        {
            bCompareCard1[j] = arrCards.GetAt(bIndex[i][j]);
            bCompareCard2[j] = arrCards.GetAt(bIndex[nCompareMax][j]);
        }
        
        // ����(-1:��, 0:��, 1:Ӯ)
        if (CompareCard(bCompareCard1, 5, bCompareCard2, 5) > 0)
        {
            nCompareMax = i;
        }
    }

    // ��7��������ȡ��5����������
    for (i = 0; i < 7; i++)
    {
        bResultCard[i] = arrCards.GetAt(bIndex[nCompareMax][i]);
    }

	CString str;
	str.Format("dxh: ��ǰ�����������: %d, ����ID:%d", nCompareMax, nCardKind[nCompareMax]);
	OutputDebugString(str);

    return nCardKind[nCompareMax];
}