#include "MJFanCounter.h"

namespace MyGame
{

CMJFanCounter::CMJFanCounter()
{
	// ��ָ�뺯��
	m_fpCheckFan[0] = Check000;
	m_fpCheckFan[1] = Check001;
	m_fpCheckFan[2] = Check002;
	m_fpCheckFan[3] = Check003;
	m_fpCheckFan[4] = Check004;
	m_fpCheckFan[5] = Check005;
	m_fpCheckFan[6] = Check006;
	m_fpCheckFan[7] = Check007;
	m_fpCheckFan[8] = Check008;
	m_fpCheckFan[9] = Check009;

	m_fpCheckFan[10] = Check010;
	m_fpCheckFan[11] = Check011;
	m_fpCheckFan[12] = Check012;
	m_fpCheckFan[13] = Check013;
	m_fpCheckFan[14] = Check014;
	m_fpCheckFan[15] = Check015;
	m_fpCheckFan[16] = Check016;
	m_fpCheckFan[17] = Check017;
	m_fpCheckFan[18] = Check018;
	m_fpCheckFan[19] = Check019;

	m_fpCheckFan[20] = Check020;
	m_fpCheckFan[21] = Check021;
	m_fpCheckFan[22] = Check022;
	m_fpCheckFan[23] = Check023;
	m_fpCheckFan[24] = Check024;
	m_fpCheckFan[25] = Check025;
	m_fpCheckFan[26] = Check026;
	m_fpCheckFan[27] = Check027;
	m_fpCheckFan[28] = Check028;
	m_fpCheckFan[29] = Check029;

	m_fpCheckFan[30] = Check030;
	m_fpCheckFan[31] = Check031;
	m_fpCheckFan[32] = Check032;
	m_fpCheckFan[33] = Check033;
	m_fpCheckFan[34] = Check034;
	m_fpCheckFan[35] = Check035;
	m_fpCheckFan[36] = Check036;
	m_fpCheckFan[37] = Check037;
	m_fpCheckFan[38] = Check038;
	m_fpCheckFan[39] = Check039;

	m_fpCheckFan[40] = Check040;
	m_fpCheckFan[41] = Check041;
	m_fpCheckFan[42] = Check042;
	m_fpCheckFan[43] = Check043;
	m_fpCheckFan[44] = Check044;
	m_fpCheckFan[45] = Check045;
	m_fpCheckFan[46] = Check046;
	m_fpCheckFan[47] = Check047;
	m_fpCheckFan[48] = Check048;
	m_fpCheckFan[49] = Check049;

	m_fpCheckFan[50] = Check050;
	m_fpCheckFan[51] = Check051;
	m_fpCheckFan[52] = Check052;
	m_fpCheckFan[53] = Check053;
	m_fpCheckFan[54] = Check054;
	m_fpCheckFan[55] = Check055;
	m_fpCheckFan[56] = Check056;
	m_fpCheckFan[57] = Check057;
	m_fpCheckFan[58] = Check058;
	m_fpCheckFan[59] = Check059;

	m_fpCheckFan[60] = Check060;
	m_fpCheckFan[61] = Check061;
	m_fpCheckFan[62] = Check062;
	m_fpCheckFan[63] = Check063;
	m_fpCheckFan[64] = Check064;
	m_fpCheckFan[65] = Check065;
	m_fpCheckFan[66] = Check066;
	m_fpCheckFan[67] = Check067;
	m_fpCheckFan[68] = Check068;
	m_fpCheckFan[69] = Check069;

	m_fpCheckFan[70] = Check070;
	m_fpCheckFan[71] = Check071;
	m_fpCheckFan[72] = Check072;
	m_fpCheckFan[73] = Check073;
	m_fpCheckFan[74] = Check074;
	m_fpCheckFan[75] = Check075;
	m_fpCheckFan[76] = Check076;
	m_fpCheckFan[77] = Check077;
	m_fpCheckFan[78] = Check078;
	m_fpCheckFan[79] = Check079;

	m_fpCheckFan[80] = Check080;
	m_fpCheckFan[81] = Check081;
	m_fpCheckFan[82] = Check082;
	m_fpCheckFan[83] = Check083;
	m_fpCheckFan[84] = Check084;
	m_fpCheckFan[85] = Check085;
	m_fpCheckFan[86] = Check086;
	m_fpCheckFan[87] = Check087;
	m_fpCheckFan[88] = Check088;
	m_fpCheckFan[89] = Check089;

	m_fpCheckFan[90] = Check090;
	m_fpCheckFan[91] = Check091;
	m_fpCheckFan[92] = Check092;
	m_fpCheckFan[93] = Check093;
	m_fpCheckFan[94] = Check094;
	m_fpCheckFan[95] = Check095;
	m_fpCheckFan[96] = Check096;
	m_fpCheckFan[97] = Check097;
	m_fpCheckFan[98] = Check098;
	m_fpCheckFan[99] = Check099;

	m_fpCheckFan[100] = Check100;
	m_fpCheckFan[101] = Check101;
	m_fpCheckFan[102] = Check102;
	m_fpCheckFan[103] = Check103;
	m_fpCheckFan[104] = Check104;
	m_fpCheckFan[105] = Check105;
	m_fpCheckFan[106] = Check106;
	m_fpCheckFan[107] = Check107;
	m_fpCheckFan[108] = Check108;
	m_fpCheckFan[109] = Check109;

	m_fpCheckFan[110] = Check110;
	m_fpCheckFan[111] = Check111;
	m_fpCheckFan[112] = Check112;
	m_fpCheckFan[113] = Check113;
	m_fpCheckFan[114] = Check114;
	m_fpCheckFan[115] = Check115;
	m_fpCheckFan[116] = Check116;
	m_fpCheckFan[117] = Check117;
	m_fpCheckFan[118] = Check118;
	m_fpCheckFan[119] = Check119;

	m_fpCheckFan[120] = Check120;
	m_fpCheckFan[121] = Check121;
	m_fpCheckFan[122] = Check122;
	m_fpCheckFan[123] = Check123;
	m_fpCheckFan[124] = Check124;
	m_fpCheckFan[125] = Check125;
	m_fpCheckFan[126] = Check126;
	m_fpCheckFan[127] = Check127;
}

CMJFanCounter::~CMJFanCounter()
{

}

/**
*  @brief ��ȡ����
*  @return  ����������ܷ���
*/	
int CMJFanCounter::GetFanCount()
{
	int i, nFanCount = 0;

	// ���㷬��
	for (i = 0; i < MAX_FAN_NUMBER; i++)
	{
        // ��¼��ǰ�жϵķ���ID
        m_byFanID = i;

		// ��ⷬ��
		CheckFanNode(m_tagFanCount.m_FanNode[i]);
	}

	// ͳ�Ʒ���
	for (i = 0; i < MAX_FAN_NUMBER; i++)
	{
		if (m_tagFanCount.m_FanNode[i].bFan)
		{
			// �ۼƷ���
			nFanCount += m_tagFanCount.m_FanNode[i].byFanPoint;
		}
	}

	return nFanCount;
}

/**
*  @brief �����㷬�Ļ���
*  @param[in]  env ����������
*/	
void CMJFanCounter::SetEnv( ENVIRONMENT& env )
{
	m_tagEnv = env;
}

/**
*  @brief �������н����Ϣ
*  @param[in]  tagFanCount ����������
*/	
void CMJFanCounter::SetFanCount( FAN_COUNT& tagFanCount )
{
	m_tagFanCount = tagFanCount;
}

/**
*  @brief ��������CHECK���
*
*  �������ý���CHECK����,��ȷ���´��㷬��ʱ���ܱ�����
*/	
void CMJFanCounter::ResetFanCount()
{

}

/**
*  @brief ���ָ����ҵ�����
*  @param[out]  mjHand �����÷�ʽ�������ø���
*  @param[in]  nChair �����ȡ�����
* 
*  ��ȡ���ݵ���Դ�����õĻ������ݽṹ
*/	
void CMJFanCounter::GetHandTiles( CMJHand& mjHand, int nChair )
{
	mjHand.ReleaseAll();
	for (int i = 0; i < m_tagEnv.byHandCount[nChair]; i++)
	{
		mjHand.AddTile(m_tagEnv.tHand[nChair][i]);
	}
}

/**
*  @brief ���ָ����ҵ�ȫ������
*  @param[out]  mjAll �����÷�ʽ�������ø���
*  @param[in]  nChair �����ȡ�����
*
*  ��ȡ���ݵ���Դ�����õĻ������ݽṹ,�˺��������ҵ��������ϵ�������һ�𷵻�,�ܱ����ϻ����
*/	
void CMJFanCounter::GetAllTiles( CMJHand& mjAll, int nChair )
{
	// ��ȡ����
	GetHandTiles(mjAll, nChair);

	// ���������
	for (int i = 0; i < m_tagEnv.bySetCount[nChair]; i++)
	{
		if (ACTION_TRIPLET == m_tagEnv.tSet[nChair][i][0]                // ��
		|| ACTION_QUADRUPLET_PATCH == m_tagEnv.tSet[nChair][i][0]        // ��(����)
		|| ACTION_QUADRUPLET_CONCEALED == m_tagEnv.tSet[nChair][i][0]    // ����
		|| ACTION_QUADRUPLET_REVEALED == m_tagEnv.tSet[nChair][i][0]     // ����
        || ACTION_QUADRUPLET_DNXB == m_tagEnv.tSet[nChair][i][0]         // ����������
        || ACTION_QUADRUPLET_ZFB == m_tagEnv.tSet[nChair][i][0])         // �з��׸�
		{
			// ����������ϳɿ���
			mjAll.AddTile(m_tagEnv.tSet[nChair][i][1], 3);
		}
		else if (ACTION_COLLECT == m_tagEnv.tSet[nChair][i][0])          // ��
		{
			// �жϳ��Ƶ�λ��
			//switch (m_tagEnv.tSet[nChair][i][2])
			//{
			//case 0:    // ͷ
			//	{
			//		mjAll.AddTile(m_tagEnv.tSet[nChair][i][1], 3, TRUE);
			//	}
			//	break;
			//case 1:    // ��
			//	{
			//		mjAll.AddTile(m_tagEnv.tSet[nChair][i][1]-1, 3, TRUE);
			//	}
			//	break;
			//case 2:    // β
			//	{
			//		mjAll.AddTile(m_tagEnv.tSet[nChair][i][1]-2, 3, TRUE);
			//	}
			//	break;
   //         default:
   //             break;
			//}

            mjAll.AddTile(m_tagEnv.tSet[nChair][i][1], 3, TRUE);
		}
	}
}


/**
*  @brief ���һ�����Ľ��
*  @param[in/out]  tagFan �������ݽṹ
*
*  ����������Զ����ý��ΪTRUE�����ر��������õĳ�����͹رյķ���
*/	
void CMJFanCounter::CheckFanNode( FAN_NODE &tagFan )
{
	if (tagFan.nCheckNbr <= 0)
	{
		// �˷������ж�
		tagFan.bFan = FALSE;

		return ;
	}

	int i;

	// �жϷ���
	for (i = 0; i < tagFan.nCheckNbr; i++)
	{
		if (!m_fpCheckFan[tagFan.nCheckFunID[i]](this))
		{
			// ���Ͳ�����
			tagFan.bFan = FALSE;

			return ;
		}
	}

	// ���ͳ���
	tagFan.bFan = TRUE;

	// �ų�����Ҫ����ķ���
	for (i = 0; i < tagFan.nNoCheckNbr; i++)
	{
		m_tagFanCount.m_FanNode[tagFan.nNoCheckFan[i]].bFan = FALSE;
	}

	//��ȫ�������߶ԡ����ǲ��������߶ԡ��������ơ�ʮ���۵������������ж�(���Ʋ����ˣ���������)
	if ((42 == m_byFanID)		//ȫ����
		|| (53 == m_byFanID)	//�߶�
		|| (54 == m_byFanID)	//���ǲ���
		|| (78 == m_byFanID)	//���߶�
		|| (76 == m_byFanID)	//��������
		|| (79 == m_byFanID))	//ʮ����
	{
		if (MJ_WIN_SELFDRAW == m_tagEnv.byFlag)	//���������
		{
			m_tagFanCount.m_FanNode[0].bFan = TRUE;
		}
	}
}

/**
*  @brief ������ID��Ӱ�亯���Ƿ����
*  @param[in]  nID ��Ӱ��ķ�ID
*  @return �����������TRUE,���򷵻�FALSE
*/	
BOOL CMJFanCounter::CheckFanByID( int nID )
{
	if (nID >= 0 && nID < MAX_FAN_NUMBER)
	{
		return m_fpCheckFan[nID](this);
	}

	return FALSE;
}

/**
*  @brief �����ͨ����
*  @param[in]  tilesHand ��������
*  @param[in]  bNoJiang �Ƿ�Ҫ�жϽ���,���ΪTRUE,�������н���
*  @return �����������TRUE, ���򷵻�FALSE
*/	
BOOL CMJFanCounter::CheckWinNormal(CMJHand tilesHand, BOOL bNoJiang)
{
	if (tilesHand.CurrentLength() <= 0)
	{
		return FALSE;
	}

	BOOL bFail = FALSE;     // ��־������Ƿ�ʧ��
	int i, j, tmpTile;

	CMJHand mjHand;
	CMJSet  mjSet;

	// ��¼ÿ��������
	int nMjNum[TILE_END] = {0};

    BYTE byBlockTile[4];

	for (int nStart = 0; nStart < tilesHand.CurrentLength(); nStart++)
	{
		mjHand = tilesHand;    // ÿ�ж�һ��, �������ݻ�ԭ

		tmpTile = mjHand.GetTile(nStart);

		mjSet.ReleaseAll();

        // �ж��Ƿ���Ҫ����
        if (bNoJiang)
        {
		    // �Ȱ�һ�Խ��������
		    if (mjHand.IsHave(tmpTile, 2))
		    {
			    mjHand.DelTile(tmpTile, 2);
            }
            else
            {
                // �ǽ���, ����һ�����ж�
                continue ;
            }
        }

        bFail = FALSE;
        memset(nMjNum, 0, sizeof(nMjNum));

        // �������
        for (i = 0; i < mjHand.CurrentLength(); i++)
        {
            nMjNum[mjHand.GetTile(i)] += 1;  // �ۼ�������
        }

        // ------------------------- �����жϿ�ʼ ------------------------
        // �����Ƴ���, �ȼ����, �����˳��
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
        
        if (m_tagEnv.byCollectWind > 0)
        {
            // ����˳��
            BYTE nFengCount[4][2];

            for (i = TILE_EAST; i <= TILE_NORTH; i++)
            {
                nFengCount[i-TILE_EAST][0] = i;            // �洢��ֵ
                nFengCount[i-TILE_EAST][1] = nMjNum[i];    // �洢�Ƶ�����
            }

            while ((nFengCount[0][1] + nFengCount[1][1] + nFengCount[2][1] + nFengCount[3][1]) > 0)
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
                    // ƥ��ʧ��
                    bFail = TRUE;
                    break;
                }
            }
        }
		else
		{
			// ���Ʋ��ܳ�, �����ж��Ƿ���ʣ��ķ���, ����˴��ж�ʧ��
			for (i = TILE_EAST; i <= TILE_NORTH; i++)
			{
				if (nMjNum[i] >= 1)
				{
					bFail = TRUE;
					break;
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
        // ��ͷ�Ƴ���, �ȼ����, �����˳��
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

        if (m_tagEnv.byCollectArrow > 0)
        {
            // ����˳��
            BYTE nArrowCount[3][2];

            for (i = TILE_ZHONG; i <= TILE_BAI; i++)
            {
                nArrowCount[i-TILE_ZHONG][0] = i;
                nArrowCount[i-TILE_ZHONG][1] = nMjNum[i];
            }

            while ((nArrowCount[0][1] + nArrowCount[1][1] + nArrowCount[2][1]) > 0)
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
                    // ƥ��ʧ��
                    bFail = TRUE;
                    break;
                }
            }
        }
		else
		{
			// ��ͷ�Ʋ��ܳ�, �����ж��Ƿ���ʣ��ļ�ͷ��, ����˴��ж�ʧ��
			for (i = TILE_ZHONG; i <= TILE_BAI; i++)
			{
				if (nMjNum[i] >= 1)
				{
					bFail = TRUE;
					break;
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
                    // ���Ϊ˳��
                    if (nMjNum[i+1] > 0 && nMjNum[i+2] > 0)
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
                    else
                    {
                        // ƥ��ʧ��
                        bFail = TRUE;
                        break;
                    }
                }
                else
                {
                    // ʧ��
                    bFail = TRUE;
                    break;
                }
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
                bFail = TRUE;
            }
        }

        if (bFail)
        {
            break;
        }

        return TRUE;
        // ----------------------- ����Ͳ���жϽ��� ------------------------
    }//for

	return FALSE;
}

