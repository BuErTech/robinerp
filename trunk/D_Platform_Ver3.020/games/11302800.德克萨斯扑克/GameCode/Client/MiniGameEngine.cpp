#include "StdAfx.h"
#include "MiniGameEngine.h"
#include "AFCMessageBox.h"
#include <math.h>
#include "io.h"
#include "LoadSelection.h"

#ifdef  SUPER_VERSION
//#include "..\..\�ͻ������\�����.2003\resource.h"
#include "platform/CardSet.h"
#endif

//#include "..\..\��Ϸ����������\include\�ͻ���\�����\resource.h"
//#pragma comment(lib,"vfw32.lib")

//#define SAIZI_IMAGE      "awardpoint.bmp"
//#define SAIZI_IMAGEA     "1.bmp"O


//�Զ�����Ϣ��// To Be Continued..
enum MB_AFC_STYLE          //KT��Ϣ������
{
	AFC_CONFIRM = 500,       //ȷ�϶Ի���
	AFC_YESNO,               //��ʾ���ǡ������񡱶Ի���
	AFC_WARN                 //����Ի���
};

//���캯��
CMiniGameEngine::CMiniGameEngine() : CLoveSendClassInExe(),m_pSESpeak(NULL),m_pFinishDlg(NULL),m_pSoundCfgDlg(NULL)
{
	m_pSEBase = createIrrKlangDevice();
	m_pSEHurryUp = createIrrKlangDevice();
	m_pSESpeak = createIrrKlangDevice();
	m_pSEBkMusic = createIrrKlangDevice();

	//ʱ�Ӽ���
	m_iClockCounter = 0;
	m_pLoadSelectionDlg = NULL;
}

//��������
CMiniGameEngine::~CMiniGameEngine()
{
	if(m_pSEBase)
	{
		m_pSEBase->drop();
	}
	if(m_pSEHurryUp)
	{
		m_pSEHurryUp->drop();
	}
	if(m_pSESpeak)
	{
		m_pSESpeak->drop();
	}
	if(m_pSEBkMusic)
	{
		m_pSEBkMusic->drop();
	}
}

BEGIN_MESSAGE_MAP(CMiniGameEngine, CLoveSendClassInExe)
	ON_WM_TIMER()
	ON_WM_WINDOWPOSCHANGED()	// ���ڵ���Flash��������ڸ����ڵ�λ��
	ON_MESSAGE(MSG_FLASH_CMD, FlashEndMessage)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//��ʱ��
void CMiniGameEngine::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case 110:
		{
			KillTimer(nIDEvent);

			AFCCloseFrame();

			__super::OnCancel();
		}
		break;
	case IDT_CLOCK:
		{
			if (m_iClockCounter <= 8)
			{
				//PlayBaseSound(DZPKSound::BT_HURRYUP);
			}
			if (m_iClockCounter == 0)
			{
				KillTimer(IDT_CLOCK);
			}
			m_iClockCounter--;
		}
		break;
	case IDT_OFF_BALANCE:
		{
			KillTimer(IDT_OFF_BALANCE);

			//ֱ���йܰ�ȷ����ʼ��һ��

			if (m_pFinishDlg)
			{
				m_pFinishDlg->EndModalLoop(EVENT_CLOSE_MAINFRAME);
			}
		}
		break;
	case IDT_AUTO_READY:
		{
			KillTimer(IDT_AUTO_READY);

			// -----------------------���ö�ʱ�� -----------------------
			BzNumberPanel *pNumPanel = dynamic_cast<BzNumberPanel*>(GetControlById(IDC_CLCKTIME));
			if (pNumPanel != NULL)
			{
				// �������ö�ʱ��
				pNumPanel->SetCharIndex(m_GameMachine.m_byMeStation);
				pNumPanel->SetEventIndex(IDT_AUTO_READY);
				pNumPanel->SetNumber(20);  // 5��
				pNumPanel->StartTiming();
				pNumPanel->ShowWindow(SW_SHOW);
			}
			// -----------------------------------------------------------------
		}
		break;
	case IDT_SEND_CARD_GAP:
		{
			//���Ƶ�ʱ�����ѵ�������
			CCardSender* pCardSender = (CCardSender*)m_GameMachine.GetSubMachine(IDSM_CARD_SENDER);
			if (pCardSender)
			{
				pCardSender->OnTimerGap();
			}
		}
		break;

	case IDT_SEND_MOVING_GAP:
		{
			//�����˶���ʱ�����ѵ�
			CCardSender* pCardSender = (CCardSender*)m_GameMachine.GetSubMachine(IDSM_CARD_SENDER);
			if (pCardSender)
			{
				pCardSender->OnTimerMovingGap();
			}
		}
		break;
	case IDT_MONEY_LACK:
		{
			ShowControl(IDC_MONEYLACK, SW_HIDE);

			CString str;
			str.Format("dxh: ��Ҳ��㣬�Զ��˳� UserMoney %d nMoney %d", m_pUserInfo[m_GameMachine.m_byMeStation]->GameUserInfo.dwMoney, m_GameMachine.m_tagDeskCfg.dz.iSubMinMoney);
			OutputDebugString(str);

			SendGameData(MDM_GM_GAME_NOTIFY, ASS_MONEY_LACK, 0);
			SendGameData(MDM_GR_ROOM, ASS_GR_USER_EXIT_BYSELF,0);

			OnCancel();
		}
		break;

	case IDT_COLLECT_JETONS:
		{
			//�ռ����붯�����
			KillTimer(IDT_COLLECT_JETONS);
			ShowControl(IDC_ZHUANG, SW_SHOW);
			ShowControl(IDC_BIG_BLIND,SW_SHOW);
			ShowControl(IDC_SMALL_BLIND,SW_SHOW);

			SetTimer(IDT_WAIT_DISPATCH_JETONS,1500,NULL);
		}
		break;

	case IDT_WAIT_DISPATCH_JETONS:
		{
			//�ȴ��������
			KillTimer(IDT_WAIT_DISPATCH_JETONS);
			m_GameMachine.OnHandleDispatchJetons();
		}
		break;

	case IDT_DISPATCH_JETONS:
		{
			//�䷢�������
			KillTimer(IDT_DISPATCH_JETONS);
			OnRemoveCollectJotens();
			OnShowRoundResult(&m_GameMachine.m_tagResult);
		}
		break;
	case IDT_TIME_SOUND:
		{
			// ���ŵ���ʱ����
			KillTimer(IDT_TIME_SOUND);
			PlayBaseSound(DZPKSound::BT_ALARM_CLOCK);
		}
		break;
	default:
		break;
	}

	CLoveSendClassInExe::OnTimer(nIDEvent);
}

void CMiniGameEngine::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	//::BZSetFlashPosWithParent(this);	// ���ڵ���Flash��������ڸ����ڵ�λ��
	if(NULL != m_pFlashManager)
	{
		m_pFlashManager->SetFlashPosWithParent();
	}

	CLoveSendClassInExe::OnWindowPosChanged(lpwndpos);
}


int CMiniGameEngine::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return __super::OnCreate(lpCreateStruct);
}

void CMiniGameEngine::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	static long dwClickCount = 0;
	long dwValue = GetTickCount();

	if((dwValue - dwClickCount) > 10 && (dwValue - dwClickCount) < 200)
	{
		OnPostMessage(IDM_DLBCLICK,NULL);
		dwClickCount = 0;
	}
	else
	{
		dwClickCount = dwValue;
	}
	CLoveSendClassInExe::OnLButtonDown(nFlags, point);	
}

void CMiniGameEngine::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CLoveSendClassInExe::OnLButtonUp(nFlags, point);
}


void CMiniGameEngine::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	OnPostMessage(IDB_OUT_CARD,NULL);

	CLoveSendClassInExe::OnRButtonDown(nFlags, point);
}

void CMiniGameEngine::OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags)
{

#ifdef CD_CAN_SETPAI

	if(nChar == VK_F8)
	{
		//�Ƿ����������Ϸ��ֻ������Ϸ����ʱ����������
		if (AfxMessageBox(_TEXT("�����Ƿ����������Ϸ��"),MB_YESNO) == IDYES)
		{
			SendGameData(MDM_GM_GAME_NOTIFY,ASS_FINISH_ROUND_REQ,0);
		}
	}

#endif
}

/*-----------------------------------------------------------------------------------------------------------
	һϵ��ƽ̨�ṩ����Ҫ������������Ϸ�ĺ���ʵ�֣������ô������Ϸ�Ŀ�������ÿ��������ʵ������Ϸ���졣
  ----------------------------------------------------------------------------------------------------------*/

BOOL CMiniGameEngine::OnInitDialog()
{
	// ���г������������
	m_bInLisureRoom = m_pGameInfo->dwRoomRule & GRR_SEISURE_ROOM;

	if (!m_bInLisureRoom)
	{
		//�������鶯��
		PlayWaitGroup();
	}
	else
	{
		// �Ǵ�������Ҫ�Լ�����׼����Ϣ���������Żᷢ��������λ
		MSG_GM_S_ClientInfo ClientInfo;
		ClientInfo.bEnableWatch=m_pGameInfo->bEnableWatch;
		SendGameData(&ClientInfo,sizeof(ClientInfo),MDM_GM_GAME_FRAME,ASS_GM_GAME_INFO,0);

	}

	OutputDebugString("SJClient��OnInitDialog0");
	ShowControl(1045,SW_SHOW);

	// ���ؿؼ�
	DispalyAllControl(SW_HIDE);

	//��ʼ�ƿؼ�
	OnInitCardsControl();

	//��ʼ������ֿؼ�
	OnInitMoneyStatic();

	//m_uSendCardFlashID = 0;

	//���λ��
	m_iUser = 0;
	
	//��������
	memset(m_iUserDataCard, 0, sizeof(m_iUserDataCard));

	//������
	m_iCardCounts = 0;

	// ���������ʾ
	memset(m_iCardData, 0, sizeof(m_iCardData));

	// ��¼ÿ����Ҵ�����
	memset(m_iMoneys, 0, sizeof(m_iMoneys));

	//������3
	m_iCardDataCounts = 0;

	//��ʼ���ӿؼ�
	OnInitClocks();

	//��ʼ�����ؼ�
	OnInitHanHuaControl();

	return __super::OnInitDialog();
}

//������Ϸ
void CMiniGameEngine::ResetGameStation(int iGameStation)//��Ϸ��ʼ
{
	switch (iGameStation)
	{
	case RS_ALL_VIEW:// ��ʼ�����б���		
	case RS_GAME_CUT:
		{
			ResetGameStation(RS_GAME_BEGIN);
			ResetGameStation(RS_GAME_END);
		}
		break;
	case RS_GAME_BEGIN:
		{
			break ;
		}
	case RS_GAME_END:
		{	
			break ;
		}
	default:
		break;
	}

	return ;
	//__super::ResetGameStation(iGameStation);
}

//������Ϸ״̬
bool CMiniGameEngine::SetGameStation(void * pStationData, UINT uDataSize)
{
	m_GameMachine.m_byMeStation = GetMeUserInfo()->bDeskStation;

	//����Ϸ״̬�������йܸ���Ϸ״̬��m_GameMachine����
	BYTE byStationParam = GetStationParameter();

	m_GameMachine.StartUp(this,byStationParam,pStationData);

	return true;
}

//�������ý���
void CMiniGameEngine::ResetGameFrame()
{
	return __super::ResetGameFrame();
}

//���Ż��㵽��ͼλ��
BYTE CMiniGameEngine::ViewStation(BYTE bDeskStation)
{
	if (GetMeUserInfo()->bDeskStation >= PLAY_COUNT)
	{
		return bDeskStation;
	}

	return ((bDeskStation + PLAY_COUNT + MY_VIEWSTATION - GetMeUserInfo()->bDeskStation) % PLAY_COUNT);
}

//��Ϸ��Ϣ������
bool CMiniGameEngine::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{

	if (pNetHead->bMainID==MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{
		case ASS_GAME_BEGIN:
			{
				//��Ϸ��ʼ
				return m_GameMachine.OnHandleGameBegin(pNetHead,uDataSize);
			}
			break;

		case ASS_SYS_DESK_CFG:
			{
				//�յ�ϵͳ�������ӵ���Ϣ����������
				return m_GameMachine.OnHandleGetSysDeskCfg(pNetData,uDataSize);
			}
	
			break;
		case ASS_CFG_DESK: 
			{
				//���õ�ע
				return m_GameMachine.OnHandleCfgDesk(pNetData,uDataSize);
			}
			break;
		case ASS_CFG_DESK_TIMEOUT:
			{
				//�������õ�ע��ʱ            
				return m_GameMachine.OnHandleWaitCfgDeskTimeout();
			}
			break;
		case ASS_DESK_CFG: 
			{
				//�յ�ע����Ϣ����������
				return m_GameMachine.OnHandleGetDeskCfg(pNetData,uDataSize);
			}
			break;
		case ASS_SHOW_IN_MONEY: 
			{
				//�յ���������Ϣ����������
				return m_GameMachine.OnHandleInMoney(pNetData,uDataSize);
			}
			break;
		case ASS_SEND_A_CARD:
			{
				//�յ���������һ���ư���������
				return m_GameMachine.OnHandleSendACards(pNetData,uDataSize);
			}
			break;

		case ASS_BETPOOL_UP:
			{
				//�յ��������߳ظ��°���������
				return m_GameMachine.OnHandleBetPoolUp(pNetData,uDataSize);
			}
			break;
			
		case ASS_SEND_3_CARD:
			{
				//�յ���������һ���ư���������
				return m_GameMachine.OnHandleSend3Cards(pNetData,uDataSize);
			}
			break;

		case ASS_SEND_4_5_CARD:
			{
				//�յ���������һ���ư���������
				return m_GameMachine.OnHandleSend1Cards(pNetData,uDataSize);
			}
			break;
		case ASS_SEND_CARD_FINISH:
			{
				//�յ�����������������ҷ�����ɵ���Ϣ��������
				return m_GameMachine.OnHandleSendCardFinish();
			}
			break;

		case ASS_BET_INFO:
			{
				//�յ������ע����Ϣ
				return m_GameMachine.OnHandleGetBetInfo(pNetData,uDataSize);
			}
			break;

		case ASS_TOKEN:
			{
				//�յ�������Ϣ�������û�
				return m_GameMachine.OnHandleGetToken(pNetData,uDataSize);
			}
			break;
		case ASS_COMPARE_CARD:
			{
				//������Ϣ
				return m_GameMachine.OnHandleCompareCard(pNetData,uDataSize);
			}
			break;
		case ASS_RESULT:
			{
				//�յ�������Ϣ����������
				return m_GameMachine.OnHandleRoundResult(pNetData,uDataSize);
			}
			break;
		case ASS_NEW_ROUND_BEGIN:
			{
				//�յ��µ�һ�غϿ�ʼ��Ϣ����������
				return m_GameMachine.OnHandleNewRoundBegin(pNetData,uDataSize);
			}
			break;
		case ASS_NO_PLAYER:
			{
				//û����ҽ�����Ϸ���˳�����
				return m_GameMachine.OnHandleNoPlayer(pNetData,uDataSize);
			}
			break;
		case ASS_AGREE:
			{
				//����Ƿ�ͬ�����Ϸ�ģ�����
				return m_GameMachine.OnHandleAgree(pNetData,uDataSize);
			}
			break;
		case ASS_NOT_ENOUGH_MONEY:
			{
				//������ϵ�Ǯ���㹻ʱ������
				return m_GameMachine.OnHandleNotEnoughMoney(pNetData,uDataSize);
			}
			break;

		case ASS_SORT_OUT:
			{
				//ϵͳ�Զ�����Ƭ
				return m_GameMachine.OnUserSortOut();
			}
			break;
		}		
	}
	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize);
}

