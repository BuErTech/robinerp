// BaseTilePlayerMgr.cpp: implementation of the CBaseTilePlayerMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseTilePlayerMgr.h"

#ifndef _WINDOWS
    #define OutputDebugString(p) {;}
    #define _snprintf  snprintf
#endif
        
namespace MyGame
{
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


template<int nPlayerNbr>
CBaseTilePlayerMgr<nPlayerNbr>::CBaseTilePlayerMgr()
{
	memset( m_bBlockEnable, 0, sizeof(m_bBlockEnable) );
	memset( m_nTingType, 0, sizeof(m_nTingType) );
	m_ucCurCheckChair = 0;
	m_bIsSet = FALSE;
}


template<int nPlayerNbr>
CBaseTilePlayerMgr<nPlayerNbr>::~CBaseTilePlayerMgr()
{

}

/************************************************************************/
/* ��Ϸ����    
*/
/************************************************************************/
template<int nPlayerNbr>
void CBaseTilePlayerMgr<nPlayerNbr>::InitForNext()
{
	memset( m_bBlockEnable, 0, sizeof(m_bBlockEnable) );
	memset( m_nTingType, 0, sizeof(m_nTingType) );
}

/************************************************************************/
/* ����2���¼��ӿ�                                                      */
/************************************************************************/
template<int nPlayerNbr>
void CBaseTilePlayerMgr<nPlayerNbr>::SetStateEvent( IMJStateEvent* pStateEvent )
{
	m_pStateEvent = pStateEvent;
}

template<int nPlayerNbr>
void CBaseTilePlayerMgr<nPlayerNbr>::SetTileEvent( IMJTileEvent* pTileEvent )
{
	m_pTileEvent = pTileEvent;
}

/************************************************************************/
/* ��������,�Ƿ�Ҫ�������                                              */
/************************************************************************/
template<int nPlayerNbr>
void CBaseTilePlayerMgr<nPlayerNbr>::SetHandTile( int nChair, CMJHand &mjHand, BOOL bNew )
{
	if ( nChair < 0 || nChair >= nPlayerNbr )
		return;

	if ( TRUE == bNew )
	{
		m_TilesHand[nChair].ReleaseAll();
	}

	m_TilesHand[nChair].AddTiles( mjHand );
}

template<int nPlayerNbr>
void CBaseTilePlayerMgr<nPlayerNbr>::SetSetTile( int nChair, CMJSet &mjSet, BOOL bNew )
{
	if ( nChair < 0 || nChair >= nPlayerNbr )
		return;

	if ( TRUE == bNew )
	{
		m_TilesSet[nChair].ReleaseAll();
	}

	m_TilesSet[nChair].AddSet( mjSet );
}

/************************************************************************/
/* 
����  
*/
/************************************************************************/ 
template<int nPlayerNbr>
void CBaseTilePlayerMgr<nPlayerNbr>::AddHandTile( int nChair, TILE ucTile )
{
	if ( nChair < 0 || nChair >= nPlayerNbr )
		return;

	if ( GetTileGenre( ucTile ) < TILE_GENRE_FLOWER )
	{
		m_TilesHand[nChair].AddTile( ucTile );
	}
}

/************************************************************************/
/* 
����
ָ����ҵ�TILE���ɹ�����TRUE
*/
/************************************************************************/
template<int nPlayerNbr>
BOOL CBaseTilePlayerMgr<nPlayerNbr>::DelHandTile( int nChair, TILE ucTile )
{	
	if ( nChair < 0 || nChair >= nPlayerNbr )
		return FALSE;

	return m_TilesHand[nChair].DelTile( ucTile );
}

/************************************************************************/
/* �����й�                                                                     */
/************************************************************************/
template<int nPlayerNbr>
void CBaseTilePlayerMgr<nPlayerNbr>::SetTing( int nChair, BOOL bTing )
{
	if ( nChair < 0 || nChair >= nPlayerNbr )
		return;

	if ( bTing )
	{
		if ( m_TilesSet[nChair].CurrentLength() == 0 
			&&
			m_TilesGive[nChair].CurrentLength() == 0 )
		{
			m_nTingType[nChair] = MJ_TING_BEGIN;
		}
		else
		{
			m_nTingType[nChair] = MJ_TING_NORMAL;
		}
	}
	else
	{
		m_nTingType[nChair] = MJ_TING_NONE;
	}
}

template<int nPlayerNbr>
BOOL CBaseTilePlayerMgr<nPlayerNbr>::GetTing( int nChair )
{
	if ( nChair < 0 || nChair >= nPlayerNbr )
		return FALSE;

	if ( m_nTingType[nChair] == MJ_TING_NONE )
		return FALSE;
	return TRUE;
}

template<int nPlayerNbr>
int CBaseTilePlayerMgr<nPlayerNbr>::GetTilesHand( int nChair, CMJHand& mjHand )
{
	if ( nChair < 0 || nChair >= nPlayerNbr )
		return 0;

	mjHand.ReleaseAll();
	mjHand.AddTiles( m_TilesHand[nChair] );
	return mjHand.CurrentLength();
}

template<int nPlayerNbr>
int CBaseTilePlayerMgr<nPlayerNbr>::GetTilesSet( int nChair, CMJSet& mjSet )
{
	if ( nChair < 0 || nChair >= nPlayerNbr )
		return 0;

// 	mjSet.ReleaseAll();
// 	mjSet.AddSet( m_TilesHand[nChair] );
	memcpy( &mjSet, &m_TilesSet[nChair], sizeof(mjSet) );
	return mjSet.CurrentLength();
}

template<int nPlayerNbr>
int CBaseTilePlayerMgr<nPlayerNbr>::GetTilesGive( int nChair, CMJGive& mjGive )
{
	if ( nChair < 0 || nChair >= nPlayerNbr )
		return 0;

// 	mjGive.ReleaseAll();
// 	mjGive.AddTiles( m_TilesGive[nChair] );
	memcpy( &mjGive, &m_TilesGive[nChair], sizeof(mjGive) );
	return mjGive.CurrentLength();
}

template<int nPlayerNbr>
BOOL CBaseTilePlayerMgr<nPlayerNbr>::GetShowTile( int nChair )
{
	if ( nChair < 0 || nChair >= nPlayerNbr )
		return FALSE;
	
	return m_bShowTile[nChair];
}

template<int nPlayerNbr>
short CBaseTilePlayerMgr<nPlayerNbr>::CheckBlockSelfWin(  int nChair, short &shBlockLevel, TILE ucLastTile )
{
	// �Լ�����
	// ��
	if ( TRUE == IsWin( m_TilesHand[nChair], m_TilesSet[nChair] ) )
	{
		if ( GetMJSetting().nBlockLevelWin > 0 )
		{
			m_bBlockEnable[MJ_BLOCK_WIN] = TRUE;
			if ( shBlockLevel < GetMJSetting().nBlockLevelWin )
			{
				shBlockLevel = GetMJSetting().nBlockLevelWin;
			}

			CMJHand mjGodTrans= GetTilesGodTrans();
			m_TilesPlayerGod[nChair].ReleaseAll();
			m_TilesPlayerGod[nChair].AddTiles( mjGodTrans );
		}
	}

	return shBlockLevel;
}

template<int nPlayerNbr>
short CBaseTilePlayerMgr<nPlayerNbr>::CheckBlockSelfQuadruplet(  int nChair, short &shBlockLevel, TILE ucLastTile )
{
	if ( TRUE == IsCanQuadrupletSelf( m_TilesHand[nChair], m_TilesSet[nChair] ) )
	{
		BOOL bRight = FALSE;

		if ( 0 == GetTing( nChair ) )
		{
			bRight = TRUE;
		}

		// ������˺��������˶���û������ɸܣ��������
		if ( GetTing( nChair ) && 1 == GetMJSetting().nTingCanGang )
		{
			// ����Ϊ������
			bRight = FALSE;

			int i;
			for ( i = 0; i < m_TilesSet[nChair].CurrentLength(); ++ i )
			{	
				if ( bRight )
					break;
				TILE_SET tagSet;
				m_TilesSet[nChair].GetSet( i, tagSet );
				if ( ACTION_TRIPLET == tagSet.m_ucFlag && TRUE == m_TilesHand[nChair].IsHave( tagSet.m_ucTile[0] ) )
				{
					CMJHand mjTemp;
					mjTemp.AddTiles( m_TilesHand[nChair] );
					mjTemp.DelTile( tagSet.m_ucTile[0] );

					CMJSet mjSet;
					mjSet.AddSet( m_TilesSet[nChair] );
					tagSet.m_ucFlag = ACTION_QUADRUPLET_REVEALED;
					mjSet.SetSet( i, tagSet );

                    // �ж��Ƿ���Ժ���
                    if (IsWin(mjTemp, mjSet, 1))
					{
						bRight = TRUE;
					}
				}
			}

			// �Լ��Ƿ���4��
			for ( i = 0; i < m_TilesHand[nChair].CurrentLength(); ++i )
			{
				if ( bRight )
                {
                    break;
                }
				
                TILE tile = m_TilesHand[nChair].GetTile(i);

				if ( TRUE == m_TilesHand[nChair].IsHave( tile, 4 ) )
				{
					CMJHand mjTemp;
					mjTemp=  m_TilesHand[nChair];
					mjTemp.DelTile( tile, 4 );

					CMJSet mjSet;
					mjSet = m_TilesSet[nChair];

                    BYTE byTile[4];
                    byTile[0] = tile;
                    byTile[1] = tile;
                    byTile[2] = tile;
                    byTile[3] = tile;
					mjSet.AddSet( /*ucLastTile*/byTile, ACTION_QUADRUPLET_CONCEALED, nChair );

                    // dxh �ж��Ƿ���Ժ���, IsTing�ĳ�IsWin
					if (IsWin(mjTemp, mjSet, 1))
					{
						bRight = TRUE;
					}
				}
			}
		}

		if ( GetMJSetting().nBlockLevelQuadruplet > 0 && bRight )
		{
			m_bBlockEnable[MJ_BLOCK_QUADRUPLET] = TRUE;
			if ( shBlockLevel < GetMJSetting().nBlockLevelQuadruplet )
			{
				shBlockLevel = GetMJSetting().nBlockLevelQuadruplet;
			}
		}
	}

	return shBlockLevel;
}

template<int nPlayerNbr>
short CBaseTilePlayerMgr<nPlayerNbr>::CheckBlockTing(  int nChair, short &shBlockLevel, TILE ucLastTile )
{
	if ( GetMJSetting().nBlockLevelTing > 0 )
	{
		if ( TRUE == CheckTing( nChair ) )
		{
			m_bBlockEnable[MJ_BLOCK_TING] = TRUE;
			if ( shBlockLevel < GetMJSetting().nBlockLevelTing )
			{
				shBlockLevel = GetMJSetting().nBlockLevelTing;
			}
		}
	}

	return shBlockLevel;
}

template<int nPlayerNbr>
short CBaseTilePlayerMgr<nPlayerNbr>::CheckBlockCollect( int nChair,  short &shBlockLevel, TILE ucLastTile, int nGiveChair )
{
	// ��������������������ˣ��Ͳ��ܳ���
	if ( m_TilesGod.IsHave( ucLastTile ) && 0 == GetMJSetting().nCanWinLaizi )
	{
		return shBlockLevel;
	}

	// ����������������
	if (GetTing(nChair))
	{
		return shBlockLevel;
	}

	// ��
	if (IsCanCollect(m_TilesHand[nChair], ucLastTile, nGiveChair, nChair, nPlayerNbr))
	{		
		if (GetMJSetting().nBlockLevelCollect > 0)
		{
			m_bBlockEnable[MJ_BLOCK_COLLECT] = TRUE;
			if ( shBlockLevel < GetMJSetting().nBlockLevelCollect )
			{
				shBlockLevel = GetMJSetting().nBlockLevelCollect;
			}
		}
	}

	return shBlockLevel;
}

template<int nPlayerNbr>
short CBaseTilePlayerMgr<nPlayerNbr>::CheckBlockTriplet( int nChair, short &shBlockLevel, TILE ucLastTile )
{
	// ��������������������ˣ��Ͳ�������
	if ( m_TilesGod.IsHave( ucLastTile ) && 0 == GetMJSetting().nCanWinLaizi )
	{
		return shBlockLevel;
	}

	if ( TRUE == IsCanTriplet( m_TilesHand[nChair], ucLastTile ) && FALSE == GetTing( nChair ) )
	{
		if ( GetMJSetting().nBlockLevelTriplet > 0 )
		{
			m_bBlockEnable[MJ_BLOCK_TRIPLET] = TRUE;
			if ( shBlockLevel < GetMJSetting().nBlockLevelTriplet )
			{
				shBlockLevel = GetMJSetting().nBlockLevelTriplet;
			}
		}
	}

	return shBlockLevel;
}

template<int nPlayerNbr>
short CBaseTilePlayerMgr<nPlayerNbr>::CheckBlockQuadruplet(  int nChair, short &shBlockLevel, TILE ucLastTile )
{
	// ��������������������ˣ��Ͳ��ܸ���
	if ( m_TilesGod.IsHave( ucLastTile ) && 0 == GetMJSetting().nCanWinLaizi )
	{
		return shBlockLevel;
	}

	if ( m_pTileEvent == NULL )
	{
		return 0;
	}

	// Ҫôû����Ҫô�������˺�����
	if ( TRUE == IsCanQuadruplet( m_TilesHand[nChair], ucLastTile ) )
	{
		BOOL bRight = FALSE;
		if ( FALSE == GetTing( nChair ) )
		{
			bRight = TRUE;
		}

		if ( GetTing( nChair ) && GetMJSetting().nTingCanGang )
		{
			CMJHand mjTemp, mjTing;
			mjTemp.AddTiles( m_TilesHand[nChair] );
			mjTemp.DelTile( ucLastTile );
			mjTemp.DelTile( ucLastTile );
			mjTemp.DelTile( ucLastTile );

			CMJSet mjSet;
			mjSet.AddSet( m_TilesSet[nChair] );

            BYTE byTile[4];
            byTile[0] = ucLastTile;
            byTile[0] = ucLastTile;
            byTile[0] = ucLastTile;
            byTile[0] = ucLastTile;
			mjSet.AddSet( byTile/*ucLastTile*/, ACTION_QUADRUPLET_REVEALED, m_pTileEvent->GetTurn() );
			if ( IsTing( mjTemp, mjSet, mjTing ) )
			{
				bRight = TRUE;
			}
		}

		if ( GetMJSetting().nBlockLevelQuadruplet > 0 && bRight )
		{
			m_bBlockEnable[MJ_BLOCK_QUADRUPLET] = TRUE;
			if ( shBlockLevel < GetMJSetting().nBlockLevelQuadruplet )
			{
				shBlockLevel = GetMJSetting().nBlockLevelQuadruplet;
			}
		}
	}

	return shBlockLevel;
}

template<int nPlayerNbr>
short CBaseTilePlayerMgr<nPlayerNbr>::CheckBlockWinOther(  int nChair, short &shBlockLevel, TILE ucLastTile )
{
	// ������˴�������Ʋ��ܺ�
	if ( m_TilesGod.IsHave( ucLastTile ) && 0 == GetMJSetting().nCanWinLaizi )
	{
		return shBlockLevel;
	}

 	if ( TRUE == IsCanWin( m_TilesHand[nChair], m_TilesSet[nChair], ucLastTile ) 
		&&
		FALSE == CheckLoseHu( nChair, ucLastTile ) )
	{
		if ( GetMJSetting().nBlockLevelWin > 0 )
		{
			m_bBlockEnable[MJ_BLOCK_WIN] = TRUE;
			SetLoseHu( nChair, ucLastTile );
			
			if ( shBlockLevel < GetMJSetting().nBlockLevelWin )
			{
				shBlockLevel = GetMJSetting().nBlockLevelWin;
			}

			CMJHand mjGodTrans= GetTilesGodTrans();
			m_TilesPlayerGod[nChair].ReleaseAll();
			m_TilesPlayerGod[nChair].AddTiles( mjGodTrans );

		}
	}

	return shBlockLevel;
}

/**************************************************************************/
/*   
�ж�����
bSelf�Ƿ�ֻ�ж��Լ������ֻ�ж��Լ�����2������û������
������жϱ��˳��Ƶģ�ucLastTile�����������������
����������ƣ�������ߵȼ�������
*/
/************************************************************************/
template<int nPlayerNbr>
short CBaseTilePlayerMgr<nPlayerNbr>::CheckBlock(  int nChair, BOOL bSelf, TILE ucLastTile, short shLevel )
{
	if (nChair < 0 || nChair >= nPlayerNbr)
    {
        return 0;
    }

	if (m_pTileEvent == NULL)
	{
		return 0;
	}

	// �������ڱ��жϵ���
	m_ucCurCheckChair = nChair;

    // �����������
    m_tmpBlockSet.ReleaseAll();
    m_tmpTingGive.ReleaseAll();

	short shBlockLevel = 0;
	memset(m_bBlockEnable, 0, sizeof(m_bBlockEnable));

	if ( TRUE == bSelf )
	{		
		// ��������˵ĵȼ�
		memset(m_nBlockLevel, 0, sizeof(m_nBlockLevel));

		// �Լ�����
		CheckBlockSelfWin(nChair, shBlockLevel, ucLastTile);
		
		// ��
		CheckBlockSelfQuadruplet(nChair, shBlockLevel, ucLastTile);

		// ��
		CheckBlockTing(nChair, shBlockLevel, ucLastTile);
	}
	else
	{
        // �����˵���
		// ���Լ��ĵȼ����
		if (nChair == m_pTileEvent->GetTurn())
        {
            m_nBlockLevel[nChair] = 0;
        }

		// ��
		CheckBlockCollect(nChair, shBlockLevel, ucLastTile, m_pTileEvent->GetTurn());

		// ��
		CheckBlockTriplet(nChair, shBlockLevel, ucLastTile);
		
		// ��
		CheckBlockQuadruplet(nChair, shBlockLevel, ucLastTile);

		// ��
		CheckBlockWinOther(nChair, shBlockLevel, ucLastTile);

	}

    _DOUT2("���%d, ���������ж�, ���Ƶȼ�Ϊ=%d\n", nChair, shBlockLevel);

	// ������ȼ����ڹ涨�ȼ�����Ϊ��������
	if (shBlockLevel < shLevel)
    {
        shBlockLevel = 0;
    }

	m_nBlockLevel[nChair] = shBlockLevel;

	return shBlockLevel;
}


template<int nPlayerNbr>
BOOL CBaseTilePlayerMgr<nPlayerNbr>::CheckTing( int nChair )
{
	m_bBlockEnable[MJ_BLOCK_TING] = FALSE;

	if ( GetMJSetting().nBlockLevelTing > 0 )
	{
		CMJHand mjTing;
		if ( TRUE == IsTing( m_TilesHand[nChair], m_TilesSet[nChair], mjTing ) && FALSE == GetTing( nChair ) )
		{
			m_bBlockEnable[MJ_BLOCK_TING] = TRUE;

            // ��¼���ƺ���Դ��ȥ����
            m_tmpTingGive.ReleaseAll();
            m_tmpTingGive.AddTiles(mjTing);

			return TRUE;
		}
	}

	return FALSE;
}


/************************************************************************/
/* 
�жϲ�����
bAllSend �Ƿ������Ƿ��л��ƶ����󲹻�
���ز��˼��Ż�
*/
/************************************************************************/
template<int nPlayerNbr>
int CBaseTilePlayerMgr<nPlayerNbr>::CheckFlower(  int nChair )
{
	if ( nChair < 0 || nChair >= nPlayerNbr )
		return 0;

	int nFlowerNbr = 0;
	for ( int i = 0; i < m_TilesHand[nChair].CurrentLength(); ++i )
	{
		TILE ucTile = m_TilesHand[nChair].GetTile( i );

		if ( GetTileGenre( ucTile ) == TILE_GENRE_FLOWER )
		{
			nFlowerNbr++;
		}
	}

	return nFlowerNbr;
}

template<int nPlayerNbr>
void CBaseTilePlayerMgr<nPlayerNbr>::SetLastTile( TILE ucTile )
{
	// ������������
	m_ucLastTile = ucTile;
}

template<int nPlayerNbr>
void CBaseTilePlayerMgr<nPlayerNbr>::ClearLoseHu( int nChair )
{
	if ( FALSE == GetMJSetting().nLouHu )
	{
		return;
	}

	if ( nChair >= 0 && nChair < nPlayerNbr )
	{
		// ��������Ϊ0
		memset( m_ucLoseHu[nChair], 0, sizeof(m_ucLoseHu[nChair]) );
	}
}

template<int nPlayerNbr>
void CBaseTilePlayerMgr<nPlayerNbr>::SetLoseHu( int nChair, TILE ucTile )
{
	if ( FALSE == GetMJSetting().nLouHu )
	{
		return;
	}

	if ( nChair < 0 || nChair >= nPlayerNbr )
		return;

	if ( ucTile > TILE_BAI )
		return;

	int length = m_ucLoseHu[nChair][MAX_HAND_TILES-1];
	// �����˼�¼��Χ�������ϲ�����
	if ( length >= MAX_HAND_TILES - 1 || length < 0 )
	{
		return;
	}

	m_ucLoseHu[nChair][length] = ucTile;
	m_ucLoseHu[nChair][MAX_HAND_TILES-1] = length + 1;
}

template<int nPlayerNbr>
BOOL CBaseTilePlayerMgr<nPlayerNbr>::CheckLoseHu( int nChair, TILE ucTile )
{
	if ( FALSE == GetMJSetting().nLouHu )
	{
		return FALSE;
	}

	if ( nChair < 0 || nChair >= nPlayerNbr )
		return FALSE;

	if ( ucTile > TILE_BAI )
		return FALSE;
	
	int length = m_ucLoseHu[nChair][MAX_HAND_TILES-1];
	// �����˼�¼��Χ�������ϲ�����
	if ( length > MAX_HAND_TILES-1 || length < 0 )
	{
		return FALSE;
	}

	// ��������ȷ���Ƿ��Ǵ�����©������
	for ( int i = 0; i < length; ++i )
	{
		if ( ucTile == m_ucLoseHu[nChair][i] )
		{
			return TRUE;
		}
	}

	return FALSE;	
}

template<int nPlayerNbr>
void CBaseTilePlayerMgr<nPlayerNbr>::SortHandTiles( CMJHand &mjHand, int nMod, int nGod, CMJHand* ptilesGod )
{
    int nLength = mjHand.CurrentLength();

	if ( nGod == 0 || ptilesGod == NULL || ptilesGod->CurrentLength() == 0 )
	{
		mjHand.Sort( nMod );
	}
	else
	{
		mjHand.Sort( nMod, nGod, ptilesGod );
	}

    if (nLength != mjHand.CurrentLength())
    {
        _DOUT2("�齫�����������, ����ǰ������=%d, �����������=%d", nLength, mjHand.CurrentLength());

        char szText[250];
        sprintf(szText, "dxhout: �齫�����������, ����ǰ������=%d, �����������=%d", nLength, mjHand.CurrentLength());
        OutputDebugString(szText);
    }
}

template<int nPlayerNbr>
BOOL CBaseTilePlayerMgr<nPlayerNbr>::CheckIsAutoWinTile()
{
	return FALSE;
}

template<int nPlayerNbr>
void CBaseTilePlayerMgr<nPlayerNbr>::SetLastGiveChair(BYTE byChair)
{
	m_byLastChair = byChair;
}

template<int nPlayerNbr>
BYTE CBaseTilePlayerMgr<nPlayerNbr>::GetLastGiveChair()
{
	return m_byLastChair;
}

template<int nPlayerNbr>
int CBaseTilePlayerMgr<nPlayerNbr>::GetDrawWallBackNbr()
{
	int nnbr = 0;
	// ��ȡ�ĸ�������Ӧ������ǰ�����������
	// ��Ϊ�µ������ڸܺ�����ǰ���Ѿ����͹�����
	// ��������������ᵼ���ٻ�һ����
	if ( GetMJSetting().nDrawGangBack )
	{ 
		if ( m_nOldGangNbr > 0 )
			nnbr = m_nOldGangNbr; 
	}

	return nnbr;
}
/*
SO�˵�
*/
#ifndef WINDOWS_LOGIC
template<int nPlayerNbr>
CSoTilePlayerMgr<nPlayerNbr>::CSoTilePlayerMgr()
{
	m_ucEndType = MJ_END_NO;
	memset( m_bWiner, 0, sizeof(m_bWiner) );
}

template<int nPlayerNbr>
CSoTilePlayerMgr<nPlayerNbr>::~CSoTilePlayerMgr()
{

}

/************************************************************************/
/* ������Ϸ��Ϣ                                                         */
/************************************************************************/
template<int nPlayerNbr>
int  CSoTilePlayerMgr<nPlayerNbr>::OnGameMessage(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen)
{
	if(pcGameData != NULL && nDataLen >= sizeof(TCMD_HEADER))
	{
		TCMD_HEADER *pData = (TCMD_HEADER*)pcGameData;

		if ( MJFRAME_MOUDLE_MSG == pData->cCmdID )
		{
            _DOUT3("----------����CSoTilePlayerMgr::OnGameMessage(%d, %d), ���ݰ���С(%d)------------", pData->cCmdID, pData->cCmdPara, nDataLen);
			switch( pData->cCmdPara )
			{
			case CS_MJ_REQUEST_GIVE:
				ProcessRequestGive( cMsgID, nPlayerID, nChair, pcGameData, nDataLen );
                return TRUE;
			case CS_MJ_REQUEST_BLOCK:
				ProcessRequestBlock( cMsgID, nPlayerID, nChair, pcGameData, nDataLen );
                return TRUE;
			case CS_MJ_REQUEST_FLOWER:
				ProcessRequestFlower( cMsgID, nPlayerID, nChair, pcGameData, nDataLen );
                return TRUE;
			default:
				break;
			}
		}
	}

	return FALSE;
}

/**
*  @brief �ж�����
*  @param[in]  nChair �жϵ����
*  @param[in]  bSelf �Ƿ�ֻ�ж��Լ�
*  @param[in]  ucLastTile ���һ�ű��˳�����,�����ֻ�ж��Լ�,���������û������
*  @param[in]  shLevel �趨���Ƶȼ������ڵȼ���Ҳ����Ϊ��������
*  @return  �������Ƶȼ�
*/	
template<int nPlayerNbr>
short CSoTilePlayerMgr<nPlayerNbr>::CheckBlock(  int nChair, BOOL bSelf, TILE ucLastTile, short shLevel )
{
    if (nChair < 0 || nChair >= nPlayerNbr)
    {
        return 0;
    }

    if (m_pTileEvent == NULL)
    {
        return 0;
    }

    // �������ڱ��жϵ���
    m_ucCurCheckChair = nChair;

    // �����������
    m_tmpBlockSet.ReleaseAll();
    m_tmpTingGive.ReleaseAll();

    short shBlockLevel = 0;
    memset(m_bBlockEnable, 0, sizeof(m_bBlockEnable));

    if ( TRUE == bSelf )
    {		
        // ��������˵ĵȼ�
        memset(m_nBlockLevel, 0, sizeof(m_nBlockLevel));

        // �Լ�����
        CheckBlockSelfWin(nChair, shBlockLevel, ucLastTile);

        // ��
        CheckBlockSelfQuadruplet(nChair, shBlockLevel, ucLastTile);

        // ��
        CheckBlockTing(nChair, shBlockLevel, ucLastTile);
    }
    else
    {
        // �����˵���
        // ���Լ��ĵȼ����
        if (nChair == m_pTileEvent->GetTurn())
        {
            m_nBlockLevel[nChair] = 0;
        }

        if (m_shGangState != MJ_GANG_STATE_BLOCK)
        {
            // ��
            CheckBlockCollect(nChair, shBlockLevel, ucLastTile, m_pTileEvent->GetTurn());

            // ��
            CheckBlockTriplet(nChair, shBlockLevel, ucLastTile);

            // ��
            CheckBlockQuadruplet(nChair, shBlockLevel, ucLastTile);
        }

        // ��
        CheckBlockWinOther(nChair, shBlockLevel, ucLastTile);
    }

    _DOUT2("���%d, ���������ж�, ���Ƶȼ�Ϊ=%d\n", nChair, shBlockLevel);

    // ������ȼ����ڹ涨�ȼ�����Ϊ��������
    if (shBlockLevel < shLevel)
    {
        shBlockLevel = 0;
    }

    m_nBlockLevel[nChair] = shBlockLevel;

    if (!bSelf && shBlockLevel != 0)
    {
        // ����������Ϣ
        GetSoGameProcess()->SendGameDataToSeat(nChair, (char*)&GetBlockInfo(nChair), sizeof(TCMD_NOTIFY_BLOCK));
    }

    return shBlockLevel;
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::CheckGiveBlock( int nChair, BYTE ucTile )
{
	ISoUserManager* pUserMgr = GetSoUserManager();
	if ( NULL == pUserMgr )
	{
		return 1;
	}

	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return 1;
	}

	// �ж����������Ʋ��������ƾ���ס
	BOOL bBlock = FALSE;
	for ( int i = 0; i < nPlayerNbr; ++i )
	{
		m_nBlockLevel[i] = 0;

		// ���Ƶ��˲���
		if ( i != nChair )
		{
			m_nBlockLevel[i] = CheckBlock( i, FALSE, ucTile );
		}

		if ( m_nBlockLevel[i] > 0 )
        {
            bBlock = TRUE;
        }
	}

	if ( TRUE == bBlock )
	{
        // ֪ͨ���пͻ������������
        // ��Ҫ���þ�����������Ҷ�֪��������������ĳ�˸���ס��(��: �����ܺ�)
        for (int i = 0; i < nPlayerNbr; i++)
        {
            TCMD_NOTIFY_BLOCK cmd;
            cmd.ucChair = m_pTileEvent->GetTurn();  // ��ǰ�����Ƶ���
            cmd.ucFlag = ACTION_EMPTY;

            if (m_nBlockLevel[i] == 0)
            {
                GetSoGameProcess()->SendGameDataToSeat(i, (char*)&cmd, sizeof(cmd));
            }
        }

		// ��������״̬
		m_pStateEvent->SetMJState( MJ_STATE_BLOCK );

		for ( int j = 0; j < nPlayerNbr; ++j )
		{
			if (m_nBlockLevel[j] > 0 /*&& IsOffline( j ) */)
			{
				ISoPlayer* pUser = pUserMgr->GetPlayerByChair( j );
				if (pUser != NULL)
				{
                    if (pUser->IsForceExit() || pUser->IsOffline())
                    {
                        SoTrustOffline( j, pUser->GetUserID() );
                    }
				}
			}
		}
	}
	else
	{
		// ������������ƶ�
		int nTurn = ( m_pTileEvent->GetTurn() + GetMJSetting().nGiveOrder ) % nPlayerNbr;
		m_pTileEvent->SetTurn( nTurn );

		// �������Ƶȴ�
		m_pStateEvent->SetMJState(MJ_STATE_DRAW_WAIT);

		//m_pStateEvent->SetMJState( MJ_STATE_DRAW );
	}	

	return 0;
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::CheckDrawBlock( int nChair )
{
	if ( m_pStateEvent == NULL )
		return 0;

	// �ж��Լ������Ʋ�
	if ( CheckFlower( nChair ) )
	{
		// ����л����������״̬�ȣ���TIMEOUTGIVE��ʱ���ж������
		m_pStateEvent->SetMJState( MJ_STATE_GIVE );
	}
	else if ( CheckBlock( nChair ) )
	{
		m_pStateEvent->SetMJState( MJ_STATE_BLOCK );

		// ���nTurn���ߣ�SO��æ����
		/*if ( IsOffline( nChair ) )
		{
			ISoUserManager* pUserMgr = GetSoUserManager();
			if ( pUserMgr )
			{
				ISoPlayer* pUser = pUserMgr->GetPlayerByChair( nChair );
				if ( pUser )
				{
					SoTrustOffline( nChair, pUser->GetUserID() );
				}
			}
		}*/
        
        ISoUserManager* pUserMgr = GetSoUserManager();
        if ( pUserMgr != NULL )
        {
            ISoPlayer* pUser = pUserMgr->GetPlayerByChair( nChair );
            if ( pUser != NULL)
            {
                if (pUser->IsOffline() || pUser->IsForceExit())
                {
                    SoTrustOffline( nChair, pUser->GetUserID() );
                }
            }
        }
        
        return 1;
	}
	else
	{
		m_pStateEvent->SetMJState( MJ_STATE_GIVE );
	}

	return 0;
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::ProcessRequestGive(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen)
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

	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return 1;
	}

	//Ϊ���Զ���
	if ( pProcessor->GetGameState() != gsPlaying )
	{
		return 1;
	}

	TCMD_REQUEST_GIVE* pCmd = (TCMD_REQUEST_GIVE*)pcGameData;
	int nCurTurn = m_pTileEvent->GetTurn();
	TILE ucTileGive = pCmd->t;

	// ֻ�е�ǰ������Ҳſ��Է���
	if ( nChair != nCurTurn )
	{
		return 2;
	}

	// �����Ʋ����ǻ���
	/*if ( GetTileGenre(ucTileGive) >= TILE_GENRE_FLOWER )
	{
		return 3;
	}*/

	// ����ʱ�����Ƴ�����Ҫ��
	if ( m_TilesHand[nChair].CurrentLength() % 3 != 2 )
	{
		_DOUT2("���λ��:%d, ����ʱ�����Ƴ���(%d) ����ȷ, ����ˢ������", nChair, m_TilesHand[nChair].CurrentLength());

		// ���Ȳ�ͬ������ˢ������
		UpdateHandTiles( nChair );

		return 4;
	}

	// ��������Ƿ��������ƣ���Ϊִ�гɹ�
	if ( FALSE == m_TilesHand[nChair].IsHave( ucTileGive ) )
	{
		ucTileGive = m_TilesHand[nChair].GetTile( m_TilesHand[nChair].CurrentLength() - 1 );
	}
	
	if ( m_TilesHand[nChair].DelTile(ucTileGive) )
	{
        // ���Ʋ�������
        if (GetTileGenre(ucTileGive) < TILE_GENRE_FLOWER)
        {
            m_TilesGive[nChair].AddTile(ucTileGive);
        }
	}

	// ���浱ǰ���ץ��������
	TILE bLastDraw = GetLastTile();

	SetLastTile( ucTileGive );
	SetLastGiveChair( nChair );

	TCMD_NOTIFY_GIVE cmd;
	cmd.chair = m_pTileEvent->GetTurn();
	cmd.t = ucTileGive;
	cmd.ucHandType = rand()%5;       // ����ʱ, �ֵ�����
	cmd.ucIndex = pCmd->ucIndex;     // ������ҳ��������������ĸ�λ�õ���
	cmd.ucInsertPos = -1;            // Ĭ��Ϊ������
    cmd.bTing = m_bTing[cmd.chair];  // ���Ʊ�־, ֻ�е�һ���Ʋ���Ч

    // ������Ʊ�־, ����ڶ��γ��ƿͻ��˼���ִ�����ƶ���
    if (m_bTing[cmd.chair])
    {
        m_bTing[cmd.chair] = FALSE;

        // ����������Ϊ����������
        TILE_SET tileSet;
        tileSet.m_ucFlag = ACTION_TING;
        m_TilesGive[nChair].SetBlock(m_TilesGive[nChair].CurrentLength() - 1, tileSet);
    }

	if (pCmd->ucIndex < m_TilesHand[nChair].CurrentLength() && !m_bIsSet)  // û�г���������²����ò���λ��
	{
		// ���ҽ������ұ����Ÿ������ưڷ�λ��
		CMJHand mjHand = m_TilesHand[nChair];
		mjHand.Sort();
		for (int i = 0; i < mjHand.CurrentLength(); i++)
		{
			// ����ץ��������, ��������λ��
			if (mjHand.GetTile(i) == bLastDraw)
			{
				cmd.ucInsertPos = i;   // ������ҳ��ƺ�, ���ұ����Ÿ������ưڷ�λ��
				break;
			}
		}

        // ʹ�����λ��, ���ⱻ�˲³�������
        //cmd.ucInsertPos = rand() % m_TilesHand[nChair].CurrentLength();
	}

	_DOUT7("���λ��:%d, ����=%d, ����������=%d, ����λ��=%d, ����λ��=%d, �Ƿ����ƺ����=%d, �Ƿ�����=%d\n", 
		cmd.chair, cmd.t, cmd.ucHandType, cmd.ucIndex, cmd.ucInsertPos, m_bIsSet, cmd.bTing);

	// ���ó����ܼ�¼
	m_bIsSet = FALSE;	

 	pProcessor->SendGameData( (char*)&cmd, sizeof(cmd) );

	// ˢ�º���Ҫ�ڳ���֮ǰ���������������Ҷ໭һ�ų���
	if ( m_TilesHand[nChair].CurrentLength() != pCmd->ucTileLength )
	{
        _DOUT1("���%d, ��������ͻ������Ƴ��Ȳ�һ��, ǿ�����¸��¿ͻ����齫����\n", nChair);

		// ���Ȳ�ͬ��ǿˢ��
		UpdateHandTiles( nChair );
	}

	if ( m_shGangState == MJ_GANG_STATE_DRAW )
	{
		m_shGangState = MJ_GANG_STATE_GIVE;
	}
	else
	{
		m_shGangState = MJ_GANG_STATE_NONE;
	}

    // �ж��Ƿ񲹻� duanxiaohui 2012-02-23 ���Ӳ����ж�
    if (GetMJSetting().nTilesFlower == 1)
    {
        if (ucTileGive >= TILE_FLOWER_CHUN && ucTileGive <= TILE_FLOWER_JU)
        {
            // ��¼����
            m_nFlower[ucTileGive - TILE_FLOWER_CHUN] = nChair;

            // ��Ҫ������
            m_pStateEvent->SetMJState(MJ_STATE_DRAW_WAIT);
            return 0;
        }
    }

	CheckGiveBlock( nChair, ucTileGive );
	
	return 0;
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::ProcessRequestBlock(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen)
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

// 	// 	Ϊ���Զ���
// 	if ( pProcessor->GetGameState() != gsPlaying )
// 	{
// 		return 1;
// 	}

	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return 1;
	}

