// SoCardLogic.h: interface for the CGameLogic class.
//
//////////////////////////////////////////////////////////////////////

#include "SoMJFrameLogic.h"
#ifndef _WINDOWS
#include <time.h> 
#include <stdio.h> 
#endif

//#include "BaseTileDealerMgr.inl"
//#include "MJStateMgr.inl"

#ifndef _WINDOWS
#define OutputDebugString(p) {;}
#define _snprintf  snprintf
#endif

void char2Wchar(char *pchar, wchar_t * wchar)
{
    int nwLen = MultiByteToWideChar(CP_ACP, 0, pchar, -1, NULL, 0); 
    MultiByteToWideChar(CP_ACP,0,pchar,-1,wchar,nwLen);
}

void wchar2char(wchar_t * wchar, char *pchar)
{
    int len = WideCharToMultiByte(CP_ACP, 0, wchar, (int)wcslen(wchar), NULL, 0,NULL,NULL);

    // ���ֽڱ���ת���ɶ��ֽڱ���   
    WideCharToMultiByte(CP_ACP, 0, wchar, (int)wcslen(wchar), pchar, len, NULL, NULL);
}

CSoMJFrameLogic::CSoMJFrameLogic()
{
    m_pSoFrameSetting = NULL;
    memset(m_nTimeOutCount, 0, sizeof(m_nTimeOutCount));
    memset(m_bDealFinish, 0, sizeof(m_bDealFinish));
}

CSoMJFrameLogic::~CSoMJFrameLogic()
{
    m_pSoFrameSetting = NULL;
}

/**
*  @brief ��ʼ��
*/	
STATUS CSoMJFrameLogic::Initialize(ISoGameFactory *pGameFactory, int nGameID, int nTableID)
{
    time_t t = time(NULL); 
    char tmp[64]; 
    strftime( tmp, sizeof(tmp), "%Y-%m-%d", localtime(&t) ); 

    TCHAR szText[250];
    wsprintf(szText, "%s SoMJGameLogic.log", tmp);
    CLog::Init(szText);

    CSoBaseLogic::Initialize(pGameFactory, nGameID, nTableID);

    if (m_pSoFrameSetting != NULL)
    {
        m_pSoFrameSetting->Initialize(this, nGameID);
    }

    if (m_pSoGameProcessor != NULL)
    {
        m_pSoGameProcessor->AddTimerCall(this);
    }

    return RE_SUCCEED;
}

/**
*  @brief ֪ͨ����ģ�����������Ϸ��baseinfo
*  @param shPlayerID ��ҵ�PlayerID
*  @return ����ֵ��������
*  @sa
*/
int  CSoMJFrameLogic::OnSendBaseInfo(UINT uPlayerID)
{
    return 0;
}

/**
*  @brief ֪ͨ����ģ����ҵĿͻ��˰汾��
*  @param shPlayerID ��ҵ�PlayerID
*  @return ����ֵ��������
*  @sa
*/
int  CSoMJFrameLogic::OnNotityClientVer(int nVer)
{
    return 0;
}


/**
*  @brief ���е���Ҷ��Ѿ�׼���õ��¼�
*  @return ����ֵ��������
*  @sa
*/
int CSoMJFrameLogic::OnAllPlayerReady()
{
    // ��Ϸ��ʼ
    if (m_pSoGameProcessor != NULL)
    {
        m_pSoGameProcessor->SetGameStart();
    }

    return 0;
}

/**
*  @brief ��Ϸ��ʼ�¼�
*  @return ����ֵ��������
*  @sa
*/
int CSoMJFrameLogic::OnGameStart()
{
    CMJSoStateMgr<PLAYER_NBR>::OnGameStart();

    // ��ʹ������
    m_TilesDealer.ReleaseAll();
    memset(m_nDice, 0, sizeof(m_nDice));

    m_nNowTurn = 0;

    ClearFlower();

    SetLastGiveChair( PLAYER_NBR );

    for (int i = 0; i < PLAYER_NBR; i++)
    {
        m_TilesHand[i].ReleaseAll();
        m_TilesGive[i].ReleaseAll();
        m_TilesSet[i].ReleaseAll();
    }

    // ������Ϸ��ʼ
    CMJSoStateMgr<PLAYER_NBR>::SetMJState(MJ_STATE_DEAL);

    return 0;
}

/**
*  @brief ������Ϸ����������
*  @param[in]  eGameEndType ����������
*  @sa
*/	
void CSoMJFrameLogic::SetGameLogicEnd(GAME_END_TYPE eGameEndType)
{
    if (m_pSoGameProcessor != NULL)
    {
        m_pSoGameProcessor->SetGameEnd(eGameEndType);
    } 
}

/**
*  @brief ���ܽ�ɢ��Ϸ��Э���˳��¼���֪ͨ��gamesdk�������֪֮ͨǰ�Լ�֮�󲢲���������Ϸ��������Ҫgamelogic������֮�����gamesdk����Ϸ�����ӿ�
*  @param nGameEndType GAME_END_TYPEö��ֵ
*  @return ����ֵ��������
*  @sa nGameEndType
*/
int CSoMJFrameLogic::OnGameEnd(int nGameEndType)
{
    return 0;
}

/**
*  @brief ������Ϸ����ǰ�������������¼�֮�󣬻��뿪������Ϸ״̬�͵���mainsvr����Ϸ����
*  @param nGameEndType GAME_END_TYPEö��ֵ
*  @return ����ֵ��������
*  @sa nGameEndType
*/
int CSoMJFrameLogic::OnBeforeGameEnd(int nGameEndType)
{
    return 0;
}

/**
*  @brief ������Ϸ״̬�͵���mainsvr����Ϸ����֮�󴥷�
*  @param nGameEndType GAME_END_TYPEö��ֵ
*  @return ����ֵ��������
*  @sa nGameEndType
*/
int CSoMJFrameLogic::OnPostGameEnd(int nGameEndType)
{
    _DOUT("��Ϸ����, ���� OnPostGameEnd ���� , ����Ϸ���ݽ��в��ֳ�ʹ��");

    if (enEndByNetAdmin == nGameEndType 
     || enEndByPlayerEscape == nGameEndType 
     || enEndByArrange == nGameEndType)
    {
        // ���ܽ�ɢ��Ϸ�����ǿ����Ϸ���û�Э���˳���Ϸ
        m_nDealer = 0;        // ����ׯ��

        ClearRunNbr();        // �����ׯ����

        // ���������Ӯ��־
        memset(m_bWiner, 0, sizeof(m_bWiner));
    }
    else
    {
        // Ϊ��һ�ֳ�ʼ��, Ȧ����ŷ�ļ���
        InitForNext();
    }

    // ������Ϸδ��ʼ״̬
    CMJSoStateMgr<PLAYER_NBR>::SetMJState(MJ_STATE_NOSTART);

    memset(m_nDice, 0, sizeof(m_nDice));
    memset(m_nTimeOutCount, 0, sizeof(m_nTimeOutCount));
    memset(m_bDealFinish, 0, sizeof(m_bDealFinish));

    ClearFlower();

    for (int i = 0; i < PLAYER_NBR; i++)
    {
        m_TilesHand[i].ReleaseAll();
        m_TilesGive[i].ReleaseAll();
        m_TilesSet[i].ReleaseAll();
    }

    m_TilesDealer.ReleaseAll();
    m_TilesGod.ReleaseAll();

    return 0;
}