bool CMiniGameEngine::OnPostMessage(UINT pId,BBaseParameter* pParam)
{
	switch(pId)
	{
	case IDC_CLCKTIME:    // �Զ�׼����ʱ��
		{
			if (pParam->GetInt(EVENT_LPARAM) == EVENT_TIMER_STOPED) 
			{
				// -----------------------�رն�ʱ�� -----------------------
				BzNumberPanel *pNumPanel = dynamic_cast<BzNumberPanel*>(GetControlById(IDC_CLCKTIME));
				if (pNumPanel != NULL)
				{
					pNumPanel->StopTiming();
					pNumPanel->ShowWindow(SW_HIDE);
				}
				// -----------------------------------------------------------------

				if (m_iMoneys[m_GameMachine.m_byMeStation] <= m_GameMachine.m_tagDeskCfg.dz.iUntenMoney)
				{
					// �Զ����������ҿ�
					OnLoadSelectionDlg(true);
				}
				else
				{
					// �Զ�׼��(����)
					TSubMoney cmd;
					cmd.nMoney[m_GameMachine.m_byMeStation] = m_iMoneys[m_GameMachine.m_byMeStation];
					cmd.bDeskStation = m_GameMachine.m_byMeStation;
					SendGameData(&cmd, sizeof(cmd), MDM_GM_GAME_NOTIFY, ASS_SUB_MENOY, 0);		
				}
			}

			return true;
		}
		break;
	case IDB_BEGIN_GAME:	// ��ʼ��ť
		{
			if (m_iMoneys[m_GameMachine.m_byMeStation] <= m_GameMachine.m_tagDeskCfg.dz.iUntenMoney)
			{
				// �Զ����������ҿ�
				OnLoadSelectionDlg(true);
			}
			else
			{
				// �Զ�׼��(����)
				TSubMoney cmd;
				cmd.nMoney[m_GameMachine.m_byMeStation] = m_iMoneys[m_GameMachine.m_byMeStation];
				cmd.bDeskStation = m_GameMachine.m_byMeStation;
				SendGameData(&cmd, sizeof(cmd), MDM_GM_GAME_NOTIFY, ASS_SUB_MENOY, 0);	

				// -----------------------�رն�ʱ�� -----------------------
				BzNumberPanel *pNumPanel = dynamic_cast<BzNumberPanel*>(GetControlById(IDC_CLCKTIME));
				if (pNumPanel != NULL)
				{
					pNumPanel->StopTiming();
					pNumPanel->ShowWindow(SW_HIDE);
				}
				// -----------------------------------------------------------------
			}

			return true;
		}
		break;
	case IDB_ALTER_STAKE:	// �޸Ľ�ť
		{
			return m_GameMachine.OnHandleModifyMoney();
		}
		break;

	case IDB_BET:	// ��ע
		{
			return m_GameMachine.OnHandleBet();
		}
		break;

	case IDB_CALL:	// ��ע
		{
			return m_GameMachine.OnHandleCall();
		}
		break;

	case IDB_RAISE: // ��ע
		{
			return m_GameMachine.OnHandleRaise();
		}
		break;

	case IDB_CHECK: // ����
		{
			return m_GameMachine.OnHandleCheck();
		}
		break;

	case IDB_FOLD:	// ����
		{
			return m_GameMachine.OnHandleFolo();
		}
		break;

	case IDB_ALL_IN: // ȫ��
		{
			return m_GameMachine.OnHandleAllin();
		}
		break;

	case IDB_BUTTON_MONEY: // ��ť
	case IDB_BUTTON_MONEY+1:
	case IDB_BUTTON_MONEY+2:
	case IDB_BUTTON_MONEY+3:
	case IDB_BUTTON_MONEY+4:
		{
			PlayBaseSound(DZPKSound::BT_MONEY_BUTTON);
			return m_GameMachine.OnHandleButtonMoney(pId - IDB_BUTTON_MONEY);
		}
		break;

	case IDB_SLIDING_AXLE: // ������ע��
		{
			
			return m_GameMachine.OnHandleSlidingAxle();
		}
		break;

	case IDB_ADDMONEY: // ��עť
		{
			return m_GameMachine.OnHandleAddMinus();
		}
		break;

	case IDB_REDUCE: // ��עť
		{
			return m_GameMachine.OnHandleSubMinus();
		}
		break;

	case IDB_CALLMANDATE: // ��ע�й�
		{
			return m_GameMachine.OnHandleCallMandate(true);
			ShowControl(IDB_ALTER_STAKE, SW_HIDE);
		}
		break;
	case IDB_CALLMANDATE1: // ȡ����ע�й�
		{
			return m_GameMachine.OnHandleCallMandate(false);
			ShowControl(IDB_ALTER_STAKE, SW_HIDE);
		}
		break;
	case IDB_CHECKMANDATE: // �����й�
		{
			return m_GameMachine.OnHandleCheckMandate(true);
			ShowControl(IDB_ALTER_STAKE, SW_HIDE);
		}
		break;
	case IDB_CHECKMANDATE1:// ȡ�������й�
		{
			return m_GameMachine.OnHandleCheckMandate(false);
			ShowControl(IDB_ALTER_STAKE, SW_HIDE);
		}
		break;
	case IDB_CALLANYMANDATE: // ���κ�ע�й�
		{
			return m_GameMachine.OnHandleCallAnyMandate(true);
			ShowControl(IDB_ALTER_STAKE, SW_HIDE);
		}
		break;
	case IDB_CALLANYMANDATE1: // ȡ�����κ�ע�й�
		{
			return m_GameMachine.OnHandleCallAnyMandate(false);
			ShowControl(IDB_ALTER_STAKE, SW_HIDE);
		}
		break;
	case IDB_PASSABANDONMANDATE: // ���ƻ������й�
		{
			return m_GameMachine.OnHandlePassAbandonMandate(true);
			ShowControl(IDB_ALTER_STAKE, SW_HIDE);
		}
		break;
	case IDB_PASSABANDONMANDATE1: // ���ƻ������й�
		{
			return m_GameMachine.OnHandlePassAbandonMandate(false);
			ShowControl(IDB_ALTER_STAKE, SW_HIDE);
		}
		break;

	case IDB_NEXT_ROUND_REQ: // ��һ�غ�׼����������ӿ�
		{
			return m_GameMachine.OnHandleNextRoundReq(pParam);
		}
		break;

	case IDM_DLBCLICK: //˫������
		{
			return m_GameMachine.OnHandleUnUpCard();
		}
		break;

	case IDB_CUE: //��ʾ
		{
			return m_GameMachine.OnHandleCue();
		}
		break;

	case DDZ_EXIT:
		{	
			// �رհ�ť
			if (!OnCloseGame())
			{
				return false;
			}
			else
			{
				SetStationParameter(0);
				OnCancel();
			}
			return false;
		}
		break;

	case IDB_SOUND_CFG_OK: //���������Ч��
		{
			return m_GameMachine.OnHandleCfgSound(pParam);
		}
		break;

	case IDB_SET_BUTTON:  // ���ð�ť
		{
			return OnShowClickedSoundCfg();
		}
		break;

	default:
		{
			break;
		}
	}

	if (m_pBackCardAnim[MY_VIEWSTATION])
	{/*m_pBackCardAnim[MY_VIEWSTATION]->GetDlgCtrlID()*/
		if (pId == IDC_SENDCARD_ANIM + MY_VIEWSTATION)
		{
			if (pParam->GetInt(EVENT_LPARAM) == EVENT_ANIMATE_FINISHED)
			{
				OnEndSendMeACard(0);
			}
		}
	}

	if (m_pBackCardAnim[MY_VIEWSTATION + PLAY_COUNT])
	{
		if (pId == IDC_SENDCARD_ANIM + PLAY_COUNT + MY_VIEWSTATION)
		{
			if (pParam->GetInt(EVENT_LPARAM) == EVENT_ANIMATE_FINISHED)
			{
				OnEndSendMeACard(1);
			}
		}
	}
	

	//����Լ�����
	if (pId == IDC_HANDCARD_BASE + MY_VIEWSTATION)
	{
 		DWORD dwEvent = pParam->GetInt(EVENT_CARD);
		//��ҵ���ƿؼ�
 		if (EVENT_CARD_LEFT_HITCARD == dwEvent)
 		{
			if (m_pMyHandCards)
			{
				BYTE byUpCards[MAX_HANDCARDS_NUM];
				int iUpCount = m_pMyHandCards->GetUpCard(byUpCards);
				m_GameMachine.OnHandleHitHandCards(byUpCards,iUpCount);
			}
 		}
		return true;
	}

	//�������Լ�����
	if (pId == IDVB_HIT_CARD)
	{
		if (m_pMyHandCards)
		{
			BYTE byUpCards[MAX_HANDCARDS_NUM];
			int iUpCount = m_pMyHandCards->GetUpCard(byUpCards);

// 			CString str;
// 			str.Format("SJServer��iUpCount = %d",iUpCount);
// 			OutputDebugString(str);

			m_GameMachine.OnHandleHitHandCards(byUpCards,iUpCount);
		}
		return true;
	}

	//��Ӧѡ���ע��ť
	if(pId >= IDB_CFG_DESK_BASE && pId <= IDB_CFG_DESK_BASE + 5)
	{
		m_GameMachine.OnHandleCfgDeskFinish(pId - IDB_CFG_DESK_BASE);
		return true;
	}

	//���Ӽ�ʱ
	if (pId >= IDC_CLOCK_BASE && pId <= IDC_CLOCK_BASE + PLAY_COUNT)
	{
		if (pParam->GetInt(EVENT_TIMER_CURTIME) < 6 && m_pGameInfo->bEnableSound)
			;//SetPlaySound(m_SoundEngine,SOUND_WARNING);

		if (pParam->GetInt(EVENT_LPARAM) == EVENT_TIMER_STOPED)
		{
			OnFlashTimer(pParam->GetInt(EVENT_TIMER_CHAIR_INDEX), \
				pParam->GetInt(EVENT_TIMER_EVENT_INDEX));
		}

		return true;
	}

	return __super::OnPostMessage(pId, pParam);
}


//��ʾ�û�����
void CMiniGameEngine::ShowUserTalk(MSG_GR_RS_NormalTalk *pNormalTalk)
{
	TCHAR szMsg[1000];
	//��������
	UserItemStructInExe * pSendUserItem=FindOnLineUser(pNormalTalk->dwSendID);
	if (pSendUserItem==NULL) 
		return ;
	if(pSendUserItem->GameUserInfo.bUserState==USER_WATCH_GAME)
		return ;
	//lstrcpy(szMsg,pNormalTalk->szMessage);	
	int iTemp = -1;
	for (int i = 0;i<PLAY_COUNT;i++)
	{
		if (NULL == m_pUserInfo[i])
		{
			continue;
		}
		if (m_pUserInfo[i]->GameUserInfo.dwUserID == pNormalTalk->dwSendID)
		{
			iTemp = i;
			break;
		}
	}

	if (m_bShowPlayerID || GetMeUserInfo()->dwUserID == pNormalTalk->dwSendID)
	{
		//lstrcpy(szMsg,pNormalTalk->szMessage);	
		//wsprintf(szMsg,"%d:%s",pNormalTalk->dwSendID,pNormalTalk->szMessage);
		CString strUserID;
		if (iTemp>=0&&SUPER_PLAYER == m_pUserInfo[iTemp]->GameUserInfo.bGameMaster)
		{
			//GetRandUserID(strUserID,iTemp);
			wsprintf(szMsg,"%s:%s",strUserID,pNormalTalk->szMessage);
		}
		else
		{
			wsprintf(szMsg,"%d:%s",pNormalTalk->dwSendID,pNormalTalk->szMessage);
		}
	}
	else
	{
		wsprintf(szMsg,"���:%s",pNormalTalk->szMessage);

	}

	for (int i = 0; i<PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] == NULL)
		{
			continue;
		}

		if (m_pUserInfo[i]->GameUserInfo.dwUserID == pNormalTalk->dwSendID)
		{
			//�Ӵ������� JianGK.20100910
			if(pNormalTalk && m_pGameInfo->bEnableSound)
			{
				char soundfile[1024];

				if (m_pUserInfo[i]->GameUserInfo.bBoy)
				{
					wsprintf(soundfile,"Sound\\hanhua\\male\\%s.ogg",pNormalTalk->szMessage);
				}
				else
				{
					wsprintf(soundfile,"Sound\\hanhua\\female\\%s.ogg",pNormalTalk->szMessage);
				}

				if(m_pSESpeak)
				{
					m_pSESpeak->play2D(soundfile);
				}
			}
			break;
		}
	}

	ShowMsgInGame(szMsg);
	return ;
}


//�û��뿪
bool CMiniGameEngine::GameUserLeft(BYTE bDeskStation, UserItemStructInExe * pUserItem, bool bWatchUser)
{
	CString str;
	str.Format("dxh: Client ���:%d �˳�", bDeskStation);
	OutputDebugString(str);

	bool bRet = __super::GameUserLeft(bDeskStation,pUserItem,bWatchUser);
	if (!bRet)
	{
		return false;
	}

	//����뿪��ȡ��������뿪����
	/*if(!bWatchUser)
	{
		ShowUserName();
	}*/

	if(pUserItem == NULL)
		return false;

	//��������Ϸ�����뿪
	if(m_GameMachine.m_byMeStation ==bDeskStation&&!bWatchUser && TY_MATCH_GAME == GetComType())
	{
		CloseAllLogo();
		OnCancel();
	}

	// �����뿪��Ϸ��
	if(m_GameMachine.m_byMeStation != bDeskStation && !bWatchUser /*&& TY_MATCH_GAME == GetComType()*/)
	{
		CloseAllLogo(bDeskStation);	
		ShowControl(IDC_MONEY + ViewStation(bDeskStation),					SW_HIDE);	// ����׼��/����������
		ShowControl(IDC_JIN_BI + ViewStation(bDeskStation),					SW_HIDE);	// ���ش�����UIͼƬ
		ShowControl(IDC_READY + ViewStation(bDeskStation),					SW_HIDE);	// ����׼��ͼƬ
		ShowControl(IDC_USER_LOGO + ViewStation(bDeskStation),				SW_HIDE);	// �������LOGOͼƬ
		ShowControl(IDC_UDER_ID + ViewStation(bDeskStation),				SW_HIDE);	// �������ID
		ShowControl(IDPIC_BACK_CARD_BASE + ViewStation(bDeskStation),		SW_HIDE);	// ������ҵ���
		ShowControl(IDC_BET + ViewStation(bDeskStation),					SW_HIDE);	// ���������עͼƬ
		ShowControl(IDC_CALL+ ViewStation(bDeskStation),					SW_HIDE);	// ������Ҹ�עͼƬ
		ShowControl(IDC_FOLD + ViewStation(bDeskStation),					SW_HIDE);	// �����������ͼƬ
		ShowControl(IDC_CHECK + ViewStation(bDeskStation),					SW_HIDE);	// ������ҹ���ͼƬ
		ShowControl(IDC_RAISE + ViewStation(bDeskStation),					SW_HIDE);	// ������Ҽ�עͼƬ
		ShowControl(IDC_ALLIN + ViewStation(bDeskStation),					SW_HIDE);	// �������ȫ��ͼƬ

		ShowControl(IDC_USER_CHAIR + ViewStation(bDeskStation),				SW_SHOW);	// ��ʾ�������

		return bRet;
	}

	//if (!bWatchUser)//&&(bDeskStation==0))
	//{
	//	if (GetStationParameter() > GS_WAIT_ARGEE)
	//	{
	//		this->SetStationParameter(GS_WAIT_ARGEE);
	//		ResetGameStation(RS_ALL_VIEW);
	//	}
	//}

	//�Թ���Ҳ�뿪��Ϸ��
	//if(m_GameMachine.m_byMeStation==bDeskStation&&!bWatchUser)
	//{
	//	CloseAllLogo();
	//	OnCancel();
	//}

	return bRet;
}

//�û�����
bool CMiniGameEngine::GameUserCome(BYTE bDeskStation, UserItemStructInExe * pUserItem, bool bWatchUser)
{
	if(!m_hWnd)
		return FALSE;
	if (! __super::GameUserCome(bDeskStation, pUserItem, bWatchUser))
	{
		return FALSE;
	}

	//if (GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID)
	//{
	//	// �������Լ�����λ�õı�����ֵ������Ӧ�Զ���������
	//	m_GameMachine.m_byMeStation = bDeskStation;
	//}
	if(!bWatchUser)
	{
		//��ʾ�û�Logo
		ShowLogo();

		//��ʾ�û�����ID��IP�ȵ�...
		ShowUserInfo();
	}

	CString str;
	str.Format("dxh: �Լ�ID: %d(λ��:%d),���: %d ������(λ��:%d)", GetMeUserInfo()->dwUserID,  GetMeUserInfo()->bDeskStation, pUserItem->GameUserInfo.dwUserID, bDeskStation);
	OutputDebugString(str);

	if( GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID && (!bWatchUser) )
	{
		// ���������Ƶ���ڣ��ͷ���
		if((m_pGameInfo->dwRoomRule & GRR_VIDEO_ROOM)==0)
			return true;
	}
	return TRUE;
}

bool CMiniGameEngine::OnGameTimer( BYTE bDeskStation, UINT uTimeID, UINT uTimeCount )
{
	return false;
}

bool CMiniGameEngine::AFCCloseFrame()
{
	return __super::AFCCloseFrame();
}

//�˳�
void CMiniGameEngine::OnCancel()
{
	if (!OnCloseGame())
	{
		return;
	}

	//SetTimer(110, 1, NULL);

	AFCCloseFrame();

	__super::OnCancel();

	return;
}

/*-----------------------------------------------------------------------------------------------------------
		����һϵ���������к����������ô������Ϸ�Ŀ�������ÿ��������ʵ������Ϸ���졣
	����ͨ����ʾ��״̬����ʱ����Ч��ͨѶ����Դ���յȷ��档
  ----------------------------------------------------------------------------------------------------------*/

///��ʾ���ͷ��
///
void CMiniGameEngine::ShowLogo()
{
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_pUserInfo[i] != NULL)
		{
			BYTE bViewStation =  ViewStation(i);
			BzImage *pImage = dynamic_cast<BzImage*>(GetControlById(IDC_USER_LOGO + bViewStation));
			if (pImage)
			{
				UINT LogoId = m_pUserInfo[i]->GameUserInfo.bLogoID;
				TCHAR path[MAX_PATH];
				CString LeftOrRight; //;

				//�ұ�1��λ�õ��Ǹ�ͷ����һ��
				switch (bViewStation)
				{
				case 3:  LeftOrRight ="left.bmp";    break;
				case 7:  LeftOrRight ="right.bmp";   break;
				default: LeftOrRight = "logo.bmp";   break;	
				}

				//�ұ�1��λ�õ��Ǹ�ͷ����һ��
				if (m_pUserInfo[i]->GameUserInfo.bBoy)
				{
					wsprintf(path,"resources\\log\\boy_%d\\%s",LogoId,LeftOrRight);
				}
				else
				{
					wsprintf(path,"resources\\log\\girl_%d\\%s",LogoId,LeftOrRight);
				}

				if (NULL != m_pUserInfo[i])
				{
					pImage->SetImage(path);
					pImage->ShowWindow(SW_SHOW);
					ShowControl(IDC_USER_CHAIR + bViewStation, SW_HIDE);
					pImage->Invalidate();
				}
			}		
		}
		else
		{
			CloseAllLogo(i);
		}
	}
}

