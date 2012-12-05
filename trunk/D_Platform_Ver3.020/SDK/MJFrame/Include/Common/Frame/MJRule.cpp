#include "MJRule.h"

namespace MyGame
{
    CMJRule::CMJRule()
    {

    }

    CMJRule::~CMJRule()
    {

    }

    /**
    *  @brief �ж��ܲ���������Ƶ�����
    *  @param[in]  mjTilesNoGod ȥ���������Ƶ�����
    *  @param[in]  mjSet ��ҵ�����
    *  @param[in]  nLaiziCount ����������
    *  @return  ������Ժ�����TRUE,���򷵻�FALSE
    *
    *  �����Ƿ�
    */
    BOOL CMJRule::CheckWin( CMJHand mjTilesNoGod, CMJSet mjSet, int nLaiziCount )
    {
        if (mjTilesNoGod.CurrentLength() <= 0)
        {
            if (nLaiziCount > 0)
            {
                return TRUE;  // �ܺ�, ���ƿ��ܶ���������
            }
            
            return FALSE;
        }

        // 7��
        if ( GetMJSetting().nCheck7dui )
        {
            if ( TRUE == CheckWinAllPair( mjTilesNoGod, nLaiziCount ) )
            {
                return TRUE;
            }
        }

        // 131
        if ( GetMJSetting().nCheck131 )
        {
            if ( TRUE == CheckWin131( mjTilesNoGod, nLaiziCount ) )
            {
                return TRUE;
            }
        }

        // �ܷ������
        if ( GetMJSetting().nCheckLualv )
        {
            if ( TRUE == CheckWinAllGreen( mjTilesNoGod, mjSet ) )
            {
                return TRUE;
            }
        }

        // �ܷ���ҷ�
        if ( GetMJSetting().nCHeckLuafeng )
        {
            if ( TRUE == CheckWinAllWind( mjTilesNoGod, mjSet ) )
            {
                return TRUE;
            }
        }

        // �����
        if ( GetMJSetting().nCheckZuhelong )
        {
            if ( TRUE == CheckWinZuhelong( mjTilesNoGod, nLaiziCount ) )
            {
                return TRUE;
            }
        }

        // ȫ����
        if ( GetMJSetting().nCheckQuanbukao )
        {
            if ( TRUE == CheckWinQuanbukao( mjTilesNoGod, nLaiziCount ) )
            {
                return TRUE;
            }
        }

        // �ܷ���ͨ�ĺ�
        if ( TRUE == CheckWinNormal( mjTilesNoGod, nLaiziCount ) )
        {
            return TRUE;
        }

        return FALSE;
    }

    /**
    *  @brief �Ƿ��ܺ�
    *  @param[in]  mjTiles ��ҵ�����`
    *  @param[in]  mjSet ��ҵ�����
    *  @param[in]  nLaiziAdd ���ӵ�����������
    *  @param[in]  bCheckGod �Ƿ���������
    *  @return  ������Ժ�����TRUE,���򷵻�FALSE
    *
    *  ���Ƿ�������,���жϺ��Ƶ����ĺ���
    */
    BOOL CMJRule::IsWin( CMJHand mjTiles, CMJSet mjSet, int nLaiziAdd, int bCheckGod)
    {
        int nLaiziCount = 0;
        CMJHand mjTilesNoGod;
        CMJHand mjTilesGod;

        if ( bCheckGod )
        {
            nLaiziCount = GetTilesNoGod( mjTiles, mjTilesNoGod, mjTilesGod );
        }
        else
        {
            mjTilesNoGod.AddTiles( mjTiles );
        }

        nLaiziCount += nLaiziAdd;

        if ( FALSE == CheckWin( mjTilesNoGod, mjSet, nLaiziCount ) )
        {
            return FALSE;
        }

        if ( 0 == CheckNeedFan( mjTilesNoGod, mjTilesGod, mjSet, nLaiziCount, GetMJSetting().nMinFan ) ) // ������ж��·�
        {
            return FALSE;
        }

        return TRUE;
    }

    /**
    *  @brief �Ƿ�����
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  mjSet ��ҵ�����
    *  @param[out]  mjGive ����������������ƿ��Ա����ȥ���Ǽ���
    *  @return  �������������TRUE,���򷵻�FALSE
    */
    BOOL CMJRule::IsTing( CMJHand mjTiles, CMJSet mjSet, CMJHand& mjGive )
    {        
        if (mjTiles.CurrentLength() % 3 == 2) // ��2��˵��Ҫȥ��һ���ж�
        {
            mjGive.ReleaseAll();
            TILE ucTile;
            CMJHand mjHand;

            BOOL bTileFalg[TILE_END];
            memset(bTileFalg, 0, sizeof(bTileFalg));

            for (int  i = 0; i < mjTiles.CurrentLength(); i++)
            {
                ucTile = mjTiles.GetTile(i);

                if (bTileFalg[ucTile])
                {
                    // ��ͬ�����Ѿ��жϹ�
                    continue ;
                }

                bTileFalg[ucTile] = TRUE;

                mjHand = mjTiles;

                mjHand.DelTile(ucTile);

                if (IsWin(mjHand, mjSet, 1))
                {
                    // ���ܺ�, ��֤�������ƿ��Դ��ȥ
                    mjGive.AddTile(ucTile);
                }
            }

            if (mjGive.CurrentLength() > 0)
            {
                return TRUE;
            }
        }

        return FALSE;
    }

    /**
    *  @brief �Ƿ�����,����ȡ����ʲô
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  mjSet ��ҵ�����
    *  @param[out]  mjTing ������������ؿ��Ժ�����
    *  @return  �������������TRUE,���򷵻�FALSE
    *
    *  ��IsTing���������ڣ�IsTing��mjTiles�ǰ�����1�Ų��ܺ����ơ��������mjTiles���Ѿ������˵�����
    */	
    BOOL CMJRule::GetTingTiles( CMJHand mjTiles, CMJSet mjSet, CMJHand& mjTing )
    {
        /*��IsTing��������Ҫ���ڣ�IsTing�ǻ�ȡ���Դ�����ƣ��������ǿ��Ժ����ơ��˺�����Ҫ��ʹ�õص�����ѡ�����Ƶ�ʱ�򣬻�ȡ������ƺ���Ժ����ļ����ơ�
        ������ʵ�ַ������Ǳ������е��ƣ��ռ������п��Ժ����ƣ��ڿ��Ժ��ܶ��Ƶ�ʱ�����ܻ���Ӱ�졣�������������Ż���
        �������������û����һɫ���ƣ��������ƣ�������Ͳ�����Ͳ���ȥ������һɫ���ơ��������Ż��и�ȱ�㣬
        ���ǳ�������ͻ�������ֽ��޵ĺ��ƹ���ᵼ�²��ֿɺ��ƽ����©�������磨�ҽ�������258���ƶ����Ժ�����*/

        mjTing.ReleaseAll();

        // �������������ֻ�ɫ���ƲŻ�ȥ������һɫ����
        BOOL bTileGenre[6] = { 0 };

        int i, nGenre;

        for (i = 0; i < mjTiles.CurrentLength(); i++)
        {
            nGenre = mjTiles.GetTileGenre(mjTiles.GetTile(i));
            bTileGenre[nGenre] = TRUE;
        }

        for (i = 0; i < TILE_FLOWER_CHUN; i++)
        {
            nGenre = mjTiles.GetTileGenre( i );

            //if (bTileGenre[nGenre] || m_TilesGod.IsHave(i))   bTileGenre[] ȫ�������ܸ���
            { 
                CMJHand mjTemp;

                mjTemp.AddTiles(mjTiles);

                if (mjTiles.GetTileGenre( i ) <= TILE_GENRE_WIND)
                {
                    mjTemp.AddTile( i );

                    if (IsWin(mjTemp, mjSet))
                    {
                        mjTing.AddTile(i);
                    }
                }
            }
        }

        if (mjTing.CurrentLength() > 0)
        {
            return TRUE;
        }

        return FALSE;
    }

    /**
    *  @brief �Ƿ��ܺ�ָ����
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  mjSet ��ҵ�����
    *  @param[in]  ucTile ָ������
    *  @return  ������Ժ�����TRUE,���򷵻�FALSE
    */
    BOOL CMJRule::IsCanWin( CMJHand mjTiles, CMJSet mjSet, TILE ucTile )
    {
        mjTiles.AddTile(ucTile);

        return IsWin(mjTiles, mjSet);
    }

    /**
    *  @brief �Ƿ��ܳ���
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  ucTile ָ��Ҫ�Ե���
    *  @param[in]  nChairGive ���Ƶ����
    *  @param[in]  nChairCheck �жϳ��Ƶ����
    *  @param[in]  nPlayerNbr �ܵ���Ϸ����(�����жϳԵ��ǲ����ϼ�)
    *  @return  ������ԳԷ���TRUE,���򷵻�FALSE
    */
    BOOL CMJRule::IsCanCollect( CMJHand& mjTiles, TILE ucTile, int nChairGive, int nChairCheck, int nPlayerNbr)
    {
        // �жϵ�ǰ��������Ƿ���ڳ�����ҵ���һ��
        if (((nChairGive + GetMJSetting().nGiveOrder) % nPlayerNbr) == nChairCheck)
        {
            // �жϳ���
            return mjTiles.IsCanCollect(ucTile, m_tagMJSetting.nCollateWind, m_tagMJSetting.nCollateArrow, m_tmpBlockSet);
        }

        return FALSE;
    }

    /**
    *  @brief �Ƿ�������
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  ucTile ָ��Ҫ������
    *  @return  �������������TRUE,���򷵻�FALSE
    */
    BOOL CMJRule::IsCanTriplet( CMJHand& mjTiles, TILE ucTile )
    {
        // �����������в��ܱ������ܣ�Ҫ�ɳ���
        if (GetMJSetting().nCanBlockLaizi == 0 && GetTilesGod().CurrentLength() > 0)
        {
            if (GetTilesGod().IsHave(ucTile))
            {
                return FALSE;
            }
        }

        if (mjTiles.IsHave(ucTile, 2))
        {
            BYTE byTile[4];
            byTile[0] = ucTile;
            byTile[1] = ucTile;
            byTile[2] = ucTile;
            byTile[3] = 0;
            m_tmpBlockSet.AddSet(byTile, ACTION_TRIPLET, 0);
            return TRUE;
        }

        return FALSE;
    }

    /**
    *  @brief �ܷ��ָ������
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  ucTile ָ��Ҫ�ܵ���
    *  @return  ������Ըܷ���TRUE,���򷵻�FALSE
    */	
    BOOL CMJRule::IsCanQuadruplet( CMJHand& mjTiles, TILE ucTile )
    {
        if (mjTiles.IsHave(ucTile, 3))
        {
            BYTE byTile[4];
            byTile[0] = ucTile;
            byTile[1] = ucTile;
            byTile[2] = ucTile;
            byTile[3] = ucTile;
            m_tmpBlockSet.AddSet(byTile, ACTION_QUADRUPLET_REVEALED, 0);
            return TRUE;
        }

        return FALSE;
    }