/**
*  @brief �յ���ҷ���GameMsg�����Լ������¼�������GameMsg
*  @param cMsgID Msg��ID
*  @param nPlayerID ���ʹ�GameMsg�����PlayerId
*  @param nChair ���ʹ�GameMsg����ҵ�ChairId
*  @param pcGameData ��Ϣ��ṹ��ָ��
*  @param nDataLen ��Ϣ�ĳ���
*  @return ����ֵ��������
*  @sa
*/
int CSoMJFrameLogic::OnGameMessage(UINT nMsgAssistantID, UINT nPlayerID, UINT nChair, void * pcGameData, UINT nDataLen)
{
    if (CMJSoStateMgr<PLAYER_NBR>::OnGameMessage(nMsgAssistantID, nPlayerID, nChair, pcGameData, nDataLen))
    {
        return TRUE;
    }

    // �����Լ�������
    if (pcGameData != NULL && nDataLen >= sizeof(TCMD_HEADER))
    {
        TCMD_HEADER *pData = (TCMD_HEADER*)pcGameData;

        if (MJFRAME_MOUDLE_MSG == pData->cCmdID)
        {
            _DOUT3("----------����CSoMJFrameLogic::OnGameMessage(%d, %d), ���ݰ���С(%d)------------", pData->cCmdID, pData->cCmdPara, nDataLen);
            switch( pData->cCmdPara )
            {
            case CS_MJ_REQUEST_GMCMD:   // GM����
                {
                    ProcessRequestGMCmd(nPlayerID, nChair, pcGameData, nDataLen);
                    return TRUE;
                }
            default:
                break;
            }
        }
    }

    return FALSE;
}

/**
*  @brief �յ��Թ۷���GameMsg�����Լ������¼�����Թ�GameMsg
*  @param cMsgID Msg��ID
*  @param nPlayerID ���ʹ�GameMsg���Թ�PlayerId
*  @param nChair ���ʹ�GameMsg���Թ۵�ChairId
*  @param pcGameData ��Ϣ��ṹ��ָ��
*  @param nDataLen ��Ϣ�ĳ���
*  @return ����ֵ��������
*  @sa
*/
int  CSoMJFrameLogic::OnViewerMessage(UINT nMsgAssistantID, UINT nPlayerID, UINT nChair, void * pcGameData, UINT nDataLen)
{
    return 0;
}

/**
*  @brief ���¼�ÿ�봥��һ�Σ�����ֻ����Ϸ���й����д���
*  @return ����ֵ��������
*  @sa
*/
int  CSoMJFrameLogic::OnGameTimer()
{
    return 0;
}

//ISoUserManagerEvent
/**
*  @brief ��ҽ����¼�
*  @param[in]  uPlayerID ���ID
*/
int  CSoMJFrameLogic::OnGameUserEnter(UINT uPlayerID)
{
    /*for (int i = 0; i < PLAYER_NBR; i++)
    {
    m_TilesHand[i].ReleaseAll();

    m_TilesHand[i].AddTile(TILE_CHAR_1);
    m_TilesHand[i].AddTile(TILE_CHAR_1);
    m_TilesHand[i].AddTile(TILE_CHAR_1);

    m_TilesHand[i].AddTile(TILE_CHAR_2);
    m_TilesHand[i].AddTile(TILE_CHAR_2);
    m_TilesHand[i].AddTile(TILE_CHAR_2);

    m_TilesHand[i].AddTile(TILE_CHAR_3);
    m_TilesHand[i].AddTile(TILE_CHAR_3);
    m_TilesHand[i].AddTile(TILE_CHAR_3);

    m_TilesHand[i].AddTile(TILE_CHAR_4);
    m_TilesHand[i].AddTile(TILE_CHAR_4);
    m_TilesHand[i].AddTile(TILE_CHAR_4);

    m_TilesHand[i].AddTile(TILE_CHAR_5);
    }

    m_TilesHand[1].AddTile(TILE_CHAR_5);

    SetLastTile( TILE_CHAR_5 );
    SetLastGiveChair(1);
    SetTurn(1);
    m_bWiner[1] = TRUE;
    SendFinishMsg();*/

    return 0;
}

/**
*  @brief ����˳��¼�
*  @param[in]  uPlayerID ���ID
*/
int CSoMJFrameLogic::OnGameUserExit(UINT uPlayerID)
{
    //if (NULL == m_pSoUserMgr)
    //{
    //	return 0;
    //}

    //int nState = CMJSoStateMgr<PLAYER_NBR>::GetMJState();

    //// �ж��˳�������Ƿ�Ϊ��Ϸ���(���Թ�)����Ϸ��������
    //if ( m_pSoUserMgr->IsPlayer(uPlayerID) 
    //  && nState > MJ_STATE_NOSTART && nState < MJ_STATE_END )
    //{
    //	// ���ܴ���
    //	OnPlayerEscapeInGame(uPlayerID);

    //	// ����ǿ�˽���
    //	SetGameLogicEnd(enEndByPlayerEscape);
    //}

    return 0;
}

// ĳ�����׼����
int CSoMJFrameLogic::OnGameUserReady(UINT uPlayerID)
{
    return 0;
}

// ����ؽ�
int CSoMJFrameLogic::OnGameUserReplay(UINT uPlayerID)
{
    // ������Ϸ������Ϣ
    OnCollectBaseMsg(uPlayerID);

	ISoPlayer * pSoPlayer = GetSoUserManager()->GetPlayerByID(uPlayerID);
	if (pSoPlayer != NULL)
	{
		// ���½����, �����ǰ���й�״̬������Ҫȡ���й�
		if (pSoPlayer->GetTrusteeStatusVal())
		{
			GetSoUserManager()->SetPlayerTrusteeStatus(pSoPlayer->GetChairID(), FALSE);
		}
	}
	
    return 0;
}

/**
*  @brief ��Ҷ����¼�, ĳ����Ҷ��ߺ�SDK���ɷ��¼�
*  @param[in]  uPlayerID ���ID
*	�ж��Ƿ�Ϊ��Ҷ��ߣ���Ҷ��ߺ��Ƿ���Ҫ������״̬����Ϊ�й�״̬��
*   �����й�״̬�󣬶��ڵ�ǰ����Ƕ��ߵ���ң���ô��Ҫ��ʾ����һ�µ�ǰ״̬�ĳ�ʱ
*   (�����������ֵ���ǰ��ҳ���,��ǰ��Ҷ����ˣ���ô���������йܵ�ͬʱ����ǿ�Ƴ���)
*/
int CSoMJFrameLogic::OnGameUserOffLine(UINT uPlayerID)
{
    if (m_pSoUserMgr != NULL)
    {
		ISoUser* pUser = NULL;

		int nOfflineCount = 0;
		for (BYTE i = 0; i < PLAYER_NBR; i++)
		{
			ISoUser* pUser = m_pSoUserMgr->GetPlayerByChair(i);
			if (pUser != NULL)
			{
				if (pUser->IsOffline())
				{
					nOfflineCount++;
				}
			}
		}

		_DOUT1("��ǰ��������: %d", nOfflineCount);

		if (nOfflineCount == PLAYER_NBR)
		{
			// ��Ϸ��ȫ����
			_DOUT("����Ҷ�ȫ�����ߣ���Ϸ��ȫ����");
			m_ucEndType = MJ_END_SYSTEM;
			m_pStateEvent->SetMJState( MJ_STATE_END );
		}
		else
		{
			pUser = m_pSoUserMgr->GetUserByID(uPlayerID);
			if (pUser != NULL)
			{
				if (pUser->IsPlayer())
				{
					char chChair = (char)pUser->GetChairID();

					// �ж�����Ƿ��Ѿ��й�
					if (FALSE == m_pSoUserMgr->GetPlayerTrusteeStatus(chChair))
					{
						// �����й�, ������Ϊ�й�״̬
						m_pSoUserMgr->SetPlayerTrusteeStatus(chChair, TRUE);
					}

					// �����й�
					SoTrustOffline( chChair, pUser->GetUserID() );
				}
			}
		}
    }

    return 0;
}

