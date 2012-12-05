#include "Stdafx.h"
#include "Resource.h"
#include "GameMachine.h"
#include "MiniGameEngine.h"
#include "LoadSelection.h"

///���ƣ�CGameMachine
///���������캯��
///@param 
///@return 
CGameMachine::CGameMachine()
{
	//MiniGame����ָ��
	m_pGameEngine = NULL;

	//�Լ����߼�λ��
	m_byMeStation = 255;

	//���ƻ�ָ��
	m_pCardSender = NULL;

	//Ĭ��ʹ�ñ�׼����
	m_ltLangType = DZPKSound::LT_COMMON;

	//��Ϸ����������Ч����
	m_bBgMusic = true;

	//�Ƿ����й�״̬
	m_bTuoGuan = 0;

	//��Ϸ��Ч����
	m_bGameSound = true;

	//�õ����Ƶ����
	m_byTokenUser = 255;

	//�Ƿ���֪ͨ���������׼����Ϸ
	m_bAgreeGame = false;

	//ׯ��λ��
	m_byNTUser = 255;
	
	// ��ʼ���ۼӽ��
	m_nCurrentMoney = 0;
		
	// ��¼��ע���
	m_iBetCounts = 0;

}

///���ƣ�~CGameMachine
///��������������
CGameMachine::~CGameMachine()
{

}

//��������ʼ������
bool CGameMachine::StartUp(CMiniGameEngine* p,BYTE byGameStation,void* pGSData)
{
	m_pGameEngine = p;

	//��ʼ�������
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		m_UserData[i].Init();
	}
	
	// ������Ч����
	LoadSoundCfg();

	if (m_pGameEngine)
	{
		// ��ʼע����Ϣ
		m_tagDeskCfg.Clear();

		TGSBase* pGSBase = (TGSBase*)pGSData;
		m_pGameEngine->m_bShowPlayerID = pGSBase->tagDeskCfg.bShowUserInfo;

		//֪ͨ����л滭����
		m_pGameEngine->OnShowTableEnv();
	}

	m_pGameEngine->SetGameStatus(byGameStation);

	//����һЩ����״̬
	OnRecurBase(byGameStation,(TGSBase*)pGSData);

	//������Ϸ��״̬�ľ�������

	switch(byGameStation)
	{
	case GS_WAIT_SETGAME:
	case GS_WAIT_ARGEE:
	case GS_WAIT_NEXT_ROUND:
		{
			//����״̬���ȴ�ͬ��
			OutputDebugString("dxh SJClient��GS_WAIT_ARGEE");
			OnRecurWaitAgree(pGSData);
			m_pGameEngine->OnLoadSelectionDlg();
		}
		break;
	case GS_PLAY_GAME:
		{
			//����״̬����Ϸ���ڽ�����
            OutputDebugString("dxh SJClient��GS_PLAY_GAME");
			OnRecurPlayingGame(pGSData);
		}
		break;
	//case GS_WAIT_NEXT_ROUND:
	//	{
	//		//����״̬���ȴ���һ�غ�
 //           OutputDebugString("dxh SJClient��GS_WAIT_NEXT_ROUND");
	//		/*OnRecurWaitNextRound(pGSData);
	//		m_pGameEngine->OnLoadSelectionDlg();*/
	//	}
		break;
	}

	return true;
}

//�ر�
bool CGameMachine::ShutDown()
{
	//�ͷŷ��ƻ�
	if (m_pCardSender)
	{
		delete m_pCardSender;
		m_pCardSender = NULL;
	}
	return true;
}

// ��ʼ������
bool CGameMachine::Init()
{
	//�õ����Ƶ����
	m_byTokenUser = 255;

	//�Ƿ���֪ͨ���������׼����Ϸ
	m_bAgreeGame = false;

	//ׯ��λ��
	m_byNTUser = 255;

	// ��ʼ���ۼӽ��
	m_nCurrentMoney = 0;
	m_nCallMoney = 0; // ��ע���

	// ��¼��ע���
	m_iBetCounts = 0;
	
	// ��һ�β�������
	memset(&m_emLastOperation, ET_UNKNOWN,sizeof(m_emLastOperation));

	memset(m_nMoney, 0, sizeof(m_nMoney));
	memset(m_nBetMoney, 0, sizeof(m_nBetMoney));
	memset(m_bGiveUp, 0, sizeof(m_bGiveUp));
	memset(m_bTrusteeship, 0, sizeof(m_bTrusteeship));
	memset(m_bAllBet, 0, sizeof(m_bAllBet));

	// ���ջ
	ClearAllStack();
	return true;
}

//��Ϸ��Ч���ü���
bool CGameMachine::LoadSoundCfg()
{
	CString strFileName;
	strFileName.Format("%sbzGame.ini",CINIFile::GetAppPath());
	CINIFile f(strFileName);

	int nSelDiFang = f.GetKeyVal(_TEXT("SoundCfg"),_TEXT("DiFang_Select"),0);

	if (nSelDiFang == 0) //��׼����
	{
		m_ltLangType = DZPKSound::LT_COMMON;
	}
	else if (nSelDiFang == 1) //�ط�����
	{
		m_ltLangType = DZPKSound::LT_DIFANG01;
	}

	int nChkBgMusic	  = f.GetKeyVal(_TEXT("SoundCfg"),_TEXT("BK_Music"),1);
	int nChkGameSound = f.GetKeyVal(_TEXT("SoundCfg"),_TEXT("Game_Sound"),1);

	m_bBgMusic = (nChkBgMusic == 1);
	m_bGameSound = (nChkGameSound == 1);

	return true;
}

//����ID��ȡ��״̬��
void* CGameMachine::GetSubMachine(UINT uiMacID)
{
	if (uiMacID == IDSM_CARD_SENDER)
	{
		return m_pCardSender;
	}
	return NULL;
}

/*--------------------------------------------------------------------
     ����ˢ��״̬�����ݵĽӿڵ�ʵ��
  ------------------------------------------------------------------*/

//��ʼ��Ϸ�Ľӿ�
bool CGameMachine::OnHandleGameBegin(void* pData,UINT uiSize)
{
	if (!m_pGameEngine)
	{
		return false;
	}

	m_pGameEngine->SetGameStatus(GS_PLAY_GAME);

	Init();

	//֪ͨ������л滭����
	m_pGameEngine->ShowControl(IDB_BEGIN_GAME, SW_HIDE);
	m_pGameEngine->ShowControl(IDB_ALTER_STAKE, SW_HIDE);
	m_pGameEngine->ShowControl(IDC_READY_GO, SW_HIDE);
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		m_pGameEngine->ShowControl(IDC_READY + i, SW_HIDE);
	}
	
	
	if (m_pGameEngine->m_pLoadSelectionDlg != NULL)
	{
		m_pGameEngine->m_pLoadSelectionDlg->EndModalLoop(EVENT_CONTINUE_MAINFRAME);
	}

	//m_pGameEngine->OnShowTableEnv();

	return true;
}

//Ϊ����ṩѡ��ע��Ľӿ�
bool CGameMachine::OnHandleCfgDesk(void* pData,UINT uiSize)
{
	TCfgDesk* pCfgDesk = (TCfgDesk*)pData;

	if (!m_pGameEngine)
	{
		return false;
	}

	//������Ϸ��״̬
	m_pGameEngine->SetGameStatus(GS_CONFIG_NOTE);

	//֪ͨ������л滭����

	//if (pCfgDesk->dz.bCanSet)
	//{
	//	m_pGameEngine->OnShowCfgDesk(pCfgDesk);

	//	//15��ȴ�
	//	m_pGameEngine->ShowFlashTimer(m_byMeStation,m_tagDeskCfg.Time.,IDT_TIMEOUT_CFG_DESK);

	//}
	//else
	//{
	//	m_pGameEngine->OnShowWaitCfgDesk();
	//}

	return true;
}

//ϵͳ�ṩѡ��ע������Է�������ע����Ϣ�Ĵ����տ�
bool CGameMachine::OnHandleGetSysDeskCfg(void* pData,UINT uiSize)
{
	TDeskCfg* pDeskCfg = (TDeskCfg*)pData;

	if (!m_pGameEngine)
	{
		return false;
	}

	//��������
	::memcpy(&m_tagDeskCfg,pDeskCfg,sizeof(TDeskCfg));

	//֪ͨ������л滭����
	m_pGameEngine->OnShowSysDeskCfg(&m_tagDeskCfg);

	return true;
}