//�����������ʾ
void CMiniGameEngine::ShowUserInfo(void)
{
	//RedrawWindow();

	char szText[256];
	for (BYTE i = 0; i < PLAY_COUNT; ++i)
	{
		int nView = ViewStation(i);
		BzImage* pImagBak = dynamic_cast<BzImage*>(GetControlById(IDC_USER_BASE_MAP + nView));
		BzImage* pImageTips = dynamic_cast<BzImage*>(GetControlById(IDPIC_TIPS_BASE + nView));

		// ��ʾ��ͼ
		if (pImagBak != NULL)
		{
			if (NULL != m_pUserInfo[i])
			{
				ShowControl(IDC_USER_BASE_MAP + nView,SW_SHOW);
				pImagBak->Invalidate();
			}
			else
			{
				ShowControl(IDC_USER_BASE_MAP + nView,SW_HIDE);
				pImagBak->ShowWindow(SW_HIDE);
			}
		}

		CTransparentStatic* pStatic = NULL;

		pStatic = dynamic_cast<CTransparentStatic*>(GetControlById(IDC_USER_THIS_ID));
		if (pStatic)
		{
			CString strID;
			strID.Format("%d", GetMeUserInfo()->dwUserID);
			pStatic->SetMyText(strID.GetBuffer());
			pStatic->ShowWindow(SW_SHOW);
		}
		
		// ��ʾ���ID
		CTransparentStatic *pName = dynamic_cast<CTransparentStatic*>(GetControlById(IDC_UDER_ID + nView));

		if (NULL != pName)
		{
			if (NULL != m_pUserInfo[i])
			{
				if ( m_GameMachine.m_byMeStation == i || m_bShowPlayerID )
				{
					if (SUPER_PLAYER == m_pUserInfo[i]->GameUserInfo.bGameMaster)
					{
						wsprintf(szText, "%d", m_pUserInfo[i]->GameUserInfo.dwUserID);
						pName->SetMyText(szText);						
					}
					else
					{
						wsprintf(szText, "%d", m_pUserInfo[i]->GameUserInfo.dwUserID);
						pName->SetMyText(szText);						
					}
				}
				else
				{
					wsprintf(szText, "%s","���");
					pName->SetMyText(szText);
				}

			}
			else
			{
				pName->SetMyText("");
			}
			pName->ShowWindow(SW_SHOW);
		}

		
		//��ʾIP��ַ��������
		CTransparentStatic *pUserIP = dynamic_cast<CTransparentStatic*>(GetControlById(IDC_ADDRESS));
		if (pUserIP)
		{
			if (NULL != m_pUserInfo[i])
			{
				::wsprintf(szText,"%s",GetIPLocation(m_pUserInfo[i]->GameUserInfo.dwUserIP));
				pUserIP->SetMyText(szText);
				pUserIP->ShowWindow(SW_SHOW);
			}
		}

		//��ʾ��ʣ���ٽ��
		CTransparentStatic *pMyMoney = dynamic_cast<CTransparentStatic*>(GetControlById(IDC_REAL_MONEY));
		if (pMyMoney)
		{
			if (NULL != m_pUserInfo[i])
			{
				CString strMoney;
				strMoney.Format("%d", GetMeUserInfo()->dwMoney);
				pMyMoney->SetMyText(strMoney.GetBuffer());
				pMyMoney->ShowWindow(SW_SHOW);
				pMyMoney->Invalidate();
			}
		}

		//��ʾ��ʣ���ٽ��
		BzImage *pMoneyMage = dynamic_cast<BzImage*>(GetControlById(IDC_REAL_DINAR));
		if (pMoneyMage)
		{
			if (NULL != m_pUserInfo[i])
			{
				CString strMoney;
				strMoney.Format("%d", GetMeUserInfo()->dwMoney);
				pMoneyMage->SetMyText(strMoney.GetBuffer());
				pMoneyMage->ShowWindow(SW_SHOW);
				pMoneyMage->Invalidate();
			}
		}

		//��ʾ���UIͼƬ
		BzImage *pImageMap = dynamic_cast<BzImage*>(GetControlById(IDC_DINAR_MAP));
		if (pImageMap)
		{
			if (NULL != m_pUserInfo[i])
			{
				pImageMap->SetImage("resources\\shengji\\shengji\\jb.bmp");
				ShowControl(IDC_DINAR_MAP, SW_SHOW);
				pImageMap->Invalidate();
			}
		}

		//��ʾ��ʣ���ٽ��
		BzImage *pImageTo = dynamic_cast<BzImage*>(GetControlById(IDC_MONEY));
		if (pImageTo)
		{
			if (NULL != m_pUserInfo[i])
			{
				::wsprintf(szText,"%d",m_pUserInfo[i]->GameUserInfo.dwMoney);
				pImageTo->SetMyText(szText);
				pImageTo->ShowWindow(SW_SHOW);
				pImageTo->Invalidate();
			}
		}

		//һЩװ��
		if (NULL != m_pUserInfo[i])
		{
			ShowControl(IDPIC_COINT_BASE + nView,SW_SHOW);
		}
		else
		{
			ShowControl(IDPIC_COINT_BASE + nView,SW_HIDE);
		}
	}
}

///�ر��������ͷ��
///@param BYTE bDesk Ҫ�رյ�ͷ��ID��255�����������ʱ��ʾ�ر�����
///@return void
void CMiniGameEngine::CloseAllLogo(BYTE bDesk)
{
	if (bDesk >= PLAY_COUNT)
	{
		for (int i=0; i<PLAY_COUNT; i++)
		{
			BzMyAnimator *pAnimal = dynamic_cast<BzMyAnimator *>(GetControlById(IDC_USER_LOGO + i));
			if (pAnimal)
			{
				pAnimal->Stop();
				pAnimal->ShowWindow(SW_HIDE);
			}
		}
	}
	else
	{
		BzMyAnimator *pAnimal = dynamic_cast<BzMyAnimator *>(GetControlById(IDC_USER_LOGO+ViewStation(bDesk)));
		if (pAnimal)
		{
			pAnimal->Stop();
			pAnimal->ShowWindow(SW_HIDE);
		}
	}
}

//�ؼ���ʾ������
void CMiniGameEngine::DispalyAllControl(int nShow)
{
	ShowControl(DDZ_LIAOTIANJILU,nShow);					// ������Ϣ

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		ShowControl(IDC_USER_LOGO		+ i, nShow);		// ���Logo
		ShowControl(IDC_UDER_ID			+ i, nShow);		// ���ID
		ShowControl(IDC_SHOW_CARD		+ i, nShow);		// ������ƿؼ�
		ShowControl(IDC_CLOCK_BASE		+ i, nShow);		// ʱ��
		ShowControl(IDPIC_QIPAO_BASE	+ i, nShow);		// ������ʾ����
		ShowControl(IDT_QIPAO_BASE		+ i, nShow);		// ������ʾ
	}

	ShowControl(DDZ_LIAOTIAN_LIEBIAO,nShow);				// �б�ؼ�
	ShowControl(DDZ_FASONG,nShow);							// ���Ͱ�ť
	ShowControl(DDZ_STOP_MUSIC,nShow);						// ��Ч��
	ShowControl(DDZ_LABA,nShow);							// ������


	//��ע��ťȺ
	for(int i = 0; i < 5; ++i)
	{
		ShowControl(IDB_BUTTON_MONEY + i,nShow);
	}

	return;

}

// ��ID�ű�ʶ��ʱ�������ã�����Ϊλ�ã����ʱ������;
void CMiniGameEngine::ShowFlashTimer(BYTE bDeskStation, UINT uMaxTime, UINT uTimeID)
{
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		int nView = ViewStation(i);
		if (!m_pClocks[nView])
		{
			continue;
		}

		if (bDeskStation == i)
		{
			m_pClocks[nView]->SetCharIndex(bDeskStation);
			m_pClocks[nView]->SetEventIndex(uTimeID);
			m_pClocks[nView]->SetNumber(uMaxTime);
			m_pClocks[nView]->StartTiming();
			m_pClocks[nView]->ShowWindow(SW_SHOW);

			m_iClockCounter = uMaxTime;
			SetTimer(IDT_CLOCK,1000,NULL);
		}
		else
		{
			;//m_pClocks[nView]->ShowWindow(SW_HIDE);
		}
	}
	int nView = ViewStation(bDeskStation);

	//��ȡ���logo�ؼ�����
	Node *pNode = NULL;
	pNode =  GetNodeById(IDC_USER_LOGO + MY_VIEWSTATION);
	if (NULL == pNode)
	{
		return;
	}

	if (m_uTimerFlashID[MY_VIEWSTATION] == 0)
	{
		m_uTimerFlashID[MY_VIEWSTATION] = m_pFlashManager->CreateAndPlayFlash(this,m_uTimerFlashID[MY_VIEWSTATION],
			CPoint(pNode->startx - 28,pNode->starty - 22),"hallAnima\\hallAnimaCfg.xml",4,NULL);
	}
}

// ʱ�䶯����������ñ��������������жϸ���ʲô��������
void CMiniGameEngine::OnFlashTimer(BYTE bDeskStation, UINT uEventID)
{
	switch(uEventID)
	{
	case IDT_TIMEOUT_CFG_DESK:
		{
			//���õ�ע��ʱ����
			m_GameMachine.OnTimeoutCfgDesk(bDeskStation);
		}
		break;
	case IDT_TIMEOUT_WAIT_CONFIG_NOTE:
		{
			//�ȴ����õ�ע��ʱ����
			m_GameMachine.OnTimeoutWaitCfgNote();
		}
		break;
	case IDT_TIMEOUT_WAIT_AGREE:
		{
			//�ȴ�ͬ���ע��ʱ����
			m_GameMachine.OnTimeoutAgreeNote(bDeskStation);
		}
		break;
	case IDT_TIMEOUT_NO_PLAYER:
		{
			//û����ҵȴ��˳��ĳ�ʱ����
			OnCancel(); 
		}
		break;
	case IDT_TIMEOUT_NEXT_ROUND_BEGIN:
		{
			//�ȴ���һ�غϿ�ʼʱ���ù���
			m_GameMachine.OnTimeoutNextRoundBegin(bDeskStation);
		}
		break;
	default:
		break;
	}
}

//ֹͣ�����ض�ʱ��
void CMiniGameEngine::StopAndHideFlashTimer(BYTE bDeskStation)
{
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		int nView = ViewStation(i);
		if (!m_pClocks[nView])
		{
			continue;
		}

		if (bDeskStation == i)
		{
			m_pClocks[nView]->StopTiming();
			m_pClocks[nView]->SetNumber(0);
			m_pClocks[nView]->ShowWindow(SW_HIDE);

			KillTimer(IDT_CLOCK);
		}
		else
		{
			;//m_pClocks[nView]->ShowWindow(SW_SHOW);
		}
	}
}

// ��������������Flash����������Ϣ
LRESULT CMiniGameEngine::FlashEndMessage(WPARAM wParam, LPARAM lParam)
{
	FlashMsgData *pFlashMsg = (FlashMsgData *)lParam;
	if (NULL == pFlashMsg)
	{
		return 0;
	}

	// ���ƶ���
// 	if (pFlashMsg->uFlashID== m_uSendCardFlashID)
// 	{
// 
// 		if(0 == strcmp(pFlashMsg->sFlashCmd, "QuitFlash"))
// 		{
// 			m_pFlashManager->DeleteFlashWidthID(m_uSendCardFlashID);
// 			m_uSendCardFlashID = 0;
// 		}
// 		else
// 		{
// 			
// 			m_pFlashManager->DeleteFlashWidthID(m_uSendCardFlashID);
// 			//::ShowWindow(hwndFlash, SW_HIDE);
// 		}
// 
// 		//��ʾ�շ�������
// 		ShowNextCardFlashAction();
// 	}

	CString str;
	str.Format("dxh:pFlashMsg->uFlashID = %d %s",pFlashMsg->uFlashID,pFlashMsg->sFlashCmd);
	OutputDebugString(str);

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_uTimerFlashID[i] == pFlashMsg->uFlashID)
		{
			if(0 == strcmp(pFlashMsg->sFlashCmd, "HurryUp"))
			{
			    PlayBaseSound(DZPKSound::BT_ALARM_CLOCK,true);
				//AfxMessageBox("s");
				//SetTimer(IDT_TIME_SOUND, 390, NULL);
				break;
			}
		}
	}


	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_uTimerFlashID[i] == pFlashMsg->uFlashID)
		{
			if(0 == strcmp(pFlashMsg->sFlashCmd, "QuitFlash"))
			{
				KillTimer(IDT_TIME_SOUND);
				if (m_pSEHurryUp)
				{
					m_pSEHurryUp->stopAllSounds();
				}

				SafeCloseFlash(m_uTimerFlashID[i]);

				if (i == MY_VIEWSTATION)
				{
					OnPostMessage(IDB_FOLD,NULL);
				}
				break;
			}
		}
	}

	delete pFlashMsg;
	pFlashMsg = NULL;
	return 0;
}

// ��ȡFlash�е��ƺ�
BYTE CMiniGameEngine::GetFlashCardValue(BYTE bCardValue)
{
	if (bCardValue == 0)
	{
		return 52;
	}
	if (bCardValue == 0x4F) 
	{ 
		return 54; 
	} 
	else if (bCardValue == 0x4E) 
	{ 
		return 53; 
	} 
	int iHs = bCardValue / 16; 
	int iPd = bCardValue % 16; 

	if (iHs < 4 && iPd > 13) 
	{ 
		return 52;   //Ϊ�Ʊ��� 
	} 
	if (iPd < 13) 
	{ 
		return bCardValue - iHs * 3; 
	} 
	else if (iPd == 13) 
	{ 
		return bCardValue - 13 - iHs *3; 
	} 
	return 52; 
}

//�ж��Ƿ�����ʾ�����
bool CMiniGameEngine::IsResultDlgShowing()
{
	return (m_pFinishDlg != NULL);
}

//ͨ����ʾ��
UINT CMiniGameEngine::AFCMessageBox(CString strInfo,CString strTitel, UINT style)
{
	if (NULL == m_pMsgDlg)
	{
		m_pMsgDlg = new CAFCMessageBox();
	}
	else
	{
		m_pMsgDlg->EndModalLoop(IDD_DLG_BTN_CANCLE);
		m_pMsgDlg = new CAFCMessageBox();
	}
	m_pMsgDlg->SetParent(this);

	if (style == DLG_MSG_MB_OK)
	{
		m_pMsgDlg->LoadConfig(IDD_MSG_DLG_RES_CONFIRM);		
	}
	else
	{
		m_pMsgDlg->LoadConfig(IDD_MSG_DLG_RES_SELECT);
	}
	m_pMsgDlg->ShowAllControl(SW_SHOW);

	m_pMsgDlg->SetMessage(strInfo);	

	UINT uRet = m_pMsgDlg->DoModal();

	// ����Ҫ�ǵ�ɾ��
	delete m_pMsgDlg;
	m_pMsgDlg = NULL;

	return uRet;

};

//���������Ч���UI����
void CMiniGameEngine::OnShowCfgSound()
{
	// �����������øı�
	if (m_GameMachine.m_bBgMusic)
	{
		//֪ͨ��Ч����й��������ű�������
		PlayBaseSound(DZPKSound::BT_GAME_MUSIC1, true);
	}
	else
	{
		m_pSEBkMusic->stopAllSounds();
	}

	return;
}


//����������Ч
//@param bDeskStation��λ�����ڲ���������Ů����st_typeҪ˵�Ļ���lt_type�������
void CMiniGameEngine::Speak(BYTE bDeskStation,DZPKSound::SpeakType st_type,DZPKSound::LanguageType lt_type)
{
	if (!m_GameMachine.m_bGameSound)
	{
		return;
	}

	if (st_type < 0 || st_type >= DZPKSound::ST_NUMS)
	{
		return;
	}

	//if (lt_type < 0 || lt_type >= DZPKSound::LT_NUMS)
	//{
	//	return;
	//}

	//�õ�˵���е��Ա�
	TCHAR szSex[16];
	if (m_pUserInfo[bDeskStation] != NULL && m_pUserInfo[bDeskStation]->GameUserInfo.bBoy)
	{
		::strcpy(szSex,_TEXT("male"));
	}
	else
	{
		::strcpy(szSex,_TEXT("female"));
	}

	TCHAR szFileName[MAX_PATH];
	::wsprintf(szFileName,"Sound\\speak\\%s\\%s\\%s",\
		DZPKSound::g_szLangPath[lt_type],szSex,
		DZPKSound::g_szSpeakFiles[st_type][rand() % 4]);

	OutputDebugString(szFileName);

	if (m_pSESpeak)
	{
		m_pSESpeak->play2D(szFileName);
	}

	return;
}

//������Ϸ��Ч
ISound* CMiniGameEngine::PlayBaseSound(DZPKSound::BaseType type, bool bMusic)
{
	// 	if (!m_pGameInfo->bEnableSound)
	// 	{
	// 		return;
	// 	}

	if (type < 0 || type >= DZPKSound::BT_NUMS)
	{
		return NULL;
	}

	TCHAR szFileName[MAX_PATH];
	::wsprintf(szFileName,"Sound//base//%s",DZPKSound::g_szBaseFiles[type]);

	if (m_GameMachine.m_bBgMusic)
	{
		if (m_pSEBkMusic)
		{
			if (DZPKSound::BT_GAME_MUSIC1 == type)
			{
				//AfxMessageBox(szFileName);
				return m_pSEBkMusic->play2D(szFileName,bMusic);
			}
		}
	}

	if (m_GameMachine.m_bGameSound)
	{
		if (m_pSEHurryUp)
		{
			if (DZPKSound::BT_ALARM_CLOCK == type)
			{
				return m_pSEHurryUp->play2D(szFileName,bMusic);
			}
		}

		if (m_pSESpeak)
		{
			return m_pSEBase->play2D(szFileName,bMusic);
		}
	}
	

	return NULL;
}

//����������Ϣ����
int CMiniGameEngine::SendGameData(BYTE bMainID,BYTE bAssistantID, BYTE bHandleCode)
{
	return __super::SendGameData(bMainID,bAssistantID,bHandleCode);
}

//����������Ϣ����
int CMiniGameEngine::SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return __super::SendGameData(pData,uBufLen,bMainID,bAssistantID,bHandleCode);
}

bool CMiniGameEngine::OnCloseGame()
{
	//��ѯ״̬
	BYTE byCurrStation = GetStationParameter();
	if (byCurrStation >= GS_PLAY_GAME && byCurrStation < GS_WAIT_NEXT_ROUND)
	{
		char sz[256];
		wsprintf(sz, "��������Ϸ�У��Ƿ�Ҫ�˳���Ϸ��");
		if (IDD_DLG_BTN_OK != AFCMessageBox(sz,m_pGameInfo->szGameName, DLG_MSG_MB_YESNO))//|MB_DEFBUTTON2|MB_ICONINFORMATION,this)) 
		{
			return false;
		}

		//SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	}

	SafeCloseLoadSelectDlg();
	SafeCloseFinishDlg();
	SafeCloseSoundCfgDlg();

	// ������г���
	ClearAnimateNodes();

	// �رն���
	CloseWiatGroup();

	// �رն�ʱ������
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		SafeCloseFlash(m_uTimerFlashID[i]);
	}

	return true;
}

bool CMiniGameEngine::SafeCloseFinishDlg()
{
	if (!m_pFinishDlg)
	{
		return false;
	}

	HWND hFinish = m_pFinishDlg->GetSafeHwnd();
	if (NULL != hFinish)
	{
		BBzParameter *myParam = new BBzParameter(); 
		::PostMessage(hFinish, EVENT_BZ_GUI, EVENT_CLOSE_MAINFRAME, (LPARAM)myParam); 
	}
	
	return true;
}