/**
*  @brief ĳ�����(�������Թ�)�޸����Ƿ������Թ����ú��ɷ����¼�
*  @param[in]  nPlayerID ���ID
*  @param[in]  bLookOn �Ƿ������Թ�
*  �������Թ�ʱ����������ϵ�����Ϊ�Ʊ����͸���λ�õ��Թ����
*  �����Թ�ʱ��������������е��Ʒ��͸���λ�õ��Թ����
*/	
int CSoMJFrameLogic::OnChangeLookOnSetting(UINT nPlayerID, BOOL bLookOn)
{
    if (m_pSoUserMgr != NULL)
    {
        ISoPlayer* pSoUser = m_pSoUserMgr->GetPlayerByID(nPlayerID);

        if (pSoUser != NULL)
        {
            int i;
            int nChair = pSoUser->GetChairID();
            TILE_SET tileSet;

            TCMD_NOTIFY_CHANGE_LOOKON msg;
            msg.ucLookOn = bLookOn;
            msg.ucHandTilesLength = m_TilesHand[nChair].CurrentLength();

            // �������
            for (i = 0; i < msg.ucHandTilesLength; i++)
            {
                msg.ucHandTiles[i] = m_TilesHand[nChair].GetTile(i);
            }

            // ���(��)����
            for (i = 0; i < m_TilesSet[nChair].CurrentLength(); i++)
            {
                m_TilesSet[nChair].GetSet(i, tileSet);

                if (ACTION_QUADRUPLET_CONCEALED == tileSet.m_ucFlag)
                {
                    msg.ucQuadrupletConcealedNbr++;
                    msg.ucQuadrupletConcealedTiles[i] = tileSet.m_ucTile[0];
                }
            }

            if (m_pSoGameProcessor != NULL)
            {
                // ֪ͨ���λ�õ������Թ����
                m_pSoGameProcessor->SendGameDataToLookOnUsers(nChair, (char*)&msg, sizeof(msg));
            }
        }
    }

    return 0;
}

/**
* @brief ĳ����һ������¼�
*pbResult ��ʼ��ΪTRUE, ���ΪFALSE�� FRAME�ǲ����û����иı���λ��
*/
int CSoMJFrameLogic::OnBeforeGameUserChangeChair(UINT nPlayerID, BYTE bSrcChair, BYTE bDstChair, BOOL* pbResult)
{
    return 0;
}

/**
*  @brief ����������й�״̬���¼�
*  @param cChair ��ҵ�ChairId
*  @param bTrustee �Ƿ����й�
*  @sa 
*/
int  CSoMJFrameLogic::OnChangeTrustee(UINT cChair, BOOL bIsTrustee)
{
    return 0;
}

// ���뼶��ʱ��
int CSoMJFrameLogic::OnTimer(int nTimerID)
{
    CMJSoStateMgr<PLAYER_NBR>::OnMSTimer(nTimerID);

    return 0;
}

/**
*  @brief ��ʱʱ�䵽������ʱ��
*/	
void CSoMJFrameLogic::OnTimeOut(int nTimerID)
{
	_DOUT1("OnTimerOut(%d)", nTimerID);
    CMJSoStateMgr<PLAYER_NBR>::OnTimeOut(nTimerID);

    if (MJ_TIMER_GIVE == nTimerID)
    {
        m_nTimeOutCount[GetTurn()]++;

        if (m_nTimeOutCount[GetTurn()] >= MJ_TURST_TIMEOUT_MAX)
        {
            m_nTimeOutCount[GetTurn()] = 0;

            // ��ҳ�ʱ���γ��� MJ_TURST_TIMEOUT_MAX ��, Ĭ���й�
            if (NULL != m_pSoUserMgr)
            {
                m_pSoUserMgr->SetPlayerTrusteeStatus(GetTurn(), TRUE);
            }
        }
    }
}

/**
*  @brief ���������������¼�
*  @return ����ֵ��������
*  @sa
*/	
int CSoMJFrameLogic::OnLoadCfg()
{
    if (m_pSoFrameSetting != NULL)
    {
        m_pSoFrameSetting->OnLoadCfg();
    }

    return 0;
}

/**
*  @brief ˢ���������¼�
*  @return ����ֵ��������
*  @sa
*/
int CSoMJFrameLogic::OnReloadCfg()
{
    if (m_pSoFrameSetting != NULL)
    {
        m_pSoFrameSetting->OnReloadCfg();
    }

    return 0;
}

/**
*  @brief �û��������ӣ��������ӵȵ���, �ռ���Ϸ��������
*  @param[in]  uPlayerID ���ID
*  ���緿���������
*/
int CSoMJFrameLogic::OnCollectBaseMsg(UINT uPlayerID)
{
    TCMD_NOTIFY_MJ_BASE_INFO cmd;

    // �ռ���Ϸ�����������
    GetBaseInfoMsg( uPlayerID, &cmd );

    ISoUser* pSoUser = m_pSoUserMgr->GetUserByID(uPlayerID);

    // У���û��Ƿ����
    if (NULL == pSoUser)
    {
        return 0;
    }

    if (pSoUser->IsPlayer())
    {
        // ���͸���Ϸ���
        m_pSoGameProcessor->SendGameDataTo(uPlayerID, (char*)&cmd, sizeof(cmd));
    }
    else
    {
        // ���͸��Թ����
        m_pSoGameProcessor->SendGameDataToLookOnUsers(pSoUser->GetChairID(), (char*)&cmd, sizeof(cmd));
    }

    return 0;
}

/**
*  @brief �ռ��ԹۺͶ������������
*  @param[in]  shPlayerID ���ID
*/
int CSoMJFrameLogic::OnCollectMsg(UINT uPlayerID)
{
    TCMD_NOTIFY_VIEW_INFO cmd;

    // �ռ����ü���Ϸ��Ϣ��ʹ�ã����������Ϣ����Ȼ����ͨ���˺��������
    GetViewInfoMsg( uPlayerID, &cmd );

    ISoUser* pSoUser = m_pSoUserMgr->GetUserByID(uPlayerID);

    // У���û��Ƿ����
    if (NULL == pSoUser)
    {
        return 0;
    }

    if (pSoUser->IsPlayer())
    {
        // ���͸���Ϸ���
        m_pSoGameProcessor->SendGameDataTo(uPlayerID, (char*)&cmd, sizeof(cmd));
    }
    else
    {
        // ���͸��Թ����
        m_pSoGameProcessor->SendGameDataToLookOnUsers(pSoUser->GetChairID(), (char*)&cmd, sizeof(cmd));
    }

    // ������������������Լ�����Щ�����ĺ���
    SendOtherGameInfo( uPlayerID );

    return 0;
}