    /**
    *  @brief �����Ƿ����ƿɸ�
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  mjSet ��ҵ�����
    *  @return  ������Ըܷ���TRUE,���򷵻�FALSE
    */	
    BOOL CMJRule::IsCanQuadrupletSelf( CMJHand& mjTiles, CMJSet &mjSet )
    {
        int i;
        BYTE byTile[4];
        BOOL bIsGang = FALSE;
        for (i = 0; i < mjSet.CurrentLength(); i++)
        { 
            TILE_SET tagSet;

            mjSet.GetSet( i, tagSet );

            if (ACTION_TRIPLET == tagSet.m_ucFlag && mjTiles.IsHave(tagSet.m_ucTile[0]))
            {
                // �����������в��ܱ������ܣ�Ҫ�ɳ���
                if (GetMJSetting().nCanBlockLaizi == 0 && GetTilesGod().CurrentLength() > 0)
                {
                    if (GetTilesGod().IsHave(tagSet.m_ucTile[0]))
                    {
                        continue;
                    }
                }

                byTile[0] = tagSet.m_ucTile[0];
                byTile[1] = tagSet.m_ucTile[0];
                byTile[2] = tagSet.m_ucTile[0];
                byTile[3] = tagSet.m_ucTile[0];
                m_tmpBlockSet.AddSet(byTile, ACTION_QUADRUPLET_PATCH, 0);

                bIsGang = TRUE;
            }
        }

        // �Լ��Ƿ���4��
        BYTE byTempTile[39] = {0};
        for (i = 0; i < mjTiles.CurrentLength(); i++)
        {
            TILE ucTile = mjTiles.GetTile( i );

            // �����������в��ܱ������ܣ�Ҫ�ɳ���
            if (GetMJSetting().nCanBlockLaizi == 0 && GetTilesGod().CurrentLength() > 0)
            {
                if (GetTilesGod().IsHave(ucTile))
                {
                    continue;
                }
            }

            if (byTempTile[ucTile] == 0 && mjTiles.IsHave(ucTile, 4))
            {
                byTempTile[ucTile] = 1;

                byTile[0] = ucTile;
                byTile[1] = ucTile;
                byTile[2] = ucTile;
                byTile[3] = ucTile;
                m_tmpBlockSet.AddSet(byTile, ACTION_QUADRUPLET_CONCEALED, 0);

                bIsGang = TRUE;
            }
        }

        if (GetMJSetting().nDongNanXiBeiGang == 1)
        {
            // ������ж�: ����������
            if (mjTiles.IsHave(TILE_EAST) && mjTiles.IsHave(TILE_SOUTH) && mjTiles.IsHave(TILE_WEST) && mjTiles.IsHave(TILE_NORTH))
            {
                // ���������ܳ���
                byTile[0] = TILE_EAST;
                byTile[1] = TILE_SOUTH;
                byTile[2] = TILE_WEST;
                byTile[3] = TILE_NORTH;
                m_tmpBlockSet.AddSet(byTile, ACTION_QUADRUPLET_DNXB, 0);
                bIsGang = TRUE;
            }
        }

        if (GetMJSetting().nZhongFaBaiGang == 1)
        {
            // ������ж�: �з���+(�з�������һ��)��
            if (mjTiles.IsHave(TILE_ZHONG) && mjTiles.IsHave(TILE_FA) && mjTiles.IsHave(TILE_BAI))
            {
                byTile[0] = TILE_ZHONG;
                byTile[1] = TILE_FA;
                byTile[2] = TILE_BAI;

                if (mjTiles.IsHave(TILE_ZHONG, 2))
                {
                    byTile[3] = TILE_ZHONG;
                    m_tmpBlockSet.AddSet(byTile, ACTION_QUADRUPLET_ZFB, 0);
                    bIsGang = TRUE;
                }

                if (mjTiles.IsHave(TILE_FA, 2))
                {
                    byTile[3] = TILE_FA;
                    m_tmpBlockSet.AddSet(byTile, ACTION_QUADRUPLET_ZFB, 0);
                    bIsGang = TRUE;
                }

                if (mjTiles.IsHave(TILE_BAI, 2))
                {
                    byTile[3] = TILE_BAI;
                    m_tmpBlockSet.AddSet(byTile, ACTION_QUADRUPLET_ZFB, 0);
                    bIsGang = TRUE;
                }
            }
        }

        return bIsGang;
    }

    /**
    *  @brief ��ȡ���ٷ�
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  mjSet ��ҵ�����
    *  @param[in]  env �����ƵĻ�����Ϣ
    *  @return ���ط�����Ŀ
    */	
    int CMJRule::GetFanCount( CMJHand mjTiles, CMJSet mjSet, ENVIRONMENT env )
    {
        if ( NULL == m_pFanCount )
        {
            return 0;
        }

        // �㷬
        //env.byChair = 0;
        env.bySetCount[env.byChair] = mjSet.CurrentLength();

        int i = 0;
        for (i = 0; i < mjSet.CurrentLength(); i++)
        {
            TILE_SET tagSet;
            mjSet.GetSet(i, tagSet);
            env.tSet[env.byChair][i][0] = tagSet.m_ucFlag;
            env.tSet[env.byChair][i][1] = tagSet.m_ucTile[0];
            env.tSet[env.byChair][i][2] = tagSet.m_ucChair;
        }

        env.byHandCount[env.byChair] = mjTiles.CurrentLength();

        for (i = 0; i < mjTiles.CurrentLength();  i++)
        {
            env.tHand[env.byChair][i] = mjTiles.GetTile(i);
        }

        m_pFanCount->SetEnv(env);

        return m_pFanCount->GetFanCount();
    }