//��ȫ�ر��޸�ע��Ի���
bool CMiniGameEngine::SafeCloseLoadSelectDlg()
{
	if (m_pLoadSelectionDlg == NULL)
	{
		return false;
	}

	HWND hWnd = m_pLoadSelectionDlg->GetSafeHwnd();
	if (NULL != hWnd)
	{
		BBzParameter *myParam = new BBzParameter(); 
		::PostMessage(hWnd, EVENT_BZ_GUI, EVENT_CLOSE_MAINFRAME, (LPARAM)myParam); 
	}

	return true;

}

// �޸�ע��
bool CMiniGameEngine::OnLoadSelectionDlg(bool bAuto)
{
	if (m_pLoadSelectionDlg != NULL)
	{
		// ��Ϊ�գ�˵�����ֶ����������Щ�����Զ���ʼ
		m_pLoadSelectionDlg->SetAutoStart();
		return true;
	}

	m_pLoadSelectionDlg = new CLoadSelectionDlg(1,this);

	if (m_pLoadSelectionDlg != NULL)
	{
		if (m_pLoadSelectionDlg->LoadConfig(IDD_LOADSELECTION_DLG))
		{
			m_pLoadSelectionDlg->ShowWindow(SW_SHOW);
			SubMoney subData;
			subData.nMaxMoney = m_GameMachine.m_tagDeskCfg.dz.iSubMaxMoney;		 // ���������
			subData.nMinMoney = m_GameMachine.m_tagDeskCfg.dz.iSubMinMoney;		 // ������С���
			subData.nCurrentMoney = m_GameMachine.m_tagDeskCfg.dz.iSubPlanMoney; // ������
			subData.nSelfMoney = GetMeUserInfo()->dwMoney;						 // ��ȡ�Լ�����Ľ��
			subData.nMinusMoney = m_GameMachine.m_tagDeskCfg.dz.iMinusMoney;	 // �޸�ע��Ӽ����
			subData.bAuto = bAuto;

			m_pLoadSelectionDlg->SetMessage(subData);

			if (m_pLoadSelectionDlg->DoModal() == EVENT_CONTINUE_MAINFRAME)
			{
				// ȷ�ϴ���	
				TSubMoney cmd;
				cmd.nMoney[m_GameMachine.m_byMeStation] = m_pLoadSelectionDlg->m_subMoney.nCurrentMoney;
				cmd.bDeskStation = m_GameMachine.m_byMeStation;
				SendGameData(&cmd, sizeof(cmd), MDM_GM_GAME_NOTIFY, ASS_SUB_MENOY, 0);

				// -----------------------�رն�ʱ�� -----------------------
				BzNumberPanel *pNumPanel = dynamic_cast<BzNumberPanel*>(GetControlById(IDC_CLCKTIME));
				if (pNumPanel != NULL)
				{
					pNumPanel->StopTiming();
					pNumPanel->ShowWindow(SW_HIDE);
				}
				// -----------------------------------------------------------------
			}
		}

		delete m_pLoadSelectionDlg;
		m_pLoadSelectionDlg = NULL;
	}
	return true;
}

//ȡ����Ϸ״̬
int	CMiniGameEngine::GetGameStatus()
{
	if(this)
		return GetStationParameter();
	else
		return 0;
};

//�޸���Ϸ״̬
void CMiniGameEngine::SetGameStatus(BYTE bGameStation)
{
	SetStationParameter(bGameStation);
	return;
}

// ��ȫ�ر��������ÿ�
bool CMiniGameEngine::SafeCloseSoundCfgDlg()
{
	if (!m_pSoundCfgDlg)
	{
		return false;
	}

	HWND hSoundCfgDlg = m_pSoundCfgDlg->GetSafeHwnd();
	if (NULL != hSoundCfgDlg)
	{
		BBzParameter *myParam = new BBzParameter(); 
		::PostMessage(hSoundCfgDlg, EVENT_BZ_GUI, EVENT_CLOSE_MAINFRAME, (LPARAM)myParam); 
	}

	return true;
}

//�رն���
void CMiniGameEngine::SafeCloseFlash(UINT &ruFlashId)
{
	if (ruFlashId)
	{
		m_pFlashManager->DeleteFlashWidthID(ruFlashId);
		ruFlashId = 0;
	}
}

/*----------------------------------------------------------------------------------------------------------------
	һϵ���������Žӿڣ����ڣ�������ʵ��������Ⱦ�����⣨��GameEngine����¶�����ӿڣ�ÿ����Ϸ����ͬ
  ---------------------------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------------------------
	һϵ�н�����µĽӿڵ�ʵ�֣����ڣ�������ʵ��UI�滭�����⣨��GameEngine����¶�����ӿڣ�ÿ����Ϸ����ͬ
  ---------------------------------------------------------------------------------------------------------------*/


//��ʾ���ӻ���
void CMiniGameEngine::OnShowTableEnv()
{
	//�رշ��鶯��
	CloseWiatGroup();

	if (m_GameMachine.m_bBgMusic)
	{
		//֪ͨ��Ч����й��������ű�������
		PlayBaseSound(DZPKSound::BT_GAME_MUSIC1, true);
	}

	//��ʾ�û�Logo
	ShowLogo();

	//��ʾ�û�����ID��IP�ȵ�...
	ShowUserInfo();

	ShowControl(IDC_VICE_CARD,SW_SHOW);						// ��ʾһ����
	ShowControl(DDZ_LIAOTIANJILU,SW_SHOW);					// ������Ϣ
	ShowControl(DDZ_LIAOTIAN_LIEBIAO,SW_SHOW);				// �б�ؼ�0
	ShowControl(DDZ_FASONG,SW_SHOW);						// ���Ͱ�ť
	ShowControl(DDZ_STOP_MUSIC,SW_SHOW);					// ��Ч��
	ShowControl(DDZ_LABA,SW_SHOW);							// ������
	ShowControl(IDB_BET, SW_HIDE);							// ������ע��ť
	ShowControl(IDB_CALL, SW_HIDE);							// ���ظ�ע��ť
	ShowControl(IDB_RAISE, SW_HIDE);						// ���ؼ�ע��ť
	ShowControl(IDB_CHECK, SW_HIDE);						// ���ع��ư�ť
	ShowControl(IDB_FOLD, SW_HIDE);							// �������ư�ť
	ShowControl(IDB_ALL_IN, SW_HIDE);						// ����ȫ��
	ShowControl(IDB_SLIDING_AXLE, SW_HIDE);					// ���ػ�����ע��
	ShowControl(IDB_ADDMONEY,		SW_HIDE);				// ���ؼ�ע��+����ť
	ShowControl(IDB_REDUCE,			SW_HIDE);				// ���ؼ�ע��-����ť
	ShowControl(IDB_CALLMANDATE, SW_HIDE);					// ��ע�йܰ�ť
	ShowControl(IDB_CHECKMANDATE, SW_HIDE);		 			// �����йܰ�ť
	ShowControl(IDB_CALLANYMANDATE, SW_HIDE);	 			// ���κ�ע��ť
	ShowControl(IDB_PASSABANDONMANDATE, SW_HIDE);			// ����/�����йܰ�ť
	ShowControl(IDB_CALLMANDATE1, SW_HIDE);					// ��ע�йܰ�ť
	ShowControl(IDB_CHECKMANDATE1, SW_HIDE);		 		// �����йܰ�ť
	ShowControl(IDB_CALLANYMANDATE1, SW_HIDE);	 			// ���κ�ע��ť
	ShowControl(IDB_PASSABANDONMANDATE1, SW_HIDE);			// ����/�����йܰ�ť

	for (int i = 0; i < 5; i++)
	{
		ShowControl(IDB_BUTTON_MONEY + i, SW_HIDE);			// ���ؽ�ť
	}
	
	for (int  i = 0; i < PLAY_COUNT; i++)
	{
		ShowControl(IDC_TABLE_BOARD_MONEY + i, SW_HIDE);	// ���������� * 8
		ShowControl(IDC_MONEY_MAP + i, SW_HIDE);			// ����������ͼƬ
	}
	

	ShowControl(IDC_HANDCARD_BASE + MY_VIEWSTATION, SW_SHOW);

	return;
}

// �������滷��
void CMiniGameEngine::OnShowDeskTop()
{
	ShowControl(IDB_BET, SW_HIDE);							// ������ע��ť
	ShowControl(IDB_CALL, SW_HIDE);							// ���ظ�ע��ť
	ShowControl(IDB_RAISE, SW_HIDE);						// ���ؼ�ע��ť
	ShowControl(IDB_CHECK, SW_HIDE);						// ���ع��ư�ť
	ShowControl(IDB_FOLD, SW_HIDE);							// �������ư�ť
	ShowControl(IDB_ALL_IN, SW_HIDE);						// ����ȫ�°�ť
	ShowControl(IDB_SLIDING_AXLE, SW_HIDE);					// ���ػ�����ע��
	ShowControl(IDB_ADDMONEY,		SW_HIDE);				// ��ʾ��ע��+����ť
	ShowControl(IDB_REDUCE,			SW_HIDE);				// ��ʾ��ע��-����ť
	ShowControl(IDB_CALLMANDATE, SW_HIDE);					// ���ظ�ע�йܰ�ť
	ShowControl(IDB_CALLANYMANDATE, SW_HIDE);				// ���ظ��κ�ע��ť
	ShowControl(IDB_CHECKMANDATE, SW_HIDE);					// ���ع����йܰ�ť
	ShowControl(IDB_PASSABANDONMANDATE, SW_HIDE);			// ���ع���/�����йܰ�ť
	ShowControl(IDB_CALLMANDATE1, SW_HIDE);					// ���ظ�ע�йܰ�ť
	ShowControl(IDB_CALLANYMANDATE1, SW_HIDE);				// ���ظ��κ�ע��ť
	ShowControl(IDB_CHECKMANDATE1, SW_HIDE);				// ���ع����йܰ�ť
	ShowControl(IDB_PASSABANDONMANDATE1, SW_HIDE);			// ���ع���/�����йܰ�ť
	

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		ShowControl(IDB_BUTTON_MONEY + i, SW_HIDE);				// ���������ť
		ShowControl(IDC_MONEY_MAP + i, SW_HIDE);				// ����������ͼƬ
		ShowControl(IDC_TABLE_BOARD_MONEY + i, SW_HIDE);		// ������������ͼƬ
		ShowControl(IDC_SHOW_IMAGE1 + i, SW_HIDE);				// ���ش�Сäע
		ShowControl(IDC_SHOW_IMAGE + i, SW_HIDE);				// ����ׯ
		ShowControl(IDC_READY + i, SW_HIDE);					// ����׼��ͼƬ
		ShowControl(IDPIC_BACK_CARD_BASE + i, SW_HIDE);			// ���ص���ͼƬ
		
		
		BzImage * pImage  = NULL;
		
		// ����ȫ��ͼƬ
		pImage = dynamic_cast<BzImage *> (GetControlById(IDC_ALLIN + ViewStation(i)));
		if (pImage != NULL)
		{
			ShowControl(IDC_ALLIN + ViewStation(i), SW_HIDE);
			pImage->Invalidate();
		}
		
		// ���ش�Сäע
		pImage = dynamic_cast<BzImage *> (GetControlById(IDC_SHOW_IMAGE1 + ViewStation(i)));
		if (pImage != NULL)
		{
			ShowControl(IDC_SHOW_IMAGE1 + ViewStation(i), SW_HIDE);
			pImage->Invalidate();
		}
		
		// ����ׯ
		pImage = dynamic_cast<BzImage *> (GetControlById(IDC_SHOW_IMAGE + ViewStation(i)));
		if (pImage != NULL)
		{
			ShowControl(IDC_SHOW_IMAGE + ViewStation(i), SW_HIDE);
			pImage->Invalidate();
		}	

		// ��������ͼƬ
		pImage = dynamic_cast<BzImage *> (GetControlById(IDC_SHOW_IMAGE + ViewStation(i)));
		if (pImage != NULL)
		{
			ShowControl(IDC_FOLD + ViewStation(i), SW_HIDE);						
			pImage->Invalidate();
		}	

		// ������עͼƬ
		pImage = dynamic_cast<BzImage *> (GetControlById(IDC_BET + ViewStation(i)));
		if (pImage != NULL)
		{
			ShowControl(IDC_BET + ViewStation(i), SW_HIDE);						
			pImage->Invalidate();
		}
		// ���ظ�עͼƬ
		pImage = dynamic_cast<BzImage *> (GetControlById(IDC_CALL + ViewStation(i)));
		if (pImage != NULL)
		{
			ShowControl(IDC_CALL + ViewStation(i), SW_HIDE);						
			pImage->Invalidate();
		}
		// ���ؼ�עͼƬ
		pImage = dynamic_cast<BzImage *> (GetControlById(IDC_RAISE + ViewStation(i)));
		if (pImage != NULL)
		{
			ShowControl(IDC_RAISE + ViewStation(i), SW_HIDE);						
			pImage->Invalidate();
		}
		// ���ؼ�עͼƬ
		pImage = dynamic_cast<BzImage *> (GetControlById(IDC_CHECK + ViewStation(i)));
		if (pImage != NULL)
		{
			ShowControl(IDC_CHECK + ViewStation(i), SW_HIDE);						
			pImage->Invalidate();
		}	
		// ����������
		pImage = dynamic_cast<BzImage *> (GetControlById(IDC_TABLE_BOARD_MONEY + ViewStation(i)));
		if (pImage != NULL)
		{
			ShowControl(IDC_TABLE_BOARD_MONEY + ViewStation(i), SW_HIDE);						
			pImage->Invalidate();
		}	
	}
	return;
}

//���������ҵ��ƿؼ�
void CMiniGameEngine::OnClearCard()
{
	CUpGradeGameCard* pGameCard = NULL; 
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			pGameCard = dynamic_cast<CUpGradeGameCard *>(GetControlById(IDC_SHOW_BASE_CARD));
			if (pGameCard != NULL)
			{
				pGameCard->SetCard(NULL, NULL, 0, 0);
				ShowControl(IDC_SHOW_BASE_CARD, SW_HIDE);
				pGameCard->Invalidate();
			}

			pGameCard = dynamic_cast<CUpGradeGameCard *>(GetControlById(IDC_SHOW_CARD + ViewStation(m_GameMachine.m_byMeStation)));
			if (pGameCard != NULL)
			{
				pGameCard->SetCard(NULL, NULL, 0, 0);
				ShowControl(IDC_SHOW_CARD + ViewStation(m_GameMachine.m_byMeStation), SW_HIDE);
				pGameCard->Invalidate();
			}


			pGameCard = dynamic_cast<CUpGradeGameCard *>(GetControlById(IDC_SHOW_CARD + ViewStation(i)));
			if (pGameCard != NULL)
			{
				pGameCard->SetCard(NULL, NULL, 0, 0);
				ShowControl(IDC_SHOW_CARD + ViewStation(i), SW_HIDE);
				pGameCard->Invalidate();
			}

			BzImage * pImage = dynamic_cast<BzImage *> (GetControlById(IDC_SHOW_CARDTYPE_NAME + ViewStation(i)));
			if (pImage != NULL)
			{
				ShowControl(IDC_SHOW_CARDTYPE_NAME + ViewStation(i), SW_HIDE);		// ��������ͼƬ
				pImage->Invalidate();
			}
		}

			ShowControl(IDC_YOU_WIN + i, SW_HIDE);					// ������Ӯ��ͼƬ
	}
	return;
}

//���õ�עUI����
bool CMiniGameEngine::OnShowCfgDesk(const TCfgDesk* pNoteSelect)
{
	HideAllNoticeControl();

	ShowControl(IDPIC_DLG_BG,SW_SHOW);

	BzImage* pImageBg = dynamic_cast<BzImage*>(GetControlById(IDPIC_DLG_BG));
	if (pImageBg)
	{
		pImageBg->SetImage(_TEXT("resources\\shengji\\shengji\\notices\\cfg_note.png"));
	}

	TCHAR szDizhu[16];

	for (int i = 0; i < 5; ++i)
	{
		BzButton* pButton = dynamic_cast<BzButton*>(GetControlById(IDB_CFG_DESK_BASE + i));
		if (pButton)
		{
			pButton->EnableWindow(pNoteSelect->dz.bSelects[i]);
			sprintf_s(szDizhu,"%d",pNoteSelect->dz.iLowers[i]);
			pButton->SetText(szDizhu);
			pButton->ShowWindow(SW_SHOW);
		}
	}

	return true;
}

//�ȴ����õ�עUI����
bool CMiniGameEngine::OnShowWaitCfgDesk()
{
	HideAllNoticeControl();

	ShowControl(IDPIC_WAIT_AGREE_DIZHU1,SW_SHOW);

	return true;
}

//ϵͳ�����������ݺ��UI������Ҫ��ʾ��Ϸ����
bool CMiniGameEngine::OnShowSysDeskCfg(const TDeskCfg* pDeskCfg)
{
	CTransparentStatic* pStatic = NULL;
	TCHAR szStatic[16];

	for (int i = 0; i < 5; ++i)
	{
		::wsprintf(szStatic,"%d",pDeskCfg->dz.iLowers[i]);
		BzButton* pButton = dynamic_cast<BzButton*>(GetControlById(IDB_BUTTON_MONEY + i));
		if (pButton)
		{
			pButton->SetText(szStatic);
		}
		ShowControl(IDB_BUTTON_MONEY + i,SW_HIDE);
	}

	return true;
}

//����ע����Ϣ
bool CMiniGameEngine::OnShowDeskCfg(const TDeskCfg* pDeskCfg)
{
	return true;
}