/**
*  @brief ���ȫ˳�Ӻ���
*  @param[in]  tilesHand ��������
*  @return �����������TRUE,���򷵻�FALSE
*/	
BOOL CMJFanCounter::CheckWinCollect(CMJHand tilesHand)
{
	if (tilesHand.CurrentLength() <= 0)
	{
		return FALSE;
	}

	BOOL bFail = FALSE;     // ��־������Ƿ�ʧ��
	int i, j, tmpTile;

	CMJHand mjHand;
	CMJSet  mjSet;

	// ��¼ÿ��������
	int nMjNum[TILE_END] = {0};

    // ˳������
    BYTE byBlockTile[4];

	for (int nStart = 0; nStart < tilesHand.CurrentLength(); nStart++)
	{
		mjHand = tilesHand;    // ÿ�ж�һ��, �������ݻ�ԭ

		tmpTile = mjHand.GetTile(nStart);

		// �Ȱ�һ�Խ��������
		if (mjHand.IsHave(tmpTile, 2))
		{
			mjHand.DelTile(tmpTile, 2);

			bFail = FALSE;
			memset(nMjNum, 0, sizeof(nMjNum));

			// �������
			for (i = 0; i < mjHand.CurrentLength(); i++)
			{
				nMjNum[mjHand.GetTile(i)] += 1;  // �ۼ�������
			}

            // ------------------------- �����жϿ�ʼ ------------------------
            // �����Ƴ���
            if (m_tagEnv.byCollectWind > 0)
            {
                // ����˳��
                BYTE nFengCount[4][2];

                for (i = TILE_EAST; i <= TILE_NORTH; i++)
                {
                    nFengCount[i-TILE_EAST][0] = i;            // �洢��ֵ
                    nFengCount[i-TILE_EAST][1] = nMjNum[i];    // �洢�Ƶ�����
                }

                while ((nFengCount[0][1] + nFengCount[1][1] + nFengCount[2][1] + nFengCount[3][1]) > 0)
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
                        // ƥ��ʧ��
                        bFail = TRUE;
                        break;
                    }
                }

                if (bFail)
                {
                    // ���������ʧ��, ��������ѭ��
                    continue ;
                }
            }
            // ------------------------- �����жϽ��� ------------------------

            // ------------------------ ��ͷ���жϿ�ʼ ------------------------
            if (m_tagEnv.byCollectArrow > 0)
            {
                // ����˳��
                BYTE nArrowCount[3][2];

                for (i = TILE_ZHONG; i <= TILE_BAI; i++)
                {
                    nArrowCount[i-TILE_ZHONG][0] = i;
                    nArrowCount[i-TILE_ZHONG][1] = nMjNum[i];
                }

                while ((nArrowCount[0][1] + nArrowCount[1][1] + nArrowCount[2][1]) > 0)
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
                        // ƥ��ʧ��
                        bFail = TRUE;
                        break;
                    }
                }

                if (bFail)
                {
                    // ��ͷ�������ʧ��, ��������ѭ��
                    continue ;
                }
            }
            // ------------------------ ��ͷ���жϽ��� ------------------------

            // ----------------------- ����Ͳ���жϿ�ʼ ------------------------
			// �������
			for (i = TILE_CHAR_1; i <= TILE_BALL_9; i++)
			{
				// 10��20������ || ����������0�߲��ж�
				if ((i % 10) == 0 || nMjNum[i] <= 0)
				{
					continue ;
				}

ret:
				// �ж�˳��
				if (nMjNum[i] > 0)
				{
					tmpTile = i % 10;

					if (tmpTile < 8)
					{
						// ���Ϊ˳��
						if (nMjNum[i+1] > 0 && nMjNum[i+2] > 0)
						{
							nMjNum[i] -= 1;
							nMjNum[i+1] -= 1;
							nMjNum[i+2] -= 1;

							// ���Ϊ˳��
                            byBlockTile[0] = i;
                            byBlockTile[1] = i + 1;
                            byBlockTile[2] = i + 2;
                            byBlockTile[3] = 0;
							mjSet.AddSet(byBlockTile/*i*/, ACTION_COLLECT, 1);

							// nMjNum[i]���ܻ���һ����, ����ȥ�����ж�
							goto ret;
						}
						else
						{
							// ƥ��ʧ��
							bFail = TRUE;
							break;
						}
					}
					else
					{
						// ʧ��
						bFail = TRUE;
						break;
					}
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
					// �����ƴ���
					return FALSE;
				}
			}

			return TRUE;
			// ----------------------- ����Ͳ���жϽ��� ------------------------
		}//if
	}//for

	return FALSE;
}

/**
*  @brief ���ȫ���Ӻ���
*  @param[in]  tilesHand ��������
*  @return �����������TRUE,���򷵻�FALSE
*/	
BOOL CMJFanCounter::CheckWinTriplet(CMJHand tilesHand)
{
	if (tilesHand.CurrentLength() % 3 != 2)
	{
		return FALSE;
	}

	// ȥ�������еĿ�����
	for (int i = 0; i < tilesHand.CurrentLength(); i++)
	{
		if (tilesHand.IsHave(tilesHand.GetTile(i), 3))
		{
			tilesHand.DelTile(tilesHand.GetTile(i), 3);
			i = 0;
		}
	}

	// ȥ�������ƺ�, ʣ���Ƶ���2����, �ж��Ƿ�Ϊһ�Խ���
	if (tilesHand.CurrentLength() == 2)
	{
		if (tilesHand.IsHave(tilesHand.GetTile(0), 2))
		{
			return TRUE;
		}
	}

	return FALSE;
}

/**
*  @brief ���ȫ��X����(XΪĳ���Ƶ�)
*  @param[in]  tilesHand ��������
*  @param[in]  byNum     �ж�ÿ�鼰����Ҫ�������Ƶ�
*  @return �����������TRUE,���򷵻�FALSE
*/	
BOOL CMJFanCounter::CheckWinWithX(CMJHand tilesHand, BYTE byNum)
{
	if (tilesHand.CurrentLength() <= 0)
	{
		return FALSE;
	}

	BOOL bFail = FALSE;     // ��־������Ƿ�ʧ��
	int i, tmpTile, tmpGenre, tmpNumber;

	CMJHand mjHand;
	CMJSet  mjSet;

	// ��¼ÿ��������
	int nMjNum[TILE_END] = {0};

    BYTE byBlockTile[4];

	for (i = 0; i < tilesHand.CurrentLength(); i++)
	{
		tmpTile = tilesHand.GetTile(i);
		tmpNumber = tilesHand.GetTileNumber(tmpTile);
		tmpGenre = tilesHand.GetTileGenre(tmpTile);

		if ((tmpGenre < TILE_GENRE_CHAR || tmpGenre > TILE_GENRE_BALL) 
		|| (tmpNumber < (byNum - 2) || tmpNumber > (byNum + 2)))
		{
			// ����, ������Ͳ�� �� ��ֵ�ڼ����Ƶ�(-2,2)�ķ�Χ֮�⡣��: X=5, 3-7���ܺ�5�й�ϵ(��: 345��567)
			return FALSE;
		}
	}

	for (int nStart = 0; nStart < tilesHand.CurrentLength(); nStart++)
	{
		mjHand = tilesHand;    // ÿ�ж�һ��, �������ݻ�ԭ

		tmpTile = mjHand.GetTile(nStart);

		// �Ȱ�һ�Խ��������
		if (mjHand.IsHave(tmpTile, 2))
		{
			if (mjHand.GetTileNumber(tmpTile) != byNum)
			{
				// ������û��X
				continue ;
			}

			mjHand.DelTile(tmpTile, 2);

			bFail = FALSE;
			memset(nMjNum, 0, sizeof(nMjNum));

			// �������
			for (i = 0; i < mjHand.CurrentLength(); i++)
			{
				nMjNum[mjHand.GetTile(i)] += 1;  // �ۼ�������
			}

			// ----------------------- ����Ͳ���жϿ�ʼ ------------------------
			// �������
			for (i = TILE_CHAR_1; i <= TILE_BALL_9; i++)
			{
				tmpTile = i % 10;

				// 10��20������ || ����������0�߲��ж�
				if (tmpTile == 0 || nMjNum[i] <= 0)
				{
					continue ;
				}

				if (tmpTile == byNum)
				{
					// ֻ��X����Ϊ����
					if ((nMjNum[i] > 3 && nMjNum[i+1] < 2)    // ��: X=5, 5��4��, 6С��2��, �������5Ϊ����, ���: 555 56*
					 || (nMjNum[i] == 3 && nMjNum[i+1] < 0))  // ��: X=5, 5��3��, 6û��һ����, �������5Ϊ����
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

ret:
				// �ж�˳��
				if (nMjNum[i] > 0)
				{
                    // ��: X=5, if (tmpTile >= 5 - 2 && tmpTile <= 5)
					if (tmpTile >= byNum - 2 && tmpTile <= byNum)
					{
						// ���Ϊ˳��
						if (nMjNum[i+1] > 0 && nMjNum[i+2] > 0)
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
						else
						{
							// ƥ��ʧ��
							bFail = TRUE;
							break;
						}
					}
					else
					{
						// ƥ��ʧ��
						bFail = TRUE;
						break;
					}
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
					// �����ƴ���
					return FALSE;
				}
			}

			return TRUE;
			// ----------------------- ����Ͳ���жϽ��� ------------------------
		}//if
	}//for

	return FALSE;
}

/**
*  @brief ��鴿ȫ19����
*  @param[in]  tilesHand ��������
*  @return �����������TRUE,���򷵻�FALSE
*/	
BOOL CMJFanCounter::CheckWinWith19(CMJHand tilesHand)
{
	if (tilesHand.CurrentLength() <= 0)
	{
		return FALSE;
	}

	BOOL bFail = FALSE;     // ��־������Ƿ�ʧ��
	int i, tmpTile, tmpGenre, tmpNumber;

	CMJHand mjHand;
	CMJSet  mjSet;

	// ��¼ÿ��������
	int nMjNum[TILE_END] = {0};

    BYTE byBlockTile[4];

	for (i = 0; i < tilesHand.CurrentLength(); i++)
	{
		tmpTile = tilesHand.GetTile(i);
		tmpNumber = tilesHand.GetTileNumber(tmpTile);
		tmpGenre = tilesHand.GetTileGenre(tmpTile);

		if ((tmpGenre < TILE_GENRE_CHAR || tmpGenre > TILE_GENRE_BALL) 
			|| (tmpNumber != 1 || tmpNumber != 9))
		{
			// ����, ������Ͳ�� �� ��ֵ����1��9����
			return FALSE;
		}
	}

	for (int nStart = 0; nStart < tilesHand.CurrentLength(); nStart++)
	{
		mjHand = tilesHand;    // ÿ�ж�һ��, �������ݻ�ԭ

		tmpTile = mjHand.GetTile(nStart);

		// �Ȱ�һ�Խ��������
		if (mjHand.IsHave(tmpTile, 2))
		{
			mjHand.DelTile(tmpTile, 2);

			bFail = FALSE;
			memset(nMjNum, 0, sizeof(nMjNum));

			// �������
			for (i = 0; i < mjHand.CurrentLength(); i++)
			{
				nMjNum[mjHand.GetTile(i)] += 1;  // �ۼ�������
			}

			// ----------------------- ����Ͳ���жϿ�ʼ ------------------------
			// �������
			for (i = TILE_CHAR_1; i <= TILE_BALL_9; i++)
			{
				tmpTile = i % 10;

				// 10��20������ || ����������0�߲��ж�
				if (tmpTile == 0 || nMjNum[i] <= 0)
				{
					continue ;
				}

				// ֻ��5����Ϊ����
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

				if (nMjNum[i] > 0)
				{
					// ��ʣ��, ʧ��
					bFail = TRUE;
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
					// �����ƴ���
					return FALSE;
				}
			}

			// 19ȫΪ����
			return TRUE;
			// ----------------------- ����Ͳ���жϽ��� ------------------------
		}//if
	}//for

	return FALSE;
}

/**
*  @brief ��һɫ - ֻ����һ�ֻ�ɫ(��Ͳ��)����
*/
BOOL CMJFanCounter::Check000(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	CMJHand mjHand;

	// ��ȡ����������(����+����)
	pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

	int tmpGenre;

	// ��¼��һ���ƵĻ�ɫ, �Դ˻�ɫ��Ϊ�Ƚ϶���
	int nFirstGenre = mjHand.GetTileGenre(mjHand.GetTile(0));

	for (int i = 0; i < mjHand.CurrentLength(); i++)
	{
		// ��ȡ�ƵĻ�ɫֵ
		tmpGenre = mjHand.GetTileGenre(mjHand.GetTile(i));

		if (tmpGenre < TILE_GENRE_CHAR || tmpGenre > TILE_GENRE_BALL)
		{
			// ��ɫ����������Ͳ��Χ
			return FALSE;
		}

		if (nFirstGenre != tmpGenre)
		{
			// ��ɫ��һ��
			return FALSE;
		}
	}

	return TRUE;
}

/**
*  @brief ��һɫ - ����һ�ֻ�ɫ���ƺ����ƣ���������ȫ��һ�ֻ�ɫ���ƻ�ȫ��������
*/
BOOL CMJFanCounter::Check001(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	CMJHand mjHand;

	// ��ȡ����������(����+����)
	pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

	// ��¼�����е���Ͳ������������
	int iWan = 0, iTong = 0, iTiao = 0, iZi = 0;

	for (int i = 0; i < mjHand.CurrentLength(); i++)
	{
		switch (mjHand.GetTileGenre(mjHand.GetTile(i)))
		{
		case TILE_GENRE_CHAR:   // ��
			{
				iWan = 1;
				break; 
			}
		case TILE_GENRE_BALL:   // Ͳ
			{
				iTong = 1; 
				break;
			}
		case TILE_GENRE_BAMBOO: // ��
			{
				iTiao = 1;
				break;   
			}
		case TILE_GENRE_WIND:   // ��
			{
				iZi = 1;
				break;  
			}
		default:
			{
				// �Ƿ�������
				return FALSE;
			}
		}
	}

    if ((iWan + iTong + iTiao) == 1 && iZi == 1)
    {
        // ����
        return TRUE;
    }

	return FALSE;
}

/**
*  @brief �߶� - 7������
*/
BOOL CMJFanCounter::Check002(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	CMJHand mjHand;

	// ��ȡ��������
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	if (mjHand.CurrentLength() != 14)
	{
		// ����������, �߶Ա�����14����
		return FALSE;
	}

	while (mjHand.CurrentLength() > 0)
	{
		// �жϴ������������Ƿ���һ��
		if (mjHand.IsHave(mjHand.GetTile(0), 2))
		{
			// ɾ���˶���, �����������ж�
			mjHand.DelTile(mjHand.GetTile(0), 2);
		}
		else
		{
			// ���Ƕ���
			return FALSE;
		}
	}

	return TRUE;
}

/**
*  @brief ��19 - ������û��һ���ż����� 
*/
BOOL CMJFanCounter::Check003(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	CMJHand mjHand;
    TILE tile;

	// ��ȡ����������(����+����)
	pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

	for (BYTE i = 0; i < mjHand.CurrentLength(); i++)
	{
        tile = mjHand.GetTile(i);
        if (tile % 10 == 1 || tile % 10 == 9 || (tile >= TILE_EAST && tile <= TILE_BAI))
        {
            // �������� 1��9 �������ݴ���
            return FALSE;
        }
	}

	return TRUE;
}

/**
*  @brief ������ - 4���Ƽ�����û�г��ơ�����(��������)����������
*/
BOOL CMJFanCounter::Check004(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	// ���ԡ������ܵ��Ʊ�־
	for (int i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_QUADRUPLET_CONCEALED != pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
         && ACTION_QUADRUPLET_DNXB != pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
         && ACTION_QUADRUPLET_ZFB != pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			// �����в��ǰ���, ʧ��
			return FALSE;
		}
	}

    if (MJ_WIN_SELFDRAW == pCounter->m_tagEnv.byFlag   // ����
     || MJ_WIN_GANGDRAW == pCounter->m_tagEnv.byFlag)  // ���ϻ�
    {
        return TRUE;
    }

	return FALSE;
}

/**
*  @brief ���۾� - ���۾��ƺ�������ɵ����� 
*/
BOOL CMJFanCounter::Check005(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	CMJHand mjHand;

	// ��ȡ����������(����+����)
	pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

	int tmpGenre, nNumber;
	BOOL  b19 = FALSE;     // ��־19�Ƿ����
	BOOL  bWind = FALSE;   // ��־�����Ƿ����

	for (int i = 0; i < mjHand.CurrentLength(); i++)
	{
		// ��ȡ�ƵĻ�ɫֵ
		tmpGenre = mjHand.GetTileGenre(mjHand.GetTile(i));

		if (tmpGenre >= TILE_GENRE_CHAR && tmpGenre <= TILE_GENRE_BALL)
		{
			// ��ȡ�Ƶĵ���
			nNumber = mjHand.GetTileNumber(mjHand.GetTile(i));

			if ((1 == nNumber || 9 == nNumber) 
			 && mjHand.IsHave(mjHand.GetTile(i), 2))   // ������������2����
			{
				b19 = TRUE;
			}
			else
			{
				// ��19
				return FALSE;
			}
		}
		else 
		{
			if (TILE_GENRE_WIND == tmpGenre)
			{
				// ��־���ƴ���
				bWind = TRUE;
			}
			else
			{
				// δ֪����
				return FALSE;
			}
		}
	}

	if (b19 && bWind)
	{
		return TRUE;
	}

	return FALSE;
}

/**
*  @brief ȫ���� - ȫ�����ơ����ơ����������������ƺ��ơ����Ƶ���
*/
BOOL CMJFanCounter::Check006(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	CMJHand mjHand;

	// ��ȡ��������
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	if (mjHand.CurrentLength() != 2)
	{
		// ����������, �����ܺ����ֻʣ2��
		return FALSE;
	}

	// ���ԡ������ܵ��Ʊ�־
	for (int i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
        if (ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			// ȫ���˲��ܰ���
			return FALSE;
		}
	}

	// ����Ƿ���˵��ڡ����ܡ�������
	if (MJ_WIN_GUN == pCounter->m_tagEnv.byFlag        // ����
		|| MJ_WIN_GANG == pCounter->m_tagEnv.byFlag       // ����
		|| MJ_WIN_GANGGIVE == pCounter->m_tagEnv.byFlag)  // ������
	{
		return TRUE;
	}

	return FALSE;
}	