//����ṩѡ��ע������Է�������ע����Ϣ�Ĵ����տ�
bool CGameMachine::OnHandleGetDeskCfg(void* pData, UINT uiSize)
{

	TDeskCfg* pDeskCfg = (TDeskCfg*)pData;

	if (!m_pGameEngine)
	{
		return false;
	}

	//������Ϸ��״̬
	m_pGameEngine->SetGameStatus(GS_CONFIG_NOTE);

	//��������
	::memcpy(&m_tagDeskCfg,pDeskCfg,sizeof(TDeskCfg));

	//���õ�ע����Ҳ���Ҫ15�붨ʱ

	//֪ͨ������л滭����
	m_pGameEngine->OnShowDeskCfg(&m_tagDeskCfg);
	
	return true;
}

//����ṩѡ��ע������Է�������ע����Ϣ�Ĵ����տ�
bool CGameMachine::OnHandleInMoney(void* pData, UINT uiSize)
{
	TSubMoney* pSubMoney = (TSubMoney*)pData;

	if (!m_pGameEngine)
	{
		return false;
	}
	
	//֪ͨ������л滭����
	m_pGameEngine->OnShowSubMoney(pSubMoney);
	
	return true;
}

//��Ҿ�����ע���ӿ�
bool CGameMachine::OnHandleCfgDeskFinish(BYTE byIndex)
{
	if (!m_pGameEngine)
	{
		return false;
	}

	//֪ͨ������л滭����
	m_pGameEngine->StopAndHideFlashTimer(m_byMeStation);
	m_pGameEngine->OnShowClickedBottomCasting();

	//֪ͨͨѶ�๤��
	TCfgDesk tagCfgDesk;
	tagCfgDesk.dz.bySelect = byIndex;
	m_pGameEngine->SendGameData(&tagCfgDesk,sizeof(TCfgDesk),MDM_GM_GAME_NOTIFY,ASS_CFG_DESK,0);
	
	return true;
}

//�ȴ�ׯ��ѡ��ע��ĳ�ʱ����
bool CGameMachine::OnHandleWaitCfgDeskTimeout()
{
	if (!m_pGameEngine)
	{
		return false;
	}

	//֪ͨ������л滭����
	m_pGameEngine->ShowFlashTimer(m_byMeStation, 5, IDT_TIMEOUT_WAIT_CONFIG_NOTE);
	m_pGameEngine->OnShowWaitCfgDeskTimeout();
	//m_pMedia->m_bExitDesk = true;

	return true;
}

// ��Ҹ�ע�йܴ���ӿ�
bool CGameMachine::OnHandleCallMandate(bool bFlag)
{
	if (m_pGameEngine)
	{	 
		if (bFlag)
		{
			m_pGameEngine->ShowControl(IDB_CALLMANDATE, SW_HIDE);
			m_pGameEngine->ShowControl(IDB_CALLMANDATE1, SW_SHOW);

			// ���������й�ȡ����
			m_pGameEngine->ShowControl(IDB_CALLANYMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_CALLANYMANDATE1, SW_HIDE);

			m_pGameEngine->ShowControl(IDB_CHECKMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_CHECKMANDATE1, SW_HIDE);

			m_pGameEngine->ShowControl(IDB_PASSABANDONMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_PASSABANDONMANDATE1, SW_HIDE);
		}
		else
		{
			m_pGameEngine->ShowControl(IDB_CALLMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_CALLMANDATE1, SW_HIDE);
		}
	
		m_bTrusteeship[0] = bFlag;
		m_bTrusteeship[1] = false;
		m_bTrusteeship[2] = false;
		m_bTrusteeship[3] = false;
	}
	return true;
}

// ��Ҹ��κ�ע�йܴ���ӿ�
bool CGameMachine::OnHandleCallAnyMandate(bool bFlag)
{
	if (m_pGameEngine)
	{	 
		if (bFlag)
		{
			m_pGameEngine->ShowControl(IDB_CALLANYMANDATE, SW_HIDE);
			m_pGameEngine->ShowControl(IDB_CALLANYMANDATE1, SW_SHOW);

			// ���������й�ȡ����
			m_pGameEngine->ShowControl(IDB_CALLMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_CALLMANDATE1, SW_HIDE);

			m_pGameEngine->ShowControl(IDB_CHECKMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_CHECKMANDATE1, SW_HIDE);

			m_pGameEngine->ShowControl(IDB_PASSABANDONMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_PASSABANDONMANDATE1, SW_HIDE);
		}
		else
		{
			m_pGameEngine->ShowControl(IDB_CALLANYMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_CALLANYMANDATE1, SW_HIDE);
		}

		m_bTrusteeship[0] = false;
		m_bTrusteeship[1] = bFlag;
		m_bTrusteeship[2] = false;
		m_bTrusteeship[3] = false;
	}

	return true;
}


// ��ҹ����йܴ���ӿ�
bool CGameMachine::OnHandleCheckMandate(bool bFlag)
{
	if (m_pGameEngine)
	{	 
		// ֪ͨͨѶ�๤��
		//m_pGameEngine->StopAndHideFlashTimer(m_byMeStation);

		if (bFlag)
		{
			m_pGameEngine->ShowControl(IDB_CHECKMANDATE, SW_HIDE);
			m_pGameEngine->ShowControl(IDB_CHECKMANDATE1, SW_SHOW);

			// ���������й�ȡ����
			m_pGameEngine->ShowControl(IDB_CALLMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_CALLMANDATE1, SW_HIDE);

			m_pGameEngine->ShowControl(IDB_CALLANYMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_CALLANYMANDATE1, SW_HIDE);

			m_pGameEngine->ShowControl(IDB_PASSABANDONMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_PASSABANDONMANDATE1, SW_HIDE);
		}
		else
		{
			m_pGameEngine->ShowControl(IDB_CHECKMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_CHECKMANDATE1, SW_HIDE);
		}

		m_bTrusteeship[0] = false;
		m_bTrusteeship[1] = false;
		m_bTrusteeship[2] = bFlag;
		m_bTrusteeship[3] = false;
	}

	return true;
}

// ��ҹ��ƻ������йܴ���ӿ�
bool CGameMachine::OnHandlePassAbandonMandate(bool bFlag)
{
	if (m_pGameEngine)
	{	 
		if (bFlag)
		{

			m_pGameEngine->ShowControl(IDB_PASSABANDONMANDATE, SW_HIDE);
			m_pGameEngine->ShowControl(IDB_PASSABANDONMANDATE1, SW_SHOW);

			// ���������й�ȡ����
			m_pGameEngine->ShowControl(IDB_CALLMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_CALLMANDATE1, SW_HIDE);

			m_pGameEngine->ShowControl(IDB_CALLANYMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_CALLANYMANDATE1, SW_HIDE);

			m_pGameEngine->ShowControl(IDB_CHECKMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_CHECKMANDATE1, SW_HIDE);
		}
		else
		{
			m_pGameEngine->ShowControl(IDB_PASSABANDONMANDATE, SW_SHOW);
			m_pGameEngine->ShowControl(IDB_PASSABANDONMANDATE1, SW_HIDE);
		}

		m_bTrusteeship[0] = false;
		m_bTrusteeship[1] = false;
		m_bTrusteeship[2] = false;
		m_bTrusteeship[3] = bFlag;
	}
	return true;
}

//��ʼ��ť����ӿ�
bool CGameMachine::OnHandleBeginGame(BBaseParameter* pParam)
{
	//m_pGameEngine->EndModalLoop(EVENT_CONTINUE_MAINFRAME);
	return true;
}

//�޸Ĵ������ӿ�
bool CGameMachine::OnHandleModifyMoney()
{
	m_pGameEngine->OnLoadSelectionDlg();
	return true;
}