	TCMD_REQUEST_BLOCK* pCmd = (TCMD_REQUEST_BLOCK*)pcGameData;

    _DOUT5("�������Ʋ���: ��������=%d, ��������: %d, %d, %d, %d", pCmd->ucFlag, pCmd->ucTile[0], pCmd->ucTile[1], pCmd->ucTile[2], pCmd->ucTile[3]);

	// ������ƣ��������Ƹ�������
	if ( SwitchBlockTing( nChair, pCmd->ucFlag, /*pCmd->ucTile*/0 ) == 0 )
	{
		return 0;
	}

	// ����BLOCK����������
	SwitchBlock( nChair, pCmd->ucFlag, pCmd->ucTile );

	return 0;
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::SwitchBlockCollect( int nNextChair, BYTE ucRequestTile, TCMD_NOTIFY_BLOCK_RESULT &cmd )
{
	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return 0;
	}

	if ( NULL == GetSoGameProcess() )
	{
		return 0;
	}

    // ������������ˣ��Ͳ�ִ����
    if ( cmd.ucFlag != ACTION_EMPTY )
    {
        return 0;
    }
   
	if ( FALSE == IsCanCollect( m_TilesHand[nNextChair], GetLastTile(), m_pTileEvent->GetTurn(), nNextChair, nPlayerNbr ) )
	{
		return 0;
	}