//���´�������Ϣ
bool CMiniGameEngine::OnShowSubMoney(const TSubMoney * pSubMoney)
{
	memcpy(m_iMoneys, pSubMoney->nMoney, sizeof(m_iMoneys));	
	if (pSubMoney->bDeskStation == m_GameMachine.m_byMeStation)
	{
		ShowControl(IDC_READY_GO, SW_SHOW);
		ShowControl(IDB_BEGIN_GAME, SW_HIDE);
	}
	
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			CString str;
			str.Format("dxh: Client %d λ�ô�����: %d", i, pSubMoney->nMoney[i]);
			OutputDebugString(str);
		}

		if (m_pUserInfo[i] != NULL && /*m_iMoneys[i] > 0*/pSubMoney->bIn[i])
		{
			CTransparentStatic * pStatic = dynamic_cast<CTransparentStatic *>(GetControlById(IDC_MONEY + ViewStation(i)));
			if (pStatic)
			{
				CString strnMoney;
				strnMoney.Format("%d", m_iMoneys[i]);
				pStatic->SetMyText(strnMoney.GetBuffer());
				pStatic->ShowWindow(SW_SHOW);
			}

			//��ʾ������UIͼƬ
			ShowControl(IDC_JIN_BI + ViewStation(i), SW_SHOW);

			ShowControl(IDC_READY + ViewStation(i), SW_SHOW);

			ShowControl(IDC_PROMPT, SW_HIDE);
		}
	}
	return true;
}

//���ѡ���ע��ť���UI�仯
void CMiniGameEngine::OnShowClickedBottomCasting()
{
	HideAllNoticeControl();
	return;
}

//�ȴ����õ�ע��ʱUI����
void CMiniGameEngine::OnShowWaitCfgDeskTimeout()
{
	HideAllNoticeControl();
	ShowControl(IDPIC_DLG_BG,SW_SHOW);
	BzImage* pImage = dynamic_cast<BzImage*>(GetControlById(IDPIC_DLG_BG));
	if (pImage)
	{
		pImage->SetImage(_TEXT("resources\\shengji\\shengji\\notices\\set_dizhu_timeout.png"));
	}
	return;
}

//�����������Ƿ�ͬ���ע
void CMiniGameEngine::OnShowClickedAgreeDeskCfg()
{
	HideAllNoticeControl();
	return;
}

//��ʾׯ�ҵı�Ƕ���
void CMiniGameEngine::OnPlayMoveFlagAnim(BYTE bDeskStation,UINT type,bool bHandout)
{
	BYTE byView = ViewStation(bDeskStation);

	//��ʼ�ؼ�����
	Node *pNode = NULL;

	if (bHandout)
	{
		pNode = GetNodeById(type);
		if (NULL == pNode)
		{
			return;
		}
	}
	else
	{
		if (type == IDC_ZHUANG)
		{
			pNode = GetNodeById(IDC_SHOW_IMAGE + byView);
			
		}
		else if (type == IDC_SMALL_BLIND)
		{
			pNode = GetNodeById(IDC_SHOW_IMAGE1 + byView);

		}
		else if (type == IDC_BIG_BLIND)
		{
			pNode = GetNodeById(IDC_SHOW_IMAGE1 + byView);
		}
	}
	
	char szPos[32];

	BzAnimateNode *pNoteAnimate = NULL;
	wsprintf(szPos, "%d;0;%d;%d",0,pNode->Width,pNode->Height);

	if (type == IDC_ZHUANG)
	{
		pNoteAnimate = AddAnimateNodeControl(pNode->startx,pNode->starty,pNode->Width,pNode->Height,RGB(255,0,255),szPos,
			"",RGB(255,255,255),"resources\\shengji\\shengji\\zhuangIco.bmp",this);
		if (m_pFlagAnim[0])
		{
			m_pFlagAnim[0]->ShowWindow(SW_HIDE);
		}
		m_pFlagAnim[0] = pNoteAnimate;
	}
	else if (type == IDC_SMALL_BLIND)
	{
		pNoteAnimate = AddAnimateNodeControl(pNode->startx,pNode->starty,pNode->Width,pNode->Height,RGB(255,0,255),szPos,
			"",RGB(255,255,255),"resources\\shengji\\shengji\\xiao.bmp",this);
		if (m_pFlagAnim[1])
		{
			m_pFlagAnim[1]->ShowWindow(SW_HIDE);
		}
		m_pFlagAnim[1] = pNoteAnimate;
	}
	else if (type == IDC_BIG_BLIND)
	{
		pNoteAnimate = AddAnimateNodeControl(pNode->startx,pNode->starty,pNode->Width,pNode->Height,RGB(255,0,255),szPos,
			"",RGB(255,255,255),"resources\\shengji\\shengji\\da.bmp",this);
		if (m_pFlagAnim[2])
		{
			m_pFlagAnim[2]->ShowWindow(SW_HIDE);
		}
		m_pFlagAnim[2] = pNoteAnimate;
	}

	//��ȡ������Ŀ������

	if (bHandout)
	{
		if (type == IDC_ZHUANG)
		{
			pNode = GetNodeById(IDC_SHOW_IMAGE + byView);
		}
		else if (type == IDC_SMALL_BLIND)
		{
			pNode = GetNodeById(IDC_SHOW_IMAGE1 + byView);
		}
		else if (type == IDC_BIG_BLIND)
		{
			pNode = GetNodeById(IDC_SHOW_IMAGE1 + byView);
		}
	}
	else
	{
		pNode = GetNodeById(type);
	}

	if (NULL == pNode)
	{
		return;
	}

	CRect rcEnd;
	rcEnd.left = pNode->startx;
	rcEnd.top = pNode->starty;
	rcEnd.right = rcEnd.left + 1;
	rcEnd.bottom = rcEnd.top + 1;

	if(pNoteAnimate)
	{
		if(bHandout)
		{
			ShowControl(type,SW_HIDE);
		}
		else
		{
			//ShowControl(type,SW_SHOW);
		}
		
		pNoteAnimate->setIsAlpha(true);
		pNoteAnimate->ShowWindow(SW_SHOW);
		pNoteAnimate->SetEndRect(rcEnd);
		pNoteAnimate->SetSpeed(50);
		pNoteAnimate->Play();
	}

	return;
}

//��ʾׯ�ҡ�Сäע����äע
void CMiniGameEngine::OnShowNTFlag(const TNextRoundBeginInfo* pNextRoundBeginInfo)
{		
	CTransparentStatic* pStatic = NULL;
	TCHAR szStatic[32];
	BzImage * pImage = NULL;

	if (pNextRoundBeginInfo->byNTUser == pNextRoundBeginInfo->bSmallBlind)
	{

		// ��ʾׯ��ͼƬ		
		BYTE iZhang = pNextRoundBeginInfo->byNTUser;
		::wsprintf(szStatic,"%d",iZhang);
		pImage = dynamic_cast<BzImage*>(GetControlById(IDC_SHOW_IMAGE + ViewStation(iZhang)));
		if (pImage != NULL)
		{
			pImage->SetImage("resources\\shengji\\shengji\\zhuangIco.bmp");
		}
		ShowControl(IDC_SHOW_IMAGE + ViewStation(iZhang), SW_SHOW);
		ShowControl(IDC_ZHUANG, SW_HIDE);
		ShowControl(IDC_SMALL_BLIND, SW_HIDE);

		// ��ʾ��äעͼƬ
		BYTE iZhang2 = pNextRoundBeginInfo->bBigBlind;
		::wsprintf(szStatic,"%d",iZhang2);
		pImage = dynamic_cast<BzImage*>(GetControlById(IDC_SHOW_IMAGE1 + ViewStation(iZhang2)));
		if (pImage != NULL)
		{
			pImage->SetImage("resources\\shengji\\shengji\\da.bmp");
		}
		ShowControl(IDC_SHOW_IMAGE1 + ViewStation(iZhang2), SW_SHOW);
		ShowControl(IDC_BIG_BLIND, SW_HIDE);
		
	}
	else
	{
		// ��ʾׯ��ͼƬ		
		BYTE iZhang = pNextRoundBeginInfo->byNTUser;
		::wsprintf(szStatic,"%d",iZhang);
		pImage = dynamic_cast<BzImage*>(GetControlById(IDC_SHOW_IMAGE + ViewStation(iZhang)));
		if (pImage != NULL)
		{
			pImage->SetImage("resources\\shengji\\shengji\\zhuangIco.bmp");
		}
		ShowControl(IDC_SHOW_IMAGE + ViewStation(iZhang), SW_SHOW);
		ShowControl(IDC_ZHUANG, SW_HIDE);
	
		// ��ʾСäעͼƬ
		BYTE iZhang1 = pNextRoundBeginInfo->bSmallBlind;
		::wsprintf(szStatic,"%d",iZhang1);
		pImage = dynamic_cast<BzImage*>(GetControlById(IDC_SHOW_IMAGE1 + ViewStation(iZhang1)));
		if (pImage != NULL)
		{
			pImage->SetImage("resources\\shengji\\shengji\\xiao.bmp");
		}
		ShowControl(IDC_SHOW_IMAGE1 + ViewStation(iZhang1), SW_SHOW);
		ShowControl(IDC_SMALL_BLIND, SW_HIDE);

		// ��ʾ��äעͼƬ
		BYTE iZhang2 = pNextRoundBeginInfo->bBigBlind;
		::wsprintf(szStatic,"%d",iZhang2);
		pImage = dynamic_cast<BzImage*>(GetControlById(IDC_SHOW_IMAGE1 + ViewStation(iZhang2)));
		if (pImage != NULL)
		{
			pImage->SetImage("resources\\shengji\\shengji\\da.bmp");
		}
		ShowControl(IDC_SHOW_IMAGE1 + ViewStation(iZhang2), SW_SHOW);
		ShowControl(IDC_BIG_BLIND, SW_HIDE);
	}

	return;
}

//�յ���ע��Ϣ���UI�仯
bool CMiniGameEngine::OnShowGetBetInfo(const TBet* pBet)
{
	KillTimer(IDT_TIME_SOUND);

	// ������ע��ť
	OnShowOtherPlayerGetToken();

	if (pBet->nType == ET_AUTO || pBet->nType == ET_BET || pBet->nType == ET_ADDNOTE || pBet->nType == ET_ALLIN || pBet->nType == ET_CALL)
	{
		m_GameMachine.m_nBetMoney[pBet->byUser] += pBet->nMoney; 
		m_iMoneys[pBet->byUser] -= pBet->nMoney;
		
		CTransparentStatic * pStatic = dynamic_cast<CTransparentStatic *>(GetControlById(IDC_MONEY + ViewStation(pBet->byUser)));
		if (pStatic != NULL)
		{
			CString strnMoney;
			strnMoney.Format("%d",m_iMoneys[pBet->byUser]);
			pStatic->SetMyText(strnMoney.GetBuffer());
			pStatic->Invalidate();
		}

		// ��������
		PlayBaseSound(DZPKSound::BT_CHIP_MUSIC);
	}

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		SafeCloseFlash(m_uTimerFlashID[i]);
	}

	if (m_pSEHurryUp)
	{
		m_pSEHurryUp->stopAllSounds();
	}

	// ��¼ȫ����������
	if (pBet->nType == ET_ALLIN)
	{
		m_GameMachine.m_bAllBet[pBet->byUser] = true;
	}
	else if (pBet->nType == ET_FOLD)
	{
		m_GameMachine.m_bGiveUp[pBet->byUser] = true;
	}

	if (m_GameMachine.m_byBigBlind == pBet->byUser)
	{
		OutputDebugString("dxh: -------------����ͼƬ���غ��� Client--------------");
		m_GameMachine.OnUserSortOut();
	}

	// ��¼�����Ҳ�������
	m_GameMachine.m_emLastOperation[pBet->byUser] = pBet->nType;

	// ��¼�������
	m_GameMachine.m_stackLastStation.push(pBet->byUser);

	BzImage * pImage = NULL;
	
	switch (pBet->nType)
	{
	case ET_BET:		// ��עͼƬ��ʾ
		{
			pImage = dynamic_cast<BzImage*>(GetControlById(IDC_BET + ViewStation(pBet->byUser)));

			if (pImage != NULL)
			{
				ShowControl(IDC_BET + ViewStation(pBet->byUser), SW_SHOW);
				pImage->Invalidate();
			}
		}
		break;
	case ET_CALL:		// ��עͼƬ��ʾ
		{
			pImage = dynamic_cast<BzImage*>(GetControlById(IDC_CALL + ViewStation(pBet->byUser)));

			if (pImage != NULL)
			{
				ShowControl(IDC_CALL + ViewStation(pBet->byUser), SW_SHOW);
				pImage->Invalidate();
			}

			Speak(pBet->byUser,DZPKSound::ST_CALL);
		}
		break;
	case ET_ADDNOTE:	// ��עͼƬ��ʾ
		{
			pImage = dynamic_cast<BzImage*>(GetControlById(IDC_RAISE + ViewStation(pBet->byUser)));
			if (pImage != NULL)
			{
				ShowControl(IDC_RAISE + ViewStation(pBet->byUser), SW_SHOW);
				pImage->Invalidate();
			}

			Speak(pBet->byUser,DZPKSound::ST_RAISE);
		}
		break;
	case ET_CHECK:		// ����ͼƬ��ʾ
		{
			pImage = dynamic_cast<BzImage*>(GetControlById(IDC_CHECK + ViewStation(pBet->byUser)));
			if (pImage != NULL)
			{
				ShowControl(IDC_CHECK + ViewStation(pBet->byUser), SW_SHOW);
				pImage->Invalidate();
			}
			Speak(pBet->byUser,DZPKSound::ST_CHECK);
		}
		break;
	case ET_FOLD:		// ����ͼƬ��ʾ
		{
			pImage = dynamic_cast<BzImage*>(GetControlById(IDC_FOLD + ViewStation(pBet->byUser)));
			if (pImage != NULL)
			{
				ShowControl(IDC_FOLD +  ViewStation(pBet->byUser), SW_SHOW);
				pImage->Invalidate();

				if (m_GameMachine.m_byNTUser == m_GameMachine.m_bySmallBlind)
				{
					if (pBet->byUser == m_GameMachine.m_byBigBlind)
					{
						OnPlayMoveFlagAnim(pBet->byUser,IDC_BIG_BLIND,false);
					}
					else if (pBet->byUser == m_GameMachine.m_byNTUser)
					{
						OnPlayMoveFlagAnim(pBet->byUser,IDC_ZHUANG,false);
					} 
				}
				else
				{
					if (pBet->byUser == m_GameMachine.m_bySmallBlind)
					{
						OnPlayMoveFlagAnim(pBet->byUser,IDC_SMALL_BLIND,false);
					}
					else if (pBet->byUser == m_GameMachine.m_byNTUser)
					{
						OnPlayMoveFlagAnim(pBet->byUser,IDC_ZHUANG,false);
					} 
					else if (pBet->byUser == m_GameMachine.m_byBigBlind)
					{
						OnPlayMoveFlagAnim(pBet->byUser,IDC_BIG_BLIND,false);
					}
				}		
			}
			Speak(pBet->byUser,DZPKSound::ST_FOLD);
		}
		break;
	case ET_ALLIN:		// ȫ��ͼƬ��ʾ
		{
			pImage = dynamic_cast<BzImage*>(GetControlById(IDC_ALLIN + ViewStation(pBet->byUser)));
			if (pImage != NULL)
			{
				ShowControl(IDC_ALLIN + ViewStation(pBet->byUser), SW_SHOW);
				pImage->Invalidate();
			}
			Speak(pBet->byUser,DZPKSound::ST_ALL_IN);
		}
		break;
	}

	//��ʾ������UIͼƬ
	pImage = dynamic_cast<BzImage*>(GetControlById(IDC_MONEY_MAP + ViewStation(pBet->byUser)));
	if (pImage != NULL && pBet->nType != ET_FOLD)
	{	
		ShowControl(IDC_MONEY_MAP + ViewStation(pBet->byUser), SW_SHOW);
		pImage->Invalidate();
	}
	//else
	//{
	//	ShowControl(IDC_MONEY_MAP + ViewStation(pBet->byUser), SW_HIDE);
	//}

	// ���ý����ʾ

	pImage = dynamic_cast<BzImage*>(GetControlById(IDC_TABLE_BOARD_MONEY + ViewStation(pBet->byUser)));
	if (pImage != NULL)
	{
		CString str;
		str.Format("%d",m_GameMachine.m_nBetMoney[pBet->byUser]);
		pImage->SetMyText(str.GetBuffer());

		if (m_GameMachine.m_nBetMoney[pBet->byUser] <= 0)
		{
			pImage->ShowWindow(SW_HIDE);
			pImage->Invalidate();
		}
		else
		{
			pImage->ShowWindow(SW_SHOW);
			pImage->Invalidate();
		}
	}

	return true;
}

//��ʾ��һ��ҽ�ҵ���ע��ťUI����
bool CMiniGameEngine::OnShowCall()
{
	return true;
}