/**
* @brief ��ܺ���----����Ϸ��������ܴ���
* @param [in] shPlaerID ���ID
*/
void CSoMJFrameLogic::OnPlayerEscapeInGame(UINT shPlayerID)
{
    ISoGameProcessor* pProcessor = GetSoGameProcess();
    if ( NULL == pProcessor )
    {
        return;
    }

    ISoUserManager *pUserMgr = GetSoUserManager();

    if ( NULL == pUserMgr )
    {
        return;
    }

    ISoUser* pUser = pUserMgr->GetUserByID( shPlayerID );
    if ( NULL == pUser )
    {
        return;
    }

    TCMD_NOTIFY_FINISH cmd;

    m_ucEndType = MJ_END_ESCAPE;

    // ȷ�����
    cmd.byFlag = m_ucEndType;

    // ����������Ҫ�۵��ķ�
    int nEscapeChair = pUser->GetChairID();
    int nEsacpeScore = GetEscapeScore( nEscapeChair );

    cmd.nScore[nEscapeChair] = -nEsacpeScore;

    int nShareScore = 0;
    int nShareMoney = 0;

    // ��������˷�����֣���ô��Ҫ�������÷���
    if ( GetMJSetting().nEscapeShareScore > 0  )
    {
        nShareScore = SetEscapeShareScore( &cmd, nEsacpeScore, nEscapeChair );
        nShareMoney = SetEscapeShareMoney( &cmd, nEsacpeScore, nEscapeChair );
    }

    // ��ȡ���ܿ۷ֽ��
    int nShareScoreRes = SetScoreByFinishMsg( &cmd );

    char strEscapeMsg[128] = {0};
    char strName[MAX_NAME_LEN] = {0};
    memset( strEscapeMsg, 0, sizeof(strEscapeMsg) );
    pUser->GetNickName( strName, MAX_NAME_LEN );

    int i;

    // ������ʾ��������
    if ( nShareMoney > 0 )
    {
        TCMD_MJ_NOTIFY_SHOWDLG cmdMsg;
        memset( cmdMsg.szMsg, 0, sizeof(cmdMsg.szMsg) );

        char strMoney[16] = {0};
        if ( GetMJSetting().nMoneyType == MJ_MONEY_TYPE_GAMEMONEY )
        {
            _snprintf( strMoney, 15, "��Ϸ��" );
        }
        else if ( GetMJSetting().nMoneyType == MJ_MONEY_TYPE_HAPPYMONEY )
        {
            _snprintf( strMoney, 15, "���ֶ�" );
        }

        // ����ɹ���
        if ( nShareScoreRes == 1 )
        {
            _snprintf( cmdMsg.szMsg, sizeof(cmdMsg.szMsg)-1, "��� %s ���ܣ����֣�%d��%s��%d�����%d%s��", 
                strName, cmd.nScore[nEscapeChair], strMoney, cmd.nMoney[nEscapeChair], nShareMoney, strMoney);
            pProcessor->SendGameData( (char*)&cmdMsg, sizeof(cmdMsg) );
        }
        else
        {
            // ��Ҫ��һ����ÿһ����
            for ( i = 0; i < PLAYER_NBR; ++i )
            {
                ISoPlayer* pSeatPlayer = pUserMgr->GetPlayerByChair( i );
                if ( pSeatPlayer && i != nEscapeChair )
                {
                    if ( cmd.nScore[i] != 0 )
                    {
                        _snprintf( cmdMsg.szMsg, sizeof(cmdMsg.szMsg)-1, "��� %s ���ܣ����֣�%d��%s��%d�����%d%s��", 
                            strName, cmd.nScore[nEscapeChair], strMoney, cmd.nMoney[nEscapeChair], nShareMoney, strMoney);
                    }
                    else
                    {
                        _snprintf( cmdMsg.szMsg, sizeof(cmdMsg.szMsg)-1, "��� %s ���ܣ����֣�%d��%s��%d��", 
                            strName, cmd.nScore[nEscapeChair], strMoney, cmd.nMoney[nEscapeChair] );
                    }
                    pProcessor->SendGameDataTo( pSeatPlayer->GetUserID(), (char*)&cmdMsg, sizeof(cmdMsg) );
                }
            }
        }

        // ����ʾ
        if ( GetMJSetting().nTicket > 0 )
        {
            // �����Թ�Ҫ������
            for ( i = 0; i < PLAYER_NBR; ++i )
            {
                pProcessor->SendGameDataToLookOnUsers( i, (char*)&cmd, sizeof(cmd) );
            }
            memset( cmdMsg.szMsg, 0, sizeof(cmdMsg.szMsg) );
            if ( nShareScoreRes == 1 )
            {
                _snprintf( cmdMsg.szMsg, sizeof(cmdMsg.szMsg)-1, "��� %s ���ܣ����֣�%d��%s��%d�������%d%s��������Ϸ��������������%d%s��", 
                    strName, cmd.nScore[nEscapeChair], strMoney, cmd.nMoney[nEscapeChair], nShareMoney, strMoney, GetMJSetting().nTicket, strMoney );
                pProcessor->SendGameData( (char*)&cmdMsg, sizeof(cmdMsg) );
            }
            else
            {
                for ( i = 0; i < PLAYER_NBR; ++i )
                {
                    ISoPlayer* pSeatPlayer = pUserMgr->GetPlayerByChair( i );
                    if ( pSeatPlayer && i != nEscapeChair )
                    {
                        if( cmd.nScore[i] != 0 )
                        {
                            _snprintf( cmdMsg.szMsg, sizeof(cmdMsg.szMsg)-1, "��� %s ���ܣ����֣�%d��%s��%d�������%d%s��������Ϸ��������������%d%s��", 
                                strName, cmd.nScore[nEscapeChair], strMoney, cmd.nMoney[nEscapeChair], nShareMoney, strMoney, GetMJSetting().nTicket, strMoney );
                        }
                        else
                        {
                            _snprintf( cmdMsg.szMsg, sizeof(cmdMsg.szMsg)-1, "��� %s ���ܣ����֣�%d��%s��%d��������Ϸ��������������%d%s��", 
                                strName, cmd.nScore[nEscapeChair], strMoney, cmd.nMoney[nEscapeChair], GetMJSetting().nTicket, strMoney );
                        }
                        pProcessor->SendGameDataTo( pSeatPlayer->GetUserID(), (char*)&cmdMsg, sizeof(cmdMsg) );
                    }
                }
            }			
        }
    }
    else if ( nShareScoreRes == 1 )
    {
        _snprintf( strEscapeMsg, 127, "��� %s ���ܣ����֣�%d��������˻��֣�%d��", 
            strName, cmd.nScore[nEscapeChair], nShareScore );
        ShowDlgMsg( strEscapeMsg, 128 );
    }
    else
    {
        for ( i = 0; i < PLAYER_NBR; i++ )
        {
            ISoPlayer* pSeatPlayer = pUserMgr->GetPlayerByChair( i );
            if ( pSeatPlayer && i != nEscapeChair )
            {
                if( cmd.nScore[i] != 0 )
                {
                    _snprintf( strEscapeMsg, 127, "��� %s ���ܣ����֣�%d��������˻��֣�%d��", 
                        strName, cmd.nScore[nEscapeChair], nShareScore );
                }
                else
                {
                    _snprintf( strEscapeMsg, 127, "��� %s ���ܣ����֣�%d", strName, cmd.nScore[nEscapeChair] );		
                }

                ShowDlgMsg( strEscapeMsg, 128, 0, pSeatPlayer->GetUserID() );
            }
        }
    }
    return ;
}

/**
*  @brief ����
*  @param[in]  nTurn ������Ŀ��
*/	
void CSoMJFrameLogic::DrawTile( int nTurn )
{
    CMJSoStateMgr<PLAYER_NBR>::DrawTile(nTurn);
}

/**
*  @brief ���ݽ�����Ϣ������ҵ�����
*  @param[in]  cmd ��������Ϣ
*  @return  1��ʾ�ɹ���-1��ʾʧ�ܣ�0��ʾ���ܷ�ȡ����ʧ��
*  
*  ��ÿ����ҵ����Ƶ������Ϣ�ֱ��͸�ÿһ�����
*/	
int CSoMJFrameLogic::SetScoreByFinishMsg(TCMD_NOTIFY_FINISH *pCmd)
{
    if (NULL == pCmd)
    {
        return -1;
    }

    ISoGameProcessor* pProcessor = GetSoGameProcess();
    if (NULL == pProcessor)
    {
        return -1;
    }

    ISoUserManager *pUserMgr = GetSoUserManager();

    if (NULL == pUserMgr)
    {
        return -1;
    }

    pUserMgr->SetGameScore(m_nDealer, pCmd->byWhoWin, pCmd->nScore, pCmd->nMoney);

    return 1;
}

/*
* @brief ��ܺ���----���ܿ۷�
*
* ���ܿ�48���Ի����֣��൱�����ÿ��5��
*/
int  CSoMJFrameLogic::GetEscapeScore( BYTE ucChair )
{
    return GetMJSetting().nBaseScore * 48;
}

/**
* @brief  ��ܺ���----�������ܿ۷ֻ�ȡ�������
*/
int  CSoMJFrameLogic::SetEscapeShareScore( TCMD_NOTIFY_FINISH* pCmd, int nEscapeScore, BYTE ucEscapeChair )
{
    int nScore = abs(nEscapeScore)/4;

    for (int i = 0; i < 4; i++)
    {
        if (i != nEscapeScore)
        {
            pCmd->nScore[i] = nScore;
        }
    }

    return nScore;
}

/**
* @brief  ��ܺ���----�������ܿ۽�һ�ȡ������
*/
int  CSoMJFrameLogic::SetEscapeShareMoney( TCMD_NOTIFY_FINISH* pCmd, int nEscapeScore, BYTE ucEscapeChair )
{
    int nMoney = abs(nEscapeScore)/4;

    for (int i = 0; i < 4; i++)
    {
        if (i != ucEscapeChair)
        {
            pCmd->nMoney[i] = nMoney;
        }
    }

    return nMoney;
}