/**
*  @brief ƽ�� - ��4��˳�Ӽ�������������ɵĺ���(���Ʋ�������)���ߡ���������Ӱ��ƽ��
*/
BOOL CMJFanCounter::Check007(CMJFanCounter* pCounter)
{   
	if (NULL == pCounter)
	{
		return FALSE;
	}

    BYTE i;

    // ���ԡ������ܵ��Ʊ�־
    for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_COLLECT != pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            // ����, ƽ�͵����Ʊ����ǳ���
            return FALSE;
        }
    }

   
	CMJHand mjHand;

	// ��ȡ��������
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);
	
    for (i = 0; i < 7; i++)
    {
        if (mjHand.IsHave(TILE_EAST + i))
        {
            return FALSE;  // ����������
        }
    }

	// �ж��Ƿ�ȫ˳�Ӻ���
	if (pCounter->CheckWinCollect(mjHand))
	{
		return TRUE;
	}

	return FALSE;
}

/**
*  @brief ���� - ֻҪ����������, �����ǲ��Ǹ��ϻ�
*/
BOOL CMJFanCounter::Check008(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	// ��������
	if (MJ_WIN_SELFDRAW == pCounter->m_tagEnv.byFlag   // ����
	 || MJ_WIN_GANGDRAW == pCounter->m_tagEnv.byFlag)  // ���ϻ�
	{
		return TRUE;
	}

	return FALSE;
}

/**
*  @brief ���� - ����������м�һ�ţ��룺2��3��4 ����3
*/
BOOL CMJFanCounter::Check009(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	if (pCounter->m_tagEnv.tLast % 10 == 1 
	 || pCounter->m_tagEnv.tLast % 10 == 9)
	{
		// 1��9������Ϊ����
		return FALSE;
	}

	CMJHand mjHand, mjTemp;

	// ��ȡ��������
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	//�Ƿ��ǿ�
	BOOL bIsKan = FALSE;
	mjTemp = mjHand;

	if (mjHand.IsHave(pCounter->m_tagEnv.tLast - 1) 
	 && mjHand.IsHave(pCounter->m_tagEnv.tLast + 1))
	{
		// �͵�������ǰһ�����һ�ű������
		mjHand.DelTile(pCounter->m_tagEnv.tLast - 1);
		mjHand.DelTile(pCounter->m_tagEnv.tLast);
		mjHand.DelTile(pCounter->m_tagEnv.tLast + 1);

		// ȥ���͵���һ����, �ж��Ƿ񻹿��Ժ���
		if (pCounter->CheckWinNormal(mjHand, TRUE))
		{
			bIsKan = TRUE;
		}
	}

	// ������ǿ���ֱ�ӷ���FALSE
	if (!bIsKan)
	{
		return FALSE;
	}

	// �ж��Ƿ�ֻ�ܺ���һ����(���������������������һ���������ƣ����ԣ������ж�)
	TILE ucTile;
	int nTileNum = mjHand.GetTileNumber(pCounter->m_tagEnv.tLast);
	if (nTileNum > 3)
	{
		// ǰһ����
		ucTile = pCounter->m_tagEnv.tLast - 3;
		mjHand = mjTemp;

		// �ж��Ƿ��ܺ�ǰһ����
		mjHand.DelTile(pCounter->m_tagEnv.tLast);
		mjHand.AddTile(ucTile);

		//����ܺ������ǿ���
		if (pCounter->CheckWinNormal(mjHand, TRUE))
		{
			return FALSE;
		}
	}

	if (nTileNum < 7)
	{
		// ��һ����
		ucTile = pCounter->m_tagEnv.tLast + 3;
		mjHand = mjTemp;

		// �ж��Ƿ��ܺ���һ����
		mjHand.DelTile(pCounter->m_tagEnv.tLast);
		mjHand.AddTile(ucTile);

		//����ܺ������ǿ���
		if (pCounter->CheckWinNormal(mjHand, TRUE))
		{
			return FALSE;
		}
	}
	
	return TRUE;
}		

/**
*  @brief ���� - ���������123��3��789��7
*/
BOOL CMJFanCounter::Check010(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	if (pCounter->m_tagEnv.tLast % 10 != 3
	 && pCounter->m_tagEnv.tLast % 10 != 7)
	{
		// �͵������Ʋ�����3��7
		return FALSE;
	}

	CMJHand mjHand, mjTemp;

	// ��ȡ��������
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	mjTemp = mjHand;
	BOOL bIsBian = FALSE;

	if (pCounter->m_tagEnv.tLast % 10 == 3)
	{
		// �жϱ���3
		if (mjHand.IsHave(pCounter->m_tagEnv.tLast - 1) 
		 && mjHand.IsHave(pCounter->m_tagEnv.tLast - 2))
		{
			// �͵�������ǰ����ű������
			mjHand.DelTile(pCounter->m_tagEnv.tLast - 1);
			mjHand.DelTile(pCounter->m_tagEnv.tLast - 2);
			mjHand.DelTile(pCounter->m_tagEnv.tLast);

			// ȥ���͵���һ����, �ж��Ƿ񻹿��Ժ���
			if (pCounter->CheckWinNormal(mjHand, TRUE))
			{
				bIsBian = TRUE;
			}
		}
	}
	else if (pCounter->m_tagEnv.tLast % 10 == 7)
	{
		// �жϱ���7
		if (mjHand.IsHave(pCounter->m_tagEnv.tLast + 1) 
		 && mjHand.IsHave(pCounter->m_tagEnv.tLast + 2))
		{
			// �͵������ƺ�����ű������
			mjHand.DelTile(pCounter->m_tagEnv.tLast);
			mjHand.DelTile(pCounter->m_tagEnv.tLast + 1);
			mjHand.DelTile(pCounter->m_tagEnv.tLast + 2);

			// ȥ���͵���һ����, �ж��Ƿ񻹿��Ժ���
			if (pCounter->CheckWinNormal(mjHand, TRUE))
			{
				bIsBian = TRUE;
			}
		}
	}

	// ������Ǳ��ţ�ֱ�ӷ���FALSE
	if (!bIsBian)
	{
		return FALSE;
	}

	// �ж��Ƿ�ֻ�ܺ���һ����(���������������������һ���������ƣ����ԣ������ж�)
	TILE ucTile;
	int nTileNum = mjHand.GetTileNumber(pCounter->m_tagEnv.tLast);
	if (nTileNum > 3)
	{
		// ǰһ����
		ucTile = pCounter->m_tagEnv.tLast - 3;
		mjHand = mjTemp;

		// �ж��Ƿ��ܺ�ǰһ����
		mjHand.DelTile(pCounter->m_tagEnv.tLast);
		mjHand.AddTile(ucTile);

		//����ܺ������ǿ���
		if (pCounter->CheckWinNormal(mjHand, TRUE))
		{
			return FALSE;
		}
	}

	if (nTileNum < 7)
	{
		// ��һ����
		ucTile = pCounter->m_tagEnv.tLast + 3;
		mjHand = mjTemp;

		// �ж��Ƿ��ܺ���һ����
		mjHand.DelTile(pCounter->m_tagEnv.tLast);
		mjHand.AddTile(ucTile);

		//����ܺ������ǿ���
		if (pCounter->CheckWinNormal(mjHand, TRUE))
		{
			return FALSE;
		}
	}

	return TRUE;
}

/**
*  @brief ���� - û������
*/
BOOL CMJFanCounter::Check011(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	CMJHand mjHand;

	// ��ȡ����������(����+����)
	pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

	for (int i = 0; i < mjHand.CurrentLength(); i++)
	{
		if (mjHand.GetTile(i) >= TILE_EAST && mjHand.GetTile(i) <= TILE_BAI)
		{
			// ���ƴ���
			return FALSE;
		}
	}

	return TRUE;
}

/**
*  @brief ȱһ�� - ��ɫ��һ��,���԰�������
*/
BOOL CMJFanCounter::Check012(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	CMJHand mjHand;

	// ��ȡ����������(����+����)
	pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

	// ��¼�����е���Ͳ�����Ƿ����
	BOOL bWan = FALSE, bTong = FALSE, bTiao = FALSE;

	for (int i = 0; i < mjHand.CurrentLength(); i++)
	{
		switch (mjHand.GetTileGenre(mjHand.GetTile(i)))
		{
		case TILE_GENRE_CHAR:   // ��
			{
				bWan = TRUE;
				break; 
			}
		case TILE_GENRE_BALL:   // Ͳ
			{
				bTong = TRUE; 
				break;
			}
		case TILE_GENRE_BAMBOO: // ��
			{
				bTiao = TRUE;
				break;   
			}
		}
	}

	// �ж����ֻ�ɫ״̬
	if (bWan && bTong && !bTiao
	 || bWan && !bTong && bTiao
	 || !bWan && bTong && bTiao)
	{
		// ��һ�ֻ�ɫ������
		return TRUE;
	}

	return FALSE;
}

/**
* @brief �� - �������ﶬ��÷����գ�ÿ����һ�֡���������ͷ��ڣ����ƺ���ܼƷ֡����Ʋ����ɺͼ������֣����Ƹ��Ͽ���
*/
BOOL CMJFanCounter::Check013(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    pCounter->m_tagFanCount.m_FanNode[pCounter->m_byFanID].byFanPoint = pCounter->m_tagEnv.byFlowerCount[pCounter->m_tagEnv.byChair];
    
	return (BOOL)(pCounter->m_tagEnv.byFlowerCount[pCounter->m_tagEnv.byChair] > 0);
}

/**
* @brief ���� - �������ǽ���
*/
BOOL CMJFanCounter::Check014(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;

    // ��ȡ��������
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    if (mjHand.CurrentLength() <= 2)
    {
        // ֻʣ��������, �����ǵ���
        return TRUE;
    }

	//�жϺ͵��������Ƿ����3�Ż�4�ţ�����ǣ��ǾͲ��㵥����
	if (mjHand.IsHave(pCounter->m_tagEnv.tLast, 3) || mjHand.IsHave(pCounter->m_tagEnv.tLast, 4))
	{
		return FALSE;
	}

    // �жϺ͵��������Ƿ����2��
    if (mjHand.IsHave(pCounter->m_tagEnv.tLast, 2))
    {
        // ɾ����һ����
        mjHand.DelTile(pCounter->m_tagEnv.tLast, 2);

        if (pCounter->CheckWinNormal(mjHand, FALSE))
        {
            return TRUE;
        }
    }

	return FALSE;
}

/**
* @brief ���� - �Լ��а��̣������˴����һ����ͬ���ƿ��ܣ����Լ�ץ��һ��������������ͬ���ƿ���
*/
BOOL CMJFanCounter::Check015(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    // ��������벹��
    for (BYTE i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            // ���ܳ���
            return TRUE;
        }
    }

	return FALSE;
}		

/**
* @brief ���� - ��ץ4����ͬ���ƿ���
*/
BOOL CMJFanCounter::Check016(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    // ��ⰵ��
    for (BYTE i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            // ���ܳ���
            return TRUE;
        }
    }

    return FALSE;
}	

/**
*  @brief ���� - һ�ֻ�ɫ6�������ӵ�������
*/
BOOL CMJFanCounter::Check017(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE i, j;
    CMJHand mjSet, mjTemp;

    for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
            mjTemp.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1], 3, TRUE);
        }
    }

    for (i = 0; i < mjTemp.CurrentLength(); i++)
    {
        if (mjTemp.IsHave(mjTemp.GetTile(i), 6, TRUE))
        {
            return TRUE;   // ��������
        }
    }

    CMJHand mjHand;
    TILE tile;

    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);
    mjTemp = mjHand;

    // ����+�����ж��Ƿ�������
    for (i = 0; i < mjSet.CurrentLength(); i++)
    {
        tile = mjSet.GetTile(i);
        mjHand = mjTemp;

        // ���ж�ǰ��
        if (mjHand.IsHave(tile - 3, 3, TRUE))
        {
            mjHand.DelTile(tile - 3, 3, TRUE);

            // ȥ��˳�Ӻ�, �ж��Ƿ񻹿��Ժ���
            if (pCounter->CheckWinNormal(mjHand, TRUE))
            {
                return TRUE; // ��������
            }
        }

        mjHand = mjTemp;

        // ���жϺ���
        if (mjHand.IsHave(tile + 3, 3, TRUE))
        {
            mjHand.DelTile(tile + 3, 3, TRUE);

            // ȥ��˳�Ӻ�, �ж��Ƿ񻹿��Ժ���
            if (pCounter->CheckWinNormal(mjHand, TRUE))
            {
                return TRUE; // ��������
            }
        } 
    }

    // �ж��������Ƿ�������
    for (i = 1; i < 5; i++)
    {
        for (j = 0; j < 3; j++)
        {
            tile = j * 10 + i;
            mjHand = mjTemp;

            if (mjHand.IsHave(tile, 6, TRUE))
            {
                mjHand.DelTile(tile, 6, TRUE);

                // ȥ��˳�Ӻ�, �ж��Ƿ񻹿��Ժ���
                if (pCounter->CheckWinNormal(mjHand, TRUE))
                {
                    return TRUE; // ��������
                }
            }
        }
    }

    return FALSE;
}		

/**
*  @brief ���ٸ� - һ�ֻ�ɫ�Ƶ�123��789����˳�� 
*/
BOOL CMJFanCounter::Check018(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }
	
	int i, j, l;
	CMJHand mjHand, mjTemp, mjSet;
	TILE tile;

	//������ϵ���
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	//��������гԵ���
	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
		}
	}

	//�ֱ��������Ͳ�����ж�
	for (i = 0; i < 3; i++)
	{
		CMJHand mjHand1, mjSet1, mjTemp1, mjTemp2;
		mjHand1.ReleaseAll();
		mjSet1.ReleaseAll();
		mjTemp1.ReleaseAll();

		//�������м��ͬһɫ����
		for (j = 0; j < mjHand.CurrentLength(); j++)
		{
			tile = mjHand.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjHand1.AddTile(tile);
				mjTemp1.AddTile(tile);
			}
		}

		//�ӳ����м��ͬһɫ����
		for (j = 0; j < mjSet.CurrentLength(); j++)
		{
			tile = mjSet.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjSet1.AddTile(tile);
				mjTemp1.AddTile(tile, 3, TRUE);
			}
		}

		//�������С��6�ţ���һ���������ٸ�
		if (mjTemp1.CurrentLength() < 6)
		{
			continue;
		}

		//����Ƿ������ٸ�
		tile = i * 10;
		if (mjTemp1.IsHave(tile + 1, 3, TRUE) && mjTemp1.IsHave(tile + 7, 3, TRUE))
		{
			//�������ٸ�����
			mjTemp2.ReleaseAll();
			mjTemp2.AddTile(tile+1, 3, TRUE);
			mjTemp2.AddTile(tile+7, 3, TRUE);

			//���м�ȥ�Ե��ƣ���ʣ�µ���һ���������е���
			for (l = 0; l < mjSet1.CurrentLength(); l++)
			{
				tile = mjSet1.GetTile(l);
				if (mjTemp2.IsHave(tile, 3, TRUE))
				{
					mjTemp2.DelTile(tile, 3, TRUE);
				}
			}

			//�������м�ȥ��Щ��
			mjTemp = mjHand;
			mjTemp.DelTiles(mjTemp2);
			if (pCounter->CheckWinNormal(mjTemp, TRUE))
			{
				return TRUE;
			}  
		}
	}

	return FALSE;
}		

/**
*  @brief 1ɫ2ͬ˳(һ���) - ��һ�ֻ�ɫ2����ͬ��˳����ɵ���
*/
BOOL CMJFanCounter::Check019(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

	int i, j, k, l;
	CMJHand mjHand, mjTemp, mjSet;
	TILE tile;

	//������ϵ���
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	//��������гԵ���
	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
		}
	}

	//�ֱ��������Ͳ�����ж�
	for (i = 0; i < 3; i++)
	{
		CMJHand mjHand1, mjSet1, mjTemp1, mjTemp2;
		mjHand1.ReleaseAll();
		mjSet1.ReleaseAll();
		mjTemp1.ReleaseAll();

		//�������м��ͬһɫ����
		for (j = 0; j < mjHand.CurrentLength(); j++)
		{
			tile = mjHand.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjHand1.AddTile(tile);
				mjTemp1.AddTile(tile);
			}
		}

		//�ӳ����м��ͬһɫ����
		for (j = 0; j < mjSet.CurrentLength(); j++)
		{
			tile = mjSet.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjSet1.AddTile(tile);
				mjTemp1.AddTile(tile, 3, TRUE);
			}
		}

		//�������С��6�ţ���һ������1ɫ2ͬ˳
		if (mjTemp1.CurrentLength() < 6)
		{
			continue;
		}

		//����Ƿ���1ɫ2ͬ˳
		for(k = 1; k < 8; k++)
		{
			tile = i * 10 + k;
			if (mjTemp1.IsHave(tile, 2) && mjTemp1.IsHave(tile+1, 2) && mjTemp1.IsHave(tile+2, 2))
			{
				//����1ɫ2ͬ˳����
				mjTemp2.ReleaseAll();
				mjTemp2.AddTile(tile, 2);
				mjTemp2.AddTile(tile+1, 2);
				mjTemp2.AddTile(tile+2, 2);

				//���м�ȥ�Ե��ƣ���ʣ�µ���һ���������е���
				for (l = 0; l < mjSet1.CurrentLength(); l++)
				{
					tile = mjSet1.GetTile(l);
					if (mjTemp2.IsHave(tile, 3, TRUE))
					{
						mjTemp2.DelTile(tile, 3, TRUE);
					}
				}

				//�������м�ȥ��Щ��
				mjTemp = mjHand;
				mjTemp.DelTiles(mjTemp2);
				if (pCounter->CheckWinNormal(mjTemp, TRUE))
				{
					return TRUE;
				}  
			}
		}
	}

	return FALSE;
}		