//�����ע����ť��ӿ�
bool CGameMachine::OnHandleBet()
{
	if (m_pGameEngine)
	{	 
		// ֪ͨͨѶ�๤��
		m_pGameEngine->StopAndHideFlashTimer(m_byMeStation);
		
		if (m_nCurrentMoney > 0 )
		{
			TBet tagBet;
			tagBet.nType = ET_BET;
			tagBet.nMoney = m_nCurrentMoney;
			m_pGameEngine->SendGameData(&tagBet, sizeof(tagBet), MDM_GM_GAME_NOTIFY, ASS_BET, 0);	
		}		
	}

	return true;
}

//��Ҹ�ע��ť����ӿ�
bool CGameMachine::OnHandleCall()
{
	if (m_pGameEngine)
	{
		//֪ͨ������л滭����
		m_pGameEngine->StopAndHideFlashTimer(m_byMeStation);

		if (m_byTokenUser == m_byMeStation)
		{
			TBet tagBet;
			tagBet.nMoney = m_nCallMoney;
			tagBet.nType = ET_CALL;
			m_pGameEngine->SendGameData(&tagBet,sizeof(tagBet),MDM_GM_GAME_NOTIFY,ASS_BET,0);	
		}
	}
		return true;
}

//��Ҽ�ע��ť����ӿ�
bool CGameMachine::OnHandleRaise()
{
	if (m_pGameEngine)
	{
		//֪ͨ������л滭����
		m_pGameEngine->StopAndHideFlashTimer(m_byMeStation);

		if (m_byTokenUser == m_byMeStation)
		{
			if (m_nCurrentMoney > 0 && m_nCurrentMoney <= m_pGameEngine->m_iMoneys[m_byMeStation])
			{
				//֪ͨͨѶ�๤��
				TBet tagBet;
				tagBet.nType = ET_ADDNOTE;
				tagBet.nMoney = m_nCurrentMoney + m_nCallMoney;  // �����ϼҽ����ͬ��������ټ�m_nCurrentMoney;
				m_pGameEngine->SendGameData(&tagBet,sizeof(tagBet),MDM_GM_GAME_NOTIFY,ASS_BET,0);
			}
		}
	}

	return true;
}

//��ҹ��ư�ť����ӿ�
bool CGameMachine::OnHandleCheck()
{
	if (m_pGameEngine)
	{
		//֪ͨ������л滭����
		m_pGameEngine->StopAndHideFlashTimer(m_byMeStation);

		//֪ͨͨѶ�๤��
		TBet tagVerb;
		tagVerb.nType = ET_CHECK;
		m_pGameEngine->SendGameData(&tagVerb,sizeof(tagVerb),MDM_GM_GAME_NOTIFY,ASS_BET,0);
	}
	return true;
}

//������ư�ť����ӿ�
bool CGameMachine::OnHandleFolo()
{
	if (m_pGameEngine)
	{
		//֪ͨ������л滭����
		m_pGameEngine->StopAndHideFlashTimer(m_byMeStation);
	
		//֪ͨͨѶ�๤��
		TBet tagVerb;
		tagVerb.nType = ET_FOLD;
		m_pGameEngine->SendGameData(&tagVerb,sizeof(tagVerb),MDM_GM_GAME_NOTIFY,ASS_BET,0);
	}
	return true;
}

//���ȫ�°�ť����ӿ�
bool CGameMachine::OnHandleAllin()
{
	if (m_pGameEngine)
	{
		//֪ͨ������л滭����
		m_pGameEngine->StopAndHideFlashTimer(m_byMeStation);

		//֪ͨͨѶ�๤��
		TBet tagVerb;
		tagVerb.nType = ET_ALLIN;
		m_pGameEngine->SendGameData(&tagVerb,sizeof(tagVerb),MDM_GM_GAME_NOTIFY,ASS_BET,0);
	}
	return true;
}

//��ҽ�ť����ӿ�
bool CGameMachine::OnHandleButtonMoney(int nID)
{
	//int nMoney = m_tagDeskCfg.dz.iLower * m_tagDeskCfg.dz.iRoomMultiple * m_tagDeskCfg.dz.iLowers[nID];   // ���㵱ǰ����İ�ť���
	
	m_nCurrentMoney = m_tagDeskCfg.dz.iLowers[nID];
	
	if (m_nCurrentMoney > m_pGameEngine->m_iMoneys[m_byMeStation])
	{
		// �����ע��Ҵ����������еĽ�ң�������н�Ҹ�ֵ����ע���
		m_nCurrentMoney = m_pGameEngine->m_iMoneys[m_byMeStation];
	}
	
	CString strMoney;
	BzButton *pButton = dynamic_cast<BzButton*>(m_pGameEngine->GetControlById(IDB_BET));
	if (pButton != NULL) 
	{
		strMoney.Format("       %d", m_nCurrentMoney);
		pButton->SetText(strMoney.GetBuffer());
		pButton->Invalidate();
	}

	pButton = dynamic_cast<BzButton*>(m_pGameEngine->GetControlById(IDB_RAISE));
	if (pButton != NULL) 
	{
		strMoney.Format("       %d", m_nCurrentMoney);
		pButton->SetText(strMoney.GetBuffer());
		pButton->Invalidate();
	}

	BzGameScrollbar * pScrollBar = dynamic_cast<BzGameScrollbar *>(m_pGameEngine->GetControlById(IDB_SLIDING_AXLE));
	if (pScrollBar != NULL)
	{
		int nNumber = m_pGameEngine->m_iMoneys[m_byMeStation]/100; // ���Լ�ӵ�еĽ�ҷֳɰٷ�

		int nMoney = m_nCurrentMoney;
		if (nMoney > nNumber)
		{
			// �������ע�Ľ��ռ�Լ�ӵ�еĽ�Ұٷ�֮��
			nMoney = nMoney / nNumber;
		}
		else if (nMoney < nNumber)
		{
			// �������ע�Ľ��ռ�Լ�ӵ�еĽ�Ұٷ�֮��
			nMoney = nNumber / nMoney;
		}

		CString str;
		str.Format("dxh:  ӵ�н��:%d, ��ǰ��ע:%d,, ռ�ٷ�֮: %f", m_pGameEngine->m_iMoneys[m_byMeStation], m_nCurrentMoney, float(nMoney)/100.00);
		OutputDebugString(str);

		// ���ðٷֱ�
		pScrollBar->SetCurrentProcess(float(nMoney)/100.00);
	}
	return true;
}

//��һ�����ע�ᴦ��ӿ�
bool CGameMachine::OnHandleSlidingAxle()
{
	if (m_pGameEngine)
	{
		//֪ͨ������л滭����
		BzGameScrollbar * pScrollBar = dynamic_cast<BzGameScrollbar *>(m_pGameEngine->GetControlById(IDB_SLIDING_AXLE));
		if (pScrollBar != NULL)
		{
			int nPercentMoney = m_pGameEngine->m_iMoneys[m_byMeStation];
			float number =  m_pGameEngine->m_iMoneys[m_byMeStation] * pScrollBar->GetCurrentProcess();
			//CString str; str.Format("dxh: ��ǰֵ:%f",number);OutputDebugString(str);

			m_nCurrentMoney = number;
			
			// �������ؼ��е㲻��׼, ���ܻᳬ��һ����, �����ڴ��жϽ��
			if (m_nCurrentMoney > nPercentMoney)
			{
				m_nCurrentMoney = nPercentMoney;
			}
			else if (m_nCurrentMoney < m_tagDeskCfg.dz.iMinusMoney)
			{
				m_nCurrentMoney = m_tagDeskCfg.dz.iMinusMoney;

				int nMoney = m_nCurrentMoney;
				if (nMoney > nPercentMoney)
				{
					// �������ע�Ľ��ռ�Լ�ӵ�еĽ�Ұٷ�֮��
					nMoney = nMoney / nPercentMoney / 100;
				}
				else if (nMoney < nPercentMoney)
				{
					// �������ע�Ľ��ռ�Լ�ӵ�еĽ�Ұٷ�֮��
					nMoney = nPercentMoney / nMoney / 100;
				}

				// ���ðٷֱ�
				pScrollBar->SetCurrentProcess(float(nMoney)/100.00);
			}

			CString strMoney;
			BzButton *pButton = dynamic_cast<BzButton*>(m_pGameEngine->GetControlById(IDB_BET));
			if (pButton != NULL) 
			{
				strMoney.Format("    %d", m_nCurrentMoney);
				pButton->SetText(strMoney.GetBuffer());
				pButton->Invalidate();
			}

			pButton = dynamic_cast<BzButton*>(m_pGameEngine->GetControlById(IDB_RAISE));
			if (pButton != NULL) 
			{
				strMoney.Format("       %d", m_nCurrentMoney);
				pButton->SetText(strMoney.GetBuffer());
				pButton->Invalidate();
			}
 		}
	}
	return true;
}

