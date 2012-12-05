#include "MJ.h"

namespace MyGame
{
	CMJHand::CMJHand()
	{
		m_nCurrentLength = 0;
	}

	CMJHand::~CMJHand()
	{
		m_nCurrentLength = 0;
	}

    /**
    *  @brief ����CMJHand������
    *  @param[in] mjHands ����������
    *	@return �����������
    *  @sa
    */		
    CMJHand CMJHand::operator + (const CMJHand& mjHands)
    {
        CMJHand mjResult;
        mjResult.AddTiles(*this);
        mjResult.AddTiles((CMJHand)mjHands);
        return mjResult;
    }

    CMJHand CMJHand::operator - (const CMJHand& mjHands)
    {
        CMJHand mjResult, mjTmp;
        mjTmp.AddTiles((CMJHand)mjHands);

        for (BYTE i = 0; i < m_nCurrentLength; i++)
        {
            if (!mjTmp.IsHave(m_ucTile[i]))
            {
                mjResult.AddTile(m_ucTile[i]);
            }
            else
            {
                mjTmp.DelTile(m_ucTile[i]);
            }
        }

        return mjResult;
    }

    CMJHand CMJHand::operator += (const CMJHand& mjHands)
    {
        AddTiles((CMJHand)mjHands);
        return *this;
    }

    CMJHand CMJHand::operator -= (const CMJHand& mjHands)
    {
        DelTiles((CMJHand)mjHands);
        return *this;
    }

	/**
	*  @brief ����ָ��λ���ϵ�����
	*  @param[in]  nPos ��Ҫ���õ�λ��
	*  @param[in]  ucTile ���õ�������
	*  @warning  ������õ�λ�ó�����ǰ���Ƶ�ʵ�ʳ��ȣ��������õ������ݳ������ֵTILE_END��������ʧ��
	*/	
	void CMJHand::SetTile( int nPos, TILE ucTile )
	{
		if (nPos < 0 || nPos >= m_nCurrentLength || nPos >= MAX_TOTAL_TILES || ucTile >= TILE_END)
		{
			return ;
		}

		m_ucTile[nPos] = ucTile;
	}

	/**
	*  @brief ��ȡָ��λ���ϵ���
	*  @param[in]  nPos ��Ҫ��ȡ��λ��
	*  @return  �����ȡ��λ�ó�����ǰ���Ƶ�ʵ�ʳ��ȣ��ͻ᷵��TILE_BEGIN
	*/	
	TILE CMJHand::GetTile( int nPos )
	{
		if (nPos < 0 || nPos >= m_nCurrentLength)
		{
			return TILE_BEGIN;
		}

		return m_ucTile[nPos];
	}

	/**
	*  @brief ���һ������
	*  @param[in]  ucTile ��ӵ�������
	*  @param[in]  nNbr ��ӵ�����
	*  @param[in]  bIncrease �Ƿ�Ҫ��������
	*  @return  ������õ�λ�ó������Ƶ�ʵ����󳤶ȣ��������õ������ݳ������ֵTILE_END�������ʧ��
	*/
	BOOL CMJHand::AddTile( TILE ucTile, int nNbr, int bIncrease)
	{
		if (m_nCurrentLength + nNbr > MAX_TOTAL_TILES || ucTile >= TILE_END)
		{
			return FALSE;
		}

		if (bIncrease)
		{
			if (ucTile >= TILE_CHAR_1 && ucTile <= TILE_CHAR_9)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_CHAR_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BAMBOO_1 && ucTile <= TILE_BAMBOO_9)      // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAMBOO_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BALL_1 && ucTile <= TILE_BALL_9)          // Ͳ
			{
				if (ucTile + nNbr - 1 > TILE_BALL_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_EAST && ucTile <= TILE_BAI)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAI)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_FLOWER_CHUN && ucTile <= TILE_FLOWER_JU)  // ��
			{
				if (ucTile + nNbr - 1 > TILE_FLOWER_JU)
				{
					return FALSE;
				}
			}

			// �����Ӧ��������
			for (int i = 0; i < nNbr; i++)
			{
				m_ucTile[m_nCurrentLength + i] = ucTile + i;
			}