/**
*  @brief 1ɫ3ͬ˳ - ����ʱ��һ�ֻ�ɫ3��������ͬ��˳�ˡ�����һɫ���ڸ� 
*/
BOOL CMJFanCounter::Check020(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }
	
	int i, j, k, l;
	CMJHand mjHand, mjTemp, mjSet;
	TILE tile;

	//������ϵ���
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	//��������гԵ���
	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
		}
	}

	//�ֱ��������Ͳ�����ж�
	for (i = 0; i < 3; i++)
	{
		CMJHand mjHand1, mjSet1, mjTemp1, mjTemp2;
		mjHand1.ReleaseAll();
		mjSet1.ReleaseAll();
		mjTemp1.ReleaseAll();

		//�������м��ͬһɫ����
		for (j = 0; j < mjHand.CurrentLength(); j++)
		{
			tile = mjHand.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjHand1.AddTile(tile);
				mjTemp1.AddTile(tile);
			}
		}

		//�ӳ����м��ͬһɫ����
		for (j = 0; j < mjSet.CurrentLength(); j++)
		{
			tile = mjSet.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjSet1.AddTile(tile);
				mjTemp1.AddTile(tile, 3, TRUE);
			}
		}

		//�������С��9�ţ���һ������1ɫ3ͬ˳
		if (mjTemp1.CurrentLength() < 9)
		{
			continue;
		}

		//����Ƿ���1ɫ3ͬ˳
		for(k = 1; k < 8; k++)
		{
			tile = i * 10 + k;
			if (mjTemp1.IsHave(tile, 3) && mjTemp1.IsHave(tile+1, 3) && mjTemp1.IsHave(tile+2, 3))
			{
				//����1ɫ3ͬ˳����
				mjTemp2.ReleaseAll();
				mjTemp2.AddTile(tile, 3);
				mjTemp2.AddTile(tile+1, 3);
				mjTemp2.AddTile(tile+2, 3);

				//���м�ȥ�Ե��ƣ���ʣ�µ���һ���������е���
				for (l = 0; l < mjSet1.CurrentLength(); l++)
				{
					tile = mjSet1.GetTile(l);
					if (mjTemp2.IsHave(tile, 3, TRUE))
					{
						mjTemp2.DelTile(tile, 3, TRUE);
					}
				}

				//�������м�ȥ��Щ��
				mjTemp = mjHand;
				mjTemp.DelTiles(mjTemp2);
				if (pCounter->CheckWinNormal(mjTemp, TRUE))
				{
					return TRUE;
				}  
			}
		}
	}

    return FALSE;
}	

/**
*  @brief 1ɫ4ͬ˳ - һ�ֻ�ɫ4��������ͬ��˳�ӣ�����һɫ���ڸߡ�һ��ߡ��Ĺ�һ
*/
BOOL CMJFanCounter::Check021(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

	int i, j, k, l;
	CMJHand mjHand, mjTemp, mjSet;
	TILE tile;

	//������ϵ���
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	//��������гԵ���
	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
		}
	}

	//�ֱ��������Ͳ�����ж�
	for (i = 0; i < 3; i++)
	{
		CMJHand mjHand1, mjSet1, mjTemp1, mjTemp2;
		mjHand1.ReleaseAll();
		mjSet1.ReleaseAll();
		mjTemp1.ReleaseAll();

		//�������м��ͬһɫ����
		for (j = 0; j < mjHand.CurrentLength(); j++)
		{
			tile = mjHand.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjHand1.AddTile(tile);
				mjTemp1.AddTile(tile);
			}
		}

		//�ӳ����м��ͬһɫ����
		for (j = 0; j < mjSet.CurrentLength(); j++)
		{
			tile = mjSet.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjSet1.AddTile(tile);
				mjTemp1.AddTile(tile, 3, TRUE);
			}
		}

		//�������С��12�ţ���һ������1ɫ4ͬ˳
		if (mjTemp1.CurrentLength() < 12)
		{
			continue;
		}

		//����Ƿ���1ɫ4ͬ˳
		for(k = 1; k < 8; k++)
		{
			tile = i * 10 + k;
			if (mjTemp1.IsHave(tile, 4) && mjTemp1.IsHave(tile+1, 4) && mjTemp1.IsHave(tile+2, 4))
			{
				//����1ɫ4ͬ˳����
				mjTemp2.ReleaseAll();
				mjTemp2.AddTile(tile, 4);
				mjTemp2.AddTile(tile+1, 4);
				mjTemp2.AddTile(tile+2, 4);

				//���м�ȥ�Ե��ƣ���ʣ�µ���һ���������е���
				for (l = 0; l < mjSet1.CurrentLength(); l++)
				{
					tile = mjSet1.GetTile(l);
					if (mjTemp2.IsHave(tile, 3, TRUE))
					{
						mjTemp2.DelTile(tile, 3, TRUE);
					}
				}

				//�������м�ȥ��Щ��
				mjTemp = mjHand;
				mjTemp.DelTiles(mjTemp2);
				if (pCounter->CheckWinNormal(mjTemp, TRUE))
				{
					return TRUE;
				}  
			}
		}
	}

	return FALSE;
}		

/**
*  @brief 2ɫ2ͬ˳ - ��2�ֻ�ɫ2��������ͬ��˳��
*/
BOOL CMJFanCounter::Check022(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE i;
    CMJHand mjSet;

    for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
        }
    }

    // ���ж���������
    for (i = 1; i <= 7; i++)
    {
        if (mjSet.IsHave(i) && mjSet.IsHave(10 + i) 
         || mjSet.IsHave(i) && mjSet.IsHave(20 + i) 
         || mjSet.IsHave(10 + i) && mjSet.IsHave(20 + i))
        {
            return TRUE;
        }
    }

    CMJHand mjHand, mjTemp;

    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);
    mjTemp = mjHand;

    // �ж���������
    for (i = 1; i <= 7; i++)
    {
        mjHand = mjTemp;
        if (mjHand.IsHave(i, 3, TRUE) && mjHand.IsHave(10 + i, 3, TRUE))
        {
            mjHand.DelTile(i, 3, TRUE);
            mjHand.DelTile(10 + i, 3, TRUE);

            // ȥ��˳�Ӻ�, �ж��Ƿ񻹿��Ժ���
            if (pCounter->CheckWinNormal(mjHand, TRUE))
            {
                return TRUE; // 2ɫ2ͬ˳����
            }
        }

        mjHand = mjTemp;
        if (mjHand.IsHave(i, 3, TRUE) && mjHand.IsHave(20 + i, 3, TRUE))
        {
            mjHand.DelTile(i, 3, TRUE);
            mjHand.DelTile(20 + i, 3, TRUE);

            // ȥ��˳�Ӻ�, �ж��Ƿ񻹿��Ժ���
            if (pCounter->CheckWinNormal(mjHand, TRUE))
            {
                return TRUE; // 2ɫ2ͬ˳����
            }
        }

        mjHand = mjTemp;
        if (mjHand.IsHave(10 + i, 3, TRUE) && mjHand.IsHave(20 + i, 3, TRUE))
        {
            mjHand.DelTile(10 + i, 3, TRUE);
            mjHand.DelTile(20 + i, 3, TRUE);

            // ȥ��˳�Ӻ�, �ж��Ƿ񻹿��Ժ���
            if (pCounter->CheckWinNormal(mjHand, TRUE))
            {
                return TRUE; // 2ɫ2ͬ˳����
            }
        }
    }

    // ���� + ����һ���ж�
    for (i = 1; i <= 7; i++)
    {
        if (mjSet.IsHave(i))
        {
            if (mjHand.IsHave(10 + i, 3, TRUE))
            {
                mjHand.DelTile(10 + i, 3, TRUE);

                // ȥ��˳�Ӻ�, �ж��Ƿ񻹿��Ժ���
                if (pCounter->CheckWinNormal(mjHand, TRUE))
                {
                    return TRUE; // 2ɫ2ͬ˳����
                }
            }

            if (mjHand.IsHave(20 + i, 3, TRUE))
            {
                mjHand.DelTile(20 + i, 3, TRUE);

                // ȥ��˳�Ӻ�, �ж��Ƿ񻹿��Ժ���
                if (pCounter->CheckWinNormal(mjHand, TRUE))
                {
                    return TRUE; // 2ɫ2ͬ˳����
                }
            }
        }
        else if (mjSet.IsHave(10 + i))
        {
            if (mjHand.IsHave(i, 3, TRUE))
            {
                mjHand.DelTile(i, 3, TRUE);

                // ȥ��˳�Ӻ�, �ж��Ƿ񻹿��Ժ���
                if (pCounter->CheckWinNormal(mjHand, TRUE))
                {
                    return TRUE; // 2ɫ2ͬ˳����
                }
            }

            if (mjHand.IsHave(20 + i, 3, TRUE))
            {
                mjHand.DelTile(20 + i, 3, TRUE);

                // ȥ��˳�Ӻ�, �ж��Ƿ񻹿��Ժ���
                if (pCounter->CheckWinNormal(mjHand, TRUE))
                {
                    return TRUE; // 2ɫ2ͬ˳����
                }
            }
        }
        else if (mjSet.IsHave(20 + i))
        {
            if (mjHand.IsHave(i, 3, TRUE))
            {
                mjHand.DelTile(i, 3, TRUE);

                // ȥ��˳�Ӻ�, �ж��Ƿ񻹿��Ժ���
                if (pCounter->CheckWinNormal(mjHand, TRUE))
                {
                    return TRUE; // 2ɫ2ͬ˳����
                }
            }

            if (mjHand.IsHave(10 + i, 3, TRUE))
            {
                mjHand.DelTile(10 + i, 3, TRUE);

                // ȥ��˳�Ӻ�, �ж��Ƿ񻹿��Ժ���
                if (pCounter->CheckWinNormal(mjHand, TRUE))
                {
                    return TRUE; // 2ɫ2ͬ˳����
                }
            }
        }
    }

    return FALSE;
}	

/**
*  @brief 3ɫ3ͬ˳ - ��3�ֻ�ɫ1��������ͬ��˳��
*/
BOOL CMJFanCounter::Check023(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }
	
	int i, l;
	CMJHand mjHand, mjTemp, mjSet;
	TILE tile;

	//������ϵ���
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);
	mjTemp = mjHand;	//��������

	//��������гԵ���
	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
			mjHand.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1], 3, TRUE);	// ���뵽������
		}
	}	

	// ����Ƿ���3ɫ3ͬ˳
	for (i = 1; i < 8; i++)
	{
		if (mjHand.IsHave(i, 3, TRUE) && mjHand.IsHave(10 + i, 3, TRUE) && mjHand.IsHave(20 + i, 3, TRUE))
		{
			//����3ɫ3ͬ˳����
			CMJHand mjTemp1;
			mjTemp1.ReleaseAll();

			mjTemp1.AddTile(i, 3, TRUE);
			mjTemp1.AddTile(10 + i, 3, TRUE);
			mjTemp1.AddTile(20 + i, 3, TRUE);

			//���м�ȥ�Ե��ƣ���ʣ�µ���һ���������е���
			for (l = 0; l < mjSet.CurrentLength(); l++)
			{
				tile = mjSet.GetTile(l);
				if (mjTemp1.IsHave(tile, 3, TRUE))
				{
					mjTemp1.DelTile(tile, 3, TRUE);
				}
			}

			//�������м�ȥ��Щ��
			CMJHand mjHand1;
			mjHand1 = mjTemp;
			mjHand1.DelTiles(mjTemp1);
			if (pCounter->CheckWinNormal(mjHand1, TRUE))
			{
				return TRUE;
			}			
		}
	}

    return FALSE;
}		

/**
*  @brief 19�� - 3����ͬ��һ������������ɵĿ���(���) 
*/
BOOL CMJFanCounter::Check024(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand, mjTemp;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);
    if (mjHand.CurrentLength() != 14)
    {
        return FALSE;
    }

	//��¼19�̵ĸ���
	int nCount = 0;
    // �ж��Ƽ���ֻ��1��9����(������)
	mjTemp = mjHand;
    BYTE byTile[] = {1, 9, 11, 19, 21, 29};

    for (BYTE i = 0; i < 6; i++)
    {
        if (mjHand.IsHave(byTile[i], 3))
        {
			mjHand = mjTemp;
			mjHand.DelTile(byTile[i], 3);
			if (pCounter->CheckWinNormal(mjHand, TRUE))
			{
				nCount++;
			}
        }
    }

	pCounter->m_tagFanCount.m_FanNode[pCounter->m_byFanID].byFanPoint = nCount;

	return (BOOL)(nCount > 0);
}	

BOOL CMJFanCounter::Check025(CMJFanCounter* pCounter)
{
	return FALSE;
}		

/**
*  @brief ˫���� - 2������
*/
BOOL CMJFanCounter::Check026(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE i, byCount = 0;
    TILE tile;
    CMJHand mjHand, mjTemp;
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (i = 0; i < mjHand.CurrentLength(); i++)
    {
        tile = mjHand.GetTile(i);
        if (!mjTemp.IsHave(tile) && mjHand.IsHave(tile, 3))
        {
			// ������������������ƣ�Ҫ�������ж�
			if (tile == pCounter->m_tagEnv.tLast)
			{
				//����������Ǳ��˵��ڵ�
				if (MJ_WIN_GUN == pCounter->m_tagEnv.byFlag        // ����
					|| MJ_WIN_GANG == pCounter->m_tagEnv.byFlag       // ����
					|| MJ_WIN_GANGGIVE == pCounter->m_tagEnv.byFlag)  // ������
				{
					//ֻ����������4ʱ�ſ����ǰ���
					if (mjHand.IsHave(tile, 4))
					{
						mjTemp.AddTile(tile);
					}
				}
				else
				{
					mjTemp.AddTile(tile);
				}
			}
			else
			{
				mjTemp.AddTile(tile);
			}
        }
    }

    for (i = 0; i < mjTemp.CurrentLength(); i++)
    {
        tile = mjTemp.GetTile(i);
        mjHand.DelTile(tile, 3);

        if (pCounter->CheckWinNormal(mjHand, TRUE))
        {
            byCount++;
            
            if (byCount == 2)
            {
                return TRUE;  // ˫���̳���
            }
        }
        else
        {
            mjHand.AddTile(tile, 3);
        }
    }

    return FALSE;
}		

/**
*  @brief 3���� - 3������
*/
BOOL CMJFanCounter::Check027(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE i, byCount = 0;
    TILE tile;
    CMJHand mjHand, mjTemp;

    for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            byCount++;
        }
    }

    if (byCount >= 3)
    {
        return TRUE;
    }

    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (i = 0; i < mjHand.CurrentLength(); i++)
    {
        tile = mjHand.GetTile(i);
        if (!mjTemp.IsHave(tile) && mjHand.IsHave(tile, 3))
        {
			// ������������������ƣ�Ҫ�������ж�
			if (tile == pCounter->m_tagEnv.tLast)
			{
				//����������Ǳ��˵��ڵ�
				if (MJ_WIN_GUN == pCounter->m_tagEnv.byFlag        // ����
					|| MJ_WIN_GANG == pCounter->m_tagEnv.byFlag       // ����
					|| MJ_WIN_GANGGIVE == pCounter->m_tagEnv.byFlag)  // ������
				{
					//ֻ����������4ʱ�ſ����ǰ���
					if (mjHand.IsHave(tile, 4))
					{
						mjTemp.AddTile(tile);
					}
				}
				else
				{
					mjTemp.AddTile(tile);
				}
			}
			else
			{
				mjTemp.AddTile(tile);
			}
        }
    }

    for (i = 0; i < mjTemp.CurrentLength(); i++)
    {
        tile = mjTemp.GetTile(i);
        mjHand.DelTile(tile, 3);

        if (pCounter->CheckWinNormal(mjHand, TRUE))
        {
            byCount++;

            if (byCount == 3)
            {
                return TRUE;  // �����̳���
            }
        }
        else
        {
            mjHand.AddTile(tile, 3);
        }
    }

    return FALSE;
}	

/**
*  @brief 3��� - �������к��������������ƿ̻��
*/
BOOL CMJFanCounter::Check028(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE i, byCount = 0;

    for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            if (pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] >= TILE_EAST
                && pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] <= TILE_NORTH)
            {
                byCount++;
            }
        }
    }

    if (byCount >= 3)
    {
        return TRUE;
    }

    CMJHand mjHand;
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (i = 0; i < 4; i++)
    {
        if (mjHand.IsHave(TILE_EAST + i, 3))
        {
            byCount++;
        }
    }

    if (byCount >= 3)
    {
        return TRUE;
    }

    return FALSE;
}	

/**
*  @brief 2ͬ�� -  2��������ͬ�Ŀ���
*/
BOOL CMJFanCounter::Check029(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }
	
	int i, l;
	CMJHand mjHand, mjTemp, mjSet;
	TILE tile;

	//������ϵ���
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);
	mjTemp = mjHand;	//��������

	//��ҵĿ�����
	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
		|| ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
		|| ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
		|| ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
			mjHand.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1], 3);	// ���뵽������
		}
	}

	// ����Ƿ���2ͬ��
	for (i = 1; i <= 9; i++)
	{
		//����2ͬ�̵���
		CMJHand mjTemp1;

		if (mjHand.IsHave(i, 3) && mjHand.IsHave(10 + i, 3))
		{
			mjTemp1.ReleaseAll();

			mjTemp1.AddTile(i, 3);
			mjTemp1.AddTile(10 + i, 3);

			//���м�ȥ�����ƣ���ʣ�µ���һ���������е���
			for (l = 0; l < mjSet.CurrentLength(); l++)
			{
				tile = mjSet.GetTile(l);
				if (mjTemp1.IsHave(tile, 3))
				{
					mjTemp1.DelTile(tile, 3);
				}
			}

			//�������м�ȥ��Щ��
			CMJHand mjHand1;
			mjHand1 = mjTemp;
			mjHand1.DelTiles(mjTemp1);
			if (pCounter->CheckWinNormal(mjHand1, TRUE))
			{
				return TRUE;
			}			
		}
		else if (mjHand.IsHave(i, 3) && mjHand.IsHave(20 + i, 3))
		{
			mjTemp1.ReleaseAll();

			mjTemp1.AddTile(i, 3);
			mjTemp1.AddTile(20 + i, 3);

			//���м�ȥ�����ƣ���ʣ�µ���һ���������е���
			for (l = 0; l < mjSet.CurrentLength(); l++)
			{
				tile = mjSet.GetTile(l);
				if (mjTemp1.IsHave(tile, 3))
				{
					mjTemp1.DelTile(tile, 3);
				}
			}

			//�������м�ȥ��Щ��
			CMJHand mjHand1;
			mjHand1 = mjTemp;
			mjHand1.DelTiles(mjTemp1);
			if (pCounter->CheckWinNormal(mjHand1, TRUE))
			{
				return TRUE;
			}			
		}
		else if (mjHand.IsHave(10 + i, 3) && mjHand.IsHave(20 + i, 3))
		{
			mjTemp1.ReleaseAll();

			mjTemp1.AddTile(10 + i, 3);
			mjTemp1.AddTile(20 + i, 3);

			//���м�ȥ�����ƣ���ʣ�µ���һ���������е���
			for (l = 0; l < mjSet.CurrentLength(); l++)
			{
				tile = mjSet.GetTile(l);
				if (mjTemp1.IsHave(tile, 3))
				{
					mjTemp1.DelTile(tile, 3);
				}
			}

			//�������м�ȥ��Щ��
			CMJHand mjHand1;
			mjHand1 = mjTemp;
			mjHand1.DelTiles(mjTemp1);
			if (pCounter->CheckWinNormal(mjHand1, TRUE))
			{
				return TRUE;
			}			
		}
	}

    return FALSE;
}

