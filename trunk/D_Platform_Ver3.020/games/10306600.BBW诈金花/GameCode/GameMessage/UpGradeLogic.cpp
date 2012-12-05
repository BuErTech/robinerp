#include "StdAfx.h"
#include "UpGradeLogic.h"
//���캯��
CUpGradeGameLogic::CUpGradeGameLogic(void)
{
	//m_iNTNum=0;
	//m_iNTHuaKind=UG_ERROR_HUA;
	m_iStation[4]=500;
	for (int i=0;i<4;i++) m_iStation[i]=100*i;
	
}


//��ȡ�˿˻�ɫ
BYTE CUpGradeGameLogic::GetCardHuaKind(BYTE iCard)
{ 
	int iHuaKind=(iCard&UG_HUA_MASK);
	
	return iHuaKind; 
}

//��ȡ�˿˴�С ��2 - 18 �� 15 ���������� �� 2 - 21 �� 15 ����������
int CUpGradeGameLogic::GetCardBulk(BYTE iCard, BOOL bExtVol)
{
	if ((iCard==0x4E)||(iCard==0x4F))
	{
		return bExtVol?(iCard-14):(iCard-62);	//��С��64+14-62=16			ֻ���ش�Сè��ֵ
	}

	int iCardNum=GetCardNum(iCard);
	int iHuaKind=GetCardHuaKind(iCard);

//	if (iCardNum==2)		
//	{
//		if(bExtVol)		
//			return ((iHuaKind>>4)+(15*4));
//		else
//			return 15;
//	}
	return ((bExtVol)?((iHuaKind>>4)+(iCardNum*4)):(iCardNum));
}