//�Լ��õ����Ƶ�UI����
bool CMiniGameEngine::OnShowMeGetToken(const TToken * pToken)
{

	PlayBaseSound(DZPKSound::BT_TRANSFER_TOKEN);

    if (pToken->bNewTurn)
    {
        // һ���е�һ��������, ������в�����־
        memset(m_GameMachine.m_emLastOperation, ET_UNKNOWN, sizeof(m_GameMachine.m_emLastOperation));

        // ��ղ����˼�¼ջ
        m_GameMachine.ClearAllStack();
    }
    else
    {
        // �ж��йܲ���
        if (AutoTrusteeship(pToken))
        {
            // �Զ��йܳɹ�
            return true;
        }
    }
    
	BYTE byVerbFlag = pToken->byVerbFlag;

	//int nMoney = GetMeUserInfo()->dwMoney;

	
	//�����ڰ�ť����ʾע��ֵ
	
	//CString str;
	//str.Format("dxh: Client: ��ǰ�������: %d, ����ֵ: 0x:%x", pToken->byUser, byVerbFlag);
	//OutputDebugString(str);

	// �����Զ��йܰ�ť�����ְ�����ʾ
    OnShowPdg(SW_HIDE);
	OnAutoTrusteeship(SW_HIDE);

	if (byVerbFlag & UD_VF_BET)	// ������ע
	{
		EnableControl(IDB_BET,					true);	//��ע��ť����
		EnableControl(IDB_CHECK,				true);	//���ư�ť����
		EnableControl(IDB_FOLD,					true);	//���ư�ť����
		EnableControl(IDB_SLIDING_AXLE,			true);	//�����ᰴť����
		EnableControl(IDB_ADDMONEY,				true);	//��ע��+����ť����
		EnableControl(IDB_REDUCE,				true);	//��ע��-����ť����
		ShowControl(IDB_BET,				SW_SHOW);	//��ʾ��ע��ť����
		ShowControl(IDB_CHECK,				SW_SHOW);	//��ʾ���ư�ť����
		ShowControl(IDB_FOLD,				SW_SHOW);	//��ʾ���ư�ť����
		ShowControl(IDB_SLIDING_AXLE,		SW_SHOW);	//��ʾ�����ᰴť
		ShowControl(IDB_ADDMONEY,			SW_SHOW);	//��ʾ��ע��+����ť
		ShowControl(IDB_REDUCE,				SW_SHOW);	//��ʾ��ע��-����ť

		BzGameScrollbar * pScrollBar = dynamic_cast<BzGameScrollbar *>(GetControlById(IDB_SLIDING_AXLE));
		if (pScrollBar != NULL)
		{
			pScrollBar->SetCurrentProcess(0);
		}

		for (int i = 0; i < 5; i++)
		{
			EnableControl(IDB_BUTTON_MONEY + i,	true);	//����ע��ť����
			ShowControl(IDB_BUTTON_MONEY + i, SW_SHOW);
		}

		m_GameMachine.OnHandleSlidingAxle();
	}
	else
	{
	    int nMoney = m_GameMachine.m_nBetMoney[m_GameMachine.GetLastUserStation(m_GameMachine.m_byMeStation)] - m_GameMachine.m_nBetMoney[m_GameMachine.m_byMeStation];
		if (m_iMoneys[m_GameMachine.m_byMeStation] <= nMoney)
		{
			// ֻ��ȫ��
			EnableControl(IDB_ALL_IN,	true);					//ȫ�°�ť����
			EnableControl(IDB_FOLD,		true);					//���ư�ť����
			ShowControl(IDB_ALL_IN,	SW_SHOW);					//��ʾ��עȫ�°�ť
			ShowControl(IDB_FOLD,	SW_SHOW);					//��ʾ���ư�ť����
		}
		else if (byVerbFlag & UD_VF_ADD)
		{
			EnableControl(IDB_CALL,					true);		//��ע��ť����
			EnableControl(IDB_RAISE,				true);		//��ע��ť����
			EnableControl(IDB_FOLD,					true);		//���ư�ť����
			EnableControl(IDB_SLIDING_AXLE,			true);		//�����ᰴť����
			EnableControl(IDB_ADDMONEY,				true);		//��ע��+����ť����
			EnableControl(IDB_REDUCE,				true);		//��ע��-����ť����
			ShowControl(IDB_CALL,					SW_SHOW);	//��ʾ��ע��ť����
			ShowControl(IDB_RAISE,					SW_SHOW);	//��ʾ��ע��ť����
			ShowControl(IDB_FOLD,					SW_SHOW);	//��ʾ���ư�ť����
			ShowControl(IDB_SLIDING_AXLE,			SW_SHOW);	//��ʾ�����ᰴť����
			ShowControl(IDB_ADDMONEY,				SW_SHOW);	//��ʾ��ע��+����ť����
			ShowControl(IDB_REDUCE,					SW_SHOW);	//��ʾ��ע��-����ť����

			BzGameScrollbar * pScrollBar = dynamic_cast<BzGameScrollbar *>(GetControlById(IDB_SLIDING_AXLE));
			if (pScrollBar != NULL)
			{
				pScrollBar->SetCurrentProcess(0);
			}

			for (int i = 0; i < 5; i++)
			{
				ShowControl(IDB_BUTTON_MONEY + i,	SW_SHOW);	//��ʾ����ע��ť����
				EnableControl(IDB_BUTTON_MONEY + i,	true);		//����ע��ť����
			}

			// ������Ը�ע�Ľ��, �����ø�ע��ť
			m_GameMachine.m_nCallMoney = pToken->nCallMoney;

			// ����ϼҲ����Ƿ���, ��������ϼҵĲ�����ע���
			//stack<BYTE> skTemp(m_GameMachine.m_stackLastStation);
			//if (!skTemp.empty())
			//{
			//	BYTE bLastStation = skTemp.top();
			//	skTemp.pop();

			//	bool bReset = false;

			//	// �����ϼ���ע����ע��ȫ�µ����
			//	while (bReset || (bLastStation != 255 && m_GameMachine.m_emLastOperation[bLastStation] == ET_FOLD))
			//	{
			//		if (!skTemp.empty())
			//		{
			//			// ��ջ��ȡ��һ��Ԫ��
			//			bLastStation = skTemp.top();
			//			skTemp.pop();
			//		}
			//		else
			//		{
			//			// ջΪ��
			//			bLastStation = -1;
			//			break;
			//		}

			//		if (bLastStation == m_GameMachine.m_byMeStation)
			//		{
			//			OutputDebugString("dxh-Client: OnShowMeGetToken û���ҵ��ϼҿɸ�ע����");
			//			bLastStation = -1;
			//			break;
			//		}

			//		// �ж��������Ƿ�ȫ��
			//		if (m_GameMachine.m_emLastOperation[bLastStation] == ET_ALLIN)
			//		{
			//			if (m_GameMachine.m_nBetMoney[bLastStation] <= m_GameMachine.m_nBetMoney[m_GameMachine.m_byMeStation])
			//			{
			//				// �ϼ�ȫ��֮����û�����ϴ���ע�Ľ�Ҷ�, ������ж�
			//				bReset = true;
			//			}
			//		}
			//	}
	
			//	if (bLastStation != 255)
			//	{
			//		// ����Ҫ��ע�Ľ��
			//		m_GameMachine.m_nCallMoney = m_GameMachine.m_nBetMoney[bLastStation] - m_GameMachine.m_nBetMoney[m_GameMachine.m_byMeStation];
			//	}
			//	else
			//	{
			//		OutputDebugString("dxh: ��ע����, ����ջû���ҵ��ϼҲ�����");
			//		m_GameMachine.m_nCallMoney = 0;
			//	}
			//}
			//else
			//{
			//	OutputDebugString("dxh: ��ע����, ����ջΪ��");
			//	m_GameMachine.m_nCallMoney = 0;
			//}


			BzButton *pButton = dynamic_cast<BzButton*>(GetControlById(IDB_CALL));
			if (pButton != NULL)
			{			
				CString strMoney;
				strMoney.Format("    %d",m_GameMachine.m_nCallMoney);
				pButton->SetText(strMoney.GetBuffer());
				pButton->Invalidate();

				CString str;
				str.Format("dxy: UserID :%d",GetMeUserInfo()->dwUserID, m_GameMachine.m_nCallMoney);
				OutputDebugString(str);
			}

			m_GameMachine.OnHandleSlidingAxle();
		}
	}

	//��˸����
	FlashWindowEx(FLASHW_TIMERNOFG|FLASHW_TRAY,1,0);

	//��ȡ���logo�ؼ�����
	Node *pNode = NULL;
	pNode =  GetNodeById(IDC_USER_LOGO + MY_VIEWSTATION);
	if (NULL == pNode)
	{
		return false;
	}

	if (m_uTimerFlashID[MY_VIEWSTATION] == 0)
	{
		m_uTimerFlashID[MY_VIEWSTATION] = m_pFlashManager->CreateAndPlayFlash(this,m_uTimerFlashID[MY_VIEWSTATION],
			CPoint(pNode->startx - 31,pNode->starty - 20),"hallAnima\\hallAnimaCfg.xml",4,NULL);
	}

	return true;
}

//�Զ��йܴ���
bool CMiniGameEngine::AutoTrusteeship(const TToken * pToken)
{
    if (!m_GameMachine.m_stackLastStation.empty())
    {
		// ������Ը�ע�Ľ��, �����ø�ע��ť
		m_GameMachine.m_nCallMoney = 0;

		// ����ϼҲ����Ƿ���, ��������ϼҵĲ�����ע���
		stack<BYTE> skTemp(m_GameMachine.m_stackLastStation);
		if (!skTemp.empty())
		{
			BYTE bLastStation = skTemp.top();
			skTemp.pop();

			// �����ϼ���ע����ע��ȫ�µ����
			while (bLastStation != 255 && m_GameMachine.m_emLastOperation[bLastStation] == ET_FOLD)
			{
				if (!skTemp.empty())
				{
					// ��ջ��ȡ��һ��Ԫ��
					bLastStation = skTemp.top();
					skTemp.pop();
				}
				else
				{
					// ջΪ��
					bLastStation = -1;
					break;
				}
			}

			if (bLastStation != 255)
			{
				// ����Ҫ��ע�Ľ��
				m_GameMachine.m_nCallMoney = m_GameMachine.m_nBetMoney[bLastStation] - m_GameMachine.m_nBetMoney[m_GameMachine.m_byMeStation];
			}
			else
			{
				OutputDebugString("dxh: AutoTrusteeship ��ע����, ����ջû���ҵ��ϼҲ�����");
			}
		}
		else
		{
			OutputDebugString("dxh: AutoTrusteeship ��ע����, ����ջΪ��");
		}

        stack<BYTE> skTemp2(m_GameMachine.m_stackLastStation);
        BYTE bLastStation = skTemp2.top();
        skTemp2.pop();

        if (bLastStation != 255)
        {
            if (pToken->byVerbFlag & UD_VF_BET)	// ������ע
            {
                 // �ڴ˽����Զ��йܹ������Զ��й������ж�
                if (m_GameMachine.m_emLastOperation[bLastStation] == ET_CHECK)
                {
                    // �Զ�����, �ϼ���������²�
                    if (m_GameMachine.m_bTrusteeship[2] || m_GameMachine.m_bTrusteeship[3])
                    {
                        // �����Զ����Ʋ���
                        m_GameMachine.m_bTrusteeship[2] = false;  // (ֻ��һ��)

                        OutputDebugString("dxh: ִ���Զ����Ʋ���");

                        m_GameMachine.OnHandleCheck();
                        return true;
                    }
                }
                else if (m_GameMachine.m_emLastOperation[bLastStation] == ET_FOLD)
                {
                    // �Զ�����
                    if (m_GameMachine.m_bTrusteeship[3])
                    {
                        // �����Զ����Ʋ���
                        OutputDebugString("dxh: ִ���Զ����Ʋ���");

                        m_GameMachine.OnHandleFolo();
                        return true;
                    } 
                }
            }
            else
            {
                // �Զ���ע(ֻ��һ��) �� �Զ����κ�ע(ÿ�ζ���)
                if (m_GameMachine.m_bTrusteeship[0] || m_GameMachine.m_bTrusteeship[1])
                {
                    // �ڴ˽����Զ���ע����
                    OutputDebugString("dxh: ִ���Զ���ע����");
                    m_GameMachine.m_bTrusteeship[0] = false;

                    m_GameMachine.OnHandleCall();
                    return true;
                }
                else if (m_GameMachine.m_bTrusteeship[3])
                {
                    // �Զ�����/����
                    if (m_GameMachine.m_emLastOperation[bLastStation] == ET_FOLD)
                    {
                        // �����Զ����Ʋ���
                        OutputDebugString("dxh: ִ���Զ����Ʋ���2");

                        m_GameMachine.OnHandleFolo();
                        return true;
                    }
                }
            }
        }
        else
        {
            OutputDebugString("dxh: Error: �ǵ�һ������, �������˵�ջΪ��");
        }
    }

    return false;
}

//������ҵõ����Ƶ�UI����
bool CMiniGameEngine::OnShowOtherPlayerGetToken(const TToken * pToken)
{
	ShowControl(IDB_SLIDING_AXLE,		SW_HIDE);	//��ʾ�����ᰴť����
	ShowControl(IDB_ADDMONEY,			SW_HIDE);	//��ʾ��ע��+����ť����
	ShowControl(IDB_REDUCE,				SW_HIDE);	//��ʾ��ע��-����ť����
	ShowControl(IDB_CUE,  		 		SW_HIDE);	// ��ʾ��ʾ��ť
	ShowControl(IDB_BET,				SW_HIDE);	// ��ע��ť
	ShowControl(IDB_CALL,				SW_HIDE);	// ��ע��ť
	ShowControl(IDB_RAISE,				SW_HIDE);	// ��ע��ť
	ShowControl(IDB_CHECK,				SW_HIDE);	// ���ư�ť
	ShowControl(IDB_FOLD,				SW_HIDE);	// ���ư�ť
	ShowControl(IDB_ALL_IN,				SW_HIDE);	// ȫ��
	ShowControl(IDB_SLIDING_AXLE,		SW_HIDE);	// ������
	ShowControl(IDB_ADDMONEY,			SW_HIDE);	// ��ע��+����ť
	ShowControl(IDB_REDUCE,				SW_HIDE);	// ��ע��-����ť

	if (pToken)
	{
		BYTE byView = ViewStation(pToken->byUser);

		//��ȡ���logo�ؼ�����
		Node *pNode = NULL;
		pNode =  GetNodeById(IDC_USER_LOGO + byView);
		if (NULL == pNode)
		{
			return false;
		}

		if (m_uTimerFlashID[byView] == 0)
		{
			m_uTimerFlashID[byView] = m_pFlashManager->CreateAndPlayFlash(this,m_uTimerFlashID[byView],
				CPoint(pNode->startx - 31,pNode->starty - 20),"hallAnima\\hallAnimaCfg.xml",4,NULL);
		}
	}

	for (int i = 0; i < 5; i++)
	{
		ShowControl(IDB_BUTTON_MONEY + i,SW_HIDE);			// ��ť
	}

	return true;
}

//�йܰ�ť��ʾ/����
void CMiniGameEngine::OnAutoTrusteeship(int nCmdShow)
{
	ShowControl(IDB_CALLMANDATE,	SW_HIDE);
	ShowControl(IDB_CALLMANDATE1,	SW_HIDE);	
	ShowControl(IDB_CALLANYMANDATE,	SW_HIDE);
	ShowControl(IDB_CALLANYMANDATE1,SW_HIDE);	
	ShowControl(IDB_CHECKMANDATE,	SW_HIDE);	
	ShowControl(IDB_CHECKMANDATE1,	SW_HIDE);	
	ShowControl(IDB_PASSABANDONMANDATE,	SW_HIDE);	
	ShowControl(IDB_PASSABANDONMANDATE1,SW_HIDE);	

	// ��ע�йܰ�ť
	if (m_GameMachine.m_bTrusteeship[0])
	{
		ShowControl(IDB_CALLMANDATE1,	nCmdShow);			
	}
	else
	{
		ShowControl(IDB_CALLMANDATE,	nCmdShow);
	}

	// �����κ��йܰ�ť
	if (m_GameMachine.m_bTrusteeship[1])
	{
		ShowControl(IDB_CALLANYMANDATE1,nCmdShow);	    
	}
	else
	{
		ShowControl(IDB_CALLANYMANDATE,	nCmdShow);
	}

	// �����йܰ�ť
	if (m_GameMachine.m_bTrusteeship[2])
	{
		ShowControl(IDB_CHECKMANDATE1,	nCmdShow);	        
	}
	else
	{
		ShowControl(IDB_CHECKMANDATE,	nCmdShow);
	}

	// ����/�����йܰ�ť
	if (m_GameMachine.m_bTrusteeship[3])
	{
		ShowControl(IDB_PASSABANDONMANDATE1,nCmdShow);   
	}
	else
	{
		ShowControl(IDB_PASSABANDONMANDATE,	nCmdShow);
	}
}

//������ҵõ����Ƶ�UI����
bool CMiniGameEngine::OnShowPdg(int nCmdShow)
{
	for (int i = 0; i < 9; i++)
	{
		ShowControl(IDC_GAME_HELP + i,	SW_HIDE);	
	}

	ShowControl(IDC_GAME_HELP + (rand()%9), nCmdShow);	

	return true;
}

//����UI����ӿ�
bool CMiniGameEngine::OnShowCollectJotens(BYTE bDeskStation)
{
	BYTE nView = ViewStation(bDeskStation);
	//���Ŷ���

	//��ȡ���ƵĿؼ�����
	Node *pNode = NULL;
	pNode = GetNodeById(IDC_MONEY_MAP + nView);
	ShowControl(IDC_MONEY_MAP + nView,SW_HIDE);
	ShowControl(IDC_TABLE_BOARD_MONEY + nView,SW_HIDE);

	if (NULL == pNode)
	{
		return false;
	}

	char szPos[32];
	TCHAR szMoney[16];
	_stprintf(szMoney, "%d", m_GameMachine.m_nBetMoney[bDeskStation]);

	BzAnimateNode *pAnimate = NULL;
	wsprintf(szPos, "%d;0;%d;%d",nView*41,41,33);

	pAnimate = AddAnimateNodeControl(pNode->startx, pNode->starty,41, 33, RGB(255,0,255), szPos,
		szMoney, RGB(255,255,0), "resources\\shengji\\shengji\\cm.bmp", this);
	m_pJetonAnim[nView] = pAnimate;

	

	PlayBaseSound(DZPKSound::BT_CHIP_MONEY_MUSIC);

	//��ȡ������Ŀ������

	// 	CString str;
	// 	str.Format("DZPKClient:%d,%d,%d,%d",pNode->startx,pNode->starty,\
	// 	pNode->Width,pNode->Height);
	// 	OutputDebugString(str);

	CRect rcEnd;
	rcEnd.left = 380;
	rcEnd.top = 300;
	rcEnd.right = rcEnd.left + 1;
	rcEnd.bottom = rcEnd.top + 1;

	if(pAnimate)
	{
		pAnimate->setIsAlpha(true);
		pAnimate->ShowWindow(SW_SHOW);
		pAnimate->SetEndRect(rcEnd);
		pAnimate->SetSpeed(80);
		pAnimate->Play();
	}

	return true;
}

//��ճ���UI����ӿ�
bool CMiniGameEngine::OnRemoveCollectJotens()
{
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (m_pJetonAnim[i])
		{
			m_pJetonAnim[i]->ShowWindow(SW_HIDE);
			m_pJetonAnim[i] = NULL;
		}
	}
	return true;
}

