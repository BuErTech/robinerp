/**
 *	���ܣ� A-Star�㷨 ���ҹ������·��
 *  ���ߣ� Wolf.Li
 *  ���ڣ� 2010.02.04
 *  �ؼ��֣� Hֵ��Gֵ��Fֵ�������б��ر��б�
 *  ��ע�� �ؼ��������A���㷨
 */

extern void Msg(const char *lpszFormat, ...);

/**
 * ����Gֵ
 */
UINT CJqBoard::CountGVal()
{
	return m_CurrentPos.GVal+10;
}

/**
 * ����Hֵ
 * @param x,y �����λ������
 * @param iToX, iToY Ŀ��λ������
 */
UINT CJqBoard::CountHVal(int x, int y, int iToX, int iToY)
{
	int iUxy=0;
	POINT pt;
	pt.x = x;
	pt.y = y;

	if (GetCenterFarmWordPath(pt))
	{
		int iCount=2;
		int iMin = x<iToX ? x : iToX;
		int iMax = x>iToX ? x : iToX;

		while (iCount--)
		{
			//�ȴ�ֵ�ټ�������while���ʽΪ1��while����Ϊ0
			if (0==iCount)
			{
				iMin = y<iToY ? y : iToY;
				iMax = y>iToY ? y : iToY;
			}

			for (int i=iMin; i<=iMax; i++)
			{
				//ͳ������·�����ж�����λ��֮��ո�ģ��ҵ����ŵ�Hֵ�����'��'�֣�
				if ( (6==i || 8==i || 10==i)  &&  (6==i-2 || 8==i-2 || 10==i-2) )
				{
					iUxy++;
				}
			}
		}
	}

	Msg("Test:Step ���㣨%d,%d��Hֵ%d ",x,y,(abs(x-iToX) + abs(y-iToY) - iUxy)*10);
	return (abs(x-iToX) + abs(y-iToY) - iUxy)*10;
}

/**
 * ����Fֵ
 * @param POS ����POS��Fֵ
 */
bool CJqBoard::SetFVal(AStarStruct & POS)
{
	bool bFind = false;
	for (std::vector<AStarStruct>::iterator i=m_OpenList.begin(); i<m_OpenList.end(); ++i)
	{
		if ((*i).ptPos.x==POS.ptPos.x && (*i).ptPos.y==POS.ptPos.y)
		{
			//��ǰ����·���������
			if ((*i).FVal > POS.GVal+POS.HVal)
			{
				POS.FVal = POS.GVal+POS.HVal;
				*i = POS;

				for (std::vector<AStarStruct>::iterator iAll=m_AllList.begin(); iAll<m_AllList.end(); ++iAll)
				{
					if ((*iAll).ptPos.x==POS.ptPos.x && (*iAll).ptPos.y==POS.ptPos.y)
					{
						*iAll = POS;
						break;
					}
				}
			}
			
			Msg("Test:DDDD ��λ�� > %d,%d  ����λ��%d,%d (f%d, h%d, g%d)",POS.ptParent.x, POS.ptParent.y,POS.ptPos.x, POS.ptPos.y,\
				POS.FVal,POS.HVal,POS.GVal);
			bFind = true;
			break;
		}
	}

	//û�ҵ���������Gֵ��Hֵ
	if ( !bFind )
		POS.FVal = POS.GVal+POS.HVal;

	return bFind;
}

/**
 * ���Ҹ�λ���Ƿ��ڹر�λ��
 */
bool CJqBoard::FindCloseList(POINT pt)
{
	for (std::vector<POINT>::iterator i=m_CloseList.begin(); i<m_CloseList.end(); ++i)
	{
		if (pt.x==(*i).x && pt.y==(*i).y)
		{
			return true;
		}
	}

	return false;
}

/**
 * �����СFֵ
 */