/**
* @brief  ��ȡ����������Ϣ
*/
void CSoMJFrameLogic::GetViewInfoMsg( UINT shPlayerID, TCMD_NOTIFY_VIEW_INFO* pCmd )
{
    if (NULL == m_pSoUserMgr || NULL == m_pSoGameProcessor || NULL == m_pStateEvent || NULL == pCmd)
    {
        return ;
    }

    ISoPlayer* pSoPlayer = m_pSoUserMgr->GetPlayerByID(shPlayerID);

    if (NULL == pSoPlayer)
    {
        return ;
    }

    int i, j, k;
    UINT ucChairID = pSoPlayer->GetChairID();

    pCmd->ucDealer = m_nDealer;
    pCmd->ucWindRound = m_nWindRound;
    pCmd->ucWindRoundNbr = m_nWindRoundNbr;
    pCmd->ucCurTurn = m_nNowTurn;
    pCmd->ucWallLength = m_TilesDealer.CurrentLength();
    pCmd->ucNowTime = GetRemainTime();
    pCmd->ucGameState = m_pStateEvent->GetMJState();
    pCmd->ucBLookOn = pSoPlayer->GetBCanBeLookedOn();
    pCmd->ucLastTile = m_ucLastTile;
    pCmd->ucLastGiveChair = m_byLastChair;
    pCmd->ucDice[0] = m_nDice[0];
    pCmd->ucDice[1] = m_nDice[1];
	pCmd->ucDarwTileLength = m_TilesDraw.CurrentLength();
	pCmd->ucGangTileLength = m_TilesGang.CurrentLength();

    for (i = 0; i < 4; i++)
    {
        ISoPlayer* pUser = m_pSoUserMgr->GetPlayerByChair(i);
        if (pUser == NULL)
        {
            continue ;
        }
       
        // ׼��״̬
        pCmd->ucReady[i] = pUser->IsReady() ? 1 : 0;

		// �й�״̬
		pCmd->ucTrusteeFlag[i] = pUser->GetTrusteeStatusVal() ? 1 : 0;
    }

    // ������ơ����ơ����������ݳ���
    for (i = 0; i < 4; i++)
    {
        pCmd->ucHandTilesLength[i] = m_TilesHand[i].CurrentLength();
        pCmd->ucGiveTilesLength[i] = m_TilesGive[i].CurrentLength();
        pCmd->ucSetTilesLength[i] = m_TilesSet[i].CurrentLength();
    }

    // ��ӻ�������
    for (i = 0; i < MJ_FLOWER_NBR; i++)
    {
        pCmd->ucFlower[i] = m_nFlower[i];
    }

    // �����������
    for (i = 0; i < pCmd->ucHandTilesLength[ucChairID]; i++)
    {
        if (pSoPlayer->GetChairID() == ucChairID
        && (pSoPlayer->IsPlayer() || pCmd->ucBLookOn))
        {
            // ��Ϸ��� �� �����Թ�״̬
            pCmd->ucHandTiles[i] = m_TilesHand[ucChairID].GetTile(i);
        }
        else
        {
            // ������� �� �Թ���Ҳ��ܿ���
            pCmd->ucHandTiles[i] = TILE_BEGIN;
        }
    }

    TILE_SET tileSet;

    // ����������������
    for (i = 0; i < 4; i++)
    {
        k = 0;

        // �������
        for (j = 0; j < pCmd->ucSetTilesLength[i]; j++)
        {
            m_TilesSet[i].GetSet(j, tileSet);

            // ������������
            for (k = 0; k < 4; k++)
            {
                if (ACTION_QUADRUPLET_CONCEALED == tileSet.m_ucFlag)
                {
                    // ����ǰ���, ����Ҫ�ж��Թ���ҽ���ʱ�Ƿ���Թۿ�
                    if (GetMJSetting().nAnGangShow == 1 || (i == ucChairID && (pSoPlayer->IsPlayer() || pCmd->ucBLookOn)))
                    {
                        // ���Թۿ�
                        pCmd->ucBlockTiles[i][j * 4 + k] = tileSet.m_ucTile[k];
                    }
                    else
                    {
                        // �����Կ���������
                        pCmd->ucBlockTiles[i][j * 4 + k] = TILE_BEGIN;
                    }
                }
                else
                {
                    pCmd->ucBlockTiles[i][j * 4 + k] = tileSet.m_ucTile[k];
                }
            }

            // ��������
            pCmd->ucBlockTilesFlag[i][j] = tileSet.m_ucFlag;

            // ����˭����, ����������һ��(��Գ�����Ч)
            pCmd->ucBlockTilesWhich[i][j] = tileSet.m_ucChair;
        }

        // ��ӳ���
        for (j = 0; j < pCmd->ucGiveTilesLength[i]; j++)
        {
            pCmd->ucGiveTiles[i][j] = m_TilesGive[i].GetTile(j);

            m_TilesGive[i].GetBlock(j, tileSet);
            if (tileSet.m_ucFlag == ACTION_TING)
            {
                pCmd->bGiveTingTiles[i][j] = TRUE;
                pCmd->bGiveBlockTiles[i][j] = FALSE;
            }
            else if (tileSet.m_ucFlag != ACTION_EMPTY)
            {
                pCmd->bGiveTingTiles[i][j] = FALSE;
                pCmd->bGiveBlockTiles[i][j] = TRUE;
            }
            else
            {
                pCmd->bGiveTingTiles[i][j] = FALSE;
                pCmd->bGiveBlockTiles[i][j] = FALSE;
            }
        }
    }

    // �������������
    pCmd->ucGodTilesLength = m_TilesGod.CurrentLength();

    for (i = 0; i < pCmd->ucGodTilesLength; i++)
    {
        pCmd->ucGodTiles[i] = m_TilesGod.GetTile(i);
    }

    for (i = 0; i < 4; i++)
    {
        // ��һ�������
        pCmd->ucFlowerNbr[i] = GetFlowerNbr(i);

        // �����������
        pCmd->ucTingType[i] = m_nTingType[i];
    }

    // ��ǰ���״̬������״̬, ����Ҫ�������ƺ���Ժ���������
    if (m_nTingType[ucChairID] != MJ_TING_NONE)
    {
        CMJHand tmpHand;
        CMJHand mjTestHand = m_TilesHand[ucChairID];

        if (mjTestHand.CurrentLength() % 2 == 0)
        {
            // ��Ҫɾ����ǰ������
            mjTestHand.DelTile(GetLastTile());  
        }

        GetTingTiles( mjTestHand, m_TilesSet[ucChairID], tmpHand );

        // ������ƺ���Ժ���Щ��
        for (i = 0; i < tmpHand.CurrentLength() && i < 20; i++)
        {
            pCmd->ucTingTiles[i] = tmpHand.GetTile(i);
        }
    }
}