//�䷢����UI����ӿ�
bool CMiniGameEngine::OnShowDispatchJotens(BYTE bDeskStation,int iMoney)
{
	BYTE nView = ViewStation(bDeskStation);
	//���Ŷ���
	
	TCHAR szPos[32];
	TCHAR szMoney[16];
	_stprintf(szMoney,"%d",iMoney);

	BzAnimateNode *pAnimate = NULL;
	wsprintf(szPos, "%d;0;%d;%d",nView * 41, 41, 33);

	pAnimate = AddAnimateNodeControl(380,300,41,33,RGB(255,0,255),szPos,
		szMoney,RGB(255,255,0),"resources\\shengji\\shengji\\cm.bmp", this);
	m_pJetonAnim[nView] = pAnimate;

	//��ȡ������Ŀ������

	// 	CString str;
	// 	str.Format("DZPKClient:%d,%d,%d,%d",pNode->startx,pNode->starty,\
	// 	pNode->Width,pNode->Height);
	// 	OutputDebugString(str);

	//��ȡ���ƵĿؼ�����
	Node *pNode = NULL;
	pNode = GetNodeById(IDC_JIN_BI + nView);

	if (NULL == pNode)
	{
		return false;
	}

	CRect rcEnd;
	rcEnd.left = pNode->startx;
	rcEnd.top = pNode->starty;
	rcEnd.right = rcEnd.left + 1;
	rcEnd.bottom = rcEnd.top + 1;

	if(pAnimate)
	{
		pAnimate->setIsAlpha(true);
		pAnimate->ShowWindow(SW_SHOW);
		pAnimate->SetEndRect(rcEnd);
		pAnimate->SetSpeed(80);
		pAnimate->Play();
	}

	if (m_pSEBkMusic)
	{
		m_pSEBkMusic->stopAllSounds();
	}

	return true;
}

//�����Ч���ð�ť��UI�ı仯����
bool CMiniGameEngine::OnShowClickedSoundCfg()
{
	if (m_pSoundCfgDlg)
	{
		return true;
	}

	m_pSoundCfgDlg = new CSoundCfgDlg(2,this);

	if (m_pSoundCfgDlg)
	{
		if (m_pSoundCfgDlg->LoadConfig(IDD_SOUND_DLG))
		{
			m_pSoundCfgDlg->ShowAllControl(SW_SHOW);

			int nRet = m_pSoundCfgDlg->DoModal();

			if (m_pSoundCfgDlg->GetSafeHwnd()!=NULL)
			{
				m_pSoundCfgDlg->EndModalLoop(EVENT_CONTINUE_MAINFRAME);
			}
		}

		delete m_pSoundCfgDlg;
		m_pSoundCfgDlg = NULL;
	}

	return true;
}

//�غϽ�����ʾ��UI������������Ի���
void CMiniGameEngine::OnShowRoundResult(const TResult* pResult)
{
	KillTimer(IDT_TIME_SOUND);

	//����������ж������
	ClearAnimateNodes();

	// ��������
	OnShowDeskTop();

	//�����й������ְ���
	OnShowPdg(SW_HIDE);
	OnAutoTrusteeship(SW_HIDE);

	ShowControl(IDB_BEGIN_GAME, SW_SHOW);
	ShowControl(IDB_ALTER_STAKE, SW_SHOW);

	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_GameMachine.m_tagResult.nMoney[i] >= 0 && m_pUserInfo[i] != NULL && m_GameMachine.m_tagResult.bWin[i])
		{
			BzImage * pImage = dynamic_cast<BzImage *>(GetControlById(IDC_YOU_WIN + ViewStation(i)));
			if (pImage != NULL)
			{
				ShowControl(IDC_YOU_WIN + ViewStation(i), SW_SHOW);
				pImage->Invalidate();
			}
		}
	}

	// ������Ӯ����
	if (m_GameMachine.m_tagResult.nMoney[m_GameMachine.m_byMeStation] >= 0)
	{	
		// Ӯ
		PlayBaseSound(DZPKSound::BT_WIN);
	}
	else
	{
		// ��
		PlayBaseSound(DZPKSound::BT_LOSE);
	}

	CString strMsg, strTemp;

    // ����Ϸ������������������
    for (int i = 0; i < PLAY_COUNT; i++)
    {
        // ֻ��ʾӮ����Ϣ
        if (pResult->nbyUserID[i] > 0)
        {
			strTemp.Format("%d: ", pResult->nbyUserID[i]);
			strMsg = strTemp;

            for (int j = 0; j < 8; j++)
            {
                if (pResult->nUserBet[i][j] > 0)
                {
                    if (pResult->bWinBetPools[i][j])
                    {
                        if (j == 0)
                        {
                            strTemp.Format("����: Ӯ%d", pResult->nWinPoolsMoney[i][j]);
                        }
                        else
                        {
                            strTemp.Format("�߳�%d: Ӯ%d", j, pResult->nWinPoolsMoney[i][j]);
                        }
                    }
                    else
                    {   
                        if (j == 0)
                        {
                            strTemp.Format("����: ��%d", pResult->nUserBet[i][j]);
                        }
                        else
                        {
                            strTemp.Format("�߳�%d: ��%d", j, pResult->nUserBet[i][j]);
                        }
                    }

                    strMsg += strTemp;
                }
            }

            strTemp.Format(" �ܼ���Ӯ: %d ", pResult->nMoney[i]);
			strMsg += strTemp;
            ShowMsgInGame(strMsg.GetBuffer());
        }
    }


	// ���´����Ҽ���ҽ��
	for (int i = 0; i < PLAY_COUNT; i++)
	{
		if (m_pUserInfo[i] != NULL)
		{
			m_iMoneys[i] = pResult->nSubMoney[i];
			
			CString str;
			str.Format("dxh: ���:%d ���´�����:%d",  m_pUserInfo[i]->GameUserInfo.dwUserID, m_iMoneys[i]);
			OutputDebugString(str);

			CTransparentStatic * pStatic = dynamic_cast<CTransparentStatic *>(GetControlById(IDC_MONEY + ViewStation(i)));
			if (pStatic)
			{
				CString strnMoney;
				strnMoney.Format("%d", m_iMoneys[i]);
				pStatic->SetMyText(strnMoney.GetBuffer());
			}
		}
	}

	// ���������ӵ�еĽ��
	CString strMoney;
	CTransparentStatic * pStatic = NULL;
	pStatic = dynamic_cast<CTransparentStatic *>(GetControlById(IDC_REAL_MONEY));
	if (pStatic != NULL)
	{
		strMoney.Format("%d",pResult->nSelfMoney[GetMeUserInfo()->bDeskStation]);
		pStatic->SetMyText(strMoney.GetBuffer());
	}

	BzImage * pImage = dynamic_cast<BzImage *>(GetControlById(IDC_REAL_DINAR));
	if (pImage != NULL)
	{
		strMoney.Format("%d", pResult->nSelfMoney[GetMeUserInfo()->bDeskStation]);
		pImage->SetMyText(strMoney.GetBuffer());
	}

	// ���������
	if (m_pFinishDlg)
	{
		return;
	}

	m_pFinishDlg = new CFinishDlg(1,this);

	if (m_pFinishDlg)
	{
		SetTimer(IDT_OFF_BALANCE ,3000, 0);
		if (m_pFinishDlg->LoadConfig(CN_FINISH_DLG))
		{
			m_pFinishDlg->SetMessage(*pResult);
			//m_pFinishDlg->Create(IDD_FINISH_DLG, this);
			//m_pFinishDlg->ShowWindow(SW_SHOW);
			m_pFinishDlg->DoModal();
			delete m_pFinishDlg;
			m_pFinishDlg = NULL;
		}

		KillTimer(IDT_OFF_BALANCE);

		//if (!m_bInLisureRoom)
		//{
		//	// �����г�����ʾ׼����ť
		//	ShowControl(IDB_BEGIN_GAME, SW_HIDE);
		//}
		//else
		//{
		//	ShowControl(IDB_BEGIN_GAME, SW_SHOW);
		//}
		//
		//// �޸Ľ��
		//ShowControl(IDB_ALTER_STAKE, SW_SHOW);

		// �����С����ʹ�����, ���������ҶԻ���
		SetTimer(IDT_AUTO_READY, 1,0);

		// ���������ʾ����
		if (m_iMoneys[m_GameMachine.m_byMeStation] == m_GameMachine.m_tagDeskCfg.dz.iUntenMoney)
		{
			ShowControl(IDC_PROMPT, SW_SHOW);
		} 
		else
		{
			ShowControl(IDC_PROMPT, SW_HIDE);
		}
		
		// ������������ʹ��뽫����߳�
		if (m_pUserInfo[m_GameMachine.m_byMeStation] != NULL)
		{
			if (pResult->nSelfMoney[m_GameMachine.m_byMeStation] < m_GameMachine.m_tagDeskCfg.dz.iSubMinMoney)
			{
				ShowControl(IDC_MONEYLACK, SW_SHOW);
				SetTimer(IDT_MONEY_LACK, 5000, 0);
			}
		}

		//���������ҵ��ƿؼ�
		OnClearCard();
	}

	PlayBaseSound(DZPKSound::BT_GAME_MUSIC1,true);

	return;
}

//��һ�غ�׼�������������ȴ���
void CMiniGameEngine::OnShowNextRoundReady()
{
	HideAllNoticeControl();

	//����һЩ��ť

	//������ж�ʱ��
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		StopAndHideFlashTimer(i);
	}


	//������ϵ���
	/*ClearHandCardTo(MY_VIEWSTATION);*/

	//����������еĳ���
	/*for (int i = 0; i < PLAY_COUNT; ++i)
	{
		ClearOutCardTo(i);
	}*/

	return;
}

//�»غϿ�ʼ��UI����
void CMiniGameEngine::OnShowNewRoundBegin(const TNextRoundBeginInfo* pNextRoundBeginInfo)
{
	ShowControl(IDPIC_DLG_BG, SW_HIDE);
	
	// �޸�ע���
	//OnLoadSelectionDlg();

	// ��ʾ��ҵ���
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		CUpGradeGameCard* pGameCard = dynamic_cast<CUpGradeGameCard*>(GetControlById(IDC_SHOW_CARD + i));
		if (pGameCard)
		{
			pGameCard->SetCard(NULL,NULL,0,0);
		}
	}

	for (BYTE i = 0; i < PLAY_COUNT; ++i)
	{
		//��������ϵ����˶��ؼ�
		m_pBackCardAnim[i] = NULL;
		m_pBackCardAnim[PLAY_COUNT + i] = NULL;
		if (m_pJetonAnim[i])
		{
			m_pJetonAnim[i]->ShowWindow(SW_HIDE);
			m_pJetonAnim[i] = NULL;
		}
	}

	//����־���˶��ؼ�
	m_pFlagAnim[0] = NULL;
	m_pFlagAnim[1] = NULL;
	m_pFlagAnim[2] = NULL;

	// ��ʾׯ�ҡ�Сäע����äע
	if (pNextRoundBeginInfo->byNTUser == pNextRoundBeginInfo->bSmallBlind)
	{
		ShowControl(IDC_SMALL_BLIND,SW_HIDE);
		OnPlayMoveFlagAnim(pNextRoundBeginInfo->byNTUser,IDC_ZHUANG,true);
		//OnPlayMoveFlagAnim(pNextRoundBeginInfo->bSmallBlind,IDC_SMALL_BLIND,true);
		OnPlayMoveFlagAnim(pNextRoundBeginInfo->bBigBlind,IDC_BIG_BLIND,true);
	}
	else
	{
		OnPlayMoveFlagAnim(pNextRoundBeginInfo->byNTUser,IDC_ZHUANG,true);
		OnPlayMoveFlagAnim(pNextRoundBeginInfo->bSmallBlind,IDC_SMALL_BLIND,true);
		OnPlayMoveFlagAnim(pNextRoundBeginInfo->bBigBlind,IDC_BIG_BLIND,true);
	}

	return;
}

//��2�ŵ��Ƶ�UI����
void CMiniGameEngine::OnShowACard(const TCards * pCards)
{
	// ��ʾ���ŵ���
	if (pCards->nTypeCard == SEND_A_CAND)	//����
	{
		//���±�������			
		m_iUser = pCards->byUser;

		if (MY_VIEWSTATION == ViewStation(m_iUser))
		{
			::memcpy(m_iUserDataCard, pCards->byCards,MAX_HANDCARDS_NUM);
			m_iCardCounts = pCards->iCardsNum;
		}

		ShowControl(IDC_SHOW_CARD + ViewStation(m_iUser),SW_SHOW);
	}

	return;
}

//��1���ƵĶ�������
void CMiniGameEngine::OnPlayACardAnimate(BYTE bDeskStation,int nIndex)
{
	BYTE byView = ViewStation(bDeskStation);

	//��ȡ���ƵĿؼ�����
	Node *pNode = NULL;
	pNode = GetNodeById(IDC_VICE_CARD);
	if (NULL == pNode)
	{
		return;
	}

	char szPos[32];

	BzAnimateNode *pNoteAnimate = NULL;
	wsprintf(szPos, "%d;0;%d;%d",byView*44,44,42);

 	if (nIndex == 0)
 	{
 		pNoteAnimate = AddAnimateNodeControl(pNode->startx,pNode->starty,44,42,RGB(255,0,255),szPos,
 			"",RGB(255,255,255),"resources\\shengji\\shengji\\gaip01.bmp",this,IDC_SENDCARD_ANIM + byView);
 		m_pBackCardAnim[byView] = pNoteAnimate;
 	}
 	else if (nIndex == 1)
	{
		pNoteAnimate = AddAnimateNodeControl(pNode->startx,pNode->starty,44,42,RGB(255,0,255),szPos,
			"",RGB(255,255,255),"resources\\shengji\\shengji\\gaip02.bmp",this,IDC_SENDCARD_ANIM + PLAY_COUNT + byView);
		m_pBackCardAnim[byView + PLAY_COUNT] = pNoteAnimate;
	}


	//��ȡ������Ŀ������
	pNode = GetNodeById(IDPIC_BACK_CARD_BASE + byView);
	if (NULL == pNode)
	{
		return;
	}

// 	CString str;
// 	str.Format("DZPKClient:%d,%d,%d,%d",pNode->startx,pNode->starty,\
// 	pNode->Width,pNode->Height);
// 	OutputDebugString(str);

	CRect rcEnd;
	rcEnd.left = pNode->startx;
	rcEnd.top = pNode->starty;
	rcEnd.right = rcEnd.left + 1;
	rcEnd.bottom = rcEnd.top + 1;

	OutputDebugString("DZPKClient:OnShowACard");
	if(pNoteAnimate)
	{
		OutputDebugString("DZPKClient:pNoteAnimate");
		pNoteAnimate->setIsAlpha(true);
		pNoteAnimate->ShowWindow(SW_SHOW);
		pNoteAnimate->SetEndRect(rcEnd);
		pNoteAnimate->SetSpeed(100);
		pNoteAnimate->Play();
	}
	return;
}
//��1���ƣ����˶������󣬵�UI����
void CMiniGameEngine::OnEndSendMeACard(int nIndex)
{
//	BYTE nView = ViewStation(m_GameMachine.m_byMeStation);

	//����������ؼ�
// 	BzAnimateNode* pMeCardAnim = NULL;
// 	if (nIndex == 0)
// 	{
// 		pMeCardAnim = m_pBackCardAnim[nView];
// 		m_pBackCardAnim[nView] = NULL;
// 	}
// 	else if (nIndex == 1)
// 	{
// 		pMeCardAnim = m_pBackCardAnim[nView + PLAY_COUNT];
// 		m_pBackCardAnim[nView + PLAY_COUNT] = NULL;
// 	}
// 
// 	if (pMeCardAnim)
// 	{
// 		pMeCardAnim->Stop();
// 		pMeCardAnim->ShowWindow(SW_HIDE);
// 	}
	
	//���µ�����ʾ
	CUpGradeGameCard* pCard = dynamic_cast<CUpGradeGameCard*>(GetControlById(IDC_SHOW_CARD + MY_VIEWSTATION));
	if (pCard)
	{
		pCard->SetCard(m_iUserDataCard,NULL,nIndex + 1,0);
	}
	return;
}

//��ʾ�߳����ݴ���
void CMiniGameEngine::OnShowPot(const TBetPool* pBetPool)
{
	for (int i = 0; i < 8; i++)
	{
		if (pBetPool->iBetPools[i] > 0)
		{
			ShowControl(IDC_SIDE_POT + i, SW_SHOW);

			CTransparentStatic * pStatic = dynamic_cast<CTransparentStatic *>(GetControlById(IDC_POT_DINAR + i));

			if (pStatic != NULL)
			{
				CString str;
				if (i == 0)
				{
					str.Format("����:%d", pBetPool->iBetPools[i]);
				}
				else
				{
					str.Format("�߳�%d:%d", i, pBetPool->iBetPools[i]);
				}
				pStatic->SetMyText(str.GetBuffer());
				pStatic->ShowWindow(SW_SHOW);
			}
		}
	}
	return;
}

//��ʾ����������ݴ���
void CMiniGameEngine::OnShowPots(const TBetPool* pBetPool)
{
	for (int i = 0; i < 8; i++)
	{
		if (pBetPool->iBetPools[i] > 0)
		{
			ShowControl(IDC_BALANCE_POT + i, SW_SHOW);

			CTransparentStatic * pStatic = dynamic_cast<CTransparentStatic *>(GetControlById(IDC_BALANCE_POT_MONEY + i));

			if (pStatic != NULL)
			{
				CString str;
				if (i == 0)
				{
					str.Format("����:%d", pBetPool->iBetPools[i]);
				}
				else
				{
					str.Format("�߳�%d:%d", i, pBetPool->iBetPools[i]);
				}
				pStatic->SetMyText(str.GetBuffer());
				pStatic->ShowWindow(SW_SHOW);
			}
		}
	}
	return;
}

//��3�Ź������Ƶ�UI����
void CMiniGameEngine::OnShow3Card(const TCards * pCards)
{
	// ��ʾ3�Ź�����
	CUpGradeGameCard* pGameCrad = dynamic_cast<CUpGradeGameCard*>(GetControlById(IDC_SHOW_BASE_CARD));
	if (pGameCrad)
	{
		memcpy(m_iCardData, pCards->byCards, sizeof(BYTE) * (pCards->iCardsNum));
		m_iCardDataCounts += pCards->iCardsNum;
		pGameCrad->SetCard((BYTE *)pCards->byCards, NULL, pCards->iCardsNum, NULL);
	}
	ShowControl(IDC_SHOW_BASE_CARD, SW_SHOW);
	CString str;
	str.Format("dxy: byCards = %d, iCardsNum = %d", pCards->byCards, pCards->iCardsNum);
	OutputDebugString(str);

	return;
}