    // �ܳ�, �����Ƿ�Ե����Ƿ����
    CMJHand mjHand;
    for (BYTE i = 0; i < 3; i++)
    {
        if (cmd.ucTile[i] != ucRequestTile)
        {
            mjHand.AddTile(cmd.ucTile[i]); // ��ȡ���Լ�����Ҫ�ԵĶ�����
        }
    }

    if (!m_TilesHand[nNextChair].IsSubSet(mjHand))
    {
        return 0;  // ���������������в�����
    }

 /*	
	// ������������ˣ��Ͳ�ִ����
	if ( cmd.ucFlag != ACTION_EMPTY )
	{
		return 0;
	}

	// ���Ƶ�ucWhich��ǵ��ǳ����е���һ�š�
	cmd.ucWhich = GetLastTile() - ucRequestTile;
	//m_TilesGive[m_pTileEvent->GetTurn()].DelTile( GetLastTile() );  // ��ɾ��, �ͻ��˲�����ʱ����
    m_TilesGive[m_pTileEvent->GetTurn()].SetBlockFlag(m_TilesGive[m_pTileEvent->GetTurn()].CurrentLength() - 1, TRUE);
    m_TilesGive[m_pTileEvent->GetTurn()].SetBlockChair(m_TilesGive[m_pTileEvent->GetTurn()].CurrentLength() - 1, nNextChair);
    

	switch ( cmd.ucWhich )
	{			
	case 0:
		// XOO
		{
			m_TilesHand[nNextChair].DelTile( ucRequestTile + 1 );

            // �˴����ڶ��ֳԷ� 
            if (m_TilesHand[nNextChair].IsHave(ucRequestTile + 2))
            {
                // ���ƴ���, ��Ե���ͷ
                m_TilesHand[nNextChair].DelTile(ucRequestTile + 2);
            }
            else
            {
                // ������, ��Ե����м�
                m_TilesHand[nNextChair].DelTile(ucRequestTile - 1);

                // �ı����λ��, ����ͻ��˻���ʾ�������ݻ����
                cmd.ucWhich = 1;

                // ��������Ҳ��Ҫ�ı�һ��, ����ͻ���ɾ�ƻ����
                ucRequestTile = ucRequestTile - 1;
            }
		}
		break;
	case 1:
		// OXO
		{
			m_TilesHand[nNextChair].DelTile( ucRequestTile );
			m_TilesHand[nNextChair].DelTile( ucRequestTile + 2 );
		}
		break;
	case 2:
		// OOX
		{
			m_TilesHand[nNextChair].DelTile( ucRequestTile );
			m_TilesHand[nNextChair].DelTile( ucRequestTile + 1 );
		}
		break;
	default:
		break;
	}
*/

    // ɾ��������ɾ��Ҫ�ԵĶ�����
    m_TilesHand[nNextChair].DelTiles(mjHand);

    CMJHand mjTemp;
    mjTemp.AddTile(cmd.ucTile[0]);
    mjTemp.AddTile(cmd.ucTile[1]);
    mjTemp.AddTile(cmd.ucTile[2]);
    mjTemp.Sort();

    cmd.ucTile[0] = mjTemp.GetTile(0);
    cmd.ucTile[1] = mjTemp.GetTile(1);
    cmd.ucTile[2] = mjTemp.GetTile(2);

    // ���һ��λ�ü�¼Ϊ�Ե�������
    cmd.ucTile[3] = ucRequestTile;
    cmd.ucFlag = ACTION_COLLECT;
    cmd.ucWhich = m_pTileEvent->GetTurn();
    GetSoGameProcess()->SendGameData( (char*)&cmd, sizeof(cmd) );

    TILE_SET tileSet;

    memcpy(tileSet.m_ucTile, cmd.ucTile, sizeof(tileSet.m_ucTile));
    tileSet.m_ucChair = m_pTileEvent->GetTurn();
    tileSet.m_ucFlag = ACTION_COLLECT;
    m_TilesSet[nNextChair].AddSet( tileSet );   // �������ݼӵ������б�

    // ���������λ����Ҫ�仯һ��
    tileSet.m_ucChair = nNextChair;

    if (GetMJSetting().nGiveBlockShow == 1)
    {
        // �������򱣴汻�����ܵ�������, ������͸����־
        m_TilesGive[m_pTileEvent->GetTurn()].SetBlock(m_TilesGive[m_pTileEvent->GetTurn()].CurrentLength() - 1, tileSet);
    }
    else
    {
        // ����������Ҫ���汻�����ܵ�������, ���Գ���������������1, �ѱ���������ȥ��, CMJGive����ʹ��DelTile, ����������ǰ�濪ʼɾ����ֵ.
        m_TilesGive[m_pTileEvent->GetTurn()].CurrentLength(m_TilesGive[m_pTileEvent->GetTurn()].CurrentLength() - 1);
    }
    
    _DOUT4("��������: %d, %d, %d, %d", cmd.ucTile[0], cmd.ucTile[1], cmd.ucTile[2], cmd.ucTile[3]);
    
	m_pTileEvent->SetTurn( nNextChair );

	return 1;
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::SwitchBlockTriplet( int nNextChair, BYTE ucRequestTile, TCMD_NOTIFY_BLOCK_RESULT &cmd )
{
	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return 0;
	}

	if ( NULL == GetSoGameProcess() )
	{
		return 0;
	}

	// �ж������Ƿ�Ϸ�
	if ( FALSE == m_TilesHand[nNextChair].IsHave( ucRequestTile, 2 ) )
	{
		return 0;
	}

	m_TilesHand[nNextChair].DelTile( ucRequestTile );
	m_TilesHand[nNextChair].DelTile( ucRequestTile );
	//m_TilesGive[m_pTileEvent->GetTurn()].DelTile( ucRequestTile );  // ��ɾ��, �ͻ��˲�����ʱ����

	cmd.ucFlag = ACTION_TRIPLET;
	GetSoGameProcess()->SendGameData( (char*)&cmd, sizeof(cmd) );

    TILE_SET tileSet;
    
    memcpy(tileSet.m_ucTile, cmd.ucTile, sizeof(tileSet.m_ucTile));
    tileSet.m_ucFlag = ACTION_TRIPLET;
    tileSet.m_ucChair = m_pTileEvent->GetTurn();
    tileSet.m_ucTile[3] = tileSet.m_ucTile[0];
    m_TilesSet[nNextChair].AddSet(tileSet);

    // ���������λ����Ҫ�仯һ��
    tileSet.m_ucChair = nNextChair;

    if (GetMJSetting().nGiveBlockShow == 1)
    {
        // �������򱣴汻�����ܵ�������, ������͸����־
        m_TilesGive[m_pTileEvent->GetTurn()].SetBlock(m_TilesGive[m_pTileEvent->GetTurn()].CurrentLength() - 1, tileSet);
    }
    else
    {
        // ����������Ҫ���汻�����ܵ�������, ���Գ���������������1, �ѱ���������ȥ��, CMJGive����ʹ��DelTile, ����������ǰ�濪ʼɾ����ֵ.
        m_TilesGive[m_pTileEvent->GetTurn()].CurrentLength(m_TilesGive[m_pTileEvent->GetTurn()].CurrentLength() - 1);
    }

    _DOUT5("���Ʋ���: ������=%d, ������=%d, ��������: %d,%d,%d", nNextChair, m_pTileEvent->GetTurn(), ucRequestTile, cmd.ucTile[1], cmd.ucTile[2]);

    // �������Ƶ���Ϊ��ǰ�������
    m_pTileEvent->SetTurn( nNextChair );

	return 1;
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::SwitchBlockQuadrupletConcealed( int nNextChair, BYTE ucRequestTile, TCMD_NOTIFY_BLOCK_RESULT &cmd )
{
	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return 0;
	}

	if ( NULL == GetSoGameProcess() )
	{
		return 0;
	}

	if (nNextChair != m_pTileEvent->GetTurn())
	{
		return 0; 
	}

    if (ACTION_QUADRUPLET_CONCEALED == m_nBlockRequest[nNextChair])
    {
        // �����ĸ��ƴ���
        // �ж������Ƿ�Ϸ�
        if ( FALSE == m_TilesHand[nNextChair].IsHave( ucRequestTile, 4 ) )
        {
            return 0;
        }

        m_TilesHand[nNextChair].DelTile( ucRequestTile );
        m_TilesHand[nNextChair].DelTile( ucRequestTile );
        m_TilesHand[nNextChair].DelTile( ucRequestTile );
        m_TilesHand[nNextChair].DelTile( ucRequestTile );

        cmd.ucFlag = ACTION_QUADRUPLET_CONCEALED;
    }
    else if (ACTION_QUADRUPLET_DNXB == m_nBlockRequest[nNextChair])
    {
        if (GetMJSetting().nDongNanXiBeiGang == 0)
        {
            return 0;
        }

        // �����: ����������
        if (m_TilesHand[nNextChair].IsHave(TILE_EAST) 
         && m_TilesHand[nNextChair].IsHave(TILE_SOUTH) 
         && m_TilesHand[nNextChair].IsHave(TILE_WEST) 
         && m_TilesHand[nNextChair].IsHave(TILE_NORTH))
        {
            // ���������ܳ���
            // ɾ���ܵ�������
            m_TilesHand[nNextChair].DelTile(TILE_EAST);
            m_TilesHand[nNextChair].DelTile(TILE_SOUTH);
            m_TilesHand[nNextChair].DelTile(TILE_WEST);
            m_TilesHand[nNextChair].DelTile(TILE_NORTH);

            cmd.ucFlag = ACTION_QUADRUPLET_DNXB;
        }
        else
        {
            _DOUT("����: ����������ʧ��\n");
            return 0;
        }
    }
    else if (ACTION_QUADRUPLET_ZFB == m_nBlockRequest[nNextChair])
    {
        if (GetMJSetting().nZhongFaBaiGang == 0)
        {
            return 0;
        }

        // �����: �з���X��
        if (m_TilesHand[nNextChair].IsHave(TILE_ZHONG)
         && m_TilesHand[nNextChair].IsHave(TILE_FA)
         && m_TilesHand[nNextChair].IsHave(TILE_BAI)
         && m_TilesHand[nNextChair].IsHave(ucRequestTile))  // ucRequestTile: �������ɿͻ��˷��͹���
        {

            // �з��׸ܳ���
            // ɾ���ܵ�������
            m_TilesHand[nNextChair].DelTile(TILE_ZHONG);
            m_TilesHand[nNextChair].DelTile(TILE_FA);
            m_TilesHand[nNextChair].DelTile(TILE_BAI);
            m_TilesHand[nNextChair].DelTile(ucRequestTile);

            cmd.ucFlag = ACTION_QUADRUPLET_ZFB;
        }
        else
        {
            _DOUT("����: �з��׸�ʧ��\n");
            return 0;
        }
    }

    TILE_SET tileSet;
    memcpy(tileSet.m_ucTile, cmd.ucTile, sizeof(tileSet.m_ucTile));
    tileSet.m_ucFlag = cmd.ucFlag;
    tileSet.m_ucChair = m_pTileEvent->GetTurn();
    m_TilesSet[nNextChair].AddSet(tileSet);

    // ֪ͨ�ͻ��˰��ܲ���
    for (char i = 0; i < PLAYER_NBR; i++)
    {
		if (ACTION_QUADRUPLET_DNXB == m_nBlockRequest[nNextChair])
		{
			// ������������ʾ��������ҿ�
			cmd.ucTile[0] = TILE_EAST;
			cmd.ucTile[1] = TILE_SOUTH;
			cmd.ucTile[2] = TILE_WEST;
			cmd.ucTile[3] = TILE_NORTH;
		}
		else if (ACTION_QUADRUPLET_ZFB == m_nBlockRequest[nNextChair])
		{
			// �з���X����ʾ��������ҿ�
			cmd.ucTile[0] = TILE_ZHONG;
			cmd.ucTile[1] = TILE_FA;
			cmd.ucTile[2] = TILE_BAI;
			cmd.ucTile[3] = ucRequestTile;
		}
		else
		{
			// ��������:ACTION_QUADRUPLET_CONCEALED������Ҫ����ʵ�ʸ������ݸ�������
			if (GetMJSetting().nAnGangShow == 1 || (i == nNextChair || cmd.ucFlag != ACTION_QUADRUPLET_CONCEALED))
			{
				cmd.ucTile[0] = ucRequestTile;
				cmd.ucTile[1] = ucRequestTile;
				cmd.ucTile[2] = ucRequestTile;
				cmd.ucTile[3] = ucRequestTile;
			}
			else
			{
				memset(cmd.ucTile, TILE_BEGIN, sizeof(cmd.ucTile));
			}
		}

        GetSoGameProcess()->SendGameDataToSeat(i, (char*)&cmd, sizeof(cmd));
    }


	m_pTileEvent->SetTurn( nNextChair );
	m_shGangState = MJ_GANG_STATE_DRAW;	
	m_nAllGangNbr++;

	return 1;
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::SwitchBlockQuadrupletRevealed( int nNextChair,  BYTE ucRequestTile,   TCMD_NOTIFY_BLOCK_RESULT &cmd )
{
	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return 0;
	}

	if ( NULL == GetSoGameProcess() )
	{
		return 0;
	}