/**
*  @brief 3ͬ�� -  3����ɫ����ͬ�Ŀ��ӣ���Ҳ����Ϊ�ǿ�
*/
BOOL CMJFanCounter::Check030(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }
	
	int i, l;
	CMJHand mjHand, mjTemp, mjSet;
	TILE tile;

	//������ϵ���
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);
	mjTemp = mjHand;	//��������

	//��ҵĿ�����
	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
		|| ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
		|| ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
		|| ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
			mjHand.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1], 3);	// ���뵽������
		}
	}
	
	// ����Ƿ���3ͬ��
	for (i = 1; i <= 9; i++)
	{
		if (mjHand.IsHave(i, 3) && mjHand.IsHave(10 + i, 3) && mjHand.IsHave(20 + i, 3))
		{
			//����3ͬ�̵���
			CMJHand mjTemp1;
			mjTemp1.ReleaseAll();

			mjTemp1.AddTile(i, 3);
			mjTemp1.AddTile(10 + i, 3);
			mjTemp1.AddTile(20 + i, 3);

			//���м�ȥ�����ƣ���ʣ�µ���һ���������е���
			for (l = 0; l < mjSet.CurrentLength(); l++)
			{
				tile = mjSet.GetTile(l);
				if (mjTemp1.IsHave(tile, 3))
				{
					mjTemp1.DelTile(tile, 3);
				}
			}

			//�������м�ȥ��Щ��
			CMJHand mjHand1;
			mjHand1 = mjTemp;
			mjHand1.DelTiles(mjTemp1);
			if (pCounter->CheckWinNormal(mjHand1, TRUE))
			{
				return TRUE;
			}			
		}
	}

	return FALSE;
}

/**
* @brief ������ - ����������,��,�����
*/
BOOL CMJFanCounter::Check031(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    for (int i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            return FALSE;
        }
    }

    CMJHand mjHand;
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    // ����Ƿ�ȫ���Ӻ�
    return pCounter->CheckWinTriplet(mjHand);
}

/**
* @brief �Ĺ�һ - �����У���4����ͬ���ƹ���һ�ҵ�˳�����ӡ��ԡ�������(����������)
*/
BOOL CMJFanCounter::Check032(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE i = 0;
    CMJHand mjSet;
    for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
         || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
         || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
        }
    }

    CMJHand mjHand;
    TILE tile;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (i = 0; i < mjHand.CurrentLength(); i++)
    {
        tile = mjHand.GetTile(i);
        if (mjHand.IsHave(tile, 4) && !mjSet.IsHave(tile))
        {
            return TRUE;   // �Ĺ�һ����
        }
    }

	return FALSE;
}

/**
* @brief �ŷ�� - �뱾�ŷ���ͬ�ķ��
*/
BOOL CMJFanCounter::Check033(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    if (pCounter->m_tagEnv.byPlayerWind == pCounter->m_tagEnv.byChair)
    {
        for (BYTE i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
        {
            if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
             || ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
             || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
             || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
            {
                if (pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] == (TILE_EAST + pCounter->m_tagEnv.byPlayerWind))
                {
                    return TRUE;  // �ŷ�̳���
                }
            }
        }

        CMJHand mjHand;
        pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

        if (mjHand.IsHave(TILE_EAST + pCounter->m_tagEnv.byPlayerWind, 3))
        {
            return TRUE;    // �ŷ�̳���
        }
    }

	return FALSE;
}

/**
* @brief Ȧ��� - ��Ȧ����ͬ�ķ�� 
*/
BOOL CMJFanCounter::Check034(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    for (BYTE i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            if (pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] == (TILE_EAST + pCounter->m_tagEnv.byRoundWind))
            {
                return TRUE;  // Ȧ��̳���
            }
        }
    }

    CMJHand mjHand;
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    if (mjHand.IsHave(TILE_EAST + pCounter->m_tagEnv.byRoundWind, 3))
    {
        return TRUE;    // Ȧ��̳���
    }

    return FALSE;
}

/**
* @brief ���� - ���С�������3����ͬ������ɵĿ��� 
*/
BOOL CMJFanCounter::Check035(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    for (BYTE i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            if (pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] >= TILE_ZHONG 
                && pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] <= TILE_BAI)
            {
                return TRUE;  // ���̳���
            }
        }
    }

    CMJHand mjHand;
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    if (mjHand.IsHave(TILE_ZHONG, 3) || mjHand.IsHave(TILE_FA, 3) || mjHand.IsHave(TILE_BAI, 3))
    {
        return TRUE;    // ���̳���
    }

    return FALSE;
}

/**
* @brief ���� - ���Ƴء�������������3������ʣ�ĵ�4����(���ܺͲ��ƺ;���)
*/
BOOL CMJFanCounter::Check036(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    if (MJ_WIN_GANG == pCounter->m_tagEnv.byFlag)
    {
        return FALSE;  // ����
    }

    BYTE i, j, byCount = 0;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < pCounter->m_tagEnv.bySetCount[i]; j++)
        {
            if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[i][j][0])
            {
                if (pCounter->m_tagEnv.tSet[i][j][1] == pCounter->m_tagEnv.tLast)
                {
                    byCount += 3;
                }
            }
            else if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[i][j][0])
            {
                if (pCounter->m_tagEnv.tLast >= pCounter->m_tagEnv.tSet[i][j][1]
                    && pCounter->m_tagEnv.tLast <= pCounter->m_tagEnv.tSet[i][j][1] + 2)
                {
                    byCount += 1;
                }
            }
        }
    }

    if (byCount >= 4)
    {
        return FALSE;
    }

	//�����Ƴ��е���
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < pCounter->m_tagEnv.byGiveCount[i]; j++)
		{
			if (pCounter->m_tagEnv.tLast == pCounter->m_tagEnv.tGive[i][j])
			{
				byCount++;
			}
		}
	}

	// ����Ƿ���˵��ڡ����ܡ�������
	if (MJ_WIN_GUN == pCounter->m_tagEnv.byFlag        // ����
		|| MJ_WIN_GANGGIVE == pCounter->m_tagEnv.byFlag)  // ������
	{
		if (4 == byCount)
		{
			return TRUE;
		}
	}
	else if (MJ_WIN_SELFDRAW == pCounter->m_tagEnv.byFlag	//����
			|| MJ_WIN_GANGDRAW == pCounter->m_tagEnv.byFlag)  //���ϻ�
	{
		if (3 == byCount)
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

/**
* @brief ˫���� - 2������
*/
BOOL CMJFanCounter::Check037(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE byGangCount = 0;

    for (BYTE i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            byGangCount++;
        }
    }

    if (byGangCount >= 2)
    {
        return TRUE;   // ˫���ܳ���
    }

	return FALSE;
}

/**
* @brief 3�� - 3���� 
*/
BOOL CMJFanCounter::Check038(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE byGangCount = 0;

    for (BYTE i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            byGangCount++;
        }
    }

    if (byGangCount >= 3)
    {
        return TRUE;   // 3�ܳ���
    }

    return FALSE;
}

/**
* @brief 4�� - 4���� 
*/
BOOL CMJFanCounter::Check039(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE byGangCount = 0;

    for (BYTE i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            byGangCount++;
        }
        else
        {
            return FALSE;
        }
    }

    if (byGangCount == 4)
    {
        return TRUE;   // 4�ܳ���
    }
   
	return FALSE;
}		

/**
*  @brief ������ - ����ʱ3�������ơ��硢������ȫ 
*/
BOOL CMJFanCounter::Check040(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BOOL bGenre[5] = { FALSE };
    TILE tile;
    CMJHand mjHand;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);
    
    for (BYTE i = 0; i < mjHand.CurrentLength(); i++)
    {
        tile = mjHand.GetTile(i);
        if (!bGenre[0] && tile >= 1 && tile <= 9)
        {
            bGenre[0] = TRUE;  // ��
        }
        else if (!bGenre[1] && tile >= 11 && tile <= 19)
        {
            bGenre[1] = TRUE;  // ��
        }
        else if (!bGenre[2] && tile >= 21 && tile <= 29)
        {
            bGenre[2] = TRUE;  // Ͳ
        }
        else if (!bGenre[3] && tile >= 31 && tile <= 34)
        {
            bGenre[3] = TRUE;  // ��
        }
        else if (!bGenre[4] && tile >= 35 && tile <= 37)
        {
            bGenre[4] = TRUE;  // ��
        }
    }

    if (bGenre[0] && bGenre[1] && bGenre[2] && bGenre[3] && bGenre[4])
    {
        return TRUE;  // ���������
    }

	return FALSE;
}

/**
*  @brief 1ɫ3���� - ����ʱ����һ�ֻ�ɫ3�����ε���һλ�����ε�����λ���ֵ�˳��
*/
BOOL CMJFanCounter::Check041(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }
	
	int i, j, k, l;
	CMJHand mjHand, mjTemp, mjSet;
	TILE tile;

	//������ϵ���
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	//��������гԵ���
	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
		}
	}
	
	//�ֱ��������Ͳ�����ж�
	for (i = 0; i < 3; i++)
	{
		CMJHand mjHand1, mjSet1, mjTemp1, mjTemp2;
		mjHand1.ReleaseAll();
		mjSet1.ReleaseAll();
		mjTemp1.ReleaseAll();

		//�������м��ͬһɫ����
		for (j = 0; j < mjHand.CurrentLength(); j++)
		{
			tile = mjHand.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjHand1.AddTile(tile);
				mjTemp1.AddTile(tile);
			}
		}

		//�ӳ����м��ͬһɫ����
		for (j = 0; j < mjSet.CurrentLength(); j++)
		{
			tile = mjSet.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjSet1.AddTile(tile);
				mjTemp1.AddTile(tile, 3, TRUE);
			}
		}

		//�������С��9�ţ���һ������һɫ������
		if (mjTemp1.CurrentLength() < 9)
		{
			continue;
		}

		//����Ƿ���һɫ������
		//���ε���1λʱ����567 678 789
		for(k = 1; k < 6; k++)
		{
			tile = i * 10 + k;
			if (mjTemp1.IsHave(tile) && mjTemp1.IsHave(tile+1, 2) && mjTemp1.IsHave(tile+2, 3) && mjTemp1.IsHave(tile+3, 2) && mjTemp1.IsHave(tile+4))
			{
				//����һɫ�����ߵ���
				mjTemp2.ReleaseAll();
				mjTemp2.AddTile(tile);
				mjTemp2.AddTile(tile+1, 2);
				mjTemp2.AddTile(tile+2, 3);
				mjTemp2.AddTile(tile+3, 2);
				mjTemp2.AddTile(tile+4);

				//���м�ȥ�Ե��ƣ���ʣ�µ���һ���������е���
				for (l = 0; l < mjSet1.CurrentLength(); l++)
				{
					tile = mjSet1.GetTile(l);
					if (mjTemp2.IsHave(tile, 3, TRUE))
					{
						mjTemp2.DelTile(tile, 3, TRUE);
					}
				}

				//�������м�ȥ��Щ��
				mjTemp = mjHand;
				mjTemp.DelTiles(mjTemp2);
				if (pCounter->CheckWinNormal(mjTemp, TRUE))
				{
					return TRUE; // һɫ�����߳���
				}  
			}
		}

		//���ε���2λʱ����345 567 789
		for (k = 1; k < 4; k++)
		{
			tile = i * 10 + k;
			if (mjTemp1.IsHave(tile) && mjTemp1.IsHave(tile+1) && mjTemp1.IsHave(tile+2, 2) && mjTemp1.IsHave(tile+3) && mjTemp1.IsHave(tile+4, 2) && mjTemp1.IsHave(tile+5) && mjTemp1.IsHave(tile+6))
			{
				//����һɫ�����ߵ���
				mjTemp2.ReleaseAll();
				mjTemp2.AddTile(tile);
				mjTemp2.AddTile(tile+1);
				mjTemp2.AddTile(tile+2, 2);
				mjTemp2.AddTile(tile+3);
				mjTemp2.AddTile(tile+4, 2);
				mjTemp2.AddTile(tile+5);
				mjTemp2.AddTile(tile+6);

				//���м�ȥ�Ե��ƣ���ʣ�µ���һ���������е���
				for (l = 0; l < mjSet1.CurrentLength(); l++)
				{
					tile = mjSet1.GetTile(l);
					if (mjTemp2.IsHave(tile, 3, TRUE))
					{
						mjTemp2.DelTile(tile, 3, TRUE);
					}
				}

				//�������м�ȥ��Щ��
				mjTemp = mjHand;
				mjTemp.DelTiles(mjTemp2);
				if (pCounter->CheckWinNormal(mjTemp, TRUE))
				{
					return TRUE; // һɫ�����߳���
				}  
			}
		}
	}
	
	return FALSE;
}		

/**
*  @brief 1ɫ4���� - һ�ֻ�ɫ4�����ε���һλ�������ε�����λ����˳�� 
*/
BOOL CMJFanCounter::Check042(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }
	
	int i, j, k, l;
	CMJHand mjHand, mjTemp, mjSet;
	TILE tile;

	//������ϵ���
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	//��������гԵ���
	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
		}
	}
	
	//�ֱ��������Ͳ�����ж�
	for (i = 0; i < 3; i++)
	{
		CMJHand mjHand1, mjSet1, mjTemp1, mjTemp2;
		mjHand1.ReleaseAll();
		mjSet1.ReleaseAll();
		mjTemp1.ReleaseAll();

		//�������м��ͬһɫ����
		for (j = 0; j < mjHand.CurrentLength(); j++)
		{
			tile = mjHand.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjHand1.AddTile(tile);
				mjTemp1.AddTile(tile);
			}
		}

		//�ӳ����м��ͬһɫ����
		for (j = 0; j < mjSet.CurrentLength(); j++)
		{
			tile = mjSet.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjSet1.AddTile(tile);
				mjTemp1.AddTile(tile, 3, TRUE);
			}
		}

		//�������С��12�ţ���һ������һɫ�Ĳ���
		if (mjTemp1.CurrentLength() < 12)
		{
			continue;
		}

		//����Ƿ���һɫ�Ĳ���
		//���ε���1λʱ����456 
		//                  567 
		//                   678 
		//                    789
		for(k = 1; k < 5; k++)
		{
			tile = i * 10 + k;
			if (mjTemp1.IsHave(tile) && mjTemp1.IsHave(tile+1, 2) && mjTemp1.IsHave(tile+2, 3) && mjTemp1.IsHave(tile+3, 3) && mjTemp1.IsHave(tile+4, 2) && mjTemp1.IsHave(tile+5))
			{
				//����һɫ�����ߵ���
				mjTemp2.ReleaseAll();
				mjTemp2.AddTile(tile);
				mjTemp2.AddTile(tile+1, 2);
				mjTemp2.AddTile(tile+2, 3);
				mjTemp2.AddTile(tile+3, 3);
				mjTemp2.AddTile(tile+4, 2);
				mjTemp2.AddTile(tile+5);

				//���м�ȥ�Ե��ƣ���ʣ�µ���һ���������е���
				for (l = 0; l < mjSet1.CurrentLength(); l++)
				{
					tile = mjSet1.GetTile(l);
					if (mjTemp2.IsHave(tile, 3, TRUE))
					{
						mjTemp2.DelTile(tile, 3, TRUE);
					}
				}

				//�������м�ȥ��Щ��
				mjTemp = mjHand;
				mjTemp.DelTiles(mjTemp2);
				if (pCounter->CheckWinNormal(mjTemp, TRUE))
				{
					return TRUE; // һɫ�Ĳ��߳���
				}  
			}
		}

		//���ε���2λʱ����123 345 567 789
		tile = i * 10 + 1;
		if (mjTemp1.IsHave(tile) && mjTemp1.IsHave(tile+1) && mjTemp1.IsHave(tile+2, 2) && mjTemp1.IsHave(tile+3) && mjTemp1.IsHave(tile+4, 2) && mjTemp1.IsHave(tile+5) && mjTemp1.IsHave(tile+6, 2) &&  mjTemp1.IsHave(tile+7) &&  mjTemp1.IsHave(tile+8))
		{
			//����һɫ�����ߵ���
			mjTemp2.ReleaseAll();
			mjTemp2.AddTile(tile);
			mjTemp2.AddTile(tile+1);
			mjTemp2.AddTile(tile+2, 2);
			mjTemp2.AddTile(tile+3);
			mjTemp2.AddTile(tile+4, 2);
			mjTemp2.AddTile(tile+5);
			mjTemp2.AddTile(tile+6, 2);
			mjTemp2.AddTile(tile+7);
			mjTemp2.AddTile(tile+8);

			//���м�ȥ�Ե��ƣ���ʣ�µ���һ���������е���
			for (l = 0; l < mjSet1.CurrentLength(); l++)
			{
				tile = mjSet1.GetTile(l);
				if (mjTemp2.IsHave(tile, 3, TRUE))
				{
					mjTemp2.DelTile(tile, 3, TRUE);
				}
			}

			//�������м�ȥ��Щ��
			mjTemp = mjHand;
			mjTemp.DelTiles(mjTemp2);
			if (pCounter->CheckWinNormal(mjTemp, TRUE))
			{
				return TRUE; // һɫ�Ĳ��߳���
			}  
		}
	}

    return FALSE;
}