//��Ҽ�עť����ӿ�
bool CGameMachine::OnHandleAddMinus()
{
	// ֪ͨ������л滭����
	BzGameScrollbar * pScrollBar = dynamic_cast<BzGameScrollbar *>(m_pGameEngine->GetControlById(IDB_SLIDING_AXLE));
	if (pScrollBar != NULL)
	{
		// ÿ�����Ӱٷ�֮һ
		int nPercentMoney = m_pGameEngine->m_iMoneys[m_byMeStation]/100;

		// ÿ�μ� m_tagDeskCfg.dz.iMinusMoney
		if ((m_nCurrentMoney + m_tagDeskCfg.dz.iMinusMoney) > m_pGameEngine->m_iMoneys[m_byMeStation])
		{
			m_nCurrentMoney = m_pGameEngine->m_iMoneys[m_byMeStation];
		}
		else
		{
			m_nCurrentMoney += m_tagDeskCfg.dz.iMinusMoney;
		}

		int nMoney = m_nCurrentMoney;
		if (nMoney > nPercentMoney)
		{
			// �������ע�Ľ��ռ�Լ�ӵ�еĽ�Ұٷ�֮��
			nMoney = nMoney / nPercentMoney;
		}
		else if (nMoney < nPercentMoney)
		{
			// �������ע�Ľ��ռ�Լ�ӵ�еĽ�Ұٷ�֮��
			nMoney = nPercentMoney / nMoney;
		}

		// ���ðٷֱ�
		pScrollBar->SetCurrentProcess(float(nMoney)/100.00);

		CString strMoney;
		BzButton *pButton = dynamic_cast<BzButton*>(m_pGameEngine->GetControlById(IDB_BET));
		if (pButton != NULL) 
		{
			strMoney.Format("       %d", m_nCurrentMoney);
			pButton->SetText(strMoney.GetBuffer());
			pButton->Invalidate();
		}

		pButton = dynamic_cast<BzButton*>(m_pGameEngine->GetControlById(IDB_RAISE));
		if (pButton != NULL) 
		{
			strMoney.Format("       %d", m_nCurrentMoney);
			pButton->SetText(strMoney.GetBuffer());
			pButton->Invalidate();
		}
		return true;
	}
}

//��Ҽ�עť����ӿ�
bool CGameMachine::OnHandleSubMinus()
{
	// ֪ͨ������л滭����
	BzGameScrollbar * pScrollBar = dynamic_cast<BzGameScrollbar *>(m_pGameEngine->GetControlById(IDB_SLIDING_AXLE));
	if (pScrollBar != NULL)
	{
		// ÿ�����Ӱٷ�֮һ
		int nPercentMoney = m_pGameEngine->m_iMoneys[m_byMeStation]/100;

		if ((m_nCurrentMoney - m_tagDeskCfg.dz.iMinusMoney) < m_tagDeskCfg.dz.iMinusMoney)
		{
			m_nCurrentMoney = m_tagDeskCfg.dz.iMinusMoney;
		}
		else
		{
			m_nCurrentMoney -= m_tagDeskCfg.dz.iMinusMoney;
		}

		int nMoney = m_nCurrentMoney;
		if (nMoney > nPercentMoney)
		{
			// �������ע�Ľ��ռ�Լ�ӵ�еĽ�Ұٷ�֮��
			nMoney = nMoney / nPercentMoney;
		}
		else if (nMoney < nPercentMoney)
		{
			// �������ע�Ľ��ռ�Լ�ӵ�еĽ�Ұٷ�֮��
			nMoney = nPercentMoney / nMoney;
		}

		// ���ðٷֱ�
		pScrollBar->SetCurrentProcess(float(nMoney)/100.00);

		CString strMoney;
		BzButton *pButton = dynamic_cast<BzButton*>(m_pGameEngine->GetControlById(IDB_BET));
		if (pButton != NULL) 
		{
			strMoney.Format("       %d", m_nCurrentMoney);
			pButton->SetText(strMoney.GetBuffer());
			pButton->Invalidate();
		}

		pButton = dynamic_cast<BzButton*>(m_pGameEngine->GetControlById(IDB_RAISE));
		if (pButton != NULL) 
		{
			strMoney.Format("       %d", m_nCurrentMoney);
			pButton->SetText(strMoney.GetBuffer());
			pButton->Invalidate();
		}
	}

	return true;
}


//�������·�2���ƴ���ӿ�
bool CGameMachine::OnHandleSendACards(void* pData,UINT uiSize)
{
	if (!m_pGameEngine)
	{
		return false;
	}
	
	TCards* pCards = (TCards*)pData;

	//��������////////////

	//������������״̬��������״̬��
	m_pCardSender = new CCardSender();
	if (m_pCardSender)
	{
		m_pCardSender->StartUp(this,0,NULL);
	}

	//֪ͨ������л滭����
	m_pGameEngine->OnShowACard(pCards);

	return true;
}

//���������±߳��¼�����ӿ�
bool CGameMachine::OnHandleBetPoolUp(void* pData,UINT uiSize)
{
	if (!m_pGameEngine)
	{
		return false;
	}
	
	TBetPool* pBetPool = (TBetPool *)pData;

	memcpy(&m_tagBetPool, pBetPool, sizeof(m_tagBetPool));

	//֪ͨ������л滭����
	m_pGameEngine->OnShowPot(pBetPool);	

	return true;
}

//��������ע�����¼�����ӿ�
bool CGameMachine::OnHandleBetInfo(void* pData,UINT uiSize)
{
	//if (!m_pGameEngine)
	//{
	//	return false;
	//}

	////֪ͨ������л滭����
	//TBet * pBet = (TBet *)pData;
	//if (pBet->byUser == m_byMeStation)
	//{
	//	
	//}
	
	return true;
}

//�������·�3�Ź������ƴ���ӿ�
bool CGameMachine::OnHandleSend3Cards(void* pData,UINT uiSize)
{
	if (!m_pGameEngine)
	{
		return false;
	}
	
	TCards * pCards = (TCards *)pData;

	//֪ͨ������л滭����
	m_pGameEngine->OnShow3Card(pCards);
	

	return true;
}

//�������·�1�ŵ��ƴ���ӿ�
bool CGameMachine::OnHandleSend1Cards(void* pData,UINT uiSize)
{
	if (!m_pGameEngine)
	{
		return false;
	}

	//֪ͨ������л滭����

	TCards * pCards = (TCards *)pData;
	m_pGameEngine->OnShow1Card(pCards);

	return true;
}

//����������������ҷ�����ɵ���Ϣ����ӿ�
bool CGameMachine::OnHandleSendCardFinish()
{
	if (!m_pGameEngine)
	{
		return false;
	}

	//֪ͨ������л滭����
	//m_pGameEngine->SendGameData(MDM_GM_GAME_NOTIFY, ASS_SEND_CARD_FINISH, 0);
	return true;
}