AStarStruct CJqBoard::GetMinFVal()
{
	AStarStruct ptTmp;
	memset(&ptTmp, 0, sizeof(ptTmp));
	UINT uMinFVal=65536; 
	
	for (std::vector<AStarStruct>::size_type i=0; i<m_OpenList.size(); ++i)
	{
		if (FindCloseList(m_OpenList.at(i).ptPos))
			continue;

		if (uMinFVal>m_OpenList.at(i).FVal)
		{
			uMinFVal = m_OpenList.at(i).FVal;
			ptTmp = m_OpenList.at(i);
		}
	}

	return ptTmp;
}

/**
 * ���Ҹ�λ���Ƿ������б
 * @param pt ���ҵ�λ��
 */
bool CJqBoard::GetGoTiltPath(POINT pt)
{
	//����λ�ö�������б
	return (	(pt.x ==  6  && pt.y ==  5)	||  \
				(pt.x ==  5  && pt.y ==  6)	||  \
				(pt.x ==  5  && pt.y == 10)	||  \
				(pt.x ==  6  && pt.y == 11)	||  \
				(pt.x == 10  && pt.y == 11)	||  \
				(pt.x == 11  && pt.y == 10)	||  \
				(pt.x == 10  && pt.y ==  5)	||  \
				(pt.x == 11  && pt.y ==  6)
				);
}

/**
 * �����Ƿ����м����Ӹ�λ��
 * @param pt ���ҵ�λ��
 */
bool CJqBoard::GetCenterFarmWordPath(POINT pt)
{
	//����λ�ö����м䡮���λ��
	return (	(pt.x ==  6  && pt.y ==  6)	||  \
				(pt.x ==  6  && pt.y ==  8)	||  \
				(pt.x ==  6  && pt.y == 10)	||  \
				(pt.x ==  8  && pt.y ==  6)	||  \
				(pt.x ==  8  && pt.y ==  8)	||  \
				(pt.x ==  8  && pt.y == 10)	||  \
				(pt.x == 10  && pt.y ==  6)	||  \
				(pt.x == 10  && pt.y ==  8)	||  \
				(pt.x == 10  && pt.y == 10)
				);
}

/**
 * ���뿪���б�
 * @param ptParent �����λ������
 * @param iToX, iToY Ŀ��λ������
 */
void CJqBoard::AddOpenList(int iTo,int jTo, POINT ptParent)
{
	AStarStruct tmpPOS;
	memset(&tmpPOS, 0, sizeof(tmpPOS));
	char cAboutPos=4;
	//ǰ�ĸ��ؼ��������ң����ĸ�����б�� X����
	int iNumX[]={0, 1,  0, -1,   1, -1, -1,  1};
	//ǰ�ĸ��ؼ��������ң����ĸ�����б�� Y����
	int iNumY[]={1, 0, -1,  0,   1, -1,  1, -1};
	tmpPOS.ptParent.x = ptParent.x;
	tmpPOS.ptParent.y = ptParent.y;
	
	// �ø���λ�ò�������б
	if (GetGoTiltPath(ptParent))
		cAboutPos = 8;

	for (char i=0; i<cAboutPos; i++)
	{
		tmpPOS.ptPos.x = ptParent.x+iNumX[i];
		tmpPOS.ptPos.y = ptParent.y+iNumY[i];
		
		//�������������
		if ( !bIsInBoard(tmpPOS.ptPos.x,tmpPOS.ptPos.y) )
		{
			//�м����ָ�λ�õ��������ڲ��ֿ���һ��������Ҫ�ٲ�һ��·��ͼ������һ��·��ͨ�ģ�
			if (GetCenterFarmWordPath(ptParent))
			{
				tmpPOS.ptPos.x += iNumX[i];
				tmpPOS.ptPos.y += iNumY[i];
			}
			else
				continue;
		}

		//��λ������ ���Ҳ���Ŀ������
		if ( mBoard[tmpPOS.ptPos.x][tmpPOS.ptPos.y]!=JQ_TYPE_NONE && (tmpPOS.ptPos.x!=iTo || tmpPOS.ptPos.y!=jTo))
			continue;

		//���������·��(����һ���ı�����������)
		if (!bIsInRailway(tmpPOS.ptPos.x, tmpPOS.ptPos.y))
		{
			Msg("Test:DDDD ���������·��");
			continue;
		}

		//�����λ���ڹر��б��Ѵ��ڷ���
		if (FindCloseList(tmpPOS.ptPos))
			continue;

		tmpPOS.GVal = CountGVal();
		tmpPOS.HVal = CountHVal(tmpPOS.ptPos.x, tmpPOS.ptPos.y, iTo, jTo);
		
		if ( !SetFVal(tmpPOS) )
		{
			Msg("Test:DDDD ֧������ %d ��λ��%d,%d  ����λ��%d,%d (f%d, h%d, g%d)",cAboutPos,ptParent.x, ptParent.y,tmpPOS.ptPos.x, tmpPOS.ptPos.y,\
				tmpPOS.FVal,tmpPOS.HVal,tmpPOS.GVal);
			m_OpenList.push_back(tmpPOS);
			m_AllList.push_back(tmpPOS);
		}
	}
}

