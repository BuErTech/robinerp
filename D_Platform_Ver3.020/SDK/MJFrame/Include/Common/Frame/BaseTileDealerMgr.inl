// BaseTileDealerMgr.cpp: implementation of the CBaseTileDealerMgr class.
//
//////////////////////////////////////////////////////////////////////

// #include "BaseTileDealerMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _WINDOWS
   
#else
	#include <sys/time.h>
#endif

namespace MyGame
{

template<int nPlayerNbr>
CBaseTileDealerMgr<nPlayerNbr>::CBaseTileDealerMgr()
{
	m_TilesDealer.ReleaseAll();
	m_TilesGod.ReleaseAll();
	m_TilesDraw.ReleaseAll();
	m_TilesGang.ReleaseAll();

	memset( m_nDice, 0, sizeof(m_nDice) );
    
	m_nWindRoundNbr = 0;
	m_nWindRound = 0;
	m_nDealer = 0; 
	m_nNowTurn = 0;

// 	m_bConfigTiles = FALSE;
	ClearFlower();
	ClearRunNbr();

	SetLastGiveChair( nPlayerNbr );

	SetTileEvent((IMJTileEvent*)this);
}

template<int nPlayerNbr>
CBaseTileDealerMgr<nPlayerNbr>::~CBaseTileDealerMgr()
{

}
	
template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::OnTileEvent( int nEventID )
{

}

/************************************************************************/
/* ��ʼ��������                                                                     */
/************************************************************************/
template<int nPlayerNbr>
int CBaseTileDealerMgr<nPlayerNbr>::InitDealer()
{
// 	m_TilesDealer.FillTiles( m_tagMJSetting.nTilesNumber, 1 );
// 	m_TilesDealer.RandomTiles();
// 	RandomDice();
	return 0;
}

template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::RandomDealer()
{
	m_TilesDealer.RandomTiles();
}

template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::SendTilesToPlayer()
{
}

template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::DrawTile( int nTurn )
{

}

// template<int nPlayerNbr>
// void CBaseTileDealerMgr<nPlayerNbr>::SetMJSetting( MJ_SETTING_DATA& mjdata )
// {
// 	memcpy( &m_tagMJSetting, &mjdata, sizeof(m_tagMJSetting) );
// }


template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::OnDealerPostGameEnd()
{
	m_TilesDealer.ReleaseAll();
	m_TilesDraw.ReleaseAll();
	m_TilesGang.ReleaseAll();
}

template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::ResetWindRound()
{
	m_nWindRound = 0;
	m_nWindRoundNbr = 0;
}

template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::InitForNext()
{
// 	InitDealer();
// 	ClearFlower();

	// ת����
	m_nWindRoundNbr++;
	if(m_nWindRoundNbr == 4)
	{
		m_nWindRoundNbr = 0;
		m_nWindRound++;
	}

	if(m_nWindRound == 4)
	{
		m_nWindRound = 0;
	}

	SetLastGiveChair( nPlayerNbr );
	CBaseTilePlayerMgr<nPlayerNbr>::InitForNext();

}

template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::SetTurn( int nTurn )
{
	if ( nTurn >= 0 && nTurn < nPlayerNbr )
	{
		m_nNowTurn = nTurn;
		SetShowTurn( nTurn );
	}
}

template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::SetShowTurn( int nTurn )
{
	m_nShowTurn = nTurn;
}

template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::RandomDice()
{
    for ( int i = 0; i < DICE_NBR; ++i )
    {
        m_nDice[i] = ( rand() % 6 ) + 1;
    }
}

template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::SetWindRound( int nRound )
{
	if ( nRound >= 0 && nRound < nPlayerNbr )
		m_nWindRound = nRound;
}

template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::SetWindRoundNbr( int nNbr )
{
	if ( nNbr >= 0 )
		m_nWindRoundNbr = nNbr;
}

template<int nPlayerNbr>
int CBaseTileDealerMgr<nPlayerNbr>::GetDice( int nIndex )
{
	if ( nIndex < 0 || nIndex >= DICE_NBR )
		return 1;

	return m_nDice[nIndex];
}

template<int nPlayerNbr>
int CBaseTileDealerMgr<nPlayerNbr>::GetTilesWall( CMJWall& mjWall )
{
	mjWall.ReleaseAll();
	mjWall.CurrentLength( m_TilesDealer.CurrentLength() );

	for ( int i = 0; i < m_TilesDealer.CurrentLength(); ++i )
	{
		mjWall.SetTile( i, m_TilesDealer.GetTile(i) );
	}

	return 0;
}

template<int nPlayerNbr>
BOOL CBaseTileDealerMgr<nPlayerNbr>::CheckNoTile()
{
	if ( m_TilesDealer.CurrentLength() == 0 )
		return TRUE;

	return FALSE;
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
template<int nPlayerNbr>
int CBaseTileDealerMgr<nPlayerNbr>::CheckNeedFan( CMJHand mjTilesNoGod, CMJHand& mjTilesGod, CMJSet mjSet, int nLaiziCount, int bNeedFanCount)
{
	// �������Ҫ��ȡ������ֱ�ӷ���
	if (GetMinFan() < 1 || FALSE == bNeedFanCount)
	{
		return 1;
	}

	m_TilesGodTrans.ReleaseAll();

	int nResCount = 0;
	if ( nLaiziCount > 0 )
	{
		CMJHand mjLaizi; 
		nResCount = ConfigGodTilesFast( mjTilesNoGod, mjSet, nLaiziCount, mjLaizi, mjTilesGod );

		// �������ƻ����滻����ƴ���ȥ
		mjTilesGod.ReleaseAll();
		mjTilesGod.AddTiles( mjLaizi );
	}
	else
	{
		ENVIRONMENT env;
		//SetFanEvn( env, 0, FALSE );
		SetFanEvn( env, m_ucCurCheckChair );
		
		//��Ϊ�жϺ���ʱ������ֻ��13�ţ�Ҫ�����һ��Ҳ�ӽ�ȥ�����ԣ���������
		env.byHandCount[env.byChair] = mjTilesNoGod.CurrentLength();
		for(int i = 0; i < mjTilesNoGod.CurrentLength(); i++)
		{
			env.tHand[env.byChair][i] = mjTilesNoGod.GetTile(i);
		}

		m_pFanCount->SetEnv( env );
		nResCount = m_pFanCount->GetFanCount();;
	}

	//���Ʒֲ�������ͷ��ڣ��ʼ�ȥ���Ʒ�
	if ((GetMJSetting().nTilesFlower == 1) && (m_pFanCount->m_tagFanCount.m_FanNode[80].bFan))
	{
		nResCount -= m_pFanCount->m_tagFanCount.m_FanNode[80].byFanPoint;
	}

	//���Ʒ�Ҳ��������ͷ��ڣ��ʼ�ȥ���Ʒ�
	if ((GetMJSetting().nBlockLevelTing > 0) && (m_pFanCount->m_tagFanCount.m_FanNode[82].bFan))
	{
		nResCount -= m_pFanCount->m_tagFanCount.m_FanNode[82].byFanPoint;
	}

	if (GetMinFan() > nResCount)
	{
		if ((GetMJSetting().nWuFanHu == 1) && (nResCount == 0)) //֧���޷���
		{
			return 1;  // ֧���޷���
		}

		return 0;
	}

	return nResCount;
}

template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::SetFanEvn( ENVIRONMENT &env, int nChair,  int bSetChairTile )
{
	memset( &env, 0, sizeof(env) );
	int i,j;

	env.byChair = nChair;
	if ( nChair == GetTurn() )
	{
		// ����
		if ( m_shGangState == MJ_GANG_STATE_DRAW )
		{
			env.byFlag = MJ_WIN_GANGDRAW;
		}
		else
		{
			env.byFlag = MJ_WIN_SELFDRAW;
		}
	}
	else
	{	
		if ( m_shGangState == MJ_GANG_STATE_GIVE )
		{
			env.byFlag = MJ_WIN_GANGGIVE;
		}
		else
		{
			env.byFlag = MJ_WIN_GUN;
		}
	}

	if ( m_shGangState == MJ_GANG_STATE_BLOCK )
	{
		env.byFlag = MJ_WIN_GANG;
	}

	env.byTurn = GetTurn();
	env.ucDealer = GetDealer();
	env.byTilesLeft = m_TilesDealer.CurrentLength();
	env.byCollectWind = GetMJSetting().nCollateWind;
    env.byCollectArrow = GetMJSetting().nCollateArrow;
    env.byPlayerWind = GetWindRound();
    env.byRoundWind  = GetWindRoundNbr();
	env.tLast = GetLastTile();

	for ( i = 0; i < DICE_NBR && i < 2; ++i )
	{ 
		env.ucDice[i] = GetDice( i );
	}

	for ( i = 0; i < MJ_FLOWER_NBR; ++i )
	{
		if ( m_nFlower[i] < nPlayerNbr )
			env.byFlowerCount[m_nFlower[i]] ++;
	}

	for ( i = 0; i < nPlayerNbr; ++i )
	{
		// �������
		env.byTing[i] = m_nTingType[i];

		CMJSet  mjSet; 
		GetTilesSet( i, mjSet );
		CMJHand mjHand;
		GetTilesHand( i, mjHand );
		CMJGive mjGive;
		GetTilesGive( i, mjGive );

		if ( bSetChairTile == TRUE )
		{
			env.byHandCount[i] = mjHand.CurrentLength();
			// ����Ǻ����ˣ�Ҫ����������������滻��������㷬
			if ( nChair == i )
			{
                CMJHand mjTilesNoGod;
				CMJHand mjTilesGod;
				int nLaiziCount = GetTilesNoGod( m_TilesHand[i], mjTilesNoGod, mjTilesGod );
				
				if ( nLaiziCount > 0 )
				{
// 					CMJHand mjLaizi;
// 					ConfigGodTiles( mjTilesNoGod, m_TilesSet[i], nLaiziCount, mjLaizi, mjTilesGod );
					mjTilesNoGod.AddTiles( m_TilesPlayerGod[i] );
				}
				
				for ( j = 0; j < mjTilesNoGod.CurrentLength(); ++j )
				{
					env.tHand[i][j] = mjTilesNoGod.GetTile( j );
				}
				
			}
			else
			{
				for ( j = 0; j < mjHand.CurrentLength(); ++j )
				{
					env.tHand[i][j] = mjHand.GetTile( j );
				}
			}
		}

		TILE_SET tagSet;
		env.bySetCount[i] = mjSet.CurrentLength();
		for ( j = 0; j < mjSet.CurrentLength(); ++j )
		{
			mjSet.GetSet( j, tagSet );
			env.tSet[i][j][0] = tagSet.m_ucFlag;
			env.tSet[i][j][1] = tagSet.m_ucTile[0];
			env.tSet[i][j][2] = tagSet.m_ucChair;
		}

		//�������Ʋ�����
		int nLen = 0;
		for (j = 0; j < mjGive.CurrentLength(); j++)
		{
			mjGive.GetBlock(j, tagSet);
			if (ACTION_EMPTY == tagSet.m_ucFlag)
			{
				env.tGive[i][nLen] = mjGive.GetTile(j);
				nLen++;
			}
		}
		env.byGiveCount[i] = nLen;
		/*
		env.byGiveCount[i] = mjGive.CurrentLength();
		for ( j = 0; j < mjGive.CurrentLength(); ++j )
		{
			env.tGive[i][j] = mjGive.GetTile( j );
		}
		*/
	}
}
/************************************************************************/
/* 
��ȡ��������
*/
/************************************************************************/
template<int nPlayerNbr>
int CBaseTileDealerMgr<nPlayerNbr>::GetFlowerNbr( int nChair )
{
	int nFlowerNbr = 0;
	for ( int i = 0; i < MJ_FLOWER_NBR; ++i )
	{
		if ( m_nFlower[i] == nChair )
		{
			nFlowerNbr++;
		}
	}

	return nFlowerNbr;
}

template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::ClearFlower()
{
	for ( int i = 0; i < MJ_FLOWER_NBR; ++i )
	{
		m_nFlower[i] = nPlayerNbr;
	}
}

template<int nPlayerNbr>
void CBaseTileDealerMgr<nPlayerNbr>::ClearRunNbr()
{
	m_nRunNbr = 0;
	m_nRunWinNbr = 0;
	m_nRunNotileNbr = 0;
}

/*
SO��
*/
#ifndef WINDOWS_LOGIC
template<int nPlayerNbr>
CSoTileDealerMgr<nPlayerNbr>::CSoTileDealerMgr()
{
	m_bReplaceWall = FALSE;
	m_bReplaceDice = FALSE;
	m_bReplaceDealer = FALSE;
	m_bWindReroll = FALSE;
	m_TilesNextConfig.ReleaseAll();
	m_TielsDiceConfig.ReleaseAll();
	m_TilesDealerConfig.ReleaseAll();

	for (BYTE i = 0; i < nPlayerNbr; i++)
	{
		m_TilesSet[i].ReleaseAll();
	}

	memset( m_bConfigTiles, 0, sizeof(m_bConfigTiles) );
}
 
template<int nPlayerNbr>
CSoTileDealerMgr<nPlayerNbr>::~CSoTileDealerMgr()
{

}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::OnGameMessage(char cMsgID, short nPlayerID, int nChair,  void * pcGameData, short nDataLen)
{
	if (CSoTilePlayerMgr<nPlayerNbr>::OnGameMessage( cMsgID, nPlayerID, nChair, pcGameData, nDataLen ))
    {
        return TRUE;
    }

	if (pcGameData != NULL && nDataLen >= sizeof(TCMD_HEADER))
	{
		TCMD_HEADER *pData = (TCMD_HEADER*)pcGameData;

		if ( MJFRAME_MOUDLE_MSG == pData->cCmdID )
		{
            _DOUT3("----------����CSoTileDealerMgr::OnGameMessage(%d, %d), ���ݰ���С(%d)------------", pData->cCmdID, pData->cCmdPara, nDataLen);

			switch( pData->cCmdPara )
			{
			case CS_MJ_REQUSET_REPLACE_ALLTILES:
				ProcessRequestReplaceAllCards( cMsgID, nPlayerID, nChair, pcGameData, nDataLen );
				return TRUE;
			case CS_MJ_REQUEST_CHANGE_TILES:
				ProcessRequestChangeTiles( cMsgID, nPlayerID, nChair, pcGameData, nDataLen );
				return TRUE;
			case CS_MJ_REQUEST_GETPOS_TILE:
				ProcessRequestGetPosTile( cMsgID, nPlayerID, nChair, pcGameData, nDataLen );
				return TRUE;
			case CS_MJ_REQUEST_GETTILES:
				ProcessRequestGetTiles( cMsgID, nPlayerID, nChair, pcGameData, nDataLen );
				return TRUE;
			case CS_MJ_REQUEST_CONFIGTILE:
				ProcessRequestConfigTiles( cMsgID, nPlayerID, nChair, pcGameData, nDataLen );
				return TRUE;
            case CS_MJ_REQUEST_DEAL_FINISH:
                ProcessRequestDealFinish( cMsgID, nPlayerID, nChair, pcGameData, nDataLen);
                return TRUE;
			default:
				break;
			}
		}
	}

	return FALSE;
}

template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::InitSetDealer()
{
	if (GetMJSetting().nOpenGM == 1 && m_TilesDealerConfig.CurrentLength() > 0)
	{
		// ������ׯ��
		m_nDealer = m_TilesDealerConfig.GetTile(0);
		m_TilesDealerConfig.DelTile(0);
		return ;
	}	

    // ��һ�ֵĻ�������
    if ( m_nRunNbr == 0 )
    {
        srand((unsigned int)GetTickCount() + rand());
        m_nDealer = rand() % nPlayerNbr;
    }
    else if ( m_nWindRoundNbr == 0 && m_nWindRound == 0 && m_bWindReroll )
    {
        m_nDealer = rand() % nPlayerNbr;
    }
    else
    {
        if (m_bWiner[m_nDealer])
        {
            // ׯ��Ӯ��, ������ׯ
            return ;
        }

        // ������˺�����ׯ����һ����ׯ
        m_nDealer = (m_nDealer + GetMJSetting().nGiveOrder) % nPlayerNbr;
    }
}

template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::FillWallTiles()
{
	m_TilesDraw.ReleaseAll();
	m_TilesGang.ReleaseAll();
	m_TilesDealer.ReleaseAll();
	m_TilesDealer.CurrentLength( m_tagMJSetting.nTilesNumber );

	// ���ܳ���ֱ�Ӱ�����������
    int i = 0, j = 0, k = 0;
    
    // ��
	if ( m_tagMJSetting.nTilesChar )
	{
		for(k = 0; k < 4; k++)
		{
			for(j = TILE_CHAR_1; j <= TILE_CHAR_9; j++)
			{
				m_TilesDealer.SetTile( i, j );
				i++;
			}
		}
	}

    // ��
	if ( m_tagMJSetting.nTilesBamboo )
	{
		for(k = 0; k < 4; k++)
		{
			for(j = TILE_BAMBOO_1; j <= TILE_BAMBOO_9; j++)
			{
				m_TilesDealer.SetTile( i, j );
				i++;
			}
		}
	}
    // Ͳ
	if ( m_tagMJSetting.nTilesBall )
	{
		for(k = 0; k < 4; k++)
		{
			for(j = TILE_BALL_1; j <= TILE_BALL_9; j++)
			{
				m_TilesDealer.SetTile( i, j );
				i++;
			}
		}
	}

	// ��
	if ( m_tagMJSetting.nTilesWind )
	{
		for(k = 0; k < 4; k++)
		{
			for(j = TILE_EAST; j <= TILE_NORTH; j++)
			{
				m_TilesDealer.SetTile( i, j );
				i++;
			}
		}
	}

	// ��
	if ( m_tagMJSetting.nTilesArrow )
	{
		for(k = 0; k < 4; k++)
		{
			for(j = TILE_ZHONG; j <= TILE_BAI; j++)
			{
				m_TilesDealer.SetTile( i, j );
				i++;
			}
		}
	}

	// ��
	if ( m_tagMJSetting.nTilesFlower )
	{
		for(j = TILE_FLOWER_CHUN; j <= TILE_FLOWER_JU; j++)
		{
			m_TilesDealer.SetTile( i, j );
			i++;
		}
	}
	
	m_TilesDealer.RandomTiles();
}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::InitDealer()
{
    // �������������
    if ( FALSE == m_bReplaceWall )
    {
        // ������ں�����ǰ������, ��Ϊ�������������
        FillWallTiles();
    }

	// ����ׯ��
	if ( FALSE == m_bReplaceDealer )
	{
		InitSetDealer();
	}

    // ҡɫ��
	if ( FALSE == m_bReplaceDice )
	{
		RandomDice();

		if (GetMJSetting().nOpenGM == 1 && m_TielsDiceConfig.CurrentLength() >= DICE_NBR)
		{
			// ����������, �������ӵ���
			m_nDice[0] = m_TielsDiceConfig.GetTile(0);
			m_nDice[1] = m_TielsDiceConfig.GetTile(1);
			m_TielsDiceConfig.DelTile(0);
			m_TielsDiceConfig.DelTile(1);
		}	
	}

	memset(m_bWiner, 0, sizeof(m_bWiner));
    memset(m_bDealFinish, 0, sizeof(m_bDealFinish));

	m_ucEndType = MJ_END_NO;
	m_nNowTurn = m_nDealer;
	m_shGangState = MJ_GANG_STATE_NONE;

	ClearFlower();

	// ��ԭ
	m_bReplaceDice = FALSE;
	m_bReplaceWall = FALSE;
	m_bReplaceDealer = FALSE;

    // ֪ͨׯ��λ��
    TCMD_NOTIFY_DEALER cmd;
    cmd.byDealer = m_nDealer;
    GetSoGameProcess()->SendGameData((char*)&cmd, sizeof(cmd));

	return 0;
} 

template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::SendStartInfoToClient()
{
	ISoGameProcessor* pProcessor = GetSoGameProcess();
	if ( NULL == pProcessor )
	{
		return;
	}

	ISoUserManager* pUserMgr = GetSoUserManager();
	if ( NULL == pUserMgr )
	{
		return;
	}

	TCMD_NOTIFY_DEAL cmd;

    // ����������ָ����: 0: ʳָ 1: ��ָ
    cmd.ucHandType = rand()%2;

	// ÿ�������Լ���������Ϣ����ÿ����
	for (int i = 0; i < nPlayerNbr; ++i )
	{
		GetDealInfo( i, &cmd );

		ISoPlayer* pUser = GetSoUserManager()->GetPlayerByChair( i );
		if ( NULL == pUser )
		{
			continue;
		}

 		pProcessor->SendGameDataTo( pUser->GetUserID(), (char*)&cmd, sizeof(cmd) );

        // �������Ҳ������Թ�������������Ϊ��
        if (!pUser->GetBCanBeLookedOn())
        {
            memset((void*)cmd.tiles, 0, sizeof(cmd.tiles));
        }

        // �����ݷ��͸��ͻ����Թ����
 		pProcessor->SendGameDataToLookOnUsers( i, (char*)&cmd, sizeof(cmd) );
	}

	// ������ϢΪ0
	memset( m_bConfigTiles, 0, sizeof(m_bConfigTiles) );
}

template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::SendTilesToPlayer()
{
	int i, j, nLength = m_tagMJSetting.nTilesHandNbr;
	CMJHand mjHand;

	//// ���Ʒ���������
	//for ( i = 0; i < nPlayerNbr; ++i )
	//{
	//	mjHand.ReleaseAll();
	//	m_TilesHand[i].ReleaseAll();
	//	m_TilesSet[i].ReleaseAll();

	//	// ��Ԥ�����һ������Ϣ
	//	//ConfigureTiles( i ); �˹�����ʱû����
	//	//mjHand.AddTiles( m_TilesHand[i] );

	//	for ( j = mjHand.CurrentLength(); j < nLength; ++j )
	//	{
	//		mjHand.AddTile( m_TilesDealer.PopTile() );
	//	}

 //       if ( i == m_nDealer )
 //       {
	//		// ׯ��������Ҫ��ץһ����
	//		mjHand.AddTile( m_TilesDealer.PopTile() );

 //           mjHand.Sort();
 //           SetLastTile( mjHand.GetTile(nLength - 1) );
 //       }

	//	SetHandTile( i, mjHand );
	//}

    // �ӵڼ��տ�ʼץ
    int nPos = 0;     
    int nStartPos, nCount = 0;
    int nMJWallNumber = m_tagMJSetting.nTilesNumber / 4;   // ÿ����λ��ǰ�ڷ�����
    TILE ucTile;
    for (i = 0; i < nPlayerNbr; i++)
    {
        mjHand.ReleaseAll();

        for (j = 0; j < nLength; j++)
        {
            nStartPos = m_nDealer * nMJWallNumber + nPos + nCount;
            if (nStartPos >= GetMJSetting().nTilesNumber)
            {
                nStartPos = nStartPos - GetMJSetting().nTilesNumber; 
            }

            ucTile = m_TilesDealer.GetTile(nStartPos);
            mjHand.AddTile(ucTile);

            m_TilesDealer.SetTile(nStartPos,  -1);

            nCount++;
        }

        if (i == m_nDealer)
        {
            nStartPos = m_nDealer * nMJWallNumber + nPos + nCount;
            if (nStartPos >= GetMJSetting().nTilesNumber)
            {
                nStartPos = nStartPos - GetMJSetting().nTilesNumber; 
            }

            ucTile = m_TilesDealer.GetTile(nStartPos);
            mjHand.AddTile(ucTile);

            mjHand.Sort();
            SetLastTile( mjHand.GetTile(nLength) );

            // ׯ�Ҷ�ץһ��
            m_TilesDealer.SetTile(nStartPos,  -1);
            nCount++;
        }

        for (j = 0; j < mjHand.CurrentLength(); j++)
        {
            _DOUT3("���%d, ��%d����ֵ: %d", i, j + 1, mjHand.GetTile(j));
        }

        SetHandTile( i, mjHand );
    }

    
    // ������ǽ��ץ���벹��λ��
    m_TilesDealer.SetPos(m_nDealer * nMJWallNumber + nPos + nCount, m_nDealer * nMJWallNumber + nPos - 1);

    for (i = 0; i < GetMJSetting().nTilesNumber; i++)
    {
        _DOUT2("m_TilesDealer λ��: %d, ֵ: %d", i, m_TilesDealer.GetTile(i));
    }
}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::OnGameStart()
{
	CSoTilePlayerMgr<nPlayerNbr>::OnGameStart();
	
	// ����������Ϣ
	SendRunNbr();

	return 0;
}

/**
 *  @brief ��Ϸ����
 *
 */	
template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::OnGameFinish()
{
	m_TilesNextConfig.ReleaseAll();
	m_TielsDiceConfig.ReleaseAll();
	m_TilesDealerConfig.ReleaseAll();

	for (BYTE i = 0; i < nPlayerNbr; i++)
	{
		m_TilesSet[i].ReleaseAll();
	}

	return 0;
}

/************************************************************************/
/* 
������ʱ������
*/
/************************************************************************/
template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::FillTilesForFinishMsg( TCMD_NOTIFY_FINISH *pCmd )
{
	if ( pCmd == NULL )
		return;

	int i, j, k;
	for ( i = 0; i < nPlayerNbr; ++i )
	{
        m_TilesHand[i].Sort();
        if (pCmd->byWhoWin[i])
        {
            // ���������Ʒ������
            m_TilesHand[i].DelTile(pCmd->tLast);
            m_TilesHand[i].AddTile(pCmd->tLast);
        }

		for ( j = 0; j < m_TilesHand[i].CurrentLength(); ++j )
		{
			pCmd->tile[i][j] = m_TilesHand[i].GetTile( j );
		}

		int nSetLength = 0;
        for ( j = 0; j < m_TilesSet[i].CurrentLength(); ++j )
        {
            TILE_SET tagSet;
            m_TilesSet[i].GetSet( j, tagSet );

            // ������������
            for (k = 0; k < 4; k++)
            {
                pCmd->byBlockTiles[i][j * 4 + k] = tagSet.m_ucTile[k];
            }

            // ��������
            pCmd->byBlockTilesFlag[i][j] = tagSet.m_ucFlag;

            // ����˭����, ����������һ��(��Գ�����Ч)
            pCmd->byBlockTilesWhich[i][j] = tagSet.m_ucChair;
        }

        if (pCmd->byWhoWin[i])
        {
            for (j = 0; j < m_TilesSet[i].CurrentLength(); j++)
            {
                _DOUT7("Ӯ����������=> ��%d������, ���Ʊ�־:0x%x, ��˭��:%d, ��������:%d,%d,%d,%d", 
                    j, pCmd->byBlockTilesFlag[i][j], pCmd->byBlockTilesWhich[i][j], pCmd->byBlockTiles[i][j*4], pCmd->byBlockTiles[i][j*4+1], pCmd->byBlockTiles[i][j*4+2], pCmd->byBlockTiles[i][j*4+3]);
            }
        }
    }
}

/************************************************************************/
/* 
��䷬��,ÿ���˵�score����ĸ��˵õ��ķ���
*/
/************************************************************************/
template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::FillFanForFinishMsg( TCMD_NOTIFY_FINISH *pCmd )
{
	if ( pCmd == NULL )
		return;

	int i,j;
	for ( i = 0; i < nPlayerNbr; ++i )
	{
		pCmd->nScore[i] = 0;

		if ( TRUE == m_bWiner[i] )
		{
			// �㷬�����
			ENVIRONMENT env;
			SetFanEvn( env, i );
			m_pFanCount->SetEnv( env );
			
			int nFanCount = m_pFanCount->GetFanCount();
			for ( j = 0; j < MAX_FAN_NUMBER; ++j )
			{
				if ( m_pFanCount->m_tagFanCount.m_FanNode[j].bFan == TRUE )
				{
					pCmd->byFan[i][j] = TRUE;
					pCmd->byFanScore[i][j] = m_pFanCount->m_tagFanCount.m_FanNode[j].byFanPoint;
					//pCmd->byFanNumber[i][j] = m_pFanCount->m_tagFanCount.m_FanNode[j].byCount;
				}
			}

			// ���������
			if ( nFanCount > GetMJSetting().nMaxFan )
				nFanCount = GetMJSetting().nMaxFan;

			pCmd->nScore[i] = nFanCount;
		}
	}
}

/************************************************************************/
/* 
��תǮ
*/
/************************************************************************/
template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::ScoreToMoneyForFinishMsg( TCMD_NOTIFY_FINISH *pCmd )
{
	if ( pCmd == NULL )
	{
		return;
	}

	// �������Ǯ���ܿ۸������Ǯ���ܳ������õ���
	// ���Ӯ��Ǯ���ܳ����Լ���ǰ�����Ǯ
	int i;
	if ( GetMJSetting().nScoreToMoney > 0 )
	{
		// Ӯ��ǮҪ�����Ǯ������
		int nWinMoney = 0;
		int nScoreToMoney = GetMJSetting().nScoreToMoney;
		int nMaxMoney = -GetMJSetting().nMaxMoney;
		int nAllWinScore = 0;
		for ( i = 0; i < nPlayerNbr; ++i )
		{			
			if ( pCmd->nScore[i] > 0 )
			{
				nAllWinScore+=pCmd->nScore[i];
			}
		}
		
		// �洦��Ǯ
		BOOL bMoreWin = FALSE;
		int nRealWinMoney = 0;
		// ��������Ӯ�˵�Ǯ
		for ( i = 0; i < nPlayerNbr; ++i )
		{
			if ( pCmd->nScore[i] > 0 )
			{
				// ����ҵ�ǰ��Ǯȡ��
				int nUserMoney = GetMJUserMoney( i );
				pCmd->nMoney[i] = nScoreToMoney * pCmd->nScore[i];
				if ( pCmd->nMoney[i] > nUserMoney )
				{
					pCmd->nMoney[i] = nUserMoney;
					bMoreWin = TRUE;
				}
				nRealWinMoney += pCmd->nMoney[i];
			}
		}

		// ��Ӯ��Ǯ��̯���������˵��ˣ�������Ǯ��������Ҫ���·�Ӯ�ҵ�Ǯ
		int nRealLostMoney = 0;
		BOOL bNeedReset = FALSE;
		for ( i = 0; i < nPlayerNbr; ++i )
		{
			if ( pCmd->nScore[i] < 0 )
			{
				int nUserMoney = GetMJUserMoney( i );

				int nLostMoney = pCmd->nScore[i] * nScoreToMoney;
				if ( TRUE == bMoreWin )
				{
					nLostMoney = pCmd->nScore[i] * nRealWinMoney / nAllWinScore;
				}

				// ��ó���������
				if ( nLostMoney < nMaxMoney )
				{
					nLostMoney = nMaxMoney;
					bNeedReset = TRUE;
				}

				// ���ûǮ��
				if ( nLostMoney + nUserMoney < 0 )
				{
					nLostMoney = -nUserMoney;
					bNeedReset = TRUE;
				}
				nRealLostMoney -= nLostMoney;
				pCmd->nMoney[i] = nLostMoney;
			}
		}

		// ���·�Ӯ�ҵ�Ǯ
		if ( bNeedReset )
		{
			for ( i = 0; i < nPlayerNbr; ++i )
			{
				if ( pCmd->nScore[i] > 0 )
				{
					pCmd->nMoney[i] = nRealLostMoney * pCmd->nScore[i] / nAllWinScore;
				}
			}
		}
	}
}

/************************************************************************/
/* 
���
*/
/************************************************************************/
template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::CountScoreForFinishMsg( TCMD_NOTIFY_FINISH *pCmd )
{
	if ( pCmd == NULL )
		return;

	int i,k;
	int nFanCount[nPlayerNbr] = {0};

	BOOL bWinAgain = FALSE;
	for ( i = 0; i < nPlayerNbr; ++i )
	{
		nFanCount[i] = pCmd->nScore[i];
		pCmd->nScore[i] = 0;

		if ( GetDealer() == i && m_bWiner[i] == TRUE )
		{
			bWinAgain = TRUE;
		}
	}
	
	// ������3��������ǲ�����ֵ�
	if ( m_ucEndType == MJ_END_REQUEST 
		 ||
		 m_ucEndType == MJ_END_SYSTEM 
		 ||
		 m_ucEndType == MJ_END_ESCAPE )
		return;

	// ��ʼ���
	for ( i = 0; i < nPlayerNbr; ++i )
	{
		// ֻ�����˵�
		if ( nFanCount[i] > 0 )
		{
			int nBaseScore = nFanCount[i] * GetMJSetting().nBaseScore;
			if ( pCmd->byFlag == MJ_END_GUN || pCmd->byFlag == MJ_END_QIANG )
			{
				// ����ֻ��һ����
				pCmd->nScore[pCmd->byWhoGun] -= nBaseScore;
				pCmd->nScore[i] += nBaseScore;
			}
			else if ( pCmd->byFlag == MJ_END_SELF )
			{
				// ����û�����˶�Ҫ����
				// ��Ϊ֧��Ѫս
				for ( k = 0; k < nPlayerNbr; ++k )
				{
					if ( TRUE != m_bWiner[k] )
					{
						pCmd->nScore[k] -= nBaseScore;
						pCmd->nScore[i] += nBaseScore;
					}
				}
			}
		}
	}

	_DOUT("��Ϸ�������:");
	for (int i = 0; i < nPlayerNbr; i++)
	{
		pCmd->nFanPoint[i] = pCmd->nScore[i];
		_DOUT2("���λ��: %d, �÷�: %d", i, pCmd->nScore[i]);    
	}
}

template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::CountGangScoreForFinishMsg( TCMD_NOTIFY_FINISH *pCmd )
{
	if ( pCmd == NULL )
		return;

	int i,j,k;
	
	// ������3��������ǲ�����ֵ�
	if ( m_ucEndType == MJ_END_REQUEST 
		 ||
		 m_ucEndType == MJ_END_SYSTEM 
		 ||
		 m_ucEndType == MJ_END_ESCAPE )
    {
        return;
    }

	// ����ܷ�Ϊ0��������
	if ( GetMJSetting().nGangScore <= 0 )
	{
		return;
	}

	// ��ʼ���
	for ( i = 0; i < nPlayerNbr; ++i )
	{
		// ͳ�Ƹܷ֣�����ĸ����Ƶ�û��ϵ
		for ( j = 0; j < m_TilesSet[i].CurrentLength(); ++j )
		{
			TILE_SET tagSet;
			m_TilesSet[i].GetSet( j, tagSet );

			int nBaseGangScore = GetMJSetting().nGangScore;
			if ( ACTION_QUADRUPLET_CONCEALED == tagSet.m_ucFlag )
			{
				nBaseGangScore = GetMJSetting().nGangScore * 2;
				for ( k = 0; k < nPlayerNbr; ++k )
				{
					if ( i != k )
					{
						pCmd->nGangFen[k] -= nBaseGangScore;
					}
					else 
					{
						pCmd->nGangFen[k] += nBaseGangScore * ( nPlayerNbr - 1 );
					}
				}
			}
			else if ( ACTION_QUADRUPLET_REVEALED == tagSet.m_ucFlag )
			{
				nBaseGangScore = GetMJSetting().nGangScore * 2;
				for ( k = 0; k < nPlayerNbr; ++k )
				{
					if ( i == k )
					{
						pCmd->nGangFen[k] += nBaseGangScore;
					}
					else if ( k == tagSet.m_ucChair )
					{
						pCmd->nGangFen[k] -= nBaseGangScore;
					}
				}
			}
			else if ( ACTION_QUADRUPLET_PATCH == tagSet.m_ucFlag )
			{
				nBaseGangScore = GetMJSetting().nGangScore;
				for ( k = 0; k < PLAYER_NBR; ++k )
				{
					if ( i == k )
					{
						pCmd->nGangFen[k] += nBaseGangScore * ( nPlayerNbr - 1 );
					}
					else
					{
						pCmd->nGangFen[k] -= nBaseGangScore;
					}
				}
			}
		}
	}

	_DOUT("��Ϸ�ֽܷ���:");
	for (int i = 0; i < nPlayerNbr; i++)
	{
		_DOUT2("���λ��: %d, �ܷ�: %d", i, pCmd->nGangFen[i]);
		pCmd->nScore[i] += pCmd->nGangFen[i];
	}
}

template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::CountRunNbr( TCMD_NOTIFY_FINISH *pCmd )
{
	// ��ɢ����һ��
	if ( m_ucEndType == MJ_END_SYSTEM ||
		m_ucEndType == MJ_END_ESCAPE ||
		m_ucEndType == MJ_END_REQUEST )
		return;

	BOOL bWinAgain = FALSE;
	for ( int i = 0; i < nPlayerNbr; ++i )
	{
		if ( GetDealer() == i && m_bWiner[i] == TRUE && m_nRunNbr > 0 )
		{
			bWinAgain = TRUE;
		}
	}


	m_nRunNbr++;
	// ����65535�ֵ��������û��
	if ( m_nRunNbr >= 65535 )
	{
		m_nRunNbr = 0;
	}

	if ( m_ucEndType == MJ_END_NOTILE && m_nRunNbr > 1 )
	{
		m_nRunNotileNbr++;
	}
	else
	{
		m_nRunNotileNbr = 0;
	}

	if ( bWinAgain )
	{
		m_nRunWinNbr++;
	}
	else
	{
		m_nRunWinNbr = 0;
	}
}

template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::SendRunNbr()
{
	ISoGameProcessor* pProcessor = GetSoGameProcess();
	if ( NULL == pProcessor )
	{
		return;
	}

	TCMD_NOTIFY_RUN_NBR cmd;
	cmd.nRunNbr = m_nRunNbr;
	cmd.nRunWinNbr = m_nRunWinNbr;
	cmd.nRunNotileNbr = m_nRunNotileNbr;

	pProcessor->SendGameData( (char*)&cmd, sizeof(cmd) );
}

template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::SendFinishMsg()
{
	ISoGameProcessor* pProcessor = GetSoGameProcess();
	if ( NULL == pProcessor )
	{
		return;
	}

	ISoUserManager* pUserMgr = GetSoUserManager();
	if ( NULL == pUserMgr )
	{
		return;
	}

    for (char i = 0; i < nPlayerNbr; i++)
    {
        if (pUserMgr->GetPlayerTrusteeStatus(i))
        {
            // ȡ���й�
            pUserMgr->SetPlayerTrusteeStatus(i, FALSE);
        }
    }

	TCMD_NOTIFY_FINISH cmd;

	// �����ƣ��������֣����Ƶ���Ϣ
    FillFinishMsg( &cmd );

	// ���
	CountScoreForFinishMsg( &cmd );

	// ����ܷ�
	CountGangScoreForFinishMsg( &cmd );

	// ���ͽ�����Ϣ
	SendSaveFinishMsg( &cmd, sizeof(cmd) );
}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::FillFinishMsg(  TCMD_NOTIFY_FINISH *pCmd  )
{
	if ( pCmd == NULL )
		 return 1;

	pCmd->byFlag = m_ucEndType;
	pCmd->tLast = GetLastTile();

	if ( pCmd->byFlag == MJ_END_GUN || pCmd->byFlag == MJ_END_QIANG )
	{
		pCmd->byWhoGun = GetTurn();
	}

	int i;
	for ( i = 0; i < nPlayerNbr; ++i )
	{
		if ( TRUE == m_bWiner[i] )
		{
			pCmd->byWhoWin[i] = TRUE;
		}
	}
	
	// ������
	CountRunNbr( pCmd );
	
	// �㷬
	FillFanForFinishMsg( pCmd );
	
	// ����
	FillTilesForFinishMsg( pCmd );

	return 0;
}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::SendSaveFinishMsg(  TCMD_NOTIFY_FINISH *pCmd, int nCmdLen  )
{
	if ( pCmd == NULL )
		return 1;

	ISoGameProcessor* pProcessor = GetSoGameProcess();
	if ( NULL == pProcessor )
	{
		return 1;
	}

	// ����תǮ
	ScoreToMoneyForFinishMsg( pCmd );
	
    // ������ҵ�����
	SetScoreByFinishMsg( pCmd );
	
    // ֪ͨ�ͻ��˽�������
    pProcessor->SendGameData( (char*)pCmd, nCmdLen );

	// ��¼������Ϣ�����͸��Զ�������ʹ�ã��Զ�������ֻ��ʹ�õ�����Ϣ�Ĳ��֣������ȡ��Ϣ����Ĳ���
	memcpy( &m_cmdFinish, pCmd, sizeof(TCMD_NOTIFY_FINISH) );

	return 0;
}

/************************************************************************/
/* 
�۳���Ʊ
*/
/************************************************************************/
template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::PayTicket()
{
	// ϵͳ����Ĳ�ִ��
	if ( m_ucEndType == MJ_END_SYSTEM )
		return;

	ISoGameProcessor* pProcessor = GetSoGameProcess();
	if ( NULL == pProcessor )
	{
		return;
	}

	ISoUserManager *pUserMgr = GetSoUserManager();

	if ( NULL == pUserMgr )
		return;
	
	int nTicket = GetMJSetting().nTicket;
	if ( nTicket <= 0 )
		return;

	int nPoint[nPlayerNbr];
    //int nRePoint[nPlayerNbr];
	for ( int i = 0; i < nPlayerNbr; ++i )
	{
		int nUserMoney = GetMJUserMoney( i );
		int nCutTicket = nTicket;
		if ( nCutTicket > nUserMoney )
		{
			nCutTicket = nUserMoney;
		}

        nPoint[i] = nCutTicket;
	}

    //pUserMgr->SetGameScore(nPoint, nRePoint);
}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::SetScoreByFinishMsg( TCMD_NOTIFY_FINISH *pCmd )
{
	if ( pCmd == NULL )
		return -1;

	ISoGameProcessor* pProcessor = GetSoGameProcess();
	if ( NULL == pProcessor )
	{
		return -1;
	}

	ISoUserManager *pUserMgr = GetSoUserManager();

	if ( NULL == pUserMgr )
		return -1;

    // ����Ƿ����ܿ۷ѣ�����·����������ܿ۷ѣ���ר�ŵ���
    if ( pCmd->byFlag != MJ_END_ESCAPE )
    {
        //pUserMgr->SetGameScore(pCmd->nScore);
    }

	return 1;
}

template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::OnTimeOutFlower()
{
	memset( m_bCheckFlower, 0, sizeof(m_bCheckFlower) );
	
	// ֪ͨ��C
	TCMD_NOTIFY_FLOWER_OVER cmd;
	ISoGameProcessor* pProcess = GetSoGameProcess();

	_DOUT("���벹��״̬, MJ_TIMER_FLOWER ʱ�䵽, ֪ͨ�ͻ��˲�������, ������һ���׶�, ����\n");

    int nTurn = GetTurn();

    TCMD_NOTIFY_BLOCK tagBlock;
    if (CheckDrawBlock( nTurn ) != 0)
    {
        // ��ȡ������Ϣ
        memcpy(&tagBlock, &GetBlockInfo(nTurn), sizeof(TCMD_NOTIFY_BLOCK));
    }

    for (char i = 0; i < PLAYER_NBR; i++)
    {
        ISoPlayer *pSoPlayer = GetSoUserManager()->GetPlayerByChair(i);

        if (pSoPlayer == NULL)
        {
            continue ;
        }

        if (nTurn == i)
        {
            if (pSoPlayer->IsPlayer())
            {
                // ������Ϣ
                cmd.ucFlag = tagBlock.ucFlag;
                cmd.ucChair = tagBlock.ucChair;
                memcpy(&cmd.ucTile, &tagBlock.ucTile, sizeof(cmd.ucTile));
                memcpy(&cmd.ucTingTile, &tagBlock.ucTingTile, sizeof(cmd.ucTingTile));

                pProcess->SendGameDataTo(pSoPlayer->GetUserID(), (char*)&cmd, sizeof(cmd));
            }
            else
            {
                // �Թ۲�����ʵ����

                // ������Ϣ
                cmd.ucFlag = 0;
                cmd.ucChair = 0;
                memset(cmd.ucTile, 0, sizeof(cmd.ucTile));
                memset(cmd.ucTingTile, 0, sizeof(cmd.ucTingTile));
                pProcess->SendGameDataToLookOnUsers(i, (char*)&cmd, sizeof(cmd));
            }
        }
        else
        {
            // ������Ҳ�����ʵ����

            // ������Ϣ
            cmd.ucFlag = 0;
            cmd.ucChair = 0;
            memset(cmd.ucTile, 0, sizeof(cmd.ucTile));
            memset(cmd.ucTingTile, 0, sizeof(cmd.ucTingTile));
            pProcess->SendGameDataTo(pSoPlayer->GetUserID(), (char*)&cmd, sizeof(cmd));
        }
    }
}

/************************************************************************/
/* 
��������ж�
*/
/************************************************************************/
template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::CheckOverFlower( int nChair )
{
	if ( NULL == m_pStateEvent )
		return;

	// ����״̬�������Ƿ�Ҫ�ȴ�ȫ���˲���
	if ( MJ_STATE_FLOWER == m_pStateEvent->GetMJState()  )
	{
		// ���յ���ҵ������ˣ��Ͷ�ΪTRUE
		// �����Ȼ�л��ƣ�����û�����ٽ���״̬
		if ( 0 == CheckFlower( nChair ) )
		{
			m_bCheckFlower[nChair] = TRUE;
			
			// ���ȫ��OK��������һ�׶�
			for ( int i = 0; i < nPlayerNbr; ++i )
			{
				if ( m_bCheckFlower[i] == FALSE )
				{
					return;
				}
			}
			
			// ������һ�׶�
			OnTimeOutFlower();
		}
	}
	else
	{
		CheckDrawBlock( nChair );
	}
}

template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::DrawTile( int nTurn )
{
	if ( nTurn < 0 || nTurn >= nPlayerNbr )
		return;

	if ( NULL == m_pStateEvent )
		return;

	ISoGameProcessor* pProcess = GetSoGameProcess();
	if ( NULL == pProcess )
	{
		return;
	}

	if ( TRUE == CheckNoTile() )
	{
		_DOUT("�ľ�\n");
		//���ƣ���Ϸ����
		// д�����������ж�
		m_ucEndType = MJ_END_NOTILE;
		m_pStateEvent->SetMJState( MJ_STATE_END );
		return;
	}

	ClearLoseHu( nTurn );
	BOOL bCheckFlower = FALSE;
	TILE ucTile = TILE_BEGIN;
	if (m_shGangState != MJ_GANG_STATE_DRAW)
	{
		// ������������
		ucTile = m_TilesDealer.PopTile();

		// ��¼ץ������
		m_TilesDraw.AddTile(ucTile);
	}
	else
	{
		// ��������
		ucTile =  m_TilesDealer.PopFrontTile();

		// ��¼��������
		m_TilesGang.AddTile(ucTile);
	}

	//////////////////// ����Ƿ����������� ///////////////////
	if (GetMJSetting().nOpenGM == 1)
	{
		if (m_TilesSetConfig[nTurn].CurrentLength() > 0)
		{
			// ������Լ�����������
			ucTile = m_TilesSetConfig[nTurn].GetTile(0);
			m_TilesSetConfig[nTurn].DelTile(ucTile);
		}
		else if (m_TilesNextConfig.CurrentLength() > 0)
		{
			// ��������һ��ץ������
			ucTile = m_TilesNextConfig.GetTile(0);
			m_TilesNextConfig.DelTile(ucTile);
		}
	}
	///////////////////////////////////////////////////////////

    TCMD_NOTIFY_DRAW cmd;
    cmd.chair = nTurn;
    cmd.t_target = m_shGangState;  // ���Ʊ�־, ����ʶ��ͻ��˽����ǰ��ץ�ƻ��ǴӺ���ץ��
	m_TilesHand[nTurn].AddTile( ucTile );
	SetLastTile( ucTile );

    _DOUT4("���:%d, ��һ����:%d, ���Ʒ�λ=%d, ��ǽʣ��������=%d \n", nTurn, ucTile, m_shGangState, m_TilesDealer.CurrentLength());

	if ( m_shGangState != MJ_GANG_STATE_DRAW )
	{
		// �Ǹ��Ͽ�����־������Ҫ���ø��Ʊ�־
		m_shGangState = MJ_GANG_STATE_NONE;
	}

	if ( CheckIsAutoWinTile() )
	{
		if ( GetMJSetting().nBlockLevelWin == CheckBlock( nTurn ) )
		{
			m_pStateEvent->SetMJState( MJ_STATE_BLOCK );

			// �൱�ڶ��ߵĴ���ʽ��������TIMEOUT��TIMEOUT��ֱ�ӷ���
			ISoUserManager* pUserMgr = GetSoUserManager();
			if ( pUserMgr )
			{
				ISoPlayer* pUser = pUserMgr->GetPlayerByChair( nTurn );
				if ( pUser )
				{
					SoTrustOffline( nTurn, pUser->GetUserID() );
				}
			}
		}
		else 
		{
			nTurn = ( m_pTileEvent->GetTurn() + GetMJSetting().nGiveOrder ) % nPlayerNbr;
			SetTurn( nTurn );
			m_pStateEvent->SetMJState( MJ_STATE_DRAW );
		}

		return;
	}


    TCMD_NOTIFY_BLOCK tagBlock;
	if (CheckDrawBlock( nTurn ) != 0)
    {
        // ��ȡ������Ϣ
        memcpy(&tagBlock, &GetBlockInfo(nTurn), sizeof(TCMD_NOTIFY_BLOCK));

		if (tagBlock.ucFlag & 0x04)
		{
			for (int i = 0; i < 12 && tagBlock.ucTile[2][i] > 0; i += 4)
			{
				_DOUT5("���λ��: %d, ��������, ��������; %d,%d,%d,%d", nTurn, tagBlock.ucTile[2][i], tagBlock.ucTile[2][i + 1], tagBlock.ucTile[2][i + 2], tagBlock.ucTile[2][i+ 3]);
			}
		}
    }

    for (char i = 0; i < nPlayerNbr; i++)
    {
        ISoPlayer *pSoPlayer = GetSoUserManager()->GetPlayerByChair(i);

        if (pSoPlayer == NULL)
        {
            continue ;
        }

        if (nTurn == i)
        {
            if (pSoPlayer->IsPlayer())
            {
                cmd.t = ucTile;

                // ������Ϣ
                cmd.ucFlag = tagBlock.ucFlag;
                cmd.ucChair = tagBlock.ucChair;
                memcpy(&cmd.ucTile, &tagBlock.ucTile, sizeof(cmd.ucTile));
                memcpy(&cmd.ucTingTile, &tagBlock.ucTingTile, sizeof(cmd.ucTingTile));

                pProcess->SendGameDataTo(pSoPlayer->GetUserID(), (char*)&cmd, sizeof(cmd));
            }
            else
            {
                // �Թ۲�����ʵ����
                cmd.t = TILE_BEGIN;

                // ������Ϣ
                cmd.ucFlag = 0;
                cmd.ucChair = 0;
                memset(cmd.ucTile, 0, sizeof(cmd.ucTile));
                memset(cmd.ucTingTile, 0, sizeof(cmd.ucTingTile));
                pProcess->SendGameDataToLookOnUsers(i, (char*)&cmd, sizeof(cmd));
            }
        }
        else
        {
            // ������Ҳ�����ʵ����
            cmd.t = TILE_BEGIN;

            // ������Ϣ
            cmd.ucFlag = 0;
            cmd.ucChair = 0;
            memset(cmd.ucTile, 0, sizeof(cmd.ucTile));
            memset(cmd.ucTingTile, 0, sizeof(cmd.ucTingTile));
            pProcess->SendGameDataTo(pSoPlayer->GetUserID(), (char*)&cmd, sizeof(cmd));
        }
    }
}

/** 
* @brief ���뼶��ʱ��
* @param[in] nTimerID ��ʱ��ID
*/
template<int nPlayerNbr>
void CSoTileDealerMgr<nPlayerNbr>::OnMSTimer(int nTimerID)
{
    if (nTimerID == MJ_TIMER_DEAL_FINISH)
    {
        // ����ʱ�䳬ʱ, ����ʱ�����, δ�����Ƶ����, �������Զ����䴦��
        TCMD_REQUEST_DEAL_FINISH cmd;

        for (BYTE i = 0; i < PLAYER_NBR; i++)
        {
            if (!m_bDealFinish[i])
            {
                ProcessRequestDealFinish(0, 0, i, (void *)&cmd, sizeof(TCMD_REQUEST_DEAL_FINISH));
            }
        }
    }
}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::ProcessRequestFlower(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen)
{
	_DOUT1("���:%d, ����\n", nChair);

	ISoGameProcessor* pProcessor = GetSoGameProcess();
	if ( NULL == pProcessor )
	{
		return 1;
	}

	ISoUserManager* pUserMgr = GetSoUserManager();
	if ( NULL == pUserMgr )
	{
		return 1;
	}

	if ( NULL == m_pStateEvent )
	{
		return 1;
	}

	// Ϊ���Զ���
	if ( pProcessor->GetGameState() != gsPlaying )
	{
		return 1;
	}

	TCMD_REQUEST_FLOWER* pCmd = (TCMD_REQUEST_FLOWER*)pcGameData;
	if ( pCmd->ucChair >= nPlayerNbr )
	{
		return 1;
	}

    pCmd->ucChair = nChair;

	TCMD_NOTIFY_FLOWER cmd;
	cmd.ucChair = pCmd->ucChair;
	cmd.ucNumber = 0;
	int i = 0;
	for ( i = 0; i < m_TilesHand[pCmd->ucChair].CurrentLength(); ++i )
	{
		TILE ucTile = m_TilesHand[pCmd->ucChair].GetTile( i );

		if ( GetTileGenre( ucTile ) == TILE_GENRE_FLOWER )
		{
			cmd.ucOldTile[cmd.ucNumber] = ucTile;
			if ( TRUE == CheckNoTile() )
			{
				m_ucEndType = MJ_END_NOTILE;
				m_pStateEvent->SetMJState( MJ_STATE_END );
				return 1;
			}
			cmd.ucTile[cmd.ucNumber] = m_TilesDealer.PopTile();
			if ( pCmd->ucChair == GetTurn() )
			{

				SetLastTile( cmd.ucTile[cmd.ucNumber] );
			}
			m_TilesHand[pCmd->ucChair].SetTile( i, cmd.ucTile[cmd.ucNumber] );
			int nPos = ucTile - TILE_FLOWER_CHUN;
			if ( nPos >= 0 && nPos < 8 )
				m_nFlower[ucTile - TILE_FLOWER_CHUN ] = nChair;
			cmd.ucNumber++;
		}
	}


	for ( i = 0; i < MJ_FLOWER_NBR; ++i )
	{
		cmd.ucFlower[i] = m_nFlower[i];
	}

	pProcessor->SendGameDataToSeat( pCmd->ucChair, (char*)&cmd, sizeof(cmd) );
	
	memset( cmd.ucTile, 0, sizeof(cmd.ucTile) );
	pProcessor->SendGameDataExceptSeat( pCmd->ucChair, (char*)&cmd, sizeof(cmd) );

	CheckOverFlower( pCmd->ucChair );

	return 0;
}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::ProcessRequestReplaceAllCards(char cMsgID, short nPlayerID, 
																int nChair, void * pcGameData, short nDataLen)
{
// 	return 0;

	if ( 0 == GetMJSetting().nOpenGM )
	{
		return 1;
	}

	ISoGameProcessor* pProcessor = GetSoGameProcess();
	if ( NULL == pProcessor )
	{
		return 1;
	}

	ISoUserManager* pUserMgr = GetSoUserManager();
	if ( NULL == pUserMgr )
	{
		return 1;
	}

	TCMD_REQUEST_REPLACE_ALLCARDS* pCmd = (TCMD_REQUEST_REPLACE_ALLCARDS*)pcGameData;
	m_TilesDealer.CurrentLength( pCmd->nCount );

	_DOUT1( "ProcessRequestReplaceAllCards �յ��޸�������Ϣ:%d \n", pCmd->nCount );
	for ( int i = 0; i < pCmd->nCount; ++i )
	{
		m_TilesDealer.SetTile( i, pCmd->szCards[i] );
		_DOUT1(" %d", pCmd->szCards[i]);
	}
	if ( pCmd->cbBankUser < nPlayerNbr )
	{
		m_nDealer = pCmd->cbBankUser;
	}

	m_nDice[0] = pCmd->ucDice[0];
	m_nDice[1] = pCmd->ucDice[1];

	m_bReplaceWall = TRUE;
	m_bReplaceDice = TRUE;
	m_bReplaceDealer = TRUE;

	TCMD_NOTIFY_REPLACE_ALLCARDS cmd;
	cmd.nResult = TRUE;
	pProcessor->SendGameDataTo( nPlayerID, (char*)&cmd, sizeof(cmd) );
	return 0;
}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::ProcessRequestConfigTiles(char cMsgID, short nPlayerID, 
															int nChair, void * pcGameData, short nDataLen)
{
	ISoGameProcessor* pProcess = GetSoGameProcess();
	if ( NULL == pProcess )
	{
		return 1;
	}

	if ( NULL == m_pStateEvent )
	{
		return 1;
	}

	TCMD_NOTIFY_CONFIGTILE cmd;
	cmd.ucChair = nChair;
	cmd.bResult = FALSE;
	if ( m_pStateEvent->GetMJState() == MJ_STATE_END 
		|| m_pStateEvent->GetMJState() == MJ_STATE_NOSTART )
	{
		m_bConfigTiles[nChair] = TRUE;
		cmd.bResult = TRUE;
	}

	pProcess->SendGameDataTo( nPlayerID, (char*)&cmd, sizeof(cmd) );

	return 0;
}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::ProcessRequestGive(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen)
{
	int res = CSoTilePlayerMgr<nPlayerNbr>::ProcessRequestGive( cMsgID, nPlayerID, nChair, pcGameData, nDataLen );
	
	if ( res == 0 )
	{
		SetShowTurn( ( nChair + GetMJSetting().nGiveOrder ) % nPlayerNbr ); 
	}

	return res;
}

/*
*  @brief ��������������Ϣ
*/
template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::ProcessRequestDealFinish(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen)
{
    if (nDataLen != sizeof(TCMD_REQUEST_DEAL_FINISH))
    {
        return -1;
    }

    //TCMD_REQUEST_DEAL_FINISH * pCmd = (TCMD_REQUEST_DEAL_FINISH *)pcGameData;
	_DOUT1("���%d, ���������", nChair);

    if (m_bDealFinish[nChair])
    {
        return 0;  // ������ѷ������
    }

    m_bDealFinish[nChair] = TRUE;

    int nCount = 0;
    for (BYTE i = 0; i < nPlayerNbr; i++)
    {
        if (m_bDealFinish[i])
        {
            nCount++;
        }
    }

    if (nCount == nPlayerNbr)
    {
		_DOUT("����ȷ�����, ���벹��״̬");

        // ���Ʋ������
        GetSoGameProcess()->KillTimer(MJ_TIMER_DEAL_FINISH);

        // ���벹��״̬
        m_pStateEvent->SetMJState( MJ_STATE_FLOWER );
    }  

    return 0;
}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::ProcessRequestChangeTiles(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen)
{
	ISoGameProcessor* pProcess = GetSoGameProcess();
	if ( NULL == pProcess )
	{
		return 1;
	}

	ISoUserManager* pUserMgr = GetSoUserManager();
	if ( NULL == pUserMgr )
	{
		return 1;
	}

	TCMD_REQUEST_CHANGE_TILES* pCmd = (TCMD_REQUEST_CHANGE_TILES*)pcGameData;
	TCMD_NOTIFY_CHANGE_TILES cmd;

	if ( NULL == m_pStateEvent )
	{
		return 1;
	}

	cmd.ucResult = TRUE;

	// ���ڳ���״̬Ҳ���ܻ�
	if ( nChair != GetTurn() || m_pStateEvent->GetMJState() != MJ_STATE_GIVE )
	{
		cmd.ucResult = FALSE;
	}

	// ��ͷû�����ƣ�����
	if ( FALSE == m_TilesHand[nChair].IsHave( pCmd->ucRequestTile ) )
	{
		cmd.ucResult = FALSE;
	}
	
	cmd.ucChangeType = pCmd->ucChangeType;
	cmd.ucRequestTile = pCmd->ucRequestTile;
	cmd.ucTargetTile = pCmd->ucTargetTile;

	if ( cmd.ucResult == TRUE )
	{
		if ( MJ_CHANGE_TARGET == pCmd->ucChangeType )
		{
			if ( m_TilesDealer.IsHave( pCmd->ucTargetTile ) )
			{
				m_TilesDealer.DelTile( pCmd->ucTargetTile );
				m_TilesDealer.AddTile( pCmd->ucRequestTile );
				m_TilesDealer.RandomTiles();
				cmd.ucResult = TRUE;
			}
			else
			{
				cmd.ucResult = FALSE;
			}
		}
		else
		{
			if ( m_TilesDealer.CurrentLength() == 0 )
			{
				cmd.ucResult = FALSE;
			}
			else
			{
				cmd.ucTargetTile = m_TilesDealer.PopTile();
				m_TilesDealer.AddTile( pCmd->ucRequestTile );
				m_TilesDealer.RandomTiles();
				cmd.ucResult = TRUE;
			}
		}

	}

	pProcess->SendGameDataToSeat( nChair, (char*)&cmd, sizeof(cmd) );
	if ( TRUE == cmd.ucResult )
	{
		m_TilesHand[nChair].AddTile( cmd.ucTargetTile );
		m_TilesHand[nChair].DelTile( cmd.ucRequestTile );

		SetLastTile( cmd.ucTargetTile );
		
		// �ж��Լ������Ʋ�
		CheckDrawBlock( GetTurn() );
	}

	return 0;
}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::ProcessRequestGetPosTile(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen)
{
	ISoGameProcessor* pProcessor = GetSoGameProcess();
	if ( NULL == pProcessor )
	{
		return 1;
	}

	ISoUserManager* pUserMgr = GetSoUserManager();
	if ( NULL == pUserMgr )
	{
		return 1;
	}

	TCMD_REQUEST_GETPOS_TILE* pCmd = (TCMD_REQUEST_GETPOS_TILE*)pcGameData;
	TCMD_NOTIFY_GETPOS_TILE cmd;

	cmd.ucRequestTile = pCmd->ucRequestTile;
	cmd.ucTilePos = m_TilesDealer.CurrentLength();
	for ( int i = m_TilesDealer.CurrentLength() - 1; i >= 0; --i )
	{
		TILE ucTile = m_TilesDealer.GetTile( i );
		if ( ucTile == pCmd->ucRequestTile )
		{
			cmd.ucTilePos = cmd.ucTilePos - i;
			break;
		}
	}

	// �ҵ����û�Ĺ���˵��û�ҵ�
	if ( cmd.ucTilePos == m_TilesDealer.CurrentLength() )
		cmd.ucTilePos = 0;

	pProcessor->SendGameDataTo( nPlayerID, (char*)&cmd, sizeof(cmd) );
	return 0;
}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::ProcessRequestGetTiles(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen)
{
	ISoGameProcessor* pProcessor = GetSoGameProcess();
	if ( NULL == pProcessor )
	{
		return 1;
	}

	ISoUserManager* pUserMgr = GetSoUserManager();
	if ( NULL == pUserMgr )
	{
		return 1;
	}

	TCMD_REQUEST_GETTILES* pCmd = (TCMD_REQUEST_GETTILES*)pcGameData;
	TCMD_NOTIFY_GETTILES cmd;

	for ( int i = 0; i < pCmd->ucGetNbr && i < MJ_GETTILES_NBR; ++i )
	{
		cmd.ucGetTile[i] = m_TilesDealer.GetTile( m_TilesDealer.CurrentLength() - 1 - i );
	}

	cmd.ucGetNbr = pCmd->ucGetNbr;

	if ( pCmd->ucGetNbr > MJ_GETTILES_NBR )
		cmd.ucGetNbr = MJ_GETTILES_NBR;
	
	pProcessor->SendGameDataTo( nPlayerID , (char*)&cmd, sizeof(cmd) );

	return 0;
}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::ConfigureTiles( int nChair )
{
	if ( nChair < 0 || nChair >= nPlayerNbr )
		return 1;

	if ( FALSE == m_bConfigTiles[nChair] )
		return 1;

	m_TilesHand[nChair].ReleaseAll();

	int nRandom = rand() % 1000;
	BOOL bRes = FALSE;
	if ( nRandom < 200 )
	{
		// ��һɫ
		if ( GetMJSetting().nTilesNumber == MAX_TOTAL_TILES )
			bRes = ConfigureZiyise( nChair );
		else
			bRes = ConfigureQingyise( nChair );
	}
	else if ( nRandom < 700 )
	{
		// �İ���
		bRes = ConfigureSianke( nChair );
	}
	else if ( nRandom < 1000 )
	{
		// ��һɫ
		bRes = ConfigureQingyise( nChair );
	}

	return bRes;
}

template<int nPlayerNbr>
BOOL CSoTileDealerMgr<nPlayerNbr>::ConfigureQingyise( int nChair )
{
	int ntype = rand() %3;
	int nmin = ntype*10+1;
	int nmax = ntype*10+9;
	int nnbr = 0;
	CMJWall mj;
	for ( int i = 0; i < m_TilesDealer.CurrentLength(); ++i )
	{
		TILE tile = m_TilesDealer.GetTile(i);
		if ( tile >= nmin && tile <= nmax )
		{
			nnbr++;
			if ( nnbr<8 )
			{
				mj.AddTile(tile);
				m_TilesHand[nChair].AddTile(tile);
			}		
			else 
			{
				break;
			}
		}
	}

	m_TilesDealer.DelTiles(mj);

	return TRUE;
}

template<int nPlayerNbr>
BOOL CSoTileDealerMgr<nPlayerNbr>::ConfigureSianke( int nChair )
{
	int i;
	CMJWall mj;
	CMJWall savemj;
	int nbr = 0;
	for ( i = 0; i <m_TilesDealer.CurrentLength(); ++i )
	{
		TILE tile = m_TilesDealer.GetTile(i);

		if ( mj.IsHave(tile) )
			continue;
		
		savemj.ReleaseAll();
		savemj.AddTile(tile);
		savemj.AddTile(tile);
		if ( nbr < 2 )
			savemj.AddTile(tile);

		if ( m_TilesDealer.IsHave( tile, savemj.CurrentLength() ) )
		{
			mj.AddTiles(savemj);
			nbr++;
			if ( nbr >=3 )
				break;
		}
		
	}

	if ( nbr < 3 )
		return FALSE;

	m_TilesDealer.DelTiles(mj);

	for ( i = 0; i < mj.CurrentLength(); ++i )
	{
		m_TilesHand[nChair].AddTile(mj.GetTile(i));
	}

	return TRUE;
}

template<int nPlayerNbr>
BOOL CSoTileDealerMgr<nPlayerNbr>::ConfigureZiyise( int nChair )
{
	int nnbr = 0;
	CMJWall mj;
	for ( int i = 0; i < m_TilesDealer.CurrentLength(); ++i )
	{
		TILE tile = m_TilesDealer.GetTile(i);
		if ( tile >= TILE_EAST && tile <= TILE_BAI )
		{
			nnbr++;
			if ( nnbr<8 )
			{
				mj.AddTile(tile);
				m_TilesHand[nChair].AddTile(tile);
			}	
			else
			{
				break;
			}
		}
	}

	m_TilesDealer.DelTiles(mj);
	return TRUE;
}

template<int nPlayerNbr>
int CSoTileDealerMgr<nPlayerNbr>::GetDealInfo( int nChair, TCMD_NOTIFY_DEAL* pCmd )
{
	if ( pCmd == NULL )
		return 1;

	pCmd->ucDealer = m_nDealer;
	pCmd->ucRoundWind = m_nWindRound;
	pCmd->ucRoundWindNbr = m_nWindRoundNbr;

	int i,j; 
	for ( i = 0; i < DICE_NBR && i < 2; ++i )
	{
		pCmd->ucDice[i] = m_nDice[i];
	}

	pCmd->ucGodLength = m_TilesGod.CurrentLength();
	for ( i = 0; i < 8; ++i )
	{
		pCmd->ucGodTiles[i] = m_TilesGod.GetTile( i );
	}

	memset( pCmd->tiles, 0, sizeof(pCmd->tiles) );
	for ( j = 0; j < m_TilesHand[nChair].CurrentLength(); ++j )
	{
		pCmd->tiles[j] = m_TilesHand[nChair].GetTile( j );
	}

	return 0;
}

#else
/************************************************************************/
/* 
C��
                                                                     */
/************************************************************************/
template<int nPlayerNbr>
CGameTileDealerMgr<nPlayerNbr>::CGameTileDealerMgr()
{
	m_nOldGangNbr = -1;
	m_bLastDrawBack = FALSE;
}

template<int nPlayerNbr>
CGameTileDealerMgr<nPlayerNbr>::~CGameTileDealerMgr()
{

}

template<int nPlayerNbr>
void CGameTileDealerMgr<nPlayerNbr>::OnGameMessage(UINT dwLen,  LPBYTE  pGameMsg)
{
	// ֻ���ڱ������������²�����λִ����Ϣ
	// ��Ȼ�ᵼ���ظ�ִ����Ϣ
	CGameTilePlayerMgr<nPlayerNbr>::OnGameMessage(dwLen, pGameMsg);

	if(pGameMsg != NULL && dwLen >= sizeof(TCMD_HEADER))
	{
		TCMD_HEADER *pData = (TCMD_HEADER *)pGameMsg;
		if ( MJFRAME_MOUDLE_MSG == pData->cCmdID )
		{
			switch( pData->cCmdPara )
			{
// 			case SC_MJ_NOTIFY_DEAL:
// 				OnNotifyDeal( dwLen, pGameMsg );
// 				break;
// 			case SC_MJ_NOTIFY_DRAW:
// 				OnNotifyDraw( dwLen, pGameMsg );
// 				break;
			case SC_MJ_NOTIFY_CONFIGTILE:
				OnNotifyConfigTiles( dwLen, pGameMsg );
				break;
			case SC_MJ_NOTIFY_CHANGE_TILES:
				OnNotifyChangeTiles( dwLen, pGameMsg );
				break;
			case SC_MJ_NOTIFY_FINISH:
				OnNotifyFinish( dwLen, pGameMsg );
				break;
			//case SC_MJ_NOTIFY_GIVE:  // dxh del ��һ���Ѿ�����
			//	{
			//		SetLastGiveChair( ((TCMD_NOTIFY_GIVE*)pGameMsg)->chair );
			//		RefreshMJCtrl();
			//	}
			//	break;
			case SC_MJ_NOTIFY_RUN_NBR:
				{
					OnNotifyRunNbr( dwLen, pGameMsg );
				}
				break;
			default:
				break;
			}
		}
	}


}

template<int nPlayerNbr>
void CGameTileDealerMgr<nPlayerNbr>::OnNotifyDeal(UINT nLen, BYTE *pBuf)
{
	if ( pBuf == NULL || nLen == 0 )
		return;

// 	if ( FALSE == CheckIsPlayer() )
// 	{
// 		return;
// 	}

	int i;
	TCMD_NOTIFY_DEAL *pCmd = (TCMD_NOTIFY_DEAL*)pBuf;
	for ( i = 0; i < nPlayerNbr; ++i )
	{
		m_bShowTile[i] = FALSE;
	}
	m_nDealer = pCmd->ucDealer;
// 	m_nNowTurn = m_nDealer;
	for ( i = 0; i < DICE_NBR && i < 2; ++i )
	{
		m_nDice[i] = pCmd->ucDice[i];
	}
	m_TilesGod.CurrentLength( pCmd->ucGodLength );
	for ( i = 0; i < pCmd->ucGodLength; ++i )
	{
		m_TilesGod.SetTile( i, pCmd->ucGodTiles[i] );
	}
	
	m_nWindRound = pCmd->ucRoundWind;
	m_nWindRoundNbr = pCmd->ucRoundWindNbr;
	SetTurn( m_nDealer );
	int nLength = GetMJSetting().nTilesNumber - nPlayerNbr * GetMJSetting().nTilesHandNbr - 1 - m_TilesGod.CurrentLength();
	m_TilesDealer.FillTiles( nLength, 1 );
	
	CGameTilePlayerMgr<nPlayerNbr>::OnNotifyDeal( nLen, pBuf );
}

template<int nPlayerNbr>
void CGameTileDealerMgr<nPlayerNbr>::OnNotifyFlower(UINT nLen, BYTE *pBuf)
{
	if ( pBuf == NULL || nLen == 0 )
		return;

	if ( NULL == m_pStateEvent )
		return;

	TCMD_NOTIFY_FLOWER *pCmd = (TCMD_NOTIFY_FLOWER*)pBuf;
	int i = 0;
	for ( i = 0; i < MJ_FLOWER_NBR; ++i )
	{
		m_nFlower[i] = pCmd->ucFlower[i];
	}


	for ( i = 0; i < pCmd->ucNumber; ++i )
	{
		m_TilesDealer.PopTile();
	}

	CGameTilePlayerMgr<nPlayerNbr>::OnNotifyFlower( nLen, pBuf );
	
	RefreshMJCtrl();
}

template<int nPlayerNbr>
void CGameTileDealerMgr<nPlayerNbr>::OnNotifyDraw(UINT nLen, BYTE *pBuf)
{

	if ( pBuf == NULL || nLen == 0 )
		return;

	TCMD_NOTIFY_DRAW *pCmd = (TCMD_NOTIFY_DRAW*)pBuf;
	SetTurn( pCmd->chair );
    SetShowTurn( pCmd->chair );  // dxh add

	m_TilesDealer.PopTile();

	// ���һ�������ǴӺ�������
	if( GetAllGangNbr() != m_nOldGangNbr && GetMJSetting().nDrawGangBack )
	{
		m_bLastDrawBack = TRUE;
		m_nOldGangNbr = GetAllGangNbr();
	}
	else
	{
		m_bLastDrawBack = FALSE;
	}

	CGameTilePlayerMgr<nPlayerNbr>::OnNotifyDraw( nLen, pBuf );	
}

template<int nPlayerNbr>
void CGameTileDealerMgr<nPlayerNbr>::SetTurn( int nTurn )
{
	CBaseTileDealerMgr<nPlayerNbr>::SetTurn( nTurn );
}

template<int nPlayerNbr>
void CGameTileDealerMgr<nPlayerNbr>::OnNotifyFinish(UINT nLen, BYTE *pBuf)
{
	if (pBuf == NULL || nLen == 0)
    {
        return;
    }

	TCMD_NOTIFY_FINISH *pCmd = (TCMD_NOTIFY_FINISH*)pBuf;
	int i,j;

	// ���ڣ��ӳ�����ȥ������
	if ( pCmd->byFlag == MJ_END_GUN )
	{
		// ���ڵ�Ҫȥ������
		m_TilesGive[pCmd->byWhoGun].DelTile( GetLastTile() );
	}

	// ������Ϣ��������������
	// ��ʾ���
    // ���4�ҵ���
    CMJHand mjHand;
    CMJSet  mjSet;

    SetAllCanSelect();

	for ( i = 0; i < nPlayerNbr; ++i )
	{
        m_bShowTile[i] = TRUE;

        mjHand.ReleaseAll();
        mjSet.ReleaseAll();

		GetTilesSet(i, mjSet);

        for (j = 0; j < MAX_HAND_TILES; j++)
        {
            if (0 == pCmd->tile[i][j])
            {
                // ��0���ݽ���, ��������������
                break;
            }

            // �Է�������������������Ϊ׼
            mjHand.AddTile(pCmd->tile[i][j]);
        }

        // ������������
        SortHandTiles(mjHand, 0, 1, &GetTilesGod());

		if (pCmd->byWhoWin[i])
		{
            // ������� �����л���14����, ��Ҫɾ������������
            mjHand.DelTile(pCmd->tLast);

            // �Ѻ������Ʒŵ����һ��λ��
            mjHand.AddTile(pCmd->tLast);
		}
		
        // ������������
        SetHandTile(i, mjHand);

		// ֻ�滻���ܵ��ƣ��������Ѿ���¼��
		int nPos = 0;
		for (j = 0; j < mjSet.CurrentLength(); j++)
		{
			TILE_SET tagSet;
			mjSet.GetSet(j, tagSet);

			if (tagSet.m_ucFlag == ACTION_QUADRUPLET_CONCEALED)
			{
				tagSet.m_ucTile = pCmd->tSet[i][nPos++];
				mjSet.SetSet(j, tagSet);
			}
		}

		SetSetTile(i, mjSet);
	}    

	m_pStateEvent->SetMJState(MJ_STATE_END);

    // ������Ʊ�־
    memset(m_bBlockEnable, 0, sizeof(m_bBlockEnable));

    // ˢ�������������
    RefreshMJCtrl(TRUE, TRUE);
}

template<int nPlayerNbr>
void CGameTileDealerMgr<nPlayerNbr>::OnNotifyConfigTiles( UINT nLen, BYTE *pBuf )
{
#if 0
	if ( pBuf == NULL || nLen == 0 )
		return;

	TCMD_NOTIFY_CONFIGTILE *pCmd = (TCMD_NOTIFY_CONFIGTILE*)pBuf;
	IUIManager* pUIMgr = GetUIManager();
	if ( FALSE == pCmd->bResult )
	{
		if ( pUIMgr )
		{
			//pUIMgr->MessageBox( "ʹ��Ǭ����Ų��ʧ�ܣ�ʹ��ʱ�����󣬱���Ҫ�ڷ���Ϸ��ʹ�ã�", MB_OK );
		}
	}
	else 
	{
		if ( pUIMgr )
		{
			//pUIMgr->MessageBox( "ʹ��Ǭ����Ų�Ƴɹ���", MB_OK );
		}

	}
#endif
}

template<int nPlayerNbr>
void CGameTileDealerMgr<nPlayerNbr>::OnNotifyChangeTiles( UINT nLen, BYTE *pBuf )
{
	if ( pBuf == NULL || nLen == 0 )
		return;

	TCMD_NOTIFY_CHANGE_TILES *pCmd = (TCMD_NOTIFY_CHANGE_TILES*)pBuf;
	if ( FALSE == pCmd->ucResult )
	{
		// ʹ��ʧ��,����������ѡ

#if 0   // �ں����ټ��ϴ�Щ����
		IUIManager* pUIMgr = GetUIManager();
		if ( pUIMgr )
		{
			pUIMgr->MessageBox( "ʹ��͵�컻��ʧ�ܣ�ʹ�ó�ʱ���߳�ǽ���Ѿ�û���������ˣ�", MB_OK );
		}
#endif
		return;
	}

	_DOUT("ʹ��͵�컻��!");
	int nChair = GetLocalChair();

	m_TilesHand[nChair].DelTile( pCmd->ucRequestTile );
	m_TilesHand[nChair].AddTile( pCmd->ucTargetTile );
	SetLastTile( pCmd->ucTargetTile );
	SortHandTiles( m_TilesHand[nChair], 0, 1, &GetTilesGod() );
	// ��������ƣ�����֮

	ClearLoseHu( nChair );
	if ( CheckFlower( nChair ) )
	{
		// ������Ϣ���󲹻��������ڵȴ�״̬
		RequestFlower( nChair );
		m_pStateEvent->SetMJState( MJ_STATE_WAIT );
	}
	else if ( CheckBlock( GetLocalChair() ) )
	{
		m_pStateEvent->SetMJState( MJ_STATE_BLOCK );
	}
	else
	{
		m_pStateEvent->SetMJState( MJ_STATE_GIVE );
	}

	RefreshMJCtrl();

	
}

template<int nPlayerNbr>
void CGameTileDealerMgr<nPlayerNbr>::OnNotifyRunNbr( UINT nLen, BYTE *pBuf )
{
	if ( pBuf == NULL || nLen == 0 )
		return;

	TCMD_NOTIFY_RUN_NBR *pCmd = (TCMD_NOTIFY_RUN_NBR*)pBuf;
	m_nRunNbr = pCmd->nRunNbr;
	m_nRunWinNbr = pCmd->nRunWinNbr;
	m_nRunNotileNbr = pCmd->nRunNotileNbr;
}


#endif

}