//�������·����ƴ���ӿ�
bool CGameMachine::OnHandleGetToken(void* pData,UINT uiSize)
{
	TToken* pToken = (TToken*)pData;

	if (!m_pGameEngine)
	{
		return false;
	}

	//��������
	m_byTokenUser = pToken->byUser;

	CString str;
	str.Format("dxh: Client: ��ǰ�������: %d, �Լ�λ��: %d", m_byTokenUser, m_byMeStation);
	OutputDebugString(str);

	//֪ͨ������л滭����
	if (m_byMeStation == pToken->byUser)
	{
		m_pGameEngine->OnShowCall();

		// ��ǰ��ע��
		m_nCurrentMoney = 0;

		// ��հ�ť�ϵ�����
		BzButton *pButton = dynamic_cast<BzButton*>(m_pGameEngine->GetControlById(IDB_BET));
		if (pButton != NULL) 
		{
			pButton->SetText("       0");
		}

		pButton = dynamic_cast<BzButton*>(m_pGameEngine->GetControlById(IDB_RAISE));
		if (pButton != NULL) 
		{
			pButton->SetText("       0");
		}

		// �Լ��õ������
		m_pGameEngine->OnShowMeGetToken(pToken);
	}
	else
	{
		m_pGameEngine->OnShowOtherPlayerGetToken(pToken);

		if (m_bGiveUp[m_byMeStation] || m_bAllBet[m_byMeStation])
		{
			// ȫ�»�������ֻ��ʾ���ֽ̳�
			m_pGameEngine->OnAutoTrusteeship(SW_HIDE);
			m_pGameEngine->OnShowPdg(SW_SHOW);
			return true;
		}

		int nbyUser = GetNextUserStation(m_byTokenUser);
		int iUser = GetNextUserStation(nbyUser);

		if (nbyUser == m_byMeStation || iUser == m_byMeStation)
		{
			m_pGameEngine->OnShowPdg(SW_HIDE);
			m_pGameEngine->OnAutoTrusteeship(SW_SHOW);
		}
		else
		{
			m_pGameEngine->OnAutoTrusteeship(SW_HIDE);
			m_pGameEngine->OnShowPdg(SW_SHOW);
		}
	}

	// ǿ��ˢ���������
	/*CWnd * pWnd = m_pGameEngine->GetControlById(IDC_REFRESH_RECT);
	if (pWnd != NULL)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		m_pGameEngine->InvalidateRect(&rect);
	}*/

	return true;
}


//�յ������ע����Ϣ�ӿ�
bool CGameMachine::OnHandleGetBetInfo(void* pData,UINT uiSize)
{
	TBet* pBet = (TBet*)pData;
	if (!m_pGameEngine)
	{
		return false;
	}

	//֪ͨ������л滭����//////////
	m_pGameEngine->OnShowGetBetInfo(pBet);
	
	return true;
}

//����������ƴ���ӿ�
bool CGameMachine::OnHandleHitHandCards(BYTE byCard[],int iUpCount)
{
	if (!m_pGameEngine)
	{
		return false;
	}

	//��������////////////

	CUserDataCliObject* pMeUser = &m_UserData[m_byMeStation];

	//�����ƵĺϷ���

	//�����ƵĺϷ���

	//֪ͨ������л滭����//////////

	return true;
}

//��Ұ���ȫ���´���ӿ�
bool CGameMachine::OnHandleUnUpCard()
{
	if (!m_pGameEngine)
	{
		return false;
	}
	
	CUserDataCliObject* pMeData = &m_UserData[m_byMeStation];

	//��ȡ����///////////////////////////////////////////////////////////

	if (pMeData->m_iCardsNum == 0)
	{
		return false;
	}

	//֪ͨ������л滭����////////////////
	//m_pGameEngine->OnShowClieckedCue(pMeData->m_byMyCards,pMeData->m_iCardsNum,NULL,0);

	//����һ���������ƿؼ�����Ϣ���Ա���������������ܷ�ɳ���
	m_pGameEngine->OnPostMessage(IDVB_HIT_CARD,NULL);

	return true;
}

//�����ʾ����ӿ�
bool CGameMachine::OnHandleCue()
{
	if (!m_pGameEngine)
	{
		return false;
	}

	//��ȡ����///////////////////////////////////////////////////////////

	
	//֪ͨ������л滭����////////////////
	//m_pGameEngine->OnShowClieckedCue(pMeData->m_byMyCards,pMeData->m_iCardsNum,byUpCards,pMeData->m_iCardsNum);

	//����һ���������ƿؼ�����Ϣ���Ա���������������ܷ�ɳ���
	m_pGameEngine->OnPostMessage(IDVB_HIT_CARD,NULL);

	return true;
}

//���ƴ���ӿ�
bool CGameMachine::OnHandleCompareCard(void* pData,UINT uiSize)
{
	TCompareCard *pCmd = (TCompareCard*)pData;
	
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		m_pGameEngine->ShowControl(IDC_SIDE_POT + i, SW_HIDE);
		m_pGameEngine->ShowControl(IDC_POT_DINAR + i, SW_HIDE);

		if (m_pGameEngine->GetUserInfo(i) == NULL || pCmd->nHandCardCount[i] <= 0)
		{
			continue ;
		}
		
		// ��ʾ����߳�����
		m_pGameEngine->OnShowPots(&m_tagBetPool);

		CUpGradeGameCard * pCard = dynamic_cast<CUpGradeGameCard * >(m_pGameEngine->GetControlById(IDC_SHOW_CARD + m_pGameEngine->ViewStation(i)));
		if (pCard != NULL)
		{
			BYTE bCard[2] = {0};
			if (i == m_byMeStation)
			{
				// �Լ�����2�ſ��õ��Ƶ���
				for (int j = 0; j < 5; j++)
				{
					if (bCard[0] == 0 && pCmd->bHandCards[i][0] == pCmd->bCards[i][j])
					{
						bCard[0] = 1;
					}

					if (bCard[1] == 0 && pCmd->bHandCards[i][1] == pCmd->bCards[i][j])
					{
						bCard[1] = 1;
					}

					if (bCard[0] && bCard[1])
					{
						break;
					}
				}
			}
			/*CString str;
			str.Format("dxh: ���������λ��: %d,%d", bCard[0], bCard[1]);
			OutputDebugString(str);*/

			pCard->SetCard(pCmd->bHandCards[i], bCard, 2, 2);
			pCard->ShowWindow(SW_SHOW);
		}
		
		BYTE bCardType[] = {0, 9, 8, 7, 6, 5, 5, 4, 3, 2, 1, 1, 0};

		BzImage * pImage = dynamic_cast<BzImage *>(m_pGameEngine->GetControlById(IDC_SHOW_CARDTYPE_NAME + m_pGameEngine->ViewStation(i)));
		if (pImage != NULL)
		{
			pImage->SetStartPosition(bCardType[pCmd->nCardKind[i]] * 150, 0);
			pImage->ShowWindow(SW_SHOW);
			pImage->Invalidate();
		}

		if (i == m_byMeStation)
		{
			CUpGradeGameCard* pGameCrad = dynamic_cast<CUpGradeGameCard*>(m_pGameEngine->GetControlById(IDC_SHOW_BASE_CARD));
			if (pGameCrad != NULL)
			{
				BYTE bCard[5] = {0};
				BYTE bCardCount = 0;

				// �Լ�����2�ſ��õ��Ƶ���
				for (int j = 0; j < 5; j++)
				{
					for (int k = 0; k < 5; k++)
					{
						if (bCard[k] == 0 && m_pGameEngine->m_iCardData[k] == pCmd->bCards[i][j])
						{
							bCard[k] = 1;
							bCardCount++;
							break;
						}
					}
				}

				/*CString str;
				str.Format("dxh: ����Ĺ�����λ��: %d,%d,%d,%d,%d", bCard[0], bCard[1], bCard[2], bCard[3], bCard[4]);
				OutputDebugString(str);*/
				pGameCrad->SetCard((BYTE *)m_pGameEngine->m_iCardData, bCard, m_pGameEngine->m_iCardDataCounts, 5);
				pGameCrad->Invalidate();
			}
		}
	}
	
	return true;
}