			m_nCurrentLength += nNbr;
		}
		else
		{
			//if (nNbr > 4)
			//{
			//	return FALSE;
			//}

			// ���nNbr����ͬ����
			for (int i = 0; i < nNbr; i++)
			{
				m_ucTile[m_nCurrentLength + i] = ucTile;
			}

			m_nCurrentLength += nNbr;
		}

		return TRUE;
	}

	/**
	*  @brief ɾ��һ������
	*  @param[in]  ucTile ָ��Ҫɾ������
	*  @param[in]  nNbr ɾ��������
	*  @param[in]  bIncrease �Ƿ�Ҫ����ɾ��
	*  @return  ���û�������ƾͻ᷵��FALSE
	*  @warning ɾ������������Ƶ�˳��
	*/	
	BOOL CMJHand::DelTile( TILE ucTile, int nNbr, int bIncrease)
	{
		if (m_nCurrentLength - nNbr < 0)
		{
			return FALSE;
		}

		if (bIncrease)
		{
			if (ucTile >= TILE_CHAR_1 && ucTile <= TILE_CHAR_9)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_CHAR_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BAMBOO_1 && ucTile <= TILE_BAMBOO_9)      // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAMBOO_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BALL_1 && ucTile <= TILE_BALL_9)          // Ͳ
			{
				if (ucTile + nNbr - 1 > TILE_BALL_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_EAST && ucTile <= TILE_BAI)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAI)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_FLOWER_CHUN && ucTile <= TILE_FLOWER_JU)  // ��
			{
				if (ucTile + nNbr - 1 > TILE_FLOWER_JU)
				{
					return FALSE;
				}
			}

			CMJHand mjHand;
			for (int i = 0; i < nNbr; i++)
			{
				mjHand.AddTile(ucTile + i, 1, FALSE);
			}

			return DelTiles(mjHand);
		}
		else
		{
			TILE ucTileArray[MAX_TOTAL_TILES] = {0};
			int nCount = 0;

			for (int i = 0, j = 0; i < m_nCurrentLength; i++)
			{
				if (m_ucTile[i] == ucTile && nCount < nNbr)
				{
					nCount++;
				}
				else
				{
					ucTileArray[j] = m_ucTile[i];
					j++;
				}
			}

			if (nCount > 0)
			{
				memcpy(m_ucTile, ucTileArray, sizeof(m_ucTile));
				m_nCurrentLength -= nCount;

				return TRUE;
			}
		}

		return FALSE;
	}

	/**
	*  @brief ���һ������
	*  @param[in]  tiles ��������
	*  @return  ��������;���ȳ�����󳤶�,����FALSE
	*/	
	BOOL CMJHand::AddTiles( CMJHand &tiles )
	{
		int nLenTiles = tiles.CurrentLength();

		if (m_nCurrentLength + nLenTiles > MAX_TOTAL_TILES)
		{
			return FALSE;
		}

		for (int i = 0; i < nLenTiles; i++)
		{
			m_ucTile[m_nCurrentLength + i] = tiles.GetTile(i);
		}

		m_nCurrentLength += nLenTiles;

		return TRUE;
	}

	/**
	*  @brief ɾ��һ������
	*  @param[in]  tiles ��������
	*  @return  ����Ҳ��������е���, ����FALSE
	*/	
	BOOL CMJHand::DelTiles( CMJHand &tiles )
	{
		TILE tmp;
		BYTE ucTile[MAX_TOTAL_TILES] = {0};
		int i, j, nDelCount = 0;
		int nLenTiles = tiles.CurrentLength();

		for (i = 0; i < nLenTiles; i++)
		{
			tmp = tiles.GetTile(i);
			for (j = 0; j < m_nCurrentLength; j++)
			{
				if (tmp == m_ucTile[j] && 0 == ucTile[j])
				{
					ucTile[j] = 1;
					nDelCount++;
					break;
				}
			}
		}

		for (i = 0, j = 0; i < m_nCurrentLength; i++)
		{
			if (0 == ucTile[i])
			{
				m_ucTile[j] = m_ucTile[i];
				j++;
			}
		}

		m_nCurrentLength -= nDelCount;

		return TRUE;
	}

	/**
	*  @brief �Ƿ�ָ�������ƶ������
	*  @param[in]  tiles ���ƶ���
	*  @return  �����������ȫ�������ڶ���tiles��, ����TRUE, ���򷵻�FALSE
	*/	
	BOOL CMJHand::IsSubSet( CMJHand &tiles )
	{
		BOOL bFind = TRUE;
		TILE tmp;
		BYTE ucTile[MAX_TOTAL_TILES] = {0};
		int i, j, nLenTiles = tiles.CurrentLength();

		for (i = 0; i < nLenTiles; i++)
		{
			bFind = FALSE;
			tmp = tiles.GetTile(i);

			for (j = 0; j < m_nCurrentLength; j++)
			{
				if (tmp == m_ucTile[j] && 0 == ucTile[j])
				{
					ucTile[j] = 1;
					bFind = TRUE;
					break;
				}
			}

			if (!bFind)
			{
				return FALSE;
			}
		}

		return TRUE;
	}

	/**
	*  @brief �Ƿ����ָ��������
	*  @param[in]  ucTile    ָ������
	*  @param[in]  nNbr      ����������
	*  @param[in]  bIncrease �Ƿ���������߿ɲ�������Ϊ9��������һ��˳�ӣ����Դ������nNbr = 3, bIncrease = TRUE
	*  @return �������ָ����������,����TRUE,���򷵻�FALSE
	*/	
	BOOL CMJHand::IsHave( TILE ucTile, int nNbr, int bIncrease)
	{
		if (ucTile > TILE_END)
		{
			return FALSE;
		}

		if (bIncrease)
		{
			if (ucTile >= TILE_CHAR_1 && ucTile <= TILE_CHAR_9)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_CHAR_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BAMBOO_1 && ucTile <= TILE_BAMBOO_9)      // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAMBOO_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BALL_1 && ucTile <= TILE_BALL_9)          // Ͳ
			{
				if (ucTile + nNbr - 1 > TILE_BALL_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_EAST && ucTile <= TILE_BAI)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAI)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_FLOWER_CHUN && ucTile <= TILE_FLOWER_JU)  // ��
			{
				if (ucTile + nNbr - 1 > TILE_FLOWER_JU)
				{
					return FALSE;
				}
			}

			BOOL bFind;
			int i, j;
			for (i = 0; i < nNbr; i++)
			{
				bFind = FALSE;

				for (j = 0; j < m_nCurrentLength; j++)
				{
					if (m_ucTile[j] == ucTile + i)
					{
						bFind = TRUE;
						break;
					}
				}

				if (!bFind)
				{
					return FALSE;
				}
			}

			return TRUE;
		}
		else
		{
			if (nNbr > 4)
			{
				return FALSE;
			}

			int nCount = 0;
			for (int i = 0; i < m_nCurrentLength; i++)
			{
				if (m_ucTile[i] == ucTile)
				{
					nCount++;

					if (nCount == nNbr)
					{
						return TRUE;
					}
				}
			}
		}

		return FALSE;
	}

	/**
	*  @brief �Ƿ��л���
	*  @return ���Ƶ�����
	*/	
	int CMJHand::IsHaveFlower()
	{
		int nCount = 0;
		for (int i = 0; i < m_nCurrentLength; i++)
		{
			if (m_ucTile[i] >= TILE_FLOWER_CHUN && m_ucTile[i] <= TILE_FLOWER_JU)
			{
				nCount++;
			}
		}

		return nCount;
	}

	/**
	*  @brief ����
	*  @param[in]  nMod 0:����,1:����
	*  @param[in]  nGod 0:�����Ʋ�������������,1:�����Ʒ������е�ͷ��,2:�����Ʒ������е�β��
	*  @param[in]  tilesGod ��������
	*/	
	void CMJHand::Sort( int nMod, int nGod, CMJHand* tilesGod)
	{
		int i, j;
		int nGodCount = 0;
		TILE tempTile;

		if (tilesGod != NULL)
		{
			if (1 == nGod || 2 == nGod)
			{
				// �����Ʒŵ����е�ͷ��
				for (i = 0; i < m_nCurrentLength; i++)
				{
					if (tilesGod->IsHave(m_ucTile[i]))
					{
						tempTile = m_ucTile[nGodCount];
						m_ucTile[nGodCount] = m_ucTile[i];
						m_ucTile[i] = tempTile;
						nGodCount++;
					}
				}

				// �����Ʒŵ����е�β��
				if (2 == nGod)
				{
					for (i = 0; i < nGodCount; i++)
					{
						Swap(i, m_nCurrentLength-i-1);
					}
				}
			}
		}

		if (0 == nMod)
		{
			// ����
			if (1 == nGod)
			{
				// ��������ǰͷ��
				// ����������
				for (i = 0; i < nGodCount; i++)
				{
					for (j = 0; j < nGodCount - i - 1; j++)
					{
						if (m_ucTile[j] > m_ucTile[j+1])
						{
							Swap(j, j+1);
						}
					}
				}

				// ������������
				for (i = 0; i < m_nCurrentLength - nGodCount; i++)
				{
					for (j = 0; j < m_nCurrentLength - nGodCount - i - 1; j++)
					{
						if (m_ucTile[j+nGodCount] > m_ucTile[j+nGodCount+1])
						{
							Swap(j+nGodCount, j+nGodCount+1);
						}
					}
				}
			}
			else
			{
				// ��������ǰβ��
				// ����������
				for (i = 0; i < nGodCount; i++)
				{
					for (j = 0; j < nGodCount - i - 1; j++)
					{
						if (m_ucTile[j+m_nCurrentLength-nGodCount] > m_ucTile[j+m_nCurrentLength-nGodCount+1])
						{
							Swap(j+m_nCurrentLength-nGodCount, j+m_nCurrentLength-nGodCount+1);
						}
					}
				}

				// ������������
				for (i = 0; i < m_nCurrentLength - nGodCount; i++)
				{
					for (j = 0; j < m_nCurrentLength - nGodCount - i - 1; j++)
					{
						if (m_ucTile[j] > m_ucTile[j+1])
						{
							Swap(j, j+1);
						}
					}
				}
			}
		}
		else if (1 == nMod)
		{
			// ����
			if (1 == nGod)  
			{
				// ��������ǰͷ��
				// ����������
				for (i = 0; i < nGodCount; i++)
				{
					for (j = 0; j < nGodCount - i - 1; j++)
					{
						if (m_ucTile[j] < m_ucTile[j+1])
						{
							Swap(j, j+1);
						}
					}
				}

				// ������������
				for (i = 0; i < m_nCurrentLength - nGodCount; i++)
				{
					for (j = 0; j < m_nCurrentLength - nGodCount - i - 1; j++)
					{
						if (m_ucTile[j+nGodCount] < m_ucTile[j+nGodCount+1])
						{
							Swap(j+nGodCount, j+nGodCount+1);
						}
					}
				}
			}
			else
			{
				// ��������ǰβ��
				// ����������
				for (i = 0; i < nGodCount; i++)
				{
					for (j = 0; j < nGodCount - i - 1; j++)
					{
						if (m_ucTile[j+m_nCurrentLength-nGodCount] < m_ucTile[j+m_nCurrentLength-nGodCount+1])
						{
							Swap(j+m_nCurrentLength-nGodCount, j+m_nCurrentLength-nGodCount+1);
						}
					}
				}

				// ������������
				for (i = 0; i < m_nCurrentLength - nGodCount; i++)
				{
					for (j = 0; j < m_nCurrentLength - nGodCount - i - 1; j++)
					{
						if (m_ucTile[j] < m_ucTile[j+1])
						{
							Swap(j, j+1);
						}
					}
				}
			}
		}
        else if (2 == nMod)
        {
            // ��������������������
            int temp, nNum[TILE_END] = {0};
  
            for (i = 0; i < m_nCurrentLength; i++)
            {
                nNum[i] = GetTileCount(m_ucTile[i]);
            }

            for (i = 0; i < m_nCurrentLength; i++)
            {
                for (j = i + 1; j < m_nCurrentLength; j++)
                {
                    if (nNum[i] > nNum[j])
                    {
                        temp = nNum[i];
                        nNum[i] = nNum[j];
                        nNum[j] = temp;
                        Swap(i, j);
                    }
                    else if (nNum[i] == nNum[j]) 
                    {
                        // ���������, �Ƚ��Ƶ��С
                        if (m_ucTile[i] > m_ucTile[j])
                        {
                            temp = nNum[i];
                            nNum[i] = nNum[j];
                            nNum[j] = temp;
                            Swap(i, j);
                        }
                    }
                }
            }
        }
        else if (3 == nMod)
        {
            // �����������н�������
            int temp, nNum[TILE_END] = {0};

            for (i = 0; i < m_nCurrentLength; i++)
            {
                nNum[i] = GetTileCount(m_ucTile[i]);
            }

            for (i = 0; i < m_nCurrentLength; i++)
            {
                for (j = i + 1; j < m_nCurrentLength; j++)
                {
                    if (nNum[i] < nNum[j])
                    {
                        temp = nNum[i];
                        nNum[i] = nNum[j];
                        nNum[j] = temp;
                        Swap(i, j);
                    }
                    else if (nNum[i] == nNum[j]) 
                    {
                        // ���������, �Ƚ��Ƶ��С
                        if (m_ucTile[i] < m_ucTile[j])
                        {
                            temp = nNum[i];
                            nNum[i] = nNum[j];
                            nNum[j] = temp;
                            Swap(i, j);
                        }
                    }
                }
            }
        }
	}

	/**
	*  @brief �������
	*/	
	void CMJHand::ReleaseAll()
	{
		m_nCurrentLength = 0;

		for (int i = 0; i < MAX_TOTAL_TILES; i++)
		{
			m_ucTile[i] = TILE_BEGIN;
		}
	}

	/**
	*  @brief �Ƿ�ɳ���
	*  @param[in]  ucTile ָ���Ե���
	*  @param[in]  bWind  �����Ƿ�ɳ�
    *  @param[in]  bArrow ��ͷ���Ƿ�ɳ�
    *  @param[in/out] mjSet ���ؿ��ԳԵ�������
	*  @return  100:ͷ, 10:��, 1:β

	*
	*  ���ص���10��������,���ͬʱ������ͷ��β��,�ͻ᷵��101.
	*/	
	int CMJHand::IsCanCollect( TILE ucTile, int bWind, int bArrow, CMJSet & mjSet )
	{   
		if (!bWind && !bArrow && ucTile >= TILE_EAST && ucTile <= TILE_BAI)
		{
			// ���ƺͼ�ͷ�Ʋ��ɳ�
			return 0;
		}

		int nReValue = 0;
		int nValue = GetTileNumber(ucTile);
        BYTE byTile[4];

		if (ucTile >= TILE_CHAR_1 && ucTile <= TILE_BALL_9)
		{
			// ������Ͳ 1-9�����ж�
			if (nValue <= 7)
			{
				// ͷ
				if (IsHave(ucTile+1) && IsHave(ucTile+2))
				{
					nReValue += 100;

                    byTile[0] = ucTile;
                    byTile[1] = ucTile + 1;
                    byTile[2] = ucTile + 2;
                    byTile[3] = ucTile;
                    mjSet.AddSet(byTile, ACTION_COLLECT, 0);
				}
			}

			if (nValue >= 2 && nValue <= 8)
			{
				// ��
				if (IsHave(ucTile-1) && IsHave(ucTile+1))
				{
					nReValue += 10;

                    byTile[0] = ucTile - 1;
                    byTile[1] = ucTile;
                    byTile[2] = ucTile + 1;
                    byTile[3] = ucTile;
                    mjSet.AddSet(byTile, ACTION_COLLECT, 0);
				}
			}

			if (nValue >= 3)
			{
				// β
				if (IsHave(ucTile-1) && IsHave(ucTile-2))
				{
					nReValue += 1; 

                    byTile[0] = ucTile - 2;
                    byTile[1] = ucTile - 1;
                    byTile[2] = ucTile;
                    byTile[3] = ucTile;
                    mjSet.AddSet(byTile, ACTION_COLLECT, 0);
				}
			}
		}

        if (nReValue > 0)
        {
            return nReValue;
        }

        if (bWind && ucTile >= TILE_EAST && ucTile <= TILE_NORTH)
        {
            BOOL bIsHave = FALSE;

            // �����ϡ������������ж�(��������, �����뱣֤�����е�3���ƶ���һ��)
            switch (ucTile)
            {
            case TILE_EAST:  // ��
                {
                    if (IsHave(ucTile+1) && IsHave(ucTile+2))
                    {
                        byTile[0] = ucTile;
                        byTile[1] = ucTile + 1;
                        byTile[2] = ucTile + 2;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);

                        bIsHave = TRUE;
                    }

                    if (IsHave(ucTile+1) && IsHave(ucTile+3))
                    {
                        byTile[0] = ucTile;
                        byTile[1] = ucTile + 1;
                        byTile[2] = ucTile + 3;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);

                        bIsHave = TRUE;
                    }

                    if (IsHave(ucTile+2) && IsHave(ucTile+3))
                    {
                        byTile[0] = ucTile;
                        byTile[1] = ucTile + 2;
                        byTile[2] = ucTile + 3;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);

                        bIsHave = TRUE;
                    }

                    if (bIsHave)
                    {
                        nReValue += 100;
                    }

                    /*if (IsHave(ucTile+1) && IsHave(ucTile+2)
                     || IsHave(ucTile+1) && IsHave(ucTile+3)
                     || IsHave(ucTile+2) && IsHave(ucTile+3))
                    {
                        nReValue += 100;
                    }*/
                }
                break;
            case TILE_SOUTH: // ��
                {
                    if (IsHave(ucTile+1) && IsHave(ucTile+2))
                    {
                        byTile[0] = ucTile;
                        byTile[1] = ucTile + 1;
                        byTile[2] = ucTile + 2;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);

                        // ͷ
                        nReValue += 100;
                    }

                    if (IsHave(ucTile-1) && IsHave(ucTile+1))
                    {
                        byTile[0] = ucTile - 1;
                        byTile[1] = ucTile;
                        byTile[2] = ucTile + 1;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);

                        // ��
                        bIsHave = TRUE;
                    }

                    if (IsHave(ucTile-1) && IsHave(ucTile+2))
                    {
                        byTile[0] = ucTile - 1;
                        byTile[1] = ucTile;
                        byTile[2] = ucTile + 2;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);

                        // ��
                        bIsHave = TRUE;
                    }

                    //if (IsHave(ucTile+1) && IsHave(ucTile+2))
                    //{
                    //    // ͷ
                    //    nReValue += 100;
                    //}

                    //if (IsHave(ucTile-1) && IsHave(ucTile+1)
                    //    || IsHave(ucTile-1) && IsHave(ucTile+2))
                    //{
                    //    // ��
                    //    nReValue += 10;
                    //}

                    if (bIsHave)
                    {
                        nReValue += 10;
                    }
                }
                break;
            case TILE_WEST:  // ��
                {
                    if (IsHave(ucTile-1) && IsHave(ucTile+1))
                    {
                        byTile[0] = ucTile - 1;
                        byTile[1] = ucTile;
                        byTile[2] = ucTile + 1;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);

                        // ��
                        nReValue += 100;
                    }

                    if (IsHave(ucTile-2) && IsHave(ucTile+1))
                    {
                        byTile[0] = ucTile - 2;
                        byTile[1] = ucTile;
                        byTile[2] = ucTile + 1;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);

                        // ��
                        bIsHave = TRUE;
                    }

                    if (IsHave(ucTile-1) && IsHave(ucTile-2))
                    {
                        byTile[0] = ucTile - 2;
                        byTile[1] = ucTile - 1;
                        byTile[2] = ucTile;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);

                        // β
                        nReValue += 1;
                    }

                    //if (IsHave(ucTile-1) && IsHave(ucTile+1)
                    //    || IsHave(ucTile-2) && IsHave(ucTile+1))
                    //{
                    //    // ��
                    //    nReValue += 10;
                    //}

                    //if (IsHave(ucTile-1) && IsHave(ucTile-2))
                    //{
                    //    // β
                    //    nReValue += 1;
                    //}

                    if (bIsHave)
                    {
                        nReValue += 10;
                    }
                }
                break;
            case TILE_NORTH: // ��
                {
                    if (IsHave(ucTile-1) && IsHave(ucTile-2))
                    {
                        byTile[0] = ucTile - 2;
                        byTile[1] = ucTile - 1;
                        byTile[2] = ucTile;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);

                        bIsHave = TRUE;
                    }

                    if (IsHave(ucTile-1) && IsHave(ucTile-3))
                    {
                        byTile[0] = ucTile - 3;
                        byTile[1] = ucTile - 1;
                        byTile[2] = ucTile;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);

                        bIsHave = TRUE;
                    }

                    if (IsHave(ucTile-2) && IsHave(ucTile-3))
                    {
                        byTile[0] = ucTile - 3;
                        byTile[1] = ucTile - 2;
                        byTile[2] = ucTile;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);

                        bIsHave = TRUE;
                    }

                    if (bIsHave)
                    {
                        nReValue += 1;
                    }

                    //if (IsHave(ucTile-1) && IsHave(ucTile-2)
                    //    || IsHave(ucTile-1) && IsHave(ucTile-3)
                    //    || IsHave(ucTile-2) && IsHave(ucTile-3))
                    //{
                    //    // β
                    //    nReValue += 1;
                    //}
                }
                break;
            }

            if (nReValue > 0)
            {
                return nReValue;
            }
        }

        if (bArrow && ucTile >= TILE_ZHONG && ucTile <= TILE_BAI)
		{
			// �С������׳����ж�(�����б������з���)
			switch (ucTile)
			{
			case TILE_ZHONG: // ��
				{
					if (IsHave(ucTile+1) && IsHave(ucTile+2))
					{
						// ͷ
						nReValue += 100;

                        byTile[0] = ucTile;
                        byTile[1] = ucTile + 1;
                        byTile[2] = ucTile + 2;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);
					}
				}
				break;
			case TILE_FA:    // ��
				{
					if (IsHave(ucTile-1) && IsHave(ucTile+1))
					{
						// ��
						nReValue += 10;

                        byTile[0] = ucTile - 1;
                        byTile[1] = ucTile;
                        byTile[2] = ucTile + 1;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);
					}
				}
				break;
			case TILE_BAI:   // ��
				{
					if (IsHave(ucTile-1) && IsHave(ucTile-2))
					{
						// β
						nReValue += 1;

                        byTile[0] = ucTile - 2;
                        byTile[1] = ucTile - 1;
                        byTile[2] = ucTile;
                        byTile[3] = ucTile;
                        mjSet.AddSet(byTile, ACTION_COLLECT, 0);
					}
				}
				break;
			}
		}

		return nReValue;
	}

	/**
	*  @brief ����
	*  @param[in]  nPos1 ��һ��λ��
	*  @param[in]  nPos2 �ڶ���λ��
	*  @return  �����ɹ�����TRUE,���򷵻�FALSE
	*
	*  ����������ʵ�ʵĳ����н���,����ʧ��
	*/	
	BOOL CMJHand::Swap( int nPos1, int nPos2 )
	{
		if (nPos1 < 0 || nPos2 < 0
			|| nPos1 >= m_nCurrentLength || nPos2 >= m_nCurrentLength
			|| nPos1 == nPos2)
		{
			return FALSE;
		}

		TILE tempTile;
		tempTile = m_ucTile[nPos1];
		m_ucTile[nPos1] = m_ucTile[nPos2];
		m_ucTile[nPos2] = tempTile;

		return TRUE;
	}

	/**
	*  @brief ���õ�ǰ����
	*  @param[in]  nCurLength ����
	*/	
	void CMJHand::CurrentLength( const int& nCurLength )
	{
		if (nCurLength > MAX_TOTAL_TILES)
		{
			return ;
		}

		m_nCurrentLength = nCurLength;
	}

	/**
	*  @brief ��ȡָ���Ƶ�����
	*  @param[in]  ucTile ������
	*  @return  ��:0��Ͳ:1����:2����:3����:4������:5
	*
	*  ����ֵ�к궨��ɲο�
	*/	
	int CMJHand::GetTileGenre( TILE ucTile )
	{
        if (ucTile % 10 == 0
         || ucTile > TILE_BAI && ucTile < TILE_FLOWER_CHUN
         || ucTile > TILE_FLOWER_JU)
        {
            return TILE_GENRE_OTHER;
        }

		int nKind = ucTile/10;

		if (nKind >= TILE_GENRE_CHAR && nKind <= TILE_GENRE_FLOWER)
		{
			return nKind;
		}

		return TILE_GENRE_OTHER;
	}

	/**
	*  @brief ��ȡָ���Ƶ��Ƶ�
	*  @param[in]  ucTile ������
	*  @return  ȥ�����͵���ֵ
	*
	*  ����ֵ�к궨��ɲο�
	*/	
	int CMJHand::GetTileNumber( TILE ucTile )
	{
		return ucTile%10;
	}

    /**
    *  @brief ��ȡָ���Ƶ�ӵ�е�����
    *  @param[in]  ucTile ������
    *  @return  ���Ƶ�����
    */	
    int CMJHand::GetTileCount( TILE ucTile )
    {
        int nCount = 0;
        for (int i = 0; i < m_nCurrentLength; i++)
        {
            if (m_ucTile[i] == ucTile)
            {
                nCount++;
            }
        }

        return nCount;
    }

	/**
	*  @brief �������Ƶ����һ��
	*  @return  ���û���˷���TILE_BEGIN
	*/	
	TILE CMJHand::PopTile()
	{
		if (m_nCurrentLength <= 0)
		{
			return TILE_BEGIN;
		}

		m_nCurrentLength--;

		TILE tile = m_ucTile[m_nCurrentLength];

		m_ucTile[m_nCurrentLength] = TILE_BEGIN;

		return tile;
	}

	/**
	*  @brief ���һ������
	*  @param[in]  ucTile ���ӵ�����
	*
	*  �൱��ִ����3�����ͬһ����
	*/	
	void CMJHand::AddTriplet( TILE ucTile )
	{
		AddTile(ucTile, 3);
	}

	/**
	*  @brief ���һ��˳��
	*  @param[in]  ucTile ˳�ӵ�һ��������
	*  @warning ���Խ����Ȼ����ӽ�ȥ,�������9��,��ʵ�����9,10,11(һ��)��3������
	*/	
	void CMJHand::AddCollect( TILE ucTile )
	{
		AddTile(ucTile, 3, TRUE);
	}

	/**
	*  @brief ��ȡ����������
	*  @param[in] nHandTiles ������
	*  @return  �Ƴ���
	*/	
	int CMJHand::GetAllTile( int nHandTiles[] )
	{
		memset(nHandTiles, 0, sizeof(nHandTiles));

		for (int i = 0; i < m_nCurrentLength; i++)
		{
			nHandTiles[i] = m_ucTile[i];
		}

		return m_nCurrentLength;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*   CMJSet */
	CMJSet::CMJSet()
	{
		m_nCurrentLength = 0;
		for (int i = 0; i < MAX_SET_TILES; i++)
		{
			m_pParam[i] = NULL;
		}
	}

	CMJSet::~CMJSet()
	{
		m_nCurrentLength = 0;

		// ��⸽����Ϣ�Ƿ���Ҫ�ͷ��ڴ�
		for (int i = 0; i < MAX_SET_TILES; i++)
		{
			if (m_pParam[i] != NULL)
			{
				delete m_pParam[i];
				m_pParam[i] = NULL;
			}
		}
	}

	/**
	*  @brief ���һ������
	*  @param[in]  ucTile  �����ܵ�������
	*  @param[in]  ucFlag  ���Ʊ��(��,��,��)
	*  @param[in]  ucChair ����Ŀ�����(����ǳ��ƾͱ�ǵ��ǳԵ���һ��)
	*  @return �ɹ�TRUE,ʧ��FALSE
	*/	
	BOOL CMJSet::AddSet( TILE ucTile[4], BYTE ucFlag, BYTE ucChair)
	{
		if (m_nCurrentLength >= MAX_SET_TILES)
		{
			return FALSE;
		}

		TILE_SET tileSet;
		/*tileSet.m_ucTile  = ucTile;*/
        memcpy(tileSet.m_ucTile, ucTile, sizeof(tileSet.m_ucTile));
		tileSet.m_ucFlag  = ucFlag;
		tileSet.m_ucChair = ucChair;

		m_TileSet[m_nCurrentLength] = tileSet;
		m_nCurrentLength++;

		return TRUE;
	}

	/**
	*  @brief ���һ������
	*  @param[in]  tagSet ��������
	*  @return �ɹ�TRUE,ʧ��FALSE
	*/	
	BOOL CMJSet::AddSet( TILE_SET tagSet )
	{
		if (m_nCurrentLength >= MAX_SET_TILES)
		{
			return FALSE;
		}

		m_TileSet[m_nCurrentLength] = tagSet;
		m_nCurrentLength++;

		return TRUE;
	}

	/**
	*  @brief ���һ������
	*  @param[in]  mjSet Ҫ��ӵ��������ݽṹ
	*  @return �ɹ�TRUE,ʧ��FALSE
	*/	
	BOOL CMJSet::AddSet( CMJSet& mjSet )
	{
		int nLen = mjSet.CurrentLength();
		if (nLen <= 0 || m_nCurrentLength + nLen > MAX_SET_TILES)
		{
			return FALSE;
		}

		TILE_SET tileSet;
		for (int i = 0; i < nLen; i++)
		{
			mjSet.GetSet(i, tileSet);
			m_TileSet[m_nCurrentLength + i] = tileSet;
		}

		m_nCurrentLength += nLen;

		return TRUE;
	}

	/**
	*  @brief ��ȡһ������
	*  @param[in]  nPos Ҫ��ȡ��λ��
	*  @param[out]  tileset ��ȡ�����ݽṹ
	*  @return �ɹ�TRUE,ʧ��FALSE
	*/	
	BOOL CMJSet::GetSet( int nPos, TILE_SET &tileSet )
	{
		if (nPos < 0 || nPos >= m_nCurrentLength)
		{
            memset(&tileSet, 0, sizeof(tileSet));
			return FALSE;
		}

		tileSet = m_TileSet[nPos];

		return TRUE;
	}

	/**
	*  @brief �趨һ������
	*  @param[in]  nPos Ҫ�趨��λ��
	*  @param[in]  tileset �趨�����ݽṹ
	*  @return �ɹ�TRUE,ʧ��FALSE
	* 
	*  λ�ò��ܳ���MAX_SET_TILES, �����趨ʧ��
	*/	
	void CMJSet::SetSet( int nPos, TILE_SET &tileSet )
	{
		if (nPos < 0 || nPos >= m_nCurrentLength || nPos >= MAX_SET_TILES)
		{
			return ;
		}

		m_TileSet[nPos] = tileSet;
	}

	/**
	*  @brief �����������е���������������
	*  @param[in]  ucTile �������ܵ���
	*  @return ��������д�������Ƶ����ͳɹ�TRUE,����ʧ��FALSE
	*/
	BOOL CMJSet::TripletToQuadruplet( TILE ucTile )
	{
		for (int i = 0; i < m_nCurrentLength; i++)
		{
			if (m_TileSet[i].m_ucTile[0] == ucTile && m_TileSet[i].m_ucFlag == ACTION_TRIPLET)
			{
				// ������Ϊ��(��)
                m_TileSet[i].m_ucTile[3] = ucTile;
				m_TileSet[i].m_ucFlag = ACTION_QUADRUPLET_PATCH;
				return TRUE;
			}
		}

		return FALSE;
	}

	/**
	*  @brief �������
	*/	
	void CMJSet::ReleaseAll()
	{
		m_nCurrentLength = 0;

		TILE_SET tileSet;
		memset(&tileSet, 0, sizeof(tileSet));

		for (int i = 0; i < MAX_SET_TILES; i++)
		{
			m_TileSet[i] = tileSet;

			// ��⸽����Ϣ�Ƿ���Ҫ�ͷ��ڴ�
			if (m_pParam[i] != NULL)
			{
				delete m_pParam[i];
				m_pParam[i] = NULL;
			}
		}
	}

	/**
	*  @brief ���õ�ǰ����
	*  @param[in]  nCurLength ����
	*/	
	void CMJSet::CurrentLength( const int& nCurLength )
	{
		if (nCurLength > MAX_SET_TILES)
		{
			return ;
		}

		m_nCurrentLength = nCurLength;
	}

	/**
	*  @brief ���������еĸ�����Ϣ
	*  @param[in]  nPos Ҫ�趨��λ��
	*  @param[in]  pParam Ҫ���õ����Ƹ�����Ϣ
	*  @return �ɹ�TRUE, ʧ��FALSE
	*/	
	BOOL CMJSet::SetParam( int nPos, void * pParam )
	{
		if (nPos < 0 || nPos >= MAX_SET_TILES)
		{
			return FALSE;
		}

		if (m_pParam[nPos] != NULL)
		{
			delete m_pParam[nPos];
			m_pParam[nPos] = NULL;
		}

		m_pParam[nPos] = pParam;

		return TRUE;
	}

	/**
	*  @brief ��ȡ���������õĸ�����Ϣ
	*  @param[in]  nPos Ҫ��ȡ��λ��
	*  @return ʧ�ܷ���NULL
	*/	
	void * CMJSet::GetParam( int nPos)
	{
		if (nPos < 0 || nPos >= MAX_SET_TILES)
		{
			return NULL;
		}

		return m_pParam[nPos];
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// CMJGive ///
	CMJGive::CMJGive()
	{
		m_nCurrentLength = 0;
        memset(&m_TileSet, 0 , sizeof(m_TileSet));
	}

	CMJGive::~CMJGive()
	{
		m_nCurrentLength = 0;
	}

	/**
	*  @brief ���ó�������
	*  @sa CMJGive
	*/	
	void CMJGive::SetTile( int nPos, TILE ucTile )
	{
		if (nPos < 0 || nPos >= m_nCurrentLength || nPos >= MAX_GIVE_TILES
			|| ucTile >= TILE_END)
		{
			return;
		}

		m_ucTile[nPos] = ucTile;
	}

	/**
	*  @brief ��ȡ��������
	*  @sa CMJGive
	*/	
	TILE CMJGive::GetTile( int nPos )
	{
		if (nPos < 0 || nPos >= m_nCurrentLength)
		{
			return TILE_BEGIN;
		}

		return m_ucTile[nPos];
	}

    /** 
    *  @brief ����Xλ�õ�������Ϣ
    *  @sa CMJGive
    */
    void CMJGive::SetBlock(int nPos, TILE_SET &tileSet)
    {
        if (nPos < 0 || nPos >= m_nCurrentLength)
        {
            return ;
        }

        m_TileSet[nPos] = tileSet;
    }

    /** 
    *  @brief ��ȡXλ�õ�������Ϣ
    *  @sa CMJGive
    */
    void CMJGive::GetBlock(int nPos, TILE_SET &tileSet)
    {
        if (nPos < 0 || nPos >= m_nCurrentLength)
        {
            memset(&tileSet, 0, sizeof(tileSet));
            return ;
        }

        tileSet = m_TileSet[nPos];
    }

    /**
	*  @brief ���һ����
	*  @param[in]  ucTile ��ӵ�������
	*  @param[in]  nNbr ��ӵ�����
	*  @param[in]  bIncrease �Ƿ�Ҫ��������
	*  @return  ������õ�λ�ó����Ƶ�ʵ����󳤶ȣ��������õ������ݳ������ֵTILE_END�������ʧ��
	*/
	BOOL CMJGive::AddTile( TILE ucTile, int nNbr, int bIncrease)
	{
		if (m_nCurrentLength + nNbr > MAX_GIVE_TILES
			|| ucTile >= TILE_END)
		{
			return FALSE;
		}

		if (bIncrease)
		{
			if (ucTile >= TILE_CHAR_1 && ucTile <= TILE_CHAR_9)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_CHAR_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BAMBOO_1 && ucTile <= TILE_BAMBOO_9)      // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAMBOO_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BALL_1 && ucTile <= TILE_BALL_9)          // Ͳ
			{
				if (ucTile + nNbr - 1 > TILE_BALL_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_EAST && ucTile <= TILE_BAI)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAI)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_FLOWER_CHUN && ucTile <= TILE_FLOWER_JU)  // ��
			{
				if (ucTile + nNbr - 1 > TILE_FLOWER_JU)
				{
					return FALSE;
				}
			}

			// �����Ӧ��������
			for (int i = 0; i < nNbr; i++)
			{
				m_ucTile[m_nCurrentLength + i] = ucTile + i;
			}

			m_nCurrentLength += nNbr;
		}
		else
		{
			if (nNbr > 4)
			{
				return FALSE;
			}

			// ���nNbr����ͬ����
			for (int i = 0; i < nNbr; i++)
			{
				m_ucTile[m_nCurrentLength + i] = ucTile;
			}

			m_nCurrentLength += nNbr;
		}

		return TRUE;
	}

	/**
	*  @brief ɾ��һ������
	*  @param[in]  ucTile ָ��Ҫɾ������
	*  @param[in]  nNbr ɾ��������
	*  @param[in]  bIncrease �Ƿ�Ҫ����ɾ��
	*  @return  ���û�������ƾͻ᷵��FALSE
	*  @warning ɾ������������Ƶ�˳��
	*/
	BOOL CMJGive::DelTile( TILE ucTile, int nNbr, int bIncrease)
	{
		if (m_nCurrentLength - nNbr < 0)
		{
			return FALSE;
		}

		if (bIncrease)
		{
			if (ucTile >= TILE_CHAR_1 && ucTile <= TILE_CHAR_9)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_CHAR_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BAMBOO_1 && ucTile <= TILE_BAMBOO_9)      // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAMBOO_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BALL_1 && ucTile <= TILE_BALL_9)          // Ͳ
			{
				if (ucTile + nNbr - 1 > TILE_BALL_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_EAST && ucTile <= TILE_BAI)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAI)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_FLOWER_CHUN && ucTile <= TILE_FLOWER_JU)  // ��
			{
				if (ucTile + nNbr - 1 > TILE_FLOWER_JU)
				{
					return FALSE;
				}
			}

			CMJGive mjGive;
			for (int i = 0; i < nNbr; i++)
			{
				mjGive.AddTile(ucTile + i, 1, FALSE);
			}

			return DelTiles(mjGive);
		}
		else
		{
			TILE ucTileArray[MAX_GIVE_TILES] = {0};
			int nCount = 0;

			for (int i = 0, j = 0; i < m_nCurrentLength; i++)
			{
				if (m_ucTile[i] == ucTile && nCount < nNbr)
				{
					nCount++;
				}
				else
				{
					ucTileArray[j] = m_ucTile[i];
					j++;
				}
			}

			if (nCount > 0)
			{
				memcpy(m_ucTile, ucTileArray, sizeof(m_ucTile));
				m_nCurrentLength -= nCount;

				return TRUE;
			}
		}

		return FALSE;
	}

	/**
	*  @brief ���һ�����
	*  @sa CMJHand
	*/
	BOOL CMJGive::AddTiles( CMJGive &tiles )
	{
		int nLenTiles = tiles.CurrentLength();

		if (m_nCurrentLength + nLenTiles > MAX_GIVE_TILES)
		{
			return FALSE;
		}

		for (int i = 0; i < nLenTiles; i++)
		{
			m_ucTile[m_nCurrentLength + i] = tiles.GetTile(i);
		}

		m_nCurrentLength += nLenTiles;

		return TRUE;
	}

	/**
	*  @brief ɾ��һ���������
	*  @sa CMJHand
	*/
	BOOL CMJGive::DelTiles( CMJGive &tiles )
	{
		TILE tmp;
		BYTE ucTile[MAX_GIVE_TILES] = {0};
		int i, j, nDelCount = 0;
		int nLenTiles = tiles.CurrentLength();

		for (i = 0; i < nLenTiles; i++)
		{
			tmp = tiles.GetTile(i);
			for (j = 0; j < m_nCurrentLength; j++)
			{
				if (tmp == m_ucTile[j] && 0 == ucTile[j])
				{
					ucTile[j] = 1;
					nDelCount++;
					break;
				}
			}
		}

		for (i = 0, j = 0; i < m_nCurrentLength; i++)
		{
			if (0 == ucTile[i])
			{
				m_ucTile[j] = m_ucTile[i];
				j++;
			}
		}

		m_nCurrentLength -= nDelCount;

		return TRUE;
	}

	/**
	*  @brief �Ƿ����ָ������
	*  @param[in]  ucTile    ָ������
	*  @param[in]  nNbr      ����������
	*  @param[in]  bIncrease �Ƿ���������߿ɲ�������Ϊ9��������һ��˳�ӣ����Դ������nNbr = 3, bIncrease = TRUE
	*  @return �������ָ����������,����TRUE,���򷵻�FALSE
	*/	
	BOOL CMJGive::IsHave( TILE ucTile, int nNbr, int bIncrease)
	{
		if (ucTile > TILE_END)
		{
			return FALSE;
		}

		if (bIncrease)
		{
			if (ucTile >= TILE_CHAR_1 && ucTile <= TILE_CHAR_9)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_CHAR_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BAMBOO_1 && ucTile <= TILE_BAMBOO_9)      // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAMBOO_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BALL_1 && ucTile <= TILE_BALL_9)          // Ͳ
			{
				if (ucTile + nNbr - 1 > TILE_BALL_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_EAST && ucTile <= TILE_BAI)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAI)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_FLOWER_CHUN && ucTile <= TILE_FLOWER_JU)  // ��
			{
				if (ucTile + nNbr - 1 > TILE_FLOWER_JU)
				{
					return FALSE;
				}
			}

			BOOL bFind;
			int i, j;
			for (i = 0; i < nNbr; i++)
			{
				bFind = FALSE;

				for (j = 0; j < m_nCurrentLength; j++)
				{
					if (m_ucTile[j] == ucTile + i)
					{
						bFind = TRUE;
						break;
					}
				}

				if (!bFind)
				{
					return FALSE;
				}
			}

			return TRUE;
		}
		else
		{
			if (nNbr > 4)
			{
				return FALSE;
			}

			int nCount = 0;
			for (int i = 0; i < m_nCurrentLength; i++)
			{
				if (m_ucTile[i] == ucTile)
				{
					nCount++;

					if (nCount == nNbr)
					{
						return TRUE;
					}
				}
			}
		}

		return FALSE;
	}

	/**
	*  @brief �������
	*/	
	void CMJGive::ReleaseAll()
	{
		m_nCurrentLength = 0;

        memset(&m_TileSet, 0 , sizeof(m_TileSet));

		for (int i = 0; i < MAX_GIVE_TILES; i++)
		{
			m_ucTile[i] = TILE_BEGIN;
		}
	}

	/**
	*  @brief ���ó��Ƶĳ���
	*  @sa CMJHand
	*/
	void CMJGive::CurrentLength( const int& nCurLength )
	{
		if (nCurLength > MAX_GIVE_TILES)
		{
			return ;
		}

		m_nCurrentLength = nCurLength;
	}

	/**
	*  @brief ����ָ��2��λ���ϵ�������
	*  @sa CMJHand
	*/	
	BOOL CMJGive::Swap( int nPos1, int nPos2 )
	{
		if (nPos1 < 0 || nPos2 < 0
			|| nPos1 >= m_nCurrentLength || nPos2 >= m_nCurrentLength
			|| nPos1 == nPos2)
		{
			return FALSE;
		}

		TILE tempTile;
		tempTile = m_ucTile[nPos1];
		m_ucTile[nPos1] = m_ucTile[nPos2];
		m_ucTile[nPos2] = tempTile;

		return TRUE;
	}

	/**
	*  @brief ��ȡ����������
	*  @param[in] nGiveTiles ������
	*  @return  �Ƴ���
	*/	
	int CMJGive::GetAllTile( int nGiveTiles[] )
	{
		memset(nGiveTiles, 0, sizeof(nGiveTiles));

		for (int i = 0; i < m_nCurrentLength; i++)
		{
			nGiveTiles[i] = m_ucTile[i];
		}

		return m_nCurrentLength;
	}

	///// CMJWall ////

	CMJWall::CMJWall()
	{
		m_nCurrentLength = 0;
		m_nStartPos = 0;
		m_nEndPos = 0;
	}

	CMJWall::~CMJWall()
	{
		m_nCurrentLength = 0;
	}

	/**
	*  @brief �������
	*/	
	void CMJWall::ReleaseAll()
	{
		m_nCurrentLength = 0;
		m_nStartPos = 0;
		m_nEndPos = 0;

		for (int i = 0; i < MAX_TOTAL_TILES; i++)
		{
			m_ucTile[i] = TILE_BEGIN;
		}
	}

	/**
	*  @brief �������
	*  @param[in]  nLength Ҫ���ĳ���
	*  @param[in]  ucClear �Ƿ�Ҫȫ������ָ������,Ϊ0��ʾ�Զ���˳�����
	*
	*  ���ucClear��Ĭ��Ϊ0�ͻὫָ�����ȵ��ư���,��,Ͳ,��,����˳�����144��,����ǰ���Ȼ�����Ϊָ����nLength
	*/	
	void CMJWall::FillTiles( int nLength, BYTE ucClear)
	{
		ReleaseAll();

		int i, j;
		int nCurrentLength = nLength;

		if (0 == nLength || nLength > MAX_TOTAL_TILES)
		{
			// ������
			nCurrentLength = MAX_TOTAL_TILES;
		}

		if (ucClear != 0)
		{
			// ���ָ������
			for (i = 0; i < nCurrentLength; i++)
			{
				AddTile(ucClear);
			}
		}
		else 
		{
			// �Զ���˳�����
			// ��
			for (i = 0; i < 9; i++)
			{
				for (j = 0; j < 4; j++)
				{
					AddTile(TILE_CHAR_1 + i);

					if (m_nCurrentLength >= nCurrentLength)
					{
						return ;
					}
				}
			}

			// ��
			for (i = 0; i < 9; i++)
			{
				for (j = 0; j < 4; j++)
				{
					AddTile(TILE_BAMBOO_1 + i);

					if (m_nCurrentLength >= nCurrentLength)
					{
						return ;
					}
				}
			}

			// Ͳ
			for (i = 0; i < 9; i++)
			{
				for (j = 0; j < 4; j++)
				{
					AddTile(TILE_BALL_1 + i);

					if (m_nCurrentLength >= nCurrentLength)
					{
						return ;
					}
				}
			}

			// �������ƶ��������з���
			for (i = 0; i < 7; i++)
			{
				for (j = 0; j < 4; j++)
				{
					AddTile(TILE_EAST + i);

					if (m_nCurrentLength >= nCurrentLength)
					{
						return ;
					}
				}
			}

			// ���ӻ��ƴ����ﶬ÷�����
			for (i = 0; i < 8; i++)
			{
				AddTile(TILE_FLOWER_CHUN + i);

				if (m_nCurrentLength >= nCurrentLength)
				{
					return ;
				}
			}
		}	
	}

	/**
	*  @brief ϴ��
	*
	*  ���ҵ�ǰ���ȵ��Ƶ�˳��
	*/
	void CMJWall::RandomTiles()
	{
		int nIndex = 0;
		TILE tmp;

		/*time_t rawtime;
		time (&rawtime);
		srand((GULONG)rawtime);*/
	    //srand(GetTickCount());

        srand((unsigned)time(NULL));

		for (int i = 0; i < m_nCurrentLength; i++)
		{
			nIndex = rand()%m_nCurrentLength;
			tmp = m_ucTile[i];
			m_ucTile[i] = m_ucTile[nIndex];
			m_ucTile[nIndex] = tmp;
		}
	}

	/**
	*  @brief ����ָ��2��λ�õ���
	*  @sa CMJHand
	*/
	BOOL CMJWall::Swap( int nPos1, int nPos2 )
	{
		if (nPos1 < 0 || nPos2 < 0
			|| nPos1 >= m_nCurrentLength || nPos2 >= m_nCurrentLength
			|| nPos1 == nPos2)
		{
			return FALSE;
		}

		TILE tempTile;
		tempTile = m_ucTile[nPos1];
		m_ucTile[nPos1] = m_ucTile[nPos2];
		m_ucTile[nPos2] = tempTile;

		return TRUE;
	}

	/**
	*  @brief ���һ����
	*  @param[in]  ucTile ��ӵ�������
	*  @param[in]  nNbr ��ӵ�����
	*  @param[in]  bIncrease �Ƿ�Ҫ��������
	*  @return  ������õ�λ�ó����Ƶ�ʵ����󳤶ȣ��������õ������ݳ������ֵTILE_END�������ʧ��
	*/
	BOOL CMJWall::AddTile( TILE ucTile, int nNbr, int bIncrease)
	{
		if (m_nCurrentLength + nNbr > MAX_TOTAL_TILES
			|| ucTile >= TILE_END)
		{
			return FALSE;
		}

		if (bIncrease)
		{
			if (ucTile >= TILE_CHAR_1 && ucTile <= TILE_CHAR_9)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_CHAR_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BAMBOO_1 && ucTile <= TILE_BAMBOO_9)      // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAMBOO_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BALL_1 && ucTile <= TILE_BALL_9)          // Ͳ
			{
				if (ucTile + nNbr - 1 > TILE_BALL_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_EAST && ucTile <= TILE_BAI)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAI)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_FLOWER_CHUN && ucTile <= TILE_FLOWER_JU)  // ��
			{
				if (ucTile + nNbr - 1 > TILE_FLOWER_JU)
				{
					return FALSE;
				}
			}

			// �����Ӧ��������
			for (int i = 0; i < nNbr; i++)
			{
				m_ucTile[m_nCurrentLength + i] = ucTile + i;
			}

			m_nCurrentLength += nNbr;
		}
		else
		{
			if (nNbr > 4)
			{
				return FALSE;
			}

			// ���nNbr����ͬ����
			for (int i = 0; i < nNbr; i++)
			{
				m_ucTile[m_nCurrentLength + i] = ucTile;
			}

			m_nCurrentLength += nNbr;
		}

		return TRUE;
	}

	/**
	*  @brief ɾ��һ������
	*  @param[in]  ucTile ָ��Ҫɾ������
	*  @param[in]  nNbr ɾ��������
	*  @param[in]  bIncrease �Ƿ�Ҫ����ɾ��
	*  @return  ���û�������ƾͻ᷵��FALSE
	*  @warning ɾ������������Ƶ�˳��
	*/
	BOOL CMJWall::DelTile( TILE ucTile, int nNbr, int bIncrease)
	{
		if (m_nCurrentLength - nNbr < 0)
		{
			return FALSE;
		}

		if (bIncrease)
		{
			if (ucTile >= TILE_CHAR_1 && ucTile <= TILE_CHAR_9)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_CHAR_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BAMBOO_1 && ucTile <= TILE_BAMBOO_9)      // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAMBOO_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BALL_1 && ucTile <= TILE_BALL_9)          // Ͳ
			{
				if (ucTile + nNbr - 1 > TILE_BALL_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_EAST && ucTile <= TILE_BAI)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAI)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_FLOWER_CHUN && ucTile <= TILE_FLOWER_JU)  // ��
			{
				if (ucTile + nNbr - 1 > TILE_FLOWER_JU)
				{
					return FALSE;
				}
			}

			CMJWall mjWall;
			for (int i = 0; i < nNbr; i++)
			{
				mjWall.AddTile(ucTile + i, 1, FALSE);
			}

			return DelTiles(mjWall);
		}
		else
		{
			TILE ucTileArray[MAX_TOTAL_TILES] = {0};
			int nCount = 0;

			for (int i = 0, j = 0; i < m_nCurrentLength; i++)
			{
				if (m_ucTile[i] == ucTile && nCount < nNbr)
				{
					nCount++;
				}
				else
				{
					ucTileArray[j] = m_ucTile[i];
					j++;
				}
			}

			if (nCount > 0)
			{
				memcpy(m_ucTile, ucTileArray, sizeof(m_ucTile));
				m_nCurrentLength -= nCount;

				return TRUE;
			}
		}

		return FALSE;
	}

	/**
	*  @brief ���һ����
	*  @sa CMJHand
	*/	
	BOOL CMJWall::AddTiles( CMJWall &tiles )
	{
		int nLenTiles = tiles.CurrentLength();

		if (m_nCurrentLength + nLenTiles > MAX_TOTAL_TILES)
		{
			return FALSE;
		}

		for (int i = 0; i < nLenTiles; i++)
		{
			m_ucTile[m_nCurrentLength + i] = tiles.GetTile(i);
		}

		m_nCurrentLength += nLenTiles;

		return TRUE;
	}

	/**
	*  @brief ɾ��һ����
	*  @sa CMJHand
	*/	
	BOOL CMJWall::DelTiles( CMJWall &tiles )
	{
		TILE tmp;
		BYTE ucTile[MAX_TOTAL_TILES] = {0};
		int i, j, nDelCount = 0;
		int nLenTiles = tiles.CurrentLength();

		for (i = 0; i < nLenTiles; i++)
		{
			tmp = tiles.GetTile(i);
			for (j = 0; j < m_nCurrentLength; j++)
			{
				if (tmp == m_ucTile[j] && 0 == ucTile[j])
				{
					ucTile[j] = 1;
					nDelCount++;
					break;
				}
			}
		}

		for (i = 0, j = 0; i < m_nCurrentLength; i++)
		{
			if (0 == ucTile[i])
			{
				m_ucTile[j] = m_ucTile[i];
				j++;
			}
		}

		m_nCurrentLength -= nDelCount;

		return TRUE;
	}

	/**
	*  @brief �趨ָ��λ���ϵ���
	*  @sa CMJHand
	*/	
	void CMJWall::SetTile( int nPos, TILE ucTile )
	{
		if (nPos < 0 || nPos >= m_nCurrentLength || nPos >= MAX_TOTAL_TILES
			/*|| ucTile >= TILE_END*/)
		{
			return;
		}

		m_ucTile[nPos] = ucTile;
	}

	/**
	*  @brief ��ȡָ��λ���ϵ���
	*  @sa CMJHand
	*/	
	TILE CMJWall::GetTile( int nPos )
	{
		if (nPos < 0 || nPos >= m_nCurrentLength)
		{
			return TILE_BEGIN;
		}

		return m_ucTile[nPos];
	}

	/**
	*  @brief �Ƿ����ָ������
	*  @param[in]  ucTile    ָ������
	*  @param[in]  nNbr      ����������
	*  @param[in]  bIncrease �Ƿ���������߿ɲ�������Ϊ9��������һ��˳�ӣ����Դ������nNbr = 3, bIncrease = TRUE
	*  @return �������ָ����������,����TRUE,���򷵻�FALSE
	*/	
	BOOL CMJWall::IsHave( TILE ucTile, int nNbr, int bIncrease)
	{
		if (ucTile > TILE_END)
		{
			return FALSE;
		}

		if (bIncrease)
		{
			if (ucTile >= TILE_CHAR_1 && ucTile <= TILE_CHAR_9)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_CHAR_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BAMBOO_1 && ucTile <= TILE_BAMBOO_9)      // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAMBOO_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_BALL_1 && ucTile <= TILE_BALL_9)          // Ͳ
			{
				if (ucTile + nNbr - 1 > TILE_BALL_9)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_EAST && ucTile <= TILE_BAI)               // ��
			{
				if (ucTile + nNbr - 1 > TILE_BAI)
				{
					return FALSE;
				}
			}
			else if (ucTile >= TILE_FLOWER_CHUN && ucTile <= TILE_FLOWER_JU)  // ��
			{
				if (ucTile + nNbr - 1 > TILE_FLOWER_JU)
				{
					return FALSE;
				}
			}

			BOOL bFind;
			int i, j;
			for (i = 0; i < nNbr; i++)
			{
				bFind = FALSE;

				for (j = 0; j < m_nCurrentLength; j++)
				{
					if (m_ucTile[j] == ucTile + i)
					{
						bFind = TRUE;
						break;
					}
				}

				if (!bFind)
				{
					return FALSE;
				}
			}

			return TRUE;
		}
		else
		{
			if (nNbr > 4)
			{
				return FALSE;
			}

			int nCount = 0;
			for (int i = 0; i < m_nCurrentLength; i++)
			{
				if (m_ucTile[i] == ucTile)
				{
					nCount++;

					if (nCount == nNbr)
					{
						return TRUE;
					}
				}
			}
		}

		return FALSE;
	}

	/**
	*  @brief �������һ����
	*  @sa CMJHand
	*/	
	TILE CMJWall::PopTile()
	{
#ifdef WINDOWS_LOGIC
		// �ͻ���, ץ��ʹ��

		int nPos = m_nStartPos;
		TILE tile = m_ucTile[nPos];
		SetTile(nPos, -1);

		m_nStartPos++;

		if (m_nStartPos >= m_nCurrentLength)
		{
			// ���������0�ӹ�
			m_nStartPos = m_nStartPos - m_nCurrentLength;
		}

		if (tile == 255)
		{
			// ���λ�õ��ƿ��ܱ���(��)������, ������һ�����õ���
			for (BYTE i = 0; i < MAX_TOTAL_TILES; i++)
			{
				tile = m_ucTile[m_nStartPos];
				SetTile(m_nStartPos, -1);
				m_nStartPos++;

				if (m_nStartPos >= m_nCurrentLength)
				{
					// ���������0�ӹ�
					m_nStartPos = m_nStartPos - m_nCurrentLength;
				}

				if (tile != 255)
				{
					break;
				}
			}
		}

		return nPos;

#else
		// �����ʹ��
		//if (m_nCurrentLength <= 0)
		//{
		//	return TILE_BEGIN;
		//}

		//m_nCurrentLength--;

		//TILE tile = m_ucTile[m_nCurrentLength];

		//m_ucTile[m_nCurrentLength] = TILE_BEGIN;
		TILE tile = m_ucTile[m_nStartPos];

		int nPos = m_nStartPos;
		SetTile(nPos, -1);

		m_nStartPos++;

		if (m_nStartPos >= m_nCurrentLength)
		{
			// ���������0�ӹ�
			m_nStartPos = m_nStartPos - m_nCurrentLength;
		}

		if (tile == TILE_BEGIN || tile == 255)
		{
			_DOUT2("dxhout-Server: PopTile(), ��ֵλ��: %d, ��ֵ����: %d", tile, nPos);

			// ���λ�õ��ƿ��ܱ���(��)������, ������һ�����õ���
			for (BYTE i = 0; i < MAX_TOTAL_TILES; i++)
			{
				tile = m_ucTile[m_nStartPos];
				SetTile(m_nStartPos, -1);
				m_nStartPos++;
				
				if (m_nStartPos >= m_nCurrentLength)
				{
					// ���������0�ӹ�
					m_nStartPos = m_nStartPos - m_nCurrentLength;
				}

				if (tile != TILE_BEGIN && tile != 255)
				{
					break;
				}
			}
		}

		return tile;
#endif
	}

    /**
	*  @brief ������ǰһ����
	*  @param[in] nPos ���Ƶ�λ��
	*  @sa CMJWall
	*/	
    TILE CMJWall::PopFrontTile(int nPos)
    {
#ifdef WINDOWS_LOGIC
		// �ͻ���, ����ʹ��
		if (nPos > 1)
		{
			// ָ��������λ�õ���
			nPos = GetEndPos(nPos);

			// ָ������λ�õ���
			SetTile(nPos, -1);
		}
		else
		{
			nPos = m_nEndPos;
			SetTile(nPos, -1);
			m_nEndPos--;

			if (m_nEndPos < 0)
			{
				// 0����������ӹ�
				m_nEndPos = m_nCurrentLength - abs(m_nEndPos);
			}
		}

        return nPos;
#else
		// �����ʹ��
		//if (m_nCurrentLength <= 0)
		//{
		//	return TILE_BEGIN;
		//}

		//// ȡ��ǰһ����
		//TILE tile = m_ucTile[0];

		//// ɾ��������
		//DelTile(tile);

		//return tile;

		int nSrcPos = nPos;
		if (nPos > 1)
		{
			// ָ��������λ�õ���
			nSrcPos = nPos;
			nPos = GetEndPos(nSrcPos);
		}
		else
		{
			nPos = m_nEndPos;
			m_nEndPos--;

			if (m_nEndPos < 0)
			{
				// 0����������ӹ�
				m_nEndPos = m_nCurrentLength - abs(m_nEndPos);
			}
		}

		TILE tile = m_ucTile[nPos];

		// ָ������λ�õ���
		SetTile(nPos, -1);

		_DOUT3("dxhout-Server: ���� PopFrontTile(%d), ��ֵλ��: %d, ��ֵ: %d", nSrcPos, nPos, tile);

		return tile;
#endif
    }

	/**
	*  @brief ���ó�ǽ����
	*  @sa CMJHand
	*/
	void CMJWall::CurrentLength( const int &nCurLength )
	{
		if (nCurLength > MAX_TOTAL_TILES)
		{
			return ;
		}

		m_nCurrentLength = nCurLength;
	}

	/**
	*  @brief ���ó�ǽ����
	*  @sa CMJHand
	*/
	int CMJWall::CurrentLength()
	{
		int nLen = 0;
		for (BYTE i = 0; i < m_nCurrentLength; i++)
		{
			if (m_ucTile[i] == 255 || m_ucTile[i] == TILE_BEGIN)
			{
				continue;
			}

			nLen++;
		}

		return nLen;
	}

	/**
	*  @brief ����ץ���벹�Ƶ�λ��
	*  @param[in] nStartPos ץ��λ��
	*  @param[in] nEndPos   ����λ��
	*  @sa CMJWall
	*/	
	void CMJWall::SetPos(int nStartPos, int nEndPos)
	{
		SetStartPos(nStartPos);
		SetEndPos(nEndPos);
	}

	/**
	*  @brief ����ץ��λ��
	*  @sa CMJWall
	*/	
	void CMJWall::SetStartPos(int nPos)
	{
		if (nPos < 0)
		{
			return ;  // û�������˵Ŀ���
		}

		if (nPos >= m_nCurrentLength)
		{
			// ���������0�ӹ�
			m_nStartPos = nPos - m_nCurrentLength;
		}
		else
		{
			m_nStartPos = nPos;
		}
	}

	/**
	*  @brief ���ò���λ��
	*  @sa CMJWall
	*/	
	void CMJWall::SetEndPos(int nPos)
	{
		if (nPos >= m_nCurrentLength)
		{
			return ; // û����ǰ�ߵĿ���
		}

		if (nPos < 0)
		{
			// 0����������ӹ�
			m_nEndPos = m_nCurrentLength - abs(nPos);
		}
		else
		{
			m_nEndPos = nPos;
		}
	}

	/**
	*  @brief ��ȡץ��λ��
	*  @sa CMJWall
	*/	
	int CMJWall::GetStartPos()
	{
		return m_nStartPos;
	}

	/**
	 *  @brief ���ò���λ��
	 *  @param[in] nOffset ��ȡ���Ƶ�ƫ��λ��
	 *  @sa CMJWall
	 */	
	int CMJWall::GetEndPos(int nOffset)
	{
		if (nOffset == 0)
		{
			return m_nEndPos;
		}

		int nPos, nCount = 0;

		if (m_nEndPos % 2 == 0)
		{
			// ����һ��
			for (int i = 0; i < MAX_TOTAL_TILES; i += 2)
			{
				nPos = m_nEndPos - i;
				if (nPos < 0)
				{
					// 0����������ӹ�
					nPos = m_nCurrentLength - abs(nPos);
				}

				if (m_ucTile[nPos] != 255 || m_ucTile[nPos + 1] != 255)
				{
					nCount++;
				}

				if (nCount == nOffset)
				{
					if (m_ucTile[nPos] == 255)
					{
						return nPos + 1;   // ������������Ʊ�ץ����ץ����������
					}

					return nPos;
				}
			}
		}
		else
		{
			// ����һ��
			for (int i = 0; i < MAX_TOTAL_TILES; i += 2)
			{
				nPos = m_nEndPos - i;
				if (nPos < 0)
				{
					// 0����������ӹ�
					nPos = m_nCurrentLength - abs(nPos);
				}

				if (m_ucTile[nPos] != 255 || m_ucTile[nPos - 1] != 255)
				{
					nCount++;
				}

				if (nCount == nOffset)
				{
					if (m_ucTile[nPos - 1] != 255)
					{
						return nPos - 1;   // ����ץ����������
					}

					return nPos;
				}
			}
		}

		return m_nEndPos;
	}
}