/**
*  @brief 3ɫ3���� - ���ֻ�ɫ���Ƶ��������ε���һλ�ĸ�һ��˳�ӣ���ͼ֮567��678��789����Ҫע�������λ���㣨��234��456��678�� 
*/
BOOL CMJFanCounter::Check043(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }
	
	int i, j, l;
	CMJHand mjHand, mjTemp, mjSet;
	TILE tile;

	//������ϵ���
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);
	mjTemp = mjHand;	//��������

	//��������гԵ���
	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
			mjHand.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1], 3, TRUE);	// ���뵽������
		}
	}	

	// ��������
	BYTE byWinMode[6][3] = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2,}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};


	// ����Ƿ���3ɫ3����
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 6; j++)
		{
			if (mjHand.IsHave(byWinMode[j][0] * 10 + i, 3, TRUE) 
				&& mjHand.IsHave(byWinMode[j][1] * 10 + i + 1, 3, TRUE) 
				&& mjHand.IsHave(byWinMode[j][2] * 10 + i + 2, 3, TRUE))
			{
				//����3ɫ3���ߵ���
				CMJHand mjTemp1;
				mjTemp1.ReleaseAll();

				mjTemp1.AddTile(byWinMode[j][0] * 10 + i, 3, TRUE);
				mjTemp1.AddTile(byWinMode[j][1] * 10 + i + 1, 3, TRUE);
				mjTemp1.AddTile(byWinMode[j][2] * 10 + i + 2, 3, TRUE);

				//���м�ȥ�Ե��ƣ���ʣ�µ���һ���������е���
				for (l = 0; l < mjSet.CurrentLength(); l++)
				{
					tile = mjSet.GetTile(l);
					if (mjTemp1.IsHave(tile, 3, TRUE))
					{
						mjTemp1.DelTile(tile, 3, TRUE);
					}
				}

				//�������м�ȥ��Щ��
				CMJHand mjHand1;
				mjHand1 = mjTemp;
				mjHand1.DelTiles(mjTemp1);
				if (pCounter->CheckWinNormal(mjHand1, TRUE))
				{
					return TRUE;
				} 
			}
		}
	}

    return FALSE;
}

/**
*  @brief ˫���� - 2������
*/
BOOL CMJFanCounter::Check044(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE byCount = 0;
    for (BYTE i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            byCount++;
        }
    }

    if (byCount >= 2)
    {
        return TRUE;
    }

    return FALSE;
}

/**
*  @brief ���� - �����Ǳ��˲����������ƣ�ͨ����������MJ_WIN_GANG�������Ƿ�������
*/
BOOL CMJFanCounter::Check045(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	if (MJ_WIN_GANG == pCounter->m_tagEnv.byFlag)
	{
		return TRUE;
	}

	return FALSE;
}

/**
*  @brief ���ϻ� - ����֮������
*/
BOOL CMJFanCounter::Check046(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	if (MJ_WIN_GANGDRAW == pCounter->m_tagEnv.byFlag)
	{
		return TRUE;
	}

	return FALSE;
}

/**
*  @brief ������ - ������Ҹ���֮����һ���ƶ������Լ��ں�
*/
BOOL CMJFanCounter::Check047(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	if (MJ_WIN_GANGGIVE == pCounter->m_tagEnv.byFlag)
	{
		return TRUE;
	}

	return FALSE;
}

/**
*  @brief �����ں� - �ʹ�������һ����
*/
BOOL CMJFanCounter::Check048(CMJFanCounter* pCounter)
{
    if (pCounter->m_tagEnv.byTilesLeft == 0)
    {
        if (MJ_WIN_GUN == pCounter->m_tagEnv.byFlag        // ����
         || MJ_WIN_GANG == pCounter->m_tagEnv.byFlag       // ����
         || MJ_WIN_GANGGIVE == pCounter->m_tagEnv.byFlag)  // ������
        {
            return TRUE;
        }
    }

	return FALSE;
}		

/**
*  @brief �������� - �����һ����
*/
BOOL CMJFanCounter::Check049(CMJFanCounter* pCounter)
{
    if (pCounter->m_tagEnv.byTilesLeft == 0)
    {
        if (MJ_WIN_SELFDRAW == pCounter->m_tagEnv.byFlag   // ����
         || MJ_WIN_GANGDRAW == pCounter->m_tagEnv.byFlag)  // ���ϻ�
        {
            return TRUE;
        }
    }

	return FALSE;
}		

/**
*  @brief �Ƶ��� - 
*/
BOOL CMJFanCounter::Check050(CMJFanCounter* pCounter)
{
	return FALSE;
}	

/**
*  @brief �Ʋ��� - ������ͼ��û���������������ɵĺ��ƣ�����1234589����245689�����װ塣����ȱһ�� 
*/
BOOL CMJFanCounter::Check051(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand, mjTemp;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

    // ����Ʋ�������ֵ
    mjTemp.AddTile(TILE_BALL_1, 5, TRUE);   // 1-5Ͳ
    mjTemp.AddTile(TILE_BALL_8, 2, TRUE);   // 8-9Ͳ
    mjTemp.AddTile(TILE_BAMBOO_2);          // 2��
    mjTemp.AddTile(TILE_BAMBOO_4, 3, TRUE); // 4-6��
    mjTemp.AddTile(TILE_BAMBOO_8, 2, TRUE); // 8-9��
    mjTemp.AddTile(TILE_BAI);               // �װ�
    
    for (BYTE i = 0; i < mjHand.CurrentLength(); i++)
    {
        if (!mjTemp.IsHave(mjHand.GetTile(i)))
        {
            return FALSE;
        }
    }

	return TRUE;
}		

/**
*  @brief ���� - 3�ֻ�ɫ��3��˳�����ӳ�1-9��������
*/
BOOL CMJFanCounter::Check052(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }
	
	int i, j;
	CMJHand mjHand, mjTemp, mjSet;
	TILE tile;

	//������ϵ���
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);
	mjTemp = mjHand;	//��������

	//��������гԵ���
	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
			mjHand.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1], 3, TRUE);	// ���뵽������
		}
	}	

	// ��������
	BYTE byWinMode[6][3] = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2,}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};


	// ����Ƿ��л���
	for (i = 0; i < 6; i++)		
	{
		if (mjHand.IsHave(byWinMode[i][0] * 10 + 1, 3, TRUE) 
			&& mjHand.IsHave(byWinMode[i][1] * 10 + 4, 3, TRUE) 
			&& mjHand.IsHave(byWinMode[i][2] * 10 + 7, 3, TRUE))
		{
			//���㻨������
			CMJHand mjTemp1;
			mjTemp1.ReleaseAll();

			mjTemp1.AddTile(byWinMode[i][0] * 10 + 1, 3, TRUE);
			mjTemp1.AddTile(byWinMode[i][1] * 10 + 4, 3, TRUE);
			mjTemp1.AddTile(byWinMode[i][2] * 10 + 7, 3, TRUE);

			//���м�ȥ��1��4��7��ͷ�ĳ��ƣ���ʣ�µ���һ����������
			for (j = 0; j < mjSet.CurrentLength(); j++)
			{
				tile = mjSet.GetTile(j);
				if (((tile % 10) % 3 == 1) && mjTemp1.IsHave(tile, 3, TRUE))
				{
					mjTemp1.DelTile(tile, 3, TRUE);
				}
			}

			//�������м�ȥ��Щ��
			CMJHand mjHand1;
			mjHand1 = mjTemp;
			mjHand1.DelTiles(mjTemp1);
			if (pCounter->CheckWinNormal(mjHand1, TRUE))
			{
				return TRUE;
			} 
		}
	}	

    return FALSE;
}		

/**
*  @brief ���� - ����ʱ����һ�ֻ�ɫ1-9�����ӵ������� 
*/
BOOL CMJFanCounter::Check053(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

	int i, j, l;
	CMJHand mjHand, mjTemp, mjSet;
	TILE tile;

	//������ϵ���
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	//��ҳԵ���
	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
		}
	}

	//�ֱ��������Ͳ�����ж�
	for (i = 0; i < 3; i++)
	{
		CMJHand mjHand1, mjSet1, mjTemp1, mjTemp2;
		mjHand1.ReleaseAll();
		mjSet1.ReleaseAll();
		mjTemp1.ReleaseAll();

		//�������м��ͬһɫ����
		for (j = 0; j < mjHand.CurrentLength(); j++)
		{
			tile = mjHand.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjHand1.AddTile(tile);
				mjTemp1.AddTile(tile);
			}
		}

		//�ӳ����м��ͬһɫ����
		for (j = 0; j < mjSet.CurrentLength(); j++)
		{
			tile = mjSet.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjSet1.AddTile(tile);
				mjTemp1.AddTile(tile, 3, TRUE);
			}
		}

		//�������С��9�ţ���һ���������� 
		if (mjTemp1.CurrentLength() < 9)
		{
			continue;
		}

		//����Ƿ�������		
		tile = i * 10;
		if (mjTemp1.IsHave(tile + 1, 3, TRUE) && mjTemp1.IsHave(tile + 4, 3, TRUE) && mjTemp1.IsHave(tile + 7, 3, TRUE))
		{
			//������������
			mjTemp2.ReleaseAll();
			mjTemp2.AddTile(tile + 1, 3, TRUE);
			mjTemp2.AddTile(tile + 4, 3, TRUE);
			mjTemp2.AddTile(tile + 7, 3, TRUE);

			//���м�ȥ��1��4��7��ͷ�ĳ��ƣ���ʣ�µ���һ����������
			for (l = 0; l < mjSet1.CurrentLength(); l++)
			{
				tile = mjSet1.GetTile(l);
				if (((tile % 10) % 3 == 1) && mjTemp2.IsHave(tile, 3, TRUE))
				{
					mjTemp2.DelTile(tile, 3, TRUE);
				}
			}

			//�������м�ȥ��Щ��
			mjTemp = mjHand;
			mjTemp.DelTiles(mjTemp2);
			if (pCounter->CheckWinNormal(mjTemp, TRUE))
			{
				return TRUE; 
			}  
		}		
	}

	return FALSE;
}		

/**
*  @brief ȫС - ��������123��ɵ�˳�ӡ�����(��)���Ƶĵĺ��ơ��������� 
*/
BOOL CMJFanCounter::Check054(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (BYTE i = 0; i < mjHand.CurrentLength(); i++)
    {
        if (mjHand.GetTileNumber(mjHand.GetTile(i)) > 3 || mjHand.GetTile(i) >= TILE_EAST)
        {
            return FALSE;
        }
    }

	return TRUE;
}		

/**
*  @brief ȫ�� - ��������456��ɵ�˳�ӡ�����(��)�����Ƶĺ��ơ����ƶ��� 
*/
BOOL CMJFanCounter::Check055(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (BYTE i = 0; i < mjHand.CurrentLength(); i++)
    {
        if (mjHand.GetTileNumber(mjHand.GetTile(i)) <= 3 || mjHand.GetTileNumber(mjHand.GetTile(i)) >= 7 || mjHand.GetTile(i) >= TILE_EAST)
        {
            return FALSE;
        }
    }

    return TRUE;
}		

/**
*  @brief ȫ�� - ��������789��ɵ�˳�ˡ�����(��)�����Ƶĺ��ơ��������� 
*/
BOOL CMJFanCounter::Check056(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (BYTE i = 0; i < mjHand.CurrentLength(); i++)
    {
        if (mjHand.GetTileNumber(mjHand.GetTile(i)) < 7 || mjHand.GetTile(i) >= TILE_EAST)
        {
            return FALSE;
        }
    }

    return TRUE;
}	

/**
*  @brief ��ɫһͬ˳ - 
*/
BOOL CMJFanCounter::Check057(CMJFanCounter* pCounter)
{
	return FALSE;
}	

/**
*  @brief ȫ˫ - ��������2,4,6,8���, ���Ʋ���
*/
BOOL CMJFanCounter::Check058(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;
    TILE tile;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (BYTE i = 0; i < mjHand.CurrentLength(); i++)
    {
        tile = mjHand.GetTile(i);
        if (tile >= TILE_EAST && tile <= TILE_BAI)
        {
            return FALSE;  // ���Ʋ�����
        }

        if (mjHand.GetTileNumber(tile) % 2 != 0)
        {
            return FALSE;
        }
    }

    return TRUE;
}		

/**
*  @brief ȫ�� - ��������1,3,5,7,9���, ���Ʋ���
*/
BOOL CMJFanCounter::Check059(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;
    TILE tile;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (BYTE i = 0; i < mjHand.CurrentLength(); i++)
    {
        tile = mjHand.GetTile(i);
        if (tile >= TILE_EAST && tile <= TILE_BAI)
        {
            return FALSE;  // ���Ʋ�����
        }

        if (mjHand.GetTileNumber(tile) % 2 != 1)
        {
            return FALSE;
        }
    }

    return TRUE;
}		

/**
*  @brief ȫ���� - �ɵ���3�ֻ�ɫ147��258��369���ܴ�λ�������Ƽ����������з�����
*/
BOOL CMJFanCounter::Check060(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    if (pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair] > 0)
    {
        return FALSE;  // �����г�����
    }

    CMJHand mjHand, mjTemp;
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

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
*  @brief 1ɫ3�ڸ� - ����ʱ��һ�ֻ�ɫ3�����ε���һλ���ֵĿ��ӡ�����һɫ��ͬ˳
*/
BOOL CMJFanCounter::Check061(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }
	
	int i, j, k, l;
	CMJHand mjHand, mjTemp, mjSet;
	TILE tile;

	//������ϵ���
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	//��������еĿ���
	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
		|| ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
		|| ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
		|| ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
		}
	}

	//�ֱ��������Ͳ�����ж�
	for (i = 0; i < 3; i++)
	{
		CMJHand mjHand1, mjSet1, mjTemp1, mjTemp2;
		mjHand1.ReleaseAll();
		mjSet1.ReleaseAll();
		mjTemp1.ReleaseAll();

		//�������м��ͬһɫ����
		for (j = 0; j < mjHand.CurrentLength(); j++)
		{
			tile = mjHand.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjHand1.AddTile(tile);
				mjTemp1.AddTile(tile);
			}
		}

		//�������м��ͬһɫ����
		for (j = 0; j < mjSet.CurrentLength(); j++)
		{
			tile = mjSet.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjSet1.AddTile(tile);
				mjTemp1.AddTile(tile, 3);
			}
		}

		//�������С��9�ţ���һ������1ɫ3�ڸ� 
		if (mjTemp1.CurrentLength() < 9)
		{
			continue;
		}

		//����Ƿ���1ɫ3�ڸ�
		for(k = 1; k < 8; k++)
		{
			tile = i * 10 + k;
			if (mjTemp1.IsHave(tile, 3) && mjTemp1.IsHave(tile+1, 3) && mjTemp1.IsHave(tile+2, 3))
			{
				//����1ɫ3�ڸߵ���
				mjTemp2.ReleaseAll();
				mjTemp2.AddTile(tile, 3);
				mjTemp2.AddTile(tile+1, 3);
				mjTemp2.AddTile(tile+2, 3);

				//���м�ȥ�����ƣ���ʣ�µ���һ���������е���
				for (l = 0; l < mjSet1.CurrentLength(); l++)
				{
					tile = mjSet1.GetTile(l);
					if (mjTemp2.IsHave(tile, 3))
					{
						mjTemp2.DelTile(tile, 3);
					}
				}

				//�������м�ȥ��Щ��
				mjTemp = mjHand;
				mjTemp.DelTiles(mjTemp2);
				if (pCounter->CheckWinNormal(mjTemp, TRUE))
				{
					return TRUE; // 1ɫ3�ڸ߳���
				}  
			}
		}
	}

	return FALSE;
}		

/**
*  @brief 1ɫ4�ڸ� - һ�ֻ�ɫ4�����ε���һλ���Ŀ��ӡ�����һɫ��ͬ˳��������  
*/
BOOL CMJFanCounter::Check062(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

	int i, j, k, l;
	CMJHand mjHand, mjTemp, mjSet;
	TILE tile;

	//������ϵ���
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	//��������еĿ���
	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
		|| ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
		|| ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
		|| ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			mjSet.AddTile(pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1]);
		}
	}

	//�ֱ��������Ͳ�����ж�
	for (i = 0; i < 3; i++)
	{
		CMJHand mjHand1, mjSet1, mjTemp1, mjTemp2;
		mjHand1.ReleaseAll();
		mjSet1.ReleaseAll();
		mjTemp1.ReleaseAll();

		//�������м��ͬһɫ����
		for (j = 0; j < mjHand.CurrentLength(); j++)
		{
			tile = mjHand.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjHand1.AddTile(tile);
				mjTemp1.AddTile(tile);
			}
		}

		//�������м��ͬһɫ����
		for (j = 0; j < mjSet.CurrentLength(); j++)
		{
			tile = mjSet.GetTile(j);
			if ((tile > i * 10) && (tile < (i + 1) * 10))
			{
				mjSet1.AddTile(tile);
				mjTemp1.AddTile(tile, 3);
			}
		}

		//�������С��12�ţ���һ������1ɫ4�ڸ� 
		if (mjTemp1.CurrentLength() < 12)
		{
			continue;
		}

		//����Ƿ���1ɫ4�ڸ�
		for(k = 1; k < 7; k++)
		{
			tile = i * 10 + k;
			if (mjTemp1.IsHave(tile, 3) && mjTemp1.IsHave(tile+1, 3) && mjTemp1.IsHave(tile+2, 3) && mjTemp1.IsHave(tile+3, 3))
			{
				//����1ɫ4�ڸߵ���
				mjTemp2.ReleaseAll();
				mjTemp2.AddTile(tile, 3);
				mjTemp2.AddTile(tile+1, 3);
				mjTemp2.AddTile(tile+2, 3);
				mjTemp2.AddTile(tile+3, 3);

				//���м�ȥ�����ƣ���ʣ�µ���һ���������е���
				for (l = 0; l < mjSet1.CurrentLength(); l++)
				{
					tile = mjSet1.GetTile(l);
					if (mjTemp2.IsHave(tile, 3))
					{
						mjTemp2.DelTile(tile, 3);
					}
				}

				//�������м�ȥ��Щ��
				mjTemp = mjHand;
				mjTemp.DelTiles(mjTemp2);
				if (pCounter->CheckWinNormal(mjTemp, TRUE))
				{
					return TRUE; // 1ɫ4�ڸ߳���
				}  
			}
		}
	}

    return FALSE;
}