//���㴦��ӿ�
bool CGameMachine::OnHandleRoundResult(void* pData,UINT uiSize)
{
	TResult* pResult = (TResult*)pData;
	
	if (!m_pGameEngine)
	{
		return false;
	}

	//��¼��Ϸ����
	::memcpy(&m_tagResult,pResult,sizeof(TResult));

	//������Ϸ��״̬
	m_pGameEngine->SetGameStatus(GS_WAIT_NEXT_ROUND);

	//֪ͨ������л滭����

	//�ռ�����
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_nBetMoney[i] > 0)
		{
			m_pGameEngine->OnShowCollectJotens(i);
		}
	}

	m_pGameEngine->SetTimer(IDT_COLLECT_JETONS,1500,NULL);

	// ��ʾׯ�ҡ�Сäע����äע
	if (m_byNTUser == m_bySmallBlind)
	{
		m_pGameEngine->ShowControl(IDC_SMALL_BLIND, SW_HIDE);
		if (!m_bGiveUp[m_byNTUser])
		{
			m_pGameEngine->OnPlayMoveFlagAnim(m_byNTUser,IDC_ZHUANG,false);
		}
		if (!m_bGiveUp[m_byBigBlind])
		{
			m_pGameEngine->OnPlayMoveFlagAnim(m_byBigBlind,IDC_BIG_BLIND,false);
		}
	}
	else
	{
		if (!m_bGiveUp[m_bySmallBlind])
		{
			m_pGameEngine->OnPlayMoveFlagAnim(m_bySmallBlind,IDC_SMALL_BLIND,false);
		}

		if (!m_bGiveUp[m_byBigBlind])
		{
			m_pGameEngine->OnPlayMoveFlagAnim(m_byBigBlind,IDC_BIG_BLIND,false);
		}

		if (!m_bGiveUp[m_byNTUser])
		{
			m_pGameEngine->OnPlayMoveFlagAnim(m_byNTUser,IDC_ZHUANG,false);
		}
	}
	
	//m_pGameEngine->OnShowRoundResult(pResult);

	return true;
}

//����䷢����ӿ�
bool CGameMachine::OnHandleDispatchJetons()
{
	if (!m_pGameEngine)
	{
		return false;
	}

	//֪ͨ������л滭����
	m_pGameEngine->OnRemoveCollectJotens();

	//�ռ�����
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		bool bIsWin = false;

		for (int j = 0; j < 8; j++)
		{
			if (m_tagResult.nUserBet[i][j] > 0)
			{
				if (m_tagResult.bWinBetPools[i][j])
				{
					bIsWin = true;
					break;
				}
			}
		}

		if (bIsWin)
		{
			m_pGameEngine->OnShowDispatchJotens(i,m_tagResult.nMoney[i]);
		}
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			m_pGameEngine->ShowControl(IDC_BALANCE_POT + i, SW_HIDE);			// ���ر߳ؽ���
			m_pGameEngine->ShowControl(IDC_BALANCE_POT_MONEY + i, SW_HIDE);		// ���ر߳ؽ�����ʾ���
		}
	}
	m_pGameEngine->PlayBaseSound(DZPKSound::BT_CHIP_MONEY_MUSIC);
	m_pGameEngine->SetTimer(IDT_DISPATCH_JETONS,1500,NULL);

}

//��һ�غ�׼����������ӿ�
bool CGameMachine::OnHandleNextRoundReq(BBaseParameter* pParam)
{
	if (!m_pGameEngine)
	{
		return false;
	}

	if (pParam->GetByte(IDB_NEXT_ROUND_REQ) == 0x01) //�����ѱ�Finish
	{
		m_pGameEngine->OnCancel();
		return true;
	}

	TNextRoundReq::emType emReqType = (TNextRoundReq::emType)(pParam->GetInt(IDB_NEXT_ROUND_REQ));

	if (emReqType == TNextRoundReq::READY)
	{
		//֪ͨ������л滭����
		m_pGameEngine->OnShowNextRoundReady();
	}

	TNextRoundReq tagNextRoundReq;
	tagNextRoundReq.nType = emReqType;

	//֪ͨͨѶ�๤��
	m_pGameEngine->SendGameData(&tagNextRoundReq,sizeof(TNextRoundReq),MDM_GM_GAME_NOTIFY,ASS_NEXT_ROUND_REQ,0);	

	if (emReqType == TNextRoundReq::EXIT)
	{
		//֪ͨ������л滭����
		m_pGameEngine->SendGameData(MDM_GR_ROOM,ASS_GR_USER_EXIT_BYSELF,0);
		m_pGameEngine->OnCancel();
	}

	return true;
}

//�»غϿ�ʼ����ӿ�
bool CGameMachine::OnHandleNewRoundBegin(void* pData,UINT uiSize)
{
	TNextRoundBeginInfo* pNextRoundBeginInfo = (TNextRoundBeginInfo*)pData;

	if (!m_pGameEngine)
	{
		return false;
	}

	//��¼ׯ�ҵ�λ��
	m_byNTUser = pNextRoundBeginInfo->byNTUser;

	//��¼��Сäע
	m_bySmallBlind = pNextRoundBeginInfo->bSmallBlind;
	m_byBigBlind = pNextRoundBeginInfo->bBigBlind;
	
	//�й�״̬
	m_bTuoGuan = false;

	//������գ���������
	::memset(m_byBackCard,0,MAX_BACKCARDS_NUM);
	m_iBackCardNums = 0;


	//��ʼһЩ��������/////////////////////////////


	TDeskCfg::TRule* pRule = &m_tagDeskCfg.Rule;

	//��ʼ�������
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		m_UserData[i].Init();
	}
	
	//֪ͨ������л滭����
	m_pGameEngine->OnShowNewRoundBegin(pNextRoundBeginInfo);

	return true;
}

//û�����ͬ���ע����û����ҵ�ʱ����ӿ�
bool CGameMachine::OnHandleNoPlayer(void* pData,UINT uiSize)
{
	TNoPlayer* pNoPlayer = (TNoPlayer*)pData;

	if (!m_pGameEngine)
	{
		return false;
	}

	//֪ͨ������л滭����
	if (!m_pGameEngine->IsResultDlgShowing())
	{
		m_pGameEngine->StopAndHideFlashTimer(m_byMeStation);
		m_pGameEngine->ShowFlashTimer(m_byMeStation,5,IDT_TIMEOUT_NO_PLAYER);
		m_pGameEngine->OnShowNoPlayer(pNoPlayer);
		//m_pMedia->m_bExitDesk = true;
	}
	else
	{
		m_pGameEngine->OnShowNoPlayer(pNoPlayer,true);
	}

	return true;
}

//����Ƿ�ͬ����Ϸ�Ĵ���ӿ�
bool CGameMachine::OnHandleAgree(void* pData,UINT uiSize)
{
	TAgree* pAgree = (TAgree*)pData;

	if (!m_pGameEngine)
	{
		return false;
	}

	//֪ͨ������л滭����
	if (pAgree->bAgree)
	{
		if (!pAgree->bAllAgree)
		{
			m_pGameEngine->OnShowAgree(pAgree->byUser);
		}
		else
		{
			m_pGameEngine->OnShowAllAgree();
		}
	}
	else
	{
		m_pGameEngine->OnShowDisAgree(pAgree->byUser);
	}

	//֪ͨ��Ч����в��Ź���
	if (pAgree->bAgree)
	{
		;//m_pGameEngine->Speak(pAgree->byUser,DZPKSound::ST_READY,m_ltLangType);
	}


	return true;
}

//������ϵ�Ǯ���㹻ʱ�Ĵ���ӿ�
bool CGameMachine::OnHandleNotEnoughMoney(void* pData,UINT uiSize)
{
	TMoneyLack * pMoneyLack = (TMoneyLack *)pData;
	if (!m_pGameEngine)
	{
		return false;
	}

	if (m_byMeStation == pMoneyLack->nbyUser)
	{
		CString str;
		str.Format("dxh: ���:%d(λ��:%d) �˳�", m_pGameEngine->GetUserInfo(m_byMeStation)->GameUserInfo.dwUserID, pMoneyLack->nbyUser);
		OutputDebugString(str);
		m_pGameEngine->SendGameData(MDM_GR_ROOM, ASS_GR_USER_EXIT_BYSELF,0);
	}

	//֪ͨ������л滭����
	m_pGameEngine->OnShowNotEnoughMoney(pMoneyLack);
	m_pGameEngine->StopAndHideFlashTimer(m_byMeStation);
	
	return true;
}

//ϵͳ�Զ�����Ƭ
bool CGameMachine::OnUserSortOut()
{
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		m_pGameEngine->ShowControl(IDC_BET	 + i, SW_HIDE);	// ������עͼƬ
		m_pGameEngine->ShowControl(IDC_RAISE + i, SW_HIDE);	// ���ؼ�עͼƬ
		m_pGameEngine->ShowControl(IDC_CHECK + i, SW_HIDE);	// ���ع���ͼƬ
		m_pGameEngine->ShowControl(IDC_CALL  + i, SW_HIDE);	// ���ظ���ͼƬ
	}

	return true;
}