/**
* @brief  ��ȡ����������Ϣ
*/
void CSoMJFrameLogic::GetBaseInfoMsg( UINT shPlayerID, TCMD_NOTIFY_MJ_BASE_INFO* pCmd )
{
    if (NULL == pCmd)
    {
        return ;
    }

    // ��������ļ�����
    MJ_SETTING_DATA tagSetting;	
    m_pSoFrameSetting->GetMJSettingData(tagSetting);

	pCmd->nOpenGM        = tagSetting.nOpenGM;

    pCmd->ucTimerGive    = tagSetting.nTimerGive;
    pCmd->ucTimerFlower  = tagSetting.nTimerFlower;
    pCmd->ucTimerBlock   = tagSetting.nTimerBlock;
    pCmd->ucTimerReady   = tagSetting.nTimerReady;

    pCmd->ucTimerCutCard = tagSetting.nTimerCutCard;
    pCmd->ucTimerDice    = tagSetting.nTimerDice;
    pCmd->ucTimerDeal    = tagSetting.nTimerDeal;
    pCmd->ucTimerJP      = tagSetting.nTimerJP;  

    pCmd->ucOpenLookTile  = tagSetting.nOpenLookTile;
    pCmd->ucTimerLookTile = tagSetting.nTimerLookTile;

    pCmd->ucGiveBlockShow = tagSetting.nGiveBlockShow;

	pCmd->ucTilesHandNbr = tagSetting.nTilesHandNbr;
	pCmd->ucTilesNumber  = tagSetting.nTilesNumber;

	pCmd->ucGangScore = tagSetting.nGangScore;   // ���Ƶĵ׷�
	pCmd->ucMinFan    = tagSetting.nMinFan;      // ��С��
	pCmd->ucBaseScore = tagSetting.nBaseScore;   // ��Ϸ�׷�
	pCmd->ucMoneyType = tagSetting.nMoneyType;   // �۷�����


    //pCmd->ucTicket = tagSetting.nTicket;
    //pCmd->nVoicePrice = tagSetting.nVoicePrice;
    //pCmd->nBuyItemMoney = tagSetting.nBuyItemMoney;
    

    //pCmd->ucGangScore = tagSetting.nGangScore;
    //pCmd->ucMinFan = tagSetting.nMinFan;
    //pCmd->ucBaseScore = tagSetting.nBaseScore;
    //pCmd->ucMoneyType = tagSetting.nMoneyType;

    //pCmd->ucTilesNumber = tagSetting.nTilesNumber;
    //pCmd->ucTilesHandNbr = tagSetting.nTilesHandNbr;

    //pCmd->ucBlockLevelCollect = tagSetting.nBlockLevelCollect;
    //pCmd->ucBlockLevelTriplet = tagSetting.nBlockLevelTriplet;
    //pCmd->ucBlockLevelQuadruplet = tagSetting.nBlockLevelQuadruplet;
    //pCmd->ucBlockLevelTing = tagSetting.nBlockLevelTing;
    //pCmd->ucBlockLevelWin = tagSetting.nBlockLevelWin;

    //pCmd->ucTimerGive = tagSetting.nTimerGive;
    //pCmd->ucTimerFlower = tagSetting.nTimerFlower;
    //pCmd->ucTimerBlock = tagSetting.nTimerBlock;
    //pCmd->ucTimerReady = tagSetting.nTimerReady;

    //pCmd->ucLouHu = tagSetting.nLouHu;
    //pCmd->ucCollateWind = tagSetting.nCollateWind;
    //pCmd->ucCollateArrow = tagSetting.nCollateArrow;
    //pCmd->ucAllCanCollate = tagSetting.nAllCanCollate;
    //pCmd->ucShowTing = tagSetting.nShowTing;
    //pCmd->ucTingCanGang = tagSetting.nTingCanGang;
    //pCmd->ucDongNanXiBeiGang = tagSetting.nDongNanXiBeiGang;
    //pCmd->ucZhongFaBaiGang = tagSetting.nZhongFaBaiGang;
    //pCmd->ucQiangGang = tagSetting.nQiangGang;
    //pCmd->ucMoreWin = tagSetting.nMoreWin;
    //pCmd->ucDrawGangBack = tagSetting.nDrawGangBack;
    //pCmd->ucCanWinLaizi = tagSetting.nCanWinLaizi;
    //pCmd->ucCanBlockLaizi = tagSetting.nCanBlockLaizi;

    //pCmd->ucCheck7dui = tagSetting.nCheck7dui;
    //pCmd->ucCheckQuanbukao = tagSetting.nCheckQuanbukao;
    //pCmd->ucCheck131 = tagSetting.nCheck131;
    //pCmd->ucCheckZuhelong = tagSetting.nCheckZuhelong;
    //pCmd->ucCheckLualv = tagSetting.nCheckLualv;
    //pCmd->ucCHeckLuafeng = tagSetting.nCHeckLuafeng;
}

/**
* @brief  ���Ͳ��ָ�����Ϸ��Ϣ
*/
void CSoMJFrameLogic::SendOtherGameInfo( UINT shPlayerID )
{

}

/**
* @brief  ���Ͳ��ָ�����Ϸ��Ϣ
*/
void CSoMJFrameLogic::SendItemLimitInfo( UINT shPlayerID )
{

}

ISoUserManager* CSoMJFrameLogic::GetSoUserManager()
{
    return GetSoUserManagerEx();
}

ISoGameProcessor* CSoMJFrameLogic::GetSoGameProcess()
{
    return GetSoGameProcessEx();
}

int Mystrnlen(char* str, UINT maxLen)
{
    UINT cuMaxLen = maxLen;
    char   *pCu = str;
    while(cuMaxLen > 0 )
    {
        if (!(*pCu))
        {
            return (maxLen - cuMaxLen);
        }
        pCu++;
        cuMaxLen--;
    }

    if (cuMaxLen == 0)
    {
        return maxLen;
    }
    else
    {
        return (pCu - str);
    }   

    return 0;
}

char* Mystrstr(char* str, char dst, int maxLen)
{
    UINT cuMaxLen = maxLen;
    char   *pCu = str;
    if (str==NULL)
    {
        return NULL;
    }
    
    while(cuMaxLen > 0 )
    {
        if ((*pCu) == dst)
        {
            while(((*pCu) == dst) && (cuMaxLen>0))
            {
                pCu++;
                cuMaxLen--;
            }

            if (cuMaxLen==0)
            {
                return NULL;
            }
            return (pCu);
        }
        pCu++;
        cuMaxLen--;
    }

    return NULL;
}

/*
    pStr                 :    һ��Ҫ����һ���Կս������ִ�
*/
char* MyStrFindChar(char* pStr, char tag)
{
    while(*pStr)
    {
        if ((*pStr) == tag)
        {
            return pStr;
        }
        pStr++;
    }
    return NULL;
}
/*
    pStr                   :       �������ڵ�Ҫ������ִ���ͷ,û����ֽ����ƣ���һ��Ҫ��һ��NULL�������ִ�
    pNextStr            :       �ڳɹ������˲�ѡȡ�����ڵ�ʵ����ָ����һ�ºϸ���ִ�
    pCuStoreStr       :       ָ�����ڴ���ѡȡ���ִ��Ŀռ�
    storeStrLen         :      �����ռ�ĳ���
    pTagStr             :       ָ�����ڷָ���ִ�, ֧���12������

    return              :       ����ɹ���ָ������ռ���ִ�����ʧ�ܷ���NULL
                                    ����ڲ�û�б��ָ�����ֱ�ӷ���ԭ��ֵ��pNextStr ����NULL
*/
//��û�в���ѽ��
/*
char* Mystrtok(char* pStr, char* pNextStr, char* pCuStoreStr, UINT storeStrLen, char* pTagStr)
{
    return NULL;
//    UINT cuMaxLen = maxLen;
//    char   *pCu = pStr;
//    UINT cuTagLen = 0;
//    UINT cuStrLen = 0; 
//    UINT  cuCountLen = storeStrLen;          //���ص��ִ����ǳ������ֳ�
//    UINT  tempLen = 0;
//    BYTE*  pReturnStr = NULL;
//    char*    pStart = NULL;
//    char*    pEnd = NULL;
//    
//    if (pStr == NULL || pNextStr==NULL 
//        || pCuStoreStr==NULL || pTagStr==NULL)
//    {
//        return NULL;
//    }
//
//    cuTagLen = Mystrnlen(pTagStr, 12);
//
//    if (cuStrLen ==0 || cuTagLen==0)
//    {
//        return NULL;
//     }
//    
//    *pNextStr =  NULL;
//
//    pReturnStr = pStr;
//
//    //�����һ���ִ��Ƿ��ҵ����������жϣ������һ��û����
//    //����ֱ�ӷ�?
//    
//    pReturnStr = MyStrFindChar(pTagStr, *pCu);
//    while (pReturnStr != NULL && (*pCu))
//    {
//        pCu++;
//        pReturnStr = MyStrFindChar(pTagStr, *pCu);
//    }
//
//    //�ҵ�һ��������ͷ��û����������ִ�
//    if (!(*pCu))
//    {
//        goto error;
//    }
//
//    //����һ���ֽ�
//    pStart = pCu; 
//        
//    tempLen = cuTagLen;
//    while((*pCu) &&  (cuTagLen>0))
//    {
//        if ((pEnd= MyStrFindChar(pTagStr, *pCu)) == NULL)
//         {
//                pCu++;
//                cuTagLen--;
//         }
//        else
//        {
//            if ((pEnd - pStart) >= storeStrLen)
//            {
//                goto error;
//            }
//
//            memcpy(pCuStoreStr, pStart, pEnd - pStart);
//            pCuStoreStr[pEnd - pStart] = 0;
//            pReturnStr =pCuStoreStr; 
//            break;
//            
//         }
//    }
//
//    return pReturnStr;
//
//error:
//    return NULL;
}

*/

