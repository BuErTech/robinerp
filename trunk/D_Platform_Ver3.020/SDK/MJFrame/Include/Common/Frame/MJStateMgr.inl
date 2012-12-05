// MJStateMgr.cpp: implementation of the CMJStateMgr class.
//
//////////////////////////////////////////////////////////////////////

// #include "MJStateMgr.h"

namespace MyGame
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
template<int nPlayerNbr>
CMJStateMgr<nPlayerNbr>::CMJStateMgr()
{
	SetStateEvent( (IMJStateEvent*)this );
}

template<int nPlayerNbr>
CMJStateMgr<nPlayerNbr>::~CMJStateMgr()
{
 
}

/************************************************************************/
/* 
����״̬�����޸�״̬�󣬻�ִ�����״̬�µĲ���
*/
/************************************************************************/
template<int nPlayerNbr>
void CMJStateMgr<nPlayerNbr>::SetMJState( int nState, BOOL bTimelag )
{
	if ( nState >= MJ_STATE_NOSTART && nState < MJ_STATE_NONE )
	{
		m_nGameState = nState;
	}
}

/*
SO
*/
#ifndef WINDOWS_LOGIC
template<int nPlayerNbr>
CMJSoStateMgr<nPlayerNbr>::CMJSoStateMgr()
{
	m_nGameState = MJ_STATE_NOSTART;
}

template<int nPlayerNbr>
CMJSoStateMgr<nPlayerNbr>::~CMJSoStateMgr()
{
 
}

template<int nPlayerNbr>
void CMJSoStateMgr<nPlayerNbr>::SetMJState( int nState, BOOL bTimelag )
{
	if ( nState < MJ_STATE_NOSTART || nState >= MJ_STATE_NONE )
		return;

	// �л�״̬Ҫ���¼�ʱ��
	KillTimer();

	int nOldState = m_nGameState;
	m_nGameState = nState;
	switch ( m_nGameState )
	{
	case MJ_STATE_DEAL:
		OnSetStateDeal( nOldState );
		break;
	case MJ_STATE_DRAW:
		OnSetStateDraw( nOldState );
		break;	
	case MJ_STATE_BLOCK:
		OnSetStateBlock( nOldState );
		break;
	case MJ_STATE_END:
		OnSetStateEnd( nOldState );
		break;
	case MJ_STATE_GIVE:
		OnSetStateGive( nOldState );
		break;
	case MJ_STATE_FLOWER:
		OnSetStateFlower( nOldState );
		break;
	case MJ_STATE_DRAW_WAIT:
		GetSoGameProcess()->SetTimer(MJ_TIMER_DRAW, 1000);  // ���ƶ�ʱ��
		break;
	default:
		break;
	}
}

template<int nPlayerNbr>
void CMJSoStateMgr<nPlayerNbr>::OnSetStateEnd( int nOldState )
{	
	_DOUT1("������Ϸ����״̬:%d\n", nOldState);

	// �㷬
	PayTicket();

	// ���ܽ���ר�Ŵ���
	if (m_ucEndType != MJ_END_ESCAPE)
    {
        // ���㴦��
        SendFinishMsg();
    }
	
	// ��Ϸ����
	if (m_ucEndType == MJ_END_SELF
     || m_ucEndType == MJ_END_GUN
     || m_ucEndType == MJ_END_NOTILE
     || m_ucEndType == MJ_END_QIANG)
	{		
		SetGameLogicEnd( enEndByGameOver );
	}
	else if ( m_ucEndType == MJ_END_REQUEST )
	{
		SetGameLogicEnd( enEndByArrange );
	}
	else if ( m_ucEndType == MJ_END_SYSTEM )
	{
		SetGameLogicEnd( enEndByNetAdmin );
	}
	else if ( m_ucEndType == MJ_END_ESCAPE )
	{
		// ������ܷŵ�����˳����ﴦ����Ȼȡ��������˵��׼��ˡ�
	}
	
	SetMJState( MJ_STATE_NOSTART );
}