    /**
    *  @brief �ܷ���ͨ�ĺ�
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  nLaiziCount �����Ƶ�����
    *  @param[in]  bNoJiang û�н���
    *  @return ���������ͨ����,����TRUE,���򷵻�FALSE
    */
    BOOL CMJRule::CheckWinNormal(CMJHand mjTiles, int nLaiziCount, int bNoJiang)
    {
        if (mjTiles.CurrentLength() <= 0 || (mjTiles.CurrentLength() + nLaiziCount) % 3 != 2)
        {
            return FALSE;
        }

        BOOL bFail = FALSE;     // ��־������Ƿ�ʧ��
        int i, j, n, tmpTile, nLaiziLength;
        BYTE nCheckFlag = 0;    // Ϊ0ʱ��ʾ������������ȡһ�Խ���, ���ж������. 
        // Ϊ1ʱ��ʾ0��־�����ʧ��, ����ȡ����+һ���������һ�Խ���, ���ж������.
        BYTE nCheckCount = 0;

        CMJHand mjHand, mjLaizi;  // ����ת�������
        CMJSet  mjSet;

        // ��¼ÿ��������
        int nMjNum[TILE_END] = {0};

        // --------------
        // ����һЩ��ʱ���ݴ�ű���
        int nMjNumTemp[TILE_END];
        int nLaiZiTemp;
        CMJSet mjSetTemp;
        CMJHand mjLaiziTemp;
        // --------------

        BYTE byBlockTile[4];

        // �������ݽ���������������, ��ȡ����ʱ, �ӵ���->����->����->����˳����ȡ
        mjTiles.Sort(2);
RestFlag:
        if (0 == nCheckFlag)
        {
            nCheckCount = 2; // 0��־��������ȡһ�Խ���
        }
        else if (1 == nCheckFlag)
        {
            nCheckCount = 1; // 1��־��������ȡһ����+һ��������Ϊһ�Խ���
        }

        // ��forѭ����ִ��2��, 0��־һ��, 1��־һ��
        for (int nStart = 0; nStart < mjTiles.CurrentLength(); nStart++)
        {
            mjHand = mjTiles;           // ÿ�ж�һ��, �������ݻ�ԭ
            nLaiziLength = nLaiziCount; // ÿ�ж�һ��, ����������ԭ
			mjSet.ReleaseAll();
            tmpTile = mjHand.GetTile(nStart);

            // �Ȱ�һ�Խ��������
            if (mjHand.IsHave(tmpTile, nCheckCount))
            {
                mjHand.DelTile(tmpTile, nCheckCount);

                if (1 == nCheckCount)
                {
                    // 1��־
                    nLaiziLength -= 1;  // ȡһ��������Ϊ����
                }

                bFail = FALSE;
                memset(nMjNum, 0, sizeof(nMjNum));

                // �������
                for (i = 0; i < mjHand.CurrentLength(); i++)
                {
                    nMjNum[mjHand.GetTile(i)] += 1;  // �ۼ�������
                }

                // ------------------------- �����жϿ�ʼ ------------------------
                // �����Ƿ�ɳ�
                if (m_tagMJSetting.nCollateWind)    
                {
                    // ���Գ�, �ȼ����, �����˳��
                    // �������
                    for (i = TILE_EAST; i <= TILE_NORTH; i++)
                    {
                        if (nMjNum[i] >= 3)
                        {
                            nMjNum[i] -= 3;

                            // ���Ϊ����
                            byBlockTile[0] = i;
                            byBlockTile[1] = i;
                            byBlockTile[2] = i;
                            byBlockTile[3] = 0;
                            mjSet.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                        }
                    }

                    // ����˳��
                    BYTE nFengCount[4][2];

                    for (i = TILE_EAST; i <= TILE_NORTH; i++)
                    {
                        nFengCount[i-TILE_EAST][0] = i;            // �洢��ֵ
                        nFengCount[i-TILE_EAST][1] = nMjNum[i];    // �洢�Ƶ�����
                    }

                    while (1)
                    {
                        // ����ð������
                        for (i = 3; i >= 0; i--)
                        {
                            for (j = 0; j < i; j++)
                            {
                                if (nFengCount[j][1] < nFengCount[j + 1][1])
                                {
                                    tmpTile = nFengCount[j + 1][0];
                                    nFengCount[j + 1][0] = nFengCount[j][0];
                                    nFengCount[j][0] = tmpTile;

                                    tmpTile = nFengCount[j + 1][1];
                                    nFengCount[j + 1][1] = nFengCount[j][1];
                                    nFengCount[j][1] = tmpTile;
                                }
                            }
                        }

                        if (nFengCount[0][1] > 0)
                        {
                            if (nFengCount[0][1] > 0 && nFengCount[1][1] > 0 && nFengCount[2][1] > 0)
                            {
                                nFengCount[0][1] -= 1;
                                nFengCount[1][1] -= 1;
                                nFengCount[2][1] -= 1;

                                nMjNum[nFengCount[0][0]] -= 1;
                                nMjNum[nFengCount[1][0]] -= 1;
                                nMjNum[nFengCount[2][0]] -= 1;

                                // ���Ϊ˳��
                                byBlockTile[0] = nFengCount[0][0];
                                byBlockTile[1] = nFengCount[1][0];
                                byBlockTile[2] = nFengCount[2][0];
                                byBlockTile[3] = 0;

                                mjSet.AddSet(byBlockTile/*nFengCount[0][0]*/, ACTION_COLLECT, 0);
                            }
                            else
                            {
                                // ���˳���Ʋ�����, ��ʹ�������滻
                                if (nFengCount[0][1] == 1 && nFengCount[1][1] == 0 && nLaiziLength >= 2)
                                {
                                    nLaiziLength -= 2;
                                    nFengCount[0][1] = 0;
                                    nMjNum[nFengCount[0][0]] = 0;

                                    // �������滻�ɵ��Ƽ�¼, �Լ�����Ҫ��ӵ�mjLaizi��
                                    mjLaizi.AddTile(nFengCount[1][0]);
                                    mjLaizi.AddTile(nFengCount[2][0]);
                                }
                                else if (nFengCount[0][1] == 1 && nFengCount[1][1] == 1 && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1;
                                    nFengCount[0][1] = 0;
                                    nFengCount[1][1] = 0;
                                    nMjNum[nFengCount[0][0]] = 0;
                                    nMjNum[nFengCount[1][0]] = 0;

                                    // �������滻�ɵ��Ƽ�¼, �Լ�����Ҫ��ӵ�mjLaizi��
                                    mjLaizi.AddTile(nFengCount[2][0]);
                                }
                                else if (nFengCount[0][1] == 2 && nFengCount[1][1] == 0 && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1;
                                    nFengCount[0][1] = 0;
                                    nMjNum[nFengCount[0][0]] = 0;

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nFengCount[0][0]);
                                }
                                else if (nFengCount[0][1] == 2 && nFengCount[1][1] == 1 && nLaiziLength >= 3)
                                {
                                    nLaiziLength -= 3;
                                    nFengCount[0][1] = 0;
                                    nFengCount[1][1] = 0;
                                    nMjNum[nFengCount[0][0]] = 0;
                                    nMjNum[nFengCount[1][0]] = 0;

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nFengCount[0][0]);

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nFengCount[1][0], 2);
                                }
                                else if (nFengCount[0][1] == 2 && nFengCount[1][1] == 2 && nLaiziLength >= 2)
                                {
                                    nLaiziLength -= 2;
                                    nFengCount[0][1] = 0;
                                    nFengCount[1][1] = 0;
                                    nMjNum[nFengCount[0][0]] = 0;
                                    nMjNum[nFengCount[1][0]] = 0;

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nFengCount[0][0]);

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nFengCount[1][0]);
                                }
                                else
                                {
                                    //OutputDebugString("dxhout: ��� �������� �����������--��������ʧ��");
                                    // ƥ��ʧ��
                                    bFail = TRUE;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    // �����Գ�, ֻ�����
                    for (i = TILE_EAST; i <= TILE_NORTH; i++)
                    {
                        if (nMjNum[i] >= 3)
                        {
                            nMjNum[i] -= 3;

                            // ���Ϊ����
                            byBlockTile[0] = i;
                            byBlockTile[1] = i;
                            byBlockTile[2] = i;
                            byBlockTile[3] = 0;
                            mjSet.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                        }
                        else if (nMjNum[i] > 0)
                        {
                            // �������������, ��ʹ�������滻
                            if (nLaiziLength >= 3 - nMjNum[i])
                            {
                                nLaiziLength -= 3 - nMjNum[i];

                                // ���Ϊ����
                                mjLaizi.AddTile(i, 3 - nMjNum[i]);

                                nMjNum[i] = 0;
                            }
                            else
                            {
                                // ƥ��ʧ��
                                bFail = TRUE;
                                break;
                            }
                        }
                    }
                }

                if (bFail)
                {
                    // ���������ʧ��, ��������ѭ��
                    continue ;
                }
                // ------------------------- �����жϽ��� ------------------------

                // ------------------------ ��ͷ���жϿ�ʼ ------------------------
                // ��ͷ���Ƿ�ɳ�
                if (m_tagMJSetting.nCollateArrow)      
                {
                    // ���Գ�, �ȼ����, �����˳��
                    // �������
                    for (i = TILE_ZHONG; i <= TILE_BAI; i++)
                    {
                        if (nMjNum[i] >= 3)
                        {
                            nMjNum[i] -= 3;

                            // ���Ϊ����
                            byBlockTile[0] = i;
                            byBlockTile[1] = i;
                            byBlockTile[2] = i;
                            byBlockTile[3] = 0;
                            mjSet.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                        }
                    }

                    // ����˳��
                    BYTE nArrowCount[3][2];

                    for (i = TILE_ZHONG; i <= TILE_BAI; i++)
                    {
                        nArrowCount[i-TILE_ZHONG][0] = i;
                        nArrowCount[i-TILE_ZHONG][1] = nMjNum[i];
                    }

                    while (1)
                    {
                        // ����ð������
                        for (i = 2; i >= 0; i--)
                        {
                            for (j = 0; j < i; j++)
                            {
                                if (nArrowCount[j][1] < nArrowCount[j + 1][1])
                                {
                                    tmpTile = nArrowCount[j + 1][0];
                                    nArrowCount[j + 1][0] = nArrowCount[j][0];
                                    nArrowCount[j][0] = tmpTile;

                                    tmpTile = nArrowCount[j + 1][1];
                                    nArrowCount[j + 1][1] = nArrowCount[j][1];
                                    nArrowCount[j][1] = tmpTile;
                                }
                            }
                        }

                        if (nArrowCount[0][1] > 0)
                        {
                            if (nArrowCount[0][1] > 0 && nArrowCount[1][1] > 0 && nArrowCount[2][1] > 0)
                            {
                                nArrowCount[0][1] -= 1;
                                nArrowCount[1][1] -= 1;
                                nArrowCount[2][1] -= 1;
                                nMjNum[nArrowCount[0][0]] -= 1;
                                nMjNum[nArrowCount[1][0]] -= 1;
                                nMjNum[nArrowCount[2][0]] -= 1;

                                // ���Ϊ˳��
                                byBlockTile[0] = nArrowCount[0][0];
                                byBlockTile[1] = nArrowCount[1][0];
                                byBlockTile[2] = nArrowCount[2][0];
                                byBlockTile[3] = 0;

                                mjSet.AddSet(byBlockTile/*nArrowCount[0][0]*/, ACTION_COLLECT, 0);
                            }
                            else
                            {
                                // ���˳���Ʋ�����, ��ʹ�������滻
                                if (nArrowCount[0][1] == 1 && nArrowCount[1][1] == 0 && nLaiziLength >= 2)
                                {
                                    nLaiziLength -= 2;
                                    nArrowCount[0][1] = 0;
                                    nMjNum[nArrowCount[0][0]] = 0;

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nArrowCount[0][0], 2);
                                }
                                else if (nArrowCount[0][1] == 1 && nArrowCount[1][1] == 1 && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1;
                                    nArrowCount[0][1] = 0;
                                    nArrowCount[1][1] = 0;
                                    nMjNum[nArrowCount[0][0]] = 0;
                                    nMjNum[nArrowCount[1][0]] = 0;

                                    // ���Ϊ˳��
                                    mjLaizi.AddTile(nArrowCount[2][0]);
                                }
                                else if (nArrowCount[0][1] == 2 && nArrowCount[1][1] == 0 && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1;
                                    nArrowCount[0][1] = 0;
                                    nMjNum[nArrowCount[0][0]] = 0;

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nArrowCount[0][0]);
                                }
                                else if (nArrowCount[0][1] == 2 && nArrowCount[1][1] == 1 && nLaiziLength >= 3)
                                {
                                    nLaiziLength -= 3;
                                    nArrowCount[0][1] = 0;
                                    nArrowCount[1][1] = 0;
                                    nMjNum[nArrowCount[0][0]] = 0;
                                    nMjNum[nArrowCount[1][0]] = 0;

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nArrowCount[0][0]);

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nArrowCount[1][0], 2);
                                }
                                else if (nArrowCount[0][1] == 2 && nArrowCount[1][1] == 2 && nLaiziLength >= 2)
                                {
                                    nLaiziLength -= 2;
                                    nArrowCount[0][1] = 0;
                                    nArrowCount[1][1] = 0;
                                    nMjNum[nArrowCount[0][0]] = 0;
                                    nMjNum[nArrowCount[1][0]] = 0;

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nArrowCount[0][0]);

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nArrowCount[1][0]);
                                }
                                else
                                {
                                    //OutputDebugString("dxhout: ��� ��ͷ������ �����������--��ͷ������ʧ��");
                                    // ƥ��ʧ��
                                    bFail = TRUE;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    // �����Գ�, ֻ�����
                    for (i = TILE_ZHONG; i <= TILE_BAI; i++)
                    {
                        if (nMjNum[i] >= 3)
                        {
                            nMjNum[i] -= 3;

                            // ���Ϊ����
                            byBlockTile[0] = i;
                            byBlockTile[1] = i;
                            byBlockTile[2] = i;
                            byBlockTile[3] = 0;
                            mjSet.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                        }
                        else if (nMjNum[i] > 0)
                        {
                            // �������������, ��ʹ�������滻
                            if (nLaiziLength >= 3 - nMjNum[i])
                            {
                                nLaiziLength -= 3 - nMjNum[i];

                                // ���Ϊ����
                                mjLaizi.AddTile(i, 3 - nMjNum[i]);

                                nMjNum[i] = 0;
                            }
                            else
                            {
                                // ƥ��ʧ��
                                bFail = TRUE;
                                break;
                            }
                        }
                    }
                }

                if (bFail)
                {
                    // ��ͷ�������ʧ��, ��������ѭ��
                    continue ;
                }
                // ------------------------ ��ͷ���жϽ��� ------------------------

                // ----------------------- ����Ͳ���жϿ�ʼ ------------------------
                nLaiZiTemp = nLaiziLength;
                mjSetTemp = mjSet;
                mjLaiziTemp = mjLaizi;
                memcpy(nMjNumTemp, nMjNum, sizeof(nMjNumTemp));