// ����GM��Ϣ
int CSoMJFrameLogic::ProcessRequestGMCmd(UINT nPlayerID, int nChair, void * pcGameData, UINT nDataLen)
{
    /***************************************************************************************
    * GM���Ʋ�������ָ��:
    * ��ֵ: 1-9 ��(w), Ͳ(t), ��(t), ��(df), ��(nf), ��(xf), ��(bf), ��(hz), ��(fc), ��(bb), ��(ch), ��(xh), ��(qh), ��(dh), ÷(mh), ��(lh), ��(zh), ��(jh)
    * swap *,1w2w3w4w5w6w7w8w9w1s2s3s4s4s    ��������������
    * swap 1w2w3w,1s2s3s                     ���������е���
    * next 1w1t1s                            ����Ҫ������
    * set 1w2w3w                             �Լ�����Ҫ������
	* dice 1,6,5,7                           ������Ϸ�����������ӵ���, ÿ��ȡ2������
    ****************************************************************************************/

    if (0 == GetMJSetting().nOpenGM)
    {
        return 1;
    }

    ISoGameProcessor* pProcessor = GetSoGameProcess();
    if (NULL == pProcessor)
    {
        return 1;
    }

    ISoUserManager* pUserMgr = GetSoUserManager();
    if (NULL == pUserMgr)
    {
        return 1;
    }

    TCMD_REQUEST_GMCMD* pCmd = (TCMD_REQUEST_GMCMD*)pcGameData;

    if (NULL == pCmd)
    {
        return 1;
    }

    TCMD_NOTIFY_GMCMD cmd;
    cmd.ucChair = nChair;
    cmd.ucGMCmd = 0;

    char chGMCmd[500 + 1] = {0};
    wchar2char(pCmd->strGMCmd, chGMCmd);
    UINT tempLen = Mystrnlen(chGMCmd, sizeof(chGMCmd));
    if ((tempLen ==0) || (tempLen== sizeof(chGMCmd)))
    {
        return 1;
    }

    char *pData = NULL;
    pData = Mystrstr(chGMCmd, ' ', tempLen);
    if (pData == NULL)
    {
        return 1;
    }
    // ��ȡ�ؼ�������, �� swap 1w,3w�ַ����л�ȡ swap�ؼ���
    //pKey = strtok(chGMCmd, " ");
    // ��ȡ��ȥ�ؼ��ֵ��ַ���
    //pData = strtok(NULL, "");

	if (memcmp(GMCMD_SET_DICE, chGMCmd, sizeof(GMCMD_SWAP) - 1) == 0)
	{
		// �����������趨
		if (TransGMCmdSetDice(nChair, pData))
		{
			_snprintf(cmd.strGMResult, sizeof(cmd.strGMResult), "GMCMD: ���%dִ�и�����������ɹ�", nChair);
		}
		else
		{
			_snprintf(cmd.strGMResult, sizeof(cmd.strGMResult), "GMCMD: ���%dִ�и�����������ʧ��", nChair);
		}
	}
	else if (memcmp(GMCMD_SET_DEALER, chGMCmd, sizeof(GMCMD_SET_DEALER) - 1) == 0)
	{
		// ����ׯ���趨
		if (TransGMCmdSetDealer(nChair, pData))
		{
			_snprintf(cmd.strGMResult, sizeof(cmd.strGMResult), "GMCMD: ���%dִ�и���ׯ������ɹ�", nChair);
		}
		else
		{
			_snprintf(cmd.strGMResult, sizeof(cmd.strGMResult), "GMCMD: ���%dִ�и���ׯ������ʧ��", nChair);
		}
	}
    else if (memcmp(GMCMD_SWAP, chGMCmd, sizeof(GMCMD_SWAP) - 1) == 0)
    {
        // ���ƽ��л���
        if (TransGMCmdSwap(nChair, pData))
        {
            _snprintf(cmd.strGMResult, sizeof(cmd.strGMResult), "GMCMD: ���%dִ�л�������ɹ�", nChair);
        }
        else
        {
            _snprintf(cmd.strGMResult, sizeof(cmd.strGMResult), "GMCMD: ���%dִ�л�������ʧ��", nChair);
        }
    }
    else if (memcmp(GMCMD_NEXT, chGMCmd, sizeof(GMCMD_NEXT) - 1) == 0)
    {
        // ������һ��ץ������
        if (TransGMCmdNext(nChair, pData))
        {
            _snprintf(cmd.strGMResult, sizeof(cmd.strGMResult), "GMCMD: ���%d������һ���������ݳɹ�", nChair);
        }
        else
        {
            _snprintf(cmd.strGMResult, sizeof(cmd.strGMResult), "GMCMD: ���%d������һ����������ʧ��", nChair);
        }
    }
    else if (memcmp(GMCMD_SET, chGMCmd, sizeof(GMCMD_SET) - 1) == 0)
    {
        // �����Լ�������Ҫץ��������
        if (TransGMCmdSet(nChair, pData))
        {
            _snprintf(cmd.strGMResult, sizeof(cmd.strGMResult), "GMCMD: ���%d�����Լ�������Ҫץ�������ݳɹ�", nChair);
        }
        else
        {
            _snprintf(cmd.strGMResult, sizeof(cmd.strGMResult), "GMCMD: ���%d�����Լ�������Ҫץ��������ʧ��", nChair);
        }
    }

    _DOUT1("\n%s\n", cmd.strGMResult);

    if (m_pSoGameProcessor != NULL)
    { 
        m_pSoGameProcessor->SendGameDataTo(nPlayerID, (char*)&cmd, sizeof(cmd));
    }

    return 0;
}

BYTE CSoMJFrameLogic::TransChatToTile(char* pStr)
{
    // �жϵ�һλ�ַ�Ϊ��ֵ, �ڶ�λ�ַ�Ϊ��ɫ
    // �ж�: ��Χ(1-9����)
    if ((pStr[0] - '0') >= 1 && (pStr[0] - '0') <= 9)
    {
        // ��
        if (pStr[1] == 'w') 
        {
            return (pStr[0] - '0');
        }

        // ��
        if (pStr[1] == 's') 
        {
            return (pStr[0] - '0') + 10;
        }

        // Ͳ
        if (pStr[1] == 't') 
        {
            return (pStr[0] - '0') + 20;
        }

        // �жϻ���(8����: �����ﶫ÷�����)
        if (pStr[1] == 'h' && (pStr[0] - '0') <= 8) 
        {
            return (pStr[0] - '0') + 40;
        }

        return TILE_BEGIN;
    }

    // �ж϶�������
    if (pStr[1] == 'f')
    {
        if (pStr[0] == 'd')
        {
            return TILE_EAST;
        }

        if (pStr[0] == 'n')
        {
            return TILE_SOUTH;
        }

        if (pStr[0] == 'x')
        {
            return TILE_WEST;
        }

        if (pStr[0] == 'b')
        {
            return TILE_NORTH;
        }

        return TILE_BEGIN;
    }

    // �ж��з���
    if (pStr[0] == 'h' && pStr[1] == 'z')
    {
        return TILE_ZHONG;
    }

    if (pStr[0] == 'f' && pStr[1] == 'c')
    {
        return TILE_FA;
    }

    if (pStr[0] == 'b' && pStr[1] == 'b')
    {
        return TILE_BAI;
    }


    // �жϴ����ﶬ÷�����
    if (pStr[1] == 'h')
    {
        if (pStr[0] == 'c')
        {
            return TILE_FLOWER_CHUN; 
        }

        if (pStr[0] == 'x')
        {
            return TILE_FLOWER_XIA; 
        }

        if (pStr[0] == 'q')
        {
            return TILE_FLOWER_QIU; 
        }

        if (pStr[0] == 'd')
        {
            return TILE_FLOWER_DONG; 
        }

        if (pStr[0] == 'm')
        {
            return TILE_FLOWER_MEI; 
        }

        if (pStr[0] == 'l')
        {
            return TILE_FLOWER_LAN; 
        }

        if (pStr[0] == 'z')
        {
            return TILE_FLOWER_ZHU; 
        }

        if (pStr[0] == 'j')
        {
            return TILE_FLOWER_JU; 
        }
    }

    return TILE_BEGIN;
}