/**
 * ��������
 * @param iToX, iToY Ŀ��λ������
 * @param iFrom, jFrom Դλ������
 */
bool CJqBoard::bCanArrival(int iTo,int jTo,int iFrom,int jFrom)
{
	if(GET_A_JQ_NAME( mBoard[iFrom][jFrom]) != JQ_TYPE_NAME_GB)
		return false;

	bool bFind = false;
	AStarStruct tmpPOS;
	tmpPOS.ptPos.x = iFrom;
	tmpPOS.ptPos.y = jFrom;
	tmpPOS.FVal = 0;
	tmpPOS.GVal = 0;
	tmpPOS.HVal = 0;
	m_CurrentPos = tmpPOS;

	m_OpenList.clear();
	m_AllList.clear();
	m_CloseList.clear();
	m_OpenList.push_back(tmpPOS);
	m_AllList.push_back(tmpPOS);

	while (1)
	{
		if (m_OpenList.empty())
			break;

		for (std::vector<AStarStruct>::iterator it=m_OpenList.begin(); it<m_OpenList.end(); ++it)
		{
			if ((*it).ptPos.x==m_CurrentPos.ptPos.x && (*it).ptPos.y==m_CurrentPos.ptPos.y)
			{
				m_CloseList.push_back((*it).ptPos);	//���뵽�ر��б��в��ٲ�����λ��
				m_OpenList.erase(it);	//�ӿ����б���ɾ��
				break;
			}
		}

		//���õ�ǰλ�õ�����λ��
		AddOpenList(iTo, jTo, m_CurrentPos.ptPos);
		//����СFֵ
		m_CurrentPos = GetMinFVal();
		
		//�ҵ�Ŀ�ĵ�
		if (iTo==m_CurrentPos.ptPos.x && jTo==m_CurrentPos.ptPos.y)
		{
			bFind = true;
			break;
		}
	}

	//�õ�·��
	POINT ptOperator = m_CurrentPos.ptParent;
	
	//���ǵ����費�࣬��ԭ��˼·������
	//����·��ʱֱ��ѭ���������ң����ı�����ṹ
	while (bFind)
	{
		//�Ѿ��õ�·������λ�� ����
		if (ptOperator.x==iFrom && ptOperator.y==jFrom)
			break;

		for (std::vector<AStarStruct>::size_type i=0; i<m_AllList.size(); ++i)
		{
			if (m_AllList.at(i).ptPos.x==ptOperator.x && m_AllList.at(i).ptPos.y==ptOperator.y)
			{
				g_StepLine.Push(ptOperator.x, ptOperator.y);
				ptOperator = m_AllList.at(i).ptParent;
				break;
			}
		}
	}

	return bFind;
}

/// ������˸
void CJqBoard::SetFlashChess(int &iX, int &iJ, bool bFlash)
{ 
	m_bShowStartChess = bFlash; 
	iX = m_iStartI;
	iJ = m_iStartJ;
}