                // ���жϲ�����, Ȼ���ٱ�˳��
                for (n = 0; n < 2; n++)
                {
                    bFail = FALSE;
                    nLaiziLength = nLaiZiTemp;
                    mjSet = mjSetTemp;
                    mjLaizi = mjLaiziTemp;
                    memcpy(nMjNum, nMjNumTemp, sizeof(nMjNum));

                    // �������
                    for (i = TILE_CHAR_1; i <= TILE_BALL_9; i++)
                    {
                        // 10��20������ || ����������0�߲��ж�
                        if ((i % 10) == 0 || nMjNum[i] <= 0)
                        {
                            continue ;
                        }

                        if (nMjNum[i] >= 3)
                        {
                            nMjNum[i] -= 3;

                            // ���Ϊ����
                            byBlockTile[0] = i;
                            byBlockTile[1] = i;
                            byBlockTile[2] = i;
                            byBlockTile[3] = 0;
                            mjSet.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                        }

ret:
                        // �ж�˳��
                        if (nMjNum[i] > 0)
                        {
                            tmpTile = i % 10;

                            if (tmpTile < 8)
                            {
                                // n==0 ����ӿ���, �����˳��
                                if (n == 0 && nMjNum[i] == 2 && nLaiziLength >= 1)
                                {   
                                    nLaiziLength -= 1;

                                    nMjNum[i] = 0;

                                    // ������ת�����Ƶ�ֵ����
                                    mjLaizi.AddTile(i);

                                    // ���Ϊ����
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i;
                                    byBlockTile[2] = i;
                                    byBlockTile[3] = 0;
                                    mjSet.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                                }
                                // �����2�ţ��Һ�һ�Ż�����û�ƣ����������ƻ�����ʹ��
                                else if (nMjNum[i] == 2 && (nMjNum[i+1] <= 0 || nMjNum[i+2] <= 0) && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1;

                                    nMjNum[i] = 0;

                                    // ������ת�����Ƶ�ֵ����
                                    mjLaizi.AddTile(i);

                                    // ���Ϊ����
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i;
                                    byBlockTile[2] = i;
                                    byBlockTile[3] = 0;
                                    mjSet.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                                }
                                // ���Ϊ˳��
                                else if (nMjNum[i+1] > 0 && nMjNum[i+2] > 0)
                                {
                                    nMjNum[i] -= 1;
                                    nMjNum[i+1] -= 1;
                                    nMjNum[i+2] -= 1;

                                    // ���Ϊ˳��
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i + 1;
                                    byBlockTile[2] = i + 2;
                                    byBlockTile[3] = 0;
                                    mjSet.AddSet(byBlockTile/*i*/, ACTION_COLLECT, 0);

                                    // nMjNum[i]���ܻ���һ����, ����ȥ�����ж�
                                    goto ret;
                                }
                                else if (nMjNum[i+1] > 0 && nMjNum[i+2] == 0 && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1;
                                    nMjNum[i] -= 1;
                                    nMjNum[i+1] -= 1;

                                    // ������ת�����Ƶ�ֵ����
                                    mjLaizi.AddTile(i+2);

                                    // ���Ϊ˳��
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i + 1;
                                    byBlockTile[2] = i + 2;
                                    byBlockTile[3] = 0;
                                    mjSet.AddSet(byBlockTile/*i*/, ACTION_COLLECT, 0);
                                }
                                else if (nMjNum[i+1] == 0 && nMjNum[i+2] > 0 && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1; 
                                    nMjNum[i] -= 1;
                                    nMjNum[i+2] -= 1;

                                    // ������ת�����Ƶ�ֵ����
                                    mjLaizi.AddTile(i+1);

                                    // ���Ϊ˳��
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i + 1;
                                    byBlockTile[2] = i + 2;
                                    byBlockTile[3] = 0;
                                    mjSet.AddSet(byBlockTile/*i*/, ACTION_COLLECT, 0);
                                }
                                else if (nLaiziLength >= 3 - nMjNum[i])
                                {
                                    // �������Ų�������˳��, ��������������ӱ�Ϊ����
                                    nLaiziLength -= 3 - nMjNum[i];

                                    // ������ת�����Ƶ�ֵ����
                                    mjLaizi.AddTile(i, 3 - nMjNum[i]);

                                    // ���Ϊ����
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i;
                                    byBlockTile[2] = i;
                                    byBlockTile[3] = 0;
                                    mjSet.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);

                                    nMjNum[i] = 0;
                                }
                                else
                                {
                                    // ƥ��ʧ��
                                    bFail = TRUE;
                                    break;
                                }
                            }
                            else if (8 == tmpTile)
                            {
                                if (nMjNum[i] == 1)
                                {
                                    if (nMjNum[i+1] > 0 && nLaiziLength >= 1)
                                    {
                                        nLaiziLength -= 1;

                                        nMjNum[i] -= 1;
                                        nMjNum[i+1] -= 1;

                                        // ������ת�����Ƶ�ֵ����
                                        mjLaizi.AddTile(i-1);

                                        // ���Ϊ˳��
                                        byBlockTile[0] = i - 1;
                                        byBlockTile[1] = i;
                                        byBlockTile[2] = i + 1;
                                        byBlockTile[3] = 0;
                                        mjSet.AddSet(byBlockTile/*i-1*/, ACTION_COLLECT, 0);
                                    }
                                    else if (nLaiziLength >= 2)
                                    {
                                        nLaiziLength -= 2;

                                        nMjNum[i] = 0;

                                        // ������ת�����Ƶ�ֵ����
                                        mjLaizi.AddTile(i, 2);

                                        // ���Ϊ����
                                        byBlockTile[0] = i;
                                        byBlockTile[1] = i;
                                        byBlockTile[2] = i;
                                        byBlockTile[3] = 0;
                                        mjSet.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                                    }
                                    else
                                    {
                                        // ʧ��
                                        bFail = TRUE;
                                        break;
                                    }
                                }
                                else if (nMjNum[i] == 2 && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1;

                                    nMjNum[i] = 0;

                                    // ������ת�����Ƶ�ֵ����
                                    mjLaizi.AddTile(i);

                                    // ���Ϊ����
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i;
                                    byBlockTile[2] = i;
                                    byBlockTile[3] = 0;
                                    mjSet.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                                }
                                else
                                {
                                    // ʧ��
                                    bFail = TRUE;
                                    break;
                                }
                            }
                            else if (9 == tmpTile)
                            {
                                // ���Ϊ����, ʹ�������滻
                                if (nLaiziLength >= 3 - nMjNum[i])
                                {
                                    nLaiziLength -= 3 - nMjNum[i];

                                    // ������ת�����Ƶ�ֵ����
                                    mjLaizi.AddTile(i, 3 - nMjNum[i]);

                                    // ���Ϊ����
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i;
                                    byBlockTile[2] = i;
                                    byBlockTile[3] = 0;
                                    mjSet.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);

                                    nMjNum[i] = 0;
                                }
                                else
                                {
                                    // ƥ��ʧ��
                                    bFail = TRUE;
                                    break;
                                }
                            }
                        }
                    } //for

                    if (!bFail)
                    {
                        break;
                    }

                } //for


                if (bFail)
                {
                    // ��Ͳ�������ʧ��, ��������ѭ��
                    continue ;
                }

                for (i = 0; i < TILE_END; i++)
                {
                    if (nMjNum[i] > 0)
                    {
                        // �����ƴ���, ����ʧ��
                        mjLaizi.ReleaseAll();

                        bFail = TRUE;
                    }
                }

                if (bFail)
                {
                    break;
                }

                return TRUE;
                // ----------------------- ����Ͳ���жϽ��� ------------------------
            }//if
        }//for

        if (0 == nCheckFlag)
        {
            nCheckFlag = 1;

            // �������жϱ�־1
            goto RestFlag;
        }

        return FALSE;
    }

    /**
    *  @brief �ܷ�7��
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  nLaiziCount �����Ƶ�����
    *  @return ������Ժ���,����TRUE,���򷵻�FALSE
    */
    BOOL CMJRule::CheckWinAllPair( CMJHand mjTiles, int nLaiziCount )
    {
        if (mjTiles.CurrentLength() + nLaiziCount != 14)
        {
            // ������������
            return FALSE;
        }

        CMJHand mjSingleTiles = mjTiles;   // ����
        CMJHand mjPairsTiles;              // ����
        int  nPairsCount = 0;           // ��¼��������

        TILE tmp;
        for (int i = 0; i < mjTiles.CurrentLength(); i++)
        {
            tmp = mjTiles.GetTile(i);

            // �ж��������Ƿ���ڶ���
            if (mjSingleTiles.IsHave(tmp, 2))
            {
                nPairsCount++;                  // ��������++
                mjPairsTiles.AddTile(tmp, 2);   // �Ѷ��Ƽӵ�����������
                mjSingleTiles.DelTile(tmp, 2);  // ɾ������, ʣ��ĵ���ȫ�������������
            }
        }

        if (7 == nPairsCount)
        {
            // �߶�
            return TRUE;
        }

        // ��ǰ�߶Բ�����, ʹ�����ӻ���
        while (nLaiziCount > 0)
        {
            if (mjSingleTiles.CurrentLength() > 0)
            {
                // �Ȼ�����
                nLaiziCount--;

                tmp = mjSingleTiles.GetTile(0);

                mjSingleTiles.DelTile(tmp);
                mjPairsTiles.AddTile(tmp, 2);
                nPairsCount++; 
            }
            else 
            {
                if (nLaiziCount >= 2)
                {
                    // û�е�����, ֱ���ö������ӻ���һ����
                    nLaiziCount -= 2;

                    mjPairsTiles.AddTile(TILE_CHAR_1, 2);

                    nPairsCount++; 
                }
                else
                {
                    // û�����ӻ���, ����ʧ��
                    return FALSE;
                }
            }

            if (7 == nPairsCount)
            {
                // �����߶�
                return TRUE;
            }
        }

        return FALSE;
    }

    /**
    *  @brief �ܷ��ʮ����
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  nLaiziCount �����Ƶ�����
    *  @return ������Ժ���,����TRUE,���򷵻�FALSE
    */
    BOOL CMJRule::CheckWin131( CMJHand mjTiles, int nLaiziCount )
    {
        if (mjTiles.CurrentLength() + nLaiziCount != 14)
        {
            // ������������
            return FALSE;
        }

        int i, have_num = 0;         // ��¼���е�������
        BOOL have_jiang = FALSE;      // ��¼�ǶԽ���

        CMJHand mjHand;

        // ���һ��ʮ��������Ϊ���ռ�
        mjHand.AddTile(TILE_CHAR_1);
        mjHand.AddTile(TILE_CHAR_9);
        mjHand.AddTile(TILE_BAMBOO_1);
        mjHand.AddTile(TILE_BAMBOO_9);
        mjHand.AddTile(TILE_BALL_1);
        mjHand.AddTile(TILE_BALL_9);
        mjHand.AddTile(TILE_EAST, 7, TRUE);

        // ������������
        for (i = 0; i < mjTiles.CurrentLength(); i++)
        {
            TILE tmp = mjTiles.GetTile(i);

            if (mjTiles.IsHave(tmp, 3))
            {
                // ��3�Ż�4����ͬ���Ʒ���
                return FALSE;
            }

            if (mjTiles.IsHave(tmp, 2))
            {
                // �ж϶����Ƿ�Ϊʮ���������һ����
                if (mjHand.IsHave(tmp))
                {
                    // ��־�н���
                    have_jiang = TRUE;
                }
                else 
                {
                    return FALSE;
                }
            }
        }

        if (!have_jiang)
        {
            // û��һ�Խ��ƣ����������滻��һ�Խ���.
            while (nLaiziCount > 0) 
            {
                nLaiziCount--;

                // �����ӻ�һ�Խ��Ƴ���
                mjTiles.AddTile(TILE_CHAR_1);

                if (mjTiles.IsHave(TILE_CHAR_1, 2))
                {
                    have_jiang = TRUE;
                    break;
                }
            }
        }

        if (!have_jiang)
        {
            // û�жԽ��ƣ�����
            return FALSE;
        }

        // ���Ƽ�1(�����ж���, ����ֻ��ⵥ��)
        have_num++;

        // �ж����� 1��9��1����9����1Ͳ��9Ͳ �Ѵ��ڵ�����
        for (i = 0; i < 3; i++)
        {
            if (mjTiles.IsHave(i * 10 + 1))
            {
                have_num++;
            }
            else
            {
                if (nLaiziCount > 0)
                {
                    nLaiziCount--;

                    // �������滻��
                    mjTiles.AddTile(i * 10 + 1);

                    have_num++;
                }
            }

            if (mjTiles.IsHave(i * 10 + 9))
            {
                have_num++;
            }
            else
            {
                if (nLaiziCount > 0)
                {
                    nLaiziCount--;

                    // �������滻��
                    mjTiles.AddTile(i * 10 + 9);

                    have_num++;
                }
            }
        }

        // �ж����ƶ��������з����Ѵ��ڵ�����
        for (i = 0; i < 7; i++)
        {
            if (mjTiles.IsHave(TILE_EAST + i)) 
            {
                have_num++;
            }
            else
            {
                if (nLaiziCount > 0)
                {
                    nLaiziCount--;

                    // �������滻��
                    mjTiles.AddTile(TILE_EAST + i);

                    have_num++;
                }
            }
        }

        // �ж��Ƿ����ʮ���۵�����
        if (have_num == 14)
        {
            return TRUE;
        }

        return FALSE;
    }

    /**
    *  @brief �ܷ������
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  mjSet   ��ҵ���������
    *  @return ������Ժ���,����TRUE,���򷵻�FALSE
    */
    BOOL CMJRule::CheckWinAllGreen( CMJHand mjTiles, CMJSet mjSet )
    {
        int i;
        TILE tmp;
        TILE_SET tileSet;

        for (i = 0; i < mjSet.CurrentLength(); i++)
        {
            mjSet.GetSet(i, tileSet);

            if (ACTION_COLLECT == tileSet.m_ucFlag)
            {
                //// �ж��Ƿ�Ե���234��(����Ϊ234��, ����)
                //if (COLLECT_RES_OOX == tileSet.m_ucChair && tileSet.m_ucTile != TILE_BAMBOO_2
                //	|| COLLECT_RES_OXO == tileSet.m_ucChair && tileSet.m_ucTile != TILE_BAMBOO_3
                //	|| COLLECT_RES_XOO == tileSet.m_ucChair && tileSet.m_ucTile != TILE_BAMBOO_4)
                //{
                //	// �ԵĲ���234, ������
                //	return FALSE;
                //} 

                // �ж��Ƿ�Ե���234��(����Ϊ234��, ����)
                for (BYTE j = 0; j < 3; j++)
                {
                    if (tileSet.m_ucTile[j] != TILE_BAMBOO_2 && tileSet.m_ucTile[j] != TILE_BAMBOO_3 && tileSet.m_ucTile[j] != TILE_BAMBOO_4)
                    {
                        // �ԵĲ���234, ������
                        return FALSE;
                    }
                }
            }
            else
            {
                // �жϸ����Ƿ���������
                if (tileSet.m_ucTile[0] != TILE_FA
                    && tileSet.m_ucTile[0] != TILE_BAMBOO_2
                    && tileSet.m_ucTile[0] != TILE_BAMBOO_3
                    && tileSet.m_ucTile[0] != TILE_BAMBOO_4
                    && tileSet.m_ucTile[0] != TILE_BAMBOO_6
                    && tileSet.m_ucTile[0] != TILE_BAMBOO_8)
                {
                    // ������
                    return FALSE;
                }
            }
        }

        // �ж������е�����
        for (i = 0; i < mjTiles.CurrentLength(); i++)
        {
            tmp = mjTiles.GetTile(i);

            if (tmp != TILE_FA
                && tmp != TILE_BAMBOO_2
                && tmp != TILE_BAMBOO_3
                && tmp != TILE_BAMBOO_4
                && tmp != TILE_BAMBOO_6
                && tmp != TILE_BAMBOO_8)
            {
                // ������
                return FALSE;
            }
        }

        return TRUE;
    }

    /**
    *  @brief �ܷ���ҷ�
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  mjSet   ��ҵ���������
    *  @return ������Ժ���,����TRUE,���򷵻�FALSE
    */	
    BOOL CMJRule::CheckWinAllWind( CMJHand mjTiles, CMJSet mjSet )
    {
        int i;
        TILE_SET tileSet;

        for (i = 0; i < mjSet.CurrentLength(); i++)
        {
            mjSet.GetSet(i, tileSet);

            if (tileSet.m_ucTile[0] < TILE_EAST || tileSet.m_ucTile[0] > TILE_BAI)
            {

                return FALSE;
            }
        }

        for (i = 0; i < mjTiles.CurrentLength(); i++)
        {
            if (mjTiles.GetTile(i) < TILE_EAST || mjTiles.GetTile(i) > TILE_BAI)
            {
                return FALSE;
            }
        }

        return TRUE;
    }

    /**
    *  @brief �ܷ������� - 3�ֻ�ɫ��147��258��369���ܴ�λ��������
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  nLaiziCount �����Ƶ�����
    *  @return ������Ժ���,����TRUE,���򷵻�FALSE
    */
    BOOL CMJRule::CheckWinZuhelong( CMJHand mjTiles, int nLaiziCount )
    {
        if (mjTiles.CurrentLength() < 11)
        {
            return FALSE;
        }

        CMJHand mjHand = mjTiles;
        int nLaiziLenght = nLaiziCount;
        BYTE i, j;
        bool bFlag = FALSE;

        // ��������
        BYTE byWinMode[6][3] = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2,}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};
        BYTE byNumber[9] = {1, 4, 7, 2, 5, 8, 3, 6, 9};

        for (i = 0; i < 6; i++)
        {
            mjHand = mjTiles;

            if (mjHand.IsHave(byWinMode[i][0] * 10 + 1) && mjHand.IsHave(byWinMode[i][0] * 10 + 4) && mjHand.IsHave(byWinMode[i][0] * 10 + 7)
             && mjHand.IsHave(byWinMode[i][1] * 10 + 2) && mjHand.IsHave(byWinMode[i][1] * 10 + 5) && mjHand.IsHave(byWinMode[i][1] * 10 + 8)
             && mjHand.IsHave(byWinMode[i][2] * 10 + 3) && mjHand.IsHave(byWinMode[i][2] * 10 + 6) && mjHand.IsHave(byWinMode[i][2] * 10 + 9))
            {
                mjHand.DelTile(byWinMode[i][0] * 10 + 1);
                mjHand.DelTile(byWinMode[i][0] * 10 + 4);
                mjHand.DelTile(byWinMode[i][0] * 10 + 7);
                mjHand.DelTile(byWinMode[i][1] * 10 + 2);
                mjHand.DelTile(byWinMode[i][1] * 10 + 5);
                mjHand.DelTile(byWinMode[i][1] * 10 + 8);
                mjHand.DelTile(byWinMode[i][2] * 10 + 3);
                mjHand.DelTile(byWinMode[i][2] * 10 + 6);
                mjHand.DelTile(byWinMode[i][2] * 10 + 9);

                if (CheckWinNormal(mjHand, nLaiziCount))
                {
                    return TRUE;
                }  
            }
        }

        if (nLaiziCount > 0)
        {
            for (i = 0; i < 6; i++)
            {
                mjHand = mjTiles;
                nLaiziLenght = nLaiziCount;
                bFlag = TRUE;

                for (j = 0; j < 9; j++)
                {
                    if (mjHand.IsHave(byWinMode[i][i / 3] * 10 + byNumber[j]))
                    {
                        mjHand.DelTile(byWinMode[i][i / 3] * 10 + byNumber[j]);
                    }
                    else if (nLaiziLenght > 0)
                    {
                        nLaiziLenght--;  // ��������
                    }
                    else
                    {
                        bFlag = FALSE;
                        break;  // ʧ��
                    }
                }

                if (bFlag && CheckWinNormal(mjHand, nLaiziCount))
                {
                    return TRUE;
                }  
            }
        }
     
        return FALSE; 
    }

    /**
    *  @brief �ܷ��ȫ���� - �ɵ���3�ֻ�ɫ147��258��369���ܴ�λ�������Ƽ����������з����е��κ�14������ɵĺ���
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  nLaiziCount �����Ƶ�����
    *  @return ������Ժ���,����TRUE,���򷵻�FALSE
    */	
    BOOL CMJRule::CheckWinQuanbukao( CMJHand mjTiles, int nLaiziCount )
    { 
        if (mjTiles.CurrentLength() < 11)
        {
            return FALSE;
        }

        CMJHand mjHand = mjTiles;
        CMJHand mjTemp; 
        int nLaiziLenght = nLaiziCount;
        BYTE i, j;

        for (i = 0; i < mjHand.CurrentLength(); i++)
        {
            if (mjHand.IsHave(mjHand.GetTile(i), 2))
            {
                return FALSE;   // ȫ���������ж�
            }
        }

        // ��������ȡ����
        for (i = 0; i < 7; i++)
        {
            if (mjHand.IsHave(TILE_EAST + i))
            {
                mjHand.DelTile(TILE_EAST + i);
            }
        }

        mjTemp = mjHand;

        // ��������
        BYTE byWinMode[6][3] = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2,}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};
        BYTE byNumber[9] = {1, 4, 7, 2, 5, 8, 3, 6, 9};

        for (i = 0; i < 6; i++)
        {
            mjHand = mjTemp;

            for (j = 0; j < 9; j++)
            {
                if (mjHand.IsHave(byWinMode[i][j / 3] * 10 + byNumber[j]))
                {
                    mjHand.DelTile(byWinMode[i][j / 3] * 10 + byNumber[j]);
                }
            }

            if (mjHand.CurrentLength() <= 0)
            {
                return TRUE;
            }
        }

        return FALSE;
    }

    /**
    *  @brief �����㷬ָ��
    *  @param[in]  pFanCount �������ָ��
    */	
    void CMJRule::SetFanCounter( CMJFanCounter* pFanCount )
    {
        m_pFanCount = pFanCount;
    }

    /**
    *  @brief ����������
    *  @return ����0
    */	
    int CMJRule::SetGodTiles()
    {
        return 0;
    }

    /**
    *  @brief �����������Ϣ
    */	
    void CMJRule::ClearGodTiles()
    {
        m_TilesGod.ReleaseAll();
    }

    /**
    *  @brief ������Ҳ���뵽������ȥ
    *  @param[out]  mjAll ���Ϻ����������
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[in]  mjSet ��ҵ�����
    */	
    void CMJRule::GetAllTiles( CMJHand &mjAll, CMJHand mjTiles, CMJSet mjSet )
    {
        TILE_SET tileSet;
        mjAll.ReleaseAll();
        mjAll.AddTiles(mjTiles);

        for (int i = 0; i < mjSet.CurrentLength(); i++)
        {
            mjSet.GetSet(i, tileSet);

            switch (tileSet.m_ucFlag)
            {
            case ACTION_COLLECT:    // ����
                {
                    /*for (int j = 0 - tileSet.m_ucChair; j < (3 - tileSet.m_ucChair); j++)
                    {
                    mjAll.AddTile(tileSet.m_ucTile + j);
                    }*/
                    for (int j = 0; j < 3; j++)
                    {
                        mjAll.AddTile(tileSet.m_ucTile[j]);
                    }
                }
                break;
            case ACTION_TRIPLET:              // ����
            case ACTION_QUADRUPLET_PATCH:     // ����
            case ACTION_QUADRUPLET_CONCEALED:
            case ACTION_QUADRUPLET_REVEALED:
                {
                    mjAll.AddTile(tileSet.m_ucTile[0], 3);
                }
                break;
            default:
                break;
            }  
        }
    }

    /**
    *  @brief ��ȡһ��ȡ���������Ƶ�����
    *  @param[in]  mjTiles ��ҵ�����
    *  @param[out]  mjTilesNoGod ȥ�������ƺ������
    *  @param[out]  mjTilesGod ȥ��������ʣ�µ�����
    *  @return ���������Ƶ�����
    */
    int CMJRule::GetTilesNoGod( CMJHand& mjTiles, CMJHand& mjTilesNoGod, CMJHand& mjTilesGod )
    {
        mjTilesNoGod.ReleaseAll();
        mjTilesGod.ReleaseAll();

        if (m_TilesGod.CurrentLength() <= 0)
        {
            mjTilesNoGod = mjTiles;
        }
        else
        {
            TILE tile;
            int nTileCount = 0;
            mjTilesNoGod = mjTiles;
            for (int i = 0; i < m_TilesGod.CurrentLength(); i++)
            {
                tile = m_TilesGod.GetTile(i);
                if (mjTilesNoGod.IsHave(tile))
                {
                    nTileCount = mjTilesNoGod.GetTileCount(tile);
                    mjTilesNoGod.DelTile(tile, nTileCount);
                    mjTilesGod.AddTile(tile, nTileCount);
                }
            }
        }

        return mjTilesGod.CurrentLength();
    }

    /**
    *  @brief �������Ƶ����������ʵ�ʵ���
    *  @param[out] mjTilesNoGod ȥ�������ƺ������
    *  @param[in]  mjSet ��ҵ�����
    *  @param[in]  nLaiziCount �����Ƶ�����
    *  @param[out] mjLaizi �滻�����
    *  @param[in]  mjLaiziTiles �����Ʊ������
    *  @return �������ļ��㷬��
    */
    int CMJRule::ConfigGodTiles( CMJHand &mjTilesNoGod, CMJSet mjSet, int nLaiziCount, CMJHand &mjLaizi, CMJHand mjLaiziTiles )
    {  
        // ע: �˺���Ŀ���CheckWinNormal����һ����, Ŀǰֻʵ���������������, ��û�в��������
        mjLaizi.ReleaseAll();

        BOOL bFail = FALSE;     // ��־������Ƿ�ʧ��
        int i, j, n, tmpTile, nLaiziLength;
        BYTE nCheckFlag = 0;    // Ϊ0ʱ��ʾ������������ȡһ�Խ���, ���ж������. 
        // Ϊ1ʱ��ʾ0��־�����ʧ��, ����ȡ����+һ���������һ�Խ���, ���ж������.
        BYTE nCheckCount = 0;

        CMJHand mjHand;
        CMJSet  mjSetEx;  // �˱�����ʱ�������е�����, δ����������
        TILE_SET tileSet;

        // --------------
        // ����һЩ��ʱ���ݴ�ű���
        int nMjNumTemp[49];
        int nLaiZiTemp;
        CMJSet mjSetTemp;
        CMJHand mjLaiziTemp;
        // --------------

        // ��¼ÿ��������
        int nMjNum[TILE_END] = {0};

        BYTE byBlockTile[4];

        // --------------------------------- �߶��ж� ----------------------------------
        if ( GetMJSetting().nCheck7dui )
        {
            CMJHand mjSingleTiles = mjTilesNoGod;   // ����
            int  nPairsCount = 0;                // ��¼��������

            for (i = 0; i < mjTilesNoGod.CurrentLength(); i++)
            {
                tmpTile = mjTilesNoGod.GetTile(i);

                // �ж��������Ƿ���ڶ���
                if (mjSingleTiles.IsHave(tmpTile, 2))
                {
                    nPairsCount++;						// ��������++
                    mjSingleTiles.DelTile(tmpTile, 2);  // ɾ������, ʣ��ĵ���ȫ�������������
                }
            }

            if (7 == nPairsCount)
            {
                // �߶�
                return TRUE;
            }

            nLaiziLength = nLaiziCount;

            // ��ǰ�߶Բ�����, ʹ�����ӻ���
            while (nLaiziLength > 0)
            {
                if (mjSingleTiles.CurrentLength() > 0)
                {
                    // �Ȼ�����
                    nLaiziLength--;

                    tmpTile = mjSingleTiles.GetTile(0);

                    mjSingleTiles.DelTile(tmpTile);
                    mjLaizi.AddTile(tmpTile);

                    nPairsCount++; 
                }
                else 
                {
                    if (nLaiziLength >= 2)
                    {
                        // û�е�����, ֱ���ö������ӻ���һ����
                        nLaiziLength -= 2;

                        mjLaizi.AddTile(TILE_CHAR_1, 2);

                        nPairsCount++; 
                    }
                    else
                    {
                        break;
                    }
                }

                if (7 == nPairsCount)
                {
                    // �����߶�
                    return TRUE;
                }
            }
        }
        // --------------------------------- �߶��жϽ��� ----------------------------------

        // --------------------------------- ʮ�����ж� ----------------------------------
        if ( GetMJSetting().nCheck131 )
        {
            int have_num = 0;            // ��¼���е�������
            BOOL have_jiang = FALSE;      // ��¼�ǶԽ���

            mjHand = mjTilesNoGod;

            CMJHand mjRefer;

            // ���һ��ʮ��������Ϊ���ռ�
            mjRefer.AddTile(TILE_CHAR_1);
            mjRefer.AddTile(TILE_CHAR_9);
            mjRefer.AddTile(TILE_BAMBOO_1);
            mjRefer.AddTile(TILE_BAMBOO_9);
            mjRefer.AddTile(TILE_BALL_1);
            mjRefer.AddTile(TILE_BALL_9);
            mjRefer.AddTile(TILE_EAST, 7, TRUE);

            // ������������
            for (i = 0; i < mjHand.CurrentLength(); i++)
            {
                TILE tmp = mjHand.GetTile(i);

                if (mjHand.IsHave(tmp, 3))
                {
                    // ��3�Ż�4����ͬ���Ʒ���
                    break;
                }

                if (mjHand.IsHave(tmp, 2))
                {
                    // �ж϶����Ƿ�Ϊʮ���������һ����
                    if (mjRefer.IsHave(tmp))
                    {
                        // ��־�н���
                        have_jiang = TRUE;
                    }
                    else 
                    {
                        break;
                    }
                }
            }

            nLaiziLength = nLaiziCount;
            mjLaizi.ReleaseAll();

            if (!have_jiang)
            {
                // û��һ�Խ��ƣ����������滻��һ�Խ���.
                while (nLaiziLength > 0) 
                {
                    nLaiziLength--;

                    // �����ӻ�һ�Խ��Ƴ���
                    mjLaizi.AddTile(TILE_CHAR_1);

                    mjHand.AddTile(TILE_CHAR_1);

                    if (mjHand.IsHave(TILE_CHAR_1, 2))
                    {
                        have_jiang = TRUE;
                        break;
                    }
                }
            }

            if (have_jiang)
            {
                // �жԽ��ƣ�����
                // ���Ƽ�1(�����ж���, ����ֻ��ⵥ��)
                have_num++;

                // �ж����� 1��9��1����9����1Ͳ��9Ͳ �Ѵ��ڵ�����
                for (i = 0; i < 3; i++)
                {
                    if (mjHand.IsHave(i * 10 + 1))
                    {
                        have_num++;
                    }
                    else
                    {
                        if (nLaiziLength > 0)
                        {
                            nLaiziLength--;

                            // �������滻��
                            mjLaizi.AddTile(i * 10 + 1);

                            have_num++;
                        }
                    }

                    if (mjHand.IsHave(i * 10 + 9))
                    {
                        have_num++;
                    }
                    else
                    {
                        if (nLaiziLength > 0)
                        {
                            nLaiziLength--;

                            // �������滻��
                            mjLaizi.AddTile(i * 10 + 9);

                            have_num++;
                        }
                    }
                }

                // �ж����ƶ��������з����Ѵ��ڵ�����
                for (i = 0; i < 7; i++)
                {
                    if (mjHand.IsHave(TILE_EAST + i)) 
                    {
                        have_num++;
                    }
                    else
                    {
                        if (nLaiziLength > 0)
                        {
                            nLaiziLength--;

                            // �������滻��
                            mjLaizi.AddTile(TILE_EAST + i);

                            have_num++;
                        }
                    }
                }

                // �ж��Ƿ����ʮ���۵�����
                if (have_num == 14)
                {
                    return TRUE;
                }
            }
        }
        // --------------------------------- ʮ�����жϽ��� ----------------------------------

        // --------------------------------- �ҷ��ж� ----------------------------------
        if ( GetMJSetting().nCHeckLuafeng )
        {
            BOOL bFeng = TRUE;
            for (i = 0; i < mjSet.CurrentLength(); i++)
            {
                mjSet.GetSet(i, tileSet);

                if (tileSet.m_ucTile[0] < TILE_EAST || tileSet.m_ucTile[0] > TILE_BAI)
                {
                    // ���Ƿ���
                    bFeng = FALSE;
                    break;
                }
            }

            for (i = 0; i < mjTilesNoGod.CurrentLength(); i++)
            {
                if (mjTilesNoGod.GetTile(i) < TILE_EAST || mjTilesNoGod.GetTile(i) > TILE_BAI)
                {
                    // ���Ƿ���
                    bFeng = FALSE;
                    break;
                }
            }

            if (bFeng)
            {
                nLaiziLength = nLaiziCount;
                mjLaizi.ReleaseAll();

                // ȫ���̷�
                while (nLaiziLength > 0)
                {
                    nLaiziLength--;
                    mjLaizi.AddTile(TILE_EAST);
                }

                // �ҷ�
                return TRUE;
            }
        }
        // --------------------------------- �ҷ��жϽ��� ----------------------------------

        // --------------------------------- �����ж� ----------------------------------
        if ( GetMJSetting().nCheckLualv )
        {
            BOOL bGreen = TRUE;
            for (i = 0; i < mjSet.CurrentLength(); i++)
            {
                mjSet.GetSet(i, tileSet);

                if (ACTION_COLLECT == tileSet.m_ucFlag)
                {
                    // �ж��Ƿ�Ե���234��(����Ϊ234��, ����)
                    //if (COLLECT_RES_OOX == tileSet.m_ucChair && tileSet.m_ucTile != TILE_BAMBOO_2
                    //	|| COLLECT_RES_OXO == tileSet.m_ucChair && tileSet.m_ucTile != TILE_BAMBOO_3
                    //	|| COLLECT_RES_XOO == tileSet.m_ucChair && tileSet.m_ucTile != TILE_BAMBOO_4)
                    //{
                    //	// �ԵĲ���234, ������
                    //	bGreen = FALSE;
                    //	break;
                    //} 

                    for (BYTE j = 0; j < 3; j++)
                    {
                        if (tileSet.m_ucTile[j] != TILE_BAMBOO_2 && tileSet.m_ucTile[j] != TILE_BAMBOO_3 && tileSet.m_ucTile[j] != TILE_BAMBOO_4)
                        {
                            // �ԵĲ���234, ������
                            bGreen = FALSE;
                            break;
                        }
                    }
                }
                else
                {
                    // �жϸ����Ƿ���������
                    if (tileSet.m_ucTile[0] != TILE_FA
                        && tileSet.m_ucTile[0]  != TILE_BAMBOO_2
                        && tileSet.m_ucTile[0]  != TILE_BAMBOO_3
                        && tileSet.m_ucTile[0]  != TILE_BAMBOO_4
                        && tileSet.m_ucTile[0]  != TILE_BAMBOO_6
                        && tileSet.m_ucTile[0]  != TILE_BAMBOO_8)
                    {
                        // ������
                        bGreen = FALSE;
                        break;
                    }
                }
            }

            // �ж������е�����
            for (i = 0; i < mjTilesNoGod.CurrentLength(); i++)
            {
                tmpTile = mjTilesNoGod.GetTile(i);

                if (tmpTile != TILE_FA
                    && tmpTile != TILE_BAMBOO_2
                    && tmpTile != TILE_BAMBOO_3
                    && tmpTile != TILE_BAMBOO_4
                    && tmpTile != TILE_BAMBOO_6
                    && tmpTile != TILE_BAMBOO_8)
                {
                    // ������
                    bGreen = FALSE;
                    break;
                }
            }

            if (bGreen)
            {
                nLaiziLength = nLaiziCount;
                mjLaizi.ReleaseAll();

                // ȫ������
                while (nLaiziLength > 0)
                {
                    nLaiziLength--;
                    mjLaizi.AddTile(TILE_BAMBOO_2);
                }

                // ����
                return TRUE;
            }
        }
        // --------------------------------- �����жϽ��� ----------------------------------

        // �������ݽ���������������, ��ȡ����ʱ, �ӵ���->����->����->����˳����ȡ
        mjTilesNoGod.Sort(2);

RestFlag:
        if (0 == nCheckFlag)
        {
            nCheckCount = 2; // 0��־��������ȡһ�Խ���
        }
        else if (1 == nCheckFlag)
        {
            nCheckCount = 1; // 1��־��������ȡһ����+һ��������Ϊһ�Խ���
        }

        // ��forѭ��ֻ��ִ�б�, 0��־һ��, 1��־һ��
        for (int nStart = 0; nStart < mjTilesNoGod.CurrentLength(); nStart++)
        {
            mjHand = mjTilesNoGod;      // ÿ�ж�һ��, �������ݻ�ԭ
            nLaiziLength = nLaiziCount; // ÿ�ж�һ��, ����������ԭ
            mjLaizi.ReleaseAll();
			mjSetEx.ReleaseAll();
            tmpTile = mjHand.GetTile(nStart);

            // �Ȱ�һ�Խ��������
            if (mjHand.IsHave(tmpTile, nCheckCount))
            {
                mjHand.DelTile(tmpTile, nCheckCount);

                if (1 == nCheckCount)
                {
                    // 1��־
                    nLaiziLength -= 1;  // ȡһ��������Ϊ����

                    // �������ӻ��ɵ���ֵ
                    mjLaizi.AddTile(tmpTile);
                }

                bFail = FALSE;
                memset(nMjNum, 0, sizeof(nMjNum));

                // �������
                for (i = 0; i < mjHand.CurrentLength(); i++)
                {
                    nMjNum[mjHand.GetTile(i)] += 1;  // �ۼ�������
                }

                // ------------------------- �����жϿ�ʼ ------------------------
                // �����Ƿ�ɳ�
                if (m_tagMJSetting.nCollateWind)    
                {
                    // ���Գ�, �ȼ����, �����˳��
                    // �������
                    for (i = TILE_EAST; i <= TILE_NORTH; i++)
                    {
                        if (nMjNum[i] >= 3)
                        {
                            nMjNum[i] -= 3;

                            // ���Ϊ����
                            byBlockTile[0] = i;
                            byBlockTile[1] = i;
                            byBlockTile[2] = i;
                            byBlockTile[3] = 0;
                            mjSetEx.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                        }
                    }

                    // ����˳��
                    BYTE nFengCount[4][2];

                    for (i = TILE_EAST; i <= TILE_NORTH; i++)
                    {
                        nFengCount[i-TILE_EAST][0] = i;            // �洢��ֵ
                        nFengCount[i-TILE_EAST][1] = nMjNum[i];    // �洢�Ƶ�����
                    }

                    while (1)
                    {
                        // ����ð������
                        for (i = 3; i >= 0; i--)
                        {
                            for (j = 0; j < i; j++)
                            {
                                if (nFengCount[j][1] < nFengCount[j + 1][1])
                                {
                                    tmpTile = nFengCount[j + 1][0];
                                    nFengCount[j + 1][0] = nFengCount[j][0];
                                    nFengCount[j][0] = tmpTile;

                                    tmpTile = nFengCount[j + 1][1];
                                    nFengCount[j + 1][1] = nFengCount[j][1];
                                    nFengCount[j][1] = tmpTile;
                                }
                            }
                        }

                        if (nFengCount[0][1] > 0)
                        {
                            if (nFengCount[0][1] > 0 && nFengCount[1][1] > 0 && nFengCount[2][1] > 0)
                            {
                                nFengCount[0][1] -= 1;
                                nFengCount[1][1] -= 1;
                                nFengCount[2][1] -= 1;

                                nMjNum[nFengCount[0][0]] -= 1;
                                nMjNum[nFengCount[1][0]] -= 1;
                                nMjNum[nFengCount[2][0]] -= 1;

                                // ���Ϊ˳��
                                byBlockTile[0] = nFengCount[0][0];
                                byBlockTile[1] = nFengCount[1][0];
                                byBlockTile[2] = nFengCount[2][0];
                                byBlockTile[3] = 0;

                                mjSetEx.AddSet(byBlockTile/*nFengCount[0][0]*/, ACTION_COLLECT, 0);
                            }
                            else
                            {
                                // ���˳���Ʋ�����, ��ʹ�������滻
                                if (nFengCount[0][1] == 1 && nFengCount[1][1] == 0 && nLaiziLength >= 2)
                                {
                                    nLaiziLength -= 2;
                                    nFengCount[0][1] = 0;
                                    nMjNum[nFengCount[0][0]] = 0;

                                    // �������滻�ɵ��Ƽ�¼, �Լ�����Ҫ��ӵ�mjLaizi��
                                    mjLaizi.AddTile(nFengCount[1][0]);
                                    mjLaizi.AddTile(nFengCount[2][0]);
                                }
                                else if (nFengCount[0][1] == 1 && nFengCount[1][1] == 1 && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1;
                                    nFengCount[0][1] = 0;
                                    nFengCount[1][1] = 0;
                                    nMjNum[nFengCount[0][0]] = 0;
                                    nMjNum[nFengCount[1][0]] = 0;

                                    // �������滻�ɵ��Ƽ�¼, �Լ�����Ҫ��ӵ�mjLaizi��
                                    mjLaizi.AddTile(nFengCount[2][0]);
                                }
                                else if (nFengCount[0][1] == 2 && nFengCount[1][1] == 0 && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1;
                                    nFengCount[0][1] = 0;
                                    nMjNum[nFengCount[0][0]] = 0;

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nFengCount[0][0]);
                                }
                                else if (nFengCount[0][1] == 2 && nFengCount[1][1] == 1 && nLaiziLength >= 3)
                                {
                                    nLaiziLength -= 3;
                                    nFengCount[0][1] = 0;
                                    nFengCount[1][1] = 0;
                                    nMjNum[nFengCount[0][0]] = 0;
                                    nMjNum[nFengCount[1][0]] = 0;

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nFengCount[0][0]);

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nFengCount[1][0], 2);
                                }
                                else if (nFengCount[0][1] == 2 && nFengCount[1][1] == 2 && nLaiziLength >= 2)
                                {
                                    nLaiziLength -= 2;
                                    nFengCount[0][1] = 0;
                                    nFengCount[1][1] = 0;
                                    nMjNum[nFengCount[0][0]] = 0;
                                    nMjNum[nFengCount[1][0]] = 0;

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nFengCount[0][0]);

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nFengCount[1][0]);
                                }
                                else
                                {
                                    //OutputDebugString("dxhout: ��� �������� �����������--��������ʧ��");
                                    // ƥ��ʧ��
                                    bFail = TRUE;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    // �����Գ�, ֻ�����
                    for (i = TILE_EAST; i <= TILE_NORTH; i++)
                    {
                        if (nMjNum[i] >= 3)
                        {
                            nMjNum[i] -= 3;

                            // ���Ϊ����
                            byBlockTile[0] = i;
                            byBlockTile[1] = i;
                            byBlockTile[2] = i;
                            byBlockTile[3] = 0;
                            mjSetEx.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                        }
                        else if (nMjNum[i] > 0)
                        {
                            // �������������, ��ʹ�������滻
                            if (nLaiziLength >= 3 - nMjNum[i])
                            {
                                nLaiziLength -= 3 - nMjNum[i];

                                // ���Ϊ����
                                mjLaizi.AddTile(i, 3 - nMjNum[i]);

                                nMjNum[i] = 0;
                            }
                            else
                            {
                                // ƥ��ʧ��
                                bFail = TRUE;
                                break;
                            }
                        }
                    }
                }

                if (bFail)
                {
                    // ���������ʧ��, ��������ѭ��
                    continue ;
                }
                // ------------------------- �����жϽ��� ------------------------

                // ------------------------ ��ͷ���жϿ�ʼ ------------------------
                // ��ͷ���Ƿ�ɳ�
                if (m_tagMJSetting.nCollateArrow)      
                {
                    // ���Գ�, �ȼ����, �����˳��
                    // �������
                    for (i = TILE_ZHONG; i <= TILE_BAI; i++)
                    {
                        if (nMjNum[i] >= 3)
                        {
                            nMjNum[i] -= 3;

                            // ���Ϊ����
                            byBlockTile[0] = i;
                            byBlockTile[1] = i;
                            byBlockTile[2] = i;
                            byBlockTile[3] = 0;
                            mjSetEx.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                        }
                    }

                    // ����˳��
                    BYTE nArrowCount[3][2];

                    for (i = TILE_ZHONG; i <= TILE_BAI; i++)
                    {
                        nArrowCount[i-TILE_ZHONG][0] = i;
                        nArrowCount[i-TILE_ZHONG][1] = nMjNum[i];
                    }

                    while (1)
                    {
                        // ����ð������
                        for (i = 2; i >= 0; i--)
                        {
                            for (j = 0; j < i; j++)
                            {
                                if (nArrowCount[j][1] < nArrowCount[j + 1][1])
                                {
                                    tmpTile = nArrowCount[j + 1][0];
                                    nArrowCount[j + 1][0] = nArrowCount[j][0];
                                    nArrowCount[j][0] = tmpTile;

                                    tmpTile = nArrowCount[j + 1][1];
                                    nArrowCount[j + 1][1] = nArrowCount[j][1];
                                    nArrowCount[j][1] = tmpTile;
                                }
                            }
                        }

                        if (nArrowCount[0][1] > 0)
                        {
                            if (nArrowCount[0][1] > 0 && nArrowCount[1][1] > 0 && nArrowCount[2][1] > 0)
                            {
                                nArrowCount[0][1] -= 1;
                                nArrowCount[1][1] -= 1;
                                nArrowCount[2][1] -= 1;

                                nMjNum[nArrowCount[0][0]] -= 1;
                                nMjNum[nArrowCount[1][0]] -= 1;
                                nMjNum[nArrowCount[2][0]] -= 1;

                                // ���Ϊ˳��
                                byBlockTile[0] = nArrowCount[0][0];
                                byBlockTile[1] = nArrowCount[1][0];
                                byBlockTile[2] = nArrowCount[2][0];
                                byBlockTile[3] = 0;

                                mjSetEx.AddSet(byBlockTile/*nArrowCount[0][0]*/, ACTION_COLLECT, 0);
                            }
                            else
                            {
                                // ���˳���Ʋ�����, ��ʹ�������滻
                                if (nArrowCount[0][1] == 1 && nArrowCount[1][1] == 0 && nLaiziLength >= 2)
                                {
                                    nLaiziLength -= 2;
                                    nArrowCount[0][1] = 0;
                                    nMjNum[nArrowCount[0][0]] = 0;

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nArrowCount[0][0], 2);
                                }
                                else if (nArrowCount[0][1] == 1 && nArrowCount[1][1] == 1 && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1;
                                    nArrowCount[0][1] = 0;
                                    nArrowCount[1][1] = 0;

                                    nMjNum[nArrowCount[0][0]] = 0;
                                    nMjNum[nArrowCount[1][0]] = 0;

                                    // ���Ϊ˳��
                                    mjLaizi.AddTile(nArrowCount[2][0]);
                                }
                                else if (nArrowCount[0][1] == 2 && nArrowCount[1][1] == 0 && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1;
                                    nArrowCount[0][1] = 0;

                                    nMjNum[nArrowCount[0][0]] = 0;

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nArrowCount[0][0]);
                                }
                                else if (nArrowCount[0][1] == 2 && nArrowCount[1][1] == 1 && nLaiziLength >= 3)
                                {
                                    nLaiziLength -= 3;
                                    nArrowCount[0][1] = 0;
                                    nArrowCount[1][1] = 0;

                                    nMjNum[nArrowCount[0][0]] = 0;
                                    nMjNum[nArrowCount[1][0]] = 0;

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nArrowCount[0][0]);

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nArrowCount[1][0], 2);
                                }
                                else if (nArrowCount[0][1] == 2 && nArrowCount[1][1] == 2 && nLaiziLength >= 2)
                                {
                                    nLaiziLength -= 2;
                                    nArrowCount[0][1] = 0;
                                    nArrowCount[1][1] = 0;

                                    nMjNum[nArrowCount[0][0]] = 0;
                                    nMjNum[nArrowCount[1][0]] = 0;

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nArrowCount[0][0]);

                                    // ���Ϊ����
                                    mjLaizi.AddTile(nArrowCount[1][0]);
                                }
                                else
                                {
                                    //OutputDebugString("dxhout: ��� ��ͷ������ �����������--��ͷ������ʧ��");
                                    // ƥ��ʧ��
                                    bFail = TRUE;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    // �����Գ�, ֻ�����
                    for (i = TILE_ZHONG; i <= TILE_BAI; i++)
                    {
                        if (nMjNum[i] >= 3)
                        {
                            nMjNum[i] -= 3;

                            // ���Ϊ����
                            byBlockTile[0] = i;
                            byBlockTile[1] = i;
                            byBlockTile[2] = i;
                            byBlockTile[3] = 0;
                            mjSetEx.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                        }
                        else if (nMjNum[i] > 0)
                        {
                            // �������������, ��ʹ�������滻
                            if (nLaiziLength >= 3 - nMjNum[i])
                            {
                                nLaiziLength -= 3 - nMjNum[i];

                                // ���Ϊ����
                                mjLaizi.AddTile(i, 3 - nMjNum[i]);

                                nMjNum[i] = 0;
                            }
                            else
                            {
                                // ƥ��ʧ��
                                bFail = TRUE;
                                break;
                            }
                        }
                    }
                }

                if (bFail)
                {
                    // ��ͷ�������ʧ��, ��������ѭ��
                    continue ;
                }
                // ------------------------ ��ͷ���жϽ��� ------------------------

                // ----------------------- ����Ͳ���жϿ�ʼ ------------------------
                nLaiZiTemp = nLaiziLength;
                mjSetTemp = mjSetEx;
                mjLaiziTemp = mjLaizi;
                memcpy(nMjNumTemp, nMjNum, sizeof(nMjNumTemp));

                // ���жϲ�����, Ȼ���ٱ�˳��
                for (n = 0; n < 2; n++)
                {
                    bFail = FALSE;
                    nLaiziLength = nLaiZiTemp;
                    mjSetEx = mjSetTemp;
                    mjLaizi = mjLaiziTemp;
                    memcpy(nMjNum, nMjNumTemp, sizeof(nMjNum));

                    // �������
                    for (i = TILE_CHAR_1; i <= TILE_BALL_9; i++)
                    {
                        // 10��20������ || ����������0�߲��ж�
                        if ((i % 10) == 0 || nMjNum[i] <= 0)
                        {
                            continue ;
                        }

                        if (nMjNum[i] >= 3)
                        {
                            nMjNum[i] -= 3;

                            // ���Ϊ����
                            byBlockTile[0] = i;
                            byBlockTile[1] = i;
                            byBlockTile[2] = i;
                            byBlockTile[3] = 0;
                            mjSetEx.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                        }

ret:
                        // �ж�˳��
                        if (nMjNum[i] > 0)
                        {
                            tmpTile = i % 10;

                            if (tmpTile < 8)
                            {
                                // n==0 ����ӿ���, �����˳��
                                if (n == 0 && nMjNum[i] == 2 && nLaiziLength >= 1)
                                {   
                                    nLaiziLength -= 1;

                                    nMjNum[i] = 0;

                                    // ������ת�����Ƶ�ֵ����
                                    mjLaizi.AddTile(i);

                                    // ���Ϊ����
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i;
                                    byBlockTile[2] = i;
                                    byBlockTile[3] = 0;
                                    mjSetEx.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                                }
                                // �����2�ţ��Һ�һ�Ż�����û�ƣ����������ƻ�����ʹ��
                                else if (nMjNum[i] == 2 && (nMjNum[i+1] <= 0 || nMjNum[i+2] <= 0) && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1;

                                    nMjNum[i] = 0;

                                    // ������ת�����Ƶ�ֵ����
                                    mjLaizi.AddTile(i);

                                    // ���Ϊ����
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i;
                                    byBlockTile[2] = i;
                                    byBlockTile[3] = 0;
                                    mjSetEx.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                                }
                                // ���Ϊ˳��
                                else if (nMjNum[i+1] > 0 && nMjNum[i+2] > 0)
                                {
                                    nMjNum[i] -= 1;
                                    nMjNum[i+1] -= 1;
                                    nMjNum[i+2] -= 1;

                                    // ���Ϊ˳��
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i + 1;
                                    byBlockTile[2] = i + 2;
                                    byBlockTile[3] = 0;
                                    mjSetEx.AddSet(byBlockTile/*i*/, ACTION_COLLECT, 0);

                                    // nMjNum[i]���ܻ���һ����, ����ȥ�����ж�
                                    goto ret;
                                }
                                else if (nMjNum[i+1] > 0 && nMjNum[i+2] == 0 && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1;
                                    nMjNum[i] -= 1;
                                    nMjNum[i+1] -= 1;

                                    // ������ת�����Ƶ�ֵ����
                                    mjLaizi.AddTile(i+2);

                                    // ���Ϊ˳��
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i + 1;
                                    byBlockTile[2] = i + 2;
                                    byBlockTile[3] = 0;
                                    mjSetEx.AddSet(byBlockTile/*i*/, ACTION_COLLECT, 0);
                                }
                                else if (nMjNum[i+1] == 0 && nMjNum[i+2] > 0 && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1; 
                                    nMjNum[i] -= 1;
                                    nMjNum[i+2] -= 1;

                                    // ������ת�����Ƶ�ֵ����
                                    mjLaizi.AddTile(i+1);

                                    // ���Ϊ˳��
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i + 1;
                                    byBlockTile[2] = i + 2;
                                    byBlockTile[3] = 0;
                                    mjSetEx.AddSet(byBlockTile/*i*/, ACTION_COLLECT, 0);
                                }
                                else if (nLaiziLength >= 3 - nMjNum[i])
                                {
                                    // �������Ų�������˳��, ��������������ӱ�Ϊ����
                                    nLaiziLength -= 3 - nMjNum[i];

                                    // ������ת�����Ƶ�ֵ����
                                    mjLaizi.AddTile(i, 3 - nMjNum[i]);

                                    // ���Ϊ����
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i;
                                    byBlockTile[2] = i;
                                    byBlockTile[3] = 0;
                                    mjSetEx.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);

                                    nMjNum[i] = 0;
                                }
                                else
                                {
                                    // ƥ��ʧ��
                                    bFail = TRUE;
                                    break;
                                }
                            }
                            else if (8 == tmpTile)
                            {
                                if (nMjNum[i] == 1)
                                {
                                    if (nMjNum[i+1] > 0 && nLaiziLength >= 1)
                                    {
                                        nLaiziLength -= 1;

                                        nMjNum[i] -= 1;
                                        nMjNum[i+1] -= 1;

                                        // ������ת�����Ƶ�ֵ����
                                        mjLaizi.AddTile(i-1);

                                        // ���Ϊ˳��
                                        byBlockTile[0] = i - 1;
                                        byBlockTile[1] = i;
                                        byBlockTile[2] = i + 1;
                                        byBlockTile[3] = 0;
                                        mjSetEx.AddSet(byBlockTile/*i-1*/, ACTION_COLLECT, 0);
                                    }
                                    else if (nLaiziLength >= 2)
                                    {
                                        nLaiziLength -= 2;

                                        nMjNum[i] = 0;

                                        // ������ת�����Ƶ�ֵ����
                                        mjLaizi.AddTile(i, 2);

                                        // ���Ϊ����
                                        byBlockTile[0] = i;
                                        byBlockTile[1] = i;
                                        byBlockTile[2] = i;
                                        byBlockTile[3] = 0;
                                        mjSetEx.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                                    }
                                    else
                                    {
                                        // ʧ��
                                        bFail = TRUE;
                                        break;
                                    }
                                }
                                else if (nMjNum[i] == 2 && nLaiziLength >= 1)
                                {
                                    nLaiziLength -= 1;

                                    nMjNum[i] = 0;

                                    // ������ת�����Ƶ�ֵ����
                                    mjLaizi.AddTile(i);

                                    // ���Ϊ����
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i;
                                    byBlockTile[2] = i;
                                    byBlockTile[3] = 0;
                                    mjSetEx.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);
                                }
                                else
                                {
                                    // ʧ��
                                    bFail = TRUE;
                                    break;
                                }
                            }
                            else if (9 == tmpTile)
                            {
                                // ���Ϊ����, ʹ�������滻
                                if (nLaiziLength >= 3 - nMjNum[i])
                                {
                                    nLaiziLength -= 3 - nMjNum[i];

                                    // ������ת�����Ƶ�ֵ����
                                    mjLaizi.AddTile(i, 3 - nMjNum[i]);

                                    // ���Ϊ����
                                    byBlockTile[0] = i;
                                    byBlockTile[1] = i;
                                    byBlockTile[2] = i;
                                    byBlockTile[3] = 0;
                                    mjSetEx.AddSet(byBlockTile/*i*/, ACTION_TRIPLET, 0);

                                    nMjNum[i] = 0;
                                }
                                else
                                {
                                    // ƥ��ʧ��
                                    bFail = TRUE;
                                    break;
                                }
                            }
                        }
                    } //for

                    if (!bFail)
                    {
                        break;
                    }

                } //for


                if (bFail)
                {
                    // ��Ͳ�������ʧ��, ��������ѭ��
                    continue ;
                }

                for (i = 0; i < TILE_END; i++)
                {
                    if (nMjNum[i] > 0)
                    {
                        // �����ƴ���, ����ʧ��
                        mjLaizi.ReleaseAll();

                        bFail = TRUE;
                    }
                }

                if (bFail)
                {
                    break;
                }

                return TRUE;
                // ----------------------- ����Ͳ���жϽ��� ------------------------
            }//if
        }//for

        if (0 == nCheckFlag)
        {
            nCheckFlag = 1;

            // �������жϱ�־1
            goto RestFlag;
        }

        mjLaizi.ReleaseAll();
        return 0;
    }

    /**
    *  @brief �������Ƶ����������ʵ�ʵ���
    *  @param[out]  mjTilesNoGod ȥ�������ƺ������
    *  @param[in]  mjSet ��ҵ�����
    *  @param[in]  nLaiziCount �����Ƶ�����
    *  @param[out]  mjLaizi �滻�����
    *  @param[in]  mjLaiziTiles �����Ʊ������
    *  @return ����һ������������ֵ
    *
    *  ����������ȥ�����ҳ���󷬣�ֻҪ���������ķ��Ϳ�����
    */
    int CMJRule::ConfigGodTilesFast( CMJHand &mjTilesNoGod, CMJSet mjSet, int nLaiziCount, CMJHand &mjLaizi, CMJHand mjLaiziTiles )
    {
        // ��ʱֻ�ҵ����������ı�����
        return ConfigGodTiles(mjTilesNoGod, mjSet, nLaiziCount, mjLaizi, mjLaiziTiles);
    }

    /**
    *  @brief �������Ƶ����������ʵ�ʵ���
    *  @param[in]  mjTilesNoGod ȥ�������ƺ������
    *  @param[out]  mjTileoGod ȥ�������������飬�������Ǳ��滻�ص���
    *  @param[in]  mjSet ��ҵ�����
    *  @param[in]  nLaiziCount �����Ƶ�����
    *  @param[in]  bNeedFanCount �Ƿ���Ҫ���Ʒ���,�������Ҫ,ֱ�ӷ���1
    *  @return  �����������,����0,���򷵻������˵ķ���ֵ
    */	
    int CMJRule::CheckNeedFan( CMJHand mjTilesNoGod, CMJHand& mjTilesGod, CMJSet mjSet, int nLaiziCount, int bNeedFanCount)
    {
        return 0;
    }

    /**
    *  @brief ���������ļ�
    *  @param[in]  mjdata ���õ�����
    */
    void CMJRule::SetMJSetting( MJ_SETTING_DATA mjdata )
    {
        m_tagMJSetting = mjdata;
    }

    /**
    *  @brief �����㷬��������
    *  @param[in]  env ���������ṹ
    *  @param[in]  nChair Ҫ���㷬����
    *  @param[in]  bSetChairTile Ҫ���÷����˵���
    */
    void CMJRule::SetFanEvn( ENVIRONMENT &env, int nChair, int bSetChairTile)
    {
        if (m_pFanCount != NULL)
        {
            m_pFanCount->m_tagEnv = env;
        }
    }

    /**
    *  @brief ��ȡ���Ƶ�����
    *  @return ���ؽ������ 100 �γ�ͷ 10 ���м� 1�Ժ��棬0ʲô�����ܳ�
    */	
    int CMJRule::GetCollectRes( CMJHand mjHand, TILE ucTile, int nChair, int nGiveChair, int bWind)
    {
        if (nChair == (nGiveChair + GetMJSetting().nGiveOrder) % PLAYER_NBR)
        {
            return mjHand.IsCanCollect(ucTile, bWind, GetMJSetting().nCollateArrow, m_tmpBlockSet);
        }

        // ���ܳ���
        return 0;
    }

    /**
    *  @brief ��ȡָ���Ƶ�����
    *  @param[in]  ucTile ������
    *  @return  ��:0��Ͳ:1����:2����:3����:4������:5
    *
    *  ����ֵ�к궨��ɲο�
    */
    int CMJRule::GetTileGenre( TILE ucTile )
    {
        int nKind = ucTile/10;

        if (nKind >= TILE_GENRE_CHAR && nKind <= TILE_GENRE_FLOWER)
        {
            return nKind;
        }

        return TILE_GENRE_OTHER;
    }
}