	// ���ֳܷ��Լ��������ɸܺ͸ܱ��˵���
	if ( nNextChair != m_pTileEvent->GetTurn() )
	{
		// �ܱ��˵�
		if ( FALSE == m_TilesHand[nNextChair].IsHave( ucRequestTile, 3 ) )
		{
			return 0;
		}

		m_TilesHand[nNextChair].DelTile( ucRequestTile );
		m_TilesHand[nNextChair].DelTile( ucRequestTile );
		m_TilesHand[nNextChair].DelTile( ucRequestTile );
		// m_TilesGive[m_pTileEvent->GetTurn()].DelTile( ucRequestTile );  // ��ɾ��, �ͻ��˲�����ʱ����

		m_nAllGangNbr++;

        TILE_SET tileSet;
        memcpy(tileSet.m_ucTile, cmd.ucTile, sizeof(tileSet.m_ucTile));
        tileSet.m_ucFlag = ACTION_QUADRUPLET_REVEALED;
        tileSet.m_ucChair = m_pTileEvent->GetTurn();
        m_TilesSet[nNextChair].AddSet(tileSet);

        // ���������λ����Ҫ�仯һ��
        tileSet.m_ucChair = nNextChair;

        if (GetMJSetting().nGiveBlockShow == 1)
        {
            // �������򱣴汻�����ܵ�������, ������͸����־
            m_TilesGive[m_pTileEvent->GetTurn()].SetBlock(m_TilesGive[m_pTileEvent->GetTurn()].CurrentLength() - 1, tileSet);
        }
        else
        {
            // ����������Ҫ���汻�����ܵ�������, ���Գ���������������1, �ѱ���������ȥ��, CMJGive����ʹ��DelTile, ����������ǰ�濪ʼɾ����ֵ.
            m_TilesGive[m_pTileEvent->GetTurn()].CurrentLength(m_TilesGive[m_pTileEvent->GetTurn()].CurrentLength() - 1);
        }

        m_shGangState = MJ_GANG_STATE_DRAW;	

		cmd.ucFlag = ACTION_QUADRUPLET_REVEALED;
	}
	else
	{
		if ( FALSE == m_TilesHand[nNextChair].IsHave( ucRequestTile ) )
		{

			return 0;
		}

		// ���Լ���
		BOOL bRight = FALSE;
		for ( int k = 0; k < m_TilesSet[nNextChair].CurrentLength(); ++k )
		{
			TILE_SET tagSet;
			m_TilesSet[nNextChair].GetSet( k, tagSet );
			if ( ACTION_TRIPLET == tagSet.m_ucFlag && tagSet.m_ucTile[0] == ucRequestTile )
			{
				bRight = TRUE;
				m_TilesHand[nNextChair].DelTile( ucRequestTile );
				m_TilesSet[nNextChair].TripletToQuadruplet( ucRequestTile );

				// ��Ҫ�޸�Ŀ�꣬��Ϊ�Լ�
				// tagSet.m_ucChair = nNextChair;  // ����Ҫ�ı�Ŀ��, �����������Ƶ�Ŀ����
				tagSet.m_ucFlag = ACTION_QUADRUPLET_PATCH; // ���ܸĳɲ���

                m_TilesSet[nNextChair].SetSet( k, tagSet );

                // ���������λ����Ҫ�仯һ��
                for (int n = 0; n < m_TilesGive[tagSet.m_ucChair].CurrentLength(); n++)
                {
                    if (m_TilesGive[tagSet.m_ucChair].GetTile(n) == ucRequestTile)
                    {
                        m_TilesGive[tagSet.m_ucChair].GetBlock(n, tagSet);
                        tagSet.m_ucFlag = ACTION_QUADRUPLET_PATCH;
                        m_TilesGive[tagSet.m_ucChair].SetBlock(n, tagSet);
                    }
                }

                m_nAllGangNbr++;

                m_shGangState = MJ_GANG_STATE_DRAW;	

                cmd.ucFlag = ACTION_QUADRUPLET_PATCH;

				break;
			}
		}

		if ( FALSE == bRight )
		{
			return 0;
		}

		// ���ܵ��ж�
		if ( GetMJSetting().nQiangGang )
		{
            // ����������ܣ�����Ҫ�����һ���ƣ����óɸܵ�����
            SetLastTile( ucRequestTile );

			BOOL bQianggang = FALSE;
            short shOldGangState = m_shGangState;
            m_shGangState = MJ_GANG_STATE_BLOCK;
			for ( int k = 0; k < nPlayerNbr; ++k )
			{
				m_nBlockLevel[k] = 0;
				if ( k != nNextChair )
				{
                    short shBlockLevel = 0;
                    CheckBlockWinOther(k, shBlockLevel, ucRequestTile);
                    if (shBlockLevel >= GetMJSetting().nBlockLevelWin)
                    {
                        m_nBlockLevel[k] = shBlockLevel;

                        // ��֪ͨ���ƽ��, ����֪ͨ������Ϣ
                        cmd.ucFlag = ACTION_QUADRUPLET_PATCH;
                        GetSoGameProcess()->SendGameData( (char*)&cmd, sizeof(cmd) );

                        bQianggang = TRUE;
                        m_shGangState = MJ_GANG_STATE_BLOCK;

                        // ����������Ϣ
                        TCMD_NOTIFY_BLOCK cmd;
                        cmd.ucChair = k;
                        cmd.ucFlag = 1 << 4;
                        GetSoGameProcess()->SendGameDataToSeat(k, (char*)&cmd, sizeof(TCMD_NOTIFY_BLOCK));
                    }

					//if ( GetMJSetting().nBlockLevelWin == CheckBlock( k, FALSE, ucRequestTile, GetMJSetting().nBlockLevelWin ) )
					//{
					//	bQianggang = TRUE;
					//	m_shGangState = MJ_GANG_STATE_BLOCK;
					//}
				}
			}

			if( bQianggang == TRUE )
			{
                _DOUT("��������״̬ ");
                
                // ��������״̬��
				m_pStateEvent->SetMJState( MJ_STATE_BLOCK );
                m_pTileEvent->SetTurn( nNextChair );

				// ���ߵĴ�����
				ISoUserManager* pUserMgr = GetSoUserManager();
				if ( pUserMgr != NULL )
				{
					for ( int j = 0; j < nPlayerNbr; ++j )
					{
						if ( m_nBlockLevel[j] > 0 /*&& IsOffline( j ) */&& j != nNextChair )
						{
							ISoPlayer* pUser = pUserMgr->GetPlayerByChair( j );
							if ( pUser != NULL )
							{
                                if (pUser->IsOffline() || pUser->IsForceExit())
                                {
                                    SoTrustOffline( j, pUser->GetUserID() );
                                }
							}
						}
					}
				}

				return 1;
			}
            else
            {
                m_shGangState = shOldGangState;
            }
		}
	}

	GetSoGameProcess()->SendGameData( (char*)&cmd, sizeof(cmd) );
	m_pTileEvent->SetTurn( nNextChair );

	return 0;
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::SwitchBlockWin( int nNextChair, BYTE ucRequestTile, TCMD_NOTIFY_BLOCK_RESULT &cmd )
{
	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return 1;
	}

	if ( NULL == GetSoGameProcess() )
	{
		return 1;
	}
	
	CMJHand mjCheck;
	CMJHand mjTilesGod;
	int nLaiziCount = GetTilesNoGod( m_TilesHand[nNextChair], mjCheck, mjTilesGod );

	if ( nNextChair != m_pTileEvent->GetTurn() )
	{
		if ( m_TilesGod.IsHave( ucRequestTile ) )
		{
			mjTilesGod.AddTile( ucRequestTile );
		}
		else
		{
			mjCheck.AddTile( ucRequestTile );
		}
		
		if ( m_TilesGod.IsHave( ucRequestTile ) && 0 == GetMJSetting().nCanWinLaizi )
		{
			return 1;
		}
	}

	if ( nLaiziCount > 0 )
	{
		// ������Ƶ�ʱ�����һ��������
		CMJHand mjLaizi;
		ConfigGodTiles( mjCheck, m_TilesSet[nNextChair], nLaiziCount, mjLaizi, mjTilesGod );
		if ( mjLaizi.CurrentLength() == 0 )
		{
			mjLaizi.AddTiles(mjTilesGod);
		}

		m_TilesPlayerGod[nNextChair].ReleaseAll();
		m_TilesPlayerGod[nNextChair].AddTiles( mjLaizi );

 		mjCheck.AddTiles( m_TilesPlayerGod[nNextChair] );
	}

	if ( FALSE == IsWin( mjCheck, m_TilesSet[nNextChair], 0, FALSE ) )
	{
		return 1;
	}

	// ���ǰ���Ѿ����˺��ˣ��ڷǶ��˺�����£�ֻ��Ϊֻ����ǰ��һ���˺�
	if ( cmd.ucFlag == ACTION_WIN && GetMJSetting().nMoreWin == 0 )
	{
		return 1;
	}

	m_bWiner[nNextChair] = TRUE;
	cmd.ucFlag = ACTION_WIN;
	if ( nNextChair != m_pTileEvent->GetTurn() )
	{
		m_ucEndType = MJ_END_GUN;

		int nTurn = m_pTileEvent->GetTurn();
		TILE_SET tileSet;

		if ( m_shGangState == MJ_GANG_STATE_BLOCK )
		{
			m_ucEndType = MJ_END_QIANG;

            // ��������ܵĻ�, ����Ҫ��֮ǰ����ȡ��, ���ܻ�ԭ����
            for (int i = 0; i < m_TilesSet[nTurn].CurrentLength(); i++)
            {
                m_TilesSet[nTurn].GetSet(i, tileSet);
                if (tileSet.m_ucTile[0] == ucRequestTile && tileSet.m_ucFlag == ACTION_QUADRUPLET_PATCH)
                {
                    tileSet.m_ucFlag = ACTION_TRIPLET;
                    m_TilesSet[nTurn].SetSet(i, tileSet);
                }
            }
		}
		else
		{
			// �ѷ�����������������Ʊ�־����һ��
			int nLen = m_TilesGive[nTurn].CurrentLength() - 1;
			if (m_TilesGive[nTurn].GetTile(nLen) == ucRequestTile)
			{
				m_TilesGive[nTurn].GetBlock(nLen, tileSet);
				tileSet.m_ucFlag = ACTION_WIN;
				m_TilesGive[nTurn].SetBlock(nLen, tileSet);
			}

			//m_TilesGive[m_pTileEvent->GetTurn()].DelTile( ucRequestTile );
		}

		m_TilesHand[nNextChair].AddTile( ucRequestTile );
	}
	else
	{
		m_ucEndType = MJ_END_SELF;
	}

	GetSoGameProcess()->SendGameData( (char*)&cmd, sizeof(cmd) );

	return 0;
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::SwitchBlockOver( int nChair, TCMD_NOTIFY_BLOCK_RESULT &cmd )
{
	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return 0;
	}

	if ( NULL == GetSoGameProcess() )
	{
		return 0;
	}

	BOOL bSelfBlock = FALSE;
	if ( m_TilesHand[m_pTileEvent->GetTurn()].CurrentLength() % 3 == 2 )
	{
		bSelfBlock = TRUE;
	}

	// ִ�еĺ��Ʋ������������״̬
	if ( ACTION_WIN == cmd.ucFlag )
	{
		m_pStateEvent->SetMJState( MJ_STATE_END );
		return MJ_STATE_END;
	}
	// ִ�еĸܲ��������뷢��״̬
	else if (cmd.ucFlag >= ACTION_QUADRUPLET_PATCH && cmd.ucFlag <= ACTION_QUADRUPLET_ZFB)
	{
		m_shGangState = MJ_GANG_STATE_DRAW;
		m_pStateEvent->SetMJState(MJ_STATE_DRAW);
		ClearLoseHu(m_pTileEvent->GetTurn());

		return MJ_STATE_DRAW;
	}
	else if ( ACTION_EMPTY == cmd.ucFlag && bSelfBlock == FALSE )
	{
		// û������
		// ������������ƶ�
		int nTurn = m_pTileEvent->GetTurn();

		// ��������ܵ�ȡ�����ͱ��˷���
		if ( GetMJSetting().nQiangGang && m_shGangState == MJ_GANG_STATE_BLOCK )
		{
			nTurn = m_pTileEvent->GetTurn();
			m_shGangState = MJ_GANG_STATE_DRAW;
			ClearLoseHu( m_pTileEvent->GetTurn() );
		}
		else
		{
			m_shGangState = MJ_GANG_STATE_NONE;
			nTurn = ( m_pTileEvent->GetTurn() + GetMJSetting().nGiveOrder ) % nPlayerNbr;
		}
		m_pTileEvent->SetTurn( nTurn );

		m_pStateEvent->SetMJState( MJ_STATE_DRAW_WAIT/*MJ_STATE_DRAW*/ );
		return MJ_STATE_DRAW;
	}
	// ����������ƺ���������˵ĳ���״̬
	else 
	{
		m_pStateEvent->SetMJState( MJ_STATE_GIVE );
		ClearLoseHu( m_pTileEvent->GetTurn() );
		return MJ_STATE_GIVE;
	}

	return 0;
}

template<int nPlayerNbr>
void CSoTilePlayerMgr<nPlayerNbr>::SwitchBlock( int nChair, BYTE ucFlag, BYTE ucTile[4], BOOL bTimeOut )
{
	if ( nChair < 0 || nChair >= nPlayerNbr )
		return;

	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return;
	}

	if ( NULL == GetSoGameProcess() )
	{
		return;
	}

	// ����״̬�У�����Ӧ
	if ( MJ_STATE_BLOCK != m_pStateEvent->GetMJState() )
	{
        _DOUT1("��ǰ״̬��������״̬: %d", MJ_STATE_BLOCK);
		return;
	}

	// ����ȼ�Ϊ0��û�в���Ȩ��
	if ( 0 == m_nBlockLevel[nChair] && FALSE == bTimeOut )
	{
        _DOUT("����ȼ�Ϊ0��û�в���Ȩ��");
		return;
	}

	int i = 0;
	if ( FALSE == bTimeOut )
	{
		int nRequestLevel = GetRequestLevel( nChair, ucFlag, /*ucTile*/0 );

		// ��¼����ĵȼ�
		if ( nRequestLevel <= m_nBlockLevel[nChair] )
		{
			m_nBlockLevel[nChair] = nRequestLevel;
		}

		m_nBlockRequest[nChair] = ucFlag;
		memcpy(&m_ucRequestTile[nChair], ucTile, sizeof(m_ucRequestTile[nChair]));
	}
	else
	{
		// ȫ����Ϊȡ������
		for ( i = 0; i < nPlayerNbr; ++i )
		{
			m_nBlockLevel[i] = 0;
		}
	}

	int nMaxLevel = 0;

	// ����Ҫ������
	if ( IsWaitOtherRequest( nChair, ucFlag, /*ucTile*/0, nMaxLevel ) )
	{
		return;
	}

	TCMD_NOTIFY_BLOCK_RESULT cmd;
    memcpy(cmd.ucTile, ucTile, sizeof(cmd.ucTile));
	cmd.ucWhich = m_pTileEvent->GetTurn();
	cmd.ucFlag = ACTION_EMPTY;

	if( SwitchBlockProcess( nMaxLevel, cmd ) != 0 )
		return;

	// ȫ����ȡ���ˣ������¸�״̬
	SwitchBlockOver( nChair, cmd );
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::ProcessRequestFlower(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen)
{
	return 0;
}

template<int nPlayerNbr>
void CSoTilePlayerMgr<nPlayerNbr>::SoTrustOfflineGive(BYTE ucChair, short shPlayerID)
{
	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return;
	}

	if ( m_pTileEvent->GetTurn() == ucChair )
	{
		int nFlowerNbr = CheckFlower( ucChair );
		if ( nFlowerNbr > 0 )
		{
			// ���󲹻�
			TCMD_REQUEST_FLOWER cmd;
			cmd.ucChair = ucChair;
			cmd.ucNumber = nFlowerNbr;
			ProcessRequestFlower( 0, shPlayerID, ucChair, &cmd, sizeof(cmd) );
		}
		else
		{
			OnTimeOutGive();
		}
	}
}

template<int nPlayerNbr>
void CSoTilePlayerMgr<nPlayerNbr>::SoTrustOfflineBlock(BYTE ucChair, short shPlayerID)
{
	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return;
	}

	// ������ܺ�������ȡ��
	BOOL bBlock = FALSE;
	if ( m_pTileEvent->GetTurn() == ucChair )
	{
		bBlock = TRUE;
	}
	else if ( CheckBlock( ucChair, FALSE, GetLastTile() ) )
	{
		bBlock = TRUE;
	}

	// ί������
	if ( bBlock )
	{
		TCMD_REQUEST_BLOCK cmdRequestBlock;
		if ( m_nBlockLevel[ucChair] == GetMJSetting().nBlockLevelWin )
		{
			cmdRequestBlock.ucFlag = ACTION_WIN;
			cmdRequestBlock.ucTile[0] = GetLastTile();
		}
		else
		{
			cmdRequestBlock.ucFlag = ACTION_EMPTY;
			cmdRequestBlock.ucTile[0] = TILE_BEGIN;
		}

		ProcessRequestBlock( 0, shPlayerID, ucChair, &cmdRequestBlock, sizeof(cmdRequestBlock) );
	}
}
 
template<int nPlayerNbr>
void CSoTilePlayerMgr<nPlayerNbr>::SoTrustOfflineFlower(BYTE ucChair, short shPlayerID)
{
	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return;
	}

	// ����״̬��ֱ������
	TCMD_REQUEST_FLOWER cmd;
	cmd.ucChair = ucChair;
	cmd.ucNumber = CheckFlower( ucChair );
	ProcessRequestFlower( 0, shPlayerID, ucChair, &cmd, sizeof(cmd) );
}

template<int nPlayerNbr>
void CSoTilePlayerMgr<nPlayerNbr>::SoTrustOffline(BYTE ucChair, short shPlayerID)
{
	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return;
	}

	if ( m_pStateEvent->GetMJState() == MJ_STATE_GIVE )
	{
		SoTrustOfflineGive( ucChair, shPlayerID );
	}
	else if ( m_pStateEvent->GetMJState() == MJ_STATE_BLOCK )
	{
		SoTrustOfflineBlock( ucChair, shPlayerID );
	}
	else if ( m_pStateEvent->GetMJState() == MJ_STATE_FLOWER )
	{
		SoTrustOfflineFlower( ucChair, shPlayerID );
	}
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::OnGameStart()
{
		
	for ( int i = 0; i < nPlayerNbr; ++i )
	{
		ClearLoseHu( i );
		m_TilesGive[i].ReleaseAll();
        m_bTing[i] = FALSE;
        SetTing(i, FALSE);
	}
	
	m_nAllGangNbr = 0;

	return 0;
}
template<int nPlayerNbr>
void CSoTilePlayerMgr<nPlayerNbr>::UpdateHandTiles( int nChair )
{
	if ( nChair < 0 || nChair >= nPlayerNbr )
		return;

	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
	{
		return;
	}

	ISoGameProcessor* pProcess = GetSoGameProcess();
	if ( NULL == pProcess )
	{
		return;
	}
	
	ISoUserManager* pUserMgr = GetSoUserManager();
	if ( NULL == pUserMgr )
	{
		return;
	}

	ISoPlayer* pUser = pUserMgr->GetPlayerByChair( nChair );
	if ( NULL == pUser )
	{
		return;
	}

	TCMD_NOTIFY_UPDATEHANDTILES cmd;
	cmd.ucChair = nChair;
	int i, j;
	for ( i = 0; i < m_TilesHand[nChair].CurrentLength(); ++i )
	{
		cmd.ucHandTiles[i] = m_TilesHand[nChair].GetTile( i );
	}
	cmd.ucHandCount = m_TilesHand[nChair].CurrentLength();

	for ( i = 0; i < nPlayerNbr && i < 4; ++i )
	{
		cmd.ucGiveCount[i] = m_TilesGive[i].CurrentLength();
		for ( j = 0; j < cmd.ucGiveCount[i]; ++j )
		{
			cmd.ucGiveTiles[i][j] = m_TilesGive[i].GetTile( j );
		}
	}
	
	pProcess->SendGameDataTo( pUser->GetUserID(), (char*)&cmd, sizeof(cmd) );
}
/************************************************************************/
/* 
��ȡ��ҵ�Ǯ
*/
/************************************************************************/
template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::GetMJUserMoney( int nChair )
{
	ISoUserManager* pUserMgr = GetSoUserManager();
	if ( NULL == pUserMgr )
	{
		return 0;
	}

	ISoPlayer* pUser = pUserMgr->GetPlayerByChair( nChair );
	if ( NULL == pUser )
		return 0;

	// ����ҵ�ǰ��Ǯȡ��
	int nUserMoney = 0;
	if ( GetMJSetting().nMoneyType == MJ_MONEY_TYPE_HAPPYMONEY )
	{
		//int64_t n64HappyMoney = 0;
        //���ﲻ֧�ִ�ѡ��
		//pUserMgr->GetHappyEnergy( pUser->GetUserID(), n64HappyMoney, FALSE );
		//nUserMoney = (int)n64HappyMoney;
	}
	else if ( GetMJSetting().nMoneyType == MJ_MONEY_TYPE_GAMEMONEY )
	{
		pUserMgr->GetMoney( pUser->GetUserID(), &nUserMoney, FALSE );
	}
	else
	{
		// Ĭ�Ͼ�����Ϸ��
		pUserMgr->GetMoney( pUser->GetUserID(), &nUserMoney, FALSE );
	}
	return nUserMoney;

	
}