//�����˿�
BOOL CUpGradeGameLogic::SortCard(BYTE iCardList[], BYTE bUp[], int iCardCount)
{
	BOOL bSorted=TRUE,bTempUp;
	int iTemp,iLast=iCardCount-1,iStationVol[45];

	//��ȡλ����ֵ
	for (int i=0;i<iCardCount;i++)
	{
		iStationVol[i]=GetCardBulk(iCardList[i],TRUE);
		///if (iStationVol[i]>=15) iStationVol[i]+=m_iStation[4];
		///else iStationVol[i]+=m_iStation[GetCardHuaKind(iCardList[i],FALSE)>>4];
	}

	//�������
	do
	{
		bSorted=TRUE;
		for (int i=0;i<iLast;i++)
		{
			if (iStationVol[i]<iStationVol[i+1])
			{	
				//����λ��
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
		
	return TRUE;
}



//��������
BOOL CUpGradeGameLogic::ReSortCard(BYTE iCardList[], int iCardCount)
{
	SortCard(iCardList,NULL,iCardCount);
	//====�������Ŵ�С
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

			if(step>=i)			//�ҵ�һ������i����ȵ����鴮��ʼλ��Ϊj,����λ��Ϊj+setp-1
			{					//����Start��ʼ��j���������setp��
				if(j!=Start)				//�ų���ʼ��������
				{
					for(;j>=Start;j--)					//��Start����j�ź��ƶ�i��
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
	return true;
}


//��ȡ�˿�
BYTE CUpGradeGameLogic::GetCardFromHua(int iHuaKind, int iNum)
{
	if (iHuaKind!=UG_ERROR_HUA) return (iHuaKind+iNum-1);
	return 0;
}

//�Ƿ����
BOOL CUpGradeGameLogic::IsDouble(BYTE iCardList[], int iCardCount)
{
	if (iCardCount!=3)
		return FALSE;
	int temp[17]={0},itwo=0;
	for(int i=0;i<iCardCount;i++)
		temp[GetCardNum(iCardList[i])]++;

	for(int i=0;i<17;i++)
	{
		if(temp[i]==2)
			itwo++;
	}
	return (itwo==1);
}

//�Ƿ�����
BOOL CUpGradeGameLogic::IsThree(BYTE iCardList[], int iCardCount)
{
	if (iCardCount!=3)
		return FALSE;
	int temp[17]={0},ithree=0;;
	for(int i=0;i<iCardCount;i++)
		temp[GetCardNum(iCardList[i])]++;

	for(int i=0;i<17;i++)
	{
		if(temp[i]==3)
			ithree++;
	}
	return (ithree==1);
}
/*
//�Ƿ����ԣ��������ӣ�
BOOL CUpGradeGameLogic::IsCompleDouble(BYTE iCardList[], int iCardCount)
{
	if (iCardCount <4)
		return FALSE;

	int temp[17]={0},itwo=0;;
	for(int i=0;i<iCardCount;i++)
		temp[GetCardNum(iCardList[i])]++;

	for(int i=0;i<17;i++)
	{
		if(temp[i]==2)
			itwo++;
	}
	return (itwo==2);
}
*/
//�Ƿ�ͬ��(ͬ��Ϊ������Ϊһ�ֻ�ʽ)
BOOL CUpGradeGameLogic::IsSameHua(BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 3) return FALSE;
	int hs = -1;
	SortCard(iCardList,NULL,iCardCount);
	for (int i = 0;i < iCardCount;i++)
	{
		int hua = GetCardHuaKind(iCardList[i]);
		if (hs < 0)
		{
			hs = hua;
			continue;
		}
		if (hs != hua) return FALSE;
	}
	return TRUE;
}
BOOL CUpGradeGameLogic::IsSpecial(BYTE iCardList[],int iCardCount)
{
	if(iCardCount!=3)
		return false;
	SortCard(iCardList,NULL,iCardCount);
	if(GetCardNum(iCardList[0])==5
		&&GetCardNum(iCardList[1])==3
		&&GetCardNum(iCardList[2])==2)
		return true;
	return false;
}
/*
//�Ƿ�Ϊ��«������һ�����ƴ�2��һ�����ƣ�
BOOL CUpGradeGameLogic::IsHuLu(BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 5) return FALSE;
	
	ReSortCard(iCardList,iCardCount);

	int pd0 = GetCardNum(iCardList[0]),
		pd1 = GetCardNum(iCardList[1]),
		pd2 = GetCardNum(iCardList[2]),
		pd3 = GetCardNum(iCardList[3]),
		pd4 = GetCardNum(iCardList[4]);

	return ((pd0 == pd1) && (pd1 == pd2) && (pd3 == pd4));
}

//�Ƿ�Ϊ��֧��4��ͬ����С���ƣ�
BOOL CUpGradeGameLogic::IsTieZhi(BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 4) 
		return FALSE;
	int pd = -1;
	for (int i = 0;i < iCardCount;i++)
	{
		int dian = GetCardNum(iCardList[i]);
		if (pd < 0)
		{
			pd =dian;
			continue;
		}
		if (pd != dian) 
			return FALSE;
	}
	return TRUE;
}
*/
//�Ƿ�ͬ��˳��5�ţ�
BOOL CUpGradeGameLogic::IsSameHuaContinue(BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 3) 
		return FALSE;

	if (!IsSameHua(iCardList,iCardCount)) 
		return FALSE;
	if (!IsSingleContinue(iCardList,iCardCount)) return FALSE;

	return TRUE;
}


//�Ƿ�����һ
/*BOOL CUpGradeGameLogic::IsThreeAndOne(BYTE iCardList[], int iCardCount)
{
	if (!(iCardCount == 4 || iCardCount == 5)) return FALSE;
	
	//AfxMessageBox("�Ƿ�����һ");
	SortCard(iCardList,NULL,iCardCount);

	//��ͬ�Ŵ�һ���ƻ��һ����
	int iTemp = -1;
	for (int i = 0;i < iCardCount - 2;i++)
	{
		if (GetCardNum(iCardList[i]) == GetCardNum(iCardList[i+2]))
		{
			iTemp = GetCardNum(iCardList[i]);
			break;
		}
	}

	if (iTemp < 0) return FALSE;

	BYTE iTempCard[2];
	int iCnt = 0,ishunCnt = 0;
	
	for (int i = 0;i < iCardCount;i++)
	{
		if (iTemp == GetCardNum(iCardList[i])) 
		{
			ishunCnt++;
			continue;
		}
		iTempCard[iCnt++] = iCardList[i];
	}
	
	if (ishunCnt != 3) return FALSE;

	if (iCnt == 1)//����һ����
	{
		if (iTemp == GetCardNum(iTempCard[0])) return FALSE;

		BYTE iBackCard[4];
		int count = 0;
		for (int i = 0;i < iCardCount;i++)
		{
			if (iTemp == GetCardNum(iCardList[i])) 
			{
				iBackCard[count++] = iCardList[i];
				continue;
			}
			else
			{
				iBackCard[3] = iCardList[i];
			}
		}
		::CopyMemory(iCardList,iBackCard,sizeof(iBackCard));
		 
	}
	else if (iCnt == 2)//����һ��
	{
		if ((GetCardNum(iTempCard[0]) != GetCardNum(iTempCard[1])) || (iTemp == GetCardNum(iTempCard[0])))
			return FALSE;
		////AfxMessageBox("����һ��");
		BYTE iBackCard[5];
		int count = 0;
		int num = 0;
		for (int i = 0;i < iCardCount;i++)
		{
			if (iTemp == GetCardNum(iCardList[i])) 
			{
				iBackCard[count++] = iCardList[i];
				continue;
			}
			else
			{
				iBackCard[3+num] = iCardList[i];
				num++;
			}
		}
		::CopyMemory(iCardList,iBackCard,sizeof(iBackCard));
	}
	
	return TRUE;
}
*/
//�Ƿ�˳(����3��)
BOOL CUpGradeGameLogic::IsSingleContinue(BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 3) return FALSE;
	
	//if (IsSameHua(iCardList,iCardCount)) return FALSE;

	SortCard(iCardList,NULL,iCardCount);
	
	int hs = -1;
	int temp1,temp2,temp3;
	temp1=GetCardNum(iCardList[0]);
	temp2=GetCardNum(iCardList[1]);
	temp3=GetCardNum(iCardList[2]);
	for (int i = 0;i < iCardCount - 1;i++)
	{
		//if (GetCardNum(iCardList[i]) == 2 || iCardList[i] == 0x4E || iCardList[i] == 0x4F)//���ܺ�2�ʹ�С��
		//	return FALSE;
		
		if (GetCardNum(iCardList[i]) % 15 != (GetCardNum(iCardList[i+1]) + 1) % 15)
		{
		if(temp1==14 && temp2==3 &&temp3==2)//1,2,3��˳��
			return true;
		else 
			return FALSE;
				
		}
	}
	return TRUE;
}

//�Ƿ�˫˳(��������3��)
/*BOOL CUpGradeGameLogic::IsDoubleContinue(BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 6 || iCardCount % 2 != 0) return FALSE;

	SortCard(iCardList,NULL,iCardCount);

	for (int i = 0;i < iCardCount - 2;i++)
	{
		if (GetCardNum(iCardList[i]) == 2 || iCardList[i] == 0x4E || iCardList[i] == 0x4F)
			return FALSE;
		if ((GetCardNum(iCardList[i]) % 15) != ((GetCardNum(iCardList[i+2]) + 1) % 15))
			return FALSE;
	}
	return TRUE;
}

//�Ƿ���˳������ͬ������3����
BOOL CUpGradeGameLogic::IsThreeContinue(BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 9 || iCardCount % 3 != 0) return FALSE;

	SortCard(iCardList,NULL,iCardCount);

	for (int i = 0;i < iCardCount - 3;i++)
	{
		if (GetCardNum(iCardList[i]) == 2 || iCardList[i] == 0x4E || iCardList[0] == 0x4F)
			return FALSE;
		if ((GetCardNum(iCardList[i]) % 15) != ((GetCardNum(iCardList[i+3]) + 1) % 15))
			return FALSE;
	}
	return TRUE;
}

//�Ƿ�ɻ������(��˳+ͬ�����ĵ��Ż����)
BOOL CUpGradeGameLogic::IsPlaneAndWing(BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 8) return FALSE;
	
	SetCard(iCardList,iCardCount);
	
	bool IsDaiOne = true;
	int shunCount = 0,daiCount = 0;
	PSTypeNode *first = NULL;
	
	int iTempNum[5];
	int iCnt = 0;
	for (POSITION pos = m_ptrPSList.GetHeadPosition();pos != NULL;)
	{
		PSTypeNode *pn = (PSTypeNode *) m_ptrPSList.GetNext(pos);
		if (!pn) continue;
		if (!first)
		{
			first = pn;
			shunCount++;
			iTempNum[iCnt++] = pn->mps;
			continue;
		}
		if (first->mcount != 3) return FALSE;
		if (pn->mcount == 3)
		{
			if ((pn->mps + 1) % 15 == first->mps % 15)
			{
				first = pn;
				shunCount++;
				iTempNum[iCnt++] = pn->mps;
				continue;
			}
			else return FALSE;
		}

		if (pn->mcount == 2) 
		{
			daiCount++;
			IsDaiOne = false;
			continue;
		}
		else if (pn->mcount == 1)
		{
			if (!IsDaiOne) return FALSE;

			daiCount++;
			continue;
		}
		
	}

	if (shunCount != daiCount) return FALSE;

	BYTE iBackCard[20];
	iCnt = 0;
	int j = 0, p = 0;
	for (int i = 0;i < iCardCount;i++)
	{
		if (j == 3)
		{
			p++;
			j = 0;
		}
		if (GetCardNum(iCardList[i]) == iTempNum[p])
		{
			iBackCard[iCnt++] = iCardList[i];
			iCardList[i] = 0;
			j++;

		}
	}
	
	for (int i = 0;i < iCardCount;i++)
	{
		if (iCardList[i] != 0) iBackCard[iCnt++] = iCardList[i];
	}
	
	::CopyMemory(iCardList,iBackCard,sizeof(BYTE)*iCnt);

	return TRUE;
}

//�Ƿ��Ĵ�������ͬ��+2���Ż�2���ƣ�
BOOL CUpGradeGameLogic::IsFourAndTwo(BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 6) return FALSE;
	
	SortCard(iCardList,NULL,iCardCount);

	int iTempNum = -1;
	for (int i = 0;i < iCardCount - 3;i++)
	{
		if (GetCardNum(iCardList[i]) == GetCardNum(iCardList[i+3])) 
		{
			iTempNum = GetCardNum(iCardList[i]);
			break;
		}
	}

	if (iTempNum < 0) return FALSE;
	
	BYTE iTempCard[4];
	int iCnt = 0;

	for (int i = 0;i < iCardCount;i++)
	{
		if (iTempNum != GetCardNum(iCardList[i]))
		{
			iTempCard[iCnt++] = iCardList[i];
			continue;
		}
	}
	
	
	if (iCnt == 2) //�Ĵ�2����
	{
		if (GetCardNum(iTempCard[0]) == GetCardNum(iTempCard[1])) return FALSE;

		BYTE iBackCard[6];
		int j = 0;
		for (int i = 0;i < iCardCount;i++)
		{
			if (GetCardNum(iCardList[i]) == iTempNum) iBackCard[j++] = iCardList[i];
		}
		iBackCard[4] = (GetCardNum(iTempCard[0]) > GetCardNum(iTempCard[1])) ? iTempCard[0] : iTempCard[1];
		::CopyMemory(iCardList,iBackCard,sizeof(iBackCard));
	}
	else if (iCnt == 4) //�Ĵ�2����
	{
		if (!((IsDouble(iTempCard,2) && IsDouble(&iTempCard[2],2)) && (GetCardNum(iTempCard[0]) != GetCardNum(iTempCard[2])))) return FALSE;
		
		BYTE iBackCard[8];
		int j = 0;
		for (int i = 0;i < iCardCount;i++)
		{
			if (GetCardNum(iCardList[i]) == iTempNum) iBackCard[j++] = iCardList[i];
		}
		for (int i = 0;i < 4;i++)
		{
			iBackCard[j+i] = iTempCard[i];
		}
		::CopyMemory(iCardList,iBackCard,sizeof(iBackCard));
	}
	
	
	return TRUE;
}

//�Ƿ�510K
BOOL CUpGradeGameLogic::Is510KBomb(BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 3) return FALSE;

	bool h5 = false,
		 h10 = false,
		 hk = false;

	SortCard(iCardList,NULL,iCardCount);
	if (GetCardNum(iCardList[0]) == 13) hk = true;//13 Is K
	if (GetCardNum(iCardList[1]) == 10) h10 = true;
	if (GetCardNum(iCardList[2]) == 5) h5 =true;
	if (hk && h10 && h5) return TRUE;
	return FALSE;
}


//�Ƿ�ը��
BOOL CUpGradeGameLogic::IsBomb(BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 4) return FALSE;

	int pd1 = GetCardNum(iCardList[0]);
	
	for (int i = 0;i < iCardCount;i++)
	{
		if (GetCardNum(iCardList[i]) != pd1) return FALSE;
		else continue;
	}

	return TRUE;
}

//�Ƿ���
BOOL CUpGradeGameLogic::IsRocket(BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 4) return FALSE;
	SortCard(iCardList,NULL,iCardCount);
	return ((iCardList[0] == 0x4F)&&(iCardList[1] == 0x4F)&&(iCardList[2] == 0x4E)&&(iCardList[3] == 0x4E));
}

//�Ƿ�ͬ��
BOOL CUpGradeGameLogic::IsSameHua(BYTE iCardList[], int iCardCount)
{
	if (iCardCount <= 0) return FALSE;
	
	int iFirstHua = GetCardHuaKind(iCardList[0],FALSE);
	
	for (int i = 0;i < iCardCount;i++)
	{
		if (GetCardHuaKind(iCardList[i],FALSE) != iFirstHua) return FALSE;
		else continue;
	}

	return TRUE;
}
*/

/*void CUpGradeGameLogic::SetCard(BYTE iCardList[], int iCardCount)
{
	if (iCardCount <= 0) return;
	ClearPSInfo();
	
	//add node
	for (int i = 0;i < iCardCount;i++)
	{
		int pd = GetCardNum(iCardList[i]);
		bool flag = false;

		for (POSITION pos = m_ptrPSList.GetHeadPosition();pos != NULL;)
		{
			PSTypeNode * ps = (PSTypeNode *) m_ptrPSList.GetNext(pos);
			if (!ps) continue;
			if (pd == ps->mps)
			{
				ps->mcount++;
				flag = true;
				break;
			}
		}

		if (!flag)
		{
			PSTypeNode * pn = new PSTypeNode();
			pn->mps = pd;
			pn->mcount = 1;
			m_ptrPSList.AddTail(pn);
		}
	}

	CPtrList temp;
	bool Is2In = false;

	for (POSITION pos = m_ptrPSList.GetHeadPosition();pos != NULL;)
	{
		PSTypeNode * pi = (PSTypeNode *) m_ptrPSList.GetNext(pos);
		if (!pi) continue;
		PSTypeNode *pn = new PSTypeNode();
		pn->mps = pi->mps;
		pn->mcount = pi->mcount;
		temp.AddTail(pn);
		if (pn->mps == 2) Is2In = true;
	}

	ClearPSInfo();
	//sort
	for (POSITION pos = temp.GetHeadPosition();pos != NULL;)
	{
		PSTypeNode *pi = (PSTypeNode *) temp.GetNext(pos);
		if (!pi) continue;
		PSTypeNode *pn = new PSTypeNode();
		pn->mps = pi->mps;
		pn->mcount = pi->mcount;

		if (m_ptrPSList.IsEmpty())
		{
			m_ptrPSList.AddTail(pn);
			continue;
		}

		bool bAdd = false;
		for (POSITION pos1 = m_ptrPSList.GetHeadPosition();pos1 != NULL;)
		{	
			POSITION tp = pos1;
			PSTypeNode *pii = (PSTypeNode *) m_ptrPSList.GetNext(pos1);
			if (!pii) continue;

			if ((pn->mcount > pii->mcount) || ((pn->mcount == pii->mcount) && (pn->mps > pii->mps))) 
			{
				m_ptrPSList.InsertBefore(tp,pn);
				bAdd = true;
				continue;
			}

		}

		if (!bAdd)
		{
			m_ptrPSList.AddTail(pn);
		}
	}

	while (!temp.IsEmpty())
	{
		PSTypeNode *ps = (PSTypeNode *) temp.RemoveHead();
		//delete ps;
	}
	temp.RemoveAll();

	if (Is2In)
	{
		PSTypeNode *pii = (PSTypeNode *) m_ptrPSList.RemoveHead();
		if (!pii) return;
		if (pii->mps == 14)//14ΪA
		{
			m_ptrPSList.AddTail(pii);
		}
		else
		{
			m_ptrPSList.AddHead(pii);
		}
	}

	return;
}



void CUpGradeGameLogic::ClearPSInfo()
{
	while (!m_ptrPSList.IsEmpty())
	{
		PSTypeNode *ps = (PSTypeNode *) m_ptrPSList.RemoveHead();
		//delete ps;
	}
	m_ptrPSList.RemoveAll();

	return;
}

*/
//�õ����л�ɫ
BYTE CUpGradeGameLogic::GetCardListHua(BYTE iCardList[], int iCardCount)
{
	int iHuaKind=GetCardHuaKind(iCardList[0]);
	if (GetCardHuaKind(iCardList[iCardCount-1])!=iHuaKind) return UG_ERROR_HUA;
	return iHuaKind;
}

//��ȡ����
BYTE CUpGradeGameLogic::GetCardShape(BYTE iCardList[], int iCardCount)
{
	/***************************************************
	ͬ��˳>��֧>��«>ͬ��>˳��>����>����>����>ɢ��
	***************************************************/

	if (iCardCount <= 0) 	return UG_ERROR_KIND;//�Ƿ���
//	if (IsSameHuaContinue(iCardList,iCardCount)) return SH_SAME_HUA_CONTINUE;//ͬ��˳
//	if (IsTieZhi(iCardList,iCardCount)) return SH_TIE_ZHI;//��֧
//	if (IsHuLu(iCardList,iCardCount)) return SH_HU_LU;//��«
//	if (IsSameHua(iCardList,iCardCount)) return SH_SAME_HUA;//ͬ��
//	if (IsSingleContinue(iCardList,iCardCount)) return SH_CONTINUE;//˳��
//	if (IsThree(iCardList,iCardCount)) return SH_THREE;//����
//	if (IsCompleDouble(iCardList,iCardCount)) return SH_TWO_DOUBLE;//����
//	if (IsDouble(iCardList,iCardCount)) return SH_DOUBLE;//����
	if(IsThree(iCardList,iCardCount)) return SH_THREE;		//�������
	if(IsSameHuaContinue(iCardList,iCardCount)) return SH_SAME_HUA_CONTINUE;
	if (IsSameHua(iCardList,iCardCount)) return SH_SAME_HUA;//ͬ��
	if (IsSingleContinue(iCardList,iCardCount)) return SH_CONTINUE;//˳��
	if (IsDouble(iCardList,iCardCount)) return SH_DOUBLE;//����
	if(IsSpecial(iCardList,iCardCount)) return SH_SPECIAL;//����
	return SH_OTHER;//ɢ��
}

//�Ƚ��������ƵĴ�С
int CUpGradeGameLogic::CompareCard(BYTE iFirstCard[], int iFirstCount, BYTE iSecondCard[], int iSecondCount)
{
	//return 1:iFirstCard > iSecondCard
	/***************************************************
	ͬ��˳>��֧>��«>ͬ��>˳��>����>����>����>ɢ��
	***************************************************/

	//����>˳��>��>˳��>����>����>����
	//for example
	//AAA>����678>����679>����6����7��Ƭ8>88A>��ɫ����ͬ��235>������û���κ�������ͬ����
	BYTE iFirCard[3],iSecCard[3];

	::CopyMemory(iFirCard,iFirstCard,sizeof(BYTE)*iFirstCount);
	::CopyMemory(iSecCard,iSecondCard,sizeof(BYTE)*iSecondCount);

	ReSortCard(iFirCard,iFirstCount);
	ReSortCard(iSecCard,iSecondCount);

	BYTE iFirstCardKind = GetCardShape(iFirCard,iFirstCount),
		 iSecondCardKind = GetCardShape(iSecCard,iSecondCount);

	//���Ͳ�ͬ 235�Ա���
	if (iFirstCardKind != iSecondCardKind) 
	{
		if (iFirstCardKind == SH_THREE && iSecondCardKind == SH_SPECIAL)
			return -1;
		if (iFirstCardKind == SH_SPECIAL && iSecondCardKind == SH_THREE)
			return 1;
		return (iFirstCardKind - iSecondCardKind > 0) ? 1 : -1;
	}

	//������ͬ(�ȱȽ�����ƣ���Ƚϻ�ɫ)
	switch (iFirstCardKind)
	{
	case SH_SAME_HUA_CONTINUE://ͬ��˳
	case SH_CONTINUE://˳��
		{
			int s1 = GetCardNum(iFirCard[0]);
			int s2 = GetCardNum(iFirCard[1]);
			int s3 = GetCardNum(iFirCard[2]);
			int d1 = GetCardNum(iSecCard[0]);
			int d2 = GetCardNum(iSecCard[1]);
			int d3 = GetCardNum(iSecCard[2]);
			if (s1==14 && s2==3 && s3==2)
			{
				s1 = 0;
			}
			if (d1==14 && d2==3 && d3==2)
			{
				d1 = 0;
			}
			// �ȴ�С
			if (s1>d1) {return 1;}
			else if (s1<d1){return -1;}
			else
			{
				if (s2>d2)return 1;
				else if (s2<d2)return -1;
				else
				{
					if (s3>d3) return 1;
					else if (s3<d3) return -1;
					else return -1;	// ��������ƶ���һ���ģ���ȵ�����
				}
			}
			// �����A��K��Q��ɵ�˳�ӣ���������
			return -1;
		}
	case SH_THREE://����
	case SH_SAME_HUA://ͬ��
	case SH_OTHER:	//���ƱȽ�
		{
			//�Ƚϵ�һ����
			int pd1 = GetCardNum(iFirCard[0]),
				pd2 = GetCardNum(iSecCard[0]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}
			//�Ƚϵڶ�����
			pd1 = GetCardNum(iFirCard[1]),
				pd2 = GetCardNum(iSecCard[1]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}
			//�Ƚϵ�������
			pd1 = GetCardNum(iFirCard[2]),
				pd2 = GetCardNum(iSecCard[2]);
			if (pd1 != pd2)
			{
				return (pd1 - pd2 > 0 ? 1 : -1);
			}
			return -1;//who ����who��
		}
	case SH_SPECIAL:
		{
			return -1;//who ����who��
		}
	case SH_DOUBLE://����
		{

			int Fpd1 = GetCardNum(iFirCard[0]),
				Fpd2 = GetCardNum(iFirCard[1]);
			int Spd1 = GetCardNum(iSecCard[0]),
				Spd2 = GetCardNum(iSecCard[1]);
			int DuF = Fpd1 == Fpd2?Fpd1:Fpd2;
			int DuS = Spd1 == Spd2?Spd1:Spd2;
			if (DuF != DuS)
			{
				return (DuF - DuS > 0 ? 1 : -1);
			}
			int TreeCardF = Fpd1 == Fpd2?GetCardNum(iFirCard[2]):Fpd1;
			int TreeCardS = Spd1 == Spd2?GetCardNum(iSecCard[2]):Spd1;
			if (TreeCardF != TreeCardS)
			{
				return (TreeCardF - TreeCardS > 0 ? 1 : -1);
			}
			return -1;
		}
	}
	return -1;
}


//�Աȵ���
BOOL CUpGradeGameLogic::CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard)
{
	int iFirstNum = GetCardNum(iFirstCard);//������
	int iNextNum  = GetCardNum(iNextCard);//������

	if (iFirstCard == 0x4F) return FALSE;//����
	if (iNextCard == 0x4F) return TRUE;

	if (iFirstCard == 0x4E) return FALSE;//С��
	if (iNextCard == 0x4E) return TRUE;

	if (iFirstNum == 2) return FALSE;//2
	if (iNextNum == 2) return TRUE;

	return ((iNextNum - iFirstNum) > 0 ? TRUE : FALSE);//����

	/*int iFristHua=GetCardHuaKind(iFirstCard,FALSE);
	int iNextHua=GetCardHuaKind(iNextCard,FALSE);
	if (iFristHua!=iNextHua)
	{
		//��ͬ��ɫ�Ա�
		if (iFristHua==UG_NT_CARD) return TRUE;
		return (iNextHua!=UG_NT_CARD);
	}
	
	//ͬ��ɫ�Ա�
	return GetCardBulk(iFirstCard,FALSE)>=GetCardBulk(iNextCard,FALSE);*/
}

//�Զ����ƺ���
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


//��� 0 λ�˿�
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

//�����˿�
BYTE CUpGradeGameLogic::RandCard(BYTE iCard[], int iCardCount,int iDeskIndex)
{
	static const BYTE m_CardArray[54]={
			0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,		//���� 2 - A
			0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,		//÷�� 2 - A
			0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,		//���� 2 - A
			0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,		//���� 2 - A
			0x4E, 0x4F};																		//С�����

	BYTE iSend=0,iStation=0,iCardList[162];
	srand(GetTickCount()+iDeskIndex);
	//���Ż�ɫֻȡ8-A
/*	int j = 0,n = 0;
	for (int i = 0;i < iCardCount;i += 7)
	{
		j = n * 13 + 6;
		::CopyMemory(&iCardList[i],&m_CardArray[j],sizeof(BYTE)*7);
		n++;
	}*/
	for (int i=0;i<iCardCount;i+=52)
		::CopyMemory(&iCardList[i],m_CardArray,sizeof(m_CardArray));

	do
	{
		iStation=rand()%(iCardCount-iSend);
		iCard[iSend]=iCardList[iStation];
		iSend++;
		iCardList[iStation]=iCardList[iCardCount-iSend];
	} while (iSend<iCardCount);

	//for (int i=0;i<108;i++)
	//	::CopyMemory(&iCard[i],&m_CardArray[53],1);

	return iCardCount;
}

//ɾ���˿�
int CUpGradeGameLogic::RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount)
{
	//��������
	if ((iRemoveCount>iCardCount)) return 0;

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
	if (iDeleteCount!=iRemoveCount) return 0;

	return iDeleteCount;
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