template<int nPlayerNbr>
void CMJSoStateMgr<nPlayerNbr>::OnSetStateGive( int nOldState )
{
	SetTimer( MJ_TIMER_GIVE, GetMJSetting().nTimerGive + GetMJSetting().nTimerServer );

	// ���������ʱ�䵽�ˣ�ֱ�ӳ���
	//if ( IsOffline( GetTurn() ) )
	//{
	//	ISoUserManager* pUserMgr = GetSoUserManager();
	//	if (pUserMgr != NULL)
	//	{
	//		ISoPlayer* pUser = pUserMgr->GetPlayerByChair(GetTurn());

	//		if (pUser != NULL)
	//		{
	//			// �����Ҷ��ߣ��Ͱ�������
	//			if (IsOffline(GetTurn()))
	//			{
	//				SoTrustOfflineGive( GetTurn(), pUser->GetUserID() );
	//			}
	//		}
	//	}
	//}


    ISoUserManager* pUserMgr = GetSoUserManager();
    if (pUserMgr != NULL)
    {
        // �жϸ�����Ƿ�Ϊ�й�״̬
        if (!pUserMgr->GetPlayerTrusteeStatus(GetTurn()))
        {
            // �����й�״̬
            return ;
        }

        ISoPlayer* pUser = pUserMgr->GetPlayerByChair(GetTurn());

        if (pUser != NULL)
        {
            // �����Ҷ��ߣ��Ͱ�������
            if (pUser->IsOffline() || pUser->IsForceExit())
            {
                _DOUT1("���%d: �Զ�����\n", GetTurn());
                SoTrustOfflineGive(GetTurn(), pUser->GetUserID());
            }
        }
    }
}

template<int nPlayerNbr>
void CMJSoStateMgr<nPlayerNbr>::OnSetStateBlock( int nOldState )
{	
	_DOUT2("��������״̬=%d, �������ƶ�ʱ�� %d ��\n", nOldState, GetMJSetting().nTimerBlock + GetMJSetting().nTimerServer);

	SetTimer( MJ_TIMER_BLOCK, GetMJSetting().nTimerBlock + GetMJSetting().nTimerServer );

	memset( m_nBlockRequest, 0, sizeof(m_nBlockRequest) );
	memset( m_ucRequestTile, 0, sizeof(m_ucRequestTile) );
}

template<int nPlayerNbr>
int CMJSoStateMgr<nPlayerNbr>::OnGameMessage(char cMsgID, short nPlayerID, int nChair, void * pcGameData, short nDataLen)
{
	return CSoTileDealerMgr<nPlayerNbr>::OnGameMessage( cMsgID, nPlayerID, nChair, pcGameData, nDataLen );
}

template<int nPlayerNbr>
void CMJSoStateMgr<nPlayerNbr>::OnSetStateDraw( int nOldState )
{
	DrawTile( GetTurn() );
}

template<int nPlayerNbr>
void CMJSoStateMgr<nPlayerNbr>::OnSetStateDeal( int nOldState )
{
	InitDealer();
	SendTilesToPlayer();
	SetGodTiles();
	SendStartInfoToClient();
    m_nGameState = MJ_STATE_DEAL;

    // ���õȴ�������ɶ�ʱ��
    GetSoGameProcess()->SetTimer(MJ_TIMER_DEAL_FINISH, GetMJSetting().nTimerDeal * 1000);

    // ���벹��״̬
    // SetMJState( MJ_STATE_FLOWER );
}

template<int nPlayerNbr>
void CMJSoStateMgr<nPlayerNbr>::OnSetStateFlower( int nOldState )
{
	_DOUT1("���벹��״̬, ���ö�ʱ�� MJ_TIMER_FLOWER: %d��\n", GetMJSetting().nTimerFlower + GetMJSetting().nTimerServer);

	// ֻ�ܵ�������Ҳ���
	SetTimer( MJ_TIMER_FLOWER, GetMJSetting().nTimerFlower + GetMJSetting().nTimerServer );

	memset( m_bCheckFlower, 0, sizeof(m_bCheckFlower) );
}

// ���뼶��ʱ��
template<int nPlayerNbr>
void CMJSoStateMgr<nPlayerNbr>::OnMSTimer(int nTimerID)
{
    if (nTimerID == MJ_TIMER_DRAW)
    {
        // ����
        SetMJState(MJ_STATE_DRAW);
    }
    else
    {
        CSoTileDealerMgr<nPlayerNbr>::OnMSTimer(nTimerID);
    }
}