//���������Ч����ӿ�
bool CGameMachine::OnHandleCfgSound(BBaseParameter* pParam)
{
	BYTE  byCfgType = pParam->GetByte(IDB_SOUND_CFG_OK);
	DWORD dwCfgValue = pParam->GetDWORD(IDB_SOUND_CFG_OK);

	bool bBgMusic = m_bBgMusic;
	bool bGameSound = m_bGameSound;

	if (byCfgType == 0x01) //������Ϸ����
	{
		//��Ϸ�������ֵĿ����
		bBgMusic = (dwCfgValue & 0x0001L);
		
		//��Ϸ��Ч�Ŀ����
		bGameSound = (dwCfgValue & 0x0002L);
	}

	CString str;
	str.Format("dxh1:bBgMusic = %d m_bBgMusic = %d",bBgMusic,m_bBgMusic);
	OutputDebugString(str);

	if (bBgMusic == m_bBgMusic)
	{
		// ��������δ�����ı�
		m_bGameSound = bGameSound;
		return true;
	}

	m_bBgMusic = bBgMusic;
	m_bGameSound = bGameSound;

	//֪ͨ������л滭������
	m_pGameEngine->OnShowCfgSound();

	return true;
}

/*--���ڶ�ʱ�Ĵ���------------------------------------------------------------------------*/

/*--���ڳ�ʱ�Ĵ���------------------------------------------------------------------------*/

//ѡ��ע��ĳ�ʱ�ӿ�
bool CGameMachine::OnTimeoutCfgDesk(BYTE bDeskStation)
{
	if (!m_pGameEngine)
	{
		return false;
	}

	if (bDeskStation != m_byMeStation)
	{
		return false;
	}

	//֪ͨ������л滭����
	m_pGameEngine->OnTimeoutCfgDesk();

	//ͨ��ͨѶ����й���
	m_pGameEngine->SendGameData(MDM_GM_GAME_NOTIFY,ASS_CFG_DESK_TIMEOUT,0);
	m_pGameEngine->OnCancel();

	return true;
}

//ѡ��ע��ĳ�ʱ�ӿ�
bool CGameMachine::OnTimeoutWaitCfgNote()
{
	if (!m_pGameEngine)
	{
		return false;
	}

	//ͨ��ͨѶ����й���
	//m_pMedia->SendGameData(MDM_GR_ROOM,ASS_GR_USER_EXIT_BYSELF,0);
	m_pGameEngine->OnCancel();

	return true;
}

//�ȴ���עͬ��ĳ�ʱ�ӿ�
bool CGameMachine::OnTimeoutAgreeNote(BYTE bDeskStation)
{
	if (!m_pGameEngine)
	{
		return false;
	}

	if (bDeskStation != m_byMeStation)
	{
		return false;
	}

	//Ĭ�ϲ���
//	OnHandleAgreeDeskCfg(true);

	//֪ͨ������л滭����
	m_pGameEngine->OnTimeoutAgreeDeskCfg();

	return true;
}

//�ȴ���һ�غϿ�ʼ��ʱ���ù⴦��ӿ�
bool CGameMachine::OnTimeoutNextRoundBegin(BYTE bDeskStation)
{
	if (!m_pGameEngine)
	{
		return false;
	}

	//֪ͨ������л滭����

	if (bDeskStation == m_byMeStation)
	{
		m_pGameEngine->StopAndHideFlashTimer(bDeskStation);
	}		

	m_pGameEngine->OnTimeoutNextRoundBegin();

	return true;
}

//���û��Ǯ��ʱ����ӿ�
bool CGameMachine::OnTimeoutNotEnoughMoney(BYTE bDeskStation)
{
	if (!m_pGameEngine)
	{
		return false;
	}

	//ͨ��ͨѶ����й���
	if (bDeskStation == m_byMeStation) //�Լ���ʱ
	{
		m_pGameEngine->SendGameData(MDM_GR_ROOM,ASS_GR_USER_EXIT_BYSELF,0);
		m_pGameEngine->OnCancel();
	}

	return true;
}

/*--����״̬���ֵĴ���------------------------------------------------------------------------*/

//��Ϸ������״̬����
bool CGameMachine::OnRecurBase(BYTE byGameStation,TGSBase* pGSBase)
{
	if (!m_pGameEngine)
	{
		return false;
	}

   /*-------------------------------------------------------------------------------------------
		��һ�δ����Ǹ�����Ϸ�Ļ�������
	-------------------------------------------------------------------------------------------*/
    //�����������

	::memcpy(&m_tagDeskCfg,&pGSBase->tagDeskCfg,sizeof(TDeskCfg));

	//ʱ�����
	m_iVerbTime = pGSBase->iVerbTime;

	return true;
}

//��Ϸ�ȴ�ͬ��״̬����
bool CGameMachine::OnRecurWaitAgree(void* pData)
{
	TGSWaitAgree* pGSWaitAgree = (TGSWaitAgree*)pData;

	if (!m_pGameEngine)
	{
		return false;
	}

    // ����������Ҵ�����״̬
    TSubMoney cmd;
    cmd.bDeskStation = -1;
    memcpy(cmd.nMoney, pGSWaitAgree->nSubMoney, sizeof(cmd.nMoney));
	memcpy(cmd.bIn, pGSWaitAgree->bReady, sizeof(cmd.bIn));

    m_pGameEngine->OnShowSubMoney(&cmd);

	//m_pGameEngine->ShowControl(IDB_BEGIN_GAME, m_pGameEngine->m_bInLisureRoom ? SW_SHOW : SW_HIDE); // �����г�����ʾ׼����ť
	m_pGameEngine->ShowControl(IDB_BEGIN_GAME, SW_SHOW);					// ��ʾ׼����ť
	m_pGameEngine->ShowControl(IDB_ALTER_STAKE, SW_SHOW);					// ��ʾ�޸Ľ��
	m_pGameEngine->ShowControl(IDC_ZHUANG,		SW_SHOW);					// ��ʾׯ��
	m_pGameEngine->ShowControl(IDC_SMALL_BLIND,SW_SHOW);					// ��ʾСäע
	m_pGameEngine->ShowControl(IDC_BIG_BLIND,SW_SHOW);						// ��ʾ��äע


	return true;
}