/**
*  @brief 3ɫ3�ڸ� - ����ʱ����3�ֻ�ɫ3�����ε���һλ���Ŀ���
*/
BOOL CMJFanCounter::Check063(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE i, j;
    CMJHand mjHand, mjTemp;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);
    mjTemp = mjHand;

    // ��������
    BYTE byWinMode[6][3] = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2,}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};

    // �ж���������
    for (i = 1; i <= 7; i++)
    {
        for (j = 0; j < 6; j++)
        {
            mjHand = mjTemp;
            if (mjHand.IsHave(byWinMode[j][0] * 10 + i, 3) && mjHand.IsHave(byWinMode[j][1] * 10 + i + 1, 3) && mjHand.IsHave(byWinMode[j][2] * 10 + i + 2, 3))
            {
                mjHand.DelTile(byWinMode[j][0] * 10 + i, 3);
                mjHand.DelTile(byWinMode[j][1] * 10 + i + 1, 3);
                mjHand.DelTile(byWinMode[j][2] * 10 + i + 2, 3);

                if (pCounter->CheckWinNormal(mjHand, TRUE))
                {
                    return TRUE;
                }
            }
        }
    }

	return FALSE;
}

/**
*  @brief ��һɫ - �����ƵĿ���(��)������ɵĺ��ơ����������� 
*/
BOOL CMJFanCounter::Check064(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (BYTE i = 0; i < 7; i++)
    {
        if (mjHand.IsHave(TILE_EAST + i, 3))
        {
            mjHand.DelTile(TILE_EAST + i, 3);
        }
    }

    if (mjHand.CurrentLength() == 2 && mjHand.IsHave(mjHand.GetTile(0), 2) && mjHand.GetTile(0) >= TILE_EAST && mjHand.GetTile(0) <= TILE_BAI)
    {
        return TRUE;   // ��һɫ����
    }

    return FALSE;
}		

/**
*  @brief С��Ԫ - ����ʱ�м��Ƶ��������Ӽ����ơ����Ƽ��� 
*/
BOOL CMJFanCounter::Check065(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE i, byCount = 0;
    BOOL bFlag[3] = {FALSE};  // ��ʶ�з��׿�(��)�Ƿ����
    BOOL bJiang = FALSE;      // ��ʶ�з��׽���

    for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            if (pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] >= TILE_ZHONG
                && pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] <= TILE_BAI)
            {
                // ��ʶ�з���ĳ�ֿ���(��)����
                bFlag[pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] - TILE_ZHONG] = TRUE;
            }
        }
    }

    CMJHand mjHand;
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (i = 0; i < 3; i++)
    {
        if (mjHand.IsHave(TILE_ZHONG + i))
        {
            if (mjHand.GetTileCount(TILE_ZHONG + i) == 3)
            {
                bFlag[i] = TRUE;
            }
            else if (mjHand.GetTileCount(TILE_ZHONG + i) == 2)
            {
                bJiang = TRUE;
            }
        }

        if (bFlag[i])
        {
            byCount++;
        }
    }

    if (byCount == 2 && bJiang)
    {
        return TRUE;  // С��Ԫ����
    }

    return FALSE;
}		

/**
*  @brief ����Ԫ - �����У����з���3�����ӡ����Ʒ�� 
*/
BOOL CMJFanCounter::Check066(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE i;
    BOOL bFlag[3] = {FALSE};  // ��ʶ�з��׿�(��)�Ƿ����

    for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            if (pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] >= TILE_ZHONG
               && pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] <= TILE_BAI)
            {
                // ��ʶ�з���ĳ�ֿ���(��)����
                bFlag[pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] - TILE_ZHONG] = TRUE;
            }
        }
    }

    CMJHand mjHand;
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (i = 0; i < 3; i++)
    {
        if (mjHand.IsHave(TILE_ZHONG + i))
        {
            if (mjHand.GetTileCount(TILE_ZHONG + i) == 3)
            {
                bFlag[i] = TRUE;
            }
        }
    }

    if (bFlag[0] && bFlag[1] && bFlag[2])
    {
        return TRUE;  // ����Ԫ����
    }

	return FALSE;
}	

/**
*  @brief С��ϲ - ����ʱ�з��Ƶ�3�����Ӽ����ơ���������� 
*/
BOOL CMJFanCounter::Check067(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE i, byCount = 0;
    BOOL bFlag[4] = {FALSE}; // ��ʶ����������(��)�Ƿ����
    BOOL bIsJiang = FALSE;   // ��ʶ������������

    for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            if (pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] >= TILE_EAST
                && pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] <= TILE_NORTH)
            {
                // ��ʶ����������ĳ�ֿ���(��)����
                bFlag[pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] - TILE_EAST] = TRUE;
            }
        }
    }

    CMJHand mjHand;
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (i = 0; i < 4; i++)
    {
        if (mjHand.IsHave(TILE_EAST + i))
        {
            if (mjHand.GetTileCount(TILE_EAST + i) == 3)
            {
                bFlag[i] = TRUE;
            }
            else if (mjHand.GetTileCount(TILE_EAST + i) == 2)
            {
                bIsJiang = TRUE;
            }
        }

        if (bFlag[i])
        {
            byCount++;
        }
    }

    if (byCount == 3 && bIsJiang)
    {
        return TRUE;  // С��ϲ����
    }

    return FALSE;
}		

/**
*  @brief ����ϲ - ��4�����(��)��ɵĺ��ơ�����Ȧ��̡��ŷ�̡�����̡�������
*/
BOOL CMJFanCounter::Check068(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE i;
    BOOL bFlag[4] = {FALSE};  // ��ʶ����������(��)�Ƿ����

    for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
           if (pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] >= TILE_EAST
               && pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] <= TILE_NORTH)
           {
               // ��ʶ����������ĳ�ֿ���(��)����
               bFlag[pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] - TILE_EAST] = TRUE;
           }
        }
        else
        {
            return FALSE;
        }
    }

    CMJHand mjHand;
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (i = 0; i < 4; i++)
    {
        if (mjHand.IsHave(TILE_EAST + i))
        {
            if (mjHand.GetTileCount(TILE_EAST + i) == 3)
            {
                bFlag[i] = TRUE;
            }
        }
    }

    if (bFlag[0] && bFlag[1] && bFlag[2] && bFlag[3])
    {
        return TRUE;  // ����ϲ����
    }

	return FALSE;
}	

/**
*  @brief ʮ���� - ��3�������Ƶ�һ�����ƣ�7�����Ƽ�����һ��������ɵĺ��ơ����������롢�����ˡ����� 
*/
BOOL CMJFanCounter::Check069(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    if (mjHand.CurrentLength() != 14)
    {
        return FALSE;  // ������������, ����Ϊ����14��
    }

    CMJHand mjTemp;
    BOOL bIsPair = FALSE;  // �Ƿ��ж�
    BYTE byTile[] = {1, 9, 11, 19, 21, 29, 31, 32, 33, 34, 35, 36, 37};
    for (BYTE i = 0; i < 13; i++)
    {
        mjTemp.AddTile(byTile[i]);

        if (mjHand.GetTileCount(byTile[i]) == 2)
        {
            bIsPair = TRUE;
        }
    }

    if (mjHand.IsSubSet(mjTemp) && bIsPair)
    {
        return TRUE;   // ʮ���۳���
    }

	return FALSE;
}		

/** 
*  @brief �������� - ��һ�ֻ�ɫ�������Ӱ�1112345678999��ɵ��ض����ͣ���ͬ��ɫ�κ�1�������Ƽ��ɺ��ơ�������һɫ 
*/
BOOL CMJFanCounter::Check070(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    if (mjHand.CurrentLength() != 14)
    {
        return FALSE;  // ������������, ����Ϊ����14��
    }

    BYTE i;
    BYTE byGenre = mjHand.GetTileGenre(mjHand.GetTile(0));

    for (i = 1; i < mjHand.CurrentLength(); i++)
    {
        if (byGenre != mjHand.GetTileGenre(mjHand.GetTile(i)))
        {
            return FALSE;   // ���Ƶ���������ɫ��ͳһ
        }
    }

    CMJHand mjTemp;
    BYTE byTile[] = {1, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9};
    for (i = 0; i < 13; i++)
    {
        mjTemp.AddTile(byTile[i] + byGenre * 10);
    }

    if (mjHand.IsSubSet(mjTemp))
    {
        return TRUE;  // �������Ƴ���
    }

	return FALSE;
}	

/**
*  @brief ��һɫ - ��23468���������е��κ�����ɵ�˳�ӡ����塢���ĺ��ơ����ƻ�һɫ������"��"����ɵĸ��ƣ��ɼ���һɫ
*/
BOOL CMJFanCounter::Check071(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }
  
    CMJHand mjHand;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

    TILE tile;

    // �ж������е�����
    for (BYTE i = 0; i < mjHand.CurrentLength(); i++)
    {
        tile = mjHand.GetTile(i);

        if (tile != TILE_FA
            || tile != TILE_BAMBOO_2
            || tile != TILE_BAMBOO_3
            || tile != TILE_BAMBOO_4
            || tile != TILE_BAMBOO_6
            || tile != TILE_BAMBOO_8)
        {
            // ������
            return FALSE;
        }
    }

    return TRUE;
}	

/**
* @brief С��5 - ��������1-4��˳�ӡ����ӡ�������ɵĺ��ơ��������� 
*/
BOOL CMJFanCounter::Check072(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;

    // ��ȡ��������
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (BYTE i = 0; i < mjHand.CurrentLength(); i++)
    {
        if (mjHand.GetTileNumber(mjHand.GetTile(i)) > 4)
        {
            return FALSE;
        }
    }

    return TRUE;
}	

/**
* @brief ����5 - ��������6-9��˳�ӡ����ӡ�������ɵĺ��ơ���������
*/
BOOL CMJFanCounter::Check073(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;

    // ��ȡ��������
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

    for (BYTE i = 0; i < mjHand.CurrentLength(); i++)
    {
        if (mjHand.GetTileNumber(mjHand.GetTile(i)) < 6)
        {
            return FALSE;
        }
    }

	return TRUE;
}		

/**
* @brief ����� - 3�ֻ�ɫ��147��258��369���ܴ�λ�������� 
*/
BOOL CMJFanCounter::Check074(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    //if (pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair] > 0)
    //{
    //    return FALSE;  // �����г�����
    //}

    BYTE i;
    CMJHand mjHand, mjTemp, mjSet;

    // ��������
    BYTE byWinMode[6][3] = { \
        0, 1, 2, \
        0, 2, 1, \
        1, 0, 2, \
        1, 2, 0, \
        2, 0, 1, \
        2, 1, 0};

    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);
    mjTemp = mjHand;

    // ����Լ��������Ƿ������
    for (i = 0; i < 6; i++)
    {
        mjHand = mjTemp;

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

            if (pCounter->CheckWinNormal(mjHand, TRUE))
            {
                return TRUE; // �����
            } 

			//�ж�ȫ�������Ƿ���������
			if (5 == mjHand.CurrentLength())
			{
				BOOL bIsPair = FALSE;
				BOOL bIsZi = TRUE;
				for (int j = 0; j < mjHand.CurrentLength(); j++)
				{
					TILE tile = mjHand.GetTile(j);
					if (tile < TILE_EAST || tile > TILE_BAI)
					{
						bIsZi = FALSE;
					}
					if(mjHand.IsHave(tile, 2))
					{
						bIsPair = TRUE;
					}
				}

				//���ȫ�����ƣ���û�жԣ���Ϊȫ���������
				if (bIsZi && (!bIsPair))
				{
					return TRUE;
				}
			}
        }
    }

    return FALSE;
}		

/**
* @brief ���ǲ��� - ȫ�����������У�����������ȫ��Ϊ���ǲ�����������ϲμ����ǲ���
*/
BOOL CMJFanCounter::Check075(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	if (pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair] > 0)
	{
		return FALSE;  // �����г�����
	}

	CMJHand mjHand, mjTemp;
	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	if (mjHand.CurrentLength() != 14)
	{
		return FALSE;  // ������������, ����Ϊ����14��
	}

	BYTE i, j;
	for (i = 0; i < mjHand.CurrentLength(); i++)
	{
		if (mjHand.IsHave(mjHand.GetTile(i), 2))
		{
			return FALSE;   // ȫ���������ж�
		}
	}

	BYTE byTile[] = {31, 32, 33, 34, 35, 36, 37};
	for (i = 0; i < 7; i++)
	{
		mjTemp.AddTile(byTile[i]);
	}
	//�����������
	if (!mjHand.IsSubSet(mjTemp))
	{
		return FALSE;   
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

		if (mjHand.CurrentLength() > 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}		

/**
* @brief ȫ��5 - ÿ���Ƽ����Ʊ�����5�������ơ����ƶ��� 
*/
BOOL CMJFanCounter::Check076(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    for (BYTE i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            if (pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] % 10 != 5)
            {
                return FALSE;   // û�д�5
            }
        }
        else if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            if (5 < pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] % 10
            || 5 > pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] % 10 + 2)
            {
                return FALSE;   // û�д�5
            }
        }
    }

    CMJHand mjHand;

    // ��ȡ��������
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    if (pCounter->CheckWinWithX(mjHand, 5))
    {
        return TRUE;
    }

	return FALSE;
}		


/**
* @brief ˫�� - ׯ�����������������а���֮�������
*/
BOOL CMJFanCounter::Check077(CMJFanCounter* pCounter)
{
	return FALSE;
}

/**
* @brief ��� - ׯ�����������������а���֮�������
*/
BOOL CMJFanCounter::Check078(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    if (pCounter->m_tagEnv.byFlag != MJ_WIN_SELFDRAW)
    {
        // ������
        return FALSE;
    }

    for (char i = 0; i < 4; i++)
    {
        // ����������
        if (pCounter->m_tagEnv.bySetCount[i] > 0)
        {
            return FALSE;
        }

        // ������һ�ų�������
        if (pCounter->m_tagEnv.byGiveCount[i] > 0)
        {
            return FALSE;
        }
    }

    CMJHand mjHand;

    // ��ȡ��������
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    if (mjHand.CurrentLength() % 3 == 2)
    {
        return TRUE;
    }

	return FALSE;
}

/**
* @brief �غ� - ����ׯ�����ֺ�ׯ�ҵ���
*/
BOOL CMJFanCounter::Check079(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    if (pCounter->m_tagEnv.byFlag != MJ_WIN_GUN)
    {
        // �ǵ���
        return FALSE;
    }

    if (pCounter->m_tagEnv.byTurn != pCounter->m_tagEnv.ucDealer)
    {
        // �����ڵ��˲���ׯ��
        return FALSE;
    }

    if (pCounter->m_tagEnv.byGiveCount[pCounter->m_tagEnv.byChair] > 0)
    {
        // ���Ƶ���ұ���û�г���һ����
        return FALSE;
    }

    for (char i = 0; i < 4; i++)
    {
        // ����������
        if (pCounter->m_tagEnv.bySetCount[i] > 0)
        {
            return FALSE;
        }
    }

    CMJHand mjHand;

    // ��ȡ��������
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    if (mjHand.CurrentLength() % 3 == 2)
    {
        return TRUE;
    }

    return FALSE;
}

/**
* @brief �˺� - ����ׯ����������
*/
BOOL CMJFanCounter::Check080(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    if (pCounter->m_tagEnv.byFlag != MJ_WIN_SELFDRAW)
    {
        // ������
        return FALSE;
    }

    if (pCounter->m_tagEnv.byChair == pCounter->m_tagEnv.ucDealer)
    {
        // ���Ƶ��˱��벻��ׯ��
        return FALSE;
    }

    if (pCounter->m_tagEnv.byGiveCount[pCounter->m_tagEnv.byChair] > 0)
    {
        // ���Ƶ���ұ���û�г���һ����
        return FALSE;
    }

    for (char i = 0; i < 4; i++)
    {
        // ����������
        if (pCounter->m_tagEnv.bySetCount[i] > 0)
        {
            return FALSE;
        }
    }

    CMJHand mjHand;

    // ��ȡ��������
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    if (mjHand.CurrentLength() % 3 == 2)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CMJFanCounter::Check081(CMJFanCounter* pCounter)
{
	return FALSE;
}				
BOOL CMJFanCounter::Check082(CMJFanCounter* pCounter)
{
	return FALSE;
}				
BOOL CMJFanCounter::Check083(CMJFanCounter* pCounter)
{
	return FALSE;
}			
BOOL CMJFanCounter::Check084(CMJFanCounter* pCounter)
{
	return FALSE;
}	

/**
* @brief ˫���� - 2������, ���С�������3����ͬ������ɵĿ���(���)
*/
BOOL CMJFanCounter::Check085(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    BYTE byCount = 0;
    for (BYTE i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            if (pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] >= TILE_ZHONG 
                && pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] <= TILE_BAI)
            {
                byCount++;
            }
        }
    }

    if (byCount >= 2)
    {
        return TRUE;
    }

    CMJHand mjHand;
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	for (TILE tile = TILE_ZHONG; tile <= TILE_BAI; tile++)
	{
		if (mjHand.IsHave(tile, 3))
		{
			byCount++;
		}
	}

    if (byCount >= 2)
    {
        return TRUE;
    }

    return FALSE;
}