//��1�Ź������Ƶ�UI����
void CMiniGameEngine::OnShow1Card(const TCards * pCards)
{
	// ��ʾ4 - 5 ������
	CUpGradeGameCard* pGameCrad = dynamic_cast<CUpGradeGameCard*>(GetControlById(IDC_SHOW_BASE_CARD));
	if (pGameCrad)
	{
		memcpy(m_iCardData, pCards->byCards, sizeof(BYTE) * (pCards->iCardsNum));
		m_iCardDataCounts = pCards->iCardsNum;

		pGameCrad->SetCard((BYTE *)pCards->byCards, NULL, pCards->iCardsNum, NULL);
	}
	ShowControl(IDC_SHOW_BASE_CARD, SW_SHOW);

	CString str;
	str.Format("dxy: byCards = %d, iCardsNum = %d", pCards->byCards, pCards->iCardsNum);
	OutputDebugString(str);

	return;
}

//û������ڳ���UI����������ע�����ʾ
void CMiniGameEngine::OnShowNoPlayer(const TNoPlayer* pNoPlayer,bool bHaveResultDlg)
{
	HideAllNoticeControl();

	if (!bHaveResultDlg)
	{
		ShowControl(IDPIC_DLG_BG,SW_SHOW);
		BzImage* pImage = dynamic_cast<BzImage*>(GetControlById(IDPIC_DLG_BG));
		if (pImage)
		{
			pImage->SetImage(_TEXT("resources\\shengji\\shengji\\notices\\no_player.png"));
		}
	}
	else
	{
		if (m_pFinishDlg)
		{
			m_pFinishDlg->SetNotice(_TEXT("û����Ҽ�����Ϸ����ѡ���뿪��"),false);
			m_pFinishDlg->SetGameFinish(pNoPlayer->bGameFinished);
		}
	}

	return;
}

//���ͬͬ�⣬��ͬ����Ϸ��UI����
void CMiniGameEngine::OnShowAgree(BYTE bDeskStation)
{
	int nView = ViewStation(bDeskStation);

	if (nView == MY_VIEWSTATION)
	{
		HideAllNoticeControl();
	}

	//OnShowTips(bDeskStation,TT_READY);
	return;
}

//�������ͬ����Ϸ��UI����
void CMiniGameEngine::OnShowAllAgree()
{
	//HideAllNoticeControl();

	/*for (int i = 0; i < PLAY_COUNT; ++i)
	{
		OnShowTips(i,TT_NONE);
	}*/

	return;
}

//��Ҳ�ͬ����Ϸ��UI����
void CMiniGameEngine::OnShowDisAgree(BYTE bDeskStation)
{
	int nView = ViewStation(bDeskStation);

	ShowControl(IDC_USER_LOGO + nView,SW_HIDE);				// ���Logo
	ShowControl(IDC_UDER_ID + nView,SW_HIDE);				// ���ID
	ShowControl(IDPIC_PLAYER_BG + nView,SW_HIDE);			// ��ҵ�ͼ
	ShowControl(IDT_PLAYER_MONEY_BASE + nView,SW_HIDE);		// ��ҵ���
	ShowControl(IDC_OUTCARD_BASE + nView,SW_HIDE);			// ������ƿؼ�
	ShowControl(IDPIC_COINT_BASE + nView,SW_HIDE);			// ���ͼ��
	ShowControl(IDC_CLOCK_BASE + nView,SW_HIDE);			// ʱ��
	ShowControl(IDC_CARD_POINT_VALUE_BASE + nView,SW_HIDE); // �������
	ShowControl(IDT_CARD_POINT_VALUE_BASE + nView,SW_HIDE); // �������`

	return;
}

//�������Ǯ������UI����
void CMiniGameEngine::OnShowNotEnoughMoney(const TMoneyLack * pMoneyLack)
{
	int nUser = nUser = ViewStation(pMoneyLack->nbyUser);	// ��ȡ���λ��
	ShowControl(IDC_USER_LOGO			+ nUser, SW_HIDE);	// �������ͷ��LOGO
	ShowControl(IDC_UDER_ID				+ nUser, SW_HIDE);	// �������ID
	ShowControl(IDC_READY				+ nUser, SW_HIDE);	// ����׼��ͼƬ
	ShowControl(IDC_JIN_BI				+ nUser, SW_HIDE);	// ������ҽ��ͼƬ
	ShowControl(IDC_MONEY				+ nUser, SW_HIDE);	// ������ҽ����
	ShowControl(IDC_USER_CHAIR			+ nUser, SW_SHOW);	// ��ʾ�������
	ShowControl(IDC_BET					+ nUser, SW_HIDE);	// ���������עͼƬ
	ShowControl(IDC_CALL				+ nUser, SW_HIDE);	// ������Ҹ�עͼƬ
	ShowControl(IDC_FOLD				+ nUser, SW_HIDE);	// �����������ͼƬ
	ShowControl(IDC_CHECK				+ nUser, SW_HIDE);	// ������ҹ���ͼƬ
	ShowControl(IDC_RAISE				+ nUser, SW_HIDE);	// ������Ҽ�עͼƬ
	ShowControl(IDC_ALLIN				+ nUser, SW_HIDE);	// �������ȫ��ͼƬ
	ShowControl(IDPIC_BACK_CARD_BASE	+ nUser, SW_HIDE);	// ���ص���ͼƬ

	// ����ƽ̨�������û��뿪����������û���Ϣ��Ϊ�˱���Ӱ����Ϸ���ڴ�����Ϊ��  duanxiaohui
	//m_pUserInfo[pMoneyLack->nbyUser] = NULL;

	return;
}

//�ȴ����õ�עUI����
void CMiniGameEngine::OnTimeoutCfgDesk()
{
	return;
}

//�ȴ����õ�עUI����
void CMiniGameEngine::OnTimeoutAgreeDeskCfg()
{
	return;
}

//�ȴ���һ�غϵĳ�ʱ���UI����
void CMiniGameEngine::OnTimeoutNextRoundBegin()
{
	if (IsResultDlgShowing()) //����Ի�����
	{
		//ֱ���йܰ�ȷ����ʼ��һ��
		BBzParameter bbp;
		m_pFinishDlg->OnPostMessage(EVENT_CONTINUE_MAINFRAME,&bbp);

		if (m_pFinishDlg->GetSafeHwnd()!=NULL)
		{
			m_pFinishDlg->EndModalLoop(EVENT_CONTINUE_MAINFRAME);
		}

	}
	return;
}

//���Ƴ�ʱ���UI����
void CMiniGameEngine::OnTimeoutOutCard()
{
	return;
}


/*-----------------------------------------------------------------------------------------------------------------
	����ͨ�ý�������������壬�����ڲ�ʹ�ã�ÿ����Ϸ����ͬ
  ----------------------------------------------------------------------------------------------------------------*/


//��ʼ���ӿؼ�
void CMiniGameEngine::OnInitClocks()
{
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		m_pClocks[i] = dynamic_cast<BzNumberPanel*>(GetControlById(IDC_CLOCK_BASE + i));
		//��ʼ�����㶯��
		m_uTimerFlashID[i] = 0;
	}
}

//��ʼ�����ʾ�ؼ�
void CMiniGameEngine::OnInitMoneyStatic()
{
	return;
}


//��ʼ�����ؼ�
void CMiniGameEngine::OnInitHanHuaControl()
{
	CComboBox* pComboBox = dynamic_cast<CComboBox*>(GetControlById(DDZ_LIAOTIAN_LIEBIAO)); 

	std::list<CString*> lstCanTalk;
	std::list<CString*>::iterator itCanTalk;
	if (pComboBox)
	{
		char strGet[MAX_PATH];
		int iCount = pComboBox->GetCount();

		for (int i = 0; i<iCount; i++)
		{	
			memset(strGet, 0, sizeof(strGet));

			pComboBox->SetCurSel(i);
			pComboBox->GetWindowText(strGet,MAX_PATH);

			if (GetMeUserInfo()->bBoy)
			{
				if (strGet[0] == '0' || strGet[0] == '1')
				{
					CString* pStr = new CString;
					pStr->Format("%s",strGet+1);
					lstCanTalk.push_back(pStr);
				}
			}
			else
			{
				if (strGet[0] == '0' || strGet[0] == '2')
				{
					CString* pStr = new CString;
					pStr->Format("%s",strGet+1);
					lstCanTalk.push_back(pStr);
				}
			}
		}

		pComboBox->ResetContent();

		itCanTalk = lstCanTalk.begin();
		while (itCanTalk != lstCanTalk.end())
		{
			CString* pStr = (CString*)*itCanTalk;
			pComboBox->AddString(pStr->GetBuffer(MAX_PATH));
			delete pStr;
			itCanTalk++;
		}
	}

	return;
}

//��ʼ�����ƿؼ�
void CMiniGameEngine::OnInitCardsControl()
{
	for (BYTE i = 0; i < PLAY_COUNT; ++i)
	{
		m_pOutCards[i] = dynamic_cast<CUpGradeGameCard*>(GetControlById(IDC_OUTCARD_BASE + i));
		ClearOutCardTo(i);

		//��������ϵ����˶��ؼ�
		m_pBackCardAnim[i] = NULL;
		m_pBackCardAnim[PLAY_COUNT + i] = NULL;
		m_pJetonAnim[i] = NULL;
	}

	m_pMyHandCards = dynamic_cast<CUpGradeGameCard*>(GetControlById(IDC_HANDCARD_BASE + MY_VIEWSTATION));
	ClearHandCardTo(MY_VIEWSTATION);
}


//����������ʾ���͵�UI�ؼ�
void CMiniGameEngine::HideAllNoticeControl()
{
	ShowControl(IDPIC_WAIT_AGREE_DIZHU1,SW_HIDE);
	ShowControl(IDPIC_WAIT_AGREE_DIZHU2,SW_HIDE);
	ShowControl(IDPIC_WAIT_DIZHU,SW_HIDE);
	ShowControl(IDPIC_DLG_BG,SW_HIDE);
	ShowControl(IDB_AGREE_DESK_CFG,SW_HIDE);
	ShowControl(IDB_DISAGREE_DESK_CFG,SW_HIDE);

	for (int i = 0; i < 5; ++i)
	{
		ShowControl(IDB_CFG_DESK_BASE + i,SW_HIDE);
	}

	return;
}

// ��ʾ���Ƹ�ĳ��ҵĶ���
void CMiniGameEngine::ShowCardFlash(BYTE bDeskStation,BYTE byCard,int nIndex)
{
	// �ѷ���״̬�ŵ�������
	tagSendCardFlash scf;
	scf.byUser = bDeskStation;
	scf.byCard = byCard;
	scf.nIndex = nIndex;
	m_lstSendCardFlash.push_back(scf);

// 	if (m_uSendCardFlashID > 0)
// 	{
// 		return;
// 	}

	ShowCardFlashAction();
}

// ������ʼ���ƶ���
void CMiniGameEngine::ShowCardFlashAction(void)
{
	std::list<tagSendCardFlash>::iterator it = m_lstSendCardFlash.begin();

	if (it == m_lstSendCardFlash.end())
	{
// 		 m_pFlashManager->DeleteFlashWidthID(m_uSendCardFlashID);
// 		m_uSendCardFlashID = 0;
		return;
	}

	//������ʼλ��
	int nStartX = 460;
	int nStartY = 280;
	int nEndX = 0;
	int nEndY = 0;

	//��ȡ�����յ�λ��
	Node *pNode = GetNodeById(IDC_OUTCARD_BASE + ViewStation(it->byUser));
	if (NULL == pNode)
	{
		return;
	}
	nEndX = pNode->startx;
	nEndY = pNode->starty;

	CUpGradeGameCard *pCard = dynamic_cast<CUpGradeGameCard*>(pNode->n_Wnd);
	if (NULL == pCard)
	{
		return;
	}
	nEndY += pCard->GetCardUpSpace();
	nEndX += pCard->GetCardDisplayPositionX();
	nEndX += pCard->GetCardHSpace() * (it->nIndex);

	//������������������붯��
	char szCommand[64];
	BYTE bCard = it->byCard;
	wsprintf(szCommand,"FapaiAnima|res\\danfapai.xml|%d,%d,%d,%d,%d,5,8|1",GetFlashCardValue(bCard),
		nStartX,nStartY - 30,nEndX,nEndY - 30);

	//������Ч����
	//PlayBaseSound(DZPKSound::BT_SENDACARD);


// 	if (m_uSendCardFlashID <= 0)
// 	{
// 		m_uSendCardFlashID = m_pFlashManager->CreateAndPlayFlash(this, m_uSendCardFlashID, CPoint(0,30),"shAnima\\shCfg.xml",1,szCommand);
// 	}
// 	else
// 	{
// 		m_pFlashManager->SendMsgToFlash(m_uSendCardFlashID,szCommand);
// 	}
}

//��ʾ�շ������ƣ��������δ��ʾ�������ƣ��򷵻ط�0���޴�����������0
int CMiniGameEngine::ShowNextCardFlashAction(void)
{
	std::list<tagSendCardFlash>::iterator it = m_lstSendCardFlash.begin();
	if (it == m_lstSendCardFlash.end())
	{
		return 0;
	}

	CUserDataCliObject* pUserData = &m_GameMachine.m_UserData[it->byUser];

	SetOutCardsTo(it->byUser,pUserData->m_byMyCards,NULL,it->nIndex + 1,0);

	m_lstSendCardFlash.pop_front();
	ShowCardFlashAction();

	return 1;
}

// ����������ҵ���
void CMiniGameEngine::SetOutCardsTo(BYTE bDeskStation,BYTE *pCard,BYTE *pUpList,int nCount,int nUpCount)
{
	Node *pNode = NULL;
	pNode = GetNodeById(IDC_OUTCARD_BASE + ViewStation(bDeskStation));
	if (NULL == pNode)
	{
		return;
	}

	//��ȡ���ƵĿؼ�����
	RECT rcHandCard;
	rcHandCard.left = pNode->startx;
	rcHandCard.top  = pNode->starty;
	rcHandCard.right  = rcHandCard.left + pNode->Width;
	rcHandCard.bottom = rcHandCard.top + pNode->Height;

	CUpGradeGameCard *pCardControl = NULL;
	pCardControl = dynamic_cast<CUpGradeGameCard*>(pNode->n_Wnd);
	if (NULL != pCardControl)
	{
		pCardControl->SetCard(pCard,pUpList,nCount,nUpCount);
		pCardControl->Invalidate();
		pCardControl->ShowWindow(SW_SHOW);
	}
	RedrawWindow(&rcHandCard);

	return;
}

//�õ���������˿���
void CMiniGameEngine::GetHandCards(BYTE bDeskStation,BYTE* pCard,BYTE* pUpList)
{
	BYTE byView = ViewStation(bDeskStation);
	if (NULL != m_pOutCards[byView])
	{
		m_pOutCards[byView]->GetCard(pCard,pUpList);
	}

	return;
}

// ��ʾ������ϵ��˿���
void CMiniGameEngine::ShowHandCards(BYTE bDeskStation, bool bShow)
{
	Node *pNode = NULL;
	pNode = GetNodeById(IDC_OUTCARD_BASE + ViewStation(bDeskStation));
	if (NULL == pNode)
	{
		return;
	}

	//��ȡ���ƵĿؼ�����
	RECT rcHandCard;
	rcHandCard.left = pNode->startx;
	rcHandCard.top  = pNode->starty;
	rcHandCard.right  = rcHandCard.left + pNode->Width;
	rcHandCard.bottom = rcHandCard.top + pNode->Height;

	CUpGradeGameCard *pCardControl = NULL;
	pCardControl = dynamic_cast<CUpGradeGameCard*>(pNode->n_Wnd);
	if (NULL != pCardControl)
	{
		pCardControl->SetShowCard(bShow);
		pCardControl->ShowWindow(SW_SHOW);
	}
	RedrawWindow(&rcHandCard);

	return;
}

// ���ĳ����������ѳ�����
void CMiniGameEngine::ClearOutCardTo(BYTE bDeskStation)
{
	int nView = ViewStation(bDeskStation);
	Node* pNode = GetNodeById(IDC_OUTCARD_BASE + nView);
	if (pNode)
	{
		RECT rc;
		rc.left = pNode->startx;
		rc.top = pNode->starty;
		rc.right = rc.left + pNode->Width;
		rc.bottom= rc.top + pNode->Height;

		if (m_pOutCards[nView])
		{
			m_pOutCards[nView]->SetCard(NULL,NULL,0,0);
		}

		RedrawWindow(&rc);
	}

	return;
}

//���ĳ������ϵ���
void CMiniGameEngine::ClearHandCardTo(BYTE bDeskStation)
{
	Node* pNode = GetNodeById(IDC_HANDCARD_BASE + MY_VIEWSTATION);
	if (pNode)
	{
		RECT rc;
		rc.left = pNode->startx;
		rc.top = pNode->starty;
		rc.right = rc.left + pNode->Width;
		rc.bottom= rc.top + pNode->Height;
		
		if (m_pMyHandCards)
		{
			m_pMyHandCards->SetCard(NULL,NULL,0,0);
		}

		RedrawWindow(&rc);
	}
	return;
}


//ȡ���붯��λ��
//@param ����������bDeskStation����λ�ã�ptStartĳ���붯����ʼλ�ã�ptEndĳ�붯������λ��
//@return
bool CMiniGameEngine::GetJetonAnimRect(BYTE bDeskStation,CRect &rcStart,CRect &rcEnd)
{

	//�õ��������
	Node *pArea = this->GetNodeById(IDC_JETON_PRE_BASE + ViewStation(bDeskStation));

	if (NULL == pArea)
	{
		return false;
	}


	rcStart.left = pArea->startx;
	rcStart.top = pArea->starty;
	rcStart.right = rcStart.left + 200;
	rcStart.bottom = rcStart.top + 200;

	//�õ������յ�
	pArea = this->GetNodeById(IDC_JETON_BASE + ViewStation(bDeskStation));

	if (NULL == pArea)
	{
		return false;
	}

	rcEnd.left = pArea->startx;
	rcEnd.top = pArea->starty;
	rcEnd.right = rcEnd.left + 200;
	rcEnd.bottom = rcEnd.top + 200;

	return true;
}