template<int nPlayerNbr>
void CMJSoStateMgr<nPlayerNbr>::OnTimeOut( int nTimerID )
{
	KillTimer();
	switch( nTimerID )
	{
	case MJ_TIMER_BLOCK:
		{
			OnTimeOutBlock();
		}
		break;
	case MJ_TIMER_GIVE:
		{
			OnTimeOutGive();
		}
		break;
	case MJ_TIMER_FLOWER:
		{
			OnTimeOutFlower();
		}
		break;
    // ����뼶��ʱ������ȷ
	//case MJ_TIMER_DRAW: 
	//	{
	//		// ����
	//		SetMJState(MJ_STATE_DRAW);
	//	}
	//	break;
	default:
		break;
	}
}

/************************************************************************/
/* C                                                                    */
/************************************************************************/
#else

template<int nPlayerNbr>
CMJGameStateMgr<nPlayerNbr>::CMJGameStateMgr()
{
}

template<int nPlayerNbr>
CMJGameStateMgr<nPlayerNbr>::~CMJGameStateMgr()
{
 
}

template<int nPlayerNbr>
void CMJGameStateMgr<nPlayerNbr>::SetMJState( int nState, BOOL bTimelag )
{
	if ( nState < MJ_STATE_NOSTART || nState >= MJ_STATE_NONE )
		return;

	int nOldState = m_nGameState;
	m_nGameState = nState;
	switch ( m_nGameState )
	{
	case MJ_STATE_GIVE:
        {
            if (bTimelag)
            {
                // ��ʱ���Ʋ���
                GetGameProcess()->KillTimer(MJ_GAME_BLOCKING_GIVE);
                GetGameProcess()->SetTimer(MJ_GAME_BLOCKING_GIVE, 1500);
            }
            else
            {
                OnSetStateGive( /*nOldState*/MJ_STATE_GIVE );
            }
        }
		break;
	case MJ_STATE_WAIT:
		OnSetStateWait( nOldState );
		break;	
	case MJ_STATE_BLOCK:
        {
            if (bTimelag)
            {
               // ��ʱ���Ʋ���
               GetGameProcess()->KillTimer(MJ_GAME_BLOCK_TIMELAG);
               GetGameProcess()->SetTimer(MJ_GAME_BLOCK_TIMELAG, 1500);
            }
            else
            {
                OnSetStateBlock( nOldState );
            }
        }
		break;
	case MJ_STATE_END:
		OnSetStateEnd( nOldState );
		break;
	case MJ_STATE_FLOWER:
		OnSetStateFlower( nOldState );
		break;
	default:
		break;
	}
}

template<int nPlayerNbr>
void CMJGameStateMgr<nPlayerNbr>::OnSetStateFlower( int nOldState )
{
	IGameProcess* pProcess = GetGameProcess();
	if ( NULL == pProcess )
	{
		return;
	}

	// C�˽��벹��״̬��ֱ��CHECK�˷������󲹻���Ϣ
	SetLogicTimer(MJ_TIMER_FLOWER, GetMJSetting().nTimerFlower );
	RequestFlower( GetLocalChair() );

// 	SetMJState( MJ_STATE_WAIT );
}

template<int nPlayerNbr>
void CMJGameStateMgr<nPlayerNbr>::OnSetStateBlock( int nOldState )
{	
	// ����������״̬
	// ��״̬��������������Ʋ���������û�к��ƣ�����Գ��ƣ�������ȻҪ�ȴ�
	CMJHand mjHand;
	GetLocalMJHand( mjHand );
	if ( GetTurn() == GetLocalChair() && m_bBlockEnable[MJ_BLOCK_WIN] == FALSE 
		&&
		mjHand.CurrentLength() % 3 == 2 ) 
	{
		SetStateSelect( MJ_STATE_SELECT_GIVE );
	}
	else
	{
		SetStateSelect( MJ_STATE_SELECT_NONE );
	}
	
    char szText[250];
    sprintf(szText, "dxhout: ���: %d, �������ƶ�ʱ��=%d, ����ʱ��%d��", GetLocalChair(), MJ_TIMER_BLOCK, GetMJSetting().nTimerBlock);
    OutputDebugString(szText);

    _DOUT2("�������ƶ�ʱ��ID=%d, ʱ��%d��", MJ_TIMER_BLOCK, GetMJSetting().nTimerBlock);

	//SetLogicTimer(MJ_TIMER_BLOCK, GetMJSetting().nTimerBlock );

    IUserManager *pUserManager = GetSoUserManager();
    if ( NULL == pUserManager )
    {
        return;
    }

    IUser* pUser = pUserManager->GetPlayerByChair(GetLocalChair());
    if ( NULL == pUser )
    {
        return;
    }

    if (TRUE == pUser->GetTrusteeStatus())
    {
        OnLogicTimeOut( MJ_TIMER_BLOCK );
    }
    else
    {
        if (GetTurn() == GetLocalChair())
        {
            // �Լ�ץ�ƺ�, ���ƶ�ʱ��
            SetLogicTimer(MJ_TIMER_BLOCK, GetMJSetting().nTimerBlock);
        }
        else
        {
            // ����ʱ, ����������Ƽ�ʱ, ��Ҫ�ӳ�һ��
            GetGameProcess()->KillTimer(MJ_GAME_BLOCK_TIMELAG);   // 2010-12-14 �Ѹ��ĵ��ɳ��ƻص�������ʾ���ư�ť
            GetGameProcess()->SetTimer(MJ_GAME_BLOCK_TIMELAG, 2000);/**/
        }
    }
}