/**
*  @brief ��ȡ������Ϣ
*  @param[in]  nChair ����ȡ�����
*  @return TCMD_NOTIFY_BLOCK �ṹ
*/	
template<int nPlayerNbr>
TCMD_NOTIFY_BLOCK CSoTilePlayerMgr<nPlayerNbr>::GetBlockInfo( int nChair )
{
    BYTE i, j;
    TILE_SET tileSet;
    BYTE byChiCount = 0;
    BYTE byPengCount = 0;
    BYTE byGangCount = 0;

    TCMD_NOTIFY_BLOCK cmd;
    cmd.ucChair = nChair;

    for (i = 0; i < MJ_BLOCK_NBR; i++)
    {
        if (m_bBlockEnable[i])
        {
            cmd.ucFlag += (1 << i);  // 1, 2, 4, 8, 16, 32, ...  ����������... �Ķ�����λ
        }
    }

    for (i = 0; i < m_tmpBlockSet.CurrentLength(); i++)
    {
        m_tmpBlockSet.GetSet(i, tileSet);

        switch (tileSet.m_ucFlag)
        {
        case ACTION_COLLECT:   // ��
            {
                for (j = 0; j < 3; j++)
                {
                    cmd.ucTile[0][byChiCount++] = tileSet.m_ucTile[j];
                }
            }
            break;
        case ACTION_TRIPLET:   // ��
            {
                for (j = 0; j < 3; j++)
                {
                    cmd.ucTile[1][byPengCount++] = tileSet.m_ucTile[j];
                }
            }
            break;
        case ACTION_QUADRUPLET_REVEALED:   // ����
        case ACTION_QUADRUPLET_CONCEALED:  // ����
        case ACTION_QUADRUPLET_PATCH:      // ����
        case ACTION_QUADRUPLET_DNXB:       // ����������
        case ACTION_QUADRUPLET_ZFB:        // �з���X��
            {
				// 24/4=6, �����6����
                if (byGangCount >= 24)
                {
                    break;
                }

                for (j = 0; j < 4; j++)
                {
                    cmd.ucTile[2][byGangCount++] = tileSet.m_ucTile[j];
                }
            }
            break;
        default:
            break;
        }
    }

    if (m_bBlockEnable[MJ_BLOCK_TING])
    {
        // ������ƺ��������ܺ���Щ��
        for (BYTE i = 0; i < m_tmpTingGive.CurrentLength() && i < 24; i++)
        {
            // ������Դ������
            cmd.ucTile[3][i] = m_tmpTingGive.GetTile(i);

            _DOUT2("���ƺ���Դ���ĵ� %d ����: %d", i + 1, m_tmpTingGive.GetTile(i));

            CMJHand tmpHand;
            CMJHand mjTestHand = m_TilesHand[nChair];
            mjTestHand.DelTile(cmd.ucTile[3][i]);
            GetTingTiles( mjTestHand, m_TilesSet[nChair], tmpHand );

            // ������ƺ���Ժ���Щ��
            for (BYTE j = 0; j < tmpHand.CurrentLength() && j < 15; j++)
            {
                _DOUT3("�����: %d ��, ���Ժ���%d����: %d", cmd.ucTile[3][i], j + 1, tmpHand.GetTile(j));
                cmd.ucTingTile[i][j] = tmpHand.GetTile(j);
            }
        }
    }


    _DOUT2("���λ��:%d, ���Ʊ�ʶ(0x%x)", nChair, cmd.ucFlag);

    return cmd;
}
/************************************************************************/
/* 
TIMER������
*/
/************************************************************************/
template<int nPlayerNbr>
void CSoTilePlayerMgr<nPlayerNbr>::OnTimeOutBlock( BOOL bAutoWin )
{
    _DOUT("���Ƴ�ʱ, ������ȡ����������\n");
    BYTE ucTile[4] = {0};
	SwitchBlock( 0, 0, ucTile, TRUE );
}

template<int nPlayerNbr>
BOOL CSoTilePlayerMgr<nPlayerNbr>::IsOffline(BYTE ucChair)
{
	BOOL bOffline = FALSE;
	ISoUserManager *pUserMgr = NULL;

	pUserMgr = GetSoUserManager();

	if(pUserMgr != NULL)
	{
		ISoPlayer* pUser = pUserMgr->GetPlayerByChair( ucChair );
		if ( pUser != NULL )
		{
			bOffline = pUser->IsOffline();
		}
	}

	return bOffline;
}


template<int nPlayerNbr>
void CSoTilePlayerMgr<nPlayerNbr>::OnTimeOutGive()
{
	if ( NULL == m_pTileEvent )
	{
		return;
	}

	ISoUserManager* pUserMgr = GetSoUserManager();
	if ( NULL == pUserMgr )
	{
		return;
	}
	ISoPlayer* pUser = pUserMgr->GetPlayerByChair( m_pTileEvent->GetTurn() );
	if ( NULL == pUser )
	{
		return;
	}

	int nTurn = m_pTileEvent->GetTurn();

	_DOUT1("������, ���λ��=%d, ��ʱ����: OnTimeOutGive\n", nTurn);

	// ϵͳ��ʱ��ȷ�����ѻ��ƴ��ȥ�������ĵ�һ����һ�Ų��ǻ��Ƶ�������
	TILE ucLastTile = GetLastTile();

	// ����в��������󲹻��������������
	int nFlowerNbr = CheckFlower( nTurn );
	if ( nFlowerNbr > 0 )
	{		
        // ���󲹻�
        // duanxiaohui 2012-02-23 ��3D�ͻ��˶�Ӧ, ÿ�β�һ��
		/*TCMD_REQUEST_FLOWER cmd;
		cmd.ucChair = nTurn;
		cmd.ucNumber = nFlowerNbr;
		ProcessRequestFlower( 0, pUser->GetUserID(), m_pTileEvent->GetTurn(), &cmd, sizeof(cmd) );*/

        // ѡһ�����Ļ��Ƴ�ȥ
        for (BYTE i = 7; i >= 0; i--)
        {
            if (m_TilesHand[nTurn].IsHave(TILE_FLOWER_CHUN + i))
            {
                _DOUT2("���:%d, ����: %d", nTurn, TILE_FLOWER_CHUN + i);

                // ģ����Ϣ�������
                TCMD_REQUEST_GIVE cmd;
                cmd.t = TILE_FLOWER_CHUN + i;
                cmd.ucTileLength = m_TilesHand[nTurn].CurrentLength() - 1;
                cmd.ucIndex = cmd.ucTileLength;  // ���һ����λ��
                ProcessRequestGive( 0, pUser->GetUserID(), m_pTileEvent->GetTurn(), &cmd, sizeof(cmd) );
                break;
            }
        }
	}
	else
	{
		// ģ����Ϣ�������
		TCMD_REQUEST_GIVE cmd;
        cmd.ucTileLength = m_TilesHand[nTurn].CurrentLength() - 1;
		cmd.ucIndex = cmd.ucTileLength;  // ���һ����λ��

        // �Զ����Ʋ�������
        for (BYTE i = cmd.ucTileLength; i >= 0; i--)
        {
            cmd.t = m_TilesHand[nTurn].GetTile(i);
            if (!m_TilesGod.IsHave(cmd.t))
            {
                break;
            }
        }

		int nres = ProcessRequestGive( 0, pUser->GetUserID(), m_pTileEvent->GetTurn(), &cmd, sizeof(cmd) );
		if ( nres != 0 )
		{
			int nCurTurn = m_pTileEvent->GetTurn();

			// ˵��ʧ���ˣ�����ԭ��
			if ( nres == 3 && nCurTurn < nPlayerNbr)
			{
				// �Ʋ���
				// �ӵ�һ�ſ�ʼ��һ�Ÿ������ȥ
				for ( int i = 0; i < m_TilesHand[nCurTurn].CurrentLength(); ++i )
				{
					cmd.t = m_TilesHand[nCurTurn].GetTile(i);
					if ( m_TilesHand[nCurTurn].GetTileGenre( cmd.t ) < TILE_GENRE_FLOWER )
					{
						ProcessRequestGive( 0, pUser->GetUserID(), m_pTileEvent->GetTurn(), &cmd, sizeof(cmd) );
						break;
					}
				}
			}
			else if ( nres == 4 )
			{
				// ����ƻ᲻��������ˡ����������2�У������ƹ����ͳ����ģ�û���ƹ����������ҵ��¼ҷ���
				BOOL bGiveTile = FALSE;
				for ( int i = 0; i < nPlayerNbr; ++i )
				{
					// ������������ƶ�
					int nTurn = i;
					if ( m_TilesHand[nTurn].CurrentLength() % 3 == 2 )
					{
						// ��������
						m_pTileEvent->SetTurn( nTurn );
						m_pStateEvent->SetMJState( MJ_STATE_GIVE );
						bGiveTile = TRUE;
						break;
					}
				}

				if ( bGiveTile == FALSE )
				{
					int nTurn = 0;
					if ( nCurTurn < nPlayerNbr )
					{
						// û�˿��Գ��ƣ����¼ҷ���
						nTurn = ( nCurTurn + GetMJSetting().nGiveOrder ) % nPlayerNbr;
					}
					m_pTileEvent->SetTurn( nTurn );
					m_pStateEvent->SetMJState( MJ_STATE_DRAW );
				}
			}
		}
	}
	
}

template<int nPlayerNbr>
void CSoTilePlayerMgr<nPlayerNbr>::ShowDlgMsg(char* pMsg, int nLen, int nMsgID /* = 0 */, short nPlayerID /* = -1 */, BYTE byFlag /* = 0 */)
{
    if(NULL == pMsg || nLen < 1)
    {
        return;
    }
    	
	ISoGameProcessor* pProcess = GetSoGameProcess();
	if ( NULL == pProcess )
	{
		return;
	}
	
	TCMD_MJ_NOTIFY_SHOWDLG cmd;
	cmd.nMsgID = nMsgID;
	cmd.ucFlag = byFlag;
	
	if(nLen > 128)
    {
        nLen = 128;
    }
    strncpy(cmd.szMsg, pMsg, nLen);
    cmd.szMsg[127] = 0;
    
    if(nPlayerID == -1)
    {
        pProcess->SendGameData((char *)&cmd, sizeof(cmd));
    }
    else
    {
        pProcess->SendGameDataTo(nPlayerID, (char *)&cmd, sizeof(cmd));        
    }    
}