/**
*  @brief һɫ˫���� - һ�ֻ�ɫ���������ٸ���5Ϊ���ơ�����ƽ�����߶ԡ���һɫ 
*/
BOOL CMJFanCounter::Check086(CMJFanCounter* pCounter)
{
    // ���Ƹ�ʽ: 11223355778899
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

    BYTE i;
    BYTE byGenre = mjHand.GetTileGenre(mjHand.GetTile(0));
    for (i = 1; i < mjHand.CurrentLength(); i++)
    {
        if (byGenre != mjHand.GetTileGenre(mjHand.GetTile(i)))
        {
            return FALSE;   // ���Ƶ���������ɫ��ͳһ
        }
    }

    BYTE byTile[] = {1, 1, 2, 2, 3, 3, 5, 5, 7, 7, 8, 8, 9, 9};
    CMJHand mjTemp;
    for (i = 0; i < 14; i++)
    {
        mjTemp.AddTile(byTile[i] + byGenre * 10);
    }

    if (mjHand.IsSubSet(mjTemp))
    {
        return TRUE;  // һɫ˫�������
    }

	return FALSE;
}		

/**
*  @brief ��7�� - ��һ�ֻ�ɫ�������������������7�����ӵĺ��ơ�������һɫ�������ˡ����� 
*/
BOOL CMJFanCounter::Check087(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    if (mjHand.CurrentLength() != 14)
    {
        return FALSE;  // ������������, ����Ϊ����14��
    }

    BYTE i;
    BYTE byGenre = mjHand.GetTileGenre(mjHand.GetTile(0));
    TILE tile;
    CMJHand mjTemp;

    for (i = 1; i < mjHand.CurrentLength(); i++)
    {
        tile = mjHand.GetTile(i);
        if (byGenre != mjHand.GetTileGenre(tile))
        {
            return FALSE;   // ���Ƶ���������ɫ��ͳһ
        }

        if ((!mjTemp.IsHave(tile)) && (mjHand.GetTileCount(tile) == 2))
        {
            mjTemp.AddTile(tile);
        }
    }

    if (mjTemp.CurrentLength() != 7)
    {
        return FALSE;
    }

    mjTemp.Sort();
    if (mjTemp.IsHave(mjTemp.GetTile(0), 7, TRUE))
    {
        return TRUE;   // ��7�Գ���
    }
    
	return FALSE;
}	

/**
*  @brief ���� - ���ƺ󲻿����ٻ��ƣ����п��ܸܡ�������һ��������������������Ʒ֣��������ƶ��޷����ļ�����Ӱ��
*/
BOOL CMJFanCounter::Check088(CMJFanCounter* pCounter)
{
	return pCounter->m_tagEnv.byTing[pCounter->m_tagEnv.byChair] > 0;
}				
BOOL CMJFanCounter::Check089(CMJFanCounter* pCounter)
{
	return FALSE;
}				

/**
*  @brief ��7�� - �߶�����4����ͬ�Ķ������߶�
*/
BOOL CMJFanCounter::Check090(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	if (Check002(pCounter))
	{
		CMJHand mjHand;

		// ��ȡ��������
		pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);
		
		for (BYTE i = 0; i < mjHand.CurrentLength(); i++)
		{
			if (mjHand.IsHave(mjHand.GetTile(i), 4))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CMJFanCounter::Check091(CMJFanCounter* pCounter)
{
	return FALSE;
}			

/**
*  @brief �ں� - �ں������ܡ������ڶ����ں�
*/
BOOL CMJFanCounter::Check092(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	// ��������	
	if (MJ_WIN_GUN == pCounter->m_tagEnv.byFlag        // ����
	 || MJ_WIN_GANG == pCounter->m_tagEnv.byFlag       // ����
	 || MJ_WIN_GANGGIVE == pCounter->m_tagEnv.byFlag)  // ������
	{
		return TRUE;
	}

	return FALSE;
}				
BOOL CMJFanCounter::Check093(CMJFanCounter* pCounter)
{
	return FALSE;
}				
BOOL CMJFanCounter::Check094(CMJFanCounter* pCounter)
{
	return FALSE;
}				
BOOL CMJFanCounter::Check095(CMJFanCounter* pCounter)
{
	return FALSE;
}

/**
*  @brief ׯ - ������Ϊׯ��ʱ
*/
BOOL CMJFanCounter::Check096(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    if (pCounter->m_tagEnv.byChair == pCounter->m_tagEnv.ucDealer)
    {
        return TRUE;
    }

	return FALSE;
}

/**
*  @brief ����� - ����ȫ���ǳԡ��������ܡ�����, ������ֻ����2��
*/
BOOL CMJFanCounter::Check097(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	// �ж������Ƿ�Ϊһ�Խ���, �����Ƿ���4������
	if (pCounter->m_tagEnv.byHandCount[pCounter->m_tagEnv.byChair] == 2 
     && pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair] == 4)
	{
		// ������ֻ��2�Ž��� �� ����������4������
		return TRUE;
	}

	return FALSE;
}

/**
*  @brief ���۾� - ��������һ���ſ�����ɵĺ��ơ����������͡�ͬ�̡����� 
*/
BOOL CMJFanCounter::Check098(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);
    if (mjHand.CurrentLength() != 14)
    {
        return FALSE;
    }

    // �ж��Ƽ���ֻ��1��9����
    BYTE byTile[] = {1, 9, 11, 19, 21, 29};
    for (BYTE i = 0; i < 6; i++)
    {
        if (mjHand.IsHave(byTile[i], 3))
        {
            mjHand.DelTile(byTile[i], 3);
        }
        else if (mjHand.IsHave(byTile[i], 2))
        {
            mjHand.DelTile(byTile[i], 2);
        }
    }

    if (mjHand.CurrentLength() <= 0)
    {
        return TRUE;  // ���۾ų���
    }

    return FALSE;
}

/**
*  @brief �İ��� - 
*/
BOOL CMJFanCounter::Check099(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	BYTE i, byCount = 0;
	TILE tile;
	CMJHand mjHand, mjTemp;

	for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
	{
		if (ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
		{
			byCount++;
		}
	}

	if (byCount >= 4)
	{
		return TRUE;
	}

	pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

	for (i = 0; i < mjHand.CurrentLength(); i++)
	{
		tile = mjHand.GetTile(i);
		if (!mjTemp.IsHave(tile) && mjHand.IsHave(tile, 3))
		{
			// ������������������ƣ�Ҫ�������ж�
			if (tile == pCounter->m_tagEnv.tLast)
			{
				//����������Ǳ��˵��ڵ�
				if (MJ_WIN_GUN == pCounter->m_tagEnv.byFlag        // ����
					|| MJ_WIN_GANG == pCounter->m_tagEnv.byFlag       // ����
					|| MJ_WIN_GANGGIVE == pCounter->m_tagEnv.byFlag)  // ������
				{
					//ֻ����������4ʱ�ſ����ǰ���
					if (mjHand.IsHave(tile, 4))
					{
						mjTemp.AddTile(tile);
					}
				}
				else
				{
					mjTemp.AddTile(tile);
				}
			}
			else
			{
				mjTemp.AddTile(tile);
			}
		}
	}

	for (i = 0; i < mjTemp.CurrentLength(); i++)
	{
		tile = mjTemp.GetTile(i);
		mjHand.DelTile(tile, 3);

		if (pCounter->CheckWinNormal(mjHand, TRUE))
		{
			byCount++;

			if (byCount == 4)
			{
				return TRUE;  // �İ��̳���
			}
		}
		else
		{
			mjHand.AddTile(tile, 3);
		}
	}

	return FALSE;
}

/**
*  @brief ϲ��� - 2�ֻ�ɫ2��������ͬ��˳�� 
*/
BOOL CMJFanCounter::Check100(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }
    
    CMJHand mjSet; // ��¼˳�ӵ�ͷ
    TILE tile;
    BYTE i, j, byGenre, byNumber;
    BYTE byNum[49] = {0};

    for (i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            tile = pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1];
            byNum[tile] = 1;
            mjSet.AddTile(tile);
        }
    }

    for (i = 1; i < 8; i++)
    {
        if ((byNum[i] + byNum[10 + i] + byNum[20 + i]) >= 2)
        {
            return TRUE;   // ϲ������
        }
    }

    CMJHand mjHand, mjTemp;

    // ��ȡ��������
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    if (mjHand.CurrentLength() < 5)
    {
        return FALSE;
    }

    mjTemp = mjHand;

    // �����������ж�ϲ���
    for (i = 0; i < mjSet.CurrentLength(); i++)
    {
        tile = mjSet.GetTile(i);
        byGenre = mjSet.GetTileGenre(tile);
        byNumber = mjSet.GetTileNumber(tile);

        for (j = 0; j < 3; j++)
        {
            if (j == byGenre)
            {
                continue ;
            }

            mjHand = mjTemp;

			if (mjHand.IsHave(j * 10 + byNumber, 3, TRUE))
			{
				mjHand.DelTile(j * 10 + byNumber, 3, TRUE);

				// �ж��Ƿ񻹿��Ժ���
				if (pCounter->CheckWinNormal(mjHand, TRUE))
				{
					return TRUE;  // ϲ������
				}
			}
        }
    }


    // �����ж�ϲ���
    for (i = 1; i < 8; i++)
    {
        if (mjHand.IsHave(i, 3, TRUE) && mjHand.IsHave(10 + i, 3, TRUE))
        {
            mjHand = mjTemp;
            mjHand.DelTile(i, 3, TRUE);
            mjHand.DelTile(10 + i, 3, TRUE);

            // �ж��Ƿ񻹿��Ժ���
            if (pCounter->CheckWinNormal(mjHand, TRUE))
            {
                return TRUE;  // ϲ������
            }
        }
        
        if (mjHand.IsHave(i, 3, TRUE) && mjHand.IsHave(20 + i, 3, TRUE))
        {
            mjHand = mjTemp;
            mjHand.DelTile(i, 3, TRUE);
            mjHand.DelTile(20 + i, 3, TRUE);

            // �ж��Ƿ񻹿��Ժ���
            if (pCounter->CheckWinNormal(mjHand, TRUE))
            {
                return TRUE;  // ϲ������
            }
        }
        
        if (mjHand.IsHave(10 + i, 3, TRUE) && mjHand.IsHave(20 + i, 3, TRUE))
        {
            mjHand = mjTemp;
            mjHand.DelTile(10 + i, 3, TRUE);
            mjHand.DelTile(20 + i, 3, TRUE);

            // �ж��Ƿ񻹿��Ժ���
            if (pCounter->CheckWinNormal(mjHand, TRUE))
            {
                return TRUE;  // ϲ������
            }
        }
    }

    return FALSE;
}			
	
/**
*  @brief ��ǰ�� - û�гԡ��������ܣ��ͱ��˴������
*/
BOOL CMJFanCounter::Check101(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    // ���ԡ������ܵ��Ʊ�־
    for (int i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_QUADRUPLET_CONCEALED != pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        && ACTION_QUADRUPLET_DNXB != pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        && ACTION_QUADRUPLET_ZFB != pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            // �����в��ǰ���, ʧ��
            return FALSE;
        }
    }

    // ��������	
    if (MJ_WIN_GUN == pCounter->m_tagEnv.byFlag        // ����
     || MJ_WIN_GANG == pCounter->m_tagEnv.byFlag       // ����
     || MJ_WIN_GANGGIVE == pCounter->m_tagEnv.byFlag)  // ������
    {
        return TRUE;
    }

    return FALSE;
}

/**
* @brief ȫ���� - ����ʱ��ÿ���ơ����ƶ�������
*/
BOOL CMJFanCounter::Check102(CMJFanCounter* pCounter)
{
    if (NULL == pCounter)
    {
        return FALSE;
    }

    for (BYTE i = 0; i < pCounter->m_tagEnv.bySetCount[pCounter->m_tagEnv.byChair]; i++)
    {
        if (ACTION_COLLECT == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_TRIPLET == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_CONCEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_PATCH == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0]
        || ACTION_QUADRUPLET_REVEALED == pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][0])
        {
            if (pCounter->m_tagEnv.tSet[pCounter->m_tagEnv.byChair][i][1] % 10 != 1)
            {
                return FALSE;   // û�д�1
            }
        }
    }

    CMJHand mjHand;

    // ��ȡ����������
    pCounter->GetHandTiles(mjHand, pCounter->m_tagEnv.byChair);

    if (pCounter->CheckWinWithX(mjHand, 1))
    {
        return TRUE;
    }

	return FALSE;
}		

/**
* @brief ��ɫ˫���� - 2�ֻ�ɫ2�����ٸ�����һ�ֻ�ɫ5�����ĺ��ơ�����ϲ��ꡢ���ٸ������֡�ƽ��
*/
BOOL CMJFanCounter::Check103(CMJFanCounter* pCounter)
{
    // ���Ƹ�ʽ: 11223355778899
    if (NULL == pCounter)
    {
        return FALSE;
    }

    CMJHand mjHand;
    pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);

    // ��������
    BYTE byWinMode[6][3] = {{0, 1, 2}, {0, 2, 1}, {1, 0, 2,}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};
    for (BYTE i = 0; i < 6; i++)
    {
        if (mjHand.IsHave(byWinMode[i][0] * 10 + 1, 3, TRUE) && mjHand.IsHave(byWinMode[i][0] * 10 + 7, 3, TRUE)
         && mjHand.IsHave(byWinMode[i][1] * 10 + 5, 2)                                                         
         && mjHand.IsHave(byWinMode[i][2] * 10 + 1, 3, TRUE) && mjHand.IsHave(byWinMode[i][2] * 10 + 7, 3, TRUE))
        {
            return TRUE;
        }
    }

    return FALSE;
}		

/**
*  @brief 19�� - 3����ͬ�ķ�����ɵĿ���(���) 
*/
BOOL CMJFanCounter::Check104(CMJFanCounter* pCounter)
{
	if (NULL == pCounter)
	{
		return FALSE;
	}

	CMJHand mjHand;
	pCounter->GetAllTiles(mjHand, pCounter->m_tagEnv.byChair);
	if (mjHand.CurrentLength() != 14)
	{
		return FALSE;
	}

	//��¼19�̵ĸ���
	int nCount = 0;
	// �ж��Ƽ���ֻ�з��ƴ���
	BYTE byTile[] = {31, 32, 33, 34};

	for (BYTE i = 0; i < 4; i++)
	{
		if (mjHand.IsHave(byTile[i], 3))
		{
			nCount++;
		}
	}

	pCounter->m_tagFanCount.m_FanNode[pCounter->m_byFanID].byFanPoint = nCount;

	return (BOOL)(nCount > 0);
}

/**
*  @brief ƨ�� - �޷���
*/
BOOL CMJFanCounter::Check105(CMJFanCounter* pCounter)
{
	// ����Ҫ�κ��ж�, ���˾���
	return TRUE;
}

BOOL CMJFanCounter::Check106(CMJFanCounter* pCounter)
{
	return FALSE;
}
BOOL CMJFanCounter::Check107(CMJFanCounter* pCounter)
{
	return FALSE;
}
BOOL CMJFanCounter::Check108(CMJFanCounter* pCounter)
{
	return FALSE;
}
BOOL CMJFanCounter::Check109(CMJFanCounter* pCounter)
{
	return FALSE;
}

BOOL CMJFanCounter::Check110(CMJFanCounter* pCounter)
{
	return FALSE;
}				
BOOL CMJFanCounter::Check111(CMJFanCounter* pCounter)
{
	return FALSE;
}			
BOOL CMJFanCounter::Check112(CMJFanCounter* pCounter)
{
	return FALSE;
}				
BOOL CMJFanCounter::Check113(CMJFanCounter* pCounter)
{
	return FALSE;
}				
BOOL CMJFanCounter::Check114(CMJFanCounter* pCounter)
{
	return FALSE;
}				
BOOL CMJFanCounter::Check115(CMJFanCounter* pCounter)
{
	return FALSE;
}
BOOL CMJFanCounter::Check116(CMJFanCounter* pCounter)
{
	return FALSE;
}
BOOL CMJFanCounter::Check117(CMJFanCounter* pCounter)
{
	return FALSE;
}
BOOL CMJFanCounter::Check118(CMJFanCounter* pCounter)
{
	return FALSE;
}
BOOL CMJFanCounter::Check119(CMJFanCounter* pCounter)
{
	return FALSE;
}

BOOL CMJFanCounter::Check120(CMJFanCounter* pCounter)
{
	return FALSE;
}				
BOOL CMJFanCounter::Check121(CMJFanCounter* pCounter)
{
	return FALSE;
}			
BOOL CMJFanCounter::Check122(CMJFanCounter* pCounter)
{
	return FALSE;
}				
BOOL CMJFanCounter::Check123(CMJFanCounter* pCounter)
{
	return FALSE;
}				
BOOL CMJFanCounter::Check124(CMJFanCounter* pCounter)
{
	return FALSE;
}				
BOOL CMJFanCounter::Check125(CMJFanCounter* pCounter)
{
	return FALSE;
}
BOOL CMJFanCounter::Check126(CMJFanCounter* pCounter)
{
	return FALSE;
}
BOOL CMJFanCounter::Check127(CMJFanCounter* pCounter)
{
	return FALSE;
}
}