template<int nPlayerNbr>
void CMJGameStateMgr<nPlayerNbr>::OnSetStateEnd( int nOldState )
{	
	KillLogicTImer(FALSE);
	StartClock( FALSE, FALSE );
	SetMJState( MJ_STATE_NOSTART );

	// ��Ϸ���������ٶ����ƽ��в���
	SetStateSelect( MJ_STATE_SELECT_NONE );

	IUserManager *pUserManager = GetSoUserManager();
	if ( NULL == pUserManager )
		return;

// 	IUser* pUser = pUserManager->GetPlayerByChair( GetLocalChair() );
// 	if ( NULL == pUser )
// 		return;
// 	
// 	// �����ã�����ʼ,�Զ���ʼ
// 	if ( TRUE == pUser->GetTrusteeStatus() )
// 	{
// 		pUserManager->SetGameReady();
// 	}

}

template<int nPlayerNbr>
void CMJGameStateMgr<nPlayerNbr>::OnSetStateGive( int nOldState )
{
    // ���õ�ǰ�������
    SetShowTurn(m_pTileEvent->GetTurn());

	char szText[250];
	sprintf(szText, "dxhout: ���: %d, ���ó��ƶ�ʱ��=%d, ����ʱ��%d��", GetLocalChair(), MJ_TIMER_GIVE, GetMJSetting().nTimerGive);
	OutputDebugString(szText);

    _DOUT2("���ó��ƶ�ʱ��=%d, ����ʱ��%d��",  MJ_TIMER_GIVE, GetMJSetting().nTimerGive);

	SetLogicTimer(MJ_TIMER_GIVE, GetMJSetting().nTimerGive);

	memset( m_bBlockEnable, 0, sizeof(m_bBlockEnable) );	
	SetStateSelect( MJ_STATE_SELECT_GIVE );
	IUserManager *pUserManager = GetSoUserManager();
	if ( NULL == pUserManager )
		return;

	IUser* pUser = pUserManager->GetPlayerByChair( GetLocalChair() );
	if ( NULL == pUser )
		return;

    if (GetTing(GetLocalChair()))
    {
        // dxh ��Ϊ�г��ƶ���, ֱ�ӳ����ٶ�̫��, ��Ҫ�ӳٳ���
        OutputDebugString("dxhout: �����Զ�����");

        GetGameProcess()->KillTimer(MJ_GAME_GIVE_TIMELAG);
        GetGameProcess()->SetTimer(MJ_GAME_GIVE_TIMELAG, 1000);
    }
    else if (pUser->GetTrusteeStatus())
	{
        OutputDebugString("dxhout: �й��Զ�����");
        // dxh ��Ϊ�г��ƶ���, ֱ�ӳ����ٶ�̫��, ��Ҫ�ӳٳ���
        GetGameProcess()->KillTimer(MJ_GAME_GIVE_TIMELAG);
        GetGameProcess()->SetTimer(MJ_GAME_GIVE_TIMELAG, 2000);
		// OnLogicTimeOut( m_nLogicTimerID );
	}
}