// ����������
BOOL CSoMJFrameLogic::TransGMCmdSetDice(int nChair, char* pStr)
{
	char * pData = NULL;
	CMJHand mjDice;

	// ��ȡ��Ҫ�������Ƽ�
	pData = strtok(pStr, ",");
	while (pData != NULL)
	{
		int nDataLength = Mystrnlen(pData, 128);
		if (nDataLength != 1)
		{
			return FALSE;
		}

		mjDice.AddTile(atoi(&pData[0]));

		pData = strtok(NULL, ",");
	}

	if (mjDice.CurrentLength() > 0 && mjDice.CurrentLength() % 2 == 0)
	{
		m_TielsDiceConfig.ReleaseAll();
		m_TielsDiceConfig.AddTiles(mjDice);
		return TRUE;
	}

	return FALSE;
}

// ����ׯ��
BOOL CSoMJFrameLogic::TransGMCmdSetDealer(int nChair, char* pStr)
{
	// �ж������Ƿ����Ϊׯ��λ��
	int nDataLength = Mystrnlen(pStr, 128);
	if (nDataLength != 1)
	{
		return FALSE;
	}

	int nStation = atoi(&pStr[0]);
	if (nStation>= 0 && nStation < PLAYER_NBR)
	{
		m_TilesDealerConfig.ReleaseAll();
		m_TilesDealerConfig.AddTile(nStation);
		return TRUE;
	}

	return FALSE;
}

// ���ƹ���
BOOL CSoMJFrameLogic::TransGMCmdSwap(int nChair, char* pStr)
{
    int i;
	char chStr[2], * pData1, *pData2;
    TILE  tile;

    // ��ȡ��Ҫ�������Ƽ�
    pData1 = strtok(pStr, ",");

    // ��ȡ��Ҫ�ı��Ŀ���Ƽ�
    pData2 = strtok(NULL, " ");

    if (pData1 == NULL || pData2 == NULL)
    {
        return FALSE;
    }

    if (strncmp("*", pData1, strlen("*")) == 0)
    {
        // * ��, ������������
        int nDataLength = Mystrnlen(pData2, 128);
        //int nDataLength = strlen(pData2);

        CMJHand mjHand;
        mjHand.ReleaseAll();
        for (i = 0; i < nDataLength; i += 2)
        {
            chStr[0] = pData2[i];
            chStr[1] = pData2[i+1];

            // ���ַ���ת����һ����
            tile = TransChatToTile(chStr);

            if (tile > TILE_BEGIN)
            {
                mjHand.AddTile(tile);
            }
        }

        // �жϻ��������ݳ���, �Ƿ���ڻ���������Ƴ���
        if (mjHand.CurrentLength() == m_TilesHand[nChair].CurrentLength())
        {
            // ����
            m_TilesHand[nChair] = mjHand;

            // ǿˢ���¿ͻ���������
            UpdateHandTiles(nChair);

            // �жϵ�ǰ�Ƿ�Ϊ�������
            if (GetTurn() == nChair)
            {
                if (m_TilesHand[nChair].CurrentLength() % 3 == 2)
                {
                    // ��⻻�ƺ�, �Ƿ��������
                    if (CheckBlock(nChair))
                    {
                        m_pStateEvent->SetMJState(MJ_STATE_BLOCK);

                        GetSoGameProcess()->SendGameDataToSeat((UINT)nChair, (char*)&GetBlockInfo(nChair), sizeof(TCMD_NOTIFY_BLOCK));
                    }
                }
            }

            return TRUE;
        }
    }
    else
    {
        // ���������Ѿ��е���
        int nDataLength1 = Mystrnlen(pData1, 128);
        int nDataLength2 = Mystrnlen(pData2, 128);

        // �ж�Ҫ�������Ƴ����Ƿ����
        if (nDataLength1 == nDataLength2)
        {
            CMJHand mjHand, tmpHand;
            mjHand.ReleaseAll();
            tmpHand.ReleaseAll();

            for (i = 0; i < nDataLength1; i += 2)
            {
                chStr[0] = pData1[i];
                chStr[1] = pData1[i+1];

                // ���ַ���ת����һ����
                tile = TransChatToTile(chStr);

                if (tile > TILE_BEGIN)
                {
                    mjHand.AddTile(tile);
                }
            }

            if (!m_TilesHand[nChair].IsSubSet(mjHand))
            {
                // Ҫ����������, �������в�����
                return FALSE;
            }

            for (i = 0; i < nDataLength2; i += 2)
            {
                chStr[0] = pData2[i];
                chStr[1] = pData2[i+1];

                // ���ַ���ת����һ����
                tile = TransChatToTile(chStr);

                if (tile > TILE_BEGIN)
                {
                    tmpHand.AddTile(tile);
                }
            }

            if (mjHand.CurrentLength() == tmpHand.CurrentLength())
            {
                // ���Ի���
                m_TilesHand[nChair].DelTiles(mjHand);
                m_TilesHand[nChair].AddTiles(tmpHand);

                // ǿˢ���¿ͻ���������
                UpdateHandTiles(nChair);

                // �жϵ�ǰ�Ƿ�Ϊ�������
                if (GetTurn() == nChair)
                {
                    if (m_TilesHand[nChair].CurrentLength() % 3 == 2)
                    {
                        // ��⻻�ƺ�, �Ƿ��������
                        if (CheckBlock(nChair))
                        {
                            m_pStateEvent->SetMJState(MJ_STATE_BLOCK);
                        }
                    }
                }

                return TRUE;
            }
        }
    }

    return FALSE;
}

// ���ý�����������
BOOL CSoMJFrameLogic::TransGMCmdNext(int nChair, char* pStr)
{
    // ��ȡ��Ҫ�������Ƽ�
    char * pData = strtok(pStr, " ");
    int nDataLength = Mystrnlen(pData, 128);

    char chStr[2];
    CMJHand mjHand;
    int i;
    TILE tile;
    for (i = 0; i < nDataLength; i += 2)
    {
        chStr[0] = pData[i];
        chStr[1] = pData[i+1];

        // ���ַ���ת����һ����
        tile = TransChatToTile(chStr);

        if (tile > TILE_BEGIN)
        {
            mjHand.AddTile(tile);
        }
    }

    if (mjHand.CurrentLength() > 0)
    {
		m_TilesNextConfig.ReleaseAll();
		m_TilesNextConfig.AddTiles(mjHand);

        return TRUE;
    }

    return FALSE;
}

// ����ĳ����ҽ�����Ҫ������
BOOL CSoMJFrameLogic::TransGMCmdSet(int nChair, char* pStr)
{
    // ��ȡ��Ҫ�������Ƽ�
    char * pData = strtok(pStr, " ");
    int nDataLength = Mystrnlen(pData, 128);

    char chStr[2];
    CMJHand mjHand;
    int i;
    TILE tile;
    for (i = 0; i < nDataLength; i += 2)
    {
        chStr[0] = pData[i];
        chStr[1] = pData[i+1];

        // ���ַ���ת����һ����
        tile = TransChatToTile(chStr);

        if (tile > TILE_BEGIN)
        {
            mjHand.AddTile(tile);
        }
    }

    if (mjHand.CurrentLength() > 0)
    {
        m_TilesSetConfig[nChair].ReleaseAll();
        m_TilesSetConfig[nChair].AddTiles(mjHand);

        return TRUE;
    }

    return FALSE;
}

BYTE CSoMJFrameLogic::TransChatToChair(int nChair, char str)
{
    return 0;
}