//��Ϸ�����е�״̬����
bool CGameMachine::OnRecurPlayingGame(void* pData)
{
	TGSPlaying* pGSPlaying = (TGSPlaying*)pData;

	if (!m_pGameEngine)
	{
		return false;
	}

	// ��õõ����Ƶ����
	m_byTokenUser = pGSPlaying->byTokenUser;

    // ׯ�Ҵ�С��
	m_byNTUser = pGSPlaying->byNTUser;
    m_bySmallBlind = pGSPlaying->bySmallBlind;
    m_byBigBlind = pGSPlaying->byBigBlind;

	BzImage * pImage = NULL;

	if (m_byNTUser != 255)
	{
		// ��ʾׯ��ͼƬ
		m_pGameEngine->ShowControl(IDC_SHOW_IMAGE + m_pGameEngine->ViewStation(m_byNTUser), SW_SHOW);
	}
	else
	{
		// ��ʾ��ׯ��ԭʼλ��
		m_pGameEngine->ShowControl(IDC_ZHUANG, SW_SHOW);
	}
	if (m_bySmallBlind != 255)
	{
		pImage = dynamic_cast<BzImage*>(m_pGameEngine->GetControlById(IDC_SHOW_IMAGE1 + m_pGameEngine->ViewStation(m_bySmallBlind)));
		if (pImage != NULL)
		{
			pImage->SetImage("resources\\shengji\\shengji\\xiao.bmp");
			pImage->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		m_pGameEngine->ShowControl(IDC_SMALL_BLIND, SW_SHOW);
	}
	if (m_byBigBlind != 255)
	{
		pImage = dynamic_cast<BzImage*>(m_pGameEngine->GetControlById(IDC_SHOW_IMAGE1 + m_pGameEngine->ViewStation(m_byBigBlind)));
		if (pImage != NULL)
		{
			pImage->SetImage("resources\\shengji\\shengji\\da.bmp");
			pImage->ShowWindow(SW_SHOW);
		}

	}
	else
	{
		m_pGameEngine->ShowControl(IDC_BIG_BLIND, SW_SHOW);
	}
	
	// �����ť
	m_pGameEngine->OnShowSysDeskCfg(&m_tagDeskCfg);


    // ��������
    m_pGameEngine->m_iCardCounts = pGSPlaying->nHandCardNums;
    memcpy(m_pGameEngine->m_iUserDataCard, pGSPlaying->byHandCard, sizeof(BYTE) * m_pGameEngine->m_iCardCounts);
    CUpGradeGameCard* pGameCard = dynamic_cast<CUpGradeGameCard *>(m_pGameEngine->GetControlById(IDC_SHOW_CARD + m_pGameEngine->ViewStation(m_byMeStation)));
    if (pGameCard != NULL)
    {
        pGameCard->SetCard(m_pGameEngine->m_iUserDataCard, NULL, m_pGameEngine->m_iCardCounts, NULL);
        pGameCard->ShowWindow(SW_SHOW);
    }

    // ���ƹ�����
    m_pGameEngine->m_iCardDataCounts = pGSPlaying->nCardsCount;
    memcpy(m_pGameEngine->m_iCardData, pGSPlaying->byCards, sizeof(BYTE) *  pGSPlaying->nCardsCount);
    pGameCard = dynamic_cast<CUpGradeGameCard*>(m_pGameEngine->GetControlById(IDC_SHOW_BASE_CARD));
    if (pGameCard != NULL)
    {
        pGameCard->SetCard((BYTE *)m_pGameEngine->m_iCardData, NULL, m_pGameEngine->m_iCardDataCounts, NULL);
        pGameCard->ShowWindow(SW_SHOW);
    }

    // ȫ�����
    memcpy(m_bAllBet, pGSPlaying->bNoBet, sizeof(pGSPlaying->bNoBet));

    // �������
    memcpy(m_bGiveUp, pGSPlaying->bGiveUp, sizeof(pGSPlaying->bGiveUp));

    // ��ȡ������
    memcpy(m_pGameEngine->m_iMoneys, pGSPlaying->nSubMoney, sizeof(pGSPlaying->nSubMoney));	

    // ��ע���
    for (int i = 0; i < PLAY_COUNT; i++)
    {
        if (m_pGameEngine->GetUserInfo(i) == NULL)
        {
            continue ;
        }

        // ��ʾ������
        CTransparentStatic * pStatic = dynamic_cast<CTransparentStatic *>(m_pGameEngine->GetControlById(IDC_MONEY + m_pGameEngine->ViewStation(i)));
        if (pStatic != NULL)
        {
            CString strnMoney;
            strnMoney.Format("%d", m_pGameEngine->m_iMoneys[i]);
            pStatic->SetMyText(strnMoney.GetBuffer());
            pStatic->ShowWindow(SW_SHOW);
        }

        //��ʾ������UIͼƬ
        m_pGameEngine->ShowControl(IDC_JIN_BI + m_pGameEngine->ViewStation(i), SW_SHOW);

       
		BzImage *pImage = NULL;
		 // ��ʾ��ע����ͼƬ
		pImage = dynamic_cast<BzImage*>(m_pGameEngine->GetControlById(IDC_MONEY_MAP + m_pGameEngine->ViewStation(i)));
		if (pImage != NULL)
		{
			pImage->ShowWindow(SW_SHOW);
			pImage->Invalidate();
		}
		 // ��ʾ��ע���
        m_nBetMoney[i] = 0;
        for (int j = 0; j < 8; j++)
        {
            m_nBetMoney[i] += pGSPlaying->nBetMoney[i][j];
        }

        pImage = dynamic_cast<BzImage*>(m_pGameEngine->GetControlById(IDC_TABLE_BOARD_MONEY + m_pGameEngine->ViewStation(i)));
        if (pImage != NULL)
        {
            CString str;
            str.Format("%d", m_nBetMoney[i]);
            pImage->SetMyText(str.GetBuffer());
			pImage->ShowWindow(SW_SHOW);
			pImage->Invalidate();
        }

		pImage = dynamic_cast<BzImage*>(m_pGameEngine->GetControlById(IDPIC_BACK_CARD_BASE + m_pGameEngine->ViewStation(i)));
		if (pImage != NULL)
		{
			pImage->ShowWindow(SW_SHOW);
			pImage->Invalidate();
		}
    }

    // ��ǰ�������
    TToken toKen;
    toKen.bNewTurn = pGSPlaying->bNewTurn;
    toKen.byUser = pGSPlaying->byTokenUser;
    toKen.byVerbFlag = pGSPlaying->byVerbFlag;
	toKen.nCallMoney = pGSPlaying->nCallMoney;
    OnHandleGetToken(&toKen, sizeof(toKen));

	return true;

}

//�ȴ���һ����Ϸ��״̬����
bool CGameMachine::OnRecurWaitNextRound(void* pData)
{
	TGSWaitNextRound* pGSWaitNextRound = (TGSWaitNextRound*)pData;

	if (!m_pGameEngine)
	{
		return false;
	}

	// ֪ͨ������л滭����
    // ����������Ҵ�����״̬
    TSubMoney cmd;
    cmd.bDeskStation = -1;
    memcpy(cmd.nMoney, pGSWaitNextRound->nSubMoney, sizeof(cmd.nMoney));
    m_pGameEngine->OnShowSubMoney(&cmd);

	//m_pGameEngine->ShowControl(IDB_BEGIN_GAME, m_pGameEngine->m_bInLisureRoom ? SW_SHOW : SW_HIDE); // �����г�����ʾ׼����ť
	m_pGameEngine->ShowControl(IDB_BEGIN_GAME, SW_SHOW);					// ��ʾ��ʼ��ť
	m_pGameEngine->ShowControl(IDB_ALTER_STAKE, SW_SHOW);					// ��ʾ�޸Ľ��
	m_pGameEngine->ShowControl(IDC_ZHUANG,		SW_SHOW);					// ��ʾׯ��
	m_pGameEngine->ShowControl(IDC_SMALL_BLIND,SW_SHOW);					// ��ʾСäע
	m_pGameEngine->ShowControl(IDC_BIG_BLIND,SW_SHOW);						// ��ʾ��äע

	return true;

}

//˳ʱ��˳��õ��¼�λ��
BYTE CGameMachine::GetNextUserStation(BYTE byStation, BOOL bIsAll)
{
	int nCount = 0;
	int iNextStation = (byStation + PLAY_COUNT + 1) % PLAY_COUNT;

	// �ƹ���Ч������ѷ��������
	while (m_pGameEngine->GetUserInfo(iNextStation) == NULL || m_bGiveUp[iNextStation] || (!bIsAll && m_bAllBet[iNextStation]))
	{
		iNextStation = (iNextStation + PLAY_COUNT + 1) % PLAY_COUNT;
		nCount++;

		if (nCount > PLAY_COUNT)
		{
			iNextStation = -1;
			break;
		}
	}

	return iNextStation;
}

//��ʱ��˳��õ��ϼ�λ��
BYTE CGameMachine::GetLastUserStation(BYTE byStation, BOOL bIsAll)
{
	int nCount = 0;
	int iLastStation = (byStation + PLAY_COUNT - 1) % PLAY_COUNT;

	// �ƹ���Ч������ѷ��������
	while (m_pGameEngine->GetUserInfo(iLastStation) == NULL || m_bGiveUp[iLastStation] || (!bIsAll && m_bAllBet[iLastStation]))
	{
		iLastStation = (iLastStation + PLAY_COUNT - 1) % PLAY_COUNT;

		nCount++;

		if (nCount > PLAY_COUNT)
		{
			iLastStation = -1;
			break;
		}
	}

	return iLastStation;
}

//��ռ�¼��һ���������ջ�б�
void CGameMachine::ClearAllStack()
{
	while (!m_stackLastStation.empty())
	{
		m_stackLastStation.pop();   // �Ƴ�ջ������
	}
}