template<int nPlayerNbr>
void CMJGameStateMgr<nPlayerNbr>::OnSetStateWait( int nOldState )
{
    // dxh ע��, ֻ���Լ��ж�ʱ��, ����ط�û������
	/*IGameProcess *pProcess = GetGameProcess();
	if ( pProcess )
	{
		if ( gsPlaying == pProcess->GetGameState() )
        {
            SetLogicTimer(MJ_TIMER_WAIT, GetMJSetting().nTimerGive );
        }
		else
        {
            KillLogicTImer( FALSE );
        }
	}*/
	
    KillLogicTImer(FALSE);

	SetStateSelect( MJ_STATE_SELECT_NONE );
}

template<int nPlayerNbr>
void CMJGameStateMgr<nPlayerNbr>::OnGameMessage(UINT dwLen,  LPBYTE  pGameMsg)
{
	CGameTileDealerMgr<nPlayerNbr>::OnGameMessage( dwLen, pGameMsg );
}

template<int nPlayerNbr>
void CMJGameStateMgr<nPlayerNbr>::OnGameTimer( UINT dwGameTimerID )
{
	switch( dwGameTimerID )
	{
	case MJ_GAME_TIMER_LOGIC:
		{
			RefreshLogicTimer();
		}
        break;
    case MJ_GAME_GIVE_TIMELAG:    // ��ʱ�Զ����Ʋ���
        {
            GetGameProcess()->KillTimer(MJ_GAME_GIVE_TIMELAG);

            // ��Ϊ�г��ƶ���, ֱ�ӳ����ٶ�̫��, ��Ҫ�ӳٳ���
            OnLogicTimeOut(MJ_TIMER_GIVE);
        }
		break;
    case MJ_GAME_BLOCK_TIMELAG:   // ��ʱ���Ʋ���ʱ��
        {
            GetGameProcess()->KillTimer(MJ_GAME_BLOCK_TIMELAG);

            if (GetTurn() == GetLocalChair())
            {
                OnSetStateBlock(m_nGameState);

                // ˢ����ʾ�����������ؼ�
                RefreshMJCtrl();
            }
            else
            {
                // ���ƶ�ʱ���������������ư�ťͬʱ��ʾ
                if (GetMJState() == MJ_STATE_BLOCK)
                {
                    OutputDebugString("dxhout: ��ʱ�����������״̬");

                    // �ж��Լ��Ƿ�������
                    if (m_nBlockLevel[GetLocalChair()] > 0)
                    {
                        // ˢ����ʾ�����������ؼ�
                        RefreshMJCtrl();

                        SetLogicTimer(MJ_TIMER_BLOCK, GetMJSetting().nTimerBlock);
                    }
                }
            }
        }
        break;
    case MJ_GAME_BLOCKING_GIVE:  // ���Ʋ����󣬳�����ʱ
        {
            GetGameProcess()->KillTimer(MJ_GAME_BLOCKING_GIVE);

            // ���Ʋ���
            OnSetStateGive( m_nGameState );
        }
        break;
	default:
		break;
	}
}

template<int nPlayerNbr>
void CMJGameStateMgr<nPlayerNbr>::OnLogicTimeOut( int nLogicTimerID )
{
	KillLogicTImer();

    _DOUT2("���%d: ����ʱ��ID=%d: CMJGameStateMgr<nPlayerNbr>::���ó�ʱ�Զ�����", GetLocalChair(), nLogicTimerID);

	switch( nLogicTimerID )
	{
	case MJ_TIMER_BLOCK:
		OnTimeOutBlock();
		break;
	case MJ_TIMER_GIVE:
		OnTimeOutGive();
		break;
	case MJ_TIMER_FLOWER:
		OnTimeOutFlower();
		break;
	case MJ_TIMER_WAIT:
		OnTimeOutWait();
		break;
	case MJ_TIMER_READY:
		OnTimeOutReady();
		break;
	default:
		break;
	}
}

template<int nPlayerNbr>
void CMJGameStateMgr<nPlayerNbr>::OnTimeOutWait()
{
	// �����ȴ�
	SetLogicTimer(MJ_TIMER_WAIT, GetMJSetting().nTimerGive );
}

template<int nPlayerNbr>
void CMJGameStateMgr<nPlayerNbr>::OnTimeOutReady()
{
	// ����ʼ
	IUserManager *pUserManager = GetSoUserManager();
	if ( NULL == pUserManager )
		return;

	IUser* pUser = pUserManager->GetLocalUser();
	if ( pUser )
	{
		if ( sReady != pUser->GetState() )
		{
			pUserManager->SetGameReady();
		}
	}
}


#endif
}