template<int nPlayerNbr>
void CSoTilePlayerMgr<nPlayerNbr>::SetTing( int nChair, BOOL bTing )
{
	if ( nChair >= 0 && nChair < nPlayerNbr )
    {
        if ( bTing ) 
        {
            // dxh add
            if (m_nTingType[nChair] == MJ_TING_NONE)
            {
                m_bTing[nChair] = bTing;
            }

            if ( m_TilesHand[nChair].CurrentLength() == 0 && m_TilesGive[nChair].CurrentLength() == 0 )
            {
                m_nTingType[nChair] = MJ_TING_BEGIN;
            }
            else
            {
                m_nTingType[nChair] = MJ_TING_NORMAL;
            }
        }
        else 
        {
            m_nTingType[nChair] = MJ_TING_NONE;
        }
	}
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::GetRequestLevel( int nChair, BYTE ucFlag, BYTE ucTile )
{
	int nRequestLevel = 0;
	
	// ��ȡ����ĵȼ�
	switch( ucFlag )
	{
	case ACTION_EMPTY:
		{
			nRequestLevel = 0;
		}
		break;
	case ACTION_QUADRUPLET_CONCEALED:
	case ACTION_QUADRUPLET_REVEALED:
	case ACTION_QUADRUPLET_PATCH:
		{
			nRequestLevel = GetMJSetting().nBlockLevelQuadruplet;
		}
		break;
	case ACTION_COLLECT:
		{
			nRequestLevel = GetMJSetting().nBlockLevelCollect;
		}
		break;
	case ACTION_TRIPLET:
		{
			nRequestLevel = GetMJSetting().nBlockLevelTriplet;
		}
		break;
	case ACTION_WIN:
		{
			nRequestLevel = GetMJSetting().nBlockLevelWin;
		}
		break;
	default:
		break;
	}

	return nRequestLevel;
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::IsWaitOtherRequest( int nChair, BYTE ucFlag, BYTE ucTile, int &nMaxLevel )
{
	if ( m_pTileEvent == NULL )
    {
        return 0;
    }
	
	int i;
	int nWait = 0;
	nMaxLevel = 0;
	// �������в������ҳ�������߲���Ȩ�޵�ֵ
	for ( i = 0; i < nPlayerNbr; ++i )
	{
		if ( m_nBlockLevel[i] >= nMaxLevel && m_nBlockLevel[i] > 0 )
		{
			nMaxLevel = m_nBlockLevel[i];
		}
	}

	int nOldTurn = m_pTileEvent->GetTurn();
	for ( i = 0; i < nPlayerNbr; ++i )
	{
		int nTurn = (nOldTurn + i) % nPlayerNbr;

		if (GetMJSetting().nGiveOrder > 1)
		{
			// ��ʱ��
			nTurn = (nOldTurn + nPlayerNbr - i) % nPlayerNbr;
		}

		if ( m_nBlockLevel[nTurn] == nMaxLevel && nMaxLevel > 0 )
		{
			// ֻ��ȴ���ߵ���
			if ( ACTION_EMPTY == m_nBlockRequest[nTurn] )
			{
				nWait = 1;
			}
		}

		if ( nTurn == nChair && m_nBlockLevel[nChair] == nMaxLevel )
		{
			// �������һ�ڶ��죬����Ҫ�������˵�
			if ( m_nBlockLevel[nChair] == GetMJSetting().nBlockLevelWin && GetMJSetting().nMoreWin )
			{
				continue;
			}
			else
			{
				break;
			}
		}	
	}
	
	return nWait;
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::SwitchBlockProcess( int nMaxLevel, TCMD_NOTIFY_BLOCK_RESULT &cmd )
{
	int nOldTurn = m_pTileEvent->GetTurn();

    for ( int i = 0; i < nPlayerNbr; ++i )
    {
        int nNextChair = (nOldTurn + i) % nPlayerNbr;

		if (GetMJSetting().nGiveOrder > 1)
		{
			// ��ʱ��
			nNextChair = (nOldTurn + nPlayerNbr - i) % nPlayerNbr;
		}

		int nLevel = m_nBlockLevel[nNextChair];
		m_nBlockLevel[nNextChair] = 0;
		if ( nLevel < nMaxLevel )
		{
			continue;
		}
		 
		cmd.ucChair = nNextChair;

		if ( 0 != SwitchBlockProcessOne( nNextChair, m_ucRequestTile[nNextChair], cmd ) )
		{
			return 1;
		}
	}	

	return 0;
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::SwitchBlockProcessOne( int nNextChair, BYTE ucRequestTile[4], TCMD_NOTIFY_BLOCK_RESULT &cmd )
{
	// �����жϵ����ΪĿǰִ�е����
	m_ucCurCheckChair = nNextChair;
	memcpy(&cmd.ucTile, ucRequestTile, sizeof(cmd.ucTile));
    
	switch( m_nBlockRequest[nNextChair] )
	{
	case ACTION_COLLECT:
		{
			if (SwitchBlockCollect(nNextChair, GetLastTile(), cmd) != 0)
			{
				// dxh �������Ƽ�¼
				m_bIsSet = TRUE;
			}
		}
		break;
	case ACTION_TRIPLET:
		{
			if (SwitchBlockTriplet(nNextChair, ucRequestTile[0], cmd) != 0)
			{
				// dxh �������Ƽ�¼
				m_bIsSet = TRUE;
			}
		}
		break;
	case ACTION_QUADRUPLET_CONCEALED:
	case ACTION_QUADRUPLET_DNXB:
		{
			SwitchBlockQuadrupletConcealed(nNextChair, ucRequestTile[0], cmd);
		}
		break;
	case ACTION_QUADRUPLET_ZFB:
		{
			SwitchBlockQuadrupletConcealed(nNextChair, ucRequestTile[3], cmd);
		}
		break;
	case ACTION_QUADRUPLET_REVEALED:
    case ACTION_QUADRUPLET_PATCH:
		{
			if (SwitchBlockQuadrupletRevealed(nNextChair, ucRequestTile[0], cmd) != 0)
			{
				return 1;
			}
		}
		break;
	case ACTION_WIN:
		{
			SwitchBlockWin(nNextChair, GetLastTile(), cmd);
		}
		break;
	default:
		break;
	}

	return 0;
}

template<int nPlayerNbr>
int CSoTilePlayerMgr<nPlayerNbr>::SwitchBlockTing( int nChair, BYTE ucFlag, BYTE ucTile )
{
	if ( ACTION_TING == ucFlag )
	{
		SetTing( nChair );
		TCMD_NOTIFY_BLOCK_RESULT cmd;
		cmd.ucChair = nChair;
		cmd.ucFlag = ACTION_TING;
		cmd.ucWhich = TRUE;
		GetSoGameProcess()->SendGameData( (char*)&cmd, sizeof(cmd) );
		return 0;
	}

	return 1;
}

#else
/******************************************************************************************************
C�˵�ʵ��
**************************/

template<int nPlayerNbr>
CGameTilePlayerMgr<nPlayerNbr>::CGameTilePlayerMgr()
{
	m_bAllowLookOn = TRUE;
}


template<int nPlayerNbr>
CGameTilePlayerMgr<nPlayerNbr>::~CGameTilePlayerMgr()
{

}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnGameMessage(UINT dwLen,  LPBYTE  pGameMsg)
{		
	if(pGameMsg != NULL && dwLen >= sizeof(TCMD_HEADER))
	{
		TCMD_HEADER *pData = (TCMD_HEADER *)pGameMsg;
		if ( MJFRAME_MOUDLE_MSG == pData->cCmdID )
		{
			switch( pData->cCmdPara )
			{
			case SC_MJ_NOTIFY_GIVE:
				OnNotifyGive( dwLen, pGameMsg );
				break;
			case SC_MJ_NOTIFY_DRAW:
				OnNotifyDraw( dwLen, pGameMsg );
				break;
			case SC_MJ_NOTIFY_FLOWER:
				OnNotifyFlower( dwLen, pGameMsg );
				break;
			case SC_MJ_NOTIFY_BLOCK:
				OnNotifyBlock( dwLen, pGameMsg );
				break;
			case SC_MJ_NOTIFY_DEAL:
				OnNotifyDeal( dwLen, pGameMsg );
				break;
			case SC_MJ_UPDATEHANDTILES:
				OnNotifyUpdateHandTiles( dwLen, pGameMsg );
				break;
			case SC_MJ_NOTIFY_CHANGE_LOOKON:
				OnNotifyChangeLookOn( dwLen, pGameMsg );
				break;
			case SC_MJ_NOTIFY_FLOWER_OVER:
				OnNotifyFlowerOver( dwLen, pGameMsg );
				break;
			default:
				break;
			}
		}
	}
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnNotifyChangeLookOn( UINT nLen, BYTE *pBuf )
{
	// �޸��Թ�״̬
	if ( pBuf == NULL || nLen == 0 )
		return;

	if ( NULL == m_pStateEvent || NULL == m_pTileEvent )
		return;

	// ����Ҳ����Թ��ߣ���������Ϣ
	if ( TRUE == CheckIsPlayer() )
	{
		return;
	}

	TCMD_NOTIFY_CHANGE_LOOKON *pCmd = (TCMD_NOTIFY_CHANGE_LOOKON*)pBuf;
	int nChair = GetLocalChair();
	m_bAllowLookOn = pCmd->ucLookOn;

    _DOUT1("�Թ���ҿ���, λ��=%d", nChair);

	if ( m_bAllowLookOn == TRUE )
	{
		int i;
		m_TilesHand[nChair].CurrentLength( pCmd->ucHandTilesLength );
		for ( i = 0; i < m_TilesHand[nChair].CurrentLength(); ++i )
		{
			m_TilesHand[nChair].SetTile( i, pCmd->ucHandTiles[i] );
		}

		SortHandTiles( m_TilesHand[nChair], 0, 1, &GetTilesGod() );

		int nGangNbr = 0;
		for ( i = 0; i < m_TilesSet[nChair].CurrentLength(); ++i )
		{
			TILE_SET tagSet;
			m_TilesSet[nChair].GetSet( i, tagSet );
			if ( ACTION_QUADRUPLET_CONCEALED == tagSet.m_ucFlag )
			{
				tagSet.m_ucTile = pCmd->ucQuadrupletConcealedTiles[nGangNbr];
				m_TilesSet[nChair].SetSet( i, tagSet );
				nGangNbr++;
				if ( nGangNbr >= pCmd->ucQuadrupletConcealedNbr )
				{
					break;
				}
			}
		}
	}

	RefreshMJCtrl();

}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnNotifyFlowerOver(UINT nLen, BYTE *pBuf)
{
	if ( pBuf == NULL || nLen == 0 )
		return;

	if ( NULL == m_pStateEvent || NULL == m_pTileEvent )
		return;

	if ( CheckIsPlayer() )
	{
		// ������ǳ����߼�飬�����˵ȴ�
		if ( GetLocalChair() != m_pTileEvent->GetTurn() )
		{
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
	}
	else
	{
		m_pStateEvent->SetMJState( MJ_STATE_WAIT );
	}
	
	RefreshMJCtrl();
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnNotifyFlower(UINT nLen, BYTE *pBuf)
{
	_DOUT("����--OnNotifyFlower");

	if ( pBuf == NULL || nLen == 0 )
		return;

	if ( NULL == m_pStateEvent || NULL == m_pTileEvent )
		return;

	TCMD_NOTIFY_FLOWER *pCmd = (TCMD_NOTIFY_FLOWER*)pBuf;
	// ������������Լ���������ҪCHECK
	int i;
	if ( pCmd->ucChair == GetLocalChair() )
	{
		for ( i = 0; i < pCmd->ucNumber; ++i )
		{
			m_TilesHand[pCmd->ucChair].DelTile( pCmd->ucOldTile[i] );
			m_TilesHand[pCmd->ucChair].AddTile( pCmd->ucTile[i] );
			// ��Ϸ�в����Ͳ�����
			if (  m_pStateEvent->GetMJState() == MJ_STATE_FLOWER )
			{
				SortHandTiles( m_TilesHand[pCmd->ucChair], 0, 1, &GetTilesGod() );
			}
		}

		ClearLoseHu( GetLocalChair() );
		if ( CheckFlower( pCmd->ucChair ) )
		{
			// ������Ϣ���󲹻��������ڵȴ�״̬
			RequestFlower( pCmd->ucChair );
			// ������ڲ����׶Σ�����
			if ( m_pStateEvent->GetMJState() != MJ_STATE_FLOWER )
			{
				m_pStateEvent->SetMJState( MJ_STATE_WAIT );
			}
		}
		else if ( m_pStateEvent->GetMJState() != MJ_STATE_FLOWER )
		{
			// ���ǳ��Ƶ���
			if ( GetLocalChair() != m_pTileEvent->GetTurn() )
			{
				m_pStateEvent->SetMJState( MJ_STATE_WAIT );			
			}
			else if ( CheckBlock(pCmd->ucChair) )
			{
				m_pStateEvent->SetMJState( MJ_STATE_BLOCK );
			}
			else
			{
				m_pStateEvent->SetMJState( MJ_STATE_GIVE );
			}
		}
	}
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnNotifyBlockCollect( TCMD_NOTIFY_BLOCK_RESULT *pCmd, int nOldTurn )
{
	if (NULL == m_pStateEvent || NULL == m_pTileEvent || pCmd == NULL)
    {
        return;
    }

	// �Ե�WHICH��ʾ���ǳԵ���һ��
	//m_TilesSet[pCmd->ucChair].AddSet( pCmd->ucTile, ACTION_COLLECT, nOldTurn);  // ����3: ������Ϊ�Ե���һ��, �ͻ���Ϊ�Ե��Ǹ���
    m_TilesSet[pCmd->ucChair].AddSet( pCmd->ucTile + pCmd->ucWhich, ACTION_COLLECT, pCmd->ucWhich);

	//m_TilesGive[nOldTurn].DelTile( pCmd->ucTile + pCmd->ucWhich );  // dxh ��������
    m_TilesGive[nOldTurn].SetBlockFlag(m_TilesGive[nOldTurn].CurrentLength() - 1, TRUE);

	if ( pCmd->ucChair != GetLocalChair() )
	{
		m_TilesHand[pCmd->ucChair].CurrentLength( m_TilesHand[pCmd->ucChair].CurrentLength() - 2 );
		m_pStateEvent->SetMJState( MJ_STATE_WAIT );
	}
	else
	{
		for ( int i = 0; i < 3; ++i )
		{
			if ( i != pCmd->ucWhich )
            {
                m_TilesHand[pCmd->ucChair].DelTile( pCmd->ucTile + i );
            }
		}

		m_pStateEvent->SetMJState( MJ_STATE_GIVE, TRUE );
	}

	if ( pCmd->ucChair == GetLocalChair() )
	{
		/* û������, ��������Ҫ����
		SortHandTiles( m_TilesHand[pCmd->ucChair], 0, 1, &GetTilesGod() );
		*/ 

		if ( TRUE == CheckTing( pCmd->ucChair ) && FALSE == GetTing( pCmd->ucChair ) )
		{
			m_pStateEvent->SetMJState( MJ_STATE_BLOCK );
		}
	}

}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnNotifyBlockTriplet( TCMD_NOTIFY_BLOCK_RESULT *pCmd, int nOldTurn )
{
	if ( NULL == m_pStateEvent || NULL == m_pTileEvent || pCmd == NULL )
		return;

	m_TilesSet[pCmd->ucChair].AddSet( pCmd->ucTile, ACTION_TRIPLET, pCmd->ucWhich );
	//m_TilesGive[pCmd->ucWhich].DelTile( pCmd->ucTile );  // dxh ��������
    m_TilesGive[pCmd->ucWhich].SetBlockFlag(m_TilesGive[pCmd->ucWhich].CurrentLength() - 1, TRUE);

	if ( pCmd->ucChair != GetLocalChair() )
	{
		m_TilesHand[pCmd->ucChair].CurrentLength( m_TilesHand[pCmd->ucChair].CurrentLength() - 2 );
		m_pStateEvent->SetMJState( MJ_STATE_WAIT );
	}
	else
	{
		m_TilesHand[pCmd->ucChair].DelTile( pCmd->ucTile );
		m_TilesHand[pCmd->ucChair].DelTile( pCmd->ucTile );
		m_pStateEvent->SetMJState( MJ_STATE_GIVE, TRUE );
	}

	if ( pCmd->ucChair == GetLocalChair() )
	{
		/* û������, ��������Ҫ����
		SortHandTiles( m_TilesHand[pCmd->ucChair], 0, 1, &GetTilesGod() );
		*/

		if ( TRUE == CheckTing( pCmd->ucChair ) && FALSE == GetTing( pCmd->ucChair ) )
		{
			m_pStateEvent->SetMJState( MJ_STATE_BLOCK );
		}
	}

}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnNotifyBlockQuadrupletRevealed( TCMD_NOTIFY_BLOCK_RESULT *pCmd, int nOldTurn )
{
	if ( NULL == m_pStateEvent || NULL == m_pTileEvent || pCmd == NULL )
		return;

	// �������DRAW
	m_shGangState = MJ_GANG_STATE_DRAW;

	m_pStateEvent->SetMJState( MJ_STATE_WAIT );
	BOOL bBuGang = FALSE;
	for ( int i = 0; i < m_TilesSet[pCmd->ucChair].CurrentLength(); ++i )
	{
		TILE_SET tagSet;
		m_TilesSet[pCmd->ucChair].GetSet( i, tagSet );
		if ( tagSet.m_ucFlag == ACTION_TRIPLET && tagSet.m_ucTile == pCmd->ucTile )
		{
			bBuGang = TRUE;
			break;
		}
	}

	// �Լ���
	if ( pCmd->ucChair == nOldTurn )
	{
		if ( pCmd->ucChair != GetLocalChair() )
		{
			m_TilesHand[pCmd->ucChair].CurrentLength( m_TilesHand[pCmd->ucChair].CurrentLength() - 1 );
		}
		else
		{
			m_TilesHand[pCmd->ucChair].DelTile( pCmd->ucTile );
		}

		// ��������
		for ( i = 0; i < m_TilesSet[pCmd->ucChair].CurrentLength(); ++i )
		{
			TILE_SET tagSet;
			m_TilesSet[pCmd->ucChair].GetSet( i, tagSet );
			if ( tagSet.m_ucFlag == ACTION_TRIPLET && tagSet.m_ucTile == pCmd->ucTile )
			{
				//tagSet.m_ucChair = pCmd->ucChair;  // dxh ��������, ����֮ǰ��˭����
				tagSet.m_ucFlag = ACTION_QUADRUPLET_PATCH;
				m_TilesSet[pCmd->ucChair].SetSet( i, tagSet );
				break;
			}
		}

		// m_TilesSet[pCmd->ucChair].TripletToQuadruplet( pCmd->ucTile );

		// �����ж�
		// �Լ��Ͳ����ж���
		if ( GetMJSetting().nQiangGang && GetLocalChair() != pCmd->ucChair )
		{
			// ����������ܣ�����Ҫ�����һ���ƣ����óɸܵ�����
			SetLastTile( pCmd->ucTile );

			if ( GetMJSetting().nBlockLevelWin == CheckBlock( GetLocalChair(), FALSE, pCmd->ucTile, GetMJSetting().nBlockLevelWin ) )
			{
				// �ֶ������������Ʊ��
				memset( m_bBlockEnable, 0, sizeof(m_bBlockEnable ) );
				m_bBlockEnable[MJ_BLOCK_WIN] = TRUE;
				m_pStateEvent->SetMJState( MJ_STATE_BLOCK );

				// �Լ����л���ȥ
				m_shGangState = MJ_GANG_STATE_BLOCK;
			}
            else
            {
                memset(m_bBlockEnable, 0, sizeof(m_bBlockEnable));
            }
		}
	}
	else
	{
		m_TilesSet[pCmd->ucChair].AddSet( pCmd->ucTile, ACTION_QUADRUPLET_REVEALED, pCmd->ucWhich );
		//m_TilesGive[nOldTurn].DelTile( pCmd->ucTile );  // dxh ��������
        m_TilesGive[nOldTurn].SetBlockFlag(m_TilesGive[nOldTurn].CurrentLength() - 1, TRUE);

		if ( pCmd->ucChair != GetLocalChair() )
		{
			m_TilesHand[pCmd->ucChair].CurrentLength( m_TilesHand[pCmd->ucChair].CurrentLength() - 3 );
		}
		else
		{
			m_TilesHand[pCmd->ucChair].DelTile( pCmd->ucTile );
			m_TilesHand[pCmd->ucChair].DelTile( pCmd->ucTile );
			m_TilesHand[pCmd->ucChair].DelTile( pCmd->ucTile );
		}
	}

	m_nAllGangNbr++;

	/* û������, ��������Ҫ����
	if ( pCmd->ucChair == GetLocalChair() )
	{
		SortHandTiles( m_TilesHand[pCmd->ucChair], 0, 1, &GetTilesGod() );
	}*/
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnNotifyBlockQuadrupletConcealed( TCMD_NOTIFY_BLOCK_RESULT *pCmd, int nOldTurn )
{
    if (NULL == m_pStateEvent || NULL == m_pTileEvent || pCmd == NULL)
    {
        return;
    }

    // ������Ƶ�������
    m_shGangState = MJ_GANG_STATE_DRAW;
    m_pStateEvent->SetMJState(MJ_STATE_WAIT);

    OutputDebugString("dxhout: ����");

    if (ACTION_QUADRUPLET_CONCEALED == pCmd->ucFlag)
    {
        // ��������
        m_TilesSet[pCmd->ucChair].AddSet(pCmd->ucTile, ACTION_QUADRUPLET_CONCEALED, pCmd->ucWhich);

        if (pCmd->ucChair != GetLocalChair())
        {
            m_TilesHand[pCmd->ucChair].CurrentLength(m_TilesHand[pCmd->ucChair].CurrentLength() - 4);
        }
        else
        {
            m_TilesHand[pCmd->ucChair].DelTile( pCmd->ucTile );
            m_TilesHand[pCmd->ucChair].DelTile( pCmd->ucTile );
            m_TilesHand[pCmd->ucChair].DelTile( pCmd->ucTile );
            m_TilesHand[pCmd->ucChair].DelTile( pCmd->ucTile );
        }
    }
    else if (ACTION_QUADRUPLET_DNXB == pCmd->ucFlag)
    {
        // �����: ����������
        m_TilesSet[pCmd->ucChair].AddSet(TILE_EAST, ACTION_QUADRUPLET_DNXB, pCmd->ucWhich);

        if (pCmd->ucChair != GetLocalChair())
        {
            m_TilesHand[pCmd->ucChair].CurrentLength(m_TilesHand[pCmd->ucChair].CurrentLength() - 4);
        }
        else
        {
            // ɾ���ܵ�������
            m_TilesHand[pCmd->ucChair].DelTile(TILE_EAST);
            m_TilesHand[pCmd->ucChair].DelTile(TILE_SOUTH);
            m_TilesHand[pCmd->ucChair].DelTile(TILE_WEST);
            m_TilesHand[pCmd->ucChair].DelTile(TILE_NORTH);
        }
    }
    else if (ACTION_QUADRUPLET_ZFB == pCmd->ucFlag)
    {
        // �����: �з���X��
        m_TilesSet[pCmd->ucChair].AddSet(pCmd->ucTile, ACTION_QUADRUPLET_ZFB, pCmd->ucWhich);

        if (pCmd->ucChair != GetLocalChair())
        {
            m_TilesHand[pCmd->ucChair].CurrentLength(m_TilesHand[pCmd->ucChair].CurrentLength() - 4);
        }
        else
        {
            // ɾ���ܵ�������
            m_TilesHand[pCmd->ucChair].DelTile(TILE_ZHONG);
            m_TilesHand[pCmd->ucChair].DelTile(TILE_FA);
            m_TilesHand[pCmd->ucChair].DelTile(TILE_BAI);
            m_TilesHand[pCmd->ucChair].DelTile(pCmd->ucTile);
        }
    }

    m_nAllGangNbr++;

	/* û������, ��������Ҫ����
	if ( pCmd->ucChair == GetLocalChair() )
	{
		SortHandTiles( m_TilesHand[pCmd->ucChair], 0, 1, &GetTilesGod() );
	}*/
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnNotifyBlockTing( TCMD_NOTIFY_BLOCK_RESULT *pCmd, int nOldTurn )
{
	if ( NULL == m_pStateEvent || NULL == m_pTileEvent || pCmd == NULL )
		return;

    // ���������ƶ���
    SetTing( pCmd->ucChair, pCmd->ucWhich );

	/* û������, ��������Ҫ����
	if ( pCmd->ucChair == GetLocalChair() )
	{
		SortHandTiles( m_TilesHand[pCmd->ucChair], 0, 1, &GetTilesGod() );
	}*/
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnNotifyBlock(UINT nLen, BYTE *pBuf)
{
	if ( pBuf == NULL || nLen == 0 )
		return;

	if ( NULL == m_pStateEvent || NULL == m_pTileEvent )
		return;


	TCMD_NOTIFY_BLOCK_RESULT *pCmd = (TCMD_NOTIFY_BLOCK_RESULT *)pBuf;

    if (pCmd->ucChair == GetLocalChair() )
    {
        // ���©����ʶ
        ClearLoseHu(GetLocalChair());
    }

    // dxh ��������������״̬
    memset(m_nBlockLevel, 0, sizeof(m_nBlockLevel));
    memset(m_bBlockEnable, 0, sizeof(m_bBlockEnable));

    int nOldTurn = m_pTileEvent->GetTurn();
    m_pTileEvent->SetTurn( pCmd->ucChair );
    SetShowTurn(pCmd->ucChair);   // dxh add

	switch( pCmd->ucFlag )
	{
	case ACTION_COLLECT:
		{
			OnNotifyBlockCollect( pCmd, nOldTurn );
		}
		break;
	case ACTION_TRIPLET:
		{
			OnNotifyBlockTriplet( pCmd, nOldTurn );
		}
		break;
	case ACTION_QUADRUPLET_REVEALED:
	case ACTION_QUADRUPLET_PATCH:
		{
			OnNotifyBlockQuadrupletRevealed( pCmd, nOldTurn );
		}
		break;
	case ACTION_QUADRUPLET_CONCEALED:
    case ACTION_QUADRUPLET_DNXB:
    case ACTION_QUADRUPLET_ZFB:
		{
			OnNotifyBlockQuadrupletConcealed( pCmd, nOldTurn );
		}
		break;
	case ACTION_TING:
		{
			OnNotifyBlockTing( pCmd, nOldTurn );
		}
		break;
	default:
		break;
	}

	//RefreshMJCtrl();
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnNotifyGive(UINT nLen, BYTE *pBuf)
{
	if ( pBuf == NULL || nLen == 0 )
		return;

	if ( NULL == m_pStateEvent || NULL == m_pTileEvent )
		return;
	
	TCMD_NOTIFY_GIVE *pCmd = (TCMD_NOTIFY_GIVE*)pBuf;

    // �����ƺ�, ���ڴ˵����õ�ǰ������Ҷ���ָ����һ��, ����һ��ץ����ʱ, �����õ�ǰ�������ָʾ
	//m_pTileEvent->SetTurn( pCmd->chair );
	//SetShowTurn( ( m_pTileEvent->GetTurn() + GetMJSetting().nGiveOrder ) % nPlayerNbr );

    _DOUT4("=============��ǰ���λ��: %d, ����ǰ, ��������=%d, �������λ��: %d, ��������=%d,=================", 
        pCmd->chair, m_TilesHand[pCmd->chair].CurrentLength(), GetLocalChair(), m_TilesHand[GetLocalChair()].CurrentLength());

	SetLastGiveChair( pCmd->chair );
	SetLastTile( pCmd->t );

    if (pCmd->bTing)
    {
        // ����������Ϊ����������
        m_TilesGive[pCmd->chair].SetTingFlag(pCmd->chair, TRUE);
    }

	if ( m_shGangState == MJ_GANG_STATE_DRAW )
		m_shGangState = MJ_GANG_STATE_GIVE;
	else
		m_shGangState = MJ_GANG_STATE_NONE;

	if ( pCmd->chair == GetLocalChair() )
	{
		// �Ƿ���Լ��������ͬһ��
		if ( m_ucLocalGive != pCmd->t && CheckIsPlayer() )
		{
			// ��һ����ͬ��SO
			// �����û����ſ�����֣���ô�Ʊ���
			if ( m_ucLocalGive != TILE_BEGIN )
			{
				// �п����Ƿ��������
				m_TilesHand[pCmd->chair].AddTile( m_ucLocalGive );
				m_TilesGive[pCmd->chair].DelTile( m_ucLocalGive );
            }

			m_TilesHand[pCmd->chair].DelTile( pCmd->t );
			m_TilesGive[pCmd->chair].AddTile( pCmd->t );
		}
		else if ( FALSE == CheckIsPlayer() )
		{
			m_TilesHand[pCmd->chair].DelTile( /*pCmd->t*/ TILE_BEGIN);
			m_TilesGive[pCmd->chair].AddTile( pCmd->t );
			//SortHandTiles( m_TilesHand[pCmd->chair], 0, 1, &GetTilesGod() );
		}
		
		SetLocalGive( TILE_BEGIN );

		// �ȴ�
		m_pStateEvent->SetMJState( MJ_STATE_WAIT );
	}
	else
	{
		m_TilesHand[pCmd->chair].CurrentLength(m_TilesHand[pCmd->chair].CurrentLength()-1);
		m_TilesGive[pCmd->chair].AddTile( pCmd->t );

		// ��鱾�ص��Ƿ������ƣ�������������ͽ������Ƶ�״̬����֪ͨSO��SO��Ҳ��ͬ���Ĳ���
		if ( CheckBlock( GetLocalChair(), FALSE, pCmd->t ) )
		{
			m_pStateEvent->SetMJState( MJ_STATE_BLOCK );
		}
		else
		{
			// �ȴ�
			m_pStateEvent->SetMJState( MJ_STATE_WAIT );
		}
	}

    _DOUT4("=============��ǰ���λ��: %d, ���ƺ�, ��������=%d, �������λ��: %d, ��������=%d,=================", 
        pCmd->chair, m_TilesHand[pCmd->chair].CurrentLength(), GetLocalChair(), m_TilesHand[GetLocalChair()].CurrentLength());
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnNotifyUpdateHandTiles(UINT nLen, BYTE *pBuf)
{
	_DOUT("�ͻ����������������һ��, �ӷ������ϸ�������");

	if ( pBuf == NULL || nLen == 0 )
		return;

	if ( NULL == m_pStateEvent || NULL == m_pTileEvent )
		return;

	TCMD_NOTIFY_UPDATEHANDTILES *pCmd = (TCMD_NOTIFY_UPDATEHANDTILES*)pBuf;

	if ( pCmd->ucChair != GetLocalChair() )
		return;

	m_TilesHand[pCmd->ucChair].CurrentLength( pCmd->ucHandCount );
	int i,j;
	for ( i = 0; i < pCmd->ucHandCount; ++i )
	{
		m_TilesHand[pCmd->ucChair].SetTile( i, pCmd->ucHandTiles[i]);
	}
	for ( i = 0; i < nPlayerNbr && i < 4; ++i )
	{
		m_TilesGive[i].CurrentLength( pCmd->ucGiveCount[i] );
		for ( j = 0; j < pCmd->ucGiveCount[i]; ++j )
		{
			m_TilesGive[i].SetTile( j, pCmd->ucGiveTiles[i][j] );
		}
	}

	SortHandTiles( m_TilesHand[pCmd->ucChair], 0, 1, &GetTilesGod() );

    // dxh ------------ GMCmd �����ʱ, ���¼��������Ϣ -------------
    // �жϵ�ǰ�Ƿ�Ϊ�������
    //if (CheckFlower(pCmd->chair))
    //{
    //    // ������Ϣ���󲹻��������ڵȴ�״̬
    //    RequestFlower( pCmd->chair );
    //    m_pStateEvent->SetMJState(MJ_STATE_WAIT);
    //}
    //else 
    if (m_pTileEvent->GetTurn() == pCmd->ucChair)
    {
        if (CheckBlock(pCmd->ucChair))
        {
            m_pStateEvent->SetMJState(MJ_STATE_BLOCK);
        }
    }
    // ------------ GMCmd �����ʱ, ���¼��������Ϣ -------------------

    _DOUT2("ͬ����������������: ���λ��:%d, ������=%d", pCmd->ucChair, pCmd->ucHandCount);

    // ǿ��ˢ������
	RefreshMJCtrl(TRUE);	
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnNotifyDraw(UINT nLen, BYTE *pBuf)
{
	if ( pBuf == NULL || nLen == 0 )
		return;

	if ( NULL == m_pStateEvent || NULL == m_pTileEvent )
		return;

	TCMD_NOTIFY_DRAW *pCmd = (TCMD_NOTIFY_DRAW*)pBuf;

    _DOUT2("=================���λ��: %d, ץ��ǰ, ��������=%d==================", pCmd->chair, m_TilesHand[pCmd->chair].CurrentLength());

	m_TilesHand[pCmd->chair].AddTile( pCmd->t );

	SetLastTile( pCmd->t );
	
	// ��������ƣ�����֮
	if ( CheckIsAutoWinTile() )
	{
		m_pStateEvent->SetMJState( MJ_STATE_WAIT );
	}
	else if ( pCmd->chair == GetLocalChair() )
	{
		ClearLoseHu( GetLocalChair() );
		if ( CheckFlower( pCmd->chair ) )
		{
			// ������Ϣ���󲹻��������ڵȴ�״̬
			RequestFlower( pCmd->chair );
			m_pStateEvent->SetMJState( MJ_STATE_WAIT );
		}
		else if ( CheckBlock(pCmd->chair) )
		{
			m_pStateEvent->SetMJState( MJ_STATE_BLOCK );
		}
		else
		{
			m_pStateEvent->SetMJState( MJ_STATE_GIVE );
		}
	}
	else
	{
		m_pStateEvent->SetMJState( MJ_STATE_WAIT );
	}

	if ( m_shGangState != MJ_GANG_STATE_DRAW )
	{
		m_shGangState = MJ_GANG_STATE_NONE;
	}

    _DOUT2("=================���λ��: %d, ץ�ƺ�, ��������=%d==================", pCmd->chair, m_TilesHand[pCmd->chair].CurrentLength());
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnNotifyDeal(UINT nLen, BYTE *pBuf)
{
	if ( NULL == pBuf || nLen == 0 )
		return;

	IUserManager* pUserMgr = GetSoUserManager();
	if ( NULL == pUserMgr )
	{
		return;
	}

	IUser* pUser = pUserMgr->GetLocalUser();
	if ( NULL == pUser )
	{
		return;
	}

	if ( NULL == m_pStateEvent || NULL == m_pTileEvent )
	{
		return;
	}

	
	TCMD_NOTIFY_DEAL *pCmd = (TCMD_NOTIFY_DEAL*)pBuf;

	
	int i,j;
	for ( i = 0; i < nPlayerNbr; ++i )
	{
		m_TilesHand[i].ReleaseAll();
		m_TilesSet[i].ReleaseAll();
		m_TilesGive[i].ReleaseAll();
		ClearLoseHu( i ); 
	}

	m_TilesGod.ReleaseAll();
	m_TilesGod.CurrentLength( pCmd->ucGodLength );
	if ( pCmd->ucGodLength > 0 )
	{
		for ( i = 0; i < pCmd->ucGodLength; ++i )
		{
			m_TilesGod.SetTile( i, pCmd->ucGodTiles[i] );
		}
	}
	
	// ��ո�����Ϣ
	m_nAllGangNbr = 0;
	m_nOldGangNbr = 0;

    // 2010-12-16 dxh ����ҷ��������ƺ��һ�ξ�����������, ��ձ��س�������
    SetLocalGive( TILE_BEGIN );

	for ( j = 0; j < nPlayerNbr; ++j )
	{
		CMJHand mjHand;
		// �����Լ���������Ϣֱ���������ˣ���Ϊ��Ϣ��ֻ�������Լ�������
		if ( j == pUser->GetSeatID() )
		{
			for ( i = 0; i < m_tagMJSetting.nTilesHandNbr; ++i )
			{
				mjHand.AddTile( pCmd->tiles[i] );
                _DOUT2("���շ���������, ��%d����, ��ֵ=%d", i, pCmd->tiles[i]);
			}

			if ( j == m_pTileEvent->GetDealer() )
			{
				mjHand.AddTile( pCmd->tiles[m_tagMJSetting.nTilesHandNbr] );
				// �������һ��
				SetLastTile( pCmd->tiles[m_tagMJSetting.nTilesHandNbr] );
			}
		}
		else
		{
			//int nLength = m_tagMJSetting.nTilesHandNbr;
			//if ( j == m_pTileEvent->GetDealer() )
			//{
			//	nLength++;
			//	// �������һ��
			//	SetLastTile( TILE_BEGIN );
			//}
			//mjHand.CurrentLength(nLength);

            for ( i = 0; i < m_tagMJSetting.nTilesHandNbr; ++i )
            {
                mjHand.AddTile( TILE_BEGIN );
                _DOUT2("���շ���������, ��%d����, ��ֵ=%d", i, pCmd->tiles[i]);
            }

            if ( j == m_pTileEvent->GetDealer() )
            {
                mjHand.AddTile( pCmd->tiles[m_tagMJSetting.nTilesHandNbr] );

                // �������һ��
                SetLastTile( pCmd->tiles[m_tagMJSetting.nTilesHandNbr] );
            }
		}

		SetHandTile( j, mjHand );
	}
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::RequestGive( TILE ucTile, int nSel )
{
	if ( NULL == m_pStateEvent || NULL == m_pTileEvent )
		return;

	if ( MJ_STATE_SELECT_GIVE != m_nStateSelect )
	{
        _DOUT("����4: ��ǰ״̬���ǳ���״̬");
		return;
	}

	IGameProcess* pProcess = GetGameProcess();
	if ( NULL == pProcess )
	{
		return;
	}

	if ( GetTileGenre( ucTile ) >= TILE_GENRE_FLOWER )
	{
		return;
	}

	// �Թ���Ҳ��������
	if ( FALSE == CheckIsPlayer() )
	{
        _DOUT("����5: �Թ���Ҳ��ܳ���");
		return;
	}

    // ��������
    //m_TilesGive[2].AddTile( ucTile );
    ////m_TilesGive[2].SetBlockFlag(m_TilesGive[2].CurrentLength() - 1, TRUE);

    //char szText[250];
    //sprintf(szText, "dxhout: ����%d����: %d", m_TilesGive[2].CurrentLength(), ucTile);
    //OutputDebugString(szText);
    //SetLastTile(ucTile);
    //return ;

	// ������Ҫ�󣬱����Լ����2��
	if ( m_TilesHand[GetLocalChair()].CurrentLength() % 3 != 2 )
	{
        _DOUT1("����6: �����������, ���Ƴ�����������, ������=%d ȡ��3������2", m_TilesHand[GetLocalChair()].CurrentLength());
		return;
	}

    _DOUT2("============���:%d, �������ǰ, ��������=%d============", GetLocalChair(), m_TilesHand[GetLocalChair()].CurrentLength());

	// ���ر���
	SetLocalGive( ucTile );	

	m_TilesHand[GetLocalChair()].DelTile( ucTile );
    m_TilesGive[GetLocalChair()].AddTile( ucTile );
	SortHandTiles( m_TilesHand[GetLocalChair()], 0, 1, &GetTilesGod() );

	TCMD_REQUEST_GIVE cmd;
	cmd.t = ucTile;

	// �ѱ��ص��Ƴ��ϱ���ȥ����ֹ���ֳ��Ȳ���ͷ
	cmd.ucTileLength = m_TilesHand[GetLocalChair()].CurrentLength();

    cmd.ucIndex = nSel;  // dxh add
    // ������ó�����λ�ã����ⱻ������Ҳ³�����
    //cmd.ucIndex = nSel % cmd.ucTileLength;  // dxh add

	pProcess->SendGameMessage( (char*)&cmd );

	// ����ȫ����ѡ
	SetAllCanSelect();

	// �����ƣ��ȴ�
	m_pStateEvent->SetMJState( MJ_STATE_WAIT );

    _DOUT2("============���:%d, ������ƺ�, ��������=%d============", GetLocalChair(), m_TilesHand[GetLocalChair()].CurrentLength());
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::RequestBlock( BYTE ucFlag, TILE ucTile )
{
    /*if (ucTile == 0)
    {
        TILE_SET tiles;
        tiles.m_ucChair = 1;
        tiles.m_ucTile = TILE_CHAR_4;
        tiles.m_ucFlag = ACTION_TRIPLET;

        m_TilesSet[3].AddSet(tiles);
        m_TilesGive[2].SetBlockFlag(m_TilesGive[2].CurrentLength() - 1, TRUE);
    }
    else
    {
        TILE_SET tiles;
        m_TilesSet[3].GetSet(m_TilesSet[3].CurrentLength() - 1, tiles);
        tiles.m_ucFlag = ACTION_QUADRUPLET_PATCH;
        m_TilesSet[3].SetSet(m_TilesSet[3].CurrentLength() - 1, tiles);
    }
    return ;*/

	if ( NULL == m_pStateEvent || NULL == m_pTileEvent )
		return;

	IGameProcess* pProcess = GetGameProcess();
	if ( NULL == pProcess )
	{
		return;
	}

	// ���BLOCK״̬��
	memset( m_bBlockEnable, 0, sizeof(m_bBlockEnable) );
	SetAllCanSelect();

	if ( FALSE == CheckIsPlayer() )
	{
		return;
	}

	TCMD_REQUEST_BLOCK cmd;
	cmd.ucFlag = ucFlag;
	cmd.ucTile = ucTile;

	// ���������ĺ��ƣ�Ҫ���Լ����������滻Ϊ���ķ�����������ĵ��Ƚϴ��ʱ�䣬��ߴ�4��
// 	if ( ACTION_WIN == ucFlag && GetTilesGod().CurrentLength() > 0 )
// 	{
// 		int nLaiziCount = 0;
// 		CMJHand mjTilesNoGod;
// 		CMJHand mjTilesGod;
// 		CMJHand mjLaizi;
// 		BYTE ucMyChair = GetLocalChair();
// 		nLaiziCount = GetTilesNoGod( m_TilesHand[GetLocalChair()], mjTilesNoGod, mjTilesGod );
// 		if ( nLaiziCount > 0 )
// 		{
// 			ConfigGodTiles( mjTilesNoGod, m_TilesSet[ucMyChair], nLaiziCount, mjLaizi, mjTilesGod );
// 			for ( int i = 0; i < nLaiziCount; ++i )
// 			{
// 				cmd.ucBlockInfo[i] = mjLaizi.GetTile( i );
// 			}
// 		}	
// 	}

	pProcess->SendGameMessage( (char*)&cmd );

	if ( m_pStateEvent->GetMJState() == MJ_STATE_BLOCK )
	{
		if ( ACTION_EMPTY == ucFlag )
		{
			// �Լ�ȡ�����ƣ��������״̬��������ȡ������ȴ�
			if ( m_pTileEvent->GetTurn() != GetLocalChair() )
			{
				m_pStateEvent->SetMJState( MJ_STATE_WAIT );
			}
			else
			{
				m_pStateEvent->SetMJState( MJ_STATE_GIVE );
			}
		}
		else 
		{ 
			m_pStateEvent->SetMJState( MJ_STATE_WAIT );
		}
	}

    RefreshMJCtrl(TRUE);
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::ClearPlayerTiles( BYTE ucChair )
{
	m_TilesHand[ucChair].ReleaseAll();
	m_TilesSet[ucChair].ReleaseAll();
	m_TilesGive[ucChair].ReleaseAll();
}

template<int nPlayerNbr>
BOOL CGameTilePlayerMgr<nPlayerNbr>::CheckIsPlayer()
{
	IUserManager* pUserMgr = GetSoUserManager();
	if ( NULL == pUserMgr )
		return FALSE;

	IUser* pUser = pUserMgr->GetLocalUser();
	if ( NULL == pUser )
		return FALSE;

    // 2011-1-6 dxh add
    return (pUser->GetState() != EUS_SPECTATING);
	//return pUser->IsPlayer();
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::RequestFlower( int nChair )
{
	if ( NULL == m_pStateEvent || NULL == m_pTileEvent )
		return;

	IGameProcess* pProcess = GetGameProcess();
	if ( NULL == pProcess )
	{
		return;
	}

	if ( FALSE == CheckIsPlayer() )
	{
		return;
	}

	TCMD_REQUEST_FLOWER cmd;
	cmd.ucChair = nChair;
	cmd.ucNumber = CheckFlower( nChair );
	pProcess->SendGameMessage( (char*)&cmd );
}

/************************************************************************/
/* ��Ϸ��ʼ                                                                     */
/************************************************************************/
template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnGameStart()
{
	m_ucLocalGive = TILE_BEGIN;
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::SetAllCanSelect( BOOL bCan )
{
	if ( TRUE == bCan )
	{
		memset( m_nTileSelect, 0, sizeof(m_nTileSelect) );
	}
	else
	{
		for ( int i = 0; i < MAX_HAND_TILES; ++i )
		{
			m_nTileSelect[i] = MJ_TILES_SELECT_NO;
		}
	}
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::SetTilesSelect( int* pTilesSelect, int nLength )
{
	if ( NULL == pTilesSelect )
		return;

	if ( nLength >= MAX_HAND_TILES || nLength < 0 )
		return;

	for ( int i = 0; i < nLength; ++i )
	{
		m_nTileSelect[i] = pTilesSelect[i];
	}
}

// ��ȡѡ�����Ϣ
template<int nPlayerNbr>
int CGameTilePlayerMgr<nPlayerNbr>::GetTileSelect( int nIndex )
{
	if ( nIndex < 0 || nIndex >= MAX_HAND_TILES )
		return 0;

	return m_nTileSelect[nIndex];
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::SetTileSelect( int nIndex, int nSelect )
{
	if ( nIndex < 0 || nIndex >= MAX_HAND_TILES )
		return;

	if ( nSelect < 0 || nSelect > MJ_TILES_SELECT_CHOOSE )
		nSelect = 0;
	
	m_nTileSelect[nIndex] = nSelect;
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnSetSelectStateColloect()
{
	//int i;
	//// ֻ�����ֿ�ѡ
	//int nChair = GetLocalChair();
	//CMJHand* pMJHand = &m_TilesHand[nChair];
	//TILE ucLastTile = GetLastTile();
	//CMJHand mjCheck;

	//// �����������в��ܱ������ܣ�Ҫ�ɳ���
	//if ( GetMJSetting().nCanBlockLaizi == 0 && GetTilesGod().CurrentLength() > 0 )
	//{
	//	CMJHand mjGod = GetTilesGod();
	//	for ( int i = 0; i < m_TilesHand[nChair].CurrentLength(); ++i )
	//	{
	//		TILE ucTile = m_TilesHand[nChair].GetTile( i );
	//		if ( !mjGod.IsHave( ucTile ) )
	//		{
	//			mjCheck.AddTile( ucTile );
	//		}
	//	}
	//}
	//else
	//{
	//	mjCheck.AddTiles( m_TilesHand[nChair] );
	//}
	//
	//int nSelNbr = mjCheck.IsCanCollect( ucLastTile, GetMJSetting().nCollateWind );
	//if ( 0 == nSelNbr )
	//	return;

	//// ȫ������ѡ��
	//SetAllCanSelect( FALSE );

	//// ���Ƶ�����ǰ��
	//if ( 1 == ( nSelNbr / COLLECT_RES_XOO ) )
	//{
	//	// ���Ƶ�ǰ2������Ϊ��ѡ
	//	for ( i = 0; i < pMJHand->CurrentLength(); ++i )
	//	{
	//		if ( ( ucLastTile + 2 ) == pMJHand->GetTile( i ) 
	//			||
	//			( ucLastTile + 1 ) == pMJHand->GetTile( i )  )
	//		{
	//			SetTileSelect( i, MJ_TILES_SELECT_CAN );
	//		}
	//	}
	//	
	//	nSelNbr = nSelNbr % COLLECT_RES_XOO;
	//}

	//// �м�
	//if ( 1 == ( nSelNbr / COLLECT_RES_OXO ) )
	//{
	//	// ���Ƶ�ǰ1�źͺ�1������Ϊ��ѡ
	//	for ( i = 0; i < pMJHand->CurrentLength(); ++i )
	//	{
	//		if ( ( ucLastTile + 1 ) == pMJHand->GetTile( i ) 
	//			||
	//			( ucLastTile - 1 ) == pMJHand->GetTile( i )  )
	//		{
	//			SetTileSelect( i, MJ_TILES_SELECT_CAN );
	//		}
	//	}
	//	
	//	nSelNbr = nSelNbr % COLLECT_RES_OXO;
	//}

	//// ����
	//if ( 1 == ( nSelNbr / COLLECT_RES_OOX ) )
	//{
	//	// ���Ƶ�ǰ1�źͺ�1������Ϊ��ѡ
	//	for ( i = 0; i < pMJHand->CurrentLength(); ++i )
	//	{
	//		if ( ( ucLastTile - 1 ) == pMJHand->GetTile( i ) 
	//			||
	//			( ucLastTile - 2 ) == pMJHand->GetTile( i )  )
	//		{
	//			SetTileSelect( i, MJ_TILES_SELECT_CAN );
	//		}
	//	}
	//	
	//	nSelNbr = nSelNbr % COLLECT_RES_OOX;
	//}

    // ȫ������ѡ��
    SetAllCanSelect(FALSE);

    RefreshMJCtrl(TRUE);
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnSetSelectStateQuadruplet()
{
	//int i, j;

	//// ѡ��
	//CMJHand* pMJHand = &m_TilesHand[GetLocalChair()];
	//CMJSet* pMJSet = &m_TilesSet[GetLocalChair()];

	//// ȫ������ѡ��
	//SetAllCanSelect(FALSE);

	//for (i = 0; i < pMJSet->CurrentLength(); i++)
	//{
	//	TILE_SET tagSet;
	//	pMJSet->GetSet(i, tagSet);

	//	if (ACTION_TRIPLET == tagSet.m_ucFlag)
	//	{
	//		// ������������һ��
	//		for (j = 0; j < pMJHand->CurrentLength(); j++)
	//		{
	//			if (tagSet.m_ucTile == pMJHand->GetTile(j))
	//			{
	//				SetTileSelect(j, MJ_TILES_SELECT_CAN);
	//			}
	//		}
	//	}
	//}

	//for (i = 0; i < pMJHand->CurrentLength(); i++)
	//{
	//	if (pMJHand->IsHave( pMJHand->GetTile( i ), 4))
	//	{
	//		SetTileSelect(i, MJ_TILES_SELECT_CAN);
	//	}
	//}

 //   if (GetMJSetting().nDongNanXiBeiGang == 1)
 //   {
 //       // �����: ����������
 //       if (pMJHand->IsHave(TILE_EAST) && pMJHand->IsHave(TILE_SOUTH) && pMJHand->IsHave(TILE_WEST) && pMJHand->IsHave(TILE_NORTH))
 //       {
 //           // ���������ܳ���
 //           for (i = 0; i < pMJHand->CurrentLength(); i++)
 //           {
 //               TILE tile = pMJHand->GetTile(i);

 //               if (tile == TILE_EAST || tile == TILE_SOUTH || tile == TILE_WEST || tile == TILE_NORTH)
 //               {
 //                   SetTileSelect(i, MJ_TILES_SELECT_CAN);
 //               }
 //           }
 //       } 
 //   }

 //   if (GetMJSetting().nZhongFaBaiGang == 1)
 //   {
 //       // �����: �з���X��
 //       if (pMJHand->IsHave(TILE_ZHONG) && pMJHand->IsHave(TILE_FA) && pMJHand->IsHave(TILE_BAI)
 //       && (pMJHand->IsHave(TILE_ZHONG, 2) || pMJHand->IsHave(TILE_FA, 2) || pMJHand->IsHave(TILE_BAI, 2)))
 //       {
 //           // �з���X�ܳ���
 //           for (i = 0; i < pMJHand->CurrentLength(); i++)
 //           {
 //               TILE tile = pMJHand->GetTile(i);

 //               if (tile == TILE_ZHONG || tile == TILE_FA || tile == TILE_BAI)
 //               {
 //                   SetTileSelect(i, MJ_TILES_SELECT_CAN);
 //               }
 //           }
 //       }
 //   }

    // ȫ������ѡ��
    SetAllCanSelect(FALSE);

    RefreshMJCtrl(TRUE);
}

template<int nPlayerNbr>
BOOL CGameTilePlayerMgr<nPlayerNbr>::GetTingGiveWinTiles( TILE ucSelTile, CMJHand &mjTilesTing )
{
	int i;
	for (i = 0; i < m_TilesTingGive.CurrentLength(); ++i )
	{
		if ( ucSelTile == m_TilesTingGive.GetTile(i) )
		{
			mjTilesTing.ReleaseAll();
			mjTilesTing.AddTiles( m_TilesTingGiveWin[i] );
			return TRUE;
		}
	}

	// �������Ƽ��뵽�б�����
	int nIndex = m_TilesTingGive.CurrentLength();
	m_TilesTingGive.AddTile(ucSelTile);
	CMJHand mjTestHand;
	mjTestHand.ReleaseAll();
	mjTestHand.AddTiles(m_TilesHand[GetLocalChair()]);
	mjTestHand.DelTile(ucSelTile);
	m_TilesTingGiveWin[i].ReleaseAll();
	GetTingTiles( mjTestHand, m_TilesSet[GetLocalChair()], m_TilesTingGiveWin[nIndex] );
	mjTilesTing.AddTiles( m_TilesTingGiveWin[nIndex] );

	return FALSE;
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnSetSelectStateTing()
{
	int i;

	CMJHand mjGive;
	CMJHand* pMJHand = &m_TilesHand[GetLocalChair()];
	CMJSet* pMJSet = &m_TilesSet[GetLocalChair()];
	if (TRUE == IsTing( m_TilesHand[GetLocalChair()], m_TilesSet[GetLocalChair()], mjGive))
	{
		// ȫ������ѡ��
		SetAllCanSelect( FALSE );
		m_TilesTingGive.ReleaseAll();

		for (i = 0; i < pMJHand->CurrentLength(); ++i)
		{
			TILE ucTile = pMJHand->GetTile( i );
			if (mjGive.IsHave(ucTile))
			{
				SetTileSelect(i, MJ_TILES_SELECT_CAN);
			}
		}

        RefreshMJCtrl(TRUE);

		// ���������ʾ�ܺ����ƣ���Ҫ�����������
// 		if ( GetMJSetting().nShowTing )
// 		{
// 			// �������Ƽ��뵽�б�����
// 			m_TilesTingGive.AddTiles(mjGive);
// 			CMJHand mjTestHand;
// 			for ( i = 0; i < mjGive.CurrentLength(); ++i )
// 			{
// 				mjTestHand.ReleaseAll();
// 				mjTestHand.AddTiles(m_TilesHand[GetLocalChair()]);
// 				mjTestHand.DelTile(mjGive.GetTile(i));
// 				m_TilesTingGiveWin[i].ReleaseAll();
// 				GetTingTiles( mjTestHand, m_TilesSet[GetLocalChair()], m_TilesTingGiveWin[i] );
// 			}
// 		}
	}
	else
	{
		SetStateSelect( MJ_STATE_SELECT_GIVE );
	}
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnSetSelectStateGive()
{
	SetAllCanSelect();
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::SetStateSelect(int nState)
{
	if (nState < MJ_STATE_SELECT_NONE || nState > MJ_STATE_SELECT_TING)
    {
        nState = MJ_STATE_SELECT_NONE;
    }

	m_nStateSelect = nState;

	switch ( m_nStateSelect )
	{
	case MJ_STATE_SELECT_COLLECT:
		{
			OnSetSelectStateColloect();
		}
		break;
	case MJ_STATE_SELECT_QUADRUPLET:
		{
			OnSetSelectStateQuadruplet();
		}
		break;
	case MJ_STATE_SELECT_TING:
		{
			OnSetSelectStateTing();
		}		
		break;
	case MJ_STATE_SELECT_GIVE:
		{
			OnSetSelectStateGive();
		}
		break;
	default:
		{
			SetAllCanSelect();
		}
		break;
	}
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::GetLocalMJHand( CMJHand& mjHand )
{
	int nChair = GetLocalChair();
	GetTilesHand( nChair, mjHand );
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::GetLocalMJSet( CMJSet& mjSet )
{
	int nChair = GetLocalChair();
	GetTilesSet( nChair, mjSet );
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::GetLocalMJGive( CMJGive& mjGive )
{
	int nChair = GetLocalChair();
	GetTilesGive( nChair, mjGive );	
}

template<int nPlayerNbr>
BOOL CGameTilePlayerMgr<nPlayerNbr>::SetCanBeSelect()
{
	BOOL bDown = FALSE;
	for( int i = 0; i < MAX_HAND_TILES; ++i )
	{
		if ( MJ_TILES_SELECT_CHOOSE == m_nTileSelect[i] )
		{
			m_nTileSelect[i] = MJ_TILES_SELECT_CAN;
			bDown = TRUE;
		}
	}

	return bDown;
}

template<int nPlayerNbr>
int  CGameTilePlayerMgr<nPlayerNbr>::GetLocalChair()
{
	IUserManager* pUserMgr = GetSoUserManager();
	if ( NULL == pUserMgr )
		return 0;

	IUser* pUser = pUserMgr->GetLocalUser();
	if ( NULL == pUser )
		return 0;

	
	return pUser->GetSeatID();
}

// �������ű�������
template<int nPlayerNbr>
int CGameTilePlayerMgr<nPlayerNbr>::GetCanBeSelect()
{
	for( int i = 0; i < MAX_HAND_TILES; ++i )
	{
		if ( MJ_TILES_SELECT_CHOOSE == m_nTileSelect[i] )
		{
			return i;
		}
	}

	return -1;
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::SetLocalGive( TILE ucTIle )
{
	m_ucLocalGive = ucTIle;
}

// �趨�ͻ�ȡ���Ƶ�״̬
template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::SetBlockEnable( int nIndex, BOOL bEnble )
{
	if ( nIndex < 0 || nIndex >= MJ_BLOCK_NBR )
		return;

	m_bBlockEnable[nIndex] = bEnble;
}

template<int nPlayerNbr>
BOOL CGameTilePlayerMgr<nPlayerNbr>::GetBlockEnable( int nIndex )
{
	if ( nIndex < 0 || nIndex >= MJ_BLOCK_NBR )
		return FALSE;

	return m_bBlockEnable[nIndex];
}

// TIMEOUT
template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnTimeOutBlock( BOOL bAutoWin )
{
	if ( NULL == m_pTileEvent || NULL == m_pStateEvent )
		return;
	
    _DOUT("���ƶ�ʱ����ʱ, ȡ�����Ʋ���");
    OutputDebugString("dxout: ���ƶ�ʱ����ʱ, ȡ�����Ʋ���");

	// ������Ǻ��ƣ�ǿ��ȡ������
	// ֻ�����ƵĻ������ؽ��
	if ( m_nBlockLevel[GetLocalChair()] != GetMJSetting().nBlockLevelWin || FALSE == bAutoWin )
	{
		BOOL bOnlyTing = FALSE;
		if ( GetMJSetting().nBlockLevelTing )
		{
			bOnlyTing = TRUE;
		}
		
		for ( int i = 0; i < MJ_BLOCK_NBR; ++i )
		{
			if ( TRUE == m_bBlockEnable[i] && i != MJ_BLOCK_TING )
			{
				bOnlyTing = FALSE;
			}
		}

        if (m_pTileEvent->GetTurn() != GetLocalChair())
        {
            // ȡ������
            RequestBlock(ACTION_EMPTY, GetLastTile());
            m_pStateEvent->SetMJState( MJ_STATE_WAIT );
        }
        else
        {
            // ����״̬
            m_pStateEvent->SetMJState( MJ_STATE_GIVE );
        }
		//// ����ڵ�����ѡ����״̬���������״̬
		//if ( MJ_STATE_SELECT_TING == GetStateSelect() )
		//{
		//	m_pStateEvent->SetMJState( MJ_STATE_GIVE );
		//}
		//else if ( bOnlyTing == TRUE && MJ_STATE_SELECT_NONE == GetStateSelect() )
		//{
		//	m_pStateEvent->SetMJState( MJ_STATE_GIVE );
		//}
		//else
		//{
		//	RequestBlock( ACTION_EMPTY, GetLastTile() );
		//}
	}
	else
	{
		// ����ڵ�����ѡ����״̬��ѡ�����״̬���������״̬
		if ( MJ_STATE_SELECT_TING == GetStateSelect() || MJ_STATE_SELECT_QUADRUPLET == GetStateSelect() )
		{
			m_pStateEvent->SetMJState( MJ_STATE_GIVE );
		}
		else if ( MJ_STATE_SELECT_COLLECT == GetStateSelect() )
		{
			m_pStateEvent->SetMJState( MJ_STATE_WAIT );
			RequestBlock( ACTION_EMPTY, GetLastTile() );
		}
		else 
		{
			m_pStateEvent->SetMJState( MJ_STATE_WAIT );
			//RequestBlock( ACTION_WIN, GetLastTile() );  // 2010-12-15 dxh ȡ�������ƺ�����

            RequestBlock(ACTION_EMPTY, GetLastTile());
		}
	}

    // ˢ�����ư�ť
    memset(m_nBlockLevel, 0, sizeof(m_nBlockLevel));
    memset(m_bBlockEnable, 0, sizeof(m_bBlockEnable));

    RefreshMJCtrl();
}

template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::OnTimeOutGive()
{
	// �Զ������һ��
	TILE ucTile = m_TilesHand[GetLocalChair()].GetTile( m_TilesHand[GetLocalChair()].CurrentLength() - 1 );

    _DOUT("���Ƴ�ʱ, �Զ��������һ����");

    char szText[250];
    sprintf(szText, "dxhout:  ���%d, ���Ƴ�ʱ, �Զ��������һ����=%d", GetLocalChair(), ucTile);
    OutputDebugString(szText);

	RequestGive( ucTile,  m_TilesHand[GetLocalChair()].CurrentLength() - 1);
}

// ��ȡ���ƻ���
template<int nPlayerNbr>
void CGameTilePlayerMgr<nPlayerNbr>::RefreshGangScore()
{
	int i,j,k;
	memset( m_nGangScore, 0, sizeof(m_nGangScore) );

	int nBaseGangScore = GetMJSetting().nGangScore;
	if ( nBaseGangScore <= 0 )
		return;

	for ( i = 0; i < nPlayerNbr; ++i )
	{
		for ( j = 0; j < m_TilesSet[i].CurrentLength(); ++j )
		{
			TILE_SET tagSet;
			m_TilesSet[i].GetSet( j, tagSet );
			if ( tagSet.m_ucFlag == ACTION_QUADRUPLET_CONCEALED )
			{
				for ( k = 0; k < nPlayerNbr; ++k )
				{
					if ( i != k )
					{
						m_nGangScore[k] -= nBaseGangScore * 2;
					}
					else 
					{
						m_nGangScore[k] += (nPlayerNbr-1)*nBaseGangScore * 2;
					}
				}
			}
			else if ( tagSet.m_ucFlag == ACTION_QUADRUPLET_REVEALED )
			{
				BOOL bMin = FALSE;
				if ( tagSet.m_ucChair == i )
				{
					bMin = TRUE;
				}

				if ( FALSE == bMin )
				{
					for ( k = 0; k < nPlayerNbr; ++k )
					{
						if ( i == k )
						{
							m_nGangScore[k] += nBaseGangScore * 2;
						}
						else if ( k == tagSet.m_ucChair )
						{
							m_nGangScore[k] -= nBaseGangScore * 2;
						}
					}
				}
				else
				{
					for ( k = 0; k < nPlayerNbr; ++k )
					{
						if ( i == k )
						{
							m_nGangScore[k] += nBaseGangScore * (nPlayerNbr-1);
						}
						else
						{
							m_nGangScore[k] -= nBaseGangScore;
						}
					}
				}
				
			}

		}
	}

}

template<int nPlayerNbr>
int CGameTilePlayerMgr<nPlayerNbr>::GetGangScore( BYTE ucChair )
{
	if ( ucChair >= nPlayerNbr )
		return 0;

	return m_nGangScore[ucChair];
}

#endif


}


